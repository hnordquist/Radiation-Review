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
/* ======================================================================= */
/* ============================== pl_win.cpp ============================= */
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
* pl_win.cpp - PlWinXxx routines
*
* Purpose:
*		The PlWinXxx functions are focused on the actual plot window.  They
*		include scrolling and zooming functions, the 'event loop', functions
*		to handle the scaling between 'data units' and 'pixels', etc.
*
* Notes:
* 1.	The routines in this module do relatively little checking of
*		input arguments.  It is assumed that the calling routine is
*		well tested and that it has already done the appropriate checking
*		on the input arguments.
*
* Date     Author		Revision
* -------- ------------	--------
* 03-23-95 R. Cole		created
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
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "pl.h"
#include "pl_pvt.h"
#include "wu.h"

#if !defined WIN32
#define MY_EXPORT __export
#else
#define MY_EXPORT
#endif

/*---------------------------------------------------------------------------
* STRUCTURED HIERARCHY CHART
*
*		This chart shows the control structure in PL_WIN.CPP.
*
*		The chart is organized into columns (arranged in groups of 5
*		for easier reading) and rows.  The rows consist mostly of
*		dots, to assist the reader in following across the page, but
*		with no intrinsic meaning.
*
*		Each row has a name (or action) at its right.  This name is the
*		"target" of the row.  In many cases, the names is a routine.
*		A ">" on the row indicates a call to (or invocation of) the
*		target.
*
*		In many cases, a column will have a "$" in it.  The $ occurs
*		on the row that "owns" the column.  There will be one or more
*		">" in the column indicating all of the targets that are called
*		by the owner of the column.
*
*		This chart can be used in two ways.  To find all the targets
*		that are called by a routine:  1) find the row with the routine's
*		name; 2) on that row, go to the column that has a $; and 3) go
*		up and down the column finding all the rows that have a > to
*		locate all the targets called by the routine.
*
*		To find all the routines that call a target routine:  1) find the
*		row with the target routine's name; 2) for each > on the row,
*		follow the column up or down to find a $; 3) for each $ found,
*		look to the right to see the name of one of the routines that
*		call the target routine.
*
*		If a row has no >, then the target isn't referenced by any
*		routine in PL_WIN.CPP.  If a row has no $, then the target
*		doesn't reference any other targets in PL_WIN.CPP.
*
*		(NOTE WELL: Some routines from PL_WIN.CPP aren't included in
*		this chart, since they don't introduce any substantive flow-
*		of-control issues.  Some Windows API calls aren't shown, for
*		the same reason.)

..>.. ..... ..... ..... $.... ..... ..... .....  WM_ERASEBKGND
..>.. ..... ..... ..... .$... ..... ..... .....  WM_PAINT
..>.. ..... ..... ..... ..$.. ..... ..... .....  WM_LBUTTONDOWN
..>.. ..... ..... ..... ...$. ..... ..... .....  WM_LBUTTONDBLCLK
..>.. ..... ..... ..... ....$ ..... ..... .....  WM_KILLFOCUS
..>.. ..... ..... ..... ..... $.... ..... .....  WM_SETFOCUS
..>.. ..... ..... ..... ..... .$... ..... .....  WM_KEYDOWN
..>.. ..... ..... ..... ..... ..$.. ..... .....  WM_GETDLGCODE
..>.. ..... ..... ..... ..... ...$. ..... .....  WM_HSCROLL
..>.. ..... ..... ..... ..... ....$ ..... .....  WM_VSCROLL

$.... ..... ..... ..... ..... ..... ..... .....  PlWinMove
.$... ..... ..... ..... ..... ..... ..>>. .....  PlWinProc
.>$.. ..... ..... ..... ..... ..... ..... .....  PlWinProc_helper
...$. ..... ..... ..... ..... .>... ..... .....  PlWinProcKeydown
....$ ..... ..... ..... ..... ..... ..... .....  PlWinZoom
....> $.... ..... ..... ..... ..... ..... .....  PlWinScroll_pos
..... .$... ..... ..... ..... ..... ..... .....  PlWinRepaint
..... ...$. ..... ..... ..... ...>. ..... .....  PlWinScroll_h
..... ....$ ..... ..... ..... ....> ..... .....  PlWinScroll_v

..... >..>> $.... ..... ..... ..... ..... .....  PlWinScroll
>>>>> ..... .$... ..... ..... ..... ..... .....  PlWinScale
....> ...>> >.... ..... ..... ..... ..... .....  pPlot->bNeedScale = 1
..... ..... .>$.. ..... ..... ..... ..... .....  PlWinScaleAxis
..... ..... >.>.. ..... ..... ..... ..... .....  PlWinScrollAxis

..... .>... ..... ..... .>... ..... .$... .....  PlWinPaint
..... ..... ..... ..... ..>.> >.... .>... >....  PlWinPaintBorder
>...> >>.>> ..... ..... ..... ..... ..$.. .....  InvalidateRect
.>>>. ..... ..... ..... ..... ..... ..... .....  DefWindowProc
>.... ..... ..... ..... ..... ..... ..... .....  MoveWindow
...>. ..... ..... ..... ..... ..... ...$. .....  PostMessage
..... ...>> ..... ..... ..>.. ..... ..... .....  SetFocus
..... >..>> ..... ..... >.... ..... ..... .....  SetScrollPos

..... ..... ..... ..... ..... ..... .>... .....  pPlot->pPlotFn
...>. ..... ..... ..... ..>>> >.... ..... .....  pPlot->pCursorFn
.>... ..... ..... ..... ..... ..... ..... .....  pPlot->pEventPreFn
..>.. ..... ..... ..... ..... ..... ..... .....  pPlot->pEventPostFn
....> .>... >.... ..... ..... ..... ..... .....  PlAreaArraySelect
..... ..... ..... ..... >.... ..... ..... $....  PlPlotClear

*--------------------------------------------------------------------------*/

#define CALL_CURS_FN pPlot->pCursorFn(pPlot, pPlot->pCursorArg,

LRESULT CALLBACK MY_EXPORT
PlWinProc(
HWND	hWnd,
UINT	iMsg,
WPARAM	wParam,
LPARAM	lParam);

int PASCAL
PlWinScaleAxis(
PL_CTX	*pPlot,		// IO pointer to plot context structure
PL_AXIS	*pAxis);	// IO pointer to axis structure

int PASCAL
PlWinScrollAxis(
PL_CTX	*pPlot,		// IO pointer to plot context structure
PL_AXIS	*pAxis);	// IO pointer to axis structure

