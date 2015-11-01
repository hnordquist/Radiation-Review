#ifndef _ZAPPWPLAT32_INCLUDED
#define _ZAPPWPLAT32_INCLUDED 1
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



#define ZWIN_ONLY 1
#define Z_WIN


#ifdef _MSC_VER
#if _MSC_VER<800
#pragma optimize("e", off)
#endif
#endif

#if defined (Z_NTIIDLL)
#define Z_NTUSEDLL 1
#endif



#if defined(Z_NTIMPDLL) || defined(Z_NTUSEDLL)

#if defined(Z_NTIMPDLL)
#ifdef _MSC_VER
#define Z_DLLSPEC
#define Z_DLLVSPEC __declspec(dllexport)
#define Z_DLLVBSPEC
#else
#define Z_DLLSPEC _export
#define Z_DLLVSPEC
#define Z_DLLVBSPEC _export
#endif

#else


#ifdef _MSC_VER
#define Z_DLLSPEC
//__declspec(dllimport)
#define Z_DLLVSPEC __declspec(dllimport)
#define Z_DLLVBSPEC
#else
#define Z_DLLSPEC _import
#define Z_DLLVSPEC
#define Z_DLLVBSPEC _import
#endif
#endif


#else
#define Z_DLLSPEC
#define Z_DLLVSPEC
#define Z_DLLVBSPEC
#endif

// Force template instantiations when using the 
// C++ Standard Library as shipped by Rogue Wave

#if defined(__BORLANDC__) && !defined(RW_NO_STL)
class zPrinterSpec;
class zFontDescript;
template class list<RWCString*, allocator>; 
template class rw_slist<zPrinterSpec*>;
template class rw_slist<zFontDescript*>;
template class rw_slist<RWCString*>;
template class rw_slist<int*>;
#endif

#if defined(Z_NTUSEDLL)
#ifdef Z_CLIDLL
#ifdef _MSC_VER
#define Z_USERDLL __declspec(dllexport)
#else
#define Z_USERDLL _export
#endif
#else
#ifdef _MSC_VER
#define Z_USERDLL __declspec(dllimport)
#else
#define Z_USERDLL _import
#endif

#endif

#else
#define Z_USERDLL
#endif





#include <zapph\presys.hpp>
#include <windows.h>
extern "C" {
	int FAR PASCAL fontGrabber(LPLOGFONT, LPTEXTMETRIC, int, LPSTR);
}

#ifdef __CTL3D__
extern "C" {
#include <ctl3d.h>
}
#endif


#define ZHMODULE HANDLE



#ifndef IDOK
#define IDOK 1
#endif

#ifndef IDCANCEL
#define IDCANCEL 2
#endif

#ifndef IDYES
#define IDYES 3
#endif

#ifndef IDNO
#define IDNO 4
#endif

typedef UINT zInt;
typedef UINT MESSAGE;
typedef DWORD PARM1;
typedef	long PARM2;


#define zSCROLLV WS_VSCROLL
#define zSCROLLH WS_HSCROLL
#define zSTDFRAME WS_OVERLAPPEDWINDOW
extern "C" {
typedef LONG (FAR PASCAL *WNDCASTPROC)(HWND,zInt,zInt,LONG);
}

extern HBRUSH _zAppGlobalBrush;


#ifndef VK_BUTTON1
#define VK_BUTTON1 VK_LBUTTON
#define VK_BUTTON2 VK_MBUTTON
#define VK_BUTTON3 VK_RBUTTON
#endif

#if !defined(VK_BACKSPACE)
#define VK_BACKSPACE VK_BACK
#endif

#if !defined(VK_NEWLINE)
#define VK_NEWLINE VK_RETURN
#endif

#define VK_CTRL VK_CONTROL
#define VK_CAPSLOCK VK_CAPITAL

#if !defined(VK_ESC)
#define VK_ESC VK_ESCAPE
#endif

#define VK_PAGEUP VK_PRIOR
#define VK_PAGEDOWN VK_NEXT
#define VK_PRINTSCRN VK_SNAPSHOT
#define VK_SCRLLOCK VK_SCROLL

// zDialogFrame::show() modifier flags NOT IMPLEMENTED ON THIS PLATFORM
#define SWM_MODAL_PRI    0  /* Modal relative to parent(PRImary) window */
#define SWM_MODAL_APP    0  /* Modal relative to entire application */
#define SWM_MODAL_SYS    0  /* Modal to entire system */
#define SWM_MODAL        SWM_MODAL_PRI   /* Synonym for SW_MODAL_PRI */
#define SWM_MASK         0  /* Mask of all show modifier flags */

#define APIEXPORT

#ifdef STRICT
#define ZREGION_HNDL HRGN
#else
#define ZREGION_HNDL HANDLE
#endif

extern "C" {
void Z_DLLVSPEC cdecl debug(char* s, ...);
void Z_DLLVSPEC Z_DLLVBSPEC cdecl debug(char* s, ...);
}
extern PRINTDLG *zpdlg;

#include <ole2.h>      
#include <mapi.h>

#endif
