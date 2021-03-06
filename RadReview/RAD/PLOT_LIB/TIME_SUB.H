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
#if ! defined HDR_time_sub  // {
#define HDR_time_sub
/* ======================================================================= */
/* ============================== time_sub.h ============================= */
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
*
*		Also, much of the information in this file is used by Visual
*		Basic programs.  See individual '* Related Files:' discussions
*		for additional information.  For the affected areas, changes in
*		this file will require corresponding changes in the related files.
*--------------------------------------------------------------------------*/


/*+/mod----------------------------------------------------------------------
* time_sub.h - prototypes for time_sub.c
*
* Purpose:
*		This header file provides function prototypes for the functions
*		provided in time_sub.c
*
* Required for use with:
*		time_sub.c
*
* Automatically includes:
*		none
*
* Date     Author		Revision
* -------- ------------	--------
* 12-04-94 R. Cole		created
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

typedef struct {
	int		bRunning;		// 1 if timer is running
	long	lStartTime;		// time timer started
	long	lTimerSec;		// desired elapsed time
	long	lLeftSec;		// remaining time
	float	fPctDone;		// percent complete
} TIME_TIMER;

/*---------------------------------------------------------------------------
* prototypes
*
* Related Files:
*		o	WMCA_DEF.BAS  contains VB versions of some of these prototypes
*--------------------------------------------------------------------------*/

#if defined __cplusplus
extern "C" {
#endif

#if defined MAKE_DLL && defined WIN32
#define EXPORT16
#define EXPORT32 __declspec(dllexport)
#elif defined MAKE_DLL
#define EXPORT16 __export
#define EXPORT32
#else
#define EXPORT16
#define EXPORT32
#endif

EXPORT32 void EXPORT16 PASCAL
TimeLSecToParts(
	long	lEpochSec,		// I sec since Windows epoch start
	int		*piYear,		// O place to put year (yyyy), or NULL
	int		*piMonth,		// O place to put month (1-12), or NULL
	int		*piDay,			// O place to put day (1-31), or NULL
	int		*piHour,		// O place to put hour (0-23), or NULL
	int		*piMinute,		// O place to put minute (0-59), or NULL
	int		*piSecond);		// O place to put second (0-59), or NULL

EXPORT32 char * EXPORT16 PASCAL
TimeLSecToSz(
	const char *szFormat,	// I format string
	long	lEpochSec,		// I sec since Windows epoch start or delta sec
	int		bufDim,			// I dimension of text buffer
	char	*szBuf);		// O place to store formatted time

EXPORT32 long EXPORT16 PASCAL
TimeNowAsLSec(void);

EXPORT32 void EXPORT16 PASCAL
TimeNowInParts(
	int		*piYear,		// O place to put year (yyyy), or NULL
	int		*piMonth,		// O place to put month (1-12), or NULL
	int		*piDay,			// O place to put day (1-31), or NULL
	int		*piHour,		// O place to put hour (0-23), or NULL
	int		*piMinute,		// O place to put minute (0-59), or NULL
	int		*piSecond);		// O place to put second (0-59), or NULL

EXPORT32 char * EXPORT16 PASCAL
TimeNowToSz(
	const char *szFormat,	// I format string
	int		bufDim,			// I dimension of text buffer
	char	*szBuf);		// O place to store formatted time

EXPORT32 void EXPORT16 PASCAL
TimeNowToTitleBar(
	HWND	hwWin,			// I window whose title bar is to display time
	int		bShowSec,		// I 1 to show seconds
	int		bLongDate);		// I 1 to use long date format

EXPORT32 long EXPORT16 PASCAL
TimePartsToLSec(
	int		iYear,			// I year (yyyy)
	int		iMonth,			// I month (1-12)
	int		iDay,			// I day (1-31)
	int		iHour,			// I hour (0-23)
	int		iMinute,		// I minute (0-59)
	int		iSecond);		// I second (0-59)

EXPORT32 char * EXPORT16 PASCAL
TimePartsToSz(
	const char *szFormat,	// I format string
	int		iYear,			// I year (yyyy)
	int		iMonth,			// I month (1-12)
	int		iDay,			// I day (1-31)
	int		iHour,			// I hour (0-23)
	int		iMinute,		// I minute (0-59)
	int		iSecond,		// I second (0-59)
	int		bufDim,			// I dimension of text buffer
	char	*szBuf);		// O place to store formatted time

EXPORT32 void EXPORT16 PASCAL
TimeSetSystemTime();

EXPORT32 long EXPORT16 PASCAL
TimeTimerCheck(
	TIME_TIMER *pTimer);	// IO timer structure

EXPORT32 void EXPORT16 PASCAL
TimeTimerReset(
	TIME_TIMER *pTimer,		// IO timer structure
	const long lSec);		// I number of seconds for timer to run, or -1

EXPORT32 void EXPORT16 PASCAL
TimeTimerStart(
	TIME_TIMER *pTimer,		// IO timer structure
	const long lSec);		// I number of seconds for timer to run, or -1

EXPORT32 int EXPORT16 PASCAL
TimeToTitleBar(
	HWND	hwWin,			// I window whose title bar is to display time
	WORD	bKill,			// I 0 to start display, 1 to stop
	int		bShowSec,		// I 1 to show seconds
	int		bLongDate);		// I 1 to use long date format

#undef EXPORT16
#undef EXPORT32

#if defined __cplusplus
}
#endif

#endif // ! defined HDR_time_sub }