/*+/csubr/TOC----------------------------------------------------------------
* PlWinCreate - create a window for "active mode" plotting
*
* Purpose:
*		Creates a window for use with the PlXxx routines.  The window
*		provides "active mode" plotting services, including:
*
*		o	repainting the plot window as needed
*		o	scrolling the plot window
*
* Return Value:
*		hwPlot  the handle to the plot window, or
*		NULL  if the window can't be created
*
* Notes:
* 1.	The window position and size are specified in terms of the
*		parent's coordinate system.
* 2.	This routine can't be used with Visual Basic programs.
* 3.	A useful technique for setting the size and position of the plot
*		window is to have it "shadow" a window that is created at design
*		time.  With this technique, a pseudo plot window is created using
*		the GUI layout editor.  At runtime, this routine is called to
*		create the actual plot window, with the pseudo plot window as the
*		parent, and with the same size as the pseudo plot window.
*
* See Also:
*		PlPlotInit_win, PlWinRepaint, PlWinZoom, PlWinDestroy, PlWinMove
*		PlWinRegPlotCallback, PlWinRegCursorCallback, PlWinRegEventCallback
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
PlWinCreate(
HWND	hwParent,		// I parent window handle
COLORREF rgbBkg,		// I color for background
COLORREF rgbFg,			// I color for foreground
int		iLeft,			// I coordinate of left side of window
int		iTop,			// I coordinate of top side of window
int		iWidth,			// I width of window
int		iHeight)		// I height of window
{
#if !defined WIN32
	HINSTANCE hInstance;	// parents's instance handle
#else
	HINSTANCE hInstance;	// parents's instance handle
#endif
	HWND	hwPlot=0;
	HDC		hDC;
	WNDCLASS plotClass;
	ATOM	atom;
	int		stat;

	hInstance = WuGetHInstance(hwParent);
	if (hInstance == 0) {
		MessageBox(NULL, "zero hInstance", "PlWinCreate", MB_OK);
		goto done;
	}
	stat = GetClassInfo(hInstance, PL_CLASS, &plotClass);
	if (stat == 0) {  // the class hasn't been registered yet
		plotClass.style			= CS_OWNDC | CS_DBLCLKS;
		plotClass.lpfnWndProc	= PlWinProc;
		plotClass.cbClsExtra	= 0;
		plotClass.cbWndExtra	= 1 * sizeof(void *);
		plotClass.hInstance		= hInstance;
		plotClass.hIcon			= NULL;
		plotClass.hCursor		= NULL;
		plotClass.hbrBackground	= NULL;
		plotClass.lpszMenuName	= NULL;
		plotClass.lpszClassName = PL_CLASS;
		atom = RegisterClass(&plotClass);
		if (atom == 0) {
			MessageBox(NULL, "error in RegisterClass", "PlWinCreate", MB_OK);
			goto done;
		}
	}

	hwPlot = CreateWindow(PL_CLASS, "",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | WM_HSCROLL | WM_VSCROLL,
		iLeft, iTop, iWidth, iHeight, hwParent, 0, hInstance, NULL);

	if (hwPlot == 0)
		goto done;
	SetWindowLong(hwPlot, PL_EXTRA_PPL_CTX, 0L);
	SetScrollRange(hwPlot, SB_HORZ, 0, 0, FALSE);
	SetScrollRange(hwPlot, SB_VERT, 0, 0, FALSE);
	hDC = GetDC(hwPlot);
	if (hDC == 0) {
		MessageBox(NULL, "error in GetDC", "PlWinCreate", MB_OK);
		DestroyWindow(hwPlot);
		hwPlot = 0;
		goto done;
	}
	SetBkColor(hDC, rgbBkg);
	SetTextColor(hDC, rgbFg);
	ReleaseDC(hwPlot, hDC);

done:
	return hwPlot;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinCvtPtsToPix - convert printer points to pixels
*
* Purpose:
*		Converts a distance in printer points to the corresponding
*		distance in pixels.
*
*		Sometimes this conversion depends on orientation--some devices
*		have different vertical and horizontal pixel pitches.  This routine
*		hides that detail from the caller.
*
*		Some devices have pixels so small that they are effectively
*		invisible.  When this routine is asked for the smallest point
*		size (by having fPts == 0), it takes special action for printers.
*		If the pixel size for the printer is less than .01 inch, then
*		this routine returns .01 inch worth of pixels.
*
* Return Value:
*		number of pixels
*
* Notes:
* 1.	If neither bHoriz nor bVert is 1, then this routine uses the
*		average of the horizontal and vertical pixel pitches.
*
* See Also:
*		PL_XPTS_TO_XPIX and PL_YPTS_TO_YPIX
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-25-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinCvtPtsToPix(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		bHoriz,			// I 1 for horizontal points to pixels
int		bVert,			// I 1 for vertical points to pixels
double	fPts)			// I number of points, or 0
{
	int		iPix;
	float	fTwipsPerPix;

	if (bVert)			fTwipsPerPix = pPlot->fTwipsPerPixY;
	else if (bHoriz)	fTwipsPerPix = pPlot->fTwipsPerPixX;
	else
		fTwipsPerPix = .5F * (pPlot->fTwipsPerPixX + pPlot->fTwipsPerPixY);

	// There are 1440 twips per inch.  For printers, we will say that
	// the smallest legible dot is .01 inch.
	if (pPlot->bPrinter && fPts <= 0.F) {
		float	fPixPerInch;
		fPixPerInch = 1440.F / fTwipsPerPix;
		iPix = (int)ceil(.01F * fPixPerInch);
	}
	else
		iPix = (int)ceil(20.F * fPts / fTwipsPerPix);

	if (iPix < 1)
		iPix = 1;

	return iPix;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinCvtXPixToAreaCol - convert X pixel coordinate to area array column
*
* Purpose:
*		Converts an X pixel coordinate to the number of the corresponding
*		column in the plot area array.
*
* Return Value:
*		column number (starting with 0) in plot area array, or
*		-1 if the coordinate doesn't lie within any column's data area
*
* See Also:
*		PlWinCvtYPixToAreaRow, PlWinCvtXPixToXVal
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-03-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinCvtXPixToAreaCol(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dXPix)			// I X pixel value
{
	int		col, colSave=pPlot->iAxisCol, rowSave=pPlot->iAxisRow;

	for (col=0; col<pPlot->nAxisCols; col++) {
		PlAreaArraySelect(pPlot, rowSave, col);
		if (dXPix >= pPlot->pXAxis->pxB && dXPix <= pPlot->pXAxis->pxT)
			break;
	}
	if (col >= pPlot->nAxisCols) col = -1;

	PlAreaArraySelect(pPlot, rowSave, colSave);
	return col;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinCvtXPixToXVal - convert X pixel coordinate to X data value
*
* Purpose:
*		Converts an X pixel coordinate to the corresponding X data value
*		in the currently selected plot area.  The conversion takes into
*		account the current zoom factor and the current scroll position.
*
*		If the X pixel coordinate is outside the data area, then this
*		routine returns a "virtual" data value based on the current pixel-
*		to-data-value scaling for the plot area.  This "virtual" data value
*		may be outside the valid range of data values for the plot area.
*
*		Use PlWinCvtXPixToAreaCol to determine which plot area column an
*		X pixel coordinate lies in, so that the appropriate plot area
*		can be selected prior to calling this routine.
*
* Return Value:
*		X data value
*
* See Also:
*		PlWinCvtXPixToAreaCol
*		PlWinCvtYPixToYVal, PlWinCvtXValToXPix
*
*-Date     Author		Revision
* -------- ------------	--------
* 05-04-95 R. Cole		created
*--------------------------------------------------------------------------*/
double PASCAL
PlWinCvtXPixToXVal(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dXPix)			// I X pixel value
{
	double	xVal;

	if (pPlot->pXAxis->dPxPerLVal == 0.)
		return 0.;

	xVal = pPlot->pXAxis->dLValBS +
					(dXPix - pPlot->pXAxis->pxB) / pPlot->pXAxis->dPxPerLVal;
	if (pPlot->pXAxis->eScaleType == PL_LOG) {
		xVal = pow(10., xVal);
		if (xVal <= pPlot->pXAxis->dDValB_minNon0)
			xVal = 0.;
	}

	return xVal;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinCvtXValToXPix - convert X data value to X pixel coordinate
*
* Purpose:
*		Converts an X data value to the corresponding X pixel coordinate.
*		The conversion takes into account the current zoom factor and
*		the current scroll position.
*
* Return Value:
*		X pixel coordinate
*
* Notes:
* 1.	The X pixel coordinate may be outside the window, if the X axis
*		has been zoomed and the data value is scrolled out of view.
*
* See Also:
*		PlWinCvtYValToYPix, PlWinCvtXPixToXVal
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-22-95 R. Cole		created
*--------------------------------------------------------------------------*/
double PASCAL
PlWinCvtXValToXPix(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dXVal)			// I X data value
{
	double	dXPix;

	PL_WIN_CVT_X_VAL_TO_X_PIX(dXVal, dXPix)

	return dXPix;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinCvtYPixToAreaRow - convert Y pixel coordinate to area array row
*
* Purpose:
*		Converts a Y pixel coordinate to the number of the corresponding
*		row in the plot area array.
*
* Return Value:
*		row number (starting with 0) in plot area array, or
*		-1 if the coordinate doesn't lie within any row's data area
*
* See Also:
*		PlWinCvtXPixToAreaCol, PlWinCvtYPixToYVal
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-03-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinCvtYPixToAreaRow(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dYPix)			// I Y pixel value
{
	int		row, colSave=pPlot->iAxisCol, rowSave=pPlot->iAxisRow;

	for (row=0; row<pPlot->nAxisRows; row++) {
		PlAreaArraySelect(pPlot, row, colSave);
		if (dYPix >= pPlot->pYAxis->pxT && dYPix <= pPlot->pYAxis->pxB)
			break;
	}
	if (row >= pPlot->nAxisRows) row = -1;

	PlAreaArraySelect(pPlot, rowSave, colSave);
	return row;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinCvtYPixToYVal - convert Y pixel coordinate to Y data value
*
* Purpose:
*		Converts a Y pixel coordinate to the corresponding Y data value
*		in the currently selected plot area.  The conversion takes into
*		account the current zoom factor and the current scroll position.
*
*		If the Y pixel coordinate is outside the data area, then this
*		routine returns a "virtual" data value based on the current pixel-
*		to-data-value scaling for the plot area.  This "virtual" data value
*		may be outside the valid range of data values for the plot area.
*
*		Use PlWinCvtYPixToAreaRow to determine which plot area row a Y
*		pixel coordinate lies in, so that the appropriate plot area
*		can be selected prior to calling this routine.
*
* Return Value:
*		Y data value
*
* See Also:
*		PlWinCvtYPixToAreaRow
*		PlWinCvtXPixToXVal, PlWinCvtYValToYPix
*
*-Date     Author		Revision
* -------- ------------	--------
* 05-04-95 R. Cole		created
*--------------------------------------------------------------------------*/
double PASCAL
PlWinCvtYPixToYVal(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dYPix)			// I Y pixel value
{
	double	yVal;

	if (pPlot->pYAxis->dPxPerLVal == 0.)
		return 0.;

	yVal = pPlot->pYAxis->dLValBS +
			(dYPix - pPlot->pYAxis->pxB) / pPlot->pYAxis->dPxPerLVal;
	if (pPlot->pYAxis->eScaleType == PL_LOG) {
		yVal = pow(10., yVal);
		if (yVal < pPlot->pYAxis->dDValB_minNon0)
			yVal = 0.;
	}

	return yVal;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinCvtYValToYPix - convert Y data value to Y pixel coordinate
*
* Purpose:
*		Converts a Y data value to the corresponding Y pixel coordinate.
*		The conversion takes into account the current zoom factor and
*		the current scroll position.
*
* Return Value:
*		Y pixel coordinate
*
* Notes:
* 1.	The Y pixel coordinate may be outside the window, if the Y axis
*		has been zoomed and the data value is scrolled out of view.
*
* See Also:
*		PlWinCvtXValToXPix, PlWinCvtYPixToYVal
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-22-95 R. Cole		created
*--------------------------------------------------------------------------*/
double PASCAL
PlWinCvtYValToYPix(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dYVal)			// I Y data value
{
	double	dYPix;

	PL_WIN_CVT_Y_VAL_TO_Y_PIX(dYVal, dYPix)

	return dYPix;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinDestroy - destroy a plot window
*
* Purpose:
*		Destroys a plot window that was created using PlWinCreate.  This
*		effectively stops the event loop for the window.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* See Also:
*		PlWinCreate
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-31-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinDestroy(
PL_CTX	*pPlot,			// I pointer to plot context structure
HWND	hwPlot)			// I handle to plot window
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, !IsWindow(hwPlot), done)

	DestroyWindow(hwPlot);

done:
	PL_IF_INP_ERR("PlWinDestroy")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlWinHDCClipToArea - set a clip region in an HDC
*
* Purpose:
*		Sets a clip region in the HDC, to clip to plot area boundaries.
*
* Return Value:
*		PL_OK, or
*		other error codes
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-27-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinHDCClipToArea(
PL_CTX	*pPlot,			// I pointer to plot context structure
HDC		hDC,			// I HDC
int		bDataArea)		// I 1 to drawing area
{
	int		stat;
	HRGN	rgn=0;

	if (bDataArea) {
		rgn = CreateRectRgn(pPlot->pXAxis->pxB, pPlot->pYAxis->pxT,
							pPlot->pXAxis->pxT+1, pPlot->pYAxis->pxB+1);
	}
	else {
		rgn = CreateRectRgn( pPlot->pXAxis->pxAreaB, pPlot->pYAxis->pxAreaT,
							pPlot->pXAxis->pxAreaT+1, pPlot->pYAxis->pxAreaB+1);
	}
	if (rgn == 0) goto gdi_error;
	stat = SelectClipRgn(hDC, rgn);
	if (stat == ERROR) goto gdi_error;
	DeleteObject(rgn);

	return PL_OK;

gdi_error:
	if (rgn != 0) DeleteObject(rgn);
	return PL_GDI_FAIL;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlWinHDCSetup - set up an HDC
*
* Purpose:
*		Sets up an HDC with the appropriate color, thickness, and pattern.
*
*		When the plot operation is complete, the caller must do the
*		following.  (This must also be done for error returns from
*		this routine.)
*
*		SetROP2(hDC, R2_COPYPEN);  // (only for eGetRGB of 4 or 5)
*		if (hPenOld != 0) SelectObject(hDC, hPenOld);
*		if (hPen != 0) DeleteObject(hPen);
*		if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
*
* Return Value:
*		PL_OK, or
*		other error codes
*
* Notes:
* 1.	For monochrome printers, the color to use for drawing is set to
*		BLACK when eGetRGB is 0.
*
* BUGS:
* 1.	This routine has gotten to be cluttered, clumsy, and somewhat
*		error prone to use.  For example, eGetRGB==4 and 5 users have
*		to call SetROP2.  This routine ought to be rewritten, perhaps
*		based on SaveDC and RestoreDC.
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-27-95 R. Cole		created
*--------------------------------------------------------------------------*/
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
						//   3:use rgb ^ GetBkColor (use with R2_XORPEN)
						//   4:use rgb, and OPAQUE XOR pen
						//   5:use rgb ^ GetBkColor, and OPAQUE XOR pen
COLORREF rgb,			// I color to use for drawing
HDC		*phDC,			// O place to store HDC
HPEN	*phPen,			// O place to store new HPEN; if NULL, pen not changed
HPEN	*phPenOld,		// O place to store old HPEN, or NULL
COLORREF *pRgb)			// O place to store new RGB, or NULL
{
	RECT	rectPlot;
	HPEN	hPen, hPenOld;
	HBRUSH	hBrush;
	HRGN	rgn=0;
	COLORREF myRgb=rgb;
	int		iThick;
	int		pattern;
	int		stat;

	*phDC = 0;
	if (phPen != NULL) *phPen = 0;
	if (phPenOld != NULL) *phPenOld = 0;

	if (pPlot->hDC == 0)	*phDC = GetDC(pPlot->hwPlot);
	else					*phDC = pPlot->hDC;
	if (*phDC == 0) goto gdi_error;

	if (eGetRGB == 4 || eGetRGB == 5)
		SetBkMode(*phDC, OPAQUE);
	else
		SetBkMode(*phDC, TRANSPARENT);

	if (eGetRGB == 1) myRgb = GetBkColor(*phDC);
	else if (eGetRGB == 2) myRgb = GetTextColor(*phDC);
	else if (eGetRGB == 3) myRgb ^= GetBkColor(*phDC);
	else if (eGetRGB == 5) myRgb ^= GetBkColor(*phDC);
	else if (pPlot->bPrinter && !pPlot->bPrinterColor) myRgb = BLACK;

	switch (ePattern) {
		case PL_LINE_DASH:		pattern = PS_DASH; break;
		case PL_LINE_DOT:		pattern = PS_DOT; break;
		case PL_LINE_DASHDOT:	pattern = PS_DASHDOT; break;
		case PL_LINE_DASHDOTDOT:pattern = PS_DASHDOTDOT; break;
		default:				pattern = PS_SOLID; break;
	}

	iThick = PL_PTS_TO_PIX(fPts);
	if (iThick < 1) iThick = 1;
	if (phPen != NULL) {
		if ((hPen = CreatePen(pattern, iThick, myRgb)) == 0) goto gdi_error;
		//if ((hPenOld = SelectObject(*phDC, hPen)) == 0) goto gdi_error;
/*KG*/	if ((hPenOld = (HPEN)SelectObject(*phDC, hPen)) == 0) goto gdi_error;
	}
	if (eGetRGB == 4 || eGetRGB == 5) {
		if (SetROP2(*phDC, R2_XORPEN) == 0) goto gdi_error;
	}
	else {
		if (SetROP2(*phDC, R2_COPYPEN) == 0) goto gdi_error;
	}
		//if ((hBrush = GetStockObject(NULL_BRUSH)) == 0) goto gdi_error;
/*KG*/	if ((hBrush = (HBRUSH)GetStockObject(NULL_BRUSH)) == 0) goto gdi_error;
	if (SelectObject(*phDC, hBrush) == 0) goto gdi_error;
	if (phPen != NULL) *phPen = hPen;
	if (phPenOld != NULL) *phPenOld = hPenOld;
	if (pRgb != NULL) *pRgb = myRgb;

	if (bBorder && pPlot->hwPlot != NULL)
		GetClientRect(pPlot->hwPlot, &rectPlot);
	else
		rectPlot = pPlot->rectPlot;
	rgn = CreateRectRgn(rectPlot.left, rectPlot.top,
											rectPlot.right, rectPlot.bottom);
	if (rgn == 0) goto gdi_error;
	stat = SelectClipRgn(*phDC, rgn);
	if (stat == ERROR) goto gdi_error;
	DeleteObject(rgn);
	if (bArea || bDataArea)
		PlWinHDCClipToArea(pPlot, *phDC, bDataArea);
	return PL_OK;

gdi_error:
	if (hPen != 0) DeleteObject(hPen);
	if (rgn != 0) DeleteObject(rgn);
	return PL_GDI_FAIL;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinMove - set position and size for plot window
*
* Purpose:
*		Sets the position (relative to the parent) and size of the plot
*		window.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Side Effects:
*		o	The plot window is repainted.
*
* Notes:
* 1.	The window position and size are specified in terms of the
*		parent's coordinate system.
* 2.	A useful technique for setting the size and position of the plot
*		window is to have it "shadow" a window that is created at design
*		time.  At runtime, this routine is called to modify the actual
*		plot window whenever the pseudo plot window is moved or resized.
*
* See Also:
*		PlWinCreate, PlWinDestroy
*
*-Date     Author		Revision
* -------- ------------	--------
* 06-27-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinMove(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		iLeft,			// I coordinate of left side of window
int		iTop,			// I coordinate of top side of window
int		iWidth,			// I width of window
int		iHeight)		// I height of window
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pPlot->hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, !IsWindow(pPlot->hwPlot), done)

	MoveWindow(pPlot->hwPlot, iLeft, iTop, iWidth, iHeight, FALSE);

done:
	PL_IF_INP_ERR("PlWinMove")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlWinPaint - paint the plot window
*
* Purpose:
*		Paint the plot window.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 06-29-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlWinPaint(
PL_CTX	*pPlot,
HDC		hDC)
{
	int		retStat;
	void (*pPlotFn)(PL_CTX *, void *);
	void	*pPlotArg;
	HDC		hMyDC, hSaveDC;

	if (pPlot->bNeedScale)
		PlWinScale(pPlot);

	if (hDC == 0)	hMyDC = GetDC(pPlot->hwPlot);
	else			hMyDC = hDC;
	if (hMyDC == 0) goto done;

	pPlotFn = pPlot->pPlotFn;
	pPlotArg = pPlot->pPlotArg;
	if (pPlot->bBorder)
		retStat = PlWinPaintBorder(pPlot->hwPlot, hMyDC, -1);
	if (pPlotFn != NULL) {
		hSaveDC = pPlot->hDC;
		pPlot->hDC = hMyDC;
		pPlotFn(pPlot, pPlotArg);
		pPlot->hDC = hSaveDC;
	}
done:
	if (hDC == 0 && hMyDC != 0) ReleaseDC(pPlot->hwPlot, hMyDC);
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlWinPaintBorder - paint border around plot window
*
* Purpose:
*		Paint border around plot window.
*
* Return Value:
*		PL_OK, or
*		another code if an error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 06-16-95 R. Cole		created
* 08-02-95 R. Cole		changed to use CAPTION colors if BORDER colors are
*						the same;
*--------------------------------------------------------------------------*/
int
PlWinPaintBorder(
HWND	hwPlot,			// I handle to plot window
HDC		hDC,			// I DC for drawing, or NULL
int		iActive)		// I border control -1,0,1 for figure_out,inact,act
{
	int		retStat=PL_OK;
	RECT	rect;
	HRGN	rgn;
	HPEN	hPen=0, hPenOld=0;
	int		xBorder, yBorder;
	COLORREF rgb, rgbActive, rgbInactive;
	int		bFocus;
	int		bMyDC=0;
	int		stat;

	if (hDC == 0) {
		bMyDC = 1;
		hDC = GetDC(hwPlot);
		if (hDC == 0) goto done;
	}
	GetClientRect(hwPlot, &rect);
	xBorder = 2*GetSystemMetrics(SM_CXBORDER);
	yBorder = 2*GetSystemMetrics(SM_CYBORDER);
	rgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
	if (rgn == 0)goto done;
	stat = SelectClipRgn(hDC, rgn);
	if (stat == ERROR) goto done;
	DeleteObject(rgn);

	rgbActive = GetSysColor(COLOR_ACTIVEBORDER);
	rgbInactive = GetSysColor(COLOR_INACTIVEBORDER);
	if (rgbActive == rgbInactive) {
		rgbActive = GetSysColor(COLOR_ACTIVECAPTION);
		rgbInactive = GetSysColor(COLOR_INACTIVECAPTION);
	}
	bFocus = (GetFocus() == hwPlot);
	if (iActive == 1 || (bFocus && (iActive == -1)))
		rgb = rgbActive;
	else
		rgb = rgbInactive;

	if ((hPen = CreatePen(PS_SOLID, xBorder, rgb)) == 0) goto x_done;
		//if ((hPenOld = SelectObject(hDC, hPen)) == 0) goto x_done;
/*KG*/	if ((hPenOld = (HPEN)SelectObject(hDC, hPen)) == 0) goto x_done;

	MoveToEx(hDC, rect.left, rect.top + yBorder/2, NULL);
	if (LineTo(hDC, rect.right, rect.top + yBorder/2) == 0)
		goto gdi_error;
	MoveToEx(hDC, rect.left, rect.bottom - yBorder/2, NULL);
	if (LineTo(hDC, rect.right, rect.bottom - yBorder/2) == 0)
		goto gdi_error;
x_done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);

	hPen = hPenOld = 0;
	if ((hPen = CreatePen(PS_SOLID, yBorder, rgb)) == 0) goto y_done;
		//if ((hPenOld = SelectObject(hDC, hPen)) == 0) goto y_done;
/*KG*/	if ((hPenOld = (HPEN)SelectObject(hDC, hPen)) == 0) goto y_done;

	MoveToEx(hDC, rect.left + xBorder/2, rect.top, NULL);
	if (LineTo(hDC, rect.left + xBorder/2, rect.bottom) == 0)
		goto gdi_error;
	MoveToEx(hDC, rect.right - xBorder/2, rect.top, NULL);
	if (LineTo(hDC, rect.right - xBorder/2, rect.bottom) == 0)
		goto gdi_error;
y_done:
done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (bMyDC && hDC != 0) ReleaseDC(hwPlot, hDC);
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlWinProc - Windows event processing for PlPlot window class
*
* Purpose:
*		Process Windows events for the PlPlot window class.
*
* Return Value:
*		void
*
* Implicit Inputs:
*		PL_EXTRA_PPLOT "window long" data item is the PL_CTX * pointer
*				to the plot structure for the window
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-06-95 R. Cole		created
*--------------------------------------------------------------------------*/
LRESULT CALLBACK MY_EXPORT
PlWinProc(
HWND	hWnd,
UINT	iMsg,
WPARAM	wParam,
LPARAM	lParam)
{
	PL_CTX *pPlot;

	pPlot = (PL_CTX *)GetWindowLong(hWnd, PL_EXTRA_PPL_CTX);

	if (pPlot == NULL || strcmp(pPlot->szIntInit, "yes") != 0)
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	else
		return PlWinProc_helper(pPlot, hWnd, iMsg, wParam, lParam);
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinProc_helper - process events for a plot window
*
* Purpose:
*		Processes the various events for a plot window.
*
* Return Value:
*		0, or other value appropriate for the Windows message
*
* Side Effects:
*		o	The plot window is repainted for the following events:
*				WM_PAINT,
*
* Notes:
* 1.	For a caller-owned plot window, this function can be used as
*		a substitute for DefWindowProc.  This allows the caller to
*		process a subset of the plot window messages and to allow the
*		pl_plot module to handle the rest.
* 2.	NOTE WELL!!  This function will operate unpredictably if
*		PlPlotInit hasn't been called.
*
* See Also:
*		PlWinProcKeydown, PlWinCreate, PlPlotInit
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-17-95 R. Cole		created
* 08-16-95 R. Cole		added handling for WM_GETDLGCODE
*--------------------------------------------------------------------------*/
LRESULT PASCAL
PlWinProc_helper(
PL_CTX *pPlot,			// IO pointer to plot context structure
HWND	hWnd,			// I handle for plot window
UINT	iMsg,			// I WM_xxx message code
WPARAM	wParam,			// I wParam for message
LPARAM	lParam)			// I lParam for message
{
	HDC		hDC=0;
	PL_OBJ_CTX	*pObjCtx=pPlot->pObjCtx;
	PL_MOUSE *pMouse=pPlot->pMouse;
	int		retStat;
	PAINTSTRUCT ps;
	LRESULT	lresult=0;	// for most (not all) iMsg, 0 says "I processed it"

	if (hWnd != pPlot->hwPlot)
		goto default_proc;

	if (pPlot->pObjCtx->hwObjDialog != 0) {
		if (iMsg == WM_LBUTTONDOWN || iMsg == WM_LBUTTONUP ||
								iMsg == WM_MBUTTONDOWN || iMsg == WM_MBUTTONUP ||
								iMsg == WM_RBUTTONDOWN || iMsg == WM_RBUTTONUP ||
								iMsg == WM_KEYDOWN || iMsg == WM_KEYUP ||
								iMsg == WM_MOUSEMOVE || iMsg == WM_SETCURSOR) {
			goto default_proc;
		}
	}
	if (iMsg == WM_SETCURSOR)
		goto set_cursor;

	if (pPlot->bNeedScale)
		PlWinScale(pPlot);

	// Find out where the mouse pointer is; convert to plot window coordinates;
	// check to see if it is in the window, a plot area, on a PL_BOX, etc.;
	// and make a preliminary selection for a new mouse pointer.
	PlWinMousePreProcess(pPlot, hWnd, iMsg, wParam, lParam);

	if (pPlot->pEventPreFn != NULL) {
		lresult = pPlot->pEventPreFn(pPlot,
					pPlot->pEventPreArg, hWnd, iMsg, wParam, lParam, 0);
		if (lresult != 0)
			return lresult;
	}

	if (iMsg == WM_ERASEBKGND) {
		PlPlotClear(pPlot);
		lresult = 1;	// 1 says background was erased
	}
	else if (iMsg == WM_SIZE)
		pPlot->bNeedScale = 1;
	else if (iMsg == WM_PAINT) {
		HRGN	hRGN;
		int		stat;
		hRGN = CreateRectRgn(0, 0, 1, 1);
		if (hRGN != 0) {
			stat = GetUpdateRgn(hWnd, hRGN, FALSE);
			if (stat == SIMPLEREGION || stat == COMPLEXREGION)
				pPlot->hRgnPaint = hRGN;
		}
		hDC = BeginPaint(hWnd, &ps);
		if (hDC != 0)
			PlWinPaint(pPlot, hDC);
		if (hRGN != 0) DeleteObject(hRGN);
		pPlot->hRgnPaint = 0;
		EndPaint(hWnd, &ps);
	}
	else if (iMsg == WM_LBUTTONDOWN) {
		// Start an edit/create/copy.  (If nothing is done, the WM_LBUTTONUP
		// will trigger a click for cursor and focus processing.)
		PlObjEditStart(pPlot);
	}
	else if (iMsg == WM_RBUTTONUP) {
		HCURSOR	hCurOld;
		pPlot->pObjCtx->bObjEditDone = 0;
		hCurOld = SetCursor(LoadCursor(NULL, IDC_ARROW));
		PlObjWinDialog(pPlot, pPlot->hwPlot, 1, NULL);
		while (!pPlot->pObjCtx->bObjEditDone)
			WuDoEvents();
		if (hCurOld != 0) SetCursor(hCurOld);
	}
	else if (iMsg == WM_LBUTTONUP) {
		int		bInArea=pMouse->bInOtherArea || pMouse->bInCurrArea;
		int		bClickUsedUp=0;

		// Finish the edit/create, including any painting that's needed.
		if (pObjCtx->bCreateActive || pObjCtx->bEditActive)
			PlObjEditDone(pPlot);
		if (pMouse->bTossNextClick) {
			bClickUsedUp = 1;			// ignore the click
			pMouse->bTossNextClick = 0;
		}
		else if (pMouse->bLeftClick) {
			if (pPlot->bEnbCursor) {
				if (bInArea)
					bClickUsedUp = 0;	// if in data area, pass the click on
				else
					bClickUsedUp = 1;	// not in data area; ignore the click
			}
			else
				bClickUsedUp = 1;		// if cursor not enabled, ignore the click
		}
		else
			bClickUsedUp = 1;
		if (pMouse->bInPlotWin && GetFocus() != hWnd && bClickUsedUp == 1) {
			// Even if we've "used" this button up for bounding box operations,
			// pass it onward if the plot window doesn't yet have the focus
			// (unless the mouse-up is outside the plot window).
			bClickUsedUp = 0;
		}
		PlWinMousePreProcess(pPlot, hWnd, iMsg, wParam, lParam);
		pMouse->bPlotCursorClick = 1;
		if (pObjCtx->pSelObj != NULL) {
			bClickUsedUp = 1;
			pMouse->bPlotCursorClick = 0;
		}
		if (GetFocus() != hWnd) {
			if (pPlot->bBorder)
				retStat = PlWinPaintBorder(pPlot->hwPlot, 0, 1);
			SetFocus(hWnd);
			pMouse->bPlotCursorClick = 0;
		}
		else if (bClickUsedUp)
			;		// no further action
		else {
			if (pPlot->pCursorFn == NULL)
				goto default_proc;
			else if (pMouse->bInOtherArea) {
				if (pPlot->bCursorVisible)
					CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
				CALL_CURS_FN PL_CURS_AREA, pMouse->coCurr.y.iRowCol,
													pMouse->coCurr.x.iRowCol);
			}
			else if (pMouse->bInCurrArea) {
				if (pPlot->bCursorVisible)
					CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
				CALL_CURS_FN PL_LMOUSE, pMouse->coCurr.x.dVal,
														pMouse->coCurr.y.dVal);
			}
			else
				goto default_proc;
		}
	}
	else if (iMsg == WM_MOUSEMOVE) {
		if (pMouse->bLeftClick || pMouse->bRightClick || pMouse->bMiddleClick) {
			// ignore motion until PlWinMousePreProcess says it exceeds
			// the click threshold
			goto default_proc;
		}
		else if (pMouse->bLeftDown) {
			if (pObjCtx->bEditActive || pObjCtx->bCreateActive)
				PlObjEditMove(pPlot);
			else {
				pMouse->bUseNOTPointer = 1;
			}
		}
		else
			goto default_proc;
	}
	else if (iMsg == WM_LBUTTONDBLCLK) {
		if (pObjCtx->bCreateActive || pObjCtx->bEditActive)
			;	// no additional action
		else if (pObjCtx->pSelObj != NULL) {
			wParam = PlObjEditDialog(pPlot, 1, 0);
			if (wParam != '\0')
				SendMessage(pPlot->hwPlot, WM_KEYDOWN, wParam, 0);
		}
		else if (pPlot->pCursorFn == NULL)
			goto default_proc;
		else if (pMouse->bInOtherArea) {
			if (pPlot->bCursorVisible)
				CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
			CALL_CURS_FN PL_CURS_AREA, pMouse->coCurr.y.iRowCol,
													pMouse->coCurr.x.iRowCol);
		}
		else if (pMouse->bInCurrArea) {
			if (pPlot->bCursorVisible)
				CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
			CALL_CURS_FN PL_LMOUSE, pMouse->coCurr.x.dVal,
													pMouse->coCurr.y.dVal);
		}
		else
			goto default_proc;
	}
	else if (iMsg == WM_KILLFOCUS) {
		if (pPlot->bBorder)
			retStat = PlWinPaintBorder(pPlot->hwPlot, 0, 0);
		if (pPlot->pCursorFn != NULL && pPlot->bCursorVisible)
			CALL_CURS_FN PL_LOST_FOCUS, 0., 0.);
		else
			goto default_proc;
	}
	else if (iMsg == WM_SETFOCUS) {
		if (pPlot->bBorder)
			retStat = PlWinPaintBorder(pPlot->hwPlot, 0, 1);
		if (pPlot->pCursorFn != NULL && !pPlot->bCursorVisible)
			CALL_CURS_FN PL_GOT_FOCUS, 0., 0.);
		else
			goto default_proc;
	}
	else if (iMsg == WM_KEYDOWN) {
		PlWinProcKeydown(pPlot, hWnd, wParam);
		PlWinMousePreProcess(pPlot, hWnd, iMsg, wParam, lParam);
	}
	else if (iMsg == WM_GETDLGCODE)
		goto claim_keyboard;
	else if (iMsg == WM_HSCROLL)
		PlWinScroll_h(pPlot, wParam, lParam);
	else if (iMsg == WM_VSCROLL)
		PlWinScroll_v(pPlot, wParam, lParam);
	else if (iMsg != WM_SETCURSOR)
		goto default_proc;

	// If PlWinMousePreProcess wants a new cursor (or if pEventPreFn overrode
	// or set its own cursor), set it now.
	// BUGS:
	// 1.	The cursor has ghosts when moving the pointer around.  One
	//		source suggests the problem is a window whose class defines
	//		a cursor for the class; but PlWinCreate has a NULL cursor.
	//		I thought that the ghosts occurred only when using system
	//		cursors; it did't seem to matter whether I stored the handle
	//		for re-use or did a LoadCursor each time I wanted it.  But I
	//		created a facsimile of the IDC_ARROW cursor using the same
	//		technique that I use for creating the other custom cursors.
	//		The facsimile arrow also showed ghosts--which puzzles me,
	//		because I haven't seen the ghosts when switching, for example,
	//		between 'plus and box' and 'select'.  (I also tried CopyCursor
	//		using IDC_ARROW, with similar results.)
set_cursor:
	if (iMsg == WM_SETCURSOR)
		lresult = 1;
	if (pMouse->bUseNOTPointer)
		pMouse->hPointerNew = pMouse->hPointerNOT;
	else if (pMouse->hPointerEdit != 0)
		pMouse->hPointerNew = pMouse->hPointerEdit;
	if (iMsg == WM_SETCURSOR) {
		if (pMouse->hPointerNew != 0) {
			SetCursor(pMouse->hPointerNew);
			pMouse->hPointerCurr = pMouse->hPointerNew;
			pMouse->hPointerNew = 0;
		}
		else
			SetCursor(pMouse->hPointerCurr);
		goto done;
	}
	if (pMouse->hPointerNew != 0) {
		HCURSOR	hCursor=GetCursor();
		POINT	ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(pPlot->hwPlot, &ptMouse);
		if (ptMouse.x < 0 || ptMouse.x >= pPlot->rectPlot.right) goto done;
		if (ptMouse.y < 0 || ptMouse.y >= pPlot->rectPlot.bottom) goto done;
		SetCursor(pMouse->hPointerNew);
		pMouse->hPointerCurr = pMouse->hPointerNew;
		pMouse->hPointerNew = 0;
	}
done:
	if (pPlot->pEventPostFn != NULL) {
		LRESULT	lrslt1;
		lrslt1 = pPlot->pEventPostFn(pPlot, pPlot->pEventPostArg,
											hWnd, iMsg, wParam, lParam, 0);
		if (lrslt1 != 0)
			lresult = lrslt1;
	}
	return lresult;
default_proc:
	if (pPlot->pEventPostFn != NULL) {
		lresult = pPlot->pEventPostFn(pPlot, pPlot->pEventPostArg,
											hWnd, iMsg, wParam, lParam, 0);
		if (lresult != 0)
			return lresult;
	}
	lresult = DefWindowProc(hWnd, iMsg, wParam, lParam);
	return lresult;
claim_keyboard:
	// handling WM_GETDLGCODE allows the plot window to be used in Windows dialogs
	lresult = DLGC_WANTCHARS | DLGC_WANTARROWS;
	goto done;
}
/*+--------------------------------------------------------------------------
*
* Example:
*
*		This example demonstrates how the caller's event loop might
*		be modified to allow PL.LIB to process most of the messages
*		for the caller's plot window.  This mechanism is needed (and
*		allowed) only if the caller's plot window was not created using
*		PlWinCreate.
*
*		This example assumes that the handle to the plot window is
*		ghwMyPlotWin and that the PL_CTX structure is pointed to
*		by gpPlPlot.
*
*		LRESULT CALLBACK EXPORT
*		WndProc(
*		HWND	hWnd,
*		UINT	iMsg,
*		WPARAM	wParam,
*		LPARAM	lParam)
*		{
*			if (iMsg == WM_COMMAND) {
*				process messages
*			}
*			else if (hWnd == ghwMyPlotWin) {
*				if (iMsg == WM_...) {
*					process the message
*				}
*				else {
*					return PlWinProc_helper(gpPlPlot,
*							hWnd, iMsg, wParam, lParam);
*				}
*			}
*			else
*				goto default_proc;
*			return 0;
*		default_proc:
*			return DefWindowProc(hWnd, iMsg, wParam, lParam);
*		}
*
*--------------------------------------------------------------------------*/

