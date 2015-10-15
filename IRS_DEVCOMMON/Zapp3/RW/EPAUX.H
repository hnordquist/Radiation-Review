#ifndef __RWEPAUX_H__
#define __RWEPAUX_H__

/***************************************************************************
 *
 * epaux.h - helper components for pointer-based STL wrapper classes
 *   
 * $Id: epaux.h@#/main/29  09/05/97 15:42:46  kevinj (TOOLS707_WIN32_971021)
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
*/

#include "rw/compiler.h"
#ifndef RW_HEADER_WITH_EXTENSION
# include <iterator>
# include <functional>
# include <utility>
#else
# include <utility.hpp>
# include <iterator.hpp>
# include <functional.hpp>
#endif


#ifdef RWSTD_NO_CONST_INST
#  define RWCONST            /* #undef'd below */
#else
#  define RWCONST const      /* #undef'd below */
#endif

template <class T, class U>
struct rw_deref_pair_equal
: binary_function<const pair<T*RWCONST, U*>&,
                  const pair<T*RWCONST, U*>&, bool>
{
  bool operator()(const pair<T*RWCONST, U*>& x,
                  const pair<T*RWCONST, U*>& y) const
  { return *x.first == *y.first && *x.second == *y.second; }
};

template <class T, class U>
struct rw_deref_pair_less
: binary_function<const pair<T*RWCONST, U*>&,
                  const pair<T*RWCONST, U*>&, bool>
{
  bool operator()(const pair<T*RWCONST, U*>& x,
                  const pair<T*RWCONST, U*>& y) const
    { return (*x.first < *y.first) || ( !(*y.first < *x.first) &&
                                         (*x.second < *y.second) ) ; }
};

template <class Compare, class T>
struct rw_deref_compare : public binary_function<T*, T*, bool> {
  Compare cmp_;  
  rw_deref_compare(const Compare& c=Compare()) : cmp_(c) {}
  bool operator()(const T* x, const T* y) const
      { return cmp_(*x,*y); }
};

template <class Hash, class T>
struct rw_deref_hash : public unary_function<T*, unsigned> {
  Hash hash_;
  rw_deref_hash(const Hash& h=Hash()) : hash_(h) {}
  unsigned operator()(const T* x) const
      { return hash_(*x); }
};

#undef RWCONST

#endif /* __RWEPAUX_H__ */
