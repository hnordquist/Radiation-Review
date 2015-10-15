/*----------------------------------------------------------------------
   query.h  -- db_QUERY status code and external variables header file

   Copyrights (c) Raima Corporation 1986 - 1993, All Rights Reserved.
----------------------------------------------------------------------*/

#ifndef _QUERY_
#define _QUERY_



#ifdef NO_PASCAL
#undef  _pascal
#define _pascal /**/
#endif

#ifndef DB_ADDR
#include "vista.h"
#endif                                 /* DB_ADDR */

#ifdef WINDOWS
#endif

#define DB_LOADDS /**/
#define WFAR      /**/
#define DB_EXPORT /**/

#define WFAR      /**/
#define DB_LOADDS /**/
#define DB_EXPORT /**/

#ifndef MULTI_TASK
extern volatile int  q_status;   /* Status code set by db_QUERY functions */
#endif

/* q_nextval value container */
#ifndef OM_EXPORT
#define OM_EXPORT    /* needed by OMAN */
#endif

/* The VALUE struct should never be packed except under Windows */
#include "packlib.h"

typedef struct OM_EXPORT expression_value
{
   union OM_EXPORT
   {
      char WFAR  *cv;      /* character result value */
      double      dv;      /* long result value */
      long        lv;      /* long result value */
      int         iv;      /* integer result value */
      DB_ADDR     av;      /* database address result value */
      struct {
         char WFAR  *ptr;  /* binary char array */
         size_t      len;  /* Size of buffer pointed to by 'ptr'. */
      }           bv;      /* binary char value */
   }  result;
   short          type;    /* 'i' = int, 'l' = long,  'F' = double, 
                              'u' = unsigned int,     'U' = unsigned long, 
                              'd' = database address, 'z' = div by zero,
                              'c' = string,           'b' = binary char data,
                              'n' = null data,        '\0' = no data, 
                           */
} VALUE;

#include "nopack.h"

/* db_QUERY status code constants */
#define Q_OKAY           0

#define Q_END           -1
#define Q_SET           -2
#define Q_DEFINE        -3
#define Q_FIELD         -4
#define Q_RELATION      -5
#define Q_VIEW          -5
#ifndef Q_BATCH   /* might have been defined first by apidefs.h */
#define Q_BATCH         -6
#endif
#define Q_HELP          -7
#define Q_SHOW          -8
#define Q_NOTFOUND      -9
#define Q_UNAVAIL      -10
#define Q_BREAK        -11

/* db_QUERY error code constants */
/* The ones commented out are obsolete, but not deleted for maintenance. */
#define Q_INVSTMT        1
#define Q_INVFLD         2
#define Q_SYSERR         3
#define Q_FLDEXPR        4
#define Q_DBCLOSE        5
#define Q_PROCNEEDED     6
#define Q_ARGNUM         7
#define Q_FRMFORMAT      8
#define Q_FLDREL         9
#define Q_SORTED        10
#define Q_PRINTER       11
#define Q_FILERR        12
#define Q_INTOERR       13
#define Q_NOMEMORY      14
#define Q_LINKTO        15
#define Q_FLDSPEC       16
#define Q_DECIMALS      17
#define Q_INVNUM        18
#define Q_GROUPED       19
#define Q_LENGTH        20
#define Q_WIDTH         21
#define Q_TITLE         22
#define Q_DATE          23
#define Q_BADREL        24
#define Q_YACCSTACK     25
#define Q_INVREC        26
#define Q_INVTHRU       27
#define Q_SETREL        28
#define Q_LINKREC       29
#define Q_NOFIELD       30
#define Q_NOLINK        31
#define Q_INVREL        32
/*
   #define Q_NOTSELECTED   33
*/
#define Q_MAXFIELDS     34
#define Q_TOOBIG        35
#define Q_NOBATCH       36
#define Q_SORTKEY       37
#define Q_NOFORM        38
#define Q_DDLFIELD      39
#define Q_DATEFMT       40
#define Q_TOOLONG       41
#define Q_OUTPUT        42
/*
   #define Q_NOTAKEY       43
   #define Q_INVPATHDIR    44
   #define Q_NOSELECT      45
   #define Q_FLDREF        46
   #define Q_NOEXPR        47
*/
#define Q_INVEXPR       48
/*
   #define Q_INVTERM       49
   #define Q_NOTERM        50
   #define Q_INVPRIM       51
   #define Q_MATHOPR       52
   #define Q_HUGE          53
*/
#define Q_INVVAL        54
#define Q_INVTYPE       55
#define Q_INVCALC       56
#define Q_REPVAL        57
#define Q_NOOCCUR       58
/*
   #define Q_INVGROUP      59
*/
#define Q_DISTINCT      60
#define Q_SUBSCRIPT     61
#define Q_NULLOUT       62
/*
   #define Q_ONEDB         63
*/
#define Q_MAXDIMS       64
#define Q_SUBRANGE      65
#define Q_INVDB         66
/*
   #define Q_KEYWORD       67
   #define Q_GROUPSEL      68
*/
#define Q_MAXSORT       69
#define Q_INVFMT        70
#define Q_NOMOREFLDS    71
#define Q_NOSUBS        72
#define Q_MAXCOLS       73
#define Q_MAXARGS       74
#define Q_MAXGROUP      75
#define Q_COLREF        76
#define Q_ORDPATH       77
#define Q_RECREL        78
#define Q_DIVBYZERO     79
#define Q_IFEXPR        80
#define Q_DISTARG       81
#define Q_SMALLCACHE    82
#define Q_BIGFORM       83
#define Q_DBOPEN        84
#define Q_NUMARGS       85
#define Q_INVFUNC       86
#define Q_INVARG        87
#define Q_BASEYR        88
#define Q_FLDWIDTH      89
#define Q_INVNOFMT      90
#define Q_FORKEY        91
#define Q_DISPLAY       92
#define Q_INVTASK       93
#define Q_INVPARM       94

