#ifndef __RWTVMAP_H__
#define __RWTVMAP_H__

/***************************************************************************
 *
 * tvmap.h - RWTValMap<Key,T,Compare>
 *   
 *     : value-based key/data dictionary wrapper for STL map
 * 
 * $Id: tvmap.h@#/main/18  10/10/96 17:32:33  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tvmap.h,v $
 * Revision 7.19  1996/10/10 17:32:33  griswolf
 * Typo
 *
 * Revision 7.18  1996/10/09 22:43:01  griswolf
 * Oops: provide impl_ initializer.
 *
 * Revision 7.17  1996/10/09 22:11:54  griswolf
 * Scopus #4893: provide ctor(T*,T*) even if impl does
 *
 * Revision 7.16  1996/09/19 18:26:05  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.15  1996/08/23 18:30:02  hart
 * removed work around for HP aCC compiler not inheriting operators from base
 *
 * Revision 7.14  1996/08/09 19:38:17  hart
 * HP (aCC compiler) port
 *
 * Revision 7.13  1996/05/07 19:26:22  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.12  1996/04/29 19:12:43  groening
 * needed to include compiler.h prior to making any references to
 * RW_NO_STL.
 *
 * Revision 7.11  96/02/19  12:34:12  12:34:12  jims (Jim Shur)
 * Port to Stdlib 1.2
 * 
 * Revision 7.10  1996/02/18 01:49:16  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.9  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.8  1996/01/13 01:43:44  kevinj
 * Simplified External Template Persistence (ETP).
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
 * Revision 7.13  1995/09/26  17:14:13  griswolf
 * Add insert and apply as synonyms ...KeyAndValue methods.
 *
 * Revision 7.12  1995/09/05  21:55:15  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.11  1995/08/18  22:22:45  kevinj
 * Added RW_DEFINE_PERSISTABLE_COLLECTION
 *
 * Revision 7.10  1995/08/15  19:59:00  kevinj
 * *** empty log message ***
 *
 * Revision 7.9  1995/08/11  17:03:34  kevinj
 * Template persistence 2.2.
 *
 * Revision 7.9  1995/08/11  17:03:34  kevinj
 * Template persistence 2.2.
 *
 * Revision 7.8  1995/08/02  00:04:36  jims
 * Qualify include file names with "rw/"
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
#include "rw/ev_inmap.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class K, class T, class C>
#ifndef RW_ALLOCATOR_PLAIN
class RWTExport RWTValMap :
     private RW_VMapAssoc<map<K,T,C,allocator<T> >,RWTValMap<K,T,C>,K,T>
#else
class RWTExport RWTValMap :
     private RW_VMapAssoc<map<K,T,C,allocator>,RWTValMap<K,T,C>,K,T>
#endif
{
public:
// typedefs
#ifndef RW_ALLOCATOR_PLAIN
  typedef map<K,T,C,allocator<T> >                      container_type;
#else
  typedef map<K,T,C,allocator>                          container_type;
#endif
  typedef RWTValMap<K,T,C>                              this_type;
  typedef RW_VMapAssoc<container_type,this_type,K,T>    base_type;
  typedef typename container_type::iterator             iterator;
  typedef typename container_type::const_iterator       const_iterator;
  typedef typename container_type::size_type            size_type;
  typedef typename container_type::value_type           value_type;
  typedef typename container_type::key_type             key_type;
  typedef T                                             data_type;
  typedef typename container_type::reference            reference;
  typedef typename container_type::const_reference      const_reference;
  typedef pair<K,T>                                     hit_type;

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
  bool insert(const K& key, const T& val)
    { return (std().insert(value_type(key,val))).second; }

  bool insertKeyAndValue(const K& key, const T& val) // deprecated
    { return insert(key,val); }

  T& operator[](const K& x)
    { return impl_[x]; }

// assignment operators
  RWTValMap<K,T,C>&   operator=(const RWTValMap<K,T,C>& rwmap)
    { impl_ = rwmap.impl_; return *this; }
  RWTValMap<K,T,C>&   operator=(const container_type& stdmap)
    { impl_ = stdmap; return *this; }
  
// constructors
  RWTValMap(const C& cmp = C()) : impl_(cmp) { }
  RWTValMap(const container_type& s) : impl_(s) { }
  RWTValMap(const RWTValMap<K,T,C>& rws) : impl_(rws.impl_) { }
  RWTValMap(const value_type* first, const value_type* last,
             const C& cmp = C())
#ifdef RW_STLCTOR_PTR_HACK
    : impl_(first,last,cmp) { }
#else
    : impl_(cmp)
   { while(first != last) {impl_.insert(*first); ++first; } }
#endif

private:
  container_type impl_;
  friend class RW_VBase<container_type,this_type>;
};

RWDECLARE_PERSISTABLE_TEMPLATE_3(RWTValMap)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tvmap.cc"
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWTValMapIterator<K,T,C>               *
 *                                                              *
 ****************************************************************/

template <class K, class T, class C>
class RWTExport RWTValMapIterator
{
public:

  RWTValMapIterator(RWTValMap<K,T,C>& m)
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

  RWTValMap<K,T,C>* container() const   { return rwmap_; }

  K             key() const   { return (*iter_).first; }

  void          reset()       { head_ = true; tail_ = false; } 
  void          reset(RWTValMap<K,T,C>& m)
                              { head_ = true; tail_ = false; rwmap_ = &m; } 

  T             value() const { return (*iter_).second; }

private:
  RWTValMap<K,T,C>*                       rwmap_;
  typename RWTValMap<K,T,C>::iterator     iter_;
  bool                                    head_;
  bool                                    tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWTVMAP_H__ */
