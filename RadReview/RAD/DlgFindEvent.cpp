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
//    File Name : DlgFindEvent.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include <gui_tbl.h>            
#include "RAD.H"     
#include "DateTimeUtils.h"
#include "DbDefault.h"
#include "DbEvent.h"
#include "DbGeneralDbSupport.h"
#include "DbInterval.h"
#include "DbLimit.h"
#include "DlgFindEvent.h"
#include "FindEvent.h" 
#include "Interval.h"
#include "Plot.h"
#include "PrintFacilityHeader.h"
#include "RADInit.H"
#include "Utilities.h"
#include "FacilityConfig.h"
#include "MyDateTime.h"

extern char *glnaszIntervalUnitsPicklist[];
extern RADInit *pRAD_Init;  
extern short glsFacNum;
extern CMyDateTime glMyDateTime;
extern char glszOldDbName[]; 


static FILE *gllpFileHandle = NULL;

// parameters normally retrieved from dialog box
static int glliNumRows;    
static unsigned int glluiCheckBoxCol;
static unsigned int gllusChanCol;                 
static unsigned int glluiThresCol;
static unsigned int glluiBkgCol;
static BOOL *pgllbSelect = NULL;  
static double *pglldThresholds = NULL;
static double *pglldBkg = NULL;
static char **gllaszChanList = NULL;
static CGUI_TextWin *pWin = NULL;    
static short gllsStaNum;
static short gllsStaType;
static short gllsFacNum;
static DATE glldReviewPeriodStart = 0.0; 
static DATE glldReviewPeriodEnd = 0.0;
static unsigned long gllulInterval = 0;
static unsigned int glliIntervalUnits = 0;                 
static BOOL gllbPrintAll = FALSE;    
static BOOL gllbClearEvents = FALSE;
static struct GEN_DATE_STRUCT gllGenDate;
static struct GEN_TIME_STRUCT gllGenTime;
static struct DATA_ANALYZE gllAnalyzeParms;
static char gllcSep = '\t';
static 	short gllsMaxNameLen;     

//#define NUM_ALGORITHMS 4
#define NUM_ALGORITHMS 3
static char *gllAlgorithmsPicklist[] = {
				    "Threshold",
				    "Sliding Window with Background",
				    "Sliding Window using Local Background",
					//"Relative Threshold",
				    (char *) 0
				    };               

#define THRESHOLD_STR "Threshold"
//#define RELATIVE_THRESHOLD_STR "Relative Threshold"

