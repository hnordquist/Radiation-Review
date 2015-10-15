//---------------------------------------------------------
//  OMBLOB.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _OMBLOB_HPP_
#define _OMBLOB_HPP_

// Maximum array size for passing blob to server extension
#define MAX_SIZE 32000
//#define MAX_SIZE       0xFFDC
#define BytesInHeader  4

// defined in this file
OMCLASS(OmBlob);
OMCLASS(VariLenText);

// used in this file
OMCLASS(OmFile);
OMCLASS(OmMem);
OMCLASS(GenericMemObj);


class OM_EXPORT OmBlob : public VistaObj
{
private:
   T_F            bNeedRead;
   PStoreObj      pOwner;
   SET_TYPE       set;

   void           Initialize();
   REC_TYPE       GetWriteMemberType(RGenericMemObj, long) const;

protected:
   Phugevoid      Buffer;      // Buffer for storing data in memory
   long           VLSize;      // Current amount of buffer Used
   long           BuffSize;    // Size of buffer allocated

   // heterogenously put and get the length of the blob from the header
   long           HGetHeader(CPvoid header) const;
   Pvoid          HPutHeader(Pvoid header, long length);

protected:
   OmBlob(SET_TYPE Set, PStoreDb pdb = 0);
   OmBlob(SET_TYPE Set, PStoreObj owner, PStoreDb pdb = 0);

   // copy info to and from record slices of the blob
   virtual long   LoadBuffer(Pvoid to, CPvoid from, long maxcopy);
   virtual long   LoadMember(Pvoid to, CPvoid from, long maxcopy);

   // header manipulation
   virtual int    GetHeader(CPvoid ) { return BytesInHeader; }
   virtual int    SetHeader(Pvoid header, long len) { HPutHeader(header, len); return BytesInHeader; }

   // buffer manipulation
   virtual void   AfterRead() {}
   virtual void   DeallocateBuffer();

   // notification functions
   virtual void   OnDelete() {}
   virtual void   OnRead()   {}
   virtual void   OnWrite()  {}

public:
   // free the buffer
   virtual       ~OmBlob() { DeallocateBuffer(); }

   // buffer manipulation
   virtual T_F    ReadyBuffer(long size);
   Phugevoid      GetBuffer()       { if (bNeedRead) Read(); return Buffer; }
   long           GetBuffSize()     { if (bNeedRead) Read(); return BuffSize; }
   void           SetBuffer(Phugevoid buf, long len);

   long           GetAmtUsed()           { if (bNeedRead) Read(); return VLSize; }
   void           SetAmtUsed(long size)  { VLSize = size; }

   // actions on the Blob, optionally specify a new owner
   virtual T_F    Delete(PStoreObj owner = (PStoreObj)NULL);
   virtual T_F    Read(PStoreObj owner = (PStoreObj)NULL);
   virtual T_F    Write(PStoreObj owner = (PStoreObj)NULL);

   void           SetOwner(PStoreObj owner, T_F readnow = True);
   PStoreObj      GetOwner() const            { return pOwner; }
   SET_TYPE       GetSet() const              { return set; }

   // Blob Import/Export support from flat files
   virtual T_F    Import(CPchar FName);
   virtual T_F    Export(CPchar FName);


   // NOTE: the following functions have been replaced with more suitable
   //  functions for a number of reasons.  For backward compatibility, 
   //  they have been left in here.  They will be removed in a 
   //  future release.

   // Init() is no longer needed
   T_F            Init() { return True; }

   // GetVLSize() has been replaced with GetAmtUsed() to correspond with SetAmtUsed()
   long           GetVLSize() { if (bNeedRead) Read(); return VLSize; }

   // DeleteAll() has been replaced with Delete(), thus changing the semantics 
   //  of the delete.  Previous to ROM 3.0, DeleteAll() would delete the blob
   //  attached to the passed in owner rather than deleting the blob the 
   //  function was operated on (if the owners didn't match).
   //  In addition, Delete() corresponds with StoreObj::Delete().
   T_F            DeleteAll(RStoreObj Owner) { return Delete(&Owner); }

   // Both Save() functions have been replaced with Write() to correspond 
   //  with StoreObj;:Write().  In addition, Write() will only attach the 
   //  blob to the owner that OmBlob::pOwner points to. 
   virtual void   Save(RStoreObj Owner) { Write(&Owner); } // replace with Write()
   void           Save(RStoreObj Owner, long size) { SetAmtUsed(size); Write(&Owner); } // replace with Write()

   // Import() has been changed to not accept an owner for the same reason
   //  that DeleteAll and been changed -- a blob could be used to import 
   //  information into an owner unrelated to the blob itself.
   virtual T_F    Import(CPchar FName, RStoreObj Owner);
};


class OM_EXPORT VariLenText : public OmBlob
{
protected:
   virtual void  AfterRead();  // Null Terminate the blob

public:
   VariLenText(SET_TYPE Set, PStoreDb pdb = 0);
   VariLenText(SET_TYPE Set, PStoreObj owner, PStoreDb pdb = 0) : OmBlob(Set, owner, pdb) {}

   // Allocate 1 byte extra for NULL
   virtual T_F   ReadyBuffer(long len)
      { return OmBlob::ReadyBuffer(len + 1); }
};


#endif
