#ifndef _ZAPPCONTROLS_INCLUDED
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

#define _ZAPPCONTROLS_INCLUDED 1

#ifndef ZAPP_FIELDS_DEFINED
#define ZAPP_FIELDS_DEFINED

#define FLD_ALPHA           0x00000001L
#define FLD_NUMERIC         0x00000002L
#define FLD_SIGNEDNUMERIC   0x00000004L
#define FLD_HEXNUMERIC      0x00000008L
#define FLD_ALPHANUMERIC    0X00000010L
#define FLD_CHARSETMASK     0x000000ffL

#define FLD_NOTREQUIRED     0x00000100L
#define FLD_STRIPLIT        0x00040000L
#define FLD_COMPLETE        0x00080000L

#define FLD_NOAUTOSELECT    0x00100000L
#define FLD_TOUPPER         0x00004000L
#define FLD_TOLOWER         0x00008000L
#define FLD_PROPER          0x00000020L
#define FLD_SHOWGUIDES      0x00200000L

#endif

enum SubClassType { ZCTRL_BUTTON,ZCTRL_LISTBOX,ZCTRL_SCROLLBAR,ZCTRL_STATIC,ZCTRL_EDIT,ZCTRL_COMBOBOX,ZCTRL_NOSUB,ZCTRL_SUPER,ZCTRL_USERBASE,ZCTRL_USEROVR };


long Z_DLLSPEC zControlWndProc(zEvent *Ev,int *offset,FARPROC *fp);

class Z_DLLSPEC zSuperClass;
declSlist(zSuperClass);

class Z_DLLSPEC zSuperClass {
static zSuperClassSlist* superclassed;
public:
	RWCString oldclass;
	WNDCASTPROC oldproc;
	WNDCASTPROC newproc;
	FARPROC callback;
	HANDLE inst;
	HANDLE dllInst;
	zSuperClass();
	zSuperClass(char * name,WNDCASTPROC proc,HANDLE hLib);
	zSuperClass* lookup(RWCString& name);
	void unregister();
};

extern "C" {
struct subclass {
	char * oldclass;
	char * newclass;
	WNDCASTPROC oldproc;
	HANDLE inst;
	int offset;
};
}

extern "C" {
typedef long (FAR PASCAL *FPproc)(HWND,zInt,zInt,unsigned long);
}


class Z_DLLSPEC zStaticText;


class Z_DLLSPEC zGenericControl;

class Z_DLLSPEC zControl:public zChildWin {
public:
	virtual ~zControl();

	virtual void show(int sw=SW_SHOWNORMAL);

	zColor textColor() const;
	zColor textColor(const zColor&);

	void text(const RWCString&);
	void text(const zString&);
	void text(const char *);
	RWCString text();

private:
	zColor foreground;
	zGenericControl *prevControl;
	HBRUSH _cBrush;
protected:
 	zControl(zWindow*,zSizer*,unsigned long initalStyle=0,const char* =0,int ctrlId=0);
 	zControl(zWindow*,int ctrlId);

	void ziCopyFromPrevious(zControl*, zWindow*, int);
	WNDCASTPROC _oldproc;	
	SubClassType subClass;

public:
	virtual void create();
	SubClassType getSubType() {return subClass;}
	virtual void ziBackgroundColor(const zColor& d);
	WNDCASTPROC getOldProc() {return _oldproc;}
	HBRUSH ziCtlBrush() { return _cBrush; }
};

inline zColor zControl::textColor() const { return foreground; }


class Z_DLLSPEC zGenericControl:public zControl {
public:
	zGenericControl(zWindow*,zSizer*,unsigned long style,const char* =0,int ctrlId=0,char *className=0,void *ctldata=0);
	zGenericControl(zWindow*,int ctrlId);
	~zGenericControl();
	void shadow(zControl *);
	zControl* shadow();

private:
	zSuperClass * scinfo;
	WNDCASTPROC scproc;
	zControl *_shadow;
	void *tmpCtldata;
public:
	virtual void create();

