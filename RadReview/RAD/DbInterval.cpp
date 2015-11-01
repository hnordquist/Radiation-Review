///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbInterval.cpp     
//Primary Author: Shirley Klosterbuer
///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		GetDataStatistics - Replaces FindIntervalInDb
//		CvtTimeToDayStart
//		SameDay
//
//		All times are adjusted times
//
///////////////////////////////////////////////////////////////////////////
#include "RAD.H"
#include "AdjustTime.h"
#include "DbDay.h"
#include "DbInterval.h"
#include "DbTime.h"
#include "Interval.h"
#include "FacilityConfig.h"
#include "TimeAlign.h"	// rad 3.00
#include "MyDateTime.h"
#include "DbInterval.h"		// must be after FacilityConfig.h
#include "TypeDefinitions.h"
#include "EOSSImport.h"
#include "BinaryImport.h"
#include <math.h>

extern short glsFacNum; //KM
extern CEOSSImport *pglEOSSImport;
extern CBinaryImport *pglBinaryImport;
extern CDbDay glDbDay;

static int FindDayContainingClosestStartTime(short sSta, DATE dRequestedStartTime, DATE dEndTime, DATE *pdActualStartTime);
static int FindDayContainingClosestEndTime(short sSta, DATE dStartTime, DATE dRequestedEndTime, DATE *pdActualEndTime);

