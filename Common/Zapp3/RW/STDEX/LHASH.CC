/***************************************************************************
 *
 * implement hash table of iterators into an slist
 *
 * $Id: lhash.cc@#/main/13  09/05/97 15:43:20  kevinj (TOOLS707_WIN32_971021)
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

#undef FORMALTYPES
#undef TYPES
#ifndef RW_ALLOCATOR_PLAIN
#  define FORMALTYPES class T, class Hash, class EQ, class A
#  define TYPES T,Hash,EQ,A
#else
#  define FORMALTYPES class T, class Hash, class EQ
#  define TYPES T,Hash,EQ
#endif

#undef VECSIZE
#define VECSIZE sizeof(vector<bucket_t,allocator>)
#undef LISTSIZE
#define LISTSIZE sizeof(rw_slist<node_t>)

#include "rw/stdex/destroy.h"

template <FORMALTYPES>
LHashTable<TYPES>::LHashTable(
			      typename LHashTable<TYPES>::size_type sz,
			      const Hash& h, const EQ& equals
			     )
: table_(0), list_(0), hash_(h), eq_(equals)
{
// use placement new...
#ifndef RW_ALLOCATOR_PLAIN
  list_ =
    new (RW_ALLOC_TYPE(list_t,listAllocator,listAllocator,1,0))
	 rw_slist<node_t, list_allocator_t>;
  table_ =
    new (RW_ALLOC_TYPE(table_t,vectorAllocator,vectorAllocator,1,0))
	  vector<bucket_t,vector_allocator_t> (sz); 
#else
  list_ =
    new (RW_ALLOC_TYPE(rw_slist<node_t>,listAllocator,listAllocator,LISTSIZE,0))
	 rw_slist<node_t>;
  table_ =
    new (RW_ALLOC_TYPE(table_t,vectorAllocator,vectorAllocator,VECSIZE,0))
	  vector<bucket_t,allocator> (sz); 
#endif
}


template <FORMALTYPES>
LHashTable<TYPES>::LHashTable(
			      ConstIterator first, ConstIterator bound,
			      size_type sz, const Hash& h, const EQ& equals
			     )
: table_(0), list_(0), hash_(h), eq_(equals)
{
// use placement new...
#ifndef RW_ALLOCATOR_PLAIN
  list_ =
    new (RW_ALLOC_TYPE(list_t,listAllocator,listAllocator,1,0))
	 rw_slist<node_t,list_allocator_t>;
  table_ =
    new (RW_ALLOC_TYPE(table_t,vectorAllocator,vectorAllocator,1,0))
	  vector<bucket_t,vector_allocator_t> (sz); 
#else
  list_ =
    new (RW_ALLOC_TYPE(rw_slist<node_t>,listAllocator,listAllocator,LISTSIZE,0))
	 rw_slist<node_t>;
  table_ =
    new (RW_ALLOC_TYPE(table_t,vectorAllocator,vectorAllocator,VECSIZE,0))
	  vector<bucket_t,allocator> (sz); 
#endif
  Iterator dum;
  insertMaybe(first,bound,dum,false); // assume donor was multi
}

template <FORMALTYPES>
LHashTable<TYPES>::LHashTable(
			      const T* first, const T* bound,
			      size_type sz, const Hash& h, const EQ& equals
			     )
: table_(0), list_(0), hash_(h), eq_(equals)
{
// use placement new...
#ifndef RW_ALLOCATOR_PLAIN
  list_ =
    new (RW_ALLOC_TYPE(list_t,listAllocator,listAllocator,1,0))
	 rw_slist<node_t,list_allocator_t>;
  table_ =
    new (RW_ALLOC_TYPE(table_t,vectorAllocator,vectorAllocator,1,0))
	  vector<bucket_t,vector_allocator_t> (sz); 
#else
  list_ =
    new (RW_ALLOC_TYPE(rw_slist<node_t>,listAllocator,listAllocator,LISTSIZE,0))
	 rw_slist<node_t>;
  table_ =
    new (RW_ALLOC_TYPE(table_t,vectorAllocator,vectorAllocator,VECSIZE,0))
	  vector<bucket_t,allocator> (sz); 
#endif
  Iterator dum;
  insertMaybe(first,bound,dum,false); // assume donor was multi
}

template <FORMALTYPES>
LHashTable<TYPES>::LHashTable(const LHashTable<TYPES>& t)
: table_(0), list_(0), hash_(t.hash_), eq_(t.eq_)
{
#ifndef RW_ALLOCATOR_PLAIN
  list_ =
    new (RW_ALLOC_TYPE(list_t,listAllocator,listAllocator,1,0))
	 rw_slist<node_t,list_allocator_t>;
  table_ =
    new (RW_ALLOC_TYPE(table_t,vectorAllocator,vectorAllocator,1,0))
	  vector<bucket_t,vector_allocator_t> (t.capacity()); 
#else
  list_ =
    new (RW_ALLOC_TYPE(rw_slist<node_t>,listAllocator,listAllocator,LISTSIZE,0))
	 rw_slist<node_t>;
  table_ =
    new (RW_ALLOC_TYPE(table_t,vectorAllocator,vectorAllocator,VECSIZE,0))
	  vector<bucket_t,allocator> (t.capacity()); 
#endif
  Iterator dum;
  insertMaybe(t.begin(),t.end(),dum,false);
 }

template <FORMALTYPES>
LHashTable<TYPES>::~LHashTable()
{
  clear();
  rw_destroy(table_);
  RW_DEALLOC_TYPE(vectorAllocator,vectorAllocator,table_);
  rw_destroy(list_);
  RW_DEALLOC_TYPE(listAllocator,listAllocator,list_);
}

template <FORMALTYPES>
LHashTable<TYPES>&
LHashTable<TYPES>::operator=(const LHashTable<TYPES>& t)
{
  if(this != &t) {
    clear();
    if(capacity() > t.capacity()) {
      /* destroy(table_); */
