#ifndef __STDEX_SLIST_H__
#define __STDEX_SLIST_H__

/***************************************************************************
 *
 * STL Sequence-compliant singly-linked list
 * 
 * $Id: slist.h@#/main/9  09/05/97 15:43:25  kevinj (TOOLS707_WIN32_971021)
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
#include <memory>
#include <iterator>
#include <algorithm> // equal filter
#else
#include <memory.hpp>
#include <iterator.hpp>
#include <algorithm.hpp> 
#endif /* RW_HEADER_WITH_EXTENSION */

#ifndef RW_ALLOCATOR_PLAIN
template <class T, class A>
#else
template <class T>
#endif
class rw_slist {
// internal class predeclarations
protected:
  struct Node;
public:
  class Iterator;
  class ConstIterator;
// friendships
friend class Iterator;
friend class ConstIterator;
protected:
  //////// scoped Node class /////////////////////////////////////////////////
  struct Node {                                                             //
    // data                                                                 //
    Node        *next;                                                      //
    T           item_;                                                      //
    // methods                                                              //
                                                                            //
    Node(const T& t, Node* nx) : item_(t), next(nx) {}                      //
    /* void     init(const T&t, Node* nx) {item_=t;next=nx;} */             //
                                                                            //
    Node**      mynext() { return &next; }                                  //
    T&          item() { return item_; }                                    //
    const T&    item() const { return item_; }                              //
  };                                                                        //
                                                                            //
  //////// end of scoped Node class //////////////////////////////////////////

public:      
// typedefs
  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
#ifndef RW_ALLOCATOR_PLAIN
  typedef A allocator_type;
  typedef A::difference_type difference_type;
  typedef A::size_type size_type;
#else
  typedef allocator allocator_type;
  typedef allocator::difference_type difference_type;
  typedef allocator::size_type size_type;
#endif

