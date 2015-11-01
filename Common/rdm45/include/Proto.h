/*----------------------------------------------------------------------------
   proto.h: db_VISTA function declaration header file

   (This file must be included after dbtype.h)

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef PROTO_H
#define PROTO_H

/* From dberr.c */
void EXTERNAL_FIXED dberr_msg(P1(ERR_INFO DB_FAR *) Pi(char DB_FAR *) Pi(int));
ERR_INFO DB_FAR * EXTERNAL_FIXED dberr_handler(P1(ERRORPROC) Pi(ERR_INFO DB_FAR *) Pi(char DB_FAR *));

#ifdef DBERR_EX
#define DBERR_FILE __FILE__
#define DBERR_LINE __LINE__
#else
#define DBERR_FILE NULL
#define DBERR_LINE 0
#endif

#ifndef NOLIB
int  EXTERNAL_FIXED _dberr(P1(int) Pi(char DB_FAR *) Pi(int) Pi(TASK DB_FAR *));
void EXTERNAL_FIXED flush_dberr(P1(TASK DB_FAR *));
#define dberr(n)  _dberr(n, DBERR_FILE, DBERR_LINE, NULL)
#else
int  EXTERNAL_FIXED _dberr(P1(int) Pi(char DB_FAR *) Pi(int));
void EXTERNAL_FIXED flush_dberr(P0);
#define dberr(n)  _dberr(n, DBERR_FILE, DBERR_LINE)
#endif

/* From alloc.c: */
DB_VOID DB_FAR *INTERNAL_FIXED d_alloc(CHAR_P DB_FAR *, size_t);
void INTERNAL_FIXED d_free(P1(CHAR_P DB_FAR *));

/* From bfile.c: */
BFILE DB_FAR * INTERNAL_FIXED b_open(P1(char DB_FAR *) Pi(size_t));
int INTERNAL_FIXED b_close(P1(BFILE DB_FAR *));
long INTERNAL_FIXED b_lseek(P1(BFILE DB_FAR *) Pi(long) Pi(int));
char DB_FAR * INTERNAL_FIXED b_gets(P1(BFILE DB_FAR *));
int INTERNAL_FIXED b_getc(P1(BFILE DB_FAR *));

/* From dberr.c: */
void INTERNAL_FIXED dbautorec(P0);

/* From dblfcns.c: */
int INTERNAL_FIXED taskinit(P1(TASK DB_FAR *));
int INTERNAL_FIXED cacheinit(P1(CACHE DB_FAR *));
unsigned int INTERNAL_FIXED get_process_id(P0);
int INTERNAL_FIXED initdbt(P1(CONST char DB_FAR *));
int INTERNAL_FIXED alloc_table(P1(CHAR_P DB_FAR *) Pi(size_t) Pi(size_t));
int INTERNAL_FIXED calloc_table(P1(CHAR_P DB_FAR *) Pi(size_t) Pi(size_t) Pi(size_t));
#define FREE_TABLE(a, b, c, d, e) free_table((CHAR_P DB_FAR *)a, b, c, d, e)
int INTERNAL_FIXED free_table(P1(CHAR_P DB_FAR *) Pi(int) Pi(int) Pi(int) Pi(size_t));
int INTERNAL_FIXED recovery_check(P0);

/* From lockfcns.c: */
int INTERNAL_FIXED send_lock_pkt(P0);
int INTERNAL_FIXED send_free_pkt(P0);
void INTERNAL_FIXED reset_locks(P0);
int rdm_trans (P1(MTYPE) Pi(void DB_FAR *) Pi(size_t) Pi(void DB_FAR *) Pi(size_t));

int  INTERNAL_FIXED free_dblocks(DBN_D1);
void INTERNAL_FIXED termfree(DBN_D1);

int INTERNAL_FIXED neterr(P0);

#ifdef MULTI_TASK
int INTERNAL_FIXED dupid_check(P0);
#endif

