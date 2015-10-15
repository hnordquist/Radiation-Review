//---------------------------------------------------------
//  DBADDR.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _DBADDR_HPP_
#define _DBADDR_HPP_

//---------------------------------------------------------
// The Arguments to encode and decode are different for
// Velocis, RDM321 and RDM330+.
//---------------------------------------------------------

#if defined(OM_RDS)
   typedef long   DBA_SLOT;
   typedef Plong  PDBA_SLOT;
   typedef short  DBA_FILENO;
   typedef Pshort PDBA_FILENO;
#endif

#if defined(OM_RDM)
# ifdef V321
   typedef long   DBA_SLOT;
   typedef Plong  PDBA_SLOT;
   typedef int    DBA_FILENO;
   typedef Pint   PDBA_FILENO;
#   else // V330 +
   typedef ULONG  DBA_SLOT;
   typedef PULONG PDBA_SLOT;
   typedef short  DBA_FILENO;
   typedef Pshort PDBA_FILENO;
#   endif
#endif


OMCLASS(DbAddr);

class OM_EXPORT DbAddr
{
protected:
   DB_ADDR  dba;

public:
            DbAddr(CRDbAddr dbaddr) : dba(dbaddr.dba) {}
            DbAddr(DB_ADDR dbaddr = NULL_DBA) : dba(dbaddr) {}
            DbAddr(DBA_FILENO file, DBA_SLOT slot);

   void     Encode(DBA_FILENO file,  DBA_SLOT slot);
   void     Decode(PDBA_FILENO file, PDBA_SLOT slot) const;

   DB_ADDR  GetDba() const
               { return dba; }
   void     SetDba(DB_ADDR dbaddr)
               { dba = dbaddr; }
   RDbAddr  operator = (CRDbAddr dbaddr)
               { dba = dbaddr.dba; return *this; }
   RDbAddr  operator = (DB_ADDR dbaddr)
               { dba = dbaddr; return *this; }
};


#endif // _DBADDR_HPP_
