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
//    File Name : DlgRestoreDb.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		UpdateINI_Parms - parameters to RAD.INI
//		RestoreDbMenuOption - menu dialog and action
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
#include "RadInit.H"
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
    *	Get the current values for the path from which to restore to
    * ----------------------------------------------------------------*/
	pDlg->FldTxtGetValue(IDC_RDB_EDIT_BACKPATH, &Str, PATH_LEN);
	pRAD_Init->Set_dbRestore_From_Path((const char *)Str);

//	pDlg->FldTxtGetValue(IDC_RDB_EDIT_COPYPATH, &Str, PATH_LEN);
//	pRAD_Init->Set_dbRestore_To_Path((const char *)Str);	
	
   /* ------------------------------------------------------------------
    *	Write the changes to the .ini file
    * ----------------------------------------------------------------*/      
   	pRAD_Init->Save_Parameters();
	
}        
         

// 03-11-2003	Added links to binary import Com
// 27-Aug-2004	SFK	Extensive mods to support fac\data directory structure
void RestoreDbMenuOption()
{                                 
    
    static char szDfltRootPath[PATH_LEN+1] = "";
    static char szDfltCurrentDbPath[PATH_LEN+1] = "";                   
    static BOOL bFirst = TRUE;
    
	char szCurrentDbPath[PATH_LEN+1] = "";
	char szMainCurrentDbPath[PATH_LEN+1] = "";    
    char szRootBackupPath[PATH_LEN+1] = "";
    char szMainBackupPath[PATH_LEN+1] = "";
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
    	pStr = pRAD_Init->Get_dbRestore_From_Path();     
		MyStrnCpy(szRootBackupPath, pStr, PATH_LEN); 
		bFirst = FALSE;
		strcpy(szDfltCurrentDbPath, szCurrentDbPath);
		strcpy(szDfltRootPath, szRootBackupPath);
	}
	
	strcpy(szRootBackupPath, szDfltRootPath);
  
   /* ------------------------------------------------------------------
    *	Define the buttons and actions in the dialog box
    * ----------------------------------------------------------------*/  
    CGUI_Dlg RestoreDbDlg(IDD_FILE_RESTORE_DB, NULL, DLG_POSN_NO_SAVE);
    if (RestoreDbDlg.IsValid()) {	
		RestoreDbDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		RestoreDbDlg.DefinePushBtn(IDOK, GUI_OK);
		RestoreDbDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateINI_Parms);            
		
		RestoreDbDlg.DefineFldTxt(IDC_RDB_EDIT_BACKPATH, szRootBackupPath, PATH_LEN);  
		RestoreDbDlg.DefineFldStaticTxt(IDC_RDB_CURR_DB);
		RestoreDbDlg.FldStaticTxtSetValue(IDC_RDB_CURR_DB, szCurrentDbPath);
			
		RestoreDbDlg.SetFocus(IDOK);
		
	    DlgReturn = RestoreDbDlg.Go();
		    
	    if (DlgReturn == GUI_OK) {  
		    	
	    	RestoreDbDlg.RetrieveAllControlValues(); 
	    	
		   /* ------------------------------------------------------------------
		    *	Strip spaces and check length of restore from paths.
		    * ----------------------------------------------------------------*/
	    	if (PathCopy(szMainBackupPath, szRootBackupPath) != TRUE) return;
	    	strcat(szMainBackupPath, "data\\rad\\");

	    	if (PathCopy(szMainCurrentDbPath, szCurrentDbPath) != TRUE) return;
	    	strcat(szMainCurrentDbPath, "data\\rad\\");

	    	
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
		    if (FileExists(szMainBackupPath, "db_main.dbd") != TRUE) {
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
		    if (FileExists(szMainCurrentDbPath, "db_main.dbd") == TRUE) {
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
		    status = CopyDirectory(szMainBackupPath, szMainCurrentDbPath);

		   /* ------------------------------------------------------------------
		    *	Open the original database again.
		    * ----------------------------------------------------------------*/
			OpenDatabase(szCurrentDbPath, glsFacNum);

			// Customize for new COMs
			//	Backup the database associated with the various COMs
			if (status) status = pglGrandImport->RestoreDatabase(glsFacNum, szRootBackupPath, szCurrentDbPath, false, true);
			if (status) status = pglVifmImport->RestoreDatabase(glsFacNum, szRootBackupPath, szCurrentDbPath, false, true);
			if (status) status = pglBinaryImport->RestoreDatabase(glsFacNum, szRootBackupPath, szCurrentDbPath, false, true);
			//HN 4-78-08 Add EOSS to list
			if (status) status = pglEOSSImport->RestoreDatabase(glsFacNum, szRootBackupPath, szCurrentDbPath, false, true);
		    if (status == TRUE) {
		    	MyStrnCpy(szDfltRootPath,szRootBackupPath, PATH_LEN);	// remember the default for next time
				if (status) {
					msg.Printf("Database copied to %s \nsuccessfully and opened.", szCurrentDbPath);	
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
