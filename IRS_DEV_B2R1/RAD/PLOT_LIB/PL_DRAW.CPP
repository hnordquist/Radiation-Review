/* ======================================================================= */
/* ============================== pl_draw.cpp ============================ */
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
* pl_draw.cpp - draw-related plotting functions
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

/*+/csubr/TOC/internal-------------------------------------------------------
* PlDraw_discrete - draw a data mark or point and error bars
*
* Purpose:
*		Draws a mark or point (and optional error bars) at the specified
*		position.
*
*		See PlDrawMark for a list of plot marks.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
* BUGS:
* 1.	Doesn't check to see if the item is outside the current clip RECT.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-26-95 R. Cole		created
*--------------------------------------------------------------------------*/
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
COLORREF rgb)			// I color to use for drawing
{
	int		retStat=PL_OK;
	int		xL, xR, yB, yT;
	int		xGapL, xGapR, yGapB, yGapT;
	HBRUSH	hBrush=0, hOldBrush=0;
	HPEN	hPen=0, hOldPen=0;

	// Treat widths (and heights) of 2 and 3 (and larger even,odd pairs)
	// the same--i.e., i.e., enlarge the size so that the figure will always
	// be symmetric and centered on the caller's coordinate.
	if (wid % 2 == 0) wid++;
	if (ht % 2 == 0) ht++;
	xL = xPx - (wid - 1) / 2;
	xR = xL + wid - 1;
	yT = yPx - (ht - 1) / 2;
	yB = yT + ht - 1;

	xGapL = xL - 1;
	xGapR = xR + 1;
	yGapT = yT - 1;
	yGapB = yB + 1;

	if (eDrawType >= PL_SQUARE_BOLD && eDrawType <= PL_CIRCLE_BOLD) {
		int		iThickPix, iPts1, iPts3;
		// 'Bold' means at least 1 pt thick.  If the mark size is large
		// enough, then 'bold' means 1/6 of the mark size, up to a maximum
		// of 3 pts.
		iPts1 = PlWinCvtPtsToPix(pPlot, 0, 0, 1.F);
		iPts3 = PlWinCvtPtsToPix(pPlot, 0, 0, 3.F);
		iThickPix = (wid + ht) / 12;
		iThickPix = PL_MAX(iThickPix, iPts1);
		iThickPix = PL_MIN(iThickPix, iPts3);
		if ((hPen = CreatePen(PS_SOLID, iThickPix, rgb)) == 0) goto gdi_error;
		/* KG */
		//hOldPen = SelectObject(hDC, hPen);
		hOldPen = (HPEN)SelectObject(hDC, hPen);
	}
	if (eDrawType >= PL_SQUARE_SOLID && eDrawType <= PL_CIRCLE_SOLID) {
		if ((hBrush = CreateSolidBrush(rgb)) == 0) goto gdi_error;
		/* KG */
		//hOldBrush = SelectObject(hDC, hBrush);
		hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	}

#define PT_COORD(iSub, iX, iY) aPt[iSub].x = iX, aPt[iSub].y = iY;
#define SHAPE_IS(shape) eDrawType == PL_##shape || \
		eDrawType == PL_##shape##_SOLID || eDrawType == PL_##shape##_BOLD

	if (eDrawType == PL_POINT) {
		if (wid <= 1 && ht <= 1)
			SetPixel(hDC, xPx, yPx, rgb);
		else {
			if ((hPen = CreatePen(PS_SOLID, 1, rgb)) == 0) goto gdi_error;
			/* KG */
			//hOldPen = SelectObject(hDC, hPen);
			hOldPen = (HPEN)SelectObject(hDC, hPen);
			if ((hBrush = CreateSolidBrush(rgb)) == 0) goto gdi_error;
			/* KG */
			//hOldBrush = SelectObject(hDC, hBrush);
			hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, xL, yT, xR+1, yB+1);
		}
	}
	else if (SHAPE_IS(SQUARE)) {
		POINT	aPt[4];
		PT_COORD(0, xL, yT);
		PT_COORD(1, xR, yT);
		PT_COORD(2, xR, yB);
		PT_COORD(3, xL, yB);
		Polygon(hDC, aPt, 4);
	}
	else if (SHAPE_IS(TRI_UP)) {
		POINT	aPt[3];
		PT_COORD(0, xL, yB);
		PT_COORD(1, xPx, yT);
		PT_COORD(2, xR, yB);
		Polygon(hDC, aPt, 3);
		xGapL = xL + wid / 2;
		xGapR = xR - wid / 2;
	}
	else if (SHAPE_IS(TRI_DOWN)) {
		POINT	aPt[3];
		PT_COORD(0, xL, yT);
		PT_COORD(1, xR, yT);
		PT_COORD(2, xPx, yB);
		Polygon(hDC, aPt, 3);
		xGapL = xL + wid / 2;
		xGapR = xR - wid / 2;
	}
	else if (SHAPE_IS(DIAMOND)) {
		POINT	aPt[4];
		PT_COORD(0, xL, yPx);
		PT_COORD(1, xPx, yT);
		PT_COORD(2, xR, yPx);
		PT_COORD(3, xPx, yB);
		Polygon(hDC, aPt, 4);
	}
	else if (SHAPE_IS(CIRCLE))
		Ellipse(hDC, xL, yT, xR, yB);
	else if (eDrawType == PL_X || eDrawType == PL_X_BOLD) {
		MoveToEx(hDC, xL, yT, NULL);
		LineTo(hDC, xR, yB);
		SetPixel(hDC, xR, yB, rgb);
		MoveToEx(hDC, xL, yB, NULL);
		LineTo(hDC, xR, yT);
		SetPixel(hDC, xR, yT, rgb);
		xGapL = xL - 1;
		xGapR = xR + 1;
		yGapT = yT - 1;
		yGapB = yB + 1;
	}
	else if (eDrawType == PL_CROSS || eDrawType == PL_CROSS_BOLD) {
		MoveToEx(hDC, xPx, yT, NULL);
		LineTo(hDC, xPx, yB);
		SetPixel(hDC, xPx, yB, rgb);
		MoveToEx(hDC, xL, yPx, NULL);
		LineTo(hDC, xR, yPx);
		SetPixel(hDC, xR, yPx, rgb);
	}
	else if (eDrawType == PL_X || eDrawType == PL_X_BOLD) {
		MoveToEx(hDC, xL, yT, NULL);
		LineTo(hDC, xR, yB);
		SetPixel(hDC, xR, yB, rgb);
		MoveToEx(hDC, xL, yB, NULL);
		LineTo(hDC, xR, yT);
		SetPixel(hDC, xR, yT, rgb);
	}
	if (bDrawXBar) {
		int		tickHt;
		tickHt = PL_YPTS_TO_YPIX(4.F);
		yT = yPx - tickHt / 2;
		yB = yT + tickHt + 1;
		MoveToEx(hDC, xPxLeft, yT, NULL);
		LineTo(hDC, xPxLeft, yB);
		MoveToEx(hDC, xPxLeft, yPx, NULL);
		LineTo(hDC, xGapL+1, yPx);
		MoveToEx(hDC, xGapR, yPx, NULL);
		LineTo(hDC, xPxRight, yPx);
		MoveToEx(hDC, xPxRight, yT, NULL);
		LineTo(hDC, xPxRight, yB);
	}
	if (bDrawYBar) {
		int		tickWid;
		tickWid = PL_XPTS_TO_XPIX(4.F);
		xL = xPx - tickWid / 2;
		xR = xL + tickWid + 1;
		MoveToEx(hDC, xL, yPxTop, NULL);
		LineTo(hDC, xR, yPxTop);
		MoveToEx(hDC, xPx, yPxTop, NULL);
		LineTo(hDC, xPx, yGapT+1);
		MoveToEx(hDC, xPx, yGapB, NULL);
		LineTo(hDC, xPx, yPxBottom);
		MoveToEx(hDC, xL, yPxBottom, NULL);
		LineTo(hDC, xR, yPxBottom);
	}

