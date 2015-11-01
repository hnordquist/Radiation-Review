/*------------------------------------------------------------------------
   nolib.h -- Raima Data Manager header file for utilities

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
--------------------------------------------------------------------------*/

#ifndef NOLIB
#error You must specify -DNOLIB to the compiler.
#endif

#undef pgzero
#undef dbopen
#undef dbdpath
#undef dbfpath
#undef file_table
#undef record_table
#undef field_table
#undef set_table
#undef member_table
#undef key_table
#undef sort_table
#undef size_ft
#undef size_rt
#undef size_fd
#undef size_st
#undef size_mt
#undef size_kt
#undef size_srt
#undef page_size
#undef curr_own
#undef curr_mem
#undef curr_rec
#undef key_info
#undef net_status
#undef db_lockmgr
#undef last_file

#undef country_tbl
#undef ctbl_activ
#undef ctbpath
#undef db_status
#undef db_error
#undef dboptflag
#undef dboptions
#undef rdmIniFile
#undef dberror_func

#ifdef MAIN
#define EXTERN /**/
#else
#define EXTERN extern
#endif

EXTERN PGZERO          *pgzero;
EXTERN int              dbopen;
EXTERN char             dbdpath[FILENMLEN * 2];
EXTERN char             dbfpath[FILENMLEN * 2];
EXTERN FILE_ENTRY      *file_table;
EXTERN RECORD_ENTRY    *record_table;
EXTERN FIELD_ENTRY     *field_table;
EXTERN SET_ENTRY       *set_table;
EXTERN MEMBER_ENTRY    *member_table;
EXTERN KEY_ENTRY       *key_table;
EXTERN SORT_ENTRY      *sort_table;
EXTERN DB_SHORT         size_ft;
EXTERN DB_SHORT         size_rt;
EXTERN DB_SHORT         size_fd;
EXTERN DB_SHORT         size_st;
EXTERN DB_SHORT         size_mt;
EXTERN DB_SHORT         size_kt;
EXTERN DB_SHORT         size_srt;
EXTERN DB_SHORT         page_size;
EXTERN DB_ADDR         *curr_own;
EXTERN DB_ADDR         *curr_mem;
EXTERN DB_ADDR          curr_rec;
EXTERN KEY_INFO         key_info;

EXTERN volatile int     db_status;
EXTERN ERR_INFO         db_error;
EXTERN unsigned long    dboptflag;
EXTERN unsigned long    dboptions;
EXTERN char             rdmIniFile[FILENMLEN];
EXTERN ERRORPROC        dberror_func;

EXTERN CNTRY_TBL       *country_tbl;
EXTERN int              ctbl_activ;
EXTERN char             ctbpath[FILENMLEN];

#undef RETURN
#define RETURN(x)       return (x)

#ifdef MAIN

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

#else

extern int (INTERNAL_FIXED *rdm_errstr)(P1(char DB_FAR *) Pi(int) Pi(int));
extern int (INTERNAL_FIXED *initFromIniFile)(P0);
extern int (INTERNAL_FIXED *initenv)(P0);

#endif /* MAIN */

#undef EXTERN

extern int tabinit(P1(char *));
extern char *fldtotxt(P1(int) Pi(char *) Pi(char *));

/* $Revision:   1.19  $*/

/* vpp NOLIB.H (Fri Sep 19 11:56:12 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

