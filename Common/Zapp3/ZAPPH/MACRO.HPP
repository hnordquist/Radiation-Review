#ifndef _ZAPPMACRO_INCLUDED
#define _ZAPPMACRO_INCLUDED 1
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


#define ZSETCURSOR(x) if (x) SetCursor(x)


#define ZIWIN95PLUSCHECK ((LOWORD(GetVersion())&255)>=4)


extern subclass sc[8];



#define DOCOMPACT(xxx)


#define DOKEYBOARD(xxx)							\
	case WM_CHAR:								\
	case WM_SYSCHAR:							\
		tmp=wp->ch((zKeyEvt*)xxx);				\
		break;									


#define NOTRACKER(xxx)									\
	case WM_MOUSEMOVE:									\
		ZSETCURSOR(wp->cursor().sysId());				\
		break;
	
#define DOSCROLL(xxx)									\
	case WM_HSCROLL:									\
	case WM_VSCROLL:									\
		zScrollInfo *si;								\
		if (IsWindow((HWND)(xxx->lParam))) {					\
			zScrollBarBase *sbp;						\
			sbp=(zScrollBarBase*)(zAppGetAppVar(app)->winMap()->get((HWND)xxx->lParam));\
			if (!sbp || !sbp->isA(ZTYP_zScrollBarBase)) break; \
			si=sbp->info();								\
		} else {										\
			if (xxx->Msg==WM_VSCROLL)					\
				si=(zScrollInfo*)wp->scrollBarVert();	\
			else										\
				si=(zScrollInfo*)wp->scrollBarHoriz();	\
		}											\
		if (si!=0) {								\
			switch (LOWORD(Ev->wParam)) {			\
			case SB_BOTTOM:							\
				si->pos((si->limits()).hi());		\
				break;								\
			case SB_ENDSCROLL:						\
				break;								\
			case SB_LINEDOWN:						\
				si->pos(si->pos()+si->moveDelta());	\
				break;								\
			case SB_LINEUP:							\
				si->pos(si->pos()-si->moveDelta());	\
				break;								\
			case SB_PAGEDOWN:						\
				si->pos(si->pos()+si->pageDelta());	\
				break;								\
			case SB_PAGEUP:							\
				si->pos(si->pos()-si->pageDelta());	\
				break;								\
			case SB_THUMBPOSITION:					\
				si->posOnly(HIWORD(Ev->wParam));	\
				break;								\
			case SB_THUMBTRACK:						\
				si->posOnly(HIWORD(Ev->wParam));	\
				break;								\
			case SB_TOP:							\
				si->pos((si->limits()).lo());		\
				break;								\
				}									\
			}										\
		tmp=wp->scroll(xxx);						\
		break;
	
				


#endif


