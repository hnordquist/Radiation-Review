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
//    File Name : RadError.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include <fstream.h>
#include <gen.h>
#include "RAD.H"                             
#include "DbDay.h"
#include "DbGeneralDbSupport.h"
#include "MyDateTime.h"
#include "RadError.h"

extern void CloseDataGraph(void);
extern CMyDateTime glMyDateTime;

static int RAD_Unhandled_Errors(unsigned int uiError, CGFmtStr& szStr);

/////////////////////////////////////////////////////////////////////////////////////
//  Error handling functions
/////////////////////////////////////////////////////////////////////////////////////

void RadReviewMsg(unsigned int uiError) 
{
	CGFmtStr szAbortMsg;
	CGFmtStr szErrorMsg;
	CGFmtStr szWarnMsg;
	//int iTemp;
		
	switch(uiError) { 
		case uiDB_BAD_ERR :
			szErrorMsg.Printf("Database error.\nTry clearing database and reimporting the data.");
			break;
			
		case uiDB_EMPTY_STATUS :
			szErrorMsg.Printf("The database is empty or facility has no RAD stations.\nPlease import data or change facilities.");
			break;

		case uiDEFAULT_CANNOT_BE_DELETED:
			szWarnMsg.Printf("Graph configuration \"default\" cannot be deleted.");
			break;

		case uiDEFAULT_CFG_FILE_ERR:
			szErrorMsg.Printf("Fatal error reading the default.cfg file.  Please fix it.");
			break;

		case uiDISPLAY_AT_LEAST_ONE_CHANNEL_ERR:
			szErrorMsg.Printf("At least one channel\nmust be enabled.");
			break;

		case uiEVENT_NOT_FOUND_WARN:
			szWarnMsg.Printf("Couldn't find event to update.  Nothing will be changed in database.");
			break;
			
		case uiFAC_MGR_CHANGE_INFO:
			szWarnMsg.Printf("If you have added or deleted a station in a facility,\nyou must re-import the data.");
			break;

		case uiINVALID_DFLT_DB_FIELD_ERR:
			szErrorMsg.Printf("Requested an invalid field from the default database.  The option will abort.");
			break;

		case uiMEMORY_ALLOCATION_ERR :
			szErrorMsg.Printf("Memory allocation error: Option will abort.  If possible, select a smaller interval.");
			break;

		case uiNAMES_CFG_READ_ERR:
			szErrorMsg.Printf("Database cleared okay, but could not read .cfg file.");
			break;

		case uiNO_CHANS_SELECTED_WARN:
			szWarnMsg.Printf("No channels were selected.  The option will abort.");
			break;

		case uiNO_CHANNEL_STRING_MATCH_SUPPORTED_ERR:
			szErrorMsg.Printf("Matching to a channel string is not supported. The option will abort.");
			break;          

		case uiNO_DIR_SELECTED_WARN:
			szWarnMsg.Printf("No direction analysis conditions were selected.  The option will abort.");
			break;

		case uiNO_FACILITIES_ERR:
			szErrorMsg.Printf("There are no facilities defined.");
			break;

		case uiINSPEC_INI_ERR:
			szErrorMsg.Printf("Hint: Check RT_INSPEC_INFO_COMMON_FILENAME in RAD.INI for valid path.");
			break;

		case uiNO_MATCHING_CHAN_EVENTS_WARN: //????
			szWarnMsg.Printf("No channel events found for any station in specified time interval.  The option will abort.");
			break;          


		case uiNO_MATCHING_DATA_WARN:
			szWarnMsg.Printf("No data found for any station in specified time interval.  The option will abort.");
			break;

		case uiNO_MATCHING_DIR_EVENTS_WARN:// ???
			szWarnMsg.Printf("No channel events meet the condition for specified direction in specified time interval.");
			break;

		case uiNO_MATCHING_RAW_DATA_WARN:
			szWarnMsg.Printf("No conditions selected for raw data display.  The option will abort.");
			break;

		case uiNO_MEAS_SELECTED_WARN:
			szWarnMsg.Printf("No measurements were selected.  No measurements will be sent to INCC.");
			break;

		case uiNO_MEAS_TYPES_SELECTED_WARN:
			szWarnMsg.Printf("No measurement types were selected.  The option will abort.");
			break;

		case uiNO_MEMORY_FOR_APP_ERR:
			szErrorMsg.Printf("Not enough memory available to begin\nthe application.  Please try to free\nmore memory and begin again.");
			break;

		case uiNO_NCC_FILE_REQUESTED_ERR :
			szErrorMsg.Printf("Requested not to write NCC file.\nTo write files, change parameter RAD_WRITE_MEASUREMENT_RESULTS_FILE \nin RAD.INI file to TRUE.");
			break;
	
		case uiNO_STAS_SELECTED_WARN:
			szWarnMsg.Printf("No stations were selected.  The option will abort.");
			break;

		case uiNO_STATIONS_FOR_FACILITY_ERR:
			szErrorMsg.Printf("There are no stations in the database for the default facility.");
			break;

		case uiNOT_ENOUGH_DATA_TO_ANALYZE_WARN:
			szWarnMsg.Printf("Not enough data in interval to analyze.\n Please select larger time interval.   Event search will abort now.");
			break;

		case uiONLY_ONE_GRAPH_WARN:
			szWarnMsg.Printf("Only one graph may be displayed.");
			break;

		case uiPEAK_WIDTH_TOO_LARGE_WARN:
			szWarnMsg.Printf("Not enough data in interval to analyze.\n Please select larger time interval or\nreduce the peak width.   Event search will abort now.");
			break;

		case uiRT_IMPORT_ERR:
			szErrorMsg.Printf("The Radiation Review Import process did not successfully complete.");
			szErrorMsg.PrintfAppend("\nThis could be due to an error during the import or");
			szErrorMsg.PrintfAppend("\nthe user selecting to cancel the process.");
			szErrorMsg.PrintfAppend("\nPress okay to continue the Import process for the other tools.");
			break;

		case uiSUMMARY_NOT_FOUND_STATUS:
			szWarnMsg.Printf("No summaries were found.");
			break;

		case uiUNKNOWN_EVENT_TYPE_ERR:
			szErrorMsg.Printf("The event type is unknown.");
			break;

		case uiWILDCARDS_TOO_LONG_ERR:
			//iTemp = WILDCARDS;
			szErrorMsg.Printf("Too many wildcards are specified, reduce the length to < %d.", WILDCARDS);//iTemp);
			break;
	                                      
		default: 
			if (!RAD_Unhandled_Errors(uiError, szErrorMsg)) {
				szWarnMsg.Printf("An unrecognizable error (%u) has occurred. (No args)", uiError);
			}
			break;			
   	} 

	if (szErrorMsg.GetDim() > 1) {
		GUI_MsgBox(szErrorMsg, GUI_ICON_ERROR);
	}	 
	
	if (szWarnMsg.GetDim() > 1) {
		GUI_MsgBox(szWarnMsg, GUI_ICON_WARN);
	}	 
	
	if (szAbortMsg.GetDim() > 1) {
		GUI_MsgBox(szAbortMsg, GUI_ICON_ERROR);
		exit(-1);
	}	 
}