#ifdef DBSTAT
/* From dbstat.c: */
void sync_MEM_STATS(P1(MEM_STATS DB_FAR *) Pi(MEM_STATS DB_FAR *));
void sync_CACHE_STATS(P1(CACHE_STATS DB_FAR *) Pi(CACHE_STATS DB_FAR *));

#ifndef INLINE_STATS

void INTERNAL_FIXED STAT_mem_alloc(PAGE_TABLE DB_FAR *cache, size_t size);
void INTERNAL_FIXED STAT_lookups(FILE_NO fno);
void INTERNAL_FIXED STAT_hits(FILE_NO fno);
void INTERNAL_FIXED STAT_pages(FILE_NO fno, DB_SHORT num);
void INTERNAL_FIXED STAT_file_open(FILE_NO fno);
void INTERNAL_FIXED STAT_pz_read(FILE_NO fno, size_t amt);
void INTERNAL_FIXED STAT_pz_write(FILE_NO fno, size_t amt);
void INTERNAL_FIXED STAT_pg_read(FILE_NO fno, size_t amt);
void INTERNAL_FIXED STAT_pg_write(FILE_NO fno, size_t amt);
void INTERNAL_FIXED STAT_rlb_read(FILE_NO fno, size_t amt);
void INTERNAL_FIXED STAT_rlb_write(FILE_NO fno, size_t amt);
void INTERNAL_FIXED STAT_new_page(FILE_NO fno);
void INTERNAL_FIXED STAT_log_open(void);
void INTERNAL_FIXED STAT_log_read(size_t amt);
void INTERNAL_FIXED STAT_log_write(size_t amt);
void INTERNAL_FIXED STAT_taf_open(void);
void INTERNAL_FIXED STAT_taf_read(size_t amt);
void INTERNAL_FIXED STAT_taf_write(size_t amt);
void INTERNAL_FIXED STAT_dbl_open(void);
void INTERNAL_FIXED STAT_dbl_read(size_t amt);
void INTERNAL_FIXED STAT_dbl_write(size_t amt);
void INTERNAL_FIXED STAT_max_open(int num);
void INTERNAL_FIXED STAT_trbegin(void);
void INTERNAL_FIXED STAT_trend(void);
void INTERNAL_FIXED STAT_trabort(void);
void INTERNAL_FIXED STAT_lock(FILE_NO fno, int type);
void INTERNAL_FIXED STAT_send_msg(MTYPE mtype, size_t msglen, size_t tot_pkts);
void INTERNAL_FIXED STAT_recv_msg(MTYPE mtype, size_t msglen, size_t tot_pkts);

#else

#define STAT_mem_alloc(cache, size)   \
{                                                                 \
   (cache)->mem_stats.allocs++;                                   \
   (cache)->mem_stats.mem_used += (size);                         \
   if ((cache)->mem_stats.mem_used > (cache)->mem_stats.max_mem)  \
      (cache)->mem_stats.max_mem = (cache)->mem_stats.mem_used;   \
}
#define _STAT_lookups(cache_stats) \
{                                                  \
   if ((cache_stats)->lookups > LONG_MAX)          \
   { (cache_stats)->lookups >>= 1; (cache_stats)->hits >>= 1; }  \
   (cache_stats)->lookups++;                       \
}
#define STAT_lookups(fno)   \
{                                                  \
   if (fno != ov_file)                             \
   {                                               \
      _STAT_lookups(&file_stats[fno].cache_stats); \
      _STAT_lookups(&db_pgtab.cache_stats);        \
   }                                               \
   else                                            \
      _STAT_lookups(&ix_pgtab.cache_stats);        \
}
#define STAT_hits(fno)   \
{                                            \
   if (fno != ov_file)                       \
   {                                         \
      file_stats[fno].cache_stats.hits++;    \
      db_pgtab.cache_stats.hits++;           \
   }                                         \
   else                                      \
      ix_pgtab.cache_stats.hits++;           \
}
#define STAT_pages(fno, num)   \
{                                                     \
   if (fno != ov_file)                                \
   {                                                  \
      file_stats[fno].cache_stats.num_pages += (num); \
      db_pgtab.cache_stats.num_pages += (num);        \
   }                                                  \
   else                                               \
      ix_pgtab.cache_stats.num_pages += (num);        \
}
#define STAT_file_open(fno)     \
   file_stats[fno].file_opens++, gen_stats.file_opens++
