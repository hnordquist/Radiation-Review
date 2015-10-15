#ifndef _ZAPPFRAMEWIN_INCLUDED
#define _ZAPPFRAMEWIN_INCLUDED 1
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


class Z_DLLSPEC zFrameWin:public zWindow {
public:
	virtual ~zFrameWin();

	virtual const char* caption();
	virtual void caption(const RWCString& s);
	virtual void caption(const zString& s);
	virtual void caption(const char* s);

	void setIcon(zIcon*);

	void menu(zMenu* m);
	zMenu* menu();

	void setMinimumSize();
protected:
	zFrameWin(zWindow*,zSizer*,DWORD style=0,const char* =0);
	zFrameWin(zSizer*,DWORD style=0,const char* =0);
	zFrameWin(zWindow*);

	zMenu* pMenu;

	friend class Z_DLLSPEC zMDIAppFrame;

	zDimension _keepSize;
public:

	virtual void create();
	virtual long dispatch(zEvent*);
};

inline void zFrameWin::menu(zMenu* m) {
	if ((pMenu)&&(pMenu!=m)) delete pMenu;
	pMenu=m;
}

inline zMenu* zFrameWin::menu() { return pMenu;  }


#endif
