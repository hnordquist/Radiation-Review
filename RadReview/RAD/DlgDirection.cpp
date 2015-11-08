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
//    File Name : DlgDirection.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

//19 Apr 2006 - pjm - changed code to reduce compiler warnings.

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include "gui_tbl.h"
#include "RAD.H"                        
#include "DateTimeUtils.h"
#include "DbDefault.h"
#include "DbEvent.h"
#include "DbGeneralDbSupport.h"
#include "DbLimit.h"
#include "DlgDirection.h"
#include "EventTypeLists.h"
#include "FacilityConfig.h"
#include "Interval.h"
#include "PrintFacilityHeader.h"
#include "ProgressBars.h"
#include "RADInit.H" 
#include "TextFile.h"
#include "upperlayer/TXTWRDB.H"			// 8-Dec-04 SFK Added relative path
#include "Utilities.h"
#include "FacilityConfig.h" 
#include "MyDateTime.h"
#include <malloc.h>

extern RADInit *pRAD_Init;
extern BOOL glImportExit;
extern short glsFacNum;
extern short glsStaNum;
extern short glsChanNum;
extern char *glnaszIntervalUnitsPicklist[];
extern char *glnaszDirectionList[];
extern char *glnaszLocationList[];
extern CMyDateTime glMyDateTime;
extern char glszOldDbName[];

//extern TextFile *pDebugFile;

static char **aszChanList = NULL;
static int glluiNumRows;    
static unsigned int glluiCheckBoxCol;
static unsigned int glluiFromPickListCol;
static unsigned int glluiToPickListCol;
static unsigned int glluiDirectionPickListCol;
static unsigned int glluiLocationPickListCol;
static unsigned int glluiTypePickListCol;
static unsigned int glluiMinCol;
static unsigned int glluiMaxCol;
static unsigned int glluiRatioPickListCol;
static BOOL 		*pgllSelect = NULL;
static unsigned int *pglluiFromValues = NULL;
static unsigned int *pglluiToValues = NULL;
static unsigned int *pglluiLocValues = NULL;
static unsigned int *pglluiDirValues = NULL;
static unsigned int *pglluiTypeValues = NULL;
static double 		*pglldMinValues = NULL;
static double 		*pglldMaxValues = NULL;
static unsigned int *pglluiRatioValues = NULL;
static CGUI_TextWin *pWin = NULL;
static FILE *gllpFileHandle = NULL;
static char gllcSep;
static int glliMaxNameLen;
static unsigned long gllulUserInterval = 0;
static DATE glldUserIntervalStart = 0.0;
static DATE glldEnd = 0.0;
static int glliUserUnits = 0;                 
static short gllsFacNum;  
static struct GEN_DATE_STRUCT gllGenDate;
static struct GEN_TIME_STRUCT gllGenTime;
static Text_Write_Database *gllpWriteDB = NULL;
static short gllsStaNum; 
static BOOL gllbLogFile = FALSE;
static BOOL gllbWriteIrFile = FALSE;
static BOOL gllbClearAll;
static unsigned short gllusPairs=1;               
static char *gllnaszTypeList[6] = {"Start To Start", "Start To End  ", "End To Start  ", "End To End    ", "Max To Max    ", NULL};
static char *gllnaszRatioList[4] = {"Numerator  ", "Denominator", "None       ", NULL};

static BOOL CloseRawDataWindow(CGUI_TextWin *pWinArg);
static void WriteDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL ReadDefaultDbParameters();
static BOOL PrintDirEventToTextDBFile(struct db_dir_event_rec *pdbDirEvent, unsigned long ulNumPrinted, unsigned long ulTotalRecs);
static BOOL ReadAndPrintRadiationRecords(DATE dStart, DATE dEnd, CGUI_TextWin *pWin, TextFile* pFile, unsigned short usNumPairs);
static BOOL WriteDirEventToDB(struct db_event_rec *pFromEvent, struct db_event_rec *pToEvent, struct db_dir_event_rec *pdbDirEvent, unsigned int uiRatioType);
static BOOL OnlyDialogBox(BOOL bSetupOnly);
static BOOL DoIt();



/////////////////////////////////////////////////////////////////////////////////////
//	CloseTextWindow
//
//	Allows the user to close the window.
/////////////////////////////////////////////////////////////////////////////////////
static BOOL CloseRawDataWindow(CGUI_TextWin *pWinArg)
{
	BOOL bCanClose = TRUE;
	pWin = NULL;
	return(bCanClose);
}               

/////////////////////////////////////////////////////////////////////////////////////
//	CleanUpDirectionAnalysis
//
//	Deallocate storage used in the Event Summary dialog box.
/////////////////////////////////////////////////////////////////////////////////////
void CleanUpDirectionAnalysis()
{				          
	if (pgllSelect) free(pgllSelect); 
	pgllSelect = NULL;
	
	if (pglluiFromValues) free(pglluiFromValues);
	pglluiFromValues = NULL;
	
	if (pglluiToValues) free(pglluiToValues);
	pglluiToValues = NULL;
	
	if (pglluiLocValues) free(pglluiLocValues);
	pglluiLocValues = NULL;
	
	if (pglluiTypeValues) free(pglluiTypeValues);
	pglluiTypeValues = NULL;

	if (pglluiDirValues) free(pglluiDirValues);
	pglluiDirValues = NULL;
	
	if (pglldMinValues) free(pglldMinValues);
	pglldMinValues = NULL;
	
	if (pglldMaxValues) free(pglldMaxValues);
	pglldMaxValues = NULL;
	
	if (pglluiRatioValues) free(pglluiRatioValues);
	pglluiRatioValues = NULL;
	
	pglFacCfg->DestroyPicklist(aszChanList);
	aszChanList = NULL;

	gllpFileHandle = NULL;
}	


/*===========================================================================
 *
 *  Name	 :  WriteDefaultDbParameters, UpdateDbParms, ReadDefaultDbParameters
 *
 *  Purpose	 :  Called from the dialog box when the NewDefaults button is pushed.
 *				The present values are read from the dialog box and written to
 *				the Default database.
 *
 *  Return value :  None
 *
 *  Special notes:  User is notified if a problem is encountered in updating
 *					the default database
 *
 *  Implicit Input : 
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK	Created
 *
===========================================================================*/
static void WriteDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    struct db_dir_rec dbDir;
    CGUI_Table *pTable;
    int i;
    int status;  
    short sNum;
	struct IDStruct ChanID;
    BOOL bBool;
	unsigned int uiPickIndex;
	BOOL bSuccess;
	double dDbl;
    
   /* ------------------------------------------------------------------
    *	Get the current parameters set in the dialog box and write
    *	to default database.
    * ----------------------------------------------------------------*/ 
    pDlg->FldNumGetValue(IDC_RAD_EVENT_SRCH_PAIRS, &sNum);
	status = AccessDefaultDatabase(WRITE, S_RAD_EVENT_SRCH_PAIRS, &sNum);
	if (status != TRUE) goto exit;  
	
	pDlg->CheckBoxGetValue(IDC_RAD_ANLZ_CLEAR_ALL, &bBool);
	status = AccessDefaultDatabase(WRITE, B_RAD_EVENT_SRCH_CLEAR, (int*)&bBool);	
	if (status != TRUE) goto exit;
	
	
   /* ------------------------------------------------------------------
    *	Read the current table entry values and update the default
    *	parms in the db.
    * ----------------------------------------------------------------*/
    pDlg->GetTable(IDC_ANLZ_PICKTABLE, &pTable);
	if (pTable != NULL) {
		for (i=0; i<glluiNumRows; i++) {
			bSuccess = pTable->CheckBoxGetValue(i, glluiCheckBoxCol, &bBool);
			dbDir.b_dir_select[i] =  bBool; 
			
			bSuccess = pTable->PickListGetValue(i, glluiFromPickListCol, &uiPickIndex);
			pglFacCfg->GetChannelIDForPicklistChoice(uiPickIndex, &ChanID);
			dbDir.s_dir_srch_from_chan_num[i] = ChanID.sChanID;
			dbDir.s_dir_srch_from_sta_num[i] = ChanID.sStaID;
			dbDir.s_dir_srch_from_sta_type[i] = ChanID.sStaType;
			
			bSuccess = pTable->PickListGetValue(i, glluiToPickListCol, &uiPickIndex);
			pglFacCfg->GetChannelIDForPicklistChoice(uiPickIndex, &ChanID);
			dbDir.s_dir_srch_to_chan_num[i] = ChanID.sChanID;
			dbDir.s_dir_srch_to_sta_num[i] = ChanID.sStaID;
			dbDir.s_dir_srch_to_sta_type[i] = ChanID.sStaType;

			bSuccess = pTable->PickListGetValue(i, glluiTypePickListCol, &uiPickIndex);
			dbDir.b_dir_srch_type[i] = uiPickIndex;
			bSuccess = pTable->PickListGetValue(i, glluiDirectionPickListCol, &uiPickIndex);
			dbDir.s_dir_srch_direction[i] = uiPickIndex;
			bSuccess = pTable->PickListGetValue(i, glluiLocationPickListCol, &uiPickIndex);
			dbDir.s_dir_srch_location[i] = uiPickIndex;
			bSuccess = pTable->NumGetValue(i, glluiMinCol, &dDbl);
			dbDir.us_dir_srch_min_secs[i] = (unsigned short)dDbl;
			bSuccess = pTable->NumGetValue(i, glluiMaxCol, &dDbl);			                                                                   
			dbDir.us_dir_srch_max_secs[i] = (unsigned short)dDbl;
			bSuccess = pTable->PickListGetValue(i, glluiRatioPickListCol, &uiPickIndex);
			dbDir.s_dir_srch_ratio[i] = uiPickIndex;

		}	

		for (i=glluiNumRows; i <MAX_DB_CHANS;  i++) {  // fill in the remainder of entries
			dbDir.b_dir_select[i] =  0; 

			dbDir.s_dir_srch_from_chan_num[i] = 0;
			dbDir.s_dir_srch_from_sta_num[i] = 0;
			dbDir.s_dir_srch_from_sta_type[i] = 0;

			dbDir.s_dir_srch_to_chan_num[i] = 0;
			dbDir.s_dir_srch_to_sta_num[i] = 0;
			dbDir.s_dir_srch_to_sta_type[i] = 0;

			dbDir.s_dir_srch_direction[i] = 0;
			dbDir.s_dir_srch_location[i] = 0;
			dbDir.us_dir_srch_min_secs[i] = 0;
			dbDir.us_dir_srch_max_secs[i] = 0;
			dbDir.s_dir_srch_ratio[i] = 0;
		}	
		status = AccessDefaultDatabase(WRITE, 0, &dbDir); 
		if (status != TRUE) goto exit;
	}
	SaveDatabaseNow();

   /* ------------------------------------------------------------------
    *	Write the changes to the .ini file
    * ----------------------------------------------------------------*/      
	pDlg->CheckBoxGetValue(IDC_ANLZ_SRCH_FILE, &bBool);
	pRAD_Init->Set_WriteDirEventResults(bBool);
   	pRAD_Init->Save_Parameters();
	
	return;
	
