//---------------------------------------------------------
//  NAVMACRO.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------

//---------------------------------------------------------
// OBJECT TO OBJECT NAVIGATION MACROS
//---------------------------------------------------------
// These Macros define object to object relationship functions and operators
// The set of macros which follow use operator>> to navigate between
// objects.  Alternative navigation macros are defined below.  The
// alternatives us a NavTo() function in place of operator >>.
// operator<< is replaced by NavToOwner().
// USENAVTO must be #defined to use the alternative macros

#ifndef USENAVTO

#define MEMBEROFNAV(OBJ, SET) \
   OBJ OM_FAR &operator<<(OBJ OM_FAR &y) \
   { \
      ((PStoreObj)&y)->FromMember((PStoreObj)(this), SET); \
      return y; \
   }

#define BLOBMEMBEROFNAV(OBJ, SET) \
   OBJ OM_FAR &operator<<(OBJ OM_FAR &y) \
   { \
      ((PStoreObj)&y)->GotoDirect((RDbAddr)*(this->GetOwner())); \
      return y; \
   }

#define OWNEROFNAV(OBJ,SET) \
   OBJ OM_FAR &operator>>(OBJ OM_FAR &x) \
   { \
      ((PStoreObj)&x)->FromOwner((PStoreObj)this, SET); \
      return x; \
   }

#define OWNEROFBLOBNAV(OBJ, SET) \
   OBJ OM_FAR &operator>>(OBJ OM_FAR &y) \
   { \
      ((POmBlob)&y)->SetOwner(this); \
      return y; \
   }

#define RELATEDTONAV(OBJ,FLDCONST,FLD) \
   OBJ OM_FAR & operator>>(OBJ OM_FAR &rObj) \
   { \
      return (OBJ OM_FAR &)((PStoreObj)&rObj)->Find(KeyObj(FLDCONST,FLD)); \
   }

#define DIRECTREFNAV(OBJ,FLD) \
   OBJ OM_FAR & operator>>(OBJ OM_FAR &r) \
   { \
      ((PStoreObj)&r)->GotoDirect(DbAddr(FLD)); return r; \
   }

#define REFERENCETONAV(OBJ,FLD) \
   OBJ OM_FAR & operator>>(OBJ OM_FAR &r) \
   { \
      ((PStoreObj)&r)->GotoDirect(DbAddr(FLD)); \
      return r; \
   }

#else // USENAVTO

#define MEMBEROFNAV(OBJ, SET) \
   OBJ OM_FAR &NavToOwner(OBJ OM_FAR & y, SET_TYPE set = SET) \
   { \
      (OBJ OM_FAR &)((PStoreObj)&y)->FromMember((PStoreObj)(this), set); \
      return y; \
   }

#define BLOBMEMBEROFNAV(OBJ, SET) \
   OBJ OM_FAR &NavToOwner(OBJ OM_FAR &y, SET_TYPE set = SET) \
   { \
      y.GotoDirect((RDbAddr)*(this->GetOwner()));
      return y; \
   }

#define OWNEROFNAV(OBJ,SET) \
   OBJ OM_FAR &NavTo(OBJ OM_FAR &x, SET_TYPE set = SET) \
   { \
      (OBJ OM_FAR &)((PStoreObj)&x)->FromOwner((PStoreObj)this, set); \
      return x; \
   }

#define OWNEROFBLOBNAV(OBJ, SET) \
   OBJ OM_FAR &NavTo>>(OBJ OM_FAR &y, SET_TYPE set = SET) \
   { \
      (OBJ OM_FAR &)((POmBlob)&y)->SetOwner(this); \
      return y; \
   }

#define RELATEDTONAV(OBJ,FLDCONST,FLD) \
   OBJ OM_FAR & NavTo(OBJ OM_FAR &rObj) \
   { \
      return (OBJ OM_FAR &)((PStoreObj)&rObj)->Find(KeyObj(FLDCONST,FLD)); \
   }

#define DIRECTREFNAV(OBJ,FLD) \
   OBJ OM_FAR & NavTo(OBJ OM_FAR &r) \
   { \
      ((PStoreObj)&r)->GotoDirect(DbAddr(FLD)); \
      return r; \
   }

#define REFERENCETONAV(OBJ,FLD) \
   OBJ OM_FAR & NavTo(OBJ OM_FAR &r) \
   { \
      ((PStoreObj)&r)->GotoDirect(DbAddr(FLD)); \
      return r; \
   }

#endif


#define MEMBEROF(OBJ,SET) \
   T_F Connect(OBJ OM_FAR &owner, SET_TYPE set = SET) \
   { \
      return StoreObj::ConnectTo((RStoreObj)owner, set); \
   } \
   T_F DisConnect(OBJ OM_FAR &owner, SET_TYPE set = SET) \
   { \
      return StoreObj::DisConFrom((RStoreObj)owner, set); \
   } \
   RStoreObj GotoMemberOf(OBJ OM_FAR &owner, NavCtrl nc, SET_TYPE set = SET) \
   { \
      return StoreObj::GotoMember((RStoreObj)owner, set, nc); \
   } \
   SET_TYPE Set(OBJ OM_FAR &) \
   { \
      return SET; \
   } \
   RStoreObj NewMemberOf(OBJ OM_FAR &owner, SET_TYPE set = SET) \
   { \
      return StoreObj::NewMember((RStoreObj)owner, set); \
   } \
   T_F IsMemberOf(OBJ OM_FAR &owner, SET_TYPE set = SET) \
   { \
      return StoreObj::IsMember((RStoreObj)owner, set); \
   } \
   MEMBEROFNAV(OBJ,SET)

