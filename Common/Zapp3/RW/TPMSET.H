#ifndef __RWTPMSET_H__
#define __RWTPMSET_H__

/***************************************************************************
 *
 * tpmset.h - RWTPtrMultiSet<T,Compare>
 *   
 *     : pointer-based set wrapper for STL multiset
 * 
 * $Id: tpmset.h@#/main/20  10/09/96 22:02:48  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tpmset.h,v $
 * Revision 7.21  1996/10/09 22:02:48  griswolf
 * Oops: provide impl_ initializer.
 *
 * Revision 7.20  1996/10/09 21:54:41  griswolf
 * Scopus #4893: provide ctor(T*,T*) even if impl does not.
 *
 * Revision 7.19  1996/09/19 18:25:21  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.18  1996/08/23 18:30:02  hart
 * removed work around for HP aCC compiler not inheriting operators from base
 *
 * Revision 7.17  1996/08/09 19:25:26  hart
 * HP (aCC compiler) port
 *
 * Revision 7.16  1996/05/07 19:56:46  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.15  1996/02/19 12:34:12  jims
 * Port to Stdlib 1.2
 *
 * Revision 7.14  1996/02/18 01:47:45  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.13  1996/02/11 19:59:30  pearson
 * Make isSubsetof, isProperSubsetOf and isEquivalent const member functions
 *
 * Revision 7.12  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.11  1996/01/24 21:07:41  jims
 * Simplify implementation of otis-stl ptr-based collections
 * by using container<T*> impls instead of container<void*>
 *
 * Revision 7.10  1996/01/23 07:40:54  jims
 * Use template args when class used as formal parameter
 *
 * Revision 7.9  1996/01/13 22:31:59  kevinj
 * Simple External Template Persistence (ETP).
 *
 * Revision 7.8  1996/01/04 00:51:37  pearson
 * Change isSubset to isSubsetOf
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
 * Revision 7.1  1995/11/29 18:27:27  jims
 * Move to revision 7.1
 *
 * Revision 7.13  1995/10/20  06:46:20  jims
 * Port to msvc 4.0: don't use access adjustment for typedefs
 *
 * Revision 7.12  1995/10/06  20:15:52  griswolf
 * Scopus #2160: remove public access to Union(std()) etc.
 *
 * Revision 7.11  1995/09/25  11:09:35  jims
 * Add NO_CONST_INST version of T**,T** ctor
 *
 * Revision 7.10  1995/09/05  21:51:38  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.9  1995/08/18  21:49:20  kevinj
 * Added RW_DEFINE_PERSISTABLE_COLLECTION.
 *
 * Revision 7.8  1995/08/15  18:58:58  kevinj
 * Added restoreFrom and saveOn.
 *
 * Revision 7.7  1995/08/09  00:22:54  jims
 * multiset now in <set> header file
 *
 * Revision 7.6  1995/07/27  22:02:33  jims
 * Add header with file name, short description, $Id and $Log
 * 
 ****************************************************************************/

#include "rw/compiler.h"

#ifdef RW_NO_STL
#  error Cannot include header if RW_NO_STL macro is defined for your compiler
#endif

#include <set>
#include "rw/ep_inset.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T,class C>
class RWTExport RWTPtrMultiSet
#ifndef RW_ALLOCATOR_PLAIN
    : private RW_PAssoc<multiset<T*, rw_deref_compare<C,T>,allocator<T*> >,
                        RWTPtrMultiSet<T,C>, T>
#else
    : private RW_PAssoc<multiset<T*, rw_deref_compare<C,T>,allocator>,
                        RWTPtrMultiSet<T,C>, T>