#define STAT_pz_read(fno, amt)  \
   file_stats[fno].pz_stats.read_count++, file_stats[fno].pz_stats.read_bytes += (amt),   \
   gen_stats.pz_stats.read_count++, gen_stats.pz_stats.read_bytes += (amt)
#define STAT_pz_write(fno, amt) \
   file_stats[fno].pz_stats.write_count++, file_stats[fno].pz_stats.write_bytes += (amt), \
   gen_stats.pz_stats.write_count++, gen_stats.pz_stats.write_bytes += (amt)
#define STAT_pg_read(fno, amt)  \
   file_stats[fno].pg_stats.read_count++, file_stats[fno].pg_stats.read_bytes += (amt),   \
   gen_stats.pg_stats.read_count++, gen_stats.pg_stats.read_bytes += (amt)
#define STAT_pg_write(fno, amt) \
   file_stats[fno].pg_stats.write_count++, file_stats[fno].pg_stats.write_bytes += (amt), \
   gen_stats.pg_stats.write_count++, gen_stats.pg_stats.write_bytes += (amt)
#define STAT_rlb_read(fno, amt) \
   file_stats[fno].rlb_stats.read_count++, file_stats[fno].rlb_stats.read_bytes += (amt), \
   gen_stats.rlb_stats.read_count++, gen_stats.rlb_stats.read_bytes += (amt)
#define STAT_rlb_write(fno, amt)\
   file_stats[fno].rlb_stats.write_count++, file_stats[fno].rlb_stats.write_bytes += (amt), \
   gen_stats.rlb_stats.write_count++, gen_stats.rlb_stats.write_bytes += (amt)
#define STAT_new_page(fno)   \
   file_stats[fno].new_pages++, gen_stats.new_pages++
#define STAT_log_open()      \
   gen_stats.log_opens++
#define STAT_log_read(amt)   \
   gen_stats.log_stats.read_count++, gen_stats.log_stats.read_bytes += (amt)
#define STAT_log_write(amt)  \
   gen_stats.log_stats.write_count++, gen_stats.log_stats.write_bytes += (amt)
#define STAT_taf_open()      \
   gen_stats.taf_opens++
#define STAT_taf_read(amt)   \
   gen_stats.taf_stats.read_count++, gen_stats.taf_stats.read_bytes += (amt)
#define STAT_taf_write(amt)  \
   gen_stats.taf_stats.write_count++, gen_stats.taf_stats.write_bytes += (amt)
#define STAT_dbl_open()      \
   gen_stats.dbl_opens++
#define STAT_dbl_read(amt)   \
   gen_stats.dbl_stats.read_count++, gen_stats.dbl_stats.read_bytes += (amt)
#define STAT_dbl_write(amt)  \
   gen_stats.dbl_stats.write_count++, gen_stats.dbl_stats.write_bytes += (amt)
#define STAT_max_open(num)   \
{                                          \
   if ((num) > gen_stats.max_files_open)   \
      gen_stats.max_files_open = (num);    \
}
#define STAT_trbegin() \
   gen_stats.trbegins++
#define STAT_trend()   \
{                          \
   gen_stats.trends++;     \
   if (cache_ovfl)         \
      gen_stats.trovfl++;  \
}
#define STAT_trabort() \
   gen_stats.traborts++
#define STAT_lock(fno, type)  \
   ((DB_ULONG DB_FAR *)&file_stats[fno].lock_stats)[type]++, \
   ((DB_ULONG DB_FAR *)&gen_stats.lock_stats)[type]++
