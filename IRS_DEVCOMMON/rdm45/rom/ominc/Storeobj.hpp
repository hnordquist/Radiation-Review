//-----------------------------------------------------
//  STOREOBJ.HPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _STOREOBJ_HPP_
#define _STOREOBJ_HPP_

#include "vpp.hpp"
#include "dbaddr.hpp"
#include "vistaobj.hpp"
#include "storedb.hpp"
#include "navmacro.hpp"
#include "keyobj.hpp"
#include "setobj.hpp"
#include "nav.hpp"

class OM_EXPORT StoreTask;

OMCLASS(StoreObj);
OMCLASS(OmAutoLoad);
OMCLASS(Polymorph);
OMCLASS(OmBlob);
OMTYPE(RECORD_ENTRY)


//-----------------------------------------------------
//  StoreObj
//-----------------------------------------------------
// Adds Ability to the VistaObj to navigate thru the
// database and load itself into memory
class OM_EXPORT StoreObj : public VistaObj, public DbAddr
{
   // These Classes are able to directly affect StoreObj
   friend RelationalNav;
   friend DirectNav;
   friend NetworkNav;
   friend OmAutoLoad;
   friend Polymorph;
   friend OmBlob;

private:
   virtual void     SetRecType(REC_TYPE) {}
   virtual REC_TYPE CurrRecType() { return RecType(); }

protected:
   PNavigator pNav;            // May be Relational, Network, or Direct

   //-----------------------------------------------------
   // CONSTRUCTORS, protected to inforce inheritence
   //-----------------------------------------------------
   StoreObj(CRSetObj set, PStoreDb pdb = 0L);
   StoreObj(CRKeyObj key, PStoreDb pdb = 0L);
   StoreObj(PStoreDb pdb = 0L);

   //-----------------------------------------------------
   // NETWORK MODEL SPECIFIC FUNCTIONS
   //-----------------------------------------------------
   long   MemCount(CRStoreObj , SET_TYPE set);
   T_F    CsmSet(SET_TYPE set, CPchar mode = "r");
   short  MakeOwner(SET_TYPE set, CPchar mode = "r");

   //-----------------------------------------------------
   // GENERAL FUNCTIONS
   //-----------------------------------------------------
   // Read() reads the current record into this object
   // Protected, so no one reads without having current rec correct
   virtual  RStoreObj Read();

   DbAddr  CrGet();          // Set objects DB_ADDR from current db rec
   T_F     CrSet();          // Set current db rec from this object

   virtual  Pvoid    RecData();
   virtual  USHORT   RecDataSize() const;

   // notification functions
   virtual void   OnDelete() {}
   virtual void   OnNewObj() {}
   virtual void   OnRead()   {}
   virtual void   OnWrite()  {}

public:
            StoreObj(CRStoreObj);
   virtual ~StoreObj();

   virtual  REC_TYPE RecType() const = 0;
   virtual  Pchar OMLOADDS RecName() const    { return ""; }
   virtual  void  OMLOADDS SetDefaultValues() {}

   virtual RStoreObj NewObj();        // create a new Object on disk
   virtual RStoreObj Write();         // set this as the current rec and write it
   virtual RStoreObj Delete();        // set this as the current rec and delete it

   // determine if this object has been loaded
   T_F NotRead() const { return T_F(!Okay() || GetDba() == NULL_DBA); }

   // re-read this object
   RStoreObj  ReRead() { CrSet(); return Read(); }

   // switch the navigation method
   NavigatorType NavType() const { return pNav->NavType(); }
   void          SwitchNav();
   void          SwitchNav(CRKeyObj);
   void          SwitchNav(CRSetObj);

   virtual PStoreObj OMLOADDS Clone();
   RStoreObj  operator=(CRStoreObj);

   RStoreObj  operator[](NavCtrl nc);
   RStoreObj  operator[](CRDbAddr d) { return GotoDirect(d);  }
   RStoreObj  operator[](CRKeyObj k) { return Find(k);  }

   // indexed lookup function
   RStoreObj  Find(CRKeyObj);

#ifndef CPP20                        // if not 2.0 C++ Post operators
   RStoreObj  operator++(int);       // increment to next
   RStoreObj  operator--(int);       // decrement to prev
#endif
                                     // Pre operators for new C++,
   RStoreObj  operator++();          // increment to next
   RStoreObj  operator--();          // decrement to prev

