///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbLimit.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		GetDbTimeRange
//
//		All times are adjusted times
//
///////////////////////////////////////////////////////////////////////////
#include "RAD.H"
#include "AdjustTime.h"
#include "DbDay.h"
#include "DbInterval.h"
#include "DbLimit.h"
#include "FacilityConfig.h" //KM
#include "MainDbCfg.h"
#include "PerformTimerForGuiLib.h"
#include "TimeAlign.h"
#include "Interval.h"
#include "MyDateTime.h"
#include "TypeDefinitions.h"
#include "EOSSImport.h"
#include "BinaryImport.h"

extern short glsFacNum; //km
extern CEOSSImport *pglEOSSImport;
extern CBinaryImport *pglBinaryImport;

static int FindDbStartDay(struct IDStruct StaID, DATE *pdDay, BOOL bErrDisplay);
static int FindDbLastDay(struct IDStruct StaID, DATE *pdDay, BOOL bErrDisplay);


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDbTimeRange
//
//	Description:
//	Determine the first time and last time in the database(s) for the specified 
//	station.  If the station is -1, then find the first and last time for any
//	station in the database(s).  The COMs as well as the main Rad db are searched.
//	All times are expressed as adjusted values.
//
//	Declaration:
//	BOOL GetDbTimeRange(const struct IDStruct StaID, DATE *pdStartTime, DATE *pdEndTime)
//
//	Input:	StaID	station want to find limits for (-1 means any station)
//			
//	Output: pdStartTime		timestamp of first record for station in database
//			pdEndTime		timestamp of last record for station in database
//
//	Return: TRUE - some data in DB
//			FALSE - no data or error
//	
//  date    /	author	revision
//  -----------------	--------
//  ??			SFK		Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
//	24-Sep-2003	SFK		Version 3.0.3 modified to support new FacMgr
//////////////////////////////////////////////////////////////////
// Replaces int FindDbStartDay(short sSta,unsigned long *pdDay) and 
//	int FindDbLastDay(short sSta,unsigned long *pdDay, BOOL bErrDisplay)
BOOL GetDbTimeRange(const struct IDStruct StaID, DATE *pdStartTime, DATE *pdEndTime)
{
	DATE dDay;

	*pdStartTime = 0.0;
	*pdEndTime = 0.0;

	// Find start day and convert to DATE
	BOOL bStatus = FindDbStartDay(StaID, &dDay, FALSE);
	if (!bStatus) 
		return(FALSE);

	*pdStartTime = dDay;

	// Find end day and convert to DATE
	bStatus = FindDbLastDay(StaID, &dDay, FALSE);
	if (!bStatus) 
		return(FALSE);

	*pdEndTime = dDay;

	return(TRUE);
}

