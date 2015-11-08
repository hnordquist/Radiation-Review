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
/* ============================== pl_r_2_z.cpp =========================== */
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
* pl_r_2_z.cpp - miscellaneous plotting functions
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
* PlTitleLineToPx - get pixel coordinates of a title line
*
* Purpose:
*		Within a rectangle, calculate the pixel coordinates and orientation
*		for a particular title line.
*
* Return Value:
*		none
*
* Notes:
* 1.	NOTE WELL!! The right and bottom coordinates are the ACTUAL pixel
*		coordinates, NOT 1 past, as with the usual Windows RECT convention.
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
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
float	*pfDeg)			// O text orientation, in degrees
{
	int		iDeltaX, iDeltaY, iDeltaX1;

	iDeltaX = PL_XPTS_TO_XPIX(fTitlePts + 2.F);
	iDeltaY = PL_YPTS_TO_YPIX(fTitlePts + 2.F);
	iDeltaX1 = PL_XPTS_TO_XPIX(1.F);
	if (eSide == PL_SIDE_LEFT) {
		*pfDeg = 90.F;
		*pxPx = leftPx + iLine * iDeltaX + iDeltaX1;
		*pyPx = bottomPx;
		iDeltaY = (int)ceil(fFrac * (*pyPx - topPx));
		*pyPx -= iDeltaY;
	}
	else if (eSide == PL_SIDE_RIGHT) {
		*pfDeg = 90.F;
		*pxPx = rightPx - (nLines - iLine) * iDeltaX + iDeltaX1;
		*pyPx = bottomPx;
		iDeltaY = (int)ceil(fFrac * (*pyPx - topPx));
		*pyPx -= iDeltaY;
	}
	else if (eSide == PL_SIDE_TOP) {
		*pfDeg = 0.F;
		*pxPx = leftPx;
		*pyPx = topPx + iLine * iDeltaY;
		iDeltaX = (int)ceil(fFrac * (rightPx - *pxPx));
		*pxPx += iDeltaX;
	}
	else if (eSide == PL_SIDE_BOTTOM) {
		*pfDeg = 0.F;
		*pxPx = leftPx;
		*pyPx = bottomPx - (nLines - iLine) * iDeltaY + 1;
		iDeltaX = (int)ceil(fFrac * (rightPx - *pxPx));
		*pxPx += iDeltaX;
	}
}
/*+/csubr/TOC----------------------------------------------------------------
* PlVersionCheck - check version compatibility
*
* Purpose:
*		Checks for compatibility between:
*		o	the version of the library the program is linked with and
*		o	the version of PL.H the program was compiled with.
*
* Return Value:
*		PL_OK  if the versions are compatible, or
*		PL_VERSION_MISMATCH  if there is a mismatch
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-20-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlVersionCheck(
int		iVersion)		// I version number from PL.H: PL_VER
{
	if (iVersion == PL_VER)
		return PL_OK;
	else
		return PL_VERSION_MISMATCH;
}
/*+--------------------------------------------------------------------------
*
* Example:
*
* This example demonstrates how to check to see if a program is linked
* with a version of the library that is compatible with the header file
* used for compiling the program.
*
*		if (PlVersionCheck(PL_VER) != PL_OK)
*			error action;
*
*--------------------------------------------------------------------------*/

/*+/csubr/TOC----------------------------------------------------------------
* PlVisibleCheck - check to see if a data point is visible
*
* Purpose:
*		Checks for visibility in the current plot area of a data point.
*
* Return Value:
*		1 if the point is visible, or
*		0 if the point is not visible, or
*		-1 if an error occurs
*
* See Also:
*		PlPaintCursor, PlWinScroll_pos
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-27-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlVisibleCheck(
PL_CTX	*pPlot,			// I pointer to plot context structure
double	dXVal,			// I X data value
double	dYVal)			// I Y data value
{
	double	xPx, yPx;
	int		iXPx, iYPx;

	if (pPlot == NULL) return -1;

	if (pPlot->bNeedScale)
		PlWinScale(pPlot);

	xPx = PlWinCvtXValToXPix(pPlot, dXVal);
	yPx = PlWinCvtYValToYPix(pPlot, dYVal);
	if (xPx < 0 || xPx > 32766)
		return 0;
	if (yPx < 0 || yPx > 32766)
		return 0;
	iXPx = (int)(xPx + .5);
	iYPx = (int)(yPx + .5);
	if (iXPx < pPlot->pXAxis->pxB || iXPx > pPlot->pXAxis->pxT)
		return 0;
	if (iYPx < pPlot->pYAxis->pxT || iYPx > pPlot->pYAxis->pxB)
		return 0;
	return 1;
}
