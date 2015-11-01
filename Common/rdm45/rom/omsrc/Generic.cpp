//-----------------------------------------------------
//  GENERIC.CPP 
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <storedb.hpp>
#include <storeobj.hpp>
#include <generic.hpp>

#ifdef OM_RDS
#include <hlfproto.h>
#endif

  GenericObj ::
~GenericObj()
{ 
   if (recdata) 
      delete recdata; 
}
 
    void
  GenericObj ::
AllocateRecData()
{
   // force the generic object to have a valid RecType
   if (!rectype)
   {
      ROMERROR(S_INVRTYPE);
      return;
   }

   if (recdata) 
      delete recdata;

   // allocate a buffer for the record data
   if ((recdata = (Pvoid) new OM_FAR char[RecDataSize()]) == NULL)
   {
      ROMERROR(S_NOMEMORY);
      SetStatus( S_NOMEMORY );
   }
}

    Pvoid
  GenericObj ::
RecData()
{
	if (!recdata)
		AllocateRecData();
	return recdata;
}

  GenericMemObj ::
~GenericMemObj()
{ 
   if (mmsetobj) 
      delete mmsetobj; 
}

    void
  GenericMemObj ::
AllocateRecData()
{
   if (mmsetobj)
      delete mmsetobj;

   if ((mmsetobj = new OM_FAR MMSetObj(((NetworkNav *)pNav)->Set(), GetDb())) == NULL)
   {
      ROMERROR(S_NOMEMORY);
      SetStatus( S_NOMEMORY );
      return;
   }

   USHORT max_size = 0;

   // iterate thru all set members finding the largest
   for (short idx = 0; idx < mmsetobj->MemCount(); idx++)
   {
      if ((*mmsetobj)[idx]->recdatasize > max_size) 
         max_size = (*mmsetobj)[idx]->recdatasize;
   }

   if (recdatasize < max_size)
   {
      recdatasize = max_size;

      if (recdata) 
         delete recdata;

      // allocate a buffer for the record data
      if ((recdata = (Pvoid) new OM_FAR char[recdatasize]) == NULL)
      {
         ROMERROR(S_NOMEMORY);
         SetStatus( S_NOMEMORY );
      }
   }
}

    RStoreObj
  GenericMemObj ::
Read()
{
#ifdef OM_RDS
   DB_ADDR  dba     = GetDba();
   REC_TYPE rectype = 0;
   short lminfo = GetDb()->GetTask()->LockModeInfo();
   SetStatus( hlf_Read(&dba, &rectype, lminfo, RecDataSize(), RecData(), GetDb()->GethDb()) );
   SetDba(dba);
   SetRecType(rectype);
#else // OM_RDM
   StoreObj::Read();
   if (Okay())
   {
      REC_TYPE Rtype;
      GetDb()->crtype(&Rtype);
      SetRecType(Rtype);
   }
#endif

   return *this;
}

    void
  GenericMemObj ::
SetRecType(REC_TYPE rectype)
{
   // make sure mmsetobj has been setup
   GetMMSetObj();

   for (short ct = 0; ct < mmsetobj->MemCount(); ct++) 
   {
      if ((*mmsetobj)[ct]->rectype == rectype) 
      {
         index = ct;
         break;
      }
   }
}