// These have been removed because they make no sense...
//  - operator << does the exact same thing as MemberUsingSet!
//  - The naming makes no sense...MemberUsingSet to load the owner? come on!
// OBJ OM_FAR &MemberUsingSet(OBJ OM_FAR &x, SET_TYPE set)
// { 
//    ((PStoreObj)&x)->FromMember((PStoreObj)(this), set);
//    return x;
// }
//  - GotoSet has been replaced with GotoMember
// RStoreObj GotoSet(OBJ OM_FAR &rO, NavCtrl nc, SET_TYPE set = SET)
// {
//    return (RStoreObj)GotoMember((RStoreObj)rO, set, nc);
// }


#define BLOBMEMBEROF(OBJ,SET) \
   BLOBMEMBEROFNAV(OBJ,SET)


#define OWNEROF(OBJ,SET) \
   long Members(OBJ OM_FAR &m, SET_TYPE set = SET) \
   { \
      return MemCount((RStoreObj)m, set); \
   } \
   T_F IsOwnerOf(OBJ OM_FAR &member, SET_TYPE set = SET) \
   { \
      return StoreObj::IsOwner((RStoreObj)member, set); \
   } \
   T_F MakeOwnerOf(OBJ OM_FAR &member, SET_TYPE set = SET) \
   { \
       return T_F(StoreObj::MakeOwner(set) == S_OKAY); \
   } \
   OWNEROFNAV(OBJ,SET)

// This has been removed because it makes no sense...
//  - operator >> does the exact same thing!
//  - The naming makes no sense...OwnerUsingSet to load the member? come on!
// OBJ OM_FAR &OwnerUsingSet(OBJ OM_FAR &y, SET_TYPE set)
// {
//    ((PStoreObj)&y)->FromOwner((PStoreObj)this, set);
//    return y;
// }

#define OWNEROFBLOB(OBJ,SET) \
   OWNEROFBLOBNAV(OBJ,SET)


#define RELATEDTO(OBJ,FLDCONST,FLD) \
   RELATEDTONAV(OBJ,FLDCONST,FLD)


#define DIRECTREF(OBJ,FLD) \
   T_F Ref(OBJ OM_FAR &rO) \
   { \
      FLD = ((PStoreObj)&rO)->GetDba(); \
      Write(); \
      return (FLD)? True : False; \
   } \
   T_F UnRef(OBJ OM_FAR &) \
   { \
      FLD = NULL_DBA; return True; \
   } \
   DIRECTREFNAV(OBJ,FLD)


#define REFERENCETO(OBJ,FLD) \
   void OmSet##FLD(DbAddr &dba) \
   { \
      FLD = dba.GetDba(); \
      Write(); \
   } \
   REFERENCETONAV(OBJ,FLD)


#define REFERENCEDBY(OBJ,FLD) \
   T_F Connect(OBJ OM_FAR &rO) \
   { \
      DbAddr dba(GetDba()); \
      rO.OmSet##FLD(dba); \
      return (dba.GetDba()) ? True : False; \
   } \
   T_F DisConnect(OBJ OM_FAR &rO) \
   { \
      rO.OmSet##FLD(DbAddr(0L)); \
      return True; \
   }



//---------------------------------------------------------
// Other Macros
//---------------------------------------------------------

// Customizes the [] and ++ -- Operators to return the derived class
// instead of a StoreObj.  This allows MyObj[FIRST].Myfunction() calls

#define CUSTOMOPS(THISOBJ) \
   THISOBJ OM_FAR &operator[](NavCtrl nc) \
   { \
      return (THISOBJ OM_FAR &)StoreObj::operator[](nc); \
   } \
   THISOBJ OM_FAR &operator[](CRKeyObj k )\
   { \
      return (THISOBJ OM_FAR &)StoreObj::operator[](k ); \
   } \
   THISOBJ OM_FAR &operator[](CRDbAddr d )\
   { \
      return (THISOBJ OM_FAR &)StoreObj::operator[](d ); \
   } \
   THISOBJ OM_FAR &operator++() \
   { \
      return (THISOBJ OM_FAR &)StoreObj::operator++(); \
   } \
   THISOBJ OM_FAR &operator--() \
   { \
      return (THISOBJ OM_FAR &)StoreObj::operator--(); \
   } \
   THISOBJ OM_FAR &operator++(int) \
   { \
      return (THISOBJ OM_FAR &)StoreObj::operator++(); \
   } \
   THISOBJ OM_FAR &operator--(int) \
   { \
      return (THISOBJ OM_FAR &)StoreObj::operator--(); \
   }

#define CLONE(OBJ) \
   PStoreObj OMLOADDS Clone() \
   { \
      PStoreObj tmp = new OBJ; \
      *tmp = *this; \
      return tmp; \
   }
