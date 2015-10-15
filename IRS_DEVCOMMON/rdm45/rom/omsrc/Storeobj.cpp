//-----------------------------------------------------
//  STOREOBJ.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <storedb.hpp>
#include <storeobj.hpp>
#include <keyobj.hpp>
#include <nav.hpp>

#ifdef OM_RDS
#  include <hlfproto.h>
#  ifdef SETOOLKIT
#    include <parmsend.hpp>
#    include <parmrecv.hpp>
#  endif
#endif

#ifdef OM_RDM
extern "C" {
#undef NO_DT_COVER
#include <internal.h>
#undef NEXT
#undef LAST
#undef FIRST
}
#endif


//-----------------------------------------------------
// StoreObj Routines
//-----------------------------------------------------

// Create a StoreObj with Direct Default navigation
  StoreObj ::
StoreObj(PStoreDb pdb) : VistaObj(pdb)
{
   pNav = new OM_FAR DirectNav();
}

// Create a StoreObj with NetWork Default navigation
  StoreObj ::
StoreObj(CRSetObj set, PStoreDb pdb) : VistaObj(pdb)
{
   pNav = new OM_FAR NetworkNav(set);
}

// Create a StoreObj with Relational Default navigation
  StoreObj ::
StoreObj(CRKeyObj key, PStoreDb pdb) : VistaObj(pdb)
{
   pNav = new OM_FAR RelationalNav(key);
}

  StoreObj ::
StoreObj(CRStoreObj SO)
{
   pNav = 0L;
   *this = SO;
}

  StoreObj ::
~StoreObj()
{
   if (pNav)
      pNav->EndUse();
}

#if defined(OM_RDS) && defined(SETOOLKIT)
    seParmSend&
  StoreObj::
SendParms(seParmSend& _send)
{
   REC_TYPE      rectype = RecType();
   NavigatorType navtype = NavType();
   RDM_DB        hDb = GetDb()->GethDb();

   _send << dba;

#ifndef OM_SERVER
   _send << hDb << rectype << (long)navtype;
   if (navtype == Relational)
      _send << ((RelationalNav *)(pNav))->Key();
   else if (navtype == Network)
      _send << ((NetworkNav *)(pNav))->Set();
#endif

   _send.Put(RecData(), seDataDescriptor(rectype, hDb));
   return _send;
}

    seParmReceive&
  StoreObj::
ReceiveParms(seParmReceive& _receive)
{
   _receive >> dba;

#ifdef OM_SERVER
   // need to make the server StoreObj match the Client StoreObj
   REC_TYPE      rectype;
   NavigatorType navtype;
   RDM_DB        hDb;

   _receive >> hDb >> rectype >> (long&)navtype;

   GetDb()->SethDb(hDb);
   if (navtype == Relational)
   {
      FLD_TYPE key;
      _receive >> key;
      if (NavType() != Relational)
         SwitchNav(KeyObj(key));
   }
   else if (navtype == Network)
   {
      SET_TYPE set;
      _receive >> set;
      if (NavType() != Network)
         SwitchNav(SetObj(set));
   }

   if (!RecType())
      SetRecType(rectype);
#endif

   _receive.Get(RecData(), RecDataSize());
   return _receive;
}
#endif

    Pvoid
  StoreObj::
RecData() 
{
   //NOTE: this is being left in here for compatibility with ROM < 2.1
   //  you should redefine RecData() in your class because this method
   //  is not portable to all machines.
   return (Pvoid)((Pchar)this + sizeof(*this));
}

    USHORT
  StoreObj::
RecDataSize() const
{
   return (USHORT)GetDb()->rec_len(RecType());
}


//-----------------------------------------------------
// PROTECTED FUNCTIONS
//-----------------------------------------------------

    RStoreObj
  StoreObj ::
Goto(NavCtrl nc)
{
   return pNav->Navigate(nc, *this);
}


    RStoreObj
  StoreObj ::
