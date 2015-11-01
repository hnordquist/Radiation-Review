
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