#define STAT_send_msg(mtype, msglen, tot_pkts) \
{                                                  \
   MSG_STATS DB_FAR *mstat = &lmcmsg_stats[mtype]; \
   size_t len = (msglen); size_t pkts = (tot_pkts);\
   mstat->msg_count++;                             \
   mstat->send_packets += pkts;                    \
   mstat->send_bytes += len;                       \
   mstat = &gen_stats.msg_stats;                   \
   mstat->msg_count++;                             \
   mstat->send_packets += pkts;                    \
   mstat->send_bytes += len;                       \
}
#define STAT_recv_msg(mtype, msglen, tot_pkts) \
{                                                  \
   MSG_STATS DB_FAR *mstat = &lmcmsg_stats[mtype]; \
   size_t len = (msglen); size_t pkts = (tot_pkts);\
   mstat->recv_packets += pkts;                    \
   mstat->recv_bytes += len;                       \
   mstat = &gen_stats.msg_stats;                   \
   mstat->recv_packets += pkts;                    \
   mstat->recv_bytes += len;                       \
}

#endif   /* INLINE_STATS */

/* Constants for use by STAT_lock() - they are offsets into the
   LOCK_STATS structure
*/
#define STAT_LOCK_r       0   /* r = read lock */
#define STAT_LOCK_w       1   /* w = write lock */
#define STAT_LOCK_x       2   /* x = exclusive lock */
#define STAT_LOCK_R       3   /* R = record lock bit */
#define STAT_LOCK_r2w     4
#define STAT_LOCK_r2x     5
#define STAT_LOCK_r2R     6
#define STAT_LOCK_w2r     7
#define STAT_LOCK_x2r     8
#define STAT_LOCK_R2r     9
#define STAT_FREE_r      10
#define STAT_FREE_w      11
#define STAT_FREE_x      12
#define STAT_FREE_R      13

#endif   /* DBSTAT */

/* From dio.c: */
int  INTERNAL_FIXED dio_open(P1(FILE_NO));
int  INTERNAL_FIXED dio_closelru(P0);
int  INTERNAL_FIXED dio_close(P1(FILE_NO));
int  INTERNAL_FIXED dio_init(P0);
void INTERNAL_FIXED dio_free(DBN_D1);
int  INTERNAL_FIXED dio_clear(DBN_D1);
int  INTERNAL_FIXED dio_clrfile(P1(FILE_NO));
int  INTERNAL_FIXED dio_clrpage(P1(PAGE_ENTRY DB_FAR *));

void INTERNAL_FIXED dio_ixclear(P0);
int  INTERNAL_FIXED dio_flush(P0);
int  INTERNAL_FIXED dio_get(P1(FILE_NO) Pi(F_ADDR) Pi(char DB_FAR *DB_FAR *) Pi(int));
int  INTERNAL_FIXED dio_touch(P1(FILE_NO) Pi(F_ADDR) Pi(int));
int  INTERNAL_FIXED dio_unget(P1(FILE_NO) Pi(F_ADDR) Pi(int));
int  INTERNAL_FIXED dio_read(P1(DB_ADDR) Pi(char DB_FAR *DB_FAR *) Pi(int));
int  INTERNAL_FIXED dio_write(P1(DB_ADDR) Pi(int));
int  INTERNAL_FIXED dio_release(P1(DB_ADDR) Pi(int));
int  INTERNAL_FIXED dio_rrlb(P1(DB_ADDR) Pi(DB_SHORT DB_FAR *));
int  INTERNAL_FIXED dio_wrlb(P1(DB_ADDR) Pi(DB_SHORT));
int  INTERNAL_FIXED dio_findpg(P1(FILE_NO) Pi(F_ADDR)
                               Pi(PAGE_ENTRY DB_FAR * DB_FAR *)
                               Pi(PAGE_ENTRY DB_FAR * DB_FAR *));
