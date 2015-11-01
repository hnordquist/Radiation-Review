//-----------------------------------------------------
//  DBRDX.HPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _DBRDX_HPP_
#define _DBRDX_HPP_

#include "vpp.hpp"

#ifdef OM_RDM 
extern "C" {
#include <internal.h>
};
#ifdef V321
#define DBXTRN_H
#define PROTO_H
#include <dbtype.h>
#endif
#ifdef NULL
#undef NULL
#define NULL 0
#endif
#undef NEXT   // undefine these because RDM defines them
#undef LAST
#undef FIRST
OMTYPE(DB_ENTRY);
#endif

OMTYPE(RECORD_ENTRY);
OMTYPE(SET_ENTRY);
OMTYPE(FIELD_ENTRY);
OMTYPE(MEMBER_ENTRY);

OMCLASS(RDXDb);
OMCLASS(StoreTask);

#if defined(OM_RDS) && defined(SETOOLKIT)
class seParmSend;
class seParmReceive;
#endif


class OM_EXPORT RDXDb
{
private:
   short     DbStatus;
#ifdef OM_RDS
   RDM_DB    hDb;              // Handle to database
   RDM_SESS  hSession;         // Session handle
#else
   int       Dbn;              // Database Number, Maintained by StoreTask
   PDB_TASK  pDbTask;          // Ptr to the DB_TASK same as in StoreTask
#endif

protected:
   void      Init(CPStoreTask pTask);
   void      SetDbStatus(short stat) { DbStatus = stat; }

#if !defined(OM_COMMONDB) && defined(OM_RDM)
   void      SetDbn(int newdbn) { Dbn = newdbn; }
#endif

public:
#ifdef OM_RDS
             RDXDb() : hDb(0), DbStatus(S_INVDB) {}
#else
             RDXDb() : Dbn(0), DbStatus(S_INVDB) {}
#endif
   virtual  ~RDXDb();

   short     GetDbStatus() const { return DbStatus; }
   T_F       Okay()        const { return T_F(DbStatus == S_OKAY); }

   void      GetFldEntry(Pvoid pFE, FLD_TYPE fldnum)	   { GetFldEntry((PFIELD_ENTRY)pFE, fldnum); }
   void      GetMembEntry(Pvoid pME, INDX_TYPE membix)   { GetMembEntry((PMEMBER_ENTRY)pME, membix); }
   void      GetRecEntry(Pvoid pRE, REC_TYPE recnum)	   { GetRecEntry((PRECORD_ENTRY)pRE, recnum); }
   void      GetSetEntry(Pvoid pSE, SET_TYPE setnum)	   { GetSetEntry((PSET_ENTRY)pSE, setnum); }

   void      GetFldEntry(PFIELD_ENTRY pFE, FLD_TYPE fldnum);
   void      GetMembEntry(PMEMBER_ENTRY pME, INDX_TYPE membix);
   void      GetRecEntry(PRECORD_ENTRY pRE, REC_TYPE recnum);
   void      GetSetEntry(PSET_ENTRY pSE, SET_TYPE setnum);
   REC_TYPE  GetRecFromIndex(INDX_TYPE);

#ifndef OM_COMMONDB
#ifdef OM_RDS
   RDM_DB    GethDb()     const { return hDb; }
   RDM_SESS  GetSession() const { return hSession; }
   void      SethDb(RDM_DB newdb) { hDb = newdb; }
#else // OM_RDM
   int       GetDbn() const { return Dbn; }
   PDB_TASK  GetDbTask() const { return pDbTask; }
   void      GetDbEntry(Pvoid pDbe)  { GetDbEntry((PDB_ENTRY)pDbe); }
   void      GetDbEntry(PDB_ENTRY pDbe);
#endif
#endif

#if defined(OM_RDS) && defined(SETOOLKIT)
   virtual seParmSend& SendParms(seParmSend& _send);
   virtual seParmReceive& ReceiveParms(seParmReceive& _receive);
#endif

   // table locking functions common to both RDM and RDS
   // differenct implementation, but same interface
#ifdef OM_RDS
   short recfree(REC_TYPE rec);
   short reclock(REC_TYPE rec, CPchar ltype);
   short setfree(SET_TYPE set);
   short setlock(SET_TYPE set, CPchar ltype);
#else // OM_RDM
   short recfree(REC_TYPE rec);
   short reclock(REC_TYPE rec, CPchar ltype);
   short setfree(SET_TYPE set);
   short setlock(SET_TYPE set, CPchar ltype);
#endif

#ifndef OM_COMMONDB
#ifdef OM_RDS
   // RDS specific Instance Locking functions
   short cmfree(SET_TYPE set);
   short cmlock(SET_TYPE set, CPchar ltype);
   short cofree(SET_TYPE set);
   short colock(SET_TYPE set, CPchar ltype);
   short crfree();
   short crlock(CPchar ltype);
   short crslock(SET_TYPE set, CPchar ltype);
   short csfree(SET_TYPE set);
   short cslock(SET_TYPE set, CPchar ltype);
   short dbafree(DB_ADDR dba);
   short dbalock(DB_ADDR dba, CPchar ltype);

