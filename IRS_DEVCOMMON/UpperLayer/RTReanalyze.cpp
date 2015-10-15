// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 8/27/98

// RTReanalyze.CPP
// Reanalyze class definition for the abstract class Review Tool
       
       
#include "RTReanalyze.H"
	

/////////////////////////////////////////////////////////////////////////////////////
//  RTReanalyze constructor, destructor
/////////////////////////////////////////////////////////////////////////////////////

RTReanalyze::RTReanalyze(void) :
	bCommon_Params_Set(FALSE),
	pCommon_Params(NULL),
	bCommon_Pipes_Set(FALSE),
	pCommon_Pipes(NULL)
{}

 
RTReanalyze::~RTReanalyze(void) {}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Default app creation member function
//////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTReanalyze::RT_Reanalyze_Create_App(const char *pszTool_Name) {

	// This should ONLY be called if Review_Tool was told to create the app
	return FALSE;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Only needs to be overidden for tools which need to keep track of when another tool's setup is done
//////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTReanalyze::RT_Reanalyze_Done(const char *pszTool) {

	return FALSE;
	
}


/////////////////////////////////////////////////////////////////////
// Access to the common data items across all review tool operations
/////////////////////////////////////////////////////////////////////

BOOL RTReanalyze::Set_Common_Params(RTInit *const pRT_Common_Params) {
    
    pCommon_Params = pRT_Common_Params;
    bCommon_Params_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Name(pCommon_Params->Get_My_Name());

	return bCommon_Params_Set;
	
}


BOOL RTReanalyze::Set_Common_Pipes(RTPipes *const pRT_Commmon_Pipes) {

    pCommon_Pipes = pRT_Commmon_Pipes;
    bCommon_Pipes_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Pipes(pCommon_Pipes);

	return bCommon_Pipes_Set;

}
 

/////////////////////////////////////////////////////////
// Possible useful member functions 
/////////////////////////////////////////////////////////

// Unlike Review, this one requires a bool value returned
BOOL RTReanalyze::RT_Reanalyze_Exit_All_Tools(void) {

	UINT ctIndex;
	char *pszResponse = NULL;
	BOOL bSuccess = TRUE;
	
	
	// Tell all of the review tools connected to exit
	for (ctIndex = 0; ctIndex < pCommon_Params->Get_Tools_Num(); ctIndex++) {
		if (pCommon_Pipes->Connected(pCommon_Params->Get_Pipe_Other_Name(ctIndex))) {
			// Check for a pipe failure
			if ((RTPipes::RTPIPE_RESULT::FAIL == pCommon_Pipes->Send(pCommon_Params->Get_Pipe_Other_Name(ctIndex), &pszResponse, szPIPE_EXIT_ALL, "")) ||
				(pszResponse == NULL)) {
				RT_Reanalyze_Error(uiRT_REANALYZE_ERROR_PIPE, pCommon_Params->Get_Tool_Name(ctIndex));
				bSuccess = FALSE;
				}
			// Check for a busy message
			else if (strncmp(pszResponse, "BUSY", strlen("BUSY")) == 0) {
				// It is busy, but still needs to close it's pipe, just tell it to close it's pipe
				if (RTPipes::RTPIPE_RESULT::FAIL == pCommon_Pipes->Send(pCommon_Params->Get_Pipe_Other_Name(ctIndex), FALSE, szPIPE_EXITED, pCommon_Params->Get_Pipe_My_Name(), ""))
					RT_Reanalyze_Error(uiRT_REANALYZE_ERROR_PIPE, pCommon_Params->Get_Tool_Name(ctIndex));
				RT_Reanalyze_Error(uiRT_REANALYZE_ERROR_BUSY_EXIT, pCommon_Params->Get_Tool_Name(ctIndex));
				bSuccess = FALSE;
				}
			// Failed
			else if (strncmp(pszResponse, szPIPE_EXIT_ALL, strlen(szPIPE_EXIT_ALL)) != 0) {
				RT_Reanalyze_Error(uiRT_REANALYZE_ERROR_PIPE, pCommon_Params->Get_Tool_Name(ctIndex));
				bSuccess = FALSE;
				}
			// Delete each response
			if (pszResponse) {
				delete [] pszResponse;
				pszResponse = NULL;
				}
			// Close the pipe
			pCommon_Pipes->Close(pCommon_Params->Get_Pipe_Other_Name(ctIndex));
			}
		}


	return bSuccess;

}


// Call this member function to put incoming displaying messages on hold when you are busy 
//    during long operations (Example - loading and saving)
BOOL RTReanalyze::Set_Messages_On_Hold(BOOL bStatus) {
    
    return (pCommon_Pipes->Set_Messages_On_Hold(bStatus));
	
}


//////////////////////////////////////////////////////////////////////////////////////////
// Use the default error messages below except when an error occurs specific to your tool
//////////////////////////////////////////////////////////////////////////////////////////

void RTReanalyze::RT_Reanalyze_Error(uiRT_REANALYZE_ERROR uiError, const char *pszError_Parameter_1) {

	CGStr Message;


	switch(uiError) {
		case uiRT_REANALYZE_ERROR_NO_MEMORY :
			RTError.Error(uiRT_ERROR_NO_MEMORY);
			break;
		case uiRT_REANALYZE_ERROR_PIPE :
			RTError.Error(uiRT_ERROR_PIPE, pszError_Parameter_1);
			break;
		case uiRT_REANALYZE_ERROR_BUSY_EXIT :
			Set_Messages_On_Hold(TRUE);
			Message = pszError_Parameter_1;
			Message += " is busy right now.  You need to exit it manually";
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			Set_Messages_On_Hold(FALSE);
			break;
		default:
			RTError.Error(uiError, pszError_Parameter_1);
			break;			
    	}


}


