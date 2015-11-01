/*----------------------------------------------------------------------------
   dbxtrn.h: db_VISTA external data header file

   (This file is included by dbtype.h)

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef DBXTRN_H
#define DBXTRN_H


typedef struct
{
   struct PAGE_TAG_S DB_FAR *ptr;
   LOCK_DESC
}  PAGE_TAG_P;

typedef struct PAGE_TAG_S
{
   DB_SHORT lru_page;     /* last lru page */
   DB_SHORT num_pages;    /* number of pages currently in cache */
   DB_ULONG lookups;
   DB_ULONG hits;
} PAGE_TAG;

typedef struct PAGE_TABLE_S
{
   PAGE_ENTRY DB_FAR   *pg_table;   /* page table */
   DB_SHORT             pgtab_sz;   /* max pages to manage */
   LOOKUP_ENTRY DB_FAR *lookup;     /* hash lookup table */
   DB_SHORT             lookup_sz;  /* number of buckets in hash */
#ifdef DBSTAT
   MEM_STATS            mem_stats;
   CACHE_STATS          cache_stats;
#endif
} PAGE_TABLE;

/* Define Cache table */
typedef struct CACHE_S
{
   PAGE_TABLE     Db_pgtab;         /* Database page table */
   PAGE_TABLE     Ix_pgtab;         /* Index page table */

   PAGE_TAG_P     Tag_table;
   DB_SHORT       Max_tags;
   DB_SHORT       Num_tags;

   DB_BOOLEAN     Shareable;
   DB_BOOLEAN     Prealloc;
   DB_SHORT       Task_count;       /* number of tasks using this cache */

   DB_SHORT       Largest_page;     /* size of largest page in pg_cache */
   char DB_FAR   *Dbpgbuff;         /* allocated by dio_init used by o_update */
} CACHE;

#ifdef NO_DB_GLOBAL
#define db_global       (*Vistatask.v.ptr)
#define db_cache        (*db_global.Cache.ptr)
#endif

#ifndef INTERNAL_H

#define db_pgtab        db_cache.Db_pgtab
#define ix_pgtab        db_cache.Ix_pgtab

/* Dynamically allocated global variables */
#define dbpgbuff        db_cache.Dbpgbuff
#define dbpg_table      db_pgtab.pg_table
#define db_lookup       db_pgtab.lookup
#define ixpg_table      ix_pgtab.pg_table
#define ix_lookup       ix_pgtab.lookup
#define tag_table       db_cache.Tag_table.ptr

/* Non-dynamically allocated global variables */
#define shareable_cache db_cache.Shareable
#define prealloc_cache  db_cache.Prealloc
#define task_count      db_cache.Task_count
#define db_pgtab_sz     db_pgtab.pgtab_sz
#define db_lookup_sz    db_pgtab.lookup_sz
#define ix_pgtab_sz     ix_pgtab.pgtab_sz
#define ix_lookup_sz    ix_pgtab.lookup_sz
#define max_tags        db_cache.Max_tags
#define num_tags        db_cache.Num_tags
#define largest_page    db_cache.Largest_page

#endif /* INTERNAL_H */


/* Database Dictionary Tables */

typedef int  (INTERNAL_FIXED * LMC_AVAIL_PROC)   (P0);
typedef int  (INTERNAL_FIXED * LMC_OPEN_PROC)    (P0);
typedef int  (INTERNAL_FIXED * LMC_CLOSE_PROC)   (P0);
typedef int  (INTERNAL_FIXED * LMC_CHECKID_PROC) (P1(char DB_FAR *));
typedef int  (INTERNAL_FIXED * LMC_TRANS_PROC)   (P1(MTYPE) Pi(void DB_FAR *) Pi(size_t) Pi(void DB_FAR *) Pi(size_t));
typedef int  (INTERNAL_FIXED * LMC_LOCKING_PROC) (P1(int) Pi(int) Pi(long));
typedef int  (INTERNAL_FIXED * LMC_ERRSTR_PROC)  (P1(char DB_FAR *) Pi(int) Pi(int));
typedef int  (INTERNAL_FIXED * LMC_FILEID_PROC)  (P1(void DB_FAR *) Pi(char DB_FAR *));
typedef void DB_FAR * (INTERNAL_FIXED * LMC_ALLOC_PROC) (P1(size_t));
typedef void (INTERNAL_FIXED * LMC_FREE_PROC)    (P1(void DB_FAR *));

