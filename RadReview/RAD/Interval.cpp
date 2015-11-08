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
///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : Interval.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		ConvertStructsToSearchIntervalParameters - Replaces IntervalToSearch
//		GetDatabaseIntervalParameters - Replaces GetDatabaseIntervalStrs
//		GetReviewPeriodParameters - Replaces GetSearchIntervalParms
//
///////////////////////////////////////////////////////////////////////////
#include "Radbuild.h"
#include "MyDateTime.h"
#include "DbLimit.h"
#include "FacilityConfig.h"
#include "UpperLayerGlue.h"
#include "DlgReviewPeriod.h"
#include <math.h>

extern CMyDateTime glMyDateTime;

static void GetReviewPeriodAsEntireDb(DATE *pdStart, DATE *pdEnd, unsigned long *pulIntervalInDays);
static void GetReviewPeriodFromUpperLayer(DATE *pdStart, DATE *pdEnd, unsigned long *pulIntervalInDays);

#define ANY_STATION -1			// from RadDefs.h
#define ANY_TYPE -1			// from RadDefs.h

///////////////////////////////////////////////////////////////////////////
//	Name:	ConvertStructsToSearchIntervalParameters
//			Replaces IntervalToSearch
//
//	Description:
//	From GEN_DATE and GEN_TIME structs, interval and interval units, 
//  calculate a starting and ending time in seconds.
//  Gets start time in seconds  from GEN_DATE and GEN_TIME structures
//  Uses iUserUnits and ulUserInterval to calculate interval in secondsConverts a DATE timestamp to the number of seconds since 1/1/1952 (historical NIS-5 timestamp)
//
//	Declaration:
//	BOOL ConvertStructsToSearchIntervalParameters(struct GEN_DATE_STRUCT *pGenDate, struct GEN_TIME_STRUCT *pGenTime, const int iIntervalUnits, unsigned long ulInterval, DATE *pdAdjStart, DATE *pdAdjEnd)
//
//	Input:	pAdjGenDate	- date in GEN_DATE_STRUCT
//			pAdjGenTime	- time in GEN_DATE_STRUCT
//			iIntervalUnits	- units for the time interval, can be 0 (days), 1(hours), 2(minutes) --
//							determined by position chosen on a picklist in dialog boxes
//			ulUserInterval -	how many of iUserUnits are in the interval
//			
//	Output:	pdStart	- timestamp of first data point >= pGenDate and pGenTime
//			pdEnd		- timestamp of data point iUserUnits*ulUserInterval beyond pulStart
//
//	Return:	TRUE (data in interval)/FALSE (no data in interval)
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//	08-Aug-02	SFK		More DATE conversion
//	19-May-2005	SFK		Fix to calculate time in minutes properly (was dividing by 3600)
//////////////////////////////////////////////////////////////////
BOOL ConvertStructsToSearchIntervalParameters(struct GEN_DATE_STRUCT *pAdjGenDate, struct GEN_TIME_STRUCT *pAjdGenTime, const int iIntervalUnits, unsigned long ulInterval, DATE *pdAdjStart, DATE *pdAdjEnd)
{	    	
	double dIntervalInDays;
	DATE dAdjFirstDbDay, dAdjLastDbDay;
	CGenDateTime Gen;
	IDStruct StaID;
		
	// calculate the start date
	*pdAdjStart = glMyDateTime.GenStructsToDATETimestamp(pAdjGenDate, pAjdGenTime);
	    	
	// calculate the interval (fractional days) based on iIntervalUnits and ulInterval
	switch (iIntervalUnits) {
		case 0:		// days
		dIntervalInDays = (double)ulInterval;
		break;
				
		case 1:		// hours
		dIntervalInDays = ((double)ulInterval)/24.0;
		break;
				
		case 2:		// minutes
		dIntervalInDays = ((double)ulInterval)/1440.0;
		break;
	}

	// calculate the end date
	*pdAdjEnd = *pdAdjStart + dIntervalInDays;

	// Get the range of all data in the database
	StaID.sStaID = ANY_STATION;
	StaID.sStaType = ANY_TYPE;
	if (!GetDbTimeRange(StaID, &dAdjFirstDbDay, &dAdjLastDbDay)) return(FALSE);

	// check if the search interval (or part of it) is in the range of data in the db
	if ((*pdAdjStart <= dAdjLastDbDay) && (*pdAdjEnd >= dAdjFirstDbDay)) return (TRUE);
	return(FALSE);

}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetDatabaseIntervalParameters
//			Replaces GetDatabaseIntervalStrs
//
//	Description:
//	Create strings for the dates represented by the first and last day in the database
//	and calculate the number of days in the database (rounded to whole days).
//	This is displayed on every dialog box that has a database interval
//	Based on the method for determining the review period (whole database, interval
//	from Review Period menu option or inspec.ini file) calculate the amount of time
//	in the review period (in seconds)
//
//	Declaration:
//	BOOL GetDatabaseIntervalParameters(char *pszAdjFirstDate, char *pszAdjLastDate, unsigned long *pulDbIntervalInWholeDays, DATE *pdDbExactInterval)
//
//	Input:	none
//			
//	Output:	pszAdjFirstDate	- string representing first day in db
//			pszAdjLastDate		- string representing last day in db
//			pulDbIntervalInWholeDays - number of days in the interval from and including pszFirstDate to pszLastDate
//			pdDbExactInterval - number of fractional days in pszAdjFirstDate to pszAdjLastDate
//
//	Return:	TRUE 
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//				SFK		Version 2.05 SFK  Added support of getting search interval from .ini file if running in IMPORT mode
//	20-May-02	SFK		Switched times to DATE type
//	08-Aug-02	SFK		More DATE conversion
//////////////////////////////////////////////////////////////////
BOOL GetDatabaseIntervalParameters(char *pszAdjFirstDate, char *pszAdjLastDate, unsigned long *pulDbIntervalInWholeDays, DATE *pdDbExactInterval)
{
	DATE dAdjFirstDbDay, dAdjLastDbDay;//, dIntervalInDaysAndSecs;
	char szDum[MAX_DT_LEN+1];       

	// Get the parameters associated with the entire database
	GetReviewPeriodAsEntireDb(&dAdjFirstDbDay, &dAdjLastDbDay, pulDbIntervalInWholeDays);

	// Form the date strings associated with the first and last daysfind the first day in the database and form the string representing that date.
	glMyDateTime.DATETimestampToDateTimeStrs(dAdjFirstDbDay, pszAdjFirstDate, szDum);
	glMyDateTime.DATETimestampToDateTimeStrs(dAdjLastDbDay, pszAdjLastDate, szDum);

	// calculate the database interval from the first time in the db to the last time in the db in
	// whole days and fractional days.
	*pdDbExactInterval = dAdjLastDbDay - dAdjFirstDbDay;
	return(TRUE);
}




