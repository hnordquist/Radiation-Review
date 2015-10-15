#ifndef __RWEPFUNC_H__
#define __RWEPFUNC_H__

/***************************************************************************
 *
 * epfunc.h - work-around classes for problems with some STL functors 
 *   
 * $Id: epfunc.h@#/main/13  09/04/97 17:24:54  griswolf (TOOLS707_WIN32_971021)
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
 **************************************************************************/

#include "rw/compiler.h"

#ifndef RW_HEADER_WITH_EXTENSION
#  include <functional>
#else
#  include <functional.hpp>
#endif  /* RW_HEADER_WITH_EXTENSION */

template <class Arg1, class Arg2, class Result>
class x_pointer_to_binary_function : public binary_function<Arg1, Arg2, Result>
{ 
protected:
    Result (*ptr)(Arg1, Arg2);
public:
    x_pointer_to_binary_function(Result (*x)(Arg1, Arg2)) : ptr(x) {}
    Result operator()(const Arg1 x, const Arg2 y) const {
        return ptr(x, y); 
    }
};


template <class Operation> 
class x_binder2nd : public unary_function<typename Operation::first_argument_type,
                                          typename Operation::result_type> {
protected:
    Operation op;
    typename Operation::second_argument_type value;
public:
    x_binder2nd(const Operation& x, const typename Operation::second_argument_type y) 
        : op(x), value(y) {}
    
#ifdef RW_NO_CONST_PARAM_OVERLOAD
    typename Operation::result_type operator()(argument_type x) const
#else
    typename Operation::result_type operator()(const typename
                 unary_function<typename Operation::first_argument_type,
                 typename Operation::result_type>::argument_type x) const
#endif
                 {return op(x, value);}
};

template <class Operation, class T>
x_binder2nd<Operation> x_bind2nd(const Operation& op, const T& x) {
    return x_binder2nd<Operation>(op, Operation::second_argument_type(x));
}

#endif /* __RWEPFUNC_H__ */