  //////// scoped iterator classes ///////////////////////////////////////////
                                                                            //
  class Iterator : public                                                   //
#ifndef RW_ITER_NO_TAGS
        iterator<forward_iterator_tag, T, allocator_type::difference_type> {  //
#else
        forward_iterator<T, allocator_type::difference_type> {              //
#endif
  friend class ConstIterator;                                               //
  public:                                                                   //
    Iterator(const Iterator& i) : prelink_(i.prelink()) {}                  //
    Iterator&           operator=(const Iterator& i)                        //
          { prelink() = i.prelink(); return *this; }                        //
                                                                            //
    bool                operator==(const Iterator& i) const                 //
                { return *prelink() == *i.prelink(); }                      //
                                                                            //
    bool                operator!=(const Iterator& i) const                 //
                                                                            //
                { return *prelink() != *i.prelink(); }                      //
                                                                            //
    T&          operator*() const { return (*prelink())->item(); }          //
                                                                            //
    Iterator&           operator++()            //++iter                    //
        { prelink() = (*prelink())->mynext(); return *this; }               //
    Iterator            operator++(int)         //iter++                    //
        { Node** tmp = prelink(); ++(*this); return Iterator(tmp); }        //
    // The following causes ambiguities under bcc45 for mysterious reasons: //
    //    operator bool() const { return isValid(); }                       //
    bool                isValid() const                                     //
        { return prelink() != 0 && *prelink() != 0; }                       //
                                                                            //
    Iterator()          : prelink_(0) {}                                    //
    Iterator(Node** spot) : prelink_(spot) {}                               //
    Node**              prelink() const { return prelink_; }                //
    Node**&             prelink() { return prelink_; }                      //
    bool                reset()                                             //
        { bool ret = isValid(); prelink_ = 0; return ret; }                 //
  private:                                                                  //
    Node**      prelink_;                                                   //
  };                                                                        //
                                                                            //
class ConstIterator : public                                                //
#ifndef RW_ITER_NO_TAGS
        iterator<forward_iterator_tag, T, allocator_type::difference_type> {  //
#else
        forward_iterator<T, allocator_type::difference_type> {              //
#endif
  friend class Iterator;                                                    //
  public:                                                                   //
    ConstIterator(const ConstIterator& i) : prelink_(i.prelink()) {}        //
    ConstIterator(const Iterator& i) : prelink_(i.prelink()) {}             //
    ConstIterator&              operator=(const ConstIterator& i)           //
          { prelink() = i.prelink(); return *this; }                        //
    ConstIterator&              operator=(const Iterator& i)                //
          { prelink() = i.prelink(); return *this; }                        //
                                                                            //
    bool                operator==(const ConstIterator& i) const            //
                { return *prelink() == *i.prelink(); }                      //
    bool                operator!=(const ConstIterator& i) const            //
                { return *prelink() != *i.prelink(); }                      //
                                                                            //
    const T&            operator*() const { return (*prelink())->item(); }  //
                                                                            //
    ConstIterator&              operator++()            //++iter            //
        { prelink() = (*prelink())->mynext(); return *this; }               //
    ConstIterator               operator++(int)         //iter++            //
        { Node** tmp = prelink(); ++(*this); return ConstIterator(tmp); }   //
                                                                            //
    operator bool() const { return isValid(); }                             //
    bool                isValid() const                                     //
        { return prelink() != 0 && *prelink() != 0; }                       //
                                                                            //
    ConstIterator()             : prelink_(0) {}                            //
    ConstIterator(Node** spot) : prelink_(spot) {}                          //
    Node**              prelink() const { return prelink_; }                //
    Node**&             prelink() { return prelink_; }                      //
    bool                reset()                                             //
        { bool ret = isValid(); prelink_ = 0; return ret; }                 //
  private:                                                                  //
    Node**      prelink_;                                                   //
  };                                                                        //
  /////// end of scoped iterator classes /////////////////////////////////////

  typedef Iterator iterator;
  typedef ConstIterator const_iterator;

// allocation deallocation and assignment
  rw_slist();
  // rw_slist(size_type, const T& value = T()); /* don't require def ctor */
  rw_slist(size_type, const T& value);
#ifndef RW_ALLOCATOR_PLAIN
  rw_slist(const rw_slist<T,A>&);
#else
  rw_slist(const rw_slist<T>&);
#endif
  rw_slist(ConstIterator begin, ConstIterator end); 
  rw_slist(const T* begin, const T* end); 
  ~rw_slist();
#ifndef RW_ALLOCATOR_PLAIN
  rw_slist<T,A>& operator=(const rw_slist<T,A>&);
  void swap(rw_slist<T,A>&);
#else
  rw_slist<T>& operator=(const rw_slist<T>&);
  void swap(rw_slist<T>&);
#endif

// accessors

  iterator begin()
        { return iterator(head_); }
  const_iterator begin() const
        { return const_iterator(head_); }
  iterator end()
        { return iterator(tail_); }
  const_iterator end() const
        { return const_iterator(tail_); }
  bool empty() const
        { return 0 == entries_; }
  size_type size() const
        { return entries_; }
  size_type max_size() const; /** use todd\'s templatized stuff? **/
  /** for the moment leave it undefined **/
  /** or use Alex's Allocator::max_size() which doesn't seem right
   ** since it limits us to one allocation buffer  **/
  T& front()
        { return *(begin()); }
  const T& front() const
        { return *(begin()); }
  T& back()  
/**     { return (*((dynamic_cast<Node*>)tail_))->item(); }
 * warning: this is a re-interpretation cast from a pointer to the
 * (first item in  a struct) to a pointer to the struct itself
 **/
        { return ((Node*)tail_)->item(); }
  const T& back() const 
/**{ return ((dynamic_cast<Node*>)tail_)->item(); }
 * same warning for the const version too
 **/
        { return ((Node*)tail_)->item(); }
// insert/erase
  void push_front(const T& t)
        { insert(begin(), t); }
  void push_back(const T& t)
        { insert(end(), t); }

  // don't require def ctor:
  // iterator insert(iterator loc, const T& val = T()); 
  iterator insert(iterator loc, const T& val);

  void insert(iterator, const_iterator, const_iterator);
  void insert(iterator, const T*, const T*);

  // don't require def ctor:
  // void insert(iterator, size_type, const T& value = T());
  void insert(iterator, size_type, const T& value);

  void pop_front()
        { erase(begin()); }
  iterator erase(iterator);
  iterator erase(iterator, iterator);

// special slist mutators
#ifndef RW_ALLOCATOR_PLAIN
  void splice(iterator pos, rw_slist<T,A> &donor)
        { cutPaste(pos,donor.size(),donor,donor.begin(),donor.end()); }
  void splice(iterator pos, rw_slist<T,A>& donor, iterator item);
  void splice(iterator pos, rw_slist<T,A>& donor,iterator begin, iterator end);
#else
  void splice(iterator pos, rw_slist<T> &donor)
        { cutPaste(pos,donor.size(),donor,donor.begin(),donor.end()); }
  void splice(iterator pos, rw_slist<T>& donor, iterator item);
  void splice(iterator pos, rw_slist<T>& donor,iterator begin, iterator end);
#endif
  void remove(const T& val);
/**  template <class UnaryPredicate>
 **  void remove_if(UnaryPredicate<T,bool>&);
 **/

  void unique(); // assumes equal elements are adjacent

/**  template <class BinaryPredicate>
 **  void unique(BinaryPredicate<T,T,bool>&);
 **/

#ifndef RW_ALLOCATOR_PLAIN
  void merge(rw_slist<T,A>&);
#else
  void merge(rw_slist<T>&);
#endif

/**  template <class Compare>
 **  void merge(rw_slist<T>&, Comparison<T>&);
 **/

  void reverse();
  void sort();

/**  tempate<class Compare>
 **  void sort(Comparison<T>&);
 **/

// RW internal debugging methods
#if 0
public:
  void showOff(const char *s= "LIST") const;
  void iprint(const char *p,const_iterator i,const char* t) const;
#endif
protected:
#ifndef RW_ALLOCATOR_PLAIN
  iterator cutPaste(iterator,size_type,rw_slist<T,A>&,iterator,iterator);
#else
  iterator cutPaste(iterator,size_type,rw_slist<T>&,iterator,iterator);
#endif
  void     sortPairs();
  iterator adjacentMerge(iterator,iterator,iterator,size_type,size_type);

/** (protected template members)
 **  void          sortPairs(Comparison<T>&);
 **  iterator adjacentMerge(iterator,iterator,iterator,
 **                      size_type,size_type, Comparison<T>&);
 **/
private:
  Node          *toHead_;
  Node          **head_;
  Node          **tail_;
  size_type     entries_;

  allocator_type nodeAllocator;
};

// global boolean operators on rw_slists

#ifndef RW_ALLOCATOR_PLAIN
template <class T, class A>
inline bool 
operator==(const rw_slist<T,A> &a, const rw_slist<T,A> &b)
{
  return a.size() == b.size() && equal(a.begin(),a.end(), b.begin());
}

template <class T, class A>
inline bool 
operator<(const rw_slist<T,A> &a, const rw_slist<T,A> &b )
{
  return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}
#else
template <class T>
inline bool 
operator==(const rw_slist<T> &a, const rw_slist<T> &b)
{
  return a.size() == b.size() && equal(a.begin(),a.end(), b.begin());
}

template <class T>
inline bool 
operator<(const rw_slist<T> &a, const rw_slist<T> &b )
{
  return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}
#endif

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/stdex/slist.cc"
#endif

#endif /* __STDEX_SLIST_H__ */