#ifndef RW_ALLOCATOR_PLAIN 
      table_->vector<bucket_t,vector_allocator_t>::~vector();
#else
      table_->vector<bucket_t,allocator>::~vector();
#endif
      RW_DEALLOC_TYPE(vectorAllocator,vectorAllocator,table_);
#ifndef RW_ALLOCATOR_PLAIN
      table_ = new (RW_ALLOC_TYPE(bucket_t,vectorAllocator,vectorAllocator,1,0))
	vector<bucket_t,vector_allocator_t>(t.capacity());
#else
      table_ = new (RW_ALLOC_TYPE(bucket_t,vectorAllocator,vectorAllocator,VECSIZE,0))
	vector<bucket_t,allocator>(t.capacity());
#endif
    }
    else {
      table_->reserve(t.capacity());
      bucket_t dum;
      while(table_->size() < t.capacity())
	table_->push_back(dum);
    }
    Iterator dum;
    insertMaybe(t.begin(),t.end(),dum,false);
  }
  return *this;
}

template <FORMALTYPES> void
LHashTable<TYPES>::clear()
{
  list_->erase(list_->begin(),list_->end());
  typename table_t::iterator kill = table_->begin();
  while(kill != table_->end()) {
    (*kill).valid() = false;
    ++kill;
  }
}
#if __SUNPRO_CC!=0x420 && !defined(RW_NO_STL)
template <FORMALTYPES>
bool
LHashTable<TYPES>::testEqual(const LHashTable<TYPES>& t) const
{
/** precondition(size() == t.size()) **/
  ConstIterator i = begin();
  
  while(i != end()) {
    size_type num = t.count(*i);
    if(num != count(*i)) {
      return false;
    }
    else {
      while(num-- ) ++i;
    }
  }
  return true;
}
#endif /* !SunPro */
template <FORMALTYPES>
typename LHashTable<TYPES>::size_type
LHashTable<TYPES>::erase(typename LHashTable<TYPES>::const_reference key)
{
  size_type ret = size();
  erase(equal_range(key));
  return ret - size();
}

