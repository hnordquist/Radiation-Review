#ifndef __RWTVDLIST_H__
#define __RWTVDLIST_H__

/***************************************************************************
 *
 * tvdlist.h - RWTValDlist<T>
 *   
 *     : value-based doubly-linked list wrapper for STL list
 * 
 * $Id: tvdlist.h@#/main/41  09/04/97 17:25:55  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tvdlist.h,v $
 * Revision 7.27  1996/10/15 23:51:18  griswolf
 * #5000: MSVC4.2 StdLib incorrectly forces 'greater' semantics
 *        for list::sort().
 *
 * Revision 7.26  1996/10/10 00:47:55  griswolf
 * Typo
 *
 * Revision 7.25  1996/10/09 22:10:29  griswolf
 * Scopus #4893: provide ctor(T*,T*) even if impl does
 *
 * Revision 7.24  1996/08/23 18:30:02  hart
 * removed work around for HP aCC compiler not inheriting operators from base
 *
 * Revision 7.23  1996/08/15 19:51:40  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.22  1996/08/09 19:34:47  hart
 * HP (aCC compiler) port
 *
 * Revision 7.21  1996/07/18 04:13:33  hart
 * ObjectStore changes
 *
 * Revision 7.20  1996/05/07 19:24:50  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.19  1996/04/29 19:12:43  groening
 * needed to include compiler.h prior to making any references to
 * RW_NO_STL.
 *
 * Revision 7.18  96/02/19  12:34:12  12:34:12  jims (Jim Shur)
 * Port to Stdlib 1.2
 * 
 * Revision 7.17  1996/02/18 01:48:56  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.16  1996/02/16 21:26:13  kyle
 * Millenium adds support for compilers with broken access adjust
 *
 * Revision 7.15  1996/01/29 23:55:45  pearson
 * Reverse sense of RW_NO_NEW_HEADERS
 *
 * Revision 7.14  1996/01/29 18:38:20  kyle
 * Millenium updates to support pre-C++ Standard headers using
 * RWSTD_NO_NEW_HEADER config define
 *
 * Revision 7.13  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.12  1996/01/23 07:38:00  jims
 * Remove duplicate inclusion of rw/tvdlist.cc
 *
 * Revision 7.11  1996/01/13 23:02:26  kevinj
 * Added RW_COMPILE_INSTANTIATE.
 *
 * Revision 7.10  1996/01/13 02:34:45  kevinj
 * Simplified External Template Persistence (ETP).
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
 * Revision 7.4  1995/11/13 12:47:12  jims
 * New stl-based collections now one with Tools-6 templates
 *
 * Revision 7.12  1995/09/18 19:09:10  jims
 * Fix precondition in removeLast to be !isEmpty() instead of isEmpty()
 *
 * Revision 7.11  1995/09/15  21:14:32  griswolf
 * Remove un-needed destructor, leaving it commented as documentation.
 *
 * Revision 7.10  1995/09/05  21:53:53  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.9  1995/08/18  17:23:04  kevinj
 * Added RW_DEFINE_PERSISTABLE_COLLECTION.
 *
 * Revision 7.8  1995/08/15  00:10:05  kevinj
 * Added restoreFrom and saveOn.
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

#include "rw/xvdlist.h"

#else

#ifndef RW_HEADER_WITH_EXTENSION
# include <list>
#else
# include <list.hpp>
#endif  /* RW_HEADER_WITH_EXTENSION */
#include "rw/ev_seq.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T>
#ifndef RW_ALLOCATOR_PLAIN
class RWTExport RWTValDlist :
                private RW_VSeq<list<T,allocator<T> >, RWTValDlist<T> >
#else
class RWTExport RWTValDlist : 
                private RW_VSeq<list<T,allocator>, RWTValDlist<T> >
