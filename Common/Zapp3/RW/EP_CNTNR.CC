/***************************************************************************
 *
 * ep_cntnr.cc - template definitions for RW_PBase<StdColl,RWColl,DataTypes>
 * 
 * $Id: ep_cntnr.cc@#/main/18  09/09/97 16:23:52  kevinj (TOOLS707_WIN32_971021)
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


#define INFILE_TEMPL_DECL template <class StdColl, class RWColl>
#define PC       RW_PBase<StdColl,RWColl>

#if !defined(__SUNPRO_CC) || __SUNPRO_CC<0x420

INFILE_TEMPL_DECL
void
PC::apply(void (*fn)(typename PC::value_type,void*),void *x) {
  my_iter_ iter = begin();
  while(iter != end()) {
    (*fn)(*iter,x);
    ++iter;
  }
}

INFILE_TEMPL_DECL
void
PC::apply(void (*fn)(typename PC::reference,void*),void *x) {
  my_iter_ iter = begin();
  while(iter != end()) {
    (*fn)(*iter,x);
    ++iter;
  }
}

INFILE_TEMPL_DECL
bool
PC::contains(bool(*fn)(typename PC::value_type,void*),void* x) const {
  const_my_iter_ iter = begin();
  while(iter != end()) {
    if((*fn)(*iter,x)) break;
    ++iter;
  }
  return iter != end();
}

#ifndef RW_BROKEN_NAME_RESOLUTION

INFILE_TEMPL_DECL
typename PC::size_type
PC::index(bool (*fn)(typename PC::value_type, void*), void* x) const
{
  const_my_iter_ i = begin();
  size_type idx = 0;
  for(/**/; i != end(); ++i) {
    if((*fn)(*i,x))
      return idx;
    ++idx;
  }
  return npos;
}


INFILE_TEMPL_DECL
typename PC::size_type
PC::occurrencesOf(bool (*fn)(typename PC::value_type,void*),void* x) const
{
  size_type ret = 0;
  const_my_iter_ iter = begin();
  while(iter != end()) {
    if((*fn)(*iter,x)) ++ret;
    ++iter;
  }
  return ret;
}
#endif
#endif /* !SunPro 4.2 */

INFILE_TEMPL_DECL
typename PC::size_type PC::npos = ~(typename PC::size_type)0;

#undef INFILE_TEMPL_DECL
#undef PC









