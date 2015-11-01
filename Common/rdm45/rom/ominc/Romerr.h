//-----------------------------------------------------
//  ROMERR.H
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#ifndef _ROMERR_H_
#define _ROMERR_H_

static char *rom_error[] = 
{
   /* S_BADERR      -6000 */ "bad ROM error code",
   /* S_NOSESS      -6001 */ "can not begin database session",
   /* S_FIRSTOPEN   -6002 */ "first database open failed",
   /* S_INCROPEN    -6003 */ "incremental database open failed",
   /* S_NODBNAME    -6004 */ "no database name specified",
   /* S_NOFREE      -6005 */ "memory was not unlocked and can not be freed",
   /* S_INVSTASK    -6006 */ "invalid StoreTask",
   /* S_BLOBREAD    -6007 */ "blob size not synced with stored length",
   /* S_NOBLOBOWN   -6008 */ "blob has no specified owner",
   /* S_BLOBWRITE   -6009 */ "blob write failed",
   /* S_INVRTYPE    -6010 */ "invalid polymorph member record type",
   /* S_TRANSERR    -6011 */ "TransAction is in error",
   /* S_GLACTIVE    -6012 */ "GroupLock is currently active",
   /* S_GLNOLOCK    -6013 */ "GroupLock is not currently locked",
   /* S_DBISOPEN    -6014 */ "database is currently open",
   /* S_DBACTIVE    -6015 */ "a database is already active",
   /* S_MAXDB       -6016 */ "maximum number of StoreDbs exceeded for this StoreTask",
   /* S_ACTALL      -6017 */ "all databases could not be activated",
   /* S_SWAPFAIL    -6018 */ "database swap failed",
   /* S_CLONE       -6019 */ "missing virtual Clone(), use CLONE macro",
   /* S_QUERYERR    -6020 */ "OmQuery error",
   /* S_TWODBS      -6021 */ "can not have two databases with same name managed by one task",
   /* S_NOTNAV      -6022 */ "object has not been navigated to"
   /* S_SERVEREXT   -6023 */
};

#endif // _ROMERR_H_

