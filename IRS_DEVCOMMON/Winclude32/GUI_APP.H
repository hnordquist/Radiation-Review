
// GUI_APP.h

#if !defined GUI_APP_H
	#define GUI_APP_H
	
	#include <zapph\mdi.hpp>
	
	class CGStr;
	class CGUI_Menu;
	class CGUI_App;
	class CGUI_EventHndlr;
	class CGUI_BkgTask;
	class CGUI_TimerFn;
	class CGUI_AppStatusBar;
	
#ifdef GUI_LIB_PRIVATE		
	declDlist(CGUI_BkgTask)	// Declare zApp doubly linked list of CGUI_BkgTask's.
	declDlist(CGUI_TimerFn)	// Declare zApp doubly linked list of CGUI_TimerFn's.
	declDlist(CGUI_EventHndlr)	// Declare zApp doubly linked list of CGUI_EventHndlr's.
#endif	// GUI_LIB_PRIVATE

	class CGUI_TimerFn
	{                  
		public:
			CGUI_TimerFn(void (*mUserCallbackFn)(void *pUserData),
				CGUI_CallBack *pCB_Obj, GUI_CB_WithUserData_Fn pTimerCallbackFn,
				double dPeriodInSecs, UINT uiNumPeriods,
				void *pUserData, BOOL bActive);
			~CGUI_TimerFn();
			BOOL Execute();
			BOOL IsSame(void (*pUserBkgFn)(void *pUserData), CGUI_CallBack *pCB_Obj,
				GUI_CB_WithUserData_Fn pTimerCallbackFn, void *pUserData);
			BOOL Activate(BOOL bActive);		// Returns previous state.
			UINT GetTimerID() {return(muiTimerID);};
			BOOL SetActive(BOOL bNewActiveState);
			BOOL IsValid() {return(mbValid);};
			
		private:
			BOOL mbValid;
			UINT muiTimerID;
			BOOL SetTimer(BOOL bActive = TRUE);
			void (*mpUserCallbackFn)(void *pUserData);
			void *mpUserData;
			double mdPeriodInSecs;
			UINT muiNumPeriods;
			BOOL mbActive;
			BOOL mbCurrentlyCalled;
			UINT muiNumCalls;
			CGUI_CallBack *mpCB_Obj;
			GUI_CB_WithUserData_Fn mpCBO_UserCallbackFn;
	};
	
	
	class CGUI_App : public zMDIAppFrame
	{
		public:
			CGUI_App(zApp *pApplication, const char *pszAppTitle, const char *pszAppName);
			virtual ~CGUI_App();
			virtual void Go(void (*pUserStartupFn)(void *pUserData) = NULL, void *pUserData = NULL);

			BOOL GetWinPosition(int *piLeft, int *piTop, int *piWidth = NULL, int *piHeight = NULL);
			static BOOL IsBoldEditText() {return(mbBoldEditText);};
			static void EnableBoldEditText(BOOL bNewEnabledState) {mbBoldEditText = bNewEnabledState;};
			static const CGStr *GetName();
			static const CGStr *GetTitle();
			static void SetTitle(const char *szAppTile);
			static void SetTitle(RESOURCE_ID StringTableStringID);
			static CGUI_App *GetApp();
			static zFrameWin *GetTopLevel();
			static const char *GetCmdLineArgs();
			static void GetCmdLineArgs(UINT *puiNumArgs, char **argv[], char **envp[]);
			static HINSTANCE GetInstance();
			static HWND SetFocus(HWND hAppWnd);	// hAppWnd = NULL => this application; returns prev focus window.
			static CGUI_Menu *GetMenu();
			static void Terminate(void);
			static void ProcessEvents(BOOL bAllowCPU_Loss = TRUE);
			static void MDI_Cascade();
			static void MDI_Tile();
			static void MDI_ArrangeIcons();
			static void PrinterSetup();

			static void RegisterMayTerminateFn(BOOL (*pMayTermFn)(void));
			static void SetMenu(CGUI_Menu *theMenu);
			static BOOL SetIcon(RESOURCE_ID IconID);	
            
            static void ShowStatusBar(BOOL bVisible);
			static const char *GetStatusBarText();
			static void SetStatusBarText(const char *szMsg = NULL);
			static void SetStatusBarFont(const char *szFontName, int iPointSize);
            
			static void Wait(double dSeconds);
			static double GetElapsedSeconds();
			static BOOL IsAppTerminating();
			static void CGUI_App::Show(int iShowState);
			
			static void RegisterBkgFn(void (*pUserBkgFn)(void *pUserData),
				void *pUserData = NULL, BOOL bActive = TRUE);
			static void RemoveBkgFn(void (*pUserBkgFn)(void *pUserData));
			static void RemoveBkgFn(void (*pUserBkgFn)(void *pUserData),
				void *pUserData);
			static void ActivateBkgFn(void (*pUserBkgFn)(void *pUserData),
				BOOL bActive = TRUE);
			
			// For non-class-based callback functions.
			static BOOL RegisterTimerFn(void (*pTimerCallbackFn)(void *pUserData),
				double dPeriodInSecs, UINT uiNumPeriods = 0,
				void *pUserData = NULL, BOOL bActive = TRUE);
			static BOOL RemoveTimerFn(void (*pTimerCallbackFn)(void *pUserData),
				void *pUserData = NULL);
			static BOOL ActivateTimerFn(void (*pTimerCallbackFn)(void *pUserData),
				void *pUserData = NULL, BOOL bActive = TRUE);
			// For class-based callback functions. Class derived from CGUI_CallBack.
			// gui_defs.h defines GUI_CB_WithUserData_Fn as: typedef void (CGUI_CallBack::*GUI_CB_WithUserData_Fn)(void *pUserData);
			static BOOL RegisterTimerFn(CGUI_CallBack *pCB_Obj,
				GUI_CB_WithUserData_Fn pTimerCallbackFn,
				double dPeriodInSecs, UINT uiNumPeriods = 0,
				void *pUserData = NULL, BOOL bActive = TRUE);
			static BOOL RemoveTimerFn(CGUI_CallBack *pCB_Obj,
				GUI_CB_WithUserData_Fn pTimerCallbackFn,
				void *pUserData = NULL);
			static BOOL ActivateTimerFn(CGUI_CallBack *pCB_Obj,
				GUI_CB_WithUserData_Fn pTimerCallbackFn,
				void *pUserData = NULL, BOOL bActive = TRUE);
			
			static BOOL WriteProfileInfo(const char *szSectionName, const char *szKeyword, const char *szText);
			static BOOL WriteProfileInfo(const char *szSectionName, const char *szKeyword, int iNumber);
			static BOOL WriteProfileInfo(const char *szSectionName, const char *szKeyword, UINT uiNumber);
			static BOOL WriteProfileInfo(const char *szSectionName, const char *szKeyword, long lNumber);
			static BOOL WriteProfileInfo(const char *szSectionName, const char *szKeyword, ULONG ulNumber);
			static BOOL WriteProfileInfo(const char *szSectionName, const char *szKeyword,
				double dNumber, const char *szFormat = "%lf");
			static BOOL ReadProfileInfo(const char *szSectionName, const char *szKeyword,
				CGStr *pText, const char *szDefaultStr);
			static BOOL ReadProfileInfo(const char *szSectionName, const char *szKeyword,
				char *szText, UINT uiTextDim, const char *szDefaultStr);
			static BOOL ReadProfileInfo(const char *szSectionName, const char *szKeyword,
				int *piValue, int iDefaultValue);
			static BOOL ReadProfileInfo(const char *szSectionName, const char *szKeyword,
				UINT *puiValue, UINT uiDefaultValue);
			static BOOL ReadProfileInfo(const char *szSectionName, const char *szKeyword,
				long *plValue, long lDefaultValue);
			static BOOL ReadProfileInfo(const char *szSectionName, const char *szKeyword,
				ULONG *pulValue, ULONG ulDefaultValue);
			static BOOL ReadProfileInfo(const char *szSectionName, const char *szKeyword,
				double *pdValue, double dDefaultValue);
			static BOOL AddEventNotifyFn(UINT uiMsg, BOOL (*UserNotifyFn)(HWND, UINT, WPARAM, LPARAM));
			static void ClearEventNotifyFns();
			static BOOL RemoveEventNotifyFn(UINT uiMsg, BOOL (*UserNotifyFn)(HWND, UINT, WPARAM, LPARAM));
			static BOOL SetEventNotifyFn(UINT uiMsg,
				BOOL (*UserNotifyFn)(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam), BOOL bOn = TRUE);
			static BOOL SetEventNotifyFn(const char *szMsgName,
				BOOL (*UserNotifyFn)(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam), BOOL bOn = TRUE);
			static LRESULT SendAppMsg(UINT uiMsg, HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL bWaitForReply);
			static LRESULT SendAppMsg(const char *szMsgName, HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL bWaitForReply);
			static const char *szCopyrightText;
			static const char *szCopyrightTextSmallScreen;
			static void SetMenuFont(const char *szFontName, int iPointSize);

			// Call SetLocale(NULL) to set back to the computer orignal setting.

			// For how to specify the the parameter of "SetLocale", please
			// refer to the online help in the Developer Studio about the second
			// argument of "setlocale".
			// In the guilib, if "szLanguage" is not "NULL", then it passes "szLanguage"
			// as the second argument to the "setlocale" function.

			static void SetLocale(const char *szLanguage) {mszLocale = szLanguage;};
			
#ifdef GUI_LIB_PRIVATE		
			BOOL IsMenuFontSet() {if (mpMenuFont) return TRUE; else return FALSE;};
			
		private:
			virtual long dispatch(zEvent*);
			friend BOOL CGUI_TimerFn::SetTimer(BOOL);
			static BOOL SetTimer(CGUI_TimerFn *pTimerFn, UINT *puiTimerID, double dTimerPeriod, BOOL bActive = TRUE);
			static BOOL FindTimer(UINT uiTimerID, CGUI_TimerFn **ppTimerFn);
			static BOOL FindTimer(void (*pUserCallbackFn)(void *pUserData),
				CGUI_CallBack *pCB_Obj, GUI_CB_WithUserData_Fn pCBO_UserCallbackFn,
				void *pUserData, CGUI_TimerFn **ppTimerFn);
			
			static BOOL mbCmdLineArgsDecoded;
			static UINT muiNumCmdLineArgs;
			static char **maszCmdLineParms;
			static void DecodeCmdLineArgs();
			static void FreeCmdLineArgs();

			static FARPROC mlpfnTimerProc;
			static void CALLBACK TimerNotification(HWND hWnd, UINT uiMsg, UINT uiTimerID, DWORD dwTime);
			static LONG FAR PASCAL _export MsgWndProc (HWND hWnd, UINT mMsg,
				WPARAM wParam, LPARAM lParam);
			void CreateMsgWindow();
			static BOOL mbTerminateFlagSet;
			static BOOL mbGoCalled;
			static void Close();
			HWND mhMsgWnd;
			virtual int command(zCommandEvt *CmdEvent);
			int EndNotifyFn(zEvent *Ev);
			static void GetProfileSectionName(const char *szSectionName, CGStr *pSectionName);
			int move(zMoveEvt *pEvent);
			int poll();
			int size(zSizeEvt *pEvent);
			int MenuItemHiliteNotify(zMenuHiliteEvt *theEvent);

			zRect *mpAppExtRect;
			HINSTANCE GUI_Lib_hInstance;
			BOOL mbShowIconized;
			BOOL mbShowMaximized;

			static long mlOriginalTickCount;
			static unsigned int muiNextBkgTask;
			static BOOL mbMenuBarActive;
			static BOOL mbHideStatusBarWhenMenuCloses;
			static CGStr mPrevStatusBarText;
			static CGUI_BkgTaskDlist *mpBkgTaskList;
			static CGUI_TimerFnDlist *mpTimerFnList;
			static CGUI_EventHndlrDlist *mpEventHndlrList;
			static zApp *mpzApp;                          
			static zIcon *mpzIcon;
			static CGStr *mpAppName;
			static CGStr *mpAppTitle;
			static CGUI_Menu *mpAppMenu;
	        static CGUI_AppStatusBar *mpAppStatusBar;
	        static zMDIMarginFrame *mpStatusBarFrame;
	        static BOOL mbStatusBarVisible;
			static CGUI_App *mpCGUI_App;                          
			static BOOL (*mpUserMayTermFn)(void);
			static CGStr mNoName;
			static CGStr mNoTitle;
			static BOOL mbAppIsTerminating;
			static BOOL mbBoldEditText;
			static void SetMenuFontHelper(HMENU);
			static zFont *mpMenuFont;
			static const char *mszLocale;
#endif	// GUI_LIB_PRIVATE
	};
	
#endif
