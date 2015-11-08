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
/* ================================ wu.cpp =============================== */
/* ======================================================================= */

/*---------------------------------------------------------------------------
*                         SPECIAL NOTE TO DEVELOPERS
*
*		Information from this file is extracted to create portions of
*		WMCA_UM.DOC.  Because of this, the appearance of that document
*		is highly dependent on the formatting of information in this file.
*		Thus, it is recommended to remain consistent with existing format
*		conventions in this file.  Please observe the following:
*		o	keep the use of tabs and spaces consistent with existing usage
*			in this file
*		o	set tab width to 4
*		o	use "preserve tabs"
*		o	keep extracted lines shorter than 80 characters
*--------------------------------------------------------------------------*/

/*+/mod/TOC------------------------------------------------------------------
* wu.cpp - Windows API-related functions
*
* Purpose:
*		The wu.cpp module contains a variety of utility functions
*		for operating in Windows.
*
*		The routines in this module allow:
*		o	creating a "main" window
*		o	creating various types of "control" windows
*		o	printf-like access to TextBox controls
*		o	VB programs to store text in C null-terminated strings
*		o	strn...-line routines that are more robust
*		o	and more...
*
* Quick Reference:
*    #include <windows.h>
*    #include <wu.h>
*        WU_QUAD  *pQuad;       // bounding quadrilateral for text
*
*     int MmSound          (szSoundName, bWait)
*     int WuAlert          (hwParent, szAlert, iIcon)
*                      szAlert = "title|subject|discussion|btn0|choice0|..."
*                      iIcon = WU_ICON_{INFO|QUES|WARN|ABORT}
*    HWND WuButton         (hwParent, idControl, szCaption,
*                                     iLeft, iTop, iWidth, iHeight)
*    HWND WuButton_dflt    (hwParent, idControl, szCaption,
*                                     iLeft, iTop, iWidth, iHeight)
*    HWND WuCheckBox       (hwParent, idControl, szCaption,
*                                     iLeft, iTop, iWidth, iHeight)
*     int WuCheckBox_chk   (hwCtrl)
*    void WuCheckBox_set   (hwCtrl, bSet)
*    void WuControlMove    (hwCtrl, iLeft, iTop, iWidth, iHeight)
*    void WuControlShow    (hwCtrl, bShow)
*    void WuDelay          (iSeconds)
*    void WuDirAdd         (szDest, destDim, szDir, szFile)
*    void WuDirStrip       (szDest, destDim, szSource, szDfltDir)
*    void WuDoEvents       ()
*    HWND WuEdit           (hwParent, idControl,
*                                     iLeft, iTop, iWidth, iHeight)
*    HWND WuEditBig        (hwParent, idControl,
*                                     iLeft, iTop, iWidth, iHeight)
*    void WuEditAddLine    (hwEdit, szLine)
*    void WuEditClear      (hwEdit)
*    void WuEditGetText    (hwEdit, szDest, destDim)
*    void WuEditPrintf     (hwEdit, szFmt, ...)
*    void WuEditSetText    (hwEdit, szText)
*    void WuEditSetTextPrintf (hwEdit, szFmt, ...)
*     int WuFileAutonumCheck (szDir, szMask, iBegRun, iEndRun, >pnExist)
*     int WuFileAutonumName (szMask, <>piRun, iBufDim, >szBuf)
*     int WuFileModDateGet (szPath, >lSec)
*     int WuFileModDateSet (szPath, lSec)
*    void WuFileStrip      (szDest, destDim, szSource)
*   HFONT WuFontCreate     (szFaceName, iSize, bItalic,
*                                     bBold, bUnderline, iDegrees)
*    void WuGetCSzFromPsz  (>szDest, destDim, pszText)
* HINSTANCE WuGetHInstance (hwControl)
*    void WuGetWinVersion  (>pfVersion, >pbWin95, >pbWinNT)
*    HWND WuGroupBox       (hwParent, idControl, szCaption,
*                                     iLeft, iTop, iWidth, iHeight)
*    HWND WuLabel          (hwParent, idControl, szCaption,
*                                     iLeft, iTop, iWidth, iHeight)
*    HWND WuListNASz       (hwParent, idControl, naszItems,
*                                     iLeft, iTop, iWidth, iHeight)
*    void WuListNASz_get   (hwCtrl, >szDest, destDim)
*    void WuListNASz_set   (hwCtrl, szText)
*    HWND WuMainWin        (hPrev, idControl, wndProc, nCmdShow, bShow,
*                                     iLeft, iTop, iWidth, iHeight)
*    HWND WuMainWin_noBorder (hPrev, idControl, wndProc, nCmdShow, bShow,
*                                     iLeft, iTop, iWidth, iHeight)
*    HWND WuPlListColor    (hwParent, idControl,
*                                     iLeft, iTop, iWidth, iHeight)
* COLORREF WuPlListColor_get (hwCtrl)
*    void WuPlListColor_set (hwCtrl, rgbSet)
*    HWND WuPlListPattern  (hwParent, idControl,
*                                     iLeft, iTop, iWidth, iHeight)
*     int WuPlListPattern_get (hwCtrl)
*    void WuPlListPattern_set (hwCtrl, ePattern)
*    HWND WuPlListThick    (hwParent, idControl,
*                                     iLeft, iTop, iWidth, iHeight)
*   float WuPlListThick_get (hwCtrl)
*    void WuPlListThick_set (hwCtrl, fPts)
* CALLBACK WuPlotProc      (hWnd, iMsg, wParam, lParam)
*    void WuPlotKeydown    (hwPlot, wParam, lParam)
*    HWND WuPlotWin        (hwParent, szClass, wndProc,
*                                     rgbBkg, rgbFg,
*                                     iLeft, iTop, iWidth, iHeight)
*     int WuPrtText_font   (hDC, szText, xPx, yPx, eRefPt, szFaceName,
*                                     fPtSize, bBold, fDegrees, bNoVSpace,
*                                     bBoundsOnly, pInQuad, pOutQuad)
*     int WuPrtText_font_ROP2 (hDC, szText, xPx, yPx, szFaceName,
*                                     fPtSize, bBold, bVert)
*     int WuQuadClear      (hDC, pQuad)
*     int WuQuadsDoOverlap (pQuad1, pQuad2, fXDist, fYDist)
*     int WuSzCmpWild      (szText, szPattern, bIgnoreCase)
*    void WuSzMCat         (szDest, destDim, szSource)
*    void WuSzMCopy        (szDest, destDim, szSource)
*
* Notes:
* 1.	Visual Basic programs can access many of these functions via
*		WMCA_DEF.BAS.
*
* Date     Author		Revision
* -------- ------------	--------
* 03-01-95 R. Cole		created
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
#include <ctype.h>
#include <io.h>		// for _access()
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utime.h>

#include "pl.h"
#include "wu.h"

#if !defined WIN32
#define FAR16 _far
#define EXPORT16 __export
#define EXPORT32
#else
#define FAR16
#define EXPORT16
#define EXPORT32 __declspec(dllexport)
#endif


#define WUA_WIN_WIDTH 500

#define IDC_WUA_BTN0 0
#define IDC_WUA_BTN1 1
#define IDC_WUA_BTN2 2

typedef struct {
	int		iIcon;
	RECT	rectSubject;
	const char *szSubject;
	RECT	rectMessage;
	const char *szMessage;
	int		nChoice;
	UINT	aidBtn[WU_ALERT_MAX_N_CHOICE];
	char	acAccel[WU_ALERT_MAX_N_CHOICE];
	RECT	arectChoice[WU_ALERT_MAX_N_CHOICE];
	const char *(aszChoice[WU_ALERT_MAX_N_CHOICE]);
} WU_ALERT_INF;

void PASCAL
WuAlert_choice(
HWND	hwParent,		// I I parent window handle
HDC		hDC,			// I DC of parent, for drawing text
int		iChoice,		// I item number
WU_ALERT_INF *alertInfo,// IO array of WU_ALERT_INF items
UINT	idBtn,			// I resource ID of the button
const char *szBtn,		// I caption for button
const char *szChoice,	// I choice text
int		*piYpx,			// IO Y location
int		bDefault)		// I 1 if this is the default choice
{
	char	*pAccel;
	HWND	hwBtn;
	int		iTextHt;

	// Remember the button ID, to use when an event occurs.
	alertInfo->aidBtn[iChoice] = idBtn;
	// Find out if the button caption has an accelerator.  If so, remember it
	// so that when a keystroke is received it can be used to click the
	// appropriate button.
	pAccel = strchr(szBtn, '&');
	if (pAccel != NULL)
		alertInfo->acAccel[iChoice] = *(pAccel+1);
	else
		alertInfo->acAccel[iChoice] = '\0';
	alertInfo->aszChoice[iChoice] = szChoice;
	// Create the button and place it on the alert window.
	if (bDefault)
		hwBtn = WuButton_dflt(hwParent, idBtn, szBtn, 5, *piYpx, 100, 22);
	else
		hwBtn = WuButton(hwParent, idBtn, szBtn, 5, *piYpx, 100, 22);
	// Find out how much space is needed for the text associated with the
	// button, and remember it.
	SetRect(&alertInfo->arectChoice[iChoice], 115, *piYpx, WUA_WIN_WIDTH-20, 10);
	iTextHt = DrawText(hDC, alertInfo->aszChoice[iChoice], -1,
									&alertInfo->arectChoice[iChoice],
									DT_WORDBREAK | DT_CALCRECT | DT_EXPANDTABS);
	// Find the 'bottom' of the button or the text, whichever is lower.
	if (iTextHt > 22)
		*piYpx += iTextHt;
	else
		*piYpx += 22;
	// And create a little space below, as a 'margin' between buttons.
	*piYpx += 5;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* WuAlertProc - handle events for an alert window
*
* Purpose:
*		Handles events for an alert window.
*
* Return Value:
*
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-12-96 R. Cole		created
*--------------------------------------------------------------------------*/
LRESULT EXPORT16 CALLBACK
WuAlertProc(
HWND	hWnd,
UINT	iMsg,
WPARAM	wParam,
LPARAM	lParam)
{
	int		i, iBtn=-1;
	WU_ALERT_INF *alertInfo;
	int		*piExitCode;

	alertInfo = (WU_ALERT_INF *)GetWindowLong(hWnd, 0);
	piExitCode = (int *)GetWindowLong(hWnd, sizeof(DWORD));

	if (iMsg == WM_COMMAND) {
		for (i=0; i<alertInfo->nChoice; i++) {
			if (wParam == (WPARAM)alertInfo->aidBtn[i]) {
				iBtn = i;
				break;
			}
		}
		if (iBtn < 0)
			goto default_proc;
		*piExitCode = iBtn;
		DestroyWindow(hWnd);
	}
	else if (iMsg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC		hDC;

		hDC = BeginPaint(hWnd, &ps);

		if (alertInfo->iIcon == WU_ICON_INFO)
			DrawIcon(hDC, 5, 5, LoadIcon(NULL, IDI_ASTERISK));
		else if (alertInfo->iIcon == WU_ICON_QUES)
			DrawIcon(hDC, 5, 5, LoadIcon(NULL, IDI_QUESTION));
		else if (alertInfo->iIcon == WU_ICON_WARN)
			DrawIcon(hDC, 5, 5, LoadIcon(NULL, IDI_EXCLAMATION));
		else if (alertInfo->iIcon == WU_ICON_ABORT)
			DrawIcon(hDC, 5, 5, LoadIcon(NULL, IDI_HAND));
	
		DrawText(hDC, alertInfo->szSubject, -1, &alertInfo->rectSubject,
										DT_CENTER | DT_WORDBREAK);
		DrawText(hDC, alertInfo->szMessage, -1, &alertInfo->rectMessage,
										DT_WORDBREAK | DT_EXPANDTABS);
		for (i=0; i<alertInfo->nChoice; i++) {
			DrawText(hDC, alertInfo->aszChoice[i], -1,
										&alertInfo->arectChoice[i],
										DT_WORDBREAK | DT_EXPANDTABS);
		}
		EndPaint(hWnd, &ps);
	}
	else if (iMsg == WM_KEYDOWN) {
		if (wParam == VK_RETURN) goto dflt_exit;
		if (wParam == VK_ESCAPE) goto dflt_exit;
		for (i=0; i<alertInfo->nChoice; i++) {
			if (toupper(alertInfo->acAccel[i]) == toupper((char)wParam)) {
				iBtn = i;
				break;
			}
		}
		if (iBtn < 0) {
			MessageBeep(0);
			goto default_proc;
		}
		*piExitCode = iBtn;
		DestroyWindow(hWnd);
	}
	else if (iMsg == WM_SYSCOMMAND) {
		if (wParam == SC_CLOSE) {
dflt_exit:
			*piExitCode = alertInfo->nChoice - 1;
			DestroyWindow(hWnd);
		}
		else
			goto default_proc;
	}
	else
		goto default_proc;
	return 0;
default_proc:
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
/*+/csubr/TOC/internal-------------------------------------------------------
* WuAlertWin - create an alert window
*
* Purpose:
*		Creates an alert window.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-12-96 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuAlertWin(
HWND	hwParent,		// I I parent window handle
WU_ALERT_INF *alertInfo,// I array of button definitions
int		*piExitCode)	// IO place to store exit code
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwAlert=0;
	char	*szClass="WUALERT";
	WNDCLASS alertClass;
	ATOM	atom;
	int		stat;

	*piExitCode = -1;
	stat = GetClassInfo(hInst, szClass, &alertClass);
	if (stat == 0) {  // the class hasn't been registered yet
		alertClass.style		= CS_OWNDC;
		alertClass.lpfnWndProc	= WuAlertProc;
		alertClass.cbClsExtra	= 0;
		alertClass.cbWndExtra	= 2 * sizeof(void *);
		alertClass.hInstance	= hInst;
		alertClass.hIcon		= NULL;
		alertClass.hCursor		= NULL;
		alertClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		alertClass.lpszMenuName	= NULL;
		alertClass.lpszClassName = szClass;
		atom = RegisterClass(&alertClass);
		if (atom == 0)
			goto done;
	}

	hwAlert = CreateWindow(szClass, "", WS_POPUPWINDOW | WS_CAPTION,
						CW_USEDEFAULT, CW_USEDEFAULT, WUA_WIN_WIDTH, 200,
						hwParent, 0, hInst, NULL);
	if (hwAlert == 0)
		goto done;
	SetWindowLong(hwAlert, 0, (DWORD)alertInfo);
	SetWindowLong(hwAlert, sizeof(DWORD), (DWORD)piExitCode);
done:
	return hwAlert;
}

/*+/csubr/TOC----------------------------------------------------------------
* WuAlert - display an alert dialog
*
* Purpose:
*		Display an alert dialog and get the operator's response.
*
*		The primary input to this routine is a text string that is
*		broken into substrings with "|" characters, as suggested by
*		the following code fragment.  The substrings and their effect
*		are discussed below.  The maximum number of choices allowed
*		is WU_ALERT_MAX_N_CHOICE.
*
*		char	*szAlert;
*
*		szAlert = "titleBarText|subjectText|discussionText"
*				"|choice_0_Caption|choice_0_Text"
*				"|choice_1_Caption|choice_1_Text"   ' optional
*				"|choice_2_Caption|choice_2_Text"   ' optional
*
*		o	"titleBarText" appears on the title bar, along with
*		    the program name.  Typically this will be a verb, such
*		    as "File Open".
*		o	"subjectText" appears in the "subject" text box.  This
*		    will usually be a noun, such as the path that was
*		    being acted on by the File Open.
*		o	"discussionText" goes into the "discussion" text box,
*		    to give fuller details of the situation
*		o	"choice_n_Caption" is used as the caption for the button
*		    next to one of the "choice" text boxes.  This caption can
*		    indicate an accelerator character by preceding it with "&".
*		    The choices are numbered from 0 to 2; the first is required
*		    and the others are optional.  Each choice box has an associated
*		    button, with the same number as the choice.  See Note 1.
*		o	"choice_n_Text" is used as the text in one of the "choice"
*		    text boxes.  This describes one of the possible choices
*		    and its ramifications.
*
* Return Value:
*		the number (0 based) of the choice that was invoked, or
*		-1 if bad arguments were supplied
*
* Notes:
* 1.	The last choice is treated as both the default button and the
*		cancel button.  That is, it will be invoked if:
*		o	the user clicks the mouse on the button;
*		o	the user types the button's accelerator;
*		o	the user presses the <escape> key; or
*		o	SYSTEM | CLOSE is invoked.
*		Thus, the caller should carefully consider the order when more than
*		one choice is specified.  The last choice should select the action
*		that will avoid loss of data or other undesirable results.
* 2.	The title bar of the alert dialog will contain the name of the
*		executable followed by the "title" substring.
* 3.	The iIcon argument isone of the WU_ICON_XXX styles:
*		o	WU_ICON_INFO  in order to give the operator information
*			that is noteworthy but that doesn't affect operation
*		o	WU_ICON_QUES  to ask the operator a question that affects
*			the course of execution
*		o	WU_ICON_WARN  when an unusual situation exists that may
*			affect results, but where it may be okay for the operator to
*			continue
*		o	WU_ICON_ABORT  when an unusual situation exists from which no
*			recovery is possible or which clearly makes it inappropriate
*			to continue execution
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-12-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuAlert(
HWND	hwParent,		// I I parent window handle
const char *szAlert,	// I specially formatted alert text
int		iIcon)			// I icon to display--WU_ICON_XXX
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwAlert;
	RECT	rectAlert, rectParent;
	int		iAlertLeft, iAlertTop, iAlertWid, iAlertHt;
	int		iParentWid, iParentHt;
	int		iBtn;
	HDC		hDC;
	WU_ALERT_INF alertInfo;
	int		iChoice;	// choice number
	char	*szTemp=NULL, *pszBase, *pcBar, *psz, *pszCaption;
	int		iYpx=5, iOffset, bDefault=0;
	HCURSOR	hcurSave;

	pszBase = szTemp = (char *)malloc(strlen(szAlert) + 1);
	if (szTemp == NULL)
		return -1;
	strcpy(szTemp, szAlert);

	hwAlert = WuAlertWin(hwParent, &alertInfo, &iChoice);
	if (hwAlert == NULL)
		return -1;
	hDC = GetDC(hwAlert);

#define NEXT_SUBSTRING \
	psz = szTemp; \
	pcBar = strchr(szTemp, '|'); \
	if (pcBar != NULL) { \
		*pcBar = '\0'; \
		szTemp = pcBar + 1; \
	} \
	else \
		szTemp = "";

	alertInfo.iIcon = iIcon;	// icon

	NEXT_SUBSTRING				// titleBarText
	SetWindowText(hwAlert, psz);

	NEXT_SUBSTRING				// subjectText
	alertInfo.szSubject = psz;
	SetRect(&alertInfo.rectSubject, 50, iYpx, WUA_WIN_WIDTH-20, 10);
	iYpx += DrawText(hDC, alertInfo.szSubject, -1, &alertInfo.rectSubject,
								DT_CENTER | DT_WORDBREAK | DT_CALCRECT);
	iOffset = (WUA_WIN_WIDTH - alertInfo.rectSubject.right) / 2;
	alertInfo.rectSubject.left += iOffset;
	alertInfo.rectSubject.right += iOffset;
	if (iYpx < 30) iYpx = 30;

	iYpx += 10;
	NEXT_SUBSTRING				// discussionText
	alertInfo.szMessage = psz;
	SetRect(&alertInfo.rectMessage, 5, iYpx, WUA_WIN_WIDTH-20, 10);
	iYpx += DrawText(hDC, alertInfo.szMessage, -1, &alertInfo.rectMessage,
								DT_WORDBREAK | DT_CALCRECT | DT_EXPANDTABS);

	iYpx += 10;
	alertInfo.nChoice = 0;

	for (iBtn=0; iBtn<WU_ALERT_MAX_N_CHOICE; iBtn++) {
		NEXT_SUBSTRING		// choice_n_Caption
		pszCaption = psz;
		NEXT_SUBSTRING		// choice_n_Text
		if (psz == NULL || *psz == '\0')
			break;
		alertInfo.nChoice++;
		if (*szTemp == '\0')
			bDefault = 1;	// if this is last, make button the default
		WuAlert_choice(hwAlert, hDC, iBtn, &alertInfo,
							iBtn, pszCaption, psz, &iYpx, bDefault);
	}
	ReleaseDC(hwAlert, hDC);
	free(pszBase);

	// Now calculate the size and position of the alert, centering it on the
	// parent if possible.
	GetWindowRect(hwParent, &rectParent);
	GetWindowRect(hwAlert, &rectAlert);
	iAlertLeft = rectAlert.left;
	iAlertTop = rectAlert.top;

	iParentWid = rectParent.right - rectParent.left;
	iAlertWid = WUA_WIN_WIDTH;
	if (iParentWid > iAlertWid)
		iAlertLeft = rectParent.left + (iParentWid - iAlertWid) / 2;

	iParentHt = rectParent.bottom - rectParent.top;
	iAlertHt = iYpx + GetSystemMetrics(SM_CYCAPTION) +
									GetSystemMetrics(SM_CYBORDER);
	if (iParentHt > iAlertHt)
		iAlertTop = rectParent.top + (iParentHt - iAlertHt) / 2;

	// Set the size and position of the alert window, make it visible, and
	// wait for the operator to do something.
	MoveWindow(hwAlert, iAlertLeft, iAlertTop, iAlertWid, iAlertHt, FALSE);
	hcurSave = SetCursor(LoadCursor(NULL, IDC_ARROW));
	ShowWindow(hwAlert, SW_SHOW);
	while (iChoice < 0)
		WuDoEvents();
	hcurSave = SetCursor(hcurSave);
	return iChoice;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuButton - create a button control
*
* Purpose:
*		Creates a button control.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuButton(
HWND	hwParent,		// I I parent window handle
UINT	idControl,		// I resource ID of the control
const char *szCaption,	// I caption for control
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwCtrl;

	hwCtrl = CreateWindow("BUTTON", szCaption,
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
					iLeft, iTop, iWidth, iHeight,
					hwParent, (HANDLE)idControl, hInst, NULL);
	ShowWindow(hwCtrl, SW_SHOW);	// make the window visible
	UpdateWindow(hwCtrl);		// trigger a WM_PAINT message
	return hwCtrl;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuButton_dflt - create a button control as the default
*
* Purpose:
*		Creates a button control as the default.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-12-96 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuButton_dflt(
HWND	hwParent,		// I I parent window handle
UINT	idControl,		// I resource ID of the control
const char *szCaption,	// I caption for control
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwCtrl;

	hwCtrl = CreateWindow("BUTTON", szCaption,
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
					iLeft, iTop, iWidth, iHeight,
					hwParent, (HANDLE)idControl, hInst, NULL);
	ShowWindow(hwCtrl, SW_SHOW);	// make the window visible
	UpdateWindow(hwCtrl);		// trigger a WM_PAINT message
	return hwCtrl;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuCheckBox - create a checkbox control
*
* Purpose:
*		Creates a checkbox control.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuCheckBox(
HWND	hwParent,		// I I parent window handle
UINT	idControl,		// I resource ID of the control
const char *szCaption,	// I caption for control
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwCtrl;

	hwCtrl = CreateWindow("BUTTON", szCaption,
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
					iLeft, iTop, iWidth, iHeight,
					hwParent, (HANDLE)idControl, hInst, NULL);
	ShowWindow(hwCtrl, SW_SHOW);	// make the window visible
	UpdateWindow(hwCtrl);		// trigger a WM_PAINT message
	return hwCtrl;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuCheckBox_chk - check the state of a checkbox control
*
* Purpose:
*		Checks the state of a checkbox control.
*
* Return Value:
*		1 if the checkbox is "checked", or
*		0
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuCheckBox_chk(
HWND	hwCtrl)			// I checkbox handle
{
	DWORD	dwStat;
	int		bSet;

	dwStat = SendMessage(hwCtrl, BM_GETCHECK, 0, 0L);
	bSet = dwStat?1:0;
	return bSet;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuCheckBox_set - alter the state of a checkbox control
*
* Purpose:
*		Alters the state of a checkbox control.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuCheckBox_set(
HWND	hwCtrl,			// I checkbox handle
int		bSet)			// I 1,0 to check,uncheck checkbox
{
	DWORD	dwStat;
	int		bMySet;

	bMySet = bSet?1:0;
	dwStat = SendMessage(hwCtrl, BM_SETCHECK, (WORD)bMySet, 0L);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuClipIt - clip a line to its intersection with a rectangle
*
* Purpose:
*		Calculate the intersection of a line with a rectangle.
*
* Return Value:
*		0 ==> the line segment is not visible.
*		1 ==> no clipping--entire line segment is visible
*		2 ==> the first point is clipped
*		3 ==> the second point is clipped
*		4 ==> both points are clipped
*
* Notes:
* 1.	NOTE WELL!! The right and bottom rect coordinates are the ACTUAL
*		pixel coordinates, NOT 1 past, as with the usual Windows RECT
*		convention.
*
*-Date     Author		Revision
* -------- ------------	--------
* 08-23-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuClipIt(
int		bAlter,		// I 1 to alter the coordinates of the point
int		iL,			// I X coordinate of left side of rect
int		iT,			// I Y coordinate of top side of rect
int		iR,			// I X coordinate of right side of rect
int		iB,			// I Y coordinate of bottom side of rect
double	*pdX1,		// IO X coordinate of first point
double	*pdY1,		// IO Y coordinate of first point
double	*pdX2,		// IO X coordinate of second point
double	*pdY2)		// IO Y coordinate of second point
{
	double	xMin, xMax, yMin, yMax;
	double	x1, y1, x2, y2;
	double	dSlope, dInt;	// slope and Y intercept of segment
	double	dYL, dYR;		// Y intercept of left and right sides
	double	dXB, dXT;		// X intercept of bottom and top sides
	int		nInt;			// number of intersections on rect boundary
	double	xInt[2], yInt[2];// intersection points on rect boundary

	if ((x1=*pdX1) <= (x2=*pdX2))	xMin = x1, xMax = x2;
	else							xMax = x1, xMin = x2;
	if ((y1=*pdY1) <= (y2=*pdY2))	yMin = y1, yMax = y2;
	else							yMax = y1, yMin = y2;

	if (xMin >= iL && xMax <= iR && yMin >= iT && yMax <= iB)
		return 1;	// line segment is totally inside

	if (xMax < iL || xMin > iR) {
		// the segment's projection on top (bottom) is null
		return 0;	// line segment is totally outside
	}
	if (yMax < iT || yMin > iB) {
		// The segment has a projection on top or bottom; return the
		// projection
allOutside:
		if (bAlter) {
			x1 = max(x1, iL); x1 = min(x1, iR);
			x2 = max(x2, iL); x2 = min(x2, iR);
			y1 = max(y1, iT); y1 = min(y1, iB);
			y2 = max(y2, iT); y2 = min(y2, iB);
			*pdX1 = x1; *pdY1 = y1; *pdX2 = x2; *pdY2 = y2;
		}
		return 0;	// line segment is totally outside
	}

	dXB = dXT = dYL = dYR = -1.;
	if (x1 == x2)		// the segment intersects top and/or bottom
		dXT = dXB = x1;
	else if (y1 == y2)	// the segment intersects left and/or right
		dYL = dYR = y1;
	else {
		dSlope = (y2 - y1) / (x2 - x1);
		dInt = y1 - dSlope * x1;
		dYL = dInt + dSlope * iL;
		dYR = dInt + dSlope * iR;
		dXT = (iT - dInt) / dSlope;
		dXB = (iB - dInt) / dSlope;
	}
	nInt = 0;
	if (dYL >= iT && dYL <= iB) {
		// extended line intersects left side; check segment
		if (xMin <= iL && xMax >= iL) {
			xInt[nInt] = iL;
			yInt[nInt] = dYL;
			nInt++;
		}
	}
	if (dYR >= iT && dYR <= iB) {
		// extended line intersects right side; check segment
		if (xMin <= iR && xMax >= iR) {
			xInt[nInt] = iR;
			yInt[nInt] = dYR;
			nInt++;
		}
	}
	if (nInt < 2 && dXT >= iL && dXT <= iR) {
		// extended line intersects top; check segment
		if (yMin <= iT && yMax >= iT) {
			xInt[nInt] = dXT;
			yInt[nInt] = iT;
			nInt++;
		}
	}
	if (nInt < 2 && dXB >= iL && dXB <= iR) {
		// extended line intersects bottom; check segment
		if (yMin <= iB && yMax >= iB) {
			xInt[nInt] = dXB;
			yInt[nInt] = iB;
			nInt++;
		}
	}
	if (nInt == 0)
		goto allOutside;	// line segment is totally outside
	if (nInt == 1) {
		if (x1 >= iL && x1 <= iR && y1 >= iT && y1 <= iB) {
			if (bAlter) {
				*pdX2 = xInt[0]; *pdY2 = yInt[0];
			}
			return 3;	// the second point is clipped
		}
		else {
			if (bAlter) {
				*pdX1 = xInt[0]; *pdY1 = yInt[0];
			}
			return 2;	// the first point is clipped
		}
	}
	if (bAlter) {
		// The line segment goes clean through the rectangle and
		// the caller wants to see the clipped ends.
		double	dist0, dist1;
		int		n, n1;
//fprintf(pFile, "%4d %4d %4d %4d  (caller's values)\n", x1, y1, x2, y2);
//fprintf(pFile, "%4d %4d %4d %4d  (intercepts)\n", xInt[0], yInt[0], xInt[1], yInt[1]);
		dist0 = sqrt((double)(x1-xInt[0])*(double)(x1-xInt[0]) +
							(double)(y1-yInt[0])*(double)(y1-yInt[0]));
		dist1= sqrt((double)(x1-xInt[1])*(double)(x1-xInt[1]) +
							(double)(y1-yInt[1])*(double)(y1-yInt[1]));
		if (dist0 <= dist1)		n = 0, n1 = 1;
		else					n = 1, n1 = 0;
		*pdX1 = xInt[n]; *pdY1 = yInt[n];
		*pdX2 = xInt[n1]; *pdY2 = yInt[n1];
//fprintf(pFile, "   %6.0f %6.0f %4d %4d %4d %4d\n", dist0, dist1, *pdX1, *pdY1, *pdX2, *pdY2);
	}
	return 4;	// both points were clipped
}
/*+/csubr/TOC----------------------------------------------------------------
* WuControlMove - change size/position of a control
*
* Purpose:
*		Changes the size and/or position of a control.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-27-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuControlMove(
HWND	hwCtrl,			// I control handle
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	MoveWindow(hwCtrl, iLeft, iTop, iWidth, iHeight, 1);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuControlShow - change visible property of a control
*
* Purpose:
*		Changes the visible property of a control.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-27-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuControlShow(
HWND	hwCtrl,			// I control handle
int		bShow)			// I 1 to make control visible, else 0
{
	ShowWindow(hwCtrl, bShow ? SW_SHOWNA : SW_HIDE);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuDelay - delay the specified number of seconds
*
* Purpose:
*		Delays the specified number of seconds, processing Windows events
*		in the interim.
*
*		WuDelay monitors the caller's cancel flag, if it is supplied.  If
*		the flag becomes true, then WuDelay returns immediately.
*
* Return Value:
*		void
*
* Notes:
* 1.	Programs that use WuDelay must exercise some care if the operator
*		can exit from the program while a delay is in progress.  In such
*		cases, the exit routine in the program should set the cancel flag
*		for WuDelay before exiting.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-06-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuDelay(
int		iSeconds,		// I the number of seconds to delay
int		*pbCancel)		// I cancel flag, or NULL
{
	time_t	timeBeg, timeEnd;
	time_t	timeDone;

	timeBeg = time(NULL);
	timeDone = timeBeg + (time_t)iSeconds;
	while (1) {
		WuDoEvents();
		timeEnd = time(NULL);
		if (timeEnd >= timeDone)
			break;
		if (pbCancel != NULL && *pbCancel)
			break;
	}
}
/*+/csubr/TOC----------------------------------------------------------------
* WuDirAdd - add a directory in front of a file name
*
* Purpose:
*		Adds a disk/directory specification in front of a file name.
*
* Return Value:
*		void
*
* Notes:
* 1.	If the file specification is an absolute path, it is copied to the
*		destination buffer without adding the disk/directory specification
*		to the front.
* 2.	Only as much of the disk/directory/file as will fit is stored in
*		the destination buffer.
*
*-Date     Author		Revision
* -------- ------------	--------
* 11-13-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuDirAdd(
char	*szDest,		// O place to store text
int		destDim,		// I dimension of szDest
const char *szDir,		// I disk/directory specification, or NULL
const char *szFile)		// I file name
{
	int		iC=0;

	if (szDir == NULL) szDir = "";

	szDest[destDim-1] = '\0';
	if (szFile[0] == '\\') goto absolute_path;
	if (strlen(szFile) >= 2 && szFile[1] == ':') goto absolute_path;
	while (1) {
		// copy szDir.  iC will point to the '\0'
		if (iC >= destDim-1) goto done;
		szDest[iC] = *szDir;
		if (*szDir == '\0') break;
		iC++, szDir++;
	}
	if (iC > 0) {
		// if both szDir and szFile contain something, put in a
		// '\'.  iC will point to the '\0' that follows
		if (szDest[iC] != '\\' && *szFile != '\0') {
			if (iC >= destDim-1) goto done;
			szDest[iC++] = '\\';
			szDest[iC] = '\0';
		}
	}
absolute_path:
	while (1) {
		// copy szFile (covering up the '\0' from above)
		if (iC >= destDim-1) goto done;
		szDest[iC] = *szFile;
		if (*szFile == '\0') break;
		iC++, szFile++;
	}
done:
	return;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuDirStrip - strip the directory from a path
*
* Purpose:
*		Removes the disk/directory part of a path.
*
*		If the optional directory specification is supplied, the
*		disk/directory part of the path is removed only if it matches
*		the optional directory specification.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 11-13-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuDirStrip(
char	*szDest,		// O place to store text
int		destDim,		// I dimension of szDest
const char *szSource,	// I source text
const char *szDfltDir)	// I default directory, or NULL, or ""
{
	const char *pC=NULL;
	int		iLen;

	if (szDfltDir == NULL || strlen(szDfltDir) == 0) {
		pC = strrchr(szSource, '\\');
		if (pC == NULL && szSource[1] == ':')
			pC = szSource + 1;
		if (pC == NULL)		pC = szSource;
		else			pC++;
	}
	else {
		iLen = strlen(szDfltDir);
		if (iLen > 0) {
			if (_strnicmp(szSource, szDfltDir, iLen) == 0) {
				pC = szSource + iLen;
				if (*pC == '\\')
					pC++;
			}
		}
	}
	if (pC == NULL)
		pC = szSource;
	WuSzMCopy(szDest, destDim, pC);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuDoEvents - process Windows events
*
* Purpose:
*		Processes Windows events until the Windows event queue is empty.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-06-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuDoEvents(void)
{
	MSG	msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message != WM_CLOSE) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
/*+/csubr/TOC----------------------------------------------------------------
* WuEdit - create an edit control
*
* Purpose:
*		Creates a single line edit control.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuEdit(
HWND	hwParent,		// I I parent window handle
UINT	idControl,		// I resource ID of the control
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwCtrl;

	hwCtrl = CreateWindow("EDIT", "",
					WS_CHILD | WS_VISIBLE | WS_BORDER |
						ES_AUTOHSCROLL | ES_RIGHT | ES_NOHIDESEL,
					iLeft, iTop, iWidth, iHeight,
					hwParent, (HANDLE)idControl, hInst, NULL);
	ShowWindow(hwCtrl, SW_SHOW);	// make the window visible
	UpdateWindow(hwCtrl);			// trigger a WM_PAINT message
	return hwCtrl;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuEditBig - create a multi-line edit control
*
* Purpose:
*		Creates a multi-line edit control.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
* Notes:
* 1.	Tab stops are set every 3 characters.
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuEditBig(
HWND	hwParent,		// I I parent window handle
UINT	idControl,		// I resource ID of the control
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwCtrl;
	WORD	wTabDist=12;	// 12 dialog units is 3 characters

	hwCtrl = CreateWindow("EDIT", "",
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | WS_VSCROLL |
					ES_AUTOVSCROLL | ES_LEFT | ES_NOHIDESEL | ES_MULTILINE,
				iLeft, iTop, iWidth, iHeight,
				hwParent, (HANDLE)idControl, hInst, NULL);
	SendMessage(hwCtrl, EM_SETTABSTOPS, 1, (DWORD)(&wTabDist));
	ShowWindow(hwCtrl, SW_SHOW);	// make the window visible
	UpdateWindow(hwCtrl);		// trigger a WM_PAINT message
	return hwCtrl;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuEditAddLine - add a line to a multi-line edit control
*
* Purpose:
*		Adds a line to a multi-line edit control.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuEditAddLine(
HWND	hwEdit,
const char *szLine)
{
	SendMessage(hwEdit, EM_SETSEL, 0, MAKELONG(-1,-1));
	SendMessage(hwEdit, EM_REPLACESEL, 0, (LPARAM)szLine);
	SendMessage(hwEdit, EM_SETSEL, 0, MAKELONG(-1,-1));
	SendMessage(hwEdit, EM_REPLACESEL, 0, (LPARAM)((LPSTR)("\r\n")));
}
/*+/csubr/TOC----------------------------------------------------------------
* WuEditClear - clear a multi-line edit control
*
* Purpose:
*		Clears a multi-line edit control.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuEditClear(
HWND	hwEdit)
{
	SendMessage(hwEdit, WM_SETTEXT, 0, (long)(char FAR16 *)"");
}
/*+/csubr/TOC----------------------------------------------------------------
* WuEditGetText - get the text from an edit control
*
* Purpose:
*		Gets the text from an edit control.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 02-25-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuEditGetText(
HWND	hwEdit,
char	*szDest,		// O place to store text
int		destDim)		// I dimension of szDest
{
	SendMessage(hwEdit, WM_GETTEXT, destDim, (LPARAM)szDest);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuEditPrintf - print to an edit control
*
* Purpose:
*		Prints to an edit control.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
void
WuEditPrintf(
HWND	hwEdit,
const char *szFmt,
...)
{
	va_list	marker;
	char	*szBuf;
	char	*pC, *pZ;
	int		stat;

	va_start(marker, szFmt);
	if ((szBuf = (char *)calloc(1000, 1)) == NULL)
		goto done;

	pC = szBuf;
	stat = _vsnprintf(szBuf, 1000, szFmt, marker);
	if (stat > 0) {
		for (pZ=pC; *pZ!='\0'; pZ++) {
			if (*pZ == '\n') {
				*pZ = '\0';
				WuEditAddLine(hwEdit, pC);
				pC = ++pZ;
			}
		}
		if (*pC != '\0') {
			SendMessage(hwEdit, EM_SETSEL, 0, MAKELONG(-1,-1));
			SendMessage(hwEdit, EM_REPLACESEL, 0, (LPARAM)pC);
		}
	}

	free(szBuf);
done:
	va_end(marker);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuEditSetText - set the text in an edit control
*
* Purpose:
*		Sets the text in an edit control.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-06-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuEditSetText(
HWND	hwEdit,
const char *szText)
{
	SendMessage(hwEdit, WM_SETTEXT, 0, (LPARAM)szText);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuEditSetTextPrintf - print to an edit control, erasing previous
*
* Purpose:
*		Prints to an edit control, overwriting the previous contents.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-05-97 R. Cole		created
*--------------------------------------------------------------------------*/
void
WuEditSetTextPrintf(
HWND	hwEdit,
const char *szFmt,
...)
{
	va_list	marker;
	char	*szBuf;
	char	*pC, *pZ;
	int		stat;

	va_start(marker, szFmt);
	if ((szBuf = (char *)calloc(1000, 1)) == NULL)
		goto done;

	SendMessage(hwEdit, WM_SETTEXT, 0, (long)(char FAR16 *)"");

	pC = szBuf;
	stat = _vsnprintf(szBuf, 1000, szFmt, marker);
	if (stat > 0) {
		for (pZ=pC; *pZ!='\0'; pZ++) {
			if (*pZ == '\n') {
				*pZ = '\0';
				WuEditAddLine(hwEdit, pC);
				pC = ++pZ;
			}
		}
		if (*pC != '\0') {
			SendMessage(hwEdit, EM_SETSEL, 0, MAKELONG(-1,-1));
			SendMessage(hwEdit, EM_REPLACESEL, 0, (LPARAM)pC);
		}
	}

	free(szBuf);
done:
	va_end(marker);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuFileAutonumCheck - check the files named by an autonumber mask
*
* Purpose:
*		Checks the existence of files whose names are implied by an
*		autonumber mask and a set of run numbers.
*
*		See the description for WuFileAutonumName for rules on the
*		syntax of the mask.
*
* Return Value:
*		0  if the mask doesn't contain at least one # character, or
*		1  if the mask has bad syntax, or
*		2  if the run number is too big for the run number field
*		3  if all files exist, or
*		4  if none of the files exist, or
*		5  if some of the files exist, or
*
*-Date     Author		Revision
* -------- ------------	--------
* 02-13-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuFileAutonumCheck(
const char *szDir,		// I directory containing the files
const char *szMask,		// I the mask
int		iBegRun,		// I the beginning run number
int		iEndRun,		// I the ending run number
int		*pnExist)		// O the number of files that exist
{
	int		stat;
	char	szPath[200], szName[50];
	int		nExist=0, iRun;

	if (iBegRun > iEndRun) return 1;	// call it a syntax error
	for (iRun=iBegRun; iRun<=iEndRun; ) {
		stat = WuFileAutonumName(szMask, &iRun, 50, szName);
		if (stat != 3) return stat;
		WuDirAdd(szPath, 200, szDir, szName);
		if (_access(szPath, 00) == 0)
			nExist++;
	}
	if (nExist == iEndRun - iBegRun + 1) return 3;	// all exist
	else if (nExist == 0) return 4;					// none exist
	return 5;										// some exist
}
/*+/csubr/TOC----------------------------------------------------------------
* WuFileAutonumName - create a file name using an autonumber mask
*
* Purpose:
*		Creates a file name using an autonumber mask and a run number.
*
*		The autonumber mask has either of the forms suggested by:
*			ccc##cc.ccc
*			ccccccc.c#c
*
*		The mask can contain only one series of # characters, which
*		must not span the . that separates the name and extension.  The
*		series of # characters can be one or more characters long, and
*		can appear anywhere in the name (or extension) field.  I.e.,
*		the series of # characters can be at the beginning, end, or
*		somewhere in the middle of the field.  There must be enough
*		# characters in the series to accomodate the largest run number.
*
* Return Value:
*		0  if the mask doesn't contain at least one # character, or
*		1  if the mask has bad syntax, or
*		2  if the run number is too big for the run number field
*		3  if all is OK
*
*-Date     Author		Revision
* -------- ------------	--------
* 02-13-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuFileAutonumName(
const char *szMask,		// I the mask
int		*piRun,			// IO the run number
int		iBufDim,		// I dimension of szBuf
char	*szBuf)			// O buffer to receive new name
{
	char	*psz, *pSpace, szRun[10];
	int		iLen, nMask=0;

	WuSzMCopy(szBuf, iBufDim, szMask);
	psz = strchr(szBuf, '#');
	if (psz == NULL) return 0;		// mask has no # characters
	*psz = '\0';	// null-terminate the prefix
	if (strchr(szMask, '.') == NULL) return 1;	// mask syntax
	psz = strchr(szMask, '#');
	while (1) {
		nMask++;
		++psz;
		if (*psz != '#') break;
	}
	// We've skipped over the first series of # characters and reached
	// a non-# character.  If there are any #'s in what's left, it's an error.
	if (strchr(psz, '#') != NULL) return 1;	// mask syntax
	// Print the run number using the number of #'s as a minimum field
	// width.  If the result is wider, it's an error.  Replace all the
	// leading spaces with 0's.
	_snprintf(szRun, 9, "%*d", nMask, *piRun);
	szRun[9] = '\0';
	iLen = strlen(szRun);
	if (iLen > nMask) return 2;			// run # too big
	while (1) {
		pSpace = strchr(szRun, ' ');
		if (pSpace == NULL) break;
		*pSpace = '0';
	}
	WuSzMCat(szBuf, iBufDim, szRun);
	WuSzMCat(szBuf, iBufDim, psz);
	(*piRun)++;
	return 3;	// all is OK
}
/*+/csubr/TOC----------------------------------------------------------------
* WuFileModDateGet - get the modification date for a file
*
* Purpose:
*		Gets the modification date for a file.
*
* Return Value:
*		0, or
*		non-zero if an error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-02-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuFileModDateGet(
const char *szPath,		// I path to file
long	*lSec)			// O mod date, sec since Windows epoch start
{
	int		stat;
	struct _stat fileStat;

	stat = _stat(szPath, &fileStat);
	if (stat == 0)
		*lSec = fileStat.st_mtime;
	return stat;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuFileModDateSet - set the modification date for a file
*
* Purpose:
*		Sets the modification date for a file.
*
* Return Value:
*		0, or
*		non-zero if an error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 09-02-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuFileModDateSet(
const char *szPath,		// I path to file
long	lSec)			// I mod date, sec since Windows epoch start
{
	int		stat;
	struct _utimbuf timeBuf;

	timeBuf.actime = 0;
	timeBuf.modtime = lSec;
	stat = _utime(szPath, &timeBuf);
	return stat;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuFileStrip - strip the file/extension from a path
*
* Purpose:
*		Removes the file/extension part of a path.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 11-15-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuFileStrip(
char	*szDest,		// O place to store text
int		destDim,		// I dimension of szDest
const char *szSource)	// I source text
{
	char	*pC;

	WuSzMCopy(szDest, destDim, szSource);
	if ((pC = strrchr(szDest, '\\')) != NULL)
		*pC = '\0';	// remove the backslash
	else if ((pC = strrchr(szDest, ':')) != NULL)
		*(++pC) = '\0';	// keep the :
	else
		szDest[0] = '\0';
}
/*+/csubr/TOC----------------------------------------------------------------
* WuFontCreate - creates a logical font
*
* Purpose:
*		Creates a logical font with the desired characteristics.
*
* Return Value:
*		handle for font, or
*		NULL if error occurs
*
* Notes:
* 1.	The caller is responsible for deleting the font.
* 2.	For an hDC that uses pixels, the 'points' to 'pixel' conversion is:
*			size_px = pt * 20 twip/pt / 1440 twip/inch * px/inch
*		iSize_px = fPtSize * 20.F / 1440.F * GetDeviceCaps(hDC, LOGPIXELSX);
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
HFONT PASCAL
WuFontCreate(
const char *szFaceName,	// I name of "font"
int		iSize,			// I size, in logical units
int		bItalic,		// I 1 to create an italic font
int		bBold,			// I 1 to create a bold font
int		bUnderline,		// I 1 to create an underlined font
int		iDegrees)		// I orientation, in degrees
{
	LOGFONT	lf;

	lf.lfHeight = iSize;
	lf.lfWidth = 0;			// don't care
	lf.lfEscapement = iDegrees * 10;
	lf.lfOrientation = iDegrees * 10;
	lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	lf.lfItalic = bItalic;
	lf.lfUnderline = bUnderline;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_CHARACTER_PRECIS;
	lf.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH || FF_DONTCARE;
	WuSzMCopy(lf.lfFaceName, LF_FACESIZE, szFaceName);

	return CreateFontIndirect(&lf);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuGetCSzFromPsz - retrieve a C string from a pointer
*
* Purpose:
*		Gets the VB string that corresponds to a pointer to a null-
*		terminated C string, where the 'pointer' in VB is actually a Long.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 11-13-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuGetCSzFromPsz(
char	*szDest,		// O place to store text
int		destDim,		// I dimension of szDest
const char *pszText)	// I pointer to source text
{
	WuSzMCopy(szDest, destDim, pszText);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuGetHInstance - get the instance handle of a window
*
* Purpose:
*		Gets the instance handle of a window.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
HINSTANCE PASCAL
WuGetHInstance(
HWND	hwWindow)		// I window handle
{
#if !defined WIN32
	return GetWindowWord(hwWindow, GWW_HINSTANCE);
#else
	return (HINSTANCE)GetWindowLong(hwWindow, GWL_HINSTANCE);
#endif
}
/*+/csubr/TOC----------------------------------------------------------------
* WuGetWinVersion - get Windows version
*
* Purpose:
*		Gets information about the Windows version.
*
* Return Value:
*		void
*
* BUGS:
* 1.	This routine does only a partial job of getting information about
*		Windows version.  It doesn't distinguish between the various
*		versions of WindowsNT.
*
*-Date     Author		Revision
* -------- ------------	--------
* 02-05-97 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuGetWinVersion(
float	*pfVersion,		// O version number
int		*pbWin95,		// O 1 if Windows95
int		*pbWinNT)		// O 1 if WindowsNT
{
	int		iMajor, iMinor;
#if defined WIN32
	OSVERSIONINFO osVerInf;
#else
	DWORD	dwVer;
	DWORD	dwFlags;
#endif

// This routine goes halfway toward allowing a 16-bit application
// that DOESN'T use Win32S to figure out the execution platform.
// The glaring deficiency is that it doesn't distinguish WinNT 4.0 and
// later from previous versions.  In order to fully diagnose the
// platform, it would be necessary to use "generic thunks",which are
// discussed in knowledge base articles Q131371 and Q114341.

#define WF_WINNT 0x4000

	*pbWin95 = *pbWinNT = 0;

#if !defined WIN32
	dwVer = GetVersion();
	dwFlags = GetWinFlags();
	iMajor = LOBYTE(LOWORD(dwVer));
	iMinor = HIBYTE(LOWORD(dwVer));
	*pfVersion = iMajor + iMinor / 100.F;
	if (*pfVersion >= 3.94F)
		*pbWin95 = 1;
	if (dwFlags & WF_WINNT)
		*pbWinNT = 1;
#else
	osVerInf.dwOSVersionInfoSize = sizeof(osVerInf);
	GetVersionEx(&osVerInf);
	iMajor = (int)osVerInf.dwMajorVersion;
	iMinor = (int)osVerInf.dwMinorVersion;
	*pfVersion = iMajor + iMinor / 100.F;
	if (osVerInf.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		*pbWin95 = 1;
	if (osVerInf.dwPlatformId == VER_PLATFORM_WIN32_NT)
		*pbWinNT = 1;
#endif

}
/*+/csubr/TOC----------------------------------------------------------------
* WuGroupBox - create a group box control
*
* Purpose:
*		Creates a group box control.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-27-96 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuGroupBox(
HWND	hwParent,		// I I parent window handle
UINT	idControl,		// I resource ID of the control
const char *szCaption,	// I caption for control
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwCtrl;

	hwCtrl = CreateWindow("BUTTON", szCaption,
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX,
					iLeft, iTop, iWidth, iHeight,
					hwParent, (HANDLE)idControl, hInst, NULL);
	ShowWindow(hwCtrl, SW_SHOW);	// make the window visible
	UpdateWindow(hwCtrl);			// trigger a WM_PAINT message
	return hwCtrl;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuLabel - create a label control
*
* Purpose:
*		Creates a label control.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuLabel(
HWND	hwParent,		// I I parent window handle
UINT	idControl,		// I resource ID of the control
const char *szCaption,	// I caption for control
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwCtrl;

	hwCtrl = CreateWindow("STATIC", szCaption,
					WS_CHILD | WS_VISIBLE,
					iLeft, iTop, iWidth, iHeight,
					hwParent, (HANDLE)idControl, hInst, NULL);
	ShowWindow(hwCtrl, SW_SHOW);	// make the window visible
	UpdateWindow(hwCtrl);		// trigger a WM_PAINT message
	return hwCtrl;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuListNASz - create a text listbox
*
* Purpose:
*		Creates a text dropdown listbox using a NULL-terminated array of
*		pointers to '\0'-terminated strings.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
* Notes:
* 1.	The 'iHeight' item specifies the height of the dropdown list, NOT
*		the visible height of the textbox part of the control.  Windows
*		determines the height of the textbox part of the control.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-17-96 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuListNASz(
HWND	hwParent,		// I I parent window handle
UINT	idControl,		// I resource ID of the control
const char *naszItems[],// I NULL terminated array of sz items
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwCtrl;
	int		iItem;

	hwCtrl = CreateWindow("COMBOBOX", "pattern",
					CBS_DROPDOWNLIST | CBS_HASSTRINGS |
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL,
					iLeft, iTop, iWidth, iHeight,
					hwParent, (HANDLE)idControl, hInst, NULL);
	SendMessage(hwCtrl, CB_RESETCONTENT, 0, 0L);
	for (iItem=0; naszItems[iItem]!=NULL; iItem++)
		SendMessage(hwCtrl, CB_ADDSTRING, 0, (DWORD)naszItems[iItem]);
	SendMessage(hwCtrl, CB_SETCURSEL, 0, 0L);
	ShowWindow(hwCtrl, SW_SHOW);	// make the window visible
	return hwCtrl;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuListNASz_get - get the current text from a text listbox
*
* Purpose:
*		Gets the text from the currently selected item of a text listbox.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-17-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuListNASz_get(
HWND	hwCtrl,			// I text listbox handle
char	*szDest,		// O place to store text
int		destDim)		// I dimension of szDest
{
	DWORD	dwStat, dwNChar;
	char	*pszBuf=NULL;

	*szDest = '\0';
	if ((dwStat = SendMessage(hwCtrl, CB_GETCURSEL, 0, 0L)) < 0L) goto done;
	dwNChar = SendMessage(hwCtrl, CB_GETLBTEXTLEN, (WORD)dwStat, 0L);
	if (dwNChar <= 0) goto done;
	pszBuf = (char *)malloc(((int)dwNChar + 1) * sizeof(char));
	if (pszBuf == NULL) goto done;
	dwStat = SendMessage(hwCtrl, CB_GETLBTEXT, (WORD)dwStat, (DWORD)pszBuf);
	if (dwStat <= 0 || dwStat > dwNChar) goto done;
	pszBuf[dwStat] = '\0';
	WuSzMCopy(szDest, destDim, pszBuf);
done:
	if (pszBuf != NULL) free(pszBuf);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuListNASz_set - set the current selection for a text listbox
*
* Purpose:
*		Sets the current selection for a text listbox to match the caller's
*		text.
*
* Return Value:
*		void
*
* Notes:
* 1.	If the caller's text doesn't match one of the items in the listbox,
*		then the current selection isn't changed.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-17-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuListNASz_set(
HWND	hwCtrl,			// I text listbox handle
const char *szText)		// I desired text selection
{
	DWORD	dwStat;

	dwStat = SendMessage(hwCtrl, CB_FINDSTRING, 0, (DWORD)szText);
	if (dwStat >= 0L)
		SendMessage(hwCtrl, CB_SETCURSEL, (WORD)dwStat, 0L);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuMainWin - create a "main" window
*
* Purpose:
*		Creates a "main" window.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
* Notes:
* 1.	If the number of control in the window is great, it may be desirable
*		to defer showing the window until after all the controls have been
*		created.  To accomplish this, set 'bShow' to 0 and then, when the
*		setup for the window is complete, execute:
*				ShowWindow(hwMain, nCmdShow);	// make the window visible
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuMainWin(
HINSTANCE hInst,		// I handle to this instance
HINSTANCE hPrev,		// I handle to previous instance
const char *szAppName,	// I application name, for menu, class, etc.
WNDPROC	wndProc,		// I pointer to window procedure
int		nCmdShow,		// I how to show window--SW_SHOW...
int		bShow,			// I 1 to show the window now
int		iLeft,			// I coordinate of left side of window
int		iTop,			// I I coordinate of top side of window
int		iWidth,			// I width of window
int		iHeight)		// I height of window
{
	WNDCLASS wndClass;
	HWND	hwMain;

	if (!hPrev) {
		wndClass.style			= CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc	= wndProc;
		wndClass.cbClsExtra		= 0;
		wndClass.cbWndExtra		= 0;
		wndClass.hInstance		= hInst;
		wndClass.hIcon			= 0;
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wndClass.lpszMenuName	= szAppName;
		wndClass.lpszClassName	= szAppName;
		if (!RegisterClass(&wndClass))
			return 0;
	}
	hwMain = CreateWindow(
		szAppName,			// class name
		szAppName,			// window name
		WS_OVERLAPPEDWINDOW, // window style
		iLeft,				// x position on screen
		iTop,				// I y position on screen
		iWidth,				// width of window
		iHeight,			// height of window
		NULL,				// I parent window handle (NULL means "none")
		NULL,				// I menu handle (NULL means "use class menu")
		hInst,				// APP instance handle
		NULL);
	if (bShow)
		ShowWindow(hwMain, nCmdShow);	// make the window visible
	return hwMain;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuMainWin_noBorder - create a "main" window without borders
*
* Purpose:
*		Creates a "main" window without borders, title bar, etc.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
* Notes:
* 1.	If the number of control in the window is great, it may be desirable
*		to defer showing the window until after all the controls have been
*		created.  To accomplish this, set 'bShow' to 0 and then, when the
*		setup for the window is complete, execute:
*				ShowWindow(hwMain, nCmdShow);	// make the window visible
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuMainWin_noBorder(
HINSTANCE hInst,		// I handle to this instance
HINSTANCE hPrev,		// I handle to previous instance
const char *szAppName,	// I application name, for menu, class, etc.
WNDPROC	wndProc,		// I pointer to window procedure
int		nCmdShow,		// I I how to show window--SW_SHOW...
int		bShow,			// I 1 to show the window now
int		iLeft,			// I coordinate of left side of window
int		iTop,			// I I coordinate of top side of window
int		iWidth,			// I width of window
int		iHeight)		// I height of window
{
	WNDCLASS wndClass;
	HWND	hwMain;

	if (!hPrev) {
		wndClass.style			= CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc	= wndProc;
		wndClass.cbClsExtra		= 0;
		wndClass.cbWndExtra		= 0;
		wndClass.hInstance		= hInst;
		wndClass.hIcon			= 0;
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wndClass.lpszMenuName	= szAppName;
		wndClass.lpszClassName	= szAppName;
		if (!RegisterClass(&wndClass))
			return 0;
	}
	hwMain = CreateWindow(
		szAppName,		// class name
		szAppName,		// window name
		WS_POPUP,		// window style
		iLeft,			// x position on screen
		iTop,			// I y position on screen
		iWidth,			// width of window
		iHeight,		// height of window
		NULL,			// I parent window handle (NULL means "none")
		NULL,			// I menu handle (NULL means "use class menu")
		hInst,			// APP instance handle
		NULL);
	if (bShow)
		ShowWindow(hwMain, nCmdShow);	// make the window visible
	return hwMain;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuPlListColor - create a dropdown list control for colors
*
* Purpose:
*		Creates a combo box control as a dropdown list, with the edit
*		area being a display of the current color.  The list shows the
*		PL.LIB colors.
*
* Usage:
*		This is a quick-and-dirty routine, and it requires some assistance
*		from the caller:
*		o	create the list:
*				hwColorList = WuPlListColor(hwMain, IDC_COLOR_LIST,...)
*		o	in the event loop (WndProc), add:
*			else if (iMsg == WM_DRAWITEM) {
*				LPDRAWITEMSTRUCT lpDIS=(LPDRAWITEMSTRUCT)lParam;
*				if (lpDIS->CtlID == IDC_COLOR_LIST) {
*					retVal = WuPlListColor_event(hWnd,iMsg,wParam,lParam);
*					return retVal;
*				}
*				else
*					return DefWindowProc(hWnd, iMsg, wParam, lParam);
*			}
*			else if (iMsg == WM_MEASUREITEM) {
*				LPMEASUREITEMSTRUCT lpMIS=(LPMEASUREITEMSTRUCT)lParam;
*				if (lpMIS->CtlID == IDC_COLOR_LIST) {
*					retVal = WuPlListColor_event(hWnd,iMsg,wParam,lParam);
*					return retVal;
*				}
*				else
*					return DefWindowProc(hWnd, iMsg, wParam, lParam);
*			}
*		o	if you're using several WuPlListXxx routines, the fragments
*			above will have to be expanded so that each control is
*			associated with the proper WuPlListXxx_event routine.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-11-96 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuPlListColor(
HWND	hwParent,		// I I parent window handle
UINT	idControl,		// I resource ID of the control
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwCtrl;

	hwCtrl = CreateWindow("COMBOBOX", "color",
					CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED |
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL,
					iLeft, iTop, iWidth, iHeight,
					hwParent, (HANDLE)idControl, hInst, NULL);
	SendMessage(hwCtrl, CB_RESETCONTENT, 0, 0L);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, WHITE);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, GRAY);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, GRAY_DARK);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, BLACK);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, RED);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, RED_DARK);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, YELLOW);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, GOLD);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, GREEN);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, GREEN_DARK);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, CYAN);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, BLUE);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, BLUE_DARK);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, BLUE_GREEN);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, MAGENTA);
	SendMessage(hwCtrl, CB_ADDSTRING, 0, MAGENTA_DARK);
	SendMessage(hwCtrl, CB_SETCURSEL, 4, 0L);
	ShowWindow(hwCtrl, SW_SHOW);	// make the window visible
	return hwCtrl;
}
LRESULT PASCAL
WuPlListColor_event(
HWND	hWnd,
UINT	iMsg,
WPARAM	wParam,
LPARAM	lParam)
{
	LPDRAWITEMSTRUCT lpDIS;
	LPMEASUREITEMSTRUCT lpMIS;
	int		retVal=0;		// 0 says "I didn't process it"
	HBRUSH	hBrush;
	RECT	rcSmaller;

	if (iMsg == WM_DRAWITEM) {
		lpDIS = (LPDRAWITEMSTRUCT)lParam;
		switch (lpDIS->itemAction) {
			case ODA_DRAWENTIRE:
				hBrush = CreateSolidBrush(lpDIS->itemData);
				CopyRect((LPRECT)&rcSmaller, (LPRECT)&lpDIS->rcItem);
				InflateRect((LPRECT)&rcSmaller, -2, -2);
				FillRect(lpDIS->hDC, (LPRECT)&rcSmaller, hBrush);
				DeleteObject(hBrush);
				break;
			case ODA_SELECT:
				if (lpDIS->itemState & ODS_SELECTED)
					hBrush = GetStockObject(BLACK_BRUSH);
				else
					hBrush = GetStockObject(WHITE_BRUSH);
				FrameRect(lpDIS->hDC, (LPRECT)&lpDIS->rcItem, hBrush);
				DeleteObject(hBrush);
				break;
			default:
				return DefWindowProc(hWnd, iMsg, wParam, lParam);
		}
	}
	else if (iMsg == WM_MEASUREITEM) {
		lpMIS = (LPMEASUREITEMSTRUCT)lParam;
		if (lpMIS->itemID == -1)
			lpMIS->itemHeight = 25;		// height of top edit control
		else
			lpMIS->itemHeight = 20;		// height of list item
		retVal = 1;						// 1 says "I processed it"
	}
	else
		return DefWindowProc(hWnd, iMsg, wParam, lParam);

	return retVal;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuPlListColor_get - get the current color from a color listbox
*
* Purpose:
*		Gets the 'current color' from a color listbox.
*
* Return Value:
*		rgb value
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-12-96 R. Cole		created
*--------------------------------------------------------------------------*/
COLORREF PASCAL
WuPlListColor_get(
HWND	hwCtrl)			// I checkbox handle
{
	DWORD	dwStat;
	COLORREF rgbSel=RED;

	dwStat = SendMessage(hwCtrl, CB_GETCURSEL, 0, 0L);
	if (dwStat >= 0L)
		rgbSel = SendMessage(hwCtrl, CB_GETITEMDATA, (WORD)dwStat, 0L);
	return rgbSel;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuPlListColor_set - set the color for a color listbox
*
* Purpose:
*		Sets the 'current color' in a color listbox.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-12-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuPlListColor_set(
HWND	hwCtrl,			// I checkbox handle
COLORREF rgbSet)		// I color to set in color listbox
{
	DWORD	dwStat;

	dwStat = SendMessage(hwCtrl, CB_FINDSTRING, 0, rgbSet);
	if (dwStat >= 0L)
		SendMessage(hwCtrl, CB_SETCURSEL, (WORD)dwStat, 0L);
}
#define WU_PL_PATTERN_VALUES \
		PL_LINE, PL_LINE_DASH, PL_LINE_DOT, PL_LINE_DASHDOT, PL_LINE_DASHDOTDOT
/*+/csubr/TOC----------------------------------------------------------------
* WuPlListPattern - create a dropdown list control for line patterns
*
* Purpose:
*		Creates a combo box control as a dropdown list, with the edit
*		area being a display of the current line pattern.  The list shows
*		the PL.LIB patterns (PL_LINE_XXX).
*
* Usage:
*		This is a quick-and-dirty routine, and it requires some assistance
*		from the caller:
*		o	create the list:
*				hwPatternList = WuPlListPattern(hwMain, IDC_PATTERN_LIST,...)
*		o	in the event loop (WndProc), add:
*			else if (iMsg == WM_DRAWITEM) {
*				LPDRAWITEMSTRUCT lpDIS=(LPDRAWITEMSTRUCT)lParam;
*				if (lpDIS->CtlID == IDC_PATTERN_LIST) {
*					retVal = WuPlListPattern_event(hWnd,iMsg,wParam,lParam);
*					return retVal;
*				}
*				else
*					return DefWindowProc(hWnd, iMsg, wParam, lParam);
*			}
*			else if (iMsg == WM_MEASUREITEM) {
*				LPMEASUREITEMSTRUCT lpMIS=(LPMEASUREITEMSTRUCT)lParam;
*				if (lpMIS->CtlID == IDC_PATTERN_LIST) {
*					retVal = WuPlListPattern_event(hWnd,iMsg,wParam,lParam);
*					return retVal;
*				}
*				else
*					return DefWindowProc(hWnd, iMsg, wParam, lParam);
*			}
*		o	if you're using several WuPlListXxx routines, the fragments
*			above will have to be expanded so that each control is
*			associated with the proper WuPlListXxx_event routine.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-11-96 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuPlListPattern(
HWND	hwParent,		// I I parent window handle
UINT	idControl,		// I resource ID of the control
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwCtrl;
	long	alPattern[]={WU_PL_PATTERN_VALUES};
	int		iPattern, nPattern=sizeof(alPattern)/sizeof(long);

	hwCtrl = CreateWindow("COMBOBOX", "pattern",
					CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED |
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL,
					iLeft, iTop, iWidth, iHeight,
					hwParent, (HANDLE)idControl, hInst, NULL);
	SendMessage(hwCtrl, CB_RESETCONTENT, 0, 0L);
	for (iPattern=0; iPattern<nPattern; iPattern++)
		SendMessage(hwCtrl, CB_ADDSTRING, 0, alPattern[iPattern]);
	SendMessage(hwCtrl, CB_SETCURSEL, 0, 0L);
	ShowWindow(hwCtrl, SW_SHOW);	// make the window visible
	return hwCtrl;
}
LRESULT PASCAL
WuPlListPattern_event(
HWND	hWnd,
UINT	iMsg,
WPARAM	wParam,
LPARAM	lParam)
{
	LPDRAWITEMSTRUCT lpDIS;
	LPMEASUREITEMSTRUCT lpMIS;
	int		retVal=0;		// 0 says "I didn't process it"
	HPEN	hPen, hPenOld;
	HBRUSH	hBrush;
	RECT	rcSmaller;
	long	ePattern;
	int		iLPx, iRPx, iYPx;

	if (iMsg == WM_DRAWITEM) {
		lpDIS = (LPDRAWITEMSTRUCT)lParam;
		switch (lpDIS->itemAction) {
			case ODA_DRAWENTIRE:
				ePattern = lpDIS->itemData;
				switch (ePattern) {
					case PL_LINE_DASH:		ePattern = PS_DASH; break;
					case PL_LINE_DOT:		ePattern = PS_DOT; break;
					case PL_LINE_DASHDOT:	ePattern = PS_DASHDOT; break;
					case PL_LINE_DASHDOTDOT:ePattern = PS_DASHDOTDOT; break;
					default:				ePattern = PS_SOLID; break;
				}
				hPen = CreatePen((int)ePattern, 1, BLACK);
				hPenOld = SelectObject(lpDIS->hDC, hPen);
				CopyRect((LPRECT)&rcSmaller, (LPRECT)&lpDIS->rcItem);
				InflateRect((LPRECT)&rcSmaller, -2, -2);
				iLPx = rcSmaller.left;
				iRPx = rcSmaller.right;
				iYPx = (rcSmaller.top + rcSmaller.bottom) / 2;
				MoveToEx(lpDIS->hDC, iLPx, iYPx, NULL);
				LineTo(lpDIS->hDC, iRPx, iYPx);
				SelectObject(lpDIS->hDC, hPenOld);
				DeleteObject(hPen);
				break;
			case ODA_SELECT:
				if (lpDIS->itemState & ODS_SELECTED)
					hBrush = GetStockObject(BLACK_BRUSH);
				else
					hBrush = GetStockObject(WHITE_BRUSH);
				FrameRect(lpDIS->hDC, (LPRECT)&lpDIS->rcItem, hBrush);
				DeleteObject(hBrush);
			default:
				return DefWindowProc(hWnd, iMsg, wParam, lParam);
		}
	}
	else if (iMsg == WM_MEASUREITEM) {
		lpMIS = (LPMEASUREITEMSTRUCT)lParam;
		if (lpMIS->itemID == -1)
			lpMIS->itemHeight = 25;		// height of top edit control
		else
			lpMIS->itemHeight = 20;		// height of list item
		retVal = 1;						// 1 says "I processed it"
	}
	else
		return DefWindowProc(hWnd, iMsg, wParam, lParam);

	return retVal;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuPlListPattern_get - get the current value from a line pattern listbox
*
* Purpose:
*		Gets the 'current pattern' from a line pattern listbox.
*
* Return Value:
*		line pattern, as PL_LINE_XXX
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-12-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuPlListPattern_get(
HWND	hwCtrl)			// I checkbox handle
{
	DWORD	dwStat;
	long	ePattern;

	dwStat = SendMessage(hwCtrl, CB_GETCURSEL, 0, 0L);
	if (dwStat >= 0L)
		ePattern = SendMessage(hwCtrl, CB_GETITEMDATA, (WORD)dwStat, 0L);
	return (int)ePattern;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuPlListPattern_set - set the value for a line pattern listbox
*
* Purpose:
*		Sets the 'current pattern' in a line pattern listbox.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-12-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuPlListPattern_set(
HWND	hwCtrl,			// I checkbox handle
int		ePattern)		// I line pattern--PL_LINE_XXX
{
	DWORD	dwStat;
	long	alPattern[]={WU_PL_PATTERN_VALUES};
	int		iPattern, nPattern=sizeof(alPattern)/sizeof(long);
	long	lSetPat=alPattern[0];

	for (iPattern=0; iPattern<nPattern; iPattern++) {
		if (alPattern[iPattern] == ePattern) {
			lSetPat = alPattern[iPattern];
			break;
		}
	}

	dwStat = SendMessage(hwCtrl, CB_FINDSTRING, 0, lSetPat);
	if (dwStat >= 0L)
		SendMessage(hwCtrl, CB_SETCURSEL, (WORD)dwStat, 0L);
}
#define WU_PL_THICK_VALUES .1F, .5F, 1.F, 2.F, 3.F, 4.F
/*+/csubr/TOC----------------------------------------------------------------
* WuPlListThick - create a dropdown list control for line thickness
*
* Purpose:
*		Creates a combo box control as a dropdown list, with the edit
*		area being a display of the current thickness.  The list shows
*		a few of the possible PL.LIB line thicknesses.
*
* Usage:
*		This is a quick-and-dirty routine, and it requires some assistance
*		from the caller:
*		o	create the list:
*				hwThicknessList = WuPlListThick(hwMain, IDC_THICK_LIST,...)
*		o	in the event loop (WndProc), add:
*			else if (iMsg == WM_DRAWITEM) {
*				LPDRAWITEMSTRUCT lpDIS=(LPDRAWITEMSTRUCT)lParam;
*				if (lpDIS->CtlID == IDC_THICK_LIST) {
*					retVal = WuPlListThick_event(hWnd,iMsg,wParam,lParam);
*					return retVal;
*				}
*				else
*					return DefWindowProc(hWnd, iMsg, wParam, lParam);
*			}
*			else if (iMsg == WM_MEASUREITEM) {
*				LPMEASUREITEMSTRUCT lpMIS=(LPMEASUREITEMSTRUCT)lParam;
*				if (lpMIS->CtlID == IDC_THICK_LIST) {
*					retVal = WuPlListThick_event(hWnd,iMsg,wParam,lParam);
*					return retVal;
*				}
*				else
*					return DefWindowProc(hWnd, iMsg, wParam, lParam);
*			}
*		o	if you're using several WuPlListXxx routines, the fragments
*			above will have to be expanded so that each control is
*			associated with the proper WuPlListXxx_event routine.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-11-96 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuPlListThick(
HWND	hwParent,		// I I parent window handle
UINT	idControl,		// I resource ID of the control
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwCtrl;
	float	afThick[]={WU_PL_THICK_VALUES};
	int		nThick=sizeof(afThick)/sizeof(float);
	long	lThick;

	hwCtrl = CreateWindow("COMBOBOX", "thickness",
					CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED |
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL,
					iLeft, iTop, iWidth, iHeight,
					hwParent, (HANDLE)idControl, hInst, NULL);
	SendMessage(hwCtrl, CB_RESETCONTENT, 0, 0L);
	for (lThick=0; lThick<nThick; lThick++)
		SendMessage(hwCtrl, CB_ADDSTRING, 0, lThick);
	SendMessage(hwCtrl, CB_SETCURSEL, 0, 0L);
	ShowWindow(hwCtrl, SW_SHOW);	// make the window visible
	return hwCtrl;
}
LRESULT PASCAL
WuPlListThick_event(
HWND	hWnd,
UINT	iMsg,
WPARAM	wParam,
LPARAM	lParam)
{
	LPDRAWITEMSTRUCT lpDIS;
	LPMEASUREITEMSTRUCT lpMIS;
	int		retVal=0;		// 0 says "I didn't process it"
	HPEN	hPen, hPenOld;
	HBRUSH	hBrush;
	RECT	rcSmaller;
	// I couldn't figure out why, but I had to make the following static
	// to avoid a floating point fault.  Roger Cole, 12-12-96
	static float afThick[]={WU_PL_THICK_VALUES};
	int		pxPerX_in, pxPerY_in, iThick;
	int		iLPx, iRPx, iYPx;
	float	fTwipsPerPixX, fTwipsPerPixY, fTwipsPerPix, fPts;

	if (iMsg == WM_DRAWITEM) {
		lpDIS = (LPDRAWITEMSTRUCT)lParam;
		switch (lpDIS->itemAction) {
			case ODA_DRAWENTIRE:
				pxPerX_in = GetDeviceCaps(lpDIS->hDC, LOGPIXELSX);
				fTwipsPerPixX = 1440.F / pxPerX_in;
				pxPerY_in = GetDeviceCaps(lpDIS->hDC, LOGPIXELSY);
				fTwipsPerPixY = 1440.F / pxPerY_in;
				fTwipsPerPix = .5F * (fTwipsPerPixX + fTwipsPerPixY);
				fPts = afThick[lpDIS->itemData];
				iThick = (int)ceil(20.F * fPts / fTwipsPerPix);
				if (iThick < 1) iThick = 1;
				hPen = CreatePen(PS_SOLID, iThick, BLACK);
				hPenOld = SelectObject(lpDIS->hDC, hPen);
				CopyRect((LPRECT)&rcSmaller, (LPRECT)&lpDIS->rcItem);
				InflateRect((LPRECT)&rcSmaller, -2, -2);
				iLPx = rcSmaller.left;
				iRPx = rcSmaller.right;
				iYPx = (rcSmaller.top + rcSmaller.bottom) / 2;
				MoveToEx(lpDIS->hDC, iLPx, iYPx, NULL);
				LineTo(lpDIS->hDC, iRPx, iYPx);
				SelectObject(lpDIS->hDC, hPenOld);
				DeleteObject(hPen);
				retVal = 1;						// 1 says "I processed it"
				break;
			case ODA_SELECT:
				if (lpDIS->itemState & ODS_SELECTED)
					hBrush = GetStockObject(BLACK_BRUSH);
				else
					hBrush = GetStockObject(WHITE_BRUSH);
				FrameRect(lpDIS->hDC, (LPRECT)&lpDIS->rcItem, hBrush);
				DeleteObject(hBrush);
				retVal = 1;						// 1 says "I processed it"
				break;
			default:
				return DefWindowProc(hWnd, iMsg, wParam, lParam);
		}
	}
	else if (iMsg == WM_MEASUREITEM) {
		lpMIS = (LPMEASUREITEMSTRUCT)lParam;
		if (lpMIS->itemID == -1)
			lpMIS->itemHeight = 25;		// height of top edit control
		else
			lpMIS->itemHeight = 20;		// height of list item
		retVal = 1;						// 1 says "I processed it"
	}
	else
		return DefWindowProc(hWnd, iMsg, wParam, lParam);

	return retVal;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuPlListThick_get - get the current value from a line thickness listbox
*
* Purpose:
*		Gets the 'current thickness' from a line thickness listbox.
*
* Return Value:
*		thickness, in printer points
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-12-96 R. Cole		created
*--------------------------------------------------------------------------*/
float PASCAL
WuPlListThick_get(
HWND	hwCtrl)			// I checkbox handle
{
	DWORD	dwStat;
	float	afThick[]={WU_PL_THICK_VALUES};
	long	lThick;

	dwStat = SendMessage(hwCtrl, CB_GETCURSEL, 0, 0L);
	if (dwStat >= 0L)
		lThick = SendMessage(hwCtrl, CB_GETITEMDATA, (WORD)dwStat, 0L);
	return afThick[lThick];
}
/*+/csubr/TOC----------------------------------------------------------------
* WuPlListThick_set - set the value for a line thickness listbox
*
* Purpose:
*		Sets the 'current thickness' in a line thickness listbox.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-12-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuPlListThick_set(
HWND	hwCtrl,			// I checkbox handle
float	fPts)			// I thickness to set in listbox, in printer points
{
	DWORD	dwStat;
	float	afThick[]={WU_PL_THICK_VALUES};
	int		iThick, nThick=sizeof(afThick)/sizeof(float);
	long	lThickSet=0;

	for (iThick=nThick-1; iThick>=0; iThick--) {
		if (afThick[iThick] <= fPts) {
			lThickSet = iThick;
			break;
		}
	}

	dwStat = SendMessage(hwCtrl, CB_FINDSTRING, 0, lThickSet);
	if (dwStat >= 0L)
		SendMessage(hwCtrl, CB_SETCURSEL, (WORD)dwStat, 0L);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuPlotProc - handle events for a plot window
*
* Purpose:
*		Handles events for a plot window.
*
* Return Value:
*		
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
LRESULT EXPORT16  CALLBACK
WuPlotProc(
HWND	hWnd,
UINT	iMsg,
WPARAM	wParam,
LPARAM	lParam)
{
	HDC		hDC;
	PAINTSTRUCT ps;
	int		x, y;
	char	szMsg[100];

	if (iMsg == WM_ERASEBKGND) {
		HBRUSH	hBrush, hBrushOld;
		HPEN	hPen, hPenOld;
		RECT	rect;
		GetClientRect(hWnd, &rect);
		hDC = GetDC(hWnd);
		hBrush = CreateSolidBrush(GetBkColor(hDC));
		hBrushOld = SelectObject(hDC, hBrush);
		hPen = CreatePen(PS_SOLID, 1, GetBkColor(hDC));
		hPenOld = SelectObject(hDC, hPen);
		Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(hDC, hBrushOld);
		DeleteObject(hBrush);
		SelectObject(hDC, hPenOld);
		DeleteObject(hPen);
		ReleaseDC(hWnd, hDC);
		return 1;
	}
	else if (iMsg == WM_HSCROLL) {
		iMsg = iMsg;
		goto default_proc;
	}
	else if (iMsg == WM_VSCROLL) {
		iMsg = iMsg;
		goto default_proc;
	}
	else if (iMsg == WM_PAINT) {
		HPEN	hPen, hPenOld;
		hDC = BeginPaint(hWnd, &ps);
		hPen = CreatePen(PS_SOLID, 1, GetTextColor(hDC));
		hPenOld = SelectObject(hDC, hPen);
		MoveToEx(hDC, ps.rcPaint.left+1, ps.rcPaint.top+1, NULL);
		LineTo(hDC, ps.rcPaint.right-1, ps.rcPaint.bottom-1);
		SelectObject(hDC, hPenOld);
		DeleteObject(hPen);
		EndPaint(hWnd, &ps);
	}
	else if (iMsg == WM_LBUTTONDOWN) {
		if (GetFocus() != hWnd)
			SetFocus(hWnd);
		else {
		}
	}
	else if (iMsg == WM_MOUSEMOVE) {
		hDC = GetDC(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		sprintf(szMsg, "x=%d", x);
		TextOut(hDC, 5, 5, szMsg, strlen(szMsg));
		sprintf(szMsg, "y=%d", y);
		TextOut(hDC, 5, 30, szMsg, strlen(szMsg));
		ReleaseDC(hWnd, hDC);
	}
	else if (iMsg == WM_KEYDOWN) {
		hDC = GetDC(hWnd);
		sprintf(szMsg, "key=0x%x", wParam);
		TextOut(hDC, 5, 5, szMsg, strlen(szMsg));
		ReleaseDC(hWnd, hDC);
	}
	else
		goto default_proc;
	return 0;
default_proc:
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
/*+/csubr/TOC----------------------------------------------------------------
* WuPlotKeydown - handle keyboard events for a plot window
*
* Purpose:
*		Handles keyboard events for a plot window.
*
* Return Value:
*		
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuPlotKeydown(
HWND	hwPlot,
WPARAM	wParam,
LPARAM	lParam)
{
	UINT	msg=0;
	WORD	wP=0;
	int		shift=GetKeyState(VK_SHIFT) & 0x8000;
	if (wParam == VK_RIGHT)
		;
	else if (wParam == VK_LEFT)
		;
	else if (wParam == VK_DOWN)
		;
	else if (wParam == VK_UP)
		;
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
	if (msg != 0) {
		PostMessage(hwPlot, msg, wP, 0);
	}
}
/*+/csubr/TOC----------------------------------------------------------------
* WuPlotWin - create a window for plotting
*
* Purpose:
*		Creates a window for plotting.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
* BUGS:
* 1.	szClass seems to have restrictions on what characters can be used.
*		For example, "TST_CLAS" causes the class to be improperly set up,
*		while "TSTCLASS" is OK.  The symptom that has been observed is
*		that wndProc isn't registered as the window procedure and the
*		background and foreground colors aren't properly set.  The Windows
*		GDI call don't indicate an error.
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
HWND PASCAL
WuPlotWin(
HWND	hwParent,		// I I parent window handle
const char *szClass,	// I "window class"
WNDPROC	wndProc,		// I pointer to window procedure, or NULL
COLORREF rgbBkg,		// I color for background
COLORREF rgbFg,			// I color for foreground (e.g., text)
int		iLeft,			// I coordinate of left side of control
int		iTop,			// I I coordinate of top side of control
int		iWidth,			// I width of control
int		iHeight)		// I height of control
{
	HINSTANCE hInst=WuGetHInstance(hwParent);
	HWND	hwPlot=0;
	HDC		hDC;
	WNDCLASS plotClass;
	ATOM	atom;
	int		stat;

	stat = GetClassInfo(hInst, szClass, &plotClass);
	if (stat == 0) {  // the class hasn't been registered yet
		plotClass.style			= CS_OWNDC;
		plotClass.lpfnWndProc	= wndProc;
		plotClass.cbClsExtra	= 0;
		plotClass.cbWndExtra	= 3 * sizeof(void *);
		plotClass.hInstance		= hInst;
		plotClass.hIcon			= NULL;
		plotClass.hCursor		= NULL;
		plotClass.hbrBackground	= NULL;
		plotClass.lpszMenuName	= NULL;
		plotClass.lpszClassName = szClass;
		atom = RegisterClass(&plotClass);
		if (atom == 0)
			goto done;
	}

	hwPlot = CreateWindow(szClass, "",
					WS_CHILD | WS_VISIBLE | WS_TABSTOP |
							WS_BORDER | WS_HSCROLL | WS_VSCROLL,
					iLeft, iTop, iWidth, iHeight,
					hwParent, 0, hInst, NULL);
	if (hwPlot == 0)
		goto done;
	SetScrollRange(hwPlot, SB_HORZ, 0, 0, FALSE);
	SetScrollRange(hwPlot, SB_VERT, 0, 0, FALSE);
	hDC = GetDC(hwPlot);
	if (hDC == 0) {
		MessageBox(NULL, "error in GetDC", "WuPlotWin", MB_OK);
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
* WuPrtText_font - prints text through a DC
*
* Purpose:
*		Handles printing text through a DC.  Several services are available:
*		1.	Print the text.
*		2.	Return the coordinates of the quadrilateral that bounds the
*			text.
*		3.	Print the text only if its bounding quadrilateral doesn't
*			intersect a supplied quadrilateral.
*		4.	If the text has the form "abc[def]", the brackets are discarded
*			and the text inside the brackets is printed as superscript.
*			(Only one superscripted string may appear and it must be at the
*			end of the text.)
*
* Return Value:
*		0, or
*		1 if an error occurs
*
* Notes:
* 1.	The caller's DC controls whether the text is drawn OPAQUE or
*		TRANSPARENT.
* 2.	If pInQuad is non-NULL, then the current text is printed only if
*		its quadrilateral doesn't overlap pInQuad.  If the caller wants
*		to suppress printing based on the proximity of the text to another
*		quadrilateral--rather than based on overlap--then the caller must
*		do the check.  WuQuadsDoOverlap can be used for this purpose.
*
* BUGS:
* 1.	The intersection check is accurate only of the rotation angle is
*		a multiple of 90 degrees.
* 2.	This routine is a horrible kludge, with lots of hard-coded special
*		cases to get text alignment with pixel accuracy under Windows 3.1.
*
*-Date     Author		Revision
* -------- ------------	--------
* 04-02-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuPrtText_font(
HDC		hDC,			// I handle to DC to use for printing
const char *szText,		// I text to print
int		xPx,			// I X pixel location
int		yPx,			// I Y pixel location
int		eRefPt,			// I reference point in bounding box
const char *szFaceName,	// I name of "font"
float	fPtSize,		// I size, in points
int		bBold,			// I 1 to create a bold font
float	fDegrees,		// I angle
int		bNoVSpace,		// I 1 to suppress internal leading and descent
int		bBoundsOnly,	// I 1 to obtain quadrilateral but not print
const WU_QUAD *pInQuad,	// I input quadrilateral, or NULL
WU_QUAD	*pOutQuad)		// O place to store current quadrilateral, or NULL
{
	HFONT	hFont=0, hFontSuper=0, hFontOld=0;
	WORD	wDrawFlags;
	WORD	wAlignOld=0;
	TEXTMETRIC textMet;
	float	myFontSize;
	float	fTxtWid, fTxtHt;
	float	fRadians, fAngleCos, fAngleSin;
	float	fQuadX, fTextX, fQuadY, fTextY;
	int		stat=1;			// 1 signals an error
	WU_QUAD	newQuad;
	RECT	rText, rSuper;
	int		bRJust;			// right justify
	int		bBAlign;		// bottom align
	int		bCJust;			// center justify (horizontally)
	int		bCAlign;		// center align (vertically)
	int		i;
	const char *szBegBrack=NULL, *szEndBrack=NULL;
	int		nTextChar=0, nSuperChar=0;

	if (szText == NULL) szText = "";

	// Locate superscript text.
	szBegBrack = strchr(szText, '[');
	if (szBegBrack != NULL) {
		nTextChar = szBegBrack - szText;
		szBegBrack++;
		szEndBrack = strchr(szBegBrack, ']');
	}
	if (szEndBrack != NULL)
		nSuperChar = szEndBrack - szBegBrack;
	if (nSuperChar == 0)
		nTextChar = strlen(szText);

	if (fDegrees == 0.F) fAngleCos = 1.F, fAngleSin = 0.F;
	else if (fDegrees == 90.F) fAngleCos = 0.F, fAngleSin = 1.F;
	else if (fDegrees == -90.F) fAngleCos = 0.F, fAngleSin = -1.F;
	else {
		fRadians = fDegrees * .017453292F;
		fAngleCos = (float)cos(fRadians);
		fAngleSin = (float)sin(fRadians);
	}

	bRJust = eRefPt == PRT_TR || eRefPt == PRT_BR || eRefPt == PRT_RC;
	bCJust = eRefPt == PRT_TC || eRefPt == PRT_BC || eRefPt == PRT_C;
	bBAlign = eRefPt == PRT_BL || eRefPt == PRT_BR || eRefPt == PRT_BC;
	bCAlign = eRefPt == PRT_LC || eRefPt == PRT_RC || eRefPt == PRT_C;

	// size_px = pt * 20 twip/pt / 1440 twip/inch * px/inch
	myFontSize = fPtSize * 20.F / 1440.F * GetDeviceCaps(hDC, LOGPIXELSX);
	hFont = WuFontCreate(szFaceName, (int)myFontSize, 0, bBold, 0, (int)fDegrees);
	if (hFont == 0) goto done;
	hFontOld = SelectObject(hDC, hFont);
	if (nSuperChar > 0) {
		hFontSuper = WuFontCreate(szFaceName,
							(int)(myFontSize*.7F), 0, bBold, 0, (int)fDegrees);
		if (hFontSuper == 0)
			nSuperChar = 0;
	}

	stat = GetTextMetrics(hDC, &textMet);
	if (stat == 0) {
		stat = 1;	// error
		goto done;
	}

	wAlignOld = SetTextAlign(hDC, TA_NOUPDATECP | TA_TOP | TA_LEFT);
	wDrawFlags = DT_SINGLELINE | DT_NOPREFIX | DT_LEFT | DT_TOP | DT_CALCRECT;
	// Go through the text string, segment by segment, to find out how
	// large the whole thing is.  If subscripts or superscripts are
	// encountered, the font is changed for the affected segment(s).
	rText.left = xPx;
	rText.right = xPx + 1;
	rText.top = yPx;
	rText.bottom = yPx + textMet.tmHeight;
	if (szText[0] != '\0') {
		stat = DrawText(hDC, szText, nTextChar, &rText, wDrawFlags);
		if (stat == 0) {
			stat = 1;	// error
			goto done;
		}
	}
	if (nSuperChar > 0) {
		rSuper.left = rText.right;
		rSuper.top = rText.top;
		SelectObject(hDC, hFontSuper);
		stat = DrawText(hDC, szBegBrack, nSuperChar, &rSuper, wDrawFlags);
		if (stat == 0) {
			stat = 1;	// error
			goto done;
		}
		SelectObject(hDC, hFont);
		if (fDegrees > 0.F) {
			if (bRJust)
				rText.left -= 1;
			else if (bCJust)
				rText.left -= 1;
			else
				rText.left += 1;
		}
		rText.right = rSuper.right;
	}

	fQuadX = fTextX = (float)xPx;
	fQuadY = fTextY = (float)yPx;
	fTxtWid = 1.F + (float)(rText.right - rText.left);
	fTxtHt = (float)(rText.bottom - rText.top);
	// Make adjustments in the x,y coordinates if caller wants to suppress
	// descent and internal leading.
	if (bNoVSpace) {
		fTxtHt -= textMet.tmInternalLeading + textMet.tmDescent;
		fTextY -= (int)(textMet.tmInternalLeading * fAngleCos);
		fTextX -= (int)(textMet.tmInternalLeading * fAngleSin);
	}
/*	if (fDegrees == 0.F) {
		if (bRJust) {
			fTextX += 1.F;
			fQuadX += 1.F;
		}
		else if (!bCJust) {
			fTextX -= 1.F;
		}
	}
*/
	if (fDegrees > 0.F) {
		if (bRJust) {
			fTextY -= 1.F;
			fQuadY -= 1.F;
		}
		else if (!bCJust) {
			fTextY += 2.F;
		}
		if (bBAlign)
			;	// no action
		else if (bCAlign)
			;	// no action
		else
			fTextX -= 1.F;
	}
/*	if (fDegrees < 0.F) {
		if (bRJust) {
			fTextY += 2.F;
			fQuadY += 1.F;
		}
		else if (!bCJust) {
			fTextY -= 1.F;
		}
		if (bBAlign)
			fTextX += 1.F;
		else if (bCAlign)
			;	// no action
		else
			fTextX += 2.F;
	}
*/
	// Rotate the top line, assuming TL anchoring.
	newQuad.x[0] = fQuadX;
	newQuad.y[0] = fQuadY;
	newQuad.x[1] = fQuadX + (fTxtWid-1.F) * fAngleCos;
	newQuad.y[1] = fQuadY - (fTxtWid-1.F) * fAngleSin;
	// Translate the top line sideways, to match the H justification
	if (bRJust) {
		fTextX -= fTxtWid * fAngleCos;
		fTextY += fTxtWid * fAngleSin;
		for (i=0; i<=1; i++) {
			newQuad.x[i] -= fTxtWid * fAngleCos;
			newQuad.y[i] += fTxtWid * fAngleSin;
		}
	}
	else if (bCJust) {
		fTextX -= .5F * fTxtWid * fAngleCos;
		fTextY += .5F * fTxtWid * fAngleSin;
		for (i=0; i<=1; i++) {
			newQuad.x[i] -= .5F * fTxtWid * fAngleCos;
			newQuad.y[i] += .5F * fTxtWid * fAngleSin;
		}
	}
	// Shift the top line, to match the V alignment
	if (bBAlign) {
		fTextX -= (fTxtHt-1) * fAngleSin;
		fTextY -= (fTxtHt-1) * fAngleCos;
		for (i=0; i<=1; i++) {
			newQuad.x[i] -= (fTxtHt-1) * fAngleSin;
			newQuad.y[i] -= (fTxtHt-1) * fAngleCos;
		}
	}
	else if (bCAlign) {
		fTextX -= .5F * (fTxtHt-1) * fAngleSin;
		fTextY -= .5F * (fTxtHt-1) * fAngleCos;
		for (i=0; i<=1; i++) {
			newQuad.x[i] -= .5F * (fTxtHt-1) * fAngleSin;
			newQuad.y[i] -= .5F * (fTxtHt-1) * fAngleCos;
		}
	}
	// And, finally, convert the top line to whole pixels and
	// calculate the bottom line.
	newQuad.x[0] = (float)floor(newQuad.x[0]);
	newQuad.x[1] = (float)ceil(newQuad.x[1]);
	newQuad.y[0] = (float)floor(newQuad.y[0]);
	newQuad.y[1] = (float)ceil(newQuad.y[1]);
	for (i=0; i<=1; i++) {
		newQuad.x[3-i] = newQuad.x[i] + (fTxtHt-1) * fAngleSin;
		newQuad.y[3-i] = newQuad.y[i] + (fTxtHt-1) * fAngleCos;
	}

	stat = 0;	// now use "success" as default status
	if (bBoundsOnly)
		goto done;
	if (pInQuad != NULL) {
		// Check for intersection between this quad and caller's.
		// Skip printing if they intersect.
		if (WuQuadsDoOverlap(pInQuad, &newQuad, 0.F, 0.F))
			goto done;
	}
#ifdef TEST
	MoveToEx(hDC, (int)(.5F+newQuad.x[0]), (int)(.5F+newQuad.y[0]), NULL);
	LineTo(hDC, (int)(.5F+newQuad.x[1]), (int)(.5F+newQuad.y[1]));
	LineTo(hDC, (int)(.5F+newQuad.x[2]), (int)(.5F+newQuad.y[2]));
	LineTo(hDC, (int)(.5F+newQuad.x[3]), (int)(.5F+newQuad.y[3]));
	LineTo(hDC, (int)(.5F+newQuad.x[0]), (int)(.5F+newQuad.y[0]));
#endif
	// And, finally, print the text.
	if (szText[0] != '\0') {
		stat = TextOut(hDC, (int)(floor(.5F+fTextX)), (int)(floor(.5F+fTextY)),
							szText, nTextChar);
		if (stat == 0) {
			stat = 1;	// error
			goto done;
		}
	}
	if (nSuperChar > 0) {
		SelectObject(hDC, hFontSuper);
		SetTextAlign(hDC, TA_NOUPDATECP | TA_TOP | TA_RIGHT);
		if (fDegrees == 0.F) {
			stat = TextOut(hDC, (int)newQuad.x[1], (int)(floor(.5F+fTextY))+1,
							szBegBrack, nSuperChar);
		}
		else if (fDegrees > 0.F) {
			if (!bBAlign) {
				stat = TextOut(hDC, (int)(floor(.5F+fTextX))+1, (int)newQuad.y[1],
							szBegBrack, nSuperChar);
			}
			else {
				stat = TextOut(hDC, (int)(floor(.5F+fTextX))+1, (int)newQuad.y[1],
							szBegBrack, nSuperChar);
			}
		}
		else {
				stat = TextOut(hDC, (int)(floor(.5F+fTextX))-1, (int)newQuad.y[1],
							szBegBrack, nSuperChar);
		}
		if (stat == 0) {
			stat = 1;	// error
			goto done;
		}
		SelectObject(hDC, hFont);
	}
	stat = 0;	// OK
#ifdef TEST
	SetPixel(hDC, xPx, yPx, RED);
	SetPixel(hDC, (int)(.5F+fTextX), (int)(.5F+fTextY), MAGENTA);
#endif

done:
	if (wAlignOld != 0) SetTextAlign(hDC, wAlignOld);
	if (hFontOld != 0) SelectObject(hDC, hFontOld);
	if (hFont != 0) DeleteObject(hFont);
	if (hFontSuper != 0) DeleteObject(hFontSuper);

	if (pOutQuad != NULL)
		*pOutQuad = newQuad;
	return stat;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuPrtText_font_ROP2 - prints text through a DC, using ROP2
*
* Purpose:
*		Handles printing text through a DC, using the DC's current
*		ROP2.  This routine is an alternate entry to WuPrtText_font,
*		which does not handle ROP2; see WuPrtText_font for additional
*		information.
*
*		This routine prints the text with:
*		o	orientation angle is 0
*		o	reference point is PRT_TL
*		o	internal leading and descent are not suppressed.
*
*		This routine handles only R2_XORPEN and R2_COPYPEN.
*
* Return Value:
*		0, or
*		1 if an error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-29-97 R. Cole		cloned from WuPrtText_font
*--------------------------------------------------------------------------*/
int PASCAL
WuPrtText_font_ROP2(
HDC		hDC,			// I handle to DC to use for printing
const char *szText,		// I text to print
int		xPx,			// I X pixel location
int		yPx,			// I Y pixel location
const char *szFaceName,	// I name of "font"
float	fPtSize,		// I size, in points
int		bBold,			// I 1 to create a bold font
int		bVert)			// I 1 to draw text vertically
{
	int		stat;
	HDC		hDCBM=0;
	HBRUSH	hBrushBM=0;
	HBITMAP	hBM=0;
	WU_QUAD	quadText;
	int		nTextWid, nTextHt;
	float	fTextDeg;

	if (bVert)		fTextDeg = 90.F;
	else			fTextDeg = 0.F;

	// Get the quad for the text.  Then create a bitmap of the same size.
	stat = WuPrtText_font(hDC, szText, 0, 0, PRT_TL, szFaceName, fPtSize,
					bBold, fTextDeg, 0, 1, NULL, &quadText);
	if (stat == 1) goto gdi_error;
	if (bVert) {
		nTextWid = 1 + (int)(quadText.x[2] - quadText.x[0]);
		nTextHt = 3 + (int)(quadText.y[0] - quadText.y[2]);
	}
	else {
		nTextWid = 3 + (int)(quadText.x[2] - quadText.x[0]);
		nTextHt = 1 + (int)(quadText.y[2] - quadText.y[0]);
	}
	hBM = CreateCompatibleBitmap(hDC, nTextWid, nTextHt);
	if (hBM == 0) goto gdi_error;
	// Set up for drawing in the bitmap.  Then draw the text into the bitmap.
	hDCBM = CreateCompatibleDC(hDC);
	if (hDCBM == 0) goto gdi_error;
	SelectObject(hDCBM, hBM);
	hBrushBM = CreateSolidBrush(GetBkColor(hDC));
	if (hBrushBM == 0) goto gdi_error;
	SelectObject(hDCBM, hBrushBM);
	PatBlt(hDCBM, 0, 0, nTextWid, nTextHt, PATCOPY);
	SetBkColor(hDCBM, GetBkColor(hDC));
	SetBkMode(hDCBM, GetBkMode(hDC));
	SetTextColor(hDCBM, GetTextColor(hDC));
	if (bVert) {
		stat = WuPrtText_font(hDCBM, szText, 0, 2, PRT_TR, szFaceName, fPtSize,
					bBold, fTextDeg, 0, 0, NULL, NULL);
		if (stat == 1) goto gdi_error;
	}
	else {
		stat = WuPrtText_font(hDCBM, szText, 2, 0, PRT_TL, szFaceName, fPtSize,
					bBold, fTextDeg, 0, 0, NULL, NULL);
		if (stat == 1) goto gdi_error;
	}
	// Finally, write the bitmap through the caller's DC.
	if (GetROP2(hDC) == R2_XORPEN)
		BitBlt(hDC, xPx, yPx, nTextWid, nTextHt, hDCBM, 0, 0, SRCINVERT);
	else
		BitBlt(hDC, xPx, yPx, nTextWid, nTextHt, hDCBM, 0, 0, SRCCOPY);

done:
	if (hDCBM != 0) DeleteObject(hDCBM);
	if (hBrushBM != 0) DeleteObject(hBrushBM);
	if (hBM != 0) DeleteObject(hBM);
	return stat;
gdi_error:
	stat = 1;
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuQuadClear - clears a quadrilateral
*
* Purpose:
*		Fills a quadrilateral with the background color
*
* Return Value:
*		0, or
*		1 if an error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 02-11-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuQuadClear(
HDC		hDC,			// I handle to DC to use for printing
const WU_QUAD *pQuad)	// I quadrilateral
{
	HBRUSH	hBrush, hBrushOld;
	HPEN	hPen=0, hPenOld=0;
	POINT	aPt[4];
	int		i;
	int		stat=1;		// 1 signals an error

	if ((hBrush = CreateSolidBrush(GetBkColor(hDC))) == 0) goto done;
	if ((hBrushOld = SelectObject(hDC, hBrush)) == 0) goto done;
	if ((hPen = CreatePen(PS_SOLID, 1, GetBkColor(hDC))) == 0) goto done;
	if ((hPenOld = SelectObject(hDC, hPen)) == 0) goto done;

	for (i=0; i<4; i++) {
		aPt[i].x = (int)(pQuad->x[i] + .5F);
		aPt[i].y = (int)(pQuad->y[i] + .5F);
	}
	if (Polygon(hDC, aPt, 4) == 0) goto done;

	stat = 0;	// success

done:
	if (hBrushOld != 0) SelectObject(hDC, hBrushOld);
	if (hBrush != 0) DeleteObject(hBrush);
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);

	return stat;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuQuadsDoOverlap - checks to see of two quadrilaterals overlap
*
* Purpose:
*		Checks to see of two quadrilaterals overlap.
*
* Return Value:
*		1 if there is an overlap, or
*		0 otherwise
*
*-Date     Author		Revision
* -------- ------------	--------
* 07-18-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuQuadsDoOverlap(
const WU_QUAD *pQuad1,	// I quadrilateral
const WU_QUAD *pQuad2,	// I quadrilateral
float	fXDist,			// I X spacing required
float	fYDist)			// I Y spacing required
{
	int		i;
	float	fQuad1XMax, fQuad1XMin, fQuad1YMax, fQuad1YMin;
	float	fQuad2XMax, fQuad2XMin, fQuad2YMax, fQuad2YMin;

#define MY_MAX(max, value) if (i == 0) max = value; else if (max < value) max = value;
#define MY_MIN(min, value) if (i == 0) min = value; else if (min > value) min = value;
	for (i=0; i<4; i++) {
		MY_MAX(fQuad1XMax, pQuad1->x[i]);
		MY_MAX(fQuad1YMax, pQuad1->y[i]);
		MY_MIN(fQuad1XMin, pQuad1->x[i]);
		MY_MIN(fQuad1YMin, pQuad1->y[i]);
		MY_MAX(fQuad2XMax, pQuad2->x[i]);
		MY_MAX(fQuad2YMax, pQuad2->y[i]);
		MY_MIN(fQuad2XMin, pQuad2->x[i]);
		MY_MIN(fQuad2YMin, pQuad2->y[i]);
	}
	// If the X's are further apart than the required X spacing,
	// there's no intersection
	if (fQuad2XMin - fQuad1XMax > fXDist || fQuad1XMin - fQuad2XMax > fXDist)
		return 0;
	// The X spacing is less than required; check the Y's.  Essentially the
	// same rule applies.
	if (fQuad2YMin - fQuad1YMax > fYDist || fQuad1YMin - fQuad2YMax > fYDist)
		return 0;
	// There is an intersection of _both_ X _and_ Y, indicating that the
	// distance between quadrilaterals is less than required.
	return 1;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuSzCmpWild - compare two strings, using wild-card matching
*
* Purpose:
*		Compares a text string a text pattern, where the pattern
*		can contain wildcard characters.  This routine is analogous
*		to the C library strcmp routine, with the enhancement that
*		"equal" has been changed to "match".
*
*		The wildcard characters and their meanings are:
*			? matches any single character
*			* matches any sequence of zero or more characters
*
*		text         pattern     result
*
*		""            anything     1
*		anything      ""           1
*		"abc"         "ab"         1
*		"ab"          "abc"        1
*		"abc"         "ab*"        0
*		"abc"         "ab?"        0
*		"abc"         "???"        0
*		"abc"         "????"       1
*		"abc"         "ab*d"       1
*
* Return Value:
*		0  if a match is obtained, or
*		1  otherwise
*
* Notes:
* 1.	The following conditions always cause a return value of 1:
*		o	szText is NULL or the length of szText is 0
*		o	szPattern is NULL or the length of szPattern is 0
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-29-92 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
WuSzCmpWild(
const char *szText,		// I input text to be checked, or NULL
const char *szPattern,	// I pattern, possibly with wildcards, or NULL
int		bIgnoreCase)	// I 1 to ignore upper/lower case differences
{
	const char *szStarP=NULL; // pointer in szPattern at beginning of * wildcard
	const char *szStarT=NULL; // pointer in szText at beginning of * wildcard
	int		bMatched;		// char in szText matches char in pattern
	char	cT, cP;			// char from szText and szPattern, respectively

	if (szText == NULL) szText = "";
	if (szPattern == NULL) szPattern = "";

	if (*szText == '\0')
		return 1;
	while (*szPattern != '\0' && *szText != '\0') {
		if (*szPattern == '*') {
			szStarP = ++szPattern;
			if (*szPattern == '\0')
			    return 0;
			szStarT = szText;
		}
		else {
			if (*szText == *szPattern || *szPattern == '?')
				bMatched = 1;
			else
				bMatched = 0;
			if (bIgnoreCase && !bMatched) {
				cT = *szText; cP = *szPattern;
				if (isascii(cT) && isascii(cP) && isalpha(cT) && isalpha(cP)) {
					if (isupper(cT)) cT = tolower(cT);
					if (isupper(cP)) cP = tolower(cP);
					if (cT == cP)
						bMatched = 1;
				}
			}
			if (bMatched) {
				szText++, szPattern++;
				if (*szPattern == '\0') {
					if (*szText == '\0' || *(szPattern-1) == '*')
						return 0;
					szText = ++szStarT;
					if (szStarP == NULL)
						return 1;
					szPattern = szStarP;
				}
			}
			else if (szStarP == NULL)
				return 1;
			else {
				szText = ++szStarT;
				szPattern = szStarP;
			}
		}
	}
	if (*szText == '\0') {
		if (*szPattern == '\0')
			return 0;
		if (*szPattern != '*')
			return 1;
		if (*(++szPattern) == '\0')
			return 0;
	}
	return 1;
}
/*+/csubr/TOC----------------------------------------------------------------
* WuSzMCopy - copy text into a C null-terminated string
*
* Purpose:
*		Copies text into a C null-terminated string, respecting the
*		dimension of the destination buffer.
*
* Return Value:
*		void
*
* Notes:
* 1.	At exit, the destination string is guaranteed to be properly '\0'
*		terminated.
* 2.	This routine can be used by VB programs to put text into C
*		structures.
*
*-Date     Author		Revision
* -------- ------------	--------
* 10-05-95 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuSzMCopy(
char	*szDest,		// O place to store text
int		destDim,		// I dimension of szDest
const char *szSource)	// I source text, or NULL
{
	if (szSource == NULL) szSource = "";
	strncpy(szDest, szSource, destDim-1);
	szDest[destDim-1] = '\0';
}
/*+/csubr/TOC----------------------------------------------------------------
* WuSzMCat - append text into a C null-terminated string
*
* Purpose:
*		Copies text to the end of a C null-terminated string, respecting the
*		dimension of the destination buffer.
*
* Return Value:
*		void
*
* Notes:
* 1.	At exit, the destination string is guaranteed to be properly '\0'
*		terminated.
* 2.	This routine can be used by VB programs to put text into C
*		structures.
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-06-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
WuSzMCat(
char	*szDest,		// O place to store text
int		destDim,		// I dimension of szDest
const char *szSource)	// I source text, or NULL
{
	int		iC;

	if (szSource == NULL) szSource = "";

	iC = strlen(szDest);
	while (1) {
		if (iC >= destDim-1) {
			szDest[destDim-1] = '\0';
			break;
		}
		szDest[iC] = *szSource;
		if (*szSource == '\0')
			break;
		iC++;
		szSource++;
	}
}