template <FORMALTYPES>
typename LHashTable<TYPES>::Iterator
LHashTable<TYPES>::erase(typename LHashTable<TYPES>::Iterator iter)
{
  if(iter == end())
    return iter;
  // else
  typename list_t::iterator kill = iter.iter_;
  typename list_t::iterator killnext = kill; ++killnext;
  typename table_t::iterator ti = index((*kill).second);
  typename list_t::iterator slot = (*ti).chain();
  size_type kbucket = (*kill).second%capacity();
  if(slot == kill) // erasing first item in slot
  {
    // does slot have another item after us?
    if(killnext != list_->end() && (*killnext).second%capacity() == kbucket)
    {
      (*ti).chain() = killnext = list_->erase(kill);
    }
    else // slot had only one item. invalidate slot, fixup next in list.
    {
      (*ti).valid() = false;
      killnext = list_->erase(kill);
      if(killnext != list_->end()) {
	(*index((*killnext).second)).chain() = killnext;
      }
    }
  }
  else // erasing item not first in slot
  {
    // does slot have another item after us?
    if(killnext != list_->end() && (*killnext).second%capacity() == kbucket) 
    {
      killnext = list_->erase(kill);
    }
    else // erasing last item in slot. fixup next
    {
      killnext = list_->erase(kill);
      if(killnext != list_->end()) {
	(*index((*killnext).second)).chain() = killnext;
      }
    }
  }
  return killnext;
}

template <FORMALTYPES>
typename LHashTable<TYPES>::ConstIterator
LHashTable<TYPES>::erase(typename LHashTable<TYPES>::ConstIterator iter)
{
  if(iter == ((const LHashTable<TYPES>*)this)->end())
    return iter;
  // else
  typename list_t::iterator kill = iter.iter_;
  typename list_t::iterator killnext = kill; ++killnext;
  typename table_t::iterator ti = index((*kill).second);
  typename list_t::iterator slot = (*ti).chain();
  size_type kbucket = (*kill).second%capacity();
  if(slot == kill) // erasing first item in slot
  {
    // does slot have another item after us?
    if(killnext != list_->end() && (*killnext).second%capacity() == kbucket)
    {
      (*ti).chain() = killnext = list_->erase(kill);
    }
    else // slot had only one item. invalidate slot, fixup next in list.
    {
      (*ti).valid() = false;
      killnext = list_->erase(kill);
      if(killnext != list_->end()) {
	(*index((*killnext).second)).chain() = killnext;
      }
    }
  }
  else // erasing item not first in slot
  {
    // does slot have another item after us?
    if(killnext != list_->end() && (*killnext).second%capacity() == kbucket) 
    {
      killnext = list_->erase(kill);
    }
    else // erasing last item in slot. fixup next
    {
      killnext = list_->erase(kill);
      if(killnext != list_->end()) {
	(*index((*killnext).second)).chain() = killnext;
      }
    }
  }
  return killnext;
}

template <FORMALTYPES>
typename LHashTable<TYPES>::Iterator
LHashTable<TYPES>::erase(
			 typename LHashTable<TYPES>::Iterator first,
			 typename LHashTable<TYPES>::Iterator bound
			)
{
  Iterator iter;
  
  if (first != bound) {
    if(bound == end()) {
      while(first != end()) {
	first = erase(first);
      }
    }
    else {
      /*
       * erasing the element to the left of bound invalidates bound,
       * so we have to look ahead and do one last erase after the while loop.
       */
      iter = first;
      ++iter;
      while(iter != bound) {
	first = erase(first);
	iter = first;
	++iter;
      }
      first = erase(first);
    }
  }
  return first;
}

