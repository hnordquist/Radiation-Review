// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 8/27/98

// RTReanalyze.H
// Reanalyze class definition for the abstract class Review Tool
// NOTE: This class is a pure virtual class and is never meant
// to be instantiated.  It is only meant to extract common elements
// away from your class.  Your review tool should inherit
// and overide all pure virtual classes and desired virtual classes
// in order to provide operations specific to your review tool.
       
       


#ifndef RTREANALYZE_H

	#define RTREANALYZE_H

	#include <gui_lib.h>
	#include <stdlib.h>
	
	#include "RTInit.H"
	#include "RTPipe.H"
	#include "RTError.H"
	

	#define RTREANALYZE_VERSION			__TIMESTAMP__


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
	#define uiRT_REANALYZE_ERROR						UINT
	#define uiRT_REANALYZE_ERROR_NO_MEMORY				100
	#define uiRT_REANALYZE_ERROR_PIPE					103
	#define uiRT_REANALYZE_ERROR_BUSY_EXIT				110

	
	
	
	class RTReanalyze : public CGUI_CallBack {

		public:
		
			// Operator class functions
			RTReanalyze(void); 
			virtual ~RTReanalyze(void); 
			

			// This member function will always be called as the first operation for this mode.
			// This allows an app to exist before the mode begins.
			// Overide it if you want to be sure that an app exists as soon as possible
			// NOTE: Only create the app (and do a gomodeless if you want), however do not do a go!!!
			// Return TRUE if app successfully created, FALSE otherwise
			virtual BOOL RT_Reanalyze_Create_App(const char *pszTool_Name);


			// Return type for RT_Reanalyze_Begin            
            typedef enum {
            	REANALYZE_TITLE,	// Next operation should be to return to the title screen
            	REANALYZE_SETUP,	// Next operation should be to setup new review
            	REANALYZE_IMPORT,	// Next operation should be to go to reanalyze mode
            	REANALYZE_REVIEW,	// Next operation should be to continue the last data being reviewed
            	REANALYZE_EXIT,		// Exit the tool immediately
            	REANALYZE_FAIL		// A fatal error occurred while working within this object
            	} REANALYZE_OPERATION;
            // MUST provide the Reanalyze Begin procedure which is specific to your tool
			virtual REANALYZE_OPERATION RT_Reanalyze_Begin(void) = 0;

			// Only needs to be overidden for tools which need to keep track of when another tool's reanalyze is done
			virtual BOOL RT_Reanalyze_Done(const char *pszTool);

            // MUST provide the Reanalyze Exit procedure which is specific to your tool
     		virtual BOOL RT_Reanalyze_Exit(void) = 0; 
     		

			// Member functions for setting the common data items across all review tool operations
			// Note: If you overide these then you must also set this level's RTInit *pCommon_Params,
			//		 RTPipes *pCommon_Pipes, and their boolean values
			// 
			virtual BOOL Set_Common_Params(RTInit *const pRT_Common_Params);
			virtual BOOL Set_Common_Pipes(RTPipes *const pRT_Commmon_Pipes);


     		// Call this procedure to close all open tools and provide proper busy messages
     		BOOL RT_Reanalyze_Exit_All_Tools(void);



		protected:

			// Access to the common data items across all review tool operations
			// Note: No internal checking is provided when accessing the common data, it is left
			//			up to the user to use the boolean values to check to see if they are set yet.
			BOOL bCommon_Params_Set;
            RTInit *pCommon_Params;
			BOOL bCommon_Pipes_Set;
            RTPipes *pCommon_Pipes;


			// Call this member function to put incoming displaying messages on hold when you are busy 
			//    during long operations (Example - loading and saving)
			BOOL Set_Messages_On_Hold(BOOL bStatus);

			
			// Use the default error messages below except when an error occurs specific to your tool
     		Review_Tool_Error RTError;
            void RT_Reanalyze_Error(uiRT_REANALYZE_ERROR uiError, const char *pszError_Parameter_1 = NULL);



        private:
			

		};
		
#endif

