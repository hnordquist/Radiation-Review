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

#ifndef GUI_PLOT_H
	
	#define GUI_PLOT_H
	
	#include <pl.h>
	class CPlotArea;
	const UINT uiMAX_NUM_PLOT_AREAS = 10;
	const UINT GUI_PLOT_ALL_AREAS = UINT_MAX;
	const UINT GUI_PLOT_CURRENT_AREA = UINT_MAX - 1;
    
	// Derive a class from CGUI_CallBack so that we can have callback fns
	// which are member functions of the CGUI_PlotPane class rather than "C"
	// callback fns. This allows better encapsulation of the plot details.
	
	class CGUI_PlotPane : public CGUI_CallBack
	{
		public:
			CGUI_PlotPane(CGUI_Dlg *pDlg, RESOURCE_ID PaneID);
			virtual ~CGUI_PlotPane();

			// Add, remove or change data for plot areas.
			BOOL AddArea(UINT *puiAreaIndex, double huge *adX_Data, double huge *adY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel);
			BOOL AddArea(UINT *puiAreaIndex, float huge *afX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel);
			BOOL AddArea(UINT *puiAreaIndex, ULONG huge *aulX_Data, ULONG huge *aulY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel);
			BOOL AddArea(UINT *puiAreaIndex, ULONG huge *aulX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel);

			BOOL InsertArea(UINT uiAreaIndexBefore, UINT *puiAreaIndex);
			BOOL RemoveArea(UINT uiAreaIndex);

			BOOL UpdateAreaData(UINT uiAreaIndex, double huge *adX_Data, double huge *adY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel = NULL, BOOL bUpdateDsp = TRUE);
			BOOL UpdateAreaData(UINT uiAreaIndex, float huge *afX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel = NULL, BOOL bUpdateDsp = TRUE);
			BOOL UpdateAreaData(UINT uiAreaIndex, ULONG huge *aulX_Data, ULONG huge *aulY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel = NULL, BOOL bUpdateDsp = TRUE);
			BOOL UpdateAreaData(UINT uiAreaIndex, ULONG huge *aulX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel = NULL, BOOL bUpdateDsp = TRUE);

			// Y-axis control.
			BOOL GetStyleY(UINT uiAreaIndex, BOOL *pbLogY_Axis, BOOL *pbZoomAutoScale, int *piMajorGridLines = NULL);
			BOOL SetStyleY(UINT uiAreaIndex, BOOL bLogY_Axis = FALSE, BOOL bZoomAutoScale = TRUE, int iMajorGridLines = 4);
			BOOL SetLogY(UINT uiAreaIndex = GUI_PLOT_ALL_AREAS, BOOL bLogY_Axis = FALSE);
			BOOL SetZoomAutoScaleY(UINT uiAreaIndex = GUI_PLOT_ALL_AREAS, BOOL bZoomAutoScale = TRUE);
			BOOL GetLimitsY(UINT uiAreaIndex, BOOL *pbAutoScale, double *pdY_Min, double *pdY_Max);
			BOOL SetLimitsY(UINT uiAreaIndex = GUI_PLOT_ALL_AREAS, BOOL bAutoScale = TRUE, double dY_Min = 0.0, double dY_Max = 0.0);

			// X-axis control.
			UINT GetZoomFactorX();
			void SetZoomFactorX(UINT uiZoomFactor);
			void GetLimitsX(double *pdMinFullX, double *pdMaxFullX, double *pdMinVisibleX = NULL, double *pdMaxVisibleX = NULL);
			void SetLimitsX(BOOL bAutoScaleX, double dX_Min, double dX_Max);

			// Cursor routines.
			void CenterCursor(UINT uiAreaIndex = GUI_PLOT_CURRENT_AREA);
			void GetCursorPos(double *pdCursorX, double *pdCursorY = NULL, UINT *puiAreaIndex = NULL);
			void GetCursorUsingIndex(ULONG *pdCursorIndexX,	UINT *puiAreaIndex = NULL);
			void SetCursorPos(double dCurrentCursX, UINT uiAreaIndex = GUI_PLOT_CURRENT_AREA, BOOL bOnNearestDataPt = TRUE, BOOL bUpdateDisplay = TRUE);
			void SetCursorUsingIndex(ULONG ulNewCursIndex, UINT uiAreaIndex = GUI_PLOT_CURRENT_AREA, BOOL bUpdateDisplay = TRUE);
			void MoveCursor(double dStepsX);	// moved to public by SFK
			

			// Highlighted Regions
//			BOOL AddHiliteRegion(UINT uiAreaIndex, ULONG ulFirstPtIndex, ULONG ulLastPtIndex, COLORREF Color, const char *szLabel = NULL);
			BOOL AddHiliteRegion(UINT uiAreaIndex, DATE dFirstPtXValue, DATE dLastPtXValue, COLORREF Color,	const char *szLabel = NULL);
			void ClearHiliteRegions(UINT uiAreaIndex);
			BOOL DeleteHiliteRegion(UINT uiAreaIndex, ULONG ulFirstPtIndex,	ULONG ulLastPtIndex);
			BOOL DeleteHiliteRegion(UINT uiAreaIndex, double dMarkerValueX);
			virtual BOOL NextHiliteRegion(BOOL bMoveToNextRegion = TRUE, double *pdNextRegLowX = NULL, double *pdNextRegHighX = NULL);
			virtual BOOL PrevHiliteRegion(BOOL bMoveToNextRegion = TRUE, double *pdPrevRegLowX = NULL, double *pdPrevRegHighX = NULL);
			void ShowHiliteRegions(UINT uiAreaIndex, BOOL bVisible);
            BOOL IsCursorInHiliteRegion(UINT *puiAreaIndex, double *pdLowValueX = NULL, double *pdHighValueX = NULL, COLORREF *pHiliteColor = NULL, CGStr *pLabel = NULL);

			BOOL EmphasizeRegion(UINT uiAreaIndex, double dMinX, double dMaxX, BOOL bNearestActualPt, COLORREF Color, float fLineThicknessPts, double *pdActualMinX = NULL, double *pdActualMaxX = NULL);
                
			// Markers
			BOOL AddMarker(UINT uiAreaIndex, double dMarkerLocX, const char *szLabel, COLORREF TextColor = GUI_BLACK);
			void ClearMarkers(UINT uiAreaIndex);
			BOOL DeleteMarker(UINT uiAreaIndex,	double dMarkerLocX);
			void ShowMarkers(UINT uiAreaIndex, BOOL bVisible);
            BOOL IsCursorOnMarker(UINT *puiAreaIndex = NULL, double *pdMarkerLocX = NULL, COLORREF *pMarkerTextColor = NULL);

			// General.
			PL_CTX *GetPlotContext();	// Or use protected data member mPlCtx.
			CGUI_Dlg *GetParentDlg();	// Or use protected data member mpDlg.
			void InvalidateGraphDisplay();
			void Print(const char *szHeader = NULL, const char *szFooter = NULL);
			void SetFocus();
			BOOL SetUserAnnotation(BOOL bUserAnnotX, BOOL bUserAnnotY);
			virtual void ShowPlotLibErrMsg(int iStatus);
			void DeletePlotLibErrMsgWin();
			BOOL SuppressUpdate(BOOL bSuppressUpdate);
			BOOL UpdateIsSuppressed() {return(mbSuppressUpdate);};
			BOOL IsPrinting() {return(mbPrinting);};
			
			UINT GetNumAreas();
			UINT GetCurrentArea();
			BOOL SetCurrentArea(UINT uiAreaIndex);
			BOOL SetCursorVisible(BOOL bVisible);

			void RefreshDisplay(PL_CTX *pCtx = NULL, BOOL bEraseFirst = TRUE);

			// Set dialog controls "glue" for dialog box resizing.
			BOOL RegisterRightCtrls(RESOURCE_ID *naCtrlID, BOOL bClearPrevList = FALSE);
			BOOL RegisterRightCtrls(RESOURCE_ID *aCtrlID, UINT uiNumCtrls, BOOL bClearPrevList = FALSE);
			BOOL RegisterBottomCtrls(RESOURCE_ID *naCtrlID, BOOL bClearPrevList = FALSE);
			BOOL RegisterBottomCtrls(RESOURCE_ID *aCtrlID, UINT uiNumCtrls, BOOL bClearPrevList = FALSE);
			BOOL RegisterBottomRightCtrls(RESOURCE_ID *naCtrlID, BOOL bClearPrevList = FALSE);
			BOOL RegisterBottomRightCtrls(RESOURCE_ID *aCtrlID, UINT uiNumCtrls, BOOL bClearPrevList = FALSE);
			
			static COLORREF GetGraphBkgColor() {return(mGraphBkgColor);};
			static COLORREF GetGraphFG_Color() {return(mGraphFG_Color);};
			static COLORREF GetTraceColor() {return(mTraceColor);};
			static COLORREF GetLabelColor() {return(mLabelColor);};
			static float GetLinePtSize() {return(mfLinePtSize);};
			static float GetRegionLinePtSize() {return(mfRegionLinePtSize);};
			static float GetTextPtSize() {return(mfTextPtSize);};
			static int GetDrawType() {return(meDrawType);};
			static const char *GetTypeFace() {return(mszTypeFace);};

		
		protected:
			virtual void GetAnnotationX(double dValue, CGStr *pAnnotationStr, BOOL bForCursorValue = FALSE);
			virtual void GetAnnotationY(double dValue, CGStr *pAnnotationStr, BOOL bForCursorValue = FALSE);
			virtual BOOL DisplayCursorReadoutText(PL_CTX *pPlCtx, const CGStr &AreaName, UINT uiCursorArea, double dCursorX, double dCursorY, BOOL bInterpolatedY, BOOL bCursorInRange, CGFmtStr *pCursorReadoutText, COLORREF *pCursorReadoutColor);
			virtual LRESULT PlotLibEventPreFilter(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
			virtual void ResizeNotifyFn(CGUI_Dlg *pDlg, int iDeltaDlgWidth, int iDeltaDlgHeight);
			virtual BOOL ServiceRightMouseBtn(UINT uiWhichArea, double dXValue, double dYValue);
			virtual void UpdateDisplay(PL_CTX *pPlCtx, BOOL bEraseFirst = TRUE);
			virtual void ZoomChangeNotify(UINT uiNewZoomFactorX);

			CGUI_Dlg *mpDlg;
			PL_CTX mPlCtx;
			PL_CTX *mpPlPrtCtx;		// sfk added to remember printing context for callbacks 2.07
			// virtual void MouseClickedNotify(double dCursorX, double dCursorY, UINT uiAreaIndex);
			// virtual void MouseDblClickedNotify(double dCursorX, double dCursorY, UINT uiAreaIndex);
			// virtual void MouseDraggedNotify(double dLeftX, double dUpperY, double dRightX,
			//	double dLowerY, UINT uiAreaIndex);

			static COLORREF mGraphBkgColor;
			static COLORREF mGraphFG_Color;
			static COLORREF mCursorColor;	
			static COLORREF mCursorReadoutColor;
			static COLORREF mTraceColor;
			static COLORREF mLabelColor;
			static char *mszTypeFace;
			static float mfTextPtSize;
			static int meDrawType;
			static float mfLinePtSize;
			static float mfRegionLinePtSize;
			static float mfRowSpacingPts;	// Spacing between plot areas.
			// Log Y axis
			static float mfAreaY_AnnotDegLog;	// 90 => vertical; 0 => horizontal
			static int miNumColHorzY_AnnotLog;	// 0 for vertical OR number of y-annot chars
			static char *mszY_AnnotDefaultFmtLog;	// PL_FMT_TENTO or PL_FMT_AUTO etc.
			// Linear Y axis
			static float mfAreaY_AnnotDegLinear;	// 90 => vertical; 0 => horizontal
			static int miNumColHorzY_AnnotLinear;	// 0 for vertical OR number of y-annot chars
			static char *mszY_AnnotDefaultFmtLinear;	// PL_FMT_TENTO or PL_FMT_AUTO etc.

		private:
			// static member functions to register as call back fns to plot library.
			static LRESULT PlotLibEventPreFilterCallBackFn(PL_CTX *pPlot, void *pUserData, HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam, int bDone);
			static void PlotLibPlotCallBackFn(PL_CTX *pCtx, void *pUserData);
			//static int CGUI_PlotPane::PlotLibObjCallBackFn(PL_CTX *pPlCtx, void *pUserData,
			//	PL_OBJ_CTX *pObjCtx, PL_OBJ *pObj, int eEvent, double dArg1, double dArg2);
			static void PlotLibCursorCallBackFn(PL_CTX *pPlot, void *pUserData, int eEvent, double dArg1, double dArg2);
			static const char *PlotLibX_AxisAnnotCallBackFn(PL_CTX *pCtx, void *pUserData, double dX);
			static const char *PlotLibY_AxisAnnotCallBackFn(PL_CTX *pCtx, void *pUserData, double dY);

			BOOL mbAutoScaleX;
			double mdManualMinX;
			double mdManualMaxX;
			void GetFirstAndLastAreaIndex(UINT uiAreaIndex, UINT *puiFirstArea, UINT *puiLastArea);
			BOOL mbSuppressUpdate;
			BOOL RegisterNewPlotArea(CPlotArea *pPlotArea, UINT *puiAreaIndex, UINT uiNewAreaIndex);
			BOOL SetRangeX();
			BOOL SetRangeY(UINT uiAreaIndex);
			void CursorCallBackFn(PL_CTX *pPlot, int eEvent, double dArg1, double dArg2);
			BOOL mbCursorVisible;
			int miMajorGridLines;

			void UpdateCursorReadout(PL_CTX *pPlCtx);
	
			static void FmtDoubleValue(double dValue, CGStr *pStr);
			BOOL AutoY_ScaleVisibleRange(double dCurrentAnnotX);
	
			BOOL mbPrinting;
			CGUI_TextWin *mpPlotLibMsgWin;
	
			void MoveCursorToValueX(double dX);
			void ShowCursor(BOOL bShow, PL_CTX *pPlCtx = NULL);
			BOOL RefreshData();
			void SetNumAreas(UINT uiNumAreas);
	
			UINT muiZoomFactor;
			BOOL mbAbortUpdate;
			BOOL mbRefreshBlocked;
			double mdCurrentCursX;
	
			BOOL GetAreaPlot(UINT uiAreaIndex, CPlotArea **ppAreaPlot);
			void InitializeGraphCtrls();
	
			//BOOL mbGraphHasFocus;  //pjm 20 Dec 2007 Resolution of issue 82 eliminates need for this variable
			UINT muiNumAreas;
			CPlotArea *mapCPlotAreaAxes[uiMAX_NUM_PLOT_AREAS];
			UINT muiCurrentArea;
			ULONG mulCursorIndex;
			RESOURCE_ID mPaneID;
			BOOL mbValidPlotPane;
			BOOL mbUserAnnotX;
			BOOL mbUserAnnotY;

			RESOURCE_ID *mnaRightCtrlID;
			RESOURCE_ID *mnaBottomCtrlID;
			RESOURCE_ID *mnaBottomRightCtrlID;
	};

#endif

