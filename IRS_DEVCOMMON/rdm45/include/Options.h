/*-----------------------------------------------------------------------
   options.h -- Raima Data Manager option default definitions

   Copyright (c) 1995, Raima Corporation, All Rights Reserved
-----------------------------------------------------------------------*/

#ifdef SYNC_BY_DEFAULT
#define DEF_SYNC  SYNCFILES
#else
#define DEF_SYNC  0L
#endif

#define DEF_TRUENAME  LMC_OPT_TRUENAME

#define DEF_SHORTNAME  LMC_OPT_SHORTNAME

#define DEFAULT_OPTS        (DCHAINUSE | TRLOGGING | DBYIELD | SHARETEST | \
                             ALIGNMENT_CHECK | DEF_SYNC)

#define DEFAULT_LMC_OPTS    (~LMC_OPT & (DEF_TRUENAME | DEF_SHORTNAME))

#ifdef DB_DEBUG
#define DEFAULT_DEBUG_OPTS  (~DEBUG_OPT & (PZVERIFY))
#endif
#ifdef DB_TRACE
#define DEFAULT_TRACE_OPTS  (os_getenv("DBTRACE") ? 0x0FFFFFFF : 0)
#endif

/*
   some options cannot be changed once a database has been opened
*/
#define OPEN_OPTS        (TRLOGGING | ARCLOGGING | SHARETEST | SYNCFILES | \
                          NOVELLTRX | IGNOREENV  | READONLY  | NORECOVER | \
                          PREALLOC_CACHE)

#define LMC_OPEN_OPTS    (LMC_OPT_TRUENAME | LMC_OPT_PORTABLE | LMC_OPT_LOGICALSETS | LMC_OPT_LANA)

#ifdef DB_DEBUG
#define DEBUG_OPEN_OPTS  (PZVERIFY | PAGE_CHECK | LOCK_CHECK)
#endif
#ifdef DB_TRACE
#define TRACE_OPEN_OPTS  (0)
#endif

/* $Revision:   1.3  $ */

/* vpp OPTIONS.H (Fri Sep 19 11:56:09 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

