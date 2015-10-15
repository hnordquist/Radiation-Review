#ifndef _ZAPPAPP_INCLUDED
#define _ZAPPAPP_INCLUDED 1
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

#if defined(Z_IMPDLL) || defined(Z_IMPINTDLL) || defined(Z_USEDLL)

struct Z_DLLSPEC zAppGlobalStruct;

#endif

class zApp {
	int _cmdshow;
	char *_cmdline;
	zWindow* root;
	zTaskDev* taskdev;
	zTimerDev* tdev;
	zNotifyNodeDlist* nlist;

	long _behavior;
	int _defDlgFontWeight;

	ziWindowMap *_winMap;
	friend void Z_DLLVSPEC Z_DLLVBSPEC debug(char* s, ...);
	HINSTANCE hDefRes;
#ifdef STRICT
	HINSTANCE hInst;			
	HINSTANCE hPrevInst;
#else	
	HANDLE hInst;			
	HANDLE hPrevInst;
#endif
	RWCStringSlist *classNameList;
	HANDLE hAccel;
public:
	char * Z_DLLSPEC zAppDefResource;
	void Z_DLLSPEC go();
	void main();

	int execute(const char*commandLine);
		
	int defaultDlgFontWeight() const;
	void setDefaultDlgFontWeight(int =-1);
	
	int Z_DLLSPEC cmdShow() { return _cmdshow; }
	char * Z_DLLSPEC cmdLine() { return _cmdline; }
	Z_DLLSPEC ~zApp();	
	void Z_DLLSPEC setRootWindow(zFrameWin* w) { root=w; }
	zFrameWin* Z_DLLSPEC rootWindow() { return (zFrameWin*)root; }
	int MainExitVal;
	BOOL Z_DLLSPEC loadAccelerator(const zResId& r);

    int sendMail(const RWCString& message,const RWCString& file="");
    int sendMail(const RWCString& message,const RWTPtrSlist<RWCString>& fileList);
    int isMailAvail();

	zTaskDev*& Z_DLLSPEC tasks() { return taskdev; }
	void Z_DLLSPEC timers(zTimerDev* t) { tdev=t; }
	zTimerDev* Z_DLLSPEC timers() { return tdev; }
	zNotifyNodeDlist*& Z_DLLSPEC notifyDlist() { return nlist; }
	void Z_DLLSPEC setHandler(void *,NotifyProc,MESSAGE);
	void Z_DLLSPEC removeHandler(void *,NotifyProc,MESSAGE);
	Z_DLLVSPEC inline BOOL debugOn();
	Z_DLLVSPEC inline void debugOn(BOOL v);
	Z_DLLVSPEC routeDebugMessages(FILE *);
	void Z_DLLSPEC quit();

	void Z_DLLSPEC beep(int i =0) {
	 	MessageBeep(i);
	}
	void Z_DLLSPEC setBehavior(unsigned long flag) { 	_behavior |= flag; }
	void Z_DLLSPEC clearBehavior(unsigned long flag) {	_behavior &= ~flag;}
	int Z_DLLSPEC isBehavior(unsigned long flag) { return (_behavior & flag) ? 1 : 0; }
	void Z_DLLSPEC setDefaultResourceModule(const char *);


	ziWindowMap * Z_DLLSPEC winMap() {return _winMap;}
#ifdef __CTL3D__
	Z_DLLSPEC int ctl3denabled;
#endif
	Z_DLLSPEC zApp(HANDLE,HANDLE,char *,int);
	BOOL modalDialog;
	RWCStringSlist * Z_DLLSPEC winClassList() {return classNameList;}
	void Z_DLLSPEC registerUserClass(char *name,WNDCASTPROC stub,char *dll =0);
#ifdef STRICT
	HINSTANCE Z_DLLSPEC instance() const { return hInst; }
	HINSTANCE Z_DLLSPEC prevInstance() const { return hPrevInst; }
#else
	HANDLE Z_DLLSPEC instance() const { return hInst; }
	HANDLE Z_DLLSPEC prevInstance() const { return hPrevInst; }
#endif
#if defined(Z_IMPDLL) || defined(Z_IMPINTDLL) || defined(Z_USEDLL)
zAppGlobalStruct* globalVars;
#endif
private:
    zMail appMail;
};

#define ZCOMPATIBLE_DIALOGSIZING 0x00000001
#define ZCOMPATIBLE_FILEDLG  0x00000002
#define ZCOMPATIBLE_OLDOS2POPUPMENU  0x00000004
#define ZCOMPATIBLE_FONTSIZES  0x00000008
#define ZCOMPATIBLE_ZAPPVER22_DIALOGSIZING  0x00000010
#define ZRES_FROMEXECUTABLE ((char *)-1)
	
extern Z_DLLVSPEC zApp* Z_DLLVBSPEC app;

class Z_DLLSPEC zAppFrame:public zFrameWin {
public:
	zAppFrame(zWindow*,zSizer*,DWORD style=0,const char* =0);
	virtual void show(int=SW_SHOWNORMAL);
	virtual ~zAppFrame();
private:
	virtual WNDCLASS* registerHook();
};

#endif
