#ifndef __RW_STRINGID_H__
#define __RW_STRINGID_H__

/*
 * Implements a richer set of IDs than RWClassID
 *
 * $Id: stringid.h@#/main/21  08/14/96 23:12:06  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: stringid.h,v $
 * Revision 7.21  1996/08/14 23:12:06  griswolf
 * Port to standard iostreams.
 *
 * Revision 7.20  1996/07/18 04:16:22  hart
 * ObjectStore changes
 *
 * Revision 7.19  1996/06/18 19:27:48  griswolf
 * Scopus 3693: RWExport doesn't always work for partial declarations.
 *
 * Revision 7.18  1996/05/22 15:29:28  griswolf
 * Export RWStringID for those OSs that need the help.
 *
 * Revision 7.17  1996/05/17 18:15:52  griswolf
 * Scopus #3273: Remove redundant inline method last(char) const.
 *
 * Revision 7.16  1996/03/14 22:54:59  kyle
 * Millenium adds support for RW_HEADER_WITH_EXTENSION
 *
 * Revision 7.15  1996/02/18 01:46:47  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.14  1996/01/30 16:44:19  pearson
 * Include utility only after cstring.h is processed
 *
 * Revision 7.13  1996/01/24 17:27:45  pearson
 * #2437   When the C++ Standard Library is available, include <utility> so
 *         that the global template definitions of <= >= > and != are present
 *         Otherwise, use those definitions provided by Tools.
 *
 * Revision 7.12  1995/12/20  17:49:00  pearson
 * Comment out <= >= > and != operators defined for (const RWStringID &,
 * const RWStringID &) because these conflict with the global templated
 * operator definitions provided by the C++ Standard Library
 *
 * Revision 7.11  1995/10/23 17:39:37  pearson
 * Include index(const char*, size_t ext, size_t i) under RW_NO_STL exclusion
 * because Watcom DLL's require a definition for every exported name
 *
 * Revision 7.10  1995/09/05  19:44:08  jims
 * Use new copyright macro
 *
 * Revision 7.9  1995/07/13  14:36:53  griswolf
 * Scopus #1960: Overloaded last() in RWCString, so must forward it for
 * CFront compilers in RWStringID.
 *
 * Revision 7.8  1995/07/06  18:46:52  jims
 * Modified static hash function to use member hash function instead of
 * static base class (RWCString) hash function.  Avoids odd ambiguity
 * reported by SunPro 4.0.
 *
 * Revision 7.7  1995/07/06  18:11:46  jims
 * Make member function hash() const
 *
 * Revision 7.6  1995/07/03  23:24:54  griswolf
 * Scopus #1741: cfront cannot adjust access to two hash methods, so
 * inline them. Add inline forwarding for RW_NO_ACCESS_ADJUST.
 *
 * Revision 7.5  1995/06/27  23:40:49  kevinj
 * #1876: Added RW_NO_STL to guard against futile compilation of classes
 * that depend on the existence of C++ Standard Library containers
 * and iterators.
 *
 * Revision 7.4  1995/06/26  23:37:53  griswolf
 * Change friend operator functions to take (const) references.
 *
 * Revision 7.3  1995/06/26  18:05:21  griswolf
 * Move to the version 7 branch level
 *
 * Revision 1.3  1995/06/23  23:04:54  jims
 * Remove access adjustment for overloaded member functions
 *
 * Revision 1.2  1995/05/31  22:10:32  griswolf
 * move friend functions inside class to help xlC
 */

#include "rw/vstream.h"
#include "rw/rwfile.h"
#include "rw/cstring.h"

#ifndef RW_NO_STL
#  ifndef RW_HEADER_WITH_EXTENSION
#    include<utility>
#  else
#    include<utility.hpp>
#  endif /* RW_HEADER_WITH_EXTENSION */
#endif /* RW_NO_STL */

#if defined(_RWTOOLSDLL)  /*_export variants very complex to get right */
#  include "rw/rstream.h" /*so: use header*/
#else
#  ifndef RW_NO_IOSTD
#    include <iosfwd>
#  else
     class ostream;
     class istream;
#  endif
#endif

class RWExport RWStringID : protected RWCString {
public:
  RWStringID()
    : RWCString("NoID") {}

  RWStringID(const RWStringID& sid)
    : RWCString((const RWCString&)sid) {}

  RWStringID(const RWCString& s)
    : RWCString(s) {}

  RWStringID(const char* name)
    : RWCString(name) {}

  ~RWStringID() {}

  RWStringID& operator=(const RWStringID& sid)
  { return (RWStringID&)RWCString::operator=(sid);  }

  RWStringID& operator=(const RWCString& s)
  { return (RWStringID&)RWCString::operator=(s);  }

