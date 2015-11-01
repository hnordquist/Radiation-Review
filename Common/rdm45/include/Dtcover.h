/*----------------------------------------------------------------------------
   dtcover.h: converts d_* calls to dt_* for MULTI_TASKing

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef DTCOVER_H
#define DTCOVER_H

#define d_set_alignment_proc dt_set_alignment_proc
#define d_check_alignment  dt_check_alignment
#define d_internals        dt_internals
#define d_setpages         dt_setpages
#define d_iclose           dt_iclose
#define d_close            dt_close
#define d_closeall         dt_closeall
#define d_checkid          dt_checkid
#define d_cmstat           dt_cmstat
#define d_cmtype           dt_cmtype
#define d_connect          dt_connect
#define d_costat           dt_costat
#define d_cotype           dt_cotype
#define d_crget            dt_crget
#define d_crread           dt_crread
#define d_crset            dt_crset
#define d_crstat           dt_crstat
#define d_crtype           dt_crtype
#define d_crwrite          dt_crwrite
#define d_csmget           dt_csmget
#define d_csmread          dt_csmread
#define d_csmset           dt_csmset
#define d_csmwrite         dt_csmwrite
#define d_csoget           dt_csoget
#define d_csoread          dt_csoread
#define d_csoset           dt_csoset
#define d_csowrite         dt_csowrite
#define d_csstat           dt_csstat
#define d_ctbpath          dt_ctbpath
#define d_ctscm            dt_ctscm
#define d_ctsco            dt_ctsco
#define d_ctscr            dt_ctscr
#define d_curkey           dt_curkey
#define d_dberr            dt_dberr
#define d_dbnum            dt_dbnum
#define d_dbdpath          dt_dbdpath
#define d_dbfpath          dt_dbfpath
#define d_dblog            dt_dblog
#define d_dbstat           dt_dbstat
#define d_dbtaf            dt_dbtaf
#define d_dbtmp            dt_dbtmp
#define d_dbuserid         dt_dbuserid
#define d_def_opt          dt_def_opt
#define d_delete           dt_delete
#define d_disdel           dt_disdel
#define d_recnext          dt_recnext
#define d_recprev          dt_recprev
#define d_destroy          dt_destroy
#define d_discon           dt_discon
#define d_fillnew          dt_fillnew
#define d_findco           dt_findco
#define d_findfm           dt_findfm
#define d_findlm           dt_findlm
#define d_findnm           dt_findnm
#define d_findpm           dt_findpm
#define d_fldnum           dt_fldnum
#define d_freeall          dt_freeall
#define d_gtscm            dt_gtscm
#define d_gtsco            dt_gtsco
#define d_gtscr            dt_gtscr
#define d_gtscs            dt_gtscs
#define d_initialize       dt_initialize
#define d_initfile         dt_initfile
#define d_ismember         dt_ismember
#define d_isowner          dt_isowner
#define d_keybuild         dt_keybuild
#define d_keydel           dt_keydel
#define d_keyexist         dt_keyexist
#define d_keyfind          dt_keyfind
#define d_keyfree          dt_keyfree
#define d_keyfrst          dt_keyfrst
#define d_keylast          dt_keylast
#define d_keylock          dt_keylock
#define d_keylstat         dt_keylstat
#define d_keynext          dt_keynext
#define d_keyprev          dt_keyprev
#define d_keyread          dt_keyread
#define d_keystore         dt_keystore
#define d_lmclear          dt_lmclear
#define d_lmstat           dt_lmstat
#define d_lock             dt_lock
#define d_lockcomm         dt_lockcomm
#define d_lockmgr          dt_lockmgr
#define d_makenew          dt_makenew
#define d_mapchar          dt_mapchar
#define d_members          dt_members
#define d_off_opt          dt_off_opt
#define d_on_opt           dt_on_opt
#define d_iopen            dt_iopen
#define d_open             dt_open
#define d_rdcurr           dt_rdcurr
#define d_rdmini           dt_rdmini
#define d_rerdcurr         dt_rerdcurr
#define d_recfree          dt_recfree
#define d_recfrst          dt_recfrst
#define d_reclast          dt_reclast
#define d_reclock          dt_reclock
#define d_reclstat         dt_reclstat
#define d_recnum           dt_recnum
#define d_recover          dt_recover
#define d_recread          dt_recread
#define d_recset           dt_recset
#define d_recstat          dt_recstat
#define d_recwrite         dt_recwrite
#define d_renfile          dt_renfile
#define d_renclean         dt_renclean
#define d_rlbclr           dt_rlbclr
#define d_rlbset           dt_rlbset
#define d_rlbtst           dt_rlbtst
#define d_set_dberr        dt_set_dberr
#define d_setdb            dt_setdb
#define d_setfiles         dt_setfiles
#define d_setfree          dt_setfree
#define d_setkey           dt_setkey
#define d_setlock          dt_setlock
#define d_setlstat         dt_setlstat
#define d_setmm            dt_setmm
#define d_setmo            dt_setmo
#define d_setmr            dt_setmr
#define d_setom            dt_setom
#define d_setoo            dt_setoo
#define d_setor            dt_setor
#define d_setrm            dt_setrm
#define d_setro            dt_setro
#define d_setnum           dt_setnum
#define d_stscm            dt_stscm
#define d_stsco            dt_stsco
#define d_stscr            dt_stscr
#define d_stscs            dt_stscs
#define d_timeout          dt_timeout
#define d_trabort          dt_trabort
#define d_trbegin          dt_trbegin
#define d_trend            dt_trend
#define d_utscm            dt_utscm
#define d_utsco            dt_utsco
#define d_utscr            dt_utscr
#define d_utscs            dt_utscs
#define d_wrcurr           dt_wrcurr

#endif /* DTCOVER_H */

/* $Revision:   1.19  $ */

/* vpp DTCOVER.H (Fri Sep 19 11:56:10 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

