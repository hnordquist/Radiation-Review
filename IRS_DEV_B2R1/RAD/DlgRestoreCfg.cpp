///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgRestoreCfg.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		UpdateINI_Parms - update parameters in RAD.INI
//		RestoreCfgMenuOption - menu dialog and action
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"    
#include "CopyDir.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DlgRestoreCfg.h"
#include "Questions.h"
#include "RadInit.H"
#include "RadReview.H"
#include "Utilities.h"

extern RADInit *pRAD_Init; 
extern RADReview *pRAD_Review; 
extern short glsFacNum;

static void UpdateINI_Parms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);


 
/*===========================================================================
 *
 *  Name	 :  	UpdateINI_Parms
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
 *  27-Aug-2004 SFK		Created
 *
===========================================================================*/
static void UpdateINI_Parms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    CGStr Str;
    int status;  
	             
	status = AskUpdateINI_Question();
	if (status != TRUE) return;
	
   /* ------------------------------------------------------------------
    *	Get the current values for the path from which to restore to
    * ----------------------------------------------------------------*/
	pDlg->FldTxtGetValue(IDC_RCFG_EDIT_BACKPATH, &Str, PATH_LEN);
	pRAD_Init->Set_CfgRestore_From_Path((const char *)Str);

//	pDlg->FldTxtGetValue(IDC_RDB_EDIT_COPYPATH, &Str, PATH_LEN);
//	pRAD_Init->Set_dbRestore_To_Path((const char *)Str);	
	
   /* ------------------------------------------------------------------
    *	Write the changes to the .ini file
    * ----------------------------------------------------------------*/      
   	pRAD_Init->Save_Parameters();
	
}        
         

