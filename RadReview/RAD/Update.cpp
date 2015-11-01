///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : Update.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "DlgDirection.h"
#include "DlgImportAll.h"
#include "DlgImportLatest.h"
#include "DlgFindEvent.h"
#include "Plot.h"



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
 *
===========================================================================*/
# if TRUE==FALSE
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	BOOL bOverwrite, bInitDB;
    char szNewPaths[PATH_LEN+1];
    char szNewWildCard[DOSNAME_LEN+1];
    int status;  
	             
	status = AskUpdateQuestion();
	if (status != TRUE) return;
	
   /* ------------------------------------------------------------------
    *	Get the current values for the path and extension fields and
    *	check boxes.
    * ----------------------------------------------------------------*/
	pDlg->FldTxtGetValue(IDC_IMPORT_EDIT_PATH, szNewPaths, PATH_LEN);
	pDlg->FldTxtGetValue(IDC_IMPORT_EDIT_WILDCARD, szNewWildCard, DOSNAME_LEN);
	pDlg->CheckBoxGetValue(IDC_IMPORT_OVERWRITE_BOX, &bOverwrite);
	pDlg->CheckBoxGetValue(IDC_IMPORT_INITDB_BOX, &bInitDB);
	
   /* ------------------------------------------------------------------
    *	Write the current values to the database
    * ----------------------------------------------------------------*/
	status = AccessDefaultDatabase(WRITE, SZ_IMPORT_PATH, szNewPaths);
	if (status != TRUE) goto exit;
	status = AccessDefaultDatabase(WRITE, SZ_IMPORT_WILDCARD, szNewWildCard);
	if (status != TRUE) goto exit;
	status = AccessDefaultDatabase(WRITE, B_IMPORT_ALL_OVERWRITE, &bOverwrite);
	if (status != TRUE) goto exit;
	status = AccessDefaultDatabase(WRITE, B_IMPORT_ALL_CLEAR, &bInitDB); 
	if (status != TRUE) goto exit;
	SaveDatabaseNow();
	return;
	
exit:
	display_err_msg1(status, "when writing to Default database.  Update not completed.");	
	return;
}        

#endif         
                                    
static BOOL bAllDataSetup = FALSE;                                    
static BOOL bTodaysDataSetup = TRUE;
static BOOL bDataIntegritySetup = FALSE;   
static BOOL bChannelEventsSetup = FALSE;   
static BOOL bRadiationEventsSetup = FALSE;   
static BOOL bAllDataRun = FALSE;
static BOOL bTodaysDataRun = TRUE;
static BOOL bDataIntegrityRun = FALSE;   
static BOOL bChannelEventsRun = FALSE;   
static BOOL bRadiationEventsRun = FALSE;   
static BOOL bGraphRun = TRUE;   
static BOOL bDialogOk = FALSE;

BOOL UpdateDoIt()
{   

	if(bDialogOk == TRUE) {	                     
		if (bAllDataSetup == TRUE) ImportAllDialogBox();	                     
		if (bTodaysDataSetup == TRUE) ImportTodayDialogBox();
		//	if (bDataIntegrity == TRUE) DataIntegrityDoIt();
		if (bChannelEventsSetup == TRUE) FindEventsDialogBox();
		if (bRadiationEventsSetup == TRUE) DirectionAnalysisDialogBox(); 

		if (bAllDataRun == TRUE) ImportAllDoIt(TRUE);		
		if (bTodaysDataRun == TRUE) ImportTodayDoIt();
		//	if (bDataIntegrity == TRUE) DataIntegrityDoIt();
		if (bChannelEventsRun == TRUE) FindEventsDoIt();
		if (bRadiationEventsRun == TRUE) DirectionAnalysisDoIt();
		if (bGraphRun == TRUE) {
			CloseDataGraph();
			CGUI_App::Wait(1);
			GraphData();
		}	
		return(TRUE);
	}	
	return(FALSE);
}



