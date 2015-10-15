//-----------------------------------------------------
//  DBRDX.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <dbrdx.hpp>
#include <storetsk.hpp>

#ifdef OM_RDS
#  ifdef SETOOLKIT
#    include <parmsend.hpp>
#    include <parmrecv.hpp>
#  endif
#  define DT_(fnc) ::d_##fnc
#  define DB_ARGS hDb
   extern "C" { short REXTERNAL c_rt_data(short record, RDM_DB hDb); };
#else // OM_RDM
#  define DT_(fnc) ::dt_##fnc
#  define DB_ARGS pDbTask,Dbn
#endif


  RDXDb ::
~RDXDb()
{ 
#ifdef OM_RDS
   freedict();
#endif
}

    void
  RDXDb ::
Init(CPStoreTask pTask)
{ 
#ifdef OM_RDS
   hSession = pTask->GetSession();
#else
   pDbTask = pTask->DbTask();
#endif
}


// GetSetEntry, GetRecEntry and GetFldEntry arguments use
// SET, RECORD and FIELD constants as arguments.  Therefore these functions
// calculate the offset into the runtime tables using the database number.
// GetMembEntry, however takes an offset (usually found in the set table)
// and does not add the database member offset to the value

    void
  RDXDb ::
GetSetEntry(PSET_ENTRY pSE, SET_TYPE set)
{
#ifdef OM_RDS
   pSE->st_memtot  = st_memtot(set);
   pSE->st_members = st_members(set); 
   pSE->st_order   = st_order(set); 
   pSE->st_own_rt  = st_own_rt(set);
#else // OM_RDM
   DB_ENTRY Dbe;
   GetDbEntry((Pvoid)&Dbe);
   internals(TOPIC_SET_TABLE, 0, Dbe.st_offset + (set - SETMARK), pSE, sizeof(SET_ENTRY));
#endif
}


    void
  RDXDb ::
GetRecEntry(PRECORD_ENTRY pRE, REC_TYPE recnum)
{
#ifdef OM_RDS
   pRE->rt_len    = rec_len(recnum);
   pRE->rt_data   = rt_data(recnum);
   pRE->rt_fields = rt_fields(recnum);
   pRE->rt_fdtot  = rt_fdtot(recnum);
#else // OM_RDM
   DB_ENTRY Dbe;
   GetDbEntry((Pvoid)&Dbe);
   internals(TOPIC_RECORD_TABLE, 0, Dbe.rt_offset + (recnum - RECMARK), pRE, sizeof(RECORD_ENTRY));
#endif
}


    void
  RDXDb ::
GetFldEntry(PFIELD_ENTRY pFE, FLD_TYPE fldnum)
{
#ifdef OM_RDS
   pFE->fd_flags   = fd_flags((short)fldnum);
   pFE->fd_len     = fd_len((short)fldnum);
   pFE->fd_keyfile = fd_keyfile((short)fldnum); 
   pFE->fd_rec     = fd_rec((short)fldnum);
   pFE->fd_type    = fd_type((short)fldnum);
   pFE->fd_ptr     = field_offset((short)fldnum);
#else // OM_RDM
   RECORD_ENTRY_S RE;
   DB_ENTRY Dbe;
   GetDbEntry((Pvoid)&Dbe);
   internals(TOPIC_RECORD_TABLE, 0, (short)(Dbe.rt_offset + (fldnum / FLDMARK)), (Pvoid)&RE, sizeof(RECORD_ENTRY));
   int fldix = (int)(fldnum % FLDMARK) + (int)RE.rt_fields;
   internals(TOPIC_FIELD_TABLE, 0, (int)Dbe.fd_offset + fldix, pFE, sizeof(FIELD_ENTRY));
#endif
}


    void
  RDXDb ::
GetMembEntry(PMEMBER_ENTRY pME, INDX_TYPE memberindex)
{
#ifdef OM_RDS
   pME->mt_record   = mt_record(memberindex);
   pME->mt_sort_fld = mt_sort_fld(memberindex);
   pME->mt_totsf    = mt_totsf(memberindex);
#else // OM_RDM
   internals(TOPIC_MEMBER_TABLE, 0, memberindex, pME, sizeof(MEMBER_ENTRY));
#endif
}


    REC_TYPE
  RDXDb ::
GetRecFromIndex(INDX_TYPE index)
{
#ifdef OM_RDS
   // Make record ID number from the index of record entry in records table
   return (REC_TYPE)(index + RECMARK);
#else // OM_RDM
   DB_ENTRY dbe;
   GetDbEntry((Pvoid)&dbe);
   return (index - dbe.rt_offset + RECMARK);
#endif
}


#ifdef OM_RDM
    void
  RDXDb ::
GetDbEntry(PDB_ENTRY pDbe)
{
   internals(TOPIC_DB_TABLE, 0, GetDbn(), pDbe, sizeof(DB_ENTRY));
}
#endif


#if defined(OM_RDS) && defined(SETOOLKIT)
    seParmSend&
  RDXDb::
SendParms(seParmSend& _send)
{
//   return _send << DbStatus << hDb << hSession;
   _send << DbStatus;
   _send << hDb; 
   _send << hSession;
   return _send;
}

    seParmReceive&
  RDXDb::
ReceiveParms(seParmReceive& _receive)
{
   return _receive >> DbStatus >> hDb >> hSession;
}
#endif


   // table locking functions common to both RDM and RDS
   // differenct implementation, but same interface
#ifdef OM_RDS
short RDXDb::recfree(REC_TYPE rec)
{ 
   return (DbStatus = (short)DT_(rtfree)(rec, DB_ARGS)); 
}

short RDXDb::reclock(REC_TYPE rec, CPchar ltype)
{ 
   return (DbStatus = (short)DT_(rtlock)(rec, (Pchar)ltype, DB_ARGS)); 
}

short RDXDb::setfree(SET_TYPE set)
{ 
   return (DbStatus = (short)DT_(stfree)(set, DB_ARGS)); 
}

short RDXDb::setlock(SET_TYPE set, CPchar ltype)
{ 
   return (DbStatus = (short)DT_(stlock)(set, (Pchar)ltype, DB_ARGS)); 
}
#else // OM_RDM
short RDXDb::recfree(REC_TYPE rec)
{ 
   return (DbStatus = (short)DT_(recfree)(rec, DB_ARGS)); 
}

