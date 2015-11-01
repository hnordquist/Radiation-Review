#ifndef __RWDISKPAGE_H__
#define __RWDISKPAGE_H__

/*
 * RWDiskPageHeap
 *
 *       : Specializing buffered page heap that swaps pages out to disk.
 *
 * $Id: diskpage.h@#/main/8  05/08/96 16:59:47  kyle (TOOLS707_WIN32_971021)
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
 * $Log: diskpage.h,v $
 * Revision 7.4  1996/05/08 16:59:47  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.3  1996/02/18 01:39:52  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.2  1995/09/05 18:29:19  jims
 * Use new copyright macro
 *
 * Revision 7.1  1994/10/16  02:48:33  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.2  1994/07/12  18:46:56  vriezen
 * Update Copyright notice
 *
 * Revision 6.1  1994/04/15  19:09:48  vriezen
 * Move all files to 6.1
 *
 * Revision 2.2  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.0   11 Mar 1992 14:09:58   KEFFER
 * Initial revision.
 */

#include "rw/bufpage.h"
#include "rw/bitvec.h"
STARTWRAP
#include <stdio.h>
ENDWRAP

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWDiskPageHeap : public RWBufferedPageHeap {

public:

  RWDiskPageHeap(const char* filename=0, unsigned nbufs=10, unsigned pgsize=512);
  virtual               ~RWDiskPageHeap();

  RWBoolean             isValid() const {return tempfp_!=0;}

  // Inherited from RWPageBuffer:
  virtual RWHandle      allocate();
  virtual void          deallocate(RWHandle);

protected:

  enum HandleStatus { NotUsed, NoSwapSpace, HasSwapSpace };

  RWBoolean             allocateDiskPage(RWHandle);
  RWBoolean             handleValid(RWHandle);
  RWoffset              offsetOfHandle(RWHandle);
  void                  resize(unsigned);

  // Inherited from RWBufferedPageHeap:
  virtual RWBoolean     swapIn(RWHandle, void*);
  virtual RWBoolean     swapOut(RWHandle, void*);

private:

  static const unsigned         initialPages_;
  static const unsigned         initialHandles_;
  static const unsigned         pageIncrement_;
  static const unsigned         handleIncrement_;
  RWBitVec                      freePageMap_;   // Bit flags for free disk pages
  unsigned*                     handleMap_;     // Array that maps from handle number to disk page
  HandleStatus*                 handleStatus_;  // Status of each slot in handleMap_
  unsigned                      nHandles_;      // Length of handleMap_ and handleStatus_
  FILE*                         tempfp_;

};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif  /* __RWDISKPAGE_H__ */
