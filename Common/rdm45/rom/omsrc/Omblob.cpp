//-----------------------------------------------------
//   OMBLOB.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <storeobj.hpp>
#include <omblob.hpp>
#include <generic.hpp>
#include <ommem.hpp>
#include <omfile.hpp>

#ifdef OM_RDS
#include <hlfproto.h>
#endif

#if defined(OM_RDM)
#undef NO_DT_COVER   // don't need d_delete redefinition
extern "C" {
#include <internal.h>
};
#undef NEXT   // undefine these because RDM defines them
#undef LAST
#undef FIRST
#endif // OM_RDM

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif


static long om_hmemcpy(Phugevoid pp1, CPhugevoid pp2, long c)
{
#ifdef OM_16BIT
   long count = c;
   Phugechar p1 = (Phugechar)pp1;
   Phugechar p2 = (Phugechar)pp2;

   for(;count > 0; count--, p1++, p2++)
      *p1 = *p2;
#else
   om_memcpy(pp1, pp2, (size_t)c);
#endif

   return c;
}

//----------------------------------------------------------
// OmBlob class functions
//----------------------------------------------------------
  OmBlob ::
OmBlob(SET_TYPE Set, PStoreDb pdb) : VistaObj(pdb)
{
   pOwner = (PStoreObj)NULL;
   set = Set;
   bNeedRead = False;
   Initialize();
}


  OmBlob ::
OmBlob(SET_TYPE Set, PStoreObj owner, PStoreDb pdb) : VistaObj(pdb)
{
   pOwner = owner;
   bNeedRead = True;
   set = Set;
   Initialize();
}


    void
  OmBlob ::
Initialize()
{
   Buffer   = NULL;
   BuffSize = 0;
   VLSize   = 0;
}


    long
  OmBlob ::
HGetHeader(CPvoid header) const
{
   unsigned char  *curbyte = (unsigned char *)header;
   int             index;
   long            length = 0L;

   for (index = 0; index < BytesInHeader; ++index)
   {
      length *= 256L;
      length += (long)(curbyte[BytesInHeader - index - 1]);
   }
   return length;
}


    Pvoid
  OmBlob ::
HPutHeader(Pvoid header, long length)
{
   unsigned char  *curbyte = (unsigned char *)header;
   int             index;

   for (index = 0; index < BytesInHeader; ++index)
   {
      if (length)
      {
         curbyte[index] = (unsigned char)(length % 256L);
         length /= 256L;
      }
      else
         curbyte[index] = (unsigned char)0;
   }
   return (header);
}


    T_F
  OmBlob ::
ReadyBuffer(long len)
{
   T_F worked = True;

   if (len <= 0)
      worked = False;
   else if ((BuffSize < len) || (BuffSize > USHRT_MAX && (len*3) < BuffSize))
   {
      // try not to be a memory hog
      DeallocateBuffer();
      BuffSize = len;

#ifdef OM_16BIT
      if (BuffSize > 131072L)
      {
         BuffSize += sizeof(long);
         Buffer = (Phugevoid) om_halloc(BuffSize / sizeof(long), sizeof(long));
      }
      else
         Buffer = (Phugevoid) om_halloc(BuffSize, sizeof(char));
#else
      Buffer = (Phugevoid) om_halloc(BuffSize, sizeof(char));
#endif

      if (!Buffer)
      {
         worked = False;
         ROMERROR(S_NOMEMORY);
         SetStatus( S_NOMEMORY );
      }
   }

   if (!worked)
      BuffSize = 0;
   return worked;
}


    void
  OmBlob ::
DeallocateBuffer()
{
   if (Buffer)
      om_hfree(Buffer);
   
   // Initialize it just in case...
   Initialize();
}


    void
  OmBlob ::
SetBuffer(Phugevoid buf, long len)
{ 
   DeallocateBuffer(); 
   Buffer = buf; 
   BuffSize = VLSize = len; 
}


    T_F
  OmBlob ::
