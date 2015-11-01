//-----------------------------------------------------
//  STORETSK.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------

//#include <string.h>
#include <stdlib.h>

#include <storetsk.hpp>

#if defined(OM_RDS) && defined(SETOOLKIT)
#include <parmsend.hpp>
#include <parmrecv.hpp>
#endif


// Initialize static variable
PStoreTask StoreTask::pCurrTask = 0;


//************************************************************************
//  StoreTask functions
//************************************************************************

#ifdef OM_RDM
  StoreTask ::
StoreTask(CPchar otype) : RDXTask()
{
   SetOpenType(otype);
   DbCount[Active] = DbCount[Inactive] = 0;
   pTransaction = (PTransAction)NULL;
   pCurrTask = this;
   sTransactionRefCount = 0;
   bTransactionAbort = False;
}
#endif


  StoreTask ::
StoreTask(CPchar otype, CRActivateTask a) : RDXTask(a)
{
   SetOpenType(otype);
   DbCount[Active] = DbCount[Inactive] = 0;
   pTransaction = (PTransAction)NULL;
   pCurrTask = this;
   sTransactionRefCount = 0;
   bTransactionAbort = False;
}


  StoreTask ::
StoreTask(CRActivateTask a, CPchar otype) : RDXTask(a)
{
   SetOpenType(otype);
   DbCount[Active] = DbCount[Inactive] = 0;
   pTransaction = (PTransAction)NULL;
   pCurrTask = this;
   sTransactionRefCount = 0;
   bTransactionAbort = False;
}


#if defined(OM_RDS) 
  StoreTask :: 
StoreTask(RDM_SESS hSess) : RDXTask(hSess)
{
   OpenType[0] = '\0';
   DbCount[Active] = DbCount[Inactive] = 0;
   pTransaction = (PTransAction)NULL;
   pCurrTask = this;
   sTransactionRefCount = 0;
   bTransactionAbort = False;
}
#endif

    
  StoreTask ::
~StoreTask()
{
   // delete (and close) all databases in this Task
   int i;
   for (int a = Active; a <= Inactive; a++)
   {
      while ((i = DbCount[a] - 1) > -1)
         pDbs[a][i]->Close();
   }

   if (pCurrTask == this)
      pCurrTask = 0;
}


    int
  StoreTask ::
GetDbNames(DbStat a, Pchar buf) const
{
   *buf = 0;
   for (int i = 0; i < DbCount[a]; i++)
   {
      if (i)
         om_strcat(buf, (Pchar)";");

      om_strcat(buf, pDbs[a][i]->GetName());
   }

   return DbCount[a];
}


    PStoreTask
  StoreTask ::
GetCurrTask()
{
   return pCurrTask;
}


    void
  StoreTask ::
MakeCurrent()
{ 
   pCurrTask = this; 
}


    void
  StoreTask ::
SetCurrTask(PStoreTask Task)
{
   pCurrTask = Task;
}


//-----------------------------------------------------
// when an Open is performed, the database is put in the active array
// StoreDb's are kept in dbn order in the active array
// returns database Number
    T_F
  StoreTask ::
ActivateDb(PStoreDb pDb)
{
   if (PosInArray(pDb, Active) != -1)
      return True;

   return Swap(pDb, Inactive, Active);
}


//-----------------------------------------------------
// when a Close is performed, the database is put in the Inactive array
    T_F
  StoreTask ::
DeactivateDb(PStoreDb pDbToFind)
{
   if (PosInArray(pDbToFind, Inactive) != -1)
      return True;

   return Swap(pDbToFind, Active, Inactive);
}


// To use ActivateAll() there must be NO database currently active -
//  used only with query 
    T_F
  StoreTask ::
ActivateAll()
{
   T_F rtn = False;

   if (DbCount[Active])
   {
      ROMERROR(S_DBACTIVE);
      return rtn;
   }

   PStoreDb pdb;
   int count = DbCount[Inactive];

   for (int i = 0; i < count; i++)
   {
      pdb = pDbs[Inactive][0];
      if ((rtn = ActivateDb(pdb)) == False)
         break;
#ifdef OM_RDM
      else
         pdb->SetQueryOpened(True);
#endif
   }

   if (rtn == False)
      ROMERROR(S_ACTALL);
   return rtn;
}


