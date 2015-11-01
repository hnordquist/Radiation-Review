//-----------------------------------------------------
//  SINGLELR.HPP                  
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _SINGLELR_HPP_
#define _SINGLELR_HPP_

#include <storeobj.hpp>

#ifdef OM_RDS
OMTYPE(GROUPLOCK);
#else
OMTYPE(LOCK_REQUEST);
#endif

#define RQ_ALLOC_SZ   20
#define RQ_INC_SZ     10

OMCLASS(RDX_GroupLock);
OMCLASS(SingleLockRqst);
OMCLASS(SLRArray);

OMCLASS(StoreObj)

#ifdef OM_RDM
#ifdef MSC
#pragma pack(1)
#else
#ifdef TURBO
#pragma option -a-
#endif /* TURBO */
#endif /* MSC */
#endif

#ifdef OM_RDS
class OM_EXPORT SingleLockRqst : public GROUPLOCK
#else
class SingleLockRqst : public LOCK_REQUEST
#endif
{
private:
   PStoreDb pDb;
   
   void RecRqst(const char, REC_TYPE, CPStoreDb);
   void SetRqst(const char, SET_TYPE, CPStoreDb);

public:
   SingleLockRqst() { Reset(); }
   SingleLockRqst(CRStoreObj a)                                { RecRqst('r', a.RecType(), a.GetDb()); }
   SingleLockRqst(REC_TYPE r, CPStoreDb Db)                    { RecRqst('r', r, Db); }
   SingleLockRqst(const char t, CRStoreObj a)                  { RecRqst(t, a.RecType(), a.GetDb()); }
   SingleLockRqst(const char t, REC_TYPE r, CPStoreDb Db)      { RecRqst(t, r, Db); }

   SingleLockRqst(CRSetObj s, CRStoreObj o)               { SetRqst('r', s.Num(), o.GetDb()); }
   SingleLockRqst(CRSetObj s, CPStoreDb Db)               { SetRqst('r', s.Num(), Db); }
   SingleLockRqst(const char t, CRSetObj s, CRStoreObj o) { SetRqst(t, s.Num(), o.GetDb()); }
   SingleLockRqst(const char t, CRSetObj s, CPStoreDb Db) { SetRqst(t, s.Num(), Db); }

   virtual ~SingleLockRqst() {}

#ifdef OM_RDS
   char      GetLockMode() const       { return lmode; }
   REC_TYPE  GetLockType() const       { return rstype; }
   void      SetLockMode(const char m) { lmode = m; }
   void      SetLockType(REC_TYPE t)   { rstype = t; }
#else
   char      GetLockMode() const       { return type; }
   REC_TYPE  GetLockType() const       { return item; }
   void      SetLockMode(const char m) { type = m; }
   void      SetLockType(REC_TYPE t)   { item = t; }
   T_F       IsLocked() const;
   T_F       IsUpgrade() const;
#endif

   PStoreDb  GetDb() const { return pDb; }
   void      Reset();

   T_F operator <  (CRSingleLockRqst) const;
   T_F operator <= (CRSingleLockRqst) const;
   RSingleLockRqst operator = (CRSingleLockRqst);
}; 


#ifdef OM_RDM
#ifdef MSC
#pragma pack()
#else
#ifdef TURBO
#pragma option -a.
#endif /* TURBO */
#endif /* MSC */
#endif

class OM_EXPORT SLRArray 
{
private:
   PSingleLockRqst pArray;
   short           nRequests;
   short           nSize;
#ifdef OM_RDS
   PGROUPLOCK      pPacket;
#else
   PLOCK_REQUEST   pPacket;
#endif

   T_F  ExpandArray(short size = 5);
   T_F  QuickAdd(CRSingleLockRqst);

public:
   SLRArray();
   SLRArray(CRSLRArray);
   virtual ~SLRArray() { Reset(); }

   void Reset();

   short ArraySize() const { return nSize; }
   short Requests()  const { return nRequests; }

   T_F   Add(CRSingleLockRqst);
   T_F   Remove(CRSingleLockRqst);
   T_F   Upgrade(CRSingleLockRqst);

   short Find(CRSingleLockRqst) const;
   T_F   IsUpgrade(CRSingleLockRqst) const;

   RSLRArray operator  = (CRSLRArray);
   RSLRArray operator += (CRSLRArray);

   PSingleLockRqst operator[](short) const;

#ifdef OM_RDS
   operator PGROUPLOCK();
#else
   operator PLOCK_REQUEST();
#endif
};

#endif
