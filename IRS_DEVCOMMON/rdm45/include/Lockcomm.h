/*----------------------------------------------------------------------
  lockcomm.h: LockManger Communication header file, included by vista.h

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#define MAX_LOCKCOMM  13    /*  This must be increased if a type is added */

typedef int (INTERNAL_FIXED *COMM_FUNC)(P1(int) TASK_Di);
typedef int (INTERNAL_FIXED *LMCERR_FUNC)(P1(char DB_FAR *) Pi(int) Pi(int));

typedef struct COMM_TYPES_S
{
   char DB_FAR *name;
   COMM_FUNC    comm_func;
   LMCERR_FUNC  err_func;
} COMM_TYPES;

#ifdef MULTI_TASK

extern int INTERNAL_FIXED lmcnonet_lockcomm(P1(int) TASK_Di);
extern int INTERNAL_FIXED lmcusrt_lockcomm(P1(int) TASK_Di);
extern int INTERNAL_FIXED lmcnetbt_lockcomm(P1(int) TASK_Di);
extern int INTERNAL_FIXED lmcit_lockcomm(P1(int) TASK_Di);
extern int INTERNAL_FIXED lmcgt_lockcomm(P1(int) TASK_Di);
extern int INTERNAL_FIXED lmcspxt_lockcomm(P1(int) TASK_Di);
extern int INTERNAL_FIXED tcpt_lockcomm(P1(int) TASK_Di);
extern int INTERNAL_FIXED ipt_lockcomm(P1(int) TASK_Di);
extern int INTERNAL_FIXED semt_lockcomm(P1(int) TASK_Di);
extern int INTERNAL_FIXED lmcvt_lockcomm(P1(int) TASK_Di);
extern int INTERNAL_FIXED lmcqt_lockcomm(P1(int) TASK_Di);
extern int INTERNAL_FIXED lmcnovt_lockcomm(P1(int) TASK_Di);

#else /* MULTI_TASK */

extern int INTERNAL_FIXED lmcnone_lockcomm(P1(int));
extern int INTERNAL_FIXED lmcusr_lockcomm(P1(int));
extern int INTERNAL_FIXED lmcnetb_lockcomm(P1(int));
extern int INTERNAL_FIXED lmci_lockcomm(P1(int));
extern int INTERNAL_FIXED lmcg_lockcomm(P1(int));
extern int INTERNAL_FIXED lmcspx_lockcomm(P1(int));
extern int INTERNAL_FIXED tcp_lockcomm(P1(int));
extern int INTERNAL_FIXED ip_lockcomm(P1(int));
extern int INTERNAL_FIXED sem_lockcomm(P1(int));
extern int INTERNAL_FIXED lmcv_lockcomm(P1(int));
extern int INTERNAL_FIXED lmcq_lockcomm(P1(int));
extern int INTERNAL_FIXED lmcnov_lockcomm(P1(int));

#endif /* MULTI_TASK */

extern int INTERNAL_FIXED lmcnone_errstr(P1(char DB_FAR *) Pi(int) Pi(int));
extern int INTERNAL_FIXED lmcusr_errstr(P1(char DB_FAR *) Pi(int) Pi(int));
extern int INTERNAL_FIXED lmcnetb_errstr(P1(char DB_FAR *) Pi(int) Pi(int));
extern int INTERNAL_FIXED lmci_errstr  (P1(char DB_FAR *) Pi(int) Pi(int));
extern int INTERNAL_FIXED lmcg_errstr  (P1(char DB_FAR *) Pi(int) Pi(int));
extern int INTERNAL_FIXED lmcspx_errstr(P1(char DB_FAR *) Pi(int) Pi(int));
extern int INTERNAL_FIXED tcp_errstr   (P1(char DB_FAR *) Pi(int) Pi(int));
extern int INTERNAL_FIXED ip_errstr    (P1(char DB_FAR *) Pi(int) Pi(int));
extern int INTERNAL_FIXED sem_errstr   (P1(char DB_FAR *) Pi(int) Pi(int));
extern int INTERNAL_FIXED lmcv_errstr  (P1(char DB_FAR *) Pi(int) Pi(int));
extern int INTERNAL_FIXED lmcq_errstr  (P1(char DB_FAR *) Pi(int) Pi(int));
extern int INTERNAL_FIXED lmcnov_errstr(P1(char DB_FAR *) Pi(int) Pi(int));


