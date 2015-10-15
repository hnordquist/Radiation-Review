//---------------------------------------------------------
//  OMMEM.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _OMMEM_HPP_
#define _OMMEM_HPP_

#include "vpp.hpp"


class OM_EXPORT OmMem
{
private:
   Pvoid  pBuf;
   int    LockCnt;
   long   size;

public:
   OmMem();
   OmMem(long);
   virtual ~OmMem() { UnLockMem(); DeallocateMem(); }

   long   GetSize()    const { return size; }
   Pvoid  GetAddress() const { return pBuf; }

   virtual T_F  AllocateMem(long len);
   virtual T_F  ReAllocateMem(long len);
   virtual void DeallocateMem();

   virtual Pvoid GetPtr() const { return pBuf; }

   virtual T_F  LockMem()   { LockCnt++; return True; }
   virtual void UnLockMem() { if (LockCnt) LockCnt--; }
};

#endif // OMMEM
