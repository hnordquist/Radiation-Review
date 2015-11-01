//-----------------------------------------------------
//  GRLCKRDX.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <grlckrdx.hpp>


  RDXGroupLock::
RDXGroupLock(CPStoreTask task)
{
   SetTask(task);
}

    void 
  RDXGroupLock::
sUpdateAfterTransAction()
{ 
#ifdef OM_RDS
   RequestArray = LockedArray;
   LockedArray.Reset();
#else
   for (short ct = 0; ct < DbArray.ArraySize(); ct++)
   {
      RSLRArray LockedArray = DbArray[ct]->LockedArray();
      RSLRArray RequestArray = DbArray[ct]->RequestArray();

      RequestArray = LockedArray;
      LockedArray.Reset();
   }
#endif
}

    void
  RDXGroupLock::
SetTask(CPStoreTask task)
{
   pTask = (PStoreTask)task;
#ifdef OM_RDM
   if (pTask)
      DbArray.Reset(pTask->GetDbTotNumber());
#endif
}

    T_F
  RDXGroupLock::
sAdd(CRSingleLockRqst lock)
{
   T_F   ret = True;

#ifdef OM_RDM
   short dbn = lock.GetDb()->GetDbn();
   RSLRArray LockedArray = DbArray[dbn]->LockedArray();
   RSLRArray RequestArray = DbArray[dbn]->RequestArray();
#endif

	// check to see if lock request is already locked
   if (LockedArray.Find(lock) == -1)
   {
      // lock request is NOT currently locked
      // check to see if it is a duplicate request
	   if (RequestArray.Find(lock) == -1)
	   {
#ifdef OM_RDM
              // if the lock is not an upgrade request and was locked 
              //  external to the GroupLock, we'll assume control
	      if (lock.IsLocked() && !lock.IsUpgrade())
                 ret = LockedArray.Add(lock);
              else
#endif
                 ret = RequestArray.Add(lock);
	   }
   }

   return ret;
}

    T_F
  RDXGroupLock::
sRemove(CRSingleLockRqst lock)
{
#ifdef OM_RDM
   short dbn = lock.GetDb()->GetDbn();
   RSLRArray RequestArray = DbArray[dbn]->RequestArray();
#endif

   return T_F(RequestArray.Remove(lock));
}

    T_F
  RDXGroupLock::
sIsInRequest(CRSingleLockRqst lock) const
{
#ifdef OM_RDM
   short dbn = lock.GetDb()->GetDbn();
   RSLRArray RequestArray = DbArray[dbn]->RequestArray();
#endif

   return T_F(RequestArray.Find(lock) != -1);
}

    T_F
  RDXGroupLock::
sIsLocked(CRSingleLockRqst lock) const
{
#ifdef OM_RDM
   short dbn = lock.GetDb()->GetDbn();
   RSLRArray LockedArray = DbArray[dbn]->LockedArray();
#endif

   return T_F(LockedArray.Find(lock) != -1);
}

    short
  RDXGroupLock::
sLock(short nRetry)
{
   short ret = S_OKAY;

   if (nRetry <= 0)
      nRetry = 1;

   while (nRetry > 0)
   {
      nRetry--;

#ifdef OM_RDS
      if (RequestArray.Requests())
      {
	      ret = GetTask()->grplock((PGROUPLOCK)RequestArray, RequestArray.Requests());
	      if (ret == S_OKAY)
	      {
	         LockedArray += RequestArray;
	         RequestArray.Reset();
	      }
      }
#else
      for (short ct = 0; ct < DbArray.ArraySize() && ret == S_OKAY; ct++)
      {
         RSLRArray LockedArray = DbArray[ct]->LockedArray();
         RSLRArray RequestArray = DbArray[ct]->RequestArray();

         if (RequestArray.Requests())
         {
            ret = GetTask()->GetDb(ct)->lock(RequestArray.Requests(), (PLOCK_REQUEST)RequestArray);
            if (ret == S_OKAY)
            {
               LockedArray += RequestArray;
               RequestArray.Reset();
            }
         }
      }
#endif
		// keep looping while locks are unavailable
      if (ret != S_UNAVAIL)
         break;
   }

#ifdef OM_RDM
   // if all locks are not granted, the locks that are granted
   // need to be freed
   if (ret != S_OKAY && !GetTask()->GetTransact())
      GetTask()->freeall();
#endif

   return ret;
}

    short
  RDXGroupLock::
sUnLock()
{
   short ret = S_OKAY;
#ifdef OM_RDS
   // ACK: this provides much better performance, but
   //      also makes the RDM and RDS implementations 
   //      behave differently
   ret = GetTask()->freeall();
   RequestArray = LockedArray;
   LockedArray.Reset();
#else
   for (short ct = 0; ct < DbArray.ArraySize(); ct++)
   {
      RSLRArray LockedArray = DbArray[ct]->LockedArray();
      RSLRArray RequestArray = DbArray[ct]->RequestArray();
		PStoreDb  pDb = GetTask()->GetDb(ct);
      REC_TYPE item;

      for (short ct2 = 0; ct2 < LockedArray.Requests(); ct2++)
      {
         item = LockedArray[ct2]->GetLockType();
         if (item >= SETMARK)
            ret = pDb->setfree(item);
         else
            ret = pDb->recfree(item);
      }

      RequestArray = LockedArray;
      LockedArray.Reset();
   }
#endif

   return ret;
}

    void
  RDXGroupLock::
sReset()
{
#ifdef OM_RDS
   LockedArray.Reset();
   RequestArray.Reset();
#else
   for (short ct = 0; ct < DbArray.ArraySize(); ct++)
   {
      DbArray[ct]->LockedArray().Reset();
      DbArray[ct]->RequestArray().Reset();
   }
#endif
}

    short
  RDXGroupLock::
SetLockModes(short i, short j)
{
#ifdef OM_RDS
   return GetTask()->rdlockmodes(i, j);
#else
   return S_OKAY;
#endif
}


#ifdef OM_RDM
    T_F
  DbLockArray::
Reset(short size)
{
   if (pArray)
   {
      delete [] pArray;
      pArray = 0L;
   }

   if (size)
   {
      pArray = new OM_FAR DbLock[size];
      if (!pArray)
      {   
         ROMERROR(S_NOMEMORY);
         return False;
      }
   }

   nSize = size;
   return True;
}

    PDbLock
  DbLockArray::
operator [] (short idx) const
{
   if (idx >= 0 && idx < nSize)
      return (pArray+idx);
   else
      return 0L;
}
#endif

