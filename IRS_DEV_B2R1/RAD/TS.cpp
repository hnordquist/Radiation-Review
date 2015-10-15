//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: TS.cpp $
// 
// *****************  Version 2  *****************
// User: Longo        Date: 10/18/04   Time: 3:29p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// more comments added to this RAD to FDMS exchnage mechanism
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// This file contains the class implementation of the class "TS"
//
// TS is used to convert a char[23] time stamp into a
// COleDateTime object
// The char[23] string is of the form sssssYYYYMMDDHHMMSSmmm
// where:
//   sssss      Station number (not used, always 0)
//   YYYYMMDD   Date
//   HHMMSS     Time
//   mmm        milliseconds (always 0)
//////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include <afxdisp.h>
#include ".\TS.h"

#define tsDATE_LENGTH 9	// maximum length of a date subsection source string, including trailing null
#define tsTIME_LENGTH 10	// maximum length of a time subsection source string, including trailing null
#define tsDATE_START 5			// index of start of date component of string: YYYYMMDD
#define tsTIME_START 13			// index of start of date component of string: HHMMSSmmm
#define tsTIME_STAMP_LENGTH 23	// maximum length of a wacky time stamp, including trailing null


///////////////////////////////////////////////////////
// TS::TS
// TS::TS
//
// Constructors 
// 
// Input:
//	str		pointer to string representation of time
///////////////////////////////////////////////////////
TS::TS(void)
{
}
TS::TS(char* ts)
{
	Convert(ts);
}

TS::~TS(void)
{
}

///////////////////////////////////////////////////////
// TS::Convert
//
// Transforms the instance into a COleDateTime representation
// of the timestamp string parameter
// 
// Input:
//	str		pointer to string representation of time
///////////////////////////////////////////////////////
void TS::Convert(LPCTSTR str)
{
	char _ts[tsTIME_STAMP_LENGTH];
    char dval[tsDATE_LENGTH];
    char tval[tsTIME_LENGTH];
	int year, mon, day, hr, min, sec;

	year = 0;	mon = 0;	day = 0;	hr = 0;	min = 0;	sec = 0;
	ZeroMemory(_ts, sizeof(_ts));

	strncpy(_ts, str, tsTIME_STAMP_LENGTH);

    strncpy (dval, &_ts[tsDATE_START], tsDATE_LENGTH);
    dval[tsDATE_LENGTH-1] = '\0';
    convert_from_yyyymmdd (dval, year, mon, day);	

	strncpy (tval, &_ts[tsTIME_START], tsTIME_LENGTH);
    tval[tsTIME_LENGTH-1] = '\0';
	convert_from_hhmmss (tval, hr, min, sec);

	SetDateTime(year, mon, day, hr, min, sec);
}

///////////////////////////////////////////////////////
// TS::convert_from_yyyymmdd
//
// Converts the year, month and day of the input string to integer values
// 
// Input:
//	input_string		pointer to string representation of time
//					(4 digit year, 2 digit month, 2 digit day)
// Output:
//	year, mon, day: traditional integer values for the named fields
//
///////////////////////////////////////////////////////
bool TS::convert_from_yyyymmdd (char * input_string, int& year, int& mon, int& day)
{

    char year_string[5];
    char month_string[3];
    char day_string[3];

    /* check for a valid year */
    strncpy (year_string, input_string, 4);
    year_string[4] = '\0';
    year = atoi (year_string);
    if ((year < 1952) || (year > 2051))
    {
		return (false);
    }
    /* check for a valid month */
    strncpy (month_string, &input_string[4], 2);
    month_string[2] = '\0';
    mon = atoi (month_string);
    if ((mon < 1) || (mon > 12))
    {
		return (false);
    }
    /* check for a valid day */
    strncpy (day_string, &input_string[6], 2);
    day_string[2] = '\0';
    day = atoi (day_string);
    if ((day < 1) || (day > 31))
    {
		return (false);
    }
    return true;
}

///////////////////////////////////////////////////////
// TS::convert_from_hhmmss
//
// Converts the hour, minute and seconds of the input string to integer values
// 
// Input:
//	input_string		pointer to string representation of time
//					(2 digit hour, 2 digit minutes, 2 digit seconds)
// Output:
//	hr, min, sec: traditional integer values for the named fields
//
///////////////////////////////////////////////////////
bool TS::convert_from_hhmmss (char * input_string,  int& hr, int& min, int& sec)
{

    char hour_string[3];
    char min_string[3];
    char sec_string[3];

    /* check for a valid year */
    strncpy (hour_string, input_string, 2);
    hour_string[2] = '\0';
    hr = atoi (hour_string);
    if ((hr < 0) || (hr > 24))
    {
		return (false);
    }
    strncpy (min_string, &input_string[2], 2);
    min_string[2] = '\0';
    min = atoi (min_string);
    if ((min < 0) || (min > 59))
    {
		return (false);
    }
    strncpy (sec_string, &input_string[4], 2);
    sec_string[2] = '\0';
    sec = atoi (sec_string);
    if ((sec < 0) || (sec > 59))
    {
		return (false);
    }
    return true;
}


