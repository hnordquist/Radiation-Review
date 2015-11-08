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
