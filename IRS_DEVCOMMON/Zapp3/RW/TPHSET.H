#ifndef __RWTPHSET_H__
#define __RWTPHSET_H__

/***************************************************************************
 *
 * tphset.h - RWTPtrHashSet<T,H,EQ>
 *   
 *     : pointer-based set wrapper for STL-like rw_hashset
 * 
 * $Id: tphset.h@#/main/40  09/04/97 17:25:43  griswolf (TOOLS707_WIN32_971021)
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
 */

#include "rw/compiler.h"

#ifdef RW_NO_STL
#include "rw/xphset.h"
#define RWDefHArgs(T)

#else

#ifndef RWDefHArgs
#  define RWDefHArgs(T) ,RWTHasher<T>,equal_to<T>
#endif

// STL-based implementation:


#include "rw/stdex/hashset.h"
#include "rw/ep_hash.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T,class H,class EQ>
#ifndef RW_ALLOCATOR_PLAIN
class RWTExport RWTPtrHashSet
    : private RW_PHAssoc<
      rw_hashset<T*, rw_deref_hash<H,T>,rw_deref_compare<EQ,T>, allocator<T*> >,
                 RWTPtrHashSet<T,H,EQ>, T>
#else
class RWTExport RWTPtrHashSet
    : private RW_PHAssoc<
      rw_hashset<T*, rw_deref_hash<H,T>,rw_deref_compare<EQ,T> >,
                 RWTPtrHashSet<T,H,EQ>, T>
#endif
{
public:
// typedefs
  typedef rw_deref_compare<EQ,T>                      container_eq;
  typedef rw_deref_hash<H,T>                          container_hash;
#ifndef RW_ALLOCATOR_PLAIN
  typedef rw_hashset<T*,container_hash, container_eq, allocator<T*> >
                                                      container_type;
#else
  typedef rw_hashset<T*,container_hash, container_eq> container_type;
#endif
  typedef RWTPtrHashSet<T,H,EQ>                       this_type;
  typedef RW_PHAssoc<container_type, this_type, T>    base_type;
  typedef base_type                                   base1_type;
  
#ifndef RW_BROKEN_ACCESS_ADJUST
   typedef base1_type                                 base2_type;
   typedef base1_type                                 base3_type;
   typedef base1_type                                 base4_type;
#else  
   typedef RW_PAssoc<container_type, this_type, T>    base2_type;
   typedef RW_PCntnr<container_type, this_type, T>    base3_type;
   typedef RW_PBase<container_type, this_type>        base4_type;
#endif

  typedef typename container_type::size_type          size_type;
  typedef typename container_type::difference_type    difference_type;
  
  typedef typename base_type::iterator                iterator;
  typedef typename base_type::const_iterator          const_iterator;
  typedef typename base_type::value_type              value_type;
  typedef typename base_type::const_value             const_value;
  typedef typename base_type::reference               reference;
  typedef typename base_type::const_reference         const_reference;

// STL-level access
  const container_type& std() const { return impl_; }
  container_type& std() { return impl_; }

// inherited interface components
  base4_type::begin; 
  base4_type::isEmpty;
  base4_type::end;
  base4_type::entries;
  base2_type::clear;
  base2_type::clearAndDestroy;
  base2_type::contains;
  base2_type::find;
  base2_type::occurrencesOf;
  base2_type::remove;
  base2_type::removeAll;
  base2_type::isSubsetOf;
  base2_type::isProperSubsetOf;
  base2_type::isEquivalent;

  RWTPtrHashSet<T,H,EQ> &
    Union(const RWTPtrHashSet<T,H,EQ>& rhs)
    { base_type::UnionHash(rhs); return *this; }

  RWTPtrHashSet<T,H,EQ> &
    intersection(const RWTPtrHashSet<T,H,EQ>& rhs)
    { base_type::intersectionHash(rhs); return *this; }

  RWTPtrHashSet<T,H,EQ> &
    difference(const RWTPtrHashSet<T,H,EQ>& rhs)
    { base_type::differenceHash(rhs); return *this; }

  RWTPtrHashSet<T,H,EQ> &
    symmetricDifference(const RWTPtrHashSet<T,H,EQ>& rhs)
    { base_type::symmetricDifferenceHash(rhs); return *this; }

// overridden interface comoponents
  bool operator==(const this_type& rwset) const { 
    return this->entries() == rwset.entries() && hashCollEqual(*this, rwset);}

// selective inheritance (hides other same-name components)
  void  apply(void (*fn)(const_value,void*),void *x) const
    { base_type::apply(fn,x); }

// additional interface components
  bool  insert(value_type datum)
    { return (impl_.insert(datum)).second; }

  size_type
    capacity() const { return impl_.capacity(); }

  float
    fillRatio() const { return impl_.fill_ratio(); }

  void
    resize(size_type newsz) { impl_.resize(newsz); }

// assignment operators
  this_type&   operator=(const this_type& rwset)
    { impl_ = rwset.impl_; return *this; }
  
// constructors
  RWTPtrHashSet(size_type sz=1024, const H& h = H(), const EQ& eq = EQ())
    : impl_(sz,rw_deref_hash<H,T>(h),rw_deref_compare<EQ,T>(eq)) { }

  RWTPtrHashSet(const this_type& rws)                  : impl_(rws.impl_) { }
  RWTPtrHashSet(const container_type& s)               : impl_(s) { }

  RWTPtrHashSet(T*const* first,T*const* last,
            size_type sz=1024, const H& h = H(), const EQ& eq = EQ())
    : impl_(first,last,
            sz,rw_deref_hash<H,T>(h),rw_deref_compare<EQ,T>(eq)) {}

  // Tools-6 compatible constructor:
  RWTPtrHashSet(const H& h, size_type sz = RWDEFAULT_CAPACITY)
    : impl_(sz,h,EQ()) { }

private:
  container_type impl_;
  friend class RW_PBase<container_type, this_type>;
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

RWDECLARE_PERSISTABLE_TEMPLATE_3(RWTPtrHashSet)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tphset.cc"
#endif


/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrHashSetIterator<T,H,EQ>          *
 *                                                              *
 ****************************************************************/

template <class T, class H, class EQ>
class RWTExport RWTPtrHashSetIterator
{ 

public:

  RWTPtrHashSetIterator(RWTPtrHashSet<T,H,EQ>& t)
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

  T*    operator()()    { return (++(*this)) ? key() : rwnil; }
        
  RWTPtrHashSet<T,H,EQ>* container() const  { return table_; }

  T*    key() const   { return *iter_; }

  void  reset()       { head_ = true; tail_ = false; } 
  void  reset(RWTPtrHashSet<T,H,EQ>& t)
                              { head_ = true; tail_ = false; table_ = &t; } 

private:

  RWTPtrHashSet<T,H,EQ>*                         table_;
  typename RWTPtrHashSet<T,H,EQ>::iterator       iter_;
  bool                                           head_;
  bool                                           tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */
#endif /* __RWTPHSET_H__ */