/*+/csubr/TOC----------------------------------------------------------------
* PlWinProc_UW_event - event processing user-owned plot windows
*
* Purpose:
*		Emulates Windows events in response to events in user-owned plot
*		windows.
*
*		event:                  iEventInfo1
*		PL_UW_RESIZE
*		PL_UW_PAINT
*		PL_UW_FOCUS             0(lost) or 1(got)
*		PL_UW_KEY_DOWN          virtual key code
*
* Return Value:
*		void
*
* Notes:
* 1.	This routine is intended for use in environments where the normal
*		Windows event information is not available.  For environments where
*		that information is available, PlWinProc_helper should be used.
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-21-97 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlWinProc_UW_event(
PL_CTX *pPlot,			// IO pointer to plot context structure
int		eEvent,			// I event code, PL_UW_xxx
int		iEventInfo1,	// I information for the event
float	*pfScrollPos,	// IO scroll position for scroll events, 0
int		*peAction)		// O action code for caller
{
	HWND	hWnd=pPlot->hwPlot;
	int		bShift=(GetKeyState(VK_SHIFT) < 0) ? 1 : 0;
	int		bCtrl=(GetKeyState(VK_CONTROL) < 0) ? 1 : 0;
	UINT	iMsg=0;
	WPARAM	wParam=0;
	LPARAM	lParam=0L;

	if (pPlot == NULL || strcmp(pPlot->szIntInit, "yes") != 0)
		goto done;

	if (eEvent == PL_UW_RESIZE) {
		PlPlotClear(pPlot);
		iMsg = WM_SIZE;
		wParam = SIZENORMAL;
		goto call_helper;
	}
	else if (eEvent == PL_UW_PAINT)
		PlWinPaint(pPlot, 0);
	else if (eEvent == PL_UW_FOCUS) {
		if (iEventInfo1) iMsg = WM_SETFOCUS;
		else iMsg = WM_KILLFOCUS;
		goto call_helper;
	}
	else if (eEvent == PL_UW_KEY_DOWN) {
		iMsg = WM_KEYDOWN;
		wParam = iEventInfo1;
		goto call_helper;
	}

done:
	return;
call_helper:
	PlWinProc_helper(pPlot, hWnd, iMsg, wParam, lParam);
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinProc_UW_mouseEvent - mouse event processing user-owned plot windows
*
* Purpose:
*		Emulates Windows events in response to events in user-owned plot
*		windows.
*
*		event:                  iEventInfo1         iEventInfo2
*		PL_UW_MOUSE_UP          PL_UW_xMOUSE
*		PL_UW_MOUSE_DOWN        PL_UW_xMOUSE
*		PL_UW_MOUSE_DBL_CLICK   PL_UW_xMOUSE (see note)
*		PL_UW_MOUSE_MOVE        X position          Y position
*
* Return Value:
*		void
*
* Notes:
* 1.	This routine is intended for use in environments where the normal
*		Windows event information is not available.  For environments where
*		that information is available, PlWinProc_helper should be used.
* 2.	For PL_UW_MOUSE_DBL_CLICK, if iEventInfo1 is 0, then the currently
*		down button is taken to be the double-click button.
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-21-97 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlWinProc_UW_mouseEvent(
PL_CTX *pPlot,			// IO pointer to plot context structure
int		eEvent,			// I event code, PL_UW_xxx
int		iEventInfo1,	// I information for the event
int		iEventInfo2,	// I information for the event
int		*peAction)		// O action code for caller
{
	HWND	hWnd=pPlot->hwPlot;
	int		bShift=(GetKeyState(VK_SHIFT) < 0) ? 1 : 0;
	int		bCtrl=(GetKeyState(VK_CONTROL) < 0) ? 1 : 0;
	POINT	ptMouse;
	UINT	iMsg=0;
	WPARAM	wParam=0;
	LPARAM	lParam=0L;

	*peAction = 0;

	if (pPlot == NULL || strcmp(pPlot->szIntInit, "yes") != 0)
		goto done;

	if (eEvent == PL_UW_MOUSE_MOVE) {
		iMsg = WM_MOUSEMOVE;
		if (pPlot->pMouse->bLeftDown) wParam = MK_LBUTTON;
		else if (pPlot->pMouse->bMiddleDown) wParam = MK_MBUTTON;
		else wParam = MK_RBUTTON;
		if (bShift) wParam |= MK_SHIFT;
		if (bCtrl) wParam |= MK_CONTROL;
		lParam = MAKELONG(iEventInfo1, iEventInfo2);
		goto call_helper;
	}
	else if (eEvent == PL_UW_MOUSE_UP ||
				eEvent == PL_UW_MOUSE_DOWN || eEvent == PL_UW_MOUSE_DBL_CLICK) {
		if (iEventInfo1 == 0) {
			if (pPlot->pMouse->bLeftLast) iEventInfo1 = PL_UW_LMOUSE;
			else if (pPlot->pMouse->bMiddleLast) iEventInfo1 = PL_UW_MMOUSE;
			else iEventInfo1 = PL_UW_RMOUSE;
		}
		if (iEventInfo1 == PL_UW_LMOUSE) {
			if (eEvent == PL_UW_MOUSE_UP)
				iMsg = WM_LBUTTONUP;
			else if (eEvent == PL_UW_MOUSE_DBL_CLICK)
				iMsg = WM_LBUTTONDBLCLK, wParam = MK_LBUTTON;
			else
				iMsg = WM_LBUTTONDOWN, wParam = MK_LBUTTON;
		}
		else if (iEventInfo1 == PL_UW_MMOUSE) {
			if (eEvent == PL_UW_MOUSE_UP)
				iMsg = WM_MBUTTONUP;
			else if (eEvent == PL_UW_MOUSE_DBL_CLICK)
				iMsg = WM_MBUTTONDBLCLK, wParam = MK_MBUTTON;
			else
				iMsg = WM_MBUTTONDOWN, wParam = MK_MBUTTON;
		}
		else {
			if (eEvent == PL_UW_MOUSE_UP)
				iMsg = WM_RBUTTONUP;
			else if (eEvent == PL_UW_MOUSE_DBL_CLICK)
				iMsg = WM_RBUTTONDBLCLK, wParam = MK_RBUTTON;
			else
				iMsg = WM_RBUTTONDOWN, wParam = MK_RBUTTON;
		}
		if (bShift) wParam |= MK_SHIFT;
		if (bCtrl) wParam |= MK_CONTROL;
		GetCursorPos(&ptMouse);
		lParam = MAKELONG(ptMouse.x, ptMouse.y);
		// Need to find out if the cursor should be moved.
		PlWinProc_helper(pPlot, hWnd, iMsg, wParam, lParam);
		if (pPlot->pMouse->bPlotCursorClick)
			*peAction = PL_UW_CURSOR_MOVE;
	}

done:
	return;
call_helper:
	PlWinProc_helper(pPlot, hWnd, iMsg, wParam, lParam);
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinProcKeydown - process WM_KEYDOWN event
*
* Purpose:
*		Processes the various WM_KEYDOWN subevents.  The following
*		paragraphs list the keys that are processed directly by this
*		routine.  In addition, PlBboxKeyDown also processes keystrokes;
*		see the description for that routine for a list.
*
*		The following keys affect the vertical and horizontal
*		scroll bars.
*			HOME moves the thumb to the top
*			PAGEUP moves the thumb up one "page"
*			PAGEDOWN moves the thumb down one "page"
*			END moves the thumb to the bottom
*			SHIFT+HOME moves the thumb to the extreme left
*			SHIFT+PAGEUP moves the thumb to the left one "page"
*			SHIFT+PAGEDOWN moves the thumb to the right one "page"
*			SHIFT+END moves the thumb to the extreme right
*
*		The following arrow keys affect the plot cursor.
*			LEFT moves the cursor left 1 data point
*			RIGHT moves the cursor right 1 data point
*			SHIFT+LEFT moves the cursor left 20 data points
*			SHIFT+RIGHT moves the cursor right 20 data points
*			CTRL+SHIFT+LEFT moves the cursor left 1 "region"
*			CTRL+SHIFT+RIGHT moves the cursor right 1 "region"
*			UP moves the cursor to the data trace above
*			DOWN moves the cursor to the data trace below
*			CTRL+LEFT moves the cursor left 1 plot area array column
*			CTRL+RIGHT moves the cursor right 1 plot area array column
*			CTRL+UP moves the cursor up 1 plot area array row
*			CTRL+DOWN moves the cursor down 1 plot area array row
*
* Return Value:
*		void
*
* Notes:
* 1.	This routine is called automatically by PlWinProc_helper.
* 2.	This routine can be freely called by user programs.  It should
*		be noted, however, that the scroll-bar-related keys require
*		that the window contain scroll bars.  For this reason, Visual
*		Basic programs should use PlPicKeydown.
* 3.	If a scroll request is made for an axis that isn't zoomed, the
*		scroll request is suppressed.
*
* See Also:
*		PlWinProc_helper, PlWinRegCursorCallback, PlPicKeydown
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-17-95 R. Cole		created
* 08-16-95 R. Cole		changed to pass unprocessed keycodes to DefWindowProc
*--------------------------------------------------------------------------*/
void PASCAL
PlWinProcKeydown(
PL_CTX *pPlot,			// IO pointer to plot context structure
HWND	hwPlot,			// I handle for plot window
WPARAM	wParam)			// I virtual key code
{
	UINT	msg=0;
	WORD	wP=0;
	int		row, col;
	int		stat;
	int		shift=GetKeyState(VK_SHIFT) & 0x8000;
	int		ctrl=GetKeyState(VK_CONTROL) & 0x8000;

	if (pPlot->bNeedScale)
		PlWinScale(pPlot);

	stat = PlObjKeyDown(pPlot, wParam);
	if (stat == 1) goto done;

	if (wParam == VK_RIGHT) {
		if (pPlot->pCursorFn != NULL && pPlot->bCursorVisible) {
			if (ctrl != 0 && shift == 0) {
				if (pPlot->nAxisCols > 1) {
					CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
					col = pPlot->iAxisCol + 1;
					if (col >= pPlot->nAxisCols)
						col = pPlot->nAxisCols - 1;
					CALL_CURS_FN PL_CURS_AREA, pPlot->iAxisRow, col);
				}
			}
			else {
				CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
				if (ctrl && shift)
					CALL_CURS_FN PL_CURS_REGION, 1., 0.);
				else if (shift)
					CALL_CURS_FN PL_CURS_HORIZ, 20., 0.);
				else
					CALL_CURS_FN PL_CURS_HORIZ, 1., 0.);
			}
		}
	}
	else if (wParam == VK_LEFT) {
		if (pPlot->pCursorFn != NULL && pPlot->bCursorVisible) {
			if (ctrl != 0 && shift == 0) {
				if (pPlot->nAxisCols > 1) {
					CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
					col = pPlot->iAxisCol - 1;
					if (col < 0)
						col = 0;
					CALL_CURS_FN PL_CURS_AREA, pPlot->iAxisRow, col);
				}
			}
			else {
				CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
				if (ctrl && shift)
					CALL_CURS_FN PL_CURS_REGION, -1., 0.);
				else if (shift)
					CALL_CURS_FN PL_CURS_HORIZ, -20., 0.);
				else
					CALL_CURS_FN PL_CURS_HORIZ, -1., 0.);
			}
		}
	}
	else if (wParam == VK_DOWN) {
		if (pPlot->pCursorFn != NULL && pPlot->bCursorVisible) {
			if (!ctrl)
				CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
			if (ctrl) {
				if (pPlot->nAxisRows > 1) {
					CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
					row = pPlot->iAxisRow + 1;
					if (row >= pPlot->nAxisRows)
						row = pPlot->nAxisRows - 1;
					CALL_CURS_FN PL_CURS_AREA, row, pPlot->iAxisCol);
				}
			}
			else
				CALL_CURS_FN PL_CURS_VERT, -1., 0.);
		}
	}
	else if (wParam == VK_UP) {
		if (pPlot->pCursorFn != NULL && pPlot->bCursorVisible) {
			if (!ctrl)
				CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
			if (ctrl) {
				if (pPlot->nAxisRows > 1) {
					CALL_CURS_FN PL_CURS_ERASE, 0., 0.);
					row = pPlot->iAxisRow - 1;
					if (row < 0)
						row = 0;
					CALL_CURS_FN PL_CURS_AREA, row, pPlot->iAxisCol);
				}
			}
			else
				CALL_CURS_FN PL_CURS_VERT, 1., 0.);
		}
	}
	else if (wParam == VK_PRIOR) {
		msg = shift?WM_HSCROLL:WM_VSCROLL;
		wP = SB_PAGEUP;
	}
	else if (wParam == VK_NEXT) {
		msg = shift?WM_HSCROLL:WM_VSCROLL;
		wP = SB_PAGEDOWN;
	}
	else if (wParam == VK_HOME) {
		msg = shift?WM_HSCROLL:WM_VSCROLL;
		wP = SB_TOP;
	}
	else if (wParam == VK_END) {
		msg = shift?WM_HSCROLL:WM_VSCROLL;
		wP = SB_BOTTOM;
	}
	else
		DefWindowProc(hwPlot, WM_KEYDOWN, wParam, NULL);
