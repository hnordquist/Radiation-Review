///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgImportSome.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//	ImportSomeMenuOption()
///////////////////////////////////////////////////////////////////////////

#include <gui_tbl.h>        
#include <malloc.h>
#include <time.h>
#include "GetFile.h"
#include "RAD.H"    
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DbImport.h"
#include "DlgImportAll.h"
#include "DlgImportSome.h"
#include "FacilityConfig.h"
#include "Plot.h"
#include "ProgressBars.h"
#include "RADInit.H"
#include "Utilities.h"
#include "TypeDefinitions.h"

extern RADInit *pRAD_Init;  
extern CGUI_TextWin *glImportWinPtr;	// declared in inputall.cpp
extern short glsFacNum; //KM
extern char glszOldDbName[]; 

static int glliNumRows;	// total number of rows in table.
static UINT glluiSelectBoxCol;  
static BOOL *pgllSelect = NULL;
static UINT glluiFileNamesCol;  
static char **pgllFileNames = NULL;
static char (*gllFileNames)[DOSNAME_LEN+1] = NULL;
static UINT glluiDateCol;  
static char **pgllDates = NULL;
static char (*gllDates)[DT_LEN+1] = NULL;
static UINT gllusStationCol;  
static char **pgllStations = NULL;
static char (*gllStations)[DB_NAME_LEN+1] = NULL;
static char (*gllPaths)[PATH_LEN+1] = NULL;
static char gllszPaths[PATH_LEN+1];
static char gllszWildcard[WILDCARDS+1]; 
static BOOL gllbIncludeSubfolders = TRUE;
static RESOURCE_ID gllCancelBtnHit;

// static functions
static void UpdateINI_Parms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void CancelImportBtnServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL CloseImportLogWindow(CGUI_TextWin *pWin);
static void DeleteAllAllocatedLists();
static int CreateTableLists(CGUI_Dlg *pDlg, char *szOrigPaths, char *szOrigWildcards, int *piNumEntries);
static BOOL UpdateTable(CGUI_Dlg *pDlg);
static BOOL ValidatePathAndUpdateTable(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);
static void ButtonUpdateTable(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);

