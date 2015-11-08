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


		
