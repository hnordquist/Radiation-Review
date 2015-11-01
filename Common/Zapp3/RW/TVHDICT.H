#ifndef __RWTVHMAP_H__
#define __RWTVHMAP_H__

/***************************************************************************
 *
 * tvhdict.h - RWTValHashMap<Key,T,H,EQ>
 *   
 *     : value-based key/data dictionary wrapper for STL-like rw_hashmap
 * 
 * $Id: tvhdict.h@#/main/40  09/29/96 19:48:51  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tvhdict.h,v $
 * Revision 7.24  1996/09/29 19:48:51  griswolf
 * Fix typo
 *
 * Revision 7.23  1996/08/23 18:30:02  hart
 * remove temp fix for HP aCC compiler not nheriting all operators from base
 *
 * Revision 7.22  1996/08/16 23:44:53  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.21  1996/08/09 19:36:27  hart
 * HP (aCC compiler) port
 *
 * Revision 7.20  1996/07/18 04:13:33  hart
 * ObjectStore changes
 *
 * Revision 7.19  1996/05/07 19:25:24  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.18  1996/04/29 19:12:43  groening
 * needed to include compiler.h prior to making any references to
 * RW_NO_STL.
 *
 * Revision 7.17  96/02/19  12:34:12  12:34:12  jims (Jim Shur)
 * Port to Stdlib 1.2
 * 
 * Revision 7.16  1996/02/18 01:49:02  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.15  1996/02/16 21:28:58  kyle
 * fixed base_type
 *
 * Revision 7.14  1996/02/16 21:04:10  kyle
 * Millenium adds support for compilers where interface components cannot
 * always follow base_type
 *
 * Revision 7.13  1996/02/07 02:42:30  jims
 * Standard library extensions now in directory rw/stdex
 *
 * Revision 7.12  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.11  1996/01/16 18:26:35  kevinj
 * Simple ETP.
 *
 * Revision 7.10  1996/01/15 22:08:08  kevinj
 * OTIS0017 patch.
 *
 * Revision 7.9  1995/12/20 23:19:20  kevinj
 * External template persistence using RW*Persistor.
 *
 * Revision 7.8  1995/12/14 18:01:21  jims
 * saveOn, restoreFrom now global instead of member functions
 *
 * Revision 7.7  1995/12/13 00:22:58  jims
 * Remove member functions saveOn and restoreFrom (now global)
 *
 * Revision 7.6  1995/12/12 00:03:06  jims
 * Add RWTExport
 *
 * Revision 7.5  1995/12/11 22:30:41  jims
 * Remove 'Sca' from interior class names
 * Add 'rw_' to global helper classes
 * Change 'muterator' to 'filterator'
 *
 * Revision 7.4  1995/11/17 08:15:43  jims
 * Merge otis-et and otis-stl templates
 *
 * Revision 7.5  1995/09/26 17:14:13  griswolf
 * Add insert and apply as synonyms ...KeyAndValue methods.
 *
 * Revision 7.4  1995/09/18  18:43:31  griswolf
 * Add capacity, fillRatio and resize methods.
 *
 * Revision 7.3  1995/09/12  19:41:49  griswolf
 * Change to #include renamed rw/rwstl/ header file.
 *
 * Revision 7.2  1995/09/05  21:53:59  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.1  1995/09/05  21:47:43  jims
 * Move to revision 7.1
 *
 * Revision 1.1  1995/09/05  21:47:14  jims
 * Initial revision
 * 
 ****************************************************************************/

#include "rw/compiler.h"

#ifdef RW_NO_STL

#include "rw/xvhdict.h"
#define RWDefHArgs(T)

#else

#ifndef RWDefHArgs
#  define RWDefHArgs(T) ,RWTHasher<T>,equal_to<T>
#endif

// STL-based implementation:

#include "rw/stdex/hashmap.h"
#include "rw/ev_inmap.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class K, class T, class H, class EQ>
#ifndef RW_ALLOCATOR_PLAIN
class RWTExport RWTValHashMap : private
   RW_VMapAssoc<rw_hashmap<K,T,H,EQ,allocator<K> >,RWTValHashMap<K,T,H,EQ>,K,T>
#else
class RWTExport RWTValHashMap : private
   RW_VMapAssoc<rw_hashmap<K,T,H,EQ>,RWTValHashMap<K,T,H,EQ>,K,T>
