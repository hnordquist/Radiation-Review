/*----------------------------------------------------------------------
  vista.h: db_VISTA standard header file

   This file should be included in all programs which use db_VISTA.  It
   contains database status/error code definitions and standard type
   definitions.

   This file should only include user-oriented constructs.

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef VISTA_H
#define VISTA_H

#define dbd_VERSION "V3.00\032"

/* ------------------------------------------------------------------------
   Compiler dependencies are handled by #ifdef macros in RDM.
   If the compiler does not automatically identify itself by
   setting a reserved macro symbol, you MUST define the compiler
   on the compile line, for example:
        cc -c -DUNIX file.c

   The available compiler choices are:
      UNIX  = any UNIX compiler
      VMS   = any Vax/VMS compiler
      MSC   = Microsoft compiler   (automatically set by _MSC_VER)
      TURBO = Borland compiler     (automatically set by __BORLANDC__)
      WAT   = Watcom compiler      (automatically set by __WATCOMC__)
      ZOR   = Zortech compiler     (automatically set by __ZTC__)
*/

/* default the compiler if none specified */
#ifndef ZOR
#ifndef WAT
#ifndef TURBO
#ifndef MSC
/* determine the default */
#ifdef __ZTC__
#define ZOR
#else
#ifdef __WATCOMC__
#define WAT
#else
#ifdef __BORLANDC__
#define TURBO
#else
#ifdef _MSC_VER
#define MSC
#endif /* _MSC_VER */
#endif /* __BORLANDC__ */
#endif /* __WATCOMC__ */
#endif /* __ZTC__ */
#endif /* MSC */
#endif /* TURBO */
#endif /* WAT */
#endif /* ZOR */

#ifndef ZOR
#ifndef WAT
#ifndef TURBO
#ifndef MSC
#error You must specify a supported compiler in the compile command!
#endif /* MSC */
#endif /* TURBO */
#endif /* WAT */
#endif /* ZOR */

#define ANSI
#ifndef LINT_ARGS
#define LINT_ARGS
#endif



#ifdef NOLIB
#define DB_LIB
#ifdef MULTI_TASK
#undef MULTI_TASK
#endif
#endif

#ifndef DB_DLL
#ifndef DB_LIB
#ifndef USE_DB_DLL
#ifndef USE_DB_LIB
#define USE_DB_DLL
#endif
#endif
#endif
#endif


#ifdef DB_DLL
#define MULTI_TASK
#define NO_DT_COVER  /* NO_DT_COVER must be defined before lockcomm.h or task.h */
#endif

#ifdef USE_DB_DLL
#define MULTI_TASK
#endif


#define NO_PASCAL

#ifdef MULTI_TASK
#define NO_DB_GLOBAL
#endif

#ifndef NOLIB
/* 
   Comment out the following line if you want to use ANSI I/O functions
   instead of Win32 native IO.

   Note that, if NOLIB is defined (for building the RDM utilities)
   WIN32_IO cannot be defined - the utilities have not been modified
   to use Win32 native IO calls.
*/
#define WIN32_IO

#endif /* NOLIB */



/* ======================================================================== */
/* Status Codes                                                             */
/* ======================================================================== */

/*
   User errors
*/
#define S_USER_FIRST       -1    /* first user err */

