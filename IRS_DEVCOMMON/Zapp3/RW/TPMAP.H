#ifndef __RWTPMAP_H__
#define __RWTPMAP_H__

/***************************************************************************
 *
 * tpmap.h - RWTPtrMap<Key,T,Compare>
 *   
 *     : pointer-based key/data dictionary wrapper for STL map
 * 
 * $Id: tpmap.h@#/main/20  10/09/96 22:03:57  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tpmap.h,v $
 * Revision 7.21  1996/10/09 22:03:57  griswolf
 * Oops: provide impl_ initializer.
 *
 * Revision 7.20  1996/10/09 21:50:25  griswolf
 * Scopus #4893: provide ctor(T*,T*) even if impl does not.
 *
 * Revision 7.19  1996/09/19 18:25:38  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.18  1996/08/23 18:30:02  hart
 * removed work around for HP aCC compiler not inheriting operators from base
 *
 * Revision 7.17  1996/08/09 19:23:39  hart
 * HP (aCC compiler) port
 *
 * Revision 7.16  96/05/07  19:56:32  19:56:32  kyle (Bruse Kyle)
 * Added export pragma for Mac code fragments
 * 
 * Revision 7.15  1996/02/19 12:34:12  jims
 * Port to Stdlib 1.2
 *
 * Revision 7.14  1996/02/18 01:47:42  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.13  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.12  1996/01/24 21:07:41  jims
 * Simplify implementation of otis-stl ptr-based collections
 * by using container<T*> impls instead of container<void*>
 *
 * Revision 7.11  1996/01/13 01:43:44  kevinj
 * Simplified External Template Persistence (ETP).
 *
 * Revision 7.10  1995/12/21 18:53:32  pearson
 * Fix typo
 *
 * Revision 7.9  1995/12/21 18:32:08  pearson
 * Borland C++ 4.5 won't allow the first member of pair to be const, use
 * RWCONST instead
 *
 * Revision 7.8  1995/12/20 23:19:20  kevinj
 * External template persistence using RW*Persistor.
 *
 * Revision 7.7  1995/12/15 10:16:39  jims
 * Add typedefs for value_type_key and value_type_data
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
 * Revision 7.15  1995/10/20  06:46:20  jims
 * Port to msvc 4.0: don't use access adjustment for typedefs
 *
 * Revision 7.14  1995/09/26  17:14:13  griswolf
 * Add insert and apply as synonyms ...KeyAndValue methods.
 *
 * Revision 7.13  1995/09/22  16:34:41  griswolf
 * Fix bug in two-pointer constructor.
 *
 * Revision 7.12  1995/09/05  21:51:25  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.11  1995/08/18  22:12:54  kevinj
 * Added RW_DEFINE_PERSISTABLE_COLLECTION.
 *
 * Revision 7.10  1995/08/14  06:12:17  jims
 * change RW_NO_CONST_INST to RWSTD_NO_CONST_INST
 *
 * Revision 7.9  1995/08/12  02:15:54  jims
 * Port to SL1007 / bcc45
 *
 * Revision 7.8  1995/08/11  17:03:34  kevinj
 * Template persistence 2.2.
 *
 * Revision 7.7  1995/07/27  22:02:33  jims
 * Add header with file name, short description, $Id and $Log
 * 
 ****************************************************************************/

#include "rw/compiler.h"

#ifdef RW_NO_STL

#error Cannot include header if RW_NO_STL macro is defined for your compiler

#endif

#include <map>
#include "rw/ep_inmap.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class K, class T, class C>
class RWTExport RWTPtrMap 
#ifndef RW_ALLOCATOR_PLAIN
  : private RW_PMapAssoc<map<K*, T*,rw_deref_compare<C,K>,allocator<T*> >,
                         RWTPtrMap<K,T,C>, K, T>
#else
  : private RW_PMapAssoc<map<K*, T*,rw_deref_compare<C,K>,allocator>,
                         RWTPtrMap<K,T,C>, K, T>
#endif
{
public:
// typedefs
  typedef rw_deref_compare<C,K>                         container_comp;
#ifndef RW_ALLOCATOR_PLAIN
  typedef map<K*,T*,container_comp,allocator<T*> >      container_type;
#else
  typedef map<K*,T*,container_comp,allocator>           container_type;
#endif
  typedef RWTPtrMap<K,T,C>                              this_type;
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
 
// inherited interface components cannot always follow base_type
  base_type::apply;
  base_type::applyToKeyAndValue;
  base_type::begin;
  base_type::clearAndDestroy;
  base_type::end;
  base_type::isEmpty;
  base_type::entries;
  base_type::operator==;
  base_type::operator<;
  base_type::clear;
  base_type::contains;
  base_type::find;
  base_type::findValue;
  base_type::findKeyAndValue;
  base_type::occurrencesOf;
  base_type::remove;
  base_type::removeAll;


// additional interface components
  bool insert(K* key, T* partner)
    { return (impl_.insert(value_type (key,partner))).second; }

  bool insertKeyAndValue(K* key, T* partner) // deprecated
  { return insert(key,partner); }

  T*& operator[](const K* x)
    { return (T*&) impl_[(K*)x]; }

// assignment operators
  RWTPtrMap<K,T,C>&   operator=(const this_type& rwmap)
    { impl_ = rwmap.impl_; return *this; }
  RWTPtrMap<K,T,C>&   operator=(const container_type& stdmap)
    { impl_ = stdmap; return *this; }
  
// constructors
  RWTPtrMap(const container_comp& cmp = container_comp())  : impl_(cmp) { }
  RWTPtrMap(const container_type& s)               : impl_(s) { }
  RWTPtrMap(const this_type& rws)                  : impl_(rws.impl_) { }
  RWTPtrMap(value_type* first, value_type* last,
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

RWDECLARE_PERSISTABLE_TEMPLATE_3(RWTPtrMap)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tpmap.cc"
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrMapIterator<K,V>                 *
 *                                                              *
 ****************************************************************/

template <class K, class T, class C>
class RWTExport RWTPtrMapIterator
{
public:

  RWTPtrMapIterator(RWTPtrMap<K,T,C>& m)
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

  RWTPtrMap<K,T,C>* container() const   { return rwmap_; }

  K*    key() const   { return (*iter_).first; }

  void  reset()       { head_ = true; tail_ = false; } 
  void  reset(RWTPtrMap<K,T,C>& m)
                              { head_ = true; tail_ = false; rwmap_ = &m; } 

  T*    value() const { return (*iter_).second; }

private:
  RWTPtrMap<K,T,C>*                        rwmap_;
  typename RWTPtrMap<K,T,C>::iterator      iter_;
  bool                                     head_;
  bool                                     tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWTPMAP_H__ */
