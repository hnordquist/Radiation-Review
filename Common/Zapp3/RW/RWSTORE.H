#ifndef __RWSTORETABLE_H__
#define __RWSTORETABLE_H__

/*
 * Definitions for internal classes used to store collectables
 *
 * $Id: rwstore.h@#/main/21  08/09/96 19:12:47  hart (TOOLS707_WIN32_971021)
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
 * $Log: rwstore.h,v $
 * Revision 7.21  1996/08/09 19:12:47  hart
 * HP (aCC compiler) port
 *
 * Revision 7.20  1996/07/18 04:25:51  hart
 * ObjectStore changes
 *
 * Revision 7.19  1996/05/07 22:18:05  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.18  1996/03/07 19:54:15  kevinj
 * Bug #2684: Added RW_GLOBAL_BEFORE_CLASS_SCOPE to make inline
 * specialized template operator<= for times when <utility>'s
 * templatized operator<= is used instead of
 * the class member operator<=.
 *
 * Revision 7.17  1996/02/19 16:44:22  pearson
 * Make RWStringID parameter to RWStoreTable::add by reference to accomodate
 * bug in Borland 5.0 compiler
 *
 * Revision 7.16  1996/02/18 01:46:25  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.15  1996/02/14 23:52:07  griswolf
 * fix typo.
 *
 * Revision 7.14  1996/01/29 20:28:52  pearson
 * Specialize global template function operator<= for Borland 5.0
 *
 * Revision 7.13 1996/01/25 20:24:02 pearson #2437 - Override
 * relational operator<= and explicitly forward call to base class
 * because overload resolution chooses global STL definitions which
 * are inadequate for sets, over inherited class members.
 *
 * Revision 7.12  1995/12/19 11:06:28  jims
 * Add RWUseReadTable::getPtr(size_t) to save clients trouble of
 * casting result of operator()
 *
 * Revision 7.11  1995/11/01 17:55:07  griswolf
 * Scopus #2193: Quiet purify by initializing all bytes of
 * RWConstVoidPtr.
 *
 * Revision 7.10  1995/10/23  17:47:30  pearson
 * Add ifdef for RW_TRAILING_RWEXPORT for Watcom C++ 10.5 DLL's
 *
 * Revision 7.9  1995/09/19  10:20:11  jims
 * Add RWExport to RWReadTable
 *
 * Revision 7.8  1995/09/05  19:41:29  jims
 * Use new copyright macro
 *
 * Revision 7.7  1995/06/27  18:30:39  griswolf
 * Change const RWStringID to const RWStringID& so Borland can expand
 * inline.
 *
 * Revision 7.6  1995/06/27  16:46:22  jims
 * DEC cxx doesn't like to init union members in the ctor mem-init list,
 * so init in ctor body instead.
 *
 * Revision 7.5  1995/05/24  21:21:11  griswolf
 * Add capability to keep RWStringID info in read and store tables.
 *
 * Revision 7.4  1995/05/18  21:00:22  griswolf
 * add RWUseReadTable class to protect from memory leaks during restores.
 *
 * Revision 7.3  1995/04/26  17:50:12  jims
 * Now always defines RWnilCollectable as a function call that returns
 * the nil collectable object.  Set on first reference to avoid static init.
 *
 * Revision 7.2  1995/04/12  05:49:24  jims
 * Add firstUse member function to RWUseStoreTable class
 *
 * Revision 7.1  1994/10/16  03:36:56  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.1.7.4  1994/05/16  20:23:58  jims
 * Port to Win32 DLL
 *
 * Revision 6.1.7.3  1994/05/04  22:33:45  vriezen
 * Moved RWnilCollectable declaration to rwstore.h
 *
 * Revision 6.1.7.2  1994/05/02  15:53:43  vriezen
 * Added destructor to RWReadTable
 *
 * Revision 6.1.7.1  1994/04/29  19:55:15  vriezen
 * Adding to Tools 7 branch
 *
 * Revision 6.1  1994/04/29  19:51:31  vriezen
 * Moved from toolsrc directory, for use by template classes
 *
 * Revision 6.1  1994/04/29  19:51:31  vriezen
 * Moved from toolsrc directory, for use by template classes
 *
 * Revision 6.1.7.1  1994/04/15  22:43:46  vriezen
 * Version 7, Initial code
 *
 * Revision 6.1  1994/04/15  19:07:40  vriezen
 * Move all files to 6.1
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.3   13 Nov 1991 11:53:42   keffer
 * Typedefs RWReadTable to an ordered collection.
 * 
 *    Rev 1.1   28 Oct 1991 09:24:26   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 09:23:06   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 */


/**
 *******************  RWStoreTable declarations ***************************
 **/

#include <assert.h>
#include "rw/rwset.h"
#include "rw/stringid.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

