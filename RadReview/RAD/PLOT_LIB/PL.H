/*
This work was supported by the United States Member State Support Program to IAEA Safeguards;
the U.S. Department of Energy, Office of Nonproliferation and National Security, International
Safeguards Division; and the U.S. Department of Energy, Office of Safeguards and Security.

LA-CC-14-040. This software was exported from the United States in accordance with the Export
Administration Regulations. Diversion contrary to U.S. law prohibited.

Copyright 2015, Los Alamos National Security, LLC. This software application and associated
material ("The Software") was prepared by the Los Alamos National Security, LLC. (LANS), under
Contract DE-AC52-06NA25396 with the U.S. Department of Energy (DOE). All rights in the software
application and associated material are reserved by DOE on behalf of the Government and LANS
pursuant to the contract.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or other materials provided
with the distribution.
3. Neither the name of the "Los Alamos National Security, LLC." nor the names of its contributors
may be used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS
NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRAT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
#if ! defined HDR_pl // {
#define HDR_pl

/* ======================================================================= */
/* ================================ pl.h ================================= */
/* ======================================================================= */

/*---------------------------------------------------------------------------
*                         SPECIAL NOTE TO DEVELOPERS
*
*		Information from this file is extracted to create portions of
*		PL_UM.DOC.  Because of this, the appearance of that document
*		is highly dependent on the formatting of information in this file.
*		Thus, it is recommended to remain consistent with existing format
*		conventions in this file.  Please observe the following:
*		o	keep the use of tabs and spaces consistent with existing usage
*			in this file
*		o	set tab width to 4
*		o	use "preserve tabs"
*		o	keep extracted lines shorter than 80 characters
*
*		Also, much of the information in this file is used by Visual
*		Basic programs.  See individual '* Related Files:' discussions
*		for additional information.  For the affected areas, changes in
*		this file will require corresponding changes in the related files.
*--------------------------------------------------------------------------*/

/*+/mod/TOC------------------------------------------------------------------
* pl.h - definitions and prototypes for pl.cpp
*
* Purpose:
*		This header file supplies definitions and prototypes for use with
*		the PlPlotXxx functions in pl.cpp.
*
* Usage:
*		#include <windows.h>
*		#include <pl.h>
*
* Required for:
*		This header file is required for use with PlPlotXxx functions and to
*		supply PL_XXX parameters and structures.
*
* Date     Author		Revision
* -------- ------------	--------
* 03-19-95 R. Cole		created
* 08-22-95 R. Cole		major revision and upgrade
*
*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
* Copyright 1995-97, The Regents Of the University of California.  This
* program was prepared by the Regents of the University of California at
* Los Alamos National Laboratory (the University) under Contract No. W-7405-
* ENG-36 with the U.S. Department of Energy (DOE).  The University has
* certain rights in the program pursuant to the contract and the program
* should not be copied or distributed outside your organization.  All rights
* in the program are reserved by the DOE and the University.  Neither the
* U.S. Government nor the University makes any warranty express or implied,
* or assumes any liability or responsibility for the use of this software.
*
* This software was produced by the Safeguards Science and Technology
* Group (NIS-5).
*--------------------------------------------------------------------------*/

#if !defined WIN32
#define FAR16 _far
#define HUGE __huge
#define HALLOC halloc
#define HFREE hfree
#else
#define FAR16
#define HUGE
#define HALLOC calloc
#define HFREE free
#endif

// The version number is an INT, interpreted as a decimal number of the
// form vvrr.  "vv" is the major version number and "rr" is the revision
// level.  Thus, version 1.00 will be represented as 100.  Don't put
// leading zeros on the version number!!

// Change the text in gaszPlStatusMsg, too!!
// Change the version in PL_DEFS.BAS, too!!
// Change the mismatch comparison in PlVersionCheck, too!!

#define PL_VER 100	// 1.00

/*---------------------------------------------------------------------------
* Related Files:
*		o	PL_DEFS.BAS  contains the VB versions of these constants
*--------------------------------------------------------------------------*/

#define PL_BASE 30000		// obtained from master list in SPW.H

#define PL_CLASS "PlPlot"

// byte offsets for the PlPlot class "windowsExtra" information
#define PL_EXTRA_PPL_CTX 0	// pointer to PL_CTX

#define PL_ID_DIM 20
#define PL_MSG_DIM 100

#define PL_FMT_DFLT "%%dflt%%"
#define PL_FMT_TENTO "tenTo"
#define PL_FMT_AUTO "%%auto%%"

typedef unsigned long ULONG;

/*+/after/TOC----------------------------------------------------------------
*
*
* PL_ENUM - various PL.LIB constants
*
* Purpose:
*		These constants specify:
*		o	axis scaling type
*		o	rendering type (for how to draw points)
*		o	axis/grid type
*		o	GUI event types
*		o	plot mark types
*		o	plot object reference points
*		o	plot object event types
*
* Related Files:
*		o	PL_DEFS.BAS  contains the VB version of this list
*--------------------------------------------------------------------------*/
typedef enum {
	PL_LIN=0,			// axis scaling is linear
	PL_LOG,				// axis scaling is log10

	// These must be in sequence.  PL_POINT must be first.
	// PL_LINE_DASHDOTDOT must be last.
	PL_POINT=10,		// plot data as points
	PL_LINE,			// plot data as lines
	PL_LINE_DASH,		// plot data as dashed line
	PL_LINE_DOT,		// plot data as dotted line
	PL_LINE_DASHDOT,	// plot data as dash-dot line
	PL_LINE_DASHDOTDOT,	// plot data as dash-dot-dot line

	PL_SIDE_LEFT=20,	// left side of plot window or plot area
	PL_SIDE_RIGHT,		// right side of plot window or plot area
	PL_SIDE_TOP,		// top side of plot window or plot area
	PL_SIDE_BOTTOM,		// bottom side of plot window or plot area

	PL_LOST_FOCUS=30,	// plot window lost focus
	PL_GOT_FOCUS,		// plot window got focus
	PL_PAINT,			// plot window is being repainted
	PL_LMOUSE,			// plot window got left mouse click
	PL_CURS_ERASE,		// plot cursor needs to be erased
	PL_CURS_HORIZ,		// plot cursor moves left or right
	PL_CURS_REGION,		// plot cursor moves left or right to new region
	PL_CURS_VERT,		// plot cursor moves up or down
	PL_CURS_AREA,		// plot cursor moves to new plot area in array

	PL_NO_AXIS=40,		// no axis
	PL_LINE_AXIS,		// box axis without tick marks, origin at edge
	PL_TICK_AXIS,		// box axis with tick marks, origin at edge
	PL_GRID_AXIS,		// box axis with solid grid lines, origin at edge
	PL_GRID_DOT_AXIS,	// box axis with dotted grid lines, origin at edge
	PL_GRID_DASH_AXIS,	// box axis with dashed grid lines, origin at edge
	PL_EDGE_HALF_AXIS,	// single line axis, origin at edge
	PL_CENT_HALF_AXIS,	// single line axis, origin center

	// The first few must be in sequence.  PL_SQUARE must be first and
	// PL_CROSS must be last in the group of "plot marks".  Also,
	// PL_SQUARExxx must be first in a series and PL_CIRCLExxx must be last.
	PL_SQUARE=50,		// plot mark square
	PL_TRI_UP,			// plot mark triangle, pointing up
	PL_TRI_DOWN,		// plot mark triangle, pointing down
	PL_DIAMOND,			// plot mark diamond
	PL_CIRCLE,			// plot mark circle
	PL_SQUARE_SOLID,	// plot mark square, filled
	PL_TRI_UP_SOLID,	// plot mark triangle, filled, pointing up
	PL_TRI_DOWN_SOLID,	// plot mark triangle, filled, pointing down
	PL_DIAMOND_SOLID,	// plot mark diamond, filled
	PL_CIRCLE_SOLID,	// plot mark circle, filled
	PL_SQUARE_BOLD,		// plot mark square, bold
	PL_TRI_UP_BOLD,		// plot mark triangle, bold, pointing up
	PL_TRI_DOWN_BOLD,	// plot mark triangle, bold, pointing down
	PL_DIAMOND_BOLD,	// plot mark diamond, bold
	PL_X_BOLD,			// plot mark X, bold
	PL_CROSS_BOLD,		// plot mark cross, bold
	PL_CIRCLE_BOLD,		// plot mark circle, bold
	PL_X,				// plot mark X
	PL_CROSS,			// plot mark cross or plot cursor
	PL_CROSS_TALL,		// tall plot cursor with horizontal crossbar
	PL_CROSS_WIDE,		// wide plot cursor with vertical crossbar
	PL_CROSS_FULL,		// tall and wide plot cursor

	PL_OBJ_TL,			// resize, top left corner
	PL_OBJ_T,			// resize, top side
	PL_OBJ_TR,			// resize, top right corner
	PL_OBJ_R,			// resize, right side
	PL_OBJ_BR,			// resize, bottom right corner
	PL_OBJ_B,			// resize, bottom side
	PL_OBJ_BL,			// resize, bottom left corner
	PL_OBJ_L,			// resize, left side
	PL_OBJ_C,			// move (from center)

	PL_MOUSE_MOVE,		// mouse has moved
	PL_OBJ_EV_DEL,		// PL_OBJ is about to be deleted
	PL_OBJ_EV_EDIT,		// PL_OBJ edit event is about to occur
	PL_OBJ_EV_BEGIN,	//		the edit operation is about to begin
	PL_OBJ_EV_COORD,	//		the edit operation continues, new coordinates
	PL_OBJ_EV_DONE,		//		the edit operation is about to complete
	PL_OBJ_EV_ABORT,	//		the edit operation is about to abort
	PL_OBJ_EV_MOVE,		//		the edit is a move
	PL_OBJ_EV_RESIZE,	//		the edit is a resize
	PL_OBJ_EV_CREATE,	//		the edit is a create
	PL_OBJ_EV_COPY,		//		the edit is a copy
	PL_OBJ_EV_SEL,		// PL_OBJ is about to be selected or deselected

	PL_OBJ_TY_BOX,		// PL_OBJ is a box
	PL_OBJ_TY_LABEL,	// PL_OBJ is a data label
	PL_OBJ_TY_LEGEND,	// PL_OBJ is a legend box

	PL_OP_OBJ_L_F,		// move/resize object left, fine
	PL_OP_OBJ_L_C,		// move/resize object left, coarse
	PL_OP_OBJ_L_A,		// move/resize object left, plot area
	PL_OP_OBJ_R_F,		// move/resize object right, fine
	PL_OP_OBJ_R_C,		// move/resize object right, coarse
	PL_OP_OBJ_R_A,		// move/resize object right, plot area
	PL_OP_OBJ_U_F,		// move/resize object up, fine
	PL_OP_OBJ_U_C,		// move/resize object up, coarse
	PL_OP_OBJ_U_A,		// move/resize object up, plot area
	PL_OP_OBJ_D_F,		// move/resize object down, fine
	PL_OP_OBJ_D_C,		// move/resize object down, coarse
	PL_OP_OBJ_D_A,		// move/resize object down, plot area
	PL_OP_OBJ_SEL_N,	// select next object, forward
	PL_OP_OBJ_SEL_P,	// select next object, backward
	PL_OP_OBJ_DEL,		// delete object
	PL_OP_OBJ_C,		// move mouse pointer to object center
	PL_OP_OBJ_LB,		// move mouse pointer to object left-bottom
	PL_OP_OBJ_RT,		// move mouse pointer to object right-top
	PL_OP_OBJ_ZOOM,		// zoom object to full scale
	PL_OP_OBJ_RESTORE,	// restore zoom
	PL_OP_PCUR_L_F,		// move plot cursor left, fine
	PL_OP_PCUR_L_C,		// move plot cursor left, coarse
	PL_OP_PCUR_L_A,		// move plot cursor left, plot area
	PL_OP_PCUR_L_O,		// move plot cursor left, to next object
	PL_OP_PCUR_R_F,		// move plot cursor right, fine
	PL_OP_PCUR_R_C,		// move plot cursor right, coarse
	PL_OP_PCUR_R_A,		// move plot cursor right, plot area
	PL_OP_PCUR_R_O,		// move plot cursor right, to next object
	PL_OP_PCUR_U,		// move plot cursor up to next trace
	PL_OP_PCUR_U_A,		// move plot cursor up, plot area
	PL_OP_PCUR_D,		// move plot cursor down to next trace
	PL_OP_PCUR_D_A,		// move plot cursor down, plot area
	PL_OP_SCR_U_C,		// scroll thumb up, coarse
	PL_OP_SCR_U_T,		// scroll thumb up, to top
	PL_OP_SCR_D_C,		// scroll thumb down, coarse
	PL_OP_SCR_D_B,		// scroll thumb down, to bottom
	PL_OP_SCR_L_C,		// scroll thumb left, coarse
	PL_OP_SCR_L_L,		// scroll thumb left, to left end
	PL_OP_SCR_R_C,		// scroll thumb right, coarse
	PL_OP_SCR_R_R,		// scroll thumb right, to right end

	PL_UW_RESIZE=1000,
	PL_UW_PAINT,
	PL_UW_LMOUSE,
	PL_UW_MMOUSE,
	PL_UW_RMOUSE,
	PL_UW_MOUSE_UP,
	PL_UW_MOUSE_DOWN,
	PL_UW_MOUSE_DBL_CLICK,
	PL_UW_MOUSE_MOVE,
	PL_UW_FOCUS,
	PL_UW_KEY_DOWN,
	PL_UW_CURSOR_MOVE
} PL_ENUM;
//-  turn off extract