typedef int  (INTERNAL_FIXED * TAF_LOGIN_PROC)   (P0);
typedef int  (INTERNAL_FIXED * TAF_LOGOUT_PROC)  (P0);
typedef int  (INTERNAL_FIXED * TAF_OPEN_PROC)    (P0);
typedef int  (INTERNAL_FIXED * TAF_CLOSE_PROC)   (P0);
typedef int  (INTERNAL_FIXED * TAF_ACCESS_PROC)  (P1(int));
typedef int  (INTERNAL_FIXED * TAF_RELEASE_PROC) (P0);
typedef int  (INTERNAL_FIXED * TAF_ADD_PROC)     (P1(CONST char DB_FAR *));
typedef int  (INTERNAL_FIXED * TAF_DEL_PROC)     (P1(CONST char DB_FAR *));

typedef struct
{
   void DB_FAR *ptr;
   LOCK_DESC
}  LMC_DATA_P;


#define LOCK_STACK_SIZE    10

typedef struct TASK_S
{
   ERR_INFO          Db_error;
   CACHE_P           Cache;
   PAGE_ENTRY DB_FAR *Last_datapage;
   PAGE_ENTRY DB_FAR *Last_keypage;
   PAGE_ENTRY DB_FAR *Last_ixpage;
   short             Db_tag;
   short             Key_tag;
   short             Ix_tag;
   char              Dbuserid[USERIDLEN];
   char              Dbdpath[FILENMLEN * 2];
   char              Dbfpath[FILENMLEN * 2];
   char              Lockmgrn[FILENMLEN];
   char              Lm_addr[FILENMLEN];    /* lockmgr address */
   char              Dbtaf[FILENMLEN];
   char              RdmIniFile[FILENMLEN];
   char              Ctbpath[FILENMLEN];
   char              Trans_id[TRANS_ID_LEN];
   char              Dblog[FILENMLEN];
   CNTRY_TBL_P       Country_tbl;
   int               Ctbl_activ;
   int               Trlog_flag;       /* set only by user implemented fcns */
   int               Dbopen;
   int               Dbshflag;
   short             DbNestLevel;
   short             InRDM;
   short             Trcommit;         /* are we in d_trend()? */
   int               Lfn;
   int               No_of_keys;
   DB_ULONG          Dboptflag;        /* which Dboptions have been explicitly set */
   DB_ULONG          Dboptions;
   int               Ov_header_written;
   int               Ov_setup_done;
   int               Lock_lvl;
   char DB_FAR      *Crloc;   /* location in page buffer of current record */
   int               Lock_stack[LOCK_STACK_SIZE];
   DB_BOOLEAN        Cache_ovfl;    /* must be here, not in db_cache */
   PGZERO_P          Pgzero;
   DB_SHORT          Last_keyfld;
   KEY_INFO_P        Key_info;
   KEY_TYPE          Key_type;
   off_t             Ov_initaddr;
   off_t             Ov_rootaddr;
   off_t             Ov_nextaddr;
   RI_ENTRY_P        Root_ix;
   llist             Ren_list;
   DB_SHORT          Page_size;
   DB_ADDR           Curr_rec;
   DB_ADDR_P         Curr_own;
   DB_ADDR_P         Curr_mem;
   FILE_NO           Ov_file;
   FILE_ENTRY_P      File_table;
   DB_SHORT          Size_ft;
   RECORD_ENTRY_P    Record_table;
   DB_SHORT          Size_rt;
   SET_ENTRY_P       Set_table;
   DB_SHORT          Size_st;
   MEMBER_ENTRY_P    Member_table;
   DB_SHORT          Size_mt;
   SORT_ENTRY_P      Sort_table;
   DB_SHORT          Size_srt;
   FIELD_ENTRY_P     Field_table;
   DB_SHORT          Size_fd;
   KEY_ENTRY_P       Key_table;
   DB_SHORT          Size_kt;
   llist             Sk_list;
   ERRORPROC         error_func;
   DB_SHORT          Old_size_ft;
   DB_SHORT          Old_size_fd;
   DB_SHORT          Old_size_st;
   DB_SHORT          Old_size_mt;
   DB_SHORT          Old_size_srt;
   DB_SHORT          Old_size_kt;
   DB_SHORT          Old_size_rt;
   DB_SHORT          Old_no_of_dbs;
   int               Curr_db;
   int               Set_db;
   DB_SHORT          No_of_dbs;
   DB_ENTRY DB_FAR  *Curr_db_table;
   RN_ENTRY DB_FAR  *Curr_rn_table;
   DB_ENTRY_P        Db_table;
   RN_ENTRY_P        Rn_table;
   ULONG_P           Co_time;
   ULONG_P           Cm_time;
   ULONG_P           Cs_time;
   DB_ULONG          Cr_time;
   DB_BOOLEAN        Db_tsrecs;
   DB_BOOLEAN        Db_tssets;
   int               Dbwait_time;
   int               Db_timeout;
   int               Net_timeout;
   int               Thread_timeout;
   int               Db_lockmgr;
   int               Keyl_cnt;
   size_t            Lp_size;
   size_t            Fp_size;  
   unsigned int      TTSconn;
   INT_P             App_locks;
   INT_P             Excl_locks;
   INT_P             Kept_locks;
   LOCK_DESCR_P      Rec_locks;
   LOCK_DESCR_P      Set_locks;
   LOCK_DESCR_P      Key_locks;
   LM_LOCK DB_FAR   *Lock_pkt;
   LM_FREE DB_FAR   *Free_pkt;
   FILE_NO_P         File_refs;
   DB_BOOLEAN        Session_active;
   int               Max_open_files;   /* maximum number of open files allowed */
   int               Cnt_open_files;   /* count of currently open files */
   int               Last_file;        /* least recently used file */
   int               Rlb_status;
   double            Nap_factor;       /* lmcg sleep time */
   char              Type[2];          /* open type (s or x)--used by dopen() */
#ifndef DB_LIB
#ifndef USE_DB_LIB
   ALIGNMENT_PROC    Calc_alignment;
#endif
#endif

   int               lockMgrComm;
   DB_ULONG          Lmc_optflag;   /* which Lmc_options have been explicitly set */
   DB_ULONG          Lmc_options;   /* lmc specific options defined in vista.h */
   volatile int      Lmc_errno;     /* lmc error code, msg thru lmc_errstr  */
   LMC_DATA_P        Lmc_data;      /* Generic pointer for lmc internal use */
   LMC_AVAIL_PROC    Lmc_avail;
   LMC_CHECKID_PROC  Lmc_checkid;
   LMC_OPEN_PROC     Lmc_open;
   LMC_CLOSE_PROC    Lmc_close;
   LMC_TRANS_PROC    Lmc_trans;
   LMC_LOCKING_PROC  Lmc_locking;
   LMC_ERRSTR_PROC   Lmc_errstr;
   LMC_FILEID_PROC   Lmc_fileid;
   LMC_ALLOC_PROC    Lmc_alloc;
   LMC_FREE_PROC     Lmc_free;
   char DB_FAR      *nwFunction;    /* which novell function encountered error */

   DB_SHORT          Lckmgr_type;   /* type of lock manager: TSR, dedicated, Windows, etc. */
   DB_ULONG          Lckmgr_caps;   /* lock manager capabilities */

   TAF_LOGIN_PROC    Taf_login;
   TAF_LOGOUT_PROC   Taf_logout;
   TAF_OPEN_PROC     Taf_open;
   TAF_CLOSE_PROC    Taf_close;
   TAF_ACCESS_PROC   Taf_access;
   TAF_RELEASE_PROC  Taf_release;
   TAF_ADD_PROC      Taf_add;
   TAF_DEL_PROC      Taf_del;

   long              TcpLMHostSocket;  /* socket number for TCP lock manager */
   int               TcpLMUseSpecifiedSocket; /* nonzero if TcpLMHostSocket is to be used */

#ifdef MULTI_TASK
   volatile int      Db_status;
   unsigned int      Process_id;       /* process id */
#endif

#ifdef DBSTAT
   GEN_STATS         Gen_stats;
   FILE_STATS_P      File_stats;
   MSG_STATS         Msg_stats[L_LAST+1];
#endif

   DB_ULONG          Db_trace;
   DB_SHORT          Db_indent;
   DB_ULONG          Db_debug;

   HANDLE            hSyncMutex;
   HANDLE            hShrMemMutex;
   HANDLE            hShrMem;
   long              ShrMemBase;
} TASK;