short RDXDb::reclock(REC_TYPE rec, CPchar ltype)
{ 
   return (DbStatus = (short)DT_(reclock)(rec, (Pchar)ltype, DB_ARGS)); 
}

short RDXDb::setfree(SET_TYPE set)
{ 
   return (DbStatus = (short)DT_(setfree)(set, DB_ARGS)); 
}

short RDXDb::setlock(SET_TYPE set, CPchar ltype)
{ 
   return (DbStatus = (short)DT_(setlock)(set, (Pchar)ltype, DB_ARGS)); 
}
#endif


#ifndef OM_COMMONDB
#ifdef OM_RDS
// RDS specific Instance Locking functions
short RDXDb::cmfree(SET_TYPE set)
{ 
   return (DbStatus = (short)DT_(cmfree)(set, DB_ARGS)); 
}

short RDXDb::cmlock(SET_TYPE set, CPchar ltype)
{ 
   return (DbStatus = (short)DT_(cmlock)(set, (Pchar)ltype, DB_ARGS)); 
}

short RDXDb::cofree(SET_TYPE set)
{ 
   return (DbStatus = (short)DT_(cofree)(set, DB_ARGS)); 
}

short RDXDb::colock(SET_TYPE set, CPchar ltype)
{ 
   return (DbStatus = (short)DT_(colock)(set, (Pchar)ltype, DB_ARGS)); 
}

short RDXDb::crfree()
{ 
   return (DbStatus = (short)DT_(crfree)(DB_ARGS)); 
}

short RDXDb::crlock(CPchar ltype)
{ 
   return (DbStatus = (short)DT_(crlock)((Pchar)ltype, DB_ARGS)); 
}

short RDXDb::crslock(SET_TYPE set, CPchar ltype)
{ 
   return (DbStatus = (short)DT_(crslock)(set, (Pchar)ltype, DB_ARGS)); 
}

short RDXDb::csfree(SET_TYPE set)
{ 
   return (DbStatus = (short)DT_(csfree)(set, DB_ARGS)); 
}

short RDXDb::cslock(SET_TYPE set, CPchar ltype)
{ 
   return (DbStatus = (short)DT_(cslock)(set, (Pchar)ltype, DB_ARGS)); 
}

short RDXDb::dbafree(DB_ADDR dba)
{ 
   return (DbStatus = (short)DT_(dbafree)(dba, DB_ARGS)); 
}

short RDXDb::dbalock(DB_ADDR dba, CPchar ltype)
{ 
   return (DbStatus = (short)DT_(dbalock)(dba, (Pchar)ltype, DB_ARGS)); 
}

// RDS specific Table Locking functions
short RDXDb::rtfree(REC_TYPE rec)
{ 
   return (DbStatus = (short)DT_(rtfree)(rec, DB_ARGS)); 
}

short RDXDb::rtlock(REC_TYPE rec, CPchar ltype)
{ 
   return (DbStatus = (short)DT_(rtlock)(rec, (Pchar)ltype, DB_ARGS)); 
}

short RDXDb::stfree(SET_TYPE set)
{ 
   return (DbStatus = (short)DT_(stfree)(set, DB_ARGS)); 
}

short RDXDb::stlock(SET_TYPE set, CPchar ltype)
{ 
   return (DbStatus = (short)DT_(stlock)(set, (Pchar)ltype, DB_ARGS)); 
}
#endif // OM_RDS
#endif // OM_COMMON_DB


// RDM specific lock status "checking" functions
#ifndef OM_COMMONDB
#ifdef OM_RDM
short RDXDb::keylstat(FLD_TYPE f, CPchar pc)
{ 
   return (DbStatus = (short)DT_(keylstat)(f, (Pchar)pc, DB_ARGS));
}

short RDXDb::reclstat(REC_TYPE r, CPchar pc)
{ 
   return (DbStatus = (short)DT_(reclstat)(r, (Pchar)pc, DB_ARGS));
}

short RDXDb::setlstat(SET_TYPE s, CPchar pc)
{ 
   return (DbStatus = (short)DT_(setlstat)(s, (Pchar)pc, DB_ARGS));
}
#endif // OM_RDM
#endif // OM_COMMONDB


// RDM specific key functions
#ifndef OM_COMMONDB
#ifdef OM_RDM
short RDXDb::keybuild()
{ 
   return (DbStatus = (short)DT_(keybuild)(DB_ARGS)); 
}

short RDXDb::keyfree(FLD_TYPE f)
{ 
   return (DbStatus = (short)DT_(keyfree)(f, DB_ARGS)); 
}

short RDXDb::keylock(FLD_TYPE f, CPchar  pc)
{ 
   return (DbStatus = (short)DT_(keylock)(f, (Pchar)pc, DB_ARGS));
}
#endif // OM_RDM
#endif // OM_COMMONDB


// RDS specific key functions
#ifndef OM_COMMONDB
#ifdef OM_RDS
short RDXDb::keydir(FLD_TYPE f, short dir)
{ 
   return (DbStatus = (short)DT_(keydir)(f, dir, DB_ARGS)); 
}

short RDXDb::keyrdstate(FLD_TYPE f, Pvoid pv)
{ 
   return (DbStatus = (short)DT_(keyrdstate)(f, pv, DB_ARGS)); 
}

short RDXDb::keyszstate(FLD_TYPE f, unsigned short OM_FAR *size)
{ 
   return (DbStatus = (short)DT_(keyszstate)(f, size, DB_ARGS)); 
}

short RDXDb::keywrstate(FLD_TYPE f, Pvoid pv)
{ 
   return (DbStatus = (short)DT_(keywrstate)(f, pv, DB_ARGS)); 
}

short RDXDb::pkeyfind(FLD_TYPE f, short flds, unsigned short len, CPvoid pv)
{ 
   return (DbStatus = (short)DT_(pkeyfind)(f, flds, len, pv, DB_ARGS)); 
}

short RDXDb::pkeynext(FLD_TYPE f, short flds, unsigned short len, CPvoid pv)
{ 
   return (DbStatus = (short)DT_(pkeynext)(f, flds, len, pv, DB_ARGS)); 
}

