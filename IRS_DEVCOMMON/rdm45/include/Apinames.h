/*----------------------------------------------------------------------
   apinames.h -- text of error messages used by dberr function

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef APINAMES_H
#define APINAMES_H


/*
   Define the various argument types
*/
#define A_RDM        0x80
#define A_PTR        0x40
#define A_UNSIGNED   0x20

#define A_NULL       (0x00)

#define A_CHAR       (0x01)
#define A_PCHAR      (A_CHAR | A_PTR)
#define A_UCHAR      (A_CHAR | A_UNSIGNED)
#define A_PUCHAR     (A_CHAR | A_PTR | A_UNSIGNED)

#define A_SHORT      (0x02)
#define A_PSHORT     (A_SHORT | A_PTR)
#define A_USHORT     (A_SHORT | A_UNSIGNED)
#define A_PUSHORT    (A_SHORT | A_PTR | A_UNSIGNED)

#define A_INT        (0x03)
#define A_PINT       (A_INT | A_PTR)
#define A_UINT       (A_INT | A_UNSIGNED)
#define A_PUINT      (A_INT | A_PTR | A_UNSIGNED)

#define A_LONG       (0x04)
#define A_PLONG      (A_LONG | A_PTR)
#define A_ULONG      (A_LONG | A_UNSIGNED)
#define A_PULONG     (A_LONG | A_PTR | A_UNSIGNED)

#define A_DBA        (0x05)
#define A_PDBA       (A_DBA | A_PTR)

#define A_DBN        (0x06)
#define A_PDBN       (A_DBN | A_PTR)

#define A_DOTS       (A_RDM | 0x00)
#define A_STR        (A_RDM | 0x01)
#define A_SET        (A_RDM | 0x02)
#define A_FIELD      (A_RDM | 0x03)
#define A_REC        (A_RDM | 0x04)
#define A_FNO        (A_RDM | 0x05)
#define A_LOCK       (A_RDM | 0x06)
#define A_OPTS       (A_RDM | 0x07)
#define A_LMC        (A_RDM | 0x10)
#define A_STAT       (A_RDM | 0x11)
#define A_TOPIC      (A_RDM | 0x12)
#define A_ELEM       (A_RDM | 0x13)
#define A_DATA       (A_RDM | 0x14)


/*
   Define the various parameter lists
*/

#define P_EMPTY       0
#define P_SKIP        1
#define P_STR         2
#define P_STR_STR     3
#define P_RENFILE     4
#define P_DATA        5
#define P_a           6
#define P_pa          7
#define P_a_ul        8
#define P_F           9
#define P_F_STR      10
#define P_F_DATA     11
#define P_FNO        12
#define P_FNO_l      13
#define P_i          14
#define P_i_i        15
#define P_i_LOCK     16
#define P_LMC        17
#define P_OPTS       18
#define P_R          19
#define P_R_STR      20
#define P_R_DATA     21
#define P_S          22
#define P_S_STR      23
#define P_S_pa       24
#define P_S_F        25
#define P_S_F_DATA   26
#define P_S_S        27
#define P_S_ul       28
#define P_s_ul       29
#define P_INTERNALS  30
#define P_MAPCHAR    31
#define P_ul         32
#define P_STAT_i     33
#define P_DBN        34

#if 0 

static unsigned char *api_parms[] =
{
   /* P_EMPTY     */  { A_NULL },
   /* P_SKIP      */  { A_DOTS, A_NULL },
   /* P_STR       */  { A_STR, A_NULL },
   /* P_STR_STR   */  { A_STR, A_STR, A_NULL },
   /* P_RENFILE   */  { A_STR, A_FNO, A_NULL },
   /* P_DATA      */  { A_DATA, A_NULL },
   /* P_a         */  { A_DBA, A_NULL },
   /* P_pa        */  { A_PDBA, A_NULL },
   /* P_a_ul      */  { A_DBA, A_ULONG, A_NULL },
   /* P_F         */  { A_FIELD, A_NULL },
   /* P_F_STR     */  { A_FIELD, A_STR, A_NULL },
   /* P_F_DATA    */  { A_FIELD, A_DATA, A_NULL },
   /* P_FNO       */  { A_FNO, A_NULL },
   /* P_FNO_l     */  { A_FNO, A_LONG, A_NULL },
   /* P_i         */  { A_INT, A_NULL },
   /* P_i_i       */  { A_INT, A_INT, A_NULL },
   /* P_i_LOCK    */  { A_INT, A_LOCK, A_NULL },
   /* P_LMC       */  { A_LMC, A_NULL },
   /* P_OPTS      */  { A_OPTS, A_NULL },
   /* P_R         */  { A_REC, A_NULL },
   /* P_R_STR     */  { A_REC, A_STR, A_NULL },
   /* P_R_DATA    */  { A_REC, A_DATA, A_NULL },
   /* P_S         */  { A_SET, A_NULL },
   /* P_S_STR     */  { A_SET, A_STR, A_NULL },
   /* P_S_pa      */  { A_SET, A_PDBA, A_NULL },
   /* P_S_F       */  { A_SET, A_FIELD, A_NULL },
   /* P_S_F_DATA  */  { A_SET, A_FIELD, A_DATA, A_NULL },
   /* P_S_S       */  { A_SET, A_SET, A_NULL },
   /* P_S_ul      */  { A_SET, A_ULONG, A_NULL },
   /* P_s_ul      */  { A_SHORT, A_ULONG, A_NULL },
   /* P_INTERNALS */  { A_TOPIC, A_ELEM, A_NULL },
   /* P_MAPCHAR   */  { A_UCHAR, A_UCHAR, A_STR, A_UCHAR, A_NULL },
   /* P_ul        */  { A_ULONG, A_NULL },
   /* P_STAT_i    */  { A_STAT, A_INT, A_NULL },
   /* P_DBN       */  { A_DBN, A_NULL }
};