Goto(CRKeyObj key, NavCtrl nc)
{
   // if a keyobj is used to do a Goto, disregard the current naviagation method
   RelationalNav nav(key);
   return nav.Navigate(key, nc, *this);
}


// This GotoMember Passes the Owner,
// Owner is used to set currency when FIRST or NEXT.
// When NEXT and PREV are called CsmSet() is used to set
// currency This could actually change currency.
    RStoreObj
  StoreObj ::
GotoMember(RStoreObj owner, SET_TYPE set, NavCtrl nc)
{
   return ((NetworkNav *)pNav)->Navigate(owner, set, nc, *this);
}


// This GotoMember DOES NOT Pass the Owner,
// Owner is always set by CsmSet()
// Therefore, FIRST and LAST are only valid when the object has
// been a member before, Except in the case of the System Set
    RStoreObj
  StoreObj ::
GotoMember(SET_TYPE set, NavCtrl nc)
{
   return ((NetworkNav *)pNav)->Navigate(set, nc, *this);
}


    short
  StoreObj ::
MakeOwner(SET_TYPE set, CPchar lmode)
{
   DB_ADDR dba;

   if (RecType() == SYSTEM)
      SetStatus( S_OKAY );
   else if (Okay())
   {
      if ((dba = GetDba()) == NULL_DBA)
         SetStatus( S_INVADDR );
      else
      {
// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
         short lminfo = GetDb()->GetTask()->LockModeInfo();
         SetStatus( hlf_MakeOwner(dba, set, lmode, lminfo, GetDb()->GethDb()) );
#else // OM_RDM
         // Check to make sure this record is not already the current owner
         //  this is required for retaining proper set ordering 
         DB_ADDR currown;
         GetDb()->csoget(set, (PDB_ADDR)&currown);
         if (dba != currown) 
            SetStatus( GetDb()->csoset(set, &dba) );
/* Future development... #if defined(OM_RDS) && defined(OM_SERVER)
         short lminfo = GetDb()->GetTask()->LockModeInfo();
         if ((*lmode == 'r' && OM_IMPLICIT_READS(lminfo)) || (*lmode == 'w' && OM_IMPLICIT_WRITES(lminfo)))
            if (!ILockSet(set, lmode))
               SetStatus( S_UNAVAIL );
*/
#endif
      }
   }

   return GetStatus();
}


    T_F
  StoreObj ::
ConnectTo(RStoreObj rO, SET_TYPE set)
{
   if (!Okay())
      return False;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   short lminfo = GetDb()->GetTask()->LockModeInfo();
   SetStatus( hlf_ConnectTo(rO.GetDba(), GetDba(), set, lminfo, GetDb()->GethDb()) );
#else // OM_RDM
   if (rO.MakeOwner(set, "w") != S_OKAY)
      return False;

   if (CrSet())
      SetStatus( GetDb()->connect(set) );
#endif

   return Okay();
}


// the StoreObj & passed is the owner object "type", it is needed to
// pass the correct set but not used in this function.
    T_F
  StoreObj ::
DisConFrom(RStoreObj , SET_TYPE set)
{
   if (!Okay())
      return False;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   short lminfo = GetDb()->GetTask()->LockModeInfo();
   SetStatus( hlf_DisConFrom(GetDba(), set, lminfo, GetDb()->GethDb()) );
#else // OM_RDM
   CsmSet(set, "w");
   if (Okay())
      SetStatus( GetDb()->discon(set) );
#endif
   if (GetStatus() == S_EOS)
      SetStatus( S_OKAY );

   return (Okay());
}


// Error: returns -1
// StoreObj & is not used, but for the macro which calls this function
// to not complain when compiled, it needs to be passed
    long
  StoreObj ::