short RDXDb::pkeyprev(FLD_TYPE f, short flds, unsigned short len, CPvoid pv)
{ 
   return (DbStatus = (short)DT_(pkeyprev)(f, flds, len, pv, DB_ARGS)); 
}
#endif // OM_RDS
#endif // OM_COMMONDB


// RDM specific Timestamp usage functions
#ifndef OM_COMMONDB
#ifdef OM_RDM
// Timestamp status checking functions
short RDXDb::cmstat(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(cmstat)(s, DB_ARGS)); 
}

short RDXDb::costat(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(costat)(s, DB_ARGS)); 
}

short RDXDb::crstat()
{ 
   return (DbStatus = (short)DT_(crstat)(DB_ARGS)); 
}

short RDXDb::csstat(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(csstat)(s, DB_ARGS)); 
}

short RDXDb::recstat(DB_ADDR dba, ULONG rts)
{ 
   return (DbStatus = (short)DT_(recstat)(dba, rts, DB_ARGS)); 
}

// Creation Timestamp get functions
short RDXDb::ctscm(SET_TYPE s, PULONG pts)
{ 
   return (DbStatus = (short)DT_(ctscm)(s, pts, DB_ARGS)); 
}

short RDXDb::ctsco(SET_TYPE s, PULONG pts)
{ 
   return (DbStatus = (short)DT_(ctsco)(s, pts, DB_ARGS)); 
}

short RDXDb::ctscr(PULONG pts)
{ 
   return (DbStatus = (short)DT_(ctscr)(pts, DB_ARGS)); 
}
            
// Timestamp get functions
short RDXDb::gtscm(SET_TYPE s, PULONG pts)
{ 
   return (DbStatus = (short)DT_(gtscm)(s, pts, DB_ARGS)); 
}

short RDXDb::gtsco(SET_TYPE s, PULONG pts)
{ 
   return (DbStatus = (short)DT_(gtsco)(s, pts, DB_ARGS)); 
}

short RDXDb::gtscr(PULONG pts)
{ 
   return (DbStatus = (short)DT_(gtscr)(pts , DB_ARGS)); 
}

short RDXDb::gtscs(SET_TYPE s, PULONG pts)
{ 
   return (DbStatus = (short)DT_(gtscs)(s, pts, DB_ARGS)); 
}

// Timestamp set functions
short RDXDb::stscm(SET_TYPE s, ULONG ts)
{ 
   return (DbStatus = (short)DT_(stscm)(s, ts, DB_ARGS)); 
}

short RDXDb::stsco(SET_TYPE s, ULONG ts)
{ 
   return (DbStatus = (short)DT_(stsco)(s, ts, DB_ARGS)); 
}

short RDXDb::stscr(ULONG ts)
{ 
   return (DbStatus = (short)DT_(stscr)(ts, DB_ARGS)); 
}

short RDXDb::stscs(SET_TYPE s, ULONG ts)
{ 
   return (DbStatus = (short)DT_(stscs)(s, ts, DB_ARGS)); 
}

// Update Timestamp get functions
short RDXDb::utscm(SET_TYPE s, PULONG pts)
{ 
   return (DbStatus = (short)DT_(utscm)(s, pts, DB_ARGS)); 
}

short RDXDb::utsco(SET_TYPE s, PULONG pts)
{ 
   return (DbStatus = (short)DT_(utsco)(s, pts, DB_ARGS)); 
}

short RDXDb::utscr(PULONG pts)
{ 
   return (DbStatus = (short)DT_(utscr)(pts, DB_ARGS)); 
}

short RDXDb::utscs(SET_TYPE s, PULONG pts)
{ 
   return (DbStatus = (short)DT_(utscs)(s, pts, DB_ARGS)); 
}
#endif // OM_RDM
#endif // OM_COMMONDB


// RDM specific Record-Lock-Bit support 
#ifndef OM_COMMONDB
#ifdef OM_RDM
short RDXDb::rlbclr()
{ 
   return (DbStatus = (short)DT_(rlbclr)(DB_ARGS)); 
}

short RDXDb::rlbset()
{ 
   return (DbStatus = (short)DT_(rlbset)(DB_ARGS)); 
}

short RDXDb::rlbtst()
{ 
   return (DbStatus = (short)DT_(rlbtst)(DB_ARGS)); 
}
#endif // OM_RDM
#endif // OM_COMMONDB

// RDM specific functions (generic)
#ifndef OM_COMMONDB
#ifdef OM_RDM
#ifndef V321
short RDXDb::fldnum(Pint buf, FLD_TYPE fld)
{ 
   return (DbStatus = (short)DT_(fldnum)(buf, fld, DB_ARGS)); 
}

short RDXDb::recnum(Pint buf, REC_TYPE rec)
{ 
   return (DbStatus = (short)DT_(recnum)(buf, rec, DB_ARGS)); 
}

short RDXDb::setnum(Pint buf, SET_TYPE set)
{ 
   return (DbStatus = (short)DT_(setnum)(buf, set, DB_ARGS)); 
}
#endif // V321
short RDXDb::initialize()
{ 
   return (DbStatus = (short)DT_(initialize)(DB_ARGS)); 
}

short RDXDb::initfile(FILE_NO fid)
{ 
   return (DbStatus = (short)DT_(initfile)(fid, DB_ARGS)); 
}

short RDXDb::internals(int topic, int id, int elm, Pvoid ptr, unsigned int sz)
{ 
   return (DbStatus = (short)DT_(internals)(pDbTask, topic, id, elm, ptr, sz)); 
}

short RDXDb::lock(int cnt, LOCK_REQUEST OM_FAR * plr)
{ 
   return (DbStatus = (short)DT_(lock)(cnt, plr, DB_ARGS)); 
}

// DKR 1: DB_ADDR_P for 16 bit only. Date: may-3-97

#ifdef WINDOWS
short RDXDb::rerdcurr(DB_ADDR_P OM_FAR *buffer)
#else
short RDXDb::rerdcurr(PDB_ADDR OM_FAR *buffer)
#endif // WINDOWS
{ 
   return (DbStatus = (short)DT_(rerdcurr)((PDB_ADDR*)buffer, DB_ARGS)); 
}
#endif // OM_RDM
#endif // OM_COMMONDB


// RDS specific functions (generic)
#ifdef OM_RDS
short RDXDb::dict(short i, short e, Pvoid v)
{  
   return (DbStatus = (short)DT_(dict)(i, e, v, (Pchar)NULL, DB_ARGS)); 
}
#endif // OM_RDS

