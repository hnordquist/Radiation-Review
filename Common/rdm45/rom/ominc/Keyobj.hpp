//---------------------------------------------------------
//  KEYOBJ.HPP                  
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _KEYOBJ_HPP_
#define _KEYOBJ_HPP_

OMCLASS(KeyObj);

class OM_EXPORT KeyObj
{
   FLD_TYPE KeyFld;
   Pvoid    pVal;
    
public:
   KeyObj(FLD_TYPE k) : KeyFld(k), pVal(NULL) {}

   // Used in RELATEDTO MACROS AND SUBCLASSES
   KeyObj(FLD_TYPE k, CPchar  pc) { KeyFld = k; pVal = (Pvoid) pc; }
   KeyObj(FLD_TYPE k, CPvoid  pv) { KeyFld = k; pVal = (Pvoid) pv; }
   KeyObj(FLD_TYPE k, CPshort  v) { KeyFld = k; pVal = (Pvoid) v;  }
   KeyObj(FLD_TYPE k, CPint    v) { KeyFld = k; pVal = (Pvoid) v;  }
   KeyObj(FLD_TYPE k, CPlong   v) { KeyFld = k; pVal = (Pvoid) v;  }
   KeyObj(FLD_TYPE k, CPdouble v) { KeyFld = k; pVal = (Pvoid) v;  }
   KeyObj(FLD_TYPE k, CPfloat  v) { KeyFld = k; pVal = (Pvoid) v;  }

   KeyObj(FLD_TYPE k, const unsigned short OM_FAR * v) { KeyFld = k; pVal = (Pvoid) v;  }
   KeyObj(FLD_TYPE k, const unsigned int   OM_FAR * v) { KeyFld = k; pVal = (Pvoid) v;  }
   KeyObj(FLD_TYPE k, const unsigned long  OM_FAR * v) { KeyFld = k; pVal = (Pvoid) v;  }

   KeyObj(FLD_TYPE k, CRint    v) { KeyFld = k; pVal = (Pvoid) &v; }
   KeyObj(FLD_TYPE k, CRlong   v) { KeyFld = k; pVal = (Pvoid) &v; }
   KeyObj(FLD_TYPE k, CRdouble v) { KeyFld = k; pVal = (Pvoid) &v; }
   KeyObj(FLD_TYPE k, CRfloat  v) { KeyFld = k; pVal = (Pvoid) &v; }

   RKeyObj  Set(CPchar   pc) { pVal = (Pvoid) pc; return *this; }
   RKeyObj  Set(CPvoid   pv) { pVal = (Pvoid) pv; return *this; }
   RKeyObj  Set(CPshort  v)  { pVal = (Pvoid) v; return *this; }
   RKeyObj  Set(CPint    v)  { pVal = (Pvoid) v; return *this; }
   RKeyObj  Set(CPlong   v)  { pVal = (Pvoid) v; return *this; }
   RKeyObj  Set(CPdouble v)  { pVal = (Pvoid) v; return *this; }
   RKeyObj  Set(CPfloat  v)  { pVal = (Pvoid) v; return *this; }

   RKeyObj  Set(const unsigned short OM_FAR * v)  { pVal = (Pvoid) v; return *this; }
   RKeyObj  Set(const unsigned int   OM_FAR * v)  { pVal = (Pvoid) v; return *this; }
   RKeyObj  Set(const unsigned long  OM_FAR * v)  { pVal = (Pvoid) v; return *this; }

   RKeyObj  Set(CRint    v)  { pVal = (Pvoid) &v; return *this; }
   RKeyObj  Set(CRlong   v)  { pVal = (Pvoid) &v; return *this; }
   RKeyObj  Set(CRdouble v)  { pVal = (Pvoid) &v; return *this; }
   RKeyObj  Set(CRfloat  v)  { pVal = (Pvoid) &v; return *this; }

   FLD_TYPE Fld()       const { return KeyFld; }
   T_F      HasValue()  const { return (pVal) ? True : False; }
   Pvoid    Value()     const { return pVal; }
};

#endif
