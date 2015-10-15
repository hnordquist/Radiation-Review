/*------------------------------------------------------------------------
   lmc.h -- Raima Data Manager LMC header file
            used by lmc_ modules and lockmgr.

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
--------------------------------------------------------------------------*/

#ifndef LMC_H
#define LMC_H

#ifdef MESSAGE_TABLES

#include "packnet.h"



/* ========================================================================
 * communication headers for use with lockmgr
 * ========================================================================
 */

/*
   The HDR fields are set by the runtime, and echoed back by the lockmgr.
   (note that they must end on a word boundary)
*/
#if MSGVER < 0x331
#define HDR \
   LM_REF      nadd;  /* not used */ \
   LM_REF      uid; \
   LM_REF      pid; \
   MTYPE       mtype;
#else
#define HDR \
   LM_REF      uid; /* user's id in the lockmgr (index into usertab) */ \
   LM_REF      pid; /* the process id of the runtime process */         \
   MTYPE       mtype;                                                   \
   short       pad; /* force message to word boundary */
                    /* the pad is also used by TCP for message length */
#endif

typedef struct MESSAGE_HDR_S
{
  HDR
} MESSAGE_HDR;

typedef struct MESSAGE_S
{
   MESSAGE_HDR hdr;
   char        mtext[1];
} MESSAGE;

typedef struct HM_MESSAGE_S
{
   MESSAGE_HDR hdr;
   LM_MESSAGE  lm_message;
} HM_MESSAGE;

typedef struct HR_MESSAGE_S
{
   MESSAGE_HDR hdr;
   LR_MESSAGE  lr_message;
} HR_MESSAGE;


#define HDR_SIZE  sizeof(MESSAGE_HDR)

typedef struct HM_LOGIN_S
{
   MESSAGE_HDR hdr;
   LM_LOGIN    lm_login;
   char        lm_taffile;
}  HM_LOGIN;

typedef struct HM_OPTIONS_S
{
   MESSAGE_HDR hdr;
   LM_OPTIONS  lm_options;
}  HM_OPTIONS;

typedef struct HM_DBOPEN_S
{
   MESSAGE_HDR hdr;
   LM_DBOPEN   lm_dbopen;
}  HM_DBOPEN;

typedef struct HM_DBCLOSE_S
{
   MESSAGE_HDR hdr;
   LM_DBCLOSE  lm_dbclose;
}  HM_DBCLOSE;

typedef struct HM_LOCK_S
{
   MESSAGE_HDR hdr;
   LM_LOCK     lm_lock;
}  HM_LOCK;

typedef struct HM_RECOVERED_S
{
   MESSAGE_HDR  hdr;
   LM_RECOVERED lm_recovered;
}  HM_RECOVERED;

typedef struct HM_RECFAIL_S
{
   MESSAGE_HDR hdr;
   LM_RECFAIL  lm_recfail;
}  HM_RECFAIL;

typedef struct HM_FREE_S
{
   MESSAGE_HDR hdr;
   LM_FREE     lm_free;
}  HM_FREE;

typedef struct HM_TRCOMMIT_S
{
   MESSAGE_HDR hdr;
   LM_TRCOMMIT lm_trcommit;
}  HM_TRCOMMIT;

typedef struct HM_TREND_S
{
   MESSAGE_HDR hdr;
   LM_TREND    lm_trend;
}  HM_TREND;

typedef struct HM_SETTIME_S
{
   MESSAGE_HDR hdr;
   LM_SETTIME  lm_settime;
}  HM_SETTIME;

typedef struct HM_LOGOUT_S
{
   MESSAGE_HDR hdr;
   LM_LOGOUT   lm_logout;
}  HM_LOGOUT;

typedef struct HM_TERMINATE_S
{
   MESSAGE_HDR  hdr;
   LM_TERMINATE lm_terminate;
}  HM_TERMINATE;

typedef struct HM_VERIFYUSER_S
{
   MESSAGE_HDR   hdr;
   LM_VERIFYUSER lm_verifyuser;
}  HM_VERIFYUSER;

typedef struct HM_CLEARUSER_S
{
   MESSAGE_HDR  hdr;
   LM_CLEARUSER lm_clearuser;
}  HM_CLEARUSER;

