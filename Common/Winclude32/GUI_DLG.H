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

// GUI_DLG.h

#if !defined GUI_DLG_H
	#define GUI_DLG_H

	#include <limits.h>
	#include <float.h>

	#include <gui_str.h>
	#include <gui_misc.h>
	
	typedef enum
	{
		DLG_POSN_NO_SAVE,
		DLG_POSN_SAVE_ABS,
		DLG_POSN_SAVE_REL
	} DLG_POSN_SAVE;
	                     
	const COLORREF LED_RED = RGB(255, 0, 0);
	const COLORREF LED_GREEN = RGB(0, 255, 0);
	const COLORREF LED_YELLOW = RGB(255, 255, 0);
	const COLORREF LED_ON = RGB(255, 255, 255);
	const COLORREF LED_OFF = RGB(0, 0, 0);

	class CGUI_DlgCtrl;
	class CGUI_Dlg;
	class CGUI_Table;
	class CFmtdTime;
	//class CGUI_DlgCallBack;
	class CDlgBitmap;
	typedef struct TblInfoTag
	{
		RESOURCE_ID TblCtrlID;
		CGUI_Table *pTblCtrl;
		struct TblInfoTag *next;
	} TblInfo;
		
#ifdef GUI_LIB_PRIVATE		
	declDlist(CGUI_DlgCtrl)	// Declare zApp doubly linked list of CGUI_DlgCtrl's.
	
	class CDlgBitmap {
		public:
			CDlgBitmap(RESOURCE_ID CtrlID, RESOURCE_ID BitmapID, GUI_BITMAP_DSP_MODE eDspMode)
				{
					mCtrlID = CtrlID;
					mBitmapID = BitmapID;
					meDspMode = eDspMode;
				};
			~CDlgBitmap() {};
			RESOURCE_ID GetCtrlID() {return(mCtrlID);};
			RESOURCE_ID GetBitmapID() {return(mBitmapID);};
			void SetBitmapID(RESOURCE_ID BitmapID) {mBitmapID = BitmapID;};
			GUI_BITMAP_DSP_MODE GetDspMode() {return(meDspMode);};
			void SetDspMode(GUI_BITMAP_DSP_MODE eDspMode) {meDspMode = eDspMode;};
			
		private:
			RESOURCE_ID mCtrlID;
			RESOURCE_ID mBitmapID;
			GUI_BITMAP_DSP_MODE meDspMode;
	};
	
	declDlist(CDlgBitmap)