/*+/after/TOC----------------------------------------------------------------
*
*
* PRT_CP - constants for positioning printed text
*
* Purpose:
*		These constants allow specifying:
*		o	the reference point in the bounding box for printed text
*
* Related Files:
*		o	PL_DEFS.BAS  contains the VB version of this list
*--------------------------------------------------------------------------*/
typedef enum {
	PRT_TL=1,	// top left of box is at X,Y
	PRT_TC,		// top center
	PRT_TR,		// top right
	PRT_RC,		// right center
	PRT_BL,		// bottom left
	PRT_BC,		// bottom center
	PRT_BR,		// bottom right
	PRT_LC,		// left center
	PRT_C,		// center
	PRT_LJ,		// left-justified
	PRT_CEN,	// center-justified
	PRT_RJ,		// right-justified
} PRT_CP;
//-  turn off extract

typedef struct pl_ctx_ * PPL_CTX;

/*+/after/TOC----------------------------------------------------------------
*
*
* PL_AXIS - plot axis control and status information
*
* Purpose:
*		This structure is used to control plotting for an axis.  It contains
*		information about scaling, zooming, and scrolling.  It also has
*		information about axis labeling, annotation, axis type, and
*		axis marking.
*
*		Since this structure is used for both X and Y axes, there is
*		some potential confusion.  In particular, the discussion (and the
*		structure member names) are in terms of the Y axis.  When this
*		structure is used for the X axis, the reader should mentally
*		translate "bottom" to "left" and "top" to "right".  Bottom and
*		top are used in the discussion rather than minimum and maximum in
*		order to avoid the confusion that might arise when the data is
*		plotted with "reverse polarity".
*
* Coordinate systems
*		Several different coordinate systems are used in plotting.
*
*		DATA VALUES correspond to the set of user data points.  These
*		members have 'dDVal' as their prefix.
*
*		DISPLAY VALUES correspond to data values.  If an axis has linear
*		scaling, the set of display values is exactly the same as the
*		set of data values.  For a log axis, the set of display values
*		is the set of log10 of the data values.  These members have 'dLVal'
*		as their prefix.
*
*		PIXEL POSITIONS correspond to the pixels on the display medium.
*
* Zoom and scroll
*		Zooming is controlled by a zoom factor (fZoom), which must be
*		greater than or equal to one.
*
*		Scrollbar values range from 0 to 1000.  The scrollbar value is
*		converted to a range of 0.0 to 1.0 and stored in fScroll.  For
*		the X axis, 0 corresponds to the left; 0 is the bottom of the
*		Y axis.
*
*		The anchor point is used to control positioning after a zoom
*		operation.  fAnchorFrac specifies where, along the visible part
*		of the axis, the anchor point is fixed; the value will be between
*		0 and 1, inclusive.  dDValAnchor specifies the data value that
*		is to appear at the anchor point.  When the axis zoom factor
*		changes, fScroll is adjusted so that dAnchorVal stays in the
*		same relative position on the axis.
*
* Scaling
*		The range for display values is dValB to dValT.  For a linear axis,
*		this corresponds to the data values at the axis endpoints.
*
*		For a log10 axis, though, dValB is log10(bottom_data_value), and
*		similarly for dValT.  This implementation expressly allows data
*		values of 0. for the bottom of a log10 axis, even though log10(0.)
*		is undefined.  (This is done in order to accomodate the plotting
*		of NDA data.)  If the bottom data value is 0., then dValB is
*		set to log10(.5 * bottom_nonzero_data_value).
*
*		Pixel values range from pxB to pxT.  (Under Windows, pxT is a
*		smaller number than pxB for the Y axis, since the origin is in the
*		upper left.  Some parts of the code make use of this "insider
*		information".)
*
*		pxB and pxT will be inset from the borders of the window in the
*		following cases:
*		o	the plot has an active/inactive border
*		o	the axis has a title
*		o	the axis has annotations
*		o	this is the Y axis and the plot has a top title
*		o	this is the Y axis and the plot has a legend
*
*		dPxPerLVal is used to convert from display value to pixel position.
*			dPxPerLVal = fZoom * (pxT - pxB + 1) / (dValT - dValB)
*
*		The display value range for a linear axis is (dValT-dValB+1)/fZoom.
*		The bottommost value that can be displayed ranges from dValB to
*		(dValT-dValB+1) - (dValT-dValB+1)/fZoom.  The actual bottommost
*		value is determined by fScroll:
*			dValBS = dValB + fScroll *
*					((dValT-dValB+1) - (dValT-dValB+1)/fZoom)
*		The actual topmost value is:
*			dValTS = dValBS + (dValT - dValB + 1) / fZoom - 1
*
*		The final axis coordinate, in pixels, for a display value (dVal)
*		is given by the following equation:
*			PX = pxB + (dVal - dValBS) * dPxPerLVal
*
* Related Files:
*		o	PL_DEFS.BAS  contains the VB version of this structure
*--------------------------------------------------------------------------*/
typedef struct {
	int		eStyle;			// axis style
	int		eScaleType;		// axis scaling type: PL_LIN or PL_LOG
	int		nMajor;			// # of major divisions for axis, or 0 or -1
	int		nMinor;			// # of minor axis divisions within major
	float	fArrayPosFrac;	// area X or Y loc--frac of window, or -1
	float	fArraySizeFrac;	// area ht or wid--frac of window, or -1
	int		pxAreaB;		// bottom pixel for area, annotations, titles
	int		pxAreaT;		// top pixel for area, annotations, titles
	int		pxB;			// bottom pixel for visible part of axis
	int		pxT;			// top pixel for visible part of axis
	int		bIntAxis;		// 1 if axis data has only integral values
	int		bLogZero;		// 1 for special log axis with zero min value
	double	dLogZero;		// value to use for log10(0.0)
	double	dDValB_minNon0;	// minimum non-zero data value
	double	dDValB;			// bottom data value for axis
	double	dDValT;			// top data value for axis
	double	dLValB;			// bottom data (log) value for axis
	double	dLValT;			// top data (log) value for axis
	double	dLValBS;		// bot data (log) value for vis. part of axis
	double	dLValTS;		// top data (log) value for vis. part of axis
	double	dPxPerLVal;		// pixels/(log)val; includes effects of zoom
	float	fZoom;			// zoom factor, >= 1.
	int		bNoZoom;		// 1 if axis is not to be zoomed
	float	fScroll;		// scroll position; 0 <= pos <= 1
	float	fAnchorFrac;	// anchor point, fraction (0 to 1) along axis
	double	dDValAnchor;	// anchor point, data value
	float	fAnnotDeg;		// annotation orientation, in degrees
	int		iAnnotNChar;	// # of characters to leave room for on plot
	float	fAnnotPts;		// text height for annotations (points), or 0.
	int		nTitleLinesB;	// # of title lines at plot area bottom
	int		nTitleLinesT;	// # of title lines at plot area top
	float	fTitlePts;		// text height for title (points), or 0.
	const char * (*pAnnotFn)(PPL_CTX, void *, double);
	void	*pAnnotArg;		// arg for user call
	char	szAnnotFont[PL_MSG_DIM]; // font name for axis annotations
	char	szAnnotFmt[PL_MSG_DIM]; // C format for printing annotations
	char	szTitleFont[PL_MSG_DIM]; // font name for axis title
} PL_AXIS;
//-  turn off extract

