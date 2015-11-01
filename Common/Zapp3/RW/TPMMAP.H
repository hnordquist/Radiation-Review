#ifndef __RWTPMMAP_H__
#define __RWTPMMAP_H__

/***************************************************************************
 *
 * tpmmap.h - RWTPtrMultiMap<Key,T,Compare>
 *   
 *     : pointer-based key/data dictionary wrapper for STL mulitmap
 * 
 * $Id: tpmmap.h@#/main/19  10/09/96 22:03:24  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tpmmap.h,v $
 * Revision 7.20  1996/10/09 22:03:24  griswolf
 * Oops: provide impl_ initializer.
 *
 * Revision 7.19  1996/10/09 21:48:44  griswolf
 * Scopus #4893: provide ctor(T*,T*) even if impl does not.
 *
 * Revision 7.18  1996/09/19 18:25:32  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.17  1996/08/23 18:30:02  hart
 * remove work around for HP aCC compiler not inheriting operators from base
 *
 * Revision 7.16  1996/08/09 19:24:37  hart
 * HP (aCC compiler) port
 *
 * Revision 7.15  1996/05/07 19:55:56  kyle
 * Added export pragma for Mac code fragments
 * Changed RW_NO_STL from #endif to #else, #endif
 *
 * Revision 7.14  1996/04/29 19:12:43  groening
 * needed to include compiler.h prior to making any references to
 * RW_NO_STL.
 *
 * Revision 7.13  96/02/19  12:34:12  12:34:12  jims (Jim Shur)
 * Port to Stdlib 1.2
 * 
 * Revision 7.12  1996/02/18 01:47:43  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.11  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.10  1996/01/24 21:07:41  jims
 * Simplify implementation of otis-stl ptr-based collections
 * by using container<T*> impls instead of container<void*>
 *
 * Revision 7.9  1996/01/13 21:35:53  kevinj
 * Simple External Template Persistence (ETP).
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
 * Revision 7.16  1995/10/20  06:46:20  jims
 * Port to msvc 4.0: don't use access adjustment for typedefs
 *
 * Revision 7.15  1995/09/26  17:14:13  griswolf
 * Add insert and apply as synonyms ...KeyAndValue methods.
 *
 * Revision 7.14  1995/09/22  16:34:41  griswolf
 * Fix bug in two-pointer constructor.
 *
 * Revision 7.13  1995/09/05  21:51:31  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.12  1995/08/18  22:27:02  kevinj
 * Added RW_DEFINE_PERSISTABLE_COLLECTION.
 *
 * Revision 7.11  1995/08/15  20:20:38  kevinj
 * Added restoreFrom, saveOn.
 *
 * Revision 7.10  1995/08/11  22:15:36  jims
 * Port to SL1007 / bcc45
 *
 * Revision 7.9  1995/08/09  00:22:40  jims
 * multimap now in <map> header file
 *
 * Revision 7.8  1995/08/02  00:04:36  jims
 * Qualify include file names with "rw/"
 *
 * Revision 7.7  1995/07/27  22:02:33  jims
 * Add header with file name, short description, $Id and $Log
 * 
 ****************************************************************************/

#ifdef RW_NO_STL
#  error Cannot include header if RW_NO_STL macro is defined for your compiler
#else 

#include <map>
#include "rw/ep_inmap.h"

template <class K, class T, class C>
class RWTPtrMultiMap :
#ifndef RW_ALLOCATOR_PLAIN
     private RW_PMapAssoc<multimap<K*,T*,rw_deref_compare<C,K>,allocator<T*> >,
                          RWTPtrMultiMap<K,T,C>, K, T>
#else
     private RW_PMapAssoc<multimap<K*,T*,rw_deref_compare<C,K>,allocator>,
                          RWTPtrMultiMap<K,T,C>, K, T>
#endif
{
public:
// typedefs
  typedef rw_deref_compare<C,K>                      container_comp;
#ifndef RW_ALLOCATOR_PLAIN
  typedef multimap<K*,T*,container_comp,allocator<T*> > container_type;
#else
  typedef multimap<K*,T*,container_comp,allocator>   container_type;
#endif
  typedef RWTPtrMultiMap<K,T,C>                      this_type;
  typedef RW_PMapAssoc<container_type,this_type,K,T> base_type;
  
  typedef typename container_type::size_type          size_type;
  typedef typename container_type::difference_type    difference_type;

  typedef typename container_type::iterator           iterator;
  typedef typename container_type::const_iterator     const_iterator;
  typedef typename container_type::value_type         value_type;
  typedef typename container_type::reference          reference;
  typedef typename container_type::const_reference    const_reference;

  typedef K*                                          value_type_key;
  typedef T*                                          value_type_data;
  typedef K*&                                         reference_key;
  typedef T*&                                         reference_data;
  typedef const K*const&                              const_reference_key;
  typedef const T*const&                              const_reference_data;

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
    { impl_.insert(value_type(key,partner));
      return true; } 

  bool insertKeyAndValue(K* key, T* partner) // deprecated
  { return insert(key,partner); }

// assignment operators
  RWTPtrMultiMap<K,T,C>&   operator=(const this_type& rwmap)
    { impl_ = rwmap.impl_; return *this; }
  RWTPtrMultiMap<K,T,C>&   operator=(const container_type& stdmap)
    { impl_ = stdmap; return *this; }
  
// constructors
  RWTPtrMultiMap(const container_comp& cmp = container_comp()): impl_(cmp) { }
  RWTPtrMultiMap(const container_type& s)               : impl_(s) { }
  RWTPtrMultiMap(const this_type& rws)                  : impl_(rws.impl_) { }
  RWTPtrMultiMap(value_type* first, value_type* last,
             const container_comp& cmp = container_comp())
#ifdef RW_STLCTOR_PTR_HACK
    : impl_(first,last,cmp) { }
#else
    : impl_(cmp)
  { while(first != last) { impl_.insert(*first); ++first;} }
#endif
  
private:
  container_type impl_;
  friend class RW_PBase<container_type, this_type>;
};

RWDECLARE_PERSISTABLE_TEMPLATE_3(RWTPtrMultiMap)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tpmmap.cc"
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrMultiMapIterator<K,V>            *
 *                                                              *
 ****************************************************************/

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class K, class T, class C>
class RWTExport RWTPtrMultiMapIterator
{
public:

  RWTPtrMultiMapIterator(RWTPtrMultiMap<K,T,C>& m)
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

  RWTPtrMultiMap<K,T,C>* container() const      { return rwmap_; }

  K*    key() const   { return (*iter_).first; }

  void  reset()       { head_ = true; tail_ = false; } 
  void  reset(RWTPtrMultiMap<K,T,C>& m)
                              { head_ = true; tail_ = false; rwmap_ = &m; } 

  T*    value() const { return (*iter_).second; }

private:
  RWTPtrMultiMap<K,T,C>*            rwmap_;
  typename RWTPtrMultiMap<K,T,C>::iterator   iter_;
  bool                              head_;
  bool                              tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */
#endif /* __RWTPMMAP_H__ */