#endif   
{
public:
// typedefs
  typedef RWTValDlist<T>                               this_type;
#ifndef RW_ALLOCATOR_PLAIN
  typedef list<T,allocator<T> >                        container_type;
#else
  typedef list<T,allocator>                            container_type;
#endif
  typedef RW_VSeq<container_type,this_type>            base1_type;
  typedef typename container_type::iterator            iterator;
  typedef typename container_type::const_iterator      const_iterator;
  typedef typename container_type::size_type           size_type;
  typedef typename container_type::value_type          value_type;
  typedef typename container_type::reference           reference;
  typedef typename container_type::const_reference     const_reference;

// STL-level access
  container_type&  std()             { return impl_; }
  const container_type&  std() const { return impl_; }

# ifndef RW_BROKEN_ACCESS_ADJUST  
    typedef base1_type                                 base2_type;
    typedef base1_type                                 base3_type;
# else
      //inherited interface components cannot always follow base_type
    typedef RW_VCntnr<container_type,this_type>        base2_type;
    typedef RW_VBase<container_type,this_type>         base3_type;
# endif  /* RW_BROKEN_ACCESS_ADJUST */
  
  // inherited interface components

  base1_type::append;
  base3_type::apply;
  base3_type::at;
  base3_type::begin;
  base1_type::clear;
  base3_type::contains;
  base3_type::end;
  base3_type::entries;
  base2_type::find;
  base3_type::first;
  base1_type::get;
  base3_type::index;
  base1_type::insert;
  base1_type::insertAt;
  base3_type::isEmpty;
  base2_type::minElement;
  base2_type::maxElement;
  base3_type::npos;
  base3_type::occurrencesOf;
  base3_type::operator();
  base3_type::operator<;
  base3_type::operator==;
  base3_type::operator[];
  base1_type::prepend;
  base1_type::remove;
  base1_type::removeAll;
  base1_type::removeAt;
  base1_type::removeFirst;
  base1_type::replaceAll;
  
// overridden interface components
  void sort() { 
    std().sort(); 
#ifdef RW_SORT_LIST_BACKWARD
    std().reverse();
#endif
}
  
// additional interface components
  T&        last()
    { RWPRECONDITION(this->entries() != 0); return std().back(); }
  const T&  last() const
    { RWPRECONDITION(this->entries() != 0); return std().back(); }

  T         removeLast()
    {
#ifdef RWDEBUG
      size_type n_dbg = this->entries();
#endif      
      RWPRECONDITION(!this->isEmpty());
      T ret = *(--this->end());
      std().pop_back();
      RWPOSTCONDITION(this->entries() == n_dbg - 1);
      return ret;
    }
  
// assignment operators
  RWTValDlist<T>&   operator=(const RWTValDlist<T>& rwset)
    { impl_ = rwset.impl_; return *this; }
  RWTValDlist<T>&   operator=(const container_type& stdset)
    { impl_ = stdset; return *this; }
  
// constructors
  RWTValDlist()                                : impl_() {}
  RWTValDlist(const container_type& lst)       : impl_(lst) {}
  RWTValDlist(const RWTValDlist<T>& lst)       : impl_(lst.impl_) {}
  RWTValDlist(size_type n, const T& t = T())   : impl_(n,t) {}
  RWTValDlist(const T* first, const T*last)
#ifdef RW_STLCTOR_PTR_HACK
    : impl_(first,last) {}
#else
  { while(first != last) { impl_.push_back(*first); ++first; } }
#endif
// ~RWTValDlist(); // handled by auto-generated destructor

private:
  container_type impl_;         
  friend class RW_VBase<container_type,this_type>;
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#include "rw/edefs.h"
RWDECLARE_PERSISTABLE_TEMPLATE(RWTValDlist)


/****************************************************************
 *                                                              *
 *      Declarations for RWTValDlistIterator<T>                 *
 *                                                              *
 ****************************************************************/

template <class T> class RWTExport RWTValDlistIterator
{
private:
  RWTValDlist<T>*                    list_;
  typename RWTValDlist<T>::iterator  iter_;
  bool                               head_;  // simulate head sentinel
  bool                               tail_;  // simulate tail sentinel
/*
 * Invariants:
 *   head_ and tail_ can never both be true
 *   if head_ and tail_ are both false then iter_ is valid
 *     (unless invalidated from outside this class)
 */
public:
  RWTValDlistIterator(RWTValDlist<T>& s)
    : list_(&s), head_(true), tail_(false) { }

  // Operators:
  RWBoolean     operator++();
  RWBoolean     operator--();

  RWBoolean     operator+=(size_t n);
  RWBoolean     operator-=(size_t n);

  RWBoolean     operator()()         { return ++(*this); }

  // Methods:
  RWTValDlist<T>*       container() const { return list_; }

  RWBoolean     findNext(const T& p);
  RWBoolean     findNext(RWBoolean (*testFun)(const T&, void*), void* d);

  void                  insertAfterPoint(const T& a);

  T                     key() const   { return *iter_; }

  RWBoolean             remove();       // Remove item at cursor

  RWBoolean             removeNext(const T& p);
  RWBoolean             removeNext(RWBoolean (*testFun)(const T&, void*), void* d);

  void                  reset()       { head_ = true; tail_ = false; } 
  void                  reset(RWTValDlist<T>& s)
    { head_ = true; tail_ = false; list_ = &s; } 

private:

  // Disallow postfix increment & decrement.  Unless we hide it, some 
  // compilers will substitute the prefix operator in its place.
  RWBoolean             operator++(int);
  RWBoolean             operator--(int);
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#  include "rw/tvdlist.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */

#endif /* __RWTVDLIST_H__ */