exit:
	RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  
    
	status = AskUpdateQuestion();
	if (status != TRUE) return;
	WriteDefaultDbParameters(pDlg, CtrlID);
}        
                                                      
static BOOL ReadDefaultDbParameters()
{     
	struct db_dir_rec dbDir;
	int status;
	int i;
		
		CleanUpDirectionAnalysis();
		
	   /* ------------------------------------------------------------------
	    *	Get the default parameters from the Default database.
	    * ----------------------------------------------------------------*/
    	status = AccessDefaultDatabase(READ, S_DFLT_STATION, &gllsStaNum);
    	if (status == TRUE) status = AccessDefaultDatabase(READ, S_RAD_EVENT_SRCH_PAIRS, (short *)&gllusPairs);
		if (status == TRUE) status = AccessDefaultDatabase(READ, B_RAD_EVENT_SRCH_CLEAR, (int*)&gllbClearAll);	
		gllbLogFile = FALSE;		// always set to false
		if (status == TRUE) status = AccessDefaultDatabase(READ, 0, &dbDir); 
    	if (status != TRUE) {
			RadReviewMsg(status, "when reading from Default database.  Cannot proceed.");			
			return (FALSE);
		}
		
    	/* ------------------------------------------------------------------
	    *	Create channel picklist of all channels in this facility
	    * ----------------------------------------------------------------*/
		gllsFacNum = glsFacNum;
		pglFacCfg->DestroyPicklist(aszChanList);
		short sNumRows = glluiNumRows;
		if (!pglFacCfg->CreateChannelPicklist(gllsFacNum, true, &aszChanList, &glliMaxNameLen))return(FALSE);
		glluiNumRows = sNumRows;
	
	   /* ------------------------------------------------------------------
	    *	Allocate storage for all picklists and select boxes that will be in the table
	    * ----------------------------------------------------------------*/
		glluiNumRows = pglFacCfg->GetNumChannelsForFacility(gllsFacNum);
	    pgllSelect = (BOOL *)malloc(glluiNumRows*sizeof(BOOL));
	    if (pgllSelect == NULL) {
	    	CleanUpDirectionAnalysis();
			return(FALSE);
		}
		                          
	    pglluiFromValues = (unsigned int *)malloc(glluiNumRows*sizeof(int));
	    if (pglluiFromValues == NULL) {
	    	CleanUpDirectionAnalysis();
			return(FALSE);
		}
		
	    pglluiToValues = (unsigned int *)malloc(glluiNumRows*sizeof(int));
	    if (pglluiToValues == NULL) {
	    	CleanUpDirectionAnalysis();
			return(FALSE);
		}

	    pglluiTypeValues = (unsigned int *)malloc(glluiNumRows*sizeof(int));
	    if (pglluiTypeValues == NULL) {
	    	CleanUpDirectionAnalysis();
			return(FALSE);
		}

	    pglluiLocValues = (unsigned int *)malloc(glluiNumRows*sizeof(int));
	    if (pglluiLocValues == NULL) {
	    	CleanUpDirectionAnalysis();
			return(FALSE);
		}
		
	    pglluiDirValues = (unsigned int *)malloc(glluiNumRows*sizeof(int));
	    if (pglluiDirValues == NULL) {
	    	CleanUpDirectionAnalysis();
			return(FALSE);
		}

	    pglldMinValues = (double *)malloc(glluiNumRows*sizeof(double));
	    if (pglldMinValues == NULL) {
	    	CleanUpDirectionAnalysis();
			return(FALSE);
		}
	
	    pglldMaxValues = (double *)malloc(glluiNumRows*sizeof(double));
	    if (pglldMaxValues == NULL) {
	    	CleanUpDirectionAnalysis();
			return(FALSE);
		}
	
	    pglluiRatioValues = (unsigned int *)malloc(glluiNumRows*sizeof(int));
	    if (pglluiRatioValues == NULL) {
	    	CleanUpDirectionAnalysis();
			return(FALSE);
		}

	   /* ------------------------------------------------------------------
	    *	Fill in the table lists from the default db parameters
	    * ----------------------------------------------------------------*/
		struct IDStruct TempID, FirstID;
		short sTemp, sFirst;
		pglFacCfg->GetFirstChannelID(gllsFacNum, &FirstID);
		pglFacCfg->GetPicklistChoiceForChannelID(FirstID, &sFirst);

	    for (i=0; i<glluiNumRows; i++) {
			pgllSelect[i] = dbDir.b_dir_select[i];
			pglluiFromValues[i] = sFirst;				// default to first in facility
			TempID.sChanID = dbDir.s_dir_srch_from_chan_num[i];
			TempID.sStaID = dbDir.s_dir_srch_from_sta_num[i];
			TempID.sStaType = dbDir.s_dir_srch_from_sta_type[i];
			if (pglFacCfg->IsChannelEnabled(gllsFacNum, &TempID)) {		// another one is really selected
				pglFacCfg->GetPicklistChoiceForChannelID(TempID, &sTemp);
				pglluiFromValues[i] = sTemp;
			}
			pglluiToValues[i] = sFirst;
			TempID.sChanID = dbDir.s_dir_srch_to_chan_num[i];
			TempID.sStaID = dbDir.s_dir_srch_to_sta_num[i];
			TempID.sStaType = dbDir.s_dir_srch_to_sta_type[i];
			if (pglFacCfg->IsChannelEnabled(gllsFacNum, &TempID)) {
				pglFacCfg->GetPicklistChoiceForChannelID(TempID, &sTemp);
				pglluiToValues[i] = sTemp;
			}
			pglluiTypeValues[i] = dbDir.b_dir_srch_type[i];
			pglluiDirValues[i] = dbDir.s_dir_srch_direction[i];
			pglluiLocValues[i] = dbDir.s_dir_srch_location[i];
			pglldMinValues[i] = (double)dbDir.us_dir_srch_min_secs[i];
			pglldMaxValues[i] = (double)dbDir.us_dir_srch_max_secs[i];
			pglluiRatioValues[i] = dbDir.s_dir_srch_ratio[i];
		}
		return (TRUE);
}                                                      
 
