/*
 * Template definitions for RWTValHashDictionary<K,V> and RWTValHashDictionaryIterator<K,V>
 *
 * $Id: tvhdict.cc@#/main/18  09/05/96 15:20:49  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tvhdict.cc,v $
 * Revision 7.8  1996/09/05 15:20:49  griswolf
 * Scopus #4418: Fix problem with diff type and size of size_t.
 *
 * Revision 7.7  1996/08/09 19:36:27  hart
 * HP (aCC compiler) port
 *
 * Revision 7.6  1996/02/12 21:03:06  kevinj
 * Switched stream and class args in rw*Guts.
 *
 * Revision 7.5  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.4  1996/01/16 18:27:01  kevinj
 * Simple ETP.
 *
 * Revision 7.3  1996/01/15 20:30:20  kevinj
 * Another patch for OTIS0017.
 *
 * Revision 7.2  1995/09/05 21:23:35  jims
 * Use new copyright macro
 *
 * Revision 7.1  1994/10/16  03:19:30  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.3  1994/07/12  20:04:35  vriezen
 * Updated Copyright.
 *
 * Revision 6.2  1994/06/21  00:48:01  myersn
 * move virtual dtor out-of-line.
 *
 * Revision 6.1  1994/04/15  19:48:18  vriezen
 * Move all files to 6.1
 *
 * Revision 1.8  1993/11/08  14:57:02  jims
 * Port to ObjectStore
 *
 * Revision 1.7  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 1.6  1993/04/09  19:35:45  keffer
 * Indexing is now done using size_t
 *
 * Revision 1.5  1993/03/24  01:29:51  keffer
 * Indexing operations now used unsigned
 *
 * Revision 1.4  1993/02/17  19:46:03  keffer
 * Now uses RWTPtrVector<T> rather than RWTValVector<T*>
 *
 * Revision 1.3  1993/02/17  18:32:03  keffer
 * Now passes T's by const reference, rather than by value
 *
 * Revision 1.2  1993/02/07  22:32:54  keffer
 * Added copy constructor and assignment operator for iterator
 *
 * Revision 1.1  1993/01/29  20:46:34  keffer
 * Initial revision
 *
 *
 ***************************************************************************
 */

#include "rw/epersist.h"

RWDEFINE_PERSISTABLE_TEMPLATE_4(RWTValHashMap)

template <class K, class T, class H, class EQ, class S>
void rwSaveGuts(S& str, const RWTValHashMap<K,T,H,EQ>& c)
{
#ifdef RW_FIX_XSIZE_T
  str.putSizeT(c.entries());
#else
  str << c.entries();
#endif

  typename RWTValHashMap<K,T,H,EQ>::const_iterator it = c.begin();

  while( it != c.end() ) {
    str << (*it).first;   // key
    str << (*it).second;  // data
    it++;
  }
}

template <class K, class T, class H, class EQ, class S>
void rwRestoreGuts(S& str, RWTValHashMap<K,T,H,EQ>& c)
{
  typedef typename RWTValHashMap<K,T,H,EQ>::size_type size_type;
  typename RWTValHashMap<K,T,H,EQ>::size_type count;
#ifdef RW_FIX_XSIZE_T
  str.getSizeT(count);
#else
  str >> count;
#endif
  c.clear();
  for (size_type i=0; i < count; ++i) {
    K key;
    str >> key;
    T data;
    str >> data;
    c.insert(key,data);
  }
}

