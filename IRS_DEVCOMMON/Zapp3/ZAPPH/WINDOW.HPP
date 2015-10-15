#ifndef _ZAPPWINDOW_INCLUDED
#define _ZAPPWINDOW_INCLUDED 1
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


class Z_DLLSPEC zWindow:public zEvH {
protected:
static zWindow* ziLastChildFocus;

	unsigned short delFlags;
	zFont *_font;
	RWCString WndText;
	int WndId;
	unsigned long map;
	zSizer* sizeinfo;
	unsigned long style;
	unsigned long exStyle_;
	RWCString WndName;
	zCursor csr;
	unsigned long WndBackColor;
	WORD cStyle;
	zScrollInfoVert* sVert;
	zScrollInfoHoriz* sHoriz;
	zNotifyNodeDlist* nlist;
	zNotifyNodeDlist* ctrlNotify;
	zWindowDlist kids;
	zWindow* WndParent;
	zWindow(zSizer*,unsigned long style=0);
	zWindow(zWindow*);
	zWindow(zWindow*,zSizer*,unsigned long style=0);
	zWindow();
	zScrollerBase *scrl;	
	void setupNotification(zEvH *ob,NotifyProc n,MESSAGE notifyMSG);
	void removeNotification(zEvH *ob,NotifyProc n,MESSAGE notifyMSG);
	virtual WNDCLASS* registerHook();
	HWND hWnd;
	friend class zSizer;
public:
	virtual ~zWindow();

	operator HWND()	const { return hWnd; }
	HWND& sysId();

	zCursor& cursor();
	operator zCursor();
	void setCursor(const zCursor&);
	BOOL removeKid(zWindow *);
	zScrollInfoVert* scrollBarVert() const;
	zScrollInfoHoriz* scrollBarHoriz() const;
	virtual void resolveSize(zSizeEvt*);
	zWindowDlist& kidslist();
	zSizer* sizer();
	virtual	int activate(zActivateEvt *);
	virtual	int size(zSizeEvt *);
	virtual	int move(zMoveEvt *);
	virtual	int kill(zEvent*);
	virtual int command(zCommandEvt*);
	virtual int scroll(zEvent *);
	virtual int mouseMove(zMouseMoveEvt*);
	virtual int mouseButtonUp(zMouseClickEvt*);
	virtual int mouseButtonDown(zMouseClickEvt*);
	virtual int focus(zFocusEvt*);
	virtual int background(zEvent*);
	virtual long dispatch(zEvent *);
	void setNotifyActivationChange(zEvH*,NotifyProc);
	void setNotifyKill(zEvH*,NotifyProc);
	void setNotifyMouseMove(zEvH*,NotifyProc);
	void setNotifyMouseButtonUp(zEvH*,NotifyProc);
	void setNotifyMouseButtonDown(zEvH*,NotifyProc);
	void setNotifyFocusChange(zEvH*,NotifyProc);

	void removeNotifyActivationChange(zEvH*,NotifyProc);
	void removeNotifyKill(zEvH*,NotifyProc);
	void removeNotifyMouseMove(zEvH*,NotifyProc);
	void removeNotifyMouseButtonUp(zEvH*,NotifyProc);
	void removeNotifyMouseButtonDown(zEvH*,NotifyProc);
	void removeNotifyFocusChange(zEvH*,NotifyProc);

	zNotifyNodeDlist*& notifyDlist() { return nlist; }
	zNotifyNodeDlist*& ctrlNotifyDlist() { return ctrlNotify; }
	void setHandler(void *,NotifyProc,MESSAGE);
	void styleSet(unsigned long d)	{ style|=d; }
	void styleClear(unsigned long d) { style &= ~d; }

	void styleUpdate();
	unsigned long styleValue() const { return style; }
	void addVertScrollBar();
	void addHorzScrollBar();
	void getInterior(zRect &r);
	void classStyle(unsigned int d)	{ cStyle|=(WORD)d; }
	void classStyleClear(unsigned int d) { cStyle=(WORD)(cStyle&~d); }
	unsigned int classStyle() const { return cStyle; }
	void classStyleUpdate();
	zWindow* parent() const { return WndParent; }
	virtual void showSet(int);
	void setFocus();
	zWindow* prevWin();
	zWindow* nextWin();
	unsigned long& eventMap() { return map; }
	RWCString &wndtext() { return WndText; }
	RWCString &wndname() { return WndName; }
	virtual BOOL getPreferredSize(zDimension &);
	void removeHandler(void *,NotifyProc,MESSAGE);
	void mapToWindow(zWindow*w,zRect&r);
	void backgroundColor(const zColor& d);
	unsigned long backgroundColor();
	virtual void ziBackgroundColor(const zColor& d);

	BOOL deleteOnClose(BOOL);
	BOOL deleteOnClose() const { return (BOOL)(delFlags & zWINDOW_AUTODEL); }
	void setDestroyed() { delFlags |= zWINDOW_HWND_DEL; }
	BOOL isDestroyed() const { return delFlags & zWINDOW_HWND_DEL; }
	zScrollerBase* scroller() const { return scrl;}
	void scroller(zScrollerBase *s) { scrl = s;}	
	virtual void show(int=SW_SHOWNORMAL);
	void update();
	int isShown() const { return delFlags & zWINDOW_SHOWN; }
	zChildWin* get(int id);
	int ctrlId() const { return WndId; }
	void childId(int i) { WndId=i; }
	int childId() const { return WndId; }

	void enable();
	void disable();

	int isEnabled() const;

	virtual int checkData();
	virtual int storeData();
	virtual int invalidData(zWindow*);

	virtual void childFocusChange(zWindow *lost,zWindow *found);
	zFont* getFont() const { return _font; }
	virtual void setFont(zFont*, int redrawNow =0);

	void populateFromRes(const zResId&);
	void centerWindow(BOOL onParent =TRUE);
	void setDirty(zRect* r=0,BOOL eraseBackground =1);
	void setDirty(zRegion& r,BOOL eraseBackground =1);
	void getExterior(zRect &r);
#ifdef WIN32
	void styleExSet(unsigned long esm)   { exStyle_ |= esm; }
	void styleExClear(unsigned long esm) { exStyle_ &= ~esm; }
	void styleExUpdate();
	unsigned long styleExValue() const   { return exStyle_; }
#endif

	void move(int x, int y, int w, int h);
	void move(const zRect &r);
	void move(const zRect *r);
	virtual void vmove(int x, int y, int w, int h);

	zWindowLink* getNode(int id);

	void ziChildFocusHelper(zFocusEvt*);

	void captureMouse();
	void releaseMouse();
	void clipCursor(zRect* =0);
};

#endif
