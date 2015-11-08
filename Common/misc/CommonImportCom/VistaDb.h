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
