/*
This work was supported by the United States Member State Support Program to IAEA Safeguards;
the U.S. Department of Energy, Office of Nonproliferation and National Security, International
Safeguards Division; and the U.S. Department of Energy, Office of Safeguards and Security.

LA-CC-14-040. This software was exported from the United States in accordance with the Export
Administration Regulations. Diversion contrary to U.S. law prohibited.

Copyright 2015, Los Alamos National Security, LLC. This software application and associated
material ("The Software") was prepared by the Los Alamos National Security, LLC. (LANS), under
Contract DE-AC52-06NA25396 with the U.S. Department of Energy (DOE). All rights in the software
application and associated material are reserved by DOE on behalf of the Government and LANS
pursuant to the contract.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or other materials provided
with the distribution.
3. Neither the name of the "Los Alamos National Security, LLC." nor the names of its contributors
may be used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS
NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRAT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
// EOSSData.cpp: implementation of the CEOSSData class.

#include "stdafx.h"
#include "EOSSData.h"
#include "SafeArrayUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define EOSS_BINARY_DATA_TABLE "EOSS Binary Data"
#define STATION_DAYS_TABLE "EOSS Days Table"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
// The CEOSSData class contains unique functionality associated
// with accessing (storing and retrieving) data from the binary database.
// It implements the pure virtual functions in the CBaseAccessData class.
//////////////////////////////////////////////////////////////////////
CEOSSData::CEOSSData(
	CDbAccess* pDb, 
	const int iStaNum,  
	const bool bQuietMode) : m_plogpoints(NULL)
{

	m_ulNumPtsRequested = 0;
	m_ulNumPtsActual = 0;
	m_pDb = pDb;
	m_iStaNum = iStaNum;

	//CreateArray to hold points.
	m_plogpoints = new CEOSSDataPt [1000];
	m_iNumPoints = 0;
	
}


CEOSSData::~CEOSSData()
{
	if (m_plogpoints)
	{
		//m_plogpoints->RemoveAll();
		delete [] m_plogpoints;
		m_plogpoints = NULL;
	}
	m_iNumPoints = 0;
}


bool CEOSSData::AddEOSSData(CString *pstrErrorMsg)
{
	bool bSuccess = false;

	// Make sure our database is cool
	if (!m_pDb->mbDatabaseOpen)
	{
		m_pDb->Open();
	}

	CString addStatement,findStatement;
	int iPtsAdded = false;
	COleDateTime coleTemp;
	short sChan = 0;
	long lStat = 0;
	bool bState = 0;
	lStat = m_iStaNum;
	bool bAdded=false;
	
	// Add the points stored in the array to the db.
	for (int i = 0; i < m_iNumPoints; i ++)
	{
		coleTemp = m_plogpoints[i].m_dtEventTime;
		sChan = m_plogpoints[i].m_sChannel;

		//Since there could be duplicates, check for this record.  Dups not allowed, so insert will fail
		//findStatement.Format ("SELECT EOSSBinaryData.Date_Time, EOSSBinaryData.Seal_ID FROM EOSSBinaryData WHERE EOSSBinaryData.Date_Time = %f AND EOSSBinaryData.Seal_ID = %i;", coleTemp.m_dt,sChan);
		findStatement.Format ("SELECT EOSSBinaryData.Date_Timestamp, EOSSBinaryData.Seal_ID FROM EOSSBinaryData WHERE EOSSBinaryData.Date_Timestamp = %16.15f AND EOSSBinaryData.Seal_ID = %i;", coleTemp.m_dt,sChan);
		unsigned int uiNumRecs = 0;
		bool bOpen = true;
		
		ADODB::_RecordsetPtr pRecordSet;

		bool bRan = m_pDb->CreateRecordSet (findStatement,(ADODB::_RecordsetPtr *)&pRecordSet,&bOpen,&uiNumRecs);

		bool bExists = uiNumRecs>0;

		if (pRecordSet)
			m_pDb->DestroyRecordSet ((ADODB::_RecordsetPtr )pRecordSet,true);

		if (!bExists)
		{
			// This was not in the DB, add it.
			addStatement.Format ("INSERT INTO EOSSBinaryData (Date_Time,Date_Timestamp,Reader_ID,Seal_ID,Binary_State) VALUES (%f,%16.15f,%i,%i,%i);",(m_plogpoints[i].m_dtEventTime.m_dt),(m_plogpoints[i].m_dtEventTime.m_dt),(m_iStaNum),(m_plogpoints[i].m_sChannel),(m_plogpoints[i].m_bState));
			long lNum =0;
			bAdded = m_pDb->ExecuteSqlCommand (addStatement,lNum);
		}
		else
		{
			//It was already there, add to points
			iPtsAdded += 1;
		}

		iPtsAdded += bAdded?1:0;
		bAdded = false;
	}

	bSuccess = (iPtsAdded== m_iNumPoints);
	m_iNumPoints = 0;

	if (!bSuccess) if (pstrErrorMsg) pstrErrorMsg->Format("Database error, database is probably corrupted.");
	return(bSuccess);
}


bool CEOSSData::AddSealLogPoint(
								long lStationID, 
								short sChannel, 
								COleDateTime eventTime, 
								bool bState)
{
	bool bReturn = false;

	//Store the data point in the EOSS data object for database stuff later.
	//CEOSSDataPt* point = new CEOSSDataPt (lStationID, sChannel,eventTime,bState);
	m_plogpoints [m_iNumPoints].Set (lStationID, sChannel, eventTime, bState);
	m_iNumPoints++;
	return true;
}

bool CEOSSData::GetEOSSSealData(long lStationID, short sSealID, VARIANT *saData,DATE& Start, DATE& End)
{
	CString search;
	bool bEmptyIsSuccess = true;
	bool bWasOpen = false;
	unsigned int uiNum = 0;
	saData->vt = VT_EMPTY;
	
	search.Format ("SELECT Date_Timestamp,Seal_ID,Binary_State FROM EossBinaryData WHERE Reader_ID = %u AND Seal_ID = %u ORDER BY Date_Timestamp;", lStationID, sSealID);
	ADODB::_RecordsetPtr pDataRecordSet;
	bool bSuccess = m_pDb->CreateRecordSet(search, (ADODB::_RecordsetPtr *)&pDataRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);
	
	COleDateTime bogusTime;
	bogusTime.m_dt = 0;

	// First, return an empty VARIANT if there are no records at all
	if (uiNum == 0)
	{
		saData->vt = VT_EMPTY;
		bSuccess = true;
	}

	if (bSuccess && uiNum >0)
	{
		long lNumActualPoints = 0;

		DATE* pDateStamps = new DATE[uiNum+1];
		int* pnStates = new int [uiNum+1];

		pDataRecordSet->MoveFirst ();
		// temporary vars for processing
		COleDateTime date;
		int nValue;
			
		//vars to store prior and proceding points (0th and nth)
		COleDateTime datePriorToInterval, dateAfterInterval;
		datePriorToInterval.m_dt = 0.0;
		dateAfterInterval.m_dt = 0.0;
		int nValuePriorToInterval = -1;
		int nValueAfterInterval = -1;
		
		// Get the first record
		m_pDb->GetRecordFieldDouble ((ADODB::_RecordsetPtr) (pDataRecordSet),"Date_Timestamp",&date.m_dt);
		m_pDb->GetRecordFieldInt ((ADODB::_RecordsetPtr) (pDataRecordSet),"Binary_State",&nValue);
		//pDataRecordSet->MoveNext();

		int index = 0;
		
		SAFEARRAYBOUND saBound[2];
		
		//There is the case that this is the first point ever and within (or beyond)
		//the interval requested
		//if (date.m_dt > Start)
		if (date.m_dt >= Start)  //pjm
		{
			// previous point to be stored as a bogus point
			// Just use the init values of the first point.
		}
		else
		{
			// First point is prior to the Start
			while (date.m_dt < Start && index < uiNum)
			{
				//We were moving next before storing it.  If the first one is the one before, this would fail.
				if (date.m_dt < Start)
				{
					datePriorToInterval = date;
					nValuePriorToInterval = nValue;
				}

				pDataRecordSet->MoveNext();
				m_pDb->GetRecordFieldDouble ((ADODB::_RecordsetPtr) (pDataRecordSet),"Date_Timestamp",&date.m_dt);
				m_pDb->GetRecordFieldInt ((ADODB::_RecordsetPtr) (pDataRecordSet),"Binary_State",&nValue);
				
				index ++;
			}
			// Upon exit, we have the first point potentially in the interval stored
			// in date and bValue.
		}

		if (index >= uiNum && date.m_dt < Start)
		{
			// We got all the way through and nothing is in the interval
			// All of it is before the requested interval
			// Make a safe array of size two with the prior point and indication
			// that no point after that one exists
			saBound[0].cElements = 2;
			saBound[0].lLbound = 0;
			saBound[1].cElements=2;
			saBound[1].lLbound = 0;
			saData->vt = VT_VARIANT | VT_ARRAY;
			saData->parray = SafeArrayCreate (VT_VARIANT,2,saBound);

			//Add point before
			SA_PutElem (saData->parray,0L,0L,(COleDateTime)datePriorToInterval);
			SA_PutElem (saData->parray,0L,1L,(int)nValuePriorToInterval);
			
			//Add point after
			SA_PutElem (saData->parray,1L,0L,(COleDateTime) dateAfterInterval);
			SA_PutElem (saData->parray,1L,1L,(int) nValueAfterInterval);
			bSuccess = true;
		}
		//else if (index < uiNum && date.m_dt > Start && date.m_dt <= End)
		else if (index < uiNum && date.m_dt >= Start && date.m_dt <= End)  //pjm
		{
			//HN Don't add the fake point, it gets added below.
//			if (lNumActualPoints == 0)
//			{
				//pjm
				//Special case where the first point of the requested interval is
				//also the first point in the entire database for this channel
//				pDateStamps[lNumActualPoints] = 0.0;
//				pnStates[lNumActualPoints] = -1;
//			}
//			else
//			{
				// We found something in the interval, store them if we haven't reached
				// end of the interval, then save the one after (if there is one) in the
				// nth spot.
				pDateStamps[lNumActualPoints] = date.m_dt;
				pnStates[lNumActualPoints] = nValue;
//			}
			lNumActualPoints ++;
			
			index ++;
			
			while (index < uiNum && date.m_dt <= End)
			{
				// While there is data on the interval
				pDataRecordSet->MoveNext ();
				m_pDb->GetRecordFieldDouble ((ADODB::_RecordsetPtr) (pDataRecordSet),"Date_Timestamp",&date.m_dt);
				m_pDb->GetRecordFieldInt ((ADODB::_RecordsetPtr) (pDataRecordSet),"Binary_State",&nValue);
			
				if (date.m_dt <= End)
				{
					pDateStamps[lNumActualPoints] = date.m_dt;
					pnStates[lNumActualPoints] = nValue;
					lNumActualPoints ++;
				}
				else
				{
					dateAfterInterval = date.m_dt;
					nValueAfterInterval = nValue;
				}
				index ++;
			}

			saData->vt = VT_VARIANT | VT_ARRAY;
			saBound[0].cElements = lNumActualPoints + 2;
			saBound[0].lLbound = 0;
			saBound[1].cElements=2;
			saBound[1].lLbound = 0;
			saData->parray = SafeArrayCreate (VT_VARIANT,2,saBound);
			//Add point before
			SA_PutElem (saData->parray,0L,0L,(COleDateTime)datePriorToInterval);
			SA_PutElem (saData->parray,0L,1L,(int)nValuePriorToInterval);
			
			for (int j = 0; j < lNumActualPoints; j++)
			{
				SA_PutElem (saData->parray,j+1,0L,(COleDateTime) COleDateTime(pDateStamps[j]));
				SA_PutElem (saData->parray,j+1,1L,(int) pnStates[j]);
			}

			// Now, we either went through all the data, or we have a next point.
			// Do nothing, our inti value ok
			SA_PutElem (saData->parray,lNumActualPoints +1,0L,(COleDateTime) dateAfterInterval);
			SA_PutElem (saData->parray,lNumActualPoints +1,1L,(int) nValueAfterInterval);
		}
		else if (index <= uiNum && date.m_dt > End)
		{
			//We found something, but it is beyond our interval
			saBound[0].cElements = 2;
			saBound[0].lLbound = 0;
			saBound[1].cElements=2;
			saBound[1].lLbound = 0;
			saData->vt = VT_VARIANT | VT_ARRAY;
			saData->parray = SafeArrayCreate (VT_VARIANT,2,saBound);

			dateAfterInterval = date;
			nValueAfterInterval = nValue;

			SA_PutElem (saData->parray,0L,0L,(COleDateTime)datePriorToInterval);
			SA_PutElem (saData->parray,0L,1L,(int)nValuePriorToInterval);

			// We grabbed the one beyond it, store it as the next one.
			SA_PutElem (saData->parray,1,0L,(COleDateTime) dateAfterInterval);
			SA_PutElem (saData->parray,1,1L,(int) nValueAfterInterval);

			bSuccess = true;
		}

	
		if (pDateStamps)
		{
			delete [] pDateStamps;
			pDateStamps = NULL;
		}
		if (pnStates)
		{
			delete [] pnStates;
			pnStates = NULL;
		}
		if (pDataRecordSet)
			m_pDb->DestroyRecordSet ((ADODB::_RecordsetPtr) pDataRecordSet,true);

	}// End we have data from the seal

	return bSuccess;
}

bool CEOSSData::GetEOSSDayStatistics(
									 long lStationID, 
									 DATE dStart,
									 DATE dEnd, 
									 DATE* actualStart, 
									 DATE* actualEnd, 
									 long* plNumDays)
{
	CString search;
	COleDateTime wholeEndDay (dEnd);
	wholeEndDay.SetDateTime (wholeEndDay.GetYear (),wholeEndDay.GetMonth(),wholeEndDay.GetDay (),11,59,59);
	if (m_iStaNum != -1)
		search.Format ("SELECT Date_Timestamp FROM EossBinaryData WHERE Reader_ID = %u AND Date_Timestamp BETWEEN %u AND %16.20f ORDER BY Date_Timestamp;",lStationID,(int)dStart,wholeEndDay.m_dt);
	else
		search.Format ("SELECT Date_Timestamp FROM EossBinaryData WHERE Date_Timestamp BETWEEN %u AND %16.20f ORDER BY Date_Timestamp;",(int)dStart,wholeEndDay.m_dt);
	bool bWasOpen = false;
	unsigned int uiNum= 0;
	bool bSuccess = false;
	bool bEmptyIsSuccess = true;
	ADODB::_RecordsetPtr pDataRecordSet;
	bSuccess = m_pDb->CreateRecordSet(search, (ADODB::_RecordsetPtr *)&pDataRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);
	*plNumDays = 0;
	if (bSuccess && uiNum > 0)
	{
		pDataRecordSet->MoveFirst ();
		COleDateTime date,dayAdded;
		for (int i = 0; i < uiNum; i ++)
		{
			date.m_dt = 0.0;
			m_pDb->GetRecordFieldDouble ((ADODB::_RecordsetPtr)pDataRecordSet,"Date_Timestamp",&date.m_dt);
			if (i == 0)
			{
				// This is the start time
				*actualStart = date.m_dt;
				*plNumDays = *plNumDays +1;
				dayAdded.SetDateTime (date.GetYear(),date.GetMonth(),date.GetDay(),0,0,0);
			}
			if (i == uiNum - 1)
			{
				// This is the start time
				*actualEnd = date.m_dt;
			}

			date.SetDateTime(date.GetYear(),date.GetMonth (),date.GetDay(),0,0,0);

			if (date != dayAdded)
			{
				*plNumDays = *plNumDays + 1;
				dayAdded = date;
			}
			pDataRecordSet->MoveNext ();
		}
		bSuccess = true;
	}
	if (pDataRecordSet)
		m_pDb->DestroyRecordSet ((ADODB::_RecordsetPtr)pDataRecordSet,bWasOpen);
	return bSuccess;
}

bool CEOSSData::EOSSDeleteData(const DATE &dtStart, const DATE &dtEnd)
{
	bool bSuccess;
	CString deleteString;
	//deleteString.Format ("DELETE FROM EossBinaryData WHERE Reader_ID = miStationNumber AND Date_Timestamp >= %16.20f AND DateTime <= %16.20f;",dtStart, dtEnd);
	//Changed 4-1-08 the string was setting Date_Time instead of the double value of DateTimestamp
	//Changed 4-2-08  DeleteData should delete the entire days that are specified.
	COleDateTime dtStartOfDay (dtStart);
	dtStartOfDay.SetDateTime (dtStartOfDay.GetYear(),dtStartOfDay.GetMonth(),dtStartOfDay.GetDay(),0,0,0);
	COleDateTime dtEndOfDay(dtEnd);
	dtEndOfDay.SetDateTime (dtEndOfDay.GetYear(),dtEndOfDay.GetMonth(),dtEndOfDay.GetDay(),23,59,59);
	if (m_iStaNum != -1)
		deleteString.Format ("DELETE * FROM EossBinaryData WHERE Reader_ID = %u AND Date_Timestamp BETWEEN %16.0f AND %16.20f;",m_iStaNum,dtStartOfDay.m_dt, dtEndOfDay.m_dt);
	else
		deleteString.Format ("DELETE * FROM EossBinaryData WHERE Date_Timestamp BETWEEN %16.0f AND %16.20f;",dtStartOfDay.m_dt, dtEndOfDay.m_dt);
	long lNumRecs = 0;
	
	return (m_pDb->ExecuteSqlCommand (deleteString,lNumRecs));
}

bool CEOSSData::GetEOSSDataStatistics(
								  short sChannelID, 
								  DATE StartTime, 
								  DATE EndTime, 
								  DATE *pActualStartTime, 
								  DATE *pActualEndTime, 
								  long *plNumPoints)
{

	CString search;
	bool bSuccess = false;
	
	if (m_iStaNum == -1)
	//All stations
	{
		//lookup all seal data
		//search.Format ("SELECT Date_Timestamp FROM EossBinaryData WHERE Date_Timestamp >= %16.101f AND Date_Timestamp <= %16.101f ORDER BY Date_Timestamp;",StartTime,EndTime);
		// 4-1-08 HN Changed this search string.  Changed all formatting of doubles to .20f instead of .101f, which could result in string that is too long.
		search.Format ("SELECT Date_Timestamp FROM EossBinaryData WHERE Date_Timestamp >= %16.20f AND Date_Timestamp <= %16.20f ORDER BY Date_Timestamp;",StartTime,EndTime);
	}
	else if (sChannelID == -1)
	{
		//lookup all seal data
		search.Format ("SELECT Date_Timestamp FROM EossBinaryData WHERE Reader_ID = %u AND Date_Timestamp >= %16.20f AND Date_Timestamp <= %16.20f ORDER BY Date_Timestamp;",m_iStaNum,StartTime,EndTime);
	}
	else
	{
		search.Format ("SELECT Date_Timestamp FROM EossBinaryData WHERE Reader_ID = %u AND Seal_ID = %u AND Date_Timestamp >= %16.20f AND Date_Timestamp <= %16.20f ORDER BY Date_Timestamp;",m_iStaNum,sChannelID, StartTime,EndTime);
	}

	bool bWasOpen = false;
	unsigned int uiNum= 0;
	bool bEmptyIsSuccess = true;
	ADODB::_RecordsetPtr pDataRecordSet;
	bSuccess = m_pDb->CreateRecordSet(search, (ADODB::_RecordsetPtr *)&pDataRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);

	if (bSuccess)
	{
		*plNumPoints = uiNum;
		if (uiNum >0)
		{
			pDataRecordSet->MoveFirst();
			COleDateTime date;
			bSuccess = m_pDb->GetRecordFieldDouble ((ADODB::_RecordsetPtr) pDataRecordSet,"Date_Timestamp",&date.m_dt);
			if (bSuccess)
				*pActualStartTime = date.m_dt;
			int index = 0;
			while (index < uiNum-1)
			{
				pDataRecordSet->MoveNext();
				index ++;
			}
			bSuccess = m_pDb->GetRecordFieldDouble ((ADODB::_RecordsetPtr) pDataRecordSet,"Date_Timestamp",&date.m_dt);
			if (bSuccess)
				*pActualEndTime = date.m_dt;
		}
	}
	else
	{
		bSuccess = false;
		*pActualEndTime = 0.0;
		*pActualStartTime = 0.0;
		*plNumPoints = 0;
	}
	if (pDataRecordSet)
		m_pDb->DestroyRecordSet ((ADODB::_RecordsetPtr)pDataRecordSet,bWasOpen);
	return bSuccess;
}

bool CEOSSData::GetEOSSDataStoreTimeRange(DATE *pdStart, DATE *pdEnd)
{
	bool bSuccess = false;
	bool bWasOpen = false;
	unsigned int uiNum= 0;
	bool bEmptyIsSuccess = true;
	ADODB::_RecordsetPtr pDataRecordSet;
	CString search;
	search.Format ("SELECT * FROM EossBinaryData ORDER BY Date_Timestamp");
	bSuccess = m_pDb->CreateRecordSet(search, (ADODB::_RecordsetPtr *)&pDataRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);

	if (uiNum > 0 && bSuccess)
	{
		COleDateTime date;
		pDataRecordSet->MoveFirst ();
		bSuccess = m_pDb->GetRecordFieldDouble ((ADODB::_RecordsetPtr )pDataRecordSet, "Date_Timestamp",&date.m_dt);
		if (bSuccess)
			*pdStart = date.m_dt;
		//Modify to return last date timestamp from other table.
		/*int index = 0;
		while (index < uiNum-1)
		{
			pDataRecordSet->MoveNext();
			index ++;
		}
		bSuccess = m_pDb->GetRecordFieldDouble ((ADODB::_RecordsetPtr) pDataRecordSet,"Date_Timestamp",&date.m_dt);
		if (bSuccess)
			*pdEnd = date.m_dt;*/
		if (pDataRecordSet)
			m_pDb->DestroyRecordSet (pDataRecordSet,true);

		search="SELECT * FROM EossDataRecordsReadTable";
		bSuccess = m_pDb->CreateRecordSet(search, (ADODB::_RecordsetPtr *)&pDataRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);

		if (pDataRecordSet != NULL && uiNum> 0)
		{
			double dVal = 0;
			bool bSuccess = m_pDb->GetRecordFieldDouble (pDataRecordSet,"Date_Timestamp_LastRecord",&dVal);
			if (bSuccess)
				*pdEnd = dVal;
		}
	}
	else
	{
		*pdStart = 0;
		*pdEnd = 0;
		bSuccess = false;
	}

	if (pDataRecordSet)
		m_pDb->DestroyRecordSet ((ADODB::_RecordsetPtr)pDataRecordSet,bWasOpen);

	return bSuccess;
}


