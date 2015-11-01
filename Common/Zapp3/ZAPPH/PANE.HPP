#ifndef _ZAPPPANE_INCLUDED
#define _ZAPPPANE_INCLUDED 1
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


class Z_DLLSPEC zPrinterDisplay;
class Z_DLLSPEC zPrintJob;

class Z_DLLSPEC zPane:public zChildWin {
	zDisplay* curDisp;
	zDisplay* defDisp;
	zDisplayStack displays;
	zPane *_prevObject;
	friend Z_DLLSPEC doPaneDraw(zPane*, zEvent *);
protected:
	friend class zCaret;
	zCaret* _caret;
	virtual WNDCLASS* registerHook();
	zPane* findPrevObject(int ctrlId);
	zPane(zWindow*,int ctrlId);
public:
	zPane(zWindow*,zSizer*,unsigned long sty=0,int ctrlId=0,int autoCreate=1);

	virtual ~zPane();
	virtual	int draw(zDrawEvt*de=0);
	virtual int scroll(zEvent*);
	zDisplay* canvas() { return curDisp; }
	void pushDisplay(zDisplay *pd) {
		curDisp = pd;
		displays.push(pd);
		setMetrics();
	}
	zDisplay* popDisplay() {
		displays.pop();
		curDisp = displays.top();
		setMetrics();
		return curDisp;	
	}
	
	virtual void setMetrics();
	virtual int focus(zFocusEvt*);
	virtual int printSetup(zPrinterDisplay *pr, zPrintJob *pj);
	virtual void printCleanup(zPrinterDisplay *pr, zPrintJob *pj);
	virtual int print(zPrinterDisplay *pr, zRect *r);
	virtual BOOL morePages();
	virtual void show(int=SW_SHOWNORMAL);
	virtual void scrollPane(int x, int y, const zRect &scR);
	zDisplay** currentDisplay() { return &curDisp; }
	zColor textColor() const;
	zColor textColor(const zColor&);
	zPane(HWND hPaneWnd,zWindow*,zSizer*,DWORD style=0);
	virtual void ziBackgroundColor(const zColor& d);
};

class Z_DLLSPEC zPaneControl:public zPane {
protected:
	void sendNotification(unsigned int);	
	void postNotification(unsigned int);	

public:
	zPaneControl(zWindow*,int ctrlId_);
	zPaneControl(zWindow*,zSizer*,DWORD style_=0,const char* =0, int ctrlId=0);
	virtual ~zPaneControl();
	virtual long dispatch(zEvent*);

	virtual void onEnableDisable(int tf); // True=enable, false=disable
};


class Z_DLLSPEC zBitmapPane : public zPaneControl {
	zBitmap *theBitmap;
	int centered;
protected:
	virtual int draw(zDrawEvt *e);
public:
	zBitmapPane(zWindow* w, const zResId& id, zSizer* sz);
	zBitmapPane(zWindow* w, zBitmap*, zSizer* sz);
	zBitmapPane(zWindow*,zSizer*,unsigned long  style_=0,const char* =0,int ctrlId=0);
	zBitmapPane(zWindow*,int ctrlId_);
	~zBitmapPane();

	zBitmap* bitmap() const;
	zBitmap* setBitmap(zBitmap*,int resizeToFit=0, int centerBitmap=0);

	int size(zSizeEvt*);
};

inline zColor zPane::textColor() const { return curDisp->textColor(); }
inline zColor zPane::textColor(const zColor&c) { return curDisp->textColor(c); }

#endif

