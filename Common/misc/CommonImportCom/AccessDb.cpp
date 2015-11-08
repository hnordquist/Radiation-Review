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
// AccessDb.cpp: implementation of the CDbAccess class.
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
#include <oledb.h>
#include "AccessDb.h"
#include <direct.h>
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
// The CDbAccess class contains common functionality that happens with
// all Access based databases regardless of the type of data in them
// (this includes things like opening, closing, clearing, copying, etc).
// It is built on the CAdo class which handles the actual "talking" to
// the Access database.
// If a data type has some functionality specifically dealing with its
// data that functionality should be included in a class that inherits
// from this class (such as CBinaryDbAccess where specific actions
// associated only with binary data are handled).
//////////////////////////////////////////////////////////////////////

CDbAccess::CDbAccess()
{
	TRACE("CDbAccess::CTOR\n");

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

	// quiet mode logic must be inserted when logic
	// is added to incorporate the CoEventAnalysis component
	// and any other Access-based components.
	// NDAR must be able to turn off GUI interactions.
	m_Dir.SetQuietMode(false);  //for B2R1
}

CDbAccess::~CDbAccess()
{
	TRACE("CDbAccess::DTOR\n");
	CloseDatabase(true);  //true -> compact the database on exit
}

bool CDbAccess::DatabaseExists (const CString& csDataStorePath,const CString& dbName)
{
        return GetFileAttributes((LPCTSTR) (csDataStorePath+"\\"+ dbName)) != 0xFFFFFFFF;
        //return (FileExists ((LPCTSTR) csDataStorePath, (LPCTSTR) dbName));
}


