//-----------------------------------------------------
//  QTASK.CPP                  
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <qtask.hpp>

//-------------------------------------------------------
// Qtask Functions
//-------------------------------------------------------

  Qtask ::
Qtask(CPchar mode) : StoreTask(mode)
{
   qt_opentask(DbTask());
#ifdef WINDOWS
   fpq_display = 0L;    // procedure instance address of q_display
   fpq_printer = 0L;    // procedure instance address of q_printer
   fpq_function = 0L;   // procedure instance address of q_function
#endif
}

  Qtask ::
Qtask(CPchar mode, CRActivateTask a) : StoreTask(mode, a)
{
   qt_opentask(DbTask());
#ifdef WINDOWS
   fpq_display = 0L;    // procedure instance address of q_display
   fpq_printer = 0L;    // procedure instance address of q_printer
   fpq_function = 0L;   // procedure instance address of q_function
#endif
}

  Qtask ::
Qtask(CRActivateTask a) :  StoreTask(a)
{
   qt_opentask(DbTask());
#ifdef WINDOWS
   fpq_display = 0L;    // procedure instance address of q_display
   fpq_printer = 0L;    // procedure instance address of q_printer
   fpq_function = 0L;   // procedure instance address of q_function
#endif
}

  Qtask ::
~Qtask()
{
   CloseAll();
   qt_closetask(DbTask());
#ifdef WINDOWS
   if (fpq_display)
      FreeProcInstance((FARPROC)fpq_display);
   if (fpq_printer)
      FreeProcInstance((FARPROC)fpq_printer);
   if (fpq_function)
      FreeProcInstance((FARPROC)fpq_function);
#endif
}

    T_F
  Qtask ::
OpenAll()
{
   T_F rtn = True;
   char names[512];                     // String for database names

   GetDbNames(Inactive, (Pchar)names);  // loads names into names
   if (qt_open((Pchar)names,GetOpenType(),DbTask()) == S_OKAY)
   {   
      if (!ActivateAll())               // Try to activate all in task list
      {
         CloseAll();
         rtn = False;
      }
   }
   else
      rtn = False;
   return rtn;
}

    void
  Qtask ::
CloseAll()
{
   qt_close(DbTask());                 // Close databases
   DeactivateAll();                    // move them from task db list
}

    void
  Qtask ::
SetQueryPaths(CPchar qdb, CPchar qdf, CPchar qrf)
{
   if (qdb && *qdb)
      qt_qdbpath(qdb, DbTask());
   if (qdf && *qdf)
      qt_qdfpath(qdf, DbTask());
   if (qrf && *qrf)
      qt_qrfpath(qrf, DbTask());
}
