#ifndef __RWAUDIT_STREAMBUFFER_H__
#define __RWAUDIT_STREAMBUFFER_H__
/*
 * A streambuf that counts and allows you to "audit" each byte as it
 * passes through the buffer.
 *
 * $Id: auditbuf.h@#/main/18  01/30/97 08:29:22  vobadm (TOOLS707_WIN32_971021)
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
 * $Log: auditbuf.h,v $
 * Revision 7.18  1996/12/24 20:59:05  griswolf
 * Port to more current standard library.
 *
 * Revision 7.17  1996/08/14 17:49:32  griswolf
 * Add support for MSVC4.2 standard library.
 *
 * Revision 7.16  1996/05/08 18:52:49  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.15  1996/02/19 10:53:15  jims
 * Change RWAuditFunction to RWauditFunction
 *
 * Revision 7.14  1996/02/18 05:14:27  kyle
 * Millenium added support for RW_NO_IOSTREAM compiler flag.
 *
 * Revision 7.13  1996/02/17 23:10:53  griswolf
 * Replace all tabs with spaces per Rogue Wave standards.
 *
 * Revision 7.12  1996/02/17 00:28:52  griswolf
 * Lowercase the 'A' in RWauditfunction to match Tools.h++ standards.
 *
 * Revision 7.11  1995/10/02 17:52:33  pearson
 * Add include"rw/defs.h" to define RWExport
 *
 * Revision 7.10  1995/09/22  00:05:50  jims
 * Buffer of size 1 is sufficient for myBuf[]
 *
 * Revision 7.9  1995/09/20  10:11:04  jims
 * Moved some comments to auditbuf.cpp
 *
 * Revision 7.8  1995/09/20  08:00:07  jims
 * Increase size of buffer to allow for distinct base and gptr values
 *
 * Revision 7.7  1995/09/19  10:18:22  jims
 * Add RWExport
 *
 * Revision 7.6  1995/09/05  17:51:18  jims
 * Use new copyright macro
 *
 * Revision 7.5  1995/07/18  16:38:23  griswolf
 * Add iostream ctor and reset member function per QA's request.
 *
 * Revision 7.4  1995/07/14  20:59:44  griswolf
 * Fix prototype for RWAuditFunction.
 *
 * Revision 7.3  1995/06/16  16:01:49  griswolf
 * Better name for RWAuditFunction.
 *
 * Revision 7.2  1995/06/12  21:31:12  griswolf
 * fix typo in setbuf method.
 *
 * Revision 7.1  1995/06/12  17:50:40  griswolf
 * Initial revision. Addresses scopus bug #1794.
 */

#include "rw/rstream.h"
#include "rw/defs.h"
#if !defined(RW_NO_IOSTD)
#  if defined(RW_MSVC_STDLIB)
#    include <iosfwd>     // typedef for streamsize the MSVC way
#  else
#    include <ios>        // typedef for streamsize the standard way
#  endif
#else
  typedef int streamsize; // pre-standard, streamsize was an int
#endif

typedef void (*RWauditFunction)(unsigned char, void*);

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWAuditStreamBuffer : public streambuf {
  unsigned long     seenBytes_;
  RWauditFunction   funcP_;
  void*             funcV_;
  streambuf*        realBuf_;
  char              myBuf[1]; // Required for underflow and sputbackc.

public:
  RWAuditStreamBuffer(RWauditFunction cb=0, void* x=0);

  RWAuditStreamBuffer(istream& is, RWauditFunction cb=0, void* x=0);

#ifndef RW_NO_IOSSTREAM		// jvd
  RWAuditStreamBuffer(iostream& os, RWauditFunction cb=0, void* x=0);
#endif

  RWAuditStreamBuffer(ostream& os, RWauditFunction cb=0, void* x=0);

  RWAuditStreamBuffer(streambuf* sb, RWauditFunction cb=0, void* x=0);

  virtual ~RWAuditStreamBuffer();

  operator unsigned long() { return seenBytes_; }

  unsigned long reset(unsigned long val = 0ul) 
  { unsigned long ret = seenBytes_; seenBytes_ = val; return ret; }

  streambuf* setbuf(char*, streamsize) {
    /* does nothing: You may NOT set a buffer for this streambuf! */
    return 0;
  }

protected:
// input
  virtual int underflow();
  virtual streamsize xsgetn(char* s, streamsize n);
// output
  virtual int overflow(int i);
  streamsize xsputn(const char* s, streamsize n);
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWAUDIT_STREAMBUFFER_H__ */
