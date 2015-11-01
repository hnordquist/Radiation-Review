#ifndef __RWEV_INSET_H__
#define __RWEV_INSET_H__

/***************************************************************************
 *
 * ev_inset.h - RW_VAssoc<StdColl,RWColl>
 *   
 *     : value-based interface components for STL Associative Containers
 * 
 * $Id: ev_inset.h@#/main/24  09/04/97 17:25:13  griswolf (TOOLS707_WIN32_971021)
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

#include "rw/ev_scntn.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl>
class RWTExport RW_VAssoc : public RW_VCntnr<StdColl, RWColl> {

private:
  typedef RW_VCntnr<StdColl, RWColl>               base_type;

public:
  typedef typename StdColl::value_type             value_type;
  typedef typename StdColl::size_type              size_type;
  typedef typename StdColl::iterator               my_iter_;
  typedef typename StdColl::const_iterator         const_my_iter_;
  typedef typename StdColl::reference              reference;
  typedef typename StdColl::const_reference        const_reference;

// additional interface components
  void  clear() { this->std().erase(this->begin(),this->end()); }

  bool          remove(const_reference key)
  {
    my_iter_ hit = this->std().find(key);
    if(hit != this->end()) {
      this->std().erase(hit);
      return true;
    }
    return false;
  }
  
  bool          remove(/** extends the interface **/
                          bool (*fn)(const_reference,void*),void*
                         );

  size_type     removeAll(const_reference key);
  size_type     removeAll(bool (*fn)(const_reference, void*),void*);

  void     Union(const StdColl&);
  void     Union(const RWColl& rwcoll)        { Union(rwcoll.std()); }

  void     intersection(const StdColl&);
  void     intersection(const RWColl& rwcoll) { intersection(rwcoll.std()); }

  void     difference(const StdColl&);
  void     difference(const RWColl& rwcoll)   { difference(rwcoll.std()); }

  void     symmetricDifference(const StdColl&);
  void     symmetricDifference(const RWColl& rwcoll)
                                { symmetricDifference(rwcoll.std()); }

  bool isSubsetOf(const RWColl& rhs) const
  {
     if(this->entries() > rhs.entries())
        return FALSE;

     const_my_iter_ itr = this->begin();
     while(itr != this->end() && rhs.contains(*itr) )
          ++itr;

     return this->end() == itr;
  }


  bool isSubsetMultiOf(const RWColl& rhs) const
  {
     if(this->entries() > rhs.entries())
        return FALSE;

     const_my_iter_ itr = this->begin();
     while(itr != this->end() && this->std().count(*itr) <=
                       rhs.std().count(*itr) )
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


// overridden interface components
  bool  contains(const_reference key) const
  {
    my_iter_ iter = this->std().find(key);
    return iter != this->end();
  }
  bool find(const_reference, value_type& ret) const;
  
  
  size_type occurrencesOf(const_reference key) {return this->std().count(key);}
  
// Avoid hiding overloads
  bool  contains(bool(*fn)(const_reference,void*), void* x) const
    { return base_type::contains(fn,x); }

  bool  find(bool(*fn)(const_reference,void*), void* x, value_type& hit) const
    { return base_type::find(fn, x, hit); }

  size_type occurrencesOf(bool(*fn)(const_reference,void*), void* x) const
    { return base_type::occurrencesOf(fn, x); }
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ev_inset.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEV_INSET_H__ */