/*===========================================================================
 *
 *  Name	 :  FindDbStartDay
 *
 *  Purpose	 :  Find the first day in the db for the specified station or the
 *				first day for any station.
 *				Returns the time for 00:00:00 of the starting day in the db, not
 *				the first time in the db.
 *				Times in database are not adjusted.  Must return adjusted times
 *				since everything runs on adjusted times.
 *
 *  Return value :  TRUE (found data)
 *					uiDB_EMPTY_STATUS
 *					uiNO_DATA_FOR_STATION_WARN
 *					errors from database call
 *
 *  Special notes:
 *
 *
 *  Implicit Input :	StaID;	input - station to look for (-1 = any station)
 *  Implicit Input/Output:
 *  Implicit Output:	*pdDay;	output - julian time representing first time in day

 *  date    /	author	revision
 *  -----------------	--------
 *  30-May-95  SFK	 	Created from old GRAND Review FindDbStartDay
 *  20-Nov-98  SFK		Added concept of bErrDisplay to suppress error messages
 *	21-Mar-2001 SFK		Version 2.07 Added hooks into data residing in Access database
 *	06-Feb-2002	SFK		Added hooks into getting information about GRANDs from Grand Com
 *	07-Aug-2002	SFK		Changed to DATE timebase and vectored all calls from
 *						other modules through GetDbTimeRange
 *	24-Sep-2003	SFK		Version 3.0.3 modified to support new FacMgr
 *
===========================================================================*/
static int FindDbStartDay(struct IDStruct StaID, DATE *pdDay, BOOL bErrDisplay)
{
	int status, db_stat;
    DATE dTime;
	struct db_day_rec dbDay;

	*pdDay = 999999999999.0;		// put in a big number that will never be a realistic date

   /* ------------------------------------------------------------------
    *	Look in the COMs, if necessary
    * ----------------------------------------------------------------*/
	DATE dOthersStartDay = 999999999999.0;
	int iOthersComStatus = 0;

	DATE dEOSSStartDay = 999999999999.0;
	int iEOSSComStatus = 0;

	DATE dBinaryStartDay = 999999999999.0;
	int iBinaryComStatus = 0;	

	DATE dComStartDay = 0.0;
	int iComStatus = 0;

	//EOSS does not use time align so we do it separately.
	if ((StaID.sStaID == -1) || (StaID.sStaType == EOSS_TYPE)) 
	{
		pglEOSSImport->GetDbStartDay(glsFacNum, StaID.sStaID, &dEOSSStartDay, bErrDisplay);
		if (dEOSSStartDay != 0.0)
			iEOSSComStatus = 1;
	}

	//Binary import needs to be divorced from TimeAlign too.
	if ((StaID.sStaID == -1) || (StaID.sStaType == BINARY_TYPE)) 
	{
		pglBinaryImport->GetDbStartDay(glsFacNum, StaID.sStaID, &dBinaryStartDay, bErrDisplay);
		if (dBinaryStartDay != 0.0)
			iBinaryComStatus = 1;
	}

	//This was the original non-EOSS code
	if ((StaID.sStaID == -1) || (pglTimeAlign->COMDataType(StaID.sStaType))) 
	{
		//This will check everything else.
		pglTimeAlign->GetDbStartDay(glsFacNum, StaID.sStaID, &dOthersStartDay,(bErrDisplay != 0),StaID.sStaType);
		if (dOthersStartDay != 0.0)
			iOthersComStatus = 1;
	}

	if (iEOSSComStatus)
	{
		iComStatus = 1;
		dComStartDay = dEOSSStartDay;
	}

	if (iBinaryComStatus)
	{
		if (iComStatus == 0)
		{
			iComStatus = 1;
			dComStartDay = dBinaryStartDay;
		}
		else
		{
			if (dComStartDay > dBinaryStartDay)
				dComStartDay = dBinaryStartDay;
		}
	}

	if (iOthersComStatus)
	{
		if(iComStatus == 0)
		{
			iComStatus = 1;
			dComStartDay = dOthersStartDay;
		}
		else
		{
			if (dComStartDay > dOthersStartDay)
				dComStartDay = dOthersStartDay;
		}
	}
/*
	//Pick the earlier of the EOSS start day and the start day of all of the others
	if (iOtherComStatus && iEOSSComStatus)  //both non-zero
	{
		if (dOthersStartDay <= dEOSSStartDay)
			dComStartDay = dOthersStartDay;
		else
			dComStartDay = dEOSSStartDay;

		iComStatus = 1;
	}
	else if (!iOtherComStatus && !iEOSSComStatus) //both zero -> no data
	{
		dComStartDay = 0.0;
		iComStatus = 0;
	}
	else if (iOtherComStatus)
	{
		dComStartDay = dOthersStartDay;
		iComStatus = 1;
	}
	else if (iEOSSComStatus)
	{
		dComStartDay = dEOSSStartDay;
		iComStatus = 1;
	}
*/
	*pdDay = dComStartDay;

	if (StaID.sStaID != -1)
	{
		if (StaID.sStaType == EOSS_TYPE)
			return iComStatus;
		
		if (StaID.sStaType == BINARY_TYPE)
			return iComStatus;

		if (pglTimeAlign->COMDataType(StaID.sStaType))
			return iComStatus;
	}
   /* ------------------------------------------------------------------
    *	If necessary, look in main rad db.
	*	Find if any days exist in the Rad Main database.
    * ----------------------------------------------------------------*/
    dTime = 0;
    db_stat = d_keyfind(D_DAY_BEG_TIME_KEY, &dTime, CURR_DB);

    if (db_stat == S_NOTFOUND) 
		db_stat = d_keynext(D_DAY_BEG_TIME_KEY, CURR_DB);

    if (db_stat == S_NOTFOUND) 
	{	// no data in the main data base, check the COMs
		if (iComStatus == 1) 
		{
			*pdDay = dComStartDay;
			return 1; // found information in the COMs
		}
		// no data in the COMs or main rad db
    	if (bErrDisplay)
			RadReviewMsg(uiDB_EMPTY_STATUS);

    	return(uiDB_EMPTY_STATUS);
    }
    if (db_stat != S_OKAY) 
		goto db_exit;
    
   /* ------------------------------------------------------------------
    *	There is some data in the main rad db.  See if you need to look here.
    *	If just looking for the starting time of any station in the db have
    *	found it, read and return the starting time for the first time now.
    * ----------------------------------------------------------------*/
    if (StaID.sStaID == -1) 
	{
		// determine the earliest day in the rad main db
		db_stat = d_recread(&dbDay, CURR_DB);

		dbDay.d_day_beg_time_key = AdjustTimestamp(StaID, dbDay.d_day_beg_time_key);
    	if (db_stat != S_OKAY) 
			goto db_exit;

		if (*pdDay == 0.0)
			*pdDay = dbDay.d_day_beg_time_key;

		else if (dbDay.d_day_beg_time_key < *pdDay) 
			*pdDay = dbDay.d_day_beg_time_key;

		// now check if any COM day is before this day
		//return the first time found in either db
		if ((iComStatus == 1) && (dComStartDay < *pdDay))
				*pdDay = dComStartDay;

    	return 1;
    }	

   /* ------------------------------------------------------------------
    *	Are looking for a particular station of the data supported by the
	*	rad main db.  Find that station and see if there is any day under
	*	it.  If yes read and return the julian time of the first record in the day.
    * ----------------------------------------------------------------*/
	status = StationExistsInMainDb(StaID.sStaID);
    if (status != 1) 
		return (status);

    db_stat = d_setor(STATION_TO_DAY_SET, CURR_DB);
    if (db_stat != S_OKAY) 
		goto db_exit;

    db_stat = d_findfm(STATION_TO_DAY_SET, CURR_DB);
    if (db_stat == S_EOS)
        return(uiNO_DATA_FOR_STATION_WARN);
	if (db_stat != S_OKAY) 
		goto db_exit;

    db_stat = d_crread(D_DAY_BEG_TIME_KEY, (char *)pdDay, CURR_DB);
    if (db_stat != S_OKAY) 
		goto db_exit;

	*pdDay = AdjustTimestamp(StaID, *pdDay);

	if (dComStartDay < *pdDay) 
		*pdDay = dComStartDay;

    return 1;
        
db_exit:
    if (bErrDisplay)
		RadReviewMsg(uiDB_BAD_ERR, "FindDbStartDay", db_stat);

    return(uiDB_BAD_ERR);
}

