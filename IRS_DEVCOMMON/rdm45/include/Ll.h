/*----------------------------------------------------------------------
   ll.h -- linked list function structures and prototypes

   The functions in this file are defined in alloc.c

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef LL_H
#define LL_H

#include "packwin.h"

typedef struct
{
   struct ll_elem DB_FAR *ptr;
   LOCK_DESC
}  LL_P;

typedef struct ll_elem
{
   LL_P next;
   CHAR_P data;
}  ll_elem;

#define LL_ELEM_INIT()  { POINTER_INIT(), POINTER_INIT() }

typedef struct
{
   LL_P head;
   LL_P tail;
   LL_P DB_FAR *curr;
}  llist;

#define LLIST_INIT()    { POINTER_INIT(), POINTER_INIT(), NULL }

#include "nopack.h"

DB_BOOLEAN  INTERNAL_FIXED ll_access(P1(llist DB_FAR *));
int      INTERNAL_FIXED ll_append(P1(llist DB_FAR *) Pi(CHAR_P DB_FAR *));
void     INTERNAL_FIXED ll_deaccess(P1(llist DB_FAR *));
void     INTERNAL_FIXED ll_free(P1(llist DB_FAR *));
int      INTERNAL_FIXED ll_prepend(P1(llist DB_FAR *) Pi(CHAR_P DB_FAR *));
CHAR_P DB_FAR *   INTERNAL_FIXED ll_first(P1(llist DB_FAR *));
CHAR_P DB_FAR *   INTERNAL_FIXED ll_next(P1(llist DB_FAR *));

#endif

/* $Revision:   1.6  $ */

/* vpp LL.H (Fri Sep 19 11:56:10 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

