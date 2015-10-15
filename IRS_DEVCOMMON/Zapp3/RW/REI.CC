#ifndef __RWREI_CC__
#define __RWREI_CC__ 1

/*
 * RWTRegularExpressionImp definitions for RE Implementation class.
 *
 * $Id: rei.cc@#/main/28  03/14/96 22:18:39  pearson (TOOLS707_WIN32_971021)
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
 * $Log: rei.cc,v $
 * Revision 7.28  1996/03/14 22:18:39  pearson
 * #2828 - Fix warning message on Borland 4.52
 *
 * Revision 7.27  1996/02/18 01:45:45  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.26  1995/10/31 00:27:06  jims
 * include re.h instead of rei.h
 *
 * Revision 7.25  1995/10/24  00:00:16  jims
 * Port to msvc 4.0
 *
 * Revision 7.24  1995/09/22  20:14:06  kevinj
 * Clean up loose ends.
 *
 * Revision 7.23  1995/09/22  16:19:52  jims
 * Use string length when copying searchString for fast re
 *
 * Revision 7.22  1995/09/19  05:24:10  jims
 * Remove commented #undef and surrounding comments
 *
 * Revision 7.21  1995/09/08  20:07:07  pearson
 * Commented out an undef
 *
 * Revision 7.20  1995/09/08  19:12:30  pearson
 * Changed RW_NO_TEMPLATE_TYPEDEF_IN_OUT_OF_LINE_ARG_LIST to 
 * RW_BROKEN_TEMPLATE_TYPEDEFS
 *
 * Revision 7.19  1995/09/05  19:20:34  jims
 * Use new copyright macro
 *
 * Revision 7.18  1995/08/30  00:19:35  kevinj
 * Added RW_NO_EXCEPTIONS.
 *
 * Revision 7.17  1995/07/28  22:23:23  kevinj
 * Added #include "rw/rei.h".
 *
 * Revision 7.16  1995/06/27  23:40:49  kevinj
 * #1876: Added RW_NO_STL to guard against futile compilation of classes
 * that depend on the existence of C++ Standard Library containers
 * and iterators.
 *
 * Revision 7.15  1995/02/21  17:01:08  kevinj
 * Bug #1053: Fixed "\\xq", "b?b" and "[]" bugs.
 *
 * Revision 7.14  1995/01/26  17:39:40  kevinj
 * Bug #1038: Infinite loop on some pattern matching.
 *
 * Revision 7.13  1995/01/25  00:24:03  kevinj
 * Bug #1037: Removed all escape sequence processing. Escape sequence
 * processing will now be done either by the C++ compiler (in the case of
 * literal strings) or by the user (in the case of command line arguments).
 * This was judged to be less troublesome than the previous approach, used
 * by RWCRegexp, that required users to enter RE patterns like "\\n" in order
 * to match newlines.
 *
 * Revision 7.12  1995/01/21  01:20:02  kevinj
 * Completed port to Solaris Cafe 3.0.
 *
 * Revision 7.11  1995/01/18  00:19:05  kevinj
 * rwRegexec speed optimization.
 *
 * Revision 7.10  1995/01/16  21:21:56  kevinj
 * Optimized for speed with RWCRegexp.
 *
 * Revision 7.9  1995/01/10  21:12:06  kevinj
 * Ported to Solaris Cafe.
 *
 * Revision 7.8  1994/12/30  23:56:44  kevinj
 * Added RW_NO_POSIX_RE.  Made classes MT safe.  Commented and cleaned up code.
 * Combined NFA arrays in regex_t into one array called "node".
 * Removed RWREMatch.
 */

#include "rw/compiler.h"
#if !defined(RW_NO_STL) && !defined(RW_NO_EXCEPTIONS)

#include "rw/re.h"

#if !defined(RW_COMPILE_INSTANTIATE)
# include "rw/reposix1.cc"
# include "rw/reposix2.cc"
#endif

/////////////////////////////////////////////////////////////////////////////
//
// Constructors and Destructors
//
/////////////////////////////////////////////////////////////////////////////

template<class charT>
RWTRegularExpressionImp<charT>::RWTRegularExpressionImp()
        : compiled_(FALSE),
#   if defined(RW_NO_POSIX_RE)
      fastRePtr_(NULL), symbolSetVector_(4),
#   endif
    rePtr_(new regex_t),
    maxState_(~(size_t)1), getNextSymbol_(~(size_t)0), nextState_(0),
    nonacceptingState_(0)
{
  clear();
}

template<class charT>
RWTRegularExpressionImp<charT>::RWTRegularExpressionImp
  (const charT* reStringPtr)
        : compiled_(FALSE),
#   if defined(RW_NO_POSIX_RE)
      fastRePtr_(NULL), symbolSetVector_(4),
