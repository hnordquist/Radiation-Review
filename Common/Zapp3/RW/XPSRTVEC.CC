/*
 * Template definitions for RWTPtrSortedVector<TP>
 *
 * $Id: xpsrtvec.cc@#/main/9  09/05/96 15:20:49  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: xpsrtvec.cc,v $
 * Revision 7.10  1996/09/05 15:20:49  griswolf
 * Scopus #4418: Fix problem with diff type and size of size_t.
 *
 * Revision 7.9  1996/09/03 19:50:16  hart
 * nitems_ needs this-> for HP aCC compiler
 *
 * Revision 7.8  1996/08/26 21:54:30  hart
 * HP (aCC) compiler port
 *
 * Revision 7.7  1996/05/30 02:49:14  hart
 * fully qualified name references
 *
 * Revision 7.6  1996/03/26 22:30:38  kevinj
 * Removed TURBOC < 0x500 qualifier.
 *
 * Revision 7.5  1996/03/26 01:30:14  pearson
 * #2926 - Fix ambiguous operator << and >> in Borland 5.0 between
 *         RWTPtrSortedVector and RWTValSortedVector
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
 * Revision 7.1  1995/11/17 08:53:49  jims
 * Merge old and new families of templates
 *
 * Revision 7.4  1995/09/17 07:53:04  jims
 * Remove "pointless comparison" from postcondition
 * Check first for less than 2 elements before checking if sorted
 *
 * Revision 7.3  1995/09/05  20:55:29  jims
 * Use new copyright macro
 *
 * Revision 7.2  1995/06/26  22:31:22  griswolf
 * Scopus 1351: Partition RWPOSTCONDITION into two parts for method
 * insert(). Also add isSorted() RWPRECONDITION to bsearch().
 *
 * Revision 7.1  1994/10/16  03:18:52  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.2  1994/07/12  20:04:35  vriezen
 * Updated Copyright.
 *
 * Revision 6.1  1994/04/15  19:48:03  vriezen
 * Move all files to 6.1
 *
 * Revision 1.6  1994/01/12  03:09:17  jims
 * Add constness to T* parameters where appropriate
 *
 * Revision 1.5  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 1.4  1993/07/08  03:42:18  keffer
 * Corrected logic error in RWDEBUG version of bsearch().
 *
 * Revision 1.3  1993/04/09  19:35:45  keffer
 * Indexing is now done using size_t
 *
 * Revision 1.1  1993/01/29  03:06:18  keffer
 * Initial revision
 *
 *
 ***************************************************************************
 */


template <class TP> size_t
RWTPtrSortedVector<TP>::index(const TP* item) const
{
  size_t idx;
  if(! bsearch(item,idx))  // Return RW_NPOS if item is not in collection:
    return RW_NPOS;

  // The item is in the collection.
  // Search downwards looking for the first instance:

  while (idx && *(*this)(idx-1) == *item)
    --idx;

  RWPOSTCONDITION(*(*this)(idx) == *item);
  RWPOSTCONDITION(idx < this->nitems_);

  return idx;
}

template <class TP> void
RWTPtrSortedVector<TP>::insert(TP* p)
{
  // This algorithm uses only equality and less-than operators

#ifdef RWDEBUG
  size_t count = occurrencesOf(p);
#endif

  size_t idx;
  if (bsearch(p,idx))
  {

    // A matching item was found.  Insert after the
    // last equal item.
    while (idx < this->nitems_ && *(*this)(idx) == *p)
      ++idx;
  }
  else
  {
    // No matching item found.  Search upward
    // for the first item greater than the value
    // and insert before it.
    while (idx < this->nitems_ && *(*this)(idx) < *p)
      ++idx;
  }

  insertAt(idx, p);

  RWPOSTCONDITION(isSorted());
  RWPOSTCONDITION(occurrencesOf(p) == count+1);
}

template <class TP> size_t
RWTPtrSortedVector<TP>::occurrencesOf(const TP* p) const
{
  size_t iend;
  size_t istart = indexSpan(p, iend);
  return istart == RW_NPOS ? 0 : iend-istart+1;
}

/*
 * Remove and return the first occurrence of an object with the
 * same value as the object pointed to by "p".
 */
template <class TP> TP*
RWTPtrSortedVector<TP>::remove(const TP* p)
{
  size_t idx = index(p);
  return idx == RW_NPOS ? rwnil : removeAt(idx);
}

template <class TP> size_t
RWTPtrSortedVector<TP>::removeAll(const TP* p)
{
  size_t iend;
  size_t istart = indexSpan(p, iend);

  if (istart == RW_NPOS) return 0;
  iend++;
  size_t nremoved = iend-istart;

  // Do a "solid body" slide left of the remaining items in the collection:
  while (iend < this->nitems_)
    (*this)(istart++) = (*this)(iend++);

  this->nitems_ -= nremoved;
  RWPOSTCONDITION(!contains(p));
  return nremoved;
}

/****************************************************************
 *								*
 *	protected members of RWTPtrSortedVector<TP>		*
 *								*
 ****************************************************************/

template <class TP> RWBoolean
RWTPtrSortedVector<TP>::bsearch(const TP* key, size_t& idx) const
{
  RWPRECONDITION(isSorted());
  idx = 0;
  if (this->entries())
  {
    size_t top = this->entries() - 1;
    size_t bottom = 0;

    while (top>bottom)
    {
      idx = (top+bottom) >> 1;
      // It is important that we use only the equality and less-than
      // operators:
      if (*key == *(*this)(idx))
        return TRUE;
      else if(*key < *(*this)(idx))
        top    = idx ? idx - 1 : 0u;
      else
        bottom = idx + 1;
    }
    return *key == *(*this)(idx=bottom);
  }
  // Not found:
  return FALSE;
}

template <class TP> size_t
RWTPtrSortedVector<TP>::indexSpan(const TP* key, size_t& end) const
{
  // Do a binary search to find the first match:
  size_t istart = index(key);

  if (istart != RW_NPOS)
  {
    // Found one.  Do a linear search, looking for the last match:
    end = istart;
    while ( end+1 < this->entries() && *((*this)(end+1)) == *key)
     ++end;
  }

  return istart;
}

#ifdef RWDEBUG
template <class TP> RWBoolean
RWTPtrSortedVector<TP>::isSorted() const
{
  // dispense with trivial cases:
  if (this->nitems_ < 2)
    return TRUE;

  // otherwise:
  for(size_t idx=0; idx<this->nitems_-1; idx++)
  {
    /*
     * Not sorted if the item at this index is not less
     * than and not equal to the item at the next index
     * (i.e., it is greater than).
     */
    if(   !(*(*this)(idx) <  *(*this)(idx+1))
       && !(*(*this)(idx) == *(*this)(idx+1))
      )
      return FALSE;
  }
  return TRUE;
}
#endif


#if !defined(__TURBOC__)

#include "rw/epersist.h"

RWDEFINE_PERSISTABLE_TEMPLATE(RWTPtrSortedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
RWDEFINE_PERSIST_TEMPLATE_IO(RWTPtrSortedVector,RWpistream,RWpostream)
RWDEFINE_PERSIST_TEMPLATE_IO(RWTPtrSortedVector,RWbistream,RWbostream)
RWDEFINE_PERSIST_TEMPLATE_IO(RWTPtrSortedVector,RWeistream,RWeostream)
#endif


template <class T, class S>
void rwSaveGuts(S& str, const RWTPtrSortedVector<T>& c)
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
void rwRestoreGuts(S& str, RWTPtrSortedVector<T>& c)
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

#endif /* !defined(__TURBOC__)  */

