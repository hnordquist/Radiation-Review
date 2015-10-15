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

	
inline unsigned long zWindow::backgroundColor() { 
	return WndBackColor; 
}

class Z_DLLSPEC ziAspect {
protected:
	int _type;
public:
	ziAspect() { _type=0; }
	virtual void setup(zDisplay *);
	int operator==(ziAspect&);
	int& type() { return _type; }
	virtual ziAspect* resolve();
};

class Z_DLLSPEC ziPenAspect;
class Z_DLLSPEC ziPenAspectDList;

declDList(ziAspect);

class Z_DLLSPEC ziPenAspect:public ziAspect {
	zPen *pen;
	zColor clr;
	int _width;
	static ziPenAspectDList *penalist;
public:
	ziPenAspect(zColor,int wid= 0);
	virtual void setup(zDisplay *);
	int operator==(const ziPenAspect&);
	int operator==(ziAspect&);
	virtual ziAspect* resolve();
	zColor color() { return clr; }
	static void cleanup();
};

declDList(ziPenAspect);

class Z_DLLSPEC ziBrushAspect;
class Z_DLLSPEC ziBrushAspectDList;

class Z_DLLSPEC ziBrushAspect:public ziAspect {
	zBrush *brush;
	zColor clr;
	static ziBrushAspectDList *brushalist;
public:
	ziBrushAspect(zColor);
	virtual void setup(zDisplay *);
	int operator==(const ziBrushAspect&);
	int operator==(ziAspect&);
	virtual ziAspect* resolve();
	static void cleanup();
};

declDList(ziBrushAspect);

class Z_DLLSPEC ziFontAspect;
class Z_DLLSPEC ziFontAspectDList;

class Z_DLLSPEC ziFontAspect:public ziAspect {
	zFont *fnt;
	static ziFontAspectDList *fontalist;
public:
	ziFontAspect(zFont*);
	virtual void setup(zDisplay *);
	int operator==(const ziFontAspect&);
	int operator==(ziAspect&);
	virtual ziAspect* resolve();
	static void cleanup();
};

declDList(ziFontAspect);

class Z_DLLSPEC ziTextBackcolorAspect:public ziAspect {
	zColor back;
public:
	ziTextBackcolorAspect(zColor);
	virtual void setup(zDisplay*);
};
	
	
inline zRange zScrollInfo::limits() {
	return range;
}

inline int zScrollInfo::pos() const {
	return at;
}

inline int zScrollInfo::oldpos() const {
	return oldat;
}

inline int zScrollInfo::pageDelta() const {
	return pDelta;
}

inline void zScrollInfo::pageDelta(int i) {
    pDelta=i;
}

inline int zScrollInfo::moveDelta() const {
	return mDelta;
}

inline void zScrollInfo::moveDelta(int i) {
	mDelta=i;
}

inline HCURSOR& zCursor::sysId() {
	return hc;
}

inline HCURSOR zCursor::sysId() const {
	return hc;
}

inline zCursor::operator HCURSOR() const {
	return hc;
}

#ifdef STRICT
inline HWND& zWindow::sysId() {
	return hWnd;
}
#else
inline HANDLE& zWindow::sysId() {
	return (HANDLE&)hWnd;
}
#endif



inline void zWindow::update() {
	UpdateWindow(hWnd);
}

inline zCursor& zWindow::cursor() {
	return csr;
}

inline void zWindow::backgroundColor(const zColor& d) {
    ziBackgroundColor(d);
}

inline zScrollInfoVert* zWindow::scrollBarVert() const {
	return sVert;
}

inline zScrollInfoHoriz* zWindow::scrollBarHoriz() const {
	return sHoriz;
}

inline zWindowDlist& zWindow::kidslist() {
	return kids;
}

inline zSizer* zWindow::sizer() {
	return sizeinfo;
}

inline void zWindow::move(const zRect &r) {
    vmove(r.left(), r.top(), r.width(), r.height());
}

inline void zWindow::move(const zRect *r) {
    vmove(r->left(), r->top(), r->width(), r->height());
}

inline void zWindow::move(int x,int y,int w,int h) {
    vmove(x,y,w,h);
}


inline void zWindow::addVertScrollBar() {
	styleSet(WS_VSCROLL);
	sVert=ZNEWH zScrollInfoVert(this);
}

