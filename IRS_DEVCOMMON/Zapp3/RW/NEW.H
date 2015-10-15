#ifndef __RWNEW_H__
#define __RWNEW_H__

/*
 * Provide for missing placement new in Borland
 *
 * $Id: new.h@#/main/4  02/18/96 01:45:24  griswolf (TOOLS707_WIN32_971021)
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
 ****************************************************************************
 *
 * $Log: new.h,v $
 * Revision 7.3  1996/02/18 01:45:24  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.2  1995/09/05 19:14:44  jims
 * Use new copyright macro
 *
 * Revision 7.1  1994/10/16  03:36:53  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.1.7.1  1994/06/29  18:04:23  kevinj
 * Created
 *
 * Revision 6.1  1994/06/29  18:03:43  kevinj
 * Created
 */

#include <new.h>

#ifdef __TURBOC__
  inline void* operator new(size_t, void *p){return p;}
#endif

#endif  /* __RWNEW_H__ */
