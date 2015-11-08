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

// RTReview.CPP
// Review class definition for the abstract class Review Tool
       
       
#include "RTReview.H"
class CMsgBoxDlg;


/////////////////////////////////////////////////////////////////////////////////////
//  RTReview constructor, destructor
/////////////////////////////////////////////////////////////////////////////////////

RTReview::RTReview(void) :
	bCommon_Params_Set(FALSE),
	pCommon_Params(NULL),
	bCommon_Pipes_Set(FALSE),
	pCommon_Pipes(NULL),
	ppszLocs(NULL),
	ppszStart_Stamps(NULL),
	ppszEnd_Stamps(NULL),
	ppszRT_Last_Locations_Sent(NULL),
	uiRT_Total_Last_Locations_Sent(0),
	ppszRT_Last_Locations_Received(NULL),
	uiRT_Total_Last_Locations_Received(0),
	uiTotal_Rows_Num(0),
	pApp(NULL),
	pDlg(NULL),
	bHold_Status(FALSE) {}

 
RTReview::~RTReview(void) {

	// Make sure that everything really is deleted
	RT_Review_RT_Review_Delete();
   			
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Default app creation member function
//////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTReview::RT_Review_Create_App(const char *pszTool_Name) {

	// This should ONLY be called if Review_Tool was told to create the app
	return FALSE;

}


/////////////////////////////////////////////////////////////////////
// Delete all of the appropriate parameters created during reviewing
/////////////////////////////////////////////////////////////////////

void RTReview::RT_Review_RT_Review_Delete(void) {

	UINT ctIndex;

	
	// Delete all of the appropriate parameters created during reviewing
	if ((ppszLocs) && (uiTotal_Rows_Num))
		SEB_DELETELISTSTRING(ppszLocs, uiTotal_Rows_Num, ctIndex);
	if ((ppszStart_Stamps) && (uiTotal_Rows_Num))
		SEB_DELETELISTSTRING(ppszStart_Stamps, uiTotal_Rows_Num, ctIndex);
	if ((ppszEnd_Stamps) && (uiTotal_Rows_Num))
		SEB_DELETELISTSTRING(ppszEnd_Stamps, uiTotal_Rows_Num, ctIndex);
	if ((ppszRT_Last_Locations_Sent) && (uiRT_Total_Last_Locations_Sent))
		SEB_DELETELISTSTRING(ppszRT_Last_Locations_Sent, uiRT_Total_Last_Locations_Sent, ctIndex);
	if ((ppszRT_Last_Locations_Received) && (uiRT_Total_Last_Locations_Received))
		SEB_DELETELISTSTRING(ppszRT_Last_Locations_Received, uiRT_Total_Last_Locations_Received, ctIndex);

}


/////////////////////////////////////////////////////////////////////
// Access to the common data items across all review tool operations
/////////////////////////////////////////////////////////////////////

BOOL RTReview::Set_Common_Params(RTInit *const pRT_Common_Params) {
    
    pCommon_Params = pRT_Common_Params;
    bCommon_Params_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Name(pCommon_Params->Get_My_Name());

	return bCommon_Params_Set;
	
}


BOOL RTReview::Set_Common_Pipes(RTPipes *const pRT_Commmon_Pipes) {

    pCommon_Pipes = pRT_Commmon_Pipes;
    bCommon_Pipes_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Pipes(pCommon_Pipes);

	return bCommon_Pipes_Set;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup and configuration member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////

// Before data displaying can take place for the event numbers, timestamps must be set
// Locations passed must be the same number as Timestamps.  If they are passed, both locations
//    and timestamps will be checked and the matching event number for BOTH will be passed.
// NOTE: You must create the list of locations and timestamps dynamically, however this class deletes them
// No Y2K problem
BOOL RTReview::Set_Locations(char **ppszLocations, UINT uiTotal_Rows_Number) {

	UINT ctIndex;

    
	// Verify that the location and timestamp row numbers are the same
	if ((ppszStart_Stamps) && (uiTotal_Rows_Number != uiTotal_Rows_Num))
		return FALSE;
    
    if ((ppszLocations) && (uiTotal_Rows_Number > 0)) {
		if ((ppszLocs) && (uiTotal_Rows_Num))
			SEB_DELETELISTSTRING(ppszLocs, uiTotal_Rows_Num, ctIndex);
		ppszLocs = ppszLocations;
		uiTotal_Rows_Num = uiTotal_Rows_Number;
		return TRUE;
		}
		         
		         
	return FALSE;

}


BOOL RTReview::Set_Timestamps(char **ppszStart_Timestamps, char **ppszEnd_Timestamps, UINT uiTotal_Rows_Number) {

	UINT ctIndex;


	// Verify that the location and timestamp row numbers are the same
	if ((ppszLocs) && (uiTotal_Rows_Number != uiTotal_Rows_Num))
		return FALSE;
    
    if ((ppszStart_Timestamps) && (ppszEnd_Timestamps) && (uiTotal_Rows_Number > 0)) {
		if ((ppszStart_Stamps) && (uiTotal_Rows_Num))
			SEB_DELETELISTSTRING(ppszStart_Stamps, uiTotal_Rows_Num, ctIndex);
		if ((ppszEnd_Stamps) && (uiTotal_Rows_Num))
			SEB_DELETELISTSTRING(ppszEnd_Stamps, uiTotal_Rows_Num, ctIndex);
		ppszStart_Stamps = ppszStart_Timestamps;
		ppszEnd_Stamps = ppszEnd_Timestamps;
		uiTotal_Rows_Num = uiTotal_Rows_Number;
		return TRUE;
		}
		         
		         
	return FALSE;

}


BOOL RTReview::Get_Locations(const char ***pppszLocations, UINT *puiLocations_Num) {
    
    if ((ppszLocs) && (uiTotal_Rows_Num > 0)) {
		(*puiLocations_Num) = uiTotal_Rows_Num;
		(*pppszLocations) = (const char **)ppszLocs;
		return TRUE;
		}
	
	(*puiLocations_Num) = 0;
	(*pppszLocations) = NULL;


	return FALSE;

}


BOOL RTReview::Get_Timestamps(const char ***pppszStart_Timestamps, const char ***pppszEnd_Timestamps, UINT *puiTimestamps_Num) {
    
    if ((ppszStart_Stamps) && (ppszEnd_Stamps) && (uiTotal_Rows_Num > 0)) {
		(*puiTimestamps_Num) = uiTotal_Rows_Num;
		(*pppszStart_Timestamps) = (const char **)ppszStart_Stamps;
		(*pppszEnd_Timestamps) = (const char **)ppszEnd_Stamps;
		return TRUE;
		}

	(*puiTimestamps_Num) = 0;
	(*pppszStart_Timestamps) = NULL;
	(*pppszEnd_Timestamps) = NULL;


	return FALSE;

}


BOOL RTReview::Set_Application(CGUI_App *pApplication) {
    
    if (pApplication) {
		pApp = pApplication;
		return TRUE;
		}
	return FALSE;
	
}


BOOL RTReview::Set_Application(CGUI_Dlg *pApplication) {

    if (pApplication) {
		pDlg = pApplication;
		return TRUE;
		}
	return FALSE;
	
}


// Call this member function to put incoming displaying messages on hold when you are busy 
//    during long operations (Example - loading and saving)
BOOL RTReview::Set_Messages_On_Hold(BOOL bStatus) {
    
    if (!pCommon_Pipes->Set_Messages_On_Hold(bStatus))
		RT_Review_Error(uiRT_REVIEW_ERROR_NO_MATCHING_DATA_TOOL_START);
    bHold_Status = bStatus;

	
	return TRUE;
	
}


// RECEIVING OTHER REVIEW TOOL DATA (LOCATION AND TIMESTAMP) DISPLAY REQUEST
// MUST overide one of the member function for the virtual prototypes below.  You must write the code to
// display your specifice matching data according to the parameters passed.
// NOTE: If your tool is based on event numbers, set the timestamps (and locations if necessary) within this class and
//    then overide the RT_Display_My_Data(UINT uiEvent_Number) member functions
// NOTE: If your tool is not based on single events, overide one of the other two according to whether you want to handle
//    locations at the same time or not
// NOTE: All Event numbers should start at 1!!!
// NOTE: pszLocation will be NULL if you overide RT_Set_My_Location in order to catch location change message immediately!!!
// No Y2K problem
BOOL RTReview::RT_Display_My_Data(UINT uiEvent_Number) {

	return FALSE;
	
}


BOOL RTReview::RT_Display_My_Data(const char *pszTool_Name, const char *pszTimestamp) {

	UINT uiStart_Event;


	// If this one is not overidden then timestamp is not desired by user,
	//    user wants to just know the event number
	if (Event_Search(pszTimestamp, &uiStart_Event))
		return (RT_Display_My_Data(uiStart_Event));

		
	return FALSE;
	
}


BOOL RTReview::RT_Display_My_Data(const char *pszTool_Name, const char *pszTimestamp, const char *pszLocation, BOOL *pbLocation_Failed) {

	UINT uiStart_Event;
	BOOL bTimestamp_Failed;


	// If this one is not overidden then timestamp is not desired by user,
	//    user wants to just know the event number
	// If location is not NULL and user has set locations with this class then determine event based on location and timestamp
	// NOTE: pbLocation_Failed will not be NULL here because if not overidden a valid pointer is always passed
	if ((pszLocation) && (ppszLocs) && (uiTotal_Rows_Num)) {
		if (Event_Search(pszLocation, pszTimestamp, &uiStart_Event, pbLocation_Failed, &bTimestamp_Failed))
			return (RT_Display_My_Data(uiStart_Event));
		else return FALSE;
		}
	else {
		*pbLocation_Failed = FALSE;
		return (RT_Display_My_Data(pszTool_Name, pszTimestamp));
		}
	
}


// If your tool has the ability to display ranges of data then overide the virtual member functions
// No Y2K problem
BOOL RTReview::RT_Display_My_Data_Area(UINT uiStart_Event_Number, UINT uiEnd_Event_Number) {

	return FALSE;
	
}


BOOL RTReview::RT_Display_My_Data_Area(const char *pszTool_Name, const char *pszStart_Timestamp, const char *pszEnd_Timestamp) {

	UINT uiStart_Event, uiEnd_Event;


	// If this one is not overidden then timestamp is not desired by user,
	//    user wants to just know the event number
	if ((Event_Search(pszStart_Timestamp, &uiStart_Event)) && 
		(Event_Search(pszEnd_Timestamp, &uiEnd_Event)) &&
		(uiStart_Event <= uiEnd_Event))
		return (RT_Display_My_Data_Area(uiStart_Event, uiEnd_Event));
		
		
	return FALSE;

}


BOOL RTReview::RT_Display_My_Data_Area(const char *pszTool_Name, const char *pszStart_Timestamp, const char *pszEnd_Timestamp, const char *pszLocation, BOOL *pbLocation_Failed) {

	UINT uiStart_Event, uiEnd_Event;
	BOOL bTimestamp_Failed;


	// If this one is not overidden then timestamp is not desired by user,
	//    user wants to just know the event number
	// If location is not NULL and user has set locations with this class then determine event based on location and timestamp
	// NOTE: pbLocation_Failed will not be NULL here because if not overidden a valid pointer is always passed
	if ((pszLocation) && (ppszLocs) && (uiTotal_Rows_Num)) {
		if ((Event_Search(pszLocation, pszStart_Timestamp, &uiStart_Event, pbLocation_Failed, &bTimestamp_Failed)) && 
			(Event_Search(pszLocation, pszEnd_Timestamp, &uiEnd_Event, pbLocation_Failed, &bTimestamp_Failed)) &&
			(uiStart_Event <= uiEnd_Event))
			return (RT_Display_My_Data_Area(uiStart_Event, uiEnd_Event));
		else return FALSE;
		}
	else {
		*pbLocation_Failed = FALSE;
		return (RT_Display_My_Data_Area(pszTool_Name, pszStart_Timestamp, pszEnd_Timestamp));
		}
	
}


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
BOOL RTReview::RT_Display_Other_Data(const char *pszTool_Name, UINT uiEvent_Number, BOOL bPass_Location) {

	if ((uiEvent_Number >= 0) && (uiEvent_Number <= uiTotal_Rows_Num)) {
		// If a location is not passed and the user set locations then use the appropriate set location
		if ((bPass_Location) && (ppszLocs) && (uiTotal_Rows_Num))
			// Discreate data defaults to using the starting timestamp to match data
			return (RT_Display_Other_Data(pszTool_Name, ppszStart_Stamps[uiEvent_Number-1], ppszLocs[uiEvent_Number-1]));
		else
			// Discreate data defaults to using the starting timestamp to match data
			return (RT_Display_Other_Data(pszTool_Name, ppszStart_Stamps[uiEvent_Number-1], NULL));
		}
	else {
		RT_Review_Error(uiRT_REVIEW_ERROR_DB_CLOSED);
		return FALSE;
		}

}


BOOL RTReview::RT_Display_Other_Data(const char *pszTool_Name, const char *pszTimestamp, const char *pszLocation) {

	BOOL bSuccess = FALSE;
	char *pszResponse = NULL;
	const char *pszPipe_Other_Name = NULL;
	RTPipes::RTPIPE_RESULT eResult;


	// Check that the tool name passed is valid and get the other tools pipe name
	if ((!pszTool_Name) ||
		(!pCommon_Params->Get_Pipe_Other_Name(pszTool_Name, &pszPipe_Other_Name))) {
		RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pszTool_Name);
		return FALSE;
		}

	// First check that the tool can have data matched to it
	if (!pCommon_Params->Get_Data_Match_Tool(pCommon_Params->Get_Tool_Index_Number(pszTool_Name))) {
		RT_Review_Error(uiRT_REVIEW_ERROR_CANT_MATCH, pszTool_Name);
		return FALSE;
		}


	// Check to see if location needs to be sent
	if (!pCommon_Params->Get_Data_Match_Never_Send_Location(pCommon_Params->Get_Tool_Index_Number(pszTool_Name)))
		if (!RT_Review_Set_Other_Location(pszTool_Name, pszLocation))
			return FALSE;


	// Build the request strings and send them
	if (pszTimestamp) {
		eResult = pCommon_Pipes->Send(pszPipe_Other_Name, &pszResponse, szPIPE_DATA, pCommon_Params->Get_Pipe_My_Name(), "+", pszTimestamp, "");
		// If tool is just starting now then wait until it finishes
        if (eResult == RTPipes::RTPIPE_RESULT::CANCEL) {
			}
        else if (eResult == RTPipes::RTPIPE_RESULT::FAIL) {
			RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pszTool_Name);
			}
		else if (strncmp(pszResponse, "BUSY", strlen("BUSY")) == 0) {
			RT_Review_Error(uiRT_REVIEW_ERROR_BUSY, pszTool_Name);
			}
		// Location may not have been checked until now depending on what Display_My_Data the user overoad
		else if (strncmp(pszResponse, "LOCATION", strlen("LOCATION")) == 0) {
			// If found LOCATION then must have sent location, therefore pszLocation exists (or will just pass NULL)
			RT_Review_Error(uiRT_REVIEW_ERROR_NO_MATCHING_LOCATION, pszTool_Name, pszLocation);
			}
		// Location may not have been checked until now depending on what Display_My_Data the user overoad
		else if (strncmp(pszResponse, "TIMESTAMP", strlen("TIMESTAMP")) == 0) {
			RT_Review_Error(uiRT_REVIEW_ERROR_NO_MATCHING_DATE_AND_TIME, pszTool_Name, pszTimestamp);
			}
		else if (strncmp(pszResponse, szPIPE_DATA, strlen(szPIPE_DATA)) != 0) {
			RT_Review_Error(uiRT_REVIEW_ERROR_NO_MATCHING_DATE_AND_TIME);
			}
		else {
			if (pCommon_Params->Get_Data_Match_Minimize()) {
				if (pApp) ShowWindow(((HWND)(*(app->rootWindow()))), SW_MINIMIZE);
				if (pDlg) ShowWindow(pDlg->GetWndHandle(), SW_MINIMIZE);
				}
			bSuccess = TRUE;
			}
		if (pszResponse) {
			delete [] pszResponse;
			pszResponse = NULL;
			}
		}
	else
		RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pszTool_Name);


	// Be sure to stay foreground app if failed
	#ifdef _WIN32
		if ((!bSuccess) && (pDlg))
			SetForegroundWindow(pDlg->GetWndHandle());
	#endif


	return bSuccess;

}


