///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : Plt_Dlg.cpp     
//Primary Author: Tom Marks

#include <gui_lib.h>
#include <gen.h>
#include "plt_pane.h"
#include "plt_dlg.h"
#include "plt_db.h"
#include "plt_evnt.h"
#include "RadError.h"	//SFK
#include "resource.h"
#include <math.h>

#include "rad.h"
#include "DateTimeUtils.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DlgImportLatest.h"
#include "ProgressBars.h"
#include "RADInit.H"
#include "Utilities.h"
#include "FacilityConfig.h" //KM
#include "MyDateTime.h"
#include "TimeAlign.h"

extern RADInit *pRAD_Init;
extern short glsFacNum; //KM
extern CMyDateTime glMyDateTime;


//#include <iostream>
#include <string>
using namespace std;

// Note about terminology (26-Mar-97):
//
// There is the potential for multiple traces. In the plot library these are called "areas".
// In this code they will be refered to as "traces" and are 0-based starting at the top-most
// trace. Each trace displays data from a detector channel in a plot library "area". At some
// future time there may be multiple traces in a single plot library area.
//
// So, for a given trace there is a (detector) channel index -- this is simply the 0-based picklist
// index for the possible (detector) channels (uiPL_ChanIndex) which can be displayed as a trace.
//
// There are uiMAX_PLOT_TRACES traces POSSIBLE (these correspond to the visible check box / 
// channel pick list pairs in the upper right of the graph dialog. The uiPotentialTraceIndex
// is the 0-based control-pair index numbered down the screen. (Potential in the sense that any
// one can have its visibility turned on / off.)
//
// Given a potential trace index we can determine if it is active (visible), which trace / area
// it is displayed on and which detector channel is selected for that trace. BOOL return is TRUE if
// uiPotentialTraceIndex is displayed. If return is FALSE, 2nd and 3rd parameters values are undefined.
// BOOL PotentialTraceIndexToTraceIndex(
//		UINT uiPotentialTraceIndex,	// dlg box picklist control group; 0 = top one, uiMAX_PLOT_TRACES - 1 = bottom one.
//      UINT *puiTraceIndex,	// which trace this potential trace is displayed on (0 = top trace).
//      UINT *puiPL_ChanIndex = NULL);	// index into picklist array of detector channel names
//
// BOOL TraceIndexToPotentialTraceIndex(
//		UINT uiTraceIndex,	// Actual <<displayed>> trace; 0 = top one.
//		UINT *puiPotentialTraceIndex,	// checkbox-picklist control group index in dlg box.
//		UINT *puiPL_ChanIndex = NULL)		// index into picklist array of detector channel names
//
// It is possible, given a uiPotentialTraceIndex, to get the detector index directly:
// uiPL_ChanIndex = mauiPickListIndex[uiPotentialTraceIndex]. Or the detector index can be obtained
// by calling either of the functins above.
//
// There is a key in the database which is unique for a given detector channel (channel
// here is in the sense of data channel; NOT POTENTIAL channels to plot) index.
// This key is obtained by calling the DB function GetChannelForPlNum(uiPL_ChanIndex);
// uiPL_ChanIndex is the index into the array of detector names returned by
// CPlotDB::GetDetName(uiDet, &DetName). The return value from GetChannelForPlNum is the DB
// key/chan index (uiDB_ChanIndex).


COLORREF CPlotDlg::mMarkerTextColor = GUI_RED;

#define MINUTES_PER_DAY 1440.0
#define HOURS_PER_DAY 24.0

// structure for graph view period picklist
// changed to support DATE time units
static struct {
	const char *szPeriodLbl;
	DATE dDaysInPeriod;
} naViewPeriod[] = {
	"5 minutes", 5.0/MINUTES_PER_DAY,
	"10 minutes", 10.0/MINUTES_PER_DAY,
	"15 minutes", 15.0/MINUTES_PER_DAY,
	"30 minutes", 30.0/MINUTES_PER_DAY,
	"1 hour", 60.0/MINUTES_PER_DAY,
	"2 hours", 120.0/MINUTES_PER_DAY,
	"4 hours", 240.0/MINUTES_PER_DAY,
	"6 Hours", 360.0/MINUTES_PER_DAY,
	"8 hours", 480.0/MINUTES_PER_DAY,
	"12 hours", 12.0/HOURS_PER_DAY,
	"1 day", 24.0/HOURS_PER_DAY,
	"2 days", 48.0/HOURS_PER_DAY,
	"4 days", 4.0*24.0/HOURS_PER_DAY,
	"1 week", 7.0*24.0/HOURS_PER_DAY,
	"2 weeks", 14.0*24.0/HOURS_PER_DAY,
	"31 days", 31.0L*24.0/HOURS_PER_DAY,
	"70 days", 70.0*24.0/HOURS_PER_DAY,
	"110 days", 110.0*24.0/HOURS_PER_DAY,
	NULL, 0L};
	

CPlotDlg::CPlotDlg(
		BOOL (*GraphDlgExitNotifyFn)(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction),
	    BOOL *pbValidPlotDlg, 
		void (*RightMouseCallBackFn)(double X, double Y, UINT uiAxis))
{
	OutputDebugString("CPlotDlg::CTOR\n");

	BOOL bSuccess = FALSE;
	mpPlotPane = NULL;
	mpPlotDB = NULL;
	mpHelpDlg = NULL;
	mpOptionsDlg = NULL;
	mpEditEvents = NULL;
	mbCursorInHiliteRegion = FALSE;
	mbCursorOnMarker = FALSE;
	mbLogY_Axis = FALSE;
	miDspStartLineIncrementMins = 1;
	mdViewPeriodDays = naViewPeriod[uiINIT_VIEW_PERIOD_INDEX].dDaysInPeriod;
	muiZoomFactor = 1;
	mbChansOverlapped = FALSE;
	mdDspStartOffsetFromDbStart = 0.0;
	mbZoomTimes100 = FALSE;
	mConfigName = "default";
	mUsingTA = false;
	mpDlg = new CGUI_Dlg(IDD_GRAPH_DISPLAY, NULL, DLG_POSN_SAVE_ABS);

	if ((mpDlg != NULL) && (mpDlg->IsValid()))
	{
		mpDlg->SetDlgExitValidateFn(GraphDlgExitNotifyFn);
		BOOL bValidDB;
		mpPlotPane = new CPlotPane(mpDlg, this, IDC_PLOT_WIN, RightMouseCallBackFn);

		if (mpPlotPane != NULL)
		{ 
			mpPlotDB = new CPlotDB(&bValidDB);

			if (bValidDB) 
			{
				InitializeGraphDlgCtrls();
				// Setting graph options only takes effect if there is a change so set member
				// variables to their oppositie states and then "set" them to the desired state.
				BOOL bLogY_Axis = mbLogY_Axis;
				mbLogY_Axis = !mbLogY_Axis;
				SetLogLinearStyle(bLogY_Axis);
				SetZoomFactor(muiZoomFactor);	// ReadGraphSetup() sets muiZoomFactor.

				mpDlg->SetFocus();
				mpDlg->GoModeless(TRUE);
				bSuccess = TRUE;
				mpPlotPane->SetFocus();

				//Added code to put facility name into the title bar of the plot dialog.
				//PJM 12/21/2007
				//SCR 537
				char szFacilityName[DB_NAME_LEN+1];
				pglFacCfg->GetFacilityLongName(glsFacNum, szFacilityName, DB_NAME_LEN);
				string rr("Radiation Review: ");
				string facnam(szFacilityName);
				string title = rr + facnam;
				if (mUsingTA)
					title.append("  with station time adjustment*");
				const char *pch = title.c_str();
				mpDlg->SetTitle(pch);
			}
			else 
			{
				delete mpPlotDB;
				mpPlotDB = NULL;
			}
		}
		else 
		{
			GUI_MsgBox("Invalid plot pane.\nGraphing skipped.", GUI_ICON_ERROR);
		}
	}

	if (!bSuccess) 
	{
		delete mpDlg;
		mpDlg = NULL;
	}
	
	*pbValidPlotDlg = bSuccess;
}



CPlotDlg::~CPlotDlg()
{   
	// if the user has minimized the graph, restore it before you close the graph
	// added by SFK 3/11/99
	if ((mpDlg != NULL) && (IsIconic(mpDlg->GetWndHandle()))) {
		ShowWindow(mpDlg->GetWndHandle(), SW_RESTORE);
	}


	if (mpHelpDlg != NULL) 	{
		mpHelpDlg->Close(GUI_CANCEL);
		mpHelpDlg = NULL;
	}

	if (mpEditEvents != NULL) {
		delete mpEditEvents;
		mpEditEvents = NULL;
	}

	if (mpOptionsDlg != NULL) {
		delete mpOptionsDlg;
		mpOptionsDlg = NULL;
	}
	// I don't understand it but if SaveGraphSetup is called after deleting mpPlotPane
	// we have problems. It's not clear that SaveGraphSetup needs anything from
	// mpPlotPane but perhaps close investigation would reveal that it does.
	// In any case given time constraints call SaveGraphSetup before deleting
	// mpPlotPane. Tom 30-Jun-97
		
    SaveGraphSetup(&mConfigName);	// Save current settings
	mConfigName = "default";		// Save settings as the defaults also
	SaveGraphSetup(&mConfigName);	// Save current settings.

	if (mpPlotPane != NULL) {
		delete mpPlotPane;
		mpPlotPane = NULL;
	}
    
	if (mpPlotDB != NULL) {
		delete mpPlotDB;
		mpPlotDB = NULL;
	}

//	if (mpDlg != NULL) {
//		delete mpDlg;
//		mpDlg = NULL;
//	}
}