#define S_DBOPEN           -1    /* database not opened */
#define S_INVSET           -2    /* invalid set */
#define S_INVREC           -3    /* invalid record */
#define S_INVDB            -4    /* invalid database */
#define S_INVFLD           -5    /* invalid field name */
#define S_INVADDR          -6    /* invalid db_address */
#define S_NOCR             -7    /* no current record */
#define S_NOCO             -8    /* set has no current owner */
#define S_NOCM             -9    /* set has no current member */
#define S_KEYREQD          -10   /* key value required */
#define S_BADTYPE          -11   /* invalid lock type */
#define S_HASMEM           -12   /* record is owner of non-empty set(s) */
#define S_ISMEM            -13   /* record is member of set(s) */
#define S_ISOWNED          -14   /* member already owned */
#define S_ISCOMKEY         -15   /* field is a compound key */
#define S_NOTCON           -16   /* record not connected to set */
#define S_NOTKEY           -17   /* field is not a valid key */
#define S_INVOWN           -18   /* record not legal owner of set */
#define S_INVMEM           -19   /* record not legal member of set */
#define S_SETPAGES         -20   /* error in d_setpages (database open or bad param) */
#define S_INCOMPAT         -21   /* incompatible dictionary file */
#define S_DELSYS           -22   /* illegal attempt to delete system record */
#define S_NOTFREE          -23   /* attempt to lock previously locked rec or set */
#define S_NOTLOCKED        -24   /* attempt to access unlocked record or set */
#define S_TRANSID          -25   /* transaction id not be supplied */
#define S_TRACTIVE         -26   /* transaction already active */
#define S_TRNOTACT         -27   /* transaction not currently active */
#define S_BADPATH          -28   /* the directory string is invalid */
#define S_TRFREE           -29   /* attempt to free a lock inside a transaction */
#define S_RECOVERY         -30   /* Auto-recovery is about to occur */
#define S_NOTRANS          -31   /* attempted update outside of transaction */
#define S_EXCLUSIVE        -32   /* functions requires exclusive db access */
#define S_STATIC           -33   /* Attempted to write lock a static file */
#define S_USERID           -34   /* No user id exists */
#define S_NAMELEN          -35   /* database file/path name too long */
#define S_RENAME           -36   /* invalid file number passed to d_renfile */
#define S_NOTOPTKEY        -37   /* field is not an optional key */
#define S_BADFIELD         -38   /* field not defined in current record type */
#define S_COMKEY           -39   /* record/field has/in a compound key */
#define S_INVNUM           -40   /* invalid record or set number */
#define S_TIMESTAMP        -41   /* record or set not timestamped */
#define S_BADUSERID        -42   /* invalid user id - not alphanumeric */
#define S_NOPROTOCOL       -43   /* can not initialize LMC communications */
/* -44 */
/* -45 */
#define S_NOTYPE           -46   /* No current record type */
#define S_INVSORT          -47   /* Invalid country table sort string */
#define S_DBCLOSE          -48   /* database not closed */
#define S_INVPTR           -49   /* invalid pointer */
#define S_INVID            -50   /* invalid internal ID */
#define S_INVLOCK          -51   /* invalid lockmgr communication type */
#define S_INVTASK   			-52   /* invalid task id */
#define S_NOLOCKCOMM       -53   /* must call d_lockcomm() before d_open() */
#define S_NOTIMPLEMENTED   -54  

#define S_USER_LAST        -54   /* last user error */

/*
   System errors
*/
#define S_SYSTEM_FIRST     -900  /* first system error */

#define S_NOSPACE          -900  /* out of disk space */
#define S_SYSERR           -901  /* system error */
#define S_FAULT            -902  /* page fault -- too many locked pages */
#define S_NOWORK           -903  /* cannot access dbQuery dictionary */
#define S_NOMEMORY         -904  /* unable to allocate sufficient memory */
#define S_NOFILE           -905  /* unable to locate a file */
#define S_DBLACCESS        -906  /* unable to open TAF or DBL or log file */
#define S_DBLERR           -907  /* TAF or DBL I/O error */
#define S_BADLOCKS         -908  /* inconsistent database locks */
#define S_RECLIMIT         -909  /* file record limit reached */
#define S_KEYERR           -910  /* key file inconsistency detected */
#define S_TTSUNAVAILABLE   -911  /* Novell's Transaction Tracking is not on */
#define S_FSEEK            -912  /* Bad seek on database file */
#define S_LOGIO            -913  /* error reading/writing log file */
#define S_READ             -914  /* Bad read on database/overflow file */
#define S_NETSYNC          -915  /* Network synchronization error */
#define S_DEBUG            -916  /* Debugging check interrupt */
#define S_NETERR           -917  /* Network communications error */
/* -918 */
#define S_WRITE            -919  /* Bad write on database/overflow file */
#define S_NOLOCKMGR        -920  /* Unable to open lockmgr session */
#define S_DUPUSERID        -921  /* DBUSERID is already used by someone else */
#define S_LMBUSY           -922  /* The lock manager table(s) are full */
#define S_DISCARDED        -923  /* attempt to lock discarded memory */
#define S_SEM              -924  /* unable to open semaphore / mutex */
#define S_LMCERROR         -925  /* Network layer error code */
#define S_TTSERROR         -926  /* Novell Transaction Tracking Services error */
/* -927 */
/* -928 */
/* -929 */
/* -930 */
/* -931 */
/* -932 */
/* -933 */
/* -934 */
/* -935 */
/* -936 */
#define S_CLOSE     	      -937  /* Unable to close a file handle */
#define S_NODBQUERY        -938  /* db_QUERY not linked with this DLL */
#define S_READONLY         -939  /* d_on_opt(READONLY), unable to update file */
#define S_EACCESS          -940  /* sharing violation, file in use */
#define S_NOSHARE          -941  /* share not loaded */
#define S_PROTECTED        -942  /* must run in protected mode */
#define S_RECFAIL          -943  /* automatic recovery failed */
#define S_TAFSYNC          -944  /* TAF file not consistant with LMC */
#define S_BLOCKIOPG        -945  /* VMS block io problem */
#define S_ALIGNMENT        -946  /* dictionary alignment does not match program */
#define S_REENTER          -947  /* RDM entered re-entrantly */

