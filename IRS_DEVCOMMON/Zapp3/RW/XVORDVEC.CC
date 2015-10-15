/*
 * Template definitions for RWTValOrderedVector<T>
 *
 * $Id: xvordvec.cc@#/main/6  09/05/96 15:20:49  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: xvordvec.cc,v $
 * Revision 7.7  1996/09/05 15:20:49  griswolf
 * Scopus #4418: Fix problem with diff type and size of size_t.
 *
 * Revision 7.6  1996/08/26 21:55:15  hart
 * HP (aCC) compiler port
 *
 * Revision 7.5  1996/05/30 02:47:47  hart
 * fully qualified name references
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
 * Revision 7.10  1995/09/17  07:49:11  jims
 * Don't adjust nitems_ until no possibility of bounds error
 *
 * Revision 7.9  1995/09/13  04:08:33  jims
 * Uncomment useful code
 *
 * Revision 7.8  1995/09/07  02:06:28  jims
 * Remove more diagnostic code
 *
 * Revision 7.7  1995/09/07  02:04:11  jims
 * Remove diagnostic code
 *
 * Revision 7.6  1995/09/05  21:25:43  jims
 * Use new copyright macro
 *
 * Revision 7.5  1995/09/01  19:56:16  jims
 * Scopus 1940: Changed variable to size_t to avoid msvc warning
 *
 * Revision 7.4  1995/08/01  20:28:11  jims
 * Move clear() out of line
 *
 * Revision 7.3  1995/08/01  20:00:39  jims
 * Scopus #2019.  Reset unused cells to default value when
 * removing or clearing elements.
 *
 * Revision 7.2  1995/06/27  17:10:44  jims
 * Use <class T> instead of <class TP> in boundsCheck mf template
 *
 * Revision 7.1  1994/10/16  03:22:16  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.2  1994/07/12  20:04:35  vriezen
 * Updated Copyright.
 *
 * Revision 6.1  1994/04/15  19:48:23  vriezen
 * Move all files to 6.1
 *
 * Revision 6.1.7.2  1994/04/20  22:14:27  vriezen
 * Bug #337 Fix bounds check for ordered and sorted vectors.
 *
 * Revision 6.1.7.1  1994/04/15  22:45:57  vriezen
 * Version 7, Initial code
 *
 * Revision 6.1  1994/04/15  19:48:23  vriezen
 * Move all files to 6.1
 *
 * Revision 1.5  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 1.4  1993/04/09  19:35:45  keffer
 * Indexing is now done using size_t
 *
 * Revision 1.3  1993/03/24  01:29:51  keffer
 * Indexing operations now used unsigned
 *
 * Revision 1.2  1993/02/17  18:32:03  keffer
 * Now passes T's by const reference, rather than by value
 *
 * Revision 1.1  1993/01/28  02:02:26  keffer
 * Initial revision
 *
 *
 ***************************************************************************
 */

template <class T> void
RWTValOrderedVector<T>::clear()
{
  T defaultValue = T();		// avoid "used before set" warning
  register size_t i = 0;

  while (i < nitems_) {
    (*this)(i++) = defaultValue; 
  }
  nitems_ = 0;
  this->reshape(RWDEFAULT_CAPACITY);
}


template <class T> RWBoolean
RWTValOrderedVector<T>::find(const T& key, T& ret) const
{
  size_t idx = index(key);
  return (idx != RW_NPOS) ? (ret = this->array_[idx], TRUE) : FALSE;
}

template <class T> size_t
RWTValOrderedVector<T>::index(const T& val) const
{
  for (register size_t i=0; i<nitems_; i++) {
    if ((*this)(i) == val)
      return i;
  }
  return RW_NPOS;
}

