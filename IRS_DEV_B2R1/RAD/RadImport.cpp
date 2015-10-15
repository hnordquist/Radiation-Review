///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : RadImport.cpp     
//Primary Author: Shirley Klosterbuer/Steve Buck

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////   
    
#include <stdio.h>
#include <io.h>
#include "RAD.H"       
#include "DlgDirection.h"
#include "DlgExport.h"
#include "DlgFindEvent.h"
#include "DlgFindMeas.h"
#include "DlgImportAll.h"
#include "DlgIntegrity.h"
#include "RadImport.H"
#include "RadInit.H"
#include "RadMain.h"
#include "upperlayer/RTTitle.H"	// 8-Dec-04 SFK Added relative path

extern RTTitle *pRT_Title;
extern RADInit *pRAD_Init;  
extern BOOL glImportExit;	



/////////////////////////////////////////////////////////////////////////////////////
//  RADImport constructor, destructor
/////////////////////////////////////////////////////////////////////////////////////

RADImport::RADImport(void) :
	bRAD_Import_Done(IMPORT_FAIL)

	{
		bInImportMode = FALSE;
	}

 
RADImport::~RADImport(void) {
	RAD_Import_Delete();
	
}


/////////////////////////////////////////////////////////////////////
// Access to the common data items across all review tool operations
/////////////////////////////////////////////////////////////////////