#define S_SYSTEM_LAST      -947  /* last system error */

/*
   Internal system errors
*/
#define S_INTERNAL_FIRST   -9001 /* first internal error */

#define SYS_BADLEVEL       -9001 /* bad nesting level */
#define SYS_INVOWNER       -9002 /* illegal owner record */
#define SYS_BADTREE        -9003 /* b-tree malformed */
#define SYS_KEYEXIST       -9004 /* key value already exists */
#define SYS_INVHANDLE      -9005 /* free of invalid handle (WINDOWS) */
#define SYS_LOCKARRAY      -9006 /* lock packet array exceeded */
#define SYS_INTERNALMEM    -9007 /* not enough memory for shared table */
#define SYS_BADFREE        -9008 /* attempt to free empty table */
#define SYS_BADOPTKEY      -9009 /* calculating optkey index */
#define SYS_TIMER          -9010 /* unable to set system time (SUN) */
#define SYS_IXNOTCLEAR     -9011 /* ix-cache not reset after trans */
#define SYS_INVLOGPAGE     -9012 /* invalid page in log file */
#define SYS_INVFLDTYPE     -9013 /* illegal field type */
#define SYS_INVSORT        -9014 /* illegal sort ordering */
#define SYS_BADREOPEN      -9015 /* bad reopen status (WINDOWS) */
#define SYS_INVPGTAG       -9016 /* invalid page tag */
#define SYS_INVHOLD        -9017 /* bad hold count */
#define SYS_HASHCYCLE      -9018 /* cycle detected in hash chain */
#define SYS_INVLRU         -9019 /* invalid lru page */
#define SYS_INVPAGE        -9020 /* invalid cache page */
#define SYS_INVPGCOUNT     -9021 /* bad page tag page count */
#define SYS_INVPGSIZE      -9022 /* invalid cache page size */
#define SYS_PZACCESS       -9023 /* invalid access to page zero */
#define SYS_BADPAGE        -9024 /* wrong page */
#define SYS_INVEXTEND      -9025 /* illegal attempt to extend file */
#define SYS_BADREAD        -9026 /* bad read */
#define SYS_PZNEXT         -9027 /* bad pznext */
#define SYS_DCHAIN         -9028 /* bad dchain */
#define SYS_EOF            -9029 /* attempt to write past EOF */
#define SYS_FILEMODIFIED   -9030 /* locked file was modified by another user */

#define S_INTERNAL_LAST    -9030 /* last internal error */

/*
   Function statuses
*/
#define S_FUNCTION_FIRST   0     /* first function status */

#define S_OKAY             0     /* normal return, okay */
#define S_EOS              1     /* end of set */
#define S_NOTFOUND         2     /* record not found */
#define S_DUPLICATE        3     /* duplicate key */
#define S_KEYSEQ           4     /* field type used out of sequence in d_keynext */
#define S_UNAVAIL          5     /* database file currently unavailable */
#define S_DELETED          6     /* record/set deleted since last accessed */
#define S_UPDATED          7     /* record/set updated since last accessed */
#define S_LOCKED           8     /* current record's lock bit is set */
#define S_UNLOCKED         9     /* current record's lock bit is clear */
#define S_SETCLASH         10    /* set ptrs invalidated by another user */

#define S_FUNCTION_LAST    10    /* last function status */

