/*----------------------------------------------------------------------------
   internal.h: RDM internal constant definitions for dt_internal()

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef INTERNAL_H
#define INTERNAL_H


#ifndef DBTYPE_H
/* dbtype.h has not been included so we need to do them */
#include "dbtype.h"
#endif

#define TOPIC_GLOBALS             0
#define TOPIC_TASK                1
#define TOPIC_FILE_TABLE          2
#define TOPIC_RECORD_TABLE        3
#define TOPIC_SET_TABLE           4
#define TOPIC_MEMBER_TABLE        5
#define TOPIC_SORT_TABLE          6
#define TOPIC_FIELD_TABLE         7
#define TOPIC_KEY_TABLE           8
#define TOPIC_DB_TABLE            9
#define TOPIC_REC_LOCK_TABLE     10
#define TOPIC_SET_LOCK_TABLE     11
#define TOPIC_KEY_LOCK_TABLE     12
#define TOPIC_APP_LOCKS_TABLE    13
#define TOPIC_EXCL_LOCKS_TABLE   14
#define TOPIC_KEPT_LOCKS_TABLE   15
#define TOPIC_CURR_OWNER_TABLE   16
#define TOPIC_CURR_MEMBER_TABLE  17
#define TOPIC_PGZERO_TABLE       19

#define ID_DB_STATUS          -1
#define ID_DBOPEN              0
#define ID_DBUSERID            1
#define ID_DBDPATH             2
#define ID_DBFPATH             3
#define ID_CACHE_OVFL          4
#define ID_NO_OF_KEYS          6
#define ID_KEY_INFO            7
#define ID_KEY_TYPE            8
#define ID_LMC_STATUS          9
#define ID_DBOPTIONS          10
#define ID_OV_INITADDR        11
#define ID_OV_ROOTADDR        12
#define ID_OV_NEXTADDR        13
#define ID_ROOT_IX            14
#define ID_REN_LIST           15
#define ID_PAGE_SIZE          16
#define ID_CURR_REC           17
#define ID_OV_FILE            18
#define ID_SIZE_FT            19
#define ID_SIZE_RT            20
#define ID_SIZE_ST            21
#define ID_SIZE_MT            22
#define ID_SIZE_SRT           23
#define ID_SIZE_FD            24
#define ID_SIZE_KT            25
#define ID_LOCK_LVL           26
#define ID_LOCK_STACK         27
#define ID_SK_LIST            28
#define ID_ERROR_FUNC         29

#ifndef ID_NO_COUNTRY
#define ID_COUNTRY_TBL        31
#define ID_CTBL_ACTIV         32
#define ID_CTBPATH            33
#endif

#ifdef DB_DLL
#define ID_REOPEN_TABLE       34
#else
#ifdef USE_DB_DLL
#define ID_REOPEN_TABLE       34
#endif
#endif

#define ID_DBNAME             35

#define ID_CR_TIME            36
#define ID_CO_TIME            37
#define ID_CM_TIME            38
#define ID_CS_TIME            39
#define ID_DB_TSRECS          40
#define ID_DB_TSSETS          41


#define ID_CURR_DB            42
#define ID_CURR_DB_TABLE      43
#define ID_SET_DB             44
#define ID_NO_OF_DBS          45
#define ID_RN_TABLE           46
#define ID_CURR_RN_TABLE      47


#define ID_TRANS_ID           55
#define ID_DBLOG              56

#define ID_LOCK_TRIES         57
#define ID_DBWAIT_TIME        58
#define ID_DB_TIMEOUT         59
#define ID_DB_LOCKMGR         60
#define ID_KEYL_CNT           61
#define ID_LP_SIZE            62
#define ID_FP_SIZE            63
#define ID_LOCK_PKT           64
#define ID_FREE_PKT           65
#define ID_FILE_REFS          66
#define ID_SESSION_ACTIVE     67

#define ID_MAX_FILES          68
#define ID_MAX_CACHEPGS       69
#define ID_MAX_OVIXPGS        70
#define ID_CACHE_LOOKUP       71
#define ID_CACHE_HITS         72
#define ID_LOCKCOMM           73
#define ID_LMC_OPTIONS        74
#define ID_DBTAF              75
#define ID_LOCKMGRN           76

#endif /* INTERNAL_H */

/* $Revision:   1.18  $ */

/* vpp INTERNAL.H (Fri Sep 19 11:56:09 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

