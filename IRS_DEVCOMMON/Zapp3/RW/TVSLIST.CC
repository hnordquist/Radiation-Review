/*
 * Template definitions for and RWTValSlistIterator<T>
 *
 * $Id: tvslist.cc@#/main/20  09/05/96 15:20:49  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tvslist.cc,v $
 * Revision 7.11  1996/09/05 15:20:49  griswolf
 * Scopus #4418: Fix problem with diff type and size of size_t.
 *
 * Revision 7.10  1996/08/09 19:44:05  hart
 * HP (aCC compiler) port
 *
 * Revision 7.9  1996/02/21 23:14:06  griswolf
 * Fix an N^2 order saveGuts technique to make it linear.
 *
 * Revision 7.8  1996/02/18 01:50:17  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.7  1996/02/12 21:03:06  kevinj
 * Switched stream and class args in rw*Guts.
 *
 * Revision 7.6  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.5  1996/01/13 23:26:30  kevinj
 * Simple External Template Persistence (ETP).
 *
 * Revision 7.4  1995/12/20 22:27:55  pearson
 * Fix typo
 *
 * Revision 7.3  1995/11/13 12:47:12  jims
 * New stl-based collections now one with Tools-6 templates
 *
 * Revision 7.2  1995/09/05 21:31:35  jims
 * Use new copyright macro
 *
 * Revision 7.1  1994/10/16  03:31:10  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.4  1994/07/26  19:11:28  jims
 * Fix copy ctor to use passed in argument instead of *this
 *
 * Revision 6.3  1994/07/12  20:05:34  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/07/05  21:56:52  nevis
 * Fixes bug #117 by defining explicitly a copy constructor.
 *
 * Revision 6.2  1994/07/05  20:57:31  nevis
 * Fixes bug #117 by defining copy constructor for SGI compiler..
 *
 * Revision 6.1  1994/04/15  19:48:33  vriezen
 * Move all files to 6.1
 *
 * Revision 1.6  1993/11/08  21:14:01  jims
 * Port to ObjectStore
 *
 * Revision 1.5  1993/09/10  02:18:28  keffer
 * apply() now takes a function with a non-const reference, allowing
 * modification of the contained value.
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
 * Revision 1.1  1993/01/27  21:38:32  keffer
 * Initial revision
 *
 ***************************************************************************
 */


/****************************************************************
 *                                                              *
 *      Definitions for RWTValSlistIterator<T>                  *
 *                                                              *
 ****************************************************************/

template <class T> RWBoolean
RWTValSlistIterator<T>::operator++()
{
  RWASSERT(!(head_ == true  &&  tail_ == true)); 

  if (tail_)
    /* do nothing */ ;

  else if (head_) {
    iter_ = list_->begin();
    head_ = false;
  }
  else
    ++iter_;

  if (!tail_ && iter_ == list_->end())
    tail_ = true;

  RWASSERT(!(head_ == true  &&  tail_ == true)); 

  return !tail_;
}

template <class T> RWBoolean
RWTValSlistIterator<T>::operator+=(size_t n)
{
  while (n--) ++(*this); return !tail_;
}     

template <class T> RWBoolean
RWTValSlistIterator<T>::findNext(const T& val)
{
  while (++(*this))
  {
    if (key()==val)
      return TRUE;
  }
  return FALSE;
}

template <class T> RWBoolean
RWTValSlistIterator<T>::findNext(RWBoolean (*testFun)(const T&, void*),
                                 void* d)
{
  RWPRECONDITION(testFun!=rwnil);

  while (++(*this))
  {
    if (testFun(key(),d))
      return TRUE;
  }
  return FALSE;
}

template <class T> void
RWTValSlistIterator<T>::insertAfterPoint(const T& a)
{
  RWPRECONDITION2(!tail_, "RWTValSlistIterator::insertAfterPoint(const T& a): Attempt to insertAfterPoint while iterator is past the end");

  typename RWTValSlist<T>::iterator i;

  if (head_)
    list_->prepend(a);
  else {
    i = iter_;
    ++i;
    list_->std().insert(i,a);
  }
}

template <class T> RWBoolean
RWTValSlistIterator<T>::remove()
{
  typename RWTValSlist<T>::iterator i, j;

  if (head_ || tail_)
    return FALSE;               // iterator is not active

  if (iter_ == list_->begin()) {
    list_->removeFirst();
    head_ = true;
  }
  else {
    // As the manual warns, for slists this is "relatively inefficient:"
    i = list_->begin();  
    do {
      j = i;
      ++i;
    } while (i != iter_);

    list_->std().erase(iter_);
    iter_ = j;
  }
  return TRUE;
}

template <class T> RWBoolean
RWTValSlistIterator<T>::removeNext(const T& val)
{
  while (++(*this)) {
    if (*iter_  == val) {
      return remove();
    }
  }
  return FALSE;
}

template <class T> RWBoolean
RWTValSlistIterator<T>::removeNext(RWBoolean (*testFun)(const T&, void*),
                                   void* d)
{
  RWPRECONDITION(testFun!=rwnil);

  while (++(*this)) {
    if (testFun(*iter_,d)) {
      return remove();
    }
  }
  return FALSE;
}


#include "rw/epersist.h"

RWDEFINE_PERSISTABLE_TEMPLATE(RWTValSlist)

template <class T, class S>
void rwSaveGuts(S& str, const RWTValSlist<T>& c)
{
#ifdef RW_FIX_XSIZE_T
  str.putSizeT(c.entries());
#else
  str << c.entries();
#endif
  typename RWTValSlist<T>::const_iterator it = c.begin();
  while(it != c.end()) {
    str << *it;
    ++it;
  }
}

template <class T, class S>
void rwRestoreGuts(S& str, RWTValSlist<T>& c)
{
  typedef typename RWTValSlist<T>::size_type size_type;
  typename RWTValSlist<T>::size_type count;
#ifdef RW_FIX_XSIZE_T
  str.getSizeT(count);
#else
  str >> count;
#endif
  c.clear();
  for (size_type i=0; i < count; ++i) {
    T t;
    str >> t;
    c.insert(t);
  }
}
