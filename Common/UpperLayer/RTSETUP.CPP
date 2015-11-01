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
		