//  27-Aug-2004 SFK		Created
void RestoreCfgMenuOption()
{                                 
    
    static char szDfltRootPath[PATH_LEN+1] = "";
    static char szDfltCurrentDbPath[PATH_LEN+1] = "";                   
    static BOOL bFirst = TRUE;
    
	char szCurrentDbPath[PATH_LEN+1] = "";
	char szMainCurrentDbPath[PATH_LEN+1] = "";    
    char szRootBackupPath[PATH_LEN+1] = "";
    char szMainBackupPath[PATH_LEN+1] = "";
	char szNewOpenINI_File[PATH_LEN+1] = "";
    BOOL bRoom;                                          
    unsigned long ulDirSize, ulDiskSize, ulDiskAvailable;
    int status;
    char *pStr;
        
    GUI_ACTION DlgReturn;         
    CGFmtStr msg;
    
	SetCurrentDrive();
    GetDbName(szCurrentDbPath);
    
   /* ------------------------------------------------------------------
    *	First time or when change databases, get the default values
    *	from the .ini file else use the last successful values.
    * ------------------------------------------------------------------*/
    if ((bFirst == TRUE) || (!SameDb(szDfltCurrentDbPath))) { 
    	pStr = pRAD_Init->Get_CfgRestore_From_Path();     
		MyStrnCpy(szRootBackupPath, pStr, PATH_LEN); 
		bFirst = FALSE;
		strcpy(szDfltCurrentDbPath, szCurrentDbPath);
		strcpy(szDfltRootPath, szRootBackupPath);
	}
	
	strcpy(szRootBackupPath, szDfltRootPath);
  
   /* ------------------------------------------------------------------
    *	Define the buttons and actions in the dialog box
    * ----------------------------------------------------------------*/  
    CGUI_Dlg RestoreDbDlg(IDD_FILE_RESTORE_CFG, NULL, DLG_POSN_NO_SAVE);
    if (RestoreDbDlg.IsValid()) {	
		RestoreDbDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		RestoreDbDlg.DefinePushBtn(IDOK, GUI_OK);
		RestoreDbDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateINI_Parms);            
		
		RestoreDbDlg.DefineFldTxt(IDC_RCFG_EDIT_BACKPATH, szRootBackupPath, PATH_LEN);  
		RestoreDbDlg.DefineFldStaticTxt(IDC_RCFG_CURR_DB);
		RestoreDbDlg.FldStaticTxtSetValue(IDC_RCFG_CURR_DB, szCurrentDbPath);
			
		RestoreDbDlg.SetFocus(IDOK);
		
	    DlgReturn = RestoreDbDlg.Go();
		    
	    if (DlgReturn == GUI_OK) {  
		    	
	    	RestoreDbDlg.RetrieveAllControlValues(); 
	    	
		   /* ------------------------------------------------------------------
		    *	Strip spaces and check length of restore from paths.
		    * ----------------------------------------------------------------*/
	    	if (PathCopy(szMainBackupPath, szRootBackupPath) != TRUE) return;
	    	strcat(szMainBackupPath, "config\\rad\\");

	    	if (PathCopy(szMainCurrentDbPath, szCurrentDbPath) != TRUE) return;
	    	strcat(szMainCurrentDbPath, "config\\rad\\");
	    	
		   /* ------------------------------------------------------------------
		    *	See if the old path exists, if not quit.
		    * ----------------------------------------------------------------*/
		    status = PathExists(szMainBackupPath);
		    if (status != TRUE) {
		    	RadReviewMsg(status, (const char *)szMainBackupPath); //uiPATH_TOO_LONG_ERR or uiDRIVE_DOES_NOT_EXIST_ERR or uiPATH_DOES_NOT_EXIST_ERR
		    	return;
		    }	
	    	
		   /* ------------------------------------------------------------------
		    *	See if a database exists at the location to be restored from
		    * ----------------------------------------------------------------*/
		    if (FileExists(szMainBackupPath, "db_dflt.dbd") != TRUE) {
		    	RadReviewMsg(uiDB_DOES_NOT_EXIST_ERR, (const char *)szMainBackupPath);
				return; 
		    }
		    
		   /* ------------------------------------------------------------------
		    *	See if the path exists where want to restore to, if not make it.
		    * ----------------------------------------------------------------*/
		    status = PathExists(szMainCurrentDbPath);
		    if (status == uiPATH_DOES_NOT_EXIST_ERR) {
		    	status = AskAndMakePath(szMainCurrentDbPath);
		    	if (status != TRUE) return;
		    }	
		    if (status != TRUE) {
		    	RadReviewMsg(status, (const char *)szMainCurrentDbPath);
				return;
			}	
	        
		   /* ------------------------------------------------------------------
		    *	See if a data base already exists where want to restore to.
		    * ----------------------------------------------------------------*/
		    if (FileExists(szMainCurrentDbPath, "db_dflt.dbd") == TRUE) {
				status = AskOverwriteQuestion(szCurrentDbPath);
				if (status != TRUE) return; 
		    }	
		    
		   /* ------------------------------------------------------------------
		    *	Figure out how many bytes will need to be copied and if there
		    *	is room on the destination drive.  (Destination drive must have
		    *	directory size + 10000 to allow for directory entries, etc.)
		    * ----------------------------------------------------------------*/
		    status = GetDirSize(szMainBackupPath, "*.*", &ulDirSize);	/* check number of bytes filled in BackPath */
		    if (status != TRUE) return;

			// Address any COM specific parameters, if not in RAD directory
			
			bRoom = TRUE;
		    status = GetDriveSize(szCurrentDbPath, &ulDiskSize, &ulDiskAvailable);	/* check number of bytes available in New Path */
		    if (status != TRUE) return;
			if ((ulDirSize + 10000) > ulDiskAvailable) bRoom = FALSE;	    
		    	    
		    if (bRoom == FALSE) {
		    	if (AskNotEnoughRoomQuestion(ulDirSize, ulDiskAvailable, szCurrentDbPath) != TRUE) return;
		    }
		    
		   /* ------------------------------------------------------------------
		    *	Copy the data and show progress
		    * ----------------------------------------------------------------*/
		    CloseDatabase();	// close in case trying to do something with this one
			pRAD_Init->Initialization_File_Close();		// close the current RAD.INI

		    status = CopyDirectory(szMainBackupPath, szMainCurrentDbPath);

		   /* ------------------------------------------------------------------
		    *	Open the original database again.
		    * ----------------------------------------------------------------*/
			OpenDatabase(szCurrentDbPath, glsFacNum);

		   /* ------------------------------------------------------------------
			*	Open up the RAD.INI you just copied
			* ----------------------------------------------------------------*/
			strcpy(szNewOpenINI_File, szMainCurrentDbPath);
			if (szNewOpenINI_File[strlen(szNewOpenINI_File) -1]  != '\\') strcat(szNewOpenINI_File, "\\");
			strcat(szNewOpenINI_File, "RAD.INI");
			pRAD_Init->Set_New_Initialization_File(szNewOpenINI_File);
			pRAD_Review->RADReviewSetMainMenuOptions();

			//	Backup the database associated with the various COMs, if not in RAD directory

		    if (status == TRUE) {
		    	MyStrnCpy(szDfltRootPath,szRootBackupPath, PATH_LEN);	// remember the default for next time
				if (status) {
					msg.Printf("Configuration copied to %s \nsuccessfully.  Radiation Review restart required.", szCurrentDbPath);	
		    		GUI_MsgBox(msg, GUI_ICON_INFO);
				}
		    	return;
			}
			else {
				RadReviewMsg(uiCOPY_DB_ERR, (const char *)szRootBackupPath, (const char *)szCurrentDbPath);
		    }	
    
	    }	
	}    
}   		                                 