void CPlotDlg::InitializeGraphDlgCtrls()
{
	// initialize all the stuff for the two scroll bars
	UINT uiViewPeriodSelection = uiINIT_VIEW_PERIOD_INDEX;
	int iZoomFactorSB_Value = 0;
	int iViewPeriodSB_Value = 0;
	BOOL bUpdateIsSuppressed = FALSE;
	const int iVIEW_PERIOD_SB_MIN = 0;
	const int iVIEW_PERIOD_SB_MAX = 100;
	const int iVIEW_PERIOD_SB_LINE_INCR = 2;
	const int iVIEW_PERIOD_SB_PAGE_INCR = 20;
	const int iZOOM_FACTOR_SB_MIN = 0;
	const int iZOOM_FACTOR_SB_MAX = uiMAX_ZOOM_FACTOR;
	const int iZOOM_FACTOR_SB_LINE_INCR = 1;
	const int iZOOM_FACTOR_SB_PAGE_INCR = 4;
	const double dTHUMB_FRAC = 0.0;
	UINT ui = 0;
	
	// map the graph trace checkboxes and picklists to variables
	maCheckBoxID[0] = IDC_DET_ONE;
	maPickListID[0] = IDC_DET_PICK_ONE;
	maCheckBoxID[1] = IDC_DET_TWO;
	maPickListID[1] = IDC_DET_PICK_TWO;
	maCheckBoxID[2] = IDC_DET_THREE;
	maPickListID[2] = IDC_DET_PICK_THREE;
	maCheckBoxID[3] = IDC_DET_FOUR;
	maPickListID[3] = IDC_DET_PICK_FOUR;
	maCheckBoxID[4] = IDC_DET_FIVE;
	maPickListID[4] = IDC_DET_PICK_FIVE;
	maCheckBoxID[5] = IDC_DET_SIX;
	maPickListID[5] = IDC_DET_PICK_SIX;
	maCheckBoxID[6] = IDC_DET_SEVEN;
	maPickListID[6] = IDC_DET_PICK_SEVEN;
	maCheckBoxID[7] = IDC_DET_EIGHT;
	maPickListID[7] = IDC_DET_PICK_EIGHT;
	maCheckBoxID[8] = IDC_DET_NINE;
	maPickListID[8] = IDC_DET_PICK_NINE;
	maCheckBoxID[9] = IDC_DET_TEN;
	maPickListID[9] = IDC_DET_PICK_TEN;

	muiNumLwrRtCtrls = 23;	// this is a hardwired number
	if (muiNumLwrRtCtrls > uiMAX_LWR_RT_CTRLS) {
		GUI_MsgBox("Internal error: Too many\nlower-right controls.", GUI_ICON_ERROR);
	}

	// map all the other resource controls to a variable
	// and get them registered
	maLwrRtCtrlID[0] = IDC_FINISHED;
	maLwrRtCtrlID[1] = IDC_MY_HELP;
	maLwrRtCtrlID[2] = IDC_MARK;
	maLwrRtCtrlID[3] = IDC_OPTIONS;
	maLwrRtCtrlID[4] = IDC_ZOOM_OUT;
	maLwrRtCtrlID[5] = IDC_ZOOM_IN;
	maLwrRtCtrlID[6] = IDC_NEXT_VIEW;
	maLwrRtCtrlID[7] = IDC_PREV_VIEW;
	maLwrRtCtrlID[8] = IDC_VIEW_PERIOD_PL;
	maLwrRtCtrlID[9] = IDC_VIEW_PERIOD_LBL;
	maLwrRtCtrlID[10] = IDC_VIEW_PERIOD_SB;
	maLwrRtCtrlID[11] = IDC_ZOOM_FACTOR_DSP;
	maLwrRtCtrlID[12] = IDC_ZOOM_FACTOR_SB;
	maLwrRtCtrlID[13] = IDC_ZOOM_LBL;
	maLwrRtCtrlID[14] = IDC_VIEW_GROUP_BOX;
	maLwrRtCtrlID[15] = IDC_WHICH_VIEW_DSP;
	maLwrRtCtrlID[16] = IDC_SUPRESS_UPDATE;
	maLwrRtCtrlID[17] = IDC_DSP_BEGIN;
	maLwrRtCtrlID[18] = IDC_ZOOM_TIMES_100;
	maLwrRtCtrlID[19] = IDC_ZOOM_AUTO_SCALE;
	maLwrRtCtrlID[20] = IDC_PRINT_GRAPH;
	maLwrRtCtrlID[21] = IDC_LOG_Y;
	maLwrRtCtrlID[22] = IDC_EVENT;
	// Set "glue" for right hand controls for resizing of dialog box.
	if (mpPlotPane != NULL)  {
		mpPlotPane->RegisterBottomRightCtrls(maLwrRtCtrlID, muiNumLwrRtCtrls);
		mpPlotPane->RegisterRightCtrls(maPickListID, uiMAX_PLOT_TRACES);
		mpPlotPane->RegisterRightCtrls(maCheckBoxID, uiMAX_PLOT_TRACES);
	}
	
	// Now define all the controls in the dialog box
	if (mpDlg != NULL) {
		// Define a cancel btn so that "close option" in the dlg system
		// menu is available. Make it _WITH_VERIFY to trap errant
		// escape key hits. 
		mpDlg->DefinePushBtn(IDCANCEL, GUI_CANCEL_WITH_VERIFY);
		mpDlg->SetVisible(IDCANCEL, FALSE);

		// Define a GUI_CANCEL btn labeled "Close" which will not
		// verify operator's shutdown request.
		// See note in CPlotDlg::Close() about GUI_CANCEL vs. GUI_OK.
		mpDlg->DefinePushBtn(IDC_FINISHED, GUI_CANCEL);


		// Define static text fields.
		mpDlg->DefineFldStaticTxt(IDC_WHICH_VIEW_DSP); 
		mpDlg->DefineFldStaticTxt(IDC_VIEW_PERIOD_LBL);
		mpDlg->DefineFldStaticTxt(IDC_ZOOM_FACTOR_DSP); 
		mpDlg->DefineFldStaticTxt(IDC_DSP_BEGIN); 
		mpDlg->DefineFldStaticTxt(IDC_ZOOM_LBL); 
					
		
		// Define Help pushbutton
		mpDlg->DefinePushBtn(IDC_MY_HELP, GUI_NONE);
		mpDlg->SetCtrlNotifyFn(IDC_MY_HELP, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::HelpBtnServiceFn);

		// Define the Mark/Update pushbutton
		mpDlg->DefinePushBtn(IDC_MARK, GUI_NONE);
		if (pRAD_Init->Get_RealTimeUpdate()!= TRUE) { 
			mpDlg->SetCtrlNotifyFn(IDC_MARK, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::MarkCursorPosition);
		}
		else {
			// SFK
			// Change the text on the pushbutton to be Update and
			// associate the update function
			mpDlg->SetText(IDC_MARK, "Update Data");	// change the label
			mpDlg->SetCtrlNotifyFn(IDC_MARK, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::RealTimeUpdateBtnServiceFn);
		}

		// Define the Edit Event pushbutton
		mpDlg->DefinePushBtn(IDC_EVENT, GUI_NONE);
		mpDlg->SetCtrlNotifyFn(IDC_EVENT, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::EditEventBtnServiceFn);

		// Define the Start Time pushbutton
		mpDlg->DefinePushBtn(IDC_OPTIONS, GUI_NONE);
		mpDlg->SetCtrlNotifyFn(IDC_OPTIONS, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::MoreOptionsServiceFn); // 2.06

		// Define the Zoom In pushbutton
		mpDlg->DefinePushBtn(IDC_ZOOM_IN, GUI_NONE);
		mpDlg->SetCtrlNotifyFn(IDC_ZOOM_IN, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::ZoomInServiceFn);
		mpDlg->SetEnable(IDC_ZOOM_IN, FALSE);
	
		// Define the box that contains all the dialog controls
		// except the pushbuttons and the trace choices
		mpDlg->DefineGroupBox(IDC_VIEW_GROUP_BOX);

		// Define the Zoom Out pushbutton
		mpDlg->DefinePushBtn(IDC_ZOOM_OUT, GUI_NONE);
		mpDlg->SetCtrlNotifyFn(IDC_ZOOM_OUT, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::ZoomOutServiceFn);

		// Define the Print pushbutton
		mpDlg->DefinePushBtn(IDC_PRINT_GRAPH, GUI_NONE);
		mpDlg->SetCtrlNotifyFn(IDC_PRINT_GRAPH, this, (GUI_CB_DlgNotifyFn)PrintGraphServiceFn);

		// Define the log/linear checkbox
		mpDlg->DefineCheckBox(IDC_LOG_Y, &mbLogY_Axis);
		mpDlg->SetCtrlNotifyFn(IDC_LOG_Y, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::LogLinearToggleServiceFn);

		// Define the x100 checkbox
		mpDlg->DefineCheckBox(IDC_ZOOM_TIMES_100, &mbZoomTimes100);
		mpDlg->SetCtrlNotifyFn(IDC_ZOOM_TIMES_100, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::ZoomTimes100ServiceFn);

		// Define the zoom autoscale checkbox
		mpDlg->DefineCheckBox(IDC_ZOOM_AUTO_SCALE, &mbZoomAutoScale);
		mpDlg->SetCtrlNotifyFn(IDC_ZOOM_AUTO_SCALE, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::ZoomAutoScaleServiceFn);

		// Define the view period scroll bar
		mpDlg->DefineScrollBar(IDC_VIEW_PERIOD_SB, &iViewPeriodSB_Value, iVIEW_PERIOD_SB_MIN, iVIEW_PERIOD_SB_MAX, iVIEW_PERIOD_SB_LINE_INCR,	iVIEW_PERIOD_SB_PAGE_INCR, dTHUMB_FRAC);
		mpDlg->SetCtrlNotifyFn(IDC_VIEW_PERIOD_SB, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::ViewPeriodSB_NotifyFn);
		mpDlg->SetEnable(IDC_VIEW_PERIOD_SB, FALSE);
		mpDlg->ScrollBarSetTrackingFn(IDC_VIEW_PERIOD_SB, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::ViewPeriodSB_TrackingFn);

		// Define the zoom bar scroll bar
		mpDlg->DefineScrollBar(IDC_ZOOM_FACTOR_SB, &iZoomFactorSB_Value, iZOOM_FACTOR_SB_MIN, iZOOM_FACTOR_SB_MAX, iZOOM_FACTOR_SB_LINE_INCR, iZOOM_FACTOR_SB_PAGE_INCR, dTHUMB_FRAC);
		mpDlg->SetCtrlNotifyFn(IDC_ZOOM_FACTOR_SB, this,(GUI_CB_DlgNotifyFn)&CPlotDlg::ZoomFactorSB_NotifyFn);
		mpDlg->ScrollBarSetTrackingFn(IDC_ZOOM_FACTOR_SB, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::ZoomFactorSB_TrackingFn);

		// Define the Prev pushbutton
		mpDlg->DefinePushBtn(IDC_PREV_VIEW, GUI_NONE);
		mpDlg->SetCtrlNotifyFn(IDC_PREV_VIEW, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::PrevViewServiceFn);

		// Define the Next pushbutton
		mpDlg->DefinePushBtn(IDC_NEXT_VIEW, GUI_NONE);
		mpDlg->SetCtrlNotifyFn(IDC_NEXT_VIEW, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::NextViewServiceFn);
	
		// Define the View Period picklist controls
		mpDlg->DefinePickList(IDC_VIEW_PERIOD_PL, NULL, &uiViewPeriodSelection, NULL);
		while (naViewPeriod[ui].szPeriodLbl != NULL) {
			mpDlg->PickListAdd(IDC_VIEW_PERIOD_PL, naViewPeriod[ui].szPeriodLbl);
			ui++;
		}
		mpDlg->SetCtrlNotifyFn(IDC_VIEW_PERIOD_PL, this,
			(GUI_CB_DlgNotifyFn)&CPlotDlg::ViewPeriodNotifyFn);
			

		// As a default, turn on display of first 4 detectors or all detectors
		// if there are fewer than 4 of them.
		// Default the values for showing them (mabShowDet) and their PL choice (mauiPicklistIndex)
		// ?? Why??  sfk
		UINT uiNumDetectors = 0;
		if (mpPlotDB != NULL) uiNumDetectors = mpPlotDB->GetNumDet();
		for (ui = 0; ui < uiMAX_PLOT_TRACES; ui++) {
			if (ui < uiNumDetectors) {
	//			if (ui < 4) {
	//				mabShowDet[ui] = TRUE;
	//			}
	//			else {
					mabShowDet[ui] = FALSE;
	//			}
				mauiPickListIndex[ui] = ui;
			}
			else {
				mabShowDet[ui] = FALSE;
				mauiPickListIndex[ui] = 0;
			}
		}
	
		// Define the Suppress Update checkbox
		mpDlg->DefineCheckBox(IDC_SUPRESS_UPDATE, &bUpdateIsSuppressed);
		mpDlg->SetCtrlNotifyFn(IDC_SUPRESS_UPDATE, this, (GUI_CB_DlgNotifyFn)SuppressUpdateCB_NotifyFn);

	
		BOOL bVisible = TRUE;
		for (ui = 0; ui < uiMAX_PLOT_TRACES; ui++) {
			//UINT uiPickListIndexSaved;
			// Define axis display checkbox..
			mpDlg->DefineCheckBox(maCheckBoxID[ui], &mabShowDet[ui]);
			mpDlg->SetCtrlNotifyFn(maCheckBoxID[ui], this, (GUI_CB_DlgNotifyFn)ChanEnableCB_NotifyFn);
			// Define and load channel name picklist.
			// If an item is added to an empty pick list GUI_LIB sets the value of
			// the pick list index to that new item (i.e. 0) which calls the
			// notify function which is this case sets mauiPickListIndex[ui] to 0.
			// So save the value and call PickListSetValue after the list is built.
			//uiPickListIndexSaved = mauiPickListIndex[ui];
	
			if (mpPlotDB != NULL) {
				CGStr DetName;
				mpDlg->DefinePickList(maPickListID[ui], NULL,&mauiPickListIndex[ui], NULL); //naszChanNames
				for (UINT uiDet = 0; uiDet < uiNumDetectors; uiDet++) {
					if (mpPlotDB->GetDetName(uiDet, &DetName)) {
						mpDlg->PickListAdd(maPickListID[ui], DetName);
					}
				}
				mpDlg->PickListSetValue(maPickListID[ui], mauiPickListIndex[ui]);
			}
			
			mpDlg->SetCtrlNotifyFn(maPickListID[ui], this, (GUI_CB_DlgNotifyFn)WhichDetPL_NotifyFn);
//			mpDlg->SetEnable(maPickListID[ui], mabShowDet[ui]);
	
//			mpDlg->SetVisible(maPickListID[ui], bVisible);
//			mpDlg->SetVisible(maCheckBoxID[ui], bVisible);
		}

		// Everything define, now read all the graph parameters from the database
		ReadGraphSetup(&mConfigName, TRUE);
	}
}


// remove all the existing plot areas
void CPlotDlg::DeleteAllVisibleChannels()
{
	UINT uiTraceIndex;
	for (UINT ui = 0; ui < uiMAX_PLOT_TRACES; ui++) {
		if (mabShowDet[ui] == TRUE) {	// currently has an associated trace
			if (PotentialTraceIndexToTraceIndex(ui, &uiTraceIndex)) {
				mpPlotPane->RemoveArea(uiTraceIndex);
				mabShowDet[ui] = FALSE;
			}
		}
	}
}


// create all the plot areas that are active now
void CPlotDlg::AddInitVisibleChannels()
{
	UINT uiTraceIndex;
	
	if (mpPlotPane != NULL) {
		for (UINT ui = 0; ui < uiMAX_PLOT_TRACES; ui++) {
			if (PotentialTraceIndexToTraceIndex(ui, &uiTraceIndex)) {
				mpPlotPane->InsertNewArea(uiTraceIndex, mbLogY_Axis);
				//mabShowDet[ui] = TRUE;	// sfk 2.06 ??
			}
		}
		RefreshData();
		mpPlotPane->SetCurrentArea(0);
		mpPlotPane->CenterCursor();	
	}
}

