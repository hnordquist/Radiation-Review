///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : RadSetup.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "DbLimit.h"
#include "DlgDirection.h"
#include "DlgFindEvent.h"
#include "DlgExport.h"
#include "DlgImportAll.h"
#include "DlgIntegrity.h"
#include "DlgFindMeas.h"
#include "RadInit.H"
#include "RadMain.h"
#include "RadSetup.h"   

extern RADInit *pRAD_Init;  



/////////////////////////////////////////////////////////////////////////////////////
//  RAD_Setup constructor, destructor
/////////////////////////////////////////////////////////////////////////////////////

RADSetup::RADSetup(void) :
	bRAD_Setup_Done(SETUP_FAIL)
	{
	}
	

RADSetup::~RADSetup(void) {
	
}


/////////////////////////////////////////////////////////////////////
// Access to the common data items across all review tool operations
/////////////////////////////////////////////////////////////////////

BOOL RADSetup::Set_Common_Params(RTInit *const pRT_Common_Params) {
    
    RADSetup::pCommon_Params = (RADInit *)pRT_Common_Params;
    return RTSetup::Set_Common_Params(pCommon_Params);
    
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Required through RTSetup to provide the Setup Begin procedure which is specific to radiation review
////////////////////////////////////////////////////////////////////////////////////////////////////////
// 12-Aug-2002	Changed to DATE
RTSetup::SETUP_OPERATION  RADSetup::RT_Setup_Begin() {

	CGFmtStr msg;
	bRAD_Setup_Done = SETUP_FAIL;
	//unsigned long ulDum;
	DATE dDum;
	BOOL bImportAllOkay = FALSE;
	BOOL bIntegrityOkay = FALSE;
	BOOL bFindEventsOkay = FALSE;
	BOOL bDirectionOkay = FALSE;
	BOOL bMeasurementOkay = FALSE;
	BOOL bExportOkay = FALSE;

	if (pMy_Application == NULL) {
		// Can now create the main application 
		if (((pMy_Application = new CGUI_App((zApp *)((void *)this), szTOOL_NAME, szTOOL_NAME)) == NULL) ||
			(!pMy_Application->SetIcon(IDI_NEW_RAD_REVIEW))) {
			RadReviewMsg(uiNO_MEMORY_FOR_APP_ERR);
			return(SETUP_FAIL);
		}
		pMy_Application->SetTitle(szTOOL_NAME);
	}

	// Check if want to do import all automatically, if yes, do it.	
	if (pRAD_Init->Get_AutomateImportAll()) { 
		if (ImportAllDialogBox()) {
			struct IDStruct TempID;
			TempID.sStaID = ANY_STATION;
			TempID.sStaType = ANY_TYPE;
			if (GetDbTimeRange(TempID, &dDum, &dDum) == uiDB_EMPTY_STATUS) {
				GUI_ACTION response;
				msg.Printf("You must have data in the database to continue RadiationSetup.");
				msg.PrintfAppend("Would you like to import the data now?");
				response = GUI_MsgBox(msg, GUI_ICON_QUESTION, GUI_YESNO);
				if (response != GUI_YES) return bRAD_Setup_Done;
				if (!ImportAllDoIt(TRUE))  return bRAD_Setup_Done;
			}
			bImportAllOkay = TRUE;
		}		 
	}
	else bImportAllOkay = TRUE;
                                                                    
	// If previous was okay, then check if want to do integrity automatically, if yes, do it.	                                                                    
	if (bImportAllOkay && (pRAD_Init->Get_AutomateIntegrity())) {
		if (DataIntegrityDialogBox()) bIntegrityOkay = TRUE;
	}	
	else bIntegrityOkay = TRUE;
	
	// If previous was okay, then check if want to do find events automatically, if yes, do it.
	if (bImportAllOkay && bIntegrityOkay && (pRAD_Init->Get_AutomateFindEvents())) {
		if (FindEventsDialogBox()) bFindEventsOkay = TRUE;
	}	
	else bFindEventsOkay = TRUE;

	// If previous was okay, then check if want to do direction automatically, if yes, do it.	
	if (bImportAllOkay && bIntegrityOkay && bFindEventsOkay && (pRAD_Init->Get_AutomateDirection())) {
		if (DirectionAnalysisDialogBox()) bDirectionOkay = TRUE;
	}	
	else bDirectionOkay = TRUE;
	                                                           
	// If previous was okay, then check if want to do measurements automatically, if yes, do it.	
	if (bImportAllOkay && bIntegrityOkay && bFindEventsOkay && bDirectionOkay && (pRAD_Init->Get_AutomateMeasurement())) {
		if (DetermineMeasurementsDialogBox()) bMeasurementOkay = TRUE;
	}	
	else bMeasurementOkay = TRUE;

	// If previous was okay, then check if want to do export automatically, if yes, do it.	
	if (bImportAllOkay && bIntegrityOkay && bFindEventsOkay && bDirectionOkay && bMeasurementOkay &&(pRAD_Init->Get_AutomateExport())) {
		if (ExportToINCCDialogBox()) bExportOkay = TRUE;
	}	
	else bExportOkay = TRUE;

	// if everything setup okay, return SETUP_SAVE	                                                           
	if (bImportAllOkay && bIntegrityOkay && bFindEventsOkay && bDirectionOkay && bMeasurementOkay && bExportOkay) bRAD_Setup_Done = SETUP_REVIEW;
	
	CleanupForExit();	// 07-24-2003 sfk
	return bRAD_Setup_Done;
}



// required by RT Review
BOOL RADSetup::RT_Setup_Exit() { 
	return TRUE;
}

		

