/***************************************************************************
 *
 * ep_seq.cc - template definitions for RW_PSeq<StdColl,RWColl,DataTypes>
 * 
 * $Id: ep_seq.cc@#/main/22  09/05/97 15:42:41  kevinj (TOOLS707_WIN32_971021)
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
#include "rw/epfunc.h"

#ifndef RW_HEADER_WITH_EXTENSION
#  include <algorithm>
#else
#  include <algorithm.hpp>
#endif  /* RW_HEADER_WITH_EXTENSION */

#define INFILE_TEMPL_DECL template <class RWColl, class StdColl, class T>
#define PSS      RW_PSeq<RWColl,StdColl,T>

INFILE_TEMPL_DECL
void PSS::clearAndDestroy()
{
  for(my_iter_ it = this->begin(); it != this->end(); ++it)
    delete *it;
  clear();
}

INFILE_TEMPL_DECL
typename PSS::size_type
PSS::removeAll(bool (*fn)(const T*,void*),void* x )
{
  typedef x_pointer_to_binary_function<const_value, void*, bool>  boolPtrFun;
  size_type ret = this->entries();
  my_iter_ newEnd = ::remove_if(this->begin(),this->end(),
                                x_binder2nd<boolPtrFun>(fn,x));
  
  this->std().erase(newEnd,this->end());
  return ret - this->entries();
}

INFILE_TEMPL_DECL
typename PSS::size_type
PSS::removeAll(bool (*fn)(T*,void*),void* x )
{
  typedef x_pointer_to_binary_function<value_type, void*, bool>  boolPtrFun;
  size_type ret = this->entries();
  my_iter_ newEnd = ::remove_if(this->begin(),this->end(),
                                x_binder2nd<boolPtrFun>(fn,x));
  
  this->std().erase(newEnd,this->end());
  return ret - this->entries();
}

template <class PtrToConst>
struct deref_match : public unary_function<PtrToConst,bool> {
  PtrToConst val;
  deref_match(PtrToConst t) : val(t) {}
  bool operator()(PtrToConst t) {return *t == *val;}
};
  
INFILE_TEMPL_DECL
typename PSS::size_type
PSS::removeAll(const T* key)
{
  size_type ret = this->entries();
  my_iter_ newEnd = remove_if(this->begin(),this->end(),deref_match<const_value>(key));
  this->std().erase(newEnd,this->end());
  return ret - this->entries();
}

INFILE_TEMPL_DECL
typename PSS::value_type
PSS::remove(bool (*fn)(typename PSS::const_value key,void*),void* x )
{
  value_type ret;
  for(my_iter_ hit = this->begin(); hit != this->end(); ++hit)  {
    if((*fn)(*hit,x)) {
      ret = *hit;
      this->std().erase(hit);
      return ret;
    }
  }
  return rwnil;
}

INFILE_TEMPL_DECL
typename PSS::value_type
PSS::remove(bool (*fn)(typename PSS::value_type key,void*),void* x )
{
  value_type ret;
  for(my_iter_ hit = this->begin(); hit != this->end(); ++hit)  {
    if((*fn)(*hit,x)) {
      ret = *hit;
      this->std().erase(hit);
      return ret;
    }
  }
  return rwnil;
}

INFILE_TEMPL_DECL
typename PSS::value_type
PSS::remove(typename PSS::const_value key)
{
  value_type ret;
  for(my_iter_ hit = this->begin(); hit != this->end(); ++hit) {
    if(**hit == *key) {
      ret = *hit;
      this->std().erase(hit);
      return ret;
    }
  }
  return rwnil;
}

INFILE_TEMPL_DECL
typename PSS::size_type
PSS::replaceAll(bool (*fn)(typename PSS::const_value, void*),
                      void* x,typename PSS::value_type newVal)
{
  size_type count = 0;
  my_iter_ i = this->begin();

  for(/**/; i != this->end(); ++i) {
    if((*fn)(*i,x)) {
      *i = newVal;
      ++count;
    }
  }
  return count;
}

INFILE_TEMPL_DECL
typename PSS::size_type
PSS::replaceAll(bool (*fn)(typename PSS::value_type, void*),
                      void* x, typename PSS::value_type newVal)
{
  size_type count = 0;
  my_iter_ i = this->begin();

  for(/**/; i != this->end(); ++i) {
    if((*fn)(*i,x)) {
      *i = newVal;
      ++count;
    }
  }
  return count;
}

INFILE_TEMPL_DECL
typename PSS::size_type
PSS::replaceAll(typename PSS::const_value oldVal,typename PSS::value_type newVal)
{
  size_type count = 0;
  my_iter_ i = this->begin();

  for(/**/; i != this->end(); ++i) {
    if (**i == *oldVal) {
      *i = newVal;
      ++count;
    }
  }
  return count;
}

#undef INFILE_TEMPL_DECL
#undef PSS