// this is called only when the box is resized
// if the graph size is so small there is not room for the last two trace selections
// then remove those traces so there are only 8 possible.
void CPlotDlg::HideBottomDetPickListCtrls(CGUI_Dlg *pDlg)
{
	UINT uiTenthDetLeft, uiTenthDetTop, uiTenthDetBot;
	UINT uiNinthDetLeft, uiNinthDetTop, uiNinthDetBot;
	UINT uiDetWidth, uiDetHeight;
	UINT uiViewPeriodLeft, uiViewPeriodTop;
	BOOL bTenthDetVisible, bNinthDetVisible;
	
	if (pDlg->GetCtrlSize(IDC_DET_PICK_TEN, &uiDetWidth, &uiDetHeight)) {
		if (pDlg->GetCtrlPosition(IDC_VIEW_PERIOD_PL, &uiViewPeriodLeft, &uiViewPeriodTop)) {
			if (pDlg->GetCtrlPosition(IDC_DET_PICK_TEN, &uiTenthDetLeft, &uiTenthDetTop)) {
				uiTenthDetBot = uiTenthDetTop + uiDetHeight;
				if (uiTenthDetBot >= (uiViewPeriodTop - 4)) {	// -4 for a little aesthetic room 
					// Hide tenth detector pick list and check box.
					bTenthDetVisible = FALSE;
					// And make sure it is not "on" - checked.
					// But first, if this is the only channel on turn channel 1 on before
					// turning this one off.
					BOOL bOn;
					pDlg->CheckBoxGetValue(IDC_DET_TEN, &bOn);
					if (bOn) {
						if (GetNumAreas() == 1) {
							pDlg->CheckBoxSetValue(IDC_DET_ONE, TRUE);
							ChanEnableCB_NotifyFn(pDlg, IDC_DET_ONE);
						}
						pDlg->CheckBoxSetValue(IDC_DET_TEN, FALSE);
						// CheckBoxSetValue does not cause ChanEnableCB_NotifyFn to
						// be called so call it explicitly to update graph.
						ChanEnableCB_NotifyFn(pDlg, IDC_DET_TEN);
					}
				}
				else {
					bTenthDetVisible = TRUE;
				}
				// Now hide/show detector ten picklist and checkbox.
				pDlg->SetVisible(IDC_DET_PICK_TEN, bTenthDetVisible);
				pDlg->SetVisible(IDC_DET_TEN, bTenthDetVisible);
			}
			
			// Now do the same thing for channel nine.
			if (pDlg->GetCtrlPosition(IDC_DET_PICK_NINE, &uiNinthDetLeft, &uiNinthDetTop)) {
				uiNinthDetBot = uiNinthDetTop + uiDetHeight;
				if (uiNinthDetBot >= (uiViewPeriodTop - 4)) {	// -4 for a little aesthetic room 
					// Hide tenth detector pick list and check box.
					bNinthDetVisible = FALSE;
					// And make sure it is not "on" - checked.
					// But first, if this is the only channel on turn channel 1 on before
					// turning this one off.
					BOOL bOn;
					pDlg->CheckBoxGetValue(IDC_DET_NINE, &bOn);
					if (bOn) {
						if (GetNumAreas() == 1) {
							pDlg->CheckBoxSetValue(IDC_DET_ONE, TRUE);
							ChanEnableCB_NotifyFn(pDlg, IDC_DET_ONE);
						}
						pDlg->CheckBoxSetValue(IDC_DET_NINE, FALSE);
						// CheckBoxSetValue does not cause ChanEnableCB_NotifyFn to
						// be called so call it explicitly to update graph.
						ChanEnableCB_NotifyFn(pDlg, IDC_DET_NINE);
					}


					pDlg->CheckBoxSetValue(IDC_DET_NINE, FALSE);
					ChanEnableCB_NotifyFn(pDlg, IDC_DET_NINE);
					// there is a hole here: if only channel ten is on need to turn on
					// one of the first 9 channels before we can turn off channel ten.
				}
				else {
					bNinthDetVisible = TRUE;
				}
				// Now hide/show detector nine picklist and checkbox.
				pDlg->SetVisible(IDC_DET_PICK_NINE, bNinthDetVisible);
				pDlg->SetVisible(IDC_DET_NINE, bNinthDetVisible);
			}
		}
	}
}

void CPlotDlg::SetFocusToGraph()
{
	if (mpPlotPane != NULL) mpPlotPane->SetFocus();
}

void CPlotDlg::ResizeNotifyFn(CGUI_Dlg *pDlg, int iDeltaDlgWidth, int iDeltaDlgHeight)
{           
	if (mpDlg != NULL) HideBottomDetPickListCtrls(mpDlg);
}

void CPlotDlg::UpdateGraphDisplay()
{
	if (mpPlotPane != NULL) {
		mpPlotPane->InvalidateGraphDisplay();
	}
}

int CPlotDlg::NotifyBoxEdgePos(double dLeft, double dRight, double dTop, double dBottom)
{
	int iRetVal = 1; // 1 => continue normal processing of BBOX

	if (mpEditEvents != NULL) {
		iRetVal = mpEditEvents->EventBoxEdgeMoveNotify(dLeft, dRight, dTop, dBottom);
	}
	return(iRetVal);
}

void CPlotDlg::NotifyGraphHeightChange()
{
	if (mpEditEvents != NULL) {
		mpEditEvents->ResetEventBoxHeight();
	}
}

BOOL CPlotDlg::GetPL_Index(RESOURCE_ID ControlID, UINT *puiPL_Index,
	UINT *puiActivePL_Index)
{
	BOOL bFound = FALSE;
	UINT ui = 0;
	
	*puiActivePL_Index = 0;
	while ((ui < uiMAX_PLOT_TRACES) && (!bFound)) { 
		if (maPickListID[ui] == ControlID) {
			bFound = TRUE;
			*puiPL_Index = ui;
		}
		else {
			if (mabShowDet[ui]) {
				(*puiActivePL_Index)++;
			}
		}
		ui++;
	}
	return(bFound);
}


void CPlotDlg::WhichDetPL_NotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID ControlID)
{
	UINT uiPL_Index;		// Index counting all channel pick lists.
	UINT uiActivePL_Index;	// Index counting only enabled channel pick lists.

	if (GetPL_Index(ControlID, &uiPL_Index, &uiActivePL_Index)) {
		pDlg->PickListGetValue(ControlID, &mauiPickListIndex[uiPL_Index]);
		
		UINT uiWhichAxis = uiActivePL_Index;
		if (!UpdateIsSuppressed()) {
			UpdateAxisData(uiWhichAxis, TRUE);
		}
		SetFocusToGraph();
	}
}


BOOL CPlotDlg::GetCB_Index(RESOURCE_ID ControlID, UINT *puiCB_Index)
{
	BOOL bFound = FALSE;
	UINT ui = 0;
	
	while ((ui < uiMAX_PLOT_TRACES) && (!bFound)) { 
		if (maCheckBoxID[ui] == ControlID) {
			bFound = TRUE;
			*puiCB_Index = ui;
		}
		ui++;
	}
	return(bFound);
}

// There are uiMAX_PLOT_TRACES traces (areas / axes) POSSIBLE (these correspond to the
// visible check box / detector pick list pairs in the upper right of the graph dialog.
// Given this channel index determine if it is active (visible), which area / axis it is
// displayed on and which detector is selected for that area.

BOOL CPlotDlg::PotentialTraceIndexToTraceIndex(UINT uiChanIndex, UINT *puiTraceIndex,
	UINT *puiPL_ChanIndex)
{
	BOOL bChanIsActive = FALSE;
	UINT uiNumActiveChans = 0;
		
	UINT ui = 0;

	if (mpPlotDB != NULL) {
		while ((ui < uiMAX_PLOT_TRACES) && (!bChanIsActive)) {
			if (mabShowDet[ui]) {
				if (uiChanIndex == ui) {
					bChanIsActive = TRUE;
					*puiTraceIndex = uiNumActiveChans;
					if (puiPL_ChanIndex != NULL) {
						// Make sure it is in legitimate range.
						if (mauiPickListIndex[ui] >= mpPlotDB->GetNumDet()) {
							mauiPickListIndex[ui] = 0;
						}
						*puiPL_ChanIndex = mauiPickListIndex[ui];
					}
				}
				uiNumActiveChans++;
			}
			ui++;
		}
	}
	return(bChanIsActive);
}


BOOL CPlotDlg::TraceIndexToPotentialTraceIndex(UINT uiTraceIndex, UINT *puiChanIndex,
	UINT *puiPL_ChanIndex)
{
	BOOL bFoundActiveChanIndex = FALSE;
	UINT uiNumActiveChans = 0;
		
	UINT ui = 0;
	if (mpPlotDB != NULL) {
		while ((ui < uiMAX_PLOT_TRACES) && (!bFoundActiveChanIndex)) {
			if (mabShowDet[ui]) {
				if (uiNumActiveChans == uiTraceIndex) {
					bFoundActiveChanIndex = TRUE;
					*puiChanIndex = ui;
					if (puiPL_ChanIndex != NULL) {
						// Make sure it is in legitimate range.
						if (mauiPickListIndex[ui] >= mpPlotDB->GetNumDet()) {
							mauiPickListIndex[ui] = 0;
						}
						*puiPL_ChanIndex = mauiPickListIndex[ui];
					}
				}
				uiNumActiveChans++;
			}
			ui++;
		}
	}
	return(bFoundActiveChanIndex);
}


void CPlotDlg::ChanEnableCB_NotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID ControlID)
{
	BOOL bOn;
	UINT uiCB_Index;
	
	pDlg->CheckBoxGetValue(ControlID, &bOn);

	if (mpPlotPane != NULL) {
		if ((GetNumAreas() == 1) && (!bOn)) {
			pDlg->CheckBoxSetValue(ControlID, TRUE);
			RadReviewMsg(uiDISPLAY_AT_LEAST_ONE_CHANNEL_ERR);
			//GUI_MsgBox("At least one channel\nmust be enabled.",GUI_ICON_ERROR);
		}
		else {
			if (GetCB_Index(ControlID, &uiCB_Index)) {
				// If the cursor is currently in the channel (muiCurrentIndex) which
				// is being turned off (hidden) make the 0th channel the current channel.
				// Otherwise adjust muiCurrentIndex so that it is still the same channel;
				// i.e. if we have enabled or disabled a channel above muiCurrentAxis we
				// need to increment or decrement muiCurrentAxis respectively.
				// NB: We must do this before fiddling the mabShowDet array if a channel is
				// being turned off and after fiddling the mabShowDet array otherwise
				
				UINT uiTraceIndex;
				if (!bOn) {
					// Here if turning a channel off.
					if (PotentialTraceIndexToTraceIndex(uiCB_Index, &uiTraceIndex)) {
						// If it is the currently selected channel make the current channel 0.
						if (uiTraceIndex == mpPlotPane->GetCurrentArea()) {
							mpPlotPane->SetCurrentArea(0);
						}
						else {
							if (uiTraceIndex < mpPlotPane->GetCurrentArea()) {
								// Here if we are turning off the display of a channel above
								// the currently selected channel so decrement muiCurrentAxis;
								UINT uiCurrentAxis = mpPlotPane->GetCurrentArea();
								uiCurrentAxis--;
								mpPlotPane->SetCurrentArea(uiCurrentAxis);
							}
						}
					}
					mabShowDet[uiCB_Index] = bOn;
					mpPlotPane->RemoveArea(uiTraceIndex);
				}
				else {
					// Here if we are turning on the display of a channel.
					// If we are turning on the display of a channel above the current axis
					// we want to increment the current axis so that it is the same channel
					// (but next display down).
					mabShowDet[uiCB_Index] = bOn;	// Must be before calling PotentialTraceIndexToTraceIndex()
					if (PotentialTraceIndexToTraceIndex(uiCB_Index, &uiTraceIndex)) {
						mpPlotPane->InsertNewArea(uiTraceIndex, mbLogY_Axis);
						UpdateAxisData(uiTraceIndex, TRUE);
	
						UINT uiCurrentAxis = mpPlotPane->GetCurrentArea();
						if (uiTraceIndex <= uiCurrentAxis) {
							uiCurrentAxis++;
							mpPlotPane->SetCurrentArea(uiCurrentAxis);
						}
					}
				}
	
				pDlg->SetEnable(maPickListID[uiCB_Index], bOn);
			}
		}
	}
}


void CPlotDlg::Close()
{
	if (mpDlg != NULL) {
		// Close the dialog containing the plot pane. Close the dialog as if the
		// (hidden) CANCEL button was pushed. This will cause the validation of
		// all of the controls to be skipped (as opposed to GUI_OK which causes
		// any validate fn associated with a control to be called). Either GUI_OK
		// or GUI_CANCEL causes the "DataGraphExitValidateFn" to be called assuring
		// that the contained CPlotPane is deleted.
		mpDlg->Close(GUI_CANCEL);
	}
}


/////////////////////////////////////////////////////////////
//	Cursor routines

// BOOL SetDisplayCursorTime return is on whether requested channel is currently displayed
// and does not imply anything about the validity of the time requested.
// changed with DATE
BOOL CPlotDlg::SetDisplayCursorTime(DATE dAbsCursorTime, int iPL_ChanIndex)
{
	BOOL bValidAxis = FALSE;

	if (mpDlg != NULL) {
		if (mpPlotDB != NULL) {
			DATE dDB_StartDate, dDB_EndDate;
			UINT uiAreaIndex = GUI_PLOT_CURRENT_AREA;

			if (iPL_ChanIndex >= 0) {
				// Find the area (uiTraceIndex) which corresponds to the given PickList channel index.
				BOOL bMatchFound = FALSE;
				UINT uiPotentialTraceIndex;	// dlg box picklist control group; 0 = top one, uiMAX_PLOT_TRACES - 1 = bottom one.
				UINT uiTraceIndex;	// which trace this potential trace is displayed on (0 = top trace).
				UINT uiPL_ChanIndex;	// index into picklist array of detector channel names

				uiPotentialTraceIndex = 0;
				
				while ((uiPotentialTraceIndex < uiMAX_PLOT_TRACES) && (!bMatchFound)) {
					BOOL bValidTrace = PotentialTraceIndexToTraceIndex(uiPotentialTraceIndex, &uiTraceIndex, &uiPL_ChanIndex);
					if (bValidTrace) {
						if (uiPL_ChanIndex == (UINT)iPL_ChanIndex) {
							bMatchFound = TRUE;
							bValidAxis = TRUE;
							uiAreaIndex = uiTraceIndex;
						}
					}
					uiPotentialTraceIndex++;
				}
			}
			else {
				bValidAxis = TRUE;	// since the request is for the currently selected axis.
			}
			
			mpPlotDB->GetDateRange(&dDB_StartDate, &dDB_EndDate);
			// Set the cursor on the first data point of the requested display period.
			// This is especially important if the view is greater than that requested.
			BOOL bOnNearestDataPt = TRUE;
			if (mpPlotPane != NULL) {
				mpPlotPane->SetCursorPos(dAbsCursorTime - dDB_StartDate, uiAreaIndex, bOnNearestDataPt);
			}
		}
	}
	return(bValidAxis);
}

