//-----------------------------------------------------
//  WINBLOB.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#ifdef WINDOWS

#include <storedb.hpp>
#include <storeobj.hpp>
#include <omblob.hpp>
#include <winblob.hpp>

//-----------------------------------------------------
// WinGlobMem functions (may not be necessary in 3.1 windows)
//-----------------------------------------------------

  WinGlobMem ::
WinGlobMem()
{
   SetGlobMemPtr(0);
   LockStatus = False;
   GH = 0;
}

  WinGlobMem ::
WinGlobMem(Pvoid OM_FAR *ppv)
{
   SetGlobMemPtr(ppv);
   LockStatus = False;
   GH = 0;
}

    T_F
  WinGlobMem ::
LockGlobMem()
{
   T_F rtn = True;
   if (!IsGlobMemLocked())        // if its already locked, nothing to do
   {
      if (!HandleIsSet() || !(*ppV = (Pvoid) ::GlobalLock(GH)))
         rtn = False;           // if nothing there return False
      else
         LockStatus = True;
   }
   return rtn;
}

    void
  WinGlobMem ::
UnLockGlobMem()
{
   if (IsGlobMemLocked())
   {
      ::GlobalUnlock(GH);
      *ppV = 0;
      LockStatus = False;
   }
}

    T_F
  WinGlobMem ::
AllocateGlobMem(long len)
{
   return (GH = ::GlobalAlloc(GHND, len)) ? True : False;
}

    void
  WinGlobMem ::
DeallocateGlobMem()
{
   if (GH)
   {
      UnLockGlobMem();
      ::GlobalFree(GH);
      GH = 0;
   }
}


//------------------------------------------------------
// WinBlob Routines
//------------------------------------------------------

  WinBlob ::
WinBlob(OMHWND h, SET_TYPE set, PStoreDb pdb) : OmBlob(set, pdb)
{
   SetHwnd(h);

   // Give WinGlobMem the address of Buffer so it can manage Locking
   // and unlocking of Memory
   SetGlobMemPtr((Pvoid OM_FAR *) &Buffer);
}


  WinBlob ::
WinBlob(OMHWND h, SET_TYPE set, PStoreObj owner, PStoreDb pdb) : OmBlob(set, owner, pdb)
{
   SetHwnd(h);

   // Give WinGlobMem the address of Buffer so it can manage Locking
   // and unlocking of Memory
   SetGlobMemPtr((Pvoid OM_FAR *) &Buffer);
}


    void
  WinBlob ::
Save(RStoreObj Owner)
{
   if (LockGlobMem())
   {
      OmBlob::Save(Owner);
      UnLockGlobMem();
   }
   else
      SetStatus(S_NOMEMORY);
}


    T_F
  WinBlob ::
Write(PStoreObj Owner)
{
   T_F ret = False;

   if (LockGlobMem())
   {
      ret = OmBlob::Write(Owner);
      UnLockGlobMem();
   }
   else
      SetStatus(S_NOMEMORY);

   return ret;
}


//--------------------------------------------------
// ReadyBuffer Leaves it locked
//--------------------------------------------------
    T_F
  WinBlob ::
ReadyBuffer(long len)
{
   T_F worked = True;
   if (BuffSize < len)
   {
      DeallocateBuffer();       // clean up the old buffer, unsets handle
      BuffSize = 0;
   }

   if (!HandleIsSet())          // if handle is not set, need to set it
      AllocateGlobMem(len);

   if (HandleIsSet())           // if we have a set handle, all is well
   {                            // Buff size represents total amount available
      if (!BuffSize)            // Its okay to have more memory than len
         BuffSize = len;        // Reset it only if it has not been set
      LockGlobMem();            // Lock the memory
   }
   else
   {
      BuffSize = len;
      worked = False;
   }
   return worked;
}

    void
  WinBlob ::
DeallocateBuffer()
{
   DeallocateGlobMem();
   Buffer   = NULL;
   BuffSize = 0;
   VLSize   = 0;
}


    T_F
  WinBlob ::
GetClipBoard()
{
   T_F      rtn = False;
   OMHANDLE hdata;
   UINT     wformat;

   if (::OpenClipboard(hwind))
   {
      if (((wformat = ClipFormat()) != 0) &&
          ::IsClipboardFormatAvailable(wformat) &&
          ((hdata = ::GetClipboardData(wformat)) != 0))
      {
         rtn = LoadFromClip(hdata);
      }
      ::CloseClipboard();
   }
   return rtn;
}


    T_F
  WinBlob ::
PutClipBoard()
{
   T_F      rtn = False;
   OMHANDLE hdata;
   UINT     wformat;

   if (::OpenClipboard(hwind))
   {
      if (((wformat = ClipFormat()) != 0) &&
          ((hdata = CopyToHandle()) != 0))
      {
         ::SetClipboardData(wformat, hdata); // Give it to the ClipBoard
         // BoundsChecker says that we are leaving a GDI resource here,
         // but the Clipboard actually will do the DeleteObject() on it.
         rtn = True;
      }
      ::CloseClipboard();
   }

   return rtn;
}

#endif // WINDOWS

