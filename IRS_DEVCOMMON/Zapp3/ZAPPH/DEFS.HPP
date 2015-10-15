#ifndef _ZAPPDEFS_INCLUDED
#define _ZAPPDEFS_INCLUDED 1
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


#define Z_TEMPORARILY_DISABLED_DURING_MAC_PORT

#define Z_USEZAPPNEW 1

#define LOCKEDMEM 1
#ifndef REALPOINTERS
#define REALPOINTERS 1
#endif

typedef int zCoOrd;

#ifdef __SC__
#define main() _symantecMain()
#endif

#ifndef _SIZE_T
#define _SIZE_T
#if defined (__GNUC__) && defined (SUNOS4)
typedef unsigned long size_t;
#else
typedef unsigned size_t;
#endif
#endif

#ifndef LOWORD
#define LOWORD(l) ((unsigned short)(unsigned long)(l))
#endif

#ifndef HIWORD
#define HIWORD(l) ((unsigned short)((((unsigned long)(l)) >> 16) & 0xFFFF))
#endif

class Z_DLLSPEC zFrameWin;
class Z_DLLSPEC zChildWin;
class Z_DLLSPEC zCursor;
class Z_DLLSPEC zIcon;
class Z_DLLSPEC zTextPane;
class Z_DLLSPEC zAssert;
class Z_DLLSPEC zResId;
class Z_DLLSPEC zPair;
class Z_DLLSPEC zPoint;
class Z_DLLSPEC zDimension;
class Z_DLLSPEC zRect;
class Z_DLLSPEC zEvent;
class Z_DLLSPEC zSizeEvt;
class Z_DLLSPEC zMoveEvt;
class Z_DLLSPEC zKeyEvt;
class Z_DLLSPEC zDrawEvt;
class Z_DLLSPEC zActivateEvt;
class Z_DLLSPEC zEvH;
class Z_DLLSPEC zWindow;
class Z_DLLSPEC zPane;
class Z_DLLSPEC zAppFrame;
class Z_DLLSPEC zApp;
class Z_DLLSPEC zControl;
class Z_DLLSPEC zColor;
class Z_DLLSPEC zDrawObj;
class Z_DLLSPEC zPen;
class Z_DLLSPEC zBrush;
class Z_DLLSPEC zRange;
class Z_DLLSPEC zTextBuf;
class Z_DLLSPEC zTextPane;
class Z_DLLSPEC zScrollerBase;
class Z_DLLSPEC zRegion;
class Z_DLLSPEC zNotifyNode;
class Z_DLLSPEC zTimer;
class Z_DLLSPEC zTimerDev;
class Z_DLLSPEC zScrollBarCtrl;
class Z_DLLSPEC zFont;
class Z_DLLSPEC zDisplay;
class Z_DLLSPEC zWinDisplay;
class Z_DLLSPEC zPrintJob;
class Z_DLLSPEC zPrJobDlg;
class Z_DLLSPEC zString;
class Z_DLLSPEC zSizer;

extern int Z_DLLSPEC doPaneDraw(zPane *, zEvent*);
extern void Z_DLLSPEC zAppDoSetDraw(zWinDisplay*,int);

BOOL Z_DLLSPEC doHandler(zEvent*,zWindow*);
BOOL Z_DLLSPEC doCommandHandler(zEvent*,zWindow*);

extern Z_DLLVSPEC int Z_DLLVBSPEC __scrPixHeight, __scrPixWidth, __scrPixPerInchX,
			__scrPixPerInchY, __scrMmHeight, __scrMmWidth;


extern Z_DLLVSPEC int Z_DLLVBSPEC _debugOn;


extern HWND activeDialog;


#define zWINDOW_AUTODEL		0x0001
#define zWINDOW_HWND_DEL	0x0002
#define zWINDOW_SHOWN		0x0004
#define zWINDOW_ACTIVE          0x0008
#define ZWINDOW_DISABLEACTIVATE  0x0010
#define ZWINDOW_DISABLEMESSAGES  0x0020
#define ZWINDOW_ISPOPUP     0x0040
#define ZWINDOW_NOSCROLLBARS 0x0080
#define ZWINDOW_SCRLLBRADJ  0x0100
#define ZWINDOW_FONT_OWNER	0x0200

#define ZTYP_zEvH 1
#define ZTYP_zWindow 2
#define ZTYP_zFrameWin 3
#define ZTYP_zChildWin 4
#define ZTYP_zAppFrame 5
#define ZTYP_zDialogFrame 6
#define ZTYP_zFormDialog 7
#define ZTYP_zPane 8
#define ZTYP_zPopupFrame 9
#define ZTYP_zControl 10
#define ZTYP_zTextPane 11
#define ZTYP_zDevice 12
#define ZTYP_zMenu 13
#define ZTYP_zSystemMenu 14
#define ZTYP_zPopupMenu 15
#define ZTYP_zStaticText 16
#define ZTYP_zButton 17
#define ZTYP_zPushButton 18
#define ZTYP_zDefPushButton 19
#define ZTYP_zCheckBox 20
#define ZTYP_zRadioButton 21
#define ZTYP_zGroupBox 22
#define ZTYP_zEditBase 23
#define ZTYP_zEditLine 24
#define ZTYP_zEditBox 25
#define ZTYP_zListBoxBase 26
#define ZTYP_zListBox 27
#define ZTYP_zListBoxUnsorted 28
#define ZTYP_zComboBoxBase 29
#define ZTYP_zComboBox 30
#define ZTYP_zComboBoxFull 31
#define ZTYP_zComboBoxStatic 32
#define ZTYP_zScrollBarBase 33
#define ZTYP_zScrollBarVert 34
#define ZTYP_zScrollBarHorz 35
#define ZTYP_zScrollBarCtrl 36
#define ZTYP_zMDIAppFrame 37
#define ZTYP_zMDIChildFrame 38
#define ZTYP_zMDIMarginFrame 39
#define ZTYP_zMDIDialogFrame 40
#define ZTYP_zMDIFormDialog 41
#define ZTYP_ziCtrlPane 42
#define ZTYP_zChildFrame 43

# if defined(WIN32) && defined(Z_NATIVE_NBD)
#define ZTYP_zPropertySheet 50
#define ZTYP_zPropertyPage  51
# endif

#if Z_DB==2 && !defined(__IBMCPP__)
#ifndef __ZTC__
extern char *zAppFileName;
#endif
#endif

#ifndef Z_IMPDLL
extern FILE *zAppDebugFile;
extern unsigned long debugFlags;
#endif

#define ZAPP_PRINTDLG 7001

long Z_DLLSPEC zAppDoCtlColor(zWindow*,zEvent *ev);

#ifdef __DOS_H
#undef enable()
#undef disable()
#endif


#ifdef enable
#undef enable
#endif
#ifdef disable
#undef disable
#endif


#ifndef ZAPPID_DEFAULTCHILD
#define ZAPPID_DEFAULTCHILD 0
#endif

#define Z_FULLVKCODE(xx) xx->wParam

#endif


