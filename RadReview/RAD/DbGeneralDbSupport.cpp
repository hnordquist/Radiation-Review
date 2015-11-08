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
//    File Name : DbGeneralDbSupport.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include <direct.h>
#include "RAD.H"
#include "BinaryImport.h"
#include "CopyDir.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "GrandImport.h"
#include "RadInit.H"
#include "RadReview.h"
#include "ReadCfg.h"
#include "Utilities.h"
#include "TimeAlign.h"	// rad 3.00
#include "VifmImport.h"
#include "EOSSImport.h"

extern RADInit *pRAD_Init;
extern RADReview *pRAD_Review;

extern short glsFacNum;
extern short glsStaNum;
extern short glsChanNum;
extern bool glbFacilityPicklistFilled;
extern bool glbStationPicklistFilled;
extern bool glbChannelPicklistFilled;

DB_TASK	Currtask;                       
                      
static char gllDBOpen[PATH_LEN+1]="";

              

/*===========================================================================
 *
 *  Name	 :  InitializeDatabase
 *
 *  Purpose	 :  Tells the user this operation will destroy all data in the
 *				database and queries if they want to proceed.  If they choose
 *				to proceed, the database is cleared and the configuration
 *				information read from the names.cfg file in the database
 *				directory of the current open database.
 *
 *  Return value :  TRUE - database cleared and config information read
 *		    		FALSE - user chose not to clear the database, error in
 *						reading the names.cfg file or error in clearing the
 *						database, gllDBOpen.
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  08-Jul-1996  SFK	 Created header comment block, changed logic to print
 *							message only if not successful in the clear and reading
 *							the names.cfg file.
 *	02-Apr-1999	 SFK	 Initialize operator database as well as main database.
 *	26-Oc6-2004	SFK		Changed to initialize based on Facility ID rather than path
 *
===========================================================================*/
int InitializeDatabase(BOOL bPrompt)
{                                 
	int status = 0;                                          
	GUI_ACTION response = GUI_NO;
	CGFmtStr msg;

	SetCurrentDrive();	
	if (bPrompt) 
	{
		msg.Printf("You have selected to clear the database at \n%s\n\n", gllDBOpen);
		msg.PrintfAppend("This will destroy all data in the database!\n\n");
		msg.PrintfAppend("Are you sure you want to clear the database?");
		response = GUI_MsgBox(msg, GUI_ICON_QUESTION, GUI_YESNO);
	}	
	if( (response == GUI_YES) || !bPrompt) 
	{
		if (d_initialize(CURR_DB) != S_OKAY) 
		{
			RadReviewMsg(uiDB_INIT_ERR, "InitializeDatabase");	    			
			return 0;
		}	                                                        
		
	   /* ------------------------------------------------------------------
	    *	Read configuration files to get station and facility records.
	    *	Do not read the default file because do not want to change
	    *	the default database.
	    * ----------------------------------------------------------------*/
	    status=ReadCfgFile();
	    if (status != 1) 
		{
			RadReviewMsg(uiNAMES_CFG_READ_ERR);
		    return 0;
		}    
		if (d_initialize(OP_DB) != S_OKAY) 
		{
			RadReviewMsg(uiDB_INIT_ERR, "InitializeOperatorDatabase");	    			
			return 0;
		}
		
		// Customize for new COMs
		if (!pglGrandImport->InitializeDatabase(glsFacNum, false)) {
			RadReviewMsg(uiDB_INIT_ERR, "Initialize Grand Import Com Database");	    			
			return 0;
		}
		if (!pglVifmImport->InitializeDatabase(glsFacNum, false)) {
			RadReviewMsg(uiDB_INIT_ERR, "Initialize Vifm Import Com Database");	    			
			return 0;
		}
		if (!pglBinaryImport->InitializeDatabase(glsFacNum, false)) {
			RadReviewMsg(uiDB_INIT_ERR, "Initialize Binary Import Com Database");	    			
			return 0;
		}
		if (!pglEOSSImport->InitializeDatabase(glsFacNum, false)) {
			RadReviewMsg(uiDB_INIT_ERR, "Initialize EOSS Import Com Database");	    			
			return 0;
		}
		//rad 3.00
		if (!pglTimeAlign->InitializeDatabase(glsFacNum)) {
			RadReviewMsg(uiDB_INIT_ERR, "Initialize Time Align Com Database");	    			
			return 0;
		}
		return 1;	
	}	
	else 
	{
		return 0;	
	}	
}   		                                 


