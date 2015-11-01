#ifndef __RWTPDLIST_H__
#define __RWTPDLIST_H__

/***************************************************************************
 *
 * tpdlist.h - RWTPtrDlist<T>
 *   
 *     : pointer-based doubly-linked list wrapper for STL list
 * 
 * $Id: tpdlist.h@#/main/48  09/12/97 13:43:36  kevinj (TOOLS707_WIN32_971021)
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
#  include "rw/xpdlist.h"
#else

// STL-based implementation:

#ifndef RW_HEADER_WITH_EXTENSION
# include <list>
#else
# include <list.hpp>
#endif  /* RW_HEADER_WITH_EXTENSION */

#include "rw/ep_seq.h"

template <class T> class RWTExport RWTPtrDlistIterator;

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T>
class RWTExport RWTPtrDlist :
#ifndef RW_ALLOCATOR_PLAIN
				private RW_PSeq<list<T*,allocator<T*> >, RWTPtrDlist<T>, T>
#else
				private RW_PSeq<list<T*,allocator>, RWTPtrDlist<T>, T>
#endif   
{
public:
// typedefs
#ifndef RW_ALLOCATOR_PLAIN
  typedef list<T*,allocator<T*> >                         container_type;
#else
  typedef list<T*,allocator>                              container_type;
#endif
  typedef RWTPtrDlist<T>                                  this_type;
  typedef RW_PSeq<container_type, RWTPtrDlist<T>, T>      base1_type;
  typedef typename container_type::size_type              size_type;
  typedef typename container_type::difference_type        difference_type;

  typedef typename base1_type::my_iter_                   iterator;
  typedef typename base1_type::const_my_iter_             const_iterator;
  typedef typename base1_type::value_type                 value_type;
  typedef typename base1_type::const_value                const_value;
  typedef typename base1_type::reference                  reference;
  typedef typename base1_type::const_reference            const_reference;

#  ifndef RW_BROKEN_ACCESS_ADJUST   // djk
     typedef base1_type                                   base2_type;
     typedef base1_type                                   base3_type;
#  else
     typedef RW_PCntnr<container_type,this_type,T>        base2_type;
     typedef RW_PBase<container_type,this_type>           base3_type;
#  endif  /* RW_BROKEN_ACCESS_ADJUST */
  
// STL-level access
  const container_type&  std() const { return impl_; }
  container_type&  std() { return impl_; }

// inherited interface components
  base3_type::at;
  base3_type::begin;
  base3_type::end;
  base3_type::entries;
  base3_type::first;
  base3_type::isEmpty;
  base3_type::npos;
  base3_type::operator();
  base3_type::operator[];
  base2_type::operator<;
  base2_type::operator==;
  base2_type::apply;
  base2_type::contains;
  base2_type::find;
  base2_type::index;
  base2_type::occurrencesOf;
  base1_type::append;
  base1_type::clear;
  base1_type::get;
  base1_type::insert;
  base1_type::insertAt;
  base1_type::minElement;
  base1_type::maxElement;
  base1_type::prepend;
  base1_type::remove;
  base1_type::removeAt;
  base1_type::removeFirst;
  base1_type::replaceAll;
#if !defined(__SUNPRO_CC) || (__SUNPRO_CC<0x420 && !defined(RW_NO_STL))
  typename container_type::size_type  removeAll(const T* key);
  typename container_type::size_type  removeAll(bool (*fn)(const T*, void*),void* x);
  typename container_type::size_type  removeAll(bool (*fn)(T*, void*),void* x);
#else
  size_type  removeAll(const T* key);
  size_type  removeAll(bool (*fn)(const T*, void*),void* x);
  size_type  removeAll(bool (*fn)(T*, void*),void* x);
#endif /* !SunPro */
  void                       clearAndDestroy();

// additional interface components
  T*&       last()       { return impl_.back(); }
  T*const&  last() const { return impl_.back(); }

  T*    removeLast()
    { T* ret = *(--this->end());
      impl_.pop_back();
      return ret; }

  void  sort();

// assignment operators
  RWTPtrDlist<T>&   operator=(const RWTPtrDlist<T>& rwset)
    { impl_ = rwset.impl_; return *this; }
  RWTPtrDlist<T>&   operator=(const container_type& stdset)
    { impl_ = stdset; return *this; }

// constructors
  RWTPtrDlist()                                : impl_() {}
  RWTPtrDlist(const container_type& lst)       : impl_(lst) {}
  RWTPtrDlist(const this_type& lst)            : impl_(lst.impl_) {}
  RWTPtrDlist(size_type n, T* p = 0)           : impl_(n, p) {}
  RWTPtrDlist(T*const* first, T*const* last)
#ifdef RW_STLCTOR_PTR_HACK
   : impl_(first, last) { }
#else
   : impl_()
   { while(first != last) { impl_.push_back(*first); ++first; } }
#endif

//  ~RWTPtrDlist(); // handled by compiler-generated destructor

protected:
  void sortPairs();
  void pointerMerge(iterator, iterator, iterator);

private:
  container_type impl_;         
  friend class RW_PBase<container_type, this_type>;
  friend class RWTPtrDlistIterator<T>;
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

RWDECLARE_PERSISTABLE_TEMPLATE(RWTPtrDlist)

/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrDlistIterator<T>                 *
 *                                                              *
 ****************************************************************/

template <class T> class RWTExport RWTPtrDlistIterator
{
private:
  RWTPtrDlist<T>*           list_;
  typename RWTPtrDlist<T>::iterator  iter_;
  bool                      head_;  // simulate head sentinel
  bool                      tail_;  // simulate tail sentinel
/*
 * Invariants:
 *   head_ and tail_ can never both be true
 *   if head_ and tail_ are both false then iter_ is valid
 *     (unless invalidated from outside this class)
 */
public:
  RWTPtrDlistIterator(RWTPtrDlist<T>& s)
    : list_(&s), head_(true), tail_(false) { }

  // Operators:
  RWBoolean     operator++();
  RWBoolean     operator--();
  RWBoolean     operator+=(size_t n);
  RWBoolean     operator-=(size_t n);
  T*            operator()()   { return ++(*this) ? key() : rwnil; }         

  // Methods:
  RWTPtrDlist<T>* container() const  { return list_; }

  T*            findNext(const T* p);
  T*            findNext(RWBoolean (*testFun)(T*, void*), void* d);

  void          insertAfterPoint(T* p);

  T*            key() const { return *((RWTPtrDlistIterator<T>*)this)->iter_; }

  T*            remove();

  T*            removeNext(const T* p);
  T*            removeNext(RWBoolean (*testFun)(T*, void*), void* d);

  void          reset()       { head_ = true; tail_ = false; } 
  void          reset(RWTPtrDlist<T>& s)
    { head_ = true; tail_ = false; list_ = &s; } 

private:

  // Disallow postfix operators.  Unless we hide it, some compilers will
  // substitute the prefix operator in its place.
  RWBoolean             operator++(int);
  RWBoolean             operator--(int);
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/tpdlist.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */

#endif /* __RWTPDLIST_H__ */
