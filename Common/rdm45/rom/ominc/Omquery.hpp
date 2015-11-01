//-----------------------------------------------------
//  OMQUERY.HPP                  
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _OMQUERY_HPP_
#define _OMQUERY_HPP_

#include <string.h>
#include <stdarg.h>
#include "qtask.hpp"
#include "storedb.hpp"    // task/database Object Manager include
#include "storeobj.hpp"


#define MAXQOBJS 20
#define MAXVALS  24


class OM_EXPORT DynString
{
private:
   int   Len;                      // Length of string
   int   BufLen;                   // Length of buffer
   Pchar pStr;                     // pointer to string / buffer

public:
   DynString(int l)           { Set(l); }
   DynString(CPchar p, int l) { Set(p); }
   DynString()                { pStr = 0; BufLen = 0; Len = 0; }
   ~DynString();

   void  Set(CPchar);         // Set from string passed
   void  Set(int l);          // Set buffer to length passed
   Pchar Get()    const { return pStr;}
   int   Length() const { return Len; }
};


class OM_EXPORT OmQuery
{
private:
   DynString QryStmt;              // Query Statement String
   DynString FromStmt;             // From Statement String
   DynString SelFldStmt;           // Select Fields
   PStoreObj ObjArray[MAXQOBJS];   // Array of object pointers to manage

   int       SelCount;             // Count of object pointers to manage
   int       Epos;                 // Position of last error in QryStmt
   Pchar     Emsg;                 // Last Error message
   int       SqlStatus;            // status of last sqlinit/query call
   T_F       BreakStatus;
   int       ValCount;             // Count of Values placed in Vals[]
   VALUE     OM_FAR Vals[MAXVALS]; // Array of Values returned from nextvals
   PQtask    pQtask;               // pointer to a Qtask object

public:
   OmQuery(PQtask pqt);            // Must pass a valid Qtask pointer
   virtual ~OmQuery() {}           // DynString clean up themselves

   int    Query(CPchar stmt);      // Simple free form call to query

   PQtask GetQtask() const { return pQtask; };

   void  SelectObjects(PStoreObj, ...); // What Object to Select
   void  SelectFields(CPchar);// If additional fields are desired
   void  From(CPchar);        // if from clause is not in statement
   int   Statement(CPchar);   // rest of the Statement, call sqlinit()
   T_F   MoreRows();               // Sets objects each row of query results
   T_F   IsBreak()   const { return BreakStatus; }    // Break returned from next row
   Pchar LastQuery() const { return QryStmt.Get(); }  // return last QryStmt
   void  FieldVals(Pint count, VALUE OM_FAR * OM_FAR *);  // returns field values 

   int   ErrPosition() const { return Epos; }    // Error Position of last error
   Pchar ErrMsg()      const { return Emsg; }    // Last Error message
   virtual void ErrHandler();                    // Error Handler if not Q_OKAY
};

#endif
