//---------------------------------------------------------
//  VISTAOBJ.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _VISTAOBJ_HPP_
#define _VISTAOBJ_HPP_

OMCLASS(StoreDb);
OMCLASS(VistaObj);

#if defined(OM_RDS) && defined(SETOOLKIT)
class seParmSend;
class seParmReceive;
#endif


class OM_EXPORT VistaObj
{
private:
   PStoreDb _pdb;

#if defined(OM_RDS) && defined(SETOOLKIT)
   virtual  seParmSend& SendParms(seParmSend& _send) { return _send;}
   virtual  seParmReceive& ReceiveParms(seParmReceive& _receive) { return _receive;}
#endif

protected:
   short    status;

            VistaObj(PStoreDb pdb = 0) : status(S_OKAY), _pdb(pdb) {}

public:
   virtual ~VistaObj() {}

   T_F      Okay() const        { return (status == S_OKAY) ? True : False; }
   short    GetStatus() const   { return status; }
   void     SetStatus(short s)  { status = s; }

   // _GetDb() is re-defined in STOREDIN() macro
   virtual  PStoreDb OMLOADDS _GetDb() const = 0; 
   virtual  PStoreDb           GetDb() const                 { return (_pdb ? _pdb : _GetDb()); } 
   virtual  PStoreDb           SetDb(PStoreDb pdb = 0) { return (_pdb = pdb); } 
   
private:
   void     VistaObjCompilerFix() const; // added for out-of-line function for some compilers
};

#endif