// static functions
static void WriteDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL ReadDefaultDbParameters();
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL CheckPeakWidth(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL OnlyDialogBox(BOOL bAutomated);
static BOOL CloseDataResultsWindow(CGUI_TextWin *pWinArg);
static BOOL DoIt();

                
///////////////////////////////////////////////////////////////////////////
//	Name:	FindEventsMenuOption - both dialog and action
//			FindEventsDoIt - only action
//			FindEventsDialogBox - only dialog box
//
//	Description:
//	The actions associated with FindEvents menu option are separated into
//	just the dialog box, just the action and both the dialog box and action.
//	Different parts are called depending on whether running in SETUP, IMPORT
//	or REVIEW mode.
//
//	Declaration:
//	void FindEventsMenuOption()
//	BOOL FindEventsDoIt()
//	BOOL FindEventsDialogBox()
//
//	Input:	none
//			
//	Output:	none
//
//	Return:	TRUE (completed successfully)/FALSE (problem)
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//////////////////////////////////////////////////////////////////
void FindEventsMenuOption()
{      
	if (OnlyDialogBox(FALSE)){
		DoIt();
	}
}	


BOOL FindEventsDoIt()		// Running in IMPORT mode
{
	// get all the parameters
	ReadDefaultDbParameters();
	GetReviewPeriodParameters(&glldReviewPeriodStart, &glldReviewPeriodEnd, &gllulInterval);
	glliIntervalUnits = 0;
	// do the action
	return(DoIt());
}	

BOOL FindEventsDialogBox()	// Running in SETUP mode
{	
	// get all the parameters
	ReadDefaultDbParameters();
	GetReviewPeriodParameters(&glldReviewPeriodStart, &glldReviewPeriodEnd, &gllulInterval); // either db limits or from .ini
	glliIntervalUnits = 0;
	// do the dialog box
	return(OnlyDialogBox(TRUE));
}

///////////////////////////////////////////////////////////////////////////////////////////////
//	Executed when you change the algorithm selected in the dialog box.
////////////////////////////////////////////////////////////////////////////////////////////////
static void EnableFields(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	unsigned int uiIndex;
	CGUI_Table *pTbl;

	// Get the current index for the algorithm selected.
	pDlg->PickListGetValue(IDC_CHAN_EVENTS_SRCH_ALG, &uiIndex);

	pDlg->GetTable(IDC_EVENTS_PICKTABLE, &pTbl);

    if (strcmp(gllAlgorithmsPicklist[uiIndex], THRESHOLD_STR)  == 0) {
		// disable some fields in the dialog box
		pDlg->SetEnable(IDC_CHAN_EVENTS_SRCH_PEAK_PTS, FALSE);
		pDlg->SetEnable(IDC_CHAN_EVENTS_SRCH_SMOOTH, FALSE);
		pDlg->SetEnable(IDC_EVENTS_LABEL3, FALSE);
		pTbl->SetColWriteEnable(glluiBkgCol, FALSE);
    }
	/*
	else if (strcmp(gllAlgorithmsPicklist[uiIndex], RELATIVE_THRESHOLD_STR)  == 0) {
		// disable some fields in the dialog box
		pDlg->SetEnable(IDC_CHAN_EVENTS_SRCH_PEAK_PTS, FALSE);
		pDlg->SetEnable(IDC_CHAN_EVENTS_SRCH_SMOOTH, FALSE);
		pDlg->SetEnable(IDC_EVENTS_LABEL3, FALSE);
		pTbl->SetColWriteEnable(glluiBkgCol, FALSE);
    }
	*/
	else {
		// enable all fields in the dialog box
		pDlg->SetEnable(IDC_CHAN_EVENTS_SRCH_PEAK_PTS, TRUE);
		pDlg->SetEnable(IDC_CHAN_EVENTS_SRCH_SMOOTH, TRUE);
		pDlg->SetEnable(IDC_EVENTS_LABEL3, TRUE);
		pTbl->SetColWriteEnable(glluiBkgCol, TRUE);
	}
}



///////////////////////////////////////////////////////////////////////////
//	Name:	OnlyDialogBox
//
//	Description:
//	The dialog box is displayed where user inputs conditions for the 
//	event search to happen.
//
//	Declaration:
//	static BOOL OnlyDialogBox(BOOL bAutomated)
//
//	Input:	bAutomated - 
//			
//	Output:	none
//
//	Return:	TRUE (completed successfully)/FALSE (problem)
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//////////////////////////////////////////////////////////////////
static BOOL OnlyDialogBox(BOOL bAutomated)
{                                 
	static BOOL bFirst = TRUE;
//	static char szOldDbName[DRIVE_LEN+PATH_LEN+1] ={""};
	static DATE dDbExactIntervalLastTime = 0.0;
	
   // struct db_fac_rec dbFac;    
        
    GUI_ACTION DlgReturn;                                     
    CGUI_Table *pTbl;    
	CGFmtStr szInterval; 
	CGStr DumStr;
	
	BOOL bStatus = TRUE;     
	DATE dDbExactInterval;
	unsigned long ulDbIntervalInDays;

	char szFirstDate[DT_LEN+1];
	char szLastDate[DT_LEN+1];              
	short sNumChans;
	int i;
	char szFacName[DB_NAME_LEN+1];
	BOOL bWriteFile;

	const char *FileTypes[] = {"*.txt", NULL};
	
	// verify the facility is defined
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) return(FALSE);
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return (FALSE);

	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);


   /* ------------------------------------------------------------------
    *	Always find the first and last days in the database and set
	*	up the strings for display in the dialog box.
    * ----------------------------------------------------------------*/
	SetCurrentDrive();
	bStatus = GetDatabaseIntervalParameters(szFirstDate, szLastDate, &ulDbIntervalInDays, &dDbExactInterval);
	if (bStatus != TRUE) return(FALSE);
	if (dDbExactInterval <= 0.0) {
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return(FALSE);
	}


	if ((bFirst == TRUE) || (!SameDb(glszOldDbName)) || (dDbExactInterval != dDbExactIntervalLastTime) || (gllsFacNum != glsFacNum)) {
		ReadDefaultDbParameters();
		GetReviewPeriodParameters(&glldReviewPeriodStart, &glldReviewPeriodEnd, &gllulInterval);
		glliIntervalUnits = 0;
		gllAnalyzeParms.pFileHandle = NULL;

		// remember the current conditions for future entries to this menu option
		gllsFacNum = glsFacNum;
	    GetDbName(glszOldDbName);	
		dDbExactIntervalLastTime = dDbExactInterval;
		bFirst = FALSE;
	}	
   
   /* ------------------------------------------------------------------
    *	Define the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg EventsDlg(IDD_TOOLS_EVENTS, NULL, DLG_POSN_NO_SAVE);
    if (EventsDlg.IsValid()) {
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		EventsDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		EventsDlg.DefinePushBtn(IDOK, GUI_OK);                   
		EventsDlg.DefinePushBtn(IDC_EVENTS_ALL_BUTTON, GUI_NONE, SelectAll);          
		EventsDlg.DefinePushBtn(IDC_EVENTS_NONE_BUTTON, GUI_NONE, DeselectAll); 
		EventsDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateDbParms);  		   
		
	   /* ------------------------------------------------------------------
	    *	Set facility label in dialog box
	    * ----------------------------------------------------------------*/
	    EventsDlg.DefineFldStaticTxt(IDC_EVENTS_FACILITY);
		EventsDlg.FldStaticTxtSetValue(IDC_EVENTS_FACILITY, szFacName);
			
	   /* ------------------------------------------------------------------
	    *	Display current starting ending database dates and interval in dialog box
	    * ----------------------------------------------------------------*/
		EventsDlg.DefineFldStaticTxt(IDC_EVENTS_CURR_DB_START);
		EventsDlg.FldStaticTxtSetValue(IDC_EVENTS_CURR_DB_START, szFirstDate);
					
		EventsDlg.DefineFldStaticTxt(IDC_EVENTS_CURR_DB_END);
		EventsDlg.FldStaticTxtSetValue(IDC_EVENTS_CURR_DB_END, szLastDate);
	
		szInterval.Printf("%ld days", ulDbIntervalInDays);  			
		EventsDlg.DefineFldStaticTxt(IDC_EVENTS_CURR_DB_INTERVAL);
		EventsDlg.FldStaticTxtSetValue(IDC_EVENTS_CURR_DB_INTERVAL, szInterval);
		
	   /* ------------------------------------------------------------------
	    *	Set up the search interval parameters in dialog box
	    * ----------------------------------------------------------------*/
		//Date.DATETimestampToGenStructs(glldReviewPeriodStart, &gllGenDate, &gllGenTime);
		glMyDateTime.DATETimestampToGenStructs(glldReviewPeriodStart, &gllGenDate, &gllGenTime);
		EventsDlg.DefineFldDate(IDC_EVENTS_SRCH_STRTDATE, &gllGenDate);
		EventsDlg.DefineFldTime(IDC_EVENTS_SRCH_STRTTIME, &gllGenTime);
	
		EventsDlg.DefineFldNum(IDC_EVENTS_SRCH_INTERVAL, &gllulInterval, 1, 10000,"%ld",CheckPeakWidth);
		EventsDlg.DefinePickList(IDC_EVENTS_INTRVL_UNITS_PL, glnaszIntervalUnitsPicklist, (unsigned int *)&glliIntervalUnits);

	   /* ------------------------------------------------------------------
	    *	Set up the analysis parameters in dialog box
	    * ----------------------------------------------------------------*/
		EventsDlg.DefineFldNum(IDC_CHAN_EVENTS_SRCH_PEAK_PTS, &gllAnalyzeParms.sNumPeakPts, 1, 500,"%d");
		EventsDlg.DefinePickList(IDC_CHAN_EVENTS_SRCH_ALG, gllAlgorithmsPicklist, (unsigned int *)&gllAnalyzeParms.sWhichAlg, &DumStr, EnableFields);
		//IntegrityDlg.DefinePickList(IDC_INTEGRITY_STATION, aszStaList, &uiTemp, &DumStr, ValidateStation);

		
	   /* ------------------------------------------------------------------
	    *	Set up the output display options in dialog box
	    * ----------------------------------------------------------------*/
		EventsDlg.DefineCheckBox(IDC_CHAN_EVENTS_SRCH_SMOOTH, &gllAnalyzeParms.bSmooth);
		gllbPrintAll = FALSE;
		EventsDlg.DefineCheckBox(IDC_EVENTS_PRINT, &gllbPrintAll);
		EventsDlg.DefineCheckBox(IDC_CHAN_EVENT_SRCH_CLEAR, &gllbClearEvents);
		gllAnalyzeParms.bLogFile = FALSE;
		EventsDlg.DefineCheckBox(IDC_CHAN_EVENT_SRCH_LOG_FILE, &gllAnalyzeParms.bLogFile);

		int iLen;
		if (!pglFacCfg->CreateChannelPicklist(glsFacNum, true, &gllaszChanList, &iLen)) return(FALSE);
		EventsDlg.DefineTable(IDC_EVENTS_PICKTABLE, &pTbl);	
		if (pTbl != NULL) {
			glluiCheckBoxCol = pTbl->AddColCheckBox("Select", glliNumRows, pgllbSelect);		
			gllusChanCol = pTbl->AddColTxt("Channel", glliNumRows, gllaszChanList, iLen+1);
			glluiThresCol = pTbl->AddColNum("Threshold", glliNumRows, pglldThresholds,-1000.0, 1000000.0 ,"%10.3lf");
			glluiBkgCol = pTbl->AddColNum("Background", glliNumRows, pglldBkg,-1000.0, 1000000.0 ,"%10.3lf");
			pTbl->SetColWriteEnable(gllusChanCol, FALSE);
		}	

	    if (strcmp(gllAlgorithmsPicklist[gllAnalyzeParms.sWhichAlg], THRESHOLD_STR)  == 0) {
			// disable some fields in the dialog box
			EventsDlg.SetEnable(IDC_CHAN_EVENTS_SRCH_PEAK_PTS, FALSE);
			EventsDlg.SetEnable(IDC_CHAN_EVENTS_SRCH_SMOOTH, FALSE);
			EventsDlg.SetEnable(IDC_EVENTS_LABEL3, FALSE);
			pTbl->SetColWriteEnable(glluiBkgCol, FALSE);
	    }
		/*
		else if (strcmp(gllAlgorithmsPicklist[gllAnalyzeParms.sWhichAlg], RELATIVE_THRESHOLD_STR)  == 0) {
			// disable some fields in the dialog box
			EventsDlg.SetEnable(IDC_CHAN_EVENTS_SRCH_PEAK_PTS, FALSE);
			EventsDlg.SetEnable(IDC_CHAN_EVENTS_SRCH_SMOOTH, FALSE);
			EventsDlg.SetEnable(IDC_EVENTS_LABEL3, FALSE);
			pTbl->SetColWriteEnable(glluiBkgCol, FALSE);
	    }
		*/
		else {
			// enable all fields in the dialog box
			EventsDlg.SetEnable(IDC_CHAN_EVENTS_SRCH_PEAK_PTS, TRUE);
			EventsDlg.SetEnable(IDC_CHAN_EVENTS_SRCH_SMOOTH, TRUE);
			EventsDlg.SetEnable(IDC_EVENTS_LABEL3, TRUE);
		}

		bWriteFile = pRAD_Init->Get_WriteChanEventResults();
		EventsDlg.DefineCheckBox(IDC_CHAN_EVENT_SRCH_FILE, &bWriteFile);


			
	   /* ------------------------------------------------------------------
	    *	If running in REVIEW mode, set up the focus on the search interval.
		*	If running in SETUP mode, set the focus on OK
	    * ----------------------------------------------------------------*/
		EventsDlg.SetFocus(IDC_EVENTS_SRCH_INTERVAL);
		if (bAutomated) EventsDlg.SetFocus(IDOK);
		DlgReturn = EventsDlg.Go();      
				    
	    if (DlgReturn == GUI_OK) {  
	    	EventsDlg.RetrieveAllControlValues();
	    	if (pTbl != NULL) pTbl->RetrieveAllCellValues();  

			pRAD_Init->Set_WriteChanEventResults(bWriteFile);

		   /* ------------------------------------------------------------------
		    *	Check that they selected at least 1 channel to search for events
		    * ----------------------------------------------------------------*/
		    sNumChans = 0;
		    i = 0;
			while ((sNumChans == 0) && (i<glliNumRows)) {
				if (pgllbSelect[i] == TRUE) sNumChans++;
				i++;
			}		

			// no channels were selected for the search, abort now
		    if (sNumChans == 0) {
		    	RadReviewMsg(uiNO_CHANS_SELECTED_WARN);
		    	return(FALSE);
		    }	

			// if running in automated mode, write the parameters back to the default db
			if (bAutomated) {
				WriteDefaultDbParameters(&EventsDlg, IDD_TOOLS_EVENTS);		    
			}	

		   	// check to see if there is any data in the interval for any station
			bStatus = ConvertStructsToSearchIntervalParameters(&gllGenDate, &gllGenTime, glliIntervalUnits, gllulInterval, &glldReviewPeriodStart, &glldReviewPeriodEnd);
			if (bStatus != TRUE) {	// no data for any station in the interval
				RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, glldReviewPeriodStart, glldReviewPeriodEnd, "any station");
				return(FALSE);
			}
	    	return(TRUE);
	    }
	}
	return(FALSE);
}             


