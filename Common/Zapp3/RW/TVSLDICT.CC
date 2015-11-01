/*
 * Template definitions for RWTValSlistDictionary<K,V>
 *   and RWTValSlistDictionaryIterator<K,V>
 *
 * $Id: tvsldict.cc@#/main/15  09/04/97 17:25:58  griswolf (TOOLS707_WIN32_971021)
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
 ***************************************************************************/

// Copy constructor (some of these names get pretty darn long, eh?)
template <class K, class V>
RWTValSlistDictionary<K,V>::RWTValSlistDictionary(const RWTValSlistDictionary<K,V>& d)
{
  // Cast away "constness", which we will honor anyway...
  RWTValSlistDictionaryIterator<K,V> next((RWTValSlistDictionary<K,V>&)d);

  while (++next)
    insertKeyAndValue(next.key(), next.value());

  RWPOSTCONDITION(entries()==d.entries());
}

template <class K, class V> RWTValSlistDictionary<K,V>&
RWTValSlistDictionary<K,V>::operator=(const RWTValSlistDictionary<K,V>& d)
{
  if (this!=&d)
  {

    clear();

    // Cast away "constness", which we will honor anyway...
    RWTValSlistDictionaryIterator<K,V> next((RWTValSlistDictionary<K,V>&)d);

    while (++next)
      insertKeyAndValue(next.key(), next.value());

  }
  RWPOSTCONDITION(entries()==d.entries());
  return *this;
}

template <class K, class V> V&
RWTValSlistDictionary<K,V>::operator[](const K& key)
{
  RWTValAssocLink<K,V>* assoc = findLink(key);

  if (assoc==rwnil)
    RWTIsvSlist<RWTValAssocLink<K,V> >
      ::insert(assoc = RW_NEW(this,(RWTValAssocLink<K,V>::get_os_typespec()))
               RWTValAssocLink<K,V>(key));
  RWPOSTCONDITION(key == assoc->key_);
  return assoc->value_;
}

template <class K, class V> void
RWTValSlistDictionary<K,V>::applyToKeyAndValue(void (*applyFun)(const K&, V&, void*), void* a)
{
  RWTValAssocLink<K,V>* link = this->firstLink();
  while (link != this->tailLink())
  {
    applyFun(link->key_, link->value_, a);
    link = link->next();
  }
}

template <class K, class V> RWBoolean
RWTValSlistDictionary<K,V>::contains(const K& key) const
{
  return findLink(key)!=rwnil;
}

template <class K, class V> RWBoolean
RWTValSlistDictionary<K,V>::find(const K& key, K& retKey) const
{
  RWTValAssocLink<K,V>* assoc = findLink(key);
  if (assoc)
  {
    retKey   = assoc->key_;
    RWPOSTCONDITION(retKey == key);
    return TRUE;
  }
  return FALSE;
}

template <class K, class V> RWBoolean
RWTValSlistDictionary<K,V>::findKeyAndValue(const K& key, K& retKey, V& retValue) const
{
  RWTValAssocLink<K,V>* assoc = findLink(key);
  if (assoc)
  {
    retKey   = assoc->key_;
    retValue = assoc->value_;
    RWPOSTCONDITION(retKey == key);
    return TRUE;
  }
  return FALSE;
}

template <class K, class V> RWBoolean
RWTValSlistDictionary<K,V>::findValue(const K& key, V& retValue) const
{
  RWTValAssocLink<K,V>* assoc = findLink(key);
  if (assoc)
  {
    retValue = assoc->value_;
    RWPOSTCONDITION(assoc->key_ == key);
    return TRUE;
  }
  return FALSE;
}

template <class K, class V> RWBoolean
RWTValSlistDictionary<K,V>::remove(const K& key)
{
  RWTValAssocLink<K,V>* assoc = removeLink(key);
  return assoc ? (delete assoc, TRUE) : FALSE;
}

template <class K, class V> RWBoolean
RWTValSlistDictionary<K,V>::remove(const K& key, K& retKey)
{
  RWTValAssocLink<K,V>* assoc = removeLink(key);
  if (assoc)
  {
    retKey = assoc->key_;
    RWPOSTCONDITION(retKey == key);
    delete assoc;
    return TRUE;
  }
  RWPOSTCONDITION(assoc==rwnil);
  return FALSE;
}

// Protected functions ---

template <class K, class V> RWTValAssocLink<K,V>*
RWTValSlistDictionary<K,V>::findLink(const K& key) const
{
  RWTValAssocLink<K,V>* link = this->firstLink();
  while (link != this->tailLink())
  {
    if (link->key_ == key)
      return link;
    link = link->next();
  }
  return rwnil;
}

template <class K, class V> RWTValAssocLink<K,V>*
RWTValSlistDictionary<K,V>::removeLink(const K& key)
{
  RWPRECONDITION(this->lastLink()->next()==this->tailLink());

  // Set up a sentinel:
  RWTValAssocLink<K,V> newTail(key);
  this->lastLink()->next_ = &newTail;

  // Now search for it:
  RWTValAssocLink<K,V>* link = this->headLink();
  while (!(link->next()->key_ == key))
    link = link->next();

  // Reset the pointer in the last link:
  this->lastLink()->next_ = this->tailLink();

  if (link != this->lastLink())
  {
    RWPOSTCONDITION(link->next()->key_ == key);
    //return removeRight(link);
    return (RWTValAssocLink<K,V>*) removeRight(link);
  }
  return rwnil;
}

template <class K, class V>
RWTValSlistDictionaryIterator<K,V>::RWTValSlistDictionaryIterator
       (const RWTValSlistDictionaryIterator<K,V>& sdi)
   : RWTIsvSlistIterator<RWTValAssocLink<K,V> >(sdi)
{ ; }
