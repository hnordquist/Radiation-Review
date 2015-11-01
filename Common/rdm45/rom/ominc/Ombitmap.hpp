//---------------------------------------------------------
//  OMBITMAP.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _OMBITMAP_HPP_
#define _OMBITMAP_HPP_

#include "winblob.hpp"
#ifdef WINDOWS

class OM_EXPORT OmBitmap : public WinBlob
{
protected:
	BITMAP OM_FAR * Bmap;   // stored in Blob header

	int  GetHeader(CPvoid p);
	int  SetHeader(Pvoid p, long len);

public:
	OmBitmap(OMHWND h, SET_TYPE set, PStoreDb pdb = 0) : WinBlob(h, set, pdb) 
		{ Bmap = new BITMAP; }
	OmBitmap(OMHWND h, SET_TYPE set, PStoreObj owner, PStoreDb pdb = 0) : WinBlob(h, set, owner, pdb) 
		{ Bmap = new BITMAP; }
	~OmBitmap() 
		{ delete Bmap; };

	// overiding pure virtuals
	virtual UINT      ClipFormat() { return CF_BITMAP; }
	virtual T_F       LoadFromClip(OMHANDLE hdata);
	virtual OMHANDLE  CopyToHandle();      // used to copy to clipboard
	virtual void      Show(int xStart, int yStart);
};

#endif // WINDOWS
#endif // OMBITMAP_HPP
