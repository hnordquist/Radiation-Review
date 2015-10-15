///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbEvent.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

//19 Apr 2006 - pjm - changed code to reduce compiler warnings.

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		FindChanEventRec
//		ReadEventRec
//		CreateChanEventRec
//		UpdateChanEvent
//		ReplaceChanEvent
//		GetDataStatisticsForEventsByChannel - Replaces NumChanEventsInDbInterval
//		GetDataStatisticsForEventsByStation - Replaces NumChanEventsInDbInterval
//		GetDataByChannelEvents - Replaces ReadChanEventsInDbInterval
//		DeleteDataByChannelEvents - Replaces DeleteDbChanEvents
//		DeleteDataByChannelEvents - Replaces DeleteDbChanEvent
//		ScanChannelEvents - Replaces ScanChanEventRecs
//		GroupChanEvents
//		FindDirEventRec
//		CreateDirEventRec
//		UpdateDirEvent
//		ReadDirEvent
//		GetDataStatisticsForDirEventsByChannel - Replaces NumDirEventsInDbInterval
//		GetDataByChannelDirEvents - Replaces ReadDirEventsInDbInterval
//		DeleteDbDirEvents
//		ScanDirEventRecs
//		FindDirEventAssociatedWithChanEvent
//		
//		All times are adjusted times.
//
///////////////////////////////////////////////////////////////////////////
#include "RAD.H"
#include "AdjustTime.h"
#include "DbDay.h"
#include "DbEvent.h"
#include "DbInterval.h"
#include "DbReadStruct.h"
#include "FacilityConfig.h"
#include "MyDateTime.H"
#include "MainDbCfg.h"
#include "TextFile.h"
#include "TypeDefinitions.h"
#include "ComDataStructures.h"
#include <math.h>

//static int FindChanEventRec(struct db_event_rec *pdbChanEvent);
//static int UpdateChanEvent(struct db_event_rec *pdbChanEvent);

// Needed for special case when need to immediately notify another
// part of the program that an event has been added/changed.
static void (*gll_pUpdateFnc)() = NULL;

extern short glsFacNum;
extern TextFile *pDebugFile;
extern CMyDateTime glMyDateTime;
extern CDbDay glDbDay;