void CPlotDlg::SetCursorInHiliteRegionFlag(BOOL bCursorInHiliteRegion)
{
	if (mbCursorInHiliteRegion != bCursorInHiliteRegion) {
		// Set the parameters
		mbCursorInHiliteRegion = bCursorInHiliteRegion;
	}
}


void CPlotDlg::SetCursorOnMarkerFlag(BOOL bCursorOnMarker)
{
	if (pRAD_Init->Get_RealTimeUpdate() == TRUE) return;	// SFK Using this button for RealTime update

	if (mbCursorOnMarker != bCursorOnMarker) {
		// Only change text on button when the cursor-on-marker state changes
		// to avoid unnecessary screen paints (every time cursor moves).
		mbCursorOnMarker = bCursorOnMarker;
		if (mpDlg != NULL) {
			if (bCursorOnMarker) mpDlg->SetText(IDC_MARK, "Un&Mark...");
			else mpDlg->SetText(IDC_MARK, "&Mark");
		}
	}
}

void CPlotDlg::SetDialogTAFlag(bool activeTA)
{
	if (!mUsingTA)
	{
		mUsingTA = activeTA;
	}
}
//////////////////////////////////////////////////////////////////////////
//	Read Graph Data

void CPlotDlg::CancelGraphReadServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID ID)
{
	mbAbortUpdate = TRUE;
}


UINT CPlotDlg::GetNumAreas()
{
	UINT uiNumAreas = 0;
	if (mpPlotPane != NULL) {
		uiNumAreas = mpPlotPane->GetNumAreas();
	}
	return(uiNumAreas);
}

// plot the data on each trace
// Changed daCounts, daDates from floats to double in 2.06f to support graphing doubles
// Changed with DATE
BOOL CPlotDlg::UpdateAxisData(
	UINT uiTraceIndex, 
	BOOL bRefreshDisplay,
	TwoLineProgressBar *pProgress,
	ULONG *pulNumPrevPts, 
	double dFracBefore, 
	double dFracAfter)
{
	OutputDebugString("CPlotDlg::UpdateAxisData\n");

	if (UpdateIsSuppressed())
		return FALSE;

	if ((mpPlotPane == NULL) || (mpPlotDB == NULL))
		return FALSE;

	if (uiTraceIndex >= GetNumAreas())
		return FALSE;

	BOOL bSuccess = FALSE;
	double huge *daCounts;
	double huge *daDates;
	ULONG ulNumPts;
	ULONG ulNumEvents;
	EVENTS1 *aEvents;
	ULONG ulNumMarks;
	MARKS1 *aMarks;
	UINT uiDummyPotentialTraceIndex;
	UINT uiPL_ChanIndex;

	// Some of the parameters passed in are pointers and can be defaulted to NULL.
	// However those are passed on to ReadChannelData which is not prepared for
	// this and tries to dereference NULL => GPF. So point those pointers to some
	// where harmless before passing them on.
	ULONG ulDummyNumPrevPts;
	if (pulNumPrevPts == NULL) 
		pulNumPrevPts = &ulDummyNumPrevPts;

	GUI_SetCursorShape(GUI_CURSOR_HOURGLASS);

	if (TraceIndexToPotentialTraceIndex(uiTraceIndex, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex)) 
	{
		bSuccess = mpPlotDB->ReadChannelData(uiPL_ChanIndex, mdDspStartOffsetFromDbStart, mdViewPeriodDays, &mbAbortUpdate, &daCounts, &daDates, &ulNumPts, &ulNumEvents, &aEvents, &ulNumMarks, &aMarks, pProgress,  pulNumPrevPts, dFracBefore, dFracAfter);
		// Even if we did not succeed in getting new data we need to set the channel
		// data arrays and num pts. etc. to the appropriate null values.
		// Having the CGUI_Plot class handle copying the data so we can delete the
		// the arrays allocated in CPlotPane::ReadChannelData is inefficient but makes
		// the memory alloc/free book-keeping easier so do that and change it later
		// if there is sufficient gain to be gotten.
		CGStr DetName;

		// Always use short names for the axis
		mpPlotDB->GetDetShortName(uiPL_ChanIndex, &DetName);
		if (pglTimeAlign->IsTimeAlignEnabled(glsFacNum, uiPL_ChanIndex)) // JFL annotate name with time-align indicator
		{
				DetName = ("*") + DetName;
				SetDialogTAFlag(true);
		}
		mpPlotPane->UpdateAxisData(
			uiTraceIndex, 
			daCounts, daDates, 
			ulNumPts, DetName, 
			bRefreshDisplay, 
			ulNumEvents, aEvents, 
			ulNumMarks, aMarks);

		// Now that CGUI_Plot has made a copy of the data free the temporary copies.

		CGUI_HugeMem::Free(daDates);
		CGUI_HugeMem::Free(daCounts);
		
		delete [] aEvents;
		delete [] aMarks;

	}
	GUI_SetCursorShape(GUI_CURSOR_ARROW);
	return(bSuccess);
}


// highlight the events on each trace
// Changed with DATE
BOOL CPlotDlg::UpdateAxisEvents(UINT uiTraceIndex, BOOL bRefreshDisplay)
{
	BOOL bSuccess = FALSE;
	ULONG ulNumEvents;
	EVENTS1 *aEvents;
	UINT uiDummyPotentialTraceIndex;
	UINT uiMasterPL_ChanIndex;
	UINT uiPL_ChanIndex;
	UINT uiNumTraces = GetNumAreas();

	if (!UpdateIsSuppressed()) {
		if ((mpPlotPane != NULL) && (mpPlotDB != NULL)) {
			if (uiTraceIndex < GetNumAreas()) {
				GUI_SetCursorShape(GUI_CURSOR_HOURGLASS);
			
				if (TraceIndexToPotentialTraceIndex(uiTraceIndex, &uiDummyPotentialTraceIndex, &uiMasterPL_ChanIndex)) {
					bSuccess = mpPlotDB->ReadChannelEvents(uiMasterPL_ChanIndex, mdDspStartOffsetFromDbStart, mdViewPeriodDays, &ulNumEvents, &aEvents);
					
					if (!bSuccess) {
						ulNumEvents = 0;
						aEvents = NULL;
					}

					// Loop through all traces so that if the same data is displayed on more
					// than one trace all affected traces will be updated.
					for (UINT uiTraceLoopIndex = 0; uiTraceLoopIndex < uiNumTraces; uiTraceLoopIndex++) {
						TraceIndexToPotentialTraceIndex(uiTraceLoopIndex, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex);
	
						if (uiMasterPL_ChanIndex == uiPL_ChanIndex) {
							mpPlotPane->UpdateAxisEvents(uiTraceLoopIndex, bRefreshDisplay, ulNumEvents, aEvents);
						}
					}

					delete [] aEvents;
				}
				GUI_SetCursorShape(GUI_CURSOR_ARROW);
			}
			UpdateGraphDisplay();
		}
	}
	return(bSuccess);
}

// Reads the data from the data base and replots the axes.
BOOL CPlotDlg::RefreshData()
{
	OutputDebugString("CPlotDlg::RefreshData()\n");

	const BOOL bEchoPercent = TRUE;
	BOOL bAnyFailures = FALSE;
	double dFracBefore, dFracAfter;
	BOOL bSuccess;
	UINT uiPrevNumAxes = GetNumAreas();
	ULONG ulNumPrevPts = 0;

	CGFmtStr Msg1, Msg2;	//sfk
	TwoLineProgressBar *pTwoLineProgressBar = NULL;	// sfk

	if (mpPlotPane == NULL) 
	{
		bAnyFailures = TRUE;
	}
	else 
	{
		// We come in here during graph startup so to minimize screen flashing check to see
		// if muiNumAxes > 0 before doing anything on the screen... This probably should be fixed
		// in the logic that calls this but for now .... 14-Nov-96.
		if (GetNumAreas() > 0) 
		{
			HCURSOR OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

			mbAbortUpdate = FALSE;
			double dCursorX, dDummyCursorY;
			UINT uiCurrentTraceIndex;

			mpPlotPane->GetCursorPos(&dCursorX, &dDummyCursorY, &uiCurrentTraceIndex);
			if (!UpdateIsSuppressed()) {
				pTwoLineProgressBar = new TwoLineProgressBar(1, 1, "", "", "Radiation Review: Reading Graph Data");
			}
					
			UINT uiTrace = 0;
			while ((uiTrace < GetNumAreas()) && (!mbAbortUpdate)) {
				dFracBefore = (double) uiTrace / (double) GetNumAreas();
				dFracAfter = (double) (uiTrace + 1) / (double) GetNumAreas();
										
				bSuccess = UpdateAxisData(uiTrace, FALSE, pTwoLineProgressBar, &ulNumPrevPts, dFracBefore, dFracAfter);
				if (bSuccess) {
					// If we are just now drawing the graph (uiPrevNumAxes == 0) and
					// this is the currently selected plot area (muiCurrentAxis == ui)
					// set the cursor location to be half way between end points.
					if ((mpPlotPane->GetCurrentArea() == uiTrace) && (uiPrevNumAxes == 0)) {
						//SetCurrentArea(0);
						mpPlotPane->CenterCursor();	
					}
				}
				else {
					bAnyFailures = TRUE;
				}
				uiTrace++;
			}
			
			BOOL bOnNearestDataPt = TRUE;
			BOOL bUpdateDisplay = FALSE;
			mpPlotPane->SetCursorPos(dCursorX, uiCurrentTraceIndex, bOnNearestDataPt, bUpdateDisplay);
			if (pTwoLineProgressBar)
			{
				pTwoLineProgressBar->Close();
				delete pTwoLineProgressBar;
				pTwoLineProgressBar = NULL;
			}
			::SetCursor(OldCur);
		}
		
		
		UpdateGraphDisplay();
	}
	// if the user has minimized the graph, restore it before you refresh data
	// added by SFK 3/11/99
	if ((mpDlg != NULL) && (IsIconic(mpDlg->GetWndHandle())) && (!bAnyFailures)) {
		ShowWindow(mpDlg->GetWndHandle(), SW_RESTORE);
	}
	if (mpDlg != NULL) {
		::SetWindowPos(mpDlg->GetWndHandle(), HWND_TOP, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);

	}

    return(!bAnyFailures);
}


/////////////////////////////////////////////////////////////////////////////
// Dialog Control Actions

/////////////////////////////////////////////////////////////////////////////////
//  Mark Button Actions

void CPlotDlg::DeleteScreenMark(UINT uiTraceIndex, double dMarkerValueX, BOOL bRefreshDisplay)
{
	ULONG ulNumMarks = 0;
	ULONG *aulMarks = NULL;
	UINT uiMasterPL_ChanIndex;
	UINT uiDummyPotentialTraceIndex;

	if (TraceIndexToPotentialTraceIndex(uiTraceIndex, &uiDummyPotentialTraceIndex, &uiMasterPL_ChanIndex)) {
		// Loop through all traces so that if the same data is displayed on more
		// than one trace all affected traces will be updated.
		if (mpPlotPane != NULL) {
			if (mpPlotDB != NULL) {
				UINT uiNumTraces = GetNumAreas();
				UINT uiTraceLoopIndex;
				UINT uiPL_ChanIndex;
				
				for (uiTraceLoopIndex = 0; uiTraceLoopIndex < uiNumTraces; uiTraceLoopIndex++) {
					TraceIndexToPotentialTraceIndex(uiTraceLoopIndex, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex);
	
					if (uiMasterPL_ChanIndex == uiPL_ChanIndex) {
						mpPlotPane->DeleteMarker(uiTraceLoopIndex, dMarkerValueX);
					}
				}
			
				if (bRefreshDisplay) {
					// Don't just redisplay uiAxisIndex beccause more than one trace may have
					// been updated. Force clear first to eliminate ghost cursor between
					// traces.
					UpdateGraphDisplay();
				}
			}
		}
	}
}

// part of Mark button
void CPlotDlg::AddScreenMark(UINT uiTraceIndex, double dMarkerValueX, BOOL bRefreshDisplay)
{
	ULONG ulNumMarks = 0;
	ULONG *aulMarks = NULL;
	UINT uiMasterPL_ChanIndex;
	UINT uiDummyPotentialTraceIndex;

	if (TraceIndexToPotentialTraceIndex(uiTraceIndex, &uiDummyPotentialTraceIndex, &uiMasterPL_ChanIndex)) {
		// Loop through all traces so that if the same data is displayed on more
		// than one trace all affected traces will be updated.
		if (mpPlotPane != NULL) {
			if (mpPlotDB != NULL) {
				UINT uiNumTraces = GetNumAreas();
				UINT uiTraceLoopIndex;
				UINT uiPL_ChanIndex;
				CGFmtStr DateStr;
				
				DateStr.Printf("%s", RelDateToTimeStr(dMarkerValueX));
				
				for (uiTraceLoopIndex = 0; uiTraceLoopIndex < uiNumTraces; uiTraceLoopIndex++) {
					TraceIndexToPotentialTraceIndex(uiTraceLoopIndex, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex);
	
					if (uiMasterPL_ChanIndex == uiPL_ChanIndex) {
						mpPlotPane->AddMarker(uiTraceLoopIndex, dMarkerValueX, DateStr, mMarkerTextColor);
					}
				}
			
				if (bRefreshDisplay) {
					// Don't just redisplay uiAxisIndex beccause more than one trace may have
					// been updated. Force clear first to eliminate ghost cursor between traces.
					UpdateGraphDisplay();
				}
			}
		}
	}
}

