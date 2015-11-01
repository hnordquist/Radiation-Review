///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgBackupDb.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		UpdateINI_Parms - write to RAD.INI
//		BackupDbMenuOption - menu dialog and actions
//		XcopyDirectory - not used for the time being
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"    
#include "BinaryImport.h"
#include "EOSSImport.h"
#include "CopyDir.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DlgBackupDb.h"
#include "GrandImport.h"
#include "Questions.h"
#include "RADInit.H"
#include "Utilities.h"
#include "VifmImport.h"

extern RADInit *pRAD_Init;  
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
 *  16-Mar-1996 SFK		Created
 *	28-Oct-2004	SFK		Added glsFacNum to GetDataStoreSize and BackupDatabase calls - interface spec compatability
===========================================================================*/
static void UpdateINI_Parms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    CGStr Str;
    int status;  
	             
	status = AskUpdateINI_Question();
	if (status != TRUE) return;
	
   /* ------------------------------------------------------------------
    *	Get the current values for the path to backup to
    * ----------------------------------------------------------------*/
	pDlg->FldTxtGetValue(IDC_BDB_EDIT_NEW_PATH, &Str, PATH_LEN);
	pRAD_Init->Set_dbBackup_Path((const char *)Str);
	
   /* ------------------------------------------------------------------
    *	Write the changes to the .ini file
    * ----------------------------------------------------------------*/      
   	pRAD_Init->Save_Parameters();
	
}        


// 03-11-2003	Added links to binary import Com
// 27-Aug-2004	Extensive mods to support fac\data directory structure
void BackupDbMenuOption()
{                                 

    static char szDfltRootPath[PATH_LEN+1] = "";
    static char szDfltCurrentDbPath[PATH_LEN+1] = "";                   
    static BOOL bFirst = TRUE;
    
	char szCurrentDbPath[PATH_LEN+1] = "";    
	char szMainCurrentDbPath[PATH_LEN+1] = "";    
    char szRootBackupPath[PATH_LEN+1] = "";
	char szMainBackupPath[PATH_LEN+1] = "";
    unsigned long ulDirSize, ulDiskSize, ulDiskAvailable;
    BOOL bRoom;     
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
    	pStr = pRAD_Init->Get_dbBackup_Path(); 
		MyStrnCpy(szDfltRootPath, pStr, PATH_LEN); 
   		bFirst = FALSE;
    	strcpy(szDfltCurrentDbPath, szCurrentDbPath);
	}
	
	strcpy(szRootBackupPath, szDfltRootPath);

   /* ------------------------------------------------------------------
    *	Define the buttons and actions in the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg BackupDbDlg(IDD_FILE_BACKUP_DB, NULL, DLG_POSN_NO_SAVE);
    if (BackupDbDlg.IsValid()) {
		BackupDbDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		BackupDbDlg.DefinePushBtn(IDOK, GUI_OK);
		BackupDbDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateINI_Parms);              
			                                               
		BackupDbDlg.DefineFldStaticTxt(IDC_BDB_CURR_DB);
		BackupDbDlg.FldStaticTxtSetValue(IDC_BDB_CURR_DB, szCurrentDbPath);
		BackupDbDlg.DefineFldTxt(IDC_BDB_EDIT_NEW_PATH, szRootBackupPath, PATH_LEN);
			
		BackupDbDlg.SetFocus(IDOK);
		
	    DlgReturn = BackupDbDlg.Go();
		    
	    if (DlgReturn == GUI_OK) {  
		    	
	    	BackupDbDlg.RetrieveAllControlValues();

		   /* ------------------------------------------------------------------
		    *	Remove spaces, check length
		    * ----------------------------------------------------------------*/
	    	if (PathCopy(szMainBackupPath, szRootBackupPath) != TRUE) return;
	    	strcat(szMainBackupPath, "data\\rad\\");

	    	if (PathCopy(szMainCurrentDbPath, szCurrentDbPath) != TRUE) return;
	    	strcat(szMainCurrentDbPath, "data\\rad\\");

		   /* ------------------------------------------------------------------
		    *	See if the new path exists, if not make it.  If can't make it give
		    *	reason why.
		    * ----------------------------------------------------------------*/
		    status = PathExists(szMainBackupPath); // returns TRUE, uiPATH_TOO_LONG_ERR, uiDRIVE_DOES_NOT_EXIST_ERR oruiPATH_DOES_NOT_EXIST_ERR
		    if ( status == uiPATH_DOES_NOT_EXIST_ERR) {
		    	status = AskAndMakePath(szMainBackupPath);
		    	if (status != TRUE) return;
		    }	
		    if ( status != TRUE) {
		    	RadReviewMsg(status, (const char *)szMainBackupPath);
				return;
			}	

		   /* ------------------------------------------------------------------
		    *	See if a database exists at the location specified.  If yes,
		    *	see if they want to overwrite it.
		    * ----------------------------------------------------------------*/
		    if (FileExists(szMainBackupPath, "db_main.dbd") == TRUE) {
		    	status = AskOverwriteQuestion(szMainBackupPath);
		    	if (status == FALSE) return;
		    }	
		    
		   /* ------------------------------------------------------------------
		    *	Figure out how many bytes will need to be copied and if there
		    *	is room on the destination drive.  (Destination drive must have
		    *	directory size + 10000 to allow for directory entries, etc.)
		    * ----------------------------------------------------------------*/
		    if (GetDirSize(szMainCurrentDbPath, "*.*", &ulDirSize) != TRUE) return;	/* check number of bytes filled in szCurrentDbPath */
			
			// Customize for new COMs

			// find the size of the individual data stores
			double dGrandSizeInBytes;
			pglGrandImport->GetDataStoreSize(glsFacNum, &dGrandSizeInBytes);
			double dVifmSizeInBytes;
			pglVifmImport->GetDataStoreSize(glsFacNum, &dVifmSizeInBytes);
			double dBinarySizeInBytes;
			pglBinaryImport->GetDataStoreSize(glsFacNum, &dBinarySizeInBytes);

			// add the individual data stores to the internal one.
			ulDirSize += (unsigned long)dGrandSizeInBytes + (unsigned long)dVifmSizeInBytes + (unsigned long)dBinarySizeInBytes;
		                                                                       
		    if (GetDriveSize(szMainBackupPath, &ulDiskSize, &ulDiskAvailable) != TRUE) return;	/* check number of bytes available on drive */
			
			bRoom = TRUE;
			if ((ulDirSize + 10000) > ulDiskAvailable) bRoom = FALSE;	    
		    	    
		    if (bRoom == FALSE) {
		    	if (AskNotEnoughRoomQuestion(ulDirSize, ulDiskAvailable, szMainBackupPath) == FALSE) return;
		    }
		    
		   /* ------------------------------------------------------------------
		    *	Close the current database in case are trying to copy it.
		    *	Copy the data and show progress. Inform of the results of the copy.	
		    * ----------------------------------------------------------------*/
		    GetDbName(szCurrentDbPath);
		    CloseDatabase();
			    
		    status = CopyDirectory(szMainCurrentDbPath, szMainBackupPath);

		   /* ------------------------------------------------------------------
		    *	Open the original database again.
		    * ----------------------------------------------------------------*/
			OpenDatabase(szCurrentDbPath, glsFacNum);
			
			
			// Customize for new COMs

			//	Backup the database associated with the various COMs
			if (status) status = pglGrandImport->BackupDatabase(glsFacNum, szRootBackupPath, false, true);
			if (status) status = pglVifmImport->BackupDatabase(glsFacNum, szRootBackupPath, false, true);
			if (status) status = pglBinaryImport->BackupDatabase(glsFacNum, szRootBackupPath, false, true);
			//HN 4-7-08 Add EOSS to list
			if (status) status = pglEOSSImport->BackupDatabase(glsFacNum, szRootBackupPath, false, true);

		    if (status != TRUE) { 
		    	RadReviewMsg(uiCOPY_DB_ERR, (const char *)szCurrentDbPath, (const char *)szRootBackupPath);
		    }
		    else {	
		    	MyStrnCpy(szDfltRootPath,szRootBackupPath, PATH_LEN);	// remember the default for next time
				msg.Printf("Database copied to %s.", szRootBackupPath);	
			    GUI_MsgBox(msg, GUI_ICON_INFO);
		    }	

	    }	
	}    
}   		                                 


