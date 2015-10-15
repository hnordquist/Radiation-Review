/*
 * Template definitions for RWTValSortedVector<T>
 *
 * $Id: tvsrtvec.cc@#/main/24  09/04/97 17:26:06  griswolf (TOOLS707_WIN32_971021)
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
 ***************************************************************************/

#include "rw/epersist.h"

template <class T, class C>
RWTValSortedVector<T,C>::RWTValSortedVector(const T* first, const T* last) {
  iterator it = this->begin();
  while(first != last)
    it = insertSorted(it,*first++);
}

RWDEFINE_PERSISTABLE_TEMPLATE_2(RWTValSortedVector)

template <class T, class C, class S>
void rwSaveGuts(S& str, const RWTValSortedVector<T,C>& c)
{
#ifdef RW_FIX_XSIZE_T
  str.putSizeT(c.entries());
#else
  str << c.entries();
#endif
  for (typename RWTValSortedVector<T,C>::size_type i=0; i < c.entries(); ++i)
    str << c[i];
}

template <class T, class C, class S>
void rwRestoreGuts(S& str, RWTValSortedVector<T,C>& c)
{
  typedef typename RWTValSortedVector<T,C>::size_type size_type;
  typename RWTValSortedVector<T,C>::size_type count;
#ifdef RW_FIX_XSIZE_T
  str.getSizeT(count);
#else
  str >> count;
#endif
  c.clear();
  for (size_type i=0; i < count; ++i) {
    T t;
    str >> t;
    c.insert(t);
  }
}
