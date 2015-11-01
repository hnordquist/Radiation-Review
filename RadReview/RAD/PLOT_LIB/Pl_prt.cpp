/* ======================================================================= */
/* ============================== pl_prt.cpp ============================= */
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
* pl_prt.cpp - PlPrtXxx routines
*
* Purpose:
*		The PlPrtXxx functions are focused on the printer.  They include
*		functions to get a DC to the default printer, to start and end
*		documents, etc.
*
* Notes:
* 1.	The routines in this module do relatively little checking of
*		input arguments.  It is assumed that the calling routine is
*		well tested and that it has already done the appropriate checking
*		on the input arguments.
*
* Date     Author		Revision
* -------- ------------	--------
* 06-08-95 R. Cole		created
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

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <commdlg.h>
#include "pl.h"
#include "pl_pvt.h"
#include "wu.h"
#define IDC_PLPRT_CANCEL 100

#if !defined WIN32
#define FAR16 _far
#define EXPORT16 __export
#define EXPORT32
#else
#define FAR16
#define EXPORT16
#define EXPORT32 __declspec(dllexport)
#endif

/*+/csubr/TOC----------------------------------------------------------------
* PlPrtClose - close a print job for a Windows printer
*
* Purpose:
*		Closes a print job for a Windows printer.  "end page" and "end
*		document" commands are sent to the printer.
*
*		After closing the print job, this routine wraps up operations
*		for the PL_CTX for the printout.
*
* Return Value:
*		PL_OK, or
*		PL_PRT_CANCEL  if bAbort is 1 or if PlPrtStatusWindow has been
*					called and the operator clicks Cancel, or
*		other codes for other errors
*
* Notes:
* 1.	If bAbort is 1 (or if pPlot->bPrintCancel is 1), then the printout
*		is aborted, so that nothing is sent to the printer.
* 2.	This routine automatically calls PlotWrapup.
*
* See Also:
*		PlPrtOpenDefault, PlPrtOpenDialog, PlPrtStatusWindow
*
*-Date     Author		Revision
* -------- ------------	--------
* 06-08-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPrtClose(
PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
int		bAbort)			// I 1 to abort the printout
{
	int		retStat=PL_OK;
	int		stat;
	PRINTDLG *pPd=NULL;

	if (pPlot->hDC != 0) {
		if (bAbort || pPlot->bPrintCancel)
			stat = AbortDoc(pPlot->hDC);
		else {
			stat = EndPage(pPlot->hDC);
			stat = EndDoc(pPlot->hDC);
		}
	}
	if (pPlot->pPd != NULL) {
		pPd = (PRINTDLG *)pPlot->pPd;
		if (pPd->hDevMode != 0) GlobalFree(pPd->hDevMode);
		if (pPd->hDevNames != 0) GlobalFree(pPd->hDevNames);
		GlobalUnlock(pPlot->hPD);
	}
	if (pPlot->hPD != 0) GlobalFree(pPlot->hPD);
	if (pPlot->hDC != 0) DeleteDC(pPlot->hDC);
	pPlot->hDC = 0;
	pPlot->hPD = 0;
	pPlot->pPd = NULL;

	retStat = PlPlotWrapup(pPlot);
	strcpy(pPlot->szPrintOpen, "cls");

	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPrtNewPage - finish a page and start a new one
*
* Purpose:
*		Creates a page break, finishing the current page and starting
*		a new one.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-25-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPrtNewPage(
PL_CTX	*pPlot)			// IO pointer to plot context struct for printing
{
	int		retStat=PL_OK;
	int		stat;

	if (pPlot->bPrintCancel) { retStat = PL_PRT_CANCEL; goto done; }

	if (pPlot->hDC == 0) goto done;
	stat = EndPage(pPlot->hDC);
	if (stat < 0)
	    goto gdi_error;
	stat = StartPage(pPlot->hDC);
	if (stat <= 0)
	    goto gdi_error;

done:
	return retStat;
gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlPrtOpen - open a print job for a Windows printer
*
* Purpose:
*		Opens a print job for a Windows printer, and issues "start
*		document" and "start page" commands.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-19-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPrtOpen(
PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
HDC		hDC,			// I handle for device context
HGLOBAL	hPD,			// I handle for PRINTDLG, or 0
void	*pPd,			// I pointer to PRINTDLG, or NULL
const char *szPrtName,	// I printer name, or "" or NULL
const char *szPrtPort,	// I printer port, or "" or NULL
const char *szPrtJob,	// I name of print job, or "" or NULL
const char *szPrtFile)	// I path for print file, or "" or NULL
{
	int		retStat=PL_OK;
	DOCINFO	docInfo;
	int		stat;

	if (szPrtName == NULL) szPrtName = "";
	if (szPrtPort == NULL) szPrtPort = "";
	if (szPrtJob == NULL) szPrtJob = "";
	if (szPrtFile == NULL) szPrtFile = "";

	pPlot->hDC = hDC;
	pPlot->hPD = hPD;
	pPlot->pPd = pPd;
	pPlot->szPrtName = pPlot->szPrtPort = pPlot->szPrtFile = pPlot->szPrtJob = NULL;

	docInfo.cbSize = sizeof(docInfo);
	docInfo.lpszDocName = szPrtJob;
	if (szPrtFile[0] != '\0') docInfo.lpszOutput = szPrtFile;
	else docInfo.lpszOutput = NULL;
	docInfo.lpszDatatype = NULL;
	docInfo.fwType = 0;
	char printer_string[256];
	strcpy (printer_string, szPrtPort);
	stat = StartDoc(hDC, &docInfo);
	if (stat <= 0)
	    goto gdi_error;
	stat = StartPage(hDC);
	if (stat <= 0)
	    goto gdi_error;

#define STORE_IT(psz, sz) \
	psz = (char *)malloc((strlen(sz) + 1) * sizeof(char)); \
	if (psz == NULL) goto malloc_fail; \
	strcpy(psz, sz);
	STORE_IT(pPlot->szPrtName, szPrtName)
	STORE_IT(pPlot->szPrtPort, szPrtPort)
	STORE_IT(pPlot->szPrtFile, szPrtFile)
	STORE_IT(pPlot->szPrtJob, szPrtJob)
#undef STORE_IT
	pPlot->bPrintCancel = 0;
	pPlot->hwPrintCancel = NULL;

	// szPrintOpen is the basis for PlPlotInit_prt deciding whether
	// to preserve hDC, hPD, and pPd.  The casual caller will be using
	// an unitialized PL_CTX for printing, and the odds of having this
	// member accidentally be "prt" are small.
	strcpy(pPlot->szPrintOpen, "prt");
	retStat = PlPlotInit_prt(pPlot, NULL, 0.F, 1.F, 1.F, 1.F, 1.F);
	if (retStat != PL_OK) goto done;

done:
	//if (retStat != PL_OK) {
#define FREE_IT(psz) if (psz != NULL) { free(psz); psz = NULL; }
		FREE_IT(pPlot->szPrtName)
		FREE_IT(pPlot->szPrtPort)
		FREE_IT(pPlot->szPrtFile)
		FREE_IT(pPlot->szPrtJob)
#undef FREE_IT
	//}
	return retStat;
gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
malloc_fail:
	retStat = PL_MALLOC_FAIL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPrtOpenDefault - open a print job to Windows default printer
*
* Purpose:
*		Opens a print job to the Windows default printer, and issues "start
*		document" and "start page" commands.
*
*		If szPrtJob is "" and szPrtFile is not "", then the output is
*		formatted for the default printer but written to szPrtFile.
*
*		The print job is started with default margins of 1 inch on
*		each side.  Call PlPlotInit_prt to change the margins.
*
*		PlPlotInit_prt can also be used to set up an area on the paper
*		as a facsimile of a plot window on the screen.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
* Notes:
* 1.	When printing is complete, PlPrtClose must be called to wrap up
*		printing (and to releasee print-related resources).
* 2.	If both szPrtJob and szPrtFile are "" or both are not "", then
*		the printout is directed to the default printer.  The user can
*		print the file at a later time by copying it to the printer's port.
*
* See Also:
*		PlPrtOpenDialog, PlPrtClose, PlPrtStatusWindow
*
*-Date     Author		Revision
* -------- ------------	--------
* 06-08-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPrtOpenDefault(
PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
const char *szPrtJob,	// I name of print job, or "" or NULL
const char *szPrtFile)	// I path for print file, or "" or NULL
{
	int		retStat=PL_OK;
	int		stat;
	HDC		hDC=0;
	char	szPrt[64], *szDrv, *szDev, *szPort;
	HCURSOR	hCurOld=0;

	if (szPrtJob == NULL) szPrtJob = "";
	if (szPrtFile == NULL) szPrtFile = "";

	// Get the default printer from the [windows] section, device= record.
	// It will be something like:
	//		device=HP LaserJet IIID,hppcl5a,LPT1:
	stat = GetProfileString("windows", "device", "", szPrt, 64);
	if (stat < 1)
	    goto gdi_error;
	szDev = strtok(szPrt, ",");	// HP LaserJet IIID
	szDrv = strtok(NULL, ",");	// hppcl5a
	szPort = strtok(NULL, ",");	// LPT1:

	hDC = CreateDC(szDrv, szDev, szPort, NULL);
	if (hDC == 0)
	    goto gdi_error;
	retStat = PlPrtOpen(pPlot, hDC, 0, NULL, szDev, szPort, szPrtJob, "");
	if (retStat != PL_OK) goto bail_out;

done:
	return retStat;
gdi_error:
	retStat = PL_GDI_FAIL;
bail_out:
	if (hDC != 0) DeleteDC(hDC);
	pPlot->hDC = 0;
	pPlot->hPD = 0;
	pPlot->pPd = NULL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPrtOpenDialog - open a print job to operator-selected printer
*
* Purpose:
*		Opens a print job to the operator-selected printer.
*
*		A 'Print' dialog box is displayed to the operator, to allow
*		choosing the printer to be used.  "start document" and "start
*		page" commands are issued to the printer.
*
*		If the operator clicks Cancel, this routine will return
*		PL_PRT_CANCEL.  In this case, the calling program must not proceed
*		with printing, and PlPrtClose must not be called.
*
*		The print job is started with default margins of 1 inch on
*		each side.  Call PlPlotInit_prt to change the margins.
*
*		PlPlotInit_prt can also be used to set up an area on the paper
*		as a facsimile of a plot window on the screen.
*
*		When printing operations are complete, PlPrtClose must be called
*		to complete the print job and to wrap up interactions with the
*		plotting library.
*
* Return Value:
*		PL_OK, or
*		PL_PRT_CANCEL  if the operator clicks Cancel in the dialog box, or
*		PL_FILE  if the operator checks the 'Print to file' box,
*		other codes for other errors
*
* Notes:
* 1.	When printing is complete, PlPrtClose must be called to wrap up
*		printing (and to releasee print-related resources).
* 2.	If szPrtFile isn't "", then the 'Print' dialog will display a
*		'Print to file' checkbox.  If the user checks the box, then the
*		printout will be formatted for the selected printer, but written
*		to the file specified by szPrtFile.  The user can print the file
*		at a later time by copying it to the printer's port.
*
* See Also:
*		PlPrtOpenDefault, PlPrtClose, PlPrtStatusWindow
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-19-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPrtOpenDialog(
PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
const char *szPrtJob,	// I name of print job, or "" or NULL
const char *szPrtFile)	// I path for print file, or "" or NULL
{
	int		retStat=PL_OK;
	int		stat;
	HDC		hDC=0;
	HGLOBAL	hPD=0;
	PRINTDLG *pPd=NULL;
	DEVNAMES *pDevNames=NULL;
	HCURSOR	hCurOld=0;

	if (szPrtJob == NULL) szPrtJob = "";
	if (szPrtFile == NULL) szPrtFile = "";

	hPD = GlobalAlloc(GPTR, sizeof(PRINTDLG));
	if (hPD == 0)
	    goto gdi_error;
	pPd = (PRINTDLG *)GlobalLock(hPD);
	if (pPd == NULL)
	    goto gdi_error;
	pPd->lStructSize = sizeof(PRINTDLG);
	pPd->hwndOwner = NULL;
	pPd->Flags = PD_RETURNDC | PD_ALLPAGES | PD_NOPAGENUMS | PD_NOSELECTION;
	if (szPrtFile[0] == '\0') pPd->Flags |= PD_HIDEPRINTTOFILE;
	hCurOld = SetCursor(LoadCursor(NULL, IDC_ARROW));
	stat = PrintDlg(pPd);
	if (hCurOld != 0) SetCursor(hCurOld);
	if (stat == 0) goto cancel;
	pDevNames = (DEVNAMES *)GlobalLock(pPd->hDevNames);
	if (pDevNames == NULL)
	    goto gdi_error;
	hDC = pPd->hDC;
	if (hDC == 0)
	    goto gdi_error;
	if ((pPd->Flags & PD_PRINTTOFILE) == 0)
		szPrtFile = "";

	retStat = PlPrtOpen(pPlot, hDC, hPD, pPd,
						(char *)pDevNames + pDevNames->wDeviceOffset,
						(char *)pDevNames + pDevNames->wOutputOffset,
						szPrtJob, szPrtFile);
	if (retStat != PL_OK) goto bail_out;
	if (pPd->Flags & PD_PRINTTOFILE)
		retStat = PL_FILE;

done:
	return retStat;
cancel:
	retStat = PL_PRT_CANCEL;
	goto bail_out;
gdi_error:
	retStat = PL_GDI_FAIL;
bail_out:
	if (pDevNames != NULL) GlobalUnlock(pPd->hDevNames);
	if (pPd != NULL) {
		if (pPd->hDevMode != 0) GlobalFree(pPd->hDevMode);
		if (pPd->hDevNames != 0) GlobalFree(pPd->hDevNames);
		GlobalUnlock(hPD);
	}
	if (hPD != 0) GlobalFree(hPD);
	if (hDC != 0) DeleteDC(hDC);
	pPlot->hDC = 0;
	pPlot->hPD = 0;
	pPlot->pPd = NULL;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPrtPrintSimple - print the plot as it appears on the screen
*
* Purpose:
*		Prints the plot that is currently on the screen, with the current
*		zoom factors and scroll positions.
*
*		The margin arguments control the size and position of the plot on
*		the printed page.  Note that this provides independence from the
*		paper orientation that is currently in effect.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	A dialog box is presented to the operator to allow choosing the
*		printer to be used.
* 2.	When this routine begins sending the plot to the printer, a dialog
*		box is presented to the operator to allow canceling the print job.
* 3.	If fBorderPts is 0, no border is drawn around the plot on the page.
*
* See Also:
*		PlPrtOpenDefault, PlPrtOpenDialog, PlPlotInit_prt
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-29-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPrtPrintSimple(
PL_CTX	*pPlot,			// I pointer to plot context struct for screen
const char *szJobName,	// I name for print job in Windows Print Manager
float	fBorderPts,		// I thickness of border to be drawn, or 0.F
float	fLMarg_in,		// I margin left of plot area, in inches
float	fRMarg_in,		// I margin right of plot area, in inches
float	fTMarg_in,		// I margin above plot area, in inches
float	fBMarg_in)		// I margin below plot area, in inches
{
	int		retStat=PL_OK, inpErr=0;
	HCURSOR	hCurOld=0;
	PL_CTX	plPrtCtx;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	if (szJobName == NULL) szJobName = "plot";

	hCurOld = SetCursor(LoadCursor(NULL, IDC_WAIT));
	retStat = PlPrtOpenDialog(&plPrtCtx, szJobName, "");
	if (retStat != PL_OK) goto done;
	retStat = PlPlotInit_prt(&plPrtCtx, pPlot,
			fBorderPts, fLMarg_in, fRMarg_in, fTMarg_in, fBMarg_in);
	if (retStat != PL_OK) goto print_error;
	PlPrtStatusWindow(&plPrtCtx, pPlot->hwPlot, 0);
	PlWinRepaint(&plPrtCtx);
	PlPrtStatusWindow(&plPrtCtx, pPlot->hwPlot, 1);
	retStat = PlPrtClose(&plPrtCtx, 0);

done:
	if (hCurOld != 0) SetCursor(hCurOld);
	PL_IF_INP_ERR("PlPrtPrintSimple")
	return retStat;
print_error:
	PlPrtClose(&plPrtCtx, 1);
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlPrtStatusPaint - paint the information in the status window
*
* Purpose:
*		Paints the information in the status window.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 11-02-96 R. Cole		created
*--------------------------------------------------------------------------*/
void
PlPrtStatusPaint(
PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
int		bPaint,			// I 1 to print, 0 to lay out window
RECT	*prWin,			// I pointer to window's rect
HDC		hDC,			// IO handle to DC for status window
int		*piYpx)			// IO pointer to Y position
{
	RECT	rectLine;
	char	szLine[PL_MSG_DIM];
	UINT	uiFlags=DT_CENTER | DT_WORDBREAK;

	if (!bPaint)
		uiFlags |= DT_CALCRECT;
	rectLine.left = 5;
	rectLine.right = prWin->right - prWin->left - 9;
	rectLine.top = *piYpx;
	rectLine.bottom = *piYpx + 50;
	*piYpx += DrawText(hDC, "Printing", -1, &rectLine, uiFlags);
	*piYpx += 5;
	if (pPlot->szPrtJob != NULL && *pPlot->szPrtJob != '\0') {
		rectLine.right = prWin->right - prWin->left - 9;
		rectLine.top = *piYpx;
		rectLine.bottom = *piYpx + 50;
		*piYpx += DrawText(hDC, pPlot->szPrtJob, -1, &rectLine, uiFlags);
		*piYpx += 5;
	}
	if (pPlot->szPrtName != NULL && pPlot->szPrtPort != NULL) {
		rectLine.right = prWin->right - prWin->left - 9;
		rectLine.top = *piYpx;
		rectLine.bottom = *piYpx + 50;
		_snprintf(szLine, PL_MSG_DIM-1, "to '%s' on '%s'",
									pPlot->szPrtName, pPlot->szPrtPort);
		*piYpx += DrawText(hDC, szLine, -1, &rectLine, uiFlags);
		*piYpx += 5;
	}
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlPrtProc - handle events for a status window
*
* Purpose:
*		Handles events for a status window.
*
* Return Value:
*		
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-25-96 R. Cole		created
*--------------------------------------------------------------------------*/
LRESULT EXPORT16  CALLBACK
PlPrtProc(
HWND	hWnd,
UINT	iMsg,
WPARAM	wParam,
LPARAM	lParam)
{
	PL_CTX	*pPlot;

	pPlot = (PL_CTX *)GetWindowLong(hWnd, 0);

	if (iMsg == WM_COMMAND) {
		if (wParam == IDC_PLPRT_CANCEL)
			goto cancel_exit;
		else
			goto default_proc;
	}
	else if (iMsg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC		hDC;
		RECT	rectWin;
		int		iYpx=5;

		hDC = BeginPaint(hWnd, &ps);
		GetWindowRect(hWnd, &rectWin);
		PlPrtStatusPaint(pPlot, 1, &rectWin, hDC, &iYpx);
		EndPaint(hWnd, &ps);
	}
	else if (iMsg == WM_KEYDOWN) {
		if (wParam == VK_RETURN)
			goto cancel_exit;
		else {
			MessageBeep(0);
			goto default_proc;
		}
	}
	else if (iMsg == WM_SYSCOMMAND) {
		if (wParam == SC_CLOSE)
			DestroyWindow(hWnd);
		else
			goto default_proc;
	}
	else
		goto default_proc;

default_proc:
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
cancel_exit:
	pPlot->bPrintCancel = 1;
	DestroyWindow(pPlot->hwPrintCancel);
	pPlot->hwPrintCancel = 0;
	return 0;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlPrtWin - create a status window
*
* Purpose:
*		Creates a status window.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-25-96 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
PlPrtWin(
HINSTANCE hInst,		// I handle to this instance
HWND	hwParent,		// I parent window handle
PL_CTX	*pPlot)			// IO pointer to plot context struct for printing
{
	HWND	hwStatus=0;
	char	*szClass="PLPRTST";
	WNDCLASS statusClass;
	ATOM	atom;
	int		stat;

	stat = GetClassInfo(hInst, szClass, &statusClass);
	if (stat == 0) {  // the class hasn't been registered yet
		statusClass.style		= CS_OWNDC;
		statusClass.lpfnWndProc	= PlPrtProc;
		statusClass.cbClsExtra	= 0;
		statusClass.cbWndExtra	= 1 * sizeof(void *);
		statusClass.hInstance	= hInst;
		statusClass.hIcon		= NULL;
		statusClass.hCursor		= NULL;
		statusClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		statusClass.lpszMenuName = NULL;
		statusClass.lpszClassName = szClass;
		atom = RegisterClass(&statusClass);
		if (atom == 0)
			goto done;
	}

	hwStatus = CreateWindow(szClass, "", WS_POPUPWINDOW | WS_CAPTION,
							50, 50, 300, 100, hwParent, 0, hInst, NULL);
	if (hwStatus == 0)
		goto done;
	SetWindowLong(hwStatus, 0, (DWORD)pPlot);
done:
	return hwStatus;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlPrtStatusWindow - show or dismiss the printing status window
*
* Purpose:
*		Shows or dismisses the printing status window.
*
*		This routine can be called before printing begins to display
*		a status window to the operator.  If the operator clicks the
*		CANCEL button, then the program is notified (via PL.LIB
*		return status codes) to cancel the printout.
*
*		After printing is complete, this routine is called to dismiss
*		the status window.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
* Notes:
* 1.	If this routine is used, it must be called after PlPrtOpen is
*		called.
* 2.	To dismiss the printing status window, call this routine prior
*		to the call to PlPrtClose.
* 3.	Typically, hwParent will be the handle for the program's main
*		window or a dialog box.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-25-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlPrtStatusWindow(
PL_CTX	*pPlot,			// IO pointer to plot context struct for printing
HWND	hwParent,		// I handle for parent window
int		bDismiss)		// I 0 (or 1) to show (or dismiss) the window
{
	int		retStat=PL_OK;
	HINSTANCE hInst;
	HWND	hwStatus, hwBtn;
	RECT	rectWin;
	HDC		hDC=0;
	int		iYpx=5;

	if (!bDismiss && pPlot->bPrintCancel) goto done;

	if (bDismiss) {
		WuDoEvents();	// allow possible "Cancel" click to take effect
		if (pPlot->hwPrintCancel != 0)
			DestroyWindow(pPlot->hwPrintCancel);
		pPlot->hwPrintCancel = 0;
	}
	else {
		hInst = WuGetHInstance(hwParent);
		hwStatus = PlPrtWin(hInst, hwParent, pPlot);
		if (hwStatus == 0)
		    goto gdi_error;
		pPlot->hwPrintCancel = hwStatus;
		hDC = GetDC(hwStatus);
		if (hDC == 0)
		    goto gdi_error;
		GetWindowRect(hwStatus, &rectWin);
		SetWindowText(hwStatus, "Printing");
		PlPrtStatusPaint(pPlot, 0, &rectWin, hDC, &iYpx);
		iYpx += 5;
		hwBtn = WuButton_dflt(hwStatus, IDC_PLPRT_CANCEL, "Cancel", 
			(rectWin.right - rectWin.left) / 2 - 50, iYpx, 100, 22);
		if (hwBtn == 0)
		    goto gdi_error;
		iYpx += 27;
		rectWin.bottom = rectWin.top + iYpx +
						GetSystemMetrics(SM_CYCAPTION) +
						GetSystemMetrics(SM_CYBORDER);
		MoveWindow(hwStatus, rectWin.left, rectWin.top,
						rectWin.right - rectWin.left + 1,
						rectWin.bottom - rectWin.top + 1, FALSE);
		ShowWindow(hwStatus, SW_SHOW);
		WuDoEvents();
	}

done:
	if (hDC != 0) ReleaseDC(hwStatus, hDC);
	return retStat;
gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
