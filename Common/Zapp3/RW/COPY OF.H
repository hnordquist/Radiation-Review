#ifndef __RWTIME_H__
#define __RWTIME_H__

/*
 * Declarations for class RWTime.
 *
 * $Id: rwtime.h@#/main/58  09/12/97 13:43:33  kevinj (TOOLS707_WIN32_971021)
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
 *
 */

#include"rw/compiler.h"

#ifndef RW_NO_STL
// #2437 Bring in global definitions of  <= >= != if they are available 
#  ifndef RW_HEADER_WITH_EXTENSION
#    include<utility>
#  else
#    include<utility.hpp>
#  endif /* RW_HEADER_WITH_EXTENSION */
#endif /* RW_NO_STL */

#include "rw/locale.h"
STARTWRAP
#include <time.h>                /* System time management. */
ENDWRAP

#if defined(__IBMCPP__) && !defined(_AIX)
# undef min
# undef max
#endif 

class RWExport RWDate;

/*
 *  Conversion from Unix to RW time (number of seconds
 *  between 1/1/1901 and 1/1/1970):
 */
const RW4Byte rwEpoch    = 2177452800UL;  
const RW4Byte rwEpoch_68 = 2114294400UL;  // non-std: 1/1/01 --> 1/1/68

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWTime
{

public:

  RWTime() : sec(RWTime::currentTime()) {;}     // current time
  RWTime(RW4Byte s) : sec(s) {;}  // Seconds since 00:00:00 Jan 1, 1901
  RWTime(unsigned h, unsigned m, unsigned s=0,  // today
         const RWZone& = RWZone::local());
  RWTime(const RWDate&, unsigned h=0, unsigned m=0, unsigned s=0,
         const RWZone& = RWZone::local());
  RWTime(const RWDate&, const RWCString& str,
         const RWZone& = RWZone::local(),
         const RWLocale& = RWLocale::global());

  // This constructor depends on the following members of the struct tm arg:
  //    tm_year, tm_mon, tm_mday, tm_hour, tm_min, and tm_sec.
  RWTime(const struct tm*, const RWZone& = RWZone::local());

  // The following two should not be necessary, but are required to
  // support feeble compilers:
  RWTime(const RWTime& t) : sec(t.sec) {;}
  RWTime& operator=(const RWTime& t) {sec = t.sec; return *this;}

  // this function sets all members of its struct tm argument:
  void          extract(struct tm*, const RWZone& = RWZone::local()) const;
  void          extractGMT(struct tm*) const;

  RWCString     asString(char format = '\0',
                         const RWZone& = RWZone::local(),
                         const RWLocale& = RWLocale::global()) const;
  RWCString     asString(const char *format,
                         const RWZone& = RWZone::local(),
                         const RWLocale& = RWLocale::global()) const;
  RWBoolean     between(const RWTime& a, const RWTime& b) const
#                  if defined(RW_NO_STL)
                        { return *this >= a && *this <= b;}
#                  elif defined(RW_GLBL_SCP_BUG4)
                        { return (!(*this < a) && (*this < b || *this == b));}
#                  else
                        { return *this >= a && *this <= b;}
#                  endif
  RWspace       binaryStoreSize() const {return sizeof(RW4Byte);}
  int           compareTo(const RWTime*) const;
  unsigned      hash()    const;
  unsigned      hour(const RWZone& = RWZone::local())  const;
  unsigned      hourGMT() const;        // hour: UTC
  RWBoolean     isDST(const RWZone& = RWZone::local()) const;
  RWBoolean     isValid() const { return sec != 0; }
  RWTime        max(const RWTime& t) const;
  RWTime        min(const RWTime& t) const;
  unsigned      minute(const RWZone& = RWZone::local()) const;
  unsigned      minuteGMT()     const;  // minute: UTC
  void          restoreFrom(RWFile&);
  void          restoreFrom(RWvistream&);
  void          saveOn(RWFile&) const;
  void          saveOn(RWvostream&) const;
  unsigned      second() const;         // second: local time or UTC 
  RW4Byte seconds() const {return sec;}
#ifndef RW_TRAILING_RWEXPORT
  friend rwexport ostream& operator<<(ostream&, const RWTime&);
#else
  friend ostream& rwexport operator<<(ostream&, const RWTime&);
#endif

  RWTime        operator++()            { return RWTime(++sec); }
  RWTime        operator--()            { return RWTime(--sec); }
#ifndef RW_NO_POSTFIX
  RWTime        operator++(int)         { return RWTime(sec++); }
  RWTime        operator--(int)         { return RWTime(sec--); }
#endif

  RWTime&       operator+=(RW4Byte s)     { sec += s; return *this; }
  RWTime&       operator-=(RW4Byte s)     { sec -= s; return *this; }

  // Static member functions:
  static RWTime beginDST(unsigned year, const RWZone& = RWZone::local());
  static RWTime endDST  (unsigned year, const RWZone& = RWZone::local());
  static RWTime now();          // Return present time

  // useful for supplying hash functions to template hash collection ctors:  
  static unsigned hash(const RWTime&);

  // Logical operators.
#if !defined(__SUNPRO_CC) || ( __SUNPRO_CC<0x420 && !defined(RW_NO_STL)) 
  RW_INLINE_FRIEND RWBoolean    operator< (const RWTime& t1, const RWTime& t2);
  RW_INLINE_FRIEND RWBoolean    operator==(const RWTime& t1, const RWTime& t2);
#else
   friend RWBoolean    operator< (const RWTime& t1, const RWTime& t2)
    { return t1.sec<t2.sec;}
   friend RWBoolean    operator==(const RWTime& t1, const RWTime& t2)
    {return t1.sec==t2.sec;}
#endif
#if defined(RW_NO_STL) || defined(__PowerCpp__) || defined(RW_RWV2X_STDLIB)
  RW_INLINE_FRIEND RWBoolean    operator<=(const RWTime& t1, const RWTime& t2);
  RW_INLINE_FRIEND RWBoolean    operator> (const RWTime& t1, const RWTime& t2);
  RW_INLINE_FRIEND RWBoolean    operator>=(const RWTime& t1, const RWTime& t2);
  RW_INLINE_FRIEND RWBoolean    operator!=(const RWTime& t1, const RWTime& t2);
#endif

  // Add or subtract seconds.
#ifndef RWTOOLS610_COMPATIBLE
  RW_INLINE_FRIEND RWTime       operator+(const RWTime& t, RW4Byte s);
  RW_INLINE_FRIEND RWTime       operator-(const RWTime& t, RW4Byte s);
  RW_INLINE_FRIEND RWTime       operator+(RW4Byte s, const RWTime& t);
#else
  RW_INLINE_FRIEND RWTime       operator+(const RWTime& t,  RW4ByteSigned s );
  RW_INLINE_FRIEND RWTime       operator-(const RWTime& t,  RW4ByteSigned s );
  RW_INLINE_FRIEND RWTime       operator+( RW4ByteSigned s , const RWTime& t);
#endif


protected:

  static RWBoolean      assertDate(const RWDate&);

private:

  static  RW4Byte buildFrom(const RWDate&, unsigned h, unsigned m, 
                                  unsigned s, const RWZone& = RWZone::local());
  static  RW4Byte currentTime();

   RW4Byte         sec;            // Seconds since 1/1/1901.

  RW_TYPESPEC  /* This macro usually expands to nothing */

};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#if !defined(__SUNPRO_CC) || (__SUNPRO_CC<0x420 && !defined(RW_NO_STL)) 
inline RWBoolean        operator==(const RWTime& t1, const RWTime& t2)
                { return t1.sec == t2.sec; }
