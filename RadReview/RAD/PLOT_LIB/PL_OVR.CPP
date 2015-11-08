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
/* ============================== pl_ovr.cpp ============================= */
/* ======================================================================= */

/*+/mod/TOC------------------------------------------------------------------
* pl_ovr.cpp - overloaded plotting functions
*
* Purpose:
*		The pl_ovr.cpp module contains the implementation of some overloaded
*		PL.LIB functions:
*			PlAreaRangeSetXYVectors
*			PlAreaRangeSetYVector
*			PlDrawXYVectors
*			PlDrawYVector
*
* Date     Author		Revision
* -------- ------------	--------
* 09-27-95 R. Cole		created
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
#include <stdio.h>
#include <stdlib.h>
#include "pl.h"
#include "pl_pvt.h"
#include "wu.h"

/*+/subr/TOC/internal--------------------------------------------------------
* XYRANGE - macros for PlAreaRangeSetXYVectors
*
* Purpose:
*		These macros compartmentalize the code needed for an
*		incarnation of PlAreaRangeSetXYVectors.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-26-95 R. Cole		created
*--------------------------------------------------------------------------*/
#define XYRANGE(type_Vec, name_XVec, name_YVec, bIntData) \
int PlAreaRangeSetXYVectors(  \
PL_CTX	*pPlot, \
long	lNPoints, \
const type_Vec HUGE *name_XVec, \
const type_Vec HUGE *name_YVec) \
{ \
	int		retStat=PL_OK; \
	double	dXMin, dXMax, dYMin, dYMax; \
	double	dXMin_nonZero=0., dYMin_nonZero=0.; \
	long	L; \
 \
	dXMin = dXMax = name_XVec[0]; \
	dYMin = dYMax = name_YVec[0]; \
	for (L=1; L<lNPoints; L++) { \
		if (name_XVec[L] < dXMin) dXMin = name_XVec[L]; \
		if (name_XVec[L] > dXMax) dXMax = name_XVec[L]; \
		if (name_XVec[L] > 0. && (name_XVec[L] < dXMin_nonZero || dXMin_nonZero == 0.)) \
			dXMin_nonZero = name_XVec[L]; \
		if (name_YVec[L] < dYMin) dYMin = name_YVec[L]; \
		if (name_YVec[L] > dYMax) dYMax = name_YVec[L]; \
		if (name_YVec[L] > 0. && (name_YVec[L] < dYMin_nonZero || dYMin_nonZero == 0.)) \
			dYMin_nonZero = name_YVec[L]; \
	} \
	pPlot->pXAxis->dDValB_minNon0 = dXMin_nonZero; \
	pPlot->pXAxis->bIntAxis = pPlot->pXAxis->eScaleType == bIntData; \
	pPlot->pYAxis->dDValB_minNon0 = dYMin_nonZero; \
	pPlot->pYAxis->bIntAxis = pPlot->pYAxis->eScaleType == bIntData; \
	retStat = PlAreaRangeSet(pPlot, 1, dXMin, dXMax, 1, dYMin, dYMax); \
 \
	return retStat; \
}


XYRANGE(short, psXVec, psYVec, 1)
XYRANGE(long, plXVec, plYVec, 1)
XYRANGE(float, pfXVec, pfYVec, 0)
XYRANGE(double, pdXVec, pdYVec, 0)
XYRANGE(ULONG, pulXVec, pulYVec, 1)