///////////////////////////////////////////////////////////////////////////
//	Name:	DoIt
//
//	Description:
//	Figure out whether you have data in the review period for any of the
//	channels selected (and enough data).  Do all the bookkeeping stuff
//	like getting the results window up with the common header information
//	printed in it.  The actual searching is done in the function.EventFind.
//
//	Declaration:
//	static BOOL DoIt()
//
//	Input:	none 
//			
//	Output:	none
//
//	Return:	TRUE (completed successfully)/FALSE (problem)
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//	21-Jun-2002	SFK		Modified for DATE
//////////////////////////////////////////////////////////////////
static BOOL DoIt()
{                                 
	
    struct db_chan_rec dbChan; 
    struct db_sta_rec dbSta;
    struct ANALYZE_PARMS *pAnalyzeChans = NULL;
	int status;     
	int i,j;    
    short sNumSta;     
    short sDbChanNum;      
	DATE dDum;
	unsigned long ulTotalPts, ulPts;
	DATE dLastDbDay, dFirstDbDay;
	struct IDStruct ChanID;
	                  
	SetCurrentDrive();
	
	// check to see if there is any data in the db corresponding to the review period
	ChanID.sStaID = ANY_STATION;
	ChanID.sStaType = ANY_TYPE;
	if (!GetDbTimeRange(ChanID, &dFirstDbDay, &dLastDbDay)) {	// get db range
		pglFacCfg->DestroyPicklist(gllaszChanList);
		gllaszChanList = NULL;
		return(FALSE);
	}
	if ((glldReviewPeriodStart <= dLastDbDay) && (glldReviewPeriodEnd >= dFirstDbDay)) {
	}
	else {
		RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, glldReviewPeriodStart, glldReviewPeriodEnd, "any station");
		pglFacCfg->DestroyPicklist(gllaszChanList);
		gllaszChanList = NULL;
		return(FALSE);
	}
	
   /* ------------------------------------------------------------------
    *	Clear Channel and Unexplained events in database if requested
	*	by checking box on dialog box.
    * ----------------------------------------------------------------*/
	if (gllbClearEvents == TRUE) {
		if ((DeleteDataByChannelEvents(gllsFacNum, CHANNEL_EVENT) != TRUE) ||
			(DeleteDataByChannelEvents(gllsFacNum, UNEXPLAINED_EVENT) != TRUE)) {
			pglFacCfg->DestroyPicklist(gllaszChanList);
			gllaszChanList = NULL;
			return (FALSE);
		}
	}	

   /* ------------------------------------------------------------------
    *	Set which separator and whether to do log file.  
	*	Use the same parameter from the rad.ini file as is used to 
	*	determine when you have been asked to write rad.db file.
	* ----------------------------------------------------------------*/
    gllAnalyzeParms.cSep = gllcSep;
	gllAnalyzeParms.bTextWin = TRUE;
	if (gllbPrintAll == TRUE) gllAnalyzeParms.bTextWin = FALSE;
	gllAnalyzeParms.bDB_File = pRAD_Init->Get_WriteChanEventResults();;
	// if log file is requested, then open it now.
	TextFile EventTextFile;
	if (gllAnalyzeParms.bLogFile) {
		EventTextFile.OpenTextFile("ChannelEventResults.txt", TRUE);
		if (EventTextFile.GetCancelStatus()) {
			gllAnalyzeParms.bLogFile = FALSE;
		}
		else {
			if (!EventTextFile.GetOpenStatus()) {
				RadReviewMsg(uiFILE_WRITE_ERR, EventTextFile.GetFilename());
				pglFacCfg->DestroyPicklist(gllaszChanList);
				gllaszChanList = NULL;
    			return(FALSE);	
    		}	
		}
	}
	gllAnalyzeParms.pFile = &EventTextFile;

   /* ------------------------------------------------------------------
    *	Process the table by sorting thru channels according to the
    *	station they belong to.
    * ----------------------------------------------------------------*/
    // allocate storage for all stations at this facility and init the structures
	sNumSta = pglFacCfg->GetNumStations(gllsFacNum);

 	pAnalyzeChans = (struct ANALYZE_PARMS  *)malloc(sNumSta*sizeof(struct ANALYZE_PARMS));
    if (pAnalyzeChans == NULL) 
		return(FALSE);

    for (j=0; j<sNumSta; j++) 
	{
		pAnalyzeChans[j].StaID.sStaID = -1;

   		for (i=0; i < MAX_NUM_DATA_CHANS; i++)
   			pAnalyzeChans[j].sChanOffset[i] = -1;	
    }	
		                                          
	// for now, the event search is done by stations, so have to figure out
	// which station is associated with every channel that is checked.
	// sort through the channels selected, associate them with their station, 
	// fill in the analysis structure with known information 
	for (i=0; i<glliNumRows; i++) {
		if (pgllbSelect[i] == TRUE) {	// channel selected on dialog box
			if (!pglFacCfg->GetChannelIDForPicklistChoice(i, &ChanID)) {
				pglFacCfg->DestroyPicklist(gllaszChanList);
				gllaszChanList = NULL;
				return(FALSE);
			}
			gllsStaNum = ChanID.sStaID;
			gllsStaType = ChanID.sStaType;
			if (!pglFacCfg->GetChannelRecord(glsFacNum, &ChanID, &dbChan)) {
				pglFacCfg->DestroyPicklist(gllaszChanList);
				gllaszChanList = NULL;
				return(FALSE);
			}
			sDbChanNum = ChanID.sChanID;
			for (j=0; j<sNumSta; j++) {
				if ((pAnalyzeChans[j].StaID.sStaID == gllsStaNum) && (pAnalyzeChans[j].StaID.sStaType == gllsStaType)) break;	// already in our list
				if (pAnalyzeChans[j].StaID.sStaID == -1) {
					pAnalyzeChans[j].StaID.sStaID = gllsStaNum;			// put station in our list
					pAnalyzeChans[j].StaID.sStaType = gllsStaType;			// put station in our list
					break;
				}	
			}
			// fill in the db channel number, background and threshold for each channel in the station
			pAnalyzeChans[j].StaID.sStaType = ChanID.sStaType;
			pAnalyzeChans[j].StaID.sChanID = sDbChanNum;
			pglFacCfg->GetStationID(glsFacNum, &(pAnalyzeChans[j].StaID));	// fill in the rest of the ID info
			pAnalyzeChans[j].sChanOffset[dbChan.s_chan_offset-1] = sDbChanNum;
			pAnalyzeChans[j].fThres[dbChan.s_chan_offset-1] = (float)pglldThresholds[i];
			if (gllAnalyzeParms.sWhichAlg == SLIDING_WINDOW_WITH_LOCAL_BKG) {	
				pAnalyzeChans[j].fBkg[dbChan.s_chan_offset-1] = 0;
			}
			else {
				pAnalyzeChans[j].fBkg[dbChan.s_chan_offset-1] = (float)pglldBkg[i];
			}	
				
					
		}	
	}	

			
   /* ------------------------------------------------------------------
    *	Now that we know which stations the channels go with, 
	*	Check to see that there is any data in the interval for
    *	any of the selected stations.
    * ----------------------------------------------------------------*/
    ulTotalPts = 0;
	for (j=0; j<sNumSta; j++) {
		 if(pAnalyzeChans[j].StaID.sStaID != -1) { // this entry is used
			//status = GetDataStatistics(pAnalyzeChans[j].StaID.sStaID, glldReviewPeriodStart, glldReviewPeriodEnd, &dDum, &dDum, &ulPts);
			 status = GetDataStatistics(pAnalyzeChans[j].StaID, true, glldReviewPeriodStart, glldReviewPeriodEnd, &dDum, &dDum, &ulPts);
    		ulTotalPts += ulPts;			// accumulate the number of data points for each station used
		 } 
    }	

	// no stations have any data in the review period, put up error msg and quit
	if (ulTotalPts == 0) {     
		CGFmtStr szMsg;	
	    szMsg.Printf("");
		for (j=0; j<sNumSta; j++) {		// generate a list of names for all stations without data
			if(pAnalyzeChans[j].StaID.sStaID != -1) {//KM
				if (pglFacCfg->GetStationRecord(glsFacNum, &(pAnalyzeChans[j].StaID), &dbSta)) {
					szMsg.PrintfAppend("%s", dbSta.sz_sta_name);
					if (j != (sNumSta-1)) szMsg.PrintfAppend(" or ");
				}	
			}
		}	
		RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, glldReviewPeriodStart, glldReviewPeriodEnd, szMsg);    
		if (pAnalyzeChans) free(pAnalyzeChans);
		return(FALSE);
	}	

	// at least one station has some data in review period so we can proceed
	// for the sophisticated algorithms we need a minimal number of data points, check that we have them
    unsigned long ulPtsNeeded;
    ulPtsNeeded = gllAnalyzeParms.sNumPeakPts*11;

    if ((gllAnalyzeParms.sWhichAlg != SIMPLE_THRESHOLD)
		//&& (gllAnalyzeParms.sWhichAlg != RELATIVE_THRESHOLD)
		&& (ulTotalPts <= ulPtsNeeded)) 
	{
    	RadReviewMsg(uiNOT_ENOUGH_DATA_TO_ANALYZE_WARN);
		if (pAnalyzeChans) free(pAnalyzeChans);
		pglFacCfg->DestroyPicklist(gllaszChanList);
		gllaszChanList = NULL;
		return(FALSE);
    }	
	
	// Open up the text window where the events will be displayed, get focus on it
	if (pWin == NULL) {                
		pWin = new CGUI_TextWin("Channel Events Found");
		if (pWin) pWin->ShowStatusBar(TRUE);
	}   
	else {
		pWin->Clear();
	}
	if (pWin) pWin->SetFocus();
			
	CGFmtStr StrHdr, StrTmp;
	// Display the general part of the header -- some of this stuff
	// should probably go in the algorithm specific part
	if (!PrintFacilityHeader(&StrHdr, gllsFacNum, glldReviewPeriodStart, glldReviewPeriodEnd)) return(FALSE);
