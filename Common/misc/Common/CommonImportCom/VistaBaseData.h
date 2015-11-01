// BaseVistaData.h: interface for the CBaseVistaData class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

// when searching thru a multiple record, if not an exact match then either 
// match to the one before or after the requested time

enum SNAP
{
	BEFORE = 0,
	AFTER = 1
};

#define DB_START 0
#define DB_END 1

#define NUM_SECS_PER_DAY 86400L

typedef struct DB_D_INTERVAL_TAG {
	double dStart;
	double dEnd;
} DB_D_INTERVAL;


#define ANY_STATION	-1

// Status returns
// Values used by CBaseVistaData::miErrorNum
#define IBASEVISTA			10
#define	iADDED_OKAY		IBASEVISTA
#define iBAD_INTERVAL	IBASEVISTA+1
#define	iDAY_IN_DB		IBASEVISTA+2
#define	iDAY_NOT_IN_DB	IBASEVISTA+3
#define	iDB_BAD			IBASEVISTA+4
#define iDB_EMPTY		IBASEVISTA+5
#define iDB_NOT_OPEN	IBASEVISTA+6
#define iNO_DATA_FOR_STATION	IBASEVISTA+7
#define iSTA_NOT_IN_DB	IBASEVISTA+8
#define iTIME_IN_REC	IBASEVISTA+9
#define iTIME_NOT_IN_DB	IBASEVISTA+10
#define iNO_STORAGE		IBASEVISTA+11
#define iNOT_DB_ISSUE	-1  //added by pjm 7-12-2007
#define iINTERNAL_ERROR	-2

#include "vista.h"
#include "VistaDb.h"

#pragma pack(1)
#ifdef MINIGRAND
	#include <..\CommonImportCom\db_rmon.h>
#endif
#ifdef VIFM
	#include <..\CommonImportCom\db_vifm.h>
#endif
#ifdef SR					// 04-Aug-2005 SFK Added SR type
	#include <..\CommonImportCom\db_sr.h>
#endif

#ifdef MCA					// 29-Aug-2005 SFK Added MCASR type
	#include <..\CommonImportCom\db_mca.h>
#endif

#pragma pack(8)



/////////////////////////////////////////////////////////////////////////////
// CDoubleXYData - object producing arrays needed for analysis
//class CDoubleXYData : public CObject {
//public:
//	double mdTimestamp;
//	double mdData;
//};

/////////////////////////////////////////////////////////////////////////////
// DayData - contains all information about a day  also defined in CoGrandImport.idl
typedef struct DayDataStruct {
	DATE	dFirstDataPt;		// julian data of first data point in day
	DATE	dLastDataPt; 		// julian data of last data point in day
	double	dNumPtsInDay;  	// number of data points in day
}DayDataStruct;


class CBaseVistaData
{
public:
	DATE mdTimestampOfNextData;
	CBaseVistaData(CDbVista* pDb, const int iStaNum, const bool bInterpolated, const bool bQuietMode);
	virtual ~CBaseVistaData();

	virtual bool AddData(CString *pstrErrorMsg=NULL) = 0;
	virtual bool GetDataStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrorMsg=NULL)=0;
	virtual bool GetChannelData(const int &iChanOffset, const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, double *pdTimestamp, double *pdData, CString *pstrErrorMsg=NULL)=0;
	virtual bool GetStationData(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, CString *pstrErrorMsg=NULL)=0;

	bool m_bQuietMode;
	
	int  AddDayData(struct db_day_rec* pdbDay);
	int  CreateDay(DATE dDay);
	int  DayExists(DATE dTime, unsigned long *pulNumPts, DB_D_INTERVAL *pdActual);
	int  DeleteDay(DATE dTime);
	bool GetDayStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrMsg=NULL);
	bool GetDayData(DB_D_INTERVAL Requested, DB_D_INTERVAL *pActual, DayDataStruct* pDayData, CString *pErrMsg=NULL);
	int  GetDayData(DATE dDayStart, struct db_day_rec* pdbDay);
	bool GetFirstTimeInDb(DATE *pdFirstTime, bool bDisplayErr);
	bool GetLastTimeInDb(DATE *pdLastTime, bool bDisplayErr);

	void DbVistaError(CString *pstrMsg, bool bShowMsg = true);  //<<QA>>FOR RAD FOR B2R1. THIS WILL BREAK NDAR. PUT LOGIC IN FOR THIS LATER.
	bool CheckDbOpen(bool bShowMsg = true);						//<<QA>>FOR RAD FOR B2R1. THIS WILL BREAK NDAR.  PUT LOGIC IN FOR THIS LATER.
	bool StationDataExists();
	void GetDatabaseErrorText(CString& csDBError);
	CString GetDatabaseErrorText();

	CDbVista* mpDb;

	unsigned long mulNumPtsRequested;
	unsigned long mulNumPtsActual;
	int   miErrorNum;
	bool  mbInterpolated;
	short msStaNum;
	bool AnyDaysExistInDb(int iDbEnd, bool bShowMsg = false);

protected:
	int  DayExists(DATE dTime);
	DATE DayStart(DATE dTime);
	int  FindDayContainingClosestRecord(DATE dRequestedTime, SNAP eSnap, DATE *pdActualTime);
	bool SameDay(DATE dTime1, DATE dTime2);


};

