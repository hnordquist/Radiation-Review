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
/* ================================ pl_c.c =============================== */
/* ======================================================================= */

/*+/mod/TOC------------------------------------------------------------------
* pl_c.c - C-compatible interface to pl.cpp
*
* Purpose:
*		The pl_c.c module provides access from C (and VB) programs to
*		routines in pl.cpp.
*
*		Only a subset of the PL_LIB.LIB routines require special access.
*		The descriptions for those routines give the details for accessing
*		the routine from C and VB.
*
* Date     Author		Revision
* -------- ------------	--------
* 03-23-95 R. Cole		created
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
#include "pl.h"
#include "pl_pvt.h"

int PASCAL
PlDrawXYVectors_sht(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
const short HUGE *psXVec,// I X data vector
const short HUGE *psYVec,// I Y data vector
int		eDrawType,	// I drawing type--PL_POINT or PL_LINE
float	fPts,		// I drawing thickness or mark size, in points
COLORREF rgb)		// I color to use for drawing
{ return PlDrawXYVectors(pPlot, n, psXVec, psYVec, eDrawType, fPts, rgb); }

int PASCAL
PlDrawXYVectors_lng(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
const long HUGE *plXVec,// I X data vector
const long HUGE *plYVec,// I Y data vector
int		eDrawType,	// I drawing type--PL_POINT or PL_LINE
float	fPts,		// I drawing thickness or mark size, in points
COLORREF rgb)		// I color to use for drawing
{ return PlDrawXYVectors(pPlot, n, plXVec, plYVec, eDrawType, fPts, rgb); }

int PASCAL
PlDrawXYVectors_flt(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
const float HUGE *pfXVec,// I X data vector
const float HUGE *pfYVec,// I Y data vector
int		eDrawType,	// I drawing type--PL_POINT or PL_LINE
float	fPts,		// I drawing thickness or mark size, in points
COLORREF rgb)		// I color to use for drawing
{ return PlDrawXYVectors(pPlot, n, pfXVec, pfYVec, eDrawType, fPts, rgb); }

int PASCAL
PlDrawXYVectors_dbl(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
const double HUGE *pdXVec,// I X data vector
const double HUGE *pdYVec,// I Y data vector
int		eDrawType,	// I drawing type--PL_POINT or PL_LINE
float	fPts,		// I drawing thickness or mark size, in points
COLORREF rgb)		// I color to use for drawing
{ return PlDrawXYVectors(pPlot, n, pdXVec, pdYVec, eDrawType, fPts, rgb); }

int PASCAL
PlDrawXYVectors_ul(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
const ULONG HUGE *pulXVec,// I X data vector
const ULONG HUGE *pulYVec,// I Y data vector
int		eDrawType,	// I drawing type--PL_POINT or PL_LINE
float	fPts,		// I drawing thickness or mark size, in points
COLORREF rgb)		// I color to use for drawing
{ return PlDrawXYVectors(pPlot, n, pulXVec, pulYVec, eDrawType, fPts, rgb); }

int PASCAL
PlDrawYVector_sht(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
double	dXLeft,		// I X value at left end of X axis
double	dXIncr,		// I X value spacing between points
const short HUGE *psYVec,// I Y data vector
int		eDrawType,	// I drawing type--PL_POINT or PL_LINE
float	fPts,		// I drawing thickness or mark size, in points
COLORREF rgb)		// I color to use for drawing
{ return PlDrawYVector(pPlot, n, dXLeft, dXIncr, psYVec, eDrawType, fPts, rgb); }

int PASCAL
PlDrawYVector_lng(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
double	dXLeft,		// I X value at left end of X axis
double	dXIncr,		// I X value spacing between points
const long HUGE *plYVec,// I Y data vector
int		eDrawType,	// I drawing type--PL_POINT or PL_LINE
float	fPts,		// I drawing thickness or mark size, in points
COLORREF rgb)		// I color to use for drawing
{ return PlDrawYVector(pPlot, n, dXLeft, dXIncr, plYVec, eDrawType, fPts, rgb); }

int PASCAL
PlDrawYVector_flt(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
double	dXLeft,		// I X value at left end of X axis
double	dXIncr,		// I X value spacing between points
const float HUGE *pfYVec,// I Y data vector
int		eDrawType,	// I drawing type--PL_POINT or PL_LINE
float	fPts,		// I drawing thickness or mark size, in points
COLORREF rgb)		// I color to use for drawing
{ return PlDrawYVector(pPlot, n, dXLeft, dXIncr, pfYVec, eDrawType, fPts, rgb); }

int PASCAL
PlDrawYVector_dbl(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
double	dXLeft,		// I X value at left end of X axis
double	dXIncr,		// I X value spacing between points
const double HUGE *pdYVec,// I Y data vector
int		eDrawType,	// I drawing type--PL_POINT or PL_LINE
float	fPts,		// I drawing thickness or mark size, in points
COLORREF rgb)		// I color to use for drawing
{ return PlDrawYVector(pPlot, n, dXLeft, dXIncr, pdYVec, eDrawType, fPts, rgb); }

int PASCAL
PlDrawYVector_ul(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
double	dXLeft,		// I X value at left end of X axis
double	dXIncr,		// I X value spacing between points
const ULONG HUGE *pulYVec,// I Y data vector
int		eDrawType,	// I drawing type--PL_POINT or PL_LINE
float	fPts,		// I drawing thickness or mark size, in points
COLORREF rgb)		// I color to use for drawing
{ return PlDrawYVector(pPlot, n, dXLeft, dXIncr, pulYVec, eDrawType, fPts, rgb); }

int PASCAL
PlAreaRangeSetXYVectors_sht(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
const short HUGE *psXVec,// I X data vector
const short HUGE *psYVec)// I Y data vector
{ return PlAreaRangeSetXYVectors(pPlot, n, psXVec, psYVec); }

int PASCAL
PlAreaRangeSetXYVectors_lng(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
const long HUGE *plXVec,// I X data vector
const long HUGE *plYVec)// I Y data vector
{ return PlAreaRangeSetXYVectors(pPlot, n, plXVec, plYVec); }

int PASCAL
PlAreaRangeSetXYVectors_flt(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
const float HUGE *pfXVec,// I X data vector
const float HUGE *pfYVec)// I Y data vector
{ return PlAreaRangeSetXYVectors(pPlot, n, pfXVec, pfYVec); }

int PASCAL
PlAreaRangeSetXYVectors_dbl(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
const double HUGE *pdXVec,// I X data vector
const double HUGE *pdYVec)// I Y data vector
{ return PlAreaRangeSetXYVectors(pPlot, n, pdXVec, pdYVec); }

int PASCAL
PlAreaRangeSetXYVectors_ul(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
const ULONG HUGE *pulXVec,// I X data vector
const ULONG HUGE *pulYVec)// I Y data vector
{ return PlAreaRangeSetXYVectors(pPlot, n, pulXVec, pulYVec); }

int PASCAL
PlAreaRangeSetYVector_sht(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
double	dXLeft,		// I X value at left end of X axis
double	dXIncr,		// I X value spacing between points
const short HUGE *psYVec)// I Y data vector
{ return PlAreaRangeSetYVector(pPlot, n, dXLeft, dXIncr, psYVec); }

int PASCAL
PlAreaRangeSetYVector_lng(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
double	dXLeft,		// I X value at left end of X axis
double	dXIncr,		// I X value spacing between points
const long HUGE *plYVec)// I Y data vector
{ return PlAreaRangeSetYVector(pPlot, n, dXLeft, dXIncr, plYVec); }

int PASCAL
PlAreaRangeSetYVector_flt(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
double	dXLeft,		// I X value at left end of X axis
double	dXIncr,		// I X value spacing between points
const float HUGE *pfYVec)// I Y data vector
{ return PlAreaRangeSetYVector(pPlot, n, dXLeft, dXIncr, pfYVec); }

int PASCAL
PlAreaRangeSetYVector_dbl(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
double	dXLeft,		// I X value at left end of X axis
double	dXIncr,		// I X value spacing between points
const double HUGE *pdYVec)// I Y data vector
{ return PlAreaRangeSetYVector(pPlot, n, dXLeft, dXIncr, pdYVec); }

int PASCAL
PlAreaRangeSetYVector_ul(
PL_CTX	*pPlot,		// IO pointer to plot context structure
long	n,			// I number of points
double	dXLeft,		// I X value at left end of X axis
double	dXIncr,		// I X value spacing between points
const ULONG HUGE *pulYVec)// I Y data vector
{ return PlAreaRangeSetYVector(pPlot, n, dXLeft, dXIncr, pulYVec); }
