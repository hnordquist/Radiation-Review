// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 4/22/97

// RTTitle.H
// Title class definition for the abstract class Review Tool
// NOTE: This class is a virtual class and is meant only to be
// overidden if you want to display a different title screen and/or
// perform a different set of operations during the review tool
// startup.  This class is ONLY used when the tool is run
// individually or is the first tool begun with a team of tools.
       


#ifndef RTTITLE_H

	#define RTTITLE_H

	#include <gui_lib.h>
	#include <gen.h>
	#include <gui_misc.h>
	
	#include <stdlib.h>
	#include <FLOAT.H> 
	
	#define COMPILE_WITH_TIMER
	#include "SEBMacro.H"  // Timer helper macro - must be after above #define

	#include "RTInit.H"
	#include "RTPipe.H"
	#include "RTError.H"
	
	#include "resource.h"
	

	#define RTTITLE_VERSION		__TIMESTAMP__


	// Use the default error messages below except when an error occurs specific to your tool
	// The commented ones are already defined
	//#define uiRT_ERROR_NO_MEMORY								1
	//#define uiRT_ERROR_NOT_INSTALLED							2		// Requires tool name to be passed
	//#define uiRT_ERROR_INIT_LOAD								3		// Requires param errored for first and param warning for second, NULL accepted
	//#define uiRT_ERROR_CANT_START_TOOL						4		// Requires tool name to be passed
	//#define uiRT_ERROR_PIPE_OPEN_FAIL							5		// Requires tool name and your name to be passed
	//#define uiRT_ERROR_PIPE									6		// Requires tool name to be passed
	//#define uiRT_ERROR_PIPE_DEAD								15		// Requires tool name to be passed
	//#define uiRT_ERROR_OPTIONS_SAVE_FAIL						20
	//#define uiRT_ERROR_OPTIONS_RESET_FAIL						21
	//#define uiRT_ERROR_DB_CANT_SAVE							35
	//#define uiRT_ERROR_INSPEC_CHANGE							40
	//#define uiRT_ERROR_OVERWRITE_RAWDATA						50
	//#define uiRT_ERROR_COMMON_INSPEC_LOAD						60		// Requires label warning name to be passed as first parameter
	//#define uiRT_ERROR_COMMON_INSPEC_SAVE						61
	#define uiRT_TITLE_ERROR									UINT
	#define uiRT_TITLE_ERROR_NO_MEMORY							100
	#define uiRT_TITLE_ERROR_INSPEC_CHANGE						105
	#define uiRT_TITLE_ERROR_MIC_INSPECTION_INFORMATION_FILE	110
	
	

	class RTTitle : public CGUI_CallBack {

		public:
		
			// Operator class functions
			RTTitle(RESOURCE_ID Icon = NULL, const char *pszName_Version = NULL, const char *pszProgrammer_Email = NULL, BOOL bCreate_Title_As_App = TRUE);
			virtual ~RTTitle(void); 


			// This member function will always be called as the first operation for this mode.
			// This allows an app to exist before the mode begins.
			// Overide it if you want to be sure that an app exists as soon as possible
			// NOTE: Only create the app (and do a gomodeless if you want), however do not do a go!!!
			// Return TRUE if app successfully created, FALSE otherwise
			virtual BOOL RT_Title_Create_App(const char *pszTool_Name);


			// Return type for RT_Title_Begin            
            typedef enum {
            	START_TITLE,		// Next operation should be to title of the tool
            	START_SETUP,		// Next operation should be to setup the tool (Then import, review)
            	START_IMPORT,		// Next operation should be to import new data	(Then review)
            	START_REANALYZE,	// Next operation should be to go to reanalyze mode
            	START_REVIEW,		// Next operation should be to continue the last data being reviewed
            	START_EXIT,			// Exit the tool immediately
            	START_FAIL			// A fatal error occurred while working within this object
            	} START_OPERATION;
			// Creates and displays default\common review tool title screen
			// NOTE: This class is used (this member functions called) only when this tool is
			//       run individually or is the first tool to be run within a team of tools
			// Return value is the next operation to be taken
			START_OPERATION RT_Title_Begin(void);
			BOOL RT_Title_Splash_Display(UINT uiSplash_Screen_Timeout_Seconds);
			BOOL RT_Title_Splash_Close(void);


            // Provide the Title Exit procedure if necessary
     		BOOL RT_Title_Exit(void);


			// Member functions for setting the common data items across all review tool operations
			// Note: If you overide these then you must also set this level's RTInit *pCommon_Params,
			//		 RTPipes *pCommon_Pipes, and their boolean values
			BOOL Set_Common_Params(RTInit *const pRT_Common_Params);
			BOOL Set_Common_Pipes(RTPipes *const pRT_Commmon_Pipes);


			// GUI.LIB FIX - Only global Exit validate functions work
			//               Must know when splash timesout and closes so that do not use splash dialog pointer later
			BOOL bSplash_Closed;



		protected:
            
			// Access to the common data items across all review tool operations
			// Note: No internal checking is provided when accessing the common data, it is left
			//			up to the user to use the boolean values to check to see if they are set yet.
			BOOL bCommon_Params_Set;
            RTInit *pCommon_Params;
			BOOL bCommon_Pipes_Set;
            RTPipes *pCommon_Pipes;


     		// Use the default error messages below except when an error occurs specific to your tool
     		Review_Tool_Error RTError;
            void RT_Title_Error(uiRT_TITLE_ERROR uiError, const char *pszError_Parameter_1 = NULL);


            
        private:

			// The title dialog box
			CGUI_Dlg *pSplash;
			CGUI_DlgApp *pTitle_DlgApp;
			CGUI_Dlg *pTitle_Dlg;
			CGUI_DlgApp *pCommon_Title;
			START_OPERATION eStart_Operation;
			RESOURCE_ID uiIcon;
			char *pszSplash_Title1, *pszSplash_Title2;
			BOOL bCreate_As_App, bInspection_Info_Open;
			
			// The title dialog box craetion and delation member functions
			BOOL RT_Title_Create(void);
			void RT_Title_Delete(void);
			void RT_Splash_Delete(void);

			// The title dialog box screen displaying member functions
		 	void RT_Title_Start_Screen_ON(void);
		 	void RT_Title_Start_Screen_OFF(void);

			// The title dialog box button member functions
			void RT_Title_Change(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void RT_Title_Continue(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void RT_Title_New(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void RT_Title_Exit(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL RT_Title_Exit_Validate(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);

			// Loads inspection information passed by MIC
			BOOL RT_Title_Load_MIC_Inspection_Information(void);

		};
		
#endif


		