void RegisterTableUpdateFunction(void (*pUpdateFnc)(void))
{
	gll_pUpdateFnc = pUpdateFnc;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	FindChanEventRec
//
//	Description:
//	Determine if the db_event_rec exists in the database.  A matching event
//	is one that has the same beginning, ending and channel number -- no other
//	fields in the record are examined.
//	Note : this routine assumes any time adjust is done before data are handed to it.  It
//	"speaks" only in terms of adjusted times.
//
//	Declaration:
//	static int FindChanEventRec(struct db_event_rec *pdbChanEvent)
//
//	Input:	pdbChanEvent	structure containing event to search for
//			
//	Output: none
//
//	Return: TRUE - event already exists
//		    uiEVENT_NOT_IN_DB_INFO - event does not exist
//			uiDB_BAD_ERR - unexpected dbVista error
//	
//  date    /	author	revision
//  -----------------	--------
//  20-Jul-1995 SFK		Created
//  22-Oct-1996 SFK		Changed to do search using keys
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
//////////////////////////////////////////////////////////////////
static int FindChanEventRec(struct db_event_rec *pdbChanEvent)
{
    
    struct db_event_rec dbChanEvent1;
    int db_stat;
	double dEventBegTimeKey = pdbChanEvent->d_event_beg_time_key;
    
   /* ------------------------------------------------------------------
    *	Look for a matching key of the time.  If no match, then return.
	*   Problems if Rad has calculated the key from a string.  In some
	*	cases round-off produces a key slightly beyond the key we are looking for.
    * ----------------------------------------------------------------*/
    db_stat = d_keyfind(D_EVENT_BEG_TIME_KEY, &(dEventBegTimeKey), CURR_DB);
	do {	// added do loop 10-01-2003 sfk
   		if (db_stat != S_OKAY) db_stat = d_keynext(D_EVENT_BEG_TIME_KEY, CURR_DB);	// lock onto a key
   		if (db_stat == S_NOTFOUND) return(uiEVENT_NOT_IN_DB_INFO);
		if (db_stat != S_OKAY) goto db_exit;

		// read the record and verify that the beginning times match.
		if (d_recread(&dbChanEvent1, CURR_DB) != S_OKAY) goto db_exit;
	} while (dbChanEvent1.d_event_beg_time_key < dEventBegTimeKey);
	if (dEventBegTimeKey != dbChanEvent1.d_event_beg_time_key) return(uiEVENT_NOT_IN_DB_INFO);

   /* ------------------------------------------------------------------
    *	Matched, now check if chan and endtime match.  If not, then
    *	search through all keys that match the beginning time.
    * ----------------------------------------------------------------*/
	db_stat = d_recread(&dbChanEvent1, CURR_DB);	//found matching event time    
	while ((db_stat == S_OKAY) && 
		//(dbChanEvent1.us_event_sta == pdbChanEvent->us_event_sta) &&
		(dbChanEvent1.d_event_beg_time_key == dEventBegTimeKey)) {
		if ((pdbChanEvent->us_event_sta == dbChanEvent1.us_event_sta) &&	// 18-May-2005 Add condition of station match, too
			(pdbChanEvent->us_event_chan == dbChanEvent1.us_event_chan) &&
			(pdbChanEvent->d_event_end_time == dbChanEvent1.d_event_end_time)) return(TRUE);
		db_stat = d_keynext(D_EVENT_BEG_TIME_KEY, CURR_DB);					// advance to next event
		if (db_stat == S_OKAY) db_stat = d_recread(&dbChanEvent1, CURR_DB);	// and read the next event   
	 }
	if ((db_stat == S_NOTFOUND) || (db_stat == S_OKAY)) return (uiEVENT_NOT_IN_DB_INFO);

db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "FindChanEventRec", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}       


///////////////////////////////////////////////////////////////////////////
//	Name:	ReadEventRec
//
//	Description:
//	Find the event record specified by the start/end time and channel number
//	in the calling event record.  Read the entire record into another structure.
//	Note : this routine assumes any time adjust is done before data are handed to it.  It
//	"speaks" only in terms of adjusted times.
//
//	Declaration:
//	int ReadEventRec(struct db_event_rec *pdbChanEvent, struct db_event_rec *pdbChanEvent1)
//
//	Input:	pdbChanEvent	structure containing event to search for
//			
//	Output: pdbChanEvent1	sturcutre containing the event read from the database
//
//	Return: TRUE - event already exists
//		    uiEVENT_NOT_IN_DB - event does not exist
//			uiDB_BAD_ERR - unexpected dbVista error
//	
//  date    /	author	revision
//  -----------------	--------
//  01-Apr-1996 SFK		Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
//////////////////////////////////////////////////////////////////
int ReadEventRec(struct db_event_rec *pdbChanEvent, struct db_event_rec *pdbChanEvent1)       
{
    int status, db_stat;
        
   /* ------------------------------------------------------------------
    *	Find the event record. If event	does not exist, cannot do anything.
    * ----------------------------------------------------------------*/
    status = FindChanEventRec(pdbChanEvent);
    if (status != 1) 
		return(status);
       
   /* ------------------------------------------------------------------
    *	Read the record.
    * ----------------------------------------------------------------*/
	db_stat = d_recread(pdbChanEvent1, CURR_DB);
    if (db_stat == S_OKAY) return(TRUE);
    
    RadReviewMsg(uiDB_BAD_ERR, "ReadEventRec", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}                                         


///////////////////////////////////////////////////////////////////////////
//	Name:	CreateChanEventRec
//
//	Description:
//	If an event with the specified start/end and channel number does not
//	already exist in the database, create a new event record and insert
//	in the db.
//	Note : this routine assumes any time adjust is done before data are handed to it.  It
//	"speaks" only in terms of adjusted times.
//
//	Declaration:
//	int CreateChanEventRec(struct db_event_rec *pdbChanEvent)
//
//	Input:	pdbChanEvent	structure containing event to write to the database
//			
//	Output: none
//
//	Return: TRUE - event already exists
//			Error from FindChanEventRec, GetStationOfChan, FindDayRecInMainDb
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  01-Apr-1996 SFK		Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
//	16-Jun-2003	SFK		Added to create day if it doesn't already exist in db (most
//						raw data now stored in COMs)
//	29-Sep-2003	SFK		Added StructID to call.
//////////////////////////////////////////////////////////////////
int CreateChanEventRec(struct IDStruct ChanID, struct db_event_rec *pdbChanEvent)
{         
   /* ------------------------------------------------------------------
    *	Check if the event is already in the db
    * ----------------------------------------------------------------*/
    int status = FindChanEventRec(pdbChanEvent);
    if (status != uiEVENT_NOT_IN_DB_INFO) {
		status = FindChanEventRec(pdbChanEvent);
		return(status);	//either found it or db error
	}
       
   /* ------------------------------------------------------------------
    *	Now find a day record for the event record.  If the day does
    *	not exit, cannot write the record.
    * ----------------------------------------------------------------*/
    status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, pdbChanEvent->d_event_beg_time_key);
	if (status == uiDAY_NOT_IN_DB_STATUS) 
	{		// sk added 6/16/2003
		DATE dTemp = floor(pdbChanEvent->d_event_beg_time_key);
		status = glDbDay.CreateDayRecInMainDb(ChanID.sStaID, dTemp);
	}
    if (status != 1) 
		return (status);
    
   /* ------------------------------------------------------------------
    *	Create and attach new record
    * ----------------------------------------------------------------*/
    if ( d_setor(DAY_TO_EVENT_SET, CURR_DB) != S_OKAY) goto db_exit; // make day the owner
    if (d_fillnew(DB_EVENT_REC, (char *)pdbChanEvent, CURR_DB) != S_OKAY) goto db_exit;
    if (d_connect(DAY_TO_EVENT_SET, CURR_DB) != S_OKAY) goto db_exit;  

	//temp for debug
	db_event_rec Temp;
    status = FindChanEventRec(pdbChanEvent);
	if (status) {
		status = d_keyfind(D_EVENT_BEG_TIME_KEY, &(pdbChanEvent->d_event_beg_time_key), CURR_DB);
		status = ReadEventRec(pdbChanEvent, &Temp);
		if ((Temp.us_event_sta == 1) || (Temp.us_event_sta == 16))
			Temp.us_event_sta_type = Temp.us_event_sta_type;	// debug
	}
	return(TRUE);
	
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "CreateChanEventRec");
    return(uiDB_BAD_ERR);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	UpdateChanEvent
//
//	Description:
//	If an event with the specified start/end and channel number exists
//	in the database, then update the us_event_status and d_event_grp_time
//	fields in the record and write the record with the updated fields to
//	the database.
//	Note : this routine assumes any time adjust is done before data are handed to it.  It
//	"speaks" only in terms of adjusted times.
//
//	Declaration:
//	static int UpdateChanEvent(struct db_event_rec *pdbChanEvent)
//
//	Input:	pdbChanEvent	structure containing event to write to the database
//			
//	Output: none
//
//	Return: TRUE - event updated
//			Error from FindChanEventRec
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  20-Jul-1995 SFK		Created
//  21-Oct-96	SFK		Added update of group event time
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
//////////////////////////////////////////////////////////////////
static int UpdateChanEvent(struct db_event_rec *pdbChanEvent)
{
    struct db_event_rec dbChanEvent1;
    int status;
       
   /* ------------------------------------------------------------------
    *	Check if the event is in the db
    * ----------------------------------------------------------------*/
    status = FindChanEventRec(pdbChanEvent);
    if (status != 1) 
		return(status);
       
   /* ------------------------------------------------------------------
    *	Read the event record, replace the status and group time and
    *	write the record back to the database
    * ----------------------------------------------------------------*/
	db_event_rec Temp;		// debug
	status = ReadEventRec(pdbChanEvent, &Temp);	// debug

    if (d_recread((char *)&dbChanEvent1, CURR_DB) != S_OKAY) 
	dbChanEvent1.us_event_status = pdbChanEvent->us_event_status;
	dbChanEvent1.d_event_grp_time = pdbChanEvent->d_event_grp_time;
	strcpy(dbChanEvent1.sz_event_id, pdbChanEvent->sz_event_id);
	if (d_recwrite((char *)&dbChanEvent1, CURR_DB) != S_OKAY)goto db_exit;
	return(TRUE);
	
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "UpdateChanEvent");
    return(uiDB_BAD_ERR);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	ReplaceChanEvent
//
//	Description:
//	Find the original event using these fields:
//		us_event_chan, d_event_beg_time_key, d_event_end_time.
//	The new event must have valid information in these fields:
//		us_event_chan, d_event_beg_time_key, d_event_end_time, us_event_type, us_event_type1.
//	Based on the station type associated with us_event_chan in the new event,
//	new values will be calculated for these fields in the new event:
//		d_event_max_time, d_event_integral_sum, d_event_max_value,
//		d_event_avg_reals_rate, d_event_avg_totals_rate.
//	All other fields will be copied from the old event to the new event times.
//	The old event will be deleted from the database and the new event will
//	be written to the database.
//	Note : this routine assumes any time adjust is done before data are handed to it.  It
//	"speaks" only in terms of adjusted times.
//
//	Declaration:
//	static int UpdateChanEvent(struct db_event_rec *pdbChanEvent)
//
//	Input:	pdbChanEvent	structure containing event to write to the database
//			
//	Output: none
//
//	Return: TRUE - event updated
//			Error from FindChanEventRec
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  20-Jul-1995 SFK		Created
//  21-Oct-96	SFK		Added update of group event time
//	11-Dec-2001 SFK		Add the call to unadjust the event times before do search in db for raw data.
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
//	05-Aug-2002	SFK		Changed to not unadjust GRAND and BOT data before FindIntervalInDb
//	29-Sep-2003	SFK		Added StructID to call.
//	28-Nov-2004	SFK		3.0.0.16.  Corrected calculation of average total and reals rate if SR event
//	05-Jan-2005	SFK		Fill number of runs in event into events coming from GRAND type instruments
//						in addition to calculating integral sums, etc.
//////////////////////////////////////////////////////////////////
#define AT_BIT			0x00010000L
int ReplaceChanEvent(struct IDStruct OldChanID, struct db_event_rec *pdbOldEvent, struct IDStruct NewChanID, struct db_event_rec *pdbNewEvent)
{
	struct db_sta_rec dbSta;
	struct db_chan_rec dbChan;
	struct WHOLE_DOUBLE_DATA_PT1 *pdPts;

	double dActualStartTime, dActualEndTime, dNextStartTime;
	unsigned long ulTotalPts, ulNumPts, ulNumPtsLeft, ulPtsRead, ulNumAllocPts, ulPtsToRead;
	int i,j;
	double dRealsSum, dTotalsSum, dSumTime;
	double dRealsRate;
	CGFmtStr msg;

   /* ------------------------------------------------------------------
    *	Check if the old event is in the db.  If not, create a fake
	*	event with some default values that we can use in this function.
    * ----------------------------------------------------------------*/
	int status = FindChanEventRec(pdbOldEvent);
    if (status != 1) 
	{		// no old event exists so make up one to use
		pdbOldEvent->us_event_chan = -1;	// set up for delete later on
		//pdbNewEvent->us_event_chan		// filled in by calling routine
		//pdbNewEvent->d_event_beg_time_key	// filled in by calling routine
		//pdbNewEvent->d_event_end_time 	// filled in by calling routine
		pdbOldEvent->d_event_max_time = 0;
		pdbOldEvent->d_event_integral_sum = 0.0;
		pdbOldEvent->d_event_max_value = 0.0;
		pdbOldEvent->d_event_avg_reals_rate = 0.0;
		pdbOldEvent->d_event_avg_totals_rate = 0.0;
		//pdbNewEvent->us_event_type 		// filled in by calling routine
		pdbOldEvent->us_event_status = -1;
		//pdbNewEvent->us_event_type1 		// filled in by calling routine
		pdbOldEvent->us_event_status1 = -1;
		pdbOldEvent->us_event_type2   = -1;
		pdbOldEvent->us_event_status2 = -1;
		pdbOldEvent->s_num_runs = 0;	
	}
	else {	// event to replace exists in the database, read it now
		if (d_recread(pdbOldEvent, CURR_DB) != S_OKAY) return(FALSE);
	}
	       
   /* ------------------------------------------------------------------
    *	Figure out which station the channel belongs to and how much data 
	*	exists for the station in the db
    * ----------------------------------------------------------------*/
	if (!pglFacCfg->GetStationRecord(glsFacNum, &OldChanID, &dbSta)) return(FALSE);

	//	If the data on the graph (from which this routine is called) has been time adjusted, you
	//	must unadjust the times before you find the raw data in the database since those routines
	//	apply an adjustment on what is returned.  This is a short term measure which should eventually 
	//	be cleaned up by doing time adjust properly.
	status = GetDataStatistics(OldChanID, true, pdbNewEvent->d_event_beg_time_key, pdbNewEvent->d_event_end_time, &dActualStartTime, &dActualEndTime, &ulTotalPts);
    if (status != 1) 
		return 0; 

	pdbNewEvent->d_event_beg_time_key = dActualStartTime;	// 26-Sep-2005 SFK Reset time to the real time on the data record (think roundoff is getting to us)
	pdbNewEvent->d_event_end_time = dActualEndTime;			// 26-Sep-2005 SFK Reset time to the real time on the data record

    if (ulTotalPts == 0) {
		RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, pdbNewEvent->d_event_beg_time_key, pdbNewEvent->d_event_end_time, dbSta.sz_sta_name);
    	return(FALSE);
    }
	ulNumPtsLeft = ulTotalPts;
	ulNumAllocPts = ulTotalPts;
	dNextStartTime = dActualStartTime;

	if (!pglFacCfg->GetChannelRecord(glsFacNum, &NewChanID, &dbChan)) return(FALSE);
	j = dbChan.s_chan_offset - 1;


   /* ------------------------------------------------------------------
    *	Before we can replace (or create) the event in the database, we
	*	have to go through and calculate the sums, etc. that are expected
	*	to be in the event record.  The way you do this is different for
	*	SR type instruments from everything else.
    * ----------------------------------------------------------------*/
	if ((dbSta.s_sta_type == GRAND_TYPE) || (dbSta.s_sta_type == BOT_TYPE)) {
			
	   /* ------------------------------------------------------------------
		*	Allocate array needed in the processing
		* ----------------------------------------------------------------*/
		pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
		while ((pdPts == NULL) && (ulNumAllocPts > 50)){		// try a smaller number if can't allocate the memory
			ulNumAllocPts = ulNumAllocPts - 50;
			pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
		}   
		if (ulNumAllocPts <= 0) {
			RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
			if (pdPts) free(pdPts);
			pdPts = NULL;
			return(FALSE);
		}	

		ulNumPts = 0;	
		do 
		{				
		   /* ------------------------------------------------------------------
			*	Figure out how many points to read and process
			*	If necessary, read one point past the end of this segment to determine
			*	where to start the next read.
			* ----------------------------------------------------------------*/
			if (ulNumPtsLeft > ulNumAllocPts)
				ulPtsToRead = ulNumAllocPts;
			else
				ulPtsToRead = ulNumPtsLeft;

			ulNumPtsLeft = ulNumPtsLeft - ulPtsToRead;
			status = GetDoubleDataByStation(NewChanID, dNextStartTime, dActualEndTime, ulPtsToRead, &ulPtsRead, pdPts, &dNextStartTime, TRUE);				
			if (status == uiDAY_NOT_IN_DB_STATUS) 
				status = 1;  // handle case of no data in this subinterval
			
			if (status != 1) 
			{
				if (pdPts) free(pdPts);
				pdPts = NULL;
				return 0;
			}
		
		   /* ------------------------------------------------------------------
			*	Go through the points in the event, updating the integral sum
			*	and max value fields.
			* ----------------------------------------------------------------*/
			for (i=0; i<(int)ulPtsRead; i++) {              
				if (ulNumPts == 0){ // first point in the event
					pdbNewEvent->d_event_max_time = pdPts[i].dJulianTime;
					pdbNewEvent->d_event_integral_sum = pdPts[i].dData[j] * pdPts[i].dElapsedTime;
					pdbNewEvent->d_event_max_value = pdPts[i].dData[j];
				}
				else {
					pdbNewEvent->d_event_integral_sum += pdPts[i].dData[j] * pdPts[i].dElapsedTime;	// accumlate the integral sum
					if (pdPts[i].dData[j] > pdbNewEvent->d_event_max_value) { //new max value??
						pdbNewEvent->d_event_max_time = pdPts[i].dJulianTime;
						pdbNewEvent->d_event_max_value = pdPts[i].dData[j];
					}
				}
				ulNumPts++;
			}
			
		} while (ulNumPtsLeft > 0);

	   /* ------------------------------------------------------------------
		*	Update the other fields by reading information from the old record 
		*	through the points in the event, updating the integral sum
		* ----------------------------------------------------------------*/
		//pdbNewEvent->us_event_chan			filled in by calling routine
		//pdbNewEvent->d_event_beg_time_key		filled in by calling routine
		//pdbNewEvent->d_event_end_time 		filled in by calling routine
		//pdbNewEvent->d_event_max_time 		filled in by this function
		//pdbNewEvent->d_event_integral_sum		filled in by this function
		//pdbNewEvent->d_event_max_value		filled in by this function
		pdbNewEvent->d_event_avg_reals_rate = pdbOldEvent->d_event_avg_reals_rate;
		pdbNewEvent->d_event_avg_totals_rate = pdbOldEvent->d_event_avg_totals_rate;
		//pdbNewEvent->us_event_type 			filled in by calling routine
		pdbNewEvent->us_event_status = pdbOldEvent->us_event_status;
		//pdbNewEvent->us_event_type1 			filled in by calling routine
		pdbNewEvent->us_event_status1 = pdbOldEvent->us_event_status1;
		pdbNewEvent->us_event_type2   = pdbOldEvent->us_event_type2;
		pdbNewEvent->us_event_status2 = pdbOldEvent->us_event_status2;
		pdbNewEvent->d_event_grp_time = pdbOldEvent->d_event_grp_time;
		//pjm 26-Jan-2006 Added explicit cast to eliminate compiler warning.
		pdbNewEvent->s_num_runs		  = (short)ulNumPts;	// 05-Jan-2005 SFK Fill in number of points in the event
		if (pdPts != NULL) free(pdPts);
		pdPts = NULL;

	}
			
	// ver 2.05 added support for JSR1_TYPE
	if ((dbSta.s_sta_type == JSR_TYPE) || (dbSta.s_sta_type == ISR_TYPE) || dbSta.s_sta_type == JSR1_TYPE) {
	   /* ------------------------------------------------------------------
		*	Allocate array needed in the processing
		* ----------------------------------------------------------------*/
		pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
		while ((pdPts == NULL) && (ulNumAllocPts > 50)){
			ulNumAllocPts = ulNumAllocPts - 50;
			pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
		}   
		if (ulNumAllocPts <= 0) {
			RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
			if (pdPts != NULL) free(pdPts);
			pdPts = NULL;
			return(FALSE);
		}	
		ulNumPts = 0;

		do 
		{		
		   /* ------------------------------------------------------------------
			*	Figure out how many points to read and process
			*	If necessary, read one point past the end of this segment to determine
			*	where to start the next read.
			* ----------------------------------------------------------------*/
			if (ulNumPtsLeft > ulNumAllocPts)
				ulPtsToRead = ulNumAllocPts;
			else
				ulPtsToRead = ulNumPtsLeft;

			ulNumPtsLeft = ulNumPtsLeft - ulPtsToRead;

			status = GetDoubleDataByStation(NewChanID, dNextStartTime, dActualEndTime, ulPtsToRead, &ulPtsRead, pdPts, &dNextStartTime, TRUE);							
			if (status == uiDAY_NOT_IN_DB_STATUS) 
				status = 1;  // handle case of no data in this subinterval 
			if (status != 1) 
			{
				if (pdPts != NULL) free(pdPts);
				pdPts = NULL;
				return 0; 
			}
		
		   /* ------------------------------------------------------------------
			*	Depending on what type of an event this is, update the rest of the
			*	event record in one of two ways.  For CHANNEL_EVENT or UNEXPLAINED_EVENT
			*	calcuate the integral sum and max value like for GRAND types.CGo through the points in the event, updating the integral sum
			*	and max value fields.
			*	This mess should be fixed when the event stuff goes into COMs.
			* ----------------------------------------------------------------*/
			for (i=0; i<(int)ulPtsRead; i++) {
				// these are like threshold events
				if ((pdbNewEvent->us_event_type == CHANNEL_EVENT) || (pdbNewEvent->us_event_type == UNEXPLAINED_EVENT)) {
					//dRealsRate = pdPts[i].dData[j]/pdPts[i].dElapsedTime;
					dRealsRate = pdPts[i].dData[j];					// 3.0.3.16  Data already has rate
					if (ulNumPts == 0){ // first point in the event
						pdbNewEvent->d_event_max_time = pdPts[i].dJulianTime;
						pdbNewEvent->d_event_integral_sum = pdPts[i].dData[j] * pdPts[i].dElapsedTime;
						pdbNewEvent->d_event_max_value = dRealsRate;
					}
					else {
						pdbNewEvent->d_event_integral_sum += pdPts[i].dData[j] *pdPts[i]. dElapsedTime;	// accumlate the integral sum
						if (dRealsRate > pdbNewEvent->d_event_max_value) { //new max value??
							pdbNewEvent->d_event_max_time = pdPts[i].dJulianTime;
							pdbNewEvent->d_event_max_value = dRealsRate;
						}
					}
					dRealsSum  = 0;
					dTotalsSum = 0;
					dSumTime   = 1;

				}
				else {
					// these are the events found through SR analysis
					if (ulNumPts == 0){ // first point in the event
						pdbNewEvent->d_event_max_time = pdPts[i].dJulianTime;
						pdbNewEvent->d_event_integral_sum = pdPts[i].dData[REALS_OFFSET];
						pdbNewEvent->d_event_max_value = pdPts[i].dData[REALS_OFFSET];
						if ((pdPts[i].ulStatus & AT_BIT) != AT_BIT) { // use old A/T infor
							dRealsSum  = pdPts[i].dData[REALS_OFFSET] * pdPts[i].dElapsedTime;
							dTotalsSum = pdPts[i].dData[TOTALS_OFFSET] * pdPts[i].dElapsedTime;
							dSumTime   = pdPts[i].dElapsedTime;
						}
					} 
					else {
						if (ulNumPts != (ulTotalPts - 1)) { // SR Collect did not use the end point values
							pdbNewEvent->d_event_integral_sum += pdPts[i].dData[REALS_OFFSET];	// accumlate the integral sum
							if ((pdPts[i].ulStatus & AT_BIT) != AT_BIT) { // use old A/T infor
								dRealsSum  += pdPts[i].dData[REALS_OFFSET] * pdPts[i].dElapsedTime;
								dTotalsSum += pdPts[i].dData[TOTALS_OFFSET] * pdPts[i].dElapsedTime;
								dSumTime   += pdPts[i].dElapsedTime;
							}
							if (pdPts[i].dData[REALS_OFFSET] > pdbNewEvent->d_event_max_value) { //new max value??
								pdbNewEvent->d_event_max_time = pdPts[i].dJulianTime;
								pdbNewEvent->d_event_max_value = pdPts[i].dData[REALS_OFFSET];
							}
						}
					}
				}
				ulNumPts++;
			}

		} while (ulNumPtsLeft > 0);

	   /* ------------------------------------------------------------------
		*	Update the other fields by reading information from the old record.
		* ----------------------------------------------------------------*/
		//pdbNewEvent->us_event_chan			filled in by calling routine
		//pdbNewEvent->d_event_beg_time_key	filled in by calling routine
		//pdbNewEvent->d_event_end_time 		filled in by calling routine
		//pdbNewEvent->d_event_max_time 		filled in by this function
		//pdbNewEvent->d_event_integral_sum		filled in by this function
		//pdbNewEvent->d_event_max_value		filled in by this function
		pdbNewEvent->d_event_avg_reals_rate = dRealsSum/dSumTime;
		pdbNewEvent->d_event_avg_totals_rate = dTotalsSum/dSumTime;
		//pdbNewEvent->us_event_type 			filled in by calling routine
		pdbNewEvent->us_event_status = pdbOldEvent->us_event_status;
		//pdbNewEvent->us_event_type1 			filled in by calling routine
		pdbNewEvent->us_event_status1 = pdbOldEvent->us_event_status1;
		pdbNewEvent->us_event_type2   = pdbOldEvent->us_event_type2;
		pdbNewEvent->us_event_status2 = pdbOldEvent->us_event_status2;
		pdbNewEvent->d_event_grp_time = pdbOldEvent->d_event_grp_time;
		pdbNewEvent->s_num_runs = (short)ulNumPts;	
		if (pdPts != NULL) free(pdPts);
		pdPts = NULL;
	}

	if (pdbOldEvent->us_event_chan != -1) {		// -1 is the "fake" event we created at the beginning of this function
		DeleteDataByChannelEvents(pdbOldEvent);
	}
	CreateChanEventRec(NewChanID, pdbNewEvent);
	if (gll_pUpdateFnc != NULL) {
		gll_pUpdateFnc();
	}	


	return(TRUE);
	
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStatisticsForEventsByChannel
//			Replaces NumChanEventsInDbInterval
//
//	Description:
//	Count the number of events for the specified channel
//	in the specified interval in the database.  Based on the type
//	of event requested, then different types of channel events
//	are counted.  (Qualifiers are channel number and event type).
//	Note : this routine assumes any time adjust is done before data are handed to it.  It
//	"speaks" only in terms of adjusted times.
//
//	Declaration:
//	int GetDataStatisticsForEventsByChannel(short sChan, DATE dStartTime, DATE dEndTime, unsigned long *pulNumEvents, unsigned short usEventType)
//
//	Input:	sChan		channel want to find events for
//			dStartTime	interval start to search for events
//			dEndTime	interval end to search for events
//			usEventType	type of event looking to match
//			
//	Output: pulNumEvents	number of events that match the conditions
//
//	Return: TRUE - event updated
//			Error from GetStationOfChan, FindIntervalInDb, FindDayRec
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  31-MAY-1995  SFK	Created from old GRAND Review getpts.c
//	24-Mar-1998	 SFK	Added concept of specifying which type of events to count
//	05-Oct-1998  SFK	Added GRAPH EVENTS
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec) and
//						changed name to GetDataStatisticsForEventsByChannel
//	29-Sep-2003	SFK		Added StructID to call.
//////////////////////////////////////////////////////////////////
// new with DATE
int GetDataStatisticsForEventsByChannel(
	IDStruct ChanID, 
	DATE dStartTime, 
	DATE dEndTime, 
	unsigned long *pulNumEvents, 
	unsigned short usEventType)
{
    struct db_event_rec dbChanEvent;
    int  db_stat, status;           

    DATE dCurrentDay;		    /* day you want to find in database in jul secs */
    BOOL bDone = FALSE;   
	BOOL bLastDay = FALSE;
    BOOL ThisDayDone;

    *pulNumEvents = 0;

	if (dEndTime < dStartTime) 
		return(1);
           
	// times are DATE so start of day is just dStartTime with no fractional part
	dCurrentDay = floor(dStartTime);
    
   /* ------------------------------------------------------------------
    *	Will search through all the events under this day record one by one.
	*	An event "count" if the start timestamp of an event for the 
	*	specified channel falls within the day -- AND the event is of
	*	the expected type.  The choices for event type are:
	*		BACKGROUND_EVENT - a SR analysis type
	*		NORMAL_BKG_MEAS - a SR analysis type
	*		MEASUREMENT_EVENT - a SR analysis type
	*		CHANNEL_EVENT - comes from threshold analysis -- applies to all station types
	*	Based on where this routine is called from the user can indicate 
	*	which of the events he wants:
	*		ALL_EVENTS - return all events meeting the above criteria
	*		GRAPH_EVENTS - return only normal background events, measurement 
	*				events and channel events.  (16-Dec-2004 changed to return all bkg events)
	*	Special caution:  For GRAPH_EVENTS, the event counts if any part
	*	of the event falls within the day -- either the start or end of
	*	the event.
	* ----------------------------------------------------------------*/
    do 
	{		// sk 6/16/2003 added this to find the first day with events in
		status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dCurrentDay);

		if (status == uiDAY_NOT_IN_DB_STATUS) 
			dCurrentDay += 1.0f;
		else if (status != 1) 
			return status;

		if (dCurrentDay > dEndTime) 
			return 1;

	} while (status != 1);

    do 
	{
	    if (d_setor(DAY_TO_EVENT_SET, CURR_DB)!= S_OKAY) 
			goto db_exit; 	/* select Event set */
	    
    	ThisDayDone = FALSE;
	    db_stat = d_findfm(DAY_TO_EVENT_SET, CURR_DB); 	/* see if any events */
	    if (db_stat == S_EOS) {
	    	ThisDayDone = TRUE;
	    }
	    else {	
	    	if (db_stat != S_OKAY) goto db_exit;
	    }	
	                                             
		if (ThisDayDone == FALSE) {	                                             
		   	do {                                            /* count the events with start times in this day */
				db_stat = d_recread(&dbChanEvent, CURR_DB);
				if (db_stat != S_OKAY) goto db_exit;
				if ((dbChanEvent.d_event_beg_time_key >= dStartTime) &&
				   	(dbChanEvent.d_event_beg_time_key <= dEndTime) && 
					(dbChanEvent.us_event_sta == ChanID.sStaID) &&		// 23-May-2005 Add condition of station match too
					(dbChanEvent.us_event_chan == (unsigned short)ChanID.sChanID)) {
					if ((usEventType == ALL_EVENTS) || (dbChanEvent.us_event_type == usEventType)) (*pulNumEvents)++;
					if(usEventType == GRAPH_EVENTS) { // only display measurements and all background for GRAPH
						//if (((dbChanEvent.us_event_type == BACKGROUND_EVENT) && (dbChanEvent.us_event_type1 == NORMAL_BKG_MEAS)) ||
						if ((dbChanEvent.us_event_type == BACKGROUND_EVENT) ||		// 16-Dec-2004 SFK Changed to graph all background events
							(dbChanEvent.us_event_type == MEASUREMENT_EVENT) ||
							(dbChanEvent.us_event_type == CHANNEL_EVENT)) {
							(*pulNumEvents)++;
						}
					}
				} else {
					// special case for graph where only end time of event in day
					if (usEventType == GRAPH_EVENTS){
						if ((dbChanEvent.d_event_end_time >= dStartTime) &&
				   		(dbChanEvent.d_event_end_time <= dEndTime) && 
						(dbChanEvent.us_event_sta == ChanID.sStaID) &&		// 23-May-2005 Add condition of station match too
						(dbChanEvent.us_event_chan == (unsigned short)ChanID.sChanID)) {
							//if (((dbChanEvent.us_event_type == BACKGROUND_EVENT) && (dbChanEvent.us_event_type1 == NORMAL_BKG_MEAS)) ||
							if ((dbChanEvent.us_event_type == BACKGROUND_EVENT) ||		// 16-Dec-2004 SFK Changed to graph all background events
								(dbChanEvent.us_event_type == MEASUREMENT_EVENT) ||
								(dbChanEvent.us_event_type == CHANNEL_EVENT)) {
								(*pulNumEvents)++;
							}
						}
					}
				}
				db_stat = d_findnm(DAY_TO_EVENT_SET, CURR_DB);
			} while (db_stat == S_OKAY);

			if (db_stat != S_EOS) 
				goto db_exit;
		}	
		
		if ((int)dCurrentDay == (int)dEndTime) 
			bDone = TRUE;

		if (!bDone) 
		{	// current day is not the last day, find the next day
			do 
			{
				dCurrentDay += 1.0;				// add 1 to get to next day in DATE format
				if ((int)dCurrentDay == (int)dEndTime) 
					bLastDay = TRUE;	// the new day is the last one
				status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dCurrentDay);		// see if the new day is in the db
	    	} while ((status != 1) && (bLastDay == 0)); 	
	    }
		if ((bLastDay == 1) && (status == uiDAY_NOT_IN_DB_STATUS)) 	// 20-Dec-2004 SFK if the last day isn't in DB, then quit
			bDone = TRUE;
		// We don't get an error above when we try to set owner so have to abort here.

    } while (!bDone);
    
	return 1;
	
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "GetDataStatisticsForEventsByChannel", db_stat);
    return(uiDB_BAD_ERR);
}       


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStatisticsForEventsByStation
//			Replaces NumChanEventsInDbInterval
//
//	Description:
//	Count the number of events for the specified station
//	in the specified interval in the database.  (Only qualifier
//	is if the event is associated with the given station).
//	An event "counts" if its start time falls within the interval.
//	Note : this routine assumes any time adjust is done before data are handed to it.  It
//	"speaks" only in terms of adjusted times.
//
//	Declaration:
//	int GetDataStatisticsForEventsByStation(short sSta, DATE dStartTime, DATE dEndTime, unsigned long *pulNumEvents)
//
//	Input:	sSta		station want to find events for all channels
//			dStartTime	interval start to search for events
//			dEndTime	interval end to search for events
//			
//	Output: pulNumEvents	number of events that match the conditions
//
//	Return: TRUE - event updated
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  26-Oct-2001  SFK	Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec) and
//						changed name to GetDataStatisticsForEventsByStation
//	29-Sep-2003	SFK		Added StructID to call.
//////////////////////////////////////////////////////////////////
// new with DATE
//int GetDataStatisticsForEventsByStation(short sSta, DATE dStartTime, DATE dEndTime, unsigned long *pulNumEvents)
int GetDataStatisticsForEventsByStation(IDStruct StaID, DATE dStartTime, DATE dEndTime, unsigned long *pulNumEvents)
{
    int db_stat;
	struct db_event_rec dbChanEvent;

	*pulNumEvents = 0;

    // find the key closest to the start time and lock in on it, read the record
   	db_stat = d_keyfind(D_EVENT_BEG_TIME_KEY, &dStartTime, CURR_DB);
   	if (db_stat != S_OKAY) {
   		db_stat = d_keynext(D_EVENT_BEG_TIME_KEY, CURR_DB);
   	}	
   	if (db_stat == S_NOTFOUND) return(uiEVENT_NOT_IN_DB_INFO);
	if (db_stat != S_OKAY) goto db_exit;
	if (d_recread(&dbChanEvent, CURR_DB) != S_OKAY) goto db_exit;

	// search through all the events and count them if they belong
	// to the station specified.
	do {
		if (dbChanEvent.us_event_sta == StaID.sStaID) (*pulNumEvents)++;        
		db_stat = d_keynext(D_EVENT_BEG_TIME_KEY, CURR_DB);
		if (db_stat == S_NOTFOUND) break;	// at the end of the events
		if (d_recread(&dbChanEvent, CURR_DB) != S_OKAY) goto db_exit;
	} while (dbChanEvent.d_event_beg_time_key < dEndTime);

    return(TRUE);
    
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "GetDataStatisticsForEventsByStation");
    return(uiDB_BAD_ERR);
}    