inline RWBoolean        operator< (const RWTime& t1, const RWTime& t2)
                { return t1.sec <  t2.sec; }
#endif /* !SunPro 420 */

#if defined(RW_NO_STL) || defined(__PowerCpp__) || defined(RW_RWV2X_STDLIB)

inline RWBoolean        operator<=(const RWTime& t1, const RWTime& t2)
                { return t1.sec <= t2.sec; }
inline RWBoolean        operator> (const RWTime& t1, const RWTime& t2)
                { return t1.sec >  t2.sec; }
inline RWBoolean        operator>=(const RWTime& t1, const RWTime& t2)
                { return t1.sec >= t2.sec; }
inline RWBoolean        operator!=(const RWTime& t1, const RWTime& t2)
                { return t1.sec != t2.sec; }
#endif


#ifndef RWTOOLS610_COMPATIBLE
  inline RWTime         operator+(const RWTime& t, RW4Byte s)
                { return RWTime(t.sec+s); }
  inline RWTime         operator-(const RWTime& t, RW4Byte s)
                { return RWTime(t.sec-s); }
  inline RWTime         operator+(RW4Byte s, const RWTime& t)
                { return RWTime(t.sec+s); }
#else
  inline RWTime         operator+(const RWTime& t, RW4ByteSigned s)
                { return RWTime(t.sec+s); }
  inline RWTime         operator-(const RWTime& t, RW4ByteSigned s)
                { return RWTime(t.sec-s); }
  inline RWTime         operator+(RW4ByteSigned s, const RWTime& t)
                { return RWTime(t.sec+s); }
#endif

inline RWvistream&      operator>>(RWvistream& str,       RWTime& t)
                { t.restoreFrom(str);  return str;  }
inline RWFile&          operator>>(RWFile& file,          RWTime& t)
                { t.restoreFrom(file); return file; }
inline RWvistream&      operator>>(RWvistream& str,       RWTime*& t)
                { t = new RWTime; t->restoreFrom(str);  return str;  }
inline RWFile&          operator>>(RWFile& file,          RWTime*& t)
                { t = new RWTime; t->restoreFrom(file); return file; }
inline RWvostream&      operator<<(RWvostream& str, const RWTime& t)
                { t.saveOn(str);       return str;  }
inline RWFile&          operator<<(RWFile& file,    const RWTime& t)
                { t.saveOn(file);      return file; }

inline RWTime::RWTime(const RWDate& date, unsigned h, unsigned m, unsigned s,
                      const RWZone& zone)
    { sec = buildFrom(date, h, m, s, zone); }

#ifdef __OS2__
# include<stdlib.h>   //Bring back definitions of min & max macros
#endif

#endif  /* __RWTIME_H__ */