typedef struct pl_obj_ PL_OBJ;
typedef struct pl_legend_entry_ PL_LEGEND_ENTRY;

/*+/after/TOC----------------------------------------------------------------
*
*
* PL_BOX - 'box object' properties
*
*--------------------------------------------------------------------------*/
typedef struct pl_box_side_ {
	int		bValOK;			// 1 if dVal is in range for the axis data
	int		iRowCol;		// plot area row or column containing side
	double	dVal;			// data value for side
} PL_BOX_SIDE;

typedef struct pl_box_props_ {
	// "operator interaction" properties
	int		bNoEdit;		// 1 if box can't be edited
	int		bNoMove;		// 1 if box can't be moved
	int		bNoResize;		// 1 if box can't be resized
	int		bNoDelete;		// 1 if box can't be deleted
	// "visual" properties
	int		bHide;			// 1 if box is not to be shown
	int		bRounded;		// 1 if box has rounded corners
	int		eBorderType;	// line style for box border, PL_LINE_xxx
	float	fBorderPts;		// drawing thickness for box border, in points
	int		bFillBox;		// 1 if box (when not selected) is shown filled
	COLORREF rgbBox;		// color for drawing (and filling) box
	int		bShowFullHt;	// 1 to draw box the height of the data area
	// "program interaction" properties
	double	dUserVal;		// user value
	void	*pUserStruct;	// user pointer
	char	szID[PL_ID_DIM];// user text
} PL_BOX_PROPS;

typedef struct pl_box_ {
	PL_OBJ	*pNext;			// pointer to next in PL_OBJ list
	PL_OBJ	*pPrev;			// pointer to previous in PL_OBJ list
	int		eObjType;		// type of object--always PL_OBJ_TY_BOX
	PL_BOX_PROPS props;		// box properties
	PL_BOX_SIDE aSides[4];	// the 4 sides, left,bottom,right,top
} PL_BOX;
//-  turn off extract

/*---------------------------------------------------------------------------
* PL_COORD - X,Y coordinate for a point
*
*	For a discussion of all this complexity, see PlBboxPaint_box.
*
*	.bEdgeVis  1 indicates that the edge is visible.  For the ordinary
*					case, this is synonymous with "dVal is visible".
*					For .bShowFullHt boxes, this member indicates that
*					the 'phantom' top or bottom edge is visible.
*	.bValOK  1 indicates that the X|Y data value is within the range
*					of the data for the X|Y axis.
*	.iVisPx  is the pixel coordinate for the visible edge of the
*					rectangle whose edge is represented by dVal.
*	.iDrawPx  is the pixel coordinate to use in drawing the edge
*					of the rectangle.  If the actual edge is hidden, then
*					this member is 'expanded' beyond .iVisPx so that
*					if the box is an RRect its 'shoulders' will be hidden.
*	.iClipPx  is the pixel coordinate to use in constructing a clip
*					region for drawing the edge.
*	.dVirtPx  is the pixel coordinate corresponding to dVal,
*					based strictly on the DVal to pixel mapping for the
*					axis.  If the axis is zoomed, this value is likely
*					to be far outside the data area.
*--------------------------------------------------------------------------*/
typedef struct pl_coord_half {
	int		bValOK;			// 1 if dVal is valid
	int		bEdgeVis;		// 1 if edge is visible
	int		iRowCol;		// plot area row or column containing dVal
	int		iVisPx;			// visible pixel value for coordinate
	int		iDrawPx;		// pixel value to use for drawing item
	int		iClipPx;		// pixel value to use to clip iDrawPx
	double	dVirtPx;		// "virtual" pixel value for coordinate
	double	dVal;			// data value
} PL_COORD_HALF;

typedef struct pl_coord_ {
	PL_COORD_HALF x;		// X half of coordinate
	PL_COORD_HALF y;		// Y half of coordinate
} PL_COORD;

/*+/after/TOC----------------------------------------------------------------
*
*
* PL_LABEL - 'label object' properties
*
* Terminology:
*		o	'near end' refers to the end of the line closest to the
*			data point.
*		o	'far end' refers to the end of the line farthest from the
*			data point.
*--------------------------------------------------------------------------*/
typedef struct pl_label_props_ {
	int		bNoEdit;		// 1 if label can't be edited
	int		bNoMove;		// 1 if label can't be moved
	int		bNoDelete;		// 1 if label can't be deleted
	int		bSnapToTop;		// 1 to snap label to top of data area
	int		bSnapToBottom;	// 1 to snap label to bottom of data area
	int		bLineHide;		// 1 to hide the line between datum and text
	int		bBoxHide;		// 1 to hide the box around the text
	int		bTextBold;		// 1 to use bold text
	int		bTextVert;		// 1 to draw text vertical, 0 for horiz
	float	fLineFarX_pts;	// datum to far end of line, X distance, pts
	float	fLineFarY_pts;	// datum to far end of line, Y distance, pts
	float	fLineMeet_frac;	// line meets at horiz frac of text length
	float	fFontSize_pts;	// font size, printer points
	COLORREF rgbLabel;		// color for line and text
	char	szFont[PL_MSG_DIM];// font name for text
} PL_LABEL_PROPS;

typedef struct pl_label_ {
	PL_OBJ	*pNext;			// pointer to next in PL_OBJ list
	PL_OBJ	*pPrev;			// pointer to previous in PL_OBJ list
	int		eObjType;		// type of object--always PL_OBJ_TY_LABEL
	PL_LABEL_PROPS props;	// label properties
	int		iRow;			// plot area row containing label
	int		iCol;			// plot area column containing label
	double	dXVal;			// X data value that label is associated with
	double	dYVal;			// Y data value that label is associated with
	char	szText[PL_MSG_DIM];// text for the label
} PL_LABEL;
//-  turn off extract

/*+/after/TOC----------------------------------------------------------------
*
*
* PL_LEGEND - 'legend object' properties
*
*--------------------------------------------------------------------------*/
typedef struct pl_legend_entry_ {
	PL_LEGEND_ENTRY *pNext;	// pointer to next in PL_LEGEND_ENTRY list
	PL_LEGEND_ENTRY *pPrev;	// pointer to previous in PL_LEGEND_ENTRY list
	int		iTextHt_px;		// height of text, pixels
	int		iTextWid_px;	// width of text, pixels
	int		eDrawType;		// "continuous" drawing type--PL_POINT,
							// PL_LINE_XXX, or 0
	int		eMarkType;		// "intermittent" drawing type (see PlDrawMark)
	float	fDrawPts;		// thickness for eDrawType, printer points
	float	fMarkPts;		// size for eMarkType, printer points
	COLORREF rgbData;		// color for drawing points, lines, and marks
	COLORREF rgbText;		// color for the text
	char	szText[PL_MSG_DIM];// text for the entry
} PL_LEGEND_ENTRY;

typedef struct pl_legend_props_ {
	// "operator interaction" properties
	int		bNoEdit;		// 1 if legend can't be edited
	int		bNoMove;		// 1 if legend can't be moved
	int		bNoDelete;		// 1 if legend can't be deleted
	// "visual" properties
	int		bHide;			// 1 to hide the entire legend
	int		bBorderHide;	// 1 to hide the border
	int		bFill;			// 1 if legend (when not selected) is shown filled
	int		bTitleTop;		// 1 if title is at top of box
	int		bTitleBold;		// 1 to use bold text for title
	int		bEntryTextLeft;	// 1(0) if entry order is text draw (draw text)
	float	fBorderPts;		// drawing thickness for legend border, in points
	float	fFontSize_pts;	// title and entry text font size, printer points
	COLORREF rgbBox;		// color for border and fill
	COLORREF rgbTitle;		// color for title
	char	szTitle[PL_MSG_DIM];// title for the legend
	char	szFont[PL_MSG_DIM];// font name for title and entry text
} PL_LEGEND_PROPS;

typedef struct pl_legend_ {
	PL_OBJ	*pNext;			// pointer to next in PL_OBJ list
	PL_OBJ	*pPrev;			// pointer to previous in PL_OBJ list
	int		eObjType;		// type of object--always PL_OBJ_TY_LEGEND
	PL_LEGEND_PROPS props;	// legend properties
	PL_LEGEND_ENTRY *pHead;	// pointer to head of PL_LEGEND_ENTRY list
	PL_LEGEND_ENTRY *pTail;	// pointer to tail of PL_LEGEND_ENTRY list
	int		eRefPt;			// 'reference point' on border, PRT_xx
	int		iRow;			// plot area row containing legend, or -1
	int		iCol;			// plot area column containing legend, or -1
	float	fXFrac;			// X position, frac of plot window or data area
	float	fYFrac;			// Y position, frac of plot window or data area
} PL_LEGEND;
//-  turn off extract

/*---------------------------------------------------------------------------
*
*
* PL_MOUSE - mouse operations structure
*
* Notes:
* 1.	Two members indicate the mouse pointer's logical position in
*		the plot window:
*
*		v  pointer position  v            .bInCurrArea    .bInOtherArea
*		outside any data area                   0               0
*		in currently selected data area         1               0
*		in non-selected data area               0               1
* 2.	Some pMouse items are "disabled" in some cases:
*		o	if pObjCtx->mode.bEditMode is 0:
*			-	bHitNonSel  is always 0
*			-	bHitSelected  is always 0
*			-	pHitObj  is always NULL
*
* Related Files:
*		o	none
*--------------------------------------------------------------------------*/
typedef struct pl_mouse_ {
	int		bLeftDown;		// 1 if left mouse button is down
	int		bLeftLast;		// 1 if last mouse button down was the left
	int		bMiddleDown;	// 1 if middle mouse button is down
	int		bMiddleLast;	// 1 if last mouse button down was the middle
	int		bRightDown;		// 1 if right mouse button is down
	int		bRightLast;		// 1 if last mouse button down was the right
	int		bShift;			// 1 if shift key was down at WM_...BUTTONDOWN
	int		bCtrl;			// 1 if ctrl key was down at WM_...BUTTONDOWN
	PL_COORD coDown;		// pointer position at WM_...BUTTONDOWN
	PL_COORD coCurr;		// pointer position for this event
	int		bLeftClick;		// 1 if this is being classified as a click
	int		bMiddleClick;	// 1 if this is being classified as a click
	int		bRightClick;	// 1 if this is being classified as a click
	int		bTossNextClick;	// 1 to throw away next WM_...BUTTONUP
	int		bPlotCursorClick;// 1 to use click for plot cursor
	int		bInPlotWin;		// 1 if mouse is anywhere in plot window
	int		bInOtherArea;	// 1 if mouse in a data area (Note 1)
	int		bInCurrArea;	// 1 if mouse in current data area (Note 1)
	int		bHitNonSel;		// 1 if mouse hit a non-selected PL_OBJ (Note 2)
	int		bHitSelected;	// 1 if mouse hit a selected PL_OBJ (Note 2)
	PL_OBJ	*pHitObj;		// pointer to PL_OBJ that mouse hit, or NULL
	int		iHitType;		// if pHitObj != NULL, hit type: PL_OBJ_TL, etc.

	int		bUseNOTPointer;	// 1 if "NOT!" pointer is currently appropriate
	HCURSOR	hPointerCurr;	// current pointer
	HCURSOR	hPointerNew;	// proposed new pointer
	HCURSOR	hPointerEdit;	// pointer that's being used for create/edit
	HCURSOR	hPointerHit;	// pointer corresponding to iHitType
	HCURSOR	hPointerPlus;	// can move the plot cursor
	HCURSOR	hPointerCreate;	// can create a new object
	HCURSOR	hPointerSelect;	// can select an existing object
	HCURSOR	hPointerSize;	// resize pointer
	HCURSOR	hPointerMove;	// move pointer
	HCURSOR	hPointerArrow;	// arrow pointer
	HCURSOR	hPointerNOT;
} PL_MOUSE;