#endif

typedef struct
{
   char *name;
   int   parms;
} APIDEF;

/*
   d_ API names
*/

static APIDEF rdm_api[] =
{
   { "d_alignment_check", P_EMPTY },
   { "d_checkid",     P_STR },
   { "d_close",       P_EMPTY },
   { "d_closeall",    P_EMPTY },
   { "d_cmstat",      P_S },
   { "d_cmtype",      P_S },
   { "d_connect",     P_S },
   { "d_costat",      P_S },
   { "d_cotype",      P_S },
   { "d_crget",       P_EMPTY },
   { "d_crread",      P_F },
   { "d_crset",       P_pa },
   { "d_crstat",      P_EMPTY },
   { "d_crtype",      P_S },
   { "d_crwrite",     P_F_DATA },
   { "d_csmget",      P_S },
   { "d_csmread",     P_S_F },
   { "d_csmset",      P_S_pa },
   { "d_csmwrite",    P_S_F_DATA },
   { "d_csoget",      P_S },
   { "d_csoread",     P_S_F },
   { "d_csoset",      P_S_pa },
   { "d_csowrite",    P_S_F_DATA },
   { "d_csstat",      P_S },
   { "d_ctbpath",     P_STR },
   { "d_ctscm",       P_S },
   { "d_ctsco",       P_S },
   { "d_ctscr",       P_EMPTY },
   { "d_curkey",      P_EMPTY },
   { "d_dbdpath",     P_STR },
   { "d_dberr",       P_i },
   { "d_dbfpath",     P_STR },
   { "d_dblog",       P_STR },
   { "d_dbnum",       P_STR },
   { "d_dbstat",      P_STAT_i },
   { "d_dbtaf",       P_STR },
   { "d_dbtmp",       P_STR },
   { "d_dbuserid",    P_STR },
   { "d_def_opt",     P_OPTS },
   { "d_delete",      P_EMPTY },
   { "d_destroy",     P_STR },
   { "d_discon",      P_S },
   { "d_disdel",      P_EMPTY },
   { "d_fillnew",     P_R_DATA },
   { "d_findco",      P_S },
   { "d_findfm",      P_S },
   { "d_findlm",      P_S },
   { "d_findnm",      P_S },
   { "d_findpm",      P_S },
   { "d_fldnum",      P_F },
   { "d_freeall",     P_EMPTY },
   { "d_gtscm",       P_S },
   { "d_gtsco",       P_S },
   { "d_gtscr",       P_EMPTY },
   { "d_gtscs",       P_S },
   { "d_iclose",      P_EMPTY },
   { "d_initfile",    P_FNO },
   { "d_initialize",  P_EMPTY },
   { "d_internals",   P_INTERNALS },
   { "d_iopen",       P_STR },
   { "d_ismember",    P_S },
   { "d_isowner",     P_S },
   { "d_keybuild",    P_EMPTY },
   { "d_keydel",      P_F },
   { "d_keyexist",    P_F },
   { "d_keyfind",     P_F_DATA },
   { "d_keyfree",     P_F },
   { "d_keyfrst",     P_F },
   { "d_keylast",     P_F },
   { "d_keylock",     P_F_STR },
   { "d_keylstat",    P_F },
   { "d_keynext",     P_F },
   { "d_keyprev",     P_F },
   { "d_keyread",     P_EMPTY },
   { "d_keystore",    P_F },
   { "d_lmstat",      P_STR },
   { "d_lock",        P_i_LOCK },
   { "d_lockcomm",    P_LMC },
   { "d_lockmgr",     P_STR },
   { "d_makenew",     P_R },
   { "d_mapchar",     P_MAPCHAR },
   { "d_members",     P_S },
   { "d_off_opt",     P_OPTS },
   { "d_on_opt",      P_OPTS },
   { "d_open",        P_STR_STR },
   { "d_rdcurr",      P_EMPTY },
   { "d_rdmini",      P_STR },
   { "d_recfree",     P_R },
   { "d_recfrst",     P_R },
   { "d_reclast",     P_R },
   { "d_reclock",     P_R_STR },
   { "d_reclstat",    P_R },
   { "d_recnext",     P_EMPTY },
   { "d_recnum",      P_R },
   { "d_recover",     P_STR },
   { "d_recprev",     P_EMPTY },
   { "d_recread",     P_EMPTY },
   { "d_recset",      P_R },
   { "d_recstat",     P_a_ul },
   { "d_recwrite",    P_DATA },
   { "d_renclean",    P_EMPTY },
   { "d_renfile",     P_RENFILE },
   { "d_rerdcurr",    P_EMPTY },
   { "d_rlbclr",      P_EMPTY },
   { "d_rlbset",      P_EMPTY },
   { "d_rlbtst",      P_EMPTY },
   { "d_set_alignment_proc", P_SKIP },
   { "d_set_dberr",   P_SKIP },
   { "d_setdb",       P_DBN },
   { "d_setfiles",    P_i },
   { "d_setfree",     P_S },
   { "d_setkey",      P_F_DATA },
   { "d_setlock",     P_S_STR },
   { "d_setlstat",    P_S },
   { "d_setmm",       P_S_S },
   { "d_setmo",       P_S_S },
   { "d_setmr",       P_S },
   { "d_setnum",      P_S },
   { "d_setom",       P_S_S },
   { "d_setoo",       P_S_S },
   { "d_setor",       P_S },
   { "d_setpages",    P_i_i },
   { "d_setrm",       P_S },
   { "d_setro",       P_S },
   { "d_stscm",       P_S_ul },
   { "d_stsco",       P_S_ul },
   { "d_stscr",       P_ul },
   { "d_stscs",       P_S_ul },
   { "d_timeout",     P_i },
   { "d_trabort",     P_EMPTY },
   { "d_trbegin",     P_STR },
   { "d_trend",       P_EMPTY },
   { "d_utscm",       P_S },
   { "d_utsco",       P_S },
   { "d_utscr",       P_EMPTY },
   { "d_utscs",       P_S },
   { "d_wrcurr",      P_SKIP },
   { "dt_opentask",   P_EMPTY },
   { "dt_closetask",  P_EMPTY },
   { "dt_newcache",   P_EMPTY },
   { "dt_sharecache", P_SKIP },
   { "d_lmclear",     P_STR }
};