inline void zWindow::addHorzScrollBar() {
	styleSet(WS_HSCROLL);
	sHoriz=ZNEWH zScrollInfoHoriz(this);
}

inline void zWindow::setFocus() { 
		if (IsWindow(hWnd)) SetFocus(hWnd); 
}

inline void zWindow::getInterior(zRect &r) {
	if (IsWindow(hWnd)) GetClientRect(hWnd,(LPRECT)&r);
}


inline void zWindow::getExterior(zRect &r) {
	if (IsWindow(hWnd)) GetWindowRect(hWnd,(LPRECT)&r);
}


inline void zWindow::setNotifyActivationChange(zEvH* ob,NotifyProc n) {
	setupNotification(ob,n,WM_ACTIVATE);
}

inline void zWindow::removeNotifyActivationChange(zEvH* ob,NotifyProc n) {
	removeNotification(ob,n,WM_ACTIVATE);
}

inline void zWindow::setNotifyKill(zEvH* ob,NotifyProc n) {
	setupNotification(ob,n,WM_CLOSE);
}

inline void zWindow::removeNotifyKill(zEvH* ob,NotifyProc n) {
	removeNotification(ob,n,WM_CLOSE);
}

inline void zWindow::setNotifyMouseMove(zEvH* ob,NotifyProc n) {
	setupNotification(ob,n,WM_MOUSEMOVE);
}

inline void zWindow::removeNotifyMouseMove(zEvH* ob,NotifyProc n) {
	removeNotification(ob,n,WM_MOUSEMOVE);
}

inline void zWindow::setNotifyMouseButtonUp(zEvH* ob,NotifyProc n) {
	setupNotification(ob,n,WM_LBUTTONUP);
	setupNotification(ob,n,WM_RBUTTONUP);
}

inline void zWindow::removeNotifyMouseButtonUp(zEvH* ob,NotifyProc n) {
	removeNotification(ob,n,WM_LBUTTONUP);
	removeNotification(ob,n,WM_RBUTTONUP);
}

inline void zWindow::setNotifyMouseButtonDown(zEvH* ob,NotifyProc n) {
	setupNotification(ob,n,WM_LBUTTONDOWN);
	setupNotification(ob,n,WM_RBUTTONDOWN);
}

inline void zWindow::removeNotifyMouseButtonDown(zEvH* ob,NotifyProc n) {
	removeNotification(ob,n,WM_LBUTTONDOWN);
	removeNotification(ob,n,WM_RBUTTONDOWN);
}

inline void zWindow::setNotifyFocusChange(zEvH* ob,NotifyProc n) {
	setupNotification(ob,n,WM_SETFOCUS);
	setupNotification(ob,n,WM_KILLFOCUS);
}

inline void zWindow::removeNotifyFocusChange(zEvH* ob,NotifyProc n) {
	removeNotification(ob,n,WM_SETFOCUS);
	removeNotification(ob,n,WM_KILLFOCUS);
}

inline void zWindow::setDirty(zRect* r,BOOL eraseBackground) {
	InvalidateRect(hWnd, (LPRECT)r, eraseBackground);
}


inline void zWindow::setDirty(zRegion& r,BOOL eraseBackground) {
#ifdef STRICT
	InvalidateRgn(hWnd, (HRGN)r, eraseBackground);
#else	
	InvalidateRgn(hWnd, (HANDLE)r, eraseBackground);
#endif
}

inline BOOL zWindow::deleteOnClose(BOOL b) {
	if (b) 
		delFlags |= zWINDOW_AUTODEL;
	else 
		delFlags &= ~zWINDOW_AUTODEL;
	return b;
}



inline void zWindow::captureMouse() {
	SetCapture(hWnd);
}

inline void zWindow::releaseMouse() {
	ReleaseCapture();
}

inline void zWindow::clipCursor(zRect*r) {
    ClipCursor((RECT*)r);
}

inline int zTaskNode::go() { 
	return (procObject->*callme)(); 
}

inline void zTaskDev::add(zEvH *h,CallBackProc p) {
	t->append(ZNEWH zTaskNode(h,p));
	numtasks++;
}

inline void zTaskDev::remove() {
        if (iter!=0) {
	    (*iter).remove();
	    (*iter).reset();
	    numtasks--;
	}
}



	Z_DLLVSPEC inline void zApp::debugOn(BOOL v) {
		_debugOn = v;

	};