// action for Mark pushbutton
void CPlotDlg::MarkCursorPosition(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    double dCursorX;
    UINT uiCurrentMarkerTraceIndex;
    BOOL bCursorOnPrevMark;
    UINT uiPL_ChanIndex;
    UINT uiDummyPotentialTraceIndex;
	    
	if (mpPlotDB != NULL) {
		if (mpPlotPane != NULL) {
			bCursorOnPrevMark = mpPlotPane->IsCursorOnMarker(&uiCurrentMarkerTraceIndex, &dCursorX);
			TraceIndexToPotentialTraceIndex(uiCurrentMarkerTraceIndex, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex);
	
			if (bCursorOnPrevMark) {
				CGFmtStr Msg;
				Msg.Printf("Delete marker at\n%s?", RelDateToDateAndTimeStr(dCursorX));
				GUI_ACTION Return = GUI_MsgBox(Msg, GUI_ICON_QUESTION, GUI_YESNO);
				if (Return == GUI_YES) {
					// Delete mark from DB.
					//if (mpPlotDB->DeleteMarkRecord(uiPL_ChanIndex, (ULONG)dCursorX)) {
					if (mpPlotDB->DeleteMarkRecord(uiPL_ChanIndex, dCursorX)) {
						DeleteScreenMark(uiCurrentMarkerTraceIndex, dCursorX, TRUE);	// TRUE => redraw graph to delete marker text.
					}
					else {
						RadReviewMsg(uiINTERNAL_ERR, "Delete mark failed");
						//GUI_MsgBox("Delete Mark Failed.", GUI_ICON_ERROR, GUI_OK);
					}
				}
			}
			else { 
				int raison = S_OK;
				if (mpPlotDB->CreateMarkRecord(uiPL_ChanIndex, dCursorX, raison)) {
					AddScreenMark(uiCurrentMarkerTraceIndex, dCursorX, TRUE);	// TRUE => redraw graph to show marker text.
				}
				else {
					switch (raison)
					{

					case uiDAY_NOT_IN_DB_STATUS:
					case uiINVALID_TIME_INTERVAL_ERR:
					case uiMARK_NOT_IN_DB_WARN:
					case uiNO_DATA_FOR_STATION_WARN:
					case uiSTA_NOT_IN_DB_ERR:
					case uiTIME_NOT_IN_DB_STATUS:
						RadReviewMsg(raison, "Mark creation failed");
					break;
					default:
						RadReviewMsg(uiINTERNAL_ERR, "Create mark failed");
					}
					
				}
			}
		
			SetFocusToGraph();
		}
	}
}


// SFK  Added service function for real time update button
void CPlotDlg::RealTimeUpdateBtnServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{                            
	ImportTodayDoIt();
}


//////////////////////////////////////////////////////////////////////
//	Edit Event Button

// delete the highlighted event that the cursor is now in
void CPlotDlg::DeleteScreenEvent(UINT uiTraceIndex, double dMarkerValueX, BOOL bRefreshDisplay)
{
	UINT uiMasterPL_ChanIndex;
	UINT uiDummyPotentialTraceIndex;

	if (TraceIndexToPotentialTraceIndex(uiTraceIndex, &uiDummyPotentialTraceIndex, &uiMasterPL_ChanIndex)) {
		// Loop through all traces so that if the same data is displayed on more
		// than one trace all affected traces will be updated.
		if (mpPlotPane != NULL) {
			if (mpPlotDB != NULL) {
				UINT uiNumTraces = GetNumAreas();
				UINT uiTraceLoopIndex;
				UINT uiPL_ChanIndex;
				
				for (uiTraceLoopIndex = 0; uiTraceLoopIndex < uiNumTraces; uiTraceLoopIndex++) {
					TraceIndexToPotentialTraceIndex(uiTraceLoopIndex, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex);
	
					if (uiMasterPL_ChanIndex == uiPL_ChanIndex) {
						mpPlotPane->DeleteHiliteRegion(uiTraceLoopIndex, dMarkerValueX);
					}
				}
			
				if (bRefreshDisplay) {
					// Don't just redisplay uiAxisIndex beccause more than one trace may have
					// been updated. Force clear first to eliminate ghost cursor between traces.
					UpdateGraphDisplay();
				}
			}
		}
	}
}

void CPlotDlg::EditEventBtnServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	// Main plot dialog is running modeless. We came here from there and cannot have it
	// disappear while here so we could disable it to prevent that from happening.
	// However it may be desirable to allow user to interact with plot dialog while help
	// dialog is up so create help and start it modeless and return to main plot dialog.
	// Make sure we close the help dialog if it is open when we are shutting down CPlotDlg.
	// CGUI_Dlg Dlg(IDD_GRAPH_HELP);
	// Dlg.Go();
	if (mpEditEvents != NULL) mpEditEvents->SetFocusTo();
	else mpEditEvents = new CEditEvents(this, mpPlotPane, mpPlotDB);
}

void CPlotDlg::UpdateEditEventDlg()
{
	if (mpEditEvents != NULL) mpEditEvents->UpdateDlg();
}



// Called from changing picklist view period choice.
// Determine the view period from the picklist
// Calculate and set the values for the view period scroll bar
// Added SetCurrentViewStartDate to update the next, prev buttons and scroll bar chicklet
// Changed with DATE
void CPlotDlg::SetViewPeriodParameters()
{
	const int iLINE_INCREMENT = 1;
	int iPageIncrement;
	int iMaxValue; 
	const int iMinValue = 0;	//INT_MIN;
	ULONG ulNumDB_Mins;
	UINT uiPeriodPL_Index;
	//ULONG ulMaxSB_Range = (ULONG)INT_MAX;	//(ULONG)UINT_MAX;
	ULONG ulMaxSB_Range = (ULONG)65535;	//sfk - must be 16 bit integer

	mdViewPeriodDays = naViewPeriod[uiINIT_VIEW_PERIOD_INDEX].dDaysInPeriod;

		// Find out the View period set by the user via the pick list control.
	if (mpDlg != NULL) {
		if (mpDlg->PickListGetValue(IDC_VIEW_PERIOD_PL, &uiPeriodPL_Index)) {
			mdViewPeriodDays = naViewPeriod[uiPeriodPL_Index].dDaysInPeriod;
			// Protect from division by zero below -- set default to 24 hours.
			if (mdViewPeriodDays == 0,0) {
				mdViewPeriodDays = 1.0;
			}
	
			// the ViewPeriod scroll bar is going to be expressed in minutes so
			//	convert DATE stuff to minutes
			DATE dDB_StartDate, dDB_EndDate;
			if (mpPlotDB != NULL) {
				mpPlotDB->GetDateRange(&dDB_StartDate, &dDB_EndDate); // get start and end julian times
				ulNumDB_Mins = (unsigned long)ceil((dDB_EndDate - dDB_StartDate) * 1440.0); 
				// If the number of minutes of data in the data base > the maximum range
				// of a Windows scroll bar set the line increment to some factor times a minute.
				if (ulNumDB_Mins < ulMaxSB_Range) {
					miDspStartLineIncrementMins = 1;
					iMaxValue = iMinValue + (int)ulNumDB_Mins;
				}
				else {
					miDspStartLineIncrementMins = (int)((ulNumDB_Mins + ulMaxSB_Range - 1L) / ulMaxSB_Range);
					iMaxValue = iMinValue + (int)(ulNumDB_Mins / (ULONG)miDspStartLineIncrementMins);
				}
			}
	
	// ??????????????????
//	miDspStartLineIncrementMins = 10;
//	iMaxValue = iMinValue + (int)((ulNumDB_Mins + (ULONG)miDspStartLineIncrementMins) / (ULONG)miDspStartLineIncrementMins);
	
			iPageIncrement = (int)(mdViewPeriodDays * MINUTES_PER_DAY) / miDspStartLineIncrementMins;

			// set up the scroll bar parameters and where the thumb is now
			// the scroll bar range is from 0 to the number of minutes in the db
			mpDlg->ScrollBarSetParameters(IDC_VIEW_PERIOD_SB, iMinValue, iMaxValue, iLINE_INCREMENT, iPageIncrement);
			SetViewPeriodSB_Value(mdDspStartOffsetFromDbStart);
			mpDlg->SetEnable(IDC_VIEW_PERIOD_SB, TRUE);
	
			// have the scroll bar stuff taken care of, now set the correct limits
			// for the data to be displayed in the different graph areas
			double dMinX = mdDspStartOffsetFromDbStart;
			double dMaxX = dMinX + mdViewPeriodDays;
			BOOL bAutoScaleX = FALSE;	// Otherwise x-axis limits reflect actual first & last
										// time points rather than nice interval based on day etc.
			if (mpPlotPane != NULL) mpPlotPane->SetLimitsX(bAutoScaleX, dMinX, dMaxX);

			SetCurrentViewStartDate(mpDlg, mdDspStartOffsetFromDbStart); // sfk added to correctly enable Next, Prev
		}
	}
}



// action for Print pushbutton
void CPlotDlg::PrintGraphServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{                            
	CGFmtStr Header, Footer, CurrentTimeStr, CurrentDateStr;	//, Facility_Name;
	char szFacilityName[DB_NAME_LEN+1];		// sfk 5-20-2002 can't get the name into CGFmtStr so use this method
	// Get current date and time strings.
	CFmtdDate Date;	// Default set is current date.
	Date.Get(&CurrentDateStr);
	CFmtdTime Time;	// Default set is current time.
	Time.Get(&CurrentTimeStr);                                                           
	if (mpPlotDB != NULL) {
		pglFacCfg->GetFacilityLongName(glsFacNum, szFacilityName, DB_NAME_LEN);
	}
	Footer.Printf("Printed at %s on %s",
		CurrentTimeStr.Get_sz(), CurrentDateStr.Get_sz());

	Header.Printf("Facility: %s", szFacilityName);
	if (pRAD_Init->Get_MIC_Inspec_Info_File_Use() == TRUE) {
		Header.PrintfAppend("      Inspection Number = %s", pRAD_Init->Get_Inspection_Number());
	}
	if (mUsingTA)
	{
		Header.PrintfAppend("  with station time adjustment*");
	}

	if (mpPlotPane != NULL) mpPlotPane->Print(Header, Footer); 
}


// action for Help pushbutton
void CPlotDlg::HelpBtnServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	// Main plot dialog is running modeless. We came here from there and cannot have it
	// disappear while here so we could disable it to prevent that from happening.
	// However it may be desirable to allow user to interact with plot dialog while help
	// dialog is up so create help and start it modeless and return to main plot dialog.
	// Make sure we close the help dialog if it is open when we are shutting down CPlotDlg.
	// CGUI_Dlg Dlg(IDD_GRAPH_HELP);
	// Dlg.Go();
	if (mpHelpDlg != NULL) mpHelpDlg->SetFocus();
	else {
		mpHelpDlg = new CGUI_Dlg(IDD_GRAPH_HELP);
		if (mpHelpDlg != NULL) {
			mpHelpDlg->DefinePushBtn(IDOK, GUI_OK);
			mpHelpDlg->SetDlgExitValidateFn(this, (GUI_CB_DlgExitFn)&CPlotDlg::HelpDlgExitValidateFn);
			mpHelpDlg->GoModeless(TRUE);
		}
	}
}

BOOL CPlotDlg::HelpDlgExitValidateFn(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction)
{
	BOOL bOkToClose = TRUE;  // We never block the closing; we just want to know about it.
	mpHelpDlg = NULL;	// So that we don't try to close it in CPlotDlg destructor.
	return(bOkToClose);
}

//////////////////////////////////////////////////////////////////////////////
//	Zoom Actions

void CPlotDlg::EchoZoomFactor(UINT uiZoomFactor)
{
	CGFmtStr ZoomFactorMsg;

	if (uiZoomFactor > 1) {
		if (mbZoomTimes100) {
			uiZoomFactor *= 100;
		}
		ZoomFactorMsg.Printf("(x 1/%u)", uiZoomFactor);
	}
	else {
		if (mbZoomTimes100) {
			ZoomFactorMsg.Printf("(x 1/100)");
		}
		else {
			ZoomFactorMsg.Printf("(x 1)");
		}
	}
	
	if (mpDlg != NULL) {
		mpDlg->FldStaticTxtSetValue(IDC_ZOOM_FACTOR_DSP, ZoomFactorMsg); 
	}
}


// label the total zoom amount about the zoom scroll bar
// disable/enable In/Out pushbuttons
void CPlotDlg::SetZoomCtrlsEnable(CGUI_Dlg *pDlg)
{
	BOOL bZoomInServiceFnEnable;
	BOOL bZoomOutServiceFnEnable;
	
	EchoZoomFactor(muiZoomFactor);	// label the zoom amount
	
	if (muiZoomFactor > 1) {
		if (muiZoomFactor < uiMAX_ZOOM_FACTOR) {
			bZoomInServiceFnEnable = TRUE;
		}
		else {
			bZoomInServiceFnEnable = FALSE;
		}
		bZoomOutServiceFnEnable = TRUE;
	}
	else {
		bZoomInServiceFnEnable = TRUE;
		bZoomOutServiceFnEnable = FALSE;
	}

	pDlg->SetEnable(IDC_ZOOM_OUT, bZoomOutServiceFnEnable);
	pDlg->SetEnable(IDC_ZOOM_IN, bZoomInServiceFnEnable);
}


// action for ZoomIn pushbutton
void CPlotDlg::ZoomInServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	SetZoomFactor(muiZoomFactor * 2);
	SetFocusToGraph();
}


// action for ZoomOut pushbutton
void CPlotDlg::ZoomOutServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	if (muiZoomFactor >= 2) {
		SetZoomFactor(muiZoomFactor / 2);
	}
	else {
		GUI_Beep();
	}
	SetFocusToGraph();
}


