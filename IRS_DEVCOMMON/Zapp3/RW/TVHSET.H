#ifndef __RWTVHSET_H__
#define __RWTVHSET_H__

/***************************************************************************
 *
 * tvhset.h - RWTValHashSet<T,Hash,Eq>
 *
 *     : value-based hash-set wrapper for STL-like rw_hashset
 * 
 * $Id: tvhset.h@#/main/42  08/23/96 18:30:02  hart (TOOLS707_WIN32_971021)
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
 * $Log: tvhset.h,v $
 * Revision 7.31  1996/08/23 18:30:02  hart
 * removed temp work around for HP aCC compiler not inheriting some operators from base
 *
 * Revision 7.30  1996/08/20 18:47:34  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.29  1996/08/09 19:37:41  hart
 * HP (aCC compiler) port
 *
 * Revision 7.28  1996/07/18 04:13:33  hart
 * ObjectStore changes
 *
 * Revision 7.27  1996/05/07 19:26:06  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.26  1996/03/14 17:18:36  griswolf
 * fix minor typo.
 *
 * Revision 7.25  1996/02/19 12:34:12  jims
 * Port to Stdlib 1.2
 *
 * Revision 7.24  1996/02/18 01:49:06  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.23  1996/02/16 21:29:56  kyle
 * fixed base_type
 *
 * Revision 7.22  1996/02/16 20:59:04  kyle
 * Millenium added support for compilers where interface components cannot
 * always follow base_type
 *
 * Revision 7.21  1996/02/07 02:42:30  jims
 * Standard library extensions now in directory rw/stdex
 *
 * Revision 7.20  1996/02/01 21:50:02  jims
 * Separate out hash-only set operations into separate header
 *
 * Revision 7.19  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.18  1996/01/16 22:37:38  kevinj
 * Simple External Template Persistence (ETP).
 *
 * Revision 7.17  1996/01/15 22:08:08  kevinj
 * OTIS0017 patch.
 *
 * Revision 7.16  1996/01/08 18:05:59  kevinj
 * *** empty log message ***
 *
 * Revision 7.15  1996/01/04 16:28:27  pearson
 * Change isSubset to isSubsetOf
 *
 * Revision 7.14  1995/12/20 23:19:20  kevinj
 * External template persistence using RW*Persistor.
 *
 * Revision 7.13  1995/12/15 00:28:03  pearson
 * Remove isUnequal and rename isEqual to isEquivalent
 * Move isEquivalent, isSubset and isProperSubset to the base class
 * Add access adjustment due to private inheritance
 *
 * Revision 7.12  1995/12/14 21:43:42  pearson
 * Remove intersectWith and streamline isSubset
 *
 * Revision 7.11  1995/12/14 19:05:56  pearson
 * Use available interface to do intersectWith and isSubset
 *
 * Revision 7.10  1995/12/14  17:54:25  jims
 * saveOn, restoreFrom now global instead of member functions
 *
 * Revision 7.9  1995/12/13 17:59:59  pearson
 * Dig out hash function when constructing temporary RWTValHashSet in isSubset
 *
 * Revision 7.8  1995/12/13 00:23:45  pearson
 * Update interface to match non-STL hashset container
 *
 * Revision 7.7  1995/12/12 00:03:06  jims
 * Add RWTExport
 *
 * Revision 7.6  1995/12/11 22:30:41  jims
 * Remove 'Sca' from interior class names
 * Add 'rw_' to global helper classes
 * Change 'muterator' to 'filterator'
 *
 * Revision 7.5  1995/12/01 18:26:44  jims
 * disallow postfix increment on external iterator
 *
 * Revision 7.4  1995/11/17 08:15:43  jims
 * Merge otis-et and otis-stl templates
 *
 * Revision 7.6  1995/09/25 11:12:07  jims
 * Add #include for ev_hash.cc
 *
 * Revision 7.5  1995/09/21  00:23:55  griswolf
 * Provide correct set-method calls.
 * Add default size to constructor.
 *
 * Revision 7.4  1995/09/18  18:42:20  griswolf
 * Add capacity, fillRatio, resize methods.
 * Add difference, intersection, symmetric difference and Union methods.
 *
 * Revision 7.3  1995/09/12  19:41:49  griswolf
 * Change to #include renamed rw/rwstl/ header file.
 *
 * Revision 7.2  1995/09/05  21:54:56  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.1  1995/08/31  06:45:31  jims
 * Move to revision 7.1
 * 
 ****************************************************************************/

