//---------------------------------------------------------
//  POLY.HPP Polymorphic behaviour managing class
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _POLY_HPP_
#define _POLY_HPP_

#include "generic.hpp"

OMCLASS(StoreObj);
OMCLASS(StoreDb);

OMCLASS(Polymorph);


class OM_EXPORT Polymorph : public GenericMemObj
{
private:
   virtual void     SetRecType(REC_TYPE);
   virtual REC_TYPE CurrRecType() { return pCurr->RecType(); }
   void             CreateList();

protected:
   PStoreObj         pCurr;   // Current StoreObj
   PStoreObj OM_FAR *ppInfo;  // ptr to ptr to the first StoreObj maintained
   short             Num;     // Number of multi-members

protected:
   Polymorph(SET_TYPE set, short Members, PStoreDb pdb = 0)
      : GenericMemObj(set, pdb), pCurr((PStoreObj)NULL), ppInfo((PStoreObj OM_FAR *)NULL), Num(Members) {}
   Polymorph(SET_TYPE set, PStoreDb pdb)
      : GenericMemObj(set, pdb), pCurr((PStoreObj)NULL), ppInfo((PStoreObj OM_FAR *)NULL), Num(0) {}

   PStoreObj OM_FAR *GetMemberInfo() const { return ppInfo; }

   T_F            Select(REC_TYPE Rtype);

   void           Init() {}  // no longer required (ROM 3.0)

public:
   virtual       ~Polymorph();

   // Each member of the set must be included in the subclass of
   // Polymorph and Registered in the constructor of Polymorph
   T_F            Register(RStoreObj);

   // Use the Smart pointer to operate on the current storable
   //  object of the Polymorph
   PStoreObj      operator->() const { return pCurr;  }
   PStoreObj      GetObj()     const { return pCurr;  }
   PStoreObj      GetObj(REC_TYPE Rtype);

   // Syncs dba and status of Polymorph object with current storable
   //  object of the Polymorph.
   void           Sync() { ((RStoreObj) *this) = *pCurr; }
};

#endif
