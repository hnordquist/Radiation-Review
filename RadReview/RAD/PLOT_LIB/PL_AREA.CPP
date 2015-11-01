/* ======================================================================= */
/* ============================= pl_area.cpp ============================= */
/* ======================================================================= */

/*--------------------------------------------------------------------------
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
* pl_area.cpp - plot area plotting functions
*
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

#define SPRINT_DIM 120

typedef struct {
	WU_QUAD	quad;			// quad that bounds the annotation
	char	szText[SPRINT_DIM];// annotation text
	double	dVal;			// annotation value (for PL_LIN and PL_LOG)
	double	dLVal;			// annotation log value (for PL_LOG)
	int		nInt;			// # col for integer part
	int		nDec;			// # decimal places needed
	int		bEnable;		// 1 if this annotation is enabled
	int		bTested;		// 1 if overlap already tested
	int		bPrintOK;		// 1 if it's OK to print this annot
	int		eAnchor;		// annotation's anchor point--PRT_xx
	int		xPx;			// x coord (for X axis annotations)
	int		yPx;			// y coord (for Y axis annotations)
} ANNOT;

void
PlAutoLinearEnds(
double dBO, double dTO, double *pdBN, double *pdTN);

void
PlAutoLogEnds(
double dBO, double dTO, double *pdBN, double *pdTN);

void PASCAL
PlAreaRangeAutoEndsAxis(
	PL_AXIS	*pAxis);	// IO plot axis structure

void PASCAL
PlPlotInit_axis(
	PL_AXIS	*pAxis);	// IO pointer to plot axis structure


/*+/csubr/TOC/internal-------------------------------------------------------
* PlArea_tick - draw tick marks or grid lines
*
* Purpose:
*		Draws tick marks or grid lines using the style for the axis.
*
* Return Value:
*		PL_OK, or
*		another code if an error occurs
*
* Notes:
* 1.	To suppress a line, set either its x0 or y0 coordinate to -1.
*
*-Date     Author		Revision
* -------- ------------	--------
* 11-29-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlArea_tick(
HDC		hDC,
int		eStyle,
int		tick0_x0,
int		tick0_y0,
int		tick0_x1,
int		tick0_y1,
int		tick1_x0,
int		tick1_y0,
int		tick1_x1,
int		tick1_y1,
COLORREF rgb)
{
	int		retStat=PL_OK;
	HPEN	hPen=0, hPenOld=0;
	int		pattern=0;

	if (eStyle == PL_GRID_DOT_AXIS)		pattern = PS_DOT;
	else if (eStyle == PL_GRID_DASH_AXIS)	pattern = PS_DASH;
	if (pattern != 0) {
		if ((hPen = CreatePen(pattern, 1, rgb)) == 0) goto gdi_error;
		/* KG Type casting problem "converting form void * to struct HPEN_ *" */
		//if ((hPenOld = SelectObject(hDC, hPen)) == 0) goto gdi_error;
		if ((hPenOld = (HPEN)SelectObject(hDC, hPen)) == 0) goto gdi_error;
	}
	if (tick0_x0 >= 0 && tick0_y0 >= 0) {
		MoveToEx(hDC, tick0_x0, tick0_y0, NULL);
		if (LineTo(hDC, tick0_x1, tick0_y1) == 0) goto gdi_error;
		SetPixel(hDC, tick0_x1, tick0_y1, rgb);
	}
	if (tick1_x0 >= 0 && tick1_y0 >= 0) {
		MoveToEx(hDC, tick1_x0, tick1_y0, NULL);
		if (LineTo(hDC, tick1_x1, tick1_y1) == 0) goto gdi_error;
		SetPixel(hDC, tick1_x1, tick1_y1, rgb);
	}
done:
	if (hPenOld != 0) {
		SelectObject(hDC, hPenOld);
		DeleteObject(hPen);
	}
	return retStat;
gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaArrayInit - set up an array of plot areas
*
* Purpose:
*		Creates and initializes an array of plot areas.  This allows having
*		several plot areas within the a single plot window.  Features
*		of a plot area array include:
*		o	Most PL.LIB calls are generic--that is they are the same when
*			there is no plot area array as when there is an array.  All
*			PL.LIB operations (drawing, setting styles, etc.) that affect
*			a plot area occur for the "currently selected plot area".  This
*			imposes the requirement on the application program that the
*			affected plot area be selected (by calling PlAreaArraySelect)
*			prior to calling the PL.LIB routines that operate in the area.
*		o	PL.LIB assists the application program in moving the plot cursor
*			between plot areas.
*		o	When the plot window is zoomed, it is the plot window that gets
*			scroll bars.  Thus, zooming affects all plot areas, and when
*			the plot window is scrolled all plot areas are affected.  (Areas
*			can be exempted from zooming on an individual basis by calling
*			PlAreaZoomStyle.)
*
*		Each plot area is initialized with
*		o	no grid or axis
*		o	no annotations or titles
*		o	both the X and Y axes are linear, with a range from
*			0 to 10
*		o	zoom affects both X and Y axis
*
*		The plot area array is initialized so that the data area for all
*		plot areas are the same size.  (For example, consider an array
*		with 2 rows and 1 column--i.e., two areas stacked vertically.
*		Suppose that the top area has no annotations and no title lines
*		above or below, while the bottom area has annotations and two title
*		lines below.  Then the data area (i.e., the area enclosed by the
*		grid) will be the same size for both plot areas.  The overall
*		height of the bottom area, however, will be larger because of
*		the space required for annotations and title lines.)  PlAreaMove
*		can be used to customize the size and position of individual
*		plot areas.
*
*		If a plot area array already exists, then
*		o	the existing array is deallocated
*		o	an entirely new plot area array is created
*		o	the new array is given the same zoom factors and scroll
*			positions as the existing array
*		o	if the number of rows (or columns) is decreased and the plot
*			cursor consequently is beyond the last row (or column), then
*			the plot cursor is moved to the last row (or column)
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		another code if an error occurs
*
* Notes:
* 1.	It is necessary to call this routine only if an array of plot
*		areas is being used.
* 2.	If PlAreaArrayInit has been called, then PlPlotWrapup (or
*		PlPrtClose) must be called to free the memory that was allocated.
*		(The memory allocated by a prior PlAreaArrayInit call is free'd
*		by a subsequent PlAreaArrayInit call.)
*
* See Also:
*		PlAreaArraySelect, PlAreaMove
*		PlPlotInit_win, PlPlotInit_prt, PlPlotWrapup
*		PlAreaXStyle, PlAreaYStyle, PlAreaZoomStyle
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-21-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaArrayInit(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		nRow,			// I number of rows in axis array
float	fVertPts,		// I spacing between rows, in points
int		nCol,			// I number of columns in axis array
float	fHorizPts)		// I spacing between columns, in points
{
	int		retStat=PL_OK, inpErr=0;
	int		row, col;
	int		iRowSave, iColSave;
	float	fXZoom, fXScroll, fYZoom, fYScroll;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		nRow must be > 0
* 3		fVertPts must be >= 0.
* 4		nCol must be > 0
* 5		fHorizPts must be >= 0.
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, nRow <= 0, done)
	PL_INP_CHK(3, fVertPts < 0.F, done)
	PL_INP_CHK(4, nCol <= 0, done)
	PL_INP_CHK(5, fHorizPts < 0.F, done)

	pPlot->bNeedScale = 1;

	iRowSave = pPlot->iAxisRow;
	iColSave = pPlot->iAxisCol;

	if (pPlot->paXAxis != NULL) {
		fXZoom = pPlot->paXAxis[0].fZoom;
		fXScroll = pPlot->paXAxis[0].fScroll;
		free(pPlot->paXAxis);
	}
	else {
		fXZoom = pPlot->xAxis.fZoom;
		fXScroll = pPlot->xAxis.fScroll;
	}
	if (pPlot->paYAxis != NULL) {
		fYZoom = pPlot->paYAxis[0].fZoom;
		fYScroll = pPlot->paYAxis[0].fScroll;
		free(pPlot->paYAxis);
	}
	else {
		fYZoom = pPlot->yAxis.fZoom;
		fYScroll = pPlot->yAxis.fScroll;
	}
	pPlot->paXAxis = pPlot->paYAxis = NULL;

	// Now, allocate needed memory.
	pPlot->paXAxis = (PL_AXIS *)malloc(nRow*nCol*sizeof(PL_AXIS));
	pPlot->paYAxis = (PL_AXIS *)malloc(nRow*nCol*sizeof(PL_AXIS));

	// Find out how things went, and make sure pointers are safe.
	if (pPlot->paXAxis == NULL || pPlot->paYAxis == NULL) {
		retStat = PL_MALLOC_FAIL;
		if (pPlot->paXAxis != NULL) free(pPlot->paXAxis);
		if (pPlot->paYAxis != NULL) free(pPlot->paYAxis);
		pPlot->paXAxis = pPlot->paYAxis = NULL;
		pPlot->pXAxis = &pPlot->xAxis;
		pPlot->pYAxis = &pPlot->yAxis;
		pPlot->nAxisRows = pPlot->nAxisCols = 1;
	}
	else {
		pPlot->nAxisRows = nRow;
		pPlot->nAxisCols = nCol;
	}
	pPlot->fVertPts = fVertPts;
	pPlot->fHorizPts = fHorizPts;

	// Next, initialize the axis structures.  The new structures
	// get the zoom factor and scroll position that existed at entry.
	for (row=0; row<pPlot->nAxisRows; row++) {
		for (col=0; col<pPlot->nAxisCols; col++) {
			PlAreaArraySelect(pPlot, row, col);
			PlPlotInit_axis(pPlot->pXAxis);
			PlPlotInit_axis(pPlot->pYAxis);
			pPlot->pXAxis->fZoom = fXZoom;
			pPlot->pXAxis->fScroll = fXScroll;
			pPlot->pYAxis->fZoom = fYZoom;
			pPlot->pYAxis->fScroll = fYScroll;
		}
	}

	// Finally, put the selected plot area back to the one that
	// was selected at entry.  And also deal with the plot cursor--
	// if the plot area that contained the plot cursor has been
	// deleted, then change the plot cursor to a smaller row and/or
	// column number in the plot area array.
	if (iRowSave > nRow-1)	iRowSave = nRow - 1;
	if (iColSave > nCol-1)	iColSave = nCol - 1;
	PlAreaArraySelect(pPlot, iRowSave, iColSave);
	if (pPlot->iCursorRow > nRow-1) pPlot->iCursorRow = nRow - 1;
	if (pPlot->iCursorCol > nCol-1) pPlot->iCursorCol = nCol - 1;

done:
	PL_IF_INP_ERR("PlAreaArrayInit")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaArraySelect - select a member of an array of plot areas
*
* Purpose:
*		Selects which member of an array of plot areas will be used
*		for subsequent plotting operations.  The selected member will
*		be used until PlAreaArraySelect is called again.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		another code if an error occurs
*
* Notes:
* 1.	The top row of the array is number 0; the left column is number 0.
* 2.	If PlAreaArrayInit hasn't been called, this routine does nothing.
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-21-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaArraySelect(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		iRow,			// I row in axis array, starting with 0
int		iCol)			// I column in axis array, starting with 0
{
	int		retStat=PL_OK, inpErr=0;
	int		iElement;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		iRow must be >= 0 and < actual number of rows
* 3		iCol must be >= 0 and < actual number of cols
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, iRow < 0 || iRow >= pPlot->nAxisRows, done)
	PL_INP_CHK(3, iCol < 0 || iCol >= pPlot->nAxisCols, done)

	iElement = iRow + iCol * pPlot->nAxisRows;
	if (pPlot->paXAxis != NULL && pPlot->paYAxis != NULL) {
		pPlot->pXAxis = &pPlot->paXAxis[iElement];
		pPlot->pYAxis = &pPlot->paYAxis[iElement];
	}
	else {
		pPlot->pXAxis = &pPlot->xAxis;
		pPlot->pYAxis = &pPlot->yAxis;
	}
	pPlot->iAxisRow = iRow;
	pPlot->iAxisCol = iCol;

done:
	PL_IF_INP_ERR("PlAreaArraySelect")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlArea_annotCheck - set bPrintOK flags for annotations