/*+/subr/TOC/internal--------------------------------------------------------
* YRANGE - macros for PlAreaRangeSetYVector
*
* Purpose:
*		These macros compartmentalize the code needed for an
*		incarnation of PlAreaRangeSetYVector.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-26-95 R. Cole		created
*--------------------------------------------------------------------------*/
#define YRANGE(type_Vec, name_YVec, bIntData) \
int PlAreaRangeSetYVector(  \
PL_CTX	*pPlot, \
long	lNPoints, \
double	dXLeft, \
double	dXIncr, \
const type_Vec HUGE *name_YVec) \
{ \
	int		retStat=PL_OK; \
	double	dXMin, dXMax, dYMin, dYMax; \
	double	dXMin_nonZero=0., dYMin_nonZero=0.; \
	long	L; \
 \
	dXMin = dXLeft; \
	dXMax = dXLeft + (lNPoints - 1) * dXIncr; \
	dYMin = dYMax = name_YVec[0]; \
	for (L=1; L<lNPoints; L++) { \
		if (name_YVec[L] < dYMin) dYMin = name_YVec[L]; \
		if (name_YVec[L] > dYMax) dYMax = name_YVec[L]; \
		if (name_YVec[L] > 0. && (name_YVec[L] < dYMin_nonZero || dYMin_nonZero == 0.)) \
			dYMin_nonZero = name_YVec[L]; \
	} \
	if (dXLeft > 0.) dXMin_nonZero = dXLeft; \
	else dXMin_nonZero = dXLeft + dXIncr; \
	pPlot->pXAxis->dDValB_minNon0 = dXMin_nonZero; \
	pPlot->pXAxis->bIntAxis = pPlot->pXAxis->eScaleType == 1; \
	pPlot->pYAxis->dDValB_minNon0 = dYMin_nonZero; \
	pPlot->pYAxis->bIntAxis = pPlot->pYAxis->eScaleType == bIntData; \
	retStat = PlAreaRangeSet(pPlot, 1, dXMin, dXMax, 1, dYMin, dYMax); \
 \
	return retStat; \
}

YRANGE(short, psYVec, 1)
YRANGE(long, plYVec, 1)
YRANGE(float, pfYVec, 0)
YRANGE(double, pdYVec, 0)
YRANGE(ULONG, pulYVec, 1)

