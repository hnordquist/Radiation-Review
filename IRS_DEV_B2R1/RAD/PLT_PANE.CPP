///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : PltPane.cpp     
//Primary Author: Tom Marks

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////


#include <gui_lib.h>
#include "plt_pane.h"


// =====================================================================
// =========================== CPlotPane ========================
// =====================================================================

CPlotPane::CPlotPane(CGUI_Dlg *pDlg, CPlotDlg *pPlotDlg, RESOURCE_ID PaneID,
	void (*RightMouseCallBackFn)(double X, double Y, UINT uiAxis))
	: CGUI_PlotPane(pDlg, PaneID)
{
	OutputDebugString("CPlotPane::CTOR\n");

	BOOL bUserAnnotX = TRUE;
	BOOL bUserAnnotY = TRUE; //sfk
	SetUserAnnotation(bUserAnnotX, bUserAnnotY);
	mRightMouseCallBackFn = RightMouseCallBackFn;
	mbFirstDisplayCursorReadoutText = TRUE;
	mpPlotDlg = pPlotDlg;
	mpDlg = pDlg;

	mszY_AnnotDefaultFmtLinear = "%.0f";	// PL_FMT_AUTO;

	int iStatus = PlWinRegObjectCallback(&mPlCtx, &CPlotPane::PlotLibObjCallBackFn, this);
}			


CPlotPane::~CPlotPane()
{
//	SaveGraphSetup();	// Save current settings.
}

	
// This function must be static so it can be registered with the pl lib.
// Once it gets called get a pointer to the appropriate CGUI_PlotPane which
// was registered with the pl lib and is now handed to us as pUserData.

int CPlotPane::PlotLibObjCallBackFn(PL_CTX *pPlCtx, void *pUserData,
	PL_OBJ_CTX *pObjCtx, PL_OBJ *pObj, int eEvent, double dArg1, double dArg2)
{
	CPlotPane *pPlotPane = (CPlotPane *)pUserData;
	int iRetVal = 1; // 1 => continue normal processing of BBOX
	//PL_MOUSE *pMouse = pPlCtx->pMouse;
	//pMouse->bLeftDown
	
	if (pPlotPane != NULL) {
		switch (eEvent) {
			case PL_OBJ_EV_SEL:
				break;
			case PL_OBJ_EV_COORD:
			case PL_MOUSE_MOVE:
			default:
				break;
			case PL_OBJ_EV_EDIT:
				break;
			case PL_OBJ_EV_DONE:		//		the edit operation is about to complete
				GUI_Beep();
				break;

		};

		if (pObj != NULL) {
			if (pObj->eObjType == PL_OBJ_TY_BOX) {
				double dLeft, dBottom, dRight, dTop;

				PL_BOX_SIDE *aSides = ((PL_BOX *)pObj)->aSides;
				if (aSides != NULL) {
					if (aSides[0].bValOK) dLeft = aSides[0].dVal;
					if (aSides[1].bValOK) dBottom = aSides[1].dVal;
					if (aSides[2].bValOK) dRight = aSides[2].dVal;
					if (aSides[3].bValOK) dTop = aSides[3].dVal;
					
					if ((pPlotPane->mpPlotDlg) != NULL) {
						iRetVal = (pPlotPane->mpPlotDlg)->NotifyBoxEdgePos(dLeft, dRight, dTop, dBottom);
					}
				}
			}
		}
	}	
	
	return(iRetVal);
}


void CPlotPane::DeletePlObj()
{
	BOOL bNoCallBack = TRUE;

	int iStatus = PlObjDeleteAll(&mPlCtx, bNoCallBack);
	if (iStatus != PL_OK) {
		GUI_Beep();
	}
}


void CPlotPane::UpdateDisplay(PL_CTX *pPlCtx, BOOL bEraseFirst)
{
	CGUI_PlotPane::UpdateDisplay(pPlCtx, bEraseFirst);	// Call base class update fn.
	if (mpPlotDlg != NULL) {
	 	mpPlotDlg->UpdateEditEventDlg();	// To repaint emphasized trace as necessary.
		// Note that the emphasized trace is temporary and not stored as an "event" so
		// will not update when the screen is repainted.
		
		// This call to UpdateDisplay may have been the result of scrolling through zoomed
		// data and therefore the y scale may change so any existing edit box neds to be
		// redrawn to the full graph height.
		// Antother possibility is that the user switched between log and linear which also
		// requires redrawing any existing edit box.
		mpPlotDlg->NotifyGraphHeightChange();
	}
}