///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByChannelEvents
//			Replaces ReadChanEventsInDbInterval
//
//	Description:
//	Read the specified number of the specified type of events for the
//	specified station in the specified interval in the database. 
//	An event meets conditions if its start time falls within the interval (except
//	for GRAPH_EVENTS which count if any part of the event falls within the interval).
//	Note : this routine assumes any time adjust is done before data are handed to it.  It
//	"speaks" only in terms of adjusted times.
//
//	Declaration:
//	int GetDataByStationEvents(short sChan, DATE dStartTime, DATE dEndTime, unsigned long ulNumEventsToRead, unsigned long *pulNumEventsRead, struct db_event_rec huge *pChanEvents, unsigned short usEventType)
//
//	Input:	sSta		station want to find events for all channels
//			dStartTime	interval start to search for events
//			dEndTime	interval end to search for events
//			ulNumEventsToRead	number of events expected to read that have had storage allocated
//			usEventType	event type that determines which events to read (see details below)
//			
//	Output: pulNumEventsRead	number of events actually read from db
//			pChanEvents			array where events read are stored
//
//	Return: TRUE - event updated
//			Error from GetStationOfChan, FindIntervalInDb, FindDayRec
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  20-Jul-1995  SFK	 Created
//	24-Mar-1998	 SFK	Added concept of specifying which type of events to read
//	05-Oct-1998  SFK	Added GRAPH EVENTS
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec) and
//						changed name to GetDataStatisticsForEventsByStation
//	29-Sep-2003	SFK		Added StructID to call.
//////////////////////////////////////////////////////////////////
// new with DATE
int GetDataByChannelEvents(IDStruct ChanID, DATE dStartTime, DATE dEndTime, unsigned long ulNumEventsToRead, unsigned long *pulNumEventsRead, struct db_event_rec huge *pChanEvents, unsigned short usEventType)
{
	*pulNumEventsRead = 0;

	if ((ulNumEventsToRead == 0) || (dEndTime < dStartTime))
		return 1;

	struct db_event_rec dbChanEvent;
	int db_stat, status;
	unsigned long  i;
	DATE dCurrentDay;		// day you want to find in database in jul secs
	DATE dActualEndTime;
	DATE dActualStartTime;
	unsigned long ulDum;
	BOOL bDone = FALSE;
	BOOL bLastDay = FALSE;
	BOOL bEventsInDay;


	/* ------------------------------------------------------------------
	*	Find the actual start and end time of data in the database
	* ----------------------------------------------------------------*/
	status = GetDataStatistics(ChanID, true, dStartTime, dEndTime, &dActualStartTime, &dActualEndTime, &ulDum);
	if (status != 1) 
		return(status);
	dCurrentDay = floor(dActualStartTime);

	/* ------------------------------------------------------------------
	*	Will search through all the events under this day record one by one.
	*	An event is read if the start timestamp of an event for the 
	*	specified channel falls within the day -- AND the event is of
	*	the expected type.  The choices for event type are:
	*		CHANNEL_EVENT - comes from threshold analysis -- applies to all station types
	*		ALL_EVENTS - return all events meeting the above criteria
	*		GRAPH_EVENTS - return only normal background events, measurement 
	*				events and channel events. (16-Dec-2004 SFK changed to display all bkg events)
	*	Special caution:  For GRAPH_EVENTS, the event counts if any part
	*	of the event falls within the day -- either the start or end of
	*	the event.
	* ----------------------------------------------------------------*/
	do 
	{		// sk 6/16/2003 added this to find the first day with events in
		status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dCurrentDay);
		if (status == uiDAY_NOT_IN_DB_STATUS) 
			dCurrentDay += 1;
		else if (status != 1) 
			return(status);

		if (dCurrentDay > dEndTime) 
			return 1;

	} while (status != 1);

	i = 0;
	do 
	{
		if(d_setor(DAY_TO_EVENT_SET, CURR_DB) != S_OKAY) 
			goto db_exit;	// select Event set

		bEventsInDay = TRUE;
		db_stat = d_findfm(DAY_TO_EVENT_SET, CURR_DB); 	/* see if any events */
		if (db_stat == S_EOS) 
		{
			bEventsInDay = FALSE;
		}
		else
		{
			if (db_stat != S_OKAY) 
				goto db_exit;
		}

		if (bEventsInDay == TRUE) 
		{	// some events in this day, see if any belong to the station
			do
			{			// sort thru the events, saving those that match the conditions                                         
				if (d_recread(&dbChanEvent, CURR_DB) != S_OKAY) 
					goto db_exit;

				if ((dbChanEvent.d_event_beg_time_key >= dActualStartTime) &&
					(dbChanEvent.d_event_beg_time_key <= dActualEndTime) && 
					(dbChanEvent.us_event_sta == ChanID.sStaID) &&		// 23-May-2005 Add condition of station match too
					(dbChanEvent.us_event_sta_type == ChanID.sStaType) &&	//09-Apr-2009 Add condition on sta type to prevent spurious events
					(dbChanEvent.us_event_chan == (unsigned short)ChanID.sChanID)) 
				{
					if ((usEventType == ALL_EVENTS) || (dbChanEvent.us_event_type == usEventType)) 
					{
						pChanEvents[i] = dbChanEvent;
						i++;
					}
					if (usEventType == GRAPH_EVENTS)  // only display measurements and normal background for GRAPH
					{
						if ((dbChanEvent.us_event_type == BACKGROUND_EVENT) ||		// 16-Dec-2004 SFK Changed to graph all background events
							(dbChanEvent.us_event_type == MEASUREMENT_EVENT) ||
							(dbChanEvent.us_event_type == CHANNEL_EVENT) || 
							(dbChanEvent.us_event_type == UNEXPLAINED_EVENT)) 
						{
							pChanEvents[i] = dbChanEvent;
							i++;
						}
					}
				}
				else
				{
					// special case for graph where only last part of event in range
					if (usEventType == GRAPH_EVENTS)
					{
						if ((dbChanEvent.d_event_end_time >= dActualStartTime) &&
						(dbChanEvent.d_event_end_time <= dActualEndTime) && 
						(dbChanEvent.us_event_sta == ChanID.sStaID) &&		// 23-May-2005 Add condition of station match too
						(dbChanEvent.us_event_sta_type == ChanID.sStaType) &&	//09-Apr-2009 Add condition on sta type to prevent spurious events
						(dbChanEvent.us_event_chan == (unsigned short)ChanID.sChanID)) 
						{
							if ((dbChanEvent.us_event_type == BACKGROUND_EVENT) ||		// 16-Dec-2004 SFK Changed to graph all background events
								(dbChanEvent.us_event_type == MEASUREMENT_EVENT) ||
								(dbChanEvent.us_event_type == CHANNEL_EVENT) || 
								(dbChanEvent.us_event_type == UNEXPLAINED_EVENT)) 
							{
								pChanEvents[i] = dbChanEvent;
								i++;
							}
						}
					}
				}
				db_stat = d_findnm(DAY_TO_EVENT_SET, CURR_DB);
//#ifdef _DEBUG
//				if (dbChanEvent.us_event_sta_type > 60000)
//					dbChanEvent.us_event_sta_type = dbChanEvent.us_event_sta_type;		//debug
//#endif

			} while ((db_stat == S_OKAY) && (i < ulNumEventsToRead));	
		}
		
		if ((int)dCurrentDay == (int)dActualEndTime) 
			bDone = TRUE;

		if (!bDone)	// no, find the next day
		{
			do
			{
				dCurrentDay += 1.0;					// move on to the next day
				if ((int)dCurrentDay == (int)dActualEndTime) 
					bLastDay = TRUE;	// new day is the last one
				status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dCurrentDay);
				// new day in db??
		} while ((status != 1) && (bLastDay == FALSE)); 
			
			if ((bLastDay == TRUE) && (status == uiDAY_NOT_IN_DB_STATUS)) 
				bDone = TRUE;	// 20-Dec-2004 SFK if the last day isn't in DB, then quit
			// We don't get an error above when we try to set owner so have to abort here.
		}
	} while (!bDone);
	
	*pulNumEventsRead = i;

	return(TRUE);