///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStatistics
//			Replaces FindIntervalInDb
//
//	Description:
//	Calculate the number of data points	in the database for the specified
//	station during the requested interval. Determine the first and last
//	actual times in the db in the interval for the station.
//	All requests are assumed to be in adjusted times.  Data are stored in 
//	the database in unadjusted times.  So data values must be properly
//	adjusted before calls to the respective database holding the data.
//
//	Declaration:
//	int GetDataStatistics(short sSta, DATE dStartTime, DATE dEndTime, DATE *pdActualStartTime, DATE *pdActualAdjEndTime, unsigned long *pulNumPts)
//
//	Input:	sSta		number of station want to find statistics for
//			dStartTime	start of search interval
//			dEndTime	end of search interval
//			
//	Output: pdActualStartTime	timestamp of first data point within interval
//			pdActualAdjEndTime		timestamp of last data point within interval
//			pulNumPts			number of data points within interval
//
//	Return: TRUE - day and associated records deleted
//			FAIL	- day not found in db
//			DB_BAD	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  31-MAY-1995  SFK	Created from old GRAND Review getpts.c
//	09-Feb-1996	 SFK	Fixed problem exceeding RAW_DATA_RECS_PER_DB_RECORD in index
//	22-Oct-1996  SFK	Changed to also return actual end time.
//	21-Mar-2001  SFK	Version 2.07 Added hooks into data residing in Access database
//  01-Feb-2002  SFK	Integrated with GRAND Com  Version 2.10
//	20-May-2002	 SFK	Integrated with VIFM COM
//	07-Aug-2002  SFK	Replaced all calls to FindIntervalInDb to this function.
//////////////////////////////////////////////////////////////////
// this returns time-aligned data
// supports new FacMgr 9-18-2003
int GetDataStatistics(
	IDStruct StaID, 
	BOOL bStation, 
	DATE dAdjStartTime, 
	DATE dAdjEndTime, 
	DATE *pdActualAdjStartTime, 
	DATE *pdActualAdjEndTime, 
	unsigned long *pulNumPts)
{
    struct db_day_rec	dbCurrentDay;		// day presently reading 
    struct db_float_data_rec 	dbfData; 	// record containing raw float data 
    struct db_double_data_rec 	dbdData; 	// record containing raw double data 

    int  i, status = 0;
	DATE dCurrentDay;		    // day you want to find in database in jul secs 
    unsigned long ulPtsInDayBeforeStart;
	DATE dStart, dEnd;
    DATE dActualEndTime, dActualStartTime;
    unsigned long ulPtsSoFar;
    BOOL bOnlyOneDay;   
    CGFmtStr msg;
	short sChan;
    
    *pulNumPts = 0;
    *pdActualAdjStartTime = 0.0;
	*pdActualAdjEndTime = 0.0;

	// GRAND or BOT/VIFM or EOSS data in separate COM component that handles time adjustments
	if (pglTimeAlign->COMDataType(StaID.sStaType)) 
	{
		if (bStation) 
		{
			sChan = -1;
		}
		else 
		{
			sChan = StaID.sChanID;
		}

		if (StaID.sStaType == EOSS_TYPE)
		{
			//Don't use TimeAlign for EOSS.
			//Use the CEOSSImport class directly.
			status = pglEOSSImport->GetDataStatistics(
				glsFacNum, StaID.sStaID, sChan, 
				dAdjStartTime, dAdjEndTime, 
				pdActualAdjStartTime, pdActualAdjEndTime, 
				pulNumPts);
		}
		else if (StaID.sStaType == BINARY_TYPE)
		{
			status = pglBinaryImport->GetDataStatistics(
				glsFacNum, StaID.sStaID, sChan, 
				dAdjStartTime, dAdjEndTime, 
				pdActualAdjStartTime, pdActualAdjEndTime, 
				pulNumPts);
		}
		else
		{
			status = pglTimeAlign->GetDataStatistics(
				glsFacNum, StaID.sStaID, 
				sChan, dAdjStartTime, dAdjEndTime, 
				pdActualAdjStartTime, pdActualAdjEndTime, 
				pulNumPts, StaID.sStaType); 
		}
		if (StaID.sStaType != -1) 
			return(status);
	}

	// now handle the data in the old dbVista Database which is always on a channel basis

	// since data are stored in non-adjusted format in database, unadjust the calling times now
	dStart = UnadjustTimestamp(StaID, dAdjStartTime);
	dEnd = UnadjustTimestamp(StaID, dAdjEndTime);

	// all other data types are still in the main dbVista database
	
    // find the beginning and end times in the requested interval
	status = FindDayContainingClosestEndTime(StaID.sStaID, dStart, dEnd, &dActualEndTime);
    if (status != TRUE) 
		return(status);

	status = FindDayContainingClosestStartTime(StaID.sStaID, dStart, dActualEndTime, &dActualStartTime);
    if (status != TRUE) 
		return(status);
    
	// handle case of one point
    if (dActualStartTime == dActualEndTime) 
	{
		*pdActualAdjStartTime = AdjustTimestamp(StaID, dActualStartTime);
		*pdActualAdjEndTime = *pdActualAdjStartTime;
		*pulNumPts = 1;
    	return(TRUE);
    }
	
	// these cases should never happen; if they do return 0 points.
    if ((dEnd < dActualStartTime) || (dStart > dActualEndTime)) 
	{
		*pdActualAdjStartTime = dAdjStartTime;
		*pdActualAdjEndTime = *pdActualAdjStartTime;
    	return 1;
    }	
    
    dCurrentDay = dActualStartTime;
   
	//	find the start time within its multiple record
	if (StaID.sDataType == FLOAT_TYPE) 
	{
		status = FindDbDataRecContainingTime(StaID, dActualStartTime, SEARCH_START, &dbfData);	// read record containing start time
		if (status != TRUE) 
			return(status);
		
		i = 0;
		while ((dbfData.d_fdata_time[i] < dActualStartTime) && (i < (RAW_DATA_RECS_PER_DB_RECORD-1)) &&
		   (dbfData.d_fdata_time[i] != 0.0)) 
		{			// look for the actual start in the multiple record.
			i++;
		}

	    if (d_setmr(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) 
			goto db_exit;

	    if (d_csoread(DAY_TO_FLOAT_DATA_SET, UL_DAY_TOTAL_PTS, (char *)pulNumPts, CURR_DB) != S_OKAY) 
			goto db_exit;

	    if ((dbfData.d_fdata_time[i] == 0.0) && (i != 0))
			i--; // back up if hit the 0s at end of day as they signal no data
		//{
		//	if (i != 0) i--; // back up if hit the 0s at end of day as they signal no data
		//}
	    ulPtsInDayBeforeStart = dbfData.ul_fdata_pts_so_far + (long)i;
	    *pulNumPts = *pulNumPts - ulPtsInDayBeforeStart;  
	    dActualStartTime = dbfData.d_fdata_time[i];
	}	

	// find the start time within its multiple record
	if (StaID.sDataType == DOUBLE_TYPE) {
		status = FindDbDataRecContainingTime(StaID, dActualStartTime, SEARCH_START, &dbdData); // read record containing start timre
		if (status != TRUE) return(status);
    			
		i = 0;
		while ((dbdData.d_ddata_time[i] < dActualStartTime) && (i < (RAW_DATA_RECS_PER_DB_RECORD-1)) &&
		   (dbdData.d_ddata_time[i] != 0.0)) {			// look for the actual start in the multiple record.		
			i++;
		}
		
	    if (d_setmr(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
	    if (d_csoread(DAY_TO_DOUBLE_DATA_SET, UL_DAY_TOTAL_PTS, (char *)pulNumPts, CURR_DB) != S_OKAY) goto db_exit;
	    if (dbdData.d_ddata_time[i] == 0) if (i != 0.0) i--; // back up if hit the 0s at end of day as they signal no data
		ulPtsInDayBeforeStart = dbdData.ul_ddata_pts_so_far + (long)i;
	    *pulNumPts = *pulNumPts - ulPtsInDayBeforeStart;  
	    dActualStartTime = dbdData.d_ddata_time[i];
	}	
	
	if (dEnd < dActualStartTime) { // requested interval before any real data in day
		*pdActualAdjStartTime = AdjustTimestamp(StaID, dActualStartTime);
		*pdActualAdjEndTime = AdjustTimestamp(StaID, dActualEndTime);
		*pulNumPts = 0;
		return(TRUE);
	}			
	
	// finished with start day, now see if more than one day in the interval
	bOnlyOneDay = FALSE;
	if ((int)dActualStartTime == (int)dActualEndTime) bOnlyOneDay = TRUE;

	// there is more than one day so add the points in each dat to the total
    if (!bOnlyOneDay) 
	{
		bool bDone = false;
		do 
		{  // 09-24-98 rewrite handle case of a missing day
           	dCurrentDay = dCurrentDay + 1.0;			// go on to the next day in the db

			if ((int)dCurrentDay == (int)dActualEndTime)
			{
				bDone = true;
			}
			else 
			{									// not on the last day
				status = glDbDay.FindDayRecInMainDb(StaID.sStaID, dCurrentDay);
	    		if (status == TRUE) 
				{
					if (d_recread((char*)&dbCurrentDay, CURR_DB) != S_OKAY) 
						goto db_exit;

			    	*pulNumPts = *pulNumPts + dbCurrentDay.ul_day_total_pts;	// add the number of pts in this day to the total
				}
	    	}
	    } while (!bDone);
    }


	// on the end day, find the end time within its multiple record
	if (StaID.sDataType == FLOAT_TYPE) {
		status = FindDbDataRecContainingTime(StaID, dActualEndTime, SEARCH_END, &dbfData);
		if (status != TRUE) return(status);
					                        
	    i = 0;
	    while ((dbfData.d_fdata_time[i] <= dActualEndTime) && (i < (RAW_DATA_RECS_PER_DB_RECORD-1)) &&
		   (dbfData.d_fdata_time[i] != 0.0)) {
			i++;
	    }
		// find the actual end time.  If at the end of the day record (==0) or
		// have an exact match, then the end time is that of the previous record
		// For non-exact matches, use the time just beyond the end point but don't count that point.
		if ((dbfData.d_fdata_time[i] == 0.0) || (dbfData.d_fdata_time[i] == dActualEndTime)){ // back up if hit the 0s at end of day
			dActualEndTime = dbfData.d_fdata_time[i-1];
		}
		else {
			dActualEndTime = dbfData.d_fdata_time[i];
		}
        ulPtsSoFar = dbfData.ul_fdata_pts_so_far;
    }    

    // on the end day, find the end time within its multiple record
	if (StaID.sDataType == DOUBLE_TYPE) {
		status = FindDbDataRecContainingTime(StaID, dActualEndTime, SEARCH_END, &dbdData);
		if (status != TRUE) return(status);

	    i = 0;
	    while ((dbdData.d_ddata_time[i] <= dActualEndTime) && (i < (RAW_DATA_RECS_PER_DB_RECORD-1)) &&
		   (dbdData.d_ddata_time[i] != 0.0)) {
			i++;
	    }
		// find the actual end time.  If at the end of the day record (==0) or
		// have an exact match, then the end time is that of the previous record
		// For non-exact matches, use the time just beyond the end point but don't count
		// that point.
		if ((dbdData.d_ddata_time[i] == 0.0) || (dbdData.d_ddata_time[i] == dActualEndTime)){ // back up if hit the 0s at end of day
			dActualEndTime = dbdData.d_ddata_time[i-1];
		}
		else {
			dActualEndTime = dbdData.d_ddata_time[i];
		}
        ulPtsSoFar = dbdData.ul_ddata_pts_so_far;
    }    

   /* ------------------------------------------------------------------
	*   Found ending time - calculate how many pts are in day so far
	* ----------------------------------------------------------------*/
	if (bOnlyOneDay) {
	    *pulNumPts = ulPtsSoFar + i - ulPtsInDayBeforeStart;
	}
	else {
		*pulNumPts = *pulNumPts + ulPtsSoFar + i;
	}

	*pdActualAdjStartTime = AdjustTimestamp(StaID, dActualStartTime);
	*pdActualAdjEndTime = AdjustTimestamp(StaID, dActualEndTime);

db_exit:
    return(status);
}

/*===========================================================================
 *
 *  Name	 :  CvtTimeToDayStart
 *
 *  Purpose	 :  Calculate the julian time at the exact start of the
 *			    given day (00:00:00)
 *
 *  Return value :  None
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  20-Oct-1989  SFK	 Created
 *
===========================================================================*/
void CvtTimeToDayStart(unsigned long ulDayInSecs,unsigned long *pulDayStart)

{
   char szDate[DT_LEN+1];
   char szTime[DT_LEN+1];

   /* ------------------------------------------------------------------
    *	Find the number of seconds at the start of the day that we're
    *	looking for since this must be an exact match in the database.
    *	Convert the time first to ints and then back to secs starting at
    *	0hr 0min 0sec
    * ----------------------------------------------------------------*/
    gen_seconds_to_date_time_str(szDate, szTime, &ulDayInSecs, GEN_DTF_IAEA);
    gen_date_time_str_to_seconds(pulDayStart, szDate, "00:00:00", GEN_DTF_IAEA);
}

/*===========================================================================
 *
 *  Name	 :  SameDay
 *
 *  Purpose	 :  Check if the two times belong to the same day.
 
 *
 *  Return value :  TRUE - same day
 *					FALSE - different days

 *  date    /	author	revision
 *  -----------------	--------
 *  30-May-1995  SFK	 Created
 *
===========================================================================*/
int SameDay(unsigned long ulTime1,unsigned long ulTime2)

{
   char szDate1[DT_LEN+1];
   char szDate2[DT_LEN+1];
   char szTime[DT_LEN+1];

   /* ------------------------------------------------------------------
    *	Find the number of seconds at the start of the day that we're
    *	looking for since this must be an exact match in the database.
    *	Convert the time first to ints and then back to secs starting at
    *	0hr 0min 0sec
    * ----------------------------------------------------------------*/
    gen_seconds_to_date_time_str(szDate1, szTime, &ulTime1, GEN_DTF_IAEA);
    gen_seconds_to_date_time_str(szDate2, szTime, &ulTime2, GEN_DTF_IAEA);
    if (strcmp(szDate1, szDate2) != 0) return(FALSE);
    return(TRUE);
}



// 24-May-2005  Added function to check two DATEs for same day     
int SameDay(DATE dTime1, DATE dTime2)
{

   /* ------------------------------------------------------------------
    *	Find the number of seconds at the start of the day that we're
    *	looking for since this must be an exact match in the database.
    *	Convert the time first to ints and then back to secs starting at
    *	0hr 0min 0sec
    * ----------------------------------------------------------------*/
	if ((int)dTime1 != (int)(dTime2)) return(FALSE);
    return(TRUE);
}                                                             
              

///////////////////////////////////////////////////////////////////////////
//	This function finds the day containing the last record in database
//	for the specified interval.
//	All times are assumed to be unadjusted times.
//
//	static int FindDayContainingClosestEndTime(short sSta, unsigned long ulStartTime, unsigned long ulRequestedEndTime, unsigned long *pulActualEndTime)
//
//	Input:
//		sSta				database number for station
//		ulStartTime			julian seconds of search interval start time
//		ulRequestedEndTime	julian seconds of search interval end time
//
//	Output:
//		pulActualEndTime	julian seconds of the last record in the database in the search interval
//	
//	External Dependencies:
//	Multiple functions to read information from database
///////////////////////////////////////////////////////////////////////////
static int FindDayContainingClosestEndTime(short sSta, DATE dStartTime, DATE dRequestedEndTime, DATE *pdActualEndTime)
{
	db_day_rec dbEndDay;
	int status;
	DATE dDayStartTime;
		
	// verify that the requested interval is a valid one
    if (dRequestedEndTime < dStartTime) return(uiINVALID_TIME_INTERVAL_ERR);
       
    *pdActualEndTime = dRequestedEndTime;

	// find day containing the requested end time
	BOOL bSameDayAsStart = FALSE;
    status = glDbDay.FindDayRecInMainDb(sSta, dRequestedEndTime);
    if (status != 1) {		// day containing ulRequested End not in database
		*pdActualEndTime = floor(dRequestedEndTime);
    	do {					// so back up until you find a day in the database
    		*pdActualEndTime -= 1.0;
    		status = glDbDay.FindDayRecInMainDb(sSta, *pdActualEndTime);
    		if (status == TRUE) {
    			if (d_recread((char*)&dbEndDay, CURR_DB) != S_OKAY) return(uiDB_BAD_ERR);
    			*pdActualEndTime = dbEndDay.d_day_end_time;
    		}	
			//if ((int)dStartTime == (int)*pdActualEndTime) bSameDayAsStart = TRUE;
			bSameDayAsStart = SameDay(dStartTime, *pdActualEndTime);	// 24-May-2005 use function for SameDay
       	} while ((status != TRUE) && (!bSameDayAsStart) && (*pdActualEndTime > dStartTime));
       	if (status != TRUE) return (uiDAY_NOT_IN_DB_STATUS);		// didn't find a day
       	if (*pdActualEndTime < dStartTime) return (uiDAY_NOT_IN_DB_STATUS);
    }   	 
    	
    // read the record of the day found
	if (d_recread((char*)&dbEndDay, CURR_DB) != S_OKAY) return(uiDB_BAD_ERR);
    if ((*pdActualEndTime != dRequestedEndTime) || (*pdActualEndTime > dbEndDay.d_day_end_time)){  // get new end time if needed 
        *pdActualEndTime = dbEndDay.d_day_end_time;
    }

	// if requested end time is before the start of this day, then you must back up further
	bSameDayAsStart = FALSE;
    if ((*pdActualEndTime < dbEndDay.d_day_beg_time)){  // check if end time before the start
		dDayStartTime = *pdActualEndTime;
		*pdActualEndTime = floor(dDayStartTime);
    	do {
    		*pdActualEndTime -= 1.0;
    		status = glDbDay.FindDayRecInMainDb(sSta, *pdActualEndTime);
    		if (status == TRUE) {
    			if (d_recread((char*)&dbEndDay, CURR_DB) != S_OKAY) return(uiDB_BAD_ERR);
    			*pdActualEndTime = dbEndDay.d_day_end_time;
    		}	
			//if ((int)dStartTime == (int)*pdActualEndTime) bSameDayAsStart = TRUE;
			bSameDayAsStart = SameDay(dStartTime, *pdActualEndTime);	// 24-May-2005 use function for SameDay
       	} while ((status != TRUE) && (!bSameDayAsStart) && (*pdActualEndTime > dStartTime));
       	if (status != TRUE) return (uiDAY_NOT_IN_DB_STATUS);
       	if (*pdActualEndTime < dStartTime) return (uiDAY_NOT_IN_DB_STATUS);
    }

    return(TRUE);
}    



///////////////////////////////////////////////////////////////////////////
//	This function finds the day containing the first record in database
//	for the specified interval.
//
//	static int FindDayContainingClosestEndTime(short sSta, unsigned long ulStartTime, unsigned long ulRequestedEndTime, unsigned long *pulActualEndTime)
//
//	Input:
//		sSta				database number for station
//		ulStartTime			julian seconds of search interval start time
//		ulRequestedEndTime	julian seconds of search interval end time
//
//	Output:
//		pulActualEndTime	julian seconds of the last record in the database in the search interval
//	
//	External Dependencies:
//	Multiple functions to read information from database
///////////////////////////////////////////////////////////////////////////
static int FindDayContainingClosestStartTime(short sSta, DATE dRequestedStartTime, DATE dEndTime, DATE *pdActualStartTime)
{
	struct db_day_rec dbStartDay;
	int status;
	DATE dDayStartTime;	
	
	// verify that the requested interval is a valid one
    if (dEndTime < dRequestedStartTime) return(uiINVALID_TIME_INTERVAL_ERR);

    *pdActualStartTime = dRequestedStartTime;
	BOOL bSameDayAsEnd = FALSE;

	//	find day containing the requested start time
    status = glDbDay.FindDayRecInMainDb(sSta, dRequestedStartTime);
    if (status != TRUE) {	// day containing ulRequestedStartTime not in database
		*pdActualStartTime = floor(dRequestedStartTime);
    	do {				// move forward until you find a data in the database
    		*pdActualStartTime += 1.0;
    		status = glDbDay.FindDayRecInMainDb(sSta, *pdActualStartTime);
     		if (status == TRUE) {
    			if (d_recread((char*)&dbStartDay, CURR_DB) != S_OKAY) return(uiDB_BAD_ERR);
    			*pdActualStartTime = dbStartDay.d_day_beg_time;
    		}	
			//if ((int)*pdActualStartTime == (int)dEndTime) bSameDayAsEnd = TRUE;
			bSameDayAsEnd = SameDay(*pdActualStartTime, dEndTime);	// 24-May-2005 use function for SameDay
       	} while ((status != TRUE) && (!bSameDayAsEnd) && (*pdActualStartTime < dEndTime));
       	if (status != TRUE) return (uiDAY_NOT_IN_DB_STATUS);
    }   	 

	// read the record of the day found
    if (d_recread((char*)&dbStartDay, CURR_DB) != S_OKAY) return(uiDB_BAD_ERR);
    if ((*pdActualStartTime != dRequestedStartTime) || (*pdActualStartTime < dbStartDay.d_day_beg_time)){  /* get new end time if needed */
        *pdActualStartTime = dbStartDay.d_day_beg_time;
    }
	
	// if the start time is beyond the last time in this day then must move forward in db to next day
    if ((*pdActualStartTime > dbStartDay.d_day_end_time)){ 
		dDayStartTime = *pdActualStartTime;
		*pdActualStartTime = floor(dDayStartTime);
		bSameDayAsEnd = FALSE;
    	do {
    		*pdActualStartTime += 1.0;
    		status = glDbDay.FindDayRecInMainDb(sSta, *pdActualStartTime);
     		if (status == TRUE) {
    			if (d_recread((char*)&dbStartDay, CURR_DB) != S_OKAY) return(uiDB_BAD_ERR);
    			*pdActualStartTime = dbStartDay.d_day_beg_time;
    		}	
			//if ((int)*pdActualStartTime == (int)dEndTime) bSameDayAsEnd = TRUE;
			bSameDayAsEnd = SameDay(*pdActualStartTime, dEndTime);	// 24-May-2005 use function for SameDay
       	} while ((status != TRUE) && (!bSameDayAsEnd) && (*pdActualStartTime < dEndTime));
       	if (status != TRUE) return (uiDAY_NOT_IN_DB_STATUS);
        
    }                                         

    return(TRUE);
}    
	                                             