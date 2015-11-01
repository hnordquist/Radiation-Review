/* ======================================================================= */
/* ================================ pl.cpp =============================== */
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
*--------------------------------------------------------------------------*/

/*+/mod/TOC------------------------------------------------------------------
* PL.LIB - PROGRAMMER'S REFERENCE TO PL.LIB FUNCTIONS
*
* Purpose:
*		PL.LIB contains a variety of functions for plotting data in the
*		Windows environment.
*
*		The features in PL.LIB include:
*		o	X and Y axes can be separately configured as linear or log10
*		o	plot areas can be configured as a box with tick marks, a box
*			with grid lines, and in other ways
*		o	several sets of data can be plotted simultaneously, with no
*			intrinsic limit on the number of data points
*		o	data that includes zero values can be plotted on log axes (which
*			would normally forbid values of 0)
*		o	data can be plotted with solid/patterned lines, points, and
*			geometric shapes
*		o	X and Y axes can be zoomed and scrolled separately
*		o	axes can be divided into into major and minor divisions, with
*			value annotations at the boundaries between major divisions
*		o	when axis annotations would overwrite each other (due to
*			crowding), extra annotations are suppressed
*		o	a powerful plot cursor feature allows showing the viewer the
*			"current point", and also provides an "auto scroll" capability
*		o	a bounding box capability allows the operator to select a
*			portion of the data, to automatically zoom so that a set
*			of data fills a plot area, and to automatically scroll between
*			regions
*		o	the plot window can be subdivided into rows and columns of plot
*			areas
*		o	very little additional coding is required to send the plot to a
*			monochrome or color printer
*
* Quick Reference:
*    #include <windows.h>
*    #include <pl.h>
*
*           PL_CTX   plCtx, *pPlot=&plCtx;
*
*       int PlAreaArrayInit     (<>pPlot, nRow, fVertPts, nCol, fHorizPts)
*       int PlAreaArraySelect   (<>pPlot, iRow, iCol)
*       int PlAreaBackground    (pPlot)
*       int PlAreaClear         (pPlot)
*       int PlAreaFPtsAuto      (pPlot, nDataPoints, fMaxPts, >pfPts)
*       int PlAreaMove          (<>pPlot, fXFrac, fYFrac, fWidFrac, fHtFrac)
*       int PlAreaRangeAutoEnds (<>pPlot, bXAxis, bYAxis)
*       int PlAreaRangeGet      (pPlot, bVisible, >dXLeft, >dXRight,
*                                    >dYBottom, >dYTop)
*       int PlAreaRangeSet      (<>pPlot, bXAxis, dXLeft, dXRight,
*                                    bYAxis, dYBottom, dYTop)
*       int PlAreaRangeSetXYVectors (<>pPlot, n, p<ty>XVec, p<ty>YVec)
*       int PlAreaRangeSetYVector (<>pPlot, n, dXLeft, dXIncr, p<ty>YVec)
*       int PlAreaRegXAnnotCallback (pPlot, pFn, pArg)
*       int PlAreaRegYAnnotCallback (pPlot, pFn, pArg)
*       int PlAreaTitleText     (pPlot, eSide, iLine, fFrac,
*                                    szText, eJust, bAxisRelative, rgb)
*                       eSide=PL_SIDE_{LEFT|RIGHT|TOP|BOTTOM}
*                       eJust=PRT_{LJ|CEN|RJ}
*       int PlAreaXStyle        (<>pPlot, eStyle, eScaleType,
*                                    nMajor, nMinor,
*                                    fAnnotDeg, iAnnotNChar,
*                                    fAnnotPts, szAnnotFont, szAnnotFmt,
*                                    nTitleLinesBelow, nTitleLinesAbove,
*                                    fTitlePts, szTitleFont)
*                       eStyle=PL_{NO|LINE|TICK|GRID|GRID_DOT|GRID_DASH|
*                                    EDGE_HALF|CENT_HALF}_AXIS
*                       eScaleType=PL_{LIN|LOG}
*                       szAnnotFmt=C 'double' format or PL_FMT_{AUTO|TENTO}
*       int PlAreaYStyle        (<>pPlot, eStyle, eScaleType,
*                                    nMajor, nMinor,
*                                    fAnnotDeg, iAnnotNChar,
*                                    fAnnotPts, szAnnotFont, szAnnotFmt,
*                                    nTitleLinesLeft, nTitleLinesRight,
*                                    fTitlePts, szTitleFont)
*                       eStyle=PL_{NO|LINE|TICK|GRID|GRID_DOT|GRID_DASH|
*                                    EDGE_HALF|CENT_HALF}_AXIS
*                       eScaleType=PL_{LIN|LOG}
*                       szAnnotFmt=C 'double' format or PL_FMT_{AUTO|TENTO}
*       int PlAreaZoomStyle     (pPlot, bXNoZoom, bYNoZoom)
*       int PlDrawErrorBars     (pPlot, dXVal, dYVal,
*                                    bDrawXBar, dXLeft, dXRight,
*                                    bDrawYBar, dYBottom, dYTop,
*                                    eDrawType, fPts, rgb)
*                       eDrawType=see PlDrawMark for marks
*                                =PL_POINT
*       int PlDrawLine          (pPlot, dX1, dY1, dX2, dY2, fPts, rgb)
*       int PlDrawLine_patterned (pPlot, dX1, dY1, dX2, dY2,
*                                    ePattern, fPts, rgb)
*                       ePattern=PL_LINE{|_DASH|_DOT|_DASHDOT|_DASHDOTDOT}
*       int PlDrawMark          (pPlot, dXVal, dYVal, eMark, fPts, rgb)
*                       eMark=PL_shape_attr, where 'shape'=
*                           {SQUARE|TRI_UP|TRI_DOWN|DIAMOND|CIRCLE|CROSS|X}
*                           '_attr'=nothing or {_SOLID|_BOLD}
*       int PlDrawPoint         (pPlot, dXVal, dYVal, fPts, rgb)
*       int PlDrawRect          (pPlot, dX1, dY1, dX2, dY2,
*                                    fPts, rgbRect, rgbFill)
*       int PlDrawRectRounded   (pPlot, dX1, dY1, dX2, dY2, fCornerPts,
*                                    fPts, rgbRect, rgbFill)
*       int PlDrawText          (pPlot, dXVal, dYVal, szText,
*                                    eRef, fPtsOffset, fPts,
*                                    szFont, bBold, bTransparent,
*                                    fDegrees, rgb)
*                       eRef=PRT_{TL|TC|TR|RC|BL|BC|BR|LC|C}
*       int PlDrawXYVectors     (pPlot, n, p<ty>XVec, p<ty>YVec,
*                                    eDrawType, fPts, rgb)
*                       eDrawType=PL_LINE{|_DASH|_DOT|_DASHDOT|_DASHDOTDOT}
*                                =see PlDrawMark for marks
*                                =PL_POINT
*       int PlDrawYVector       (pPlot, n, dXLeft, dXIncr, p<ty>YVec,
*                                    eDrawType, fPts, rgb)
*       int PlErrorCodeCheck    (statusCode)
*     char *PlErrorMessage      (<>pPlot, statusCode)
*      void PlErrorMessage_VB   (pPlot, statusCode, bufDim, >szBuf)
*       int PlMoveTo            (<>pPlot, dXVal, dYVal, bDraw, rgb)
*       int PlObjBoxCreateAtPlotCursor (<>pPlot, bNoCallback, >ppBox,
*                                    bSelect, bHide, bPreferClip,
*                                    dValL, dValB, dValR, dValT, rgbBox)
*       int PlObjBoxCreateGeneral (<>pPlot, bNoCallback, >ppBox,
*                                    bSelect, bHide,
*                                    dValL, iColL, dValB, iRowB,
*                                    dValR, iColR, dValT, iRowT, rgbBox)
*       int PlObjBoxCreateSimple (<>pPlot, bNoCallback, >ppBox,
*                                    bSelect, bHide,
*                                    dValL, dValB, dValR, dValT, rgbBox)
*       int PlObjBoxProps       (<>pPlot, pBox, bNoPaint, bSet, <>pBoxProps)
*       int PlObjBoxPropsDflt   (<>pPlot, bSet, <>pBoxProps)
*       int PlObjBoxZoom        (<>pPlot, pBox, bZoom, >fXZoom, >fYZoom,
*                                    >fXScroll, >fYScroll)
*       int PlObjDeleteAll      (<>pPlot, bNoCallback)
*       int PlObjDelete         (<>pPlot, bNoCallback, pObj)
*       int PlObjFontSetDflt    (<>pPlot, fPts, szFont)
*       int PlObjLabelCreate    (<>pPlot, bNoCallback, >ppLabel,
*                                    bSelect, dXVal, dYVal,
*                                    fFarX_pts, fFarY_pts, fMeet_frac,
*                                    szText, rgbLabel)
*       int PlObjLabelProps     (<>pPlot, <>pLabel, bNoPaint, bSet,
*                                    <>pLabelProps)
*       int PlObjLabelPropsDflt (<>pPlot, bSet, <>pLabelProps)
*       int PlObjLegendCreate   (<>pPlot, bNoCallback, >ppLegend
*                                    bSelect, bHide,
*                                    eRefPt, bPlotWin, fXFrac, fYFrac,
*                                    bBox, rgbBox, szTitle, rgbTitle)
*       int PlObjLegendCreateEntry (<>pPlot, <>pLegend, eDrawType, fDrawPts,
*                                    eMarkType, fMarkPts, rgbData,
*                                    szText, rgbText)
*       int PlObjLegendProps    (<>pPlot, <>pLegend, bNoPaint, bSet,
*                                    <>pLegendProps)
*       int PlObjLegendPropsDflt (<>pPlot, bSet, <>pLegendProps)
*       int PlObjMode           (<>pPlot, bNoCallback, bSet, <>pObjMode)
*       int PlObjPaintAll       (pPlot)
*       int PlObjSelect         (<>pPlot, bNoCallback, pObj, bSelect)
*       int PlPaintCursor       (pPlot, bErase, dXVal, dYVal,
*                                    eMark, fPts, rgb)
*                       eMark=PL_CROSS{|_TALL|_WIDE|_FULL}
*       int PlPlotClear         (pPlot)
*       int PlPlotInit_prt      (>pPlot, pPlotWin, fBorderPts,
*                                    fLMarg_in, fRMarg_in,
*                                    fTMarg_in, fBMarg_in)
*       int PlPlotInit_win      (>pPlot, hwPlot, bBorder, bScrollBars)
*       int PlPlotStyle         (<>pPlot, nTitleLinesTop, nTitleLinesBottom,
*                                    nTitleLinesLeft, nTitleLinesRight,
*                                    fTitlePts, szTitleFont)
*       int PlPlotTitleClear    (pPlot, eSide, iLine)
*                       eSide=PL_SIDE_{LEFT|RIGHT|TOP|BOTTOM}
*       int PlPlotTitleLine     (pPlot, eSide, iLine, fFrac1, fFrac2,
*                                    ePattern, fPts, rgb)
*       int PlPlotTitleMark     (pPlot, eSide, iLine, fFrac1, fFrac2,
*                                    eMark, fPts, rgb)
*       int PlPlotTitlePoint    (pPlot, eSide, iLine, fFrac1, fFrac2,
*                                    fPts, rgb)
*       int PlPlotTitleText     (pPlot, eSide, iLine, fFrac,
*                                    szText, eJust, rgb)
*                       eSide=PL_SIDE_{LEFT|RIGHT|TOP|BOTTOM}
*                       eJust=PRT_{LJ|CEN|RJ}
*       int PlPlotWrapup        (<>pPlot)
*       int PlPrtClose          (<>pPlot, bAbort)
*       int PlPrtOpenDefault    (<>pPlot, szPrtJob, szPrtFile)
*       int PlPrtOpenDialog     (<>pPlot, szPrtJob, szPrtFile)
*       int PlPrtNewPage        (<>pPlot)
*       int PlPrtPrintSimple    (>pPlot, szPrtJob, fBorderPts,
*                                    fLMarg_in, fRMarg_in,
*                                    fTMarg_in, fBMarg_in)
*       int PlPrtStatusWindow   (<>pPlot, hwParent, bDismiss)
*       int PlVersionCheck      (PL_VER)
*       int PlVisibleCheck      (pPlot, dXVal, dYVal)
*
*
* NOTE: functions below are preceded with '#' if they cause a repaint.
*
*      HWND PlWinCreate         (hwParent, rgbBkg, rgbFg,
*                                    iLeft, iTop, iWid, iHt)
*       int PlWinCvtPtsToPix    (pPlot, bHoriz, bVert, fPts)
*       int PlWinCvtXPixToAreaCol (pPlot, dXPix)
*    double PlWinCvtXPixToXVal  (pPlot, dXPix)
*    double PlWinCvtXValToXPix  (pPlot, dXVal)
*       int PlWinCvtYPixToAreaRow (pPlot, dYPix)
*    double PlWinCvtYPixToYVal  (pPlot, dYPix)
*    double PlWinCvtYValToYPix  (pPlot, dYVal)
*       int PlWinDestroy        (pPlot, hWnd)
* #     int PlWinMove           (pPlot, iLeft, iTop, iWid, iHt)
* # LRESULT PlWinProc_helper    (<>pPlot, hWnd, iMsg, wParam, lParam)
*      void PlWinProcKeydown    (<>pPlot, hwPlot, wParam)
*       int PlWinRegCursorCallback (<>pPlot, pFn, pArg)
*       int PlWinRegEventCallback (<>pPlot, pFn, pArg, bPre)
*       int PlWinRegObjectCallback (<>pPlot, pFn, pArg)
*       int PlWinRegPlotCallback (<>pPlot, pFn, pArg)
* #     int PlWinRepaint        (<>pPlot)
*       int PlWinScale          (<>pPlot)
*       int PlWinScroll         (<>pPlot, fXScroll, fYScroll)
* #     int PlWinScroll_pos     (<>pPlot, dXAnch, dYAnch, fXFrac, fYFrac)
* #     int PlWinScroll_h       (<>pPlot, wParam, lParam)
* #     int PlWinScroll_v       (<>pPlot, wParam, lParam)
* #     int PlWinZoom           (<>pPlot, fXZoom, fYZoom)
*
* Notes:
* 1.	Since this library is intended for use with a variety of languages,
*		boolean variables (such as bScrollBars) are described as having
*		values of 0 and 1 meaning FALSE and TRUE, respectively.  On input,
*		the caller may use the program's definitions for FALSE and TRUE;
*		on output, though, this library will always use 0 and 1.
*
* Date     Author		Revision
* -------- ------------	--------
* 03-20-95 R. Cole		created
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

#include <windows.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pl.h"
#include "pl_pvt.h"
#include "wu.h"

/*  KG  */
const char * const gaszPlStatusMsg[]={
//const char * FAR16 const gaszPlStatusMsg[]={
	/* PL_OK             */ "PL.LIB: succeeded",
	/* PL_PRT_CANCEL     */ "PL.LIB: operator canceled printout",
	/* PL_FILE           */ "PL.LIB: user selected 'Print to file'",
	/* PL_VERSION_MISMATCH */ "PL.LIB: incompatible versions: PL.LIB is 1.00",
	/* PL_BAD_INPUT      */ "PL.LIB: input check failed",
	/* PL_FILE_IO        */ "PL.LIB: file IO error",
	/* PL_MALLOC_FAIL    */ "PL.LIB: couldn't malloc memory",
	/* PL_GDI_FAIL       */ "PL.LIB: error in Windows GDI call",
	/* PL_VETOED         */ "PL.LIB: program vetoed an operation",
	/* PL_GEOM_CHANGED   */ "PL.LIB: geometry was modified",
	/* PL_FAIL           */ "PL.LIB: internal PL.LIB failure",
	/* PL_DIALOG_BUSY    */ "PL.LIB: a PL.LIB dialog box is busy",

	/* PL_ILLEGAL        */ "PL.LIB: unknown status code"
};