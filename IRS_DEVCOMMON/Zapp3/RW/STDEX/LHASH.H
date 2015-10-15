#ifndef _STDEX_LHASH_TABLE_H_
#define _STDEX_LHASH_TABLE_H_

/***************************************************************************
 *
 * Hash table implemented as a vector of iterators into an slist, with
 * "chunky" insertion, so that all equal items are adjacent.
 *
 * This class is intended primarily to be used as an underlying
 * implementation. As it is currently written, there are public
 * methods, since that facilitates testing. It may be that the better
 * course is to make all methods protected or private, and then
 * require that interface classes be derived or friends.
 * 
 * There is a subtle change from the "standard meaning" for the methods
 * lower_bound(key) and upper_bound(key).
 * Since hash tables are intrinsically unordered, we have relaxed the 
 * meaning very slightly so that:
 *    lower_bound(key) returns an iterator which points either
 *      "just past" the hash table slot where key would have been but was not
 *         or
 *      the "first" iterator which references something EQ to key.
 *
 *    upper_bound(key) returns an iterator which points either
 *      "just past" the hash table slot where key would have been but was not
 *         or
 *      "just past" the "last" iterator which references something EQ to key
 *
 * Note that all items which are EQ to each other are in consecutive
 * iterator locations. Thus, while equal_range(key) is useful, no other
 * range (except the entire range of the hash table) is likely to be
 * meaningful.
 *
 ***************************************************************************
 *
 * $Id: lhash.h@#/main/20  09/05/97 15:43:22  kevinj (TOOLS707_WIN32_971021)
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
#include <functional>        // stl binary-function
#include <vector>            // stl vector
#include <iterator>          // stl forward_iterator for the tags
#else
#include <memory.hpp>
#include <functional.hpp>        
#include <vector.hpp>            
#include <iterator.hpp>          
#endif  /* RW_HEADER_WITH_EXTENSION */

#include "rw/stdex/slist.h"  // value based slist

// because finding a match requires an "equalitor" the class is
// templatized on "EQ"
// because there must be a hash function, the class is templatized
// on "Hash"

#ifndef RW_ALLOCATOR_PLAIN
template <class T, class Hash, class EQ, class A >
#else
template <class T, class Hash, class EQ >
#endif
class LHashTable 
{
public:
  struct bucket_t {
    bool                        valid_;
#ifndef RW_ALLOCATOR_PLAIN
    typename rw_slist<pair<T,unsigned long>, A >::iterator chain_;
    bucket_t(bool v, const typename rw_slist<pair<T,unsigned long>,A >::iterator& c)
      : valid_(v), chain_(c) {}
    typename rw_slist<pair<T,unsigned long>, A>::iterator& chain() {return chain_;}
    typename rw_slist<pair<T,unsigned long>, A>::iterator chain() const {return chain_;}
#else
    typename rw_slist<pair<T,unsigned long> >::iterator chain_;
    bucket_t(bool v, const typename rw_slist<pair<T,unsigned long> >::iterator& c)
      : valid_(v), chain_(c) {}
    typename rw_slist<pair<T,unsigned long> >::iterator& chain() {return chain_;}
    typename rw_slist<pair<T,unsigned long> >::iterator chain() const {return chain_;}
#endif
    bucket_t() : valid_(false) {}
    operator bool() const { return valid_; }
    bool&       valid() { return valid_; }
  };

///// typdefs /////
  typedef pair<T,unsigned long>               node_t;

  typedef T                                   value_type;
  typedef T&                                  reference;
  typedef const T&                            const_reference;
#ifdef RW_ALLOC_INTERFACE_HACK
  typedef allocator_interface<allocator, rw_slist<node_t> >
                                              list_allocator_t;
  typedef allocator_interface<allocator, vector<bucket_t,allocator> >
                                              vector_allocator_t;
#else
#  ifndef RW_ALLOCATOR_NO_REBIND
    typedef A::rebind<node_t>::other          list_allocator_t;
    typedef A::rebind<bucket_t>::other        vector_allocator_t;
#  else
    typedef allocator<node_t>                 list_allocator_t;
    typedef allocator<bucket_t>               vector_allocator_t;
#  endif
#endif
#ifndef RW_ALLOCATOR_PLAIN
  typedef rw_slist<node_t,list_allocator_t>   list_t;
  typedef vector<bucket_t,vector_allocator_t> table_t;
#else
  typedef rw_slist<node_t>                    list_t;
  typedef vector<bucket_t,allocator>          table_t;
#endif
  typedef typename list_t::size_type          size_type;
  typedef typename list_t::difference_type    difference_type;
  typedef EQ                                  key_equal_type;
  typedef Hash                                key_hash_type;