/* <f> */
/*===========================================================================
 *
 *  Name	 :  OpenDatabase
 *
 *  Purpose	 :  Checks that the path name of the database to open is within length
 *				limits.  If yes, goes through dbVista sequence to open the database
 *				and current task.
 *
 *  Return value :  TRUE - database opened
 *		    		FALSE - database could not be opened
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  08-Jul-1996  SFK	Created header comment block, removed logic for multiple copies
 *						of database name in other static variables.
 *	01-Apr-1999	 SFK	Added open of db_oper
 *  20-Mar-2001	 SFK	Added open of Access database
 *	29-Sep-2003	SFK		Changed name of main db from db_rmon to db_main
 *	07-Jul-2004	SFK		Changed to find main rad database in \rad subdirectory and 
 *						default db in \config subdirectory
 *	29-Oct-2004	SFK		Changed to open databases based on Facility ID, not path
 *
===========================================================================*/
bool OpenDatabase(char *path, short sFacNum)
{
	CGFmtStr msg;
	char TempPath[DB_NAME_LEN+1];
//	char TempName[DB_NAME_LEN+DOSNAME_LEN+1];
	int status;
	BOOL bAtLeastOneOpen = false;
	BOOL bTimeAlign = false;
	                                    
	SetCurrentDrive();
	status = PathCopy(TempPath, path);
	if (status != TRUE) 
	{	                                    
		msg.Printf("Database path name of %s is too long.", TempPath);
		goto errorexit;
	}
		
   /* ------------------------------------------------------------------
    *	Open the database
    * ----------------------------------------------------------------*/
    if (dt_opentask(&Currtask) != S_OKAY) {
    	msg.Printf("Could not open the database task.");
   		goto errorexit;
   	}	
   	if (d_dbtaf(TempPath) != S_OKAY) {                   
   		msg.Printf("Could not set the database transaction file path variable.");
		goto errorexit; 
  	}	
   	if (d_dbdpath(TempPath) != S_OKAY) {                   
   		msg.Printf("Could not set the database path variable.");
		goto errorexit; 
  	}	
   	if (d_dbfpath(TempPath) != S_OKAY) {                           
   		msg.Printf("Could not set the database file variable.");
		goto errorexit;
   	}	
	if (d_open("data\\rad\\db_main;config\\rad\\db_dflt;data\\rad\\db_oper","o") != S_OKAY) {                     
 		msg.Printf("Could not open database, check path is correct.");
      goto errorexit;
 	}	

	status = OpenGrandImportDatabase(sFacNum);		//Open the GrandImport COM object
	if (!status)
		RadReviewMsg(uiOPEN_COM_DB_WARN, "GrandImport", TempPath);
	else 
		bAtLeastOneOpen = true;
	
	status = OpenVifmImportDatabase(sFacNum);	//Open the VifmImport COM object
	if (status != TRUE) {
		RadReviewMsg(uiOPEN_COM_DB_WARN, "VifmImport", TempPath);
	}
	else bAtLeastOneOpen = true;
	
	status = OpenBinaryImportDatabase(sFacNum);	// Open the GrandImport COM object
	if (status != TRUE) {
		RadReviewMsg(uiOPEN_COM_DB_WARN, "BinaryImport", TempPath);
	}
	else bAtLeastOneOpen = true;

	status = OpenTimeAlignDatastore(sFacNum);	//	Open the TimeAlign COM object
	if (status != TRUE) {
		RadReviewMsg(uiOPEN_COM_DB_WARN, "Time Align", TempPath);
	}
	else bTimeAlign = true;
   	
   /* ------------------------------------------------------------------
    *	If one Com is there, then TimeAlign is also needed.  If were 
	*	able to open the original or at least one Com,
	*	save the pathname of the currently open database
    * ----------------------------------------------------------------*/
	if (bAtLeastOneOpen  && !bTimeAlign) 
		return false;

	strcpy(gllDBOpen, TempPath);
	return true;
    
errorexit:
	RadReviewMsg(uiCANNOT_OPEN_ANY_DATABASE_ERR, path);
   	return false;

}   


 
/* <f> */
/*===========================================================================
 *
 *  Name	 :  CloseDatabase
 *
 *  Purpose	 :  Close the database and task.  Set the name of the currently open db to ""
 *
 *  Return value :  TRUE 
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  08-Jul-1996  SFK	Created header comment block
 *
===========================================================================*/
int CloseDatabase()
{                         
	if (gllDBOpen[0] != '\0')
	{
		SetCurrentDrive();
		d_close();
		dt_closetask(&Currtask);                        
		strcpy(gllDBOpen, "");

		// Customize for new COMs
		// close any ComObject databases
		if (pglGrandImport) pglGrandImport->CloseDatabase();
		if (pglVifmImport) pglVifmImport->CloseDatabase();
		if (pglBinaryImport) pglBinaryImport->CloseDatabase();
		if (pglEOSSImport) pglEOSSImport->CloseDatabase();
		if (pglTimeAlign) pglTimeAlign->CloseDatabase();	// rad 3.00
	}
    return 1;                                 	
}
                             
                             
                             

