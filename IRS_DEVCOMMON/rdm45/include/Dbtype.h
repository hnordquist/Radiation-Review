/*----------------------------------------------------------------------------
   dbtype.h: Standard Raima Data Manager header file containing:

      - DDL dictionary type definitions
      - Machine dependent constant definitions
      - Miscellaneous constants used by runtime functions
      - Virtual memory page table type definitions

   (An #include "vista.h" must precede this include)

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef DBTYPE_H
#define DBTYPE_H

#include <time.h>

/* If INTERNAL_H is defined, then this file is being included only for the
   struct definitions needed to use d_internals().  Therefore, much of the
   stuff in here is not needed and may even cause conflicts with other
   software packages.
   If this file is needed for all the other stuff as well as the struct
   definitions and "internal.h" is also needed, then this file must be
   #include-d BEFORE "internal.h" is #include-d.
*/

/* ------------------------------------------------------------------------ */


#if _MSC_VER >= 700
#pragma warning(disable:4100)    /* unreferenced formal parameter */
#pragma warning(disable:4103)    /* use of pack() from include file */
#else
#ifdef TURBO
#pragma warn -par                /* unreferenced formal parameter */
#endif
#endif


/* ---------------------------------------------------------------------------
   Configure the runtime
*/

#define DBSTAT

#ifndef DB_DEBUG
#define INLINE_STATS
#endif



#define DB_SHARE
#define SYNC_BY_DEFAULT   

#ifdef DB_DEBUG
#define DBERR_EX     /* display file and line number for dberr */
#define DB_TRACE     /* enable runtime tracing */
#endif

/* ------------------------------------------------------------------------ */

typedef unsigned char   DB_BYTE;
typedef unsigned short  DB_WORD;
typedef unsigned long   DB_DWORD;



#include <limits.h>
#define MAX_ALLOC   ((unsigned long)USHRT_MAX)  /* max allocation size */
#define MAX_TABLE   ((unsigned long)USHRT_MAX)  /* max table entries */


#ifndef OMAN         /* Object MANager does not like these */
#ifndef INTERNAL_H

#define PGHOLD    1
#define NOPGHOLD  0
#define PGFREE    1
#define NOPGFREE  0

#define KEYFIND   0
#define KEYNEXT   1
#define KEYPREV   2
#define KEYFRST   3
#define KEYLAST   4

#endif /* INTERNAL_H */
#endif /* OMAN */

/* dictionary attribute flags */
#define SORTFLD         0x0001   /* field is a set sort field */
#define STRUCTFLD       0x0002   /* field is sub-field of struct */
#define UNSIGNEDFLD     0x0004   /* field is unsigned */
#define COMKEYED        0x0010   /* (record contains) or (field included in) compound key */

#define TIMESTAMPED     0x0001   /* record/set is timestamped */
#define STATIC          0x0002   /* file/record is static */

#define TEMPORARY       0x0020   /* file is temporary (no lockmgr & or logging) */
#define NEEDS_COMMIT    0x0001   /* this file has been written to during trx */
#define TS_IS_CURRENT   0x0004   /* the timestamp for the file has already been updated */
#define NOT_TTS_FILE    0x0008   /* this file is NEVER under TTS control */

#define READ_LOCK       0x0100   /* file has been read locked by TTS */
#define WRITE_LOCK      0x0200   /* file has been write locked by TTS */
#define TTSCHECKED      0x0400   /* file has been checked for TTS */
#define NO_LOCK         0xFCFF   /* file has not been locked for TTS */

#ifndef INTERNAL_H

/* Number of bytes in a disk block */
#define D_BLKSZ      512

#ifndef TRUE
#define FALSE  0
#define TRUE   (! FALSE)
#endif

#define YES    1
#define NO     0

#ifndef OMAN

#define DBUSER_ALIVE_BYTE  1
#define DBUSER_COMMIT_BYTE 2

#define BITS_PER_BYTE      8

/* Number of bits in a "type" */
#define BITS( type ) (BITS_PER_BYTE * sizeof( type ))

/* Number of elements in a vector */
#define arraysize(v) (sizeof(v)/sizeof(*(v)))

#ifndef MAX
#define MIN(a, b)    ((a) < (b) ? (a) : (b))
#define MAX(a, b)    ((a) > (b) ? (a) : (b))
#endif

#define DBD_COMPAT_LEN 6
#define INT_SIZE     sizeof(int)
#define SHORT_SIZE   sizeof(short)
#define LONG_SIZE    sizeof(long)
#define CHAR_SIZE    sizeof(char)
#define DB_ADDR_SIZE sizeof(long)
#define PGHDRSIZE    4
#define CTBNAME      "vista.ctb"

