#ifndef _ZAPPBASE_INCLUDED
#define _ZAPPBASE_INCLUDED 1
/*
 * (c) Copyright 1991 - 1996 Rogue Wave Software, Inc.
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



#define Z3 1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <rw/tpslist.h>
#include <rw/tpdlist.h>
#include <rw/tvslist.h>
#include <rw/rwtime.h>
#include <rw/rwdate.h>
#include <rw/collect.h>
#include <rw/disktree.h>
#include <rw/slistcol.h>
#include <rw/ctoken.h>
#include <rw/btree.h>
#include <rw/collstr.h>

#include <zapph/platform.hpp>
#include <zapph/enumdef.hpp>
#include <zapph/assert.hpp>
#include <zapph/memmgr.hpp>
#include <zapph/darray.hpp>
#include <zapph/persist.hpp>
#include <zapph/llink.hpp>
#include <zapph/zslist.hpp>
#include <zapph/dlist.hpp>

declDlist(zWindow)

#include <rw/cstring.h>
#include <zapph/string.hpp>
#include <zapph/general.hpp>
#include <zapph/events.hpp>

#include <zapph/winmap.hpp>

#include <zapph/evh.hpp>
#include <zapph/scroll.hpp>
#include <zapph/resid.hpp>
#include <zapph/cursor.hpp>
#include <zapph/sizer.hpp>
#include <zapph/window.hpp>
#include <zapph/device.hpp>
#include <zapph/framewin.hpp>
#include <zapph/popupfr.hpp>
#include <zapph/childwin.hpp>
#include <zapph/display.hpp>
#include <zapph/caret.hpp>
#include <zapph/pane.hpp>
#include <zapph/task.hpp>
#include <zapph/text.hpp>
#include <zapph/dialog.hpp>
#include <zapph/control.hpp>
#include <zapph/multi.hpp>
#include <zapph/timer.hpp>
#include <zapph/mail.hpp>
#include <zapph/app.hpp>
#include <zapph/prjob.hpp>
#include <zapph/message.hpp>
#include <zapph/childfr.hpp>
#include <zapph/zapp.inl>
#include <zapph/clip.hpp>
#include <zapph/scroller.hpp>
#include <zapph/formdlg.hpp>
#include <zapph/comdlg.hpp>
#include <zapph/editor.hpp>
#include <zapph/tab.hpp>

#include <zapph/zappole.hpp>
#include <zapph/dataobj.hpp>
#include <zapph/dataobji.hpp>
#include <zapph/drgdrpi.hpp>
#include <zapph/drgsrc.hpp>
#include <zapph/drptrg.hpp>
#include <zapph/registry.hpp>
#include <zapph/regimp.hpp>


#include <zapph/mdi.hpp>
#include <zapph/lastdefs.hpp>
#endif