/*
   LMC API names
*/
static APIDEF lmc_api[] =
{
   { "l_lmcalloc", P_EMPTY },
   { "l_lmcavail", P_EMPTY },
   { "l_lmcclose", P_EMPTY },
   { "l_lmcfree",  P_EMPTY },
   { "l_lmcopen",  P_EMPTY },
   { "l_lmctrans", P_EMPTY },

   { "lmcg_lockcomm",    P_EMPTY },
   { "lmcnetb_lockcomm", P_EMPTY },
   { "lmcnone_lockcomm", P_EMPTY },
   { "lmcnov_lockcomm",  P_EMPTY },
   { "lmcspx_lockcomm",  P_EMPTY }
};

/*
   Query API names
*/
static APIDEF qry_api[] =
{
   { "dt_query",          P_EMPTY },
   { "q_batch",           P_EMPTY },
   { "q_close",           P_EMPTY },
   { "q_def_macro",       P_EMPTY },
   { "q_get_global",      P_EMPTY },
   { "q_getmacro",        P_EMPTY },
   { "q_hdrline",         P_EMPTY },
   { "q_lockcount",       P_EMPTY },
   { "q_nextrow",         P_EMPTY },
   { "q_nextval",         P_EMPTY },
   { "q_off_opt",         P_EMPTY },
   { "q_on_opt",          P_EMPTY },
   { "q_open",            P_EMPTY },
   { "q_qdbpath",         P_EMPTY },
   { "q_qdfpath",         P_EMPTY },
   { "q_qrfpath",         P_EMPTY },
   { "q_set_ctod",        P_EMPTY },
   { "q_set_displayfunc", P_EMPTY },
   { "q_set_function",    P_EMPTY },
   { "q_set_global",      P_EMPTY },
   { "q_set_printerfunc", P_EMPTY },
   { "q_set_translate",   P_EMPTY },
   { "q_set_workingfunc", P_EMPTY },
   { "q_sqlinit",         P_EMPTY },
   { "qt_closetask",      P_EMPTY },
   { "qt_opentask",       P_EMPTY }
};

#endif /* APINAMES_H */

/* $Revision:   1.4  $ */

/* vpp APINAMES.H (Fri Sep 19 11:56:08 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

