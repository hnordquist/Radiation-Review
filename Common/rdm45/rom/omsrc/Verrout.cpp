//-----------------------------------------------------
//  VERROUT.CPP 
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>
#include <verrout.hpp>
#include <storetsk.hpp>
#include <romerr.h>

extern "C" {
#include <stdio.h>
#include <string.h>
};


Pchar ROMErrorLookup(int errnum)
{
   // check for valid error code
   if (errnum < -6999 || errnum > -6000)
      return (Pchar)rom_error[0];

   return (Pchar)rom_error[((-errnum)-6000)];
}   


#ifdef OM_RDS
void OMEXTERNAL ROMErrorHandler(short errnum, RDM_SESS hSess, void OM_FAR *svcptr)
#else // OM_RDM
void OMEXTERNAL ROMErrorHandler(int errnum, Pchar errtxt)
#endif
{
   ROM_ERRINFO errinfo;
   errinfo.errnum = errnum;

#ifdef OM_RDS
   if (errnum <= -6000 && errnum > -7000)
      om_strcpy((Pchar)errinfo.errtxt, ROMErrorLookup(errnum));
   else
      s_errinfo(errnum, (Pchar)errinfo.errtxt, 255, hSess);
//      d_errinfo((Pchar)errinfo.errtxt, 255, hSess);

   errinfo.hSess = hSess;
   errinfo.svcptr = NULL;
#else
   om_strcpy((Pchar)errinfo.errtxt, errtxt);
#endif

   ROM_ERRORPROC errhandler;
   PStoreTask    pVTask = StoreTask::GetCurrTask();
   if (pVTask && (errhandler = pVTask->GetErrorHandler()))
   {
      // call user's error handler
      errhandler(errinfo);
   }
   else
   {
      // have ROM display the error
      char header[60];
      sprintf(header, "Raima Object Manager Error: %d", errnum);

#ifdef WINDOWS
      MessageBox(GetFocus(), (Pchar)errinfo.errtxt, (Pchar)header, 0);
#else
      fprintf(stderr, "%s\n%s\npress <return> to continue", header, errinfo.errtxt);
      fflush(stderr);
      char ch;
      do
      {
         ch = (char) getchar();
      } while ((ch != '\n') && (ch != EOF));
#endif
   }
}

