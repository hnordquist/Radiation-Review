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