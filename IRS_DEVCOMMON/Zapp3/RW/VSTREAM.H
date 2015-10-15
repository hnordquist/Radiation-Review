#ifndef __RWVSTREAM_H__
#define __RWVSTREAM_H__

/*
 * rw[io]stream --- abstract base class for I/O
 *
 * $Id: vstream.h@#/main/38  09/19/97 08:50:10  griswolf (TOOLS707_WIN32_971021)
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

#include "rw/rstream.h"

/*
 * vstreams maintain a version number to help with backward compatibility 
 * issues.  See RWCString::saveOn for an example of how the version number
 * can be used internally.  Users of the library can set the version
 * number of a stream to force it to behave as in a previous incarnation.
 *
 *     Version 0: Baseline (virtually introduced with Tool600)
 *     Version 1: RWCStrings now save/restore themselves to/from virtual
 *                streams as sequence of characters, not numbers.  (Introduced
 *                with Tool602).
 */
static const unsigned current_version = 1;

/************************************************
 *                                              *
 *              class RWvios                    *
 *                                              *
 ************************************************/

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWvios
{
public:

  virtual ~RWvios();

  // Parallel the ios interface:
  virtual int           eof()          = 0;
  virtual int           fail()         = 0;
  virtual int           bad()          = 0;
  virtual int           good()         = 0;
  virtual int           rdstate()      = 0;
  virtual void          clear(int v=0) = 0;
// replace with operator bool() when bool becomes commonly available
// Causes "not implemented" message for cfront compilers in bstream.h
#ifndef __ATT__
  operator void*()      { return fail() ? 0 : this; }
#endif
};


/************************************************
 *                                              *
 *              class RWios                     *
 *                                              *
 ************************************************/

// Convenience class for virtual streams that want ios type functionality,
// but cannot use streambufs in their implementation:

class RWExport RWios : public virtual RWvios
{
public:

  virtual ~RWios();
  RWios() : status(0) {;}
  virtual int           eof()           {return status & ios::eofbit;}
  virtual int           fail()          {return status & ios::failbit;}
  virtual int           bad()           {return status & ios::badbit;}
  virtual int           good()          {return status==0;}
  virtual int           rdstate()       {return status;}
  virtual void          clear(int v=0)  {status = v;}

protected:

  int status;

};


/************************************************
 *                                              *
 *              class RWvistream                *
 *                                              *
 ************************************************/

class RWExport RWvistream : public virtual RWvios
{
public:

  virtual ~RWvistream();
  virtual int           eof()          = 0;
  virtual int           fail()         = 0;
  virtual int           bad()          = 0;
  virtual int           good()         = 0;
  virtual int           rdstate()      = 0;
  virtual void          clear(int v=0) = 0;

  virtual int           get()          = 0;     // Treat as number; EOF upon eof
  virtual RWvistream&   get(char&)     = 0;     // Treat as number
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvistream&   get(wchar_t&)  = 0;     // Treat as character
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvistream&   get(unsigned char&) = 0; // Treat as number
#endif
#ifndef RW_NO_BOOL
  virtual RWvistream&   get(RWBoolean& b) = 0;
  virtual RWvistream&   get(RWBoolean*, size_t N) = 0;
#else /* RWBoolean is an int then */
          RWvistream&   get(RWBoolean& b) { return get(&b,1); }
//virtual RWvistream&   get(RWBoolean*, size_t N) = 0; // is intvec
#endif
  virtual RWvistream&   get(char*   , size_t N) = 0;
#ifdef RW_SEGMENTED_ARCHITECTURE
  virtual RWvistream&   get(char rwhuge *, unsigned long N) = 0;
#endif
  virtual RWvistream&   get(double* , size_t N) = 0;
  virtual RWvistream&   get(float*  , size_t N) = 0;
  virtual RWvistream&   get(int*    , size_t N) = 0;
  virtual RWvistream&   get(long*   , size_t N) = 0;
  virtual RWvistream&   get(short*  , size_t N) = 0;
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvistream&   get(wchar_t*, size_t N) = 0;
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvistream&   get(unsigned char* , size_t N) = 0;
#endif
  virtual RWvistream&   get(unsigned short*, size_t N) = 0;
  virtual RWvistream&   get(unsigned int*  , size_t N) = 0;
  virtual RWvistream&   get(unsigned long* , size_t N) = 0;
  virtual RWvistream&   getString(char* s  , size_t N) = 0; // Treat as string
#ifdef RW_FIX_XSIZE_T
  virtual RWvistream&   getSizeT(size_t&) = 0;              // for estreams
#endif
  virtual RWvistream&   operator>>(char&)    = 0;           // Treat as char
//  virtual RWvistream&   operator>>(bool&  )  = 0;
  virtual RWvistream&   operator>>(double&)  = 0;
  virtual RWvistream&   operator>>(float&)   = 0;
  virtual RWvistream&   operator>>(int&)     = 0;
  virtual RWvistream&   operator>>(long&)    = 0;
  virtual RWvistream&   operator>>(short&)   = 0;
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvistream&   operator>>(wchar_t&) = 0;
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvistream&   operator>>(unsigned char&) = 0;
#endif
  virtual RWvistream&   operator>>(unsigned short&) = 0;
  virtual RWvistream&   operator>>(unsigned int&)   = 0;
  virtual RWvistream&   operator>>(unsigned long&)  = 0;

// For backwards compatibility
private:
  unsigned version_;
public:
  void version(unsigned v) { version_ = v; }
  unsigned version() { return version_; }
// constructor
  RWvistream() : version_(current_version) { }
};


