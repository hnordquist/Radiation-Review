#ifndef __RWTPHMMAP_H__
#define __RWTPHMMAP_H__

/***************************************************************************
 *
 * tphmmap.h - RWTPtrHashMultiMap<Key,T,H,EQ>
 *   
 *  : pointer-based key/data dictionary wrapper for STL-like rw_hashmulitmap
 * 
 * $Id: tphmmap.h@#/main/17  09/16/97 12:46:21  griswolf (TOOLS707_WIN32_971021)
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
#error Cannot include header if RW_NO_STL macro is defined for your compiler
#endif

#ifndef RWDefHArgs
#  define RWDefHArgs(T) ,RWTHasher<T>,equal_to<T>
#endif

#include "rw/stdex/hashmmap.h"
#include "rw/ep_inmap.h"

template <class K, class T, class H, class EQ>
class RWTPtrHashMultiMap
#ifndef RW_ALLOCATOR_PLAIN
  : private RW_PMapAssoc<rw_hashmultimap<K*,T*,rw_deref_hash<H,K>,
                            rw_deref_compare<EQ,K>,allocator<K*> >,
                            RWTPtrHashMultiMap<K,T,H,EQ>, K, T>
#else
  : private RW_PMapAssoc<rw_hashmultimap<K*,T*,rw_deref_hash<H,K>,
                            rw_deref_compare<EQ,K> >,
                            RWTPtrHashMultiMap<K,T,H,EQ>, K, T>
#endif
{
public:
// typedefs
  typedef rw_deref_hash<H,K>                            container_hash;
  typedef rw_deref_compare<EQ,K>                        container_eq; 
#ifndef RW_ALLOCATOR_PLAIN
  typedef rw_hashmultimap<K*,T*,rw_deref_hash<H,K>,
                 rw_deref_compare<EQ,K>, allocator<K*> > container_type;
#else
  typedef rw_hashmultimap<K*,T*,rw_deref_hash<H,K>,
                     rw_deref_compare<EQ,K> >           container_type;
#endif
  typedef RWTPtrHashMultiMap<K,T,H,EQ>                  this_type;
  typedef RW_PMapAssoc<container_type, this_type, K, T> base_type;

  typedef typename container_type::size_type            size_type;
  typedef typename container_type::difference_type      difference_type;

  typedef typename container_type::iterator             iterator;
  typedef typename container_type::const_iterator       const_iterator;
  typedef typename container_type::value_type           value_type;
  typedef typename container_type::reference            reference;
  typedef typename container_type::const_reference      const_reference;

  typedef K*                                            value_type_key;
  typedef T*                                            value_type_data;
  typedef K*&                                           reference_key;
  typedef T*&                                           reference_data;
  typedef const K*const&                                const_reference_key;
  typedef const T*const&                                const_reference_data;

// STL-level access
  const container_type&  std() const { return impl_; }
  container_type& std() { return impl_; }
  
// inherited interface components
  base_type::apply;
  base_type::applyToKeyAndValue; // deprecated
  base_type::begin;
  base_type::clearAndDestroy;
  base_type::end;
  base_type::isEmpty;
  base_type::entries;
  base_type::clear;
  base_type::contains;
  base_type::find;
  base_type::findValue;
  base_type::findKeyAndValue;
  base_type::occurrencesOf;
  base_type::remove;
  base_type::removeAll;

// overridden interface comoponents
  bool operator==(const this_type& rwmap) const {
    return   this->entries() == rwmap.entries()
          && hashMapCollEqual(*this, rwmap); 
}

// additional interface components
  bool insert(K* key, T* partner)
    { impl_.insert(value_type(key,partner));
      return true; } 

  bool insertKeyAndValue(K* key, T* partner) // deprecated
  { return insert(key,partner); }

  size_type
    capacity() const { return impl_.capacity(); }

  float
    fillRatio() const { return impl_.fill_ratio(); }

  void
    resize(size_type newsz) { impl_.resize(newsz); }

// assignment operators
  RWTPtrHashMultiMap<K,T,H,EQ>&   operator=(const this_type& rwmap)
    { impl_ = rwmap.impl_; return *this; }
  RWTPtrHashMultiMap<K,T,H,EQ>&   operator=(const container_type& stdmap)
    { impl_ = stdmap; return *this; }
  
// constructors
  RWTPtrHashMultiMap() { }
  RWTPtrHashMultiMap(const container_type& s)        : impl_(s) { }
  RWTPtrHashMultiMap(const this_type& rws)           : impl_(rws.impl_) { }
  RWTPtrHashMultiMap(value_type* first, value_type* last)
    : impl_(first, last) { }
  
  // Tools-6 style constructor:
  RWTPtrHashMultiMap(const H& h, size_type sz = RWDEFAULT_CAPACITY)
    : impl_(sz,h,EQ()) { }

private:
  container_type impl_;
  friend class RW_PBase<container_type, this_type>;
};

RWDECLARE_PERSISTABLE_TEMPLATE_4(RWTPtrHashMultiMap)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tphmmap.cc"
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrHashMultiMapIterator<K,V,H,EQ>   *
 *                                                              *
 ****************************************************************/

template <class K, class T, class H, class EQ>
class RWTExport RWTPtrHashMultiMapIterator
{
public:

  RWTPtrHashMultiMapIterator(RWTPtrHashMultiMap<K,T,H,EQ>& m)
          : rwmap_(&m), head_(true), tail_(false) { }

  RWBoolean     operator++()            // Advance and test
  {
    RWASSERT(!(head_ == true  &&  tail_ == true)); 

    if (tail_)
      /* do nothing */ ;

    else if (head_) {
      iter_ = rwmap_->begin();
      head_ = false;
    }
    else
      ++iter_;

    if (!tail_ && iter_ == rwmap_->end())
      tail_ = true;

    RWASSERT(!(head_ == true  &&  tail_ == true)); 

    return !tail_;
  }

  K*    operator()()  { return ++(*this) ? key() : rwnil; }

  RWTPtrHashMultiMap<K,T,H,EQ>* container() const       { return rwmap_; }

  K*    key() const   { return (*iter_).first; }

  void  reset()       { head_ = true; tail_ = false; } 
  void  reset(RWTPtrHashMultiMap<K,T,H,EQ>& m)
                              { head_ = true; tail_ = false; rwmap_ = &m; } 

  T*    value() const { return (*iter_).second; }

private:
  RWTPtrHashMultiMap<K,T,H,EQ>*                       rwmap_;
  typename RWTPtrHashMultiMap<K,T,H,EQ>::iterator     iter_;
  bool                                                head_;
  bool                                                tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
};

#endif /* __RWTPHMMAP_H__ */
