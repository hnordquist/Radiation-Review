// CoGrandImport.cpp : Implementation of DLL Exports.

//1-May-2006 pjm Pulled in SK's BL3 code.

// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f CoGrandImportps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "CoGrandImport.h"

#include "CoGrandImport_i.c"
#include "GrandImport.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_GrandImport, CGrandImport)
END_OBJECT_MAP()

class CCoGrandImportApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoGrandImportApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCoGrandImportApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CCoGrandImportApp, CWinApp)
	//{{AFX_MSG_MAP(CCoGrandImportApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CCoGrandImportApp theApp;

BOOL CCoGrandImportApp::InitInstance()
{
#ifdef _DEBUG
	Beep(500,50);
#endif
	_Module.Init(ObjectMap, m_hInstance, &LIBID_COGRANDIMPORTLib);
    return CWinApp::InitInstance();
}

int CCoGrandImportApp::ExitInstance()
{
	//int i=_Module.GetLockCount();
    _Module.Term();
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TRACE("CoGrandImport.cpp: DllCanUnloadNow\n");

    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


