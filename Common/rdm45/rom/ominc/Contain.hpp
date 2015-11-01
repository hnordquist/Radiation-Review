//---------------------------------------------------------
//  CONTAIN.HPP a container class for StoreObj
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _CONTAIN_HPP_
#define _CONTAIN_HPP_

#include "vpp.hpp"
#include "storeobj.hpp"


#define OBJARRAY(ARRAYNAME,OBJ)                                             \
ARRAYNAME : public StoreObjArray                                            \
{                                                                           \
public:                                                                     \
   ARRAYNAME(OBJ OM_FAR &obj, int size = 100) : StoreObjArray(obj, size) {}   \
   OBJ OM_FAR *NewAdd() { return (OBJ OM_FAR *)StoreObjArray::NewAdd(); }   \
   OBJ OM_FAR &operator[](int i) const { return (OBJ OM_FAR &)Get(i); }     \
};



class OM_EXPORT StoreObjArray
{
private:
   PStoreObj OM_FAR * elems;
   PStoreObj          seed;
   PStoreObj          endobj;
   int                maxx;
   int                next;
   int                curr;

   void  Init(RStoreObj, int size);
   void  Load();

public:
   StoreObjArray(RStoreObj, int size = 100);
   ~StoreObjArray();

   void       Save();
   T_F        Add(PStoreObj);
   PStoreObj  NewAdd();
   RStoreObj  Get(int) const;
   int        End() const { return next; }
};

#endif
