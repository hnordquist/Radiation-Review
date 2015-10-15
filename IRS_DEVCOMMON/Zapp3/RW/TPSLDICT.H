#ifndef __RWTPSLDICT_H__
#define __RWTPSLDICT_H__
/*
 * RWTPtrSlistDictionary<KP,VP>: A dictionary of pointers to keys of type KP,
 *   and pointers to values of type VP, implemented as a singly-linked list.
 *
 * $Id: tpsldict.h@#/main/25  09/04/97 17:25:47  griswolf (TOOLS707_WIN32_971021)
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
 * Assumes that KP has:
 *   - well-defined equality semantics (equiv of KP::operator==(const KP&)).
 *
 ***************************************************************************/

#include "rw/tislist.h"
#include "rw/tasslnk.h"

template <class KP, class VP> class RWTExport RWTPtrSlistDictionaryIterator;

/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrSlistDictionary<KP,VP>           *
 *                                                              *
 ****************************************************************/

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class KP, class VP>
class RWTExport RWTPtrSlistDictionary 
                         : private RWTIsvSlist< RWTPtrAssocLink<KP, VP> >
{

public:

  RWTPtrSlistDictionary() {;}
  RWTPtrSlistDictionary(const RWTPtrSlistDictionary<KP,VP>&);
  ~RWTPtrSlistDictionary()
        {RWTIsvSlist<RWTPtrAssocLink<KP,VP> >::clearAndDestroy();}

  // Operators:
  RWTPtrSlistDictionary<KP,VP>&
                        operator=(const RWTPtrSlistDictionary<KP,VP>&);
  VP*&                  operator[](KP* key); // Look up key, return value

  // Member functions:
  void                  applyToKeyAndValue(void (*applyFun)(KP*,VP*&,void*), void*);

  void                  clear()
        {RWTIsvSlist<RWTPtrAssocLink<KP,VP> >::clearAndDestroy();}

  void                  clearAndDestroy();

  RWBoolean             contains(const KP*) const;      // Contain key?

  size_t                entries() const
        {return RWTIsvSlist<RWTPtrAssocLink<KP,VP> >::entries();}

  RWBoolean             isEmpty() const
        {return RWTIsvSlist<RWTPtrAssocLink<KP,VP> >::isEmpty();}

  KP*                   find(const KP* key) const;
  KP*                   findKeyAndValue(const KP* key, VP*& retVal) const;
  VP*                   findValue(const KP* key) const;

  void                  insertKeyAndValue(KP* key, VP* value)
        { (*this)[key] = value; }

  KP*                   remove(const KP* key);
  KP*                   removeKeyAndValue(const KP* key, VP*& retVal);

protected:

  RWTPtrAssocLink<KP,VP > *     findLink(const KP*) const;
  RWTPtrAssocLink<KP,VP > *     removeLink(const KP*);

private:

  friend class RWTExport RWTPtrSlistDictionaryIterator<KP,VP>;

  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};


/****************************************************************
 *                                                              *
 *     Declarations for RWTPtrSlistDictionaryIterator<KP,VP>    *
 *                                                              *
 ****************************************************************/

template <class KP, class VP>
class RWTExport RWTPtrSlistDictionaryIterator :
        private RWTIsvSlistIterator<RWTPtrAssocLink<KP,VP> >
{

public:

  // Constructor:
  RWTPtrSlistDictionaryIterator(RWTPtrSlistDictionary<KP,VP>& d)
        : RWTIsvSlistIterator<RWTPtrAssocLink<KP,VP> >(d) {;}

  RWTPtrSlistDictionaryIterator(const RWTPtrSlistDictionaryIterator<KP,VP>&);
    // Added to address BUG#117

  // Operators:
  RWBoolean     operator++() { 
    this->advance(); 
    return this->cursor()!=container()->tailLink();
  }

  RWBoolean     operator+=(size_t n)  { 
    return RWTIsvSlistIterator<RWTPtrAssocLink<KP,VP> >::operator+=(n)!=rwnil;
  }

  KP*           operator()()    { 
    this->advance(); 
    return this->cursor()==container()->tailLink() ? 
      rwnil : this->cursor()->key_;
  }

  RWTPtrSlistDictionary<KP,VP>* container() const  { 
    return (RWTPtrSlistDictionary<KP,VP>*)RWTIsvSlistIterator<RWTPtrAssocLink<KP,VP> >::container();
  }

  KP*           key() const
        {return this->cursor()->key_;}

  void          reset()
        {RWTIsvSlistIterator<RWTPtrAssocLink<KP,VP> >::reset();}
  void          reset(RWTPtrSlistDictionary<KP,VP>& s)
        {RWTIsvSlistIterator<RWTPtrAssocLink<KP,VP> >::reset(s);}

  VP*           value() const
        {return this->cursor()->value_;}

private:

  // Disallow postfix increment.  Unless we hide it, some compilers will
  // substitute the prefix increment operator in its place.
  RWBoolean             operator++(int);
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tpsldict.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif  /* __RWTPSLDICT_H__ */
