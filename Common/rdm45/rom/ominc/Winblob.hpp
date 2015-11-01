//---------------------------------------------------------
//  WINBLOB.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _WINBLOB_HPP_
#define _WINBLOB_HPP_

#include "omblob.hpp"
#ifdef WINDOWS

typedef HANDLE  OMHANDLE;
typedef HWND    OMHWND;


// WinGlobMem manages Windows Global memory and an external
//  void pointer which points to the memory when its locked

class OM_EXPORT WinGlobMem
{
protected:
   T_F            LockStatus;   // Bool for Memory locking
   GLOBALHANDLE   GH;           // Handle to global memory for Buffer
   Pvoid OM_FAR   *ppV;
   
public:
   WinGlobMem();
   WinGlobMem(Pvoid OM_FAR *ppv);
   virtual ~WinGlobMem() { DeallocateGlobMem(); }
   
   void  SetGlobMemPtr(Pvoid OM_FAR *ppv)  { ppV = ppv; }
        
   T_F   LockGlobMem();
   void  UnLockGlobMem();
   
   T_F   IsGlobMemLocked() const { return LockStatus; }
   T_F   HandleIsSet()     const { return (GH) ? True: False; }
   
   T_F   AllocateGlobMem(long len);
   void  DeallocateGlobMem();
};


class OM_EXPORT WinBlob : public OmBlob, public WinGlobMem
{
protected:
   OMHWND  hwind;

   virtual void  DeallocateBuffer();
   virtual void  AfterRead() { LockGlobMem(); }

   virtual OMHANDLE CopyToHandle() = 0;
   virtual T_F      LoadFromClip(OMHANDLE hdata) = 0;

   WinBlob(OMHWND h, SET_TYPE set, PStoreDb pdb = 0);
   WinBlob(OMHWND h, SET_TYPE set, PStoreObj owner, PStoreDb pdb = 0);

public:
   virtual ~WinBlob() { DeallocateBuffer(); }

   virtual T_F ReadyBuffer(long len);

   OMHWND  GetHwnd() const    { return hwind; }
   void    SetHwnd(OMHWND h)  { hwind = h;    }

   T_F   GetClipBoard();
   T_F   PutClipBoard();

   virtual UINT ClipFormat() = 0;

   virtual T_F  Write(PStoreObj owner = (PStoreObj)NULL);

   // old name -- should use Write() now
   virtual void Save(RStoreObj Owner);
};

#endif // WINDOWS
#endif // WINBLOB_HPP
