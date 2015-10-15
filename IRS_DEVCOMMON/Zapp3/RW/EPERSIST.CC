/***************************************************************************
 *
 * epersist.cc
 *
 * $Id: epersist.cc@#/main/1  03/18/96 06:49:49  jims (TOOLS707_WIN32_971021)
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
 * $Log: epersist.cc,v $
 * Revision 7.2  1996/03/18 06:49:49  jims
 * Add header
 *
 *
 */

#ifdef RW_NO_STL
// provide functions so we can use apply() to stream data out
#  ifndef RW_NO_REF_TO_REF
template<class T>
void rwInsertInStream(T& item, void*x) {
  *(RWvostream*)x << item;
}

template<class T>
void rwInsertInFile(T& item, void*x) {
  *(RWFile*)x << item;
}
#  else
template<class T>
void rwInsertInStream(T item, void*x) {
  *(RWvostream*)x << item;
}

template<class T>
void rwInsertInFile(T item, void*x) {
  *(RWFile*)x << item;
}
#  endif /* RW_NO_REF_TO_REF */

#endif /* RW_NO_STL */