// To use DeactivateAll() there must be some database currently active -
//  used only with query 
    T_F
  StoreTask ::
DeactivateAll()
{
   T_F rtn = False;
   PStoreDb pdb;
   for (int i = 0; i < DbCount[Active]; i++)
   {
      pdb = pDbs[Active][0];
      if ((rtn = DeactivateDb(pdb)) == False)
         break;
#ifdef OM_RDM
      else
         pdb->SetQueryOpened(False);
#endif
   }

   return rtn;
}


//-----------------------------------------------------
// Add this StoreDb to the inactive list
// This is performed as a function of the StoreDb constructor
    T_F
  StoreTask ::
AddInactive(PStoreDb pDb)
{
   // Add directly to the Inactive array
   if (DbCount[Inactive] + 1 < MAXDBS)
      pDbs[Inactive][ DbCount[Inactive]++ ] = (PStoreDb)pDb;
   else
   {
      ROMERROR(S_MAXDB);
      return False;
   }

   return True;
}


    PStoreDb
  StoreTask ::
FindDb(CPchar dbname) const
{
   if (!dbname || !*dbname)
      return (PStoreDb)NULL;

   int i;
   for (int a = Active; a <= Inactive; a++)
   {
      for (i=0; i < DbCount[a]; i++)
      {
         if (!om_strcmp(pDbs[a][i]->GetName(), dbname))
            return pDbs[a][i];
      }
   }

   return (PStoreDb)NULL;
}


//-----------------------------------------------------
// Go thru the array to see if its there, return index or -1
    int
  StoreTask ::
PosInArray(CPStoreDb pDbToFind, DbStat stat) const
{
   for (int i = 0; i < DbCount[stat]; i++)
   {
      if (pDbToFind == pDbs[stat][i])
         return i;
   }
   return -1;
}


//-----------------------------------------------------
// Must be Inactive
// This is performed as a function of the StoreDb Destructor
    T_F
  StoreTask ::
RemoveDb(PStoreDb pDb)
{
   // if pDb is not last in the Inactive array, swap() will do it
   if (pDb != pDbs[Inactive][ DbCount[Inactive] - 1])
   {
      if (Swap(pDb, Inactive, Inactive) == False)
         return False;
   }

   pDbs[Inactive][ DbCount[Inactive] - 1 ] = 0;    // Null it
   DbCount[Inactive]--;
   return True;
}


    T_F
  StoreTask ::
SetUpDb(CPchar dbname, PStoreDb pDb)
{
   // should never get to here without a database name
   if (!dbname || !*dbname)
   {
      ROMERROR(S_NODBNAME);
      return False;
   }

   // can not have two databases of the same name handled by one Task
   if (FindDb(dbname))
   {
      ROMERROR(S_TWODBS);
      return False;
   }

   // try to find Db in Active list, if it exists, Error
   //  else if it's also not in Inactive list, add it
   if (PosInArray(pDb, Active) != -1)
   {
      ROMERROR(S_DBISOPEN);
      return False;
   }
   else if (PosInArray(pDb, Inactive) == -1)
      AddInactive(pDb);

   return True;
}


//-----------------------------------------------------
// returns index Number of the "To" array or -1 if not found
// When "To" is the Active array this number represents the dbn
// Also When "From" is Active, we send StoreDbs a message,
// setting any changed dbn's (database numbers)
// When "From" and "To" are the same, Swap() will put pDb at the end
    T_F
  StoreTask ::