int  INTERNAL_FIXED dio_getpg(P1(FILE_NO) Pi(F_ADDR) Pi(short)
                              Pi(PAGE_ENTRY DB_FAR * DB_FAR *)
                              Pi(PAGE_ENTRY DB_FAR * DB_FAR *));

int INTERNAL_FIXED dio_out(P1(PAGE_ENTRY DB_FAR *));
int INTERNAL_FIXED dio_in(P1(PAGE_ENTRY DB_FAR *));

DB_ULONG INTERNAL_FIXED dio_pzsetts(P1(FILE_NO));
DB_ULONG INTERNAL_FIXED dio_pzgetts(P1(FILE_NO));

int INTERNAL_FIXED dio_pzread(P1(FILE_NO));
int INTERNAL_FIXED dio_pzalloc(P1(FILE_NO) Pi(F_ADDR DB_FAR *));
int INTERNAL_FIXED dio_pzdel(P1(FILE_NO) Pi(F_ADDR));
F_ADDR INTERNAL_FIXED dio_pznext(P1(FILE_NO));
F_ADDR INTERNAL_FIXED dio_pages(P1(FILE_NO));
void INTERNAL_FIXED dio_pzclr(P0);
#ifdef DB_DEBUG
int INTERNAL_FIXED dio_pzcheck(P1(FILE_NO));
off_t INTERNAL_FIXED dio_filesize(P1(FILE_NO));
#endif
int INTERNAL_FIXED dio_readfile(P1(FILE_NO) Pi(off_t) Pi(char DB_FAR *) Pi(size_t) Pi(DB_ULONG));
int INTERNAL_FIXED dio_writefile(P1(FILE_NO) Pi(off_t) Pi(char DB_FAR *) Pi(size_t) Pi(DB_ULONG));
#ifdef DB_DEBUG
int INTERNAL_FIXED check_cache(P1(char DB_FAR *));
#endif

/* From inifile.c */
int   INTERNAL_FIXED rdmIniFileOpen(P0);
int   INTERNAL_FIXED rdmIniFileClose(P0);
short INTERNAL_FIXED rdmGetIniShort(P1(char DB_FAR *) Pi(char DB_FAR *) Pi(short));
short INTERNAL_FIXED rdmGetIniString(P1(char DB_FAR *) Pi(char DB_FAR *)
                                     Pi(char DB_FAR *) Pi(char DB_FAR *) Pi(int));

/* From inittab.c: */
int INTERNAL_FIXED inittab(P0);

/* From keyfcns.c: */
int INTERNAL_FIXED key_open(P0);
void INTERNAL_FIXED key_close(DBN_D1);
int INTERNAL_FIXED key_init(P1(int));
int INTERNAL_FIXED key_reset(P1(FILE_NO));
int INTERNAL_FIXED key_locpos(P1(CONST char DB_FAR *) Pi(DB_ADDR DB_FAR *));
int INTERNAL_FIXED key_scan(P1(int) Pi(DB_ADDR DB_FAR *));
int INTERNAL_FIXED key_boundary(P1(int) Pi(DB_ADDR DB_FAR *));
int INTERNAL_FIXED key_insert(P1(int) Pi(CONST char DB_FAR *) Pi(DB_ADDR));
int INTERNAL_FIXED key_delete(P1(int) Pi(CONST char DB_FAR *) Pi(DB_ADDR));

/* Due to a problem with MSC 5.1, we couldn't declare the 2nd parm
   of key_bldcom as const (see keyfcns.c)
*/
int INTERNAL_FIXED key_bldcom(P1(int)
                              Pi(char DB_FAR *)
                              Pi(char DB_FAR *) Pi(int));