/* <f> */
/*===========================================================================
 *
 *  Name	 :  	ImportSomeMenuOption
 *
 *  Purpose	 :		Called from the main menu.  Displays the options of
 *					files to import and imports those selected.
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
 //	27-Oct-2003	SFK		Modified to use file extension of *.bny supported by MIC 1.805+
 *
===========================================================================*/
void ImportSomeMenuOption()
{                                 
    static BOOL bOverwrite;
    static BOOL bInitDB;   
    static BOOL bFirst = TRUE;
//	static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = "";
	
    
    CGUI_Table *pTbl = NULL;
    GUI_ACTION DlgReturn;
    CGFmtStr msg, msg2;

    char WholeName[PATH_LEN+DOSNAME_LEN+1];
    char LastPath[PATH_LEN+1];
	char szTempPaths[PATHS+1];
    long lStart, lEnd, lCurrent;
    int iNumImported, iNumToImport;
    int i;
    int status;
    int iNumPaths, iNumTotalFiles, iNumWildcards;
    char *pStr;
	CGFmtStr szAllPaths;
	unsigned int uiNumPaths;
	struct _finddata_t file;
	long lFileHandle;

    

	SetCurrentDrive();    

	strcpy (szTempPaths,"\\"); // default to drive root LOL 
   
   /* ------------------------------------------------------------------
    *	Get the current defaults from the ini file and set the local globals
    * ------------------------------------------------------------------*/
    if ((bFirst == TRUE) || (!SameDb(glszOldDbName))) {
   		pStr = pRAD_Init->Get_Import_Select_Path();
		MyStrnCpy(gllszPaths, pStr, PATH_LEN); 
		
		pStr = pRAD_Init->Get_Import_Select_Wildcard(); 
		MyStrnCpy(gllszWildcard, pStr, WILDCARDS);
		
		bOverwrite = pRAD_Init->Get_Import_Select_Overwrite();
		bInitDB = pRAD_Init->Get_Import_Select_Init();

		gllbIncludeSubfolders = pRAD_Init->Get_Import_Select_Subfolders();

    	GetDbName(glszOldDbName);
   		bFirst = FALSE;
    }	
        
   /* ------------------------------------------------------------------
    *	Check if there are any files in any of the subdirectories.
    *	Create list of files.  If there are no paths that exist, then default to root.
    * ------------------------------------------------------------------*/
	if (gllbIncludeSubfolders == TRUE) {
		status = DetermineAllDirectories(gllszPaths, &szAllPaths, &uiNumPaths);
		if (status == TRUE) { // found only one root
			status = PathsCopy(szTempPaths, (char *)szAllPaths.Get_sz());
		}
		else {
			if (uiNumPaths > 1)	
				RadReviewMsg(uiBROWSE_INFO_WARN, gllszPaths);	// warning about more than one path on browse
		}
	}
	else {
		PathCopy(szTempPaths, gllszPaths);
	}


	status = CountPathsAndFiles(szTempPaths, gllszWildcard, &iNumPaths, &iNumTotalFiles, &iNumWildcards);
	if ((status == uiPATH_DOES_NOT_EXIST_ERR) || (status == uiDRIVE_DOES_NOT_EXIST_ERR)) {
		strcpy (szTempPaths,"\\");
		strcpy(gllszPaths, szTempPaths);
	}	
    

   /* ------------------------------------------------------------------
    *	Define the buttons and actions in the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg ImportSomeDlg(IDD_IMPORT_SOME_DIALOG, NULL, DLG_POSN_NO_SAVE);
    if (ImportSomeDlg.IsValid()) {

		if (gllbIncludeSubfolders == TRUE) { // using all the subfolders
			status = CreateTableLists(&ImportSomeDlg, szTempPaths, gllszWildcard, &glliNumRows);
		}
		else { // only using a single root path
	    	status = CreateTableLists(&ImportSomeDlg, gllszPaths, gllszWildcard, &glliNumRows);
		}
		// don't check status as always want the box to come up so user could enter new path
//		if (status != TRUE) return;

		ImportSomeDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		ImportSomeDlg.DefinePushBtn(IDOK, GUI_OK);          
		ImportSomeDlg.DefinePushBtn(IDC_ALL_BUTTON, GUI_NONE, SelectAll);          
		ImportSomeDlg.DefinePushBtn(IDC_NONE_BUTTON, GUI_NONE, DeselectAll);    
		ImportSomeDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateINI_Parms);          
		ImportSomeDlg.DefinePushBtn(IDC_UPDATE_TABLE, GUI_NONE, ButtonUpdateTable);          
		
		ImportSomeDlg.DefineFldStaticTxt(IDC_SOME_DEFAULT_DB_NAME);
		ImportSomeDlg.FldStaticTxtSetValue(IDC_SOME_DEFAULT_DB_NAME, glszOldDbName);
			                                               
		ImportSomeDlg.DefineFldTxt(IDC_SOME_EDIT_PATH, gllszPaths, PATH_LEN);
		ImportSomeDlg.DefineFldTxt(IDC_SOME_EDIT_WILDCARD, gllszWildcard, WILDCARDS);
		ImportSomeDlg.DefineCheckBox(IDC_SOME_SUBFOLDER_BOX, &gllbIncludeSubfolders);		

		ImportSomeDlg.DefinePushBtn(IDC_SOME_BROWSE, GUI_BROWSE);
		ImportSomeDlg.SetBrowseOutputCtrl(IDC_SOME_BROWSE, IDC_SOME_EDIT_PATH);
		
	   /* ------------------------------------------------------------------
	    *	Define and create the table
	    * ----------------------------------------------------------------*/
		ImportSomeDlg.DefineTable(IDC_IMPORT_PICKTABLE, &pTbl);	
		if (pTbl != NULL) {
			pTbl->SetColWidths(CGUI_TABLE_COL_WIDTH_SCALED);
			glluiSelectBoxCol = pTbl->AddColCheckBox("Select", glliNumRows, pgllSelect);		
			glluiDateCol = pTbl->AddColTxt("Date", glliNumRows, pgllDates, DT_LEN);
			gllusStationCol = pTbl->AddColTxt("Station", glliNumRows, pgllStations, DB_NAME_LEN);
			glluiFileNamesCol = pTbl->AddColTxt("File Name", glliNumRows, pgllFileNames, DOSNAME_LEN);
			
			pTbl->SetColWriteEnable(glluiDateCol, FALSE);
			pTbl->SetColWriteEnable(gllusStationCol, FALSE);
			pTbl->SetColWriteEnable(glluiFileNamesCol, FALSE);
		}	
			
	   /* ------------------------------------------------------------------
	    *	Define and create the checkboxes
	    * ----------------------------------------------------------------*/
		ImportSomeDlg.DefineCheckBox(IDC_SOME_OVERWRITE, &bOverwrite);		
		ImportSomeDlg.DefineCheckBox(IDC_SOME_INITDB, &bInitDB);		

		ImportSomeDlg.SetDlgExitValidateFn(ValidatePathAndUpdateTable);	// get the files really specified
		ImportSomeDlg.SetFocus(IDC_SOME_EDIT_PATH);
		
	    DlgReturn = ImportSomeDlg.Go();
		    
	    if (DlgReturn == GUI_OK) {  
		    	
	    	ImportSomeDlg.RetrieveAllControlValues();
		   	pTbl->RetrieveAllCellValues();
	
		   /* ------------------------------------------------------------------
			*   Check if any files were selected
			* ----------------------------------------------------------------*/
			iNumToImport = 0;
	    	for (i=0; i<glliNumRows; i++) {
	    		if (pgllSelect[i] == TRUE) {               
				    iNumToImport++;     
			   	}
			}   
			
			if (iNumToImport == 0) { 
				GUI_MsgBox("No files selected to import.", GUI_ICON_INFO);
				goto exit;
			}	
	
	    	
		   /* ------------------------------------------------------------------
			*   Initialize database if requested
			* ----------------------------------------------------------------*/
	       	if (bInitDB) {
	    		status = InitializeDatabase(TRUE);
		   		if (status != TRUE) return;
			}		
			
		   /* ------------------------------------------------------------------
			*   Create log window and write the page header
			* ----------------------------------------------------------------*/
		   	if (glImportWinPtr == NULL) {                
				glImportWinPtr = new CGUI_TextWin("Data Import Log for Selected Files");
				if (glImportWinPtr != NULL) glImportWinPtr->ShowStatusBar(TRUE);
			}   
			else {
				glImportWinPtr->Clear();       
				glImportWinPtr->SetTitle("Data Import Log for Selected Files");
			}	    
			glImportWinPtr->SetFocus();		

			glImportWinPtr->Printf("Database Directory = %s", glszOldDbName);		
		    MyStrnCpy(LastPath, "", PATH_LEN);
			
   			//  Create the progress bar
			CGFmtStr Msg1, Msg2;
			TwoLineProgressBar *pTwoLineProgressBar;
			BOOL bContinue = true;

			Msg1.Printf("");
			Msg2.Printf("");
			// use glliNumRows*2 since call update twice in the loop
			pTwoLineProgressBar = new TwoLineProgressBar(glliNumRows*2, 1, Msg1, Msg2, "Radiation Review: File Import Progress");
			if (pTwoLineProgressBar) {
		                            
				time(&lStart);      
				iNumImported = 0;
			   /* ------------------------------------------------------------------
				*   Import the selected files and calculate the time required for the import
				* ----------------------------------------------------------------*/
			    for (i=0; i<glliNumRows; i++) {
			    	if (pgllSelect[i] == TRUE) {                   
			    		/* indicate if now in another path */
			    		MyStrnCpy(WholeName, gllPaths[i], PATH_LEN);
			    		MyStrnCat(WholeName, gllFileNames[i], PATH_LEN+DOSNAME_LEN);
			    		if (strcmp(LastPath, gllPaths[i]) != 0) {  
							glImportWinPtr->Printf("\n\nImport Path = %s", gllPaths[i]);
							if (strstr(strlwr(WholeName), ".id") != 0) { // id files don't have a date
								glImportWinPtr->Printf("\nFilename");			    	
							}
							else if ((strstr(strlwr(WholeName), "*.bot") == 0) ||
									 (strstr(strlwr(WholeName), "*.bid") == 0) || (strstr(strlwr(WholeName), "*.bi0") == 0) ||
									 (strstr(strlwr(WholeName), "*.bny") == 0) ||
									 (strstr(strlwr(WholeName), "*.evt") == 0)) { // all Coms display times in ms
								glImportWinPtr->Printf("\nDate                        Station  Filename        First Rec       Last Rec     Num Recs");			    
							}
							else if ((strstr(strlwr(WholeName), "*sl*.*") == 0) ||
									 (strstr(strlwr(WholeName), "*.vbf") ==0)) { // VIFM Com displays date/times
								glImportWinPtr->Printf("\nDate                        Station  Filename     First Rec              Last Rec            Num Recs");			    
							}
							else {
								glImportWinPtr->Printf("\nDate                        Station  Filename       First Rec   Last Rec   Num Recs");			    
							}

		    				MyStrnCpy(LastPath, gllPaths[i], PATH_LEN);
		    			}	
						//	Update progress bar to get filename on it
						Msg1.Printf("Processing file %s.\n", WholeName);
						bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
        				status = ImportData(WholeName, bOverwrite, glImportWinPtr, gllFileNames[i], FALSE);
					    if (status != TRUE) {
							/* check that you can access files on the drive */
							lFileHandle = _findfirst(WholeName, &file); /* get all file info */
							_findclose(lFileHandle);
							if (lFileHandle == -1L) {
								if (glImportWinPtr != NULL) glImportWinPtr->Printf("\nImport Error Reading File %s", WholeName);
								if (glImportWinPtr != NULL) glImportWinPtr->Printf("\nImport Aborted Due to System Error.");
								break;
							}
						}
						iNumImported++;     
					}
					
		           /* ------------------------------------------------------------------
					*	Data from this file successfully imported. Update the
					*	total import time so far on the status line and continue importing.
					* ----------------------------------------------------------------*/
					time(&lCurrent);
					Msg2.Printf("Total import time for %d files is %ld secs\n",iNumImported, (lCurrent-lStart));
					bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
					if (!bContinue) break; /* abort if user canceled */ 
				}
			}   
			pTwoLineProgressBar->Close();
			delete pTwoLineProgressBar;

			time(&lEnd); 
			
		   /* ------------------------------------------------------------------
			*   If some files imported, then remember the new path and wildcard.
			*	Close and Open the database for database stability.
			* ----------------------------------------------------------------*/
	       	if (iNumImported > 0) {
	       		SaveDatabaseNow();
				UpdateGraphData(FALSE); // if the graph is displayed, update its data now.
	       	}	
	                      
			glImportWinPtr->Printf("\n\nTotal import time for %d files is %ld secs\n", iNumImported, (lEnd - lStart));
			if (glImportWinPtr != NULL) glImportWinPtr->EnableUserClose(CloseImportLogWindow);	
		}	
	    
exit:    // clean up the stuff used in the table
		if (pgllStations) free(pgllStations);
		pgllStations = NULL;
		if (gllStations) free(gllStations);
		gllStations = NULL;
		if (pgllDates)free(pgllDates);
		pgllDates = NULL;
		if (gllDates)free(gllDates);
		gllDates = NULL;
		if (gllPaths)free(gllPaths);
		gllPaths = NULL;
		if (pgllFileNames)free(pgllFileNames);
		pgllFileNames = NULL;
		if (gllFileNames)free(gllFileNames);
		gllFileNames = NULL;
		if (pgllSelect)free(pgllSelect);
		pgllSelect = NULL;
	}	
	return;
	
}   		                                 
   		                                 
                                 


