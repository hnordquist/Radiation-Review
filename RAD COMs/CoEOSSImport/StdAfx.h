#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
//#include <afxodlgs.h>       // MFC OLE dialog classes
//#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#import <C:\Program Files (x86)\Common Files\System\ado\msado15.dll> \
rename( "EOF", "adoEOF" ) \
rename_namespace( "ADODB" )

using namespace ADODB;

#import <C:\Program Files (x86)\Common Files\System\ado\msadox.dll> \
rename( "EOF", "adoxEOF" ) \
rename_namespace( "ADOX" )

#import <C:\Program Files (x86)\Common Files\System\ado\msjro.dll> \
rename( "EOF", "jroEOF" ) \
rename_namespace( "JRO" )