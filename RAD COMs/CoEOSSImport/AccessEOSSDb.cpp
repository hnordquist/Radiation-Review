// AccessEOSSDb.cpp: implementation of the CEOSSDbAccess class.
//

#include "stdafx.h"
#include "AccessEOSSDb.h"
#include "commondefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CEOSSDbAccess::CEOSSDbAccess()
{
	mstrComponentName = "Data\\Import\\EOSS";	
	mstrDatabaseName = "EOSSImport.mdb";		
	mstrMyDbFilename = "EOSSImport.mdb";
}


CEOSSDbAccess::~CEOSSDbAccess()
{
}

void CEOSSDbAccess::SetQuietMode(bool bQuietMode)
{
	mbQuietMode = bQuietMode;
}

bool CEOSSDbAccess::ClearDatabase(const CString &strDbName, bool bPrompt, CString *pstrErrorMsg)
{
	// Shouldn't this actually do something?
	return(true);
}


bool CEOSSDbAccess::OpenTheDatabase(short sFacNum, CString* pstrErrorMsg,const CString& csFacDir)
{
	CString strFacilityDirectory = csFacDir;// ((BSTR) g_pIFacMgr->GetFacilityDirectory(sFacNum));
	mstrDatabaseRootPath = csFacDir;/*strFacilityDirectory;*/
	strFacilityDirectory = m_Dir.AppendPath(strFacilityDirectory, mstrComponentName);
	msFacNum = sFacNum;

	BOOL bStatus = false;
	CString strErrorMsg;
	
	//If the datastore doesn't exist, create it and try again. 
	if (!m_Dir.FileExists (strFacilityDirectory, mstrDatabaseName)) {
		//Create the data store if it doesn't exist.
		if (!CreateDataStore (sFacNum))
		{
			//We still didn't create the datastore.
			bStatus = false;
			strErrorMsg.Format("Database could not be created at %s.", strFacilityDirectory);
			MessageBox(NULL, strErrorMsg, "Database Creation Error", MB_OK|MB_ICONWARNING);
			pstrErrorMsg->Format("%s", strErrorMsg);
		}
		else
		{
			bStatus = true;
		}
	}

	return(OpenDatabase(strFacilityDirectory, mstrDatabaseName));
	return true;
}

bool CEOSSDbAccess::CreateDataStore(short sFacilityID)
{
//	CEOSSImportSupport ImportSupport(m_pIFacMgr, mbQuietMode);

	//Determine the empty data store and destination path for the facility's data store
	//location, then copy the contents of the empty db to the place it should be.
	// The result is whether or not this copy was successful. hn 6/8
	CString emptyDBPath, dataStorePath;
//	emptyDBPath = ImportSupport.GetEmptyDBLocation();
//	dataStorePath = ImportSupport.GetEOSSDataStorePath (sFacilityID);
	
	//Copy the empty db, return the success of the copy operation.
	return (m_Dir.CopyDirectory (emptyDBPath,dataStorePath,TRUE));

}

bool CEOSSDbAccess::IsFacilityDBOpen(short sFacilityID,const CString& csFacDir)
{
	// Checks to see if the database for the specified facility is the one currently open
	CString currentPath, currentName;
	GetDatabasePathAndName (&currentPath,&currentName);
	
	CString strFacilityDirectory  = csFacDir;//((BSTR) g_pIFacMgr->GetFacilityDirectory(sFacilityID));
	
	
	mstrDatabaseRootPath = csFacDir;
	strFacilityDirectory = m_Dir.AppendPath(strFacilityDirectory, mstrComponentName);

	return (currentPath == strFacilityDirectory && currentName == EOSS_DB_NAME);

}