/* Because of a "problem" with the MSC 5.1 compiler, we couldn't declare
   the second argument to key_cmpcpy with a const attribute.  Hence we
   were not able to define the 2nd argument of key_bldcom as const.
*/
void INTERNAL_FIXED key_cmpcpy(P1(char DB_FAR *)
                                 Pi(char DB_FAR *)
                                 Pi(DB_SHORT));
void INTERNAL_FIXED key_acpy(P1(char DB_FAR *)
                              Pi(char DB_FAR *)
                              Pi(DB_SHORT));

/* From libfcns.c: */
int INTERNAL_FIXED nset_check(P1(int) Pi(int DB_FAR *)
                              Pi(SET_ENTRY DB_FAR *DB_FAR *));
int INTERNAL_FIXED nfld_check(P1(long)
                              Pi(int DB_FAR *) Pi(int DB_FAR *)
                              Pi(RECORD_ENTRY DB_FAR *DB_FAR *)
                              Pi(FIELD_ENTRY DB_FAR *DB_FAR *));
int INTERNAL_FIXED nrec_check(P1(int)
                              Pi(int DB_FAR *)
                              Pi(RECORD_ENTRY DB_FAR *DB_FAR *));
int INTERNAL_FIXED fldcmp(P1(FIELD_ENTRY DB_FAR *)
                           Pi(CONST char DB_FAR *)
                           Pi(CONST char DB_FAR *));
int INTERNAL_FIXED SHORTcmp(P1(CONST char DB_FAR *) Pi(CONST char DB_FAR *));
int INTERNAL_FIXED ADDRcmp(P1(CONST DB_ADDR DB_FAR *)
                           Pi(CONST DB_ADDR DB_FAR *));
int INTERNAL_FIXED null_dba(P1(CONST DB_ADDR));
int INTERNAL_FIXED check_dba(P1(DB_ADDR));
int INTERNAL_FIXED ctblcmp(P1(CONST unsigned char DB_FAR *)
                           Pi(CONST unsigned char DB_FAR *)
                           Pi(int));

/* From makenew.c: */
int INTERNAL_FIXED sk_free(P0);


/* From opens.c: */
int   INTERNAL_FIXED open_b(P1(CONST char DB_FAR *) Pi(unsigned int));
int   INTERNAL_FIXED open_u(P1(CONST char DB_FAR *));
int   INTERNAL_FIXED open_x(P1(CONST char DB_FAR *));
int   INTERNAL_FIXED os_open(P1(CONST char DB_FAR *) Pi(unsigned int) Pi(unsigned int));
int   INTERNAL_FIXED commit_file(P1(FILE_NO) Pi(int));
int   INTERNAL_FIXED os_access(P1(CONST char DB_FAR *) Pi(int));
int   INTERNAL_FIXED os_share(P0);
int   INTERNAL_FIXED os_lseek(P1(int) Pi(off_t) Pi(int));
int   INTERNAL_FIXED os_read(P1(int) Pi(char DB_FAR *) Pi(size_t));
int   INTERNAL_FIXED os_pread(P1(int) Pi(off_t) Pi(char DB_FAR *) Pi(size_t));
int   INTERNAL_FIXED os_plread(P1(int) Pi(off_t) Pi(char DB_FAR *) Pi(size_t));
int   INTERNAL_FIXED os_write(P1(int) Pi(CONST char DB_FAR *) Pi(size_t));
int   INTERNAL_FIXED os_pwrite(P1(int) Pi(off_t) Pi(CONST char DB_FAR *) Pi(size_t));
int   INTERNAL_FIXED os_plwrite(P1(int) Pi(off_t) Pi(CONST char DB_FAR *) Pi(size_t));
int   INTERNAL_FIXED os_stat(P1(CONST char DB_FAR *) Pi(struct _stat DB_FAR *));
off_t INTERNAL_FIXED os_filesize(P1(int));
time_t INTERNAL_FIXED os_mtime(P1(int));
int   INTERNAL_FIXED os_commit(P1(int handle));
int   INTERNAL_FIXED os_close(P1(int handle));
int   INTERNAL_FIXED os_unlink(P1(char DB_FAR *));
char DB_FAR * INTERNAL_FIXED os_getenv(P1(char DB_FAR *));
int  INTERNAL_FIXED RelinquishControl(P0);
#define BY_NAP_SUCCESS 1
#define BY_NAP_FAILURE 0