/*+/csubr/TOC/internal-------------------------------------------------------
* PlPOINTPlot - plot an array of POINT structures
*
* Purpose:
*		Plots an array of POINT structures.
*
*		If the number of points is 1, then PL_LINE... gets plotted as
*		though it were PL_POINT.
*
* Return Value:
*		PL_OK, or
*		other error codes
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-27-95 R. Cole		created
*--------------------------------------------------------------------------*/
int
PlPOINTPlot(
PL_CTX	*pPlot,			// I pointer to plot context structure
HDC		hDC,			// I HDC
const RECT *pRectPts,	// I pointer to rect bounding points, or NULL
const POINT *aPts,		// I array of POINT structures
int		nPts,			// I number of array elements
int		eDrawType,		// I PL_LINE... or PL_POINT or PL_... mark
float	fPts,			// I drawing thickness or mark size, in points
COLORREF rgb)			// I color to use for drawing
{
	int		retStat=PL_OK;
	HBRUSH	hBrush=0, hOldBrush=0;
	HPEN	hPen=0, hOldPen=0;
	int		stat, j;
	RECT	rectPts;

	// If the caller has told us the RECT that bounds the points to be
	// drawn, check to see if the RECT (or a part of it) lies within
	// the area of the window that needs to be drawn.  If not, just
	// exit from this routine.
	if (pRectPts != NULL) {
		rectPts = *pRectPts;
		// Make the rect follow the Windows convention.
		rectPts.right += 1;
		rectPts.bottom += 1;

		if (pPlot->hRgnPaint != 0) {
			if (!RectInRegion(pPlot->hRgnPaint, &rectPts))
				goto done;
		}
		if (!RectVisible(hDC, &rectPts))
			goto done;
	}

	// Now draw the actual data points.  There is a separate section
	// here for each drawing type.  One of the overriding objectives
	// in how this is coded is to minimize the number of subroutine
	// calls.
	if (nPts >= 2 && eDrawType >= PL_LINE &&
					eDrawType <= PL_LINE_DASHDOTDOT) {
// Draw all "line" types.
		stat = Polyline(hDC, aPts, nPts);
		if (stat == 0) {
			// In some cases (not well understood), Polyline
			// fails.  When this happens, draw the line one
			// point at a time.
			MoveToEx(hDC, aPts[0].x, aPts[0].y, NULL);
			for (j=1; j<nPts; j++) 
				LineTo(hDC, aPts[j].x, aPts[j].y);
		}
		SetPixel(hDC, aPts[nPts-1].x, aPts[nPts-1].y, rgb);
	}
	else if (eDrawType >= PL_SQUARE && eDrawType <= PL_CROSS) {
// Draw all "plot mark" types.
		int		wid, ht;

		ht = PL_YPTS_TO_YPIX(fPts);
		wid = PL_XPTS_TO_XPIX(fPts);
		for (j=0; j<nPts; j++)  {
			retStat = PlDraw_discrete(pPlot, hDC,
				aPts[j].x, aPts[j].y,
				wid, ht, eDrawType,
				0, 0, 0, 0, 0, 0,
				rgb);
			if (retStat != PL_OK) goto done;
		}
	}
	else {
// Draw PL_POINT type.
		int		nX, nY;
		nX = PL_XPTS_TO_XPIX(fPts);
		nY = PL_YPTS_TO_YPIX(fPts);
		if (nX <= 1 && nY <= 1) {
			for (j=0; j<nPts; j++)
				SetPixel(hDC, aPts[j].x, aPts[j].y, rgb);
		}
		else {
			int		xL, xR, yB, yT;
			int		iHalfWid=(nX - 1)/2;
			int		iHalfHt=(nY - 1)/2;
			if ((hPen = CreatePen(PS_SOLID, 1, rgb)) == 0) goto gdi_error;
			/* KG */
			//hOldPen = SelectObject(hDC, hPen);
			hOldPen = (HPEN)SelectObject(hDC, hPen);
			if ((hBrush = CreateSolidBrush(rgb)) == 0) goto gdi_error;
			/* KG */
			//hOldBrush = SelectObject(hDC, hBrush);
			hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			for (j=0; j<nPts; j++)  {
				xL = aPts[j].x - iHalfWid;
				xR = xL + nX;
				yT = aPts[j].y - iHalfHt;
				yB = yT + nY;
				Ellipse(hDC, xL, yT, xR, yB);
			}
		}
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
/*+/subr/TOC/internal--------------------------------------------------------
* XYVEC_xxx - macros for PlDrawXYVectors
*
* Purpose:
*		These macros compartmentalize most of the code needed for an
*		incarnation of PlDrawXYVectors.
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-27-95 R. Cole		created
*--------------------------------------------------------------------------*/
#define XYVEC_DEF(type_Vec, name_XVec, name_YVec) \
int PlDrawXYVectors(  \
PL_CTX	*pPlot, \
long	lNPoints, \
const type_Vec HUGE *name_XVec, \
const type_Vec HUGE *name_YVec, \
int		eDrawType, \
float	fPts, \
COLORREF rgb)

#define XYVEC_PREAMBLE \
	int		retStat=PL_OK, inpErr=0, stat; \
	HDC	hDC=0; \
	HPEN	hPen=0, hPenOld=0; \
	RECT	rectArea; \
	COLORREF myRGB; \
	float	fDrawPts=fPts; \
	long	L, lFirstPt, lLastPt; \
	POINT	*aPts=NULL; \
	RECT	rectPts; \
	int		iPt, bFirst=1, bNeedClip0, bNeedClip; \
	double	dXPix0, dXPix, dYPix0, dYPix; \
	int		iXPix, iYPix; \
	int		pxL, pxR, pxB, pxT; \
	PL_AXIS	*pXAx=pPlot->pXAxis; \
 \
/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		lNPoints must >= 1
* 4		eDrawType must be a valid drawing type
* 5		if eDrawType is for a plot mark, then fPts must be >= 1
*--------------------------------------------------------------------------*/ \
	PL_INP_CHK(1, pPlot == NULL, sub_exit) \
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit) \
	PL_INP_CHK(3, lNPoints < 1, sub_exit) \
	if (eDrawType >= PL_LINE && eDrawType <= PL_LINE_DASHDOTDOT) \
		;	/* no action, just checking */ \
	else if (eDrawType == PL_POINT) \
		;	/* no action, just checking */ \
	else if (eDrawType >= PL_SQUARE && eDrawType <= PL_CROSS) { \
		fDrawPts = .5F;	/* no action, just checking */ \
		PL_INP_CHK(5, fPts < 1.F, sub_exit) \
	} \
	else \
		PL_INP_CHK(4, 1, sub_exit) \
 \
	if (pPlot->hRgnPaint != 0) { \
		rectArea.left = pPlot->pXAxis->pxB; \
		rectArea.right = pPlot->pXAxis->pxT+1; \
		rectArea.top = pPlot->pYAxis->pxT; \
		rectArea.bottom = pPlot->pYAxis->pxB+1; \
		if (!RectInRegion(pPlot->hRgnPaint, &rectArea)) \
			goto done; \
	} \
 \
	retStat = PlWinHDCSetup(pPlot, 0, 0, 1, eDrawType, fDrawPts, \
				0, rgb, &hDC, &hPen, &hPenOld, &myRGB); \
	if (retStat != PL_OK) goto done; \
 \
	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; } \
 \
	if (pPlot->bNeedScale) { \
		retStat = PlWinScale(pPlot); \
		if (retStat != PL_OK) goto done; \
	} \
 \
	aPts = (POINT *)malloc(2000 * sizeof(POINT)); \
	if (aPts == NULL) { retStat = PL_MALLOC_FAIL; goto done; } \
	pxL = pPlot->pXAxis->pxB; \
	pxR = pPlot->pXAxis->pxT; \
	pxB = pPlot->pYAxis->pxB, \
	pxT = pPlot->pYAxis->pxT; \
	iPt = 1;


