// AccessBaseData.cpp: implementation of the CBaseAccessData class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : AccessBaseData.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//         Date : 03/28/2003
//  Description : This file contains the methods for the CBaseAccessData object.
//		The following CBaseAccessData functions are here:
//			DoesTableExist - checks whether table is in db
//			CreateDaysTable - create table for days information that is kept in all db's 
//			AddDayData - add a record to the days table
//			DayExists - determine if any data exists in the db for this station on this day
//			DeleteDay - delete the information about this day in the day table
//			CreateTimeIntervalString - format a time interval string
//			GetDataStoreTimeRange - determine the first and last times in the db
//			GetDayStatistics - get the number of days in the time range for the station
//			GetDayData - read the day data in the time range for the station
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccessBaseData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
// The CBaseAccessData class contains common functionality that happens 
// with accessing data in Access data bases -- such as keeping track of
// the data associated with each day in the database.  Many of the functions
// in this class are pure virtual and must be implemented by the
// class that inherits from this dealing specifically with the unique
// data for that instrument.
// If a data type has some functionality specifically dealing with its
// data that functionality should be included in a class that inherits
// from this class (such as CBinaryData where specific actions
// associated only with binary data are handled).
//////////////////////////////////////////////////////////////////////
CBaseAccessData::CBaseAccessData(
	CDbAccess* pDb, 
	const int iStaNum, 
	const int iInstType, 
	const bool bInterpolated,
	bool bQuietMode)
{
	m_ulNumPtsRequested = 0;
	m_ulNumPtsActual = 0;
	m_pDb = pDb;
	m_bInterpolated = bInterpolated;
	m_iStaNum = iStaNum;
	m_iInstType = iInstType;

	// initialize the names of the DaysTable column
	m_strDaysCols[DAYS_STATION_NUMBER_COL]		= "Station Number";
	m_strDaysCols[DAYS_INSTRUMENT_TYPE_COL]		= "Instrument Type";
	m_strDaysCols[DAYS_DAY_START_TIMESTAMP_COL] = "Day Start Timestamp";
	m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL]= "Data Start Timestamp";
	m_strDaysCols[DAYS_LAST_DATA_TIMESTAMP_COL] = "Data End Timestamp";
	m_strDaysCols[DAYS_NUM_PTS_COL]				= "Number Data Points";
}


CBaseAccessData::~CBaseAccessData()
{

}


///////////////////////////////////////////////////////////////////////////
//	Name:	DoesTableExist
//
//	Description:
//	Checks whether a table with the specified name exists in the database.
//	
//	Declaration:
//	bool CBaseAccessData::DoesTableExist(const CString& strTableName, CString *pstrErrorMsg)
//
//	Input:	strTableName	name of table looking for
//
//	Output:	pstrErrorMsg	string containing error message, if any
//
//	Return:	true (table exists) / false (no table by this name)
//	
//  date    /	author	revision
//  -----------------	--------
//	08-Feb-2001	SFK		Created 
//	13-Sep-2002 SFK		Modified from Rad 3.0 CBaseData
//////////////////////////////////////////////////////////////////
bool CBaseAccessData::DoesTableExist(const CString& strTableName, CString *pstrErrorMsg)
{
	if (pstrErrorMsg) pstrErrorMsg->Empty();
	return(m_pDb->DoesTableExist(strTableName));
}