MemCount(CRStoreObj , SET_TYPE set)
{
   long count = -1;
   short stat;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   short lminfo = GetDb()->GetTask()->LockModeInfo();
   if ((stat = hlf_MemCount(GetDba(), set, lminfo, &count, GetDb()->GethDb())) != S_OKAY)
      count = -1;
#else // OM_RDM
   if (MakeOwner(set) == S_OKAY)
      stat = GetDb()->members(set, &count);
#endif

   SetStatus(stat);
   return count;
}


    RStoreObj
  StoreObj ::
NewMember(RStoreObj rO, SET_TYPE set)
{
// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   Pvoid data = (RecDataSize() > 0 ? RecData() : NULL);
   short lminfo = GetDb()->GetTask()->LockModeInfo();
   SetStatus( hlf_NewMember(&dba, RecType(), data, rO.GetDba(), set, lminfo, GetDb()->GethDb()) );
   if (Okay())
      OnNewObj();
#else // OM_RDM
   if (NewObj().Okay())
      ConnectTo(rO, set);
#endif

   return *this;
}


    DbAddr
  StoreObj ::
CrGet()
{
   if (Okay()) 
      SetStatus( GetDb()->crget(&dba) );
   return DbAddr(dba);
}


    T_F
  StoreObj ::
CrSet()
{
   if (Okay()) 
      SetStatus( GetDb()->crset(&dba) );
   return Okay();
}


    T_F
  StoreObj ::
CsmSet(SET_TYPE set, CPchar lmode)
{
   if (!Okay())
      return False;

   PStoreDb pDb = GetDb();

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   short lminfo = pDb->GetTask()->LockModeInfo();
   SetStatus( hlf_CsmSet(GetDba(), set, lmode, lminfo, pDb->GethDb()) );
#else // OM_RDM

/* Future development... #if defined(OM_RDS) && defined(OM_SERVER)
   short stat;

   if (CrSet() && lmode)
   {
      short lminfo = pDb->GetTask()->LockModeInfo();
      if ((*lmode == 'r' && OM_IMPLICIT_READS(lminfo)) || (*lmode == 'w' && OM_IMPLICIT_WRITES(lminfo)))
         if ((stat = pDb->crslock(set, lmode)) != S_OKAY && stat != S_LOCKED)
            SetStatus( S_UNAVAIL );
   }
#endif
*/

   SetStatus( pDb->csmset(set, &dba) );
#endif

   return Okay();
}


//-----------------------------------------------------
// DIRECT ACCESS SPECIFIC FUNCTION
//-----------------------------------------------------

    RStoreObj
  StoreObj ::
GotoDirect(CRDbAddr d)   // Direct Navigation function
{
   SetDba(d.GetDba());
   if (GetDba() == NULL_DBA)
      SetStatus( S_NOTFOUND );
   else
   {
      SetStatus( S_OKAY );
      CrSet();
      Read();
   }

   return *this;
}


// Read() reads the current record into this object
    RStoreObj
  StoreObj ::
Read()
{
   if (!Okay())
      return *this;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   REC_TYPE rectype = RecType();
   short lminfo = GetDb()->GetTask()->LockModeInfo();
   SetStatus( hlf_Read(&dba, &rectype, lminfo, RecDataSize(), RecData(), GetDb()->GethDb()) );
   SetRecType(rectype);
#else // OM_RDM
   // Note: this is tricky.  A CrGet() would read into this object whatever the
   //   current record is.  A CrSet() would read into this object whatever is
   //   at this object's dba.
   CrGet();

/* Future development... #if defined(OM_RDS) && defined(OM_SERVER)
   if (Okay() && OM_IMPLICIT_READS(GetDb()->GetTask()->LockModeInfo()))
      if (!ILock("r"))
         SetStatus( S_UNAVAIL );
#endif
*/

   if (Okay())
   {
      REC_TYPE rectype = (RecType() ? RecType() : CurrRecType());
      SetStatus( GetDb()->recread(RecData(), rectype) );
   }
#endif

   if (Okay())
      OnRead();

   return *this;
}


//-----------------------------------------------------
// PUBLIC FUNCTIONS
//-----------------------------------------------------
    void
  StoreObj ::