#define XYVEC_ONLY_ONE(name_XVec, name_YVec) \
	if (lNPoints == 1) { \
		PL_WIN_CVT_X_VAL_TO_X_PIX(name_XVec[0], dXPix0) \
		PL_WIN_CVT_Y_VAL_TO_Y_PIX(name_YVec[0], dYPix0) \
		if (dXPix0 < -32767 || dXPix0 > 32766 || \
				dYPix0 < -32767 || dYPix0 > 32766) { \
			goto done; \
		} \
		aPts[0].x = (int)(dXPix0+.5); \
		aPts[0].y = (int)(dYPix0+.5); \
		retStat = PlPOINTPlot(pPlot, hDC, NULL, \
			aPts, iPt, eDrawType, fPts, myRGB); \
		if (retStat != PL_OK) goto done; \
		goto done; \
	}

// XYVEC_FIND_ENDS looks inward from each end of the vector, eliminating
// the points at the ends that are outside the visible range of the X axis.
// The search at each end stops just prior to the first point that is within
// the visible part of the axis.  The macro always produces two points (which
// may actually not be visible) to be plotted.
#define XYVEC_FIND_ENDS(name_XVec, name_YVec) \
	if (pPlot->pXAxis->fZoom > 1.F && pPlot->pXAxis->eScaleType == PL_LIN) { \
		for (lLastPt=lNPoints-1; lLastPt>1; lLastPt--) { \
			if (name_XVec[lLastPt] <= pPlot->pXAxis->dLValTS) \
				break; /* this one is visible */ \
		} \
		if (lLastPt < lNPoints - 1) lLastPt++; \
		for (lFirstPt=1; lFirstPt<lLastPt; lFirstPt++) { \
			if (name_XVec[lFirstPt-1] >= pPlot->pXAxis->dLValBS) \
				break; /* this one is visible */ \
		} \
		if (lFirstPt > 1) lFirstPt--; \
	} \
	else \
		lFirstPt = 1, lLastPt = lNPoints - 1;

// XYVEC_DO_IT processes the input points a pair at a time--point0 and
// point1 form the first pair, point1 and point2 the second, etc.  The
// technique is to convert the points from data values to pixels, store
// the pixel coordinates in an array of POINTs, and call Windows to draw
// the array.

// Several special cases exist:
// o  both points of a pair are on the left of the plot area (or both
//    are on the right):
//	This case exists mostly only when an axis is zoomed.  Since
//	the test for this case adds overhead for (but does not benefit)
//	unzoomed axes, this case is ignored.
// o  one or both points of a pair produce a pixel coordinate that exceeds
//    the capacity of an 'int':
//	a.  the existing points (if any) in the POINT array are drawn
//	b.  the intersection between the plot area and the line between
//	    points is calculated and drawn
// o  the POINT array contains 2 or more points (the last one is equal to
//    the first point in the current pair), and the 2nd point has the same
//    pixel coordinate as the first:
//	The second point isn't stored into the array.

//	for (L=lFirstPt; L<=lLastPt; L++) {
//		XYVEC_DO_IT(psXVec, psYVec)
//	}