*
* Purpose:
*		Checks whether annotations overlap, and sets the bPrintOK flag
*		for those annotations that don't overlap.
*
*		If the axis is linear and the number of major divisions is a
*		multiple of 2, the checks are done on a hierarchical basis.  For
*		log axes and linear axes where nMajor is not a multiple of 2,
*		checking is on a left-to-right basis.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-24-96 R. Cole		created
*--------------------------------------------------------------------------*/
void
PlArea_annotCheck(
ANNOT	*aAnnot,
int		nMajor,
int		bLogAxis,
float	fXDist,
float	fYDist)
{
	int		iAnnot;
	int		iInterval, bDoThisLevel; 
	int		iLastOK=0;

	aAnnot[0].bPrintOK = aAnnot[0].bEnable;
	aAnnot[nMajor].bPrintOK = aAnnot[nMajor].bEnable;
	if ((nMajor % 2) == 0 && !bLogAxis) {
		for (iInterval=nMajor/2; iInterval>0; iInterval/=2) {
			bDoThisLevel = 1;
			for (iAnnot=iInterval; iAnnot<nMajor; iAnnot+=iInterval) {
				if (!aAnnot[iAnnot].bEnable || aAnnot[iAnnot].bTested)
					continue;
				aAnnot[iAnnot].bTested = 1;
				if (aAnnot[iAnnot-iInterval].bEnable) {
					if (WuQuadsDoOverlap( &aAnnot[iAnnot-iInterval].quad,
								&aAnnot[iAnnot].quad, fXDist, fYDist)) {
						bDoThisLevel = 0;
						break;
					}
				}
				if (aAnnot[iAnnot+iInterval].bEnable) {
					if (WuQuadsDoOverlap( &aAnnot[iAnnot+iInterval].quad,
								&aAnnot[iAnnot].quad, fXDist, fYDist)) {
						bDoThisLevel = 0;
						break;
					}
				}
			}
			if (bDoThisLevel) {
				for (iAnnot=iInterval; iAnnot<nMajor; iAnnot+=iInterval)
					aAnnot[iAnnot].bPrintOK = 1;
			}
			else
				break;
		}
	}
	else {
		for (iAnnot=1; iAnnot<nMajor; iAnnot++) {
			if (aAnnot[iAnnot].bEnable) {
				if (aAnnot[iLastOK].bEnable) {
					if (WuQuadsDoOverlap( &aAnnot[iLastOK].quad,
								&aAnnot[iAnnot].quad, fXDist, fYDist)) {
						continue;
					}
				}
				if (aAnnot[nMajor].bEnable) {
					if (WuQuadsDoOverlap( &aAnnot[nMajor].quad,
								&aAnnot[iAnnot].quad, fXDist, fYDist)) {
						continue;
					}
				}
				aAnnot[iAnnot].bPrintOK = 1;
				iLastOK = iAnnot;
			}
		}
	}
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlArea_axisCommon - common code for annotations and ticks/grid lines
*
* Purpose:
*		o	calculates x coordinate for X axis tick marks or y coordinate
*			for Y axis tick marks
*
*		If annotations are being done,
*		o	calculates anchor point for text
*		o	creates the text strings for annotations.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-03-96 R. Cole		created
*--------------------------------------------------------------------------*/
void
PlArea_axisCommon(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		bXAxis,
ANNOT	*aAnnot,
int		nMajor)
{
	PL_AXIS	*pXAxis=pPlot->pXAxis, *pYAxis=pPlot->pYAxis;
	PL_AXIS	*pAxis=bXAxis?pXAxis:pYAxis;
	int		bLinAxis=pAxis->eScaleType == PL_LIN;
	int		bLogAxis=pAxis->eScaleType == PL_LOG;
	float	fAnnotDeg=pAxis->fAnnotDeg;
	double	dBase, dRange;
	char	*pszAnnot;
	char	*pszFmt, szFmt[20];
	char	cFmt='f';
	int		nPrec=0, maxPrec=-1;
	int		bAnnot, bAutoFmt=0;
	int		i;
	int		pixRange, pixBase;

	double	dLValL, dLValR, dLBase;
	double	dLValLFloor, dLValRCeil;
	double	dLValB, dLValT;
	double	dLValBFloor, dLValTCeil;

	dRange = pAxis->dLValTS - pAxis->dLValBS;
	if (bXAxis)
		pixRange = pAxis->pxT+1 - (pAxis->pxB-1);
	else
		pixRange = pAxis->pxB+1 - (pAxis->pxT-1);

	if (bLogAxis && bXAxis) {
		dLValL = pXAxis->dLValBS;
		dLValR = pXAxis->dLValTS;
		if (pAxis->bLogZero && pAxis->dLValBS <= pAxis->dLogZero)
			dLValLFloor = ceil(pAxis->dLogZero);
		else
			dLValLFloor = floor(dLValL);
		if (pAxis->bLogZero && pAxis->dLValTS <= pAxis->dLogZero)
			dLValRCeil = 0.;
		else
			dLValRCeil = ceil(dLValR);
	}
	else if (bLogAxis && !bXAxis) {
		dLValB = pYAxis->dLValBS;
		dLValT = pYAxis->dLValTS;
		if (pYAxis->bLogZero && pYAxis->dLValBS <= pYAxis->dLogZero)
			dLValBFloor = ceil(pYAxis->dLogZero);
		else
			dLValBFloor = floor(dLValB);
		if (pYAxis->bLogZero && pYAxis->dLValTS <= pYAxis->dLogZero)
			dLValTCeil = 0.;
		else
			dLValTCeil = ceil(dLValT);
	}

	bAnnot = pAxis->fAnnotPts > 0.F;
	pszFmt = pAxis->szAnnotFmt;
	if (strstr(pszFmt, PL_FMT_AUTO) != NULL) {
		const char *psz;
		psz = pszFmt + strlen(PL_FMT_AUTO);
		if (*psz == '.') {
			// PL_FMT_AUTO".n" was specified; grab the 'n'.
			psz++;
			maxPrec = atoi(psz);
		}
		bAutoFmt = 1;
	}

	// Get the values at the tick marks, the anchor points for the
	// annotations, and the x (or y) coordinate for X (or Y) axis
	// annotations.
	for (i=0; i<=nMajor; i++) {
		aAnnot[i].bEnable = bAnnot;
		aAnnot[i].bTested = aAnnot[i].bPrintOK = 0;
		if (i == 0) {
			if (bLinAxis) {
				aAnnot[i].dVal = dBase = pAxis->dLValBS;
				if (bXAxis)
					aAnnot[i].xPx = pixBase = pAxis->pxB - 1;
				else
					aAnnot[i].yPx = pixBase = pAxis->pxB + 1;
			}
			else {
				if (bXAxis) {
					dLBase = dLValLFloor;
					aAnnot[i].dLVal = dLValL;
				}
				else {
					dLBase = dLValBFloor;
					aAnnot[i].dLVal = dLValB;
				}
				if (pAxis->bLogZero && aAnnot[i].dLVal <= pAxis->dLogZero)
					aAnnot[i].dVal = 0.;
				else if (pAxis->fZoom == 1.F || pAxis->fScroll == 0.F)
					aAnnot[i].dVal = pAxis->dDValB;
				else
					aAnnot[i].dVal = pow(10., aAnnot[i].dLVal);
				if (bXAxis)
					aAnnot[i].xPx = pAxis->pxB - 1;
				else
					aAnnot[i].yPx = pAxis->pxB + 1;
			}
			if (fAnnotDeg == 0.F)
				aAnnot[i].eAnchor = bXAxis ? PRT_TL : PRT_BR;
			else if (fAnnotDeg == 90.F)
				aAnnot[i].eAnchor = bXAxis ? PRT_TR : PRT_BL;
			else
				aAnnot[i].eAnchor = bXAxis ? PRT_BL : PRT_TR;
		}
		else if (i == nMajor) {
			if (bLinAxis) {
				aAnnot[i].dVal = pAxis->dLValTS;
				if (bXAxis)
					aAnnot[i].xPx = pAxis->pxT + 1;
				else
					aAnnot[i].yPx = pAxis->pxT - 1;
			}
			else {
				if (bXAxis)
					aAnnot[i].dLVal = dLValR;
				else
					aAnnot[i].dLVal = dLValT;
				if (pAxis->bLogZero && aAnnot[i].dLVal <= pAxis->dLogZero)
					aAnnot[i].dVal = 0.;
				else if (pAxis->fZoom == 1.F || pAxis->fScroll == 1.F)
					aAnnot[i].dVal = pAxis->dDValT;
				else
					aAnnot[i].dVal = pow(10., aAnnot[i].dLVal);
				if (bXAxis)
					aAnnot[i].xPx = pAxis->pxT + 1;
				else
					aAnnot[i].yPx = pAxis->pxT - 1;
			}
			if (fAnnotDeg == 0.F)
				aAnnot[i].eAnchor = bXAxis ? PRT_TR : PRT_TR;
			else if (fAnnotDeg == 90.F)
				aAnnot[i].eAnchor = bXAxis ? PRT_BR : PRT_BR;
			else
				aAnnot[i].eAnchor = bXAxis ? PRT_TL : PRT_TL;
		}
		else {
			if (bLinAxis) {
				double	dFrac;
				dFrac = (double)i / (double)nMajor;
				if (dFrac == .5) {
					if (bXAxis) {
						if (pPlot->pYAxis->eStyle == PL_CENT_HALF_AXIS)
							aAnnot[i].bEnable = 0;
					}
					else if (pPlot->pXAxis->eStyle == PL_CENT_HALF_AXIS)
							aAnnot[i].bEnable = 0;
				}
				aAnnot[i].dVal = dBase + dRange * dFrac;
				if (bXAxis)
					aAnnot[i].xPx = pixBase + (int)(.5 + pixRange * dFrac);
				else
					aAnnot[i].yPx = pixBase - (int)(.5 + pixRange * dFrac);
			}
			else {
				double	dTemp;
				if (bXAxis)
					aAnnot[i].dLVal = dLValLFloor + i;
				else
					aAnnot[i].dLVal = dLValBFloor + i;
				if (pAxis->bLogZero && aAnnot[i].dLVal <= pAxis->dLogZero)
					aAnnot[i].dVal = 0.;
				else
					aAnnot[i].dVal = pow(10., aAnnot[i].dLVal);
				if (bXAxis) {
					PL_WIN_CVT_X_VAL_TO_X_PIX(aAnnot[i].dVal, dTemp);
					aAnnot[i].xPx = (int)(.5 + dTemp);
				}
				else {
					PL_WIN_CVT_Y_VAL_TO_Y_PIX(aAnnot[i].dVal, dTemp);
					aAnnot[i].yPx = (int)(.5 + dTemp);
				}
			}
			if (fAnnotDeg == 0.F)
				aAnnot[i].eAnchor = bXAxis ? PRT_TC : PRT_RC;
			else if (fAnnotDeg == 90.F)
				aAnnot[i].eAnchor = bXAxis ? PRT_RC : PRT_BC;
			else
				aAnnot[i].eAnchor = bXAxis ? PRT_LC : PRT_TC;
		}
		if (pAxis->bIntAxis && pAxis->eScaleType == PL_LIN) {
			double	dNear; 
			// If this is a linear axis with integral data, and if the
			// annotation value is "close to" the nearest data value,
			// then force the annotation value to be _exactly_ the data
			// value.
			dNear = floor(aAnnot[i].dVal + .5);
			if (fabs(aAnnot[i].dVal - dNear) < .001)
				aAnnot[i].dVal = dNear;
		}
	}

	if (!bAnnot)
		goto done;

	// Do the processing to figure out what format to use when the
	// caller has asked for "auto format"--PL_FMT_AUTO.
	if (bAutoFmt) {
		int		nCol=pAxis->iAnnotNChar;
		int		maxSign=0, maxInt=0, maxDot=0, maxDec=0;
		for (i=0; i<=nMajor; i++) {
			double	dVal, dTemp;
			int		nInt, nDec, nSign, nDot;
			dVal = aAnnot[i].dVal;
			nInt = 1;
			nDec = 0;
			nSign = 0;
			nDot = 0;
			if (dVal < 0.) {
				nSign = 1;
				dVal = -dVal;
			}
			if (dVal > 0. && dVal < 1.) {
				nDot = 1;
				nDec = (-1) * (int)floor(log10(dVal));
				while (nSign + nInt + nDot + nDec < nCol) {
					dTemp = dVal * pow(10., nDec);
					if (fmod(dTemp, 1.) == 0.)
						break;
					nDec++;
				}
			}
			else if (dVal >= 1.) {
				double	dFracPart=fmod(dVal, 1.);
				nInt =(int)ceil(log10(dVal));
				if (fmod(dVal, 10.) == 0)
					nInt++;
				if (dFracPart != 0.) {
					nDot = 1;
					nDec = 1;
					while (nSign + nInt + nDot + nDec < nCol) {
						dTemp = dFracPart * pow(10., nDec);
						if (fmod(dTemp, 1.) == 0.)
							break;
						nDec++;
					}
				}
			}
			// See if there will be trailing zeros when the
			// value is printed.  If so, then decrease the
			// number of decimal places to eliminate the zeros.
			if (nInt < 10 && nDec > 0 && nDec < 10) {
				char	szTemp[100];
				int		iCh;
				sprintf(szFmt, "%%.%df", nDec);
				sprintf(szTemp, szFmt, dVal);
				iCh = strlen(szTemp) - 1;
				while (iCh > 0 && szTemp[iCh]=='0') {
					iCh--;
					nDec--;
				}
			}
			if (i > 0) {
				// Eliminate "duplicate" annotations that
				// result from successive values that are
				// too close together to be "seen" by the
				// current precision.
				double	dDiff;
				int		nDiffDec;
				dDiff = fabs(aAnnot[i].dVal - aAnnot[i-1].dVal);
				if (dDiff > 0. && dDiff < 1.) {
					nDiffDec = (-1) * (int)floor(log10(dDiff));
					if (nDiffDec > nDec)
						nDec = nDiffDec;
				}
			}
			if (maxPrec >= 0 && nDec > maxPrec)
				nDec = maxPrec;
			if (nSign + nInt + nDot >= nCol) {
				// There's no room for fractional part.
				nDec = nDot = 0;
			}
			// Decide whether to use E format.
			if (nSign + nInt > nCol)
				cFmt = 'e';	// number is too big for %f
			else if (dVal > -1. && dVal < 1. &&
					nSign + nInt + nDot + nDec > nCol) {
				cFmt = 'e';	// number is too small for %f
			}
			else if (nCol > nSign + 3 + 5 && nSign + nInt + nDot + nDec > nCol) {
				// There's room for +n.nE+ppp, and E is
				// needed.  (If there's not room, E won't
				// be used.)
				cFmt = 'e';
			}
			nPrec = PL_MAX(nPrec, nDec);
			maxSign = PL_MAX(maxSign, nSign);
			maxInt = PL_MAX(maxInt, nInt);
			maxDot = PL_MAX(maxDot, nDot);
			maxDec = PL_MAX(maxDec, nDec);
			aAnnot[i].nInt = nSign + nInt;
			aAnnot[i].nDec = nDec;
		}
		if (cFmt == 'e') {
			int		iTemp;
			// Number of decimal places is: #col -
			// optional_sign - digit - dec_pt - E+000.
			iTemp = nCol - maxSign - 1 - 1 - 5;
			if (iTemp > 0)
				nPrec = iTemp;
			else {
				// If only one significant digit can
				// be shown with %e, see if a better job
				// can be done with %f.
				if (maxSign + maxInt <= nCol) {
					if (maxDec == 0) {
						cFmt = 'f';
						nPrec = 0;
					}
					else if (maxSign + maxInt + 1 < nCol) {
						cFmt = 'f';
						nPrec = nCol - maxSign - maxInt - 1;
					}
					else
						nPrec = 0;
				}
				else
					nPrec = 0;
			}
		}
		sprintf(szFmt, "%%.%d%c", nPrec, cFmt);
		pszFmt = szFmt;
	}
	// Create the annotation text.
	for (i=0; i<=nMajor; i++) {
		if (pAxis->pAnnotFn == NULL) {
default_annot:
			if (bLogAxis && strcmp(pszFmt, PL_FMT_TENTO) == 0) {
				if (aAnnot[i].dVal == 0.)
					strcpy(aAnnot[i].szText, "0");
				else
					_snprintf(aAnnot[i].szText, SPRINT_DIM, "10[%.0f]", aAnnot[i].dLVal);
			}
			else
				_snprintf(aAnnot[i].szText, SPRINT_DIM, pszFmt, aAnnot[i].dVal);
			aAnnot[i].szText[SPRINT_DIM-1] = '\0';
		}
		else {
			pszAnnot = (char *)pAxis->pAnnotFn(pPlot,
											pAxis->pAnnotArg, aAnnot[i].dVal);
			WuSzMCopy(aAnnot[i].szText, SPRINT_DIM, pszAnnot);
			if (strcmp(pszAnnot, PL_FMT_DFLT) == 0)
				goto default_annot;
		}
	}
done:
	;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlArea_xAxisLin - do the actual work for a PL_LIN axis
*
* Purpose:
*		Draws the annotations for a PL_LIN X axis and applies the grid lines
*		or tick marks.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
* Notes:
* 1.	If yMinB_0 is -1, then minor tick marks won't be drawn on the
*		bottom X axis.  And similarly for yMinT_0.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-17-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlArea_xAxisLin(
PL_CTX	*pPlot,			// I pointer to plot context structure
HDC		hDC,			// I handle to device context
COLORREF rgb,			// I color for drawing text
int		bNoVSpace,		// I 1 to suppress internal leading and/or descent
float	fXHalfFrac,		// I where (along Y axis) the axis has been drawn
int		yPx,			// I vertical position for uppermost side of annot.
int		yMajB_0,		// I lower end of major tick on bottom axis
int		yMajB_1,		// I upper end of major tick on bottom axis
int		yMajT_0,		// I upper end of major tick on top axis
int		yMajT_1,		// I lower end of major tick on top axis
int		yMinB_0,		// I lower end of minor tick on bottom axis, or -1
int		yMinB_1,		// I upper end of minor tick on bottom axis
int		yMinT_0,		// I upper end of minor tick on top axis, or -1
int		yMinT_1)		// I lower end of minor tick on top axis
{
	int		retStat=PL_OK;
	PL_AXIS	*pXAxis=pPlot->pXAxis, *pYAxis=pPlot->pYAxis;
	int		nMajor=pXAxis->nMajor, nMinor=pXAxis->nMinor;
	float	fAnnotDeg=pXAxis->fAnnotDeg;
	float	fXDist, fYDist;
	int		iAnnot;
	int		stat, j;
	int		xPx, xRange;
	int		bAnnot=0;
	ANNOT	*aAnnot=NULL;

	if (pXAxis->eStyle == PL_NO_AXIS) goto done;

	if (pXAxis->fAnnotPts > 0.F) {
		bAnnot = 1;
		fXDist = (float)PL_XPTS_TO_XPIX(pXAxis->fAnnotPts * .25);
		fYDist = (float)PL_YPTS_TO_YPIX(pXAxis->fAnnotPts * .25);
	}

	xRange = pXAxis->pxT+1 - (pXAxis->pxB-1);
	if (nMajor < 1)
		nMajor = 1;

	aAnnot = (ANNOT *)malloc(sizeof(ANNOT)*(nMajor+1));
	if (aAnnot == NULL) {
		retStat = PL_MALLOC_FAIL;
		goto done;
	}
	PlArea_axisCommon(pPlot, 1, aAnnot, nMajor);	// do X axis

	// Go through all the annotations, getting their quads.
	for (iAnnot=0; iAnnot<=nMajor; iAnnot++) {
		if (aAnnot[iAnnot].bEnable && bAnnot) {
			stat = WuPrtText_font(hDC, aAnnot[iAnnot].szText,
								aAnnot[iAnnot].xPx, yPx, aAnnot[iAnnot].eAnchor,
								pXAxis->szAnnotFont, pXAxis->fAnnotPts,
								0, fAnnotDeg, bNoVSpace,
								1, NULL, &aAnnot[iAnnot].quad);
			if (stat != 0) goto gdi_error;
		}
	}
	// Now go through all the annotations, checking for overlaps.  If an
	// annotation doesn't overlap its neighbors, it will have bPrintOK
	// set to 1, allowing it to be printed.
	if (bAnnot)
		PlArea_annotCheck(aAnnot, nMajor, 0, fXDist, fYDist);

	// Finally, draw the annotations and tick marks.
	for (iAnnot=0; iAnnot<=nMajor; iAnnot++) {
		xPx = aAnnot[iAnnot].xPx;
		if ((iAnnot != 0 && iAnnot != nMajor) ||
								pXAxis->eStyle == PL_EDGE_HALF_AXIS ||
								pXAxis->eStyle == PL_CENT_HALF_AXIS) {
			// For non-endpoint spots, draw major "tick" marks.
			retStat = PlArea_tick(hDC, pXAxis->eStyle,
								xPx, yMajB_0, xPx, yMajB_1,
								xPx, yMajT_0, xPx, yMajT_1, rgb);
			if (retStat != PL_OK) goto done;
		}
		if (aAnnot[iAnnot].bPrintOK) {
			stat = WuPrtText_font(hDC, aAnnot[iAnnot].szText,
								xPx, yPx, aAnnot[iAnnot].eAnchor,
								pXAxis->szAnnotFont, pXAxis->fAnnotPts,
								0, fAnnotDeg, bNoVSpace,
								0, NULL, NULL);
			if (stat != 0) goto gdi_error;
		}
		if (iAnnot < nMajor && nMinor > 1) {
			// Draw the minor tick marks.
			for (j=1; j<nMinor; j++) {
				int		xMinPx;
				xMinPx = (int)(.5 + xPx + (double)j /
										(double)nMinor * xRange / nMajor);
				if (yMinB_0 >= 0) {
					MoveToEx(hDC, xMinPx, yMinB_0, NULL);
					if (LineTo(hDC, xMinPx, yMinB_1-1) == 0)
						goto gdi_error;
				}
				if (yMinT_0 >= 0) {
					MoveToEx(hDC, xMinPx, yMinT_0, NULL);
					if (LineTo(hDC, xMinPx, yMinT_1+1) == 0)
						goto gdi_error;
				}
			}
		}
	}

done:
	if (aAnnot != NULL) free(aAnnot);
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlArea_xAxisLog - do the actual work for a PL_LOG axis
*
* Purpose:
*		Draws the annotations for a PL_LOG X axis and applies the grid lines
*		or tick marks.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
* Notes:
* 1.	If yMinB_0 is -1, then minor tick marks won't be drawn on the
*		bottom X axis.  And similarly for yMinT_0.
* 2.	If pXAxis->szAnnotFmt is PL_FMT_TENTO, then annotations appear as
*		10 with a superscripted power.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-17-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlArea_xAxisLog(
PL_CTX	*pPlot,			// I pointer to plot context structure
HDC		hDC,			// I handle to device context
COLORREF rgb,			// I color for drawing text
int		bNoVSpace,		// I 1 to suppress internal leading and/or descent
float	fXHalfFrac,		// I where (along Y axis) the axis has been drawn
int		yPx,			// I vertical position for uppermost side of annot.
int		yMajB_0,		// I lower end of major tick on bottom axis
int		yMajB_1,		// I upper end of major tick on bottom axis
int		yMajT_0,		// I upper end of major tick on top axis
int		yMajT_1,		// I lower end of major tick on top axis
int		yMinB_0,		// I lower end of minor tick on bottom axis, or -1
int		yMinB_1,		// I upper end of minor tick on bottom axis
int		yMinT_0,		// I upper end of minor tick on top axis, or -1
int		yMinT_1)		// I lower end of minor tick on top axis
{
	int		retStat=PL_OK;
	PL_AXIS	*pXAxis=pPlot->pXAxis, *pYAxis=pPlot->pYAxis;
	int		nMajor=pXAxis->nMajor, nMinor=pXAxis->nMinor;
	float	fAnnotDeg=pXAxis->fAnnotDeg;
	float	fXDist, fYDist;
	int		iAnnot;
	int		stat, j;
	int		xPx, xRange;
	int		bAnnot=0;
	ANNOT	*aAnnot=NULL;
	double	dLVal, dLValL, dLValR, dTemp;
	int		nDecades;	// # full decades to cover visible axis
	double	dLValLFloor, dLValRCeil;

	if (pXAxis->eStyle == PL_NO_AXIS) goto done;

	if (pXAxis->fAnnotPts > 0.F) {
		bAnnot = 1;
		fXDist = (float)PL_XPTS_TO_XPIX(pXAxis->fAnnotPts * .25);
		fYDist = (float)PL_YPTS_TO_YPIX(pXAxis->fAnnotPts * .25);
	}

	xRange = pXAxis->pxT+1 - (pXAxis->pxB-1);
	dLValL = pXAxis->dLValBS;
	dLValR = pXAxis->dLValTS;

	// Figure out how many decades are required to span the visible
	// axis.  For the special log axes that accomodate values of 0,
	// the first decade goes from 0 to nn (rather than the usual 1
	// to nn).  It is easily possible for the axis to be zoomed and
	// scrolled so that the visible part of the axis has a range of
	// .4 to .9--i.e., not only does the visible part of the axis not
	// include a decade boundary, but it doesn't even include the
	// possibility of a data point!
	if (pXAxis->bLogZero && pXAxis->dLValBS <= pXAxis->dLogZero)
		dLValLFloor = ceil(pXAxis->dLogZero);
	else
		dLValLFloor = floor(dLValL);
	if (pXAxis->bLogZero && pXAxis->dLValTS <= pXAxis->dLogZero)
		dLValRCeil = 0.;
	else
		dLValRCeil = ceil(dLValR);
	nDecades = (int)(dLValRCeil - dLValLFloor);
	if (nMajor > 1)
		nMajor = nDecades;
	if (nMajor < 1)
		nMajor = 1;

	// Since this is a log axis, we'll have 1, 2, or 9 minor subdivisions.
	if (nMinor < 1)
		nMinor = 1;
	else if (nMinor > 2)
		nMinor = 9;

	aAnnot = (ANNOT *)malloc(sizeof(ANNOT)*(nMajor+1));
	if (aAnnot == NULL) {
		retStat = PL_MALLOC_FAIL;
		goto done;
	}
	PlArea_axisCommon(pPlot, 1, aAnnot, nMajor);	// do X axis

	// Go through all the annotations, getting their quads.
	for (iAnnot=0; iAnnot<=nMajor; iAnnot++) {
		if (aAnnot[iAnnot].bEnable && bAnnot) {
			stat = WuPrtText_font(hDC, aAnnot[iAnnot].szText,
								aAnnot[iAnnot].xPx, yPx, aAnnot[iAnnot].eAnchor,
								pXAxis->szAnnotFont, pXAxis->fAnnotPts,
								0, fAnnotDeg, bNoVSpace,
								1, NULL, &aAnnot[iAnnot].quad);
			if (stat != 0) goto gdi_error;
		}
	}
	// Now go through all the annotations, checking for overlaps.  If an
	// annotation doesn't overlap its neighbors, it will have bPrintOK
	// set to 1, allowing it to be printed.
	if (bAnnot)
		PlArea_annotCheck(aAnnot, nMajor, 1, fXDist, fYDist);

	// Finally, draw the annotations and tick marks.
	for (iAnnot=0; iAnnot<=nMajor; iAnnot++) {
		int		bEnable;
		bEnable = 1;	// enable tick mark
		xPx = aAnnot[iAnnot].xPx;
		// If a decade boundary is outside the data area, don't
		// draw a tick mark or print an annotation.
		if (xPx < pXAxis->pxB - 1 || xPx > pXAxis->pxT + 1)
			aAnnot[iAnnot].bEnable = bEnable = 0;
		if (bEnable) {
			// For non-endpoint spots (and endpoints on "half"
			// axes), draw major "tick" marks.
			if ((iAnnot != 0 && iAnnot != nMajor) ||
								pXAxis->eStyle == PL_EDGE_HALF_AXIS ||
								pXAxis->eStyle == PL_CENT_HALF_AXIS) {
				retStat = PlArea_tick(hDC, pXAxis->eStyle,
									xPx, yMajB_0, xPx, yMajB_1,
									xPx, yMajT_0, xPx, yMajT_1, rgb);
				if (retStat != PL_OK) goto done;
			}
		}
		if (aAnnot[iAnnot].bEnable && bAnnot && aAnnot[iAnnot].bPrintOK) {
			stat = WuPrtText_font(hDC, aAnnot[iAnnot].szText,
							xPx, yPx, aAnnot[iAnnot].eAnchor,
							pXAxis->szAnnotFont, pXAxis->fAnnotPts,
							0, fAnnotDeg, bNoVSpace,
							0, NULL, NULL);
			if (stat != 0) goto gdi_error;
		}
		// Get poised to do minor subdivisions.
		if (iAnnot == 0)
			dLVal = dLValLFloor;
		else
			dLVal = aAnnot[iAnnot].dLVal;
		if (iAnnot < nMajor && nMinor > 1) {
			double	dDMajor=pow(10., dLVal);
			// Draw the minor tick marks.  If "nMinor" is 2,
			// then the ".5" subdivision in each decade will
			// have a minor tick mark; values greater than 2
			// result in minor tick marks at the ".2" - ".9"
			// subdivisions in each decade.
			for (j=2; j<=nMinor; j++) {
				double	dDMinor;

				if (nMinor == 2) dDMinor = dDMajor * 5.;
				else dDMinor = dDMajor * j;
				PL_WIN_CVT_X_VAL_TO_X_PIX(dDMinor, dTemp);
				xPx = (int)(.5 + dTemp);
				if (xPx > pXAxis->pxB && xPx < pXAxis->pxT) {
					if (yMinB_0 >= 0) {
						MoveToEx(hDC, xPx, yMinB_0, NULL);
						if (LineTo(hDC, xPx, yMinB_1-1) == 0)
							goto gdi_error;
					}
					if (yMinT_0 >= 0) {
						MoveToEx(hDC, xPx, yMinT_0, NULL);
						if (LineTo(hDC, xPx, yMinT_1+1) == 0)
							goto gdi_error;
					}
				}
			}
		}
	}

done:
	if (aAnnot != NULL) free(aAnnot);
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlArea_xAxis - annotate the X axis and draw grid lines/tick marks
*
* Purpose:
*		Draws the annotations for an X axis and applies the grid lines
*		or tick marks.
*
*		This routine:
*		o	always (even when zoomed) honors nMajor and nMinor
*		o	when zoomed 10X or more, uses "auto format" rather than the
*		    caller's annotation format
*		o	gives special treatment to nMajor and nMinor if the axis has
*			PL_LOG scaling:
*			-	if nMajor is > 1, then major divisions will be drawn at
*				decade boundaries
*			-	if nMinor is 2, then a minor division at .5 will be drawn
*			-	if nMinor > 2, then minor divisions will be drawn at the
*				".1" through ".9" subdivisions of each decade
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlArea_xAxis(
PL_CTX	*pPlot,			// I pointer to plot context structure
HDC		hDC,			// I handle to device context
COLORREF rgb,
float	fXHalfFrac)
{
	int		retStat=PL_OK;
	PL_AXIS	*pXAxis=pPlot->pXAxis, *pYAxis=pPlot->pYAxis;
	int		nMajor=pXAxis->nMajor, nMinor=pXAxis->nMinor;
	int		bNoVSpace;
	float	fAnnotDeg=pXAxis->fAnnotDeg;
	int		yPx, yMajLen, yMinLen;
	int		yMajB_0, yMajB_1, yMajT_0, yMajT_1;	// ends of major ticks
	int		yMinB_0, yMinB_1, yMinT_0, yMinT_1;	// ends of minor ticks

	if (nMajor < 1) nMajor = 1;
	if (nMinor < 1) nMinor = 1;

	// Set up "tick" length.
	yMinLen = (int)ceil(.5F * PL_YPTS_TO_YPIX(2.F));
	yMajLen = yMinLen * 3;
	// And calculate the actual vertical endpoints of the "tick" marks.
	if (pXAxis->eStyle == PL_NO_AXIS || pXAxis->eStyle == PL_LINE_AXIS)
		yMajB_0 = yMajT_0 = yMinB_0 = yMinT_0 = -1;
	else if (pXAxis->eStyle == PL_CENT_HALF_AXIS) {
		yMajB_0 = yMinB_0 = yMajB_1 = yMinB_1 = (int)(.5F + pYAxis->pxB+1 -
							fXHalfFrac * (pYAxis->pxB+1 - (pYAxis->pxT-1)));
		yMajB_0 += yMajLen / 2;
		yMajB_1 -= yMajLen / 2;
		yMinB_0 += yMinLen / 2;
		yMinB_1 -= yMinLen / 2;
		yMajT_0 = yMinT_0 = -1;
	}
	else {
		if (pXAxis->eStyle == PL_GRID_AXIS ||
									pXAxis->eStyle == PL_GRID_DOT_AXIS ||
									pXAxis->eStyle == PL_GRID_DASH_AXIS) {
			yMajB_0 = pYAxis->pxB;
			yMajB_1 = pYAxis->pxT;
			yMajT_0 = -1;
		}
		else {
			yMajB_0 = pYAxis->pxB;
			yMajB_1 = yMajB_0 - yMajLen + 1;
			yMajT_0 = pYAxis->pxT;
			yMajT_1 = yMajT_0 + yMajLen - 1;
		}
		yMinB_0 = pYAxis->pxB;
		yMinB_1 = yMinB_0 - yMinLen + 1;
		yMinT_0 = pYAxis->pxT;
		yMinT_1 = yMinT_0 + yMinLen - 1;
		if (pXAxis->eStyle == PL_EDGE_HALF_AXIS)
			yMajT_0 = yMinT_0 = -1;
	}

	// Set vertical position for uppermost side of annotations.
	yPx = (int)(.5F + pYAxis->pxB+1 - fXHalfFrac *
										(pYAxis->pxB+1 - (pYAxis->pxT-1)));
	yPx += (int)ceil(.5F * PL_YPTS_TO_YPIX(2.F));
	if (pXAxis->eStyle == PL_CENT_HALF_AXIS)
		yPx += yMajLen / 2;

	if (fAnnotDeg == 0.F)	bNoVSpace = 0;
	else					bNoVSpace = 1;

	if (pXAxis->eScaleType == PL_LIN) {
		retStat = PlArea_xAxisLin(pPlot, hDC, rgb, bNoVSpace, fXHalfFrac, yPx,
								yMajB_0, yMajB_1, yMajT_0, yMajT_1,
								yMinB_0, yMinB_1, yMinT_0, yMinT_1);
		if (retStat != PL_OK) goto done;
	}
	else {
		retStat = PlArea_xAxisLog(pPlot, hDC, rgb, bNoVSpace, fXHalfFrac, yPx,
								yMajB_0, yMajB_1, yMajT_0, yMajT_1,
								yMinB_0, yMinB_1, yMinT_0, yMinT_1);
		if (retStat != PL_OK) goto done;
	}

done:
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlArea_yAxisLin - do the actual work for a PL_LIN axis
*
* Purpose:
*		Draws the annotations for a PL_LIN Y axis and applies the grid lines
*		or tick marks.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
* Notes:
* 1.	If xMinL_0 is -1, then minor tick marks won't be drawn on the
*		left Y axis.  And similarly for xMinR_0.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-17-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlArea_yAxisLin(
PL_CTX	*pPlot,			// I pointer to plot context structure
HDC		hDC,			// I handle to device context
COLORREF rgb,			// I color for drawing text
int		bNoVSpace,		// I 1 to suppress internal leading and/or descent
float	fYHalfFrac,		// I where (along X axis) the axis has been drawn
int		xPx,			// I horiz position for rightmost side of annot.
int		xMajL_0,		// I left end of major tick on left axis
int		xMajL_1,		// I right end of major tick on left axis
int		xMajR_0,		// I right end of major tick on right axis
int		xMajR_1,		// I left end of major tick on right axis
int		xMinL_0,		// I left end of minor tick on left axis, or -1
int		xMinL_1,		// I right end of minor tick on left axis
int		xMinR_0,		// I right end of minor tick on right axis, or -1
int		xMinR_1)		// I left end of minor tick on right axis
{
	int		retStat=PL_OK;
	PL_AXIS	*pXAxis=pPlot->pXAxis, *pYAxis=pPlot->pYAxis;
	int		nMajor=pYAxis->nMajor, nMinor=pYAxis->nMinor;
	float	fAnnotDeg=pYAxis->fAnnotDeg;
	float	fXDist, fYDist;
	int		iAnnot;
	int		stat, j;
	int		yPx, yRange;
	int		bAnnot=0;
	ANNOT	*aAnnot=NULL;

	if (pYAxis->eStyle == PL_NO_AXIS) goto done;

	if (pYAxis->fAnnotPts > 0.F) {
		bAnnot = 1;
		fXDist = (float)PL_XPTS_TO_XPIX(pYAxis->fAnnotPts * .25);
		fYDist = (float)PL_YPTS_TO_YPIX(pYAxis->fAnnotPts * .25);
	}

	yRange = pYAxis->pxB+1 - (pYAxis->pxT-1);
	if (nMajor < 1)
		nMajor = 1;

	aAnnot = (ANNOT *)malloc(sizeof(ANNOT)*(nMajor+1));
	if (aAnnot == NULL) {
		retStat = PL_MALLOC_FAIL;
		goto done;
	}
	PlArea_axisCommon(pPlot, 0, aAnnot, nMajor);	// do Y axis

	// Go through all the annotations, getting their quads.
	for (iAnnot=0; iAnnot<=nMajor; iAnnot++) {
		if (aAnnot[iAnnot].bEnable && bAnnot) {
			stat = WuPrtText_font(hDC, aAnnot[iAnnot].szText,
								xPx, aAnnot[iAnnot].yPx, aAnnot[iAnnot].eAnchor,
								pYAxis->szAnnotFont, pYAxis->fAnnotPts,
								0, fAnnotDeg, bNoVSpace,
								1, NULL, &aAnnot[iAnnot].quad);
			if (stat != 0) goto gdi_error;
		}
	}
	// Now go through all the annotations, checking for overlaps.  If an
	// annotation doesn't overlap its neighbors, it will have bPrintOK
	// set to 1, allowing it to be printed.
	if (bAnnot)
		PlArea_annotCheck(aAnnot, nMajor, 0, fXDist, fYDist);

	// Finally, draw the annotations and tick marks.
	for (iAnnot=0; iAnnot<=nMajor; iAnnot++) {
		yPx = aAnnot[iAnnot].yPx;
		if ((iAnnot != 0 && iAnnot != nMajor) ||
									pYAxis->eStyle == PL_EDGE_HALF_AXIS ||
									pYAxis->eStyle == PL_CENT_HALF_AXIS) {
			// For non-endpoint spots, draw major "tick" marks.
			retStat = PlArea_tick(hDC, pYAxis->eStyle,
									xMajL_0, yPx, xMajL_1, yPx,
									xMajR_0, yPx, xMajR_1, yPx, rgb);
			if (retStat != PL_OK) goto done;
		}
		if (aAnnot[iAnnot].bPrintOK) {
			stat = WuPrtText_font(hDC, aAnnot[iAnnot].szText,
								xPx, yPx, aAnnot[iAnnot].eAnchor,
								pYAxis->szAnnotFont, pYAxis->fAnnotPts,
								0, fAnnotDeg, bNoVSpace,
								0, NULL, NULL);
			if (stat != 0) goto gdi_error;
		}
		if (iAnnot < nMajor && nMinor > 1) {
			// Draw the minor tick marks.
			for (j=1; j<nMinor; j++) {
				int		yMinPx;
				yMinPx = (int)(.5 + yPx - (double)j /
										(double)nMinor * yRange / nMajor);
				if (xMinL_0 >= 0) {
					MoveToEx(hDC, xMinL_0, yMinPx, NULL);
					if (LineTo(hDC, xMinL_1+1, yMinPx) == 0)
						goto gdi_error;
				}
				if (xMinR_0 >= 0) {
					MoveToEx(hDC, xMinR_0, yMinPx, NULL);
					// QUIRK: Under Win3.1, the last pixel
					// gets drawn when going backward.
					if (LineTo(hDC, xMinR_1, yMinPx) == 0)
						goto gdi_error;
				}
			}
		}
	}
done:
	if (aAnnot != NULL) free(aAnnot);
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlArea_yAxisLog - do the actual work for a PL_LOG axis
*
* Purpose:
*		Draws the annotations for a PL_LOG Y axis and applies the grid lines
*		or tick marks.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
* Notes:
* 1.	If xMinL_0 is -1, then minor tick marks won't be drawn on the
*		left Y axis.  And similarly for xMinR_0.
* 2.	If pYAxis->szAnnotFmt is PL_FMT_TENTO, then annotations appear as
*		10 with a superscripted power.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-17-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlArea_yAxisLog(
PL_CTX	*pPlot,			// I pointer to plot context structure
HDC		hDC,			// I handle to device context
COLORREF rgb,			// I color for drawing text
int		bNoVSpace,		// I 1 to suppress internal leading and/or descent
float	fYHalfFrac,		// I where (along X axis) the axis has been drawn
int		xPx,			// I horiz position for rightmost side of annot.
int		xMajL_0,		// I left end of major tick on left axis
int		xMajL_1,		// I right end of major tick on left axis
int		xMajR_0,		// I right end of major tick on right axis
int		xMajR_1,		// I left end of major tick on right axis
int		xMinL_0,		// I left end of minor tick on left axis, or -1
int		xMinL_1,		// I right end of minor tick on left axis
int		xMinR_0,		// I right end of minor tick on right axis, or -1
int		xMinR_1)		// I left end of minor tick on right axis
{
	int		retStat=PL_OK;
	PL_AXIS	*pXAxis=pPlot->pXAxis, *pYAxis=pPlot->pYAxis;
	int		nMajor=pYAxis->nMajor, nMinor=pYAxis->nMinor;
	float	fAnnotDeg=pYAxis->fAnnotDeg;
	float	fXDist, fYDist;
	int		iAnnot;
	int		stat, j;
	int		yPx, yRange;
	int		bAnnot=0;
	ANNOT	*aAnnot=NULL;
	double	dLVal, dLValB, dLValT, dTemp;
	int		nDecades;	// # full decades to cover visible axis
	double	dLValBFloor, dLValTCeil;

	if (pYAxis->eStyle == PL_NO_AXIS) goto done;

	if (pYAxis->fAnnotPts > 0.F) {
		bAnnot = 1;
		fXDist = (float)PL_XPTS_TO_XPIX(pYAxis->fAnnotPts * .25);
		fYDist = (float)PL_YPTS_TO_YPIX(pYAxis->fAnnotPts * .25);
	}

	yRange = pYAxis->pxB+1 - (pYAxis->pxT-1);
	dLValB = pYAxis->dLValBS;
	dLValT = pYAxis->dLValTS;

	// Figure out how many decades are required to span the visible
	// axis.  For the special log axes that accomodate values of 0,
	// the first decade goes from 0 to nn (rather than the usual 1
	// to nn).  It is easily possible for the axis to be zoomed and
	// scrolled so that the visible part of the axis has a range of
	// .4 to .9--i.e., not only does the visible part of the axis not
	// include a decade boundary, but it doesn't even include the
	// possibility of a data point!
	if (pYAxis->bLogZero && pYAxis->dLValBS <= pYAxis->dLogZero)
		dLValBFloor = ceil(pYAxis->dLogZero);
	else
		dLValBFloor = floor(dLValB);
	if (pYAxis->bLogZero && pYAxis->dLValTS <= pYAxis->dLogZero)
		dLValTCeil = 0.;
	else
		dLValTCeil = ceil(dLValT);
	nDecades = (int)(dLValTCeil - dLValBFloor);
	if (nMajor > 1)
		nMajor = nDecades;
	if (nMajor < 1)
		nMajor = 1;

	// Since this is a log axis, we'll have 1, 2, or 9 minor subdivisions.
	if (nMinor < 1)
		nMinor = 1;
	else if (nMinor > 2)
		nMinor = 9;

	aAnnot = (ANNOT *)malloc(sizeof(ANNOT)*(nMajor+1));
	if (aAnnot == NULL) {
		retStat = PL_MALLOC_FAIL;
		goto done;
	}
	PlArea_axisCommon(pPlot, 0, aAnnot, nMajor);	// do Y axis

	// Go through all the annotations, getting their quads.
	for (iAnnot=0; iAnnot<=nMajor; iAnnot++) {
		if (aAnnot[iAnnot].bEnable && bAnnot) {
			stat = WuPrtText_font(hDC, aAnnot[iAnnot].szText,
								xPx, aAnnot[iAnnot].yPx, aAnnot[iAnnot].eAnchor,
								pYAxis->szAnnotFont, pYAxis->fAnnotPts,
								0, fAnnotDeg, bNoVSpace,
								1, NULL, &aAnnot[iAnnot].quad);
			if (stat != 0) goto gdi_error;
		}
	}
	// Now go through all the annotations, checking for overlaps.  If an
	// annotation doesn't overlap its neighbors, it will have bPrintOK
	// set to 1, allowing it to be printed.
	if (bAnnot)
		PlArea_annotCheck(aAnnot, nMajor, 1, fXDist, fYDist);

	// Finally, draw the annotations and tick marks.
	// Finally, draw the annotations and tick marks.
	for (iAnnot=0; iAnnot<=nMajor; iAnnot++) {
		int		bEnable;
		bEnable = 1;	// enable tick mark
		yPx = aAnnot[iAnnot].yPx;
		// If a decade boundary is outside the data area, don't
		// draw a tick mark or print an annotation.
		if (yPx > pYAxis->pxB + 1 || yPx < pYAxis->pxT - 1)
			bEnable = 0;
		if (bEnable) {
			// For non-endpoint spots (and endpoints on "half"
			// axes), draw major "tick" marks.
			if ((iAnnot != 0 && iAnnot != nMajor) ||
									pYAxis->eStyle == PL_EDGE_HALF_AXIS ||
									pYAxis->eStyle == PL_CENT_HALF_AXIS) {
				retStat = PlArea_tick(hDC, pYAxis->eStyle,
									xMajL_0, yPx, xMajL_1, yPx,
									xMajR_0, yPx, xMajR_1, yPx, rgb);
				if (retStat != PL_OK) goto done;
			}
		}
		if (aAnnot[iAnnot].bEnable && bAnnot && aAnnot[iAnnot].bPrintOK) {
			stat = WuPrtText_font(hDC, aAnnot[iAnnot].szText,
								xPx, yPx, aAnnot[iAnnot].eAnchor,
								pYAxis->szAnnotFont, pYAxis->fAnnotPts,
								0, fAnnotDeg, bNoVSpace,
								0, NULL, NULL);
			if (stat != 0) goto gdi_error;
		}
		// Get poised to do minor subdivisions.
		if (iAnnot == 0)
			dLVal = dLValBFloor;
		else
			dLVal = aAnnot[iAnnot].dLVal;
		if (iAnnot < nMajor && nMinor > 1) {
			double	dDMajor=pow(10., dLVal);
			// Draw the minor tick marks.  If "nMinor" is 2,
			// then the ".5" subdivision in each decade will
			// have a minor tick mark; values greater than 2
			// result in minor tick marks at the ".2" - ".9"
			// subdivisions in each decade.
			for (j=2; j<=nMinor; j++) {
				double	dDMinor;

				if (nMinor == 2) dDMinor = dDMajor * 5.;
				else dDMinor = dDMajor * j;
				PL_WIN_CVT_Y_VAL_TO_Y_PIX(dDMinor, dTemp);
				yPx = (int)(.5 + dTemp);
				if (yPx < pYAxis->pxB && yPx > pYAxis->pxT) {
					if (xMinL_0 >= 0) {
						MoveToEx(hDC, xMinL_0, yPx, NULL);
						if (LineTo(hDC, xMinL_1+1, yPx) == 0)
							goto gdi_error;
					}
					if (xMinR_0 >= 0) {
						MoveToEx(hDC, xMinR_0, yPx, NULL);
						// QUIRK: Under Win3.1, the last pixel
						// gets drawn when going backward.
						if (LineTo(hDC, xMinR_1, yPx) == 0)
							goto gdi_error;
					}
				}
			}
		}
	}

done:
	if (aAnnot != NULL) free(aAnnot);
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlArea_yAxis - annotate the Y axis
*
* Purpose:
*		Draws the annotations for a Y axis.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlArea_yAxis(
PL_CTX	*pPlot,			// I pointer to plot context structure
HDC		hDC,			// I handle to device context
COLORREF rgb,
float	fYHalfFrac)
{
	int		retStat=PL_OK;
	PL_AXIS	*pXAxis=pPlot->pXAxis, *pYAxis=pPlot->pYAxis;
	int		nMajor=pYAxis->nMajor, nMinor=pYAxis->nMinor;
	float	fAnnotDeg=pYAxis->fAnnotDeg;
	int		bNoVSpace;
	int		xPx, xMajLen, xMinLen;
	int		xMajL_0, xMajL_1, xMajR_0, xMajR_1;	// ends of major ticks
	int		xMinL_0, xMinL_1, xMinR_0, xMinR_1;	// ends of minor ticks

	if (nMajor < 1) nMajor = 1;
	if (nMinor < 1) nMinor = 1;

	// Set up "tick" length.
	xMinLen = (int)ceil(.5F * PL_XPTS_TO_XPIX(2.F));
	xMajLen = xMinLen * 3;
	// And calculate the actual vertical endpoints of the "tick" marks.
	if (pYAxis->eStyle == PL_NO_AXIS || pYAxis->eStyle == PL_LINE_AXIS)
		xMajL_0 = xMajR_0 = xMinL_0 = xMinR_0 = -1;
	else if (pYAxis->eStyle == PL_CENT_HALF_AXIS) {
		xMajL_0 = xMinL_0 = xMajL_1 = xMinL_1 =
				(int)(.5F + pXAxis->pxB-1 + fYHalfFrac *
					(pXAxis->pxT+1 - (pXAxis->pxB-1)));
		xMajL_0 -= xMajLen / 2;
		xMajL_1 += xMajLen / 2;
		xMinL_0 -= xMinLen / 2;
		xMinL_1 += xMinLen / 2;
		xMajR_0 = xMinR_0 = -1;
	}
	else {
		if (pYAxis->eStyle == PL_GRID_AXIS ||
					pYAxis->eStyle == PL_GRID_DOT_AXIS ||
					pYAxis->eStyle == PL_GRID_DASH_AXIS) {
			xMajL_0 = pXAxis->pxB;
			xMajL_1 = pXAxis->pxT;
			xMajR_0 = -1;
		}
		else {
			xMajL_0 = pXAxis->pxB;
			xMajL_1 = xMajL_0 + xMajLen - 1;
			xMajR_0 = pXAxis->pxT;
			xMajR_1 = xMajR_0 - xMajLen + 1;
		}
		xMinL_0 = pXAxis->pxB;
		xMinL_1 = xMinL_0 + xMinLen - 1;
		xMinR_0 = pXAxis->pxT;
		xMinR_1 = xMinR_0 - xMinLen + 1;
		if (pYAxis->eStyle == PL_EDGE_HALF_AXIS)
			xMajR_0 = xMinR_0 = -1;
	}

	// Set horizontal position for rightmost side of annotations.
	xPx = (int)(.5F + pXAxis->pxB-1 + fYHalfFrac *
					(pXAxis->pxT+1 - (pXAxis->pxB-1)));
	xPx -= (int)ceil(.5F * PL_XPTS_TO_XPIX(2.F));
	if (pYAxis->eStyle == PL_CENT_HALF_AXIS)
		xPx -= xMajLen / 2;

	if (fAnnotDeg == 0.F)	bNoVSpace = 1;
	else					bNoVSpace = 0;

	if (pYAxis->eScaleType == PL_LIN) {
		retStat = PlArea_yAxisLin(pPlot, hDC, rgb, bNoVSpace,
			fYHalfFrac, xPx,
			xMajL_0, xMajL_1, xMajR_0, xMajR_1,
			xMinL_0, xMinL_1, xMinR_0, xMinR_1);
		if (retStat != PL_OK) goto done;
	}
	else {
		retStat = PlArea_yAxisLog(pPlot, hDC, rgb, bNoVSpace,
			fYHalfFrac, xPx,
			xMajL_0, xMajL_1, xMajR_0, xMajR_1,
			xMinL_0, xMinL_1, xMinR_0, xMinR_1);
		if (retStat != PL_OK) goto done;
	}

done:
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaBackground - perform the actual grid/axis drawing
*
* Purpose:
*		Draws the grid or axes for the selected plot area and draws the
*		desired axis annotations.
*
*		The style for the axis, the number of divisions, etc. are set
*		using PlAreaXStyle and PlAreaYStyle.
*
*		Annotations are drawn at the endpoints of each axis.  If an axis
*		has been divided into major divisions, than an annotation is also
*		drawn at each major division.  (If an annotation at a major
*		division would overlap another annotation, then drawing is skipped
*		for the annotation.)
*
*		If the .bIntAxis member of an axis is 1 and the axis is linear,
*		then annotations that are close (within .001) to being an integer
*		are forced to be exactly integer.  This creates annotations that
*		will seem more natural to an operator.
*
*		If an annotation callback function has been registered for an
*		axis, then the function is called with the annotation value.
*		The text string returned by the function is drawn on the axis.
*		(The test for overlap is performed as described above.)  If the
*		function returns the special text string PL_FMT_DFLT, then the
*		annotation format specified in the PlArea{X|Y}Style call is
*		used to convert the annotation value to a text string.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* See Also:
*		PlArea{X|Y}Style, PlAreaArraySelect
*		PlAreaTitleText, PlAreaReg{X|Y}AnnotCallback
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaBackground(
PL_CTX	*pPlot)			// I pointer to plot context structure
{
	int		retStat=PL_OK, inpErr=0;
	PL_AXIS	*pXAxis, *pYAxis;
	HDC		hDC;
	HPEN	hPen, hPenOld;
	COLORREF myRgb;
	int		bXBox=0, bXHalf=0, bYBox=0, bYHalf=0;
	float	fXHalfFrac=0.F, fYHalfFrac=0.F;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit)

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto sub_exit; }

	pXAxis = pPlot->pXAxis;
	pYAxis = pPlot->pYAxis;

	retStat = PlWinHDCSetup(pPlot, 1, 0, 0,			// full plot window
							PL_LINE, 0.F, 2,		// use GetTextColor
							0, &hDC, &hPen, &hPenOld, &myRgb);
	if (retStat != PL_OK) goto done;

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	// Decide where to draw the axes.
	if (pXAxis->eStyle == PL_EDGE_HALF_AXIS)
		bXHalf = 1;
	else if (pXAxis->eStyle == PL_CENT_HALF_AXIS) {
		bXHalf = 1;
		fXHalfFrac = .5F;
	}
	else if (pXAxis->eStyle != PL_NO_AXIS)
		bXBox = 1;
	if (pYAxis->eStyle == PL_EDGE_HALF_AXIS)
		bYHalf = 1;
	else if (pYAxis->eStyle == PL_CENT_HALF_AXIS) {
		bYHalf = 1;
		fYHalfFrac = .5F;
	}
	else if (pYAxis->eStyle != PL_NO_AXIS)
		bYBox = 1;

	// Draw the axes.
	if (bXBox) {
		MoveToEx(hDC, pXAxis->pxT+1, pYAxis->pxB+1, NULL);
		if (LineTo(hDC, pXAxis->pxB-1, pYAxis->pxB+1) == 0)
			goto gdi_error;
		MoveToEx(hDC, pXAxis->pxT+1, pYAxis->pxT-1, NULL);
		if (LineTo(hDC, pXAxis->pxB-1, pYAxis->pxT-1) == 0)
			goto gdi_error;
	}
	if (bYBox) {
		MoveToEx(hDC, pXAxis->pxB-1, pYAxis->pxB+1, NULL);
		if (LineTo(hDC, pXAxis->pxB-1, pYAxis->pxT-1) == 0)
			goto gdi_error;
		MoveToEx(hDC, pXAxis->pxT+1, pYAxis->pxB+1, NULL);
		if (LineTo(hDC, pXAxis->pxT+1, pYAxis->pxT-1) == 0)
			goto gdi_error;
	}
	if (bXHalf) {
		int		pxY;
		pxY = (int)(.5F + pYAxis->pxB+1 - fXHalfFrac *
			(pYAxis->pxB+1 - (pYAxis->pxT-1)));
		MoveToEx(hDC, pXAxis->pxB-1, pxY, NULL);
		// offset by additional 1 to draw end pixel
		if (LineTo(hDC, pXAxis->pxT+1+1, pxY) == 0)
			goto gdi_error;
	}
	if (bYHalf) {
		int		pxX;
		pxX = (int)(.5F + pXAxis->pxB-1 + fYHalfFrac *
			(pXAxis->pxT+1 - (pXAxis->pxB-1)));
		MoveToEx(hDC, pxX, pYAxis->pxB+1, NULL);
		// offset by additional 1 to draw end pixel
		if (LineTo(hDC, pxX, pYAxis->pxT-1-1) == 0)
			goto gdi_error;
	}

	// Annotate the axes and draw tick marks and/or grid lines.
	retStat = PlArea_xAxis(pPlot, hDC, myRgb, fXHalfFrac);
	if (retStat != PL_OK) goto done;
	retStat = PlArea_yAxis(pPlot, hDC, myRgb, fYHalfFrac);
	if (retStat != PL_OK) goto done;

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
sub_exit:
	PL_IF_INP_ERR("PlAreaBackground")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaClear - clear a plot area
*
* Purpose:
*		Clear a plot area, filling it with the window's background color.
*		The data area, annotations, and title lines are erased.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		another code if an error occurs
*
* Notes:
* 1.	Typical applications will not need to call PlAreaClear--when the
*		plot area needs to be repainted, the damaged areas will already
*		have been cleared automatically.
*
* See Also:
*		PlPlotClear
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-08-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaClear(
PL_CTX	*pPlot)			// I pointer to plot context structure
{
	int		retStat=PL_OK, inpErr=0;
	HDC		hDC;
	HBRUSH	hBrush, hBrushOld;
	HPEN	hPen=0, hPenOld=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if plotting is to the screen, pPlot->hwPlot must be a valid window
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit)

	if (pPlot->bPrinter) goto sub_exit;

	retStat = PlWinHDCSetup(pPlot, 0, 0, 0,
							PL_LINE, 0.F, 1,	// use GetBkColor
							0, &hDC, &hPen, &hPenOld, NULL);
	if (retStat != PL_OK) goto done;
	if ((hBrush = CreateSolidBrush(GetBkColor(hDC))) == 0) goto gdi_error;
	/* KG */
	//if ((hBrushOld = SelectObject(hDC, hBrush)) == 0) goto gdi_error;
	if ((hBrushOld = (HBRUSH)SelectObject(hDC, hBrush)) == 0) goto gdi_error;

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	if (Rectangle(hDC, pPlot->pXAxis->pxAreaB, pPlot->pYAxis->pxAreaT,
		pPlot->pXAxis->pxAreaT + 1, pPlot->pYAxis->pxAreaB + 2) == 0) {
		goto gdi_error;
	}

done:
	if (hBrushOld != 0) SelectObject(hDC, hBrushOld);
	if (hBrush != 0) DeleteObject(hBrush);
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
sub_exit:
	PL_IF_INP_ERR("PlAreaClear")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaFPtsAuto - calculate a suitable drawing size
*
* Purpose:
*		Calculates a suitable drawing size to be used in drawing points,
*		taking into account the X zoom factor.  The drawing size is chosen
*		to make the points as large as possible but small enough to avoid
*		overlap.  This routine is especially valuable when plotting on
*		a high-resolution monitor or on printers with small pixel size.
*
*		This routine works well with data that has a uniform X spacing
*		between data points.  It isn't intended for use with a log X
*		axis, since the X spacing is variable.
*
*		In some cases, this routine chooses the size of the pixel on the
*		display device:
*		o	if the X axis scaling type is PL_LOG
*		o	if the spacing between data points is less than or equal to 3
*			pixels
*
*		This routine allows an application program to automatically adapt
*		the size of the points to the number of data points and to the
*		X zoom factor.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		another code if an error occurs
*
* Notes:
* 1.	nDataPoints is the total number of data points for the X axis.
*		This routine calculates the number of points that are visible
*		based on nDataPoints and the X zoom factor.
*
* See Also:
*		PlDrawPoint
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-21-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaFPtsAuto(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		nDataPoints,	// I number of data points along X axis
float	fMaxPts,		// I maximum size for fPts
float	*pfPts)			// O size, in points, to use in drawing
{
	int		retStat=PL_OK, inpErr=0;
	PL_AXIS	*pXAxis;
	float	fPts=.1F, fPixPerDatum;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pfPts must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pXAxis = pPlot->pXAxis;
	PL_INP_CHK(2, pfPts == NULL, done)

	// For log axes, use a small point size.
	if (pXAxis->eScaleType == PL_LOG) goto done;

	if (pXAxis->dLValTS > pXAxis->dLValBS) {
		fPixPerDatum = (float)((pXAxis->pxT - pXAxis->pxB) /
				(nDataPoints / pXAxis->fZoom));
		if (fPixPerDatum > 3.F) {
			fPts = .5F * pPlot->fTwipsPerPixX * fPixPerDatum / 20.F;
			if (fPts > fMaxPts) fPts = fMaxPts;
		}
	}

done:
	*pfPts = fPts;
	PL_IF_INP_ERR("PlAreaFPtsAuto")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaMove - set the position and size of a plot area
*
* Purpose:
*		Sets the size and position for the selected plot area.  Sizes
*		and positions are specified as fractions of the plot window width
*		and height, as follows:
*
*		fXFrac  is the distance from the left edge of the plot window
*				to the left side of the plot area, as a fraction of the
*				width of the plot window
*		fYFrac  is the distance from the top edge of the plot window
*				to the top side of the plot area, as a fraction of the
*				height of the plot window
*		fWidFrac  is the width of the plot area, as a fraction of the
*				width of the plot window
*		fHtFrac  is the height of the plot area, as a fraction of the
*				height of the plot window
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		another code if an error occurs
*
* Notes:
* 1.	PlAreaArrayInit initializes a plot area array so that the data
*		area is the same size for all plot areas.  To override this default
*		behavior, this routine must be called AFTER any calls to
*		PlAreaArrayInit.
* 2.	This routine automatically adjusts the positions and sizes to
*		accomodate the spacing between rows and columns that is specified
*		in the PlAreaArrayInit call.
*
* BUGS:
* 1.	This routine functions properly only if all plot areas in a
*		particular column have the same fXFrac and all have the same
*		fWidFrac.  Similarly, all plot areas in a particular row must
*		have the same fYFrac and all must have the same fHtFrac.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-21-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaMove(
PL_CTX	*pPlot,			// IO pointer to plot context structure
float	fXFrac,			// I pos of left side from window left, or -1
float	fYFrac,			// I pos of top side from window top, or -1
float	fWidFrac,		// I width, or -1
float	fHtFrac)		// I height, or -1
{
	int		retStat=PL_OK, inpErr=0;
	int		nNeg=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		position and size arguments:
*		all must be >= 0, or
*		all must be < 0 
* 3		position and size arguments, if >= 0, must be <= 1
* 4		size arguments must not be 0
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	if (fXFrac < 0.F) nNeg++;
	if (fYFrac < 0.F) nNeg++;
	if (fWidFrac < 0.F) nNeg++;
	if (fHtFrac < 0.F) nNeg++;
	PL_INP_CHK(2, nNeg != 4 && nNeg != 0, done)
	PL_INP_CHK(3, fXFrac > 1.F || fYFrac > 1.F ||
				fWidFrac > 1.F || fHtFrac > 1.F, done)
	PL_INP_CHK(4, fWidFrac == 0.F || fHtFrac == 0.F, done)

	pPlot->pXAxis->fArrayPosFrac = fXFrac;
	pPlot->pXAxis->fArraySizeFrac = fWidFrac;
	pPlot->pYAxis->fArrayPosFrac = fYFrac;
	pPlot->pYAxis->fArraySizeFrac = fHtFrac;
	pPlot->bNeedScale = 1;

done:
	PL_IF_INP_ERR("PlAreaMove")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaRangeAutoEnds - adjust the endpoints for axes
*
* Purpose:
*		Adjust the endpoints for either or both axes to a new range that
*		will produce more tasteful axis calibration.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		another code if an error occurs
*
* Notes:
* 1.	The axis endpoints should be set (prior to calling this routine),
*		to the actual data range.  The PlAreaRangeSet... routines can be
*		used to set the data range.
* 2.	This routine treats as a special case axes with a range of 0 to
*		(2**n) - 1.  In this case, the endpoints are set to 0 to 2**n.
*
* See Also:
*		PlAreaRangeSetXYVectors, PlAreaRangeSetYVector, and PlAreaRangeSet
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-27-95 R. Cole		created
* 08-02-95 R. Cole		renamed
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaRangeAutoEnds(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bXAxis,			// I 1 to adjust X axis endpoints, else 0
int		bYAxis)			// I 1 to adjust Y axis endpoints, else 0
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)

	if (bXAxis)
		PlAreaRangeAutoEndsAxis(pPlot->pXAxis);
	if (bYAxis)
		PlAreaRangeAutoEndsAxis(pPlot->pYAxis);

	pPlot->bNeedScale = 1;
	PlWinScale(pPlot);

done:
	PL_IF_INP_ERR("PlAreaRangeAutoEnds")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlAreaRangeAutoEndsAxis - choose `clean' endpoint values for an axis
*
* Purpose:
*		Choose axis endpoint values that will produce tasteful axis
*		calibration.
*
*		Using the current numeric range for the axis, this routine
*		calculates a possibly altered numeric range that will produce
*		tasteful axis calibration.  The axis endpoint values are set
*		to the new range.
*
*		If the current anchor value for the axis lies outside the new
*		range, then the axis anchor value is set to the new value at
*		the current anchor fraction on the axis.
*
* Return Value:
*		void
*
* BUGS:
* 1.	This routine should probably focus some attention on choice of
*		number of intervals for an axis; presently, the new endpoints
*		chosen by this routine may be difficult to use for choosing
*		interval size.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-90 R. Cole		initial version
* 11-20-94 R. Cole		cloned into dem_dll
* 03-27-95 R. Cole		cloned into PL.LIB
* 08-02-95 R. Cole		renamed
*--------------------------------------------------------------------------*/
void PASCAL
PlAreaRangeAutoEndsAxis(
PL_AXIS	*pAxis)			// IO plot axis structure
{
	double	newDValB, newDValT;
	double	dDValMin, dDValMax;

	if (pAxis->eScaleType == PL_LIN) {
		PlAutoLinearEnds(pAxis->dDValB, pAxis->dDValT,
				&newDValB, &newDValT);
	}
	else {
		PlAutoLogEnds(pAxis->dDValB, pAxis->dDValT,
				&newDValB, &newDValT);
	}

	dDValMin = PL_MIN(newDValB, newDValT);
	dDValMax = PL_MAX(newDValB, newDValT);

	pAxis->dDValB = newDValB;
	pAxis->dDValT = newDValT;
	pAxis->bLogZero = 0;
	if (pAxis->eScaleType == PL_LOG) {
		if (newDValB <= 0.) {
			pAxis->dLValB = pAxis->dLogZero;
			pAxis->bLogZero = 1;
		}
		else
			pAxis->dLValB = log10(newDValB);
		pAxis->dLValT = log10(newDValT);
		if (pAxis->dDValAnchor < dDValMin ||
					pAxis->dDValAnchor > dDValMax) {
			pAxis->dDValAnchor = pAxis->dLValB + pAxis->fAnchorFrac *
						(pAxis->dLValT - pAxis->dLValB);
			pAxis->dDValAnchor = pow(10., pAxis->dDValAnchor);
		}
	}
	else {
		pAxis->dLValB = newDValB;
		pAxis->dLValT = newDValT;
		if (pAxis->dDValAnchor < dDValMin || pAxis->dDValAnchor > dDValMax) {
			pAxis->dDValAnchor = newDValB +
								pAxis->fAnchorFrac * (newDValT - newDValB);
		}
	}
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaRangeGet - get data ranges for the plot area
*
* Purpose:
*		Gets the X and Y data ranges for the plot area.
*
*		If "bVisible" is 1, then the range returned corresponds to the
*		visible part of the axis.  If "bVisible" is 0, then the entire
*		axis range is reported.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* See Also:
*		PlAreaRangeSet
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-20-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaRangeGet(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		bVisible,		// I 1 if only visible part of axis is wanted
double	*pdXLeft,		// O ptr for left X data value, or NULL
double	*pdXRight,		// O ptr for right X data value, or NULL
double	*pdYBottom,		// O ptr for bottom Y data value, or NULL
double	*pdYTop)		// O ptr for top Y data value, or NULL
{
	int		retStat=PL_OK, inpErr=0;
	double	dDVal, dLVal;
	PL_AXIS	*pXAxis, *pYAxis;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)

	pXAxis = pPlot->pXAxis;
	pYAxis = pPlot->pYAxis;

	if (pdXLeft != NULL) {
		if (bVisible)	dLVal = pXAxis->dLValBS;
		else		dLVal = pXAxis->dLValB;
		if (pXAxis->eScaleType == PL_LIN)
			dDVal = dLVal;
		else {
			if (pXAxis->bLogZero && dLVal <= pXAxis->dLogZero)
				dDVal = 0.;
			else
				dDVal = pow(10., dLVal);
		}
		*pdXLeft = dDVal;
	}
	if (pdXRight != NULL) {
		if (bVisible)	dLVal = pXAxis->dLValTS;
		else		dLVal = pXAxis->dLValT;
		if (pXAxis->eScaleType == PL_LIN)
			dDVal = dLVal;
		else {
			if (pXAxis->bLogZero && dLVal <= pXAxis->dLogZero)
				dDVal = 0.;
			else
				dDVal = pow(10., dLVal);
		}
		*pdXRight = dDVal;
	}
	if (pdYBottom != NULL) {
		if (bVisible)	dLVal = pYAxis->dLValBS;
		else		dLVal = pYAxis->dLValB;
		if (pYAxis->eScaleType == PL_LIN)
			dDVal = dLVal;
		else {
			if (pYAxis->bLogZero && dLVal <= pYAxis->dLogZero)
				dDVal = 0.;
			else
				dDVal = pow(10., dLVal);
		}
		*pdYBottom = dDVal;
	}
	if (pdYTop != NULL) {
		if (bVisible)	dLVal = pYAxis->dLValTS;
		else		dLVal = pYAxis->dLValT;
		if (pYAxis->eScaleType == PL_LIN)
			dDVal = dLVal;
		else {
			if (pYAxis->bLogZero && dLVal <= pYAxis->dLogZero)
				dDVal = 0.;
			else
				dDVal = pow(10., dLVal);
		}
		*pdYTop = dDVal;
	}

done:
	PL_IF_INP_ERR("PlAreaRangeGet")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaRangeSet - set data ranges for the plot area
*
* Purpose:
*		Sets up the X and Y data ranges for the plot area, based on
*		caller specified limits and on the axis scale types (linear or log).
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If dXLeft equals dXRight, then the X axis range is set for dXLeft
*		to dXLeft+1.  Similar treatment is used for the Y axis range.
* 2.	Axis scale types must have been set prior to calling this routine.
*		(Calling PlArea...Style is the typical way to set axis scale type.)
* 3.	If the axis is log and the minimum value is zero, then the axis is
*		set up for special "log 0" processing.  pAxis->dDValB_minNon0 must
*		have previously been set up, which is done by the PlAreaRangeSet...
*		vector routines.
* 4.	This routine doesn't alter the .bIntAxis members of the axes.  These
*		members can be manually set to indicate whether the axis data belongs
*		to the set of integers or not.  (These members are automatically set
*		by PlAreaRangeSetXYVectors and PlAreaRangeSetYVector to correspond to
*		the data type.)  When the .bIntAxis member is 1, PlAreaBackground
*		does special processing for the axis annotations.
*
* See Also:
*		PlAreaRangeGet
*		PlWinCreate, PlPlotInit_win, PlPlot
*		PlAreaRangeAutoEnds
*		PlAreaRangeSetXYVectors, PlAreaRangeSetYVector
*
* BUGS:
* 1.	For log10 axes, the axis must be "ascending".
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-21-95 R. Cole		created
* 08-02-95 R. Cole		renamed
* 08-22-95 R. Cole		removed call to PlWinScale
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaRangeSet(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bXAxis,			// I 1 if X axis range is to be set
double	dXLeft,			// I data value at left end of X axis
double	dXRight,		// I data value at right end of X axis
int		bYAxis,			// I 1 if Y axis range is to be set
double	dYBottom,		// I data value at bottom end of Y axis
double	dYTop)			// I data value at top end of Y axis
{
	int		retStat=PL_OK, inpErr=0;
	double	dDValMin, dDValMax, dDValMin_nonZero;
	PL_AXIS	*pXAxis, *pYAxis;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
*		if bXAxis is 1 and if the X axis has LOG scaling,
* 3			neither dXLeft nor dXRight can be negative
* 4			dXLeft must be <= dXRight
* 5			if dXLeft is 0., then dXRight must be > 0.
*		if bYAxis is 1 and if the Y axis has LOG scaling,
* 6			neither dYBottom nor dYTop can be negative
* 7			dYBottom must be <= dYTop
* 8			if dYBottom is 0., then dYTop must be > 0.
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), done)
	pXAxis = pPlot->pXAxis;
	pYAxis = pPlot->pYAxis;
	if (bXAxis && pXAxis->eScaleType == PL_LOG) {
		PL_INP_CHK(3, dXLeft < 0. || dXRight < 0., done)
		PL_INP_CHK(4, dXLeft > dXRight, done)
		PL_INP_CHK(5, dXLeft == 0. && dXRight <= 0., done)
	}
	if (bYAxis && pYAxis->eScaleType == PL_LOG) {
		PL_INP_CHK(6, dYBottom < 0. || dYTop < 0., done)
		PL_INP_CHK(7, dYBottom > dYTop, done)
		PL_INP_CHK(8, dYBottom == 0. && dYTop <= 0., done)
	}

	pPlot->bNeedScale = 1;

	if (dXLeft == dXRight) dXRight = dXLeft + 1.;
	if (dYBottom == dYTop) dYTop = dYBottom + 1.;

	if (bXAxis) {
		pXAxis->bLogZero = 0;
		pXAxis->dDValB = dXLeft;
		pXAxis->dDValT = dXRight;
		if (pXAxis->eScaleType == PL_LIN) {
			pXAxis->dLValB = dXLeft;
			pXAxis->dLValT = dXRight;
		}
		else {
			if (dXLeft <= 0.) {
				dDValMin_nonZero = pXAxis->dDValB_minNon0;
				if (dDValMin_nonZero == 0.)
					dDValMin_nonZero = 1.;
				pXAxis->dLogZero = log10(.5 * dDValMin_nonZero);
				pXAxis->bLogZero = 1;
				pXAxis->dLValB = pXAxis->dLogZero;
			}
			else
				pXAxis->dLValB = log10(dXLeft);
			pXAxis->dLValT = log10(dXRight);
		}
		dDValMin = PL_MIN(dXLeft, dXRight);
		dDValMax = PL_MAX(dXLeft, dXRight);
/*---------------------------------------------------------------------------
*    If the current anchor value is outside the new range, set the anchor
*    value to the value at the current anchor fraction of the axis.
*--------------------------------------------------------------------------*/
		if (pXAxis->dDValAnchor < dDValMin ||
					pXAxis->dDValAnchor > dDValMax) {
			pXAxis->dDValAnchor = pXAxis->dLValB +
					pXAxis->fAnchorFrac *
					(pXAxis->dLValT - pXAxis->dLValB);
			if (pXAxis->eScaleType == PL_LOG)
				pXAxis->dDValAnchor = pow(10., pXAxis->dDValAnchor);
		}
	}

	if (bYAxis) {
		pYAxis->bLogZero = 0;
		pYAxis->dDValB = dYBottom;
		pYAxis->dDValT = dYTop;
		if (pYAxis->eScaleType == PL_LIN) {
			pYAxis->dLValB = dYBottom;
			pYAxis->dLValT = dYTop;
		}
		else {
			if (dYBottom <= 0.) {
				dDValMin_nonZero = pYAxis->dDValB_minNon0;
				if (dDValMin_nonZero == 0.)
					dDValMin_nonZero = 1.;
				pYAxis->dLogZero = log10(.5 * dDValMin_nonZero);
				pYAxis->bLogZero = 1;
				pYAxis->dLValB = pYAxis->dLogZero;
			}
			else
				pYAxis->dLValB = log10(dYBottom);
			pYAxis->dLValT = log10(dYTop);
		}
		dDValMin = PL_MIN(dYBottom, dYTop);
		dDValMax = PL_MAX(dYBottom, dYTop);
/*---------------------------------------------------------------------------
*    If the current anchor value is outside the new range, set the anchor
*    value to the value at the current anchor fraction of the axis.
*--------------------------------------------------------------------------*/
		if (pYAxis->dDValAnchor < dDValMin || pYAxis->dDValAnchor > dDValMax) {
			pYAxis->dDValAnchor = pYAxis->dLValB + pYAxis->fAnchorFrac *
							(pYAxis->dLValT - pYAxis->dLValB);
			if (pYAxis->eScaleType == PL_LOG)
				pYAxis->dDValAnchor = pow(10., pYAxis->dDValAnchor);
		}
	}
	PlWinScale(pPlot);

done:
	PL_IF_INP_ERR("PlAreaRangeSet")
	return retStat;
}
/*+/subr/TOC-----------------------------------------------------------------
* PlAreaRangeSetXYVectors - set range for vectors of X and Y data points
*
* Purpose:
*		Sets the X and Y axis ranges based on the minimum and maximum values
*		in vectors of X and Y data points.
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
* 1.	In addition to finding the min and max values, this routine also
*		finds (for both the X vector and the Y vector) the smallest value
*		that is greater than zero.  This value is used for log axes to
*		determine how many decades are needed between 0 and 1.
* 2.	Both axes have the .bIntAxis members set to 0 for float and double,
*		and to 1 for other types.  For programs that use float or double
*		arrays to store integer data, the program must manually set the
*		member(s) appropriately after calling this routine.  When the
*		.bIntAxis member is 1, PlAreaBackground does special processing for
*		the axis annotations.
* 3.	This routine can be called by C++ routines for a variety of vector
*		types.  C and VB routines must call this routine with variant
*		names that correspond to the supported vector types.  The supported
*		vector types and the variant names are:
*			short	PlAreaRangeSetXYVectors_sht
*			long	PlAreaRangeSetXYVectors_lng
*			float	PlAreaRangeSetXYVectors_flt
*			double	PlAreaRangeSetXYVectors_dbl
*			ULONG	PlAreaRangeSetXYVectors_ul
*
* See Also:
*		PlAreaRangeAutoEnds and PlAreaRangeSet
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-21-95 R. Cole		created
* 08-02-95 R. Cole		renamed
*--------------------------------------------------------------------------*/
/*+--------------------------------------------------------------------------
* SYNOPSIS:
* int
* PlAreaRangeSetXYVectors(
* PL_CTX  *pPlot,          // IO pointer to plot context structure
* long    n,               // I number of points
* const <type>  *p...XVec, // I X data vector
* const <type>  *p...YVec) // I Y data vector
*
*--------------------------------------------------------------------------*/
/*+/subr/TOC-----------------------------------------------------------------
* PlAreaRangeSetYVector - set range for a vector of Y data points
*
* Purpose:
*		Sets the X and Y axis ranges based on the starting and ending X
*		values and on the minimum and maximum values in a vector of Y
*		data points.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	In addition to finding the min and max values, this routine also
*		finds the smallest value that is greater than zero.  This value
*		is used for log axes to determine how many decades are needed
*		between 0 and 1.
* 2.	The Y axis has the .bIntAxis member set to 0 for float and double,
*		and to 1 for other types.  For programs that use float or double
*		arrays to store integer data, the program must manually set the
*		member appropriately after calling this routine.  The X axis has
*		the .bIntAxis member set to 1.  When the .bIntAxis member is 1,
*		PlAreaBackground does special processing for the axis annotations.
* 3.	This routine can be called by C++ routines for a variety of vector
*		types.  C and VB routines must call this routine with variant
*		names that correspond to the supported vector types.  The supported
*		vector types and the variant names are:
*			short	PlAreaRangeSetYVector_sht
*			long	PlAreaRangeSetYVector_lng
*			float	PlAreaRangeSetYVector_flt
*			double	PlAreaRangeSetYVector_dbl
*			ULONG	PlAreaRangeSetYVector_ul
*
* See Also:
*		PlAreaRangeAutoEnds and PlAreaRangeSet
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-21-95 R. Cole		created
* 08-02-95 R. Cole		renamed
* 08-16-95 R. Cole		added XLeft and XIncr args
*--------------------------------------------------------------------------*/
/*+--------------------------------------------------------------------------
* SYNOPSIS:
* int
* PlAreaRangeSetYVector(
* PL_CTX  *pPlot,          // IO pointer to plot context structure
* long    n,               // I number of points
* double  dXLeft,          // I X data value at left end of X axis
* double  dXIncr,          // I X data value spacing between points
* const <type>  *p...YVec) // I Y data vector
*
*--------------------------------------------------------------------------*/
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaRegXAnnotCallback - register "plot annotation" function
*
* Purpose:
*		Registers the function that will be called when PlAreaBackground
*		draws the annotations for the X axis.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	The szAnnotFmt in the PlAreaXStyle is usually not used by PL.LIB
*		when an annotation callback function is being used.  However,
*		if the annotation callback function returns the the value
*		PL_FMT_DFLT then PL.LIB uses szAnnotFmt.
* 2.	If the annotation would overlap an adjacent annotation, then
*		the plotting of the annotation is skipped.
*
* See Also:
*		PlAreaXStyle, PlAreaRegYAnnotCallback
*		PlAreaBackground
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-06-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaRegXAnnotCallback(
PL_CTX	*pPlot,			// I pointer to plot context structure
const char * (*pAnnotFn)(PL_CTX *, void *, double),// user callback function
void	*pAnnotArg)		// arg for user call
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)

	pPlot->pXAxis->pAnnotFn = pAnnotFn;
	pPlot->pXAxis->pAnnotArg = pAnnotArg;

done:
	PL_IF_INP_ERR("PlAreaXAnnotCallback")
	return retStat;
}
/*+--------------------------------------------------------------------------
*
* Example:
*
*		This example demonstrates how to use the annotation callback
*		feature.  In this example, the data values for the axis are
*		LONG seconds past the Windows epoch.  The desired annotation
*		is the corresponding "short date" as specified by the Windows
*		international setting.
*
*		There are two distinct pieces of code:
*
*		First is the function you define that creates the text string
*		that corresponds to a particular value.  This function is called
*		by PL.LIB whenever it is necessary to draw an annotation on the
*		axis.
*
*		This function returns a pointer to the text string for the
*		annotation.  The text string can't be on the stack.  PL.LIB
*		uses the text string only once, as soon as the callback function
*		returns, so the same string buffer can be used for all
*		annotations.
*
*		const char * MyAnnotFn(
*		PL_CTX *pPlot,	// IO pointer to plot context structure
*		void *arg,		// I pointer to user structure
*		double dVal)	// I the data value at the annotation point
*		{
*			int		stat;
*			static char szAnnot[20];
*
*			TimeLSecToSz("short", (long)dVal, 20, szAnnot);
*			return szAnnot;
*		}
*
*		The second piece of code consists of the statements that register
*		the callback function.  (The last argument in this example is
*		NULL, but it can be used to pass a pointer to the annotation
*		callback function.)
*
*		stat = PlAreaRegXAnnotCallback(pPlot, &MyAnnotFn, NULL);
*		if (stat != PL_OK)
*			goto pl_error;
*
*--------------------------------------------------------------------------*/

/*+/csubr/TOC----------------------------------------------------------------
* PlAreaRegYAnnotCallback - register "plot annotation" function
*
* Purpose:
*		Registers the function that will be called when PlAreaBackground
*		draws the annotations for the Y axis.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	The szAnnotFmt in the PlAreaYStyle is usually not used by PL.LIB
*		when an annotation callback function is being used.  However,
*		if the annotation callback function returns the the value
*		PL_FMT_DFLT then PL.LIB uses szAnnotFmt.
* 2.	If the annotation would overlap an adjacent annotation, then
*		the plotting of the annotation is skipped.
* 3.	For an example of setting up an annotation callback function, see
*		PlAreaRegXAnnotCallback.
*
* See Also:
*		PlAreaYStyle, PlAreaRegXAnnotCallback
*		PlAreaBackground
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-06-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaRegYAnnotCallback(
PL_CTX	*pPlot,			// I pointer to plot context structure
const char * (*pAnnotFn)(PL_CTX *, void *, double),// user callback function
void	*pAnnotArg)		// arg for user call
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)

	pPlot->pYAxis->pAnnotFn = pAnnotFn;
	pPlot->pYAxis->pAnnotArg = pAnnotArg;

done:
	PL_IF_INP_ERR("PlAreaYAnnotCallback")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaTitleText - draw text in a plot area title line
*
* Purpose:
*		Draw text in a title line for the currently selected plot area,
*		positioning the text relative to an "anchor point".
*
*		The 'eSide' argument selects the set of title lines, corresponding
*		to the desired side of the plot area:
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
* Notes:
* 1.	If 'bAxisRelative' is 1, then the title line is treated as though
*		it is the same length as the axis.  Otherwise, the title line
*		length is the same as the corresponding side of the plot area.
*
* See Also:
*		PlAreaXStyle, PlAreaYStyle
*		PlAreaBackground
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaTitleText(
PL_CTX	*pPlot,			// I pointer to plot context structure
int		eSide,			// I selector for set of lines--PL_SIDE_xxx
int		iLine,			// I line number, starting with 0
float	fFrac,			// I anchor point, as fraction of title line length
const char *szText,		// I text to print
int		eJust,			// I positioning code: PRT_{LJ, CEN, RJ}
int		bAxisRelative,	// I 1 position text relative to axis
COLORREF rgb)			// I color to use for printing
{
	int		retStat=PL_OK, inpErr=0;
	HDC		hDC;
	HPEN	hPen, hPenOld;
	COLORREF myRgb, rgbOld;
	int		xPx, yPx;
	int		pxL, pxR, pxT, pxB;
	int		stat;
	int		eRefPt, nLines;
	float	fDeg;
	float	fTitlePts;
	char	*szTitleFont;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pPlot->hDC is 0 then pPlot->hwPlot must be a valid window
* 3		iLine must be >= 0
* 4		eSide must be one of the PL_SIDE_xxx values
* 5		the line selection must be consistent with the PlArea...Style call
* 6		eJust must be a valid code
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, sub_exit)
	PL_INP_CHK(2, pPlot->hDC == 0 && !IsWindow(pPlot->hwPlot), sub_exit)
	PL_INP_CHK(3, iLine < 0, sub_exit)
	if (eSide == PL_SIDE_LEFT) {
		nLines = pPlot->pYAxis->nTitleLinesB;
		fTitlePts = pPlot->pYAxis->fTitlePts;
		szTitleFont = pPlot->pYAxis->szTitleFont;
	}
	else if (eSide == PL_SIDE_RIGHT) {
		nLines = pPlot->pYAxis->nTitleLinesT;
		fTitlePts = pPlot->pYAxis->fTitlePts;
		szTitleFont = pPlot->pYAxis->szTitleFont;
	}
	else if (eSide == PL_SIDE_TOP) {
		nLines = pPlot->pXAxis->nTitleLinesT;
		fTitlePts = pPlot->pXAxis->fTitlePts;
		szTitleFont = pPlot->pXAxis->szTitleFont;
	}
	else if (eSide == PL_SIDE_BOTTOM) {
		nLines = pPlot->pXAxis->nTitleLinesB;
		fTitlePts = pPlot->pXAxis->fTitlePts;
		szTitleFont = pPlot->pXAxis->szTitleFont;
	}
	else
		PL_INP_CHK(4, 1, sub_exit)
	PL_INP_CHK(5, iLine >= nLines, sub_exit)

	if (eJust == PRT_LJ)		eRefPt = PRT_TL;
	else if (eJust == PRT_CEN)	eRefPt = PRT_TC;
	else if (eJust == PRT_RJ)	eRefPt = PRT_TR;
	else				PL_INP_CHK(6, 1, sub_exit)

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto sub_exit; }

	retStat = PlWinHDCSetup(pPlot, 0, 1, 0,	// clip to "gross" area
							PL_LINE, 0.F, 0, rgb,
							&hDC, &hPen, &hPenOld, &myRgb);
	if (retStat != PL_OK) goto done;
	rgbOld = SetTextColor(hDC, myRgb);

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	pxL = pPlot->pXAxis->pxAreaB;
	pxR = pPlot->pXAxis->pxAreaT;
	pxT = pPlot->pYAxis->pxAreaT;
	pxB = pPlot->pYAxis->pxAreaB;
	if (bAxisRelative) {
		if (eSide == PL_SIDE_TOP || eSide == PL_SIDE_BOTTOM) {
			pxL = pPlot->pXAxis->pxB;
			pxR = pPlot->pXAxis->pxT;
		}
		else {
			pxT = pPlot->pYAxis->pxT;
			pxB = pPlot->pYAxis->pxB;
		}
	}
	PlTitleLineToPx(pPlot, nLines, pxL, pxR, pxT, pxB,
			fTitlePts, eSide, iLine, fFrac,
			&xPx, &yPx, &fDeg);
	stat = WuPrtText_font(hDC, szText, xPx, yPx, eRefPt,
			szTitleFont, fTitlePts, 0, fDeg, 0, 0, NULL, NULL);
	if (stat != 0) goto gdi_error;

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (hDC != 0) SetTextColor(hDC, rgbOld);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
sub_exit:
	PL_IF_INP_ERR("PlAreaTitleText")
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlArea_style - set axis style, generic
*
* Purpose:
*		Set the X or Y axis style.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		another code if an error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 11-21-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlArea_style(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bXAxis,			// I 1,0 to set X,Y axis style
int		eStyle,			// I style for axis
int		eScaleType,		// I axis scaling type--PL_LIN or PL_LOG
int		nMajor,			// I number of major divisions for axis, or 0 or -1
int		nMinor,			// I number of minor axis divisions within major
float	fAnnotDeg,		// I axis annotations, ccw degrees from horizontal
int		iAnnotNChar,	// I max length of annotation
float	fAnnotPts,		// I font size in points of axis annotations, or 0
const char *szAnnotFont,// I name of font for axis annotations, or "" or NULL
const char *szAnnotFmt,	// I format for printing axis annotations
int		nTitleLinesBelow,// I # of title lines below (or left of) plot area
int		nTitleLinesAbove,// I # of title lines above (or right of) plot area
float	fTitlePts,		// I font size in points of axis title, or 0
const char *szTitleFont)// I name of font for axis title, or "" or NULL
{
	int		retStat=PL_OK, inpErr=0;
	PL_AXIS	*pAxis;

	pPlot->inpErr = 0;

	if (bXAxis)	pAxis = pPlot->pXAxis;
	else		pAxis = pPlot->pYAxis;

	PL_INP_CHK(3, eScaleType != PL_LIN && eScaleType != PL_LOG, done)
	PL_INP_CHK(4, eStyle < PL_NO_AXIS || eStyle > PL_CENT_HALF_AXIS, done)
	PL_INP_CHK(5, nTitleLinesBelow < 0, done)
	PL_INP_CHK(6, nTitleLinesAbove < 0, done)
	if (fAnnotPts > 0.F) {
		if (fAnnotDeg != 0.F && fAnnotDeg != 90.F)
			PL_INP_CHK(7, 1, done)
		if (fAnnotDeg != 0.F && bXAxis)
			PL_INP_CHK(8, iAnnotNChar <= 0, done)
		if (fAnnotDeg != 90.F && !bXAxis)
			PL_INP_CHK(8, iAnnotNChar <= 0, done)
		if (szAnnotFont == NULL) szAnnotFont = "";
		PL_INP_CHK(9, strlen(szAnnotFont) >= PL_MSG_DIM, done)
		PL_INP_CHK(10, szAnnotFmt == NULL, done)
		PL_INP_CHK(11, strlen(szAnnotFmt) >= PL_MSG_DIM, done)
	}
	if (nTitleLinesBelow + nTitleLinesAbove > 0)
		PL_INP_CHK(12, fTitlePts <= 0.F, done)
	if (fTitlePts > 0.F) {
		if (szTitleFont == NULL) szTitleFont = "";
		PL_INP_CHK(13, strlen(szTitleFont) >= PL_MSG_DIM, done)
	}

	pPlot->bNeedScale = 1;

	pAxis->eStyle = eStyle;
	// If scale type changes, call PlAreaRangeSet, just in case the caller
	// doesn't do it.  This is important because the dLValXxx items
	// in the PL_AXIS structure differ for the two scale types.
	if (pAxis->eScaleType != eScaleType) {
		double dDValB=pAxis->dDValB, dDValT=pAxis->dDValT;
		if (eScaleType == PL_LOG) {
			if (dDValB < 0. || dDValT < 0.) {
				dDValB = 1.;
				dDValT = 10.;
			}
		}
		pAxis->eScaleType = eScaleType;
		retStat = PlAreaRangeSet(pPlot,
				bXAxis, dDValB, dDValT, bXAxis^1, dDValB, dDValT);
	}
	if (retStat != PL_OK) goto done;
	pAxis->nMajor = nMajor;
	pAxis->nMinor = nMinor;
	pAxis->nTitleLinesB = nTitleLinesBelow;
	pAxis->nTitleLinesT = nTitleLinesAbove;
	if (fTitlePts > 0.F)	strcpy(pAxis->szTitleFont, szTitleFont);
	else			pAxis->szTitleFont[0] = '\0';
	pAxis->fTitlePts = fTitlePts;
	if (fAnnotPts > 0.F) {
		strcpy(pAxis->szAnnotFont, szAnnotFont);
		strcpy(pAxis->szAnnotFmt, szAnnotFmt);
	}
	else {
		pAxis->szAnnotFont[0] = '\0';
		pAxis->szAnnotFmt[0] = '\0';
	}
	pAxis->fAnnotPts = fAnnotPts;
	pAxis->fAnnotDeg = fAnnotDeg;
	pAxis->iAnnotNChar = iAnnotNChar;
	PlWinScale(pPlot);
done:
	PL_IF_INP_ERR("")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaXStyle - sets the style-related information for the X axis
*
* Purpose:
*		Configures the X axis, including:
*		o	reserving title lines at the top and bottom of the plot area,
*		    to be used for text or graphics
*		o	specifying the font to be used for text in title lines
*		o	specifying the axis style and scaling type
*		o	specifying how the axis should be divided into major and
*		    minor divisions
*		o	specifying whether value annotations are to appear for
*		    major axis divisions
*		o	specifying the orientation, font, and format to use for
*		    value annotations
*
*		This routine controls the allocation of space within the plot
*		area, leaving room for title lines at the top and bottom of the
*		plot area and annotations below the X axis.
*
*		Top and bottom title lines will always be the full width of the
*		plot area, with the text oriented horizontally.
*
*		The axes and annotations for a plot area (both X and Y axis) are
*		drawn by calling PlAreaBackground.  PlAreaTitleText is used to
*		draw the title lines for the axes.
*
*		The "eStyle" argument selects the type of axis.  It can be one of:
*			PL_NO_AXIS		no X axis is drawn
*			PL_LINE_AXIS	borders without tick marks are drawn at the
*							top and bottom of the plot area
*			PL_TICK_AXIS	borders with tick marks are drawn at the top
*							and bottom of the plot area
*			PL_GRID_AXIS	borders are drawn at the top and bottom of
*							the plot area, with solid grid lines for
*							the major divisions and tick marks for the
*							minor divisions
*			PL_GRID_DOT_AXIS is similar to PL_GRID_AXIS, except the
*							grid lines are dotted rather than solid
*			PL_GRID_DASH_AXIS is similar to PL_GRID_AXIS, except the
*							grid lines are dashed rather than solid
*			PL_EDGE_HALF	the X axis consists of a single line
*							drawn at the bottom of the plot area, with
*							tick marks
*			PL_CENT_HALF	is similar to PL_EDGE_HALF, except the
*							axis line is drawn at the vertical
*							midpoint of the plot area
*
*		The "nMajor" and "nMinor" arguments control how the X axis is
*		broken into divisions.
*
*		The endpoints of "major" axis divisions are where annotations
*		will appear.  For endpoints that lie inside the boundaries of
*		the axis, grid lines or major tick marks are drawn if "nMajor"
*		is greater than 1.  (If "nMajor" is 3, then the axis will be
*		divided into 3 parts, with 2 grid lines or major tick marks at
*		the 1/3 and 2/3 points on the axis.)
*
*		If "nMinor" is greater than 1, then each major axis division
*		will be broken into the specified number of divisions, with
*		minor tick marks indicating the endpoints of the minor divisions.
*
*		"nMajor" and "nMinor" are given special treatment for a log
*		axis.  If "nMajor" is greater than 1, then the axis is divided
*		into decades, which are treated as the major divisions.  The
*		number of major divisions thus depends on the data rather than
*		on the value of "nMajor".  If "nMinor" is 2, then the ".5"
*		subdivision in each decade will have a minor tick mark; values
*		greater than 2 result in minor tick marks at the ".2" - ".9"
*		subdivisions in each decade.
*
*		"szAnnotFmt" controls converting the values at major tick marks
*		to text.  Most frequently, this will be an ordinary "C" format
*		string compatible for use with values of type 'double'--even if
*		integer, long, or float data values are being used.  Two special
*		format can be specified:
*		o	PL_FMT_AUTO causes PL.LIB to create a format that adapts to
*			the annotation values.  If none of the annotation values has
*			a fractional part, then the annotations will be printed as
*			integers; if one or more have a fractional part, then the
*			required number of decimal places are printed; if one or
*			more is too large or too small to represent with a "%f"
*			format, then a "%e" format will be used.  When PL_FMT_AUTO
*			is used, then 'iAnnotNChar' must be specified.  'iAnnotNChar'
*			controls not only the "indent" if the annotations aren't
*			parallel to the axis, but also affect how PL.LIB creates
*			the format--so that the length of the annotation string
*			won't exceed 'iAnnotNChar' characters.
*		o	PL_FMT_AUTO".2" functions similar to PL_FMT_AUTO, except
*			no more than 2 decimal places will be shown.  (The number
*			2 is used here as an example; any number can be specified.)
*		o	PL_FMT_TENTO can be used with PL_LOG axes.  This format causes
*		    annotations to appear as 10 with a superscripted power.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If an annotation callback function is being used, "szAnnotFmt" is
*		ignored--unless the callback returns a value of PL_FMT_DFLT, which
*		causes "szAnnotFmt" to be used.
* 2.	The value of 'iAnnotNChar' is used by PL.LIB when:
*		o	PL_FMT_AUTO is used
*		o	the annotation isn't parallel to the axis.  In this case, the
*			axis is "indented" far enough to create a "margin" wide
*			enough to display iAnnotNChar characters.
* 3.	If the size in points for title or annotations is specified as 0.,
*		then the title or annotations, respectively, won't appear on the
*		plot.
*
* See Also:
*		PlAreaBackground, PlAreaTitleText
*		PlAreaRegXAnnotCallback, PlPlotStyle, PlAreaYStyle, PlAreaArrayInit
*
*-Date     Author		Revision
* -------- ------------	--------
* 06-05-95 R. Cole		created
* 08-16-95 R. Cole		split into PlArea{X|Y}Style
* 08-16-95 R. Cole		added major/minor axis divisions; added annotation
*						rotation and width; changed to allow multi-line
*						plot area title above and/or below the plot area;
*--------------------------------------------------------------------------*/
int PASCAL
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
const char *szTitleFont)// I name of font for axis title, or "" or NULL
{
	int		retStat=PL_OK, inpErr=0;
	PL_AXIS	*pAxis;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pPlot->pXAxis must not be NULL
* 3		eScaleType must be legal
* 4		eStyle must be legal
* 5		nTitleLinesBelow must be >= 0
* 6		nTitleLinesAbove must be >= 0
*		if fAnnotPts is greater than zero, then
* 7			fAnnotDeg must be either 0 or 90
*			if fAnnotDeg is not 0, then
* 8				iAnnotNChar must be greater than 0
* 9		the length of szAnnotFont must be less than PL_MSG_DIM
* 10		szAnnotFmt must not be null
* 11		the length of szAnnotFmt must be less than PL_MSG_DIM
*		if nTitleLinesBelow and/or nTitleLinesAbove is > 0, then
* 12		fTitlePts must be greater than zero
*		if fTitlePts is greater than zero, then
* 13		the length of szTitleFont must be less than PL_MSG_DIM
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pAxis = pPlot->pXAxis;
	PL_INP_CHK(2, pAxis == NULL, done)

	retStat = PlArea_style(pPlot, 1, eStyle, eScaleType, nMajor, nMinor,
		fAnnotDeg, iAnnotNChar, fAnnotPts, szAnnotFont, szAnnotFmt,
		nTitleLinesBelow, nTitleLinesAbove, fTitlePts, szTitleFont);
	inpErr = pPlot->inpErr;

done:
	PL_IF_INP_ERR("PlAreaXStyle")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaYStyle - sets the style-related information for the Y axis
*
* Purpose:
*		Configures the appearance for the Y axis, including:
*		o	reserving title lines at the left and right of the plot area,
*			to be used for text or graphics
*		o	specifying the font to be used for text in title lines
*		o	specifying the axis style and scaling type
*		o	specifying how the axis should be divided into major and
*			minor divisions
*		o	specifying whether value annotations are to appear at
*			major axis markings
*		o	specifying the orientation, font, and format to use for
*			annotations
*
*		This routine controls the allocation of space within the plot
*		area, leaving room for title lines at the left and right of
*		the plot area and annotations to the left of the Y axis.
*
*		Left and right title lines will be shorter than the height of the
*		plot area, depending on the vertical space occupied by the top and
*		bottom title lines.  For left and right title lines, text is
*		oriented vertically with the left end of the line toward the bottom
*		of the plot area.
*
*		The axes and annotations for a plot area (both X and Y axis) are
*		drawn by calling PlAreaBackground.  PlAreaTitleText is used to
*		draw the title lines for the axes.
*
*		The "eStyle" argument selects the type of axis.  It can be one of:
*			PL_NO_AXIS		no Y axis is drawn
*			PL_LINE_AXIS	borders without tick marks are drawn at the
*							left and right of the plot area
*			PL_TICK_AXIS	borders with tick marks are drawn at the left
*							and right of the plot area
*			PL_GRID_AXIS	borders are drawn at the left and right of
*							the plot area, with solid grid lines for
*							the major divisions and tick marks for the
*							minor divisions
*			PL_GRID_DOT_AXIS is similar to PL_GRID_AXIS, except the
*							grid lines are dotted rather than solid
*			PL_GRID_DASH_AXIS is similar to PL_GRID_AXIS, except the
*							grid lines are dashed rather than solid
*			PL_EDGE_HALF	the Y axis consists of a single line
*							drawn at the left of the plot area, with
*							tick marks
*			PL_CENT_HALF	is similar to PL_EDGE_HALF, except the
*							axis line is drawn at the horizontal
*							midpoint of the plot area
*
*		The "nMajor" and "nMinor" arguments control how the Y axis is
*		broken into divisions.
*
*		The endpoints of "major" axis divisions are where annotations
*		will appear.  For endpoints that lie inside the boundaries of
*		the axis, grid lines or major tick marks are drawn if "nMajor"
*		is greater than 1.  (If "nMajor" is 3, then the axis will be
*		divided into 3 parts, with 2 grid lines or major tick marks at
*		the 1/3 and 2/3 points on the axis.)
*
*		If "nMinor" is greater than 1, then each major axis division
*		will be broken into the specified number of divisions, with
*		minor tick marks indicating the endpoints of the minor divisions.
*
*		"nMajor" and "nMinor" are given special treatment for a log
*		axis.  If "nMajor" is greater than 1, then the axis is divided
*		into decades, which are treated as the major divisions.  The
*		number of major divisions thus depends on the data rather than
*		on the value of "nMajor".  If "nMinor" is 2, then the ".5"
*		subdivision in each decade will have a minor tick mark; values
*		greater than 2 result in minor tick marks at the ".2" - ".9"
*		subdivisions in each decade.
*
*		"szAnnotFmt" controls converting the values at major tick marks
*		to text.  Most frequently, this will be an ordinary "C" format
*		string compatible for use with values of type 'double'--even if
*		integer, long, or float data values are being used.  Two special
*		format can be specified:
*		o	PL_FMT_AUTO causes PL.LIB to create a format that adapts to
*			the annotation values.  If none of the annotation values has
*			a fractional part, then the annotations will be printed as
*			integers; if one or more have a fractional part, then the
*			required number of decimal places are printed; if one or
*			more is too large or too small to represent with a "%f"
*			format, then a "%e" format will be used.  When PL_FMT_AUTO
*			is used, then 'iAnnotNChar' must be specified.  'iAnnotNChar'
*			controls not only the "indent" if the annotations aren't
*			parallel to the axis, but also affect how PL.LIB creates
*			the format--so that the length of the annotation string
*			won't exceed 'iAnnotNChar' characters.
*		o	PL_FMT_AUTO".2" functions similar to PL_FMT_AUTO, except
*			no more than 2 decimal places will be shown.  (The number
*			2 is used here as an example; any number can be specified.)
*		o	PL_FMT_TENTO can be used with PL_LOG axes.  This format causes
*			annotations to appear as 10 with a superscripted power.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If an annotation callback function is being used, "szAnnotFmt" is
*		ignored--unless the callback returns a value of PL_FMT_DFLT, which
*		causes "szAnnotFmt" to be used.
* 2.	The value of 'iAnnotNChar' is used by PL.LIB when:
*		o	PL_FMT_AUTO is used
*		o	the annotation isn't parallel to the axis.  In this case, the
*			axis is "indented" far enough to create a "margin" wide
*			enough to display iAnnotNChar characters.
* 3.	If the size in points for title or annotations is specified as 0.,
*		then the title or annotations, respectively, won't appear on the
*		plot.
*
* See Also:
*		PlAreaBackground, PlAreaTitleText
*		PlAreaRegYAnnotCallback, PlPlotStyle, PlAreaXStyle, PlAreaArrayInit
*
*-Date     Author		Revision
* -------- ------------	--------
* 06-05-95 R. Cole		created
* 08-16-95 R. Cole		split into PlArea{X|Y}Style
* 08-16-95 R. Cole		added major/minor axis divisions; added annotation
*						rotation and width; changed to allow multi-line
*						plot area title left and/or right of the plot area;
*--------------------------------------------------------------------------*/
int PASCAL
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
const char *szTitleFont)// I name of font for axis title, or "" or NULL
{
	int		retStat=PL_OK, inpErr=0;
	PL_AXIS	*pAxis;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pPlot->pYAxis must not be NULL
* 3		eScaleType must be legal
* 4		eStyle must be legal
* 5		nTitleLinesLeft must be >= 0
* 6		nTitleLinesRight must be >= 0
*		if fAnnotPts is greater than zero, then
* 7			fAnnotDeg must be either 0 or 90
*			if fAnnotDeg is not 90, then
* 8				iAnnotNChar must be greater than 0
* 9		the length of szAnnotFont must be less than PL_MSG_DIM
* 10		szAnnotFmt must not be null
* 11		the length of szAnnotFmt must be less than PL_MSG_DIM
*		if nTitleLinesLeft and/or nTitleLinesRight is > 0, then
* 12		fTitlePts must be greater than zero
*		if fTitlePts is greater than zero, then
* 13		the length of szTitleFont must be less than PL_MSG_DIM
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pAxis = pPlot->pYAxis;
	PL_INP_CHK(2, pAxis == NULL, done)

	retStat = PlArea_style(pPlot, 0, eStyle, eScaleType, nMajor, nMinor,
		fAnnotDeg, iAnnotNChar, fAnnotPts, szAnnotFont, szAnnotFmt,
		nTitleLinesLeft, nTitleLinesRight, fTitlePts, szTitleFont);
	inpErr = pPlot->inpErr;

done:
	PL_IF_INP_ERR("PlAreaYStyle")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlAreaZoomStyle - sets zoom flags for plot area
*
* Purpose:
*		Controls whether zoom affects the X axis and/or Y axis of the
*		currently selected plot area.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	PlAreaArrayInit sets all plot areas so that both axes are
*		affected by zooming.  This routine must be called for any plot
*		areas for which zoom is inhibited following the PlAreaArrayInit
*		call.
*
* See Also:
*		PlAreaArrayInit
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-25-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlAreaZoomStyle(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bXNoZoom,		// I 1 to inhibit X zoom
int		bYNoZoom)		// I 1 to inhibit Y zoom
{
	int		retStat=PL_OK, inpErr=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pPlot->pXAxis->bNoZoom = bXNoZoom ? 1 : 0;
	pPlot->pYAxis->bNoZoom = bYNoZoom ? 1 : 0;

done:
	PL_IF_INP_ERR("PlAreaZoomStyle")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlAutoLinearEnds - choose `clean' endpoint values for an axis
*
* Purpose:
*		Choose axis endpoint values that will produce tasteful axis
*		calibration.
*
* Return Value:
*		void
*
* Notes:
* 1.	The new endpoints are guaranteed not to be equal.
*
* BUGS:
* 1.	This routine should probably focus some attention on choice of
*		number of intervals for an axis; presently, the new endpoints
*		chosen by this routine may be difficult to use for choosing
*		interval size.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-90 R. Cole		initial version
* 11-20-94 R. Cole		cloned into dem_dll
* 03-27-95 R. Cole		cloned into PL.LIB
* 08-02-95 R. Cole		renamed
*--------------------------------------------------------------------------*/
void
PlAutoLinearEnds(
double dBO, double dTO, double *pdBN, double *pdTN)
{
	//char buf[200];
	//sprintf(buf,"PlAutoLinearEnds(dBO = %f, dTO = %f)\n",dBO, dTO);
	//OutputDebugString("PlAutoLinearEnds\n");

	/////////////////////////////////////////////////////////////////
	// And yet another special case for BINARY and EOSS data
	// PJM April 6, 2008
	// 
	// CGUI_PlotPane::SetRangeY() in RAD sets up these canned 0 - 2 limits
	// on the Y-axis.  The EXACTNESS of the values is key and not likely
	// if any calculations are envolved.
	//
	if ((dBO == 0.0) && (dTO == 2.0))
	{
		//Beep(500,100);
		*pdBN = 0.0;	//bottom
		*pdTN = 2.0;	//top
		return;
	}

	/////////////////////////////////////////////////////////////////////////
	//  THIS is the "new" part.  Written by PJM for the Rokkasho standalone.
	//  Date: September 18, 2006
	//  TeamTrack DR 329.
	//	Modified again to take care of the zero case on October 22, 2006.
	//
	//  SPECIAL CASE IF BOTH ARE NON-NEGATIVE AND BOTH ARE LESS THAN 1
	//
	double dMinY = dBO;
	double dMaxY = dTO;

	if (dMinY >= 0.0 &&
		dMinY < 1.0 &&
		dMaxY > 0.0 &&
		dMaxY < 1.0)
	{
		int iminexp;
		double d;

		if (dMinY == 0.0)  //special case within special case PJM 22 October 2006
		{
			iminexp = 0;
			d = 0.0;
		}
		else
		{
			//truncate fpr the minimum (i.e. 0.002345 goes to 0.002)
			iminexp = -1;
			double d = dMinY/10.;
			do
			{
				d *= 10.0;
				iminexp++;
			}while (d < 1.0);
		}

		double dmin = (int)d / pow(10.0, (double)iminexp);

		//bump up for the maximum y (i.e. 0.0745 goes to 0.08
		int imaxexp = -1;
		d = dMaxY/10.0;
		do
		{
			d *= 10.0;
			imaxexp++;
		}while (d < 1.0);

		double dmax = (1 + (int)d)/ pow(10.0, (double)imaxexp);

		//if the difference between the bottom and the top vs zero and the top is
		//10% or less, then make the bottom zero.  Arbitrary...

		double dratio = dmin/dmax;//bottomtotop/dzerototop;

		if (dratio <= .10)
			*pdBN = 0.0;
		else
			*pdBN = dmin;

		*pdTN = dmax;

		return;
	}
	//
	//  END OF "NEW" PART  PJM
	//////////////////////////////////////////////////////////////////////



	double	dBot, dTop;
	double	x1, x2, x1a, x2a, new1, new2;
	double	pwr1, pwr2, pwr;

	if (dBO == 0.) {
		ULONG	ulVal=(ULONG)dTO + 1;
		if (ulVal % 2 == 0) {
			int		i;
			ULONG	twoPwr=1;
			for (i=1; i<31; i++) {
				twoPwr *= 2;
				if (ulVal == twoPwr) {
					*pdBN = 0.;
					*pdTN = twoPwr;
					return;
				}
			}
		}
	}

	dBot = dBO;
	dTop = dTO;
	if (dBot == dTop) {
		if (dBot == 0.)		dBot = -1., dTop = 1.;
		else if (dBot < 0.)	dBot -= 1., dTop = 0.;
		else			dBot = 0., dTop += 1.;
	}

	// for "reversed" axis, temporarily put it "normal", to make life easy
	if (dBot > dTop) {	x1 = dTop;	x2 = dBot; }
	else {			x1 = dBot;	x2 = dTop; }
	// and get the absolute values, which are used for some operations
	x1a = x1 >= 0. ? x1 : -x1;
	x2a = x2 >= 0. ? x2 : -x2;

/*---------------------------------------------------------------------------
*    Now, find a reasonable place to round each end to.
*
*    If the numbers are the same sign then the magnitude of the difference
*    controls what "boundary" to round to.
*
*    Otherwise, the larger magnitude value controls what "boundary" to
*    round to.
*
*    Absolute values are used in the sleuthing.
*--------------------------------------------------------------------------*/
	if ((x1 >= 0. && x2 >= 0.) || (x1 < 0. && x2 < 0.))
		pwr = pow(10., (double)((int)log10(x2 - x1)));
	else {
		if (x1a == 0.)
			pwr1 = x1a;
		else
			pwr1 = pow(10., (double)((int)log10(x1a)));
		if (x2a == 0.)
			pwr2 = x2a;
		else
			pwr2 = pow(10., (double)((int)log10(x2a)));
		pwr = pwr1>pwr2 ? pwr1 : pwr2;
	}

/*---------------------------------------------------------------------------
*    actually do the rounding; and restore the values' original signs
*--------------------------------------------------------------------------*/
//	if (x1 < 0.) {	new1 = (1+floor(x1a/pwr-.0001)) * pwr; new1 = -new1; }
//	else		new1 = (floor(x1a/pwr-.0001)) * pwr;
//	if (x1 < 0.) {	new1 = (1+floor(x1a/pwr)) * pwr; new1 = -new1; }
	if (x1 < 0.) {	new1 = (ceil(x1a/pwr)) * pwr; new1 = -new1; }
	else if (x1 == 0.) new1 = 0.;
	else		new1 = floor(x1a/pwr) * pwr;

//	if (x2 < 0.) {	new2 = (floor(x2a/pwr-.0001)) * pwr;	new2 = -new2; }
//	else		new2 = (1+floor(x2a/pwr-.0001)) * pwr;
	if (x2 < 0.) {	new2 = floor(x2a/pwr) * pwr;	new2 = -new2; }
	else if (x2 == 0.) new2 = 0.;
//	else		new2 = (1+floor(x2a/pwr)) * pwr;
	else		new2 = (ceil(x2a/pwr)) * pwr;

	// Unscramble if input was "reversed"; and give values to caller.
	if (dBot < dTop)	*pdBN = new1, *pdTN = new2;
	else			*pdBN = new2, *pdTN = new1;

	return;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlAutoLogEnds - choose `clean' endpoint values for an axis
*
* Purpose:
*		Choose axis endpoint values that will produce tasteful axis
*		calibration.
*
* Return Value:
*		void
*
* Notes:
* 1.	The new endpoints are guaranteed not to be equal.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-19-95 R. Cole		created
*--------------------------------------------------------------------------*/
void
PlAutoLogEnds(
double dBO, double dTO, double *pdBN, double *pdTN)
{
	double	newB, newT;

	newB = dBO;
	newT = dTO;
	if (newB == newT) {
		if (newB <= 0.)		newB = 0., newT = 10.;
		else			newB *= .5, newT *= 2.;
	}
	if (newB <= 0.) {
		*pdBN = 0.;
	}
	else {
		newB = floor(log10(newB));
		*pdBN = pow(10., newB);
	}
	newT = ceil(log10(newT));
	*pdTN = pow(10., newT);
}