db_exit:
	RadReviewMsg(uiDB_BAD_ERR, "GetDataByStationEvents", db_stat);
	return(uiDB_BAD_ERR);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	DeleteDataByChannelEvents
//			Replaces DeleteDbChanEvents
//
//	Description:
//	Delete all events of the specified type from the database at the 
//	specified facility
//
//	Declaration:
//	int DeleteDataByChannelEvents(short sFacNum, unsigned short usEventType)
//
//	Input:	sFacNum		facility want to find events for
//			usEventType	event type that determines which events to delete
//			
//	Output: none
//
//	Return: TRUE - all events for facility deleted
//			Error from FindFacRec
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//   26-Jan-1996 SFK	Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec) and
//						changed name from DeleteDbChanEvents
//////////////////////////////////////////////////////////////////
int DeleteDataByChannelEvents(short sFacNum, unsigned short usEventType)
{
    DB_ADDR dbStaAdr, dbDayAdr;	/* db addresses */
    int db_stat;
	struct db_event_rec dbChanEvent;
	struct db_sta_rec dbSta;
	struct db_day_rec dbDay;
       
   /* ------------------------------------------------------------------
    *	Check if the facility is in the database
    * ----------------------------------------------------------------*/
	int status = FacilityExistsInMainDb(sFacNum);
    if (status != 1) 
		return(status);

   /* ------------------------------------------------------------------
    *	Next find all the stations in this facility.  Then for each
    *   station look for all the days for it and finally delete all
    *   the events associated with every day.
    * ----------------------------------------------------------------*/
	// find all the stations in the facility                                                      
    if (d_setor(FACILITY_TO_STATION_SET, CURR_DB) != S_OKAY) goto db_exit;
    for (db_stat = d_findfm(FACILITY_TO_STATION_SET, CURR_DB);
    	 db_stat == S_OKAY;
		 db_stat = d_findnm(FACILITY_TO_STATION_SET, CURR_DB)) {

		db_stat = d_recread(&dbSta, CURR_DB);		//
		// find all the days belonging to this station                                                             	
		if (d_crget(&dbStaAdr, CURR_DB)!= S_OKAY) goto db_exit;	/* save station adr for later */
		if (d_setor(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) goto db_exit; // make sta owner of sta-day set
	    for (db_stat = d_findfm(STATION_TO_DAY_SET, CURR_DB); 
	    	 db_stat == S_OKAY;
			 db_stat = d_findnm(STATION_TO_DAY_SET, CURR_DB)) {   
			 
            // find and delete all event records for this day         	
			db_stat = d_recread(&dbDay, CURR_DB);
			if (d_crget(&dbDayAdr, CURR_DB) != S_OKAY) goto db_exit;	/* save day adr for later */
			if (d_setor(DAY_TO_EVENT_SET, CURR_DB)!= S_OKAY) goto db_exit; 
			long lMembers;
			db_stat = d_members(DAY_TO_EVENT_SET, &lMembers, CURR_DB);
		    for (db_stat = d_findfm(DAY_TO_EVENT_SET, CURR_DB); 
		    	 db_stat == S_OKAY;
				 db_stat = d_findnm(DAY_TO_EVENT_SET, CURR_DB)) { 

				// only delete events that match the type supplied
				db_stat = d_recread(&dbChanEvent, CURR_DB);
				if (dbChanEvent.us_event_type == usEventType) {
		   			if (d_disdel(CURR_DB)!= S_OKAY) goto db_exit;
				}
			}
			if (d_crset(&dbDayAdr, CURR_DB)!= S_OKAY) goto db_exit;	/* restore day adr */
	    }
		if (d_crset(&dbStaAdr, CURR_DB)!= S_OKAY) goto db_exit;	/* restore sta adr */
    }	
    return(TRUE);

db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "DeleteDataByChannelEvents");
    return(uiDB_BAD_ERR);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	DeleteDataByChannelEvents
//			Replaces DeleteDbChanEvent
//
//	Description:
//	Delete the event that matches (time, channel number) the specified event.
//
//	Declaration:
//	int DeleteDataByChannelEvents(struct db_event_rec *pdbChanEvent)
//
//	Input:	pdbChanEvent	event to delete matches (time, channel)
//			
//	Output: none
//
//	Return: TRUE - event deleted
//			Error from FindChanEventRec
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  26-Jan-1996 SFK	Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec) and
//						changed name from DeleteDbChanEvent
//////////////////////////////////////////////////////////////////
int DeleteDataByChannelEvents(struct db_event_rec *pdbChanEvent)
{
    int status = FindChanEventRec(pdbChanEvent);	// event in db?
    if (status != 1) 
		return(status);
    
    if (d_disdel(CURR_DB)!= S_OKAY) 
		goto db_exit; // delete it

    return 1;

db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "DeleteDataByChannelEvents");
    return(uiDB_BAD_ERR);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	ScanChannelEvents