void CPlotPane::SetPlotObjOptions(BOOL bVisible, BOOL bEnbResize, BOOL bEnbBoxNew, BOOL bEnbMove)
{
	int iStatus;

	PL_OBJ_MODE PlotObjMode;
	iStatus = PlObjMode(&mPlCtx, 1, 0, &PlotObjMode);	// Get current mode flags.

	if (iStatus == PL_OK) {
		//eCreateObjType;	// current 'create type', PL_OBJ_TY_xxx
		PlotObjMode.bEditMode = TRUE;		// 1 to allow editing objects
		// The following bEnbXxx are ignored if bEnbEdit is 0.
		PlotObjMode.bEnbBoxNew = bEnbBoxNew;		// 1 allows creating PL_BOXs
		PlotObjMode.bEnbBoxSpan = FALSE;	// 1 allows PL_BOX spanning plot areas
		PlotObjMode.bEnbBoxMany = FALSE;	// 1 allows multiple PL_BOXs
		PlotObjMode.bEnbLabelNew = FALSE;	// 1 allows creating PL_LABELSs
		// The following are 'global' overrides.
		PlotObjMode.bEnbMove = bEnbMove;		// 1 allows moving even if props.bNoMove is 0
		PlotObjMode.bEnbResize = bEnbResize;		// 1 allows resizing even if props.bNoResize is 0
		PlotObjMode.bEnbDelete = FALSE;		// 1 allows deleting even if props.bNoDelete is 0
		// The following are used when the keyboard is used to move/resize
		// an object.
		//PlotObjMode.bValuesAsPx;	// 1 (or 0) if values are data (or pixels)
		//PlotObjMode.dXFine;			// size of "fine" horizontal move
		//PlotObjMode.dXCoarse;		// size of "coarse" horizontal move
		//PlotObjMode.dYFine;			// size of "fine" vertical move
		//PlotObjMode.dYCoarse;		// size of "coarse" vertical move

		iStatus = PlObjMode(&mPlCtx, 1, 1, &PlotObjMode);	// Set new mode flags.
	}
}



BOOL CPlotPane::InsertNewArea(UINT uiAreaIndex, BOOL bLogY_Axis)
{
	UINT uiDummyNewAreaIndex;	// Dummy because it will be wherever we inserted it.
	BOOL bSuccess = TRUE;
	
	InsertArea(uiAreaIndex, &uiDummyNewAreaIndex);
	SetLogY(uiDummyNewAreaIndex, bLogY_Axis);
	return(bSuccess);
}


BOOL CPlotPane::DeleteArea(UINT uiAreaIndex)
{
	BOOL bSuccess = DeleteArea(uiAreaIndex);
	return(bSuccess);
}

// changed with DATE
//BOOL CPlotPane::UpdateAxisEvents(UINT uiAxisIndex, BOOL bRefreshDisplay, ULONG ulNumEvents, EVENTS *aEvents)
BOOL CPlotPane::UpdateAxisEvents(UINT uiAxisIndex, BOOL bRefreshDisplay, ULONG ulNumEvents, EVENTS1 *aEvents)
{
	BOOL bSuccess = TRUE;	// set to true by SFK
	
	ClearHiliteRegions(uiAxisIndex);
	for (ULONG ul = 0; ul < ulNumEvents; ul++) {
//		AddHiliteRegion(uiAxisIndex, aEvents[ul].ulRelDateFirst, aEvents[ul].ulRelDateLast, aEvents[ul].Color, aEvents[ul].Label);
		AddHiliteRegion(uiAxisIndex, aEvents[ul].dRelDateFirst, aEvents[ul].dRelDateLast, aEvents[ul].Color, aEvents[ul].Label);
	}
	return(bSuccess);
}