#ifdef __cplusplus
extern "C"
{
#endif

#define QEXTERNAL_FIXED WFAR PASCAL DB_LOADDS DB_EXPORT

/**************** db_QUERY User Function Prototypes **********************/

   typedef void (QEXTERNAL_FIXED * QCTODPROC)(
                  CONST char WFAR *, double WFAR *);
   typedef int  (QEXTERNAL_FIXED * QFUNCTIONPROC)(CONST char WFAR *, 
                  int, CONST VALUE WFAR *, VALUE WFAR *);
   typedef void (QEXTERNAL_FIXED * QTRANSLATEPROC)(CONST DB_VOID WFAR *, 
                  long, CONST DB_VOID WFAR *, char WFAR *);
   typedef void (QEXTERNAL_FIXED * QWORKINGPROC)(
                  unsigned long, unsigned long);
   typedef void (QEXTERNAL_FIXED * QDISPLAYPROC)(CONST char WFAR *, 
                  int, unsigned long, int);
   typedef void (QEXTERNAL_FIXED * QPRINTERPROC)(CONST char WFAR *, 
                  int, unsigned long, int, char WFAR *);

#ifdef MULTI_TASK
   int QEXTERNAL_FIXED dt_query(char WFAR *, int WFAR *, 
                                char WFAR * WFAR *, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_closetask(DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_open(char WFAR *, char WFAR *, 
                               DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_opentask(DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_batch(char WFAR *, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_close(DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_get_global(int, void WFAR *, unsigned, 
                                     DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_hdrline(int, char WFAR *, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_lockcount(int, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_nextrow(char WFAR *, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_nextval(int WFAR *, VALUE WFAR *, 
                                  DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_qdbpath(CONST char WFAR *, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_qrfpath(CONST char WFAR *, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_qdfpath(CONST char WFAR *, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_set_ctod(QCTODPROC, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_set_displayfunc(QDISPLAYPROC, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_set_function(QFUNCTIONPROC, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_set_printerfunc(QPRINTERPROC, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_set_translate(QTRANSLATEPROC, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_set_workingfunc(QWORKINGPROC, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_set_global(int, void WFAR *, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_sqlinit(char WFAR *, int WFAR *, 
                                  char WFAR * WFAR *, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_on_opt(unsigned short, DB_TASK WFAR *);
   int QEXTERNAL_FIXED qt_off_opt(unsigned short, DB_TASK WFAR *);

#ifndef   NO_QT_COVER
#define  d_query(a, b, c)       dt_query(a, b, c, &Currtask)
#define  q_open(a, b)           qt_open(a, b, &Currtask)
#define  q_batch(a)             qt_batch(a, &Currtask)
#define  q_close()              qt_close(&Currtask)
#define  q_get_global(a, b, c)  qt_get_global(a, b, c, &Currtask)
#define  q_hdrline(a, b)        qt_hdrline(a, b, &Currtask)
#define  q_lockcount(a)         qt_lockcount(a, &Currtask)
#define  q_nextrow(a)           qt_nextrow(a, &Currtask)
#define  q_nextval(a, b)        qt_nextval(a, b, &Currtask)
#define  q_qdbpath(a)           qt_qdbpath(a, &Currtask)
#define  q_qrfpath(a)           qt_qrfpath(a, &Currtask)
#define  q_qdfpath(a)           qt_qdfpath(a, &Currtask)
#define  q_set_ctod(a)          qt_set_ctod(a, &Currtask)
#define  q_set_displayfunc(a)   qt_set_displayfunc(a, &Currtask)
#define  q_set_printerfunc(a)   qt_set_printerfunc(a, &Currtask)
#define  q_set_function(a)      qt_set_function(a, &Currtask)
#define  q_set_translate(a)     qt_set_translate(a, &Currtask)
#define  q_set_workingfunc(a)   qt_set_workingfunc(a, &Currtask)
#define  q_set_global(a, b)     qt_set_global(a, b, &Currtask)
#define  q_sqlinit(a, b, c)     qt_sqlinit(a, b, c, &Currtask)
#define  q_on_opt(a)            qt_on_opt(a, &Currtask)
#define  q_off_opt(a)           qt_off_opt(a, &Currtask)
#endif         /* NOT NO_QT_COVER */

#else          /* MULTI_TASK */
   int  QEXTERNAL_FIXED d_query(char WFAR *stmt, int WFAR *epos, 
                                char WFAR *WFAR *emsg);
   int  QEXTERNAL_FIXED q_batch(char WFAR *batfile);
   int  QEXTERNAL_FIXED q_close(void);
   int  QEXTERNAL_FIXED q_hdrline(int hline, char WFAR *text);
   int  QEXTERNAL_FIXED q_open(char WFAR *db_list, char WFAR *type);
   int  QEXTERNAL_FIXED q_lockcount(int);
   int  QEXTERNAL_FIXED q_nextrow(char WFAR *);
   int  QEXTERNAL_FIXED q_nextval(int WFAR *, VALUE WFAR *);
   int  QEXTERNAL_FIXED q_sqlinit(char WFAR *str, int WFAR *ep,
                                  char WFAR * WFAR *em);
   int  QEXTERNAL_FIXED q_set_ctod(QCTODPROC func);
   int  QEXTERNAL_FIXED q_set_displayfunc(QDISPLAYPROC func);
   int  QEXTERNAL_FIXED q_set_printerfunc(QPRINTERPROC func);
   int  QEXTERNAL_FIXED q_set_function(QFUNCTIONPROC func);
   int  QEXTERNAL_FIXED q_set_translate(QTRANSLATEPROC func);
   int  QEXTERNAL_FIXED q_set_workingfunc(QWORKINGPROC func);
   int  QEXTERNAL_FIXED q_get_global(int id, DB_VOID WFAR * fpDestIn, 
                                     unsigned index);
   int  QEXTERNAL_FIXED q_set_global(int id, DB_VOID WFAR * fpSrcIn);
   int  QEXTERNAL_FIXED q_qrfpath(CONST char WFAR * path);
   int  QEXTERNAL_FIXED q_qdfpath(CONST char WFAR * path);
   int  QEXTERNAL_FIXED q_qdbpath(CONST char WFAR * path);
   int  QEXTERNAL_FIXED q_on_opt(unsigned short);
   int  QEXTERNAL_FIXED q_off_opt(unsigned short);

#endif         /* MULTI_TASK */

#ifdef __cplusplus
}
#endif

/**************************************************************************/

#define MAXTRANS      128  /* Max len of translated coded-value string */
#define MAXERRTXT      80  /* Max len of qerrinfo string & error messages */
#define MAXSTMT      4096  /* Max len of any SQL statement in QRF */
#define PRLEN          40  /* max len of qprompt  string */
#define LPLEN          23  /* max len of qprinter string */
#define MAXSTRING     512  /* Max len of any quoted string in a statement */
                           /*    and max length of output string.         */


/* q_get_global and q_set_global global ids */

#define QG_QBREAK          1
#define QG_QECHO           2
#define QG_QERRINFO        3
#define QG_QFORMFEED       4
#define QG_QMAXALIASES     5
#define QG_QTABSTOP        6
#define QG_QUSEOPTKEY      7
#define QG_QREPSIZE        8
#define QG_QTRACE          9
#define QG_QERRPOS         10
#define QG_QERRMSG         11
#define QG_QPROMPT         12
#define QG_QLOCKCOUNT      13
#define QG_QWILDONE        14
#define QG_QWILDALL        15
#define QG_QCOLSEP         16
#define QG_QROWSEP         17
#define QG_QWIDTH          18
#define QG_QLENGTH         19
#define QG_QYIELDCOUNT     20       /* used under Windows only */
#define QG_QHASHSIZE       21
#define QG_QOPTIONS        22
#define QG_QDBPATH         23
#define QG_QDFPATH         24
#define QG_QRFPATH         25
#define QG_QMEMORY         26
#define QG_QSTATUS         27

#define QO_VIEW2CACHE      0x0001   /* check worst case view vs cache size */
#define QO_TITLE2UPPER     0x0002   /* force default column titles to CAPS */
#define QO_DATEFROM1970    0x0004   /* system date starts at 1/1/1970 or needs not be converted */
#define QO_QUERYPLAN       0x0008   /* print query optimization plan */

#define QO_MEMROLL			0x0008   /* RESERVED */

/* display and printer function line output types */
#define    Q_HEADERLINE    0
#define    Q_DETAILLINE    1
#define    Q_BREAKLINE     2
#define    Q_FOOTERLINE    3
#define    Q_SHOWLINE      4
#define    Q_ERRORLINE     5
#define    Q_FORMFEEDLINE  6

#endif /* _QUERY_ */

/* $Revision:   1.82  $ */

/* vpp QUERY.H (Fri Sep 19 11:56:12 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

