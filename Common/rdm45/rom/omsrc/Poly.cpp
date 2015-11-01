//-----------------------------------------------------
//   POLY.CPP Uses Multi Member Set for Polymorphic behaviour
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <storedb.hpp>
#include <storeobj.hpp>
#include <poly.hpp>


  Polymorph ::
~Polymorph()
{
   if (ppInfo)
      delete [] ppInfo;
}


    void
  Polymorph ::
CreateList()
{
	// need to have db and MMSetObj setup before this works
	if (!GetDb() || !GetMMSetObj())
		return;
		
   // get the number of members of the polymorph set      
   if (!Num)
      Num = GetDb()->st_memtot(GetSet());

   if ((ppInfo = new OM_FAR PStoreObj[Num]) != NULL)
   {
      for (int idx = 0; idx < Num; idx++)
         ppInfo[idx] = 0;
   }
   else
   {
      ROMERROR(S_NOMEMORY);
      SetStatus( S_NOMEMORY );
   }
}


//-------------------------------------------------
// When a Storable object is registered it is added to 
// the list of member objects to track during Multi-
// membered set navigation.  
//-------------------------------------------------
    T_F
  Polymorph :: 
Register(RStoreObj SO)
{
   T_F rtn = False;
   
   if (!ppInfo)
      CreateList();

   for (int idx = 0; idx < Num; idx++)
   {
      if (!ppInfo[idx])
      {   
         pCurr = ppInfo[idx] = &SO;
         rtn = True;
         break;
      }
   }

   return rtn;
}


//----------------------------------------------------------
// Return a pointer to the Current object
//----------------------------------------------------------
    PStoreObj 
  Polymorph ::
GetObj(REC_TYPE rectype) 
{   
   Select(rectype);
   return pCurr;
}


//----------------------------------------------------------
// Look thru the list of managed member objects and make the 
// matching one the current object
//----------------------------------------------------------
    T_F
  Polymorph ::
Select(REC_TYPE rectype)
{
   if (!ppInfo)
      CreateList();

   T_F rtn = False;

   for (int idx = 0; idx < Num; idx++)
   {
      if (ppInfo[idx]->RecType() == rectype)
      {   
         pCurr = ppInfo[idx];
         rtn = True;
         break;
      }
   }

   if (rtn == False)
   {
      pCurr = (PStoreObj)NULL;
      ROMERROR(S_INVRTYPE);
   }
   
   return rtn;
}


    void
  Polymorph ::
SetRecType(REC_TYPE rectype)
{
   if (!Okay())
      return;
      
   if (Select(rectype))
   {
      (RStoreObj)*pCurr = (RStoreObj)*this;
      om_memcpy(pCurr->RecData(), RecData(), pCurr->RecDataSize());
   }

   // call base class to sync it up also 
   GenericMemObj::SetRecType(rectype);
}