// Added in 2.06f to support graphing doubles
// Plots the raw data as doubles, then the events and then the marks.
// Changed with DATE
BOOL CPlotPane::UpdateAxisData(UINT uiAxisIndex, double huge *daCounts, double huge *daDates,
	ULONG ulNumPts, const CGStr &DetName, BOOL bRefreshDisplay,	ULONG ulNumEvents,
	EVENTS1 *aEvents, ULONG ulNumMarks, MARKS1 *aMarks)
{
	OutputDebugString("CPlotPane::UpdateAxisData(double,double)\n");

	const BOOL bCOPY_DATA_TRUE = TRUE;
	BOOL bSuccess;
	
	bSuccess = UpdateAreaData(uiAxisIndex, daDates, daCounts, ulNumPts,
		bCOPY_DATA_TRUE, DetName, bRefreshDisplay);

	ClearHiliteRegions(uiAxisIndex);
	for (ULONG ul = 0; ul < ulNumEvents; ul++) {
//		AddHiliteRegion(uiAxisIndex, aEvents[ul].ulRelDateFirst, aEvents[ul].ulRelDateLast, aEvents[ul].Color, aEvents[ul].Label);
		AddHiliteRegion(uiAxisIndex, aEvents[ul].dRelDateFirst, aEvents[ul].dRelDateLast, aEvents[ul].Color, aEvents[ul].Label);
	}

	ClearMarkers(uiAxisIndex);
	CGStr MarkerLabel;
	
	if (mpPlotDlg != NULL) {
		for (ul = 0; ul < ulNumMarks; ul++) {
//			MarkerLabel = mpPlotDlg->RelDateToTimeStr(aMarks[ul].ulRelDateSec);
//			AddMarker(uiAxisIndex, aMarks[ul].ulRelDateSec, MarkerLabel, CPlotDlg::mMarkerTextColor);
			MarkerLabel = mpPlotDlg->RelDateToTimeStr(aMarks[ul].dRelDateSec);
			AddMarker(uiAxisIndex, aMarks[ul].dRelDateSec, MarkerLabel, CPlotDlg::mMarkerTextColor);
		}
	}

	return(bSuccess);
}

// Plots the raw data as floats, then the events and then the marks.
// Changed with DATE
BOOL CPlotPane::UpdateAxisData(UINT uiAxisIndex, float huge *faCounts, float huge *faDates,
	ULONG ulNumPts, const CGStr &DetName, BOOL bRefreshDisplay,	ULONG ulNumEvents,
	EVENTS1 *aEvents, ULONG ulNumMarks, MARKS1 *aMarks)
{
	const BOOL bCOPY_DATA_TRUE = TRUE;
	BOOL bSuccess;
	
	bSuccess = UpdateAreaData(uiAxisIndex, faDates, faCounts, ulNumPts,
		bCOPY_DATA_TRUE, DetName, bRefreshDisplay);

	ClearHiliteRegions(uiAxisIndex);
	for (ULONG ul = 0; ul < ulNumEvents; ul++) {
//		AddHiliteRegion(uiAxisIndex, aEvents[ul].ulRelDateFirst, aEvents[ul].ulRelDateLast, aEvents[ul].Color, aEvents[ul].Label);
		AddHiliteRegion(uiAxisIndex, aEvents[ul].dRelDateFirst, aEvents[ul].dRelDateLast, aEvents[ul].Color, aEvents[ul].Label);
	}

	ClearMarkers(uiAxisIndex);
	CGStr MarkerLabel;
	
	if (mpPlotDlg != NULL) {
		for (ul = 0; ul < ulNumMarks; ul++) {
//			MarkerLabel = mpPlotDlg->RelDateToTimeStr(aMarks[ul].ulRelDateSec);
//			AddMarker(uiAxisIndex, aMarks[ul].ulRelDateSec, MarkerLabel, CPlotDlg::mMarkerTextColor);
			MarkerLabel = mpPlotDlg->RelDateToTimeStr(aMarks[ul].dRelDateSec);
			AddMarker(uiAxisIndex, aMarks[ul].dRelDateSec, MarkerLabel, CPlotDlg::mMarkerTextColor);
		}
	}

	return(bSuccess);
}

// Select the area in which the cursor was clicked as the current area.  Also perform
// any special action previously assigned to a right mouse click.
BOOL CPlotPane::ServiceRightMouseBtn(UINT uiWhichAxis, double dXValue, double dYValue)
{
	CGFmtStr Msg;
	
	BOOL bVisible = TRUE;	// Get only visible (vs. full) range.
	double dXLeft, dXRight, dYBot, dYTop;
	int iStatus;
	BOOL bValidHit = FALSE;
	BOOL bFullyServiced = TRUE;	// Avoid Plot lib handling of right mouse btn (popup help window)

	// the cursor position passed in is that of the mouse pointer but in this case we
	// want the plot cursor position so...
	GetCursorPos(&dXValue, &dYValue, &uiWhichAxis);

	// Select hit axis.
	UINT uiSavedCurrentArea = GetCurrentArea();
	iStatus = PlAreaArraySelect(&mPlCtx, uiWhichAxis, 0);
	if (iStatus == PL_OK) {
		iStatus = PlAreaRangeGet(&mPlCtx, bVisible, &dXLeft, &dXRight, &dYBot, &dYTop);
		if (iStatus == PL_OK) {
			if ((dXValue >= dXLeft) && (dXValue <= dXRight) && (dYValue >= dYBot) && (dYValue <= dYTop)) {
				bValidHit = TRUE;
				if (mRightMouseCallBackFn != NULL) {
					(*mRightMouseCallBackFn)(dXValue, dYValue, uiWhichAxis);
				}
			}
			else GUI_Beep();
		}
		// Reselect current axis.
		SetCurrentArea(uiSavedCurrentArea);
	}
	return(bFullyServiced);
}