#ifdef DB_DLL
#ifndef NO_DT_COVER
#undef LOCKCOMM_DATA
#endif
#endif


/* Definition to have structure compiled by application insted of runtime */
#ifndef LOCKCOMM_DATA

extern COMM_TYPES DB_HUGE lockcomm_type[MAX_LOCKCOMM];

extern int (INTERNAL_FIXED *rdm_errstr)(P1(char DB_FAR *) Pi(int) Pi(int));
extern int (INTERNAL_FIXED *initFromIniFile)(P0);
extern int (INTERNAL_FIXED *initenv)(P0);
extern int (INTERNAL_FIXED *initTTS)(P0);
extern char DB_FAR * (INTERNAL_FIXED *rdm_apistr)(P1(int));

#else      /* LOCKCOMM_DATA */

#ifndef LMC_NONE
#ifndef LMC_USER
#ifndef LMC_INTERNAL
#ifndef LMC_GENERAL
#ifndef LMC_NETBIOS
#ifndef LMC_SPX
#ifndef LMC_NOVELL
#ifndef LMC_TCP
#ifndef LMC_IP
#ifndef LMC_SEMAPHORE
#ifndef LMC_VMS
#ifndef LMC_QNX

/*
   Default to all lock managers
*/
#define LMC_GENERAL
#define LMC_INTERNAL

#define LMC_NETBIOS
#define LMC_TCP
#ifdef RDM_NOVELL
/* These two Lock Manager types require the Novell SDK */
#define LMC_SPX
#define LMC_NOVELL
#endif


#endif         /* LMC_QNX */
#endif         /* LMC_VMS */
#endif         /* LMC_SEMAPHORE */
#endif         /* LMC_IP */
#endif         /* LMC_TCP */
#endif         /* LMC_SPX */
#endif         /* LMC_NOVELL */
#endif         /* LMC_NETBIOS */
#endif         /* LMC_GENERAL */
#endif         /* LMC_INTERNAL */
#endif         /* LMC_USER */
#endif         /* LMC_NONE */

#ifdef NO_LMC_USER
#undef LMC_USER
#endif
#ifdef NO_LMC_INTERNAL
#undef LMC_INTERNAL
#endif
#ifdef NO_LMC_GENERAL
#undef LMC_GENERAL
#endif
#ifdef NO_LMC_NETBIOS
#undef LMC_NETBIOS
#endif
#ifdef NO_LMC_SPX
#undef LMC_SPX
#endif
#ifdef NO_LMC_NOVELL
#undef LMC_NOVELL
#endif
#ifdef NO_LMC_TCP
#undef LMC_TCP
#endif
#ifdef NO_LMC_IP
#undef LMC_IP
#endif
#ifdef NO_LMC_SEMAPHORE
#undef LMC_SEMAPHORE
#endif
#ifdef NO_LMC_VMS
#undef LMC_VMS
#endif
#ifdef NO_LMC_QNX
#undef LMC_QNX
#endif


#ifdef MULTI_TASK
#define lmcnone_lockcomm      lmcnonet_lockcomm
#define lmci_lockcomm         lmcit_lockcomm
#define lmcusr_lockcomm       lmcusrt_lockcomm
#define lmcnetb_lockcomm      lmcnetbt_lockcomm
#define lmcg_lockcomm         lmcgt_lockcomm
#define lmcs_lockcomm         lmcst_lockcomm
#define sem_lockcomm          semt_lockcomm
#define ip_lockcomm           ipt_lockcomm
#define tcp_lockcomm          tcpt_lockcomm
#define lmcv_lockcomm         lmcvt_lockcomm
#define lmcq_lockcomm         lmcqt_lockcomm
#define lmcspx_lockcomm       lmcspxt_lockcomm
#define lmcnov_lockcomm       lmcnovt_lockcomm
#define lmcnov_lockcomm       lmcnovt_lockcomm
#endif

#ifndef NO_ERRSTR
#define LMC_ERRFUNC(fn)     fn
#else
#define LMC_ERRFUNC(fn)     lmcnone_errstr
#endif

/* Conditional compile so that not all of the lock manager
   communication types need to be linked into the app.
*/