#define XYVEC_DO_IT(name_XVec, name_YVec) \
		if (bFirst) { \
			bFirst = 0; \
			PL_WIN_CVT_X_VAL_TO_X_PIX(name_XVec[L-1], dXPix0) \
			PL_WIN_CVT_Y_VAL_TO_Y_PIX(name_YVec[L-1], dYPix0) \
			if (dXPix0 < -32767. || dXPix0 > 32766. || \
					dYPix0 < -32767. || dYPix0 > 32766.) { \
				bNeedClip0 = 1; \
			} \
			else \
				bNeedClip0 = 0; \
		} \
		else { \
			dXPix0 = dXPix; \
			dYPix0 = dYPix; \
			bNeedClip0 = bNeedClip; \
		} \
		PL_WIN_CVT_X_VAL_TO_X_PIX(name_XVec[L], dXPix) \
		PL_WIN_CVT_Y_VAL_TO_Y_PIX(name_YVec[L], dYPix) \
		if (dXPix < -32767. || dXPix > 32766. || \
					dYPix < -32767. || dYPix > 32766.) { \
			bNeedClip = 1; \
		} \
		else \
			bNeedClip = 0; \
		if (bNeedClip0 || bNeedClip) { \
			/* One or both points exceeds capacity of int, */ \
			/* which means that Windows can't deal with it. */ \
			if (iPt > 1) { \
				/* Flush existing points in buffer */ \
				retStat = PlPOINTPlot(pPlot, hDC, &rectPts, \
					aPts, iPt, eDrawType, fPts, myRGB); \
				if (retStat != PL_OK) goto done; \
			} \
			stat = WuClipIt(1, pxL, pxT, pxR, pxB, \
					&dXPix0, &dYPix0, &dXPix, &dYPix); \
			if (stat != 0) { \
				/* Draw the intersection */ \
				aPts[0].x = (int)(dXPix0+.5); \
				aPts[0].y = (int)(dYPix0+.5); \
				rectPts.left = rectPts.right = aPts[0].x; \
				rectPts.top = rectPts.bottom = aPts[0].y; \
				aPts[1].x = (int)(dXPix+.5); \
				aPts[1].y = (int)(dYPix+.5); \
				if (aPts[1].x < rectPts.left) rectPts.left = aPts[1].x; \
				if (aPts[1].x > rectPts.right) rectPts.right = aPts[1].x; \
				if (aPts[1].y < rectPts.top) rectPts.top = aPts[1].y; \
				if (aPts[1].y > rectPts.bottom) rectPts.bottom = aPts[1].y; \
				retStat = PlPOINTPlot(pPlot, hDC, &rectPts, \
					aPts, 2, eDrawType, fPts, myRGB); \
				if (retStat != PL_OK) goto done; \
			} \
			iPt = 1; \
		} \
		else { \
			iXPix = (int)(dXPix+.5); \
			iYPix = (int)(dYPix+.5); \
			if (iPt == 1) { \
				aPts[0].x = (int)(dXPix0+.5); \
				aPts[0].y = (int)(dYPix0+.5); \
				rectPts.left = rectPts.right = aPts[0].x; \
				rectPts.bottom = rectPts.top = aPts[0].y; \
				aPts[iPt].x = iXPix; \
				aPts[iPt].y = iYPix; \
				iPt++; \
			} \
			else if (iXPix != aPts[iPt-1].x || \
						iYPix != aPts[iPt-1].y) { \
				/* point differs from last stored--save it */ \
				aPts[iPt].x = iXPix; \
				aPts[iPt].y = iYPix; \
				iPt++; \
			} \
			/* enlarge the "bounding rectangle" if necessary */ \
			if (iXPix < rectPts.left) rectPts.left = iXPix; \
			if (iXPix > rectPts.right) rectPts.right = iXPix; \
			if (iYPix < rectPts.top) rectPts.top = iYPix; \
			if (iYPix > rectPts.bottom) rectPts.bottom = iYPix; \
			if (L == lLastPt || iPt >= 2000) { \
				retStat = PlPOINTPlot(pPlot, hDC, &rectPts, \
						aPts, iPt, eDrawType, fPts, myRGB); \
				if (retStat != PL_OK) goto done; \
				iPt = 1; \
			} \
		}

#define XYVEC_REST \
done: \
	if (aPts != NULL) free(aPts); \
	if (hPenOld != 0) SelectObject(hDC, hPenOld); \
	if (hPen != 0) DeleteObject(hPen); \
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC); \
sub_exit: \
	PL_IF_INP_ERR("PlDrawXYVectors") \
	return retStat;

XYVEC_DEF(short, psXVec, psYVec)
{
	XYVEC_PREAMBLE
	XYVEC_ONLY_ONE(psXVec, psYVec)
	XYVEC_FIND_ENDS(psXVec, psYVec)
	for (L=lFirstPt; L<=lLastPt; L++) {
		XYVEC_DO_IT(psXVec, psYVec)
	}
	XYVEC_REST
}

XYVEC_DEF(long, plXVec, plYVec)
{
	XYVEC_PREAMBLE
	XYVEC_ONLY_ONE(plXVec, plYVec)
	XYVEC_FIND_ENDS(plXVec, plYVec)
	for (L=lFirstPt; L<=lLastPt; L++) {
		XYVEC_DO_IT(plXVec, plYVec)
	}
	XYVEC_REST
}