//			Replaces ScanChanEventRecs
//
//	Description:
//	This function allows the user to scan through all event records found
//	in the database.  They are stored in the db by time.  User can
//	select to find the record closest to a specified time or just
//	the next record in the list.
//
//	Declaration:
//	int ScanChannelEvents(DATE dStartTime, short sScanType, struct db_event_rec *pdbChanEvent)
//
//	Input:	dStartTime	start time for event search
//			sScanType	either FIRST event after dStartTime or NEXT event from previous call
//			
//	Output: pdbChanEvent	event read from database
//
//	Return: TRUE - found an event
//		    uiEVENT_NOT_IN_DB_INFO - event not in db
//			FALSE - Type not FIRST or NEXT
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  20-Jul-1995 SFK	Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec) and
//						changed name from ScanChanEventRecs
//////////////////////////////////////////////////////////////////
int ScanChannelEvents(DATE dStartTime, short sScanType, struct db_event_rec *pdbChanEvent)
{
    
    int db_stat;
        
    if (sScanType == FIRST) {
    	db_stat = d_keyfind(D_EVENT_BEG_TIME_KEY, &dStartTime, CURR_DB);
    	if (db_stat != S_OKAY) {
    		db_stat = d_keynext(D_EVENT_BEG_TIME_KEY, CURR_DB);
    	}	
    	if (db_stat == S_NOTFOUND) return(uiEVENT_NOT_IN_DB_INFO);
        if (db_stat != S_OKAY) goto db_exit;
        
        if (d_recread(pdbChanEvent, CURR_DB) != S_OKAY) goto db_exit;

        return(TRUE);
    }
    
    if (sScanType == NEXT) {
   		db_stat = d_keynext(D_EVENT_BEG_TIME_KEY, CURR_DB);

   		if (db_stat == S_NOTFOUND) return(uiEVENT_NOT_IN_DB_INFO);
        if (db_stat != S_OKAY) goto db_exit;
        
		if (d_recread(pdbChanEvent, CURR_DB) != S_OKAY) goto db_exit;
        return(TRUE);
    }                                                  
    return(FALSE);
    
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "ScanChannelEvents");
    return(uiDB_BAD_ERR);

}    


 
///////////////////////////////////////////////////////////////////////////
//	Name:	ScanChannelEvents
//			Replaces ScanChanEventRecs
//
//	Description:
//	This function allows the user to scan through all event records found
//	in the database.  They are stored in the db by time.  User can
//	select to find the record closest to a specified time or just
//	the next record in the list. This function differs from that above in
//	that event must match the specified station
//
//	Declaration:
//	int ScanChannelEvents(DATE dStartTime,short sScanType, struct db_event_rec *pdbChanEvent, short sSta)
//
//	Input:	dStartTime	start time for event search
//			sScanType	either FIRST event after dStartTime or NEXT event from previous call
//			sSta		number of station events must be associated with
//			
//	Output: pdbChanEvent	event read from database
//
//	Return: TRUE - found an event
//		    uiEVENT_NOT_IN_DB_INFO - event not in db
//			FALSE - Type not FIRST or NEXT
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  23-Sep-1998 SFK		Created by adding capability to specify matching station to argument list
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec) and
//						changed name from ScanChanEventRecs
//	19-Aug-2004	SFK		Changed to check for both channel number and station type
//////////////////////////////////////////////////////////////////
int ScanChannelEvents(DATE dStartTime,short sScanType, struct db_event_rec *pdbChanEvent, IDStruct ChanID)
{
    
    int db_stat;
        
    if (sScanType == FIRST) {
  		db_stat = d_keyfind(D_EVENT_BEG_TIME_KEY, &dStartTime, CURR_DB);
    	if (db_stat != S_OKAY) {
    		db_stat = d_keynext(D_EVENT_BEG_TIME_KEY, CURR_DB);
    	}
	   	if (db_stat == S_NOTFOUND) return(uiEVENT_NOT_IN_DB_INFO);
		if (db_stat != S_OKAY) goto db_exit;
		if (d_recread(pdbChanEvent, CURR_DB) != S_OKAY) goto db_exit;
		if (pdbChanEvent->us_event_sta == ChanID.sStaID) {
			if (pdbChanEvent->us_event_sta_type == ChanID.sStaType) return(TRUE);
		}
		BOOL bMatch = FALSE;
		do {
			db_stat = d_keynext(D_EVENT_BEG_TIME_KEY, CURR_DB);
	   		if (db_stat == S_NOTFOUND) return(uiEVENT_NOT_IN_DB_INFO);
	        if (db_stat != S_OKAY) goto db_exit;
			if (d_recread(pdbChanEvent, CURR_DB) != S_OKAY) goto db_exit;
			if (pdbChanEvent->us_event_sta == ChanID.sStaID) {
				if (pdbChanEvent->us_event_sta_type == ChanID.sStaType) bMatch = TRUE;
			}
		//} while ((pdbChanEvent->us_event_sta != ChanID.sStaID) || (pdbChanEvent->us_event_type != ChanID.sStaType));
		} while (bMatch == FALSE);
        return(TRUE);
    }
    
    if (sScanType == NEXT) {
		do {
   			db_stat = d_keynext(D_EVENT_BEG_TIME_KEY, CURR_DB);

   			if (db_stat == S_NOTFOUND) return(uiEVENT_NOT_IN_DB_INFO);
			if (db_stat != S_OKAY) goto db_exit;
        
			if (d_recread(pdbChanEvent, CURR_DB) != S_OKAY) goto db_exit;
		} while ((pdbChanEvent->us_event_sta != ChanID.sStaID) || (pdbChanEvent->us_event_sta_type != ChanID.sStaType));
        return(TRUE);
    }                                                  
    return(FALSE);
    
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "ScanChannelEvents (by Station)", db_stat);
    return(uiDB_BAD_ERR);
}    


