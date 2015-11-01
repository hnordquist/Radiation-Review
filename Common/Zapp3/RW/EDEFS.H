#ifndef __RWEDEFS_H__
#define __RWEDEFS_H__

/***************************************************************************
 *
 * edefs.h - definitions for STL-based containers
 *
 * $Id: edefs.h@#/main/47  03/18/97 15:06:14  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: edefs.h,v $
 * Revision 7.39  1996/08/14 20:27:09  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.38  1996/04/15 23:06:20  griswolf
 * Fix syntax-error by non-template compilers which #include this file.
 *
 * Revision 7.37  1996/04/04 22:41:32  kevinj
 * Added missing ")"'s to RWEXPORT_RETURN macros.
 *
 * Revision 7.36  1996/03/25 22:55:40  kevinj
 * Bug #2941: Added RWTEXPORT_RETURN and changed RWTExport to RWExport
 * in RWTRegularExpressionTrait<char> specialization for MSVC DLL's.
 *
 * Revision 7.35  1996/03/21 22:34:41  kevinj
 * Added operator>>(...,...*&) to template declarations.
 *
 * Revision 7.34  1996/03/20 02:55:55  kyle
 * Changed #include from having <> to ""
 *
 * Revision 7.33  1996/03/15 05:21:17  griswolf
 * Bug #2805: Added RWEXPORT_RETURN(x) to new extraction and insertion
 * operators to make them visible to DLL's import libraries.
 *
 * Revision 7.32  1996/02/18 01:40:05  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.31  1996/02/15 16:36:33  griswolf
 * Format macros for easier reading.
 *
 * Revision 7.30  1996/02/14 18:15:56  kevinj
 * Shortened macro names to less than 32 characters to deal
 * with a compiler limitation.
 *
 * Revision 7.29  1996/01/25 01:54:52  kevinj
 * Improved ETP macros.
 *
 * Revision 7.28  1996/01/19 01:15:05  kevinj
 * ETP for RW classes.
 *
 * Revision 7.27  1996/01/13 22:08:59  kevinj
 * Simple External Template Persistence (ETP).
 *
 * Revision 7.26  1996/01/12 23:30:16  kevinj
 * RWT*Map simpler External Template Persistence (ETP).
 *
 * Revision 7.25  1996/01/12 01:39:14  kevinj
 * RWT{Val,Ptr}Set External Template Persistence (ETP).
 *
 * Revision 7.24  1996/01/10 21:09:42  kevinj
 * RWDEFINE_PERSISTABLE_CONTAINER and RWDECLARE_PERSISTABLE_CONTAINER.
 *
 * Revision 7.23  1996/01/09 22:26:03  kevinj
 * Simpler external template persistence.
 *
 * Revision 7.22  1996/01/08 17:56:48  kevinj
 * Removed RW*Persistor stuff.
 *
 * Revision 7.21  1996/01/03 22:22:18  pearson
 * Dedossify
 *
 * Revision 7.20  1995/12/20  23:19:20  kevinj
 * External template persistence using RW*Persistor.
 *
 * Revision 7.19  1995/12/15 06:57:56  jims
 * Remove RW_DONT_PERSIST macro
 *
 * Revision 7.18  1995/12/14 17:54:25  jims
 * saveOn, restoreFrom now global instead of member functions
 *
 * Revision 7.17  1995/11/17 08:20:46  jims
 * Add struct RWTHasher to allow constructing stl-based collections
 * from pointers to hash functions Tools 6 style.
 *
 * Revision 7.16  1995/09/05 21:39:44  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.15  1995/08/31  12:03:51  jims
 * next rev
 *
 * Revision 7.14  1995/08/21  03:38:01  jims
 * add include for rw/toolerr.h
 *
 * Revision 7.13  1995/08/18  22:42:15  kevinj
 * Added RW_DEFINE_PERSISTABLE_COLLECTION_2 and _3.
 *
 * Revision 7.12  1995/08/17  15:38:29  kevinj
 * Clean up.
 *
 * Revision 7.11  1995/08/17  15:27:01  kevinj
 * Added [NOPERSIST] exception.
 *
 * Revision 7.9  1995/08/16  20:37:33  jims
 * Add return statements to overloaded ops in DONT_PERSIST macro
 *
 * Revision 7.8  1995/08/14  18:57:37  jims
 * Fix spacing in RW_DONT_PERSIST macro strings
 *
 * Revision 7.7  1995/08/12  03:52:11  jims
 * Add include directive for rw/rwfile.h
 *
 * Revision 7.6  1995/08/12  00:25:10  kevinj
 * Template persistence design 2.3
 *
 * Revision 7.5  1995/08/11  17:03:34  kevinj
 * Template persistence 2.2.
 *
 * Revision 7.4  1995/08/11  16:02:29  kevinj
 * Removed RW_DEFINE_PERSISTABLE_TEMPLATE macro.
 *
 * Revision 7.3  1995/08/02  00:04:36  jims
 * Qualify include file names with "rw/"
 *
 * Revision 7.2  1995/07/28  23:06:46  kevinj
 * Added #include <rw/vstream.h> to deal with an EDG compile-time error.
 *
 * Revision 7.1  1995/07/28  18:59:11  jims
 * Move revision to 7.1
 *
 * Revision 1.8  1995/07/28  17:53:35  kevinj
 * Modified in accordance with Template Persistence Design Document 2.1.
 *
 * Revision 1.7  1995/07/27  23:14:34  kevinj
 * Added RWPersist.
 *
 * Revision 1.6  1995/07/27  22:02:33  jims
 * Add header with file name, short description, $Id and $Log
 */