///////////////////////////////////////////////////////////////////////////
//	Name:	CreateDaysTable
//
//	Description:
//	Create a table that will hold the information about the days for all stations
//	in the database.  This table is always named as DaysDataTable. The table 
//	contains the following fields:
//		StationNumber - the station id from Facility Cfg
//		InstrumentType - which type of instrument this is for the station (ILONs have multiple types)
//		DayStartTimestamp - time of 00:00:00 in the day
//		FirstDataTimestamp - time associated with first data point for StationNumber
//		LastDataTimestamp - time associated with last data point for StationNumber
//		NumPtsInDay - total number of data points for this day
//	
//	Declaration:
//	bool CBaseAccessData::CreateDaysTable(CString *pstrErrorMsg)
//
//	Input:	none
//
//	Output:	pstrErrorMsg	string containing error message, if any
//
//	Return:	true (table created or exists) / false (couldn't make table)
//	
//  date    /	author	revision
//  -----------------	--------
// 22-Oct-2002	SFK		Created
//////////////////////////////////////////////////////////////////
bool CBaseAccessData::CreateDaysTable(CString *pstrErrorMsg)
{
	CString strQuery;
	CString strTemp;

	if (pstrErrorMsg) pstrErrorMsg->Empty();
	if (DoesTableExist(DAYS_TABLE)) return(true);	// exists, don't need to make it
	
	strQuery.Format("CREATE TABLE [%s] (", DAYS_TABLE),
	strTemp.Format("[%s] INTEGER, ",  m_strDaysCols[DAYS_STATION_NUMBER_COL]);
	strQuery += strTemp;
	strTemp.Format("[%s] INTEGER, ",  m_strDaysCols[DAYS_INSTRUMENT_TYPE_COL]);
	strQuery += strTemp;
	strTemp.Format("[%s] DATETIME, ", m_strDaysCols[DAYS_DAY_START_TIMESTAMP_COL]);
	strQuery += strTemp;
	strTemp.Format("[%s] DATETIME, ", m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL]);
	strQuery += strTemp;
	strTemp.Format("[%s] DATETIME, ", m_strDaysCols[DAYS_LAST_DATA_TIMESTAMP_COL]);
	strQuery += strTemp;
	strTemp.Format("[%s] INTEGER ",   m_strDaysCols[DAYS_NUM_PTS_COL]);
	strQuery += strTemp;
	strTemp.Format(");");
	strQuery += strTemp;
	bool bSuccess = m_pDb->ExecuteSqlCommand (strQuery);
	if (!bSuccess) if (pstrErrorMsg) pstrErrorMsg->Format("Database error, database is probably corrupted.");
	return(bSuccess);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	AddDayData
//
//	Description:
//	Add a record with information for the existing day, station and instrument
//	type to the DaysDataTable.  If there is existing information for the day, 
//	station and instrument type, replace it with the new stuff.
//	
//	Declaration:
//	bool CBaseAccessData::AddDayData(const DB_D_INTERVAL& ActualDay, unsigned long ulNumPtsInDay, CString *pstrErrorMsg)
//
//	Input:	ActualDay		Portion of day that data exist for
//			ulNumPtsInDay	Number of data points in the day (uninterpolated??)
//
//			m_iStaNum		station number data is from
//			m_iInstType		which type of instrument at the station
//
//	Output:	pstrErrorMsg	string containing error message, if any
//
//	Return:	true / false
//	
//  date    /	author	revision
//  -----------------	--------
// 22-Oct-2002	SFK		Created
//////////////////////////////////////////////////////////////////
bool CBaseAccessData::AddDayData(const DB_D_INTERVAL& ActualDay, unsigned long ulNumPtsInDay, CString *pstrErrorMsg)
{
	CString strQuery;
	CString strTemp;
	CString strTableName;
	bool bTableExists = true;
	bool bSuccess = false;

	if (pstrErrorMsg) pstrErrorMsg->Empty();
	strTableName = DAYS_TABLE;

	// if the DaysDataTable does not exist, create it now.
	if (!DoesTableExist(strTableName)) {
		bTableExists = CreateDaysTable(pstrErrorMsg);
	}

	// the table is there.  See if there is any old data to delete, then add the new stuff
	if (bTableExists) {
		//	Delete any existing record about this day, station and instrument type in the database
		bSuccess = DeleteDay(ActualDay.dStart);

		// are rid of any old stuff, now add the new day record
		if (bSuccess) {
			DATE dDayStart = (int)ActualDay.dStart;
			// are ready to add this day's record into the Station Days Table
			strQuery.Format("INSERT INTO [%s] ([%s], [%s], [%s], [%s], [%s], [%s])",strTableName,
							m_strDaysCols[DAYS_STATION_NUMBER_COL],
							m_strDaysCols[DAYS_INSTRUMENT_TYPE_COL],
							m_strDaysCols[DAYS_DAY_START_TIMESTAMP_COL],
							m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL],
							m_strDaysCols[DAYS_LAST_DATA_TIMESTAMP_COL],
							m_strDaysCols[DAYS_NUM_PTS_COL]);
			strTemp.Format( "VALUES (%d, %d, %lf, %lf, %lf, %d ",
							m_iStaNum, m_iInstType,
							dDayStart, ActualDay.dStart, ActualDay.dEnd,
							ulNumPtsInDay);
			strQuery += strTemp;	
			strTemp.Format( ");");			// add closing parenthesis
			strQuery += strTemp;	
			bSuccess = m_pDb->ExecuteSqlCommand(strQuery);
		}

	}
	if (!bSuccess) if (pstrErrorMsg) pstrErrorMsg->Format("Database error, database is probably corrupted.");
	return(bSuccess);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	DayExists
