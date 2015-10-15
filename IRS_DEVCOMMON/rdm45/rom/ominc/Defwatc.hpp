//---------------------------------------------------------
//  defwatc.h - Microsoft C/C++ definitions
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _DEFWATC_HPP_
#define _DEFWATC_HPP_

// quick check for correct compiler
#ifndef __WATCOMC__
#error Wrong compiler. This file is intended for Watcom C++ only
#endif

#ifndef WATC
#define WATC
#endif

// Watcom C++ uses (what we call) standard preprocessor concatenation
#define STD_CONCAT

#if defined(__WINDOWS__) || defined(__WINDOWS_386__) && !defined(__NETWARE_386__)

#error Watcom is not a supported client compiler yet.
#error Call Raima at 1(800)DB-RAIMA if you would like it to be supported.

#ifndef WINDOWS
#define WINDOWS
#endif

#include <windows.h>

#endif

#define OM_FAR
#define OM_NEAR
#define OM_HUGE
#define OMLOADDS

//// _WINDLL from /GD compile option
//#if (defined(_WINDLL) || defined(DLL)) && !defined(USE_DLL)
//#  define OM_EXPORT __export  // while creating a DLL
//#else
//#  define OM_EXPORT OM_FAR    // while using Object Manager as a DLL
//#endif // _WINDLL

#define om_memcpy  _fmemcpy
#define om_memset  _fmemset
#define om_strcpy  _fstrcpy
#define om_strdup  _fstrdup
#define om_strncpy _fstrncpy
#define om_strcmp  _fstrcmp
#define om_strcat  _fstrcat
#define om_strlen  _fstrlen
#define om_calloc  _fcalloc
#define om_free    _ffree
#define om_strchr  _fstrchr
#define om_halloc  halloc
#define om_hfree   hfree


#if defined(OM_RDS) && defined(__cplusplus)
/////////////////////////////////////////////////////////////////////////////
// setjmp for Windows and C++
typedef  int  jmp_buf[9];
extern "C" {
int  __cdecl setjmp(jmp_buf);
void __cdecl longjmp(jmp_buf, int);
}
#endif // OM_RDS

#endif // _DEFWATC_HPP_
