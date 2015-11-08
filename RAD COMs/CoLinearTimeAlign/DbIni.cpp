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
// DbIni.cpp: implementation of the CDbIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "DbIni.h"
#include "DirUtilities.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDbIni::CDbIni()
{
	m_strComponentName.Empty();
	m_strDefaultPath.Empty();
	m_strMyIniFilename.Empty();
	m_strDataStorePath.Empty();
	m_bOpen = false;

}

CDbIni::~CDbIni()
{

}


///////////////////////////////////////////////////////////////////////////
//	Name:	CopyDataStore
//
//	Description:
//	Copy all the files in the specified source directory to the specified
//	destination directory.  Based on bQuestion ask a question about overwriting
//	any existing files, else just copy the files whether or not something
//	already exists.
//	
//	Declaration:
//	bool CDbIni::CopyDataStore(CString &strSourceRootPath, CString &strDestinationRootPath, bool bQuestion)
//
//	Input:	strSourceRootPath		base path of where to get the datastore to copy from
//			strDestinationRootPath	base path of where to copy the datastore to
//			bQuestion				whether or not to ask a question if the datastore exists at the destination
//			m_strComponentName		name of the subpath where this instrument's datastore is kept
//			m_strMyIniFilename		name of file that indicates a datastore is present
//
//	Output:	none
//
//	Return:	true (db exists and initialized) / false (error in operation)
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//////////////////////////////////////////////////////////////////
bool CDbIni::CopyDataStore(CString &strSourceRootPath, CString &strDestinationRootPath, bool bQuestion)
{
	CDirUtilities Dir(false);  //QUIET MODE IS ALWAYS OFF FOR TIMEALIGN pjm 11/27/2007 for B2R1
	bool bOverwrite;
	bool bSuccess = false;
	CString strError;
	
	if ((m_strComponentName.GetLength() == 0) || (m_strMyIniFilename.GetLength() == 0)) {
		strError.Format("BackupDataStore: No component name or ini filename program error, cannot continue.");
		MessageBox(NULL, strError, "Fatal Error", MB_OK|MB_ICONWARNING);
		return(false);
	}

	CString strFullDestinationPath = Dir.AppendPath(strDestinationRootPath, m_strComponentName);	// create the Instrument Com specific path
	CString strFullSourcePath = Dir.AppendPath(strSourceRootPath, m_strComponentName);	// create the Instrument Com specific path

	bool bExists = Dir.FileExists(strFullDestinationPath, m_strMyIniFilename);	// does a datastore exist there already?

	if (bExists && bQuestion) {
		bOverwrite = Dir.OverwriteDbQuestion(strFullDestinationPath);		// datastore exists, see if they want to overwrite
	}
	else bOverwrite = true;

	if ((!bExists) || (bExists && bOverwrite)) {	// copy current datastore to the specified destination
		bExists = Dir.FileExists(strFullSourcePath, m_strMyIniFilename);	// is a file even there to copy?
		if (bExists) bSuccess = Dir.CopyDirectory(strFullSourcePath, strFullDestinationPath, true);
		else bSuccess = true;
	}
	return(bSuccess);
}