#endif
{
public:
// typedefs
#ifndef RW_ALLOCATOR_PLAIN
  typedef rw_hashmap<K,T,H,EQ,allocator<K> >         container_type;
#else
  typedef rw_hashmap<K,T,H,EQ>                       container_type;
#endif
  typedef RWTValHashMap<K,T,H,EQ>                    this_type;
  typedef RW_VMapAssoc<container_type,this_type,K,T> base1_type;
  
# ifndef RW_BROKEN_ACCESS_ADJUST
  typedef base1_type                                 base2_type;
  typedef base1_type                                 base3_type;
# else  
  typedef RW_VMapCntnr<container_type,this_type,K,T> base2_type;
  typedef RW_VBase<container_type,this_type>         base3_type;
# endif  /* RW_BROKEN_ACCESS_ADJUST */
  
  typedef typename container_type::iterator          iterator;
  typedef typename container_type::const_iterator    const_iterator;
  typedef typename container_type::size_type         size_type;
  typedef typename container_type::value_type        value_type;
  typedef typename container_type::key_type          key_type;
  typedef T                                          data_type;
  typedef typename container_type::reference         reference;
  typedef typename container_type::const_reference   const_reference;
  typedef pair<K,T>                                  hit_type;

// STL-level access
  container_type&  std()             { return impl_; }
  const container_type&  std() const { return impl_; }

// inherited interface components
  base1_type::apply;
  base1_type::applyToKeyAndValue; // deprecated
  base1_type::begin;
  base1_type::end;
  base3_type::isEmpty;
  base3_type::entries;
  base1_type::operator==;
  base1_type::clear;
  base1_type::contains;
  base2_type::find;
  base1_type::findValue;
  base1_type::findKeyAndValue;
  base1_type::occurrencesOf;
  base1_type::remove;
  base1_type::removeAll;

// additional interface components
  bool insert(const K& key, const T& val)
    { return (std().insert(value_type(key,val))).second; }

  bool insertKeyAndValue(const K& key, const T& val) // deprecated
    { return insert(key,val); }

  T& operator[](const K& x)
    { return impl_[x]; }

  size_type
    capacity() const { return impl_.capacity(); }

  float
    fillRatio() const { return impl_.fill_ratio(); }

  void
    resize(size_type newsz) { impl_.resize(newsz); }

// assignment operators
  RWTValHashMap<K,T,H,EQ>&   operator=(const RWTValHashMap<K,T,H,EQ>& rwmap)
    { impl_ = rwmap.impl_; return *this; }
#ifndef RW_ALLOCATOR_PLAIN
  RWTValHashMap<K,T,H,EQ>&   operator=(const rw_hashmap<K,T,H,EQ,allocator<K> >& stdmap)
    { impl_ = stdmap; return *this; }
#else
  RWTValHashMap<K,T,H,EQ>&   operator=(const rw_hashmap<K,T,H,EQ>& stdmap)
    { impl_ = stdmap; return *this; }
#endif
  
// constructors
  RWTValHashMap() { }
#ifndef RW_ALLOCATOR_PLAIN
  RWTValHashMap(const rw_hashmap<K,T,H,EQ,allocator<K> >& s) : impl_(s) { }
#else
  RWTValHashMap(const rw_hashmap<K,T,H,EQ>& s) : impl_(s) { }
#endif
  RWTValHashMap(const RWTValHashMap<K,T,H,EQ>& rws) : impl_(rws.impl_) { }
  RWTValHashMap(const value_type* first, const value_type* last)
    : impl_(first,last) { }

  // Tools-6 compatible constructor:
  RWTValHashMap(const H& h, size_type sz = RWDEFAULT_CAPACITY)
    : impl_(sz,h,EQ()) { }

private:
#ifndef RW_ALLOCATOR_PLAIN
  rw_hashmap<K,T,H,EQ,allocator<K> > impl_;
#else
  rw_hashmap<K,T,H,EQ>               impl_;
#endif
  friend class RW_VBase<container_type,this_type>;
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

RWDECLARE_PERSISTABLE_TEMPLATE_4(RWTValHashMap)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tvhdict.cc"
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWTValHashMapIterator<K,V>     *
 *                                                              *
 ****************************************************************/

template <class K, class T, class H, class EQ>
class RWTExport RWTValHashMapIterator
{
public:

  RWTValHashMapIterator(RWTValHashMap<K,T,H,EQ>& m)
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

  RWTValHashMap<K,T,H,EQ>* container() const    { return rwmap_; }

  K             key() const   { return (*iter_).first; }

  void          reset()       { head_ = true; tail_ = false; } 
  void          reset(RWTValHashMap<K,T,H,EQ>& m)
                              { head_ = true; tail_ = false; rwmap_ = &m; } 

  T             value() const { return (*iter_).second; }


private:
  RWTValHashMap<K,T,H,EQ>*                       rwmap_;
  typename RWTValHashMap<K,T,H,EQ>::iterator     iter_;
  bool                                           head_;
  bool                                           tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#define RWTValHashDictionary         RWTValHashMap
#define RWTValHashDictionaryIterator RWTValHashMapIterator

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */

#endif /* __RWTVHMAP_H__ */