short RDXDb::cmtype(SET_TYPE s, PREC_TYPE pr)
{ 
   return (DbStatus = (short)DT_(cmtype)(s, pr , DB_ARGS)); 
}

short RDXDb::connect(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(connect)(s, DB_ARGS)); 
}

short RDXDb::cotype(SET_TYPE s, PREC_TYPE  pr)
{ 
   return (DbStatus = (short)DT_(cotype)(s, pr, DB_ARGS)); 
}

short RDXDb::crget(PDB_ADDR pdba)
{ 
   return (DbStatus = (short)DT_(crget)(pdba, DB_ARGS)); 
}

short RDXDb::crread(FLD_TYPE f, Pvoid pv)
{ 
   return (DbStatus = (short)DT_(crread)(f, pv, DB_ARGS)); 
}

short RDXDb::crset(PDB_ADDR pdba)
{ 
   return (DbStatus = (short)DT_(crset)(pdba, DB_ARGS)); 
}

short RDXDb::crtype(PREC_TYPE pt)
{ 
   return (DbStatus = (short)DT_(crtype)(pt, DB_ARGS)); 
}

short RDXDb::crwrite(FLD_TYPE f, Pvoid  pv)
{ 
   return (DbStatus = (short)DT_(crwrite)(f, pv, DB_ARGS)); 
}

short RDXDb::csmget(SET_TYPE s, PDB_ADDR  pdba)
{ 
   return (DbStatus = (short)DT_(csmget)(s, pdba, DB_ARGS)); 
}

short RDXDb::csmread(SET_TYPE s, FLD_TYPE f, Pvoid  pv)
{ 
   return (DbStatus = (short)DT_(csmread)(s, f, pv, DB_ARGS)); 
}

short RDXDb::csmset(SET_TYPE s, PDB_ADDR  pdba)
{ 
   return (DbStatus = (short)DT_(csmset)(s, pdba, DB_ARGS)); 
}

short RDXDb::csmwrite(SET_TYPE s, FLD_TYPE f, Pvoid  pv)
{ 
   return (DbStatus = (short)DT_(csmwrite)(s, f, pv, DB_ARGS)); 
}

short RDXDb::csoget(SET_TYPE s, PDB_ADDR  pdba)
{ 
   return (DbStatus = (short)DT_(csoget)(s, pdba, DB_ARGS)); 
}

short RDXDb::csoread(SET_TYPE s, FLD_TYPE f, Pvoid  pv)
{ 
   return (DbStatus = (short)DT_(csoread)(s, f, pv, DB_ARGS)); 
}

short RDXDb::csoset(SET_TYPE s, PDB_ADDR  pdba)
{ 
   return (DbStatus = (short)DT_(csoset)(s, pdba, DB_ARGS)); 
}

short RDXDb::csowrite(SET_TYPE s, FLD_TYPE f, Pvoid  pv)
{ 
   return (DbStatus = (short)DT_(csowrite)(s, f, pv, DB_ARGS)); 
}

short RDXDb::curkey()
{ 
   return (DbStatus = (short)DT_(curkey)(DB_ARGS)); 
}

short RDXDb::d_delete()
{ 
   return (DbStatus = (short)DT_(delete)(DB_ARGS)); 
}

short RDXDb::discon(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(discon)(s, DB_ARGS)); 
}

short RDXDb::disdel()
{ 
   return (DbStatus = (short)DT_(disdel)(DB_ARGS)); 
}

short RDXDb::fillnew(REC_TYPE r, Pvoid  pv)
{ 
   return (DbStatus = (short)DT_(fillnew)(r, pv, DB_ARGS)); 
}

short RDXDb::findco(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(findco)(s, DB_ARGS)); 
}

short RDXDb::findfm(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(findfm)(s, DB_ARGS)); 
}

short RDXDb::findlm(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(findlm)(s , DB_ARGS)); 
}

short RDXDb::findnm(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(findnm)(s , DB_ARGS)); 
}

short RDXDb::findpm(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(findpm)(s, DB_ARGS)); 
}

short RDXDb::ismember(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(ismember)(s, DB_ARGS)); 
}

short RDXDb::isowner(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(isowner)(s, DB_ARGS)); 
}

short RDXDb::keydel(FLD_TYPE f)
{ 
   return (DbStatus = (short)DT_(keydel)(f, DB_ARGS)); 
}

short RDXDb::keyexist(FLD_TYPE f)
{ 
   return (DbStatus = (short)DT_(keyexist)(f, DB_ARGS)); 
}

short RDXDb::keyfind(FLD_TYPE f, Pvoid  pv)
{ 
   return (DbStatus = (short)DT_(keyfind)(f, pv, DB_ARGS)); 
}

short RDXDb::keyfrst(FLD_TYPE f)
{ 
   return (DbStatus = (short)DT_(keyfrst)(f, DB_ARGS)); 
}

short RDXDb::keylast(FLD_TYPE f)
{ 
   return (DbStatus = (short)DT_(keylast)(f, DB_ARGS)); 
}

short RDXDb::keynext(FLD_TYPE f)
{ 
   return (DbStatus = (short)DT_(keynext)(f, DB_ARGS)); 
}

short RDXDb::keyprev(FLD_TYPE f)
{ 
   return (DbStatus = (short)DT_(keyprev)(f, DB_ARGS)); 
}

short RDXDb::keyread(Pvoid  pv, FLD_TYPE f)
{ 
#ifdef OM_RDS
   return (DbStatus = (short)DT_(keyread)(f, pv, hDb)); 
#else
   return (DbStatus = (short)DT_(keyread)(pv, pDbTask)); 
#endif
}

short RDXDb::keystore(FLD_TYPE f)
{ 
   return (DbStatus = (short)DT_(keystore)(f, DB_ARGS)); 
}

short RDXDb::makenew(REC_TYPE r)
{ 
   return (DbStatus = (short)DT_(makenew)(r, DB_ARGS)); 
}

short RDXDb::members(SET_TYPE s, Plong  pl)
{ 
   return (DbStatus = (short)DT_(members)(s, pl, DB_ARGS)); 
}