Read(PStoreObj owner)
{
   // Must have an owner in order to read the blob
   if (!owner && !pOwner)
   {
      ROMERROR(S_NOBLOBOWN);
      SetStatus( S_NOBLOBOWN );
      return False;
   }

   // give the blob a new owner
   if (owner)
      pOwner = owner;

   Phugechar  buffer_ptr;
   long       buffer_left;
   int        hdrlen;
   long       stored_len;
   USHORT     recdatasize;
   Pchar      recdata;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   short      stat;
   short      more_flag;
   short      lminfo = GetDb()->GetTask()->LockModeInfo();

   recdatasize = MAX_SIZE;
   recdata = new OM_FAR char[recdatasize];
   
   if (!recdata)
   {
      SetStatus(S_NOMEMORY);
      return False;
   }
   om_memset((Pvoid)recdata, '\0', recdatasize);

   if ((stat = hlf_BlobReadHeader(pOwner->GetDba(), set, lminfo, &recdatasize, (Pvoid)recdata, &more_flag, GetDb()->GethDb())) != S_OKAY)
   {
      SetStatus( stat );
      delete recdata;
      return False;
   }

   stored_len = buffer_left = HGetHeader(recdata);
   if (!stored_len)
   {
      VLSize = 0;
      bNeedRead = False;
      SetStatus( S_EOS );
      delete recdata;
      return True;
   }
   hdrlen = GetHeader(recdata);

   if (!ReadyBuffer(stored_len))
   {
      // ReadyBuffer() sets the status
      delete recdata;
      return False;
   }

   buffer_ptr = (Phugechar)Buffer;
   recdatasize = (USHORT)min(((long)recdatasize-hdrlen), buffer_left);
   LoadBuffer((Pvoid)buffer_ptr, (Pvoid)(recdata+hdrlen), recdatasize);
   buffer_ptr += recdatasize;
   buffer_left -= recdatasize;

   while (stat == S_OKAY && more_flag)
   {
      recdatasize = MAX_SIZE;
      om_memset((Pvoid)recdata, '\0', recdatasize);

      if ((stat = hlf_BlobRead(set, lminfo, &recdatasize, (Pvoid)recdata, &more_flag, FALSE, GetDb()->GethDb())) != S_OKAY)
         break;

      recdatasize = (USHORT)min((long)recdatasize, buffer_left);
      LoadBuffer((Pvoid)buffer_ptr, (Pvoid)recdata, recdatasize);

      buffer_ptr += recdatasize;
      buffer_left -= recdatasize;
   }

   SetStatus( stat );
   delete recdata;

#else

   GenericMemObj  genobj(set, GetDb());

   // find first member
   if (!genobj.GotoMember(*pOwner, set, FIRST).Okay())
   {
      SetStatus( genobj.GetStatus() );
      return False;
   }

   // get header information
   recdata = (Pchar)genobj.RecData();
   recdatasize = genobj.RecDataSize();
   
   stored_len = buffer_left = HGetHeader(recdata);
   hdrlen = GetHeader(recdata);

   // prepare the buffer for the blob
   if (!ReadyBuffer(stored_len))
      return (Okay());

   buffer_ptr  = (Phugechar)Buffer;

   recdatasize = (USHORT)min((long)(recdatasize-hdrlen), buffer_left);
   LoadBuffer((Pvoid)buffer_ptr, (Pvoid)(recdata+hdrlen), recdatasize);
   buffer_ptr += recdatasize;
   buffer_left -= recdatasize;

   // go through each member while there is buffer_left && more members
   genobj++;
   for (; buffer_left > 0 && genobj.Okay(); genobj++)
   {
      recdatasize = (USHORT)min((long)genobj.RecDataSize(), buffer_left);
      LoadBuffer((Pvoid)buffer_ptr, (Pvoid)recdata, recdatasize);

      buffer_ptr += recdatasize;
      buffer_left -= recdatasize;
   }
#endif
   
   VLSize = (long)(buffer_ptr - (Phugechar)Buffer);

   // This Should not be possible...
   if (stored_len != VLSize)
   {
      ROMERROR(S_BLOBREAD);
      SetStatus( S_BLOBREAD );
   }
   else
   {
      bNeedRead = False;
      SetStatus(S_OKAY);
      AfterRead();
      OnRead();
   }

   return (Okay());
}


    T_F
  OmBlob ::