//	if (!PrintFacilityHeader(pWin, gllsFacNum, glldReviewPeriodStart, glldReviewPeriodEnd)) return(FALSE);
		
	/* print search criteria */
    if (gllAnalyzeParms.sWhichAlg != SIMPLE_THRESHOLD) {
/*		if (pWin) pWin->Printf("\nEvent Search Criteria:");
		if (pWin) pWin->Printf("\n    Peak Width = %-5d", gllAnalyzeParms.sNumPeakPts);
		if (gllAnalyzeParms.bSmooth == TRUE) if (pWin) pWin->Printf("  Smoothed");
		else if (pWin) pWin->Printf("  Not Smoothed");	*/
		StrTmp.Printf("\nEvent Search Criteria:\n    Peak Width = %-5d  %sSmoothed" \
			,gllAnalyzeParms.sNumPeakPts,(gllAnalyzeParms.bSmooth?"":"Not "));
		StrHdr += StrTmp;
	}
 
	StrTmp.Printf("\nSearch Algorithm= %s", gllAlgorithmsPicklist[gllAnalyzeParms.sWhichAlg]);
	StrHdr += StrTmp;
//	if (pWin) pWin->Printf("\nSearch Algorithm= %s", gllAlgorithmsPicklist[gllAnalyzeParms.sWhichAlg]);
	gllAnalyzeParms.pWin = pWin;			// need to pass to analysis for output
	if (gllAnalyzeParms.pWin) 
		gllAnalyzeParms.pWin->Printf(StrHdr);
	if (gllAnalyzeParms.bLogFile) 
		gllAnalyzeParms.pFile->WriteLineToTextFile(StrHdr);
	
   /* ------------------------------------------------------------------
	*	Do the analysis for all the channels selected.
	* ----------------------------------------------------------------*/
	BOOL bFoundEvents = FALSE;
	unsigned long ulTotalEvents = 0;
	unsigned long ulEvents = 0;
	for (j=0; j<sNumSta; j++) {
		status = EventFind(glldReviewPeriodStart, glldReviewPeriodEnd, &pAnalyzeChans[j], &gllAnalyzeParms, gllbPrintAll, &ulEvents);
		if (ulEvents > 0) {
			bFoundEvents = TRUE;	// at least this station has events
			ulTotalEvents += ulEvents;
		}
		if (status == CANCELED) break;
	}
	if (bFoundEvents == FALSE) if (pWin) pWin->Printf("\nNo events found in the requested interval.");	// no events for any station
					
	if (pAnalyzeChans) free(pAnalyzeChans);

	// close Steves text db file if we have one open
	CloseTextDBFileForIR(ulTotalEvents);
