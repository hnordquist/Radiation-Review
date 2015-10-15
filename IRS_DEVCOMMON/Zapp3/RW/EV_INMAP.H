#ifndef __RWEV_INMAP_H__
#define __RWEV_INMAP_H__

/***************************************************************************
 *
 * ev_inmap.h - RW_VMapAssoc<StdColl,RWColl,Key,T>
 *   
 *     : value-based Key/Data interface components for STL Associative
 *       Containers
 *
 * $Id: ev_inmap.h@#/main/24  09/11/97 13:44:04  griswolf (TOOLS707_WIN32_971021)
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

#include "rw/ev_mcntn.h"

// handy macros, #undef'd below
#define KEY(i)    ((*i).first)
#define DATA(i)   ((*i).second)

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl, class Key, class T>
class RWTExport RW_VMapAssoc
    : public RW_VMapCntnr<StdColl, RWColl, Key, T>
{
public:
  typedef RW_VMapCntnr<StdColl, RWColl, Key, T>     base_type;
  typedef typename base_type::value_type            value_type;
  typedef typename base_type::size_type             size_type;
  typedef typename base_type::iterator              my_iter_;
  typedef typename base_type::const_iterator        const_my_iter_;
  typedef typename base_type::reference             reference;
  typedef typename base_type::const_reference       const_reference;

  void  apply(void (*fn)(const Key&, T&, void*), void* x);
  void  apply(void (*fn)(const Key&, const T&, void*),void*x) const;

  // deprecated in favor of "apply":
  void  applyToKeyAndValue(void (*fn)(const Key&, T&, void*),void* x)
  { apply(fn,x); }
      
  // deprecated in favor of "apply":
  void  applyToKeyAndValue(void (*fn)(const Key&, const T&, void*),
                                void*x) const
  { apply(fn,x); }

  void       clear() { this->std().erase(this->begin(),this->end()); }

  bool       contains(const Key& key) const
    { const_my_iter_ i = this->std().find(key); return i != this->end(); }

  bool          contains(bool(*fn)(const_reference,void*),void* x) const
    { return base_type::contains(fn,x); }

  bool       find(const Key& key, Key& ret) const
  {
    const_my_iter_ i=this->std().find(key);
    if(i != this->end()) {
      ret = KEY(i);
      return true;
    }
    return false;
  }

#ifndef RW_REF_SEEMS_PTR
  bool find(bool(*fn)(const_reference,void*),void* x, pair<Key,T>& val) const
  { return base_type::find(fn,x,val); }
#endif

  bool       findValue(const Key& key, T& ret) const
  {
    const_my_iter_ i=this->std().find(key);
    if(i != this->end()) {
      ret = DATA(i);
      return true;
    }
    return false;
  }

  bool       findKeyAndValue(const Key& key, Key& kret, T& vret) const
  {
    const_my_iter_ i=this->std().find(key);
    if(i != this->end()) {
      kret = KEY(i);
      vret = DATA(i);
      return true;
    }
    return false;
  }

  size_type  occurrencesOf(const Key& k) const { return this->std().count(k); }

  size_type  occurrencesOf(bool (*fn)(const_reference,void*),void* x) const
    { return base_type::occurrencesOf(fn,x); }

  bool       remove(const Key& key)
  {
    my_iter_ i = this->std().find(key);
    if(i != this->end()) {
      this->std().erase(i);
      return true;
    }
    return false;
  }

  bool remove(bool (*fn)(const_reference,void*),void* x);

  size_type  removeAll(const Key& key) { return this->std().erase(key); }

  size_type  removeAll(bool (*fn)(const_reference, void*), void* x);
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#undef KEY
#undef DATA

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ev_inmap.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEV_INMAP_H__ */