///////////////////////////////////////////////////////////////////////////
//	Name:	GroupChanEvents
//
//	Description:
//	All events in the specified interval are examined and grouped.  They
//	are grouped by whether their beginning and/or ending times fall within
//	a min/max interval.  For events that meet the specified conditions
//	their event records are modified to contain the group time of the
//	first event beginning time in the group.  The first match is kept -- no
//	examination to find if the event might belong better in another group is
//	done.  The group extends until an event is more the ulMaxDelta from the 
//	event comparison time.
//
//	Declaration:
//	int GroupChanEvents(DATE dStartTime, DATE dEndTime, unsigned long ulMinDeltaInSeconds, unsigned long ulMaxDeltaInSeconds, short sType)
//
//	Input:	dStartTime	start time for events to group
//			dEndTime	end time for events to group
//			ulMinDeltaInSeconds		minimum difference allowed between group time and event time
//			ulMaxDeltaInSeconds		maximum difference allowed between group time and event time
//			sType	which "edges" of the group to compare:  START_TO_START, START_TO_END,
//					END_TO_START, END_TO_END
//			
//	Output: none -- although event record's d_event_grp_time is updated in the database
//
//	Return: TRUE - found an event
//		    Error from ScanChanEventRecs,
//	
//  date    /	author	revision
//  -----------------	--------
//  11-Mar-1997 SFK	Created
//  12-May-1998 SFK Added the four types of grouping
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec) and
//						changed name from ScanChanEventRecs
//////////////////////////////////////////////////////////////////
int GroupChanEvents(DATE dStartTime, DATE dEndTime, unsigned long ulMinDeltaInSeconds, unsigned long ulMaxDeltaInSeconds, short sType)
{ 
	int status;			
	DATE dTimeToCompare, dGroupTime;
	CGFmtStr TempStr;
	
	struct db_event_rec dbChanEvent;
	double dMinDelta = (double)ulMinDeltaInSeconds/86400.0;
	double dMaxDelta = (double)ulMaxDeltaInSeconds/86400.0;

	int iDebug = 0;	


	switch (sType) 
	{
	case START_TO_START:	// group events whose beginning times fall within the min/max
		// Algorithm: Set dGroupTime to first event d_event_beg_time_key, check to see if the next
		// event d_event_beg_time_key time falls within delta of dGroupTime.  If no, set dGroupTime to 
		// next event d_event_beg_time_key.  In either case, set d_event_grp_time to dGroupTime.

		pDebugFile->WriteLineToTextFile("GroupChanEvents by START_TO_START\n");

		status = ScanChannelEvents(dStartTime, FIRST, &dbChanEvent);	// Find the closest event to the start
		if (status != 1) 
			return(uiNO_MATCHING_CHAN_EVENTS_WARN);			  
    
		dGroupTime = dbChanEvent.d_event_beg_time_key;
		dTimeToCompare = dbChanEvent.d_event_beg_time_key;
    	
		do {    
    		if ((dTimeToCompare < (dGroupTime + dMinDelta)) || (dTimeToCompare > (dGroupTime + dMaxDelta))) {
				if (dGroupTime < dbChanEvent.d_event_beg_time_key) dGroupTime = dbChanEvent.d_event_beg_time_key;	// new chan group
				TempStr.Printf("\n");
				pDebugFile->WriteLineToTextFile(TempStr);
			}	
			dbChanEvent.d_event_grp_time = dGroupTime;
			status = UpdateChanEvent(&dbChanEvent);			// update the chan group time in chan event rec
			// debug print
			FormatEventRecordForDisplay(&dbChanEvent, &TempStr);
			TempStr.PrintfAppend("\n");
			pDebugFile->WriteLineToTextFile(TempStr);
				
			status = ScanChannelEvents(dStartTime, NEXT, &dbChanEvent);
			dTimeToCompare = dbChanEvent.d_event_beg_time_key;

			iDebug++;
					
		} while ((status == 1) && (dTimeToCompare < dEndTime));    
	break;
	
	case START_TO_END:		// group events whose beginning to ending times fall within the min/max
		// Algorithm: Set dGroupTime to first event d_event_beg_time_key, check to see if the next
		// event d_event_end_time time falls within delta of dGroupTime.  If no, set dGroupTime to 
		// next event d_event_beg_time_key.  In either case, set d_event_grp_time to dGroupTime.

		status = ScanChannelEvents(dStartTime, FIRST, &dbChanEvent);
		if (status != 1) 
			return(uiNO_MATCHING_CHAN_EVENTS_WARN);			   // Find the closest event to the start
    
		dGroupTime = dbChanEvent.d_event_end_time;
		dTimeToCompare = dbChanEvent.d_event_end_time;
		    			
		do {    
    		if ((dTimeToCompare < (dGroupTime + dMinDelta)) || (dTimeToCompare > (dGroupTime + dMaxDelta))) {
				if (dGroupTime < dbChanEvent.d_event_beg_time_key) dGroupTime = dbChanEvent.d_event_beg_time_key;	// new chan group
			}	
			dbChanEvent.d_event_grp_time = dGroupTime;
			status = UpdateChanEvent(&dbChanEvent);			// update the chan group time in chan event rec
				
			status = ScanChannelEvents(dStartTime, NEXT, &dbChanEvent);
			dTimeToCompare = dbChanEvent.d_event_end_time;
					
		} while ((status == TRUE) && (dTimeToCompare < dEndTime));    
	break;	
	
	case END_TO_START:	// group events whose ending to beginning times fall within the min/max
		// Algorithm: Set dGroupTime to first event d_event_end_time, check to see if the next
		// event d_event_beg_time_key time falls within delta of dGroupTime.  If no, set dGroupTime to 
		// next event d_event_end_time.  In either case, set d_event_grp_time to dGroupTime.

		status = ScanChannelEvents(dStartTime, FIRST, &dbChanEvent);
		if (status != 1) 
			return(uiNO_MATCHING_CHAN_EVENTS_WARN);			   
    
		dGroupTime = dbChanEvent.d_event_beg_time_key;
		dTimeToCompare = dbChanEvent.d_event_beg_time_key;
    		
		do {    
			if ((dTimeToCompare < (dGroupTime + dMinDelta)) || (dTimeToCompare > (dGroupTime + dMaxDelta))) {
				if (dGroupTime < dbChanEvent.d_event_end_time) dGroupTime = dbChanEvent.d_event_end_time;
			}	
			dbChanEvent.d_event_grp_time = dGroupTime;
			status = UpdateChanEvent(&dbChanEvent);			// update the chan group time in chan event rec
				
			status = ScanChannelEvents(dStartTime, NEXT, &dbChanEvent);
			dTimeToCompare = dbChanEvent.d_event_beg_time_key;
					
		} while ((status == TRUE) && (dTimeToCompare < dEndTime));    
	break;
	
	case END_TO_END:	// group events whose ending times fall withint the min/max
		// Algorithm: Set dGroupTime to first event d_event_end_time, check to see if the next
		// event d_event_end_time time falls within delta of dGroupTime.  If no, set dGroupTime to 
		// next event d_event_end_time.  In either case, set d_event_grp_time to dGroupTime.

		status = ScanChannelEvents(dStartTime, FIRST, &dbChanEvent);
		if (status != 1) 
			return(uiNO_MATCHING_CHAN_EVENTS_WARN);			   
    
		dGroupTime = dbChanEvent.d_event_end_time;
		dTimeToCompare = dbChanEvent.d_event_end_time;
    
		do {    
			if ((dTimeToCompare < (dGroupTime + dMinDelta)) || (dTimeToCompare > (dGroupTime + dMaxDelta))) {
				if (dGroupTime < dbChanEvent.d_event_end_time) dGroupTime = dbChanEvent.d_event_end_time;
			}	
			dbChanEvent.d_event_grp_time = dGroupTime;
			status = UpdateChanEvent(&dbChanEvent);			// update the chan group time in chan event rec
	
			status = ScanChannelEvents(dStartTime, NEXT, &dbChanEvent);
			dTimeToCompare = dbChanEvent.d_event_end_time;

					
		} while ((status == TRUE) && (dTimeToCompare < dEndTime));    
	break;
	}

	return 1;
}	

///////////////////////////////////////////////////////////////////////////
//	Name:	FormatEventRecordForDisplay
//
//	Description:
//	Form a string that contains all fields in a channel even record
//
//	Declaration:
//	int FormatEventRecordForDisplay(struct db_event_rec *pdbChanEvent)
//
//	Input:	pdbChanEvent	event to display
//			
//	Output: pFormattedString	formatted event
//
//	Return: none
//	
//  date    /	author	revision
//  -----------------	--------
//  27-Mar-2003 SFK	Created
//////////////////////////////////////////////////////////////////
void FormatEventRecordForDisplay(struct db_event_rec *pdbChanEvent, CGFmtStr *pFormattedString)
{
	if (pFormattedString == NULL) 
		return;

	char DateStr[DT_LEN+1];
	char BegTime[DT_LEN+1], EndTime[DT_LEN+1], GrpTime[DT_LEN+1], MaxTime[DT_LEN+1];

	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_beg_time_key, DateStr, BegTime);
	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_end_time, DateStr, EndTime);
	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_grp_time, DateStr, GrpTime);
	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_max_time, DateStr, MaxTime);

	pFormattedString->Printf("%d %s %s %s %s %s %8.1lf %16.0lf %8.1lf %8.1lf %3d %04x %04x %04x %04x",
					pdbChanEvent->us_event_chan,
					DateStr, BegTime, EndTime, GrpTime, MaxTime,
					pdbChanEvent->d_event_max_value,
					pdbChanEvent->d_event_integral_sum,
					pdbChanEvent->d_event_avg_reals_rate,
					pdbChanEvent->d_event_avg_totals_rate,
					pdbChanEvent->s_num_runs,
					pdbChanEvent->sz_event_id,
					pdbChanEvent->us_event_status,
					pdbChanEvent->us_event_status1,
					pdbChanEvent->us_event_status2,
					pdbChanEvent->us_event_type,
					pdbChanEvent->us_event_type1,
					pdbChanEvent->us_event_type2);

}
///////////////////////////////////////////////////////////////////////////
//	Name:	FindDirEventRec
//
//	Description:
//	Determine if the db_dir_event_rec exists in the database.  A matching event
//	is one that has the same beginning time, location and direction -- no other
//	fields in the record are examined.
//	Note : this routine assumes any time adjust is done before data are handed to it.  It
//	"speaks" only in terms of adjusted times.
//
//	Declaration:
//	static int FindChanEventRec(struct db_event_rec *pdbChanEvent)
//
//	Input:	pdbChanEvent	structure containing event to search for
//			
//	Output: none
//
//	Return: TRUE - matching event exists in db
//		    uiSUMMARY_NOT_FOUND_STATUS - event does not exist
//			uiDB_BAD_ERR - unexpected dbVista error
//	
//  date    /	author	revision
//  -----------------	--------
//  22-Oct-1996 SFK		Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
//////////////////////////////////////////////////////////////////
int FindDirEventRec(struct db_dir_event_rec *pdbDirEvent)
{
    struct db_dir_event_rec dbDirEvent1;
    int db_stat;
            
   /* ------------------------------------------------------------------
    *	Look for a direction event matching the beginning time key
    * ----------------------------------------------------------------*/
	db_stat = d_keyfind(D_DIR_EVENT_BEG_TIME_KEY, &(pdbDirEvent->d_dir_event_beg_time_key), CURR_DB);
    if (db_stat == S_NOTFOUND) return(uiSUMMARY_NOT_FOUND_STATUS);
    if (db_stat != S_OKAY) goto db_exit;
       
   /* ------------------------------------------------------------------
    *	Found a direction event with the time.  Now check to see if the location
    *	and direction match.
    * ----------------------------------------------------------------*/
	db_stat = d_recread(&dbDirEvent1, CURR_DB);
	while ((db_stat == S_OKAY) && (pdbDirEvent->d_dir_event_beg_time_key == dbDirEvent1.d_dir_event_beg_time_key)) {
		if ((pdbDirEvent->us_dir_event_location == dbDirEvent1.us_dir_event_location) &&
			(pdbDirEvent->us_dir_event_direction == dbDirEvent1.us_dir_event_direction)) {
			return(TRUE);
		}	
		db_stat = d_keynext(D_DIR_EVENT_BEG_TIME_KEY, CURR_DB);
		if (db_stat == S_OKAY) {
			db_stat = d_recread(&dbDirEvent1, CURR_DB);
		}
	}		
	if ((db_stat == S_NOTFOUND) || (db_stat == S_OKAY)) return(uiSUMMARY_NOT_FOUND_STATUS);

db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "FindDirEventRec", db_stat);
    return(uiDB_BAD_ERR);
}       



///////////////////////////////////////////////////////////////////////////
//	Name:	CreateDirEventRec
//
//	Description:
//	If an event with the specified start time, location and direction does not
//	already exist in the database, create a new direction event record and insert
//	in the db.
//	Note : this routine assumes any time adjust is done before data are handed to it.  It
//	"speaks" only in terms of adjusted times.
//
//	Declaration:
//	static int FindChanEventRec(struct db_event_rec *pdbChanEvent)
//
//	Input:	pdbChanEvent	structure containing event to search for
//			
//	Output: none
//
//	Return: TRUE - event record created or already exists
//		    Error from FindDirEventRec, GetStationOfChan, FindDayRecInMainDb
//			uiDB_BAD_ERR - unexpected dbVista error
//	
//  date    /	author	revision
//  -----------------	--------
//  20-Jul-1995 SFK		Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
//////////////////////////////////////////////////////////////////
int CreateDirEventRec(struct db_dir_event_rec *pdbDirEvent)
{
    int db_stat;
           
   /* ------------------------------------------------------------------
    *	Check if the event is already in the db
    * ----------------------------------------------------------------*/
    int status = FindDirEventRec(pdbDirEvent);
    if (status != uiSUMMARY_NOT_FOUND_STATUS) 
		return(status);	//either found it or db error
       
   /* ------------------------------------------------------------------
    *	The record is not in the db, add it now.  Find the day associated
    *	with the FromChan[0] and attach it there (need this to delete all
    *	stuff associated with the day)
    * ----------------------------------------------------------------*/
	status = glDbDay.FindDayRecInMainDb(pdbDirEvent->us_dir_event_from_sta[0], pdbDirEvent->d_dir_event_beg_time_key);
    if (status != 1) 
		return (status);         
    
    if (d_setor(DAY_TO_DIR_EVENT_SET, CURR_DB) != S_OKAY) 
		goto db_exit;
    db_stat = d_fillnew(DB_DIR_EVENT_REC, (char *)pdbDirEvent, CURR_DB); 
	if (db_stat!= S_OKAY) 
		goto db_exit;
    if (d_connect(DAY_TO_DIR_EVENT_SET, CURR_DB) != S_OKAY) 
		goto db_exit;  

	return 1;
	
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "CreateDirEventRec");
    return(uiDB_BAD_ERR);
}