#endif
{
public:
// typedefs
  typedef rw_deref_compare<C,T>                          container_comp;
#ifndef RW_ALLOCATOR_PLAIN
  typedef multiset<T*, container_comp,allocator<T*> >    container_type;
#else
  typedef multiset<T*, container_comp,allocator>         container_type;
#endif
  typedef RWTPtrMultiSet<T,C>                            this_type;
  typedef RW_PAssoc<container_type, this_type, T>        base_type;

  typedef typename container_type::size_type             size_type;
  typedef typename container_type::difference_type       difference_type;

  typedef typename base_type::iterator                   iterator;
  typedef typename base_type::const_iterator             const_iterator;
  typedef typename base_type::value_type                 value_type;
  typedef typename base_type::const_value                const_value;
  typedef typename base_type::reference                  reference;
  typedef typename base_type::const_reference            const_reference;

// STL-level access
  const container_type&  std() const { return impl_; }
  container_type& std() { return impl_; }
  
// inherited interface components
  base_type::begin; 
  base_type::isEmpty;
  base_type::end;
  base_type::entries;
  base_type::operator==;
  base_type::operator<;
  base_type::clear;
  base_type::clearAndDestroy;
  base_type::contains;
  base_type::find;
  base_type::occurrencesOf;
  base_type::remove;
  base_type::removeAll;

  void  Union(const this_type& s) { base_type::Union(s); }
  void  intersection(const this_type& s) { base_type::intersection(s); }
  void  difference(const this_type& s) { base_type::difference(s); }
  void  symmetricDifference(const this_type& s)
        { base_type::symmetricDifference(s); }
   
  RWBoolean 
     isSubsetOf(const RWTPtrMultiSet<T,C>& rhs) const
     { return base_type::isSubsetMultiOf(rhs); }

  RWBoolean
     isProperSubsetOf(const RWTPtrMultiSet<T,C>& rhs) const
     { return base_type::isProperSubsetMultiOf(rhs); }

  RWBoolean
     isEquivalent(const RWTPtrMultiSet<T,C>& rhs) const
     { return base_type::isEquivalentMulti(rhs); }


// selective inheritance (hides other same-name components)
  void  apply(void (*fn)(const T*,void*),void *x) const
    { base_type::apply(fn,x); }

// additional interface components
  bool  insert(T* datum) { impl_.insert(datum); return true; } 

// assignment operators
  this_type&   operator=(const this_type& rwmset)
    { impl_ = rwmset.impl_; return *this; }
  this_type&   operator=(const container_type& stdmset)
    { impl_ = stdmset; return *this; }
  
// constructors
  RWTPtrMultiSet(const container_comp& cmp = container_comp()) : impl_(cmp) {}
  RWTPtrMultiSet(const container_type& s)               : impl_(s) { }
  RWTPtrMultiSet(const this_type& rws)                  : impl_(rws.impl_) { }

  RWTPtrMultiSet(T*const* first, T*const* last,
                 const container_comp& cmp = container_comp())
#ifdef RW_STLCTOR_PTR_HACK
    : impl_(first,last,cmp) { }
#else
  : impl_(cmp)
   { while(first != last) { impl_.insert(*first); ++first; } }
#endif

private:
  container_type impl_;
  friend class RW_PBase<container_type, this_type>;
};

RWDECLARE_PERSISTABLE_TEMPLATE_2(RWTPtrMultiSet)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tpmset.cc"
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrMultiSetIterator<T,H,EQ>         *
 *                                                              *
 ****************************************************************/

template <class T, class C>
class RWTExport RWTPtrMultiSetIterator
{ 

public:

  RWTPtrMultiSetIterator(RWTPtrMultiSet<T,C>& t)
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

  T*              operator()()       { return (++(*this)) ? key() : rwnil; }
  RWTPtrMultiSet<T,C>*
                  container() const  { return table_; }
  T*              key() const        { return *iter_; }
  void            reset()            { head_ = true; tail_ = false; } 

  void  reset(RWTPtrMultiSet<T,C>& t)
    { head_ = true; tail_ = false; table_ = &t; } 

private:

  RWTPtrMultiSet<T,C>*                        table_;
  typename RWTPtrMultiSet<T,C>::iterator      iter_;
  bool                                        head_;
  bool                                        tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /*  __RWTPMSET_H__ */
