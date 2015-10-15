#ifndef __RWSTL_ALGORITHM_H__
#define __RWSTL_ALGORITHM_H__
/***************************************************************************
 *
 * STL-(almost)-compliant algorithms
 * 
 * $Id: algo.h@#/main/4  01/30/97 11:04:34  vobadm (TOOLS707_WIN32_971021)
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
 * 
 * $Log: algo.h,v $
 * Revision 7.1  1996/01/11 23:21:07  jims
 * Move to revision 7.1
 *
 * Revision 1.2  1996/01/05 00:41:55  griswolf
 * Scopus #2160: Fix typo, add the remaining inplace set algorithms.
 *
 */

template <class IIter1, class IIter2, class OIter, class Compare>
void
// depends on the "inplace" container
//   -- being sorted
//   -- remaining sorted after an insert at any iterator
set_union_inplace(
		IIter1 first1, IIter1 bound1,
		IIter2 first2, IIter2 bound2,
		OIter self_inserter,
		Compare comp
	       )
{
  while (first1 != bound1 && first2 != bound2) {
    if (comp(*first1,*first2))
      ++first1;
    else if (comp(*first2,*first1))
      self_inserter = *first2++;
    else {
      ++first1;
      ++first2;
    }
  }
  while (first2 != bound2)
    self_inserter = *first2++;
}

template <class Container, class IIter1, class IIter2, class Compare>
void
// depends on the "inplace" container
//   -- being sorted
//   -- remaining sorted after a deletion at any iterator
set_intersection_inplace(
		Container& con,
		IIter1 first1, IIter1 bound1,
		IIter2 first2, IIter2 bound2,
		Compare comp
	       )
{
  while (first1 != bound1 && first2 != bound2) {
    if (comp(*first1,*first2))
      con.erase(first1++);
    else if (comp(*first2,*first1))
      ++first2;
    else {
      ++first1;
      ++first2;
    }
  }
  con.erase(first1,bound1);
}

template <class Container, class IIter1, class IIter2, class Compare>
void
// depends on the "inplace" container
//   -- being sorted
//   -- remaining sorted after a deletion at any iterator
set_difference_inplace(
		Container& con,
		IIter1 first1, IIter1 bound1,
		IIter2 first2, IIter2 bound2,
		Compare comp
	       )
{
  while (first1 != bound1 && first2 != bound2) {
    if (comp(*first1,*first2))
      ++first1;
    else if (comp(*first2,*first1))
      ++first2;
    else {
      con.erase(first1++);
      ++first2;
    }
  }
}

template <class Container, class IIter1, class IIter2, class Compare>
void
// depends on the "inplace" container
//   -- being sorted
//   -- remaining sorted after a deletion or insertion at any iterator
set_symmetric_difference_inplace(
		Container& con,
		IIter1 first1, IIter1 bound1,
		IIter2 first2, IIter2 bound2,
		Compare comp
	       )
{
  while (first1 != bound1 && first2 != bound2) {
    if (comp(*first1,*first2))
      first1++;
    else if (comp(*first2,*first1))
      con.insert(*first2++);
    else {
      con.erase(first1++);
      ++first2;
    }
  }
  while (first2 != bound2) con.insert(*first2++); 
}

#endif /*__RWSTL_ALGORITHM_H__*/
