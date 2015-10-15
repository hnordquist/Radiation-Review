//-----------------------------------------------------
//  SINGLELR.CPP                  
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <singlelr.hpp>

    void 
  SingleLockRqst::
RecRqst(const char t, REC_TYPE r, CPStoreDb Db)
{
   pDb = (PStoreDb)Db;
#ifdef OM_RDS
   ftype = RTLOCK;
   hdb = Db->GethDb();
   rstype = r;
   // for RDM compatability, there is no 'x' lock in RDS
   lmode = (t == 'x' ? 'w' : t); 
#else
   item = r;
   type = t;
#endif
}

    void
  SingleLockRqst::
SetRqst(const char t, SET_TYPE s, CPStoreDb Db)
{
   pDb = (PStoreDb)Db;
#ifdef OM_RDS
   ftype = STLOCK;
   hdb = Db->GethDb();
   rstype = s;
   // for RDM compatability, there is no 'x' lock in RDS
   lmode = (t == 'x' ? 'w' : t); 
#else
   item = s;
   type = t;
#endif
}

#ifdef OM_RDM
    T_F
  SingleLockRqst::
IsLocked() const
{
   char ltype;
   if (GetLockType() >= SETMARK)
      GetDb()->setlstat(GetLockType(), &ltype);
   else
      GetDb()->reclstat(GetLockType(), &ltype);

   return T_F(ltype != 'f');
}

    T_F
  SingleLockRqst::
IsUpgrade() const
{
   char ltype;
   if (GetLockType() >= SETMARK)
      GetDb()->setlstat(GetLockType(), &ltype);
   else
      GetDb()->reclstat(GetLockType(), &ltype);

   return T_F(type > ltype);
}
#endif

    void 
  SingleLockRqst::
Reset()
{
   pDb = 0L;
#ifdef OM_RDS
   ftype = 0;
   hdb = 0;
   rstype = 0;
   lmode = '\0';
#else
   item = 0;
   type = 0;
#endif
}


    T_F
  SingleLockRqst::
operator < (CRSingleLockRqst lock) const
{
#ifdef OM_RDS
   return T_F(hdb == lock.hdb &&
              ftype  == lock.ftype &&
              rstype == lock.rstype && 
              lmode < lock.lmode);
#else
   return T_F(pDb == lock.pDb && 
              item == lock.item && 
              type < lock.type);
#endif
}

    T_F
  SingleLockRqst::
operator <= (CRSingleLockRqst lock) const
{
#ifdef OM_RDS
   return T_F(hdb == lock.hdb &&
              ftype  == lock.ftype &&
              rstype == lock.rstype && 
              lmode <= lock.lmode);
#else
   return T_F(pDb == lock.pDb && 
              item == lock.item && 
              type <= lock.type);
#endif
}

    RSingleLockRqst
  SingleLockRqst::
operator = (CRSingleLockRqst lock)
{
   pDb = lock.pDb;
#ifdef OM_RDS
   ftype = lock.ftype;
   hdb = lock.hdb;
   rstype = lock.rstype;
   lmode = lock.lmode;
#else
   item = lock.item;
   type = lock.type;
#endif
   return *this;
}

  SLRArray::
SLRArray()
{
   pArray = 0L;
   nSize = 0;
   nRequests = 0;
   pPacket = 0L;
}

  SLRArray::
SLRArray(CRSLRArray array)
{
   pArray = 0L;
   nSize = 0;
   nRequests = 0;
   pPacket = 0L;
   *this = array;
}

    void
  SLRArray::
Reset()
{
   if (pArray)
   {
      delete [] pArray;
      pArray = 0L;
      nSize = 0;
      nRequests = 0;
   }
   if (pPacket)
   {
      delete [] pPacket;
      pPacket = 0L;
   }
}

    T_F
  SLRArray::
ExpandArray(short size)
{  
   PSingleLockRqst tmp;
   short           newsize;

   newsize = nSize + size;

   tmp = new OM_FAR SingleLockRqst[newsize];
   if (!tmp)
   {   
      ROMERROR(S_NOMEMORY);
      return False;
   }

   short ct;
   for (ct = 0; ct < nSize; ct++)
      tmp[ct] = pArray[ct];

   for (; ct < newsize; ct++)
      tmp[ct].Reset();

   nSize = newsize;

   if (pArray)
      delete [] pArray;

   pArray = tmp;
   return True;
}

    short
  SLRArray::
Find(CRSingleLockRqst lock) const
{
   short ct;
   for (ct = 0; ct < nRequests; ct++)
   {
      if (lock <= pArray[ct])
         break;
   }
   return (ct < nRequests ? ct : -1);
}

    T_F
  SLRArray::
IsUpgrade(CRSingleLockRqst lock) const
{
   short ct;
   for (ct = 0; ct < nRequests; ct++)
   {
      if (pArray[ct] < lock)
         break;
   }
   return T_F(ct < nRequests);
}

    T_F
  SLRArray::
Add(CRSingleLockRqst lock)
{
   T_F   ret = True;
   char  lmode = lock.GetLockMode();

   if (lmode != 'k' && IsUpgrade(lock))
      ret = Upgrade(lock);
   else
      ret = QuickAdd(lock);

   return ret;
}

    T_F
  SLRArray::
QuickAdd(CRSingleLockRqst lock)
{
   if (nSize == nRequests)
   {
      if (!ExpandArray())
         return False;
   }

   pArray[nRequests++] = lock;
   return True;
}

    T_F
  SLRArray::
Remove(CRSingleLockRqst lock)
{
   short idx = -1;

   if ((idx = Find(lock)) == -1)
      return False;

   // if the lock request is found, shuffle all 
   // requests after it down one
   nRequests--;
   for (short ct = idx; ct < nRequests; ct++)
      pArray[ct] = pArray[ct+1];

   return True;
}

    T_F
  SLRArray::
Upgrade(CRSingleLockRqst lock)
{
   short ct;
   for (ct = 0; ct < nRequests; ct++)
   {
      if (pArray[ct] < lock)
         break;
   }

	if (ct < nRequests)
	{
   	pArray[ct].SetLockMode(lock.GetLockMode());
	   return True;
	}

   return False;
}

    RSLRArray
  SLRArray::
operator = (CRSLRArray _array)
{
   Reset();
   *this += _array;
   return *this;
}

    RSLRArray
  SLRArray::
operator += (CRSLRArray _array)
{
   for (short ct = 0; ct < _array.Requests(); ct++)
      Add(*(_array[ct]));
   return *this;
}

#ifdef OM_RDS
SLRArray::operator PGROUPLOCK()
#else
SLRArray::operator PLOCK_REQUEST()
#endif
{
   if (!nRequests)
      return 0L;

   if (pPacket)
      delete [] pPacket;

#ifdef OM_RDS
   pPacket = new OM_FAR GROUPLOCK[nRequests];
#else
   pPacket = new OM_FAR LOCK_REQUEST[nRequests];
#endif
   if (!pPacket)
   {   
      ROMERROR(S_NOMEMORY);
      return 0L;
   }

   for (short ct = 0; ct < nRequests; ct++)
   {
#ifdef OM_RDS
      om_memcpy(&pPacket[ct], (PGROUPLOCK)&pArray[ct], sizeof(GROUPLOCK));
#else
      om_memcpy(&pPacket[ct], (PLOCK_REQUEST)&pArray[ct], sizeof(LOCK_REQUEST));
#endif
   }

   return pPacket;
}

    PSingleLockRqst
  SLRArray::
operator [] (short idx) const
{
   if (idx >= 0 && idx < nSize)
      return (pArray+idx);
   else
      return 0L;
}