///////////////////////////////////////////////////////////////////////////
//	Name:	GetReviewPeriodParameters
//			Replaces GetSearchIntervalParms
//
//	Description:
//	Gets the review period information (start/end date and interval).  The Review Period
//	is the interval that will actually be searched by the various menu options. The information can
//	come from one of three places: entire database interval, interval specified by the upper
//	layer or interval specified in the Tools/Review Period dialog box.
//
//	Declaration:
//	void GetReviewPeriodParameters(DATE *pdReviewPeriodStart, DATE *pdReviewPeriodEnd, unsigned long *pulReviewPeriodIntervalInDays)
//
//	Input:	none
//			
//	Output:	pdReviewPeriodStart				time of start of review period
//			pdReviewPeriodEnd				time of end of review period
//			pulReviewPeriodIntervalInDays	number of days in the review period (including endpoints)interval from and including pszFirstDate to pszLastDate
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//////////////////////////////////////////////////////////////////
void GetReviewPeriodParameters(DATE *pdReviewPeriodStart, DATE *pdReviewPeriodEnd, unsigned long *pulReviewPeriodIntervalInDays)
{
	// if the default search start time and interval has been set in the Tools/Review Period
	// dialog box, then use those values
	if (ReviewPeriodMenuOptionExecuted() == TRUE) {
		GetReviewPeriodFromMenuOption(pdReviewPeriodStart, pdReviewPeriodEnd, pulReviewPeriodIntervalInDays);
		return;
	}

	// if Rad is running in \IMPORT mode, then use the default search start time and interval
	// from the inspec.ini (or rad.ini) file.
	if (GetInImportModeFromRADImport() == TRUE) { // IR is being used and running in IMPORT mode
		GetReviewPeriodFromUpperLayer(pdReviewPeriodStart, pdReviewPeriodEnd, pulReviewPeriodIntervalInDays);
		return;
	}

	// if neither of the above conditions are true, then the default search start time and interval
	// is the entire db.
	GetReviewPeriodAsEntireDb(pdReviewPeriodStart, pdReviewPeriodEnd, pulReviewPeriodIntervalInDays);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetReviewPeriodAsEntireDb
//
//	Description:
//	Determine the first and last date of the database.  Also determine the number of
//	days between and including the first and last dates.
//
//	Declaration:
//	static void GetReviewPeriodAsEntireDb(DATE *pdStart, DATE *pdEnd, unsigned long *pulIntervalInDays)
//
//	Input:	none
//			
//	Output:	pdStart				start time of the review period from the upper layer in DATE
//			pdEnd				end time of the review period from the upper layer in DATE
//			pulIntervalInDays	number of days in the interval from and including end days
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//	08-Aug-2002	SFK		Changed to DATE
//////////////////////////////////////////////////////////////////
static void GetReviewPeriodAsEntireDb(DATE *pdStart, DATE *pdEnd, unsigned long *pulIntervalInDays)
{	
	IDStruct StaID;

	*pulIntervalInDays = 0;
	StaID.sStaID = ANY_STATION;
	StaID.sStaType = ANY_TYPE;
	GetDbTimeRange(StaID, pdStart, pdEnd);
	if (*pdEnd > 0.0) *pulIntervalInDays = (int)ceil(*pdEnd - *pdStart);  // 0.0 signals no data

}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetReviewPeriodFromUpperLayer
//
//	Description:
//	The upper layer can retrieve the review start/end date/time from three different places:
//		from the inspec.ini file
//		from the inspection summary file generated by MIC
//		from the rad.ini file.
//	The upper layer looks for the information to fill the parameters associated with
//	RT_INSPECTION_DATA_DATE_START, RT_INSPECTION_DATA_TIME_START, RT_INSPECTION_DATA_DATE_END,
//	RT_INSPECTION_DATA_TIME_END from the three places above in that order.
//	From the upper layer information, this routine returns the start/end times and interval
//	to be used for the review period.
//
//	Declaration:
//	static void GetReviewPeriodFromUpperLayer(DATE *pdStart, DATE *pdEnd, unsigned long *pulIntervalInDays)
//
//	Input:	none
//			
//	Output:	pdStart 	- start time of the review period from the upper layer in DATE
//			pdEnd		- end time of the review period from the upper layer in DATE
//			pulIntervalInDays - number of days in the interval from and including end days
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//	4-Jun-2002	SFK		Modified when changed Rad timebase to DATE
//////////////////////////////////////////////////////////////////
static void GetReviewPeriodFromUpperLayer(DATE *pdStart, DATE *pdEnd, unsigned long *pulIntervalInDays)
{

	const struct GEN_DATE_STRUCT *pGenStartDate;
	const struct GEN_DATE_STRUCT *pGenEndDate;
	const struct GEN_TIME_STRUCT *pGenStartTime;
	const struct GEN_TIME_STRUCT *pGenEndTime;

	// Get the parameters from the upper layer
	pGenStartDate = GetStartDateFromRADInit();
	pGenEndDate = GetEndDateFromRADInit();
	pGenStartTime = GetStartTimeFromRADInit();
	pGenEndTime = GetEndTimeFromRADInit();

	// convert the Gen structure of the start and end date/time to DATEs
	*pdStart = glMyDateTime.GenStructsToDATETimestamp(pGenStartDate, pGenStartTime);
	*pdEnd = glMyDateTime.GenStructsToDATETimestamp(pGenEndDate, pGenEndTime);
	
	// convert the interval to whole days.
	*pulIntervalInDays = (unsigned long)(ceil(*pdEnd - *pdStart));
}