// Insert value at position "ipt"; value formerly at "ipt"
// gets moved to "ipt+1".
template <class T> void
RWTValOrderedVector<T>::insertAt(size_t ipt, const T& val)
{
  if (ipt > 0)
    boundsCheck(ipt-1);

  // Check for overflow:
  if(nitems_>=RWTValVector<T>::length())
    this->reshape(nitems_ + RWDEFAULT_RESIZE);

  // Slide right (could be very expensive)
  // Need to increment nitems_ before indexing to avoid bounds error!
  for(register size_t i=nitems_++; i>ipt; i--)
    (*this)(i) = (*this)(i-1);

  (*this)(ipt) = val;
}

template <class T> size_t
RWTValOrderedVector<T>::occurrencesOf(const T& val) const
{
  size_t count = 0;
  for (register size_t i=0; i<nitems_; i++)
    if ((*this)(i) == val) ++count;
  return count;
}

/*
 * Remove and return the first occurrence of an object which is equal to p
 */
template <class T> RWBoolean
RWTValOrderedVector<T>::remove(const T& p)
{
  size_t idx = index(p);
  if(idx == RW_NPOS)
    return FALSE;

  //else
  removeAt(idx);
  return TRUE;
}


/*
 * Remove and return the last element of self
 */
template <class T> T
RWTValOrderedVector<T>::removeLast()
{
  T ret( (*this)(nitems_-1) );  // construct ret from last item

  // reset now-unused cell to default value:
  (*this)(nitems_-1) = T();

  // now we can decrement nitems_ without causing a bounds error:
  --nitems_;
  return ret;
}


/*
 * Remove all items equal to val
 */
template <class T> size_t
RWTValOrderedVector<T>::removeAll(const T& val)
{
  register size_t i;
  size_t j = 0;

  for (i=0; i<nitems_; i++) {
    // Are they unequal?  If so, this value should be saved.
    if (!((*this)(i) == val))
    {
      // Avoid the copy if possible:
      if (i!=j) (*this)(j) = (*this)(i);
      ++j;
    }
  }

  size_t nremoved = nitems_-j;

  // reset now-unused cells to default value:
  T defaultValue = T();	          // avoid "used before set" warning
  while (j < i)
    (*this)(j++) = defaultValue;

  // now we can decrement nitems_ without a bounds error:
  nitems_ -= nremoved;

  return nremoved;
}

template <class T> T
RWTValOrderedVector<T>::removeAt(size_t ipt)
{
  boundsCheck(ipt);

  T temp = (*this)(ipt);

  // Slide left (could be very expensive):
  for(register size_t i=ipt; i<nitems_-1; i++) {
    (*this)(i) = (*this)(i+1);
  }

  // reset now-unused cell to default value:
  (*this)(nitems_-1) = T();

  // now we can decrement nitems_ without a bounds error:
  --nitems_;

  return temp;
}

template <class T> void
RWTValOrderedVector<T>::boundsCheck(size_t i) const {
  if (i>=nitems_)
    RWTHROW( RWBoundsErr(RWMessage(RWTOOL_INDEXERR, (unsigned)i, (unsigned)nitems_) ));
}


#include "rw/epersist.h"

RWDEFINE_PERSISTABLE_TEMPLATE(RWTValOrderedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
RWDEFINE_PERSIST_TEMPLATE_IO(RWTValOrderedVector,RWpistream,RWpostream)
RWDEFINE_PERSIST_TEMPLATE_IO(RWTValOrderedVector,RWbistream,RWbostream)
RWDEFINE_PERSIST_TEMPLATE_IO(RWTValOrderedVector,RWeistream,RWeostream)
#endif     


template <class T, class S>
void rwSaveGuts(S& str, const RWTValOrderedVector<T>& c)
{
#ifdef RW_FIX_XSIZE_T
  str.putSizeT(c.entries());
#else
  str << c.entries();
#endif
  for (size_t i=0; i < c.entries(); ++i)
    str << c[i];
}

template <class T, class S>
void rwRestoreGuts(S& str, RWTValOrderedVector<T>& c)
{
  size_t count;
#ifdef RW_FIX_XSIZE_T
  str.getSizeT(count);
#else
  str >> count;
#endif
  c.clear();
  for (size_t i=0; i < count; ++i) {
    T t;
    str >> t;
    c.insert(t);
  }
}