/*===========================================================================
 *
 *  Name	 :  UpdateDirEvent
 *
 *  Purpose	 :  Find the event and write the new recordstatus.
 *
 *
 *  Return value :	TRUE - event record updated
 *				    Error from FindChanEventRec
 *					uiDB_BAD_ERR	- unexpected db error
 *
 *  Special notes:	if TRUE, curr_rec = event just updated
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  20-Jul-1995 SFK		Created
 *  21-Oct-96	SFK		Added update of group event time
 *
===========================================================================*/
int UpdateDirEvent(struct db_dir_event_rec *pdbDirEvent)
{
   /* ------------------------------------------------------------------
    *	Check if the event is in the db
    * ----------------------------------------------------------------*/
    int status = FindDirEventRec(pdbDirEvent);
    if (status != 1) 
		return(status);
       
   /* ------------------------------------------------------------------
    *	Read the old record and then update with the new record with a new
    *	end time and add the channels to the choices.
    * ----------------------------------------------------------------*/
    struct db_dir_event_rec dbDirEvent1;
   	if (d_recread((char *)&dbDirEvent1, CURR_DB) != S_OKAY) 
		goto db_exit;
   	// write the new record -- could change anything except matching conditions of start time, location , direction
	if (d_recwrite((char *)pdbDirEvent, CURR_DB) != S_OKAY) 
		goto db_exit;  

	return 1;
	
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "UpdateDirEvent");
    return(uiDB_BAD_ERR);
}
/*===========================================================================
 *
 *  Name	 :  ReadDirEvent
 *
 *  Purpose	 :  Find the event summary record and read it
 *
 *  Return value :	TRUE
 *				    Error from FindDirEventRec
 *					uiDB_BAD_ERR	- unexpected db error
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  22-Oct-96	SFK		Created
 *
===========================================================================*/
int ReadDirEvent(struct db_dir_event_rec *pdbDirEvent)
{ 
   /* ------------------------------------------------------------------
    *	Check if the event is in the db and read it.
    * ----------------------------------------------------------------*/
    int status = FindDirEventRec(pdbDirEvent);
    if (status != 1) 
		return(status);
   	if (d_recread((char *)pdbDirEvent, CURR_DB) != S_OKAY) 
		goto db_exit;

   	return 1;
   	
db_exit:   	
    RadReviewMsg(uiDB_BAD_ERR, "ReadDirEvent");
    return(uiDB_BAD_ERR);
}
///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStatisticsForDirEventsByChannel
//			Replaces NumDirEventsInDbInterval
//
//	Description:
//	Count the number of direction events for the specified channel
//	in the specified interval in the database.  A direction event
//	is said to match if its ul_dir_event_beg_time_key field value is
//	within the dStartTime to dEndTime interval and the 
//	us_dir_event_from_chan channel is the same as sChan.
//	Note : this routine assumes any time adjust is done before data 
//	are handed to it.  It "speaks" only in terms of adjusted times.
//
//	Declaration:
//	int GetDataStatisticsForDirEventsByChannel(short sChan, DATE dStartTime, DATE dEndTime, unsigned long *pulNumEvents)
//
//	Input:	sChan		channel want to find events for
//			dStartTime	interval start to search for events
//			dEndTime	interval end to search for events
//			
//	Output: pulNumEvents	number of events that match the conditions
//
//	Return: TRUE - events counted
//			Error from GetStationOfChan, FindIntervalInDb, FindDayRec
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  31-MAY-1995  SFK	Created from old GRAND Review getpts.c
//	24-Mar-1998	 SFK	Added concept of specifying which type of events to count
//	05-Oct-1998  SFK	Added GRAPH EVENTS
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec) and
//						changed name from NumDirEventsInDbInterval
//	29-Sep-2003	 SFK	Added IDStruct to call
//////////////////////////////////////////////////////////////////
int GetDataStatisticsForDirEventsByChannel(IDStruct ChanID, DATE dStartTime, DATE dEndTime, unsigned long *pulNumEvents)
{
    *pulNumEvents = 0;
    if (dEndTime < dStartTime) 
		return 1;

    struct db_dir_event_rec dbDirEvent;
    int  db_stat;           
    DATE dCurrentDay;	
    DATE dActualEndTime;
    DATE dActualStartTime;
    unsigned long ulDum;
    BOOL bDone = FALSE;
	BOOL bLastDay = FALSE;  
    BOOL ThisDayDone;
        
	int status = GetDataStatistics(ChanID, true, dStartTime, dEndTime, &dActualStartTime, &dActualEndTime, &ulDum);
	if (status != 1) 
		return(status);

	dCurrentDay = floor(dActualStartTime);
	status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dCurrentDay);	// position curr rec at first day's record
    if (status != 1) 
		return(status);
    
   /* ------------------------------------------------------------------
    *	Look through the events for all days and find those that occur
    *	at or after the start time and at or before the end time.
    * ----------------------------------------------------------------*/
    do 
	{
	    db_stat = d_setor(DAY_TO_DIR_EVENT_SET, CURR_DB); 	/* select Event set */
	    if (db_stat != S_OKAY) goto db_exit;
	    
    	ThisDayDone = FALSE;
	    db_stat = d_findfm(DAY_TO_DIR_EVENT_SET, CURR_DB); 	/* see if any events */
	    if (db_stat == S_EOS) {
	    	ThisDayDone = TRUE;
	    }
	    else {	
	    	if (db_stat != S_OKAY) goto db_exit;
	    }	
	                                             
		if (ThisDayDone == FALSE) {	// look thru the direction events for this day                                             
		   	do {                                            /* count the events */
				db_stat = d_recread(&dbDirEvent, CURR_DB);
				if (db_stat != S_OKAY) goto db_exit;
				if ((dbDirEvent.d_dir_event_beg_time_key >= dActualStartTime) &&
				   	(dbDirEvent.d_dir_event_beg_time_key <= dActualEndTime) && 
					(dbDirEvent.us_dir_event_from_chan[0] == (unsigned short)ChanID.sChanID)) {
					(*pulNumEvents)++;
				}
				db_stat = d_findnm(DAY_TO_DIR_EVENT_SET, CURR_DB);	// move on to next direction event in day
			} while (db_stat == S_OKAY);
			if (db_stat != S_EOS) goto db_exit;
		}	
		
		if ((int)dCurrentDay == (int)dActualEndTime) bDone = TRUE;	// is this the last day in the interval
		if (!bDone) 
		{	// no, find the next day
			do 
			{
				dCurrentDay += 1.0;				// go on to the next day
				if ((int)dCurrentDay == (int)dActualEndTime) 
					bLastDay = TRUE;
				status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dCurrentDay);
	    	} while ((status != 1) && (bLastDay == 0)); 	
	    }	
	} while (!bDone);
    
	return 1;
	
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "GetDataStatisticsForDirEventsByChannel", db_stat);
    return(uiDB_BAD_ERR);
}       



///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByChannelDirEvents
//			Replaces ReadDirEventsInDbInterval
//
//	Description:
//	Read the number of direction events for the specified channel
//	in the specified interval in the database.  A direction event
//	is said to match if its ul_dir_event_beg_time_key field value is
//	within the dStartTime to dEndTime interval and the 
//	us_dir_event_from_chan channel is the same as sChan.
//	Note : this routine assumes any time adjust is done before data 
//	are handed to it.  It "speaks" only in terms of adjusted times.
//
//	Declaration:
//	int GetDataByChannelDirEvents(short sChan, DATE dStartTime, DATE dEndTime, unsigned long ulNumEventsToRead, unsigned long *pulNumEventsRead, struct db_dir_event_rec *pDirEvents)
//
//	Input:	sSta		station want to find events for all channels
//			dStartTime	interval start to search for events
//			dEndTime	interval end to search for events
//			ulNumEventsToRead	number of events expected to read that have had storage allocated
//			
//	Output: pulNumEventsRead	number of events actually read from db
//			pDirEvents			array where events read are stored
//
//	Return: TRUE - events read
//			Error from GetStationOfChan, FindIntervalInDb, FindDayRec
//			uiDB_BAD_ERR	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  20-Jul-1995   SFK	 Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec) and
//						changed name from ReadDirEventsInDbInterval
//	29-Sep-2003	SFK		Added IDStruct to call.
//////////////////////////////////////////////////////////////////
int GetDataByChannelDirEvents(IDStruct ChanID, DATE dStartTime, DATE dEndTime, unsigned long ulNumEventsToRead, unsigned long *pulNumEventsRead, struct db_dir_event_rec *pDirEvents)
{
	struct db_dir_event_rec dbDirEvent;
	unsigned long  i;
	int  db_stat, status;
	DATE dCurrentDay;		    
	DATE dActualEndTime;
	DATE dActualStartTime;
	unsigned long ulDum;
	BOOL bEventsInDay;
	BOOL bDone = FALSE;
	BOOL bLastDay = FALSE;   
    
	*pulNumEventsRead = 0;  
	if (ulNumEventsToRead ==0) return(TRUE);
	if (dEndTime < dStartTime) return(TRUE);
           
	status = GetDataStatistics(ChanID, true, dStartTime, dEndTime, &dActualStartTime, &dActualEndTime, &ulDum);
	if (status != 1) return(status);
	dCurrentDay = floor(dActualStartTime);
    
   /* ------------------------------------------------------------------
    *	Look through the events for all days and read the first ulNumEvents
    *	events that occur at or after the start time.
    * ----------------------------------------------------------------*/
	i = 0;
	do 
	{
	    db_stat = d_setor(DAY_TO_DIR_EVENT_SET, CURR_DB); 	/* select Event set */
	    if (db_stat != S_OKAY) goto db_exit;
	    
	    bEventsInDay = TRUE;
	    db_stat = d_findfm(DAY_TO_DIR_EVENT_SET, CURR_DB); 	/* see if any events */
	    if (db_stat == S_EOS) {
	    	bEventsInDay = FALSE;
	    }
	    else {	
	    	if (db_stat != S_OKAY) goto db_exit;
	    }	
	                                          
		if (bEventsInDay == TRUE) {
		   	do {                                            /* count the events */
				db_stat = d_recread(&dbDirEvent, CURR_DB);
				if (db_stat != S_OKAY) goto db_exit;
				if ((dbDirEvent.d_dir_event_beg_time_key >= dActualStartTime) &&
					(dbDirEvent.d_dir_event_beg_time_key <= dActualEndTime) && 
					(dbDirEvent.us_dir_event_from_chan[0] == (unsigned short)ChanID.sChanID)) {
					pDirEvents[i] = dbDirEvent;
					i++;
				}	 
				db_stat = d_findnm(DAY_TO_DIR_EVENT_SET, CURR_DB);
			} while ((db_stat == S_OKAY) && (i < ulNumEventsToRead));
		}	
		
		if ((int)dCurrentDay == (int)dActualEndTime) bDone = TRUE;	// last day in interval?
		if (!bDone) 
		{	// no, find the next day
			do 
			{
				dCurrentDay += 1.0;				/* more days to search */
				if ((int)dCurrentDay == (int)dActualEndTime) 
					bLastDay = TRUE;
				status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dCurrentDay);
	    	} while ((status != 1) && (bLastDay == 0)); 	
	    }	
	} while (!bDone); 
	
	*pulNumEventsRead = i;
    
	return 1;
	
