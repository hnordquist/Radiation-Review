///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgImportAll.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//	ImportAllMenuOption()
//	ImportAllDialogBox()
//	ImportAllDoIt()
//	DetermineAllDirectories()
//	
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"    
#include "RADInit.H"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DlgImportAll.h"
#include "InputDataFiles.h"
#include "Plot.h"
#include "Utilities.h"

extern RADInit *pRAD_Init;  
extern char glszOldDbName[]; 

static char gllszPaths[PATH_LEN+1] = "";
static char gllszWildCards[WILDCARDS+1] = "";
static BOOL gllbAllOverwrite = TRUE;
static BOOL gllbAllInitDB = TRUE;   
static BOOL gllbDialogParmsOk = FALSE;   
static BOOL gllbSkipOutOfOrder = FALSE;
static BOOL gllbIncludeSubfolders = TRUE;



// static functions in this file
static void UpdateValuesForIni(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void UpdateINI_Parms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void ReadINI_Parms();
static BOOL ValidateImportPath(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);
static BOOL DoIt(BOOL bPrompt, BOOL bClearIfRequested);
static BOOL OnlyDialogBox(BOOL bAuto);


/* <f> */
/*===========================================================================
 *
 *  Name	 :  	ImportAllMenuOption
 *
 *  Purpose	 :		For the main menu option, combine the separate functions of
 *					displaying the dialog box and actually doing the import.
 *
 *  Return value :  none
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
 *	15-Oct-1996 SFK		Broke main import routine apart.
 *
===========================================================================*/
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Perform both the dialog box and import with option to save to default.  Called from Main Menu selection
///////////////////////////////////////////////////////////////////////////////////////////////////
void ImportAllMenuOption()
{

	if (OnlyDialogBox(FALSE))	// display the dialog box and get inputs
		DoIt(TRUE, TRUE);		// import the specified files
}	

///////////////////////////////////////////////////////////////////////////////////////////////
//	Perform only the dialog box setup. 	Called when started with /SETUP
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ImportAllDialogBox()	                      
{

	return(OnlyDialogBox(TRUE));
}	

///////////////////////////////////////////////////////////////////////////////////////////////////
//	Perform just the import.  Called when started with /IMPORT
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ImportAllDoIt(BOOL bClearIfRequested)	                      
{

	ReadINI_Parms();	// read the parameters to use this time		
	return(DoIt(FALSE, bClearIfRequested));          // do the import
}	



/*===========================================================================
 *
 *  Name	 :  	DetermineAllDirectories
 *
 *  Purpose	 :		Finds all the subdirectories under the root path.
 *
 *
 *  Return value :  TRUE - browse completed
 *					FALSE- either not valid root path or >1 root path
 *
 *  Special notes:  
 *
 *  Implicit Input :
    				    				
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *	18-Nov-1998 SFK		Created
 *	06-Jul-1999  SFK Version 2.04 - changed check for FILE_ATTRIBUTE_DIRECTORY to check
 *							`		if that bit is set rather than having the patterns match exactly
 *
===========================================================================*/
BOOL DetermineAllDirectories(char *pDlgPath, CGFmtStr *pAllPaths, unsigned int *puiNumPaths)
{
	char szTemp[PATH_LEN+1];
	
	unsigned int uiNumFound;
	unsigned int ui;
	int iLen;
	int iDum, iDum1;
	int status;
	long lAttributes;
	CGFmtStr szMsg;
	int iTemp;
	GUI_ACTION response;
	HCURSOR OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

	// Determine how many valid paths are in the pDlgPath entry.
	status = CountPathsAndFiles(pDlgPath, "*.*", (int *)puiNumPaths, &iDum, &iDum1); 
	::SetCursor(OldCur);
	if (status != TRUE) return(FALSE);
	if (*puiNumPaths != 1) return(FALSE);	// only allow one path for the browse

	OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));
	CGStrPtrDynaArray *pResult = BrowseDirectory("*.", pDlgPath, TRUE);
	::SetCursor(OldCur);

	OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));
	uiNumFound = pResult->size();
	pAllPaths->Printf("%s;", pDlgPath);		// always include the root
	*puiNumPaths = 1;
	for (ui=0; ui<uiNumFound; ui++) {
		MyStrnCpy(szTemp, (*pResult)[ui]->Get_sz(), PATH_LEN);	// char * to work with
		iLen = strlen(szTemp);
		lAttributes = GetFileAttributes(szTemp); // address of the name of a file or directory 
		if ((lAttributes &FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {	// only examine the subdirectories
			if ((szTemp[iLen-1] != '.') && // skip all . and .. paths
				(strstr((strupr(szTemp)), "ARCHIVE") == NULL)) {	// skip folders named archive
				if ((strlen(pAllPaths->Get_sz()) + strlen(szTemp)) >= PATHS) {
					szMsg.Printf("The subfolder list generated with the root path of %s contains %d ", pDlgPath, uiNumFound);
					iTemp = PATHS;
					szMsg.PrintfAppend("files and is too long.  Do you want to proceed using only the first %d characters?", iTemp); 
					response = GUI_MsgBox(szMsg, GUI_ICON_QUESTION, GUI_NOYES);
					if (response == GUI_YES) break;
					*puiNumPaths = 0;
					::SetCursor(OldCur);
					return(FALSE);
				}
				pAllPaths->PrintfAppend("%s;", szTemp);
				(*puiNumPaths)++;
			}
		}
	}
	::SetCursor(OldCur);

	BrowseCleanup(pResult);
	return(TRUE);
}




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
	pDlg->FldTxtGetValue(IDC_IMPORT_EDIT_PATH, &Str, PATH_LEN);
	pRAD_Init->Set_Import_All_Path((const char *)Str);

	pDlg->FldTxtGetValue(IDC_IMPORT_EDIT_WILDCARD, &Str, WILDCARDS);
	pRAD_Init->Set_Import_All_Wildcard((const char *)Str);
	
	pDlg->CheckBoxGetValue(IDC_IMPORT_OVERWRITE_BOX, &bBool);
	pRAD_Init->Set_Import_All_Overwrite(bBool);
	
	pDlg->CheckBoxGetValue(IDC_IMPORT_INITDB_BOX, &bBool);
	pRAD_Init->Set_Import_All_Init(bBool);

	pDlg->CheckBoxGetValue(IDC_IMPORT_SUBFOLDER_BOX, &bBool);
	pRAD_Init->Set_Import_All_Subfolders(bBool);
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
	
	pStr = pRAD_Init->Get_Import_All_Path();
	MyStrnCpy(gllszPaths, pStr, PATH_LEN); 
		
	pStr = pRAD_Init->Get_Import_All_Wildcard(); 
	MyStrnCpy(gllszWildCards, pStr, WILDCARDS);
		
	gllbAllOverwrite = pRAD_Init->Get_Import_All_Overwrite();
	gllbAllInitDB = pRAD_Init->Get_Import_All_Init();
	gllbIncludeSubfolders = pRAD_Init->Get_Import_All_Subfolders();
		
	gllbDialogParmsOk = TRUE;
}         

 
 
