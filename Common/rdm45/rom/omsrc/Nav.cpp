//-----------------------------------------------------
//  NAV.CPP      Navigation objects used in StoreObj Objects
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <storedb.hpp>
#include <storeobj.hpp>
#include <nav.hpp>

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
#include <hlfproto.h>
#endif


//-----------------------------------------------------
// Navigator Routines
//-----------------------------------------------------
    void
  Navigator ::
EndUse()
{
   nUseCount--;
   if (nUseCount <= 0)
      delete (Pvoid) this;
}


//-----------------------------------------------------
// DirectNav Routines
//-----------------------------------------------------
    RStoreObj
  DirectNav ::
Navigate(NavCtrl nc, RStoreObj obj)
{
   if (obj.GetDba() == NULL_DBA)
   {
      if (nc == NEXT)
         nc = FIRST;
      else if (nc == PREV)
         nc = LAST;
   }

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   DB_ADDR dba = obj.GetDba();
   short lminfo = obj.GetDb()->GetTask()->LockModeInfo();
   obj.SetStatus( hlf_DirectNavigation(&dba, obj.RecType(), (short)nc, lminfo, obj.RecDataSize(), obj.RecData(), obj.GetDb()->GethDb()) );
   obj.SetDba(dba);
   if (obj.Okay())
      obj.OnRead();
#else // OM_RDM
   short     stat = S_OKAY;
   PStoreDb  pDb = obj.GetDb();

   if (nc == NEXT || nc == PREV)
   {
      if (obj.CrSet())
         pDb->recset(obj.RecType());
   }

   switch (nc)
   {
      case FIRST: stat = pDb->recfrst(obj.RecType()); break;
      case LAST:  stat = pDb->reclast(obj.RecType()); break;
      case NEXT:  stat = pDb->recnext();              break;
      case PREV:  stat = pDb->recprev();              break;
   }

   obj.SetStatus(stat);
   obj.Read();
#endif

   return obj;
}


//-----------------------------------------------------
// RelationalNav Routines
//-----------------------------------------------------
    RStoreObj
  RelationalNav ::
Navigate(FLD_TYPE keyfld, NavCtrl nc, RStoreObj obj)
{
// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   DB_ADDR dba = obj.GetDba();
   short lminfo = obj.GetDb()->GetTask()->LockModeInfo();
   obj.SetStatus( hlf_KeyNavigation(&dba, obj.RecType(), keyfld, (short)nc, lminfo, obj.RecDataSize(), obj.RecData(), obj.GetDb()->GethDb()) );
   obj.SetDba(dba);
   if (obj.Okay())
      obj.OnRead();
#else // OM_RDM
   short     stat = S_NOTFOUND;
   PStoreDb  pDb = obj.GetDb();

   if (nc == NEXT || nc == PREV)
   {
      if (obj.GetDba() != NULL_DBA && obj.CrSet())
         pDb->curkey();
   }

   switch (nc)
   {
      case FIRST: stat = pDb->keyfrst(keyfld); break;
      case LAST:  stat = pDb->keylast(keyfld); break;
      case NEXT:  stat = pDb->keynext(keyfld); break;
      case PREV:  stat = pDb->keyprev(keyfld); break;
   }

   obj.SetStatus(stat);
   obj.Read();
#endif

   return obj;
}


//-----------------------------------------------------
// NetworkNav Routines
//-----------------------------------------------------
// default network navigation
    RStoreObj
  NetworkNav ::
