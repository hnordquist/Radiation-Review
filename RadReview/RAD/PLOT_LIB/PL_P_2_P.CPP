/* ======================================================================= */
/* ============================== pl_p_2_p.cpp =========================== */
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

/*---------------------------------------------------------------------------
* pl_p_2_p.cpp - miscellaneous plotting functions
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

#include <listsubr.h>
#include "pl.h"
#include "pl_pvt.h"
#include "wu.h"

/*+/csubr/TOC----------------------------------------------------------------
* PlPaintCursor - perform the actual cursor drawing
*
* Purpose:
*		Performs the actual cursor drawing operations.  The shape of the
*		cursor is selected using the following constants.
*
*		PL_CROSS        cross (similar to +)
*		PL_CROSS_TALL   vertical hairline with horizontal crossbar
*		PL_CROSS_WIDE   horizontal hairline with vertical crossbar
*		PL_CROSS_FULL   vertical hairline with horizontal hairline
*
*		The hairlines extend the full height or width of the area used
*		for plotting data.  In the case of plot area arrays, the vertical
*		hairline extends from the bottom of the lower row to the top
*		of the upper row; the horizontal from the left edge of the
*		leftmost column to the right edge of the rightmost column.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other error codes
*
* Notes:
* 1.	When using an array of plot areas, the plot area containing the
*		plot cursor must be selected prior to calling this routine.
* 2.	In order to erase properly, the X and Y data values and the rgb
*		color must be exactly the same as when the cursor was drawn.
* 3.	This routine draws the cursor in a way that allows preserving
*		the other plotted information without having to redraw that
*		information.
*
* See Also:
*		PlVisibleCheck, PlWinScroll_pos
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-23-95 R. Cole		created
* 08-16-95 R. Cole		added tall and wide cursor shapes
*--------------------------------------------------------------------------*/
int PASCAL
PlPaintCursor(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		bErase,			// I 0,1 to draw,erase
double	dXVal,			// I X data value
double	dYVal,			// I Y data value
int		eMark,			// I code for cursor shape
float	fPts,			// I size of mark, in points
COLORREF rgb)			// I color to use for drawing
{
	int		retStat=PL_OK, inpErr=0;
	double	dXPx, dYPx;			// pixel coordinates for point
	int		rowSave, colSave;
	double	xSize, ySize;		// cursor size, in pixels
	int		pxL, pxR, pxB, pxT;	// logical endpoints of cursor
	double	dX1, dY1, dX2, dY2;	// after clipping
	int		iXPx, iYPx;
	int		left, top, right, bottom;
	HDC		hDC=0;
	HPEN	hPen, hPenOld;
	COLORREF myRgb, rgbOld;
	int		iDrawModeOld;
	int		stat;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		eMark must be a valid cursor shape
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit)
	if (eMark == PL_CROSS || eMark == PL_CROSS_TALL ||
			eMark == PL_CROSS_WIDE || eMark == PL_CROSS_FULL)
		;	// just checking
	else
		PL_INP_CHK(3, 1, sub_exit)

	if (!pPlot->bPrinter) {
		retStat = PlWinHDCSetup(pPlot, 0, 0, 0,
				PL_LINE, 0.F,
				3, rgb, // use rgb ^ GetBkColor (for use with R2_XORPEN)
				&hDC, &hPen, &hPenOld, &myRgb);
		if (retStat != PL_OK) goto done;
		rgbOld = SetTextColor(hDC, myRgb);
		iDrawModeOld = SetROP2(hDC, R2_XORPEN);
	}
	else {
		retStat = PlWinHDCSetup(pPlot, 0, 0, 0,
				PL_LINE, 0.F, 0, rgb, &hDC, &hPen, &hPenOld, &myRgb);
		if (retStat != PL_OK) goto done;
		rgbOld = SetTextColor(hDC, myRgb);
	}

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	// Do the initial setup assuming a simple cross of the
	// caller-specified size.
	xSize = .5F * PL_XPTS_TO_XPIX(fPts);
	ySize = .5F * PL_YPTS_TO_YPIX(fPts);
	dXPx = PlWinCvtXValToXPix(pPlot, dXVal);
	dYPx = PlWinCvtYValToYPix(pPlot, dYVal);
	left = pPlot->pXAxis->pxB;
	top = pPlot->pYAxis->pxT;
	right = pPlot->pXAxis->pxT;
	bottom = pPlot->pYAxis->pxB;
	pxL = (int)(dXPx - xSize);
	pxR = (int)(dXPx + xSize);
	pxT = (int)(dYPx - ySize);
	pxB = (int)(dYPx + ySize);

	// Now, if either a tall or wide arm is needed, make the
	// appropriate adjustments.  Both the endpoints and the clip
	// boundaries are adjusted.
	if (eMark == PL_CROSS_TALL || eMark == PL_CROSS_FULL) {
		colSave = pPlot->iAxisCol;
		rowSave = pPlot->iAxisRow;
		stat = PlAreaArraySelect(pPlot, pPlot->nAxisRows-1, 0);
		if (stat != PL_OK) goto tall_error;
		pxB = bottom = pPlot->pYAxis->pxB;
		stat = PlAreaArraySelect(pPlot, 0, 0);
		if (stat != PL_OK) goto tall_error;
		pxT = top = pPlot->pYAxis->pxT;
tall_error:
		PlAreaArraySelect(pPlot, rowSave, colSave);
	}
	if (eMark == PL_CROSS_WIDE || eMark == PL_CROSS_FULL) {
		colSave = pPlot->iAxisCol;
		rowSave = pPlot->iAxisRow;
		stat = PlAreaArraySelect(pPlot, 0, pPlot->nAxisCols-1);
		if (stat != PL_OK) goto wide_error;
		pxR = right = pPlot->pXAxis->pxT;
		stat = PlAreaArraySelect(pPlot, 0, 0);
		if (stat != PL_OK) goto wide_error;
		pxL = left = pPlot->pXAxis->pxB;
wide_error:
		PlAreaArraySelect(pPlot, rowSave, colSave);
	}

	if (bErase)	pPlot->bCursorVisible = 0;
	else		pPlot->bCursorVisible = 1;

	if (pPlot->bCursorVisible) {
		// Remember where the cursor is, for use during zooms.
		PL_AXIS	*pXAxis=pPlot->pXAxis;
		PL_AXIS	*pYAxis=pPlot->pYAxis;
		pXAxis->dDValAnchor = dXVal;
		pXAxis->fAnchorFrac = (float)((dXPx - pXAxis->pxB) /
					(pXAxis->pxT - pXAxis->pxB));
		if (pXAxis->fAnchorFrac < 0.F) pXAxis->fAnchorFrac = 0.F;
		if (pXAxis->fAnchorFrac > 1.F) pXAxis->fAnchorFrac = 1.F;
		pYAxis->dDValAnchor = dYVal;
		pYAxis->fAnchorFrac = (float)((dYPx - pYAxis->pxB) /
					(pYAxis->pxT - pYAxis->pxB));
		if (pYAxis->fAnchorFrac < 0.F) pYAxis->fAnchorFrac = 0.F;
		if (pYAxis->fAnchorFrac > 1.F) pYAxis->fAnchorFrac = 1.F;
		// And fill in the other cursor info.
		pPlot->iCursorRow = pPlot->iAxisRow;
		pPlot->iCursorCol = pPlot->iAxisCol;
		pPlot->dCursorX = dXVal;
		pPlot->dCursorY = dYVal;
	}

	iXPx = (int)(dXPx + .5);
	iYPx = (int)(dYPx + .5);

	// Draw the horizontal arm of the cursor.  If the center of the
	// cursor is below or above the plot area, don't draw this arm.
	if (iYPx >= pPlot->pYAxis->pxT && iYPx <= pPlot->pYAxis->pxB) {
		dX1 = pxL; dY1 = iYPx; dX2 = pxR; dY2 = iYPx;
		stat = WuClipIt(1, left, top, right, bottom, &dX1, &dY1, &dX2, &dY2);
		if (stat != 0) {
			MoveToEx(hDC, (int)(dX1+.5), (int)(dY1+.5), NULL);
			if (LineTo(hDC, (int)(dX2+1.5), (int)(dY2+.5)) == 0)
				goto gdi_error;
		}
	}
	// Draw the vertical arm of the cursor.  If the center of the
	// cursor is left or right of the plot area, don't draw this arm.
	if (iXPx >= pPlot->pXAxis->pxB && iXPx <= pPlot->pXAxis->pxT) {
		dX1 = iXPx; dY1 = pxT; dX2 = iXPx; dY2 = pxB;
		stat = WuClipIt(1, left, top, right, bottom, &dX1, &dY1, &dX2, &dY2);
		if (stat != 0) {
			MoveToEx(hDC, (int)(dX1+.5), (int)(dY1+.5), NULL);
			if (LineTo(hDC, (int)(dX2+.5), (int)(dY2+1.5)) == 0)
				goto gdi_error;
		}
	}

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (hDC != 0) {
		SetTextColor(hDC, rgbOld);
		if (!pPlot->bPrinter)
			SetROP2(hDC, iDrawModeOld);
		if (pPlot->hDC == 0) ReleaseDC(pPlot->hwPlot, hDC);
	}
