/*---------------------------------------------------------
//  IMPLLOCK.H
//---------------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//--------------------------------------------------------- */
#ifndef _IMPLLOCK_H_
#define _IMPLLOCK_H_

#ifdef OM_RDS
/* lock mode information flags */
#define OMF_DIRTY_READS          0x0001
#define OMF_FREEABLE_READLOCKS   0x0010
#define OMF_IMPLICIT_READLOCKS   0x0100
#define OMF_IMPLICIT_WRITELOCKS  0x1000
/* lock mode information checking macros */
#define OM_DIRTY_READS(a)         (a & OMF_DIRTY_READS)
#define OM_FREEABLE_READLOCKS(a)  (a & OMF_DIRTY_READS)
#define OM_IMPLICIT_READS(a)      (!(a & OMF_DIRTY_READS) && (a & OMF_IMPLICIT_READLOCKS))
#define OM_IMPLICIT_WRITES(a)     (a & OMF_IMPLICIT_WRITELOCKS)
#endif


#endif