#define FLOAT_SIZE   sizeof(float)
#define DOUBLE_SIZE  sizeof(double)

#endif /* OMAN */
#endif /* INTERNAL_H */

#define RECHDRSIZE   (sizeof(DB_SHORT) + DB_ADDR_SIZE)
#define NONE         -1L
#define FILEMASK     0xFFL
#define ADDRMASK     MAXRECORDS
#define FILESHIFT    24
#define OPTKEYNDX    0x003F
#define OPTKEY_LIMIT 63

#if DB_SHORT_LEN == 4
#define OPTKEYSHIFT  26
#define OPTKEYMASK   0xFC000000
#define RLBMASK      0x40000000
#endif
#if DB_SHORT_LEN == 2
#define OPTKEYSHIFT  10
#define OPTKEYMASK   0xFC00
#define RLBMASK      0x4000
#endif

#undef TRUENAME
#define TRUENAME      0x8000
#define TRUENAME_MASK 0x7FFF

/* creation timestamp in rec hdr */
#define RECCRTIME    (sizeof(DB_SHORT) + DB_ADDR_SIZE)

/* update timestamp in rec hdr */
#define RECUPTIME    (RECCRTIME + sizeof(long))

#ifndef OMAN

/* set pointer structure definition */
typedef struct
{
   long total;                /* total number of members in set */
   DB_ADDR first;             /* database address of first member in set */
   DB_ADDR last;              /* database address of last member in set */
   DB_ULONG timestamp;        /* set update timestamp - if used */
}  SET_PTR;

/* member pointer structure definition */
typedef struct
{
   DB_ADDR owner;             /* database address of owner record */
   DB_ADDR prev;              /* database address of previous member in set */
   DB_ADDR next;              /* database address of next member in set */
}  MEM_PTR;

/* max size of set pointer */
/* member count + prior + next + timestamp (opt) */
#define SETPSIZE  (sizeof(SET_PTR))
#define SP_MEMBERS 0             /* Offset to total members in set ptr */
#define SP_FIRST   sizeof(long)  /* Offset to first member ptr in set ptr */
#define SP_LAST    (SP_FIRST+DB_ADDR_SIZE) /* Offset to last member ptr in set ptr */

#define SP_UTIME   (SP_LAST+DB_ADDR_SIZE)  /* Offset to timestamp in set ptr */

#define MEMPSIZE   (sizeof(MEM_PTR))       /* Size of member pointer = 3*DB_ADDR_SIZE */
#define MP_OWNER   0                       /* Offset to owner ptr in member ptr */
#define MP_PREV    (MP_OWNER+DB_ADDR_SIZE) /* Offset to previous member ptr in member ptr */
#define MP_NEXT    (MP_PREV+DB_ADDR_SIZE)  /* Offset to next member ptr in member ptr */

#endif /* OMAN */

/* maximum length of a dbuserid (including null) */
#define USERIDLEN       16

/* maximum length of a database file name (including path + extension) */
#define FILENMLEN       256

/* maximum length of a database name (base file w/o path w/o extension) */
#define DBNMLEN         32
/*
   DB_PATHLEN = maxFullPath    - maxFile     + null
              = ((FILENMLEN-1) - (DBNMLEN-1) + 1)
*/
#define DB_PATHLEN      (FILENMLEN - DBNMLEN - 1)

/* maximum length of a database file name passed to lock manager */
#define TRUENMLEN       256

/* maximum length allowed for a share name (used in generating UNC path) */
#define SHARENMLEN      16

/* maximum length allowed for a computer name (used in generating UNC path) */
#define COMPUTERNMLEN   16

/* log file name length in lockmgr messages - same as RDM 4.0 */
#define LOGFILELEN      48

/* lock manager name length in TAF file - same as RDM 4.0 */
#define LOCKMGRLEN      48

#define NAMELEN         32       /* max record/set/field name + null */
#define MAXKEYSIZE      256      /* if changed, qdefns.h needs updating */

#ifndef INTERNAL_H
#ifndef OMAN

#define DEFIXPAGES      5        /* default number of index cache pages */
#define DEFDBPAGES      17       /* default number of database cache pages */
#define MINIXPAGES      3        /* minimum number of index cache pages */
#define MINDBPAGES      5        /* minimum number of database cache pages */
#define LENVDBID        48

#define DEFOPENFILES    8
#define DB_MAXHANDLES   19

#endif /* OMAN */
#endif /* INTERNAL_H */

#define MAXDIMS         3

#define OPEN            'o'
#define CLOSED          'c'
#define DATA            'd'
#define KEY             'k'
#define DB_OVERFLOW     'o'

#ifndef OMAN

