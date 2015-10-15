//---------------------------------------------------------
//  ACTIVATE.CPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
extern "C" {
#include <stdlib.h>
}
#include <activate.hpp>

  ActivateTask ::
ActivateTask(Pchar srv, Pchar usr, Pchar pwd)
{
#ifdef OM_RDS
   pczUser = pczServer = pczPassword = (Pchar)NULL;
   SetServer(srv);
   SetUser(usr);
   SetPassword(pwd);
#else // OM_RDM
   pczDbd = pczDbf = pczTaf = pczLog = pczUid = NULL;
#endif
}


  ActivateTask ::
~ActivateTask()
{
#ifdef OM_RDS
   FreeStr(pczServer);
   FreeStr(pczUser);
   FreeStr(pczPassword);
#else // OM_RDM
   FreeStr(pczDbd);
   FreeStr(pczDbf);
   FreeStr(pczTaf);
   FreeStr(pczLog);
   FreeStr(pczUid);
#endif
}

Pchar OM_EXPORT DupStr(Pchar OM_FAR * dst, CPchar src)
{
   if (*dst)
      om_free(*dst);

   return (*dst = (src ? om_strdup(src) : (Pchar)NULL));
}

void FreeStr(Pchar s)
{
   if (s)
      om_free(s);
}


