///////////////////////////////////////////////////////////////////////////
//	Copyright, 2004, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (N-1)
//
//    File Name : DlgBackupCfg.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		UpdateINI_Parms - parameters to RAD.INI
//		BackupCfgMenuOption - menu dialog and action
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"    
#include "CopyDir.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DlgBackupCfg.h"
#include "Questions.h"
#include "RADInit.H"
#include "Utilities.h"

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
    *	Get the current values for the path to backup to
    * ----------------------------------------------------------------*/
	pDlg->FldTxtGetValue(IDC_BCFG_EDIT_NEW_PATH, &Str, PATH_LEN);
	pRAD_Init->Set_CfgBackup_Path((const char *)Str);
	
   /* ------------------------------------------------------------------
    *	Write the changes to the .ini file
    * ----------------------------------------------------------------*/      
   	pRAD_Init->Save_Parameters();
	
}        


// 27-Aug-2004	SFK Created
void BackupCfgMenuOption()
{                                 

    static char szDfltRootPath[PATH_LEN+1] = "";
    static char szDfltCurrentDbPath[PATH_LEN+1] = "";                   
    static bool bFirst = true;
    
	char szCurrentDbPath[PATH_LEN+1] = "";    
	char szMainCurrentDbPath[PATH_LEN+1] = "";    
    char szRootBackupPath[PATH_LEN+1] = "";
	char szMainBackupPath[PATH_LEN+1] = "";
    unsigned long ulDirSize, ulDiskSize, ulDiskAvailable;
    //bool bRoom;     
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
    if (bFirst || (!SameDb(szDfltCurrentDbPath))) {
    	pStr = pRAD_Init->Get_CfgBackup_Path(); 
		MyStrnCpy(szDfltRootPath, pStr, PATH_LEN); 
   		bFirst = false;
    	strcpy(szDfltCurrentDbPath, szCurrentDbPath);
	}
	
	strcpy(szRootBackupPath, szDfltRootPath);

   /* ------------------------------------------------------------------
    *	Define the buttons and actions in the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg BackupDbDlg(IDD_FILE_BACKUP_CFG, NULL, DLG_POSN_NO_SAVE);
    if (BackupDbDlg.IsValid()) {
		BackupDbDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		BackupDbDlg.DefinePushBtn(IDOK, GUI_OK);
		BackupDbDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateINI_Parms);              
			                                               
		BackupDbDlg.DefineFldStaticTxt(IDC_BCFG_CURR_DB);
		BackupDbDlg.FldStaticTxtSetValue(IDC_BCFG_CURR_DB, szCurrentDbPath);
		BackupDbDlg.DefineFldTxt(IDC_BCFG_EDIT_NEW_PATH, szRootBackupPath, PATH_LEN);
			
		BackupDbDlg.SetFocus(IDOK);
		
	    DlgReturn = BackupDbDlg.Go();
		    
	    if (DlgReturn == GUI_OK) {  
		    	
	    	BackupDbDlg.RetrieveAllControlValues();

		   /* ------------------------------------------------------------------
		    *	Remove spaces, check length
		    * ----------------------------------------------------------------*/
	    	if (PathCopy(szMainBackupPath, szRootBackupPath) != TRUE) return;
	    	strcat(szMainBackupPath, "config\\rad\\");

	    	if (PathCopy(szMainCurrentDbPath, szCurrentDbPath) != TRUE) return;
	    	strcat(szMainCurrentDbPath, "config\\rad\\");

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
		    if (FileExists(szMainBackupPath, "db_dflt.dbd") == TRUE) {
		    	status = AskOverwriteQuestion(szMainBackupPath);
		    	if (status == FALSE) return;
		    }	
		    
		   /* ------------------------------------------------------------------
		    *	Figure out how many bytes will need to be copied and if there
		    *	is room on the destination drive.  (Destination drive must have
		    *	directory size + 10000 to allow for directory entries, etc.)
		    * ----------------------------------------------------------------*/
		    if (GetDirSize(szMainCurrentDbPath, "*.*", &ulDirSize) != TRUE) return;	/* check number of bytes filled in szCurrentDbPath */
			
			// Customize for new COMs,if config not with RAD config
	                                                                       
		    if (GetDriveSize(szMainBackupPath, &ulDiskSize, &ulDiskAvailable) != TRUE) return;	/* check number of bytes available on drive */
			
			bool bRoom = true;
			if ((ulDirSize + 10000) > ulDiskAvailable) 
				bRoom = false;	    
		    	    
		    if (!bRoom)
			{
		    	if (AskNotEnoughRoomQuestion(ulDirSize, ulDiskAvailable, szMainBackupPath) == FALSE) 
					return;
			}
		    
		   /* ------------------------------------------------------------------
		    *	Close the current database in case are trying to copy it.
		    *	Copy the data and show progress. Inform of the results of the copy.	
		    * ----------------------------------------------------------------*/
		    GetDbName(szCurrentDbPath);
		    CloseDatabase();
			_flushall();
			    
		    status = CopyDirectory(szMainCurrentDbPath, szMainBackupPath);

		   /* ------------------------------------------------------------------
		    *	Open the original database again.
		    * ----------------------------------------------------------------*/
			OpenDatabase(szCurrentDbPath, glsFacNum);
			
			// Customize for new COMs,if config not with RAD config			

		    if (status != TRUE) { 
		    	RadReviewMsg(uiCOPY_DB_ERR, (const char *)szCurrentDbPath, (const char *)szRootBackupPath);
		    }
		    else {	
		    	MyStrnCpy(szDfltRootPath,szRootBackupPath, PATH_LEN);	// remember the default for next time
				msg.Printf("Configuration copied to %s.", szRootBackupPath);	
			    GUI_MsgBox(msg, GUI_ICON_INFO);
		    }	

	    }	
	}    
}   		                                 