  ///// nested Iterator classes //////////////////////////////////////////////
                                                                            //
  class ConstIterator;                                                      //
#ifndef RW_ITER_NO_TAGS
  class Iterator : public iterator<forward_iterator_tag,T,difference_type>  //
#else
  class Iterator : public forward_iterator<T,difference_type>               //
#endif
  {
#ifndef RW_ALLOCATOR_PLAIN
    friend LHashTable<T,Hash,EQ,A>;                                         //
#else
    friend class LHashTable<T,Hash,EQ>;                                     //
#endif
    typename list_t::iterator iter_;                                        //
    friend class ConstIterator;                                             //
  public:                                                                   //
    Iterator() {}                                                           //
#ifndef RW_ALLOCATOR_PLAIN
    Iterator(const typename rw_slist<node_t,A>::iterator& i) : iter_(i) {}  // 
#else
    Iterator(const typename rw_slist<node_t>::iterator& i) : iter_(i) {}    // 
#endif
    Iterator(const Iterator& i) : iter_(i.iter_) {}                         //
    ~Iterator() {}                                                          //
                                                                            //
    reference           operator*()  const { return (*iter_).first; }       //    
    Iterator&           operator++()       { ++iter_; return *this; }       //
    Iterator            operator++(int)                                     //
    { Iterator t(*this); ++(*this); return t; }                             //
                                                                            //
    bool                operator==(const Iterator& rhs) const               //
    { return iter_ == rhs.iter_; }                                          //
                                                                            //
    bool                operator==(const ConstIterator& rhs) const          //
    { return rhs == *this; }                                                //
                                                                            //

 //   Dot operator used here to access != to prevent conflict with 
 //   C++ Std. Lib's templated != operator                                      

    bool                operator!=(const Iterator& rhs) const               //
    { return iter_.operator!=(rhs.iter_); }                                 //
                                                                            //
    bool                operator!=(const ConstIterator& rhs) const          //
    { return rhs != *this; }                                                //
                                                                            //
    Iterator&           operator=(const Iterator& rhs)                      //
    { iter_ = rhs.iter_; return *this; }                                    //
  };                                                                        //
                                                                            //
#ifndef RW_ITER_NO_TAGS
  class ConstIterator : public iterator<forward_iterator_tag,T,difference_type>//
#else
  class ConstIterator : public forward_iterator<T,difference_type>          //
#endif
  {
    typename list_t::iterator iter_;                                        //
#ifndef RW_ALLOCATOR_PLAIN
    friend LHashTable<T,Hash,EQ,A>;                                         //
#else
    friend class LHashTable<T,Hash,EQ>;                                     //
#endif
  public:                                                                   //
    ConstIterator() {}                                                      //
#ifndef RW_ALLOCATOR_PLAIN
    ConstIterator(const typename rw_slist<node_t, A>::iterator& i) : iter_(i)        //
#else
    ConstIterator(const typename rw_slist<node_t>::iterator& i) : iter_(i)           //
#endif
    {}                                                                      //
    ConstIterator(const ConstIterator& i) : iter_(i.iter_) {}               //
    ConstIterator(const Iterator& i) : iter_(i.iter_) {}                    //
    ~ConstIterator() {}                                                     //
                                                                            //
    const_reference             operator*() const                           //
    { return (*iter_).first; }                                              //
    ConstIterator&              operator++() { ++iter_; return *this; }     //
    ConstIterator               operator++(int)                             //
    { ConstIterator t(*this); ++(*this); return t; }                        //
                                                                            //
    bool                operator==(const ConstIterator& rhs) const          //
    { return iter_ == rhs.iter_; }                                          //
                                                                            //
    bool                operator==(const Iterator& rhs) const               //
    { return iter_ == rhs.iter_; }                                          //
                                                                            //

 //  Dot operator used here to access != to prevent conflict with 
 //  C++ Std. Lib's templated != operator       

    bool                operator!=(const ConstIterator& rhs) const          //
    { return iter_.operator!=(rhs.iter_); }                                 //


 //  Dot operator used here to access != to prevent conflict with 
 //  C++ Std. Lib's templated != operator       

    bool                operator!=(const Iterator& rhs) const               //
    { return iter_.operator!=(rhs.iter_); }                                 //
                                                                            //
    ConstIterator&              operator=(const ConstIterator& rhs)         //
    { iter_ = rhs.iter_; return *this; }                                    //
                                                                            //
    ConstIterator&              operator=(const Iterator& rhs)              //
    { iter_ = rhs.iter_; return *this; }                                    //
  };                                                                        //
  ///// end of nested Iterator classes ///////////////////////////////////////
      

