/***************************************************************************
 *
 * tpsrtdli.cc - template definitions for RWTPtrSortedDlistIterator<T,C>
 * 
 * $Id: tpsrtdli.cc@#/main/16  09/04/97 17:25:52  griswolf (TOOLS707_WIN32_971021)
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
 ****************************************************************************/

template <class T, class C>
RWTPtrSortedDlist<T,C>::RWTPtrSortedDlist(T*const* first, T*const* last)
{
  iterator it = impl_.begin();
  while(first != last)
    it = insertSorted(it,*first++);
}

// the following code is optimum if iterators are not random
template <class T, class C> typename RWTPtrSortedDlist<T,C>::size_type
RWTPtrSortedDlist<T,C>::index(
            RWTPtrSortedDlist<T,C>::const_value key
			      ) const
{
// be sure there is a last() to compare.
  if(this->isEmpty())
    return this->npos;
  C comp = C();
// Must stop before end(): last() "<" key: we would not stop
  if(comp(*(last()),*key))
    return this->npos;
  size_type ret = 0;
  const_iterator it = this->begin();
// loop as long as key is "<" than *iterator
  while(comp(**it,*key)) {
    ++it;
    ++ret;
  }
// if *iterator "less" than key, then we have no hit
  return (comp(**it,*key)) ? this->npos : ret;
}

template <class T, class C> RWBoolean
RWTPtrSortedDlistIterator<T,C>::operator++()
{
  RWASSERT(!(head_ == true  &&  tail_ == true)); 

  if (tail_)
    /* do nothing */ ;

  else if (head_) {
    iter_ = list_->RWTPtrSortedDlist<T,C>::base_type::begin();
    head_ = false;
  }
  else
    ++iter_;

  if (!tail_ && iter_ == list_->RWTPtrSortedDlist<T,C>::base_type::end())
    tail_ = true;

  RWASSERT(!(head_ == true  &&  tail_ == true)); 

  return !tail_;
}

template <class T, class C> RWBoolean
RWTPtrSortedDlistIterator<T,C>::operator--()
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
  else if (iter_ == list_->RWTPtrSortedDlist<T,C>::base_type::begin())
    head_ = true;
  else
    --iter_;

  RWASSERT(!(head_ == true  &&  tail_ == true)); 

  return !head_;
}

template <class T, class C> RWBoolean
RWTPtrSortedDlistIterator<T,C>::operator+=(size_t n)
{
  while (n--) ++(*this); return !tail_;
}     

template <class T, class C> RWBoolean
RWTPtrSortedDlistIterator<T,C>::operator-=(size_t n)
{
  while (n--) --(*this); return !head_;
}     

template <class T, class C> T*
RWTPtrSortedDlistIterator<T,C>::findNext(const T* p)
{
  T* a;
  while ( (a = (*this)()) != rwnil)
  {
    if (*a == *p)
      return a;
  }
  return rwnil;
}

template <class T, class C> T*
RWTPtrSortedDlistIterator<T,C>::findNext(RWBoolean (*testFun)(T*, void*), 
                                 void* d)
{
  RWPRECONDITION(testFun!=rwnil);

  T* a;
  while ( (a = (*this)()) != rwnil)
  {
    if (testFun(a, d))
      return a;
  }
  return rwnil;
}

template <class T, class C> T*
RWTPtrSortedDlistIterator<T,C>::remove()
{
  typename RWTPtrSortedDlist<T,C>::iterator i;

  if (head_ || tail_)
    return rwnil;               // iterator is not active

  T* ret = *iter_;
  i = iter_;
  --i;
  list_->impl_.erase(iter_);
  iter_ = i;
  return ret;
}

template <class T, class C> T*
RWTPtrSortedDlistIterator<T,C>::removeNext(const T* p)
{
  while (++(*this)) {
    if (*(*iter_)  == *p) {
      return remove();
    }
  }
  return rwnil;
}

template <class T, class C> T*
RWTPtrSortedDlistIterator<T,C>::removeNext(RWBoolean (*testFun)(T*, void*),
                                           void* d)
{
  RWPRECONDITION(testFun!=rwnil);

  while (++(*this)) {
    if (testFun(*iter_,d)) {
      return remove();
    }
  }
  return rwnil;
}

#include "rw/epersist.h"

RWDEFINE_PERSISTABLE_TEMPLATE_2(RWTPtrSortedDlist)

template <class T, class C, class S>
void rwSaveGuts(S& str, const RWTPtrSortedDlist<T,C>& c)
{
#ifdef RW_FIX_XSIZE_T
  str.putSizeT(c.entries());
#else
  str << c.entries();
#endif
  if(c.entries()) {
// iterate backward so RestoreGuts is linear...
    typename RWTPtrSortedDlist<T,C>::const_iterator it = c.end();
    do {
      --it;
      str << *(*it);
    } while(it != c.begin());
  }
}

template <class T, class C, class S>
void rwRestoreGuts(S& str, RWTPtrSortedDlist<T,C>& c)
{
  typedef typename RWTPtrSortedDlist<T,C>::size_type size_type;
  typename RWTPtrSortedDlist<T,C>::size_type count;
#ifdef RW_FIX_XSIZE_T
  str.getSizeT(count);
#else
  str >> count;
#endif
  c.clear();
  for (size_type i=0; i < count; ++i) {
    T* tp;
    str >> tp;
    c.insert(tp);
  }
}