int XcopyDirectory(char *szToDir, char *szFromDir)
{
	char szCmd[MAX_LINE_LEN+1];
	int status;
	int iTemp;
	int bSuccess;
	
	_flushall();
	strcpy(szCmd,"xcopy ");
	strcat(szCmd, "\"");
	strcat(szCmd, szFromDir);
	int len = strlen(szCmd);
	if (szCmd[strlen(szCmd)-1] == '\\') szCmd[strlen(szCmd)-1] = '\0';
	strcat(szCmd, "\" \"");
	strcat(szCmd, szToDir);
	strcat(szCmd, "\"");
	strcat(szCmd, " /s /y /v");
	status = system(szCmd);
	_flushall();
	if (status == 0) 
		bSuccess = TRUE;
	else {
		bSuccess = FALSE;
		iTemp = errno;
	}

	if (status == 4) {	// sometimes get this interrupted system call but works second time
		_flushall();
		strcpy(szCmd,"xcopy ");
		strcat(szCmd, "\"");
		strcat(szCmd, szFromDir);
		int len = strlen(szCmd);
		if (szCmd[strlen(szCmd)-1] == '\\') szCmd[strlen(szCmd)-1] = '\0';
		strcat(szCmd, "\" \"");
		strcat(szCmd, szToDir);
		strcat(szCmd, "\"");
		strcat(szCmd, " /s /y /v");
		status = system(szCmd);
		_flushall();
		if (status == 0) 
			bSuccess = TRUE;
		else {
			bSuccess = FALSE;
			iTemp = errno;
		}
	}

	return(bSuccess);
}