SwitchNav()
{
   if (pNav)
      pNav->EndUse();
   pNav = new OM_FAR DirectNav();
}

    void
  StoreObj ::
SwitchNav(CRSetObj set)
{
   if (pNav)
      pNav->EndUse();
   pNav = new OM_FAR NetworkNav(set);
}

    void
  StoreObj ::
SwitchNav(CRKeyObj key)
{
   if (pNav)
      pNav->EndUse();
   pNav = new OM_FAR RelationalNav(key);
}


    RStoreObj
  StoreObj ::
NewObj()
{
// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   Pvoid data = (RecDataSize() > 0 ? RecData() : NULL);
   SetStatus( hlf_NewObj(&dba, RecType(), data, GetDb()->GethDb()) );
#else // OM_RDM
   REC_TYPE rectype = (RecType() ? RecType() : CurrRecType());
   if (RecDataSize() > 0)
      SetStatus( GetDb()->fillnew(rectype, RecData()) );
   else
      SetStatus( GetDb()->makenew(rectype) );
   CrGet();
#endif

   if (Okay())
      OnNewObj();

   return *this;
}


    RStoreObj
  StoreObj ::
Write()
{
   if (!Okay())
      return *this;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   Pvoid data = (RecDataSize() > 0 ? RecData() : NULL);
   short lminfo = GetDb()->GetTask()->LockModeInfo();
   SetStatus( hlf_Write(GetDba(), RecType(), lminfo, data, GetDb()->GethDb()) );
#else // OM_RDM

   CrSet();

/* Future development... #if defined(OM_RDS) && defined(OM_SERVER)
   if (Okay() && OM_IMPLICIT_WRITES(GetDb()->GetTask()->LockModeInfo()))
      if (!ILock("w"))
         SetStatus( S_UNAVAIL );
#endif
*/

   if (Okay())
   {
      REC_TYPE rectype = (RecType() ? RecType() : CurrRecType());
      if (RecDataSize() > 0)
         SetStatus( GetDb()->recwrite(RecData(), rectype) );
      else
         SetStatus( GetDb()->recwrite(NULL, rectype) );
   } 
#endif

   if (Okay())
      OnWrite();

   return *this;
}


    RStoreObj
  StoreObj ::
Delete()
{
   if (!Okay())
      return *this;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   short lminfo = GetDb()->GetTask()->LockModeInfo();
   SetStatus( hlf_Delete(GetDba(), lminfo, GetDb()->GethDb()) );
#else // OM_RDM
   CrSet();

/* Future development... #if defined(OM_RDS) && defined(OM_SERVER)
   if (Okay() && OM_IMPLICIT_WRITES(GetDb()->GetTask()->LockModeInfo()))
      if (!ILock("w"))
         SetStatus( S_UNAVAIL );
#endif
*/

   if (Okay())
      SetStatus( GetDb()->d_delete() );
#endif

   if (Okay())
   {
      SetDba(NULL_DBA);
      OnDelete();
   }

   return *this;
}


//-----------------------------------------------------
// RELATIONAL MODEL SPECIFIC FUNCTION
//-----------------------------------------------------
    RStoreObj
  StoreObj ::
Find(CRKeyObj Key)
{
   if (Key.HasValue())
   {
// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
      short lminfo = GetDb()->GetTask()->LockModeInfo();
      SetStatus( hlf_Find(&dba, Key.Fld(), Key.Value(), RecType(), lminfo, RecDataSize(), RecData(), GetDb()->GethDb()) );
      if (Okay())
         OnRead();
#else // OM_RDM
      SetStatus( GetDb()->keyfind(Key.Fld(), Key.Value()) );
      Read();
#endif
   }
   return *this;
}

    RStoreObj
  StoreObj ::
