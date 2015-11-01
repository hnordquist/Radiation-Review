#ifndef __RWTOOLERR_H__
#define __RWTOOLERR_H__ 1

/*
 * Error messages for Tools.h++
 *
 * $Id: toolerr.h@#/main/17  06/07/96 19:35:19  griswolf (TOOLS707_WIN32_971021)
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
 *
 * $Log: toolerr.h,v $
 * Revision 7.9  1996/06/07 19:35:19  griswolf
 * Add CRABSS: Attempt to create abstract class from StringID.
 *
 * Revision 7.8  1996/05/07 21:03:20  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.7  1996/02/26 18:04:57  jims
 * Remove extra const qualifier from already const typedef
 *
 * Revision 7.6  1995/09/05 20:08:38  jims
 * Use new copyright macro
 *
 * Revision 7.5  1995/08/17  15:27:01  kevinj
 * Added [NOPERSIST] exception.
 *
 * Revision 7.4  1995/08/17  15:24:28  kevinj
 * Added [NOPERSIST} error.
 *
 * Revision 7.4  1995/08/17  15:24:28  kevinj
 * Added [NOPERSIST} error.
 *
 * Revision 7.3  1995/06/22  15:25:38  griswolf
 * Scopus #69: Added error message NPOSINDEX.
 *
 * Revision 7.2  1994/12/22  21:03:24  jims
 * Remove unnecessary macro recursion for preprocessors which don't
 * support it. (Leave it alone for the others for link compatibility.)
 *
 * Revision 7.1  1994/10/16  03:00:08  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.3  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 *
 * Revision 6.2  1994/06/14  16:42:18  jims
 * Now #defines RWTOOL_XXX to be a function for all builds
 *
 * Revision 6.1  1994/04/15  19:11:16  vriezen
 * Move all files to 6.1
 *
 * Revision 2.4  1994/03/02  19:19:33  jims
 * New message INDEXERR shows invalid index and number of
 * items in collection
 *
 * Revision 2.3  1993/10/27  19:10:54  jims
 * For DLLs, RWTOOL_XXX are exported functions, not objects
 *
 * Revision 2.2  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.1  1993/05/18  00:40:48  keffer
 * Rewrote to use new messaging and exception handling facility.
 *
 */

#include "rw/message.h"

/*
 * RWMsgId is a typedef for RWCatMsg*.
 * For each RWMsgId of the form RWTOOL_XXX there is an RWTOOLMSG_XXX
 *   of type RWCatMsg.
 * Instead of simply defining a variable: RWMsgId RWTOOL_XXX = &RWTOOLMSG_XXX, 
 *   RWTOOL_XXX is #defined to be a function that returns &RWTOOLMSG_XXX.
 * This is done to side-step the problem of compilers (e.g. DEC)
 *   that generate pre-main code for the initialization of address constants.
 * It also serves us in the case of DLLs where variables cannot be exported.
 */