   // RDS specific Table Locking functions
   short rtfree(REC_TYPE rec);
   short rtlock(REC_TYPE rec, CPchar ltype);
   short stfree(SET_TYPE set);
   short stlock(SET_TYPE set, CPchar ltype);
#endif // OM_RDS
#endif // OM_COMMON_DB

   // RDM specific lock status "checking" functions
#ifndef OM_COMMONDB
#ifdef OM_RDM
   short keylstat(FLD_TYPE f, CPchar pc);
   short reclstat(REC_TYPE r, CPchar pc);
   short setlstat(SET_TYPE s, CPchar pc);
#endif // OM_RDM
#endif // OM_COMMONDB

   // RDM specific key functions
#ifndef OM_COMMONDB
#ifdef OM_RDM
   short keybuild();
   short keyfree(FLD_TYPE f);
   short keylock(FLD_TYPE f, CPchar  pc);
#endif // OM_RDM
#endif // OM_COMMONDB

   // RDS specific key functions
#ifndef OM_COMMONDB
#ifdef OM_RDS
   short keydir(FLD_TYPE f, short dir);
   short keyrdstate(FLD_TYPE f, Pvoid pv);
   short keyszstate(FLD_TYPE f, unsigned short OM_FAR *size);
   short keywrstate(FLD_TYPE f, Pvoid pv);
   short pkeyfind(FLD_TYPE f, short flds, unsigned short len, CPvoid pv);
   short pkeynext(FLD_TYPE f, short flds, unsigned short len, CPvoid pv);
   short pkeyprev(FLD_TYPE f, short flds, unsigned short len, CPvoid pv);
#endif // OM_RDS
#endif // OM_COMMONDB

   // RDM specific Timestamp usage functions
#ifndef OM_COMMONDB
#ifdef OM_RDM
   // Timestamp status checking functions
   short cmstat(SET_TYPE s);
   short costat(SET_TYPE s);
   short crstat();
   short csstat(SET_TYPE s);
   short recstat(DB_ADDR dba, ULONG rts);
          
   // Creation Timestamp get functions
   short ctscm(SET_TYPE s, PULONG pts);
   short ctsco(SET_TYPE s, PULONG pts);
   short ctscr(PULONG pts);
            
   // Timestamp get functions
   short gtscm(SET_TYPE s, PULONG pts);
   short gtsco(SET_TYPE s, PULONG pts);
   short gtscr(PULONG pts);
   short gtscs(SET_TYPE s, PULONG pts);
            
   // Timestamp set functions
   short stscm(SET_TYPE s, ULONG ts);
   short stsco(SET_TYPE s, ULONG ts);
   short stscr(ULONG ts);
   short stscs(SET_TYPE s, ULONG ts);
            
   // Update Timestamp get functions
   short utscm(SET_TYPE s, PULONG pts);
   short utsco(SET_TYPE s, PULONG pts);
   short utscr(PULONG pts);
   short utscs(SET_TYPE s, PULONG pts);
#endif // OM_RDM
#endif // OM_COMMONDB

   // RDM specific Record-Lock-Bit support 
#ifndef OM_COMMONDB
#ifdef OM_RDM
   short rlbclr();
   short rlbset();
   short rlbtst();
#endif // OM_RDM
#endif // OM_COMMONDB

   // RDM specific functions (generic)
#ifndef OM_COMMONDB
#ifdef OM_RDM
#ifndef V321
   short fldnum(Pint buf, FLD_TYPE fld);
   short recnum(Pint buf, REC_TYPE rec);
   short setnum(Pint buf, SET_TYPE set);
#endif // V321
   short initialize();
   short initfile(FILE_NO fid);
   short internals(int topic, int id, int elm, Pvoid ptr, unsigned int sz);
   short lock(int cnt, LOCK_REQUEST OM_FAR * plr);
#ifdef WINDOWS
   short rerdcurr(DB_ADDR_P OM_FAR *buffer);
#else
   short rerdcurr(PDB_ADDR OM_FAR *buffer);
#endif // WINDOWS
#endif // OM_RDM
#endif // OM_COMMONDB

   // RDS specific functions (generic)
#ifdef OM_RDS
   short dict(short i, short e, Pvoid v);
#endif // OM_RDS

