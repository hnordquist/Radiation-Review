/***************************************************************************
 *
 * tpslist.cc - template definitions for RWTPtrSlist<T>
 * 
 * $Id: tpslist.cc@#/main/24  09/04/97 17:25:49  griswolf (TOOLS707_WIN32_971021)
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

/*
 * Having trouble with these overridden clearAndDestroy and
 * removeAll functions.
 * For now we'll use the version inherited from the base class.
 */

#if 0
template <class T> void
RWTPtrSlist<T>::clearAndDestroy() {
  for(iterator i = this->begin(); i != this->end(); /**/) {
    iterator nx = i;
    ++nx;
    delete *i;
    impl_.erase(i);
    i = nx;
  }
}
#endif

#if 0
template <class T> rw_slist<T*>::size_type
RWTPtrSlist<T>::removeAll(bool (*fn)(RWTPtrSlist<T>::const_value key,void*),void* x)
{
  rw_slist<T*>::size_type ret = 0;
  for(iterator it = this->begin(); it != this->end();/**/) {
    if((*fn)(*it,x)) {
      impl_.erase(it);
      ++ret;
    }
    ++it;
  }
  return ret;
}

template <class T> rw_slist<T*>::size_type
RWTPtrSlist<T>::removeAll(const_value key)
{
  rw_slist<T*>::size_type ret = 0;
  for(iterator it = this->begin(); it != this->end();/**/) {
    if(**it == *key) {
      impl_.erase(it);
      ++ret;
    }
    ++it;
  }
  return ret;
}
#endif

/*
 * This is a "for now" sorting algorithm.  Ultimately the underlying
 * implementation will have a template sort member function that
 * takes a comparitor, needed for dereferencing the elements
 * in the rw_slist<T*> impl_.
 *
 * So, for now, here is an insertion sort just like the one in "Algorithms"
 * by Robert Sedgwick, 2nd Edition, p.100:
 */

template <class T>
void RWTPtrSlist<T>::sort()
{
  int i, j;
  T* v;
  for (i=1; i<this->entries(); i++) {
    v = this->at(i);
    j = i;
    while (j > 0 && *(this->at(j-1)) > *v) {
      this->at(j) = this->at(j-1);
      --j;
    }
    this->at(j) = v;
  }
}
        

//===========================================

template <class T> RWBoolean
RWTPtrSlistIterator<T>::operator++()
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
RWTPtrSlistIterator<T>::operator+=(size_t n)
{
  while (n--) ++(*this); return !tail_;
}     

template <class T> T*
RWTPtrSlistIterator<T>::findNext(const T* p)
{
  T* a;
  while ( (a = (*this)()) != rwnil)
  {
    if (*a == *p)
      return a;
  }
  return rwnil;
}

template <class T> T*
RWTPtrSlistIterator<T>::findNext(RWBoolean (*testFun)(T*, void*), 
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

template <class T> void
RWTPtrSlistIterator<T>::insertAfterPoint(T* a)
{
  RWPRECONDITION2(!tail_, "RWTPtrSlistIterator::insertAfterPoint(T* a): Attempt to insertAfterPoint while iterator is past the end");

  typename RWTPtrSlist<T>::iterator i;

  if (head_)
    list_->prepend(a);
  else {
    i = iter_;
    ++i;
    list_->impl_.insert(i,a);
  }
}

template <class T> T*
RWTPtrSlistIterator<T>::remove()
{
  typename RWTPtrSlist<T>::iterator i, j;
  T* ret;

  if (head_ || tail_)
    return rwnil;               // iterator is not active

  if (iter_ == list_->begin()) {
    ret = list_->removeFirst();
    head_ = true;
  }
  else {
    // As the manual warns, for slists this is "relatively inefficient:"
    i = list_->begin();  
    do {
      j = i;
      ++i;
    } while (i != iter_);

    ret = *iter_;
    list_->impl_.erase(iter_);
    iter_ = j;
  }
  return ret;
}

template <class T> T*
RWTPtrSlistIterator<T>::removeNext(const T* p)
{
  while (++(*this)) {
    if (*(*iter_)  == *p) {
      return remove();
    }
  }
  return rwnil;
}

template <class T> T*
RWTPtrSlistIterator<T>::removeNext(RWBoolean (*testFun)(T*, void*),
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

RWDEFINE_PERSISTABLE_TEMPLATE(RWTPtrSlist)

template <class T, class S>
void rwSaveGuts(S& str, const RWTPtrSlist<T>& c)
{
#ifdef RW_FIX_XSIZE_T
  str.putSizeT(c.entries());
#else
  str << c.entries();
#endif
  typename RWTPtrSlist<T>::const_iterator it = c.begin();
  while(it != c.end()) {
    str << *(*it);
    ++it;
  }
}

template <class T, class S>
void rwRestoreGuts(S& str, RWTPtrSlist<T>& c)
{
  typedef typename RWTPtrSlist<T>::size_type size_type;
  size_type count;
#ifdef RW_FIX_XSIZE_T
  str.getSizeT(count);
#else
  str >> count;
#endif
  c.clear();
  for (size_type i=0; i < count; ++i) {
    T* p;
    str >> p;
    c.insert(p);
  }
}
