//-----------------------------------------------------
//  OMFILE.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------

extern "C" {
#include <stdlib.h>
#include <stdio.h>
#ifdef UNIX
//#include <sys/stat.h>
#include <sys/io.h>
#include <fcntl.h>
#include <unistd.h>
#else
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#endif
#include <errno.h>
}
#include <omfile.hpp>

//-----------------------------------------------------
// Om file service class
//-----------------------------------------------------
    void 
  OmFile::
Clear()
{
   ErrCode = 0;
   FileHandle = 0;
   pos = 0l;
   size = 0l;
}


    int 
  OmFile::
PrepareFlags(OmFileOpenMode Mode)
{
#ifdef UNIX
   int F = 0;
#else
   int F = O_BINARY;
#endif

   CurrMode = Mode;

   if (Mode == OM_FILE_READ)
      F |= O_RDONLY;
   else if (Mode == OM_FILE_WRITE)
      F |= (O_CREAT | O_WRONLY | O_TRUNC);

   return F;
}


    T_F 
  OmFile::
Open(CPchar FileName, OmFileOpenMode Mode)
{
   T_F res = False;

#ifdef UNIX
   if ((FileHandle = open(FileName, PrepareFlags(Mode), O_RDWR)) == -1)
#else
   if ((FileHandle = open(FileName, PrepareFlags(Mode), S_IREAD|S_IWRITE)) == -1)
#endif
   {
      ErrCode = errno;
      FileHandle = 0;
   } 
   else
   {
      size = Seek(0l, SEEK_END);
      pos = Seek(0l, SEEK_SET);
      res = True;
   }

   return res;
}


    void 
  OmFile::
Close()
{
   if (FileHandle)
      close(FileHandle);

   Clear();
}


    long 
  OmFile::
Read(Phugechar Buf, long Len)
{
   long          res     = 0l; 
   unsigned int  res1    = 0;
   Phugechar     buf_ptr = Buf;

   if (!FileHandle)
   {
      ErrCode = EBADF;
   } 
   else if (CurrMode == OM_FILE_READ) 
   {
      while ((unsigned long)Len >= (unsigned long)MAX_READ_WRITE)
      {
         if ((res1 = read(FileHandle, (Pchar)buf_ptr, MAX_READ_WRITE)) == MAX_READ_WRITE)
         {
            res += MAX_READ_WRITE;
            Len -= MAX_READ_WRITE;
            pos += MAX_READ_WRITE;
            buf_ptr += MAX_READ_WRITE;
         } 
         else 
            break;
      }

      if ((res1 != -1) && Len)
      {
         if ((res1 = read(FileHandle, (Pchar)buf_ptr, (unsigned int)Len)) == (unsigned int)Len)
         {
            res += Len;
            pos += Len;
         }
      }
   }

   if (res1 == -1)
      ErrCode = errno;

   return res;
}


    long 
  OmFile::
Write(CPhugechar Buf, long Len)
{
   long          res     = 0l; 
   unsigned int  res1    = 0;
   Phugechar     buf_ptr = (Phugechar)Buf;

   if (!FileHandle)
   {
      ErrCode = EBADF;
   } 
   else if (CurrMode == OM_FILE_WRITE)
   {
      while ((unsigned long)Len >= (unsigned long)MAX_READ_WRITE)
      {
         if ((res1 = write(FileHandle, (Pchar)buf_ptr, MAX_READ_WRITE)) == MAX_READ_WRITE)
         {
            res += MAX_READ_WRITE;
            Len -= MAX_READ_WRITE;
            pos += MAX_READ_WRITE;
            buf_ptr += MAX_READ_WRITE;
         } 
         else 
            break;
      }

      if ((res1 != -1) && Len)
      {
         if ((res1 = write(FileHandle, (Pchar)buf_ptr, (unsigned int)Len)) == (unsigned int)Len )
         {
            res += Len;
            pos += Len;
         }
      }
   }

   if (res1 == -1)
      ErrCode = errno;
   else if (pos > size)
      size = pos;

   return res;
}


    long 
  OmFile::
Seek(long Len, int Origin)
{
   long res = -1;

   if (!FileHandle)
      ErrCode = EBADF;
   else
   {
      if ((res = lseek(FileHandle, Len, Origin)) == -1)
         ErrCode = errno;
      else
         pos = res;
   }

   return res;
}