bool CEOSSData::GetEOSSDaySummaryData(long lStationID, DATE dtStart, DATE dtEnd, VARIANT *pvDayData)
{
	CString search;
	search.Format ("SELECT Date_Timestamp FROM EossBinaryData WHERE Reader_ID = %u AND Date_Timestamp >= %16.20f AND Date_Timestamp <= %16.20f ORDER BY Date_Timestamp;",lStationID,dtStart,dtEnd);
	bool bWasOpen = false;
	unsigned int uiNum= 0;
	bool bSuccess = false;
	bool bEmptyIsSuccess = true;

	DATE dActualStart = 0;
	DATE dActualEnd = 0;

	long lNumDays;

	bSuccess = GetEOSSDayStatistics (lStationID,dtStart,dtEnd,&dActualStart,&dActualEnd,&lNumDays);
	ADODB::_RecordsetPtr pDataRecordSet;
	SAFEARRAYBOUND saBound [2];
	if (lNumDays > 0)
	{
		saBound[0].cElements = lNumDays;
		saBound[0].lLbound = 0;
		saBound[1].cElements=3;
		saBound[1].lLbound = 0;
		pvDayData->vt = VT_VARIANT|VT_ARRAY;
		pvDayData->parray = SafeArrayCreate (VT_VARIANT,2,saBound);
		bSuccess = true;
	}
	else
	{
		pvDayData->vt = VT_EMPTY;
		bSuccess = false;
	}

	if (bSuccess)
	{
		bSuccess = m_pDb->CreateRecordSet(search, (ADODB::_RecordsetPtr *)&pDataRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);
	
		if (bSuccess)
		{
			pDataRecordSet->MoveFirst ();
			COleDateTime date,dayAdded,day,lastInDay;
			long lDayIndex = 0;
			double dPtsInDay = 0.0;
			for (int i = 0; i < uiNum; i ++)
			{
				date.m_dt = 0.0;
				
				m_pDb->GetRecordFieldDouble ((ADODB::_RecordsetPtr)pDataRecordSet,"Date_Timestamp",&date.m_dt);
				if (i == 0)
				{
					// This is the start time of the first day
					SA_PutElem (pvDayData->parray,lDayIndex,0,(COleDateTime)date);
					dayAdded.SetDateTime (date.GetYear(),date.GetMonth(),date.GetDay(),0,0,0);
					dPtsInDay += 1;
				}

				day.SetDateTime(date.GetYear(),date.GetMonth (),date.GetDay(),0,0,0);

				if (day == dayAdded)
				{
					dPtsInDay +=1;
					lastInDay = date;
				}
				else
				{
					SA_PutElem (pvDayData->parray,lDayIndex,1,(COleDateTime) lastInDay);
					SA_PutElem (pvDayData->parray,lDayIndex,2,(double) dPtsInDay);
					dPtsInDay = 1;
					lastInDay = 0.0;
					lDayIndex +=1;
					SA_PutElem (pvDayData->parray,lDayIndex,0,(COleDateTime)date);
					dayAdded.SetDateTime (date.GetYear(),date.GetMonth(),date.GetDay(),0,0,0);
				}
				if (i == uiNum-1)
				{
					SA_PutElem (pvDayData->parray,lDayIndex,1,(COleDateTime) lastInDay);
					SA_PutElem (pvDayData->parray,lDayIndex,2,(double) dPtsInDay);
				}
				pDataRecordSet->MoveNext ();
			}
			bSuccess = true;
		}
	}
	if (pDataRecordSet)
		m_pDb->DestroyRecordSet ((ADODB::_RecordsetPtr)pDataRecordSet,bWasOpen);
	return bSuccess;
}