Swap(PStoreDb pDbToFind, DbStat From, DbStat To)
{
   // Go thru the From array and Find pDbToFind
   T_F      Found = False;
   PStoreDb pTest;

   for (int i=0; i < DbCount[From]; i++)
   {
      pTest = pDbs[From][i];

      if (!Found && pDbToFind == pTest)
      {
         if (DbCount[To] + 1 < MAXDBS)
         {
            pDbs[To][ DbCount[To]++ ] = pDbToFind;
            Found = True;
#ifdef OM_RDM
            if (To == Active)
               pDbToFind->SetDbn(DbCount[To] - 1); // let it know its dbn
#endif
         }
         else
         {
            ROMERROR(S_MAXDB);
            return False;
         }
      }

      // Compress till the end of pDbs[][]
      if (Found == True)
      {
         if ( i + 1 < DbCount[From])
         {
            pDbs[From][i] = pDbs[From][i + 1];
#ifdef OM_RDM
            if (From == Active)
               pDbs[From][i]->SetDbn(i);
#endif
         }
         else
         {
            // Last one, decrement count to reflect compression
            if (DbCount[From] > 0)
               DbCount[From]--;
         }
      }
   }

   if (Found == False)
      ROMERROR(S_SWAPFAIL);

   return Found;
}


    void
  StoreTask ::
SetOpenType(CPchar opentype)
{ 
#ifdef OM_RDS
   if (opentype[0] == 'o')
   {
      OpenType[0] = 'x';
      OpenType[1] = 'n';
      OpenType[2] = '\0';
      return;
   }
#endif

   OpenType[0] = opentype[0];
   OpenType[1] = opentype[1];
   OpenType[2] = '\0';
}


    void 
  StoreTask :: 
SetUserPaths(CPchar path, CPchar userid)
{
#ifdef OM_RDM
   if (path && *path)
   {
      dbdpath(path);
      dbfpath(path);
      dbtaf(path);
      dblog(path);
   }
   if (userid && *userid)
      dbuserid(userid);
#else
   // This function is not used for RDS
#endif
}


    void 
  StoreTask :: 
SetPaths(CPchar dbd, CPchar dbf, CPchar taf, CPchar log, CPchar uid)
{
#ifdef OM_RDM
   if (dbd && *dbd)  dbdpath (dbd);
   if (dbf && *dbf)  dbfpath (dbf);
   if (taf && *taf)  dbtaf   (taf);
   if (log && *log)  dblog   (log);
   if (uid && *uid)  dbuserid(uid);
#else
   // This function is not used for RDS
#endif
}


    void 
  StoreTask ::
UseEnv(long uniq)            
{                           
   // uniq is a unique number to be appended to  the DBUSERID.  It is used 
   // mainly for multi-Tasking or where DBUSERID is reused

#ifdef OM_RDM
   char uid[70];
   Pchar p;
   static char tmp[20];

   p = getenv("DBUSERID");
   sprintf(tmp, "%ld", uniq);
   if (p)
      om_strncpy((Pchar)uid, p, 50);   // copy from env
   else 
      uid[0] = 0;
   om_strcat((Pchar)uid,tmp);         // add unique number

   SetPaths (getenv("DBDPATH"),
             getenv("DBFPATH"), 
             getenv("DBTAF"), 
             getenv("DBLOG"),
             uid);

   Pchar lk = getenv("LOCKMGR");
   if (lk && *lk)
       lockmgr(lk);
       
#else
   // This function is not used for RDS
#endif
}


#if defined(OM_RDS) && defined(SETOOLKIT)
    seParmSend&
  StoreTask::
SendParms(seParmSend& _send)
{
   _send << OpenType;
   _send.Put(&DbCount, 2);
   _send.Put(&pDbs, 2*MAXDBS);
   return RDXTask::SendParms(_send);
}

    seParmReceive&
  StoreTask::
ReceiveParms(seParmReceive& _receive)
{
   _receive >> OpenType;
   _receive.Get(&DbCount, 2);
   _receive.Get(&pDbs, 2*MAXDBS);
   return RDXTask::ReceiveParms(_receive);
}
#endif


// ACK: what is this for and where is it used?
   void OM_EXPORT
deleteDb(PStoreDb pdb)
{
   if (pdb) 
      delete pdb;
}