done:
	if (msg == WM_VSCROLL && pPlot->pYAxis->fZoom <= 1.F) msg = 0;
	if (msg == WM_HSCROLL && pPlot->pXAxis->fZoom <= 1.F) msg = 0;
	if (msg != 0)
		PostMessage(hwPlot, msg, wP, 0);
	if (msg == WM_VSCROLL || msg == WM_HSCROLL) {
		// PlWinScroll_h and PlWinScroll_v don't go into action until a
		// SB_ENDSCROLL is received.
		PostMessage(hwPlot, msg, SB_ENDSCROLL, 0);
	}
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinRegCursorCallback - register "cursor callback" function
*
* Purpose:
*		Registers the function that will be called when the cursor needs to
*		be redrawn.  The function must have the following prototype (with
*		the choice of function name being arbitrary).
*
*		void MyCursorFn(
*		PL_CTX  *pPlot,      // IO pointer to plot context structure
*		void    *pCursorArg, // I value from PlWinRegCursorCallback call
*		void    *arg,        // I pointer to user structure
*		int     eEvent,      // I code for event causing call to function
*		double  dArg1,       // I informational argument, depends on eEvent
*		double  dArg2)       // I informational argument, depends on eEvent
*
*		The cursor callback function is called whenever PL.LIB needs it to
*		perform a cursor-related operation.  (Remember that the application
*		program contains the actual calls for painting the cursor.  PL.LIB
*		never paints the cursor directly, but always by calling the cursor
*		callback function to perform the desired operation.)
*
*		The eEvent argument indicates why the function was called.  For
*		some events, additional information is supplied in dArg1 and dArg2.
*		In most cases, cursor operations should be performed by calling
*		PlPaintCursor.
*
*		The values for eEvent, their meanings, and the corresponding values
*		of dArg1 and dArg2 are:
*
*		PL_PAINT--the cursor needs to be painted.  Draw the cursor.
*		PL_CURS_ERASE--the cursor needs to be erased.  Erase the cursor.
*		PL_GOT_FOCUS--the cursor isn't visible and the plot just got
*				focus.  Draw the cursor.
*		PL_LOST_FOCUS--the plot just lost focus.  Erase the cursor.
*		PL_LMOUSE--the left mouse button was clicked in the plot.  Draw
*				the cursor.
*				o	dArg1 has the X data value corresponding to mouse
*				o	dArg2 has the Y data value corresponding to mouse
*		PL_CURS_HORIZ--the cursor needs to move right or left.  Choose
*				the new data point, and draw the cursor.
*				o	dArg1 contains the number of data points to move,
*					with positive values indicating right and negative for
*					left.  The value will be plus or minus 1 to move one
*					point, or plus or minus 20 to jump several points.
*		PL_CURS_VERT--the cursor needs to move up or down.  Choose the
*				new data point, and draw the cursor.
*				o	dArg1 is plus or minus 1 to move up or down, respectively.
*		PL_CURS_REGION--the cursor needs to move to a new region.  Choose
*				the new region (and a data point within it), and draw the
*				cursor.  (If the plot isn't divided into regions, it is
*				recommended to either ignore this event or to treat it
*				the same as PL_CURS_HORIZ.)
*				o	dArg1 is plus or minus 1 to move right or left,
*					respectively.
*		PL_CURS_AREA--the cursor needs to move to a different column in
*				the plot area array.  Select the new plot area, choose the
*				new data point, and draw the cursor.
*				o	dArg1 has the plot area array row number
*				o	dArg2 has the plot area array column number
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	For a list of the keys that PL.LIB associates with cursor motion,
*		see the writeup for PlWinProcKeydown.
* 2.	When using a plot cursor, the programmer must take great care
*		that nothing ever gets drawn "on top" of the plot cursor.  If
*		it happens that a drawing operation (of whatever sort) changes
*		a pixel in the plot cursor, then it won't be possible for PL.LIB
*		to properly erase the cursor and stray pixels will accumulate.
*
*		To avoid this situation, it is important that the programmer
*		erase the cursor before performing any actions that change pixels
*		on the screen.  Likewise, it is important that the cursor be drawn
*		only after all pixel-changing operations are complete.
* 3.	To turn off cursor processing, call this routine with pCursorFn
*		set to NULL.
* 4.	Typically, pCursorArg is a pointer to a structure that the
*		application program uses for managing the data that is being
*		plotted.  Using this approach can avoid the need to use global
*		variables.
*
* See Also:
*		PlPaintCursor, PlVisibleCheck, PlWinCreate
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-24-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinRegCursorCallback(
PL_CTX	*pPlot,			// IO pointer to plot context structure
void (*pCursorFn)		// I user cursor callback function, or NULL
	(PL_CTX *, void *, int, double, double),
void	*pCursorArg)	// I arg for user call
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pPlot->hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, !IsWindow(pPlot->hwPlot), done)

	pPlot->pCursorFn = pCursorFn;
	pPlot->pCursorArg = pCursorArg;
	if (pCursorFn != NULL)
		pPlot->bEnbCursor = 1;
	else
		pPlot->bEnbCursor = 0;

