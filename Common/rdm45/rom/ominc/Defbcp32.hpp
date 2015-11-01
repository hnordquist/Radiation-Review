//---------------------------------------------------------
//  defbcp32.h - Borland C++ definitions
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _DEFBCP32_HPP_
#define _DEFBCP32_HPP_

// quick check for correct compiler
#ifndef __BORLANDC__
#error Wrong compiler. This file is intended for Borland C++ only.
#endif

// Raima Database Manager requires this define for Borland C++
#ifndef TURBO
#define TURBO
#endif

#define STD_CONCAT

#ifndef WINDOWS
#define WINDOWS
#endif

#ifndef WIN32
#define WIN32
#endif

#ifndef _WIN32
#define _WIN32
#endif

#include <windows.h>

#define OM_FAR   
#define OM_NEAR  
#define OM_HUGE  
#define OMLOADDS 
#define OM_ERROREXPORT _export


#if (defined(__DLL__) || defined(DLL)) && !defined(USE_DLL)
#  define OM_EXPORT _export
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

#endif