template <FORMALTYPES>
typename LHashTable<TYPES>::ConstIterator
LHashTable<TYPES>::erase(
			 typename LHashTable<TYPES>::ConstIterator first,
			 typename LHashTable<TYPES>::ConstIterator bound
			)
{
  ConstIterator iter;
  
  if (first != bound) {
    if(bound == end()) {
      while(first != end()) {
	first = erase(first);
      }
    }
    else {
      /*
       * erasing the element to the left of bound invalidates bound,
       * so we have to look ahead and do one last erase after the while loop.
       */
      iter = first;
      ++iter;
      while(iter != bound) {
	first = erase(first);
	iter = first;
	++iter;
      }
      first = erase(first);
    }
  }
  return first;
}

template <FORMALTYPES>
typename LHashTable<TYPES>::size_type
LHashTable<TYPES>::count(typename LHashTable<TYPES>::const_reference key) const
{
  size_type ret = 0;
  typename table_t::const_iterator slot = index(key);
  if(*slot) {
    bool found;
    typename list_t::iterator it =
      chunkyLowerBound((*slot).chain(), list_->end(), key, found);
    if(found) {
      while((it != list_->end() && eq_((*it).first,key))) {
	++ret;
	++it;
      }
    }
  }
  return ret;
}

template <FORMALTYPES>
typename LHashTable<TYPES>::Iterator
LHashTable<TYPES>::find(typename LHashTable<TYPES>::const_reference key)
{
  Iterator ret = end();
  typename table_t::iterator slot = index(key);
  typename list_t::iterator i;
  if(*slot) {
    bool found;
    i =  chunkyLowerBound((*slot).chain(), list_->end(), key, found);
    if(found)
      ret = i;
  }
  return ret;
}

template <FORMALTYPES>
typename LHashTable<TYPES>::ConstIterator
LHashTable<TYPES>::find(typename LHashTable<TYPES>::const_reference key) const
{
  ConstIterator ret = end();
  typename table_t::const_iterator slot = index(key);
  typename list_t::iterator i;
  if(*slot) {
    bool found;
    i = chunkyLowerBound((*slot).chain(), list_->end(), key, found);
    if(found)
      ret = ConstIterator(i);
  }
  return ret;
}

template <FORMALTYPES>
typename LHashTable<TYPES>::Iterator
LHashTable<TYPES>::lower_bound(
		       typename LHashTable<TYPES>::const_reference key
			      )
{
  Iterator ret = end();
  typename table_t::iterator slot = index(key);
  if(*slot) {
    bool dum;
    ret = chunkyLowerBound((*slot).chain(), list_->end(), key, dum);
  }
  return ret;
}

template <FORMALTYPES>
typename LHashTable<TYPES>::ConstIterator
LHashTable<TYPES>::lower_bound(
		       typename LHashTable<TYPES>::const_reference key
			      ) const
{
  ConstIterator ret = end();
  typename table_t::iterator slot = index(key);
  if(*slot) {
    bool dum;
    ret =
      ConstIterator(chunkyLowerBound((*slot).chain(), list_->end(), key, dum));
  }
  return ret;
}

template <FORMALTYPES>
typename LHashTable<TYPES>::Iterator
LHashTable<TYPES>::upper_bound(
		       typename LHashTable<TYPES>::const_reference key
			      )
{
  Iterator ret = end();
  typename table_t::iterator slot = index(key);
  if(*slot) {
    ret = chunkyUpperBound((*slot).chain(), list_->end(), key);
  }
  return ret;
}

template <FORMALTYPES>
typename LHashTable<TYPES>::ConstIterator
LHashTable<TYPES>::upper_bound(
		       typename LHashTable<TYPES>::const_reference key
			      ) const
{
  ConstIterator ret = end();
  typename table_t::iterator slot = index(key);
  if(*slot) {
    ret =
      ConstIterator(chunkyUpperBound((*slot).chain(), list_->end(), key));
  }
  return ret;
}

#if 0
template <FORMALTYPES>
pair<LHashTable<TYPES>::ConstIterator,
     LHashTable<TYPES>::ConstIterator> 