#ifdef RW_TRAILING_RWEXPORT
extern RWCollectableP rwexport getRWNilCollectable();
#else
extern rwexport RWCollectableP getRWNilCollectable();
#endif //RW_TRAILING_RWEXPORT

# define RWnilCollectable (getRWNilCollectable())

class RWExport RWStoreTable : public RWSet {
public:
  RWStoreTable();
  ~RWStoreTable();
  RWBoolean             add(const void*, int&);

// RWStringID is by-reference to accomodate bug in Borland 5.0 compiler
  RWBoolean             add(const RWStringID&, int&);  

// #2437 - Forward calls to relational ops. in presence of C++ Std. Lib
#ifndef RW_NO_STL
  RWBoolean             operator<=(const RWStoreTable& rws)
                        { return RWSet::operator<=(rws); }
#endif

  static RWStoreTable* getStoreTable();
  static RWStoreTable* newStoreTable();
  static void freeStoreTable();

  RW_TYPESPEC  /* This macro usually expands to nothing */
};

class RWExport RWUseStoreTable {
  RWBoolean  firstUse_;
  RWStoreTable * table_;
  RWUseStoreTable(const RWUseStoreTable&); // Not defined!
  void operator=(const RWUseStoreTable&);  // Not defined!
public:
  RWUseStoreTable();
  RWBoolean firstUse() { return firstUse_; }
  RWBoolean add(const void* p, int& i)
    { assert(0 != table_); return table_->add(p,i); }
  RWBoolean add(const RWStringID& p, int& i)
    { assert(0 != table_); return table_->add(p,i); }
  ~RWUseStoreTable();
};

class RWExport RWStoreEntry : public RWCollectable {
friend class RWStoreTable;
  const void*           item;
  int                   objectNumber;
public:
  RWStoreEntry(const void* c, int n): item(c), objectNumber(n) {}
  unsigned              hash() const;
  RWBoolean             isEqual(const RWCollectable* c) const;

public:
  RW_TYPESPEC  /* This macro usually expands to nothing */
};


class RWExport RWStoreStringID : public RWCollectable {
friend class RWStoreTable;
  const RWStringID      item;
  int                   objectNumber;
public:
  RWStoreStringID(const RWStringID& id, int n) : item(id), objectNumber(n) {}
  unsigned              hash() const;
  RWBoolean             isEqual(const RWCollectable* c) const;

  RW_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

/**
 *******************  RWReadTable declarations ***************************
 **/

// For the read table, just use a simple extendable vector of pointers or
// RWClassID. We may assume that both are numeric types and that RWClassID
// is no greater than a pointer. So the union works fine.

struct RWConstVoidPtr {
  union {
    const void   *ptr_;
    RWClassID    id_;
  };

  // DEC cxx doesn't like union members in mem-init list:
  RWConstVoidPtr() { ptr_ = 0; }
  RWConstVoidPtr(void* p) { ptr_ = p; }
  RWConstVoidPtr(RWClassID i) { ptr_ = 0; id_ = i; }

  operator const void*() { return ptr_; }
  operator RWClassID() { return id_; }
};

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWReadTable {
  RWConstVoidPtr *table_;
  size_t entries_;
  size_t capacity_;
  enum {InitialSize = 30, IncrementSize = 100};
public:
  RWReadTable();
  ~RWReadTable();
  RWConstVoidPtr append(RWConstVoidPtr);
  size_t entries() { return entries_; }
  
  RWConstVoidPtr operator() (size_t i) { return table_[i]; }

  static RWReadTable* getReadTable();
  static RWReadTable* newReadTable();
  static void freeReadTable();
};

class RWExport RWUseReadTable {
  RWBoolean firstUse_;
  RWReadTable *table_;
  RWUseReadTable(const RWUseReadTable&); // Not defined
  void operator=(const RWUseReadTable&); // Not defined
public:
  RWUseReadTable();
  ~RWUseReadTable();
  RWBoolean       firstUse() const { return firstUse_; }
  RWConstVoidPtr append(RWConstVoidPtr p)
    { assert(0 != table_); return table_->append(p); }
  size_t          entries() { return table_->entries(); }
  RWConstVoidPtr  operator() (size_t i) { return (*table_)(i); }
  const void*     getPtr(size_t i)
    { return (const void*)((*this)(i)); }
};

//Special fix for Borland 5.0, this is related to bug #2437.  Borland 5.0
//will not choose class scope over global scope ever, but will recognize
//this as a function template specialization.

#if !defined(RW_NO_STL) && (defined __TURBOC__ && (__TURBOC__ < 0x500))
inline bool operator<=
(const RWStoreTable& wasThis, const RWStoreTable& btr)
{
     return wasThis.RWSet::operator<=(btr);
}
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif  /* __RWSTORETABLE_H__ */