sub_exit:
	PL_IF_INP_ERR("PlPaintCursor")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPlotClear - clear a plot window
*
* Purpose:
*		Clear a plot window, filling it with the window's background color.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		another code if an error occurs
*
* Notes:
* 1.	Typical applications will not need to call PlPlotClear--when the
*		plot needs to be repainted, the damaged areas will already have
*		been cleared automatically.
* 2.	If PlPlotWinCreate is used to create the plot window, it establishes
*		the window's background color.  For windows that are created in other
*		ways, SetBkColor can be used to set the background color.
*
* See Also:
*		PlAreaClear
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPlotClear(
PL_CTX	*pPlot)			// I pointer to plot context structure
{
	int		retStat=PL_OK, inpErr=0;
	HDC		hDC=0;
	HBRUSH	hBrush=0, hBrushOld=0;
	HPEN	hPen, hPenOld;
	RECT	rect;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if plotting is to the screen, pPlot->hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit)

	if (pPlot->bPrinter) goto sub_exit;

	retStat = PlWinHDCSetup(pPlot, 1, 0, 0,
			PL_LINE, 0.F, 1, // use GetBkColor
			0, &hDC, &hPen, &hPenOld, NULL);
	if (retStat != PL_OK) goto done;
	if ((hBrush = CreateSolidBrush(GetBkColor(hDC))) == 0) goto gdi_error;
	/* KG */
	//if ((hBrushOld = SelectObject(hDC, hBrush)) == 0) goto gdi_error;
	if ((hBrushOld = (HBRUSH)SelectObject(hDC, hBrush)) == 0) goto gdi_error;

	// Need to get full rect; pPlot->rectPlot excludes border.
	GetClientRect(pPlot->hwPlot, &rect);
	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	if (pPlot->bBorder) {
		retStat = PlWinPaintBorder(pPlot->hwPlot, hDC, -1);
		if (retStat != PL_OK)
			goto done;
	}

done:
	if (hBrushOld != 0) SelectObject(hDC, hBrushOld);
	if (hBrush != 0) DeleteObject(hBrush);
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
sub_exit:
	pPlot->bCursorVisible = 0;
	PL_IF_INP_ERR("PlPlotClear")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlPlotInit - do some initialization for a plot window
