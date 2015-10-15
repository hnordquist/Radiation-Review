#ifndef __RWEV_HASH_H__
#define __RWEV_HASH_H__

/***************************************************************************
 *
 * ev_hash.h - RW_VHAssoc<StdColl,RWColl>
 *   
 * : value-based interface components for STL Hashed Associative Containers
 * 
 * $Id: ev_hash.h@#/main/2  07/18/96 04:27:33  hart (TOOLS707_WIN32_971021)
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
 * $Log: ev_hash.h,v $
 * Revision 7.3  1996/07/18 04:27:33  hart
 * ObjectStore changes
 *
 * Revision 7.2  1996/05/07 23:45:09  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.1  1996/02/01 21:51:13  jims
 * Move to revision 7.1
 *
 * 
 ****************************************************************************/

#include "rw/ev_inset.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl>
class RWTExport RW_VHAssoc : public RW_VAssoc<StdColl, RWColl> {

private:
  typedef RW_VAssoc<StdColl, RWColl> base_type;

public:

  void	   UnionHash(const StdColl&);
  void	   UnionHash(const RWColl& rwcoll)    { UnionHash(rwcoll.std()); }

  void     intersectionHash(const StdColl&);
  void     intersectionHash(const RWColl& hc) { intersectionHash(hc.std()); }

  void     differenceHash(const StdColl&);
  void     differenceHash(const RWColl& hc)   { differenceHash(hc.std()); }

  void     symmetricDifferenceHash(const StdColl&);
  void     symmetricDifferenceHash(const RWColl& rwcoll)
				{ symmetricDifferenceHash(rwcoll.std()); }
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ev_hash.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEV_INSET_H__ */