// 15-Dec-2004 SFK Pick up status type from general lists in GetDirEventTypeStr
void FormatDirEvent(struct db_dir_event_rec *pdbDirEvent, unsigned long ulNumPrinted, CGFmtStr *pFormattedStr)
{
	int i;
	char szTime[DT_LEN+1], szDate[DT_LEN+1];
	struct db_chan_rec dbFromChan, dbToChan;  
	CGFmtStr Str;
	//CMyDateTime Date;
	struct IDStruct TempID;
	
	if (pFormattedStr == NULL) return;

	// First time, print the column headers for the text window			
	if (ulNumPrinted == 0) {
		Str.Printf("\n               Start    End                 Event");
		*pFormattedStr += Str;
		Str.Printf("\n      Date     Time     Time     Dir    Loc Type         Ratio Pairs");
		*pFormattedStr += Str;
			          //96.10.10 13:44:44 13:44:44 In  A   Unreconciled   1.2 SW FC A, SW FC C
	}

	// print an event number
	Str.Printf("\n%5ld ", ulNumPrinted+1);
	*pFormattedStr += Str;

	// print beginning date, time				                       
	glMyDateTime.DATETimestampToDateTimeStrs(pdbDirEvent->d_dir_event_beg_time_key, szDate, szTime);
	Str.Printf("%s %s ", szDate, szTime);
	*pFormattedStr += Str;

	/* print end time */
	glMyDateTime.DATETimestampToDateTimeStrs(pdbDirEvent->d_dir_event_end_time, szDate, szTime);
	Str.Printf("%s ", szTime);  
	*pFormattedStr += Str;

	/* Print direction */
	Str.Printf("%s ", glnaszDirectionList[pdbDirEvent->us_dir_event_direction]);
	*pFormattedStr += Str;

	/* Print location */
	Str.Printf("%s ", glnaszLocationList[pdbDirEvent->us_dir_event_location]);
	*pFormattedStr += Str;

	char szEventTypeStr[MAX_LINE_LEN+1];
	GetDirEventStatusStr(pdbDirEvent, szEventTypeStr);
	Str.Printf("%s ", szEventTypeStr);				// 15-Dec-2004 SFK Added getting Event Str from GetEventTypeStr
	*pFormattedStr += Str;
	
	/* Print ratio */ 
	if ((pdbDirEvent->d_dir_event_ratio_denominator > 0.0) &&
		(pdbDirEvent->s_dir_event_num_numerator > 0) &&
		(pdbDirEvent->s_dir_event_num_denominator > 0)) {

		double dNum =  pdbDirEvent->d_dir_event_ratio_numerator/(double)pdbDirEvent->s_dir_event_num_numerator;
		double dDenom = pdbDirEvent->d_dir_event_ratio_denominator/(double)pdbDirEvent->s_dir_event_num_denominator;
		Str.Printf("%5.1lf ", dNum/dDenom);
	}
	else {	
		Str.Printf("  N/A ");
	}
	*pFormattedStr += Str;

	if (pdbDirEvent->us_dir_event_status != MISSING_DATA_EVENT) {    							
		/* Print pairs used to make decision */
		for (i=0; i < MAX_PAIRS; i++) {
			Str.Printf("");		// clear the string
			if (pdbDirEvent->us_dir_event_from_chan[i] != 65535) {
				TempID.sChanID = pdbDirEvent->us_dir_event_from_chan[i];
				TempID.sStaID = pdbDirEvent->us_dir_event_from_sta[i];
				TempID.sStaType = pdbDirEvent->us_dir_event_from_sta_type[i];
				if (!pglFacCfg->GetChannelRecord(gllsFacNum, &TempID, &dbFromChan)) continue;
				TempID.sChanID = pdbDirEvent->us_dir_event_to_chan[i];
				TempID.sStaID = pdbDirEvent->us_dir_event_to_sta[i];
				TempID.sStaType = pdbDirEvent->us_dir_event_to_sta_type[i];
				if (!pglFacCfg->GetChannelRecord(gllsFacNum, &TempID, &dbToChan)) continue;
				if (i == 0) {
					Str.Printf("%s to %s", dbFromChan.sz_chan_short_name,
										     dbToChan.sz_chan_short_name);
				}										   
				else {
					Str.Printf(", %s to %s",
											 dbFromChan.sz_chan_short_name,
											 dbToChan.sz_chan_short_name);
				}										   
			}
			*pFormattedStr += Str;
		}
	}

}


/*===========================================================================
 *
 *  Name	 :  PrintDirEventToTextWin
 *
 *  Purpose	 :  Standard routine to print a radiation event record to the
 *				text window.  If the counter ulNumPrinted is 0, then
 *				header columns are also printed.
 *
 *  Return value :  TRUE/FALSE
 *
 *  Special notes:  
 *
 *  Implicit Input : 
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK	Created
 *  20-Aug-1999 SFK	  Version 2.05	Fixed columns so they line up correctly
===========================================================================*/
void PrintDirEventToTextWin(struct db_dir_event_rec *pdbDirEvent, CGUI_TextWin *pWin, unsigned long ulNumPrinted)
{
	CGFmtStr Str;

	FormatDirEvent(pdbDirEvent, ulNumPrinted, &Str);
	
	pWin->Printf("%s", Str.Get_sz());

	return;
}
                
	

/*===========================================================================
 *
 *  Name	 :  PrintDirEventToTextFile
 *
 *  Purpose	 :  Print the radiation event to the log file if requested.
 *
 *  Return value :  TRUE/FALSE
 *
 *  Implicit Input : 
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			 SFK	Created
 *
===========================================================================*/
void PrintDirEventToTextFile(struct db_dir_event_rec *pdbDirEvent, TextFile *pFile, unsigned long ulNumPrinted)
{

	CGFmtStr Str;

	// format the raw data point
	FormatDirEvent(pdbDirEvent, ulNumPrinted, &Str);
	
	pFile->WriteLineToTextFile(Str);

}



/*===========================================================================
 *
 *  Name	 :  PrintDirEventToTextDBFile
 *
 *  Purpose	 :  Prints the radiation event to the text file to be used by IR.
 *
 *  Return value :  TRUE/FALSE
 *
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK		Created
 *	15-Dec-2004 SFK Pick up status type from general lists in GetDirEventTypeStr
===========================================================================*/

static char *ppszLabels[] = {"RAD-Event ID",
							 "RAD-Start Timestamp",
							 "RAD-End TimeStamp",
							 "RAD-Location",
							 "RAD-Direction",
							 "RAD-Start Date/Time",
							 "RAD-End Date/Time",							 
							 "RAD-Start Date",
							 "RAD-Start Time",
							 "RAD-End Date",
							 "RAD-End Time",
							 "RAD-n/g ratio",
							 "RAD-Status",
							 "RAD-1a",
							 "RAD-1b",
							 "RAD-2a",
							 "RAD-2b",
							 "RAD-3a",
							 "RAD-3b",
							 "RAD-4a",
							 "RAD-4b"};
							 // 21 columns 
							 // max column name length = RAD-Start Timestamp = 19 chars
							 // max column data length = timestamp 0000019970502090810000 = 22           
							 