*
* Purpose:
*		Initialize a PL_CTX structure.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-20-95 R. Cole		created
* 08-22-95 R. Cole		modified to accomodate new structure members
*--------------------------------------------------------------------------*/
int PASCAL
PlPlotInit(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_CTX	*pPlotWin)		// I pointer to screen plot context, or NULL
{
	int		retStat=PL_OK;
	int		row, col;
	int		rowSave=0, colSave=0;
	int		nRow=1, nCol=1;

	// First, make a shallow copy of the screen plot context.
	if (pPlotWin != NULL) {
		*pPlot = *pPlotWin;
		strcpy(pPlot->szIntInit, "not");
	}

	// Initialize non-plot-area fields.
	pPlot->hwPlot = NULL;
	pPlot->hDC = 0;
	pPlot->hPD = 0;
	pPlot->pPd = NULL;
	pPlot->szPrtName = pPlot->szPrtPort =
								pPlot->szPrtFile = pPlot->szPrtJob = NULL;
	pPlot->hRgnPaint = 0;
	pPlot->bPrinter = pPlot->bPrintCancel = pPlot->bPrinterColor = 0;
	pPlot->hwPrintCancel = NULL;
	pPlot->bScrollBars = 0;
	pPlot->bBorder = 0;
	pPlot->bNeedScale = 1;

	// Do "internal PL.LIB" initialization if it hasn't been done yet.
	if (strcmp(pPlot->szIntInit, "yes") != 0) {
		strcpy(pPlot->szIntInit, "yes");
		pPlot->paXAxis = pPlot->paYAxis = NULL;
		pPlot->pObjCtx = (PL_OBJ_CTX *)malloc(sizeof(PL_OBJ_CTX));
		pPlot->pMouse = (PL_MOUSE *)malloc(sizeof(PL_MOUSE));
		if (pPlot->pObjCtx == NULL || pPlot->pMouse == NULL) {
			if (pPlot->pObjCtx != NULL) free(pPlot->pObjCtx);
			if (pPlot->pMouse != NULL) free(pPlot->pMouse);
			pPlot->pObjCtx = NULL;
			pPlot->pMouse = NULL;
			retStat = PL_MALLOC_FAIL;
			goto done;
		}
		PlObjInit(pPlot);
	}

	if (pPlotWin == NULL) {
		// Only initialize these fields if a copy isn't being used.
		pPlot->pCursorFn = NULL;
		pPlot->pCursorArg = NULL;
		pPlot->pEventPreFn = pPlot->pEventPostFn = NULL;
		pPlot->pEventPreArg = pPlot->pEventPostArg = NULL;
		pPlot->pObjectFn = NULL;
		pPlot->pObjectArg = NULL;
		pPlot->pPlotFn = NULL;
		pPlot->pPlotArg = NULL;
		pPlot->bCursorVisible = 0;	// cursor is invisible
		pPlot->iCursorRow = pPlot->iCursorCol = 0;
		pPlot->dCursorX = pPlot->dCursorY = 0.;
		pPlot->pXAxis = &pPlot->xAxis;
		pPlot->pYAxis = &pPlot->yAxis;
		if (pPlot->paXAxis != NULL) free(pPlot->paXAxis);
		if (pPlot->paYAxis != NULL) free(pPlot->paYAxis);
		pPlot->paXAxis = pPlot->paYAxis = NULL;
		pPlot->nAxisRows = nRow;
		pPlot->nAxisCols = nCol;
		pPlot->fVertPts = pPlot->fHorizPts = 0.F;
		pPlot->iAxisRow = pPlot->iAxisCol = 0;
		pPlot->nTitleLinesLeft = pPlot->nTitleLinesRight = 0;
		pPlot->nTitleLinesBottom = pPlot->nTitleLinesTop = 0;
		pPlot->fTitlePts = 0.F;
		pPlot->szTitleFont[0] = '\0';
	}
	else {
		// Allocate memory that will be used to "deepen" the copy.
		rowSave = pPlot->iAxisRow;
		colSave = pPlot->iAxisCol;
		nRow = pPlot->nAxisRows;
		nCol = pPlot->nAxisCols;
		// Now, allocate needed memory.
		if (pPlotWin->paXAxis == NULL)
			pPlot->paXAxis = pPlot->paYAxis = NULL;
		else {
			pPlot->paXAxis = (PL_AXIS *)malloc(nRow*nCol*sizeof(PL_AXIS));
			pPlot->paYAxis = (PL_AXIS *)malloc(nRow*nCol*sizeof(PL_AXIS));
			if (pPlot->paXAxis == NULL || pPlot->paYAxis == NULL) {
				retStat = PL_MALLOC_FAIL;
				if (pPlot->paXAxis != NULL) free(pPlot->paXAxis);
				if (pPlot->paYAxis != NULL) free(pPlot->paYAxis);
				pPlot->paXAxis = pPlot->paYAxis = NULL;
				pPlot->pXAxis = &pPlot->xAxis;
				pPlot->pYAxis = &pPlot->yAxis;
				nRow = nCol = 1;
				goto done;
			}
		}
		// Deepen the copy for objects.
		retStat = PlObjCopyAll(pPlot, pPlotWin);
		if (retStat != PL_OK) goto done;
	}

	// Initialize the axes.  If we're copying from the screen plot
	// context, just copy the screen's axes (memory is already
	// allocated if necessary).
	for (row=0; row<nRow; row++) {
		for (col=0; col<nCol; col++) {
			PlAreaArraySelect(pPlot, row, col);
			if (pPlotWin == NULL) {
				PlPlotInit_axis(pPlot->pXAxis);
				PlPlotInit_axis(pPlot->pYAxis);
			}
			else {
				PlAreaArraySelect(pPlotWin, row, col);
				*pPlot->pXAxis = *pPlotWin->pXAxis;
				*pPlot->pYAxis = *pPlotWin->pYAxis;
			}
			pPlot->pXAxis->pxB = pPlot->pXAxis->pxT = 0;
			pPlot->pYAxis->pxB = pPlot->pYAxis->pxT = 0;
		}
	}

done:
	PlAreaArraySelect(pPlot, rowSave, colSave);
	if (pPlotWin != NULL)
		PlAreaArraySelect(pPlotWin, rowSave, colSave);
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlPlotInit_axis - do some initialization for a plot axis
*
* Purpose:
*		Initialize a PL_AXIS structure.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-21-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlPlotInit_axis(
PL_AXIS	*pAxis)			// IO pointer to plot axis structure
{
	pAxis->eStyle = PL_NO_AXIS;
	pAxis->eScaleType = PL_LIN;
	pAxis->nMajor = pAxis->nMinor = 1;
	pAxis->fArrayPosFrac = pAxis->fArraySizeFrac = -1.F;
	pAxis->pxAreaB = pAxis->pxB = 0;
	pAxis->pxAreaT = pAxis->pxT = 10;
	// (Since we're initializing for PL_LIN, dLVal is same as dDVal.)
	pAxis->dLValB = pAxis->dLValBS = pAxis->dDValB = 0.;
	pAxis->dLValT = pAxis->dLValTS = pAxis->dDValT = 10.;
	pAxis->dPxPerLVal = 1.;
	pAxis->bIntAxis = 0;
	pAxis->bLogZero = 0;
	pAxis->dLogZero = log10(.5);
	pAxis->dDValB_minNon0 = 1.;
	pAxis->fZoom = 1.F;
	pAxis->bNoZoom = 0;
	pAxis->fScroll = 0.F;
	pAxis->fAnchorFrac = .5F;
	pAxis->dDValAnchor = 5.F;
	pAxis->fAnnotPts = pAxis->fAnnotDeg = 0.F;
	pAxis->iAnnotNChar = 0;
	pAxis->szAnnotFont[0] = pAxis->szAnnotFmt[0] = '\0';
	pAxis->pAnnotFn = NULL;
	pAxis->pAnnotArg = NULL;
	pAxis->nTitleLinesB = pAxis->nTitleLinesT = 0;
	pAxis->fTitlePts = 0.F;
	pAxis->szTitleFont[0] = '\0';

}
/*+/csubr/TOC----------------------------------------------------------------
* PlPlotInit_prt - initialize for plotting on a printer
*
* Purpose:
*		Initialize a PL_CTX structure prior to using it for plotting,
*		or reinitialize the structure at a later time.  This function
*		initializes for plotting on a printer.
*
*		This routine:
*		o	sets the plot for no grid or axis
*		o	sets the plot for no annotations, titles, or legend
*		o	set the X and Y axes to be linear, with a range from
*			0 to 10
*		o	optionally copies the current plot setup for the screen so
*			that the printout will be a good facsimile
*
*		PlPrtClose must be called when plot operations to the printer are
*		complete, in order to actually send the print job to the print
*		manager and also to avoid memory leaks.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Implicit Inputs:
*		pPlot->hDC  handle for Windows device context to be used
*		pPlot->hPD  handle for PRINTDLG structure, or 0
*		pPlot->pPd  pointer to PRINTDLG structure, or NULL
*
* Notes:
* 1.	NOTE WELL!!  This function must be called prior to calling
*		any of the other PlPlotXxx routines.  If this isn't done,
*		plotting will be unpredictable.
* 2.	If pPlotWin is non-NULL, then the plot context for the printer
*		is automatically set up as a facsimile of the screen plot context.
* 3.	If fBorderPts is > 0, then a border is drawn around the area.
* 4.	This routine can be freely called to reinitialize various
*		parameters in the plot context.  Most frequently, this is done
*		to modify the margins for drawing in separate parts of the page.
*
* See Also:
*		PlPrtPrintSimple
*		PlPrtOpenDefault, PlPrtOpenDialog
*		PlPlotStyle, PlArea...Style, and PlPlot
*		PlAreaRangeSet
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-20-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPlotInit_prt(
PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
PL_CTX	*pPlotWin,		// I pointer to screen plot context, or NULL
float	fBorderPts,		// I thickness of border to be drawn, or 0.F
float	fLMarg_in,		// I margin left of plot area, in inches
float	fRMarg_in,		// I margin right of plot area, in inches
float	fTMarg_in,		// I margin above plot area, in inches
float	fBMarg_in)		// I margin below plot area, in inches
{
	int		retStat=PL_OK, inpErr=0;
	int		pxPerX_in, pxPerY_in;
	int		stat;

	// the following are set up by PlPrtOpen
	HDC		hDC=pPlot->hDC;
	HGLOBAL	hPD=pPlot->hPD;
	void	*pPd=pPlot->pPd;
	char	*szPrtName, *szPrtPort, *szPrtFile, *szPrtJob;

	HPEN	hPen=0, hPenOld=0;
	HRGN	rgn=0;
	int		iThick;
	POINT	ptSize, ptOffset;
	int		iWid_px, iHt_px, iXOff_px, iYOff_px;
	int		bPrtOpen=0;
	int		bPrintCancel;
	HWND	hwPrintCancel;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pPlot->hDC must be not be 0
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, pPlot->hDC == 0, done)

	// Save some of the PL_CTX members that are wiped out by PlPlotInit.
	hDC = pPlot->hDC;
	hPD = pPlot->hPD;
	pPd = pPlot->pPd;
	szPrtName = pPlot->szPrtName;
	szPrtPort = pPlot->szPrtPort;
	szPrtFile = pPlot->szPrtFile;
	szPrtJob = pPlot->szPrtJob;
	bPrintCancel = pPlot->bPrintCancel;
	hwPrintCancel = pPlot->hwPrintCancel;

	if (strcmp(pPlot->szPrintOpen, "prt") == 0) bPrtOpen = 1;
	retStat = PlPlotInit(pPlot, pPlotWin);
	if (bPrtOpen) {
		// Restore the PL_CTX members
		pPlot->hDC = hDC;
		pPlot->hPD = hPD;
		pPlot->pPd = pPd;
		pPlot->bPrintCancel = bPrintCancel;
		pPlot->hwPrintCancel = hwPrintCancel;
		pPlot->szPrtName = szPrtName;
		pPlot->szPrtPort = szPrtPort;
		pPlot->szPrtFile = szPrtFile;
		pPlot->szPrtJob = szPrtJob;
		strcpy(pPlot->szPrintOpen, "prt");
	}
	if (retStat != PL_OK) goto done;
	pPlot->bPrinter = 1;

/*---------------------------------------------------------------------------
*		Find out if the printer supports color.  If it doesn't, set the
*		background color to white.  (PlWinHDCSetup takes care of special
*		handling for colored pens.)
*--------------------------------------------------------------------------*/
	stat = GetDeviceCaps(hDC, NUMCOLORS);
	if (stat > 2)	pPlot->bPrinterColor = 1;
	else			SetBkColor(hDC, WHITE);

/*---------------------------------------------------------------------------
*		Now figure out some scaling information.
*--------------------------------------------------------------------------*/
	pxPerX_in = GetDeviceCaps(hDC, LOGPIXELSX);
	if (pxPerX_in <= 0) goto gdi_error;
	pPlot->fTwipsPerPixX = 1440.F / pxPerX_in;
	pxPerY_in = GetDeviceCaps(hDC, LOGPIXELSY);
	if (pxPerY_in <= 0) goto gdi_error;
	pPlot->fTwipsPerPixY = 1440.F / pxPerY_in;

/*---------------------------------------------------------------------------
*		Set up the actual rectangle for printing, based on the caller's
*		margins, on the physical page size, and on the size of the
*		non-printable area at the edges of the paper.
*--------------------------------------------------------------------------*/

	stat = Escape(hDC, GETPHYSPAGESIZE, NULL, NULL, &ptSize);
	if (stat <= 0) goto gdi_error;
	iWid_px = ptSize.x;	// physical width of page
	iHt_px = ptSize.y;	// physical height of page
	stat = Escape(hDC, GETPRINTINGOFFSET, NULL, NULL, &ptOffset);
	if (stat <= 0) goto gdi_error;
	iXOff_px = ptOffset.x;	// width of horiz non-printing area
	iYOff_px = ptOffset.y;	// width of vert non-printing area

	pPlot->rectPlot.left = (int)(fLMarg_in * pxPerX_in - iXOff_px);
	pPlot->rectPlot.right = (int)(iWid_px - iXOff_px - fRMarg_in * pxPerX_in);
	pPlot->rectPlot.top = (int)(fTMarg_in * pxPerY_in - iYOff_px);
	pPlot->rectPlot.bottom = (int)(iHt_px - iYOff_px - fBMarg_in * pxPerY_in);

	rgn = CreateRectRgn(pPlot->rectPlot.left, pPlot->rectPlot.top,
			pPlot->rectPlot.right+1, pPlot->rectPlot.bottom+1);
	if (rgn == 0) goto gdi_error;
	stat = SelectClipRgn(hDC, rgn);
	if (stat == ERROR) goto gdi_error;

	if (fBorderPts > 0.F) {
		// Print a box around the "plot area".
		iThick = PL_PTS_TO_PIX(fBorderPts);
		if (iThick < 1) iThick = 1;
		if ((hPen = CreatePen(PS_SOLID, iThick, BLACK)) == 0) goto gdi_error;
		/* KG */
		//if ((hPenOld = SelectObject(hDC, hPen)) == 0) goto gdi_error;
		if ((hPenOld = (HPEN)SelectObject(hDC, hPen)) == 0) goto gdi_error;
		MoveToEx(hDC, pPlot->rectPlot.left, pPlot->rectPlot.top, NULL);
		if (LineTo(hDC, pPlot->rectPlot.right, pPlot->rectPlot.top) == 0)
			goto gdi_error;
		if (LineTo(hDC, pPlot->rectPlot.right, pPlot->rectPlot.bottom) == 0)
			goto gdi_error;
		if (LineTo(hDC, pPlot->rectPlot.left, pPlot->rectPlot.bottom) == 0)
			goto gdi_error;
		if (LineTo(hDC, pPlot->rectPlot.left, pPlot->rectPlot.top) == 0)
			goto gdi_error;
	}

	// Make .1 inch margins within the box.
	pPlot->rectPlot.left += (int)(.1F * pxPerX_in);
	pPlot->rectPlot.right -= (int)(.1F * pxPerX_in);
	pPlot->rectPlot.right++;	// conform to RECT convention
	pPlot->rectPlot.top += (int)(.1F * pxPerY_in);
	pPlot->rectPlot.bottom -= (int)(.1F * pxPerY_in);
	pPlot->rectPlot.bottom++;	// conform to RECT convention
	retStat = PlWinZoom(pPlot, pPlot->pXAxis->fZoom, pPlot->pYAxis->fZoom);
	if (retStat != PL_OK) goto done;

done:
	if (rgn != 0) DeleteObject(rgn);
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	PL_IF_INP_ERR("PlPlotInit_prt")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPlotInit_win - initialize for plotting in a plot window
*
* Purpose:
*		Initialize a PL_CTX structure prior to using it for plotting,
*		or reinitialize the structure at a later time.
*
*		This routine:
*		o	sets the zoom factors to 1
*		o	sets the scroll position to the bottom left
*		o	sets the flag indicating the plot cursor isn't visible
*		o	sets the plot for no grid or axis
*		o	sets the plot for no annotations, titles, or legend
*		o	set the X and Y axes to be linear, with a range from
*			0 to 10
*
*		In general, you should call PlAreaRangeSet after calling this
*		routine (even though there may be cases where it's not actually
*		necessary).
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If the user will be using the keyboard to interact with the plot
*		(e.g., Page Up to scroll up and down), then it is recommended
*		that the bBorder argument be set to true.  This will give the
*		user a visual cue when the plot window has the focus and thus is
*		able to receive characters from the keyboard.
* 2.	Plot windows that are created with PlWinCreate have scroll
*		bar capability, and should have the bScrollBars argument set to 1.
*		Visual Basic PictureBox controls don't support scroll bars.  The
*		programmer must determine the capability for other windows.
* 3.	NOTE WELL!!  This function must be called prior to calling
*		any of the other PlPlotXxx routines.  If this isn't done,
*		plotting will be unpredictable.
* 4.	NOTE WELL!!  This function must not be called as part of the
*		handling of a WM_PAINT message.
* 5.	This function can be called any time it is desired to change
*		parameters associated with its arguments.
*
* See Also:
*		PlWinCreate, PlPlotStyle, PlArea...Style, and PlPlot
*		PlAreaRangeSet
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-20-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPlotInit_win(
PL_CTX	*pPlot,			// IO pointer to plot context structure
HWND	hwPlot,			// I handle to plot window
int		bBorder,		// I 1 to cause active/inactive border to be drawn
int		bScrollBars)	// I 1 if the window has scroll bar capability
{
	int		retStat=PL_OK, inpErr=0;
#define CLASS_DIM 20
	char	szClass[CLASS_DIM];
	int		stat, iTemp;
	HDC		hDC=0;
	int		bAlreadyInit=0;

	if (WM_SETFOCUS != 0x0007 || WM_KILLFOCUS != 0x0008) {
		// If you get this message, it means that the constants in
		// pl_defs.bas are wrong and need to be changed.  When you
		// change them (to agree with <windows.h>, change the test
		// in the 'if' statement, too.
		MessageBox(NULL, "WM_XXX constant bad", "PlPlotInit_win", MB_OK);
	}

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pPlot->hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, !IsWindow(hwPlot), done)

	if (strcmp(pPlot->szIntInit, "yes") == 0)
		bAlreadyInit = 1;

	retStat = PlPlotInit(pPlot, NULL);
	if (retStat != PL_OK) goto done;
	pPlot->hwPlot = hwPlot;
	pPlot->bScrollBars = bScrollBars?1:0;
	pPlot->bBorder = bBorder?1:0;
	if (!bAlreadyInit)
		PlWinMouseInit(pPlot);

/*---------------------------------------------------------------------------
*		Now figure out some scaling information.
*--------------------------------------------------------------------------*/
	GetClientRect(hwPlot, &pPlot->rectPlot);
	if (bBorder) {
		int		xBorder, yBorder;
		xBorder = 1 + 2*GetSystemMetrics(SM_CXBORDER);
		yBorder = 1 + 2*GetSystemMetrics(SM_CYBORDER);
		pPlot->rectPlot.left += xBorder;
		pPlot->rectPlot.right -= xBorder;
		pPlot->rectPlot.top += yBorder;
		pPlot->rectPlot.bottom -= yBorder;
	}
	hDC = GetDC(hwPlot);
	if (hDC == 0) goto gdi_error;

	// tw/pix = 1440 twip/inch * inch/pix
	iTemp = GetDeviceCaps(hDC, LOGPIXELSX);
	if (iTemp <= 0) goto gdi_error;
	pPlot->fTwipsPerPixX = 1440.F / iTemp;
	iTemp = GetDeviceCaps(hDC, LOGPIXELSY);
	if (iTemp <= 0) goto gdi_error;
	pPlot->fTwipsPerPixY = 1440.F / iTemp;

	stat = GetClassName(hwPlot, szClass, CLASS_DIM-1);
	if (stat == (int)strlen(PL_CLASS) && strcmp(szClass, PL_CLASS) == 0) {
		SetWindowLong(hwPlot, PL_EXTRA_PPL_CTX, (LONG)pPlot);
	}
	retStat = PlWinZoom(pPlot, 1.F, 1.F);
	if (retStat != PL_OK)
		goto done;

done:
	if (hDC != 0) ReleaseDC(hwPlot, hDC);
	PL_IF_INP_ERR("PlPlotInit_win")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
#undef CLASS_DIM
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPlotStyle - control for plot titles
*
* Purpose:
*		This routine controls the allocation of space within the plot
*		window, leaving room for title lines at the top, bottom, left, and
*		right of the plot window.
*
*		Top and bottom title lines will always be the full width of the
*		plot window.  Left and right title lines will be shorter than
*		the height of the plot window, depending on the vertical space
*		occupied by the top and bottom title lines.
*
*		For top and bottom title lines, text is oriented horizontally.  For
*		left and right title lines, text is oriented vertically with the
*		left end of the line toward the bottom of the plot window.
*
*		Axis-specific items are specified with PlArea...Style.
*
*		The actual drawing of the title lines must be done with PlPlotTitle.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* See Also:
*		PlPlotInit, PlAxisDraw
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-30-95 R. Cole		created
* 08-22-95 R. Cole		eliminated concept of legend; added arguments to
*						allow multi-line titles on all 4 sides of window
*--------------------------------------------------------------------------*/
int PASCAL
PlPlotStyle(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		nTitleLinesTop,	// I # of title lines at window top, or 0
int		nTitleLinesBottom,// I # of title lines at window bottom, or 0
int		nTitleLinesLeft,// I # of title lines at window left, or 0
int		nTitleLinesRight,// I # of title lines at window right, or 0
float	fTitlePts,		// I font size in points of titles, or 0.
const char *szTitleFont)// I name of font for titles, or "" or NULL
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		nTitleLinesLeft must be >= 0
* 3		nTitleLinesRight must be >= 0
* 4		nTitleLinesBottom must be >= 0
* 5		nTitleLinesTop must be >= 0
*		if fTitlePts is greater than zero, then
* 6			the length of szTitleFont must less than PL_MSG_DIM
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, nTitleLinesLeft < 0, done)
	PL_INP_CHK(3, nTitleLinesRight < 0, done)
	PL_INP_CHK(4, nTitleLinesBottom < 0, done)
	PL_INP_CHK(5, nTitleLinesTop < 0, done)
	if (szTitleFont == NULL) szTitleFont = "";
	if (fTitlePts > 0.F) {
		PL_INP_CHK(6, strlen(szTitleFont) >= PL_MSG_DIM, done)
	}

	pPlot->bNeedScale = 1;

	if (fTitlePts > 0.F)	strcpy(pPlot->szTitleFont, szTitleFont);
	else					pPlot->szTitleFont[0] = '\0';
	pPlot->fTitlePts = fTitlePts;
	pPlot->nTitleLinesLeft = nTitleLinesLeft;
	pPlot->nTitleLinesRight = nTitleLinesRight;
	pPlot->nTitleLinesBottom = nTitleLinesBottom;
	pPlot->nTitleLinesTop = nTitleLinesTop;
	PlWinScale(pPlot);

