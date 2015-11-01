/*
 * Template definitions for RWTPtrOrderedVector<TP>
 *
 * $Id: xpordvec.cc@#/main/6  09/05/96 15:20:49  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: xpordvec.cc,v $
 * Revision 7.7  1996/09/05 15:20:49  griswolf
 * Scopus #4418: Fix problem with diff type and size of size_t.
 *
 * Revision 7.6  1996/08/26 21:52:13  hart
 * HP (aCC) compiler port
 *
 * Revision 7.5  1996/06/26 22:25:26  griswolf
 * Use register size_t instead of register (default int) two places.
 *
 * Revision 7.4  1996/03/16 15:15:37  jims
 * Port non-stdlib template persistence to DEC and Watcom
 *
 * Revision 7.3  1996/02/12 21:03:06  kevinj
 * Switched stream and class args in rw*Guts.
 *
 * Revision 7.2  1996/01/27 01:36:34  kevinj
 * Simple External Template Persistence (ETP).
 *
 * Revision 7.1  1995/11/13 12:30:40  jims
 * Move to revision 7.1
 *
 * Revision 7.3  1995/09/17 07:50:07  jims
 * increment nitems_ before trying to access new last element
 *
 * Revision 7.2  1995/09/05  20:23:04  jims
 * Use new copyright macro
 *
 * Revision 7.1  1994/10/16  03:17:29  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.2  1994/07/12  20:04:35  vriezen
 * Updated Copyright.
 *
 * Revision 6.1  1994/04/15  19:47:53  vriezen
 * Move all files to 6.1
 *
 * Revision 6.1.7.2  1994/04/20  22:16:14  vriezen
 * Bug #337 Fix bounds check for ordered and sorted vectors.
 *
 * Revision 6.1.7.1  1994/04/15  22:45:41  vriezen
 * Version 7, Initial code
 *
 * Revision 6.1  1994/04/15  19:47:53  vriezen
 * Move all files to 6.1
 *
 * Revision 1.5  1994/01/12  03:09:17  jims
 * Add constness to T* parameters where appropriate
 *
 * Revision 1.4  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 1.3  1993/04/09  19:35:45  keffer
 * Indexing is now done using size_t
 *
 * Revision 1.2  1993/03/24  01:29:51  keffer
 * Indexing operations now used unsigned
 *
 * Revision 1.1  1993/01/28  02:39:38  keffer
 * Initial revision
 *
 *
 ***************************************************************************
 */

template <class TP> void
RWTPtrOrderedVector<TP>::clearAndDestroy()
{
  for (size_t i=0; i<entries(); i++)
  {
    delete (*this)(i);
  }
  clear();
}

template <class TP> TP*
RWTPtrOrderedVector<TP>::find(const TP* p) const
{
  size_t i=index(p);
  return i == RW_NPOS ? rwnil : (*this)(i);
}

template <class TP> size_t
RWTPtrOrderedVector<TP>::index(const TP* val) const
{
  for (register size_t i=0; i<nitems_; i++)
    if (*(*this)(i) == *val)
      return i;
  return RW_NPOS;
}

// Insert value at position "ipt"; value formerly at "ipt"
// gets moved to "ipt+1".
template <class TP> void
RWTPtrOrderedVector<TP>::insertAt(size_t ipt, TP* val)
{
  if (ipt > 0)
    boundsCheck(ipt-1);

  // Check for overflow:
  if(nitems_>=RWTPtrVector<TP>::length())
    this->reshape(nitems_ + RWDEFAULT_RESIZE);

  // Slide right (could be very expensive)
  // Need to increment nitems_ before indexing to avoid bounds error!
  for(register size_t i=nitems_++; i>ipt; i--)
    (*this)(i) = (*this)(i-1);

  (*this)(ipt) = val;
}

template <class TP> size_t
RWTPtrOrderedVector<TP>::occurrencesOf(const TP* val) const
{
  size_t count = 0;
  for (register size_t i=0; i<nitems_; i++)
    if (*(*this)(i) == *val)
      ++count;
  return count;
}

/*
 * Remove and return the first occurrence of an object with the
 * same value as the object pointed to by "p".
 */
template <class TP> TP*
RWTPtrOrderedVector<TP>::remove(const TP* p)
{
  size_t idx = index(p);
  return idx == RW_NPOS ? rwnil : removeAt(idx);
}

/*
 * Remove all occurrences of the value
 */
template <class TP> size_t
RWTPtrOrderedVector<TP>::removeAll(const TP* val)
{
  size_t j = 0;
  for (register size_t i=0; i<nitems_; i++) {
    // Are they unequal?  If so, this value should be saved.
    if (!(*(*this)(i) == *val)) {
      // Avoid the copy if possible:
      if (i!=j) (*this)(j) = (*this)(i);
      ++j;
    }
  }

  size_t nremoved = nitems_-j;
  nitems_ = j;
  return nremoved;
}

template <class TP> TP*
RWTPtrOrderedVector<TP>::removeAt(size_t ipt)
{
  boundsCheck(ipt);
  //  RWPRECONDITION2(ipt<nitems_, "RWTPtrOrderedVector::removeAt(size_t): index out of range");

  TP* temp = (*this)(ipt);

  // Slide left (could be very expensive):
  for(register size_t i=ipt; i<nitems_-1; i++)
    (*this)(i) = (*this)(i+1);

  --nitems_;
  return temp;
}

template <class TP> void
RWTPtrOrderedVector<TP>::boundsCheck(size_t i) const {
  if (i>=nitems_)
    RWTHROW( RWBoundsErr(RWMessage(RWTOOL_INDEXERR, (unsigned)i, (unsigned)nitems_) ));
}


#include "rw/epersist.h"

RWDEFINE_PERSISTABLE_TEMPLATE(RWTPtrOrderedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
RWDEFINE_PERSIST_TEMPLATE_IO(RWTPtrOrderedVector,RWpistream,RWpostream)
RWDEFINE_PERSIST_TEMPLATE_IO(RWTPtrOrderedVector,RWbistream,RWbostream)
RWDEFINE_PERSIST_TEMPLATE_IO(RWTPtrOrderedVector,RWeistream,RWeostream)
#endif     


template <class T, class S>
void rwSaveGuts(S& str, const RWTPtrOrderedVector<T>& c)
{
#ifdef RW_FIX_XSIZE_T
  str.putSizeT(c.entries());
#else
  str << c.entries();
#endif
  for (size_t i=0; i < c.entries(); ++i)
    str << *c[i];
}

template <class T, class S>
void rwRestoreGuts(S& str, RWTPtrOrderedVector<T>& c)
{
  size_t count;
#ifdef RW_FIX_XSIZE_T
  str.getSizeT(count);
#else
  str >> count;
#endif
  c.clear();
  for (size_t i=0; i < count; ++i) {
    T* p;
    str >> p;
    c.insert(p);
  }
}


