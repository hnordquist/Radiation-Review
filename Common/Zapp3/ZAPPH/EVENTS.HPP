#ifndef _ZAPPEVENTS_INCLUDED
#define _ZAPPEVENTS_INCLUDED 1
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


class Z_DLLSPEC zEvent {
public:
	unsigned long lParam;
	unsigned int wParam;	
	unsigned int Msg;		
	HWND who;
	unsigned int hiParam() const { return HIWORD(lParam); }
	unsigned int loParam() const { return LOWORD(lParam); }
	PARM1 parm1() const { return wParam; }
	PARM2 parm2() const { return lParam; }
	operator HANDLE() const { return who; }
#ifdef STRICT
	operator HWND() const { return who; }
#endif	
};

class Z_DLLSPEC zDrawEvt:public zEvent {};

class Z_DLLSPEC zCommandEvt:public zEvent {
public:
	inline int cmd() const;
    inline BOOL isControlCmd() const;
    inline BOOL isMenuCmd() const;
    inline BOOL isAcceleratorCmd() const;
};

class Z_DLLSPEC zMouseMoveEvt:public zEvent {
public:
	BOOL isButtonDown(int) const;
	inline BOOL isShift() const;
	inline BOOL isCtrl() const;
	zPoint pos();
	operator zPoint();
};

class Z_DLLSPEC zMouseClickEvt:public zEvent {
public:
	BOOL isButton(int) const;
	BOOL isButtonDown(int) const;
	inline BOOL isShift() const;
	inline BOOL isCtrl() const;
	BOOL isDoubleClick() const;
	zPoint pos();
	operator zPoint();
};

class Z_DLLSPEC zSizeEvt:public zEvent {
public:
	inline zCoOrd height() const;
	inline zCoOrd width() const;
	inline zDimension size() const;
	inline operator zDimension() const;
};

class Z_DLLSPEC zMoveEvt:public zEvent {
public:
	zPoint newOrigin() const;
	operator zPoint() const;
};

class Z_DLLSPEC zKeyEvt:public zEvent {
public:
	inline BOOL alt() const;
	inline BOOL ctrl() const;
	inline BOOL shift() const;
	inline BOOL extended() const;
	inline int prevState() const;
	inline int tranState() const;
	inline int code() const;
	inline int repeat() const;
	char ch() const;
};

class Z_DLLSPEC zActivateEvt:public zEvent {
public:
	BOOL active() const;
};

class Z_DLLSPEC zFocusEvt:public zEvent {
public:
	BOOL gainFocus() const;
};

class Z_DLLSPEC zMenuHiliteEvt:public zEvent {
public:
	inline BOOL closed() const;
	inline int id() const;
	inline BOOL isBitmap() const;
	inline BOOL isChecked() const;
	inline BOOL isDisabled() const;
	inline BOOL isGrayed() const;
	inline BOOL isSelected() const;
	inline BOOL isOwnerDraw() const;
	inline BOOL isPopup() const;
	inline BOOL isSystemMenu() const;
	inline BOOL isSeparator() const;
	inline unsigned int flags() const;
};

class Z_DLLSPEC zNotifyEvt:public zEvent {
public:
	inline int identifier() const;
	inline BOOL clicked() const;
};

#include <zapph/events.inl>
#endif




