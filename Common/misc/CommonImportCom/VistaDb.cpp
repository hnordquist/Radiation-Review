// DbVista.cpp: implementation of the CDbVista class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support generic dbVista database functions.
//
//	Functions:
//		OpenDatabase
//		CloseDatabase
//		ClearDatabase
//		GetDatabasePathAndName
//		SameDatabase
//		SaveDatabaseNow
//		InitializeDatabase
//		BackupDatabase
//		RestoreDatabase
//		
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vista.h"
#include "VistaDb.h"
#include <direct.h>

#define Currtask m_Currtask

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static bool bStaticQuiet;  //has to be static for this function - initialized in constructor
void EXTERNAL_FIXED CustomDbVistaErrorHandler(int iErrNum, char *ErrMsg)
{
	CString Msg;
	if (iErrNum != S_EACCESS && !bStaticQuiet) 
	{	// 12-Jan-2005 Look for this error to indicate an access problem with another program using the COM, don't put out message
		Msg.Format("Unexpected dbVista error %d\n\n(%s).\n\nContact LANL expert for help.", iErrNum, ErrMsg);

		//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
		if (!bStaticQuiet)
			MessageBox(NULL, Msg, "COM DbVista Error", MB_OK|MB_ICONSTOP);
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDbVista::CDbVista(bool bQuietMode)
{
	m_bQuietMode = bQuietMode;
	bStaticQuiet = bQuietMode;  //static used by CustomDbVistaErrorHandler() above
	m_Dir.SetQuietMode(bQuietMode);

	mstrComponentName.Empty();
	mstrDatabaseName.Empty();	
	mstrDatabasePath.Empty();
	mstrDatabaseRootPath.Empty();
	mstrDefaultPath.Empty();
	mstrMyDbFilename.Empty();
	
	mbDatabaseOpen = false;
	mbRequestToClear = false;
	mbDatabaseCleared = false;
	mbAskClearDbQuestion = false;	
}

CDbVista::~CDbVista()
{
	CloseDatabase();
}


///////////////////////////////////////////////////////////////////////////
//	Name:	OpenDatabase
//
//	Description:
//	Performs the steps to open the database.  Note that the name of any filename
//	or path cannot exceed 47 characters (a dbVista limit).  Also the vista.h file
//	expects a DB_TASK called Currtask.  If an error is encountered anywhere in the
//	steps to open the file, an error message is formed, displayed and returned.
//	If opened successfully, mstrDatabaseName, mstrDatabasePath, and mbDatabaseOpen
//	contain valid information.
//	
//	Declaration:
//	bool CDbVista::OpenDatabase(const CString &strDbPath, const CString &strDbName, CString *pstrErrorMsg)
//
//	Input:
//		strDbPath			directory where the database is found (make sure this pathname has
//								the trailing \ or the open does not work correctly!) (Should
//								include the component name, if applicable).
//		strDbName			name of the database found at strDbPath (may be more than one)
//		pstrErroMsg			blank if no error during open, otherwise informative error message
//
//	Output:
//		
//
//	Return:
//		true (opened successfully) or false (error somewhere in open process)
//
//	Member variables:
//		mstrDatabaseName	name(s) of databases now open
//		mstrDbPath			path where mstrDatabaseName is found
//		mbDatabaseOpen		whether database is currently open or not 
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from OpenDatabase in DbGeneralDbSupport.cpp
///////////////////////////////////////////////////////////////////////////
DB_TASK	Currtask;			// SFK - this has to be named Currtask because of the extern in vista.h
#define DBVISTA_NAMELEN	47
bool CDbVista::OpenDatabase(const CString &strDbPath, const CString &strDbName, CString *pstrErrorMsg)
{
	char strTempPath[DBVISTA_NAMELEN+1];
	char strTempName[DBVISTA_NAMELEN+1];
	char strTempDbFile[DBVISTA_NAMELEN + DBVISTA_NAMELEN +1];
	CString strErrorMsg;

	int iLen = strlen(strDbPath);
	if (iLen > (DBVISTA_NAMELEN - 1)) 
	{
		strErrorMsg.Format("Path formed by %s is %d characters\nDbVista limit is 46 characters.",
			strDbPath, iLen);

		//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
		if (!m_bQuietMode)
			MessageBox(NULL, strErrorMsg, "Database Path Error", MB_OK|MB_ICONSTOP);

		if (pstrErrorMsg) *pstrErrorMsg = strErrorMsg;
		return(false); 
	}
	strncpy(strTempPath, strDbPath, DBVISTA_NAMELEN);
	strncpy(strTempName, strDbName, DBVISTA_NAMELEN);
	if (pstrErrorMsg) pstrErrorMsg->Empty();				// clear the error string

	if (mbDatabaseOpen) CloseDatabase();				// if db is already open, must close it

	//SetCurrentDrive();		// check if we have to implement this in new version SFK

	strcpy(strTempDbFile, strTempName);
	strcat(strTempDbFile, ".dbd");
	if (!m_Dir.FileExists(strTempPath, strTempDbFile)) 
	{
		strErrorMsg.Format("Database does not exist at %s.", strDbPath);

		//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
		if (!m_bQuietMode)
			MessageBox(NULL, strErrorMsg, "Database Open Error", MB_OK|MB_ICONWARNING);

		if (pstrErrorMsg) *pstrErrorMsg = strErrorMsg;
		return(false);
	}
	
    //	Open the database.  This takes several steps.
    if (dt_opentask(&Currtask) != S_OKAY) 
	{
		strErrorMsg.Format("Could not open the dbVista task.\nDatabase at %s could not be opened.", strDbPath);

		//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
		if (!m_bQuietMode)
			MessageBox(NULL, strErrorMsg, "Database Open Error", MB_OK|MB_ICONWARNING);

		if (pstrErrorMsg) *pstrErrorMsg = strErrorMsg;
		return(false);
   	}	

	d_set_dberr(CustomDbVistaErrorHandler);


	if (d_dbtaf(strTempPath) != S_OKAY) 
	{                   
		strErrorMsg.Format("Could not set the dbVista transaction file path variable.\nDatabase at %s could not be opened.", strDbPath);

		//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
		if (!m_bQuietMode)		
			MessageBox(NULL, strErrorMsg, "Database Open Error", MB_OK|MB_ICONWARNING);

		if (pstrErrorMsg) *pstrErrorMsg = strErrorMsg;
		return(false); 
  	}

   	if (d_dbdpath(strTempPath) != S_OKAY) 
	{                   
		strErrorMsg.Format("Could not set the dbVista path variable.\nDatabase at %s could not be opened.", strDbPath);

		//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
		if (!m_bQuietMode)		
			MessageBox(NULL, strErrorMsg, "Database Open Error", MB_OK|MB_ICONWARNING);

		if (pstrErrorMsg) *pstrErrorMsg = strErrorMsg;
		return(false); 
  	}	

   	if (d_dbfpath(strTempPath) != S_OKAY) 
	{                           
		strErrorMsg.Format("Could not set the dbVista file variable.\nDatabase at %s could not be opened.", strDbPath);

		//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
		if (!m_bQuietMode)		
			MessageBox(NULL, strErrorMsg, "Database Open Error", MB_OK|MB_ICONWARNING);

		if (pstrErrorMsg) *pstrErrorMsg = strErrorMsg;
		return(false);
   	}
	
	if (d_open(strTempName,"o") != S_OKAY) 
	{
		strErrorMsg.Format("Error on database open. Database at %s could not be opened.", strDbPath);

		//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
		if (!m_bQuietMode)		
			MessageBox(NULL, strErrorMsg, "Database Open Error", MB_OK|MB_ICONWARNING);

		if (pstrErrorMsg) 
			*pstrErrorMsg = strErrorMsg;

		return(false);
   	}	

 		
	// Save the pathname of the currently open database
	// database was successfully opened, now remember the name and path
	mstrDatabaseName = strDbName;		// name of the database
	mstrDatabasePath = strTempPath;		// path on disk where database is found
	mbDatabaseOpen = true;

	return(true);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	CloseDatabase
//
//	Description:
//	Close the database and task.
//	
//	Declaration:
//	void CDbVista::CloseDatabase()
//
//	Input:	none
//
//	Output:	none
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from CloseDatabase in DbGeneralDbSupport.cpp
//////////////////////////////////////////////////////////////////
void CDbVista::CloseDatabase()
{
	if (mbDatabaseOpen)
	{
		//SetCurrentDrive();		// implement in new version??? SFK
		d_close();
		dt_closetask(&Currtask);                        
		mbDatabaseOpen = false;
	}
}

///////////////////////////////////////////////////////////////////////////
//	Name:	CheckDatabaseSize
//
//	Description:
//	Get size of database and compare it aginst maxallowed using percent arg.
//	
//	Input:	maximum percnate of maximum DB size permitted
//
//	Output:	true iff db size is at or greater than max percent of max allowed DB size
//
//	Return:	bool
//	
//////////////////////////////////////////////////////////////////

#define MAXIMUM_RECOMMENDED_DATABASE_SIZE (1024.0 * 1024.0 * 1024.0 * 2)

void CDbVista::CheckDatabaseSize(double PercentMax, ULONG& CurDBSizeBytes, ULONG& MaxDBSizeBytes, bool& bTooBig)
{
	CString strSearch = mstrDatabasePath;
	int iDum =0;

	strSearch += "DB_DATA.DBF";

	WIN32_FIND_DATA FileData;
	HANDLE pHandle = ::FindFirstFile(strSearch, &FileData);
	if (pHandle == INVALID_HANDLE_VALUE) 
		iDum = GetLastError();


	if (pHandle != INVALID_HANDLE_VALUE) 
	{
		if ((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) 
		{   // don't add if dir
			CurDBSizeBytes = (ULONG)((FileData.nFileSizeHigh * MAXDWORD) + FileData.nFileSizeLow);
		}
	}

	FindClose(pHandle);

	// Determine if size is within PercentMax%
	ULONG deltamax = ULONG(MAXIMUM_RECOMMENDED_DATABASE_SIZE * PercentMax);
	// Determine if size is greater than the maximum recommended size
	bTooBig = (CurDBSizeBytes > deltamax);
	MaxDBSizeBytes = ULONG(MAXIMUM_RECOMMENDED_DATABASE_SIZE);

}


///////////////////////////////////////////////////////////////////////////
//	Name:	ClearDatabase
//
//	Description:
//	If requested display a message warning the user that all data in the database
//	will be destroyed and give them a chance to answer No.  If answer is Yes, then
//	use database function to clear any data in the database.
//	Note:  user must rebuild any structure that is expected in the database
//		following this call such as was done by reading names.cfg in previous version
//	
//	Declaration:
//	bool CDbVista::ClearDatabase(const CString &strDbName, bool bPrompt, CString *pstrErrorMsg)
//
//	Input:	strDbName	name used when opened this database
//			bPrompt		whether to display prompt asking user to clear the db
//
//	Output:	pstrErrorMsg	string containing error message, if any
//
//	Return:	true (db cleared) / false (db not open or error from db internal -- see pstrErrorMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from InitializeDatabase in DbGeneralDbSupport.cpp
//////////////////////////////////////////////////////////////////
bool CDbVista::ClearDatabase(const CString &strDbName, const bool bPrompt, CString *pstrErrorMsg)
{
	int iResponse = IDNO;
	CString strMsg, strTemp;

	mbDatabaseCleared = false;
	mbRequestToClear = false;

	if (pstrErrorMsg) 
		pstrErrorMsg->Empty();

	if (!mbDatabaseOpen)
	{
		if (pstrErrorMsg) 
			pstrErrorMsg->Format("Database not open.");

		return false;
	}

	//	SetCurrentDrive();	// do we have to implement this in new version?? SFK

	//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
	if (bPrompt && !m_bQuietMode) 
	{
		strMsg.Format("You have selected to clear the \n%s database at \n%s\n\n", strDbName, mstrDatabasePath);
		strTemp.Format("This will destroy all data in the database!\n\n");
		strMsg += strTemp;
		strTemp.Format("Are you sure you want to clear the database?");
		strMsg += strTemp;
		iResponse = MessageBox(NULL, strMsg, "Clear Database", MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
	}
	else
	{
		iResponse = IDYES;
	}

	//if((iResponse == IDYES) || !bPrompt)
	if(iResponse == IDYES)
	{
		if (d_initialize(CURR_DB) != S_OKAY) 
		{
			if (pstrErrorMsg) pstrErrorMsg->Format("Could not initialize the &s database.", strDbName);
			return(false);
		}
		mbDatabaseCleared = true;
		mbRequestToClear = true;
	}
	return(true);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetDatabasePathAndName
//
//	Description:
//	Get the path where the currenly opened database resides.
//	
//	Declaration:
//	bool CDbVista::GetDatabasePathAndName(CString *pstrPath, CString *pstrName, CString *pstrErrorMsg)
//
//	Input:	none
//
//	Output:	pstrPath	path where currently opened database is located
//			pstrName	name of currently opened database
//			pstrErrorMsg	string containing error message, if any
//
//	Return:	true (all okay) /false (no database is open)
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from GetDbName in DbGeneralDbSupport.cpp
//////////////////////////////////////////////////////////////////
bool CDbVista::GetDatabasePathAndName(CString *pstrPath, CString *pstrName, CString *pstrErrorMsg)
{
	if (pstrErrorMsg) pstrErrorMsg->Empty();
	if (!mbDatabaseOpen){
		if (pstrErrorMsg) pstrErrorMsg->Format("Database not open.");
		return(false);
	}
	*pstrPath = mstrDatabasePath;
	*pstrName = mstrDatabaseName;
	return(true);

}


///////////////////////////////////////////////////////////////////////////
//	Name:	SameDatabase
//
//	Description:
//	Checks whether the given path and name are the same as that of the currently
//	open database.
//	
//	Declaration:
//	bool CDbVista::SameDatabase(const CString &strPath, const CString &strName, CString* pstrErrorMsg)
//
//	Input:	strPath		path of db to compare to
//			strName		name of db to compare to
//
//	Output:	pstrErrorMsg	string containing error message, if any
//
//	Return:	true (same db) / false (different db or db not open -- see pstrErrorMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from SameDb in DbGeneralDbSupport.cpp
//////////////////////////////////////////////////////////////////
bool CDbVista::SameDatabase(const CString &strPath, const CString &strName, CString* pstrErrorMsg)
{
	if (pstrErrorMsg) pstrErrorMsg->Empty();
	if (!mbDatabaseOpen){
		if (pstrErrorMsg) pstrErrorMsg->Format("Database not open.");
		return(false);
	}

	//	SetCurrentDrive();		// implement in new version?? sfk
	if ((strPath == mstrDatabasePath) && (strName == mstrDatabaseName)) {
		return(true);
	}
	return(false);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	SaveDatabaseNow
//
//	Description:
//	Remember the name and path of the currently open database.  Close the database
//	to flush all buffers, then reopen it.
//	
//	Declaration:
//	bool CDbVista::SaveDatabaseNow();
//
//	Input:	none
//
//	Output:	none
//
//	Return:	true (db saved) / false (error when reopening)
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from SaveDatabaseNow in DbGeneralDbSupport.cpp
//////////////////////////////////////////////////////////////////
bool CDbVista::SaveDatabaseNow()
{

//	SetCurrentDrive();		// implement in new version?? SFK
	CloseDatabase();
	return (OpenDatabase(mstrDatabasePath, mstrDatabaseName));
}


///////////////////////////////////////////////////////////////////////////
//	Name:	InitializeDatabase
//
//	Description:
//	If a database does not exist at the specified directory, copy an empty one
//	to it from the default directory location.  Clear all information from the
//	database at the specified directory.
//	
//	Declaration:
//	bool CDbVista::InitializeDatabase(CString &strDbBasePath)
//
//	Input:	none
//			mstrComponentName	name of the subpath where this instrument's data is kept
//			mstrMyDbFilename	name of file that indicates a database is present
//			mstrDefaultPath		directory where an empty database is found (under mstrComponentName)
//			mstrDatabaseName	name of the database that is thought to exist at the specified dir (under mstrComponentName)
//
//	Output:	none
//
//	Return:	true (db exists and initialized) / false (error in operation)
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//////////////////////////////////////////////////////////////////
bool CDbVista::InitializeDatabase(const CString &strDbBasePath,const bool bPrompt)
{
	//CDirUtilities m_Dir;
	CString strError, strEmptyDbPath;
	
	if ((mstrComponentName.GetLength() == 0) || (mstrMyDbFilename.GetLength() == 0)) {
		strError.Format("Initialize Database: No component name or db filename set program error, cannot continue.");

		//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
		if (!m_bQuietMode)
			MessageBox(NULL, strError, "Fatal Error", MB_OK|MB_ICONWARNING);

		return(false);
	}

	CString strTemp = m_Dir.AppendPath(strDbBasePath, mstrComponentName);	// create the instrument Com specific path
	
	bool bSuccess = m_Dir.FileExists(strTemp, mstrMyDbFilename);		// does a database exist there already?
	if (mbDatabaseOpen) CloseDatabase();							// close any database that might be open now

	if (!bSuccess) 
	{			// no database where we are to initialize one
		if (mstrDefaultPath.GetLength() == 0) 
		{
			strError.Format("Initialize Database: No Default Path Set program error, cannot continue.");

			//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
			if (!m_bQuietMode)		
				MessageBox(NULL, strError, "Fatal Error", MB_OK|MB_ICONWARNING);

			return(false);
		}
		strEmptyDbPath = m_Dir.AppendPath(mstrDefaultPath, mstrComponentName);
		bSuccess = m_Dir.CopyDirectory(strEmptyDbPath, strTemp, true);	// copy an empty one over there
	}

	if (bSuccess) 		// database exists, open it and clear it
	{
		bSuccess = OpenDatabase(strTemp, mstrDatabaseName);

		//NDAR change so that ClearDatabase() will do the right thing.
		//Changed back PJM 11/27/2007 for B2R1
		bool b = bPrompt;

		if (m_bQuietMode)
			b = false;

		if (bSuccess) 
			bSuccess = ClearDatabase(mstrDatabaseName, b);
	}

	return (bSuccess);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	BackupDatabase
//
//	Description:
//	Copy all the files in the directory specified by mstrDatabasePath 
//	(under mstrComponentName) to the specified directory.  If a database is
//	already at the destination directory, ask if the user wants to copy
//	over it.
//	
//	Declaration:
//	bool CDbVista::BackupDatabase(CString &strDestinationBasePath)
//
//	Input:	strDestinationBasePath	base path of where to copy the database to
//			mstrComponentName		name of the subpath where this instrument's data is kept
//			mstrMyDbFilename	name of file that indicates a database is present
//			mstrDatabaseName	name of the database that is thought to exist at the specified dir (under mstrComponentName)
//
//	Output:	none
//
//	Return:	true (db exists and initialized) / false (error in operation)
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//	01-May-2002	SFK		Add boolean to pass in overwrite directions
//////////////////////////////////////////////////////////////////
bool CDbVista::BackupDatabase(const CString &strDestinationBasePath, const bool bPrompt, const bool bOverwrite)
{
	//CDirUtilities m_Dir;
	bool bOverwriteAnswer = bOverwrite;
	bool bSuccess = false;
	CString strError;
	
	if ((mstrComponentName.GetLength() == 0) || (mstrMyDbFilename.GetLength() == 0)) {
		strError.Format("Backup Database: No component name or db filename set program error, cannot continue.");

		//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
		if (!m_bQuietMode)		
			MessageBox(NULL, strError, "Fatal Error", MB_OK|MB_ICONWARNING);

		return(false);
	}

	CString strFullDestPath = m_Dir.AppendPath(strDestinationBasePath, mstrComponentName);	// create the Instrument Com specific path

	bool bExists = m_Dir.FileExists(strFullDestPath, mstrMyDbFilename);		// does a database exist there already?

	//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
	if (!m_bQuietMode && bExists && bPrompt) 
		bOverwriteAnswer = m_Dir.OverwriteDbQuestion(strFullDestPath);		// database exists, see if they want to overwrite

	if ((!bExists) || (bExists && bOverwriteAnswer)) 	// copy current database to the backup location
	{
		CString strDbPath = mstrDatabasePath;
		if (mbDatabaseOpen) 
		{						// have to close the current database during the copy
			CString strDbName = mstrDatabaseName;
			CloseDatabase();
			bSuccess = m_Dir.CopyDirectory(strDbPath, strFullDestPath, true);	// copy current db to destination
			OpenDatabase(strDbPath, strDbName);
		}
		else 	// database is closed, just copy
		{
			bSuccess = m_Dir.CopyDirectory(strDbPath, strFullDestPath, true);	// copy current db to destination
		}
	}

	if (!bOverwriteAnswer) bSuccess = true;	// chose not to overwrite

	return(bSuccess);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	RestoreDatabase
//
//	Description:
//	Copy all the files frp, the directory specified to the current database
//	diectory specified by mstrDatabasePath (under mstrComponentName).
//	If a database is already at the destination directory, ask if the user 
//	wants to copy over it.
//	
//	Declaration:
//	bool CDbVista::RestoreDatabase(const CString &strSourceBasePath, const CString &strDestBasePath, const bool bPrompt, const bool bOverwrite)
//
//	Input:	strSourceBasePath	base path of from where to copy the database
//			mstrComponentName	name of the subpath where this instrument's data is kept
//			mstrMyDbFilename	name of file that indicates a database is present
//			mstrDatabaseName	name of the database that is thought to exist at the specified dir (under mstrComponentName)
//
//	Output:	none
//
//	Return:	true (db exists and initialized) / false (error in operation)
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//	01-May-2002	SFK		Add boolean to pass in overwrite directions
//////////////////////////////////////////////////////////////////
bool CDbVista::RestoreDatabase(const CString &strSourceBasePath, const CString &strDestBasePath, const bool bPrompt, const bool bOverwrite)
{
	//CDirUtilities m_Dir;
	bool bOverwriteAnswer = bOverwrite;
	bool bSuccess = false;
	
	CString strSrcFullPath = m_Dir.AppendPath(strSourceBasePath, mstrComponentName);	// create the instrument Com specific path
	CString strDestFullPath = m_Dir.AppendPath(strDestBasePath, mstrComponentName);	// create the instrument Com specific path

	bool bExists = m_Dir.FileExists(strDestBasePath, mstrMyDbFilename);		// does a database exist there already?
	
	//WAS "NOT FOR NDAR".  Reenabled by pjm 11/27/2007 for B2R1
	if (!m_bQuietMode && bExists && bPrompt) 
		bOverwriteAnswer = m_Dir.OverwriteDbQuestion(strDestBasePath);	// database exists, see if they want to overwrite

	if ((!bExists) || (bExists && bOverwriteAnswer)) 
	{	// overwrite the destination database with the restored one
		CString strDbPath = mstrDatabasePath;
		if (mbDatabaseOpen) 
		{							// close the current database during the copy to be safe
			CString strDbName = mstrDatabaseName;
			CloseDatabase();
			bSuccess = m_Dir.CopyDirectory(strSrcFullPath, strDestFullPath, true);	// copy from source to destination
			OpenDatabase(strDbPath, strDbName);
		}
		else 
		{
			bSuccess = m_Dir.CopyDirectory(strSrcFullPath, strDestFullPath, true);	// copy from source to destination
		}
	}

	if (!bOverwriteAnswer) 
		bSuccess = true;	// chose not to overwrite
	
	return(bSuccess);
}



