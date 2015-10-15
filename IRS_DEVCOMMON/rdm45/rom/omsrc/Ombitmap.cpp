//-----------------------------------------------------
//  OMBITMAP.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#ifdef WINDOWS

#include <storedb.hpp>
#include <storeobj.hpp>
#include <ombitmap.hpp>


//-----------------------------------------------------
// OmBitMap Routines
//-----------------------------------------------------
    int
  OmBitmap ::
GetHeader(CPvoid p)
{
   Pchar pc = (Pchar)((Pchar)p + BytesInHeader);

   // Copy Bitmap from Buffer to Bmap
   om_memcpy(( Pchar )Bmap,( Pchar )pc,sizeof(BITMAP));
   Bmap->bmBits = 0;
   return (BytesInHeader + sizeof(BITMAP));
}

    int
  OmBitmap ::
SetHeader(Pvoid p, long len)
{
   OmBlob::SetHeader(p, len);
   Pchar pc = (Pchar)((Pchar)p + BytesInHeader);

   om_memcpy(pc, (Pchar)Bmap, sizeof(BITMAP)); 
   return (BytesInHeader + sizeof(BITMAP));
}


//------------------------------------------------------
// Clip Board is open, load necessary information to Blob area
//------------------------------------------------------
    T_F
  OmBitmap ::
LoadFromClip(OMHANDLE hdata)
{
   T_F rtn = False;
   long dwcount;

   // GetObject copys hBitmap( hdata) info to local OmBitmap.Bmap
   // which is saved in the blob header area
   ::GetObject(hdata, sizeof(BITMAP), (LPSTR)Bmap);

   dwcount = Bmap->bmWidthBytes;   // put into dwcount to force long math
   dwcount = dwcount * Bmap->bmHeight * Bmap->bmPlanes;

   if (ReadyBuffer(dwcount))   // allocates if needed, and Locks Mem
   {   
      ::GetBitmapBits((HBITMAP)hdata, dwcount, (Pchar)GetBuffer()) ;
      SetAmtUsed(dwcount);
      UnLockGlobMem();
      rtn = True;
   }
   return rtn;
}

    OMHANDLE 
  OmBitmap ::
CopyToHandle()
{   
   HBITMAP hBitmap;
   if (LockGlobMem())
   {
      Bmap->bmBits = (Pchar)GetBuffer();

      if ((hBitmap = ::CreateBitmapIndirect(Bmap)) != 0)
         UnLockGlobMem();
   }
   return (OMHANDLE)hBitmap;
}

    void
  OmBitmap ::
Show(int xStart, int yStart)
{
   HBITMAP hBitmap;

   if (LockGlobMem())
   {
      Bmap->bmBits = (Pchar)GetBuffer();

      if ((hBitmap = ::CreateBitmapIndirect(Bmap)) != 0)
      {
         POINT ptSize, ptOrigin;
         HDC hDC = ::GetDC(hwind);
         HDC hDCMem = ::CreateCompatibleDC(hDC);

         ::SelectObject(hDCMem, hBitmap);
         ::SetMapMode(hDCMem, GetMapMode(hDC));

         ptSize.x = Bmap->bmWidth;     // = GetSize( hDC );
         ptSize.y = Bmap->bmHeight;    // = GetSize( hDC );
         ptOrigin.x = 0;
         ptOrigin.y = 0;
         ::DPtoLP(hDCMem, &ptOrigin, 1);

         ::BitBlt(hDC, xStart, yStart, ptSize.x, ptSize.y,
               hDCMem, ptOrigin.x, ptOrigin.y, SRCCOPY);
         ::DeleteDC(hDCMem);
         ::DeleteObject(hBitmap);
         ::ReleaseDC(hwind, hDC);
     }
     UnLockGlobMem();
   }
}

#endif // WINDOWS

