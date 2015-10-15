#ifndef __RWTHASHFUN_H__
#define __RWTHASHFUN_H__

/*
 * RWTHashFun: A parameterized wrapper for hash functions for use
 *             with the ObjectStore version of Tools.h++
 *
 * $Id: thashfun.h@#/main/1  10/08/96 23:45:31  kyle (TOOLS707_WIN32_971021)
 *
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
 * $Log: thashfun.h,v $
 * Revision 7.2  1996/10/08 23:45:31  kyle
 * ObjectStore update
 *
 * Revision 7.1  1996/07/18 20:45:08  hart
 * OBjectStore change
 *
 * Revision 7.1  1996/07/18 04:35:29  hart
 * ObjectStore changes
 *
 * Revision 1.1.1.1.1.1  1995/08/25  21:10:21  gmf
 * 	* Make it compile.
 *
 * Revision 1.1.1.1  1995/05/25  13:58:47  adama
 * Migrate changes from ClearCase to TRC.
 *
 * Revision 1.1  1993/11/08  11:03:06  jims
 * Initial revision
 *
 */

#include "rw/compiler.h"

#if defined(RW_USE_OBJECTSTORE) || defined(RW_USE_OBJECTSTORE_PSE)
#define RW_HASH_FUNC(TP,NAME) RWTHashFun<TP>* NAME

template <class T>
class RWTHashFun {
  virtual unsigned hashFun(const T&) = 0;
public:
  unsigned operator()(const T& p) { return hashFun(p); }
};

#else
#define RW_HASH_FUNC(TP,NAME) unsigned (*NAME)(const TP&)
#endif

#endif /* __RWTHASHFUN_H__ */

