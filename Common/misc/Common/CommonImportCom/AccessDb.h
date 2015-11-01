// AccessDb.h: interface for the CDbAccess class.

#pragma once

#include "DirUtilities.h"
#include "AdoDb.h"

class CDbAccess  :	public CAdoDb
{
public:

	CDbAccess();
	virtual ~CDbAccess();
	bool BackupDatabase(const CString& strDestinationBasePath, const bool bPrompt, const bool bOverwrite);
	virtual bool ClearDatabase(const CString &strDbName, const bool bPrompt=true, CString *pstrErrorMsg=NULL);
	void CloseDatabase(bool bCompactDb = false);
	virtual bool CreateDataStore (short sFacilityID) = 0;		// 11-Jul-2005 SFK added
	bool GetDatabasePathAndName(CString *pstrPath, CString *pstrName, CString *pstrErrorMsg= NULL);
	bool InitializeDatabase(const CString &strDbBasePath, const bool bPrompt);
	bool OpenDatabase(const CString &strDbPath, const CString &strDbName, CString *pstrErrorMsg = NULL);
	bool RestoreDatabase(const CString &strSourceBasePath, const CString &strDestBasePath, const bool bPrompt, const bool bOverwrite);
	bool SameDatabase(const CString &strPath, const CString &strName, CString* pstrErrorMsg = NULL);
	bool SaveDatabaseNow();
	bool DatabaseExists (const CString& csDataStorePath,const CString& dbName);
	
	bool mbDatabaseOpen;
	bool mbDatabaseCleared;
	bool mbRequestToClear;
	bool mbAskClearDbQuestion;
	CString mstrDatabasePath;		// exact path on disk where db is found
	CString mstrDatabaseName;		// name of database
	CString mstrComponentName;		// the subpath where my database is found
	CString mstrMyDbFilename;		// presence of this file indicates the database is there
	CString mstrDefaultPath;		// the base path where any default information can be found under the MyIDName directory

	short msFacNum;
	CString mstrDatabaseRootPath;
	bool mbQuietMode;
	CDirUtilities m_Dir;
};
