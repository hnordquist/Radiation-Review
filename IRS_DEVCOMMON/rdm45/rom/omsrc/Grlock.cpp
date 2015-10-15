//-----------------------------------------------------
//  GRLOCK.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>

extern "C" {
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
}

#include <grlock.hpp>
#include <storedb.hpp>
#include <vistaobj.hpp>
#include <storeobj.hpp>


//-----------------------------------------------------
// System independent group lock functionality
//-----------------------------------------------------
  GroupLock::
GroupLock(const char c, PStoreObj PO, ...)
   : RDXGroupLock((PO->GetDb())->GetTask())
{
   va_list    ap;
   PStoreObj  arg;
   int        status = S_OKAY;

   SetLockStatus(GR_EMPTY);
   nRetry = 1;

   va_start(ap, PO);
   for (arg = PO; status == S_OKAY && arg != NULL; arg = va_arg(ap, PStoreObj))
      status = Add(c, *arg);
   va_end(ap);
}


  GroupLock::
GroupLock(const char c, RStoreObj O, SET_TYPE set, ...)
   : RDXGroupLock((O.GetDb())->GetTask())
{
   SetLockStatus(GR_EMPTY);
   nRetry = 1;

   Add(c, O);

   SET_TYPE Set = set;
   va_list marker;
   va_start( marker, set );
   while (Set)
   {
      Add(c, SetObj(Set), O);
      Set = va_arg(marker, SET_TYPE);
   }
   va_end(marker);
}


  GroupLock::
~GroupLock()
{
   if (LockStatus == GR_LOCKED)
      Unlock();
}

    void
  GroupLock::
UpdateAfterTransAction()
{
   sUpdateAfterTransAction();
   SetLockStatus(GR_UNLOCKED);
}

    void
  GroupLock::
Reset()
{
   if (GetLockStatus() == GR_LOCKED || GetLockStatus() == GR_LOCKEDINTRANS)
      ROMERROR(S_GLACTIVE);
   else
   {
      SetTask((PStoreTask)NULL);
      LockStatus = GR_EMPTY; 
      nRetry = 1;
      sReset();
   }
}

    int
  GroupLock::
GetAddStatus(CPStoreDb Db)
{
   int res = S_OKAY;

   if (GetLockStatus() == GR_EMPTY)
      SetTask(Db->GetTask());
   else if (Db->GetTask() != GetTask())
   {
      ROMERROR(S_INVSTASK);
      res = S_INVSTASK;
      SetLockStatus(GR_FAULT);
   }

   return res;
}


    int
  GroupLock::
GetRemoveStatus(CPStoreDb Db)
{
   int res = S_OKAY;

   if (GetLockStatus() == GR_EMPTY)
   {
      // return S_NOTFOUND so Remove() won't do anything when empty.
      // using S_NOTFOUND for no real reason other than that it isn't 
      //  S_OKAY and it sort of fits
      res = S_NOTFOUND;
   }
   else if (GetLockStatus() == GR_LOCKED)
   {
      ROMERROR(S_GLACTIVE);
      res = S_GLACTIVE;
   }
   else if (Db->GetTask() != GetTask())
   {
      ROMERROR(S_INVSTASK);
      res = S_INVSTASK;
      SetLockStatus(GR_FAULT);
   }

   return res;
}


    T_F
  GroupLock::
IsInRequest(CRStoreObj O) const
{
   T_F Res = False;

   if (GetLockStatus() != GR_EMPTY)
      Res = sIsInRequest(SingleLockRqst(O));

   return Res;
}


    T_F
  GroupLock::
IsInRequest(CRSetObj S, CPStoreDb Db) const
{
   T_F Res = False;

   if (GetLockStatus() != GR_EMPTY)
      Res = sIsInRequest(SingleLockRqst(S, Db));

   return Res;
}

    T_F
  GroupLock::
IsLocked(CRStoreObj O) const
{
   T_F Res = False;

   if (GetLockStatus() != GR_EMPTY)
      Res = sIsLocked(SingleLockRqst(O));

   return Res;
}


    T_F
  GroupLock::
IsLocked(CRSetObj S, CPStoreDb Db) const
{
   T_F Res = False;

   if (GetLockStatus() != GR_EMPTY)
      Res = sIsLocked(SingleLockRqst(S, Db));

   return Res;
}


    int
  GroupLock::
Add(const char c, CRStoreObj O)
{
   int res;

   if ((res = GetAddStatus(O.GetDb())) == S_OKAY)
   {
      sAdd(SingleLockRqst(c, O));
      SetLockStatus(GR_UNLOCKED);
   }

   return res;
}


    int
  GroupLock::
