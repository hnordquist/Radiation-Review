#ifndef __RWTVSLIST_H__
#define __RWTVSLIST_H__

/***************************************************************************
 *
 * tvslist.h - RWTValSlist<T>
 *   
 *     : value-based singly-linked list wrapper for STL list
 * 
 * $Id: tvslist.h@#/main/37  09/04/97 17:26:01  griswolf (TOOLS707_WIN32_971021)
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
#  include "rw/xvslist.h"
#else
// STL-based implementation:

#include "rw/stdex/slist.h"       // Rogue Wave STL-type singly linked list
#include "rw/ev_seq.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T>
#ifndef RW_ALLOCATOR_PLAIN
class RWTExport RWTValSlist :
    private RW_VSeq<rw_slist<T,allocator<T> >,RWTValSlist<T> >
#else
#ifndef RW_INHERIT_ACCESS_PROBLEM
class RWTExport RWTValSlist : private RW_VSeq<rw_slist<T>,RWTValSlist<T> >
#else
class RWTExport RWTValSlist : public RW_VSeq<rw_slist<T>,RWTValSlist<T> >
#endif /* RW_INHERIT_ACCESS_PROBLEM */
#endif /* RW_ALLOCATOR_PLAIN */
{
public:
// typedefs
#ifndef RW_ALLOCATOR_PLAIN
  typedef rw_slist<T,allocator<T> >                    container_type;
#else
  typedef rw_slist<T>                                  container_type;
#endif
  typedef RWTValSlist<T>                               this_type;
  typedef RW_VSeq<container_type,this_type>            base1_type;
  
#ifndef RW_BROKEN_ACCESS_ADJUST
  typedef base1_type                                   base2_type;
  typedef base1_type                                   base3_type;
#else
  typedef RW_VCntnr<container_type,this_type>          base2_type;
  typedef RW_VBase<container_type,this_type>           base3_type;
#endif  /* RW_BROKEN_ACCESS_ADJUST */
  
  typedef typename container_type::iterator            iterator;
  typedef typename container_type::const_iterator      const_iterator;
  typedef typename container_type::size_type           size_type;
  typedef typename container_type::value_type          value_type;
  typedef typename container_type::reference           reference;
  typedef typename container_type::const_reference     const_reference;

// STL-level access
  container_type&  std()             { return impl_; }
  const container_type&  std() const { return impl_; }

// inherited interface components
  base1_type::append;
  base3_type::apply;
  base3_type::at;
  base1_type::begin;
  base1_type::clear;
  base3_type::contains;
  base1_type::end;
  base3_type::entries;
  base2_type::find;
  base1_type::first;
  base1_type::get;
  base3_type::index;
  base1_type::insert;
  base1_type::insertAt;
  base3_type::isEmpty;
  base1_type::minElement;
  base1_type::maxElement;
  base1_type::npos;
  base3_type::occurrencesOf;
  base1_type::operator();
  base1_type::operator<;
  base1_type::operator==;
  base3_type::operator[];
  base1_type::prepend;
  base1_type::remove;
  base1_type::removeAll;
  base1_type::removeAt;
  base1_type::removeFirst;
  base1_type::replaceAll;
  
// overridden interface components
  void sort() { this->std().sort(); }
  
// additional interface components
  T     last()       { return this->std().back(); }
  T     removeLast() {T ret=last(); removeAt(this->entries()-1); return ret; }
  
// assignment operators
  RWTValSlist<T>&   operator=(const RWTValSlist<T>& rwset)
    { impl_ = rwset.impl_; return *this; }
#ifndef RW_ALLOCATOR_PLAIN
  RWTValSlist<T>&   operator=(const rw_slist<T, allocator<T> >& stdset)
#else
  RWTValSlist<T>&   operator=(const rw_slist<T>& stdset)
#endif
    { impl_ = stdset; return *this; }
  
// constructors
  RWTValSlist()                                : impl_() {}
#ifndef RW_ALLOCATOR_PLAIN
  RWTValSlist(const rw_slist<T,allocator<T> >& lst) : impl_(lst) {}
#else
  RWTValSlist(const rw_slist<T>& lst)          : impl_(lst) {}
#endif
  RWTValSlist(const RWTValSlist<T>& lst)       : impl_(lst.impl_) {}
  RWTValSlist(size_type n, const T& t = T())   : impl_(n,t) {}
  RWTValSlist(const T* first, const T*last)    : impl_(first,last) {}
//  ~RWTValSlist(); // handled by auto-generated destructor

private:
  container_type impl_;         // djk: use typedef
  friend class RW_VBase<container_type,this_type>;

  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

RWDECLARE_PERSISTABLE_TEMPLATE(RWTValSlist)

/****************************************************************
 *                                                              *
 *      Declarations for RWTValSlistIterator<T>                 *
 *                                                              *
 ****************************************************************/

template <class T> class RWTExport RWTValSlistIterator
{
private:
  RWTValSlist<T>*                   list_;
  typename RWTValSlist<T>::iterator iter_;
  bool                              head_;  // simulate head sentinel
  bool                              tail_;  // simulate tail sentinel
/*
 * Invariants:
 *   head_ and tail_ can never both be true
 *   if head_ and tail_ are both false then iter_ is valid
 *     (unless invalidated from outside this class)
 */

public:

  RWTValSlistIterator(RWTValSlist<T>& s)
          : list_(&s), head_(true), tail_(false) { }

  // Operators:
  RWBoolean     operator++();
  RWBoolean     operator+=(size_t n);
  RWBoolean     operator()()        { return ++(*this); }

  // Methods
  RWTValSlist<T>* container() const { return list_; }

  RWBoolean     findNext(const T& a);
  RWBoolean     findNext(RWBoolean (*testFun)(const T&, void*), void*);

  void          insertAfterPoint(const T& a);

  T             key() const   { return *iter_; }

  RWBoolean     remove();       // Remove item at cursor

  RWBoolean     removeNext(const T&);
  RWBoolean     removeNext(RWBoolean (*testFun)(const T&, void*), void*);

  void          reset()       { head_ = true; tail_ = false; } 
  void          reset(RWTValSlist<T>& s)
    { head_ = true; tail_ = false; list_ = &s; } 

private:

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/tvslist.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */

#endif /* __RWTVSLIST_H__ */
