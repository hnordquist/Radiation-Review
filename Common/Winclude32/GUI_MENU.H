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

// GUI_MENU.h

#if !defined GUI_MENU_H
	#define GUI_MENU_H
	class CGUI_App;
	class CGUI_Menu;
	
	class CGUI_MenuItem
	{ 
		public:                                                         
			CGUI_MenuItem(void);
			CGUI_MenuItem(RESOURCE_ID ID,
				void (*HandlerFn)(void),
				const char *pszStatusBarMsg = NULL);
			CGUI_MenuItem(RESOURCE_ID ID,
				void (*HandlerFn)(void *pUserData),
				void *pUserData,
				const char *pszStatusBarMsg = NULL);

			CGUI_MenuItem(RESOURCE_ID ID,
				CGUI_CallBack *pCB_Obj,
				GUI_CB_Fn pHandlerFn,
				const char *StatusBarMsg = NULL);
			CGUI_MenuItem(RESOURCE_ID ID,
				CGUI_CallBack *pCB_Obj,
				GUI_CB_WithUserData_Fn pHandlerFn,
				void *pUserData,
				const char *StatusBarMsg = NULL);


			~CGUI_MenuItem();
			BOOL CallHandlerFn();
			RESOURCE_ID GetID();
			const char *GetStatusBarMsg();
			void SetStatusBarMsg(const char *szStatusBarMsg);
			BOOL SetEnabled(CGUI_Menu *pMenu, BOOL bNewEnabledState);
			BOOL GetEnabled(CGUI_Menu *pMenu);
			BOOL SetChecked(CGUI_Menu *pMenu, BOOL bNewCheckedState);
			BOOL GetChecked(CGUI_Menu *pMenu);

#ifdef GUI_LIB_PRIVATE		
		private:
/*
			void (*GetHandlerWithDataFunction())(void *pUserData);
			void (*GetHandlerFunction())(void);
			void *GetUserData();
*/
			void Initialize(RESOURCE_ID ID);
			void (*mpHandlerWithDataFn)(void *pUserData);
			void (*mpHandlerFn)(void);
			RESOURCE_ID mID;
			//const char *mpszStatusBarMsg;
			CGStr mStatusBarMsg;
			void *mpUserData;		
			BOOL mbEnabled;
			BOOL mbChecked;
			CGUI_CallBack *mpCB_Obj;
			GUI_CB_Fn mpCBO_HandlerFn;
			GUI_CB_WithUserData_Fn mpCBO_HandlerWithDataFn;
#endif	// GUI_LIB_PRIVATE
	};
	
#ifdef GUI_LIB_PRIVATE		
	declDlist(CGUI_MenuItem)	// Declare zApp doubly linked list of CGUI_MenuItems.
	declDlist(CGUI_Menu)
#endif	// GUI_LIB_PRIVATE
	
	class CGUI_Menu : public zMenu
	{
		public:
			CGUI_Menu(CGUI_App *pApplication, RESOURCE_ID MenuID);
			CGUI_Menu(RESOURCE_ID MenuID);
			CGUI_Menu();
			~CGUI_Menu();
			
			// Set up callback functions for menu hits.
			BOOL SetUpMenuItem(RESOURCE_ID MenuItemID, void (*HandlerFn)(void),
				const char *StatusBarMsg = NULL);
			BOOL SetUpMenuItem(RESOURCE_ID MenuItemID, void (*HandlerFn)(void),
				RESOURCE_ID StatusBarMsgID);

			BOOL SetUpMenuItem(RESOURCE_ID MenuItemID, void (*HandlerFn)(void *pUserData),
				void *pUserData, const char *StatusBarMsg = NULL);
			BOOL SetUpMenuItem(RESOURCE_ID MenuItemID, void (*HandlerFn)(void *pUserData),
				void *pUserData, RESOURCE_ID StatusBarMsgID);

			// Set up object-based callback functions.
			BOOL SetUpMenuItem(RESOURCE_ID MenuItemID, CGUI_CallBack *pCB_Obj,
				GUI_CB_Fn pHandlerFn, const char *StatusBarMsg = NULL);
			BOOL SetUpMenuItem(RESOURCE_ID MenuItemID, CGUI_CallBack *pCB_Obj,
				GUI_CB_Fn pHandlerFn, RESOURCE_ID StatusBarMsgID);

			BOOL SetUpMenuItem(RESOURCE_ID MenuItemID, CGUI_CallBack *pCB_Obj,
				GUI_CB_WithUserData_Fn pHandlerFn, void *pUserData,
				const char *StatusBarMsg = NULL);
			BOOL SetUpMenuItem(RESOURCE_ID MenuItemID, CGUI_CallBack *pCB_Obj,
				GUI_CB_WithUserData_Fn pHandlerFn, void *pUserData,
				RESOURCE_ID StatusBarMsgID);

			// Set up menu hit builtin GUI Lib functions.
			BOOL SetUpMenuItem(RESOURCE_ID MenuItemID, GUI_ACTION ActionType,
				const char *StatusBarMsg = NULL);
			BOOL SetUpMenuItem(RESOURCE_ID MenuItemID, GUI_ACTION ActionType,
				RESOURCE_ID StatusBarMsgID);
			
			BOOL SetStatusBarMsg(RESOURCE_ID MenuItemID, const char *szStatusBarMsg);
			BOOL SetStatusBarMsg(RESOURCE_ID MenuItemID, RESOURCE_ID StatusBarMsgID);
			
			void SetEnabledAll(BOOL bNewEnabledState);
			BOOL SetEnabled(RESOURCE_ID MenuItemID, BOOL bNewEnabledState);
			BOOL GetEnabled(RESOURCE_ID MenuItemID);
			BOOL SetChecked(RESOURCE_ID MenuItemID, BOOL bNewCheckedState);
			BOOL GetChecked(RESOURCE_ID MenuItemID);
			BOOL GetMenuItem(RESOURCE_ID EventID, CGUI_MenuItem **ppMenuItem);
			BOOL CallHandlerFn(RESOURCE_ID EventID);
			BOOL SetMenuText(UINT uID, const char *szNewText, BOOL bByPosition = TRUE);
			BOOL InsertMenuItem(UINT uPos, const char *szText, RESOURCE_ID &MenuItemID);
			BOOL InsertMenuSeparator(UINT uPos);
			BOOL InsertNewSubMenu(UINT uPos, const char *szText, CGUI_Menu **ppNewSubMenu);
			BOOL InsertMenuItemToSubMenu(UINT uSubMenuPos, UINT uNewMenuItemPos, const char *szText, RESOURCE_ID &MenuItemID);
			BOOL DeleteMenuItem(RESOURCE_ID MenuItemID);
			BOOL DeleteMenuSeparator(UINT uPos);
			BOOL DeleteSubMenu(CGUI_Menu *pMenuToDelete);
#ifdef GUI_LIB_PRIVATE		
			void SetOwner(zWindow *pWin) {hW=pWin;};
			void DestroyUserDataHelper(HMENU);
		private:
			// BOOL mbMenuBarActive;
			// int MenuItemHiliteNotify(zMenuHiliteEvt *theEvent);
			CGUI_MenuItemDlist *pHandlerList;
			CGUI_MenuDlist *mpNewMenuList;
			BOOL RemoveHandler(RESOURCE_ID MenuItemID);
			static RESOURCE_ID AvailableMenuID;
#endif	// GUI_LIB_PRIVATE
	};                    
	
#endif
