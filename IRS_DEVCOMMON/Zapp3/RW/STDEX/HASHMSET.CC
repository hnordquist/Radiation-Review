/***************************************************************************
 *
 * Implementation for STL-like multi set based on a hash table
 *
 * $Id: hashmset.cc@#/main/2  08/13/96 23:13:39  griswolf (TOOLS707_WIN32_971021)
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
 ***************************************************************************
 *
 * $Log: hashmset.cc,v $
 * Revision 7.5  1996/08/13 23:13:39  griswolf
 * Add support for MSVC4.2 standard lib.
 *
 * Revision 7.4  1996/08/09 18:39:12  hart
 * HP (aCC compiler) port
 *
 * Revision 7.3  1995/09/12 18:19:51  griswolf
 * Move contents to this filename.
 *
 * Revision 7.2  1995/09/05  19:38:01  jims
 * Update header section
 *
 * Revision 7.1  1995/08/29  09:16:50  jims
 * Port to sgi and Borland
 *
 * Revision 1.2  1995/08/28  23:18:22  griswolf
 * Fix insert method.
 *
 * Revision 1.1  1995/08/28  22:12:03  griswolf
 * Initial revision
 *
 */

#undef FORMALTYPES
#undef TYPES
#ifndef RW_ALLOCATOR_PLAIN
#  define FORMALTYPES class T, class Hash, class EQ, class A
#  define TYPES T,Hash,EQ,A
#else
#  define FORMALTYPES class T, class Hash, class EQ
#  define TYPES T,Hash,EQ
#endif

template <FORMALTYPES>
rw_hashmultiset<TYPES>::rw_hashmultiset(
			  const rw_hashmultiset<TYPES>::value_type* first,
			  const rw_hashmultiset<TYPES>::value_type* bound,
			  size_type sz,
			  const Hash& h, const EQ& eq
				 )
	: impl(sz,h,eq)
{
  typename impl_type::Iterator dum;
  while(first != bound)
    impl.insertMaybe(*first++,dum,false);
}

template <FORMALTYPES>
rw_hashmultiset<TYPES>::rw_hashmultiset(
				  const_iterator first,
				  const_iterator bound,
				  size_type sz,
				  const Hash& h, const EQ& eq
				 )
	: impl(sz,h,eq)
{
  typename impl_type::Iterator dum;
  impl.insertMaybe(first,bound,dum,false);
}

#if 0
// make INLINE for Borland 4.5
template <FORMALTYPES>
pair<rw_hashmultiset<TYPES>::iterator,bool>
rw_hashmultiset<TYPES>::insert(
		       rw_hashmultiset<TYPES>::const_reference k
			      )
{
  impl_type::Iterator itr;
  bool fits = impl.insertMaybe(k,itr,false);
  return pair<rw_hashmultiset<TYPES>::iterator,bool> (itr,fits);
}
#endif

template <FORMALTYPES>
#ifdef RW_NO_TYPEDEF_ITERATOR
rw_hashmultiset<TYPES>::Iterator
#else
typename rw_hashmultiset<TYPES>::iterator
#endif
rw_hashmultiset<TYPES>::insert(
			      rw_hashmultiset<TYPES>::iterator, /*ignore*/
			      rw_hashmultiset<TYPES>::const_reference k
			     )
{
  typename impl_type::Iterator ret;
  if(! impl.insertMaybe(k,ret,false))
    ret = impl.end();
  return ret;
}

#undef TYPES
#undef FORMALTYPES