/*===========================================================================
 *
 *  Name	 :  	UpdateINI_Parms
 *
 *  Purpose	 :		Called from the dialog box when the NewDefaults button is pushed.
 *					The present values are read from the dialog box and written to
 *					the current rad.ini file.
 *
 *
 *  Return value :  None
 *
 *  Implicit Input :
     				    				
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  16-Mar-1996 SFK		Created
 *  16-Jan-1997 SFK		Changed to use rad.ini file
 *
===========================================================================*/
static void UpdateINI_Parms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  
    CGStr Str;
    BOOL bBool;
	             
	status = AskUpdateINI_Question();
	if (status != TRUE) return;
	
   /* ------------------------------------------------------------------
    *	Get the current values for the path and extension fields and
    *	check boxes.
    * ----------------------------------------------------------------*/
	pDlg->FldTxtGetValue(IDC_SOME_EDIT_PATH, &Str, PATH_LEN);
	pRAD_Init->Set_Import_Select_Path((const char *)Str);

	pDlg->FldTxtGetValue(IDC_SOME_EDIT_WILDCARD, &Str, WILDCARDS);
	pRAD_Init->Set_Import_Select_Wildcard((const char *)Str);

	pDlg->CheckBoxGetValue(IDC_SOME_OVERWRITE, &bBool); 
	pRAD_Init->Set_Import_Select_Overwrite(bBool);
	
	pDlg->CheckBoxGetValue(IDC_SOME_INITDB, &bBool);
	pRAD_Init->Set_Import_Select_Init(bBool);

	pDlg->CheckBoxGetValue(IDC_SOME_SUBFOLDER_BOX, &bBool);
	pRAD_Init->Set_Import_Select_Subfolders(bBool);

		
   /* ------------------------------------------------------------------
    *	Write the new values to the .ini file
    * ----------------------------------------------------------------*/      
   	pRAD_Init->Save_Parameters();

}        

