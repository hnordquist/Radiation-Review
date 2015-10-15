/*----------------------------------------------------------------------
   dblock.h -- runtime/lockmgr common definitions and structures

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef DBLOCK_H
#define DBLOCK_H

/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
  368 28-Jul-88 RSC Integrate BSD changes into code
  115 16-Aug-88 RSC Integrate VAX/VMS changes into source code
  423 09-Sep-88 RSC Change variables to be compatible with MULTI_TASK
  420 07-Oct-88 RSC Unoptimized use of fl_list (full of bugs)
  441 09-Dec-88 RSC Modified defn of FCNPID for general lockmgr
      10-Sep-91 TWP Added NetBIOS error code for CANCEL'd NCB.
*/

/* Lock Manager/Runtime function interaction */

/* Function/Status codes */
#if MSGVER >= 0x331
#define L_BADTAF       -9
#endif
#define L_PENDING      -8
#define L_LOCKHANDLE   -7
#define L_DUPUSER      -6
#define L_RECOVER      -5
#define L_QUEUEFULL    -4
#define L_TIMEOUT      -3
#define L_UNAVAIL      -2
#define L_SYSERR       -1

#define L_OKAY          0

/* mtype values for message to lockmgr */
#define L_EMPTY         0
#define L_DBOPEN        1
#define L_DBCLOSE       2
#define L_LOCK          3
#define L_FREE          4
#define L_TRCOMMIT      5
#define L_TREND         6
#define L_SETTIME       7
#define L_RECDONE       8
#define L_RECFAIL       9
#define L_LOGIN        10
#define L_LOGOUT       11
#define L_TERMINATE    12
#define L_OPTIONS      13
#define L_VERIFYUSER   14
#define L_CLEARUSER    15
#define L_STATUS       16

#define L_LAST         16  /* this must be updated when a new message is added */

/* sub-type values for the L_STATUS message */
#define ST_GENSTAT    0
#define ST_TABLES     1
#define ST_USERINFO   2
#define ST_FILEINFO   3
#define ST_USERSTAT   4
#define ST_TAFTAB     5

/* lock manager types from L_LOGIN message */
#define LM_TSR        'T'
#define LM_DEDICATED  'D'
#define LM_WINDOWS    'W'


typedef unsigned short LM_REF;

typedef struct
{
   LM_REF DB_FAR *ptr;
   LOCK_DESC
} LM_REF_P;

#define WORDPAD(a) ((sizeof(int)-(a)%sizeof(int))%sizeof(int))

#include "packnet.h"

typedef struct DB_LOCKREQ_S
{
   LM_REF         fref;
   short          type;
}  DB_LOCKREQ;

typedef struct LM_TABSIZE_S
{
   LM_REF         lm_maxusers;
   LM_REF         lm_maxfiles;
   LM_REF         lm_maxqueue;
   LM_REF         lm_numusers;
   LM_REF         lm_numfiles;
   DB_USHORT      lm_userbytes;
   DB_USHORT      lm_filebytes;
   LM_REF         lm_maxtafs;
   LM_REF         lm_numtafs;
} LM_TABSIZE;


/* ==========================================================================
   Message packets, Runtime to Lock Manager
*/

#define LM_HDR                      /* no header currently required */

typedef struct LM_MESSAGE_S
{
   LM_HDR
   char           mtext[1];
} LM_MESSAGE;

typedef struct LM_LOGIN_S
{
   LM_HDR
#if MSGVER >= 0x331
   DB_USHORT      version;       /* inform lockmgr of runtime version */
#endif
   char           dbusrid[80];
}  LM_LOGIN;

typedef struct LM_OPTIONS_S
{
   LM_HDR
#if MSGVER >= 0x331
   DB_ULONG       options;
#endif
}  LM_OPTIONS;

typedef struct LM_DBOPEN_S
{
   LM_HDR
   LM_REF         nfiles;
   short          type;
   char           fnames[1];
}  LM_DBOPEN;

typedef struct LM_DBCLOSE_S
{
   LM_HDR
   LM_REF         nfiles;
   LM_REF         frefs[1];
}  LM_DBCLOSE;

typedef struct LM_LOCK_S
{
   LM_HDR
   LM_REF         nfiles;
   DB_LOCKREQ     locks[1];
}  LM_LOCK;

typedef struct LM_RECOVERED_S
{
   LM_HDR
   char           mtext[1];
}  LM_RECOVERED;