XYVEC_DEF(float, pfXVec, pfYVec)
{
	XYVEC_PREAMBLE
	XYVEC_ONLY_ONE(pfXVec, pfYVec)
	XYVEC_FIND_ENDS(pfXVec, pfYVec)
	for (L=lFirstPt; L<=lLastPt; L++) {
		XYVEC_DO_IT(pfXVec, pfYVec)
	}
	XYVEC_REST
}

XYVEC_DEF(double, pdXVec, pdYVec)
{
	XYVEC_PREAMBLE
	XYVEC_ONLY_ONE(pdXVec, pdYVec)
	XYVEC_FIND_ENDS(pdXVec, pdYVec)
	for (L=lFirstPt; L<=lLastPt; L++) {
		XYVEC_DO_IT(pdXVec, pdYVec)
	}
	XYVEC_REST
}

XYVEC_DEF(ULONG, pulXVec, pulYVec)
{
	XYVEC_PREAMBLE
	XYVEC_ONLY_ONE(pulXVec, pulYVec)
	XYVEC_FIND_ENDS(pulXVec, pulYVec)
	for (L=lFirstPt; L<=lLastPt; L++) {
		XYVEC_DO_IT(pulXVec, pulYVec)
	}
	XYVEC_REST
}

/*+/subr/TOC/internal--------------------------------------------------------
* YVEC_xxx - macros for PlDrawYVector
*
* Purpose:
*		These macros compartmentalize most of the code needed for an
*		incarnation of PlDrawYVector.
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-27-95 R. Cole		created
*--------------------------------------------------------------------------*/
#define YVEC_DEF(type_YVec, name_YVec) \
int PlDrawYVector(  \
PL_CTX	*pPlot, \
long	lNPoints, \
double	dXLeft, \
double	dXIncr, \
const type_YVec HUGE *name_YVec, \
int		eDrawType, \
float	fPts, \
COLORREF rgb)

#define YVEC_PREAMBLE \
	int		retStat=PL_OK, inpErr=0, stat; \
	HDC	hDC=0; \
	HPEN	hPen=0, hPenOld=0; \
	RECT	rectArea; \
	COLORREF myRGB; \
	float	fDrawPts=fPts; \
	long	L, lFirstPt, lLastPt; \
	POINT	*aPts=NULL; \
	RECT	rectPts; \
	int		iPt, bFirst=1, bNeedClip0, bNeedClip; \
	double	dX, dXPix0, dXPix, dYPix0, dYPix; \
	int		iXPix, iYPix; \
	int		pxL, pxR, pxB, pxT; \
	PL_AXIS	*pXAx=pPlot->pXAxis; \
 \
/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		lNPoints must >= 1
* 4		eDrawType must be a valid drawing type
* 5		if eDrawType is for a plot mark, then fPts must be >= 1
*--------------------------------------------------------------------------*/ \
	PL_INP_CHK(1, pPlot == NULL, sub_exit) \
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit) \
	PL_INP_CHK(3, lNPoints < 1, sub_exit) \
	if (eDrawType >= PL_LINE && eDrawType <= PL_LINE_DASHDOTDOT) \
		;	/* no action, just checking */ \
	else if (eDrawType == PL_POINT) \
		;	/* no action, just checking */ \
	else if (eDrawType >= PL_SQUARE && eDrawType <= PL_CROSS) { \
		fDrawPts = .5F;	/* no action, just checking */ \
		PL_INP_CHK(5, fPts < 1.F, sub_exit) \
	} \
	else \
		PL_INP_CHK(4, 1, sub_exit) \
 \
	retStat = PlWinHDCSetup(pPlot, 0, 0, 1, eDrawType, fDrawPts, \
				0, rgb, &hDC, &hPen, &hPenOld, &myRGB); \
	if (retStat != PL_OK) goto done; \
 \
	if (pPlot->hRgnPaint != 0) { \
		rectArea.left = pPlot->pXAxis->pxB; \
		rectArea.right = pPlot->pXAxis->pxT+1; \
		rectArea.top = pPlot->pYAxis->pxT; \
		rectArea.bottom = pPlot->pYAxis->pxB+1; \
		if (!RectInRegion(pPlot->hRgnPaint, &rectArea)) \
			goto done; \
	} \
 \
	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; } \
 \
	if (pPlot->bNeedScale) { \
		retStat = PlWinScale(pPlot); \
		if (retStat != PL_OK) goto done; \
	} \
 \
	aPts = (POINT *)malloc(2000 * sizeof(POINT)); \
	if (aPts == NULL) { retStat = PL_MALLOC_FAIL; goto done; } \
	pxL = pPlot->pXAxis->pxB; \
	pxR = pPlot->pXAxis->pxT; \
	pxB = pPlot->pYAxis->pxB, \
	pxT = pPlot->pYAxis->pxT; \
	iPt = 1;