// If your tool has the ability to request ranges of data then call this member function
// No Y2K problem
BOOL RTReview::RT_Display_Other_Data_Area(const char *pszTool_Name, const UINT uiStart_Event_Number, const UINT uiEnd_Event_Number, BOOL bPass_Location) {

	if ((uiStart_Event_Number >= 0) && (uiStart_Event_Number <= uiTotal_Rows_Num) &&
		(uiEnd_Event_Number >= 0) && (uiEnd_Event_Number <= uiTotal_Rows_Num) &&
		(uiStart_Event_Number <= uiEnd_Event_Number)) {
		// If a location is not passed and the user set locations then use the appropriate set location
		if ((bPass_Location) && (ppszLocs) && (uiTotal_Rows_Num))
			return (RT_Display_Other_Data_Area(pszTool_Name, ppszStart_Stamps[uiStart_Event_Number-1], ppszEnd_Stamps[uiEnd_Event_Number-1], ppszLocs[uiStart_Event_Number-1]));
		else
			return (RT_Display_Other_Data_Area(pszTool_Name, ppszStart_Stamps[uiStart_Event_Number-1], ppszEnd_Stamps[uiEnd_Event_Number-1], NULL));
		}
	else {
		RT_Review_Error(uiRT_REVIEW_ERROR_DB_CLOSED);
		return FALSE;
		}

}


BOOL RTReview::RT_Display_Other_Data_Area(const char *pszTool_Name, const char *pszStart_Timestamp, const char *pszEnd_Timestamp, const char *pszLocation) {

	BOOL bSuccess = FALSE;
	char *pszResponse = NULL;
	const char *pszPipe_Other_Name = NULL;
	RTPipes::RTPIPE_RESULT eResult;


	// Check that the tool name passed is valid and get the other tools pipe name
	if ((!pszTool_Name) ||
		(!pCommon_Params->Get_Pipe_Other_Name(pszTool_Name, &pszPipe_Other_Name))) {
		RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pszTool_Name);
		return FALSE;
		}

	// First check that the tool can have data matched to it
	if (!pCommon_Params->Get_Data_Match_Tool(pCommon_Params->Get_Tool_Index_Number(pszTool_Name))) {
		RT_Review_Error(uiRT_REVIEW_ERROR_CANT_MATCH, pszTool_Name);
		return FALSE;
		}


	// Check to see if location needs to be sent
	if (!pCommon_Params->Get_Data_Match_Never_Send_Location(pCommon_Params->Get_Tool_Index_Number(pszTool_Name)))
		if (!RT_Review_Set_Other_Location(pszTool_Name, pszLocation))
			return FALSE;


	// Build the request strings and send them
	if ((pszStart_Timestamp) && (pszEnd_Timestamp)) {
		eResult = pCommon_Pipes->Send(pszPipe_Other_Name, &pszResponse, szPIPE_DATA_AREA, pCommon_Params->Get_Pipe_My_Name(), "+", pszStart_Timestamp, "-", pszEnd_Timestamp, "");
        if (eResult == RTPipes::RTPIPE_RESULT::CANCEL) {
			}
		else if (eResult == RTPipes::RTPIPE_RESULT::FAIL) {
			RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pszTool_Name);
			}
		else if (strncmp(pszResponse, "BUSY", strlen("BUSY")) == 0) {
			RT_Review_Error(uiRT_REVIEW_ERROR_BUSY, pszTool_Name);
			}
		// Location may not have been checked until now depending on what Display_My_Data the user overoad
		else if (strncmp(pszResponse, "LOCATION", strlen("LOCATION")) == 0) {
			// If found LOCATION then must have sent location, therefore pszLocation exists (or will just pass NULL)
			RT_Review_Error(uiRT_REVIEW_ERROR_NO_MATCHING_LOCATION, pszTool_Name, pszLocation);
			}
		// Location may not have been checked until now depending on what Display_My_Data the user overoad
		else if (strncmp(pszResponse, "TIMESTAMP", strlen("TIMESTAMP")) == 0) {
			RT_Review_Error(uiRT_REVIEW_ERROR_NO_MATCHING_DATE_AND_TIME, pszTool_Name, pszStart_Timestamp);
			}
		else if (strncmp(pszResponse, szPIPE_DATA_AREA, strlen(szPIPE_DATA_AREA)) != 0) {
			RT_Review_Error(uiRT_REVIEW_ERROR_NO_MATCHING_DATE_AND_TIME);
			}
		else {
			if (pCommon_Params->Get_Data_Match_Minimize()) {
				if (pApp) ShowWindow(((HWND)(*(app->rootWindow()))), SW_MINIMIZE);
				if (pDlg) ShowWindow(pDlg->GetWndHandle(), SW_MINIMIZE);
				}
			bSuccess = TRUE;
			}			
		if (pszResponse) {
			delete [] pszResponse;
			pszResponse = NULL;
			}

		}                                       
	else
		RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pszTool_Name);
	
	
	// Be sure to stay foreground app if failed
	#ifdef _WIN32
		if ((!bSuccess) && (pDlg))
			SetForegroundWindow(pDlg->GetWndHandle());
	#endif


	return bSuccess;

}


