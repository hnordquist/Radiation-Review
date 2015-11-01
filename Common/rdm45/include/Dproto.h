/*----------------------------------------------------------------------------
   dproto.h: RDM API (user) function declaration header file

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef DPROTO_H
#define DPROTO_H

/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
*/

int EXTERNAL_FIXED d_decode_dba(P1(DB_ADDR) Pi(short DB_FAR *) Pi(unsigned long DB_FAR *));
int EXTERNAL_FIXED d_encode_dba(P1(short) Pi(unsigned long) Pi(DB_ADDR DB_FAR *));
int EXTERNAL_FIXED d_trbound(P0);
int EXTERNAL_FIXED d_trlog(P1(int) Pi(long) Pi(CONST char DB_FAR *) Pi(int));
int EXTERNAL_FIXED d_trmark(P0);
int EXTERNAL_FIXED d_troff(P0);
int EXTERNAL_FIXED d_tron(P0);
int EXTERNAL_FIXED d_rdmver(P1(char DB_FAR *) Pi(char DB_FAR *) Pi(int));

#ifndef MULTI_TASK

#define dt_opentask(a)
#define dt_closetask(a)
#define dt_newcache(a, b)
#define dt_sharecache(a, b)

int EXTERNAL_FIXED d_set_alignment_proc(P1(ALIGNMENT_PROC));
int EXTERNAL_DBN   d_check_alignment(DBN_D1);
int EXTERNAL_FIXED d_checkid(P1(char DB_FAR *));
int EXTERNAL_FIXED d_close(TASK_D1);
int EXTERNAL_FIXED d_closeall(TASK_D1);
int EXTERNAL_DBN   d_cmstat(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_cmtype(P1(int) Pi(int DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_connect(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_costat(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_cotype(P1(int) Pi(int DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_crget(P1(DB_ADDR DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_crread(P1(long) Pi(DB_VOID DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_crset(P1(DB_ADDR DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_crstat(DBN_D1);
int EXTERNAL_DBN   d_crtype(P1(int DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_crwrite(P1(long) Pi(DB_VOID DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_csmget(P1(int) Pi(DB_ADDR DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_csmread(P1(int) Pi(long) Pi(DB_VOID DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_csmset(P1(int) Pi(DB_ADDR DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_csmwrite(P1(int) Pi(long) Pi(CONST DB_VOID DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_csoget(P1(int) Pi(DB_ADDR DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_csoread(P1(int) Pi(long) Pi(DB_VOID DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_csoset(P1(int) Pi(DB_ADDR DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_csowrite(P1(int) Pi(long) Pi(CONST DB_VOID DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_csstat(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_ctscm(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_ctsco(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_ctscr(P1(DB_ULONG DB_FAR *) DBN_Dn);
int EXTERNAL_FIXED d_ctbpath(P1(CONST char DB_FAR *));
int EXTERNAL_DBN   d_curkey(DBN_D1);
int EXTERNAL_FIXED d_dberr(P1(int));
int EXTERNAL_FIXED d_dbdpath(P1(CONST char DB_FAR *));
int EXTERNAL_FIXED d_dbfpath(P1(CONST char DB_FAR *));
int EXTERNAL_FIXED d_dblog(P1(CONST char DB_FAR *));
int EXTERNAL_FIXED d_dbstat(P1(int) Pi(int) Pi(void DB_FAR *) Pi(int));
int EXTERNAL_FIXED d_dbtaf(P1(CONST char DB_FAR *));
int EXTERNAL_FIXED d_dbtmp(P1(CONST char DB_FAR *));
int EXTERNAL_FIXED d_dbnum(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED d_dbuserid(P1(CONST char DB_FAR *));
int EXTERNAL_FIXED d_def_opt(P1(unsigned long));
int EXTERNAL_DBN   d_delete(DBN_D1);
int EXTERNAL_FIXED d_destroy(P1(CONST char DB_FAR *));
int EXTERNAL_DBN   d_discon(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_disdel(DBN_D1);
int EXTERNAL_DBN   d_fillnew(P1(int) Pi(CONST DB_VOID DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_findco(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_findfm(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_findlm(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_findnm(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_findpm(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_fldnum(P1(int DB_FAR *) Pi(long) DBN_Dn);
int EXTERNAL_FIXED d_freeall(P0);
int EXTERNAL_DBN   d_gtscm(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_gtsco(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_gtscr(P1(DB_ULONG DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_gtscs(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn);
int EXTERNAL_FIXED d_iclose(TASK_DBN_D1);
int EXTERNAL_DBN   d_initfile(P1(FILE_NO) DBN_Dn);
int EXTERNAL_DBN   d_initialize(DBN_D1);
int EXTERNAL_FIXED d_internals(P1(int) Pi(int) Pi(int) Pi(DB_VOID DB_FAR *) Pi(unsigned));
int EXTERNAL_FIXED d_iopen(P1(CONST char DB_FAR *));
int EXTERNAL_DBN   d_ismember(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_isowner(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_keybuild(DBN_D1);
int EXTERNAL_DBN   d_keydel(P1(long) DBN_Dn);
int EXTERNAL_DBN   d_keyexist(P1(long) DBN_Dn);
int EXTERNAL_DBN   d_keyfind(P1(long) Pi(CONST DB_VOID DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_keyfree(P1(long) DBN_Dn);
int EXTERNAL_DBN   d_keyfrst(P1(long) DBN_Dn);
int EXTERNAL_DBN   d_keylast(P1(long) DBN_Dn);
int EXTERNAL_DBN   d_keylock(P1(long) Pi(char DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_keylstat(P1(long) Pi(char DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_keynext(P1(long) DBN_Dn);
int EXTERNAL_DBN   d_keyprev(P1(long) DBN_Dn);
int EXTERNAL_FIXED d_keyread(P1(DB_VOID DB_FAR *));
int EXTERNAL_DBN   d_keystore(P1(long) DBN_Dn);
int EXTERNAL_FIXED d_lmclear(P1(CONST char DB_FAR *) Pi(CONST char DB_FAR *) Pi(int));
int EXTERNAL_FIXED d_lmstat(P1(char DB_FAR *) Pi(int DB_FAR *));
int EXTERNAL_DBN   d_lock(P1(int) Pi(LOCK_REQUEST DB_FAR *) DBN_Dn);
int EXTERNAL_FIXED d_lockcomm(P1(int));
int EXTERNAL_FIXED d_lockmgr(P1(CONST char DB_FAR *));
int EXTERNAL_DBN   d_makenew(P1(int) DBN_Dn);
int EXTERNAL_FIXED d_mapchar(P1(unsigned char) Pi(unsigned char) Pi(CONST char DB_FAR *) Pi(unsigned char));
int EXTERNAL_DBN   d_members(P1(int) Pi(long DB_FAR *) DBN_Dn);
int EXTERNAL_FIXED d_off_opt(P1(unsigned long));
int EXTERNAL_FIXED d_on_opt(P1(unsigned long));
int EXTERNAL_FIXED d_open(P1(CONST char DB_FAR *) Pi(CONST char DB_FAR *));
int EXTERNAL_FIXED d_rdmini(P1(CONST char DB_FAR *));
int EXTERNAL_DBN   d_recfree(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_recfrst(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_reclast(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_reclock(P1(int) Pi(char DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_reclstat(P1(int) Pi(char DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_recnext(DBN_D1);
int EXTERNAL_DBN   d_recnum(P1(int DB_FAR *) Pi(int) DBN_Dn);
int EXTERNAL_FIXED d_recover(P1(CONST char DB_FAR *));
int EXTERNAL_DBN   d_recprev(DBN_D1);
int EXTERNAL_DBN   d_recread(P1(DB_VOID DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_recset(P1(int) DBN_Dn);
int EXTERNAL_FIXED d_recstat(P1(DB_ADDR) Pi(DB_ULONG) DBN_Dn);
int EXTERNAL_DBN   d_recwrite(P1(CONST DB_VOID DB_FAR *) DBN_Dn);
int EXTERNAL_FIXED d_renclean(TASK_D1);
int EXTERNAL_FIXED d_renfile(P1(CONST char DB_FAR *) Pi(FILE_NO) Pi(CONST char DB_FAR *));
int EXTERNAL_FIXED d_setdb(P1(int));
int EXTERNAL_FIXED d_setfiles(P1(int));
int EXTERNAL_DBN   d_setfree(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_setkey(P1(long) Pi(CONST DB_VOID DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_setlock(P1(int) Pi(char DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_setlstat(P1(int) Pi(char DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_setmm(P1(int) Pi(int) DBN_Dn);
int EXTERNAL_DBN   d_setmo(P1(int) Pi(int) DBN_Dn);
int EXTERNAL_DBN   d_setmr(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_setnum(P1(int DB_FAR *) Pi(int) DBN_Dn);
int EXTERNAL_DBN   d_setom(P1(int) Pi(int) DBN_Dn);
int EXTERNAL_DBN   d_setoo(P1(int) Pi(int) DBN_Dn);
int EXTERNAL_DBN   d_setor(P1(int) DBN_Dn);
int EXTERNAL_FIXED d_setpages(P1(int) Pi(int));
int EXTERNAL_DBN   d_setrm(P1(int) DBN_Dn);
int EXTERNAL_DBN   d_setro(P1(int) DBN_Dn);
int EXTERNAL_FIXED d_set_dberr(P1(ERRORPROC));
int EXTERNAL_DBN   d_stscm(P1(int) Pi(DB_ULONG) DBN_Dn);
int EXTERNAL_DBN   d_stsco(P1(int) Pi(DB_ULONG) DBN_Dn);
int EXTERNAL_DBN   d_stscr(P1(DB_ULONG) DBN_Dn);
int EXTERNAL_DBN   d_stscs(P1(int) Pi(DB_ULONG) DBN_Dn);
int EXTERNAL_FIXED d_timeout(P1(int));
int EXTERNAL_FIXED d_trabort(P0);
int EXTERNAL_FIXED d_trbegin(P1(CONST char DB_FAR *));
int EXTERNAL_FIXED d_trend(P0);
int EXTERNAL_DBN   d_utscm(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_utsco(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_utscr(P1(DB_ULONG DB_FAR *) DBN_Dn);
int EXTERNAL_DBN   d_utscs(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn);

int EXTERNAL_FIXED d_rlbclr(DBN_D1);
int EXTERNAL_FIXED d_rlbset(DBN_D1);
int EXTERNAL_FIXED d_rlbtst(DBN_D1);

int EXTERNAL_FIXED d_rerdcurr(P1(DB_ADDR DB_FAR * DB_FAR *) DBN_Dn);
int EXTERNAL_FIXED d_rdcurr(P1(DB_ADDR DB_FAR * DB_FAR*) Pi(int DB_FAR *) DBN_Dn);
int EXTERNAL_FIXED d_wrcurr(P1(DB_ADDR DB_FAR *) DBN_Dn);

#endif /* ! MULTI_TASK */

#endif /* ! DPROTO_H */

/* $Revision:   1.29  $ */

/* vpp DPROTO.H (Fri Sep 19 11:56:10 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