// action for x100 checkbox
void CPlotDlg::ZoomTimes100ServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	// Any time the X100 check box is clickd we end up here and update the
	// member variable, mbZoomTimes100, which is used several other places.
	pDlg->CheckBoxGetValue(CtrlID, &mbZoomTimes100);
	SetZoomFactor(muiZoomFactor);
	SetFocusToGraph();
}

// calculate a "good" value for the zoom factor
// set the scroll bar thumb position
// thru SetZoomCtrlsEnable label the zoom amount and enable the In/Out buttons
void CPlotDlg::SetZoomFactor(UINT uiZoomFactor)
{
	const float fVERT_ZOOM_FACTOR = 1.0;
	
	// If the overall zoom factor is >= 100 and mbZoomTimes100 is TRUE
	// reduce uiZoomFactor by 100 then check for in bounds. This is necessary
	// because muiZoomFactor is set equal to uiZoomFactor. muiZoomFactor is the
	// slider bar value 1-100 and can be multiplied by 100 if mbZoomTimes100 == TRUE.
	// uiMAX_ZOOM_FACTOR = 99
	if ((uiZoomFactor > uiMAX_ZOOM_FACTOR) && (mbZoomTimes100)) {
		//uiZoomFactor = uiMAX_ZOOM_FACTOR;
		if (uiZoomFactor != 128) {
			uiZoomFactor /= 100;
		}
		else { // special case of 128 by hitting In with x100
			uiZoomFactor = uiMAX_ZOOM_FACTOR;
		}
				
	}
	
	if ((uiZoomFactor > uiMAX_ZOOM_FACTOR) && (!mbZoomTimes100)) {
		uiZoomFactor = uiMAX_ZOOM_FACTOR;
	}
	
	if (uiZoomFactor < 1) {
		uiZoomFactor = 1;
	}
	
	muiZoomFactor = uiZoomFactor;
	if (mpDlg != NULL) {
		mpDlg->ScrollBarSetValue(IDC_ZOOM_FACTOR_SB, (int)muiZoomFactor);
	}

	SetZoomCtrlsEnable(mpDlg);

	if (mbZoomTimes100) {
		uiZoomFactor *= 100;
	}
	if (mpPlotPane != NULL) mpPlotPane->SetZoomFactorX(uiZoomFactor);
}
 
 
void CPlotDlg::TurnZoomOff()
{
	mbZoomTimes100 = FALSE;
	if (mpDlg != NULL) {
		mpDlg->CheckBoxSetValue(IDC_ZOOM_TIMES_100, mbZoomTimes100);
	}
	SetZoomFactor(1);
}


// action for AutoScale checkbox
void CPlotDlg::ZoomAutoScaleServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	// Any time the "Auto Scale During Zoom check box is clickd we end up here and
	// update the member variable, mbZoomAutoScale, which is used several other places.
	pDlg->CheckBoxGetValue(CtrlID, &mbZoomAutoScale);
	if (mpPlotPane != NULL) mpPlotPane->SetZoomAutoScaleY(GUI_PLOT_ALL_AREAS, mbZoomAutoScale);
	SetFocusToGraph();
}


// action for zoom factor scroll bar change
void CPlotDlg::ZoomFactorSB_NotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	int iNewZoomFactor;
	
	pDlg->ScrollBarGetValue(IDC_ZOOM_FACTOR_SB, &iNewZoomFactor);
	SetZoomFactor(iNewZoomFactor);
	SetFocusToGraph();
}


// action for zoom factor scroll bar
void CPlotDlg::ZoomFactorSB_TrackingFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	int iNewZoomFactor;
	
	// Echo scroll bar setting on the screen as it is being moved. This does
	// not change the graph data -- that is done in SetZoomFactor().
	pDlg->ScrollBarGetValue(IDC_ZOOM_FACTOR_SB, &iNewZoomFactor);
	EchoZoomFactor(iNewZoomFactor);
}


////////////////////////////////////////////////////////////////////////////////////
// View Period Actions

// update where the "chicklet" is on the view period scroll bar
// changed with DATE
void CPlotDlg::SetViewPeriodSB_Value(DATE dOffsetFromDB_StartDate)
{
	int iScrollValue;
	int iMinValue, iMaxValue;
	int iIncrementValueMins;

	if (mpDlg != NULL) {
		mpDlg->ScrollBarGetParameters(IDC_VIEW_PERIOD_SB, &iMinValue, &iMaxValue, &iIncrementValueMins);
		// Note that if the user typed in a date & time value it may not be
		// an integral number of iIncrementValue from ulDB_StartDate.
		// In this case, the next movement of the scroll bar will force
		// the date and time back into "alignment" rather than bumping the
		// date and time by iIncrementValue.

		// The scroll bar is in minutes so convert the offset to minutes before position the thumb
		int iOffsetFromDB_StartDateMins;
		double dOffsetFromDB_StartDateMins = (dOffsetFromDB_StartDate * MINUTES_PER_DAY) + (0.5/MINUTES_PER_DAY); 
		iOffsetFromDB_StartDateMins = (int)(dOffsetFromDB_StartDateMins);
		iScrollValue = iMinValue + ((iOffsetFromDB_StartDateMins + (miDspStartLineIncrementMins/2))/miDspStartLineIncrementMins);
		mpDlg->ScrollBarSetValue(IDC_VIEW_PERIOD_SB, iScrollValue);
		EchoViewPeriodSB_Value(dOffsetFromDB_StartDate);

	}
}

// print time of viewperiod thumb above scroll bar
void CPlotDlg::EchoViewPeriodSB_Value(DATE dOffsetFromDB_StartDate)
{
	const char *szDateTimeStr = RelDateToDateAndTimeStr(dOffsetFromDB_StartDate);
	UINT uiFullStrLen = strlen(szDateTimeStr);
	char *szNoSecsStr = new char[uiFullStrLen + 1];

	if (szNoSecsStr != NULL) {
		if (mpDlg != NULL) {
			strcpy(szNoSecsStr, szDateTimeStr);
			if (uiFullStrLen >= 3) {
				szNoSecsStr[uiFullStrLen - 3] = '\0';	// Get rid of trailing ":ss".
			}
			mpDlg->FldStaticTxtSetValue(IDC_WHICH_VIEW_DSP, szNoSecsStr); 
			delete szNoSecsStr;
		}
	}
}

// Called from previous, next button pushes, scroll bar notify, when read graph parameters,
//    when change action from start time dialog
// Based on a new starting view time, calculate the limits of when to enable the Next, Prev buttons
// Enable/disable the previous, next buttons as appropriate
// Set a new value on the view period scroll bar
// Calculate new x limits
// Changed with DATE
void CPlotDlg::SetCurrentViewStartDate(CGUI_Dlg *pDlg, DATE dNewViewPeriodOffsetFromDbStart)
{

	if (mpPlotDB != NULL) {
		DATE dDB_StartDate, dDB_EndDate;

		mpPlotDB->GetDateRange(&dDB_StartDate, &dDB_EndDate);
		// figure out the earliest/latest possible view periods based on whole db
		DATE dEarliestViewStartDate = dDB_StartDate - mdViewPeriodDays;
		DATE dLatestViewStartDate = dDB_EndDate;
	
		// calculate what the new requested view period start/end are and adjust if
		// necessary to stay within the db range.
	    DATE dNewViewPeriodStartDate = dDB_StartDate + dNewViewPeriodOffsetFromDbStart;
	    DATE dNewViewPeriodEndDate = dNewViewPeriodStartDate + mdViewPeriodDays;
	    
		// Make sure the new date is within the data range.
		if (dNewViewPeriodStartDate < dEarliestViewStartDate) {
			dNewViewPeriodStartDate = dEarliestViewStartDate;
		}
		else if (dNewViewPeriodStartDate > dLatestViewStartDate) {
			dNewViewPeriodStartDate = dLatestViewStartDate;
		}
		 
		// Set previous and next view btns. enable / disable.
		// based on this new view period
		if (dNewViewPeriodStartDate > dDB_StartDate) {
			pDlg->SetEnable(IDC_PREV_VIEW, TRUE);
		}
		else {
			pDlg->SetEnable(IDC_PREV_VIEW, FALSE);
		}
			
		if (dNewViewPeriodEndDate < dDB_EndDate) {
			pDlg->SetEnable(IDC_NEXT_VIEW, TRUE);
		}
		else {
			// This may be a little misleading to the user. If there is data showing in the
			// current "time slice" but there is no data beyond the right-hand (end) time
			// showing on the plot the "NEXT" button will be disabled. One can still scroll
			// the starting time forward using the page or line scroll bar capabilities.
			pDlg->SetEnable(IDC_NEXT_VIEW, FALSE);
		}
			
		mdDspStartOffsetFromDbStart = dNewViewPeriodStartDate - dDB_StartDate;
		if (mdDspStartOffsetFromDbStart >= 0) {
			SetViewPeriodSB_Value(mdDspStartOffsetFromDbStart);
		}
		else {	// push chicklet as far to the left as possible sfk
			SetViewPeriodSB_Value(0);
		}

		// now that you know the view period to be displayed, get
		// that information to the stuff that actually plots the data
		double dMinX = mdDspStartOffsetFromDbStart;
		double dMaxX = dMinX + mdViewPeriodDays;
		BOOL bAutoScaleX = FALSE;	// Otherwise x-axis limits reflect actual first & last
									// time points rather than nice interval based on day etc.
		if (mpPlotPane != NULL) {
			mpPlotPane->SetLimitsX(bAutoScaleX, dMinX, dMaxX);
			
			//double dMinFullX, &dMaxFullX;
			//mpPlotPane->GetLimitsX(&dMinFullX, &dMaxFullX);
			//SetViewPeriodSB_Value(mdDspStartOffsetFromDbStart);
		}
		RefreshData();
	}
}


// Action for Previous View pushbutton
void CPlotDlg::PrevView()
{
	PrevViewServiceFn(mpDlg, NULL);
}

// Action for Next View pushbutton
void CPlotDlg::NextView()
{
	NextViewServiceFn(mpDlg, NULL);
}

// part of Previous View button
void CPlotDlg::PrevViewServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	DATE dNewViewStart = mdDspStartOffsetFromDbStart - mdViewPeriodDays;
	SetCurrentViewStartDate(pDlg, dNewViewStart);
	TurnZoomOff();
	if (mpPlotPane != NULL) {
		if (!mpPlotPane->UpdateIsSuppressed()) {
			SetFocusToGraph();
		}
	}
}

// part of next view button
void CPlotDlg::NextViewServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	DATE dNewViewStart = mdDspStartOffsetFromDbStart + mdViewPeriodDays;
	SetCurrentViewStartDate(pDlg, dNewViewStart);
	TurnZoomOff();
	if (mpPlotPane != NULL) {
		if (!mpPlotPane->UpdateIsSuppressed()) {
			SetFocusToGraph();
		}
	}
}


// action for view period picklist choice
void CPlotDlg::ViewPeriodNotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	SetViewPeriodParameters();
	// If we are zoomed in turn the zoom off to minimize confusion for end-user
	// per Mark Abhold 12-Feb-97. Turning zoom off causes screen to update then
	// but new view period parameters have not yet taken effect so we have to
	// update data AND screen via call to RefreshData. This causes extra screen flash.
	// However if RefreshData is called before TurnZoomOff the problem goes away.
//	RefreshData();
	if ((muiZoomFactor != 1) || (mbZoomTimes100)) {
		TurnZoomOff();
	}
}

// if the view period scroll bar changes, then you must update the date displayed.
void CPlotDlg::ViewPeriodSB_NotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	int iScrollValue;
	int iMinValue, iMaxValue;
	int iIncrementValueMins;
	long lNewViewPeriodOffsetDateInSecs;

	// get the scroll bar parameters
	pDlg->ScrollBarGetValue(IDC_VIEW_PERIOD_SB, &iScrollValue);
	pDlg->ScrollBarGetParameters(IDC_VIEW_PERIOD_SB, &iMinValue, &iMaxValue, &iIncrementValueMins);

	// from the scroll bar parameters, figure out how many seconds you are past the start of the scroll bar
	lNewViewPeriodOffsetDateInSecs = (long)(iScrollValue - iMinValue) *	((long)miDspStartLineIncrementMins * 60L);
	
	// convert the number of seconds to DATE units
	DATE dNewViewPeriodOffsetInDATE = lNewViewPeriodOffsetDateInSecs/SECONDS_PER_DAY;

	// based on where we are now on the view period scroll bar, update the next/pres buttons and the scroll bar
	SetCurrentViewStartDate(pDlg, dNewViewPeriodOffsetInDATE); // update next, prev, scroll bar

	if (!mpPlotPane->UpdateIsSuppressed()) {
		SetFocusToGraph();
	}
}


// action for view period scroll bar end button
void CPlotDlg::ViewPeriodSB_TrackingFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	int iScrollValue;
	int iMinValue, iMaxValue;
	int iIncrementValueMins;
	ULONG ulCurrentDateOffsetInSecs;
	
	// Echo scroll bar setting on the screen as it is being moved. This does
	// not change the graph data -- that is done in SetCurrentViewStartDate().
	pDlg->ScrollBarGetValue(IDC_VIEW_PERIOD_SB, &iScrollValue);
	pDlg->ScrollBarGetParameters(IDC_VIEW_PERIOD_SB, &iMinValue, &iMaxValue, &iIncrementValueMins);

	// from the scroll bar parameters, figure out how many seconds you are past the start of the scroll bar
	ulCurrentDateOffsetInSecs = (long)(iScrollValue - iMinValue) * (long)miDspStartLineIncrementMins * 60L;
	
	// convert the number of seconds to DATE units
	DATE dCurrentDateOffsetInDATE = ulCurrentDateOffsetInSecs/SECONDS_PER_DAY;

	EchoViewPeriodSB_Value(dCurrentDateOffsetInDATE); // echo where the "thumb" is
}