typedef struct HM_STATUS_S
{
   MESSAGE_HDR hdr;
   LM_STATUS   lm_status;
}  HM_STATUS;


/* ----------------------------------------
   Message packets, Lock Manager to Runtime
   ---------------------------------------- */

typedef struct HR_LOGIN_S
{
   MESSAGE_HDR hdr;
   LR_LOGIN    lr_login;
}  HR_LOGIN;

typedef struct HR_OPTIONS_S
{
   MESSAGE_HDR hdr;
   LR_OPTIONS  lr_options;
}  HR_OPTIONS;

typedef struct HR_DBOPEN_S
{
   MESSAGE_HDR hdr;
   LR_DBOPEN   lr_dbopen;
}  HR_DBOPEN;

typedef struct HR_LOCK_S
{
   MESSAGE_HDR hdr;
   LR_LOCK     lr_lock;
}  HR_LOCK;

typedef struct HR_TRCOMMIT_S
{
   MESSAGE_HDR hdr;
   LR_TRCOMMIT lr_trcommit;
}  HR_TRCOMMIT;

typedef struct HR_STATUS_S
{
   MESSAGE_HDR hdr;
   LR_STATUS   lr_status;
}  HR_STATUS;

typedef struct HR_TABLES_S
{
   MESSAGE_HDR hdr;
   LR_TABLES   lr_tables;
}  HR_TABLES;

typedef struct HR_USERINFO_S
{
   MESSAGE_HDR hdr;
   LR_USERINFO lr_userinfo;
}  HR_USERINFO;

typedef struct HR_FILEINFO_S
{
   MESSAGE_HDR hdr;
   LR_FILEINFO lr_fileinfo;
}  HR_FILEINFO;

typedef struct HR_USERSTAT_S
{
   MESSAGE_HDR hdr;
   LR_USERSTAT lr_userstat;
}  HR_USERSTAT;

typedef struct HR_TAFTAB_S
{
   MESSAGE_HDR hdr;
   LR_TAFTAB   lr_taftab;
}  HR_TAFTAB;

typedef union 
{
   HM_DBOPEN      hm_dbopen;
   HM_DBCLOSE     hm_dbclose;
   HM_LOCK        hm_lock;
   HM_FREE        hm_free;
   HM_TRCOMMIT    hm_trcommit;
   HM_TREND       hm_trend;
   HM_SETTIME     hm_settime;
   HM_RECOVERED   hm_recovered;
   HM_RECFAIL     hm_recfail;
   HM_LOGIN       hm_login;
   HM_LOGOUT      hm_logout;
   HM_TERMINATE   hm_terminate;
   HM_CLEARUSER   hm_clearuser;
   HM_VERIFYUSER  hm_verifyuser;
   HM_STATUS      hm_status;
   HM_OPTIONS     hm_options;
   HR_LOGIN       hr_login;
   HR_DBOPEN      hr_dbopen;
   HR_LOCK        hr_lock;
} MESSAGE_PKT;

/*
   Message structure used by Lock Manager for internal linked list
   of messages awaiting processing. Not used by Runtime Library.
*/
typedef struct LM_Msg
{
   DB_LONG     lSize;
   DB_LONG     lUsed;
   LM_REF      lmrNode;       /* array index in nodes */
   char        *pData;
   char        *pPos;
   struct LM_Msg  *pNext;
} LM_MSG;

/*
   Packet header used by Lock Manager and Runtime Library. Messages are
   split up into one or more packets (depending on their size), and each
   packet is sent with a packet header. The amount of memory actually
   allocated for the packet depends on the transport - it will be larger
   than the LM_Packet structure itself, and cData will be used as if it
   was a much larger array.
*/
typedef struct LM_Packet
{
   LM_REF      lmrNode;       /* array index in nodes */
   DB_SHORT    sNumber;
   DB_USHORT   usPacketSize;
   DB_SHORT    sCount;
   DB_USHORT   usMessageSize;
   char        cData[2];
} LM_PACKET;

/* Length of packet header added to each packet */
#define LM_PACKETLEN (sizeof(LM_REF)+sizeof(DB_SHORT)+sizeof(DB_USHORT)+sizeof(DB_SHORT)+sizeof(DB_USHORT))