  typedef Iterator iterator; // users may not alter table directly
  typedef ConstIterator const_iterator;

  /*
   * There is no standard specification for hash tables in the STL,
   * but they are more similar to Associative Containers than anything
   * else.  STL containers are supposed to have a typedef key_compare
   * for their comparitors.  Since items in a hash table are not ordered
   * in that way, we are providing a trivial comparitor that always
   * returns false (from STL's point of view that means everything is
   * equivalent).  This is mainly to guard against over-achieving
   * compilers that insist on compiling even those template functions 
   * that are never used.  This will help in using hashed collections
   * in code that makes use of their commonality of interface with
   * the standard Associative Containers.
   */

   struct degenerative_compare : public binary_function<T, T, bool>
   {
     bool operator() (const T&, const T&) const {

#ifndef __TURBOC__
        return false;
#else
        RWBoolean tmpry = false;
        return tmpry;
#endif

     }
   };

   typedef degenerative_compare key_compare;
   
///// constructors /////
/*
 * Unsolved problem with msvc precludes the default params 
 * commented out below.
 */
  
  LHashTable(size_type   /* sz = 1024 */,
             const Hash& /*h = Hash()*/,
             const EQ&   /*equals = EQ()*/);

#ifndef RW_ALLOCATOR_PLAIN
  LHashTable(const LHashTable<T,Hash,EQ,A>&);
#else
  LHashTable(const LHashTable<T,Hash,EQ>&);
#endif

  LHashTable(ConstIterator first, ConstIterator bound, size_type /*sz=1024*/,
            const Hash& /*h = Hash()*/, const EQ& /*equals = EQ()*/
           );

  LHashTable(const T* first, const T* bound,size_type /*sz=1024*/,
            const Hash& /*h = Hash()*/, const EQ& /*equals = EQ()*/
           );

  ~LHashTable();

///// operators /////
#ifndef RW_ALLOCATOR_PLAIN
  LHashTable<T,Hash,EQ,A>& operator=(const LHashTable<T,Hash,EQ,A>& t);

  bool          operator==(const LHashTable<T,Hash,EQ,A>& t) const
  { return size() == t.size() && testEqual(t); }
#else
  LHashTable<T,Hash,EQ>& operator=(const LHashTable<T,Hash,EQ>& t);
    
  bool          operator==(const LHashTable<T,Hash,EQ>& t) const
  { return size() == t.size() && testEqual(t); }
#endif

///// various public methods /////
  Iterator
    begin()
    { return Iterator(list_->begin()); }

  ConstIterator
    begin() const
    { return ConstIterator(list_->begin()); }

  size_type
    capacity() const { return table_->size(); } // number of buckets

  void
    clear();

  size_type
#ifndef RW_ALLOCATOR_PLAIN
    count(typename LHashTable<T,Hash,EQ,A>::const_reference) const;
#else
    count(typename LHashTable<T,Hash,EQ>::const_reference) const;
#endif
  bool
    empty() const { return size() == 0; }