#   endif
    rePtr_(new regex_t),
    maxState_(~(size_t)1), getNextSymbol_(~(size_t)0), nextState_(0),
    nonacceptingState_(0), reString_(reStringPtr)
{
  compile();
}

#if defined(RW_BROKEN_TEMPLATE_TYPEDEFS)
template<class charT>
RWTRegularExpressionImp<charT>::RWTRegularExpressionImp
  (const RWTRegularExpressionTraits<charT>::tStringT& reString)
#else
template<class charT>
RWTRegularExpressionImp<charT>::RWTRegularExpressionImp
  (const stringT& reString)
#endif

        : compiled_(FALSE),
#   if defined(RW_NO_POSIX_RE)
      fastRePtr_(NULL), symbolSetVector_(4),
#   endif
    rePtr_(new regex_t),
    maxState_(~(size_t)1), getNextSymbol_(~(size_t)0), nextState_(0),
    nonacceptingState_(0), reString_(reString)
{
  compile();
}

template<class charT>
RWTRegularExpressionImp<charT>::RWTRegularExpressionImp
(const RWTRegularExpressionImp<charT>&   other)
        : compiled_(FALSE),
#   if defined(RW_NO_POSIX_RE)
      fastRePtr_(NULL), symbolSetVector_(4),
#   endif
    rePtr_(new regex_t),
    maxState_(~(size_t)1), getNextSymbol_(~(size_t)0), reString_(other.reString_)
{
  compile();
}

