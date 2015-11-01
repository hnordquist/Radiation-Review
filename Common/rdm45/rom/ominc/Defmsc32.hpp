//---------------------------------------------------------
//  defmsc32.h - Microsoft C/C++ definitions
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _DEFMSC32_HPP_
#define _DEFMSC32_HPP_

// quick check for correct compiler
#ifndef _MSC_VER
#error Wrong compiler. This file is intended for Microsoft C++ only.
#endif

// Raima Database Manager requires this define for Microsoft C++
#ifndef MSC
#define MSC
#endif

#define STD_CONCAT

#ifndef WINDOWS
#define WINDOWS
#endif

#ifndef WIN32
#define WIN32
#endif

#include <windows.h>

#define OM_FAR   
#define OM_NEAR  
#define OM_HUGE  
#define OMLOADDS 
#define OM_ERROREXPORT 

#if defined(_DLL) && !defined(USE_DLL) // while compiling Object Manager as a DLL 
#  define OM_EXPORT __declspec(dllexport)
#else
#  define OM_EXPORT
#endif

#define om_memcpy  memcpy
#define om_memset  memset
#define om_strcpy  strcpy
#define om_strdup  strdup
#define om_strncpy strncpy
#define om_strcmp  strcmp
#define om_strcat  strcat
#define om_strlen  strlen
#define om_calloc  calloc
#define om_free    free
#define om_strchr  strchr
#define om_halloc  calloc
#define om_hfree   free

#if defined(OM_RDS) && defined(__cplusplus)
#include <setjmp.h>
#endif // OM_RDS

#pragma warning( disable: 4100 4514)

#endif