	virtual long dispatch(zEvent*);
};

class Z_DLLSPEC zStaticText:public zControl {
public:
	zStaticText(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zStaticText(zWindow*,int);
	~zStaticText();

private:
	
public:
	virtual long dispatch(zEvent*);
	HICON iconStatic;
};


class Z_DLLSPEC zButton:public zControl {
public:
	void setNotifyClicked(zEvH *,NotifyProc);
	void setNotifyDoubleClicked(zEvH *,NotifyProc);
	void setNotifyClicked(zEvH *,ClickProc);
	void setNotifyDoubleClicked(zEvH *,ClickProc);

	void removeNotifyClicked(zEvH *,NotifyProc);
	void removeNotifyDoubleClicked(zEvH *,NotifyProc);
	void removeNotifyClicked(zEvH *,ClickProc);
	void removeNotifyDoubleClicked(zEvH *,ClickProc);
protected:
	zButton(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zButton(zWindow*,int);

	friend class Z_DLLSPEC ziDialogReader;
	friend class Z_DLLSPEC zMDIDialogFrame;
private:
public:
	virtual long dispatch(zEvent*);
};

class Z_DLLSPEC zPushButton:public zButton {
public:
	zPushButton(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zPushButton(zWindow*,int);
	~zPushButton();
};

class Z_DLLSPEC zDefPushButton:public zButton {
public:
	zDefPushButton(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zDefPushButton(zWindow*,int);
	~zDefPushButton();
};

class Z_DLLSPEC zCheckBox:public zButton {
public:
	zCheckBox(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zCheckBox(zWindow*,int);
	zCheckBox(zWindow*,int, int *dest_, unsigned long fieldFlags_=0);
	~zCheckBox();

	void check(BOOL);
	BOOL check();
	virtual int storeData();
	virtual int setToDefault();
	void setStoreAddr(int *);

protected:
	void *_dest;
	int _destType;
	unsigned long _fieldFlags;
};

class Z_DLLSPEC zRadioButton:public zButton {
public:
	zRadioButton(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zRadioButton(zWindow*,int);
	void check(BOOL);
	BOOL check();
	~zRadioButton();
};

class Z_DLLSPEC zGroupBox:public zButton {
public:
	zGroupBox(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zGroupBox(zWindow*,int);
	~zGroupBox();
};


class Z_DLLSPEC zEditBase : public zControl {
public:
	~zEditBase();
	int checkData();
	int storeData();
	int setToDefault();

	void getRect(zRect&);
	void setRect(zRect*);

