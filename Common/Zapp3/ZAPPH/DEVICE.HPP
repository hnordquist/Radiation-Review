#ifndef _ZAPPDEVICE_INCLUDED
#define _ZAPPDEVICE_INCLUDED 1
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


class Z_DLLSPEC zMenu;
class Z_DLLSPEC zBitmap;
class Z_DLLSPEC zMDIChildFrame;


class Z_DLLSPEC zNotifyNode {
	unsigned int idLo,idHi;
	MESSAGE message;
	HWND w;
protected:
	NotifyProc propagate;
	zEvH* procObject;
	
public:
	zNotifyNode(NotifyProc p,void*ob,MESSAGE=WM_COMMAND,zWindow* =0);
	zNotifyNode(int itemId,NotifyProc p,void*ob,MESSAGE=WM_COMMAND);
	zNotifyNode(unsigned itemLo,unsigned itemHi,NotifyProc p,void*ob,MESSAGE=WM_COMMAND);
	zRange id() { return zRange(idLo,idHi); }
	MESSAGE msg() { return message; }
	virtual BOOL check(zEvent*);
	BOOL checkIdent(zEvH* e,NotifyProc p);
	int go(zEvent *Ev) { return (procObject->*propagate)(Ev); }
};

class Z_DLLSPEC zCtrlNotifyNode:public zNotifyNode {
public:
	zCtrlNotifyNode(int itemId,HWND hChild,int option,NotifyProc,void *ob);
	virtual BOOL check(zEvent*);

private:
	HWND controlWnd;
	int notifyOption;
};

class Z_DLLSPEC zDevice:public zEvH {
public:
	zDevice();
	~zDevice();
	void attach(zWindow*,zNotifyNode*);
	void remove(zWindow*,void *object,NotifyProc p,MESSAGE msg);
	void devType(unsigned long);
	virtual BOOL dispatchEvt(zEvent*);

	zNotifyNodeDlist*& notifyDlist();

protected:
	zNotifyNodeDlist* nlist;
	unsigned long type;
};

inline zNotifyNodeDlist*& zDevice::notifyDlist() { return nlist; }


class Z_DLLSPEC zMenu:public zDevice {
public:
	zMenu();
	zMenu(zWindow*);
	zMenu(zWindow*,const zResId&);
	zMenu(zMDIChildFrame*);
	zMenu(zMDIChildFrame*,const zResId&);
	zMenu(const zResId&);
	virtual ~zMenu();

	BOOL appendItem(int id,const char *str,int mf=MF_STRING);
	BOOL appendItem(int id,zBitmap*);
	BOOL deleteItem(int id);
	BOOL insertItem(int id,int pos,const char *str,int mf=MF_STRING);
	BOOL insertItem(int id,int pos,zBitmap*);
	int enableItem(int id,BOOL which);
	int grayItem(int id,BOOL which);
	int checkItem(int id,BOOL which);
	void update();

	BOOL addStr(int id,const char *str);
	BOOL addHbreak(int id,const char *str);
	BOOL addHbreak();
	BOOL addVbreak(int id,const char *str);
	BOOL appendDropDown(zMenu*,const char *);
	BOOL appendDropDown(zMenu*,zBitmap*);
	BOOL insertDropDown(zMenu*,const char *,int pos);
	BOOL insertDropDown(zMenu*,zBitmap*,int pos);
	BOOL deleteDropDown(int pos);

	void setCommand(void *object,NotifyProc p,int item);
	void setCommand(void *object,NotifyProc p,int itemLo,int itemHi);
	void setCommand(void *object,NotifyProc p);
	void removeCommand(void *object,NotifyProc p);
	void setCommand(void *object,CommandProc p,int item);
	void setCommand(void *object,CommandProc p,int itemLo,int itemHi);
	void setCommand(void *object,CommandProc p);
	void removeCommand(void *object,CommandProc p);
	void setHilite(void *object,NotifyProc p,int item);
	void setHilite(void *object,NotifyProc p,int itemLo,int itemHi);
	void setHilite(void *object,NotifyProc p);
	void removeHilite(void *object,NotifyProc p);
	void setSetup(void *object,NotifyProc p,int item);
	void setSetup(void *object,NotifyProc p,int itemLo,int itemHi);
	void setSetup(void *object,NotifyProc p);
	void removeSetup(void *object,NotifyProc p);

private:
	void loadFromResource(const zResId &r);
protected:
	zWindow* hW;
	HMENU hMenu;
public:
	HMENU& Id() { return hMenu; }
};

class Z_DLLSPEC zSystemMenu:public zMenu {
public:
	zSystemMenu(zWindow*);
	~zSystemMenu();
};

class Z_DLLSPEC zPopupMenu:public zMenu {
public:
	zPopupMenu(const zPoint&,zWindow*);
	zPopupMenu(const zPoint&,zWindow*,const zResId&);
	~zPopupMenu();

	void go(int whichButton =3);
	void go(const zPoint&,int whichButton =3);

private:
	friend class Z_DLLSPEC zMenu;
	zPoint here;
	HMENU hResMenu;
public:
	int ziExitMessageLoop;
};


#endif