///////////////////////////////////////////////////////////////////////////
//	Name:	OpenDatabase
//
//	Description:
//	Opens the database by using the Open function in CAdoDb.  This is basically
//	a pass thru function.
//	If opened successfully, mstrDatabaseName and mstrDatabasePath
//	contain valid information.
//	
//	Declaration:
//	bool CDbAccess::OpenDatabase(const CString &strDbPath, const CString &strDbName, CString *pstrErrorMsg)
//
//	Input:
//		strDbPath			directory where the database is found. (Should
//								include the component name, if applicable).
//		strDbName			name of the database found at strDbPath (may be more than one)
//		pstrErrorMsg		blank if no error during open, otherwise informative error message
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
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Sep-2002	SFK		Created from OpenDatabase in DbVista.cpp and DbBaseGlue.cpp in Rad and
//						DbVista.cpp in CommonImportCom
///////////////////////////////////////////////////////////////////////////
bool CDbAccess::OpenDatabase(
	const CString &strDbPath, 
	const CString &strDbName, 
	CString *pstrErrorMsg)
{
	bool bStatus = false;

	mstrDatabasePath.Empty();
	
	if (mbDatabaseOpen) CloseDatabase();

	// check if the file exists at the location
	if (m_Dir.FileExists(strDbPath, strDbName)) 
	{
		bStatus = Open(strDbName, strDbPath, "", pstrErrorMsg);
		if (bStatus) 
		{
			// Save the pathname of the currently open database
			// database was successfully opened, now remember the name and path
			mstrDatabasePath = strDbPath;		// path on disk where database is found
			mbDatabaseOpen = true;
		}
	}
	
	return(bStatus);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	CloseDatabase
//
//	Description:
//	Close the database by calling Close in CAdoDb
//	
//	Declaration:
//	void CDbAccess::CloseDatabase()
//
//	Input:	none
//
//	Output:	none
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Sep-2002	SFK		Created from CloseDatabase in DbVista.cpp
//	04-Dec-2003	SFK		Added the suppression of error messages during the compaction
//////////////////////////////////////////////////////////////////
void CDbAccess::CloseDatabase(bool bCompactDb)
{
	if (mbDatabaseOpen)
	{
		if (bCompactDb)
		{
			m_bSuppressErrorMsg = true;
			CompactDb();
			m_bSuppressErrorMsg = false;
		}
		Close();
		mbDatabaseOpen = false;
	}
}



///////////////////////////////////////////////////////////////////////////
//	Name:	ClearDatabase
//
//	Description:
//	If requested display a message warning the user that all data in the database
//	will be destroyed and give them a chance to answer No.  If answer is Yes, then
//	get a list of all the tables in the database and delete them one by one.
//	Note:  user must rebuild any structure that is expected in the database
//		following this call such as was done by reading names.cfg in previous version
//	
//	Declaration:
//	bool CDbAccess::ClearDatabase(const CString &strDbName, bool bPrompt, CString *pstrErrorMsg)
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
//	03-Sep-2002	SFK		Created from InitializeDatabase in DbGeneralDbSupport.cpp
//////////////////////////////////////////////////////////////////
bool CDbAccess::ClearDatabase(const CString &strDbName, const bool bPrompt, CString *pstrErrorMsg)
{
	int iResponse = IDNO;
	CString strMsg, strTemp, strQuery;

	mbDatabaseCleared = false;
	mbRequestToClear = false;
	bool bSuccess = true;

	if (pstrErrorMsg) pstrErrorMsg->Empty();
	if (!mbDatabaseOpen){
		if (pstrErrorMsg) pstrErrorMsg->Format("Database not open.");
		return(false);
	}

	//	SetCurrentDrive();	// do we have to implement this in new version?? SFK
	/*NOT FOR NDAR
	if (bPrompt) {
		strMsg.Format("You have selected to clear the \n%s database at \n%s\n\n", strDbName, mstrDatabasePath);
		strTemp.Format("This will destroy all data in the database!\n\n");
		strMsg += strTemp;
		strTemp.Format("Are you sure you want to clear the database?");
		strMsg += strTemp;
		iResponse = MessageBox(NULL, strMsg, "Clear Database", MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
	}
	if((iResponse == IDYES) || !bPrompt)
	*/
	{
		// we have been requested to clear the database, get the names of all the tables
		// and delete them
		// First verify we really have the database we think we have.
		GetDbName(&strTemp);
		if (strTemp.Compare(strDbName) == 0) {	// the names are the same
			CString *TableNames;
			int iNumTables = GetTableNames(&TableNames);
			for (int i=0; i<iNumTables; i++) {
				strQuery.Format("DROP TABLE [%s];", TableNames[i]);	// drop the table with the station data
				bSuccess = ExecuteSqlCommand(strQuery);
				if ((!bSuccess) && (pstrErrorMsg)) pstrErrorMsg->Format("Could not drop the %s table.", TableNames[i]);
			}
			delete [] TableNames;
			if (bSuccess) {
				mbDatabaseCleared = true;
				mbRequestToClear = true;
			}
		}
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
//	bool CDbAccess::GetDatabasePathAndName(CString *pstrPath, CString *pstrName, CString *pstrErrorMsg)
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
//	03-Sep-2002	SFK		Used as is from DbVista.cpp
//////////////////////////////////////////////////////////////////
bool CDbAccess::GetDatabasePathAndName(CString *pstrPath, CString *pstrName, CString *pstrErrorMsg)
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
//	bool CDbAccess::SameDatabase(const CString &strPath, const CString &strName, CString* pstrErrorMsg)
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
//	03-Sep-2002	SFK		Used as is from DbVista.cpp
//////////////////////////////////////////////////////////////////
bool CDbAccess::SameDatabase(const CString &strPath, const CString &strName, CString* pstrErrorMsg)
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
//	bool CDbAccess::SaveDatabaseNow();
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
//	03-Sep-2002	SFK		Used as is from DbVista.cpp
//////////////////////////////////////////////////////////////////
bool CDbAccess::SaveDatabaseNow()
{
	CString strPath, strName;
//	SetCurrentDrive();		// implement in new version?? SFK

	// remember name and path now
	strName = mstrDatabaseName;
	strPath = mstrDatabasePath;

	CloseDatabase();
	return (OpenDatabase(strPath, strName));
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
//	bool CDbAccess::InitializeDatabase(const CString &strDbBasePath,const bool bPrompt)
//
//	Input:	strDbBasePath		root path of database to be initialized
//			bPrompt				whether or not to ask a question warning the user you are about to clear the database
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
//	03-Sep-2002	SFK		Used as is from DbVista.cpp
//////////////////////////////////////////////////////////////////
bool CDbAccess::InitializeDatabase(const CString &strDbBasePath,const bool bPrompt)
{
	CString strError, strEmptyDbPath;
	
	if ((mstrComponentName.GetLength() == 0) || (mstrMyDbFilename.GetLength() == 0)) {
		strError.Format("Initialize Database: No component name or db filename set program error, cannot continue.");
		/*NOT FOR NDAR
		MessageBox(NULL, strError, "Fatal Error", MB_OK|MB_ICONWARNING);
		*/
		return(false);
	}

	CString strTemp = m_Dir.AppendPath(strDbBasePath, mstrComponentName);	// create the instrument Com specific path
	
	bool bSuccess = m_Dir.FileExists(strTemp, mstrMyDbFilename);		// does a database exist there already?
	if (mbDatabaseOpen) CloseDatabase();							// close any database that might be open now

	if (!bSuccess) {			// no database where we are to initialize one
		if (mstrDefaultPath.GetLength() == 0) {
			strError.Format("Initialize Database: No Default Path Set program error, cannot continue.");
			/*NOT FOR NDAR
			MessageBox(NULL, strError, "Fatal Error", MB_OK|MB_ICONWARNING);
			*/
			return(false);
		}
		strEmptyDbPath = m_Dir.AppendPath(mstrDefaultPath, mstrComponentName);
		bSuccess = m_Dir.CopyDirectory(strEmptyDbPath, strTemp, true);	// copy an empty one over there
	}

	if (bSuccess) {		// database exists, open it and clear it
		bSuccess = OpenDatabase(strTemp, mstrDatabaseName);
		if (bSuccess) bSuccess = ClearDatabase(mstrDatabaseName, bPrompt);
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
//	bool CDbAccess::BackupDatabase(const CString &strDestinationBasePath, const bool bPrompt, const bool bOverwrite)
//
//	Input:	strDestinationBasePath	base path of where to copy the database to
//			bPrompt				boolean indicating whether to ask a question about overwriting data if one exists at strDestinationBasePath
//			bOverwrite			boolean indicating the answer to the question about overwriting data if the question is not asked.
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
//	03-Sep-2002	SFK		Used as is from DbVista.cpp
//////////////////////////////////////////////////////////////////
bool CDbAccess::BackupDatabase(const CString &strDestinationBasePath, const bool bPrompt, const bool bOverwrite)
{
	bool bOverwriteAnswer = bOverwrite;
	bool bSuccess = false;
	CString strError;
	
	if ((mstrComponentName.GetLength() == 0) || (mstrMyDbFilename.GetLength() == 0)) 
	{
		strError.Format("Backup Database: No component name or db filename set program error, cannot continue.");
		/*NOT FOR NDAR
		MessageBox(NULL, strError, "Fatal Error", MB_OK|MB_ICONWARNING);
		*/
		return(false);
	}

	CString strFullDestPath = m_Dir.AppendPath(strDestinationBasePath, mstrComponentName);	// create the Instrument Com specific path

	bool bExists = m_Dir.FileExists(strFullDestPath, mstrMyDbFilename);		// does a database exist there already?
	
	/* NOT FOR NDAR
	if (bExists && bPrompt) bOverwriteAnswer = Dir.OverwriteDbQuestion(strFullDestPath);		// database exists, see if they want to overwrite
	*/

	if (!bExists || (bExists && bOverwriteAnswer)) 
	{	// copy current database to the backup location
		CString strDbPath = mstrDatabasePath;
		if (mbDatabaseOpen) 
		{						// have to close the current database during the copy
			CString strDbName = mstrDatabaseName;
			CloseDatabase();
			bSuccess = m_Dir.CopyDirectory(strDbPath, strFullDestPath, true);	// copy current db to destination
			OpenDatabase(strDbPath, strDbName);
		}
		else 
		{	// database is closed, just copy
			bSuccess = m_Dir.CopyDirectory(strDbPath, strFullDestPath, true);	// copy current db to destination
		}
	}

	if (!bOverwriteAnswer) 
		bSuccess = true;	// chose not to overwrite

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
//	bool CDbAccess::RestoreDatabase(const CString &strSourceBasePath, const CString &strDestBasePath, const bool bPrompt, const bool bOverwrite)
//
//	Input:	strSourceBasePath	base path of from where to copy the database
//			bPrompt				boolean indicating whether to ask a question about overwriting data if one exists at strDestinationBasePath
//			bOverwrite			boolean indicating the answer to the question about overwriting data if the question is not asked.
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
//	03-Sep-2002	SFK		Used as is from DbVista.cpp
//////////////////////////////////////////////////////////////////
bool CDbAccess::RestoreDatabase(const CString &strSourceBasePath, const CString &strDestBasePath, const bool bPrompt, const bool bOverwrite)
{
	bool bOverwriteAnswer = bOverwrite;
	bool bSuccess = false;
	
	CString strSrcFullPath = m_Dir.AppendPath(strSourceBasePath, mstrComponentName);	// create the instrument Com specific path
	CString strDestFullPath = m_Dir.AppendPath(strDestBasePath, mstrComponentName);	// create the instrument Com specific path

	bool bExists = m_Dir.FileExists(strDestFullPath, mstrMyDbFilename);		// does a database exist there already?
	
	/* NOT FOR NDAR
	if (bExists && bPrompt) bOverwriteAnswer = Dir.OverwriteDbQuestion(strDestBasePath);	// database exists, see if they want to overwrite
	*/
	if (!bExists || (bExists && bOverwriteAnswer)) 
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


