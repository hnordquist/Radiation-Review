// MyDateTime.h: interface for the CMyDateTime class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GenDateTime.h"
#include <wtypes.h>

#define MAX_DT_LEN 12		// length of longest string possible 12:34:44.000
#define STEVES_TIMESTAMP_LEN 22

#define DTF_4DIGIT_YEAR	20		// 05/20/2003 -- added defintion for time format with 4 digit year for IR files
class CMyDateTime  
{
public:
	CMyDateTime();
	virtual ~CMyDateTime();

	DATE MyTimestampToDATETimestamp(const double dTimestamp);
	double DATETimestampToMyTimestamp(const DATE Timestamp);

	void DATETimestampToDateTimeStrs(const DATE dTimestamp, char *pDateStr, char *pTimeStr, const int iDateFormat= GEN_DTF_IAEA, const int iTimeFormat = GEN_DTF_HMS);
	DATE DateTimeStrsToDATETimestamp(const char *pDateStr, const char *pTimeStr,const int iDateFormat= GEN_DTF_IAEA, const int iTimeFormat = GEN_DTF_HMS);

	void DATETimestampToStevesTimestampStrings(const DATE dTimestamp, char *szStrFormat1, char *szStrFormat2);
	DATE StevesTimestampStringToDATETimestamp(const char *pszTimestamp);
		
	void DATETimestampToGenStructs(const DATE dTimestamp, GEN_DATE_STRUCT *pDate, GEN_TIME_STRUCT *pTime, const int iDateFormat= GEN_DTF_IAEA, const int iTimeFormat = GEN_DTF_HMS);
	DATE GenStructsToDATETimestamp(const GEN_DATE_STRUCT *pDate, const GEN_TIME_STRUCT *pTime, const int iDateFormat= GEN_DTF_IAEA, const int iTimeFormat = GEN_DTF_HMS);

	void GetCurrentDateTimeStr(char *pszDateStr, char*pszTimeStr);

protected:
	void SetMilliSecs(DATE *pdDateTime, const int nMilliSecs);
	int GetMilliSecs(DATE dDateTime);

};

