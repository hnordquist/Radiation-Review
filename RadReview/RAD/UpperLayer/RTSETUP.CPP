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
// 3/12/97

// RTSetup.CPP
// Setup class definition for the abstract class Review Tool
       
       
#include "RTSetup.H"
	

/////////////////////////////////////////////////////////////////////////////////////
//  RTSetup constructor, destructor
/////////////////////////////////////////////////////////////////////////////////////

RTSetup::RTSetup(void) :
	bCommon_Params_Set(FALSE),
	pCommon_Params(NULL),
	bCommon_Pipes_Set(FALSE),
	pCommon_Pipes(NULL)
{}

 
RTSetup::~RTSetup(void) {}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Default app creation member function
//////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTSetup::RT_Setup_Create_App(const char *pszTool_Name) {

	// This should ONLY be called if Review_Tool was told to create the app
	return FALSE;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Only needs to be overidden for tools which need to keep track of when another tool's setup is done
//////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTSetup::RT_Setup_Done(const char *pTool) {

	return TRUE;
	
}


/////////////////////////////////////////////////////////////////////
// Access to the common data items across all review tool operations
/////////////////////////////////////////////////////////////////////

BOOL RTSetup::Set_Common_Params(RTInit *const pRT_Common_Params) {
    
    pCommon_Params = pRT_Common_Params;
    bCommon_Params_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Name(pCommon_Params->Get_My_Name());
    
	return bCommon_Params_Set;
	
}


BOOL RTSetup::Set_Common_Pipes(RTPipes *const pRT_Commmon_Pipes) {

    pCommon_Pipes = pRT_Commmon_Pipes;
    bCommon_Pipes_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Pipes(pCommon_Pipes);

	return bCommon_Pipes_Set;

}


/////////////////////////////////////////////////////////
// Possible useful member functions 
/////////////////////////////////////////////////////////

// Call this member function to put incoming displaying messages on hold when you are busy 
//    during long operations (Example - loading and saving)
BOOL RTSetup::Set_Messages_On_Hold(BOOL bStatus) {
    
    return (pCommon_Pipes->Set_Messages_On_Hold(bStatus));
	
}


//////////////////////////////////////////////////////////////////////////////////////////
// Use the default error messages below except when an error occurs specific to your tool
//////////////////////////////////////////////////////////////////////////////////////////

void RTSetup::RT_Setup_Error(uiRT_SETUP_ERROR uiError, const char *pszError_Parameter_1) {

	switch(uiError) {
		case uiRT_SETUP_ERROR_NO_MEMORY :
			RTError.Error(uiRT_ERROR_NO_MEMORY);
			break;
		default:
			RTError.Error(uiError, pszError_Parameter_1);
			break;			
    	}

}
		