////////////////////////////////////////////////////////////////////////
// Mark all files as selected
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable;
    int i;
    
	pDlg->GetTable(IDC_IMPORT_PICKTABLE, &pTable);
	if (pTable != NULL) {
	    for (i=0; i<glliNumRows; i++) pgllSelect[i] = TRUE;
        pTable->UpdateCol(glluiSelectBoxCol);
    }
}        

//////////////////////////////////////////////////////////////////////////
// Mark all files as not selected
static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable;
	int i;
		
	pDlg->GetTable(IDC_IMPORT_PICKTABLE, &pTable);
	if (pTable != NULL) {
	    for (i=0; i<glliNumRows; i++) pgllSelect[i] = FALSE;
        pTable->UpdateCol(glluiSelectBoxCol);
    }
}        
     
//////////////////////////////////////////////////////////////////////////
// 	If Cancel hit, ask if really want to cancel
static void CancelImportBtnServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	GUI_ACTION response;
	
	response = GUI_MsgBox("Are you sure you want to cancel the import process after import of this file completes?", GUI_ICON_QUESTION, GUI_YESNO);
	if (response == GUI_YES) {
		gllCancelBtnHit = IDCANCEL;
	}		
}

//////////////////////////////////////////////////////////////////////////
// Allow the user to close the text window
static BOOL CloseImportLogWindow(CGUI_TextWin *pWin)
{

	BOOL bCanClose = TRUE;
	glImportWinPtr = NULL;
	return(bCanClose);
}                                                  


