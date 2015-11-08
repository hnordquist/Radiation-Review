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
//    File Name : DlgReviewPeriod.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

//19 Apr 2006 - pjm - changed code to reduce compiler warnings.

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <gen.h>
#include "RAD.H"  
#include "DateTimeUtils.h"        
#include "DbGeneralDbSupport.h"
#include "DlgReviewPeriod.h"
#include "Interval.h"
#include "FacilityConfig.h"
#include "Utilities.h"
#include "MyDateTime.h"

extern CMyDateTime glMyDateTime;
extern short glsFacNum;
extern char *glnaszIntervalUnitsPicklist[];
extern char glszOldDbName[]; 

static unsigned long gllulReviewPeriodOptionIntervalInDays = 0;
static BOOL gllbReviewPeriodOptionExecuted = FALSE;  // set true if SetSearchIntervalSpecifiedByUser() called.
static DATE glldReviewPeriodOptionStart = 0.0;
static DATE glldReviewPeriodOptionEnd = 0.0;

static void SetSearchIntervalSpecifiedByUser(struct GEN_DATE_STRUCT *pGenDate, struct GEN_TIME_STRUCT *pGenTime, int iUserUnits, unsigned long ulUserInterval);

/*===========================================================================
 *
 *  Name	 :  	ReviewPeriodMenuOption
 *
 *  Purpose	 :		Display the dialog box that allows the user to specify
 *					a new search interval that will be used as the default
 *					search interval for all subsequent user intervals.
 *
 *  Return value :  None
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  31-Mar-1999 SFK		Created
 *	21-Jun-2002 SFK		Modified to support DATE
 *
===========================================================================*/
void ReviewPeriodMenuOption(BOOL bAutomated)
{                                 
	static BOOL bFirst = TRUE;
//static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = {""};
	static DATE dDbExactIntervalLastTime = 0.0;
	static short sFacNum;
	static int iIntervalUnits;
	static DATE dReviewPeriodStart, dReviewPeriodEnd;
	static unsigned long ulInterval;

	struct GEN_DATE_STRUCT GenDate;
	struct GEN_TIME_STRUCT GenTime;
	CGFmtStr szInterval;    
	char strFacName[DB_NAME_LEN+1];
	BOOL bStatus = TRUE;     
	DATE dDbExactInterval;
	GUI_ACTION response = GUI_NO;
	CGFmtStr szMsg;


	char szFirstDbDate[DT_LEN+1], szLastDbDate[DT_LEN+1];  
	unsigned long ulDbIntervalInDays;
	
	GUI_ACTION DlgReturn;                                     

	// verify facility choice is still a legitimate Facility Number, if not set the
	// facility to the first facility in the database (if there is a facility)
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) {
		if (!pglFacCfg->GetFirstFacilityID(&glsFacNum)) {
			RadReviewMsg(uiNO_FACILITIES_ERR);
			return;
		}
	}
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return;


   /* ------------------------------------------------------------------
    *	Always find the first and last days in the database and set
	*	up the strings for display in the dialog box.
    * ----------------------------------------------------------------*/
	SetCurrentDrive();
	bStatus = GetDatabaseIntervalParameters(szFirstDbDate, szLastDbDate, &ulDbIntervalInDays, &dDbExactInterval);
	if (bStatus != TRUE) return;
	if (dDbExactInterval <= 0.0) {
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return;
	}

	iIntervalUnits = 0;	// only use days

    if ((bFirst == TRUE) || (!SameDb(glszOldDbName)) || (dDbExactInterval != dDbExactIntervalLastTime) || (sFacNum != glsFacNum)) {
		GetReviewPeriodParameters(&dReviewPeriodStart, &dReviewPeriodEnd, &ulInterval);
		iIntervalUnits = 0;	// GetReviewPeriodParameters always returns interval in days

		// remember the current conditions for future entries to this option
		sFacNum = glsFacNum;
	    GetDbName(glszOldDbName);								
		dDbExactIntervalLastTime = dDbExactInterval;
		bFirst = FALSE;

	}		



   /* ------------------------------------------------------------------
    *	Create dialog box to display the main export dialog options information
    * ----------------------------------------------------------------*/
    CGUI_Dlg Dlg(IDD_TOOLS_REVIEW_INTERVAL, NULL, DLG_POSN_NO_SAVE);
    if (Dlg.IsValid()) {
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		Dlg.DefinePushBtn(IDOK, GUI_OK);
				                                               
   		// Fill in Facility portion of dialog
		pglFacCfg->GetFacilityLongName(sFacNum, strFacName, DB_NAME_LEN);
		Dlg.DefineFldStaticTxt(IDC_REVIEW_FACILITY);
		Dlg.FldStaticTxtSetValue(IDC_REVIEW_FACILITY, strFacName);

	   /* ------------------------------------------------------------------
	    *	Display current starting ending database dates and interval
	    * ----------------------------------------------------------------*/
		Dlg.DefineFldStaticTxt(IDC_REVIEW_CURR_DB_START);
		Dlg.FldStaticTxtSetValue(IDC_REVIEW_CURR_DB_START, szFirstDbDate);
		Dlg.DefineFldStaticTxt(IDC_REVIEW_CURR_DB_END);
		Dlg.FldStaticTxtSetValue(IDC_REVIEW_CURR_DB_END, szLastDbDate);

		szInterval.Printf("%ld days", ulDbIntervalInDays);  	
		Dlg.DefineFldStaticTxt(IDC_REVIEW_CURR_DB_INTERVAL);
		Dlg.FldStaticTxtSetValue(IDC_REVIEW_CURR_DB_INTERVAL, szInterval);
		
	   /* ------------------------------------------------------------------
	    *	Fill in default values for user selectable search start date and time plus interval
	    * ----------------------------------------------------------------*/
		//Date.DATETimestampToGenStructs(dReviewPeriodStart, &GenDate, &GenTime);
		glMyDateTime.DATETimestampToGenStructs(dReviewPeriodStart, &GenDate, &GenTime);
		Dlg.DefineFldDate(IDC_REVIEW_SRCH_STRTDATE, &GenDate);
		Dlg.DefineFldTime(IDC_REVIEW_SRCH_STRTTIME, &GenTime);
		Dlg.DefineFldNum(IDC_REVIEW_SRCH_INTERVAL, &ulInterval, 1, 10000,"%ld");

		Dlg.SetFocus(IDC_REVIEW_SRCH_STRTDATE);
		if (bAutomated) Dlg.SetFocus(IDOK);
			
	    DlgReturn = Dlg.Go();
	    if (DlgReturn == GUI_OK) {  
	    	Dlg.RetrieveAllControlValues();						// get all values from dialog box

			szMsg.Printf("This will change the run-time defaults for Search Start\nDate/Time and Search Interval in all dialog boxes not yet\nexecuted in this run to the values you have selected.\nAre you sure you want to proceed?");
			response = GUI_MsgBox(szMsg, GUI_ICON_QUESTION, GUI_YESNO);
	
			if (response == GUI_YES) {
				// set these parameters for the new search interval
				SetSearchIntervalSpecifiedByUser(&GenDate, &GenTime, iIntervalUnits, ulInterval);
			}
			bFirst = TRUE;  // Ver 2.05, add to force update at beginning of dialog box
	    }	      
	}    
	return;
}   		                                 


   		                                 
// only called from SetReviewPeriod menu option
// modified to support DATE
static void SetSearchIntervalSpecifiedByUser(struct GEN_DATE_STRUCT *pGenDate, struct GEN_TIME_STRUCT *pGenTime, int iUnits, unsigned long ulInterval)
{
	DATE dIntervalInDays;
			
   /* ------------------------------------------------------------------
    *	Calculate the interval to search
    * ----------------------------------------------------------------*/
	glldReviewPeriodOptionStart = 
		//Date.GenStructsToDATETimestamp(pGenDate, pGenTime);
		glMyDateTime.GenStructsToDATETimestamp(pGenDate, pGenTime);
	    	
	switch (iUnits) {
		case 0: // days
		dIntervalInDays = ulInterval;
		break;
				
		case 1:	//hours
		dIntervalInDays = ((double)ulInterval)/24.0;
		break;
				
		case 2: // minutes
		dIntervalInDays = ((double)ulInterval)/1440.0;
		break;
	}
	
	glldReviewPeriodOptionEnd = glldReviewPeriodOptionStart + dIntervalInDays;
	//pjm 25 Jan 2006 Got rid of compiler warning with explicit cast to long
	gllulReviewPeriodOptionIntervalInDays = (unsigned long)ceil(dIntervalInDays);
	if (gllulReviewPeriodOptionIntervalInDays == 0) gllulReviewPeriodOptionIntervalInDays = 1;
	gllbReviewPeriodOptionExecuted = TRUE;
}


