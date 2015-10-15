#ifndef __RWITERATOR_H__
#define __RWITERATOR_H__
/*
 * Abstract base class for Iterator classes.
 *
 * $Id: iterator.h@#/main/14  05/07/96 23:25:52  kyle (TOOLS707_WIN32_971021)
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
 * $Log: iterator.h,v $
 * Revision 7.5  1996/05/07 23:25:52  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.4  1996/02/19 10:36:15  jims
 * Remove extraneous 'f' character
 *
 * Revision 7.3  1996/02/18 01:45:14  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.2  1995/09/05 19:09:34  jims
 * Use new copyright macro
 *
 * Revision 7.1  1994/10/16  02:51:50  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.6  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 *
 * Revision 6.5  1994/06/22  23:35:59  vriezen
 * Remove RW_DEFINE_HOME and RW_DECLARE_HOME, no longer needed.
 *
 * Revision 6.4  1994/06/18  02:16:02  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.3  1994/06/16  03:34:55  vriezen
 * Added RW_DECLARE_HOME
 *
 * Revision 6.2  1994/04/15  21:12:29  vriezen
 * Backed out (until next release) RW_DEFINE_HOME and RW_DECLARE_HOME support.
 *
 * Revision 6.1  1994/04/15  19:10:24  vriezen
 * Move all files to 6.1
 *
 * Revision 2.3  1994/03/23  20:42:45  vriezen
 * Add RW_DECLARE_HOME macro to RWIterator
 *
 * Revision 2.2  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 * 
 * Rev 1.2   18 Feb 1992 09:54:24   KEFFER
 * 
 * Rev 1.1   28 Oct 1991 09:08:18   keffer
 * Changed inclusions to <rw/xxx.h>
 * 
 *  Rev 1.0   28 Jul 1991 08:15:34   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/defs.h"

class RWExport RWCollectable;

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWIterator {
public:
  virtual ~RWIterator();
  virtual RWCollectable*  findNext(const RWCollectable*) = 0; //next match
  virtual RWCollectable*  key() const = 0;            //Return current key
  virtual RWCollectable*  operator()() = 0;           //Advance iterator
  virtual void            reset() = 0;
};        

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWITERATOR_H__ */
