//-----------------------------------------------------
//   SETOBJ.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <setobj.hpp>
#include <storedb.hpp>

  MMSetObj ::
MMSetObj(SET_TYPE set, PStoreDb pDb) : SetObj(set)
{
   Members = pDb->st_memtot(set);
   pInfo = (MMSET_INFO *)new OM_FAR MMSET_INFO[Members];

   short st_members = pDb->st_members(set);

   // iterate thru all set members
   for (short idx = 0; idx < Members; idx++)
   {
      pInfo[idx].rectype = (REC_TYPE)(pDb->mt_record(st_members) + RECMARK);
      pInfo[idx].recdatasize = pDb->rec_len(pInfo[idx].rectype);
      st_members++;
   }
}

