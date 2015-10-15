/***************************************************************************
 *
 * Implementation for STL-like map based on a hash table
 *
 * $Id: hashmap.cc@#/main/2  08/13/96 21:57:19  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: hashmap.cc,v $
 * Revision 7.6  1996/08/13 21:57:19  griswolf
 * Add support for MSVC4.2 standard lib.
 *
 * Revision 7.5  1995/09/21 00:20:55  griswolf
 * Provide more consistent typedefs.
 *
 * Revision 7.4  1995/09/12  17:14:35  griswolf
 * Move contents to this filename.
 *
 * Revision 7.3  1995/09/06  23:33:45  griswolf
 * Scopus #2144: Check value as well as key for container::operator==.
 *
 * Revision 7.2  1995/09/05  19:36:02  jims
 * *** empty log message ***
 *
 * Revision 7.1  1995/08/29  09:16:50  jims
 * Port to sgi and Borland
 *
 * Revision 1.1  1995/08/28  22:12:03  griswolf
 * Initial revision
 */

#undef FORMALTYPES
#undef TYPES
#ifndef RW_ALLOCATOR_PLAIN
#  define FORMALTYPES class K, class V, class Hash, class EQ, class A
#  define TYPES K,V,Hash,EQ,A
#else
#  define FORMALTYPES class K, class V, class Hash, class EQ
#  define TYPES K,V,Hash,EQ
#endif

template <FORMALTYPES>
rw_hashmap<TYPES>::rw_hashmap(
			  const rw_hashmap<TYPES>::value_type* first,
			  const rw_hashmap<TYPES>::value_type* bound,
			  size_type sz,
			  const Hash& h, const EQ& eq
				 )
	: impl(sz,value_hash_type(h),value_equal_type(eq))
{
  iterator dum;
  while(first != bound)
    impl.insertMaybe(*first++,dum,true);
}

template <FORMALTYPES>
rw_hashmap<TYPES>::rw_hashmap(
			      const_iterator first,
			      const_iterator bound,
			      size_type sz,
			      const Hash& h, const EQ& eq
			     )
	: impl(sz,value_hash_type(h),value_equal_type(eq))
{
  iterator dum;
  impl.insertMaybe(first,bound,dum,true);
}

template <FORMALTYPES>
V& rw_hashmap<TYPES>::operator[](const K& k)
{
  value_type t(k,V());
  return (*((insert(t)).first)).second;
}

#if 0 
// const version of operator[] not implemented for now
template <FORMALTYPES>
const V&
rw_hashmap<TYPES>::operator[](const K& k) const
{
  V ret;
  value_type t(k,V());
  iterator hit = impl.find(k);
  if(hit != end())
    ret = (*hit).second;
  return ret;
}
#endif

template <FORMALTYPES>
bool
rw_hashmap<TYPES>::operator==(const rw_hashmap<TYPES>& m) const
{
  if(size() == m.size())		// must be same size
  {
    const_iterator i = begin();
    while(i != end()) {
      const_iterator j = m.find((*i).first);
      if(j == m.end())			// must have same keys
	return false;
      // note use of op==() below. No relationship to EQ
      if(! ((*j).second == (*i).second)) // and same values
	return false;
      ++i;
    }
    return true;
  }
  return false;
}
    
#if 0
// make inline for Borland 4.5
template <FORMALTYPES>
pair<rw_hashmap<TYPES>::iterator,bool>
rw_hashmap<TYPES>::insert(const value_type& pr)
{
  rw_hashmap<TYPES>::iterator itr;
  bool fits = impl.insertMaybe(pr,itr,true);
  return pair<rw_hashmap<TYPES>::iterator,bool> (itr,fits);
}
#endif

#if 0
// make inline for Borland 4.5
template <FORMALTYPES>
rw_hashmap<TYPES>::iterator
rw_hashmap<TYPES>::insert(
			  iterator, /*ignore*/
			  const rw_hashmap<TYPES>::value_type& k
			 )
{
  rw_hashmap<TYPES>::iterator ret;
  if(! impl.insertMaybe(k,ret,true))
    ret = end();
  return ret;
}
#endif
#undef TYPES
#undef FORMALTYPES