	void setNotifyChange(zEvH *,NotifyProc);
	void setNotifyErrorSpace(zEvH *,NotifyProc);
	void setNotifyHScroll(zEvH *,NotifyProc);
	void setNotifyVScroll(zEvH *,NotifyProc);
	void setNotifyKillFocus(zEvH *,NotifyProc);
	void setNotifySetFocus(zEvH *,NotifyProc);
	void setNotifyMaxText(zEvH *,NotifyProc);
	void setNotifyUpdate(zEvH *,NotifyProc);
	void removeNotifyChange(zEvH *,NotifyProc);
	void removeNotifyErrorSpace(zEvH *,NotifyProc);
	void removeNotifyHScroll(zEvH *,NotifyProc);
	void removeNotifyVScroll(zEvH *,NotifyProc);
	void removeNotifyKillFocus(zEvH *,NotifyProc);
	void removeNotifySetFocus(zEvH *,NotifyProc);
	void removeNotifyMaxText(zEvH *,NotifyProc);
	void removeNotifyUpdate(zEvH *,NotifyProc);
	BOOL canUndo();
	BOOL undo();
	void emptyUndoBuffer();
	void hasChanged(BOOL);
	BOOL hasChanged();
	zRange selection();
	void limitText(int len);
	void replaceSelection(const char*);
	void selection(int start,int end);
	RWCString getSelection();
	void cut();
	void copy();
	void paste();
	void setStoreAddr(RWCString *);
	void setStoreAddr(zString *);

protected:
	zEditBase(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zEditBase(zWindow*,int, RWCString *dest_=0, unsigned long fieldFlags_=0);
	zEditBase(zWindow*,int, zString *dest_=0, unsigned long fieldFlags_=0);

	friend class Z_DLLSPEC ziDialogReader;
	friend class Z_DLLSPEC zMDIDialogFrame;

	RWCString *_dest;
	unsigned long _fieldFlags;



public:
	virtual long dispatch(zEvent*);
};

class Z_DLLSPEC zEditBox:public zEditBase {
public:
	zEditBox(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zEditBox(zWindow*,int, RWCString *dest_=0, unsigned long fieldFlags_=0);
	zEditBox(zWindow*,int, zString *dest_, unsigned long fieldFlags_=0);
	void scrollBox(int vert,int horz);
	~zEditBox();
};

class Z_DLLSPEC zEditLine:public zEditBase {
public:
	zEditLine(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zEditLine(zWindow*,int, RWCString *dest_=0, unsigned long fieldFlags_=0);
	zEditLine(zWindow*,int, zString *dest_, unsigned long fieldFlags_=0);
	virtual ~zEditLine();
	
	void text(const RWCString&);
	void text(const zString&);
	void text(const char *);
	RWCString text() { return zControl::text(); }
};

class Z_DLLSPEC zDateEdit : public zEditLine {
public:
	zDateEdit(zWindow*,int id_,RWCString *_dest_=0,unsigned long flags=0);
	zDateEdit(zWindow*,zSizer*,unsigned long styles_=0,const char* =0,
		int id_=0);
	~zDateEdit();
	
	RWDate date();
	int checkData();
};

class Z_DLLSPEC zTimeEdit : public zEditLine {
public:
	zTimeEdit(zWindow*,int id_,RWCString *_dest_=0, unsigned long flags =0);
	zTimeEdit(zWindow*,zSizer*,unsigned long styles_=0,const char* n =0, 
		int id_=0);
	~zTimeEdit();

	RWTime time();
	int checkData();
};

#define ZAPP_STORETYPE_INT	1
#define ZAPP_STORETYPE_ZSTRING	2

class Z_DLLSPEC zListBoxBase:public zControl {
public:
	~zListBoxBase();
	
	int add(const RWCString&);
	int add(const zString&);
	int add(const char*);
	int del(int);
	int insert(const RWCString&,int pos);
	int insert(const zString&,int pos);
	int insert(const char *,int pos);
	void clear();
	int count();
	int selection(int);
	int selection(const RWCString&);
	int selection(const zString&);
	int selection(const char*);
	int selection();
	void clearAllSelections();
	RWCString text(int pos);
	int getItem(RWCString &dest_, int itemNumber = -1);
	void addDir(unsigned int,const RWCString&);
	void addDir(unsigned int,const char*);
	int addStrSlist(RWCStringSlist*);
	int addStrSlist(RWTPtrSlist<RWCString>*);
	int addStrDlist(RWTPtrDlist<RWCString>*);
	int addStrSlist(zStringSlist*);
	int addCharStrings(char**);
	int addStrBlock(const zResStrBlock&);
	
	void setNotifyDoubleClick(zEvH *,NotifyProc);
	void setNotifyErrorSpace(zEvH *,NotifyProc);
	void setNotifyKillFocus(zEvH *,NotifyProc);
	void setNotifySetFocus(zEvH *,NotifyProc);
	void setNotifySelChange(zEvH *,NotifyProc);
	
	void removeNotifyDoubleClick(zEvH *,NotifyProc);
	void removeNotifyErrorSpace(zEvH *,NotifyProc);
	void removeNotifyKillFocus(zEvH *,NotifyProc);
	void removeNotifySetFocus(zEvH *,NotifyProc);
	void removeNotifySelChange(zEvH *,NotifyProc);
	
