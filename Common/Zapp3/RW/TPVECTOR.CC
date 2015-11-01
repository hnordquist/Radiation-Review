/*
 * Template definitions for RWTPtrVector<T>
 *
 * $Header: /nfs/crest/u1/rcs/rw/tpvector.cc,v 7.7 1996/12/03 22:22:10 griswolf Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 *
 * Copyright (C) 1992, 1993.
 * This software is subject to copyright protection under the laws of
 * the United States and other countries.
 *
 ***************************************************************************
 *
 * $Log: tpvector.cc,v $
 * Revision 7.7  1996/12/03 22:22:10  griswolf
 * Fix bad eol in log 7.1 comment.
 *
 * Revision 7.6  1996/09/05 15:20:49  griswolf
 * Scopus #4418: Fix problem with diff type and size of size_t.
 *
 * Revision 7.5  1996/06/27 19:52:50  griswolf
 * *** empty log message ***
 *
 * Revision 7.4  1996/03/16 15:15:37  jims
 * Port non-stdlib template persistence to DEC and Watcom
 *
 * Revision 7.3  1996/02/12 21:03:06  kevinj
 * Switched stream and class args in rw*Guts.
 *
 * Revision 7.2  1996/01/29 18:03:02  kevinj
 * Simple External Template Persistence (ETP).
 *
 * Revision 7.1  1996/01/29 17:36:50  jims
 * Move to revision 7.1
 *
 * Revision 1.2  1993/02/17 18:28:08  keffer
 * Now obsolete.
 *
 * Revision 1.1  1993/01/28  02:39:38  keffer
 * Initial revision
 *
 *
 ***************************************************************************
 */

 
#include "rw/epersist.h"

RWDEFINE_PERSISTABLE_TEMPLATE(RWTPtrVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
RWDEFINE_PERSIST_TEMPLATE_IO(RWTPtrVector,RWpistream,RWpostream)
RWDEFINE_PERSIST_TEMPLATE_IO(RWTPtrVector,RWbistream,RWbostream)
RWDEFINE_PERSIST_TEMPLATE_IO(RWTPtrVector,RWeistream,RWeostream)
#endif     

template <class T, class S>
void rwSaveGuts(S& str, const RWTPtrVector<T>& c)
{
#ifdef RW_FIX_XSIZE_T
  str.putSizeT(c.length());
#else
  str << c.length();
#endif
  for (size_t i=0; i < c.length(); ++i)
    str << *c[i];
}

template <class T, class S>
void rwRestoreGuts(S& str, RWTPtrVector<T>& c)
{
  size_t count;
#ifdef RW_FIX_XSIZE_T
  str.getSizeT(count);
#else
  str >> count;
#endif
  c.reshape(0);
  c.reshape(count);
  for (size_t i=0; i < count; ++i) {
    T* p;
    str >> p;
    c[i] = (p);
  }
}