#ifndef RW_NO_CPP_RECURSION
#define RWTOOL_ALLOCOUT     RWTOOL_ALLOCOUT()
#define RWTOOL_BADRE        RWTOOL_BADRE()
#define RWTOOL_CRABS        RWTOOL_CRABS()
#define RWTOOL_FLIST        RWTOOL_FLIST()
#define RWTOOL_ID           RWTOOL_ID()
#define RWTOOL_INDEX        RWTOOL_INDEX()
#define RWTOOL_LOCK         RWTOOL_LOCK()
#define RWTOOL_LONGINDEX    RWTOOL_LONGINDEX()
#define RWTOOL_MAGIC        RWTOOL_MAGIC()
#define RWTOOL_NEVECL       RWTOOL_NEVECL()
#define RWTOOL_NOCREATE     RWTOOL_NOCREATE()
#define RWTOOL_NOTALLOW     RWTOOL_NOTALLOW()
#define RWTOOL_READERR      RWTOOL_READERR()
#define RWTOOL_REF          RWTOOL_REF()
#define RWTOOL_SEEKERR      RWTOOL_SEEKERR()
#define RWTOOL_STREAM       RWTOOL_STREAM()
#define RWTOOL_SUBSTRING    RWTOOL_SUBSTRING()
#define RWTOOL_UNLOCK       RWTOOL_UNLOCK()
#define RWTOOL_WRITEERR     RWTOOL_WRITEERR()
#define RWTOOL_INDEXERR     RWTOOL_INDEXERR()
#define RWTOOL_NPOSINDEX    RWTOOL_NPOSINDEX()
#define RWTOOL_NOPERSIST    RWTOOL_NOPERSIST()
#define RWTOOL_CRABSS       RWTOOL_CRABSS()
#else
#define RWTOOL_ALLOCOUT     rwtool_ALLOCOUT()
#define RWTOOL_BADRE        rwtool_BADRE()
#define RWTOOL_CRABS        rwtool_CRABS()
#define RWTOOL_FLIST        rwtool_FLIST()
#define RWTOOL_ID           rwtool_ID()
#define RWTOOL_INDEX        rwtool_INDEX()
#define RWTOOL_LOCK         rwtool_LOCK()
#define RWTOOL_LONGINDEX    rwtool_LONGINDEX()
#define RWTOOL_MAGIC        rwtool_MAGIC()
#define RWTOOL_NEVECL       rwtool_NEVECL()
#define RWTOOL_NOCREATE     rwtool_NOCREATE()
#define RWTOOL_NOTALLOW     rwtool_NOTALLOW()
#define RWTOOL_READERR      rwtool_READERR()
#define RWTOOL_REF          rwtool_REF()
#define RWTOOL_SEEKERR      rwtool_SEEKERR()
#define RWTOOL_STREAM       rwtool_STREAM()
#define RWTOOL_SUBSTRING    rwtool_SUBSTRING()
#define RWTOOL_UNLOCK       rwtool_UNLOCK()
#define RWTOOL_WRITEERR     rwtool_WRITEERR()
#define RWTOOL_INDEXERR     rwtool_INDEXERR()
#define RWTOOL_NPOSINDEX    rwtool_NPOSINDEX()
#define RWTOOL_NOPERSIST    rwtool_NOPERSIST()
#define RWTOOL_CRABSS       rwtool_CRABSS()
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

extern RWMsgId rwexport RWTOOL_ALLOCOUT;
extern RWMsgId rwexport RWTOOL_BADRE;
extern RWMsgId rwexport RWTOOL_CRABS;
extern RWMsgId rwexport RWTOOL_FLIST;
extern RWMsgId rwexport RWTOOL_ID;
extern RWMsgId rwexport RWTOOL_INDEX;
extern RWMsgId rwexport RWTOOL_LOCK;
extern RWMsgId rwexport RWTOOL_LONGINDEX;
extern RWMsgId rwexport RWTOOL_MAGIC;
extern RWMsgId rwexport RWTOOL_NEVECL;
extern RWMsgId rwexport RWTOOL_NOCREATE;
extern RWMsgId rwexport RWTOOL_NOTALLOW;
extern RWMsgId rwexport RWTOOL_READERR;
extern RWMsgId rwexport RWTOOL_REF;
extern RWMsgId rwexport RWTOOL_SEEKERR;
extern RWMsgId rwexport RWTOOL_STREAM;
extern RWMsgId rwexport RWTOOL_SUBSTRING;
extern RWMsgId rwexport RWTOOL_UNLOCK;
extern RWMsgId rwexport RWTOOL_WRITEERR;
extern RWMsgId rwexport RWTOOL_INDEXERR;
extern RWMsgId rwexport RWTOOL_NPOSINDEX;
extern RWMsgId rwexport RWTOOL_NOPERSIST;
extern RWMsgId rwexport RWTOOL_CRABSS;

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif  /*  __RWTOOLERR_H__ */
