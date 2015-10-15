/*----------------------------------------------------------------------------
   inittab.h: db_VISTA header file containing:

      - Compatible dictionary file versions

   This file is intended to be included in inittab.c only.  There are
   multiple versions of inittab.c which share this header.

   (An #include "vista.h" must precede this include)

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef INITTAB_H
#define INITTAB_H

/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
  101 21-Jun-88 RSC Added decl for rn_type and rn_dba for ONE_DB
      08-Aug-88 RTK Moved rn_dba and rn_type to vista.h - needed in rec*.c
      18-Aug-88 RSC Moved rn_type/dba to separate table, add curr_rn_table..
 1430 30-Oct-91 DMB Full support of context switching using multiple caches

*/



/* Compatible dictionary versions */
char *compat_dbd[] = {
   dbd_VERSION
};
int size_compat = sizeof(compat_dbd) / sizeof(*compat_dbd);

#endif

/* $Revision:   1.6  $ */

/* vpp INITTAB.H (Fri Sep 19 11:56:09 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