#include "nopack.h"
                                                    
#endif  /* MESSAGE_TABLES */


/* HELP DaveB: the rest of this should be in lm.h */

#ifdef RUNTM_LOCKMGR
/* ========================================================================
 * Structures needed for locking handled by the runtime
 * (i.e. LMC_INTERNAL and LMC_GENERAL)
 * ========================================================================
 */

typedef struct
{
   DB_BYTE DB_FAR *ptr;
   LOCK_DESC
}  GENERAL_P;

typedef struct
{
   struct FILETAB_S DB_FAR *ptr;
   LOCK_DESC
}  FILETAB_P;

typedef struct
{
   struct REQQUEUE_S DB_FAR *ptr;
   LOCK_DESC
}  REQQUEUE_P;

typedef struct
{
   struct USERTAB_S DB_FAR *ptr;
   LOCK_DESC
}  USERTAB_P;

typedef struct G_GLOBAL_S
{
   USERTAB_P                  Usertab;
   struct USERTAB_S DB_FAR *  Userend;
   FILETAB_P                  Filetab;
   struct FILETAB_S DB_FAR *  Fileend;
   REQQUEUE_P                 Req_queue;
   struct REQQUEUE_S DB_FAR * Reqend;
   GENERAL_P                  Userbitmap;
   GENERAL_P                  Filebitmap;
   FILEID_CONTEXT             Fileid;
   DB_ULONG                   Usertabsize;
   DB_ULONG                   Filetabsize;
   DB_ULONG                   Queuesize;
   DB_ULONG                   Userbitsize;
   DB_ULONG                   Filebitsize;
   LM_REF                     Current_free_count;
   LM_REF                     Maxusers;
   LM_REF                     Numusers;
   LM_REF                     Maxfiles;
   LM_REF                     Numfiles;
   LM_REF                     Maxqueue;
   LM_REF                     Numqueue;
   DB_USHORT                  Userbytes;
   DB_USHORT                  Filebytes;
   DB_USHORT                  Maxuserbytes;
   DB_USHORT                  Maxfilebytes;
} G_GLOBAL;

typedef struct
{
   struct G_GLOBAL_S DB_FAR *ptr;
   LOCK_DESC
}  G_GLOBAL_P;

#define current_free_count    Gptr.ptr->Current_free_count

#define usertab         Gptr.ptr->Usertab.ptr
#define userend         Gptr.ptr->Userend
#define filetab         Gptr.ptr->Filetab.ptr
#define fileend         Gptr.ptr->Fileend
#define req_queue       Gptr.ptr->Req_queue.ptr
#define reqend          Gptr.ptr->Reqend
#define fileid          Gptr.ptr->Fileid

#define uptr            Uptr.ptr
#define fptr            Fptr.ptr
#define rptr            Rptr.ptr

#define maxusers        Gptr.ptr->Maxusers
#define numusers        Gptr.ptr->Numusers
#define maxfiles        Gptr.ptr->Maxfiles
#define numfiles        Gptr.ptr->Numfiles
#define maxqueue        Gptr.ptr->Maxqueue
#define numqueue        Gptr.ptr->Numqueue
#define userbytes       Gptr.ptr->Userbytes
#define filebytes       Gptr.ptr->Filebytes
#define maxuserbytes    Gptr.ptr->Maxuserbytes
#define maxfilebytes    Gptr.ptr->Maxfilebytes

#undef  userbitmap
#define userbitmap      Gptr.ptr->Userbitmap.ptr
#undef  filebitmap
#define filebitmap      Gptr.ptr->Filebitmap.ptr

#define userbitsize     Gptr.ptr->Userbitsize
#define filebitsize     Gptr.ptr->Filebitsize
#define usertabsize     Gptr.ptr->Usertabsize
#define filetabsize     Gptr.ptr->Filetabsize
#define queuesize       Gptr.ptr->Queuesize

#define LOCKMGR_TABLES

/*
 * Functions in LMC_FCNS.C
 */

