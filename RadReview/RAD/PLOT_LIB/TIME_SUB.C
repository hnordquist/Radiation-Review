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
/* ======================================================================= */
/* ============================== time_sub.c ============================= */
/* ======================================================================= */

/*---------------------------------------------------------------------------
*                         SPECIAL NOTE TO DEVELOPERS
*
*		Information from this file is extracted to create portions of
*		WMCA_UM.DOC.  Because of this, the appearance of that document
*		is highly dependent on the formatting of information in this file.
*		Thus, it is recommended to remain consistent with existing format
*		conventions in this file.  Please observe the following:
*		o	keep the use of tabs and spaces consistent with existing usage
*			in this file
*		o	set tab width to 4
*		o	use "preserve tabs"
*		o	keep extracted lines shorter than 80 characters
*--------------------------------------------------------------------------*/

/*+/mod----------------------------------------------------------------------
* time_sub.c - date and time library routines
*
* Purpose:
*		The routines in this module allow easily working with time and
*		date information.  The features include:
*		o	Conversions to text can use the "international" information
*		    stored by Windows, thus allowing the end user--rather than
*		    the application developer--to control the formatting of output
*		    text.
*		o	The caller can control the conversion to text using format
*		    strings.
*		o	The routines support working with the current time, stored
*		    times, and delta times.
*		o	Elapsed time timers are provided.
*		o	The current time can be displayed on a window's title bar.
*
* Quick reference:
*       #include <time_sub.h>
*           TIME_TIMER timer;   // timer structure
*           int   iYear;        // 4 digit year
*           int   iMonth;       // month (1 to 12)
*           int   iDay;         // day of month (1 to 31)
*           int   iHour;        // hour of day (0 to 23)
*           char  *szFormat="string" // "short hhnnss", etc., or custom format
*                               // (see TimeLSecToSz for more details)
*
*  void  TimeLSecToParts       (lEpochSec, >piYear, >piMonth, >piDay,
*                                   >piHour, >piMinute, >piSecond)
*  char *TimeLSecToSz          (szFormat, lSec, bufDim, >szBuf)
*  long  TimeNowAsLSec         ()
*  void  TimeNowInParts        (>piYear, >piMonth, >piDay,
*                                   >piHour, >piMinute, >piSecond)
*  char *TimeNowToSz           (szFormat, bufDim, >szBuf)
*  void  TimeNowToTitleBar     (hwWin, bShowSec, bLongDate)
*  long  TimePartsToLSec       (iYear, iMonth, iDay, iHour, iMinute, iSecond)
*  char *TimePartsToSz         (szFormat, iYear, iMonth, iDay,
*                                   iHour, iMinute, iSecond, bufDim, >szBuf)
*  void  TimeSetSystemTime     ()
*  long  TimeTimerCheck        (<>pTimer)
*  void  TimeTimerReset        (<>pTimer, lSec)
*  void  TimeTimerStart        (<>pTimer, lSec)
*   int  TimeToTitleBar        (hwWin, bKill, bShowSec, bLongDate)
*
* Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
* 10-09-95 R. Cole		added timer routines
* 01-20-96 R. Cole		added TimeNowToTitleBar
*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
* Copyright 1994-97, The Regents Of the University of California.  This
* program was prepared by the Regents of the University of California at
* Los Alamos National Laboratory (the University) under Contract No. W-7405-
* ENG-36 with the U.S. Department of Energy (DOE).  The University has
* certain rights in the program pursuant to the contract and the program
* should not be copied or distributed outside your organization.  All rights
* in the program are reserved by the DOE and the University.  Neither the
* U.S. Government nor the University makes any warranty express or implied,
* or assumes any liability or responsibility for the use of this software.
*
* This software was produced by the Safeguards Science and Technology
* Group (NIS-5).
*--------------------------------------------------------------------------*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "time_sub.h"
#include "wu.h"

#if defined WIN32
#define EXPORT16
#define EXPORT32 __declspec(dllexport)
#define FAR16
#else
#define EXPORT16 __export
#define EXPORT32
#define FAR16 __far
#endif

EXPORT32 void EXPORT16 FAR16 PASCAL
TimeToTitleBar_cbk(
HWND	hwWin,
UINT	wMsg,
UINT	myID,
DWORD	dwTime);

static char *gaszDayWeek[]={"Sunday", "Monday", "Tuesday", "Wednesday",
	"Thursday", "Friday", "Saturday"};
static char *gaszMonth[]={"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"};

void
SzToSz(
char	**ppOut,
char	*pIn);

void
TmTimeToSz(
char	**ppOut,
struct tm *pTime,
int		bShowSec,
int		bHideAmPm);

/*+/csubr/TOC/internal-------------------------------------------------------
* main - test time_sub.c
*
* Purpose:
*		Test the various functions implemented in time_sub.c.
*
* Return Value:
*		0
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
*--------------------------------------------------------------------------*/
#if defined TIME_TEST // {
int
main()
{
	char	szBuf[100];
	long	lEpochSec;
	int		iYear, iMonth, iDay, iHour, iMin, iSec;

	TimeNowToSz("hhnn", 100, szBuf);
	printf("%s\n", szBuf);
	TimeNowToSz("hhnnss", 100, szBuf);
	printf("%s\n", szBuf);
	TimeNowInParts(&iYear, &iMonth, &iDay, &iHour, &iMin, &iSec);
	printf("%ld\n", lEpochSec);
	printf("%02d/%02d/%02d %02d:%02d:%02d\n",
				iYear, iMonth, iDay, iHour, iMin, iSec);
	TimeLSecToSz("hhnn", lEpochSec, 100, szBuf);
	printf("%s\n", szBuf);
	TimeLSecToSz("hhnnss", lEpochSec, 100, szBuf);
	printf("%s\n", szBuf);
	lEpochSec = TimePartsToLSec(iYear, iMonth, iDay, iHour, iMin, iSec);
	printf("%ld\n", lEpochSec);
	TimeLSecToSz("hhnnss", lEpochSec, 100, szBuf);
	printf("%s\n", szBuf);

	TimeLSecToSz("short", lEpochSec, 100, szBuf);
	printf("short: %s\n", szBuf);
	TimeLSecToSz("long", lEpochSec, 100, szBuf);
	printf("long: %s\n", szBuf);
	TimeLSecToSz("mon d, yyyy", lEpochSec, 100, szBuf);
	printf("mon d, yyyy: %s\n", szBuf);
	TimeLSecToSz("month d, yyyy", lEpochSec, 100, szBuf);
	printf("month d, yyyy: %s\n", szBuf);
	TimeLSecToSz("mm/dd/yy hh:nn:ss", lEpochSec, 100, szBuf);
	printf("mm/dd/yy hh:nn:ss: %s\n", szBuf);
	TimeLSecToSz("mm/dd/yy HH:nn:ss", lEpochSec, 100, szBuf);
	printf("mm/dd/yy HH:nn:ss: %s\n", szBuf);
	TimeLSecToSz("short hhnnss", lEpochSec, 100, szBuf);
	printf("short hhnnss: %s\n", szBuf);

	return 0;
}
#endif // defined TIME_TEST }
/*+/csubr/TOC/internal-------------------------------------------------------
* IntToSz2 - convert an integer to 2 character text
*
* Purpose:
*		Converts an integer into a 2 character text field, with optional
*		leading zero.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
*--------------------------------------------------------------------------*/
void
IntToSz2(
char	**ppOut,
int		iVal,
int		bLeadZero)
{
	char	szBuf[10];
	int		i;

	szBuf[0] = '0';		// leading zero
	itoa(iVal, &szBuf[1], 10);
	if (bLeadZero && iVal < 10)
		i = 0;
	else
		i = 1;
	while (szBuf[i] != '\0') {
		**ppOut = szBuf[i++];
		(*ppOut)++;
	}
}
/*+/csubr/TOC/internal-------------------------------------------------------
* IntToSz4 - convert an integer to 4 character text
*
* Purpose:
*		Converts an integer into a 4 character text field, with optional
*		leading zero.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
*--------------------------------------------------------------------------*/
void
IntToSz4(
char	**ppOut,
int		iVal,
int		bLeadZero)
{
	char	szBuf[10];
	int		i;

	strcpy(szBuf, "0000");		// leading zeros
	itoa(iVal, &szBuf[3], 10);
	if (bLeadZero && iVal < 10)				i = 0;
	else if (bLeadZero && iVal < 100)		i = 1;
	else if (bLeadZero && iVal < 1000)		i = 2;
	else									i = 3;
	while (szBuf[i] != '\0') {
		**ppOut = szBuf[i++];
		(*ppOut)++;
	}
}
/*+/csubr/TOC/internal-------------------------------------------------------
* SzToSz - copy a text string
*
* Purpose:
*		Copies a text string from input to output, updating the output
*		pointer.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
*--------------------------------------------------------------------------*/
void
SzToSz(
char	**ppOut,
char	*pIn)
{
	while (*pIn != '\0') {
		**ppOut = *(pIn++);
		(*ppOut)++;
	}
}
/*+/csubr/TOC/internal-------------------------------------------------------
* SzToSzSQ - copy a text string enclosed in single quotes
*
* Purpose:
*		Copies a text string from input to output, updating the output
*		and input pointers.
*
*		The input string is assumed to have been enclosed in single
*		quotes.  Further, it is assumed that the leading single quote
*		has already been removed.
*
*		Two successive embedded single quote characters are treated
*		as a single quote that is copied to the output.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
*--------------------------------------------------------------------------*/
void
SzToSzSQ(
char	**ppOut,
char	**ppIn)
{
	while (**ppIn != '\0') {
		if (**ppIn != '\'') {
			**ppOut = **ppIn;
			(*ppOut)++;
			(*ppIn)++;
		}
		else {
			(*ppIn)++;
			if (**ppIn == '\'') {
				**ppOut = **ppIn;
				(*ppOut)++;
				(*ppIn)++;
			}
			else
				break;
		}
	}
}
/*+/csubr/TOC/internal-------------------------------------------------------
* SzToSz3 - copy a 3 character text string
*
* Purpose:
*		Copies a text string from input to output, updating the output
*		pointer.  Only the first 3 characters of the input string are
*		copied.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
*--------------------------------------------------------------------------*/
void
SzToSz3(
char	**ppOut,
char	*pIn)
{
	int		i;

	for (i=0; i<3; i++) {
		**ppOut = *(pIn++);
		(*ppOut)++;
	}
}
/*+/csubr/TOC----------------------------------------------------------------
* TimeLSecToParts - expand time in seconds into its parts
*
* Purpose:
*		Expands a time (expressed as the number of seconds past the
*		Windows epoch) into its component parts.
*
* Return value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 05-18-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
TimeLSecToParts(
long	lEpochSec,		// I sec since Windows epoch start
int		*piYear,		// O place to put year (yyyy), or NULL
int		*piMonth,		// O place to put month (1-12), or NULL
int		*piDay,			// O place to put day (1-31), or NULL
int		*piHour,		// O place to put hour (0-23), or NULL
int		*piMinute,		// O place to put minute (0-59), or NULL
int		*piSecond)		// O place to put second (0-59), or NULL
{
	struct tm *pTime;

	pTime = localtime(&lEpochSec);
	if (piSecond != NULL) *piSecond = pTime->tm_sec;
	if (piMinute != NULL) *piMinute = pTime->tm_min;
	if (piHour != NULL) *piHour = pTime->tm_hour;
	if (piDay != NULL) *piDay = pTime->tm_mday;
	if (piMonth != NULL) *piMonth = pTime->tm_mon+1;
	if (piYear != NULL) *piYear = pTime->tm_year+1900;
}
/*+/csubr/TOC----------------------------------------------------------------
* TimeLSecToSz - convert 'long' time to text
*
* Purpose:
*		Converts a time represented as a 'long' to text, using either the
*		format from WIN.INI or a caller specified format.
*
*		The time can represent either an actual date, or a "delta time".
*		(In the latter case, specifying formats for year, month, and day
*		conversions doesn't cause an error but is probably non-sensical.)
*
*		The format string consists of "keywords" interspersed with "literal
*		text".  The "literal text" is enclosed in ' characters.  To include
*		a ' character in the literal text, use two ' characters.  For
*		example, "HH''''mm" would produce "23'57".
*
*		When a "keyword" indicates that it uses WIN.INI, it means that the
*		conversion uses the Control Panel "international" settings.
*
*		The "keywords" that are recognized are:
*			"long" converts the date, with the long WIN.INI date format.
*			"short" converts the date, with the short WIN.INI date format.
*			"yy" and "yyyy" convert the year as either 2 or 4 digits, with
*				leading zeros.
*			"m" and "mm" convert the month.  "mm" gives leading zeros.
*			"M" and "MM" are equivalent to "m" and "mm".
*			"mon" and "month" convert the month, with either a 3 character
*				abbreviation or fully spelled out.
*			"MMM" and "MMMM" are equivalent to "mon" and "month".
*			"d" and "dd" convert the day of the month.  "dd" gives leading
*				zeros.
*			"day" and "dayWeek" convert the day of the week as either a 3
*				character abbreviation or fully spelled out.
*			"ddd" and "dddd" are equivalent to "day" and "dayWeek".
*			"hhnn" converts hours and minutes, using WIN.INI.
*			"hhnnss" converts hours, minutes, and seconds, using WIN.INI.
*			"H" and "HH" convert the hour, based on a 24 hour clock.  "HH"
*				gives leading zeros.
*			"h" and "hh" convert the hour, based on a 12 hour clock.  "hh"
*				gives leading zeros.
*			"n" and "nn" convert the minute.  "nn" gives leading zeros.
*			"s" and "ss" convert the second.  "ss" gives leading zeros.
*
* Return Value:
*		pointer to formatted text
*
* Implicit Inputs:
*		WIN.INI supplies information for formatting the time
*
* Notes:
* 1.	The "time" argument is considered to be a delta time if it is
*		less than the number of seconds in a day.
* 2.	All operations assume that local time is being used.
*
* BUGS:
* 1.	For formats that don't use WIN.INI, there is no provision for getting
*		'AM' or 'PM' appended to the time.  (A literal can be specified, but
*		there is no way to conditionally select between two literals.)
*		Note, however, that time formats using WIN.INI cover this need.
* 2.	For formats that don't use WIN.INI, alphabetic information (e.g.,
*		the name of months) isn't internationalized.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
*--------------------------------------------------------------------------*/
char * PASCAL
TimeLSecToSz(
const char *szFormat,	// I format string
long	lEpochSec,		// I sec since Windows epoch start or delta sec
int		bufDim,			// I dimension of text buffer
char	*szBuf)			// O place to store formatted time
{
	int		stat;
	const char *pMain=szFormat;
	char	*pAux=NULL, *pFmt=(char *)pMain;
	struct tm *pTime, tmStruct;
	char	szShortDate[80];// WIN.INI: format for short date
	char	szLongDate[80];	// WIN.INI: format for long date
	char	szTemp[200];	// temp buffer for building string
	char	*pOut=szTemp;
	int		bDeltaTime=0;

	if (lEpochSec < 86400) {
		bDeltaTime = 1;
		pTime = &tmStruct;
		pTime->tm_year = pTime->tm_mon = pTime->tm_mday = 0;
		pTime->tm_wday = pTime->tm_yday = pTime->tm_isdst = 0;
		pTime->tm_hour = (int)lEpochSec / 3600;
		pTime->tm_min = ((int)lEpochSec % 3600) / 60;
		pTime->tm_sec = (int)lEpochSec % 60;
	}
	else
		pTime = localtime(&lEpochSec);

	stat = GetProfileString("intl", "sShortDate", "mm/dd/yy", szShortDate, 80);
	stat = GetProfileString("intl", "sLongDate",
											"month' 'd', 'yyyy", szLongDate, 80);

	while (*pFmt != '\0') {
		if (strncmp(pFmt, "hhnnss", 6) == 0) {
			TmTimeToSz(&pOut, pTime, 1, bDeltaTime);
			pFmt += 6;
		}
		else if (strncmp(pFmt, "hhnn", 4) == 0) {
			TmTimeToSz(&pOut, pTime, 0, bDeltaTime);
			pFmt += 4;
		}
		else if (strncmp(pFmt, "short", 5) == 0) {
			pAux = pFmt = szShortDate;
			pMain += 5;
		}
		else if (strncmp(pFmt, "long", 4) == 0) {
			pAux = pFmt = szLongDate;
			pMain += 4;
		}
		else if (strncmp(pFmt, "hh", 2) == 0) {
			IntToSz2(&pOut, pTime->tm_hour%12, 1);
			pFmt += 2;
		}
		else if (strncmp(pFmt, "h", 1) == 0) {
			IntToSz2(&pOut, pTime->tm_hour%12, 0);
			pFmt++;
		}
		else if (strncmp(pFmt, "HH", 2) == 0) {
			IntToSz2(&pOut, pTime->tm_hour, 1);
			pFmt += 2;
		}
		else if (strncmp(pFmt, "H", 1) == 0) {
			IntToSz2(&pOut, pTime->tm_hour, 0);
			pFmt++;
		}
		else if (strncmp(pFmt, "nn", 2) == 0) {
			IntToSz2(&pOut, pTime->tm_min, 1);
			pFmt += 2;
		}
		else if (strncmp(pFmt, "n", 1) == 0) {
			IntToSz2(&pOut, pTime->tm_min, 0);
			pFmt++;
		}
		else if (strncmp(pFmt, "ss", 2) == 0) {
			IntToSz2(&pOut, pTime->tm_sec, 1);
			pFmt += 2;
		}
		else if (strncmp(pFmt, "s", 1) == 0) {
			IntToSz2(&pOut, pTime->tm_sec, 0);
			pFmt++;
		}
		else if (strncmp(pFmt, "dddd", 4) == 0) {
			SzToSz(&pOut, gaszDayWeek[pTime->tm_wday]);
			pFmt += 4;
		}
		else if (strncmp(pFmt, "dayWeek", 7) == 0) {
			SzToSz(&pOut, gaszDayWeek[pTime->tm_wday]);
			pFmt += 7;
		}
		else if (strncmp(pFmt, "ddd", 3) == 0 || strncmp(pFmt, "day", 3) == 0) {
			SzToSz3(&pOut, gaszDayWeek[pTime->tm_wday]);
			pFmt += 3;
		}
		else if (strncmp(pFmt, "dd", 2) == 0) {
			IntToSz2(&pOut, pTime->tm_mday, 1);
			pFmt += 2;
		}
		else if (strncmp(pFmt, "d", 1) == 0) {
			IntToSz2(&pOut, pTime->tm_mday, 0);
			pFmt++;
		}
		else if (strncmp(pFmt, "month", 5) == 0) {
			SzToSz(&pOut, gaszMonth[pTime->tm_mon]);
			pFmt += 5;
		}
		else if (strncmp(pFmt, "MMMM", 4) == 0 || strncmp(pFmt, "mmmm", 4) == 0) {
			SzToSz(&pOut, gaszMonth[pTime->tm_mon]);
			pFmt += 4;
		}
		else if (strncmp(pFmt, "MMM", 3) == 0 ||
					strncmp(pFmt, "mmm", 3) == 0 || strncmp(pFmt, "mon", 3) == 0) {
			SzToSz3(&pOut, gaszMonth[pTime->tm_mon]);
			pFmt += 3;
		}
		else if (strncmp(pFmt, "MM", 2) == 0 || strncmp(pFmt, "mm", 2) == 0) {
			IntToSz2(&pOut, pTime->tm_mon+1, 1);
			pFmt += 2;
		}
		else if (strncmp(pFmt, "M", 1) == 0 || strncmp(pFmt, "m", 1) == 0) {
			IntToSz2(&pOut, pTime->tm_mon+1, 0);
			pFmt++;
		}
		else if (strncmp(pFmt, "yyyy", 4) == 0) {
			IntToSz4(&pOut, pTime->tm_year+1900, 1);
			pFmt += 4;
		}
		else if (strncmp(pFmt, "yy", 2) == 0) {
			IntToSz2(&pOut, pTime->tm_year-100, 1);
			pFmt += 2;
		}
		else if (*pFmt == '\'') {
			pFmt++;
			SzToSzSQ(&pOut, &pFmt);
		}
		else
			*(pOut++) = *(pFmt++);

		if (*pFmt == '\0') {
			if (pAux != NULL) {
				pFmt = (char *)pMain;
				pAux = NULL;
			}
		}
	}
	*pOut = '\0';
	szBuf[bufDim-1] = '\0';
	strncpy(szBuf, szTemp, bufDim-1);
	return szBuf;
}
/*+/csubr/TOC----------------------------------------------------------------
* TimeNowAsLSec - get current local time
*
* Purpose:
*		Gets the current local time.
*
* Return value:
*		number of seconds since "epoch"
*
*-Date     Author		Revision
* -------- ------------	--------
* 02-15-95 R. Cole		created
*--------------------------------------------------------------------------*/
long PASCAL
TimeNowAsLSec(void)
{
	time_t	epochSec;

	epochSec = time(NULL);	// can handle up to 68 years worth of seconds
	return epochSec;
}
/*+/csubr/TOC----------------------------------------------------------------
* TimeNowInParts - get current local time, broken into "parts"
*
* Purpose:
*		Gets the current local time and returns selected parts of it to
*		the caller.
*
* Return value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
TimeNowInParts(
int		*piYear,		// O place to put year (yyyy), or NULL
int		*piMonth,		// O place to put month (1-12), or NULL
int		*piDay,			// O place to put day (1-31), or NULL
int		*piHour,		// O place to put hour (0-23), or NULL
int		*piMinute,		// O place to put minute (0-59), or NULL
int		*piSecond)		// O place to put second (0-59), or NULL
{
	time_t	epochSec;
	struct tm *pTime;

	epochSec = time(NULL);	// can handle up to 68 years worth of seconds

	pTime = localtime(&epochSec);
	if (piSecond != NULL) *piSecond = pTime->tm_sec;
	if (piMinute != NULL) *piMinute = pTime->tm_min;
	if (piHour != NULL) *piHour = pTime->tm_hour;
	if (piDay != NULL) *piDay = pTime->tm_mday;
	if (piMonth != NULL) *piMonth = pTime->tm_mon+1;
	if (piYear != NULL) *piYear = pTime->tm_year+1900;
}
/*+/csubr/TOC----------------------------------------------------------------
* TimeNowToSz - convert current local time to text
*
* Purpose:
*		Gets the current local time and converts it to text.
*
*		For a full discussion on the "format" argument, see TimeLSecToSz.
*
* Return Value:
*		pointer to formatted text
*
* Implicit Inputs:
*		WIN.INI supplies information for formatting the time
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
*--------------------------------------------------------------------------*/
char * PASCAL
TimeNowToSz(
const char *szFormat,	// I format string
int		bufDim,			// I dimension of text buffer
char	*szBuf)			// O place to store formatted time
{
	time_t	epochSec;

	epochSec = time(NULL);	// can handle up to 68 years worth of seconds
	TimeLSecToSz(szFormat, epochSec, bufDim, szBuf);
	return szBuf;
}
/*+/csubr/TOC----------------------------------------------------------------
* TimeNowToTitleBar - display current local time on title bar
*
* Purpose:
*		Gets the current local time, converts it to text, and displays
*		the result on the title bar of the specified window.  For Windows
*		3.1, the date and time are displayed at left and right ends,
*		respectively, of the title bar.  For WindowsNT and Windows95,
*		both date and time are displayed at the right end of the title bar.
*
*		The date and time are formatted using the Windows formats that
*		are stored in WIN.INI.
*
* Return Value:
*		void
*
* Notes:
* 1.	This routine takes account of the space occupied by the title
*		bar caption.  If either the date or the time would overlap the
*		caption, then neither is displayed.
* 2.	This routine does nothing if the window doesn't have a caption.
* 3.	This routine must be called each time the time display is to be
*		updated.  To update the time display automatically, use
*		TimeToTitleBar.
*
* See Also:
*		TimeToTitleBar
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-20-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
TimeNowToTitleBar(
HWND	hwWin,			// I window whose title bar is to display time
int		bShowSec,		// I 1 to show seconds
int		bLongDate)		// I 1 to use long date format
{
	long	lStyle;
	HDC	hDC=0;
	HPEN	hPen=0, hPenOld=0;
	HBRUSH	hBrush=0, hBrushOld=0;
	UINT	taOld;
	int		bkModeOld;
	COLORREF rgbTextOld, rgbCaption;
	int		iBtnWid, iBtnHt, iFrWid, iFrHt;
	RECT	rectWin;
	SIZE	textSize;
	int		xPx, yPx;
	int		freeWidPx, capWidPx, timeWidPx, dateWidPx;
	char	szTime[40], szDate[40], szCaption[200];
	char	szTimePadded[40], szDatePadded[40];
	int		bTimeLZ;		// WIN.INI: use leading zeros for time
	int		bRight=0;
	int		bWin95, bWinNT;
	float	fVersion;

	WuGetWinVersion(&fVersion, &bWin95, &bWinNT);
	if (bWin95 || bWinNT)
		bRight = 1;

	bTimeLZ = (int)GetProfileInt("intl", "iTLZero", 1);

	if (!IsWindow(hwWin)) goto done;
	lStyle = GetWindowLong(hwWin, GWL_STYLE);
	if ((lStyle & WS_CAPTION) == 0) goto done;
	if ((hDC = GetWindowDC(hwWin)) == 0) goto done;
	rgbTextOld = SetTextColor(hDC, GetSysColor(COLOR_CAPTIONTEXT));
	bkModeOld = SetBkMode(hDC, TRANSPARENT);
	taOld = SetTextAlign(hDC, TA_LEFT | TA_NOUPDATECP);
	if ((hPen = CreatePen(PS_NULL, 0, 0)) == 0) goto done;
	if ((hPenOld = SelectObject(hDC, hPen)) == 0) goto done;
	if (GetActiveWindow() == hwWin)
		rgbCaption = GetSysColor(COLOR_ACTIVECAPTION);
	else
		rgbCaption = GetSysColor(COLOR_INACTIVECAPTION);
	if ((hBrush = CreateSolidBrush(rgbCaption)) == 0) goto done;
	if ((hBrushOld = SelectObject(hDC, hBrush)) == 0) goto done;

	// Get the width and height of the MIN, MAX, etc. buttons on the
	// title bar.  Also get the X and Y thicknesses of the window's frame.
	iBtnWid = GetSystemMetrics(SM_CXSIZE);
	iBtnHt = GetSystemMetrics(SM_CYSIZE);
	if (lStyle & WS_THICKFRAME) {
		iFrWid = GetSystemMetrics(SM_CXFRAME);
		iFrHt = GetSystemMetrics(SM_CYFRAME);
	}
	else if (lStyle & WS_DLGFRAME) {
		iFrWid = GetSystemMetrics(SM_CXDLGFRAME);
		iFrHt = GetSystemMetrics(SM_CYDLGFRAME);
	}
	else {
		iFrWid = GetSystemMetrics(SM_CXBORDER);
		iFrHt = GetSystemMetrics(SM_CYBORDER);
	}
	// Find out how big the caller's window is.
	GetWindowRect(hwWin, &rectWin);
	// Get the length, in pixels, of the caption and the date and
	// time strings.  Then determine the amount of space available at
	// either end of the caption and compare it with the space needed.
	// (When the format doesn't display leading zeros, there is a
	// problem when the string to be displayed this time occupies less
	// space than it did last time; to avoid this problem, the "space
	// needed" always is expanded to include leading zeros--even when
	// such expansion is superfluous.)  If the space is too small, just exit.
	GetWindowText(hwWin, szCaption, 200);
	szCaption[199] = '\0';
	GetTextExtentPoint(hDC, szCaption, strlen(szCaption), &textSize);
	capWidPx = textSize.cx;
	TimeNowToSz(bLongDate?"long":"short", 40, szDate);
	WuSzMCopy(szDatePadded, 40, szDate);
	if (!bLongDate) {
		char	szShortDate[80];	// WIN.INI: format for short date
		char	szPad[10];
		szPad[0] = '\0';
		GetProfileString("intl", "sShortDate", "MM/dd/yy", szShortDate, 80);
		if (stricmp(szShortDate, "MM") != 0) WuSzMCat(szPad, 10, "0");
		if (stricmp(szShortDate, "dd") != 0) WuSzMCat(szPad, 10, "0");
		WuSzMCat(szDatePadded, 40, szPad);
	}
	GetTextExtentPoint(hDC, szDatePadded, strlen(szDatePadded), &textSize);
	dateWidPx = textSize.cx;
	TimeNowToSz(bShowSec?"hhnnss":"hhnn", 40, szTime);
	WuSzMCopy(szTimePadded, 40, szTime);
	if (!bTimeLZ) {
		int		iHour, iMin, iSec;
		char	szPad[10];
		TimeNowInParts(NULL, NULL, NULL, &iHour, &iMin, &iSec);
		szPad[0] = '\0';
		if (iHour < 10) WuSzMCat(szPad, 10, "0");
		if (iMin < 10) WuSzMCat(szPad, 10, "0");
		if (bShowSec) {
			if (iSec < 10) WuSzMCat(szPad, 10, "0");
		}
		WuSzMCat(szTimePadded, 40, szPad);
	}
	GetTextExtentPoint(hDC, szTimePadded, strlen(szTimePadded), &textSize);
	timeWidPx = textSize.cx;
	freeWidPx = rectWin.right - rectWin.left + 1;
	// the window's frames and the margins on either side of time and date.
	freeWidPx -= 6 * iFrWid;
	if (lStyle & WS_SYSMENU) freeWidPx -= iBtnWid;
	if (lStyle & WS_MAXIMIZEBOX) freeWidPx -= iBtnWid;
	if (bRight)
		freeWidPx -= iBtnWid;
	if (lStyle & WS_MINIMIZEBOX) freeWidPx -= iBtnWid;
	freeWidPx -= capWidPx;

	if (!bRight) {
		// half of what's free is available for date, half for time.
		freeWidPx /= 2;
		if (freeWidPx < dateWidPx) goto done;
		if (freeWidPx < timeWidPx) goto done;

		// Now display the time and date.  (Using Rectangle and TextOut
		// seems a little convoluted, but it's necessary when the
		// caption color isn't a solid color--SetBkColor only works for solid.)
		xPx = iFrWid;
		if (lStyle & WS_SYSMENU) xPx += iBtnWid;
		xPx += iFrWid;
		yPx = iFrHt + 1;
		Rectangle(hDC, xPx, yPx, xPx + dateWidPx, yPx + iBtnHt);
		TextOut(hDC, xPx, yPx, szDate, strlen(szDate));
		xPx = (rectWin.right - rectWin.left) - iFrWid;
		if (lStyle & WS_MAXIMIZEBOX) xPx -= iBtnWid;
		if (lStyle & WS_MINIMIZEBOX) xPx -= iBtnWid;
		xPx -= iFrWid;
		xPx -= timeWidPx;
		Rectangle(hDC, xPx, yPx, xPx + timeWidPx, yPx + iBtnHt);
		SetTextAlign(hDC, TA_RIGHT);
		TextOut(hDC, xPx + timeWidPx, yPx, szTime, strlen(szTime));
	}
	else {
		char	szCombined[80];
		if (freeWidPx < dateWidPx + timeWidPx) goto done;

		WuSzMCopy(szCombined, 80, szDate);
		WuSzMCat(szCombined, 80, " ");
		WuSzMCat(szCombined, 80, szTime);
		// Now display the time and date.  (Using Rectangle and TextOut
		// seems a little convoluted, but it's necessary when the
		// caption color isn't a solid color--SetBkColor only works for solid.)
		yPx = iFrHt + 1;
		xPx = (rectWin.right - rectWin.left) - iFrWid;
		if (lStyle & WS_MAXIMIZEBOX) xPx -= iBtnWid;
		if (lStyle & WS_MINIMIZEBOX) xPx -= iBtnWid;
		xPx -= iFrWid;
		xPx -= dateWidPx + timeWidPx;
		xPx -= iBtnWid;	// third Windows95 button
		Rectangle(hDC, xPx, yPx, xPx + dateWidPx + timeWidPx, yPx + iBtnHt);
		SetTextAlign(hDC, TA_RIGHT);
		TextOut(hDC, xPx + dateWidPx + timeWidPx, yPx, szCombined, strlen(szCombined));
	}

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (hBrushOld != 0) SelectObject(hDC, hBrushOld);
	if (hBrush != 0) DeleteObject(hBrush);
	if (hDC != 0) {
		SetTextAlign(hDC, taOld);
		SetTextColor(hDC, rgbTextOld);
		SetBkMode(hDC, bkModeOld);
		ReleaseDC(hwWin, hDC);
	}
	return;
}
/*+/csubr/TOC----------------------------------------------------------------
* TimePartsToLSec - convert time and date "parts" to seconds since epoch
*
* Purpose:
*		Converts the pieces of time and date into seconds since the epoch,
*		so that the result is in the same frame of reference as the
*		"long" time returned by TimeNowInParts().
*
* Return Value:
*		seconds past the Windows epoch
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
*--------------------------------------------------------------------------*/
long PASCAL
TimePartsToLSec(
int		iYear,			// I year (yyyy)
int		iMonth,			// I month (1-12)
int		iDay,			// I day (1-31)
int		iHour,			// I hour (0-23)
int		iMinute,		// I minute (0-59)
int		iSecond)		// I second (0-59)
{
	long	epochSec;
	struct tm timeStruct, *pTime=&timeStruct;

	pTime->tm_year = iYear - 1900;
	pTime->tm_mon = iMonth - 1;
	pTime->tm_mday = iDay;
	pTime->tm_hour = iHour;
	pTime->tm_min = iMinute;
	pTime->tm_sec = iSecond;
	pTime->tm_isdst = -1;	// let mktime decide whether dst or not
	if (iYear == 0)
		epochSec = (((iDay*24L) + iHour)*60L + iMinute) * 60L + iSecond;
	else
		epochSec = mktime(pTime);
	return epochSec;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* TmTimeToSz - convert struct tm to formatted time text
*
* Purpose:
*		Converts the time of day part of a struct tm into text, using the
*		formatting specified for time of day in WIN.INI.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
*--------------------------------------------------------------------------*/
void
TmTimeToSz(
char	**ppOut,
struct tm *pTime,
int		bShowSec,
int		bDeltaTime)		// I 1 if this is a delta time, 0 if clock time
{
	int		stat;
	int		bTime24;		// WIN.INI: use 24 hour clock
	int		bTimeLZ;		// WIN.INI: use leading zeros for time
	char	szS1159[12];	// WIN.INI: string for 0000 to 1159
	char	szS2359[12];	// WIN.INI: string for 1200 to 2359
	char	szTimeSep[12];	// WIN.INI: separator for time fields
	int		iHour;

	bTime24 = (int)GetProfileInt("intl", "iTime", 1);
	bTimeLZ = (int)GetProfileInt("intl", "iTLZero", 1);
	stat = GetProfileString("intl", "s1159", "AM", szS1159, 12);
	stat = GetProfileString("intl", "s2359", "PM", szS2359, 12);
	stat = GetProfileString("intl", "sTime", ":", szTimeSep, 12);

	iHour = pTime->tm_hour;
	if (bTime24 || bDeltaTime)
		IntToSz2(ppOut, iHour, bTimeLZ);
	else {
		if (iHour >= 12) iHour -= 12;
		if (iHour == 0) iHour = 12;
		IntToSz2(ppOut, iHour, bTimeLZ);
	}
	SzToSz(ppOut, szTimeSep);
	IntToSz2(ppOut, pTime->tm_min, bTimeLZ);
	if (bShowSec) {
		SzToSz(ppOut, szTimeSep);
		IntToSz2(ppOut, pTime->tm_sec, bTimeLZ);
	}
	if (!bDeltaTime) {
		// If this isn't a delta time, attach the desired am/pm indicator.
		if (bTime24) {
			if (szS2359[0] != '\0') {
				SzToSz(ppOut, " ");
			}
		}
		else {
			if (pTime->tm_hour < 12 && szS1159[0] != '\0') {
				SzToSz(ppOut, " ");
				SzToSz(ppOut, szS1159);
			}
			else if (pTime->tm_hour >= 12 && szS2359[0] != '\0') {
				SzToSz(ppOut, " ");
				SzToSz(ppOut, szS2359);
			}
		}
	}
}
/*+/csubr/TOC----------------------------------------------------------------
* TimePartsToSz - convert time in parts to text
*
* Purpose:
*		Converts time in parts to text.
*
*		For a full discussion on the "format" argument, see TimeLSecToSz.
*
* Return Value:
*		pointer to formatted text
*
* Implicit Inputs:
*		WIN.INI supplies information for formatting the time
*
*-Date     Author		Revision
* -------- ------------	--------
* 02-12-95 R. Cole		created
*--------------------------------------------------------------------------*/
char * PASCAL
TimePartsToSz(
const char *szFormat,	// I format string
int		iYear,			// I year (yyyy)
int		iMonth,			// I month (1-12)
int		iDay,			// I day (1-31)
int		iHour,			// I hour (0-23)
int		iMinute,		// I minute (0-59)
int		iSecond,		// I second (0-59)
int		bufDim,			// I dimension of text buffer
char	*szBuf)			// O place to store formatted time
{
	long	lSec;

	lSec = TimePartsToLSec(iYear, iMonth, iDay, iHour, iMinute, iSecond);
	TimeLSecToSz(szFormat, lSec, bufDim, szBuf);
	return szBuf;
}
/*+/csubr/TOC----------------------------------------------------------------
* TimeSetSystemTime - update the system time and date
*
* Purpose:
*		Calls the Windows "date/time" applet to allow the operator to
*		set the time and date for the computer system.
*
* Return Value:
*		void
*
* Notes:
* 1.	If this routine is unable to start the "date/time" applet, it
*		'beeps' to the operator but takes no other action.
*
* See Also:
*		TimeNowAsLSec
*
*-Date     Author		Revision
* -------- ------------	--------
* 02-03-97 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
TimeSetSystemTime()
{
	char	szWinDir[300], szSysDir[300];
	const char *szControlApplet="CONTROL.EXE date/time";
	WORD	statGetWin, statGetSys, statWinExec;

	statGetWin = GetWindowsDirectory(szWinDir, 300);
	statGetSys = GetSystemDirectory(szSysDir, 300);
	WuSzMCat(szWinDir, 300, "\\");
	WuSzMCat(szWinDir, 300, szControlApplet);
	WuSzMCat(szSysDir, 300, "\\");
	WuSzMCat(szSysDir, 300, szControlApplet);
	statWinExec = WinExec(szWinDir, SW_SHOW);
	if (statWinExec <= HINSTANCE_ERROR) {
		statWinExec = WinExec(szSysDir, SW_SHOW);
		if (statWinExec <= HINSTANCE_ERROR) {
			statWinExec = WinExec(szControlApplet, SW_SHOW);
			if (statWinExec <= HINSTANCE_ERROR)
				MessageBeep(0);
		}
	}
}
/*+/csubr/TOC----------------------------------------------------------------
* TimeTimerCheck - check the status of an elapsed time timer
*
* Purpose:
*		Checks the status of an elapsed time timer.
*
* Return value:
*		number of seconds remaining
*
* Notes:
* 1.	If the timer is running when this function is called, then a check
*		is made to see if the elapsed time has expired.  If the elapsed
*		time has expired, then pTimer->fPctDone will be 100 and this
*		function will return 0 seconds remaining; otherwise, the return
*		value and pTimer->fPctDone reflect the remaining time.
* 2.	If the timer isn't running when this function is called (either
*		because the timer was never running or because the timer earlier
*		completed), then both the return value and pTimer->fPctDone will
*		be 0.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-09-95 R. Cole		created
*--------------------------------------------------------------------------*/
long PASCAL
TimeTimerCheck(
TIME_TIMER *pTimer)		// IO timer structure
{
	long	lNowSec, lLeftSec=0;

	if (pTimer->bRunning) {
		lNowSec = TimeNowAsLSec();
		lLeftSec = pTimer->lTimerSec - (lNowSec - pTimer->lStartTime);
		if (lLeftSec <= 0) {
			lLeftSec = 0;
			pTimer->bRunning = 0;
			pTimer->fPctDone = 100.F;
		}
		else if (pTimer->lTimerSec > 0) {
			pTimer->fPctDone = 100.F *
				(1.F - (float)lLeftSec / pTimer->lTimerSec);
			if (pTimer->fPctDone > 100.F)
				pTimer->fPctDone = 100.F;
		}
		else
			pTimer->fPctDone = -1.F;
	}
	else
		pTimer->fPctDone = 0.F;

	pTimer->lLeftSec = lLeftSec;
	return lLeftSec;
}
/*+/csubr/TOC----------------------------------------------------------------
* TimeTimerReset - reset an elapsed time timer
*
* Purpose:
*		Resets an elapsed time timer, so that it isn't running.  If
*		TimeTimerCheck is subsequently called, it will return 0 seconds
*		left and 0 percent done.
*
* Return value:
*		void
*
* Notes:
* 1.	If 'lSec' is >= 0, then the timer's desired elapsed time is set.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-09-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
TimeTimerReset(
TIME_TIMER *pTimer,		// IO timer structure
const long lSec)		// I number of seconds for timer to run, or -1
{
	pTimer->bRunning = 0;
	pTimer->fPctDone = 0.F;
	if (lSec >= 0)
		pTimer->lTimerSec = lSec;
	pTimer->lLeftSec = 0;
}
/*+/csubr/TOC----------------------------------------------------------------
* TimeTimerStart - start an elapsed time timer
*
* Purpose:
*		Starts an elapsed time timer.  TimeTimerCheck must be called
*		periodically to check the elapsed time.  (I.e., this routine
*		doesn't generate interrupts to the application program.)
*
* Return value:
*		void
*
* Notes:
* 1.	If the timer is currently running, it is restarted.
* 2.	If 'lSec' is < 0, then the timer is restarted using the previously
*		set elapsed time.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-09-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
TimeTimerStart(
TIME_TIMER *pTimer,		// IO timer structure
const long lSec)		// I number of seconds for timer to run, or -1
{
	pTimer->bRunning = 1;
	pTimer->lStartTime = TimeNowAsLSec();
	pTimer->fPctDone = 0.F;
	if (lSec >= 0)
		pTimer->lTimerSec = lSec;
	pTimer->lLeftSec = 0;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* TimeToTitleBar_cbk - callback for time display
*
* Purpose:
*		Gets the current local time, converts it to text, and displays
*		the result on the title bar of the specified window.  The date
*		and time are displayed at left and right ends, respectively, of
*		the title bar.
*
*		The date and time are formatted using the Windows formats that
*		are stored in WIN.INI.
*
* Return Value:
*		nTimer  if a timer is successfully created, or
*		0
*
*-Date     Author		Revision
* -------- ------------	--------
* 02-03-97 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
TimeToTitleBar_cbk(
HWND	hwWin,
UINT	wMsg,
UINT	myID,
DWORD	dwTime)
{
	int		bShowSec=myID&1;
	int		bLongDate=(myID/2)&1;

	TimeNowToTitleBar(hwWin, bShowSec, bLongDate);
}
/*+/csubr/TOC----------------------------------------------------------------
* TimeToTitleBar - periodically displays local time on title bar
*
* Purpose:
*		Periodically gets the current local time, converts it to text,
*		and displays the result on the title bar of the specified window.
*		With a Windows 3.1 style window, the date and time are displayed
*		at left and right ends, respectively, of the title bar.  With a
*		Windows95 style window, both date and time are displayed at the
*		right end of the title bar.
*
*		This routine starts a Windows timer, which triggers the periodic
*		update of the time display.  NOTE WELL!!  In order to avoid resource
*		leaks, the program must call this routine at exit in order to kill
*		the timer.  If the program has its own timer, then it should use
*		TimeNowToTitleBar instead of this routine.
*
*		The date and time are formatted using the Windows formats that
*		are stored in WIN.INI.
*
* Return Value:
*		1  if a timer is successfully started or stopped, or
*		0 otherwise
*
* Notes:
* 1.	To start time display, bKill must be zero.  To stop the time
*		display and return the timer to Windows, call this routine with
*		bKill set to 1 and the other arguments set exactly the same as
*		when the time display was started.
* 2.	This routine takes account of the space occupied by the title
*		bar caption.  If either the date or the time would overlap the
*		caption, then neither is displayed.
* 3.	This routine does nothing if the window doesn't have a title bar.
*
* See Also:
*		TimeNowToTitleBar
*
*-Date     Author		Revision
* -------- ------------	--------
* 02-03-97 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
TimeToTitleBar(
HWND	hwWin,			// I window whose title bar is to display time
WORD	bKill,			// I 0 to start display, 1 to stop
int		bShowSec,		// I 1 to show seconds
int		bLongDate)		// I 1 to use long date format
{
	long	lStyle;
	WORD	myTimer;
	int		myID;

	if (hwWin == 0) return 0;
	if (!IsWindow(hwWin)) return 0;
	lStyle = GetWindowLong(hwWin, GWL_STYLE);
	if ((lStyle & WS_CAPTION) == 0) return 0;

	myID = bShowSec?1:0 + bLongDate?2:0;
	if (bKill == 0) {
		myTimer = SetTimer(hwWin, myID, 1000,(TIMERPROC)TimeToTitleBar_cbk);
		return myTimer;
	}
	else {
		myTimer = KillTimer(hwWin, myID);
		if (myTimer) return 1;
		else return 0;
	}
}
