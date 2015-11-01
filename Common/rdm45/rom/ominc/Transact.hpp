//-----------------------------------------------------
//  TRANSACT.hpp
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _TRANSACT_HPP_
#define _TRANSACT_HPP_

enum TrStatus { TrActive = 1, TrCreated, TrEnded, TrAborted, TrNoTask, TrError };

OMCLASS(TransAction);
OMCLASS(GroupLock);
OMCLASS(StoreTask);


class OM_EXPORT TransAction
{
private: 
   PStoreTask  pTask;
   TrStatus    Status;
   Pchar       TrName;
   
   void SetUpTask(PStoreTask pTask);
   void UpdateGroupLock(RGroupLock);
   
public:
   TransAction(PStoreTask pTask = 0L);
   TransAction(CPchar tid, PStoreTask pTask = 0L);
   virtual ~TransAction();

   TrStatus  Begin(CPchar tid);
   
   TrStatus  Abort();
   TrStatus  Abort(RGroupLock glock);

   TrStatus  End();
   TrStatus  End(RGroupLock glock);

   // Mark Transaction for Rollback (stubbed for RDM)
   TrStatus  Mark(CPchar tid);
   // Rollback Transaction to last Mark (stubbed for RDM)
   TrStatus  Rollback(CPchar tid);

   void  SetError() { Status = TrError; }
   
   TrStatus GetStatus() const { return Status; }
   Pchar    GetName()   const { return TrName; }
};

#endif
