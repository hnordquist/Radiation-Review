#ifndef __RWEV_MCNTN_H__
#define __RWEV_MCNTN_H__

/***************************************************************************
 *
 * ev_mcntn.h - RW_VMapCntnr<StdColl,RWColl,Key,T>
 *   
 *     : value-based Key/Data interface components for STL Containers
 * 
 * $Id: ev_mcntn.h@#/main/19  08/20/96 19:22:05  hart (TOOLS707_WIN32_971021)
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
 * $Log: ev_mcntn.h,v $
 * Revision 7.20  1996/08/20 19:22:05  hart
 * add typedefs for const_my_iter_ and my_iter_ which are used in .cc
 *
 * Revision 7.19  1996/08/16 21:31:02  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.18  1996/08/09 19:05:27  hart
 * HP (aCC compiler) port
 *
 * Revision 7.17  1996/07/18 04:27:33  hart
 * ObjectStore changes
 *
 * Revision 7.16  1996/05/07 23:45:49  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.15  1996/02/18 01:41:30  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.14  1995/12/20 23:19:20  kevinj
 * External template persistence using RW*Persistor.
 *
 * Revision 7.13  1995/12/15 10:19:35  jims
 * work around limitation of compilers that don't allow default
 * arguments in function templates
 *
 * Revision 7.12  1995/12/14 18:01:21  jims
 * saveOn, restoreFrom now global instead of member functions
 *
 * Revision 7.11  1995/12/12 00:03:06  jims
 * Add RWTExport
 *
 * Revision 7.10  1995/12/11 22:30:41  jims
 * Remove 'Sca' from interior class names
 * Add 'rw_' to global helper classes
 * Change 'muterator' to 'filterator'
 *
 * Revision 7.9  1995/09/05 21:53:21  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.8  1995/08/23  22:42:27  kevinj
 * Changed saveOn and restoreFrom to return bool.
 *
 * Revision 7.7  1995/08/14  05:58:25  jims
 * include edefs.h, forward declare RWFile, RWv[io]stream
 *
 * Revision 7.6  1995/08/12  00:23:12  kevinj
 * restoreFrom and saveOn for RWFile.
 *
 * Revision 7.5  1995/08/10  23:42:13  kevinj
 * Moved inline functions out of line.
 *
 * Revision 7.4  1995/07/27  22:02:33  jims
 * Add header with file name, short description, $Id and $Log
 * 
 ****************************************************************************/

#include "rw/edefs.h"
#include "rw/ev_cntnr.h"

class RWExport RWFile;
class RWExport RWvistream;
class RWExport RWvostream;

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl, class Key, class T>
class RWTExport RW_VMapCntnr : public RW_VBase<StdColl, RWColl>
{
public:
  typedef typename StdColl::value_type               value_type;
  typedef typename StdColl::size_type                size_type;
  typedef typename StdColl::const_reference          const_reference;
  typedef typename StdColl::const_iterator           const_iterator;
  typedef typename StdColl::const_iterator           const_my_iter_;
  typedef typename StdColl::iterator                 my_iter_;
  typedef typename StdColl::iterator                 iterator;
  typedef typename StdColl::reference                reference;

  bool          find(
                        const_reference,
                        pair<Key,T>& val
                      ) const;

#ifndef RW_REF_SEEMS_PTR
  bool    find(
            bool(*fn)(const_reference,void*),
            void* x, pair<Key,T>& val
                      ) const;
#endif

protected:
  RW_VMapCntnr() { } // prevent direct usage
  RW_VMapCntnr(const RW_VMapCntnr<StdColl,RWColl,Key,T>&) { }
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ev_mcntn.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEV_MCNTN_H__ */
