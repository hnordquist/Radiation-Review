#ifndef _STDEX_HASHSET_H_
#define _STDEX_HASHSET_H_
/***************************************************************************
 *
 * declaration for STL-like set based on hash table
 *
 * $Id: hashset.h@#/main/8  09/05/97 15:43:16  kevinj (TOOLS707_WIN32_971021)
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
 */


#include "rw/compiler.h"
#ifndef RW_HEADER_WITH_EXTENSION
#include <memory>
#else
#include <memory.hpp>
#endif /* RW_HEADER_WITH_EXTENSION */

#include <assert.h>
#include "rw/stdex/lhash.h"

#ifndef RW_ALLOCATOR_PLAIN
template <class T, class Hash, class EQ, class A>
#else
template <class T, class Hash, class EQ>
#endif
class rw_hashset  {
public:
///// typdefs /////
  typedef T					key_type;
  typedef T					value_type;
  typedef Hash    				key_hash_type;
  typedef EQ					key_equal_type;
#ifndef RW_ALLOCATOR_PLAIN
  typedef LHashTable<T,Hash,EQ,A>		impl_type;
#else
  typedef LHashTable<T,Hash,EQ>			impl_type;
#endif
  typedef typename impl_type::const_reference	reference;
  typedef typename impl_type::const_reference	const_reference;
  typedef typename impl_type::const_iterator	iterator;
  typedef typename impl_type::const_iterator	const_iterator;
  typedef typename impl_type::size_type		size_type;
  typedef typename impl_type::difference_type	difference_type;
#ifdef RW_NO_TYPEDEF_ITERATOR
  typedef impl_type::const_iterator     	Iterator;
  typedef impl_type::const_iterator	        Const_Iterator;
#endif

  // see "lhash.h" for comments on key_compare:
  typedef typename impl_type::key_compare	key_compare;
  key_compare key_comp()                { return key_compare(); }

  key_hash_type key_hash()		{ return impl.key_hash(); }
  key_equal_type key_equal()		{ return impl.key_equal(); }

///// constructors /////
  rw_hashset(
	     size_type sz=1024,
	     const Hash& h = Hash(), const EQ& eq=EQ()
	     )
    : impl(sz,h,eq) {}

#ifndef RW_ALLOCATOR_PLAIN
  rw_hashset(const rw_hashset<T,Hash,EQ,A>& arg) : impl(arg.impl) {}
#else
  rw_hashset(const rw_hashset<T,Hash,EQ>& arg) : impl(arg.impl) {}
#endif

  rw_hashset(const value_type* first, const value_type* bound,
	     size_type sz=1024, const Hash& h = Hash(), const EQ& eq=EQ()
	     );

  rw_hashset(const_iterator first, const_iterator bound,
	     size_type sz=1024, const Hash& h = Hash(), const EQ& eq=EQ()
	    );

  ~rw_hashset() {} // mainly to document no (non-impl) action

///// operators ////
#ifndef RW_ALLOCATOR_PLAIN
  rw_hashset<T,Hash,EQ,A>&  operator=(const rw_hashset<T,Hash,EQ,A>& t)
#else
  rw_hashset<T,Hash,EQ>&    operator=(const rw_hashset<T,Hash,EQ>& t)
#endif
  { impl = t.impl; return *this; }

  bool
#ifndef RW_ALLOCATOR_PLAIN
    operator==(const rw_hashset<T,Hash,EQ,A>& t) const
#else
    operator==(const rw_hashset<T,Hash,EQ>& t) const
#endif
  { return impl == t.impl; }

/*
 * Not to be used.  Present only to appease overacheiving compilers (the
 * ones that compile all member functions whether used or not).
 */
  bool
#ifndef RW_ALLOCATOR_PLAIN
    operator<(const rw_hashset<T,Hash,EQ,A>&) const
#else
    operator<(const rw_hashset<T,Hash,EQ>&) const
#endif
  { assert(false); return false; }
    
///// various public methods ////
  iterator
    begin() const { return impl.begin(); }

  iterator
    end() const { return impl.end(); }

  bool
    empty() const { return impl.empty(); }

  size_type
    size() const { return impl.size(); }

  size_type
    count(const_reference key) const
    { return impl.find(key) == impl.end() ? 0 : 1; }

  iterator
    find(const_reference key) const { return impl.find(key); }

  iterator
    lower_bound(const_reference key) const { return impl.lower_bound(key); }

  iterator
    upper_bound(const_reference key) const { return impl.upper_bound(key); }

  pair<iterator,iterator>
    equal_range(const_reference key) const { return impl.equal_range(key); }

  size_type
    capacity() const { return impl.capacity(); }

  float
    fill_ratio() const { return impl.fill_ratio(); }

  void
    resize(size_type s) { impl.resize(s); }

  void
#ifndef RW_ALLOCATOR_PLAIN
    swap(rw_hashset<T,Hash,EQ,A>& t) { impl.swap(t.impl); }
#else
    swap(rw_hashset<T,Hash,EQ>& t) { impl.swap(t.impl); }
#endif

///// insertion and erasure ////

  pair<iterator,bool>
    insert(const_reference k)
{
  typename impl_type::Iterator itr;
  bool fits = impl.insertMaybe(k,itr,true);
#ifndef RW_ALLOCATOR_PLAIN
  return pair<rw_hashset<T,Hash,EQ,A>::iterator,bool> (iterator(itr),fits);
#else
  return pair<rw_hashset<T,Hash,EQ>::iterator,bool> (iterator(itr),fits);
#endif
}


  iterator
    insert(iterator hint, const_reference);

  size_type
    insert(const value_type* first, const value_type* bound)
    {typename impl_type::Iterator dum; return impl.insertMaybe(first,bound,dum,true); }

  size_type
    insert(const_iterator first, const_iterator bound)
    {typename  impl_type::Iterator dum; return impl.insertMaybe(first,bound,dum,true); }

  size_type
    erase(const_reference t) { return impl.erase(t); }

  iterator
    erase(iterator it) { return impl.erase(it); }

  iterator
    erase(iterator it, iterator bound) { return impl.erase(it,bound); }

  void clear() { impl.clear(); }

private:
#ifndef RW_ALLOCATOR_PLAIN
  LHashTable<T,Hash,EQ,A> impl;
#else
  LHashTable<T,Hash,EQ> impl;
#endif
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/stdex/hashset.cc"
#endif

#endif /* _STDEX_HASHSET_H_ */
