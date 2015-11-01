#ifndef __RWWTOKEN_H__
#define __RWWTOKEN_H__

/*
 * RWTokenizer --- converts strings into sequences of tokens
 *
 * $Id: wtoken.h@#/main/9  07/18/96 04:06:00  hart (TOOLS707_WIN32_971021)
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
 * $Log: wtoken.h,v $
 * Revision 7.6  1996/07/18 04:06:00  hart
 *  ObjectStore changes
 *
 * Revision 7.5  1996/05/07 18:17:37  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.4  1996/02/18 01:50:42  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.3  1995/09/05 21:35:20  jims
 * Use new copyright macro
 *
 * Revision 7.2  1995/07/10  18:19:38  griswolf
 * Scopus #1356: Correct handling of embedded nulls.
 *
 * Revision 7.1  1994/10/16  03:13:28  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.2  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 *
 * Revision 6.1  1994/04/15  19:48:55  vriezen
 * Move all files to 6.1
 *
 * Revision 1.2  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 1.1  1993/02/11  02:05:23  myersn
 * Initial revision
 *
 */

#include "rw/wstring.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWWTokenizer {
  const RWWString*      theString;
  const wchar_t*        place;
public:
  RWWTokenizer(const RWWString& s);     // Construct to lex a string

  // Advance to next token, delimited by s:
  RWWSubString          operator()(const wchar_t* s);
  RWWSubString          operator()(const wchar_t* s, size_t); // nulls ok
  RWWSubString          operator()(); // { return operator()(W" \t\n\0",4);}

  RW_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWWTOKEN_H__ */
