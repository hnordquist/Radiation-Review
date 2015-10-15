  
// RadBuild.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

//19 Apr 2006 - pjm - changed code to use G: vice C:

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
//#include <afxcview.h>		// From ADO test project.
#include <afxdisp.h>        // MFC Automation classes
//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxctl.h>         // MFC support for OLE Custom Controls
#include <afxrich.h>

struct CCharFormat : public CHARFORMAT
{
	CCharFormat() {cbSize = sizeof(CHARFORMAT);}
	BOOL operator==(CCharFormat& cf);
};

struct CParaFormat : public _paraformat
{
	CParaFormat() {cbSize = sizeof(_paraformat);}
	BOOL operator==(PARAFORMAT& pf);
};

struct CHARHDR : public tagNMHDR
{
	CHARFORMAT cf;
	CHARHDR() {cf.cbSize = sizeof(CHARFORMAT);}
};

#define HORZ_TEXTOFFSET 15
#define VERT_TEXTOFFSET 5

#define RD_WINWORD2 0
#define RD_WINWORD6 1
#define RD_WORDPAD 2
#define RD_WRITE 3
#define RD_RICHTEXT 4
#define RD_TEXT 5
#define RD_OEMTEXT 6
#define RD_ALL 7
#define RD_EXE 8
#define RD_EMBEDDED 9
#define NUM_DOC_TYPES 10

#define RD_DEFAULT RD_WORDPAD
#define RD_NATIVE RD_RICHTEXT

#include <comdef.h>

#include "HPDefines.h"


#include <afxcview.h>
#include <afxtempl.h>
#include <afxcoll.h>
#include <atlbase.h>


