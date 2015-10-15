
// GUI_MISC.h

#if !defined GUI_MISC_H
	#define GUI_MISC_H

	class CGUI_Dlg;
	class CMsgBoxDlg;
	class CBusyMsgBoxDlg;
	class CGStr;
	class CGFmtStr;

	typedef CGStr * CGStrPtr;
	declDynaArray(CGStrPtr)
	CGStrPtrDynaArray* BrowseDirectory(char *szExt, char *szRoot, BOOL bRecursive);
	void BrowseCleanup(CGStrPtrDynaArray *DirectoryInfo);
	
//	#ifdef _DEBUG
	#if TRUE
		#define GUI_LIB_DBG_MSG(a) PresentGUI_LibDbgMsg(a)
		#define GUI_LIB_DBG_MSGS_ON
	#else
		#define GUI_LIB_DBG_MSG(a)
		#undef GUI_LIB_DBG_MSGS_ON
	#endif

	void FlushEvents();
	void GUI_AboutDlgBox(const char *szHeaderText = NULL,
		const char *szTechSupportMsg = NULL);

	BOOL GUI_Exec(const char *szPath, const char *szProgName, const char *szCmdLineArgs = NULL,
		BOOL *pbAlreadyRunning = NULL, CGFmtStr *pErrMsg = NULL);

	// GUI_Lib Error message presentation.
	extern CGFmtStr GUI_LibDbgMsg;
	void PresentGUI_LibDbgMsg(const CGStr &Msg);
	
	// Beep the speaker.
	void GUI_Beep(void);
	
	// Transfer focus to another application.
	BOOL GUI_SetAppFocus(const char *szProgName);

	// Popup message windows.
	GUI_ACTION GUI_MsgBox(const CGStr *pMsgTxt, GUI_ICON Icon = GUI_ICON_INFO,
		GUI_ACTION Btn = GUI_OK);
	GUI_ACTION GUI_MsgBox(const CGStr &MsgTxt, GUI_ICON Icon = GUI_ICON_INFO,
		GUI_ACTION Btn = GUI_OK);
	GUI_ACTION GUI_MsgBox(const char *MsgText, GUI_ICON Icon = GUI_ICON_INFO,
		GUI_ACTION Btn = GUI_OK);
	unsigned int GUI_MsgBox(const char *szMsgText, GUI_ICON Icon, const char *naszBtnLabels[],
		int iDefaultBtnIndex = -1, int iCancelbtnIndex = -1);

	// Load the text from a string table.
	BOOL GUI_LoadString(RESOURCE_ID StringID, CGStr *pStringTableText);

	// Cursor shape.
	void GUI_SetCursorShape(GUI_CURSOR CursorShape);

	// Common file dialogs.
	BOOL GUI_FileOpen(CGUI_Dlg *pDlg, const char *pFileName, CGStr *pChosenFileName,
		const char *pnaTypeNames[] = NULL, const char *pDlgTitle = NULL);
	BOOL GUI_FileSaveAs(CGUI_Dlg *pDlg, const char *pFileName, CGStr *pChosenFileName = NULL,
		const char *pTypeNames[] = NULL, const char *pDlgTitle = NULL,
		BOOL bAllowAppend = FALSE, BOOL *pAppendChosen = NULL);

	// Get text size.
	unsigned int GUI_GetTextExtent(const char *FldStr, HDC hDC = NULL);

	class CStatusMsgCtrl;
	class CFuelGaugeCtrl;

	// Busy message box.
	class CGUI_BusyMsgBox : public zEvH {
		public:
			CGUI_BusyMsgBox(const char *szTitle = NULL, const char *szMsgText = NULL,
				GUI_ICON Icon = GUI_ICON_NONE, const char *naszBtnLabels[] = NULL,
				int iDefaultBtnIndex = -1, int iCancelBtnIndex = -1);
			~CGUI_BusyMsgBox();
			BOOL GetBtnHit(BOOL bAllowCPU_Loss = FALSE, UINT *puiHitBtnIndex = NULL);
			void SetAlwaysOnTop(BOOL bOnTop);
			void ShowElapsedTime(BOOL bShow);
			void UpdateStatusMsg(const char *szMsgText);
			void UpdateFraction(double dFractionComplete);
			// void SetBtnEnable(UINT uiBtnIndex, BOOL bEnable);
#ifdef GUI_LIB_PRIVATE		
		private:
			int NotifyFocusChange(zEvent *pEvent);
			int focus(zFocusEvt *pEvent);
			int miBtnHitIndex;
			CBusyMsgBoxDlg *mpDlg;
			CStatusMsgCtrl *mpStatusMsgCtrl;
			CFuelGaugeCtrl *mpFuelGaugeCtrl;
#endif	// GUI_LIB_PRIVATE
	};

	// Status "Message" Box
	class CGUI_StatusBox {
		public:
			CGUI_StatusBox(const char *szMsgText, GUI_ACTION Btn = GUI_NONE);
			~CGUI_StatusBox();
			UpdateMsg(const char *szMsgText);
			UpdatePercent(double dPercent);
		private:
	};
	
	
	// Formatted date.
	class CFmtdDate
	{
		public: 
			CFmtdDate(const struct GEN_DATE_STRUCT &Date);
			CFmtdDate();
			~CFmtdDate();
			void Get(struct GEN_DATE_STRUCT *pDate);
			void Get(CGStr *pFmtdDateStr);
			BOOL Set(const char *szFmtdDateStr, CGStr *pFailMsg = NULL);
			BOOL Set(const CFmtdDate &Date, CGStr *pFailMsg = NULL);
			BOOL Set(const struct GEN_DATE_STRUCT &Date, CGStr *pFailMsg = NULL);
			BOOL Set(unsigned int uiYear, unsigned int uiMonth,
				unsigned int uiDay, CGStr *pFailMsg = NULL);
			static void GetTemplateString(CGStr *pDateTemplate);
			static void UpdateFormat();
			static void GetPictureString(CGStr *p_zAppPictStr);
			static char GetSeparator();
//			CFmtdDate& operator =(const struct GEN_DATE_STRUCT &Date);

#ifdef GUI_LIB_PRIVATE		
		private:  
			unsigned int muiMonth;
			unsigned int muiDay;
			unsigned int muiYear;
			static void GeneratePictureString();
			static void DecodeWinTemplate();
			static unsigned int muiMonthPos;
			static unsigned int muiDayPos;
			static unsigned int muiYearPos;
			static BOOL mbCentury;
			static char mcSeparator;
			static CGStr mFormatTemplate;
			static CGStr mPictureString;
			static unsigned int muiCenturyOffset;
			static unsigned int muiMinYear;
			static unsigned int muiMaxYear;
#endif	// GUI_LIB_PRIVATE
	};
	
	// Formatted time.
	class CFmtdTime
	{
		public: 
			CFmtdTime(const struct GEN_TIME_STRUCT &Time,
				unsigned int uiNumVisibleFlds = CFmtdTime::muiDefaultNumVisibleFlds);
			CFmtdTime();
			~CFmtdTime();
			void Get(struct GEN_TIME_STRUCT *pTime);
			void Get(CGStr *pFmtdTimeStr);
			BOOL SetPrecision(unsigned int uiNumTimeFlds);
			BOOL Set24Hour(BOOL b24Hour);
			BOOL Set(const char *szFmtdTimeStr, CGFmtStr *pFailMsg = NULL);
			BOOL Set(const struct GEN_TIME_STRUCT &Time, CGFmtStr *pFailMsg = NULL);
			BOOL Set(const CFmtdTime &Time, CGFmtStr *pFailMsg = NULL);
			BOOL Set(unsigned int uiHour, unsigned int uiMin,
				unsigned int uiSec, unsigned int uiMilliSec, CGFmtStr *pFailMsg = NULL);
			void GetPictureString(CGStr *p_zAppPictStr);
			static void GetTemplateString(CGStr *pTimeTemplate);
			static char GetSeparator();
			BOOL IsValidHour(unsigned int uiHour, CGFmtStr *pFailMsg = NULL);
			static void UpdateFormat();
			const static unsigned int muiDefaultNumVisibleFlds;
			static BOOL Is24Hour() { return(mb24Hour); };

#ifdef GUI_LIB_PRIVATE		
		private:  
			unsigned int muiHour;
			unsigned int muiMinute;
			unsigned int muiSecond;
			unsigned int muiMilliSecond;
			unsigned int muiNumVisibleFlds;       
			BOOL AfterNoon();
			BOOL IsValidTimeElement(unsigned int uiValue, unsigned int uiMin,
				unsigned int uiMax, const char *szLabel, CGFmtStr *pFailMsg);
			static void GeneratePictureString(unsigned int uiNumVisibleFlds,
				BOOL b24Hour, CGStr *pPictureStr);
			static CGStr mAM_Str;
			static CGStr mPM_Str;
			static BOOL mb24Hour;		//vs . 12 hour.
			static char mcSeparator;
			static CGStr mFormatTemplate;
			CGStr *mpCustomPictureString;
			static CGStr mPictureString;
#endif	// GUI_LIB_PRIVATE
	};
	

// ====================================================================
// ============================ CGUI_Dbg ===============================
// ====================================================================
                                                              
	class CGUI_TextWin;
	                                                              
	class CGUI_Dbg
	{
		public: 
			CGUI_Dbg();
			~CGUI_Dbg();
			static void Clear();
			static void Printf(const char *szFormat, ... );
			static void SaveAs(const char *szFileName);
			
#ifdef GUI_LIB_PRIVATE		
		private:
			static CGUI_TextWin *pDbgWin;
			static BOOL DbgWinClosing(CGUI_TextWin *pWin);
#endif	// GUI_LIB_PRIVATE
	};


	class CGUI_HugeMem
	{
		public:
			//static void huge *Alloc(ULONG ulDim, size_t SizeOfElem, const CGStr &Description);
			static void huge *Alloc(ULONG ulDim, size_t SizeOfElem);
			static void Free(void huge *pvHugeBuffer);
			static void CheckForOrphans();
			//static UINT MarkAllocations();
			//static void CheckForOrphans(UINT PriorAllocationMarker);
		
		private:
			static void huge *mapvHugeArrays[];
			static const UINT uiMAX_NUM_HUGE_PTRS;
	};	
#endif