#ifndef OM_COMMONDB
#ifdef OM_RDS
short RDXDb::rdcurr(PDB_ADDR buffer, short len, Pshort size)
{ 
   return (DbStatus = (short)DT_(rdcurr)(buffer, len, size, DB_ARGS)); 
}
#else // OM_RDM
// DKR 2: DB_ADDR_P for 16 bit only. Date: may-3-97
#ifdef WINDOWS
short RDXDb::rdcurr(DB_ADDR_P OM_FAR *buffer, Pint sz)
#else
short RDXDb::rdcurr(PDB_ADDR OM_FAR *buffer, Pint sz)
#endif // WINDOWS
{ 
   return (DbStatus = (short)DT_(rdcurr)((PDB_ADDR*)buffer, sz, DB_ARGS)); 
}
#endif // OM_RDS
#endif // OM_COMMONDB

short RDXDb::recfrst(REC_TYPE r)
{ 
   return (DbStatus = (short)DT_(recfrst)(r, DB_ARGS)); 
}

short RDXDb::reclast(REC_TYPE r)
{ 
   return (DbStatus = (short)DT_(reclast)(r, DB_ARGS)); 
}

short RDXDb::recnext()
{ 
   return (DbStatus = (short)DT_(recnext)(DB_ARGS)); 
}

short RDXDb::recprev()
{ 
   return (DbStatus = (short)DT_(recprev)(DB_ARGS)); 
}

short RDXDb::recread(Pvoid pv, REC_TYPE rt)
{
#ifdef OM_RDS
   return (DbStatus = (short)DT_(recread)(rt, pv, DB_ARGS));
#else // OM_RDM
   return (DbStatus = (short)DT_(recread)(pv, DB_ARGS));
#endif // OM_RDM
}

short RDXDb::recset(REC_TYPE r)
{ 
   return (DbStatus = (short)DT_(recset)(r, DB_ARGS)); 
}

short RDXDb::recwrite(Pvoid pv, REC_TYPE rt)
{
#ifdef OM_RDS
   return (DbStatus = (short)DT_(recwrite)(rt, pv, DB_ARGS));
#else // OM_RDM
   return (DbStatus = (short)DT_(recwrite)(pv, DB_ARGS));
#endif // OM_RDS
}

short RDXDb::setkey(FLD_TYPE f, Pvoid  pv)
{ 
   return (DbStatus = (short)DT_(setkey)(f, pv, DB_ARGS)); 
}

short RDXDb::setmm(SET_TYPE s1, SET_TYPE s2)
{ 
   return (DbStatus = (short)DT_(setmm)(s1, s2, DB_ARGS)); 
}

short RDXDb::setmo(SET_TYPE s1, SET_TYPE s2)
{ 
   return (DbStatus = (short)DT_(setmo)(s1, s2, DB_ARGS)); 
}

short RDXDb::setmr(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(setmr)(s, DB_ARGS)); 
}

short RDXDb::setom(SET_TYPE s1, SET_TYPE s2)
{ 
   return (DbStatus = (short)DT_(setom)(s1, s2, DB_ARGS)); 
}

short RDXDb::setoo(SET_TYPE s1, SET_TYPE s2)
{ 
   return (DbStatus = (short)DT_(setoo)(s1, s2, DB_ARGS)); 
}

short RDXDb::setor(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(setor)(s, DB_ARGS)); 
}

short RDXDb::setrm(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(setrm)(s, DB_ARGS)); 
}

short RDXDb::setro(SET_TYPE s)
{ 
   return (DbStatus = (short)DT_(setro)(s, DB_ARGS)); 
}

#ifndef OM_COMMONDB
#ifdef OM_RDS
short RDXDb::wrcurr(PDB_ADDR buffer, short len)
{ 
   return (DbStatus = (short)DT_(wrcurr)(buffer, len, DB_ARGS)); 
}
//  DKR 3: DB_ADDR_P for 16 bit only. Date: may-3-97
#else // OM_RDM
#ifdef WINDOWS
short RDXDb::wrcurr(DB_ADDR_P buffer)
#else
short RDXDb::wrcurr(PDB_ADDR buffer)
#endif // WINDOWS
{ 
   return (DbStatus = (short)DT_(wrcurr)(buffer.ptr, DB_ARGS)); 
}
#endif // OM_RDS
#endif // OM_COMMONDB

// Dictionary calls
short RDXDb::fd_dim(short field, short dim_elem)
{
#ifdef OM_RDS
   return ::c_fd_dim(fd_idx(field), dim_elem, hDb); 
#else
   FIELD_ENTRY fe;
   ::dt_internals(pDbTask, TOPIC_FIELD_TABLE, 0, fd_idx(field), &fe, sizeof(FIELD_ENTRY));
   return fe.fd_dim[dim_elem];
#endif
}

short RDXDb::fd_flags(short field)
{
#ifdef OM_RDS
   return ::c_fd_flags(fd_idx(field), hDb); 
#else
   FIELD_ENTRY fe;
   ::dt_internals(pDbTask, TOPIC_FIELD_TABLE, 0, fd_idx(field), &fe, sizeof(FIELD_ENTRY));
   return fe.fd_flags;
#endif
}

short RDXDb::fd_idx(long field)
{
#ifdef OM_RDS
   return ::c_fd_idx(field, hDb); 
#else
   DB_ENTRY Dbe;
   RECORD_ENTRY re;
   GetDbEntry(&Dbe);
   ::dt_internals(pDbTask, TOPIC_RECORD_TABLE, 0, (short)(Dbe.rt_offset + (field / FLDMARK)), &re, sizeof(RECORD_ENTRY));
   return (short)(field % FLDMARK) + (short)re.rt_fields;
#endif
}

short RDXDb::fd_key(short field)
{
#ifdef OM_RDS
   return ::c_fd_key(fd_idx(field), hDb); 
#else
   FIELD_ENTRY fe;
   ::dt_internals(pDbTask, TOPIC_FIELD_TABLE, 0, fd_idx(field), &fe, sizeof(FIELD_ENTRY));
   return fe.fd_key;
#endif
}

short RDXDb::fd_keyfile(short field)
{
#ifdef OM_RDS
   return ::c_fd_keyfile(fd_idx(field), hDb); 
#else
   FIELD_ENTRY fe;
   ::dt_internals(pDbTask, TOPIC_FIELD_TABLE, 0, fd_idx(field), &fe, sizeof(FIELD_ENTRY));
   return fe.fd_keyfile;
#endif
}