  operator const char*() const { return RWCString::data(); }
  char  operator[](size_t t) const {return RWCString::operator[](t); }
  char  operator()(size_t t) const {return RWCString::operator()(t); }

#ifndef RW_NO_ACCESS_ADJUSTMENT
  RWCString::binaryStoreSize;
  RWCString::copy;
  RWCString::data;
  RWCString::isAscii;
  RWCString::isNull;
  RWCString::length;
#else
  RWspace       binaryStoreSize() const
    { return RWCString::binaryStoreSize(); }
  RWCString     copy() const 
    { return RWCString::copy(); }
  const char*   data() const
    { return RWCString::data(); }
  RWBoolean     isAscii() const
    { return RWCString::isAscii(); }
  RWBoolean     isNull() const
    { return RWCString::isNull(); }
  size_t        length(char c) const
    { return RWCString::length(); }
#endif  

/*
 * cfront does not implement access adjustment
 * of overloaded member functions:
 */
  size_t        capacity() const
    { return RWCString::capacity(); }
  size_t        capacity(size_t N)
    { return RWCString::capacity(N); }
#ifndef RW_NO_LOCALE
  int           collate(const char* cs) const
    { return RWCString::collate(cs); }
  int           collate(const RWCString& st) const
    { return RWCString::collate(st); }
#endif
  int           compareTo(const char* cs,      caseCompare cmp = exact) const
    { return RWCString::compareTo(cs,cmp); }
  int           compareTo(const RWCString& st, caseCompare cmp = exact) const
    { return RWCString::compareTo(st,cmp); }
  RWBoolean     contains(const char* pat,      caseCompare cmp = exact) const
    { return RWCString::contains(pat,cmp); }
  RWBoolean     contains(const RWCString& pat, caseCompare cmp = exact) const
    { return RWCString::contains(pat,cmp); }
  size_t        first(char c) const
    { return RWCString::first(c); }
  size_t        first(const char* cs) const
    { return RWCString::first(cs); }
  unsigned      hash() const 
    { return RWCString::hash(); }
  size_t index(const char* pat, size_t i=0, caseCompare cmp = exact) const
    { return RWCString::index(pat,i,cmp); }
  size_t index(const RWCString& s, size_t i=0, caseCompare cmp = exact) const
    { return RWCString::index(s,i,cmp); }
  size_t index(const char* pat, size_t patlen, size_t i,caseCompare cmp) const
    { return RWCString::index(pat,patlen,i,cmp); }
  size_t index(const RWCString& s, size_t patlen, size_t i, caseCompare cmp) const
    { return RWCString::index(s,patlen,i,cmp); }
# if !defined(RW_NO_STL)
  size_t        index(const RWCRExpr& pat, size_t i=0) const
    { return RWCString::index(pat,i); }
  size_t        index(const RWCRExpr& pat, size_t* ext, size_t i=0) const
    { return RWCString::index(pat,ext,i); }
  size_t        index(const char* pat, size_t* ext, size_t i=0) const
    { return RWCString::index(pat,ext,i); }
# endif
  size_t        index(const RWCRegexp& pat, size_t i=0) const
    { return RWCString::index(pat,i); }
  size_t        index(const RWCRegexp& pat, size_t* ext, size_t i=0) const
    { return RWCString::index(pat,ext,i); }
  size_t        last(char c) const
    { return RWCString::last(c); }
  size_t        last(char c, size_t i) const
    { return RWCString::last(c,i); }

  void          restoreFrom(RWvistream& s)    { RWCString::restoreFrom(s); }
  void          restoreFrom(RWFile& f)        { RWCString::restoreFrom(f); }
  void          saveOn(RWvostream& s) const   { RWCString::saveOn(s); }
  void          saveOn(RWFile& f) const       { RWCString::saveOn(f); }


  friend  RWBoolean operator==(const RWStringID& lhs, const RWStringID& rhs)
  { return (const RWCString&)lhs == (const RWCString&)rhs; } 
  friend  RWBoolean operator==(const RWStringID& lhs, const char* rhs)
  { return (const RWCString&)lhs == rhs; } 
  friend  RWBoolean operator==(const char* lhs, const RWStringID& rhs)
  { return lhs == (const RWCString&)rhs; } 

  friend  RWBoolean operator!=(const RWStringID& lhs, const char* rhs)
  { return (const RWCString&)lhs != rhs; }
  friend  RWBoolean operator!=(const char* lhs, const RWStringID&  rhs)
  { return lhs != (const RWCString&)rhs; } 

  friend  RWBoolean operator<(const RWStringID& lhs, const RWStringID& rhs)
  { return (const RWCString&)lhs < (const RWCString&)rhs; }

#if defined(RW_NO_STL)
  friend  RWBoolean operator!=(const RWStringID& lhs, const RWStringID& rhs)
  { return (const RWCString&)lhs != (const RWCString&)rhs; }

  friend  RWBoolean operator>(const RWStringID& lhs, const RWStringID& rhs)
  { return (const RWCString&)lhs > (const RWCString&)rhs; }

  friend  RWBoolean operator<=(const RWStringID& lhs, const RWStringID& rhs)
  { return (const RWCString&)lhs <= (const RWCString&)rhs; }

  friend  RWBoolean operator>=(const RWStringID& lhs, const RWStringID& rhs)
  { return (const RWCString&)lhs >= (const RWCString&)rhs; }
#endif

  friend RWvostream& operator<<(RWvostream& vos, const RWStringID& sid)
  { sid.saveOn(vos); return vos; }

  friend RWvistream& operator>>(RWvistream& vis, RWStringID& sid)
  { sid.restoreFrom(vis); return vis; }

  friend RWFile& operator<<(RWFile& file, const RWStringID& sid)
  { sid.saveOn(file); return file; }

  friend RWFile& operator>>(RWFile& file, RWStringID& sid)
  { sid.restoreFrom(file); return file; }

  friend ostream& operator<<(ostream& os, const RWStringID& sid)
  { os << (const RWCString&)sid; return os; }

  friend istream& operator>>(istream& is, RWStringID& sid)
  { is >> (RWCString&)sid; return is; }

static unsigned         hash(const RWStringID& sid)
    { return sid.hash(); }

  RW_TYPESPEC  /* This macro usually expands to nothing */
};

#endif /*__RW_STRINGID_H__*/