/*===========================================================================
 *
 *  Name	 :  FindDbLastDay
 *
 *  Purpose	 :  Find the last day in the db for the specified station or the
 *				last day for any station.
 *				Returns the last time in the last day of the db.
 *				Times in database are not adjusted.  Must return adjusted times
 *				since everything runs on adjusted times.
 *
 *  Return value :  TRUE (found data)
 *					uiDB_EMPTY_STATUS
 *					uiNO_DATA_FOR_STATION_WARN
 *					errors from database call
 *  Special notes:
 *
 *
 *  Implicit Input :	StaID	input - station to look for (-1 = any station)
 *  Implicit Input/Output:
 *  Implicit Output:	*pdDay	output - julian time representing first time in day
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  30-May-95  SFK	 	Created from old GRAND Review FindDbStartDay
 *  20-Nov-98  SFK		Added concept of bErrDisplay to suppress error messages
 *	18-Dec-98  SFK		When looking for last time of any station, search through
 *						all stations for day for latest time in day
 *	21-Mar-2001 SFK		Version 2.07 Added hooks into data residing in Access database
 *	07-Aug-2002	SFK		Changed to DATE timebase and vectored all calls from
 *						other modules through GetDbTimeRange
 *	24-Sep-2003	SFK		Version 3.0.3 modified to support new FacMgr
 *
===========================================================================*/
static int FindDbLastDay(struct IDStruct StaID, DATE *pdDay, BOOL bErrDisplay)
{
	//int iComStatus = 0;
	int status, db_stat;
	DATE dTime;
	struct db_day_rec dbDay;
	unsigned long ulAccessEndDay = 0;
	BOOL bSameDay = FALSE;

	*pdDay = 0.0f;			// set to the earliest time possible
           
   /* ------------------------------------------------------------------
    *	Look in the COMs, if necessary
    * ----------------------------------------------------------------*/
	//DATE dComEndDay = 0.0;

	DATE dOthersEndDay = 999999999999.0;
	int iOthersComStatus = 0;

	DATE dEOSSEndDay = 999999999999.0;
	int iEOSSComStatus = 0;

	DATE dBinaryEndDay = 999999999999.0;
	int iBinaryComStatus = 0;

	DATE dComEndDay = 0.0;
	int iComStatus = 0;

	//EOSS does not use time align so we do it separately.
	if ((StaID.sStaID == -1) || (StaID.sStaType == EOSS_TYPE)) 
	{
		pglEOSSImport->GetDbEndDay(glsFacNum, StaID.sStaID, &dEOSSEndDay, bErrDisplay);
		if (dEOSSEndDay != 0.0)
			iEOSSComStatus = 1;
	}

	//Binary import needs to be divorced from TimeAlign too.
	if ((StaID.sStaID == -1) || (StaID.sStaType == BINARY_TYPE)) 
	{
		pglBinaryImport->GetDbEndDay(glsFacNum, StaID.sStaID, &dBinaryEndDay, bErrDisplay);
		if (dBinaryEndDay != 0.0)
			iBinaryComStatus = 1;
	}

	if ((StaID.sStaID == -1) || (pglTimeAlign->COMDataType(StaID.sStaType))) 
	{
		//This will check everything else.
		pglTimeAlign->GetDbEndDay( glsFacNum, StaID.sStaID, &dOthersEndDay,(bErrDisplay != 0),StaID.sStaType);
		if (dOthersEndDay != 0.0)
			iOthersComStatus = 1;

		if (iEOSSComStatus)
		{
			iComStatus = 1;
			dComEndDay = dEOSSEndDay;
		}

		if (iBinaryComStatus)
		{
			if (iComStatus == 0)
			{
				iComStatus = 1;
				dComEndDay = dBinaryEndDay;
			}
			else
			{
				if (dComEndDay < dBinaryEndDay)
					dComEndDay = dBinaryEndDay;
			}
		}

		if (iOthersComStatus)
		{
			if(iComStatus == 0)
			{
				iComStatus = 1;
				dComEndDay = dOthersEndDay;
			}
			else
			{
				if (dComEndDay < dOthersEndDay)
					dComEndDay = dOthersEndDay;
			}
		}

/*
		if (iOtherComStatus && iEOSSComStatus)  //both non-zero
		{
			if (dOthersEndDay >= dEOSSEndDay)
				dComEndDay = dOthersEndDay;
			else
				dComEndDay = dEOSSEndDay;

			iComStatus = 1;
		}
		else if (!iOtherComStatus && !iEOSSComStatus) //both zero -> no data
		{
			dComEndDay = 0.0;
			iComStatus = 0;
		}
		else if (iOtherComStatus)
		{
			dComEndDay = dOthersEndDay;
			iComStatus = 1;
		}
		else if (iEOSSComStatus)
		{
			dComEndDay = dEOSSEndDay;
			iComStatus = 1;
		}
*/
		*pdDay = dComEndDay;


		if (StaID.sStaID != -1)
		{
			if (StaID.sStaType == EOSS_TYPE)
				return iComStatus;

			if (StaID.sStaType == BINARY_TYPE)
				return iComStatus;

			if (pglTimeAlign->COMDataType(StaID.sStaType))
				return iComStatus;
		}
	}
   /* ------------------------------------------------------------------
    *	If necessary, look in main rad db.
    *	Find if any days exist in the Rad main database.
    * ----------------------------------------------------------------*/
    dTime = 0x7fffffff;

    db_stat = d_keyfind(D_DAY_BEG_TIME_KEY, &dTime, CURR_DB);
    if (db_stat == S_NOTFOUND) 
		db_stat = d_keyprev(D_DAY_BEG_TIME_KEY, CURR_DB);

    if (db_stat == S_NOTFOUND) 	// no data in the main db
	{
		if (iComStatus == 1) // found information in the COMs
		{
			if (dComEndDay > *pdDay) 
				*pdDay = dComEndDay;

			return 1; 
		}
		// no data in either the COMs or the main rad database
       	if (bErrDisplay)
			RadReviewMsg(uiDB_EMPTY_STATUS);

    	return(uiDB_EMPTY_STATUS);
    }

    if (db_stat != S_OKAY) 
		goto db_exit;
    
   /* ------------------------------------------------------------------
    *	If just looking for the ending time of any station in the db have
    *	found it, read and return the time for the last time now.  Must look
	*	through all stations for this day.
    * ----------------------------------------------------------------*/
    if (StaID.sStaID == -1) 
	{
		db_stat = d_recread(&dbDay, CURR_DB);
		if (db_stat != S_OKAY) 
			goto db_exit;

		*pdDay = dbDay.d_day_end_time;
		*pdDay = AdjustTimestamp(StaID, *pdDay);
		// have now found a record for the last day in the db, look
		// through all the other records for this day to make sure the
		// ending time is not after the one for the station you just found
		do 
		{
			db_stat = d_keyprev(D_DAY_BEG_TIME_KEY, CURR_DB); // check if previous rec in same day
			if ((db_stat == S_NOTFOUND) || (db_stat != S_OKAY)) 
				break;	// record that the read was okay or error in db read

			db_stat = d_recread(&dbDay, CURR_DB);

			//<<QA>> this BOOL bSameDay is never used.
			bSameDay = SameDay(*pdDay, dbDay.d_day_end_time);	// 24-May-2005 use function for SameDay
			if (dbDay.d_day_end_time > *pdDay) 
				*pdDay = dbDay.d_day_end_time;

		} while(db_stat == S_OKAY);

		if ((db_stat != S_OKAY) && (db_stat != S_NOTFOUND)) 
			goto db_exit;

		// have found the last time in the main RAD db, check if any COM later
		//return the first time found in either db
		if ((iComStatus == 1) && (dComEndDay > *pdDay)) 
			*pdDay = dComEndDay;

		return 1;
    }	

   /* ------------------------------------------------------------------
    *	Are looking for a particular station.  Find that station and see if
    *	there is any day under it.  If yes read and return the julian
    *	time of the last day record.
    * ----------------------------------------------------------------*/
	status = StationExistsInMainDb(StaID.sStaID);
    if (status != 1) 
		return (status);

    db_stat = d_setor(STATION_TO_DAY_SET, CURR_DB);
    if (db_stat != S_OKAY) 
		goto db_exit;

    db_stat = d_findlm(STATION_TO_DAY_SET, CURR_DB);
    if (db_stat == S_EOS)
        return(uiNO_DATA_FOR_STATION_WARN);
	
	if (db_stat != S_OKAY) 
		goto db_exit;

    db_stat = d_crread(D_DAY_END_TIME, (char *)pdDay, CURR_DB);
	if (db_stat != S_OKAY) 
		goto db_exit;

	*pdDay = AdjustTimestamp(StaID, *pdDay);

	// see if the COM time is later
	if ((iComStatus == 1) && (dComEndDay > *pdDay))
		*pdDay = dComEndDay;
 
    return 1;
        
db_exit:
    if (bErrDisplay)
		RadReviewMsg(uiDB_BAD_ERR,"FindDbLastDay", db_stat);
    return(uiDB_BAD_ERR);
}

