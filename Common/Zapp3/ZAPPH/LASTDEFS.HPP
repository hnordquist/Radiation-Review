#ifndef _ZAPPLASTDEFS_INCLUDED
#define _ZAPPLASTDEFS_INCLUDED 1
/*
 * (c) Copyright 1991 - 1996 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013.
 * Contractor/Manufacturer is Rogue Wave Software, Inc.,
 * P.O. Box 2328, Corvallis, Oregon 97339.
 *
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 **************************************************************************/


#if defined(Z_USEDLL) || defined(Z_IMPDLL) || defined(Z_IMPINTDLL)

#include <zapph/ddeclass.hpp>

extern "C" {
	extern zApp * PASCAL zAppInitApp(HINSTANCE,HINSTANCE ,LPSTR,int);
	extern int PASCAL zAppTerminateApp();
}

class Z_DLLSPEC TypeInfo;
class Z_DLLSPEC zAppDllInstanceMem {
public:
	zApp *app;
	int dummy1;
	TypeInfo *first;
	int dummy2;
	int ids;
	int dummy3;
#ifndef Z_NP
	TypeInfo* appPersistList;
	int dummy4;
#endif
};

struct Z_DLLSPEC zAppGlobalStruct {
	zMenu* __anyPopupMenu;
	ziPenAspectDList* penalist;
	ziBrushAspectDList* brushalist;
	ziFontAspectDList* fontalist;
	zControlTypeDlist* zctd;
	void* zAppCtlData;
	zIntlContext* zAppDefaultIntlContext;
	int zAppPaneRegistered;
	TypeInfo* first;
	int ids;
	zPrintJob* curPrJob;
	FARPROC prAbortProc;
	zLayout* defLayout;
	zWindow* ziLastChildFocus;
	zApp* app;
	COLORREF zAppCustColors[16];
	int zAppColorsInited;
	RWTPtrDlist<zAdviseTrans>* tlist;
	zEvH* errorObj;
	zDdeErrorProc errorProc;
	RWTPtrDlist<zTopicServer>* cslist;
	unsigned long _servInst;
	zDialogFrameStack* dfs;
	HWND hMDIclient;
	int __MDItrailing;
	int __curCheckedMDI;
	PRINTDLG *zpdlg;
	unsigned long debugFlags;
	HWND activeDialog; 
	int __scrPixHeight;
	int __scrPixWidth;
	int __scrPixPerInchX;
	int __scrPixPerInchY;
	int __scrMmHeight;
	int __scrMmWidth;
	int _debugOn;
	FILE* zAppDebugFile;
	zSuperClassSlist* superclassed;
	LINEPROC ddaProc;
	LINEPROC zAppDdaProc;
	HBRUSH _zAppGlobalBrush;
	int _debugInited; 
	char devTopic[20];
	char assertTypes[3][20]; 
	int childWinRegistered;
	int zChildFrameRegistered;
	int saveOldFocus;
	HWND curWnd;
	int dialogRegistered;
	int mdiMarginRegistered;
	int sem;
	zAppGlobalStruct(zApp* theApp);		
};

	
#include <dos.h>	
#ifdef enable
#undef enable
#endif
#ifdef disable
#undef disable
#endif
	
inline zAppDllInstanceMem * Z_DLLSPEC zAppAppMem() {
#if defined(Z_WIN) && !defined(WIN32) && __BORLANDC__ == 0x452
        return ((zAppDllInstanceMem*) MK_FP(_SS,0x38)); //for bc452
#else
 	return ((zAppDllInstanceMem*) MK_FP(_SS,0x24));
#endif
}
	
#endif


#if defined(Z_IMPDLL) || defined(Z_USEDLL)

#define zAppGetAppVar(v)	(zAppAppMem()->app->globalVars->v)
#else
#define zAppGetAppVar(v)	(v)
#endif

#define zAppGetStaticVar(v)	(zAppAppMem()->v)

Z_DLLVSPEC inline BOOL zApp::debugOn() {
#if defined(Z_IMPDLL) || defined(Z_USEDLL)
		return zAppGetAppVar(_debugOn);
#else
		return _debugOn;
#endif		
	};


#endif
