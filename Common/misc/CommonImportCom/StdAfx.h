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