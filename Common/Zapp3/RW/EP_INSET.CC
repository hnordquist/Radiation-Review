/***************************************************************************
 *
 * ep_inset.cc - template definitions for RW_PAssoc<StdColl,RWColl,DataTypes>
 * 
 * $Id: ep_inset.cc@#/main/27  08/21/97 12:20:21  kevinj (TOOLS707_WIN32_971021)
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

#ifndef RW_HEADER_WITH_EXTENSION
#  include <algorithm>
#else
#  include <algorithm.hpp>
#endif  /* RW_HEADER_WITH_EXTENSION */

#include "rw/stdex/algo.h"

#define INFILE_TEMPL_DECL   template <class StdColl, class RWColl, class T>
#define PSA        RW_PAssoc<StdColl,RWColl,T>

INFILE_TEMPL_DECL
void PSA::clearAndDestroy()
{
  for(iterator i = this->begin(); i != this->end(); ++i)
    delete *i;  // cast away const for non-std impls
  clear();
}

INFILE_TEMPL_DECL
typename PSA::const_value
PSA::find(const_value key) const
{
  void* ret = 0;
  iterator iter = this->std().find((T*)key);

  if(iter != this->end()) {
    ret = *iter;
  }
  return (const_value) ret;
}

INFILE_TEMPL_DECL
typename PSA::size_type
PSA::removeAll(const_value key)
{
  size_type ret = this->entries();
  pair<iterator,iterator>
    rng = this->std().equal_range((T*)key);

  if(rng.first != this->end())
    this->std().erase(rng.first,rng.second);

  return ret - this->entries();
}

/*
 * NB: Assumes that erase does not invalidate iterators referring
 * to elements in front of the one being erased.
 */
INFILE_TEMPL_DECL    
typename PSA::size_type
PSA::removeAll(bool (*fn)(typename PSA::const_value, void*),void* x)
{
  size_type    ret = this->entries();
  
  while (this->entries() && (*fn)(*this->begin(),x))
    this->std().erase(this->begin());

  iterator iter(this->begin());
  iterator iter2;

  while (iter != this->end()) {
    iter2 = iter;
    ++iter2;
    if (iter2 != this->end() && (*fn)((const_value)*iter2,x))
      this->std().erase(iter2);
    else
      ++iter;
  }

  return ret - this->entries();
}


INFILE_TEMPL_DECL
typename PSA::value_type
PSA::remove(bool (*fn)(typename PSA::const_value,void*), void* x)
{
  const_value ret = 0;
  iterator hit = this->begin();
  for(/**/; hit != this->end(); ++hit)
    if((*fn)(*hit,x))
      break;
  if(hit != this->end()) {
    ret = *hit;
    this->std().erase(hit);
  }
  return (value_type)ret;
}

INFILE_TEMPL_DECL
void PSA::Union(const StdColl& stdcoll)
{
  insert_iterator<StdColl>  putHere(this->std(),this->end());
// call the algorithm provided with Tools.h++
  set_union_inplace(this->begin(),this->end(),
                    stdcoll.begin(),stdcoll.end(),
                    putHere,this->std().key_comp());
} 


INFILE_TEMPL_DECL
void PSA::intersection(const StdColl& stdcoll)
{
// call the algorithm provided with Tools.h++
  set_intersection_inplace(this->std(),this->begin(),this->end(),
                           stdcoll.begin(),stdcoll.end(),
                           this->std().key_comp());
}

INFILE_TEMPL_DECL
void PSA::difference(const StdColl& stdcoll)
{
// call the algorithm provided with Tools.h++
  set_difference_inplace(this->std(),this->begin(),this->end(),
                         stdcoll.begin(),stdcoll.end(),this->std().key_comp());
}  

INFILE_TEMPL_DECL
void PSA::symmetricDifference(const StdColl& stdcoll)
{
// call the algorithm provided with Tools.h++
  set_symmetric_difference_inplace(
			this->std(),this->begin(),this->end(),
                        stdcoll.begin(),stdcoll.end(),this->std().key_comp());
}

#undef INFILE_TEMPL_DECL
#undef PSA
