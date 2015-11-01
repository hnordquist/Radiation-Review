/*
 * Template definitions for
 * implementation of RWTValSortedDlistIterator<T,C>
 *
 * $Id: tvsrtdli.cc@#/main/17  09/04/97 17:26:03  griswolf (TOOLS707_WIN32_971021)
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
 ***************************************************************************/

// the following code is optimum if iterators are not random
template <class T, class C>
typename RWTValSortedDlist<T,C>::size_type
RWTValSortedDlist<T,C>::index(
            typename RWTValSortedDlist<T,C>::const_reference key
			      ) const
{
// be sure there is a last() to compare.
  if(this->isEmpty())
    return this->npos;
  C comp = C();
// Must stop before end(): last() "<" key: we would not stop
  if(comp(last(),key))
    return this->npos;
  size_type ret = 0;
  const_iterator it = this->begin();
// loop as long as key is "<" than *iterator
  while(comp(*it,key)) {
    ++it;
    ++ret;
  }
// if *iterator "less" than key, then we have no hit
  return (comp(*it,key)) ? this->npos : ret;
}

template <class T, class C>
typename RWTValSortedDlist<T,C>::size_type
RWTValSortedDlist<T,C>::merge(const RWTValSortedDlist<T,C>& coll)
{
  size_type ret = coll.entries();

  if (ret) {
    typename container_type::iterator first1 = impl_.begin();
    typename container_type::iterator last1  = impl_.end();
    const_iterator    first2 = coll.begin();
    const_iterator    last2 = coll.end();

    while (first1 != last1 && first2 != last2) {
      while (first2 != last2 && C()(*first2,*first1)) {
        impl_.insert(first1,*first2);
        ++first2;
      }
      ++first1;
    }

    while (first2 != last2) {
      impl_.insert(this->end(), *first2);
      ++first2;
    }
  }
  return ret;
}


template <class T, class C> RWBoolean
RWTValSortedDlistIterator<T,C>::operator++()
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

template <class T, class C> RWBoolean
RWTValSortedDlistIterator<T,C>::operator--()
{
  RWASSERT(!(head_ == true  &&  tail_ == true)); 

  if (head_)
    /* do nothing */ ;

  else if (tail_) {
    if (list_->entries() == 0) {
      tail_ = false;
      head_ = true;
    }
    else {
      tail_ = false;
      --iter_;
    }
  }
  else if (iter_ == list_->begin())
    head_ = true;
  else
    --iter_;

  RWASSERT(!(head_ == true  &&  tail_ == true)); 

  return !head_;
}

template <class T, class C> RWBoolean
RWTValSortedDlistIterator<T,C>::operator+=(size_t n)
{
  while (n--) ++(*this); return !tail_;
}     

template <class T, class C> RWBoolean
RWTValSortedDlistIterator<T,C>::operator-=(size_t n)
{
  while (n--) --(*this); return !head_;
}     

template <class T, class C> RWBoolean
RWTValSortedDlistIterator<T,C>::findNext(const T& val)
{
  while (++(*this))
  {
    if (key()==val)
      return TRUE;
  }
  return FALSE;
}

template <class T, class C> RWBoolean
RWTValSortedDlistIterator<T,C>::findNext(RWBoolean (*testFun)(const T&, void*), 
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

template <class T, class C> RWBoolean
RWTValSortedDlistIterator<T,C>::remove()
{
#ifndef RW_ALLOCATOR_PLAIN
  typename list<T,allocator<T> >::iterator i;
#else
  typename list<T,allocator>::iterator i;
#endif
  if (head_ || tail_)
    return FALSE;               // iterator is not active

  i = iter_;
  --i;
  list_->std().erase(iter_);
  iter_ = i;
  return TRUE;
}

template <class T, class C> RWBoolean
RWTValSortedDlistIterator<T,C>::removeNext(const T& val)
{
  while (++(*this)) {
    if (*iter_  == val) {
      return remove();
    }
  }
  return FALSE;
}

template <class T, class C> RWBoolean
RWTValSortedDlistIterator<T,C>
  ::removeNext(RWBoolean (*testFun)(const T&, void*),
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

RWDEFINE_PERSISTABLE_TEMPLATE_2(RWTValSortedDlist)

template <class T, class C, class S>
void rwSaveGuts(S& str, const RWTValSortedDlist<T,C>& c)
{
#ifdef RW_FIX_XSIZE_T
  str.putSizeT(c.entries());
#else
  str << c.entries();
#endif
  if(c.entries()) {
// iterate backward so RestoreGuts is linear...
    typename RWTValSortedDlist<T,C>::const_iterator it = c.end();
    do {
      --it;
      str << *it;
    } while(it != c.begin());
  }
}

template <class T, class C, class S>
void rwRestoreGuts(S& str, RWTValSortedDlist<T,C>& c)
{
  typedef typename RWTValSortedDlist<T,C>::size_type size_type;
  typename RWTValSortedDlist<T,C>::size_type count;
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
