#ifndef __RWCOLLSTR_H__
#define __RWCOLLSTR_H__

/*
 * RWCollectableString --- collectable strings
 *
 * $Id: collstr.h@#/main/16  05/08/96 16:58:21  kyle (TOOLS707_WIN32_971021)
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
 * $Log: collstr.h,v $
 * Revision 7.8  1996/05/08 16:58:21  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.6  1996/02/18 01:37:29  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.5  1996/01/04 02:38:20  jims
 * added disambiguating inserter for estreams
 *
 * Revision 7.4  1995/09/05 18:15:37  jims
 * Use new copyright macro
 *
 * Revision 7.3  1995/04/18  20:36:15  griswolf
 * added disambiguating stream inserters
 *
 * Revision 7.2  1994/10/21  23:23:38  vriezen
 * Backed out fix for ambiguity.
 *
 * Revision 7.1  1994/10/16  00:55:03  josh
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
 * Revision 6.1  1994/04/15  19:09:42  vriezen
 * Move all files to 6.1
 *
 * Revision 6.1.7.2  1994/04/25  21:32:53  vriezen
 * Added oper<< and oper>> for RWFile and RWv?stream classes.
 *
 * Revision 6.1.7.1  1994/04/15  22:44:44  vriezen
 * Version 7, Initial code
 *
 * Revision 6.1  1994/04/15  19:09:42  vriezen
 * Move all files to 6.1
 *
 * Revision 2.5  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.4  1993/07/28  22:41:54  keffer
 * Changed to reflect new RWCString interface.
 *
 * Revision 2.3  1993/04/09  19:35:45  keffer
 * Indexing is now done using size_t
 *
 * Revision 2.2  1993/03/17  21:05:21  keffer
 * Return type of binaryStoreSize() is now RWspace
 *
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.5   22 May 1992 17:04:10   KEFFER
 * Now uses RWDECLARE_COLLECTABLE() macro
 * 
 *    Rev 1.4   23 Apr 1992 08:42:40   KEFFER
 * Added RWCollectableString(const char*, unsigned) constructor.
 * 
 *    Rev 1.3   01 Mar 1992 15:46:06   KEFFER
 * Now uses RWCString instead of RWString.
 * 
 *    Rev 1.1   28 Oct 1991 09:08:12   keffer
 * Changed inclusions to <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 08:13:56   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/cstring.h"
#include "rw/collect.h"

/****************************************************************
 *                                                              *
 *                      RWCollectableString                     *
 *                                                              *
 ****************************************************************/

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWCollectableString : public RWCollectable, public RWCString {

  RWDECLARE_COLLECTABLE(RWCollectableString)

public:

  RWCollectableString();
  RWCollectableString(const char* a) :          RWCString(a)   { }
  RWCollectableString(const char* a, size_t N) : RWCString(a,N) { }
  RWCollectableString(char c, size_t N) :       RWCString(c,N) { }
  RWCollectableString(const RWCString& s) :     RWCString(s)   { }
  RWCollectableString(const RWCSubString& s) :  RWCString(s)   { }

  virtual ~RWCollectableString();
  /* Virtual functions inherited from RWCollectable */
  virtual RWspace               binaryStoreSize() const {return RWCString::binaryStoreSize();}
  virtual int                   compareTo(const RWCollectable*) const;
  virtual unsigned              hash() const;
  virtual RWBoolean             isEqual(const RWCollectable*) const;
  virtual void                  restoreGuts(RWvistream&);
  virtual void                  restoreGuts(RWFile&);
  virtual void                  saveGuts(RWvostream&) const;
  virtual void                  saveGuts(RWFile&) const;
};

/*
 * The following functions help compilers disambiguate in the face of 
 * multiple derivation, casting operators, etc.
 */

inline ostream& operator<<(ostream& os, const RWCollectableString& cts)
{ return os << (const RWCString&)cts; }

inline istream& operator>>(istream& is, RWCollectableString& cts)
{ return is >> (RWCString&)cts; }

#ifndef RW_NO_RESOLVE_TEMPL_AMBIGUITY
#include "rw/strmshft.h"
class RWExport RWpostream;
class RWExport RWbostream;
class RWExport RWeostream;

RW_PROVIDE_DVSTREAM_INSERTER(RWpostream,RWCollectableString)
RW_PROVIDE_DVSTREAM_INSERTER(RWbostream,RWCollectableString)
RW_PROVIDE_DVSTREAM_INSERTER(RWeostream,RWCollectableString)
#endif /* RW_NO_RESOLVE_TEMPL_AMBIGUITY */

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWCOLLSTR_H__ */
