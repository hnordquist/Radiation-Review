#ifndef __RWESTREAM_H__
#define __RWESTREAM_H__

/*
 * rwe[io]stream ---    Use Binary reads and writes while allowing for
 *                      differences in big endian vs. little endian, as
 *                      well as differences in the sizes of integral types.
 *
 * $Id: estream.h@#/main/15  09/04/97 17:24:58  griswolf (TOOLS707_WIN32_971021)
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

#include "rw/bstream.h"
/************************************************
 *                                              *
 *              class RWeostream                *
 *                                              *
 ************************************************/

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWeostream : public RWbostream
{
public:
  enum Endian { LittleEndian, BigEndian, HostEndian };

  RWeostream(ostream& str, Endian = HostEndian);
  RWeostream(streambuf* s, Endian = HostEndian);

  virtual RWvostream& operator<<(char c)   
#ifndef RW_REVERSED_CR_AND_LF
                                           { return RWbostream::operator<<(c);}
#else
  ; // define in estream.cpp
#endif 
  virtual RWvostream& put(char c)          { return RWbostream::put(c); }
  virtual RWvostream& put(const char* p, size_t N)
                                           { return RWbostream::put(p,N); }
#ifndef RW_NO_BOOL
  virtual RWvostream&   put(RWBoolean b)
                                           { return RWbostream::put(b); }
  virtual RWvostream&   put(const RWBoolean* p, size_t N)
                                           { return RWbostream::put(p,N); }
//virtual RWvostream&   operator<<(bool b){ return RWbostream::put(b); }
#endif
#ifdef RW_SEGMENTED_ARCHITECTURE
  virtual RWvostream& put(const char rwhuge* p, unsigned long N)
                                           { return RWbostream::put(p,N); }
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvostream& put(unsigned char c) { return RWbostream::put(c);   }
  virtual RWvostream& put(const unsigned char* p, size_t N)
                                           { return RWbostream::put(p,N); }
  virtual RWvostream& operator<<(unsigned char c)
#  ifndef RW_REVERSED_CR_AND_LF
                                           { return RWbostream::operator<<(c);}
#  else
  ; // define in estream.cpp
#  endif
#endif  

#ifdef RW_FIX_XSIZE_T
  virtual RWvostream&   putSizeT(size_t);
#endif

  virtual RWvostream&   operator<<(unsigned int   );
  virtual RWvostream&   operator<<(         int   );
  virtual RWvostream&   operator<<(         long  );
  virtual RWvostream&   operator<<(unsigned long  );
  virtual RWvostream&   operator<<(         short );
  virtual RWvostream&   operator<<(unsigned short );
  virtual RWvostream&   operator<<(         double);
  virtual RWvostream&   operator<<(         float );
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvostream&   operator<<(        wchar_t);
  virtual RWvostream&   put(wchar_t c)  { return operator<<(c); }
#endif

  virtual RWvostream&   operator<<(const char*);
  virtual RWvostream&   put(const          short* , size_t);
  virtual RWvostream&   put(const unsigned short* , size_t);
  virtual RWvostream&   put(const          int*   , size_t);
  virtual RWvostream&   put(const unsigned int*   , size_t);
  virtual RWvostream&   put(const          long*  , size_t);
  virtual RWvostream&   put(const unsigned long*  , size_t);
  virtual RWvostream&   put(const          float* , size_t);
  virtual RWvostream&   put(const          double*, size_t);
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvostream&   put(const          wchar_t* p, size_t N);
#endif

private:
  void          putHeader();

  Endian        streamEndian_;  
};

/************************************************
 *                                              *
 *              class RWeistream                *
 *                                              *
 ************************************************/

class RWExport RWeistream : public RWbistream
{

public:

  RWeistream(istream& str);
  RWeistream(streambuf* s);

  virtual int           get()           { return RWbistream::get(); }
  virtual RWvistream&   get(char& c)    { return RWbistream::get(c); }
  virtual RWvistream&   get(char* p, size_t N)
                                        { return RWbistream::get(p,N); }
#ifdef RW_SEGMENTED_ARCHITECTURE
  virtual RWvistream&   get(char rwhuge* p, unsigned long N)
                                        { return RWbistream::get(p,N); }
#endif
  virtual RWvistream&   operator>>(char& c)
#ifndef RW_REVERSED_CR_AND_LF                           
                                        { return RWbistream::operator>>(c); }
#else
  ; // define in estream.cpp
#endif

#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvistream&   get(unsigned char& c)
                                        { return RWbistream::get(c); }
#ifndef RW_NO_BOOL
  virtual RWvistream&   get(RWBoolean& b)
                                        { return RWbistream::get(b); }
  virtual RWvistream&   get(RWBoolean* p, size_t N)
                                        { return RWbistream::get(p,N); }
//virtual RWvistream&   operator>>(bool& b){ return RWbistream::get(b); }
#endif
  virtual RWvistream&   get(unsigned char* p, size_t N)
                                        { return RWbistream::get(p,N); }
#ifdef RW_FIX_XSIZE_T
  virtual RWvistream&   getSizeT(size_t&);
#endif

  virtual RWvistream&   operator>>(unsigned char& c)
#  ifndef RW_REVERSED_CR_AND_LF
                                   { return RWbistream::operator>>(c); }
#  else
  ; // define in estream.cpp
#  endif 
#endif
                  
  virtual RWvistream&   operator>>(int&   );
  virtual RWvistream&   operator>>(short& );
  virtual RWvistream&   operator>>(long&  );
  virtual RWvistream&   operator>>(unsigned int&  );
  virtual RWvistream&   operator>>(unsigned long& );
  virtual RWvistream&   operator>>(unsigned short&);
  virtual RWvistream&   operator>>(double&);
  virtual RWvistream&   operator>>(float& );
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvistream&   operator>>(wchar_t&);
  virtual RWvistream&   get(wchar_t& c)           { return operator>>(c); }
#endif

  virtual RWvistream&   get(         int*   , size_t);
  virtual RWvistream&   get(unsigned int*   , size_t);
  virtual RWvistream&   get(         long*  , size_t);
  virtual RWvistream&   get(unsigned long*  , size_t);
  virtual RWvistream&   get(         short* , size_t);
  virtual RWvistream&   get(unsigned short* , size_t);
  virtual RWvistream&   get(         double*, size_t);
  virtual RWvistream&   get(         float* , size_t);
  virtual RWvistream&   getString(char* s  , size_t maxlen);
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvistream&   get(wchar_t*, size_t N);
#endif

public:
// note: char types and bool are always stored in single bytes
  size_t                streamSizeofShort() { return streamSizeofShort_; }
  size_t                streamSizeofInt()   { return streamSizeofInt_; }
  size_t                streamSizeofLong()  { return streamSizeofLong_; }
  size_t                streamSizeofWchar() { return streamSizeofWchar_; }
  size_t                streamSizeofSizeT() { return streamSizeofSizeT_; }
  RWeostream::Endian    streamEndian()      { return streamEndian_; }

private:
  void                  getHeader();
  void                  headerErr();
  void                  getDiffSize(void*, size_t, size_t, RWBoolean);
  
  size_t                streamSizeofShort_;
  size_t                streamSizeofInt_;
  size_t                streamSizeofLong_;
  size_t                streamSizeofWchar_;
  size_t                streamSizeofSizeT_;     
  RWeostream::Endian    streamEndian_;
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWESTREAM_H__ */