done:
	PL_IF_INP_ERR("PlPlotStyle")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPlotTitleClear - clear a plot title line
*
* Purpose:
*		Erases a plot title line.
*
*		The 'eSide' argument selects the set of title lines, corresponding
*		to the desired side of the plot window:
*
*			PL_SIDE_LEFT      left side
*			PL_SIDE_RIGHT     right side
*			PL_SIDE_TOP       top side
*			PL_SIDE_BOTTOM    bottom side
*
*		The 'iLine' argument selects a particular line within the selected
*		set, starting with line number 0.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	In many cases it won't be necessary to call this routine, since
*		clearing the plot window also clears all the title lines.  When
*		a title line is being used to display dynamic information, such
*		as the coordinates of the plot cursor, this routine will be needed.
*
* See Also:
*		PlPlotStyle
*		PlPlotTitleLine, PlPlotTitleMark, PlPlotTitlePoint, PlPlotTitleText
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPlotTitleClear(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		eSide,			// I selector for set of lines--PL_SIDE_xxx
int		iLine)			// I line number, starting with 0
{
	int		retStat=PL_OK, inpErr=0;
	HDC		hDC=0;
	HPEN	hPen, hPenOld;
	HBRUSH	hBrush=0, hBrushOld=0;
	COLORREF myRgb;
	int		xLeft, xRight, yTop, yBottom;
	int		nLines;
	float	fDeg;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		eSide must be one of the PL_SIDE_xxx values
* 4		iLine must be >= 0
* 5		the line number be consistent with the PlPlotStyle call
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit)
	if (eSide == PL_SIDE_LEFT) nLines = pPlot->nTitleLinesLeft;
	else if (eSide == PL_SIDE_RIGHT) nLines = pPlot->nTitleLinesRight;
	else if (eSide == PL_SIDE_TOP) nLines = pPlot->nTitleLinesTop;
	else if (eSide == PL_SIDE_BOTTOM) nLines = pPlot->nTitleLinesBottom;
	else
		PL_INP_CHK(3, 1, sub_exit)
	PL_INP_CHK(4, iLine < 0, sub_exit)
	PL_INP_CHK(5, iLine >= nLines, sub_exit)

	if (pPlot->bPrinter) goto done;

	retStat = PlWinHDCSetup(pPlot, 0, 0, 0,
			PL_LINE, 0.F, 1, // use GetBkColor
			0, &hDC, &hPen, &hPenOld, &myRgb);
	if (retStat != PL_OK) goto done;
	if ((hBrush = CreateSolidBrush(myRgb)) == 0) goto gdi_error;
	/* KG */
	//if ((hBrushOld = SelectObject(hDC, hBrush)) == 0) goto gdi_error;
	if ((hBrushOld = (HBRUSH)SelectObject(hDC, hBrush)) == 0) goto gdi_error;

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	// Get pixel coordinates for "top left" and "top right" of the
	// rectangle.  Then warp the coordinates into "top left" and
	// "bottom right", depending on the orientation of the rectangle.
	PlTitleLineToPx(pPlot, nLines,
		pPlot->rectPlot.left, pPlot->rectPlot.right,
		pPlot->rectPlot.top, pPlot->rectPlot.bottom,
		pPlot->fTitlePts, eSide, iLine, 0.F, &xLeft, &yBottom, &fDeg);
	PlTitleLineToPx(pPlot, nLines,
		pPlot->rectPlot.left, pPlot->rectPlot.right,
		pPlot->rectPlot.top, pPlot->rectPlot.bottom,
		pPlot->fTitlePts, eSide, iLine, 1.F, &xRight, &yTop, &fDeg);
	if (fDeg == 0.F)
		yBottom += PL_YPTS_TO_YPIX(pPlot->fTitlePts + 2.F) - 1;
	else
		xRight += PL_XPTS_TO_XPIX(pPlot->fTitlePts + 2.F) - 1;
	Rectangle(hDC, xLeft, yTop, xRight+1, yBottom+1);

