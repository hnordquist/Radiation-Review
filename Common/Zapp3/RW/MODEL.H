#ifndef __RWMODEL_H__
#define __RWMODEL_H__

/*
 * RWModel --- maintains a list of dependent clients
 *
 * $Id: model.h@#/main/13  07/18/96 04:20:22  hart (TOOLS707_WIN32_971021)
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
 * $Log: model.h,v $
 * Revision 7.5  1996/07/18 04:20:22  hart
 * ObjectStore changes
 *
 * Revision 7.4  1996/05/07 23:26:00  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.3  1996/02/18 01:45:21  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.2  1995/09/05 19:11:18  jims
 * Use new copyright macro
 *
 * Revision 7.1  1994/10/16  02:53:22  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.4  1994/07/18  19:04:25  jims
 * Add copyright
 *
 * Revision 6.3  1994/06/16  04:20:24  vriezen
 * Added outlined virtual dtors.
 *
 * Revision 6.2  1994/04/15  21:12:29  vriezen
 * Backed out (until next release) RW_DEFINE_HOME and RW_DECLARE_HOME support.
 *
 * Revision 6.1  1994/04/15  19:10:31  vriezen
 * Move all files to 6.1
 *
 * Revision 2.4  1994/03/31  23:16:08  vriezen
 * Add RW_DECLARE_HOME and RW_DEFINE_HOME for class RWModelClient
 *
 * Revision 2.3  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.1  1992/11/16  04:24:16  keffer
 * Broke out RWModel functionality into RWModel and RWModelClient
 *
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.1   18 Feb 1992 09:55:34   KEFFER
 * 
 *    Rev 1.0   12 Nov 1991 13:13:18   keffer
 * Initial revision.
 */

#include "rw/ordcltn.h"

class RWExport RWModelClient;

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWModel RWBehavior 
{
public:
  virtual ~RWModel();
  RWModel();
  void                  addDependent(RWModelClient*);
  void                  removeDependent(RWModelClient*);
  // Allows peek at dependent list:
  const RWOrdered*      dependents() {return &dependList;}
  virtual void          changed(void* pData = 0);      // Notify all dependents
private:
  RWOrdered             dependList;
  
  RW_TYPESPEC  /* This macro usually expands to nothing */
};


class RWExport RWModelClient : public RWCollectable
{
public:
  virtual ~RWModelClient();
  virtual void          updateFrom(RWModel* model, void* pData) = 0;

  RW_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif  /* __RWMODEL_H__ */