////////////////////////////////////////////////////////////////////////////////////
// Suppress Update checkbox

void CPlotDlg::SuppressUpdateCB_NotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID ControlID)
{
	BOOL bWasSuppressed = FALSE;
	BOOL bSuppressUpdate;
	
	pDlg->CheckBoxGetValue(ControlID, &bSuppressUpdate);
	if (mpPlotPane != NULL) {
		bWasSuppressed = mpPlotPane->UpdateIsSuppressed();
		mpPlotPane->SuppressUpdate(bSuppressUpdate);
	}
	
	// If update was supressed and now is not we may not have proper data (since
	// suppressing update turns off data updating) so read current data.
	if (bWasSuppressed && !bSuppressUpdate) {
		RefreshData();
	}
}

void CPlotDlg::SuppressUpdate(BOOL bSuppressUpdate)
{
	BOOL bWasSuppressed;
	if (mpPlotPane != NULL) {
		bWasSuppressed = mpPlotPane->UpdateIsSuppressed();
		mpPlotPane->SuppressUpdate(bSuppressUpdate);
	}
	
	// If update was supressed and now is not we may not have proper data (since
	// suppressing update turns off data updating) so read current data.
	if (bWasSuppressed && !bSuppressUpdate) {
		RefreshData();
	}
}

BOOL CPlotDlg::UpdateIsSuppressed()
{    
	BOOL bUpdateIsSuppressed = FALSE;
	
	if (mpPlotPane != NULL) {
		bUpdateIsSuppressed = mpPlotPane->UpdateIsSuppressed();
	}
	return(bUpdateIsSuppressed);
}


/////////////////////////////////////////////////////////////////////////////
//	Log/Linear Actions

// action for Log/Linear checkbox
void CPlotDlg::LogLinearToggleServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	SetLogLinearStyle(!mbLogY_Axis);
//	WriteGraphOptions();	// sfk removed
//	NotifyGraphHeightChange();
	SetFocusToGraph();
}

void CPlotDlg::SetLogLinearStyle(BOOL bLogY_Axis)
{
	if (mbLogY_Axis != bLogY_Axis) {
		mbLogY_Axis = bLogY_Axis;
		
		if (mpDlg != NULL) mpDlg->CheckBoxSetValue(IDC_LOG_Y, mbLogY_Axis);
		if (mpPlotPane != NULL) mpPlotPane->SetLogY(GUI_PLOT_ALL_AREAS, bLogY_Axis);
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//	Read/Write Graph Setup to DB Vista Database

// 01-14-99  Modified to read parameters from dbVista database, also introduced concept
// of naming the configuration to be expanded on later.
// Version 2.04  When checking if display offset is "valid" have all times in DB Units.  
//			Changed check to be mlDspStartOffset + ulDB_StartDate
// Version 2.05  When checking if display offset is "valid" have all times in DB Units.  
//			Corrected to set mlDspStartOffset to 0 if exceeds above test condition
//			Corrected to update both name and checkbox for picklist choices.
// Version 2.06  Capability to read multiple setups from the database
// Changed with DATE
void CPlotDlg::ReadGraphSetup(CGStr *pConfigName, BOOL bStartTime)
{

	struct db_graph_dflt_rec dbGraph;
	int status;

	if (mpPlotDB != NULL) {
		// reading parameters from dbVista database rather than system ini file
		strcpy(dbGraph.sz_config_name_key, pConfigName->Get_sz());
		status = AccessDefaultDatabase(READ, 0, &dbGraph); 
		if (status != TRUE) {
			InitDbDfltGraphRec(&dbGraph);	// get the default parameters
		}
		// SFK+++++++++++++
		if (bStartTime == TRUE) {	// change the start time read with the setup only the first time
			unsigned long ulDB_EndDate, ulDB_StartDate;
			DATE dDB_EndDate, dDB_StartDate;
			//CMyDateTime Date;
			//This needs to be fixed for DATE
			mdDspStartOffsetFromDbStart = dbGraph.d_display_offset;
			mpPlotDB->GetDateRange(&dDB_StartDate, &dDB_EndDate);
			ulDB_StartDate = (unsigned long)glMyDateTime.DATETimestampToMyTimestamp(dDB_StartDate);
			ulDB_EndDate = (unsigned long)glMyDateTime.DATETimestampToMyTimestamp(dDB_EndDate);
			if ((mdDspStartOffsetFromDbStart + dDB_StartDate) > dDB_EndDate) 
				mdDspStartOffsetFromDbStart = 0.0;
		}
		if (mpDlg != NULL) 
		{
			SuppressUpdate(TRUE);

			// based on db_Graph.ui_view_period_index, set the correct choice in the View:
			// update the viewperiod parameters and thumb position, and the date/time
			// above the scroll bar, the Prev/Next button state
			mpDlg->PickListSetValue(IDC_VIEW_PERIOD_PL, dbGraph.us_view_period_index);
			SetViewPeriodParameters();
			SetCurrentViewStartDate(mpDlg, mdDspStartOffsetFromDbStart);

			// based on db_Graph parameters, set the X100 box, the In/Out button states,
			// the zoom scroll bar and the total times value printed about zoom scroll bar
			//muiZoomFactor = dbGraph.ui_zoom_factor;
			muiZoomFactor = 1;		// SFK: 05-23-99 default to 1
			mbZoomTimes100 = FALSE;		// SFK: 05-23-99 default to off
			SetZoomFactor(muiZoomFactor);

			// set the autoscale checkbox
			mbZoomAutoScale = dbGraph.b_zoom_auto_scale;
			mpDlg->CheckBoxSetValue(IDC_ZOOM_AUTO_SCALE, mbZoomAutoScale);
			if (mpPlotPane != NULL) mpPlotPane->SetZoomAutoScaleY(GUI_PLOT_ALL_AREAS, mbZoomAutoScale);

			mbChansOverlapped = dbGraph.b_chans_overlapped;

			// set the log scale checkbox
			mbLogY_Axis = dbGraph.b_log_y_axis;
			BOOL bLogY_Axis = mbLogY_Axis;
			mbLogY_Axis = !mbLogY_Axis;
			SetLogLinearStyle(bLogY_Axis);

			// 2.06 modification to change to different graph setups
			DeleteAllVisibleChannels();	// remove any plot areas that are there now
			
			struct IDStruct ChanID;
			for (unsigned int ui=0; ui< uiMAX_PLOT_TRACES ; ui++) 
			{
				mabShowDet[ui] = dbGraph.b_trace_active[ui]; 
				ChanID.sStaID = dbGraph.us_trace_station[ui];
				ChanID.sChanID = dbGraph.us_trace_channel[ui];
				ChanID.sStaType = dbGraph.us_trace_station_type[ui];

				if (!pglFacCfg->GetPicklistChoiceForChannelID(ChanID, (short *)&(mauiPickListIndex[ui])))
					mauiPickListIndex[ui] = 0;	// couldn't find old one, just use first one

				//mauiPickListIndex[ui] = dbGraph.us_trace_index[ui];
				mpDlg->SetEnable(maPickListID[ui], mabShowDet[ui]);
				mpDlg->CheckBoxSetValue(maCheckBoxID[ui], mabShowDet[ui]);
				mpDlg->PickListSetValue(maPickListID[ui], mauiPickListIndex[ui]);
				mpDlg->SetVisible(maPickListID[ui], TRUE);
				mpDlg->SetVisible(maCheckBoxID[ui], TRUE);
			}
			AddInitVisibleChannels();	// add the plot areas for these new channels
			SuppressUpdate(FALSE);
		}
	}
}

// 08-12-2004	SFK	Added to get read of current graph parameters to use when entering
//	exiting the Options page
BOOL CPlotDlg::GetCurrentGraphSetup(CGStr *pConfigName, struct db_graph_dflt_rec *pdbGraph)
{
	CGStr Name;
	char dbName[DRIVE_LEN+PATH_LEN+1];
	
	if (mpPlotDB != NULL) {
		strcpy(dbName, pConfigName->Get_sz());
		_strlwr(dbName);	// convert always to lower case before store
		MyStrnCpy(pdbGraph->sz_config_name_key, dbName, 99);	// store only the first 99 chars
		// get the values for the rest of the db graph record from the member variables.
		pdbGraph->d_display_offset = mdDspStartOffsetFromDbStart;
		pdbGraph->b_zoom_x100 = mbZoomTimes100;
		pdbGraph->b_zoom_auto_scale = mbZoomAutoScale;
		pdbGraph->b_log_y_axis = mbLogY_Axis;
		pdbGraph->b_chans_overlapped = mbChansOverlapped;
		pdbGraph->us_zoom_factor = muiZoomFactor;
		if (mpDlg != NULL) {
			//unsigned int iTemp = pdbGraph->us_view_period_index;
			unsigned int iTemp;										// 08-Nov-2004 SFK don't set the value until read
			mpDlg->PickListGetValue(IDC_VIEW_PERIOD_PL, &iTemp);
			pdbGraph->us_view_period_index = (unsigned short)iTemp;
		}
		IDStruct ChanID;
		for (UINT ui = 0; ui < uiMAX_PLOT_TRACES; ui++) {
			pdbGraph->b_trace_active[ui] = mabShowDet[ui];
			pglFacCfg->GetChannelIDForPicklistChoice(mauiPickListIndex[ui], &ChanID);
			pdbGraph->us_trace_station[ui] = ChanID.sStaID;
			pdbGraph->us_trace_channel[ui] = ChanID.sChanID;
			pdbGraph->us_trace_station_type[ui] = ChanID.sStaType;
			//pdbGraph->us_trace_index[ui] = mauiPickListIndex[ui];
		}
		return TRUE;
	}
	return FALSE;
}

// 01-14-99  Modified to write parameters to dbVista database
void CPlotDlg::SaveGraphSetup(CGStr *pConfigName)
{
	CGStr Name;
	struct db_graph_dflt_rec dbGraph;
	int status;

	if (GetCurrentGraphSetup(pConfigName, &dbGraph)) {
		// write the record to the default database
		status = AccessDefaultDatabase(WRITE, 0, &dbGraph); 
		if (status != TRUE) {
			RadReviewMsg(status, "when writing to Default database.  Parameters cannot be saved.");			
			return;
		}

	   /* ------------------------------------------------------------------
	    *	Write the changes to the .ini file
		* ----------------------------------------------------------------*/      
   		pRAD_Init->Save_Parameters();

		SaveDatabaseNow();
	}
}


// 01-21-00  Modified to delete parameters from dbVista database
void CPlotDlg::DeleteGraphSetup(CGStr *pConfigName)
{
	CGStr Name;
	char dbName[DRIVE_LEN+PATH_LEN+1];
	struct db_graph_dflt_rec dbGraph;
	int status;

	if (mpPlotDB != NULL) {
		strcpy(dbName, pConfigName->Get_sz());
		_strlwr(dbName);	// convert always to lower case before store
		MyStrnCpy(dbGraph.sz_config_name_key, dbName, 99);	// store only the first 99 chars
		// delete the record in the default database
		status = AccessDefaultDatabase(DELETE_REC, 0, &dbGraph); 
		if (status != TRUE) {
			RadReviewMsg(status, "when deleting in Default database.  Configuration will not be deleted.");			
			return;
		}
		SaveDatabaseNow();
	}
}


////////////////////////////////////////////////////////////////////////////////
//	Time Date Actions
// Changed with DATE
const char *CPlotDlg::RelDateToDateAndTimeStr(DATE dOffsetFromDB_StartDate)
{
	const char *pDateTimeStr = "";
	
	if (mpPlotDB != NULL) {
		pDateTimeStr = mpPlotDB->RelDateToDateAndTimeStr(dOffsetFromDB_StartDate);
	}
	return(pDateTimeStr);
}
// Changed with DATE
const char *CPlotDlg::RelDateToTimeStr(DATE dOffsetFromDB_StartDate)
{
	const char *pDateTimeStr = "";
	
	if (mpPlotDB != NULL) {
		pDateTimeStr = mpPlotDB->RelDateToTimeStr(dOffsetFromDB_StartDate);
	}
	return(pDateTimeStr);
}

// Changed with DATE
void CPlotDlg::SetDisplayDateTime(DATE dAbsDateStartDATE, DATE dReqViewPeriodDATE)
{
	if (mpDlg != NULL) {
		UINT uiViewPeriodIndex = 0;
		DATE dDaysInPeriod;
		BOOL bFinished = FALSE;
			
		// Caller can default the view period to the current view period by passing in 0.
		if (dReqViewPeriodDATE == 0) {
			dReqViewPeriodDATE = mdViewPeriodDays;
		}
	
		// Find the shortest predefined period that is >= requested period.
		do {
			dDaysInPeriod = naViewPeriod[uiViewPeriodIndex].dDaysInPeriod;
			if (dReqViewPeriodDATE <= dDaysInPeriod) bFinished = TRUE;
			else if (dDaysInPeriod == 0.0) bFinished = TRUE;
			else uiViewPeriodIndex++;
		} while (!bFinished);
	
		// If we didn't hit the end of the naViewPeriod array set the new view period array index.
		if (dDaysInPeriod > 0.0) {
			mdViewPeriodDays = naViewPeriod[uiViewPeriodIndex].dDaysInPeriod;
			mpDlg->PickListSetValue(IDC_VIEW_PERIOD_PL, uiViewPeriodIndex);
			// SetViewPeriodParameters(); this is called via notification fn on IDC_VIEW_PERIOD_PL control.
		}
		
		if (mpPlotDB != NULL) {
			DATE dDB_EndDate, dDB_StartDate;

			mpPlotDB->GetDateRange(&dDB_StartDate, &dDB_EndDate);
		    DATE dNewViewStart = dAbsDateStartDATE - dDB_StartDate;
			double dCurrentCursX = dNewViewStart;
            
			// If we are displaying a longer time period than requested, center the requested data
			// in the actual displayed period.
            if (dReqViewPeriodDATE < mdViewPeriodDays) {
            	dNewViewStart -= (mdViewPeriodDays - dReqViewPeriodDATE) / 2.0;
            	// Maybe set view start period to some "rational" time; e.g. seconds = 0
            	// (if this doesn't force req. period start off scale). Later - Tom - 10-Apr-97
            }
			
			SetCurrentViewStartDate(mpDlg, dNewViewStart);

			// Set the cursor on the first data point of the requested display period.
			// This is especially important if the view is greater than that requested.
			BOOL bOnNearestDataPt = TRUE;
			if (mpPlotPane != NULL) {
				mpPlotPane->SetCursorPos(dCurrentCursX, GUI_PLOT_CURRENT_AREA, bOnNearestDataPt);
			}
			TurnZoomOff();
		}
	}
}

// added by SFK
void CPlotDlg::GetDisplayDateTime(DATE *pdDisplayStartTime, DATE *pdViewPeriodDays)
{
	if (mpDlg != NULL) {
	
		*pdViewPeriodDays = mdViewPeriodDays;
		
		if (mpPlotDB != NULL) {
			DATE dDB_EndDate, dDB_StartDate;
			mpPlotDB->GetDateRange(&dDB_StartDate, &dDB_EndDate);
			*pdDisplayStartTime = dDB_StartDate + mdDspStartOffsetFromDbStart;
    	}
	}
}


// ===========================================================================
// ====================== Dlg for Setting Display Start Time =================
// ===========================================================================


void CPlotDlg::StartTimeSB_TrackingFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	int iNewDay, iNewHour, iNewMinute;
	GEN_DATE_STRUCT Date;
	GEN_TIME_STRUCT Time;
	//CMyDateTime MyDate;

	pDlg->ScrollBarGetValue(IDC_DAY_SB, &iNewDay);
	pDlg->ScrollBarGetValue(IDC_HOUR_SB, &iNewHour);
	pDlg->ScrollBarGetValue(IDC_MINUTE_SB, &iNewMinute);

	double dDBStartStart = *((double *)pDlg->GetUserData());
	double dIncrementInDays = (double)iNewMinute / (24.0 * 60.0) + (double)iNewHour / 24.0 + (double)iNewDay;
	double dNewDateTime = dDBStartStart + dIncrementInDays;
	glMyDateTime.DATETimestampToGenStructs(dNewDateTime, &Date, &Time);
	pDlg->FldDateSetValue(IDC_START_DATE, Date);
	pDlg->FldTimeSetValue(IDC_START_TIME, Time);
}
 

void CPlotDlg::CfgRadioBtnsNotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID ControlID)
{                              
	RESOURCE_ID BtnID;
	unsigned int uiDum;
	
	pDlg->RadioGroupGetValueAbs(IDC_GRAPH_CFG_GROUP, &BtnID);
	pDlg->RadioGroupGetValue(IDC_GRAPH_CFG_GROUP, &uiDum);
	if (BtnID == IDC_WRITE_CFG_RADIO) {
		pDlg->SetVisible(IDC_GRAPH_CONFIG_NAME_EDIT, TRUE);
		pDlg->SetEnable(IDC_GRAPH_CONFIG_NAME_EDIT, TRUE);
		pDlg->SetVisible(IDC_GRAPH_CONFIG_NAME_LIST, FALSE);
		pDlg->SetEnable(IDC_GRAPH_CONFIG_NAME_LIST, FALSE);
		pDlg->SetFocus(IDC_GRAPH_CONFIG_NAME_EDIT);
	}
	else {
		pDlg->SetVisible(IDC_GRAPH_CONFIG_NAME_EDIT, FALSE);
		pDlg->SetEnable(IDC_GRAPH_CONFIG_NAME_EDIT, FALSE);
		pDlg->SetVisible(IDC_GRAPH_CONFIG_NAME_LIST, TRUE);
		pDlg->SetEnable(IDC_GRAPH_CONFIG_NAME_LIST, TRUE);
		pDlg->SetFocus(IDC_GRAPH_CONFIG_NAME_LIST);
	}
	
}


