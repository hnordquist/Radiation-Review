#ifndef __RWEP_SRTSQ_H__
#define __RWEP_SRTSQ_H__

/***************************************************************************
 *
 * ep_srtsq.h - RW_PSrtSeq<StdColl,RWColl,Compare,DataTypes>
 *   
 * : pointer-based sorted-collection interface components for STL Sequences
 *       
 * 
 * $Id: ep_srtsq.h@#/main/23  09/05/97 15:42:43  kevinj (TOOLS707_WIN32_971021)
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
#include "rw/ep_seq.h"

#ifndef RW_HEADER_WITH_EXTENSION
#  include <algorithm>
#else
#  include <algorithm.hpp>
#endif  /* RW_HEADER_WITH_EXTENSION */

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl, class Compare, class T>
class RWTExport RW_PSrtSeq : public RW_PSeq<StdColl,RWColl,T> {
private:
  typedef  RW_PSeq<StdColl,RWColl,T>             base_type;

public:
  typedef typename StdColl::size_type            size_type;
  typedef typename base_type::value_type         value_type;
  typedef typename base_type::const_value        const_value;
  typedef typename base_type::reference          reference;
  typedef typename base_type::const_reference    const_reference;
  typedef typename base_type::my_iter_           my_iter_;
  typedef typename base_type::const_my_iter_     const_my_iter_;
  typedef typename base_type::deref_equal        deref_equal; 
  typedef typename base_type::deref_less         deref_less;

  bool          contains(const_value key) const;
  bool          contains(bool(*fn)(const_value,void*),void* x) const
    { return base_type::contains(fn,x); }

  const_value   find(const_value key) const;
  const_value   find(bool (*fn)(const_value,void*), void* x) const
    { return base_type::find(fn,x); }

  size_type     index(const_value key) const;
  size_type     index(bool (*fn)(const_value, void*), void* x) const
    { return base_type::index(fn,x); }
    
  size_type     insert(const StdColl& coll);
  bool          insert(value_type a)
    { insertSorted(this->begin(),a); return true; } // only fail thru exception

  size_type     occurrencesOf(const_value key) const;
  size_type     occurrencesOf(bool (*fn)(const_value,void*),void* x) const
    { return base_type::occurrencesOf(fn,x); }

// both removeAll() methods should be hidden by the leaf class whenever
// possible since these versions both iterate to find, AND iterate to remove. 
  size_type     removeAll(const_value key);
  size_type     removeAll(bool (*fn)(const_value, void*),void* x)
    { return base_type::removeAll(fn,x); }

  value_type    remove(const_value key);
  value_type    remove(bool (*fn)(const_value,void*),void* x)
    { return base_type::remove(fn,x); }

/** should be protected by #if RWDEBUG but xlC bug prevents it. **/
  bool          isSorted() const;
  
protected:
  RW_PSrtSeq() {} // prevent direct usage

  my_iter_ insertSorted(my_iter_, value_type);
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ep_srtsq.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEP_SRTSQ_H__ */
