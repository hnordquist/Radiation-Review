/*
 * Template definitions for RWTPtrOrderedVector<TP>
 *
 * $Id: tpordvec.cc@#/main/15  09/05/96 15:20:49  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: tpordvec.cc,v $
 * Revision 7.9  1996/09/05 15:20:49  griswolf
 * Scopus #4418: Fix problem with diff type and size of size_t.
 *
 * Revision 7.8  1996/08/09 19:25:58  hart
 * HP (aCC compiler) port
 *
 * Revision 7.7  1996/02/12 21:03:06  kevinj
 * Switched stream and class args in rw*Guts.
 *
 * Revision 7.6  1996/01/25 01:51:32  kevinj
 * Improved ETP macros.
 *
 * Revision 7.5  1996/01/13 22:07:32  kevinj
 * Simple External Template Persistence (ETP).
 *
 * Revision 7.4  1996/01/02 22:49:31  jims
 * Use size_t for indexing
 *
 * Revision 7.3  1995/09/17 07:50:07  jims
 * increment nitems_ before trying to access new last element
 *
 * Revision 7.2  1995/09/05  20:23:04  jims
 * Use new copyright macro
 *
 * Revision 7.1  1994/10/16  03:17:29  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.2  1994/07/12  20:04:35  vriezen
 * Updated Copyright.
 *
 * Revision 6.1  1994/04/15  19:47:53  vriezen
 * Move all files to 6.1
 *
 * Revision 6.1.7.2  1994/04/20  22:16:14  vriezen
 * Bug #337 Fix bounds check for ordered and sorted vectors.
 *
 * Revision 6.1.7.1  1994/04/15  22:45:41  vriezen
 * Version 7, Initial code
 *
 * Revision 6.1  1994/04/15  19:47:53  vriezen
 * Move all files to 6.1
 *
 * Revision 1.5  1994/01/12  03:09:17  jims
 * Add constness to T* parameters where appropriate
 *
 * Revision 1.4  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 1.3  1993/04/09  19:35:45  keffer
 * Indexing is now done using size_t
 *
 * Revision 1.2  1993/03/24  01:29:51  keffer
 * Indexing operations now used unsigned
 *
 * Revision 1.1  1993/01/28  02:39:38  keffer
 * Initial revision
 *
 *
 ***************************************************************************
 */

#include "rw/epersist.h"

RWDEFINE_PERSISTABLE_TEMPLATE(RWTPtrOrderedVector)

template <class T, class S>
void rwSaveGuts(S& str, const RWTPtrOrderedVector<T>& c)
{
#ifdef RW_FIX_XSIZE_T
  str.putSizeT(c.entries());
#else
  str << c.entries();
#endif
  for (size_t i=0; i < c.entries(); ++i)
    str << *c[i];
}

template <class T, class S>
void rwRestoreGuts(S& str, RWTPtrOrderedVector<T>& c)
{
  typedef typename RWTPtrOrderedVector<T>::size_type size_type;
  size_type count;
#ifdef RW_FIX_XSIZE_T
  str.getSizeT(count);
#else
  str >> count;
#endif
  c.clear();
  for (size_type i=0; i < count; ++i) {
    T* p;
    str >> p;
    c.insert(p);
  }
}

