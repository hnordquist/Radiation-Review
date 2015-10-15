// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/11/97

// RTPipe.H
// Class definition for the Review Tool pipe handler
       
       


#ifndef RTPIPE_H

	#define RTPIPE_H


	#include <stdlib.h>
	#include <stdarg.h>
	#include <string.h> 

	#include "Pipe.H"
	#include "RTComs.H"
	
	#include "gen.h"
	

	#define RTPIPE_VERSION		__TIMESTAMP__


	// Not meant to be instantiated
	// Must inherit from in order for RTPipe to notify of incoming message
	// Must be before any includes
	class RTPipe_Notify {
		public:
			// Used to be notified when a setup done message is received, calls appropriate operation with data
			virtual BOOL RT_Setup_Done(const char *pszPipe_Other_Name) = 0;
			// Used to be notified when a import done message is received, calls appropriate operation with data
			virtual BOOL RT_Import_Done(const char *pszPipe_Other_Name) = 0;
			// Used to be notified when a reanalyze done message is received, calls appropriate operation with data
			virtual BOOL RT_Reanalyze_Done(const char *pszPipe_Other_Name) = 0;
			// Used to be notified when a location change message is received, calls appropriate operation with data
			virtual BOOL RT_Location_Change(const char *pszPipe_Other_Name, const char *pszLocation) = 0;
			// Used to be notified when a data message is received, calls appropriate operation with data
			virtual BOOL RT_Display_Data(const char *pszPipe_Other_Name, const char *pszTimestamp, BOOL *pbLocation_Failed) = 0;
			// Used to be notified when a data area message is received, calls appropriate operation with data
			virtual BOOL RT_Display_Data_Area(const char *pszPipe_Other_Name, const char *pszStart_Timestamp, const char *pszEnd_Timestamp, BOOL *pbLocation_Failed) = 0;
			// Used to be notified when a reanalyze message is received, calls appropriate operation with data
			virtual BOOL RT_Reanalyze_All_Data(void) = 0;
			// Used to be notified when a pipe is not opened, start tool then open pipe
			virtual BOOL RT_Pipe_Not_Open(const char *pszPipe_Other_Name) = 0;
			// Used to be notified to display exit message box when a exit message is received through a pipe, calls current operation exit if yes
			virtual BOOL RT_Receive_Exit(const char *pszPipe_Other_Name) = 0;
		private:
	};



	class RTPipes : protected Pipe_CallBack {

		public:
		    
		    // Must reference a Review tool to let tool know when and what message received
			RTPipes(RTPipe_Notify& rReview_Tool, const char *pszPipe_My_Name); 
			~RTPipes(void); 
                                             
                                             
			// Pipe creation member functions
            BOOL Create(const char *pszPipe_Other_Name, UINT uiPipe_Timeout);
            
            // Pipe connection member functions
			BOOL Connect(const char *pszPipe_Other_Name);
			BOOL Connected(const char *pszPipe_Other_Name);
			BOOL Close(const char *pszPipe_Other_Name);
			
			// Pipe sending member functions
			// NOTE: Can pass any number of strings as command being sent
			//		 Send will automatically build the command to be sent from these strings
			//		 Last paramter must be and empty string!  Example: ""
            typedef enum {
            	SUCCESS,	// Send succeeded
            	FAIL,		// Send failed
            	CANCEL		// Send was canceled
            	} RTPIPE_RESULT;
			RTPIPE_RESULT Send(const char *pszPipe_Other_Name, char **pszString_Receive, const char *pszString_Send, ...);
			RTPIPE_RESULT Send(const char *pszPipe_Other_Name, BOOL bVerify_Response, const char *pszCommand_Send, ...);			
		
			// Call this member function to put incoming displaying messages on hold when you are busy 
			//    during long operations (Example - loading and saving)
			// There must be a matching pair of unhold for each hold. Example: Hold, Hold, Unhold. Unhold, NOT: Hold, Hold, Unhold
			BOOL Set_Messages_On_Hold(BOOL bStatus);

			// Possible useful member functions
			// Checks to see if connected to anyone
			BOOL Check_Connected_To_Anyone(void);


		private :

			RTPipe_Notify& rTool;
			char *pszMy_Name;

			typedef struct PIPE_INFO_TAG {
				char *pszMy_Topics;
				char *pszOther_Name;
				char *pszOther_Topic;
				BOOL bConnected;
				Pipe *pPipe;
				struct PIPE_INFO_TAG *pNext;
				} PIPE_INFO;
				
			PIPE_INFO *pPipe_Info, *pPipe_Info_End; 
			UINT uiPipes_Num;
			BOOL bHold_Status, bStarting, bMessage_Waiting;
			UINT uiMessage_Holds;
			char **ppszMessage_Queue;
			
			
			PIPE_INFO *Find_Info(const char *pszPipe_Other_Name);

			BOOL Receive_String(char *pszRequest, char **ppszReply);
			BOOL Receive_Command(char *pszCommand);
             
			BOOL Message_Queue_Add_Message(const char *pszMessage);
			// NOTE: Will alter the string
			BOOL Message_Queue_Process_Message(char *pszMessage);

        };
        
#endif