BOOL RADImport::Set_Common_Params(RTInit *const pRT_Common_Params) {
    
    RADImport::pCommon_Params = (RADInit *)pRT_Common_Params;
    return RTImport::Set_Common_Params(pCommon_Params);
    
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Required through RTImport to provide the Import Begin procedure which is specific to operator review
////////////////////////////////////////////////////////////////////////////////////////////////////////

RTImport::IMPORT_OPERATION RADImport::RT_Import_Begin(BOOL bBypass_Import_Prompt) {

	bRAD_Import_Done = IMPORT_REVIEW;	// default to normal exit

	// if a rad.db file is already there, delete it	
	remove("rad.db");
	
	BOOL bAutomateQuestion = FALSE;
	BOOL bImportAllOkay = FALSE;
	BOOL bIntegrityOkay = FALSE;
	BOOL bFindEventsOkay = FALSE;
	BOOL bDirectionOkay = FALSE;
	BOOL bMeasurementOkay = FALSE;
	BOOL bExportOkay = FALSE;
	BOOL bMoreDisks = FALSE;
    CGFmtStr msg;

	bInImportMode = TRUE;


	pRT_Title->RT_Title_Splash_Close();		// shut down splash screen if it is up
    if (bBypass_Import_Prompt == FALSE) {
		//  If requested, ask if want to automate import and display warnings
		if (pRAD_Init->Get_AutomateQuestion()) {
			if ((!glImportExit) && (RAD_ImportAutomateQuestion())) bAutomateQuestion = TRUE; // FALSE == abort
		}
		else {	// skip asking the question
			bAutomateQuestion = TRUE;
		}
	}	
	else bAutomateQuestion = TRUE;	// skip the prompt
		
	
	//  Do the import and analysis, check for a cancel (glImportExit) along the way
	if (bAutomateQuestion && (pRAD_Init->Get_AutomateImportAll())) {
		if (!glImportExit) {
			// if will import files from same drive as OP, need a prompt
			if (pRAD_Init->Get_ImportDiskPrompt() == TRUE) {
				RAD_ImportPromptForDisk();
				if (ImportAllDoIt(TRUE) == TRUE) bImportAllOkay = TRUE;
			}
			// if will import files from multiple disks need a prompt to put in a new disk
			else if (pRAD_Init->Get_ImportMultipleDisks() == TRUE) {
				RAD_ImportPromptForDisk();		// prompt for the first disk
				bMoreDisks = FALSE;
				do {
					if (bMoreDisks == FALSE) { // on first disk
						bImportAllOkay = ImportAllDoIt(TRUE);
					}
					else {
						bImportAllOkay = ImportAllDoIt(FALSE);
					}
					if (bImportAllOkay == TRUE) bMoreDisks = RAD_ImportMultipleDisks();
				} while ((bImportAllOkay == TRUE) && (bMoreDisks == TRUE));
			}
			// normal case
		else 
			if (ImportAllDoIt(TRUE) == TRUE) bImportAllOkay = TRUE;
		}
	}		
	else bImportAllOkay = TRUE;
	
	
	if (bAutomateQuestion && bImportAllOkay && (pRAD_Init->Get_AutomateIntegrity())) {
		if ((!glImportExit) && (DataIntegrityDoIt())) bIntegrityOkay = TRUE;
	}	
	else bIntegrityOkay = TRUE;	
	
	if (bAutomateQuestion && bImportAllOkay && bIntegrityOkay && (pRAD_Init->Get_AutomateFindEvents())) {
		if ((!glImportExit) && (FindEventsDoIt())) bFindEventsOkay = TRUE;
	}	
	else bFindEventsOkay = TRUE;
	
	if (bAutomateQuestion && bImportAllOkay && bIntegrityOkay && bFindEventsOkay &&(pRAD_Init->Get_AutomateDirection())) {
		if ((!glImportExit) && (DirectionAnalysisDoIt())) bDirectionOkay = TRUE;
	}	
	else bDirectionOkay = TRUE;	
		
	if (bAutomateQuestion && bImportAllOkay && bIntegrityOkay && bFindEventsOkay && bDirectionOkay &&(pRAD_Init->Get_AutomateMeasurement())) {
		if ((!glImportExit) && (DetermineMeasurementsDoIt())) bMeasurementOkay = TRUE;
	}	
	else bMeasurementOkay = TRUE;	
		
	if (bAutomateQuestion && bImportAllOkay && bIntegrityOkay && bFindEventsOkay && bDirectionOkay &&bMeasurementOkay&&(pRAD_Init->Get_AutomateExport())) {
		if ((!glImportExit) && (ExportToINCCDoIt())) bExportOkay = TRUE;
	}	
	else bExportOkay = TRUE;

	CGUI_App::ProcessEvents();
		
	if (glImportExit) { // Received command to exit from IR
		bRAD_Import_Done = IMPORT_EXIT;
	}

	if (bAutomateQuestion && bImportAllOkay && bIntegrityOkay && bFindEventsOkay
		&& bDirectionOkay && bMeasurementOkay && bExportOkay) {
		bRAD_Import_Done = IMPORT_REVIEW;
	}
	else {
		RadReviewMsg(uiRT_IMPORT_ERR);
	}
	CleanupForExit();		// sfk 07-24-2003
	return bRAD_Import_Done;
}
		

void RADImport::RAD_Import_Delete(void) {
// nothing to do here
	//RT_Import_Exit();		//sfk 07-24-2003
}



BOOL RADImport::RAD_ImportAutomateQuestion(void)
{
	GUI_ACTION response;
	CGFmtStr msg;

	pRT_Title->RT_Title_Splash_Close();		// shut down splash screen if it is up
	msg.Printf("You have chosen to start a new inspection review.  If you continue, the following will happen:\n\n");
	if (pRAD_Init->Get_AutomateImportAll()) msg.PrintfAppend("ALL DATA WILL BE DELETED FROM THE EXISTING DATABASE!!!\nNew data will be imported from %s.\n", pRAD_Init->Get_Import_All_Path());
	if (pRAD_Init->Get_AutomateIntegrity()) msg.PrintfAppend("Data integrity search will be performed.\n");
	if (pRAD_Init->Get_AutomateFindEvents()) msg.PrintfAppend("Channel event search will be performed.\n");
	if (pRAD_Init->Get_AutomateDirection()) msg.PrintfAppend("Direction of events will be determined.\n");
	msg.PrintfAppend("\nDo you want to continue?");
	response = GUI_MsgBox(msg, GUI_ICON_WARN, GUI_NOYES);
	if(response == GUI_YES) {
		if (pRAD_Init->Get_AutomateImportAll()) msg.Printf("ARE YOU SURE YOU WANT TO DELETE THE EXISTING DATABASE?");
		response = GUI_MsgBox(msg, GUI_ICON_WARN, GUI_NOYES);
		if (response == GUI_YES) return(TRUE);
	}	
	return(FALSE);
}

BOOL RADImport::RAD_ImportPromptForDisk(void)
{
	GUI_ACTION response;
	CGFmtStr msg;
	pRT_Title->RT_Title_Splash_Close();		// shut down splash screen if it is up
	msg.Printf("Please place the diskette containing the Radiation data \nin the drive containing the path %s", pRAD_Init->Get_Import_All_Path());
	msg.PrintfAppend("\nPress OK when ready to proceed");
	response = GUI_MsgBox(msg);
	return(TRUE);
}

BOOL RADImport::RAD_ImportMultipleDisks(void)
{
	GUI_ACTION response;
	CGFmtStr msg;

	pRT_Title->RT_Title_Splash_Close();		// shut down splash screen if it is up
	msg.Printf("Do you want to import data from another diskette?");
	response = GUI_MsgBox(msg, GUI_ICON_NONE, GUI_NOYES);
	if(response == GUI_YES) {
		RAD_ImportPromptForDisk();
		return(TRUE);
	}	
	else {
		return(FALSE);
	}
	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Required through RTImport to provide the Import Exit procedure which is specific to operator review
///////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RADImport::RT_Import_Exit(void) {

	glImportExit = TRUE;
	CleanupForExit();
	return TRUE;
	
}




