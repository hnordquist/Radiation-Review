/*------------------------------------------------------------------------

   For use with Novell lockmgr & TTS.

   Copyright (c) 1992-1993, Raima Corporation, All Rights Reserved

--------------------------------------------------------------------------*/

#ifndef NOVFCNS_H
#define NOVFCNS_H

#ifndef LMCHDR_H
#define LMCHDR_H

#define MESSAGE_TABLES
#include "lmc.h"

#endif /* LMCHDR_H */

#define W1(t)        t
#define W2(t)        t,


#ifdef MSC
#define N_PLAT_MSW4
#else
Currently only Microsoft Visual C/C++ is supported
#endif

#define NWALIAS_H
#define NOAFP_INC
#define NOBINDRY_INC
#define NODEL_INC
#define NODENTRY_INC
#define NOEA_INC
#define NOERROR_INC
#define NOMESSAGES_INC
#define NONAMSPC_INC
#define NOPRINT_INC
#define NOQUEUE_INC
#define NOSYNC_INC
#define NOVOL_INC
#define NOACCT_INC
#define NOFSE_INC
#define NOMIGRATE_INC
#include <nwcalls.h>


typedef NWCCODE (__stdcall *NWCallsInitProc) (nptr, nptr);
typedef NWCCODE (__stdcall *NWGetPrimaryConnectionIDProc) (NWCONN_HANDLE N_FAR *);
typedef NWCCODE (__stdcall *NWTTSBeginTransactionProc) (NWCONN_HANDLE);
typedef NWCCODE (__stdcall *NWTTSAbortTransactionProc) (NWCONN_HANDLE);
typedef NWCCODE (__stdcall *NWTTSEndTransactionProc) (NWCONN_HANDLE, pnuint32);
typedef NWCCODE (__stdcall *NWTTSAvailableProc) (NWCONN_HANDLE);
typedef NWCCODE (__stdcall *NWGetExtendedFileAttributes2Proc) (NWCONN_HANDLE, NWDIR_HANDLE, pnstr8, pnuint8);
typedef NWCCODE (__stdcall *NWParseNetWarePathProc) (pnstr8, NWCONN_HANDLE N_FAR *, NWDIR_HANDLE N_FAR *, pnstr8);
typedef NWCCODE (__stdcall *NWLockLogicalRecordSetProc) (nuint8, nuint16);
typedef NWCCODE (__stdcall *NWLogLogicalRecordProc) (NWCONN_HANDLE, pnstr8, nuint8, nuint16);
typedef NWCCODE (__stdcall *NWClearLogicalRecordProc) (NWCONN_HANDLE, pnstr8);
typedef NWCCODE (__stdcall *NWSetExtendedFileAttributes2Proc) (NWCONN_HANDLE, NWDIR_HANDLE, pnstr8, nuint8);
typedef NWCCODE (__stdcall *NWTTSSetProcessThresholdsProc) (NWCONN_HANDLE, nuint8, nuint8);
typedef NWCCODE (__stdcall *NWTTSSetControlFlagsProc) (NWCONN_HANDLE, nuint8);
typedef NWCCODE (__stdcall *NWGetFileServerInformationProc) (NWCONN_HANDLE,
                                    pnstr8, pnuint8, pnuint8, pnuint8,
                                    pnuint16, pnuint16, pnuint16, pnuint16,
                                    pnuint8, pnuint8);



#ifdef NOVFCNS_C
#define EXTERN /**/
#else
#define EXTERN extern
#endif

EXTERN NWCallsInitProc                   fNWCallsInit;
EXTERN NWGetExtendedFileAttributes2Proc  fNWGetExtendedFileAttributes2;
EXTERN NWParseNetWarePathProc            fNWParseNetWarePath;
EXTERN NWTTSBeginTransactionProc         fNWTTSBeginTransaction;
EXTERN NWTTSAbortTransactionProc         fNWTTSAbortTransaction;
EXTERN NWTTSEndTransactionProc           fNWTTSEndTransaction; 
EXTERN NWTTSSetProcessThresholdsProc     fNWTTSSetProcessThresholds;
EXTERN NWTTSSetControlFlagsProc          fNWTTSSetControlFlags;
EXTERN NWSetExtendedFileAttributes2Proc  fNWSetExtendedFileAttributes2;
EXTERN NWLogLogicalRecordProc            fNWLogLogicalRecord;
EXTERN NWLockLogicalRecordSetProc        fNWLockLogicalRecordSet;
EXTERN NWClearLogicalRecordProc          fNWClearLogicalRecord;
EXTERN NWGetFileServerInformationProc    fNWGetFileServerInformation;

#define NWCallsInit                    (*fNWCallsInit)
#define NWGetExtendedFileAttributes2   (*fNWGetExtendedFileAttributes2)
#define NWParseNetWarePath             (*fNWParseNetWarePath)
#define NWTTSBeginTransaction          (*fNWTTSBeginTransaction)
#define NWTTSAbortTransaction          (*fNWTTSAbortTransaction)
#define NWTTSEndTransaction            (*fNWTTSEndTransaction)    
#define NWTTSSetProcessThresholds      (*fNWTTSSetProcessThresholds)
#define NWTTSSetControlFlags           (*fNWTTSSetControlFlags)
#define NWSetExtendedFileAttributes2   (*fNWSetExtendedFileAttributes2)
#define NWLogLogicalRecord             (*fNWLogLogicalRecord)
#define NWLockLogicalRecordSet         (*fNWLockLogicalRecordSet)
#define NWClearLogicalRecord           (*fNWClearLogicalRecord)
#define NWGetFileServerInformation     (*fNWGetFileServerInformation)
#undef EXTERN


int  INTERNAL_FIXED NWInitNetware(void);
int  INTERNAL_FIXED NWCloseNetware(void);
int  INTERNAL_FIXED NWFileServerID(char DB_FAR *, NWCONN_HANDLE DB_FAR *);
DB_WORD INTERNAL_FIXED NWSetError(char DB_FAR *fn, DB_WORD code);
char        DB_FAR *NWGetError(int errnum);


#endif /* NOVFCNS_H */

/* $Revision:   1.7  $ */

/* vpp NOVFCNS.H (Fri Sep 19 11:56:09 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