Add(const char c, CRSetObj S, PStoreDb pDb)
{
   int res;

   if ((res = GetAddStatus(pDb)) == S_OKAY)
   {
      // add lock request for set
      sAdd(SingleLockRqst(c, S, pDb));

      // add lock requests for owner of the set
      sAdd(SingleLockRqst(c, (pDb->st_own_rt(S.Num()) + RECMARK), pDb));

      // add lock requests for members of the set
      MMSetObj mmset(S.Num(), pDb);
      for (short idx = 0; idx < mmset.MemCount(); idx++)
         sAdd(SingleLockRqst(c, mmset[idx]->rectype, pDb));

      SetLockStatus(GR_UNLOCKED);
   }

   return res;
}


    void
  GroupLock::
Remove(CRStoreObj O)
{
   if (GetRemoveStatus(O.GetDb()) == S_OKAY)
      sRemove(SingleLockRqst(O));
}


    void
  GroupLock::
Remove(CRSetObj S, PStoreDb pDb)
{
   if (GetRemoveStatus(pDb) == S_OKAY)
   {
      // remove lock request for set
      sRemove(SingleLockRqst(S, pDb));

      // remove lock request for owner of the set
      sRemove(SingleLockRqst((pDb->st_own_rt(S.Num()) + RECMARK), pDb));

      // with RDS, you have to explicitly lock the member types
      // remove lock requests for members of the set
      MMSetObj mmset(S.Num(), pDb);
      for (short idx = 0; idx < mmset.MemCount(); idx++)
         sRemove(SingleLockRqst(mmset[idx]->rectype, pDb));
   }
}


    int
  GroupLock::
Lock()
{
   int Res;

   if (GetLockStatus() == GR_EMPTY)
      return S_OKAY;
      
   // Can lock only free objects
   if(GetLockStatus() == GR_LOCKED)
   {
      ROMERROR(S_GLACTIVE);
      Res = S_GLACTIVE;
   }
   else
   {
      // Begin of the locking process
      SetLockStatus(GR_ATTEMPT);
      if ((Res = sLock(nRetry)) == S_OKAY)
         SetLockStatus(GetTask()->GetTransact() ? GR_LOCKEDINTRANS : GR_LOCKED);
      else
         SetLockStatus(GR_FAULT);
   }

   return Res;
}


    int
  GroupLock::
Unlock()
{
   int res = S_OKAY;

   if (LockStatus != GR_LOCKED && LockStatus != GR_LOCKEDINTRANS)
   {
      ROMERROR(S_GLNOLOCK);
      res = S_GLNOLOCK;
   }
   else
   {
      if (LockStatus == GR_LOCKEDINTRANS)
      {
         if (GetTask()->GetTransact())
         {
            ROMERROR(S_TRFREE);
            res = S_TRFREE;
         }
         else
            SetLockStatus(GR_UNLOCKED);
      }
      else if ((res = sUnLock()) == S_OKAY)
         SetLockStatus(GR_UNLOCKED);
      else
         SetLockStatus(GR_FAULT);
   }

   return(res);
}


    GrLockState 
  GroupLock::
GetLockStatus() const
{
   if (LockStatus == GR_LOCKEDINTRANS && !GetTask()->GetTransact())
      return GR_UNLOCKED;

   if (LockStatus == GR_LOCKEDINTRANS)
      return GR_LOCKED;

   return LockStatus;
}


    int
  GroupLock::
AddObjects(const char c, PStoreObj PO, ...)
{
   va_list    ap;
   PStoreObj  arg;
   int        status = S_OKAY;

   va_start(ap, PO);
   for (arg = PO; status == S_OKAY && arg != NULL; arg = va_arg(ap, PStoreObj))
      status = Add(c, *arg);
   va_end(ap);
   return (status);
}


    int
  GroupLock::
AddRead(PStoreObj PO, ...)
{
   va_list    ap;
   PStoreObj  arg;
   int        status = S_OKAY;

   va_start(ap, PO);
   for (arg = PO; status == S_OKAY && arg != NULL; arg = va_arg(ap, PStoreObj))
      status = Add('r', *arg);
   va_end(ap);
   return (status);
}


    int
  GroupLock::
AddWrite(PStoreObj PO, ...)
{
   va_list    ap;
   PStoreObj  arg;
   int        status = S_OKAY;

   va_start(ap, PO);
   for (arg = PO; status == S_OKAY && arg != NULL; arg = va_arg(ap, PStoreObj))
      status = Add('w', *arg);
   va_end(ap);
   return (status);
}



