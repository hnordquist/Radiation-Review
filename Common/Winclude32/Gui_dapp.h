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

// GUI_APP.h

#if !defined GUI_DAPP_H
	#define GUI_DAPP_H
	
	#include <zapph\mdi.hpp>
	
	class CGStr;
	class CGUI_DlgApp;
	class CGUI_EventHndlr;
	class CGUI_BkgTask;
	class CGUI_TimerFn;
	
	class CGUI_DlgApp : public CGUI_Dlg
	{
		public:
			CGUI_DlgApp(const char *pszAppName, RESOURCE_ID DlgID, void *pUserData = NULL,
				DLG_POSN_SAVE eSaveType = DLG_POSN_NO_SAVE);
			CGUI_DlgApp(const char *pszAppName, const char *szDlgName, void *pUserData = NULL,
				DLG_POSN_SAVE eSaveType = DLG_POSN_NO_SAVE);
			virtual ~CGUI_DlgApp();
			virtual void Go(void (*pUserStartupFn)(void *pUserData) = NULL, void *pUserData = NULL);

			BOOL GetWinPosition(int *piLeft, int *piTop, int *piWidth = NULL, int *piHeight = NULL);
			static const CGStr *GetName();
			static const CGStr *GetTitle();
			static void SetTitle(const char *szAppTile);
			static void SetTitle(RESOURCE_ID StringTableStringID);
			static CGUI_DlgApp *GetDlgApp();
			static const char *GetCmdLineArgs();
			static void GetCmdLineArgs(UINT *puiNumArgs, char **argv[], char **envp[]);
			static HINSTANCE GetInstance();
			static HWND SetFocus(HWND hAppWnd);	// hAppWnd = NULL => this application; returns prev focus window.
			static void Terminate(void);
			static void ProcessEvents(BOOL bAllowCPU_Loss = TRUE);
			static void PrinterSetup();

			static void RegisterMayTerminateFn(BOOL (*pMayTermFn)(void));
			static BOOL SetIcon(RESOURCE_ID IconID);	
            
			static void Wait(double dSeconds);
			static double GetElapsedSeconds();
			static BOOL IsAppTerminating();
			static void CGUI_DlgApp::Show(int iShowState);
			
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
			
#ifdef GUI_LIB_PRIVATE		
		private:
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
			static void Close();
			HWND mhMsgWnd;
			virtual int command(zCommandEvt *CmdEvent);
			virtual long dispatch(zEvent *Ev);
			static void GetProfileSectionName(const char *szSectionName, CGStr *pSectionName);
			int move(zMoveEvt *pEvent);
			int poll();
			int size(zSizeEvt *pEvent);

			zRect *mpAppExtRect;
			HINSTANCE GUI_Lib_hInstance;
			BOOL mbShowIconized;
			BOOL mbShowMaximized;

			static long mlOriginalTickCount;
			static unsigned int muiNextBkgTask;
			static CGUI_BkgTaskDlist *mpBkgTaskList;
			static CGUI_TimerFnDlist *mpTimerFnList;
			static CGUI_EventHndlrDlist *mpEventHndlrList;
			static zApp *mpzApp;                          
			static zIcon *mpzIcon;
			static CGStr *mpAppName;
			static CGStr *mpAppTitle;
			static CGUI_DlgApp *mpCGUI_DlgApp;                          
			static BOOL (*mpUserMayTermFn)(void);
			static CGStr mNoName;
			static CGStr mNoTitle;
			static BOOL mbAppIsTerminating;
#endif	// GUI_LIB_PRIVATE
	};
	
#endif
