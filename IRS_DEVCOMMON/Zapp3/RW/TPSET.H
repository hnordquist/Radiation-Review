#ifndef __RWTPSET_H__
#define __RWTPSET_H__

/***************************************************************************
 *
 * tpset.h - RWTPtrSet<T,Compare>
 *   
 *     : pointer-based set wrapper for STL set
 * 
 * $Id: tpset.h@#/main/19  10/09/96 22:02:00  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tpset.h,v $
 * Revision 7.20  1996/10/09 22:02:00  griswolf
 * Scopus #4893: provide ctor(T*,T*) even if impl does not.
 *
 * Revision 7.19  1996/09/19 18:25:05  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.18  1996/08/23 18:30:02  hart
 * removed work around for HP aCC compiler not inheriting operators from base
 *
 * Revision 7.17  1996/08/09 19:27:12  hart
 * HP (aCC compiler) port
 *
 * Revision 7.16  1996/05/07 19:57:14  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.15  1996/04/29 19:12:43  groening
 * needed to include compiler.h prior to making any references to
 * RW_NO_STL.
 *
 * Revision 7.14  96/02/19  12:34:12  12:34:12  jims (Jim Shur)
 * Port to Stdlib 1.2
 * 
 * Revision 7.13  1996/02/18 01:47:49  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.12  1996/02/11 20:49:53  pearson
 * Uncomment assignment operator and constructor methods
 *
 * Revision 7.11  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.10  1996/01/24 21:07:41  jims
 * Simplify implementation of otis-stl ptr-based collections
 * by using container<T*> impls instead of container<void*>
 *
 * Revision 7.9  1996/01/12 01:39:14  kevinj
 * RWT{Val,Ptr}Set External Template Persistence (ETP).
 *
 * Revision 7.8  1996/01/04 00:51:49  pearson
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
 * Revision 7.1  1995/11/29 03:23:27  jims
 * Move to revision 7.1
 *
 * Revision 7.13  1995/10/20  06:46:20  jims
 * Port to msvc 4.0: don't use access adjustment for typedefs
 *
 * Revision 7.12  1995/10/06  20:16:51  griswolf
 * Scopus #2160: remove public access to Union(std()) etc.
 *
 * Revision 7.11  1995/09/25  11:09:35  jims
 * Add NO_CONST_INST version of T**,T** ctor
 *
 * Revision 7.10  1995/09/21  00:25:25  griswolf
 * Remove constructor and assign from base_type.
 *
 * Revision 7.9  1995/09/05  21:51:51  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.8  1995/08/18  18:10:06  kevinj
 * Added RW_DEFINE_PERSISTABLE_COLLECTION.
 *
 * Revision 7.7  1995/08/15  19:14:27  kevinj
 * Added restoreFrom and saveOn.
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
#include "rw/ep_inset.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T, class C>
#ifndef RW_ALLOCATOR_PLAIN
class RWTExport RWTPtrSet
    : private RW_PAssoc<set<T*,rw_deref_compare<C,T>,allocator<T*> >, 
          RWTPtrSet<T,C>, T>
#else
class RWTExport RWTPtrSet
    : private RW_PAssoc<set<T*,rw_deref_compare<C,T>,allocator>, 
          RWTPtrSet<T,C>, T>
#endif
{
public:
// typedefs
  typedef rw_deref_compare<C,T>                              container_comp;
#ifndef RW_ALLOCATOR_PLAIN
  typedef set<T*,container_comp,allocator<T*> >              container_type;
#else
  typedef set<T*,container_comp,allocator>                   container_type;
#endif
  typedef RWTPtrSet<T,C>                                     this_type;
  typedef RW_PAssoc<container_type, this_type, T>            base_type;
  typedef typename container_type::size_type                 size_type;
  typedef typename container_type::difference_type           difference_type;
  
  typedef typename base_type::iterator                       iterator;
  typedef typename base_type::const_iterator                 const_iterator;
  typedef typename base_type::value_type                     value_type;
  typedef typename base_type::const_value                    const_value;
  typedef typename base_type::reference                      reference;
  typedef typename base_type::const_reference                const_reference;

// STL-level access
  const container_type& std() const { return impl_; }
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
  base_type::isSubsetOf;
  base_type::isProperSubsetOf;
  base_type::isEquivalent;

  void  Union(const this_type& s) { base_type::Union(s); }
  void  intersection(const this_type& s) { base_type::intersection(s); }
  void  difference(const this_type& s) { base_type::difference(s); }
  void  symmetricDifference(const this_type& s)
        { base_type::symmetricDifference(s); }

// selective inheritance (hides other same-name components)
  void  apply(void (*fn)(const_value,void*),void* x) const
    { base_type::apply(fn,x); }

// additional interface components
  bool  insert(value_type datum)
    { return impl_.insert(datum).second; }

// assignment operators
  this_type&   operator=(const this_type& rwset)
    { impl_ = rwset.impl_; return *this; }
  this_type&   operator=(const container_type& stdset)
    { impl_ = stdset; return *this; }
  
// constructors
  RWTPtrSet(const container_comp& cmp = container_comp())  : impl_(cmp) { }
  RWTPtrSet(const container_type& s)               : impl_(s) { }
  RWTPtrSet(const this_type& rws)                  : impl_(rws.impl_) { }

  RWTPtrSet(T*const* first, T*const* last,
            const container_comp& cmp = container_comp())
#ifdef RW_STLPTR_CTR_HACK
    : impl_(first,last,cmp) { }
#else
  : impl_(cmp)
   { while(first != last) { impl_.insert(*first); ++first; } }
#endif
private:
  container_type impl_;
  friend class RW_PBase<container_type, this_type>;
};

RWDECLARE_PERSISTABLE_TEMPLATE_2(RWTPtrSet)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tpset.cc"
#endif


/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrSetIterator<T,H,EQ>              *
 *                                                              *
 ****************************************************************/

template <class T, class C>
class RWTExport RWTPtrSetIterator
{ 

public:

  RWTPtrSetIterator(RWTPtrSet<T,C>& t)
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
  RWTPtrSet<T,C>* container() const  { return table_; }
  T*              key() const        { return *iter_; }
  void            reset()            { head_ = true; tail_ = false; } 

  void  reset(RWTPtrSet<T,C>& t)
    { head_ = true; tail_ = false; table_ = &t; } 

private:

  RWTPtrSet<T,C>*                        table_;
  typename RWTPtrSet<T,C>::iterator      iter_;
  bool                                   head_;
  bool                                   tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWTPSET_H__ */
