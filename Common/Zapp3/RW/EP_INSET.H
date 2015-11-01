#ifndef __RWEP_INSET_H__
#define __RWEP_INSET_H__

/***************************************************************************
 *
 * ep_inset.h - RW_PAssoc<StdColl,RWColl,DataTypes>
 *   
 *     : pointer-based interface components for STL Associative Containers
 * 
 * $Id: ep_inset.h@#/main/19  09/12/97 13:43:13  kevinj (TOOLS707_WIN32_971021)
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

#include "rw/ep_scntn.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl, class T>
class RWTExport RW_PAssoc : public RW_PCntnr<StdColl,RWColl,T>
{
private:
  typedef RW_PCntnr<StdColl,RWColl,T>         base_type;

public:
  typedef typename StdColl::value_type        value_type;
  typedef typename StdColl::size_type         size_type;
  typedef typename StdColl::iterator          iterator;
  typedef typename StdColl::const_iterator    const_iterator;
  typedef typename StdColl::reference         reference;
  typedef typename StdColl::const_reference   const_reference;
  typedef const T*                            const_value;


// Additional interface components
  void  clear() { this->std().erase(this->begin(),this->end()); }

// clearAndDestroy should be overridden in any leaf class that knows
// how to save its iterators while deleting. This version uses 2
// passes: One to delete, then clear()
  void  clearAndDestroy();
#if !defined(__SUNPRO_CC) || (__SUNPRO_CC<0x420 && !defined (RW_NO_STL))
  typename StdColl::size_type    removeAll(const_value key);

  typename StdColl::size_type    removeAll(bool (*fn)(const_value, void*),void*);
#else
  size_type    removeAll(const_value key);
  size_type    removeAll(bool (*fn)(const_value, void*),void*);
#endif /* !SunPro */
  value_type            remove(const_value key)
    {
      const_value ret = 0;
      iterator hit = this->std().find((T*)key);
      if(hit != this->end()) {
        ret = *iterator(hit);
        this->std().erase(hit);
      }
      return (value_type)ret;
    }

  value_type  remove(bool (*fn)(const_value,void*),void*);

// overridden interface components
  bool  contains(const_value key) const
  {
    iterator iter = this->std().find((T*)key);
    return iter != this->end();
  }

  const_value   find(const_value) const;

  size_type     occurrencesOf(const_value key)
    { return this->std().count((value_type)key); }

// Avoid hiding overloads
  bool  contains(bool(*fn)(const_value, void*),void* x) const
    { return  base_type::contains(fn,x); }

  size_type     occurrencesOf(bool(*fn)(const_value,void*),void* x) const
    { return base_type::occurrencesOf(fn,x); }

  const_value   find(bool(*fn)(const_value, void*),void* x) const
    { return base_type::find(fn,x); }

  void     Union(const RWColl& rwcoll)
    { Union((StdColl&)rwcoll.std()); }

  void     intersection(const RWColl& rwcoll)
    { intersection((StdColl&)rwcoll.std()); }

  void     difference(const RWColl& rwcoll)
    { difference((StdColl&)rwcoll.std()); }

  void     symmetricDifference(const RWColl& rwcoll)
    { symmetricDifference((StdColl&)rwcoll.std()); }

  bool isSubsetOf(const RWColl& rhs) const
  {
     if(this->entries() > rhs.entries())
        return FALSE;

     const_iterator itr = this->begin();
     while(itr != this->end() && rhs.contains(*itr) )
          ++itr;

     return this->end() == itr;
  }

  bool isSubsetMultiOf(const RWColl& rhs) const
  {
     if(this->entries() > rhs.entries())
        return FALSE;

     const_iterator itr = this->begin();
     while(itr != this->end() && this->std().count(*itr) 
	                        <= rhs.std().count(*itr) )
         ++itr;

     return this->end() == itr;
  }

  bool isProperSubsetOf(const RWColl& rhs) const
  { return isSubsetOf(rhs) && (this->entries() < rhs.entries()); }

  bool isProperSubsetMultiOf(const RWColl& rhs) const
  { return isSubsetMultiOf(rhs) && (this->entries() < rhs.entries()); }

  bool isEquivalent(const RWColl& rhs) const
  { return isSubsetOf(rhs) && (this->entries() == rhs.entries()); }

  bool isEquivalentMulti(const RWColl& rhs) const
  { return isSubsetMultiOf(rhs) && (this->entries() == rhs.entries()); }

protected:
  void     Union(const StdColl&);
  void     intersection(const StdColl&);
  void     difference(const StdColl&);
  void     symmetricDifference(const StdColl&);

protected:
  RW_PAssoc() { } // can only be used as base class 
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ep_inset.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEP_INSET_H__ */