Delete(PStoreObj owner)
{
   // Must have an owner in order to delete the blob
   if (!owner && !pOwner)
   {
      ROMERROR(S_NOBLOBOWN);
      SetStatus( S_NOBLOBOWN );
      return False;
   }

   // give the blob a new owner
   if (owner)
      SetOwner(owner, False);

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   short lminfo = GetDb()->GetTask()->LockModeInfo();
   SetStatus( hlf_BlobDelete(pOwner->GetDba(), set, lminfo, GetDb()->GethDb()) );
#else
   if (pOwner->MakeOwner(set, "w") != S_OKAY)
   {
      SetStatus( pOwner->GetStatus() );
      return False;
   }

   PStoreDb pDb = GetDb();
   short stat = S_OKAY;

   while (stat == S_OKAY && (stat = pDb->findfm(set)) == S_OKAY)
   {
/* Future development... #if defined(OM_RDS) && defined(OM_SERVER)
      if (stat == S_OKAY && OM_IMPLICIT_WRITES(GetDb()->GetTask()->LockModeInfo()))
         if ((stat = pDb->crlock("w")) != S_OKAY)
            break;
#endif
*/
      stat = pDb->disdel();
   }

   // set the status of this blob to S_OKAY
   SetStatus( (stat == S_EOS) ? S_OKAY : stat );
#endif

   if (Okay())
   {
      OnDelete();
      bNeedRead = False;
   }

   return Okay();
}


    T_F
  OmBlob ::
Write(PStoreObj owner)
{
   // Must have an owner in order to save the blob
   if (!owner && !pOwner)
   {
      ROMERROR(S_NOBLOBOWN);
      SetStatus( S_NOBLOBOWN );
      return False;
   }

   // give the blob a new owner
   if (owner)
      SetOwner(owner, False);

   PStoreDb   pDb           = GetDb();
   Phugechar  buffer_ptr    = (Phugechar)Buffer;
   long       buffer_left   = VLSize;
   USHORT     recdatasize;
   Pchar      recdata;
   int        hdrlen;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   short      lminfo = GetDb()->GetTask()->LockModeInfo();
   short      stat;
   short      more_flag;
   USHORT     buffer_to_copy;

   recdatasize = MAX_SIZE;
   recdata = new OM_FAR char[MAX_SIZE];

   if (!recdata)
   {
      SetStatus(S_NOMEMORY);
      return False;
   }

   hdrlen = SetHeader(recdata, buffer_left);

   buffer_to_copy = (USHORT)min((long)recdatasize-hdrlen, buffer_left);
   LoadMember((Pvoid)(recdata+hdrlen), (Pvoid)buffer_ptr, buffer_to_copy);
   recdatasize = (USHORT)(buffer_to_copy + hdrlen);

   more_flag = (((long)buffer_to_copy != buffer_left) ? True : False);
   stat = hlf_BlobWriteHeader(pOwner->GetDba(), set, lminfo, (USHORT *)&recdatasize, (Pvoid)recdata, more_flag, pDb->GethDb());
   buffer_left -= recdatasize-hdrlen;
   buffer_ptr += recdatasize-hdrlen;

   while (stat == S_OKAY && buffer_left)
   {
      recdatasize = MAX_SIZE;
      recdatasize = (USHORT)min((long)recdatasize, buffer_left);
      LoadMember((Pvoid)recdata, (Pvoid)buffer_ptr, recdatasize);
      more_flag = (((long)recdatasize == buffer_left) ? False : True);
      
      stat = hlf_BlobWrite(set, lminfo, (USHORT *)&recdatasize, (Pvoid)recdata, more_flag, pDb->GethDb());
      
      buffer_left -= recdatasize;
      buffer_ptr += recdatasize;
   }

   delete [] recdata;
   SetStatus( stat );
#else

   // Delete old Blob
   if (!Delete(pOwner))
      return False;

   GenericMemObj  genobj(set, pDb);
   REC_TYPE   rectype;

   recdata = (Pchar)genobj.RecData();

   // copy header info
   hdrlen  = SetHeader(recdata, buffer_left);

	// must take into consideration the header length when 
	// selecting the write record type   
   rectype = GetWriteMemberType(genobj, buffer_left+hdrlen);
   recdatasize = genobj.RecDataSize();

   // copy as much buffer as will fit
   recdatasize = (USHORT)min((long)(recdatasize-hdrlen), buffer_left);
   LoadMember((Pvoid)(recdata+hdrlen), (Pvoid)buffer_ptr, recdatasize);

   genobj.NewMember(*pOwner, set);
   
   buffer_left -= recdatasize;
   buffer_ptr += recdatasize;

   // go through each member while there is buffer_left
   while (buffer_left && genobj.Okay())
   {
      rectype = GetWriteMemberType(genobj, buffer_left);
      recdatasize = (USHORT)min((long)genobj.RecDataSize(), buffer_left);
      LoadMember((Pvoid)recdata, (Pvoid)buffer_ptr, recdatasize);

      genobj.NewObj();
      if (genobj.Okay())
         genobj.SetStatus(pDb->connect(set));

      buffer_ptr += recdatasize;
      buffer_left -= recdatasize;
   }

   SetStatus( genobj.GetStatus() );
#endif

   if (Okay())
      OnWrite();
   else
      ROMERROR(S_BLOBWRITE);

   return Okay();
}


    REC_TYPE
  OmBlob ::
