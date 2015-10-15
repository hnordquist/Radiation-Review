#ifndef __RWTPSRTVEC_H__
#define __RWTPSRTVEC_H__

/***************************************************************************
 *
 * tpsrtvec.h - RWTPtrSortedVector<T,C>
 *   
 *     : pointer-based sorted vector wrapper for STL vector
 * 
 * $Id: tpsrtvec.h@#/main/43  09/04/97 17:25:54  griswolf (TOOLS707_WIN32_971021)
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
// Tools 6.1 implementation:
#  include "rw/xpsrtvec.h"
#  undef RWDefCArgs
#  define RWDefCArgs(T)

#else
#  ifndef RWDefCArgs
#    define RWDefCArgs(T) ,less<T>
#  endif

// STL-based implementation:

#include "rw/ep_srtsq.h"   // #2904 - before #include<vector>

#ifndef RW_HEADER_WITH_EXTENSION
# include <vector>
#else
# include <vector.hpp>
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T, class C>
#ifndef RW_ALLOCATOR_PLAIN
class RWTExport RWTPtrSortedVector
  : private RW_PSrtSeq<vector<T*,allocator<T*> >, RWTPtrSortedVector<T,C>,
                       rw_deref_compare<C,T>, T>
#else
class RWTExport RWTPtrSortedVector
  : private RW_PSrtSeq<vector<T*,allocator>, RWTPtrSortedVector<T,C>,
                       rw_deref_compare<C,T>, T>
#endif
{
public:
  typedef RWTPtrSortedVector<T,C>                          this_type;
#ifndef RW_ALLOCATOR_PLAIN
  typedef vector<T*,allocator<T*> >                        container_type;
#else
  typedef vector<T*,allocator>                             container_type;
#endif
  typedef rw_deref_compare<C,T>                            container_comp;
  typedef RW_PSrtSeq<container_type,
                     this_type, container_comp, T>         base1_type;

  typedef typename container_type::value_type              value_type;
  typedef typename container_type::reference               reference;
  typedef typename container_type::const_reference         const_reference;
  typedef typename container_type::const_iterator          const_iterator;
  typedef typename container_type::const_iterator          iterator;
  typedef typename container_type::size_type               size_type;
  typedef typename container_type::difference_type         difference_type;
  typedef const T*                                         const_value;

// STL-level access
  const container_type&  std() const { return impl_; }
  container_type&        std()       { return impl_; }

#ifndef RW_BROKEN_ACCESS_ADJUST
  typedef base1_type                                       base2_type;
  typedef base1_type                                       base3_type;
  typedef base1_type                                       base4_type;
#else
    // for inherited interface components cannot always follow base_type
  typedef RW_PSeq<container_type,this_type,T>              base2_type;
  typedef RW_PCntnr<container_type,this_type,T>            base3_type;
  typedef RW_PBase<container_type,this_type>               base4_type;
#endif
  
  base2_type::clear;    
  base2_type::clearAndDestroy;                      
  base1_type::contains;
  base4_type::entries;
  base1_type::find;
  base4_type::first;
  base1_type::index;
  base1_type::insert;
  base4_type::isEmpty;
  base1_type::isSorted;
  base4_type::npos;
  base1_type::occurrencesOf;
  base3_type::operator<;
  base3_type::operator==;
  base4_type::operator[];
  base4_type::operator();
  base1_type::remove;
  base1_type::removeAll;
  base2_type::removeAt;
  base2_type::removeFirst;
  /*
   * Be careful with these, misuse can lead to the collection
   * becoming out of sorts, so to speak.  (We need to allow the
   * index ops for source compatibility with Tools 6.)
   */
  base3_type::apply;
  base4_type::at;
  base4_type::begin;
  base4_type::end;

// additional interface components
  T*const*      data() const { return &(*(this->begin())); }

  T*&           last()       { return impl_.back(); }   
  T*const&      last() const { return impl_.back(); }   


  size_type         merge(const this_type& coll)
    {
      size_type ret = coll.entries();
      if (ret) {
        size_type n = this->entries() + ret;
        container_type v(n);
        ::merge(impl_.begin(), impl_.end(),
                coll.impl_.begin(), coll.impl_.end(), v.begin(),
                container_comp());
        impl_ = v;
      }
      return ret;
    }

  T*                removeLast()
    {  T* ret = (T*) last(); impl_.pop_back(); return ret; }

  size_type         length() const       { return std().size(); }
  void              resize(size_type n)  { impl_.reserve(n); }

// constructors
  RWTPtrSortedVector() { }
  RWTPtrSortedVector(const this_type& t) : impl_(t.impl_) { }
  RWTPtrSortedVector(const container_type& t) {
    resize(t.size()); // purely for efficiency
    insert(t);
  }
  RWTPtrSortedVector(size_type n, T* val) : impl_(n,val) { }

  RWTPtrSortedVector(T*const* first, T*const* last);

  // For Tools-6 compatibility (not particularly efficient):
  RWTPtrSortedVector(size_type n) { resize(n); }

// ~RWTPtrSortedVector(); // handled by auto-generated destructor

private:
  container_type impl_;
  friend class RW_PBase<container_type, this_type>;
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

RWDECLARE_PERSISTABLE_TEMPLATE_2(RWTPtrSortedVector)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tpsrtvec.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */

#endif /* __RWTPSRTVEC_H__ */
