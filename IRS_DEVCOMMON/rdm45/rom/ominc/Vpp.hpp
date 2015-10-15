/*---------------------------------------------------------
//  VPP.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------*/
#ifndef _VPP_HPP_
#define _VPP_HPP_

#if !defined(OM_RDS) && !defined(OM_RDM)
#error Either OM_RDS or OM_RDM must be defined!
#endif

/*/////////////////////////////////////////////////////////////
// Microsoft specific definitions */
#ifdef _MSC_VER
#  ifdef _WIN32
#    include "defmsc32.hpp"
#  else
#    include "defmsc.hpp"
#    define OM_16BIT
#  endif
#endif

/*/////////////////////////////////////////////////////////////
// Borland specific definitions */
#if defined(__BCPLUSPLUS__) || defined(__BORLANDC__)
#  if defined(__OS2__)
#    include "defbcos2.hpp"
#  else
#    ifdef __WIN32__
#      include "defbcp32.hpp"
#    else
#      include "defbcp.hpp"
#      define OM_16BIT
#    endif /* _WIN32 */
#  endif /* OS/2 or DOS-WIN? */
#endif /* BCP definitions */

/*/////////////////////////////////////////////////////////////
// IBMCPP specific definitions */
#if (defined(__IBMCPP__) || defined(__IBMC__)) && defined(__OS2__)
#   include "defibmc.hpp"
#endif

/*/////////////////////////////////////////////////////////////
// WATCOM specific definitions for building ROM SE as NLM */
#if defined (__WATCOMC__)
#   include "defwatc.hpp"
#endif

/*/////////////////////////////////////////////////////////////
// UNIX specific definitions */
#ifdef UNIX
#include "defunx.hpp"
#endif

/* /////////////////////////////////////////////////////////// */

#ifndef OM_FAR
#error  Your compiler has not been detected by ROM and is not supported
#endif

#include "concat.hpp"

OMTYPE(char);
OMTYPE(short);
OMTYPE(int);
OMTYPE(long);
OMTYPE(float);
OMTYPE(double);
typedef unsigned char  UCHAR;
typedef unsigned short USHORT;
typedef unsigned long  ULONG;
OMTYPE(UCHAR);
OMTYPE(USHORT);
OMTYPE(ULONG);

typedef void OM_FAR        * Pvoid;
typedef const void OM_FAR  * CPvoid;
typedef char OM_HUGE       * Phugechar;
typedef const char OM_HUGE * CPhugechar;
typedef void OM_HUGE       * Phugevoid;
typedef const void OM_HUGE * CPhugevoid;

/*/////////////////////////////////////////////////////////////
// Velocis specific definitions */
#if defined(OM_RDS)
#  if defined(WIN32) && !defined(WIN32_NT)
#    define WIN32_NT
#  endif
#  include <rds.h>

#ifdef NULL
#undef NULL
#define NULL 0
#endif

#  define SET_TYPE   short
#  define REC_TYPE   short
#  define PREC_TYPE  Pshort
#  define INDX_TYPE  short
#  define FLD_TYPE   long
#  define S_NOTFREE  -100

#  define OMEXTERNAL REXTERNAL OM_ERROREXPORT OMLOADDS
#endif /* OM_RDS */

/*/////////////////////////////////////////////////////////////
// Raima Database Manager specific definitions */
#if defined(OM_RDM)
#  define MULTI_TASK
#  define NO_DT_COVER
#  ifdef __cplusplus
     extern "C" {
#  endif
#    ifndef WINDOWS
#      include <stdio.h>
#    endif
#    include <vista.h>
#    ifdef NULL
#      undef NULL
#      define NULL 0
#    endif

#  ifdef __cplusplus
     }
#  endif

   typedef DB_TASK DB_FAR * PDB_TASK;

#  define SET_TYPE  int
#  define REC_TYPE  int
#  define PREC_TYPE Pint
#  define INDX_TYPE int
#  define FLD_TYPE  long

#  define OMEXTERNAL EXTERNAL_FIXED OM_ERROREXPORT OMLOADDS
#endif /* OM_RDM */

/* both RDM and RDS have a DB_ADDR */
OMTYPE(DB_ADDR);

/* for compatibility with ROM 1.x and 2.0 */
#if !defined(DB_FAR)
#  define DB_FAR  OM_FAR
#endif

/* for compatibility with ROM 1.x and 2.0 */
#if !defined(DB_NEAR)
#  define DB_NEAR  OM_NEAR
#endif

/* for compatibility with ROM 1.x and 2.0 */
#if !defined(RFAR)
#  define RFAR  OM_FAR
#endif

/* for compatibility with ROM 1.x and 2.0 */
#if !defined(RNEAR)
#  define RNEAR  OM_NEAR
#endif


#ifdef WINDOWS
#  if defined(WINVER) && defined(STRICT)
#    define OMHINSTANCE HINSTANCE
#  else
#    define OMHINSTANCE HANDLE
#  endif
#endif


#if defined(__OWL_H) || defined(__OWLHDR_H)
#  define T_F Boolean
#else
enum T_F { False=0, True };
#endif

#define POMNULL ((Pvoid)NULL)
#define SYSTEM 9999  /* for Vista's .h files */

/* NavCtrl Used in the [] overloaded operator
// Consider Separating NavCtrls to seperate enums.
// This would enable type checking to do direct function calls instead
// of doing a "type check" */
#undef NEXT
#undef LAST
#undef FIRST
enum NavCtrl { FIRST=1, LAST, NEXT, PREV };

#ifdef __cplusplus
#  include "verrout.hpp"
#endif

#endif /* _VPP_HPP_ */


