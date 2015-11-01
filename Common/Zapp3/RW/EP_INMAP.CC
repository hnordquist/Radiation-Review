/***************************************************************************
 *
 * ep_inmap.cc - template definitions for
 *               RW_PMapAssoc<StdColl,RWColl,Key,T>
 * 
 * $Id: ep_inmap.cc@#/main/12  09/03/97 18:41:51  griswolf (TOOLS707_WIN32_971021)
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
 ****************************************************************************/

template <class StdColl, class RWColl, class Key, class T>
void
RW_PMapAssoc<StdColl,RWColl,Key,T>::
clearAndDestroy()
{
  // first delete items we point to 
  for(iterator it = this->begin(); it != this->end(); ++it) {
    delete (Key*) (*it).first;  // cast away const for non-std impls
    delete (*it).second;
  }
  clear(); // then empty self
}

/*
 * Special function for testing equality of two hash collections.
 * The standard algorithm 'equal' is not suitable due to the
 * inherent un-orderedness of hash collections.
 */
template <class RWColl>
bool hashMapCollEqual(const RWColl& h1, const RWColl& h2)
{
  RWPRECONDITION(h1.entries() == h2.entries());

  typename RWColl::const_iterator i = h1.std().begin();
  
  while(i != h1.std().end()) {
    typename RWColl::size_type num = h1.std().count((*i).first);
    if (num != h2.std().count((*i).first)) {
      return false;
    }
    else {
      while(num-- ) ++i;
    }
  }
  return true;
}
