#if ! defined HDR_wu // {
#define HDR_wu

/* ======================================================================= */
/* ================================ wu.h ================================= */
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
*
*		Also, much of the information in this file is used by Visual
*		Basic programs.  See individual '* Related Files:' discussions
*		for additional information.  For the affected areas, changes in
*		this file will require corresponding changes in the related files.
*--------------------------------------------------------------------------*/

/*+/mod/TOC------------------------------------------------------------------
* wu.h - definitions and prototypes for wu.c
*
* Purpose:
*		This header file supplies definitions and prototypes for use with
*		the WuXxx functions in wu.c.
*
* Usage:
*		#include <windows.h>
*		#include <wu.h>
*
* Required for:
*		This header file is required for use with WuXxx functions and to
*		supply WU_XXX parameters and structures.
*
* Date     Author		Revision
* -------- ------------	--------
* 09-07-95 R. Cole		created
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

/*---------------------------------------------------------------------------
* Related Files:
*		o	WMCA_DEF.BAS  contains the VB versions of these constants
*--------------------------------------------------------------------------*/
#define WU_ALERT_MAX_N_CHOICE 4
enum {
	WU_ICON_INFO=0,		// an "i"
	WU_ICON_QUES,		// a question mark
	WU_ICON_WARN,		// an exclamation mark
	WU_ICON_ABORT		// a stop sign
};

// Typical usage for a WU_QUAD has, in order, TL, TR, BR, BL corners of the
// quadrilateral.  This assumes an orientation angle of 0 degrees.  The
// only reliable general description is that opposite corners of the
// quadrilateral have subscripts that differ by 2.
typedef struct {
	float	x[4];
	float	y[4];
} WU_QUAD;

#define BLACK			0x000000
#define BLUE			0xFF0000
#define BLUE_DARK		0x800000
#define CYAN			0xFFFF00
#define GRAY			0xC0C0C0
#define GRAY_DARK		0x808080
#define GREEN			0x00FF00
#define GREEN_DARK		0x008000
#define MAGENTA			0xFF00FF
#define MAGENTA_DARK	0x800080
#define RED				0x0000FF
#define RED_DARK		0x000080
#define WHITE			0xFFFFFF
#define YELLOW			0x00FFFF

