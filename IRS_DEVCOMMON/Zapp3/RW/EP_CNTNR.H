#ifndef __RW_EPCNTNR_H__
#define __RW_EPCNTNR_H__

/***************************************************************************
 *
 * ep_cntnr.h - RW_PBase<StdColl,RWColl>
 *   
 *     : pointer-based interface components for STL Containers
 * 
 * $Id: ep_cntnr.h@#/main/41  09/12/97 13:48:14  kevinj (TOOLS707_WIN32_971021)
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
 * This class is at the base of the implementation hierarchy
 * for the pointer-based collections.  The methods here are
 * those which only need to know that StdColl is some sort of
 * an STL-compliant container, whether it holds singleton values
 * as does a vector or set, or pairs as does a map.  RWColl is the
 * Tools.h++ wrapper class that uses StdColl as its implementation.
 * RWColl must ultimately derive from RW_PBase<StdColl, RWColl>
 * and supply a member function std() which returns an instance
 * of StdColl.
 * 
 *****************************************************************************/

#include "rw/rwerr.h"
#include "rw/toolerr.h"
#include "rw/epaux.h"

/*
 * StdColl:     the STL collection used for implementation
 * RWColl:      the RW collection which wraps the above
 */

template <class StdColl, class RWColl>
class RW_PBase {
          
public:
  typedef typename StdColl::value_type        value_type;
  typedef typename StdColl::size_type         size_type;
  typedef typename StdColl::iterator          my_iter_;
  typedef typename StdColl::const_iterator    const_my_iter_;
  typedef typename StdColl::reference         reference;
  typedef typename StdColl::const_reference   const_reference;

  StdColl&        std()       { return ((RWColl*) this)->impl_; }
  const StdColl&  std() const { return ((const RWColl*) this)->impl_; }

  my_iter_             begin()             { return std().begin(); }
  my_iter_             end()               { return std().end();   }
  const_my_iter_       begin() const       { return std().begin(); }
  const_my_iter_       end()   const       { return std().end();   }

// RW interface: public member functions
  size_type       entries() const { return std().size(); }
  bool            isEmpty() const { return std().empty(); }


/*
 * Unsolved problem with msvc forces cast away of const below:
 */

  const_reference operator() (size_type idx) const
  {
#ifdef RWDEBUG
    boundsCheck(idx);
#endif
#ifndef _MSC_VER
    const_my_iter_ i = begin();
#else
    my_iter_ i = ((StdColl&)std()).begin();
#endif
    advance(i,idx);
    return *i;
  }

  const_reference operator[](size_type idx) const
  {
    boundsCheck(idx);
#ifndef _MSC_VER
    const_my_iter_ i = begin();
#else
    my_iter_ i = ((StdColl&)std()).begin();
#endif
    advance(i,idx);
    return *i;
  }

  reference operator()(size_type idx) 
  {
#ifdef RWDEBUG
    boundsCheck(idx);
#endif
    my_iter_ i = begin();
    advance(i,idx);
    return *i;
  }

  reference operator[](size_type idx) 
  {
    boundsCheck(idx);
    my_iter_ i = begin();
    advance(i,idx);
    return *i;
  }

///// public methods
#if !defined(__SUNPRO_CC) || __SUNPRO_CC<0x420
  void  apply(void (*fn)(value_type,void*),void *x);
  void  apply(void (*fn)(reference,void*),void *x);
#else
  void apply(void (*fn)(typename RW_PBase<StdColl,RWColl>::value_type,void*),void *x)
    {
         my_iter_ iter = begin();
         while(iter != end()) {
         (*fn)(*iter,x);
         ++iter;
         }
    }
void apply(void (*fn)(typename RW_PBase<StdColl,RWColl>::reference,void*),void *x)
    {
          my_iter_ iter = begin();
          while(iter != end()) {
          (*fn)(*iter,x);
          ++iter;
          }
    }
#endif /* !SunPro 420 */

  const_reference    at(size_type idx) const
        { return (*this)[idx]; }

  reference   at(size_type idx)
        { return (*this)[idx]; }
#if !defined(__SUNPRO_CC) || __SUNPRO_CC<0x420
  bool  contains(bool(*fn)(value_type,void*),void* x) const;
#else
  bool contains(bool(*fn)(typename RW_PBase<StdColl,RWColl>::value_type,void*),void* x) const
    {
          const_my_iter_ iter = begin();
          while(iter != end()) {
          if((*fn)(*iter,x)) break;
          ++iter;
          }
          return iter != end();
    }
#endif /* !SunPro 420 */
    
  const_reference first() const  { return *begin(); }
  reference       first()        { return *begin(); }

#if defined(RW_BROKEN_NAME_RESOLUTION) || (defined(__SUNPRO_CC) && __SUNPRO_CC>=0x420)

  typename StdColl::
  size_type  index(bool (*fn)(value_type, void*), void* x) const
{
  const_my_iter_ i = begin();
  size_type idx = 0;
  for(/**/; i != end(); ++i) {
    if((*fn)(*i,x))
      return idx;
    ++idx;
  }
  return npos;
}

  typename StdColl::
  size_type  occurrencesOf(bool (*fn)(value_type,void*),void* x) const
{
  size_type ret = 0;
  const_my_iter_ iter = begin();
  while(iter != end()) {
    if((*fn)(*iter,x)) ++ret;
    ++iter;
  }
  return ret;
}
#else
  typename StdColl::
  size_type  index(bool (*fn)(value_type, void*), void* x) const;

  typename StdColl::
  size_type  occurrencesOf(bool (*fn)(value_type,void*),void* x) const;
#endif

  static size_type npos;


protected:
  void          boundsCheck(size_type i) const
    {
      if (!(i < std().size()) )
        RWTHROW( RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, entries() )));
    }

// Can only be used as a base class:
  RW_PBase() { }
  RW_PBase(const RW_PBase<StdColl,RWColl>&) { }
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};


#ifndef RWDefCArgs
#  define RWDefCArgs(T) ,less<T>
#endif

#ifndef RWDefHArgs
#  define RWDefHArgs(T) ,RWTHasher<T>,equal_to<T>
#endif



#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ep_cntnr.cc"
#endif

#endif /* __RW_EPCNTNR_H__ */