template<class charT>
RWTRegularExpressionImp<charT>::~RWTRegularExpressionImp()
{
  delete fastRePtr_;
  fastRePtr_ = NULL;
  regfree(rePtr_);
  rePtr_ = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
// operator=
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> RWTRegularExpressionImp<charT>&
RWTRegularExpressionImp<charT>::operator=
  (const RWTRegularExpressionImp<charT>& other)
{
  reString_ = other.reString_;
  compile();
  return *this;
}

template<class charT> RWTRegularExpressionImp<charT>&
RWTRegularExpressionImp<charT>::operator=(const charT* reStringCharArray)
{
  reString_ = reStringCharArray;
  compile();
  return *this;
}

/////////////////////////////////////////////////////////////////////////////
//
// clear
//
//  Clears the entire nondeterministic finite automaton (NFA).
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> void
RWTRegularExpressionImp<charT>::clear()
{
  compiled_ = FALSE;
  rePtr_->re_nsub = 0;

# if defined(RW_NO_POSIX_RE)
  fastRePtr_ = NULL;
        nextState_ = 0;
  nonacceptingState_ = 0;
  symbolSetVector_.clear();
  rePtr_->subexpression.clear();
  rePtr_->node.clear();
# endif
}

/////////////////////////////////////////////////////////////////////////////
//
//  compile
//
//  Compiles regular expressions using regcomp().
//  Preprocesses RE's to replace escape sequences with literal characters.
//
/////////////////////////////////////////////////////////////////////////////

#include <ctype.h>

template<class charT> void
RWTRegularExpressionImp<charT>::compile()
{
  clear();

#if defined(RW_NO_POSIX_RE)
  // If regular expression can be handled by old "fast" RE class RWCRegexp,
  //  then don't use POSIX functions.

  delete fastRePtr_;
  compiled_ = FALSE;
  RWBoolean charTrue = RWTRegularExpressionTraits<charT>::isChar();

  if (charTrue)
    // RWTRegularExpression<char>, a.k.a. RWCRExpr
  {
    const char*   cp = (const char*)reString_.data();
    const size_t  len(reString_.length());
    const char*   endCp = cp + len;              // Past end of string data.
    size_t        i = 0;                         // for() index.

    if (*cp != star() && *cp != plus() && *cp != questionMark())  // BAD_REPEAT
    {
      // Scan for symbols not in or treated differently in RWCRegexp;
      //  if found, then continue.
      for (; i < len; i++)
      {
        if    // Symbols treated differently in RWCRegexp.
        (
          *cp == openBrace()      || *cp == closeBrace()      ||
          *cp == openParen()      || *cp == closeParen()      ||
          *cp == hat()            || *cp == dollar()          ||
          *cp == bar()            || *cp == backslash()       ||
          (
            *cp == openBracket()  &&  cp < endCp              &&
            (*(cp + 1) == colon() || *(cp + 1) == dot() || *(cp + 1) == equal())
          )                                                   ||
          (*cp && !isprint(*cp))
        )
          break;

        if (*cp == dash() && cp < endCp && *(cp+1) == dash())
          break;  // BAD_CHAR_RANGE syntax error.

        if    // BAD_REPEAT syntax error.
        (
          (*cp == star() || *cp == plus() || *cp == questionMark()) &&
          cp < endCp                                                &&
          (
            *(cp + 1) == star() || *(cp + 1) == plus()              ||
            *(cp + 1) == questionMark()
          )
        )
          break;

        cp++;
      } // for
    } // if
    
    if (i >= len)     // No extended RE symbols found; use RWCRExpr.
    {
      compiled_ = TRUE;
      fastRePtr_ = new RWCRegexp((const char*)reString_.data());

      // Warning: Only call fastRePtr_->status() once because
      // RWCRegexp::status() resets status to OK.

      if (fastRePtr_->status() == RWCRegexp::OK)
                  status_ = RWTRegularExpression<charT>::OK;
      else
        status_ = RWTRegularExpression<charT>::BAD_PATTERN;

      return;
    }
  }

#endif  // NO_RE_POSIX

  int error  = RW_REGCOMP(rePtr_, (const charT*)reString_, REG_EXTENDED);

  if (!error)
  {
    status_ = RWTRegularExpression<charT>::OK;
    compiled_ = TRUE;
  }
  else  // Convert POSIX.2 error code to RWTRegularExpression<charT> error code.
  {
    clear();

    if (error == REG_ECOLLATE)
      status_ = RWTRegularExpression<charT>::BAD_COLLATING_ELEMENT;
    else if (error == REG_ECOLLATE)
      status_ = RWTRegularExpression<charT>::BAD_COLLATING_ELEMENT;
    else if (error == REG_ECTYPE)
      status_ = RWTRegularExpression<charT>::BAD_CHAR_CLASS_TYPE;
    else if (error == REG_EESCAPE)
      status_ = RWTRegularExpression<charT>::TRAILING_BACKSLASH;
    else if (error == REG_EBRACK)
      status_ = RWTRegularExpression<charT>::UNMATCHED_BRACKET;
    else if (error == REG_EPAREN)
      status_ = RWTRegularExpression<charT>::UNMATCHED_PARENTHESIS;
    else if (error == REG_EBRACE)
      status_ = RWTRegularExpression<charT>::UNMATCHED_BRACE;
    else if (error == REG_BADBR)
      status_ = RWTRegularExpression<charT>::BAD_BRACE;
    else if (error == REG_ERANGE)
      status_ = RWTRegularExpression<charT>::BAD_CHAR_RANGE;
    else if (error == REG_ESPACE)
      status_ = RWTRegularExpression<charT>::OUT_OF_MEMORY;
    else if (error == REG_BADRPT)
      status_ = RWTRegularExpression<charT>::BAD_REPEAT;
    else if (error == RWTRegularExpression<charT>::NOT_SUPPORTED)
      status_ = RWTRegularExpression<charT>::NOT_SUPPORTED;
    else
      status_ = RWTRegularExpression<charT>::BAD_PATTERN;
  }
}

/////////////////////////////////////////////////////////////////////////////
//
// index
//
//  Returns the index of the leftmost longest instance in searchString that
//  matches the regular expression compiled in self or RW_NPOS if there is no
//  such match.
//
/////////////////////////////////////////////////////////////////////////////

  template<class charT> size_t
  RWTRegularExpressionImp<charT>::index
  (
#if defined(RW_BROKEN_TEMPLATE_TYPEDEFS)
    const RWTRegularExpressionTraits<charT>::tStringT&
                    searchString, // String to be searched.
#else
    const stringT&  searchString, // String to be searched.
#endif
    size_t*         len,          // Pointer to length of the matching string.
    size_t          start         // Start of string matched.
  )
{

#if defined(RW_NO_POSIX_RE)
  // Use "fast" RE class if allowed.
  if (fastRePtr_)
  {
    size_t      tLen;
    size_t      result(fastRePtr_->index(searchString, &tLen, start));
    
    if (len)
      *len = tLen;
    
    return result;
  }
#endif

  const charT*  searchStringPtr = searchString.data() + start;
  size_t        searchStringLength(searchString.length() - start);
  regmatch_t    match[MAX_NO_OF_SUBEXPRESSIONS];
  errorType     error;

  for (int i(0); i < MAX_NO_OF_SUBEXPRESSIONS; i++)
  {
    match[i].rm_so = (regoff_t)-1;
    match[i].rm_eo = (regoff_t)-1;
  }

  error =
    (errorType)RW_REGEXEC
      (
       rePtr_, searchStringPtr, searchStringLength,
       MAX_NO_OF_SUBEXPRESSIONS, match, 0x0
       );
  
  if (error)
  {
    if (error == REG_NOMATCH)
      status_ = RWTRegularExpression<charT>::NO_MATCH;
    else if (error == RWTRegularExpression<charT>::NOT_SUPPORTED)
      status_ = RWTRegularExpression<charT>::NOT_SUPPORTED;
    else
      status_ = RWTRegularExpression<charT>::BAD_PATTERN;
    
    if (len)
      *len = RW_NPOS;
    
    return RW_NPOS;
  }
  else if (len)
    *len = match[0].rm_eo - match[0].rm_so;
  
  return match[0].rm_so + start;
}

#endif // !defined(RW_NO_STL)

#endif // __RWREI_CC__