/* virtual page table entry */
typedef struct page_entry
{
#ifdef MULTI_TASK
   DB_TASK        task;
#endif
   FILE_NO        file;             /* file table entry number (0..size_ft-1) */
   F_ADDR         pageno;           /* database page number */
   F_ADDR         ovfl_addr;        /* overflow file address of page */
   struct page_entry DB_FAR *prev;  /* prev hash page in bucket chain */
   struct page_entry DB_FAR *next;  /* next hash page bucket chain */
   struct page_entry DB_FAR *p_pg;  /* prev page for file */
   struct page_entry DB_FAR *n_pg;  /* next page for file */
   DB_SHORT       tag;              /* page tag */
   DB_SHORT       recently_used;
   DB_BOOLEAN     modified;         /* TRUE if page has been modified */
   DB_SHORT       holdcnt;          /* "hold-in-cache" counter */
   DB_SHORT       buff_size;        /* size of page buffer */
   char DB_FAR   *buff;             /* page buffer pointer */
} PAGE_ENTRY;

typedef struct page_entry DB_FAR *LOOKUP_ENTRY;

#endif /* OMAN */

#define   V3_FILENMLEN  48

typedef struct V3_FILE_ENTRY_S
{
   char     v3_ft_name[V3_FILENMLEN];     /* name of file */
   DB_SHORT v3_ft_desc;                   /* file descriptor */
   char     v3_ft_status;                 /* 'o'=opened, 'c'=closed */
   char     v3_ft_type;                   /* 'd'=data, 'k'=key, 'o'=overflow */
   DB_SHORT v3_ft_slots;                  /* record slots per page */
   DB_SHORT v3_ft_slsize;                 /* size of record slots in bytes */
   DB_SHORT v3_ft_pgsize;                 /* size of page */
   DB_SHORT v3_ft_flags;                  /* e.g. STATIC */
} V3_FILE_ENTRY;

typedef struct FILE_ENTRY_S
{
   char     ft_name[FILENMLEN];        /* name of file */
   int      ft_desc;                   /* file descriptor */
   char     ft_status;                 /* 'o'=opened, 'c'=closed */
   char     ft_type;                   /* 'd'=data, 'k'=key, 'o'=overflow */
   DB_SHORT ft_slots;                  /* record slots per page */
   DB_SHORT ft_slsize;                 /* size of record slots in bytes */
   DB_SHORT ft_pgsize;                 /* size of page */
   DB_SHORT ft_flags;                  /* e.g. STATIC */
   DB_SHORT ft_recently_used;
} FILE_ENTRY;


typedef struct RECORD_ENTRY_S
{
   DB_SHORT rt_file;    /* file table entry of file containing record */
   DB_SHORT rt_len;     /* total length of record */
   DB_SHORT rt_data;    /* offset to start of data in record */
   DB_SHORT rt_fields;  /* first field def in field_table */
   DB_SHORT rt_fdtot;   /* total number of fields in record */
   DB_SHORT rt_flags;
} RECORD_ENTRY;

#ifndef OMAN

#define FIRST      'f'
#define LAST       'l'
#define NEXT       'n'

#endif /* OMAN */

#define ASCENDING  'a'
#define DESCENDING 'd'
#define NOORDER    'n'

typedef struct SET_ENTRY_S
{
   DB_SHORT st_order;      /* 'f'=first, 'l'=last, 'a'=ascending,
                              'd'=descending, 'n'=no order */
   DB_SHORT st_own_rt;     /* record table entry of owner */
   DB_SHORT st_own_ptr;    /* offset to set pointers in record */
   DB_SHORT st_members;    /* index of first member record in member table */
   DB_SHORT st_memtot;     /* total number of members of set */
   DB_SHORT st_flags;      /* 0x0001 is set if record is timestamped */
} SET_ENTRY;


typedef struct MEMBER_ENTRY_S
{
   DB_SHORT mt_record;           /* record table entry for this member */
   DB_SHORT mt_mem_ptr;          /* offset to member ptrs in record */
   DB_SHORT mt_sort_fld;         /* sort table entry of first sort field */
   DB_SHORT mt_totsf;            /* total number of sort fields */
} MEMBER_ENTRY;


typedef struct SORT_ENTRY_S
{
   DB_SHORT se_fld;              /* field table entry of sort field */
   DB_SHORT se_set;              /* set table entry of sorted set */
}  SORT_ENTRY;

/* kinds of keys */
#define NOKEY      'n'
#define DUPLICATES 'd'
#define UNIQUE     'u'

/* kinds of fields */
#define FLOAT      'f'
#define DOUBLE     'F'
#define CHARACTER  'c'
#define SHORTINT   's'
#define REGINT     'i'
#define LONGINT    'l'
#define DBADDR     'd'
#define GROUPED    'g'
#define COMKEY     'k'