	virtual int checkData();
	virtual int storeData();
	virtual int setToDefault();
	void setStoreAddr(int *);
	void setStoreAddr(RWCString *);
	void setStoreAddr(zString *);

protected:
	zListBoxBase(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zListBoxBase(zWindow*,int);

	friend class Z_DLLSPEC ziDialogReader;
	friend class Z_DLLSPEC zMDIDialogFrame;

	void *_dest;
	int _destType;
	unsigned long _fieldFlags;
	int _maxTextExtent;
	int getStringWidth(HDC hdc,const char* txt);
	void adjustHorizontalScrollBar(const char* txt);
public:
	virtual long dispatch(zEvent*);
};

class Z_DLLSPEC zListBox:public zListBoxBase {
public:
	zListBox(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zListBox(zWindow*,int);
	zListBox(zWindow*,int, RWCString* dest_, unsigned long fieldFlags_=0);
	zListBox(zWindow*,int, zString* dest_, unsigned long fieldFlags_=0);
	zListBox(zWindow*,int, int *dest_, unsigned long fieldFlags_=0);
	~zListBox();
};

class Z_DLLSPEC zListBoxUnsorted:public zListBoxBase {
public:
	zListBoxUnsorted(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zListBoxUnsorted(zWindow*,int);
	zListBoxUnsorted(zWindow*,int, RWCString* dest_, unsigned long fieldFlags_=0);
	zListBoxUnsorted(zWindow*,int, zString* dest_, unsigned long fieldFlags_=0);
	zListBoxUnsorted(zWindow*,int, int *dest_, unsigned long fieldFlags_=0);
	~zListBoxUnsorted();
};
        



class Z_DLLSPEC zComboBoxBase:public zControl {
public:
	~zComboBoxBase();
	
	int add(const RWCString&);
	int add(const zString&);
	int add(const char*);
	void addDir(unsigned int,const RWCString&);
	void addDir(unsigned int,const char*);
	int addStrSlist(RWCStringSlist *);
	int addStrSlist(zStringSlist *);
	int addStrSlist(RWTPtrSlist<RWCString>*);
	int addStrDlist(RWTPtrDlist<RWCString>*);
	int addCharStrings(char **);
	int addStrBlock(const zResStrBlock&);

	int del(int);
	int insert(const RWCString&,int pos);
	int insert(const zString&,int pos);
	int insert(const char*,int pos);
	int count();
	int selection(int);
	int selection(const RWCString &);
	int selection(const zString &);
	int selection(const char*);
	int selection();
	RWCString text(int pos);
	void text(const RWCString&);
	void text(const char*);
	int getItem(RWCString &dest_, int itemNumber = -1);
	void getEditText(RWCString &dest_);
	int reset();
	int drop();
	int pop();
	
	void setNotifyErrorSpace(zEvH *,NotifyProc);
	void setNotifyKillFocus(zEvH *,NotifyProc);
	void setNotifySetFocus(zEvH *,NotifyProc);
	void setNotifySelChange(zEvH *,NotifyProc);
	void setNotifyDropDown(zEvH *,NotifyProc);
	void setNotifyEditChange(zEvH *,NotifyProc);
	void setNotifyEditUpdate(zEvH *,NotifyProc);
	
	void removeNotifyErrorSpace(zEvH *,NotifyProc);
	void removeNotifyKillFocus(zEvH *,NotifyProc);
	void removeNotifySetFocus(zEvH *,NotifyProc);
	void removeNotifySelChange(zEvH *,NotifyProc);
	void removeNotifyDropDown(zEvH *,NotifyProc);
	void removeNotifyEditChange(zEvH *,NotifyProc);
	void removeNotifyEditUpdate(zEvH *,NotifyProc);
	
	virtual int checkData();
	virtual int storeData();
	virtual int setToDefault();
	void setStoreAddr(int *);
	void setStoreAddr(RWCString *);
	void setStoreAddr(zString *);
	
	void editBackgroundColor(const zColor&);
	const zColor editBackgroundColor() const;