static BOOL PrintDirEventToTextDBFile(struct db_dir_event_rec *pdbDirEvent, unsigned long ulNumPrinted, unsigned long ulTotalRecs)
{   
	struct db_chan_rec dbFromChan, dbToChan;  
	CGFmtStr Msg;
	BOOL bStatus;
	char szStartTime[DT_LEN+1], szEndTime[DT_LEN+1], szStartDate[DT_LEN+1], szEndDate[DT_LEN+1];
	char szSteveTemp[STEVES_DT_LEN+1], szSteveStartDateStr[STEVES_DT_LEN+1], szSteveEndDateStr[STEVES_DT_LEN+1];
	char cSep;
	int i;
	//CMyDateTime Date;
	char szPathAndName[PATH_LEN+DOSNAME_LEN+1];

	// create and open the file if this is the first write	
 	if (ulNumPrinted == 0) {
		// 3.0.3.9 Added getting path from fac mgr
		if (!GetDirectory(glsFacNum, DATA_DIR, szPathAndName, PATH_LEN+DOSNAME_LEN+1)) return(FALSE);
		//pglFacCfg->GetFacilityDirectory(glsFacNum, szPathAndName, PATH_LEN); 
		//if ((szPathAndName[strlen(szPathAndName)-1] != '\\')) strcat(szPathAndName, "\\");
		//strcat(szPathAndName, "Data\\");
		MyStrnCat(szPathAndName, pRAD_Init->Get_Summary_Data_Filename(), DOSNAME_LEN);
       	//if ((gllpWriteDB  = new Text_Write_Database(pRAD_Init->Get_Summary_Data_Filename(), (UINT)21, (UINT)ulTotalRecs, (const char **) ppszLabels, (UINT)19, (UINT)22)) == NULL) return(FALSE);
		if ((gllpWriteDB  = new Text_Write_Database(szPathAndName, (UINT)21, (UINT)ulTotalRecs, (const char **) ppszLabels, (UINT)19, (UINT)22)) == NULL) return(FALSE);
    	if (bStatus = gllpWriteDB->Database_Created() != TRUE) {
    		RadReviewMsg(uiFILE_WRITE_ERR, (char *)pRAD_Init->Get_Summary_Data_Filename());
    		return(FALSE);	
    	}	
    }
                  
	// separator is always tab for these files                  
    cSep = '\t';  
    
    // write sequence number
    Msg.Printf("%04ld%c", ulNumPrinted, cSep);

	/* write start date/time as a timestamp */
	glMyDateTime.DATETimestampToStevesTimestampStrings(pdbDirEvent->d_dir_event_beg_time_key, szSteveTemp, szSteveStartDateStr);
	Msg.PrintfAppend("%s%c", szSteveTemp, cSep);
														
	/* write end date/time as a timestamp */
	glMyDateTime.DATETimestampToStevesTimestampStrings(pdbDirEvent->d_dir_event_end_time, szSteveTemp, szSteveEndDateStr);
	Msg.PrintfAppend("%s%c", szSteveTemp, cSep);
	
	/* write location */
	Msg.PrintfAppend("%s%c", glnaszLocationList[pdbDirEvent->us_dir_event_location],  cSep);
	
	/* write direction */
	Msg.PrintfAppend("%s%c", glnaszDirectionList[pdbDirEvent->us_dir_event_direction],  cSep);
	                     
	/* write start/end date/time in format of yyyymmdd - xx:xx:xx yyyymmdd - xx:xx:xx yyyymmdd xx:xx:xx yyyymmdd xx:xx:xx */
	glMyDateTime.DATETimestampToDateTimeStrs(pdbDirEvent->d_dir_event_beg_time_key, szStartDate, szStartTime, DTF_4DIGIT_YEAR);
	glMyDateTime.DATETimestampToDateTimeStrs(pdbDirEvent->d_dir_event_end_time, szEndDate, szEndTime, DTF_4DIGIT_YEAR);
	Msg.PrintfAppend("%s - %s%c%s - %s%c", szStartDate, szStartTime, cSep, szEndDate, szEndTime,cSep);
	Msg.PrintfAppend("%s%c%s%c%s%c%s%c", szStartDate, cSep, szStartTime, cSep, szEndDate, cSep, szEndTime,cSep);

	/* write neutron/gamma ratio */             
	if ((pdbDirEvent->d_dir_event_ratio_denominator > 0.0) &&
		(pdbDirEvent->s_dir_event_num_numerator > 0) &&
		(pdbDirEvent->s_dir_event_num_denominator > 0)) {
		double dNum =  pdbDirEvent->d_dir_event_ratio_numerator/(double)pdbDirEvent->s_dir_event_num_numerator;
		double dDenom = pdbDirEvent->d_dir_event_ratio_denominator/(double)pdbDirEvent->s_dir_event_num_denominator;
		Msg.PrintfAppend("%5.1lf%c", dNum/dDenom, cSep);
	}
	else Msg.PrintfAppend("  N/A%c", cSep);	
	
	char szEventTypeStr[MAX_LINE_LEN+1];
	GetDirEventStatusStr(pdbDirEvent, szEventTypeStr);
	if (pWin != NULL) Msg.PrintfAppend("%s%c", szEventTypeStr,cSep);	// 15-Dec-2004 SFK Added getting Event Str from GetEventTypeStr

	/* Print status */
//	switch (pdbDirEvent->us_dir_event_status) {						// event type
//		case NOT_RECONCILED_EVENT:
//			Msg.PrintfAppend("%s%c", "Unreconciled", cSep);
//			break;
			
//		case RECONCILED_EVENT:
//			Msg.PrintfAppend("%s%c", "Reconciled  ", cSep);
//			break;
			
//		case UNEXPLAINED_EVENT:
//			Msg.PrintfAppend("%s%c", "Unexplained ", cSep);
//			break;
			
//		case MISSING_DATA_EVENT:
//			Msg.PrintfAppend("%s%c", "Missing Data", cSep);
//			break;
			
//		case CHANNEL_EVENT:
//			Msg.PrintfAppend("%s%c", "Channel     ", cSep);
//			break;
			
//		default:
//			Msg.PrintfAppend("%s%c", "Unknown     ", cSep);
//			break;
//	}
	
	/* write pairs used to make decision */
	struct IDStruct TempID;
	for (i=0; i<MAX_PAIRS; i++) {
		if (pdbDirEvent->us_dir_event_from_chan[i] != 65535) {
			TempID.sChanID = pdbDirEvent->us_dir_event_from_chan[i];
			TempID.sStaID = pdbDirEvent->us_dir_event_from_sta[i];
			TempID.sStaType = pdbDirEvent->us_dir_event_from_sta_type[i];
			if (pglFacCfg->GetChannelRecord(glsFacNum, &TempID, &dbFromChan)) {
				TempID.sChanID = pdbDirEvent->us_dir_event_to_chan[i];
				TempID.sStaID = pdbDirEvent->us_dir_event_to_sta[i];
				TempID.sStaType = pdbDirEvent->us_dir_event_to_sta_type[i];
				if (!pglFacCfg->GetChannelRecord(glsFacNum, &TempID, &dbToChan)) {
					RadReviewMsg(uiCHAN_NOT_DEFINED_ERR, "PrintDirEventToTextDBFile", (int)TempID.sChanID);  // fix when put in new fac mgr
					return(FALSE);
				}
				Msg.PrintfAppend("%s%c%s%c", dbFromChan.sz_chan_short_name, cSep, dbToChan.sz_chan_short_name, cSep);
			}
			else {
				RadReviewMsg(uiCHAN_NOT_DEFINED_ERR, "PrintDirEventToTextDBFile", (int)TempID.sChanID);  // fix when put in new fac mgr
				return(FALSE);
			}
		}	
		else {
			Msg.PrintfAppend("N/A%cN/A%c", cSep, cSep);

		}
	}
	
	//pWin->Printf(Msg); // for debugging
	// actually write to the file
	bStatus = gllpWriteDB->Row_Add(Msg);
	if (bStatus != TRUE) {
		RadReviewMsg(uiFILE_WRITE_ERR, pRAD_Init->Get_Summary_Data_Filename());
		return(FALSE);
	}
	return(TRUE); 
}



/*===========================================================================
 *
 *  Name	 :  ReadAndPrintRadiationRecords
 *
 *  Purpose	 :  Scan through all radiation records in the database and
 *				output them to the specified devices (text window always,
 *				text database file always, log file only if requested).
 *
 *  Return value :  TRUE/FALSE
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK		Created
 *	08-Aug-2002	SFK		Changed to DATE
 *
===========================================================================*/
static BOOL ReadAndPrintRadiationRecords(DATE dStart, DATE dEnd, CGUI_TextWin *pWin, TextFile* pFile, unsigned short usNumPairs)
{
               
    CGFmtStr Msg1, Msg2;
	struct db_dir_event_rec dbDirEvent;
	int status;
	unsigned long ulTotalDirEventRecs = 0;  
	unsigned long ulEventsUpdate = 0;
	unsigned long ulTotalDirEventRecsPrinted = 0;
	unsigned long ulTotalDirEventRecsSearched = 0;

	TwoLineProgressBar *pTwoLineProgressBar = NULL;
	//pjm 26-Jan-2006 Changed bool to BOOL to get rid of of compiler warning
	//bool bContinue = true;
	BOOL bContinue = TRUE;

    // if log file is requested, open it now
/*	TextFile DirTextFile;
   	if (gllbLogFile) {
		DirTextFile.OpenTextFile("DirectionEventResults.txt", TRUE);
		if (DirTextFile.GetCancelStatus()) {
			gllbLogFile = FALSE;
		}
		else {
			if (!DirTextFile.GetOpenStatus()) {
				RadReviewMsg(uiFILE_WRITE_ERR, DirTextFile.GetFilename());
    			return(FALSE);
    		}	
		}
	}*/
	
   /* ------------------------------------------------------------------
    *	Figure out total number of qualifying records in the db
    * ----------------------------------------------------------------*/
    status = ScanDirEventRecs(dStart, FIRST, &dbDirEvent);
    if (status != TRUE) return(FALSE);			   /* unexpected db error */
	do {
		if (dbDirEvent.us_dir_event_num_pairs >= usNumPairs) ulTotalDirEventRecs++;
		status = ScanDirEventRecs(dStart, NEXT, &dbDirEvent);  
	} while (status == TRUE);	
	if (status == uiDB_BAD_ERR) return(FALSE);			   /* unexpected db error */

	Msg1.Printf("Printing matching events in database.");
	Msg2.Printf("Searched %ld of %ld events", ulTotalDirEventRecsSearched, ulTotalDirEventRecs);
	pTwoLineProgressBar = new TwoLineProgressBar(ulTotalDirEventRecs, ulTotalDirEventRecs/20, Msg1, Msg2, "Radiation Review: Event Search Progress");
	if (pTwoLineProgressBar) {
	
	   /* ------------------------------------------------------------------
		*   Print all event summaries to text window and to text db and to
		*	log file if requested.                                                     
		* ----------------------------------------------------------------*/                          
		if (ScanDirEventRecs(dStart, FIRST, &dbDirEvent) != TRUE) return(FALSE);			   /* no events in db */
    
		do {
			// only print those that have enough matching pairs                                                               
			if (dbDirEvent.us_dir_event_num_pairs >= usNumPairs) {
				// print to text window

				PrintDirEventToTextWin(&dbDirEvent, pWin, ulTotalDirEventRecsPrinted);
					
				// print to text DB file for transfer to IR if requested		
				if (gllbWriteIrFile) {
					if (!PrintDirEventToTextDBFile(&dbDirEvent, ulTotalDirEventRecsPrinted, ulTotalDirEventRecs)) return(FALSE);
				}
					
				// if requested, print to log file
				if (gllbLogFile) {
					PrintDirEventToTextFile(&dbDirEvent, pFile, ulTotalDirEventRecsPrinted);	
				}
							
				// count the event
				ulTotalDirEventRecsPrinted++;		    
			}                             
			
			Msg2.Printf("Searched %ld out of %ld events", ulTotalDirEventRecsPrinted, ulTotalDirEventRecs);
			bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);

			// get the next event in the database			    
			status = ScanDirEventRecs(dStart, NEXT, &dbDirEvent);  
		} while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < glldEnd) && bContinue);	

		pTwoLineProgressBar->Close();
		delete pTwoLineProgressBar;
	}

	// finished writing to the text database file so close it and delete the object
	if (gllpWriteDB != NULL) {
		gllpWriteDB->Database_Close();
		delete gllpWriteDB;
		gllpWriteDB = NULL;
	}

	if (bContinue) 
	{
		pWin->Printf("\nSearch successfully completed.");
		if (gllbLogFile)
			pFile->WriteLineToTextFile("\nSearch successfully completed.");
	}
	else 
	{
		pWin->Printf("\nSearch cancel by user.");
		if (gllbLogFile)
			pFile->WriteLineToTextFile("\nSearch cancel by user.");
	}
	CGFmtStr Str;
	Str.Printf("\nTotal Direction Events Found = %lu\n", ulTotalDirEventRecsPrinted);		    			
	pWin->Printf(Str);
	if (gllbLogFile)
		pFile->WriteLineToTextFile(Str);