short RDXDb::fd_keyno(short field)
{ 
   FIELD_ENTRY fe;
#ifdef OM_RDS
   dict(DICT_FD, field, &fe);
#else
   ::dt_internals(pDbTask, TOPIC_FIELD_TABLE, 0, fd_idx(field), &fe, sizeof(FIELD_ENTRY));
#endif
   return fe.fd_keyno;
}

short RDXDb::fd_len(short field)
{
#ifdef OM_RDS
   return ::c_fd_len(fd_idx(field), hDb); 
#else
   FIELD_ENTRY fe;
   ::dt_internals(pDbTask, TOPIC_FIELD_TABLE, 0, fd_idx(field), &fe, sizeof(FIELD_ENTRY));
   return fe.fd_len;
#endif
}

Pchar RDXDb::fd_name(short field)
{
#ifdef OM_RDS
   return ::c_fd_name(fd_idx(field), hDb); 
#else
//ACK
//   FIELD_ENTRY fe;
//   ::dt_internals(pDbTask, TOPIC_FIELD_TABLE, 0, fd_idx(field), &fe, sizeof(FIELD_ENTRY));
   return "";
#endif
}

short RDXDb::fd_ptr(short field)
{ 
   FIELD_ENTRY fe;
#ifdef OM_RDS
   dict(DICT_FD, field, &fe);
#else
   ::dt_internals(pDbTask, TOPIC_FIELD_TABLE, 0, fd_idx(field), &fe, sizeof(FIELD_ENTRY));
#endif
   return fe.fd_ptr;
}

short RDXDb::fd_rec(short field)
{
#ifdef OM_RDS
   return ::c_fd_rec(fd_idx(field), hDb); 
#else
   FIELD_ENTRY fe;
   ::dt_internals(pDbTask, TOPIC_FIELD_TABLE, 0, fd_idx(field), &fe, sizeof(FIELD_ENTRY));
   return fe.fd_rec;
#endif
}

unsigned char RDXDb::fd_type(short field)
{
#ifdef OM_RDS
   return ::c_fd_type(fd_idx(field), hDb); 
#else
   FIELD_ENTRY fe;
   ::dt_internals(pDbTask, TOPIC_FIELD_TABLE, 0, fd_idx(field), &fe, sizeof(FIELD_ENTRY));
   return fe.fd_type;
#endif
}

short RDXDb::field_offset(short field)
{
#ifdef OM_RDS
   return ::c_field_offset(fd_idx(field), hDb); 
#else
   FIELD_ENTRY fe;
   RECORD_ENTRY re;
   ::dt_internals(pDbTask, TOPIC_FIELD_TABLE, 0, fd_idx(field), &fe, sizeof(FIELD_ENTRY));
   ::dt_internals(pDbTask, TOPIC_RECORD_TABLE, 0, fe.fd_rec, &re, sizeof(RECORD_ENTRY));
   return re.rt_fields + fe.fd_ptr;
#endif
}

short RDXDb::freedict()
{
#ifdef OM_RDS
   return ::c_freedict(hDb); 
#else
   return S_OKAY;
#endif
}

short RDXDb::ft_desc(short element)
{ 
   FILE_ENTRY fe;
#ifdef OM_RDS
   dict(DICT_FT, element, &fe);
#else
   ::dt_internals(pDbTask, TOPIC_FILE_TABLE, 0, element, &fe, sizeof(FILE_ENTRY));
#endif
   return fe.ft_desc;
}

short RDXDb::ft_flags(short element)
{ 
   FILE_ENTRY fe;
#ifdef OM_RDS
   dict(DICT_FT, element, &fe);
#else
   ::dt_internals(pDbTask, TOPIC_FILE_TABLE, 0, element, &fe, sizeof(FILE_ENTRY));
#endif
   return fe.ft_flags;
}

Pchar RDXDb::ft_name(short element)
{
#ifdef OM_RDS
   return ::c_ft_name(element, hDb); 
#else
   FILE_ENTRY fe;
   ::dt_internals(pDbTask, TOPIC_FILE_TABLE, 0, element, &fe, sizeof(FILE_ENTRY));
   return fe.ft_name;
#endif
}

short RDXDb::ft_pgsize(short element)
{ 
   FILE_ENTRY fe;
#ifdef OM_RDS
   dict(DICT_FT, element, &fe);
#else
   ::dt_internals(pDbTask, TOPIC_FILE_TABLE, 0, element, &fe, sizeof(FILE_ENTRY));
#endif
   return fe.ft_pgsize;
}

short RDXDb::ft_slots(short element)
{
#ifdef OM_RDS
   return ::c_ft_slots(element, hDb); 
#else
   FILE_ENTRY fe;
   ::dt_internals(pDbTask, TOPIC_FILE_TABLE, 0, element, &fe, sizeof(FILE_ENTRY));
   return fe.ft_slots;
#endif
}

short RDXDb::ft_slsize(short element)
{
#ifdef OM_RDS
   return ::c_ft_slsize(element, hDb); 
#else
   FILE_ENTRY fe;
   ::dt_internals(pDbTask, TOPIC_FILE_TABLE, 0, element, &fe, sizeof(FILE_ENTRY));
   return fe.ft_slsize;
#endif
}

char RDXDb::ft_status(short element)
{ 
   FILE_ENTRY fe;
#ifdef OM_RDS
   dict(DICT_FT, element, &fe);
#else
   ::dt_internals(pDbTask, TOPIC_FILE_TABLE, 0, element, &fe, sizeof(FILE_ENTRY));
#endif
   return fe.ft_status;
}

char RDXDb::ft_type(short element)
{ 
   FILE_ENTRY fe;
#ifdef OM_RDS
   dict(DICT_FT, element, &fe);
#else
   ::dt_internals(pDbTask, TOPIC_FILE_TABLE, 0, element, &fe, sizeof(FILE_ENTRY));
#endif
   return fe.ft_type;
}

short RDXDb::kt_field(short key)
{
#ifdef OM_RDS
   return ::c_kt_field(key, hDb); 
#else
   KEY_ENTRY ke;
   ::dt_internals(pDbTask, TOPIC_KEY_TABLE, 0, key, &ke, sizeof(KEY_ENTRY));
   return ke.kt_field;
#endif
}

