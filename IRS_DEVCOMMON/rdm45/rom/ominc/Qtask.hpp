//-----------------------------------------------------
//  QTASK.HPP                  
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _QTASK_HPP_
#define _QTASK_HPP_

#include "storedb.hpp"

extern "C" { 
#include <stdlib.h> 

// ROM makes explicit calls to qt_func() so db_QUERY doesn't need to cover it
#define NO_QT_COVER
#include <query.h> 
}


OMCLASS(Qtask);


class OM_EXPORT Qtask : public StoreTask
{
public:
   Qtask(CPchar mode = "o");
   Qtask(CRActivateTask);
   Qtask(CPchar openmode, CRActivateTask a);
   ~Qtask();

   T_F   OpenAll();        // Must be use instead of Open() calls
                           // All database object must be exist and be
                           // closed (inactive)
   void  CloseAll();       // Must be used instead of Close()

   void  SetQueryPaths(CPchar qdb, CPchar qdf, CPchar qrf);

   //--------------------------------------------------------------
   // The following functions coorespond to standard query function calls
   //--------------------------------------------------------------
   int  query(CPchar stmt, Pint epos, Pchar OM_FAR *emsg)
           { return dt_query((Pchar)stmt, epos, emsg, DbTask()); }

   int  batch(CPchar filename)
           { return qt_batch((Pchar)filename, DbTask()); }

   int  hdrline(int line, CPchar txt)
           { return qt_hdrline(line, (Pchar)txt, DbTask()); }

   int  lockcount(int count)
           { return qt_lockcount(count, DbTask()); }

   int  sqlinit(CPchar stmt, Pint epos, Pchar OM_FAR *emsg)
           { return qt_sqlinit((Pchar)stmt, epos, emsg, DbTask()); }

   int  nextrow(CPchar txt)
           { return qt_nextrow((Pchar)txt, DbTask()); }

   int  nextval(Pint count, VALUE OM_FAR *vals)
           { return qt_nextval(count, vals, DbTask()); }

#ifdef WINDOWS
private:
   QDISPLAYPROC fpq_display;    // procedure instance address of q_display
   QPRINTERPROC fpq_printer;    // procedure instance address of q_printer
   QFUNCTIONPROC fpq_function;  // procedure instance address of q_function

public:
   int SetDisplayFunc(QDISPLAYPROC f, OMHINSTANCE hInst);
   int SetPrintFunc(QPRINTERPROC f, OMHINSTANCE hInst);
   int SetQFunc(QFUNCTIONPROC f, OMHINSTANCE hInst);
#endif
};


#ifdef WINDOWS

inline int Qtask :: SetDisplayFunc(QDISPLAYPROC f, OMHINSTANCE hInst)
{
   fpq_display = (QDISPLAYPROC) MakeProcInstance((FARPROC) f, hInst);
   return qt_set_displayfunc(fpq_display, DbTask());
}

inline int Qtask :: SetPrintFunc(QPRINTERPROC f, OMHINSTANCE hInst)
{
   fpq_printer = (QPRINTERPROC) MakeProcInstance((FARPROC) f, hInst);
   return qt_set_printerfunc(fpq_printer, DbTask());
}

inline int Qtask :: SetQFunc(QFUNCTIONPROC f, OMHINSTANCE hInst)
{
   fpq_function = (QFUNCTIONPROC) MakeProcInstance((FARPROC) f, hInst);
   return qt_set_function(fpq_function, DbTask());
}
#endif

#endif
