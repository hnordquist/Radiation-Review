#ifndef __RWTVHASHT_H__
#define __RWTVHASHT_H__

/***************************************************************************
 *
 * tvhasht.h - RWTValHashTable<T,Compare>
 *   
 *     : value-based multi hash set wrapper for STL-like rw_multihashset
 * 
 * $Id: tvhasht.h@#/main/41  08/23/96 18:30:02  hart (TOOLS707_WIN32_971021)
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
 * $Log: tvhasht.h,v $
 * Revision 7.24  1996/08/23 18:30:02  hart
 * removed temp work around for HP aCC compiler not inheriting some operators from base
 *
 * Revision 7.23  1996/08/16 22:12:00  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.22  1996/08/09 19:35:22  hart
 * HP (aCC compiler) port
 *
 * Revision 7.21  1996/07/18 04:13:33  hart
 * ObjectStore changes
 *
 * Revision 7.20  1996/05/07 19:25:07  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.19  1996/02/19 12:34:12  jims
 * Port to Stdlib 1.2
 *
 * Revision 7.18  1996/02/18 01:48:59  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.17  1996/02/16 21:27:36  kyle
 * fixed base_type
 *
 * Revision 7.16  1996/02/16 21:19:08  kyle
 * Millenium added support for compilers whos interface components cannot
 * always follow base_type
 *
 * Revision 7.15  1996/02/14 23:21:51  pearson
 * Make isSubsetOf, isProperSubsetOf, and isEquivalent const member functions
 *
 * Revision 7.14  1996/02/07 02:42:30  jims
 * Standard library extensions now in directory rw/stdex
 *
 * Revision 7.13  1996/02/01 21:50:02  jims
 * Separate out hash-only set operations into separate header
 *
 * Revision 7.12  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.11  1996/01/13 18:08:34  kevinj
 * Simple External Template Persistence (ETP).
 *
 * Revision 7.10  1996/01/04 16:28:14  pearson
 * Change isSubset to isSubsetOf
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
 * Revision 7.2  1995/09/05  21:54:26  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.1  1995/08/31  06:45:31  jims
 * Move to revision 7.1
 * 
 ****************************************************************************/

#include "rw/compiler.h"

#ifdef RW_NO_STL

#include "rw/xvhasht.h"
#define RWDefHArgs(T)

#else

#ifndef RWDefHArgs
#  define RWDefHArgs(T) ,RWTHasher<T>,equal_to<T>
#endif

// STL-based implementation:

#include "rw/stdex/hashmset.h"
#include "rw/ev_hash.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T,class H,class EQ>
#ifndef RW_ALLOCATOR_PLAIN
   class RWTExport RWTValHashMultiSet : private
   RW_VHAssoc<rw_hashmultiset<T,H,EQ,allocator<T> >, RWTValHashMultiSet<T,H,EQ> >
#else
   class RWTExport RWTValHashMultiSet : private
   RW_VHAssoc<rw_hashmultiset<T,H,EQ>, RWTValHashMultiSet<T,H,EQ> >
