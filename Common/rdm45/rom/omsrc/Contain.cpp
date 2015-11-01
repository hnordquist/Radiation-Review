//-----------------------------------------------------
//  Contain.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <contain.hpp>


  StoreObjArray :: 
StoreObjArray(RStoreObj seedObj, int size)
{
   Init(seedObj, size);
}


// size is the number of valid objects allowed, there is always one more
// for a terminator
    void 
  StoreObjArray :: 
Init(RStoreObj seedObj, int size)
{
   // Allocate enough pointer space for size + a terminator object   
   elems = (PStoreObj OM_FAR *)new OM_FAR char [(size+1) * sizeof(PStoreObj)];
   maxx = size;      // one is used for terminator
   next = 0;
   curr = 0;
   seed = &seedObj;
   Load();
}


    void 
  StoreObjArray :: 
Load()
{
   int i = 0;

   elems[i] = seed->Clone();   // This is the main use of the seed object
                               // to know how to clone itself

   if (!elems[i])  
      return;     // probably forgot to define CLONE...

   DbAddr dba(elems[i]->GetDba());
   elems[i]->GotoDirect(dba);
   for (next = i+1; next <= maxx && elems[i]->Okay(); i++, next = i + 1)
   {
      elems[next] = elems[i]->Clone();   // Copy to next element
      elems[next]->operator++();         // Navigate to next 
   }
   next--;

   // If we reached the end of "maxx", it is necessary to create
   // a terminating object, endobj will always point to the next object
   if (next > maxx)
   {
      endobj = seed->Clone();     // we will use an end object to indicate
      elems[next] = endobj;       // the end of the array using the extra 
                                  // element
   }
   else        // otherwise we can use the obj just created as the endobj
   {
      endobj = elems[next];
      elems[next] = 0;
   }

   endobj->SetStatus(S_OUTOFRANGE);
}


    // check next for valid end of loop in get();
  StoreObjArray :: 
~StoreObjArray()
{
   for (int i=0; i  < next && elems[i]->Okay(); i++)
   {
      if (elems[i])   
         delete elems[i];
   }

   if (endobj) 
   {
      delete endobj;
      endobj = (PStoreObj)0;
   }
   
   if (elems) 
   {
      delete [] elems;
      elems = (PStoreObj OM_FAR *)0;
   }
}


    void 
  StoreObjArray :: 
Save()
{
   for (int i=0; i  < next && elems[i]->Okay(); i++)
   {
      if (elems[i])   
         elems[i]->Write();
   }
}


    RStoreObj  
  StoreObjArray :: 
Get(int idx) const
{
   if (idx >= 0 && idx < next)
      return *elems[idx];
   else 
      return *endobj;
}


    PStoreObj  
  StoreObjArray :: 
NewAdd()
{
   PStoreObj pB=0;

   if (next <= maxx)
   {   
      pB = endobj->Clone();
      if (pB)
      {   
         if (Add(pB) == True)
         {
            pB->SetDefaultValues(); 
            pB->NewObj();
         }
         else
         {
            delete pB;
            pB = 0;
         }
      }
   }
   return pB;
}


    T_F 
  StoreObjArray :: 
Add(PStoreObj pB)
{
   if (next < maxx)
   {   
      elems[next++] = pB;
      elems[next] = endobj;   
      return True;
   }

   return False;
}