/*
   User table status
*/
#define U_OKAY          0
#define U_EMPTY         0

#define U_LIVE          1
#define U_DEAD          2
#define U_BEING_REC     3
#define U_RECOVERING    4
#define U_REC_MYSELF    5
#define U_HOLDING_X     6
/*
   User table options
*/
#define U_NO_COMPRESSION    0x0001


/* ------------------------------------------------------------------------ */

#if defined(_ALPHA_)
typedef int DB_SHORT;
#define DB_SHORT_LEN 4
#elif defined (_MIPS_)
typedef int DB_SHORT;
#define DB_SHORT_LEN 4
#else
typedef short DB_SHORT;
#define DB_SHORT_LEN 2
#endif
#if defined(_ALPHA_)
typedef unsigned int DB_USHORT;
#elif defined (_MIPS_)
typedef unsigned int DB_USHORT;
#else
typedef unsigned short DB_USHORT;
#endif
typedef unsigned int    DB_UINT;
typedef unsigned long   DB_ULONG;
typedef int             DB_BOOLEAN;
typedef long            DB_LONG;

#define MAXRECORDS   0x00FFFFFFL    /* max slots per file */
#define NULL_DBA     0x00000000L
typedef DB_ULONG     DB_ADDR;       /* RDM database address */
typedef DB_SHORT     FILE_NO;       /* RDM file number */
typedef long         F_ADDR;        /* file address: page or record number */

/* field number indicator = rec * FLDMARK + fld_in_rec */
#define FLDMARK 1000L

/* record number indicator */
#define RECMARK 10000

/* set number indicator -- must be greater than RECMARK */
#define SETMARK 20000

/* runtime option flags */
/* hi-order byte reserved for specifying option sets */
#define DCHAINUSE       0x00000001L    /* use the dchain for new records */
#define TRLOGGING       0x00000002L    /* use transaction logging/recovery */
#define ARCLOGGING      0x00000004L    /* use transaction archiving */
#define IGNORECASE      0x00000008L    /* use caseblind compares for strings */
#define SHARETEST       0x00000010L    /* dos/windows: test for share.exe */
#define CLOSEFILES      0x00000020L    /* never keep the files open */
#define GLOBALALLOCS    0x00000040L    /* never use LocalAlloc() */
#define DELETELOG       0x00000080L    /* have d_close() delete the log file */
#define SYNCFILES       0x00000100L    /* bypass any system cache */
#define _NOVELLTRX_     0x00000200L    /* use Novell's TTS, not Raima's */
   /* (NOVELLTRX is used by lockcomm.h and is defined below) */
#define IGNOREENV       0x00000400L    /* ignore environment variables */
#define READONLY        0x00000800L    /* open database for readonly access */
#define TXTEST          0x00001000L    /* test transaction recovery */
#define NORECOVER       0x00002000L    /* For Raima use only, will corrupt databases */
#define DBYIELD         0x00004000L    /* Windows Yield() during d_lock() */
#define TRUNCATELOG     0x00008000L    /* truncate log file at each d_trbegin */
#define PREALLOC_CACHE  0x00020000L    /* allocate cache pages during d_open */
#define REMOTESYNC      0x00040000L    /* use share locking to force write-thru remote cache */
#define _ALIGNMENT_CHECK_ 0x00080000L  /* do runtime alignment checking */
   /* (ALIGNMENT_CHECK is used by lockcomm.h and is defined below) */

/* LMC_OPT cannot be or'd together in one d_on/off_opt() call with non-LMC_OPTs
*/
#define LMC_OPT         0x80000000L
#define LMC_OPT_TRUENAME    (LMC_OPT | 0x00000001L)
#define LMC_OPT_PORTABLE    (LMC_OPT | 0x00000002L)  /* use guard files instead of share.exe or lock daemon */
#define LMC_OPT_DELNAME     (LMC_OPT | 0x00000004L)  /* d_close() removes dbuserid from NetBIOS tables */
#define LMC_OPT_LOGICALSETS (LMC_OPT | 0x00000008L)  /* LMC_NOVELL uses logical record sets */
#define LMC_OPT_SHORTNAME   (LMC_OPT | 0x00002000L)  /* Force use of GetShortPathName to generate short path names for lockmgr */

