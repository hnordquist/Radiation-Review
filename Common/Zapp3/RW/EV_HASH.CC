/***************************************************************************
 *
 * ev_hash.cc - 
 * 
 * $Id: ev_hash.cc@#/main/8  09/04/97 17:25:01  griswolf (TOOLS707_WIN32_971021)
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
#define VHA      RW_VHAssoc<StdColl, RWColl>

INFILE_TEMPL_DECL
void VHA::UnionHash(const StdColl& stdcoll)
{
// depends on all eq items being adjacent in each collection
  typename StdColl::key_equal_type eq = this->std().key_equal();
  typename StdColl::const_iterator probe = stdcoll.begin();
  while(probe != stdcoll.end()) {
    typename StdColl::iterator found = this->std().find(*probe);
    if(found != this->end())          // cope with (multiple) match(es)
    {
      const typename StdColl::value_type& match = *found;
      while(   found != this->end() && probe != stdcoll.end()
            && eq(match,*found) && eq(*found,*probe))
        // if we have a match, then march in sequence 1 or more times
      {
        ++found;
        ++probe;
      }
      // if no match, or no _more_ match; insert (remainder) into self
      while(probe != stdcoll.end() && eq(match,*probe))
      {
        this->std().insert(*probe++);
      }
    }
    else                        // self has no match to *probe
    {
      const typename StdColl::value_type& match = *probe;
      while(probe != stdcoll.end() && eq(match,*probe)) {
        this->std().insert(*probe++);
      }
    }
  }
}

INFILE_TEMPL_DECL
void VHA::intersectionHash(const StdColl& stdcoll)
{
  // depends on all eq items being adjacent in each collection
  typename StdColl::key_equal_type eq = this->std().key_equal();
  typename StdColl::const_iterator look = this->begin();
  while(look != this->end()) {
    typename StdColl::const_iterator probe = stdcoll.find(*look);
    const typename StdColl::value_type& match = *look;
    while(   probe != stdcoll.end() && look != this->end() 
          && eq(match,*look) && eq(*look,*probe)) 
      // if we have a match, then march in sequence 1 or more times
    {
      ++probe;
      ++look;
    }
    // no match, or no _more_ match: remove (trailing) part in self
    while(look != this->end() && eq(match,*look))
      look = this->std().erase(look); // depends on "next iter" semantics
  }
}

INFILE_TEMPL_DECL
void VHA::differenceHash(const StdColl& stdcoll)
{
  // depends on all eq items being adjacent in each collection
  typename StdColl::key_equal_type eq = this->std().key_equal();
  typename StdColl::const_iterator look = this->begin();
  while(look != this->end()) {
    typename StdColl::const_iterator probe = stdcoll.find(*look);
    const typename StdColl::value_type& match = *look;
    while(   probe != stdcoll.end() && look != this->end()
          && eq(match,*look) && eq(*look,*probe))
    { // if there is a match, then erase (them) in sequence
      look = this->std().erase(look); // depends on "next iter" semantics
      ++probe;
    }
    // no match, or no more match: skip (trailing) part in self
    while(look != this->end() && eq(match,*look))
      ++look;
  }
}  

INFILE_TEMPL_DECL
void VHA::symmetricDifferenceHash(const StdColl& stdcoll)
{
  // depends on all eq items being adjacent in each collection
  typename StdColl::key_equal_type eq = this->std().key_equal();
  typename StdColl::const_iterator probe = stdcoll.begin();
  while(probe != stdcoll.end()) {
    typename StdColl::iterator found = this->std().find(*probe);
    if(found != this->end())          // cope with (multiple) match(es)
    {
      const typename StdColl::value_type& match = *found;
      while(   found != this->end() && probe != stdcoll.end()
            && eq(match,*found) && eq(*found,*probe))
        // if we have a match, then remove in sequence 1 or more times
      {
        found = this->std().erase(found);
        ++probe;
      }
      // if no match, or no _more_ match; insert (remainder) into self
      while(probe != stdcoll.end() && eq(match,*probe))
      {
        this->std().insert(*probe++); 
      }
    }
    else                        // self has no match to *probe 
    {
      const typename StdColl::value_type& match = *probe;
      while(probe != stdcoll.end() && eq(match,*probe)) {
        this->std().insert(*probe++);
      }
    }
  }
}

#undef INFILE_TEMPL_DECL
#undef VHA