//	if (gllAnalyzeParms.pTextDbFile != NULL) {
//		// now we finally know how many events we put in the file, so rewind the file, find "Rows", back up 6 and 
//		// replace the 99999 we put in as a place holder with the actual number of rows.  Aaargh!
//		char str[6];
//		sprintf(str,"%-5ld", ulTotalEvents);
//		gllAnalyzeParms.pTextDbFile->Rows_Set_Number(str);
//		gllAnalyzeParms.pTextDbFile->Database_Close();
//		delete gllAnalyzeParms.pTextDbFile;
//		gllAnalyzeParms.pTextDbFile = NULL;
//	}


	// close the log file if we were writing it.
	if (gllAnalyzeParms.bLogFile) {
		EventTextFile.CloseTextFile();
	}
                        
	SaveDatabaseNow();		// flush all the buffers
	UpdateGraphData(FALSE); // if the graph is displayed, update its data now.
   	
   /* ------------------------------------------------------------------
	*	Results are now written, user can close anytime
	* ----------------------------------------------------------------*/
	if (pWin) pWin->EnableUserClose(CloseDataResultsWindow);	

	pglFacCfg->DestroyPicklist(gllaszChanList);
	gllaszChanList = NULL;

   /* ------------------------------------------------------------------
	*	User selected cancel during the search
	* ----------------------------------------------------------------*/
	if (status == CANCELED) return (FALSE);
	return(TRUE);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	UpdateDbParms