/* <f> */
/*===========================================================================
 *
 *  Name	 :  GetDbName
 *
 *  Purpose	 :  Return a string containing the name and path of the currently open database.
 *
 *  Return value :  none
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  12-Jul-1996  SFK	Created header comment block
 *	13-Jan-1999	 SFK	Changed to only return the first DRIVE_LEN+PATH_LEN characters of the path
 *
===========================================================================*/
void GetDbName(char *szDbName)
{
	SetCurrentDrive();	
	MyStrnCpy(szDbName, gllDBOpen, DRIVE_LEN+PATH_LEN-1);
}	              


/* <f> */
/*===========================================================================
 *
 *  Name	 :  SameDb
 *
 *  Purpose	 :  Check if the specified database is the same as the currently open database.
 *
 *  Return value :  TRUE - database specified is same as open database
 *					FALSE - database specified is different from open database
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  12-Jul-1996  SFK	Created header comment block
 
===========================================================================*/
int SameDb(char *szDbName)
{

	SetCurrentDrive();	
	if (strcmp(szDbName, gllDBOpen) == 0) {
		return(TRUE);
	}
	return(FALSE);
}		


/* <f> */
/*===========================================================================
 *
 *  Name	 :  SaveDatabaseNow
 *
 *  Purpose	 :  Saves the current databases by opening and closing them
 *
 *  Return value :  TRUE - all okay
 *					FALSE - problem
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  39-Apr-1997  SFK	Created 
 *
===========================================================================*/
BOOL SaveDatabaseNow(void)
{                         
	OutputDebugString("SaveDatabaseNow()\n");
	char szTempDBName[PATH_LEN+1];
	                            
	SetCurrentDrive();
	strcpy(szTempDBName, gllDBOpen);	                            
	CloseDatabase();
	if (OpenDatabase(szTempDBName, glsFacNum))
		return TRUE;
	else
		return(FALSE);
}		
	

