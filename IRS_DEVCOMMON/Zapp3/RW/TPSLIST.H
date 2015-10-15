#ifndef __RWTPSLIST_H__
#define __RWTPSLIST_H__

/***************************************************************************
 *
 * tpslist.h - RWTPtrSlist<T>
 *   
 *     : pointer-based singly-linked list wrapper for STL-compliant rw_slist
 * 
 * $Id: tpslist.h@#/main/41  09/04/97 17:25:50  griswolf (TOOLS707_WIN32_971021)
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

#include "rw/compiler.h"

#ifdef RW_NO_STL
#  include "rw/xpslist.h"
#else

// STL-based implementation:

#include <rw/stdex/slist.h>
#include "rw/ep_seq.h"

template <class T> class RWTExport RWTPtrSlistIterator;

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T>
class RWTExport RWTPtrSlist
#ifndef RW_ALLOCATOR_PLAIN
  : private RW_PSeq<rw_slist<T*, allocator<T*> >, RWTPtrSlist<T>, T>
#else
  : private RW_PSeq<rw_slist<T*>, RWTPtrSlist<T>, T>
#endif
{
public:
// typedefs
  typedef RWTPtrSlist<T>                                this_type;
#ifndef RW_ALLOCATOR_PLAIN
  typedef rw_slist<T*,allocator<T*> >                   container_type;
  typedef RW_PSeq<rw_slist<T*, allocator<T*> >,RWTPtrSlist<T>, T> base_type;
#else
  typedef rw_slist<T*>                                  container_type;
  typedef RW_PSeq<rw_slist<T*>,RWTPtrSlist<T>, T>       base_type;
#endif
  typedef typename container_type::size_type            size_type;
  typedef typename container_type::difference_type      difference_type;

  typedef typename base_type::my_iter_                  iterator;
  typedef typename base_type::const_my_iter_            const_iterator;
  typedef typename base_type::value_type                value_type;
  typedef typename base_type::const_value               const_value;
  typedef typename base_type::reference                 reference;
  typedef typename base_type::const_reference           const_reference;

// STL-level access
  const container_type&  std() const { return impl_; }
  container_type&  std() { return impl_; }

// inherited interface components
  base_type::append;
  base_type::apply;
  base_type::at;
  base_type::begin;
  base_type::clear;
  base_type::clearAndDestroy;
  base_type::contains;
  base_type::end;
  base_type::entries;
  base_type::find;
  base_type::first;
  base_type::get;
  base_type::index;
  base_type::insert;
  base_type::insertAt;
  base_type::isEmpty;
  base_type::minElement;
  base_type::maxElement;
  base_type::npos;
  base_type::occurrencesOf;
  base_type::operator();
  base_type::operator<;
  base_type::operator==;
  base_type::operator[];
  base_type::prepend;
  base_type::remove;
  base_type::removeAt;
  base_type::removeAll;
  base_type::removeFirst;
  base_type::replaceAll;

// additional interface components
  T*&       last()       { return impl_.back(); }
  T*const&  last() const { return impl_.back(); }
  T*        removeLast()
    { T* ret = last(); removeAt(this->entries()-1); return ret; }

  void  sort();

// assignment operators
  RWTPtrSlist<T>&   operator=(const RWTPtrSlist<T>& rwlst)
    { impl_ = rwlst.impl_; return *this; }
#ifndef RW_ALLOCATOR_PLAIN
  RWTPtrSlist<T>&   operator=(const rw_slist<T*, allocator<T*> >& stdlst)
    { impl_ = stdlst; return *this; }
#else
  RWTPtrSlist<T>&   operator=(const rw_slist<T*>& stdlst)
    { impl_ = stdlst; return *this; }
#endif

// constructors
  RWTPtrSlist()                                : impl_() {}
  RWTPtrSlist(const container_type& lst)       : impl_(lst) {}
  RWTPtrSlist(const this_type& lst)            : impl_(lst.impl_) {}
  RWTPtrSlist(size_type n, T* p = 0)           : impl_(n, p) {}
  RWTPtrSlist(T*const* first, T*const* last)   : impl_(first, last) {}

// ~RWTPtrSlist(); // handled by compiler-generated destructor

protected:
//  void sortPairs();
//  void pointerMerge(iterator, iterator, iterator);

private:
  container_type impl_;         
  friend class RW_PBase<container_type, this_type>;
  friend class RWTPtrSlistIterator<T>;
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

RWDECLARE_PERSISTABLE_TEMPLATE(RWTPtrSlist)

/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrSlistIterator<T>         *
 *                                                              *
 ****************************************************************/

template <class T> class RWTExport RWTPtrSlistIterator
{
private:
  RWTPtrSlist<T>*                    list_;
  typename RWTPtrSlist<T>::iterator  iter_;
  bool                               head_;  // simulate head sentinel
  bool                               tail_;  // simulate tail sentinel
/*
 * Invariants:
 *   head_ and tail_ can never both be true
 *   if head_ and tail_ are both false then iter_ is valid
 *     (unless invalidated from outside this class)
 */

public:
  RWTPtrSlistIterator(RWTPtrSlist<T>& s)
    : list_(&s), head_(true), tail_(false) { }

  // Operators:
  RWBoolean     operator++();
  RWBoolean     operator+=(size_t n);
  T*            operator()()   { return ++(*this) ? key() : rwnil; }         

  // Methods
  RWTPtrSlist<T>* container() const { return list_; }

  T*            findNext(const T* p);
  T*            findNext(RWBoolean (*testFun)(T*, void*), void* d);

  void          insertAfterPoint(T* a);

  T*            key() const
    { return *((RWTPtrSlistIterator<T>*)this)->iter_; }

  T*            remove();

  T*            removeNext(const T*);
  T*            removeNext(RWBoolean (*testFun)(T*, void*), void*);

  void          reset()       { head_ = true; tail_ = false; } 
  void          reset(RWTPtrSlist<T>& s)
    { head_ = true; tail_ = false; list_ = &s; } 

private:

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
  
  RW_T_TYPESPEC  /* This macro expands to nothing */
};


#ifdef RW_COMPILE_INSTANTIATE
#include "rw/tpslist.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */

#endif /* __RWTPDLIST_H__ */