//
//	Description:
//	Determine if there is an entry for m_iStaNum of m_iInstType in the DaysTable
//	for the given day and if yes, return some information about the day (number
//	of data points, time of first and last data points for the station for that day).
//
//	Declaration:
//	bool CBaseAccessData::DayExists(DATE dTime, unsigned long *pulNumPts, DB_D_INTERVAL *pdActual)
//
//	Input:	dTime				some time in the day we are interested in
//			m_iStaNum			which station
//			m_iInstType			which type of instrument at that station
//			
//	Output:	pulNumPts			number of points in the database for the specified day, station and instrument type
//			pdActual			timestamps of the first and last data points in the specified day for station and instrument type
//
//	Return:	true (day for station and inst type exists) / false (no entry for given day, station and inst type)
//	
//  date    /	author	revision
//  -----------------	--------
// 22-Oct-2002	SFK		Created
//////////////////////////////////////////////////////////////////
bool CBaseAccessData::DayExists(DATE dTime, unsigned long *pulNumPts, DB_D_INTERVAL *pdActual)
{
	CString strQuery;
	CString strTemp;
	CString strTableName;
	bool bTableExists = true;
	bool bSuccess = false;
	bool bEmptyIsSuccess = true;
	bool bWasOpen;
	ADODB::_RecordsetPtr pRecordSet;
	unsigned int uiNum;

	//if (pstrErrorMsg) pstrErrorMsg->Empty();
	*pulNumPts = 0;
	pdActual->dStart = 0;
	pdActual->dEnd = 0;
	strTableName = DAYS_TABLE;

	// check if the table of day information even exists
	if (!DoesTableExist(strTableName)) return(bSuccess);

	//Figure out the start time of the day requested
	DATE dStart = (int)dTime;

	// Get information about any days matching the requested one
	strQuery.Format("SELECT * FROM [%s] WHERE [%s] = %lf;",
					strTableName,
					m_strDaysCols[DAYS_DAY_START_TIMESTAMP_COL], dStart);
	bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);
	if (bSuccess) {
		if (uiNum > 0)  {	// search through the records for the day and see if there is one for the requested station and type
			int iStaNum, iInstType, iNumPts;
			for (unsigned int i=0; i<uiNum; i++) {
				// read the three fields of interest from the record
				bSuccess = m_pDb->GetRecordFieldInt(pRecordSet,	m_strDaysCols[DAYS_STATION_NUMBER_COL], &iStaNum);
				if (bSuccess) bSuccess = m_pDb->GetRecordFieldInt(pRecordSet, m_strDaysCols[DAYS_INSTRUMENT_TYPE_COL], &iInstType);
				if (bSuccess) bSuccess = m_pDb->GetRecordFieldInt(pRecordSet, m_strDaysCols[DAYS_NUM_PTS_COL], &iNumPts);
				// if this is what (station number and instrument type) we are looking for we are done
				if (bSuccess && (iStaNum == m_iStaNum) && (iInstType == m_iInstType)) {
					*pulNumPts = iNumPts;
					if (bSuccess) bSuccess = m_pDb->GetRecordFieldDouble(pRecordSet, m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], &pdActual->dStart);
					if (bSuccess) bSuccess = m_pDb->GetRecordFieldDouble(pRecordSet, m_strDaysCols[DAYS_LAST_DATA_TIMESTAMP_COL], &pdActual->dEnd);
					break;
				}
				pRecordSet->MoveNext();
			}
		}
		m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen); 	
	}

	//if (!bSuccess) if (pstrErrorMsg) pstrErrorMsg->Format("Database error, database is probably corrupted.");
	return(bSuccess);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	DeleteDay