done:
	if (hBrushOld != 0) SelectObject(hDC, hBrushOld);
	if (hBrush != 0) DeleteObject(hBrush);
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
sub_exit:
	PL_IF_INP_ERR("PlPlotTitleClear")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPlotTitleLine - draw a line in a plot title line
*
* Purpose:
*		Draw a line in a plot title line.
*
*		The 'eSide' argument selects the set of title lines, corresponding
*		to the desired side of the plot window:
*
*			PL_SIDE_LEFT      left side
*			PL_SIDE_RIGHT     right side
*			PL_SIDE_TOP       top side
*			PL_SIDE_BOTTOM    bottom side
*
*		The 'iLine' argument selects a particular line within the selected
*		set, starting with line number 0.
*
*		The endpoints of the line are specified as two values (between 0.0
*		and 1.0) that are fractions of the length of the plot title line.
*
*		See PlDrawLine_patterned for a list of line pattern codes.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* See Also:
*		PlPlotStyle, PlPlotTitleClear
*		PlPlotTitleMark, PlPlotTitlePoint, PlPlotTitleText
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPlotTitleLine(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		eSide,			// I selector for set of lines--PL_SIDE_xxx
int		iLine,			// I line number, starting with 0
float	fFrac1,			// I left end, fraction of title line length
float	fFrac2,			// I right end, fraction of title line length
int		ePattern,		// I line pattern--PL_LINE_XXX
float	fPts,			// I thickness of line, in points
COLORREF rgb)			// I color to use for printing
{
	int		retStat=PL_OK, inpErr=0;
	HDC		hDC=0;
	HPEN	hPen, hPenOld;
	COLORREF myRgb, rgbOld;
	int		xPx, yPx;
	float	fDeg;
	int		nLines;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		eSide must be one of the PL_SIDE_xxx values
* 4		iLine must be >= 0
* 5		the line number be consistent with the PlPlotStyle call
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit)
	if (eSide == PL_SIDE_LEFT) nLines = pPlot->nTitleLinesLeft;
	else if (eSide == PL_SIDE_RIGHT) nLines = pPlot->nTitleLinesRight;
	else if (eSide == PL_SIDE_TOP) nLines = pPlot->nTitleLinesTop;
	else if (eSide == PL_SIDE_BOTTOM) nLines = pPlot->nTitleLinesBottom;
	else PL_INP_CHK(3, 1, sub_exit)
	PL_INP_CHK(4, iLine < 0, sub_exit)
	PL_INP_CHK(5, iLine >= nLines, sub_exit)

	retStat = PlWinHDCSetup(pPlot, 0, 0, 0, ePattern, fPts, 0, rgb,
			&hDC, &hPen, &hPenOld, &myRgb);
	if (retStat != PL_OK) goto done;
	rgbOld = SetTextColor(hDC, myRgb);

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	PlTitleLineToPx(pPlot, nLines,
		pPlot->rectPlot.left, pPlot->rectPlot.right,
		pPlot->rectPlot.top, pPlot->rectPlot.bottom,
		pPlot->fTitlePts, eSide, iLine, fFrac1, &xPx, &yPx, &fDeg);
	if (fDeg == 0.F)
		yPx += PL_YPTS_TO_YPIX(pPlot->fTitlePts + 2.F) / 2;
	else
		xPx += PL_XPTS_TO_XPIX(pPlot->fTitlePts + 2.F) / 2;
	MoveToEx(hDC, xPx, yPx, NULL);
	PlTitleLineToPx(pPlot, nLines,
		pPlot->rectPlot.left, pPlot->rectPlot.right,
		pPlot->rectPlot.top, pPlot->rectPlot.bottom,
		pPlot->fTitlePts, eSide, iLine, fFrac2, &xPx, &yPx, &fDeg);
	if (fDeg == 0.F)
		yPx += PL_YPTS_TO_YPIX(pPlot->fTitlePts + 2.F) / 2;
	else
		xPx += PL_XPTS_TO_XPIX(pPlot->fTitlePts + 2.F) / 2;
	if (LineTo(hDC, xPx, yPx) == 0)
		goto gdi_error;
	SetPixel(hDC, xPx, yPx, myRgb);

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (hDC != 0) SetTextColor(hDC, rgbOld);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
sub_exit:
	PL_IF_INP_ERR("PlPlotTitleLine")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPlotTitleMark - draw plot marks in a plot title line
