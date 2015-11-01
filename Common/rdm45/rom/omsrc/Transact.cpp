//-----------------------------------------------------
//  Transact.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <grlock.hpp>
#include <storetsk.hpp>
#include <transact.hpp>


//-----------------------------------------------------
// TransAction Class functions
//-----------------------------------------------------

  TransAction :: 
TransAction(PStoreTask T)
{
   TrName = (Pchar)NULL;
   SetUpTask(T);
}
   
  TransAction :: 
TransAction(CPchar pId, PStoreTask T)
{
   TrName = (Pchar)NULL;
   SetUpTask(T);

   if (pId)
      Begin(pId);
}
    
  TransAction :: 
~TransAction()
{
   if (Status == TrActive)
      End();
   else if (Status == TrError)
      Abort();
}
    
    void 
  TransAction :: 
SetUpTask(PStoreTask T) 
{ 
   pTask = (T ? T : StoreTask::GetCurrTask());
   if (pTask)
      Status = TrCreated;
   else 
   {
      ROMERROR(S_INVSTASK);
      Status = TrNoTask;
   }
}

    void 
  TransAction :: 
UpdateGroupLock(RGroupLock lock) 
{ 
   lock.UpdateAfterTransAction();
}

    TrStatus
  TransAction :: 
Begin(CPchar pId)
{
   if (Status == TrNoTask)
      ROMERROR(S_INVSTASK);
   else if (Status == TrError)
      ROMERROR(S_TRANSERR);
   else if (Status != TrActive)
   {
      TrName = (Pchar)pId;

      if (pTask->IncrTransactRefCount() == 1)
      {
         if (pTask->trbegin(pId) == S_OKAY)
         {
            Status = TrActive;
            pTask->SetTransact(this); // Set active transaction reference
         }
         else
            Status = TrError;
      }
      else
         Status = TrActive;
   }

   return Status;
}

    
    TrStatus
  TransAction :: 
End()
{
   switch(Status)
   {
      case TrActive:
         if (pTask->DecrTransactRefCount() == 0)
         {
            if (!pTask->TransactAbortFlag() && pTask->trend() == S_OKAY)
            {
               pTask->SetTransact((PTransAction)NULL); // Clear active transaction reference
               Status = TrEnded;
               break;
            }
            // else fall thru due to error or TransactAbortFlag
         }
         else
         {
            Status = TrEnded;
            break;
         }
         
      case TrError:
         Abort();
         break;

      case TrNoTask:
         ROMERROR(S_INVSTASK);
         Status = TrError;
         break;

      default:
         break;
   }

   return Status;
}

    
    TrStatus
  TransAction :: 
End(RGroupLock L1)
{
   End();
   // update the GroupLock only if the trend() happened
   if ((Status == TrEnded || Status == TrAborted) && !pTask->TransactRefCount())
      UpdateGroupLock(L1);

   return Status;
}


    TrStatus
  TransAction :: 
Abort()
{
   if (Status == TrNoTask)
      ROMERROR(S_INVSTASK);
   else if (Status == TrActive || Status == TrError)
   {
      if (pTask->DecrTransactRefCount() == 0)
      {
         pTask->trabort();
         pTask->SetTransact((PTransAction)NULL); // Clear active transaction reference
      }
      else
         pTask->SetTransactAbortFlag();

      Status = TrAborted;
   }

   return Status;
}


    TrStatus
  TransAction :: 
Abort(RGroupLock L1)
{
   // update the GroupLock only if the trabort() really happened
   if (Abort() == TrAborted && !pTask->TransactRefCount())
      UpdateGroupLock(L1);

   return Status;
}


    TrStatus
  TransAction :: 
Mark(CPchar pId)
{
#ifdef OM_RDS
   if (Status == TrActive)
   {
      if (pTask->trmark(pId) != S_OKAY)
         Status = TrError;
   }
   else if (Status == TrNoTask)
      ROMERROR(S_INVSTASK);
   else if (Status == TrError)
      ROMERROR(S_TRANSERR);
#endif

   return Status;
}


    TrStatus
  TransAction :: 
Rollback(CPchar pId)
{
#ifdef OM_RDS
   if (Status == TrActive)
   {
      if (pTask->trrollback(pId) != S_OKAY)
         Status = TrError;
      else if (!pId)
         Status = TrAborted;
   }
   else if (Status == TrNoTask)
      ROMERROR(S_INVSTASK);
   else if (Status == TrError)
      ROMERROR(S_TRANSERR);
#endif

   return Status;
}

