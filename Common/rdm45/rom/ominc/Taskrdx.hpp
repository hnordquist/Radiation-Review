//---------------------------------------------------------
//  TASKRDX.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _TASKRDX_HPP_
#define _TASKRDX_HPP_

#include "vpp.hpp"
#include "activate.hpp"
#include "impllock.h"

#if defined(OM_RDS) && defined(SETOOLKIT)
class seParmSend;
class seParmReceive;
#endif

OMCLASS(RDXTask);


class OM_EXPORT RDXTask
{
private:
   short         TaskStatus;
   ROM_ERRORPROC ErrorHandler;

#ifdef OM_RDS
   T_F       bSessionControl;
   RDM_SESS  hSession;
   short     lockmodeinfo;
   Pchar     pczUser;
   Pchar     pczServer;
   Pchar     pczPassword;
#else // OM_RDM
   PDB_TASK  pThisTask;
#endif

   void Clr();
   T_F  OpenTask(CPchar srv, CPchar usr, CPchar pwd);

protected:
   void SetStatus(short s) { TaskStatus = s; }

public:
   RDXTask(CRActivateTask);
#if !defined(OM_COMMONDB)
#ifdef OM_RDS
   RDXTask(RDM_SESS sess);
#else
   RDXTask();
#endif
#endif
   virtual ~RDXTask();

   short GetStatus() const { return TaskStatus; }
   T_F   Okay()      const { return (TaskStatus == S_OKAY)? True : False; }

   ROM_ERRORPROC GetErrorHandler() const            { return ErrorHandler; }
   short         SetErrorHandler(ROM_ERRORPROC err) { return set_dberr(err); }

#if defined(OM_RDS) && defined(SETOOLKIT)
   virtual seParmSend& SendParms(seParmSend& _send);
   virtual seParmReceive& ReceiveParms(seParmReceive& _receive);
#endif

#ifndef OM_COMMONDB
#ifdef OM_RDS
   RDM_SESS  GetSession() const { return hSession; }
#else // OM_RDM
   PDB_TASK  DbTask() const { return pThisTask; }
#endif
#endif // OM_COMMONDB

#if !defined(OM_COMMONDB) && defined(OM_RDS)
   virtual Pchar OMLOADDS GetUser()     const { return pczUser; }
   virtual Pchar OMLOADDS GetServer()   const { return pczServer; }
   virtual Pchar OMLOADDS GetPassword() const { return pczPassword; }

   Pchar SetUser(CPchar u)     { return DupStr(&pczUser, u); }
   Pchar SetServer(CPchar s)   { return DupStr(&pczServer, s); }
   Pchar SetPassword(CPchar p) { return DupStr(&pczPassword, p); }
#endif

   // functions for enhancing concurrency with RDS 
#if !defined(OM_COMMONDB) && defined(OM_RDS)
   short LockModeInfo() { return lockmodeinfo; }
   // ability to read info without read locks
   void  EnableDirtyReads()  { lockmodeinfo |= OMF_DIRTY_READS; rdlockmodes(); }
   void  DisableDirtyReads() { lockmodeinfo &= ~OMF_DIRTY_READS; rdlockmodes(); }
   T_F   DirtyReadsEnabled() const { return (T_F)(lockmodeinfo & OMF_DIRTY_READS); }
   
   // ability to free read locks inside a transaction
   void  EnableFreeableReadLocks()  { lockmodeinfo |= OMF_FREEABLE_READLOCKS; rdlockmodes(); }
   void  DisableFreeableReadLocks() { lockmodeinfo &= ~OMF_FREEABLE_READLOCKS; rdlockmodes(); }
   T_F   FreeableReadLocksEnabled() const { return (T_F)(lockmodeinfo & OMF_FREEABLE_READLOCKS); }
   
   // Implicit Read locks only
   void  EnableImplicitReadLocks()  { lockmodeinfo |= OMF_IMPLICIT_READLOCKS; }
   void  DisableImplicitReadLocks() { lockmodeinfo &= ~OMF_IMPLICIT_READLOCKS;}
   T_F   ImplicitReadLocksEnabled() const { return (T_F)(lockmodeinfo & OMF_IMPLICIT_READLOCKS); }

   // Implicit Read and Write locks
   void  EnableImplicitLocks()  { lockmodeinfo |= (OMF_IMPLICIT_WRITELOCKS | OMF_IMPLICIT_READLOCKS); }
   void  DisableImplicitLocks() { lockmodeinfo &= ~(OMF_IMPLICIT_WRITELOCKS | OMF_IMPLICIT_READLOCKS);}
   T_F   ImplicitLocksEnabled() const { return (T_F)(lockmodeinfo & (OMF_IMPLICIT_WRITELOCKS | OMF_IMPLICIT_READLOCKS)); }

   // Free Implicit (or other) Locks
   //  FreeWriteLocks and FreeAllLocks will only work in certain open modes
   //  refer to your Velocis User's guide for more info
   T_F   FreeAllLocks()   { return T_F(release(RWLOCKS) == S_OKAY); }
   T_F   FreeReadLocks()  { return T_F(release(RDLOCKS) == S_OKAY); }
   T_F   FreeWriteLocks() { return T_F(release(WRLOCKS) == S_OKAY); }
#endif

   // "Startup" functions that are stubbed for cross-compiles
   short checkid(CPchar pc);
   short ctbpath(CPchar pc);
   short dbdpath(CPchar pc);
   short dbfpath(CPchar pc);
   short dblog(CPchar pc)  ;
   short dbnum(CPchar pc)  ;
   short dbtaf(CPchar pc)  ;
   short dbtmp(CPchar pc)  ;
   short dbuserid(CPchar pc);
   short lockcomm(int locktype);
   short lockmgr(CPchar id);
   short mapchar(unsigned char in, unsigned char out, CPchar sort, unsigned char subsort);
   short off_opt(unsigned long o);
   short on_opt(unsigned long o);
   short setfiles(int numfiles);
   short setpages(int pg, int ov);

   // functions available for both RDM and RDS
   short freeall();
   short set_dberr(ROM_ERRORPROC func, Pvoid svcptr = NULL);
   short timeout(short sec);
   short trabort();
   short trbegin(CPchar pc);
   short trend();

#ifndef OM_COMMONDB
   // RDS specific functions
#ifdef OM_RDS
   short errinfo(Pchar errbuf, short length);
   short getchgInfo(CHGINFO OM_FAR *info);
   short grplock(GROUPLOCK OM_FAR *p, unsigned short n);
   short rdlockmodes();
   short rdlockmodes(short dirty_reads, short freeable);
   short release(short freeflag);
   short tractive(Pshort flag);
   short trmark(CPchar mtag);
   short trrollback(CPchar mtag);
#endif // OM_RDS

   // RDM specific functions
#ifdef OM_RDM
   short destroy(CPchar pc)  ;
   short internals(int topic, int id, int elm, Pvoid ptr, unsigned int size);
   short newcache(int shareflag);
#ifndef V321
   short lmstat(CPchar name, Pint buf);
   short rdmini(CPchar name);
#endif // V321
   short recover(CPchar pc);
   short renfile(CPchar pc, FILE_NO fid, CPchar pcc);
   short renclean();
   short setdb(int numdb);
   short sharecache(PDB_TASK task);
#endif // OM_RDM
#endif // OM_COMMONDB
};

#endif // TASKRDX_HPP