operator=(CRStoreObj SO)   // Assignment Operator
{
   if (pNav)
      pNav->EndUse();
   pNav = SO.pNav;
   pNav->AddUse();
   SetStatus(SO.GetStatus());
   SetDba(SO.GetDba());
   SetDb(SO.GetDb());
   // this task has been left to the application developer
   //om_memcpy(RecData(), SO.RecData(), RecDataSize());
   return *this;
}


    RStoreObj
  StoreObj ::
operator[](NavCtrl nc)
{
   return pNav->Navigate(nc,*this);
}


// Post operators for new C++
#ifndef CPP20
   RStoreObj
  StoreObj ::
operator++(int)   // increment to next
{
   return pNav->Navigate(NEXT, *this);
}

   RStoreObj
  StoreObj ::
operator--(int)   // decrement to prev
{
   return pNav->Navigate(PREV, *this);
}
#endif


// post for old

   RStoreObj
  StoreObj ::
operator++()    // increment to next
{
   return pNav->Navigate(NEXT, *this);
}


   RStoreObj
  StoreObj ::
operator--()     // decrement to prev
{
   return pNav->Navigate(PREV, *this);
}

    PStoreObj OMLOADDS
  StoreObj ::
Clone()
{
   ROMERROR(S_CLONE);
   return 0L;
}


//-----------------------------------------------------
// Public, but should be used by operator>> and operator<<
// Which are created by MEMBEROF and OWNEROF macros
//-----------------------------------------------------

    RStoreObj
  StoreObj ::
FromOwner(RStoreObj owner, SET_TYPE set)
{
   return GotoMember(owner, set, FIRST);
}


    RStoreObj
  StoreObj ::
FromMember(RStoreObj member, SET_TYPE set)
{
// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   REC_TYPE rectype = RecType();
   short lminfo = GetDb()->GetTask()->LockModeInfo();
   SetStatus( hlf_FromMember(member.GetDba(), &dba, set, &rectype, lminfo, RecDataSize(), RecData(), GetDb()->GethDb()) );
   SetRecType(rectype);
   if (Okay())
      OnRead();
#else // OM_RDM
   // make member the current member of this set
   member.CsmSet(set);
   if (member.Okay())
   {
      // Make the owner the current record
      SetStatus( GetDb()->setro(set) );
      Read();
   }
   else
      SetStatus( member.GetStatus() );
#endif
   return *this;
}


    T_F
  StoreObj ::
IsMember(SET_TYPE set)
{
   short stat = S_EOS;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   stat = hlf_IsMember(GetDba(), set, GetDb()->GethDb());
#else // OM_RDM
   if (CrSet())
      stat = GetDb()->ismember(set);
#endif
   return ((stat == S_OKAY) ? True : False);
}


    T_F
  StoreObj ::
IsMember(RStoreObj owner, SET_TYPE set)
{
   short stat = S_EOS;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   stat = hlf_IsMemberOfOwner(GetDba(), set, owner.GetDba(), GetDb()->GethDb());
#else // OM_RDM
   DB_ADDR  dba;
   PStoreDb pDb = GetDb();
   if (CrSet() && (stat = pDb->ismember(set)) == S_OKAY)
   {
      pDb->findco(set);
      pDb->crget(&dba);

      stat = (dba == owner.GetDba()) ? S_OKAY : S_EOS;
   }
#endif
   return ((stat == S_OKAY) ? True : False);
}


    T_F
  StoreObj ::
IsOwner(SET_TYPE set)
{
   short stat = S_EOS;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   stat = hlf_IsOwner(GetDba(), set, GetDb()->GethDb());
#else // OM_RDM
   if (CrSet())
      stat = GetDb()->isowner(set);
#endif
   return ((stat == S_OKAY) ? True : False);
}


    T_F
  StoreObj ::