//
//	Description:
//	Called from the dialog box when the NewDefaults button is pushed.
//	The user is prompted with a question as to whether they really want
//	to update the parameters.  If yes, the present values are read from
//	the dialog box and written to the Default database.
//
//	Declaration:
//	static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
//
//	Input:	pDlg	Dialog box where called from
//			CtrlID	Which control in dialog box are at now 
//			
//	Output:	none
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	16-Mar-1996		SFK		Created
//////////////////////////////////////////////////////////////////
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  
    
	status = AskUpdateQuestion();
	if (status != TRUE) return;

	WriteDefaultDbParameters(pDlg, CtrlID);	    
    return;
}        

                                
///////////////////////////////////////////////////////////////////////////
//	Name:	WriteDefaultDbParameters
//
//	Description:
//	Read all the paramters from the dialog box and write to the default database.
//	Save the database when finished.
//
//	Declaration:
//	static void WriteDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
//
//	Input:	pDlg	Dialog box where called from
//			CtrlID	Which control in dialog box are at now 
//			
//	Output:	none
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	16-Mar-1996		SFK		Created
//////////////////////////////////////////////////////////////////
static void WriteDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  
	CGUI_Table *pTable;
    int i;
    struct db_chan_event_dflt_rec dbChanDflt;
    short sNum;
    BOOL bBool;

    pDlg->FldNumGetValue(IDC_CHAN_EVENTS_SRCH_PEAK_PTS, &sNum);
	status = AccessDefaultDatabase(WRITE, S_CHAN_EVENT_SRCH_WIDTH, &sNum);
	if (status != TRUE) goto exit;
	
    pDlg->PickListGetValue(IDC_CHAN_EVENTS_SRCH_ALG, (unsigned int *)&sNum);
    status = AccessDefaultDatabase(WRITE, S_CHAN_EVENT_SRCH_ALG, &sNum);
	if (status != TRUE) goto exit;
	
    pDlg->CheckBoxGetValue(IDC_CHAN_EVENTS_SRCH_SMOOTH, &bBool);
	status = AccessDefaultDatabase(WRITE, B_CHAN_EVENT_SRCH_SMOOTH, &bBool);
	if (status != TRUE) goto exit;
	
    pDlg->CheckBoxGetValue(IDC_CHAN_EVENT_SRCH_CLEAR, &bBool);
	status = AccessDefaultDatabase(WRITE, B_CHAN_EVENT_SRCH_CLEAR, &bBool);
	if (status != TRUE) goto exit;
    
   /* ------------------------------------------------------------------
    *	Read the current table entry values and update the default
    *	parms in the db.
    * ----------------------------------------------------------------*/
	IDStruct ChanID;
	pDlg->GetTable(IDC_EVENTS_PICKTABLE, &pTable);
	if (pTable != NULL) {	
		pTable->RetrieveAllCellValues();
	    for (i=0; i<glliNumRows; i++) {
			if (!pglFacCfg->GetChannelIDForPicklistChoice(i, &ChanID)) goto exit;
			dbChanDflt.s_chan_event_chan_num = ChanID.sChanID;
			dbChanDflt.s_chan_event_sta_num = ChanID.sStaID;
			dbChanDflt.s_chan_event_sta_type = ChanID.sStaType;
			status = AccessDefaultDatabase(READ, 0, &dbChanDflt); 
			if (status != TRUE) goto exit;
			dbChanDflt.d_chan_event_srch_threshold = pglldThresholds[i];
			dbChanDflt.d_chan_event_srch_background = pglldBkg[i];
			dbChanDflt.b_chan_event_srch_select = pgllbSelect[i];
			short iDum;
			if (status == TRUE) status = AccessDefaultDatabase(READ, S_RAD_EVENT_SRCH_PAIRS, &iDum);
			status = AccessDefaultDatabase(WRITE, 0, &dbChanDflt); 
			if (status == TRUE) status = AccessDefaultDatabase(READ, S_RAD_EVENT_SRCH_PAIRS, &iDum);		// debug statement
			if (status != TRUE) goto exit;
	    }	
    }

   /* ------------------------------------------------------------------
    *	Write the changes to the .ini file
    * ----------------------------------------------------------------*/      
	pDlg->CheckBoxGetValue(IDC_CHAN_EVENT_SRCH_FILE, &bBool);
	pRAD_Init->Set_WriteChanEventResults(bBool);
   	pRAD_Init->Save_Parameters();

    SaveDatabaseNow();
	return;    

