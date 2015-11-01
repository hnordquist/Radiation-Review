#ifndef __RWTPORDVEC_H__
#define __RWTPORDVEC_H__

/***************************************************************************
 *
 * tpordvec.h - RWTPtrOrderedVector<T>
 *   
 *     : pointer-based ordered vector wrapper for STL vector
 * 
 * $Id: tpordvec.h@#/main/41  09/04/97 17:25:44  griswolf (TOOLS707_WIN32_971021)
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
#  include "rw/xpordvec.h"
#else
// STL-based implementation:

#ifndef RW_HEADER_WITH_EXTENSION
# include <vector>
#else
# include <vector.hpp>
#endif  /* RW_HEADER_WITH_EXTENSION */

#include "rw/ep_seq.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T>
class RWTExport RWTPtrOrderedVector :
#ifndef RW_ALLOCATOR_PLAIN
             private RW_PSeq<vector<T*,allocator<T*> >, RWTPtrOrderedVector<T>, T>
#else
             private RW_PSeq<vector<T*,allocator>, RWTPtrOrderedVector<T>, T>
#endif   
{
public:
// typedefs
#ifndef RW_ALLOCATOR_PLAIN
  typedef vector<T*,allocator<T*> >                  container_type;
#else
  typedef vector<T*,allocator>                       container_type;
#endif
  typedef RWTPtrOrderedVector<T>                     this_type;
  typedef RW_PSeq<container_type,
                  RWTPtrOrderedVector<T>, T>         base1_type;

  typedef typename container_type::iterator          iterator;
  typedef typename container_type::const_iterator    const_iterator;
  typedef typename container_type::size_type         size_type;
  typedef typename container_type::difference_type   difference_type;
  typedef typename container_type::value_type        value_type;
  typedef typename container_type::reference         reference;
  typedef typename container_type::const_reference   const_reference;

// STL-level access
  const container_type&  std() const { return impl_; }
  container_type&        std()       { return impl_; }

#ifndef RW_BROKEN_ACCESS_ADJUST
  typedef base1_type                                  base2_type;
  typedef base1_type                                  base3_type;
#else
  //  inherited interface components cannot always follow base_type
  typedef RW_PCntnr<container_type,this_type,T>       base2_type;
  typedef RW_PBase<container_type,this_type>          base3_type;
#endif  
  
  base1_type::append;
  base2_type::apply;
  base3_type::at;
  base3_type::begin;
  base1_type::clear;
  base1_type::clearAndDestroy;
  base2_type::contains;
  base3_type::end;
  base3_type::entries;
  base2_type::find;
  base3_type::first;
  base2_type::index;
  base1_type::insert;
  base1_type::insertAt;
  base3_type::isEmpty;
  base1_type::minElement;
  base1_type::maxElement;
  base3_type::npos;
  base2_type::occurrencesOf;
  base3_type::operator();
  base2_type::operator<;
  base2_type::operator==;
  base3_type::operator[];
  base1_type::prepend;
  base1_type::remove;
  base1_type::removeAll;
  base1_type::removeAt;
  base1_type::removeFirst;
  base1_type::replaceAll;

// additional interface components
  T* const*     data() const       { return &(T* const &)(*(this->begin())); }
  T*&           last()             { return impl_.back(); }     
  T*const&      last() const       { return impl_.back(); }     
  size_type     length() const     { return impl_.size(); }
  void          resize(size_t n)   { impl_.reserve(n); }
  T*            removeLast()       { T* ret = last();
                                     impl_.pop_back();
                                     return ret;
                                    }
  void          sort()
    { ::sort(this->begin(),this->end(),rw_deref_compare<less<T>,T>()); }

// assignment operators
  RWTPtrOrderedVector<T>&   operator=(const RWTPtrOrderedVector<T>& ovec)
    { impl_ = ovec.impl_; return *this; }
  RWTPtrOrderedVector<T>&   operator=(const container_type& stdv)
    { impl_ = stdv; return *this; }

// constructors
  RWTPtrOrderedVector()                                : impl_() {}
  RWTPtrOrderedVector(const container_type& lst)       : impl_(lst) {}
  RWTPtrOrderedVector(const this_type& lst)            : impl_(lst.impl_) {}
  RWTPtrOrderedVector(size_type n, T* p)               : impl_(n, p) {}
  RWTPtrOrderedVector(T*const* first, T*const* last)   
#ifdef RW_STLCTOR_PTR_HACK
    : impl_(first,last) {}
#else
   { while(first != last) {impl_.push_back(*first); ++first; } }
#endif
  // For Tools-6 compatibility (not particularly efficient):
  RWTPtrOrderedVector(size_type n) { resize(n); }

//  ~RWTPtrOrderedVector(); // handled by compiler-generated destructor

private:
  container_type impl_;         

  friend class RW_PBase<container_type, this_type>;
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

RWDECLARE_PERSISTABLE_TEMPLATE(RWTPtrOrderedVector)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tpordvec.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */

#endif /* __RWTPORDVEC_H__ */