// RECEIVING AND SENDING OTHER REVIEW TOOL LOCATION CHANGE
// Overide this member function to immediately handle a location setting message from another review tool
// NOTE: If you do not need to handle location setting messages immediately, you may instead overide the
//    RT_Display_My_Data and RT_Display_My_Data_Area which handles locations below.  In this case, the
//    location is buffered for you until RT_Display_My_Data or RT_Display_My_Data_Area is called.
// NOTE: If your tool does not handle locations then do nothing
BOOL RTReview::RT_Set_My_Location(const char *pszTool_Name, const char *pszLocation) {

	UINT ctIndex, uiTool_Index_Number;


	// Get the tool index number for ease later
	uiTool_Index_Number = pCommon_Params->Get_Tool_Index_Number(pszTool_Name);

	// Store the location so that it can be used later when a display data (timestamp) message comes in
	// Create the list of last locations sent now, this is the only time when all parameters are definitely known
	if (!ppszRT_Last_Locations_Received) {
		if ((ppszRT_Last_Locations_Received = new (char (*[pCommon_Params->Get_Tools_Num()]))) != NULL) {
			uiRT_Total_Last_Locations_Received = pCommon_Params->Get_Tools_Num();
			for (ctIndex = 0; ctIndex < uiRT_Total_Last_Locations_Received; ctIndex++)
				ppszRT_Last_Locations_Received[ctIndex] = NULL;
			}
		}

	// Success - Set the new location which was just received
	if (ppszRT_Last_Locations_Received[uiTool_Index_Number]) {
		delete [] ppszRT_Last_Locations_Received[uiTool_Index_Number];
		ppszRT_Last_Locations_Received[uiTool_Index_Number] = NULL;
		}
	if ((ppszRT_Last_Locations_Received[uiTool_Index_Number] = new char [strlen(pszLocation) + 1]) != NULL)
		strcpy(ppszRT_Last_Locations_Received[uiTool_Index_Number], pszLocation);
	else
		RT_Review_Error(uiRT_REVIEW_ERROR_NO_MEMORY);


	return TRUE;

}


// Location sending member function
// Use this member function if you only need to send the location,
//    otherwise you should ALWAYS use the data matching member functions
BOOL RTReview::RT_Review_Set_Other_Location(const char *pszTool_Name, const char *pszLocation) {

	BOOL bSuccess = TRUE;
	char *pszResponse = NULL;
	const char *pszPipe_Other_Name = NULL;
	RTPipes::RTPIPE_RESULT eResult;
	UINT ctIndex, uiTool_Index_Number;


	// Check that the tool name passed is valid and get the other pipes name
	if ((!pszTool_Name) ||
		(!pCommon_Params->Get_Pipe_Other_Name(pszTool_Name, &pszPipe_Other_Name))) {
		RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pszTool_Name);
		return FALSE;
		}


	//        1. If the location parameter is NULL, do not send the location.
	//        2. If the RT_DATA_MATCH_ALWAYS_SEND_LOCATION parameter for this tool is set to TRUE then send the location.
	//        3. If a location has not been sent yet (make sure tools are in synch when they start), then send the location.
	//        4. If the last location sent to the other tool is NOT the same as this one, then send the location.
	//        5. If the last location sent to the other tool IS the same as this one, then do NOT send the location.
	if (pszLocation) {

		// Get the tool index number for ease later
		uiTool_Index_Number = pCommon_Params->Get_Tool_Index_Number(pszTool_Name);

		// Create the list of last locations sent now, this is the only time when all parameters are definitely known
		if (!ppszRT_Last_Locations_Sent) {
			if ((ppszRT_Last_Locations_Sent = new (char (*[pCommon_Params->Get_Tools_Num()]))) != NULL) {
				uiRT_Total_Last_Locations_Sent = pCommon_Params->Get_Tools_Num();
				for (ctIndex = 0; ctIndex < uiRT_Total_Last_Locations_Sent; ctIndex++)
					ppszRT_Last_Locations_Sent[ctIndex] = NULL;
				}
			}

		// If not connected to the other tool yet, be sure that the location for that tool is cleared
		// This prevents the case that when tool1 and tool2 are connected, tool1 exits then reconnects to tool2, tool2
		//    has tool1's location still stored, tool2 therefore does not send tool1 the location, tool1 keeps getting NULL
		if (!pCommon_Pipes->Connected(pszPipe_Other_Name)) {
			if (ppszRT_Last_Locations_Sent[uiTool_Index_Number]) {
				delete [] ppszRT_Last_Locations_Sent[uiTool_Index_Number];
				ppszRT_Last_Locations_Sent[uiTool_Index_Number] = NULL;
				}
			}

		// Send the location if need always send, not sent yet, this one not last one sent
		if ((pCommon_Params->Get_Data_Match_Always_Send_Location(uiTool_Index_Number)) ||
			(!ppszRT_Last_Locations_Sent[uiTool_Index_Number]) ||
			(strcmp(ppszRT_Last_Locations_Sent[uiTool_Index_Number], pszLocation) != 0)) {
			// Send the location
			eResult = pCommon_Pipes->Send(pszPipe_Other_Name, &pszResponse, szPIPE_LOCATION, pCommon_Params->Get_Pipe_My_Name(), "+", pszLocation, "");
			if (eResult == RTPipes::RTPIPE_RESULT::CANCEL) {
				bSuccess = FALSE;
				}
			else if (eResult == RTPipes::RTPIPE_RESULT::FAIL) {
				RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pszTool_Name);
				bSuccess = FALSE;
				}
			else if (strncmp(pszResponse, "BUSY", strlen("BUSY")) == 0) {
				RT_Review_Error(uiRT_REVIEW_ERROR_BUSY, pszTool_Name);
				bSuccess = FALSE;
				}
			else if (strncmp(pszResponse, szPIPE_LOCATION, strlen(szPIPE_LOCATION)) != 0) {
				RT_Review_Error(uiRT_REVIEW_ERROR_NO_MATCHING_LOCATION, pszTool_Name, pszLocation);
				bSuccess = FALSE;
				}
			else {
				// Success - Set the new location which was just sent
				if (ppszRT_Last_Locations_Sent[uiTool_Index_Number]) {
					delete [] ppszRT_Last_Locations_Sent[uiTool_Index_Number];
					ppszRT_Last_Locations_Sent[uiTool_Index_Number] = NULL;
					}
				if ((ppszRT_Last_Locations_Sent[uiTool_Index_Number] = new char [strlen(pszLocation) + 1]) != NULL)
					strcpy(ppszRT_Last_Locations_Sent[uiTool_Index_Number], pszLocation);
				else
					RT_Review_Error(uiRT_REVIEW_ERROR_NO_MEMORY);
				}

			if (pszResponse) {
				delete [] pszResponse;
				pszResponse = NULL;
				}
			}
		}


	return bSuccess;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reanalyze member functions
////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTReview::RT_Reanalyze_All_Data_Initiate(void) {

	BOOL bSuccess = FALSE;
	char *pszResponse = NULL;
	RTPipes::RTPIPE_RESULT eResult;


	// Commander tool always performs the reanalyze
	// Tell the commmander tool that a reanalyze has been initiated
	if (pCommon_Params->Get_IR_Used()) {
		// Send the reanalyze message
		eResult = pCommon_Pipes->Send(pCommon_Params->Get_Pipe_Other_Name(pCommon_Params->Get_IR_Number()), &pszResponse, szPIPE_REANALYZE, pCommon_Params->Get_Pipe_My_Name(), "");
		if (eResult == RTPipes::RTPIPE_RESULT::CANCEL) {
			}
		else if (eResult == RTPipes::RTPIPE_RESULT::FAIL) {
			RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pCommon_Params->Get_Tool_Name(pCommon_Params->Get_IR_Number()));
			}
		else if (strncmp(pszResponse, "BUSY", strlen("BUSY")) == 0) {
			RT_Review_Error(uiRT_REVIEW_ERROR_BUSY, pCommon_Params->Get_Tool_Name(pCommon_Params->Get_IR_Number()));
			}
		else if (strncmp(pszResponse, szPIPE_REANALYZE, strlen(szPIPE_REANALYZE)) != 0) {
			RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pCommon_Params->Get_Tool_Name(pCommon_Params->Get_IR_Number()));
			}
		else {
			// Success
			bSuccess = TRUE;
			}

		if (pszResponse) {
			delete [] pszResponse;
			pszResponse = NULL;
			}
		}
	

	// Display common warning message about Reanalyzing not occuring
	if (!bSuccess)
		RT_Review_Error(uiRT_REVIEW_ERROR_INITIATE_REANALYZE_FAILED);


	return bSuccess;

}


