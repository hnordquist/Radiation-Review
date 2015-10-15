#ifndef __RWEV_SCNTN_H__
#define __RWEV_SCNTN_H__

/***************************************************************************
 *
 * ev_scntn.h - RW_VCntnr<StdColl,RWColl>
 *
 *     : value-based interface components for STL Containers
 *
 * $Id: ev_scntn.h@#/main/25  09/04/97 17:25:19  griswolf (TOOLS707_WIN32_971021)
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



#include "rw/edefs.h"
#include "rw/ev_cntnr.h"

class RWExport RWFile;
class RWExport RWvistream;
class RWExport RWvostream;

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

template <class StdColl, class RWColl>
class RWTExport RW_VCntnr : public RW_VBase<StdColl, RWColl>
{
public:
  typedef typename StdColl::value_type               value_type;
  typedef typename StdColl::size_type                size_type;
  typedef typename StdColl::const_reference          const_reference;
  typedef typename StdColl::reference                reference;
  typedef typename StdColl::iterator                 my_iter_;
  typedef typename StdColl::const_iterator           const_my_iter_;


  bool          find(
            const_reference,
            value_type& val
                     ) const;

  bool          find(
            bool(*fn)(const_reference,void*),
            void* x, value_type& val
                      ) const;

  const_reference minElement() const{
    RWPRECONDITION(!this->isEmpty()); 
    return *min_element(this->begin(),this->end()); 
  }

  const_reference maxElement() const  { 
    RWPRECONDITION(!this->isEmpty()); 
    return *max_element(this->begin(),this->end()); 
  }

  reference       minElement()
  {
    RWPRECONDITION(!this->isEmpty());
#   ifdef _MSC_VER
      return (reference) *min_element(this->begin(),this->end());
#   else
      return (reference) *(my_iter_)min_element(this->begin(),this->end());
#   endif  
  }

  reference       maxElement()
  {
    RWPRECONDITION(!this->isEmpty());
#   ifdef _MSC_VER
      return (reference) *max_element(this->begin(),this->end());
#   else
      return (reference) *(my_iter_)max_element(this->begin(),this->end());
#   endif
  }

protected:
  RW_VCntnr() { } // prevent direct usage
  RW_VCntnr(const RW_VCntnr<StdColl,RWColl>&) { }
  
  RW_T_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_COMPILE_INSTANTIATE
#include "rw/ev_scntn.cc"
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWEV_SCNTN_H__ */