//////////////////////////////////////////////////////////////////////////
// Delete all lists allocated for the table and set their pointers to NULL
static void DeleteAllAllocatedLists()
{         
	if (pgllStations) free(pgllStations);
	pgllStations = NULL;
	if (gllStations) free(gllStations);
	gllStations = NULL;
	if (pgllDates)free(pgllDates);
	pgllDates = NULL;
	if (gllDates)free(gllDates);
	gllDates = NULL;
	if (gllPaths)free(gllPaths);
	gllPaths = NULL;
	if (pgllFileNames)free(pgllFileNames);
	pgllFileNames = NULL;
	if (gllFileNames)free(gllFileNames);
	gllFileNames = NULL;
	if (pgllSelect)free(pgllSelect);
	pgllSelect = NULL;
}


/* <f> */
/*===========================================================================
 *
 *  Name	 :  	CreateTableLists
 *
 *  Purpose	 :		Creates the lists of files matching the path and name specified.
 *					The files are opened and headers read to get the date and station
 *					the data is associated with.
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
 *	18-Nov-2003	SFK		Fixed to work correctly with corrupt files
===========================================================================*/
static int CreateTableLists(CGUI_Dlg *pDlg, char *szOrigPaths, char *szOrigWildcards, int *piNumEntries)
{
    
    int i, m;
    char szNameToMatch[PATH_LEN+DOSNAME_LEN+1];
    char szCurrentPathToMatch[PATH_LEN+1];
	char szWildcardToMatch[DOSNAME_LEN+1];
    char szPaths[PATHS+1];
	char szDataRoot[PATHS+1];
	char szWildcards[WILDCARDS+1];
    int iYr, iMon, iDay;
    unsigned int uiDum;
	int iLen;
	double dDum;
    int status;
	struct IDStruct StaID;
    int iNumFiles, iWriteLine; 
    int iNumPaths, iNumFilesThisPath, iNumTotalFiles, iNumWildcards;
    CGFmtStr msg;
    FILE *pFile = NULL;
	int iWildcardIndex, iPathIndex;
                    


   /* ------------------------------------------------------------------
	*   Check the length of the arguments, blank strings not allowed
	* ----------------------------------------------------------------*/
	if (strlen(szOrigWildcards) == 0) {
		RadReviewMsg(uiBLANK_STRING_ERR, "File Type Specifier"); 
		return (FALSE);
	}
	if (strlen(szOrigPaths) == 0) {
		RadReviewMsg(uiBLANK_STRING_ERR, "Path/Folder Specifier"); 
		return (FALSE);
	}
	status = WildcardsCopy(szWildcards, szOrigWildcards);
	if (status != TRUE) return(status);
	status = PathsCopy(szPaths, szOrigPaths);
	if (status != TRUE) return(status);

   /* ------------------------------------------------------------------
	*   If there is a wildcard of *.id specified, then add the
	*	common shared file directory to the list for the search
	* ----------------------------------------------------------------*/
	if (strstr(_strlwr(szWildcards), "*.id") != 0) { // id files are a special case
		if (!GetDirectory(glsFacNum, DATA_DIR, szDataRoot, PATHS+1)) return(FALSE);
		status = PathCat(szPaths, szDataRoot, TRUE);
		//status = PathCat(szPaths, (char *)pRAD_Init->Get_Common_Database_Path(), TRUE);
		if (status != TRUE) return(status);
	}

   /* ------------------------------------------------------------------
	*   Find out how many paths and files given in the paths field.
	*	If the Path or Drive does not exist, return Fail.
	*	If the Path exists but no files are found return Success.
	* ----------------------------------------------------------------*/
	HCURSOR OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));
	status = CountPathsAndFiles(szPaths, szWildcards, &iNumPaths, &iNumTotalFiles, &iNumWildcards);
	if ((status == uiPATH_DOES_NOT_EXIST_ERR) || (status == uiDRIVE_DOES_NOT_EXIST_ERR) ||
		(status == uiBLANK_STRING_ERR) || (iNumTotalFiles == 0)) {
		DeleteAllAllocatedLists();
		*piNumEntries = 0;
		if ((status == uiPATH_DOES_NOT_EXIST_ERR) || (status == uiDRIVE_DOES_NOT_EXIST_ERR)) {
		    pDlg->SetFocus(IDC_SOME_EDIT_PATH);		// no valid path found
			::SetCursor(OldCur);
			return(FALSE);
		}	
		if (iNumTotalFiles == 0) { ::SetCursor(OldCur); return(TRUE);}	// no files is okay
	}	

   /* ------------------------------------------------------------------
    *	Based on number of matching files, allocate storage for the 
    *	lists to be generated and for the lists of pointers to the strings.
    * ----------------------------------------------------------------*/            
    if (pgllSelect)free(pgllSelect); 
    pgllSelect = (BOOL *)malloc(iNumTotalFiles*sizeof(BOOL));
    if (pgllSelect == NULL) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR, "Select Column");
		DeleteAllAllocatedLists();
		::SetCursor(OldCur);
		return(FALSE);
	}
		                                              
	if (gllFileNames) free(gllFileNames);
    gllFileNames = (char (*)[DOSNAME_LEN+1])malloc(iNumTotalFiles*(DOSNAME_LEN+1));
    if (pgllSelect == NULL) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR, "File Names");
		DeleteAllAllocatedLists();
		::SetCursor(OldCur);
		return(FALSE);
	}
		                                         
	if (pgllFileNames) free(pgllFileNames);                                         
    pgllFileNames =(char **)malloc(iNumTotalFiles*sizeof(char *));
    if (pgllSelect == NULL) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR, "File Name Pointers");
		DeleteAllAllocatedLists();
		return(FALSE);
	}
		                                         
	if (gllPaths) free(gllPaths);
    gllPaths = (char (*)[PATH_LEN+1])malloc(iNumTotalFiles*(PATH_LEN+1));
    if (pgllSelect == NULL) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR, "Paths");
		DeleteAllAllocatedLists();
		::SetCursor(OldCur);
		return(FALSE);
	}

	if (gllDates) free(gllDates);                                         
    gllDates = (char (*)[DT_LEN+1])malloc(iNumTotalFiles*(DT_LEN+1));
    if (pgllSelect == NULL) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR, "Dates");
		DeleteAllAllocatedLists();
		return(FALSE);
	}
		                                         
	if (pgllDates) free(pgllDates);                                         
    pgllDates = (char **)malloc(iNumTotalFiles*sizeof(char *));
    if (pgllSelect == NULL) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR, "Date Pointers");
		DeleteAllAllocatedLists();
		::SetCursor(OldCur);
		return(FALSE);
	}

	if (gllStations) free(gllStations);                                         
    gllStations = (char (*)[DB_NAME_LEN+1])malloc(iNumTotalFiles*(DB_NAME_LEN+1));
    if (pgllSelect == NULL) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR, "Stations");
		DeleteAllAllocatedLists();
		return(FALSE);
	}
		                                         
	if (pgllStations) free(pgllStations);                                         
    pgllStations = (char **)malloc(iNumTotalFiles*sizeof(char *));
    if (pgllSelect == NULL) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR, "Station Pointers");
		DeleteAllAllocatedLists();
		::SetCursor(OldCur);
		return(FALSE);
	}
	::SetCursor(OldCur);

   	//  Create the progress bar
	TwoLineProgressBar *pTwoLineProgressBar;
	CGFmtStr Msg, Msg1, Msg2;
	BOOL bContinue = true;
	Msg1.Printf("Generating file lists, please wait.");
	Msg2.Printf("");
	pTwoLineProgressBar = new TwoLineProgressBar(iNumTotalFiles, 1, Msg1, Msg2, "Radiation Review: Progress");
	if (pTwoLineProgressBar) {
		iNumFiles = 0;
		iWriteLine = 0;
		iWildcardIndex = 0;
		int iTempNumWildcards = 0;	// 16-Jun-2005 Fixed problem where had legal file (type ok) but station not known to Facility manager
		while ((status == TRUE) && (iNumFiles != iNumTotalFiles) && (iTempNumWildcards < iNumWildcards)) {  // 16-Jun-2005 SFK Add condition of whether we are at end of wildcards
			//for (j=0; j<iNumWildcards; j++) {
			while (iTempNumWildcards < iNumWildcards) {	// 16-Jun-2005  Change way check at end of wildcards
				iTempNumWildcards++;
				status = StripWildcard(szWildcards, szWildcardToMatch, &iLen, &iWildcardIndex, DOSNAME_LEN);
				if (status != TRUE) continue;
				if (iLen == 0) continue;	// handle case of two terminators in a row

				iPathIndex = 0;

				for (m=0; m<iNumPaths; m++) {
					/* copy a single pathname by copying up to the next ; */
				
					status = StripPath(szPaths, szCurrentPathToMatch, &iLen, &iPathIndex, PATH_LEN);
					if (iLen == 0) continue;	// handle case of two terminators in a row

				   /* ------------------------------------------------------------------
					*	Form a name based on the path and wildcard and find out how
					*   many files match these conditions.
					* ----------------------------------------------------------------*/
					MyStrnCpy(szNameToMatch, szCurrentPathToMatch, PATH_LEN);
					MyStrnCat(szNameToMatch, szWildcardToMatch, PATH_LEN+DOSNAME_LEN);
					gen_count_filenames(szNameToMatch, &iNumFilesThisPath);
					
				   /* ------------------------------------------------------------------
					*	Determine how many files match the name conditions.
					* ----------------------------------------------------------------*/
					gen_get_filenames(szNameToMatch, iNumFilesThisPath, 0, &(gllFileNames[iNumFiles]), &uiDum);

				   /* ------------------------------------------------------------------
					*	Open up each one and verify it is a good file
					*   Count how many good files you have
					* ----------------------------------------------------------------*/
					int iTemp = 0;	
					int iTempNumFiles = iNumFiles;		
					for (i=0; i<iNumFilesThisPath; i++) {
						MyStrnCpy(szNameToMatch, szCurrentPathToMatch, PATH_LEN);
						MyStrnCat(szNameToMatch, gllFileNames[iTempNumFiles], PATH_LEN+DOSNAME_LEN);
						status = ReadHeader(szNameToMatch, &StaID, &iYr, &iMon, &iDay, &pFile, &dDum, &dDum);
						if (pFile != NULL) fclose(pFile);
						if (status != TRUE) {
							RadReviewMsg(uiFILE_READ_ERR, (const char *)szNameToMatch);
							iTempNumFiles++;
							continue;
						}
						if (StaID.sStaType != OP_TYPE) {
							if (pglFacCfg->IsStationEnabled(glsFacNum, &StaID)){
								iTemp++; //KM
							}
							else {
								RadReviewMsg(uiFILE_STA_NOT_IN_DB_WARN, szNameToMatch, StaID.sStaID);
							}
						}
						else {
							iTemp++;
						}
						//if (pFile != NULL) fclose(pFile);
						iTempNumFiles++;
					}
					iNumFilesThisPath = iTemp;
					
				   /* ------------------------------------------------------------------
					*	For each list entry:
					*	1. 	Look in the file headers to get the information for the station
					*		and date lists.
					*	2.	Fill in the string address in the pointer list
					* ----------------------------------------------------------------*/
					char szName[DB_NAME_LEN+1];
					for (i=0; i<iNumFilesThisPath; i++) {
						do {				
							/* read the file headers to get all info. needed */
							MyStrnCpy(szNameToMatch, szCurrentPathToMatch, PATH_LEN);
							MyStrnCat(szNameToMatch, gllFileNames[iNumFiles], PATH_LEN+DOSNAME_LEN);
							status = ReadHeader(szNameToMatch, &StaID, &iYr, &iMon, &iDay, &pFile, &dDum, &dDum);
							if (pFile != NULL) fclose(pFile);
							if (status != TRUE) { // skip past those with bad headers since they weren't counted above
				    			//RadReviewMsg(uiFILE_READ_ERR, (const char *)szNameToMatch);
				    			iNumFiles++;
							}	
						} while (status != TRUE);    
						
						if (StaID.sStaType != OP_TYPE) {
							/* get name associated with station designated in the header */
							if (pglFacCfg->IsStationEnabled(glsFacNum, &StaID)) {
								pglFacCfg->GetStationLongName(glsFacNum, &StaID, szName, DB_NAME_LEN);
							}
							else {
								MyStrnCpy(gllPaths[iNumFiles], szCurrentPathToMatch, PATH_LEN);
								iNumFiles++;
								RadReviewMsg(uiFILE_STA_NOT_IN_DB_WARN, (const char *)szNameToMatch, StaID.sStaID);
								continue;    // skip those that don't have a good station
							}
							
							/* generate strings for date and station list */
							sprintf(gllDates[iNumFiles], "%02d.%02d.%02d", iYr, iMon, iDay);
						//if (StaID.sStaType != OP_TYPE) {
							sprintf(gllStations[iNumFiles],"%s", szName);
						}
						else {
							sprintf(gllDates[iNumFiles], "%02d.%02d.%02d", iYr, iMon, iDay);
							sprintf(gllStations[iNumFiles],"ID List");
						}

						
						/* remember path associated with this file */
						MyStrnCpy(gllPaths[iNumFiles], szCurrentPathToMatch, PATH_LEN);
						
						/* fill in the string pointer array for name, date and station lists */
						pgllFileNames[iWriteLine] = gllFileNames[iNumFiles];
						pgllStations[iWriteLine] = gllStations[iNumFiles];
						pgllDates[iWriteLine] = gllDates[iNumFiles];  
						pgllSelect[iWriteLine] = TRUE;
						iNumFiles++;
						iWriteLine++;
						Msg2.Printf("Processed %d files.\n",iNumFiles);
						bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);

					}   
				}
			}
		}		
	}
	pTwoLineProgressBar->Close();
	delete pTwoLineProgressBar;

	*piNumEntries = iWriteLine;
	
	// account for some bad files.
	if (iWriteLine < iNumTotalFiles) {
		for (i=iWriteLine; i<iNumTotalFiles; i++) {
		    pgllFileNames[i] = "";
		    pgllStations[i] = "";
		    pgllDates[i] = "";  
		    pgllSelect[i] = FALSE;
			MyStrnCpy(gllPaths[i], "", PATH_LEN+1);
		}
	}	    
	
	if (*piNumEntries == 0) return (FALSE);
	return(TRUE);
}		 		
                                                                        