  Iterator
    end()
    { return Iterator(list_->end()); }

  ConstIterator
    end() const
    { return ConstIterator(list_->end()); }

  pair<Iterator,Iterator>
#ifndef RW_ALLOCATOR_PLAIN
    equal_range(typename LHashTable<T,Hash,EQ,A>::const_reference key)
#else   
    equal_range(typename LHashTable<T,Hash,EQ>::const_reference key)
#endif
{
  typename table_t::iterator slot = index(key);
  typename list_t::iterator lower = list_->end();
  typename list_t::iterator upper = list_->end();
  bool hit;
  if(slot != table_->end() && *slot) {
    lower = chunkyLowerBound((*slot).chain(), list_->end(), key, hit);
    upper = lower;
    if(hit)
      upper = chunkyUpperBound(lower,list_->end(), key);
  }
  return pair<Iterator,Iterator>(lower,upper);
}

  pair<ConstIterator,ConstIterator>
#ifndef RW_ALLOCATOR_PLAIN
    equal_range(typename LHashTable<T,Hash,EQ,A>::const_reference key) const
#else
    equal_range(typename LHashTable<T,Hash,EQ>::const_reference key) const
#endif
{
  typename table_t::const_iterator slot = index(key);
  typename list_t::iterator lower = list_->end();
  typename list_t::iterator upper = list_->end();
  bool hit;
  if(slot != table_->end() && *slot) {
    lower = chunkyLowerBound((*slot).chain(), list_->end(), key, hit);
    upper = lower;
    if(hit)
      upper = chunkyUpperBound(lower,list_->end(), key);
  }
  return pair<ConstIterator,ConstIterator>(lower,upper);
}

#ifndef RW_ALLOCATOR_PLAIN
  typename LHashTable<T,Hash,EQ,A>::size_type
    erase(LHashTable<T,Hash,EQ,A>::const_reference key);
#else
  typename LHashTable<T,Hash,EQ>::size_type
    erase(LHashTable<T,Hash,EQ>::const_reference key);
#endif

  Iterator
    erase(Iterator);    // return successor to erased item

  ConstIterator
    erase(ConstIterator);       // return successor to erased item

  Iterator                      //  return successor to range
    erase(Iterator first, Iterator bound);

  ConstIterator                 //  return successor to range
    erase(ConstIterator first, ConstIterator bound);

  Iterator                      //  return successor to range
    erase(pair<Iterator,Iterator> range)
    { return erase(range.first, range.second); }

  float
    fill_ratio() const { return ((float)size())/((float)capacity()); }

#ifndef RW_ALLOCATOR_PLAIN
  Iterator
    find(typename LHashTable<T,Hash,EQ,A>::const_reference);

  ConstIterator
    find(typename LHashTable<T,Hash,EQ,A>::const_reference) const;

  bool
    insertMaybe(
                typename LHashTable<T,Hash,EQ,A>::const_reference key,
                Iterator& ret, // holds returned iterator
                bool isSingle
                );
#else
    Iterator
    find(typename LHashTable<T,Hash,EQ>::const_reference);

  ConstIterator
    find(typename LHashTable<T,Hash,EQ>::const_reference) const;

  bool
    insertMaybe(
                typename LHashTable<T,Hash,EQ>::const_reference key,
                Iterator& ret, // holds returned iterator
                bool isSingle
                );
#endif

  size_type
    insertMaybe(
                ConstIterator first,
                ConstIterator bound,
                Iterator& ret, // holds returned iterator
                bool isSingle
                )
  {
    size_type count(0);
    while(first != bound)  {
      count += (insertMaybe(*first,ret,isSingle)) ? 1 : 0;
      ++first;
    }
    return count;
  }

  size_type
    insertMaybe(
                const T* first,
                const T* bound,
                Iterator& ret, // holds returned iterator
                bool isSingle
                )
  {
    size_type count(0);
    while(first != bound) 
      count += (insertMaybe(*first++,ret,isSingle)) ? 1 : 0;
    return count;
  }

#ifndef RW_ALLOCATOR_PLAIN
  Iterator
    lower_bound(typename LHashTable<T,Hash,EQ,A>::const_reference);

