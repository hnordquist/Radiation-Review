#ifndef __RWTVHMMAP_H__
#define __RWTVHMMAP_H__

/***************************************************************************
 *
 * tvhmmap.h - RWTValHashMultiMap<Key,T,H,EQ>
 *   
 *    : value-based key/data dictionary wrapper for STL-like rw_hashmulitmap
 * 
 * $Id: tvhmmap.h@#/main/15  09/29/96 20:08:42  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tvhmmap.h,v $
 * Revision 7.16  1996/09/29 20:08:42  griswolf
 * Port to MSVC V4.2 standard library.
 *
 * Revision 7.15  1996/08/23 18:30:02  hart
 * removed temp work around for HP aCC compiler not inheriting some operators from base
 *
 * Revision 7.14  1996/08/09 19:37:07  hart
 * HP (aCC compiler) port
 *
 * Revision 7.13  1996/05/07 19:25:48  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.12  1996/02/18 01:49:04  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.11  1996/02/07 02:42:30  jims
 * Standard library extensions now in directory rw/stdex
 *
 * Revision 7.10  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.9  1996/01/16 22:03:29  kevinj
 * Simple External Template Persistence (ETP).
 *
 * Revision 7.8  1996/01/16 00:41:16  kevinj
 * OTIS0017 patch.
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
 * Revision 7.5  1995/09/26  17:14:13  griswolf
 * Add insert and apply as synonyms ...KeyAndValue methods.
 *
 * Revision 7.4  1995/09/18  18:43:31  griswolf
 * Add capacity, fillRatio and resize methods.
 *
 * Revision 7.3  1995/09/12  19:41:49  griswolf
 * Change to #include renamed rw/rwstl/ header file.
 *
 * Revision 7.2  1995/09/05  21:54:04  jims
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

#error Cannot include header if RW_NO_STL macro is defined for your compiler

#endif

#include "rw/stdex/hashmmap.h"
#include "rw/ev_inmap.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class K, class T, class H, class EQ>
class RWTExport RWTValHashMultiMap :
#ifndef RW_ALLOCATOR_PLAIN
  private RW_VMapAssoc<rw_hashmultimap<K,T,H,EQ,allocator<K> >,
                       RWTValHashMultiMap<K,T,H,EQ>,K,T>
#else
  private RW_VMapAssoc<rw_hashmultimap<K,T,H,EQ>,
                       RWTValHashMultiMap<K,T,H,EQ>,K,T>
#endif
{
public:
// typedefs
#ifndef RW_ALLOCATOR_PLAIN
  typedef rw_hashmultimap<K,T,H,EQ,allocator<K> >      container_type;
#else
  typedef rw_hashmultimap<K,T,H,EQ>                    container_type;
#endif
  typedef RWTValHashMultiMap<K,T,H,EQ>                 this_type;
  typedef RW_VMapAssoc<container_type,this_type,K,T>   base_type;
  typedef typename container_type::iterator            iterator;
  typedef typename container_type::const_iterator      const_iterator;
  typedef typename container_type::size_type           size_type;
  typedef typename container_type::value_type          value_type;
  typedef typename container_type::key_type            key_type;
  typedef T                                            data_type;
  typedef typename container_type::reference           reference;
  typedef typename container_type::const_reference     const_reference;

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
  { return insert(key,val); }
  size_type
    capacity() const { return impl_.capacity(); }

  float
    fillRatio() const { return impl_.fill_ratio(); }

  void
    resize(size_type newsz) { impl_.resize(newsz); }

// assignment operators
  RWTValHashMultiMap<K,T,H,EQ>&
  operator=(const RWTValHashMultiMap<K,T,H,EQ>& rwmultimap)
    { impl_ = rwmultimap.impl_; return *this; }

  RWTValHashMultiMap<K,T,H,EQ>&
#ifndef RW_ALLOCATOR_PLAIN
  operator=(const rw_hashmultimap<K,T,H,EQ,allocator<K> >& stdmultimap)
#else
  operator=(const rw_hashmultimap<K,T,H,EQ>& stdmultimap)
#endif
    { impl_ = stdmultimap; return *this; }
  
// constructors
  RWTValHashMultiMap() { }
#ifndef RW_ALLOCATOR_PLAIN
  RWTValHashMultiMap(const rw_hashmultimap<K,T,H,EQ,allocator<K> >& s) 
    : impl_(s) { }
#else
  RWTValHashMultiMap(const rw_hashmultimap<K,T,H,EQ>& s) : impl_(s) { }
#endif
  RWTValHashMultiMap(const RWTValHashMultiMap<K,T,H,EQ>& rws)
    : impl_(rws.impl_) { }
  RWTValHashMultiMap(const value_type* first, const value_type* last)
    : impl_(first,last) { }

private:
#ifndef RW_ALLOCATOR_PLAIN
  rw_hashmultimap<K,T,H,EQ,allocator<K> > impl_;
#else
  rw_hashmultimap<K,T,H,EQ> impl_;
#endif
  friend class RW_VBase<container_type,this_type>;
};

RWDECLARE_PERSISTABLE_TEMPLATE_4(RWTValHashMultiMap)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tvhmmap.cc"
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWTValHashMultiMapIterator<K,V>        *
 *                                                              *
 ****************************************************************/

template <class K, class T, class H, class EQ>
class RWTExport RWTValHashMultiMapIterator
{
public:

  RWTValHashMultiMapIterator(RWTValHashMultiMap<K,T,H,EQ>& m)
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

  RWTValHashMultiMap<K,T,H,EQ>* container() const       { return rwmap_; }

  K             key() const   { return (*iter_).first; }

  void          reset()       { head_ = true; tail_ = false; } 
  void          reset(RWTValHashMultiMap<K,T,H,EQ>& m)
                              { head_ = true; tail_ = false; rwmap_ = &m; } 

  T             value() const { return (*iter_).second; }

private:
  RWTValHashMultiMap<K,T,H,EQ>*                       rwmap_;
  typename RWTValHashMultiMap<K,T,H,EQ>::iterator     iter_;
  bool                                                head_;
  bool                                                tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWTVHMMAP_H__ */
