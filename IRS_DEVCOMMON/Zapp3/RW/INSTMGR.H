#ifndef __RWINSTMGR_H__
#define __RWINSTMGR_H__

/*
 * Declarations for RWInstanceManager --- manages multi-thread and per-process
 * instance data.
 *
 * $Id: instmgr.h@#/main/28  08/21/97 12:20:33  kevinj (TOOLS707_WIN32_971021)
 *
 * (c) Copyright 1989-1997 Rogue Wave Software, Inc.
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
 ***************************************************************************
 */

#include "rw/defs.h"

#if (defined(__DLL__) && defined(__WIN16__)) || defined(RW_MULTI_THREAD)

#if defined(__DLL__) && defined(__WIN16__)
#  include "rw/rwtsd.h"

#elif defined(RW_SOLARIS_THREADS)  // MT & Sun -- assuming Solaris 2.x/Cafe
#  include <thread.h>
   typedef thread_key_t RWTSDKEY;

#elif defined(RW_POSIX_THREADS)
#  include <pthread.h>
   typedef pthread_key_t RWTSDKEY;

#elif defined(__WIN32__)
#  include <windows.h>
   typedef DWORD RWTSDKEY;

#elif defined(__OS2__)
#  if defined(__IBMCPP__)
#    define RWTHREADID *_threadid
#  elif defined(__TURBOC__)
#    define RWTHREADID _threadid
#  endif
#  define INCL_BASE
#  include <os2.h>
#  define rwfar
#  define rwpascal
   typedef unsigned long RWTSDKEY;

   RWTSDKEY rwfar rwpascal 
   RWGetTaskSpecificKey();

   int rwfar rwpascal 
   RWSetTaskSpecificData(RWTSDKEY hKey, void rwfar*);

   void rwfar* rwfar rwpascal 
   RWGetTaskSpecificData(RWTSDKEY hKey);

   void rwfar* rwfar rwpascal 
   RWReleaseTaskSpecificData(RWTSDKEY hKey);
#elif defined( macintosh )	// djk
#  include <Threads.h>
#  define TID		ThreadID	// used for our __OS2__ disguise ;-)
#  define rwfar
#  define rwpascal
   typedef unsigned long RWTSDKEY;

   RWTSDKEY rwfar rwpascal 
   RWGetTaskSpecificKey();

   int rwfar rwpascal 
   RWSetTaskSpecificData(RWTSDKEY hKey, void rwfar*);

   void rwfar* rwfar rwpascal 
   RWGetTaskSpecificData(RWTSDKEY hKey);

   void rwfar* rwfar rwpascal 
   RWReleaseTaskSpecificData(RWTSDKEY hKey);

#else
#  error RW_MULTI_THREAD not supported in this environment
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWInstanceManager
{
public:
  void rwfar*           addValue();       // set instance specific data
  void rwfar*           currentValue();   // get instance specific data
  void                  freeValue();      // release key and discard data

  // Values to be provided and discarded by the specializing class:
  virtual void rwfar*   newValue() = 0;
  virtual void          deleteValue(void rwfar*) = 0;

  RWInstanceManager();
  virtual ~RWInstanceManager();

private:
  RWTSDKEY              tsd_key;
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#else   /* neither 16-bit Windodws DLL nor MultiThread */

//#error RWInstanceManager class not meant to be used outside DLL or MT situation
extern int rwDummyInstMgr;

#endif
#endif  /* __RWINSTMGR_H__ */
