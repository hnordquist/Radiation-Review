#if !defined(OLETIMEUNITS_H)
#define OLETIMEUNITS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

COleDateTime MyTimestampToCOleDateTime(unsigned long ulTimestamp, CString *pStr = NULL);
COleDateTime MyTimestampToCOleDateTime(double dTimestamp, CString *pStr = NULL);

unsigned long COleDateTimeToMyTimestamp(const COleDateTime& DateTime);
double COleDateTimeToMyDoubleTimestamp(const COleDateTime& DateTime);

CString ColeDateTimeToStevesTimestamp(const COleDateTime& DateTime);
CString MyDoubleTimestampToStevesTimestamp(const double dTimestamp);

CString ColeDateTimeToStevesDateAndTime(const COleDateTime& DateTime);
CString MyDoubleTimestampToStevesDateAndTime(const double dTimestamp);

int GetMilliSecs(COleDateTime odtDateTime);
int GetMilliSecsFromDouble(double dTimestamp);
void SetMilliSecs(COleDateTime *podtDateTime, int nMilliSecs);

double MyDoubleTimestampToDayStart(double dTimestamp);
#endif 