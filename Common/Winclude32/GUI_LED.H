
#ifndef GUI_LED_H
	#define GUI_LED_H

	class CGUI_StatusLight : public zPane
	{
		public:
			CGUI_StatusLight(zWindow *pParent, const zRect &DspRect, const CGStr &Text, COLORREF Color,
				RESOURCE_ID CtrlID = -1, RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			CGUI_StatusLight(zWindow *pParent, RESOURCE_ID ResourceID, COLORREF Color,
				RESOURCE_ID CtrlID = -1, RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			void GetColor(COLORREF *pColor);
			void SetColor(COLORREF Color);
			~CGUI_StatusLight();
			void InitializeRects(const zRect &DspRect);
			const static COLORREF LED_RED;
			const static COLORREF LED_GREEN;
			const static COLORREF LED_YELLOW;
			const static COLORREF LED_ON;
			const static COLORREF LED_OFF;
			void Paint();
			int draw(zDrawEvt *pEvent = NULL);
			virtual int mouseButtonUp(zMouseClickEvt *);
#ifdef GUI_LIB_PRIVATE		
		private:
			COLORREF mBkgColor1;
			COLORREF mBkgColor2;
			COLORREF mBkgColor3;
			COLORREF mColor;
			CGStr mText;    
			zRect mDspRect;
			zRect mLED_Rect;
			zRect mTextRect;
			zPoint mTextOrigin;
			RESOURCE_ID mCtrlID;
			RESOURCE_ID mPopupMenuID;
			void (*mpPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID);
#endif // GUI_LIB_PRIVATE
	};

#endif
	