done:
	if (hOldPen != 0) SelectObject(hDC, hOldPen);
	if (hPen != 0) DeleteObject(hPen);
	if (hOldBrush != 0) SelectObject(hDC, hOldBrush);
	if (hBrush != 0) DeleteObject(hBrush);
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlDrawErrorBars - draw a data item with error bars
*
* Purpose:
*		Draws a data item with error bars.  The data item can be drawn
*		with either a point or a plot mark.  A horizontal and/or a
*		vertical error bar can be drawn.
*
*		'eDrawType' selects whether to use a plot mark or a point at
*		the data location.  If a plot mark is used, the overall size
*		of the mark (height and width) is specified by 'fPts'.  If a
*		point is used, its thickness is specified by 'fPts'.  'eDrawType'
*		can be PL_POINT or a drawing mark (see PlDrawMark for a list).
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-26-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
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
COLORREF rgb)			// I color to use for drawing
{
	int		retStat=PL_OK;
	HPEN	hPen, hPenOld;
	HDC		hDC;
	COLORREF myRgb;
	int		wid, ht;
	int		x, xL, xR, y, yB, yT;

	retStat = PlWinHDCSetup(pPlot, 0, 0, 1,	// clip to plot area
			PL_LINE, .5F, 0, rgb,
			&hDC, &hPen, &hPenOld, &myRgb);
	if (retStat != PL_OK) goto done;

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	ht = PL_YPTS_TO_YPIX(fPts);
	wid = PL_XPTS_TO_XPIX(fPts);

	x = (int)(.5 + PlWinCvtXValToXPix(pPlot, dXVal));
	y = (int)(.5 + PlWinCvtYValToYPix(pPlot, dYVal));

	if (bDrawXBar) {
		xL = (int)(.5 + PlWinCvtXValToXPix(pPlot, dXLeft));
		xR = (int)(.5 + PlWinCvtXValToXPix(pPlot, dXRight));
	}
	if (bDrawYBar) {
		yT = (int)(.5 + PlWinCvtYValToYPix(pPlot, dYBottom));
		yB = (int)(.5 + PlWinCvtYValToYPix(pPlot, dYTop));
	}

	retStat = PlDraw_discrete(pPlot, hDC, x, y, wid, ht, eDrawType,
		bDrawXBar, xL, xR, bDrawYBar, yB, yT, myRgb);
	if (retStat != PL_OK) goto done;

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);

	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlDrawLine - draw a line between two points