void RadReviewMsg(unsigned int uiError, const char *szStr)
{
	CGFmtStr szAbortMsg;
	CGFmtStr szErrorMsg;
	CGFmtStr szWarnMsg;
	CGFmtStr szInfoMsg;
	
	int iTemp;

	switch(uiError) { 
		case uiBLANK_STRING_ERR :
			szErrorMsg.Printf("A blank string is not a valid entry.  Please further specify the %s field.", szStr);
			break;

		case uiBROWSE_INFO_WARN:
			szInfoMsg.Printf("Subfolder search is only valid for a single root path.\nThe import will continue using only paths\n %s.", szStr);
			break;
		
		case uiCANNOT_OPEN_ANY_DATABASE_ERR:
			szErrorMsg.Printf("No database at %s could be opened.  Program cannot proceed.", szStr);
			break;
		
		case uiDATABASE_OPENED_INFO:
			szInfoMsg.Printf("Database at %s is open.", szStr);
			break;

		case uiDB_BAD_ERR :
			szErrorMsg.Printf("Database error in function %s.\nTry clearing database and reimporting the data.", szStr);
			break;
		
		case uiDB_DOES_NOT_EXIST_ERR:
			szErrorMsg.Printf("A database does not exist at \n%s.", szStr);
			break;
		
		case uiDB_INIT_ERR :
			szErrorMsg.Printf("An error occurred during the initialization of %s.  Option will abort.", szStr);
			break;

		case uiDFLT_DB_READ_ERR:
			szErrorMsg.Printf("Error while reading the default parameters for %s", szStr);
			break;

		case uiDRIVE_DOES_NOT_EXIST_ERR:
			szErrorMsg.Printf("The specified path \n%s does not exist.", szStr);
			break;

		case uiFILE_OPEN_ERR:
			szErrorMsg.Printf("The specified file \n%s \ncould not be opened.", szStr);
			break;                         

		case uiFILE_READ_ERR:    	    
		    szErrorMsg.Printf("Error reading file %s.", szStr);
		    break;
			
		case uiREQUESTED_FACILTY_NOT_CURRENT_FACILTY:
			szErrorMsg.Printf("%s is not the current default facility.", szStr);
			break;

		case uiFILE_WRITE_ERR:
			szErrorMsg.Printf("Error when writing to \n%s.", szStr);
			break;                         

		case uiINTERNAL_ERR:
			szErrorMsg.Printf("Internal Error:%s\nThis should not occur, please report it to LANL.", szStr);
			break;

		case uiINVALID_FILE_TYPE:
			szWarnMsg.Printf("Warning:\nFile type %s cannot be used with this menu option.", szStr);
			break;

		case uiMAKE_DIR_ERR:
			szErrorMsg.Printf("The specified directory \n%s \ncould not be created.", szStr);
			break;

		case uiMEMORY_ALLOCATION_ERR :
			szErrorMsg.Printf("Memory allocation error for %s: Option will abort.", szStr);
			break;

		case uiNAME_TOO_LONG_ERR:
			iTemp = DOSNAME_LEN;
			szErrorMsg.Printf("The specified name \n%s \nis too long.  Maximum length is %d characters.", szStr, iTemp);
			break;

		case uiNO_ASSOCIATED_GRAPH_TRACE_ERR:
			szErrorMsg.Printf("No traces on the graph match the specified location: %s.\nPlease manually select appropriate traces now.", szStr);
			break;

		case uiPATH_DOES_NOT_EXIST_ERR:
			szErrorMsg.Printf("The specified path \n%s \ndoes not exist.", szStr);
			break;

		case uiPATH_TOO_LONG_ERR:
			iTemp = PATH_LEN;
			szErrorMsg.Printf("The specified path \n%s \nis too long.  Maximum length is %d characters.", szStr, iTemp);
			break;
		
		case uiPATHS_TOO_LONG_ERR:
			iTemp = PATHS;
			szErrorMsg.Printf("The specified paths \n%s \nare too long.  Maximum length is %d characters.", szStr, iTemp);
			break;
		
		case uiRAD_INVALID_DATABASE_PATH_ERR:
			szErrorMsg.Printf("There is not a Radiation Review database at %s.  Program will abort now.", szStr);
			break;

		case uiRAD_START_FATAL_ERR:
			szErrorMsg.Printf("A fatal error was encountered when %s \nduring the Radiation Review startup.  \nRadiation Review will abort now.", szStr);
			break;
		
		case uiWILDCARD_DOES_NOT_EXIST:
			szErrorMsg.Printf("The specified wildcard \n%s \nis not recognized.", szStr);
			break;
		
		case uiWILDCARDS_TOO_LONG_ERR:
			iTemp = WILDCARDS;
			szErrorMsg.Printf("The specified wildcards \n%s \nis too long.  Maximum length is %d characters.", szStr, iTemp);
			break;
		
		default: 
			if (!RAD_Unhandled_Errors(uiError, szErrorMsg))
			{
				szWarnMsg.Printf("An unrecognizable error (%u) has occurred.(%s)", uiError, szStr);
			}
			else
			{
				szErrorMsg = "; " + szErrorMsg;
				szErrorMsg = szStr + szErrorMsg;
			}
			break;
   	}
    	
	if (szErrorMsg.GetDim() > 1) {
		GUI_MsgBox(szErrorMsg, GUI_ICON_ERROR);
	}	 
    	
	if (szWarnMsg.GetDim() > 1) {
		GUI_MsgBox(szWarnMsg, GUI_ICON_WARN);
	}	 
	if (szAbortMsg.GetDim() > 1) {
		GUI_MsgBox(szAbortMsg, GUI_ICON_ERROR);
		exit(-1);
	}	 
	if (szInfoMsg.GetDim() > 1) {
		GUI_MsgBox(szInfoMsg, GUI_ICON_INFO);
	}	 
}


