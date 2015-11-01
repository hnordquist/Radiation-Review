//-----------------------------------------------------
//  OMMEM.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <ommem.hpp>

extern "C" {
#include <malloc.h>
#include <memory.h>
}


  OmMem ::
OmMem()
{
   pBuf = (Pvoid)0;
   LockCnt = 0;
   size = 0;
}


  OmMem ::
OmMem(long len)
{
   LockCnt = 0;
   size = 0;
   pBuf = (Pvoid)0;

   AllocateMem(len);
}


    T_F
  OmMem ::
AllocateMem(long len)
{
   pBuf = (Pvoid) om_calloc((size_t)len, 1);
   if (!pBuf)
   {
      ROMERROR(S_NOMEMORY);
      return False;
   }

   size = len;
   return True;
}


    T_F
  OmMem ::
ReAllocateMem(long len)
{
   T_F res = False;

   if (size < len)
   {
      if (pBuf && !LockCnt)
         DeallocateMem();

      Pvoid tmp = (Pvoid) om_calloc((size_t)len, 1);
      if (!tmp)
         ROMERROR(S_NOMEMORY);
      else
      {
         if (LockCnt)
            om_memcpy(tmp, pBuf, (size_t)size);
         om_free(pBuf);
         pBuf = tmp;
         size = len;
         res = True;
      }
   } 
   else
       res = True;

   return res;
}


    void
  OmMem ::
DeallocateMem()
{  
   if (pBuf)
   {
      if (!LockCnt)
      {
         om_free(pBuf);
         pBuf = (Pvoid)0l;
         size = 0l;
      }
      else 
         ROMERROR(S_NOFREE);
   }
}




