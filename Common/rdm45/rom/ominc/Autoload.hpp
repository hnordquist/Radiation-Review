//---------------------------------------------------------
//  autoload.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _AUTOLOAD_HPP_
#define _AUTOLOAD_HPP_

#include "storeobj.hpp"


// Used to define Simple AutoLoad Objects
class OM_EXPORT OmAutoLoad 
{
private:
   PStoreObj pObj;

protected:
   void  Check();
   
protected:
   OmAutoLoad(PStoreObj pObj);
   OmAutoLoad(PStoreObj pObj, CRKeyObj k);
   OmAutoLoad(PStoreObj pObj, RStoreObj Owner, SET_TYPE set); 

public:
   virtual ~OmAutoLoad() { pObj->Write(); }
};

#endif