#endif
{
public:
// typedefs
#ifndef RW_ALLOCATOR_PLAIN
  typedef rw_hashmultiset<T,H,EQ,allocator<T> >     container_type;
#else
  typedef rw_hashmultiset<T,H,EQ>                   container_type;
#endif
  typedef RWTValHashMultiSet<T,H,EQ>                this_type;
  typedef RW_VHAssoc<container_type,this_type>      base_type;
  typedef base_type                                 base1_type;
  
# ifndef   RW_BROKEN_ACCESS_ADJUST
   typedef base1_type                               base2_type;
   typedef base1_type                               base3_type;
# else  
   typedef RW_VCntnr<container_type,this_type>      base2_type;
   typedef RW_VBase<container_type,this_type>       base3_type;
#endif  /* RW_BROKEN_ACCESS_ADJUST */

  typedef typename container_type::iterator         iterator;
  typedef typename container_type::const_iterator   const_iterator;
  typedef typename container_type::size_type        size_type;
  typedef typename container_type::value_type       value_type;
  typedef typename container_type::key_type         key_type;
  typedef typename container_type::reference        reference;
  typedef typename container_type::const_reference  const_reference;

// STL-level access
  container_type&  std()              { return impl_; }
  const container_type&  std() const  { return impl_; }

// inherited interface components
  base1_type::begin;
  base1_type::end;
  base3_type::isEmpty;
  base3_type::entries;
  base1_type::operator==;
  base1_type::clear;
  base1_type::contains;
  base2_type::find;
  base1_type::occurrencesOf;
  base1_type::remove;
  base1_type::removeAll;

  void
    Union(const RWTValHashMultiSet<T,H,EQ>& rhs) { base_type::UnionHash(rhs); }

  void
    intersection(const RWTValHashMultiSet<T,H,EQ>& rhs)
        { base_type::intersectionHash(rhs); }

  void
    difference(const RWTValHashMultiSet<T,H,EQ>& rhs)
        { base_type::differenceHash(rhs); }

  RWBoolean
    isSubsetOf(const RWTValHashMultiSet<T,H,EQ>& rhs) const
    { return base_type::isSubsetMultiOf(rhs); }

  RWBoolean
    isProperSubsetOf(const RWTValHashMultiSet<T,H,EQ>& rhs) const
    { return base_type::isProperSubsetMultiOf(rhs); }

  RWBoolean
    isEquivalent(const RWTValHashMultiSet<T,H,EQ>& rhs) const
    { return base_type::isEquivalentMulti(rhs); }

  void
  symmetricDifference(const RWTValHashMultiSet<T,H,EQ>& rhs)
        { base_type::symmetricDifferenceHash(rhs); }

// selective inheritance (hides other same-name components)
  void  apply(void (*fn)(const_reference,void*),void *x) const
    { base_type::apply(fn,x); }

// additional interface components
  bool  insert(const_reference datum) { impl_.insert(datum); return true; } 
  
  size_type
    capacity() const { return impl_.capacity(); }

  float
    fillRatio() const { return impl_.fill_ratio(); }

  void
    resize(size_type newsz) { impl_.resize(newsz); }

// assignment operators
  RWTValHashMultiSet<T,H,EQ>&   operator=(const RWTValHashMultiSet<T,H,EQ>& rwmset)
    { impl_ = rwmset.impl_; return *this; }
#ifndef RW_ALLOCATOR_PLAIN
  RWTValHashMultiSet<T,H,EQ>&   operator=(const rw_hashmultiset<T,H,EQ,allocator<T> >& stdmset)
    { impl_ = stdmset; return *this; }
#else
  RWTValHashMultiSet<T,H,EQ>&   operator=(const rw_hashmultiset<T,H,EQ>& stdmset)
    { impl_ = stdmset; return *this; }
#endif

// constructors
  RWTValHashMultiSet(size_type sz = 1024, const H& h = H(), const EQ& eq = EQ())
    : impl_(sz,h,eq)  { }

#ifndef RW_ALLOCATOR_PLAIN
  RWTValHashMultiSet(const rw_hashmultiset<T,H,EQ,allocator<T> >& s) : impl_(s) { }
#else
  RWTValHashMultiSet(const rw_hashmultiset<T,H,EQ>& s) : impl_(s) { }
#endif
  RWTValHashMultiSet(const RWTValHashMultiSet<T,H,EQ>& rws) : impl_(rws.impl_) { }

  RWTValHashMultiSet(const T* first, const T* last,
                 size_type sz = 1024, const H& h = H(), const EQ& eq = EQ())
    : impl_(first,last,sz,h,eq) { }

  // Tools-6 compatible constructor:
  RWTValHashMultiSet(const H& h, size_type sz = RWDEFAULT_CAPACITY)
    : impl_(sz,h,EQ()) { }

private:
#ifndef RW_ALLOCATOR_PLAIN
  rw_hashmultiset<T,H,EQ,allocator<T> > impl_;
#else
  rw_hashmultiset<T,H,EQ> impl_;
#endif
  friend class RW_VBase<container_type,this_type>;
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};


RWDECLARE_PERSISTABLE_TEMPLATE_3(RWTValHashMultiSet)

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tvhasht.cc"
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWTValHashMultiSetIterator<T,H,EQ>     *
 *                                                              *
 ****************************************************************/

template <class T, class H, class EQ>
class RWTExport RWTValHashMultiSetIterator
{ 

public:

  RWTValHashMultiSetIterator(RWTValHashMultiSet<T,H,EQ>& t)
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
        
  RWTValHashMultiSet<T,H,EQ>* container() const  { return table_; }

  T             key() const   { return *iter_; }

  void          reset()       { head_ = true; tail_ = false; } 
  void          reset(RWTValHashMultiSet<T,H,EQ>& t)
                              { head_ = true; tail_ = false; table_ = &t; } 

private:

  RWTValHashMultiSet<T,H,EQ>*                    table_;
  typename RWTValHashMultiSet<T,H,EQ>::iterator  iter_;
  bool                                           head_;
  bool                                           tail_;

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#define RWTValHashTable RWTValHashMultiSet
#define RWTValHashTableIterator RWTValHashMultiSetIterator

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* RW_NO_STL */

#endif /* __RWTVHASHT_H__ */
