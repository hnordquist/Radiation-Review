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