void RadReviewMsg(unsigned int uiError, const char *szStr1, const char *szStr2)
{
	CGFmtStr szAbortMsg;
	CGFmtStr szErrorMsg;
	CGFmtStr szWarnMsg;
	int iTemp;
	
	switch(uiError) { 
		case uiCOPY_DB_ERR:
			szErrorMsg.Printf("An error occurred while trying to copy the database\nfrom %s to %s.  The copy is aborted.", szStr1, szStr2);
			break;
		
		case uiDEFAULT_CFG_PATH_TOO_LONG_ERR:
			iTemp = PATH_LEN;
			szErrorMsg.Printf("Pathname of %s \nspecified in %s is too long. Please fix and start Radiation Review again.  Maximum length is %d characters.", szStr1, szStr2, iTemp);
			break;
		
		case uiNO_DEFAULT_CFG_FILE_ERR:
			szErrorMsg.Printf("File %s is not present, current working directory is %s.\nPlease fix this problem.  The program will exit now.", szStr1, szStr2);
			break;
		
		case uiNO_FILES_EXIST_WARN:
			szWarnMsg.Printf("No %s files exist at %s.", szStr1, szStr2);
			break;
		
		case uiOPEN_COM_DB_WARN:
			szWarnMsg.Printf("Could not open %s database at \n%s, \ncheck path is correct.", szStr1, szStr2);
			break;

		case uiNO_IRS_INI:	// 04-Dec-2005 SFK Added new error msg
			szErrorMsg.Printf("Expected file \"%s\" at %s.  Please fix this problem.  The program will exit now.", szStr2, szStr1);
			break;
		
		default: 
			if (!RAD_Unhandled_Errors(uiError, szErrorMsg)) {
				szWarnMsg.Printf("An unrecognizable error (%u) has occurred.(2 Strings)", uiError);
			}
			break;			
   	}
	if (szErrorMsg.GetDim() > 1) {
		GUI_MsgBox(szErrorMsg, GUI_ICON_ERROR);
	}	 
    	
	if (szWarnMsg.GetDim() > 1) {
		GUI_MsgBox(szWarnMsg, GUI_ICON_WARN);
	}	 

	if (szAbortMsg.GetDim() > 1) {
		GUI_MsgBox(szAbortMsg, GUI_ICON_ERROR);
		exit(-1);
	}	 
}	


