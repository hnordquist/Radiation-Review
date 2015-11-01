/*-----------------------------------------------------------------------

   apidefs.h - Give each API function a number.

------------------------------------------------------------------------*/

#ifndef APIDEFS_H
#define APIDEFS_H


/*
   Query API
   We use negative values because we must align Q_BATCH with its definition
   in query.h.
*/
#define D_QUERY                 -5
#ifndef Q_BATCH   /* must match definition in query.h */
#define Q_BATCH                 -6
#endif                         
#define Q_CLOSE                 -7
#define Q_DEFMACRO              -8
#define Q_GET_GLOBAL            -9
#define Q_GETMACRO             -10
#define Q_HDRLINE              -11
#define Q_LOCKCOUNT            -12
#define Q_NEXTROW              -13
#define Q_NEXTVAL              -14
#define Q_OFF_OPT              -15
#define Q_ON_OPT               -16
#define Q_OPEN                 -17
#define Q_QDBPATH              -18
#define Q_QDFPATH              -19
#define Q_QRFPATH              -20
#define Q_SET_CTOD             -21
#define Q_SET_DISPLAYFUNC      -22
#define Q_SET_FUNCTION         -23
#define Q_SET_GLOBAL           -24
#define Q_SET_PRINTERFUNC      -25
#define Q_SET_TRANSLATE        -26
#define Q_SET_WORKINGFUNC      -27
#define Q_SQLINIT              -28
#define QT_CLOSETASK           -29
#define QT_OPENTASK            -30

#define QRY_FIRST  D_QUERY
#define QRY_LAST   QT_OPENTASK


/*
   d_ API
   (can't use 0-99 because we use absolute value of Query defs)
*/
#define D_ALIGNMENT_CHECK      100
#define D_CHECKID              101
#define D_CLOSE                102
#define D_CLOSEALL             103
#define D_CMSTAT               104
#define D_CMTYPE               105
#define D_CONNECT              106
#define D_COSTAT               107
#define D_COTYPE               108
#define D_CRGET                109
#define D_CRREAD               110
#define D_CRSET                111
#define D_CRSTAT               112
#define D_CRTYPE               113
#define D_CRWRITE              114
#define D_CSMGET               115
#define D_CSMREAD              116
#define D_CSMSET               117
#define D_CSMWRITE             118
#define D_CSOGET               119
#define D_CSOREAD              120
#define D_CSOSET               121
#define D_CSOWRITE             122
#define D_CSSTAT               123
#define D_CTBPATH              124
#define D_CTSCM                125
#define D_CTSCO                126
#define D_CTSCR                127
#define D_CURKEY               128
#define D_DBDPATH              129
#define D_DBERR                130
#define D_DBFPATH              131
#define D_DBLOG                132
#define D_DBNUM                133
#define D_DBSTAT               134
#define D_DBTAF                135
#define D_DBTMP                136
#define D_DBUSERID             137
#define D_DEF_OPT              138
#define D_DELETE               139
#define D_DESTROY              140
#define D_DISCON               141
#define D_DISDEL               142
#define D_FILLNEW              143
#define D_FINDCO               144
#define D_FINDFM               145
#define D_FINDLM               146
#define D_FINDNM               147
#define D_FINDPM               148
#define D_FLDNUM               149
#define D_FREEALL              150
#define D_GTSCM                151
#define D_GTSCO                152
#define D_GTSCR                153
#define D_GTSCS                154
#define D_ICLOSE               155
#define D_INITFILE             156
#define D_INITIALIZE           157
#define D_INTERNALS            158
#define D_IOPEN                159
#define D_ISMEMBER             160
#define D_ISOWNER              161
#define D_KEYBUILD             162
#define D_KEYDEL               163
#define D_KEYEXIST             164
#define D_KEYFIND              165
#define D_KEYFREE              166
#define D_KEYFRST              167
#define D_KEYLAST              168
#define D_KEYLOCK              169
#define D_KEYLSTAT             170
#define D_KEYNEXT              171
#define D_KEYPREV              172
#define D_KEYREAD              173
#define D_KEYSTORE             174
#define D_LMSTAT               175
#define D_LOCK                 176
#define D_LOCKCOMM             177
#define D_LOCKMGR              178
#define D_MAKENEW              179
#define D_MAPCHAR              180
#define D_MEMBERS              181
#define D_OFF_OPT              182
#define D_ON_OPT               183
#define D_OPEN                 184
#define D_RDCURR               185
#define D_RDMINI               186
#define D_RECFREE              187
#define D_RECFRST              188
#define D_RECLAST              189
#define D_RECLOCK              190
#define D_RECLSTAT             191
#define D_RECNEXT              192
#define D_RECNUM               193
#define D_RECOVER              194
#define D_RECPREV              195
#define D_RECREAD              196
#define D_RECSET               197
#define D_RECSTAT              198
#define D_RECWRITE             199
#define D_RENCLEAN             200
#define D_RENFILE              201
#define D_RERDCURR             202
#define D_RLBCLR               203
#define D_RLBSET               204
#define D_RLBTST               205
#define D_SET_ALIGNMENT_PROC   206
#define D_SET_DBERR            207
#define D_SETDB                208
#define D_SETFILES             209
#define D_SETFREE              210
#define D_SETKEY               211
#define D_SETLOCK              212
#define D_SETLSTAT             213
#define D_SETMM                214
#define D_SETMO                215
#define D_SETMR                216
#define D_SETNUM               217
#define D_SETOM                218
#define D_SETOO                219
#define D_SETOR                220
#define D_SETPAGES             221
#define D_SETRM                222
#define D_SETRO                223
#define D_STSCM                224
#define D_STSCO                225
#define D_STSCR                226
#define D_STSCS                227
#define D_TIMEOUT              228
#define D_TRABORT              229
#define D_TRBEGIN              230
#define D_TREND                231
#define D_UTSCM                232
#define D_UTSCO                233
#define D_UTSCR                234
#define D_UTSCS                235
#define D_WRCURR               236
#define DT_OPENTASK            237
#define DT_CLOSETASK           238
#define DT_NEWCACHE            239
#define DT_SHARECACHE          240
#define D_LMCLEAR              241

#define RDM_FIRST  D_ALIGNMENT_CHECK
#define RDM_LAST   D_LMCLEAR


/*
   LMC API
*/
#define L_LMCALLOC             300
#define L_LMCAVAIL             301
#define L_LMCCLOSE             302
#define L_LMCFREE              303
#define L_LMCOPEN              304
#define L_LMCTRANS             305

#define LMCG_LOCKCOMM          306
#define LMCNETB_LOCKCOMM       307
#define LMCNONE_LOCKCOMM       308
#define LMCNOV_LOCKCOMM        309
#define LMCSPX_LOCKCOMM        310
#define LMCTCP_LOCKCOMM        311

#define LMC_FIRST  L_LMCALLOC
#define LMC_LAST   LMCTCP_LOCKCOMM


#endif /* APIDEFS_H */

/* $Revision:   1.4  $ */

/* vpp APIDEFS.H (Fri Sep 19 11:56:10 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

