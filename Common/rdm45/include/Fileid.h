/*-----------------------------------------------------------------------

   fileid.h - filename string table functions

------------------------------------------------------------------------*/

#ifndef FILEID_H
#define FILEID_H

typedef LM_REF LM_FILENAME;    /* LMC.H is included               */

#include "packnet.h"

typedef struct FILEIDTAB_S
{
   DB_ULONG    offset;     /* Offset from start of string table */
   DB_ULONG    key;        /* Hash code for searching */
   LM_FILENAME prefix;     /* Pathname part of filename (== EMPTY if none) */
   DB_USHORT   active;     /* Number of active users for this name */
   DB_SHORT    nov_lock;   /* >0 == read locks, =0 == not locked, <0 == write locks */
   DB_BYTE     length;     /* String length in bytes */
} FILEIDTAB;

typedef struct FILEIDINFO_S
{
   DB_ULONG    next_offset;
   LM_FILENAME max_names;
   DB_ULONG    max_size;
   DB_ULONG    free_space;
   DB_ULONG    db_task_count;
} FILEIDINFO;

#include "nopack.h"

typedef struct FILEID_CONTEXT_S
{
   FILEIDTAB DB_FAR *fileid_tab;
   char      DB_FAR *string_tab;
   FILEIDINFO        fileid_info;
} FILEID_CONTEXT;

typedef struct
{
   struct FILEID_CONTEXT_S DB_FAR *ptr;
   LOCK_DESC
}  FILEID_CONTEXT_P;


#define FILEIDTAB_MAP_OFFSET     0
#define MAXFILEIDNAMES           (4096)
#define STRINGTAB_MAP_OFFSET     (FILEIDTAB_MAP_OFFSET + MAXFILEIDNAMES * sizeof(FILEIDTAB))
#define MAXSTRINGTABSIZE         (MAXFILEIDNAMES * 64)

int  INTERNAL_FIXED fileid_init(P1(FILEID_CONTEXT DB_FAR *) Pi(LM_FILENAME) Pi(DB_ULONG));
void INTERNAL_FIXED fileid_deinit(P1(FILEID_CONTEXT DB_FAR *));
void INTERNAL_FIXED fileid_del(P1(FILEID_CONTEXT DB_FAR *) Pi(LM_FILENAME));
LM_FILENAME INTERNAL_FIXED fileid_fnd(P1(FILEID_CONTEXT DB_FAR *) Pi(char DB_FAR *));
LM_FILENAME INTERNAL_FIXED fileid_add(P1(FILEID_CONTEXT DB_FAR *) Pi(char DB_FAR *));
char DB_FAR *INTERNAL_FIXED fileid_get(P1(FILEID_CONTEXT DB_FAR *) Pi(LM_FILENAME));

#endif      /* FILEID_H */

/* $Revision:   1.7  $ */

/* vpp FILEID.H (Fri Sep 19 11:56:09 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

