#ifndef _ZAPPENUMDEF_INCLUDED
#define _ZAPPENUMDEF_INCLUDED 1
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


enum StockCaret { GrayBlob=1,SolidBlob=0 };

enum zMBtn { // Typical platform independent mouse button, Gauranteed
    zMBtnIndex  = 1,	// Mouse Button Under Index Finger
    zMBtnMiddle = 3,	// Mouse Button Under Middle Finger
    zMBtnRing   = 2	// Mouse Button Under Ring Finger
};

enum StockCursor {
	Arrow		=32512,
	Ibeam		=32513,
	Hourglass	=32514,
	Cross		=32515,
	UpArrow		=32516,
	SizeCursor	=32640,
	IconCursor	=32541,
	SizeNWSE	=32642,
	SizeNESW	=32643,
	SizeWE		=32644,
	SizeNS		=32645,
	NullCursor  =0
};


//
//	Some pre-defined color values which can be assigned to a Color

const unsigned long  RED	 = 0x000000ffL;
const unsigned long  GREEN	 = 0x0000ff00L;
const unsigned long  BLUE	 = 0x00ff0000L;
const unsigned long  BLACK	 = 0x00000000L;
const unsigned long  WHITE	 = 0x00ffffffL;
const unsigned long  LTGRAY   = 0x00c0c0c0L;
const unsigned long  GRAY	 = 0x00808080L;
const unsigned long  DKGRAY   = 0x00404040L;
	
enum PenStyle {
	Solid=PS_SOLID,
	Dash=PS_DASH,
	Dot=PS_DOT,
	DashDot = PS_DASHDOT,
	DashDotDot = PS_DASHDOTDOT,
	None = PS_NULL,
	NoPen = PS_NULL,
	InsideFrame = PS_INSIDEFRAME
};

enum StockPen {
	BlackPen = BLACK_PEN,
	WhitePen = WHITE_PEN,
	NullPen = NULL_PEN
};
	
enum StockBrush {
	WhiteBrush = WHITE_BRUSH,
	LiteGrayBrush = LTGRAY_BRUSH,
	GrayBrush = GRAY_BRUSH,
	DarkGrayBrush = DKGRAY_BRUSH,
	BlackBrush = BLACK_BRUSH,
	NullBrush = NULL_BRUSH,
	HollowBrush = NULL_BRUSH
};
	
enum BrushHatch {					
	DownHatch = HS_BDIAGONAL,
	UpHatch = HS_FDIAGONAL,
	CrossHatch = HS_CROSS,
	DiagCrossHatch = HS_DIAGCROSS,
	HorzHatch = HS_HORIZONTAL,
	VertHatch = HS_VERTICAL,
	ZBRUSH_BITMAP,
	NoHatch=99
};
	


enum StockFont {
	AnsiFixed = ANSI_FIXED_FONT,
	AnsiVariable = ANSI_VAR_FONT,
	DispDefault = DEVICE_DEFAULT_FONT,
	OemFixed = OEM_FIXED_FONT,
	SystemVariable = SYSTEM_FONT,
	SystemFixed = SYSTEM_FIXED_FONT
};

//
//	Font Styles can be ORed together.
//
#define FS_ITALIC		0x0001
#define FS_UNDERLINE	0x0002
#define FS_STRIKEOUT	0x0004

enum zChSet {
	AnsiCharSet = ANSI_CHARSET,
	OemCharSet = OEM_CHARSET,
	SymbolCharSet = SYMBOL_CHARSET,
	SJISCharSet,
	EUCCharSet,
	Big5CharSet,
	KSCCharSet,
	NoEscJISCharSet
};

enum zOutPrec {
	CharOutPrec = OUT_CHARACTER_PRECIS,
	DefaultOutPrec = OUT_DEFAULT_PRECIS,
	StringOutPrec = OUT_STRING_PRECIS,
	StrokeOutPrec = OUT_STROKE_PRECIS
	,OutlineOutPrec = OUT_TT_ONLY_PRECIS
};

