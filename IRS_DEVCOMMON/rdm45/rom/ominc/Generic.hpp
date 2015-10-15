//---------------------------------------------------------
//  GENERIC.HPP Generic StoreObj used in Omblob and Polymorph
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _GENERIC_HPP_
#define _GENERIC_HPP_

OMCLASS(KeyObj);
OMCLASS(StoreDb);
OMCLASS(StoreObj);
OMCLASS(SetObj);

OMCLASS(GenericObj);
OMCLASS(GenericMemObj);


class OM_EXPORT GenericObj : public StoreObj
{
private:
   virtual void AllocateRecData();
   virtual PStoreDb OMLOADDS _GetDb() const { return (PStoreDb)NULL; }

protected:
   REC_TYPE  rectype;
   Pvoid     recdata;

public:
   GenericObj(REC_TYPE type, PStoreDb pDb) : StoreObj(pDb), rectype(type), recdata(NULL) {}
   GenericObj(REC_TYPE type, CRSetObj set, PStoreDb pDb) : StoreObj(set, pDb), rectype(type), recdata(NULL) {}
   GenericObj(REC_TYPE type, CRKeyObj key, PStoreDb pDb) : StoreObj(key, pDb), rectype(type), recdata(NULL) {}
   virtual ~GenericObj();

   virtual Pvoid    RecData();
   virtual REC_TYPE RecType() const { return rectype; }
};


class OM_EXPORT GenericMemObj : public GenericObj
{
private:
   short     index;
   PMMSetObj mmsetobj;
   USHORT    recdatasize;

   void AllocateRecData();
   
protected:
   virtual RStoreObj Read();

public:
   GenericMemObj(SET_TYPE set, PStoreDb pDb) : GenericObj(0, SetObj(set), pDb), index(0), recdatasize(0), mmsetobj((PMMSetObj)NULL) {}
   GenericMemObj(CRSetObj set, PStoreDb pDb) : GenericObj(0, set, pDb), index(0), recdatasize(0), mmsetobj((PMMSetObj)NULL) {}
   virtual ~GenericMemObj();

//   virtual USHORT   RecDataSize() const { return (*GetMMSetObj())[index]->recdatasize; }
   virtual USHORT   RecDataSize() const { return recdatasize; }
   virtual REC_TYPE CurrRecType() { return (*GetMMSetObj())[index]->rectype; }
   virtual void     SetRecType(REC_TYPE rt);

   SET_TYPE  GetSet()      { return GetMMSetObj()->Num(); }
   PMMSetObj GetMMSetObj() { if (!mmsetobj) AllocateRecData(); return mmsetobj; }
};


#endif