#include "rw/defs.h"

#define RWDECLARE_PERSIST_IO(CLASS,ISTR,OSTR)                           \
RWEXPORT_RETURN(OSTR&) operator<<(OSTR& strm, const CLASS& item);       \
RWEXPORT_RETURN(ISTR&) operator>>(ISTR& strm, CLASS& obj);              \
RWEXPORT_RETURN(ISTR&) operator>>(ISTR& strm, CLASS*& pObj);

#define RWDECLARE_PERSIST_TEMPLATE_IO(T__CLASS, ISTR, OSTR)             \
template <class T>                                                      \
RWTEXPORT_RETURN(OSTR&) operator<<(OSTR&, const T__CLASS<T>&);          \
template <class T>                                                      \
RWTEXPORT_RETURN(ISTR&) operator>>(ISTR&, T__CLASS<T>&);                \
template <class T>                                                      \
RWTEXPORT_RETURN(ISTR&) operator>>(ISTR&, T__CLASS<T>*&);

#define RWDECLARE_PERSIST_TEMPLATE_IO_2(T__CLASS, ISTR, OSTR)           \
template <class T1, class T2>                                           \
RWTEXPORT_RETURN(OSTR&) operator<<(OSTR&, const T__CLASS<T1,T2>&);      \
template <class T1, class T2>                                           \
RWTEXPORT_RETURN(ISTR&) operator>>(ISTR&, T__CLASS<T1,T2>&);            \
template <class T1, class T2>                                           \
RWTEXPORT_RETURN(ISTR&) operator>>(ISTR&, T__CLASS<T1,T2>*&);

#define RWDECLARE_PERSIST_TEMPLATE_IO_3(T__CLASS, ISTR, OSTR)           \
template <class T1, class T2, class T3>                                 \
RWTEXPORT_RETURN(OSTR&) operator<<(OSTR&, const T__CLASS<T1,T2,T3>&);   \
template <class T1, class T2, class T3>                                 \
RWTEXPORT_RETURN(ISTR&) operator>>(ISTR&, T__CLASS<T1,T2,T3>&);         \
template <class T1, class T2, class T3>                                 \
RWTEXPORT_RETURN(ISTR&) operator>>(ISTR&, T__CLASS<T1,T2,T3>*&);

