#ifndef __RWBSTREAM_H__
#define __RWBSTREAM_H__

/*
 * rwb[io]stream ---    Use Binary reads and writes
 *
 * $Id: bstream.h@#/main/22  09/03/97 18:37:52  griswolf (TOOLS707_WIN32_971021)
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

#include "rw/vstream.h"

/************************************************
 *                                              *
 *              class RWbistream                *
 *                                              *
 ************************************************/

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWbistream : public virtual ios, public RWvistream
{

public:

  RWbistream(istream& str);
  RWbistream(streambuf* s);
  virtual ~RWbistream();

  virtual int           eof()          {return ios::eof();    }
  virtual int           fail()         {return ios::fail();   }
  virtual int           bad()          {return ios::bad();    }
  virtual int           good()         {return ios::good();   }
  virtual int           rdstate()      {return ios::rdstate();}
  virtual void          clear(int v=0) {       ios::clear(v); }

#ifndef __ATT__
  operator void*()      { return RWvistream::operator void*(); }
#endif
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
#ifdef RW_SEGMENTED_ARCHITECTURE
  virtual RWvistream&   get(char rwhuge*    , unsigned long);
#endif
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
//virtual RWvistream&   operator>>(bool& b) { return this->get(b); }
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

};

/************************************************
 *                                              *
 *              class RWbostream                *
 *                                              *
 ************************************************/

class RWExport RWbostream : public virtual ios, public RWvostream
{

public:

  RWbostream(ostream& str);
  RWbostream(streambuf* s);
  virtual ~RWbostream();

  virtual int           eof()          {return ios::eof();    }
  virtual int           fail()         {return ios::fail();   }
  virtual int           bad()          {return ios::bad();    }
  virtual int           good()         {return ios::good();   }
  virtual int           rdstate()      {return ios::rdstate();}
  virtual void          clear(int v=0) {       ios::clear(v); }

#ifndef __ATT__
  operator void*()      { return RWvostream::operator void*(); }
#endif
  virtual RWvostream&   putString(const char*, size_t);
  virtual RWvostream&   operator<<(const char*);
  virtual RWvostream&   operator<<(char);
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvostream&   operator<<(wchar_t);
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvostream&   operator<<(unsigned char);
#endif
//virtual RWvostream&   operator<<(         bool b) {return this->put(b);}
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
#ifdef RW_SEGMENTED_ARCHITECTURE
  virtual RWvostream&   put(const char rwhuge* p, unsigned long N);
#endif
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvostream&   put(wchar_t);
  virtual RWvostream&   put(const wchar_t* p, size_t N);
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvostream&   put(unsigned char);
  virtual RWvostream&   put(const unsigned char* p, size_t N);
#endif
#ifndef RW_NO_BOOL
  virtual RWvostream&   put(RWBoolean);
  virtual RWvostream&   put(const RWBoolean* p, size_t N);
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

};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWBSTREAM_H__ */