short RDXDb::kt_key(short key)
{
#ifdef OM_RDS
   return ::c_kt_key(key, hDb); 
#else
   KEY_ENTRY ke;
   ::dt_internals(pDbTask, TOPIC_KEY_TABLE, 0, key, &ke, sizeof(KEY_ENTRY));
   return ke.kt_key;
#endif
}

short RDXDb::kt_ptr(short key)
{
#ifdef OM_RDS
   return ::c_kt_ptr(key, hDb); 
#else
   KEY_ENTRY ke;
   ::dt_internals(pDbTask, TOPIC_KEY_TABLE, 0, key, &ke, sizeof(KEY_ENTRY));
   return ke.kt_ptr;
#endif
}

short RDXDb::kt_sort(short key)
{ 
   KEY_ENTRY ke;
#ifdef OM_RDS
   dict(DICT_KT, key, &ke);
#else
   ::dt_internals(pDbTask, TOPIC_KEY_TABLE, 0, key, &ke, sizeof(KEY_ENTRY));
#endif
   return ke.kt_sort;
}

short RDXDb::mt_mem_ptr(short member)
{ 
   MEMBER_ENTRY me;
#ifdef OM_RDS
   dict(DICT_MT, member, &me);
#else
   ::dt_internals(pDbTask, TOPIC_MEMBER_TABLE, 0, member, &me, sizeof(MEMBER_ENTRY));
#endif
   return me.mt_mem_ptr;
}

short RDXDb::mt_record(short member)
{
#ifdef OM_RDS
   return ::c_mt_record(member, hDb); 
#else
   // DKR Confusing (Inconsistent): passing the obsolute index value (member) to be use
   // as an index into the "member table". Better way is to send the Rectype relative to
   // that database  then find out the member record offset from  DB_ENTRY (TOPIC_DB_TABLE)
   MEMBER_ENTRY me;
   ::dt_internals(pDbTask, TOPIC_MEMBER_TABLE, 0, member, &me, sizeof(MEMBER_ENTRY));
   DB_ENTRY Dbe;
   GetDbEntry((Pvoid)&Dbe);
   return me.mt_record-Dbe.rt_offset;
#endif
}

short RDXDb::mt_sort_fld(short member)
{
#ifdef OM_RDS
   return ::c_mt_sort_fld(member, hDb); 
#else
   MEMBER_ENTRY me;
   ::dt_internals(pDbTask, TOPIC_MEMBER_TABLE, 0, member, &me, sizeof(MEMBER_ENTRY));
   return me.mt_sort_fld;
#endif
}

short RDXDb::mt_totsf(short member)
{
#ifdef OM_RDS
   return ::c_mt_totsf(member, hDb); 
#else
   MEMBER_ENTRY me;
   ::dt_internals(pDbTask, TOPIC_MEMBER_TABLE, 0, member, &me, sizeof(MEMBER_ENTRY));
   return me.mt_totsf;
#endif
}

short RDXDb::rec_len(short rec)
{
#ifdef OM_RDS
   return ::c_rec_len(rt_idx(rec), hDb);
#else
   // rt_idx gives (RECTYPE - RECMARK), to this we need to add the st_offset value
   // obtained from the DB_ENTRY with this we get the right offset into the Record 

   RECORD_ENTRY re;
   DB_ENTRY Dbe;
   GetDbEntry((Pvoid)&Dbe);
   ::dt_internals(pDbTask, TOPIC_RECORD_TABLE, 0, rt_idx(rec) + Dbe.rt_offset, &re, sizeof(RECORD_ENTRY));
   return (re.rt_len - re.rt_data);
#endif
}

short RDXDb::rt_data(short rec)
{
#ifdef OM_RDS
   return ::c_rt_data(rt_idx(rec), hDb);
#else
   RECORD_ENTRY re;
   ::dt_internals(pDbTask, TOPIC_RECORD_TABLE, 0, rt_idx(rec), &re, sizeof(RECORD_ENTRY));
   return re.rt_data;
#endif
}

short RDXDb::rt_fdtot(short rec)
{
#ifdef OM_RDS
   return ::c_rt_fdtot(rt_idx(rec), hDb);
#else
   RECORD_ENTRY re;
   ::dt_internals(pDbTask, TOPIC_RECORD_TABLE, 0, rt_idx(rec), &re, sizeof(RECORD_ENTRY));
   return re.rt_fdtot;
#endif
}

short RDXDb::rt_fields(short rec)
{
#ifdef OM_RDS
   return ::c_rt_fields(rt_idx(rec), hDb);
#else
   RECORD_ENTRY re;
   ::dt_internals(pDbTask, TOPIC_RECORD_TABLE, 0, rt_idx(rec), &re, sizeof(RECORD_ENTRY));
   return re.rt_fields;
#endif
}

short RDXDb::rt_file(short rec)
{ 
   RECORD_ENTRY re;
#ifdef OM_RDS
   dict(DICT_RT, rec, &re);
#else
   ::dt_internals(pDbTask, TOPIC_RECORD_TABLE, 0, rt_idx(rec), &re, sizeof(RECORD_ENTRY));
#endif
   return re.rt_file;
}

short RDXDb::rt_flags(short rec)
{ 
   RECORD_ENTRY re;
#ifdef OM_RDS
   dict(DICT_RT, rec, &re);
#else
   ::dt_internals(pDbTask, TOPIC_RECORD_TABLE, 0, rt_idx(rec), &re, sizeof(RECORD_ENTRY));
#endif
   return re.rt_flags;
}

short RDXDb::rt_len(short rec)
{ 
   RECORD_ENTRY re;
#ifdef OM_RDS
   dict(DICT_RT, rec, &re);
#else
   ::dt_internals(pDbTask, TOPIC_RECORD_TABLE, 0, rt_idx(rec), &re, sizeof(RECORD_ENTRY));
#endif
   return re.rt_len;
}

Pchar RDXDb::rt_name(short rec)
{
#ifdef OM_RDS
   return ::c_rt_name(rt_idx(rec), hDb);
#else
//ACK
//   RECORD_ENTRY re;
//   ::dt_internals(pDbTask, TOPIC_RECORD_TABLE, 0, rt_idx(rec), &re, sizeof(RECORD_ENTRY));
   return "";
#endif
}

