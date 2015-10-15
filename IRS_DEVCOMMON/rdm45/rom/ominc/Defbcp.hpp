//-----------------------------------------------------
//  defbcp.hpp - Borland C++ definitions
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _DEFBCP_HPP_
#define _DEFBCP_HPP_

// quick check for correct compiler
#if !defined(__BCPLUSPLUS__) && !defined(__BORLANDC__)
#error Wrong compiler. This file is intended for Borland C++ only.
#else
#if (__BCPLUSPLUS__ < 0x300) && !defined(__BORLANDC__)
#error Raima Object Manager requires Borland C++ 3.0 or greater
#endif
#endif

// Raima Database Manager requires this define for Borland C++
#ifndef TURBO
#define TURBO
#endif 

// Borland C++ uses (what we call) standard preprocessor concatenation
#define STD_CONCAT

#ifdef _Windows

#ifndef WINDOWS
#define WINDOWS
#endif

#include <windows.h>

#define OM_FAR   _far
#define OM_NEAR  _near
#define OM_HUGE  _huge
#define OMLOADDS _loadds      // always in Windows
#define OM_ERROREXPORT _export

// __DLL__ from -WD compile option
#if (defined(__DLL__) || defined(DLL)) && !defined(USE_DLL)
#  define OM_EXPORT _export   // while creating a DLL 
#else
#  define OM_EXPORT OM_HUGE   // while using Object Manager as a DLL
#endif // __DLL__

#else  // !_Windows

// for Dos Library and application
#define OM_FAR   
#define OM_NEAR  
#define OM_HUGE  _huge
#define OMLOADDS 
#define OM_ERROREXPORT

// Velocis requires this
#ifndef MSDOS
#define MSDOS
#endif

#ifdef FAR_VIRT
#define OM_EXPORT OM_HUGE
#else
#define OM_EXPORT OM_FAR
#endif

#endif // _Windows

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
#define om_halloc  farcalloc
#define om_hfree   farfree

#endif // _DEFBCP_HPP_