  ConstIterator
    lower_bound(typename LHashTable<T,Hash,EQ,A>::const_reference) const;

//  resize() fills a new table with data, rehashing everything, 
//  except that resize(0) is a no-op.
  void
    resize(typename LHashTable<T,Hash,EQ,A>::size_type);
#else
    Iterator
    lower_bound(typename LHashTable<T,Hash,EQ>::const_reference);

  ConstIterator
    lower_bound(typename LHashTable<T,Hash,EQ>::const_reference) const;

//  resize() fills a new table with data, rehashing everything, 
//  except that resize(0) is a no-op.
  void
    resize(typename LHashTable<T,Hash,EQ>::size_type);
#endif

  size_type
    size() const { return list_->size(); }

#ifndef RW_ALLOCATOR_PLAIN
  void
    swap(LHashTable<T,Hash,EQ,A>& t);

  Iterator
    upper_bound(typename LHashTable<T,Hash,EQ,A>::const_reference);

  ConstIterator
    upper_bound(typename LHashTable<T,Hash,EQ,A>::const_reference) const;
#else
  void
    swap(LHashTable<T,Hash,EQ>& t);
  Iterator
    upper_bound(typename LHashTable<T,Hash,EQ>::const_reference);

  ConstIterator
    upper_bound(typename LHashTable<T,Hash,EQ>::const_reference) const;
#endif

  key_hash_type
    key_hash() const { return hash_; }

  key_equal_type
    key_equal() const { return eq_; }

//////////////////////////////////////////////////////////////////////////
///// protected and private methods //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

protected:

  typename list_t::iterator
    chunkyLowerBound(
                     typename list_t::iterator start,
                     typename list_t::iterator bound,
                     const_reference key,
                     bool& hit
                    ) const
{
  hit = false;
  if(0 == capacity())
    return bound;
  // else
  size_type slot = ((*start).second) % capacity();
  while(start != bound && ((*start).second)%capacity() == slot) {
    if(eq_((*start).first,key)) {
      hit = true;
      break;
    }
    ++start;
  }
  return start;
}

  typename list_t::iterator
    chunkyUpperBound(
                     typename list_t::iterator start,
                     typename list_t::iterator bound,
                     const_reference key
                    ) const
{
  if(0 == capacity())
    return bound;
  // else
  size_type slot = ((*start).second) % capacity();
  bool dum;
  start = chunkyLowerBound(start, bound, key, dum);

  while(start != bound && ((*start).second)%capacity() == slot) {
    if(! eq_((*start).first,key))
      break;
    ++start;
  }
  return start;
}

  typename table_t::iterator
    index(unsigned long hv) const
    {
      if(0 == capacity())
        return table_->end();
      else
        return table_->begin() + (hv % capacity());
    }

  typename table_t::iterator
    index(const_reference t) const    { return index(hash_(t)); }

#if __SUNPRO_CC!=0x420 &&  !defined(RW_NO_STL) 
#ifndef RW_ALLOCATOR_PLAIN
  bool
    testEqual(const LHashTable<T,Hash,EQ,A>& t) const;
#else
  bool
    testEqual(const LHashTable<T,Hash,EQ>& t) const;
#endif
#else
 bool testEqual(const LHashTable<T,Hash,EQ>& t) const
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
#endif /* !SunPro*/

private:
  list_allocator_t                        listAllocator;
  vector_allocator_t                      vectorAllocator;
#ifndef RW_ALLOCATOR_PLAIN
  rw_slist<node_t,list_allocator_t>       *list_;
  vector<bucket_t,vector_allocator_t>     *table_;
#else
  rw_slist<node_t>                        *list_;
  vector<bucket_t,allocator>              *table_;
#endif
  EQ                                      eq_;
  Hash                                    hash_;
};
#ifdef RW_COMPILE_INSTANTIATE
#include "rw/stdex/lhash.cc"
#endif

#endif /* _STDEX_LHASH_TABLE_H_ */

