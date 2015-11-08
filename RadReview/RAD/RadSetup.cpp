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

		