typedef struct FIELD_ENTRY_S
{
   char fd_key;               /* see the #defines just above */
   unsigned char fd_type;     /* see the #defines just above */
   DB_SHORT fd_len;           /* length of field in bytes */
   DB_SHORT fd_dim[MAXDIMS];  /* size of each array dimension */
   DB_SHORT fd_keyfile;       /* file_table entry for key file */
   DB_SHORT fd_keyno;         /* key prefix number */
   DB_SHORT fd_ptr;           /* offset to field in record or 1st compound key field in key_table */
   DB_SHORT fd_rec;           /* record table entry of record containing field */
   DB_SHORT fd_flags;         /* see the #defines at the top of the file */
} FIELD_ENTRY;

/* compound key table entry declaration */
typedef struct KEY_ENTRY_S
{
   DB_SHORT kt_key;              /* compound key field number */
   DB_SHORT kt_field;            /* field number of included field */
   DB_SHORT kt_ptr;              /* offset to start of field data in key */
   DB_SHORT kt_sort;             /* 'a' = ascending, 'd' = descending */
} KEY_ENTRY;


/* database table entry declaration */
typedef struct DB_ENTRY_S
{
   /* NOTE: db_name, db_path and ft_offset defined in TASK if ONE_DB */
   DB_SHORT Size_ft;             /* size of this db's file_table */
   DB_SHORT ft_offset;           /* offset to this db's file_table entries */
   DB_SHORT Size_rt;             /* size of this db's record_table */
   DB_SHORT rt_offset;           /* offset to this db's record_table entries */
   DB_SHORT Size_fd;             /* size of this db's field_table */
   DB_SHORT fd_offset;           /* offset to this db's field_table entries */
   DB_SHORT Size_st;             /* size of this db's set_table */
   DB_SHORT st_offset;           /* offset to this db's set_table entries */
   DB_SHORT Size_mt;             /* size of this db's member_table */
   DB_SHORT mt_offset;           /* offset to this db's member_table entries */
   DB_SHORT Size_srt;            /* size of this db's sort_table */
   DB_SHORT srt_offset;          /* offset to this db's sort_table entries */
   DB_SHORT Size_kt;             /* size of this db's key_table */
   DB_SHORT kt_offset;           /* offset to this db's key table entries */
   DB_SHORT key_offset;          /* key prefix offset for this db */
   DB_ADDR  sysdba;              /* database address of system record */
   DB_ADDR  curr_dbt_rec;        /* this db's current record */
   DB_ULONG curr_dbt_ts;         /* this db's current record timestamp */
   DB_SHORT Page_size;           /* size of this db's page */
   char     db_path[DB_PATHLEN]; /* name of path to this database */
   char     db_name[DBNMLEN];    /* name of this database */
} DB_ENTRY;

/* Structure containing current record type & address for
   recfrst/set/next.. NOTE: rn_type and rd_dba defined in TASK when ONE_DB.
*/
typedef struct RN_ENTRY_S
{
   DB_SHORT rn_type;    /* Last record type supplied to recfrst/recset */
   DB_ADDR  rn_dba;     /* Last db addr computed by recfrst/recset/recnext */
} RN_ENTRY;

#define  DB_REF(item)            (db_global.Curr_db_table->item)
#define  RN_REF(item)            (curr_rn_table->item)
#define  NUM2INT(num, offset)    ((num) + db_global.Curr_db_table->offset)
#define  NUM2EXT(num, offset)    ((num) - db_global.Curr_db_table->offset)
#define  ORIGIN(offset)          (curr_db_table->offset)


#ifndef OMAN

#ifndef INTERNAL_H

#define  TABLE_SIZE(size)        DB_REF(size)

/* DBN_PARM... and TASK_PARM... are used for function interfaces */
#ifdef MULTI_TASK
#define  TASK_PARM               , task
#define  ATASK_PARM              , DB_TASK DB_FAR *task
#define  TASK_PARM_DBN           task ,
#define  TASK_ONLY               task
#define  ATASK_ONLY              DB_TASK DB_FAR *task
#define  TASK_DECL               DB_TASK DB_FAR *task;
#define  TASK_PTR_DECL           DB_TASK DB_FAR *task;
#define  TASK_DBN_ONLY           task DBN_PARM
#define  ATASK_DBN_ONLY          DB_TASK DB_FAR *task ADBN_PARM
#define  DBN_TASK_ONLY           DBN_ONLY , task
#else
#define  TASK_PARM               /**/
#define  ATASK_PARM              /**/
#define  TASK_PARM_DBN           /**/
#define  TASK_ONLY               /**/
#define  ATASK_ONLY              /**/
#define  TASK_DECL               /**/
#define  TASK_PTR_DECL           /**/
#define  TASK_DBN_ONLY           DBN_ONLY
#define  ATASK_DBN_ONLY          ADBN_ONLY
#define  DBN_TASK_ONLY           DBN_ONLY
#endif /* MULTI_TASK */

