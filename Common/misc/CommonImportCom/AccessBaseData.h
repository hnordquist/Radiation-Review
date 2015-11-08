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
// AccessBaseData.h: interface for the CBaseAccessData class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(ACCESSBASEDATA_H)
#define ACCESSBASEDATA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ONE_HUNDREDTH_SEC 1.0/8640000.0
#define ONE_THOU_SEC 1.0/86400000.0				// 1/1000th of a second

#define DAYS_TABLE "DaysDataTable"

#define ANY_STATION	-1			// 14-Mar-2006 SFK Add any station defintion

#define DAYS_STATION_NUMBER_COL			0
#define DAYS_INSTRUMENT_TYPE_COL		1
#define DAYS_DAY_START_TIMESTAMP_COL	2
#define DAYS_FIRST_DATA_TIMESTAMP_COL	3
#define DAYS_LAST_DATA_TIMESTAMP_COL	4
#define DAYS_NUM_PTS_COL				5
#define DAYS_NUM_COLUMNS				6


typedef struct DB_D_INTERVAL_TAG {
	double dStart;
	double dEnd;
} DB_D_INTERVAL;

/////////////////////////////////////////////////////////////////////////////
// DayData - contains all information about a day 
typedef struct DayDataStruct {
	DATE	dFirstDataPt;		// first timestamp of data point in day
	DATE	dLastDataPt; 		// last timestamp of data point in day
	double	dNumPtsInDay;  		// number of data points in day
}DayDataStruct;

#include "AccessDb.h"
/////////////////////////////////////////////////////////////////////////////
// CBaseData - base behaviors that all data objects must have
class CBaseAccessData 
{
public:
	CBaseAccessData(
		CDbAccess* pDb, 
		const int iStaNum, 
		const int iInstType, 
		const bool bInterpolated,
		bool bQuietMode);

	virtual ~CBaseAccessData();

	bool GetDayData(DB_D_INTERVAL dRequested, DB_D_INTERVAL *pdActual, DayDataStruct *pDayData, CString *pErrMsg=NULL);
	bool GetDayStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrMsg = NULL);
	bool GetDataStoreTimeRange(bool bErrDisplay, DATE *pdFirstTimestamp, DATE *pdLastTimestamp);

	// These functions must be created for each specific data type
	virtual bool AddData(CString *pstrErrorMsg= NULL) = 0;
	virtual bool GetDataStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrorMsg=NULL)=0;
	virtual bool GetChannelData(const int &iChanOffset, const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, DATE *pdTimestamp, double *pdData, CString *pstrErrorMsg=NULL)=0;
	virtual bool GetStationData(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, CString *pstrErrorMsg=NULL)=0;

	// These functions deal with the DaysDataTable that summarizes information for all stations
	bool CreateDaysTable(CString *pstrErrorMsg);
	bool AddDayData(const DB_D_INTERVAL& ActualDay, unsigned long ulNumPtsInDay, CString *pstrErrorMsg=NULL);
	bool DayExists(DATE dTime, unsigned long *pulNumPts, DB_D_INTERVAL *pdActual);

	// These functions are general purpose and can be used for all the data types
	bool DoesTableExist(const CString& strTableName, CString *pstrErrorMsg=NULL);
	void CreateTimeIntervalString(const DB_D_INTERVAL& Requested, CString *pStr);

	bool DeleteDay(DATE dTime);

	CDbAccess* m_pDb;					// which Access database is open now
	CString m_strDaysCols[DAYS_NUM_COLUMNS];	// will put the labels for the DAYS_TABLE columns here
	int m_iChanNum;
	DATE m_dTimestampOfNextData;
	unsigned long m_ulNumPtsRequested;	// number of data points requested for an action
	unsigned long m_ulNumPtsActual;		// actual number of data points produced by action
	bool  m_bInterpolated;				// should we interpolate this data type
	int m_iStaNum;						// station number we are interested in now
	int m_iInstType;					// some station numbers can have multiple types of instruments (ILONs)
};



#endif // !defined(ACCESSBASEDATA_H)