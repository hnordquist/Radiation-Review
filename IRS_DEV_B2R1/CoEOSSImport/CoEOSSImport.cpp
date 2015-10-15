// CoEOSSImport.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f CoEOSSImportps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "CoEOSSImport.h"

#include "CoEOSSImport_i.c"
#include "EOSSImport.h"
#include "imports.h"


CComModule _Module;
BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_EOSSImport, CEOSSImport)
END_OBJECT_MAP()

class CCoEOSSImportApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoEOSSImportApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCoEOSSImportApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CCoEOSSImportApp, CWinApp)
	//{{AFX_MSG_MAP(CCoEOSSImportApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CCoEOSSImportApp theApp;
BOOL CCoEOSSImportApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_COEOSSIMPORTLib);
	//sfk 04/21/2003AfxOleInit();		//sk 04/15/2003
//sk 04/21/2003	pDb = new CEOSSDbAccess;	// sk 04/15/2003
	int i=_Module.GetLockCount();
	TRACE1("CoEOSSImport InitInstance LockCount=%d\n", i);
    return CWinApp::InitInstance();
}

int CCoEOSSImportApp::ExitInstance()
{
//sk 04/21/2003	if (pDb) delete pDb;	// sk 04/15/2003
	int i=_Module.GetLockCount();
	TRACE1("CoEOSSImport ExitInstance LockCount=%d\n", i);
    _Module.Term();
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int i=_Module.GetLockCount();
	TRACE1("CoEOSSImport DllCanUnloadNow LockCount=%d\n", i);
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