///////////////////////////////////////////////////////////////////////////
//	Name:	SwitchDatabases
//
//	Description:
//	Handle opening and closing of databases to switch from one to another
//	
//	Declaration:
//	BOOL SwitchDatabases (char *szNewDbPath, short sFacNum, BOOL bMsg)
//
//	Input:	szNewDbPath		Full path to where the database to be opened is located
//			sFacNum			Facility number of the currently open database
//			bMsg			If TRUE, then put up message if successful
//
//	Output:	None
//
//	Return:	TRUE (switch successful)/FALSE (old database still the one
//	
//  date    /	author	revision
//  -----------------	--------
//	08-Jun-2005	SFK		Moved from file DlgSelectDb.Cpp
//////////////////////////////////////////////////////////////////
BOOL SwitchDatabases (char *szNewDbPath, short sFacNum, BOOL bMsg)
{
	char szCurrentDbPath[PATH_LEN+1];
	char szNewOpenINI_File[PATH_LEN+DOSNAME_LEN+1];
	char szTemp[PATH_LEN+1];
				 
	CGUI_Dlg WaitDlg(IDD_LOADINGPARMS, NULL, DLG_POSN_NO_SAVE);

	GetDbName(szCurrentDbPath);		// current database

   /* ------------------------------------------------------------------
	*	See if the new path exists.  If not, say why.
	* ----------------------------------------------------------------*/
	int status = PathExists(szNewDbPath); 
	if (( status == uiPATH_DOES_NOT_EXIST_ERR) || ( status == uiDRIVE_DOES_NOT_EXIST_ERR)) {
		RadReviewMsg(status, (const char *)szNewDbPath);
		return (FALSE);
	}	
	    	         
   /* ------------------------------------------------------------------
	*	See if a data base exists where want to open 
	* ----------------------------------------------------------------*/
	PathCopy(szTemp, szNewDbPath);
	MyStrnCat(szTemp, "data\\rad\\", PATH_LEN);
	if (FileExists(szTemp, "db_main.dbd") != TRUE) {
		RadReviewMsg(uiDB_DOES_NOT_EXIST_ERR, (const char *)szNewDbPath);
		return (FALSE); 
	}	
	
   /* ------------------------------------------------------------------
	*	Close existing database
	* ----------------------------------------------------------------*/
	CloseDatabase();

   /* ------------------------------------------------------------------
	*	Try to open the new database and read a record from it.
	* ----------------------------------------------------------------*/
	if (OpenAndAccessDatabase(szNewDbPath, sFacNum) == TRUE) { // open and get parameters from dbVista db
		WaitDlg.DefinePushBtn(IDOK, GUI_NONE);
		WaitDlg.SetVisible(IDOK, FALSE);
		if (WaitDlg != NULL) {
			WaitDlg.GoModeless(FALSE);			
		}

	   /* ------------------------------------------------------------------
		*	Switch to the .INI file associated with the new database
		* ----------------------------------------------------------------*/
		strcpy(szNewOpenINI_File, szNewDbPath);
		if (szNewOpenINI_File[strlen(szNewOpenINI_File) -1]  != '\\') strcat(szNewOpenINI_File, "\\");
		strcat(szNewOpenINI_File, "config\\Rad\\RAD.INI");
		pRAD_Init->Set_New_Initialization_File(szNewOpenINI_File);
		pRAD_Review->RADReviewSetMainMenuOptions();
		//delete WaitDlg;
		/* ------------------------------------------------------------------
		*	Have got a good database and a rad.ini, set these global parameters.
		* ----------------------------------------------------------------*/
		if (!GetDefaultFacilityNumber(&glsFacNum)) return(FALSE);
		//if ((AccessDefaultDatabase(READ, S_DFLT_FACILITY, (short *)&glsFacNum)) &&
		if ((AccessDefaultDatabase(READ, S_DFLT_STATION, (short *)&glsStaNum)) &&
			(AccessDefaultDatabase(READ, S_DFLT_CHANNEL, (short *)&glsChanNum))) {
		}
		else {
			RadReviewMsg(status, "when reading from Default database.  Cannot proceed.");			
			return (FALSE);
		}	
		// were able to open the new one
		if (bMsg) 
			RadReviewMsg(uiDATABASE_OPENED_INFO, szNewDbPath);

		glbFacilityPicklistFilled = false;
		glbStationPicklistFilled = false;
		glbChannelPicklistFilled = false;
		return(TRUE);
	}
	else 
	{
		// couldn't open the new one so go back to the old one
		bool b = OpenDatabase(szCurrentDbPath, glsFacNum);
	    if (bMsg) 
			RadReviewMsg(uiDATABASE_OPENED_INFO, szCurrentDbPath);
		return (FALSE);
	}
}


