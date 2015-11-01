//-----------------------------------------------------
//  GRLCKRDX.HPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _GRLCKRDX_HPP_
#define _GRLCKRDX_HPP_

#include "transact.hpp"
#include "singlelr.hpp"

OMCLASS(RDXGroupLock)

#ifdef OM_RDM
OMCLASS(DbLock)
OMCLASS(DbLockArray)

class OM_EXPORT DbLock 
{
private: 
   SLRArray    m_LockedArray;
   SLRArray    m_RequestArray;

public:
   RSLRArray LockedArray()  { return m_LockedArray; }
   RSLRArray RequestArray() { return m_RequestArray; }
};

class OM_EXPORT DbLockArray 
{
private:
   PDbLock     pArray;
   short       nSize;

public:
   DbLockArray(short size = 0) : pArray(0L), nSize(0) { Reset(size); }
   virtual ~DbLockArray() { Reset(); }

   T_F Reset(short size = 0);

   short ArraySize() const { return nSize; }
   PDbLock operator [] (short) const;
};
#endif

class OM_EXPORT RDXGroupLock
{
private:
   PStoreTask   pTask;
#ifdef OM_RDS
   SLRArray     LockedArray;
   SLRArray     RequestArray;
#else
   DbLockArray  DbArray;
#endif

protected:
   void sUpdateAfterTransAction();

public:
   RDXGroupLock(CPStoreTask task = 0L);
   virtual ~RDXGroupLock()  {}

   short sLock(short nRetry = 1);
   short sUnLock();
   void  sReset();

   T_F   sAdd(CRSingleLockRqst);
   T_F   sRemove(CRSingleLockRqst);

   T_F   sIsInRequest(CRSingleLockRqst) const;
   T_F   sIsLocked(CRSingleLockRqst) const;

   PStoreTask GetTask() const { return pTask; }
   void       SetTask(CPStoreTask task);
   void       SetTask(CRStoreTask task) { SetTask(&task); }
   short      SetLockModes(short, short);
};


// these are defined for RDM/RDS compatibility
//  they are not defined in RDS and are defined in RDM
#ifdef OM_RDS
#define LMC_USER                 1
#define LMC_INTERNAL             2
#define LMC_GENERAL              3
#define LMC_NETBIOS              4
#define LMC_SPX                  5
#define LMC_TCP                  6
#define LMC_IP                   7
#define LMC_SEMAPHORE            8
#define LMC_VMS                  9
#define LMC_QNX                  10
#define LMC_OPT                  11
#define LMC_OPT_TRUENAME         12
#define LMC_OPT_PORTABLE         13
#define LMC_OPT_DELNAME          14
#endif

#endif