// For the given value dX get a formatted string representing what the Y axis label will be.
// 03-11-2003  Changed to print 2 decimal places with numbers less than 10 rather than less than 1
// 11-05-2003  Changed to print 3 decimal places with numbers less than 10 rather than less than 1
void CPlotPane::GetAnnotationY(double dX, CGStr *pAnnotationStr, BOOL bForCursorValue)
{
	static CGFmtStr Annotation;
	double dXLeft, dXRight;
	double dYBotVisible, dYTopVisible, dYBotFull, dYTopFull;
    BOOL bVisible;
    BOOL bDone = FALSE;
	PL_CTX *pCtx = GetPlotContext();
	BOOL bLogAxis = pCtx->pYAxis->eScaleType == 1;
        
	// Get full axis range in case we are auto-scaling to entire data set.
	bVisible = TRUE;
	PlAreaRangeGet(pCtx, bVisible, &dXLeft, &dXRight, &dYBotVisible, &dYTopVisible);
	// Get visible axis range in case we are auto-scaling to visible part only.
	bVisible = FALSE;
	PlAreaRangeGet(pCtx, bVisible, &dXLeft, &dXRight, &dYBotFull, &dYTopFull);
		
	if ((bLogAxis) && (!bForCursorValue)) {
		// Log axis.
		// Tell plot library to use format specified previously for axis style.
		// This only works if we are labeling an axis -- cursor value display
		// doesn't get passed back to PL lib so we have to take care of it ourselves.
		Annotation = PL_FMT_DFLT;
	}
	else {
		// Annotate only top and bottom grid lines to avoid clutter.
		// If we are getting a vlue for the cursor readout always return a value string.
		if ((bForCursorValue) || (dX == dYBotFull) || (dX == dYBotVisible) || (dX == dYTopFull) || (dX == dYTopVisible)) {
			// First pick off nice easy numbers and return a nice, aesthetic annotation.
			if (dX == 100.0) {
				bDone = TRUE;
				Annotation = "100";
			}
			else if (dX == 10.0) {
				bDone = TRUE;
				Annotation = "10";
			}
			else if (dX == 1.0) {
				bDone = TRUE;
				Annotation = "1";
			}
			else if (dX == 0.1) {
				bDone = TRUE;
				Annotation = ".1";
			}
			else if (dX == 0.01) {
				bDone = TRUE;
				Annotation = ".01";
			}
			else if (dX == 0.0) {
				bDone = TRUE;
				Annotation = "0";
			}
			else if ((bLogAxis) && (!bForCursorValue)) {
				// Log axis.
				// Tell plot library to use format specified previously for axis style.
				// This only works if we are labeling an axis -- cursor value display
				// doesn't get passed back to PL lib so we have to take care of it ourselves.
				Annotation = PL_FMT_DFLT;
				bDone = TRUE;
			}
	
			// If we had no "hit" above use "normal" formatting with a reasonable number of decimal
			// places based on the size of the number.
			if (!bDone) {
			    char *szFmt;
		
				if (dX > 10.0) {
					szFmt = "%.0f";
				}
				else if (dX > 0.1) {
					szFmt = "%.3f";
				}
				else if (dX > 0.01) {
					szFmt = "%.3f";
				}
				else if (dX > 0.001) {
					szFmt = "%.4f";
				}
				else {
					szFmt = "%.2e";
				}
				Annotation.Printf(szFmt, dX);
			}
		}
		else {
			// If we are not at one of the axes, (i.e. we are on one of the decade
			// dotted lines on a log plot) there is no annotation.
			Annotation = "";
		}
	}
	
	*pAnnotationStr = Annotation;
}

// For the given value dValueX get a formatted string representing what the X axis label
// will be (a date/time string)
// Changed with DATE
void CPlotPane::GetAnnotationX(DATE dValueX, CGStr *pAnnotationStr, BOOL bForCursorValue)
{
	static CGFmtStr Annotation;
	
//	long lTime = (long)dValueX;
//	const char *pszDateStr = mpPlotDlg->RelDateToDateAndTimeStr(lTime);
	const char *pszDateStr = mpPlotDlg->RelDateToDateAndTimeStr(dValueX);
	Annotation = pszDateStr;

	*pAnnotationStr = Annotation;
}