/* apply some randomness */
#define SUCCESS_FACTOR  0.100
#define FAILURE_FACTOR  1.300

#define MAX_FACTOR      2.000
#define DEF_FACTOR      0.010
#define MIN_FACTOR      0.005
#define ONE_FACTOR      0.001    
/* if (2 * ONE_USER_NAP) >= MIN_FACTOR then change naptime() */

void INTERNAL_FIXED adjust_naptime (P1(int));
void INTERNAL_FIXED naptime (P0);


/* From ovfcns.c: */
int  INTERNAL_FIXED o_setup(P0);
int  INTERNAL_FIXED o_init(P0);
int  INTERNAL_FIXED o_fileinit(P1(FILE_NO));
int  INTERNAL_FIXED o_search(P1(FILE_NO) Pi(F_ADDR) Pi(F_ADDR DB_FAR *));
int  INTERNAL_FIXED o_pzwrite(P1(FILE_NO));
int  INTERNAL_FIXED o_flush(P0);
int  INTERNAL_FIXED o_update(P0);
long INTERNAL_FIXED o_pages(P1(FILE_NO));
void INTERNAL_FIXED o_free(DBN_D1);
int  INTERNAL_FIXED o_close(P0);
int  INTERNAL_FIXED o_write(P1(PAGE_ENTRY DB_FAR *));

/* From recfcns.c: */
int INTERNAL_FIXED r_chkfld(P1(DB_SHORT) Pi(FIELD_ENTRY DB_FAR *)
                              Pi(char DB_FAR *)
                              Pi(CONST char DB_FAR *));
int INTERNAL_FIXED r_delrec(P1(DB_SHORT) Pi(DB_ADDR));
int INTERNAL_FIXED r_gfld(P1(FIELD_ENTRY DB_FAR *) Pi(char DB_FAR *)
                           Pi(char DB_FAR *));
int INTERNAL_FIXED r_gmem(P1(int) Pi(char DB_FAR *) Pi(MEM_PTR DB_FAR *));
int INTERNAL_FIXED r_gset(P1(int) Pi(char DB_FAR *) Pi(SET_PTR DB_FAR *));
int INTERNAL_FIXED r_pfld(P1(DB_SHORT) Pi(FIELD_ENTRY DB_FAR *)
                           Pi(char DB_FAR *)
                           Pi(CONST char DB_FAR *) Pi(DB_ADDR DB_FAR *));
