#ifndef __RWEV_SEQ_H__
#define __RWEV_SEQ_H__

/***************************************************************************
 *
 * ev_seq.h - RW_VSeq<StdColl,RWColl>
 *   
 *     : value-based interface components for STL Sequences
 * 
 * $Id: ev_seq.h@#/main/25  09/04/97 17:25:22  griswolf (TOOLS707_WIN32_971021)
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

#include "rw/ev_scntn.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl>
class RWTExport RW_VSeq : public RW_VCntnr<StdColl, RWColl>
{
public:
  typedef typename StdColl::value_type        value_type;
  typedef typename StdColl::size_type         size_type;
  typedef typename StdColl::iterator          my_iter_;
  typedef typename StdColl::const_iterator    const_my_iter_;
  typedef typename StdColl::reference         reference;
  typedef typename StdColl::const_reference   const_reference;
  typedef const value_type&                   const_val_ref;

  void          append(const_reference item)
    { this->std().insert(this->end(),item); }

  void          clear() { this->std().erase(this->begin(), this->end()); }

  bool  insert(const_reference key) // fail only thru exception
    { this->std().insert(this->end(),key); return true; }

  value_type    get() { return removeFirst(); }
  
  void          insertAt(size_type index, const_reference item)
    {
      // index equal to number of entries OK (inserts at end)
      if(index > 0) boundsCheck(index-1);
      my_iter_ i = this->begin();
      advance(i,index);
      this->std().insert(i,item);
    }
  
  void          prepend(const_reference item)
    { this->std().insert(this->begin(),item); }

  bool          remove(bool (*fn)(const_reference,void*),void*);
  bool          remove(const_reference key);

  size_type     removeAll(const_reference key);
  size_type     removeAll(bool (*fn)(const_reference, void*),void*);

  value_type    removeAt(size_type index);

  value_type    removeFirst();

  size_type     replaceAll(const value_type& oldVal, const value_type& newVal);
  size_type     replaceAll(bool (*fn)(const value_type&, void*), void* x,
                           const value_type& newVal);

// hide some things
private:
  RW_VSeq(const RW_VSeq<StdColl,RWColl>&);

protected:
  RW_VSeq() {} // prevent direct use
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ev_seq.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEV_SEQ_H__ */