exit:
	SaveDatabaseNow();
	RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
	return;
}    


///////////////////////////////////////////////////////////////////////////
//	Name:	ReadDefaultDbParameters
//
//	Description:
//	Read paramters from the default database and initialize parameters
//	needed for this menu option.
//
//	Declaration:
//	static BOOL ReadDefaultDbParameters()
//
//	Input:	none
//			
//	Output:	none
//
//	Return:	TRUE(okay)/FALSE(problem)
//	
//  date    /	author	revision
//  -----------------	--------
//	16-Mar-1996		SFK		Created
//////////////////////////////////////////////////////////////////
static BOOL ReadDefaultDbParameters()
{                                
//    struct db_fac_rec dbFac;    
    struct db_chan_event_dflt_rec dbChanDflt;
 	CGFmtStr szInterval; 
	int status = TRUE;     
	int i;
	
	CleanUpFindEvents();		// get rid of any stuff already allocated
		
   /* ------------------------------------------------------------------
    *	Get the default parameters from the Default database.
    * ----------------------------------------------------------------*/
   	status = AccessDefaultDatabase(READ, S_DFLT_STATION, &gllsStaNum);
	if (status == TRUE) {
		status = AccessDefaultDatabase(READ, S_CHAN_EVENT_SRCH_WIDTH, &gllAnalyzeParms.sNumPeakPts);
		if (gllAnalyzeParms.sNumPeakPts < 0) gllAnalyzeParms.sNumPeakPts = 1;
	}	                    
	if (status == TRUE) {
		status = AccessDefaultDatabase(READ, B_CHAN_EVENT_SRCH_SMOOTH, &gllAnalyzeParms.bSmooth);
		if (gllAnalyzeParms.bSmooth > 1) gllAnalyzeParms.bSmooth = 0;
	}
	if (status == TRUE) {
		status = AccessDefaultDatabase(READ, B_CHAN_EVENT_SRCH_CLEAR, &gllbClearEvents);
		if (gllbClearEvents > 1) gllbClearEvents = 1;
	}	
	if (status == TRUE) {
		status = AccessDefaultDatabase(READ, S_CHAN_EVENT_SRCH_ALG, &gllAnalyzeParms.sWhichAlg);
		if (gllAnalyzeParms.sWhichAlg >= NUM_ALGORITHMS) gllAnalyzeParms.sWhichAlg = 0;
	}
	if (status == TRUE) {
		status = AccessDefaultDatabase(READ, B_CHAN_EVENT_SRCH_LOG_FILE, &gllAnalyzeParms.bLogFile);
		if (gllAnalyzeParms.bLogFile > 1) gllAnalyzeParms.bLogFile = 0;
	}
    else {
		RadReviewMsg(status, "when reading from Default database.  Cannot proceed.");			
		return(FALSE);
	}
		
   /* ------------------------------------------------------------------
    *	Create channel picklist of all channels in this facility
    * ----------------------------------------------------------------*/
	gllsFacNum = glsFacNum;
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) return(FALSE);	// verify facility is okay
	pglFacCfg->DestroyPicklist(gllaszChanList);
	int iLen;
	if (!pglFacCfg->CreateChannelPicklist(glsFacNum, true, &gllaszChanList, &iLen)) return(FALSE);
	gllsMaxNameLen = (short)iLen;
	glliNumRows = pglFacCfg->GetNumChannelsForFacility(glsFacNum);
		
   /* ------------------------------------------------------------------
    *	Allocate storage for all lists that will be in the table
    * ----------------------------------------------------------------*/
 	pglldThresholds = (double *)malloc(glliNumRows*sizeof(double));
    if (pglldThresholds == NULL) {
		pglFacCfg->DestroyPicklist(gllaszChanList);
		gllaszChanList = NULL;
	 	return(FALSE);
	} 	
	
    pgllbSelect = (BOOL *)malloc(glliNumRows*sizeof(BOOL));
    if (pgllbSelect == NULL) {
	 	if (pglldThresholds) free(pglldThresholds);
		pglldThresholds = NULL;
		pglFacCfg->DestroyPicklist(gllaszChanList);
		gllaszChanList = NULL;
	 	return(FALSE);
	} 	
		                             
 	pglldBkg = (double *)malloc(glliNumRows*sizeof(double));
    if (pglldBkg == NULL) {
		if (pgllbSelect) free(pgllbSelect); 
		pgllbSelect = NULL;
	 	if (pglldThresholds)free(pglldThresholds);
		pglldThresholds = NULL;
		pglFacCfg->DestroyPicklist(gllaszChanList);
		gllaszChanList = NULL;
	 	return(FALSE);
	} 	
		    
   /* ------------------------------------------------------------------
    *	Fill in the default values for the table lists
    * ----------------------------------------------------------------*/
	IDStruct ChanID;
	for (i=0; i<glliNumRows; i++) {
		if (!pglFacCfg->GetChannelIDForPicklistChoice(i, &ChanID)) return(FALSE);
		dbChanDflt.s_chan_event_chan_num = ChanID.sChanID;
		dbChanDflt.s_chan_event_sta_num = ChanID.sStaID;
		dbChanDflt.s_chan_event_sta_type = ChanID.sStaType;
		status = AccessDefaultDatabase(READ, 0, &dbChanDflt); 
		if (status != TRUE) {
			CleanUpFindEvents();
		 	return(FALSE);
		} 	
		pglldThresholds[i] = dbChanDflt.d_chan_event_srch_threshold;
		if (pglldThresholds[i] < 0) pglldThresholds[i] = 0;
		pglldBkg[i] = dbChanDflt.d_chan_event_srch_background;
		if (pglldBkg[i] < 0) pglldBkg[i] = 0;
		pgllbSelect[i] = dbChanDflt.b_chan_event_srch_select;
		if (pgllbSelect[i] > 1) pgllbSelect[i] = 0;
	}             
	
	return(TRUE);
} 


