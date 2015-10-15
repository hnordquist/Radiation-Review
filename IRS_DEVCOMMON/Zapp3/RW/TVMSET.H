#ifndef __RWTVMSET_H__
#define __RWTVMSET_H__

/***************************************************************************
 *
 * tvmset.h - RWTValMultiSet<T,Compare>
 *   
 *     : value-based set wrapper for STL multiset
 * 
 * $Id: tvmset.h@#/main/18  10/09/96 22:45:25  griswolf (TOOLS707_WIN32_971021)
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
 ****************************************************************************
 * 
 * $Log: tvmset.h,v $
 * Revision 7.19  1996/10/09 22:45:25  griswolf
 * Scopus #4893: provide ctor(T*,T*) even if impl does not.
 *
 * Revision 7.18  1996/09/19 18:25:59  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.17  1996/08/23 18:30:02  hart
 * removed work around for HP aCC compiler not inheriting operators from base
 *
 * Revision 7.16  1996/08/09 19:40:52  hart
 * HP (aCC compiler) port
 *
 * Revision 7.15  1996/05/07 19:04:58  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.14  1996/02/19 12:34:12  jims
 * Port to Stdlib 1.2
 *
 * Revision 7.13  1996/02/18 01:49:20  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.12  1996/02/11 20:34:28  pearson
 * Make isSubsetOf, isEquivalent, and isProperSubsetOf const member functions
 *
 * Revision 7.11  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.10  1996/01/23 07:57:42  jims
 * Use "this_type" for formal paramters in subset ops
 *
 * Revision 7.9  1996/01/13 22:31:34  kevinj
 * Simple External Template Persistence (ETP).
 *
 * Revision 7.8  1996/01/04 16:28:39  pearson
 * Change isSubset to isSubsetOf and isProperSubset to isProperSubsetOf
 *
 * Revision 7.7  1995/12/20 23:19:20  kevinj
 * External template persistence using RW*Persistor.
 *
 * Revision 7.6  1995/12/14 18:01:21  jims
 * saveOn, restoreFrom now global instead of member functions
 *
 * Revision 7.5  1995/12/13 00:22:58  jims
 * Remove member functions saveOn and restoreFrom (now global)
 *
 * Revision 7.4  1995/12/12 00:03:06  jims
 * Add RWTExport
 *
 * Revision 7.3  1995/12/11 22:30:41  jims
 * Remove 'Sca' from interior class names
 * Add 'rw_' to global helper classes
 * Change 'muterator' to 'filterator'
 *
 * Revision 7.2  1995/12/01 18:08:01  jims
 * Rename to 6.1 style name, add 6.1 style external iterator
 *
 * Revision 7.1  1995/11/29 20:19:34  jims
 * Move to revision 7.1
 *
 * Revision 7.11  1995/09/05  21:56:11  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.10  1995/08/18  21:54:30  kevinj
 * Added RW_DEFINE_PERSISTABLE_COLLECTION.
 *
 * Revision 7.9  1995/08/15  19:19:18  kevinj
 * Added restoreFrom, saveOn.
 *
 * Revision 7.8  1995/08/09  00:21:33  jims
 * multiset now in <set> header file
 *
 * Revision 7.7  1995/08/02  00:04:36  jims
 * Qualify include file names with "rw/"
 *
 * Revision 7.6  1995/07/27  22:02:33  jims
 * Add header with file name, short description, $Id and $Log
 * 
 ****************************************************************************/

#include "rw/compiler.h"

#ifdef RW_NO_STL
#error Cannot include header if RW_NO_STL macro is defined for your compiler
#endif

#include <set>
#include "rw/ev_inset.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T,class C> // contained type, compare object
#ifndef RW_ALLOCATOR_PLAIN
class RWTExport RWTValMultiSet
    : private RW_VAssoc<multiset<T,C,allocator<T> >, RWTValMultiSet<T,C> >
#else
class RWTExport RWTValMultiSet
    : private RW_VAssoc<multiset<T,C,allocator>, RWTValMultiSet<T,C> >