done:
	PL_IF_INP_ERR("PlWinRegCursorCallback")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinRegEventCallback - register "event callback" function
*
* Purpose:
*		Registers the function that will be called when PL.LIB receives
*		a Windows event.
*
*		If 'bPre' is 1, then the user's function is called before PL.LIB
*		processes the event.  Otherwise, the user's function is called
*		after PL.LIB processes the event.  Two callback functions can
*		be registered by making two separate calls to this function, with
*		'bPre' set to 1 for one and 0 for the other.
*
*		The callback function must have the following prototype (with the
*		choice of function name being arbitrary).
*
*		LRESULT MyEventFn(
*		PL_CTX  *pPlot,     // IO pointer to plot context structure
*		void    *pEventArg, // I value from PlWinRegEventCallback call
*		HWND    hWnd,       // I handle for plot window
*		UINT    iMsg,       // I WM_xxx message code
*		WPARAM  wParam,     // I wParam for message
*		LPARAM  lParam,     // I lParam for message
*		int     bDone)      // I 1 if PL.LIB has already handled event
*
*		For the pre-processing call, the callback function can return:
*		o	non-zero if it has processed the message.  PL.LIB will not
*			process the message, but just passes the caller's non-zero
*			value to Windows.
*		o	zero if it has not processed the message.  PL.LIB will
*			process the message in its normal fashion (including
*			calling the post-processing callback, if one has been
*			registered).
*
*		For the post-processing call, if the callback function can return:
*		o	non-zero if it has processed the message.  PL.LIB will pass
*			the caller's non-zero value to Windows.
*		o	zero if it has not processed the message.  If PL.LIB had
*			already processed the message, then that status is returned
*			to Windows; otherwise, the message is sent to DefWindowProc.
*
*		The following conditions exist when the pre-processing callback
*		function is called:
*		o	pPlot->mouse has been updated (see the description of PL_MOUSE
*			in pl.h for full details).  Of particular interest to the pre-
*			processing callback are the following items:
*			-	For WM_LBUTTONDOWN events, a SetCapture() has already
*				been done.
*			-	For WM_LBUTTONUP events, a ReleaseCapture() has already
*				been done.
*			-	For WM_LBUTTONUP events, .bLeftClick indicates how this
*				mouse interaction has been classified.  1 indicates a
*				click, 0 indicates a drag.
*			-	For a variety of events (not limited to WM_SETCURSOR),
*				the .hCursorNew member indicates what PL.LIB intends to
*				do with the mouse pointer.  If .hCursorNew is 0, then
*				no change is intended for the mouse pointer.  If it is
*				non-zero, then PL.LIB intends to change the cursor; in
*				this case, .hCursorNew will be equal to one of the other
*				.hCursorXxx members.  The event pre-processing callback
*				can modify .hCursorNew to alter what PL.LIB does with
*				the cursor.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	Event callback functions aren't called when output is directed
*		to the printer.
* 2.	To turn off event processing, call this routine with pEventFn
*		set to NULL.
* 3.	Typically, pEventArg is a pointer to a structure that the
*		application program uses for managing the data that is being
*		plotted.  Using this approach can avoid the need to use global
*		variables.
*
* See Also:
*		PlWinCreate
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-08-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinRegEventCallback(
PL_CTX	*pPlot,			// IO pointer to plot context structure
LRESULT (*pEventFn)		// I user event callback function, or NULL
	(PPL_CTX, void *, HWND, UINT, WPARAM, LPARAM, int),
void	*pEventArg,		// I arg for user call
int		bPre)			// I 1 to register pre-processing callback
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pPlot->hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, !IsWindow(pPlot->hwPlot), done)

	if (bPre) {
		pPlot->pEventPreFn = pEventFn;
		pPlot->pEventPreArg = pEventArg;
	}
	else {
		pPlot->pEventPostFn = pEventFn;
		pPlot->pEventPostArg = pEventArg;
	}

done:
	PL_IF_INP_ERR("PlWinRegEventCallback")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinRegObjectCallback - register "plot object callback" function
*
* Purpose:
*		Registers the function that will be called when changes occur with
*		plot objects.  The function must have the following prototype
*		(with the choice of function name being arbitrary).
*
*		int MyObjectFn(
*		PL_CTX  *pPlot,       // IO pointer to plot context structure
*		void    *pObjArg,     // I value from PlWinRegObjectCallback call
*		PL_OBJ_CTX *pObjCtx,  // I pointer to PL_OBJ_CTX structure
*		PL_OBJ  *pObj,        // I pointer to object that has the event
*		int     eEvent,       // I code for event causing call to function
*		double  dArg1,        // I informational argument, depends on eEvent
*		double  dArg2)        // I informational argument, depends on eEvent
*
*		The object callback function is called by PL.LIB for a variety of
*		object-related events, as described below.  For all events, PL.LIB
*		performs all required drawing operations (in contrast to the cursor
*		callback function, which requires the application program to perform
*		all cursor painting operations).
*
*		For some object-related events, the return value from the callback
*		function is ignored and for others the return value has an effect.
*		The discussion for some events describe the effect of return value,
*		using a bulleted list item for "retVal".  For these cases, "retVal"
*		has the following meaning:
*			1	permit the operation to proceed to completion
*			0	veto the operation, or cause it to be aborted if it has
*				already begun
*		It is recommended that the the callback function return a value
*		of 1 for _all_ events, except for specific cases where it wants
*		to veto an operation.
*
*		When the object callback function is called:
*		pObjCtx  contains a pointer to the PL_OBJ_CTX structure.
*		pObj  contains a pointer to the object that is being processed.
*				Notes:
*				1.	pObj->eObjType contains the code for the type of
*					object, such as PL_OBJ_TY_BOX.  This member can be
*					used to decide how to cast pObj to the correct
*					type of object pointer.
*				2.	While an object is being edited, pObj points to a
*					_copy_ of of the object, not to the object itself.
*				3.	If dArg2 is PL_OBJ_EV_ABORT, pObj will be NULL.
*		eEvent  contains the code for the event, such as PL_OBJ_EV_DEL.
*
*		The following paragraphs describe each event, tell the meaning (if
*		any) that dArg1 and dArg2 have for the event, and describe what
*		action the application program is expected and/or allowed to take.
*
*		PL_OBJ_EV_DEL--the object is about to be deleted.
*				o	retVal--0 prevents the operation from occurring
*		PL_OBJ_EV_EDIT--something about an edit activity is about to change.
*				The object of that activity is the PL_OBJ pointed to by
*				pObj.  If the object is a box (i.e., pObj->eObjType is
*				PL_OBJ_TY_BOX), the size and position of the box can be
*				obtained (to give the operator tracking feedback, for
*				example) from pBox->aSides[n] (where pBox = (PL_BOX *)pObj).
*				The PL_OBJ's properties and coordinates can be examined
*				at any time; they can be modified when the PL_OBJ_EV_DONE
*				event is received.
*				o	dArg1 indicates the type of activity as one of:
*					-	PL_OBJ_EV_COPY--the PL_OBJ is being created as
*								a copy of another PL_OBJ
*					-	PL_OBJ_EV_CREATE--the PL_OBJ is being created
*					-	PL_OBJ_EV_MOVE--the PL_OBJ is being moved
*					-	PL_OBJ_EV_RESIZE--the PL_OBJ is being resized
*				o	dArg2 indicates the state of the activity as one of:
*					-	PL_OBJ_EV_BEGIN--the activity is about to begin
*					-	PL_OBJ_EV_COORD--the activity continues with new
*								coordinates (size or position)
*					-	PL_OBJ_EV_DONE--the activity is about to finish
*					-	PL_OBJ_EV_ABORT--the activity is about to abort
*								(pObj will be NULL)
*				o	retVal--retVal == 0 causes the edit activity to abort
*								(a PL_OBJ_EV_ABORT event will occur if the
*								activity is already in progress)
*		PL_OBJ_EV_SEL--the pObj object has been selected or is about to be
*				deselected.
*				o	dArg1--is 1 or 0 for select or deselect, respectively
*				o	retVal--0 prevents the select/deselect from occurring
*		PL_MOUSE_MOVE--the mouse pointer position has changed.  The new
*				position is pPlot->mouse.coCurr.  This can be used
*				to give the operator tracking feedback while the mouse
*				is moving.
*				o	pObj--is NULL
*				o	dArg1--is 1 if an edit/create is active.  (Normally
*							mouse motion wouldn't be echoed to the operator
*							while doing a edit/create is active.  Instead,
*							the object coordinates would be shown to the
*							operator when PL_OBJ_EV_EDIT events occur.)
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	To turn off calls to the object callback function, call this routine
*		with pObjectFn set to NULL.  (Note also that some PlObjXxx functions
*		have a bNoCallback argument; if bNoCallback is 1, then the object
*		callback function is skipped on a one-time basis.)
* 2.	Typically, pObjectArg is a pointer to a structure that the
*		application program uses for managing the data that is being
*		plotted.  Using this approach can avoid the need to use global
*		variables.
*
* See Also:
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-05-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinRegObjectCallback(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int (*pObjectFn)		// I user plot object callback function, or NULL
	(PL_CTX *, void *, PL_OBJ_CTX *, PL_OBJ *, int, double, double),
void	*pObjectArg)	// I arg for user call
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pPlot->hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, !IsWindow(pPlot->hwPlot), done)

	pPlot->pObjectFn = pObjectFn;
	pPlot->pObjectArg = pObjectArg;

done:
	PL_IF_INP_ERR("PlWinRegObjectCallback")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinRegPlotCallback - register "plot callback" function
*
* Purpose:
*		Registers the function that will be called when the plot needs to
*		be redrawn.  The most common reason for calling the function is
*		the occurrence of a WM_REPAINT event.
*
*		The callback function has full responsibility for drawing the plot.
*
*		The callback function must have the following prototype (with the
*		choice of function name being arbitrary).
*
*		void MyPlotFn(
*		PL_CTX  *pPlot,     // IO pointer to plot context structure
*		void    *pPlotArg)  // I value from PlWinRegPlotCallback call
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	To turn off plot callback processing, call this routine with pPlotFn
*		set to NULL.
* 2.	Typically, pPlotArg is a pointer to a structure that the
*		application program uses for managing the data that is being
*		plotted.  Using this approach can avoid the need to use global
*		variables.
*
* See Also:
*		PlWinCreate
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-24-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinRegPlotCallback(
PL_CTX	*pPlot,			// IO pointer to plot context structure
void (*pPlotFn)(PL_CTX *, void *),// I user plotting callback function
void	*pPlotArg)		// I arg for user call
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pPlot->hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, !IsWindow(pPlot->hwPlot), done)

	pPlot->pPlotFn = pPlotFn;
	pPlot->pPlotArg = pPlotArg;

done:
	PL_IF_INP_ERR("PlWinRegPlotCallback")
	return retStat;
}