//	pWin->Printf("\nTotal Direction Events Found = %lu\n", ulTotalDirEventRecsPrinted);		    			
	if (!bContinue) 
		return(FALSE);
	return(TRUE);
}

/*===========================================================================
 *
 *  Name	 :  WriteDirEventToDb
 *
 *  Purpose	 :  Write a new record or update the existing record to the
 *				database describing the direction event.
 *
 *  Return value :  TRUE/FALSE
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK		Created
 *
===========================================================================*/

// Note 6/19/2003:  This routine doesn't work right when trying to pull together multiple pairs of
// events.  It really should look for a matching event in the db with the same group time -- not the
// same beginning time.  This will be fixed when the Sequence Analyzer is added for Rokkasho.
static BOOL WriteDirEventToDB(struct db_event_rec *pFromEvent, struct db_event_rec *pToEvent, struct db_dir_event_rec *pdbDirEvent, unsigned int uiRatioType)
{

	int status;
	int m;
	
	// check if this rad event is already in the db
	status = FindDirEventRec(pdbDirEvent);
	if (status == uiSUMMARY_NOT_FOUND_STATUS) { //must create a new summary
		pdbDirEvent->us_dir_event_status = NOT_RECONCILED_EVENT; 	// the event is unreconciled

		// fill in the pair information 
		for (m=0; m<MAX_PAIRS; m++) { // indicate no pairs or stations
			pdbDirEvent->us_dir_event_from_chan[m] = 65535;
			pdbDirEvent->us_dir_event_from_sta[m] = 65535;
			pdbDirEvent->us_dir_event_from_sta_type[m] = 65535;
			pdbDirEvent->us_dir_event_to_chan[m] = 65535;
			pdbDirEvent->us_dir_event_to_sta[m] = 65535;
			pdbDirEvent->us_dir_event_to_sta_type[m] = 65535;
			pdbDirEvent->us_dir_event_sta[m] = 65535;
			pdbDirEvent->d_dir_event_pair_int_sum[m] = 65535;
			pdbDirEvent->d_dir_event_pair_max_val_sum[m] = 65535;
		}	

		pdbDirEvent->us_dir_event_from_chan[0] = pFromEvent->us_event_chan;	// fill in first pair
		pdbDirEvent->us_dir_event_from_sta[0] = pFromEvent->us_event_sta;
		pdbDirEvent->us_dir_event_from_sta_type[0] = pFromEvent->us_event_sta_type;

		pdbDirEvent->us_dir_event_to_chan[0] = pToEvent->us_event_chan;	// fill in first pair
		pdbDirEvent->us_dir_event_to_sta[0] = pToEvent->us_event_sta;
		pdbDirEvent->us_dir_event_to_sta_type[0] = pToEvent->us_event_sta_type;

		pdbDirEvent->us_dir_event_sta[0] = pToEvent->us_event_sta;
		pdbDirEvent->us_dir_event_num_pairs = 1;
		
		// fill in the integral sums and numerator and denominator sums if required.
		pdbDirEvent->s_dir_event_num_numerator = 0;
		pdbDirEvent->s_dir_event_num_denominator = 0;
		pdbDirEvent->d_dir_event_ratio_numerator = 0;
		pdbDirEvent->d_dir_event_ratio_denominator = 0;
		pdbDirEvent->d_dir_event_pair_int_sum[0] = pFromEvent->d_event_integral_sum + pToEvent->d_event_integral_sum;
		if (uiRatioType == iNUMERATOR) {
			pdbDirEvent->d_dir_event_ratio_numerator = pdbDirEvent->d_dir_event_pair_int_sum[0];
			pdbDirEvent->s_dir_event_num_numerator = 2;
		}	
		if (uiRatioType == iDENOMINATOR) {
			pdbDirEvent->d_dir_event_ratio_denominator = pdbDirEvent->d_dir_event_pair_int_sum[0];
			pdbDirEvent->s_dir_event_num_denominator = 2;
		}	
		pdbDirEvent->d_dir_event_pair_max_val_sum[0] = pFromEvent->d_event_max_value + pToEvent->d_event_max_value;			

		if (CreateDirEventRec(pdbDirEvent) != TRUE) return(FALSE);	
	}
	else if (status == TRUE) { // know time, location and direction match 
		if (ReadDirEvent(pdbDirEvent) != TRUE) return(FALSE); // read existing event and add to it

		// update the pairs
		for (m=0; m<MAX_PAIRS; m++) {
			if ((pdbDirEvent->us_dir_event_from_chan[m] == pFromEvent->us_event_chan) &&
				(pdbDirEvent->us_dir_event_to_chan[m] == pToEvent->us_event_chan))
				break;      // already recorded

			if ((pdbDirEvent->us_dir_event_to_chan[m] == 65535) &&  // look for empty pair
				(pdbDirEvent->us_dir_event_from_chan[m] == 65535)) {

				pdbDirEvent->us_dir_event_from_chan[m] = pFromEvent->us_event_chan;	// fill in first pair
				pdbDirEvent->us_dir_event_from_sta[m] = pFromEvent->us_event_sta;
				pdbDirEvent->us_dir_event_from_sta_type[m] = pFromEvent->us_event_sta_type;

				pdbDirEvent->us_dir_event_to_chan[m] = pToEvent->us_event_chan;	// fill in first pair
				pdbDirEvent->us_dir_event_to_sta[m] = pToEvent->us_event_sta;
				pdbDirEvent->us_dir_event_to_sta_type[m] = pToEvent->us_event_sta_type;

				pdbDirEvent->us_dir_event_sta[m] = pToEvent->us_event_sta;
				pdbDirEvent->us_dir_event_num_pairs = m + 1;

				pdbDirEvent->d_dir_event_pair_int_sum[m] = pFromEvent->d_event_integral_sum + pToEvent->d_event_integral_sum;
				if (uiRatioType == iNUMERATOR) {
					pdbDirEvent->d_dir_event_ratio_numerator += pdbDirEvent->d_dir_event_pair_int_sum[m];
					pdbDirEvent->s_dir_event_num_numerator = pdbDirEvent->s_dir_event_num_numerator + 2;
				}	
				if (uiRatioType == iDENOMINATOR) {
					pdbDirEvent->d_dir_event_ratio_denominator += pdbDirEvent->d_dir_event_pair_int_sum[m];
					pdbDirEvent->s_dir_event_num_denominator = pdbDirEvent->s_dir_event_num_denominator + 2;
				}	
				pdbDirEvent->d_dir_event_pair_max_val_sum[m] = pFromEvent->d_event_max_value + pToEvent->d_event_max_value;			
				
				// update the event end time if one of this pair is later
				if (pToEvent->d_event_end_time > pdbDirEvent->d_dir_event_end_time) {
					pdbDirEvent->d_dir_event_end_time = pToEvent->d_event_end_time;
				}                     
				if (pFromEvent->d_event_end_time > pdbDirEvent->d_dir_event_end_time) {
					pdbDirEvent->d_dir_event_end_time = pFromEvent->d_event_end_time;
				}                     
				break;
			}
		}
		if (UpdateDirEvent(pdbDirEvent) != TRUE) return(FALSE);	
	}
	else return(FALSE);
	return(TRUE);
}


