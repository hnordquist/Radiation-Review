
// GUI_DDE.h	Dynamic data exchange.

#if !defined(GUI_DDE_H)

	#define GUI_DDE_H
	
	class zConversation;
	class zDdeClient;
	class zDdeServer;
	
	class CGUI_DDE_Client
	{
		public: 
			CGUI_DDE_Client(const char *szServerName, const char *szTopicName,
				BOOL *pSuccess = NULL, CGFmtStr *pErrMsg = NULL);
			~CGUI_DDE_Client();
			BOOL Execute(const char *szKeyword, const char *szParam = NULL,
				double dTimeOutSecs = 0.0);
			BOOL Request(const char *szItemName, CGStr *pRetMsg,
				double dTimeOutSecs = 0.0);
			const char *GetServerName();
			
		private:             
			CGStr mServerName;
			zConversation *mpConversation;  
			BOOL mbValid;
			HINSTANCE mhInst;
			static UINT muiClientCount;
			static zDdeClient *mpDDE_Client;
	};


	class CGUI_TopicServer;	
	class CGUI_DDE_Server
	{
		public: 
			CGUI_DDE_Server(const char *szServerName, BOOL *pSuccess = NULL,
				CGFmtStr *pErrMsg = NULL);
			~CGUI_DDE_Server();
			BOOL CreateTopic(const char *szTopicName,
				BOOL (*pTopicServerFn)(CGStr ItemName, CGStr *pReturnData),
				BOOL (*pExecuteServerFn)(CGStr Cmd));
			BOOL CreateTopic(const char *szTopicName, CGUI_CallBack *pCB_Obj,
				GUI_CB_RequestServerFn pRequestServerFn,
				GUI_CB_ExecuteServerFn pExecuteServerFn);
			static BOOL FindCmd(CGStr FullCmd, char *naszCmds[], UINT *puiCmdIndex,
				CGStr *pCmdArgs = NULL,	BOOL bCaseSensitive = FALSE);
			
		private:             
			BOOL mbValid;
			static UINT muiServerCount;
			static zDdeServer *mpDDE_Server;
			CGUI_TopicServer *mpTopicServer;
	};


#endif