#define RDM_MUTEX_NAME "RDM_SYNC_MUTEX"

#ifndef INTERNAL_H


#define LockMgrComm     db_global.lockMgrComm
#define lmc_optflag     db_global.Lmc_optflag
#define lmc_options     db_global.Lmc_options
#define lmc_errno       db_global.Lmc_errno
#define lmc_data        db_global.Lmc_data.ptr
#define nap_factor      db_global.Nap_factor

#define lmc_avail       (*db_global.Lmc_avail)
#define lmc_checkid     (*db_global.Lmc_checkid)
#define lmc_open        (*db_global.Lmc_open)
#define lmc_close       (*db_global.Lmc_close)
#define lmc_trans       (*db_global.Lmc_trans)
#define lmc_locking     (*db_global.Lmc_locking)
#define lmc_errstr      (*db_global.Lmc_errstr)
#define lmc_fileid      (*db_global.Lmc_fileid)
#define lmc_alloc       (*db_global.Lmc_alloc)
#define lmc_free        (*db_global.Lmc_free)

#define lckmgr_type     db_global.Lckmgr_type
#define lckmgr_caps     db_global.Lckmgr_caps

#define taf_login       (*db_global.Taf_login)
#define taf_logout      (*db_global.Taf_logout)
#define taf_open        (*db_global.Taf_open)
#define taf_close       (*db_global.Taf_close)
#define taf_access      (*db_global.Taf_access)
#define taf_release     (*db_global.Taf_release)
#define taf_add         (*db_global.Taf_add)
#define taf_del         (*db_global.Taf_del)