BOOL RTReview::RT_Reanalyze_All_Data_Perform(void) {

	// Was not overidden, therefore must have called the wrong tool
	return FALSE;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Part of the messaging interface, DO NOT call or do anything with these
////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL RTReview::RT_Set_Location(const char *pszTool_Name, const char *pszLocation) {

	// If RT_Set_My_Location is overidden then pszLocation is used, otherwise location is saved 
	//     by RT_Review for data matching (timestamp) later.
	if (RT_Set_My_Location(pszTool_Name, pszLocation))
		return TRUE;


	return FALSE;
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Part of the messaging interface, DO NOT call or do anything with these
////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL RTReview::RT_Display_Data(const char *pszTool_Name, const char *pszTimestamp, BOOL *pbLocation_Failed) {

	char *pszLocation = NULL;
	char pszFixed_Timestamp[uiTIMESTAMP_LENGTH + 1];


	// FIX!!!!!!!!!!!!
	// Remove any location
	strcpy(pszFixed_Timestamp, pszTimestamp);
	pszFixed_Timestamp[0] = '0';
	pszFixed_Timestamp[1] = '0';
	pszFixed_Timestamp[2] = '0';
	pszFixed_Timestamp[3] = '0';
 	pszFixed_Timestamp[4] = '0';


	// Get the last location received.  If not keeping track of last received then user is handling incoming
	//    location messges immediately, therefore just pass NULL.
	if (ppszRT_Last_Locations_Received)
		pszLocation = ppszRT_Last_Locations_Received[pCommon_Params->Get_Tool_Index_Number(pszTool_Name)];


	// Call the RT_Display_My_Data highest in the hierarchy and eventually the one overidden will be called
	if (RT_Display_My_Data(pszTool_Name, pszFixed_Timestamp, pszLocation, pbLocation_Failed)) {
		if (pCommon_Params->Get_Data_Match_Restore()) {
			if (pApp) {
				// If is minimized, restore to last size and position, otherwise just bring to the foreground
				// app is the global application pointer
				if (IsIconic((HWND)(*(app->rootWindow()))))
					ShowWindow(((HWND)(*(app->rootWindow()))), SW_RESTORE);
				SetForegroundWindow((HWND)(*(app->rootWindow())));
				}
			if (pDlg) {
				// If is minimized, restore to last size and position, otherwise just bring to the foreground
				if (IsIconic(pDlg->GetWndHandle()))
					ShowWindow(pDlg->GetWndHandle(), SW_RESTORE);
				SetForegroundWindow(pDlg->GetWndHandle());
				}
			}
		return TRUE;
		}

	// Make sure I am minimized
	if ((!bHold_Status) && (pCommon_Params->Get_Data_Match_Minimize())) {
		// app is the global application pointer
		if (pApp) ShowWindow(((HWND)(*(app->rootWindow()))), SW_MINIMIZE);
		if (pDlg) ShowWindow(pDlg->GetWndHandle(), SW_MINIMIZE);
		}

		
	return FALSE;
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Part of the messaging interface, DO NOT call or do anything with these
////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL RTReview::RT_Display_Data_Area(const char *pszTool_Name, const char *pszStart_Timestamp, const char *pszEnd_Timestamp, BOOL *pbLocation_Failed) {

	char *pszLocation = NULL;
	char pszFixed_Start_Timestamp[uiTIMESTAMP_LENGTH + 1], pszFixed_End_Timestamp[uiTIMESTAMP_LENGTH + 1];


	// FIX!!!!!!!!!!!!
	// Remove any location
	strcpy(pszFixed_Start_Timestamp, pszStart_Timestamp);
	pszFixed_Start_Timestamp[0] = '0';
	pszFixed_Start_Timestamp[1] = '0';
	pszFixed_Start_Timestamp[2] = '0';
	pszFixed_Start_Timestamp[3] = '0';
 	pszFixed_Start_Timestamp[4] = '0';
	strcpy(pszFixed_End_Timestamp, pszEnd_Timestamp);
	pszFixed_End_Timestamp[0] = '0';
	pszFixed_End_Timestamp[1] = '0';
	pszFixed_End_Timestamp[2] = '0';
	pszFixed_End_Timestamp[3] = '0';
 	pszFixed_End_Timestamp[4] = '0';


	// Get the last location received
	if (ppszRT_Last_Locations_Received)
		pszLocation = ppszRT_Last_Locations_Received[pCommon_Params->Get_Tool_Index_Number(pszTool_Name)];


	// Call the RT_Display_My_Data highest in the hierarchy and eventually the one overidden will be called
	if (RT_Display_My_Data_Area(pszTool_Name, pszFixed_Start_Timestamp, pszFixed_End_Timestamp, pszLocation, pbLocation_Failed)) {
		if (pCommon_Params->Get_Data_Match_Restore()) {
			if (pApp) {
				// If is minimized, restore to last size and position, otherwise just bring to the foreground
				// app is the global application pointer
				if (IsIconic((HWND)(*(app->rootWindow()))))
					ShowWindow(((HWND)(*(app->rootWindow()))), SW_RESTORE);
				SetForegroundWindow((HWND)(*(app->rootWindow())));
				}
			if (pDlg) {
				// If is minimized, restore to last size and position, otherwise just bring to the foreground
				if (IsIconic(pDlg->GetWndHandle()))
					ShowWindow(pDlg->GetWndHandle(), SW_RESTORE);
				SetForegroundWindow(pDlg->GetWndHandle());
				}
			}
		return TRUE;
		}
		
	// Make sure I am minimized
	if ((!bHold_Status) && (pCommon_Params->Get_Data_Match_Minimize())) {
		if (pApp) ShowWindow(((HWND)(*(app->rootWindow()))), SW_MINIMIZE);
		if (pDlg) ShowWindow(pDlg->GetWndHandle(), SW_MINIMIZE);
		}

		
	return FALSE;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Part of the messaging interface, DO NOT call or do anything with these
////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL RTReview::RT_Reanalyze_All_Data(void) {

	if (RT_Reanalyze_All_Data_Perform()) {
		if (pCommon_Params->Get_Data_Match_Restore()) {
			if (pApp) {
				// If is minimized, restore to last size and position, otherwise just bring to the foreground
				// app is the global application pointer
				if (IsIconic((HWND)(*(app->rootWindow()))))
					ShowWindow(((HWND)(*(app->rootWindow()))), SW_RESTORE);
				SetForegroundWindow((HWND)(*(app->rootWindow())));
				}
			if (pDlg) {
				// If is minimized, restore to last size and position, otherwise just bring to the foreground
				if (IsIconic(pDlg->GetWndHandle()))
					ShowWindow(pDlg->GetWndHandle(), SW_RESTORE);
				SetForegroundWindow(pDlg->GetWndHandle());
				}
			}
		return TRUE;
		}
		
	// Make sure I am minimized
	if ((!bHold_Status) && (pCommon_Params->Get_Data_Match_Minimize())) {
		if (pApp) ShowWindow(((HWND)(*(app->rootWindow()))), SW_MINIMIZE);
		if (pDlg) ShowWindow(pDlg->GetWndHandle(), SW_MINIMIZE);
		}

		
	return FALSE;
	
}


// MUST provide the RT_Review_My_Save member function which is specific to your tool
// This member function is called when a message is received to save all of your data
BOOL RTReview::RT_Review_Save(void) {

	return (RT_Review_My_Save());
	
}


// MUST provide the RT_Review_My_Exit member function which is specific to your tool
// This member function is called when a message is received to exit your tool
BOOL RTReview::RT_Review_Exit(void) {

	BOOL bSuccess;
	

	// If is minimized, restore to last size and position
	if (pApp) {
		if (IsIconic((HWND)(*(app->rootWindow()))))
			ShowWindow(((HWND)(*(app->rootWindow()))), SW_RESTORE);
		}
	if (pDlg) {
		if (IsIconic(pDlg->GetWndHandle()))
			ShowWindow(pDlg->GetWndHandle(), SW_RESTORE);
		}
	bSuccess = RT_Review_My_Exit();
	

	return bSuccess;
	
}


// MUST call this procedure to provide consistent close message and properly close
//    all other tools if desired.
// TRUE returned if should close this tool, FALSE returned if shouldn't close this tool     		
BOOL RTReview::RT_Review_Exit_Question(RESOURCE_ID uiIcon, BOOL bSingle_Tool_Question, const char *pszExit_Question, BOOL bUse_Small_Exit_Box) {

	CGStr Message;
	BOOL bConnected = FALSE;
	UINT ctIndex, ctNewline_Index;
	GUI_ACTION Result = GUI_CANCEL;
	UINT uiButton_Pressed;

	
	Set_Messages_On_Hold(TRUE);


	// Check to see if connected to any other tools
	for (ctIndex = 0; ctIndex < pCommon_Params->Get_Tools_Num(); ctIndex++)
		if (pCommon_Pipes->Connected(pCommon_Params->Get_Pipe_Other_Name(ctIndex)))
			bConnected = TRUE;

                   
	// Running with other tools
	if (bConnected) {
		if (!pszExit_Question) {
    		Message = "\nDo you want to exit all of the tools or just\n";
			Message += pCommon_Params->Get_My_Name();
			Message += "?\n\n";
			}
		else Message = pszExit_Question;

		// GUI.LIB FIX: If message contains to me characters on one line then msgbox defined with buttons does not appear
		ctNewline_Index = 90;
		for (ctIndex = 0; ctIndex < strlen(Message.Get_sz()); ctIndex++) {
			if (isupper(Message[ctIndex])) ctNewline_Index -= 1;
			if (Message[ctIndex] == '\n')
				ctNewline_Index = ctIndex + 90;
			else if (ctIndex >= ctNewline_Index) {
				if (Message[ctIndex] == ' ') {
					Message[ctIndex] = '\n';
					ctNewline_Index = ctIndex + 90;
					}
				}
			}

		// Start the box
		const char *Buttons[] = {"Exit &ALL Tools", "Exit Only &THIS Tool", "&Cancel", NULL};
		uiButton_Pressed = GUI_MsgBox(Message.Get_sz(), GUI_ICON_QUESTION, Buttons);

		// Determine what button was pressed and perform the appropriate action
		if (uiButton_Pressed == 0) {
			Result = GUI_OK;
			Review_Tool_Exit_All_Tools();
			}
		else if (uiButton_Pressed == 1) {
			Result = GUI_OK;
			Review_Tool_Exit_This_Tool();
			}
		else Result = GUI_CANCEL;
		}		
		
 	// Just this tool is running	
	else if (bSingle_Tool_Question) {
		if (!pszExit_Question) {
    		Message = "\nAre you sure you want to exit ";
			Message += pCommon_Params->Get_My_Name();
			Message += "?";
			}
		else Message = pszExit_Question;

		// Start the box
		if (GUI_YES == GUI_MsgBox(Message.Get_sz(), GUI_ICON_QUESTION, GUI_YESNO))
			Result = GUI_YES;
		else Result = GUI_CANCEL;
		}

	#ifdef _WIN32
		if (pDlg) 
			SetForegroundWindow(pDlg->GetWndHandle());
	#endif


	Set_Messages_On_Hold(FALSE);

		                          
	// If did not even present question then return TRUE (just exit)
	// MUST BE FIRST
	if (!bConnected && !bSingle_Tool_Question)
		return TRUE;
	// Whether connected or not, if canceled, then return FALSE
	else if (Result == GUI_CANCEL)
		return FALSE;
	// Whether connected or not, exit was chosen, return TRUE
	else return TRUE;
		
}     


