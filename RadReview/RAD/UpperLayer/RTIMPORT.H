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

// RTImport.H
// Import class definition for the abstract class Review Tool
// NOTE: This class is a pure virtual class and is never meant
// to be instantiated.  It is only meant to extract common elements
// away from your class.  Your review tool should inherit
// and overide all pure virtual classes and desired virtual classes
// in order to provide operations specific to your review tool.
       
       


#ifndef RTIMPORT_H

	#define RTIMPORT_H

	#include <gui_lib.h>
	#include <stdlib.h>
	
	#include "RTInit.H"
	#include "RTPipe.H"
	#include "RTError.H"
	

	#define RTIMPORT_VERSION			__TIMESTAMP__


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
	#define uiRT_IMPORT_ERROR							UINT
	#define uiRT_IMPORT_ERROR_NO_MEMORY					100
	#define uiRT_IMPORT_ERROR_PIPE						103
	#define uiRT_IMPORT_ERROR_DB_CANT_SAVE				105
	#define uiRT_IMPORT_ERROR_BUSY_EXIT					110

	
	
	
	class RTImport : public CGUI_CallBack {

		public:
		
			// Operator class functions
			RTImport(void); 
			virtual ~RTImport(void); 
			

			// This member function will always be called as the first operation for this mode.
			// This allows an app to exist before the mode begins.
			// Overide it if you want to be sure that an app exists as soon as possible
			// NOTE: Only create the app (and do a gomodeless if you want), however do not do a go!!!
			// Return TRUE if app successfully created, FALSE otherwise
			virtual BOOL RT_Import_Create_App(const char *pszTool_Name, BOOL bBypass_Import_Prompt = FALSE);


			// Return type for RT_Import_Begin            
            typedef enum {
            	IMPORT_TITLE,		// Next operation should be to return to the title screen
            	IMPORT_SETUP,		// Next operation should be to setup new review
            	IMPORT_REANALYZE,	// Next operation should be to go to reanalyze mode
            	IMPORT_REVIEW,		// Next operation should be to continue the last data being reviewed
            	IMPORT_EXIT,		// Exit the tool immediately
            	IMPORT_FAIL			// A fatal error occurred while working within this object
            	} IMPORT_OPERATION;
            // MUST provide the Import Begin procedure which is specific to your tool
			virtual IMPORT_OPERATION RT_Import_Begin(BOOL bBypass_Import_Prompt = FALSE) = 0;

			// Only needs to be overidden for tools which need to keep track of when another tool's import is done
			virtual BOOL RT_Import_Done(const char *pszTool);

            // MUST provide the Import Exit procedure which is specific to your tool
     		virtual BOOL RT_Import_Exit(void) = 0; 
     		

			// Member functions for setting the common data items across all review tool operations
			// Note: If you overide these then you must also set this level's RTInit *pCommon_Params,
			//		 RTPipes *pCommon_Pipes, and their boolean values
			// 
			virtual BOOL Set_Common_Params(RTInit *const pRT_Common_Params);
			virtual BOOL Set_Common_Pipes(RTPipes *const pRT_Commmon_Pipes);


     		// Call this procedure to close all open tools and provide proper busy messages
     		BOOL RT_Import_Exit_All_Tools(void);



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
            void RT_Import_Error(uiRT_IMPORT_ERROR uiError, const char *pszError_Parameter_1 = NULL);



        private:
			

		};
		
#endif

