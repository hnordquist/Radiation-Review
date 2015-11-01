// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/10/97

// RevTool.H
// Class definition for the abstract class Review Tool
       
       


#ifndef REV_TOOL_H

	#define REV_TOOL_H

	#include <gui_lib.h>
	#include <stdlib.h> 
	#include <stdio.h>
	#include <string.h>
	#include <fstream.h>
	
	#include "RTTitle.H"
	#include "RTSetup.H"
	#include "RTImport.H"
	#include "RTReanalyze.H"
	#include "RTReview.H"

	#include "RTPipe.H"
	#include "RTInit.H"
	#include "RTError.H"
	#include "SEBBoxes.H"
	
	#include "gen.h"


	//#define szREVIEW_TOOL_VERSION		__TIMESTAMP__


	// No command line paramter value means to start in review mode
	#define szSKIP_RUNNING_MSG							"SKIP_RUNNING_MSG"
	#define szBEGIN_TITLE								"TITLE"
	#define szBEGIN_INSPECTION							"INSPECTION"
	#define szBEGIN_SETUP								"SETUP"
	#define szBEGIN_IMPORT								"IMPORT"
	#define szBEGIN_REANALYZE							"REANALYZE"
	#define szBEGIN_REVIEW								"REVIEW"

	
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
	#define uiREV_TOOL_ERROR							UINT
	#define uiREV_TOOL_ERROR_NO_MEMORY					100
    #define uiREV_TOOL_ERROR_INVALID_COMMAND_LINE		101		// Requires bad command line parameter to be passed as parameter
	#define uiREV_TOOL_ERROR_PIPE_OPEN_FAIL				110		// Requires tool name to be passed as parameter
	#define uiREV_TOOL_ERROR_PIPE						111		// Requires tool name to be passed as parameter
	#define uiREV_TOOL_ERROR_TITLE_FAIL					120
	#define uiREV_TOOL_ERROR_SETUP_FAIL					121
	#define uiREV_TOOL_ERROR_IMPORT_FAIL				122
	#define uiREV_TOOL_ERROR_REANALYZE_FAIL				123
	#define uiREV_TOOL_ERROR_REVIEW_FAIL				124
	#define uiREV_TOOL_ERROR_NOT_INSTALLED				130		// Requires tool name to be passed as parameter
	#define uiREV_TOOL_ERROR_CANT_START_TOOL			140		// Requires tool name to be passed as parameter
	#define uiREV_TOOL_ERROR_OPTIONS_SAVE_FAIL			150
	#define uiREV_TOOL_ERROR_OPTIONS_RESET_FAIL			151
	



	/////////////////////////////////////////////////////////////////////////////////////
	//  Global procedures for starting a Review Tool
	/////////////////////////////////////////////////////////////////////////////////////
	// Gets the command line arguments without having an application
	BOOL Review_Tool_GetCmdLineArgs(HINSTANCE hInstance, CGStr &Args, UINT *puiNumArgs, char **paszCmdLineParms[]);
	// Checks ini for proper setup and locations
	BOOL Review_Tool_Startup_Check(const char *pszTOOL_INI, const char *pszTOOL_NAME, UINT uiNum_Args, const char **ppszCommand_Line_Args);

	
	

	class Review_Tool : private RTPipe_Notify, protected CGUI_CallBack {

		public :
		
			// Create a review tool with your specific review tool objects
			//   NOTE: The classes could be your inherited classes or null for the generic class
			//         The last two parameters are not required - they will created if nothing passed
			Review_Tool(RESOURCE_ID uiRTIcon, RTSetup& rRT_Setup, RTImport& rRT_Import, RTReanalyze& rRT_Reanalyze, RTReview& rRT_Review, RTInit &rRT_Init, RTTitle *pRT_Title = NULL, RTPipes *pRT_Pipe = NULL);
			virtual ~Review_Tool(void); 
			
			// If a global app is created then the first parameter is TRUE, otherwise the Create_App member functions will be called in Title, Setup, Import, and Review
			virtual void Review_Tool_Begin(BOOL bApp_Already_Created, const char *pszTool_Name, UINT uiNum_Args, char **ppszCommand_Line_Args);
 

			// Used to be notified when a setup done message is received, calls appropriate operation with data
			virtual BOOL RT_Setup_Done(const char *pszPipe_Other_Name);
			// Used to be notified when a import done message is received, calls appropriate operation with data
			virtual BOOL RT_Import_Done(const char *pszPipe_Other_Name);
			// Used to be notified when a reanalyze done message is received, calls appropriate operation with data
			virtual BOOL RT_Reanalyze_Done(const char *pszPipe_Other_Name);
			// Used to be notified when a location change message is received, calls appropriate operation with data
			virtual BOOL RT_Location_Change(const char *pszPipe_Other_Name, const char *pszLocation);
			// Used to be notified when a data message is received, calls appropriate operation with data
			virtual BOOL RT_Display_Data(const char *pszPipe_Other_Name, const char *pszTimestamp, BOOL *pbLocation_Failed);
			// Used to be notified when a data area message is received, calls appropriate operation with data
			virtual BOOL RT_Display_Data_Area(const char *pszPipe_Other_Name, const char *pszStart_Timestamp, const char *pszEnd_Timestamp, BOOL *pbLocation_Failed);
			// Used to be notified when a reanalyze message is received, calls appropriate operation with data
			virtual BOOL RT_Reanalyze_All_Data(void);
			// Used to be notified when a pipe is not opened, start tool then open pipe
			virtual BOOL RT_Pipe_Not_Open(const char *pszPipe_Other_Name);
			// Used to be notified to display exit message box when a exit message is received through a pipe, calls current operation exit if yes
			virtual BOOL RT_Receive_Exit(const char *pszPipe_Other_Name);



		protected :

			// The required basic operations
			BOOL bApp_Created;
			char *pName;
			UINT uiIcon;
			RTSetup& rSetup;
			RTImport& rImport;
			RTReanalyze& rReanalyze;
			RTReview& rReview;

			// The non-required basic operations
			RTTitle *pTitle;
			BOOL bTitle_Provided;


			// Startup types
			enum RT_TASK_TAG {
				TASK_TITLE,
				TASK_SETUP,
		        TASK_IMPORT,
		        TASK_REANALYZE,
		        TASK_REVIEW,
		        TASK_SETUP_ONLY,
		        TASK_IMPORT_ONLY,
		        TASK_REANALYZE_ONLY,
		        TASK_REVIEW_ONLY, // REVIEW_ONLY does not really exist, will really be REVIEW
		        TASK_EXIT
		        } eCurrent_Task;
		        
			virtual void Review_Tool_Begin_Normal(void);
			virtual void Review_Tool_Begin_Abnormal(void);


            // Option Preferences
            RTInit &rInit; // Must be provided becuase don't knoe ini filename
			const RTInit& Get_Common_Params(void);

			
			// Communication Pipes
			RTPipes *pPipe;
			BOOL bPipe_Provided;
			const RTPipes& Get_Common_Pipe(void);


 			// Exit member functions
     		virtual void Review_Tool_Exit(void);			    

			// Error handling
			Review_Tool_Error RTError;
            virtual void Rev_Tool_Error(uiRT_ERROR uiError, const char *pszError_Parameter_1 = NULL, const char *pszError_Parameter_2 = NULL);



		private :

            BOOL bRT_Exit, bWas_Connected;
            
     		// Helper member functions
     		BOOL RT_Start_Tool(const char *pszTool);

            
		};
		
#endif


		