#define YVEC_ONLY_ONE(name_YVec) \
	if (lNPoints == 1) { \
		PL_WIN_CVT_X_VAL_TO_X_PIX(dXLeft, dXPix0) \
		PL_WIN_CVT_Y_VAL_TO_Y_PIX(name_YVec[0], dYPix0) \
		if (dXPix0 < -32767 || dXPix0 > 32766 || \
				dYPix0 < -32767 || dYPix0 > 32766) { \
			goto done; \
		} \
		aPts[0].x = (int)(dXPix0+.5); \
		aPts[0].y = (int)(dYPix0+.5); \
		retStat = PlPOINTPlot(pPlot, hDC, NULL, \
			aPts, iPt, eDrawType, fPts, myRGB); \
		if (retStat != PL_OK) goto done; \
		goto done; \
	}

#define YVEC_FIND_ENDS(name_YVec) \
	if (pPlot->pXAxis->fZoom > 1.F && pPlot->pXAxis->eScaleType == PL_LIN) { \
		dX = dXLeft + (lNPoints - 1) * dXIncr; \
		for (lLastPt=lNPoints-1; lLastPt>1; lLastPt--) { \
			dX -= dXIncr; \
			if (dX <= pPlot->pXAxis->dLValTS) \
				break; \
		} \
		dX = dXLeft; \
		for (lFirstPt=1; lFirstPt<lLastPt; lFirstPt++) { \
			dX += dXIncr; \
			if (dX >= pPlot->pXAxis->dLValBS) \
				break; \
		} \
	} \
	else \
		dX = dXLeft + dXIncr, lFirstPt = 1, lLastPt = lNPoints - 1;