/* <f> */
/*===========================================================================
 *
 *  Name	 :  	UpdateTable
 *
 *  Purpose	 :		Called from the dialog box when the user hits the Update
 *					Table button or the Okay button.
 *					It creates the lists of files matching the path and name
 *					specified.  The files are opened and headers read to get
 *					the date and station the data is associated with.
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
 *  16-Mar-1996 SFK		Created
 *
===========================================================================*/
static BOOL UpdateTable(CGUI_Dlg *pDlg)
{
	CGUI_Table *pTable;
    char szNewPaths[PATH_LEN+1], szTempPaths[PATHS+1];
    char szNewWildcard[WILDCARDS+1];
    BOOL bOkay = FALSE;
    int status;   
	CGFmtStr szAllPaths;
	BOOL bBool;
	unsigned int uiNumPaths = -1;
    
   /* ------------------------------------------------------------------
    *	Get the current values for the path and extension fields from
	*	the dialog box.
    *	Update the table only if something has changed or if it looks like
    *	there are no matching files.
    * ----------------------------------------------------------------*/
	pDlg->FldTxtGetValue(IDC_SOME_EDIT_PATH, szNewPaths, PATH_LEN);
	pDlg->FldTxtGetValue(IDC_SOME_EDIT_WILDCARD, szNewWildcard, WILDCARDS);
	pDlg->CheckBoxGetValue(IDC_SOME_SUBFOLDER_BOX, &bBool);
	
	if ((strcmp(szNewPaths, gllszPaths) != 0) || // new path
		(strcmp(szNewWildcard, gllszWildcard) != 0) || // new wildcard
		(bBool != gllbIncludeSubfolders) ||	//new subfolder choice
		(glliNumRows == 0)) { // last time had no rows
	  
		// remember these names for next time 
	  	MyStrnCpy(gllszPaths, szNewPaths, PATH_LEN);
	  	MyStrnCpy(gllszWildcard, szNewWildcard, WILDCARDS);

	   /* ------------------------------------------------------------------
		*   Check to see whether they want to browse subdirectories.
		*	If yes, then figure out all the subdirectory paths underneath
		*	the root specified.  See if there are any files at the specified
		*	path.  If yes, then can exit the option.  If not, don't allow
		*	them to exit the dialog box.
		* ----------------------------------------------------------------*/
		pDlg->CheckBoxGetValue(IDC_SOME_SUBFOLDER_BOX, &bBool);
		if (bBool == TRUE) 
		{
			HCURSOR OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));
			status = DetermineAllDirectories(szNewPaths, &szAllPaths, &uiNumPaths);
			if (status == TRUE) {
				PathsCopy(szTempPaths, (char *)szAllPaths.Get_sz());
			}
			else {
				if (uiNumPaths > 1)	{
					RadReviewMsg(uiBROWSE_INFO_WARN, szNewPaths);	// warning about more than one path on browse
				}
				else {
					PathCopy(szTempPaths, szNewPaths);
				}
			}
			::SetCursor(OldCur);
		}
		else {
			MyStrnCpy(szTempPaths, szNewPaths, PATH_LEN);

		}

		
		// table lists created okay, now set up the new table
		pDlg->GetTable(IDC_IMPORT_PICKTABLE, &pTable);
		if (pTable != NULL) {
			pTable->SetColWidths(CGUI_TABLE_COL_WIDTH_SCALED);
			pTable->Clear();

			// make up the new lists for the table
			// if already did the check during the browse directories and found no paths, don't try to create lists.
			if ((uiNumPaths == 0) || (CreateTableLists(pDlg, szTempPaths, szNewWildcard, &glliNumRows) != TRUE)) {
				glliNumRows = 0;
				return(FALSE);
			}	
	    	
			glluiSelectBoxCol = pTable->AddColCheckBox("Select", glliNumRows, pgllSelect);		
			glluiDateCol = pTable->AddColTxt("Date", glliNumRows, pgllDates, DT_LEN);
			gllusStationCol = pTable->AddColTxt("Station", glliNumRows, pgllStations, DB_NAME_LEN);
			glluiFileNamesCol = pTable->AddColTxt("File Name", glliNumRows, pgllFileNames, DOSNAME_LEN);
			
			pTable->SetColWriteEnable(glluiDateCol, FALSE);
			pTable->SetColWriteEnable(gllusStationCol, FALSE);
			pTable->SetColWriteEnable(glluiFileNamesCol, FALSE);
			
			pTable->DisplayNew();	
		}	
	}
	if (glliNumRows == 0) return (FALSE);
	return(TRUE);
}        


// Update all the table lists if Okay is selected
static BOOL ValidatePathAndUpdateTable(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction)
{
	if (ClosingAction == GUI_OK) {    	
		return(UpdateTable(pDlg));
	}
	else return(TRUE);
}

// Update all the table lists if Update is selected
static void ButtonUpdateTable(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{	
	UpdateTable(pDlg);
}
	
    	