typedef struct pl_obj_ {
	PL_OBJ	*pNext;			// pointer to next in PL_OBJ list
	PL_OBJ	*pPrev;			// pointer to previous in PL_OBJ list
	int		eObjType;		// type of object--one of PL_OBJ_TY_xxx
} PL_OBJ;

/*+/after/TOC----------------------------------------------------------------
*
*
* PL_OBJ_CTX - plot object operations structure
*
*--------------------------------------------------------------------------*/

#define PL_OBJ_HIT_PX 3		// how close (+/- #pixels) for 'hit'

typedef struct pl_obj_mode_ {
	int		eCreateObjType;	// current 'create type', PL_OBJ_TY_xxx
	int		bEditMode;		// 1 to allow editing objects
	// The following bEnbXxx are ignored if bEnbEdit is 0.
	int		bEnbBoxNew;		// 1 allows creating PL_BOXs
	int		bEnbBoxSpan;	// 1 allows PL_BOX spanning plot areas
	int		bEnbBoxMany;	// 1 allows multiple PL_BOXs
	int		bEnbLabelNew;	// 1 allows creating PL_LABELSs
	// The following are 'global' overrides.
	int		bEnbMove;		// 1 allows moving even if props.bNoMove is 0
	int		bEnbResize;		// 1 allows resizing even if props.bNoResize is 0
	int		bEnbDelete;		// 1 allows deleting even if props.bNoDelete is 0
	// The following are used when the keyboard is used to move/resize
	// an object.
	int		bValuesAsPx;	// 1 (or 0) if values are data (or pixels)
	double	dXFine;			// size of "fine" horizontal move
	double	dXCoarse;		// size of "coarse" horizontal move
	double	dYFine;			// size of "fine" vertical move
	double	dYCoarse;		// size of "coarse" vertical move
} PL_OBJ_MODE;

typedef struct pl_obj_ctx_ {
	PL_OBJ_MODE mode;		// plot object operating mode flags
	PL_OBJ	*pSelObj;		// pointer to selected object, or NULL
	int		nObjects;		// number of existing objects
	int		nBoxes;			// number of PL_BOXs
	PL_OBJ	*pHead;			// head of object list
	PL_OBJ	*pTail;			// tail of object list

	int		bCreateActive;	// 1 if create is active
	int		bCreateIsCopy;	// 1 if the create is a "copy"
	int		bEditActive;	// 1 if move/resize is active
	int		bKbEditActive;	// 1 if keyboard-based move/resize is active
	PL_COORD coEditLBCurr;	// left bottom dotted "rubber band", current
	PL_COORD coEditRTCurr;	// right top dotted "rubber band", current
	PL_OBJ	*pEditObj;		// object corresponding to "rubber band" object

	int		bRestoreOK;		// 1 if PlObjBoxZoom has been called
	float	fXZoom;			// X zoom factor prior to PlObjBoxZoom
	float	fYZoom;			// Y zoom factor prior to PlObjBoxZoom
	float	fXScroll;		// X scroll position prior to PlObjBoxZoom
	float	fYScroll;		// Y scroll position prior to PlObjBoxZoom

	PL_COORD coMouseBegin;	// pointer coordinate when create/edit began
	int		bEditVisible;	// 1 if "rubber band" box is visible
	PL_COORD coEditLBBeg;	// LB dotted "rubber band" at edit start
	PL_COORD coEditRTBeg;	// RT dotted "rubber band" at edit start
	int		bShiftBegin;	// 1 if shift key was down at edit start
	int		bCtrlBegin;		// 1 if ctrl key was down at edit start
	int		bEditIsMove;	// 1 if the edit is a "move"
	int		bEditIsResize;	// 1 if the edit is a "resize"
	int		bMoveLeft;		// 1 if "left" side is moving
	int		bMoveBottom;	// 1 if "bottom" side is moving
	int		bMoveTop;		// 1 if "top" side is moving
	int		bMoveRight;		// 1 if "right" side is moving

	PL_BOX_PROPS boxDflt;	// default PL_BOX properties
	PL_LABEL_PROPS labelDflt;// default PL_LABEL properties
	PL_LEGEND_PROPS legendDflt;// default PL_LEGEND properties

	PL_OBJ	*pDialogObj;
	int		bObjEditDone;
	HWND	hwObjDialog;
	HWND	hwObjTextBox;

	HDC		hDC;
	HPEN	hPen;
	HPEN	hPenOld;
	COLORREF rgbBkgOld;
	COLORREF rgbTextOld;
} PL_OBJ_CTX;
//-  turn off extract

/*+/after/TOC----------------------------------------------------------------
*
*
* PL_CTX - plotting control structure
*
* Notes:
* 1.	The .hDC member deserves careful attention.  When any of the
*		PlPlotXxx(pPlot,...) functions are called from a WndProc in
*		response to a WM_PAINT message, the following code segment should
*		be carefully followed:
*
*			hDC = BeginPaint(hWnd, &ps);
*			pPlot->hDC = hDC;
*			PlPlot(pPlot);
*			pPlot->hDC = 0;
*			EndPaint(hWnd, &ps);
*
* Related Files:
*		o	PL_DEFS.BAS  contains the VB version of this structure
*--------------------------------------------------------------------------*/
typedef struct pl_ctx_ {
	HWND	hwPlot;			// handle for plot window
	HDC		hDC;			// DC handle; see special note above
	HGLOBAL	hPD;			// handle for PRINTDLG structure, or 0
	void	*pPd;			// pointer to PRINTDLG structure, or NULL
	HRGN	hRgnPaint;		// handle to WM_PAINT update region, or 0
	RECT	rectPlot;		// region to use for plotting
	int		bPrinter;		// this plot context is open to printer
	int		bPrintCancel;	// 1 if operator wants to cancel printout
	HWND	hwPrintCancel;	// handle for print cancel "dialog"
	int		bPrinterColor;	// 1 if printer has color capability
	char	*szPrtName;		// printer name
	char	*szPrtPort;		// printer port
	char	*szPrtFile;		// printer file
	char	*szPrtJob;		// printer job
	int		bScrollBars;	// plot window has scroll bar capability
	int		bZoomChanged;	// zoom factors just changed
	int		bBorder;		// draw active/inactive border inside window
	int		bNeedScale;		// 1 if PlWinScale needs to be called
	float	fTwipsPerPixX;	// twips per pixel along X axis
	float	fTwipsPerPixY;	// twips per pixel along Y axis
	int		bCursorVisible;	// plot cursor is visible
	int		iCursorRow;		// axis array row containing cursor
	int		iCursorCol;		// axis array column containing cursor
	double	dCursorX;		// X data coordinate of cursor
	double	dCursorY;		// Y data coordinate of cursor

	PL_AXIS	*pXAxis;		// pointer to current X axis structure
	PL_AXIS	*pYAxis;		// pointer to current Y axis structure
	PL_AXIS	xAxis;			// X axis structure
	PL_AXIS	yAxis;			// Y axis structure
	PL_AXIS	*paXAxis;		// pointer to 'array' of X axis structures
	PL_AXIS	*paYAxis;		// pointer to 'array' of Y axis structures
							// iEl = row + col * pPlot->nAxisRows;
	int		nAxisRows;		// number of axis array rows
	float	fVertPts;		// spacing between rows, in points
	int		nAxisCols;		// number of axis array columns
	float	fHorizPts;		// spacing between columns, in points
	int		iAxisRow;		// current axis array row
	int		iAxisCol;		// current axis array column
	float	fTitlePts;		// font size in points for plot title, or 0.
	int		nTitleLinesLeft;// # of title lines at window left
	int		nTitleLinesRight;// # of title lines at window right
	int		nTitleLinesBottom;// # of title lines at window bottom
	int		nTitleLinesTop;	// # of title lines at window top

	int		bEnbCursor;		// 1 if cursor operations are enabled
	PL_OBJ_CTX *pObjCtx;	// pointer to plot object context structure
	PL_MOUSE *pMouse;		// pointer to mouse-handling structure

	// user cursor callback function
	void (*pCursorFn)(PPL_CTX, void *, int, double, double);
	void	*pCursorArg;	// arg for pCursorFn call
	// user pre-processing event callback function
	LRESULT (*pEventPreFn)(PPL_CTX, void *,
				HWND, UINT, WPARAM, LPARAM, int);
	void	*pEventPreArg;	// arg for pEventPreFn call
	// user post-processing event callback function
	LRESULT (*pEventPostFn)(PPL_CTX, void *,
				HWND, UINT, WPARAM, LPARAM, int);
	void	*pEventPostArg;	// arg for pEventPostFn call
	// user plot object callback function
	int (*pObjectFn)(PPL_CTX, void *, PL_OBJ_CTX *, PL_OBJ *, int, double, double);
	void	*pObjectArg;		// arg for pObjectFn call
	// user plotting callback function
	void (*pPlotFn)(PPL_CTX, void *);
	void	*pPlotArg;		// arg for pPlotFn call
	int		inpErr;			// input check number, when input check fails
	char	*pszFnName;		// function name, when input check fails
	char	szErrorMsg[PL_MSG_DIM]; // buffer for library error messages
	char	szTitleFont[PL_MSG_DIM]; // font name for plot title
	char	szPrintOpen[4];	// "prt" if PlPrtOpenXxx has been called
	char	szIntInit[4];	// "yes" if internal PL.LIB init has been done
} PL_CTX;
//-  turn off extract

