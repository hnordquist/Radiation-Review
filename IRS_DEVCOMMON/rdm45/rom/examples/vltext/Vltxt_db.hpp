#ifndef VLTXT_DB_HPP
#define VLTXT_DB_HPP

/* Raima Database Manager Version 4.5 */


/* Raima Object Manager specific header files */
#include <storedb.hpp>
#include <storeobj.hpp>
#include <keyobj.hpp>
#include <omblob.hpp>

/* include record/key structure definitions */
#include "vltxt_db.h"

/* database class declaration */
class OM_EXPORT vltxt_db : public StoreDb
{
public:
   vltxt_db() : StoreDb("vltxt_db", PDB_LOCATOR) {}
   DEFINE_DB_LOCATOR;
};


/* Persistent Object classes that are defined in this file */
OMCLASS(DocumentObj)
OMCLASS(VLText_4096Obj)
OMCLASS(VLText_2048Obj)
OMCLASS(VLText_512Obj)
OMCLASS(VLText_128Obj)
OMCLASS(VLText_SetObj)
OMCLASS(Document_Name_KeyObj)


class OM_EXPORT DocumentObj : public StoreObj, public Document
{
public:
   DocumentObj(const PVistaDb pdb=vltxt_db::DbPtr) : StoreObj(pdb) {}
   DocumentObj(const RKeyObj key, const PVistaDb pdb=vltxt_db::DbPtr) : StoreObj(key, pdb) {}
   virtual ~DocumentObj() {}

   virtual REC_TYPE RecType() const { return DOCUMENT; }
   virtual Pchar OMLOADDS RecName() const { return "DOCUMENT"; }
   virtual Pvoid RecData() { return (Pvoid)(Document OM_FAR *)this; }

   STOREDIN(vltxt_db);
   CUSTOMOPS(DocumentObj);
   OWNEROF(VLText_4096Obj, VLTEXT_SET);
   OWNEROF(VLText_2048Obj, VLTEXT_SET);
   OWNEROF(VLText_512Obj, VLTEXT_SET);
   OWNEROF(VLText_128Obj, VLTEXT_SET);
   OWNEROFBLOB(VLText_SetObj, VLTEXT_SET);
};


class OM_EXPORT VLText_4096Obj : public StoreObj, public VLText_4096
{
public:
   VLText_4096Obj(const PVistaDb pdb=vltxt_db::DbPtr) : StoreObj(pdb) {}
   VLText_4096Obj(const RSetObj set, const PVistaDb pdb=vltxt_db::DbPtr) : StoreObj(set, pdb) {}
   virtual ~VLText_4096Obj() {}

   virtual REC_TYPE RecType() const { return VLTEXT_4096; }
   virtual Pchar OMLOADDS RecName() const { return "VLTEXT_4096"; }
   virtual Pvoid RecData() { return (Pvoid)(VLText_4096 OM_FAR *)this; }

   STOREDIN(vltxt_db);
   CUSTOMOPS(VLText_4096Obj);
   MEMBEROF(DocumentObj, VLTEXT_SET);
};


class OM_EXPORT VLText_2048Obj : public StoreObj, public VLText_2048
{
public:
   VLText_2048Obj(const PVistaDb pdb=vltxt_db::DbPtr) : StoreObj(pdb) {}
   VLText_2048Obj(const RSetObj set, const PVistaDb pdb=vltxt_db::DbPtr) : StoreObj(set, pdb) {}
   virtual ~VLText_2048Obj() {}

   virtual REC_TYPE RecType() const { return VLTEXT_2048; }
   virtual Pchar OMLOADDS RecName() const { return "VLTEXT_2048"; }
   virtual Pvoid RecData() { return (Pvoid)(VLText_2048 OM_FAR *)this; }

   STOREDIN(vltxt_db);
   CUSTOMOPS(VLText_2048Obj);
   MEMBEROF(DocumentObj, VLTEXT_SET);
};


class OM_EXPORT VLText_512Obj : public StoreObj, public VLText_512
{
public:
   VLText_512Obj(const PVistaDb pdb=vltxt_db::DbPtr) : StoreObj(pdb) {}
   VLText_512Obj(const RSetObj set, const PVistaDb pdb=vltxt_db::DbPtr) : StoreObj(set, pdb) {}
   virtual ~VLText_512Obj() {}

   virtual REC_TYPE RecType() const { return VLTEXT_512; }
   virtual Pchar OMLOADDS RecName() const { return "VLTEXT_512"; }
   virtual Pvoid RecData() { return (Pvoid)(VLText_512 OM_FAR *)this; }

   STOREDIN(vltxt_db);
   CUSTOMOPS(VLText_512Obj);
   MEMBEROF(DocumentObj, VLTEXT_SET);
};


class OM_EXPORT VLText_128Obj : public StoreObj, public VLText_128
{
public:
   VLText_128Obj(const PVistaDb pdb=vltxt_db::DbPtr) : StoreObj(pdb) {}
   VLText_128Obj(const RSetObj set, const PVistaDb pdb=vltxt_db::DbPtr) : StoreObj(set, pdb) {}
   virtual ~VLText_128Obj() {}

   virtual REC_TYPE RecType() const { return VLTEXT_128; }
   virtual Pchar OMLOADDS RecName() const { return "VLTEXT_128"; }
   virtual Pvoid RecData() { return (Pvoid)(VLText_128 OM_FAR *)this; }

   STOREDIN(vltxt_db);
   CUSTOMOPS(VLText_128Obj);
   MEMBEROF(DocumentObj, VLTEXT_SET);
};


class OM_EXPORT VLText_SetObj : public VariLenText
{
public:
   VLText_SetObj(const PVistaDb pdb=vltxt_db::DbPtr) : VariLenText(VLTEXT_SET, pdb) {}
   VLText_SetObj(const PStoreObj owner, const PVistaDb pdb=vltxt_db::DbPtr) : VariLenText(VLTEXT_SET, owner, pdb) {}
   virtual ~VLText_SetObj() {}

   STOREDIN(vltxt_db);
   BLOBMEMBEROF(DocumentObj, VLTEXT_SET);
};


class OM_EXPORT Document_Name_KeyObj : public KeyObj
{
public:
   char Name[21];

   Document_Name_KeyObj() : KeyObj(DOCUMENT_NAME, (Pvoid) &Name[0]) {}
   virtual ~Document_Name_KeyObj() {}
};



#endif    /* VLTXT_DB_HPP */
