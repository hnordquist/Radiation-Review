#ifndef __RWTVDEQUE_H__
#define __RWTVDEQUE_H__

/***************************************************************************
 *
 * tvdeque.h - RWTValDeque<T>
 *   
 *     : value-based double-ended queue wrapper for STL deque 
 * 
 * $Id: tvdeque.h@#/main/23  08/27/97 09:47:53  kevinj (TOOLS707_WIN32_971021)
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
# error RWTValDeque<T> not available if RW_NO_STL is defined
#else // djk: was endif

#ifndef RW_HEADER_WITH_EXTENSION     // djk
# include <deque>
#else
# include <deque.hpp>
#endif  /* RW_HEADER_WITH_EXTENSION */

#include "rw/ev_seq.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T>
#ifndef RW_ALLOCATOR_PLAIN
class RWTExport RWTValDeque : private  RW_VSeq<deque<T,allocator<T> >, RWTValDeque<T> >
#else
class RWTExport RWTValDeque : private  RW_VSeq<deque<T,allocator>, RWTValDeque<T> >
#endif   
{
public:
// typedefs
  typedef RWTValDeque<T>                           this_type;
#ifndef RW_ALLOCATOR_PLAIN
  typedef deque<T,allocator<T> >                   container_type;
#else
  typedef deque<T,allocator>                       container_type;
#endif
  typedef RW_VSeq<container_type,this_type>        base1_type;

  typedef typename container_type::iterator        iterator;
  typedef typename container_type::const_iterator  const_iterator;
  typedef typename container_type::size_type       size_type;
  typedef typename container_type::value_type      value_type;
  typedef typename container_type::reference       reference;
  typedef typename container_type::const_reference const_reference;

#ifndef RW_BROKEN_ACCESS_ADJUST
  typedef base1_type                               base2_type;
  typedef base1_type                               base3_type;
#else
  typedef RW_VCntnr<container_type, this_type>     base2_type;   // djk
  typedef RW_VBase<container_type, this_type>      base3_type;   // djk
#endif

// STL-level access
  container_type&  std()             { return impl_; }
  const container_type&  std() const { return impl_; }

// inherited interface components
  base1_type::append;
  base3_type::apply;
  base3_type::at;
  base3_type::begin;
  base1_type::clear;
  base3_type::contains;
  base3_type::end;
  base3_type::entries;
  base2_type::find;
  base3_type::first;
  base3_type::index;
  base1_type::insert;
  base1_type::insertAt;
  base3_type::isEmpty;
  base2_type::minElement;
  base2_type::maxElement;
  base3_type::npos;
  base3_type::occurrencesOf;
  base3_type::operator();
  base3_type::operator<;
  base3_type::operator==;
  base3_type::operator[];
  base1_type::prepend;
  base1_type::remove;
  base1_type::removeAll;
  base1_type::removeAt;
  base1_type::removeFirst;
  base1_type::replaceAll;
  
// additional interface components
  T&          last()                 { return std().back(); }
  const T&    last() const           { return std().back(); }
  void        pushBack(const T& t)   { std().push_back(t); }
  void        pushFront(const T& t)  { std().push_front(t); }
  T           popBack()              { return removeLast(); }
  T           popFront()             { return this->removeFirst(); }
  void        sort()                 { ::sort(this->begin(),this->end()); }
  value_type  removeLast();
  
// assignment operators
  RWTValDeque<T>&   operator=(const RWTValDeque<T>& rwset)
    { impl_ = rwset.impl_; return *this; }
  RWTValDeque<T>&   operator=(const container_type& stdset)
    { impl_ = stdset; return *this; }
  
// constructors
  RWTValDeque()                                         : impl_() { }
  RWTValDeque(const container_type& lst)                : impl_(lst) { }
  RWTValDeque(const RWTValDeque<T>& lst)                : impl_(lst.impl_) { }
  RWTValDeque(typename container_type::size_type n, const T& val = T())
                                                        : impl_(n,val) { }
  RWTValDeque(const T* first, const T* last)            
#ifdef RW_STLCTOR_PTR_HACK
    : impl_(first,last) {}
#else
   { while(first != last) { impl_.push_back(*first); ++first; } }
#endif
//  ~RWTValDeque(); // handled by compiler-generated destructor

private:
  container_type impl_;
  friend class RW_VBase<container_type,this_type>;
};

RWDECLARE_PERSISTABLE_TEMPLATE(RWTValDeque)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tvdeque.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */

#endif /* __RWTVDEQUE_H__ */
