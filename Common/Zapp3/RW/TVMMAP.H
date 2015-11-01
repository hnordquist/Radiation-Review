#ifndef __RWTVMMAP_H__
#define __RWTVMMAP_H__

/***************************************************************************
 *
 * tvmmap.h - RWTValMultiMap<Key,T,Compare>
 *   
 *     : value-based key/data dictionary wrapper for STL mulitmap
 * 
 * $Id: tvmmap.h@#/main/16  10/10/96 17:34:07  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tvmmap.h,v $
 * Revision 7.17  1996/10/10 17:34:07  griswolf
 * Typo.
 *
 * Revision 7.16  1996/10/09 22:44:01  griswolf
 * Scopus #4893: provide ctor(T*,T*) even if impl does not.
 *
 * Revision 7.15  1996/09/19 18:26:11  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.14  1996/08/23 18:30:02  hart
 * removed work around for HP aCC compiler not inheriting operators from base
 *
 * Revision 7.13  1996/08/09 19:40:23  hart
 * HP (aCC compiler) port
 *
 * Revision 7.12  1996/05/07 19:05:25  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.11  1996/02/19 12:34:12  jims
 * Port to Stdlib 1.2
 *
 * Revision 7.10  1996/02/18 01:49:18  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.9  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.8  1996/01/13 21:35:53  kevinj
 * Simple External Template Persistence (ETP).
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
 * Revision 7.12  1995/09/26  17:14:13  griswolf
 * Add insert and apply as synonyms ...KeyAndValue methods.
 *
 * Revision 7.11  1995/09/05  21:55:47  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.10  1995/08/18  22:30:25  kevinj
 * Added RW_DEFINE_PERSISTABLE_COLLECTION.
 *
 * Revision 7.9  1995/08/15  20:26:54  kevinj
 * Added restoreFrom, saveOn.
 *
 * Revision 7.8  1995/08/09  00:21:08  jims
 * multimap now in map header file
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

#include <map>
#include "rw/ev_inmap.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class K, class T, class C>
#ifndef RW_ALLOCATOR_PLAIN
class RWTExport RWTValMultiMap :
     private RW_VMapAssoc<multimap<K,T,C,allocator<T> >,RWTValMultiMap<K,T,C>,K,T>
#else
class RWTExport RWTValMultiMap :
     private RW_VMapAssoc<multimap<K,T,C,allocator>,RWTValMultiMap<K,T,C>,K,T>
#endif
{
public:
// typedefs
#ifndef RW_ALLOCATOR_PLAIN
  typedef multimap<K,T,C,allocator<T> >              container_type;
#else
  typedef multimap<K,T,C,allocator>                  container_type;
#endif
  typedef RWTValMultiMap<K,T,C>                      this_type;
  typedef RW_VMapAssoc<container_type,this_type,K,T> base_type;
  typedef typename container_type::iterator          iterator;
  typedef typename container_type::const_iterator    const_iterator;
  typedef typename container_type::size_type         size_type;
  typedef typename container_type::value_type        value_type;
  typedef typename container_type::key_type          key_type;
  typedef T                                          data_type;
  typedef typename container_type::reference         reference;
  typedef typename container_type::const_reference   const_reference;

// STL-level access
  container_type&  std()             { return impl_; }
  const container_type&  std() const { return impl_; }

// inherited interface components
  base_type::apply;
  base_type::applyToKeyAndValue; // deprecated
  base_type::begin;
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
  bool       insert(const K& key, const T& val)
  {
    std().insert(value_type(key,val));
    return true; // fails only via exception
  }

  bool       insertKeyAndValue(const K& key, const T& val) // deprecated
  { return insert(key,val);  }

// assignment operators
  RWTValMultiMap<K,T,C>&   operator=(const RWTValMultiMap<K,T,C>& rwmultimap)
    { impl_ = rwmultimap.impl_; return *this; }
  RWTValMultiMap<K,T,C>&   operator=(const container_type& stdmultimap)
    { impl_ = stdmultimap; return *this; }
  
// constructors
  RWTValMultiMap(const C& cmp = C()) : impl_(cmp) { }
  RWTValMultiMap(const container_type& s) : impl_(s) { }
  RWTValMultiMap(const RWTValMultiMap<K,T,C>& rws) : impl_(rws.impl_) { }
  RWTValMultiMap(const value_type* first, const value_type* last,
             const C& cmp = C())
#ifdef RW_STLPTR_CTR_HACK
    : impl_(first,last,cmp) { }
#else
    : impl_(cmp)
    { while(first != last) { impl_.insert(*first); ++first; } }
#endif

private:
  container_type impl_;
  friend class RW_VBase<container_type,this_type>;
};


RWDECLARE_PERSISTABLE_TEMPLATE_3(RWTValMultiMap)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tvmmap.cc"
#endif


/****************************************************************
 *                                                              *
 *      Declarations for RWTValMultiMapIterator<K,T,C>          *
 *                                                              *
 ****************************************************************/

template <class K, class T, class C>
class RWTExport RWTValMultiMapIterator
{
public:

  RWTValMultiMapIterator(RWTValMultiMap<K,T,C>& m)
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

  RWBoolean     operator()()  { return ++(*this); }

  RWTValMultiMap<K,T,C>* container() const      { return rwmap_; }

  K             key() const   { return (*iter_).first; }

  void          reset()       { head_ = true; tail_ = false; } 
  void          reset(RWTValMultiMap<K,T,C>& m)
                              { head_ = true; tail_ = false; rwmap_ = &m; } 

  T             value() const { return (*iter_).second; }

private:
  RWTValMultiMap<K,T,C>*                       rwmap_;
  typename RWTValMultiMap<K,T,C>::iterator     iter_;
  bool                                         head_;
  bool                                         tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWTVMMAP_H__ */
