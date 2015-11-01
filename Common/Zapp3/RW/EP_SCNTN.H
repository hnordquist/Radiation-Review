#ifndef __RWEP_SCNTN_H__
#define __RWEP_SCNTN_H__

/***************************************************************************
 *
 * ep_scntn.h - RW_PCntnr<StdColl,RWColl,DataTypes>
 *
 *     : pointer-based interface components for STL Containers
 *
 * $Id: ep_scntn.h@#/main/26  08/21/97 12:20:28  kevinj (TOOLS707_WIN32_971021)
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
#include "rw/edefs.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"
#include "rw/vstream.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl, class T>
class RWTExport RW_PCntnr : public RW_PBase<StdColl,RWColl>
{
public:
  typedef RW_PBase<StdColl,RWColl>            base_type;

  typedef typename StdColl::value_type        value_type;
  typedef typename StdColl::size_type         size_type;
  typedef typename StdColl::iterator          my_iter_;
  typedef typename StdColl::const_iterator    const_my_iter_;
  typedef typename StdColl::reference         reference;
  typedef typename StdColl::const_reference   const_reference;
  typedef const T*                            const_value;

  typedef rw_deref_compare<less<T>,T>         deref_less;
  typedef rw_deref_compare<equal_to<T>,T>     deref_equal;

  bool  operator==(const RWColl& rwc)   const
  { return this->entries() == rwc.entries() &&
      equal(this->begin(), this->end(), rwc.begin(), deref_equal()); }

  bool  operator<(const RWColl& rwc)   const
  { return lexicographical_compare(this->begin(), this->end(), rwc.begin(),
                                   rwc.end(), deref_less()); }

  void apply(void (*fn)(const_value,void*),void *x) const;
  void apply(void (*fn)(value_type,void*),void *x) { base_type::apply(fn,x); }
  void apply(void (*fn)(reference,void*),void *x)  { base_type::apply(fn,x); }

  bool  contains(const_value key) const;
  bool  contains(bool(*fn)(const_value,void*),void* x) const;
  bool  contains(bool(*fn)(value_type,void*),void* x) const
    { return base_type::contains(fn,x); }

  value_type  find(const_value key) const;
  value_type  find(bool (*fn)(const_value,void*), void* x) const;
  value_type  find(bool (*fn)(value_type,void*), void* x) const;

  size_type     index(bool (*fn)(const_value, void*), void* x) const;
  size_type     index(bool (*fn)(value_type, void*), void* x) const
    { return base_type::index(fn,x); }

  size_type     index(const_value key) const;

  size_type   occurrencesOf(const_value key) const;
  size_type   occurrencesOf(bool (*fn)(const_value,void*),void* x) const;
  size_type   occurrencesOf(bool (*fn)(value_type,void*),void* x) const
    { return base_type::occurrencesOf(fn,x);  }
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ep_scntn.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEP_SCNTN_H__ */
