#ifndef __RWREBASE_H__
#define __RWREBASE_H__

/*
 * Declaration for class RWREBaseClass
 *
 * $Id: rebase.h@#/main/2  02/18/96 01:45:35  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: rebase.h,v $
 * Revision 7.3  1996/02/18 01:45:35  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.2  1995/10/24 06:35:19  jims
 * Add guard against multiple inclusion
 *
 * Revision 7.1  1995/10/24  06:27:36  jims
 * Move to revision 7.1
 */

/////////////////////////////////////////////////////////////////////////////
//
// RWREBaseClass
//
//  Stores enums shared by both RWTRegularExpression
//  and RWTRegularExpresssionImp.
//
//  This is inherited by both RWTRegularExpression and RWTRegularExpressionImp
//  because RWTRegularExpressionImp uses some enums that are declared in
//  RWTRegularExpression but RWTRegularExpression has a member of type
//  RWTRegularExpressionImp. This can lead to a "chicken and egg" situation
//  that can confuse some compilers; hence this class.
//
/////////////////////////////////////////////////////////////////////////////

class RWREBaseClass
{
public:

  enum statusType {
    OK=0,
    NOT_SUPPORTED = 0x100,
    NO_MATCH, BAD_PATTERN, BAD_COLLATING_ELEMENT, BAD_CHAR_CLASS_TYPE,
    TRAILING_BACKSLASH, UNMATCHED_BRACKET, UNMATCHED_PARENTHESIS,
    UNMATCHED_BRACE, BAD_BRACE, BAD_CHAR_RANGE, OUT_OF_MEMORY, BAD_REPEAT
  };

  enum {MAX_NO_OF_SUBEXPRESSIONS = 1}; // Includes subexpr 0, the whole string
};

#endif  /* __RWREBASE_H__ */
