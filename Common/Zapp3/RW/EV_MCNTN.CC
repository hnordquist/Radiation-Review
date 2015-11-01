/***************************************************************************
 *
 * ev_mcntn.cc - template definitions for RW_VMapCntnr<StdColl,RWColl,Key,T>
 * 
 * $Id: ev_mcntn.cc@#/main/16  09/04/97 17:25:15  griswolf (TOOLS707_WIN32_971021)
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

#include "rw/rwfile.h"
#include "rw/vstream.h"

#define INFILE_TEMPL_DECL \
       template <class StdColl, class RWColl, class Key, class T>
#define VMC      RW_VMapCntnr<StdColl, RWColl, Key, T>

#ifndef RW_REF_SEEMS_PTR
INFILE_TEMPL_DECL bool
VMC::find(bool (*fn)(typename VMC::const_reference,void*), void* x, pair<Key,T>& val) const
{
  const_my_iter_ i = this->begin();
  for(/**/ ; i != this->end(); ++i)
    if((*fn)(*i,x))
      break;
  if(i != this->end()) {
    val.first = (*i).first;
    val.second =(*i).second;
    return true;
  }
  return false;
}
#endif

INFILE_TEMPL_DECL bool
VMC::find(const_reference key, pair<Key,T>& val) const
{
  const_my_iter_ i = this->begin();
  for(/**/ ; i != this->end(); ++i)
    if(*i == key)
      break;
  if(i != this->end()) {
    val.first = (*i).first;
    val.second =(*i).second;
    return true;
  }
  return false;
}

#undef INFILE_TEMPL_DECL
#undef VMC