GetWriteMemberType(RGenericMemObj memobj, long amt_left) const
{
// Future development... //#if defined(OM_RDM) || (defined(OM_RDS) && defined(OM_SERVER))
#if defined(OM_RDM) 
   // Members must be listed from Largest to Smallest (in the schema)
   // iterate thru all set members finding the largest
   RMMSetObj mmsetobj = *memobj.GetMMSetObj();
   for (short idx = 0; idx < mmsetobj.MemCount(); idx++)
   {
      if (amt_left > (long)mmsetobj[idx]->recdatasize)
      {
         if (idx > 0)
            idx--;
         break;
      }
      else if (amt_left == (long)mmsetobj[idx]->recdatasize)
         break;
   }

   // if passed end of list, use last one
   if (idx == mmsetobj.MemCount())
      idx--;

   memobj.SetRecType(mmsetobj[idx]->rectype);
   return memobj.CurrRecType();
#else
   return 0;
#endif
}


    long
  OmBlob::
LoadMember(Pvoid data, CPvoid buf, long max)
{
   return om_hmemcpy(data, buf, max);
}


    long
  OmBlob::
LoadBuffer(Pvoid buf, CPvoid data, long max)
{
   return om_hmemcpy(buf, data, max);
}


    void
  OmBlob::
SetOwner(PStoreObj owner, T_F readnow)
{
   pOwner = owner; 
   if ((bNeedRead = readnow) == True)
      Read();
}


    T_F
  OmBlob::
Import(CPchar FName)
{
   // Must have an owner in order to save the blob
   if (!pOwner)
   {
      ROMERROR(S_NOBLOBOWN);
      SetStatus( S_NOBLOBOWN );
      return False;
   }
   else
      return Import(FName, *pOwner);
}


    T_F
  OmBlob::
Import(CPchar FName, RStoreObj Owner)
{
   OmFile InputFile(FName, OM_FILE_READ);
   if (!InputFile.IsFileOpen())
      return False;

   InputFile.Seek(0l, SEEK_SET);
   long lSize = InputFile.GetSize();

   if (!ReadyBuffer(lSize))
      return (False);

   T_F ret = False;
   
   if (InputFile.Read((Phugechar)Buffer, lSize) == lSize)
   {
      bNeedRead = False;
      SetAmtUsed(lSize);
      AfterRead();
      Write(&Owner);
      ret = T_F(GetStatus() == S_OKAY);
   }

   return ret;
}


    T_F
  OmBlob::
Export(CPchar FName)
{
   OmFile OutputFile(FName, OM_FILE_WRITE);
   if (!OutputFile.IsFileOpen())
      return False;

   T_F ret = True;
   if (Buffer && VLSize)
      ret = T_F(OutputFile.Write((Phugechar)Buffer, VLSize) == VLSize);

   return ret;
}


// NULL terminate the buffer string
    void
  VariLenText ::
AfterRead()
{
   Phugechar p = (Phugechar) GetBuffer();
   p[GetAmtUsed()] = '\0';
}


// MSVC 1.51 requires that this function is out-of-line
  VariLenText ::
VariLenText(SET_TYPE Set, PStoreDb pdb) : OmBlob(Set, pdb) 
{
}