/************************************************
 *                                              *
 *              class RWvostream                *
 *                                              *
 ************************************************/

class RWExport RWvostream : public virtual RWvios
{
public:
  virtual ~RWvostream();
  virtual int           eof()          = 0;
  virtual int           fail()         = 0;
  virtual int           bad()          = 0;
  virtual int           good()         = 0;
  virtual int           rdstate()      = 0;
  virtual void          clear(int v=0) = 0;

  virtual RWvostream&   putString(const char*, size_t) = 0;// Treat as string
  virtual RWvostream&   operator<<(const char*) = 0;       // Treat as char
  virtual RWvostream&   operator<<(char) = 0;              // Treat as char
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvostream&   operator<<(wchar_t) = 0;
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvostream&   operator<<(unsigned char)  = 0;
#endif
//  virtual RWvostream&   operator<<(bool)           = 0;
  virtual RWvostream&   operator<<(double)         = 0;
  virtual RWvostream&   operator<<(float)          = 0;
  virtual RWvostream&   operator<<(int)            = 0;
  virtual RWvostream&   operator<<(unsigned int)   = 0;
  virtual RWvostream&   operator<<(long)           = 0;
  virtual RWvostream&   operator<<(unsigned long)  = 0;
  virtual RWvostream&   operator<<(short)          = 0;
  virtual RWvostream&   operator<<(unsigned short) = 0;
  virtual RWvostream&   flush()                    = 0;

  virtual RWvostream&   put(char) = 0;                     // Treat as number
#ifndef RW_NO_BOOL
  virtual RWvostream&   put(RWBoolean) = 0;
  virtual RWvostream&   put(const RWBoolean* p, size_t N) = 0;
#else  /* RWBoolean is an int then */
          RWvostream&   put(RWBoolean b) { return put(&b,1); }
//virtual RWvostream&   put(const RWBoolean* p, size_t N) = 0; // is intvec
#endif
  virtual RWvostream&   put(const char* p, size_t N) = 0;  // Treat as numbers
#ifdef RW_SEGMENTED_ARCHITECTURE
  virtual RWvostream&   put(const char rwhuge * p, unsigned long N) = 0;
#endif
#ifndef RW_NO_OVERLOAD_WCHAR
  virtual RWvostream&   put(wchar_t) = 0;
  virtual RWvostream&   put(const wchar_t* p, size_t N) = 0;
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  virtual RWvostream&   put(unsigned char) = 0;
  virtual RWvostream&   put(const unsigned char*  , size_t N) = 0;
#endif
  virtual RWvostream&   put(const short*          , size_t N) = 0;
  virtual RWvostream&   put(const unsigned short* , size_t N) = 0;
  virtual RWvostream&   put(const int*            , size_t N) = 0;
  virtual RWvostream&   put(const unsigned int*   , size_t N) = 0;
  virtual RWvostream&   put(const long*           , size_t N) = 0;
  virtual RWvostream&   put(const unsigned long*  , size_t N) = 0;
  virtual RWvostream&   put(const float*          , size_t N) = 0;
  virtual RWvostream&   put(const double*         , size_t N) = 0;
#ifdef RW_FIX_XSIZE_T
  virtual RWvostream&   putSizeT(size_t) = 0;             // for estreams
#endif

// For backwards compatibility
private:
  unsigned version_;
public:
  void version(unsigned v) { version_ = v; }
  unsigned version() { return version_; }
// constructor
  RWvostream() : version_(current_version) { }
};

// If the following function were a member function of RWvostream, rather
// than a global function, classes derived from RWvostream would not
// be able to redefine other oper<< functions, without hiding this one, or
// redefining this one also.
inline RWvostream&  operator<<(RWvostream& v, RWvostream& (*f)(RWvostream&)) {
  return (*f)(v);
}

#ifdef RW_TRAILING_RWEXPORT
RWvostream&  rwexport flush(RWvostream&);
#else
rwexport RWvostream&  flush(RWvostream&);
#endif

// The following are used for simple External Template Persistence (ETP)
// for builtin types.
// They are not virtual functions in RW{p,b,e}{i,o}stream in order to
// maintain link compatibility with Tools 6.1.

RWEXPORT_RETURN(RWvistream&) operator>>(RWvistream& s, char*& ip);
#ifndef RW_NO_OVERLOAD_WCHAR
  RWEXPORT_RETURN(RWvistream&) operator>>(RWvistream& s, wchar_t*& ip);
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  RWEXPORT_RETURN(RWvistream&) operator>>(RWvistream& s, unsigned char*& ip);
#endif

RWEXPORT_RETURN(RWvistream&) operator>>(RWvistream& s, double*& ip);
RWEXPORT_RETURN(RWvistream&) operator>>(RWvistream& s, float*& ip);
RWEXPORT_RETURN(RWvistream&) operator>>(RWvistream& s, int*& ip);
RWEXPORT_RETURN(RWvistream&) operator>>(RWvistream& s, long*& ip);
RWEXPORT_RETURN(RWvistream&) operator>>(RWvistream& s, short*& ip);
RWEXPORT_RETURN(RWvistream&) operator>>(RWvistream& s, unsigned int*& ip);
RWEXPORT_RETURN(RWvistream&) operator>>(RWvistream& s, unsigned long*& ip);
RWEXPORT_RETURN(RWvistream&) operator>>(RWvistream& s, unsigned short*& ip);

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWVSTREAM_H__ */