void RadReviewMsg(unsigned int uiError, const char *szStr, int iNum)
{
	CGFmtStr szAbortMsg;
	CGFmtStr szErrorMsg;
	CGFmtStr szWarnMsg;
	CGFmtStr szInfoMsg;
	
	switch(uiError) { 
	
		case uiCHAN_NOT_DEFINED_ERR:
			szErrorMsg.Printf("Channel %d is not defined (Function %s).", iNum, szStr);
			break;

		case uiDB_BAD_ERR :
			szErrorMsg.Printf("Database error number %d in function %s.\nTry clearing database and reimporting the data.", iNum, szStr);
			break;

		case uiFAC_NOT_IN_DB_ERR:
			szErrorMsg.Printf("Facility %d is not defined (Function %s).", iNum, szStr);
			break;

		case uiFACILITY_NOT_DEFINED_ERR:
			szErrorMsg.Printf("Facility %d is not defined.  Please select a valid facility from Diagnostics:SelectFacility menu option).", iNum);
			break;

		case uiFILE_STA_NOT_IN_DB_WARN :
			szWarnMsg.Printf("File: %s\nStation number %d is unknown to Radiation Review.\nAdd with Facility Manager", szStr, iNum);
			break;
		
		case uiINVALID_FACILITY:
			szErrorMsg.Printf("The FacilityID %d in \\IRS\\IRS.INI is not known to \nFacility Manager.  Please change IRS.INI", iNum);
			break;
		
		default: 
			if (!RAD_Unhandled_Errors(uiError, szErrorMsg)) {
				szWarnMsg.Printf("An unrecognizable error (%u) has occurred.(String & number)", uiError);
			}
			break;			
	}	
	if (szErrorMsg.GetDim() > 1) {
		GUI_MsgBox(szErrorMsg, GUI_ICON_ERROR);
	}	 
	
	if (szWarnMsg.GetDim() > 1) {
		GUI_MsgBox(szWarnMsg, GUI_ICON_WARN);
	}	 
	
	if (szAbortMsg.GetDim() > 1) {
		GUI_MsgBox(szAbortMsg, GUI_ICON_ERROR);
		exit(-1);
	}	 
}	

