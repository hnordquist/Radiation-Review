#ifndef __RWEP_INMAP_H__
#define __RWEP_INMAP_H__

/***************************************************************************
 *
 * ep_inmap.h - RW_PMapAssoc<StdColl,RWColl,Key,T>
 *   
 *     : pointer-based Key/Data interface components for STL Associative
 *       Containers 
 *
 * $Id: ep_inmap.h@#/main/23  09/04/97 15:09:35  griswolf (TOOLS707_WIN32_971021)
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
 ****************************************************************************/

#include "rw/ep_mcntn.h"

// handy macros, #undef'd below
#define KEY(i)    ((*i).first)
#define DATA(i)   ((*i).second)

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl, class Key, class T>
class RWTExport RW_PMapAssoc
  : public RW_PMapCntnr<StdColl, RWColl, Key, T> {

private:
  typedef RW_PMapCntnr<StdColl,RWColl,Key,T>  base_type;

public:
  typedef typename StdColl::value_type        value_type;
  typedef typename StdColl::size_type         size_type;
  typedef typename StdColl::iterator          iterator;
  typedef typename StdColl::const_iterator    const_iterator;
  typedef typename StdColl::reference         reference;
  typedef typename StdColl::const_reference   const_reference;

  typedef Key*              value_type_key;
  typedef const Key*        const_value_key;
  typedef Key*&             reference_key;
  typedef const Key*const&  const_reference_key;

  typedef T*                value_type_data;
  typedef const T*          const_value_data;
  typedef T*&               reference_data;
  typedef const T*const&    const_reference_data;

  void  apply(void (*fn)(const Key*, T*&, void*),void* x)
  {
    for(iterator i = this->begin(); i != this->end(); ++i)
      (*fn)(KEY(i), DATA(i), x);
  }
  
  void  apply(void (*fn)(const Key*, const T*, void*),void*x) const
  {
    for(const_iterator i = this->begin(); i != this->end(); ++i)
      (*fn)(KEY(i), DATA(i), x);
  }

  // deprecated:
  void  applyToKeyAndValue(void (*fn)(const Key*, T*&, void*),void* x)
  { apply(fn,x); }

  // deprecated:
  void  applyToKeyAndValue(void (*fn)(const Key*,
                                      const T*, void*),void*x) const
  { apply(fn,x); }

  void       clear() { this->std().erase(this->begin(),this->end()); }

  void       clearAndDestroy();

  bool       contains(const Key* key) const
    { const_iterator i = this->std().find((Key*)key); return i != this->end();}

  bool       contains(bool(*fn)(value_type,void*),void* x) const
    { return base_type::contains(fn,x); }

  const Key*         find(const Key* key) const
  {
    const_iterator i = this->std().find((Key*)key);
    return ( i == this->end()) ? 0 : (Key*) KEY(i);
  }

  value_type find(bool (*fn)(value_type, void*), void* x) const
    { return base_type::find(fn,x); }

  const T*     findValue(const Key* key) const
  {
    const_iterator i = this->std().find((Key*)key);
    return ( i == this->end()) ? 0 : (const T*) DATA(i);
  }

  T*     findValue(const Key* key)
  {
    const_iterator i = this->std().find((Key*)key);
    return ( i == this->end()) ? 0 : (T*) DATA(i);
  }

  const Key* findKeyAndValue(const Key* key, const T*& vret) const
  {
    const_iterator i = this->std().find((Key*)key);
    if(i != this->end()) {
      vret = (const T*&) DATA(i);
      return (const Key*) KEY(i);
    }
    return 0;
  }

  const Key* findKeyAndValue(const Key* key, T*& vret)
  {
    const_iterator i = this->std().find((Key*)key);
    if(!(i == this->end())) {
      vret = (T*&) DATA(i);
      return (const Key*) KEY(i);
    }
    return 0;
  }

  size_type  occurrencesOf(const Key* k) const
    { return this->std().count((Key*)k); }

  size_type  occurrencesOf(bool (*fn)(value_type, void*),void* x) const
    { return base_type::occurrencesOf(fn,x); }

  Key*       remove(const Key* key)
  {
    iterator i = this->std().find((Key*)key);
    if(i != this->end()) {
      Key* ret = (Key*)(*i).first;
      this->std().erase(i);
      return ret;
    }
    return rwnil;
  }

  Key*        remove(bool(*fn)(value_type,void*),void* x)
    {
      iterator hit = this->begin();
      for(/**/; hit != this->end(); ++hit) {
        if ((*fn)(*(value_type*)(&*hit),x)) {
          Key* ret = (Key*)(*hit).first;
          this->std().erase(hit);
          return ret;
        }
      }
      return rwnil;
    }
      
  size_type removeAll(const Key* key) { return this->std().erase((Key*)key); }

  size_type removeAll(bool (*fn)(value_type, void*),void* x)
    {
      size_type    ret = this->entries();
      
      while (this->entries() && (*fn)(*(value_type*)&*(this->begin()),x))
        this->std().erase(this->begin());

      iterator iter(this->begin());
      iterator iter2;

      while (iter != this->end()) {
        iter2 = iter;
        ++iter2;
        if (iter2 != this->end() && (*fn)(*(value_type*)&*iter2,x))
          this->std().erase(iter2);
        else
          ++iter;
      }

      return ret - this->entries();
    }

//protected:           THIS IS NEVER USED ... ELIMINATE?
//  static value_type nilValue_;  // needed for base class
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};
#undef KEY
#undef DATA

/*
 * Special function for testing equality of two hash collections.
 * The standard algorithm 'equal' is not suitable due to the
 * inherent un-orderedness of hash collections.
 */
template <class RWColl>
bool hashMapCollEqual(const RWColl& h1, const RWColl& h2);


#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ep_inmap.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEP_INMAP_H__ */
