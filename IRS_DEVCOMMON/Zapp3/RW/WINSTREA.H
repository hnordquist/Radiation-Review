#ifndef __RWWINSTREAM_H__
#define __RWWINSTREAM_H__

/*
 * RWCLIPstreambuf & RWDDEstreambuf
 *   : Specialized streambufs, used for MS Windows (TM)
 *
 * $Id: winstrea.h@#/main/11  08/17/96 00:41:00  griswolf (TOOLS707_WIN32_971021)
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
 * Under Windows, note the near similarity of the two constructors
 *
 *      RWCLIPstreambuf(int N);         // Dynamic.  Initial allocation of "N"
 *      RWCLIPstreambuf(HANDLE hMem);   // Static.   Use memory "hMem".
 *
 * This is because "HANDLE" has been typedef'd to "unsigned int", an
 * unfortunate consequence of Window's generally poor type checking.
 * Yet, the two constructors have vastly different semantics.
 * 
 * It is for this reason that an extra dummy argument was added to
 * the first of these constructors:
 *
 *      RWCLIPstreambuf(int N, int);    // 2'nd argument ignored.
 *
 * This dummy argument is ignored.
 *
 ***************************************************************************
 *
 * $Log: winstrea.h,v $
 * Revision 7.6  1996/08/17 00:41:00  griswolf
 * Note that the classes are not yet supported under MSVC 4.2
 *
 * Revision 7.5  1996/02/26 23:23:49  pearson
 * Add RWCLIPstreambuf to the library for 32-bit Windows
 *
 * Revision 7.4  1996/02/18 01:50:36  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.3  1995/12/05 23:07:08  pearson
 * Remove ZTC ifdef
 *
 * Revision 7.2  1995/09/05 21:34:37  jims
 * Use new copyright macro
 *
 * Revision 7.1  1994/10/16  03:13:08  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.2  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 *
 * Revision 6.1  1994/04/15  19:48:51  vriezen
 * Move all files to 6.1
 *
 * Revision 2.3  1994/03/18  18:42:16  jims
 * Override virtual member function sync() in RWCLIPStreambuf
 *
 * Revision 2.2  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.4   18 Feb 1992 09:54:52   KEFFER
 * 
 *    Rev 1.3   12 Nov 1991 13:16:16   keffer
 * seek_dir is now scoped for C++ V2.1 compiliant compilers.
 * 
 *    Rev 1.2   05 Nov 1991 13:53:26   keffer
 * Can now live in the DLL.
 * Scoped seek_dir with ios::.
 * 
 *    Rev 1.1   28 Oct 1991 09:08:36   keffer
 * Changed inclusions to <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 08:18:12   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/defs.h"
#include <windows.h>
#include "rw/rstream.h"
#ifndef RW_NO_IOSTD
#error These classes derived from streambuf are not yet supported
#endif

class RWExport RWCLIPstreambuf : public streambuf {
protected:
  int                   frozen;
  int                   autoAlloc;
  int                   locked;
  HANDLE                hMem;
protected:
  // Virtual functions inherted from streambuf:
  virtual int           doallocate();
  virtual int           overflow(int);
  virtual streampos     seekoff(streamoff, RWSCOPE(ios)seek_dir, int);
  virtual streambuf*    setbuf(char* s, int n);
  virtual int           underflow();
  virtual int           sync();

  // Virtual function introduced here:
  virtual HANDLE        getHandle(DWORD size);  // Type of handle can be overridden

public: 
  RWCLIPstreambuf();            // Dynamic mode
  RWCLIPstreambuf(int N, int);  // Dynamic mode w. initial allocation N; see notes above
  RWCLIPstreambuf(HANDLE hMem); // Static mode, init w. block at hMem
  ~RWCLIPstreambuf();           // Free any unfrozen memory

  void                  freeze(int n = TRUE);
  HANDLE                str();  // NB: return type
};


#ifndef __WIN32__

class RWExport RWDDEstreambuf : public RWCLIPstreambuf {
  WORD                  wFormat_;
  BOOL                  fResponse_;
  BOOL                  fAckReq_;
  BOOL                  fRelease_;
protected:
  // Virtual functions overridden from RWCLIPstreambuf:
  virtual HANDLE        getHandle(DWORD size);
  virtual int           doallocate();
public:
  /* This form usually used by server: */
  RWDDEstreambuf(WORD format,           // Format of data
                 BOOL response,         // In response to WM_DDE_REQUEST; otherwise WM_DDE_ADVISE
                 BOOL ackReq,           // Requests an acknowledment
                 BOOL release);         // Client responsible for memory release

  RWDDEstreambuf(HANDLE hMem);  /* Usually used by client */

  WORD                  format() const   {return wFormat_;}
  BOOL                  response() const {return fResponse_;}
  BOOL                  ackReq() const   {return fAckReq_;}
  BOOL                  release() const  {return fRelease_;}
};

#endif  // #ifndef __WIN32__  (RWDDEstreambuf only works in 16-bit windows)
#endif /*__RWWINSTREAM_H__*/
