#ifndef __RWEV_SRTSQ_H__
#define __RWEV_SRTSQ_H__

/***************************************************************************
 *
 * ev_srtsq.h - RW_VSrtSeq<StdColl,RWColl,Compare>
 *   
 * : value-based sorted-collection interface components for STL Sequences
 * 
 * $Id: ev_srtsq.h@#/main/22  09/12/97 13:43:27  kevinj (TOOLS707_WIN32_971021)
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

#include "rw/ev_seq.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl, class Compare>
class RWTExport RW_VSrtSeq : public RW_VSeq<StdColl,RWColl>
{
public:
  typedef typename StdColl::const_reference              const_reference;
  typedef bool (*testFunction)(const_reference, void*);
  typedef RW_VSeq<StdColl,RWColl>                        base_type;
  typedef typename StdColl::const_iterator               my_iter_;
  typedef typename StdColl::size_type                    size_type;
  typedef typename StdColl::reference                    reference;
  typedef typename StdColl::const_iterator               const_my_iter_;

  bool          contains(const_reference) const;
  bool          contains(bool(*fn)(const_reference,void*),void* x) const
    { return base_type::contains(fn,x); }

  bool          find(const_reference, reference) const;
  bool          find(bool(*fn)(const_reference,void*),void* x,
                     reference hit) const
    { return base_type::find(fn,x,hit); }

  size_type     index(const_reference key) const;
  size_type     index(bool (*fn)(const_reference, void*), void* x) const
    { return base_type::index(fn,x); }

  size_type     insert(const StdColl& coll);
  bool          insert(const_reference a)
    { insertSorted(this->begin(),a); return true; } // only fail thru exception

  size_type     occurrencesOf(const_reference) const;
  size_type     occurrencesOf(bool (*fn)(const_reference,void*),
                              void* x) const
    { return base_type::occurrencesOf(fn,x); }

  size_type     removeAll(const_reference);
  size_type     removeAll(bool (*fn)(const_reference, void*),void* x)
    { return base_type::removeAll(fn,x); }
  
  bool          remove(const_reference key);
  bool          remove(bool (*fn)(const_reference,void*),void* x)
    { return base_type::remove(fn,x); }

/** should be guarded by #if RWDEBUG but xlC bug prevents it. **/
  bool          isSorted() const;
  
protected:
#if !defined(__SUNPRO_CC) || (__SUNPRO_CC<0x420 && !defined(RW_NO_STL))
  my_iter_ insertSorted(my_iter_, const_reference);
#else
  my_iter_ insertSorted(my_iter_ hint, const_reference a) 
  {
    typename StdColl::iterator hit;
    Compare comp = Compare();  // avoid warning "comp used before set"
    if(hint == end())
     hit = upper_bound(begin(),end(),a,comp);
    else {
    // test for "held is 'less than' a or a is not 'less than' held"
    // which amounts to "less than or equal"
    if(comp(*hint,a) || ! comp(a,*hint))
//      hit = upper_bound(hint,end(),a,comp);
     hit = upper_bound(begin(),end(),a,comp);
    else // a must be "greater than" the contained value
//      hit = upper_bound(begin(),hint,a,comp);
      hit = upper_bound(begin(),end(),a,comp);
   }
   return std().insert(hit,a);
  }
#endif /* !SunPro */
protected:
  RW_VSrtSeq() { }  // can only be used as base class
  RW_VSrtSeq(const RW_VSrtSeq<StdColl,RWColl,Compare>&);
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ev_srtsq.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEV_SRTSQ_H__ */
