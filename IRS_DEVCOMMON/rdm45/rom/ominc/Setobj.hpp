//-----------------------------------------------------
//  SETOBJ.HPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _SETOBJ_HPP_
#define _SETOBJ_HPP_

OMCLASS(StoreDb);

OMCLASS(SetObj);
OMCLASS(MMSetObj);

//-----------------------------------------------------
//  SetObj
//-----------------------------------------------------
class OM_EXPORT SetObj
{
private:
   SET_TYPE SetNum;

public:
   SetObj(SET_TYPE s) : SetNum(s) {}

   SET_TYPE Num() const { return SetNum; }
};


typedef struct MMSet_Info_S
{
   REC_TYPE rectype;
   USHORT   recdatasize;
} MMSET_INFO;


class OM_EXPORT MMSetObj : public SetObj
{
private:
   MMSET_INFO *pInfo;
   short       Members;

public:
   virtual ~MMSetObj() { if (pInfo) delete [] pInfo; }
   MMSetObj(SET_TYPE set, PStoreDb pdb);

   MMSET_INFO *GetMMSetInfo() const { return pInfo; }
   MMSET_INFO *operator[](short idx) const { return (pInfo+idx); }
   short       MemCount() const { return Members; }
};

#endif