#define LMC_OPT_LANA        (LMC_OPT | 0x00001000L)  /* If this bit is set, lana has been set */
#define LMC_OPT_LANA0       (LMC_OPT | 0x00001000L)  /* LMC_NETBIOS use lana 0 */
#define LMC_OPT_LANA1       (LMC_OPT | 0x00001010L)  /* LMC_NETBIOS use lana 1 */
#define LMC_OPT_LANA2       (LMC_OPT | 0x00001020L)  /* LMC_NETBIOS use lana 2 */
#define LMC_OPT_LANA3       (LMC_OPT | 0x00001030L)  /* LMC_NETBIOS use lana 3 */
#define LMC_OPT_LANA4       (LMC_OPT | 0x00001040L)  /* LMC_NETBIOS use lana 4 */
#define LMC_OPT_LANA5       (LMC_OPT | 0x00001050L)  /* LMC_NETBIOS use lana 5 */
#define LMC_OPT_LANA6       (LMC_OPT | 0x00001060L)  /* LMC_NETBIOS use lana 6 */
#define LMC_OPT_LANA7       (LMC_OPT | 0x00001070L)  /* LMC_NETBIOS use lana 7 */

#define LMC_OPT_LANA_MASK              0x00000FF0L   /* To mask out lana from lmc_options */
#define LMC_OPT_LANA_SHIFT             4             /* Number of bits to shift lana after masking */
#define RDM_MAX_LANA                   255           /* Max value that fits into 7 bits */

/* DEBUG_OPT cannot be or'd together in one d_on/off_opt() call with non-DEBUG_OPTs
*/
#define DEBUG_OPT       0x40000000L
#define PZVERIFY    (DEBUG_OPT | 0x00000001L)  /* verify pz_next against the file length */
#define PAGE_CHECK  (DEBUG_OPT | 0x00000002L)  /* track page id from dio_in until dio_out */
#define LOCK_CHECK  (DEBUG_OPT | 0x00000004L)  /* check file and page for illegal change by another process */
#define CACHE_CHECK (DEBUG_OPT | 0x00000008L)  /* check for cache corruption after each d_ call */
/*
   Note that PAGE_CHECK and LOCK_CHECK are mutually exclusive.
   WARNING! LOCK_CHECK will cause disk I/O on each cache access !!!
*/

/* TRACE_OPT cannot be or'd together in one d_on/off_opt() call with non-TRACE_OPTs
*/
#define TRACE_OPT       0x20000000L
#define TRACE_DBERR  (TRACE_OPT | 0x00000001L)  /* trace errors if any trace flag set */
#define TRACE_API    (TRACE_OPT | 0x00000002L)  /* trace api functions */
#define TRACE_LOCKS  (TRACE_OPT | 0x00000004L)  /* trace file locks */


/* runtime cache shareable options */
#ifdef MULTI_TASK
#define CA_SHAREABLE       1
#define CA_NONSHAREABLE    0
#endif

/* ------------------------------------------------------------------------ */

#ifndef NO_CONST     /* Use the 'const' keyword unless told otherwise */
#define CONST const
#else
#define CONST /**/
#endif

/* Use the 'DB_VOID' keyword for POINTERS ONLY */
#ifndef NO_VOID
#define DB_VOID void
#else
#define DB_VOID char
#endif

#ifdef NO_WIN_H
#undef NO_WIN_H
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>



#ifndef NO_PASCAL  /* Use PASCAL calling conventions unless told otherwise */

#ifndef PASCAL
#define PASCAL _pascal
#endif

#ifndef CDECL
#define CDECL _cdecl
#endif

#else /* NO_PASCAL */

#ifndef PASCAL
#define PASCAL /**/
#endif

#ifndef CDECL
#define CDECL  /**/
#endif

#endif /* NO_PASCAL */

/* If mixed memory model use far and near keywords */
#define DB_FAR       /**/
#define DB_NEAR      /**/
#define DB_FAR_PROC  /**/

#define DB_HUGE      /**/


#define INTERNAL_FIXED     PASCAL
#define EXTERNAL_FIXED     DB_FAR_PROC PASCAL 
#define EXTERNAL_CALLBACK  EXTERNAL_FIXED
 

#define EXTERNAL_DBN      EXTERNAL_FIXED


/* ------------------------------------------------------------------------ */

