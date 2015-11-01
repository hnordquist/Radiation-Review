//---------------------------------------------------------
//  STORETSK.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _STORETSK_HPP_
#define _STORETSK_HPP_

#include "taskrdx.hpp"
#include "storedb.hpp"
#include "vpp.hpp"

OMCLASS(StoreDb);
OMCLASS(StoreTask);
#define VistaTask   StoreTask
#define PVistaTask  PStoreTask
#define RVistaTask  RStoreTask
#define CRVistaTask CRStoreTask
OMCLASS(TransAction);

#define MAXDBS 15

enum DbStat { Active = 0, Inactive };

//---------------------------------------------------------
//  StoreTask 
//---------------------------------------------------------
// Used to define a Task object

class OM_EXPORT StoreTask : public RDXTask
{
public:
   StoreTask(CPchar openmode, CRActivateTask activate);
   StoreTask(CRActivateTask activate, CPchar openmode = "o");
#if defined(OM_RDS) && !defined(OM_COMMONDB)
   StoreTask(RDM_SESS hSess);
#else
#if defined(OM_RDM) && !defined(OM_COMMONDB)
   StoreTask(CPchar openmode = "o");
#endif
#endif
   virtual ~StoreTask();

   PStoreDb  GetDb(short dbn)          const { return pDbs[Active][dbn]; }
   PStoreDb  GetDb(CPchar dbname)      const { return FindDb(dbname); }
   int       GetDbNames(DbStat, Pchar) const;
   int       GetDbTotNumber()          const { return DbCount[0]+DbCount[1]; }

   // TransAction related functions
   PTransAction  GetTransact()         const { return pTransaction; }
   T_F           IsTransactActive()    const { return (pTransaction ? True : False); }

   // Task related functions
   static  PStoreTask  GetCurrTask();
   virtual void        MakeCurrent();

   // RDM setup function (stubbed for RDS in CPP)
   void  SetUserPaths(CPchar path, CPchar userid = (Pchar)NULL);
   void  SetPaths(CPchar dbd = (Pchar)NULL, CPchar dbf = (Pchar)NULL, 
                  CPchar taf = (Pchar)NULL, CPchar log = (Pchar)NULL, 
                  CPchar uid = (Pchar)NULL);
   void  UseEnv(long uniq = 0);

#if defined(OM_RDS) && defined(SETOOLKIT)
   virtual seParmSend& SendParms(seParmSend& _send);
   virtual seParmReceive& ReceiveParms(seParmReceive& _receive);
#endif

protected:
   Pchar GetOpenType() const { return (Pchar)OpenType; }

   static void SetCurrTask(PStoreTask Task);

protected:
   T_F       ActivateAll();                            // Used by Qtask QOpenAll ()
   T_F       DeactivateAll();                          // Used by Qtask QCloseAll()

private:
   T_F       ActivateDb(PStoreDb);                     // move from inactive to active
   T_F       DeactivateDb(PStoreDb);                   // move from active to inactive

   T_F       AddInactive(PStoreDb pDb);          // Add to inactive
   PStoreDb  FindDb(CPchar name) const;           // Find a db by name in list
   int       PosInArray(CPStoreDb, DbStat) const; // Where is in in the array
   T_F       RemoveDb(PStoreDb);                       // Remove from inactive list
   T_F       SetUpDb(CPchar dbname, PStoreDb pDb);
   T_F       Swap(PStoreDb, DbStat, DbStat);           // From One PDb to the other

   void      SetOpenType(CPchar opentype);

   // used by TransAction class
   short     TransactRefCount() const  { return sTransactionRefCount; }
   short     DecrTransactRefCount()    { return --sTransactionRefCount; }
   short     IncrTransactRefCount()    { return ++sTransactionRefCount; }
   void      SetTransact(CPTransAction p) { pTransaction = (PTransAction)p; if (!p) sTransactionRefCount = 0; }
   T_F       TransactAbortFlag() const { return bTransactionAbort; }
   void      SetTransactAbortFlag()    { bTransactionAbort = True; }

private:
   char          OpenType[3];      // Within a Task, all dbs are opened same
   int           DbCount[2];       // active and inactive StoreDbs count
   PStoreDb      pDbs[2][MAXDBS];  // One dim for Active One for Inactive
   PTransAction  pTransaction;     // Active transaction address
   short         sTransactionRefCount;
   T_F           bTransactionAbort;

   static PStoreTask pCurrTask;   // Current Store Task

   friend    TransAction;
   friend    StoreDb;
};


#endif