Navigate(NavCtrl nc, RStoreObj obj)
{
   PStoreDb pDb = obj.GetDb();

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   DB_ADDR dba = obj.GetDba();
   REC_TYPE rectype = obj.RecType();
   short lminfo = pDb->GetTask()->LockModeInfo();
   obj.SetStatus( hlf_GotoMember(&dba, set, &rectype, nc, lminfo, obj.RecDataSize(), obj.RecData(), pDb->GethDb()) );
   obj.SetDba(dba);
   obj.SetRecType(rectype);
   if (obj.Okay())
      obj.OnRead();
#else // OM_RDM
   short     stat;

   if (obj.GetDba() == NULL_DBA)
   {
      // Should only occur if its the system set...and Owner not set
      if (nc == NEXT)      nc = FIRST;
      else if (nc == PREV) nc = LAST;

      // Make sure set has a valid owner
      DB_ADDR tmp;
      stat = pDb->csoget(set, (PDB_ADDR)&tmp);
   }
   else
   {
      obj.CsmSet(set);
      stat = obj.GetStatus();
      if (!obj.Okay())
      {
         stat = S_OKAY;
         if (nc == NEXT)      nc = FIRST;
         else if (nc == PREV) nc = LAST;
      }
   }

   if (stat == S_OKAY)
      DoTheNavigate(nc, obj);
#endif

   return obj;
}


    RStoreObj
  NetworkNav ::
Navigate(SET_TYPE _set, NavCtrl nc, RStoreObj obj)
{
   // it's possible for this to get called with a SET_TYPE that isn't 
   // the same as the one in pNav, so work around it, but don't modify
   // the pNav.
   SET_TYPE tmp = set;
   set = _set;
   Navigate(nc, obj);
   set = tmp;
   return obj;
}


    RStoreObj
  NetworkNav ::
Navigate(RStoreObj owner, SET_TYPE _set, NavCtrl nc, RStoreObj obj)
{
   // it's possible for this to get called with a SET_TYPE that isn't 
   // the same as the one in pNav, so work around it, but don't modify
   // the pNav.
   SET_TYPE tmp = set;
   set = _set;

// Future development... #if defined(OM_RDS) && defined(OM_CLIENT)
#if defined(OM_RDS) 
   PStoreDb pDb = obj.GetDb();
   DB_ADDR dba = obj.GetDba();
   REC_TYPE rectype = obj.RecType();
   short lminfo = pDb->GetTask()->LockModeInfo();
obj.SetStatus( hlf_GotoMemberFromOwner(owner.GetDba(), &dba, set, &rectype, nc, lminfo, obj.RecDataSize(), obj.RecData(), pDb->GethDb()) );
   obj.SetDba(dba);
   obj.SetRecType(rectype);
   if (obj.Okay())
      obj.OnRead();
#else // OM_RDM
   short     stat;

   if (nc == FIRST || nc == LAST)
   {
      // Make sure its the owner of the set
      owner.MakeOwner(set);
      stat = owner.GetStatus();
   }
   else     // NEXT or PREV
   {
      obj.CsmSet(set);
      stat = obj.GetStatus();
      if (!obj.Okay())
      {
         stat = S_OKAY;
         if (nc == NEXT)  nc = FIRST;
         else             nc = LAST;
      }
   }

   if (stat == S_OKAY)
      DoTheNavigate(nc, obj);
#endif

   set = tmp;
   return obj;
}

    RStoreObj
  NetworkNav ::
DoTheNavigate(NavCtrl nc, RStoreObj obj)
{
// Future development... // #if defined(OM_RDM) || (defined(OM_RDS) && defined(OM_SERVER))
#if defined(OM_RDM) 
   PStoreDb  pDb  = obj.GetDb();
   REC_TYPE  type;
   short     stat = S_OKAY;

   while (stat == S_OKAY)
   {
      switch (nc)
      {
         case FIRST:  stat = pDb->findfm(set); break;
         case LAST:   stat = pDb->findlm(set); break;
         case NEXT:   stat = pDb->findnm(set); break;
         case PREV:   stat = pDb->findpm(set); break;
      }                       

      // allow a normal storeobj to be used to navigate a multi-member set
      // by checking the rectype of the next set member found
      if (obj.RecType() && stat == S_OKAY)
      {
         pDb->crtype(&type);
         if (type == obj.RecType()) break;
         else if (nc == FIRST)  nc = NEXT;
         else if (nc == LAST)   nc = PREV;
      }
      else
         break;
   }

   obj.SetStatus(stat);
   obj.Read();
#endif
   return obj;
}

