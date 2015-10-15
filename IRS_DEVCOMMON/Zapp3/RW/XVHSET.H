#ifndef __RWXVHSET_H__
#define __RWXVHSET_H__
/*
 * RWTValHashSet<T>:  A Set of values of type T, using a hashed lookup
 *
 * $Id: xvhset.h@#/main/7  07/18/96 20:51:01  hart (TOOLS707_WIN32_971021)
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
 *
 * This class implements a parameterized Set of types T.  In a Set,
 * only one instance of an object of a given value can be inserted into
 * the collection.
 *
 * The implementation uses a hash table.
 *
 * Example use of this class:
 *
 *   #include <rw/cstring.h>
 *   #include <rw/tvhset.h>
 *   
 *   unsigned myHash(const RWCString& s){ return s.hash(); }
 *   
 *   RWTValHashSet<RWCString> set(myHash);	// A Set of RWCStrings
 *   
 *   set.insert("a string");	// Type conversion: char* to RWCString happens
 *   set.insert("another string");
 *   set.insert("a string");	// Rejected (already exists in collection)
 *   set.contains("a string");	// Returns true.
 *
 *
 * Note that the constructor for RWTValHashSet<T> takes a function with
 * prototype
 *
 *   unsigned hashFun(const T&);
 *
 * It should return a suitable hashing value for an instance of class T.
 * Usually, the definition for such a function is trivial because hashing
 * functions have been defined for all Rogue Wave supplied classes.
 *
 ***************************************************************************
 *
 * $Log: xvhset.h,v $
 * Revision 7.8  1996/07/18 20:51:01  hart
 * removed guard around include thashfun.h
 *
 * Revision 7.7  1996/07/18 04:03:18  hart
 * ObjectStore changes
 *
 * Revision 7.6  1996/05/07 17:39:57  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.5  1996/01/04 17:20:15  pearson
 * Change isSubset to isSubsetOf and isProperSubset to isProperSubsetOf
 *
 * Revision 7.4  1995/12/15 00:27:44  pearson
 * Remove isUnequal and rename isEqual to isEquivalent
 * Move isEquivalent, isSubset and isProperSubset to the base class
 * Add access adjustment due to private inheritance
 *
 * Revision 7.3  1995/12/14 21:45:37  pearson
 * Remove intersectWith
 *
 * Revision 7.2  1995/12/13 16:26:45  pearson
 * Make interface match the Tools STL-like container interface with respect
 * to the set operations
 *
 * Revision 7.1  1995/11/17 08:53:49  jims
 * Merge old and new families of templates
 *
 * Revision 7.3  1995/09/05 21:24:34  jims
 * Use new copyright macro
 *
 * Revision 7.2  1995/04/18  08:37:32  jims
 * Scopus TAN ID# 82: Use RWTExport to use templates with Tools DLL under
 * MSVC 2.x
 *
 * Revision 7.1  1994/10/16  03:08:43  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.1.7.3  1994/09/20  16:34:20  nevis
 * Added set operations.
 *
 * Revision 6.3  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 *
 * Revision 6.2  1994/06/21  00:34:22  myersn
 * move virtual dtor out-of-line.
 *
 * Revision 6.1  1994/04/15  19:48:22  vriezen
 * Move all files to 6.1
 *
 * Revision 2.8  1993/12/31  00:56:30  jims
 * ObjectStore version: add get_os_typespec() static member function
 *
 * Revision 2.7  1993/11/08  20:32:10  jims
 * Port to ObjectStore
 *
 * Revision 2.6  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.5  1993/06/03  20:47:30  griswolf
 * remove inline insert() to avoid Sun CC problem.
 *
 * Revision 2.4  1993/04/09  19:35:45  keffer
 * Indexing is now done using size_t
 *
 * Revision 2.3  1993/02/17  18:32:03  keffer
 * Now passes T's by const reference, rather than by value
 *
 * Revision 2.2  1993/01/29  01:27:09  keffer
 * Ported to cfront v3.0
 *
 *    Rev 1.2   17 Mar 1992 12:00:44   KEFFER
 * 
 *    Rev 1.1   04 Mar 1992 10:16:46   KEFFER
 * 
 *    Rev 1.0   02 Mar 1992 16:10:54   KEFFER
 * Initial revision.
 */

#include "rw/thashfun.h"
#include "rw/xvhasht.h"

/****************************************************************
 *								*
 *		Declarations for RWTValHashSet<T>		*
 *								*
 ****************************************************************/

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T> class RWTExport RWTValHashSet : public RWTValHashTable<T>
{

public:

  RWTValHashSet
  (
    RW_HASH_FUNC(T, hashFun),
    size_t size = RWDEFAULT_CAPACITY
  ) : RWTValHashTable<T>(hashFun, size)
  {;}

  virtual ~RWTValHashSet();

  // Member functions:
  virtual void		insert(const T& val);
 
  // Implement set operation primitives union, disjuncion, intersection,
  // subet, proper subset, and equivalence; respectively:
//***********************************************************************
//                    Pre-STDLIB interface
//***********************************************************************
//  RWTValHashSet<T> &    operator+=(const RWTValHashSet<T> & h);
//  RWTValHashSet<T> &    operator-=(const RWTValHashSet<T> & h);
//  RWTValHashSet<T> &    operator*=(const RWTValHashSet<T> & h);
//  RWBoolean             operator<=(const RWTValHashSet<T> & h) const;
//  RWBoolean             operator< (const RWTValHashSet<T> & h) const;
//  RWBoolean             operator==(const RWTValHashSet<T> & h) const;
//  RWBoolean             operator!=(const RWTValHashSet<T> & h) const
//                        { return !operator==(h); };
//***********************************************************************
  RWTValHashSet<T> &  Union              (const RWTValHashSet<T> & h);
  RWTValHashSet<T> &  difference         (const RWTValHashSet<T> & h);
  RWTValHashSet<T> &  intersection       (const RWTValHashSet<T> & h);
  RWTValHashSet<T> &  symmetricDifference(const RWTValHashSet<T> & h);
  RWBoolean           isSubsetOf       (const RWTValHashSet<T> & h) const;
  RWBoolean           isProperSubsetOf (const RWTValHashSet<T> & h) const;
  RWBoolean           isEquivalent       (const RWTValHashSet<T> & h) const;

  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/xvhset.cc"
#endif

#define RWTValHashSetIterator RWTValHashTableIterator

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif  /* __RWXVHSET_H__ */

