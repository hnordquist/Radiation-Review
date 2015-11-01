/***************************************************************************
 *
 * ev_inmap.cc
 *      - template definitions for RW_VMapAssoc<StdColl,RWColl,Key,T>
 * 
 * $Id: ev_inmap.cc@#/main/4  09/04/97 17:25:05  griswolf (TOOLS707_WIN32_971021)
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


#define INFILE_TEMPL_DECL \
          template <class StdColl, class RWColl, class Key, class T>
#define VMA      RW_VMapAssoc<StdColl, RWColl, Key, T>

// handy macros, #undef'd below
#define KEY(i)    ((*i).first)
#define DATA(i)   ((*i).second)

INFILE_TEMPL_DECL void
VMA::apply(void (*fn)(const Key&, T&, void*), void* x)
{
  for(my_iter_ i = this->begin(); i != this->end(); ++i)
    (*fn)(KEY(i), DATA(i), x);
}

INFILE_TEMPL_DECL void
VMA::apply(void (*fn)(const Key&, const T&, void*),void*x) const
{
  for(const_my_iter_ i = this->begin(); i != this->end(); ++i)
    (*fn)(KEY(i), DATA(i), x);
}

INFILE_TEMPL_DECL bool
VMA::remove(bool (*fn)(typename VMA::const_reference,void*),void* x)
{
  my_iter_ hit = this->begin();
  for(/**/; hit != this->end(); ++hit) {
    if((*fn)(*hit,x)) {
      this->std().erase(hit);
      return true;
    }
  }
  return false;
}

INFILE_TEMPL_DECL typename VMA::size_type
VMA::removeAll(bool (*fn)(typename VMA::const_reference, void*), void* x)
{
  size_type    ret = this->entries();
  
  while (this->entries() && (*fn)(*(this->begin()),x))
    this->std().erase(this->begin());

  my_iter_ iter(this->begin());
  my_iter_ iter2;

  while (iter != this->end()) {
    iter2 = iter;
    ++iter2;
    if (iter2 != this->end() && (*fn)(*iter2,x))
      this->std().erase(iter2);
    else
      ++iter;
  }
  return ret - this->entries();
}

#undef KEY
#undef DATA

#undef INFILE_TEMPL_DECL
#undef VMA
