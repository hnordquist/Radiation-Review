#ifndef _ZAPPSCROLLER_INCLUDED
#define _ZAPPSCROLLER_INCLUDED 1
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


class Z_DLLSPEC zScrollerBase : public zEvH {
	zWindow *wp;
protected:
	zScrollerBase(zWindow*);
	friend class Z_DLLSPEC zPane;
	virtual void ziSetOwner(zWindow*w);
public:
	zWindow *owner() {return wp;}
	BOOL vertScrollBarExists();
	BOOL horzScrollBarExists();
	virtual int respondToSize();

	virtual int scrollRouter(zEvent *);

	virtual int vertLineMove();
	virtual int vertPageMove();
	virtual int vertTop();
	virtual int vertBottom();
	virtual int vertThumbPos();
	virtual int vertThumbTrack();
	virtual int horzLineMove();
	virtual int horzPageMove();
	virtual int horzLeft();
	virtual int horzRight();
	virtual int horzThumbPos();
	virtual int horzThumbTrack();
        
	virtual int preScroll();
    virtual ~zScrollerBase();
};

class Z_DLLSPEC zControlScroller : public zScrollerBase {
public:
	zControlScroller(zScrollBarBase *);
	zScrollBarBase *ownerSBar();
	int scrollRouter(zEvent *);
};

class Z_DLLSPEC zScrollerWKeyboard : public zScrollerBase {
	BOOL keyboardState;
protected:
	virtual void ziSetOwner(zWindow*w);
public:
	zScrollerWKeyboard(zWindow *w);
	
	int setKeyProcessing();
	int removeKeyProcessing();
	BOOL processingKeys() {return keyboardState;}

	int doKeyboard(zEvent *);

	virtual int processKeyboard(zKeyEvt *);
	~zScrollerWKeyboard();
};

class Z_DLLSPEC zPaneScroller : public zScrollerWKeyboard {
	zRect horzScrollR;
	zRect vertScrollR;
	int horzMoveAmount;
	int vertMoveAmount;
	int horzPageAmount;
	int vertPageAmount;
public:
	zPaneScroller(zPane *);
	zPane *ownerp();
	void setHorz(zRect* hR=0, int hMove=0, int hPage=0);
	void setVert(zRect* vR = 0, int vMove = 0, int vPage=0);
	void horzRect(zRect hR);
	void vertRect(zRect vR);
	zRect& horzRect();
	zRect& vertRect();
	int& horzMove();
	int& vertMove();
	void horzMove(int);
	void vertMove(int);
	int& horzPage();
	int& vertPage();
	void horzPage(int);
	void vertPage(int);

	virtual int vertLineMove();
	virtual int vertPageMove();
	virtual int vertTop();
	virtual int vertBottom();
	virtual int vertThumbPos();
	virtual int horzLineMove();
	virtual int horzPageMove();
	virtual int horzLeft();
	virtual int horzRight();
	virtual int horzThumbPos();
	virtual int respondToSize();	
};

class Z_DLLSPEC zPaneScrollerAuto : public zPaneScroller {
	BOOL resetOrigin;
	BOOL updateImmediate;
	zDimension preferredSizeD;
public:
	zPaneScrollerAuto(zPane *);
	~zPaneScrollerAuto();
	void autoUpdate(BOOL);
	BOOL autoUpdate();
	BOOL resetOrg() {return resetOrigin;}
	zDimension& preferredSize();
	void preferredSize(zDimension&);
	virtual int vertLineMove();
	virtual int vertPageMove();
	virtual int vertTop();
	virtual int vertBottom();
	virtual int vertThumbPos();
	virtual int horzLineMove();
	virtual int horzPageMove();
	virtual int horzLeft();
	virtual int horzRight();
	virtual int horzThumbPos();

	virtual int respondToSize();
};

#endif

