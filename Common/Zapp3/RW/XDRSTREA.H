#ifndef __RWXDRSTREA_H__
#define __RWXDRSTREA_H__

/*
 * rwxdr[io]stream ---  Portable I/O streams based on XDR format.
 *
 * $Id: xdrstrea.h@#/main/16  09/04/97 17:26:11  griswolf (TOOLS707_WIN32_971021)
 *
 ****************************************************************************
 *
 *      Copyright (C) 1992 Sun Microsystems, Inc.  All Rights Reserved.
 *
 ****************************************************************************
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
 ***************************************************************************/

#include "rw/compiler.h"
STARTWRAP
#include <stddef.h>     /* Required for IBM xlC */
#include <stdio.h>      /* Required for IBM xlC */
ENDWRAP
#ifndef __hpux
STARTWRAP
#  include <rpc/types.h>  /* Due to bug in some rpc/xdr.h. */
ENDWRAP
/* Sun cfront V2.X has bug in prototype for xdr_u_char(); see toolread.doc */
#else
/*******************************************************************/
// since hp has made it impossible to use their <rpc/types.h>
// for some of what we do, we will prevent entering it...
#  ifndef _RPC_TYPES_INCLUDED
#  define _RPC_TYPES_INCLUDED
// but since we do that, we had _better_ do everything it did...
#  include <sys/types.h>
#  include <sys/time.h>	       
   extern "C" {
////// including the following weird or apparently useless stuff
#  define bool_t	int
#  define enum_t	int
#  define KU_SERVER 1
#  define KU_CLIENT 0
#  define __dontcare__	(-1)
////// here is what we needed //////////////////////
#  define mem_alloc(bsize)	_rpc_malloc(bsize)
#  define mem_free(ptr, bsize)	_rpc_free(ptr)
   char *_rpc_malloc(u_int);
   void _rpc_free(char*);
////////////////////////////////////////////////////
  }
#endif /* not _RPC_TYPES_INCLUDED */
/*******************************************************************/
#endif /* __hpux */
STARTWRAP
#include <rpc/xdr.h>
ENDWRAP
#include "rw/vstream.h"

class RWExport RWCollectable;

int xdr(XDR*, RWCollectable*&);

/************************************************
 *                                              *
 *              class RWXDRIStream              *
 *                                              *
 ************************************************/

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWXDRistream : public RWvistream, public RWios
{

public:

  RWXDRistream(XDR*);
  RWXDRistream(streambuf*);
  RWXDRistream(istream&);
  virtual ~RWXDRistream();

  virtual int           eof()           {return RWios::eof();    }
  virtual int           fail()          {return RWios::fail();   }
  virtual int           bad()           {return RWios::bad();    }
  virtual int           good()          {return RWios::good();   }
  virtual int           rdstate()       {return RWios::rdstate();}
  virtual void          clear(int v=0)  {       RWios::clear(v); }

  virtual int           get();
  virtual RWvistream&   get(char&);
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvistream&   get(wchar_t&);
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvistream&   get(unsigned char&);
#endif
#ifndef RW_NO_BOOL
  virtual RWvistream&   get(RWBoolean& b);
  virtual RWvistream&   get(RWBoolean*, size_t N);
#endif
  virtual RWvistream&   get(char*  , size_t);
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvistream&   get(unsigned char*  , size_t);
#endif
  virtual RWvistream&   get(         double*, size_t);
  virtual RWvistream&   get(         float* , size_t);
  virtual RWvistream&   get(         int*   , size_t);
  virtual RWvistream&   get(unsigned int*   , size_t);
  virtual RWvistream&   get(         long*  , size_t);
  virtual RWvistream&   get(unsigned long*  , size_t);
  virtual RWvistream&   get(         short* , size_t);
  virtual RWvistream&   get(unsigned short* , size_t);
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvistream&   get(wchar_t*, size_t N);
#endif
  virtual RWvistream&   getString(char* s  , size_t maxlen);
#ifdef RW_FIX_XSIZE_T
  virtual RWvistream&   getSizeT(size_t&);
#endif
  virtual RWvistream&   operator>>(char&  );
//virtual RWvistream&   operator>>(bool& b ) { return this->get(b); }
  virtual RWvistream&   operator>>(double&);
  virtual RWvistream&   operator>>(float& );
  virtual RWvistream&   operator>>(int&   );
  virtual RWvistream&   operator>>(long&  );
  virtual RWvistream&   operator>>(short& );
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvistream&   operator>>(wchar_t&);
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvistream&   operator>>(unsigned char& );
#endif
  virtual RWvistream&   operator>>(unsigned int&  );
  virtual RWvistream&   operator>>(unsigned long& );
  virtual RWvistream&   operator>>(unsigned short&);

private:

  XDR*  xdrp;
  int   streambufXDR; 
};

/************************************************
 *                                              *
 *              class RWXDROStream              *
 *                                              *
 ************************************************/

class RWExport RWXDRostream : public RWvostream, public RWios
{

public:

  RWXDRostream(XDR*);
  RWXDRostream(streambuf*);
  RWXDRostream(ostream&);
  virtual ~RWXDRostream();

  virtual int           eof()           {return RWios::eof();    }
  virtual int           fail()          {return RWios::fail();   }
  virtual int           bad()           {return RWios::bad();    }
  virtual int           good()          {return RWios::good();   }
  virtual int           rdstate()       {return RWios::rdstate();}
  virtual void          clear(int v=0)  {       RWios::clear(v); }

  virtual RWvostream&   putString(const char*, size_t);
  virtual RWvostream&   operator<<(const char*);
  virtual RWvostream&   operator<<(char);
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvostream&   operator<<(wchar_t);
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvostream&   operator<<(unsigned char);
#endif
//virtual RWvostream&   operator<<(         bool b){ return this->put(b); }
  virtual RWvostream&   operator<<(         double);
  virtual RWvostream&   operator<<(         float );
  virtual RWvostream&   operator<<(         int   );
  virtual RWvostream&   operator<<(unsigned int   );
  virtual RWvostream&   operator<<(         long  );
  virtual RWvostream&   operator<<(unsigned long  );
  virtual RWvostream&   operator<<(         short );
  virtual RWvostream&   operator<<(unsigned short );
  virtual RWvostream&   flush();

  virtual RWvostream&   put(char);
  virtual RWvostream&   put(const char* p, size_t N);
#ifndef RW_NO_BOOL
  virtual RWvostream&   put(RWBoolean);
  virtual RWvostream&   put(const RWBoolean* p, size_t N);
#endif
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvostream&   put(wchar_t);
  virtual RWvostream&   put(const wchar_t* p, size_t N);
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvostream&   put(unsigned char);
  virtual RWvostream&   put(const unsigned char* p, size_t N);
#endif
  virtual RWvostream&   put(const          short* , size_t);
  virtual RWvostream&   put(const unsigned short* , size_t);
  virtual RWvostream&   put(const          int*   , size_t);
  virtual RWvostream&   put(const unsigned int*   , size_t);
  virtual RWvostream&   put(const          long*  , size_t);
  virtual RWvostream&   put(const unsigned long*  , size_t);
  virtual RWvostream&   put(const          float* , size_t);
  virtual RWvostream&   put(const          double*, size_t);
#ifdef RW_FIX_XSIZE_T
  virtual RWvostream&   putSizeT(size_t);
#endif

private:

  XDR*  xdrp;
  int   streambufXDR;
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWXDRSTREA_H__ */