/*===========================================================================
 *
 *  Name	 :  	UpdateDialogBox
 *
 *  Purpose	 :		Displays dialog box where user selects which options
 *					to automatically update when running in real-time mode
  *
 *  Return value :  TRUE - okay to exit dialog box
 *					FALSE - can't exit before choosing a path with some files
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
 *
===========================================================================*/
static BOOL UpdateDialogBox()
{                                 
    static BOOL bFirst = TRUE;
    int status = TRUE;     
    
    GUI_ACTION DlgReturn;
                                        
	// only display dialog the first time                                        
    if (bFirst == TRUE) { 
    
	   /* ------------------------------------------------------------------
	    *	Get the current defaults
	    * ------------------------------------------------------------------*/
	/* 	status = AccessDefaultDatabase(READ, SZ_IMPORT_PATH, szPaths);
		if (status == TRUE) {
			status = AccessDefaultDatabase(READ, SZ_IMPORT_WILDCARD, szWildCard);
		}	
		if (status == TRUE) {
			status = AccessDefaultDatabase(READ, B_IMPORT_ALL_OVERWRITE, &bOverwrite);
		}	
		if (status == TRUE) {
			status = AccessDefaultDatabase(READ, B_IMPORT_ALL_CLEAR, &bInitDB);
		}
		else {
			display_err_msg1(status, "when reading from Default database.  Cannot proceed.");			
			return(FALSE);
		}	
	*/
	}
   /* ------------------------------------------------------------------
    *	Define the buttons and actions in the dialog box and get the
    *	choices for this session
    * ----------------------------------------------------------------*/
    CGUI_Dlg Dlg(IDD_UPDATE_DIALOG, NULL, DLG_POSN_NO_SAVE);
    if (Dlg.IsValid()) {
	    
	   	Dlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		Dlg.DefinePushBtn(IDOK, GUI_OK);
//		Dlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateDbParms);            
				                                                    
		Dlg.DefineCheckBox(IDC_UPDATE_CHC1, &bAllDataSetup);						                                                    
		Dlg.DefineCheckBox(IDC_UPDATE_CHC2, &bTodaysDataSetup);		
		Dlg.DefineCheckBox(IDC_UPDATE_CHC3, &bDataIntegritySetup);		
		Dlg.DefineCheckBox(IDC_UPDATE_CHC4, &bChannelEventsSetup);		
		Dlg.DefineCheckBox(IDC_UPDATE_CHC5, &bRadiationEventsSetup);		
		
		Dlg.DefineCheckBox(IDC_UPDATE_CHC6, &bAllDataRun);		
		Dlg.DefineCheckBox(IDC_UPDATE_CHC7, &bTodaysDataRun);		
		Dlg.DefineCheckBox(IDC_UPDATE_CHC8, &bDataIntegrityRun);		
		Dlg.DefineCheckBox(IDC_UPDATE_CHC9, &bChannelEventsRun);		
		Dlg.DefineCheckBox(IDC_UPDATE_CHC10, &bRadiationEventsRun);		
		Dlg.DefineCheckBox(IDC_UPDATE_CHC11, &bGraphRun);
		Dlg.SetFocus(IDOK);		        
	    DlgReturn = Dlg.Go();
			    
	    if (DlgReturn == GUI_OK) {  
	    	Dlg.RetrieveAllControlValues();
	    	bDialogOk = TRUE;
	    	bFirst = FALSE;
	    	return(TRUE);
	    }	
	    else {
	    	return(FALSE);
	    }	
	}    
	else {
		return(FALSE);
	}	
}   		                                 
                                 
                      
void AutomaticUpdate()
{
	if (bDialogOk == FALSE) {
		if (UpdateDialogBox())
			UpdateDoIt();
	}
	else UpdateDoIt();		
}	

void MenuAutomaticUpdate()
{
	if (UpdateDialogBox())
		UpdateDoIt();
}	

	                      