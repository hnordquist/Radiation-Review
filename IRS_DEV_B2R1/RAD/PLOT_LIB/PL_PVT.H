#if ! defined HDR_pl_pvt // {
#define HDR_pl_pvt

/* ======================================================================= */
/* ============================== pl_pvt.h =============================== */
/* ======================================================================= */


/*+/mod/TOC------------------------------------------------------------------
* pl_pvt.h - "private" prototypes for pl.cpp
*
* Purpose:
*		This header file supplies prototypes for PlPlotXxx functions that
*		are used internally in PL.LIB.
*
* Usage:
*		#include <windows.h>
*		#include <pl.h>
*		#include <pl_pvt.h>
*
* Date     Author		Revision
* -------- ------------	--------
* 12-22-96 R. Cole		created by moving prototypes from pl.h
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

int PASCAL
PlDraw_discrete(
PL_CTX	*pPlot,			// I pointer to plot context structure
HDC		hDC,			// I DC to use for drawing
int		xPx,			// I X pixel location
int		yPx,			// I Y pixel location
int		wid,			// I width of mark or point, in pixels
int		ht,				// I height of mark or point, in pixels
int		eDrawType,		// I code for mark or point
int		bDrawXBar,		// I 1 to draw horizontal error bar
int		xPxLeft,		// I X pix loc of left end of error bar
int		xPxRight,		// I X pix loc of right end of error bar
int		bDrawYBar,		// I 1 to draw vertical error bar
int		yPxBottom,		// I Y pix loc of bottom end of error bar
int		yPxTop,			// I Y pix loc of top end of error bar
COLORREF rgb);			// I color to use for drawing

int PASCAL
PlObjCopyAll(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_CTX	*pPlotSrc);		// I pointer to plot context containint the objects

WORD PASCAL
PlObjEditDialog(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bDblClick,		// I 1 if called for double click
WPARAM	wParam);		// I virtual key code

void PASCAL
PlObjEditDone(
PL_CTX	*pPlot);		// IO pointer to plot context structure

void PASCAL
PlObjEditMove(
PL_CTX	*pPlot);		// IO pointer to plot context structure

void PASCAL
PlObjEditStart(
PL_CTX	*pPlot);		// IO pointer to plot context structure

void PASCAL
PlObjInit(
PL_CTX	*pPlot);		// IO pointer to plot context structure

int PASCAL
PlObjKeyDown(
PL_CTX	*pPlot,			// IO pointer to plot context structure
WPARAM	wParam);		// I virtual key code

int PASCAL
PlObjWinDialog(
PL_CTX	*pPlot,			// IO pointer to plot context struct
HWND	hwParent,		// I handle for parent window
int		bHelpWin,		// I 1 to display the 'help' window
PL_OBJ	*pObject);		// I non-NULL (or NULL) to show (or dismiss) the window

int PASCAL
PlObjWrapup(
PL_CTX	*pPlot);		// IO pointer to plot context structure

void PASCAL
PlPlotInit_axis(
PL_AXIS	*pAxis);		// IO pointer to plot axis structure

void
PlTitleLineToPx(
PL_CTX	*pPlot,
int		nLines,			// I number of lines reserved
int		leftPx,
int		rightPx,
int		topPx,
int		bottomPx,
float	fTitlePts,
int		eSide,
int		iLine,
float	fFrac,
int		*pxPx,			// O X coordinate of "top left" corner of line
int		*pyPx,			// O Y coordinate of "top left" corner of line
float	*pfDeg);		// O text orientation, in degrees

int PASCAL
PlWinCvtPtsToPix(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		bHoriz,			// I 1 for horizontal points to pixels
int		bVert,			// I 1 for vertical points to pixels
double	fPts);			// I number of points

int PASCAL
PlWinHDCClipToArea(
PL_CTX	*pPlot,			// I pointer to plot context structure
HDC	hDC);				// I HDC

int PASCAL
PlWinHDCSetup(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		bBorder,		// I 1 to include border in clip region
int		bArea,			// I 1 to set clip region to current "gross" area
int		bDataArea,		// I 1 to set clip region to current "data" area
int		ePattern,		// I line pattern--PL_XXX
float	fPts,			// I drawing thickness or mark size, in points
int		eGetRGB,		// I 0:use rgb
						//   1:use GetBkColor
						//   2:use GetTextColor
						//   3:use rgb ^ GetBkColor (for use with R2_XORPEN)
COLORREF rgb,			// I color to use for drawing
HDC		*phDC,			// O place to store HDC
HPEN	*phPen,			// O place to store new HPEN, or NULL
HPEN	*phPenOld,		// O place to store old HPEN, or NULL
COLORREF *pRgb);		// O place to store new RGB, or NULL

void PASCAL
PlWinMouseInit(
PL_CTX	*pPlot);		// IO pointer to plot context structure

void PASCAL
PlWinMousePreProcess(
PL_CTX	*pPlot,		// IO pointer to plot context structure
HWND	hWnd,		// I handle for plot window
UINT	iMsg,		// I WM_xxx message code
WPARAM	wParam,		// I wParam for message
LPARAM	lParam);	// I lParam for message

void PASCAL
PlWinMouseSelectPointer(
PL_CTX	*pPlot,			// IO pointer to plot context structure
HWND	hWnd,			// I handle for plot window
UINT	iMsg,			// I WM_xxx message code
WPARAM	wParam,			// I wParam for message
LPARAM	lParam);		// I lParam for message

void PASCAL
PlWinMouseWrapup(
PL_CTX	*pPlot);		// IO pointer to plot context structure

int
PlWinPaintBorder(
HWND	hwPlot,			// I handle to plot window
HDC		hDC,			// I DC for drawing, or NULL
int		iActive);		// I border control -1,0,1 for figure_out,inact,act

#undef EXPORT16
#undef EXPORT32

#if defined __cplusplus
}
#endif


#endif // ! defined HDR_pl_pvt }
