///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : Plots.cpp     
//Primary Author: Shirley Klosterbuer/Tom Marks

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include <gui_lib.h>
#include "resource.h"
#include "plt_dlg.h"        
#include "FacilityConfig.h"
#include "RadError.h"	//SFK
#include "RadReview.h"  //SFK
#include "Utilities.h"
#include "MyDateTime.h"

extern RADReview *pRAD_Review;	// SFK

#pragma pack(1)	//SFK
#include "database\db_main.h"
#pragma pack(8)

#include "performtimerforGuiLib.h"
void ClearPerformanceTimer();
void DisplayPerformanceTimer();

static CPlotDlg *pPlotDlg = NULL;
static BOOL bProgramCloseInProgress = FALSE;

static BOOL CPlotDlgExitValidateFn(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);
static void RightMouseCallBackFn(double dX, double dY, UINT uiAxisNum);



void GraphData(void)
{
//	ClearPerformanceTimer();
	SetCurrentDrive();
	if (pPlotDlg != NULL) 
	{
		RadReviewMsg(uiONLY_ONE_GRAPH_WARN);
	}
	else 
	{
		HCURSOR OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));
		BOOL bValidPlotDlg;
		
		pPlotDlg = new CPlotDlg(CPlotDlgExitValidateFn, &bValidPlotDlg, RightMouseCallBackFn);
		// If CPlotDlg constructor fails because of e.g. a bad DB the pPlotDlg "closes" which calls
		// GraphExitNotifyFn() above. However, we have not returned from the new CPlotDlg call above
		// yet so pPlotDlg is still NULL and nothing happens. This then leaves pPlotDlg != NULL
		// and the next time we call GraphData we get the "Only one..." message above. So...
		if (!bValidPlotDlg) 
		{
			delete pPlotDlg;
			pPlotDlg = NULL;
		}
		::SetCursor(OldCur);
	}
//	DisplayPerformanceTimer();
}			


void CloseDataGraph(void)
{                        
	if (pPlotDlg != NULL) { 
		bProgramCloseInProgress = TRUE;
//		ShowWindow(pPlotDlg->GetWndHandle(), SW_RESTORE);
		pPlotDlg->Close();	// Causes GraphExitNotifyFn to be called.
		bProgramCloseInProgress = FALSE;
		// Since we set bProgramCloseInProgress = TRUE before calling
		// pPlotDlg->Close (which causes GraphExitNotifyFn to be called)
		// we blocked the deletion of pPlotDlg in GraphExitNotifyFn

		// so we need to delete it here.
		delete pPlotDlg;
		pPlotDlg = NULL;
	}
}


static BOOL CPlotDlgExitValidateFn(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction)
{
	BOOL bMayClose = TRUE;
	
	// The plot can be closed two ways:
	//	1) User clicks "close" btn on plot dlg. (mpDlg)
	//	2) Somewhere in the "parent" program CloseDataGraph is called.
	
	// In either case we need to delete pPlotDlg to clean up and free memory.
	// Note that the actual CGUI_Dlg dialog (mpDlg) deletes itself when it
	// is closed by virtue of the TRUE when calling GoModeless(TRUE);

	// We cannot delete pPlotDlg if we have a call on a member fn of
	// pPlotDlg in progress or we will pull the rug out from under
	// ourselves. 

	if (!bProgramCloseInProgress) {
		// We are here if the plot dialog is being closed by user click
		// (#1 above) so we are not in any way depending on pPlotDlg and
		// can delete it.
		delete pPlotDlg;
		pPlotDlg = NULL;
	}

	// Clean up properly by shutting down the modeless Data Match dialog box, if necessary
	if (pRAD_Review != NULL) pRAD_Review->RAD_Review_CloseMatchDataChoices();
	
	return(bMayClose);
}


static void RightMouseCallBackFn(double dX, double dY, UINT uiAxisNum)
{
	if (pPlotDlg != NULL) {
		const char *szDateStr = "";
		unsigned int uiDummyPotentialTraceIndex, uiPL_ChanIndex;
		struct IDStruct ChanID;

		szDateStr = pPlotDlg->RelDateToDateAndTimeStr(dX);
		if (pPlotDlg->TraceIndexToPotentialTraceIndex(uiAxisNum, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex)) {
			if (!pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID)) return;
		}
		if (pRAD_Review != NULL) pRAD_Review->RAD_MatchDataForChannel(szDateStr, ChanID);	//SFK
	}
}
	
// Aug 2002 Changed to DATE	
void GraphSetDisplayDate(DATE dAbsDisplayStart, DATE dReqViewPeriodDATE)
{
	if (pPlotDlg != NULL) {
		pPlotDlg->SetDisplayDateTime(dAbsDisplayStart, dReqViewPeriodDATE);
	}
}


// Aug 2002 Changed to DATE
BOOL GraphSetCursorTime(DATE dAbsCursorTime, int iPL_ChanIndex)

{
	BOOL bSuccess = FALSE;

	if (pPlotDlg != NULL) {
		bSuccess = pPlotDlg->SetDisplayCursorTime(dAbsCursorTime, iPL_ChanIndex);
		pPlotDlg->SetFocusToGraph();
	}	
	return(bSuccess);
}

                                       
// Aug 2002 Changed to DATE
void GraphGetViewPeriodParameters(DATE *pdDisplayStartTime, DATE *pdViewPeriodInDATE)
{

	if (pPlotDlg != NULL) {
		pPlotDlg->GetDisplayDateTime(pdDisplayStartTime, pdViewPeriodInDATE);
	}
}


BOOL GraphDisplayedNow()
{
	if (pPlotDlg != NULL) return TRUE;
	return FALSE;
}


// When calling UpdateGraphData(bDisplayFirstIfNecessary)
//	 if bDisplayFirstIfNecessary = TRUE:
//			if the graph is already displayed it will be redisplayed with the current data from the database.
//			if it is not displayed it will be started and hence will automatically have the current data.
//	 if bDisplayFirstIfNecessary = FALSE:
//			if the graph is already displayed it will be redisplayed with the current data from the database.
//			if it is not displayed nothing will happen.

void UpdateGraphData(BOOL bDisplayFirstIfNecessary)
{


	if (GraphDisplayedNow()) {
		if (pPlotDlg != NULL) {
			pPlotDlg->RefreshData();
		//	::SetWindowPos(pPlotDlg->GetWndHandle(), HWND_TOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
		//	pPlotDlg->SetViewPeriodParameters();
		}
	}
	else {
		if (bDisplayFirstIfNecessary) {
			GraphData();
		//	::SetWindowPos(pPlotDlg->GetWndHandle(), HWND_TOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
			// Note: starting up graph will automatically bring in current DB data.
		}
	}
	
}