#define tcpLMHostSocket         db_global.TcpLMHostSocket
#define tcpLMUseSpecifiedSocket db_global.TcpLMUseSpecifiedSocket

#define country_tbl     db_global.Country_tbl.ptr
#define ctbl_activ      db_global.Ctbl_activ

#define app_locks       db_global.App_locks.ptr
#define excl_locks      db_global.Excl_locks.ptr
#define kept_locks      db_global.Kept_locks.ptr
#define rec_locks       db_global.Rec_locks.ptr
#define set_locks       db_global.Set_locks.ptr
#define key_locks       db_global.Key_locks.ptr
#define lock_pkt        db_global.Lock_pkt
#define free_pkt        db_global.Free_pkt
#define file_refs       db_global.File_refs.ptr
#define key_info        db_global.Key_info.ptr
#define curr_own        db_global.Curr_own.ptr
#define curr_mem        db_global.Curr_mem.ptr
#define rn_table        db_global.Rn_table.ptr
#define db_table        db_global.Db_table.ptr
#define file_table      db_global.File_table.ptr
#define record_table    db_global.Record_table.ptr
#define set_table       db_global.Set_table.ptr
#define member_table    db_global.Member_table.ptr
#define sort_table      db_global.Sort_table.ptr
#define field_table     db_global.Field_table.ptr
#define key_table       db_global.Key_table.ptr
#define pgzero          db_global.Pgzero.ptr
#define root_ix         db_global.Root_ix.ptr

#define co_time         db_global.Co_time.ptr
#define cm_time         db_global.Cm_time.ptr
#define cs_time         db_global.Cs_time.ptr