/*+/csubr/TOC----------------------------------------------------------------
* PlWinRepaint - trigger a repaint for the plot window
*
* Purpose:
*		Triggers a repaint for the plot window.  This is useful to avoid
*		multiple repaints.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	The repaint that is triggered by this function is "deferred" if
*		plotting to the screen.  That is, the plot window will be repainted
*		only when Windows schedules the repaint.  This means that if there
*		have been several calls to PlWinRepaint, the plot window will be
*		repainted only once.
*
* See Also:
*		PlWinZoom, PlWinCreate
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-03-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinRepaint(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*		if pPlot isn't for a printer, then
* 2			pPlot->hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)

	if (pPlot->bPrinter)
		PlWinPaint(pPlot, pPlot->hDC);
	else {
		PL_INP_CHK(2, !IsWindow(pPlot->hwPlot), sub_exit)
		InvalidateRect(pPlot->hwPlot, NULL, TRUE);
	}

sub_exit:
	PL_IF_INP_ERR("PlWinRepaint")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinScale - adjust the scaling for the plot window
*
* Purpose:
*		Recalculates the scaling for the plot window.  This routine
*		must be called whenever the relationship between plot window
*		size and data range changes.  This will be whenever:
*		o	the plot window is resized, or
*		o	the title style for the plot window changes, or
*		o	the title or annotation styles for a plot area change, or
*		o	the plot area array is reconfigured, or
*		o	the X data range for a plot area changes, or
*		o	the Y data range for a plot area changes, or
*		o	horizontal or vertical zoom factor for the window changes, or
*		o	the scroll position for the window changes.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Side Effects:
*		o	recalculates the logical scroll position
*
* Notes:
* 1.	This routine automatically calls PlWinScroll.
* 2.	This routine is called automatically by PL.LIB when necessary;
*		only in unusual circumstances would an application program need
*		to directly call this routine.
*
* BUGS:
* 1.	Rather than actually determining the width of annotation characters,
*		this routine assumes the width is .5 of the height.
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-20-95 R. Cole		created
* 08-17-95 R. Cole		changed to accomodate arrays of plot areas,
*						rotated annotations, and plot area titles above
*						or to the right of the plot area
*--------------------------------------------------------------------------*/
int PASCAL
PlWinScale(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	int		retStat=PL_OK, inpErr=0;
	int		iDeltaX, iDeltaY;
	int		row, col;
	int		nRows, nCols;
	int		iRowSave, iColSave;
	int		iPlotWid_px, iPlotHt_px;
	int		iAreaL_px, iAreaR_px, iDrawWid_px;
	int		iAreaT_px, iAreaB_px, iDrawHt_px;
	float	fPts, fHorizPtsSum, fVertPtsSum;
	int		iHorizSpace_px, iVertSpace_px;
	HDC		hDC=0;
	RECT	rectPlot;
	typedef struct {
		float	fPtsBMax;		// bottom (left) title space (pts)
		float	fPtsTMax;		// top (right) title space (pts)
		float	fPtsAMax;		// annotation space (pts)
		int		pxDrawB;		// bottom (left) of drawing area
		int		pxDrawT;		// top (right) of drawing area
		int		pxAreaB;		// bottom (left) of area plus annot.
		int		pxAreaT;		// top (right) of area plus annot.
		float	fArrayPosFrac;	// X or Y loc--frac of window, or -1
		float	fArraySizeFrac;	// ht or wid--frac of window, or -1
	} INFO;
	INFO	*pVertInfo=NULL, *pHorzInfo=NULL;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit)

	if (pPlot->hDC == 0) hDC = GetDC(pPlot->hwPlot);
	else hDC = pPlot->hDC;
	if (hDC == 0) goto gdi_error;

	if (pPlot->hwPlot != NULL) {
		int		xBorder, yBorder;
		// Need to get fresh rect because scroll bars may have
		// appeared or disappeared.
		GetClientRect(pPlot->hwPlot, &pPlot->rectPlot);
		if (pPlot->bBorder) {
			xBorder = 1 + 2*GetSystemMetrics(SM_CXBORDER);
			yBorder = 1 + 2*GetSystemMetrics(SM_CYBORDER);
			pPlot->rectPlot.left += xBorder;
			pPlot->rectPlot.right -= xBorder;
			pPlot->rectPlot.top += yBorder;
			pPlot->rectPlot.bottom -= yBorder;
		}
	}
	rectPlot = pPlot->rectPlot;
	// Get spacing, in pixels, between rows and columns.
	iHorizSpace_px = PL_XPTS_TO_XPIX(pPlot->fHorizPts);
	iVertSpace_px = PL_YPTS_TO_YPIX(pPlot->fVertPts);

	// Now carve away the space that will be used for the plot window
	// titles.
	if (pPlot->fTitlePts > 0.F) {
		iDeltaX = PL_XPTS_TO_XPIX(pPlot->fTitlePts + 2.F);
		iDeltaY = PL_YPTS_TO_YPIX(pPlot->fTitlePts + 2.F);
		rectPlot.top += pPlot->nTitleLinesTop * iDeltaY;
		rectPlot.bottom -= pPlot->nTitleLinesBottom * iDeltaY;
		rectPlot.left += pPlot->nTitleLinesLeft * iDeltaX;
		rectPlot.right -= pPlot->nTitleLinesRight * iDeltaX;
	}

/*---------------------------------------------------------------------------
*		Set up the margins for the actual plot areas, based on the 'rect'
*		and on the title and annotation styles.
*--------------------------------------------------------------------------*/
	nRows = pPlot->nAxisRows;
	nCols = pPlot->nAxisCols;
	iRowSave = pPlot->iAxisRow;
	iColSave = pPlot->iAxisCol;
	pVertInfo = (INFO *)malloc(nRows * sizeof(INFO));
	pHorzInfo = (INFO *)malloc(nCols * sizeof(INFO));
	if (pVertInfo == NULL || pHorzInfo == NULL) {
		retStat = PL_MALLOC_FAIL;
		goto done;
	}
	for (row=0; row<nRows; row++) {
		pVertInfo[row].fPtsBMax =
			pVertInfo[row].fPtsTMax = pVertInfo[row].fPtsAMax = 0.F;
	}
	for (col=0; col<nCols; col++) {
		pHorzInfo[col].fPtsBMax =
			pHorzInfo[col].fPtsTMax = pHorzInfo[col].fPtsAMax = 0.F;
	}
	// The first step is to find the largest amount of space occupied
	// by annotations and titles--a max value for horizontal space that
	// will need to be carved away, and a max value for vertical.

	// For each column, go through all the rows to find the max
	// horiz space used for left titles, right titles, and annot.
	for (col=0; col<nCols; col++) {
		for (row=0; row<nRows; row++) {
			PlAreaArraySelect(pPlot, row, col);
			fPts = (pPlot->pYAxis->fTitlePts + 2.F) * pPlot->pYAxis->nTitleLinesT;
			if (fPts > pHorzInfo[col].fPtsTMax)
				pHorzInfo[col].fPtsTMax = fPts;
			fPts = (pPlot->pYAxis->fTitlePts + 2.F) * pPlot->pYAxis->nTitleLinesB;
			if (fPts > pHorzInfo[col].fPtsBMax)
				pHorzInfo[col].fPtsBMax = fPts;
			if (pPlot->pYAxis->fAnnotPts > 0.F) {
				switch (pPlot->pYAxis->eStyle) {
					case PL_LINE_AXIS:
					case PL_TICK_AXIS:
					case PL_GRID_AXIS:
					case PL_GRID_DOT_AXIS:
					case PL_GRID_DASH_AXIS:
					case PL_EDGE_HALF_AXIS:
						if (pPlot->pYAxis->fAnnotDeg == 90.F)
							fPts = pPlot->pYAxis->fAnnotPts;
						else {
							fPts = pPlot->pYAxis->iAnnotNChar *
									(.5F * pPlot->pYAxis->fAnnotPts);
						}
						break;
					default:
						fPts = 0.F;
				}
			}
			else
				fPts = 0.F;
			if (fPts > pHorzInfo[col].fPtsAMax)
				pHorzInfo[col].fPtsAMax = fPts;
		}
	}
	// For each row, go through all the columns to find the max
	// vertical space used for top titles, bottom titles, and annot.
	for (row=0; row<nRows; row++) {
		for (col=0; col<nCols; col++) {
			PlAreaArraySelect(pPlot, row, col);
			fPts = (pPlot->pXAxis->fTitlePts + 2.F) * pPlot->pXAxis->nTitleLinesT;
			if (fPts > pVertInfo[row].fPtsTMax)
				pVertInfo[row].fPtsTMax = fPts;
			fPts = (pPlot->pXAxis->fTitlePts + 2.F) * pPlot->pXAxis->nTitleLinesB;
			if (fPts > pVertInfo[row].fPtsBMax)
				pVertInfo[row].fPtsBMax = fPts;
			if (pPlot->pXAxis->fAnnotPts > 0.F) {
				switch (pPlot->pXAxis->eStyle) {
					case PL_LINE_AXIS:
					case PL_TICK_AXIS:
					case PL_GRID_AXIS:
					case PL_GRID_DOT_AXIS:
					case PL_GRID_DASH_AXIS:
					case PL_EDGE_HALF_AXIS:
						if (pPlot->pXAxis->fAnnotDeg == 0.F)
							fPts = pPlot->pXAxis->fAnnotPts;
						else {
							fPts = pPlot->pXAxis->iAnnotNChar *
									(.5F * pPlot->pXAxis->fAnnotPts);
						}
						break;
					default:
						fPts = 0.F;
				}
			}
			else
				fPts = 0.F;
			if (fPts > pVertInfo[row].fPtsAMax)
				pVertInfo[row].fPtsAMax = fPts;
		}
	}

	// Go through all the columns to find the total amount of horizontal
	// space used for left titles, right titles, and annot.  Also, get
	// the horizontal fraction of window width that will be occupied
	// by each column.
	fHorizPtsSum = (nCols - 1) * pPlot->fHorizPts;
	for (col=0; col<nCols; col++) {
		PlAreaArraySelect(pPlot, 0, col);
		fHorizPtsSum += pHorzInfo[col].fPtsBMax +
							pHorzInfo[col].fPtsTMax + pHorzInfo[col].fPtsAMax;
		pHorzInfo[col].fArrayPosFrac = pPlot->pXAxis->fArrayPosFrac;
		pHorzInfo[col].fArraySizeFrac = pPlot->pXAxis->fArraySizeFrac;
	}
	// Go through all the rows to find the total amount of vertical
	// space used for bottom titles, top titles, and annot.  Also, get
	// the vertical fraction of window width that will be occupied
	// by each row.
	fVertPtsSum = (nRows - 1) * pPlot->fVertPts;
	for (row=0; row<nRows; row++) {
		PlAreaArraySelect(pPlot, row, 0);
		fVertPtsSum += pVertInfo[row].fPtsBMax +
							pVertInfo[row].fPtsTMax + pVertInfo[row].fPtsAMax;
		pVertInfo[row].fArrayPosFrac = pPlot->pYAxis->fArrayPosFrac;
		pVertInfo[row].fArraySizeFrac = pPlot->pYAxis->fArraySizeFrac;
	}

/*---------------------------------------------------------------------------
*		Now, figure out how big the "grid area" is.  The objective
*		of all the fussing above is to make the grid area the same
*		height for all columns in a row and the same width for all
*		rows in a column.
*--------------------------------------------------------------------------*/
	// First, establish the parameters for the upper left plot area.  As
	// the process of laying out the plot areas progresses, the position
	// parameters will be changed; the height and width are constant.
	iAreaL_px = rectPlot.left;
	iAreaR_px = rectPlot.right;
	iDrawWid_px = (iAreaR_px - iAreaL_px - PL_XPTS_TO_XPIX(fHorizPtsSum)) / nCols;
	iAreaT_px = rectPlot.top;
	iAreaB_px = rectPlot.bottom;
	iDrawHt_px = (iAreaB_px - iAreaT_px - PL_YPTS_TO_YPIX(fVertPtsSum)) / nRows;
	iPlotWid_px = rectPlot.right - rectPlot.left;
	iPlotHt_px = rectPlot.bottom - rectPlot.top;

	// Next, set the horizontal and vertical values for the various
	// parameters for each row and column.
	for (row=0; row<nRows; row++) {
		// If PlAreaMove has been called, handle the layout special.
		if (pVertInfo[row].fArraySizeFrac > 0.F) {
			iAreaT_px = rectPlot.top +
							(int)(pVertInfo[row].fArrayPosFrac * iPlotHt_px);
			iDrawHt_px = (int)(iPlotHt_px * pVertInfo[row].fArraySizeFrac);
			iDrawHt_px -= PL_YPTS_TO_YPIX(pVertInfo[row].fPtsTMax +
							pVertInfo[row].fPtsBMax + pVertInfo[row].fPtsAMax);
			// Adjust for spacing between plot areas.
			if (row == 0) {
				if (nRows > 0)
					iDrawHt_px -= iVertSpace_px / 2;
			}
			else if (row == nRows-1) {
				iAreaT_px += iVertSpace_px / 2;
				iDrawHt_px -= iVertSpace_px / 2;
			}
			else {
				iAreaT_px += iVertSpace_px / 2;
				iDrawHt_px -= iVertSpace_px;
			}
		}
		// Now establish the vertical positions and sizes for the
		// overall plot areas and the drawing (i.e., grid) areas.
		pVertInfo[row].pxDrawT = iAreaT_px +
							PL_YPTS_TO_YPIX(pVertInfo[row].fPtsTMax);
		pVertInfo[row].pxDrawB = iDrawHt_px + pVertInfo[row].pxDrawT - 1;
		pVertInfo[row].pxAreaT = iAreaT_px;
		iAreaT_px = pVertInfo[row].pxDrawB + PL_YPTS_TO_YPIX(
							pVertInfo[row].fPtsBMax + pVertInfo[row].fPtsAMax);
		pVertInfo[row].pxAreaB = iAreaT_px - 1;
		iAreaT_px += iVertSpace_px;
	}
	for (col=0; col<nCols; col++) {
		// If PlAreaMove has been called, handle the layout special.
		if (pHorzInfo[col].fArraySizeFrac > 0.F) {
			iAreaL_px = rectPlot.left +
							(int)(pHorzInfo[col].fArrayPosFrac * iPlotWid_px);
			iDrawWid_px = (int)(iPlotWid_px * pHorzInfo[col].fArraySizeFrac);
			iDrawWid_px -= PL_XPTS_TO_XPIX(pHorzInfo[col].fPtsTMax +
							pHorzInfo[col].fPtsBMax + pHorzInfo[col].fPtsAMax);
			// Adjust for spacing between plot areas.
			if (col == 0) {
				if (nCols > 0)
					iDrawWid_px -= iHorizSpace_px / 2;
			}
			else if (col == nCols-1) {
				iAreaL_px += iHorizSpace_px / 2;
				iDrawWid_px -= iHorizSpace_px / 2;
			}
			else {
				iAreaL_px += iHorizSpace_px / 2;
				iDrawWid_px -= iHorizSpace_px;
			}
		}
		// Now establish the horizontal positions and sizes for the
		// overall plot areas and the drawing (i.e., grid) areas.
		pHorzInfo[col].pxDrawB = iAreaL_px + PL_XPTS_TO_XPIX(
						pHorzInfo[col].fPtsBMax + pHorzInfo[col].fPtsAMax);
		pHorzInfo[col].pxDrawT = iDrawWid_px + pHorzInfo[col].pxDrawB - 1;
		pHorzInfo[col].pxAreaB = iAreaL_px;
		pHorzInfo[col].pxAreaT = iDrawWid_px + pHorzInfo[col].pxDrawB - 1;
		iAreaL_px = pHorzInfo[col].pxDrawT +
						PL_XPTS_TO_XPIX(pHorzInfo[col].fPtsTMax);
		pHorzInfo[col].pxAreaT = iAreaL_px - 1;
		iAreaL_px += iHorizSpace_px;
	}

	// Finally!  Set the appropriate values for each plot area.
	for (row=0; row<nRows; row++) {
		for (col=0; col<nCols; col++) {
			PlAreaArraySelect(pPlot, row, col);
			pPlot->pXAxis->pxAreaB = pHorzInfo[col].pxAreaB;
			pPlot->pXAxis->pxAreaT = pHorzInfo[col].pxAreaT;
			pPlot->pYAxis->pxAreaT = pVertInfo[row].pxAreaT;
			pPlot->pYAxis->pxAreaB = pVertInfo[row].pxAreaB;
			pPlot->pXAxis->pxB = pHorzInfo[col].pxDrawB;
			pPlot->pXAxis->pxT = pHorzInfo[col].pxDrawT;
			pPlot->pYAxis->pxT = pVertInfo[row].pxDrawT;
			pPlot->pYAxis->pxB = pVertInfo[row].pxDrawB;
			pPlot->pXAxis->pxB += 1;
			pPlot->pXAxis->pxT -= 1;
			pPlot->pYAxis->pxT += 1;
			pPlot->pYAxis->pxB -= 1;

			retStat = PlWinScaleAxis(pPlot, pPlot->pXAxis);
			if (retStat != PL_OK) goto done;
			retStat = PlWinScaleAxis(pPlot, pPlot->pYAxis);
			if (retStat != PL_OK) goto done;
		}
	}
	PlAreaArraySelect(pPlot, iRowSave, iColSave);
	pPlot->bNeedScale = 0;