/* the ordering of these entries must match the #defines below */
COMM_TYPES DB_HUGE lockcomm_type[MAX_LOCKCOMM] = {
   { "LMC_NONE",        lmcnone_lockcomm, lmcnone_errstr },

#ifdef LMC_USER
   { "LMC_USER",        lmcusr_lockcomm, LMC_ERRFUNC(lmcusr_errstr) },
#else
   { "LMC_USER",        NULL, NULL },
#endif

#ifdef LMC_NETBIOS
   { "LMC_NETBIOS",     lmcnetb_lockcomm, LMC_ERRFUNC(lmcnetb_errstr) },
#else
   { "LMC_NETBIOS",     NULL, NULL },
#endif

#ifdef LMC_INTERNAL
   { "LMC_INTERNAL",    lmci_lockcomm, LMC_ERRFUNC(lmci_errstr) },
#else
   { "LMC_INTERNAL",    NULL, NULL },
#endif

#ifdef LMC_GENERAL
   { "LMC_GENERAL",     lmcg_lockcomm, LMC_ERRFUNC(lmcg_errstr) },
#else
   { "LMC_GENERAL",     NULL, NULL },
#endif

#ifdef LMC_SPX
   { "LMC_SPX",         lmcspx_lockcomm, LMC_ERRFUNC(lmcspx_errstr) },
#else
   { "LMC_SPX",         NULL, NULL },
#endif

#ifdef LMC_TCP
   { "LMC_TCP",         tcp_lockcomm, LMC_ERRFUNC(tcp_errstr) },
#else
   { "LMC_TCP",         NULL, NULL },
#endif

#ifdef LMC_IP
   { "LMC_IP",          ip_lockcomm, LMC_ERRFUNC(ip_errstr) },
#else
   { "LMC_IP",          NULL, NULL },
#endif

#ifdef LMC_SEMAPHORE
   { "LMC_SEMAPHORE",   sem_lockcomm, LMC_ERRFUNC(sem_errstr) },
#else
   { "LMC_SEMAPHORE",   NULL, NULL },
#endif

#ifdef LMC_VMS
   { "LMC_VMS",         lmcv_lockcomm, LMC_ERRFUNC(lmcv_errstr) },
#else
   { "LMC_VMS",         NULL, NULL },
#endif

#ifdef LMC_QNX
   { "LMC_QNX",         lmcq_lockcomm, LMC_ERRFUNC(lmcq_errstr) },
#else
   { "LMC_QNX",         NULL, NULL },
#endif

#ifdef LMC_LANMAN
   { "LMC_LANMAN",      lmci_lockcomm, LMC_ERRFUNC(lmci_errstr) },
#else
   { "LMC_LANMAN",      NULL, NULL },
#endif

#ifdef LMC_NOVELL
   { "LMC_NOVELL",      lmcnov_lockcomm, lmcnov_errstr },
#else
   { "LMC_NOVELL",      NULL, NULL },
#endif
};

#ifdef MULTI_TASK
#undef lmcnone_lockcomm
#undef lmci_lockcomm
#undef lmcusr_lockcomm
#undef lmcnetb_lockcomm
#undef lmcg_lockcomm
#undef lmcs_lockcomm
#undef sem_lockcomm
#undef ip_lockcomm
#undef tcp_lockcomm
#undef lmcv_lockcomm
#undef lmcq_lockcomm
#undef lmcspx_lockcomm
#undef lmcnov_lockcomm
#endif

#ifdef NO_ERRSTR
extern int INTERNAL_FIXED NoRdmErrStr(P1(char DB_FAR *) Pi(int) Pi(int));
int (INTERNAL_FIXED *rdm_errstr)(P1(char DB_FAR *) Pi(int) Pi(int)) = NoRdmErrStr;
#else
extern int INTERNAL_FIXED RdmErrStr(P1(char DB_FAR *) Pi(int) Pi(int));
int (INTERNAL_FIXED *rdm_errstr)(P1(char DB_FAR *) Pi(int) Pi(int)) = RdmErrStr;
#endif

#ifdef NO_RDMINI
extern int INTERNAL_FIXED NoInitFromIniFile(P0);
int (INTERNAL_FIXED *initFromIniFile)(P0) = NoInitFromIniFile;
#else
extern int INTERNAL_FIXED DoInitFromIniFile(P0);
int (INTERNAL_FIXED *initFromIniFile)(P0) = DoInitFromIniFile;
#endif

#ifdef NO_USEENV
extern int INTERNAL_FIXED NoInitEnv(P0);
int (INTERNAL_FIXED *initenv)(P0) = NoInitEnv;
#else
extern int INTERNAL_FIXED DoInitEnv(P0);
int (INTERNAL_FIXED *initenv)(P0) = DoInitEnv;
#endif

