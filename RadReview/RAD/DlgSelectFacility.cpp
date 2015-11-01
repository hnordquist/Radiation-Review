///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgSelectFacility.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//	SelectFacilityMenuOption()	- Diagnostics|Select Facility menu option
///////////////////////////////////////////////////////////////////////////
#include <gen.h>
#include "RAD.H"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DlgSelectFacility.h"
#include "FacilityConfig.h"
#include "RadMain.h"
#include "ReadCfg.h"
#include "Utilities.h"

extern short glsFacNum;

    
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);



/*===========================================================================
 *
 *  Name	 :  	UpdateDbParms
 *
 *  Purpose	 :		Called from the dialog box when the NewDefaults button is pushed.
 *					The present values are read from the dialog box and written to
 *					the Default database.
 *
 *
 *  Return value :  None
 *
 *  Special notes:  
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  16-Mar-1996 SFK		Created
 *	17-Sep-2003 SFK		Integrated with new FacMgr
 *
===========================================================================*/
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
//    int status;  
	unsigned int iFacChoice;
	short sFacNum;

//	status = AskUpdateQuestion();
//	if (status != TRUE) return;
	    
   /* ------------------------------------------------------------------
    *	Get the current facility chosen and find its number.
    * ----------------------------------------------------------------*/
	pDlg->PickListGetValue(IDC_SELECTFACILITY, &iFacChoice);
	if (!pglFacCfg->GetFacilityIDForPicklistChoice(iFacChoice, &sFacNum)) return;
	
   /* ------------------------------------------------------------------
    *	Write the current values to the database
    * ----------------------------------------------------------------*/
//	status = AccessDefaultDatabase(WRITE, S_DFLT_FACILITY, &sFacNum);
//	if (status != TRUE) {
//		RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
//	}
//	SetDefaultFacilityNumber(sFacNum);
	SaveDatabaseNow();
}        
         
                
//	17-Sep-2003 SFK		Integrated with new FacMgr
void SelectFacilityMenuOption()
{                          
	static bool bFirst = true;
	
	static short sFacNum;
    GUI_ACTION DlgReturn;         
    unsigned int uiPicklistChoice;
    int status;   
    char **aszFacList = NULL; 
	char strFacDirectory[MAX_LINE_LEN+1];
	
    
	SetCurrentDrive();
    if (bFirst) 
	{  
//    	status = AccessDefaultDatabase(READ, S_DFLT_FACILITY, &sFacNum);
//    	if (status != TRUE) {
//			RadReviewMsg(status, "when reading from Default database.  Cannot proceed.");			
//			return;
//		}
		if (!GetDefaultFacilityNumber(&sFacNum)) 
			return;
		bFirst = false;
	}		
  
	// verify facility choice is still a legitimate Facility Number, if not set the
	// facility to the first facility in the database (if there is a facility)
	if (!pglFacCfg->IsFacilityDefined(sFacNum)) {
		if (!pglFacCfg->GetFirstFacilityID(&sFacNum)) {
			RadReviewMsg(uiNO_FACILITIES_ERR);
			return;
		}
	}

    CGUI_Dlg SelectFacilityDlg(IDD_DIAG_SELECTFACILITY, NULL, DLG_POSN_NO_SAVE);
    if (SelectFacilityDlg.IsValid()) {
		status = pglFacCfg->CreateFacilityPicklist(&aszFacList);
		if (status != TRUE) return;
					
	   /* ------------------------------------------------------------------
	    *	Find the picklist positions for current default values
	    * ----------------------------------------------------------------*/
		
			
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		SelectFacilityDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		SelectFacilityDlg.DefinePushBtn(IDOK, GUI_OK,UpdateDbParms);		//12-3-2003 always change the default db
//		SelectFacilityDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateDbParms);  
		
		short sTemp;
		if (!pglFacCfg->GetPicklistChoiceForFacilityID(sFacNum, &sTemp)) return;
		uiPicklistChoice = sTemp;
		SelectFacilityDlg.DefinePickList(IDC_SELECTFACILITY, aszFacList, &uiPicklistChoice);
				
		SelectFacilityDlg.SetFocus(IDOK);
		
	    DlgReturn = SelectFacilityDlg.Go();
		    
	    if (DlgReturn == GUI_OK) {                
	    
		    SelectFacilityDlg.RetrieveAllControlValues();	
		    
		   /* ------------------------------------------------------------------
		    *	Get the database numbers associated with the picklist choices
		    * ----------------------------------------------------------------*/
			if (!pglFacCfg->GetFacilityIDForPicklistChoice((short)uiPicklistChoice, &sFacNum)) return;

			// know the facility we want.  Need to close the existing database and open the one associated
			// with the facility from facility configurator
			short sOldFacNum = glsFacNum;		// remember our original facility number

			// Try to switch to the database associated with this facility in Facility Manager
			// We expect the database to be in the \RAD subdirectory under this directory.
			pglFacCfg->GetFacilityDirectory(sFacNum, strFacDirectory, MAX_LINE_LEN);

			// 08-Jun-2005 SFK create a new datastore at the facility spot if necessary.  
			BOOL bOkay = CreateFacilityDatastore (strFacDirectory);
			
			// know a datastore is now at the new spot, switch from the current facility to the new one
			bOkay = SwitchDatabases(strFacDirectory, sFacNum, FALSE);

			CGFmtStr msg;
			char szDum[1];
			if (bOkay) {		// successfully found and opened the database
				if (sOldFacNum != sFacNum) {
					glsFacNum = sFacNum;		// this is now the default facility for Rad
					SetDefaultFacilityNumber(sFacNum);
					SetTitleBar(sFacNum, false, szDum);

					// inform user that they should restart Rad
	    			msg.Printf("%s has been selected as \nthe new default facility.  Please restart Radiation Review \nbefore doing a match to another review tool.", aszFacList[uiPicklistChoice]);	
	    			GUI_MsgBox(msg, GUI_ICON_INFO);
					SaveDatabaseNow();	// 16-Nov-2004 SFK 3.0.3.16 open and close everything one more time to make sure all consistent
				}
				else {
					msg.Printf("%s is still the facility.", aszFacList[uiPicklistChoice]);	
					GUI_MsgBox(msg, GUI_ICON_INFO);
				}

			}
			else {
				msg.Printf("Could not select %s as the new default facility.  \nCheck facility parameters in Facility Manager.", aszFacList[uiPicklistChoice]);	
				GUI_MsgBox(msg, GUI_ICON_INFO);
			}

	    }

		pglFacCfg->DestroyPicklist(aszFacList);
		aszFacList = NULL;
	}    
}   		                                 
   		                                 
                                 
