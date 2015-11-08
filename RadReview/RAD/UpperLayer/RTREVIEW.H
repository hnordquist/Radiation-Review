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
// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/10/97

// RTReview.H
// NOTE: This class is a pure virtual class and is never meant
// to be instantiated.  It is only meant to extract common elements
// away from your class.  Your review tool should inherit
// and overide all pure virtual classes and desired virtual classes
// in order to provide operations specific to your review tool.
       
       


#ifndef RTREVIEW_H

	#define RTREVIEW_H

	#include <gui_lib.h>
	#include <stdlib.h> 
	
	#include "RTInit.H"
	#include "RTPipe.H"
	#include "RTError.H"
	


	#define szRTREVIEW_VERSION			__TIMESTAMP__
	#define szREVIEW_TOOL_VERSION		"3.3.0.0"

	#define uiTIMESTAMP_LENGTH			22


	// The commented ones are already defined
	//#define uiRT_ERROR_NO_MEMORY						1
	//#define uiRT_ERROR_NOT_INSTALLED					2		// Requires tool name to be passed
	//#define uiRT_ERROR_INIT_LOAD						3		// Requires param errored for first and param warning for second, NULL accepted
	//#define uiRT_ERROR_CANT_START_TOOL				4		// Requires tool name to be passed
	//#define uiRT_ERROR_PIPE_OPEN_FAIL					5		// Requires tool name and your name to be passed
	//#define uiRT_ERROR_PIPE							6		// Requires tool name to be passed
	//#define uiRT_ERROR_PIPE_DEAD						15		// Requires tool name to be passed
	//#define uiRT_ERROR_OPTIONS_SAVE_FAIL				20
	//#define uiRT_ERROR_OPTIONS_RESET_FAIL				21
	//#define uiRT_ERROR_DB_CANT_SAVE					35
	//#define uiRT_ERROR_INSPEC_CHANGE					40
	//#define uiRT_ERROR_OVERWRITE_RAWDATA				50
	//#define uiRT_ERROR_COMMON_INSPEC_LOAD				60		// Requires label warning name to be passed as first parameter
	//#define uiRT_ERROR_COMMON_INSPEC_SAVE				61
	#define uiRT_REVIEW_ERROR								UINT
	#define uiRT_REVIEW_ERROR_NO_MEMORY						100
	#define uiRT_REVIEW_ERROR_PIPE							105		// Requires tool name to be passed
	#define uiRT_REVIEW_ERROR_NO_MATCHING_DATE_AND_TIME		110		
	#define uiRT_REVIEW_ERROR_NO_MATCHING_LOCATION			111		// Requires other tool name and location to be passed
	#define uiRT_REVIEW_ERROR_NO_MATCHING_DATA_TOOL_START	112	
	#define uiRT_REVIEW_ERROR_INITIATE_REANALYZE_FAILED		113
	#define uiRT_REVIEW_ERROR_DB_OPEN						115		// Requires DB name to be passed
	#define uiRT_REVIEW_ERROR_DB_CLOSED						120
	#define uiRT_REVIEW_ERROR_DB_WRITE_PROTECTED			125		// Requires DB name to be passed
	#define uiRT_REVIEW_ERROR_DB_CANT_SAVE					130		// Requires DB name to be passed
	#define uiRT_REVIEW_ERROR_BUSY							135		// Requires tool name to be passed
	#define uiRT_REVIEW_ERROR_BUSY_EXIT						140
	#define uiRT_REVIEW_ERROR_CANT_MATCH					150		// Requires tool name to be passed
		
	
	
	
	class RTReview : public CGUI_CallBack {

		public:
		
			// Operator class functions
			RTReview(void); 
			virtual ~RTReview(void); 
			

			// This member function will always be called as the first operation for this mode.
			// This allows an app to exist before the mode begins.
			// Overide it if you want to be sure that an app exists as soon as possible
			// NOTE: Only create the app (and do a gomodeless if you want), however do not do a go!!!
			// Return TRUE if app successfully created, FALSE otherwise
			virtual BOOL RT_Review_Create_App(const char *pszTool_Name);



			// Member functions for setting the common data items across all review tool operations
			// Note: If you overide these then you must also set this level's RTInit *pCommon_Params,
			//		 RTPipes *pCommon_Pipes, and their boolean values
			virtual BOOL Set_Common_Params(RTInit *const pRT_Common_Params);
			virtual BOOL Set_Common_Pipes(RTPipes *const pRT_Commmon_Pipes);



			// Return type for RT_Title_Begin            
            typedef enum {
            	REVIEW_TITLE,		// Restart at title screen
            	REVIEW_SETUP,		// Next operation should be to go to setup mode
            	REVIEW_IMPORT,		// Next operation should be to go to import mode
            	REVIEW_REANALYZE,	// Next operation should be to go to reanalyze mode
            	REVIEW_EXIT,		// Exit the tool immediately
            	REVIEW_FAIL			// A fatal error occurred while working within this object
            	} REVIEW_OPERATION;
           // MUST provide the Review Begin procedure which is specific to your tool
			virtual REVIEW_OPERATION RT_Review_Begin(void) = 0;


            
            // RECEIVING OTHER REVIEW TOOL DATA (LOCATION AND TIMESTAMP) DISPLAY REQUEST
			// MUST overide one of the member function for the virtual prototypes below.  You must write the code to
			// display your specifice matching data according to the parameters passed.
            // NOTE: If your tool is based on event numbers, set the timestamps (and locations if necessary) within this class and
			//    then overide the RT_Display_My_Data(UINT uiEvent_Number) member functions
            // NOTE: If your tool is not based on single events, overide one of the other two according to whether you want to handle
			//    locations at the same time or not
            // NOTE: All Event numbers should start at 1!!!
			// NOTE: pszLocation will be NULL if you overide RT_Set_My_Location in order to catch location change message immediately!!!
			// NOTE: Be sure to always check that pszLocation is NOT NULL!!! pszLocation will be NULL if the other review tool never or can't send location!!!
			// No Y2K problem
			virtual BOOL RT_Display_My_Data(UINT uiEvent_Number);
			virtual BOOL RT_Display_My_Data(const char *pszTool_Name, const char *pszTimestamp);
			virtual BOOL RT_Display_My_Data(const char *pszTool_Name, const char *pszTimestamp, const char *pszLocation, BOOL *pbLocation_Failed);
			// If your tool has the ability to display ranges of data then overide the virtual member functions
			// No Y2K problem
			virtual BOOL RT_Display_My_Data_Area(UINT uiStart_Event_Number, UINT uiEnd_Event_Number);
			virtual BOOL RT_Display_My_Data_Area(const char *pszTool_Name, const char *pszStart_Timestamp, const char *pszEnd_Timestamp);
			virtual BOOL RT_Display_My_Data_Area(const char *pszTool_Name, const char *pszStart_Timestamp, const char *pszEnd_Timestamp, const char *pszLocation, BOOL *pbLocation_Failed);


			// REQUESTING OTHER REVIEW TOOL DATA DISPLAY (LOCATION AND DATA TIMESTAMP)
			// Your tool MUST have the ability to request data, call one of these appropriate member functions!!!
			// NOTE: Always pass the location and the upper layer will determine whether it really needs to be passed
			//       and the other review tool's RT_Set_My_Location called.  It follows these steps to make the determination:
			//        1. If the location parameter is NULL or bPass_Location is FALSE, do not send the location.
			//        2. If the RT_DATA_MATCH_ALWAYS_SEND_LOCATION parameter for this tool is set to TRUE then send the location.
			//        3. If a location has not been sent yet (make sure tools are in synch when they start), then send the location.
			//        4. If the last location sent to the other tool is NOT the same as this one, then send the location.
			//        5. If the last location sent to the other tool IS the same as this one, then do NOT send the location.
			// NOTE: If you call the event number member functions, set bPass_Location to TRUE, and have set locations with this class, then
			//       the apropriate location will also get passed.
			// No Y2K problem
			BOOL RT_Display_Other_Data(const char *pszTool_Name, UINT uiEvent_Number, BOOL bPass_Location = TRUE);
			BOOL RT_Display_Other_Data(const char *pszTool_Name, const char *pszTimestamp, const char *pszLocation = NULL);
			// If your tool has the ability to request ranges of data then call this member function
			// No Y2K problem
			BOOL RT_Display_Other_Data_Area(const char *pszTool_Name, const UINT uiStart_Event_Number, const UINT uiEnd_Event_Number, BOOL bPass_Location = TRUE);
			BOOL RT_Display_Other_Data_Area(const char *pszTool_Name, const char *pszStart_Timestamp, const char *pszEnd_Timestamp, const char *pszLocation = NULL);



			// RECEIVING AND SENDING OTHER REVIEW TOOL LOCATION CHANGE
			// Overide this member function to immediately handle a location setting message from another review tool
			// NOTE: If you do not need to handle location setting messages immediately, you may instead overide the
			//    RT_Display_My_Data and RT_Display_My_Data_Area which handles locations below.  In this case, the
			//    location is buffered for you until RT_Display_My_Data or RT_Display_My_Data_Area is called.
			// NOTE: If your tool does not handle locations then do nothing
			virtual BOOL RT_Set_My_Location(const char *pszTool_Name, const char *pszLocation);
			// Location sending member function
			// Use this member function if you only need to send the location,
			//    otherwise you should ALWAYS use the data matching member functions
			BOOL RT_Review_Set_Other_Location(const char *pszTool_Name, const char *pszLocation);



			// REANALYZING DATA
			// Only gets called by other tools, not commander
			// Must handle return case when initiate fails
			BOOL RT_Reanalyze_All_Data_Initiate(void);
			virtual BOOL RT_Reanalyze_All_Data_Perform(void);



            // MUST provide the RT_Review_My_Save member function which is specific to your tool
            // This member function is called when a message is received to save all of your data
     		BOOL RT_Review_Save(void);
     		virtual BOOL RT_Review_My_Save(void) = 0;


     		
            // MUST provide the RT_Review_My_Exit member function which is specific to your tool
            // This member function is called when a message is received to exit your tool
			// NOTE: Return value not handled
     		BOOL RT_Review_Exit(void);
     		virtual BOOL RT_Review_My_Exit(void) = 0;
     		// MUST call this procedure to provide consistent close message and properly close
     		//    all other tools if desired.
			// TRUE returned if should close this tool, FALSE returned if shouldn't close this tool     		
     		BOOL RT_Review_Exit_Question(RESOURCE_ID uiIcon = NULL, BOOL bSingle_Tool_Question = TRUE, const char *pszExit_Question = NULL, BOOL bUse_Small_Exit_Box = TRUE);



			// Part of the messaging interface, DO NOT call or do anything with these
			// No Y2K problem
			BOOL RT_Display_Data(const char *pszTool_Name, const char *pszTimestamp, BOOL *pbLocation_Failed);
			BOOL RT_Display_Data_Area(const char *pszTool_Name, const char *pszStart_Timestamp, const char *pszEnd_Timestamp, BOOL *pbLocation_Failed);
			BOOL RT_Set_Location(const char *pszTool_Name, const char *pszLocation);
			BOOL RT_Reanalyze_All_Data(void);



		protected:

			// Access to the common data items across all review tool operations
			// Note: No internal checking is provided when accessing the common data, it is left
			//			up to the user to use the boolean values to check to see if they are set yet.
			BOOL bCommon_Params_Set;
            RTInit *pCommon_Params;
			BOOL bCommon_Pipes_Set;
            RTPipes *pCommon_Pipes;

            
			// Delete all of the appropriate parameters created during reviewing
			// NOTE: This can not just be done in the destructor because must be deleted each time entered and exited review mode
			void RT_Review_RT_Review_Delete(void);

            // Before data displaying can take place for the event numbers, timestamps must be set
			// Locations passed must be the same number as Timestamps.  If they are passed, both locations
			//    and timestamps will be checked and the matching event number for BOTH will be passed.
            // NOTE: You must create the list of locations and timestamps dynamically, however this class deletes them
			// No Y2K problem
            BOOL Set_Locations(char **ppszLocations, UINT uiTotal_Rows_Number);
            BOOL Set_Timestamps(char **ppszStart_Timestamps, char **ppszEnd_Timestamps, UINT uiTotal_Rows_Number);
            // Returns the set timestamps, useful for saving
            // NOTE: Timestamps are const, do not delete the timestamps!
			// No Y2K problem
            BOOL Get_Locations(const char ***pppszLocations, UINT *puiLocations_Num);
            BOOL Get_Timestamps(const char ***pppszStart_Timestamps, const char ***pppszEnd_Timestamps, UINT *puiTimestamps_Num);
			// To properly minimize and restore your application, you need to register it with the 
			// appropriate member function below
			BOOL Set_Application(CGUI_Dlg *pApplication);
			BOOL Set_Application(CGUI_App *pApplication);
			// Call this member function to put incoming displaying messages on hold when you are busy 
			//    during long operations (Example - loading and saving)
			// There must be a matching pair of unhold for each hold. Example: Hold, Hold, Unhold. Unhold, NOT: Hold, Hold, Unhold
			BOOL Set_Messages_On_Hold(BOOL bStatus);


			// Possible useful member functions 
			// Sets the title bar to the app name and current database
			void RT_Review_Update_App_Title(CGUI_Dlg *pDlg, const char *pszDatabase_Name = NULL, BOOL bAdd_Only_DB_Name = FALSE);
			void RT_Review_Update_App_Title(CGUI_App *pApp, const char *pszDatabase_Name = NULL, BOOL bAdd_Only_DB_Name = FALSE);
			// Returns an allocate string with a header containing inspection information
			// Reloads the common inspection information if requested through bReload_Common_Inspec_Info parameter - in case someone else changed them
			// No Y2K problem
			BOOL Print_Header(char **ppszHeader, BOOL bAdd_Tool_Name_Title = TRUE, BOOL bReload_Common_Inspec_Info = TRUE, const char *pszDatabase_Name = NULL, const char *pszAppend_String = NULL);
			// Search routines for data matching
			// No Y2K problem
			// Timestamps only - first checks between any start and end, then check within any start end + uncertainty
			BOOL Event_Search(const char *pszTimestamp, UINT *puiEvent_Number);
			BOOL Event_Search(const char **ppszStart_Timestamps, const char **ppszEnd_Timestamps, UINT uiTotal_Rows_Number, const char *pszTimestamp, UINT *puiEvent_Number);
			// Start and End Timestamps and location - first checks between any start and end, then check within any start end + uncertainty
			BOOL Event_Search(const char *pszLocation, const char *pszTimestamp, UINT *puiEvent_Number, BOOL *pbLocation_Failed = NULL, BOOL *pbTimestamp_Failed = NULL);
			BOOL Event_Search(const char **ppszLocations, const char **ppszStart_Timestamps, const char **pszEnd_Timestamps, UINT uiTotal_Rows_Number, const char *pszLocation, const char *pszTimestamp, UINT *puiEvent_Number, BOOL *pbLocation_Failed = NULL, BOOL *pbTimestamp_Failed = NULL);

			// Get the julian equivalent from a review tool timestamp
			// No Y2K problem
			BOOL Get_Timstamp_Julian(const char *pszTimestamp, time_t *pJulian_Time);
			// Provides Open dialog box and returns heap allocated new database name or NULL if canceled - you provide actual database opening
			// First parameter is NULL terminated array of extension selections as defined by GUI.LIB
			// Second parameter is the default selected DB - Example: "*.odb"
			char *RT_Review_Open_New_DB(const char **pszDefault_Types, const char *pszDefault_DB_Name, BOOL bOnly_From_Common_DB_Path_Warning = TRUE, BOOL bOnly_From_Common_DB_Path = TRUE);
			// Provides SaveAs dialog box and returns heap allocated new database name or NULL if canceled  - you provide actual database saving
			// First parameter is NULL terminated array of extension selections as defined by GUI.LIB
			// Second parameter is the default selected DB - Example: "*.odb"
			char *RT_Review_SaveAs_DB(const char **pszDefault_Types, const char *pszDefault_DB_Name, BOOL bOnly_From_Common_DB_Path_Warning = TRUE, BOOL bOnly_From_Common_DB_Path = TRUE);
			// Provides CopyAs dialog box and returns heap allocated new database name or NULL if canceled  - you provide actual database saving
			// First parameter is NULL terminated array of extension selections as defined by GUI.LIB
			// Second parameter is the default selected DB - Example: "*.odb"
			char *RT_Review_SaveCopyAs_DB(const char **pszDefault_Types, const char *pszDefault_DB_Name, BOOL bOnly_From_Common_DB_Path_Warning = TRUE, BOOL bOnly_From_Common_DB_Path = TRUE);


      		// Use the default error messages below except when an error occurs specific to your tool
      		Review_Tool_Error RTError;
            void RT_Review_Error(uiRT_REVIEW_ERROR uiError, const char *pszError_Parameter_1 = NULL, const char *pszError_Parameter_2 = NULL);



        private:

            char **ppszLocs, **ppszStart_Stamps, **ppszEnd_Stamps;
			char **ppszRT_Last_Locations_Received, **ppszRT_Last_Locations_Sent;
			UINT uiRT_Total_Last_Locations_Received, uiRT_Total_Last_Locations_Sent;
            UINT uiTotal_Rows_Num;
            BOOL bHold_Status;
            
            
            CGUI_App *pApp;
            CGUI_Dlg *pDlg;


     		void Review_Tool_Exit_All_Tools(void);
     		void Review_Tool_Exit_This_Tool(void);

		};
		
#endif

