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
////////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : OleTimeUtils.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////
#include "Radbuild.h"
#include "gen.h"
#include "OleTimeUtils.h"


COleDateTime MyTimestampToCOleDateTime(unsigned long ulTimestamp, CString *pStr)
{
	char Date[20], Time[20];
	int iYr, iMon, iDay, iHr, iMin, iSec;
	COleDateTime DateTime;
	static COleDateTime LastDateTime;
	static iLastSec;

	gen_seconds_to_date_time_str(Date, Time,&ulTimestamp, GEN_DTF_US);
	gen_unpack_date(Date, &iDay, &iMon, &iYr, GEN_DTF_US);
	gen_unpack_time(Time, &iHr, &iMin, &iSec);
	if (pStr) pStr->Format("Gen %02d:%02d:%02d", iHr, iMin, iSec);
	DateTime.SetDateTime(iYr, iMon, iDay, iHr, iMin, iSec);
	if (DateTime == LastDateTime)
		iYr = iYr;	// something to debug on
	LastDateTime = DateTime;
	iLastSec = iSec;
	return(DateTime);
}

COleDateTime MyTimestampToCOleDateTime(double dTimestamp, CString *pStr)
{
	char Date[20], Time[20];
	int iYr, iMon, iDay, iHr, iMin, iSec;
	COleDateTime DateTime;
	static COleDateTime LastDateTime;
	static iLastSec;
	unsigned long ulTimestamp;

	ulTimestamp = (unsigned long)dTimestamp;
	gen_seconds_to_date_time_str(Date, Time,&ulTimestamp, GEN_DTF_US);
	gen_unpack_date(Date, &iDay, &iMon, &iYr, GEN_DTF_US);
	gen_unpack_time(Time, &iHr, &iMin, &iSec);
	if (pStr) pStr->Format("Gen %02d:%02d:%02d", iHr, iMin, iSec);
	DateTime.SetDateTime(iYr, iMon, iDay, iHr, iMin, iSec);
	if (DateTime == LastDateTime)
		iYr = iYr;	// something to debug on
	LastDateTime = DateTime;
	iLastSec = iSec;
	return(DateTime);
}


unsigned long COleDateTimeToMyTimestamp(const COleDateTime& DateTime)
{

	char Date[20], Time[20];
	unsigned long ulJulian;

	strcpy(Date, DateTime.Format("%x"));
	sprintf(Date, "%s", DateTime.Format("%x"));
	sprintf(Time, "%s", DateTime.Format("%X"));
	gen_date_time_str_to_seconds(&ulJulian,Date, Time, GEN_DTF_US);
	return(ulJulian);
}

double COleDateTimeToMyDoubleTimestamp(const COleDateTime& DateTime)
{

	char Date[20], Time[20];
	unsigned long ulJulian;

	strcpy(Date, DateTime.Format("%x"));
	sprintf(Date, "%s", DateTime.Format("%x"));
	sprintf(Time, "%s", DateTime.Format("%X"));
	gen_date_time_str_to_seconds(&ulJulian,Date, Time, GEN_DTF_US);
	return((double)ulJulian);
}


// write COleDateTime in format of yyyymmdd - xx:xx:xx
CString ColeDateTimeToStevesDateAndTime(const COleDateTime& DateTime)
{
	CString strTemp;

	strTemp = DateTime.Format("%Y.%m.%d - %H:%M:%S");
	return(strTemp);
}

// write double Timestamp in format of yyyymmdd - xx:xx:xx
CString MyDoubleTimestampToStevesDateAndTime(const double dTimestamp)
{
	CString strTemp;
	COleDateTime DateTime;

	DateTime = MyTimestampToCOleDateTime(dTimestamp);
	strTemp = DateTime.Format("%Y.%m.%d - %H:%M:%S");
	return(strTemp);
}

// write COleDateTime in format of 000yyyymmddhhmmss000
// last 000 is for milliseconds.
CString ColeDateTimeToStevesTimestamp(const COleDateTime& DateTime)
{
	CString strTemp;

	strTemp = DateTime.Format("00000%Y%m%d%H%M%S000");
	return(strTemp);
}

// write double in format of 000yyyymmddhhmmss000
// last 000 is for milliseconds.
CString MyDoubleTimestampToStevesTimestamp(const double dTimestamp)
{
	CString strTemp, strTemp1;
	COleDateTime DateTime;

	DateTime = MyTimestampToCOleDateTime(dTimestamp);
	strTemp = DateTime.Format("00000%Y%m%d%H%M%S");
	strTemp1.Format("%03d", GetMilliSecsFromDouble(dTimestamp));
	return(strTemp);
}


int GetMilliSecsFromDouble(double dTimestamp)
{
        long l = (long)dTimestamp;
        double dFracSecs = dTimestamp - (double)l;
        int nMilliSecs = (int)(dFracSecs * 1000.0 + 0.5);
        return(nMilliSecs);
}


COleDateTime COleDateTimePlusSeconds(const COleDateTime& DateTime, unsigned long ulSecsToAdd)
{
	return(DateTime + MyTimestampToCOleDateTime(ulSecsToAdd));
}


// from ToM
int GetMilliSecs(COleDateTime odtDateTime)
{
        const double dSEC_PER_DAY = 86400.0;

        double dSecs = odtDateTime.m_dt * dSEC_PER_DAY;
		__int64 h = (__int64)dSecs;
        //long l = (long)dSecs;
        double dFracSecs = dSecs - (double)h;
        int nMilliSecs = (int)(dFracSecs * 1000.0 + 0.5);

        return(nMilliSecs);
}


// from ToM
void SetMilliSecs(COleDateTime *podtDateTime, int nMilliSecs)
{
        int nCurrentMilliSecs = GetMilliSecs(*podtDateTime);
        double dFracDays = (double)(nMilliSecs - nCurrentMilliSecs) / 1000.0 / 86400.0;

        podtDateTime->m_dt += dFracDays;
        return;
}


double MyDoubleTimestampToDayStart(double dTimestamp)
{
   char szDate[20];
   char szTime[20];
   unsigned long ulDayInSecs = (unsigned long)dTimestamp;
   unsigned long ulDayStart;
   double dDayStart;

   gen_seconds_to_date_time_str(szDate, szTime, &ulDayInSecs, GEN_DTF_IAEA);
   gen_date_time_str_to_seconds(&ulDayStart, szDate, "00:00:00", GEN_DTF_IAEA);
   dDayStart = ulDayStart;
   return(dDayStart);
}
