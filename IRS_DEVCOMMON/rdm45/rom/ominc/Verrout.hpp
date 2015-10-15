//-----------------------------------------------------
//  VERROUT.HPP
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------

#ifndef _VERROUT_HPP_
#define _VERROUT_HPP_

// Error package sent to user's error handler
typedef struct ROMErrInfo_S
{
   short     errnum;
   char      errtxt[256];
#ifdef OM_RDS
   RDM_SESS  hSess;
   Pvoid     svcptr;
#endif
} ROM_ERRINFO;


// ROM-specific status codes
#define S_OUTOFRANGE 100       // used to define the upper bound of StoreObjArray

// ROM-specific error codes
#define S_BADERR     (-6000)   // bad ROM error code
#define S_NOSESS     (-6001)   // can not begin database session
#define S_FIRSTOPEN  (-6002)   // first database open failed
#define S_INCROPEN   (-6003)   // incremental database open failed
#define S_NODBNAME   (-6004)   // no database name specified
#define S_NOFREE     (-6005)   // memory was not unlocked and can not be freed
#define S_INVSTASK   (-6006)   // invalid StoreTask
#define S_BLOBREAD   (-6007)   // blob size not synced with stored length
#define S_NOBLOBOWN  (-6008)   // blob has no specified owner
#define S_BLOBWRITE  (-6009)   // blob write failed
#define S_INVRTYPE   (-6010)   // invalid polymorph member record type
#define S_TRANSERR   (-6011)   // TransAction is in error
#define S_GLACTIVE   (-6012)   // GroupLock is currently active
#define S_GLNOLOCK   (-6013)   // GroupLock is not currently locked
#define S_DBISOPEN   (-6014)   // Database is currently open
#define S_DBACTIVE   (-6015)   // a database is already active
#define S_MAXDB      (-6016)   // maximum number of StoreDbs exceeded for this StoreTask
#define S_ACTALL     (-6017)   // all databases could not be activated
#define S_SWAPFAIL   (-6018)   // database swap failed
#define S_CLONE      (-6019)   // missing virtual Clone(), use CLONE macro
#define S_QUERYERR   (-6020)   // OmQuery error  
#define S_TWODBS     (-6021)   // can not have two databases with same name managed by one task
#define S_NOTNAV     (-6022)   // object has not been navigated to


extern Pchar ROMErrorLookup(int errnum);
typedef void (OMEXTERNAL * ROM_ERRORPROC) (ROM_ERRINFO OM_FAR &);

#ifdef OM_RDS
extern void OMEXTERNAL ROMErrorHandler(short errnum, RDM_SESS hSess, void OM_FAR * svcptr);
#define ROMERROR(a)    ROMErrorHandler(a, 0, (Pvoid)NULL)
#else // OM_RDM
extern void OMEXTERNAL ROMErrorHandler(int errnum, Pchar errtxt);
#define ROMERROR(a)    ROMErrorHandler(a, ROMErrorLookup(a))
#define ROMQERROR(a,b) ROMErrorHandler(a, b)
#endif

#endif // _VERROUT_HPP_