//
//	Description:
//	Delete the entry for m_iStaNum of m_iInstType in the DaysTable
//	for the given day.
//
//	Declaration:
//	bool CBaseAccessData::DeleteDay(DATE dTime)
//
//	Input:	dTime				start time of the day to be deleted
//			m_iStaNum			which station
//			m_iInstType			which type of instrument at that station
//			
//	Output:	pulNumPts			number of points in the database for the specified day, station and instrument type
//			pdActual			timestamps of the first and last data points in the specified day for station and instrument type
//
//	Return:	true (day deleted) / false (unexpected error)
//	
//  date    /	author	revision
//  -----------------	--------
//	??-???-????	SFK		Created
//////////////////////////////////////////////////////////////////
bool CBaseAccessData::DeleteDay(DATE dTime)
{
	bool bSuccess = false;
	CString strQuery;
	
	CString strTableName;
	strTableName = DAYS_TABLE;

	// check if the table of day information even exists
	if (!DoesTableExist(strTableName)) return(bSuccess);

	DATE dStart = (int)dTime;
	// for debug do a select
	bool bEmptyIsSuccess = true;
	bool bWasOpen;
	ADODB::_RecordsetPtr pRecordSet;
	unsigned int uiNum = 0;
	strQuery.Format("SELECT * FROM [%s] WHERE [%s] = %d AND [%s] = %d AND [%s] = %lf;",
					strTableName,
					m_strDaysCols[DAYS_STATION_NUMBER_COL], m_iStaNum,
					m_strDaysCols[DAYS_INSTRUMENT_TYPE_COL], m_iInstType,
					m_strDaysCols[DAYS_DAY_START_TIMESTAMP_COL], dStart);
	bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);
	m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen); 	

	strQuery.Format("DELETE * FROM [%s] WHERE [%s] = %d AND [%s] = %d AND [%s] = %lf;",
					strTableName,
					m_strDaysCols[DAYS_STATION_NUMBER_COL], m_iStaNum,
					m_strDaysCols[DAYS_INSTRUMENT_TYPE_COL], m_iInstType,
					m_strDaysCols[DAYS_DAY_START_TIMESTAMP_COL], dStart);
	bSuccess = m_pDb->ExecuteSqlCommand (strQuery);

	return(bSuccess);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	CreateTimeIntervalString
//
//	Description:
//	Given a time, format a string representing that interval in a standard
//	manner for use such as in error strings, etc.
//
//	Declaration:
//	void CBaseAccessData::CreateTimeIntervalString(const DB_D_INTERVAL& Requested, CString *pStr)
//
//	Input:	Requested	time interval
//			
//	Output:	pStr		formatted string representing interval
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	??-???-????	SFK		Created
//////////////////////////////////////////////////////////////////
void CBaseAccessData::CreateTimeIntervalString(const DB_D_INTERVAL& Requested, CString *pStr)
{
	CString strTemp, strTemp1;
	COleDateTime oleStartTime, oleEndTime;

	oleStartTime.m_dt = Requested.dStart;
	oleEndTime.m_dt = Requested.dEnd;
	strTemp = oleStartTime.Format("%c"), 
	strTemp1 = oleEndTime.Format("%c");
	pStr->Format("%s to %s", strTemp, strTemp1);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStoreTimeRange
//
//	Description:
//	For any station in the databast, determine the first and last times
//	in the database from the DaysDataTable.
//
//	Declaration:
//	bool CBaseAccessData::GetDataStoreTimeRange(bool bErrDisplay, DATE *pdFirstTimestamp, DATE *pdLastTimestamp)
//
//	Input:	bErrDisplay			whether to display a message if the database is empty
//			
//	Output:	pdFirstTimestamp	earliest time in the database
//			pdLastTimestamp		last time in the database
//
//	Return:	true (DaysDataTable exists and has an entry) / false (DaysDataTable non-existant or empty)
//	
//  date    /	author	revision
//  -----------------	--------
//	02-Dec-2002	SFK		Created
//  14-Mar-2006	SFK		Fix so that the station number is taken into account when asking for the date range.
//						Previously, just the first and last date in the DaysDataTable was returne.
//////////////////////////////////////////////////////////////////
bool CBaseAccessData::GetDataStoreTimeRange(bool bErrDisplay, DATE *pdFirstTimestamp, DATE *pdLastTimestamp)
{
	CString strTableName, strQuery, strTemp;
	bool bSuccess = false;

	*pdFirstTimestamp = 0.0;
	*pdLastTimestamp = 0.0;

	// check if the table of day information even exists
	strTableName = DAYS_TABLE;
	if (!DoesTableExist(strTableName)) {
		if (bErrDisplay) {
			strTemp.Format("The database at %s is empty.", m_pDb->mstrDatabasePath);
			/* NOT FOR NDAR
			MessageBox(NULL, strTemp, "Database Error", MB_OK|MB_ICONWARNING);
			*/
		}
		return(bSuccess);
	}

	bool bEmptyIsSuccess = true;
	bool bWasOpen;
	ADODB::_RecordsetPtr pRecordSet;
	unsigned int uiNumPts;
	// DR00268
	// 14-Mar-2006 SFK Add check to get the time range depending on the station
	if (m_iStaNum == ANY_STATION) {
		strQuery.Format("SELECT MIN([%s]) AS [%s] FROM [%s];", 
			m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], strTableName);
	}
	else {
		strQuery.Format("SELECT [%s] FROM [%s] WHERE [%s] = %d ORDER BY [%s];",
							m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], strTableName, 
							m_strDaysCols[DAYS_STATION_NUMBER_COL], m_iStaNum,
							m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL]);
	}
	bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNumPts, bEmptyIsSuccess);
	if (bSuccess) {
		if (uiNumPts > 0) {		// read the first time in the list
			bSuccess = m_pDb->GetRecordFieldDouble(pRecordSet, m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], pdFirstTimestamp);
		}
		m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen); 	
	}

	if (bSuccess && (uiNumPts > 0)) {
		// calculate the new ending time from the table
		if (m_iStaNum == ANY_STATION) {
			strQuery.Format("SELECT MAX([%s]) AS [%s] FROM [%s];", 
				m_strDaysCols[DAYS_LAST_DATA_TIMESTAMP_COL], m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], strTableName);
		}
		else {
			strQuery.Format("SELECT [%s] FROM [%s] WHERE [%s] = %d ORDER BY [%s] DESC;",
							m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], strTableName, 
							m_strDaysCols[DAYS_STATION_NUMBER_COL], m_iStaNum,
							m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL]);
		}
		bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNumPts, bEmptyIsSuccess);
		if (bSuccess) {
			if (uiNumPts > 0) {	// the last time is the first time in the list
				bSuccess = m_pDb->GetRecordFieldDouble(pRecordSet, m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], pdLastTimestamp);
			}
			m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen); 	
		}
	}

	// if the DaysDataTable has no entry, then post a message if requested
	if ((uiNumPts <= 0) && bErrDisplay) {
		strTemp.Format("The database at %s is empty.", m_pDb->mstrDatabasePath);
		/* NOT FOR NDAR
		MessageBox(NULL, strTemp, "Database Error", MB_OK|MB_ICONWARNING);
		*/
	}
	
	return(bSuccess);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDayStatistics
//
//	Description:
//	Get the number of days in the specified time range for the station in the DaysDataTable.
//
//	Declaration:
//	bool CBaseAccessData::GetDayStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrMsg)
//
//	Input:	dRequested			Requested time range that we want day information about	
//			
//	Output:	dActual				Start/end timestamp of actual data within dRequested
//			m_ulNumPtsActual	Number of days found in dRequested
//
//	Return:	true (DaysDataTable exists and has data) / false (DaysDataTable non-existant or empty)
//	
//  date    /	author	revision
//  -----------------	--------
//	02-Dec-2002	SFK		Created
//	27-Feb-2003	SFK		Fixed so returns the number of days -- rather than sum of points in days
//////////////////////////////////////////////////////////////////
bool CBaseAccessData::GetDayStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrMsg)
{
	CString strTableName, strQuery;
	bool bSuccess = false;

	pdActual->dStart = 999999999.0;
	pdActual->dEnd = 0.0;
	m_ulNumPtsActual = 0;

	// check if the table of day information even exists
	strTableName = DAYS_TABLE;
	if (!DoesTableExist(strTableName)) {
		if (pstrErrMsg) pstrErrMsg->Format("GetDayStatistics: No table with day data.");
		return(bSuccess);
	}

	bool bEmptyIsSuccess = true;
	bool bWasOpen;
	ADODB::_RecordsetPtr pRecordSet;
	unsigned int uiNumPts;
	strQuery.Format("SELECT * FROM [%s] WHERE ([%s] BETWEEN %16.10lf AND %16.10lf) AND ([%s] = %ld) AND ([%s] = %ld);",
			strTableName, 
			m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], dRequested.dStart - ONE_THOU_SEC, dRequested.dEnd + ONE_THOU_SEC,
			m_strDaysCols[DAYS_STATION_NUMBER_COL], m_iStaNum,
			m_strDaysCols[DAYS_INSTRUMENT_TYPE_COL], m_iInstType);
	bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNumPts, bEmptyIsSuccess);
	if (bSuccess) {
		if (uiNumPts > 0) {
			m_ulNumPtsActual = uiNumPts;			// number of days in the interval
			double dFirstData, dLastData;
			for (unsigned int ui=0; ui<uiNumPts; ui++) {
				if (bSuccess) m_pDb->GetRecordFieldDouble(pRecordSet, m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], &dFirstData);
				if (bSuccess) m_pDb->GetRecordFieldDouble(pRecordSet, m_strDaysCols[DAYS_LAST_DATA_TIMESTAMP_COL], &dLastData);
				if (bSuccess) {
					if (dFirstData < pdActual->dStart) pdActual->dStart = dFirstData;
					if (dLastData > pdActual->dEnd) pdActual->dEnd = dLastData;
				}
				pRecordSet->MoveNext();
			}
		}
		m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen); 	
	}

	if (!bSuccess && pstrErrMsg) pstrErrMsg->Format("GetDayStatistics: Unexpected error.");
	return(bSuccess);

}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetDayData
//
//	Description:
//	Read the day data for the specified time range for the station in the DaysDataTable.
//
//	Declaration:
//	bool CBaseAccessData::GetDayData(DB_D_INTERVAL dRequested, DB_D_INTERVAL *pdActual, DayDataStruct *pDayData, CString *pErrMsg)
//
//	Input:	dRequested		Requested time range that we want day data for
//			m_strDaysCols	Database column labels
//			m_iStaNum		which station number
//			m_iInstType		which type of instrument
//			m_pDb			pointer to our database
//			
//	Output:	dActual			Start/end timestamp of actual data within dRequested
//			pDayData		Day data from the database put in this array of structs
//			pErrMsg			error, if any
//			m_ulNumPtsActual number of records in pDayData
//
//	Return:	true (data read successfully) / false (error trying to get data or no data)
//	
//  date    /	author	revision
//  -----------------	--------
//	??-???-????	SFK		Created
//////////////////////////////////////////////////////////////////
bool CBaseAccessData::GetDayData(DB_D_INTERVAL dRequested, DB_D_INTERVAL *pdActual, DayDataStruct *pDayData, CString *pErrMsg)
{
	CString strTableName, strQuery;
	bool bSuccess = false;

	pdActual->dStart = 999999999.0;
	pdActual->dEnd = 0.0;
	m_ulNumPtsActual = 0;

	// check if the table of day information even exists
	strTableName = DAYS_TABLE;
	if (!DoesTableExist(strTableName)) {
		if (pErrMsg) pErrMsg->Format("GetDayData: No table with day data.");
		return(bSuccess);
	}

	bool bEmptyIsSuccess = true;
	bool bWasOpen;
	ADODB::_RecordsetPtr pRecordSet;
	unsigned int uiNumPts;
	strQuery.Format("SELECT * FROM [%s] WHERE ([%s] BETWEEN %16.10lf AND %16.10lf) AND ([%s] = %ld) AND ([%s] = %ld);",
			strTableName, 
			m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], dRequested.dStart - ONE_THOU_SEC, dRequested.dEnd + ONE_THOU_SEC,
			m_strDaysCols[DAYS_STATION_NUMBER_COL], m_iStaNum,
			m_strDaysCols[DAYS_INSTRUMENT_TYPE_COL], m_iInstType);
	bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNumPts, bEmptyIsSuccess);
	if (bSuccess) {
		if (uiNumPts > 0) {
			int iTemp;
			m_ulNumPtsActual = uiNumPts;
			for (unsigned int ui=0; ui<uiNumPts; ui++) {
				bSuccess = m_pDb->GetRecordFieldInt(pRecordSet, m_strDaysCols[DAYS_NUM_PTS_COL], &iTemp);
				pDayData[ui].dNumPtsInDay = (double)iTemp;
				if (bSuccess) m_pDb->GetRecordFieldDouble(pRecordSet, m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL], &(pDayData[ui].dFirstDataPt));
				if (bSuccess) m_pDb->GetRecordFieldDouble(pRecordSet, m_strDaysCols[DAYS_LAST_DATA_TIMESTAMP_COL], &(pDayData[ui].dLastDataPt));
				if (bSuccess) {
					if (pDayData[ui].dFirstDataPt < pdActual->dStart) pdActual->dStart = pDayData[ui].dFirstDataPt;
					if (pDayData[ui].dLastDataPt > pdActual->dEnd) pdActual->dEnd = pDayData[ui].dLastDataPt;
				}
				pRecordSet->MoveNext();
			}
		}
		m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen); 	
	}

	if (!bSuccess && pErrMsg) pErrMsg->Format("GetDayData: Unexpected db error.");
	return(bSuccess);
}
