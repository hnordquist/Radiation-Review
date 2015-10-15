//// stdafx.h : include file for standard system include files,
////      or project specific include files that are used frequently,
////      but are changed infrequently
//
//#if !defined(AFX_STDAFX_H__13AEC446_19C5_44E1_AD64_673AB807FEB1__INCLUDED_)
//#define AFX_STDAFX_H__13AEC446_19C5_44E1_AD64_673AB807FEB1__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#define STRICT
//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT 0x0501
//#endif
//#define _ATL_APARTMENT_THREADED
//
////#ifdef _DEBUG
////#define _ATL_DEBUG_INTERFACES  //New from Joe Longo
////#endif
//
//#include <afxwin.h>
//#include <afxdisp.h>
//
//#include <atlbase.h>
////You may derive a class from CComModule and use it if you want to override
////something, but do not change the name of _Module
//extern CComModule _Module;
//#include <atlcom.h>
//#include <afxtempl.h>		// 11-Jul-2005 SFK added for CList
//#include <afxdlgs.h>		// 19-Oct-2005 SFK added for File Dialog boxes
//
////{{AFX_INSERT_LOCATION}}
//// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//
//// sfk Add these lines for Access
//#include <comdef.h>
//
//#include "HPdefines.h"
//
//#pragma warning ( push)
//#pragma warning ( disable : 4146)
//
//#import <msadox.dll> \
//rename( "EOF", "adoxEOF" ) \
//rename_namespace( "ADOX" )
//
//
//#pragma warning(pop)
//
//#include <afxcmn.h>
//#include <afxcview.h>
//
//
//#endif // !defined(AFX_STDAFX_H__13AEC446_19C5_44E1_AD64_673AB807FEB1__INCLUDED)

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

#import <C:\Program Files\Common Files\System\ado\msadox.dll> \
rename( "EOF", "adoxEOF" ) \
rename_namespace( "ADOX" )