/*+/TOC----------------------------------------------------------------------
*
*
* RGB colors - color codes for plotting
*
* Purpose:
*		These constants are used for specifying the colors for plotting
*		spectra and ROIs.  The names are self explanatory.
*
*		WHITE
*		GRAY
*		GRAY_DARK
*		BLACK
*		RED
*		RED_DARK
*		YELLOW
*		GOLD
*		GREEN
*		GREEN_DARK
*		CYAN
*		BLUE
*		BLUE_DARK
*		BLUE_GREEN
*		MAGENTA
*		MAGENTA_DARK
*
* Related Files:
*		o	PL_DEFS.BAS  contains the VB version of portions of this list
*--------------------------------------------------------------------------*/
#ifndef BLACK
#define BLACK		0x000000
#endif
#ifndef BLUE
#define BLUE		0xFF0000
#endif
#ifndef BLUE_DARK
#define BLUE_DARK	0x800000
#endif
#ifndef BLUE_GREEN
#define BLUE_GREEN		0x808000
#endif
#ifndef CYAN
#define CYAN			0xFFFF00
#endif
#ifndef GRAY
#define GRAY			0xC0C0C0
#endif
#ifndef GRAY_DARK
#define GRAY_DARK		0x808080
#endif
#ifndef GREEN
#define GREEN			0x00FF00
#endif
#ifndef GREEN_DARK
#define GREEN_DARK		0x008000
#endif
#ifndef MAGENTA
#define MAGENTA			0xFF00FF
#endif
#ifndef MAGENTA_DARK
#define MAGENTA_DARK	0x800080
#endif
#ifndef RED
#define RED				0x0000FF
#endif
#ifndef RED_DARK
#define RED_DARK		0x000080
#endif
#ifndef WHITE
#define WHITE			0xFFFFFF
#endif
#ifndef YELLOW
#define YELLOW			0x00FFFF
#endif
#ifndef GOLD
#define GOLD			0x008080
#endif
/*+/after/TOC----------------------------------------------------------------
*
*
* PL_XXX - status codes returned by PlXxx functions
*
* Purpose:
*		These codes provide the primary means for a program to check
*		whether an error occurred in calling an PlXxx function.
*
*		In order to obtain "human readable" messages, use PlErrorMessage.
*
* Related Files:
*		o	PL_DEFS.BAS  contains the VB version of portions of this list
*--------------------------------------------------------------------------*/
typedef enum {
	PL_OK=PL_BASE,			// operation succeeded
	PL_PRT_CANCEL,			// user canceled an operation
	PL_FILE,				// user selected 'Print to file'
	PL_VERSION_MISMATCH,	// PL.LIB and PL.H version mismatch
	PL_BAD_INPUT,			// a function input check failed
	PL_FILE_IO,				// file I/O error
	PL_MALLOC_FAIL,			// couldn't malloc memory
	PL_GDI_FAIL,			// error in Windows GDI call
	PL_VETOED,				// program vetoed an operation
	PL_GEOM_CHANGED,		// geometry was modified on object create
	PL_FAIL,				// internal PL.LIB failure
	PL_DIALOG_BUSY,			// a PL.LIB dialog box is busy

	PL_ILLEGAL				// 1 past last legal error code
} PL_STATUS;
//-  turn off extract

/*  KG  */

extern const char * const gaszPlStatusMsg[];

//extern const char * FAR16 const gaszPlStatusMsg[];

//#if defined PL_GLOBALS // {
//const char * FAR16 const gaszPlStatusMsg[]={
//extern char * FAR16 const gaszPlStatusMsg[]={
	/* PL_OK             */ //"PL.LIB: succeeded",
	/* PL_PRT_CANCEL     */ //"PL.LIB: operator canceled printout",
	/* PL_FILE           */ //"PL.LIB: user selected 'Print to file'",
	/* PL_VERSION_MISMATCH */ //"PL.LIB: incompatible versions: PL.LIB is 1.00",
	/* PL_BAD_INPUT      */ //"PL.LIB: input check failed",
	/* PL_FILE_IO        */ //"PL.LIB: file IO error",
	/* PL_MALLOC_FAIL    */ //"PL.LIB: couldn't malloc memory",
	/* PL_GDI_FAIL       */ //"PL.LIB: error in Windows GDI call",
	/* PL_VETOED         */ //"PL.LIB: program vetoed an operation",
	/* PL_GEOM_CHANGED   */ //"PL.LIB: geometry was modified",
	/* PL_FAIL           */ //"PL.LIB: internal PL.LIB failure",
	/* PL_DIALOG_BUSY    */ //"PL.LIB: a PL.LIB dialog box is busy",

	/* PL_ILLEGAL        */ //"PL.LIB: unknown status code"
//};
//#else // }  {
//extern const char * FAR16 const gaszPlStatusMsg[];
//#endif // }

/*  KG  */

#define PL_MAX(a, b) ((a) >= (b) ? (a) : (b))
#define PL_MIN(a, b) ((a) < (b) ? (a) : (b))
#define PL_ABS(a) ((a) < 0 ? -(a) : (a))

#define PL_INP_CHK(inpNum, bExpr, gotoLabel) \
	{if (bExpr) {inpErr=inpNum; goto gotoLabel;}}
#define PL_IF_INP_ERR(szName) \
	{if (inpErr) { \
		retStat = PL_BAD_INPUT; \
		if (pPlot != NULL) { \
			pPlot->inpErr = inpErr; \
			pPlot->pszFnName = szName; \
		} \
	}}
#define PL_XPTS_TO_XPIX(fPts) (int)ceil(20.F * (fPts) / pPlot->fTwipsPerPixX)
#define PL_YPTS_TO_YPIX(fPts) (int)ceil(20.F * (fPts) / pPlot->fTwipsPerPixY)
#define PL_PTS_TO_PIX(fPts) (int)ceil(20.F * (fPts) / \
		(.5F * (pPlot->fTwipsPerPixX + pPlot->fTwipsPerPixY)))
// PL_WIN_CVT are available as macros because they are so heavily used.
// The macro form saves a function call, which is a considerable boost
// for performance.
#define PL_WIN_CVT_X_VAL_TO_X_PIX(dDVal, dPix) { \
	if (pPlot->pXAxis->dPxPerLVal == 0.) \
		dPix = 0.;  \
	else { \
		double	dLTemp; \
		if (pPlot->pXAxis->eScaleType == PL_LIN) \
			dLTemp = dDVal; \
		else { \
			if (dDVal <= 0.) dLTemp = pPlot->pXAxis->dLogZero; \
			else dLTemp = log10((double)dDVal); \
		} \
		dPix = (double)pPlot->pXAxis->pxB + ( \
			(dLTemp - pPlot->pXAxis->dLValBS) * \
			pPlot->pXAxis->dPxPerLVal  ); \
	} }
#define PL_WIN_CVT_Y_VAL_TO_Y_PIX(dDVal, dPix) { \
	if (pPlot->pYAxis->dPxPerLVal == 0.) \
		dPix = 0.;  \
	else { \
		double	dLTemp; \
		if (pPlot->pYAxis->eScaleType == PL_LIN) \
			dLTemp = dDVal; \
		else { \
			if (dDVal <= 0.) dLTemp = pPlot->pYAxis->dLogZero; \
			else dLTemp = log10((double)dDVal); \
		} \
		dPix = (double)pPlot->pYAxis->pxB + ( \
			(dLTemp - pPlot->pYAxis->dLValBS) * \
			pPlot->pYAxis->dPxPerLVal  ); \
	} }

/*+/internal/TOC-------------------------------------------------------------
* prototypes
*
* Related Files:
*		o	PL_DEFS.BAS  contains VB versions of some of these prototypes
*--------------------------------------------------------------------------*/

#if defined __cplusplus // {

int PlAreaRangeSetXYVectors(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	const short HUGE *psXVec,// I X data vector
	const short HUGE *psYVec);// I Y data vector

int PlAreaRangeSetXYVectors(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	const long HUGE *plXVec,// I X data vector
	const long HUGE *plYVec);// I Y data vector

int PlAreaRangeSetXYVectors(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	const float HUGE *pfXVec,// I X data vector
	const float HUGE *pfYVec);// I Y data vector

int PlAreaRangeSetXYVectors(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	const double HUGE *pdXVec,// I X data vector
	const double HUGE *pdYVec);// I Y data vector

int PlAreaRangeSetXYVectors(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	const ULONG HUGE *pulXVec,// I X data vector
	const ULONG HUGE *pulYVec);// I Y data vector

int PlAreaRangeSetYVector(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const short HUGE *psYVec);// I Y data vector

int PlAreaRangeSetYVector(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const long HUGE *plYVec);// I Y data vector

int PlAreaRangeSetYVector(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const float HUGE *pfYVec);// I Y data vector

int PlAreaRangeSetYVector(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const double HUGE *pdYVec);// I Y data vector

int PlAreaRangeSetYVector(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const ULONG HUGE *pulYVec);// I Y data vector