enum zClipPrec {
	CharClipPrec = CLIP_CHARACTER_PRECIS,
	DefaultClipPrec = CLIP_DEFAULT_PRECIS,
	StrokeClipPrec = CLIP_STROKE_PRECIS
};

enum zOutQual {
	DefaultQual = DEFAULT_QUALITY,
	DraftQual = DRAFT_QUALITY,
	ProofQual = PROOF_QUALITY
};

enum zFntPitch {
	DefaultPitch = DEFAULT_PITCH,
	FixedPitch = FIXED_PITCH,
	VariablePitch = VARIABLE_PITCH
};

enum zFntFam {
	ffDontCare = FF_DONTCARE,
	ffRoman = FF_ROMAN,
	ffSwiss = FF_SWISS,
	ffModern = FF_MODERN,
	ffScript = FF_SCRIPT,
	ffDecorative = FF_DECORATIVE
};

// Binary Raster Operations
//
// This section lists the binary raster-operation codes used by the
// all of zApp's graphics functions, including line drawing and shape
// drawing.   Thse codes define how graphics combines the bits from
// the current zPen with the bits in the destination bitmap or in the
// pane in which the pen is used.
// Each raster-operation code represents a Boolean operation in
// which the values of the pixels in the selected pen and the
// destination bitmap are combined. Following are the two operands
// used in these operations:
//
// Operand	Meaning
//
// P	Selected pen
// D	Destination bitmap
//
// The Boolean operators used in these operations follow:
//
// Operator	Meaning
//
// a	Bitwise AND
// n	Bitwise NOT (inverse)
// o	Bitwise OR
// x	Bitwise exclusive OR (XOR)
//
// All Boolean operations are presented in reverse Polish notation. For
// example, the following operation replaces the values of the pixels in
// the destination bitmap with a combination of the pixel values of the
// pen and the selected brush:
//
//
//
// DPo
//
// Each raster-operation code is a 32-bit integer whose high-order word
// is a Boolean operation index and whose low-order word is the
// operation code. The 16-bit operation index is a zero-extended 8-bit
// value that represents all possible outcomes resulting from the
// Boolean operation on two parameters (in this case, the pen and
// destination values). For example, the operation indexes for the DPo
// and DPan operations are shown in the following list:
//
// P	D	DPo	DPan
//
// 0	0	0	1
// 0	1	1	1
// 1	0	1	1
// 1	1	1	0
//
// The following list outlines the drawing modes and the Boolean
// operations that they represent:
//
// Raster operation	Boolean operation
//
// R2Black	     0
// R2CopyPen	 P
// R2MaskNotPen	 DPna
// R2MaskPen	 DPa
// R2MaskPenNot	 PDna
// R2MergeNotPen DPno
// R2MergePen	 DPo
// R2MergePenNot PDno
// R2Nop	     D
// R2Not	     Dn
// R2NotCopyPen	 Pn
// R2NotMaskPen	 DPan
// R2NotMergePen DPon
// R2NotXorPen	 DPxn
// R2White	     1
// R2XorPen	     DPx
//
// The zApp Graphics system maps zero to black and 1 to white.

enum BinDrawOp {
	R2Black = R2_BLACK,
	R2NotMergePen = R2_NOTMERGEPEN,
	R2MaskNotPen = R2_MASKNOTPEN,
	R2NotCopyPen = R2_NOTCOPYPEN,
	R2MaskPenNot = R2_MASKPENNOT,
	R2Not = R2_NOT,
	R2XorPen = R2_XORPEN,
	R2NotMaskPen = R2_NOTMASKPEN,
	R2MaskPen = R2_MASKPEN,
	R2NotXorPen = R2_NOTXORPEN,
	R2Nop = R2_NOP,
	R2MergeNotpen = R2_MERGENOTPEN,
	R2CopyPen = R2_COPYPEN,
	R2MergePenNot = R2_MERGEPENNOT,
	R2MergePen = R2_MERGEPEN,
	R2White = R2_WHITE
};


#define RW	0
#define RO	1
#define HIDDEN	2
#define SYSTEM	4
#define DIRS	16
#define ARCHIVE	32
#define DRIVES	0x4000
#define ONLY	0x8000

