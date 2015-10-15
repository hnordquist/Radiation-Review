#ifndef __RWEP_HASH_H__
#define __RWEP_HASH_H__

/***************************************************************************
 *
 * ep_hash.h - RW_PHAssoc<StdColl,RWColl,T>
 *   
 * : pointer-based interface components for STL Hashed Associative Containers
 * 
 * $Id: ep_hash.h@#/main/4  08/09/96 18:52:01  hart (TOOLS707_WIN32_971021)
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
 * 
 * $Log: ep_hash.h,v $
 * Revision 7.5  1996/08/09 18:52:01  hart
 * HP (aCC compiler) port
 *
 * Revision 7.4  1996/07/18 04:27:33  hart
 * ObjectStore changes
 *
 * Revision 7.3  1996/05/08 17:00:19  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.2  1996/02/02 01:32:11  jims
 * Move definition of hashCollEqual to ep_hash.cc
 *
 * Revision 7.1  1996/02/01 21:51:13  jims
 * Move to revision 7.1
 *
 * 
 ****************************************************************************/

#include "rw/ep_inset.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl, class T>
class RWTExport RW_PHAssoc : public RW_PAssoc<StdColl,RWColl,T>
{
private:
  typedef RW_PAssoc<StdColl,RWColl,T>         base_type;

public:
  typedef typename StdColl::value_type        value_type;
  typedef typename StdColl::size_type         size_type;
  typedef typename StdColl::iterator          iterator;
  typedef typename StdColl::const_iterator    const_iterator;
  typedef typename StdColl::reference         reference;
  typedef typename StdColl::const_reference   const_reference;
  typedef const T*                            const_value;

  void     UnionHash(const RWColl& rwcoll)
    { UnionHash((StdColl&)rwcoll.std()); }

  void     intersectionHash(const RWColl& rwcoll)
    { intersectionHash((StdColl&)rwcoll.std()); }

  void     differenceHash(const RWColl& rwcoll)
    { differenceHash((StdColl&)rwcoll.std()); }

  void     symmetricDifferenceHash(const RWColl& rwcoll)
    { symmetricDifferenceHash((StdColl&)rwcoll.std()); }

protected:
  void     UnionHash(const StdColl&);
  void     intersectionHash(const StdColl&);
  void     differenceHash(const StdColl&);
  void     symmetricDifferenceHash(const StdColl&);

protected:
  RW_PHAssoc() { } // can only be used as base class 
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ep_hash.cc"
#endif

/*
 * Special function for testing equality of two hash collections.
 * The standard algorithm 'equal' is not suitable due to the
 * inherent un-orderedness of hash collections.
 */
template <class RWColl>
bool hashCollEqual(const RWColl& h1, const RWColl& h2);

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEP_HASH_H__ */