#include "rw/compiler.h"

#ifdef RW_NO_STL

#include "rw/xvhset.h"
#define RWDefHArgs(T)

#else

#ifndef RWDefHArgs
#  define RWDefHArgs(T) ,RWTHasher<T>,equal_to<T>
#endif

// STL-based implementation:

#include "rw/stdex/hashset.h"
#include "rw/ev_hash.h"
#include "rw/edefs.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T,class H,class EQ>
#ifndef RW_ALLOCATOR_PLAIN
class RWTExport RWTValHashSet
    : private RW_VHAssoc<rw_hashset<T,H,EQ,allocator<T> >,RWTValHashSet<T,H,EQ> >
#else
class RWTExport RWTValHashSet
    : private RW_VHAssoc<rw_hashset<T,H,EQ>,RWTValHashSet<T,H,EQ> >
#endif
{
public:
// typedefs
  typedef RWTValHashSet<T,H,EQ>                      this_type;
#ifndef RW_ALLOCATOR_PLAIN
  typedef rw_hashset<T,H,EQ,allocator<T> >           container_type;
#else
  typedef rw_hashset<T,H,EQ>                         container_type;
#endif
  typedef RW_VHAssoc<container_type,this_type>       base_type;
  typedef base_type                                  base1_type;
  
#ifndef RW_BROKEN_ACCESS_ADJUST
  typedef base1_type                                 base3_type; 
  typedef base1_type                                 base2_type; 
#else
  typedef RW_VAssoc<container_type,this_type>        base2_type;
  typedef RW_VCntnr<container_type,this_type>        base3_type;
#endif  /* RW_BROKEN_ACCESS_ADJUST */
  
  typedef typename container_type::iterator          iterator;
  typedef typename container_type::const_iterator    const_iterator;
  typedef typename container_type::size_type         size_type;
  typedef typename container_type::value_type        value_type;
  typedef typename container_type::key_type          key_type;
  typedef typename container_type::reference         reference;
  typedef typename container_type::const_reference   const_reference;

// STL-level access
  container_type&  std()             { return impl_; }
  const container_type&  std() const { return impl_; }

// inherited interface components
  base1_type::begin;
  base1_type::end;
  base3_type::isEmpty;
  base3_type::entries;
  base1_type::operator==;
  base1_type::clear;
  base1_type::contains;
  base1_type::find;
  base1_type::occurrencesOf;
  base1_type::remove;
  base1_type::removeAll;
  base1_type::isSubsetOf;
  base1_type::isProperSubsetOf;
  base1_type::isEquivalent;

// selective inheritance (hides other same-name components)
  RWTValHashSet<T,H,EQ> &
    Union(const RWTValHashSet<T,H,EQ>& rhs)
    { base_type::UnionHash(rhs); return *this; }

  RWTValHashSet<T,H,EQ> &
#ifndef RW_ALLOCATOR_PLAIN
    Union(const rw_hashset<T,H,EQ,allocator<T> >& rhs)
#else
    Union(const rw_hashset<T,H,EQ>& rhs)
#endif
    { base_type::UnionHash(rhs); return *this; }

  RWTValHashSet<T,H,EQ> &
    intersection(const RWTValHashSet<T,H,EQ>& rhs)
    { base_type::intersectionHash(rhs); return *this; }


  RWTValHashSet<T,H,EQ> &
#ifndef RW_ALLOCATOR_PLAIN
    intersection(const rw_hashset<T,H,EQ,allocator<T> >& rhs)
#else
    intersection(const rw_hashset<T,H,EQ>& rhs)
#endif
    { base_type::intersectionHash(rhs);  return *this; }

  RWTValHashSet<T,H,EQ> &
    difference(const RWTValHashSet<T,H,EQ>& rhs)
    { base_type::differenceHash(rhs);  return *this; }

  RWTValHashSet<T,H,EQ> &
#ifndef RW_ALLOCATOR_PLAIN
    difference(const rw_hashset<T,H,EQ,allocator<T> >& rhs)
#else
    difference(const rw_hashset<T,H,EQ>& rhs)
#endif
    { base_type::differenceHash(rhs);  return *this; }

  RWTValHashSet<T,H,EQ> &
    symmetricDifference(const RWTValHashSet<T,H,EQ>& rhs)
    { base_type::symmetricDifferenceHash(rhs);  return *this; }

  RWTValHashSet<T,H,EQ> &
#ifndef RW_ALLOCATOR_PLAIN
    symmetricDifference(const rw_hashset<T,H,EQ,allocator<T> >& rhs)
#else
    symmetricDifference(const rw_hashset<T,H,EQ>& rhs)
#endif
    { base_type::symmetricDifferenceHash(rhs);  return *this; }

  void  apply(void (*fn)(const_reference,void*),void *x) const
    { base_type::apply(fn,x); }

// additional interface components
  /** bool return value is a change to Tools 6.1 interface **/
  bool  insert(const_reference datum) { return (impl_.insert(datum)).second; }

  size_type
    capacity() const { return impl_.capacity(); }

  float
    fillRatio() const { return impl_.fill_ratio(); }

  void
    resize(size_type newsz) { impl_.resize(newsz); }

// assignment operators
  RWTValHashSet<T,H,EQ>&   operator=(const RWTValHashSet<T,H,EQ>& rwset)
    { impl_ = rwset.impl_; return *this; }
#ifndef RW_ALLOCATOR_PLAIN
  RWTValHashSet<T,H,EQ>&   operator=(const rw_hashset<T,H,EQ,allocator<T> >& stdset)
    { impl_ = stdset; return *this; }
#else
  RWTValHashSet<T,H,EQ>&   operator=(const rw_hashset<T,H,EQ>& stdset)
    { impl_ = stdset; return *this; }
#endif
  
// constructors
  RWTValHashSet(size_type sz = 1024, const H& h = H(), const EQ& eq= EQ())
    : impl_(sz,h,eq) { }

#ifndef RW_ALLOCATOR_PLAIN
  RWTValHashSet(const rw_hashset<T,H,EQ,allocator<T> >& s) : impl_(s) { }
#else
  RWTValHashSet(const rw_hashset<T,H,EQ>& s) : impl_(s) { }
#endif
  RWTValHashSet(const RWTValHashSet<T,H,EQ>& rws) : impl_(rws.impl_) { }

  RWTValHashSet(const T* first, const T* last,
            size_type sz = 1024, const H& h = H(), const EQ& eq = EQ()
           )
    : impl_(first,last,sz,h,eq) { }


  // For Tools-6 compatibility
  RWTValHashSet(const H& h, size_type sz = 1024)
    : impl_(sz,h,EQ()) { }

private:
#ifndef RW_ALLOCATOR_PLAIN
  rw_hashset<T,H,EQ,allocator<T> > impl_;
#else
  rw_hashset<T,H,EQ>               impl_;
#endif
  friend class RW_VBase<container_type,this_type>;
  
  RW_T_TYPESPEC  /* This usually expands to nothing */
};


