///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgImportLatest.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//	ImportTodayDialogBox()
//	ImportTodayDoIt()
//	ImportTodayMenuOption()
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"    
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DbLimit.h"
#include "DlgImportAll.h"
#include "DlgImportLatest.h"
#include "InputDataFiles.h"
#include "Plot.h"
#include "RADInit.H"
#include "Utilities.h"

extern RADInit *pRAD_Init;
extern char glszOldDbName[];  

static char gllszPaths[PATH_LEN+1] = "";
static char gllszWildCards[WILDCARDS+1] = "";

//SCR 332 pjm 4/1/08  (want first two to be false)
static BOOL gllbTodayOverwrite = TRUE;
static BOOL gllbTodayInitDB = TRUE;   
static BOOL gllbTodayLastFiles = TRUE;

   
static BOOL gllbTodayDialogOk = FALSE;   
static BOOL gllbIncludeSubfolders = TRUE;   

// static functions for this file
static void UpdateValuesForIni(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void UpdateINI_Parms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void ReadINI_Parms();
static BOOL ValidateImportPath(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);



/*===========================================================================
 *
 *  Name	 :  	UpdateValuesForIni, 
 *  Purpose	 :		Reads the current values from the dialog box and gets them
 *					to the proper variables for the ini write that is coming.
 *
 *  Name	 :  	UpdateINI_Parms
 *  Purpose	 :		Called from the dialog box when the NewDefaults button is pushed.
 *					Asks the user whether he wants to update the ini file, if yes
 *					reads the latest values from the dialog box and then writes them
 *					to the current .ini file.
 *
 *  Return value :  None
 *
 *  Special notes:  
 *
 *  Implicit Input :	rad.ini file is located in subdirectory of db_Vista database
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  16-Mar-1996 SFK		Created
 *	15-Jan-1997 SFK		Changed to write to rad.ini file.
 *
===========================================================================*/
static void UpdateValuesForIni(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    CGStr Str; 
    BOOL bBool;
    	             
   /* ------------------------------------------------------------------
    *	Get the current values for the path and extension fields and
    *	check boxes and update the current options values.
    * ----------------------------------------------------------------*/
	pDlg->FldTxtGetValue(IDC_IMPORTT_EDIT_PATH, &Str, PATH_LEN);
	pRAD_Init->Set_Import_Today_Path((const char *)Str);

	pDlg->FldTxtGetValue(IDC_IMPORTT_EDIT_WILDCARD, &Str, WILDCARDS);
	pRAD_Init->Set_Import_Today_Wildcard((const char *)Str);
	
	//SCR 332 pjm 4/1/08
	pDlg->CheckBoxGetValue(IDC_IMPORTT_OVERWRITE_BOX, &bBool);
	//pRAD_Init->Set_Import_Today_Overwrite(bBool);
	pRAD_Init->Set_Import_Today_Overwrite(FALSE);
	
	pDlg->CheckBoxGetValue(IDC_IMPORTT_INITDB_BOX, &bBool);
	//pRAD_Init->Set_Import_Today_Init(bBool);
	pRAD_Init->Set_Import_Today_Init(FALSE);

	pDlg->CheckBoxGetValue(IDC_IMPORTT_SUBFOLDER_BOX, &bBool);
	pRAD_Init->Set_Import_Today_Subfolders(bBool);

	pDlg->CheckBoxGetValue(IDC_IMPORTT_ALL_BOX, &bBool);
	//pRAD_Init->Set_Import_Today_Since_Last(bBool);
	pRAD_Init->Set_Import_Today_Since_Last(TRUE);
}

static void UpdateINI_Parms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{

    int status; 
    	             
	status = AskUpdateINI_Question();
	if (status != TRUE) return;
	
	UpdateValuesForIni(pDlg, CtrlID);
   	pRAD_Init->Save_Parameters();
}        


/*===========================================================================
 *
 *  Name	 :  	ReadINI_Parms
 *
 *  Purpose	 :		Set static global variables to values that were read from
 *					the .ini file
 *                                                                                             
 *  Return value :  None
 *
 *  Special notes:  
 *
 *  Implicit Input :	rad.ini file is located in subdirectory of db_Vista database
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  02-May-1997 SFK		Created
 *
===========================================================================*/
static void ReadINI_Parms()
{
	char *pStr;
	
	pStr = pRAD_Init->Get_Import_Today_Path();
	MyStrnCpy(gllszPaths, pStr, PATH_LEN); 
		
	pStr = pRAD_Init->Get_Import_Today_Wildcard(); 
	MyStrnCpy(gllszWildCards, pStr, WILDCARDS);

	//SCR 332 pjm 4/1/08
	//gllbTodayOverwrite = pRAD_Init->Get_Import_Today_Overwrite();		
	//gllbTodayInitDB = pRAD_Init->Get_Import_Today_Init();
	//gllbTodayLastFiles = pRAD_Init->Get_Import_Today_Since_Last();

	gllbIncludeSubfolders = pRAD_Init->Get_Import_Today_Subfolders();
		
}         


/*===========================================================================
 *
 *  Name	 :  	ValidatePath
 *
 *  Purpose	 :		Called from the dialog box when user tries to exit it.
 *					Checks to see if there are any files matching the 
 *					condition at the specified path.
 *
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
 *	17-Aug-1998	SFK		Added support of multiple wildcards
 *
===========================================================================*/
static BOOL ValidateImportPath(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction)
{
    char szPaths[PATH_LEN+1];
    char szWildCards[WILDCARDS+1];
    int  iNumPaths;
    int  iTotalMatchingFiles, iNumWildcards; 
    CGFmtStr msg;              
    BOOL bOkToExit = FALSE;
    int status;
	BOOL bBool;
	CGFmtStr szAllPaths;
	unsigned int uiNumPaths;

	if (ClosingAction == GUI_OK) {    
		pDlg->FldTxtGetValue(IDC_IMPORTT_EDIT_PATH, szPaths, PATH_LEN);
		pDlg->FldTxtGetValue(IDC_IMPORTT_EDIT_WILDCARD, szWildCards, WILDCARDS);                                                 

	   /* ------------------------------------------------------------------
		*   Check to see whether they want to browse subdirectories.
		*	If yes, then figure out all the subdirectory paths underneath
		*	the root specified.  See if there are any files at the specified
		*	path.  If yes, then can exit the option.  If not, don't allow
		*	them to exit the dialog box.
		* ----------------------------------------------------------------*/
		pDlg->CheckBoxGetValue(IDC_IMPORTT_SUBFOLDER_BOX, &bBool);
		if (bBool == TRUE) {
			status = DetermineAllDirectories(szPaths, &szAllPaths, &uiNumPaths);
			if (status == TRUE) {
				status = CountPathsAndFiles((char *)szAllPaths.Get_sz(), szWildCards, &iNumPaths, &iTotalMatchingFiles, &iNumWildcards); 
			}
			else {
				if (uiNumPaths > 1) {
					RadReviewMsg(uiBROWSE_INFO_WARN, szPaths);
					return(TRUE);
				}
				else {
					return(FALSE);
				}
			}
		}
		else {
			status = CountPathsAndFiles(szPaths, szWildCards, &iNumPaths, &iTotalMatchingFiles, &iNumWildcards); 
		}
		if (status != TRUE) return (FALSE);
	}
	return(TRUE);
}


BOOL ImportTodayDialogBox()
{                                 
    static BOOL bFirst = TRUE;
//static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = "";
	static char szPaths[PATH_LEN+1];
	static char szWildCards[WILDCARDS+1];
	static BOOL bOverwrite = TRUE;
	static BOOL bInitDB = TRUE;   
    
    GUI_ACTION DlgReturn;

   /* ------------------------------------------------------------------
    *	If this is the first time in option, use the defaults from
    *	the ini file.
    * ------------------------------------------------------------------*/
	SetCurrentDrive();
    if ((bFirst == TRUE) || (!SameDb(glszOldDbName))) {
		ReadINI_Parms();
    	GetDbName(glszOldDbName);
   		bFirst = FALSE;
    }	

   /* ------------------------------------------------------------------
    *	Use the last known good path (gllxxxx set during validate function)
    * ----------------------------------------------------------------*/
    if (PathCopy(szPaths, gllszPaths) != TRUE) strcpy(szPaths, "C:\\");
    MyStrnCpy(szWildCards, gllszWildCards, WILDCARDS);
    
   /* ------------------------------------------------------------------
    *	Define the buttons and actions in the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg ImportTodayDlg(IDD_IMPORT_TODAY_DIALOG, NULL, DLG_POSN_NO_SAVE);
    if (ImportTodayDlg.IsValid()) {
    
	   	ImportTodayDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		ImportTodayDlg.DefinePushBtn(IDOK, GUI_OK);
		ImportTodayDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateINI_Parms);            
			                                               
		ImportTodayDlg.DefineFldStaticTxt(IDC_IMPORTT_DEFAULT_DB_NAME);
		ImportTodayDlg.FldStaticTxtSetValue(IDC_IMPORTT_DEFAULT_DB_NAME, glszOldDbName);
			
		ImportTodayDlg.DefineFldTxt(IDC_IMPORTT_EDIT_PATH, szPaths, PATH_LEN);
		ImportTodayDlg.DefineFldTxt(IDC_IMPORTT_EDIT_WILDCARD, szWildCards, WILDCARDS);
		ImportTodayDlg.DefinePushBtn(IDC_IMPORTT_BROWSE, GUI_BROWSE);
		ImportTodayDlg.SetBrowseOutputCtrl(IDC_IMPORTT_BROWSE, IDC_IMPORTT_EDIT_PATH);

		//SCR 332 pjm 4/1/08 -- uncommented 4-4-08 HN, things are broken.
		ImportTodayDlg.DefineCheckBox(IDC_IMPORTT_OVERWRITE_BOX, &gllbTodayOverwrite);
		ImportTodayDlg.DefineCheckBox(IDC_IMPORTT_INITDB_BOX, &gllbTodayInitDB);		
		ImportTodayDlg.DefineCheckBox(IDC_IMPORTT_ALL_BOX, &gllbTodayLastFiles);
		
		ImportTodayDlg.SetVisible (IDC_IMPORTT_OVERWRITE_BOX, FALSE);
		ImportTodayDlg.SetVisible (IDC_IMPORTT_INITDB_BOX, FALSE);
		ImportTodayDlg.SetVisible (IDC_IMPORTT_ALL_BOX, FALSE);

		ImportTodayDlg.DefineCheckBox(IDC_IMPORTT_SUBFOLDER_BOX, &gllbIncludeSubfolders);		
		ImportTodayDlg.SetDlgExitValidateFn(ValidateImportPath);	
		ImportTodayDlg.SetFocus(IDOK);
		

	    DlgReturn = ImportTodayDlg.Go();
		    
	    if (DlgReturn == GUI_OK) {  
	    	ImportTodayDlg.RetrieveAllControlValues();
			MyStrnCpy(gllszPaths,szPaths, PATH_LEN);
			MyStrnCpy(gllszWildCards, szWildCards, WILDCARDS);
	    	gllbTodayDialogOk = TRUE;
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


BOOL ImportTodayDoIt()
{   
	int status;
	DATE dDisplayStartTime, dViewPeriodInDATE, dDbStartTime, dDbEndTime;
	CGFmtStr szAllPaths;
	unsigned int uiNumPaths;
	struct IDStruct StaID;

	                                     
	SetCurrentDrive();

	HCURSOR OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

	// if the dialog box wasn't displayed during this run of the program, use the last
	// stuff written to the .ini
	if (gllbTodayDialogOk != TRUE) { 
		ReadINI_Parms();
	}
		
   /* ------------------------------------------------------------------
	*   Init the database if requested.
	* ----------------------------------------------------------------*/
	//SCR 332 pjm 4/1/08
	//if (gllbTodayInitDB) {
	//	status = InitializeDatabase(TRUE);
	//	if (status != TRUE) return(FALSE); 	
	//}		
				
   /* ------------------------------------------------------------------
	*   Import all the files in the specified directory(s) into the database.
	* ----------------------------------------------------------------*/
	if (gllbIncludeSubfolders == TRUE) 
	{
		status = DetermineAllDirectories(gllszPaths, &szAllPaths, &uiNumPaths);
		if (status == TRUE) 
		{
			//SCR 332 pjm 4/1/08
			status = InputTodaysFiles((char *)szAllPaths.Get_sz(), gllszWildCards, FALSE, TRUE);
			//status = InputTodaysFiles((char *)szAllPaths.Get_sz(), gllszWildCards, gllbTodayOverwrite, gllbTodayLastFiles);
		}
		else 
		{
			if (uiNumPaths > 0)
				//SCR 332 pjm 4/1/08				
				status = InputTodaysFiles(gllszPaths, gllszWildCards, FALSE, TRUE); 
				//status = InputTodaysFiles(gllszPaths, gllszWildCards, gllbTodayOverwrite, gllbTodayLastFiles); 
		}
	}
	else 
	{
		//SCR 332 pjm 4/1/08
		status = InputTodaysFiles(gllszPaths, gllszWildCards, FALSE, TRUE); 
		//status = InputTodaysFiles(gllszPaths, gllszWildCards, gllbTodayOverwrite, gllbTodayLastFiles); 
	}
	::SetCursor(OldCur);
   /* ------------------------------------------------------------------
	*	Close and Open the database for database stability.
	*	Display the most recent interval in the db if the graph is up.
	* ----------------------------------------------------------------*/
	SaveDatabaseNow();
	if (GraphDisplayedNow()) {
		OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));
		GraphGetViewPeriodParameters(&dDisplayStartTime, &dViewPeriodInDATE);
		StaID.sStaID = ANY_STATION;
		status = GetDbTimeRange(StaID, &dDbStartTime, &dDbEndTime);
		dDisplayStartTime = dDbEndTime - dViewPeriodInDATE;
		GraphSetDisplayDate(dDisplayStartTime, dViewPeriodInDATE);	// this call updates the graph data
		::SetCursor(OldCur);
		GraphSetCursorTime(dDbEndTime);	// place cursor on last time in db

	}	
	return(TRUE);
}


void ImportTodayMenuOption()
{
	if (ImportTodayDialogBox())
		ImportTodayDoIt();
}	                      