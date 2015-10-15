//---------------------------------------------------------
//  TASKRDX.CPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#include <vpp.hpp>
#include <taskrdx.hpp>

#ifdef OM_RDS
#  include <hlfproto.h>
#  ifdef SETOOLKIT
#    include <parmsend.hpp>
#    include <parmrecv.hpp>
#  endif
#  define DT_(fnc) ::d_##fnc
#  define TASK_ARGS hSession
#else // OM_RDM
#  define DT_(fnc) ::dt_##fnc
#  define TASK_ARGS pThisTask
#endif


  RDXTask :: 
RDXTask(CRActivateTask a)
{
   OpenTask(a.GetServer(), a.GetUser(), a.GetPassword());
#ifdef OM_RDS
   pczUser = pczServer = pczPassword = (Pchar)NULL;
   SetServer(a.GetServer());
   SetUser(a.GetUser());
   SetPassword(a.GetPassword());
#endif
}


#ifdef OM_RDM
  RDXTask :: 
RDXTask()
{
   OpenTask((Pchar)NULL, (Pchar)NULL, (Pchar)NULL);
}
#endif


#if defined(OM_RDS) 
  RDXTask :: 
RDXTask(RDM_SESS sess)
{ 
   Clr(); 
   hSession = sess; 
   TaskStatus = S_OKAY; 
   bSessionControl = True;
   pczUser = pczServer = pczPassword = (Pchar)NULL;
}
#endif

    
  RDXTask :: 
~RDXTask()
{
#ifdef OM_RDS
   if (hSession && !bSessionControl)
   {
      hlfCleanup(hSession);
      s_logout(hSession);
   }

   if (pczServer)
      om_free(pczServer);
   if (pczUser)
      om_free(pczUser);
   if (pczPassword)
      om_free(pczPassword);
#else // OM_RDM
   if (pThisTask)
   {
      dt_closetask((PDB_TASK)pThisTask);
      delete (PDB_TASK)pThisTask;
   }
#endif
}


    void 
  RDXTask :: 
Clr()
{
   TaskStatus = S_INVSTASK;
   ErrorHandler = (ROM_ERRORPROC)NULL;
#ifdef OM_RDS
   hSession = 0;
   lockmodeinfo = 0;
   bSessionControl = False;
#else // OM_RDM
   pThisTask = NULL;
#endif
}



    T_F
  RDXTask :: 
OpenTask(CPchar srv, CPchar usr, CPchar pwd)
{
   Clr();

#ifdef OM_RDS
   hSession = 0;
   if ((TaskStatus = s_login((Pchar)srv, (Pchar)usr, (Pchar)pwd, &hSession)) != S_OKAY)
      ROMERROR(S_NOSESS);
   else if ((TaskStatus = hlfInit(hSession)) != S_OKAY)
      ROMERROR(TaskStatus);
#else
   pThisTask = (PDB_TASK)new OM_FAR DB_TASK;
   if (pThisTask)
      TaskStatus = dt_opentask(pThisTask);
   else
   {
      ROMERROR(S_NOMEMORY);
      TaskStatus = S_NOMEMORY;
   }
#endif

   // set default ROM Error Handler
   if (Okay())
#ifdef OM_RDS
      TaskStatus = (short)DT_(set_dberr)((ERRORPROC)ROMErrorHandler, NULL, TASK_ARGS);
#else
      TaskStatus = (short)DT_(set_dberr)((ERRORPROC)ROMErrorHandler, TASK_ARGS);
#endif

   return Okay();
}

#if defined(OM_RDS) && defined(SETOOLKIT)
    seParmSend&
  RDXTask::
SendParms(seParmSend& _send)
{
   _send << hSession
         << lockmodeinfo;
//         << pczUser
//         << pczServer
//         << pczPassword;
   return _send;
}

    seParmReceive&
  RDXTask::
ReceiveParms(seParmReceive& _receive)
{
   bSessionControl = True;
   _receive >> hSession
            >> lockmodeinfo;
//            >> pczUser
//            >> pczServer
//            >> pczPassword;
   return _receive;
}
#endif



// "Startup" functions that are stubbed for cross-compiles
short RDXTask::checkid(CPchar pc)  
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(checkid)((Pchar)pc, TASK_ARGS));
#endif
}

short RDXTask::ctbpath(CPchar pc)  
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(ctbpath)(pc, TASK_ARGS)); 
#endif
}

short RDXTask::dbdpath(CPchar pc)  
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(dbdpath)(pc, TASK_ARGS)); 
#endif
}

short RDXTask::dbfpath(CPchar pc)  
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(dbfpath)(pc, TASK_ARGS)); 
#endif
}

short RDXTask::dblog(CPchar pc)    
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(dblog)(pc, TASK_ARGS)); 
#endif
}

short RDXTask::dbnum(CPchar pc)    
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(dbnum)(pc, TASK_ARGS)); 
#endif
}

short RDXTask::dbtaf(CPchar pc)    
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(dbtaf)(pc, TASK_ARGS)); 
#endif
}

#ifndef V321
short RDXTask::dbtmp(CPchar pc)    
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(dbtmp)(pc, TASK_ARGS)); 
#endif
}
#endif

short RDXTask::dbuserid(CPchar pc) 
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(dbuserid)(pc, TASK_ARGS)); 
#endif
}

short RDXTask::lockcomm(int locktype)   
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(lockcomm)(locktype, TASK_ARGS)); 
#endif
}

short RDXTask::lockmgr(CPchar id)  
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(lockmgr)(id, TASK_ARGS)); 
#endif
}