// Get the interval from the information stored in gll variables which are set when the user
// specifies an interval to search.
// Return the starting time (in seconds) and the interval (in days)
// that the user specified from the dialog box to set a subinterval of the database.	
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
//	static BOOL GetReviewPeriodFromUpperLayer(unsigned long *pulStart, unsigned long *pulEnd, unsigned long *pulIntervalInDays)
//
//	Input:	none
//			
//	Output:	pulStart	- start time of the review period from the upper layer in seconds
//			pulEnd		- end time of the review period from the upper layer in seconds
//			pulIntervalInDays - number of days in the interval from and including end days
//
//	Return:	TRUE (data in interval)/FALSE (no data in interval)
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//////////////////////////////////////////////////////////////////
void GetReviewPeriodFromMenuOption(unsigned long *pulStart, unsigned long *pulEnd, unsigned long *pulIntervalInDays)
{
	if (gllbReviewPeriodOptionExecuted) 
	{
		*pulStart = (unsigned long)glMyDateTime.DATETimestampToMyTimestamp(glldReviewPeriodOptionStart);
		*pulEnd = (unsigned long)glMyDateTime.DATETimestampToMyTimestamp(glldReviewPeriodOptionEnd);
		*pulIntervalInDays = (unsigned long)gllulReviewPeriodOptionIntervalInDays;
	}
}


void GetReviewPeriodFromMenuOption(DATE *pdStart, DATE *pdEnd, unsigned long *pulIntervalInDays)
{
	
	if (gllbReviewPeriodOptionExecuted == TRUE) {
		*pdStart = glldReviewPeriodOptionStart;
		*pdEnd = glldReviewPeriodOptionEnd;
		*pulIntervalInDays = gllulReviewPeriodOptionIntervalInDays;
		
	}
}

BOOL ReviewPeriodMenuOptionExecuted()
{
	return (gllbReviewPeriodOptionExecuted);
}