enum StockIcon {
	Application=32512,
	Asterisk   =32516,
	Exclamation=32515,
	Hand	   =32513,
	Question   =32514
};

#define ST_LEFT		SS_LEFT
#define ST_CENTER	SS_CENTER
#define ST_RIGHT	SS_RIGHT

enum StockBitmap {
	BmpClose= OBM_CLOSE,
	BmpUpArrow= OBM_UPARROW,
	BmpDownArrow= OBM_DNARROW,
	BmpRightArrow= OBM_RGARROW,
	BmpLeftArrow= OBM_LFARROW,
	BmpReduce= OBM_REDUCE,
	BmpZoom= OBM_ZOOM,
	BmpRestore= OBM_RESTORE,
	BmpReduced= OBM_REDUCED,
	BmpZoomed= OBM_ZOOMD,
	BmpRestored= OBM_RESTORED,
	BmpUpArrowD= OBM_UPARROWD,
	BmpDownArrowD= OBM_DNARROWD,
	BmpRightArrowD= OBM_RGARROWD,
	BmpLeftArrowD= OBM_LFARROWD,
	BmpMNarrow= OBM_MNARROW,
	BmpCombo= OBM_COMBO,
	BmpOldClose= OBM_OLD_CLOSE,
	BmpSize= OBM_SIZE,
	BmpOldUpArrow= OBM_OLD_UPARROW,
	BmpOldDownArrow= OBM_OLD_DNARROW,
	BmpOldRightArrow= OBM_OLD_RGARROW,
	BmpOldLeftArrow= OBM_OLD_LFARROW,
	BmpButtonSize= OBM_BTSIZE,
	BmpCheck= OBM_CHECK,
	BmpCheckBoxes= OBM_CHECKBOXES,
	BmpButtonCorners= OBM_BTNCORNERS,
	BmpOldReduce= OBM_OLD_REDUCE,
	BmpOldZoom= OBM_OLD_ZOOM,
	BmpOldRestore= OBM_OLD_RESTORE,
	BmpUpArrowDisabled= OBM_UPARROWI,
	BmpDownArrowDisabled= OBM_DNARROWI,
	BmpRightArrowDisabled= OBM_RGARROWI,
	BmpLeftArrowDisabled= OBM_LFARROWI
};

enum {
    DEVMMOVE=1,DEVSETCURSOR=2,DEVHITTEST=4,DEVMENU=8,
    DEVMOUSEBU=16,DEVMOUSEMOVE=32,DEVACTIVATE=64,
    DEVKILL=128,DEVMOUSEBD=256,DEVFOCUS=512,
    DEVKEY=1024
};
		
#define ZMENU_DROPDOWN MF_POPUP
#define ZMENU_HORZSEP  MF_SEPARATOR
#define ZMENU_VERTSEP  MF_MENUBARBREAK


enum DisplayTech {
	DispPlotter = DT_PLOTTER,
	DispRaster = DT_RASDISPLAY,
	DispRasterPrint = DT_RASPRINTER,
	DispRasterCamera = DT_RASCAMERA,
	DispCharStream = DT_CHARSTREAM,
	DispMetaFile = DT_METAFILE,
	DispDispFile = DT_DISPFILE
};

//
//	Drawing objects
//
//	DrawObject Flags

#define STOCKOBJ	0x0001

enum PolyFill {
	PolyAlternate = ALTERNATE,
	PolyWinding = WINDING
};

enum DispType { NoDisp, WinDisp, BitDisp, PrinterDisp, MetaDisp };


enum MapMode {
    MapPixels	= MM_TEXT,
    MapLoMetric = MM_LOMETRIC,
    MapHiMetric = MM_HIMETRIC,
    MapLoEnglish = MM_LOENGLISH,
    MapHiEnglish = MM_HIENGLISH,
    MapTwips = MM_TWIPS,
    MapIsotropic = MM_ISOTROPIC,
    MapAnisotropic = MM_ANISOTROPIC,
    MapNotSet = 0
};

enum zTextMode {
	ZTEXT_TRANSPARENT=TRANSPARENT,
	ZTEXT_OPAQUE=OPAQUE
};


#endif
