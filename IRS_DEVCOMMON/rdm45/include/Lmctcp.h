/*
lmctcp.h
for use with TCP lock manager
Copyright (c) 1996, Raima Corporation. All rights reserved.
*/

#ifndef LMCTCP_H
#define LMCTCP_H

#ifndef LMCHDR_H
#define LMCHDR_H

#define MESSAGE_TABLES
#include "lmc.h"
#include<winsock.h>

#endif /* LMCHDR_H */

#define TCP_DEFAULT_SOCKET 1523

#define TCPERR_OKAY             0
#define TCPERR_DLLOPEN          1
#define TCPERR_BIGDLLNAME       2
#define TCPERR_SYSNOTREADY      3
#define TCPERR_VERNOTSUPPORTED  4
#define TCPERR_WSOCKUNK         5
#define TCPERR_LMIPNOTFOUND     6
#define TCPERR_CANTSOCKET       7
#define TCPERR_CANTCONNECT      8
#define TCPERR_BIGPACKET        9
#define TCPERR_TIMEOUT         10
#define TCPERR_NETFAIL         11
#define TCPERR_SHORTPACKET     12
#define TCPERR_SHORTSEND       13

#endif    /* LMCTCP_H */

/* vpp LMCTCP.H (Fri Sep 19 11:56:09 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