// Unlike Import, this one can not have a bool value returned
void RTReview::Review_Tool_Exit_All_Tools(void) {

	UINT ctIndex;
	char *pszResponse = NULL;


	// Tell all of the review tools connected to to exit
	for (ctIndex = 0; ctIndex < pCommon_Params->Get_Tools_Num(); ctIndex++) {
		if (pCommon_Pipes->Connected(pCommon_Params->Get_Pipe_Other_Name(ctIndex))) {
			// Check for a pipe failure
			if ((RTPipes::RTPIPE_RESULT::FAIL == pCommon_Pipes->Send(pCommon_Params->Get_Pipe_Other_Name(ctIndex), &pszResponse, szPIPE_EXIT_ALL, "")) ||
				(pszResponse == NULL)) {
				RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pCommon_Params->Get_Tool_Name(ctIndex));
				}
			// Check for a busy message
			else if (strncmp(pszResponse, "BUSY", strlen("BUSY")) == 0) {
				// It is busy, but still needs to close it's pipe, just tell it to close it's pipe
				if (RTPipes::RTPIPE_RESULT::FAIL == pCommon_Pipes->Send(pCommon_Params->Get_Pipe_Other_Name(ctIndex), FALSE, szPIPE_EXITED, pCommon_Params->Get_Pipe_My_Name(), "")) 
					RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pCommon_Params->Get_Tool_Name(ctIndex));
				RT_Review_Error(uiRT_REVIEW_ERROR_BUSY_EXIT, pCommon_Params->Get_Tool_Name(ctIndex));
				}
			// Failed
			else if (strncmp(pszResponse, szPIPE_EXIT_ALL, strlen(szPIPE_EXIT_ALL)) != 0) {
				RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pCommon_Params->Get_Tool_Name(ctIndex));
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

}

			    
void RTReview::Review_Tool_Exit_This_Tool(void) {

	UINT ctIndex;
	char *pszResponse = NULL;
	BOOL bConnected_Found = FALSE;
	
	
	// Tell all of the reivew tools that this tool is only exiting
	for (ctIndex = 0; ctIndex < pCommon_Params->Get_Tools_Num(); ctIndex++) {
		if (pCommon_Pipes->Connected(pCommon_Params->Get_Pipe_Other_Name(ctIndex))) {
			if (RTPipes::FAIL == pCommon_Pipes->Send(pCommon_Params->Get_Pipe_Other_Name(ctIndex), FALSE, szPIPE_EXITED, pCommon_Params->Get_Pipe_My_Name(), "")) {
				// If failed, still could be using old exit command message, try it next
				if (RTPipes::FAIL == pCommon_Pipes->Send(pCommon_Params->Get_Pipe_Other_Name(ctIndex), FALSE, szPIPE_EXIT_ALL, ""))
					RT_Review_Error(uiRT_REVIEW_ERROR_PIPE, pCommon_Params->Get_Tool_Name(ctIndex));
				}
			pCommon_Pipes->Close(pCommon_Params->Get_Pipe_Other_Name(ctIndex));
			}
		}
		
				
	if (pszResponse) {
		delete [] pszResponse;
		pszResponse = NULL;
		}
	
}
 
 
//////////////////////////////////////////////////////
// Possible useful member functions
//////////////////////////////////////////////////////

void RTReview::RT_Review_Update_App_Title(CGUI_Dlg *pDlg, const char *pszDatabase_Name, BOOL bAdd_Only_DB_Name) {

	CGStr App_Title;
	char *pszDB_Name = NULL;
	

	App_Title = pCommon_Params->Get_My_Name();
	// Set the title of the app to also contain the current DB name
	if (pszDatabase_Name) {
		App_Title += " - ";
		if (!bAdd_Only_DB_Name) {
			App_Title += pszDatabase_Name;
			}
		// Just add the DB name
		else {
			pszDB_Name = strrchr(pszDatabase_Name, '\\');
			// if the '\' was not found then must just be the DB name
			if ((pszDB_Name) && (pszDB_Name[1] != NULL))
				App_Title += &(pszDB_Name[1]);
			else 
				App_Title += pszDatabase_Name;
			}
		}


	pDlg->SetTitle(App_Title.Get_sz());

}



void RTReview::RT_Review_Update_App_Title(CGUI_App *pApp, const char *pszDatabase_Name, BOOL bAdd_Only_DB_Name) {

	CGStr App_Title;
	char *pszDB_Name = NULL;
	

	App_Title = pCommon_Params->Get_My_Name();
	// Set the title of the app to also contain the current DB name
	if (pszDatabase_Name) {
		App_Title += " - ";
		if (!bAdd_Only_DB_Name) {
			App_Title += pszDatabase_Name;
			}
		// Just add the DB name
		else {
			pszDB_Name = strrchr(pszDatabase_Name, '\\');
			// if the '\' was not found then must just be the DB name
			if ((pszDB_Name) && (pszDB_Name[1] != NULL))
				App_Title += &(pszDB_Name[1]);
			else 
				App_Title += pszDatabase_Name;
			}
		}


	pApp->SetTitle(App_Title.Get_sz());

}


BOOL RTReview::Print_Header(char **ppszHeader, BOOL bAdd_Tool_Name_Title, BOOL bReload_Common_Inspec_Info, const char *pszDatabase_Name, const char *pszAppend_String) {

	// Integrated Review - DATABASE NAME
	// -----------------
	//
	// 72.05.16 - 15:23:31
	//
	// Facility Name .............. Rokkasho Spent Fuel Storage (JXS)
	// Location in Facility ....... Canal A
	// Inspection Date/Time ....... 1972.5.16
	// Inspection Number .......... 96005
	// Inspectors ................. Steve Buck - UofI, Shirley Klosterbuer - UofMin,
	//                              Tom Marks - Uof?
	// Data Period ................ 1996.5.16 - 1:23  to  1997.5.16 - 14:59
	// Comments ................... This is just an example of what a header would look
    //                              like for a printout. 
	     

	CGStr Header;
	time_t Time;
	struct tm *Time_Struct;
	UINT ctIndex, uiComment_Position = 0, uiComment_Length;
	char szTemp_Number[11], szComments[41], *pszComment_Position;
	

	// If requested to reload the common inspection information then do so
	if (bReload_Common_Inspec_Info)
		// Don't care about return value, if failed then it will automatically clear info and partial info string will be returned
		pCommon_Params->Reload_Common_Inspec_Info();


    Header = ""; // Must have because do not know if will add below
	if (bAdd_Tool_Name_Title) {
		Header += pCommon_Params->Get_My_Name();
		if (pszDatabase_Name) {
			Header += " - ";
			Header += pszDatabase_Name;
			}
		Header += '\n';
		for (ctIndex = 0; ctIndex < strlen(pCommon_Params->Get_My_Name()); ctIndex++)
			Header += '-';
		Header += "\n\n";
		}
	else
		Header = "\n";  // Must have for the '='


	time(&Time);
	Time_Struct = localtime(&Time);
	Header += asctime(Time_Struct);


	Header += "\n\nFacility Name .............. ";
	Header += pCommon_Params->Get_Facility_Name();
	Header += "\nLocation in Facility ....... ";
	Header += pCommon_Params->Get_Facility_Location();

	Header += "\nInspection Date/Time ....... ";
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_Inspection_Date()->yr);
	Header += szTemp_Number;
	Header += '.';
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_Inspection_Date()->mon);
	Header += szTemp_Number;
	Header += '.';
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_Inspection_Date()->day);
	Header += szTemp_Number;
	Header += " - ";
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_Inspection_Time()->hr);
	Header += szTemp_Number;
	Header += ':';
	sprintf(szTemp_Number, "%02u", pCommon_Params->Get_Inspection_Time()->min);
	Header += szTemp_Number;

	Header += "\nInspection Number .......... ";
	Header += pCommon_Params->Get_Inspection_Number();
	Header += "\nInspectors ................. ";
	for (ctIndex = 0; ctIndex < pCommon_Params->Get_Inspector_ID_Choices_Num(); ctIndex++) {
		Header += pCommon_Params->Get_Inspector_ID(pCommon_Params->Get_Inspector_ID_Choice(ctIndex));
		if (ctIndex != pCommon_Params->Get_Inspector_ID_Choices_Num() - 1)
			Header += "\n                             ";
		}
    Header += "\nData Period ................ ";
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_Start_Date()->yr);
	Header += szTemp_Number;
	Header += '.';
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_Start_Date()->mon);
	Header += szTemp_Number;
	Header += '.';
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_Start_Date()->day);
	Header += szTemp_Number;
	Header += " - ";
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_Start_Time()->hr);
	Header += szTemp_Number;
	Header += ':';
	sprintf(szTemp_Number, "%02u", pCommon_Params->Get_Start_Time()->min);
	Header += szTemp_Number;
	Header += "  to  ";
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_End_Date()->yr);
	Header += szTemp_Number;
	Header += '.';
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_End_Date()->mon);
	Header += szTemp_Number;
	Header += '.';
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_End_Date()->day);
	Header += szTemp_Number;
	Header += " - ";
	sprintf(szTemp_Number, "%u", pCommon_Params->Get_End_Time()->hr);
	Header += szTemp_Number;
	Header += ':';
	sprintf(szTemp_Number, "%02u", pCommon_Params->Get_End_Time()->min);
	Header += szTemp_Number;
    Header += "\nComments ................... ";
    uiComment_Length = strlen(pCommon_Params->Get_Inspection_Comments());
    while (uiComment_Position < uiComment_Length) {
    	strncpy(szComments, &(pCommon_Params->Get_Inspection_Comments()[uiComment_Position]), 40);
    	szComments[40] = NULL;
		// Make sure word not split and only drop if not at end
    	if ((uiComment_Position + strlen(szComments) < uiComment_Length) &&
			((pszComment_Position = (strrchr(szComments, ' '))) != NULL))
    		pszComment_Position[1] = NULL; // 1 because don't want space later
	    Header += szComments;
	    uiComment_Position += strlen(szComments);
    	if (uiComment_Position < uiComment_Length)
    		Header += "\n                             ";
    	}
    Header += '\n';

	if (pszAppend_String) {
		Header += pszAppend_String;
		Header += '\n';
		}


    return (Header.Create_sz(ppszHeader));
    
}


