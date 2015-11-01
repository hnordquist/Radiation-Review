#ifndef __RWXVSRTVEC_H__
#define __RWXVSRTVEC_H__

/*
 * Sorted value-based vector.  Inserts values using an insertion sort.
 *
 * $Id: xvsrtvec.h@#/main/9  07/18/96 04:03:18  hart (TOOLS707_WIN32_971021)
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
 * Stores a *copy* of the inserted item into the collection according
 * to an ordering determined by the less-than (<) operator.
 *
 * Assumes that T has:
 *   - well-defined copy semantics (T::T(const T&) or equiv.);
 *   - well-defined assignment semantics (T::operator=(const T&) or equiv.);
 *   - well-defined equality semantics (T::operator==(const T&));
 *   - well-defined less-than semantics (T::operator<(const T&)).
 *
 * Note that while these are automatically defined for builtin types
 * (such as "int", "double", or any pointer), you may need to provide
 * appropriate operators for your own classes, particularly those with
 * constructors and/or pointers to other objects.
 *
 * This class uses binary searches for efficient value-based retrievals.
 *
 ***************************************************************************
 *
 * $Log: xvsrtvec.h,v $
 * Revision 7.10  1996/07/18 04:03:18  hart
 * ObjectStore changes
 *
 * Revision 7.9  1996/05/07 17:38:50  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.8  1996/03/26 22:30:38  kevinj
 * Removed TURBOC < 0x500 qualifier.
 *
 * Revision 7.7  1996/03/26 21:38:48  kevinj
 * An #endif overshot a wee bit and commented out the .cc include.
 * This is now fixed.
 *
 * Revision 7.6  1996/03/26 01:25:59  pearson
 * #2926 - Fix ambiguous operator << and >> between RWTValSortedVector and
 *         RWTValOrderedVector
 *
 * Revision 7.5  1996/03/18 16:04:45  jims
 * Port to cfront template mechanism
 *
 * Revision 7.4  1996/03/16 15:15:37  jims
 * Port non-stdlib template persistence to DEC and Watcom
 *
 * Revision 7.3  1996/01/27 01:36:34  kevinj
 * Simple External Template Persistence (ETP).
 *
 * Revision 7.2  1996/01/11 15:43:35  pearson
 * Include xvordvec.h instead of tvordvec.h.  RW_NO_STL is not available if
 * the compiler reparses the header during template instantiation on the
 * Centerline compiler.
 *
 * Revision 7.1  1995/11/17 08:53:49  jims
 * Merge old and new families of templates
 *
 * Revision 7.3  1995/09/05 21:32:21  jims
 * Use new copyright macro
 *
 * Revision 7.2  1995/04/18  08:38:34  jims
 * Scopus TAN ID# 82: Use RWTExport to use templates with Tools DLL under
 * MSVC 2.x
 *
 * Revision 7.1  1994/10/16  03:11:51  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.2  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 *
 * Revision 6.1  1994/04/15  19:48:42  vriezen
 * Move all files to 6.1
 *
 * Revision 2.8  1993/12/31  00:04:42  jims
 * ObjectStore version: Add static os_typespec* get_os_typespec();
 *
 * Revision 2.7  1993/11/16  03:19:22  jims
 * Add RWTExport modifier
 *
 * Revision 2.6  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.5  1993/04/09  19:35:45  keffer
 * Indexing is now done using size_t
 *
 * Revision 2.4  1993/03/24  01:29:51  keffer
 * Indexing operations now used unsigned
 *
 * Revision 2.3  1993/02/17  18:32:03  keffer
 * Now passes T's by const reference, rather than by value
 *
 * Revision 2.2  1993/01/29  03:07:03  keffer
 * Increased code reuse between ordered and sorted vectors
 *
 *    Rev 1.3   17 Mar 1992 11:30:26   KEFFER
 * 
 *    Rev 1.2   11 Mar 1992 15:23:44   KEFFER
 * 
 *    Rev 1.0   02 Mar 1992 16:10:54   KEFFER
 * Initial revision.
 */

#include "rw/xvordvec.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class T> class RWTExport RWTValSortedVector : public RWTValOrderedVector<T>
{

public:

  //Constructors
  RWTValSortedVector
  (
    size_t capac = RWDEFAULT_CAPACITY
  ) :  RWTValOrderedVector<T>(capac) {;}

  RWTValSortedVector
  (
    const RWTValSortedVector<T>& c
  ) : RWTValOrderedVector<T>(c) {;}

  // Overridden virtual functions
  virtual size_t	index(const T& p) const;
  virtual void		insert(const T& p);
  virtual size_t	occurrencesOf(const T& p) const;
  virtual RWBoolean	remove(const T& p);
  virtual size_t	removeAll(const T& p);

protected:

  RWBoolean		bsearch(const T&, size_t&) const;	// binary search routine
  size_t		indexSpan(const T&, size_t&) const;
#ifdef RWDEBUG
  RWBoolean		isSorted() const;
#endif

  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#if !defined(__TURBOC__) 

#include "rw/edefs.h"
RWDECLARE_PERSISTABLE_TEMPLATE(RWTValSortedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
class RWExport RWpistream; class RWExport RWpostream;
class RWExport RWbistream; class RWExport RWbostream; 
class RWExport RWeistream; class RWExport RWeostream;
RWDECLARE_PERSIST_TEMPLATE_IO(RWTValSortedVector,RWpistream,RWpostream)
RWDECLARE_PERSIST_TEMPLATE_IO(RWTValSortedVector,RWbistream,RWbostream)
RWDECLARE_PERSIST_TEMPLATE_IO(RWTValSortedVector,RWeistream,RWeostream)
#endif     


template <class T, class S>
void rwSaveGuts(S& str, const RWTValSortedVector<T>& c);

template <class T, class S>
void rwRestoreGuts(S& str, RWTValSortedVector<T>& c);

#endif  /* !defined(__TURBOC__) || (__TURBOC__ < 0x500) */

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/xvsrtvec.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif  /* __RWXVSRTVEC_H__ */