void   bit_set      (P1(DB_BYTE DB_FAR *) Pi(size_t));
int    is_bit_set   (P1(DB_BYTE DB_FAR *) Pi(size_t));
int    is_map_zero  (P1(DB_BYTE DB_FAR *) Pi(size_t));
void   bit_clr      (P1(DB_BYTE DB_FAR *) Pi(size_t));
void   map_zero     (P1(DB_BYTE DB_FAR *) Pi(size_t));
void   zeromem      (P1(DB_BYTE DB_FAR *) Pi(size_t));
size_t count_bits   (P1(DB_BYTE DB_FAR *) Pi(size_t));
size_t first_bit_set(P1(DB_BYTE DB_FAR *) Pi(size_t));
void   bit_or       (P1(DB_BYTE DB_FAR *) Pi(DB_BYTE DB_FAR *) Pi(size_t));

void INTERNAL_FIXED l_dbopen(P1(LM_REF) Pi(struct HM_DBOPEN_S DB_FAR *) Pi(struct HR_DBOPEN_S DB_FAR *) Pi(G_GLOBAL_P));
void INTERNAL_FIXED l_dbclose(P1(LM_REF) Pi(struct HM_DBCLOSE_S DB_FAR *) Pi(G_GLOBAL_P));
void INTERNAL_FIXED l_lock(P1(LM_REF) Pi(struct HM_LOCK_S DB_FAR *) Pi(struct HR_LOCK_S DB_FAR *) Pi(G_GLOBAL_P));
void INTERNAL_FIXED l_free(P1(LM_REF) Pi(struct HM_FREE_S DB_FAR *) Pi(G_GLOBAL_P));
void INTERNAL_FIXED l_trcommit(P1(LM_REF) Pi(struct HM_TRCOMMIT_S DB_FAR *) Pi(struct HR_TRCOMMIT_S DB_FAR *) Pi(G_GLOBAL_P));
void INTERNAL_FIXED l_trend(P1(LM_REF) Pi(G_GLOBAL_P));

LM_REF  INTERNAL_FIXED enter_user(P1(char DB_FAR *) Pi(G_GLOBAL_P));
void    INTERNAL_FIXED lock_all(P1(G_GLOBAL_P));
void    INTERNAL_FIXED lmc_reply(P1(LM_REF) Pi(struct HR_LOCK_S DB_FAR *) Pi(G_GLOBAL_P));
LM_REF  INTERNAL_FIXED free_partial(P1(LM_REF) Pi(G_GLOBAL_P));
int     INTERNAL_FIXED close_user(P1(LM_REF) Pi(G_GLOBAL_P));
int     INTERNAL_FIXED free_pending(P1(LM_REF) Pi(G_GLOBAL_P));
int     INTERNAL_FIXED close_file(P1(LM_REF) Pi(LM_REF) Pi(G_GLOBAL_P));
int     INTERNAL_FIXED freelock(P1(LM_REF) Pi(LM_REF) Pi(G_GLOBAL_P));
void    INTERNAL_FIXED cvt_bin(P1(char DB_FAR *) Pi(DB_BYTE));
int     INTERNAL_FIXED init_gptr (P1(G_GLOBAL_P DB_FAR *));

#endif /* RUNTM_LOCKMGR */



#ifdef LOCKMGR_TABLES

/* ========================================================================
 * Lock manager structures
 * ========================================================================
 */

/*
 * User Table definition.
 *
 * One entry in this table is used for each active application.
 *
*/

typedef struct USERTAB_S
{
   char             u_name[16];/* user name, from HM_LOGIN packet          */
   LM_REF           u_pid;     /* process ID from client programs          */
   LM_REF           u_taf;     /* entry into TAFTAB                        */
   LM_REF           u_pending; /* count of pending lock requests           */
   LM_REF           u_waiting; /* flag indicating user is ready for reply  */
   short            u_timeout; /* total seconds to wait before timeout     */
   short            u_timer;   /* number of seconds left before timeout    */
   short            u_status;  /* user status                              */
   LM_REF           u_recuser; /* what user is being recovered             */
   LM_REF           u_nadd;    /* nadd index in the network layer          */
   LM_FILENAME      u_logfile; /* the logfile being committed              */
   DB_BYTE DB_FAR * u_open;    /* bit map of open files                    */
   DB_BYTE DB_FAR * u_lock;    /* bit map of locked files                  */
   DB_BYTE DB_FAR * u_req;     /* bit map of incomplete lock request locks */
   DB_ULONG         u_options;
#ifdef SMALL_DATA
   DB_BYTE          dummy[6];  /* pad out extra for compatability */
#endif
}  USERTAB;