short RDXTask::mapchar(unsigned char in, unsigned char out, CPchar sort, unsigned char subsort)
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(mapchar)(in, out, sort, subsort, TASK_ARGS)); 
#endif
}

short RDXTask::off_opt(unsigned long o) 
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(off_opt)(o, TASK_ARGS)); 
#endif
}

short RDXTask::on_opt(unsigned long o)  
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(on_opt)(o, TASK_ARGS)); 
#endif
}

short RDXTask::setfiles(int numfiles)   
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(setfiles)(numfiles, TASK_ARGS)); 
#endif
}

short RDXTask::setpages(int pg, int ov) 
{ 
#ifdef OM_RDS
   return S_OKAY;
#else
   return (TaskStatus = (short)DT_(setpages)(pg, ov, TASK_ARGS));
#endif
}

// functions available for both RDM and RDS
short RDXTask::freeall()
{
#ifdef OM_RDS
   return (TaskStatus = (short)DT_(release)(RDLOCKS, TASK_ARGS)); 
#else // OM_RDM
   return (TaskStatus = (short)DT_(freeall)(TASK_ARGS));
#endif
}

short RDXTask::set_dberr(ROM_ERRORPROC func, Pvoid svcptr)
{
   ErrorHandler = func;
   return S_OKAY;
}

short RDXTask::timeout(short sec)
{ 
   return (TaskStatus = (short)DT_(timeout)(sec, TASK_ARGS)); 
}

short RDXTask::trabort()
{ 
   return (TaskStatus = (short)DT_(trabort)(TASK_ARGS)); 
}

short RDXTask::trbegin(CPchar pc)
{ 
   return (TaskStatus = (short)DT_(trbegin)(pc, TASK_ARGS)); 
}

short RDXTask::trend()
{ 
   return (TaskStatus = (short)DT_(trend)(TASK_ARGS)); 
}
// end of functions available for both RDM and RDS

// RDS specific functions
#ifndef OM_COMMONDB
#ifdef OM_RDS
short RDXTask::errinfo(Pchar errbuf, short length)
{ 
   return (TaskStatus = (short)DT_(errinfo)(errbuf, length, TASK_ARGS)); 
}

short RDXTask::getchgInfo(CHGINFO OM_FAR *info)
{ 
   return (TaskStatus = (short)DT_(getchgInfo)(info, TASK_ARGS)); 
}

short RDXTask::grplock(GROUPLOCK OM_FAR *p, unsigned short n)
{ 
   return (TaskStatus = (short)DT_(grplock)(p, n, TASK_ARGS)); 
}

short RDXTask::rdlockmodes()
{ 
   return (TaskStatus = (short)DT_(rdlockmodes)(lockmodeinfo & OMF_DIRTY_READS, lockmodeinfo & OMF_FREEABLE_READLOCKS, TASK_ARGS)); 
}

short RDXTask::rdlockmodes(short dirty_reads, short freeable)
{ 
   dirty_reads ? EnableDirtyReads() : DisableDirtyReads();
   freeable ? EnableFreeableReadLocks() : DisableFreeableReadLocks();
   return (TaskStatus = (short)DT_(rdlockmodes)(dirty_reads, freeable, TASK_ARGS)); 
}

short RDXTask::release(short freeflag)
{ 
   return (TaskStatus = (short)DT_(release)(freeflag, TASK_ARGS)); 
}

short RDXTask::tractive(Pshort flag)
{ 
   return (TaskStatus = (short)DT_(tractive)(flag, TASK_ARGS)); 
}

short RDXTask::trmark(CPchar mtag)
{ 
   return (TaskStatus = (short)DT_(trmark)(mtag, TASK_ARGS)); 
}

short RDXTask::trrollback(CPchar mtag)
{ 
   return (TaskStatus = (short)DT_(trrollback)(mtag, TASK_ARGS)); 
}
#endif // OM_RDS

// RDM specific functions
#ifdef OM_RDM
short RDXTask::destroy(CPchar pc)  
{ 
   return (TaskStatus = (short)DT_(destroy)(pc, TASK_ARGS)); 
}

short RDXTask::internals(int topic, int id, int elm, Pvoid ptr, unsigned int size)
{ 
   return (TaskStatus = (short)DT_(internals)(TASK_ARGS, topic, id, elm, ptr, size)); 
}

#ifndef V321
short RDXTask::lmstat(CPchar name, Pint buf)
{ 
   return (TaskStatus = (short)DT_(lmstat)((Pchar)name, buf, TASK_ARGS));
}
short RDXTask::rdmini(CPchar name)
{ 
   return (TaskStatus = (short)DT_(rdmini)(name, TASK_ARGS)); 
}
#endif // V321

short RDXTask::newcache(int shareflag)
{ 
   return (TaskStatus = (short)DT_(newcache)(TASK_ARGS, shareflag)); 
}

short RDXTask::recover(CPchar pc)
{ 
   return (TaskStatus = (short)DT_(recover)(pc, TASK_ARGS)); 
}

short RDXTask::renfile(CPchar pc, FILE_NO fid, CPchar pcc)
{ 
   return (TaskStatus = (short)DT_(renfile)(pc, fid, pcc, TASK_ARGS)); 
}

short RDXTask::renclean()
{ 
   return (TaskStatus = (short)DT_(renclean)(TASK_ARGS)); 
}

short RDXTask::setdb(int numdb)
{ 
   return (TaskStatus = (short)DT_(setdb)(numdb, TASK_ARGS)); 
}

short RDXTask::sharecache(PDB_TASK task)
{ 
   return (TaskStatus = (short)DT_(sharecache)(TASK_ARGS, task)); 
}
#endif // OM_RDM
#endif // OM_COMMONDB