   // navigate via the default navigation method
   RStoreObj  First()                { return Goto(FIRST); }
   RStoreObj  Last()                 { return Goto(LAST);  }
   RStoreObj  Next()                 { return Goto(NEXT);  }
   RStoreObj  Prev()                 { return Goto(PREV);  }
   RStoreObj  Goto(NavCtrl nc);

   // navigate via the relational navigation method 
   //  (regardless of default navigation method)
   RStoreObj  First(CRKeyObj k) { return Goto(k, FIRST); }
   RStoreObj  Last(CRKeyObj k)  { return Goto(k, LAST);  }
   RStoreObj  Next(CRKeyObj k)  { return Goto(k, NEXT);  }
   RStoreObj  Prev(CRKeyObj k)  { return Goto(k, PREV);  }
   RStoreObj  Goto(CRKeyObj k, NavCtrl nc);

#if defined(OM_RDS) && defined(SETOOLKIT) && !defined(OM_COMMONDB)
   virtual seParmSend& SendParms(seParmSend& _send);
   virtual seParmReceive& ReceiveParms(seParmReceive& _receive);
#endif

   //-----------------------------------------------------
   // NETWORK MODEL SPECIFIC FUNCTIONS
   //-----------------------------------------------------
   RStoreObj  GotoMember(RStoreObj rOwner, SET_TYPE Set, NavCtrl nc);
   RStoreObj  GotoMember(SET_TYPE set, NavCtrl nc);

   // FromMember and FromOwner are used in the MEMBEROF and
   // OWNEROF Macros, to create the >> operators
   // they need to be public for that purpose
   // but need not be used directly by the programmer.
   RStoreObj  FromMember(PStoreObj pM, SET_TYPE set) { return FromMember(*pM, set); }
   RStoreObj  FromMember(RStoreObj pM, SET_TYPE set); // now takes a RStoreObj
   RStoreObj  FromOwner (PStoreObj pO, SET_TYPE set) { return FromOwner(*pO, set); }
   RStoreObj  FromOwner (RStoreObj pO, SET_TYPE set); // now takes a RStoreObj

   T_F        IsMember(SET_TYPE set);
   T_F        IsOwner(SET_TYPE set);

   T_F        IsMember(RStoreObj owner, SET_TYPE set);
   T_F        IsOwner(RStoreObj member, SET_TYPE set);

   T_F        DisConFrom(RStoreObj rO, SET_TYPE set);
   T_F        ConnectTo(RStoreObj rO, SET_TYPE set);

   RStoreObj  NewMember(RStoreObj rO, SET_TYPE set);

   //-----------------------------------------------------
   // DIRECT ACCESS SPECIFIC FUNCTION
   //-----------------------------------------------------
   // Direct Navigation function
   RStoreObj  GotoDirect(CRDbAddr);
   RStoreObj  Goto(CRDbAddr dba) { return GotoDirect(dba); }

   //-----------------------------------------------------
   // MULTI-USER LOCKING FUNCTIONS
   //-----------------------------------------------------
   // Table locking functions
   T_F   Lock(CPchar);
   T_F   FreeLock();
   T_F   ReadLock()        { return Lock("r"); }
   T_F   WriteLock()       { return Lock("w"); }

#if defined(OM_RDM) && !defined(OM_COMMONDB)
   // These functions are only available using RDM
   T_F   ExclusiveLock()   { return Lock("x"); }
   T_F   KeepLock()        { return Lock("k"); }

   // Get the lock status of this object's record type
   char  LockStatus();
#endif

#if defined(OM_RDS) && !defined(OM_COMMONDB)
   // Instance locking functions for RDS only
   T_F   ILock(CPchar ltype);
   T_F   IFreeLock();
   T_F   IReadLock()       { return ILock("r"); }
   T_F   IWriteLock()      { return ILock("w"); }

   T_F   ILockSet(SET_TYPE set, CPchar ltype);
   T_F   IFreeLockSet(SET_TYPE set);
   T_F   IReadLockSet(SET_TYPE set)   { return ILockSet(set, "r"); }
   T_F   IWriteLockSet(SET_TYPE set)  { return ILockSet(set, "w"); }
#endif

#if defined(OM_RDM) && !defined(OM_COMMONDB)
   T_F   IsRLBSet();
   T_F   RLBClear();
   T_F   RLBSet();
   short RLBStatus();
#endif

   // Get record table entry
   void  GetRecEntry(Pvoid pRte);
   void  GetRecEntry(PRECORD_ENTRY pRte) { GetRecEntry((Pvoid)pRte); }
};



#endif