db_exit:
	RadReviewMsg(uiDB_BAD_ERR, "GetDataByChannelDirEvents", db_stat);
	return(uiDB_BAD_ERR);
}       

/* <f> */
/*===========================================================================
 *
 *  Name	 :  DeleteDbDirEvents
 *
 *  Purpose	 :  Find all records associated with specified facility and
 *			    delete them from the database.
 *
 *  Return value :  TRUE - all events for facility deleted
 *					FALSE	- facility not found in db
 *					uiDB_BAD_ERR	- unexpected db error
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  26-Jan-1996 SFK	Created
 *	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
 *
===========================================================================*/
int DeleteDbDirEvents(short sFacNum)

{
    DB_ADDR dbStaAdr, dbDayAdr;	/* db addresses */
    int db_stat;
  
   /* ------------------------------------------------------------------
    *	Check if the facility is in the database
    * ----------------------------------------------------------------*/
	int status = FacilityExistsInMainDb(sFacNum);
    if (status != 1) 
		return(status);

   /* ------------------------------------------------------------------
    *	Next find all the stations in this facility.  Then for each
    *   station look for all the days for it and finally delete all
    *   the events associated with every day.
    * ----------------------------------------------------------------*/

    if (d_setor(FACILITY_TO_STATION_SET, CURR_DB) != S_OKAY) goto db_exit;
    for (db_stat = d_findfm(FACILITY_TO_STATION_SET, CURR_DB);
    	 db_stat == S_OKAY;
		 db_stat = d_findnm(FACILITY_TO_STATION_SET, CURR_DB)) {
	
		if (d_crget(&dbStaAdr, CURR_DB)!= S_OKAY) goto db_exit;	/* save station adr for later */
		if (d_setor(STATION_TO_DAY_SET, CURR_DB)!= S_OKAY) goto db_exit; 
	    for (db_stat = d_findfm(STATION_TO_DAY_SET, CURR_DB);
	    	 db_stat == S_OKAY;
			 db_stat = d_findnm(STATION_TO_DAY_SET, CURR_DB)) {
    	
			if (d_crget(&dbDayAdr, CURR_DB)!= S_OKAY) goto db_exit;	/* save day adr for later */
			if (d_setor(DAY_TO_DIR_EVENT_SET, CURR_DB)!= S_OKAY) goto db_exit; 
		    for (db_stat = d_findfm(DAY_TO_DIR_EVENT_SET, CURR_DB);
		    	 db_stat == S_OKAY;
				 db_stat = d_findnm(DAY_TO_DIR_EVENT_SET, CURR_DB)) { 
			
				db_stat = d_disdel(CURR_DB);
	   			if (db_stat != S_OKAY) goto db_exit;
			}
			if (d_crset(&dbDayAdr, CURR_DB)!= S_OKAY) goto db_exit;	/* restore day adr */
	    }
		if (d_crset(&dbStaAdr, CURR_DB)!= S_OKAY) goto db_exit;	/* restore sta adr */
    }	
	    	
    return(TRUE);

db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "DeleteDbDirEvents");
    return(uiDB_BAD_ERR);
}


/* <f> */
/*===========================================================================
 *
 *  Name	 :  DeleteDbDirEvent
 *
 *  Purpose	 :  Find the specified radiation event record and delete it from
 *				the database
 *
 *  Return value :  TRUE - all events for facility deleted
 *					Error from FindFacRec
 *					uiDB_BAD_ERR	- unexpected db error
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  26-Jan-1996 SFK	Created
  *	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
 *
===========================================================================*/
int DeleteDbDirEvent(struct db_dir_event_rec *pdbDirEvent)

{
    int status = FindDirEventRec(pdbDirEvent);	// event in db?
    if (status != 1) 
		return(status);
    
    //if (d_disdel(CURR_DB)!= S_OKAY) 
	//	goto db_exit; // delete it

    if (d_disdel(CURR_DB) == S_OKAY) 
		return 1;

//db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "DeleteDbDirEvent");
    return uiDB_BAD_ERR;
}


/* <f> */
/*===========================================================================
 *
 *  Name	 :  ScanDirEventRecs
 *
 *  Purpose	 :  This allows the user to scan through all event summary records found
 *				in the database.  They are stored in the db by time.  User can
 *				select to find the record closest to a specified time or just
 *				the next record in the list.
 *
 *  Return value :  TRUE - found an event
 *				    uiSUMMARY_NOT_FOUND_STATUS - event not in db
 *					uiUNKNOWN_EVENT_TYPE_ERR - Type not FIRST or NEXT
 *					uiDB_BAD_ERR	- unexpected db error
 *
 *  Special notes:  if TRUE, curr_rec = event looking for
 *					if uiSUMMARY_NOT_FOUND_STATUS, curr_rec = day where event should attach to
 *					uiUNKNOWN_EVENT_TYPE_ERR - didn't ask for first or next
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  20-Jul-1995 SFK	Created
 *	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
 *
===========================================================================*/
int ScanDirEventRecs(DATE dStartTime,short sType, struct db_dir_event_rec *pdbDirEvent)
{
    int db_stat;
        
    if (sType == FIRST) {
    	db_stat = d_keyfind(D_DIR_EVENT_BEG_TIME_KEY, &dStartTime, CURR_DB);
    	if (db_stat != S_OKAY) {
    		db_stat = d_keynext(D_DIR_EVENT_BEG_TIME_KEY, CURR_DB);
    	}	
    	if (db_stat == S_NOTFOUND) return(uiSUMMARY_NOT_FOUND_STATUS);
        if (db_stat != S_OKAY) goto db_exit;
        
        if (d_recread(pdbDirEvent, CURR_DB)!= S_OKAY) goto db_exit;
        return(TRUE);
    }
    
    if (sType == NEXT) {
   		db_stat = d_keynext(D_DIR_EVENT_BEG_TIME_KEY, CURR_DB);
   		if (db_stat == S_NOTFOUND) return(uiSUMMARY_NOT_FOUND_STATUS);
        if (db_stat != S_OKAY) goto db_exit;
        
        if (d_recread(pdbDirEvent, CURR_DB)!= S_OKAY) goto db_exit;
        return(TRUE);
    }                                                  
    return(uiUNKNOWN_EVENT_TYPE_ERR);
    
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "ScanDirEventRecs");
    return(uiDB_BAD_ERR);

}    
 

/* <f> */
/*===========================================================================
 *
 *  Name	 :  FindDirEventAssociatedWithChanEvent
 *
 *  Purpose	 :  Given a channel event, scan through the direction events
 *				in the database looking for a matching direction event.
 *				A match is defined as meeting these conditions:
 *					1) beginning chan event time >= beginning dir event time AND
 *						ending chan event time <= ending dir event time.
 *					2) at least one of the direction channel pairs must be at the same
 *						station as the channel event station
 *
 *  Return value :	TRUE
 *				    Error from FindDirEventRec
 *					uiDB_BAD_ERR	- unexpected db error
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  22-Oct-96	SFK		Created
 *	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
 *
===========================================================================*/
int FindDirEventAssociatedWithChanEvent(struct db_event_rec *pdbEvent, BOOL *pbMatchingEvent, struct db_dir_event_rec *pdbDirEvent)
{
    int status, db_stat; 
	BOOL bEventsInDay = TRUE;
	DATE dEventMidTime;
	int i;
	struct db_dir_event_rec dbDirTemp;
	BOOL bMatchingEvent;

	*pbMatchingEvent = FALSE;
   /* ------------------------------------------------------------------
    *	Find the mid time of the channel event
    * ----------------------------------------------------------------*/
	dEventMidTime = (pdbEvent->d_event_beg_time_key + pdbEvent->d_event_end_time)/2;
       
   /* ------------------------------------------------------------------
    *	Scan through all the direction events for the day containing the channel event
    * ----------------------------------------------------------------*/
    status = glDbDay.FindDayRecInMainDb(pdbEvent->us_event_sta, pdbEvent->d_event_beg_time_key);
    if (status != 1) 
		return (status);         
	// Note : eventually handle the case of direction event starting in one day and ending in another day.
    
   /* ------------------------------------------------------------------
    *	Look through the events for all days and find ones whose times
	*	include the range of the channel event.
	*	Once find a direction event that matches the time, then
	*	look through to see if any of the channels in the matching pairs
	*	match the channel event's station.
    * ----------------------------------------------------------------*/
	db_stat = d_setor(DAY_TO_DIR_EVENT_SET, CURR_DB); 	/* select Event set */
	if (db_stat != S_OKAY) goto db_exit;
	db_stat = d_findfm(DAY_TO_DIR_EVENT_SET, CURR_DB); 	/* see if any events */
	if (db_stat == S_EOS) {
	    bEventsInDay = FALSE;
	}
	else {	
	    if (db_stat != S_OKAY) goto db_exit;
	}	
	                                         
	if (bEventsInDay == TRUE) {	                                             
		do {                                            /* look through the events */
			db_stat = d_recread(pdbDirEvent, CURR_DB);
			if (db_stat != S_OKAY) goto db_exit;
			if ((pdbDirEvent->d_dir_event_beg_time_key <= dEventMidTime) &&
				(pdbDirEvent->d_dir_event_end_time >= dEventMidTime)) {	// check if times okay
				for (i=0; i< MAX_PAIRS; i++) {
					if (*pbMatchingEvent == FALSE) {
						if (pdbDirEvent->us_dir_event_from_sta[i] == pdbEvent->us_event_sta) {
							*pbMatchingEvent = TRUE;
							break;
						}
						if (pdbDirEvent->us_dir_event_from_sta[i] == pdbEvent->us_event_sta) {
							*pbMatchingEvent = TRUE;
							break;
						}
					}
				}
			}
			if (*pbMatchingEvent == FALSE) db_stat = d_findnm(DAY_TO_DIR_EVENT_SET, CURR_DB);
		} while ((db_stat == S_OKAY) && (*pbMatchingEvent == FALSE));
		if (db_stat == S_EOS) return(TRUE);		// no events match in this day
		if (db_stat != S_OKAY) goto db_exit;
	}
	// have found a direction event matching the event time.  Now look for special case of
	// another direction event matching the event.
	do {
		db_stat = d_findnm(DAY_TO_DIR_EVENT_SET, CURR_DB);
		if (db_stat != S_OKAY) return(TRUE);	// use the event you already found
		db_stat = d_recread(&dbDirTemp, CURR_DB);	// read the next direction event
		if (db_stat != S_OKAY) goto db_exit;
		if ((dbDirTemp.d_dir_event_beg_time_key <= dEventMidTime) &&
			(dbDirTemp.d_dir_event_end_time >= dEventMidTime)) {	// check if times okay
			bMatchingEvent = FALSE;
			for (i=0; i< MAX_PAIRS; i++) {
				if (pdbDirEvent->us_dir_event_from_sta[i] == pdbEvent->us_event_sta) {
					bMatchingEvent = TRUE;
					break;
				}
			if (pdbDirEvent->us_dir_event_to_sta[i] == pdbEvent->us_event_sta) {
					bMatchingEvent = TRUE;
					break;
				}
			}
			// if another direction event also falls within the range and has a different direction
			// then you have found the special case of both directions.  Modify the direction information
			// returned.
			if ((bMatchingEvent == TRUE) && (dbDirTemp.us_dir_event_direction != pdbDirEvent->us_dir_event_direction)) {
				pdbDirEvent->us_dir_event_direction = BOTH_DIRECTIONS;
				return(TRUE);
			}
		}
	} while ((dbDirTemp.d_dir_event_beg_time_key <= dEventMidTime) &&
			(dbDirTemp.d_dir_event_end_time >= dEventMidTime));

   	return(TRUE);
   	
db_exit:   	
    RadReviewMsg(uiDB_BAD_ERR, "FindDirEventAssociatedWithChanEvent", db_stat);
    return(uiDB_BAD_ERR);
}


                                                                  