void RadReviewMsg(unsigned int uiError, DATE dDate1, DATE dDate2)
{
	CGFmtStr szInfoMsg;
	char szDate1[DT_LEN+1], szTime1[DT_LEN+1], szDate2[DT_LEN+1], szTime2[DT_LEN+1];	
	//CMyDateTime Date;
	
	switch(uiError) { 

		case uiNO_MATCHING_CHAN_EVENTS_WARN:
			glMyDateTime.DATETimestampToDateTimeStrs(dDate1, szDate1, szTime1);
			glMyDateTime.DATETimestampToDateTimeStrs(dDate2, szDate2, szTime2);
			if ((int)dDate1 != (int)dDate2)
				szInfoMsg.Printf("No channel events found in interval:\n %s to %s.", szDate1, szDate2);
			else	                                                                                       
				szInfoMsg.Printf("No channel events on %s for time interval:\n%s to %s.", szDate1, szTime1, szTime2);			
			break;

		case uiNO_MATCHING_DIR_EVENTS_WARN:
			glMyDateTime.DATETimestampToDateTimeStrs(dDate1, szDate1, szTime1);
			glMyDateTime.DATETimestampToDateTimeStrs(dDate2, szDate2, szTime2);
			if ((int)dDate1 != (int)dDate2)
				szInfoMsg.Printf("No direction events found in interval:\n %s to %s.", szDate1, szDate2);
			else	                                                                                       
				szInfoMsg.Printf("No direction events on %s for time interval:\n%s to %s.", szDate1, szTime1, szTime2);			
			break;


		case uiNO_MEAS_TYPE_IN_INTERVAL_WARN:
			glMyDateTime.DATETimestampToDateTimeStrs(dDate1, szDate1, szTime1);
			glMyDateTime.DATETimestampToDateTimeStrs(dDate2, szDate2, szTime2);
			if ((int)dDate1 != (int)dDate2)
				szInfoMsg.Printf("No measurements of specified types\n found in interval: %s to %s.", szDate1, szDate2);
			else	                                                                                       
				szInfoMsg.Printf("No measurements of specified types\n on %s found in interval: %s to %s.", szDate1, szTime1, szTime2);			
			break;
                                 

		default: 
			if (!RAD_Unhandled_Errors(uiError, szInfoMsg)) {
				szInfoMsg.Printf("An unrecognizable message (%u) was requested. (Two dates)", uiError);
			}
			break;			
   	}

	if (szInfoMsg.GetDim() > 1) {
		GUI_MsgBox(szInfoMsg, GUI_ICON_INFO);
	}	 
}


void RadReviewMsg(unsigned int uiError, DATE dDate1, DATE dDate2, const char *pStr)
{
	CGFmtStr szInfoMsg;
	char szDate1[DT_LEN+1], szTime1[DT_LEN+1], szDate2[DT_LEN+1], szTime2[DT_LEN+1];
	//CMyDateTime Date;
	
	switch(uiError) { 

		case uiNO_DATA_IN_INTERVAL_WARN:
			glMyDateTime.DATETimestampToDateTimeStrs(dDate1, szDate1, szTime1);
			glMyDateTime.DATETimestampToDateTimeStrs(dDate2, szDate2, szTime2);
			szInfoMsg.Printf("No data for %s\nfound in interval: \n%s %s to \n%s %s.", pStr, szDate1, szTime1, szDate2, szTime2);
			break;

		default: 
			if (!RAD_Unhandled_Errors(uiError, szInfoMsg)) {
				szInfoMsg.Printf("An unrecognizable message (%u) was requested. (Two dates and string)", uiError);
			}
			break;			
   	}

	if (szInfoMsg.GetDim() > 1) {
		GUI_MsgBox(szInfoMsg, GUI_ICON_INFO);
	}	 
}

CGUI_Dlg *pDlg;

//BOOL ModelessMsgExitNotifyFn(CGUI_Dlg *pDlg, GUI_ACTION Action)
//{   
//	BOOL bCanClose = TRUE;
//	pDlg = NULL;	// signal to ourselves that the modeless dialog box is being deleted
//	return(bCanClose);
//}