///////////////////////////////////////////////////////////////////////////
//	Name:	CleanUpFindEvents
//
//	Description:
//	Deallocate any arrays hanging around for the dialog box which were kept 
//	in case this option is called again during this run of the program.
//
//	Declaration:
//	void CleanUpFindEvents()
//
//	Input:	none
//			
//	Output:	none
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	16-Mar-1996		SFK		Created
//////////////////////////////////////////////////////////////////
void CleanUpFindEvents()
{   		                                       
	if (pglldBkg) free(pglldBkg);    	
	pglldBkg = NULL;
	
	if (pgllbSelect) free(pgllbSelect);    
	pgllbSelect = NULL;
	
	if (pglldThresholds) free(pglldThresholds);
	pglldThresholds = NULL;
	
	if (gllaszChanList)pglFacCfg->DestroyPicklist(gllaszChanList);
	gllaszChanList = NULL;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	SelectAll	- select all channels in dialog box
//			DeselectAll	- deselect all channels in dialog box
//
//	Description:
//	Actions for when the SelectAll or DeselectAll buttons are clicked
//	on the dialog box.
//
//	Declaration:
//	static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
//	static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
//
//	Input:	pDlg	Dialog box where called from
//			CtrlID	Which control in dialog box are at now 
//			
//	Output:	none
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	16-Mar-1996		SFK		Created
//////////////////////////////////////////////////////////////////
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable;
    int i;
    
	pDlg->GetTable(IDC_EVENTS_PICKTABLE, &pTable);
	if (pTable != NULL) {
	    for (i=0; i<glliNumRows; i++) {
	    	pgllbSelect[i] = TRUE;
	    }	
        pTable->UpdateCol(glluiCheckBoxCol);
    }
}        


static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable;
	int i;
		
	pDlg->GetTable(IDC_EVENTS_PICKTABLE, &pTable);
	if (pTable != NULL) {
	    for (i=0; i<glliNumRows; i++) {
	    	pgllbSelect[i] = FALSE;
	    }	
        pTable->UpdateCol(glluiCheckBoxCol);
    }
}        


///////////////////////////////////////////////////////////////////////////
//	Name:	CheckPeakWidth
//
//	Description:
//	Validation that the number they type in the Peak Width
//	field is an odd number.
//
//	Declaration:
//	static BOOL CheckPeakWidth(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
//
//	Input:	pDlg	Dialog box where called from
//			CtrlID	Which control in dialog box are at now 
//			
//	Output:	none
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	??	SFK		Created
//////////////////////////////////////////////////////////////////
static BOOL CheckPeakWidth(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	short sNum;
	div_t Result;

    pDlg->FldNumGetValue(IDC_CHAN_EVENTS_SRCH_PEAK_PTS, &sNum);
    Result = div((int)sNum, 2);
    if (Result.rem == 0) sNum++;
    
	return(TRUE);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	CloseDataResultsWindow
//
//	Description:
//	Called when user closes results text window.
//
//	Declaration:
//	static BOOL CloseDataResultsWindow(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
//
//	Input:	pWinArg	Text window being closed
//			
//	Output:	none
//
//	Return:	TRUE
//	
//  date    /	author	revision
//  -----------------	--------
//	??	SFK		Created
//////////////////////////////////////////////////////////////////
static BOOL CloseDataResultsWindow(CGUI_TextWin *pWinArg)
{
	BOOL bCanClose = TRUE;
	pWin = NULL;
	return(bCanClose);
}               
       
                    
