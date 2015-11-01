// BaseVistaData.cpp: implementation of the CBaseVistaData class.

///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support generic data functions for 
//	instruments with data in dbVista databases.
//
//	Functions:
//		FindDayContainingClosestRecord
//		DayExists (2)
//		StationDataExists
//		SameDay
//		DayStart
//		GetLastTimeInDb
//		GetFirstTimeInDb
//		AnyDaysExistInDb
//		DbVistaError
//		CheckDbOpen
//		DeleteDay
//		CreateDay
//		GetDayData (2)
//		AddDayData
//		GetDayStatistics
//		
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GenDateTime.h"
#include "VistaBaseData.h"
#include <math.h>

#define Currtask (mpDb->m_Currtask)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseVistaData::CBaseVistaData(
	CDbVista* pDb, 
	const int iStaNum, 
	const bool bInterpolated,
	const bool bQuietMode)
{
	mbInterpolated = bInterpolated;
	miErrorNum = 0;
	mpDb = pDb;
	msStaNum = (short)iStaNum;
	mulNumPtsActual = 0;
	mulNumPtsRequested = 0;
	m_bQuietMode = bQuietMode;
}

CBaseVistaData::~CBaseVistaData()
{
}

///////////////////////////////////////////////////////////////////////////
//	Name:	FindDayContainingClosestRecord
//
//	Description:
//	Find the day record linked to the station that contains the closest record to the
//	time.  If the record does not fall within the data for the day, then based on
//	the iSnap parameter, it may be necessary to go to the next or previous day(s).
//	
//	Declaration:
//	int CBaseVistaData::FindDayContainingClosestRecord(DATE dRequestedTime, int iSnap, DATE *pdActualTime)
//
//	Input:	dStartTime			start time for search interval
//			dRequestedEndTime	end time for search interval
//
//	Output:	pdActualEndTime	time of actual point in database <= ulRequestedEndTime
//
//	Return:	iDAY_IN_DB			expected return value:day meeting end conditions found in db
//			iBAD_INTERVAL		start > end in requested interval
//			iDAY_NOT_IN_DB		no days for this station between start and end in db
//			iDB_BAD				dbVista error
//			iDB_NOT_OPEN		database not open
//			iSTA_NOT_IN_DB		station not in db
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from FindDayContainingClosestEndTime in DbInterval.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
int CBaseVistaData::FindDayContainingClosestRecord(DATE dRequestedTime, SNAP eSnap, DATE *pdActualTime)
{
	db_day_rec dbDay;
	DATE dAdjRequestedTime, dLastTimeInDb, dFirstTimeInDb;

	//////////////////////////////////////////////////////////////////////////////
	//Repaired problem with requested review period 
	//ending on a day boundary.
	//Version 1.0.3.16
	//pjm 3-8-07
	//ADDED THE FOLLOWING:
	if (eSnap == BEFORE)  //end point of interval
	{
		//if the requested end point is on a day boundary, back it off one second.
		int truncated = (int)dRequestedTime;
		if((dRequestedTime - (double)truncated) == 0.0)
			dRequestedTime = dRequestedTime - (double)(0.000011574074074074); //one second
	}
	//END Added for Version 1.0.3.16
	////////////////////////////////////////////////////////////////////////////////////

	*pdActualTime = dRequestedTime;
	dAdjRequestedTime = dRequestedTime;

	if (!CheckDbOpen()) return(iDB_NOT_OPEN);
       
	//	If the sSta is not in the database, can't do anything
	if (!StationDataExists()) return(iSTA_NOT_IN_DB);

	// find day for station containing the requested time
	int iStatus = DayExists(dRequestedTime);
    if (iStatus == iDB_BAD) return(iDB_BAD);

	if (iStatus == iDAY_IN_DB) 
	{
		if (d_recread((char*)&dbDay, CURR_DB) != S_OKAY) return(iDB_BAD);

		// see if the time we are looking for is between the starting and ending times
		//	of this day, if yes, we are finished.
		if ((dRequestedTime >= dbDay.d_day_beg_time) && (dRequestedTime <= dbDay.d_day_end_time)) return (iDAY_IN_DB);

		// not between the starting and ending times, now check if this is still the
		// correct day depending on the iSnap parameter.
		if ((dRequestedTime < dbDay.d_day_beg_time) && (eSnap == AFTER)) return(iDAY_IN_DB);
		if ((dRequestedTime > dbDay.d_day_end_time) && (eSnap == BEFORE)) return(iDAY_IN_DB);

		//Removed the following line for Version 1.0.3.16  pjm 3-8-07 
		//if (floor(dRequestedTime) == dbDay.d_day_beg_time_key) return(iDAY_IN_DB);		// 01-JAN-2005 Do a final check if it is in the day
	}

	// none of the above conditions matched, now must move to a previous or following day in the database.
	// if there is missing data, may move several days.
	//dAdjRequestedTime = dRequestedTime;

	if (eSnap == AFTER) {	// move forward until hit a day with data or the end of the db
		GetLastTimeInDb(&dLastTimeInDb, false);
		if (dAdjRequestedTime >= dLastTimeInDb) {	// set to the end time if beyond end of db
			DayExists(dLastTimeInDb);				// get to the end record
			if (d_recread((char*)&dbDay, CURR_DB) != S_OKAY) return(iDB_BAD);	// and read it
			*pdActualTime = dbDay.d_day_end_time;
			return(iDAY_IN_DB);
		}
		do {
			dAdjRequestedTime += 1.0;	// add one day to the search time
		} while (DayExists(dAdjRequestedTime) == iDAY_NOT_IN_DB);
		if (d_recread((char*)&dbDay, CURR_DB) != S_OKAY) return(iDB_BAD);	// read the record for the day
		*pdActualTime = dbDay.d_day_beg_time;
		return(iDAY_IN_DB);
	}
	else {		//eSnap == BEFORE	// move backward until hit a day with data or the beginning of the db
		GetFirstTimeInDb(&dFirstTimeInDb, false);
		if (dAdjRequestedTime <= dFirstTimeInDb) {	// set to beginning time if before beginning of db
			DayExists(dFirstTimeInDb);				// get to the first record
			if (d_recread((char*)&dbDay, CURR_DB) != S_OKAY) return(iDB_BAD);	// and read it
			*pdActualTime = dbDay.d_day_beg_time;
			return(iDAY_IN_DB);
		}
		do {
			dAdjRequestedTime -= 1.0;	// subtract one day from search time
		} while (DayExists(dAdjRequestedTime) == iDAY_NOT_IN_DB);
		if (d_recread((char*)&dbDay, CURR_DB) != S_OKAY) return(iDB_BAD);	// read the record for the day
		*pdActualTime = dbDay.d_day_end_time;
		return(iDAY_IN_DB);
	}
}



