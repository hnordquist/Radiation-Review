/***************************************************************************
 *
 * ev_srtsq.cc - template definitions for RW_VSrtSeq<StdColl,RWColl,Compare>
 * 
 * $Id: ev_srtsq.cc@#/main/23  09/12/97 13:43:25  kevinj (TOOLS707_WIN32_971021)
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

#include "rw/epfunc.h"

#ifndef RW_HEADER_WITH_EXTENSION
#  include <algorithm>
#else
#  include <algorithm.hpp>
#endif  /* RW_HEADER_WITH_EXTENSION */

#define INFILE_TEMPL_DECL template <class StdColl, class RWColl, class Compare> 
#define VSSS     RW_VSrtSeq<StdColl,RWColl,Compare>

INFILE_TEMPL_DECL
bool VSSS::contains(const_reference key) const
{
  const_my_iter_ hit = lower_bound(this->begin(),this->end(),key,Compare());
  if(hit == this->end())
    return false;
  return (*hit == key);
}

INFILE_TEMPL_DECL
bool VSSS::find(const_reference key, reference val) const
{
  const_my_iter_ hit = lower_bound(this->begin(),this->end(),key,Compare());
  if(hit != this->end() && *hit == key) {
    val = *hit;
    return true;
  }
  return false;
}

// Not efficient on classes without random access iterators. Override.
INFILE_TEMPL_DECL 
typename VSSS::size_type 
VSSS::index(const_reference key) const
{
  const_my_iter_ it = lower_bound(this->begin(),this->end(),key,Compare());
  if (it == this->end()) return this->npos;
#ifndef RW_3ARG_DISTANCE
  return distance(this->begin(),it);
#else
  size_type ret = 0;
  distance(this->begin(),it,ret);
  return ret;
#endif
}

INFILE_TEMPL_DECL
typename VSSS::size_type
VSSS::insert(const StdColl& coll)
{
  size_type ret = coll.size();
  if (ret) {
    my_iter_ it = this->begin();
    my_iter_ first = coll.begin();
    my_iter_ last = coll.end();
    while(first != last)
      it = insertSorted(it,*first++);
  }
  return ret;
}


INFILE_TEMPL_DECL
typename VSSS::size_type
VSSS::occurrencesOf(const_reference key) const
{
  pair<const_my_iter_,const_my_iter_> range =
                        equal_range(this->begin(),this->end(),key, Compare());
#ifndef RW_3ARG_DISTANCE
  return distance(range.first,range.second);
#else
  size_type ret(0);
  distance(range.first,range.second,ret);
  return ret;
#endif
}

INFILE_TEMPL_DECL
typename VSSS::size_type
VSSS::removeAll(const_reference key)
{
  typename StdColl::iterator start = ::find(this->begin(),this->end(),key);
  if(start == this->end())
    return 0;
  
  size_type counter(1);
  typename StdColl::iterator bound = start;
  ++bound;
  while(bound != this->end() && *bound == key) {
    ++bound;
    ++counter;
  }
  this->std().erase(start,bound);
  return counter;
}

INFILE_TEMPL_DECL
bool VSSS::remove(const_reference key)
{
  typename StdColl::iterator hit = 
    lower_bound(this->begin(),this->end(),key,Compare());
  if(hit != this->end() && *hit == key){
    this->std().erase(hit);
    return true;
  }
  return false;
}    

/** should be protected by #if RWDEBUG but xlC bug prevents it. Sigh **/
INFILE_TEMPL_DECL
bool VSSS::isSorted() const
{
  if(this->entries() < 2) return true; // empty and single are sorted
  const_my_iter_ trail;
  for(const_my_iter_ it = this->begin(); it != this->end();/**/)
  {
    trail = it++;
    if(it == this->end()) break;
    if(Compare().operator()(*it, *trail))
      return false;
  }
  return true;
}

// protected methods
#if !defined(__SUNPRO_CC) || (__SUNPRO_CC<0x420 && !defined(RW_NO_STL))
INFILE_TEMPL_DECL
typename VSSS::my_iter_
VSSS::insertSorted(my_iter_ hint, const_reference a) 
{
  typename StdColl::iterator hit;
  Compare comp = Compare();  // avoid warning "comp used before set"
  if(hint == this->end())
    hit = upper_bound(this->begin(),this->end(),a,comp);
  else {
    // test for "held is 'less than' a or a is not 'less than' held"
    // which amounts to "less than or equal"
    if(comp(*hint,a) || ! comp(a,*hint))
//      hit = upper_bound(hint,this->end(),a,comp);
      hit = upper_bound(this->begin(),this->end(),a,comp);
    else // a must be "greater than" the contained value
//      hit = upper_bound(this->begin(),hint,a,comp);
      hit = upper_bound(this->begin(),this->end(),a,comp);
  }
  return this->std().insert(hit,a);
}
#endif /* !SunPro*/
#undef INFILE_TEMPL_DECL
#undef VSS
