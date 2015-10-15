//-----------------------------------------------------
//  autoload.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <autoload.hpp>


// Direct object persistence
  OmAutoLoad :: 
OmAutoLoad(PStoreObj obj) : pObj(obj)
{
   pObj->Goto(FIRST); 
   Check();
}


// Relational object persistence
  OmAutoLoad :: 
OmAutoLoad(PStoreObj obj, CRKeyObj key) : pObj(obj)
{
   pObj->Find(key);
   Check();
}


// NetWork object persistence
  OmAutoLoad :: 
OmAutoLoad(PStoreObj obj, RStoreObj owner, SET_TYPE set) : pObj(obj)
{
   pObj->GotoMember(owner, set, FIRST);

   if (!pObj->Okay())
   {
      Check();
      pObj->ConnectTo(owner, set);
   }
}


    void 
  OmAutoLoad :: 
Check()
{
   if (!pObj->Okay())
   {
      // if we couldn't load one, set the default values
      // and Make a new one
      pObj->SetDefaultValues();
      pObj->NewObj();
   } 
}
