//-----------------------------------------------------
//  GRLOCK.HPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _GRLOCK_HPP_
#define _GRLOCK_HPP_

#include "vpp.hpp"
#include "storetsk.hpp"
#include "grlckrdx.hpp"
#include "omblob.hpp"
#include "poly.hpp"

OMCLASS(StoreDb);
OMCLASS(StoreTask);
OMCLASS(StoreObj);
OMCLASS(GroupLock);
OMCLASS(SetObj);
OMCLASS(OmBlob);


// used as last param to variable arg funcs
#define SETNULL 0

typedef enum{ GR_EMPTY, GR_ATTEMPT, GR_LOCKED, GR_LOCKEDINTRANS,\
              GR_UNLOCKED, GR_FAULT } GrLockState;

class OM_EXPORT GroupLock : public RDXGroupLock
{
   friend void TransAction::UpdateGroupLock(RGroupLock);
   void UpdateAfterTransAction();

private:
   // private set locking functions
   int  Add(const char, CRSetObj, PStoreDb);
   void Remove(CRSetObj, PStoreDb);
   T_F  IsInRequest(CRSetObj, CPStoreDb) const;
   T_F  IsLocked(CRSetObj, CPStoreDb) const;

   int  GetAddStatus(CPStoreDb);
   int  GetRemoveStatus(CPStoreDb);
   T_F  IsLockEmpty() const;
   T_F  IsLockActive() const;
   T_F  IsMatchingTask() const;

protected:
   GrLockState LockStatus;  // Lock status
   int         nRetry;      // Number of lock request retries
   void        SetLockStatus(GrLockState st) { LockStatus = st; }

public:
   GroupLock() : RDXGroupLock((PStoreTask)NULL), LockStatus(GR_EMPTY), nRetry(1) {}
   GroupLock(const char, PStoreObj, ...);
   GroupLock(const char, RStoreObj, SET_TYPE, ...);
   GroupLock(const char, CRSetObj S, RStoreObj O);
   ~GroupLock();

   // Get/Set lock retry count
   int  GetRetry() const { return nRetry; }
   void SetRetry(int n)  { nRetry = n; }

   // Lock/Unlock this GroupLock
   int  Lock();
   int  Unlock();
   void Reset();

   // Get/Set the status of this GroupLock
   GrLockState GetLockStatus() const;

   // Add lists of StoreObjs
   int  AddObjects(const char T, PStoreObj O, ...);
   int  AddRead(PStoreObj O, ...);
   int  AddWrite(PStoreObj O, ...);

   // Add single StoreObjs
   int  Add(const char LType, CRStoreObj);
   int  AddRead(CRStoreObj O)             { return Add('r', O); }
   int  AddWrite(CRStoreObj O)            { return Add('w', O); }
   void Remove(CRStoreObj O);
   T_F  IsInRequest(CRStoreObj) const;
   T_F  IsLocked(CRStoreObj O) const;

   // Add sets (RStoreObj is used ONLY for getting the db this set is in).
   // Both owner and all member record types will be locked.
   int  Add(const char Ltype, CRSetObj S, CRStoreObj O) { return Add(Ltype, S, O.GetDb()); }
   int  AddRead(CRSetObj S, CRStoreObj O)               { return Add('r', S, O.GetDb()); }
   int  AddWrite(CRSetObj S, CRStoreObj O)              { return Add('w', S, O.GetDb()); }
   void Remove(CRSetObj S, CRStoreObj O)                { Remove(S, O.GetDb()); }
   T_F  IsInRequest(CRSetObj S, CRStoreObj O) const     { return IsInRequest(S, O.GetDb()); }
   T_F  IsLocked(CRSetObj S, CRStoreObj O) const        { return IsLocked(S, O.GetDb()); }

   // Locking for Blobs locks the set and all member types
   int  Add(const char L, CROmBlob B)     { return Add(L, (RSetObj)SetObj(B.GetSet()), B.GetDb()); }
   int  AddRead(CROmBlob B)               { return Add('r', B); }
   int  AddWrite(CROmBlob B)              { return Add('w', B); }
   void Remove(CROmBlob B)                { Remove(SetObj(B.GetSet()), B.GetDb()); }
   T_F  IsInRequest(CROmBlob B) const     { return IsInRequest(SetObj(B.GetSet()), B.GetDb()); }
   T_F  IsLocked(CROmBlob B) const        { return IsLocked(SetObj(B.GetSet()), B.GetDb()); }

   // Locking for Polymorph locks the set and all member types
   int  Add(const char L, CRPolymorph P)  { return Add(L, SetObj(((RPolymorph)P).GetSet()), P.GetDb()); }
   int  AddRead(CRPolymorph P)            { return Add('r', P); }
   int  AddWrite(CRPolymorph P)           { return Add('w', P); }
   void Remove(CRPolymorph P)             { Remove(SetObj(((RPolymorph)P).GetSet()), P.GetDb()); }
   T_F  IsInRequest(CRPolymorph P) const  { return IsInRequest(SetObj(((RPolymorph)P).GetSet()), P.GetDb()); }
   T_F  IsLocked(CRPolymorph P) const     { return IsLocked(SetObj(((RPolymorph)P).GetSet()), P.GetDb()); }
};

#endif
