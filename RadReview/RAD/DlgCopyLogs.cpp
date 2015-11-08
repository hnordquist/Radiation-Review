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
//    File Name : DlgCopyLogs.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
//		UpdateINI_Parms - write parameters to RAD.INI
//		CopyLogsMenuOption - menu dialog and action
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"    
#include "CopyDir.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "Questions.h"
#include "RADInit.H"
#include "Utilities.h"

extern RADInit *pRAD_Init;  

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
	pDlg->FldTxtGetValue(IDC_CLOG_EDIT_NEW_PATH, &Str, PATH_LEN);
	pRAD_Init->Set_LogCopy_To_Path((const char *)Str);
	
   /* ------------------------------------------------------------------
    *	Write the changes to the .ini file
    * ----------------------------------------------------------------*/      
   	pRAD_Init->Save_Parameters();
	
}        


//  27-Aug-2004 SFK		Created
void CopyLogsMenuOption()
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
    	pStr = pRAD_Init->Get_LogCopy_To_Path(); 
		MyStrnCpy(szDfltRootPath, pStr, PATH_LEN); 
   		bFirst = FALSE;
    	strcpy(szDfltCurrentDbPath, szCurrentDbPath);
	}
	
	strcpy(szRootBackupPath, szDfltRootPath);

   /* ------------------------------------------------------------------
    *	Define the buttons and actions in the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg BackupDbDlg(IDD_FILE_COPY_LOGS, NULL, DLG_POSN_NO_SAVE);
    if (BackupDbDlg.IsValid()) {
		BackupDbDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		BackupDbDlg.DefinePushBtn(IDOK, GUI_OK);
		BackupDbDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateINI_Parms);              
			                                               
		BackupDbDlg.DefineFldTxt(IDC_CLOG_EDIT_NEW_PATH, szRootBackupPath, PATH_LEN);
			
		BackupDbDlg.SetFocus(IDOK);
		
	    DlgReturn = BackupDbDlg.Go();
		    
	    if (DlgReturn == GUI_OK) {  
		    	
	    	BackupDbDlg.RetrieveAllControlValues();

		   /* ------------------------------------------------------------------
		    *	Remove spaces, check length
		    * ----------------------------------------------------------------*/
	    	if (PathCopy(szMainBackupPath, szRootBackupPath) != TRUE) return;
	    	strcat(szMainBackupPath, "logs\\rad\\");

	    	if (PathCopy(szMainCurrentDbPath, szCurrentDbPath) != TRUE) return;
	    	strcat(szMainCurrentDbPath, "logs\\rad\\");

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
		    *	See if a log file exists at the location specified.  If yes,
		    *	see if they want to overwrite it.
		    * ----------------------------------------------------------------*/
		    if (FileExists(szMainBackupPath, "RadReviewLog.txt") == TRUE) {
		    	status = AskOverwriteQuestion(szMainBackupPath);
		    	if (status == FALSE) return;
		    }	
		    
		   /* ------------------------------------------------------------------
		    *	Figure out how many bytes will need to be copied and if there
		    *	is room on the destination drive.  (Destination drive must have
		    *	directory size + 10000 to allow for directory entries, etc.)
		    * ----------------------------------------------------------------*/
		    if (GetDirSize(szMainCurrentDbPath, "*.*", &ulDirSize) != TRUE) return;	/* check number of bytes filled in szCurrentDbPath */
			
			// Customize for COMs, if don't write to this log directory

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
			_flushall();
			    
		    status = CopyDirectory(szMainCurrentDbPath, szMainBackupPath);

			// Customize for COMs, if don't write to this log directory

		    if (status != TRUE) { 
		    	RadReviewMsg(uiCOPY_DB_ERR, (const char *)szCurrentDbPath, (const char *)szRootBackupPath);
		    }
		    else {	
		    	MyStrnCpy(szDfltRootPath,szRootBackupPath, PATH_LEN);	// remember the default for next time
				msg.Printf("Logs copied to %s.", szRootBackupPath);	
			    GUI_MsgBox(msg, GUI_ICON_INFO);
		    }	

	    }	
	}    
}   		                                 