/*---------------------------------------------------------------------------
* prototypes
*
* Related Files:
*		o	WMCA_DEF.BAS  contains VB versions of some of these prototypes
*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#if defined MAKE_DLL && defined WIN32
#define EXPORT16
#define EXPORT32 __declspec(dllexport)
#elif defined MAKE_DLL
#define EXPORT16 __export
#define EXPORT32
#else
#define EXPORT16
#define EXPORT32
#endif

EXPORT32 int EXPORT16 PASCAL
WuAlert(
	HWND	hwParent,		// I parent window handle
	const char *szAlert,	// I specially formatted alert text
	int		iIcon);			// I icon to display--WU_ICON_XXX

EXPORT32 HWND EXPORT16 PASCAL
WuButton(
	HWND	hwParent,		// I parent window handle
	UINT	idControl,		// I resource ID of the control
	const char *szCaption,	// I caption for control
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

EXPORT32 HWND EXPORT16 PASCAL
WuButton_dflt(
	HWND	hwParent,		// I parent window handle
	UINT	idControl,		// I resource ID of the control
	const char *szCaption,	// I caption for control
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

EXPORT32 HWND EXPORT16 PASCAL
WuCheckBox(
	HWND	hwParent,		// I parent window handle
	UINT	idControl,		// I resource ID of the control
	const char *szCaption,	// I caption for control
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

EXPORT32 int EXPORT16 PASCAL
WuCheckBox_chk(
	HWND	hwCtrl);		// I checkbox handle

EXPORT32 void EXPORT16 PASCAL
WuCheckBox_set(
	HWND	hwCtrl,			// I checkbox handle
	int		bSet);			// I 1,0 to check,uncheck checkbox

EXPORT32 int EXPORT16 PASCAL
WuClipIt(
	int		bAlter,			// I 1 to alter the coordinates of the point
	int		l,				// I X coordinate of left side of rect
	int		t,				// I Y coordinate of top side of rect
	int		r,				// I X coordinate of right side of rect
	int		b,				// I Y coordinate of bottom side of rect
	double	*pdX1,			// IO X coordinate of first point
	double	*pdY1,			// IO Y coordinate of first point
	double	*pdX2,			// IO X coordinate of second point
	double	*pdY2);			// IO Y coordinate of second point

EXPORT32 void EXPORT16 PASCAL
WuControlMove(
	HWND	hwCtrl,			// I control handle
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

EXPORT32 void EXPORT16 PASCAL
WuControlShow(
	HWND	hwCtrl,			// I control handle
	int		bShow);			// I 1 to make control visible, else 0

EXPORT32 void EXPORT16 PASCAL
WuDelay(
	int		iSeconds,		// I the number of seconds to delay
	int		*pbCancel);		// I cancel flag, or NULL

EXPORT32 void EXPORT16 PASCAL
WuDirAdd(
	char	*szDest,		// O place to store text
	int		destDim,		// I dimension of szDest
	const char *szDir,		// I disk/directory specification
	const char *szFile);	// I file name

EXPORT32 void EXPORT16 PASCAL
WuDirStrip(
	char	*szDest,		// O place to store text
	int		destDim,		// I dimension of szDest
	const char *szSource,	// I source text
	const char *szDfltDir);	// I default directory, or NULL

EXPORT32 void EXPORT16 PASCAL
WuDoEvents(void);

EXPORT32 HWND EXPORT16 PASCAL
WuEdit(
	HWND	hwParent,		// I parent window handle
	UINT	idControl,		// I resource ID of the control
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

EXPORT32 HWND EXPORT16 PASCAL
WuEditBig(
	HWND	hwParent,		// I parent window handle
	UINT	idControl,		// I resource ID of the control
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

EXPORT32 void EXPORT16 PASCAL
WuEditAddLine(
	HWND	hwEdit,
	const char *szLine);

EXPORT32 void EXPORT16 PASCAL
WuEditClear(
	HWND	hwEdit);

EXPORT32 void EXPORT16 PASCAL
WuEditGetText(
	HWND	hwEdit,
	char	*szDest,		// O place to store text
	int		destDim);		// I dimension of szDest

EXPORT32 void EXPORT16
WuEditPrintf(
	HWND	hwEdit,
	const char *szFmt,
	...);

EXPORT32 void EXPORT16 PASCAL
WuEditSetText(
	HWND	hwEdit,
	const char *szText);

EXPORT32 void EXPORT16
WuEditSetTextPrintf(
	HWND	hwEdit,
	const char *szFmt,
	...);

EXPORT32 int EXPORT16 PASCAL
WuFileAutonumCheck(
	const char *szDir,		// I directory containing the files
	const char *szMask,		// I the mask
	int		iBegRun,		// I the beginning run number
	int		iEndRun,		// I the ending run number
	int		*pnExist);		// O the number of files that exist

EXPORT32 int EXPORT16 PASCAL
WuFileAutonumName(
	const char *szMask,		// I the mask
	int		*piRun,			// IO the run number
	int		iBufDim,		// I dimension of szBuf
	char	*szBuf);		// O buffer to receive new name

EXPORT32 int EXPORT16 PASCAL
WuFileModDateGet(
	const char *szPath,		// I path to file
	long	*lSec);			// O mod date, sec since Windows epoch start

EXPORT32 int EXPORT16 PASCAL
WuFileModDateSet(
	const char *szPath,		// I path to file
	long	lSec);			// I mod date, sec since Windows epoch start

EXPORT32 void EXPORT16 PASCAL
WuFileStrip(
	char	*szDest,		// O place to store text
	int		destDim,		// I dimension of szDest
	const char *szSource);	// I source text

EXPORT32 HFONT EXPORT16 PASCAL
WuFontCreate(
	const char *szFaceName,	// I name of "font"
	int		iSize,			// I size, in logical units
	int		bItalic,		// I 1 to create an italic font
	int		bBold,			// I 1 to create a bold font
	int		bUnderline,		// I 1 to create an underlined font
	int		iDegrees);		// I orientation, in degrees

EXPORT32 void EXPORT16 PASCAL
WuGetCSzFromPsz(
	char	*szDest,		// O place to store text
	int		destDim,		// I dimension of szDest
	const char *pszText);	// I pointer to source text

EXPORT32 HINSTANCE EXPORT16 PASCAL
WuGetHInstance(
	HWND	hwWindow);		// I window handle

EXPORT32 void EXPORT16 PASCAL
WuGetWinVersion(
float	*pfVersion,		// O version number
int		*pbWin95,		// O 1 if Windows95
int		*pbWinNT);		// O 1 if WindowsNT

EXPORT32 HWND EXPORT16 PASCAL
WuGroupBox(
	HWND	hwParent,		// I I parent window handle
	UINT	idControl,		// I resource ID of the control
	const char *szCaption,	// I caption for control
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

EXPORT32 HWND EXPORT16 PASCAL
WuLabel(
	HWND	hwParent,		// I parent window handle
	UINT	idControl,		// I resource ID of the control
	const char *szCaption,	// I caption for control
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

EXPORT32 HWND EXPORT16 PASCAL
WuListNASz(
	HWND	hwParent,		// I I parent window handle
	UINT	idControl,		// I resource ID of the control
	const char *naszItems[],// I NULL terminated array of sz items
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

EXPORT32 void EXPORT16 PASCAL
WuListNASz_get(
	HWND	hwCtrl,			// I text listbox handle
	char	*szDest,		// O place to store text
	int		destDim);		// I dimension of szDest

EXPORT32 void EXPORT16 PASCAL
WuListNASz_set(
	HWND	hwCtrl,			// I text listbox handle
	const char *szText);	// I desired text selection

EXPORT32 HWND EXPORT16 PASCAL
WuMainWin(
	HINSTANCE hInst,		// I handle to this instance
	HINSTANCE hPrev,		// I handle to previous instance
	const char *szAppName,	// I application name, for icon, menu, class, etc.
	WNDPROC	wndProc,		// I pointer to window procedure
	int		nCmdShow,		// I how to show window--SW_SHOW...
	int		bShow,			// I 1 to show the window now
	int		iLeft,			// I coordinate of left side of window
	int		iTop,			// I coordinate of top side of window
	int		iWidth,			// I width of window
	int		iHeight);		// I height of window

EXPORT32 HWND EXPORT16 PASCAL
WuMainWin_noBorder(
	HINSTANCE hInst,		// I handle to this instance
	HINSTANCE hPrev,		// I handle to previous instance
	const char *szAppName,	// I application name, for menu, class, etc.
	WNDPROC	wndProc,		// I pointer to window procedure
	int		nCmdShow,		// I how to show window--SW_SHOW...
	int		bShow,			// I 1 to show the window now
	int		iLeft,			// I coordinate of left side of window
	int		iTop,			// I coordinate of top side of window
	int		iWidth,			// I width of window
	int		iHeight);		// I height of window

EXPORT32 HWND EXPORT16 PASCAL
WuPlListColor(
	HWND	hwParent,		// I I parent window handle
	UINT	idControl,		// I resource ID of the control
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

LRESULT EXPORT16 CALLBACK
WuPlListColor_event(
	HWND	hWnd,
	UINT	iMsg,
	WPARAM	wParam,
	LPARAM	lParam);

EXPORT32 COLORREF EXPORT16 PASCAL
WuPlListColor_get(
	HWND	hwCtrl);		// I checkbox handle

EXPORT32 void EXPORT16 PASCAL
WuPlListColor_set(
	HWND	hwCtrl,			// I checkbox handle
	COLORREF rgbSet);		// I color to set in color listbox

EXPORT32 HWND EXPORT16 PASCAL
WuPlListPattern(
	HWND	hwParent,		// I I parent window handle
	UINT	idControl,		// I resource ID of the control
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

LRESULT EXPORT16 CALLBACK
WuPlListPattern_event(
	HWND	hWnd,
	UINT	iMsg,
	WPARAM	wParam,
	LPARAM	lParam);

EXPORT32 int EXPORT16 PASCAL
WuPlListPattern_get(
	HWND	hwCtrl);		// I checkbox handle

EXPORT32 void EXPORT16 PASCAL
WuPlListPattern_set(
	HWND	hwCtrl,			// I checkbox handle
	int		ePattern);		// I line pattern--PL_LINE_XXX

EXPORT32 HWND EXPORT16 PASCAL
WuPlListThick(
	HWND	hwParent,		// I I parent window handle
	UINT	idControl,		// I resource ID of the control
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

LRESULT EXPORT16 CALLBACK
WuPlListThick_event(
	HWND	hWnd,
	UINT	iMsg,
	WPARAM	wParam,
	LPARAM	lParam);

EXPORT32 float EXPORT16 PASCAL
WuPlListThick_get(
	HWND	hwCtrl);		// I checkbox handle

EXPORT32 void EXPORT16 PASCAL
WuPlListThick_set(
	HWND	hwCtrl,			// I checkbox handle
	float	fPts);			// I thickness to set in listbox, in printer points

EXPORT32 void EXPORT16 PASCAL
WuPlotKeydown(
	HWND	hwPlot,
	WPARAM	wParam,
	LPARAM	lParam);

LRESULT EXPORT16 CALLBACK
WuPlotProc(
	HWND	hWnd,
	UINT	iMsg,
	WPARAM	wParam,
	LPARAM	lParam);

EXPORT32 HWND EXPORT16 PASCAL
WuPlotWin(
	HWND	hwParent,		// I parent window handle
	const char *szClass,	// I "window class"
	WNDPROC	wndProc,		// I pointer to window procedure
	COLORREF rgbBkg,		// I color for background
	COLORREF rgbFg,			// I color for foreground (e.g., text)
	int		iLeft,			// I coordinate of left side of control
	int		iTop,			// I coordinate of top side of control
	int		iWidth,			// I width of control
	int		iHeight);		// I height of control

EXPORT32 int EXPORT16 PASCAL
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
	WU_QUAD	*pOutQuad);		// O place to store current quadrilateral, or NULL

EXPORT32 int EXPORT16 PASCAL
WuPrtText_font_ROP2(
	HDC		hDC,			// I handle to DC to use for printing
	const char *szText,		// I text to print
	int		xPx,			// I X pixel location
	int		yPx,			// I Y pixel location
	const char *szFaceName,	// I name of "font"
	float	fPtSize,		// I size, in points
	int		bBold,			// I 1 to create a bold font
	int		bVert);			// I 1 to draw text vertically

EXPORT32 int EXPORT16 PASCAL
WuQuadClear(
	HDC		hDC,			// I handle to DC to use for printing
	const WU_QUAD *pQuad);	// I quadrilateral

EXPORT32 int EXPORT16 PASCAL
WuQuadsDoOverlap(
	const WU_QUAD *pQuad1,	// I quadrilateral
	const WU_QUAD *pQuad2,	// I quadrilateral
	float	fXDist,			// I X spacing required
	float	fYDist);		// I Y spacing required

EXPORT32 int EXPORT16 PASCAL
WuSzCmpWild(
	const char *szText,		// I input text to be checked, or NULL
	const char *szPattern,	// I pattern, possibly with wildcards, or NULL
	int		bIgnoreCase);	// I 1 to ignore upper/lower case differences

EXPORT32 void EXPORT16 PASCAL
WuSzMCopy(
	char	*szDest,		// O place to store text
	int		destDim,		// I dimension of szDest
	const char *szSource);	// I source text

EXPORT32 void EXPORT16 PASCAL
WuSzMCat(
	char	*szDest,		// O place to store text
	int		destDim,		// I dimension of szDest
	const char *szSource);	// I source text

#undef EXPORT16
#undef EXPORT32

#ifdef __cplusplus
}
#endif

#endif // ! defined HDR_wu }