// See the descriptive text for XYVEC_DO_IT for information about this
// macro.  The two macros are identical in function and strategy.  They
// differ in the way the X data value is obtained.  This macro calculates
// the X data value, whereas the other gets it from the caller's X vector.
#define YVEC_DO_IT(name_YVec) \
		if (bFirst) { \
			bFirst = 0; \
			PL_WIN_CVT_X_VAL_TO_X_PIX(dX-dXIncr, dXPix0) \
			PL_WIN_CVT_Y_VAL_TO_Y_PIX(name_YVec[L-1], dYPix0) \
			if (dXPix0 < -32767. || dXPix0 > 32766. || \
					dYPix0 < -32767. || dYPix0 > 32766.) { \
				bNeedClip0 = 1; \
			} \
			else \
				bNeedClip0 = 0; \
		} \
		else { \
			dXPix0 = dXPix; \
			dYPix0 = dYPix; \
			bNeedClip0 = bNeedClip; \
		} \
		PL_WIN_CVT_X_VAL_TO_X_PIX(dX, dXPix) \
		PL_WIN_CVT_Y_VAL_TO_Y_PIX(name_YVec[L], dYPix) \
		if (dXPix < -32767. || dXPix > 32766. || \
					dYPix < -32767. || dYPix > 32766.) { \
			bNeedClip = 1; \
		} \
		else \
			bNeedClip = 0; \
		if (bNeedClip0 || bNeedClip) { \
			/* One or both points exceeds capacity of int */ \
			/* which means that Windows can't deal with it. */ \
			if (iPt > 1) { \
				/* Flush existing points in buffer */ \
				retStat = PlPOINTPlot(pPlot, hDC, &rectPts, \
					aPts, iPt, eDrawType, fPts, myRGB); \
				if (retStat != PL_OK) goto done; \
			} \
			stat = WuClipIt(1, pxL, pxT, pxR, pxB, \
					&dXPix0, &dYPix0, &dXPix, &dYPix); \
			if (stat != 0) { \
				/* Draw the intersection */ \
				aPts[0].x = (int)(dXPix0+.5); \
				aPts[0].y = (int)(dYPix0+.5); \
				rectPts.left = rectPts.right = aPts[0].x; \
				rectPts.top = rectPts.bottom = aPts[0].y; \
				aPts[1].x = (int)(dXPix+.5); \
				aPts[1].y = (int)(dYPix+.5); \
				if (aPts[1].x < rectPts.left) rectPts.left = aPts[1].x; \
				if (aPts[1].x > rectPts.right) rectPts.right = aPts[1].x; \
				if (aPts[1].y < rectPts.top) rectPts.top = aPts[1].y; \
				if (aPts[1].y > rectPts.bottom) rectPts.bottom = aPts[1].y; \
				retStat = PlPOINTPlot(pPlot, hDC, &rectPts, \
					aPts, 2, eDrawType, fPts, myRGB); \
				if (retStat != PL_OK) goto done; \
			} \
			iPt = 1; \
		} \
		else { \
			iXPix = (int)(dXPix+.5); \
			iYPix = (int)(dYPix+.5); \
			if (iPt == 1) { \
				aPts[0].x = (int)(dXPix0+.5); \
				aPts[0].y = (int)(dYPix0+.5); \
				rectPts.left = rectPts.right = aPts[0].x; \
				rectPts.bottom = rectPts.top = aPts[0].y; \
				aPts[iPt].x = iXPix; \
				aPts[iPt].y = iYPix; \
				iPt++; \
			} \
			else if (iXPix != aPts[iPt-1].x || \
						iYPix != aPts[iPt-1].y) { \
				/* point differs from last stored--save it */ \
				aPts[iPt].x = iXPix; \
				aPts[iPt].y = iYPix; \
				iPt++; \
			} \
			/* enlarge the "bounding rectangle" if necessary */ \
			if (iXPix < rectPts.left) rectPts.left = iXPix; \
			if (iXPix > rectPts.right) rectPts.right = iXPix; \
			if (iYPix < rectPts.top) rectPts.top = iYPix; \
			if (iYPix > rectPts.bottom) rectPts.bottom = iYPix; \
			if (L == lLastPt || iPt >= 2000) { \
				retStat = PlPOINTPlot(pPlot, hDC, &rectPts, \
						aPts, iPt, eDrawType, fPts, myRGB); \
				if (retStat != PL_OK) goto done; \
				iPt = 1; \
			} \
		} \
		dX += dXIncr;

#define YVEC_REST \
done: \
	if (aPts != NULL) free(aPts); \
	if (hPenOld != 0) SelectObject(hDC, hPenOld); \
	if (hPen != 0) DeleteObject(hPen); \
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC); \
sub_exit: \
	PL_IF_INP_ERR("PlDrawYVector") \
	return retStat;

YVEC_DEF(short, psYVec)
{
	YVEC_PREAMBLE
	YVEC_ONLY_ONE(psYVec)
	YVEC_FIND_ENDS(psYVec)
	for (L=lFirstPt; L<=lLastPt; L++) {
		YVEC_DO_IT(psYVec)
	}
	YVEC_REST
}

YVEC_DEF(long, plYVec)
{
	YVEC_PREAMBLE
	YVEC_ONLY_ONE(plYVec)
	YVEC_FIND_ENDS(plYVec)
	for (L=lFirstPt; L<=lLastPt; L++) {
		YVEC_DO_IT(plYVec)
	}
	YVEC_REST
}

YVEC_DEF(float, pfYVec)
{
	YVEC_PREAMBLE
	YVEC_ONLY_ONE(pfYVec)
	YVEC_FIND_ENDS(pfYVec)
	for (L=lFirstPt; L<=lLastPt; L++) {
		YVEC_DO_IT(pfYVec)
	}
	YVEC_REST
}

YVEC_DEF(double, pdYVec)
{
	YVEC_PREAMBLE
	YVEC_ONLY_ONE(pdYVec)
	YVEC_FIND_ENDS(pdYVec)
	for (L=lFirstPt; L<=lLastPt; L++) {
		YVEC_DO_IT(pdYVec)
	}
	YVEC_REST
}

YVEC_DEF(ULONG, pulYVec)
{
	YVEC_PREAMBLE
	YVEC_ONLY_ONE(pulYVec)
	YVEC_FIND_ENDS(pulYVec)
	for (L=lFirstPt; L<=lLastPt; L++) {
		YVEC_DO_IT(pulYVec)
	}
	YVEC_REST
}
