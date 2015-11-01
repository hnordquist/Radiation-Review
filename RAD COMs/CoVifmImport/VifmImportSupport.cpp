// GrandImportSupport.cpp: implementation of the CGrandImportSupport class.
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
//	datastores.
//
//	Functions:
//		GetGrandImportDLLLocation
//		GetGrandDataStorePath
//		GetEmptyDBLocation
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "VifmImportSupport.h"
#include "FacCfgWrap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 06-Jun-2005 hn	The following code was flagrantly stolen from Kelly Michel in her CoGPSImport
//					project.  The GUID for GrandImport was substituted so that we could determine the
//					path of the CoGrandImport.dll, thus able to locate the empty database contained there.
// 06-Jul-2005 SFK	Copied from GrandImportSupport.cpp and modified for VIFM
CVifmImportSupport::CVifmImportSupport()
{

	//Determine where your the COM server is located.  
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
		
	lRet = ::RegOpenKey(HKEY_CLASSES_ROOT, "CLSID\\{0F0A1D74-667B-11D6-819C-00C04F60E89B}", &hKey);
	if((LONG) ERROR_SUCCESS != lRet)
	{
		//20-Sep-2005 SFK  Must comment out the next line or get an error when trying to register!
		//MessageBox(NULL, "CoVifmImport RegOpenKey failed", "Failure", MB_OK);
		delete pszClass;
		return;
	}
	
	
	//Get full user type name.
	DWORD dw = _MAX_PATH;
	lRet = RegQueryValue(hKey, "InprocServer32", pszKey, (LONG*) &dw);
	
	//Close out the key.
	if(hKey != NULL)
		RegCloseKey(hKey);
	
	//Parse the Key string to obtain the directory location of the CoVifmImport.DLL.
	CString Temp = pszKey;
	int StartAt = Temp.ReverseFind(_T('\\'));
	int Len = Temp.GetLength(); 
	Temp.Delete(StartAt+1, Len - (StartAt + 1));
	m_DLLLocationPath = Temp;
	m_DLLLocationPath.Delete(m_DLLLocationPath.GetLength() - 1);

	if(pszClass)
		delete pszClass;  //Free memory allocated.
	
}

CVifmImportSupport::~CVifmImportSupport()
{
	
}

CString CVifmImportSupport::GetVifmImportDLLLocation()
{
	//Return the location of the registered CoVifmImport COM
	return m_DLLLocationPath;
}


// 29-Sep-2005 SFK Changed FacWrap to not be a pointer (it wasn't
//					getting deleted)
CString CVifmImportSupport::GetVifmDataStorePath(short sFacID)
{
	// Using facility manager, this function returns the fully qualified
	// path to the spot where the grand database should be.
	CString path;
	CFacCfgWrap FacWrap;
	path = FacWrap.GetFacilityDirectory (sFacID);
	path += "\\Data\\Import\\Vifm\\";
	return path;
}


CString CVifmImportSupport::GetEmptyDBLocation()
{
	//This function returns the location where an empty database
	// should be loaded by the IRS install so that we can copy it to the
	// datastore path.
	return m_DLLLocationPath + "\\Import\\Vifm\\";
}
