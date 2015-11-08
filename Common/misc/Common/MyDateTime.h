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