short RDXDb::se_fld(short srt_number)
{
#ifdef OM_RDS
   return ::c_se_fld(srt_number, hDb); 
#else
   SORT_ENTRY se;
   ::dt_internals(pDbTask, TOPIC_SORT_TABLE, 0, srt_number, &se, sizeof(SORT_ENTRY));
   return se.se_fld;
#endif
}

short RDXDb::se_set(short set)
{ 
   SORT_ENTRY se;
#ifdef OM_RDS
   dict(DICT_SRT, set, &se);
#else
   ::dt_internals(pDbTask, TOPIC_SORT_TABLE, 0, set, &se, sizeof(SORT_ENTRY));
#endif
   return se.se_set;
}

short RDXDb::size_fd()
{ 
#ifdef OM_RDS
   return ::c_size_fd(hDb); 
#else
   short size;
   ::dt_internals(pDbTask, TOPIC_GLOBALS, ID_SIZE_FD, 0, &size, sizeof(short));
   return size;
#endif
}

short RDXDb::size_ft()
{ 
#ifdef OM_RDS
   return ::c_size_ft(hDb); 
#else
   short size;
   ::dt_internals(pDbTask, TOPIC_GLOBALS, ID_SIZE_FT, 0, &size, sizeof(short));
   return size;
#endif
}

short RDXDb::size_kt()
{ 
#ifdef OM_RDS
   return ::c_size_kt(hDb); 
#else
   short size;
   ::dt_internals(pDbTask, TOPIC_GLOBALS, ID_SIZE_KT, 0, &size, sizeof(short));
   return size;
#endif
}

short RDXDb::size_mt()
{ 
#ifdef OM_RDS
   return ::c_size_mt(hDb); 
#else
   short size;
   ::dt_internals(pDbTask, TOPIC_GLOBALS, ID_SIZE_MT, 0, &size, sizeof(short));
   return size;
#endif
}

short RDXDb::size_rt()
{ 
#ifdef OM_RDS
   return ::c_size_rt(hDb); 
#else
   short size;
   ::dt_internals(pDbTask, TOPIC_GLOBALS, ID_SIZE_RT, 0, &size, sizeof(short));
   return size;
#endif
}

short RDXDb::size_srt()
{ 
#ifdef OM_RDS
   return ::c_size_srt(hDb); 
#else
   short size;
   ::dt_internals(pDbTask, TOPIC_GLOBALS, ID_SIZE_SRT, 0, &size, sizeof(short));
   return size;
#endif
}

short RDXDb::size_st()
{ 
#ifdef OM_RDS
   return ::c_size_st(hDb); 
#else
   short size;
   ::dt_internals(pDbTask, TOPIC_GLOBALS, ID_SIZE_ST, 0, &size, sizeof(short));
   return size;
#endif
}

short RDXDb::st_flags(short set)
{ 
   SET_ENTRY se;
#ifdef OM_RDS
   dict(DICT_ST, st_idx(set), &se);
#else
   ::dt_internals(pDbTask, TOPIC_SET_TABLE, 0, st_idx(set), &se, sizeof(SET_ENTRY));
#endif
   return se.st_flags;
}

short RDXDb::st_members(short set)
{
#ifdef OM_RDS
   return ::c_st_members(st_idx(set), hDb); 
#else
   DB_ENTRY Dbe;
   GetDbEntry((Pvoid)&Dbe);
   SET_ENTRY se;
   ::dt_internals(pDbTask, TOPIC_SET_TABLE, 0, st_idx(set) + Dbe.st_offset, &se, sizeof(SET_ENTRY));
   return se.st_members;
#endif
}

short RDXDb::st_memtot(short set)
{
#ifdef OM_RDS
   return ::c_st_memtot(st_idx(set), hDb);
#else
   SET_ENTRY se;
   DB_ENTRY de;
   ::dt_internals(pDbTask, TOPIC_DB_TABLE, 0, GetDbn(), &de, sizeof(DB_ENTRY));
   short db_Offset = de.st_offset;
   ::dt_internals(pDbTask, TOPIC_SET_TABLE, 0, st_idx(set) + de.st_offset, &se, sizeof(SET_ENTRY));
   return se.st_memtot;
#endif
}

Pchar RDXDb::st_name(short set)
{
#ifdef OM_RDS
   return ::c_st_name(st_idx(set), hDb);
#else
//ACK
//   SET_ENTRY se;
//   ::dt_internals(pDbTask, TOPIC_SET_TABLE, 0, st_idx(set), &se, sizeof(SET_ENTRY));
   return "";
#endif
}

short RDXDb::st_order(short set)
{
#ifdef OM_RDS
   return ::c_st_order(st_idx(set), hDb);
#else
   SET_ENTRY se;
   ::dt_internals(pDbTask, TOPIC_SET_TABLE, 0, st_idx(set), &se, sizeof(SET_ENTRY));
   return se.st_order;
#endif
}

short RDXDb::st_own_ptr(short set)
{ 
   SET_ENTRY se;
#ifdef OM_RDS
   dict(DICT_ST, st_idx(set), &se);
#else
   ::dt_internals(pDbTask, TOPIC_SET_TABLE, 0, st_idx(set), &se, sizeof(SET_ENTRY));
#endif
   return se.st_own_ptr;
}

short RDXDb::st_own_rt(short set)
{
#ifdef OM_RDS
   return ::c_st_own_rt(st_idx(set), hDb);
#else
   // DKR: 4 Indexing not working, always looking in the first database entry in the table DB_TABLE. may-3-97
   // FIX: First get the DB_Entry DE from DB_TABLE in order to get the right offset into the
   // table to get the right set/record entry for the given database dbn.
   SET_ENTRY se;
   DB_ENTRY de;
   ::dt_internals(pDbTask, TOPIC_DB_TABLE, 0, GetDbn(), &de, sizeof(DB_ENTRY));
   ::dt_internals(pDbTask, TOPIC_SET_TABLE, 0, st_idx(set) + de.st_offset, &se, sizeof(SET_ENTRY));
   // se.st_own_rt gives you the offset into the RECORD table, de.rt_offset gives you the offset value
   // from which the record entry for this database starts.
   return  se.st_own_rt - de.rt_offset ;
#endif
}
