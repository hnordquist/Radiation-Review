//-----------------------------------------------------
//  defbcos2.hpp - Borland C++ for OS/2 definitions
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _DEFBCOS2_HPP_
#define _DEFBCOS2_HPP_

// quick check for correct compiler
#if (defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ < 0x320)) || !defined(__BORLANDC__)
#error Wrong compiler. Borland C++ 3.20 or above is required.
#endif

// Raima Database Manager requires this define 
#ifndef TURBO
#define TURBO
#endif

#ifndef OS2
#define OS2
#endif 

// Borland C++ uses (what we call) standard preprocessor concatenation
#define STD_CONCAT

// OS/2 has no far or huge class
#define OM_FAR    
#define OM_NEAR   
#define OM_HUGE   
#define OMLOADDS  
#define OM_ERROREXPORT _export

#if (defined(__DLL__) || defined(DLL)) && !defined(USE_DLL)
#  define OM_EXPORT _export
#else
#  define OM_EXPORT OM_FAR
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
