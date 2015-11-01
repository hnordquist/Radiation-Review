#ifndef __RWCTOKEN_H__
#define __RWCTOKEN_H__

/*
 * RWCTokenizer --- converts strings into sequences of tokens
 *
 * $Id: ctoken.h@#/main/10  07/18/96 04:33:02  hart (TOOLS707_WIN32_971021)
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
 * $Log: ctoken.h,v $
 * Revision 7.6  1996/07/18 04:33:02  hart
 * ObjectStore changes
 *
 * Revision 7.5  1996/05/08 16:59:31  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.4  1996/02/18 01:39:47  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.3  1995/09/05 18:27:53  jims
 * Use new copyright macro
 *
 * Revision 7.2  1995/07/07  23:00:56  griswolf
 * Scopus #1356: correct handling for embedded nulls.
 *
 * Revision 7.1  1994/10/16  01:00:32  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.3  1994/07/12  18:46:56  vriezen
 * Update Copyright notice
 *
 * Revision 6.2  1994/06/22  22:24:53  vriezen
 * Bug# 553.  Use value semantics on constructor
 *
 * Revision 6.1  1994/04/15  19:09:46  vriezen
 * Move all files to 6.1
 *
 * Revision 1.2  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 1.1  1993/02/11  00:51:04  myersn
 * Initial revision
 *
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.4   01 Mar 1992 15:45:24   KEFFER
 * Now uses RWCString instead of RWString.
 * 
 *    Rev 1.2   28 Oct 1991 09:08:26   keffer
 * Changed inclusions to <rw/xxx.h>
 * 
 *    Rev 1.1   22 Aug 1991 10:43:26   keffer
 * operator()() no longer uses default arguments (as per the ARM).
 * 
 *    Rev 1.0   28 Jul 1991 08:17:36   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/cstring.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWCTokenizer {
  const RWCString       theString;
  const char*           place;
public:
  RWCTokenizer(const RWCString& s);     // Construct to lex a string

  // Advance to next token, delimited by s:
  RWCSubString          operator()(const char* s);
  RWCSubString          operator()(const char*, size_t); //allow nulls
  RWCSubString          operator()(); // { return operator()(" \t\n\000",4); }

  RW_TYPESPEC  /* This macro usually expands to nothing */
};

typedef RWCTokenizer RWTokenizer;  // backward compatibility

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWCTOKEN_H__ */