IsOwner(RStoreObj member, SET_TYPE set)
{
   short stat = S_EOS;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   stat = hlf_IsOwnerOfMember(GetDba(), set, member.GetDba(), GetDb()->GethDb());
#else // OM_RDM
   DB_ADDR  dba;
   PStoreDb pDb = GetDb();
   member.CrSet();
   if (Okay() && (stat = pDb->ismember(set)) == S_OKAY)
   {
      pDb->findco(set);
      pDb->crget(&dba);

      stat = (dba == GetDba()) ? S_OKAY : S_EOS;
   }
#endif
   return ((stat == S_OKAY) ? True : False);
}


//-----------------------------------------------------
// MULTI-USER LOCKING FUNCTIONS
//-----------------------------------------------------
    T_F
  StoreObj ::
Lock(CPchar ltype)
{
#ifdef OM_RDS
   short stat = GetDb()->rtlock(RecType(), ltype);
#else // OM_RDM
   REC_TYPE rectype = (RecType() ? RecType() : CurrRecType());
   short stat = GetDb()->reclock(rectype, ltype);
#endif

   // S_NOTFREE gets returned if your current lock request is identical
   //    to the current lock you have on this record type.
   //    We are choosing to not treat this as an error with ROM.
   if (stat == S_NOTFREE)
      stat = S_OKAY;

   return (stat == S_OKAY) ? True : False;
}


    T_F
  StoreObj ::
FreeLock()
{
#ifdef OM_RDS
   short stat = GetDb()->rtfree(RecType());
#else // OM_RDM
   REC_TYPE rectype = (RecType() ? RecType() : CurrRecType());
   short stat = GetDb()->recfree(rectype);
#endif

   // if it is not locked, it's okay
   if (stat == S_NOTLOCKED)
      stat = S_OKAY;

   return (stat == S_OKAY) ? True : False;
}


#if defined(OM_RDS)
    T_F
  StoreObj ::
ILock(CPchar ltype)
{
   short stat = GetDb()->dbalock(GetDba(), ltype);
   return (stat == S_OKAY || stat == S_LOCKED || stat == S_ILLDOWNG) ? True : False;
}


   T_F
  StoreObj ::
IFreeLock()
{
   short stat = GetDb()->dbafree(GetDba());

   // if it is not locked, it's okay
   if (stat == S_NOTLOCKED)
      stat = S_OKAY;

   return (stat == S_OKAY) ? True : False;
}


    T_F
  StoreObj ::
ILockSet(SET_TYPE set, CPchar ltype)
{
   short stat = GetDb()->cslock(set, ltype);
   return (stat == S_OKAY || stat == S_LOCKED || stat == S_ILLDOWNG) ? True : False;
}


    T_F
  StoreObj ::
IFreeLockSet(SET_TYPE set)
{
   short stat = GetDb()->csfree(set);

   // if it is not locked, it's okay
   if (stat == S_NOTLOCKED)
      stat = S_OKAY;

   return (stat == S_OKAY) ? True : False;
}
#endif // OM_RDS


#if defined(OM_RDM)
    char
  StoreObj ::
LockStatus()
{
   char s;
   REC_TYPE rectype = (RecType() ? RecType() : CurrRecType());
   GetDb()->reclstat(rectype, &s);
   return s;
}

    T_F
  StoreObj ::
IsRLBSet()
{
   return T_F(RLBStatus() == S_LOCKED);
}

    T_F
  StoreObj ::
RLBSet()
{
   CrSet();
   return T_F(GetDb()->rlbset() == S_OKAY);
}

    T_F
  StoreObj ::
RLBClear()
{
   CrSet();
   return T_F(GetDb()->rlbclr() == S_OKAY);
}

    short
  StoreObj ::
RLBStatus()
{
   CrSet();
   return GetDb()->rlbtst();
}
#endif


//--------------------------------------------------------
// Fill a memory area passed with contents of the record table
// for this record type
//--------------------------------------------------------
    void
  StoreObj ::
GetRecEntry(Pvoid pRt)
{
   REC_TYPE rectype = (RecType() ? RecType() : CurrRecType());
   GetDb()->GetRecEntry(pRt, rectype);
}