///////////////////////////////////////////////////////////////////////////
//	Name:	CreateFacilityDatastore
//
//	Description:
//	If empty databases and configuration information do not exist at a current
//	directory, then create config and data directories for a new facility.
//  Also put the proper empty databases or default INI files in the directories.
//	
//	Declaration:
//	BOOL CreateFacilityDatastore (char *szFacilityRootDir)
//
//	Input:	szFacilityRootDir	Facility root path where want config and data
//
//	Output:	None
//
//	Return:	TRUE (datastore existed or created successfully)/FALSE (couldn't make datastore)
//	
//  date    /	author	revision
//  -----------------	--------
//	08-Jun-2005	SFK		Created
///////////////////////////////////////////////////////////////////////////
BOOL CreateFacilityDatastore (char *szFacilityRootDir)
{
	char szCurWkgDir[PATH_LEN+1];
	char szTemp[PATH_LEN+1];
	char szDefaultConfig[PATH_LEN+1], szDestinationConfig[PATH_LEN+1];
	char szDefaultData[PATH_LEN+1], szDestinationData[PATH_LEN+1];
	char szLine[MAX_LINE_LEN+1];
	char szTempFacilityRootDir[PATH_LEN+1];
				 
	// If the new path exists and main db file exists, assume all is there and don't do anything.
	PathCopy(szTemp, szFacilityRootDir);
	MyStrnCat(szTemp, "data\\rad\\", PATH_LEN);
	if ((PathExists(szFacilityRootDir) == TRUE) && (FileExists(szTemp, "db_main.dbd") == TRUE)) return(TRUE); 

	CGUI_Dlg WaitDlg(IDD_LOADINGPARMS, NULL, DLG_POSN_NO_SAVE);

	// Get the current working directory and create the paths to the default config and data that
	// sit under \irs\rad\ at irs\rad\DefaultConfig and irs\rad\DefaultData.

	// Create pathnames to the default configuration and data directories
	_getcwd(szCurWkgDir, PATH_LEN);
	PathCopy(szTempFacilityRootDir, szFacilityRootDir);
	PathCopy(szDefaultConfig, szCurWkgDir);
	PathCopy(szDefaultData, szCurWkgDir);
	MyStrnCat(szDefaultConfig, "DefaultConfig\\", PATH_LEN);
	MyStrnCat(szDefaultData, "DefaultData\\", PATH_LEN);

	// Create pathnames to the destination for the new facility
	PathCopy(szDestinationConfig, szTempFacilityRootDir);
	PathCopy(szDestinationData, szTempFacilityRootDir);
	MyStrnCat(szDestinationConfig, "Config\\Rad\\", PATH_LEN);
	MyStrnCat(szDestinationData, "Data\\Rad\\", PATH_LEN);

	// Now create the actual directories.  If fail, can't do anything.
	if ((!MakePath(szDestinationConfig)) || (!MakePath(szDestinationData))) return(FALSE);

	// Now can do the actual copy
	int iStatus = CopyDirectory(szDefaultConfig, szDestinationConfig);
	if (iStatus) iStatus = CopyDirectory(szDefaultData, szDestinationData);
	// Give hint to fix up the inspec.ini pathname
	if (iStatus) {
		char szNewIniFile[PATH_LEN+1], szOldIniFile[PATH_LEN+1];
		// Get path and name for "new" temporary file
		PathCopy(szNewIniFile, szDestinationConfig);
		MyStrnCat(szNewIniFile, "Temp.INI", PATH_LEN);
		// get path and name for existing RAD.INI file
		PathCopy(szOldIniFile, szDestinationConfig);
		MyStrnCat(szOldIniFile, "Rad.INI", PATH_LEN);
		// create path and name for what we want the  line to be for RT_INSPEC_INFO_COMMON_FILENAME
		sprintf(szDestinationData, "RT_INSPEC_INFO_COMMON_FILENAME\t%sData\\INSPEC.INI",szTempFacilityRootDir);
		// Copy the data from the old RAD.INI file, line by line, substituting the proper path for the INSPEC.ini
		// under the new data store.
		FILE *fNewHandle = fopen(szNewIniFile, "w+");
		FILE *fOldHandle = fopen(szOldIniFile, "r");
		if (fNewHandle && fOldHandle) {	// both files open
			while (fgets(szLine, MAX_LINE_LEN, fOldHandle) != NULL) {
				if (strstr(szLine, "RT_INSPEC_INFO_COMMON_FILENAME\t")) {
					fputs(szDestinationData, fNewHandle);
				}
				else {
					fputs(szLine, fNewHandle);
				}
			}
		}
		fcloseall();
		remove(szOldIniFile);		// delete the current Rad.ini
		rename(szNewIniFile, szOldIniFile);		// Rename temp.ini to rad.ini

		CGFmtStr szMsg;
		szMsg.Printf("No datastore existed.  A new facility datastore has been created.\nYou must now restart Radiation Review.");
		GUI_MsgBox(szMsg, GUI_ICON_INFO);
	}
	
	return(iStatus);
}
