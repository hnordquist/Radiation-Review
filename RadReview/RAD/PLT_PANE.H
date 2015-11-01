
// PLT_PANE.h


#ifndef PLT_PANE_H
	#define PLT_PANE_H

	#include "gui_plot.h"
	#include "plt_dlg.h"	// For MarkerZones definiton.
	
	// =====================================================================
	// =========================== CPlotPane ===============================
	// =====================================================================

	class CPlotPane : public CGUI_PlotPane
	{
		public:
			CPlotPane(CGUI_Dlg *pDlg, CPlotDlg *pPlotDlg, RESOURCE_ID PaneID, void (*RightMouseCallBackFn)(double X, double Y, UINT uiAxisNum));
			virtual ~CPlotPane();
			BOOL InsertNewArea(UINT uiAreaIndex, BOOL bLogY_Axis);
			BOOL DeleteArea(UINT uiAreaIndex);
			BOOL RefreshData();
//			BOOL UpdateAxisEvents(UINT uiAxisIndex, BOOL bRefreshDisplay, ULONG ulNumEvents, EVENTS *aEvents);
//			BOOL UpdateAxisData(UINT uiAxisIndex, float huge *faCounts,	float huge *faDates, ULONG ulNumPts, const CGStr &DetName, BOOL bRefreshDisplay, ULONG ulNumEvents, EVENTS *aEvents, ULONG ulNumMarks, MARKS *aMarks);
			// add prototype using doubles in 2.06f
//			BOOL UpdateAxisData(UINT uiAxisIndex, double huge *daCounts, double huge *daDates, ULONG ulNumPts, const CGStr &DetName, BOOL bRefreshDisplay, ULONG ulNumEvents, EVENTS *aEvents, ULONG ulNumMarks, MARKS *aMarks);
			BOOL UpdateAxisEvents(UINT uiAxisIndex, BOOL bRefreshDisplay, ULONG ulNumEvents, EVENTS1 *aEvents);
			BOOL UpdateAxisData(UINT uiAxisIndex, float huge *faCounts,	float huge *faDates, ULONG ulNumPts, const CGStr &DetName, BOOL bRefreshDisplay, ULONG ulNumEvents, EVENTS1 *aEvents, ULONG ulNumMarks, MARKS1 *aMarks);
			// add prototype using doubles in 2.06f
			BOOL UpdateAxisData(UINT uiAxisIndex, double huge *daCounts, double huge *daDates, ULONG ulNumPts, const CGStr &DetName, BOOL bRefreshDisplay, ULONG ulNumEvents, EVENTS1 *aEvents, ULONG ulNumMarks, MARKS1 *aMarks);
			void SetPlotObjOptions(BOOL bVisible, BOOL bEnbResize, BOOL bEnbBoxNew, BOOL bEnbMove);
			void DeletePlObj();
			static int PlotLibObjCallBackFn(PL_CTX *pPlCtx, void *pUserData, PL_OBJ_CTX *pObjCtx, PL_OBJ *pObj, int eEvent, double dArg1, double dArg2);
	
		protected:
			virtual void UpdateDisplay(PL_CTX *pPlCtx, BOOL bEraseFirst);
			virtual BOOL ServiceRightMouseBtn(UINT uiWhichAxis, double dXValue, double dYValue);
			virtual void GetAnnotationX(double dValue, CGStr *pAnnotationStr, BOOL bForCursorValue);
			virtual void GetAnnotationY(double dValue, CGStr *pAnnotationStr, BOOL bForCursorValue);
			virtual BOOL DisplayCursorReadoutText(PL_CTX *pPlCtx, const CGStr &AreaName, UINT uiCursorArea, double dCursorX, double dCursorY, BOOL bInterpolatedY, BOOL bCursorInRange, CGFmtStr *pCursorReadoutText, COLORREF *pCursorReadoutColor);
			virtual void ResizeNotifyFn(CGUI_Dlg *pDlg, int iDeltaDlgWidth,	int iDeltaDlgHeight);
			virtual void ZoomChangeNotify(UINT uiNewZoomFactorX);
			virtual LRESULT PlotLibEventPreFilter(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
			
		private:
			void SetVertAxisStyle(int iVerticalLogOrLinear, BOOL bRedrawGraph);
			void SetLogLinearStyle(int iVerticalLogOrLinear);
			ULONG FindArrayIndexGivenValue(float fValueX, float huge *afHugeData, ULONG ulArrayDim);
			void (*mRightMouseCallBackFn)(double X, double Y, UINT uiAxis);
			CPlotDlg *mpPlotDlg;
			CGUI_Dlg *mpDlg;
			BOOL mbFirstDisplayCursorReadoutText;
	};

#endif

