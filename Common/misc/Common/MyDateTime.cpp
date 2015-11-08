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
// MyDateTime.cpp: implementation of the CMyDateTime class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support generic time/data routines.
//
//	Functions:
//		DATETimestampToMyTimestamp - converts DATE to 1952-based timestamps
//		MyTimestampToDATETimestamp - converts 1952-based timestamps to DATE
//		DATETimestampToDateTimeStrs - converts DATE to OS or IAEA date/time strings
//		DateTimeStrsToDATETimestamp - converts OS or IAEA date/time strings to DATE
//		GetMilliSecs - determine number of ms in a DATE
//		SetMilliSecs - set number of ms in a DATE
//		DATETimestampToStevesTimestampStrings - converts DATE to Steve's type strings
//		StevesTimestampStringToDATETimestamp - converts Steve's type strings to DATE
//		DATETimestampToGenStructs - convert DATE to GEN lib type structs
//		GenStructsToDATETimestamp - convert GEN lib type structs to DATE
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MyDateTime.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
// CMyDateTime class contains common conversion routines associated
// with time/date type information.  It supports conversions to/from
// the legacy 1952-based julian dates, IAEA format time/date strings, 
// specials time/date strings used in IR, the GEN lib type structs.
//////////////////////////////////////////////////////////////////////

CMyDateTime::CMyDateTime()
{

}

CMyDateTime::~CMyDateTime()
{

}