*
* Purpose:
*		Draw plot marks in a plot title line, one at each end of the
*		specified segment.
*
*		The 'eSide' argument selects the set of title lines, corresponding
*		to the desired side of the plot window:
*
*			PL_SIDE_LEFT      left side
*			PL_SIDE_RIGHT     right side
*			PL_SIDE_TOP       top side
*			PL_SIDE_BOTTOM    bottom side
*
*		The 'iLine' argument selects a particular line within the selected
*		set, starting with line number 0.
*
*		The segment is specified as the line number and two values
*		(between 0.0 and 1.0) that are fractions of the line length.
*
*		See PlDrawMark for a list of plot marks that can be drawn.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If the segment length is zero (i.e., the left and right end have
*		the same value), then a single plot mark is drawn.
*
* See Also:
*		PlPlotStyle, PlPlotTitleClear
*		PlPlotTitleLine, PlPlotTitlePoint, PlPlotTitleText
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPlotTitleMark(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		eSide,			// I selector for set of lines--PL_SIDE_xxx
int		iLine,			// I line number, starting with 0
float	fFrac1,			// I left end, fraction of title line length
float	fFrac2,			// I right end, fraction of title line length
int		eMark,			// I code for mark
float	fPts,			// I size of mark, in points
COLORREF rgb)			// I color to use for printing
{
	int		retStat=PL_OK, inpErr=0;
	HDC		hDC=0;
	HPEN	hPen, hPenOld;
	COLORREF myRgb, rgbOld;
	int		xPx, yPx;
	float	fFrac, fDeg;
	int		wid, ht, nLines;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		eSide must be one of the PL_SIDE_xxx values
* 4		iLine must be >= 0
* 5		the line number be consistent with the PlPlotStyle call
* 6		eDrawType must be a valid plot mark type
* 7		fPts must be >= 1
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit)
	if (eSide == PL_SIDE_LEFT) nLines = pPlot->nTitleLinesLeft;
	else if (eSide == PL_SIDE_RIGHT) nLines = pPlot->nTitleLinesRight;
	else if (eSide == PL_SIDE_TOP) nLines = pPlot->nTitleLinesTop;
	else if (eSide == PL_SIDE_BOTTOM) nLines = pPlot->nTitleLinesBottom;
	else PL_INP_CHK(3, 1, sub_exit)
	PL_INP_CHK(4, iLine < 0, sub_exit)
	PL_INP_CHK(5, iLine >= nLines, sub_exit)
	PL_INP_CHK(6, eMark < PL_SQUARE || eMark > PL_CROSS, sub_exit);
	PL_INP_CHK(7, fPts < 1.F, sub_exit)

	retStat = PlWinHDCSetup(pPlot, 0, 0, 0, PL_LINE, .5F, 0, rgb,
			&hDC, &hPen, &hPenOld, &myRgb);
	if (retStat != PL_OK) goto done;
	rgbOld = SetTextColor(hDC, myRgb);

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	ht = PL_YPTS_TO_YPIX(fPts);
	wid = PL_XPTS_TO_XPIX(fPts);
	fFrac = fFrac1;
	while (1) {
		PlTitleLineToPx(pPlot, nLines,
			pPlot->rectPlot.left, pPlot->rectPlot.right,
			pPlot->rectPlot.top, pPlot->rectPlot.bottom,
			pPlot->fTitlePts, eSide, iLine, fFrac, &xPx, &yPx, &fDeg);
		if (fDeg == 0.F)
			yPx += PL_YPTS_TO_YPIX(pPlot->fTitlePts + 2.F) / 2;
		else
			xPx += PL_XPTS_TO_XPIX(pPlot->fTitlePts + 2.F) / 2;
		retStat = PlDraw_discrete(pPlot, hDC, xPx, yPx, wid, ht, eMark,
					0, 0, 0, 0, 0, 0, myRgb);
		if (retStat != PL_OK) goto done;
		if (fFrac == fFrac2)
			break;
		fFrac = fFrac2;
	}

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (hDC != 0) SetTextColor(hDC, rgbOld);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
sub_exit:
	PL_IF_INP_ERR("PlPlotTitleMark")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPlotTitlePoint - draw plot points in a plot title line
