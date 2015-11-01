//---------------------------------------------------------
//   NAV.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _NAV_HPP_
#define _NAV_HPP_

OMCLASS(StoreObj)
OMCLASS(Navigator)
OMCLASS(DirectNav)
OMCLASS(RelationalNav)

enum NavigatorType { Direct = 1, Relational, Network };

class OM_EXPORT Navigator
{
   friend class StoreObj;
   
private: 
   unsigned short nUseCount;   // Allows Arrays of StoreObj to use one Navigator
   NavigatorType  eNavType;

protected:
   Navigator(NavigatorType _type) : nUseCount(1) , eNavType(_type) {}

   virtual RStoreObj Navigate(NavCtrl, RStoreObj x) { return x; }

   void AddUse() { nUseCount++; }
   void EndUse();
   unsigned short UseCount() const { return nUseCount; }
   
   NavigatorType NavType() const { return eNavType; }
};


//---------------------------------------------------------
//  DirectNav
//---------------------------------------------------------
// Used to define Simple Keyed Storable Objects
class OM_EXPORT DirectNav : public Navigator
{
   friend class StoreObj;
   
protected:
   DirectNav() : Navigator(Direct) {}
   
   virtual RStoreObj Navigate(NavCtrl, RStoreObj);
};


//---------------------------------------------------------
//  RelationalNav
//---------------------------------------------------------
class OM_EXPORT RelationalNav : public Navigator
{
   friend class StoreObj;

private: 
   FLD_TYPE KeyFld;            // Data Manager Key Field number

   RStoreObj Navigate(FLD_TYPE key, NavCtrl, RStoreObj);

protected:
   RelationalNav(CRKeyObj k) : Navigator(Relational), KeyFld(k.Fld()) {}

   virtual RStoreObj Navigate(NavCtrl nc, RStoreObj so)
                        { return Navigate(KeyFld, nc, so); }
   virtual RStoreObj Navigate(CRKeyObj ko, NavCtrl nc, RStoreObj so)
                        { return Navigate(ko.Fld(), nc, so); }

public:
   FLD_TYPE Key() const { return KeyFld; }
};


//---------------------------------------------------------
//  NetworkObj 
//---------------------------------------------------------
// Used to define Simple Sets where this object is owned by another
class OM_EXPORT NetworkNav : public Navigator
{
   friend class StoreObj;
   
private: 
   SET_TYPE set;

   RStoreObj DoTheNavigate(NavCtrl, RStoreObj);

protected:
   NetworkNav(CRSetObj set) : Navigator(Network), set(set.Num()) {}
   
   virtual RStoreObj Navigate(NavCtrl, RStoreObj);

   RStoreObj Navigate(SET_TYPE, NavCtrl, RStoreObj);
   RStoreObj Navigate(RStoreObj, SET_TYPE, NavCtrl, RStoreObj);

public:
   SET_TYPE Set() const { return set; }
};

#endif