/*===========================================================================
 *
 *  Name	 :  OnlyDialogBox
 *
 *  Purpose	 :  Display the dialog box and get parameters for analysis
 *
 *  Return value :  TRUE/FALSE
 *
 *  Implicit Input : 
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK		Created
 *
===========================================================================*/
static BOOL OnlyDialogBox(BOOL bSetupOnly)
{                                 
	// these parameters remain unchanged for each execution of the menu option                                                    
	static BOOL bFirst = TRUE;
	//static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = {""};
	static DATE dLastSearchIntervalInDays = 0.0;

    	
	const char *FileTypes[] = {"*.txt", NULL};

    GUI_ACTION DlgReturn;                                     
    CGUI_Table *pTbl; 
	CGFmtStr szDbInterval;    
	
	int status;     
	int i;    


    short sNumDirs;
    static unsigned long  ulDbIntervalInDays;
	DATE dSearchIntervalInDays;
	char szFirstDbDate[DT_LEN+1];              
	char szLastDbDate[DT_LEN+1];              
	//CMyDateTime Date;
	char szFacName[DB_NAME_LEN+1];
	

	// verify the facility is defined
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) return(FALSE);
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return(FALSE);

	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);

	// always get the current range of data in the db for display
	status = GetDatabaseIntervalParameters(szFirstDbDate, szLastDbDate, &ulDbIntervalInDays, &dSearchIntervalInDays);
	if (status != TRUE) return(FALSE);
	if (dSearchIntervalInDays <= 0.0) {
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return(FALSE);
	}
	
	if ((bFirst == TRUE) || (!SameDb(glszOldDbName)) || (dSearchIntervalInDays != dLastSearchIntervalInDays) || (gllsFacNum != glsFacNum)) { //either first time or using different database
		ReadDefaultDbParameters();
		GetReviewPeriodParameters(&glldUserIntervalStart, &glldEnd, &gllulUserInterval);
		glliUserUnits = 0;

		// remember the current conditions for future entries to this option
		gllsFacNum = glsFacNum;
	    GetDbName(glszOldDbName);								
		dLastSearchIntervalInDays = dSearchIntervalInDays;	
		bFirst = FALSE;
	}

		    
   /* ------------------------------------------------------------------
    *	Define the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg AnalyzeDlg(IDD_TOOLS_RAD_ANALYSIS, NULL, DLG_POSN_NO_SAVE);
	if (AnalyzeDlg.IsValid()) {
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/

		AnalyzeDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		AnalyzeDlg.DefinePushBtn(IDOK, GUI_OK);                   
		AnalyzeDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateDbParms);  		       
		
		AnalyzeDlg.DefineFldStaticTxt(IDC_ANLZ_FACILITY);
		AnalyzeDlg.FldStaticTxtSetValue(IDC_ANLZ_FACILITY, szFacName);
		
		AnalyzeDlg.DefineFldStaticTxt(IDC_ANLZ_CURR_DB_START);
		AnalyzeDlg.FldStaticTxtSetValue(IDC_ANLZ_CURR_DB_START, szFirstDbDate);
		
		AnalyzeDlg.DefineFldStaticTxt(IDC_ANLZ_CURR_DB_END);
		AnalyzeDlg.FldStaticTxtSetValue(IDC_ANLZ_CURR_DB_END, szLastDbDate);

		szDbInterval.Printf("%ld days", ulDbIntervalInDays);  		    		
		AnalyzeDlg.DefineFldStaticTxt(IDC_ANLZ_CURR_DB_INTERVAL);
		AnalyzeDlg.FldStaticTxtSetValue(IDC_ANLZ_CURR_DB_INTERVAL, szDbInterval);

		glMyDateTime.DATETimestampToGenStructs(glldUserIntervalStart, &gllGenDate, &gllGenTime);
		AnalyzeDlg.DefineFldDate(IDC_ANLZ_SRCH_STRTDATE, &gllGenDate);
		AnalyzeDlg.DefineFldTime(IDC_ANLZ_SRCH_STRTTIME, &gllGenTime);
		
		AnalyzeDlg.DefineFldNum(IDC_ANLZ_SRCH_INTERVAL, &gllulUserInterval, 1, 10000,"%ld");
		AnalyzeDlg.DefinePickList(IDC_ANLZ_INTRVL_UNITS_PL, glnaszIntervalUnitsPicklist, (unsigned int *)&glliUserUnits);

		// 6/19/2003 allow only one matching pair until fix this in Rokkasho Sequence Analyzer
		AnalyzeDlg.DefineFldNum(IDC_RAD_EVENT_SRCH_PAIRS, &gllusPairs, 1, 1,"%u");

		gllbLogFile = FALSE;
		AnalyzeDlg.DefineCheckBox(IDC_RAD_EVENT_SRCH_LOG_FILE, &gllbLogFile);
		AnalyzeDlg.DefineCheckBox(IDC_RAD_ANLZ_CLEAR_ALL, &gllbClearAll);
		gllbWriteIrFile = pRAD_Init->Get_WriteDirEventResults();
		AnalyzeDlg.DefineCheckBox(IDC_ANLZ_SRCH_FILE, &gllbWriteIrFile);
		
		AnalyzeDlg.DefineTable(IDC_ANLZ_PICKTABLE, &pTbl);	
		if (pTbl != NULL) {
			glluiCheckBoxCol = pTbl->AddColCheckBox("Use", glluiNumRows, pgllSelect);
			glluiFromPickListCol = pTbl->AddColPickList("From", glluiNumRows, pglluiFromValues, aszChanList);
			glluiToPickListCol = pTbl->AddColPickList("To", glluiNumRows, pglluiToValues, aszChanList);			
			glluiTypePickListCol = pTbl->AddColPickList("Type", glluiNumRows, pglluiTypeValues, gllnaszTypeList);			
			glluiDirectionPickListCol = pTbl->AddColPickList("Dir", glluiNumRows, pglluiDirValues, glnaszDirectionList);			
			glluiLocationPickListCol = pTbl->AddColPickList("Loc", glluiNumRows, pglluiLocValues, glnaszLocationList);			
			glluiMinCol = pTbl->AddColNum("Min s", glluiNumRows, pglldMinValues, 0., 65535., "%5.0lf");			
			glluiMaxCol = pTbl->AddColNum("Max s", glluiNumRows, pglldMaxValues, 0., 65535., "%5.0lf");			
			glluiRatioPickListCol = pTbl->AddColPickList("Ratio", glluiNumRows, pglluiRatioValues, gllnaszRatioList);			
		}
			
		AnalyzeDlg.SetFocus(IDC_ANLZ_SRCH_INTERVAL); 
		if (bSetupOnly) AnalyzeDlg.SetFocus(IDOK);
		DlgReturn = AnalyzeDlg.Go();
			    
	   /* ------------------------------------------------------------------
	    *	Returned from dialog box, get the values now.
	    * ----------------------------------------------------------------*/
	    if (DlgReturn == GUI_OK) {  
			AnalyzeDlg.RetrieveAllControlValues();	/* get dialog values */
			pTbl->RetrieveAllCellValues(); 		    /* get table values */

		   /* ------------------------------------------------------------------
		    *	Check that they selected at least 1 direction condition
		    * ----------------------------------------------------------------*/
		    sNumDirs = 0;
			for (i=0; i<glluiNumRows; i++) {
				if (pgllSelect[i] == TRUE) {
					sNumDirs++;
				}	
			}		
		    if (sNumDirs == 0) {	// cannot proceed, no direction conditions selected
		        RadReviewMsg(uiNO_DIR_SELECTED_WARN);
		    	return(FALSE);
		    }	
		    
			// if running in the setup only mode, record parameters and quit now.
			if (bSetupOnly) {
				WriteDefaultDbParameters(&AnalyzeDlg, IDD_TOOLS_RAD_ANALYSIS);
			}
		   	// check to see if there is any data in the interval for any station
			status = ConvertStructsToSearchIntervalParameters(&gllGenDate, &gllGenTime, glliUserUnits, gllulUserInterval, &glldUserIntervalStart, &glldEnd);
			if (status != TRUE) {	// no data for any station in the interval
				RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, glldUserIntervalStart, glldEnd, "any station");
				return(FALSE);
			}
	    	return(TRUE);
	    }	
	}
   	return(FALSE);
}
                            
                            