#endif /* INTERNAL_H */


/* Country code table definition */
typedef struct CNTRY_TBL_S
{
   unsigned char out_chr;
   unsigned char sort_as1;
   unsigned char sort_as2;
   unsigned char sub_sort;
} CNTRY_TBL;


/* names lists structure declaration */
typedef struct OBJNAMES_S
{
   char     **recs;
   char     **flds;
   char     **sets;
} OBJNAMES;


#ifndef INTERNAL_H

/* directory separator character (separating dir name from file name) */
#define DIRCHAR '\\'


#define DB_TELL   tell
#define DB_OPEN   open_b

#ifdef DB_DLL

#define SELECTOROF(lp)                       HIWORD(lp)
#define OFFSETOF(lp)                         LOWORD(lp)


#define DB_CLOSE  _close
#define DB_WRITE  _write
#define DB_READ   _read
#define DB_LSEEK  _lseek


#else /* DB_DLL */


#define DB_CLOSE  _close
#define DB_WRITE  _write
#define DB_READ   _read
#define DB_LSEEK  _lseek


#endif /* DB_DLL */
#endif /* INTERNAL_H */

#include "packwin.h"

typedef struct
{
   FILE_NO DB_FAR *ptr;
   LOCK_DESC
} FILE_NO_P;

typedef struct
{
   int DB_FAR *ptr;
   LOCK_DESC
}  INT_P;

typedef struct
{
   struct NODE_PATH_S DB_FAR *ptr;
   LOCK_DESC
} NODE_PATH_P;

typedef struct
{
   struct LM_LOCK_S DB_FAR *ptr;
   LOCK_DESC
} LM_LOCK_P;

typedef struct
{
   struct LM_FREE_S DB_FAR *ptr;
   LOCK_DESC
} LM_FREE_P;

typedef struct
{
   struct LM_DBOPEN_S DB_FAR *ptr;
   LOCK_DESC
} LM_DBOPEN_P;

typedef struct
{
   struct LM_DBCLOSE_S DB_FAR *ptr;
   LOCK_DESC
} LM_DBCLOSE_P;

typedef struct
{
   struct LR_DBOPEN_S DB_FAR *ptr;
   LOCK_DESC
} LR_DBOPEN_P;

typedef struct
{
   struct KEY_INFO_S DB_FAR *ptr;
   LOCK_DESC
} KEY_INFO_P;

typedef struct
{
   struct ren_entry DB_FAR *ptr;
   LOCK_DESC
} REN_ENTRY_P;

typedef struct
{
   struct page_entry DB_FAR *ptr;
   LOCK_DESC
} PAGE_ENTRY_P;

typedef struct
{
   struct V3_FILE_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} V3_FILE_ENTRY_P;

typedef struct
{
   struct FILE_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} FILE_ENTRY_P;

#ifdef DBSTAT
typedef struct
{
   struct FILE_STATS_S DB_FAR *ptr;
   LOCK_DESC
} FILE_STATS_P;
#endif

typedef struct
{
   struct DB_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} DB_ENTRY_P;

typedef struct
{
   struct RN_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} RN_ENTRY_P;

#ifdef MULTI_TASK
typedef struct
{
   TASK_P DB_FAR *ptr;
   LOCK_DESC
}  TASK_TABLE_P;
#endif


typedef struct
{
   struct RECORD_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} RECORD_ENTRY_P;

typedef struct
{
   struct SET_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} SET_ENTRY_P;

typedef struct
{
   struct MEMBER_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} MEMBER_ENTRY_P;

typedef struct
{
   struct SORT_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
}  SORT_ENTRY_P;

typedef struct
{
   struct FIELD_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} FIELD_ENTRY_P;

typedef struct
{
   struct KEY_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} KEY_ENTRY_P;

typedef struct
{
   struct PGZERO_S DB_FAR *ptr;
   LOCK_DESC
} PGZERO_P;

typedef struct
{
   struct RI_ENTRY_S DB_FAR *ptr;
   LOCK_DESC
} RI_ENTRY_P;

typedef struct
{
   DB_ULONG DB_FAR *ptr;
   LOCK_DESC
}  ULONG_P;

typedef struct
{
   long DB_FAR *ptr;
   LOCK_DESC
} LONG_P;

