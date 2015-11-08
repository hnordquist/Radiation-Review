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

#ifndef PLT_DLG_H
	#define PLT_DLG_H

	static const UINT uiMAX_PLOT_TRACES = 10;
	static const ULONG ulSECS_PER_DAY = 24L * 60L * 60L;
	
//	struct Event {
//		ULONG ulRelDateFirst;
//		ULONG ulRelDateLast;
//		COLORREF Color;
//		CGStr Label;
//	};
	
//	typedef struct Event EVENTS;
	struct Event1 {
		DATE dRelDateFirst;
		DATE dRelDateLast;
		COLORREF Color;
		CGStr Label;
	};
	typedef struct Event1 EVENTS1;
	
//	struct Mark {
//		ULONG ulRelDateSec;
//	};
//	typedef struct Mark MARKS;

	struct Mark1 {
		DATE dRelDateSec;
	};
	typedef struct Mark1 MARKS1;
	
	static const UINT uiINIT_VIEW_PERIOD_INDEX = 10;	// One day (24 hours).
	static const UINT uiMAX_NUM_CHAN_AXES = 10;
	static const UINT uiMAX_ZOOM_FACTOR = 99;	// If this is 100 there is confusion with X100 checkbox.
	static const UINT uiMAX_LWR_RT_CTRLS = 25;
	
	
	class CPlotPane;
	class CPlotDB;
	class CEditEvents;
	
	// =====================================================================
	// =========================== CPlotDlg ================================
	// =====================================================================

	class CPlotDlg : public CGUI_CallBack
	{
		public:
			CPlotDlg(BOOL (*GraphDlgExitNotifyFn)(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction), BOOL *pbValidPlotDlg, void (*RightMouseCallBackFn)(double X, double Y, UINT uiAxis));
			~CPlotDlg();

			void Close();
			void DeleteScreenEvent(UINT uiTraceIndex, double dMarkerValueX, BOOL bRefreshDisplay);
			void GetDisplayDateTime(DATE *pdDisplayStartTime, DATE *pdViewPeriodDays);
			BOOL GetLogStyle() {return(mbLogY_Axis);};
			DATE GetViewPeriodInDays() {return(mdViewPeriodDays);}; // SFK made public
			void NextView();
			int NotifyBoxEdgePos(double dLeft, double dRight, double dTop, double dBottom);
			void NotifyGraphHeightChange();
			BOOL PotentialTraceIndexToTraceIndex(UINT uiChanIndex, UINT *puiTraceIndex, UINT *puiPL_ChanIndex = NULL);
			void PrevView();
			BOOL RefreshData();
			const char *RelDateToDateAndTimeStr(DATE dOffsetFromDB_StartDate);
			const char *RelDateToTimeStr(DATE dOffsetFromDB_StartDate);
			void ResizeNotifyFn(CGUI_Dlg *pDlg, int iDeltaDlgWidth, int iDeltaDlgHeight);
			void SetCursorInHiliteRegionFlag(BOOL bCursorInHiliteRegion);
			void SetCursorOnMarkerFlag(BOOL bCursorOnMarker);
			BOOL SetDisplayCursorTime(DATE dAbsCursorTime, int iPL_ChanIndex = -1);
			void SetDisplayDateTime(DATE dAbsDateStartDATE, DATE dReqViewPeriodDATE = 0.0);
			void SetFocusToGraph();	// SFK made public
			void SetViewPeriodParameters(); // SFK made public
			void SetZoomFactor(UINT uiZoomFactor);
			BOOL TraceIndexToPotentialTraceIndex(UINT uiTraceIndex, UINT *puiChanIndex, UINT *puiPL_ChanIndex = NULL);   // SFK made public
			BOOL UpdateAxisEvents(UINT uiTraceIndex, BOOL bRefreshDisplay = TRUE);
			void UpdateEditEventDlg();
			void UpdateGraphDisplay();
			BOOL GetCurrentGraphSetup(CGStr *pConfigName, struct db_graph_dflt_rec *pdbGraph);	// Added sfk 08-12-2004

			static COLORREF mMarkerTextColor;
			
		private:
			CGUI_Dlg *mpDlg;
			CPlotPane *mpPlotPane;
			CPlotDB *mpPlotDB;
			CGUI_Dlg *mpHelpDlg;
			CGUI_Dlg *mpOptionsDlg;
			CEditEvents *mpEditEvents;
			//long mlDspStartOffset;	// Offset from mulDB_StartDate to earliest
									// displayed point (secs.). ReadDbDataArray
									// uses a float array for the date -- the
									// faDate[0] element has a value of
									// mlDspStartOffset corresponding to
									// absolute time of
									// mulDB_StartDate + mlDspStartOffset.
									// This parameter may be negative because it is
									// someties desireable for the user to be able to
									// have the display start before the fist data in the DB.
			DATE mdDspStartOffsetFromDbStart;
			UINT muiNumLwrRtCtrls;
			BOOL mbLogY_Axis;
			BOOL mbChansOverlapped;
			DATE mdViewPeriodDays;
			RESOURCE_ID maCheckBoxID[uiMAX_PLOT_TRACES];
			RESOURCE_ID maPickListID[uiMAX_PLOT_TRACES];
			RESOURCE_ID maLwrRtCtrlID[uiMAX_LWR_RT_CTRLS];
			UINT mauiPickListIndex[uiMAX_PLOT_TRACES];
			BOOL mabShowDet[uiMAX_PLOT_TRACES];
			int miDspStartLineIncrementMins;	// If there are too many minutes worth
												// of data for max. SB range each SB
												// increment will represent this many minutes.
			CGStr mConfigName;

			
			void SetLogLinearStyle(BOOL bLogY_Axis);
			void InitializeGraphDlgCtrls();
			void AddInitVisibleChannels();
			void DeleteAllVisibleChannels(); // 2.06 sfk
			BOOL GetDB_Parameters();
			BOOL IsCursorOnMark();
			void AddScreenMark(UINT uiTraceIndex, double dMarkerValueX, BOOL bRefreshDisplay);
			void DeleteScreenMark(UINT uiTraceIndex, double dMarkerValueX, BOOL bRefreshDisplay);

			// Dialog control service functions.
			static void CfgRadioBtnsNotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);	//SFK 2.06
			BOOL DlgExitValidateFn(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);
			void EditEventBtnServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void GraphOptionsDlg(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void HelpBtnServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void HideBottomDetPickListCtrls(CGUI_Dlg *pDlg);
			void LogLinearToggleServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void MarkCursorPosition(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void MoreOptionsServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID); // SFK 2.06
			void NextViewServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void PrevViewServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void PrintGraphServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void RealTimeUpdateBtnServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID); //SFK
			void SetCurrentViewStartDate(CGUI_Dlg *pDlg, DATE dNewViewPeriodOffsetDate);
			void SetStartTimeServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void StartTimeSB_TrackingFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void ViewPeriodNotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void ViewPeriodSB_NotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void ViewPeriodSB_TrackingFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);

			void CancelGraphReadServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID ID);
			void ChanEnableCB_NotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);
			BOOL DataGraphExitValidateFn(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);
			BOOL HelpDlgExitValidateFn(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);
			void SuppressUpdateCB_NotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);
			void WhichDetPL_NotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);
			void (*mpGraphDlgExitNotifyFn)();
			
			//void OptionBtnServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID); // SFK 2.06
			//void SetCenterTimeServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID); // SFK 2.06
			//void SetTimeServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, BOOL bCenter); // SFK 2.06
			//BOOL OptionsExitNotifyFn(CGUI_Dlg *pDlg, GUI_ACTION Action);	//SFK 2.06
			//void SaveGraphOptionsServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);	//SFK 2.06
			//void ReadGraphOptionsServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);	// SFK 2.06
			//static void CfgPickListNotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);	//SFK 2.06



			// Zoom routines and parameters
			void EchoZoomFactor(UINT uiZoomFactor);
			void SetZoomCtrlsEnable(CGUI_Dlg *pDlg);
			void TurnZoomOff();
			void ZoomInServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void ZoomOutServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void ZoomFactorSB_NotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void ZoomFactorSB_TrackingFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void ZoomTimes100ServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void ZoomAutoScaleServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			
			BOOL mbZoomAutoScale;
			BOOL mbZoomTimes100;
			UINT muiZoomFactor;


			// View period fns.
			void EchoViewPeriodSB_Value(DATE dOffsetFromDB_StartDate);
			void SetViewPeriodSB_Value(DATE dOffsetFromDB_StartDate);

			// Other Assorted Stuff
			BOOL mbCursorInHiliteRegion;
			BOOL mbCursorOnMarker;
			BOOL mbAbortUpdate;

			UINT GetNumAreas();
			void SuppressUpdate(BOOL bSuppressUpdate); // 2.06 SFK
			BOOL UpdateAxisData(UINT uiAxisIndex, BOOL bRefreshDisplay,	class TwoLineProgressBar *pProgress = NULL,	ULONG *pulNumPrevPts = NULL, double dFracBefore = 0.0, double dFracAfter = 1.0);
			BOOL UpdateIsSuppressed();

			BOOL GetPL_Index(RESOURCE_ID ControlID, UINT *puiPL_Index, UINT *puiActivePL_Index);
			BOOL GetCB_Index(RESOURCE_ID ControlID, UINT *puiCB_Index);
			
			void DeleteGraphSetup(CGStr *pConfigName);	// sfk 2.06
			void ReadGraphSetup(CGStr *pConfigName, BOOL bStartTime);	// 2.06
			void SaveGraphSetup(CGStr *pConfigName);

			bool mUsingTA; // true if timealign is active for at least one trace on the graph
			void SetDialogTAFlag(bool activeTA);
	};

#endif
