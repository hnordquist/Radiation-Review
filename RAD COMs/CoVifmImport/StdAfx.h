//// stdafx.h : include file for standard system include files,
////      or project specific include files that are used frequently,
////      but are changed infrequently
//
//#if !defined(AFX_STDAFX_H__0F0A1D6A_667B_11D6_819C_00C04F60E89B__INCLUDED_)
//#define AFX_STDAFX_H__0F0A1D6A_667B_11D6_819C_00C04F60E89B__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#define STRICT
//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT _WIN32_WINNT_WINXP
//#endif
//#define _ATL_APARTMENT_THREADED
//
//#include <afxwin.h>
//#include <afxdisp.h>
//
//#include <atlbase.h>
////You may derive a class from CComModule and use it if you want to override
////something, but do not change the name of _Module
//extern CComModule _Module;
//#include <atlcom.h>
//#include <afxtempl.h>		// 6-Jul-2005 SFK  Need for CList
//
//#endif // !defined(AFX_STDAFX_H__0F0A1D6A_667B_11D6_819C_00C04F60E89B__INCLUDED)


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