*
* Purpose:
*		Draws a line between two points.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If the line thickness is specified as 0., then the thinnest possible
*		line will be drawn.
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-21-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlDrawLine(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dXVal1,			// I first X data value
double	dYVal1,			// I first Y data value
double	dXVal2,			// I second X data value
double	dYVal2,			// I second Y data value
float	fPts,			// I thickness of line, in points
COLORREF rgb)			// I color to use for drawing
{
	int		retStat=PL_OK;
	double	dX1, dY1, dX2, dY2;
	BOOL	stat;
	HDC		hDC;
	COLORREF myRgb;
	HPEN	hPen, hPenOld;

	retStat = PlWinHDCSetup(pPlot, 0, 0, 1,	// clip to plot area
			PL_LINE, fPts, 0, rgb,
			&hDC, &hPen, &hPenOld, &myRgb);
	if (retStat != PL_OK) goto done;

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	dX1 = PlWinCvtXValToXPix(pPlot, dXVal1);
	dY1 = PlWinCvtYValToYPix(pPlot, dYVal1);
	dX2 = PlWinCvtXValToXPix(pPlot, dXVal2);
	dY2 = PlWinCvtYValToYPix(pPlot, dYVal2);

	stat = WuClipIt(1, pPlot->pXAxis->pxB, pPlot->pYAxis->pxT,
			pPlot->pXAxis->pxT, pPlot->pYAxis->pxB,
			&dX1, &dY1, &dX2, &dY2);
	if (stat == 0) goto done;

	MoveToEx(hDC, (int)(dX1+.5), (int)(dY1+.5), NULL);
	if (LineTo(hDC, (int)(dX2+.5), (int)(dY2+.5)) == 0)
		goto gdi_error;
	SetPixel(hDC, (int)(dX2+.5), (int)(dY2+.5), myRgb);

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlDrawLine_patterned - draw a patterned line between two points
*
* Purpose:
*		Draws a patterned line between two points.
*
*		The allowed line patterns are:
*			PL_LINE				solid line
*			PL_LINE_DASH		dashed line
*			PL_LINE_DOT			dotted line
*			PL_LINE_DASHDOT		alternate - . - .
*			PL_LINE_DASHDOTDOT	alternate - . . - . .
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If the line thickness is specified as 0., then the thinnest possible
*		line will be drawn.
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-27-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlDrawLine_patterned(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dXVal1,			// I first X data value
double	dYVal1,			// I first Y data value
double	dXVal2,			// I second X data value
double	dYVal2,			// I second Y data value
int		ePattern,		// I line pattern--PL_LINE_XXX
float	fPts,			// I thickness of line, in points
COLORREF rgb)			// I color to use for drawing
{
	int		retStat=PL_OK;
	double	dX1, dY1, dX2, dY2;
	BOOL	stat;
	HDC		hDC;
	COLORREF myRgb;
	HPEN	hPen, hPenOld;

	retStat = PlWinHDCSetup(pPlot, 0, 0, 1,	// clip to plot area
			ePattern, fPts, 0, rgb,
			&hDC, &hPen, &hPenOld, &myRgb);
	if (retStat != PL_OK) goto done;

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	dX1 = PlWinCvtXValToXPix(pPlot, dXVal1);
	dY1 = PlWinCvtYValToYPix(pPlot, dYVal1);
	dX2 = PlWinCvtXValToXPix(pPlot, dXVal2);
	dY2 = PlWinCvtYValToYPix(pPlot, dYVal2);

	stat = WuClipIt(1, pPlot->pXAxis->pxB, pPlot->pYAxis->pxT,
			pPlot->pXAxis->pxT, pPlot->pYAxis->pxB,
			&dX1, &dY1, &dX2, &dY2);
	if (stat == 0) goto done;

	MoveToEx(hDC, (int)(dX1+.5), (int)(dY1+.5), NULL);
	if (LineTo(hDC, (int)(dX2+.5), (int)(dY2+.5)) == 0)
		goto gdi_error;
	SetPixel(hDC, (int)(dX2+.5), (int)(dY2+.5), myRgb);

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlDrawMark - draw a data mark
*
* Purpose:
*		Draws a mark at the specified position.  The mark is drawn
*		with lines that are .5 point thick.  The overall size of the
*		mark (height and width) is specified by 'fPts'.
*
*		PL_POINT            point
*		PL_SQUARE           square plot mark
*		PL_TRI_UP           triangle plot mark, pointing up
*		PL_TRI_DOWN         triangle plot mark, pointing down
*		PL_DIAMOND          diamond plot mark
*		PL_CIRCLE           circle plot mark
*		PL_SQUARE_SOLID     filled square plot mark
*		PL_TRI_UP_SOLID     filled triangle plot mark, pointing up
*		PL_TRI_DOWN_SOLID   filled triangle plot mark, pointing down
*		PL_DIAMOND_SOLID    filled diamond plot mark
*		PL_CIRCLE_SOLID     filled circle plot mark
*		PL_SQUARE_BOLD      bold square plot mark
*		PL_TRI_UP_BOLD      bold triangle plot mark, pointing up
*		PL_TRI_DOWN_BOLD    bold triangle plot mark, pointing down
*		PL_DIAMOND_BOLD     bold diamond plot mark
*		PL_CIRCLE_BOLD      bold circle plot mark
*		PL_CROSS_BOLD       bold cross plot mark
*		PL_X_BOLD           bold X plot mark
*		PL_CROSS            cross plot mark
*		PL_X                X plot mark
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-18-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlDrawMark(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dXVal,			// I X data value
double	dYVal,			// I Y data value
int		eMark,			// I code for mark
float	fPts,			// I size of mark, in points
COLORREF rgb)			// I color to use for drawing
{
	int		retStat=PL_OK;
	HPEN	hPen, hPenOld;
	HDC		hDC;
	COLORREF myRgb;
	int		wid, ht;
	int		x, xL, xR, y, yB, yT;

	retStat = PlWinHDCSetup(pPlot, 0, 0, 1,	// clip to plot area
			PL_LINE, .2F, 0, rgb,			// lines .2 points thick
			&hDC, &hPen, &hPenOld, &myRgb);
	if (retStat != PL_OK) goto done;

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	ht = PL_YPTS_TO_YPIX(fPts);
	wid = PL_XPTS_TO_XPIX(fPts);

	x = (int)(.5 + PlWinCvtXValToXPix(pPlot, dXVal));
	y = (int)(.5 + PlWinCvtYValToYPix(pPlot, dYVal));

	xL = x - wid/2;
	xR = x + wid/2;
	yT = y - ht/2;
	yB = y + ht/2;
	if (xR < pPlot->pXAxis->pxB || xL > pPlot->pXAxis->pxT)
		goto done;
	if (yB < pPlot->pYAxis->pxT || yT > pPlot->pYAxis->pxB)
		goto done;

	retStat = PlDraw_discrete(pPlot, hDC, x, y, wid, ht, eMark,
				0, 0, 0, 0, 0, 0, myRgb);
	if (retStat != PL_OK) goto done;

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlDrawPoint - draw a data point
*
* Purpose:
*		Draws a point at the specified position.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	A suitable point size for general use is .1.  This usually gives a
*		1-pixel point on the screen and produces a visible point on very-
*		high-resolution printers.
* 2.	If the point thickness is specified as 0., then the thinnest possible
*		point will be drawn.  (When drawing to a printer, the thinnest
*		legible point will be drawn--on the order of .01 inch.)
*
* See Also:
*		PlAreaFPtsAuto
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-21-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlDrawPoint(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dXVal,			// I X data value
double	dYVal,			// I Y data value
float	fPts,			// I thickness of point, in points
COLORREF rgb)			// I color to use for drawing
{
	int		retStat=PL_OK;
	double	dX, dY;
	HDC		hDC;
	COLORREF myRgb;
	int		wid, ht;

	retStat = PlWinHDCSetup(pPlot, 0, 0, 1,	// clip to plot area
			PL_LINE, fPts, 0, rgb,
			&hDC, NULL, NULL, &myRgb);
	if (retStat != PL_OK) goto done;

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	dX = PlWinCvtXValToXPix(pPlot, dXVal);
	if (dX < pPlot->pXAxis->pxB || dX > pPlot->pXAxis->pxT) goto done;
	dY = PlWinCvtYValToYPix(pPlot, dYVal);
	if (dY < pPlot->pYAxis->pxT || dY > pPlot->pYAxis->pxB) goto done;

	if (fPts == 0.F && !pPlot->bPrinter)
		wid = ht = 1;
	else {
		wid = PlWinCvtPtsToPix(pPlot, 1, 0, fPts);
		ht = PlWinCvtPtsToPix(pPlot, 0, 1, fPts);
	}
	retStat = PlDraw_discrete(pPlot, hDC, (int)(dX+.5), (int)(dY+.5),
				wid, ht, PL_POINT,
				0, 0, 0, 0, 0, 0, myRgb);
	if (retStat != PL_OK) goto done;

done:
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlDrawRect - draw a rectangle
*
* Purpose:
*		Draws a rectangle, using two points as opposite corners.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If the boundary line thickness is specified as 0., then the thinnest
*		possible boundary line will be drawn.
* 2.	If the rgbFill argument is -1, then the rectangle will not be
*		filled.
*
*-Date     Author		Revision
* -------- ------------	--------
* 05-06-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlDrawRect(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dXVal1,			// I X data value for first corner
double	dYVal1,			// I Y data value for first corner
double	dXVal2,			// I X data value for second corner
double	dYVal2,			// I Y data value for second corner
float	fPts,			// I thickness of boundary line, in points
COLORREF rgbRect,		// I color to use for drawing rectangle border
COLORREF rgbFill)		// I color to use for filling rectangle, or -1
{
	int		retStat;

	retStat = PlDrawRectRounded(pPlot, dXVal1, dYVal1, dXVal2, dYVal2,
		0.F, fPts, rgbRect, rgbFill);
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlDrawRectRounded - draw a rectangle with rounded corners
*
* Purpose:
*		Draws a rectangle with rounded corners, using two points as
*		opposite corners.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If the boundary line thickness is specified as 0., then the thinnest
*		possible boundary line will be drawn.
* 2.	If the rgbFill argument is -1, then the rectangle will not be
*		filled.
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-21-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlDrawRectRounded(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dXVal1,			// I X data value for first corner
double	dYVal1,			// I Y data value for first corner
double	dXVal2,			// I X data value for second corner
double	dYVal2,			// I Y data value for second corner
float	fCornerPts,		// I radius of rounded corner, in points
float	fPts,			// I thickness of boundary line, in points
COLORREF rgbRect,		// I color to use for drawing rectangle border
COLORREF rgbFill)		// I color to use for filling rectangle, or -1
{
	int		retStat=PL_OK;
	double	dX1, dY1, dX2, dY2, dTemp;
	int		iX1, iX2, iY1, iY2;
	double	dXMid, dYMid;
	BOOL	stat;
	HDC		hDC;
	HPEN	hPen, hPenOld;
	HBRUSH	hBrush=0, hBrushOld=0;
	COLORREF myRgbFill=rgbFill;

	if (pPlot->bPrinter && !pPlot->bPrinterColor && rgbFill != -1) {
		if (rgbFill != BLACK)
			myRgbFill = WHITE;
	}

	retStat = PlWinHDCSetup(pPlot, 0, 0, 1,	// clip to plot area
			PL_LINE, fPts, 0, rgbRect,
			&hDC, &hPen, &hPenOld, NULL);
	if (retStat != PL_OK) goto done;

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	if (myRgbFill != -1) {
		hBrush = CreateSolidBrush(myRgbFill);
		if (hBrush == 0) goto gdi_error;
		/* KG */
		//if ((hBrushOld = SelectObject(hDC, hBrush)) == 0) goto gdi_error;
		if ((hBrushOld = (HBRUSH)SelectObject(hDC, hBrush)) == 0) goto gdi_error;
	}

	// Get pixel coordinates of sides of rect and 'sort' them to
	// conform to Windows (0,0 is upper left).
	dX1 = PlWinCvtXValToXPix(pPlot, dXVal1);
	dY1 = PlWinCvtYValToYPix(pPlot, dYVal1);
	dX2 = PlWinCvtXValToXPix(pPlot, dXVal2);
	dY2 = PlWinCvtYValToYPix(pPlot, dYVal2);
	if (dX1 > dX2) {dTemp = dX1; dX1 = dX2; dX2 = dTemp; }
	if (dY1 > dY2) {dTemp = dY1; dY1 = dY2; dY2 = dTemp; }
	dXMid = (pPlot->pXAxis->pxB + pPlot->pXAxis->pxT) / 2.;
	dYMid = (pPlot->pYAxis->pxB + pPlot->pYAxis->pxT) / 2.;

	// Now clip the horizontal and vertical projections of the
	// rectangle on the visible window.  If either projection isn't
	// visible, then there is nothing to do.
	stat = WuClipIt(1, pPlot->pXAxis->pxB, pPlot->pYAxis->pxT,
			pPlot->pXAxis->pxT, pPlot->pYAxis->pxB,
			&dX1, &dYMid, &dX2, &dYMid);
	if (stat == 0) goto done;
	stat = WuClipIt(1, pPlot->pXAxis->pxB, pPlot->pYAxis->pxT,
			pPlot->pXAxis->pxT, pPlot->pYAxis->pxB,
			&dXMid, &dY1, &dXMid, &dY2);
	if (stat == 0) goto done;

	// Finally, draw the rectangle.  If it's zero width or height, just
	// draw a line.  (Add 1 to iX2 and iY2, since Windows stops drawing
	// one pixel short of specified endpoint.)
	iX1 = (int)(dX1 + .5);
	iX2 = (int)(dX2 + .5);
	iY1 = (int)(dY1 + .5);
	iY2 = (int)(dY2 + .5);
	if (iX1 == iX2 || iY1 == iY2) {
		if (iX1 == iX2 && iY1 == iY2)
			SetPixel(hDC, iX1, iX2, rgbRect);
		else {
			MoveToEx(hDC, iX1, iY1, NULL);
			if (iX1 == iX2)
				LineTo(hDC, iX2, iY2 + 1);
			else
				LineTo(hDC, iX2 + 1, iY2);
		}
	}
	else if (fCornerPts > 0.F) {
		double	dXWid, dYHt;
		dXWid = 2. * PL_XPTS_TO_XPIX(fCornerPts);
		dYHt = 2. * PL_XPTS_TO_XPIX(fCornerPts);
		if (dXWid > iX2 - iX1) dXWid = iX2 - iX1;
		if (dYHt > iY2 - iY1) dYHt = iY2 - iY1;
		RoundRect(hDC, iX1, iY1, iX2 + 1, iY2 + 1, (int)dXWid, (int)dYHt);
	}
	else
		Rectangle(hDC, iX1, iY1, iX2 + 1, iY2 + 1);

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (hBrushOld != 0) SelectObject(hDC, hBrushOld);
	if (hBrush != 0) DeleteObject(hBrush);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlDrawText - draw text in the plot area
*
* Purpose:
*		Draw text in the plot area, positioning the text relative to
*		a data coordinate.
*
*		The 'eRef' argument specifies how the text should be positioned
*		relative to the coordinate:
*
*			PRT_TL    top left of text is at X,Y
*			PRT_TC    top center
*			PRT_TR    top right
*			PRT_RC    right center
*			PRT_BL    bottom left
*			PRT_BC    bottom center
*			PRT_BR    bottom right
*			PRT_LC    left center
*			PRT_C     center
*
*		The 'fPtsOffset' argument controls how far the text should be
*		positioned from the coordinate.  'eRef' determines the direction.
*		For example, if PRT_TL is used, then the top left corner of the
*		box surrounding the text will be positioned down and to the right
*		(at a 45 degree angle) fPtsOffset from the data coordinate.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If the text has the form "abc[def]", the brackets are discarded
*		and the text between them is drawn as a superscript.
*
* See Also:
*
*-Date     Author		Revision
* -------- ------------	--------
* 06-17-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
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
COLORREF rgb)			// I color to use for printing
{
	int		retStat=PL_OK, inpErr=0;
	HDC		hDC=0;
	HPEN	hPen, hPenOld;
	COLORREF myRgb, rgbOld;
	double	dXPx, dYPx;
	int		stat;
	int		oldBkMode=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		eRef must be a valid PRT_xx positioning code from the list above
* 4		fPointsOffset must be >= 0
* 5		fPts must be >= 1
* 6		szFont must not be NULL
* 7		the length of szFont must be > 0
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, exit_sub)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), exit_sub)
	if (szText == NULL) szText = "";
	PL_INP_CHK(3, eRef < PRT_TL || eRef > PRT_C, exit_sub)
	PL_INP_CHK(4, fPtsOffset < 0.F, exit_sub)
	PL_INP_CHK(5, fPts < 1.F, exit_sub)
	PL_INP_CHK(6, szFont == NULL, exit_sub)
	PL_INP_CHK(7, strlen(szFont) == 0, exit_sub)

	retStat = PlWinHDCSetup(pPlot, 0, 0, 1,	// clip to plot area
			PL_LINE, 0.F, 0, rgb,
			&hDC, &hPen, &hPenOld, &myRgb);
	if (retStat != PL_OK) goto done;
	rgbOld = SetTextColor(hDC, myRgb);

	if (bTransparent)
		oldBkMode = SetBkMode(hDC, TRANSPARENT);
	else
		oldBkMode = SetBkMode(hDC, OPAQUE);

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	dXPx = PlWinCvtXValToXPix(pPlot, dXVal);
	dYPx = PlWinCvtYValToYPix(pPlot, dYVal);

	if (fPtsOffset > 0.F) {
		double	xDiag, yDiag, xLin, yLin;
		xLin = PL_XPTS_TO_XPIX(fPtsOffset);
		yLin = PL_YPTS_TO_YPIX(fPtsOffset);
		xDiag = .707 * xLin;
		yDiag = .707 * yLin;
		switch (eRef) {
			case PRT_TL: dXPx += xDiag;	dYPx += yDiag; break;
			case PRT_TC:				dYPx += yLin; break;
			case PRT_TR: dXPx -= xDiag;	dYPx += yDiag; break;
			case PRT_LC: dXPx += xLin;			break;
			case PRT_RC: dXPx -= xLin;			break;
			case PRT_BL: dXPx += xDiag;	dYPx -= yDiag; break;
			case PRT_BC: 				dYPx -= yLin; break;
			case PRT_BR: dXPx -= xDiag;	dYPx -= yDiag; break;
		}
	}

	stat = WuPrtText_font(hDC, szText, (int)(dXPx+.5), (int)(dYPx+.5),
					eRef, szFont, fPts, bBold, fDegrees, 0, 0, NULL, NULL);
	if (stat != 0) goto gdi_error;

done:
	if (oldBkMode != 0) SetBkMode(hDC, oldBkMode);
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (hDC != 0) SetTextColor(hDC, rgbOld);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
exit_sub:
	PL_IF_INP_ERR("PlDrawText")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/subr/TOC-----------------------------------------------------------------
* PlDrawXYVectors - draw vectors of X and Y data points
*
* Purpose:
*		Draws vectors of X and Y data points.
*
*		The type of drawing that occurs depends on 'eDrawType':
*
*		PL_POINT draws a 'pixel' at each data point, where the 'pixel'
*			is the size specified by the 'fPts' argument; if 'fPts'
*			is zero, then the 'pixel' will have a size of one pixel.
*			(When drawing to a printer, the thinnest legible point
*			will be drawn--on the order of .01 inch.)
*		PL_LINE... draws a line between each pair of data points, with
*			a line thickness specified by the 'fPts' argument; if 'fPts'
*			is zero, then the line will be one pixel thick.  The line
*			pattern is determined by 'eDrawType':
*			PL_LINE				solid line
*			PL_LINE_DASH		dashed line
*			PL_LINE_DOT			dotted line
*			PL_LINE_DASHDOT		alternate - . - .
*			PL_LINE_DASHDOTDOT	alternate - . . - . .
*		PL_<mark> draws a 'plot mark' at each data point.  The mark is drawn
*			with lines that are .5 point thick.  The overall size of the
*			mark (height and width) is specified by 'fPts'.  The mark
*			that is drawn is determined by 'eDrawType' (see PlDrawMark).
*
*		The coordinate of the first point is p...XVec[0], p...YVec[0], and
*		so on for the rest of the points.  The X and Y vectors must have
*		the same data type.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	This routine can be called by C++ routines for a variety of vector
*		types.  C and VB routines must call this routine with variant
*		names that correspond to the supported vector types.  The supported
*		vector types and the variant names are:
*			short	PlDrawXYVectors_sht
*			long	PlDrawXYVectors_lng
*			float	PlDrawXYVectors_flt
*			double	PlDrawXYVectors_dbl
*			ULONG	PlDrawXYVectors_ul
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-21-95 R. Cole		created
*--------------------------------------------------------------------------*/
/*+--------------------------------------------------------------------------
* SYNOPSIS:
* int
* PlDrawXYVectors(
* PL_CTX  *pPlot,          // I pointer to plot context structure
* long    n,               // I number of points
* const <type>  *p...XVec, // I X data vector
* const <type>  *p...YVec, // I Y data vector
* int     eDrawType,       // I drawing type
* float   fPts,            // I drawing thickness or mark size, in points
* COLORREF rgb)            // I color to use for drawing
*
*--------------------------------------------------------------------------*/
/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		n must >= 1
* 4		eDrawType must be a valid drawing type
* 5		if eDrawType is for a plot mark, then fPts must be >= 1
*--------------------------------------------------------------------------*/ \

// see pl_ovr.cpp for the actual functions

/*+/subr/TOC-----------------------------------------------------------------
* PlDrawYVector - draw a vector of Y data points
*
* Purpose:
*		Draws a vector of Y data points.
*
*		The type of drawing that occurs depends on 'eDrawType':
*
*		PL_POINT draws a 'pixel' at each data point, where the 'pixel'
*			is the size specified by the 'fPts' argument; if 'fPts'
*			is zero, then the 'pixel' will have a size of one pixel.
*			(When drawing to a printer, the thinnest legible point
*			will be drawn--on the order of .01 inch.)
*		PL_LINE... draws a line between each pair of data points, with
*			a line thickness specified by the 'fPts' argument; if 'fPts'
*			is zero, then the line will be one pixel thick.  The line
*			pattern is determined by 'eDrawType':
*			PL_LINE				solid line
*			PL_LINE_DASH		dashed line
*			PL_LINE_DOT			dotted line
*			PL_LINE_DASHDOT		alternate - . - .
*			PL_LINE_DASHDOTDOT	alternate - . . - . .
*		PL_<mark> draws a 'plot mark' at each data point.  The mark is drawn
*			with lines that are .5 point thick.  The overall size of the
*			mark (height and width) is specified by 'fPts'.  The mark
*			that is drawn is determined by 'eDrawType' (see PlDrawMark).
*
*		The coordinates of the points are:
*			dXLeft + 0 * dXIncr, p...YVec[0];
*			dXLeft + 1 * dXIncr, p...YVec[1];
*		and so on for the rest of the points.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	This routine can be called by C++ routines for a variety of vector
*		types.  C and VB routines must call this routine with variant
*		names that correspond to the supported vector types.  The supported
*		vector types and the variant names are:
*			short	PlDrawYVector_sht
*			long	PlDrawYVector_lng
*			float	PlDrawYVector_flt
*			double	PlDrawYVector_dbl
*			ULONG	PlDrawYVector_ul
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-21-95 R. Cole		created
*--------------------------------------------------------------------------*/
/*+--------------------------------------------------------------------------
* SYNOPSIS:
* int
* PlDrawYVector(
* PL_CTX  *pPlot,          // I pointer to plot context structure
* long    n,               // I number of points
* double  dXLeft,          // I X value at left end of X axis
* double  dXIncr,          // I X value spacing between points
* const <type>  *p...YVec, // I Y data vector
* int     eDrawType,       // I drawing type--PL_POINT, PL_LINE, or PL_<mark>
* float   fPts,            // I drawing thickness or mark size, in points
* COLORREF rgb)            // I color to use for drawing
*
*--------------------------------------------------------------------------*/
/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		n must >= 1
* 4		eDrawType must be a valid drawing type
* 5		if eDrawType is for a plot mark, then fPts must be >= 1
*--------------------------------------------------------------------------*/

// see pl_ovr.cpp for the actual functions