done:
	if (pVertInfo != NULL) free(pVertInfo);
	if (pHorzInfo != NULL) free(pHorzInfo);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
sub_exit:
	PL_IF_INP_ERR("PlWinScale")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlWinScaleAxis - adjust the scaling for an axis
*
* Purpose:
*		Recalculates the scaling for a plot axis.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Side Effects:
*		o	recalculates the logical scroll position
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-20-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinScaleAxis(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_AXIS	*pAxis)			// IO pointer to axis structure
{
	int		retStat=PL_OK;
	double	dDeltaPx;		// axis range in pixels
	double	dDeltaLVal;		// axis range in data (log) values
	double	dPxPerLVal;

	dDeltaPx = pAxis->pxT - pAxis->pxB;
	dDeltaLVal = pAxis->dLValT - pAxis->dLValB;

	// What we're after is a scale factor such that:
	//	pxB will be the result of the calculation:
	//		(int)(.5 + pxB + (dValT - dValB) * scale)
	//	pxT will be the result of the calculation:
	//		(int)(.5 + pxB + (dValT - dValB) * scale)
	if (dDeltaLVal != 0.)
		dPxPerLVal = pAxis->fZoom * dDeltaPx / dDeltaLVal;
	else
		dPxPerLVal = 1.;
	pAxis->dPxPerLVal = dPxPerLVal;
	retStat = PlWinScrollAxis(pPlot, pAxis);

	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinScroll - change logical scroll position
*
* Purpose:
*		Changes the logical scroll position for a plot window.  The
*		logical scroll position affects the mapping between the data
*		and the screen.
*
*		This routine is intended to be called when scroll bar events
*		have occurred, so that the logical scroll position stays in sync
*		with the scroll bars.  This routine does NOT adjust scroll bar
*		positions.
*
*		If a plot area array exists, each plot area is scrolled.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the arguments is invalid
*
* Side Effects:
* 1.	If the logical scroll position for any axis has actually changed,
*		then this routine sets pPlot->bNeedScale to 1.  This will cause
*		other PL.LIB routines to recalculate the mapping betwen the
*		data and the screen.  If none of the axes have their logical
*		scroll position altered, then pPlot->bNeedScale is unchanged.
*
* Notes:
* 1.	This routine doesn't trigger a repaint for the plot window.
* 2.	This routine is called automatically by PlWinZoom.
*
* See Also:
*		PlWinCreate, PlWinZoom
*		PlWinScroll_h, PlWinScroll_v
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-20-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinScroll(
PL_CTX	*pPlot,			// IO pointer to plot context structure
float	fXScroll,		// I X scroll position, between 0 and 1, inclusive
float	fYScroll)		// I Y scroll position, between 0 and 1, inclusive
{
	int		retStat=PL_OK, inpErr=0;
	int		iRow, iCol, iRowSave, iColSave;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)

	iRowSave = pPlot->iAxisRow;
	iColSave = pPlot->iAxisCol;

	for (iRow=0; iRow<pPlot->nAxisRows; iRow++) {
		for (iCol=0; iCol<pPlot->nAxisCols; iCol++) {
			retStat = PlAreaArraySelect(pPlot, iRow, iCol);
			if (retStat != PL_OK) goto done;
			if (pPlot->pXAxis->fScroll != fXScroll || pPlot->bZoomChanged) {
				pPlot->bNeedScale = 1;
				pPlot->pXAxis->fScroll = fXScroll;
				retStat = PlWinScrollAxis(pPlot, pPlot->pXAxis);
				if (retStat != PL_OK) goto done;
			}
			if (pPlot->pYAxis->fScroll != fYScroll || pPlot->bZoomChanged) {
				pPlot->bNeedScale = 1;
				pPlot->pYAxis->fScroll = fYScroll;
				retStat = PlWinScrollAxis(pPlot, pPlot->pYAxis);
				if (retStat != PL_OK) goto done;
			}
		}
	}

	retStat = PlAreaArraySelect(pPlot, iRowSave, iColSave);
	if (retStat != PL_OK) goto done;

done:
	PL_IF_INP_ERR("PlWinScroll")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlWinScrollAxis - apply logical scroll position to axis
*
* Purpose:
*		Changes the logical scroll position for a plot window.  The
*		logical scroll position affects the mapping between the data
*		and the screen.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the arguments is invalid
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-20-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinScrollAxis(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_AXIS	*pAxis)			// IO pointer to axis structure
{
	int		retStat=PL_OK;
	double	yB=pAxis->dLValB, yT=pAxis->dLValT;
	double	dAxisLength;	// axis length in data values
	double	dValMin, dValMax;

	dAxisLength = yT - yB;

	if (pAxis->fZoom <= 1. || dAxisLength == 0.) {
		pAxis->dLValBS = yB;
		pAxis->dLValTS = yT;
	}
	else {
		pAxis->dLValBS = yB + pAxis->fScroll *
								(dAxisLength - dAxisLength / pAxis->fZoom);
		pAxis->dLValTS = pAxis->dLValBS + dAxisLength / pAxis->fZoom;
	}
	dValMin = PL_MIN(pAxis->dLValBS, pAxis->dLValTS);
	dValMax = PL_MAX(pAxis->dLValBS, pAxis->dLValTS);
	if (!pPlot->bCursorVisible && (pAxis->dDValAnchor < dValMin ||
										pAxis->dDValAnchor > dValMax)) {
		pAxis->dDValAnchor = pAxis->dLValBS;
		if (pAxis->eScaleType == PL_LOG)
			pAxis->dDValAnchor = pow(10., pAxis->dDValAnchor);
		pAxis->fAnchorFrac = .5F;
	}

	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinScroll_h - process horizontal scroll bar messages
*
* Purpose:
*		This routine processes WM_HSCROLL messages for the plot window.
*
*		Typically, this routine would not be called by a program--it
*		would use PlWinProc_helper, which calls this routine.
*
*		A keyboard interface to horizontal scrolling is provided by
*		PlWinProc_helper:
*
*			SHIFT+HOME moves the thumb to the extreme left
*			SHIFT+PAGEUP moves the thumb to the left
*			SHIFT+PAGEDOWN moves the thumb to the right
*			SHIFT+END moves the thumb to the extreme right
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Side Effects:
*		o	The scroll bar position is updated.
*		o	The logical scroll position in PL_CTX is updated.
*		o	The plot window is given the input focus.
*		o	Causes the plot window to be repainted
*
* Notes:
* 1.	SB_LINEUP and SB_LINEDOWN events move the thumb left and right
*		1/nMajor (or 1/8, if nMajor is < 2) of the window width.
* 2.	SB_PAGEUP and SB_PAGEDOWN events move the thumb left and right
*		a full window width.
* 3.	SB_TOP and SB_BOTTOM events move the thumb to the extreme left
*		and right.
*
* BUGS:
* 1.	Scrolling is handled by repainting the entire plot window.  An
*		alternative would be to scroll the current contents and paint
*		only the newly exposed region.
*
* See Also:
*		PlWinScroll, PlWinScroll_v
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinScroll_h(
PL_CTX	*pPlot,			// IO pointer to plot context structure
WPARAM	wParam,			// I wParam arg from windows scroll event
LPARAM	lParam)			// I lParam arg from windows scroll event
{
	int		retStat=PL_OK, inpErr=0;
	HWND	hwPlot;
	float	fXScroll, fZoom, fLineFrac;
	int		bRepaint=0;
	UINT	xnewPos;
	POINT	pt;
	RECT	RclHorz;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pPlot->hwPlot must be a valid window
* 3		pPlot->pXAxis->fScroll must be between 0 and 1, inclusive
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	hwPlot = pPlot->hwPlot;
	PL_INP_CHK(2, !IsWindow(hwPlot), done)
	fXScroll = pPlot->pXAxis->fScroll;
	PL_INP_CHK(3, fXScroll < 0.F || fXScroll > 1.F, done)
	fXScroll = .001F * GetScrollPos(hwPlot, SB_HORZ);

	if ((fZoom = pPlot->pXAxis->fZoom) <= 1.F) goto done;

	if (pPlot->pXAxis->nMajor > 1)
		fLineFrac = 1.F / pPlot->pXAxis->nMajor;
	else
		fLineFrac = .125F;
	/*if (wParam == SB_TOP) fXScroll = 0.F;
	else if (wParam == SB_BOTTOM) fXScroll = 1.F;
	else if (wParam == SB_LINEUP) fXScroll -= fLineFrac / (fZoom-1.F);
	else if (wParam == SB_LINEDOWN) fXScroll += fLineFrac / (fZoom-1.F);
	else if (wParam == SB_PAGEUP) fXScroll -= 1.F / (fZoom-1.F);
	else if (wParam == SB_PAGEDOWN) fXScroll += 1.F / (fZoom-1.F);
	else if (wParam == SB_THUMBPOSITION) fXScroll = LOWORD(lParam) / 1000.F;
	else if (wParam == SB_ENDSCROLL) bRepaint = 1;
	else
		;	*/   // no action


	// Changed to switch statments to make horizontal scroll work.
	// K.G 9/27/99
	switch (LOWORD(wParam)) 
	{
	    case SB_TOP:
			fXScroll = 0.F;
			break;

		case SB_BOTTOM:
			fXScroll = 1.F;
			break;

		case SB_LINEUP:
			fXScroll -= fLineFrac / (fZoom-1.F);
			break;

		case SB_LINEDOWN:
			fXScroll += fLineFrac / (fZoom-1.F);
			break;

		case SB_PAGEUP:
			fXScroll -= 1.F / (fZoom-1.F);
			break;

		case SB_PAGEDOWN:
			fXScroll += 1.F / (fZoom-1.F);
			break;

		case SB_THUMBPOSITION:
			double dl;
				
			GetCursorPos(&pt);//Get the current cursor position.
			ScreenToClient(hwPlot, &pt);//Map the point of the cursor to the scroll window.
			dl = (double)pt.x * (double)1000;//Multiply by the maximum horizontal position
											 //and store in a double to keep from overflowing.
			GetClientRect(hwPlot, &RclHorz);//Get the client rectangle of the horizontal scroll bar.
			xnewPos = (UINT)(dl / (double)RclHorz.right);//Divide by the right side of the scroll window.
			
			//Sets the position of the scroll box (thumb) in the horizontal scroll bar and, 
			//redraws the scroll bar to reflect the new position of the scroll box. 
			SetScrollPos(hwPlot, SB_HORZ, xnewPos, TRUE);

			break;

		case SB_ENDSCROLL:
			bRepaint = 1;
			break;
	}

	if (wParam != SB_THUMBPOSITION) {  //Added this line.  K.G 9/27/99
	 if (fXScroll < 0.) fXScroll = 0.F;
	 if (fXScroll > 1.) fXScroll = 1.F;
	 if (fXScroll != pPlot->pXAxis->fScroll) {
	    SetScrollPos(hwPlot, SB_HORZ, (int)((fXScroll)*1000.F), TRUE);
		pPlot->bNeedScale = 1;
		if (bRepaint) {
			retStat = PlWinScroll(pPlot, fXScroll, pPlot->pYAxis->fScroll);
			if (retStat != PL_OK) goto done;
			InvalidateRect(hwPlot, NULL, TRUE);
			if (GetFocus() != hwPlot)
				SetFocus(hwPlot);
		}
	 }
	}

done:
	PL_IF_INP_ERR("PlWinScroll_h")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinScroll_pos - set logical scroll position and scrollbar position
*
* Purpose:
*		Changes the logical scroll position for a plot window so that
*		a particular data value appears at a particular position within
*		the window.  The scrollbars are adjusted to match.
*
*		After calling this routine, the caller should skip any remaining
*		plotting operations for the current repaint, since this routine
*		requests Windows to repaint the plot window.
*
*		If a plot area array is being used, the desired plot area must
*		be selected before calling this routine.  All the other plot
*		areas are kept synchronized with the selected plot area.
*
* Side Effects:
*		o	Causes the plot window to be repainted
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the arguments is invalid
*
* See Also:
*		PlPaintCursor, PlVisibleCheck
*
*-Date     Author		Revision
* -------- ------------	--------
* 07-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinScroll_pos(
PL_CTX	*pPlot,			// IO pointer to plot context structure
double	dXAnchDVal,		// I X anchor value, as display value
double	dYAnchDVal,		// I Y anchor value, as display value
float	fXAnchFrac,		// I X anchor position, as fraction of axis width
float	fYAnchFrac)		// I Y anchor position, as fraction of axis height
{
	int		retStat=PL_OK;
	double	dXScroll, dYScroll;	// logical scroll position
	double	dXWidth, dYWidth;	// axis length, as display value
	double	dXWVis, dYWVis;		// visible length, as display value
	double	dXAnch=dXAnchDVal, dYAnch=dYAnchDVal;

	if (fXAnchFrac < 0.F) fXAnchFrac = 0.F;
	else if (fXAnchFrac > 1.F) fXAnchFrac = 1.F;
	if (fYAnchFrac < 0.F) fYAnchFrac = 0.F;
	else if (fYAnchFrac > 1.F) fYAnchFrac = 1.F;

	if (pPlot->pXAxis->eScaleType == PL_LOG) {
		if (pPlot->pXAxis->bLogZero && dXAnch <= 0.)
			dXAnch = pPlot->pXAxis->dLogZero;
		else
			dXAnch = log10(dXAnch);
	}
	dXWidth = pPlot->pXAxis->dLValT - pPlot->pXAxis->dLValB;
	if (pPlot->pXAxis->fZoom > 1.F) {
		dXWVis = dXWidth / pPlot->pXAxis->fZoom;
		dXScroll = (dXAnch - pPlot->pXAxis->dLValB -
								fXAnchFrac * dXWVis) / (dXWidth - dXWVis);
	}
	else
		dXScroll = 0.;
	if (dXScroll < 0.)		dXScroll = 0.;
	else if (dXScroll >1.)		dXScroll = 1.;

	if (pPlot->pYAxis->eScaleType == PL_LOG) {
		if (pPlot->pYAxis->bLogZero && dYAnch <= 0.)
			dYAnch = pPlot->pYAxis->dLogZero;
		else
			dYAnch = log10(dYAnch);
	}
	dYWidth = (pPlot->pYAxis->dLValT - pPlot->pYAxis->dLValB);
	if (pPlot->pYAxis->fZoom > 1.F) {
		dYWVis = dYWidth / pPlot->pYAxis->fZoom;
		dYScroll = (dYAnch - pPlot->pYAxis->dLValB -
								fYAnchFrac * dYWVis) / (dYWidth - dYWVis);
	}
	else
		dYScroll = 0.;
	if (dYScroll < 0.)			dYScroll = 0.;
	else if (dYScroll >1.)		dYScroll = 1.;
	retStat = PlWinScroll(pPlot, (float)dXScroll, (float)dYScroll);
	if (retStat != PL_OK) goto done;	if (pPlot->bScrollBars) {
		SetScrollPos(pPlot->hwPlot, SB_HORZ, (int)((dXScroll)*1000.), TRUE);
		SetScrollPos(pPlot->hwPlot, SB_VERT, (int)((1.-dYScroll)*1000.), TRUE);
		if (IsWindow(pPlot->hwPlot))
			InvalidateRect(pPlot->hwPlot, NULL, TRUE);
	}

done:
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinScroll_v - process vertical scroll bar messages
*
* Purpose:
*		This routine processes WM_VSCROLL messages for the plot window.
*
*		Typically, this routine would not be called by a program--it
*		would use PlWinProc_helper, which calls this this routine.
*
*		A keyboard interface to vertical scrolling is provided by
*		PlWinProc_helper:
*
*			HOME moves the thumb to the top
*			PAGEUP moves the thumb up
*			PAGEDOWN moves the thumb down
*			END moves the thumb to the bottom
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Side Effects:
*		o	The scroll bar position is updated.
*		o	The logical scroll position in PL_CTX is updated.
*		o	The plot window is given the input focus.
*		o	Causes the plot window to be repainted
*
* Notes:
* 1.	SB_LINEUP and SB_LINEDOWN events move the thumb up and down
*		1/nMajor (or 1/8, if nMajor is < 2) of the window height.
* 2.	SB_PAGEUP and SB_PAGEDOWN events move the thumb up and down
*		by the full window height.
* 3.	SB_TOP and SB_BOTTOM events move the thumb to the extreme top
*		and bottom.
*
* BUGS:
* 1.	Scrolling is handled by repainting the entire plot window.  An
*		alternative would be to scroll the current contents and paint
*		only the newly exposed region.
*
* See Also:
*		PlWinScroll, PlWinScroll_h
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinScroll_v(
PL_CTX	*pPlot,			// IO pointer to plot context structure
WPARAM	wParam,			// I wParam arg from windows scroll event
LPARAM	lParam)			// I lParam arg from windows scroll event
{
	int		retStat=PL_OK, inpErr=0;
	HWND	hwPlot;
	float	fYScroll, fZoom, fLineFrac;
	int		bRepaint=0;
	UINT	ynewPos;
	POINT	pt;
	RECT	RclVert;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pPlot->hwPlot must be a valid window
* 3		pPlot->pYAxis->fScroll must be between 0 and 1, inclusive
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	hwPlot = pPlot->hwPlot;
	PL_INP_CHK(2, !IsWindow(hwPlot), done)
	fYScroll = pPlot->pYAxis->fScroll;
	PL_INP_CHK(3, fYScroll < 0.F || fYScroll > 1.F, done)
	fYScroll = 1.F - .001F * GetScrollPos(hwPlot, SB_VERT);

	if ((fZoom = pPlot->pYAxis->fZoom) <= 1.F) goto done;

	if (pPlot->pYAxis->nMajor > 1)
		fLineFrac = 1.F / pPlot->pYAxis->nMajor;
	else
		fLineFrac = .125F;
	/*if (wParam == SB_TOP) fYScroll = 1.F;
	else if (wParam == SB_BOTTOM) fYScroll = 0.F;
	else if (wParam == SB_LINEUP) fYScroll += fLineFrac / (fZoom-1.F);
	else if (wParam == SB_LINEDOWN) fYScroll -= fLineFrac / (fZoom-1.F);
	else if (wParam == SB_PAGEUP) fYScroll += 1.F / (fZoom-1.F);
	else if (wParam == SB_PAGEDOWN) fYScroll -= 1.F / (fZoom-1.F);
	else if (wParam == SB_THUMBPOSITION) fYScroll = 1.F - LOWORD(lParam) / 1000.F;
	else if (wParam == SB_ENDSCROLL) bRepaint = 1;
	else
		;*/	// no action

	// Changed to switch statments to make vertical scroll work.
	// K.G 9/27/99
	switch (LOWORD(wParam)) 
	{
	    case SB_TOP:
			fYScroll = 1.F;
			break;

		case SB_BOTTOM:
			fYScroll = 0.F;
			break;

		case SB_LINEUP:
			fYScroll += fLineFrac / (fZoom-1.F);
			break;

		case SB_LINEDOWN:
			fYScroll -= fLineFrac / (fZoom-1.F);
			break;

		case SB_PAGEUP:
			fYScroll += 1.F / (fZoom-1.F);
			break;

		case SB_PAGEDOWN:
			fYScroll -= 1.F / (fZoom-1.F);
			break;

		case SB_THUMBPOSITION:
			double dl;
				
			GetCursorPos(&pt);//Get the current cursor position.
			ScreenToClient(hwPlot, &pt);//Map the point of the cursor to the scroll window.
			dl = (double)pt.y * (double)1000;//Multiply by the maximum horizontal position
											 //and store in a double to keep from overflowing.
			GetClientRect(hwPlot, &RclVert);//Get the client rectangle of the horizontal scroll bar.
			ynewPos = (UINT)(dl / (double)RclVert.bottom);//Divide by the bottom side of the scroll window.
			
			//Sets the position of the scroll box (thumb) in the vertical scroll bar and, 
			//redraws the scroll bar to reflect the new position of the scroll box.
			SetScrollPos(hwPlot, SB_VERT, ynewPos, TRUE);

			break;

		case SB_ENDSCROLL:
			bRepaint = 1;
			break;
	}

	if (wParam != SB_THUMBPOSITION) {  //Added this line.  K.G 9/27/99
	   if (fYScroll < 0.F) fYScroll = 0.F;
	   if (fYScroll > 1.F) fYScroll = 1.F;
	   if (fYScroll != pPlot->pYAxis->fScroll) {
	       SetScrollPos(hwPlot, SB_VERT, (int)((1.F-fYScroll)*1000.F), TRUE);
		   pPlot->bNeedScale = 1;
		   if (bRepaint) {
		       retStat = PlWinScroll(pPlot, pPlot->pXAxis->fScroll, fYScroll);
			   if (retStat != PL_OK)
			       goto done;
			   InvalidateRect(hwPlot, NULL, TRUE);
			   if (GetFocus() != hwPlot)
			       SetFocus(hwPlot);
		   }
	   }
	}

done:
	PL_IF_INP_ERR("PlWinScroll_v")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlWinZoom - change zoom factor(s) for plot window
*
* Purpose:
*		Changes the horizontal and/or vertical zoom factor(s) for a plot
*		window.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Side Effects:
*		o	if the plot window has scroll bar capability:
*			-	shows or hides scroll bars, as needed
*			-	adjusts scroll position so that the anchor point remains
*				at the same relative position
*		o	sets the zoom for all plot areas
*		o	recalculates scaling for the plot window
*		o	causes the plot window to be repainted
*
* Notes:
* 1.	This routine must not be called from within a plot callback function.
* 2.	The declaration of whether the plot window has scroll bar capability
*		is done in the PlPlotInit_win call.
* 3.	Any axis that has the .bNoZoom flag set has its zoom factor set to 1.
*
* See Also:
*		PlWinCreate, PlWinScroll
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-06-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlWinZoom(
PL_CTX	*pPlot,			// IO pointer to plot context structure
float	fXZoom,			// I X zoom factor, >= 1.
float	fYZoom)			// I Y zoom factor, >= 1.
{
	int		retStat=PL_OK, inpErr=0;
	double	dXAnchor, dXLeft, dXRight;
	float	fXAnchorFrac;
	double	dYAnchor, dYBottom, dYTop;
	float	fYAnchorFrac;
	double	dLValMin, dLValMax;
	HWND	hwPlot;
	int		iRow, iCol, iRowSave, iColSave;
	PL_AXIS	*pXAxis, *pYAxis;
	int		bScrollBarChanged=0;	// 1 if visibility changed

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0, then pPlot->hwPlot must be a valid window
* 3		fXZoom must be greater than or equal to 1
* 4		fYZoom must be greater than or equal to 1
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	hwPlot = pPlot->hwPlot;
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(hwPlot), done)
	PL_INP_CHK(3, fXZoom < 1.F, done)
	PL_INP_CHK(4, fYZoom < 1.F, done)

	iRowSave = pPlot->iAxisRow;
	iColSave = pPlot->iAxisCol;

	pPlot->bZoomChanged = 1;

	// Go through each area, adjusting the zoom factor.
	for (iRow=0; iRow<pPlot->nAxisRows; iRow++) {
		for (iCol=0; iCol<pPlot->nAxisCols; iCol++) {
			retStat = PlAreaArraySelect(pPlot, iRow, iCol);
			if (retStat != PL_OK) goto done;
			if (pPlot->pXAxis->bNoZoom)
				pPlot->pXAxis->fZoom = 1.F;
			else if (pPlot->pXAxis->fZoom != fXZoom) {
				pPlot->pXAxis->fZoom = fXZoom;
				bScrollBarChanged = 1;
			}
			if (pPlot->pYAxis->bNoZoom)
				pPlot->pYAxis->fZoom = 1.F;
			else if (pPlot->pYAxis->fZoom != fYZoom) {
				pPlot->pYAxis->fZoom = fYZoom;
				bScrollBarChanged = 1;
			}
		}
	}
	// For the plot area that has the cursor, adjust the scrolling
	// so that the anchor point is in the proper place.  (PlWinScroll_pos
	// also adjusts all the other plot areas to match.)
	//
	// If the anchor point isn't in the visible area, then change the
	// anchor point to be at the current anchor fraction position.
	retStat = PlAreaArraySelect(pPlot, pPlot->iCursorRow, pPlot->iCursorCol);
	if (retStat != PL_OK) goto done;
	pXAxis = pPlot->pXAxis;
	pYAxis = pPlot->pYAxis;
	dXAnchor = pXAxis->dDValAnchor;
	if (pXAxis->eScaleType == PL_LOG) {
		if (pXAxis->bLogZero && dXAnchor <= 0.)
			dXAnchor = pXAxis->dLogZero;
		else
			dXAnchor = log10(dXAnchor);
	}
	dXLeft = pXAxis->dLValBS;
	dXRight = pXAxis->dLValTS;
	fXAnchorFrac = pXAxis->fAnchorFrac;
	dLValMin = PL_MIN(dXLeft, dXRight);
	dLValMax = PL_MAX(dXLeft, dXRight);
	if (dXAnchor < dLValMin || dXAnchor > dLValMax) {
		fXAnchorFrac = .5F;
		dXAnchor = dXLeft + fXAnchorFrac * (dXRight - dXLeft);
	}
	if (pXAxis->eScaleType == PL_LOG) {
		if (pXAxis->bLogZero && dXAnchor <= pXAxis->dLogZero)
			dXAnchor = 0.;
		else
			dXAnchor = pow(10., dXAnchor);
	}
	dYAnchor = pYAxis->dDValAnchor;
	if (pYAxis->eScaleType == PL_LOG) {
		if (pYAxis->bLogZero && dYAnchor <= 0.)
			dYAnchor = pYAxis->dLogZero;
		else
			dYAnchor = log10(dYAnchor);
	}
	dYBottom = pYAxis->dLValBS;
	dYTop = pYAxis->dLValTS;
	fYAnchorFrac = pYAxis->fAnchorFrac;
	dLValMin = PL_MIN(dYBottom, dYTop);
	dLValMax = PL_MAX(dYBottom, dYTop);
	if (dYAnchor < dLValMin || dYAnchor > dLValMax) {
		fYAnchorFrac = .5F;
		dYAnchor = dYBottom + fYAnchorFrac * (dYTop - dYBottom);
	}
	if (pYAxis->eScaleType == PL_LOG) {
		if (pYAxis->bLogZero && dYAnchor <= pYAxis->dLogZero)
			dYAnchor = 0.;
		else
			dYAnchor = pow(10., dYAnchor);
	}
	if (dXAnchor != pXAxis->dDValAnchor || fXAnchorFrac != pXAxis->fAnchorFrac ||
					dYAnchor != pYAxis->dDValAnchor ||
					fYAnchorFrac != pYAxis->fAnchorFrac) {
		bScrollBarChanged = 1;
		pPlot->bNeedScale = 1;
	}
	// Hide or show scroll bars, as appropriate.
	if (pPlot->bScrollBars && bScrollBarChanged) {
		int		iHScroll, iVScroll;
		if (fXZoom > 1.F)	iHScroll = 1000;
		else				iHScroll = 0;
		if (fYZoom > 1.F)	iVScroll = 1000;
		else				iVScroll = 0;
		SetScrollRange(hwPlot, SB_HORZ, 0, iHScroll, FALSE);
		SetScrollRange(hwPlot, SB_VERT, 0, iVScroll, FALSE);
	}

	retStat = PlWinScroll_pos(pPlot,
					dXAnchor, dYAnchor, fXAnchorFrac, fYAnchorFrac);
	if (retStat != PL_OK) goto done;

	// Finally, rescale the window and restore the plot area the
	// caller had currently selected.
	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}
	retStat = PlAreaArraySelect(pPlot, iRowSave, iColSave);
	if (retStat != PL_OK) goto done;
	if (IsWindow(hwPlot))
		InvalidateRect(hwPlot, NULL, TRUE);

done:
	pPlot->bZoomChanged = 0;
	PL_IF_INP_ERR("PlWinZoom")
	return retStat;
}
