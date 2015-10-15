/*----------------------------------------------------------------------------
   task.h: db_VISTA task cover function declaration header file

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef TASK_H
#define TASK_H

int EXTERNAL_FIXED dt_set_alignment_proc(P1(ALIGNMENT_PROC) TASK_Di);
int EXTERNAL_DBN   dt_check_alignment(TASK_D1 DBN_Dn);
int EXTERNAL_FIXED dt_checkid(P1(char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED dt_close(TASK_D1);
int EXTERNAL_FIXED dt_closeall(TASK_D1);
int EXTERNAL_FIXED dt_closetask(P1(DB_TASK DB_FAR *));
int EXTERNAL_DBN   dt_cmstat(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_cmtype(P1(int) Pi(int DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_costat(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_connect(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_cotype(P1(int) Pi(int DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_crget(P1(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_crread(P1(long) Pi(DB_VOID DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_crset(P1(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_crstat(TASK_D1 DBN_Dn);
int EXTERNAL_DBN   dt_crtype(P1(int DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_crwrite(P1(long) Pi(DB_VOID DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_csmget(P1(int) Pi(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_csmread(P1(int) Pi(long) Pi(DB_VOID DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_csmset(P1(int) Pi(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_csmwrite(P1(int) Pi(long) Pi(CONST DB_VOID DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_csoget(P1(int) Pi(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_csoread(P1(int) Pi(long) Pi(DB_VOID DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_csoset(P1(int) Pi(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_csowrite(P1(int) Pi(long) Pi(CONST DB_VOID DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_csstat(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_ctscm(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_ctsco(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_ctscr(P1(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_ctbpath(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_DBN   dt_curkey(TASK_D1 DBN_Dn);
int EXTERNAL_FIXED dt_dberr(P1(int) TASK_Di);
int EXTERNAL_FIXED dt_dbdpath(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED dt_dbfpath(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED dt_dblog(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED dt_dbstat(P1(int) Pi(int) Pi(DB_VOID DB_FAR *) Pi(int) TASK_Di);
int EXTERNAL_FIXED dt_dbtaf(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED dt_dbtmp(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED dt_dbnum(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED dt_dbuserid(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED dt_def_opt(P1(unsigned long) TASK_Di);
int EXTERNAL_DBN   dt_delete(TASK_D1 DBN_Dn);
int EXTERNAL_FIXED dt_destroy(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_DBN   dt_discon(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_disdel(TASK_D1 DBN_Dn);
int EXTERNAL_DBN   dt_fillnew(P1(int) Pi(CONST DB_VOID DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_findco(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_findfm(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_findlm(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_findnm(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_findpm(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_fldnum(P1(int DB_FAR *) Pi(long) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_freeall(TASK_D1);
int EXTERNAL_DBN   dt_gtscm(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_gtsco(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_gtscr(P1(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_gtscs(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_iclose(TASK_D1 DBN_Dn);
int EXTERNAL_DBN   dt_initialize(TASK_D1 DBN_Dn);
int EXTERNAL_DBN   dt_initfile(P1(FILE_NO) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_internals(P1(DB_TASK DB_FAR *) Pi(int) Pi(int) Pi(int) Pi(DB_VOID DB_FAR *) Pi(unsigned));
int EXTERNAL_FIXED dt_iopen(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_DBN   dt_ismember(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_isowner(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_keybuild(TASK_D1 DBN_Dn);
int EXTERNAL_DBN   dt_keydel(P1(long) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_keyexist(P1(long) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_keyfind(P1(long) Pi(CONST DB_VOID DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_keyfree(P1(long) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_keyfrst(P1(long) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_keylast(P1(long) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_keylock(P1(long) Pi(char DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_keylstat(P1(long) Pi(char DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_keynext(P1(long) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_keyprev(P1(long) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_keyread(P1(DB_VOID DB_FAR *) TASK_Di);
int EXTERNAL_DBN   dt_keystore(P1(long) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_lmclear(P1(CONST char DB_FAR *) Pi(CONST char DB_FAR *) Pi(int) TASK_Di);
int EXTERNAL_FIXED dt_lmstat(P1(char DB_FAR *) Pi(int DB_FAR *) TASK_Di);
int EXTERNAL_DBN   dt_lock(P1(int) Pi(LOCK_REQUEST DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_lockcomm(P1(int) TASK_Di);
int EXTERNAL_FIXED dt_lockmgr(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_DBN   dt_makenew(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_mapchar(P1(unsigned char) Pi(unsigned char) Pi(CONST char DB_FAR *) Pi(unsigned char) TASK_Di);
int EXTERNAL_DBN   dt_members(P1(int) Pi(long DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_newcache(P1(DB_TASK DB_FAR *) Pi(int));
int EXTERNAL_FIXED dt_off_opt(P1(unsigned long) TASK_Di);
int EXTERNAL_FIXED dt_on_opt(P1(unsigned long) TASK_Di);
int EXTERNAL_FIXED dt_open(P1(CONST char DB_FAR *) Pi(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED dt_opentask(P1(DB_TASK DB_FAR *));
int EXTERNAL_FIXED dt_rdmini(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_DBN   dt_recfree(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_recfrst(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_reclast(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_reclock(P1(int) Pi(char DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_reclstat(P1(int) Pi(char DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_recnext(TASK_D1 DBN_Dn);
int EXTERNAL_DBN   dt_recnum(P1(int DB_FAR *) Pi(int)  TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_recover(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_DBN   dt_recprev(TASK_D1 DBN_Dn);
int EXTERNAL_DBN   dt_recread(P1(DB_VOID DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_recset(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_recstat(P1(DB_ADDR) Pi(DB_ULONG) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_recwrite(P1(CONST DB_VOID DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_renfile(P1(CONST char DB_FAR *) Pi(FILE_NO) Pi(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED dt_renclean(TASK_D1);
int EXTERNAL_FIXED dt_rlbclr(TASK_D1 DBN_Dn);
int EXTERNAL_FIXED dt_rlbset(TASK_D1 DBN_Dn);
int EXTERNAL_FIXED dt_rlbtst(TASK_D1 DBN_Dn);
int EXTERNAL_FIXED dt_setdb(P1(int) TASK_Di);
int EXTERNAL_DBN   dt_setfree(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_setfiles(P1(int) TASK_Di);
int EXTERNAL_DBN   dt_setkey(P1(long) Pi(CONST DB_VOID DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_setlock(P1(int) Pi(char DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_setlstat(P1(int) Pi(char DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_setmm(P1(int) Pi(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_setmo(P1(int) Pi(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_setmr(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_setnum(P1(int DB_FAR *) Pi(int)  TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_setom(P1(int) Pi(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_setoo(P1(int) Pi(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_setor(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_setpages(P1(int) Pi(int) Pi(DB_TASK DB_FAR *));
int EXTERNAL_DBN   dt_setrm(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_setro(P1(int) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_set_dberr(P1(ERRORPROC) TASK_Di);
int EXTERNAL_FIXED dt_sharecache(P1(DB_TASK DB_FAR *) Pi(DB_TASK DB_FAR *));
int EXTERNAL_DBN   dt_stscm(P1(int) Pi(DB_ULONG) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_stsco(P1(int) Pi(DB_ULONG) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_stscr(P1(DB_ULONG) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_stscs(P1(int) Pi(DB_ULONG) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_timeout(P1(int) TASK_Di);
int EXTERNAL_FIXED dt_trabort(TASK_D1);/* dblfcns.c */
int EXTERNAL_FIXED dt_trbegin(P1(CONST char DB_FAR *) TASK_Di);
int EXTERNAL_FIXED dt_trend(TASK_D1);
int EXTERNAL_DBN   dt_utscm(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_utsco(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_utscr(P1(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_DBN   dt_utscs(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);


int EXTERNAL_FIXED dt_rdcurr(P1(DB_ADDR DB_FAR * DB_FAR *) Pi(int DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_rerdcurr(P1(DB_ADDR DB_FAR * DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_wrcurr(P1(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);



#ifndef NO_DT_COVER                    /* ! NO_DT_COVER */

#define d_set_alignment_proc(a)     dt_set_alignment_proc(a, &Currtask)
#define d_check_alignment(dbn)      dt_check_alignment(&Currtask, dbn)
#define d_checkid(a)                dt_checkid(a, &Currtask)
#define d_close()                   dt_close(&Currtask)
#define d_closeall()                dt_closeall(&Currtask)
#define d_cmstat(a, dbn)            dt_cmstat(a, &Currtask, dbn)
#define d_cmtype(a, b, dbn)         dt_cmtype(a, b, &Currtask, dbn)
#define d_connect(a, dbn)           dt_connect(a, &Currtask, dbn)
#define d_costat(a, dbn)            dt_costat(a, &Currtask, dbn)
#define d_cotype(a, b, dbn)         dt_cotype(a, b, &Currtask, dbn)
#define d_crget(a, dbn)             dt_crget(a, &Currtask, dbn)
#define d_crread(a, b, dbn)         dt_crread(a, b, &Currtask, dbn)
#define d_crset(a, dbn)             dt_crset(a, &Currtask, dbn)
#define d_crstat(dbn)               dt_crstat(&Currtask, dbn)
#define d_crtype(a, dbn)            dt_crtype(a, &Currtask, dbn)
#define d_crwrite(a, b, dbn)        dt_crwrite(a, b, &Currtask, dbn)
#define d_csmget(a, b, dbn)         dt_csmget(a, b, &Currtask, dbn)
#define d_csmread(a, b, c, dbn)     dt_csmread(a, b, c, &Currtask, dbn)
#define d_csmset(a, b, dbn)         dt_csmset(a, b, &Currtask, dbn)
#define d_csmwrite(a, b, c, dbn)    dt_csmwrite(a, b, c, &Currtask, dbn)
#define d_csoget(a, b, dbn)         dt_csoget(a, b, &Currtask, dbn)
#define d_csoread(a, b, c, dbn)     dt_csoread(a, b, c, &Currtask, dbn)
#define d_csoset(a, b, dbn)         dt_csoset(a, b, &Currtask, dbn)
#define d_csowrite(a, b, c, dbn)    dt_csowrite(a, b, c, &Currtask, dbn)
#define d_csstat(a, dbn)            dt_csstat(a, &Currtask, dbn)
#define d_ctbpath(a)                dt_ctbpath(a, &Currtask)
#define d_ctscm(a, b, dbn)          dt_ctscm(a, b, &Currtask, dbn)
#define d_ctsco(a, b, dbn)          dt_ctsco(a, b, &Currtask, dbn)
#define d_ctscr(a, dbn)             dt_ctscr(a   , &Currtask, dbn)
#define d_curkey(dbn)               dt_curkey(&Currtask, dbn)
#define d_dberr(a)                  dt_dberr(a, &Currtask)
#define d_dbdpath(a)                dt_dbdpath(a, &Currtask)
#define d_dbfpath(a)                dt_dbfpath(a, &Currtask)
#define d_dblog(a)                  dt_dblog(a, &Currtask)
#define d_dbnum(a)                  dt_dbnum(a, &Currtask)
#define d_dbstat(a, b, c, d)        dt_dbstat(a, b, c, d, &Currtask)
#define d_dbtaf(a)                  dt_dbtaf(a, &Currtask)
#define d_dbtmp(a)                  dt_dbtmp(a, &Currtask)
#define d_dbuserid(a)               dt_dbuserid(a, &Currtask)
#define d_def_opt(a)                dt_def_opt(a, &Currtask)
#define d_delete(dbn)               dt_delete(&Currtask, dbn)
#define d_destroy(a)                dt_destroy(a, &Currtask)
#define d_discon(a, dbn)            dt_discon(a, &Currtask, dbn)
#define d_disdel(dbn)               dt_disdel(&Currtask, dbn)
#define d_fillnew(a, b, dbn)        dt_fillnew(a, b, &Currtask, dbn)
#define d_findco(a, dbn)            dt_findco(a, &Currtask, dbn)
#define d_findfm(a, dbn)            dt_findfm(a, &Currtask, dbn)
#define d_findlm(a, dbn)            dt_findlm(a, &Currtask, dbn)
#define d_findnm(a, dbn)            dt_findnm(a, &Currtask, dbn)
#define d_findpm(a, dbn)            dt_findpm(a, &Currtask, dbn)
#define d_fldnum(a, b, dbn)         dt_fldnum(a, b, &Currtask, dbn)
#define d_freeall()                 dt_freeall(&Currtask)
#define d_gtscm(a, b, dbn)          dt_gtscm(a, b, &Currtask, dbn)
#define d_gtsco(a, b, dbn)          dt_gtsco(a, b, &Currtask, dbn)
#define d_gtscr(a, dbn)             dt_gtscr(a, &Currtask, dbn)
#define d_gtscs(a, b, dbn)          dt_gtscs(a, b, &Currtask, dbn)
#define d_iclose(a)                 dt_iclose(&Currtask, a)
#define d_initialize(dbn)           dt_initialize(&Currtask, dbn)
#define d_initfile(a, dbn)          dt_initfile(a, &Currtask, dbn)
#define d_internals(a, b, c, d, e)  dt_internals(&Currtask, a, b, c, d, e)
#define d_iopen(a)                  dt_iopen(a, &Currtask)
#define d_ismember(a, dbn)          dt_ismember(a, &Currtask, dbn)
#define d_isowner(a, dbn)           dt_isowner(a, &Currtask, dbn)
#define d_keybuild(dbn)             dt_keybuild(&Currtask, dbn)
#define d_keydel(a, dbn)            dt_keydel(a, &Currtask, dbn)
#define d_keyexist(a,dbn)           dt_keyexist(a, &Currtask, dbn)
#define d_keyfind(a, b, dbn)        dt_keyfind(a, b, &Currtask, dbn)
#define d_keyfree(a, dbn)           dt_keyfree(a, &Currtask, dbn)
#define d_keyfrst(a, dbn)           dt_keyfrst(a, &Currtask, dbn)
#define d_keylast(a, dbn)           dt_keylast(a, &Currtask, dbn)
#define d_keylock(a, b, dbn)        dt_keylock(a, b, &Currtask, dbn)
#define d_keylstat(a, b, dbn)       dt_keylstat(a, b, &Currtask, dbn)
#define d_keynext(a, dbn)           dt_keynext(a, &Currtask, dbn)
#define d_keyprev(a, dbn)           dt_keyprev(a, &Currtask, dbn)
#define d_keyread(a)                dt_keyread(a, &Currtask)
#define d_keystore(a, dbn)          dt_keystore(a, &Currtask, dbn)
#define d_lmclear(a, b, c)          dt_lmclear(a, b, c, &Currtask)
#define d_lmstat(a, b)              dt_lmstat(a, b, &Currtask)
#define d_lock(a, b, dbn)           dt_lock(a, b, &Currtask, dbn)
#define d_lockcomm(a)               dt_lockcomm(a, &Currtask)
#define d_lockmgr(a)                dt_lockmgr(a, &Currtask)
#define d_makenew(a, dbn)           dt_makenew(a, &Currtask, dbn)
#define d_mapchar(a, b, c, d)       dt_mapchar(a, b, c, d, &Currtask)
#define d_members(a, b, dbn)        dt_members(a, b, &Currtask, dbn)
#define d_off_opt(a)                dt_off_opt(a, &Currtask)
#define d_on_opt(a)                 dt_on_opt(a, &Currtask)
#define d_open(a, b)                dt_open(a, b, &Currtask)
#define d_rdcurr(a, b, dbn)         dt_rdcurr(a, b, &Currtask, dbn)
#define d_rdmini(a)                 dt_rdmini(a, &Currtask)
#define d_recfree(a, dbn)           dt_recfree(a, &Currtask, dbn)
#define d_recfrst(a, dbn)           dt_recfrst(a, &Currtask, dbn)
#define d_reclast(a, dbn)           dt_reclast(a, &Currtask, dbn)
#define d_reclock(a, b, dbn)        dt_reclock(a, b, &Currtask, dbn)
#define d_reclstat(a, b, dbn)       dt_reclstat(a, b, &Currtask, dbn)
#define d_recnext(dbn)              dt_recnext(&Currtask, dbn)
#define d_recnum(a, b, dbn)         dt_recnum(a, b, &Currtask, dbn)
#define d_recover(a)                dt_recover(a, &Currtask)
#define d_recprev(dbn)              dt_recprev(&Currtask, dbn)
#define d_recread(a, dbn)           dt_recread(a, &Currtask, dbn)
#define d_recset(a, dbn)            dt_recset(a, &Currtask, dbn)
#define d_recstat(a, b, dbn)        dt_recstat(a, b, &Currtask, dbn)
#define d_recwrite(a, dbn)          dt_recwrite(a, &Currtask, dbn)
#define d_renclean()                dt_renclean(&Currtask)
#define d_renfile(a, b, c)          dt_renfile(a, b, c, &Currtask)
#define d_rerdcurr(a, dbn)          dt_rerdcurr(a, &Currtask, dbn)
#define d_rlbclr(dbn)               dt_rlbclr(&Currtask, dbn)
#define d_rlbset(dbn)               dt_rlbset(&Currtask, dbn)
#define d_rlbtst(dbn)               dt_rlbtst(&Currtask, dbn)
#define d_setdb(a)                  dt_setdb(a, &Currtask)
#define d_setfiles(a)               dt_setfiles(a, &Currtask)
#define d_setfree(a, dbn)           dt_setfree(a, &Currtask, dbn)
#define d_setkey(a, b, dbn)         dt_setkey(a, b, &Currtask, dbn)
#define d_setlock(a, b, dbn)        dt_setlock(a, b, &Currtask, dbn)
#define d_setlstat(a, b, dbn)       dt_setlstat(a, b, &Currtask, dbn)
#define d_setmm(a, b, dbn)          dt_setmm(a, b, &Currtask, dbn)
#define d_setmo(a, b, dbn)          dt_setmo(a, b, &Currtask, dbn)
#define d_setmr(a, dbn)             dt_setmr(a, &Currtask, dbn)
#define d_setnum(a, b, dbn)         dt_setnum(a, b, &Currtask, dbn)
#define d_setom(a, b, dbn)          dt_setom(a, b, &Currtask, dbn)
#define d_setoo(a, b, dbn)          dt_setoo(a, b, &Currtask, dbn)
#define d_setor(a, dbn)             dt_setor(a, &Currtask, dbn)
#define d_setpages(a, b)            dt_setpages(a, b, &Currtask)
#define d_setrm(a, dbn)             dt_setrm(a, &Currtask, dbn)
#define d_setro(a, dbn)             dt_setro(a, &Currtask, dbn)
#define d_set_dberr(a)              dt_set_dberr(a, &Currtask)
#define d_stscm(a, b, dbn)          dt_stscm(a, b, &Currtask, dbn)
#define d_stsco(a, b, dbn)          dt_stsco(a, b, &Currtask, dbn)
#define d_stscr(a, dbn)             dt_stscr(a, &Currtask, dbn)
#define d_stscs(a, b, dbn)          dt_stscs(a, b, &Currtask, dbn)
#define d_timeout(a)                dt_timeout(a, &Currtask)
#define d_trabort()                 dt_trabort(&Currtask)
#define d_trbegin(a)                dt_trbegin(a, &Currtask)
#define d_trend()                   dt_trend(&Currtask)
#define d_utscm(a, b, dbn)          dt_utscm(a, b, &Currtask, dbn)
#define d_utsco(a, b, dbn)          dt_utsco(a, b, &Currtask, dbn)
#define d_utscr(a, dbn)             dt_utscr(a, &Currtask, dbn)
#define d_utscs(a, b, dbn)          dt_utscs(a, b, &Currtask, dbn)
#define d_wrcurr(a, dbn)            dt_wrcurr(a, &Currtask, dbn)


#endif /* NO_DT_COVER */

#endif /* TASK_H */

/* $Revision:   1.30  $ */

/* vpp TASK.H (Fri Sep 19 11:56:11 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

