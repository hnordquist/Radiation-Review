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
// BinaryImportSupport.cpp: implementation of the CBinaryImportSupport class.
//
//////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////////
//	Copyright, 2005, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (N-1)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support directory information for DLL and
//	datastores
//
//	Functions:
//		GetGrandImportDLLLocation
//		GetGrandDataStorePath
//		GetEmptyDBLocation
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "BinaryImportSupport.h"
#include "FacCfgWrap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 11-Jul-2005 SFK	Adapted from GRAND stuff
CBinaryImportSupport::CBinaryImportSupport()
{
	//The following code was flagrantly stolen from Kelly Michel in her CoGPSImport
	//project.  The GUID for GrandImport was substituted so that we could determine the
	//path of the CoGrandImport.dll, thus able to locate the empty database contained there.
	//hn 6/6

	//Determine where you the COM server is located.  
	UINT cStrings = 0;
	LPTSTR pszClass = NULL;
	LPTSTR pszClsid;
	LPTSTR pszKey;

	//Begin loading the list from the contents of the registry.
	pszClass = (LPTSTR) new TCHAR[_MAX_PATH*3];
	if(pszClass == NULL)
	{
		MessageBox(NULL, "Failed to allocate memory for pszClass", "Failure", MB_OK);
		delete pszClass;
		return;
	}
	
	pszClsid = pszClass + _MAX_PATH;
	pszKey = pszClsid + _MAX_PATH;
	
	//Open the root key.
	LONG lRet;
	HKEY hKey;
		
	// 11-Jul-2005 SFK Make sure you get the CLSID for your component!
	lRet = ::RegOpenKey(HKEY_CLASSES_ROOT, "CLSID\\{C6E7683F-4C2B-4586-BBA5-DFC973644B18}", &hKey);
	if((LONG) ERROR_SUCCESS != lRet)
	{
		//20-Sep-2005 SFK  Must comment out the next line or get an error when trying to register!
		//MessageBox(NULL, "RegOpenKey failed", "Failure", MB_OK);
		delete pszClass;
		return;
	}
	
	
	//Get full user type name.
	DWORD dw = _MAX_PATH;
	lRet = RegQueryValue(hKey, "InprocServer32", pszKey, (LONG*) &dw);
	
	//Close out the key.
	if(hKey != NULL)
		RegCloseKey(hKey);
	
	//Parse the Key string to obtain the directory location of the CoGrandImport.DLL.
	CString Temp = pszKey;
	int StartAt = Temp.ReverseFind(_T('\\'));
	int Len = Temp.GetLength(); 
	Temp.Delete(StartAt+1, Len - (StartAt + 1));
	m_DLLLocationPath = Temp;
	m_DLLLocationPath.Delete(m_DLLLocationPath.GetLength() - 1);

	if(pszClass)
		delete pszClass;  //Free memory allocated.
	
}

CBinaryImportSupport::~CBinaryImportSupport()
{
	
}

CString CBinaryImportSupport::GetBinaryImportDLLLocation()
{
	//Return the location of the registered CoGrandImport COM
	return m_DLLLocationPath;
}

// 29-Sep-2005 SFK Changed FacWrap to not be a pointer (it wasn't
//					getting deleted)
CString CBinaryImportSupport::GetBinaryDataStorePath(short sFacID)
{
	// Using facility manager, this function returns the fully qualified
	// path to the spot where the grand database should be.
	CString path;
	CFacCfgWrap FacWrap;
	path = FacWrap.GetFacilityDirectory (sFacID);
	path += "\\Data\\Import\\Binary\\";
	return path;
}


CString CBinaryImportSupport::GetEmptyDBLocation()
{
	//This function returns the location where an empty database
	// should be loaded by the IRS install so that we can copy it to the
	// datastore path.
	return m_DLLLocationPath + "\\Import\\Binary\\";
}