#define RWDECLARE_PERSIST_TEMPLATE_IO_4(T__CLASS, ISTR, OSTR)           \
template <class T1, class T2, class T3, class T4>                       \
RWTEXPORT_RETURN(OSTR&) operator<<(OSTR&, const T__CLASS<T1,T2,T3,T4>&);\
template <class T1, class T2, class T3, class T4>                       \
RWTEXPORT_RETURN(ISTR&) operator>>(ISTR&, T__CLASS<T1,T2,T3,T4>&);      \
template <class T1, class T2, class T3, class T4>                       \
RWTEXPORT_RETURN(ISTR&) operator>>(ISTR&, T__CLASS<T1,T2,T3,T4>*&);

#define RWDECLARE_PERSIST_TEMPLATE_IO_5(T__CLASS, ISTR, OSTR)           \
template <class T1, class T2, class T3, class T4, class T5>             \
RWTEXPORT_RETURN(OSTR&) operator<<(OSTR&, const T__CLASS<T1,T2,T3,T4,T5>&); \
template <class T1, class T2, class T3, class T4, class T5>             \
RWTEXPORT_RETURN(ISTR&) operator>>(ISTR&, T__CLASS<T1,T2,T3,T4,T5>&);   \
template <class T1, class T2, class T3, class T4, class T5>             \
RWTEXPORT_RETURN(ISTR&) operator>>(ISTR&, T__CLASS<T1,T2,T3,T4,T5>*&);

#define RWDECLARE_PERSISTABLE(CLASS)                                    \
RWDECLARE_PERSIST_IO(CLASS,RWvistream,RWvostream)                       \
RWDECLARE_PERSIST_IO(CLASS,RWFile,RWFile)

#define RWDECLARE_PERSISTABLE_TEMPLATE(T__CLASS)                        \
RWDECLARE_PERSIST_TEMPLATE_IO(T__CLASS,RWvistream,RWvostream)           \
RWDECLARE_PERSIST_TEMPLATE_IO(T__CLASS,RWFile,RWFile)

#define RWDECLARE_PERSISTABLE_TEMPLATE_2(T__CLASS)                      \
RWDECLARE_PERSIST_TEMPLATE_IO_2(T__CLASS,RWvistream,RWvostream)         \
RWDECLARE_PERSIST_TEMPLATE_IO_2(T__CLASS,RWFile,RWFile)

#define RWDECLARE_PERSISTABLE_TEMPLATE_3(T__CLASS)                      \
RWDECLARE_PERSIST_TEMPLATE_IO_3(T__CLASS,RWvistream,RWvostream)         \
RWDECLARE_PERSIST_TEMPLATE_IO_3(T__CLASS,RWFile,RWFile)

#define RWDECLARE_PERSISTABLE_TEMPLATE_4(T__CLASS)                      \
RWDECLARE_PERSIST_TEMPLATE_IO_4(T__CLASS,RWvistream,RWvostream)         \
RWDECLARE_PERSIST_TEMPLATE_IO_4(T__CLASS,RWFile,RWFile)

#define RWDECLARE_PERSISTABLE_TEMPLATE_5(T__CLASS)                      \
RWDECLARE_PERSIST_TEMPLATE_IO_5(T__CLASS,RWvistream,RWvostream)         \
RWDECLARE_PERSIST_TEMPLATE_IO_5(T__CLASS,RWFile,RWFile)


#ifndef RW_NO_TEMPLATES
/*
 * Here's a useful default hasher that gives the same semantics
 * as the Tools 6 hash tables.  That is, it allows you to construct
 * an stl-based hash collection with a function pointer.
 */

template <class T>
struct RWTHasher {
  // should not be necessary, do not use:
  RWTHasher() : hfun_(NULL) { }

  typedef unsigned (*hashfun)(const T&);

  RWTHasher(hashfun f) : hfun_(f) { }

  unsigned operator()(const T& t) const
    {
      RWPRECONDITION(hfun_ != NULL);
      return hfun_(t);
    }

private:
  hashfun hfun_;
};
#endif /* RW_NO_TEMPLATES */

#endif /* __RWEDEFS_H__ */
