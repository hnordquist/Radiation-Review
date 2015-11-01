//---------------------------------------------------------
//  OMFILE.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _OMFILE_HPP_
#define _OMFILE_HPP_

#include "vpp.hpp"
#include <limits.h>

const unsigned int MAX_READ_WRITE = (UINT_MAX - 1);

#define OM_FILENAMELENGTH 79
enum OmFileOpenMode { OM_FILE_READ, OM_FILE_WRITE };

OMCLASS(OmFile);


class OM_EXPORT OmFile
{
private:
   int            FileHandle;
   OmFileOpenMode CurrMode;
   long           pos;
   long           size;
   int            ErrCode;

   void  Clear();

protected:
   void  SetErrorCode(int e) { ErrCode = e; }
   void  SetSize(long s)     { size = s; }

   int   PrepareFlags(OmFileOpenMode Mode);

public:
   OmFile() { Clear(); }
   OmFile(CPchar file, OmFileOpenMode mode) { Clear(); Open(file, mode); }
   virtual ~OmFile() { Close(); }

   T_F  IsFileOpen()    const { return FileHandle ? True : False; }
   int  GetFileHandle() const { return FileHandle; }
   int  GetErrorCode()  const { return ErrCode; }
   long GetSize()       const { return size; }

   virtual T_F   Open(CPchar, OmFileOpenMode);
   virtual void  Close();

   virtual long  Read(Phugechar buf, long len);
   virtual long  Write(CPhugechar buf, long len);

   virtual long  Seek(long, int);
   virtual long  Tell() const { return pos; }
};

#endif

