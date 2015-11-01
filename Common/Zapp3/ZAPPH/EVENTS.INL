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

inline int zCommandEvt::cmd() const {
	return LOWORD(wParam);
}

inline BOOL zCommandEvt::isControlCmd() const {
	return (lParam!=0);
}

inline BOOL zCommandEvt::isMenuCmd() const {	
	return (lParam==0 && HIWORD(wParam)==0);
}

inline BOOL zCommandEvt::isAcceleratorCmd() const {
	return (lParam==0 && HIWORD(wParam)==1);	
}

inline BOOL zMouseMoveEvt::isShift() const {
	return (wParam&MK_SHIFT);
}

inline BOOL zMouseMoveEvt::isCtrl() const {
	return (wParam&MK_CONTROL);
}

inline BOOL zMouseClickEvt::isShift() const {
	return (wParam&MK_SHIFT);
}

inline BOOL zMouseClickEvt::isCtrl() const {
	return (wParam&MK_CONTROL);
}


inline zPoint zMouseMoveEvt::pos() {
	return zPoint(short(loParam()),short(hiParam()));
}

inline zMouseMoveEvt::operator zPoint() {
	return zPoint(short(loParam()),short(hiParam()));
}

inline zPoint zMouseClickEvt::pos() {
	return zPoint(short(loParam()),short(hiParam()));
}

inline zMouseClickEvt::operator zPoint() {
	return zPoint(short(loParam()),short(hiParam()));
}

inline zCoOrd zSizeEvt::height() const {
	return (zCoOrd)hiParam();
}

inline zCoOrd zSizeEvt::width() const {
	return (zCoOrd)loParam();
}

inline zDimension zSizeEvt::size() const {
	return zDimension(loParam(),hiParam());
}

inline zSizeEvt::operator zDimension() const {
	return zDimension(loParam(),hiParam());
}

inline zPoint zMoveEvt::newOrigin() const {
	return zPoint(loParam(),hiParam());
}

inline zMoveEvt::operator zPoint() const {
	return newOrigin();
}

inline BOOL zKeyEvt::alt() const {
	return (0x2000&hiParam());
}

inline BOOL zKeyEvt::ctrl() const {
	return ((GetKeyState(VK_CONTROL)&0x8000) ? 1:0);
}

inline BOOL zKeyEvt::shift() const {
	return ((GetKeyState(VK_SHIFT)&0x8000) ? 1:0);
}

inline BOOL zKeyEvt::extended() const {
	return (0x0100&hiParam());
}

inline int zKeyEvt::prevState() const {
	return (0x4000&hiParam());
}

inline int zKeyEvt::tranState() const {
	return (0x8000&hiParam());
}

inline char zKeyEvt::ch() const {
	return (char)wParam;
}


inline int zKeyEvt::code() const {
	return hiParam()&0xff;
}

inline int zKeyEvt::repeat() const {
	return loParam();
}

inline BOOL zActivateEvt::active() const {
	BOOL ret;
	if (Msg==WM_MDIACTIVATE)
		ret=(lParam==(unsigned long)who);
	else
		ret=(LOWORD(wParam));
	return ret;
}

inline BOOL zFocusEvt::gainFocus() const {
	return (Msg==WM_SETFOCUS);
}

inline BOOL zMenuHiliteEvt::closed() const {
	return ((HIWORD(wParam))==0xffff && lParam==NULL);
}

inline int zMenuHiliteEvt::id() const {
	return LOWORD(wParam);
}

inline BOOL zMenuHiliteEvt::isBitmap() const {
     return flags()&MF_BITMAP;
}

inline BOOL zMenuHiliteEvt::isChecked() const {
     return flags()&MF_CHECKED;
}

inline BOOL zMenuHiliteEvt::isDisabled() const {
    return flags()&MF_DISABLED;
}

inline BOOL zMenuHiliteEvt::isGrayed() const {
    return flags()&MF_GRAYED;
}

inline BOOL zMenuHiliteEvt::isSelected() const {
    return flags()&MF_MOUSESELECT;
}

inline BOOL zMenuHiliteEvt::isOwnerDraw() const {
    return flags()&MF_OWNERDRAW;
}

inline BOOL zMenuHiliteEvt::isSeparator() const {
    return flags()&MF_SEPARATOR;
}


inline BOOL zMenuHiliteEvt::isPopup() const {
    return flags()&MF_POPUP;
}

inline BOOL zMenuHiliteEvt::isSystemMenu() const {
    return flags()&MF_SYSMENU;
}

inline unsigned int zMenuHiliteEvt::flags() const {
    return HIWORD(wParam);
}


inline int zNotifyEvt::identifier() const {
	return LOWORD(wParam);
}

inline BOOL zNotifyEvt::clicked() const {
	return (HIWORD(wParam)==BN_CLICKED);
}