/*===========================================================================
 *
 *  Name	 :  	ValidateImportPath
 *
 *  Purpose	 :		Called from the dialog box when user tries to exit it by
 *					clicking on either OK or Cancel.  If clicked OK, then
 *					checks to see if there are any files matching the 
 *					condition at the specified path.  If no files are present
 *					do not allow him to exit because he has probably typed in 
 *					an incorrect path name.
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
 *	18-Nov-1998 SFK		Added Browse action
 *
===========================================================================*/
static BOOL ValidateImportPath(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction)
{
    char szPaths[PATH_LEN+1];
    char szWildCards[WILDCARDS+1];
    int  iNumPaths;
	unsigned int uiNumPaths;
    int  iNumWildcards;
	int  iTotalMatchingFiles;
    int status;
	CGFmtStr szAllPaths;
	BOOL bBool;

	if (ClosingAction == GUI_OK) {    
		pDlg->FldTxtGetValue(IDC_IMPORT_EDIT_PATH, szPaths, PATH_LEN);
		pDlg->FldTxtGetValue(IDC_IMPORT_EDIT_WILDCARD, szWildCards, WILDCARDS);

	   /* ------------------------------------------------------------------
		*   Check to see whether they want to browse subdirectories.
		*	If yes, then figure out all the subdirectory paths underneath
		*	the root specified.  See if there are any files at the specified
		*	path.  If yes, then can exit the option.  If not, don't allow
		*	them to exit the dialog box.
		* ----------------------------------------------------------------*/
		pDlg->CheckBoxGetValue(IDC_IMPORT_SUBFOLDER_BOX, &bBool);
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

         
/* <f> */
/*===========================================================================
 *
 *  Name	 :  	DoIt
 *
 *  Purpose	 :		Actually execute the actions specified by the dialog box.
 *
 *  Return value :  TRUE - action completed
 *					FALSE - chose to abort option by not erasing the database or
 *							chose to cancel the import process by hitting cancel
 *							on progress bar box.
 *
 *  Special notes:  
 *
 *  Implicit Input :	dialog box options set by running ImportAllDialogBox prior
 *						to executing this function
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  16-Mar-1996 SFK		Created
 *	15-Oct-1996 SFK		Broke function apart from main import routine.
 *	21-Jan-1999 SFK		Added bClearIfRequested functionality for case of
 *						automated input with multiple disks
 *
===========================================================================*/
static BOOL DoIt(BOOL bPrompt, BOOL bClearIfRequested)
{   

	int status;
	CGFmtStr szAllPaths;
	unsigned int uiNumPaths;

	SetCurrentDrive();	
	if (gllbDialogParmsOk != TRUE) return(FALSE);		// have not yet setup the parameters


   /* ------------------------------------------------------------------
	*   Initialize the database if requested by the setting in the dialog box.
	* ----------------------------------------------------------------*/
	if ((gllbAllInitDB) && (bClearIfRequested)) {
		status = InitializeDatabase(bPrompt);
		if (status != TRUE) return(FALSE); 	
	}		

   /* ------------------------------------------------------------------
	*   Import all the files in the specified directory(s) into the database.
	* ----------------------------------------------------------------*/
	if (gllbIncludeSubfolders == TRUE) {
		status = DetermineAllDirectories(gllszPaths, &szAllPaths, &uiNumPaths);
		if (status == TRUE) {
			status = InputAllFiles((char *)szAllPaths.Get_sz(), gllszWildCards, gllbAllOverwrite, FALSE);
		}
		else {
			if (uiNumPaths > 0) status = InputAllFiles(gllszPaths, gllszWildCards, gllbAllOverwrite, FALSE); 
		}
	}
	else {
		if ((gllbAllInitDB) && (bClearIfRequested)) {
			status = InitializeDatabase(bPrompt);
			if (status != TRUE) return(FALSE); 	
		}		
		status = InputAllFiles(gllszPaths, gllszWildCards, gllbAllOverwrite, FALSE); 
	}
	
	SaveDatabaseNow();	// save the database after the import is complete 
	UpdateGraphData(FALSE); // if the graph is displayed, update its data now.
	
	if (status == CANCELED) return(FALSE);
	return(TRUE);
}


/* <f> */
/*===========================================================================
 *
 *  Name	 :  	ImportAllDialogBox
 *
 *  Purpose	 :		Displays dialog box where user selects path(s) and
 *                  wildcard filename of files to import and whether or not to
 *					overwrite files or initialize the database.
 *
 *  Return value :  TRUE - selected new valid parameters
 *					FALSE - invalid parameters or chose to cancel
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
 *	15-Oct-1996 SFK		Broke function apart from main import routine.
 *
===========================================================================*/
static BOOL OnlyDialogBox(BOOL bAuto)
{                                 
    static BOOL bFirst = TRUE;
//	static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = "";

	char szPaths[PATH_LEN+1];
	char szWildCards[WILDCARDS+1]; 
	
    GUI_ACTION DlgReturn;
    
   /* ------------------------------------------------------------------
    *	If this is the first time in option or you have switched databases,
	*	use the defaults read from the rad.ini file.
    * ------------------------------------------------------------------*/
    SetCurrentDrive();
    if ((bFirst == TRUE) || (!SameDb(glszOldDbName))) {
    	ReadINI_Parms();
   		bFirst = FALSE;
   		GetDbName(glszOldDbName);
    }	

   /* ------------------------------------------------------------------
    *	Use the path just read from ini if first time or 
	*	last known good path if not first time in option
    * ----------------------------------------------------------------*/
    MyStrnCpy(szWildCards, gllszWildCards, WILDCARDS);
    if (PathCopy(szPaths, gllszPaths) != TRUE) strcpy(szPaths, "C:\\"); // default to c: if problem
    
   /* ------------------------------------------------------------------
    *	Define the buttons and actions in the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg ImportAllDlg(IDD_IMPORT_DIALOG, NULL, DLG_POSN_NO_SAVE);
    if (ImportAllDlg.IsValid()) {
    
	   	ImportAllDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		ImportAllDlg.DefinePushBtn(IDOK, GUI_OK);
		ImportAllDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateINI_Parms);            
					                                               
		ImportAllDlg.DefineFldStaticTxt(IDC_IMPORT_DEFAULT_DB_NAME);
		ImportAllDlg.FldStaticTxtSetValue(IDC_IMPORT_DEFAULT_DB_NAME, glszOldDbName);
			
		ImportAllDlg.DefineFldTxt(IDC_IMPORT_EDIT_PATH, szPaths, PATH_LEN);
		ImportAllDlg.DefineFldTxt(IDC_IMPORT_EDIT_WILDCARD, szWildCards, WILDCARDS);
		ImportAllDlg.DefinePushBtn(IDC_IMPORT_BROWSE, GUI_BROWSE);
		ImportAllDlg.SetBrowseOutputCtrl(IDC_IMPORT_BROWSE, IDC_IMPORT_EDIT_PATH);
			
		ImportAllDlg.DefineCheckBox(IDC_IMPORT_OVERWRITE_BOX, &gllbAllOverwrite);		
		ImportAllDlg.DefineCheckBox(IDC_IMPORT_INITDB_BOX, &gllbAllInitDB);		
		ImportAllDlg.DefineCheckBox(IDC_IMPORT_SUBFOLDER_BOX, &gllbIncludeSubfolders);		
		
		ImportAllDlg.SetDlgExitValidateFn(ValidateImportPath);	// check for path with some files
		ImportAllDlg.SetFocus(IDOK);
	    DlgReturn = ImportAllDlg.Go();
	    if (DlgReturn == GUI_OK) {  
	    	ImportAllDlg.RetrieveAllControlValues();
	    	gllbDialogParmsOk = TRUE;	// have good values for DoIt
			MyStrnCpy(gllszPaths,szPaths, PATH_LEN);	// remember these as the last good values
			MyStrnCpy(gllszWildCards, szWildCards, WILDCARDS);
			// if got to this dialog box thru /SETUP start, then save values for ini write
			if (bAuto) UpdateValuesForIni(&ImportAllDlg, IDD_IMPORT_DIALOG);
	    	return(TRUE);
	    }	
    	return(FALSE); // chose to exit dialog box with Cancel
	}    
	return(FALSE); // problem creating dialog box
}   		                                 
   		                                 
                                 
                      
