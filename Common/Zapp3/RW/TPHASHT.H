#ifndef __RWTPHMSET_H__
#define __RWTPHMSET_H__

/***************************************************************************
 *
 * tphasht.h -  RWTPtrHashMultiSet<T,H,EQ>
 *   
 *     : pointer-based set wrapper for STL-like rw_hashmultiset
 * 
 * $Id: tphasht.h@#/main/46  09/04/97 17:25:36  griswolf (TOOLS707_WIN32_971021)
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
#  include "rw/xphasht.h"
#  define RWDefHArgs(T)
#else

#ifndef RWDefHArgs
#  define RWDefHArgs(T) ,RWTHasher<T>,equal_to<T>
#endif

// STL-based implementation:

#include "rw/stdex/hashmset.h"
#include "rw/ep_hash.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

#ifndef RW_ALLOCATOR_PLAIN
template <class T, class H, class EQ>
class RWTExport RWTPtrHashMultiSet : private
 RW_PHAssoc<rw_hashmultiset<T*,rw_deref_hash<H,T>,
            rw_deref_compare<EQ,T>, allocator<T*> >,
            RWTPtrHashMultiSet<T,H,EQ>, T>
#else
template <class T, class H, class EQ>
class RWTExport RWTPtrHashMultiSet : private
 RW_PHAssoc<rw_hashmultiset<T*,rw_deref_hash<H,T>,
            rw_deref_compare<EQ,T> >,
            RWTPtrHashMultiSet<T,H,EQ>, T>
#endif   
{
public:
// typedefs
  typedef rw_deref_compare<EQ,T>                           container_eq;
  typedef rw_deref_hash<H,T>                               container_hash;
#ifndef RW_ALLOCATOR_PLAIN
  typedef rw_hashmultiset<T*,container_hash, container_eq,allocator<T*> >
                                                           container_type;
#else
  typedef rw_hashmultiset<T*,container_hash, container_eq> container_type;
#endif
  typedef RWTPtrHashMultiSet<T,H,EQ>                       this_type;
  typedef RW_PHAssoc<container_type, this_type, T>         base_type;
  typedef base_type                                        base1_type;
  
# ifndef RW_BROKEN_ACCESS_ADJUST
  typedef base1_type                                       base2_type;
  typedef base1_type                                       base3_type;
  typedef base1_type                                       base4_type;
# else
  typedef RW_PAssoc<container_type, this_type, T>          base2_type;
  typedef RW_PCntnr<container_type, this_type, T>          base3_type;
  typedef RW_PBase<container_type, this_type>              base4_type;
# endif  /* RW_BROKEN_ACCESS_ADJUST */

  typedef typename container_type::size_type               size_type;
  typedef typename container_type::difference_type         difference_type;

  typedef typename base1_type::iterator                    iterator;
  typedef typename base1_type::const_iterator              const_iterator;
  typedef typename base1_type::value_type                  value_type;
  typedef typename base1_type::const_value                 const_value;
  typedef typename base1_type::reference                   reference;
  typedef typename base1_type::const_reference             const_reference;

// STL-level access
  const container_type&  std() const { return impl_; }
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

  RWBoolean
    isSubsetOf(const RWTPtrHashMultiSet<T,H,EQ>& rhs) const
    { return base_type::isSubsetMultiOf(rhs); }

  RWBoolean
    isProperSubsetOf(const RWTPtrHashMultiSet<T,H,EQ>& rhs) const
    { return base_type::isProperSubsetMultiOf(rhs); }

  RWBoolean
    isEquivalent(const RWTPtrHashMultiSet<T,H,EQ>& rhs) const
    { return base_type::isEquivalentMulti(rhs); }

  void
    Union(const RWTPtrHashMultiSet<T,H,EQ>& rhs)
      { base_type::UnionHash(rhs); }

  void
    intersection(const RWTPtrHashMultiSet<T,H,EQ>& rhs)
        { base_type::intersectionHash(rhs); }

  void
    difference(const RWTPtrHashMultiSet<T,H,EQ>& rhs)
        { base_type::differenceHash(rhs); }

  void
    symmetricDifference(const RWTPtrHashMultiSet<T,H,EQ>& rhs)
        { base_type::symmetricDifferenceHash(rhs); }

// overridden interface comoponents
  bool operator==(const this_type& rwset) const
  { return this->entries() == rwset.entries() && hashCollEqual(*this, rwset); }

// selective inheritance (hides other same-name components)
  void  apply(void (*fn)(const T*,void*),void *x) const
    { base_type::apply(fn,x); }

// additional interface components
  bool  insert(T* datum) { impl_.insert(datum); return true; } 

  size_type
    capacity() const { return impl_.capacity(); }

  float
    fillRatio() const { return impl_.fill_ratio(); }

  void
    resize(size_type newsz) { impl_.resize(newsz); }

// assignment operators
  this_type&   operator=(const this_type& rwmset)
    { impl_ = rwmset.impl_; return *this; }

// constructors
  RWTPtrHashMultiSet(size_type sz=1024, const H& h = H(), const EQ& eq = EQ())
    : impl_(sz,rw_deref_hash<H,T>(h),rw_deref_compare<EQ,T>(eq)) { }

  RWTPtrHashMultiSet(const this_type& rws)      : impl_(rws.impl_) { }
  RWTPtrHashMultiSet(const container_type& s)   : impl_(s) { }

  RWTPtrHashMultiSet(T*const* first,T*const* last,
                 size_type sz=1024, const H& h = H(), const EQ& eq = EQ())
    : impl_(first,last,
            sz,rw_deref_hash<H,T>(h),rw_deref_compare<EQ,T>(eq)) {}

  // Tools-6 compatible constructor:
  RWTPtrHashMultiSet(const H& h, size_type sz = RWDEFAULT_CAPACITY)
    : impl_(sz,h,EQ()) { }

private:
  container_type impl_;
  friend class RW_PBase<container_type, this_type>;
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

RWDECLARE_PERSISTABLE_TEMPLATE_3(RWTPtrHashMultiSet)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tphasht.cc"
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrHashMultiSetIterator<T,H,EQ>     *
 *                                                              *
 ****************************************************************/

template <class T, class H, class EQ>
class RWTExport RWTPtrHashMultiSetIterator
{

public:

  RWTPtrHashMultiSetIterator(RWTPtrHashMultiSet<T,H,EQ>& t)
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
        
  RWTPtrHashMultiSet<T,H,EQ>* container() const  { return table_; }

  T*    key() const   { return *iter_; }

  void  reset()       { head_ = true; tail_ = false; } 
  void  reset(RWTPtrHashMultiSet<T,H,EQ>& t)
                              { head_ = true; tail_ = false; table_ = &t; } 

private:

  RWTPtrHashMultiSet<T,H,EQ>*                    table_;
  typename RWTPtrHashMultiSet<T,H,EQ>::iterator  iter_;
  bool                                           head_;
  bool                                           tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};


#define RWTPtrHashTable RWTPtrHashMultiSet
#define RWTPtrHashTableIterator RWTPtrHashMultiSetIterator

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif


#endif /* RW_NO_STL */
#endif /*  __RWTPHMSET_H__ */