///////////////////////////////////////////////////////////////////////////
//	Name:	DayExists
//
//	Description:
//	Determine if a dbDay_rec containing the given time exists in the database
//	for the station we are interested in (msStaNum)
//
//  Special notes:  if TRUE, curr_rec = day looking for
//	
//	Declaration:
//	int CBaseVistaData::DayExists(DATE dTime)
//
//	Input:	dTime				time to search for in the day records
//
//	Output:	none
//
//	Return:	iDAY_NOT_IN_DB or iDAY_IN_DB expected return based on whether day in db
//			iSTA_NOT_IN_DB		msStaNum unknown in database
//			iDB_BAD				dbVista database error
//			iDB_NOT_OPEN		database not open
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from FindDayRec in DbDay.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
int CBaseVistaData::DayExists(DATE dTime)
{

    DATE dDayBegTime;
    int db_stat;             
    short db_sSta;

	if (!CheckDbOpen()) return(iDB_NOT_OPEN);
        
   /* ------------------------------------------------------------------
    *	If the sSta is not in the database, can't do anything
    * ----------------------------------------------------------------*/
	if (!StationDataExists()) return(iSTA_NOT_IN_DB);
        
   /* ------------------------------------------------------------------
    *	Found the sSta, make it the owner of the set to search
    * ----------------------------------------------------------------*/
    if (d_setor(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) return(iDB_BAD);
                                       
   /* ------------------------------------------------------------------
    *	Search through the day records attached to this sSta.  Read
    *   out the time associated with the day and convert it to the
    *   jul secs corresponding to day start.  If this matches the
    *	day looking for then have found it.
    * ----------------------------------------------------------------*/
	// find first member in the station-day set for the given station
	db_stat = d_findfm(STATION_TO_DAY_SET, CURR_DB);
    if (db_stat == S_EOS) return(iDAY_NOT_IN_DB);		// no days at all in db  
    if (db_stat != S_OKAY) return(iDB_BAD);

	db_day_rec dbDay;
	d_recread((char*)&dbDay, CURR_DB);		//debug
	DB_ADDR dba;
	d_crget(&dba, CURR_DB);
	// Get the time of that first day record
	if (d_crread(D_DAY_BEG_TIME_KEY, &dDayBegTime, CURR_DB) != S_OKAY) return(iDB_BAD);                      
	d_recread((char*)&dbDay, CURR_DB);		//debug
	if (SameDay(dDayBegTime,dTime)) {	// same day, now verify proper owner
    		if (d_csoread(STATION_TO_DAY_SET, S_STA_NUM_KEY, &db_sSta, CURR_DB) != S_OKAY) return(iDB_BAD);
    		if (db_sSta == msStaNum ) return(iDAY_IN_DB);
    }	
	// wasn't the first day, now look through the others day records
    do {
		db_stat = d_findnm(STATION_TO_DAY_SET, CURR_DB);    
		if (db_stat == S_EOS) return(iDAY_NOT_IN_DB);    
    	if (db_stat != S_OKAY) return(iDB_BAD);	    
		if (d_crread(D_DAY_BEG_TIME_KEY, &dDayBegTime, CURR_DB) != S_OKAY) return(iDB_BAD);
		if (SameDay(dDayBegTime,dTime)) { // same day, now verify proper owner
    		if (d_csoread(STATION_TO_DAY_SET, S_STA_NUM_KEY, &db_sSta, CURR_DB) != S_OKAY) return(iDB_BAD);
    		if (db_sSta == msStaNum ) return(iDAY_IN_DB);
    	}	
    } while (db_stat == S_OKAY);	
	
	// no records for this day in the db
	return(iDAY_NOT_IN_DB);

}

///////////////////////////////////////////////////////////////////////////
//	Name:	DayExists
//
//	Description:
//	Determine if a dbDay_rec containing the given time exists in the database
//	and find the first and last time in the day.
//
//  Special notes:  if TRUE, curr_rec = day looking for
//	
//	Declaration:
//	int CBaseVistaData::DayExists(DATE dTime, unsigned long *pulNumPts, DB_D_INTERVAL *pdActual)
//
//	Input:	dTime				time to search for in the day records
//
//	Output:	pActual				start and end time of data already in the day
//
//	Return:	iDAY_NOT_IN_DB or iDAY_IN_DB expected return based on whether day in db
//			iSTA_NOT_IN_DB		msStaNum unknown in database
//			iDB_BAD				dbVista database error
//			iDB_NOT_OPEN		database not open
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from FindDayRec in DbDay.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
int CBaseVistaData::DayExists(DATE dTime, unsigned long *pulNumPts, DB_D_INTERVAL *pdActual)
{
	
	db_day_rec dbDay;

    int db_stat;             
    short db_sSta;

	pdActual->dStart = 0.0;
	pdActual->dEnd = 0.0;
	*pulNumPts = 0;

	if (!CheckDbOpen()) return(iDB_NOT_OPEN);
        
   /* ------------------------------------------------------------------
    *	If the sSta is not in the database, can't do anything
    * ----------------------------------------------------------------*/
	if (!StationDataExists()) return(iSTA_NOT_IN_DB);
        
   /* ------------------------------------------------------------------
    *	Found the sSta, make it the owner of the set to search
    * ----------------------------------------------------------------*/
    if (d_setor(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) return(iDB_BAD);
                                       
   /* ------------------------------------------------------------------
    *	Search through the day records attached to this sSta.  Read
    *   out the time associated with the day and convert it to the
    *   jul secs corresponding to day start.  If this matches the
    *	day looking for then have found it.
    * ----------------------------------------------------------------*/
	// find first member in the station-day set for the given station
	db_stat = d_findfm(STATION_TO_DAY_SET, CURR_DB);
    if (db_stat == S_EOS) return(iDAY_NOT_IN_DB);		// no days at all in db  
    if (db_stat != S_OKAY) return(iDB_BAD);

	// Get the time of that first day record
	#pragma pack(1)
	if (d_recread(&dbDay, CURR_DB) != S_OKAY) return(iDB_BAD);
	#pragma pack(8)
	//if (d_crread(D_DAY_BEG_TIME_KEY, &uldbDay, CURR_DB) != S_OKAY) return(iDB_BAD);                      
	if (SameDay(dbDay.d_day_beg_time_key,dTime)) {	// same day, now verify proper owner
    		if (d_csoread(STATION_TO_DAY_SET, S_STA_NUM_KEY, &db_sSta, CURR_DB) != S_OKAY) return(iDB_BAD);
			pdActual->dStart = dbDay.d_day_beg_time;
			pdActual->dEnd = dbDay.d_day_end_time;
			*pulNumPts = dbDay.ul_day_total_pts;
    		if (db_sSta == msStaNum ) return(iDAY_IN_DB);
    }	
	// wasn't the first day, now look through the others day records
    do {
		db_stat = d_findnm(STATION_TO_DAY_SET, CURR_DB);    
		if (db_stat == S_EOS) return(iDAY_NOT_IN_DB);    
    	if (db_stat != S_OKAY) return(iDB_BAD);	    
		if (d_recread((char*)&dbDay, CURR_DB) != S_OKAY) return(iDB_BAD);
		//if (d_crread(D_DAY_BEG_TIME_KEY, &uldbDay, CURR_DB) != S_OKAY) return(iDB_BAD);
		if (SameDay(dbDay.d_day_beg_time_key,dTime)) { // same day, now verify proper owner
    		if (d_csoread(STATION_TO_DAY_SET, S_STA_NUM_KEY, &db_sSta, CURR_DB) != S_OKAY) return(iDB_BAD);
			pdActual->dStart = dbDay.d_day_beg_time;
			pdActual->dEnd = dbDay.d_day_end_time;
			*pulNumPts = dbDay.ul_day_total_pts;
    		if (db_sSta == msStaNum ) return(iDAY_IN_DB);
    	}	
    } while (db_stat == S_OKAY);	
	
	// no records for this day in the db
	return(iDAY_NOT_IN_DB);

}



///////////////////////////////////////////////////////////////////////////
//	Name:	StationDataExists
//
//	Description:
//	Is msStaNum known to this database?  If yes, it will be in the framework.
//	
//	Declaration:
//	bool CBaseVistaData::StationDataExists()
//
//	Input:	none
//
//	Output:	none
//			miErrorNum - iDB_NOT_OPEN if db not open
//
//	Return:	true (msStaNum has record in database) / false (unknown in database or db error)
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from StationDataExists in DbStation.cpp
//////////////////////////////////////////////////////////////////
bool CBaseVistaData::StationDataExists()
{
	int db_stat;

	if (!CheckDbOpen()) {
		miErrorNum = iDB_NOT_OPEN;
		return(false);
	}

	db_stat = d_keyfind(S_STA_NUM_KEY,(char *)&msStaNum, CURR_DB);
    if (db_stat == S_OKAY) 
		return (true);
	miErrorNum = iSTA_NOT_IN_DB;
	return(false);
}




///////////////////////////////////////////////////////////////////////////
//	Name:	SameDay
//
//	Description:
//	Check if the two times belong to the same day.
//	
//	Declaration:
//	bool CBaseVistaData::SameDay(DATE dTime1, DATE dTime2)
//
//	Input:	dTime1		first time to compare
//			dTime2		second time to compare
//
//	Output:	none
//
//	Return:	true (times in same day) / false (times not in same day)
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from SameDay in DbDay.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
bool CBaseVistaData::SameDay(DATE dTime1, DATE dTime2)
{

   /* ------------------------------------------------------------------
    *	Find the number of seconds at the start of the day that we're
    *	looking for since this must be an exact match in the database.
    *	Convert the time first to ints and then back to secs starting at
    *	0hr 0min 0sec
    * ----------------------------------------------------------------*/
	if ((int)dTime1 != (int)(dTime2)) return(false);
    return(true);
}




///////////////////////////////////////////////////////////////////////////
//	Name:	DayStart
//
//	Description:
//	Determine the start time (00:00:00) of the day associated with the given time
//	
//	Declaration:
//	DATE CBaseVistaData::DayStart(DATE dTime)
//
//	Input:	dTime	time of which to find the associated day start
//
//	Output:	none
//
//	Return:	time of day start
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from CvtTimeToDayStart in DbDay.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
DATE CBaseVistaData::DayStart(DATE dTime)
{

   /* ------------------------------------------------------------------
    *	Find the number of seconds at the start of the day that we're
    *	looking for since this must be an exact match in the database.
    *	Convert the time first to ints and then back to secs starting at
    *	0hr 0min 0sec
    * ----------------------------------------------------------------*/
	int iTemp = (int)dTime;
	return((double)iTemp);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetLastTimeInDb
//
//	Description:
//	Find the last time in the last day in the db for the specified
//	station or the last time for any station.
//	Returns the last time in the last day of the db.
//	
//	Declaration:
//	bool CBaseVistaData::GetLastTimeInDb(DATE *pdLastTime, bool bDisplayErr)
//
//	Input:	bDisplayError	whether to display a message if the database is empty
//
//	Output:	pdLastTime		last time in last day of station or entire database
//			miErrorNum could be iDB_NOT_OPEN, iDB_EMPTY, iDB_BAD, iSTA_NOT_IN_DB,
//							iNO_DATA_FOR_STATION
//
//	Return:	true (found a last time) / false (error in finding the result -- see miErrorNum)
//	
//  date    /	author	revision
//  -----------------	--------
//	04-Dec-2001	SFK		Created from FindDbLastDay in DbLimit.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
bool CBaseVistaData::GetLastTimeInDb(DATE *pdLastTime, bool bDisplayErr)
{
	int db_stat;
	struct db_day_rec dbDay;
	CString Msg;

	*pdLastTime = 0.0;

	if (!CheckDbOpen()) {
		miErrorNum = iDB_NOT_OPEN;
		return(false);
	}

	if (!AnyDaysExistInDb(DB_END, false)) {	// positioned at the record with the latest end time in the db after this
		miErrorNum = iDB_EMPTY;		
		return(false);
	}

   /* ------------------------------------------------------------------
    *	If just looking for the ending time of any station in the db have
    *	found it, read and return the time for the last time now.  Must look
	*	through all stations for this day.
    * ----------------------------------------------------------------*/
    if (msStaNum == ANY_STATION) {
		if (db_stat = d_recread(&dbDay, CURR_DB) != S_OKAY) {	// read the last record in the db
			miErrorNum = iDB_BAD;		
			return(false);
		}
		*pdLastTime = dbDay.d_day_end_time;
		return(true);
    }	

   /* ------------------------------------------------------------------
    *	Are looking for a particular station.  Find that station and see if
    *	there is any day under it.  If yes read and return the julian
    *	time of the last day record.
    * ----------------------------------------------------------------*/
    if (!StationDataExists()) {
		miErrorNum = iSTA_NOT_IN_DB;		
		return(false);
	}
	//	are pointing at the station record, set it as owner of the day set
    if (d_setor(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) {
		miErrorNum = iDB_BAD;		
		return(false);
	}
	// find the last member in the day set for the station
    db_stat = d_findlm(STATION_TO_DAY_SET, CURR_DB);
    if (db_stat == S_EOS) {
		miErrorNum = iNO_DATA_FOR_STATION;
		return(false);
	}
	if (db_stat != S_OKAY) {
		miErrorNum = iDB_BAD;
		return(false);
	}
	if (d_crread(D_DAY_END_TIME, (char *)pdLastTime, CURR_DB) != S_OKAY) {
		miErrorNum = iDB_BAD;		
		return(false);
	}
    return(true);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetFirstTimeInDb
//
//	Description:
//	Find the first time in the first day in the db for the specified
//	station or the first time for any station.
//	
//	Declaration:
//	bool CBaseVistaData::GetFirstTimeInDb(DATE *pdFirstTime, bool bDisplayErr)
//
//	Input:	bDisplayError	whether to display a message if the database is empty
//
//	Output:	pdFirstTime	first time in first day of station or entire database
//			miErrorNum could be iDB_NOT_OPEN, iDB_EMPTY, iDB_BAD, iSTA_NOT_IN_DB,
//							iNO_DATA_FOR_STATION
//
//	Return:	true (found a first time) / false (error in finding the result -- see miErrorNum)
//	
//  date    /	author	revision
//  -----------------	--------
//	04-Dec-2001	SFK		Created from GetFirstTimeInDb in BaseVistaData.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
bool CBaseVistaData::GetFirstTimeInDb(DATE *pdFirstTime, bool bDisplayErr)
{
	int db_stat;
	struct db_day_rec dbDay;
	CString Msg;

	*pdFirstTime = 99999.0;

	if (!CheckDbOpen()) {
		miErrorNum = iDB_NOT_OPEN;
		return(false);
	}

	if (!AnyDaysExistInDb(DB_START, false)) {	// positioned at the record with the first time in the db after this
		miErrorNum = iDB_EMPTY;		
		return(false);
	}

   /* ------------------------------------------------------------------
    *	If just looking for the starting time of any station in the db have
    *	found it, read and return the time for the start time now. Search
	*	thru all stations for this day
    * ----------------------------------------------------------------*/
    if (msStaNum == ANY_STATION) {
		if (db_stat = d_recread(&dbDay, CURR_DB) != S_OKAY) {	// read the first record in the db
			miErrorNum = iDB_BAD;		
			return(false);
		}
		*pdFirstTime = dbDay.d_day_beg_time;
		return(true);
    }	

   /* ------------------------------------------------------------------
    *	Are looking for a particular station.  Find that station and see if
    *	there is any day under it.  If yes read and return the julian
    *	time of the last day record.
    * ----------------------------------------------------------------*/
    if (!StationDataExists()) {
		miErrorNum = iSTA_NOT_IN_DB;		
		return(false);
	}
	//	are pointing at the station record, set it as owner of the day set
    if (d_setor(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) {
		miErrorNum = iDB_BAD;		
		return(false);
	}
	// find the first member in the day set for the station
    db_stat = d_findfm(STATION_TO_DAY_SET, CURR_DB);
    if (db_stat == S_EOS) {
		miErrorNum = iNO_DATA_FOR_STATION;
		return(false);
	}
	if (db_stat != S_OKAY) {
		miErrorNum = iDB_BAD;
		return(false);
	}
	if (d_crread(D_DAY_BEG_TIME, (char *)pdFirstTime, CURR_DB) != S_OKAY) {
		miErrorNum = iDB_BAD;		
		return(false);
	}
    return(true);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	AnyDaysExistInDb
//
//	Description:
//	Determine whether there are any day records at all for any station in the database
//	Depending on iDbEnd, either be positioned at the beginning
//	or end of the database after the check.
//	
//	Declaration:
//	bool CBaseVistaData::AnyDaysExistInDb(int iDbEnd, bool bShowMsg)
//
//	Input:	iDbEnd			whether to search for the beginning or end of the database
//			bShowMsg		whether to display message if no records
//
//	Output:	none
//			miErrorNum could be iDB_NOT_OPEN, iDB_EMPTY, iDB_BAD
//
//	Return:	true (found day record) / false (no day record in the db)
//	
//  date    /	author	revision
//  -----------------	--------
//	04-Dec-2001	SFK		Created from part of FindDbLastDay in DbLimit.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//	10-Jan-2005 SFK		Return the records with the earliest value for d_day_beg_time and the 
//						latest value for d_day_end_time -- not just the first or last record 
//						in the database
//////////////////////////////////////////////////////////////////
bool CBaseVistaData::AnyDaysExistInDb(int iDbEnd, bool bShowMsg)
{
	CString Msg;
	int db_stat;
	DB_ADDR dba;
	double dTime, dDay;
	struct db_day_rec dbDay;

	if (!CheckDbOpen()) {
		miErrorNum = iDB_NOT_OPEN;
		return(false);
	}

   /* ------------------------------------------------------------------
    *	Find if any days exist in the database.
    * ----------------------------------------------------------------*/
    if (iDbEnd == DB_END) {
		DATE dSearchTime = 99999.0;
		db_stat = d_keyfind(D_DAY_BEG_TIME_KEY, &dSearchTime, CURR_DB);
		if (db_stat == S_NOTFOUND) db_stat = d_keyprev(D_DAY_BEG_TIME_KEY, CURR_DB);
		if (db_stat == S_NOTFOUND) {           
       		if (bShowMsg) {
				Msg.Format("No data found in the database at\n %s.\nPlease import data or select another database.", mpDb->mstrDatabasePath);
				/*NOT FOR NDAR
				MessageBox(NULL, Msg, "Database Empty", MB_OK|MB_ICONWARNING);
				*/
			}
			miErrorNum = iDB_EMPTY;
   			return(false);
		}
		// 10-Jan-2005 SFK  Have now found a record for the last day in the database -- but must find the station
		//	that has the last end time for this day. So need to search around the records for this day
		db_stat = d_recread(&dbDay, CURR_DB);						// read the record for the last day in the day
		if (db_stat != S_OKAY) return (false); 
		dTime = dbDay.d_day_end_time;						// end time associated with day we just found
		dDay = dbDay.d_day_beg_time_key;
		if (d_crget(&dba, CURR_DB) != S_OKAY) return(false);		// remember db address
		do {
			db_stat = d_keyprev(D_DAY_BEG_TIME_KEY, CURR_DB);		// go back one record
			if (db_stat == S_OKAY) {
				db_stat = d_recread(&dbDay, CURR_DB);				// read the record 
				if (db_stat == S_OKAY) {
					if (dbDay.d_day_end_time > dTime) {		// end time in this day is later than the one we found
						dTime = dbDay.d_day_end_time;		// remember new end time associated with day we just found
						if (d_crget(&dba, CURR_DB) != S_OKAY) return(false);	// remember db address
					}	
				}
			}
		} while ((db_stat == S_OKAY) && (dDay == dbDay.d_day_beg_time_key));
		if (d_crset(&dba, CURR_DB) != S_OKAY) return(false);
		db_stat = d_recread(&dbDay, CURR_DB);						// debug
		

	}
    if (iDbEnd == DB_START) {
		DATE dSearchTime = 0.0;
		db_stat = d_keyfind(D_DAY_BEG_TIME_KEY, &dSearchTime, CURR_DB);
		if (db_stat == S_NOTFOUND) db_stat = d_keynext(D_DAY_BEG_TIME_KEY, CURR_DB);
		if (db_stat == S_NOTFOUND) {           
       		if (bShowMsg) {
				Msg.Format("No data found in the database at\n %s.\nPlease import data or select another database.", mpDb->mstrDatabasePath);
				/*NOT FOR NDAR
				MessageBox(NULL, Msg, "Database Empty", MB_OK|MB_ICONWARNING);
				*/
			}
			miErrorNum = iDB_EMPTY;
   			return(false);
		}
		// 10-Jan-2005 SFK  Have now found a record for the first day in the database -- but must find the station
		//	that has the earliest start time for this day. So need to search around the records for this day
		db_stat = d_recread(&dbDay, CURR_DB);						// read the record for the last day in the day
		if (db_stat != S_OKAY) return (false); 
		dTime = dbDay.d_day_beg_time;						// start time associated with day we just found
		dDay = dbDay.d_day_beg_time_key;
		if (d_crget(&dba, CURR_DB) != S_OKAY) return(false);		// remember db address
		do {
			db_stat = d_keynext(D_DAY_BEG_TIME_KEY, CURR_DB);		// go forward one record
			if (db_stat == S_OKAY) {
				db_stat = d_recread(&dbDay, CURR_DB);				// read the record 
				if (db_stat == S_OKAY) {
					if (dbDay.d_day_beg_time < dTime) {		// start time in this day is before the one we found
						dTime = dbDay.d_day_beg_time;		// remember new start time associated with day we just found
						if (d_crget(&dba, CURR_DB) != S_OKAY) return(false);	// remember db address
					}	
				}
			}
		} while ((db_stat == S_OKAY) && (dDay == dbDay.d_day_beg_time));
		if (d_crset(&dba, CURR_DB) != S_OKAY) return(false);
		db_stat = d_recread(&dbDay, CURR_DB);						// debug

	}

    if (db_stat != S_OKAY) 
	{
		DbVistaError(&Msg,bShowMsg);
		/*NOT FOR NDAR
       	if (bShowMsg) 
			MessageBox(NULL, Msg, "Database Error", MB_OK|MB_ICONWARNING);
		*/

		miErrorNum = iDB_BAD;
		return(false);
	}
	return(true);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	DbVistaError
//
//	Description:
//	Generate a generic error message for database errors.
//	
//	Declaration:
//	void CBaseVistaData::DbVistaError(CString *pstrMsg, bool bShowMsg)
//
//	Input:	bShowMsg	whether to display message if no records
//
//	Output:	pstrMsg		Error message
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	06-Dec-2001	SFK		Created
//////////////////////////////////////////////////////////////////
void CBaseVistaData::DbVistaError(CString *pstrMsg, bool bShowMsg)
{
	miErrorNum = iINTERNAL_ERROR;
	if (pstrMsg) pstrMsg->Format("Internal dbVista error, database may be corrupted.");
	// WAS "NOT FOR NDAR" - Reinserted 11/28/2007 pjm for B2R1
	if (bShowMsg && !m_bQuietMode) 
		MessageBox(NULL, *pstrMsg, "Database Error", MB_OK|MB_ICONWARNING);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	CheckDbOpen
//
//	Description:
//	Determine whether the dbVista associated with the data is open
//	
//	Declaration:
//	bool CBaseVistaData::CheckDbOpen()
//
//	Input:	none
//			mpDb - pointer to dbVista database being used now
//
//	Output:	none
//
//	Return:	true (all okay) / false (problem with db)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Dec-2001	SFK		Created
//////////////////////////////////////////////////////////////////
bool CBaseVistaData::CheckDbOpen(bool bShowMsg)
{
	CString strError;

	if (!mpDb->mbDatabaseOpen && bShowMsg) 
	{
		strError.Format("Error: dbVista database at %s\nis not open, cannot proceed. Fix program.", mpDb->mstrDatabasePath);

		/*NOT FOR NDAR
		if (bShowMsg) 
			MessageBox(NULL, strError, "Database Error", MB_OK|MB_ICONSTOP);
		*/
	}
	return(mpDb->mbDatabaseOpen);

}


///////////////////////////////////////////////////////////////////////////
//	Name:	DeleteDay
//
//	Description:
//	Find all data records associated with specified day and delete them from the database.
//
//  Special notes:  if TRUE, curr_rec = day looking for
//	
//	Declaration:
//	int CBaseVistaData::DeleteDay(DATE dDay)
//
//	Input:	ulDay				day for which to delete all associated records for msStaNum				time to search for in the day records
//
//	Output:	none
//
//	Return:	iSTA_NOT_IN_DB		msStaNum unknown in database
//			iDB_BAD				dbVista database error
//			iDAY_NOT_IN_DB		no day in database containing ulTime
//			iDAY_IN_DB			day for station msStaNum containing ulTime exists in database
//	
//  date    /	author	revision
//  -----------------	--------
//	10-Dec-2001	SFK		Created from DeleteDbDay in DbDay.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
int CBaseVistaData::DeleteDay(DATE dDay)
{
    DB_ADDR dba;	/* db address of day trying to delete */
    int db_stat;
       
   /* ------------------------------------------------------------------
    *	If the sSta is not in the database, can't do anything
    * ----------------------------------------------------------------*/
	if (!StationDataExists()) return(iSTA_NOT_IN_DB);

   /* ------------------------------------------------------------------
    *	Read the station record to decide how to delete.
    *	Check if the day is in the database.
    * ----------------------------------------------------------------*/
	//	Find the day in the database
    int iStatus = DayExists(dDay);
    if (iStatus != iDAY_IN_DB) return(iStatus);
    
   /* ------------------------------------------------------------------
    *	Save db address of day, make it the owner of all data recs
    *	and then find, delete them
    * ----------------------------------------------------------------*/
    if (d_crget(&dba, CURR_DB) != S_OKAY) return(iDB_BAD);
    
	// 09-Aug-2005 SFK Add conditional compile based on the type of dbVista
	//	database that will be present for the various data types.  MINIGRAND,
	//	VIFM or SR are defined in the project settings for the various COMs
	#ifdef MINIGRAND
	if (d_setor(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) return(iDB_BAD);
	db_stat = d_findfm(DAY_TO_FLOAT_DATA_SET, CURR_DB);
	if (db_stat == S_OKAY) {
	    do {
			if (d_disdel(CURR_DB) != S_OKAY) return(iDB_BAD);
	    } while (d_findnm(DAY_TO_FLOAT_DATA_SET, CURR_DB) == S_OKAY);
	}
	else {
	    if (db_stat != S_EOS) return(iDB_BAD);
	}
	#endif

	#ifdef VIFM
	if (d_setor(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) return(iDB_BAD);
	db_stat = d_findfm(DAY_TO_FLOAT_DATA_SET, CURR_DB);
	if (db_stat == S_OKAY) {
	    do {
			if (d_disdel(CURR_DB) != S_OKAY) return(iDB_BAD);
	    } while (d_findnm(DAY_TO_FLOAT_DATA_SET, CURR_DB) == S_OKAY);
	}
	else {
	    if (db_stat != S_EOS) return(iDB_BAD);
	}
	#endif

   	#ifdef SR
	if (d_setor(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) return(iDB_BAD);
	db_stat = d_findfm(DAY_TO_DOUBLE_DATA_SET, CURR_DB);
	if (db_stat == S_OKAY) {
	    do {
			if (d_disdel(CURR_DB) != S_OKAY) return(iDB_BAD);
	    } while (d_findnm(DAY_TO_DOUBLE_DATA_SET, CURR_DB) == S_OKAY);
	}
	else {
	    if (db_stat != S_EOS) return(iDB_BAD);
	}
	#endif

   /* ---------------------------------------------------------------
    *	Have deleted all data associated with this day, restore
    *	address of day record and now delete the day record.
    * --------------------------------------------------------------*/
    if (d_crset(&dba, CURR_DB) != S_OKAY) return(iDB_BAD);
    if (d_disdel(CURR_DB) != S_OKAY) return(iDB_BAD);

    return(iDAY_IN_DB);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	CreateDay
//
//	Description:
//	Create a new day record and insert in the db.  If the day already exists in
//	the db, do nothing.
//
//  Special notes:  if TRUE, curr_rec = day looking for
//	
//	Declaration:
//	int CBaseVistaData::CreateDay(DATE dDay)
//
//	Input:	dDay				day for which to create an empty day in the database
//
//	Output:	none
//
//	Return:	iDAY_IN_DB			expected return: ulDay day for station msStaNum created (or exists)
//			iSTA_NOT_IN_DB		msStaNum unknown in database
//			iDB_BAD				dbVista database error
//			iDAY_NOT_IN_DB		no day in database containing ulTime
//			
//	
//  date    /	author	revision
//  -----------------	--------
//	10-Dec-2001	SFK		Created from CreateDayRec in DbDay.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
int CBaseVistaData::CreateDay(DATE dDay)
{
    struct db_day_rec dbDay;

   /* ------------------------------------------------------------------
    *	If the sSta is not in the database, can't do anything
    * ----------------------------------------------------------------*/
	if (!StationDataExists()) return(iSTA_NOT_IN_DB);
    
   /* ------------------------------------------------------------------
    *	If the day is already in the db do nothing.
    * ----------------------------------------------------------------*/
    int iStatus = DayExists(dDay);
    if (iStatus != iDAY_NOT_IN_DB) return(iStatus);
       
   /* ------------------------------------------------------------------
    *	Find the sSta that will own this day and make it the owner of
    *	the set.
    * ----------------------------------------------------------------*/
    if (d_keyfind(S_STA_NUM_KEY, (char *)&msStaNum, CURR_DB) != S_OKAY) return(iDB_BAD);
    if (d_setor(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) return(iDB_BAD);
       
   /* ------------------------------------------------------------------
    *	Fill the new record you want to write and connect into the database
    * ----------------------------------------------------------------*/
    dbDay.d_day_beg_time_key = dDay;
	dbDay.d_day_beg_time = dDay;
    dbDay.d_day_end_time = 0.0;
    dbDay.ul_day_total_pts = 0;
    if (d_fillnew(DB_DAY_REC,(char *)&dbDay, CURR_DB) != S_OKAY) return(iDB_BAD);
    if (d_connect(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) return(iDB_BAD);

	d_recread(&dbDay, CURR_DB);		// debug 
	DB_ADDR dba;
	d_crget(&dba, CURR_DB);
    
	return(iDAY_IN_DB);

}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDayData
//
//	Description:
//	If the day exists in the database, read the record associated with it.
//
//	Declaration:
//	int CBaseVistaData::GetDayData(DATE dDayStart, struct db_day_rec* pdbDay)
//
//	Input:	dDayStart			start time of day searching for
//
//	Output:	pdbDay				day record read from database
//
//	Return:	iDAY_IN_DB			expected return: ulDay day for station msStaNum read
//			any return from DayExists
//	
//  date    /	author	revision
//  -----------------	--------
//	17-Dec-2001	SFK		Created from CreateDayRec in DbDay.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
int CBaseVistaData::GetDayData(DATE dDayStart, struct db_day_rec* pdbDay)
{
	int iStatus;

	iStatus = DayExists(dDayStart);
	if (iStatus != iDAY_IN_DB) return(iStatus);

	if (d_recread(pdbDay, CURR_DB) != S_OKAY) return(iDB_BAD);      
	return(iDAY_IN_DB);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDayData
//
//	Description:
//	Get information about all the days in the database in the specified timerange.
//
//	Declaration:
//	bool CBaseVistaData::GetDayData(DB_D_INTERVAL Requested, DB_D_INTERVAL *pActual, DayDataStruct* pDayData, CString *pErrMsg)
//
//	Input:	Requested			Requested time looking for day information
//			mulNumPtsRequested	Number of days expected in interval
//			pDayData			array where to store the final results
//
//	Output:	pActual				Actual time range fo day data found
//			mulNumPtsActual		Number of days found in interval
//
//	Return:	true/false
//			miErrorNum
//				iNO_STORAGE			client did not allocate storage
//				any return except iDAY_NOT_IN_DB or iDAY_IN_DB from DayExists
//	
//  date    /	author	revision
//  -----------------	--------
//	18-Feb-2002	SFK		Created
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
bool CBaseVistaData::GetDayData(DB_D_INTERVAL Requested, DB_D_INTERVAL *pActual, DayDataStruct* pDayData, CString *pErrMsg)
{
	int iStatus;
	DATE dDay, dEndDayPlusOne;
	unsigned long ulNumPtsInDay;

	if (pErrMsg) pErrMsg->Empty();
	pActual->dStart = 0.0;
	pActual->dEnd = 0.0;
	if (pDayData == NULL) {
		if (pErrMsg) pErrMsg->Format(" GetDayData: Client did not allocate storage for results.");
		miErrorNum = iNO_STORAGE;
		return(false);
	}
	
	dDay = Requested.dStart;
	dEndDayPlusOne = (Requested.dEnd + 1.0);
	int i = 0;

	do {
		iStatus = DayExists(dDay, &ulNumPtsInDay, pActual);
		if (iStatus == iDAY_IN_DB) {
			pDayData[i].dFirstDataPt = pActual->dStart;
			pDayData[i].dLastDataPt = pActual->dEnd;
			pDayData[i].dNumPtsInDay = (double)ulNumPtsInDay;
			i++;
		}
		if ((iStatus == iDAY_IN_DB) || (iStatus == iDAY_NOT_IN_DB)) {
			dDay += 1.0;		// go on to next day
		}
		else {	// unexpected error from DayExists, have to quit
			if (pErrMsg) pErrMsg->Format(" GetDayData: Unexpected error of %d from DayExists.", iStatus);
			miErrorNum = iStatus;
			mulNumPtsActual = i;
			return(false);
		}
	} while (!SameDay(dDay, dEndDayPlusOne));
	mulNumPtsActual = i;	// number of days found in interval
	return(true);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	AddDayData
//
//	Description:
//	Write the day record to the data base.
//
//	Declaration:
//	int CBaseVistaData::AddData(struct db_day_rec* pdbDay)
//
//	Input:	pdbDay			day record to write to database
//
//	Output:	none				day record read from database
//
//	Return:	iDAY_IN_DB			expected return: ulDay day for station msStaNum written
//			iDB_BAD				dbVista database error
//	
//  date    /	author	revision
//  -----------------	--------
//	17-Dec-2001	SFK		Created from CreateDayRec in DbDay.cpp
//////////////////////////////////////////////////////////////////
int CBaseVistaData::AddDayData(struct db_day_rec* pdbDay)
{
	if (d_recwrite((char *)pdbDay, CURR_DB) !=S_OKAY) return(iDB_BAD);
	return(iDAY_IN_DB);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDayStatistics
//
//	Description:
//	Get number of days in the specified timerange.
//
//	Declaration:
//	bool CBaseVistaData::GetDayStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrMsg)
//
//	Input:	Requested			Requested time looking for day information
//
//	Output:	pActual				Actual time range of day data found
//			mulNumPtsActual		Number of days found in interval
//
//	Return:	true/false
//			miErrorNum
//				iSTA_NOT_IN_DB		msStaNum unknown in database
//				iDB_BAD				dbVista database error
//				iDAY_NOT_IN_DB		no day in database containing ulDayStart
//	
//  date    /	author	revision
//  -----------------	--------
//	17-Dec-2001	SFK		Created from CreateDayRec in DbDay.cpp
//	18-Apr-2002 SFK		Changed timebase to DATE
//////////////////////////////////////////////////////////////////
bool CBaseVistaData::GetDayStatistics(const DB_D_INTERVAL &Requested, DB_D_INTERVAL *pActual, CString *pErrMsg)
{
	int iStatus;
	DATE dDay, dEndDayPlusOne;
	unsigned long ulDum;
	DB_D_INTERVAL Day;

	if (pErrMsg) pErrMsg->Empty();
	pActual->dStart = 0.0;
	pActual->dEnd = 0.0;
	
	dDay = Requested.dStart;
	dEndDayPlusOne = Requested.dEnd + 1.0;
	int i = 0;

	do {
		iStatus = DayExists(dDay, &ulDum, &Day);
		if (iStatus == iDAY_IN_DB) {
			if (i==0) pActual->dStart = Day.dStart; 
			i++;
			pActual->dEnd = Day.dEnd;
		}
		if ((iStatus == iDAY_IN_DB) || (iStatus == iDAY_NOT_IN_DB)) {
			dDay += 1.0;		// go on to next day
		}
		else {	// unexpected error from DayExists, have to quit
			if (pErrMsg) pErrMsg->Format(" GetDayData: Unexpected error of %d from DayExists.", iStatus);
			miErrorNum = iStatus;
			mulNumPtsActual = i;
			return(false);
		}
	} while (!SameDay(dDay, dEndDayPlusOne));
	mulNumPtsActual = i;	// number of days found in interval
	return(true);

}

CString CBaseVistaData::GetDatabaseErrorText()
{
	CString cs;
	GetDatabaseErrorText(cs);
	return cs;
}

void CBaseVistaData::GetDatabaseErrorText(CString& csDBError)
{
	switch (miErrorNum)
	{
		case iBAD_INTERVAL:			csDBError = CString("Bad database interval.");			break;
		case iDAY_NOT_IN_DB:		csDBError = CString("Day is not in database.");			break;
		case iDB_BAD:				csDBError = CString("Database error.");					break;
		case iDB_EMPTY:				csDBError = CString("Database is empty.");				break;
		case iDB_NOT_OPEN:			csDBError = CString("Database not open.");				break;
		case iNO_DATA_FOR_STATION:	csDBError = CString("No data found for this station.");	break;
		case iSTA_NOT_IN_DB:		csDBError = CString("Station not found in database.");	break;
		case iTIME_NOT_IN_DB:		csDBError = CString("Time not found in database.");		break;
		case iNO_STORAGE:			csDBError = CString("No storage available.");			break;
		case iINTERNAL_ERROR:		csDBError = CString("Internal database error.");			break;
		case iNOT_DB_ISSUE:			csDBError.Empty();										break;
		default:					csDBError = CString("Unrecognized database error code."); break;
	}
}
