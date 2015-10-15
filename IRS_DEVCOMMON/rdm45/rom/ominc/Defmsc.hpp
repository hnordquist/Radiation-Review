//---------------------------------------------------------
//  defmsc.h - Microsoft C/C++ definitions
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _DEFMSC_HPP_
#define _DEFMSC_HPP_

// quick check for correct compiler
#ifndef _MSC_VER
#error Wrong compiler. This file is intended for Microsoft C++ only.
#endif

// Raima Database Manager requires this define for Microsoft C++
#ifndef MSC
#define MSC
#endif

// Microsoft C++ uses (what we call) standard preprocessor concatenation
#define STD_CONCAT


#ifdef _WINDOWS

#ifndef WINDOWS
#define WINDOWS
#endif

#include <windows.h>

#define OM_FAR   _far
#define OM_NEAR  _near
#define OM_HUGE  _huge
#define OMLOADDS _loadds
#define OM_ERROREXPORT __export

// _WINDLL from /GD compile option
#if (defined(_WINDLL) || defined(DLL)) && !defined(USE_DLL)
#  define OM_EXPORT __export  // while creating a DLL
#else
#  define OM_EXPORT OM_FAR    // while using Object Manager as a DLL
#endif // _WINDLL

#else  // !_WINDOWS

// for Dos Library and application
#define OM_FAR    
#define OM_NEAR   
#define OM_HUGE   _huge
#define OMLOADDS  
#define OM_EXPORT 
#define OM_ERROREXPORT

#endif // _WINDOWS


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
#define om_halloc  _halloc
#define om_hfree   _hfree


#if defined(OM_RDS) && defined(__cplusplus)
#include <setjmp.h>
#endif // OM_RDS

// silence warnings
#pragma warning( disable:4147 )
#pragma warning( disable:4005 )  // _loadds on reference ...
#pragma warning( disable:4505 )  // unreferenced functions
#pragma warning( disable:4100 )  // unreferenced formal parameters 

#endif // _DEFMSC_HPP_