typedef struct LM_RECFAIL_S
{
   LM_HDR
   char           mtext[1];
}  LM_RECFAIL;

typedef struct LM_FREE_S
{
   LM_HDR
   LM_REF         nfiles;
   LM_REF         frefs[1];
}  LM_FREE;

typedef struct LM_TRCOMMIT_S
{
   LM_HDR
   char           logfile[LOGFILELEN];
}  LM_TRCOMMIT;

typedef struct LM_TREND_S
{
   LM_HDR
   char           mtext[1];
}  LM_TREND;

typedef struct LM_SETTIME_S
{
   LM_HDR
   short          queue_timeout;
#if MSGVER >= 0x331
   short          lock_timeout;
#endif
}  LM_SETTIME;

typedef struct LM_LOGOUT_S
{
   LM_HDR
   char           mtext[1];
}  LM_LOGOUT;

typedef struct LM_TERMINATE_S
{
   LM_HDR
   char           mtext[1];
}  LM_TERMINATE;

typedef struct LM_VERIFYUSER_S
{
   LM_HDR
   char           dbusrid[80];
}  LM_VERIFYUSER;

typedef struct LM_CLEARUSER_S
{
   LM_HDR
   char           dbusrid[80];
}  LM_CLEARUSER;

typedef struct LM_STATUS_S
{
   LM_HDR
   LM_REF         type_of_status;
   LM_REF         number;
   char           username[16];
   LM_TABSIZE     tabsize;
}  LM_STATUS;


/* ==========================================================================
   Message packets, Lock Manager to Runtime
*/

#define LR_HDR short status; \
               char logfile[LOGFILELEN]; /* header for each receive packet */

typedef struct LR_MESSAGE_S
{
   LR_HDR
   char           mtext[1];
} LR_MESSAGE;

typedef struct LR_LOGIN_S
{
   LR_HDR
#if MSGVER >= 0x331
   DB_USHORT      lm_version;    /* version of the lockmgr */
   DB_SHORT       lm_type;       /* TSR, dedicated, Windows, etc. */
   DB_ULONG       lm_caps;       /* lock manager capabilities */
   LM_REF         nadd;
#endif
   LM_REF         uid;
}  LR_LOGIN;

typedef struct LR_OPTIONS_S
{
   LR_HDR
}  LR_OPTIONS;

typedef struct LR_DBOPEN_S
{
   LR_HDR
   LM_REF         nusers;
   LM_REF         nfiles;
   LM_REF         frefs[1];
}  LR_DBOPEN;

typedef struct LR_LOCK_S
{
   LR_HDR
#ifdef FILE_SEQ
   LM_REF         nfiles;
   DB_FILE_SEQ    file_seq[1];
#endif
}  LR_LOCK;

typedef struct LR_TRCOMMIT_S
{
   LR_HDR
}  LR_TRCOMMIT;

typedef struct LR_STATUS_S
{
   LR_HDR
   LM_TABSIZE     s_tabsize;
}  LR_STATUS;

typedef struct LR_TABLES_S
{
   LR_HDR
   char           mtext[1];
}  LR_TABLES;

typedef struct LR_USERINFO_S
{
   LR_HDR
   char           ui_name[16];
   LM_REF         ui_pid;
   LM_REF         ui_pending;
   short          ui_timer;
   short          ui_timeout;
   short          ui_status;
   LM_REF         ui_recovering;
   char           ui_taffile[FILENMLEN];
   char           ui_logfile[LOGFILELEN];
   char           ui_netinfo1[40];
   char           ui_netinfo2[40];
   char           ui_netinfo3[40];
   char           mtext[1];
}  LR_USERINFO;

typedef struct LR_FILEINFO_S
{
   LR_HDR
   char           fi_name[TRUENMLEN];
   short          fi_lockstat;
   LM_REF         fi_queue;
   char           mtext[1];
} LR_FILEINFO;

typedef struct LR_USERSTAT_S
{
   LR_HDR
   short          us_status;
} LR_USERSTAT;

typedef struct LR_TAFTAB_S
{
   LR_HDR
   char           mtext[1];
} LR_TAFTAB;

#include "nopack.h"

#endif /* DBLOCK_H */

/* $Revision:   1.55  $ */

/* vpp DBLOCK.H (Fri Sep 19 11:56:08 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