int PlDrawXYVectors(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	const short HUGE *psXVec,// I X data vector
	const short HUGE *psYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

int PlDrawXYVectors(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	const long HUGE *plXVec,// I X data vector
	const long HUGE *plYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

int PlDrawXYVectors(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	const float HUGE *pfXVec,// I X data vector
	const float HUGE *pfYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

int PlDrawXYVectors(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	const double HUGE *pdXVec,// I X data vector
	const double HUGE *pdYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

int PlDrawXYVectors(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	const ULONG HUGE *pulXVec,// I X data vector
	const ULONG HUGE *pulYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

int PlDrawYVector(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const short HUGE *psYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

int PlDrawYVector(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const long HUGE *plYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

int PlDrawYVector(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const float HUGE *pfYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

int PlDrawYVector(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const double HUGE *pdYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

int PlDrawYVector(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const ULONG HUGE *pulYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

#endif // defined __cplusplus }

#if defined __cplusplus
extern "C" {
#endif

#if defined MAKE_DLL && defined WIN32
#define EXPORT16
#define EXPORT32 __declspec(dllexport)
#elif defined MAKE_DLL
#define EXPORT16 __export
#define EXPORT32
#else
#define EXPORT16
#define EXPORT32
#endif

EXPORT32 int EXPORT16 PASCAL
PlAreaArrayInit(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		nRow,			// I number of rows in axis array
	float	fVertPts,		// I spacing between rows, in points
	int		nCol,			// I number of columns in axis array
	float	fHorizPts);		// I spacing between columns, in points

EXPORT32 int EXPORT16 PASCAL
PlAreaArraySelect(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		iRow,			// I row in axis array, starting with 0
	int		iCol);			// I column in axis array, starting with 0

EXPORT32 int EXPORT16 PASCAL
PlAreaBackground(
	PL_CTX	*pPlot);		// I pointer to plot context structure

EXPORT32 int EXPORT16 PASCAL
PlAreaClear(
	PL_CTX	*pPlot);		// I pointer to plot context structure

EXPORT32 int EXPORT16 PASCAL
PlAreaFPtsAuto(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		nDataPoints,	// I number of data points along X axis
	float	fMaxPts,		// I maximum size for fPts
	float	*pfPts);		// O size, in points, to use in drawing

EXPORT32 int EXPORT16 PASCAL
PlAreaMove(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	float	fXFrac,			// I pos of left side from window left, or -1
	float	fYFrac,			// I pos of top side from window top, or -1
	float	fWidFrac,		// I width, or -1
	float	fHtFrac);		// I height, or -1

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeAutoEnds(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		bXAxis,			// I 1 to adjust X axis endpoints, else 0
	int		bYAxis);		// I 1 to adjust Y axis endpoints, else 0

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeGet(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		bVisible,		// I 1 if X axis range is to be set
	double	*pdXLeft,		// O ptr for left X data value, or NULL
	double	*pdXRight,		// O ptr for right X data value, or NULL
	double	*pdYBottom,		// O ptr for bottom Y data value, or NULL
	double	*pdYTop);		// O ptr for top Y data value, or NULL

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeSet(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bXAxis,			// I 1 if X axis range is to be set
	double	dXLeft,			// I value at left end of X axis
	double	dXRight,		// I value at right end of X axis
	int		bYAxis,			// I 1 if Y axis range is to be set
	double	dYBottom,		// I value at bottom end of Y axis
	double	dYTop);			// I value at top end of Y axis

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeSetXYVectors_sht(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	const short HUGE *psXVec,// I X data vector
	const short HUGE *psYVec);// I Y data vector

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeSetXYVectors_lng(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	const long HUGE *plXVec,// I X data vector
	const long HUGE *plYVec);// I Y data vector

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeSetXYVectors_flt(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	const float HUGE *pfXVec,// I X data vector
	const float HUGE *pfYVec);// I Y data vector

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeSetXYVectors_dbl(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	const double HUGE *pdXVec,// I X data vector
	const double HUGE *pdYVec);// I Y data vector

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeSetXYVectors_ul(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	const ULONG HUGE *pulXVec,// I X data vector
	const ULONG HUGE *pulYVec);// I Y data vector

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeSetYVector_sht(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const short HUGE *psYVec);// I Y data vector

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeSetYVector_lng(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const long HUGE *plYVec);// I Y data vector

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeSetYVector_flt(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const float HUGE *pfYVec);// I Y data vector

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeSetYVector_dbl(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const double HUGE *pdYVec);// I Y data vector

EXPORT32 int EXPORT16 PASCAL
PlAreaRangeSetYVector_ul(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const ULONG HUGE *pulYVec);// I Y data vector

EXPORT32 int EXPORT16 PASCAL
PlAreaRegXAnnotCallback(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	const char * (*pAnnotFn)(PL_CTX *, void *, double),// user callback function
	void	*pAnnotArg);	// arg for user call

EXPORT32 int EXPORT16 PASCAL
PlAreaRegYAnnotCallback(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	const char * (*pAnnotFn)(PL_CTX *, void *, double),// user callback function
	void	*pAnnotArg);	// arg for user call

EXPORT32 int EXPORT16 PASCAL
PlAreaTitleText(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		eSide,			// I selector for set of lines--PL_SIDE_xxx
	int		iLine,			// I line number, starting with 0
	float	fFrac,			// I anchor point, as fraction of title line length
	const char *szText,		// I text to print
	int		eJust,			// I positioning code: PRT_{LJ, CEN, RJ}
	int		bAxisRelative,	// I 1 position text relative to axis
	COLORREF rgb);			// I color to use for printing

EXPORT32 int EXPORT16 PASCAL
PlAreaXStyle(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		eStyle,			// I style for axis
	int		eScaleType,		// I axis scaling type--PL_LIN or PL_LOG
	int		nMajor,			// I number of major divisions for axis, or 0 or -1
	int		nMinor,			// I number of minor axis divisions within major
	float	fAnnotDeg,		// I axis annotations, ccw degrees from horizontal
	int		iAnnotNChar,	// I max length of annotation
	float	fAnnotPts,		// I font size in points of axis annotations, or 0
	const char *szAnnotFont,// I name of font for axis annotations, or "" or NULL
	const char *szAnnotFmt,	// I format for printing axis annotations
	int		nTitleLinesBelow,// I # of title lines below plot area
	int		nTitleLinesAbove,// I # of title lines above plot area
	float	fTitlePts,		// I font size in points of axis title, or 0
	const char *szTitleFont);// I name of font for axis title, or "" or NULL

EXPORT32 int EXPORT16 PASCAL
PlAreaYStyle(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		eStyle,			// I style for axis
	int		eScaleType,		// I axis scaling type--PL_LIN or PL_LOG
	int		nMajor,			// I number of major divisions for axis, or 0 or -1
	int		nMinor,			// I number of minor axis divisions within major
	float	fAnnotDeg,		// I axis annotations, ccw degrees from horizontal
	int		iAnnotNChar,	// I max length of annotation
	float	fAnnotPts,		// I font size in points of axis annotations, or 0
	const char *szAnnotFont,// I name of font for axis annotations, or "" or NULL
	const char *szAnnotFmt,	// I format for printing axis annotations
	int		nTitleLinesLeft,// I # of title lines left of plot area, or 0
	int		nTitleLinesRight,// I # of title lines right of plot area, or 0
	float	fTitlePts,		// I font size in points of axis title, or 0
	const char *szTitleFont);// I name of font for axis title, or "" or NULL

EXPORT32 int EXPORT16 PASCAL
PlAreaZoomStyle(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bXNoZoom,		// I 1 to inhibit X zoom
	int		bYNoZoom);		// I 1 to inhibit Y zoom

EXPORT32 int EXPORT16 PASCAL
PlDrawErrorBars(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXVal,			// I X data value
	double	dYVal,			// I Y data value
	int		bDrawXBar,		// I 1 to draw horizontal error bar
	double	dXLeft,			// I X data value of left end of error bar
	double	dXRight,		// I X data value of right end of error bar
	int		bDrawYBar,		// I 1 to draw vertical error bar
	double	dYBottom,		// I Y data value of bottom end of error bar
	double	dYTop,			// I Y data value of top end of error bar
	int		eDrawType,		// I code for drawing
	float	fPts,			// I size of point or mark, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawLine_patterned(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXVal1,			// I first X data value
	double	dYVal1,			// I first Y data value
	double	dXVal2,			// I second X data value
	double	dYVal2,			// I second Y data value
	int		ePattern,		// I line pattern--PL_LINE_XXX
	float	fPts,			// I thickness of line, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawLine(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXVal1,			// I first X data value
	double	dYVal1,			// I first Y data value
	double	dXVal2,			// I second X data value
	double	dYVal2,			// I second Y data value
	float	fPts,			// I thickness of line, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawMark(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXVal,			// I X data value
	double	dYVal,			// I Y data value
	int		eMark,			// I code for mark
	float	fPts,			// I size of mark, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawPoint(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXVal,			// I X data value
	double	dYVal,			// I Y data value
	float	fPts,			// I thickness of point, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawRect(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXVal1,			// I X data value for first corner
	double	dYVal1,			// I Y data value for first corner
	double	dXVal2,			// I X data value for second corner
	double	dYVal2,			// I Y data value for second corner
	float	fPts,			// I thickness of boundary line, in points
	COLORREF rgbRect,		// I color to use for drawing rectangle
	COLORREF rgbFill);		// I color to use for filling rectangle, or -1

EXPORT32 int EXPORT16 PASCAL
PlDrawRectRounded(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXVal1,			// I X data value for first corner
	double	dYVal1,			// I Y data value for first corner
	double	dXVal2,			// I X data value for second corner
	double	dYVal2,			// I Y data value for second corner
	float	fCornerPts,		// I radius of rounded corner, in points
	float	fPts,			// I thickness of boundary line, in points
	COLORREF rgbRect,		// I color to use for drawing rectangle border
	COLORREF rgbFill);		// I color to use for filling rectangle, or -1

EXPORT32 int EXPORT16 PASCAL
PlDrawText(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXVal,			// I X data value
	double	dYVal,			// I Y data value
	const char *szText,		// I text to print
	int		eRef,			// I positioning code: PRT_xxx
	float	fPtsOffset,		// I amount to offset text from X,Y, in points
	float	fPts,			// I text size, in points
	const char *szFont,		// I font name
	int		bBold,			// I 1 to print as bold
	int		bTransparent,	// I 1 to print with transparent background
	float	fDegrees,		// I angle counterclockwise from horizontal
	COLORREF rgb);			// I color to use for printing

EXPORT32 int EXPORT16 PASCAL
PlDrawXYVectors_sht(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	const short HUGE *psXVec,// I X data vector
	const short HUGE *psYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawXYVectors_lng(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	const long HUGE *plXVec,// I X data vector
	const long HUGE *plYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawXYVectors_flt(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	const float HUGE *pfXVec,// I X data vector
	const float HUGE *pfYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawXYVectors_dbl(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	const double HUGE *pdXVec,// I X data vector
	const double HUGE *pdYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawXYVectors_ul(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	const ULONG HUGE *pulXVec,// I X data vector
	const ULONG HUGE *pulYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawYVector_sht(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const short HUGE *psYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawYVector_lng(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const long HUGE *plYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawYVector_flt(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const float HUGE *pfYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawYVector_dbl(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const double HUGE *pdYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlDrawYVector_ul(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	long	n,				// I number of points
	double	dXLeft,			// I X value at left end of X axis
	double	dXIncr,			// I X value spacing between points
	const ULONG HUGE *pulYVec,// I Y data vector
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE, or PL_<mark>
	float	fPts,			// I drawing thickness or mark size, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlErrorCodeCheck(
	int		status);		// I status code

EXPORT32 const char * EXPORT16 PASCAL
PlErrorMessage(
	PL_CTX	*pCtx,			// I pointer to context structure, or NULL
	int		status);		// I PL_XXX status code

EXPORT32 void EXPORT16 PASCAL
PlErrorMessage_VB(
	PL_CTX	*pCtx,			// I pointer to context structure, or NULL
	int		status,			// I PL_XXX status code
	int		msgDim,			// I dimension of message buffer
	char	*szMsg);		// O place to store message string

EXPORT32 int EXPORT16 PASCAL
PlMoveTo(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	double	dXVal,			// I X data value
	double	dYVal,			// I Y data value
	int		bDraw,			// I 1 causes drawing while moving, else 0
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlObjBoxCreateAtPlotCursor(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bNoCallback,	// I 1 if object callback fn is not to be called
	PL_BOX	**ppBox,		// O place to store pointer to new PL_BOX, or NULL
	int		bSelect,		// I 1 if the box is to be selected
	int		bHide,			// I 1 if box is not to be shown, else 0
	int		bPreferClip,	// I 1 if position is more important than size
	double	dValL,			// I data val., left side, relative to plot cursor
	double	dValB,			// I data val., bottom side, relative to plot cursor
	double	dValR,			// I data val., right side, relative to plot cursor
	double	dValT,			// I data val., top side, relative to plot cursor
	COLORREF rgbBox);		// I color to use for drawing PL_BOX

EXPORT32 int EXPORT16 PASCAL
PlObjBoxCreateGeneral(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bNoCallback,	// I 1 if object callback fn is not to be called
	PL_BOX	**ppBox,		// O place to store pointer to new PL_BOX, or NULL
	int		bSelect,		// I 1 if the box is to be selected
	int		bHide,			// I 1 if box is not to be shown, else 0
	double	dValL,			// I data value for left side of PL_BOX
	int		iColL,			// I plot area column containing left side
	double	dValB,			// I data value for bottom side of PL_BOX
	int		iRowB,			// I plot area row containing bottom side
	double	dValR,			// I data value for right side of PL_BOX
	int		iColR,			// I plot area column containing right side
	double	dValT,			// I data value for top side of PL_BOX
	int		iRowT,			// I plot area row containing top side
	COLORREF rgbBox);		// I color to use for drawing PL_BOX

EXPORT32 int EXPORT16 PASCAL
PlObjBoxCreateSimple(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bNoCallback,	// I 1 if object callback fn is not to be called
	PL_BOX	**ppBox,		// O place to store pointer to new PL_BOX, or NULL
	int		bSelect,		// I 1 if the box is to be selected
	int		bHide,			// I 1 if box is not to be shown, else 0
	double	dValL,			// I data value for left side of PL_BOX
	double	dValB,			// I data value for bottom side of PL_BOX
	double	dValR,			// I data value for right side of PL_BOX
	double	dValT,			// I data value for top side of PL_BOX
	COLORREF rgbBox);		// I color to use for drawing PL_BOX

EXPORT32 int EXPORT16 PASCAL
PlObjBoxProps(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	PL_BOX	*pBox,			// I pointer to PL_BOX; NULL implies selected box
	int		bNoPaint,		// I 1 if the box is not to be repainted
	int		bSet,			// I 1 to set PL_BOX properties, 0 to get them
	PL_BOX_PROPS *pBoxProps);// IO pointer to caller's PL_BOX_PROPS struct

EXPORT32 int EXPORT16 PASCAL
PlObjBoxPropsDflt(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bSet,			// I 1 to set PL_BOX properties, 0 to get them
	PL_BOX_PROPS *pBoxProps);// IO pointer to caller's PL_BOX_PROPS struct

EXPORT32 int EXPORT16 PASCAL
PlObjBoxZoom(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	PL_BOX	*pBox,			// I pointer to PL_BOX; NULL implies selected box
	int		bZoom,			// I 1 zooms using pBox, 0 restores
	float	*pfXZoom,		// I place to store new horizontal zoom factor
	float	*pfYZoom,		// I place to store new vertical zoom factor
	float	*pfXScroll,		// I place to store horiz. scroll position, or NULL
	float	*pfYScroll);	// I place to store vertical scroll position, or NULL

EXPORT32 int EXPORT16 PASCAL
PlObjDelete(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bNoCallback,	// I 1 to inhibit calling object callback function
	PL_OBJ	*pObj);			// I pointer to PL_OBJ; NULL implies selected box

EXPORT32 int EXPORT16 PASCAL
PlObjDeleteAll(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bNoCallback);	// I 1 to inhibit calling object callback function

EXPORT32 int EXPORT16 PASCAL
PlObjFontSetDflt(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	float	fPts,			// I default text size, in points
	const char *szFont);	// I default font name

EXPORT32 int EXPORT16 PASCAL
PlObjLabelCreate(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bNoCallback,	// I 1 if object callback fn is not to be called
	PL_LABEL **ppLabel,		// O place to store pointer to new PL_LABEL, or NULL
	int		bSelect,		// I 1 if the label is to be selected
	double	dXVal,			// I X data value for PL_LABEL
	double	dYVal,			// I Y data value for PL_LABEL
	float	fLineFarX_pts,	// I datum to far end of line, X distance, pts
	float	fLineFarY_pts,	// I datum to far end of line, Y distance, pts
	float	fLineMeet_frac,	// I line meets at horiz frac of text length
	const char *szText,		// I text for PL_LABEL
	COLORREF rgbLabel);		// I color to use for drawing PL_LABEL

EXPORT32 int EXPORT16 PASCAL
PlObjLabelProps(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	PL_LABEL *pLabel,		// I pointer to PL_LABEL; NULL implies selected label
	int		bNoPaint,		// I 1 if the label is not to be repainted
	int		bSet,			// I 1 to set PL_LABEL properties, 0 to get them
	PL_LABEL_PROPS *pLabelProps);// IO pointer to caller's PL_LABEL_PROPS struct

EXPORT32 int EXPORT16 PASCAL
PlObjLabelPropsDflt(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bSet,			// I 1 to set PL_LABEL properties, 0 to get them
	PL_LABEL_PROPS *pLabelProps);// IO pointer to caller's PL_LABEL_PROPS struct

EXPORT32 int EXPORT16 PASCAL
PlObjLegendCreate(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bNoCallback,	// I 1 if object callback fn is not to be called
	PL_LEGEND **ppLegend,	// O place to store pointer to new PL_LEGEND, or NULL
	int		bSelect,		// I 1 if the legend is to be selected
	int		bHide,			// I 1 if legend is not to be shown, else 0
	int		eRefPt,			// I 'reference point' on legend border, PRT_xx
	int		bPlotWin,		// I 1 if legend is in plot window, 0 for data area
	float	fXFrac,			// I X position, frac. of plot window or data area
	float	fYFrac,			// I Y position, frac. of plot window or data area
	int		bBox,			// I 1 to show box around legend
	COLORREF rgbBox,		// I color for box
	const char *szTitle,	// I text for legend title, or "" or NULL
	COLORREF rgbTitle);		// I color to use for drawing title

EXPORT32 int EXPORT16 PASCAL
PlObjLegendCreateEntry(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	PL_LEGEND *pLegend,		// IO pointer to PL_LEGEND
	int		eDrawType,		// I drawing type--PL_POINT, PL_LINE[_xxx], or 0
	float	fDrawPts,		// I drawing thickness, in points, or 0
	int		eMarkType,		// I mark type--PL_<mark>, or 0
	float	fMarkPts,		// I mark size, in points, or 0
	COLORREF rgbData,		// I color for points, lines, and marks, or 0
	const char *szText,		// I text
	COLORREF rgbText);		// I text color

EXPORT32 int EXPORT16 PASCAL
PlObjLegendProps(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	PL_LEGEND *pLegend,		// I pointer to PL_LEGEND; NULL implies selected legend
	int		bNoPaint,		// I 1 if the legend is not to be repainted
	int		bSet,			// I 1 to set PL_LEGEND properties, 0 to get them
	PL_LEGEND_PROPS *pLegendProps);// IO pointer to caller's PL_LEGEND_PROPS struct

EXPORT32 int EXPORT16 PASCAL
PlObjLegendPropsDflt(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bSet,			// I 1 to set PL_LEGEND properties, 0 to get them
	PL_LEGEND_PROPS *pLegendProps);// IO pointer to caller's PL_LEGEND_PROPS struct

EXPORT32 int EXPORT16 PASCAL
PlObjMode(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bNoCallback,	// I 1 to inhibit calling object callback function
	int		bSet,			// I 1 to set PL_OBJ_MODE MODE, 0 to get it
	PL_OBJ_MODE *pMode);	// IO pointer to caller's PL_OBJ_MODE struct

EXPORT32 int EXPORT16 PASCAL
PlObjPaintAll(
	PL_CTX	*pPlot);		// I pointer to plot context structure

EXPORT32 int EXPORT16 PASCAL
PlObjSelect(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		bNoCallback,	// I 1 to inhibit calling object callback function
	PL_OBJ	*pObj,			// I pointer to PL_OBJ to select if bSelect is 1
	int		bSelect);		// I 1 to select, 0 to deselect

EXPORT32 int EXPORT16 PASCAL
PlPaintCursor(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		bErase,			// I 0,1 to draw,erase
	double	dXVal,			// I X data value
	double	dYVal,			// I Y data value
	int		eMark,			// I code for cursor shape
	float	fPts,			// I size of mark, in points
	COLORREF rgb);			// I color to use for drawing

EXPORT32 int EXPORT16 PASCAL
PlPlotClear(
	PL_CTX	*pPlot);		// I pointer to plot context structure

EXPORT32 int EXPORT16 PASCAL
PlPlotInit_prt(
	PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
	PL_CTX	*pPlotWin,		// I pointer to screen plot context, or NULL
	float	fBorderPts,		// I thickness of border to be drawn, or 0.F
	float	fLMarg_in,		// I margin left of plot area, in inches
	float	fRMarg_in,		// I margin right of plot area, in inches
	float	fTMarg_in,		// I margin above plot area, in inches
	float	fBMarg_in);		// I margin below plot area, in inches

EXPORT32 int EXPORT16 PASCAL
PlPlotInit_win(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	HWND	hwPlot,			// I handle to plot window
	int		bBorder,		// I 1 to cause active/inactive border to be drawn
	int		bScrollBars);	// I 1 if the window contains scroll bars

EXPORT32 int EXPORT16 PASCAL
PlPlotStyle(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int		nTitleLinesTop,	// I # of title lines at window top, or 0
	int		nTitleLinesBottom,// I # of title lines at window bottom, or 0
	int		nTitleLinesLeft,// I # of title lines at window left, or 0
	int		nTitleLinesRight,// I # of title lines at window right, or 0
	float	fTitlePts,		// I font size in points of titles, or 0.
	const char *szTitleFont);// I name of font for titles, or "" or NULL

EXPORT32 int EXPORT16 PASCAL
PlPlotTitleClear(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		eSide,			// I selector for set of lines--PL_SIDE_xxx
	int		iLine);			// I line number, starting with 0

EXPORT32 int EXPORT16 PASCAL
PlPlotTitleLine(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		eSide,			// I selector for set of lines--PL_SIDE_xxx
	int		iLine,			// I line number, starting with 0
	float	fFrac1,			// I left end, fraction of title line length
	float	fFrac2,			// I right end, fraction of title line length
	int		ePattern,		// I line pattern--PL_LINE_XXX
	float	fPts,			// I thickness of line, in points
	COLORREF rgb);			// I color to use for printing

EXPORT32 int EXPORT16 PASCAL
PlPlotTitleMark(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		eSide,			// I selector for set of lines--PL_SIDE_xxx
	int		iLine,			// I line number, starting with 0
	float	fFrac1,			// I left end, fraction of title line length
	float	fFrac2,			// I right end, fraction of title line length
	int		eMark,			// I code for mark
	float	fPts,			// I size of mark, in points
	COLORREF rgb);			// I color to use for printing

EXPORT32 int EXPORT16 PASCAL
PlPlotTitlePoint(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		eSide,			// I selector for set of lines--PL_SIDE_xxx
	int		iLine,			// I line number, starting with 0
	float	fFrac1,			// I left end, fraction of title line length
	float	fFrac2,			// I right end, fraction of title line length
	float	fPts,			// I thickness of point, in points
	COLORREF rgb);			// I color to use for printing

EXPORT32 int EXPORT16 PASCAL
PlPlotTitleText(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		eSide,			// I selector for set of lines--PL_SIDE_xxx
	int		iLine,			// I line number, starting with 0
	float	fFrac,			// I anchor point, as fraction of title line length
	const char *szText,		// I text to print
	int		eJust,			// I positioning code: PRT_{LJ, CEN, RJ}
	COLORREF rgb);			// I color to use for printing

EXPORT32 int EXPORT16 PASCAL
PlPlotWrapup(
	PL_CTX	*pPlot);		// IO pointer to plot context structure

EXPORT32 int EXPORT16 PASCAL
PlPrtClose(
	PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
	int		bAbort);		// I 1 to abort the printout

EXPORT32 int EXPORT16 PASCAL
PlPrtNewPage(
	PL_CTX	*pPlot);		// IO pointer to plot context struct for printing

EXPORT32 int EXPORT16 PASCAL
PlPrtOpenDefault(
	PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
	const char *szPrtJob,	// I name of print job, or "" or NULL
	const char *szPrtFile);	// I path for print file, or "" or NULL

EXPORT32 int EXPORT16 PASCAL
PlPrtOpenDialog(
	PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
	const char *szPrtJob,	// I name of print job, or "" or NULL
	const char *szPrtFile);	// I path for print file, or "" or NULL

EXPORT32 int EXPORT16 PASCAL
PlPrtPrintSimple(
	PL_CTX	*pPlot,			// I pointer to plot context struct for screen
	const char *szJobName,	// I name for print job in Windows Print Manager
	float	fBorderPts,		// I thickness of border to be drawn, or 0.F
	float	fLMarg_in,		// I margin left of plot area, in inches
	float	fRMarg_in,		// I margin right of plot area, in inches
	float	fTMarg_in,		// I margin above plot area, in inches
	float	fBMarg_in);		// I margin below plot area, in inches

EXPORT32 int EXPORT16 PASCAL
PlPrtStatusWindow(
	PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
	HWND	hwParent,		// I handle for parent window
	int		bDismiss);		// I 0 (or 1) to show (or dismiss) the window

EXPORT32 int EXPORT16 PASCAL
PlVersionCheck(
	int		iVersion);		// I version number from PL.H: PL_VER

EXPORT32 int EXPORT16 PASCAL
PlVisibleCheck(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXVal,			// I X data value
	double	dYVal);			// I Y data value

EXPORT32 HWND EXPORT16 PASCAL
PlWinCreate(
	HWND	hwParent,		// I parent window handle
	COLORREF rgbBkg,		// I color for background
	COLORREF rgbFg,			// I color for foreground
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

EXPORT32 int EXPORT16 PASCAL
PlWinCvtPtsToPix(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		bHoriz,			// I 1 for horizontal points to pixels
	int		bVert,			// I 1 for vertical points to pixels
	double	fPts);			// I number of points, or 0

EXPORT32 int EXPORT16 PASCAL
PlWinCvtXPixToAreaCol(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXPix);			// I X pixel value

EXPORT32 double EXPORT16 PASCAL
PlWinCvtXPixToXVal(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXPix);			// I X pixel value

EXPORT32 double EXPORT16 PASCAL
PlWinCvtXValToXPix(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dXVal);			// I X data value

EXPORT32 int EXPORT16 PASCAL
PlWinCvtYPixToAreaRow(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dYPix);			// I Y pixel value

EXPORT32 double EXPORT16 PASCAL
PlWinCvtYPixToYVal(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dYPix);			// I Y pixel value

EXPORT32 double EXPORT16 PASCAL
PlWinCvtYValToYPix(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	double	dYVal);			// I Y data value

EXPORT32 int EXPORT16 PASCAL
PlWinDestroy(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	HWND	hwPlot);		// I handle to plot window

EXPORT32 int EXPORT16 PASCAL
PlWinMove(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	int		iLeft,			// I coordinate of left side of window
	int		iTop,			// I coordinate of top side of window
	int		iWidth,			// I width of window
	int		iHeight);		// I height of window

EXPORT32 LRESULT EXPORT16 WINAPI
PlWinProc_helper(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	HWND	hWnd,			// I handle for plot window
	UINT	iMsg,			// I WM_xxx message code
	WPARAM	wParam,			// I wParam for message
	LPARAM	lParam);		// I lParam for message

EXPORT32 void EXPORT16 PASCAL
PlWinProc_UW_event(
	PL_CTX *pPlot,			// IO pointer to plot context structure
	int		eEvent,			// I event code, PL_UW_xxx
	int		iEventInfo1,	// I information for the event
	float	*pfScrollPos,	// IO scroll position for scroll events, 0
	int		*peAction);		// O action code for caller

EXPORT32 void EXPORT16 PASCAL
PlWinProc_UW_mouseEvent(
	PL_CTX *pPlot,			// IO pointer to plot context structure
	int		eEvent,			// I event code, PL_UW_xxx
	int		iEventInfo1,	// I information for the event
	int		iEventInfo2,	// I information for the event
	int		*peAction);		// O action code for caller

EXPORT32 void EXPORT16 PASCAL
PlWinProcKeydown(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	HWND	hwPlot,			// I handle for plot window
	WPARAM	wParam);		// I virtual key code

EXPORT32 int EXPORT16 PASCAL
PlWinRegCursorCallback(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	void (*pCursorFn)		// I user cursor callback function
		(PL_CTX *, void *, int, double, double),
	void	*pCursorArg);	// I arg for user call

EXPORT32 int EXPORT16 PASCAL
PlWinRegEventCallback(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	LRESULT (*pEventFn)(PPL_CTX, void *, HWND, UINT, WPARAM, LPARAM, int),
	void	*pEventArg,		// I arg for user call
	int		bPre);			// I 1 to register pre-processing callback

EXPORT32 int EXPORT16 PASCAL
PlWinRegObjectCallback(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	int (*pObjectFn)		// I user plot object callback function, or NULL
		(PL_CTX *, void *, PL_OBJ_CTX *, PL_OBJ *, int, double, double),
	void	*ppObjectArg);	// I arg for user call

EXPORT32 int EXPORT16 PASCAL
PlWinRegPlotCallback(
	PL_CTX	*pPlot,			// I pointer to plot context structure
	void (*pPlotFn)(PL_CTX *, void *),// I user callback function
	void	*pPlotArg);		// I arg for user call

EXPORT32 int EXPORT16 PASCAL
PlWinRepaint(
	PL_CTX	*pPlot);		// IO pointer to plot context structure

EXPORT32 int EXPORT16 PASCAL
PlWinScale(
	PL_CTX	*pPlot);		// IO pointer to plot context structure

EXPORT32 int EXPORT16 PASCAL
PlWinScroll(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	float	fXScroll,		// I X scroll position, between 0 and 1, inclusive
	float	fYScroll);		// I Y scroll position, between 0 and 1, inclusive

EXPORT32 int EXPORT16 PASCAL
PlWinScroll_pos(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	double	dXAnchVal,		// I X anchor value, as display value
	double	dYAnchVal,		// I Y anchor value, as display value
	float	fXAnchFrac,		// I X anchor position, as fraction of axis width
	float	fYAnchFrac);	// I Y anchor position, as fraction of axis height

EXPORT32 int EXPORT16 PASCAL
PlWinScroll_h(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	WPARAM	wParam,			// I wParam arg from windows scroll event
	LPARAM	lParam);		// I lParam arg from windows scroll event

EXPORT32 int EXPORT16 PASCAL
PlWinScroll_v(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	WPARAM	wParam,			// I wParam arg from windows scroll event
	LPARAM	lParam);		// I lParam arg from windows scroll event

EXPORT32 int EXPORT16 PASCAL
PlWinZoom(
	PL_CTX	*pPlot,			// IO pointer to plot context structure
	float	fXZoom,			// I X zoom factor, >= 1.
	float	fYZoom);		// I Y zoom factor, >= 1.

#undef EXPORT16
#undef EXPORT32

#if defined __cplusplus
}
#endif


#endif // ! defined HDR_pl }
