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

// GUI_PANE.h

#if !defined GUI_PANE_H
	#define GUI_PANE_H

	#include "gui_ctrl.h"
	
	class CGUI_DlgCtrl;
	class CGUI_Dlg;

	// ===============================================================
	// ======================= CGUI_Pane ============================
	// ===============================================================
	
	class CGUI_Pane : public zPane
	{
		public:
			CGUI_Pane(CGUI_Dlg *pDlg, const zRect &DspRect, RESOURCE_ID CtrlID,
				void (*DrawFn)(CGUI_Dlg *pDlg, RESOURCE_ID, zDisplay *pCanvas),
				COLORREF BkgColor, UINT uiFrameWidth = 1, int iBevelWidth = 0,
				BOOL bEraseBkg = TRUE,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CGUI_Pane();
			void Paint();
			void SetBorder(UINT uiFrameWidth, int iBevelWidth);
			static void Display3DFrame(zDisplay *pCanvas, const zRect &DspRect,
				UINT uiFrameThickness, int iBevelThickness, COLORREF FaceColor,
				BOOL bEraseBkg = TRUE);
			// Display text centered on entire "face" of pane.
			void DisplayText(zDisplay *pCanvas, const CGStr &Text,
				COLORREF TextColor,	BOOL bGrayedText);
			// Display text centered within specified rectangle (with uiBorderWidth) of pane.
			void DisplayText(zDisplay *pCanvas, const zRect &DspRect, UINT uiBorderWidth,
				const CGStr &Text, COLORREF TextColor,	BOOL bGrayedText);
			// Display bitmap on entire "face" of pane.
			void DisplayBitmap(zDisplay *pCanvas, const zBitmap *pBitmap,
				GUI_BITMAP_DSP_MODE eBitmapDspMode);
			// Display bitmap within specified rectangle (with uiBorderWidth) of pane.
			void DisplayBitmap(zDisplay *pCanvas, const zRect &DspRect, UINT uiBorderWidth,
				const zBitmap *pBitmap, GUI_BITMAP_DSP_MODE eBitmapDspMode = GUI_BITMAP_FILL);
			static COLORREF GetNearestSolidColor(zDisplay *pCanvas, COLORREF InColor);
			COLORREF GetNearestSolidColor(COLORREF InColor);
//			BOOL Print(CGUI_PrintJob **ppPrintJob);
			BOOL Print();
			UINT GetBorderWidth();
			virtual int mouseButtonUp(zMouseClickEvt *);

#ifdef GUI_LIB_PRIVATE		
		private:
			void (*mpUserDrawFn)(CGUI_Dlg *pDlg, RESOURCE_ID ID,
				zDisplay *pCanvas);
			virtual int draw(zDrawEvt *pEvent = NULL);
			virtual int size(zSizeEvt *pEvent = NULL);
			virtual int background(zEvent *pEvent);
			UINT muiFrameWidth;
			int miBevelWidth;
			UINT muiBorderWidth;
			BOOL mbEraseBkg;
			COLORREF mBkgColor;
			RESOURCE_ID mCtrlID;
			zRect mDspRect;
			CGUI_Dlg *mpDlg;
			virtual int print(zPrinterDisplay *pPD, zRect *);
			RESOURCE_ID mPopupMenuID;
			void (*mpPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID);
#endif	// GUI_LIB_PRIVATE
	};
	
	
	class CCtrlPane : public CGUI_DlgCtrl
	{
		public:
			CCtrlPane(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID,
				void (*DrawFn)(CGUI_Dlg *pDlg, RESOURCE_ID, zDisplay *pCanvas),
				COLORREF BkgColor = RGB(0, 0, 0), UINT uiFrameWidth = 0,
				int iBevelWidth = 0, BOOL bEraseBkg = TRUE,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			virtual ~CCtrlPane();
			UINT GetBorderWidth();
			virtual void Update();
			virtual BOOL RetrieveUserData();
			BOOL Print();
			virtual BOOL SetPosition(UINT uiLeft, UINT uiTop);
			virtual BOOL SetSize(UINT uiWidth, UINT uiHeight);
			CGUI_Pane *mpPane;

#ifdef GUI_LIB_PRIVATE		
		private:
			UINT muiFrameWidth;
			int miBevelWidth;
			UINT muiBorderWidth;
#endif	// GUI_LIB_PRIVATE
	};


	class CCtrlProgressBar : public CCtrlPane
	{
		public:
			CCtrlProgressBar(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, BOOL bEchoPercent,
				COLORREF FG_Color = RGB(192, 192, 192), COLORREF BG_Color = RGB(128, 128, 128),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlProgressBar();
			BOOL SetValue(double dFraction);
			BOOL SetColors(COLORREF FG_Color, COLORREF BG_Color);
			
#ifdef GUI_LIB_PRIVATE		
		private:   
			COLORREF mFG_FaceColor;
			COLORREF mBG_FaceColor; 
			COLORREF mTextColor;                                                        
			void ComputeTextColor();
			static UINT muiFrameWidth;
			static int miBevelWidth;
			static COLORREF BarColor;
			static COLORREF BkgColor;
			double mdFraction;
			double mdLastPaintFraction;
			BOOL mbEchoPercent;
			double GetValue();
			static void DrawFn(CGUI_Dlg *pDlg, RESOURCE_ID, zDisplay *pCanvas);
#endif	// GUI_LIB_PRIVATE
	};


#endif
	