LHashTable<TYPES>::equal_range(
			 LHashTable<TYPES>::const_reference key
			      ) const
{
  table_t::const_iterator slot = index(key);
  list_t::iterator lower = list_->end();
  list_t::iterator upper = list_->end();
  bool hit;
  if(*slot) {
    lower = chunkyLowerBound((*slot).chain(), list_->end(), key, hit);
    upper = lower;
    if(hit)
      upper = chunkyUpperBound(lower,list_->end(), key);
  }
  return pair<ConstIterator,ConstIterator>(lower,upper);
}

template <FORMALTYPES>
pair<LHashTable<TYPES>::Iterator,
     LHashTable<TYPES>::Iterator> 
LHashTable<TYPES>::equal_range(
		       LHashTable<TYPES>::const_reference key
			      )
{
  table_t::iterator slot = index(key);
  list_t::iterator lower = list_->end();
  list_t::iterator upper = list_->end();
  bool hit;
  if(*slot) {
    lower = chunkyLowerBound((*slot).chain(), list_->end(), key, hit);
    upper = lower;
    if(hit)
      upper = chunkyUpperBound(lower,list_->end(), key);
  }
  return pair<Iterator,Iterator>(lower,upper);
}
#endif

template <FORMALTYPES>
bool
LHashTable<TYPES>::insertMaybe(
		     typename LHashTable<TYPES>::const_reference key,
		     Iterator& ret,
		     bool single /* only one == key, or multiple? */
				 )
{
  if(0 == table_->capacity())
    return false;
  // else
  unsigned long hv = hash_(key);
  typename table_t::iterator idx = index(hv);
  if(! *idx) // This bucket has never been used. Do it easy.
  {
    (*idx).valid() = true;
    (*idx).chain() = list_->insert(list_->end(),node_t(key,hv));
    ret = Iterator((*idx).chain());
  }
  else // This bucket is in use. Be thoughtful.
  {
    bool hit;
    typename list_t::iterator it;
    it = chunkyLowerBound((*idx).chain(), list_->end(), key, hit);
    ret = Iterator(it);
    if(single && hit) // then we won't insert
    {
      return false;
    }
    else // either not found or multiple inserts okay
    {
      // tricky: inserting at lower bound, if we had hit, is okay because
      // even if the slot iterator points at this lower bound, we are
      // inserting an eq_ key. If we had no hit, then lower bound ==
      // upper bound, and it references the first node in another slot
      // (or end()). In either of those cases, we will have to adjust 
      // the iterator in that new slot after the insert, because a
      // property of slist iterators is that inserting just prior to 
      // the node they reference invalidates them.

      it = list_->insert(it,node_t(key,hv));
      ret = Iterator(it);

      // now do iterator fixup, if needed
      if(++it != list_->end()) {
	unsigned long nexthv = (*it).second;
	if(nexthv%capacity() != hv%capacity()) {
	  (*index(nexthv)).chain() = it;
	}			// if fixup needed
      }				// if not at end during fixup
    }				// if single && hit (else)
  }				// if bucket empty (else)
  return true;
}

template <FORMALTYPES>
void
LHashTable<TYPES>::resize(typename LHashTable<TYPES>::size_type n)
{
  if(n == 0 || n == capacity()) return; 	// do only if changed
  LHashTable<TYPES> tmp(this->begin(),this->end(),
			    n,			// new size
			    hash_,eq_);
  swap(tmp);
  /** postconditions: new capacity() is n;
   ** every element is still here
   ** this == old_this
   **/
}

template <FORMALTYPES>
void
LHashTable<TYPES>::swap(LHashTable<TYPES>& t)
{
  ::swap(list_,t.list_);
  ::swap(table_,t.table_);
  ::swap(eq_,t.eq_);
  ::swap(hash_,t.hash_);
}

#undef FORMALTYPES
#undef TYPES