// Timestamps only
BOOL RTReview::Event_Search(const char *pszTimestamp, UINT *puiEvent_Number) {

	return (Event_Search((const char **)ppszStart_Stamps, (const char **)ppszEnd_Stamps, uiTotal_Rows_Num, pszTimestamp, puiEvent_Number));
	
}


// Timestamps only
BOOL RTReview::Event_Search(const char **ppszStart_Timestamps, const char **ppszEnd_Timestamps, UINT uiTotal_Rows_Number, const char *pszTimestamp, UINT *puiEvent_Number) {

	// NOTE: THIS CODE IS REPEATED IN OTHER EVENT_SEARCH ALGORITHM, ANY CHANGES HERE SHOULD BE MADE THERE TOO!!!!
	// This member function is written to be as fast as possible, therefore it dublicates the other event search with the removal of location

	BOOL bSuccess = FALSE;
	BOOL bBetween_Start_End_Found, bHigher_Starting_Found, bLower_Ending_Found;
	int ctPrevious_Timestamp;
	time_t Higher_Starting_Julian, Lower_Ending_Julian, Compare_Julian;


	// Y2K - No problems below, based on timestamp comparison (with 4 digit year) and Julian below is OK

	(*puiEvent_Number) = 0;
	if ((ppszStart_Timestamps) && (ppszEnd_Timestamps) && (pszTimestamp)) {

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Step #1: First try to determine if the timestamp is between any starting and ending timestamp pair
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		bBetween_Start_End_Found = FALSE;
		// Step #1 - Search for timestamp based on strcmp which is greater then requested
		while ((!bBetween_Start_End_Found) && ((*puiEvent_Number) < uiTotal_Rows_Number)) {
			// Find start and end timestamp pair which is around or equal
			if ((ppszStart_Timestamps[(*puiEvent_Number)][0] != NULL) && // Just check start, end should have same nulls
				(strcmp(ppszStart_Timestamps[(*puiEvent_Number)], pszTimestamp) >= 0) &&
				(strcmp(ppszEnd_Timestamps[(*puiEvent_Number)], pszTimestamp) <= 0)) {
				bBetween_Start_End_Found = TRUE;
				bSuccess = TRUE;
				}
			else
				(*puiEvent_Number)++;
			}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Step #2: If the timestamp is not between any starting and ending timestamp pair, try to look
		//    for the first pair which is within the allowed uncertainty
		//          If the timestamp is between a pair, then success and just bottom out
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (!bBetween_Start_End_Found) {

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Step #2A - Search for starting timestamp based on strcmp which is higher then requested
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Reset the event number since starting over
			(*puiEvent_Number) = 0;
			bHigher_Starting_Found = FALSE;
			while ((!bHigher_Starting_Found) && ((*puiEvent_Number) < uiTotal_Rows_Number)) {
				// Find my timestamp greater or equal then one sent
				if ((ppszStart_Timestamps[(*puiEvent_Number)][0] != NULL) &&
					(strcmp(ppszStart_Timestamps[(*puiEvent_Number)], pszTimestamp) >= 0))
					bHigher_Starting_Found = TRUE;
				else
					(*puiEvent_Number)++;
				}

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Step #2B - Search for ending timestamp based on strcmp which is lower then requested
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (bHigher_Starting_Found)
				// Below used when need to find next lowest from highest
				ctPrevious_Timestamp = (int)(*puiEvent_Number) - 1; // Allows starting one below
			else 
				// Below used when a higher can not be found - need to check if last in allowed range
				ctPrevious_Timestamp = (int)uiTotal_Rows_Number - 1;
			bLower_Ending_Found = FALSE;
			// Perform the lower search
			while ((!bLower_Ending_Found) && (ctPrevious_Timestamp >= 0)) {
				if (ppszEnd_Timestamps[ctPrevious_Timestamp][0] != NULL)
					bLower_Ending_Found = TRUE;
				else
					ctPrevious_Timestamp--;
				}

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Step #3 - Determine if there is an earlier event allowed within the tolerence or next event within
			//           the tolerence
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Both the previous ending timestamp and next starting timestamp was found
			// Determine which was closest and if it is within the allowed tolerence
			if ((bHigher_Starting_Found) && (bLower_Ending_Found)) {
				if ((Get_Timstamp_Julian(pszTimestamp, &Compare_Julian)) &&
					(Get_Timstamp_Julian(ppszEnd_Timestamps[ctPrevious_Timestamp], &Lower_Ending_Julian)) &&
					(Get_Timstamp_Julian(ppszStart_Timestamps[(*puiEvent_Number)], &Higher_Starting_Julian))) {

					// Find the closer one - NOTE: Start and End should never overlap
					if ((Compare_Julian - Lower_Ending_Julian) < (Higher_Starting_Julian - Compare_Julian)) {
						// Verify that lower is within allowed range
						if ((Compare_Julian - Lower_Ending_Julian) <= pCommon_Params->Get_Data_Match_Error_Seconds()) {
							(*puiEvent_Number) = ctPrevious_Timestamp;
							bSuccess = TRUE;
							}
						}
						// Verify that higher is within allowed range
					else if ((Higher_Starting_Julian - Compare_Julian) <= pCommon_Params->Get_Data_Match_Error_Seconds())
						bSuccess = TRUE;

					}

				}

			// Only the next event was found, determine if it is within the tolerence
			else if (bHigher_Starting_Found) {
				if ((Get_Timstamp_Julian(pszTimestamp, &Compare_Julian)) &&
					(Get_Timstamp_Julian(ppszStart_Timestamps[(*puiEvent_Number)], &Higher_Starting_Julian))) {

					if ((Higher_Starting_Julian - Compare_Julian) <= pCommon_Params->Get_Data_Match_Error_Seconds())
						bSuccess = TRUE;

					}
				}

			// Only the previous event was found, determine if it is within the tolerence
			else if (bLower_Ending_Found) {  // If no data then a lower not found!
				if ((Get_Timstamp_Julian(pszTimestamp, &Compare_Julian)) &&
					(Get_Timstamp_Julian(ppszEnd_Timestamps[ctPrevious_Timestamp], &Lower_Ending_Julian))) {

					if ((Compare_Julian - Lower_Ending_Julian) <= pCommon_Params->Get_Data_Match_Error_Seconds()) {
						(*puiEvent_Number) = ctPrevious_Timestamp;
						bSuccess = TRUE;
						}
					}
				}
			}
		}

	// Event number returned indexed from 1
	if (bSuccess)
		(*puiEvent_Number)++;
	else
		(*puiEvent_Number) = 0;

		
	return bSuccess;	

}


// Timestamps and location
BOOL RTReview::Event_Search(const char *pszLocation, const char *pszTimestamp, UINT *puiEvent_Number, BOOL *pbLocation_Failed, BOOL *pbTimestamp_Failed) {

	return (Event_Search((const char **)ppszLocs, (const char **)ppszStart_Stamps, (const char **)ppszEnd_Stamps, uiTotal_Rows_Num, pszLocation, pszTimestamp, puiEvent_Number, pbLocation_Failed, pbTimestamp_Failed));
	
}