typedef struct
{
   struct CNTRY_TBL_S DB_FAR *ptr;
   LOCK_DESC
} CNTRY_TBL_P;

struct sk
{
   CHAR_P sk_val;
   DB_SHORT sk_fld;
};

typedef struct
{
   struct sk DB_FAR *ptr;
   LOCK_DESC
} SK_P;

#include "nopack.h"

/* node key search path stack entry: one per level per key field */
typedef struct NODE_PATH_S
{
   F_ADDR node;                        /* node (page) number  */
   DB_SHORT slot;                      /* slot number of key */
} NODE_PATH;

/* index key information: one entry per key field */
typedef struct KEY_INFO_S
{
   DB_SHORT level;                     /* current level # in node path */
   DB_SHORT max_lvls;                  /* maximum possible levels for key */
   DB_SHORT lstat;                     /* last key function status */
   DB_SHORT fldno;                     /* field number of key */
   FILE_NO keyfile;                    /* key file containing this key */
   DB_ADDR dba;                        /* db address of last key */
   CHAR_P Keyval;                      /* ptr to last key value */
   NODE_PATH_P Node_path;              /* stack of node #s in search path */
} KEY_INFO;

/* index file node structure */
typedef struct
{
   time_t   last_chgd;           /* date/time of last change of this node */
   DB_SHORT used_slots;          /* current # of used slots in node */
   char     slots[1];            /* start of slot area */
}  NODE;

#define node_path Node_path.ptr
#define keyval Keyval.ptr

/* key slot structure */
typedef struct key_slot
{
   F_ADDR child;                       /* child node pointer */
   DB_SHORT keyno;                     /* key number */
   char data[1];                       /* start of key data */
} KEY_SLOT;

typedef union key_type
{
   char              kd[MAXKEYSIZE];   /* This makes KEY_TYPE the correct size */
   struct key_slot   ks;
} KEY_TYPE;

/* file rename table entry declarations */
typedef struct ren_entry
{
   CHAR_P      File_name;
   CHAR_P      Ren_db_name;
   FILE_NO     file_no;
} REN_ENTRY;

typedef struct ERR_INFO_S
{
   struct ERR_INFO_S DB_FAR *next;
   int errnum;       /* primary error code */
   int err_ex;       /* secondary error code */
   int errno_c;      /* O/S error code */
   /* not used unless DBERR_EX is defined */
   char DB_FAR *filename;
   int linenum;
} ERR_INFO;

/* buffered binary files */
typedef struct BFILE_S
{
   int fd;
   char DB_FAR *buf;
   size_t bufsize;   /* size of buffer */
   size_t buflen;    /* amount of data in buffer */
   char DB_FAR *bp;  /* current position in buffer */
   int trunc;        /* true if line too long */
} BFILE;


#ifndef INTERNAL_H

/* systems that do not provide a stricmp() can define NEED_STRICMP to
   use a local version called vstricmp().
*/


#ifdef NEED_STRICMP

#undef vstricmp
#undef vstrnicmp

int vstricmp(P1(char DB_FAR *) Pi(char DB_FAR *));
int vstrnicmp(P1(char DB_FAR *) Pi(char DB_FAR *, size_t));

#else

/* use the system version */
#define vstricmp(s1, s2)     stricmp((char DB_FAR *)(s1), (char DB_FAR *)(s2))
#define vstrnicmp(s1, s2, n) strnicmp((char DB_FAR *)(s1), (char DB_FAR*)(s2), (unsigned)(n))

#endif   /* NEED_STRICMP */



#define bytecpy(s1, s2, n)  memcpy((DB_VOID *)(s1), (DB_VOID *)(s2), (unsigned)(n))
#define bytecmp(s1, s2, n)  memcmp((DB_VOID *)(s1), (DB_VOID *)(s2), (unsigned)(n))
#define byteset(s, c, n)    memset((DB_VOID *)(s), (int)(c), (unsigned)(n))
#define bytemove(s1, s2, n) memmove((DB_VOID *)(s1), (DB_VOID *)(s2), (unsigned)(n))
#define vstrcpy(s1, s2)     strcpy((char *)(s1), (char *)(s2))
#define vstrcat(s1, s2)     strcat((char *)(s1), (char *)(s2))
#define vstrcmp(s1, s2)     strcmp((char *)(s1), (char *)(s2))
#define vstrlen(s)          strlen((char *)(s))
#define vstrncmp(s1, s2, n) strncmp((char *)(s1), (char *)(s2), (unsigned)(n))
#define vstrncpy(s1, s2, n) strncpy((char *)(s1), (char *)(s2), (unsigned)(n))
#define vstrncat(s1, s2, n) strncat((char *)(s1), (char *)(s2), (unsigned)(n))
#define vstrchr(s, c)       strchr((char *)(s), (int)(c))
#define vstrrchr(s, c)      strrchr((char *)(s), (int)(c))
#define vstrupr(s)          strupr((char *)(s))


