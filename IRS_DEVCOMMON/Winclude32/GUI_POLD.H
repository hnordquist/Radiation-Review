
// GUI_PANE.h

#if !defined GUI_PANE_H
	#define GUI_PANE_H

	#include "gui_ctrl.h"
	
	class CGUI_DlgCtrl;
	class CGUI_Dlg;

	// ===============================================================
	// ======================= CCtrlPane ============================
	// ===============================================================
	
	class CGUI_Pane : public zPane
	{
		public:
			CGUI_Pane(CGUI_Dlg *pDlg, const zRect &DspRect, RESOURCE_ID CtrlID,
				void (*DrawFn)(CGUI_Dlg *pDlg, RESOURCE_ID, zDisplay *pCanvas),
				COLORREF BkgColor, UINT uiFrameWidth = 1, int iBevelWidth = 0,
				BOOL bEraseBkg = TRUE);
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
#endif	// GUI_LIB_PRIVATE
	};
	
	
	class CCtrlPane : public CGUI_DlgCtrl
	{
		public:
			CCtrlPane(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID,
				void (*DrawFn)(CGUI_Dlg *pDlg, RESOURCE_ID, zDisplay *pCanvas),
				COLORREF BkgColor = RGB(0, 0, 0), UINT uiFrameWidth = 0,
				int iBevelWidth = 0, BOOL bEraseBkg = TRUE);
			virtual ~CCtrlPane();
			UINT GetBorderWidth();
			virtual void Update();
			virtual BOOL RetrieveUserData();
			BOOL Print();
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
				COLORREF FG_Color = RGB(192, 192, 192), COLORREF BG_Color = RGB(128, 128, 128));
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
	

