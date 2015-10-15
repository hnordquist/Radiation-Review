#ifndef _ZAPPMDI_INCLUDED
#define _ZAPPMDI_INCLUDED 1
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


#define MDI TRUE

class Z_DLLSPEC zMDIAppFrame;
class Z_DLLSPEC zMDIChildFrame;



class Z_DLLSPEC zMDIChildFrame:public zFrameWin {
	friend class zMDIAppFrame;
protected:
	void create();
	virtual WNDCLASS* registerHook();
	zMDIChildFrame(zFrameWin *);
public:
	zMDIChildFrame(zFrameWin*,zSizer*,DWORD style=0,const char* =0,int noAutoCreate =0);
	virtual long dispatch(zEvent*);
	virtual void show(int=SW_SHOWNORMAL);
	virtual int MDIActivate(zActivateEvt*);
	void maximize(BOOL =1);
	void next();
	void setActive();
	virtual ~zMDIChildFrame();

};

extern "C" {
	long APIENTRY MDIdialogProc(HWND, zInt, zInt, DWORD);
}

class Z_DLLSPEC zMDIDialogFrame:public zMDIChildFrame {
	void go();
protected:
	friend long APIENTRY MDIdialogProc(HWND, zInt, zInt, DWORD);
	ziDialogReader *dr;
	zResId menuRes;
	zFont *dlgFnt;
	HWND lastFocus;
	BOOL isModal;
	unsigned int defId;
	virtual WNDCLASS* registerHook();
	virtual void create();
public:
	zMDIDialogFrame(zFrameWin*,const zResId&,DWORD =0,int noAutoCreate=0);
	zMDIDialogFrame(zFrameWin*,zSizer*,DWORD style=0,const char* =0,int noAutoCreate=0);
	virtual ~zMDIDialogFrame();
	virtual void shutdown();
	
	zFont *getDlgFont() { return dlgFnt; }
	virtual int focus(zFocusEvt*);
	virtual int activate(zActivateEvt*);
	void show(int sw=SW_SHOWNORMAL);
	virtual long dispatch(zEvent*);
};

class Z_DLLSPEC zMDIFormDialog : public zMDIDialogFrame {
	unsigned long _flags;
	BOOL _completed;
public:
	zMDIFormDialog(zFrameWin*, const zResId &, DWORD = 0);
	~zMDIFormDialog();
	BOOL& completed() { return _completed; }
	int storeData();
	int setControlsToDefault();
	int endOk(zEvent *);
	int endCancel(zEvent *);
};

class Z_DLLSPEC zMDIMarginFrame:public zFrameWin {
protected:
	void create();
	void wcreate();
	virtual WNDCLASS* registerHook();
public:
	zMDIMarginFrame(zFrameWin*,zSizer*,DWORD style=0,const char* =0);
	virtual long dispatch(zEvent*);
	virtual void show(int=SW_SHOWNORMAL);
	virtual ~zMDIMarginFrame();
};

class Z_DLLSPEC zMDIAppFrame:public zFrameWin {
	friend class zMDIMarginFrame;
	friend class zMDIChildFrame;
protected:
	zMenu* listMenu;
	int firstChild;

	zFrameWin *mdicp;
	virtual WNDCLASS* registerHook();
public:
	zMDIAppFrame(zWindow*,zSizer*,DWORD style=0,const char * =0,zMenu* =0, int=1000);
	virtual void show(int=SW_SHOWNORMAL);
	virtual void create();
	virtual long dispatch(zEvent*);
	void tile();
	void cascade();
	void arrangeIcons();
	zMDIChildFrame* activeChild() const;
	zMenu* MDImenu() { return listMenu; };
	void MDImenu(zMenu* m) { listMenu=m; };
	void resetMenu();
	void setNumTrailingSubmenus(int);
	virtual ~zMDIAppFrame();
	zMDIChildFrame* activeC;
};


#endif

