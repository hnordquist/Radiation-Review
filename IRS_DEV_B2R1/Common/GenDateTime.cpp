// GenDateTime.cpp: implementation of the CGenDateTime class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support generic time handling routines
//	for legacy NIS-5 time.  Routines are taken as is from the GEN library and renamed
//	to indicate they belong to the class.  For example gen_unpack_time becomes GenUnpackTime.
//
//	Functions:
//		GenDateTimeStrToSeconds
//		GenSecondsToDateTimeStr
//		GenUnpackTime
//		GenUnpackDate
//		GenJulianDaysToStruct
//		GenDateStrToStruct
//		GenDateStructToStr
//		GenTimeStrToStruct
//		GenTimeStructToStr
//		
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GenDateTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_DATE_STR_LEN 12
#define MIN_DATE_STR_LEN 8
#define MAX_TIME_STR_LEN 13

static char gll_month_strings[12][4] = {"Jan", "Feb", "Mar", "Apr", "May",
			"Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
static int gll_days_of_month[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
// This class supports legacy code that requires conversions using
// some of the GEN library routines.
//////////////////////////////////////////////////////////////////////
CGenDateTime::CGenDateTime()
{

}

CGenDateTime::~CGenDateTime()
{

}




/*==============================================================================
 *
 *  Name / File:	GenDateTimeStrToSeconds (old gen_date_time_str_to_seconds)
 *
 *  Purpose:	 converts date and time to seconds from Jan 1 1952
 *
 *  Return value:    GEN_SUCCESS - success calculation
 *		     GEN_INVALID_YEAR - unsuccessful calculation
 *
 *  Author:           Edward A. Kern
 *
 *  Revision History:
 *  date / author          revision
 *  -----------------      --------
 *  11-Jun-1988  EAK	   Created
 *  04-Apr-1990  WCH	   Modified - changed name,
 *			   	      changed base date to Jan 1, 1952,
 *				      changed long to unsigned long.
 *  03-Oct-1990  WCH	   Modified - changed return values
 *  03-Jun-1993  WCH	   Modified - if date = 00.00.00 return 0 time
 *	??-???-????	 SFK	   Made into a CGenDateTime class
 *
==============================================================================*/
int CGenDateTime::GenDateTimeStrToSeconds(unsigned long *ptr_total_seconds, char *date, char *time, int iaea_format)
{
    static int month_day[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int status = GEN_INVALID_YEAR;
    int year_remainder;
    int days_in_year;
    int days_in_month;
    int i;
    unsigned long total_seconds;

    if ((!strncmp (&date[0], "00", 2)) &&
	(!strncmp (&date[3], "00", 2)) &&
	(!strncmp (&date[6], "00", 2)))
    {
	*ptr_total_seconds = 0L;
	status = GEN_SUCCESS;
        return status;
    }
	
    GenUnpackDate(date, &day, &month, &year, iaea_format);
    GenUnpackTime(time, &hour, &minute, &second);
    total_seconds = REF_TIME;
    if (year < 1952)
	year += 100;
    year_remainder =  year - 1952;
    if (year_remainder >= 0) {
	status = GEN_SUCCESS;
	while(year_remainder) {
	    total_seconds +=  SECS_PER_YEAR;
	    if (((year_remainder - 1) % 4)  == 0)
		total_seconds += SECS_PER_DAY;	    /* account for leap year */
	    year_remainder--;
	}
	days_in_year = 0;
	for (i = 0; i < month - 1; i++) {
	    days_in_month = month_day[i];
	    if (i == 1	&&  (year % 4) == 0)
		days_in_month = 29;		     /* February of leap year */
	    days_in_year += days_in_month;
	}
	days_in_year  += day - 1;
	total_seconds += (unsigned long)days_in_year * SECS_PER_DAY;
	total_seconds += (unsigned long)hour * 3600L + minute * 60 + second;
	*ptr_total_seconds = total_seconds;
    }
    return status;
}



/*==============================================================================
 *
 *  Name / File:	GenSecondsToDateTimeStr (old gen_seconds_to_date_time_str)
 *
 *  Purpose:		converts the time long integer into date and time
 *
 *  Return value:	none
 *
 *  Author:           Edward A. Kern
 *
 *  Revision History:
 *  date / author          revision
 *  -----------------      --------
 *  01-Jun-1988  EAK	   Created
 *  04-Apr-1990  WCH	   Modified - changed name,
 *			              changed base date to Jan 1, 1952,
 *				      changed long to unsigned long.
 *  01-Jul-1991  WCH       Modified - use GEN_DTF_US and GEN_DTF_IAEA
 *  12-Aug-1992  WCH	   Modified - added GEN_DTF_WFD format option
 *  03-Jun-1993  WCH	   Modified - if time = 0 return date = 00.00.00
 *	??-???-????	 SFK	   Made into a CGenDateTime class
 *
==============================================================================*/
void CGenDateTime::GenSecondsToDateTimeStr(char *date_str, char *time_str, unsigned long *ptr_long_time, int iaea_date_format)
{

    double dbl_time;		/* input time converted to double days */
    struct GEN_TIME_STRUCT time_int;
    struct GEN_DATE_STRUCT date_int;
    int day_int, month_int, year_int;
    char day_str[3], month_str[3], year_str[3];
    int hour_int, min_int, sec_int;
    char hour_str[3], min_str[3], sec_str[3];

    if (*ptr_long_time == 0L)
    {
	strcpy (time_str, "00:00:00");
        if (iaea_date_format != GEN_DTF_US)
	    strcpy (date_str, "00.00.00");
	else
	    strcpy (date_str, "00/00/00");
	return;
    }

    /* convert long seconds to double days */
    dbl_time = ((double) *ptr_long_time) / SECS_PER_DAY;
    GenJulianDaysToStruct(dbl_time, &date_int, &time_int);
    if (time_int.msec > 500) /* handle millisecond round off */
    {
	dbl_time += MS_ROUND_OFF;	/* add .5 sec and calc again */
        GenJulianDaysToStruct(dbl_time, &date_int, &time_int);
    }
       
/* -----------------------------------------------------------------------------
 *		get the date from the time_int structure
 * ---------------------------------------------------------------------------*/
    day_int   = date_int.day;
    month_int = date_int.mon;
    year_int  = date_int.yr;
    /* keep only the last two digits of the year */
    year_int = year_int % 100;
    _itoa(day_int, day_str, 10);
    _itoa(month_int, month_str, 10);
    _itoa(year_int, year_str, 10);
    if (day_int < 10) {
	day_str[2] = '\0';
	day_str[1] = day_str[0];
	day_str[0] = '0';
    }
    if (month_int < 10) {
	month_str[2] = '\0';
	month_str[1] = month_str[0];
	month_str[0] = '0';
    }
    if (year_int < 10) {
	year_str[2] = '\0';
	year_str[1] = year_str[0];
	year_str[0] = '0';
    }
    if (iaea_date_format == GEN_DTF_WFD) {	/*  yy/mm/dd  -  WFD format  */
	date_str[0] = year_str[0];
	date_str[1] = year_str[1];
	date_str[2] = '/';
	date_str[3] = month_str[0];
	date_str[4] = month_str[1];
	date_str[5] = '/';
	date_str[6] = day_str[0];
	date_str[7] = day_str[1];
	date_str[8] = '\0';
    }
    else if (iaea_date_format != GEN_DTF_US) {	/*  yy.mm.dd  -  IAEA format */
	date_str[0] = year_str[0];
	date_str[1] = year_str[1];
	date_str[2] = '.';
	date_str[3] = month_str[0];
	date_str[4] = month_str[1];
	date_str[5] = '.';
	date_str[6] = day_str[0];
	date_str[7] = day_str[1];
	date_str[8] = '\0';
    }
    else {				/*  mm/dd/yy  -  american format */
	date_str[0] = month_str[0];
	date_str[1] = month_str[1];
	date_str[2] = '/';
	date_str[3] = day_str[0];
	date_str[4] = day_str[1];
	date_str[5] = '/';
	date_str[6] = year_str[0];
	date_str[7] = year_str[1];
	date_str[8] = '\0';
    }
       
/* -----------------------------------------------------------------------------
 *		get the time from the time_int structure
 * ---------------------------------------------------------------------------*/
    hour_int = time_int.hr;
    min_int  = time_int.min;
    sec_int  = time_int.sec;
    _itoa(hour_int, hour_str, 10);
    _itoa(min_int, min_str, 10);
    _itoa(sec_int, sec_str, 10);
    if (hour_int < 10) {
	hour_str[2] = '\0';
	hour_str[1] = hour_str[0];
	hour_str[0] = '0';
    }
    if (min_int < 10) {
	min_str[2] = '\0';
	min_str[1] = min_str[0];
	min_str[0] = '0';
    }
    if (sec_int < 10) {
	sec_str[2] = '\0';
	sec_str[1] = sec_str[0];
	sec_str[0] = '0';
    }
    time_str[0] = hour_str[0];
    time_str[1] = hour_str[1];
    time_str[2] = ':';
    time_str[3] = min_str[0];
    time_str[4] = min_str[1];
    time_str[5] = ':';
    time_str[6] = sec_str[0];
    time_str[7] = sec_str[1];
    time_str[8] = '\0';

    return;
}




/*==============================================================================
 *
 *  Name / File:  GenUnpackTime		(old gen_unpack_time)
 *
 *  Purpose:  unpacks the time string "hh:mm:ss" into the corresponding
 *            int integer hours, minutes, and seconds.
 *
 *  Return value:
 *
 *  Error returns:
 *
 *  Special notes:
 *
 *  Implicit Inputs:
 *
 *  Implicit Outputs:
 *
 *  Calls:
 *
 *           Revision History:
 *  date / author                revision
 *  -------------                --------
 *  02/14/88  Ed Kern            Created
 *	07-Dec-01	SFK				 Moved to CGenDateTime class
 *
============================================================================== */
void CGenDateTime::GenUnpackTime(char *timestr, int *p_hour, int *p_minute, int *p_second)
{
   char buffer[10];
   char buf[3];

   //No strncpy memory overwrite possible here.  The size of the destination buffer
   //is well known as it is defined 2 lines prior.
   //pjm 9-nov-2006
   strncpy(buffer, timestr, 9);
   buffer[9] = '\0';

   /* parse out the hour */
   strcpy(buf, strtok(buffer, "/:"));
   *p_hour = atoi(buf);
   
   /* parse out the minute */
   strcpy(buf, strtok(NULL, "/:"));
   *p_minute = atoi(buf);

   /* parse out the second */
   strcpy(buf, strtok(NULL, "/:"));
   *p_second = atoi(buf);

}


/*==============================================================================
 *
 *  Name / File:  GenUnpackDate()  (old gen_unpack_date)
 *
 *  Purpose:  unpacks the date string "mm/dd/yy" into the corresponding
 *            int integer day, month, and four digit year.  for example
 *            09/24/86 would return day=24, month=9, and year = 1986.
 *
 *  Return value:
 *
 *  Error returns:
 *
 *  Special notes:
 *
 *  Implicit Inputs:
 *
 *  Implicit Outputs:
 *
 *  Calls:
 *
 *           Revision History:
 *  date / author                revision
 *  -------------                --------
 *  02/14/88  Ed Kern            Created
 *  07/01/91  WCH		 Modified - use GEN_DTF_US and GEN_DTF_IAEA
 *  07/07/93  WCH		 Made to work correctly when year reaches 2000
 *  10/23/95  WCH		 Allow .-/ as delimiters for any date format
 *	07-Dec-01 SFK		 Moved to CGenDateTime class
 *
==============================================================================*/
void CGenDateTime::GenUnpackDate(char *datestr, int *p_day, int *p_month, int *p_year, int iaea_date)
{

   char buffer[10];
   char buf[3];
   int i;

   //No strncpy memory overwrite possible here.  The size of the destination buffer
   //is well known as it is defined 3 lines prior.
   //pjm 9-nov-2006
   strncpy(buffer, datestr, 9);
   buffer[9] = '\0';

   if (iaea_date != GEN_DTF_US) {
       /* parse out the year and convert to four_digit year */
       strcpy(buf, strtok(buffer, ".-/"));
       i = atoi (buf);
       if (i < 52)
           *p_year = i + 2000;
       else
           *p_year = i + 1900;

       /* parse out the month */
       strcpy(buf, strtok(NULL, ".-/"));
       *p_month = atoi(buf);

       /* parse out the day */
       strcpy(buf, strtok(NULL, ".-/"));
       *p_day = atoi(buf);
   }
   else {
       /* parse out the month */
       strcpy(buf, strtok(buffer, ".-/"));
       *p_month = atoi(buf);

       /* parse out the day of the month */
       strcpy(buf, strtok(NULL, ".-/"));
       *p_day = atoi(buf);

       /* parse out the year and  convert to four-digit year */
       strcpy(buf, strtok(NULL, ".-/"));
       i = atoi(buf);
       if (i < 52)
           *p_year = i + 2000;
       else
           *p_year = i + 1900;
   }
}



#define BASE_YEAR	1952	/* must be a leap year */
#define BASE_OFFSET_DAYS  (-63.)
#define HALF_MILLISECOND (.5 / (1000. * 60. * 60. * 24.))
/*===========================================================================
 *
 *  Name	 :  GenJulianDaysToStruct (old gen_julian_days_to_struct)
 *
 *  Purpose	 :  Convert julian days to GenStruct format
 *
 *  Return value :  ??
 *
 *  Special notes:
 * Algorithms come from HP-67 Standard Pac manual.

 * Functions originally written by Tom Marks.  In order to incorporate
 * these functions into the gen library, functions were changed to
 * use the gen lib date and time structures by J. Painter, 10/89.  The
 * function names were changed in addition to the argument lists being
 * changed to pass date/time structures instead of individual components
 * of the date and time.  NOTE: Date must greater than
 * BASE_OFFSET_DAYS + BASE_YEAR to get the correct result.
 *
 *  Implicit Input : ??
 *  Implicit Input/Output: ??
 *  Implicit Output: ??

 *  date    /	author	revision
 *  -----------------	--------
 * Changed start date to 1/1/52, WCH 4/5/90 
 * Changed gen_struct_to_julian() to gen_struct_to_julian_days(), WCH 4/5/90 
 * Changed gen_current_julian() to gen_current_julian_days(), WCH 4/5/90 
 * Changed gen_julian_to_struct() to gen_julian_days_to_struct(), WCH 4/5/90 
 * Fixed to work when year reaches 2000, WCH 7/6/93 
 * Added .5 milliseconds to time fraction for roundoff, WCH 1/23/96 
 *
===========================================================================*/
void CGenDateTime::GenJulianDaysToStruct(double double_julian, GEN_DATE_STRUCT *p_date, GEN_TIME_STRUCT *p_time)
{
	int year_num;
	int month_num;
	int day_num;
	int iyrprm;
	int mnthprm;
	long long_julian;
	double time_frac;
	
	double_julian = double_julian - BASE_OFFSET_DAYS;
	long_julian = (long) double_julian;
	/* add .5 milliseconds for roundoff */
	time_frac = double_julian - long_julian + HALF_MILLISECOND;
	
	iyrprm = (int)((long_julian - 122.1) / 365.25);
	mnthprm = (int)((long_julian - (long)(365.25 * iyrprm)) / 30.6001);
	day_num = long_julian - (int)(365.25 * iyrprm);
	day_num = day_num - (int)(30.6001 * mnthprm);
	
	if (mnthprm < 14) month_num = mnthprm - 1;
	else month_num = mnthprm - 13;
	
	if (month_num > 2) year_num = BASE_YEAR + iyrprm;
	else year_num = BASE_YEAR + iyrprm + 1;

	p_date->yr = year_num;
	p_date->mon = month_num;
	p_date->day = day_num;

	p_time->hr = (int) (time_frac * 24.0);

	time_frac = time_frac - (double)(p_time->hr) / 24.0;
	p_time->min = (int) (time_frac * 60.0 * 24.0);

	time_frac = time_frac - (double)(p_time->min) / 24.0 / 60.0;
	p_time->sec = (int) (time_frac * 60.0 * 60.0 * 24.0);

	time_frac = time_frac - (double)(p_time->sec) / 24.0 / 60.0 / 60.0;
	p_time->msec = (int) (time_frac * 60.0 * 60.0 * 24.0 * 1000.0);

	if (p_time->msec >= 1000)
	{
	    if ((p_time->hr == 23) && (p_time->min == 59) &&
						(p_time->sec == 59))
	    {
			p_time->msec = 999;
	    }
	    else
	    {
	        p_time->sec += 1;
	        p_time->msec -= 1000;
	        if (p_time->sec >= 60)
	        {
				p_time->min += 1;
				p_time->sec -= 60;
				if (p_time->min >= 60)
				{
					p_time->hr += 1;
					p_time->min -= 60;
				}
			}
		}
	}

	return;
}


/*===========================================================================
 *
 *  Name	 :  GenValidateDate (old gen_validate_date)
 *
 *  Purpose	 :  Check that the numbers in the date structure are valid.
 *
 *  Return value :  GEN_SUCCESS - date is okay
 *		    GEN_INVALID_DATE - date is out of range
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  25-Aug-1989 SFK	Created
 *	??-???-????	 SFK	   Made into a CGenDateTime class
 *
===========================================================================*/
int GenValidateDate(
    struct GEN_DATE_STRUCT *ptr_date) /* input - date to be checked */

{
    int status;

    status = GEN_SUCCESS;
   /* ------------------------------------------------------------------
    *	Check that the converted date is valid.  That is
    *	    year - must be greater than or equal to 0
    *	    mon  - must be between 1 and 12
    *	    day  - must be greater than 1 and checked against days in month,
    *		   leap year considered by the &3
    * ----------------------------------------------------------------*/
    if (ptr_date->yr < 0)
	status = GEN_INVALID_DATE;

    if ((ptr_date->mon < 1) || (ptr_date->mon > 12))
	return (GEN_INVALID_DATE);

    if ((ptr_date->day < 1) || (ptr_date->day > gll_days_of_month[ptr_date->mon])) {
	status = GEN_INVALID_DATE;
	if ((ptr_date->day == 29) && (ptr_date->mon == 2) && ((ptr_date->yr & 3) == 0)) {
	    status = GEN_SUCCESS;
	}
    }
    return(status);
}


/*===========================================================================
 *
 *  Name	 : GenDateStrToStruct (old gen_date_str_to_struct)
 *
 *  Purpose	 : Converts a date string in one of the specified formats
 *			   to the GEN_DATE_STRUCT format.  The values in the structure
 *			   are validated.
 *
 *  Return value :
 *
 *  Special notes: These date formats are supported :
 *			    GEN_DTF_DEC	    dd-mon-yyyy  16-AUG-1989
 *				 or dd-mon-yy	 16-AUG-89
 *				    (The DEC year is normally XXXX but
 *				     if < 100 and >= 52 then 1900 is added or
 *				     if < 52 then 2000 is added.)
 *
 *			    GEN_DTF_US	    mm-dd-yy	 08-16-89
 *			    GEN_DTF_EURO     dd-mm-yy	 16-08-89
 *			    GEN_DTF_IAEA     yy.mm.dd	 89.08.16
 *			   These assumptions are made :
 *			    1.	a field composed of blanks is assumed to be a zero
 *				value for year only.  Fields composed only of blanks
 *				for month and day will return an error.
 *			    2.	separaters are assumed to be "-","/" or "." for all
 *				formats.
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  25-Aug-1989 SFK	Created
 *  05-Oct-1989 JAP	Added a check on the year for the DEC format to
 *			allow users to input only 2 digits (e.g., 89)
 *			and to add 1900 to the year if this condition
 *			(i.e., < 100) exists.
 *  07-Jun-1993 WCH	Made to work when year reaches 2000.
 *	??-???-????	 SFK	   Made into a CGenDateTime class
 *
===========================================================================*/
int CGenDateTime::GenDateStrToStruct(
    char *date_str,	/* input - ascii string containing the date */
    int  date_format,	/* input - type of date format */
    struct GEN_DATE_STRUCT *ptr_date)	/* output - int values for date components */
{

    char *yr_str;
    char *mon_str;
    char *day_str;
    char token_str[MAX_DATE_STR_LEN];
    char *ptr_token_str;
    int  status;
    int  i;

       
   /* ------------------------------------------------------------------
    *	strtok is going to change the input string so make a copy and use it.
    * ----------------------------------------------------------------*/
    if (strlen(date_str) > MAX_DATE_STR_LEN) {
	status = GEN_INVALID_DATE;
    }
    else if (strlen(date_str) < MIN_DATE_STR_LEN) {
	status = GEN_INVALID_DATE;
    }
    else {
	strcpy(token_str,date_str);
	ptr_token_str = token_str;

	status = GEN_SUCCESS;
	switch(date_format) {

	    case GEN_DTF_DEC :
		day_str = strtok(ptr_token_str,"-/.");
		mon_str = strtok(NULL,"-/.");
		yr_str	= strtok(NULL,"-/.");
		mon_str[0] = toupper(mon_str[0]);
		mon_str[1] = tolower(mon_str[1]);
		mon_str[2] = tolower(mon_str[2]);
		ptr_date->mon = 0;
		for (i=0; i<12; i++) {
		    if (strcmp(mon_str,gll_month_strings[i]) == 0)
			ptr_date->mon = i + 1;
		}

		break;

	    case GEN_DTF_WFD:
	    case GEN_DTF_US  :
		mon_str = strtok(ptr_token_str,"-/.");
		day_str = strtok(NULL,"-/.");
		yr_str = strtok(NULL,"-/.");
		break;

	    case GEN_DTF_EURO :
		day_str = strtok(ptr_token_str,"-/.");
		mon_str = strtok(NULL,"-/.");
		yr_str = strtok(NULL,"-/.");
		break;

	    case GEN_DTF_IAEA :
		yr_str = strtok(ptr_token_str,"-/.");
		mon_str = strtok(NULL,"-/.");
		day_str = strtok(NULL,"-/.");
		break;

	    default:
		status = GEN_INVALID_DATE_FORMAT;
		break;
	}

	if (status == GEN_SUCCESS)
	{
	    if ((strlen (yr_str)) < 2)
	        status = GEN_INVALID_DATE_FORMAT;
	    if ((strlen (mon_str)) < 2)
	        status = GEN_INVALID_DATE_FORMAT;
	    if ((strlen (day_str)) < 2)
	        status = GEN_INVALID_DATE_FORMAT;
	}

	if (status == GEN_SUCCESS)
	{
	    ptr_date->yr = atoi(yr_str);
	    if (ptr_date->yr < 0)
	        status = GEN_INVALID_DATE_FORMAT;
	}

	if (status == GEN_SUCCESS)
	{

	   /* ------------------------------------------------------------------
	    *	Convert the parsed strings to integers
	    * ----------------------------------------------------------------*/
	    if ((ptr_date->yr < 100) && (ptr_date->yr >= 52) &&
						(date_format == GEN_DTF_DEC))
	    {
		ptr_date->yr = ptr_date->yr + 1900;
	    }
	    else if ((ptr_date->yr < 52) && (date_format == GEN_DTF_DEC))
	    {
		ptr_date->yr = ptr_date->yr + 2000;
	    }

	    /* ptr_date->mon for the DEC format is handled above in the case */
	    if (date_format != GEN_DTF_DEC)
		ptr_date->mon = atoi(mon_str);

	    ptr_date->day = atoi(day_str);

	    status = GenValidateDate(ptr_date);
	}
    }
       
   /* ------------------------------------------------------------------
    *	If have error, return 0s for date
    * ----------------------------------------------------------------*/
    if (status != GEN_SUCCESS)
    {
	ptr_date->yr = 0;
	ptr_date->mon = 0;
	ptr_date->day = 0;
    }
    return(status);

}


/*===========================================================================
 *
 *  Name	 :  GenDateStructToStr (old gen_date_struct_to_str)
 *
 *  Purpose	 :  Checks that the date in the structure is valid, then if
 *		    valid, converts it to a string in the specified date format.
 *
 *  Return value :  GEN_SUCCESS
 *				    GEN_INVALID_DATE_FORMAT
 *		
 *  Special notes: These date formats are supported :
 *				    GEN_DTF_DEC	     dd-mon-yyyy 16-AUG-1989
 *				    GEN_DTF_US	     mm-dd-yy	 08-16-89
 *				    GEN_DTF_EURO     dd-mm-yy	 16-08-89
 *				    GEN_DTF_IAEA     yy.mm.dd	 89.08.16
 *				    GEN_DTF_WFD	     mm/dd/yy	 08/16/89
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  25-Aug-1989 SFK	Created
 *	??-???-????	 SFK	   Made into a CGenDateTime class
===========================================================================*/
int CGenDateTime::GenDateStructToStr(
    struct GEN_DATE_STRUCT date, /* input - date to convert to string */
    int format, 		/* input - type of date format */
    char **ptr_date_str)	/* output - ptr to string containing date */

{
    // sk 7/15/2003
	static char date_string[MAX_DATE_STR_LEN];
    char *month_str;
    int status;

    status = GenValidateDate(&date);


    /* ------------------------------------------------
     *	the only date format that allows for a year to
     *	be more than 2 digits is the DEC format which
     *	allows either 2 digits or 4 digits.  Therefore
     *	all years, except for DEC format, will be forced
     *	to 2 digits by subtracting 1900 or 2000 if the year is
     *	greater than 100.
     * ------------------------------------------------ */

    if (format != GEN_DTF_DEC)
    {
		if (date.yr >= 2000)
		{
			date.yr = date.yr - 2000;
		}
		else if (date.yr >= 1900)
		{
			date.yr = date.yr - 1900;
		}
    }

    if (status == GEN_SUCCESS) {

		switch(format) {

			case GEN_DTF_DEC :
			month_str = gll_month_strings[date.mon - 1];
			sprintf(date_string,"%02d-%s-%04d",date.day,month_str,date.yr);
			break;

			case GEN_DTF_WFD:
			sprintf(date_string,"%02d/%02d/%02d",date.mon,date.day,date.yr);
			break;
			case GEN_DTF_US :
			sprintf(date_string,"%02d-%02d-%02d",date.mon,date.day,date.yr);
			break;

			case GEN_DTF_EURO :
			sprintf(date_string,"%02d-%02d-%02d",date.day,date.mon,date.yr);
			break;

			case GEN_DTF_IAEA :
			sprintf(date_string,"%02d.%02d.%02d",date.yr,date.mon,date.day);
			break;

			default :
			status = GEN_INVALID_DATE_FORMAT;
			break;
		}
    }

    if (status != GEN_SUCCESS) {
	strcpy(date_string,"INVALID");
    }

    *ptr_date_str = date_string;

    return(status);
}


/*===========================================================================
 *
 *  Name	 :  gen_validate_time
 *
 *  Purpose	 :  Check that the numbers in the time structure are valid.
 *
 *  Return value :  GEN_SUCCESS
 *				    GEN_INVALID_TIME - time is out of range
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  25-Aug-1989 SFK		Created
 *	??-???-????	 SFK	   Made into a CGenDateTime class
 ===========================================================================*/
int CGenDateTime::GenValidateTime(
    struct GEN_TIME_STRUCT *ptr_time) /* input - time to be checked */
{
    int status;

    status = GEN_SUCCESS;
   /* ------------------------------------------------------------------
    *	Check that the time is valid.  That is
    *	    hr	- must be between 0 and 23
    *	    min - must be between 0 and 59
    *	    sec - must be between 0 and 59
    *	   msec - must be between 0 and 999
    * ----------------------------------------------------------------*/
    if ((ptr_time->hr < 0)  || (ptr_time->hr >	23))
	status = GEN_INVALID_TIME;

    if ((ptr_time->min < 0) || (ptr_time->min > 59))
	status = GEN_INVALID_TIME;

    if ((ptr_time->sec < 0) || (ptr_time->sec > 59))
	status = GEN_INVALID_TIME;

    if ((ptr_time->msec < 0) || (ptr_time->msec > 999))
	status = GEN_INVALID_TIME;

    return(status);
}


/*===========================================================================
 *
 *  Name	 : GenTimeStrToStruct (old gen_time_str_to_struct)
 *
 *  Purpose	 : Converts a time string in one of the specified formats
 *			  to the GEN_TIME_STRUCT format.  The values in the structure
 *			  are validated.
 *
 *  Return value :
 *
 *  Special notes: These time formats are supported :
 *				   GEN_DTF_HM	    hh:mm
 *				    GEN_DTF_HMS	    hh:mm:ss
 *				    GEN_DTF_HMSM     hh:mm:ss.xxx
 *				   These assumptions are made :
 *				    1.	a field composed of blanks is assumed to be a zero.
 *				    2.	separaters are assumed to be ":" for all formats
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  25-Aug-1989 SFK	Created
===========================================================================*/
int CGenDateTime::GenTimeStrToStruct(
    char *time_str,	/* input - ascii string containing the time */
    int  time_format,	/* input - type of time format */
    struct GEN_TIME_STRUCT *ptr_time)	/* output - int values for time components */
{

    char *hr_str;
    char *min_str;
    char *sec_str;
    char *msec_str;
    char token_str[MAX_TIME_STR_LEN];
    char *ptr_token_str;
    int  status;

       
   /* ------------------------------------------------------------------
    *	strtok is going to change the input string so make a copy and use it.
    * ----------------------------------------------------------------*/
    if (strlen(time_str) > MAX_TIME_STR_LEN) {
	status = GEN_INVALID_TIME;
    }
    else {
	strcpy(token_str,time_str);
	ptr_token_str = token_str;

	status = GEN_SUCCESS;
	switch(time_format) {

	    case GEN_DTF_HM :
		hr_str	= strtok(ptr_token_str,":");
		min_str = strtok(NULL,":");
		break;

	    case GEN_DTF_HMS  :
		hr_str	= strtok(ptr_token_str,":");
		min_str = strtok(NULL,":");
		sec_str = strtok(NULL,":");
		break;

	    case GEN_DTF_HMSM :
		hr_str	= strtok(ptr_token_str,":");
		min_str = strtok(NULL,":");
		sec_str = strtok(NULL,".");
		msec_str = strtok(NULL,".");
		break;

	    default:
		status = GEN_INVALID_TIME_FORMAT;
		break;
	}

	if (status == GEN_SUCCESS) {

	   /* ------------------------------------------------------------------
	    *	Convert the parsed strings to integers
	    * ----------------------------------------------------------------*/
	    if (time_format == GEN_DTF_HMSM) {
		ptr_time->msec = atoi(msec_str);
	    }
	    else {
		ptr_time->msec = 0;
	    }
	    if ((time_format == GEN_DTF_HMSM) || (time_format == GEN_DTF_HMS)) {
		ptr_time->sec = atoi(sec_str);
	    }
	    else {
		ptr_time->sec = 0;
	    }
	    ptr_time->min = atoi(min_str);
	    ptr_time->hr = atoi(hr_str);

	    status = GenValidateTime(ptr_time);
	}
    }
       
   /* ------------------------------------------------------------------
    *	If have error, return 0s for time
    * ----------------------------------------------------------------*/
    if (status != GEN_SUCCESS) {
	ptr_time->hr = 0;
	ptr_time->min = 0;
	ptr_time->sec = 0;
	ptr_time->msec = 0;
    }
    return(status);
}


/*===========================================================================
 *
 *  Name	 :  GenTimeStructToStr (old gen_time_struct_to_str)
 *
 *  Purpose	 :  Checks that the time in the structure is valid, then if
 *			    valid, converts it to a string in the specified time format.
 *
 *  Return value :  GEN_SUCCESS
 *				    GEN_INVALID_TIME_FORMAT
 *
 *  Special notes: These time formats are supported :
 *					GEN_DTF_HM	    hh:mm
 *					GEN_DTF_HMS	    hh:mm:ss
 *					GEN_DTF_HMSM     hh:mm:ss.xxx
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  25-Aug-1989 SFK	Created
 *	??-???-????	 SFK	   Made into a CGenDateTime class
===========================================================================*/
int CGenDateTime::GenTimeStructToStr(
    struct GEN_TIME_STRUCT time, /* input - time to convert to string */
    int format, 		/* input - type of time format */
    char **ptr_time_str)	/* output - ptr to string containing time */

{
    static char time_string[MAX_TIME_STR_LEN];
    int status;

    status = GenValidateTime(&time);

    if (status == GEN_SUCCESS) {

	switch(format) {

	    case GEN_DTF_HM :
		sprintf(time_string,"%02d:%02d",time.hr,time.min);
		break;

	    case GEN_DTF_HMS :
		sprintf(time_string,"%02d:%02d:%02d",time.hr,time.min,time.sec);
		break;

	    case GEN_DTF_HMSM :
		sprintf(time_string,"%02d:%02d:%02d.%03d",time.hr,time.min,time.sec,time.msec);
		break;

	    default :
		status = GEN_INVALID_TIME_FORMAT;
		break;
	}
    }

    if (status != GEN_SUCCESS) {
	strcpy(time_string,"INVALID");
    }

    *ptr_time_str = time_string;

    return(status);
}