/*
 * File Table definition.
 *
 * One entry in this table exists for each file that has been opened
 * by any user.
*/

typedef struct FILETAB_S
{
   LM_FILENAME      f_name;    /* index for var_fileid                     */
   LM_REF           f_taf;     /* which taf is controlling this file       */
   short            f_lockstat;/* lock status                              */
   LM_REF           f_queue;   /* pointer to head of lock request queue    */
   DB_BYTE DB_FAR * f_open;    /* bit map of users having this file open   */
   DB_BYTE DB_FAR * f_lock;    /* bit map of users having this file locked */
#ifdef SMALL_DATA
   DB_BYTE          dummy[4];  /* pad out extra for compatability */
#endif
}  FILETAB;

/*
 * File Request Queue definition.
 *
 * One entry represents one lock request for a file.  The file table
 * entry points to the first lock request, then the first lock request
 * points to the next, etc.
*/

typedef struct REQQUEUE_S
{
   short         q_locktype;/* requested lock type                      */
   LM_REF        q_user;    /* user waiting for this lock               */
   LM_REF        q_next;    /* next request for this file               */
}  REQQUEUE;

/*
 * TAF Table definition.
 *
 * One entry represents each one taf file being managed by the lock
 * manager.  All users sharing the same data and key files should be
 * using the same TAF file.
*/

typedef struct TAFTAB_S
{
   LM_FILENAME   t_name;    /* index for var_fileid                     */
   short         t_status;  /* REC_PENDING, REC_NEEDED, REC_OK          */
   LM_REF        t_recuser; /* user entry for which recovery            */
   LM_REF        t_nusers;  /* number of users using this TAF file.     */
} TAFTAB;

#endif  /* LOCKMGR_TABLES */


#ifdef REMCON_TABLES

/* Tables used by the remote console when talking to the lock manager */

#define MAXFILENMLEN 14

typedef struct GEN_USER_TAB_S
{
   LM_REF        g_user_no;
   char          g_name[16];
   short         g_timer;
   LM_REF        g_pending;
   short         g_status;
   LM_REF        g_recover;
   char          g_logfile[MAXFILENMLEN];
} GEN_USER_TAB;

typedef struct GEN_FILE_TAB_S
{
   LM_REF        g_file_no;
   char          g_name[MAXFILENMLEN];
   short         g_lockstat;
   LM_REF        g_numlocks;
   LM_REF        g_user_with_lock;
   LM_REF        g_queue_entry;
} GEN_FILE_TAB;

typedef struct GEN_TAF_TAB_S
{
   LM_REF        g_taf_no;
   char          g_name[FILENMLEN];
   short         g_status;
   LM_REF        g_recuser;
   LM_REF        g_nusers;
} GEN_TAF_TAB;

typedef struct GEN_TAB_S
{
   DB_ULONG      g_msgs_rcv;
   DB_ULONG      g_msgs_snt;
   DB_ULONG      g_lks_granted;
   DB_ULONG      g_lks_rejected;
   DB_ULONG      g_lks_timedout;
   DB_ULONG      g_lks_freed;
} GEN_TAB;

#endif  /* REMCON_TABLES */


#define DEFAULT_TIMEOUT 10

#define EMPTY              ((LM_REF) -1)

#define streq(a, b) (vstrcmp(a, b) == 0)

#ifdef ZOR
/* Zortech's locking() call can get into an infinite loop */
int _pascal Locking(int, int, long);
#define locking Locking
#endif


#define get_gl_ptr(off)          ((unsigned char *)off + db_global.ShrMemBase)
#define get_gl_off(ptr)          ((unsigned char *)ptr - db_global.ShrMemBase)


#endif   /* LMC_H */

/* $Revision:   1.42  $ */

/* vpp LMC.H (Fri Sep 19 11:56:09 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

