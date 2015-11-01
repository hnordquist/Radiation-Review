#ifndef __ZAPPASSERT_INCLUDED
#define __ZAPPASSERT_INCLUDED
/*
 * (c) Copyright 1991 - 1996 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013.
 * Contractor/Manufacturer is Rogue Wave Software, Inc.,
 * P.O. Box 2328, Corvallis, Oregon 97339.
 *
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 **************************************************************************/

enum {
	zShowAssert=1, zShowPreCond=2, zShowPostCond=4, zShowError=8,
	zShowWarning=16, zOutputAsDebug=256, zOutputAsMessageBox=512,
	zOutputAsDebugBreak=1024
};

#define ZDEBUG_MBOX			 0x0001	// Output error with MessageBox
#define ZDEBUG_DEBUGSTR 	 0x0002	// Output error with OutputDebugStr
#define ZDEBUG_BREAK		 0x0004	// Trap to debugger on error.
#define ZDEBUG_HIDE_ERRORS   0x0008	// Supress Errors.
#define ZDEBUG_HIDE_WARNINGS 0x0010	// Supress Warnings.

#define ZERROR  1
#define ZWARNING  2


#ifdef Z_DB
#define ZAPP_IMPL_ASSERTS	static char *zAppFileName=__FILE__;

#define ZAPP_PRECOND(exp) {if (!(exp)) zAppDoAssert(0,zAppFileName,__LINE__); }
#define ZAPP_ASSERT(exp)  {if (!(exp)) zAppDoAssert(1,zAppFileName,__LINE__); }
#define ZAPP_POSTCOND(exp){if (!(exp)) zAppDoAssert(2,zAppFileName,__LINE__); }

#define ZAPP_ERROR(err)			{ zAppDoError(err,0,ZERROR); }
#define ZAPP_CHECK_ERROR(exp, err) { if (!(exp)) zAppDoError(err,0,ZERROR); }
#define ZAPP_WARNING(err,level) { zAppDoError(err,level,ZWARNING); }

#else
#define ZAPP_IMPL_ASSERTS
#define ZAPP_ASSERT(exp)	
#define ZAPP_PRECOND(exp)	
#define ZAPP_POSTCOND(exp)	
#define ZAPP_ERROR(err)			
#define ZAPP_WARNING(err,level)
#define ZAPP_CHECK_ERROR(exp, err) 

#endif

extern void Z_DLLSPEC zAppDoError(int err, int level, int type);
void Z_DLLSPEC zAppDoAssert(int err,char *fileName,int lineNumber);

#endif



