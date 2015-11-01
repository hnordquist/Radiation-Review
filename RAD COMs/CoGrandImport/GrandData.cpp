// GrandData.cpp: implementation of the CGrandData class.
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the specific routines to handle GRAND data
//
//	Functions:
//		GetDataStatistics
//		GetMultipleDbRecord
//		GetStationData
//		GetChannelData
//		CreateDay
//		AddData
//		
///////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "resource.h"
#include "GrandData.h"

#define Currtask (mpDb->m_Currtask)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define BIT_D0	0x01
#define BIT_D1	0x02
#define BIT_D2	0x04
#define BIT_D3	0x08
#define BIT_D4	0x10
#define BIT_D5	0x20
#define BIT_D6	0x40
#define BIT_D7	0x80
#define BIT_D16	0x0100 //E10R3
#define BIT_D17	0x0200 //E10R3
#define BIT_D18	0x0400 //E10R3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrandData::CGrandData(CDbVista* pDb, const int iStaNum, const bool bInterpolated, bool bQuietMode)
	:CBaseVistaData(pDb, iStaNum, bInterpolated, bQuietMode)
{
	mpGrandDataPtArray = NULL;
	mbFillingExistingRec = false;
	//m_bQuietMode = bQuietMode;
}

CGrandData::~CGrandData()
{
	//if (mpGrandDataPtArray) {
		delete [] mpGrandDataPtArray;
	//	mpGrandDataPtArray = NULL;
	//}
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStatistics
//
//	Description:
//	Find the interval in the db.  Calculate the number of points
//	in the database for the specified station and facility during 
//	the requested interval (including endpoints).
//
//	
//	Declaration:
//	bool CGrandData::GetDataStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrorMsg)
//
//	Input:	dRequested	interval to search for
//
//	Output:	pdActual		interval actually found in db >=dRequested start and <=dRequested end
//			pstrErrorMsg	error message, if any
//	
//			miErrorNum		number of error, if any
//			mulNumPtsActual	number of data points in the dRequested interval (including endpoints)
//
//	Return:	true (all okay) / false (error occurred while trying to determine number of points)
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from FindIntervalInDb in DbInterval.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
bool CGrandData::GetDataStatistics(
	const DB_D_INTERVAL &dRequested, 
	DB_D_INTERVAL *pdActual, 
	CString *pstrErrorMsg)
{

    struct db_day_rec	dbCurrentDay;		// day presently reading 
    struct db_float_data_rec 	dbfData; 	// record containing raw float data 

    int  i;
    DATE dCurrentDay;		    // day you want to find in database in jul secs 
    unsigned long ulPtsInDayBeforeStart;
    DATE dActualEndTime, dActualStartTime;
	DATE dRequestedStartTime, dRequestedEndTime;
	DATE dAdjRequestedStartTime, dAdjRequestedEndTime;
    unsigned long ulPtsSoFar;
    bool bOnlyOneDay;
    bool bDone;   
    
    mulNumPtsActual = 0;
    pdActual->dStart = 0.0;
	pdActual->dEnd = 0.0;
	if (pstrErrorMsg) 
		pstrErrorMsg->Empty();
	miErrorNum = 0;

	// Check that some data exist for the station in the db
	if (!StationDataExists()) //This call sets CBaseVistaData::miErrorNum
	{
		if (pstrErrorMsg)
			pstrErrorMsg->Format(" StationDataExists() returns error." );
		return false;
	}

	dRequestedStartTime = dRequested.dStart;
	dRequestedEndTime = dRequested.dEnd;

    // Find the day containing the requested end time (or the day containing the record
	// just before the end time).

	miErrorNum = FindDayContainingClosestRecord(dRequestedEndTime, BEFORE, &dAdjRequestedEndTime);
	if (miErrorNum != iDAY_IN_DB) 
	{
		if (pstrErrorMsg) 
			pstrErrorMsg->Format(" FindDayContainingClosestEndTime() returns error.");
		return(false);
	}
	
    // Find the day containing the requested start time (or the day containing the record
	// just after the start time).
	miErrorNum = FindDayContainingClosestRecord(dRequestedStartTime, AFTER, &dAdjRequestedStartTime);
    if (miErrorNum != iDAY_IN_DB) 
	{
		if (pstrErrorMsg)
			pstrErrorMsg->Format(" FindDayContainingClosestStartTime() returns error. ");
		return(false);
	}

	//////////////////////////////  MODIFIED FOR NDAR //////////////////////////////
	// pjm 12-04-2006
	//
	// Need to rethink this for Baseline 3 because RAD uses the 1-point, times equal
	// return values.  Kelly Michel suggested leaving the adjusted start and end times
	// in the return for RAD to key off of if the number of points is zero.
	// This would require a small modification in the RAD logic.
	//
	// handle case of one point in day
	//
	//  Here are the original 7 lines
	//
    //if (dAdjRequestedStartTime == dAdjRequestedEndTime) 
	//{
    //	pdActual->dStart = dAdjRequestedStartTime;
	//	pdActual->dEnd = dAdjRequestedEndTime;
	//	mulNumPtsActual = 1;
    //	return(true);
    //}
	//
	//  Here are the new 7 lines
	//
	if (dAdjRequestedStartTime == dAdjRequestedEndTime) 
	{
    	pdActual->dStart = dAdjRequestedStartTime;
		pdActual->dEnd = dAdjRequestedEndTime;
		mulNumPtsActual = 0;

		//if (pstrErrorMsg)
		//{
		//	pstrErrorMsg->Format("AdjustedRequestedStartTime(%f) == AdjustedRequestedEndTime(%f)",
		//		dAdjRequestedStartTime,dAdjRequestedEndTime);
		//}
    	return(true);
    }
	//
	//////////////////////END MODIFIED FOR NDAR //////////////////////////////

	// these cases should never happen; if they do return 0 points.
    if ((dRequestedEndTime < dAdjRequestedStartTime) || (dRequestedStartTime > dAdjRequestedEndTime)) 
	{
		miErrorNum = iNOT_DB_ISSUE;
		if (pstrErrorMsg)
			pstrErrorMsg->Format(" Adjustment of requested times error. ");
    	return false;
    }	

    //ulCurrentDay = ulRequestedStartTime;
   
	//	get the multiple data record containing the start time.
    bool bStatus = GetMultipleDbRecord(dAdjRequestedStartTime, &i, &dbfData, AFTER);	// read record containing start time
	if (!bStatus) 
	{
		if (pstrErrorMsg) pstrErrorMsg->Format(" GetMultipleDbRecord() returns error. ");
		return(false);
	}

	// make this day the owner of the multiple data set to explore
	if (d_setmr(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) {
		DbVistaError(pstrErrorMsg, !m_bQuietMode);
		return(false);
	}
	if (d_csoread(DAY_TO_FLOAT_DATA_SET, UL_DAY_TOTAL_PTS, (char *)&mulNumPtsActual, CURR_DB) != S_OKAY) {
		DbVistaError(pstrErrorMsg, !m_bQuietMode);
		return(false);
	}
	// figure out how many points to count for the start day
	if (dbfData.d_fdata_time[i] == 0) if (i != 0) i--; // back up if hit the 0s at end of day as they signal no data
	ulPtsInDayBeforeStart = dbfData.ul_fdata_pts_so_far + (long)i;
	mulNumPtsActual = mulNumPtsActual - ulPtsInDayBeforeStart;  
	dActualStartTime = dbfData.d_fdata_time[i];
	
	if (dRequestedEndTime < dActualStartTime) 
	{ // once again verify that requested interval is not before any real data in day
	    pdActual->dStart = dRequested.dStart;
		pdActual->dEnd = dRequested.dEnd;
		mulNumPtsActual = 0;

		//miErrorNum = iNOT_DB_ISSUE;
		//
		//if (pstrErrorMsg)
		//{
		//	pstrErrorMsg->Format("Requested End Time(%f) is before Actual Start Time(%f)",
		//		dRequestedEndTime,dActualStartTime);
		//}
		return(true);
		//return false;  //<<QA>>maybe?
	}			
	
	// finished with start day, now see if more than one day in the interval
    bOnlyOneDay = SameDay(dActualStartTime, dAdjRequestedEndTime);                                                         

	// there is more than one day so add the points in each day to the total
    if (!bOnlyOneDay) 
	{
		dCurrentDay = dActualStartTime;
		do {  // 09-24-98 rewrite handle case of a missing day
           	dCurrentDay = dCurrentDay + 1.0;			// go on to the next day in the db
			bDone = SameDay(dCurrentDay, dAdjRequestedEndTime);	// if is the end day, treat it differently
			if (!bDone) {									// not on the last day
	    		int iStatus = DayExists(dCurrentDay);
	    		if (iStatus == iDAY_IN_DB) {
					if (d_recread((char*)&dbCurrentDay, CURR_DB) != S_OKAY) {
						DbVistaError(pstrErrorMsg, !m_bQuietMode);
						return(false);
					}
			    	mulNumPtsActual = mulNumPtsActual + dbCurrentDay.ul_day_total_pts;	// add the number of pts in this day to the total
				}
	    	}
	    } while (!bDone);
    }


	// on the end day, get the multiple record containing the end time
    bStatus = GetMultipleDbRecord(dAdjRequestedEndTime, &i, &dbfData, BEFORE);
	if (!bStatus) {
		if (pstrErrorMsg) 
			pstrErrorMsg->Format("Error in GetMultipleDbRecord");
		return(false);
	}

	// figure out how many points to count for the end day
	//hn SR seems to not execute this if no data.  Make GRAND consistent.
	//if (i!=0) // jfl BMEND-125 7/18/2010 But when i is 0, a data point exists, so it is still a necessary point of data, so we DO return the values
	//{
		dActualEndTime = dbfData.d_fdata_time[i];
		ulPtsSoFar = dbfData.ul_fdata_pts_so_far;
		if (bOnlyOneDay) {
			mulNumPtsActual = ulPtsSoFar + i +1 - ulPtsInDayBeforeStart;
		}
		else {
			mulNumPtsActual = mulNumPtsActual + ulPtsSoFar + i + 1;
		}

		pdActual->dStart = dActualStartTime;   
		pdActual->dEnd = dActualEndTime;
	//}

	return(true);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetMultipleDbRecord
//
//	Description:
//	Find the db_data multiple rec containing the specified ulTime.
//	If the record time does not exactly match the time requested,
//	then based on the iSnap parameter find either the prior or following
//	record in the day for the station. 
//
//  Special notes:	if TRUE, curr_rec = multiple record containing the ulTime
//	
//	Declaration:
//	bool CGrandData::GetMultipleDbRecord(DATE dTime, int* piIndex, struct db_float_data_rec *pdbfData, int iSnap)
//
//	Input:	dTime		Time looking for in database for msStaNum
//			iSnap		if the exact record is not found, tells which way to move to "snap" to a data record
//						(either BEFORE or AFTER).
//
//	Output:	piIndex		which record in "multiple" record matches
//			pdbfData	pointer to "multiple" record from database for msStaNum containing ulTime
//			
//			miErrorNum		number of error, if any
//
//	Return:	true (found record with time) /false
//			if false then miErrorNum is one of the following
//				iSTA_NOT_IN_DB		msStaNum unknown in database
//				iDB_BAD				dbVista database error
//				iDAY_NOT_IN_DB		no day in database containing ulTime
//				iDAY_IN_DB			day for station msStaNum containing ulTime exists in database
//				iTIME_NOT_IN_DB		can't find any record with ulTime
//				iTIME_IN_REC		dbfData record contains ulTime
//	
//  date    /	author	revision
//  -----------------	--------
//	04-Dec-2001	SFK		Created from FindDbDataRecContainingTime in DbInterval.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//  06-JAN-2005 SFK		Added condition of dSearchTime before record time
//	26-APR-2005 SFK		Added condition of remembering which way we moved before since the 06-JAN
//						fix didn't work with another data set.
//////////////////////////////////////////////////////////////////
bool CGrandData::GetMultipleDbRecord(DATE dTime, int* piIndex, struct db_float_data_rec *pdbfData, SNAP eSnap)
{
    
	DB_ADDR dba;
	
	bool  bdbSuccess = true;
    bool  bSameDay = true;
    int   db_stat;
	int   iStatus;
	int   i;
	short iDbSta;	
    DATE dBegTime1, dBegTime2;
	DATE dSearchTime;
	bool bMovePrev = true;

	*piIndex = 0;
	miErrorNum = 0;
    
   /* ------------------------------------------------------------------
    *	Check that the day and sSta exist in the db. If the ulTime falls
    *   after the last ulTime in this day, then move to the next day.  Read
    *   out the	ulTime associated with the first day record for the day.
    * ----------------------------------------------------------------*/
	if (!StationDataExists()) {			// if station doesn't exist, can't do anything
		miErrorNum = iSTA_NOT_IN_DB;
		return(false);
	}

	iStatus = FindDayContainingClosestRecord(dTime, eSnap, &dSearchTime);
	if (iStatus != iDAY_IN_DB) {
		miErrorNum = iStatus;
		return(false);
	}
    
	// read the beginning time for the day containing our search time
    if ( d_crread(D_DAY_BEG_TIME_KEY, (char *)&dBegTime1, CURR_DB)!= S_OKAY) goto dbVistaError;
    
	// have found the day containing the desired time.
	// find multiple record key closest to search time
	db_stat = d_keyfind(D_FDATA_BEG_TIME_KEY, &dSearchTime, CURR_DB);

	if ((db_stat != S_OKAY) && (db_stat != S_NOTFOUND)) goto dbVistaError;
	// if this is not an actual key match try to lock onto the previous key
	// if that fails, then lock onto the next key
	if (db_stat != S_OKAY) 
	{
		db_stat = d_keyprev(D_FDATA_BEG_TIME_KEY, CURR_DB);
		bMovePrev = true;
	}
	if (db_stat == S_NOTFOUND) 
	{
		db_stat = d_keynext(D_FDATA_BEG_TIME_KEY, CURR_DB);
		bMovePrev = false;
	}
	if ((db_stat != S_OKAY) && (db_stat != S_NOTFOUND)) goto dbVistaError;

	// examine the record locked onto,  if this is not in the same day, move forward
	// to the next key which should be in the correct day
	db_stat  = d_recread((char*)pdbfData, CURR_DB);
	if (!SameDay(pdbfData->d_fdata_beg_time_key, dSearchTime)) 
	{
		db_stat = d_keynext(D_FDATA_BEG_TIME_KEY, CURR_DB);
		bMovePrev = false;
		if ((db_stat != S_OKAY) && (db_stat != S_NOTFOUND)) goto dbVistaError;
	}

	//	Locked onto, see if this is the record we are looking for
	if (db_stat == S_OKAY) 
	{
		if (d_crget(&dba, CURR_DB) != S_OKAY) goto dbVistaError;		// save address of multiple record

		// get the day record that owns this multiple record
		if (d_findco(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto dbVistaError;	
    	// Get the station that owns this day record
    	if (d_findco(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) goto dbVistaError;
    	//db_stat = d_recread(&dbSta,CURR_DB);	// debug
    	if (d_crread(S_STA_NUM_KEY, &iDbSta, CURR_DB) != S_OKAY) goto dbVistaError;		// read the station number
	}
	else 
	{
		iDbSta = -1;
	}

	// we are latched onto a multiple record just after the one that 
	//	contains our time, now find the multiple record that belongs to our station
	while ((iDbSta != msStaNum) && (bMovePrev == true))  
	{ // 26-APR-2005 Added condition of remembering which way we moved before
    	//	Find the data rec just before the current one and remember its address
		if (d_keyprev(D_FDATA_BEG_TIME_KEY, CURR_DB) == S_NOTFOUND) 
			break;	// no multiple records before

		if (d_crget(&dba, CURR_DB) != S_OKAY) goto dbVistaError;									// save address of multiple record

		// get the day record that owns this multiple record and see if this is still in the correct day
		if (d_findco(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto dbVistaError;	
		if (d_crread(D_DAY_BEG_TIME_KEY, &dBegTime2, CURR_DB) != S_OKAY) goto dbVistaError;		//beginning time in day
    	//db_stat = d_recread(&dbDay, CURR_DB);	// debug
		// if this is not the day we are looking for, we've gone too far back
	  	bSameDay = SameDay(dBegTime1, dBegTime2);
    	if (!bSameDay) break;
	    	
    	// Get the station that owns this day record
    	if (d_findco(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) goto dbVistaError;
    	//db_stat = d_recread(&dbSta,CURR_DB);	// debug
    	if (d_crread(S_STA_NUM_KEY, &iDbSta, CURR_DB) != S_OKAY) goto dbVistaError;           
	    	    	    	    	
    } 

	// if still haven't found the record, look forward until get past the day we know the record has to be in
    if (iDbSta != msStaNum) 
	{
        do {
		    /*	Find the data rec just after the current one */
			if (d_keynext(D_FDATA_BEG_TIME_KEY, CURR_DB) != S_OKAY) break;  // no records before
			//db_stat  = d_recread((char*)&dbfData, CURR_DB); // debug
	    	if (d_crget(&dba, CURR_DB) != S_OKAY) goto dbVistaError;		// save address of data record
	    	if (d_findco(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto dbVistaError;
	    	//db_stat = d_recread(&dbDay, CURR_DB);	// debug
	
       	    /* 	If this is not the day we're looking for then have gone	ahead too far */      
    		if (d_crread(D_DAY_BEG_TIME_KEY, &dBegTime2, CURR_DB) != S_OKAY) goto dbVistaError;
    		bSameDay = SameDay(dBegTime1, dBegTime2);
			if (!bSameDay) break;
//	   		if (!bSameDay) {	// back up one record
//		    	if (d_keyprev(UL_FDATA_BEG_TIME_KEY, CURR_DB) != S_OKAY) break;      
//  		}	
	    	
    		/*	Find the sSta this day rec belongs to */
    		if (db_stat = d_findco(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) goto dbVistaError;     
    		//db_stat = d_recread(&dbSta,CURR_DB);	// debug
    		if (d_crread(S_STA_NUM_KEY, &iDbSta, CURR_DB) != S_OKAY) goto dbVistaError;          
	    	    	    	
    	} while (iDbSta != msStaNum);            
    }	
	    
    if (!bSameDay) {
		miErrorNum = iTIME_NOT_IN_DB;
	    return(false);
	}                                
	
	/*  Found record, restore the address of the last data record found in the key search */
	if (d_crset(&dba, CURR_DB) != S_OKAY) goto dbVistaError;
	// return the record with the value
	if (d_recread((char*)pdbfData, CURR_DB) != S_OKAY) goto dbVistaError;

	// search through the record and find the value or the point just beyond where the value would be
	i = 0;

    while ((pdbfData->d_fdata_time[i] < dTime) &&
		   (i < (NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD-1)) &&
		   (pdbfData->d_fdata_time[i] != 0)) 
	{
		i++;
    }

	// when exit this are either at the point or the point beyond where you asked for
	if (pdbfData->d_fdata_time[i] != dTime) 
	{
		if ((eSnap == BEFORE) && (i != 0)) 
			i--;
	}

	*piIndex = i;
    return(true);    

dbVistaError:
	miErrorNum = iDB_BAD;
	return(false);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetStationData
//
//	Description:
//	Read all Float data associated with a time interval in the database
//	
//	Declaration:
//	bool CGrandData::GetStationData(CDbVista *pDb, const DB_D_INTERVAL &dRequested, CString *pstrErrorMsg)
//
//	Input:	dRequested	requested interval to retrieve data from
//			
//			mulNumPtsRequested	number of points to read starting at dRequested.dStart
//
//	Output:	pdActual	actual interval data retrieved from
//			pstrErrorMsg	error msg from problem, if any
//	
//			mulNumPtsActual		number of points read starting at pdActual->dStart to pdActual->dEnd
//			mpWholeFloatDataPts	ptr to array of mulNumPtsActual CWholeFloatDbData objects
//									(user must delete this after use)
//			m_dTimestampOfNextData where to start the search for the data following this interval
//
//	Return:	true (data successfully retrieved) / false (some kind of error, see miErrorNum and pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	04-Dec-2001	SFK		Created from ReadDbDataStruct in DbReadStruct.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//	11-Jun-2002	SFK		Changed to actually find the timestamp of the next data point
//	15-Oct-2003	SFK		Changed to get the status bits as part of the station record.
//////////////////////////////////////////////////////////////////
bool CGrandData::GetStationData(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, CString *pstrErrorMsg)
{
    struct db_float_data_rec dbfData;   /* read raw data into here */
    DB_ADDR dba;

    int i;
    int db_stat;
	int iStatus;
    DATE dCurrentDay, dLastDay;
    unsigned long j;
	DATE dRequestedStartTime, dRequestedEndTime;
	bool bFinished = false;
    
    mulNumPtsActual = 0;
	pdActual->dStart = 0;
	pdActual->dEnd = 0;
	if (pstrErrorMsg) pstrErrorMsg->Empty();
	miErrorNum = 0;

	if (mulNumPtsRequested == 0) return(true);

	dRequestedStartTime = dRequested.dStart;
	dRequestedEndTime = dRequested.dEnd;

	if (dRequestedEndTime < dRequestedStartTime)
	{
		if (pstrErrorMsg) 
			pstrErrorMsg->Format("Requested end time is prior to requested start time.");
		return(false);
	}

	if (dRequestedEndTime == dRequestedStartTime)
	{
		if (pstrErrorMsg) 
			pstrErrorMsg->Format("Requested end time is equal to requested start time.");
		return(false);
	}

   /* ------------------------------------------------------------------
    *	Find the last day in the db for this sSta, will use later
    * ----------------------------------------------------------------*/
	if (!GetLastTimeInDb(&dLastDay, true)) 
	{
		if (pstrErrorMsg) 
			pstrErrorMsg->Format("Error in GetLastTimeInDb");
		return(false);
	}

	if (dLastDay < dRequestedStartTime){
		if (pstrErrorMsg) pstrErrorMsg->Format("Last day in database is earlier than requested start time.");
		return(false);
	}
    
   /* ------------------------------------------------------------------
    *	Find the multiple record containing the starting time in the database.
    * ----------------------------------------------------------------*/
	if (!GetMultipleDbRecord(dRequestedStartTime, &i, &dbfData, AFTER)) 
	{
		if (pstrErrorMsg) 
			pstrErrorMsg->Format("Error in GetMultipleDbRecord");
		return(false);
	}
    
   /* ------------------------------------------------------------------
    *	Get the correct current owner and member set relationships of
    *   the multiple record, so can scan through the records in the day later.
    * ----------------------------------------------------------------*/
   	if (d_crget(&dba, CURR_DB) != S_OKAY) {
		DbVistaError(pstrErrorMsg, !m_bQuietMode);
		return(false);
	}
    if (d_findco(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) {
		DbVistaError(pstrErrorMsg, !m_bQuietMode);
		return(false);
	}
    if (d_crset(&dba, CURR_DB) != S_OKAY) {
		DbVistaError(pstrErrorMsg, !m_bQuietMode);
		return(false);
	}
      
   /* ------------------------------------------------------------------
    *	Found starting time - transfer data into users struct array
    * ----------------------------------------------------------------*/
    dCurrentDay = dRequestedStartTime;
    j = 0;

	if (mpGrandDataPtArray) 
		delete [] mpGrandDataPtArray;

	mpGrandDataPtArray = new CGrandDbDataPt[mulNumPtsRequested];

	CGrandDbDataPt* pTemp = mpGrandDataPtArray;

	bool bool1, bool2, bool3;

	if (pTemp) 
	{
		do //while (j != mulNumPtsRequested);
		{
			do //while (bool1 && bool2 && bool3);
			{
        		pTemp[j].mdJulianTime = dbfData.d_fdata_time[i];

				if (pdActual->dStart == 0) 
					pdActual->dStart = dbfData.d_fdata_time[i];	// remember the first time

				pdActual->dEnd		= dbfData.d_fdata_time[i];		// always update the last time with each point
        		pTemp[j].mulStatus 	= dbfData.ul_fdata_status[i];
        		pTemp[j].mfData[0] 	= dbfData.f_data_chan1[i];
				pTemp[j].mfData[1]	= dbfData.f_data_chan2[i];
				pTemp[j].mfData[2]	= dbfData.f_data_chan3[i];
				pTemp[j].mfData[3]	= dbfData.f_data_chan4[i];
				pTemp[j].mfData[4]	= dbfData.f_data_chan5[i];
				pTemp[j].mfData[5]	= dbfData.f_data_chan6[i];
				pTemp[j].mfData[6]	= dbfData.f_data_chan7[i];
				pTemp[j].mfData[7] = 0;
				pTemp[j].mfData[8] = 0;
				pTemp[j].mfData[9] = 0;
				pTemp[j].mfData[10] = 0;
				pTemp[j].mfData[11] = 0;
				pTemp[j].mfData[12] = 0;
				pTemp[j].mfData[13] = 0;
				pTemp[j].mfData[14] = 0;
				pTemp[j].mfData[15] = 0;
				pTemp[j].mfData[16] = 0;
				pTemp[j].mfData[17] = 0;
				if (dbfData.ul_fdata_status[i] & BIT_D0) pTemp[j].mfData[7] = 1;		// In MII
				if (dbfData.ul_fdata_status[i] & BIT_D1) pTemp[j].mfData[8] = 1;		// No authentication
				if (dbfData.ul_fdata_status[i] & BIT_D2) pTemp[j].mfData[9] = 1;		// Cold start
				if (dbfData.ul_fdata_status[i] & BIT_D3) pTemp[j].mfData[10] = 1;		// Taking offsets
				if (dbfData.ul_fdata_status[i] & BIT_D4) pTemp[j].mfData[11] = 1;		// Over thresholds
				if (dbfData.ul_fdata_status[i] & BIT_D5) pTemp[j].mfData[12] = 1;		// Changing Signal
				if (dbfData.ul_fdata_status[i] & BIT_D6) pTemp[j].mfData[13] = 1;		// Battery low
				if (dbfData.ul_fdata_status[i] & BIT_D7) pTemp[j].mfData[14] = 1;		// AC power loss
				// 22-Feb-2006 SFK	Added setting the values in the last 3 "channels"
				if (dbfData.ul_fdata_status[i] & BIT_D16) pTemp[j].mfData[15] = 1;		// Taking offsets
				if (dbfData.ul_fdata_status[i] & BIT_D17) pTemp[j].mfData[16] = 1;		// Invalid gammas
				if (dbfData.ul_fdata_status[i] & BIT_D18) pTemp[j].mfData[17] = 1;		// Saturated gammas
				pTemp[j].mdElapsedTime = dbfData.d_fdata_etime[i];

				j++;
	    		i++;

				bool1 = (i < NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD);  //78
				bool2 = (dbfData.d_fdata_time[i] != 0);
				bool3 = (j != mulNumPtsRequested);

			} while (bool1 && bool2 && bool3);

		   /* ------------------------------------------------------------------
			*   When have read all the points requested, must figure out the timestamp
			*	of the next point to return.  
			* ----------------------------------------------------------------*/
			if (j == mulNumPtsRequested) 
			{
				// see if can use the next point in this multiple record
				if ((i < NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD) && (dbfData.d_fdata_time[i] != 0)) 
				{
					mdTimestampOfNextData = dbfData.d_fdata_time[i];		// this is the next data point to be readalways update the last time with each point
					bFinished = true;
				}
				else 
				{	// must get next record in db and read from it
	    			db_stat  = d_findnm(DAY_TO_FLOAT_DATA_SET, CURR_DB);

	       			if (db_stat == S_EOS) 
					{		// see if there is another day in the db
						if (SameDay(dCurrentDay, dLastDay)) 
						{	// no more days in db
							mulNumPtsActual = j;
							mdTimestampOfNextData = dbfData.d_fdata_time[i-1];	// best guess available
							return(true);
						}	

						do 
						{
		    				dCurrentDay += 1.0;
		    				iStatus = DayExists(dCurrentDay);
						} while ((iStatus == iDAY_NOT_IN_DB) && !SameDay(dCurrentDay, dLastDay));
					
						if (iStatus != iDAY_IN_DB) 
						{	// there is no more (good) data in the db, use the last time of last record read
							mulNumPtsActual = j;
							mdTimestampOfNextData = dbfData.d_fdata_time[i-1];	// best guess available
							return(true);
						}	
                        
						// there is another day in the db, get ready to read the first record
						if (d_setor(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) 
						{
							DbVistaError(pstrErrorMsg, !m_bQuietMode);
							return(false);
						}
						if (d_findfm(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) 
						{
							DbVistaError(pstrErrorMsg, !m_bQuietMode);
							return(false);
						}
	    			}
					if (d_recread((char *)&dbfData, CURR_DB) != S_OKAY) 
					{
						DbVistaError(pstrErrorMsg, !m_bQuietMode);
						return(false);
					}
	     			i = 0;
					mdTimestampOfNextData = dbfData.d_fdata_time[i]; // this is the next data point to be read
					bFinished = true;
				}
			}		

		   /* ------------------------------------------------------------------
			*   Move on to the next record in this day if it exists, if no
			*	more records in this day, then move on to the next day in the db.
			* ----------------------------------------------------------------*/
			if ((j != mulNumPtsRequested)  || !bFinished) 
			{
			
				if (SameDay(dCurrentDay, dLastDay)) 
				{
					j = j;
				}
	    		db_stat  = d_findnm(DAY_TO_FLOAT_DATA_SET, CURR_DB);

	       		if (db_stat == S_EOS) //end of set
				{
					if (SameDay(dCurrentDay, dLastDay)) 
					{		//	06-Jan-2005  if already at last day quite with what we've got
						mulNumPtsActual = j;
						mdTimestampOfNextData = dbfData.d_fdata_time[i-1];	// best guess available
						return(true);
					}

					do 
					{
		    			dCurrentDay += 1.0;
		    			iStatus = DayExists(dCurrentDay);
					} while ((iStatus == iDAY_NOT_IN_DB) && !SameDay(dCurrentDay, dLastDay));  //13
				
					if (iStatus != iDAY_IN_DB) //12
					{
						mulNumPtsActual = j;
						return(true);
					}	
                                          
					if (d_setor(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) 
					{
						DbVistaError(pstrErrorMsg, !m_bQuietMode);
						return(false);
					}
					if (d_findfm(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) 
					{
						DbVistaError(pstrErrorMsg, !m_bQuietMode);
						return(false);
					}
	    		}
				if (d_recread((char *)&dbfData, CURR_DB) != S_OKAY) 
				{
					DbVistaError(pstrErrorMsg, !m_bQuietMode);
					return(false);
				}
	     		i = 0;
			}
		} while (j != mulNumPtsRequested);

	}  //if pTemp
       
	mulNumPtsActual = j;  
    return(true);       
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetChannelData
//
//	Description:
//	Read only x,y data associated with a time interval in the database
//	where x is the julian time and y is one of the data channels.
//	The calling routine can specify whether to apply an offset to the x data
//	so that the time associated with ulStartTime is 0.
//	
//	Declaration:
//	bool CGrandData::GetChannelData(CDbVista *pDb, const int &iChanNuml, const DB_D_INTERVAL &dRequested,  DB_D_INTERVAL* pdRequested, CString *pstrErrorMsg)
//
//	Input:	pDb			pointer to dbVista class for database that should be open for use now
//			iChanOffset	which channel in the data array to read from 1 to 15 (last 8 are status bits)
//			dRequested	requested interval to retrieve data from
//			
//			mulNumPtsRequested	number of points to read starting at dRequested.dStart
//
//	Output:	pdActual	actual interval data retrieved from
//			pstrErrorMsg	error msg from problem, if any
//	
//			mulNumPtsActual	number of points read starting at pdActual->dStart to pdActual->dEnd
//			mpdXYDataPts	ptr to array of mulNumPtsActual CDoubleXYData objects
//									(user must delete this after use)
//			mdTimestampOfNextData where to start the search for the data following this interval
//
//	Return:	true (data successfully retrieved) / false (some kind of error, see miErrorNum and pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	04-Dec-2001	SFK		Created from ReadDbDataArray in DbReadArray.cpp
//	07-Feb-2002 SFK		For speed reasons, copy data directly into users array - rather
//							than "newing" an object of appropriate size.
//	18-Apr-2002 SFK		Changed timebase to DATE
//	11-Jun-2002	SFK		Changed to actually find the timestamp of the next data point
//	14-Oct-2003	SFK		Added ability to return the status bits as channesl
//////////////////////////////////////////////////////////////////
bool CGrandData::GetChannelData(
	const int &iChanOffset, 
	const DB_D_INTERVAL &dRequested, 
	DB_D_INTERVAL* pdActual, 
	double *pdTimestamp, 
	double *pdData,  
	CString *pstrErrorMsg)
{
    struct db_float_data_rec dbfData;   
    DB_ADDR dba;
	DATE dRequestedStartTime, dRequestedEndTime;
    
    int i;
    int db_stat;
	int iStatus;
    DATE dCurrentDay, dLastDay;
    unsigned long j;
	bool bFinished = false;

    
    mulNumPtsActual = 0;
	pdActual->dStart = 0;
	pdActual->dEnd = 0;

	if (pstrErrorMsg) 
		pstrErrorMsg->Empty();

	if (mulNumPtsRequested == 0) 
		return(true);

	if ((iChanOffset < 1) || (iChanOffset > GRAND_NUM_DATA_CHANS)) 
	{
		if (pstrErrorMsg) 
			pstrErrorMsg->Format("Invalid channel offset requested (channel = %d)\nOnly 1-15 are valid.", iChanOffset);
		return(false);
	}

	dRequestedStartTime = dRequested.dStart;
	dRequestedEndTime = dRequested.dEnd;
    
	
   /* ------------------------------------------------------------------
    *	Find the last day in the db for this station, will use later
    * ----------------------------------------------------------------*/
	if (!GetLastTimeInDb(&dLastDay, true)) 
	{
		if (pstrErrorMsg) 
			pstrErrorMsg->Format("Error in GetLastTimeInDb");

		return(false);
	}
    
   /* ------------------------------------------------------------------
	*	Find the multiple record containing the starting time in the database.
	* ----------------------------------------------------------------*/
	if (!GetMultipleDbRecord(dRequestedStartTime, &i, &dbfData, AFTER)) 
	{
		if (pstrErrorMsg) 
			pstrErrorMsg->Format("Error in GetMultipleDbRecord");
		return(false);
	}

   /* ------------------------------------------------------------------
	*	Get the correct current owner and member set relationships of
	*   the multiple record, so can scan through the records in the day later.
	* ----------------------------------------------------------------*/
   	if (d_crget(&dba, CURR_DB) != S_OKAY) 
	{
		DbVistaError(pstrErrorMsg, !m_bQuietMode);
		return(false);
	}
    if (d_findco(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) 
	{
		DbVistaError(pstrErrorMsg, !m_bQuietMode);
		return(false);
	}
    if (d_crset(&dba, CURR_DB) != S_OKAY) 
	{
		DbVistaError(pstrErrorMsg, !m_bQuietMode);
		return(false);
	}
	
   /* ------------------------------------------------------------------
	*	Found starting time - transfer data into users struct array
	* ----------------------------------------------------------------*/
	dCurrentDay = dRequestedStartTime;
	j = 0;
	int iShifts;
	long lBit;
	int iDum;


	do {
		do {
		   /* ------------------------------------------------------------------
	    	*	If some points are out of order before the start time
	    	*	asked for, set their time to start time.
	    	* ----------------------------------------------------------------*/
			if (dbfData.d_fdata_time[i] < dRequestedStartTime) 
				dbfData.d_fdata_time[i] = dRequestedStartTime;
			if (pdActual->dStart == 0) pdActual->dStart = dbfData.d_fdata_time[i];		// remember time of first point
			pdActual->dEnd = dbfData.d_fdata_time[i];		// current last time is time of this point

			//pTemp[j].mdTimestamp = (double)dbfData.ul_fdata_time[i];
			pdTimestamp[j] = dbfData.d_fdata_time[i];
		   /* ------------------------------------------------------------------
	    	*	Temporarily, if have points out of order, label them with the
			*	same time as the last time read + 0.001.
	    	* ----------------------------------------------------------------*/
//			if (j > 0) if (pdTimestamp[j] <= pdTimestamp[j-1])
//				pdTimestamp[j] = pdTimestamp[j] + 0.0010; 
			
			switch (iChanOffset) {
	        	case 1:
					pdData[j] = (double)dbfData.f_data_chan1[i];
	        		break;
		        		
	        	case 2:
	        		pdData[j] = (double)dbfData.f_data_chan2[i];
	        		break;
		        		
	        	case 3:
	        		pdData[j] = (double)dbfData.f_data_chan3[i];
	        		break;
		        		
	        	case 4:
	        		pdData[j] = (double)dbfData.f_data_chan4[i];
	        		break;
		        		
	        	case 5:
	        		pdData[j] = (double)dbfData.f_data_chan5[i];
	        		break;
		        		
	        	case 6:
	        		pdData[j] = (double)dbfData.f_data_chan6[i];
	        		break;
		        		
	        	case 7:
	        		pdData[j] = (double)dbfData.f_data_chan7[i];
	        		break;     

	        	case 8:
	        	case 9:
	        	case 10:
	        	case 11:
	        	case 12:
	        	case 13:
	        	case 14:
	        	case 15:
					iShifts = iChanOffset - 8;
					lBit = 1;
					lBit = lBit << iShifts;		// this is the bit in the status work
					iDum = dbfData.ul_fdata_status[i] & lBit;
					if (iDum == lBit) {
						pdData[j] = 1.0;
					}
					else {
						pdData[j] = .00000001;
					}
	        		break;
					
				// 11-Aug-2005 Added cases 16, 17, 18 to support new status bits created during file read
				case 16:
					if (dbfData.ul_fdata_status[i] & OFFSET_BIT) {
						pdData[j] = 1.0;
					}
					else {
						pdData[j] = .00000001;
					}
					break;

				case 17:
					if (dbfData.ul_fdata_status[i] & INVALID_G_BIT) {
						pdData[j] = 1.0;
					}
					else {
						pdData[j] = .00000001;
					}
					break;

				case 18:
					if (dbfData.ul_fdata_status[i] & SATURATED_BIT) {
						pdData[j] = 1.0;
					}
					else {
						pdData[j] = .00000001;
					}
					break;

				default:
					pdData[j] = 0;
					break;
			}		

			i++;
			j++;

		} while ((i<NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD) && (dbfData.d_fdata_time[i] != 0) && (j != mulNumPtsRequested));

	   /* ------------------------------------------------------------------
		*   When have read all the points requested, must figure out the timestamp
		*	of the next point to return.  
		* ----------------------------------------------------------------*/
		if (j == mulNumPtsRequested) {
			// see if can use the next point in this multiple record
			if ((i < NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD) && (dbfData.d_fdata_time[i] != 0)) {
				mdTimestampOfNextData = dbfData.d_fdata_time[i];		// this is the next data point to be readalways update the last time with each point
				bFinished = true;
			}
			else {	// must get next record in db and read from it
	    		db_stat  = d_findnm(DAY_TO_FLOAT_DATA_SET, CURR_DB);

	       		if (db_stat == S_EOS) {		// see if there is another day in the db
					if (SameDay(dCurrentDay, dLastDay)) {	// no more days in db
						mulNumPtsActual = j;
						mdTimestampOfNextData = dbfData.d_fdata_time[i-1];	// best guess available
						return(true);
					}	

					do {
		    			dCurrentDay += 1.0;
		    			iStatus = DayExists(dCurrentDay);
					} while ((iStatus == iDAY_NOT_IN_DB) && !SameDay(dCurrentDay, dLastDay));
				
					if (iStatus != iDAY_IN_DB) {	// there is no more (good) data in the db, use the last time of last record read
						mulNumPtsActual = j;
						mdTimestampOfNextData = dbfData.d_fdata_time[i-1];	// best guess available
						return(true);
					}	
                        
					// there is another day in the db, get ready to read the first record
					if (d_setor(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) {
						DbVistaError(pstrErrorMsg, !m_bQuietMode);
						return(false);
					}
					if (d_findfm(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) {
						DbVistaError(pstrErrorMsg, !m_bQuietMode);
						return(false);
					}
	    		}
				if (d_recread((char *)&dbfData, CURR_DB) != S_OKAY) {
					DbVistaError(pstrErrorMsg, !m_bQuietMode);
					return(false);
				}
	     		i = 0;
				mdTimestampOfNextData = dbfData.d_fdata_time[i];		// this is the next data point to be read
				bFinished = true;
			}
		}		

	   /* ------------------------------------------------------------------
		*   Move on to the next record in this day if it exists, if no
		*	more records in this day, then move on to the next day in the db.
		* ----------------------------------------------------------------*/
		if ((j != mulNumPtsRequested)  || !bFinished) {
			db_stat  = d_findnm(DAY_TO_FLOAT_DATA_SET, CURR_DB);
			if (db_stat == S_EOS) {
				if (SameDay(dCurrentDay, dLastDay)) {		//	06-Jan-2005  if already at last day quite with what we've got
					mulNumPtsActual = j;
					mdTimestampOfNextData = dbfData.d_fdata_time[i-1];	// best guess available
					return(true);
				}

				do {
			    	dCurrentDay += 1.0;
			    	iStatus = DayExists(dCurrentDay);;
				} while ((iStatus == iDAY_NOT_IN_DB) && !SameDay(dCurrentDay, dLastDay));
			
				if (iStatus != iDAY_IN_DB) {
					mulNumPtsActual = j;
					return(true);
				}	

				if (d_setor(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) {
					DbVistaError(pstrErrorMsg, !m_bQuietMode);
					return(false);
				}
				if (d_findfm(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) {
					DbVistaError(pstrErrorMsg, !m_bQuietMode);
					return(false);
				}
			}
			else {
				if (db_stat != S_OKAY) {
					DbVistaError(pstrErrorMsg, !m_bQuietMode);
					return(false);
				}
			}		    		
			if (d_recread((char *)&dbfData, CURR_DB) != S_OKAY) {
				DbVistaError(pstrErrorMsg, !m_bQuietMode);
				return(false);
			}
			i = 0;
		}	
	} while (j != mulNumPtsRequested);


	mulNumPtsActual = j;       
    return(true);       
}



///////////////////////////////////////////////////////////////////////////
//	Name:	CreateDay
//
//	Description:
//	Create the standard day record and make it the owner of the 
//	DAY_TO_FLOAT_DATA_SET.
//	
//	Declaration:
//	bool CGrandData::CreateDay(DATE dDay)
//
//	Input:	dDay		day to add to the database
//			
//	Output:	
//			miErrorNum	error number, if any
//
//	Return:	true (data successfully written) / false (some kind of error, see miErrorNum and pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	04-Dec-2001	SFK		Created from part of ImportData in DbImport.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
bool CGrandData::CreateDay(DATE dDay)
{
	int iStatus = CBaseVistaData::CreateDay(dDay);
	if (iStatus == iDAY_IN_DB) {
		if (d_setor(DAY_TO_FLOAT_DATA_SET,CURR_DB) != S_OKAY) {
			miErrorNum = iDB_BAD;
			return(false);
		}
	}
	return(true);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	StationDataExists
//
//	Description:
//	Verify that some data exists for the station in the db
//	
//	Declaration:
//	bool CGrandData::StationDataExists()
//
//	Input:	
//			
//	Output:	
//			miErrorNum	error number, if any
//
//	Return:	true (data successfully written) / false (some kind of error, see miErrorNum and pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	04-Dec-2001	SFK		Created from part of ImportData in DbImport.cpp
//////////////////////////////////////////////////////////////////
bool CGrandData::StationDataExists()
{
	int dbstat;

	bool bStatus = CBaseVistaData::StationDataExists();	// does the station number exist

	if (bStatus)  //base class fills in CBaseVistaData::miErrorNum for use by caller
	{
		dbstat = d_setor(STATION_TO_DAY_SET, CURR_DB);

		if (dbstat == S_OKAY) 
			dbstat = d_findfm(STATION_TO_DAY_SET, CURR_DB);

		if (dbstat == S_EOS) 
		{		// no data for the station
			miErrorNum = iNO_DATA_FOR_STATION;
			bStatus = false;
			//return(false);
		}
	}

	return bStatus;
	//return(true);
}

	
///////////////////////////////////////////////////////////////////////////
//	Name:	AddData
//
//	Description:
//	Add a multiple data record to the database (this can be either writing a new one
//	or rewriting an existing one)
//	
//	Declaration:
//	int CGrandData::AddData(CString &pstrErrorMsg)
//
//	Input:	
//			mbFillingExitingRec		true(rewriting an existing record)/false(creating a new record)
//			mpdbFloatDataRec		record that is to be written
//			
//	Output:	pstrErrorMsg	error msg from problem, if any
//	
//			mbFillingExistingRec	set to false if wrote an existing record
//
//	Return:	true (data successfully written) / false (some kind of error, see miErrorNum and pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	17-Dec-2001	SFK		Created from part of ImportData in DbImport.cpp
//////////////////////////////////////////////////////////////////
bool CGrandData::AddData(CString *pstrErrorMsg)
{

	bool bSuccess = true;
	int dbvstat = S_OKAY;
	if (!mbFillingExistingRec) 
	{
		if ((dbvstat = d_fillnew(DB_FLOAT_DATA_REC,(char *)&mdbFloatDataRec, CURR_DB)) != S_OKAY) 
			bSuccess = false;

		if (bSuccess)
		{
			if ((dbvstat = d_connect(DAY_TO_FLOAT_DATA_SET, CURR_DB)) != S_OKAY) 
				bSuccess = false;
		}
	}
	else 
	{	// special case if adding to an existing record in the dat
		if ((dbvstat = d_recwrite((char *)&mdbFloatDataRec, CURR_DB)) !=S_OKAY) 
			bSuccess = false;
		mbFillingExistingRec = false;
	}
	if (!bSuccess) 
	{
		miErrorNum = iDB_BAD;
		NewDbVistaError(pstrErrorMsg, false, dbvstat);
		return(false);
	}
	return(true);
}

void CGrandData::NewDbVistaError(CString *pstrMsg, bool bShowMsg, int status)
{
	miErrorNum = iINTERNAL_ERROR;
	if (pstrMsg) pstrMsg->Format("Internal dbVista error: %d, database may be corrupted.", status);
	if (bShowMsg && !m_bQuietMode) 
		MessageBox(NULL, *pstrMsg, "Database Error", MB_OK|MB_ICONWARNING);
}