#if _MSC_VER >= 700
#pragma warning(disable:4103)    /* use of pack() from include file */
#endif


#define LD_INIT()          /**/
#define LOCK_DESC          /**/
#define POINTER_INIT()     { (void DB_FAR *)0 }
#define POINTER_ASSIGN(a)  { a }



#include "packwin.h"

typedef struct
{
   char DB_FAR *ptr;
   LOCK_DESC
} CHAR_P;
typedef struct
{
   DB_ADDR DB_FAR *ptr;
   LOCK_DESC
} DB_ADDR_P;


typedef struct
{
   struct TASK_S DB_FAR *ptr;
   LOCK_DESC
} TASK_P;
typedef struct
{
   DB_VOID DB_FAR *ptr;
   LOCK_DESC
} QTASK_P;

typedef struct DB_TASK_S
{
   TASK_P v;
   QTASK_P q;
}  DB_TASK;


#ifdef MULTI_TASK
#define DB_TASK_INIT() { POINTER_INIT(), POINTER_INIT() }
#endif

#include "nopack.h"

/* Allow for function prototyping */
#ifdef LINT_ARGS
#define P0      void       /* parameterless function */
#define P1(t)   t          /* first (or only) parameter in function */
#define Pi(t)   ,t         /* subsequent (2,3,...) parameter in function */
#else
#define P0      /**/
#define P1(t)   /**/
#define Pi(t)   /**/
#endif


#define DBN_D1     P1(int)
#define DBN_Dn     Pi(int)

#ifdef MULTI_TASK

#define TASK_DBN_D1        P1(DB_TASK DB_FAR *) DBN_Dn

#define DBN_TASK_D1        DBN_D1 Pi(DB_TASK DB_FAR *)

#define TASK_D1            P1(DB_TASK DB_FAR *)
#define TASK_Di            Pi(DB_TASK DB_FAR *)
#define TASK_PTR_D1        P1(DB_TASK DB_FAR *)
#define TASK_PTR_Di        Pi(DB_TASK DB_FAR *)

extern DB_TASK Currtask;

#else /* MULTI_TASK */

#define TASK_DBN_D1        DBN_D1

#define DBN_TASK_D1        DBN_D1

#define TASK_D1            P0
#define TASK_Di            /**/
#define TASK_PTR_D1        P0
#define TASK_PTR_Di        /**/

#endif /* MULTI_TASK */


#include "packlib.h"

typedef struct lock_request
{
   unsigned int item;                  /* record or set number */
   char type;                          /* lock type: 'r', 'w', 'x', 'k' */
}  LOCK_REQUEST;

#include "nopack.h"


#define TRANS_ID_LEN 21


#define  CURR_DB        -1
#define  ALL_DBS        -2
#define  VOID_DB        -3
#define  CURR_DB_PARM   , curr_db
#define  CURR_DB_ONLY   curr_db
#define  DBN_PARM       , dbn
#define  ADBN_PARM      , int dbn

#define  DBN_ONLY       dbn
#define  ADBN_ONLY      int dbn
#define  DBN_DECL       int dbn;



#ifdef __cplusplus
extern "C"
{
#endif

typedef void (EXTERNAL_CALLBACK * ERRORPROC) (P1(int) Pi(char DB_FAR *));
typedef int  (EXTERNAL_CALLBACK * ALIGNMENT_PROC)(
                  P1(DB_SHORT DB_FAR *) Pi(DB_SHORT DB_FAR *)
                  Pi(unsigned char) Pi(DB_SHORT DB_FAR *) Pi(DB_SHORT));

#include "dproto.h"
#ifndef NOLIB
#include "lockcomm.h"
#endif
#undef  NOVELLTRX
#define NOVELLTRX       _NOVELLTRX_
#undef  ALIGNMENT_CHECK
#define ALIGNMENT_CHECK _ALIGNMENT_CHECK_

#ifdef MULTI_TASK
#include "task.h"
#endif    /* MULTI_TASK */

#ifdef __cplusplus
}
#endif

#ifndef MULTI_TASK
extern volatile int db_status;
#endif

#if _MSC_VER >= 700
#pragma warning(default:4103)    /* use of pack() from include file */
#endif

#endif /* VISTA_H */

/* $Revision:   1.123  $ */

/* vpp VISTA.H (Fri Sep 19 11:56:10 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

