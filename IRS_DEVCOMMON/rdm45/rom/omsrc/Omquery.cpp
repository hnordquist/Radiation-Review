//-----------------------------------------------------
//  OMQUERY.CPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <omquery.hpp>

extern "C" {
#include <stdlib.h>
};



//-----------------------------------------------------
// DynString Class functions
//-----------------------------------------------------

  DynString ::
~DynString()
{ 
   if (pStr) 
      delete [] pStr; 
}

   void
  DynString ::
Set(int l)
{
   if (pStr) 
      delete [] pStr;

   pStr = new OM_FAR char[l]; // allocate amount (includes null)
   if (pStr)
      BufLen = l; 
   else
      ROMERROR(S_NOMEMORY);
   Len = 0;
}

    void
  DynString ::
Set(CPchar pf)
{
   int ln = om_strlen(pf);        // Find length of string
   if (BufLen < ln && pStr)       // if buffer is to small
   {
      delete [] pStr;             // delete it
      pStr = 0;
   }

   if (!pStr && ln)               // if we need to create one
   {
      BufLen = ln + 1;            // make room for null
      pStr = new OM_FAR char[BufLen];
      if (pStr)
         Len = ln;                   
      else
         ROMERROR(S_NOMEMORY);
   }

   if (pStr)                       // if buffer exists
      om_strcpy(pStr, pf);         // copy string to buffer
}


//-----------------------------------------------------
// OmQuery Class functions
//-----------------------------------------------------

  OmQuery ::
OmQuery(PQtask pqt)
{
   pQtask = pqt;                       // Remember task pointer
   SelCount = 0;                       // init count for selected object
   Epos = 0;        
   Emsg = 0;        
   SqlStatus = Q_OKAY;   
   BreakStatus = False;
   ValCount = 0;    

   for (int i = 0; i < MAXQOBJS;i++)   // init array of object  pointers
      ObjArray[i] = 0;

   QryStmt.Set(MAXSTMT);               // allocate memory for statement
}

    int
  OmQuery ::
Query(CPchar stmt)                     // Easy way to call query()
{  
   QryStmt.Set(stmt);
   if ((SqlStatus = pQtask->query(QryStmt.Get(), &Epos, &Emsg)) > Q_OKAY)
      ErrHandler();
   return SqlStatus;
}

    void                               // make list of selected objects
  OmQuery ::                           // list must be 0 terminated
SelectObjects(PStoreObj pS,...)        // use one PStoreObj for safetyping
{
   va_list    ap;
   PStoreObj  arg;
   short      idx = 0;
   
   va_start(ap, pS);    
   for (arg = pS; arg != NULL; arg = va_arg(ap, PStoreObj))
      ObjArray[idx++] = arg;
      
   SelCount = idx;                     // remember how many are in list
   va_end(ap);
}

    int                                // Builds Query statement
  OmQuery ::
Statement(CPchar wstmt)
{
   *(QryStmt.Get()) = 0;               // start from scratch
   om_strcat(QryStmt.Get(), "select "); // add select

   for (int i=0; i < SelCount; i++)    // add each objects "db_addr(RECTYPE)"
   {
      if (i)
         om_strcat(QryStmt.Get(), ",");// comma between db_addr() statemnts

      om_strcat(QryStmt.Get(), "db_addr(");
      om_strcat(QryStmt.Get(), ObjArray[i]->RecName());
      om_strcat(QryStmt.Get(), ")");    
   }

   if (SelFldStmt.Get())               // Add additional optional
   {
      if (SelCount)                    // field select statements
         om_strcat(QryStmt.Get(), ","); // need a comma

      om_strcat(QryStmt.Get(), SelFldStmt.Get());  // add field string
   }

   if (FromStmt.Get())                 // Add additional optional
   {                                   // from statement
      om_strcat(QryStmt.Get(), " from ");
      om_strcat(QryStmt.Get(), FromStmt.Get());
   }

   om_strcat(QryStmt.Get(), " ");       // white space
   om_strcat(QryStmt.Get(), wstmt);     // the where and order clause...

   SqlStatus = pQtask->sqlinit(QryStmt.Get(), &Epos, &Emsg);

   if (SqlStatus != Q_OKAY)
      ErrHandler();
   return SqlStatus;
}

    void
  OmQuery ::
From(CPchar pf)
{
   FromStmt.Set(pf);
}

    void
  OmQuery ::
SelectFields(CPchar pf)
{
   SelFldStmt.Set(pf);
}

// Checks to see if more rows
// and load the object in from the db_addrs returned
    T_F
  OmQuery ::
MoreRows()
{
   if (SqlStatus != Q_OKAY)                 // if SqlStatus is bad...abort
      return False;

   int qrtn;
   BreakStatus = False;
   T_F rtn = True;                          // get value list from db_query

   if ((qrtn = pQtask->nextval(&ValCount, Vals)) == Q_OKAY)
   {
      for (int i=0; i < SelCount; i++)      // go thru all object in list
      {
         if (Vals[i].type == 'd')           // make sure values are DB_ADDR
         {
            DbAddr dba(Vals[i].result.av);  // construct a DbAddr
            ObjArray[i]->GotoDirect(dba);   // read data into object
         }
         else                               // complaing if wrong data type
         {
		 	ObjArray[i]->SetDba(NULL_DBA);
            ObjArray[i]->SetStatus(S_OKAY);
		 }
      }
   }
   else    // If its a Q_Break set BreakStatus, reset qrtn
   {
      if (qrtn == Q_BREAK)
      {  
         qrtn = Q_OKAY;      // Its Okay to have a break
         BreakStatus = True;
      }
   }

   return (rtn == False || qrtn != Q_OKAY) ? False : True;
}

   void
  OmQuery ::
FieldVals(Pint pcount, VALUE OM_FAR * OM_FAR *pval) // Return fields not used to set
{                                           // selected objects
   if (IsBreak())                           // When there is a break
   {                                        // return all the vals
      *pcount = ValCount;                   // other wise return vals
      *pval = &Vals[0];
   }
   else
   {
      *pcount = ValCount - SelCount;        // other wise return vals
      *pval = &Vals[SelCount];              // after dbaddrs of objects
   }
}

   void
  OmQuery ::
ErrHandler()    // Its a virtual, so you can make it nicer
{
   char lin[20];
   Pchar msg = new OM_FAR char[om_strlen(LastQuery())+80];

   if (!msg)
      ROMERROR(S_NOMEMORY);

   if (Emsg && *Emsg)
      om_strcpy(msg, Emsg);
   else
      om_strcpy(msg, "Query Error");

   sprintf(lin, "%d", Epos);
   om_strcat(msg, ": position [");
   om_strcat(msg, lin);
   om_strcat(msg, "]\n");
   om_strcat(msg, LastQuery());

   // error message length is limited to 255
   if (om_strlen(msg) > 255)
      msg[255] = '\0';

   ROMQERROR(S_QUERYERR, msg);

   delete [] msg;
}