///////////////////////////////////////////////////////////////////////////
//	Name:	DATETimestampToMyTimestamp
//
//	Description:
//	Converts a DATE timestamp to the number of seconds since 1/1/1952 (historical NIS-5 timestamp)
//
//	Declaration:
//	double CMyDateTime::DATETimestampToMyTimestamp(const DATE dTimestamp)
//
//	Input:	Timestamp	- time in DATE to be converted
//			
//	Output:	none
//
//	Return:	double			- time that has been converted to seconds since 1/1/1952 
//	
//  date    /	author	revision
//  -----------------	--------
//	01-Mar-2002	SFK		Created
//////////////////////////////////////////////////////////////////
double CMyDateTime::DATETimestampToMyTimestamp(const DATE dTimestamp)
{
	COleDateTime oleTimestamp;
	char Date[20], Time[20];
	unsigned long ulJulian;
	CGenDateTime GenDate;

	oleTimestamp.m_dt = dTimestamp;
	sprintf(Date, "%s", oleTimestamp.Format("%x"));
	sprintf(Time, "%s", oleTimestamp.Format("%X"));
	GenDate.GenDateTimeStrToSeconds(&ulJulian, Date, Time, GEN_DTF_US);
	return((double)ulJulian);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	MyTimestampToDATETimestamp
//
//	Description:
//  Converts a timestamp expressed as the number of seconds since 1/1/1952
//  to a DATE timestamp.
//
//	Declaration:
//	DATE CMyDateTime::MyTimestampToDATETimestamp(const double dTimestamp)
//
//	Input:	dTimestamp	- time in seconds since 1/1/1952 to be converted
//			
//	Output:	none
//
//	Return:	DATE				- time that has been converted to DATE format
//	
//  date    /	author	revision
//  -----------------	--------
//	01-Mar-2002	SFK		Created
//////////////////////////////////////////////////////////////////
DATE CMyDateTime::MyTimestampToDATETimestamp(const double dTimestamp)
{
	char Date[20], Time[20];
	int iYr, iMon, iDay, iHr, iMin, iSec;
	COleDateTime oleDateTime;
	CGenDateTime GenDate;
	unsigned long ulTimestamp = (unsigned long) dTimestamp;

	GenDate.GenSecondsToDateTimeStr(Date, Time, &ulTimestamp, GEN_DTF_US);
	GenDate.GenUnpackDate(Date, &iDay, &iMon, &iYr, GEN_DTF_US);
	GenDate.GenUnpackTime(Time, &iHr, &iMin, &iSec);
	oleDateTime.SetDateTime(iYr, iMon, iDay, iHr, iMin, iSec);

	// Handle case of any ms in my timestamp (like for Bot data)
	double dMilliSecs = (dTimestamp - (double)ulTimestamp) * 1000;
	SetMilliSecs(&(oleDateTime.m_dt), (int)dMilliSecs);
	return(oleDateTime);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	DATETimestampToDateTimeStrs
//
//	Description:
//	Converts a DATE timestamp to date and time strings.  The date format is specified to be either
//	the native format specified by the OS or IAEA format which is yy.mm.dd hh:mm:ss.  If the time format
//	requests milliseconds the time is expressed as hh:mm:ss.xxx where xxx is the number of milliseconds.
//
//	Declaration:
//	void CMyDateTime::DATETimestampToDateTimeStrs(const DATE dTimestamp, char *pDateStr, char *pTimeStr, const int iDateFormat, const int iTimeFormat)
//
//	Input:	dTimestamp	- timestamp in DATE format to be converted
//			iDateFormat - format supported: OS or GEN_DTF_IAEA or DTF_4DIGIT_YEAR.  Default is GEN_DTF_IAEA.
//			iTimeFormat - either hh:mm:ss (GEN_DTF_HMS) or hh:mm:ss.mmm (GEN_DTF_HMSM).  Default is GEN_DTF_HMS. 
//			
//	Output:	pDateStr	- string containing date in specified format (user must allocate storage)
//			pTimeStr	- string continaing time in specified format (user must allocate storage)
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	01-Jun-2002	SFK		Created
//	05-May-2003	SFK		Added another format defintion to always produce 4-digit years.  Used when producing files for IR.
//////////////////////////////////////////////////////////////////
void CMyDateTime::DATETimestampToDateTimeStrs(
	const DATE dTimestamp, 
	char *pDateStr, 
	char *pTimeStr, 
	const int iDateFormat, 
	const int iTimeFormat)
{
	//gen_seconds_to_date_time_str(pszFirstDate, szTime, &ulFirstDbDay, GEN_DTF_IAEA);
	COleDateTime oleTimestamp;
	DATE dTempTimestamp;
	CString strDate, strTime, strTemp;
	
	// get rid of any ms before using the conversions and format them at the end
	// appears the COle stuff rounds it up.
	int iMilliSecs = GetMilliSecs(dTimestamp);
	dTempTimestamp = dTimestamp - (double)((iMilliSecs/1000.0)/86400.0);
	oleTimestamp.m_dt = dTempTimestamp;
	
	if (iDateFormat == 0) {		// default to the format selected by the OS
		strDate = oleTimestamp.Format(VAR_DATEVALUEONLY);
	}
	else if (iDateFormat == GEN_DTF_IAEA) {	// use yy.mm.dd
		strDate = oleTimestamp.Format("%y.%m.%d");
	}
	else if (iDateFormat == DTF_4DIGIT_YEAR) {	// use yyyy.mm.dd
		strDate = oleTimestamp.Format("%Y.%m.%d");
	}

	// do the time stuff
	// sfk 7/14/2003 strTime = oleTimestamp.Format(VAR_TIMEVALUEONLY);
	strTime = oleTimestamp.Format("%H:%M:%S");
	int iDum = strTime.GetLength();
	if (strTime.GetLength() == 0) strTime = "00:00:00";
	if (iTimeFormat == GEN_DTF_HMSM) {
		strTemp.Format(".%03d", iMilliSecs);
		strTime += strTemp;		// append ms to timestring
	}

	strncpy(pDateStr, strDate.GetBuffer(MAX_DT_LEN+1), MAX_DT_LEN+1);
	strDate.ReleaseBuffer(); //KDM5
	strncpy(pTimeStr, strTime.GetBuffer(MAX_DT_LEN+1), MAX_DT_LEN+1);
	strTime.ReleaseBuffer(); //KDM5


}


///////////////////////////////////////////////////////////////////////////
//	Name:	DATETimestampToDateTimeStrs
//
//	Description:
//	Converts a DATE timestamp to date and time strings.  The date format is specified to be either
//	the native format specified by the OS or IAEA format which is yy.mm.dd hh:mm:ss.  If the time format
//	requests milliseconds the time is expressed as hh:mm:ss.xxx where xxx is the number of milliseconds.
//
//	Declaration:
//	DATE CMyDateTime::DateTimeStrsToDATETimestamp(const char *pDateStr, const char *pTimeStr, const int iDateFormat, const int iTimeFormat)
//
//	Input:	pDateStr	- string representing the date
//			pTimeStr	- string representing the time
//			iDateFormat - either the format selected by the OS or GEN_DTF_IAEA supported.  Default is GEN_DTF_IAEA.
//			iTimeFormat - either hh:mm:ss (GEN_DTF_HMS) or hh:mm:ss.mmm (GEN_DTF_HMSM).  Default is GEN_DTF_HMS. 
//			
//	Output:	pDateStr	- string containing date in specified format (user must allocate storage)
//			pTimeStr	- string continaing time in specified format (user must allocate storage)
//
//	Return:	DATE		- time represented by strings expressed in DATE format
//	
//  date    /	author	revision
//  -----------------	--------
//	01-Jun-2002	SFK		Created
//////////////////////////////////////////////////////////////////
DATE CMyDateTime::DateTimeStrsToDATETimestamp(const char *pDateStr, const char *pTimeStr, const int iDateFormat, const int iTimeFormat)
{
	
	COleDateTime oleTimestamp;
	CString DateTimeStr = pDateStr;
	DateTimeStr += " ";
	DateTimeStr += pTimeStr;

	if (iDateFormat == 0) {
		oleTimestamp.ParseDateTime(DateTimeStr);
	}
	else if (iDateFormat == GEN_DTF_IAEA) {	// format is yy.mm.dd hh:mm:ss.xxx 
		CString strTemp;
		
		CString strToParse = pDateStr;
		strTemp   = strToParse.Left(2);
		int iYear = atoi(strTemp.GetBuffer(2));
		strTemp.ReleaseBuffer(); //KDM5
		if (iYear < 52) iYear +=2000;
		else iYear += 1900;
		strTemp   = strToParse.Mid(3,2); 
		int iMon  = atoi(strTemp.GetBuffer(2));
		strTemp.ReleaseBuffer(); //kdm5
		strTemp   = strToParse.Mid(6,2);
		int iDay  = atoi(strTemp.GetBuffer(2));
		strTemp.ReleaseBuffer(); //kdm5

		strToParse = pTimeStr;
		strTemp  = strToParse.Left(2); 
		int iHr  = atoi(strTemp.GetBuffer(2));
		strTemp.ReleaseBuffer(); //kdm5
		strTemp  = strToParse.Mid(3,2); 
		int iMin = atoi(strTemp.GetBuffer(2));
		strTemp.ReleaseBuffer(); //KDM5
		strTemp  = strToParse.Mid(6,2); 
		int iSec = atoi(strTemp.GetBuffer(2));
		strTemp.ReleaseBuffer(); //KDM5

		int iMSec = 0;
		if (iTimeFormat == GEN_DTF_HMSM) {
			strTemp = strToParse.Mid(9,3); 
			int iMSec = atoi(strTemp.GetBuffer(3));
			strTemp.ReleaseBuffer(); //kdm5
		}
		oleTimestamp.SetDateTime(iYear, iMon, iDay, iHr, iMin, iSec);
		SetMilliSecs(&oleTimestamp.m_dt, iMSec);
	}

	return(oleTimestamp.m_dt);
}
		

///////////////////////////////////////////////////////////////////////////
//	Name:	GetMilliSecs
//
//	Description:
// Get the number of milliseconds associated with the current DATE value
//
//	Declaration:
//	int CMyDateTime::GetMilliSecs(DATE dDateTime)
//
//	Input:	dDateTime	- DATE to extract the milliseconds from
//			
//	Output:	none
//
//	Return:	int			- number of milliseconds in odtDateTimetime represented by strings expressed in DATE format
//	
//  date    /	author	revision
//  -----------------	--------
//	01-Jun-2002	SFK		Adapted from routine from ToM
//////////////////////////////////////////////////////////////////
int CMyDateTime::GetMilliSecs(const DATE dDateTime)
{
        const double dSEC_PER_DAY = 86400.0;
		const double dONE_MILLISEC = 1.0/86400.0;

        double dSecs = dDateTime - floor(dDateTime);	// get just the fractional part of the day
		double dTemp = dSecs * dSEC_PER_DAY;  			// convert to number of seconds in a day
        double dFracSecs = dTemp - floor(dTemp);		// get just the fractional part of a second
		double dMilliSecs = dFracSecs * 1000.0;
		dMilliSecs += 0.5;
        int nMilliSecs = (int)(dMilliSecs);
		if (nMilliSecs >=1000) nMilliSecs = 0;
		return(nMilliSecs);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	SetMilliSecs
//
//	Description:
//	Set the number of milliseconds associated with the current DATE value
//
//	Declaration:
//	void CMyDateTime::SetMilliSecs(DATE *pdDateTime, const int nMilliSecs)
//
//	Input:	pdDateTime	- DATE to set the milliseconds on
//			nMilliSecs	- number of milliseconds to set on the COleDateTime
//			
//	Output:	pdDateTime	- DATE with the proper number of milliseconds
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	01-Jun-2002	SFK		Adapted from routine from ToM
//////////////////////////////////////////////////////////////////
void CMyDateTime::SetMilliSecs(DATE *pdDateTime, const int nMilliSecs)
{
        int nCurrentMilliSecs = GetMilliSecs(*pdDateTime);		// get existing number of ms
        double dFracDays = (double)(nMilliSecs - nCurrentMilliSecs) / 1000.0 / 86400.0;

        *pdDateTime += dFracDays;
        return;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	DATETimestampToStevesTimestampStrings
//
//	Description:
//	Convert the DATE timestamp to the two formats expected for the IR table strings
//	1234519970103093456000 and yyyymmdd - xx:xx:xx
//
//	Declaration:
//	CString CMyDateTime::DATETimestampToStevesDateAndTime(const DATE dTimestamp)
//
//	Input:	Timestamp	- DATE timestamp to convert
//			
//	Output:	none
//
//	Return:	CString		- Date/Time expressed in IR format of yyyymmdd - xx:xx:xx
//	
//  date    /	author	revision
//  -----------------	--------
//	01-Jun-2002	SFK		Created from MyDoubleTimestampToStevesDateAndTime in OleTimeUtils.cpp
//	05-May-2003	SFK		Fixed to actually return strings formed.
//	20-May-2003	SFK		Fixed to return correctly formatted year in szStrFormat1
//////////////////////////////////////////////////////////////////
void CMyDateTime::DATETimestampToStevesTimestampStrings(const DATE dTimestamp, char *szStrFormat1, char *szStrFormat2)
{
	CString strTemp1, strTemp2;
	COleDateTime DateTime;
	char szTime[STEVES_TIMESTAMP_LEN+1], szDate[STEVES_TIMESTAMP_LEN+1];
	DateTime.m_dt = dTimestamp;

	DATETimestampToDateTimeStrs(dTimestamp, szDate, szTime);
	szDate[2] = '\0';
	int iYear = atoi(szDate);
	//if (iYear < 52) iYear +=20;
	//else iYear += 19;
	if (iYear < 52) iYear =20;
	else iYear = 19;
	szDate[5] = '\0';
	szTime[2] = '\0';
	szTime[5] = '\0';
	strTemp1.Format("00000%2d%s%s%s%s%s%s000", iYear, &szDate[0], &szDate[3], &szDate[6], 
					                              &szTime[0], &szTime[3], &szTime[6]);

	strTemp2 = DateTime.Format("%Y.%m.%d - %H:%M:%S");

	strcpy(szStrFormat1, strTemp1.GetBuffer(strTemp1.GetLength()));		// sfk 5/15/2003
	strcpy(szStrFormat2, strTemp2.GetBuffer(strTemp2.GetLength()));		// sfk 5/15/2003

}


///////////////////////////////////////////////////////////////////////////
//	Name:	StevesTimestampStringToDATETimestamp
//
//	Description:
//	Convert the ASCII timestamp string to its DATE equivalent.
//	TimestampStr format 1234519970103093456000
//
//	Declaration:
//	CString CMyDateTime::DATETimestampToStevesDateAndTime(const DATE dTimestamp)
//
//	Input:	Timestamp	- DATE timestamp to convert
//			
//	Output:	none
//
//	Return:	DATE		- DATE number representing string.
//	
//  date    /	author	revision
//  -----------------	--------
//	12-Aug-2002	SFK		Created from ConvertTimestampStrToJulianSecs in DateTimeUtils.cpp
//////////////////////////////////////////////////////////////////
DATE CMyDateTime::StevesTimestampStringToDATETimestamp(const char *pszTimestamp)
{

	char szTempTimestamp[STEVES_TIMESTAMP_LEN+1];
	int iYear, iMonth, iDay, iHour, iMinute, iSecond;
	
	// parse the string into year, month, day, hour, minute second
    strncpy(szTempTimestamp, pszTimestamp, STEVES_TIMESTAMP_LEN+1); 
    szTempTimestamp[9] = '\0';
	iYear = atoi(&szTempTimestamp[5]);	// parse the year

    strncpy(szTempTimestamp, pszTimestamp, STEVES_TIMESTAMP_LEN+1); 
    szTempTimestamp[11] = '\0';
	iMonth = atoi(&szTempTimestamp[9]);	// parse the month

    strncpy(szTempTimestamp, pszTimestamp, STEVES_TIMESTAMP_LEN+1); 
    szTempTimestamp[13] = '\0';
	iDay = atoi(&szTempTimestamp[11]);	// parse the day

    strncpy(szTempTimestamp, pszTimestamp, STEVES_TIMESTAMP_LEN+1); 
    szTempTimestamp[15] = '\0';
	iHour = atoi(&szTempTimestamp[13]);	// parse the hour

    strncpy(szTempTimestamp, pszTimestamp, STEVES_TIMESTAMP_LEN+1); 
    szTempTimestamp[17] = '\0';
	iMinute = atoi(&szTempTimestamp[15]);	// parse the minute

    strncpy(szTempTimestamp, pszTimestamp, STEVES_TIMESTAMP_LEN+1); 
    szTempTimestamp[19] = '\0';
	iSecond = atoi(&szTempTimestamp[17]);	// parse the second

	COleDateTime oleTimestamp(iYear, iMonth, iDay, iHour, iMinute, iSecond);
	return(oleTimestamp.m_dt);
}	


///////////////////////////////////////////////////////////////////////////
//	Name:	ConvertDATEToGenStructs
//
//	Description:
//	Convert a DATE timestamp to Gen library date and time structs used in dialog
//	boxes based on GuiLib
//
//	Declaration:
//	void CMyDateTime::DATETimestampToGenStructs(const DATE dTimestamp, GEN_DATE_STRUCT *pDate, GEN_TIME_STRUCT *pTime, const int iDateFormat, const int iTimeFormat)
//
//	Input:	Timestamp	- DATE timestamp to convert
//			iDateFormat - either the format selected by the OS or GEN_DTF_IAEA supported.  Default is GEN_DTF_IAEA.
//			iTimeFormat - either hh:mm:ss (GEN_DTF_HMS) or hh:mm:ss.mmm (GEN_DTF_HMSM).  Default is GEN_DTF_HMS. 
//			
//	Output:	pDate		- GEN_DATE_STRUCT for date of Timestamp
//			pTime		- GEN_TIME_STRUCT for time of Timestamp
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	01-Jun-2002	SFK		Created from ConvertSecondsToStructs in DateTimeUtils.cpp
//////////////////////////////////////////////////////////////////
void CMyDateTime::DATETimestampToGenStructs(const DATE dTimestamp, GEN_DATE_STRUCT *pDate, GEN_TIME_STRUCT *pTime, const int iDateFormat, const int iTimeFormat)

{
	char szDate[MAX_DT_LEN+1], szTime[MAX_DT_LEN+1];
	CGenDateTime GenDT;

	DATETimestampToDateTimeStrs(dTimestamp, szDate, szTime, iDateFormat, iTimeFormat);
	GenDT.GenDateStrToStruct(szDate, iDateFormat, pDate);
	GenDT.GenTimeStrToStruct(szTime, iTimeFormat, pTime);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GenStructsToDATETimestamp
//
//	Description:
//	Convert a date/time represented by Gen structs to a DATE timestamp
//
//	Declaration:
//	DATE CMyDateTime::GenStructsToDATETimestamp(const GEN_DATE_STRUCT *pDate, const GEN_TIME_STRUCT *pTime, const int iDateFormat, const int iTimeFormat)
//
//	Inputt:	pDate		- GEN_DATE_STRUCT for date of Timestamp
//			pTime		- GEN_TIME_STRUCT for time of Timestamp
//			iDateFormat - either the format selected by the OS or GEN_DTF_IAEA supported.  Default is GEN_DTF_IAEA.
//			iTimeFormat - either hh:mm:ss (GEN_DTF_HMS) or hh:mm:ss.mmm (GEN_DTF_HMSM).  Default is GEN_DTF_HMS. 
//			
//	Output:	none
//
//	Return:	DATE		- DATE timestamp derived from the Gen structs
//	
//  date    /	author	revision
//  -----------------	--------
//	01-Jun-2002	SFK		Created from ConvertStructsToSeconds in DateTimeUtils.cpp
//////////////////////////////////////////////////////////////////
DATE CMyDateTime::GenStructsToDATETimestamp(const GEN_DATE_STRUCT *pDate, const GEN_TIME_STRUCT *pTime, const int iDateFormat, const int iTimeFormat)
{
	CGenDateTime GenDT;
	char szDate[MAX_DT_LEN+1], szTime[MAX_DT_LEN+1];
	char *pStr;
	GenDT.GenDateStructToStr(*pDate, GEN_DTF_IAEA, &pStr);
	strcpy(szDate, pStr);
	GenDT.GenTimeStructToStr(*pTime, GEN_DTF_HMS, &pStr);
	strcpy(szTime, pStr);
	return(DateTimeStrsToDATETimestamp(szDate, szTime, iDateFormat, iTimeFormat));
}
   

///////////////////////////////////////////////////////////////////////////
//	Name:	GetCurrentDateTimeStr
//
//	Description:
//	Gets the current date/time and converts them to strings
//
//	Declaration:
//	void CMyDateTime::GetCurrentDateTimeStr(char *pszDateStr, char*pszTimeStr)
//
//	Input:	none
//			
//	Output:	pszDateStr	Current date formatted 
//			pszTimeStr	Current time formatted
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	08-Jul-2004	SFK		Created 
//////////////////////////////////////////////////////////////////
void CMyDateTime::GetCurrentDateTimeStr(char *pszDateStr, char*pszTimeStr)
{
	COleDateTime Time;
	Time = COleDateTime::GetCurrentTime();
	DATETimestampToDateTimeStrs(Time.m_dt, pszDateStr, pszTimeStr, DTF_4DIGIT_YEAR, GEN_DTF_HMS);	
}



