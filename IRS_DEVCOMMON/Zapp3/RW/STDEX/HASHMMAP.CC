/***************************************************************************
 *
 * Implementation for STL-like multimap based on a hash table
 *
 * $Id: hashmmap.cc@#/main/5  08/13/96 22:34:05  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: hashmmap.cc,v $
 * Revision 7.9  1996/08/13 22:34:05  griswolf
 * Add support for MSVC4.2 standard lib.
 *
 * Revision 7.8  1996/08/09 18:30:20  hart
 * HP (aCC compiler) port
 *
 * Revision 7.7  1996/03/19 21:58:04  kevinj
 * Bug #2684:  Added comments to operator==.
 *
 * Revision 7.6  1996/01/30 23:00:28  jims
 * Remove initialization of miss in operator== to avoid "assigned a value
 * that's never used" warning
 *
 * Revision 7.5  1995/09/21 00:20:55  griswolf
 * Provide more consistent typedefs.
 *
 * Revision 7.4  1995/09/12  18:08:39  griswolf
 * Move contents to this filename.
 *
 * Revision 7.3  1995/09/06  23:35:09  griswolf
 * Scopus #2144: Check value as well as key for container::operator==.
 *
 * Revision 7.2  1995/09/05  19:37:06  jims
 * Update header section
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
rw_hashmultimap<TYPES>::rw_hashmultimap(
		  const rw_hashmultimap<TYPES>::value_type* first,
		  const rw_hashmultimap<TYPES>::value_type* bound,
		  size_type sz,
		  const Hash& h, const EQ& eq
				 )
	: impl(sz,value_hash_type(h),value_equal_type(eq))
{
  iterator dum;
  while(first != bound)
    impl.insertMaybe(*first++,dum,false);
}

template <FORMALTYPES>
rw_hashmultimap<TYPES>::rw_hashmultimap(
				    const_iterator first,
				    const_iterator bound,
				    size_type sz,
				    const Hash& h, const EQ& eq
				   )
	: impl(sz,value_hash_type(h),value_equal_type(eq))
{
  iterator dum;
  impl.insertMaybe(first,bound,dum,false);
}

template <FORMALTYPES>
bool
rw_hashmultimap<TYPES>::operator==(
  const rw_hashmultimap<TYPES>& other
	) const
{
  if(size() != other.size()) {		// must be same size
    return false;
  }

  // Check this map against other map for equality.
  // Two multimaps are considered equal if they have identical multisets
  // of <key, value> pairs.
  // But the keys may be stored in any order depending on the Hash function.
  // However, for any key, all the values that match
  // that key will be clustered together.

  const_iterator thisIt = begin();           // thisIt iterates over this map.
#ifndef RW_ALLOCATOR_PLAIN
  rw_slist<value_type,allocator<value_type> > otherValueList;
#else
  rw_slist<value_type> otherValueList;
#endif
    // The <key, value> pairs in other map
    // that match a single <key, value> pair in this map.

  while(thisIt != end()) {

    pair<const_iterator,const_iterator> equalRange
      = other.equal_range((*thisIt).first);

    // If a key in this map is not equal to any key in other map
    // then the maps are not equal.
    if(equalRange.first == other.end()){
      return false;
    }

    // Load otherValueList with values from other map.
    // otherValueList must be empty here, see return at bottom of loop
    for(
      const_iterator loadIt = equalRange.first;
      loadIt != equalRange.second; ++loadIt
    ){
      otherValueList.push_back(*loadIt);
    }

    K eqKey = (*thisIt).first;  // Key in this map to check.

    // Check each key in this map with all the values
    // that match this key in the other map.
    while((*thisIt).first == eqKey && otherValueList.size()) {
#ifndef RW_ALLOCATOR_PLAIN
    	typename rw_slist<value_type,allocator<value_type> >::iterator otherValueIt;
#else
    	typename rw_slist<value_type>::iterator otherValueIt;
#endif
	    bool different;
      different = true;  // Must be initialized each pass.

      // Check to see that each candidate has the same value
      // as the pair in this map
    	for(
        otherValueIt = otherValueList.begin();
        otherValueIt != otherValueList.end(); ++otherValueIt
      ) {

	      if((*otherValueIt).second == (*thisIt).second) {
          // Candidate value matches, go to next candidate.
	        otherValueList.erase(otherValueIt);
    	    different = false;
	        break;
	      }
	    } // for

  	  if(different){ 		// Same key has different value so maps not equal.
	      return false;
      }

	  ++thisIt;  // Try next key in this map.

    if(thisIt == end()) {
      break;
    }
    } // while: Checking a key against candidate list.

  // If there are still value candidates left for the other map
  // for this key, then maps not equal.
  // If the end of this map has not been reached and there are still
  // keys to check in this map, then the maps aren't equal.
  if(otherValueList.size() || (thisIt != end() && (*thisIt).first == eqKey)) {
	  return false;
  }

  } // while(thisIt != end())

  return true;

}

#undef TYPES
#undef FORMALTYPES