/*
   extended string functions in libfcns.c
*/
char DB_FAR *vstrnzcpy(P1(char DB_FAR *) Pi(CONST char DB_FAR *) Pi(size_t));
char DB_FAR *vstrnzcat(P1(char DB_FAR *) Pi(CONST char DB_FAR *) Pi(size_t));
int vstrtoi(P1(char DB_FAR *));

typedef struct
{
   char DB_FAR *strbuf;
   size_t buflen;
   size_t strlen;
} DB_STRING;

/*
   The prefix "STR" cannot be used because some runtime libraries, such as
   Microsoft when linked with oldnames.lib, use names like STRcpy internally.
*/
#define STRinit   DB_STRING_init
#define STRcpy    DB_STRING_cpy
#define STRcat    DB_STRING_cat
#define STRccat   DB_STRING_ccat
#define STRavail  DB_STRING_avail

char DB_FAR *STRinit(P1(DB_STRING DB_FAR *) Pi(char DB_FAR *) Pi(size_t));
char DB_FAR *STRcpy(P1(DB_STRING DB_FAR *) Pi(char DB_FAR *));
char DB_FAR *STRcat(P1(DB_STRING DB_FAR *) Pi(char DB_FAR *));
char DB_FAR *STRccat(P1(DB_STRING DB_FAR *) Pi(int));
size_t STRavail(P1(DB_STRING DB_FAR *));


#ifdef DB_TRACE
#ifdef COMMA
#define D_ARGS(a)        , (void DB_FAR *)(a)
#define AD_ARGS_ID       , void DB_FAR *args
#define D_ARGS_ID        , args
#else
#define D_ARGS(a)        (void DB_FAR *)(a)
#define AD_ARGS_ID       void DB_FAR *args
#define D_ARGS_ID        args
#define COMMA
#endif
#define D_ARGS_DECL      void DB_FAR *args;
#define D_ARGS_Dn        , void DB_FAR *
#else
#define D_ARGS(a)        /**/
#define AD_ARGS_ID       /**/
#define D_ARGS_ID        /**/
#define D_ARGS_DECL      /**/
#define D_ARGS_Dn        /**/
#define D_ARGS_PARAM     /**/
#endif


#ifdef COMMA 
#define DB_ID           , dbn
#define ADB_ID          , int dbn
#define NO_DB_ID        , VOID_DB
#define DBN_PARAM       , dbn
#else
#define DB_ID           dbn
#define ADB_ID          int dbn
#define NO_DB_ID        VOID_DB
#define DBN_PARAM       dbn
#define COMMA
#endif /* COMMA */

#ifdef MULTI_TASK
#define ALL_DBS_PARAM   , ALL_DBS
#else
#define ALL_DBS_PARAM   ALL_DBS
#endif


#ifdef MULTI_TASK
#ifdef COMMA

#define TASK_ID         , task
#define TASK_PTR_ID     , task
#define ATASK_PTR_ID    , DB_TASK DB_FAR *task
#define NO_TASK_ID      , NULL

#else /* COMMA */

#define TASK_ID         task
#define TASK_PTR_ID     task
#define ATASK_PTR_ID    DB_TASK DB_FAR *task
#define NO_TASK_ID      NULL
#endif 

#ifdef COMMA
#define TASK_PARAM      , c
#else
#define TASK_PARAM      c
#define COMMA
#endif /* COMMA */

#else /* MULTI_TASK */

#define TASK_PARAM      /**/
#define TASK_ID         /**/
#define TASK_PTR_ID     /**/
#define ATASK_PTR_ID    /**/
#define NO_TASK_ID      /**/

#endif /* MULTI_TASK */

#define ALLOC(l, b)     d_alloc((CHAR_P DB_FAR *)(l), b)
#define FREE(cp)        d_free((CHAR_P DB_FAR *)(cp))

#ifdef DB_TRACE
#define API_ENTER(fn,a) {if (db_trace & TRACE_API) api_enter(fn,a);}
#define API_RETURN(c)   return((db_trace & TRACE_API) ? api_exit() : 0, (c))
#define API_EXIT()      {if (db_trace & TRACE_API) api_exit();}

#define FN_ENTER(fn)    {if (db_trace & TRACE_API) fn_enter(fn);}
#define FN_RETURN(c)    return((db_trace & TRACE_API) ? fn_exit() : 0, (c))
#define FN_EXIT()       {if (db_trace & TRACE_API) fn_exit();}
#else
#define API_ENTER(fn,a) /**/
#define API_RETURN(c)   return(c)
#define API_EXIT()      /**/

