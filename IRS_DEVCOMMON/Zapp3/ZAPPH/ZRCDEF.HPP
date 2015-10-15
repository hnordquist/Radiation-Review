#ifndef _ZAPPRCDEF_INCLUDED
#define _ZAPPRCDEF_INCLUDED 1
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

// For tabctrl
#define Z_TABTRAPEZOID 0x00000004L

// For notebook dialogs
# if defined(WIN32) && defined(Z_NATIVE_NBD)
#define ZND_ADDHELP	PSP_HASHELP
# else
#define ZND_ADDHELP	1L
# endif
#define ZND_REMOVEAPPLY 2L

// For progress bars
#define	Z_LEFTTORIGHT 8
#define	Z_RIGHTTOLEFT 16
#define	Z_TOPTOBOTTOM 32
#define	Z_BOTTOMTOTOP 64
#define	Z_BORDER3D 128
#define	Z_BORDERNORMAL 256
#define	Z_BORDERNONE 512

// Status lines
#define ZSLTEXT_NOSPACE      0x0800
#define ZSLTEXT_DOCLIPPING   0x0400

#define ZSLITEM_NOSPACE		 0x0800
#define ZSLITEM_DOCLIPPING	 0x0400

#define ZSLTEXT_SPACE 8
#define ZSLITEM_SPACE 8
#define ZSL_CREATEMAINITEM  0x0100
#define ZSL_NUMLOCK       0x0800
#define ZSL_CLOCK         0x1000
#define ZSL_CAPSLOCK      0x4000
#define ZSL_MENUHELPOVER  0x2000
#define ZSL_MENUHELPITEM  (0x8000 | ZSL_CREATEMAINITEM)

#define ZSL_MAINITEM     0

#define ZSL_STANDARD (ZSL_CLOCK | ZSL_CAPSLOCK | ZSL_NUMLOCK | ZSL_CREATEMAINITEM | ZSL_MENUHELPOVER)
#define ZSL_STANDARDITEM (ZSL_CLOCK | ZSL_CAPSLOCK | ZSL_NUMLOCK | ZSL_CREATEMAINITEM | ZSL_MENUHELPITEM)
 

// Table
#define ZRES_COLUMNCURSOR 30000
#define ZRES_ROWCURSOR 30001

// Toolbar
#define ZTB_BORDERAUTO 0x00000001L
#define ZTB_BORDERRIGHT 0x00000002L
#define ZTB_BORDERLEFT  0x00000004L
#define ZTB_BORDERTOP   0x00000008L
#define ZTB_BORDERBOTTOM 0x00000010L

#define ZTB_3D 0x00000200L
#define ZTB_STANDARD (ZTB_BORDERAUTO)


// Tree
#define ZTREE_OPTFULLSPACE			0x00000001
#define ZTREE_OPTSELBMPASEXPANDED   0x00000002

// Valueset
#define ZVS_NODRAWHILITE 0x0001

//  zIP Style Bit Defines
#ifndef _ZAPPVISUAL_INCLUDED
#define ZTXT3D_DEFFONT          0x00000040   
#define ZTXT3D_NOSHADING        0x00000000   
#define ZTXT3D_INSET            0x00000080
#define ZTXT3D_INSETHEAVY       0x000000C0
#define ZTXT3D_RAISED           0x00000100
#define ZTXT3D_RAISEDHEAVY      0x00000140
#define ZTXT3D_LEFT             0x00000800 
#define ZTXT3D_CENTER           0x00001000
#define ZTXT3D_RIGHT            0x00001800
#define ZCHK3D_INSET            0x00000000  
#define ZCHK3D_RAISED           0x00000200
#define ZCHK3D_CROSS            0x00000000	
#define ZCHK3D_CHECK            0x00000400	
#define ZRDO3D_INSET            0x00000000   
#define ZRDO3D_RAISED           0x00000200
#define ZGRP3D_INSET            0x00000000  
#define ZGRP3D_RAISED           0x00000200
#define ZGRP3D_BLACK            0x00000000  
#define ZGRP3D_GRAY             0x00000400
#define ZBB_ONESTATE            0x00000000 
#define ZBB_TWOSTATE            0x00000200
#define ZBB_TOP                 0x00000400
#define ZBB_LEFT                0x00000800
#define ZBB_RIGHT               0x00000000  
#define ZBB_BOTTOM              0x00000C00
#define ZBB_FOCUSRECTOFF        0x00000000	
#define ZBB_FOCUSRECTAUTO       0x00001000
#define ZBB_BORDERAUTO          0x00000000  
#define ZBB_BORDEROFF           0x00002000
#define ZBB_PARTIAL             0x00000000  
#define ZBB_FULL                0x00004000
#define ZBB_OFFONLY             0x00000000  
#define ZBB_OFFDEP              0x00000010
#define ZBB_OFFDEPON            0x00000020
#define ZBB_OFFON               0x00000030		
#define ZBB_TEXTFOCUSRECTAUTO   0x00008000
#define ZBB_TEXTFOCUSRECTOFF    0x00000000
#define ZPNL3D_OUTINSET         0x00000000  
#define ZPNL3D_OUTRAISED        0x00000200
#define ZPNL3D_GRAY             0x00000000  
#define ZPNL3D_BLACK            0x00000400
#define ZPNL3D_ININSET          0x00000000	
#define ZPNL3D_INRAISED         0x00004000
#endif

#endif
