#ifndef __RWPSTREAM_H__
#define __RWPSTREAM_H__

/*
 * rwp[io]stream ---    Portable I/O streams (use escape sequences for 
 *                      writing special characters.
 *
 * $Id: pstream.h@#/main/28  09/04/97 17:25:26  griswolf (TOOLS707_WIN32_971021)
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
 *              class RWpistream                *
 *                                              *
 ************************************************/

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWpistream : public RWvistream
{

public:

  RWpistream(istream& str);
  RWpistream(streambuf* s);
  virtual ~RWpistream();

  virtual int           eof()          {return istr_.eof();    }
  virtual int           fail()         {return istr_.fail();   }
  virtual int           bad()          {return istr_.bad();    }
  virtual int           good()         {return istr_.good();   }
  virtual int           rdstate()      {return istr_.rdstate();}
  virtual void          clear(int v=0) {       istr_.clear(v); }
  streambuf*            rdbuf()        {return istr_.rdbuf();  }

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
  virtual RWvistream&   get(char*,   size_t N);
#ifdef RW_SEGMENTED_ARCHITECTURE
  virtual RWvistream&   get(char rwhuge*, unsigned long N);
#endif
  virtual RWvistream&   get(double*, size_t N);
  virtual RWvistream&   get(float*,  size_t N);
  virtual RWvistream&   get(int*,    size_t N);
  virtual RWvistream&   get(long*,   size_t N);
  virtual RWvistream&   get(short*,  size_t N);
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvistream&   get(wchar_t*,size_t N);
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvistream&   get(unsigned char*,  size_t N);
#endif
  virtual RWvistream&   get(unsigned short*, size_t N);
  virtual RWvistream&   get(unsigned int*,   size_t N);
  virtual RWvistream&   get(unsigned long*,  size_t N);
  virtual RWvistream&   getString(char* s,   size_t maxlen);
#ifdef RW_FIX_XSIZE_T
  virtual RWvistream&   getSizeT(size_t&);
#endif
  virtual RWvistream&   operator>>(char&);
//virtual RWvistream&   operator>>(bool& b) { return this->get(b); }
  virtual RWvistream&   operator>>(double&);
  virtual RWvistream&   operator>>(float&);
  virtual RWvistream&   operator>>(int&);
  virtual RWvistream&   operator>>(long&);
  virtual RWvistream&   operator>>(short&);

#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvistream&   operator>>(wchar_t&);
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvistream&   operator>>(unsigned char&);
#endif
  virtual RWvistream&   operator>>(unsigned int&);
  virtual RWvistream&   operator>>(unsigned long&);
  virtual RWvistream&   operator>>(unsigned short&);
protected:
  void                  syntaxErr(const char* expect, char was);
  char                  getCChar();
  istream               istr_;
private:
  // Disallow copying:
  RWpistream(const RWpistream&);
  RWpistream& operator=(const RWpistream&);
};

/************************************************
 *                                              *
 *              class RWpostream                *
 *                                              *
 ************************************************/

class RWExport RWpostream : public RWvostream
{

public:

  RWpostream(ostream& str);
  RWpostream(streambuf* s);
  virtual ~RWpostream();

  virtual int           eof()          {return ostr_.eof();    }
  virtual int           fail()         {return ostr_.fail();   }
  virtual int           bad()          {return ostr_.bad();    }
  virtual int           good()         {return ostr_.good();   }
  virtual int           rdstate()      {return ostr_.rdstate();}
  virtual void          clear(int v=0) {       ostr_.clear(v); }
  streambuf*            rdbuf()        {return ostr_.rdbuf();  }
  
  int                   precision(int p)  { return ostr_.precision(p); }
// Some platforms do not have ostream::precision() as const fn.
#ifdef RW_NO_OSTR_REF_CAST
  int                   precision() const { return ostr_.precision(); }
#else
  int                   precision() const
        { return ((ostream&)ostr_).precision(); }
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
//virtual RWvostream&   operator<<(bool b) {return this->put(b);}
  virtual RWvostream&   operator<<(double);
  virtual RWvostream&   operator<<(float);
  virtual RWvostream&   operator<<(int);
  virtual RWvostream&   operator<<(unsigned int);
  virtual RWvostream&   operator<<(long);
  virtual RWvostream&   operator<<(unsigned long);
  virtual RWvostream&   operator<<(short);
  virtual RWvostream&   operator<<(unsigned short);
  virtual RWvostream&   flush();


  virtual RWvostream&   put(char);
#ifndef RW_NO_BOOL
  virtual RWvostream&   put(RWBoolean);
  virtual RWvostream&   put(const RWBoolean* p, size_t N);
#endif
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
  virtual RWvostream&   put(const unsigned char* p,  size_t N);
#endif
  virtual RWvostream&   put(const short* p,          size_t N);
  virtual RWvostream&   put(const unsigned short* p, size_t N);
  virtual RWvostream&   put(const int* p,            size_t N);
  virtual RWvostream&   put(const unsigned int* p,   size_t N);
  virtual RWvostream&   put(const long* p,           size_t N);
  virtual RWvostream&   put(const unsigned long* p,  size_t N);
  virtual RWvostream&   put(const float* p,          size_t N);
  virtual RWvostream&   put(const double* p,         size_t N);
#ifdef RW_FIX_XSIZE_T
  virtual RWvostream&   putSizeT(size_t);
#endif
protected:
  static const unsigned MAXCOL;
  void                  putwrap(char);
#ifndef RW_NO_OVERLOAD_WCHAR
  void                  putwrap(wchar_t c);
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  void                  putwrap(unsigned char c);
#endif
  void                  putwrap(const char* s, size_t len);
  void                  putCChars(const char* s, size_t len);
  ostream               ostr_;
  unsigned              column_;
private:
  // Disallow copying:
  RWpostream(const RWpostream&);
  RWpostream& operator=(const RWpostream&);
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWPSTREAM_H__ */
