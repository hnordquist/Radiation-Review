#ifndef _ZAPPDIALOG_INCLUDED
#define _ZAPPDIALOG_INCLUDED 1
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

enum zModalStyle {
    ZMODAL_PARENT=0, ZMODAL_APP
};

class Z_DLLSPEC zDialogUnit:public zLogMeasure {
	declPersis(zDialogUnit)
public:
	zDialogUnit();
	zDialogUnit(const zDialogUnit& d):zLogMeasure((const zLogMeasure&)d) {}
	zDialogUnit(zCoOrd i,zCoOrd j,zDisplay* =0);
	zDialogUnit(zCoOrd i,zCoOrd j,zFont*);
	virtual zCoOrd setLogX(zCoOrd x,zDisplay* =0);
	virtual zCoOrd setLogY(zCoOrd y,zDisplay* =0);
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};

typedef zChildWin* (*zControlBuilder)(zWindow*,zSizer*,DWORD,const char*,int);

class Z_DLLSPEC zControlType {
public:
	zControlType(const char* name,zControlBuilder);
	zChildWin* buildNew(zWindow*,zSizer*,DWORD,const char*,int);

private:
	char *_typeName;
	zControlBuilder builder;
public:
	char* typeName() { return _typeName; }
};

declDlist(zControlType)

#define CTRLBUILDERNAME(ETYPE) (zControlBuilder)zCtrlBld##ETYPE

#define ZCTRLPANENAME(ETYPE) (zControlBuilder)zCtrlBld##ETYPE

#define CTRLBUILDER(ETYPE)													\
zChildWin* Z_DLLSPEC zCtrlBld##ETYPE(zWindow*w,zSizer*siz,DWORD d,const char* t,int id_){\
	return ZNEW ETYPE(w,siz,d,t,id_);										\
}																			\

#define ZCTRLPANEIMPL(ETYPE)												\
zChildWin* Z_DLLSPEC zCtrlBld##ETYPE(zWindow*w,zSizer*siz,DWORD d,const char* t,int id_){\
	return ZNEW ETYPE(w,siz,d,t,id_);										\
}																			\



extern "C" {
	long APIENTRY DialogFrameProc(HWND, zInt, zInt, DWORD);
}


class Z_DLLSPEC ziDialogReader {
	int _status;
	LPSTR resAt;
	zResId menuRes;
	int numKids;
	zWindow *me;
	
	zRect sizeRect;
	zFont *dlgFnt;
	LPSTR pRes;
	HANDLE res;
	HANDLE hFrom;
	unsigned int defId;
	zResId _rid;
	RWCString _caption;
	int _hasMenu;
	unsigned long style;
	void getResource();
	void freeResource();
	void decodeKids();
	void decodeHeader();
	zWindow* parent();
	void createBaseTypes(char *,void*,int,zPoint,zDimension,DWORD);
public:
	ziDialogReader(zWindow* to,const zResId&);
	~ziDialogReader();
	void recalc() { decodeHeader(); }

	void createChildren();
	void setSize(zSizer*);

	int isValid() { return (_status==0); }
	int hasMenu() { return _hasMenu; }
	zResId& getMenuResId() { return menuRes; }
	unsigned long getStyle() { return style; }
	RWCString& getCaption() { return _caption; }
	zFont* getFont() { return dlgFnt; }
	unsigned int getDefault() { return defId; }
	void setWindow(zWindow *w) { me=w; }

#if defined(WIN32) && defined(Z_NATIVE_NBD)
        void setLogicalSize(zSizer*);
#endif
};


class Z_DLLSPEC zDialogFrame:public zFrameWin {
public:
	zDialogFrame(zWindow*,const zResId&,DWORD =0,int noAutoCreate=0);
	zDialogFrame(zWindow*,zSizer*,DWORD =0,const char* =0,int noAutoCreate=0);
	virtual ~zDialogFrame();
	
	virtual void init();
	virtual void show(int=SW_SHOWNORMAL);
	void modal(zModalStyle=ZMODAL_PARENT);
	virtual void shutdown();


private:
	void go();
protected:
	ziDialogReader *dr;
	friend long APIENTRY DialogFrameProc(HWND, zInt, zInt, DWORD);
	HWND lastFocus;
	BOOL isModal;
	unsigned int defId;
	virtual WNDCLASS* registerHook();

public:
	virtual void create();
	virtual int focus(zFocusEvt*);
	virtual int activate(zActivateEvt*);
};

declStack(zDialogFrame)

#endif