*
* Purpose:
*		Draw plot points in a plot title line.  The number of points drawn
*		depends on the size of the points and the length of the segment.
*
*		The 'eSide' argument selects the set of title lines, corresponding
*		to the desired side of the plot window:
*
*			PL_SIDE_LEFT      left side
*			PL_SIDE_RIGHT     right side
*			PL_SIDE_TOP       top side
*			PL_SIDE_BOTTOM    bottom side
*
*		The 'iLine' argument selects a particular line within the selected
*		set, starting with line number 0.
*
*		The segment is specified as the line number and two values
*		(between 0.0 and 1.0) that are fractions of the line length.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If the segment length is zero (i.e., the left and right end have
*		the same value), then a single point is drawn.
* 2.	If the point thickness is specified as 0., then the thinnest possible
*		point will be drawn.
*
* See Also:
*		PlPlotStyle, PlPlotTitleClear
*		PlPlotTitleLine, PlPlotTitleMark, PlPlotTitleText
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPlotTitlePoint(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		eSide,			// I selector for set of lines--PL_SIDE_xxx
int		iLine,			// I line number, starting with 0
float	fFrac1,			// I left end, fraction of title line length
float	fFrac2,			// I right end, fraction of title line length
float	fPts,			// I thickness of point, in points
COLORREF rgb)			// I color to use for printing
{
	int		retStat=PL_OK, inpErr=0;
	HDC		hDC=0;
	COLORREF myRgb;
	float	fDeg;
	int		xPx, xPx1, xPx2, yPx, yPx1, yPx2, nLines;
	int		widPx, htPx;
	int		xDelPx=0, yDelPx=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		eSide must be one of the PL_SIDE_xxx values
* 4		iLine must be >= 0
* 5		the line number must be consistent with the PlPlotStyle call
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit)
	if (eSide == PL_SIDE_LEFT) nLines = pPlot->nTitleLinesLeft;
	else if (eSide == PL_SIDE_RIGHT) nLines = pPlot->nTitleLinesRight;
	else if (eSide == PL_SIDE_TOP) nLines = pPlot->nTitleLinesTop;
	else if (eSide == PL_SIDE_BOTTOM) nLines = pPlot->nTitleLinesBottom;
	else PL_INP_CHK(3, 1, sub_exit)
	PL_INP_CHK(4, iLine < 0, sub_exit)
	PL_INP_CHK(5, iLine >= nLines, sub_exit)

	retStat = PlWinHDCSetup(pPlot, 0, 0, 0, PL_LINE, 0.F, 0, rgb,
			&hDC, NULL, NULL, &myRgb);
	if (retStat != PL_OK) goto done;

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	PlTitleLineToPx(pPlot, nLines,
			pPlot->rectPlot.left, pPlot->rectPlot.right,
			pPlot->rectPlot.top, pPlot->rectPlot.bottom,
			pPlot->fTitlePts, eSide, iLine, fFrac1,
			&xPx1, &yPx1, &fDeg);
	PlTitleLineToPx(pPlot, nLines,
			pPlot->rectPlot.left, pPlot->rectPlot.right,
			pPlot->rectPlot.top, pPlot->rectPlot.bottom,
			pPlot->fTitlePts, eSide, iLine, fFrac2,
			&xPx2, &yPx2, &fDeg);

	if (fPts == 0.F && !pPlot->bPrinter)
		widPx = htPx = 1;
	else {
		widPx = PlWinCvtPtsToPix(pPlot, 1, 0, fPts);
		htPx = PlWinCvtPtsToPix(pPlot, 0, 1, fPts);
	}
	if (fDeg == 0.F) {
		yPx1 = yPx2 += PL_YPTS_TO_YPIX(pPlot->fTitlePts + 2.F) / 2;
		xDelPx = 3 * widPx;
	}
	else {
		xPx1 = xPx2 += PL_XPTS_TO_XPIX(pPlot->fTitlePts + 2.F) / 2;
		yDelPx = -3 * htPx;
	}
	for (xPx=xPx1,yPx=yPx2; xPx<=xPx2&&yPx>=yPx2; xPx+=xDelPx,yPx+=yDelPx) {
		retStat = PlDraw_discrete(pPlot, hDC, xPx, yPx,
					widPx, htPx, PL_POINT,
					0, 0, 0, 0, 0, 0, myRgb);
		if (retStat != PL_OK) goto done;
	}

