/*----------------------------------------------------------------------
   lmncb.h -- Network Control Block definitions for NetBIOS

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef LMCNETB_H
#define LMCNETB_H

#ifndef LMCHDR_H
#define LMCHDR_H

#define MESSAGE_TABLES
#include "lmc.h"

#endif


#define MAX_NETB_DATA         512
#define MAX_NETB_PACKET       (MAX_NETB_DATA+LM_PACKETLEN)

#include "nb30.h"

/*
   The Netbios error codes used by RDM are mostly the ones defined in
   nb30.h, except for the following, which are RDM-specific errors
*/
#define N_TOOLONG      0x60
#define N_BADNAME      0x61
#define N_BADPACKET    0x62
#define N_SIZE         0x63
#define N_NCBS         0x64
#define N_NOSENDBUF    0x65
#define N_NOMEMORY     0x66
#define N_NULLBUF      0x67

#endif

/* $Revision:   1.17  $ */

/* vpp LMCNETB.H (Fri Sep 19 11:56:09 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

