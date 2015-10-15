
// GUI_CTRL.h

#if !defined GUI_CTRL_H
	#define GUI_CTRL_H

	class CGUI_Dlg;
	class CGUI_DlgCtrl;

	#include "gui_lib.h"

	// ===============================================================
	// ========================= CGUI_DlgCtrl ========================
	// ===============================================================
	
	class CGUI_DlgCtrl : public zEvH
	{                          
		public:
			CGUI_DlgCtrl(DLG_CTRL_TYPE CtrlType, CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			virtual ~CGUI_DlgCtrl();
			void GetOrigCtrlSpecs(int *piLeft, int *piTop, UINT *puiWidth = NULL, UINT *puiHeight = NULL);
			void RecordOrigCtrlSpecs();
			virtual BOOL GetPosition(int *piLeft, int *piTop, BOOL bClientRelative = TRUE);
			virtual BOOL SetPosition(int iLeft, int iTop);
			virtual BOOL GetSize(UINT *puiWidth, UINT *puiHeight);
			virtual BOOL SetSize(UINT uiWidth, UINT uiHeight);
			BOOL GetInteriorRect(zRect *pRect);
			BOOL GetExteriorRect(zRect *pRect);
			void RegisterOrigPosition();

			virtual BOOL IsHelpBtn();
			virtual BOOL IsCloseBtnClicked();
			virtual BOOL GetEscapeToCollapseDropdown() {return(mbEscapeToCollapseDropdown);};
			virtual void SetEscapeToCollapseDropdown(BOOL bValue) {/*Do nothing.*/};
			BOOL IsValid();
			RESOURCE_ID GetID();
			void SetHelpMsg(const char *szMsg);
			void SetBkgColor(COLORREF Color);
			virtual BOOL SelectText(BOOL bSelectAll);
			virtual BOOL SelectText(int iFirstChar, int iLastChar);
			void SetChildFocusWndHandle(HWND hChildFocusWnd);
			BOOL SetFocus(BOOL bSelectAllText = FALSE);
			virtual BOOL Validate(BOOL bDspLibValidFailMsg = FALSE);
			virtual BOOL GetHelpMsg(CGStr *pHelpMsg = NULL) const;
			virtual BOOL SetEnable(BOOL bNewEnabledState);	// returns prev. enable state.
			BOOL GetEnable();
			void GetResizeBehavior(GUI_RESIZE_BEHAVIOR *peResizeBehavior)
				{*peResizeBehavior = meResizeBehavior;};
			void SetResizeBehavior(GUI_RESIZE_BEHAVIOR eResizeBehavior)
				{meResizeBehavior = eResizeBehavior;};
			BOOL SetText(const char *szNewText);
			BOOL SetVisible(BOOL bNewVisibleState);	// returns prev. visible state.
			BOOL GetVisible();
			RESOURCE_ID GetResourceID() {return(mID);};
			virtual BOOL SetNumFmt(const char *szFmt);
			virtual BOOL GetValue(void *pDummy, ...);
			virtual void Update() = 0; // Abstract base class.
			virtual BOOL RetrieveUserData() = 0;		// Abstract base class.
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);

			virtual BOOL SetValidateFn(BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)) {return(FALSE);};
			virtual BOOL SetValidateFn(CGUI_CallBack *pCB_Obj, GUI_CB_DlgValidateFn pValidateFn) {return(FALSE);};

			virtual BOOL SetNotifyFn(void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)) {return(FALSE);};
			virtual BOOL SetNotifyFn(CGUI_CallBack *pCB_Obj, GUI_CB_DlgNotifyFn pNotifyFn) {return(FALSE);};

			virtual BOOL SetTrackingFn(void (*pTrackingFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)) {return(FALSE);};
			virtual BOOL SetTrackingFn(CGUI_CallBack *pCB_Obj, GUI_CB_DlgNotifyFn pTrackingFn) {return(FALSE);};
			
			virtual long dispatch(zEvent *pEv);
			virtual void PresentHelpMsg();
			
			BOOL IsType(DLG_CTRL_TYPE CtrlType);
			const char *CtrlTypeName(DLG_CTRL_TYPE Type);
			DLG_CTRL_TYPE GetCtrlType();
			HWND GetWndHandle();
			zControl *GetBaseCtrl() {return(mpBaseCtrl);};
			RESOURCE_ID GetPopupMenuID() {return(mPopupMenuID);};
			void (*GetPopupMenuNotifyFn())(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID)
				{return(mpPopupMenuNotifyFn);};
			CGUI_Dlg *GetDlg() {return(mpDlg);};

		protected:
			virtual BOOL StoreCtrlData();
			virtual BOOL RetrieveCtrlData();
			virtual void RegisterBaseCtrl(zControl *pCtrl);
			// Border widths for captioned dialog box with sizeable borders.
			void GetBorderWidths(UINT *puiLeft, UINT *puiTop);
			void PresentLibValidFailMsg(const CGStr &Msg);
			BOOL CGUI_CtrlAssert(BOOL bCondition, const char *szMsg = NULL);
			BOOL VerifyCtrlType(DLG_CTRL_TYPE RequiredType, const char *szContextMsg);
			CGUI_Dlg *mpDlg;
			RESOURCE_ID mID;
			zControl *mpBaseCtrl;
			CGFmtStr *mpConversionFmt;
			BOOL mbDspValidFailMsg;
			BOOL mbMouseDown;
			BOOL mbKeyDown;
			virtual int NotifyFocusChange(zEvent *pEvent);
			BOOL mbValid;
			BOOL mbEscapeToCollapseDropdown;			
			virtual int KeyNotifyFn(zEvent *pEvent);
			// Moved the following two variables from private to protected, because
			// CCtrlPushBtn:;SetEnable and CCtrlScrollBar::SetEnable needed to know
			// what their values were
			// Roger Rubio, ErgoTech, 3/6/98
			BOOL mbEnabled;
			BOOL mbVisible;
			RESOURCE_ID mPopupMenuID;
			void (*mpPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID);

		private:                                             
			virtual int NotifyButtonUp(zEvent *pEvent);
			virtual int NotifyButtonDown(zEvent *pEvent);
			//virtual int NotifyMouseMove(zEvent *pEvent);
			virtual int ch(zKeyEvt *pEvent);
			int NextDlgCtrlHandler(zEvent *pEvent);
			DLG_CTRL_TYPE mCtrlType;
			CGStr *mpHelpMsg;
			HWND mhChildFocusWnd;
			UINT muiOrigWidth;
			UINT muiOrigHeight;
			int miOrigLeft;	// Dlg box relative, not client relative.
			int miOrigTop;	// Dlg box relative, not client relative.
			GUI_RESIZE_BEHAVIOR meResizeBehavior;
	};
	
	
	// ===============================================================
	// ======================= CCtrlGroupBox =========================
	// ===============================================================
	
	class CCtrlGroupBox : public CGUI_DlgCtrl
	{
		public:
			CCtrlGroupBox(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			~CCtrlGroupBox();
			virtual void Update() {};
			virtual BOOL RetrieveUserData() {return(TRUE);};
		private:
	};

	// ===============================================================
	// ======================= CCtrlLED ==============================
	// ===============================================================
	
	class CGUI_StatusLight;
	
	class CCtrlLED : public CGUI_DlgCtrl
	{
		public:
			CCtrlLED(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, COLORREF Color = RGB(0, 0, 0), RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			CCtrlLED(CGUI_Dlg *pDlg, RESOURCE_ID GroupID, RESOURCE_ID CtrlID, COLORREF Color = RGB(0, 0, 0), RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlLED();
			virtual void Update() {};
			virtual BOOL RetrieveUserData() {return(TRUE);};
			void GetColor(COLORREF *pColor);
			void SetColor(COLORREF Color);
			virtual long dispatch(zEvent *pEv);
			
		private:
			CGUI_StatusLight *mpLED;
			int Paint(zEvent *pEvent);
			BOOL mbUpdateInProgress;
	};
	

	// ===============================================================
	// ======================= Validated Controls ====================
	// ===============================================================
	
	// Base class identical to CGUI_DlgCtrl except a control derived
	// from CGUI_DlgCtrlValidated will have it's validation function
	// called when the control loses focus unless the focus is lost
	// because the containing dialog has been canceled or been
	// deactivated.
	
	class CGUI_DlgCtrlValidated : public CGUI_DlgCtrl
	{
		public:
			CGUI_DlgCtrlValidated(DLG_CTRL_TYPE CtrlType,
				CGUI_Dlg *pDlg, RESOURCE_ID CtrlID,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			virtual ~CGUI_DlgCtrlValidated();
			virtual void RegisterBaseCtrl(zControl *pCtrl);
			virtual BOOL Validate(BOOL bDspLibValidFailMsg);
			virtual BOOL SetValidateFn(BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID));
			virtual BOOL SetValidateFn(CGUI_CallBack *pCB_Obj, GUI_CB_DlgValidateFn pValidateFn);

		protected:
			BOOL DoUserValidation();
			virtual int NotifyFocusChange(zEvent *pEvent);
			int NotifySetFocus(zEvent *pEvent);
		
		private:
			BOOL (*mpValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);
			BOOL mbValidateFnActive;
			BOOL mbEditBaseFocusFnsSet;
			CGUI_CallBack *mpCBO;
			GUI_CB_DlgValidateFn mpCBO_ValidateFn;
			virtual int NotifyButtonUp(zEvent *pEvent);
	};
	
	
	// ===============================================================
	// ======================= Notification Controls =================
	// ===============================================================
	
	// Base class identical to CGUI_DlgCtrl except a control derived
	// from CGUI_DlgCtrlNotified will have it's notification function
	// called when the control's state changes. This could be "clicked"
	// for a check box or push button or selection change for a list box,
	// or new setting for a scroll bar.
	
	class CGUI_DlgCtrlNotified : public CGUI_DlgCtrl
	{
		public:
			CGUI_DlgCtrlNotified(DLG_CTRL_TYPE CtrlType,
				CGUI_Dlg *pDlg, RESOURCE_ID CtrlID,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			virtual ~CGUI_DlgCtrlNotified();
			// Added by Roger Rubio, ErgoTech, 2/23/98
			virtual void RegisterBaseCtrl(zControl *pCtrl);
			virtual BOOL SetNotifyFn(void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID));
			virtual BOOL SetNotifyFn(CGUI_CallBack *pCB_Obj, GUI_CB_DlgNotifyFn pNotifyFn);
			void DoUserNotification();

		private:
			void LocalDummyNotifyFn(CGUI_Dlg *mpDlg, RESOURCE_ID mID);
			BOOL mbNotifyFnActive;
			void (*mpNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);
			CGUI_CallBack *mpCBO;
			GUI_CB_DlgNotifyFn mpCBO_NotifyFn;
	};
	
	
	// ===============================================================
	// =================== CCtrlToggleSwitch =========================
	// ===============================================================
	
	class CCtrlToggleSwitch : public CGUI_DlgCtrlNotified
	{
		public:
			CCtrlToggleSwitch(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, 
				BOOL *pOn, COLORREF OnColor, COLORREF OffColor,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID));
			~CCtrlToggleSwitch();
			virtual void Update();
			virtual BOOL RetrieveUserData();
			BOOL GetValue(BOOL *pbOn);
			BOOL SetValue(BOOL bOn);
			
		private:
			virtual long dispatch(zEvent *pEv);
			int NotifyClicked(zEvent *pEvent);
			zRadioButton *mpRadioBtnCtrl;
			CGUI_StatusLight *mpLED;
			BOOL mbUpdateInProgress;
			COLORREF mOnColor;
			COLORREF mOffColor;
			BOOL *mpbUserOn;
			BOOL mbOn;
			BOOL bLocalUserOn;
	};
	
	// ===============================================================
	// ======================= Radio Group Controls ==================
	// ===============================================================
	
	class CCtrlRadioGroup : public CGUI_DlgCtrlNotified
	{
		public:
	   		CCtrlRadioGroup(CGUI_Dlg *pDlg, RESOURCE_ID GroupID,
	   			const RESOURCE_ID *naBtnID, UINT *puiSelectedBtnID,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlRadioGroup();
			BOOL SetEnable(BOOL bNewEnabledState);	// returns DUMMY prev. enable state.
			BOOL SetEnable(UINT uiRelativeIndex, BOOL bNewEnabledState);	// returns prev. enable state.
			BOOL SetText(RESOURCE_ID CtrlID, const char *szNewText);
			virtual BOOL SetValue(UINT uiNewSelectedIndex);
			virtual BOOL GetValue(UINT *pSelectedIndex);
			BOOL GetIndexFromID(RESOURCE_ID ID, UINT *puiIndex);
			BOOL GetIDFromIndex(UINT uiIndex, RESOURCE_ID *pID);
			virtual BOOL RetrieveUserData();
			virtual void Update();
			void SetFocus();

		protected:                                             
			virtual int RadioBtnKeyNotifyFn(zEvent *pEvent);
			virtual BOOL GetHelpMsg(CGStr *pHelpMsg = NULL) const;

		private:	
			virtual BOOL RetrieveCtrlData();
			zRadioButton **mapzRadioBtn;
			CGStr **mapHelpMsg;
			BOOL *mabEnabled;
			RESOURCE_ID *maBtnID;
			UINT muiNumBtns;
			int NotifyClicked(zEvent *pEvent);
			RESOURCE_ID muiSelectedIndex;
			RESOURCE_ID *mpuiUserSelectedIndex;
			RESOURCE_ID muiFocusIndex;
			zPoint mMouseDownPos;
			BOOL bDrag;
			virtual int RadioBtnNotifyButtonDown(zEvent *pEvent);
			virtual int RadioBtnNotifyButtonUp(zEvent *pEvent);
	};
	
	
	// ===============================================================
	// ======================= Pick List Controls ====================
	// ===============================================================
	
	class CCtrlPickList : public CGUI_DlgCtrlValidated
	{
		public:
	   		CCtrlPickList(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, char *naszList[],
	   			char *pcPickIndex, CGStr *pPickedItemText,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
	   		CCtrlPickList(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, char *naszList[],
	   			int *piPickIndex, CGStr *pPickedItemText,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlPickList();          
			virtual int ch(zKeyEvt *pEvent);
			int MiscMsgHandler(zEvent *pEvent);
			virtual void Initialize(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, char *naszList[]);
			BOOL Add(const char *szChoiceText);
			void CreatePathString(CGStr *pDirPath);
			BOOL AddDirList(RESOURCE_ID StaticTextID, CGStr *pDirPath);
			BOOL AddFileList(RESOURCE_ID StaticTextID, CGStr *pFilePath);
			BOOL Clear();
			BOOL AllowEdit(BOOL bAllowEdit);
			virtual BOOL ModifyText(UINT uiPickIndex, const char *pszChoiceText);
			virtual BOOL DeleteItem(UINT uiWhichItem);
			virtual BOOL SetNotifyFn(void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID));
			virtual BOOL SetNotifyFn(CGUI_CallBack *pCB_Obj, GUI_CB_DlgNotifyFn pNotifyFn);
			virtual BOOL SetValue(int iNewIndex);
			virtual BOOL GetValue(int *piNewIndex, CGStr *pStr = NULL);
			BOOL GetNumItems(UINT *puiNumItems);
			virtual BOOL RetrieveUserData();
			virtual void Update();
			virtual BOOL GetEscapeToCollapseDropdown() {return(mbEscapeToCollapseDropdown);};
			virtual void SetEscapeToCollapseDropdown(BOOL bValue) {mbEscapeToCollapseDropdown = bValue;};

		private:	
			static BOOL PickListValidateToNotifyFn(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);
			BOOL NonStaticPickListToNotifyFn();
			virtual BOOL Validate(BOOL bDspLibValidFailMsg);
			virtual BOOL RetrieveCtrlData();
			virtual int NotifyButtonUp(zEvent *pEvent);
			virtual void RegisterBaseCtrl(zControl *pCtrl);
			int NotifySetFocus(zEvent *pEvent);
			RESOURCE_ID mStaticTextID;
			int NotifySelChange(zEvent *pEvent);
			BOOL mbAllowEdit;
			BOOL mbNewTextValidateInProgress;
			int NotifyEditChange(zEvent *pEvent);
			int miPickIndex;
			int miNumItems;	// signed for convenience of comparing with miPickIndex
			CGStr *mpTextValue;
			CGStr *mpUserPickedText;
			int *mpiUserPickIndex;
			char *mpcUserPickIndex;      
			char *mszPath;
			BOOL mbNotifyFnActive;
			void (*mpNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);
			BOOL (*mpUserValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);
			CGUI_CallBack *mpCBO;
			GUI_CB_DlgNotifyFn mpCBO_NotifyFn;
			void DoUserNotification();
			int miPrevNotifyIndex;
			CGStr mPrevNotifyText;
	};
	
	
	// ===============================================================
	// ============= Multiple Select Pick List Controls ==============
	// ===============================================================
	
	class CCtrlPL_MultiSel : public CGUI_DlgCtrlNotified
	{
		public:
		   	CCtrlPL_MultiSel(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, char *naszList[],
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlPL_MultiSel();          
			BOOL Add(const char *szChoiceText);
			BOOL Clear();
			virtual BOOL GetValue(BOOL *pabSelected[], UINT MaxDimension);
			virtual BOOL GetValue(UINT uiIndex, BOOL *pSelected,
				CGStr *pItemText = NULL);
			BOOL GetNumItems(UINT *puiNumItems);
			virtual BOOL RetrieveUserData();
			virtual BOOL SetValue(UINT uiIndex, BOOL bSelected);
			virtual void Update();
			// Added by Roger Rubio, ErgoTech, 3/2/98
			virtual BOOL Validate(BOOL bDspLibValidFailMsg);

		private:	
			virtual BOOL RetrieveCtrlData();
			// Added by Roger Rubio, ErgoTech, 2/23/98
			virtual void RegisterBaseCtrl(zControl *pCtrl);
			UINT muiNumTabCols;
			UINT *mauiColTextWidth;
			void UpdateColWidths(const char *szItemText);
			BOOL SetTabStops();
			UINT muiNumItems;
			BOOL SetNumPickChoices(UINT uiNumItems);
			virtual int NotifyButtonUp(zEvent *pEvent);
			virtual void Initialize(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, char *naszList[]);
			BOOL *mabSelected;
			int NotifySelChange(zEvent *pEvent);
			void UpdateSelItems();
	};
	
		
	// ============================================================================
	// ======================= Static Text Control ================================
	// ============================================================================
	
	class CCtrlStaticText : public CGUI_DlgCtrl
	{
		public:
			CCtrlStaticText(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			~CCtrlStaticText();
			BOOL SetValue(const char *szText);
			BOOL RetrieveUserData();
			void Update();
	};
	
	
	// ===============================================================
	// ======================= String Controls =======================
	// ===============================================================
	
	class CCtrlString : public CGUI_DlgCtrlValidated
	{
		public:
			CCtrlString(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID,
				char *szString, UINT uiDimension,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			CCtrlString(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID,
				CGStr *pString, UINT uiDimension = GUI_LIB_NO_MAX_NUM_CHAR,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			virtual ~CCtrlString();
			virtual BOOL GetValue(CGStr *pStr, UINT uiDimension = GUI_LIB_NO_MAX_NUM_CHAR);
			virtual BOOL GetValue(char *szText, UINT uiDimension);
			virtual BOOL SetValue(const CGStr &Str);
			virtual BOOL SetValue(const char *szText);
			virtual BOOL RetrieveUserData();
			virtual void Update();
			virtual BOOL SelectText(BOOL bSelectAll);
			virtual BOOL SelectText(int iFirstChar, int iLastChar);

		private:	
			virtual int NotifyButtonUp(zEvent *pEvent);
			virtual void Initialize(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, UINT uiCharArrayDim);
			UINT muiUserDimension;
			CGStr *mpString;
			CGStr *mpUserString;
			char *mpszUserString;
	};
	
	
	// ===============================================================
	// ======================= Date Controls =========================
	// ===============================================================
	
	class CCtrlDate : public CGUI_DlgCtrlValidated
	{
		public:
			CCtrlDate(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID,
				struct GEN_DATE_STRUCT *pDate,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			virtual ~CCtrlDate();
			virtual BOOL GetValue(CFmtdDate *pDate);
			virtual BOOL SetValue(const CFmtdDate &Date);
			virtual BOOL RetrieveUserData();
			virtual void Update();
			virtual BOOL SetNotifyFn(void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID));
			virtual BOOL SetNotifyFn(CGUI_CallBack *pCB_Obj, GUI_CB_DlgNotifyFn pNotifyFn);
			void NotifyChange();

		protected:                                             
			virtual int KeyNotifyFn(zEvent *pEvent);

		private:	
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);
			virtual void Initialize(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			CFmtdDate *mpDate;
			struct GEN_DATE_STRUCT *mpUserDate;
			CGStr *mpString;
			void (*mpNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);
			CGUI_CallBack *mpCBO;
			GUI_CB_DlgNotifyFn mpCBO_NotifyFn;
			BOOL mbNotifyFnActive;
	};
	
	
	// ===============================================================
	// ======================= Time Controls =========================
	// ===============================================================
	
	class CCtrlTime : public CGUI_DlgCtrlValidated
	{
		public:
			CCtrlTime(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID,
				struct GEN_TIME_STRUCT *pTime, UINT uiNumVisibleFlds = 2,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			virtual ~CCtrlTime();
			virtual BOOL GetValue(CFmtdTime *pTime);
			virtual BOOL SetValue(const CFmtdTime &Time);
			virtual BOOL RetrieveUserData();
			virtual void Update();
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);

		protected:                                             
			virtual int KeyNotifyFn(zEvent *pEvent);

		private:	
			virtual void Initialize(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			CFmtdTime *mpTime;
			struct GEN_TIME_STRUCT *mpUserTime;
			CGStr *mpString;
	};

	
	// ===============================================================
	// ==================== CGUI_DlgCtrlClick Controls ===============
	// ===============================================================

	class CGUI_DlgCtrlClick : public CGUI_DlgCtrlNotified
	{
		public:
			CGUI_DlgCtrlClick(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID,
				DLG_CTRL_TYPE CtrlType,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CGUI_DlgCtrlClick();

		private:
			virtual void ProcessBtnClicked();
			virtual int NotifyButtonDown(zEvent *pEvent);
			virtual int NotifyButtonUp(zEvent *pEvent);
    };
	
	// ===============================================================
	// ========================= CheckBox Controls ===================
	// ===============================================================
	
	class CCtrlCheckBox : public CGUI_DlgCtrlClick
	{
		public:
			CCtrlCheckBox(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, UCHAR *pucSelected,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			CCtrlCheckBox(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, BOOL *pbSelected,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlCheckBox();
			virtual BOOL RetrieveUserData();
			virtual BOOL SetValue(BOOL bNewSelected);
			virtual BOOL GetValue(BOOL *pbNewSelected);
			virtual void Update();

		private:	
			virtual int NotifyFocusChange(zEvent *pEvent);
			virtual void ProcessBtnClicked();
			void Initialize(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			int NotifyClicked(zNotifyEvt *);
			BOOL mbSelected;
			BOOL *mpbUserSelected;
			UCHAR *mpucUserSelected;
	};
	
	
	// ===============================================================
	// ========================= Push Button Controls ================
	// ===============================================================
	
	class CCtrlPushBtn : public CGUI_DlgCtrlClick
	{
		public:
			CCtrlPushBtn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, GUI_ACTION DefaultAction,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlPushBtn();
			virtual BOOL RetrieveUserData();
			virtual void Update();   
			virtual BOOL IsHelpBtn();
			virtual BOOL IsCloseBtnClicked();
			int NotifyClicked(zNotifyEvt *pEvnt);	// CGUI_Dlg needs access.
			BOOL SetLbl(const char *szLblText);
			BOOL SetBitmap(RESOURCE_ID NormBitmapID, RESOURCE_ID PushedBitmapID);
			BOOL SetBrowseOutputCtrl(RESOURCE_ID BrowseOutputCtrlID);
			BOOL MakeDefault();
			// Added by Roger Rubio, ErgoTech, 3/6/98
			virtual BOOL SetEnable(BOOL bNewEnabledState);	// returns prev. enable state.
		
		private:	
			BOOL mbBtnClickInProgress;
			virtual long dispatch(zEvent *pEv);
			virtual void ProcessBtnClicked();
			GUI_ACTION mDefaultAction;
			zBitmap *mpNormBitmap;
			zBitmap *mpPushedBitmap;
			zControl *mpBrowseOutputCtrl;
	};

	
	// ===============================================================
	// ========================= Push Button Array ===================
	// ===============================================================
	
	class CGUI_Pane;
	class CCtrlPane;
	
	class CCtrlPushBtnArray : public CGUI_DlgCtrl
	{
		public:
			CCtrlPushBtnArray(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, UINT uiNumRows, UINT uiNumCols,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, UINT uiRow, UINT uiCol),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlPushBtnArray();
			BOOL DefineState(GUI_BUTTON_ARRAY_STATE State, UINT uiFrameWidth,
				int iBevelWidth, COLORREF FaceColor, COLORREF TextColor);
			BOOL GetValue(UINT uiRow, UINT uiCol, GUI_BUTTON_ARRAY_STATE *pValue);
			BOOL SetBitmap(UINT uiRowNum, UINT uiColNum, RESOURCE_ID BitmapID);
			BOOL SetEnabled(UINT uiRow, UINT uiCol, BOOL bEnabled);
			BOOL SetLbl(UINT uiRow, UINT uiCol, const char *szLblText);
			BOOL SetValue(UINT uiRow, UINT uiCol, GUI_BUTTON_ARRAY_STATE NewValue);
			virtual void Update();
			virtual BOOL RetrieveUserData();
		
		private:
			static void StaticDrawFn(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, zDisplay *pCanvas);
			BOOL CoordToIndex(UINT uiRow, UINT uiCol, UINT *puiIndex);
			BOOL IndexToCoord(UINT uiIndex, UINT *puiRow, UINT *puiCol);
			BOOL PixCoordToIndex(int iX, int iY, UINT *puiMouseDownBtnIndex);
			void DrawBtn(UINT uiBtnIndex, BOOL bForceUpdate, zDisplay *pCanvas = NULL);
			void DrawFn(CGUI_Dlg *pDlg, zDisplay *pCanvas);
			void CalcBtnGeometry();
			void (*mpNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, UINT uiRow, UINT uiCol);
			UINT muiMouseDownBtnIndex;
			UINT muiNumRows;
			UINT muiNumCols;
			UINT muiNumBtns;
			UINT muiTotalBtnPixWidth;
			UINT muiTotalBtnPixHeight;
			UINT muiStateFrameWidth[uiNUM_BUTTON_ARRAY_STATES];
			int miStateBevelWidth[uiNUM_BUTTON_ARRAY_STATES];
			COLORREF mStateFaceColor[uiNUM_BUTTON_ARRAY_STATES];
			COLORREF mStateTextColor[uiNUM_BUTTON_ARRAY_STATES];
			GUI_BUTTON_ARRAY_STATE *maBtnState;
			BOOL *mabBtnEnabled;
			zRect *maBtnRect;
			CGFmtStr *maBtnText;
			zBitmap **mapBitmap;
			BOOL mbIsOverBtnDown;
			int NotifyMouseUp(zEvent *pEvent);
			int NotifyMouseDown(zEvent *pEvent);
			int NotifyMouseMove(zEvent *pEvent);
			CGUI_Pane *mpPane;
			CCtrlPane *mpCtrlPane;
			UINT muiFrameWidth;
			int miBevelWidth;
			UINT muiBorderWidth;
	//			virtual long dispatch(zEvent *pEv);
//			virtual void ProcessBtnClicked();
		};

	
	// ===============================================================
	// ========================= Scroll Bar Controls =================
	// ===============================================================
	
	class CCtrlScrollBar : public CGUI_DlgCtrlNotified
	{
		public:
			CCtrlScrollBar(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID,
				int *piValue, int iMinValue, int iMaxValue, int iLineIncrement,
				int iPageIncrement, double dThumbSizeFrac, 
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlScrollBar();
			virtual BOOL RetrieveUserData();
			virtual void Update();
			virtual BOOL GetValue(int *piValue);
			virtual BOOL SetValue(int iValue);
			BOOL GetParameters(int *piMinValue, int *piMaxValue,
				int *piLineIncrement, int *piPageIncrement, double *pdThumbSizeFrac);
			BOOL SetParameters(int iMinValue, int iMaxValue,
				int iLineIncrement, int iPageIncrement, double dThumbSizeFrac);
			void NotifyNewSetting();
			void NotifyTracking();
			virtual BOOL SetTrackingFn(void (*pTrackingFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID));
			virtual BOOL SetTrackingFn(CGUI_CallBack *pCB_Obj, GUI_CB_DlgNotifyFn pTrackingFn);
			// Added by Roger Rubio, ErgoTech, 3/6/98
			virtual BOOL SetEnable(BOOL bNewEnabledState);	// returns prev. enable state.
		
		private:
			int *mpiUserValue;
			int miValue;
			int miOffset;	
			int miMinValue;
			int miMaxValue;
			int miLineIncrement;
			int miPageIncrement;
			double mdThumbSizeFrac;
			CGUI_CallBack *mpTrackingCBO;
			BOOL mbTrackingFnActive;
			void (*mpTrackingFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID);
			GUI_CB_DlgNotifyFn mpCBO_TrackingFn;
			void SetMinMax(int iMinValue, int iMaxValue);
	};


#endif