// Timestamps and location
BOOL RTReview::Event_Search(const char **ppszLocations, const char **ppszStart_Timestamps, const char **ppszEnd_Timestamps, UINT uiTotal_Rows_Number, const char *pszLocation, const char *pszTimestamp, UINT *puiEvent_Number, BOOL *pbLocation_Failed, BOOL *pbTimestamp_Failed) {

	// NOTE: THIS CODE IS REPEATED IN OTHER EVENT_SEARCH ALGORITHM, ANY CHANGES HERE SHOULD BE MADE THERE TOO!!!!
	// This member function is written to be as fast as possible, therefore it dublicates the other event search with the addition of location

	BOOL bSuccess = FALSE;
	BOOL bBetween_Start_End_Found, bHigher_Starting_Found, bLower_Ending_Found;
	int ctPrevious_Timestamp;
	time_t Higher_Starting_Julian, Lower_Ending_Julian, Compare_Julian;
	UINT ctLocation_Event;
	BOOL bLocation_Found;


	if (pbLocation_Failed)
		(*pbLocation_Failed) = FALSE;
	if (pbTimestamp_Failed)
		(*pbTimestamp_Failed) = FALSE;


	// Y2K - No problems below, based on timestamp comparison (with 4 digit year) and Julian below is OK

	(*puiEvent_Number) = 0;
	if ((ppszLocations) && (pszLocation) && (ppszStart_Timestamps) && (ppszEnd_Timestamps) && (pszTimestamp)) {

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Step #1: First try to determine if the timestamp is between any starting and ending timestamp pair
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		bBetween_Start_End_Found = FALSE;
		// Step #1 - Search for timestamp based on strcmp which is greater then requested
		while ((!bBetween_Start_End_Found) && ((*puiEvent_Number) < uiTotal_Rows_Number)) {
			// Find start and end timestamp pair which is around or equal
			if ((ppszStart_Timestamps[(*puiEvent_Number)][0] != NULL) && // Just check start, end should have same nulls
				(strcmp(ppszStart_Timestamps[(*puiEvent_Number)], pszTimestamp) >= 0) &&
				(strcmp(ppszEnd_Timestamps[(*puiEvent_Number)], pszTimestamp) <= 0) &&
				// Verify that the locations are also the same
				(stricmp(ppszLocations[(*puiEvent_Number)], pszLocation) == 0)) {
				bBetween_Start_End_Found = TRUE;
				bSuccess = TRUE;
				}
			else
				(*puiEvent_Number)++;
			}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Step #2: If the timestamp is not between any starting and ending timestamp pair, try to look
		//    for the first pair which is within the allowed uncertainty
		//          If the timestamp is between a pair, then success and just bottom out
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (!bBetween_Start_End_Found) {

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Step #2A - Search for starting timestamp based on strcmp which is higher then requested
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Reset the event number since starting over
			(*puiEvent_Number) = 0;
			bHigher_Starting_Found = FALSE;
			while ((!bHigher_Starting_Found) && ((*puiEvent_Number) < uiTotal_Rows_Number)) {
				// Find my timestamp greater or equal then one sent
				if ((ppszStart_Timestamps[(*puiEvent_Number)][0] != NULL) &&
					(strcmp(ppszStart_Timestamps[(*puiEvent_Number)], pszTimestamp) >= 0) &&
					// Verify that the locations are also the same
					(stricmp(ppszLocations[(*puiEvent_Number)], pszLocation) == 0))
					bHigher_Starting_Found = TRUE;
				else
					(*puiEvent_Number)++;
				}

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Step #2B - Search for ending timestamp based on strcmp which is lower then requested
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (bHigher_Starting_Found)
				// Below used when need to find next lowest from highest
				ctPrevious_Timestamp = (int)(*puiEvent_Number) - 1; // Allows starting one below
			else 
				// Below used when a higher can not be found - need to check if last in allowed range
				ctPrevious_Timestamp = (int)uiTotal_Rows_Number - 1;
			bLower_Ending_Found = FALSE;
			// Perform the lower search
			while ((!bLower_Ending_Found) && (ctPrevious_Timestamp >= 0)) {
				if ((ppszEnd_Timestamps[ctPrevious_Timestamp][0] != NULL) &&
					// Verify that the locations are also the same
					(stricmp(ppszLocations[ctPrevious_Timestamp], pszLocation) == 0))
					bLower_Ending_Found = TRUE;
				else
					ctPrevious_Timestamp--;
				}

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Step #3 - Determine if there is an earlier event allowed within the tolerence or next event within
			//           the tolerence
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Both the previous ending timestamp and next starting timestamp was found
			// Determine which was closest and if it is within the allowed tolerence
			if ((bHigher_Starting_Found) && (bLower_Ending_Found)) {
				if ((Get_Timstamp_Julian(pszTimestamp, &Compare_Julian)) &&
					(Get_Timstamp_Julian(ppszEnd_Timestamps[ctPrevious_Timestamp], &Lower_Ending_Julian)) &&
					(Get_Timstamp_Julian(ppszStart_Timestamps[(*puiEvent_Number)], &Higher_Starting_Julian))) {

					// Find the closer one - NOTE: Start and End should never overlap
					if ((Compare_Julian - Lower_Ending_Julian) < (Higher_Starting_Julian - Compare_Julian)) {
						// Verify that lower is within allowed range
						if ((Compare_Julian - Lower_Ending_Julian) <= pCommon_Params->Get_Data_Match_Error_Seconds()) {
							(*puiEvent_Number) = ctPrevious_Timestamp;
							bSuccess = TRUE;
							}
						}
						// Verify that higher is within allowed range
					else if ((Higher_Starting_Julian - Compare_Julian) <= pCommon_Params->Get_Data_Match_Error_Seconds())
						bSuccess = TRUE;

					}

				}

			// Only the next event was found, determine if it is within the tolerence
			else if (bHigher_Starting_Found) {
				if ((Get_Timstamp_Julian(pszTimestamp, &Compare_Julian)) &&
					(Get_Timstamp_Julian(ppszStart_Timestamps[(*puiEvent_Number)], &Higher_Starting_Julian))) {

					if ((Higher_Starting_Julian - Compare_Julian) <= pCommon_Params->Get_Data_Match_Error_Seconds())
						bSuccess = TRUE;

					}
				}

			// Only the previous event was found, determine if it is within the tolerence
			else if (bLower_Ending_Found) {  // If no data then a lower not found!
				if ((Get_Timstamp_Julian(pszTimestamp, &Compare_Julian)) &&
					(Get_Timstamp_Julian(ppszEnd_Timestamps[ctPrevious_Timestamp], &Lower_Ending_Julian))) {

					if ((Compare_Julian - Lower_Ending_Julian) <= pCommon_Params->Get_Data_Match_Error_Seconds()) {
						(*puiEvent_Number) = ctPrevious_Timestamp;
						bSuccess = TRUE;
						}
					}
				}
			// Neither a higher or lower was found, was not succesful
			// If requested for a response to see if failed because of location or timestamp, check to see if location even exists
			else if (pbLocation_Failed) {
				ctLocation_Event = 0;
				bLocation_Found = FALSE;
				while ((!bLocation_Found) && (ctLocation_Event < uiTotal_Rows_Number)) {
					if (stricmp(ppszLocations[ctLocation_Event], pszLocation) == 0)
						bLocation_Found = TRUE;
					ctLocation_Event++;
					}
				if (bLocation_Found) {
					(*pbLocation_Failed) = FALSE;
					// Must have then been because of the timestamp
					if (pbTimestamp_Failed)
						(*pbTimestamp_Failed) = TRUE;
					}
				else
					(*pbLocation_Failed) = TRUE;
				}
			}
		}

	// Event number returned indexed from 1
	if (bSuccess)
		(*puiEvent_Number)++;
	else
		(*puiEvent_Number) = 0;

		
	return bSuccess;	

}


BOOL RTReview::Get_Timstamp_Julian(const char *pszTimestamp, time_t *pJulian_Time) {

	tm Julian_Struct;
	UINT uiMillisecond;
	char szLocation[23];

	
	// Y2K - No problems below

	if ((pszTimestamp) && (pJulian_Time)) {

		if (8 == sscanf(pszTimestamp, "%05s%04u%02u%02u%02u%02u%02u%03u",
						szLocation,
						&(Julian_Struct.tm_year), &(Julian_Struct.tm_mon), &(Julian_Struct.tm_mday),
						&(Julian_Struct.tm_hour), &(Julian_Struct.tm_min), &(Julian_Struct.tm_sec), &uiMillisecond)) {

			// Month is indexed from 0 for mktime
			Julian_Struct.tm_mon--;
			Julian_Struct.tm_isdst = -1;
			Julian_Struct.tm_year -= 1900;
			if (-1 != ((*pJulian_Time) = mktime(&Julian_Struct)))
				return TRUE;
			}
		}


	return FALSE;

}

	
char *RTReview::RT_Review_Open_New_DB(const char **pszDefault_Types, const char *pszDefault_DB_Name, BOOL bOnly_From_Common_DB_Path_Warning, BOOL bOnly_From_Common_DB_Path) {

	BOOL bSelection_Completed, bCanceled;
	CGStr Chosen_File, Box_Title, Box_Default_Filename;
	char szFull_Common_Database_Path[201];
	char *pszExec_Name, *pszDB_Filename;


	// Need the full common database path name for proper conparison later
	if (!GetFullPathName(pCommon_Params->Get_Common_Database_Path(), 200, szFull_Common_Database_Path, &pszExec_Name)) {
		Set_Messages_On_Hold(TRUE);
		GUI_MsgBox("The common database directory is invalid.", GUI_ICON_ERROR);
		Set_Messages_On_Hold(FALSE);
		return NULL;
		}


	Set_Messages_On_Hold(TRUE);


	if (bOnly_From_Common_DB_Path_Warning)
		GUI_MsgBox("In order to keep data consistent with the other review tools, data can only be opened from the common database (current) directory.", GUI_ICON_WARN);


	// Create the dialog box title
    Box_Title += pCommon_Params->Get_My_Name();
    Box_Title += " - Open Database";
	Box_Default_Filename = szFull_Common_Database_Path;
	Box_Default_Filename += pszDefault_DB_Name;


	bSelection_Completed = FALSE;
	bCanceled = FALSE;
    while ((!bSelection_Completed) && (!bCanceled)) {
		if ((GUI_FileOpen(NULL, Box_Default_Filename.Get_sz(), &Chosen_File, pszDefault_Types, Box_Title)) &&
			(*(Chosen_File.Get_sz()) != NULL)) {
			// Be sure that user has not changed out of current directory
			if (bOnly_From_Common_DB_Path) {
				if (strnicmp(szFull_Common_Database_Path, Chosen_File.Get_sz(), strlen(szFull_Common_Database_Path)) == 0)
					bSelection_Completed = TRUE;
				else GUI_MsgBox("The selected database is invalid.  In order to keep data consistent with the other review tools, data can only be opened from the common database (current) directory: ", GUI_ICON_WARN);
				}
			else bSelection_Completed = TRUE;
			}
		else bCanceled = TRUE;
		}


	Set_Messages_On_Hold(FALSE);


	if ((!bCanceled) && (bSelection_Completed)) {
		pszDB_Filename = NULL;
		if (Chosen_File.Create_sz(&pszDB_Filename))
			return pszDB_Filename;
		else return NULL;
		}
	else return NULL;
	
}