/*===========================================================================
 *
 *  Name	 :  AnalyzeDoIt
 *
 *  Purpose	 :  Perform the direction analysis using parameters set
 *				in the dialog box.  Basically find all channel events
 *				whose max peak times occur in the same order as specified
 *				and whose max peak times are within the min/max range of
 *				each other.
 *
 *  Return value :  TRUE/FALSE
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK		Created
*  20-Aug-1999 SFK	  Version 2.05	Fixed columns so they line up correctly
 * 27-Mar-2003 SFK		Changed algorithm to use specified "edges" in the analysis
 *						rather than always using midpoints.
===========================================================================*/
static BOOL DoIt()
{                                 
    struct db_chan_rec dbChan;  
    struct db_event_rec *pFromEvents = NULL;
    struct db_event_rec *pToEvents = NULL;
    struct db_dir_event_rec dbDirEvent;
        
    CGFmtStr Msg1, Msg2, TempStr;
        
	int status;
	int i, j, k;  
	DATE dFirstDbDay, dLastDbDay;

    unsigned long ulNumFromEvents, ulNumToEvents, ulFromEventsRead, ulToEventsRead;
    unsigned long ulEventsProcessed,  ulTotalNumEvents;
	BOOL bDataExists;
	BOOL bEventFound;
    BOOL bFirst;
	struct IDStruct FromID, ToID, TempID;
	TwoLineProgressBar *pTwoLineProgressBar = NULL;
	//pjm 26-Jan-2006 Changed bool to BOOL to get rid of of compiler warning
	//bool bContinue = true;
	BOOL bContinue = TRUE;
    
    SetCurrentDrive();

	// check to see if there is any data in the interval
	TempID.sStaID = ANY_STATION;
	TempID.sStaType = ANY_TYPE;
	GetDbTimeRange(TempID, &dFirstDbDay, &dLastDbDay);
	if ((glldUserIntervalStart <= dLastDbDay) && (glldEnd >= dFirstDbDay)) {
	}
	else {
		RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, glldUserIntervalStart, glldEnd, "any station");
		return(FALSE);
	}
	
   /* ------------------------------------------------------------------
	* Calculate the total number of channel events that will be
	* examined - use in the progress bar display
    * ----------------------------------------------------------------*/
    ulTotalNumEvents = 0;	
	for (i=0; i<glluiNumRows; i++) {
		if (pgllSelect[i] == TRUE) { 
			pglFacCfg->GetChannelIDForPicklistChoice(pglluiFromValues[i], &FromID);
			GetDataStatisticsForEventsByChannel(FromID, glldUserIntervalStart, glldEnd, &ulNumFromEvents, ALL_EVENTS); 
			ulTotalNumEvents+=ulNumFromEvents;
		}
	}		
			
   /* ------------------------------------------------------------------
    *	If no chan events in interval, then quit now.
    * ----------------------------------------------------------------*/
	if (ulTotalNumEvents == 0) {
		RadReviewMsg(uiNO_MATCHING_CHAN_EVENTS_WARN, glldUserIntervalStart, glldEnd);
		return(TRUE);
	}	

   /* ------------------------------------------------------------------
    *	Delete the event summaries, if requested
    * ----------------------------------------------------------------*/
    if (gllbClearAll == TRUE) {
    	status = DeleteDbDirEvents(gllsFacNum);
    	if (status != TRUE) {
    		RadReviewMsg(uiDB_BAD_ERR, "DlgDirection DoIt");
    		return(FALSE);
    	}	
    }	
			                             
   /* ------------------------------------------------------------------
	*   Create the dialog box that is used to show analysis progress for this station
	* ----------------------------------------------------------------*/
	ulEventsProcessed = 0;
	Msg1.Printf("Printing matching events in database.");
	Msg2.Printf("Searched %ld of %ld events", ulEventsProcessed, ulTotalNumEvents);
	pTwoLineProgressBar = new TwoLineProgressBar(ulTotalNumEvents, ulTotalNumEvents/20, Msg1, Msg2, "Radiation Review: Event Search Progress");
	if (pTwoLineProgressBar) {

	   /* ------------------------------------------------------------------
		* Sort through the pairs of channels selected and look for both members
		* of the pair.  If find them, then fill in an event summary or add to an
		* existing event summary.
		* ----------------------------------------------------------------*/
		bDataExists = FALSE;
		i = 0;
		for (i=0; i<glluiNumRows; i++) {
			if (pgllSelect[i] == TRUE) { 
				pglFacCfg->GetChannelIDForPicklistChoice(pglluiFromValues[i], &FromID);
				pglFacCfg->GetChannelIDForPicklistChoice(pglluiToValues[i], &ToID);
				if (GetDataStatisticsForEventsByChannel(FromID, glldUserIntervalStart, glldEnd, &ulNumFromEvents, ALL_EVENTS) != TRUE) continue;
				if (GetDataStatisticsForEventsByChannel(ToID, glldUserIntervalStart, glldEnd, &ulNumToEvents, ALL_EVENTS) != TRUE) continue;
						
				if ((ulNumFromEvents > 0) && (ulNumToEvents > 0)) {
					// Update which channel analyzing in progress dialgo
					pglFacCfg->GetChannelRecord(glsFacNum, &FromID, &dbChan);
					Msg1.Printf("Processing channel %s.\n", dbChan.sz_chan_name);
					
					// Group the events using the min max for this pair
					status = GroupChanEvents(glldUserIntervalStart, glldEnd, (unsigned int)pglldMinValues[i], (unsigned int)pglldMaxValues[i], (short)pglluiTypeValues[i]); 
					if (status != TRUE) {
						RadReviewMsg(status); // could be uiNO_MATCHING_CHAN_EVENTS_WARN, 			
						bContinue = false;
						break;
					}	
					
					// allocate storage for channel events to analyze 
					pFromEvents = (struct db_event_rec *)malloc((ulNumFromEvents+1) * sizeof(struct db_event_rec));
					if (pFromEvents == NULL) {
						RadReviewMsg(uiMEMORY_ALLOCATION_ERR);			
						bContinue = false;
						break;

					}	
					pToEvents = (struct db_event_rec *)malloc((ulNumToEvents+1) * sizeof (struct db_event_rec));
					if (pToEvents == NULL) {
						RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
						bContinue = false;
						break;

					}	
							
					/* read the channel events from the db */
					if (GetDataByChannelEvents(FromID, glldUserIntervalStart, glldEnd, ulNumFromEvents, &ulFromEventsRead, pFromEvents, ALL_EVENTS) != TRUE) {
						bContinue = false;
						break;
					}
					if (GetDataByChannelEvents(ToID, glldUserIntervalStart, glldEnd, ulNumToEvents, &ulToEventsRead, pToEvents, ALL_EVENTS)!= TRUE) {
						bContinue = false;
						break;
					}
						
					/* determine whether have direction event or not */			                                 
					j = 0;
					k = 0;
					do {
						// Look for the first case where the To event is larger than the From event
						while ((j < (int)ulToEventsRead) &&
							   (pToEvents[j].d_event_grp_time < pFromEvents[k].d_event_grp_time)) {
							j++;
						}                              
						
						// For the case where you have multiple To Events belonging to the same group
						// find the last To in the group.
						while ((j < (int)ulToEventsRead -1) &&
							(pToEvents[j].d_event_grp_time == pToEvents[j+1].d_event_grp_time)) {
							j++;
						} 
						
						if (j == (int)ulToEventsRead) break;
						
						//	based on the type (start to start, end to end, start to end, end to start,
						//	max to max) decide whether this pair of events meets the conditions.
						bEventFound = FALSE;
						if (pglluiTypeValues[i] == START_TO_START) {
							if ((pToEvents[j].d_event_grp_time == pFromEvents[k].d_event_grp_time) &&
								(pFromEvents[k].d_event_beg_time_key <= pToEvents[j].d_event_beg_time_key)) {
								bEventFound = TRUE;
							}
						}
						if (pglluiTypeValues[i] == START_TO_END) {
							if ((pToEvents[j].d_event_grp_time == pFromEvents[k].d_event_grp_time) &&
								(pFromEvents[k].d_event_beg_time_key <= pToEvents[j].d_event_end_time)) {
								bEventFound = TRUE;
							}
						}
						if (pglluiTypeValues[i] == END_TO_END) {
							if ((pToEvents[j].d_event_grp_time == pFromEvents[k].d_event_grp_time) &&
								(pFromEvents[k].d_event_end_time <= pToEvents[j].d_event_end_time)) {
								bEventFound = TRUE;
							}
						}
						if (pglluiTypeValues[i] == END_TO_START) {
							if ((pToEvents[j].d_event_grp_time == pFromEvents[k].d_event_grp_time) &&
								(pFromEvents[k].d_event_end_time <= pToEvents[j].d_event_beg_time_key)) {
								bEventFound = TRUE;
							}
						}
						if (pglluiTypeValues[i] == MAX_TO_MAX) {
							if ((pToEvents[j].d_event_grp_time == pFromEvents[k].d_event_grp_time) &&
								(pFromEvents[k].d_event_max_time <= pToEvents[j].d_event_max_time)) {
								bEventFound = TRUE;
							}
						}

						if (bEventFound == TRUE) {
							bDataExists = TRUE;
							
							// determine the first start time of the pair of chan events
							if (pToEvents[j].d_event_beg_time_key < pFromEvents[k].d_event_beg_time_key) {
								dbDirEvent.d_dir_event_beg_time_key = pToEvents[j].d_event_beg_time_key;
							}
							else {
								dbDirEvent.d_dir_event_beg_time_key = pFromEvents[k].d_event_beg_time_key;
							}
							// determine the last end time of the pair of chan events
							if (pToEvents[j].d_event_end_time > pFromEvents[k].d_event_end_time) {
								dbDirEvent.d_dir_event_end_time = pToEvents[j].d_event_end_time;
							}
							else {
								dbDirEvent.d_dir_event_end_time = pFromEvents[k].d_event_end_time;
							}

							// fill in location and direction
							dbDirEvent.us_dir_event_location = pglluiLocValues[i];
							dbDirEvent.us_dir_event_direction = pglluiDirValues[i];
							
							if (!WriteDirEventToDB(&pFromEvents[k], &pToEvents[j], &dbDirEvent, pglluiRatioValues[i])) {
								bContinue = false;
								break;
							}
						}	
							
						
					   /* ------------------------------------------------------------------
						*	Update the progress bar
						* ----------------------------------------------------------------*/
						ulEventsProcessed++;
						Msg2.Printf("Searched %ld out of %ld events", ulEventsProcessed, ulTotalNumEvents);
						bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
						k++;
						if (glImportExit == TRUE) bContinue = false;
					} while ((k<(int)ulFromEventsRead) && bContinue);
				  //  CGUI_App::ProcessEvents();                /* update the output page */
					if (pFromEvents) free (pFromEvents);
					if (pToEvents) free(pToEvents);
				} 
			}       
			if (!bContinue) break;
		}
	}	

	pTwoLineProgressBar->Close();
	delete pTwoLineProgressBar;
	pTwoLineProgressBar = NULL;
			    
   /* ------------------------------------------------------------------
    *	If no direction events found in interval, then quit now.
    * ----------------------------------------------------------------*/
	if (bDataExists == FALSE) {
		RadReviewMsg(uiNO_MATCHING_DIR_EVENTS_WARN, glldUserIntervalStart, glldEnd);
		return(TRUE);
	}	
	
   /* ------------------------------------------------------------------
    *	Changed the database, open and close it for stability
    * ----------------------------------------------------------------*/
	if (status == TRUE) { 
		SaveDatabaseNow();
	}	
	
	if (!bContinue) return(FALSE);  
	
   /* ------------------------------------------------------------------
    *	Open up the text window where the events will be displayed
    * ----------------------------------------------------------------*/
	if (pWin == NULL) {                
		pWin = new CGUI_TextWin("Direction Events Found");
		if (pWin != NULL) pWin->ShowStatusBar(TRUE);
	}   
	else {
		pWin->Clear();
	}
	pWin->SetFocus();
			
   /* ------------------------------------------------------------------
	*   Display the general part of the header:
	*	facility name,
	*	search interval
	*	conditions used
	* ----------------------------------------------------------------*/ 
	CGFmtStr StrHdr, StrTmp;
	if (!PrintFacilityHeader(&StrHdr, gllsFacNum, glldUserIntervalStart, glldEnd)) return(FALSE);
