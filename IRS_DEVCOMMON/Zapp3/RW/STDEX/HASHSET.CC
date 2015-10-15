/***************************************************************************
 *
 * Implementation for STL-like set based on a hash table
 *
 * $Id: hashset.cc@#/main/3  08/20/96 18:42:19  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: hashset.cc,v $
 * Revision 7.6  1996/08/20 18:42:19  griswolf
 * Reverse sense of macro protecting insert(iterator).
 *
 * Revision 7.5  1996/08/13 23:59:30  griswolf
 * Add support for MSVC4.2 standard lib.
 *
 * Revision 7.4  1996/08/09 18:39:52  hart
 * HP (aCC compiler) port
 *
 * Revision 7.3  1995/09/12 18:13:59  griswolf
 * Move contents to this filename.
 *
 * Revision 7.2  1995/09/05  19:38:35  jims
 * Update header section
 *
 * Revision 7.1  1995/08/29  09:16:50  jims
 * Port to sgi and Borland
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
rw_hashset<TYPES>::rw_hashset(
			  const rw_hashset<TYPES>::value_type* first,
			  const rw_hashset<TYPES>::value_type* bound,
			  size_type sz,
			  const Hash& h, const EQ& eq
				 )
	: impl(sz,h,eq)
{
  typename impl_type::Iterator dum;
  while(first != bound)
    impl.insertMaybe(*first++,dum,true);
}

template <FORMALTYPES>
rw_hashset<TYPES>::rw_hashset(
			      const_iterator first,
			      const_iterator bound,
			      size_type sz,
			      const Hash& h, const EQ& eq
			     )
	: impl(sz,h,eq)
{
  typename impl_type::Iterator dum;
  impl.insertMaybe(first,bound,dum,true);
}

#if 0
// make INLINE for Borland 4.5
template <FORMALTYPES>
pair<rw_hashset<TYPES>::iterator,bool>
rw_hashset<TYPES>::insert(rw_hashset<TYPES>::const_reference k)
{
  impl_type::Iterator itr;
  bool fits = impl.insertMaybe(k,itr,true);
  return pair<rw_hashset<TYPES>::iterator,bool> (iterator(itr),fits);
}
#endif

template <FORMALTYPES>
#ifndef RW_NO_TYPEDEF_ITERATOR
typename rw_hashset<TYPES>::iterator
rw_hashset<TYPES>::insert(iterator /*ignore*/, const_reference k)
#else
rw_hashset<TYPES>::Iterator
rw_hashset<TYPES>::insert(Iterator /*ignore*/, const_reference k)
#endif
{
  typename impl_type::iterator ret;
  if(! impl.insertMaybe(k,ret,true))
    ret = impl.end();
  return iterator(ret);
}

#undef TYPES
#undef FORMALTYPES
