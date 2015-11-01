/*---------------------------------------------------------
//  defunx.hpp
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//--------------------------------------------------------- */
#ifndef _DEFUNX_HPP_
#define _DEFUNX_HPP_

#ifndef ANSI
#define ANSI
#endif
#ifndef LINT_ARGS
#define LINT_ARGS
#endif

/* these don't apply to Unix */
#define OM_FAR 
#define OM_NEAR
#define OM_EXPORT
#define OM_HUGE 
#define OM_ERROREXPORT 
#define OMLOADDS 

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


#if defined(HPUX) || defined(AIX)
#define STD_CONCAT
#else
#ifdef  BSD
#define BSD_CONCAT
#else
#ifdef  SCO
#define SCO_CONCAT
#else
#define SYSV_CONCAT
#endif
#endif
#endif


#endif /* _DEFUNX_HPP_ */

