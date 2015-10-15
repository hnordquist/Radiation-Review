//---------------------------------------------------------
//  defibmc.h - IBM C-SET/2 definitions
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _DEFIBMC_HPP_
#define _DEFIBMC_HPP_

#if (defined(__IBMCPP__) && (__IBMCPP__ < 200)) || (defined(__IBMC__) && (__IBMC__ < 200))
#error Wrong compiler. IBM C-SET/2 2.0 or above is required.
#endif

// IBM C-SET/2 uses (what we call) standard preprocessor concatenation
#define STD_CONCAT

#ifndef OS2
#define OS2
#endif

#define OM_FAR   
#define OM_NEAR  
#define OM_HUGE  
#define OMLOADDS 
#define OM_ERROREXPORT _Export

#if (defined(__DLL__) || defined(DLL)) && !defined(USE_DLL)
#  define OM_EXPORT _Export
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

#ifdef OM_RDM
extern "C" {
// these are required for CSet to avoid warnings
struct TASK_S;
struct NODE_PATH_S;
struct NODE_PATH_S ;
struct LM_LOCK_S ;
struct LM_FREE_S ;
struct LM_DBOPEN_S ;
struct LM_DBCLOSE_S ;
struct LR_DBOPEN_S ;
struct KEY_INFO_S ;
struct ren_entry ;
struct page_entry ;
struct recently_used_s;
struct FILE_ENTRY_S ;
struct DB_ENTRY_S ;
struct RN_ENTRY_S ;
struct RECORD_ENTRY_S ;
struct SET_ENTRY_S ;
struct MEMBER_ENTRY_S ;
struct SORT_ENTRY_S ;
struct FIELD_ENTRY_S ;
struct KEY_ENTRY_S ;
struct PGZERO_S ;
struct RI_ENTRY_S ;
struct CNTRY_TBL_S ;
struct CACHE_S ;
struct ll_elem;
}
#endif

#endif