RWDECLARE_PERSISTABLE_TEMPLATE_3(RWTValHashSet)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tvhset.cc"
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWTValHashSetIterator<T,H,EQ>          *
 *                                                              *
 ****************************************************************/

template <class T, class H, class EQ>
class RWTExport RWTValHashSetIterator
{ 

public:

  RWTValHashSetIterator(RWTValHashSet<T,H,EQ>& t)
          : table_(&t), head_(true), tail_(false) { }

  RWBoolean     operator++()    // Advance and test
  {
    RWASSERT(!(head_ == true  &&  tail_ == true)); 

    if (tail_)
      /* do nothing */ ;

    else if (head_) {
      iter_ = table_->begin();
      head_ = false;
    }
    else
      ++iter_;

    if (!tail_ && iter_ == table_->end())
      tail_ = true;

    RWASSERT(!(head_ == true  &&  tail_ == true)); 

    return !tail_;
  }

  RWBoolean     operator()()    { return ++(*this); }
        
  RWTValHashSet<T,H,EQ>* container() const  { return table_; }

  T             key() const   { return *iter_; }

  void          reset()       { head_ = true; tail_ = false; } 
  void          reset(RWTValHashSet<T,H,EQ>& t)
                              { head_ = true; tail_ = false; table_ = &t; } 
private:

  RWTValHashSet<T,H,EQ>*                         table_;
  typename RWTValHashSet<T,H,EQ>::iterator       iter_;
  bool                                           head_;
  bool                                           tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
  
  RW_T_TYPESPEC  /* This usually expands to nothing */
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */

#endif /* __RWTVHSET_H__ */