int INTERNAL_FIXED r_pmem(P1(int) Pi(char DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED r_pset(P1(int) Pi(char DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED r_smem(P1(DB_ADDR DB_FAR *) Pi(int));
int INTERNAL_FIXED r_setopt(P1(FIELD_ENTRY DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED r_clropt(P1(FIELD_ENTRY DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED r_tstopt(P1(FIELD_ENTRY DB_FAR *) Pi(char DB_FAR *));

/* From pathfcns.c */
int INTERNAL_FIXED con_dbd(P1(char DB_FAR *) Pi(char DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED con_dbf(P1(char DB_FAR *) Pi(char DB_FAR *) Pi(char DB_FAR *)
                              Pi(char DB_FAR *));
char DB_FAR *INTERNAL_FIXED get_element(P1(CONST char DB_FAR *) Pi(int));

/* From renfile.c: */
int INTERNAL_FIXED renfiles(P0);

/* From enter.c: */
int INTERNAL_FIXED db_enter(P1(int) D_ARGS_Dn DBN_Dn TASK_PTR_Di);
int INTERNAL_FIXED db_exit(P1(int));
#ifdef DB_TRACE
int INTERNAL_FIXED api_enter(P1(int) Pi(void DB_FAR *));
int INTERNAL_FIXED api_exit(P0);
int INTERNAL_FIXED fn_enter(P1(char DB_FAR *));
int INTERNAL_FIXED fn_exit(P0);
int DB_FAR CDECL   db_printf(char DB_FAR *fmt, ...);
int INTERNAL_FIXED db_indent(P0);
int INTERNAL_FIXED db_undent(P0);
#endif

/* From taffcns.c: */
int INTERNAL_FIXED rdm_taf_open(P0);
int INTERNAL_FIXED rdm_taf_close(P0);
int INTERNAL_FIXED rdm_taf_access(P1(int));
int INTERNAL_FIXED rdm_taf_release(P0);
int INTERNAL_FIXED rdm_taf_add(P1(CONST char DB_FAR *));
int INTERNAL_FIXED rdm_taf_del(P1(CONST char DB_FAR *));
int INTERNAL_FIXED rdm_taf_login(P0);
int INTERNAL_FIXED rdm_taf_logout(P0);
int INTERNAL_FIXED rdm_taf_locking(P1(int) Pi(int) Pi(long) );
int INTERNAL_FIXED truename_supported(P0);

/* From truename.c */
int INTERNAL_FIXED truename(P1(char DB_FAR *) Pi(CONST char DB_FAR *) Pi(size_t));
int INTERNAL_FIXED NWRemoteFile(P1(CONST char DB_FAR *));

/* From task.c */
#ifdef MULTI_TASK
int INTERNAL_FIXED task_switch(P1(DB_TASK DB_FAR *));
int INTERNAL_FIXED remove_cache(P1(DB_TASK DB_FAR *));
int INTERNAL_FIXED ntask_check(P1(DB_TASK DB_FAR *));
int INTERNAL_FIXED GetTaskMutex(P1(HANDLE DB_FAR *));
#endif

/* From mapchar.c */
int  INTERNAL_FIXED ctb_init(P0);
int  INTERNAL_FIXED ctbl_alloc(P0);
void INTERNAL_FIXED ctbl_free(P0);
int  INTERNAL_FIXED ctbl_ignorecase(P0);
int  INTERNAL_FIXED ctbl_usecase(P0);


/* From libmain.c */

#ifdef LIBMAIN_C
#define EXTERN /**/
#else
#define EXTERN extern
#endif

EXTERN int (INTERNAL_FIXED *closeTTS)  (P0);
EXTERN int (INTERNAL_FIXED *checkTTS)  (P1(FILE_ENTRY DB_FAR *));
EXTERN int (INTERNAL_FIXED *startTTS)  (P0);
EXTERN int (INTERNAL_FIXED *endTTS)    (P0);
EXTERN int (INTERNAL_FIXED *abortTTS)  (P0);
EXTERN int (INTERNAL_FIXED *setTTS)    (P1(char DB_FAR *));
EXTERN int (INTERNAL_FIXED *clearTTS)  (P1(char DB_FAR *));
EXTERN int (INTERNAL_FIXED *tts_errstr)(P1(char DB_FAR *) Pi(int) Pi(int));

#undef EXTERN



/* System functions */

#ifndef NO_STD_H

#include <stdlib.h>
#include <string.h>

#include <io.h>

#ifdef TURBO
#include <alloc.h>
#else
#ifndef ZOR
#include <malloc.h>
#endif
#endif /* TURBO */

#ifndef __WATCOMC__
#ifndef ZOR

#ifdef TURBO
#include <mem.h>
#else
#include <memory.h>
#endif

#endif
#endif /* __WATCOMC__ */

#ifndef ZOR
void CDECL sleep(P1(unsigned));
#endif

#endif  /* NO_STD_H */

#endif  /* PROTO_H */

/* $Revision:   1.89  $ */

/* vpp PROTO.H (Fri Sep 19 11:56:11 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

