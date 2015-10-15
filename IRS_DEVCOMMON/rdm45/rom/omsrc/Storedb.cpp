//-----------------------------------------------------
//  STOREDB.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <storedb.hpp>

#ifdef OM_RDS
#  include <hlfproto.h>
#  ifdef SETOOLKIT
#    include <parmsend.hpp>
#    include <parmrecv.hpp>
#  endif
#endif


//-----------------------------------------------------
// StoreDb Routines
//-----------------------------------------------------
  StoreDb ::
StoreDb(CPchar dbname, PStoreDb OM_FAR * ppdb)
{   
   *ppdb = this;
   pTask = StoreTask::GetCurrTask();
   UseCount = 0;
   OpenClosed = Closed;
   DbName = (Pchar)NULL;
#ifdef OM_RDM
   QueryOpened = False;
#endif
   Init(pTask);
   SetName(dbname);
}

    
  StoreDb ::
StoreDb(PStoreDb OM_FAR * ppdb)
{
   *ppdb = this;
   pTask = StoreTask::GetCurrTask();
   UseCount = 0;
   OpenClosed = Closed;
   DbName = (Pchar)NULL;
#ifdef OM_RDM
   QueryOpened = False;
#endif
   Init(pTask);
}


  StoreDb ::
~StoreDb()
{
   UseCount = 0;
   Close();

   if (DbName)
      delete [] DbName;

   if (pTask)
      pTask->RemoveDb(this);
}


    void
  StoreDb ::
SetName(CPchar dbname)
{
   if (pTask->SetUpDb(dbname, this))
   {
      UseCount = 0;
      OpenClosed = Closed;
#ifdef OM_RDM
      QueryOpened = False;
#endif

      if (DbName)
         delete [] DbName;

      DbName = (Pchar)new OM_FAR char[om_strlen(dbname) + 1];
      if (!DbName)
         ROMERROR(S_NOMEMORY);
      else
         om_strcpy(DbName, dbname);
   }
}


   T_F
  StoreDb ::
Close()
{
   if (OpenClosed == Opened)
   {
      if (--UseCount > 0)   // UseCount must be 0 to close the database
         return False;      // Let caller know its still in use

      pTask->DeactivateDb(this);

      if (!IncrClose())
         return False;
   }

   return True;
}


    T_F
  StoreDb ::
Open()
{
   if (!GetTask()->Okay())
      return False;

   // If its already open, nothing to do
   if (OpenClosed == Opened)
   {
      UseCount++;
      return True;
   }

   T_F rtn = False;
   
   // Open using Task's OpenType
   if (!pTask->DbCount[Active])
      rtn = FirstOpen(pTask->GetOpenType());
   else
      rtn = IncrOpen(pTask->GetOpenType());

   // See if open was successful, clean up if not
   if (rtn == True)
   {
      pTask->ActivateDb(this);
      OpenClosed = Opened;
      UseCount++;
   }

   return rtn;
}


    T_F
  StoreDb ::
FirstOpen(CPchar opentype)
{
   if (DbName && *DbName)
   {
#ifdef OM_RDS
      RDM_DB _hDb;
// Future development... #ifdef OM_CLIENT
      // client side open 
      SetDbStatus(hlf_Open(DbName, opentype, GetSession(), &_hDb));
/* Future development... #else // OM_SERVER
      // server side open 
      SetDbStatus(::d_open((Pchar)DbName, opentype, GetSession(), &_hDb));
#endif
*/
      SethDb(_hDb);
#else // OM_RDM
      SetDbStatus((short)::dt_open(DbName, opentype, GetDbTask()));
#endif

      if (!Okay() && GetDbStatus() > S_OKAY)
         ROMERROR(S_FIRSTOPEN);
   }
   else
      ROMERROR(S_NODBNAME);

   return Okay();
}


    T_F
  StoreDb ::
IncrOpen(CPchar opentype)
{
   if (DbName && *DbName)
   {
#ifdef OM_RDS
      RDM_DB _hDb;
// Future development... #ifdef OM_CLIENT
      // client side open 
      SetDbStatus(hlf_IOpen(DbName, opentype, GetSession(), &_hDb));
/* Future development... #else // OM_SERVER
      // server side open 
      SetDbStatus(::d_iopen((Pchar)DbName, opentype, GetSession(), &_hDb));
#endif
*/
      SethDb(_hDb);
#else // OM_RDM
      SetDbStatus((short)::dt_iopen(DbName, GetDbTask()));
#endif

      if (!Okay() && GetDbStatus() > S_OKAY)
         ROMERROR(S_INCROPEN);
   }
   else
      ROMERROR(S_NODBNAME);

   return Okay();
}


    T_F
  StoreDb ::
IncrClose()
{
#ifdef OM_RDS
// Future development... #ifdef OM_CLIENT
   // client side close
   SetDbStatus(hlf_IClose(GethDb()));
/* Future development...
#else
   // server side close
   SetDbStatus((short)::d_iclose(GethDb()));
#endif
*/
#endif

#ifdef OM_RDM
   if (QueryOpened)
      return False;

   SetDbStatus((short)::dt_iclose(GetDbTask(), GetDbn()));
#endif

   if (Okay())
      OpenClosed = Closed;

   return Okay();
}


#ifdef OM_RDM
    void
  StoreDb ::
SetQueryOpened(T_F flag)
{
   if ((QueryOpened = flag) == True)
      OpenClosed = Opened;
   else
      OpenClosed = Closed;
}
#endif

#if defined(OM_RDS) && defined(SETOOLKIT)
    seParmSend&
  StoreDb::
SendParms(seParmSend& _send)
{
   _send << DbName;
   _send << (short)OpenClosed;
   _send << UseCount;
   // have to call base class's SendParms
   return RDXDb::SendParms(_send);
}

    seParmReceive&
  StoreDb::
ReceiveParms(seParmReceive& _receive)
{
   short temp;
   _receive >> DbName >> temp >> UseCount;
   OpenClosed = (OpenStat)temp;
   // have to call base class's ReceiveParms
   return RDXDb::ReceiveParms(_receive);
}
#endif

