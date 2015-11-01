// TimeAlign.h: interface for the CTimeAlign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEALIGN_H__C6691B5D_EA08_4DE8_B3C9_AA258C5F6B32__INCLUDED_)
#define AFX_TIMEALIGN_H__C6691B5D_EA08_4DE8_B3C9_AA258C5F6B32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ComDataStructures.h"

// TimeAlignParmsStruct - contains all information for time align parameters
struct TimeAlignParmsStruct {
	short sStaNum;
// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
// We will keep this stat number a short.  We will only switch to a LONG just as the
// interface methods are called.
	short sStaType;
	double dSlope;
	double dIntercept;
	unsigned long ulEvent1Time;
	unsigned long ulEvent2Time;
	BOOL bUseTimeAlign;
	BOOL bEnableStation;
};


//int OpenTimeAlignDatastore(const char* strDataStorePath);
int OpenTimeAlignDatastore(const short sFacilityID);

class CTimeAlign  
{
public:
	CTimeAlign(const char* szDefaultPath, short sFacID);
	~CTimeAlign();

	void CloseDatabase();
	int  GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime, short sStaType);

	//int  GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays, short sStaType);
	int  GetDataByDay(const short sFacID, const struct IDStruct *pStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays);

	int  GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, VARIANT *pvData, short sStaType, unsigned long *pulNumPts);
	BOOL  GetDataStatistics(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, DATE *pdStart, DATE *pdEnd, unsigned long *pulNumPts, short sStaType);
	int  GetDayStatistics(const short sFacID, const struct IDStruct *pStaID, const DATE dStart, const DATE dEnd, DATE *pdRealStart, DATE *pdRealEnd, unsigned long *pulNumPts);
	int  GetDayStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdRealStart, DATE *pdRealEnd, unsigned long *pulNumPts);
	int  GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay, short sStaType);
	int  GetDbStartDay(const short sFacID, const short sStaID, DATE* pdStartDay, const bool bErrDisplay, short sStaType);
	bool GetParameters(const int sFacID,  const long sNum, struct TimeAlignParmsStruct *pTimeAlignParms);
	int  InitializeDatabase(const short sFacID);
	bool  OpenDatabase(const short sFacID);
	bool SetParameters(const int sFacID, short sNum, struct TimeAlignParmsStruct *pTimeAlignParms);

	BOOL COMDataType(short sType);

	char mstrDefaultPath[256];

	bool IsTimeAlignEnabled(const int iFacID,  const int uiPL_ChanIndex);

};

extern CTimeAlign *pglTimeAlign;

#endif // !defined(AFX_TIMEALIGN_H__C6691B5D_EA08_4DE8_B3C9_AA258C5F6B32__INCLUDED_)
