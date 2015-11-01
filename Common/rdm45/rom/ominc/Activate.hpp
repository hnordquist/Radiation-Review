//---------------------------------------------------------
//  ACTIVATE.HPP
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//---------------------------------------------------------
#ifndef _ACTIVATE_HPP_
#define _ACTIVATE_HPP_

#include "vpp.hpp"
extern "C" {
#include <stdio.h>
#include <malloc.h>
#include <string.h>
}

OMCLASS(ActivateTask);

void FreeStr(Pchar s);
Pchar OM_EXPORT DupStr(Pchar OM_FAR * dst, CPchar src);

#ifdef OM_RDS
class OM_EXPORT ActivateTask
{
private:
   Pchar pczUser;
   Pchar pczServer;
   Pchar pczPassword;

public:
            ActivateTask(Pchar srv = (Pchar)NULL, Pchar usr = (Pchar)NULL, Pchar pwd = (Pchar)NULL);
   virtual ~ActivateTask();

   Pchar GetUser()     const { return pczUser; }
   Pchar GetServer()   const { return pczServer; }
   Pchar GetPassword() const { return pczPassword; }

   Pchar SetUser(CPchar u)     { return DupStr(&pczUser, u); }
   Pchar SetServer(CPchar s)   { return DupStr(&pczServer, s); }
   Pchar SetPassword(CPchar p) { return DupStr(&pczPassword, p); }

   // stub functions for cross-compilation with RDM & Velocis
   Pchar GetDbdPath()  const { return (Pchar)NULL; }
   Pchar GetDbfPath()  const { return (Pchar)NULL; }
   Pchar GetTafPath()  const { return (Pchar)NULL; }
   Pchar GetLogPath()  const { return (Pchar)NULL; }
   Pchar GetUID()      const { return (Pchar)NULL; }

   Pchar SetDbdPath(CPchar p)  { return (Pchar)p; }
   Pchar SetDbfPath(CPchar p)  { return (Pchar)p; }
   Pchar SetTafPath(CPchar p)  { return (Pchar)p; }
   Pchar SetLogPath(CPchar p)  { return (Pchar)p; }
   Pchar SetUID(CPchar p)      { return (Pchar)p; }
};

#else // OM_RDM

class OM_EXPORT ActivateTask
{
private:
   Pchar pczDbd;
   Pchar pczDbf;
   Pchar pczTaf;
   Pchar pczLog;
   Pchar pczUid;

public:
            ActivateTask(Pchar srv = (Pchar)NULL, Pchar usr = (Pchar)NULL, Pchar pwd = (Pchar)NULL);
   virtual ~ActivateTask();

   Pchar GetDbdPath()  const { return pczDbd; }
   Pchar GetDbfPath()  const { return pczDbf; }
   Pchar GetTafPath()  const { return pczTaf; }
   Pchar GetLogPath()  const { return pczLog; }
   Pchar GetUID()      const { return pczUid; }

   Pchar SetDbdPath(CPchar p)  { return DupStr(&pczDbd, p); }
   Pchar SetDbfPath(CPchar p)  { return DupStr(&pczDbf, p); }
   Pchar SetTafPath(CPchar p)  { return DupStr(&pczTaf, p); }
   Pchar SetLogPath(CPchar p)  { return DupStr(&pczLog, p); }
   Pchar SetUID(CPchar p)      { return DupStr(&pczUid, p); }

   // stub functions for cross-compilation with RDM & Velocis
   Pchar GetPassword() const { return (Pchar)NULL; }
   Pchar GetUser()     const { return (Pchar)NULL; }
   Pchar GetServer()   const { return (Pchar)NULL; }

   Pchar SetUser(CPchar u)     { return (Pchar)u; }
   Pchar SetPassword(CPchar p) { return (Pchar)p; }
   Pchar SetServer(CPchar s)   { return (Pchar)s; }
};
#endif // OM_RDS

#endif