	void setNotifyDoubleClick(zEvH *,NotifyProc);
	void removeNotifyDoubleClick(zEvH *,NotifyProc);

protected:
	zComboBoxBase(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zComboBoxBase(zWindow*,int);

	friend class Z_DLLSPEC ziDialogReader;
	friend class Z_DLLSPEC zMDIDialogFrame;
	friend class Z_DLLSPEC zFontSelForm;
	void *_dest;
	int _destType;
	unsigned long _fieldFlags;


	unsigned long _editBackColor;

public:
	virtual long dispatch(zEvent*);
	virtual int background(zEvent*);
};  


class Z_DLLSPEC zComboBox:public zComboBoxBase {
public:
	zComboBox(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zComboBox(zWindow*,int);
	zComboBox(zWindow*,int, RWCString* dest_, unsigned long fieldFlags_=0);
	zComboBox(zWindow*,int, zString* dest_, unsigned long fieldFlags_=0);
	zComboBox(zWindow*,int, int *dest_, unsigned long fieldFlags_=0);
	~zComboBox();

};

class Z_DLLSPEC zComboBoxFull:public zComboBoxBase {
public:
	zComboBoxFull(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zComboBoxFull(zWindow*,int);
	zComboBoxFull(zWindow*,int,RWCString* dest_,unsigned long fieldFlags_=0);
	zComboBoxFull(zWindow*,int,zString* dest_, unsigned long fieldFlags_=0);
	zComboBoxFull(zWindow*,int, int *dest_, unsigned long fieldFlags_=0);
	~zComboBoxFull();

};

class Z_DLLSPEC zComboBoxStatic:public zComboBoxBase {
public:
	zComboBoxStatic(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zComboBoxStatic(zWindow*,int);
	zComboBoxStatic(zWindow*,int,RWCString*dst_,unsigned long fieldFlags_=0);
	zComboBoxStatic(zWindow*,int,zString*dst_,unsigned long fieldFlags_=0);
	zComboBoxStatic(zWindow*,int,int *dest_, unsigned long fieldFlags_=0);
	~zComboBoxStatic();

};


class Z_DLLSPEC zScrollBarBase:public zControl {
public:
	~zScrollBarBase();
	zScrollInfo *info() const;
	void info(zScrollInfo *s);

protected:
	zScrollBarBase(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zScrollBarBase(zWindow*,int);

	friend class Z_DLLSPEC ziDialogReader;
	friend class Z_DLLSPEC zMDIDialogFrame;

private:
	zScrollInfo *_info;
public:
	virtual long dispatch(zEvent*);
};

inline zScrollInfo* zScrollBarBase::info() const { return _info; }
inline void zScrollBarBase::info(zScrollInfo *s) { _info=s; }


class Z_DLLSPEC zScrollBarVert:public zScrollBarBase {
public:
	zScrollBarVert(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zScrollBarVert(zWindow*,int);
	~zScrollBarVert();
};

class Z_DLLSPEC zScrollBarHorz:public zScrollBarBase {
public:
	zScrollBarHorz(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zScrollBarHorz(zWindow*,int);
	~zScrollBarHorz();
};

class Z_DLLSPEC zScrollBarCtrl:public zScrollBarBase {
public:
	zScrollBarCtrl(zWindow*,zSizer*,unsigned long =0,const char* =0,int =0);
	zScrollBarCtrl(zWindow*,int);
	~zScrollBarCtrl();
};

#define CTRLTHUNKNAME(xx) (WNDCASTPROC)xx##zCustomProc

#define declCtrlThunk(xx)												\
extern "C" {															\
long FAR PASCAL _export xx##zCustomProc(HWND hWnd,zInt message,zInt wParam,LONG lParam) {	\
	static int offset=0;												\
	static FARPROC fproc=0;												\
	zEvent ee;														\
	ee.who=hWnd;														\
	ee.Msg=message;														\
	ee.wParam=wParam;													\
	ee.lParam=lParam;													\
	return zControlWndProc(&ee,&offset,&fproc);			\
}																		\
}																		\



#define ZCBN_SETFOCUS  0x4000
#define ZCBN_KILLFOCUS 0x8000



#include <zapph/control.inl>
#endif

