// VistaDb.h: interface for the CDbVista class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "DirUtilities.h"
#include "vista.h"

class CDbVista  
{
public:

	DB_TASK m_Currtask;

	CString mstrDatabaseRootPath;

	CDbVista(bool bQuietMode);  //Need quiet mode for NDAR.
	virtual ~CDbVista();

	bool BackupDatabase(const CString& strDestinationBasePath, const bool bPrompt, const bool bOverwrite);
	virtual bool ClearDatabase(const CString& strDbName, bool bPrompt= true,  CString *pstrErrorMsg= NULL);
	virtual bool CreateDataStore (short sFacilityID) = 0;		// xx-Jun-2005 hn added
	virtual bool BuildDatabaseFramework(short sFacNum) = 0;
	virtual bool AddStationToDatabaseFramework(short sFacNum, short sStaID) = 0;
	void CloseDatabase();
	bool GetDatabasePathAndName(CString *pstrPath, CString *pstrName, CString *pstrErrorMsg= NULL);
	bool InitializeDatabase(const CString &strDbBasePath, const bool bPrompt);
	bool OpenDatabase(const CString &strDbPath, const CString &strDbName, CString *pstrErrorMsg = NULL);
	bool RestoreDatabase(const CString &strSourceBasePath, const CString &strDestBasePath, const bool bPrompt, const bool bOverwrite);
	bool SameDatabase(const CString &strPath, const CString &strName, CString* pstrErrorMsg = NULL);
	bool SaveDatabaseNow();
	void CheckDatabaseSize(double PercentMax, ULONG& CurDBSizeBytes, ULONG& MaxDBSizeBytes, bool& bTooBig);

	bool m_bQuietMode;  //added for NDAR
	CDirUtilities m_Dir;

	short msFacNum;
	bool mbDatabaseOpen;
	bool mbDatabaseCleared;
	bool mbRequestToClear;
	bool mbAskClearDbQuestion;
	CString mstrDatabasePath;		// exact path on disk where db is found
	CString mstrDatabaseName;		// name of database
	CString mstrComponentName;		// the subpath where my database is found
	CString mstrMyDbFilename;		// presence of this file indicates the database is there
	CString mstrDefaultPath;		// the base path where any default information can be found under the MyIDName directory	
};