// 2.06 Major rewrite of Graph Set Time and Options
// Changed with DATE
void CPlotDlg::MoreOptionsServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{                            
	const int iDAY_SB_LINE_INCR = 1;
	const int iDAY_SB_PAGE_INCR = 7;
	const int iHOUR_SB_LINE_INCR = 1;
	const int iHOUR_SB_PAGE_INCR = 4;
	const int iMINUTE_SB_LINE_INCR = 1;
	const int iMINUTE_SB_PAGE_INCR = 10;
	const double dTHUMB_FRAC = 0.0;
	int iDaySB_Value;
	int iHourSB_Value;
	int iMinuteSB_Value;
	GEN_DATE_STRUCT Date, OriginalDate;
	GEN_TIME_STRUCT Time, OriginalTime;
	CGUI_Dlg Dlg(IDD_MORE_GRAPH_OPTIONS);
	BOOL bUpdate, bUpdateOriginal;

	char **aszGraphList = NULL;
	
	int iNumDaysIn_DB;
	DATE dDB_EndDate, dDB_StartDate, dDateTimeAtDlgExit, dDateTimeAtDlgStart, dNewCursorDaysOffsetFromDbStart, dNewDateDaysOffsetFromDbStart;
	//CMyDateTime MyDate;


	if (Dlg.IsValid()) {
		// Main plot dialog is running modeless. We came here from there and cannot have it
		// disappear while here so disable it to prevent that from happening.
		pDlg->SetEnable(FALSE);

		Dlg.DefinePushBtn(IDOK, GUI_OK);
		Dlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);

		// setup stuff for the time part of the dialog box
		// get range of data in database

		mpPlotDB->GetDateRange(&dDB_StartDate, &dDB_EndDate);
		iNumDaysIn_DB = (int)ceil(dDB_EndDate - dDB_StartDate);	// round up to nearest whole day
		dDateTimeAtDlgStart = dDB_StartDate + mdDspStartOffsetFromDbStart;
		glMyDateTime.DATETimestampToGenStructs(dDB_StartDate, &Date, &Time);
		iDaySB_Value = (int)floor(mdDspStartOffsetFromDbStart);
		iHourSB_Value = Time.hr;
		Time.hr = 0;
		iMinuteSB_Value = Time.min;
		Time.min = 0;
		Dlg.SetUserData(&dDB_StartDate);
		Dlg.DefineScrollBar(IDC_DAY_SB, &iDaySB_Value, 0, iNumDaysIn_DB, iDAY_SB_LINE_INCR, iDAY_SB_PAGE_INCR, dTHUMB_FRAC);
		Dlg.DefineScrollBar(IDC_HOUR_SB, &iHourSB_Value, 0,	23, iHOUR_SB_LINE_INCR, iHOUR_SB_PAGE_INCR, dTHUMB_FRAC);
		Dlg.DefineScrollBar(IDC_MINUTE_SB, &iMinuteSB_Value, 0, 59, iMINUTE_SB_LINE_INCR, iMINUTE_SB_PAGE_INCR, dTHUMB_FRAC);
		Dlg.ScrollBarSetTrackingFn(IDC_DAY_SB, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::StartTimeSB_TrackingFn);
		Dlg.ScrollBarSetTrackingFn(IDC_HOUR_SB, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::StartTimeSB_TrackingFn);
		Dlg.ScrollBarSetTrackingFn(IDC_MINUTE_SB, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::StartTimeSB_TrackingFn);
	
		// setup the time edit fields
		glMyDateTime.DATETimestampToGenStructs(dDateTimeAtDlgStart, &Date, &Time);
		Dlg.DefineFldDate(IDC_START_DATE, &Date);
		Dlg.DefineFldTime(IDC_START_TIME, &Time);
		OriginalDate = Date;
		OriginalTime = Time;
		//setup the time radio buttons
		unsigned int uiTimeRadioBtnIndex = 0;
		RESOURCE_ID nauiTimeRadioBtns[] = {IDC_START_TIME_RADIO, IDC_CENTER_TIME_RADIO, NULL};
		Dlg.DefineRadioGroup(IDC_GRAPH_TIME_GROUP, nauiTimeRadioBtns, &uiTimeRadioBtnIndex, NULL);

		// setup stuff for configuration part of the dialog
		unsigned int uiCfgRadioBtnIndex = 0;	// always choose to do the read
		RESOURCE_ID nauiCfgRadioBtns[] = {IDC_READ_CFG_RADIO, IDC_WRITE_CFG_RADIO, IDC_UPDATE_CFG_RADIO, IDC_DELETE_CFG_RADIO, NULL};
		Dlg.DefineRadioGroup(IDC_GRAPH_CFG_GROUP, nauiCfgRadioBtns, &uiCfgRadioBtnIndex, CfgRadioBtnsNotifyFn);
		//read cfgs in db already; display the current one		
		if (CreateGraphDefaultNamesPicklist(&aszGraphList) != TRUE) return;
		unsigned int uiCfgChoice = 0;
//		while (aszGraphList[uiCfgChoice] != NULL) {
//			if (strcmp(aszGraphList[uiCfgChoice], mConfigName.Get_sz()) == 0) break;
//			uiCfgChoice++;
//		}
		Dlg.DefinePickList(IDC_GRAPH_CONFIG_NAME_LIST, aszGraphList, &uiCfgChoice);
		Dlg.DefineFldTxt(IDC_GRAPH_CONFIG_NAME_EDIT, &mConfigName);
		Dlg.SetVisible(IDC_GRAPH_CONFIG_NAME_EDIT, FALSE);
		Dlg.SetEnable(IDC_GRAPH_CONFIG_NAME_EDIT, FALSE);

		// Get the state of whether the real time update is enabled or not
		bUpdate = pRAD_Init->Get_RealTimeUpdate();
		Dlg.DefineCheckBox(IDC_UPDATE_GRAPH_CHECK, &bUpdate);
		bUpdateOriginal = bUpdate;


		Dlg.SetFocus(IDC_START_DATE);

		GUI_ACTION Ret = Dlg.Go();
		if (Ret == GUI_OK) {
			Dlg.RetrieveAllControlValues();

			pRAD_Init->Set_RealTimeUpdate(bUpdate);
			if (bUpdate) {
				mpDlg->SetText(IDC_MARK, "&Update Data");	// change the label
				mpDlg->SetCtrlNotifyFn(IDC_MARK, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::RealTimeUpdateBtnServiceFn);
			}
			else {
				mpDlg->SetText(IDC_MARK, "&Mark");	// change the label
				mpDlg->SetCtrlNotifyFn(IDC_MARK, this, (GUI_CB_DlgNotifyFn)&CPlotDlg::MarkCursorPosition);
			}
			if (bUpdate != bUpdateOriginal) {	// changed status of Update/Mark
				// save current graph setup to default
				mConfigName = "default";
				SaveGraphSetup(&mConfigName);
			}

			dDateTimeAtDlgExit = glMyDateTime.GenStructsToDATETimestamp(&Date, &Time);

			// check if user set new time or chose center display
			if ((dDateTimeAtDlgExit != dDateTimeAtDlgStart) || (uiTimeRadioBtnIndex != 0)){
				// set the graph times and cursor according to the new time specified
				if (uiTimeRadioBtnIndex == 1) {	// want to set a center value
					dDateTimeAtDlgExit = dDateTimeAtDlgExit - (mdViewPeriodDays/2.0);
				}
				dNewDateDaysOffsetFromDbStart = dDateTimeAtDlgExit - dDB_StartDate;
				dNewCursorDaysOffsetFromDbStart = dDateTimeAtDlgExit + (mdViewPeriodDays/2.0) - dDB_StartDate;
				SetCurrentViewStartDate(mpDlg, dNewDateDaysOffsetFromDbStart);
				SetDisplayCursorTime(dNewCursorDaysOffsetFromDbStart);	// 2.06 add position cursor in center
				// save current graph setup to default
				mConfigName = "default";
				SaveGraphSetup(&mConfigName);
			}

			// based on which cfg radio button is pushed, take an action
			unsigned int uiDum;
			
			switch (uiCfgRadioBtnIndex) {
			case 0:
				Dlg.PickListGetValue(IDC_GRAPH_CONFIG_NAME_LIST, &uiDum, &mConfigName);
				ReadGraphSetup(&mConfigName, FALSE);
				break;
			case 1:
				if (mConfigName.GetDim() != 0) SaveGraphSetup(&mConfigName);
				break;
			case 2:
				Dlg.PickListGetValue(IDC_GRAPH_CONFIG_NAME_LIST, &uiDum, &mConfigName);
				SaveGraphSetup(&mConfigName);
				break;
			case 3:
				Dlg.PickListGetValue(IDC_GRAPH_CONFIG_NAME_LIST, &uiDum, &mConfigName);
				if (uiDum == 0) {
					RadReviewMsg(uiDEFAULT_CANNOT_BE_DELETED);
				}
				else {
					DeleteGraphSetup(&mConfigName);
				}
				break;
			}

		}
		if (aszGraphList) gen_deallocate_str_array(aszGraphList);
		pDlg->SetEnable(TRUE);	// re-enable main plot dialog.
	}
}

