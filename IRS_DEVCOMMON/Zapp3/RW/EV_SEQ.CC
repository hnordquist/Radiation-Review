/***************************************************************************
 *
 * e_seq.cc - template definitions for RW_VSeq<StdColl,RWColl>
 * 
 * $Id: ev_seq.cc@#/main/21  09/05/97 15:42:53  kevinj (TOOLS707_WIN32_971021)
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

#define INFILE_TEMPL_DECL template <class StdColl, class RWColl>
#define VSS      RW_VSeq<StdColl, RWColl>

INFILE_TEMPL_DECL
typename VSS::size_type
VSS::removeAll(bool (*fn)(typename VSS::const_reference key,void*),void* x )
{
  typedef x_pointer_to_binary_function<const_reference, void*, bool>
          boolPtrFun;

  size_type ret = this->std().size();
  my_iter_ newEnd = remove_if(this->begin(),this->end(),
                                x_binder2nd<boolPtrFun>(fn,x));
  this->std().erase(newEnd,this->end());
  return ret - this->std().size();
}

INFILE_TEMPL_DECL
typename VSS::size_type
VSS::removeAll(typename VSS::const_reference key)
{
  size_type ret(this->std().size());

  // NOTE: probably bug when namespaces become supported since
  //       standard remove algorithm won't be in global namespace
  my_iter_ newEnd = ::remove(this->begin(),this->end(),key);
  this->std().erase(newEnd,this->end());
  return ret - this->std().size();
}

INFILE_TEMPL_DECL
typename VSS::value_type
VSS::removeAt(typename VSS::size_type index)
{
  boundsCheck(index);
  my_iter_ i = this->begin();
  advance(i,index);
  value_type ret = *i;
  this->std().erase(i);
  return ret;
}

INFILE_TEMPL_DECL
typename VSS::value_type
VSS::removeFirst()
{
#ifdef RWDEBUG
  size_type n_dbg = this->entries();
#endif      
  RWPRECONDITION(!this->isEmpty());
  my_iter_ i = this->begin();
  value_type ret = *i;
  this->std().erase(i);
  RWPOSTCONDITION(this->entries() == n_dbg - 1);
  return ret;
}


INFILE_TEMPL_DECL
bool VSS::remove(bool (*fn)(typename VSS::const_reference,void*),void* x )
{
  for(my_iter_ hit = this->begin(); hit != this->end(); ++hit)  {
    if((*fn)(*hit,x)) {
      this->std().erase(hit);
      return true;
    }
  }
  return false;
}

INFILE_TEMPL_DECL
bool VSS::remove(typename VSS::const_reference key)
{
  for(my_iter_ i = this->begin(); i != this->end(); ++i) {
    if(*i == key) {
      this->std().erase(i);
      return true;
    }
  }
  return false;
}


INFILE_TEMPL_DECL
typename VSS::size_type
VSS::replaceAll(bool (*fn)(typename VSS::const_val_ref, void*),void* x,
                const_val_ref newVal)
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
typename VSS::size_type
VSS::replaceAll(typename VSS::const_val_ref oldVal, typename VSS::const_val_ref newVal)
{
  size_type count = 0;
  my_iter_ i = this->begin();

  for(/**/; i != this->end(); ++i) {
    if(*i == oldVal) {
      *i = newVal;
      ++count;
    }
  }
  return count;
}

#undef INFILE_TEMPL_DECL
#undef VSS     