#define FN_ENTER(fn)    /**/
#define FN_RETURN(c)    return(c)
#define FN_EXIT()       /**/
#endif

#define DB_EXIT(x)    db_exit(x)
#define RETURN(x)     return DB_EXIT(x)

#define ENCODE_DBA(file, slot, dba) (*(dba)=((FILEMASK&(file))<<FILESHIFT)|\
                                             (ADDRMASK&(slot)))
#define DECODE_DBA(dba, file, slot) {*(file)=(short)(FILEMASK&((dba)>>FILESHIFT));\
                                     *(slot)=(unsigned long)ADDRMASK&(dba);}

#endif /* INTERNAL_H */

typedef struct {
	struct CACHE_S DB_FAR *ptr;
	LOCK_DESC
} CACHE_P;

typedef struct {
	CACHE_P DB_FAR *ptr;
	LOCK_DESC
} CACHE_TABLE_P;

#define DB_TIMEOUT     10     /* lock request wait 10 seconds in queue */
#define NET_TIMEOUT    30     /* how long to wait for a message reply */
#define THREAD_TIMEOUT 60     /* Number of seconds thread should
                                 wait for Mutex before timing out */

/* record/set lock descriptor */
struct lock_descr
{
   char        fl_type;          /* 'r'ead, 'w'rite, e'x'clusive, 'f'ree */
   char        fl_prev;          /* previous lock type */
   FILE_NO_P   fl_list;          /* array of files used by record/set */
   int         fl_cnt;           /* Number of elements in fl_list */
   DB_BOOLEAN     fl_kept;          /* Is lock kept after transaction? */
};
typedef struct
{
   struct lock_descr DB_FAR *ptr;
   LOCK_DESC
} LOCK_DESCR_P;

/* Maximum number of transactions which can commit a time, 2 sectors */
/* TAFLIMIT = ((512 - 3 * sizeof(short) - LOCKMGRLEN) / LOGFILELEN) */
#define TAFLIMIT 9

/* structure to read TAF data into */
typedef struct TAFFILE_S
{
   short    lmc_type;
   short    user_count;
   char     lmc_lockmgrn[LOCKMGRLEN];
   short    cnt;
   char     files[TAFLIMIT][LOGFILELEN];
}  TAFFILE;

#ifndef INTERNAL_H

#define DB_STATIC    static

#ifdef MULTI_TASK
#define VISTATASK_ONLY        &Vistatask
#define ONLY_VISTATASK_DBN    &Vistatask DBN_PARM
#define VISTATASK_PARM        , &Vistatask
#else
#define VISTATASK_ONLY        /**/
#define ONLY_VISTATASK_DBN    DBN_ONLY
#define VISTATASK_PARM        /**/
#endif

#endif /* INTERNAL_H */

/* MTYPE is .eq. to fcn and is used with the lockmgr */
typedef unsigned short MTYPE;

#include <sys/types.h>  /* some compilers need this before stat.h */
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>

#ifdef TURBO
typedef long off_t;
#endif

#define NO_SEEK      ((off_t)-1)

#ifdef DBSTAT
#include "dbstat.h"     /* must be before dbxtrn.h */
#endif
#include "trxlog.h"
#ifndef MSGVER
#define MSGVER  0x450
#endif
#include "dblock.h"     /* must be before dbxtrn.h */
#include "ll.h"
#include "dbxtrn.h"

#include <share.h>

#define UNLOCK_FILE  0
#define LOCK_FILE    1


#define REALP(x)  x
#define PROTP(x)  x

#ifndef RDM_REGS_DEFINED

struct RDM_WORDREGS
{
   unsigned int ax;
   unsigned int bx;
   unsigned int cx;
   unsigned int dx;
   unsigned int si;
   unsigned int di;
   unsigned int cflag;
   unsigned int es;
   unsigned int ds;
};

struct RDM_BYTEREGS
{
   unsigned char al, ah;
   unsigned char bl, bh;
   unsigned char cl, ch;
   unsigned char dl, dh;
};

union RDM_REGS
{
   struct RDM_WORDREGS x;
   struct RDM_BYTEREGS h;
};

#define RDM_REGS_DEFINED
#endif

#ifndef INTERNAL_H

#include "proto.h"


#endif /* INTERNAL_H */

#ifdef MULTI_TASK
#ifdef NO_DT_COVER
#include "dtcover.h"
#endif
#endif

#include "apidefs.h"

#endif /* OMAN */
#endif /* DBTYPE_H */

/* $Revision:   1.120  $ */

/* vpp DBTYPE.H (Fri Sep 19 11:56:08 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