void RadReviewModelessMsg(unsigned int uiError, const char *szStr)
{

	if (pDlg) pDlg = NULL;
	char szErrorMsg[MAX_LINE_LEN];

	switch(uiError) { 
			
		case uiFACILITY_NOT_DEFINED_ERR:
			sprintf(szErrorMsg, "Facility %s is not defined in Facility Manager.  Check RAD_LOCATION_MATCH_ITEM in RAD.INI", szStr);
			break;

		case uiREQUESTED_FACILTY_NOT_CURRENT_FACILTY:
			sprintf(szErrorMsg, "%s is not the current default facility.", szStr);
			break;

		case uiNO_ASSOCIATED_GRAPH_TRACE_ERR:
			sprintf(szErrorMsg, "No traces on the graph match the specified location.  Please manually select traces now for %s.  \nTo suppress this message, change RAD_LOCATION_NO_MATCH_MSG to FALSE in RAD.INI.", szStr);
			break;

		case uiNO_CHANNEL_STRING_MATCH_SUPPORTED_ERR:
			sprintf(szErrorMsg, "Matching to a channel string (%s) is not supported. Change RAD_LOCATION_MATCH_ITEM in RAD.INI ", szStr);
			break;          

		case uiNO_LOCATION_NUM_MATCH_SUPPORTED_ERR:
			sprintf(szErrorMsg, "Matching to a location number (%s) is not supported. Change RAD_LOCATION_MATCH_TYPE to 1 in RAD.INI.", szStr);
			break;          

		case uiSTA_NOT_IN_DB_ERR:
			sprintf(szErrorMsg, "Station %s is not defined in Facility Manager.", szStr);
			break;

		default: 
			{
			CGFmtStr strhack;
			if (!RAD_Unhandled_Errors(uiError, strhack)) {
				strhack.Printf("An unrecognizable error (%u) has occurred.(String)", uiError);
			}
			strhack.CopyTo_sz(szErrorMsg, MAX_LINE_LEN);
			}
		break;			
   	}
 	
	if (strlen(szErrorMsg) > 1) {
		pDlg = new CGUI_Dlg (IDD_MODELESSMSG, NULL);
		pDlg->DefinePushBtn(IDOK, GUI_OK);
		//pDlg->SetVisible(IDOK, TRUE);
		pDlg->DefineFldStaticTxt(IDC_MESSAGE_TEXT);
		pDlg->FldStaticTxtSetValue(IDC_MESSAGE_TEXT, szErrorMsg);
		//BOOL SetDlgExitValidateFn(BOOL (*pExitValidateFn)(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction));
//		pDlg->SetDlgExitValidateFn((GUI_CB_DlgExitFn)&ModelessMsgExitNotifyFn);			
		pDlg->SetFocus(IDOK);
			
		// keep this dialog box always on top
		::SetWindowPos(pDlg->GetWndHandle(), HWND_TOP, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);

		pDlg->GoModeless(TRUE);      // TRUE indicates box deletes itself on exit

	
	}	 
    	
}
static int RAD_Unhandled_Errors(unsigned int uiError, CGFmtStr& szStr)
{
	BOOL bFound = TRUE;

	switch(uiError) 
	{
	case uiFILE_FAILED_SNF_CHECK_SIGNATURE: //SCR00226 and SCR00227
		szStr.Printf("Error %d: File failed SnF check.");
		break;
	case uiDAY_NOT_IN_DB_STATUS:
		szStr.Printf("Error %d: Day not in database.", uiError);
		break;
	case uiFAC_NOT_IN_DB_ERR:
		szStr.Printf("Error %d: Facility not in database.", uiError);
		break;
	case uiFILE_DOES_NOT_EXIST_STATUS:
		szStr.Printf("Error %d: File does not exist.", uiError);
		break;
	case uiFILE_READ_ERR_STATUS:
		szStr.Printf("Error %d: File read error.", uiError);
		break;
	case uiINVALID_TIME_INTERVAL_ERR:
		szStr.Printf("Error %d: Invalid time interval.", uiError);
		break;
	case uiMARK_NOT_IN_DB_WARN:
		szStr.Printf("Error %d: Mark not in database", uiError);
		break;
	case uiNO_DATA_FOR_STATION_WARN:
		szStr.Printf("Error %d: No data for station.", uiError);
		break;
	case uiSKIP_FILE_STATUS:
		szStr.Printf("Error %d: File was skipped.", uiError);
		break;
	case uiSTA_NOT_IN_DB_ERR:
		szStr.Printf("Error %d: Station not in database.", uiError);
		break;
	case uiTIME_NOT_IN_DB_STATUS:
		szStr.Printf("Error %d: Time not in database.", uiError);
		break;
	case uiEVENT_NOT_IN_DB_INFO:
		szStr.Printf("Error %d: Event not in database.", uiError);
		break;
	case uiNO_MATCHING_OP_REC_STATUS:
		szStr.Printf("Error %d: No matching record in operator declaration.", uiError);
		break;
	case uiBLANK_STRING_ERR:
		szStr.Printf("Error %d: Blank strings are illegal.", uiError);
		break;
	case uiBROWSE_INFO_WARN:
		szStr.Printf("Error %d: Incorrect parameters for browse.", uiError);
		break;
	case uiCANNOT_OPEN_ANY_DATABASE_ERR:
		szStr.Printf("Error %d: Cannot open any database.", uiError);
		break;
	case uiCHAN_NOT_DEFINED_ERR:
		szStr.Printf("Error %d: Requested channel is not defined.", uiError);
		break;
	case uiCOPY_DB_ERR:
		szStr.Printf("Error %d: Error during database copy.", uiError);
		break;
	case uiDB_DOES_NOT_EXIST_ERR:
		szStr.Printf("Unexpected Error %d: No database exists at specified path.", uiError);
		break;
	case uiDATABASE_OPENED_INFO:
		szStr.Printf("Error %d: Databases were opened.", uiError);
		break;
	case uiDB_BAD_ERR:
		szStr.Printf("Error %d: Database error.", uiError);
		break;
	case uiDB_EMPTY_STATUS:
		szStr.Printf("Error %d: The database is empty or facility has no RAD stations.", uiError);
		break;
	case uiDB_INIT_ERR:
		szStr.Printf("Error %d: Could not initialize database.", uiError);
		break;
	case uiDEFAULT_CFG_PATH_TOO_LONG_ERR:
		szStr.Printf("Error %d: Default.cfg file is too long.", uiError);
		break;
	case uiDEFAULT_CFG_FILE_ERR:
		szStr.Printf("Error %d: Error in default.cfg.", uiError);
		break;
	case uiDFLT_DB_READ_ERR:
		szStr.Printf("Error %d: Default parameter error.", uiError);
		break;
	case uiDISPLAY_AT_LEAST_ONE_CHANNEL_ERR:
		szStr.Printf("Error %d: Select at least one channel.", uiError);
		break;
	case uiDRIVE_DOES_NOT_EXIST_ERR:
		szStr.Printf("Error %d: Drive does not exist.", uiError);
		break;
	case uiEVENT_NOT_FOUND_WARN:
		szStr.Printf("Error %d: No event found.", uiError);
		break;
	case uiFACILITY_NOT_DEFINED_ERR:
		szStr.Printf("Error %d: No facility defined.", uiError);
		break;
	case uiFILE_OPEN_ERR:
		szStr.Printf("Error %d: File open error.", uiError);
		break;
	case uiFILE_READ_ERR:
		szStr.Printf("Error %d: File read error.", uiError);
		break;
	case uiFILE_WRITE_ERR:
		szStr.Printf("Error %d: File write error.", uiError);
		break;
	case uiFILE_STA_NOT_IN_DB_WARN:
		szStr.Printf("Error %d: File station not in facility.", uiError);
		break;
	case uiINTERNAL_ERR:
		szStr.Printf("Error %d: Internal error.", uiError);
		break;
	case uiINVALID_DFLT_DB_FIELD_ERR:
		szStr.Printf("Error %d: Invalid field requested in default database.", uiError);
		break;
	case uiINVALID_FACILITY:
		szStr.Printf("Error %d: Invalid facility.", uiError);
		break;
	case uiMAKE_DIR_ERR:
		szStr.Printf("Error %d: Could not make directory path.", uiError);
		break;
	case uiMEMORY_ALLOCATION_ERR:
		szStr.Printf("Error %d: Could not allocate memory.", uiError);
		break;
	case uiNAME_TOO_LONG_ERR:
		szStr.Printf("Error %d: Name is too long.", uiError);
		break;
	case uiNAMES_CFG_READ_ERR:
		szStr.Printf("Error %d: Error reading configuration information.", uiError);
		break;
	case uiNO_ASSOCIATED_GRAPH_TRACE_ERR:
		szStr.Printf("Error %d: No associated graph trace.", uiError);
		break;
	case uiNO_CHANNEL_STRING_MATCH_SUPPORTED_ERR:
		szStr.Printf("Error %d: Matching by channel name not supported.", uiError);
		break;
	case uiNO_CHANS_SELECTED_WARN:
		szStr.Printf("Error %d: No channels selected.", uiError);
		break;
	case uiNO_DATA_IN_INTERVAL_WARN:
		szStr.Printf("Error %d: No data in interval.", uiError);
		break;
	case uiNO_DEFAULT_CFG_FILE_ERR:
		szStr.Printf("Error %d: No default.cfg file present.", uiError);
		break;
	case uiNO_DIR_SELECTED_WARN:
		szStr.Printf("Error %d: No directory selected.", uiError);
		break;
	case uiNO_FACILITIES_ERR:
		szStr.Printf("Error %d: No facilities configured.", uiError);
		break;
	case uiNO_FILES_EXIST_WARN:
		szStr.Printf("Error %d: No files exist.", uiError);
		break;
	case uiNO_LOCATION_NUM_MATCH_SUPPORTED_ERR:
		szStr.Printf("Error %d: Matching by location number not supported.", uiError);
		break;
	case uiNO_MATCHING_CHAN_EVENTS_WARN:
		szStr.Printf("Error %d: No matching channel events.", uiError);
		break;
	case uiNO_MATCHING_DATA_WARN:
		szStr.Printf("Error %d: No matching data.", uiError);
		break;
	case uiNO_MATCHING_RAW_DATA_WARN:
		szStr.Printf("Error %d: No matching raw data.", uiError);
		break;
	case uiNO_MATCHING_DIR_EVENTS_WARN:
		szStr.Printf("Error %d: No matching direction events.", uiError);
		break;
	case uiNO_MEAS_SELECTED_WARN:
		szStr.Printf("Error %d: No measurement selected.", uiError);
		break;
	case uiNO_MEAS_TYPE_IN_INTERVAL_WARN:
		szStr.Printf("Error %d: No measurements of given type in interval.", uiError);
		break;
	case uiNO_MEAS_TYPES_SELECTED_WARN:
		szStr.Printf("Error %d: No measurement types selected.", uiError);
		break;
	case uiNO_MEMORY_FOR_APP_ERR:
		szStr.Printf("Error %d: Not enough memory for application.", uiError);
		break;
	case uiNO_NCC_FILE_REQUESTED_ERR:
		szStr.Printf("Error %d: No *.ncc file requested.", uiError);
		break;
	case uiNO_STAS_SELECTED_WARN:
		szStr.Printf("Error %d: No stations selected.", uiError);
		break;
	case uiNO_STATIONS_FOR_FACILITY_ERR:
		szStr.Printf("Error %d: No stations for facility.", uiError);
		break;
	case uiNOT_ENOUGH_DATA_TO_ANALYZE_WARN:
		szStr.Printf("Error %d: Not enough data to analyze.", uiError);
		break;
	case uiONLY_ONE_GRAPH_WARN:
		szStr.Printf("Error %d: Only one graph can be displayed.", uiError);
		break;
	case uiOPEN_COM_DB_WARN:
		szStr.Printf("Error %d: Could not open COM.", uiError);
		break;
	case uiPATH_DOES_NOT_EXIST_ERR:
		szStr.Printf("Error %d: Path does not exist.", uiError);
		break;
	case uiPATH_TOO_LONG_ERR:
		szStr.Printf("Error %d: Path too long.", uiError);
		break;
	case uiPATHS_TOO_LONG_ERR:
		szStr.Printf("Error %d: Paths too long.", uiError);
		break;
	case uiPEAK_WIDTH_TOO_LARGE_WARN:
		szStr.Printf("Error %d: Peak width too large.", uiError);
		break;
	case uiRAD_INVALID_DATABASE_PATH_ERR:
		szStr.Printf("Error %d: Invalid database path.", uiError);
		break;
	case uiRAD_START_FATAL_ERR:
		szStr.Printf("Error %d: Fatal error when starting Radiation Review. Sorry . . .", uiError);
		break;
	case uiREQUESTED_FACILTY_NOT_CURRENT_FACILTY:
		szStr.Printf("Error %d: Requested facility not current facility.", uiError);
		break;
	case uiRT_IMPORT_ERR:
		szStr.Printf("Error %d: Error in /IMPORT mode.", uiError);
		break;
	case uiSUMMARY_NOT_FOUND_STATUS:
		szStr.Printf("Error %d: Summary not found.", uiError);
		break;
	case uiUNKNOWN_EVENT_TYPE_ERR:
		szStr.Printf("Error %d: Unknown event type.", uiError);
		break;
	case uiWILDCARDS_TOO_LONG_ERR:
		szStr.Printf("Error %d: Wildcards too long.", uiError);
		break;
	case uiWILDCARD_DOES_NOT_EXIST:
		szStr.Printf("Error %d: Wildcard does not exist.", uiError);
		break;
	
	default: 
		szStr.Printf("An unrecognizable status error (%u) was requested.", uiError);
		bFound = FALSE;
		break;	
	}
	return(bFound);
}
