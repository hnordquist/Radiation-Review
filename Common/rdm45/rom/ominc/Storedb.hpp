//---------------------------------------------------------
//  STOREDB.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _STOREDB_HPP_
#define _STOREDB_HPP_

#include "vpp.hpp"
#include "dbrdx.hpp"
#include "storetsk.hpp"

OMCLASS(StoreDb);
#define VistaDb   StoreDb
#define PVistaDb  PStoreDb
#define RVistaDb  RStoreDb
#define CRVistaDb CRStoreDb

enum OpenStat { Closed, Opened };

//---------------------------------------------------------
//  StoreDb 
//---------------------------------------------------------
// Used to define a database object 

class OM_EXPORT StoreDb: public RDXDb
{
protected:
   StoreDb(CPchar name, PStoreDb OM_FAR * ppdb);
   StoreDb(PStoreDb OM_FAR * ppdb);

public:
   virtual ~StoreDb();

   T_F  Close();
   T_F  IsOpen() const { return (OpenClosed == Opened ? True : False); }
   T_F  Open();

   Pchar  GetName() const { return DbName; }
   void   SetName(CPchar dbname);

   virtual void  MakeCurrent() {}
   PStoreTask    GetTask() const { return pTask; } 

#if defined(OM_RDS) && defined(SETOOLKIT)
   virtual seParmSend& SendParms(seParmSend& _send);
   virtual seParmReceive& ReceiveParms(seParmReceive& _receive);
#endif

private:
   T_F  FirstOpen(CPchar opentype);
   T_F  IncrOpen(CPchar opentype);
   T_F  IncrClose();

#ifdef OM_RDM
   void SetQueryOpened(T_F flag);
#endif

private:
   Pchar       DbName;
   PStoreTask  pTask;      // ref to the StoreTask Managing this Db
   OpenStat    OpenClosed;
   short       UseCount;   // Don't Remove until use count = 0;
#ifdef OM_RDM
   T_F         QueryOpened;
#endif

   friend StoreTask;
};



//------------------------------------------------------------
// PDB_LOCATOR must be passed to the StoreDb constructor
//------------------------------------------------------------
#define PDB_LOCATOR (PStoreDb OM_FAR *)&DbPtr

//------------------------------------------------------------
// DEFINE_DB_LOCATOR must appear in the public section of 
// database object class definition
//------------------------------------------------------------
#define DEFINE_DB_LOCATOR static PStoreDb OM_FAR DbPtr; \
     void MakeCurrent() { DbPtr = this; }

#endif