//	if (!PrintFacilityHeader(pWin, gllsFacNum, glldUserIntervalStart, glldEnd)) return(FALSE);
	StrTmp.Printf("\nPairs that must agree: %d\nDirection Search Criteria:\n     From       To         Dir    Loc   Min   Max Type", gllusPairs);
	StrHdr += StrTmp;
//    pWin->Printf("\nPairs that must agree: %d", gllusPairs);
//    pWin->Printf("\nDirection Search Criteria:");
//    pWin->Printf("\n     From       To         Dir    Loc   Min   Max Type");
    for (i=0; i<glluiNumRows; i++) 
	{
		if (pgllSelect[i] == TRUE) 
		{ 
			pglFacCfg->GetChannelIDForPicklistChoice(pglluiFromValues[i], &FromID);
			pglFacCfg->GetChannelRecord(glsFacNum, &FromID, &dbChan);
			StrTmp.Printf("\n     %-10s ", dbChan.sz_chan_short_name);
			StrHdr += StrTmp;
///			pWin->Printf("\n     %-10s ", dbChan.sz_chan_short_name);
			pglFacCfg->GetChannelIDForPicklistChoice(pglluiToValues[i], &ToID);
			pglFacCfg->GetChannelRecord(glsFacNum, &ToID, &dbChan);
			StrTmp.Printf("%-10s %s %s %5.0lf %5.0lf %s ", \
				dbChan.sz_chan_short_name,glnaszDirectionList[pglluiDirValues[i]],glnaszLocationList[pglluiLocValues[i]],pglldMinValues[i],pglldMaxValues[i],gllnaszTypeList[pglluiTypeValues[i]]);
			StrHdr += StrTmp;
//			pWin->Printf("%-10s ", dbChan.sz_chan_short_name);
//			pWin->Printf("%s ", glnaszDirectionList[pglluiDirValues[i]]);
//			pWin->Printf("%s ", glnaszLocationList[pglluiLocValues[i]]);
//			pWin->Printf("%5.0lf ", pglldMinValues[i]);
//			pWin->Printf("%5.0lf ", pglldMaxValues[i]);
//			pWin->Printf("%s ", gllnaszTypeList[pglluiTypeValues[i]]);
		}	
	}
	StrTmp.Printf("\nRatio = (");
	StrHdr += StrTmp;
//	pWin->Printf("\nRatio = (");
	bFirst = TRUE;
    for (i=0; i<glluiNumRows; i++) {
		if ((pgllSelect[i] == TRUE) && pglluiRatioValues[i] == iNUMERATOR) { 
			pglFacCfg->GetChannelIDForPicklistChoice(pglluiFromValues[i], &FromID);
			pglFacCfg->GetChannelRecord(glsFacNum, &FromID, &dbChan);
			if (bFirst == FALSE) 
			{
				StrTmp.Printf(" + ");
				StrHdr += StrTmp;
//				pWin->Printf(" + ");
			}
			StrTmp.Printf("%s + ", dbChan.sz_chan_short_name);
			StrHdr += StrTmp;
//			pWin->Printf("%s + ", dbChan.sz_chan_short_name);
			bFirst = FALSE;
			pglFacCfg->GetChannelIDForPicklistChoice(pglluiToValues[i], &ToID);
			pglFacCfg->GetChannelRecord(glsFacNum, &ToID, &dbChan);
			StrTmp.Printf("%s ", dbChan.sz_chan_short_name);
			StrHdr += StrTmp;
//			pWin->Printf("%s ", dbChan.sz_chan_short_name);
		}	
	}	
	bFirst = TRUE;
	StrTmp.Printf(")/(");
	StrHdr += StrTmp;
//	pWin->Printf(")/(");
	for (i=0; i<glluiNumRows; i++) {
		if ((pgllSelect[i] == TRUE) && pglluiRatioValues[i] == iDENOMINATOR) { 
			pglFacCfg->GetChannelIDForPicklistChoice(pglluiFromValues[i], &FromID);
			pglFacCfg->GetChannelRecord(glsFacNum, &FromID, &dbChan);
			if (bFirst == FALSE) 
			{
				StrTmp.Printf(" + ");
				StrHdr += StrTmp;
//				pWin->Printf(" + ");
			}
			StrTmp.Printf("%s + ", dbChan.sz_chan_short_name);
			StrHdr += StrTmp;
///			pWin->Printf("%s + ", dbChan.sz_chan_short_name);
			bFirst = FALSE;
			pglFacCfg->GetChannelIDForPicklistChoice(pglluiToValues[i], &ToID);
			pglFacCfg->GetChannelRecord(glsFacNum, &ToID, &dbChan);
			StrTmp.Printf("%s ", dbChan.sz_chan_short_name);
			StrHdr += StrTmp;
//			pWin->Printf("%s ", dbChan.sz_chan_short_name);
		}	
	}	
	StrTmp.Printf(")\n");
	StrHdr += StrTmp;
//    pWin->Printf(")");
//    pWin->Printf("\n");

    // if log file is requested, open it now
	TextFile DirTextFile;
   	if (gllbLogFile) {
		DirTextFile.OpenTextFile("DirectionEventResults.txt", TRUE);
		if (DirTextFile.GetCancelStatus()) {
			gllbLogFile = FALSE;
		}
		else {
			if (!DirTextFile.GetOpenStatus()) {
				RadReviewMsg(uiFILE_WRITE_ERR, DirTextFile.GetFilename());
    			return(FALSE);
    		}
			gllbLogFile = TRUE;
		}
	}
	if (pWin) 
		pWin->Printf(StrHdr);
	if (gllbLogFile) 
		DirTextFile.WriteLineToTextFile(StrHdr);
	//	Read and print all the events in the db
	status = ReadAndPrintRadiationRecords(glldUserIntervalStart, glldEnd, pWin, &DirTextFile, gllusPairs);
    if (pWin != NULL) pWin->EnableUserClose(CloseRawDataWindow);
	// close the log file if we were writing it.
	if (gllbLogFile) {
		DirTextFile.CloseTextFile();
	}
	
    return(status); 
				
}	      
   		                                 

/* <f> */
/*===========================================================================
 *
 *  Name	 :  DirectionAnalysisDialog
 *
 *  Purpose	 :  Called from the main menu to execute both the dialog box and
 *				to perform the action.
 *
 *  Return value :  TRUE/FALSE
 *
 *  Implicit Input : 
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK	Created
 *
===========================================================================*/
void DirectionAnalysisMenuOption()
{
	if (OnlyDialogBox(FALSE))
		DoIt();
}	


                                                

///////////////////////////////////////////////////////////////////////////////////////////////
//	Perform only the dialog box setup 
//	Called when started with /SETUP
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DirectionAnalysisDialogBox()
{    
return(OnlyDialogBox(TRUE));
}	
    
    
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Perform just the direction search using parameters in the default db and the ini file
//	Called when started with /IMPORT
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DirectionAnalysisDoIt()
{  
	ReadDefaultDbParameters();
	GetReviewPeriodParameters(&glldUserIntervalStart, &glldEnd, &gllulUserInterval); // either db limits or from .ini
	glliUserUnits = 0;

	return(DoIt());
}	