#define trcommit        db_global.Trcommit
#define dbwait_time     db_global.Dbwait_time
#define db_timeout      db_global.Db_timeout
#define net_timeout     db_global.Net_timeout
#define thread_timeout  db_global.Thread_timeout
#define dbopen          db_global.Dbopen
#define dbshflag        db_global.Dbshflag
#define db_lockmgr      db_global.Db_lockmgr
#define dbuserid        db_global.Dbuserid
#define lockmgrn        db_global.Lockmgrn
#define lm_addr         db_global.Lm_addr
#define dbdpath         db_global.Dbdpath
#define dbfpath         db_global.Dbfpath
#define trans_id        db_global.Trans_id
#define dblog           db_global.Dblog
#define keyl_cnt        db_global.Keyl_cnt
#define lp_size         db_global.Lp_size
#define fp_size         db_global.Fp_size
#define session_active  db_global.Session_active
#define cache_ovfl      db_global.Cache_ovfl
#define no_of_keys      db_global.No_of_keys
#define last_keyfld     db_global.Last_keyfld
#define key_type        db_global.Key_type
#define net_status      db_global.Net_status
#define dboptflag       db_global.Dboptflag
#define dboptions       db_global.Dboptions
#define ov_initaddr     db_global.Ov_initaddr
#define ov_rootaddr     db_global.Ov_rootaddr
#define ov_nextaddr     db_global.Ov_nextaddr
#define page_size       db_global.Page_size
#define curr_db         db_global.Curr_db
#define curr_db_table   db_global.Curr_db_table
#define curr_rec        db_global.Curr_rec
#define ov_file         db_global.Ov_file
#define ov_header_written  db_global.Ov_header_written
#define ov_setup_done   db_global.Ov_setup_done
#define size_ft         db_global.Size_ft
#define size_rt         db_global.Size_rt
#define size_st         db_global.Size_st
#define size_mt         db_global.Size_mt
#define size_srt        db_global.Size_srt
#define size_fd         db_global.Size_fd
#define size_kt         db_global.Size_kt
#define lock_lvl        db_global.Lock_lvl
#define crloc           db_global.Crloc
#define lock_stack      db_global.Lock_stack
#define ren_list        db_global.Ren_list
#define sk_list         db_global.Sk_list
#define max_open_files  db_global.Max_open_files
#define cnt_open_files  db_global.Cnt_open_files
#define last_file       db_global.Last_file
#define hUserLockMan    db_global.HUserLockMan
#define idLastMessage   db_global.IdLastMessage
#define dberror_func    db_global.error_func
#ifndef DB_LIB
#ifndef USE_DB_LIB
#define calc_alignment  db_global.Calc_alignment
#endif
#endif

#define cr_time         db_global.Cr_time
#define db_tsrecs       db_global.Db_tsrecs
#define db_tssets       db_global.Db_tssets

#define set_db          db_global.Set_db
#define no_of_dbs       db_global.No_of_dbs
#define curr_rn_table   db_global.Curr_rn_table
#define old_size_ft     db_global.Old_size_ft
#define old_size_fd     db_global.Old_size_fd
#define old_size_st     db_global.Old_size_st
#define old_size_mt     db_global.Old_size_mt
#define old_size_srt    db_global.Old_size_srt
#define old_size_kt     db_global.Old_size_kt
#define old_size_rt     db_global.Old_size_rt
#define old_no_of_dbs   db_global.Old_no_of_dbs

#define trlog_flag      db_global.Trlog_flag

#define lfn             db_global.Lfn
#define dbtaf           db_global.Dbtaf
#define ctbpath         db_global.Ctbpath
#define rdmIniFile      db_global.RdmIniFile
#define dbNestLevel     db_global.DbNestLevel   
#define inRDM           db_global.InRDM
#define ttsconn         db_global.TTSconn


#define last_datapage   db_global.Last_datapage
#define last_keypage    db_global.Last_keypage
#define last_ixpage     db_global.Last_ixpage

#define db_tag          db_global.Db_tag
#define key_tag         db_global.Key_tag
#define ix_tag          db_global.Ix_tag

#define rlb_status      db_global.Rlb_status

#ifdef MULTI_TASK
#define process_id      db_global.Process_id
#endif

#ifdef DBSTAT
#define gen_stats       db_global.Gen_stats
#define file_stats      db_global.File_stats.ptr
#define lmcmsg_stats    db_global.Msg_stats
#endif


#ifndef NO_DB_GLOBAL
extern TASK  db_global;
extern CACHE db_cache;
#endif

#ifdef MULTI_TASK
#define db_status       db_global.Db_status
#endif
#define db_error        db_global.Db_error
#define NWFunction      db_global.nwFunction

#define db_trace        db_global.Db_trace
#define dbIndent        db_global.Db_indent
#define db_debug        db_global.Db_debug

#ifdef MULTI_TASK
extern DB_TASK Orgtask;
extern DB_TASK Vistatask;
#endif         /* MULTI_TASK */


extern int IsShareLoaded;  /* defined in libmain.c */


#endif      /* INTERNAL_H */
#endif      /* DBXTRN_H */

/* $Revision:   1.96  $ */

/* vpp DBXTRN.H (Fri Sep 19 11:56:10 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

