#ifndef __RWEP_MCNTN_H__
#define __RWEP_MCNTN_H__

/***************************************************************************
 *
 * ep_mcntn.h - RW_PMapCntnr<StdColl,RWColl,DataTypes>
 *   
 *     : pointer-based Key/Data interface components for STL Containers
 * 
 * $Id: ep_mcntn.h@#/main/19  08/21/97 12:20:24  kevinj (TOOLS707_WIN32_971021)
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

#include "rw/ep_cntnr.h"
#include "rw/epersist.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"
#include "rw/edefs.h"

class RWExport RWFile;
class RWExport RWvistream;
class RWExport RWvostream;

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl, class Key, class T>
class RWTExport RW_PMapCntnr : public RW_PBase<StdColl,RWColl>
{
public:
  typedef RW_PBase<StdColl,RWColl>            base_type;

  typedef typename StdColl::value_type        value_type;
  typedef typename StdColl::size_type         size_type;
  typedef typename StdColl::iterator          iterator;
  typedef typename StdColl::const_iterator    const_iterator;
  typedef typename StdColl::reference         reference;
  typedef typename StdColl::const_reference   const_reference;

  typedef Key*              value_type_key;
  typedef const Key*        const_value_key;
  typedef Key*&             reference_key;
  typedef const Key*const&  const_reference_key;

  typedef T*                value_type_data;
  typedef const T*          const_value_data;
  typedef T*&               reference_data;
  typedef const T*const&    const_reference_data;

  typedef rw_deref_pair_less<Key,T>    deref_less;
  typedef rw_deref_pair_equal<Key,T>   deref_equal;

  bool  operator==(const RWColl& rwc)   const
  { return this->entries() == rwc.entries() &&
      equal(this->begin(), this->end(), rwc.begin(), deref_equal()); }

  bool  operator<(const RWColl& rwc)   const
  { return lexicographical_compare(
                           this->std().begin(), this->std().end(), 
                                    rwc.begin(),        rwc.end(), 
                           deref_less()); }

  bool  contains(const_value_key key) const {
    const_iterator iter = this->begin();
    while(iter != this->end()) {
      if(*((*iter).first) == *key) break;
      ++iter;
    }
    return iter != this->end();
  }

  bool  contains(bool(*fn)(value_type,void*),void* x) const
    { return base_type::contains(fn,x); }

  value_type nilValue() const { return value_type(0,0); }
  value_type find(const_reference key) const;
  value_type find(bool (*fn)(value_type,void*), void* x) const;

  typename StdColl::size_type     index(bool (*fn)(value_type, void*), void* x)
    { return base_type::index(fn,x); }

  typename StdColl::size_type     index(const_reference key) const;

  typename StdColl::size_type     occurrencesOf(const_value_key key) const {
    size_type ret = 0;
    const_iterator iter = this->begin();
    while(iter != this->end()) {
      if (*((*iter).first) == *key) break;
      ++iter;
    }
    while(iter != this->end()) {
      if(*((*iter).first) == *key) ++ret;
      ++iter;
    }
    return ret;
  }

  typename StdColl::size_type   occurrencesOf(bool (*fn)(value_type,void*),void* x) const
    { return base_type::occurrencesOf(fn,x);  }
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ep_mcntn.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEP_MCNTN_H__ */

