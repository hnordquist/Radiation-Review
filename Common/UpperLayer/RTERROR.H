// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/24/97

// RTError.H
// Class definition for the abstract class Review Tool Error
// Common error messages across all Review Tools
       
       


#ifndef RTERROR_H

	#define RTERROR_H

	#include <gui_lib.h>
	#include <stdlib.h> 

	#include "SEBBoxes.H"
	#include "RTPipe.H"
	
	#include "gen.h"


	#define szRTERROR_VERSION			__TIMESTAMP__


	#define uiRT_ERROR								UINT
	#define uiRT_ERROR_NO_MEMORY					1
	#define uiRT_ERROR_NOT_INSTALLED				2		// Requires tool name to be passed
	#define uiRT_ERROR_INIT_LOAD					3		// Requires param errored for first and param warning for second, NULL accepted
	#define uiRT_ERROR_CANT_START_TOOL				4		// Requires tool name to be passed
	#define uiRT_ERROR_PIPE_OPEN_FAIL				5		// Requires tool name and your name to be passed
	#define uiRT_ERROR_PIPE							6		// Requires tool name to be passed
	#define uiRT_ERROR_PIPE_DEAD					15		// Requires tool name to be passed
	#define uiRT_ERROR_OPTIONS_SAVE_FAIL			20
	#define uiRT_ERROR_OPTIONS_RESET_FAIL			21
	#define uiRT_ERROR_DB_CANT_SAVE					35		// Requires DB name to be passed
	#define uiRT_ERROR_INSPEC_CHANGE				40
	#define uiRT_ERROR_OVERWRITE_RAWDATA		    50
	#define uiRT_ERROR_COMMON_INSPEC_LOAD			60		// Requires label warning name to be passed as first parameter
	#define uiRT_ERROR_COMMON_INSPEC_SAVE			61

	
	

	class Review_Tool_Error : protected CGUI_CallBack {

		public :
		
		    Review_Tool_Error(const char *pszMy_Name, RTPipes *pRTPipes = NULL);
		    Review_Tool_Error(void);
		    ~Review_Tool_Error(void);

			virtual BOOL Set_My_Name(const char *pszMy_Name);
			virtual BOOL Set_My_Pipes(RTPipes *pRT_Pipes);
		    
			virtual void Error(uiRT_ERROR uiError, const char *pszError_Parameter_1 = NULL, const char *pszError_Parameter_2 = NULL);
            
            
        private :
        
        	char *pszName;
			RTPipes *pPipes;
        	
		};
		
#endif


		