   short cmtype(SET_TYPE s, PREC_TYPE pr);
   short connect(SET_TYPE s);
   short cotype(SET_TYPE s, PREC_TYPE  pr);
   short crget(PDB_ADDR pdba);
   short crread(FLD_TYPE f, Pvoid pv);
   short crset(PDB_ADDR pdba);
   short crtype(PREC_TYPE pt);
   short crwrite(FLD_TYPE f, Pvoid  pv);
   short csmget(SET_TYPE s, PDB_ADDR  pdba);
   short csmread(SET_TYPE s, FLD_TYPE f, Pvoid  pv);
   short csmset(SET_TYPE s, PDB_ADDR  pdba);
   short csmwrite(SET_TYPE s, FLD_TYPE f, Pvoid  pv);
   short csoget(SET_TYPE s, PDB_ADDR  pdba);
   short csoread(SET_TYPE s, FLD_TYPE f, Pvoid  pv);
   short csoset(SET_TYPE s, PDB_ADDR  pdba);
   short csowrite(SET_TYPE s, FLD_TYPE f, Pvoid  pv);
   short curkey();
   short d_delete();
   short discon(SET_TYPE s);
   short disdel();
   short fillnew(REC_TYPE r, Pvoid  pv);
   short findco(SET_TYPE s);
   short findfm(SET_TYPE s);
   short findlm(SET_TYPE s);
   short findnm(SET_TYPE s);
   short findpm(SET_TYPE s);
   short ismember(SET_TYPE s);
   short isowner(SET_TYPE s);
   short keydel(FLD_TYPE f);
   short keyexist(FLD_TYPE f);
   short keyfind(FLD_TYPE f, Pvoid  pv);
   short keyfrst(FLD_TYPE f);
   short keylast(FLD_TYPE f);
   short keynext(FLD_TYPE f);
   short keyprev(FLD_TYPE f);
   short keyread(Pvoid  pv, FLD_TYPE f = 0);
   short keystore(FLD_TYPE f);
   short makenew(REC_TYPE r);
   short members(SET_TYPE s, Plong  pl);

#ifndef OM_COMMONDB
#ifdef OM_RDS
   short rdcurr(PDB_ADDR buffer, short len, Pshort size);
#else // OM_RDM
#ifdef WINDOWS
   short rdcurr(DB_ADDR_P OM_FAR *buffer, Pint sz);
#else
   short rdcurr(PDB_ADDR OM_FAR *buffer, Pint sz);
#endif // WINDOWS
#endif // OM_RDS
#endif // OM_COMMONDB

   short recfrst(REC_TYPE r);
   short reclast(REC_TYPE r);
   short recnext();
   short recprev();
   short recread(Pvoid pv, REC_TYPE rt = 0);
   short recset(REC_TYPE r);
   short recwrite(Pvoid pv, REC_TYPE rt = 0);
   short setkey(FLD_TYPE f, Pvoid  pv);
   short setmm(SET_TYPE s1, SET_TYPE s2);
   short setmo(SET_TYPE s1, SET_TYPE s2);
   short setmr(SET_TYPE s);
   short setom(SET_TYPE s1, SET_TYPE s2);
   short setoo(SET_TYPE s1, SET_TYPE s2);
   short setor(SET_TYPE s);
   short setrm(SET_TYPE s);
   short setro(SET_TYPE s);

#ifndef OM_COMMONDB
#ifdef OM_RDS
   short wrcurr(PDB_ADDR buffer, short len);
#else // OM_RDM
#ifdef WINDOWS
   short wrcurr(DB_ADDR_P buffer);
#else
   short wrcurr(PDB_ADDR buffer);
#endif // WINDOWS
#endif // OM_RDS
#endif // OM_COMMONDB

// Dictionary calls
   short fd_dim(short field, short dim_elem);
   short fd_flags(short field);
   short fd_idx(long field);
   short fd_key(short field);
   short fd_keyfile(short field);
   short fd_keyno(short field);
   short fd_len(short field);
   Pchar fd_name(short field);
   short fd_ptr(short field);
   short fd_rec(short field);
   unsigned char fd_type(short field);
   short field_offset(short field);
   short freedict();
   short ft_desc(short element);
   short ft_flags(short element);
   Pchar ft_name(short element);
   short ft_pgsize(short element);
   short ft_slots(short element);
   short ft_slsize(short element);
   char  ft_status(short element);
   char  ft_type(short element);
   short kt_field(short key);
   short kt_key(short key);
   short kt_ptr(short key);
   short kt_sort(short key);
   short mt_mem_ptr(short member);
   short mt_record(short member);
   short mt_sort_fld(short member);
   short mt_totsf(short member);
   short rec_len(short rec);
   short rt_data(short rec);
   short rt_fdtot(short rec);
   short rt_fields(short rec);
   short rt_file(short rec);
   short rt_flags(short rec);
   short rt_idx(short rec) { return (short)(rec >= RECMARK ? rec-RECMARK : rec); }
   short rt_len(short rec);
   Pchar rt_name(short rec);
   short se_fld(short srt_number);
   short se_set(short set);
   short size_fd();
   short size_ft();
   short size_kt();
   short size_mt();
   short size_rt();
   short size_srt();
   short size_st();
   short st_flags(short set);
   short st_idx(short set) { return (short)(set >= SETMARK ? set-SETMARK : set); }
   short st_members(short set);
   short st_memtot(short set);
   Pchar st_name(short set);
   short st_order(short set);
   short st_own_ptr(short set);
   short st_own_rt(short set);
};

#endif