done:
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
sub_exit:
	PL_IF_INP_ERR("PlPlotTitlePoint")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPlotTitleText - draw text in a plot title line
*
* Purpose:
*		Draw text in a plot title line, positioning the text relative to an
*		"anchor point".
*
*		The 'eSide' argument selects the set of title lines, corresponding
*		to the desired side of the plot window:
*
*			PL_SIDE_LEFT      left side
*			PL_SIDE_RIGHT     right side
*			PL_SIDE_TOP       top side
*			PL_SIDE_BOTTOM    bottom side
*
*		The 'iLine' argument selects a particular line within the selected
*		set, starting with line number 0.
*
*		The anchor point is specified as a title line number (starting with
*		0) and a value (between 0.0 and 1.0) that is a fraction of the
*		title line length.
*
*		The 'eJust' argument specifies how the text should be positioned
*		relative to the anchor point:
*
*			PRT_LJ    left-justified
*			PRT_CEN   center-justified
*			PRT_RJ    right-justified
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* See Also:
*		PlPlotStyle, PlPlotTitleClear
*		PlPlotTitleLine, PlPlotTitleMark, PlPlotTitlePoint
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPlotTitleText(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		eSide,			// I selector for set of lines--PL_SIDE_xxx
int		iLine,			// I line number, starting with 0
float	fFrac,			// I anchor point, as fraction of title line length
const char *szText,		// I text to print
int		eJust,			// I positioning code: PRT_{LJ, CEN, RJ}
COLORREF rgb)			// I color to use for printing
{
	int		retStat=PL_OK, inpErr=0;
	HDC		hDC=0;
	HPEN	hPen, hPenOld;
	COLORREF myRgb, rgbOld;
	int		xPx, yPx;
	int		stat;
	int		eRefPt, nLines;
	float	fDeg;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		eSide must be one of the PL_SIDE_xxx values
* 4		iLine must be >= 0
* 5		the line number be consistent with the PlPlotStyle call
* 6		eJust must be a valid code
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit)
	if (eSide == PL_SIDE_LEFT) nLines = pPlot->nTitleLinesLeft;
	else if (eSide == PL_SIDE_RIGHT) nLines = pPlot->nTitleLinesRight;
	else if (eSide == PL_SIDE_TOP) nLines = pPlot->nTitleLinesTop;
	else if (eSide == PL_SIDE_BOTTOM) nLines = pPlot->nTitleLinesBottom;
	else PL_INP_CHK(3, 1, sub_exit)
	PL_INP_CHK(4, iLine < 0, sub_exit)
	PL_INP_CHK(5, iLine >= nLines, sub_exit)
	if (eJust == PRT_LJ)		eRefPt = PRT_TL;
	else if (eJust == PRT_CEN)	eRefPt = PRT_TC;
	else if (eJust == PRT_RJ)	eRefPt = PRT_TR;
	else						PL_INP_CHK(6, 1, sub_exit)

	retStat = PlWinHDCSetup(pPlot, 0, 0, 0, PL_LINE, 0.F,0, rgb,
			&hDC, &hPen, &hPenOld, &myRgb);
	if (retStat != PL_OK) goto done;
	rgbOld = SetTextColor(hDC, myRgb);

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	// Get the coordinate of the upper left corner of the line, and
	// the line's orientation.
	PlTitleLineToPx(pPlot, nLines,
		pPlot->rectPlot.left, pPlot->rectPlot.right,
		pPlot->rectPlot.top, pPlot->rectPlot.bottom,
		pPlot->fTitlePts, eSide, iLine, fFrac, &xPx, &yPx, &fDeg);
	stat = WuPrtText_font(hDC, szText, xPx, yPx, eRefPt,
			pPlot->szTitleFont, pPlot->fTitlePts, 0,
				fDeg, 0, 0, NULL, NULL);
	if (stat != 0) goto gdi_error;

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (hDC != 0) SetTextColor(hDC, rgbOld);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
sub_exit:
	PL_IF_INP_ERR("PlPlotTitleText")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPlotWrapup - wrap up plotting operations
*
* Purpose:
*		Wraps up plotting operations.
*
*		This routine should be called after all plotting is complete,
*		typically just before the window is destroyed--at program
*		exit or dialog box exit.
*
*		NOTE WELL!!  If this routine returns PL_DIALOG_BUSY, then plotting
*		can't be wrapped up at this time.  The calling program must not
*		destroy the plot window.
*
*		No more plotting operations should be done after calling this
*		routine, unless PlPlotInit_xxx is called.
*
*		When using a printer (and, thus, the PlPrtXxx routines), this
*		routine should be called only if the plot context is going to be
*		reinitialized.  PlPrtClose must be used to finalize plotting
*		operations on the printer and send the print job to the spooler.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		PL_DIALOG_BUSY if a PL.LIB dialog box is busy, or
*		another code if an error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-21-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPlotWrapup(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)

	if (strcmp(pPlot->szIntInit, "yes") == 0) {
		retStat = PlObjWrapup(pPlot);
		if (retStat != PL_OK) goto done;
		if (pPlot->paXAxis != NULL) free(pPlot->paXAxis);
		if (pPlot->paYAxis != NULL) free(pPlot->paYAxis);
		pPlot->paXAxis = pPlot->paYAxis = NULL;
		pPlot->pXAxis = &pPlot->xAxis;
		pPlot->pYAxis = &pPlot->yAxis;
		pPlot->nAxisRows = pPlot->nAxisCols = 1;

		strcpy(pPlot->szIntInit, "not");
		if (!pPlot->bPrinter)
			PlWinMouseWrapup(pPlot);
		if (pPlot->pObjCtx != NULL) free(pPlot->pObjCtx);
		if (pPlot->pMouse != NULL) free(pPlot->pMouse);
		pPlot->pObjCtx = NULL;
		pPlot->pMouse = NULL;
	}

done:
	PL_IF_INP_ERR("PlPlotWrapup")
	return retStat;
}