#endif
{
public:
// typedefs
#ifndef RW_ALLOCATOR_PLAIN
  typedef multiset<T,C,allocator<T> >                       container_type;
#else
  typedef multiset<T,C,allocator>                           container_type;
#endif
  typedef RWTValMultiSet<T,C>                               this_type;
  typedef RW_VAssoc<container_type,this_type>               base_type;
  typedef typename container_type::iterator                 iterator;
  typedef typename container_type::const_iterator           const_iterator;
  typedef typename container_type::size_type                size_type;
  typedef typename container_type::value_type               value_type;
  typedef typename container_type::key_type                 key_type;
  typedef typename container_type::reference                reference;
  typedef typename container_type::const_reference          const_reference;

// STL-level access
  container_type&  std()              { return impl_; }
  const container_type&  std() const  { return impl_; }

// inherited interface components
  base_type::begin;
  base_type::end;
  base_type::isEmpty;
  base_type::entries;
  base_type::operator==;
  base_type::operator<;
  base_type::clear;
  base_type::contains;
  base_type::find;
  base_type::occurrencesOf;
  base_type::remove;
  base_type::removeAll;
  base_type::Union;
  base_type::intersection;
  base_type::difference;
  base_type::symmetricDifference;
        
  RWBoolean 
     isSubsetOf(const this_type& rhs) const
     { return base_type::isSubsetMultiOf(rhs); }

  RWBoolean
     isProperSubsetOf(const this_type& rhs) const
     { return base_type::isProperSubsetMultiOf(rhs); }

  RWBoolean
     isEquivalent(const this_type& rhs) const
     { return base_type::isEquivalentMulti(rhs); }

// selective inheritance (hides other same-name components)
  void  apply(void (*fn)(const_reference,void*),void *x) const
    { base_type::apply(fn,x); }

// additional interface components
  bool  insert(const_reference datum) { impl_.insert(datum); return true; } 
  
// assignment operators
  RWTValMultiSet<T,C>&   operator=(const RWTValMultiSet<T,C>& rwmset)
    { impl_ = rwmset.impl_; return *this; }
  RWTValMultiSet<T,C>&   operator=(const container_type& stdmset)
    { impl_ = stdmset; return *this; }

// constructors
  RWTValMultiSet(const C& cmp = C()) : impl_(cmp) { }
  RWTValMultiSet(const container_type& s) : impl_(s) { }
  RWTValMultiSet(const RWTValMultiSet<T,C>& rws) : impl_(rws.impl_) { }
  RWTValMultiSet(const T* first, const T* last, const C& cmp = C())
#ifdef RW_STLCTOR_PTR_HACK
    : impl_(first,last,cmp) { }
#else
    : impl_(cmp)
    { while(first != last) { impl_.insert(*first); ++first; } }
#endif
private:
  container_type impl_;     // clients use std()
  friend class RW_VBase<container_type,this_type>;
};

RWDECLARE_PERSISTABLE_TEMPLATE_2(RWTValMultiSet)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tvmset.cc"
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWTValMultiSetIterator<T,C>            *
 *                                                              *
 ****************************************************************/

template <class T, class C>
class RWTExport RWTValMultiSetIterator
{ 

public:

  RWTValMultiSetIterator(RWTValMultiSet<T,C>& t)
          : table_(&t), head_(true), tail_(false) { }

  RWBoolean     operator++()    // Advance and test
  {
    RWASSERT(!(head_ == true  &&  tail_ == true)); 

    if (tail_)
      /* do nothing */ ;

    else if (head_) {
      iter_ = table_->begin();
      head_ = false;
    }
    else
      ++iter_;

    if (!tail_ && iter_ == table_->end())
      tail_ = true;

    RWASSERT(!(head_ == true  &&  tail_ == true)); 

    return !tail_;
  }

  RWBoolean     operator()()    { return ++(*this); }
        
  RWTValMultiSet<T,C>* container() const  { return table_; }

  T             key() const   { return *iter_; }

  void          reset()       { head_ = true; tail_ = false; } 
  void          reset(RWTValMultiSet<T,C>& t)
                              { head_ = true; tail_ = false; table_ = &t; } 
private:

  RWTValMultiSet<T,C>*                    table_;
  typename RWTValMultiSet<T,C>::iterator  iter_;
  bool                                    head_;
  bool                                    tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWTVMSET_H__ */