// If the builtin user interface of "X1-9" causes the X zoom factor to change
// this function gets called and updates the dialog display.

void CPlotPane::ZoomChangeNotify(UINT uiNewZoomFactorX)
{
	if (mpPlotDlg != NULL) mpPlotDlg->SetZoomFactor(uiNewZoomFactorX);
}

// Handle Alt-LeftArrow?, Alt-RightArrow?, n, N, p and P.
// These are special keys in addition to those in GUI-PlotPane?
LRESULT CPlotPane::PlotLibEventPreFilter(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT RetValue = 0;	// Always allow plot library to handle messages.

	switch (iMsg) {
		case WM_SYSKEYDOWN:		// Intercept Alt-key combinations.
			switch (wParam) {
				case VK_LEFT:
					if (!PrevHiliteRegion()) {
						GUI_MsgBox("There are no earlier events\nin the current view period.");
					}
					break;
				case VK_RIGHT: 
					if (!NextHiliteRegion()) {
						GUI_MsgBox("There are no later events\nin the current view period.");
					}
					break;
			}
			break;
		case WM_CHAR:
			switch (wParam) {
				case 'n':
				case 'N':
					if (mpPlotDlg) mpPlotDlg->NextView();
					break;
				case 'p':
				case 'P':
					if (mpPlotDlg) mpPlotDlg->PrevView();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}	

	// Call base class function.
	RetValue = CGUI_PlotPane::PlotLibEventPreFilter(hWnd, iMsg, wParam, lParam);
	return(RetValue);
}


// ?? what is this doing
BOOL CPlotPane::DisplayCursorReadoutText(PL_CTX *pPlCtx, const CGStr &AreaName,
	UINT uiCursorArea, double dCursorX, double dCursorY, BOOL bInterpolatedY,
	BOOL bCursorInRange, CGFmtStr *pCursorReadoutText, COLORREF *pCursorReadoutColor)
{
	BOOL bTextAlreadyDisplayed = FALSE;		// Allow base class to do the actual text out.
    UINT uiAreaIndex;
    double dMarkerLocX;
	double dLowValueX, dHighValueX;
	COLORREF HiliteColor;
	COLORREF MarkerColor;
	CGStr Label;

	// determine whether the cursor is on a Mark or in an Event
	BOOL bCursorOnMarker = IsCursorOnMarker(&uiAreaIndex, &dMarkerLocX, &MarkerColor);
	BOOL bCursorInHiliteRegion = IsCursorInHiliteRegion(&uiAreaIndex, &dLowValueX,
		&dHighValueX, &HiliteColor, &Label);
	
	if (mpPlotDlg != NULL) {
		// First time through here set state to !state to make sure initial
		// state is echoed properly.
		if (mbFirstDisplayCursorReadoutText) {
			mbFirstDisplayCursorReadoutText = FALSE;
			mpPlotDlg->SetCursorOnMarkerFlag(!bCursorOnMarker);
			mpPlotDlg->SetCursorInHiliteRegionFlag(!bCursorInHiliteRegion);
		}
		
		// Notify dialog window when the cursor moves on/off a marker so that it can
		// change the appropriate button label etc.
		mpPlotDlg->SetCursorOnMarkerFlag(bCursorOnMarker);
		// Notify dialog window when the cursor moves in/out of a hilite region so that it can
		// change the appropriae button label etc.
		mpPlotDlg->SetCursorInHiliteRegionFlag(bCursorInHiliteRegion);

		// Update edit-event dlg fields as needed.
		mpPlotDlg->UpdateEditEventDlg();
	}
		
	if (bCursorOnMarker) {
		*pCursorReadoutText += " MARKER";
		*pCursorReadoutColor = MarkerColor;
	}
	else if (bCursorInHiliteRegion) {
		*pCursorReadoutText += " ";
		*pCursorReadoutText += Label;
		*pCursorReadoutColor = HiliteColor;
	}

	return(bTextAlreadyDisplayed);
}


void CPlotPane::ResizeNotifyFn(CGUI_Dlg *pDlg, int iDeltaDlgWidth,	int iDeltaDlgHeight)
{
	// Pass on to base class function for most of the processing.
	CGUI_PlotPane::ResizeNotifyFn(pDlg, iDeltaDlgWidth, iDeltaDlgHeight);
	// Then let dialog box hide/show controls as real estate permits.
	if (mpPlotDlg != NULL) mpPlotDlg->ResizeNotifyFn(pDlg, iDeltaDlgWidth, iDeltaDlgHeight);
}


