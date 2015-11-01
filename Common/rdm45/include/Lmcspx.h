/*------------------------------------------------------------------------

   For use with SPX lockmgr.

   Copyright (c) 1992-1993, Raima Corporation, All Rights Reserved

--------------------------------------------------------------------------*/

#ifndef LMCSPX_H
#define LMCSPX_H

#ifndef LMCHDR_H
#define LMCHDR_H

#define MESSAGE_TABLES
#include "lmc.h"

#endif /* LMCHDR_H */

/* Values assigned to Raima Corp. by Novell in low-high format */
#define RAIMASPXSOCKET  0x823F   /* The socket for communicating with the lockmgr */
#define RAIMARDMOBJECT  0x0447   /* The lockmgr object type stored in the bindery */

#define U_OKAY          0
#define U_TIMEOUT       1
#define U_FAIL          2
#define U_NO_LOCKMGR    3
#define U_BADPACKET     4
#define U_MAX_MESSAGE   5
#define U_INV_ADDR      6
#define U_NO_ANSWER     7
#define U_NO_MEMORY     8



/*
   Do not send packets larger than 512 bytes, or else they may get split if
   the network contains a bridge / router.  A message consists of a message
   header followed by the message data.  This is then broken into packets of
   less than 512 bytes, each consisting of an LM_PACKET structure, and the
   a packet data.  The packet data is therefore be limited to <= 450 bytes.
*/
#define MAX_SIPX_DATA    450
#define MAX_SIPX_PACKET  (MAX_SIPX_DATA+LM_PACKETLEN)


#ifdef MSC
#define N_PLAT_MSW4
#else
Currently only Microsoft Visual C/C++ is supported
#endif

#include "ntypes.h"
#include "nwsipx32.h"


#define Int16Swap(w)   ((((w) << 8) | ((w) >> 8)) & 0x0ffff)
#define AVAILABLE(p) (p->TCBClientContext == NULL)
#define SET_BUSY(p)  (p->TCBClientContext = (nptr)1)
#define SET_FREE(p)  (p->TCBClientContext = NULL)

#define IPXWIN_FAR
#define IPXTASKID
#define IPXPASCAL

typedef nuint32 (__stdcall *NWSipxAcceptConnectionProc) (PNWTCB, SIPXCONN_HANDLE);
typedef nuint32 (__stdcall *NWSipxAcceptConnectionExProc) (PNWTCB, PSIPXCONN_HANDLE);
typedef nuint32 (__stdcall *NWSipxAdvertiseServiceProc) (nuint16, pnstr8, SIPXSOCK_HANDLE);
typedef nuint32 (__stdcall *NWSipxAllocControlBlockProc) (nuint32, nptr,	PPNWTCB);
typedef nuint32 (__stdcall *NWSipxCancelAdvertiseServiceProc) (nuint16, pnstr8, SIPXSOCK_HANDLE);
typedef nuint32 (__stdcall *NWSipxCancelPendingRequestProc) (PNWTCB);
typedef nuint32 (__stdcall *NWSipxCloseConnectionEndpointProc) (SIPXCONN_HANDLE);
typedef nuint32 (__stdcall *NWSipxEstablishConnectionProc) (PNWTCB);
typedef nuint32 (__stdcall *NWSipxFreeControlBlockProc) (PNWTCB);
typedef nuint32 (__stdcall *NWSipxFreeInformationProc) (nptr);
typedef nuint32 (__stdcall *NWSipxGetInformationProc) (nuint32, nptr, pnptr, pnuint32);
typedef nuint32 (__stdcall *NWSipxGetInternetAddressProc) (SIPXSOCK_HANDLE, PNETADDR);
typedef nuint32 (__stdcall *NWSipxGetMaxTsduSizeProc) (SIPXCONN_HANDLE);
typedef nuint32 (__stdcall *NWSipxListenForConnectionProc) (PNWTCB);
typedef nuint32 (__stdcall *NWSipxOpenConnectionEndpointProc) (SIPXSUBNET_HANDLE, pnuint16, PSIPXCONN_HANDLE);
typedef nuint32 (__stdcall *NWSipxQueryServicesProc) (nuint16, nuint16, PNWTCB);
typedef nuint32 (__stdcall *NWSipxReceiveMessageProc) (PNWTCB);
typedef nuint32 (__stdcall *NWSipxSendMessageProc) (PNWTCB);
typedef nuint32 (__stdcall *NWSipxTerminateConnectionProc) (PNWTCB);
typedef nuint32 (__stdcall *NWSipxWaitForSingleEventProc) (PNWTCB, nuint32);
typedef nuint32 (__stdcall *NWSipxWaitForMultipleEventsProc) (SIPXMUXGRP_HANDLE, nuint32, PPNWTCB);


#endif /* LMCSPX_H */

/* $Revision:   1.68  $ */

/* vpp LMCSPX.H (Fri Sep 19 11:56:09 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