#ifdef LMC_NOVELL
#define NOVELLTRX
#endif
#ifdef NO_NOVELLTRX
#ifdef NOVELLTRX
#undef NOVELLTRX
#endif
#endif

#ifdef NOVELLTRX
extern int INTERNAL_FIXED DoInitTTS(P0);
int (INTERNAL_FIXED *initTTS)(P0) = DoInitTTS;
#else
extern int INTERNAL_FIXED NoInitTTS(P0);
int (INTERNAL_FIXED *initTTS)(P0) = NoInitTTS;
#endif

#ifdef DB_DEBUG
#ifndef NO_API_NAMES
#define API_NAMES
#endif
#endif

#ifdef API_NAMES
extern char DB_FAR * INTERNAL_FIXED RdmApiStr(P1(int));
char DB_FAR * (INTERNAL_FIXED *rdm_apistr)(P1(int)) = RdmApiStr;
#else
extern char DB_FAR * INTERNAL_FIXED NoRdmApiStr(P1(int));
char DB_FAR * (INTERNAL_FIXED *rdm_apistr)(P1(int)) = NoRdmApiStr;
#endif


#endif     /* LOCKCOMM_DATA */

#undef LMC_NONE
#undef LMC_USER
#undef LMC_NETBIOS
#undef LMC_INTERNAL
#undef LMC_GENERAL
#undef LMC_SPX
#undef LMC_TCP
#undef LMC_IP
#undef LMC_SEMAPHORE
#undef LMC_VMS
#undef LMC_QNX
#undef LMC_LANMAN
#undef LMC_NOVELL

/* this ordering can not be changed and must match MAX_LOCKCOMM at the top */
#define LMC_NONE       0
#define LMC_USER       1
#define LMC_NETBIOS    2
#define LMC_INTERNAL   3
#define LMC_GENERAL    4
#define LMC_SPX        5
#define LMC_TCP        6
#define LMC_IP         7
#define LMC_SEMAPHORE  8
#define LMC_VMS        9
#define LMC_QNX       10
#define LMC_LANMAN    11
#define LMC_NOVELL    12


#ifndef LOCKCOMM_DATA

#ifdef DB_LIB
extern int (INTERNAL_FIXED *check_alignment)(P1(int));
extern ALIGNMENT_PROC calc_alignment;
#endif

#ifdef USE_DB_LIB
extern int (INTERNAL_FIXED *check_alignment)(P1(int));
extern ALIGNMENT_PROC calc_alignment;
#endif

#ifdef DB_DLL
extern int (INTERNAL_FIXED *check_alignment)(P1(int));
#endif

#ifdef USE_DB_DLL
#ifdef  ALIGNMENT_CHECK
#ifndef ALIGNMENT_PROC_NAME
#error "You must specify an alignment proc name"
#endif
#include <align.h>  /* defines ALIGNMENT_PROC_NAME function */
#else  /* ALIGNMENT_CHECK */
#ifdef ALIGNMENT_PROC_NAME
ALIGNMENT_PROC ALIGNMENT_PROC_NAME;
#endif
#endif /* ALIGNMENT_CHECK */
#endif /* USE_DB_DLL */

#else  /* LOCKCOMM_DATA */

#ifdef USE_DB_LIB
#ifdef NO_ALIGNMENT_CHECK
int (INTERNAL_FIXED *check_alignment)(P1(int)) = NULL;
ALIGNMENT_PROC calc_alignment = NULL;
#else /* NO_ALIGNMENT_CHECK */
extern int INTERNAL_FIXED CheckAlignment(P1(int));
int (INTERNAL_FIXED *check_alignment)(P1(int)) = CheckAlignment;
#define ALIGNMENT_PROC_NAME  CalcAlignment
#include <align.h>  /* defines ALIGNMENT_PROC_NAME function */
ALIGNMENT_PROC calc_alignment = (ALIGNMENT_PROC)ALIGNMENT_PROC_NAME;
#endif /* NO_ALIGNMENT_CHECK */
#endif /* USE_DB_LIB */

#ifdef DB_DLL
extern int INTERNAL_FIXED CheckAlignment(P1(int));
int (INTERNAL_FIXED *check_alignment)(P1(int)) = CheckAlignment;
#endif

#endif /* LOCKCOMM_DATA */

/* $Revision:   1.62  $ */

/* vpp LOCKCOMM.H (Fri Sep 19 11:56:10 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

