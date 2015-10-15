//-----------------------------------------------------
//  DBADDR.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <dbaddr.hpp>

  DbAddr ::
DbAddr(DBA_FILENO file, DBA_SLOT slot)
{
   Encode(file, slot);
}

    void
  DbAddr ::
Decode(PDBA_FILENO file, PDBA_SLOT slot) const
{
   if (file && slot)
      d_decode_dba(dba, file, slot);
}

    void
  DbAddr ::
Encode(DBA_FILENO file, DBA_SLOT slot)
{
   if (file && slot)
      d_encode_dba(file, slot, &dba);
   else
      dba = NULL_DBA;
}
