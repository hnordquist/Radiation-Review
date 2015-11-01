#ifndef __RWCOLLINT_H__
#define __RWCOLLINT_H__

/*
 * RWCollectableInt --- RWCollectable Integers.
 *
 * $Id: collint.h@#/main/15  08/14/96 17:52:21  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: collint.h,v $
 * Revision 7.9  1996/08/14 17:52:21  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.8  1996/05/08 18:54:38  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.6  1996/02/18 01:37:28  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.5  1996/01/04 02:37:46  jims
 * added disambiguating << and >> methods for estreams
 *
 * Revision 7.4  1995/09/05 18:15:07  jims
 * Use new copyright macro
 *
 * Revision 7.3  1995/04/18  20:33:15  griswolf
 * added disambiguating << and >> methods
 *
 * Revision 7.2  1994/10/21  23:21:24  vriezen
 * Back out fix for ambiguity.
 *
 * Revision 7.1  1994/10/16  00:54:20  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.3  1994/07/12  18:45:56  vriezen
 * Update Copyright notice
 *
 * Revision 6.1.7.3  1994/06/18  03:12:09  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.2  1994/06/18  03:11:05  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.1  1994/04/15  19:09:41  vriezen
 * Move all files to 6.1
 *
 * Revision 6.1.7.2  1994/04/25  21:32:53  vriezen
 * Added oper<< and oper>> for RWFile and RWv?stream classes.
 *
 * Revision 6.1.7.1  1994/04/15  22:44:43  vriezen
 * Version 7, Initial code
 *
 * Revision 6.1  1994/04/15  19:09:41  vriezen
 * Move all files to 6.1
 *
 * Revision 2.3  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.2  1993/03/17  21:05:21  keffer
 * Return type of binaryStoreSize() is now RWspace
 *
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.3   22 May 1992 17:04:10   KEFFER
 * Now uses RWDECLARE_COLLECTABLE() macro
 * 
 *    Rev 1.2   18 Feb 1992 09:54:14   KEFFER
 * 
 *    Rev 1.1   28 Oct 1991 09:08:10   keffer
 * Changed inclusions to <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 08:13:50   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/rwint.h"
#include "rw/collect.h"

/****************************************************************
 *                                                              *
 *                      RWCollectableInt                        *
 *                                                              *
 ****************************************************************/

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWCollectableInt : public RWCollectable, public RWInteger {

  RWDECLARE_COLLECTABLE(RWCollectableInt)

public:

  RWCollectableInt();
  RWCollectableInt(int j) :     RWInteger(j) { }
  RWCollectableInt(const RWCollectableInt& ci) : RWInteger(ci.value()) { }
  virtual ~RWCollectableInt();

  RWBoolean                     operator==(const RWCollectableInt& c) const
    {return value()==c.value();}

  virtual RWspace               binaryStoreSize() const {return RWInteger::binaryStoreSize();}
  virtual int                   compareTo(const RWCollectable*) const;
  virtual unsigned              hash() const;
  virtual RWBoolean             isEqual(const RWCollectable*) const;
  virtual void                  restoreGuts(RWvistream&);
  virtual void                  restoreGuts(RWFile&);
  virtual void                  saveGuts(RWvostream&) const;
  virtual void                  saveGuts(RWFile&) const;
};        

/* add the following functions to help compilers disambiguate */
#ifndef RW_NO_IOSTD
#  include <iosfwd>
#else
  class ostream;
  class istream;
#endif
  
inline ostream& operator<<(ostream& os,const RWCollectableInt& cti)
{ return os << (const RWInteger&)cti; }

inline istream& operator>>(istream& is, RWCollectableInt& cti)
{ return is >> (RWInteger&)cti; }

#ifndef RW_NO_RESOLVE_TEMPL_AMBIGUITY
#include "rw/strmshft.h"
class RWExport RWpostream;
class RWExport RWbostream;
class RWExport RWeostream;

class RWExport RWpistream;
class RWExport RWbistream;
class RWExport RWeistream;

RW_PROVIDE_DVSTREAM_INSERTER(RWpostream,RWCollectableInt)
RW_PROVIDE_DVSTREAM_INSERTER(RWbostream,RWCollectableInt)
RW_PROVIDE_DVSTREAM_INSERTER(RWeostream,RWCollectableInt)

RW_PROVIDE_DVSTREAM_EXTRACTOR(RWpistream,RWCollectableInt)
RW_PROVIDE_DVSTREAM_EXTRACTOR(RWbistream,RWCollectableInt)
RW_PROVIDE_DVSTREAM_EXTRACTOR(RWeistream,RWCollectableInt)
#endif /* RW_NO_RESOLVE_TEMPL_AMBIGUITY */

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif


#endif /* __RWCOLLINT_H__ */