#endif	// GUI_LIB_PRIVATE
	
	// MSVC v 8.00c chokes on the double field setup function unless
	// I use a const double replacement for their float.h defined DBL_MAX.
	//#define DBL_MAX     1.7976931348623158e+308 /* max value */
	const double MY_DBL_MAX = DBL_MAX;
	                        
	// Added by STEVE - 2/7/97   Note: Uses Windows SDK
   	#define DIALOG_MINIMIZE(WINDOW)							\
   			ShowWindow(WINDOW->GetWndHandle(), SW_SHOWMINIMIZED);
   	#define DIALOG_RESTORE(WINDOW)							\
   			ShowWindow(WINDOW->GetWndHandle(), SW_SHOWNORMAL);

	class CGUI_Dlg : public zFormDialog
	{
		public:
			// ================================================================
			// ============= Public Fns intended for API Consumer =============
			// ================================================================
			CGUI_Dlg(RESOURCE_ID DlgID, void *pUserData = NULL,
				DLG_POSN_SAVE eSaveType = DLG_POSN_NO_SAVE, BOOL bTopLevel = FALSE);
			CGUI_Dlg(const char *szDlgName, void *pUserData = NULL,
				DLG_POSN_SAVE eSaveType = DLG_POSN_NO_SAVE, BOOL bTopLevel = FALSE);
			~CGUI_Dlg();                          

			BOOL Close(GUI_ACTION ClosingAction = GUI_CANCEL);

			BOOL DlgHasStarted() {return(mbDlgStarted);};

			// UINT version obsolete -- prefer int version.
			BOOL GetCtrlPosition(RESOURCE_ID CtrlID, UINT *puiLeft, UINT *puiTop);
			BOOL GetCtrlPosition(RESOURCE_ID CtrlID, int *piLeft, int *piTop);
			BOOL GetCtrlSize(RESOURCE_ID CtrlID, UINT *puiWidth,
				UINT *puiHeight);
			void GetMenu(CGUI_Menu **ppDlgMenu);
			void GetPositionAbs(int *piLeft, int *piTop);
			void GetPositionRel(int *piLeft, int *piTop);
			void ScreenLocToDlgClientLoc(int *piLeft, int *piTop);
			BOOL GetRect(RESOURCE_ID CtrlID, zRect *pDspRect);
			void GetFrameWidths(UINT *puiLeftOffset, UINT *puiTopOffset);
			const char *GetTitle();
			const char *GetText() {return(GetTitle());};
			void *GetUserData();
			HWND GetWndHandle();
			HWND GetWndHandle(RESOURCE_ID CtrlID);
			void Repaint();
			void SetBkgColor(COLORREF Color);
                                                                                
			GUI_ACTION Go();                           
			void GoModeless(BOOL bAutoDeleteOnClose);

			BOOL IsExiting(GUI_ACTION *pClosingAction = NULL);
			BOOL IsValid();         
			// BOOL IsValidDlg(CGUI_Dlg *pDlg);
			BOOL IsValidBitmapID(RESOURCE_ID BitmapID, const char *szExtraInfo = NULL);
			BOOL IsValidControlID(RESOURCE_ID CtrlID, const char *szExtraInfo = NULL);
			
			BOOL RetrieveAllControlValues();

			BOOL SetCtrlHelpMsg(RESOURCE_ID CtrlID, const char *szMsg);
			BOOL SetCtrlNotifyFn(RESOURCE_ID CtrlID,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID));
			BOOL SetCtrlNotifyFn(RESOURCE_ID CtrlID, CGUI_CallBack *pCB_Obj,
				GUI_CB_DlgNotifyFn pNotifyFn);
			BOOL SetCtrlPosition(RESOURCE_ID CtrlID, int iLeft, int iTop);
			BOOL SetCtrlSize(RESOURCE_ID CtrlID, UINT uiWidth, UINT uiHeight);
			BOOL SetCtrlValidateFn(RESOURCE_ID CtrlID,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID));
			BOOL SetCtrlValidateFn(RESOURCE_ID CtrlID, CGUI_CallBack *pCB_Obj,
				GUI_CB_DlgValidateFn pValidateFn);
			BOOL SetDlgExitValidateFn(BOOL (*pExitValidateFn)(CGUI_Dlg *pDlg,
				GUI_ACTION ClosingAction));
			BOOL SetDlgExitValidateFn(CGUI_CallBack *pCB_Obj,
				GUI_CB_DlgExitFn pExitValidateFn);
			BOOL SetEnable(BOOL bNewEnableState);
			BOOL SetEnable(RESOURCE_ID CtrlID, BOOL bEnableState);
			BOOL SetFldNumFmt(RESOURCE_ID CtrlID, const char *szFormat);
			BOOL SetFocus();
			BOOL SetFocus(RESOURCE_ID CtrlID, BOOL bSelectAllText = TRUE);
			BOOL SetFocus(CGUI_DlgCtrl *pCtrl, BOOL bSelectAllText = TRUE);
			BOOL SetIcon(RESOURCE_ID IconID);
			void SetMenu(CGUI_Menu *pDlgMenu);
			void SetPositionAbs(int iLeft, int iTop);
			void SetPositionRel(int iLeft, int iTop);
			BOOL SetResizeNotifyFn(void (*pResizeNotifyFn)(CGUI_Dlg *pDlg,
				int iDeltaWidth, int iDeltaHeight));
			BOOL SetResizeNotifyFn(CGUI_CallBack *pCB_Obj,
				GUI_CB_DlgResizeFn pResizeNotifyFn);
			BOOL SetCtrlResizeBehavior(RESOURCE_ID CtrlID,
				GUI_RESIZE_BEHAVIOR ResizeBehavior);
			BOOL SetCtrlResizeBehavior(RESOURCE_ID *naCtrlID,
				GUI_RESIZE_BEHAVIOR ResizeBehavior);

			void SetDlgInitNotifyFn(void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID DlgID));
			void SetDlgInitNotifyFn(CGUI_CallBack *pCB_Obj, GUI_CB_DlgNotifyFn pNotifyFn);

			BOOL SetText(RESOURCE_ID CtrlID, const char *szNewText);
			BOOL SetText(RESOURCE_ID CtrlID, RESOURCE_ID StringTableStringID);
			BOOL SetTimeOut(double dSeconds);
			void SetTitle(const char *szNewDlgTitle);
			void SetTitle(RESOURCE_ID StringTableStringID);
			// Next two fns, SetText are obsolete; have been replaced by SetTitle. 26-Sep-96
			void SetText(const char *szNewDlgTitle) {SetTitle(szNewDlgTitle);};
			void SetText(RESOURCE_ID StringTableStringID) {SetTitle(StringTableStringID);};
			void SetUserData(void *pUserData);
			BOOL SetVisible(BOOL bNewVisibleState);
			BOOL SetVisible(RESOURCE_ID CtrlID, BOOL bVisibleState);
	
			// Redisplay field to reflect current value of variable user
			// associated with the control in the Define... function.
			BOOL Update(RESOURCE_ID CtrlID);
			void UpdateAllControlValues();
			
			// =============================================================
			// ============= DEFINE VARIOUS CONTROL TYPES ==================
			// =============================================================

			// Drawing pane for rendering bitmap.
			BOOL DefineBitmapPane(RESOURCE_ID CtrlID, RESOURCE_ID BitmapID,
				UINT uiFrameWidth = 0, int iBevelWidth = 0,
				GUI_BITMAP_DSP_MODE eMode = GUI_BITMAP_FILL,
				COLORREF BkgColor = GUI_BLACK, BOOL bEraseBkg = TRUE,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL BitmapPaneSet(RESOURCE_ID CtrlID, RESOURCE_ID BitmapID,
				GUI_BITMAP_DSP_MODE eMode = GUI_BITMAP_FILL);

			// Button Array
			BOOL DefineButtonArray(RESOURCE_ID GroupBoxID, UINT uiNumRows, UINT uiNumCols,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, UINT uiRow, UINT uiCol) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL ButtonArrayDefineState(RESOURCE_ID GroupID, GUI_BUTTON_ARRAY_STATE State,
				UINT uiFrameWidth, int iBevelWidth, COLORREF FaceColor, COLORREF TextColor);
			BOOL ButtonArrayGetValue(RESOURCE_ID GroupID, UINT uiRowNum, UINT uiColNum,
				GUI_BUTTON_ARRAY_STATE *pNewState);
			BOOL ButtonArraySetBitmap(RESOURCE_ID GroupID, UINT uiRowNum, UINT uiColNum,
				RESOURCE_ID BitmapID);
			BOOL ButtonArraySetEnable(RESOURCE_ID GroupID, UINT uiRowNum, UINT uiColNum,
				BOOL bNewEnabledState);
			BOOL ButtonArraySetText(RESOURCE_ID GroupID, UINT uiRowNum, UINT uiColNum,
				const char *szNewText);
			BOOL ButtonArraySetValue(RESOURCE_ID GroupID, UINT uiRowNum, UINT uiColNum,
				GUI_BUTTON_ARRAY_STATE NewState);

			// Check Box
			BOOL DefineCheckBox(RESOURCE_ID CtrlID, UCHAR *pucSelected,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL DefineCheckBox(RESOURCE_ID CtrlID, BOOL *pbSelected,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL CheckBoxGetValue(RESOURCE_ID CtrlID, BOOL *pbChecked);
			BOOL CheckBoxSetValue(RESOURCE_ID CtrlID, BOOL bChecked);
			
			// Combo List for arrays of null-terminated strings and user input.
			BOOL DefineComboList(RESOURCE_ID CtrlID, char *naszList[],
				int *piPickIndex, CGStr *pPickedItemText = NULL,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				UINT uiCharArrayDim = GUI_LIB_NO_MAX_NUM_CHAR,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL ComboListAdd(RESOURCE_ID CtrlID, const char *szChoiceText);
			BOOL ComboListClear(RESOURCE_ID CtrlID);
			BOOL ComboListGetValue(RESOURCE_ID CtrlID, int *piPickIndex,
				CGStr *pPickedItemText = NULL);
			BOOL ComboListSetValue(RESOURCE_ID CtrlID, int iPickIndex);
	
			// Formatted date.
			BOOL DefineFldDate(RESOURCE_ID CtrlID, struct GEN_DATE_STRUCT *pDate,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL FldDateGetValue(RESOURCE_ID CtrlID,
				struct GEN_DATE_STRUCT *pDate);
			BOOL FldDateSetValue(RESOURCE_ID CtrlID,
				const struct GEN_DATE_STRUCT &Date);

			// Numeric fields.
			// Character and UCHAR.
			BOOL DefineFldNum(RESOURCE_ID CtrlID, char *pcValue,
				char cMinValue = CHAR_MIN, char cMaxValue = CHAR_MAX,
				const char *szFormat = "%c",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL DefineFldNum(RESOURCE_ID CtrlID, UCHAR *pucValue,
				UCHAR ucMinValue = (UINT) 0,
				UCHAR ucMaxValue = UCHAR_MAX, const char *szFormat = "%uc",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			// Short and USHORT.
			BOOL DefineFldNum(RESOURCE_ID CtrlID, short *psValue,
				short sMinValue = SHRT_MIN, short sMaxValue = SHRT_MAX,
				const char *szFormat = "%d",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL DefineFldNum(RESOURCE_ID CtrlID, USHORT *pusValue,
				USHORT usMinValue = (short)0,
				USHORT usMaxValue = USHRT_MAX, const char *szFormat = "%u",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			// Integer and UINT.
			BOOL DefineFldNum(RESOURCE_ID CtrlID, int *piValue,
				int iMinValue = INT_MIN, int iMaxValue = INT_MAX,
				const char *szFormat = "%d",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL DefineFldNum(RESOURCE_ID CtrlID, UINT *puiValue,
				UINT uiMinValue = (int)0,
				UINT uiMaxValue = UINT_MAX, const char *szFormat = "%u",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			// Long and ULONG;
			BOOL DefineFldNum(RESOURCE_ID CtrlID, long *plValue,
				long lMinValue = LONG_MIN, long lMaxValue = LONG_MAX,
				const char *szFormat = "%ld",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL DefineFldNum(RESOURCE_ID CtrlID, ULONG *pulValue,
				ULONG ulMinValue = (ULONG) 0L,
				ULONG ulMaxValue = LONG_MAX, const char *szFormat = "%lu",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			// Float.	
			BOOL DefineFldNum(RESOURCE_ID CtrlID, float *pfValue,
				float fMinValue = -FLT_MAX, float fMaxValue = FLT_MAX,
				const char *szFormat = "%f",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			// Double.
			BOOL DefineFldNum(RESOURCE_ID CtrlID, double *pdValue,
				double dMinValue = -MY_DBL_MAX, double dMaxValue = MY_DBL_MAX,
				const char *szFormat = "%f",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);

			// Retrieve numeric field values.
			BOOL FldNumGetValue(RESOURCE_ID CtrlID, char *pcValue);
			BOOL FldNumGetValue(RESOURCE_ID CtrlID, UCHAR *pucValue);
			BOOL FldNumGetValue(RESOURCE_ID CtrlID, short *psValue);
			BOOL FldNumGetValue(RESOURCE_ID CtrlID, USHORT *pusValue);
			BOOL FldNumGetValue(RESOURCE_ID CtrlID, int *piValue);
			BOOL FldNumGetValue(RESOURCE_ID CtrlID, UINT *puiValue);
			BOOL FldNumGetValue(RESOURCE_ID CtrlID, long *plValue);
			BOOL FldNumGetValue(RESOURCE_ID CtrlID, ULONG *pulValue);
			BOOL FldNumGetValue(RESOURCE_ID CtrlID, float *pfValue);
			BOOL FldNumGetValue(RESOURCE_ID CtrlID, double *pdValue);

			BOOL FldNumSetValue(RESOURCE_ID CtrlID, char cValue);
			BOOL FldNumSetLimits(RESOURCE_ID CtrlID,
				char cMinValue = CHAR_MIN, char cMaxValue = CHAR_MAX);
			BOOL FldNumSetValue(RESOURCE_ID CtrlID, UCHAR ucValue);
			BOOL FldNumSetLimits(RESOURCE_ID CtrlID,
				UCHAR ucMinValue = (UINT) 0, UCHAR ucMaxValue = UCHAR_MAX);
			BOOL FldNumSetValue(RESOURCE_ID CtrlID, short sValue);
			BOOL FldNumSetLimits(RESOURCE_ID CtrlID,
				short sMinValue = SHRT_MIN, short sMaxValue = SHRT_MAX);
			BOOL FldNumSetValue(RESOURCE_ID CtrlID, USHORT usValue);
			BOOL FldNumSetLimits(RESOURCE_ID CtrlID,
				USHORT usMinValue = (short)0, USHORT usMaxValue = USHRT_MAX);
			BOOL FldNumSetValue(RESOURCE_ID CtrlID, int iValue);
			BOOL FldNumSetLimits(RESOURCE_ID CtrlID,
				int iMinValue = INT_MIN, int iMaxValue = INT_MAX);
			BOOL FldNumSetValue(RESOURCE_ID CtrlID, UINT uiValue);
			BOOL FldNumSetLimits(RESOURCE_ID CtrlID,
				UINT uiMinValue = (int)0, UINT uiMaxValue = UINT_MAX);
			BOOL FldNumSetValue(RESOURCE_ID CtrlID, long lValue);
			BOOL FldNumSetLimits(RESOURCE_ID CtrlID,
				long lMinValue = LONG_MIN, long lMaxValue = LONG_MAX);
			BOOL FldNumSetValue(RESOURCE_ID CtrlID, ULONG ulValue);
			BOOL FldNumSetLimits(RESOURCE_ID CtrlID,
				ULONG ulMinValue = (ULONG) 0L, ULONG ulMaxValue = LONG_MAX);
			BOOL FldNumSetValue(RESOURCE_ID CtrlID, float fValue);
			BOOL FldNumSetLimits(RESOURCE_ID CtrlID,
				float fMinValue = -FLT_MAX, float fMaxValue = FLT_MAX);
			BOOL FldNumSetValue(RESOURCE_ID CtrlID, double dValue);
			BOOL FldNumSetLimits(RESOURCE_ID CtrlID,
				double dMinValue = -MY_DBL_MAX, double dMaxValue = MY_DBL_MAX);

			// Formatted time.
			BOOL DefineFldTime(RESOURCE_ID CtrlID, struct GEN_TIME_STRUCT *pTime,
 				UINT uiNumVisibleTimeFlds = CFmtdTime::muiDefaultNumVisibleFlds,
 				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL FldTimeGetValue(RESOURCE_ID CtrlID,
				struct GEN_TIME_STRUCT *pTime);
			BOOL FldTimeSetValue(RESOURCE_ID CtrlID,
				const struct GEN_TIME_STRUCT &Time);

			// Text types.
			// Static Text fields.
			BOOL DefineFldStaticTxt(RESOURCE_ID CtrlID,
				const char *szInitText = NULL);
			BOOL FldStaticTxtSetValue(RESOURCE_ID CtrlID, const char *szStr);

			// CGStr.
			BOOL DefineFldTxt(RESOURCE_ID CtrlID, CGStr *szString,
				UINT uiCharArrayDim = GUI_LIB_NO_MAX_NUM_CHAR,
				const char *szFormat = "%s",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			// Null-terminated "C-style" array of characters.
			BOOL DefineFldTxt(RESOURCE_ID CtrlID, char *szString,
				UINT uiCharArrayDim,
				const char *szFormat = "%s",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);    
			// Retrieve text control value.
			BOOL FldTxtGetValue(RESOURCE_ID CtrlID, CGStr *szValue,
				UINT uiDimension = GUI_LIB_NO_MAX_NUM_CHAR);
			BOOL FldTxtGetValue(RESOURCE_ID CtrlID, char *szValue,
				UINT uiDimension);
			BOOL FldTxtSetValue(RESOURCE_ID CtrlID, const CGStr &Str);
			BOOL FldTxtSetValue(RESOURCE_ID CtrlID, const char *szStr);
		
            // Group box
			BOOL DefineGroupBox(RESOURCE_ID CtrlID);

			// Drawing pane for rendering drawings.
			BOOL DefinePane(RESOURCE_ID CtrlID, 
				void (*DrawFn)(CGUI_Dlg *pDlg, RESOURCE_ID ID, zDisplay *pCanvas) = NULL,
				COLORREF BkgColor = RGB(255, 255, 255), UINT uiFrameWidth = 0,
				int iBevelWidth = 0, BOOL bEraseBkg = TRUE, RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL PaneGetClientRect(RESOURCE_ID CtrlID, zRect *pRect);
			BOOL PanePrint(RESOURCE_ID CtrlID);
			
			// Pick List for arrays of null-terminated character arrays.
			BOOL DefinePickList(RESOURCE_ID CtrlID, char *naszList[],
				UCHAR *pucPickIndex, CGStr *pPickedItemText = NULL,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL DefinePickList(RESOURCE_ID CtrlID, char *naszList[],
				UINT *puiPickIndex, CGStr *pPickedItemText = NULL,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL DefinePickList(RESOURCE_ID ControlID, CGStr *napItems[],
				UINT *puiPickIndex, CGStr *pPickedItemText,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL DefinePickList(RESOURCE_ID ControlID, CGStr aItems[],
				UINT uiNumItems, UINT *puiPickIndex, CGStr *pPickedItemText,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL PickListAdd(RESOURCE_ID CtrlID, const char *szChoiceText);
			BOOL PickListClear(RESOURCE_ID CtrlID);
			BOOL PickListDeleteItem(RESOURCE_ID CtrlID, UINT uiWhichItem);
			BOOL PickListDirectories(RESOURCE_ID CtrlID, RESOURCE_ID StaticTextID,
		      	CGStr *pDirPath = NULL);
	      	BOOL PickListFiles(RESOURCE_ID CtrlID, RESOURCE_ID StaticTextID,
		      	CGStr *pFilePath = NULL);
			BOOL PickListGetItemText(RESOURCE_ID CtrlID, UINT uiWhichItem,
				CGStr *pItemText);
			BOOL PickListTextToIndex(RESOURCE_ID CtrlID, const char *szItemText,
				UINT *puiWhichItem);
			BOOL PickListGetNumItems(RESOURCE_ID CtrlID, UINT *puiNumItems);
			BOOL PickListGetValue(RESOURCE_ID CtrlID, UINT *puiPickIndex,
				CGStr *pPickedItemText = NULL);
			BOOL PickListGetValue(RESOURCE_ID CtrlID, UCHAR *pucPickIndex,
				CGStr *pPickedItemText = NULL);
			BOOL PickListModifyItemText(RESOURCE_ID CtrlID, UINT uiPickIndex,
				const char *szChoiceText);
			BOOL PickListSetValue(RESOURCE_ID CtrlID, UINT uiPickIndex);
	
			// Multiple selection list for arrays of null-terminated strings
			BOOL DefinePL_MultiSel(RESOURCE_ID CtrlID, char *naszList[] = NULL,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL DefinePL_MultiSel(RESOURCE_ID ControlID, CGStr *napItems[],
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL DefinePL_MultiSel(RESOURCE_ID ControlID, CGStr aItems[], UINT uiNumItems,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL PL_MultiSelAdd(RESOURCE_ID CtrlID, const char *szChoiceText);
			BOOL PL_MultiSelClear(RESOURCE_ID CtrlID);
			BOOL PL_MultiSelGetNumItems(RESOURCE_ID CtrlID, UINT *puiNumItems);
			BOOL PL_MultiSelGetValue(RESOURCE_ID CtrlID, UINT uiIndex,
				BOOL *pbSelected, CGStr *pItemText = NULL);
			BOOL PL_MultiSelSetValue(RESOURCE_ID CtrlID, UINT uiIndex,
				BOOL bSelected);
			// Added by STEVE BUCK - 2/7/97 - Note: Call this when you define a multiselect with single selection
			#define PL_MULTISELSETVALUE(WINDOW, CONTROL_ID, uiINDEX)				\
					CGUI_DlgCtrl *pDlgCtrl;											\
					WINDOW->GetDlgCtrl(CONTROL_ID, &pDlgCtrl, DCT_PLMULTISEL);		\
					HWND hWnd = pDlgCtrl->GetWndHandle();							\
					SendMessage(hWnd, LB_SETCURSEL, ((WPARAM) uiINDEX), 0L);
			// Added by STEVE BUCK - 3/21/97 - Note: Deletes all of the selected items
			#define PL_MULTISELDELSELECTED(DLG_BOX, CONTROL_ID)													\
					UINT ctSEB_Index, uiSEB_Num_Items;															\
					CGStr *pSEB_Del_Strings, SEB_String;														\
					BOOL bSEB_Selected;																			\
					DLG_BOX->PL_MultiSelGetNumItems(CONTROL_ID, &uiSEB_Num_Items);								\
					if ((pSEB_Del_Strings = new CGStr [uiSEB_Num_Items]) != NULL) {								\
					    for (ctSEB_Index = 0; ctSEB_Index < uiSEB_Num_Items; ctSEB_Index++) {					\
							DLG_BOX->PL_MultiSelGetValue(CONTROL_ID, ctSEB_Index, &bSEB_Selected, &SEB_String);	\
							if (!bSEB_Selected)	pSEB_Del_Strings[ctSEB_Index] = SEB_String;						\
							}																					\
						DLG_BOX->PL_MultiSelClear(CONTROL_ID);													\
					    for (ctSEB_Index = 0; ctSEB_Index < uiSEB_Num_Items; ctSEB_Index++) {					\
							if (strcmp(pSEB_Del_Strings[ctSEB_Index].Get_sz(), "") != 0)						\
								DLG_BOX->PL_MultiSelAdd(CONTROL_ID, pSEB_Del_Strings[ctSEB_Index].Get_sz());	\
							delete pSEB_Del_Strings[ctSEB_Index];												\
							}																					\
						}
			// Added by STEVE BUCK - 3/21/97 - Note: Deletes all of the selected items
			#define PL_MULTISELGETNUMSEL(NUM_SELECTED, pDLG_BOX, CONTROL_ID, NUM_ITEMS)							\
					UINT ctSEB_PLMSGNS_Index;																	\
					BOOL bSEB_PLMSGNS_Selected;																	\
					NUM_SELECTED = 0;																			\
					for (ctSEB_PLMSGNS_Index = 0; ctSEB_PLMSGNS_Index < NUM_ITEMS; ctSEB_PLMSGNS_Index++) {		\
						pDLG_BOX->PL_MultiSelGetValue(CONTROL_ID, ctSEB_PLMSGNS_Index, &bSEB_PLMSGNS_Selected);						\
						if (bSEB_PLMSGNS_Selected) NUM_SELECTED++;												\
						}

				
			// Progress bar to show percent completed.
			BOOL DefineProgressBar(RESOURCE_ID CtrlID, BOOL bEchoPercent = FALSE,
				COLORREF FG_Color = RGB(192, 192, 192),
				COLORREF BG_Color = RGB(128, 128, 128),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL ProgressBarSetColors(RESOURCE_ID CtrlID, COLORREF FG_Color,
				COLORREF BG_Color);
			BOOL ProgressBarSetValue(RESOURCE_ID CtrlID, double dFraction);

			// Push Button
			BOOL DefinePushBtn(RESOURCE_ID CtrlID, GUI_ACTION DefaultAction,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL PushBtnMakeDefault(RESOURCE_ID CtrlID);
			BOOL PushBtnSetBitmap(RESOURCE_ID CtrlID, RESOURCE_ID NormBitmapID,
				RESOURCE_ID PushedBitmapID = 0);
			BOOL SetBrowseOutputCtrl(RESOURCE_ID CtrlID, RESOURCE_ID BrowseResultCtrlID);
	
			// Radio Button Group.
			BOOL DefineRadioGroup(RESOURCE_ID GroupID, const RESOURCE_ID *naBtnID,
				UINT *puiSelectedIndex, 
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL RadioGroupGetValue(RESOURCE_ID GroupID, UINT *puiSelectedIndex);
			BOOL RadioGroupGetValueAbs(RESOURCE_ID GroupID, RESOURCE_ID *pBtnID);
			BOOL RadioGroupSetEnable(RESOURCE_ID GroupID, UINT uiRelativeIndex,
				BOOL bNewEnabledState);
			BOOL RadioGroupSetEnableAbs(RESOURCE_ID GroupID, RESOURCE_ID BtnID,
				BOOL bNewEnabledState);
			// BOOL RadioGroupSetEnable(RESOURCE_ID GroupID, BOOL bNewEnabledState);
			BOOL RadioGroupSetValue(RESOURCE_ID GroupID, UINT uiSelectedIndex);
			BOOL RadioGroupSetValueAbs(RESOURCE_ID GroupID, RESOURCE_ID BtnID);

			// Scroll bar.
			BOOL DefineScrollBar(RESOURCE_ID CtrlID, int *piValue,
				int iMinValue, int iMaxValue, int iLineIncrement = 1,
				int iPageIncrement = 10, double dThumbSizeFrac = 0.0,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL ScrollBarGetValue(RESOURCE_ID CtrlID, int *piValue);
			BOOL ScrollBarGetParameters(RESOURCE_ID CtrlID, int *piMinValue,
				int *piMaxValue, int *piLineIncrement = NULL,
				int *piPageIncrement = NULL, double *pdThumbSizeFrac = NULL);
			BOOL ScrollBarSetParameters(RESOURCE_ID CtrlID, int iMinValue,
				int iMaxValue, int iLineIncrement = 1, int iPageIncrement = 10,
				double dThumbSizeFrac = 0.0);
			BOOL ScrollBarSetTrackingFn(RESOURCE_ID CtrlID,
				void (*pTrackingFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID));
			// For object-based non-static callback tracking (notify) function.
			BOOL ScrollBarSetTrackingFn(RESOURCE_ID CtrlID,
				CGUI_CallBack *pCB_Obj,	GUI_CB_DlgNotifyFn pTrackingFn);
			BOOL ScrollBarSetValue(RESOURCE_ID CtrlID, int iValue);

			// Status Light
			BOOL DefineStatusLight(RESOURCE_ID CtrlID, COLORREF Color,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL DefineStatusLight(RESOURCE_ID GroupID, RESOURCE_ID CtrlID,
				COLORREF Color, RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL StatusLightGetValue(RESOURCE_ID ID, COLORREF *pColor);
			BOOL StatusLightSetValue(RESOURCE_ID ID, COLORREF Color);
                         
			// Tables (scroll forms).
			BOOL DefineTable(RESOURCE_ID CtrlID, CGUI_Table **ppTable,
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			BOOL GetTable(RESOURCE_ID CtrlID, CGUI_Table **ppTable);
            RESOURCE_ID GetResourceID();

			// ================================================================
			// =========== Public Fns NOT intended for API Consumer ===========
			// ================================================================

			// Save and retrieve previous focus field for field validation.
			BOOL GetPrevFocusCtrl(CGUI_DlgCtrl **ppCtrl);
			BOOL GetCurrentFocusCtrl(CGUI_DlgCtrl **ppCtrl);
			BOOL RegisterCurrentFocusCtrl(CGUI_DlgCtrl *pNewFocusCtrl,
				int iFirstSelChar = -1, int iLastSelChar = -1);

			// Validate entire form - field-by-field and call user-supplied
			// function for overall dlg.
			BOOL ExitNotifyFn(GUI_ACTION exitAction);
             
			// Need to make parent dlg. have focus so that the entire
			// dialog is disabled by a possible GUI_MsgBox. Otherwise
			// just the child control will be disabled and the user
			// can fiddle with the dialog box rather than being forced
			// to respond to the GUI_MsgBox.
			void FocusToParentDlg();

			void SetChildFocusWndHandle(RESOURCE_ID CtrlID, HWND hChildFocusWnd);
			void UpdateHelpBtnEnable(const CGUI_DlgCtrl *pCurrentFocusCtrl);
	
			static BOOL IsModelessDlgMsg(MSG *pMsg);

			// Given a control resource ID retrieve control pointer.			
			BOOL GetDlgCtrl(RESOURCE_ID DlgCtrlID, CGUI_DlgCtrl **theDlgCtrl,
				DLG_CTRL_TYPE DlgCtrlType = DCT_NON_SPECIFIC);
			GUI_ACTION GetClosingAction() {return (mClosingAction);};
			void SetFont(const char *szFontName, int iPointSize);
			
#ifdef GUI_LIB_PRIVATE		
			int command(zCommandEvt *pEvent);
			virtual int size(zSizeEvt *pEvent);
			// Save current position in private profile file.
			void SavePositionAbs();
			void SavePositionRel();
									
			DLG_POSN_SAVE meSavePosnType;

		private:
			virtual long dispatch(zEvent *pEv);
			BOOL mbTopLevel;
			static void DlgBitmapDrawFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, zDisplay *pCanvas);
			BOOL GetBitmapID(RESOURCE_ID PaneID, RESOURCE_ID *pBitmapID, GUI_BITMAP_DSP_MODE *peBitmapDspMode);
			BOOL SetBitmap(RESOURCE_ID CtrlID, RESOURCE_ID BitmapID, GUI_BITMAP_DSP_MODE eMode);
			static void CALLBACK TimerNotification(HWND hWnd, UINT uiMsg, UINT uiTimeID, DWORD dwTime);
			void KillTimeOutTimer();
			UINT muiTimerID;
			CGUI_Menu *mpDlgMenu;

			RESOURCE_ID mDlgID;
			CGStr mDlgName;
			
			// Dialog resizing.
			BOOL ResizeCtrls(int iDeltaWidth, int iDeltaHeight);
			void (*mpResizeNotifyFn)(CGUI_Dlg *pDlg, int iDeltaWidth, int iDeltaHeight);
			CGUI_CallBack *mpCBO_ResizeObj;
			GUI_CB_DlgResizeFn mpCBO_ResizeFn;
			UINT muiPrevHeight;
			UINT muiPrevWidth;
			void CallResizeNotifyFns(int iDeltaWidth, int iDeltaHeight, BOOL bResizeFinished);
			
			void (*mpDlgInitNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID DlgID);
			CGUI_CallBack *mpDlgInitNotifyCBO;
			GUI_CB_DlgNotifyFn mpCBO_DlgInitNotifyFn;
			
			// Dialog tables.
			BOOL AddTableCtrl(CGUI_Table *pTable, RESOURCE_ID ControlID);
			BOOL IsTableMsg(RESOURCE_ID CtrlID, CGUI_Table **ppTblCtrl);
			void DeleteTableCtrls();
			void CreateTableCtrls();
			
			int TimeOutHandler(zEvent *pEvent);
			int MinMaxInfoHandler(zEvent *pEvent);
			int NextDlgCtrlHandler(zEvent *pEvent);
			int MiscMsgHandler(zEvent *pEvent);
			virtual int ch(zKeyEvt *pEvent);
			int miOrigX;
			int miOrigY;

			void GetDlgIntSize(UINT *puiCurrentWidth, UINT *puiCurrrentHeight);
			UINT muiOrigWidth, muiOrigHeight;
			UINT muiLeftFrameWidth, muiTopFrameWidth;
			void DetermineCtrlOrigPositions();
			void DetermineDlgFrameWidths();

			int TableMsgHandler(zEvent *pEvent);
			TblInfo *mpTblCtrlHead, *mpTblCtrlCur;
			
			static HWND hModelessDlg;
			void StartUp();
			BOOL EnableSysMenuClose(BOOL bNewEnable);
			void Initialize(void *pUserData, zResId Resource, DLG_POSN_SAVE eSavePosn);
			// Move ToggleSwitch stuff back to PUBLIC when it works. ToM 1/20/95
			// Interactive (toggle-able) switch / status light
			BOOL DefineToggleSwitch(RESOURCE_ID CtrlID, BOOL *pbOn,
				COLORREF OnColor = LED_RED, COLORREF OffColor = LED_OFF,
				void (*pNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) = NULL);
			BOOL ToggleSwitchGetValue(RESOURCE_ID ID, BOOL *pbOn);
			BOOL ToggleSwitchSetValue(RESOURCE_ID ID, BOOL bOn);
			
			// Save & later use current position in private profile file.
//			void SavePositionAbs();
			BOOL RestorePositionAbs(BOOL *pbAlreadyShown);
//			void SavePositionRel();
			BOOL RestorePositionRel(BOOL *pbAlreadyShown);
			void GetSize(UINT *puiWidth, UINT *puiHeight, BOOL *pbIconized, BOOL *pbMaximized);
			void SetSize(UINT uiWidth, UINT uiHeight, BOOL bIconized, BOOL bMaximized, BOOL *pbAlreadyShown);
			void SaveSize();
			void RestoreSize(BOOL *pbAlreadyShown);
			BOOL mbFirstSizeEvent;

									
//			DLG_POSN_SAVE meSavePosnType;
            virtual int focus(zFocusEvt *pEvent);
			// Run thru list of controls calling each one's library validation.
			BOOL DoFieldByFieldValidation();
			// Given a control resource ID delete it from the dlg's list of controls.
			BOOL RemoveDlgCtrl(RESOURCE_ID DlgCtrlID);                      
			// Add a new control to the dlg's list of controls.
			BOOL AddDlgCtrl(CGUI_DlgCtrl *pCtrl);
			// Pointer to where control will store it's final value.
			void *mpUserData;
			// List of controls in this Dlg.
			CGUI_DlgCtrlDlist *mpCtrlList;      
			CDlgBitmapDlist *mpBitmapList;

			// Is this Dlg box valid.
			BOOL mbValid;                
			// Type of push button which caused dlg. to terminate.
			GUI_ACTION mClosingAction;                             

			// User supplied validation function for dlg.
			BOOL (*mpUserDlgExitValidateFn)(CGUI_Dlg *pDlg,
		   		GUI_ACTION closingAction);
			CGUI_CallBack *mpCBO_ExitObj;
			GUI_CB_DlgExitFn mpCBO_ExitFn;
			BOOL mbAutoDeleteOnClose;

			// Pointer to controls focus currently and previously.
			CGUI_DlgCtrl *mpPrevFocusCtrl;
			CGUI_DlgCtrl *mpCurrentFocusCtrl;
			CGUI_DlgCtrl *mpInitialFocusCtrl;
			// Is this dialog currently active.
			BOOL mbCurrentlyActive;
			// Set up default precision for Dlg float & double fields.
			BOOL InstallDlgFloatPrecision(int iPrecision);
			// Save pointer to "Help" pushbtn control.
			CGUI_DlgCtrl *mpHelpBtnCtrl;
			// Center dialog box in parent window for aesthetics.
			void CenterDlgBox();
			int kill(zEvent *pEvent);
			CGUI_DlgCtrl *mpCancelBtn;
			void init();
			BOOL mbDlgStarted;
			BOOL mbFocusToParentInProgress;
			BOOL mbCtrlValidateInProgress;
			CGUI_DlgCtrl *mpValidateFocusRequest;
			BOOL mbExitInProgress;
			BOOL mbCustomPositionSet;
			zIcon *mpzIcon;
			RESOURCE_ID DefaultBtnID;
			CGStr mCaption;
#endif	// GUI_LIB_PRIVATE
	};
	

#endif