char *RTReview::RT_Review_SaveAs_DB(const char **pszDefault_Types, const char *pszDefault_DB_Name, BOOL bOnly_From_Common_DB_Path_Warning, BOOL bOnly_From_Common_DB_Path) {

	BOOL bSelection_Completed, bCanceled;
	CGStr Chosen_File, Box_Title, Box_Default_Filename;
	char szFull_Common_Database_Path[201];
	char *pszExec_Name, *pszDB_Filename;


	// Need the full common database path name for proper conparison later
	if (!GetFullPathName(pCommon_Params->Get_Common_Database_Path(), 200, szFull_Common_Database_Path, &pszExec_Name)) {
		Set_Messages_On_Hold(TRUE);
		GUI_MsgBox("The common database directory is invalid.", GUI_ICON_ERROR);
		Set_Messages_On_Hold(FALSE);
		return NULL;
		}


	Set_Messages_On_Hold(TRUE);


	if (bOnly_From_Common_DB_Path_Warning)
		GUI_MsgBox("In order to keep data consistent with the other review tools, data can only be saved to the common database (current) directory.", GUI_ICON_WARN);


	// Create the dialog box title
    Box_Title += pCommon_Params->Get_My_Name();
    Box_Title += " - Save Database As";
	Box_Default_Filename = szFull_Common_Database_Path;
	Box_Default_Filename += pszDefault_DB_Name;


	bSelection_Completed = FALSE;
	bCanceled = FALSE;
    while ((!bSelection_Completed) && (!bCanceled)) {
		if ((GUI_FileSaveAs(NULL, Box_Default_Filename.Get_sz(), &Chosen_File, pszDefault_Types, Box_Title)) &&
			(*(Chosen_File.Get_sz()) != NULL)) {
			// Be sure that user has not changed out of current directory
			if (bOnly_From_Common_DB_Path) {
				if (strnicmp(szFull_Common_Database_Path, Chosen_File.Get_sz(), strlen(szFull_Common_Database_Path)) == 0)
					bSelection_Completed = TRUE;
				else GUI_MsgBox("The selected database is invalid.  In order to keep data consistent with the other review tools, data can only be saved to the common database (current) directory.", GUI_ICON_WARN);
				}
			else bSelection_Completed = TRUE;
			}
		else bCanceled = TRUE;
		}


	Set_Messages_On_Hold(FALSE);


	if ((!bCanceled) && (bSelection_Completed)) {
		pszDB_Filename = NULL;
		if (Chosen_File.Create_sz(&pszDB_Filename))
			return pszDB_Filename;
		else return NULL;
		}
	else return NULL;
		
}


char *RTReview::RT_Review_SaveCopyAs_DB(const char **pszDefault_Types, const char *pszDefault_DB_Name, BOOL bOnly_From_Common_DB_Path_Warning, BOOL bOnly_From_Common_DB_Path) {

	BOOL bSelection_Completed, bCanceled;
	CGStr Chosen_File, Box_Title, Box_Default_Filename;
	char szFull_Common_Database_Path[201];
	char *pszExec_Name, *pszDB_Filename;


	// Need the full common database path name for proper conparison later
	if (!GetFullPathName(pCommon_Params->Get_Common_Database_Path(), 200, szFull_Common_Database_Path, &pszExec_Name)) {
		Set_Messages_On_Hold(TRUE);
		GUI_MsgBox("The common database directory is invalid.", GUI_ICON_ERROR);
		Set_Messages_On_Hold(FALSE);
		return NULL;
		}


	Set_Messages_On_Hold(TRUE);


	if (bOnly_From_Common_DB_Path_Warning)
		GUI_MsgBox("In order to keep data consistent with the other review tools, data can only be saved to the common database (current) directory.", GUI_ICON_WARN);


	// Create the dialog box title
    Box_Title += pCommon_Params->Get_My_Name();
    Box_Title += " - Save Database Copy";
	Box_Default_Filename = szFull_Common_Database_Path;
	Box_Default_Filename += pszDefault_DB_Name;


	bSelection_Completed = FALSE;
	bCanceled = FALSE;
    while ((!bSelection_Completed) && (!bCanceled)) {
		if ((GUI_FileSaveAs(NULL, Box_Default_Filename.Get_sz(), &Chosen_File, pszDefault_Types, Box_Title)) &&
			(*(Chosen_File.Get_sz()) != NULL)) {
			// Be sure that user has not changed out of current directory
			if (bOnly_From_Common_DB_Path) {
				if (strnicmp(szFull_Common_Database_Path, Chosen_File.Get_sz(), strlen(szFull_Common_Database_Path)) == 0)
					bSelection_Completed = TRUE;
				else GUI_MsgBox("The selected database is invalid.  In order to keep data consistent with the other review tools, data can only be saved to the common database (current) directory.", GUI_ICON_WARN);
				}
			else bSelection_Completed = TRUE;
			}
		else bCanceled = TRUE;
		}


	Set_Messages_On_Hold(FALSE);


	if ((!bCanceled) && (bSelection_Completed)) {
		pszDB_Filename = NULL;
		if (Chosen_File.Create_sz(&pszDB_Filename))
			return pszDB_Filename;
		else return NULL;
		}
	else return NULL;

}


//////////////////////////////////////////////////////////////////////////////////////////
// Use the default error messages below except when an error occurs specific to your tool
//////////////////////////////////////////////////////////////////////////////////////////

void RTReview::RT_Review_Error(uiRT_REVIEW_ERROR uiError, const char *pszError_Parameter_1, const char *pszError_Parameter_2) {

	CGStr Message;

	
	#ifdef _WIN32
		if (pDlg)
			SetForegroundWindow(pDlg->GetWndHandle());
	#endif	

	switch(uiError) {
		case uiRT_REVIEW_ERROR_NO_MEMORY :
			RTError.Error(uiRT_ERROR_NO_MEMORY);
			break;
		case uiRT_REVIEW_ERROR_PIPE :
			RTError.Error(uiRT_ERROR_PIPE, pszError_Parameter_1);
			break;
		case uiRT_REVIEW_ERROR_DB_OPEN :
			Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_1) {
				Message = "An error occurred while trying to read the database: \"";
				Message += pszError_Parameter_1;
				Message += "\".\n\n";
				}
			else Message = "An error occurred while trying to read the database.  ";
			Message += "This could be due to:\n";
			Message += "1. The file is either not a valid database, is corrupted, or could not be found.\n"; 
			Message += "2. The cancel button was pressed.\n";
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_REVIEW_ERROR_DB_CLOSED :                                                      
			Set_Messages_On_Hold(TRUE);
			GUI_MsgBox("A valid database must be opened first.", GUI_ICON_WARN);
			Set_Messages_On_Hold(FALSE);
			break;         		   			
		case uiRT_REVIEW_ERROR_DB_CANT_SAVE :
			RTError.Error(uiRT_ERROR_DB_CANT_SAVE, pszError_Parameter_1);
			break;
		case uiRT_REVIEW_ERROR_DB_WRITE_PROTECTED :                                                      
			Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_1) {
				Message = "The database file: \"";
				Message += pszError_Parameter_1;
				Message += "\" is write protected and can not be overwritten.";
				}
			else Message = "The file is write protected and can not be overwritten.";
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_REVIEW_ERROR_NO_MATCHING_LOCATION :
			Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_1)
				Message = pszError_Parameter_1;
			else
				Message = "The review tool";
			Message += " does not have any data available at ";
			if ((pszError_Parameter_2) && (*pszError_Parameter_2 != NULL))
				Message += pszError_Parameter_2;
			else
				Message += "the current location";
			Message += ".";
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_REVIEW_ERROR_NO_MATCHING_DATE_AND_TIME :
			Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_1)
				Message = pszError_Parameter_1;
			else
				Message = "The review tool";
			Message += " does not have any matching data at ";
			if ((pszError_Parameter_2) && (*pszError_Parameter_2 != NULL)) {
				// 23 == strlen("XXXX.XX.XX XX:XX:XX.XXX")
				GEN_DATE_STRUCT Temp_Date;
				GEN_TIME_STRUCT Temp_Time;
				char szTempFormatedDateTime[25];
				if ((7 == sscanf(&(pszError_Parameter_2[uiTIMESTAMP_LOCATION_LENGTH]), "%04d%02d%02d%02d%02d%02d%03d%",
									&Temp_Date.yr, &Temp_Date.mon, &Temp_Date.day,
									&Temp_Time.hr, &Temp_Time.min, &Temp_Time.sec, &Temp_Time.msec)) &&
					(20 <= sprintf(szTempFormatedDateTime, "%04u.%u.%u %u:%02u:%02u.%03u",
							(Temp_Date.yr % 10000), (Temp_Date.mon % 100), (Temp_Date.day % 100), (Temp_Time.hr % 100), (Temp_Time.min % 100), (Temp_Time.sec % 100), (Temp_Time.msec % 1000))))
					Message += szTempFormatedDateTime;
				else Message += "the selected date and time";
				}
			else
				Message += "the selected date and time";
			Message += ".";
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_REVIEW_ERROR_NO_MATCHING_DATA_TOOL_START :
			Set_Messages_On_Hold(TRUE);
			Message = "Matching data could not be found.\n\n";
			Message += pCommon_Params->Get_My_Name();
			Message += " was starting during the requested data match and may not have been able to match data yet.  Please try the data match again.";
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_REVIEW_ERROR_INITIATE_REANALYZE_FAILED :
			Set_Messages_On_Hold(TRUE);
			GUI_MsgBox("A reanalyzation of all data could not be initiated!  Data and parameters may now be inconsistent between the review tools.", GUI_ICON_ERROR);
			Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_REVIEW_ERROR_BUSY :
			Set_Messages_On_Hold(TRUE);
			Message = pszError_Parameter_1;
			Message += " is busy right now.  It is performing an operation that does not allow data to be matched to it at this time.";
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_REVIEW_ERROR_BUSY_EXIT :
			Set_Messages_On_Hold(TRUE);
			Message = pszError_Parameter_1;
			Message += " is busy right now.  You need to exit it manually";
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_REVIEW_ERROR_CANT_MATCH :
			Set_Messages_On_Hold(TRUE);
			Message = pszError_Parameter_1;
			Message += " does not have the ability to display matching data.";
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			Set_Messages_On_Hold(FALSE);
			break;
		default:
			RTError.Error(uiError, pszError_Parameter_1, pszError_Parameter_2);
			break;			
    	}    

}
		
