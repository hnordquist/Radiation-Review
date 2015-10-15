/* ======================================================================= */
/* ============================== pl_e_2_m.cpp =========================== */
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
* pl_e_2_m.cpp - miscellaneous plotting functions
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
* PlErrorCodeCheck - check to see if a status code is a PL status
*
* Purpose:
*		Checks a status code to see if it is a PL_XXX status code.
*
* Return Value:
*		PL_OK if the status code is a PL_XXX status code, or
*		PL_ILLEGAL otherwise
*
* Notes:
* 1.	This routine is useful for programs that want to get the full
*		information corresponding to a status code.  Knowing that it is
*		an PL_XXX status code allows calling PlErrorMessage to decode
*		it.
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-17-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlErrorCodeCheck(
int		status)			// I status code
{

	if (status >= PL_OK && status <= PL_ILLEGAL) return PL_OK;
	else return PL_ILLEGAL;
}


/*  KG  For some resaon was not getting allocated so move it to PL.cpp*/
//const char *const gaszPlStatusMsg[]={
//const char * FAR16 const gaszPlStatusMsg[]={
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

/*+/csubr/TOC----------------------------------------------------------------
* PlErrorMessage - translate PL_XXX status code to text
*
* Purpose:
*		Generate a text message corresponding to a PL_XXX status code.
*
* Return Value:
*		pointer to error message
*
* Notes:
* 1.	For some status codes, information is available in addition to
*		the text for the status code.  To obtain this additional
*		information, this routine must be called with a non-NULL pPlot.
*
* See Also:
*		PlErrorCodeCheck
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-20-95 R. Cole		created
*--------------------------------------------------------------------------*/
const char * PASCAL
PlErrorMessage(
PL_CTX	*pPlot,			// I pointer to plot context structure, or NULL
int		status)			// I status code
{
	const char *pszMsg;
	char	szMsg[200];

	if (status >= PL_OK && status <= PL_ILLEGAL)
		pszMsg = gaszPlStatusMsg[status - PL_OK];
	else
		pszMsg = gaszPlStatusMsg[PL_ILLEGAL - PL_OK];
	if (pPlot != NULL && status == PL_BAD_INPUT) {
		_snprintf(szMsg, 200, "PL.LIB: input check #%d failed in %s",
			pPlot->inpErr, pPlot->pszFnName);
		szMsg[199] = '\0';
		pszMsg = szMsg;
	}
	if (pPlot != NULL) {
		WuSzMCopy(pPlot->szErrorMsg, PL_MSG_DIM, pszMsg);
		pszMsg = pPlot->szErrorMsg;
	}

	return pszMsg;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlErrorMessage_VB - translate PL_XXX status code to text
*
* Purpose:
*		Generate a text message corresponding to a PL_XXX status code.
*		This function is specifically tailored for use by Visual Basic
*		programs.
*
* Return Value:
*		void
*
* See Also:
*		PlErrorMessage, PlErrorCodeCheck
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-20-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlErrorMessage_VB(
PL_CTX	*pCtx,			// I pointer to context structure, or NULL
int		status,			// I status code
int		msgDim,			// I dimension of message buffer
char *szMsg)			// O place to store message string
{
	const char *pszMsg;

	pszMsg = PlErrorMessage(pCtx, status);
	szMsg[msgDim-1] = '\0';
	strncpy(szMsg, pszMsg, msgDim-1);
}
/*+/csubr/TOC----------------------------------------------------------------
* PlMoveTo - move the plot pen to a position
*
* Purpose:
*		Moves the plot pen to the specified position, either drawing or
*		skipping, as requested.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* BUGS:
* 1.	This routine doesn't perform any clipping services.
* 2.	No line width control is provided.
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-21-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlMoveTo(
PL_CTX	*pPlot,			// IO pointer to plot context structure
double	dXVal,			// I X data value
double	dYVal,			// I Y data value
int		bDraw,			// I 1 causes drawing while moving, else 0
COLORREF rgb)			// I color to use if bDraw==1
{
	int		retStat=PL_OK;
	double	dX1, dY1;
	HDC		hDC;
	HPEN	hPen, hPenOld;

	retStat = PlWinHDCSetup(pPlot, 0, 0, 1,	// clip to plot area
			PL_LINE, 0.F, 0, rgb,
			&hDC, &hPen, &hPenOld, NULL);
	if (retStat != PL_OK) goto done;

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->bNeedScale) {
		retStat = PlWinScale(pPlot);
		if (retStat != PL_OK) goto done;
	}

	dX1 = PlWinCvtXValToXPix(pPlot, dXVal);
	dY1 = PlWinCvtYValToYPix(pPlot, dYVal);

	if (bDraw) {
		if (LineTo(hDC, (int)dX1, (int)dY1) == 0)
			goto gdi_error;
	}
	else		MoveToEx(hDC, (int)dX1, (int)dY1, NULL);

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (pPlot->hDC == 0 && hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
	return retStat;

gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
