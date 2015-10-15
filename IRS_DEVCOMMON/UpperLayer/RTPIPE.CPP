// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/11/97

// RTPipe.CPP
// Main source file for the generic Review Tool Pipe class


#include "RTPipe.H"   


///////////////////////////////////////////////
// Review Tool Pipe constructor and destructor 
///////////////////////////////////////////////

RTPipes::RTPipes(RTPipe_Notify& rReview_Tool, const char *pszPipe_My_Name) :
	rTool(rReview_Tool),
	pszMy_Name(NULL),
	uiPipes_Num(0),
	pPipe_Info(NULL),
	pPipe_Info_End(NULL),
	bHold_Status(FALSE),
	uiMessage_Holds(0),
	bStarting(TRUE),
	bMessage_Waiting(FALSE),
	ppszMessage_Queue(NULL)
	{

	if ((pszMy_Name = new char [strlen(pszPipe_My_Name) + 1]) != NULL) {
		strcpy(pszMy_Name, pszPipe_My_Name);
		}
		
}

 
RTPipes::~RTPipes(void) {

	UINT ctIndex;
	PIPE_INFO *pCurrent = pPipe_Info, *pNext;
	

	if (pPipe_Info) {
		for (ctIndex = 0; ctIndex < uiPipes_Num; ctIndex++) {
			if (pCurrent->pszMy_Topics)
				delete [] pCurrent->pszMy_Topics;
			if (pCurrent->pszOther_Name)
				delete [] pCurrent->pszOther_Name;
			if (pCurrent->pszOther_Topic)
				delete [] pCurrent->pszOther_Topic;
			if (pCurrent->bConnected)
				pCurrent->pPipe->Close_Communications();
			if (pCurrent->pPipe)
				delete pCurrent->pPipe;
			pNext = pCurrent->pNext;
			delete pCurrent;	
			pCurrent = pNext;
			}
		}
	if (ppszMessage_Queue) {
		if (ppszMessage_Queue[0]) {
			delete [] ppszMessage_Queue[0];
			ppszMessage_Queue[0] = NULL;
			}
		if (ppszMessage_Queue[1]) {
			delete [] ppszMessage_Queue[1];
			ppszMessage_Queue[1] = NULL;
			}
		delete [] ppszMessage_Queue;
		ppszMessage_Queue = NULL;
		}
	if (pszMy_Name) {
		delete [] pszMy_Name;
		pszMy_Name = NULL;
		}

}
                                             

//////////////////////////////////////////////
// Review Tool Pipe creation member functions 
//////////////////////////////////////////////

BOOL RTPipes::Create(const char *pszPipe_Other_Name, UINT uiPipe_Timeout) {

	UINT uiTopic_Length = 0;
	
	
	// Calculate the topic length
	uiTopic_Length += strlen(pszMy_Name);
	uiTopic_Length += strlen("_TO_");
	uiTopic_Length += strlen(pszPipe_Other_Name);
	uiTopic_Length += 1;
	
	// If first then create first
	if (uiPipes_Num == 0) {
		if ((pPipe_Info = new (PIPE_INFO)) == NULL)
			return FALSE;
		pPipe_Info_End = pPipe_Info;
		}
	// If not first then create off of last
	else {
		if ((pPipe_Info_End->pNext = new (PIPE_INFO)) == NULL)
			return FALSE;
		pPipe_Info_End = pPipe_Info_End->pNext;
		}
		
	// Fill the new structure with info
	if (((pPipe_Info_End->pszMy_Topics = new char [uiTopic_Length]) == NULL) ||
		((pPipe_Info_End->pszOther_Name = new char [strlen(pszPipe_Other_Name) + 1]) == NULL) ||
		((pPipe_Info_End->pszOther_Topic = new char [uiTopic_Length]) == NULL)) {
		if (pPipe_Info_End->pszMy_Topics)
			delete [] pPipe_Info_End->pszMy_Topics;
		if (pPipe_Info_End->pszOther_Name)
			delete [] pPipe_Info_End->pszOther_Name;
		if (pPipe_Info_End->pszOther_Topic)
			delete [] pPipe_Info_End->pszOther_Topic;
		return FALSE;
		}
		
	strcpy(pPipe_Info_End->pszMy_Topics, pszPipe_Other_Name);
	strcat(pPipe_Info_End->pszMy_Topics, "_TO_");
	strcat(pPipe_Info_End->pszMy_Topics, pszMy_Name);

	strcpy(pPipe_Info_End->pszOther_Name, pszPipe_Other_Name);
		
	strcpy(pPipe_Info_End->pszOther_Topic, pszMy_Name);
	strcat(pPipe_Info_End->pszOther_Topic, "_TO_");
	strcat(pPipe_Info_End->pszOther_Topic, pszPipe_Other_Name);
	
	pPipe_Info_End->bConnected = FALSE;

	if ((pPipe_Info_End->pPipe = new Pipe(pszMy_Name,
										  pPipe_Info_End->pszMy_Topics,
										  pPipe_Info_End->pszOther_Name,
										  pPipe_Info_End->pszOther_Topic,
										  (Pipe_CallBack *)((void *)this),
										  (Pipe_CB_StringFn)&RTPipes::Receive_String,
										  (Pipe_CB_CommandFn)&RTPipes::Receive_Command)) == NULL)
		return FALSE;

	pPipe_Info_End->pPipe->Set_Timeout(uiPipe_Timeout);
	
	uiPipes_Num++;
	
	
	return (pPipe_Info_End->pPipe->Pipe_Created());

}

////////////////////////////////////////////////
// Review Tool Pipe connection member functions 
////////////////////////////////////////////////

BOOL RTPipes::Connect(const char *pszPipe_Other_Name) {

	PIPE_INFO *pCurrent;

	
	if (pCurrent = Find_Info(pszPipe_Other_Name))
		if (pCurrent->pPipe->Pipe_Created())
        	return (pCurrent->bConnected = pCurrent->pPipe->Open_Communications());
	    

	return FALSE;
	
}


BOOL RTPipes::Connected(const char *pszPipe_Other_Name) {

	PIPE_INFO *pCurrent;

	
	if (pCurrent = Find_Info(pszPipe_Other_Name))
       	return (pCurrent->bConnected);
	    

	return FALSE;

}


BOOL RTPipes::Close(const char *pszPipe_Other_Name) {

	PIPE_INFO *pCurrent;

	
	if (pCurrent = Find_Info(pszPipe_Other_Name))
		if (pCurrent->pPipe->Pipe_Created()) {
			pCurrent->bConnected = FALSE;
        	return (pCurrent->pPipe->Close_Communications());
			}
	    

	return FALSE;

}


// Call this member function to put incoming displaying messages on hold when you are busy 
//    during long operations (Example - loading and saving)
BOOL RTPipes::Set_Messages_On_Hold(BOOL bStatus) {

	if (bStatus) {
		uiMessage_Holds++;
		bHold_Status = TRUE;
		}
	else if (uiMessage_Holds > 0) {
		uiMessage_Holds--;
		if (uiMessage_Holds == 0)
			bHold_Status = FALSE;
		}

    if ((!bHold_Status) && (bStarting)) {
    	bStarting = FALSE;
	    if (bMessage_Waiting) {
	    	bMessage_Waiting = FALSE;
			// The queue should exist at this point and the first one should exist at this point
			if ((!ppszMessage_Queue) && (!ppszMessage_Queue[0]))
				return FALSE; 
			if (!Message_Queue_Process_Message(ppszMessage_Queue[0]))
				return FALSE;
			// The second one may not exist at this point
			if (ppszMessage_Queue[1])
				if (!Message_Queue_Process_Message(ppszMessage_Queue[1]))
					return FALSE;
			}
		}


	return TRUE;

}


/////////////////////////////////////////////
// Review Tool Pipe sending member functions
/////////////////////////////////////////////

RTPipes::RTPIPE_RESULT RTPipes::Send(const char *pszPipe_Other_Name, char **pszString_Receive, const char *pszString_Send, ...) {

	va_list Variable_List;
	const char *pszCurrent_String;
	char *pszSend = NULL;
	UINT uiString_Length;
	PIPE_INFO *pCurrent;
	BOOL bSuccess = FALSE;
	
	
	// Be sure valid string being sent
	if (pszString_Send == NULL)
		return (RTPIPE_RESULT::FAIL);
		

	// Get the pipe info
	if (!(pCurrent = Find_Info(pszPipe_Other_Name)))
		return (RTPIPE_RESULT::FAIL);
		
	// Check to be sure pipe is open
	if (!pCurrent->bConnected)
		// Not chosen to start
		if (!rTool.RT_Pipe_Not_Open(pszPipe_Other_Name))
			return (RTPIPE_RESULT::CANCEL);



	// Calculate the length of the string
	va_start(Variable_List, pszString_Send);
	uiString_Length = strlen(pszString_Send);
	pszCurrent_String = va_arg(Variable_List, const char *);
	while (*pszCurrent_String != NULL) {
		uiString_Length += strlen(pszCurrent_String);
		pszCurrent_String = va_arg(Variable_List, const char *);
		}

	// Build the string to be sent
	if ((pszSend = new char [uiString_Length + 1]) == NULL)
		return (RTPIPE_RESULT::FAIL);

	va_start(Variable_List, pszString_Send);
	strcpy(pszSend, pszString_Send);
	pszCurrent_String = va_arg(Variable_List, const char *);
	while (*pszCurrent_String != NULL) {
		strcat(pszSend, pszCurrent_String);
		pszCurrent_String = va_arg(Variable_List, const char *);
		}
    va_end(Variable_List);
	

	// Send the string			
   	bSuccess = pCurrent->pPipe->Send(pszSend, pszString_Receive);

	
	// Receive value checked by caller
	

    if (pszSend)
    	delete [] pszSend;
	    

	if (bSuccess)
		return (RTPIPE_RESULT::SUCCESS);
	else
		return (RTPIPE_RESULT::FAIL);
   
}


RTPipes::RTPIPE_RESULT RTPipes::Send(const char *pszPipe_Other_Name, BOOL bVerify_Response, const char *pszCommand_Send, ...) {

	va_list Variable_List;
	const char *pszCurrent_String;
	char *pszSend = NULL;
	UINT uiString_Length;
	PIPE_INFO *pCurrent;
	BOOL bSuccess = FALSE;

	
	// Calculate the length of the string
	va_start(Variable_List, pszCommand_Send);
	uiString_Length = strlen(pszCommand_Send);
	pszCurrent_String = va_arg(Variable_List, const char *);
	while (*pszCurrent_String != NULL) {
		uiString_Length += strlen(pszCurrent_String);
		pszCurrent_String = va_arg(Variable_List, const char *);
		}

	// Build the string to be sent
	if ((pszSend = new char [uiString_Length + 1]) == NULL)
		return (RTPipes::FAIL);

	va_start(Variable_List, pszCommand_Send);
	strcpy(pszSend, pszCommand_Send);
	pszCurrent_String = va_arg(Variable_List, const char *);
	while (*pszCurrent_String != NULL) {
		strcat(pszSend, pszCurrent_String);
		pszCurrent_String = va_arg(Variable_List, const char *);
		}
    va_end(Variable_List);


	if (pCurrent = Find_Info(pszPipe_Other_Name))
		if (pCurrent) {
			if (pCurrent->bConnected)
        		bSuccess = pCurrent->pPipe->Send(pszSend, bVerify_Response);
        	}
        	


    if (pszSend)
    	delete [] pszSend;
	    

	if (bSuccess)
		return (RTPipes::SUCCESS);
	else
		return (RTPipes::FAIL);
   
}


/////////////////////////////////////////
//  Messaging receiving member functions
/////////////////////////////////////////

BOOL RTPipes::Receive_Command(char *pszCommand) {
    
	PIPE_INFO *pCurrent;

	
	if (strncmp(pszCommand, szPIPE_STARTED, strlen(szPIPE_STARTED)) == 0) {
		if (pCurrent = Find_Info(&(pszCommand[strlen(szPIPE_STARTED)]))) {
			if (pCurrent->pPipe->Open_Communications()) {
				pCurrent->bConnected = TRUE;
				}
			}
		return TRUE;
		}
	else if (strncmp(pszCommand, szPIPE_SETUP_DONE, strlen(szPIPE_SETUP_DONE)) == 0) {
		// Notify that tool is done with setup
		return (rTool.RT_Setup_Done(&(pszCommand[strlen(szPIPE_SETUP_DONE)])));
		}
	else if (strncmp(pszCommand, szPIPE_IMPORT_DONE, strlen(szPIPE_IMPORT_DONE)) == 0) {
		// Notify that tool is done with import
		return (rTool.RT_Import_Done(&(pszCommand[strlen(szPIPE_IMPORT_DONE)])));
		}
	else if (strncmp(pszCommand, szPIPE_REANALYZE_DONE, strlen(szPIPE_REANALYZE_DONE)) == 0) {
		// Notify that tool is done with reanalyze
		return (rTool.RT_Reanalyze_Done(&(pszCommand[strlen(szPIPE_REANALYZE_DONE)])));
		}
	else if (strncmp(pszCommand, szPIPE_EXITED, strlen(szPIPE_EXITED)) == 0) {
		Close(&(pszCommand[strlen(szPIPE_EXITED)]));
		return TRUE;
		}
	else // Unknown command
		return FALSE;
		
}


BOOL RTPipes::Receive_String(char *pszRequest, char **ppszReply) {

	BOOL bSuccess = TRUE;
	char *pszPipe, *pszStart, *pszEnd;
	BOOL bLocation_Failed;


	//////////////////////////////////////////////////////////////////////////////////////////
	// THIS CODE IS REPRODUCED IN Message_Queue_Process_Message, if change here change there
	//////////////////////////////////////////////////////////////////////////////////////////

    // Display data
	if ((strncmp(pszRequest, szPIPE_DATA, strlen(szPIPE_DATA)) == 0) &&
		(strlen(&((strchr(pszRequest, '+'))[1])) == uiTIME_STAMP_LENGTH)) {
		// If just starting then queue the first message	
		if (bStarting) {
			bMessage_Waiting = TRUE;
			if (!Message_Queue_Add_Message(pszRequest))
				// Either the message queue creation failed or it is full
				strcpy(pszRequest, "BUSY");
			(*ppszReply) = pszRequest;
        	return TRUE;
        	}
		// If holding then report busy
		else if (bHold_Status) {
			strcpy(pszRequest, "BUSY");
			(*ppszReply) = pszRequest;
        	return TRUE;
        	}
		// Display matching data
    	else {
   			(strchr(pszRequest, '+'))[0] = '\0'; // Prepare the request string to be passed
	    	bSuccess = rTool.RT_Display_Data(&(pszRequest[strlen(szPIPE_DATA)]), &(pszRequest[strlen(pszRequest)+1]), &bLocation_Failed );
    		pszRequest[strlen(pszRequest)] = '+'; // Fix the request string
			// if the location was what actually failed then reply back with location
			if ((!bSuccess) && (bLocation_Failed))
        		strcpy(pszRequest, "LOCATION");
			// Must have been the timestamp that failed
			else if ((!bSuccess) && (!bLocation_Failed))
        		strcpy(pszRequest, "TIMESTAMP");
			(*ppszReply) = pszRequest;
			return TRUE;
			}

		// Something failed
		(*ppszReply) = &(pszRequest[1]);
		return TRUE;
		}               
		
    // Set the location
	else if ((strncmp(pszRequest, szPIPE_LOCATION, strlen(szPIPE_LOCATION)) == 0) &&
		// Check that a location actually exists
		(strlen(&((strchr(pszRequest, '+'))[1])) > 0)) {
		// If just starting then queue the message
		if (bStarting) {
			bMessage_Waiting = TRUE;
			if (!Message_Queue_Add_Message(pszRequest))
				// Either the message queue creation failed or it is full
				strcpy(pszRequest, "BUSY");
			(*ppszReply) = pszRequest;
        	return TRUE;
        	}
		// If holding then report busy
		else if (bHold_Status) {
        	strcpy(pszRequest, "BUSY");
			(*ppszReply) = pszRequest;
        	return TRUE;
        	}
		// Set the location
    	else {
   			(strchr(pszRequest, '+'))[0] = '\0'; // Prepare the request string to be passed
	    	bSuccess = rTool.RT_Location_Change(&(pszRequest[strlen(szPIPE_LOCATION)]), &(pszRequest[strlen(pszRequest)+1]) );
    		pszRequest[strlen(pszRequest)] = '+'; // Fix the request string
			// If failed reply back with location
			if (!bSuccess)
        		strcpy(pszRequest, "LOCATION");
			(*ppszReply) = pszRequest;
			return TRUE;
			}

		// Something failed
		(*ppszReply) = &(pszRequest[1]);
		return TRUE;
		}               

	// Check for exit all command
	else if (strncmp(pszRequest, szPIPE_EXIT_ALL, strlen(szPIPE_EXIT_ALL)) == 0) {
		// Check to see if messages on hold, may not be able to shut down at this time
		if (bHold_Status) {
        	strcpy(pszRequest, "BUSY");
			(*ppszReply) = pszRequest;
        	return TRUE;
        	}
		else {
			// Shutdown everything
			rTool.RT_Receive_Exit(NULL);
			(*ppszReply) = pszRequest;
			return TRUE;
			}

		// Something failed
		(*ppszReply) = &(pszRequest[1]);
		return TRUE;
		}

	// Check for reanalyze command
	else if (strncmp(pszRequest, szPIPE_REANALYZE, strlen(szPIPE_REANALYZE)) == 0) {
		// If just starting then queue the message
		if (bStarting) {
			bMessage_Waiting = TRUE;
			if (!Message_Queue_Add_Message(pszRequest))
				// Either the message queue creation failed or it is full
				strcpy(pszRequest, "BUSY");
			(*ppszReply) = pszRequest;
        	return TRUE;
        	}
		// Check to see if messages on hold, may not be able to shut down at this time
		else if (bHold_Status) {
        	strcpy(pszRequest, "BUSY");
			(*ppszReply) = pszRequest;
        	return TRUE;
        	}
		else {
			// Perform a reanalyze
			rTool.RT_Reanalyze_All_Data();
			(*ppszReply) = pszRequest;
			return TRUE;
			}

		// Something failed
		(*ppszReply) = &(pszRequest[1]);
		return TRUE;
		}

	// Display data area
	else if ((strncmp(pszRequest, szPIPE_DATA_AREA, strlen(szPIPE_DATA_AREA)) == 0) &&
    		(strlen(&((strchr(pszRequest, '+'))[1])) == uiTIME_RANGE_LENGTH)) {
		// If just starting then queue the message
		if (bStarting) {
			bMessage_Waiting = TRUE;
			if (!Message_Queue_Add_Message(pszRequest))
				// Either the message queue creation failed or it is full
				strcpy(pszRequest, "BUSY");
			(*ppszReply) = pszRequest;
        	return TRUE;
        	}
		// If holding then report busy
		else if (bHold_Status) {
        	strcpy(pszRequest, "BUSY");
			(*ppszReply) = pszRequest;
        	return TRUE;
        	}
		// Display matching data
    	else {
			// Prepare the request string to be passed
			pszEnd = strchr(pszRequest, '-');
			pszEnd[0] = '\0';
    		pszStart = strchr(pszRequest, '+');
    		pszStart[0] = '\0';
    		pszPipe = &(pszRequest[strlen(szPIPE_DATA_AREA)]);
	    	bSuccess = rTool.RT_Display_Data_Area(pszPipe, &(pszStart[1]), &(pszEnd[1]), &bLocation_Failed );
			// Fix the request string
			*strchr(pszRequest, '\0') = '+';
			*strchr(pszRequest, '\0') = '-';
			// if the location was what actually failed then reply back with location
			if ((!bSuccess) && (bLocation_Failed))
        		strcpy(pszRequest, "LOCATION");
			// Must have been the timestamp that failed
			else if ((!bSuccess) && (!bLocation_Failed))
        		strcpy(pszRequest, "TIMESTAMP");
			(*ppszReply) = pszRequest;
			return TRUE;
			}

		// Something failed
		(*ppszReply) = &(pszRequest[1]);
		return TRUE;
		}               

	// Bad command sent
	else {
		(*ppszReply) = &(pszRequest[1]);
		return TRUE;
		}  

}


///////////////////////////////////////////////////////////////
// Review Tool Pipe posibble useful and helper member functions
///////////////////////////////////////////////////////////////

BOOL RTPipes::Check_Connected_To_Anyone(void) {

	BOOL bConnected = FALSE;
	PIPE_INFO *pCurrent;
	UINT ctIndex;


	if (pPipe_Info) {
		pCurrent = pPipe_Info;
		for (ctIndex = 0; ((ctIndex < uiPipes_Num) && !bConnected); ctIndex++) {
			bConnected = pCurrent->bConnected;
	       	pCurrent = pCurrent->pNext;
			}
		}


	return bConnected;

}


RTPipes::PIPE_INFO *RTPipes::Find_Info(const char *pszPipe_Other_Name) {

	UINT ctIndex;
	PIPE_INFO *pCurrent;


	if (pPipe_Info) {
		pCurrent = pPipe_Info;
		for (ctIndex = 0; ctIndex < uiPipes_Num; ctIndex++) {
			if (strcmp(pszPipe_Other_Name, pCurrent->pszOther_Name) == 0)
	        	return (pCurrent);
	        else
	        	pCurrent = pCurrent->pNext;
		    }
		}
	    
	return (PIPE_INFO *)NULL;

}


BOOL RTPipes::Message_Queue_Add_Message(const char *pszMessage) {

	// Create the queue if it does not exist
	if (!ppszMessage_Queue) {
		if ((ppszMessage_Queue = new (char (*[2]))) != NULL) {
			ppszMessage_Queue[0] = NULL;
			ppszMessage_Queue[1] = NULL;
			}
		}

	// Fill the queue if it exists
	if (ppszMessage_Queue) {
		if (!ppszMessage_Queue[0]) {
			if ((ppszMessage_Queue[0] = new char [strlen(pszMessage) + 1]) != NULL)
			    strcpy(ppszMessage_Queue[0], pszMessage);
			// Fail if a message queue could not be created
			else return FALSE;
			}
		else if (!ppszMessage_Queue[1]) {
			if ((ppszMessage_Queue[1] = new char [strlen(pszMessage) + 1]) != NULL)
			    strcpy(ppszMessage_Queue[1], pszMessage);
			// Fail if a message queue could not be created, just return BUSY
			else return FALSE;
			}
		// Fail if the message queue is full, just return BUSY
		else return FALSE;
		}
	// Fail if a message queue could not be created, just return BUSY
	else return FALSE;


	return TRUE;

}


BOOL RTPipes::Message_Queue_Process_Message(char *pszMessage) {

	BOOL bSuccess = TRUE;
	char *pszPipe, *pszStart, *pszEnd;
	BOOL bLocation_Failed;


	if (!pszMessage) return FALSE;


	///////////////////////////////////////////////////////////////////////////////////
	// THIS CODE IS REPRODUCED IN Receive_String, if change here change there
	///////////////////////////////////////////////////////////////////////////////////

    // Display data
	if ((strncmp(pszMessage, szPIPE_DATA, strlen(szPIPE_DATA)) == 0) &&
		(strlen(&((strchr(pszMessage, '+'))[1])) == uiTIME_STAMP_LENGTH)) {

   		(strchr(pszMessage, '+'))[0] = '\0'; // Prepare the request string to be passed
	    bSuccess = rTool.RT_Display_Data(&(pszMessage[strlen(szPIPE_DATA)]), &(pszMessage[strlen(pszMessage)+1]), &bLocation_Failed );

		if ((!bSuccess) || (bLocation_Failed))
			return FALSE;
		else return TRUE;
		}               
		
    // Set the location
	else if ((strncmp(pszMessage, szPIPE_LOCATION, strlen(szPIPE_LOCATION)) == 0) &&
		// Check that a location actually exists
		(strlen(&((strchr(pszMessage, '+'))[1])) > 0)) {

   		(strchr(pszMessage, '+'))[0] = '\0'; // Prepare the request string to be passed
	    return (rTool.RT_Location_Change(&(pszMessage[strlen(szPIPE_LOCATION)]), &(pszMessage[strlen(pszMessage)+1]) ));
		}

	// Check for reanalyze command
	else if (strncmp(pszMessage, szPIPE_REANALYZE, strlen(szPIPE_REANALYZE)) == 0) {
		// Perform a reanalyze
		return (rTool.RT_Reanalyze_All_Data());
		}

	// Display data area
	else if ((strncmp(pszMessage, szPIPE_DATA_AREA, strlen(szPIPE_DATA_AREA)) == 0) &&
    		(strlen(&((strchr(pszMessage, '+'))[1])) == uiTIME_RANGE_LENGTH)) {

		// Prepare the request string to be passed
		pszEnd = strchr(pszMessage, '-');
		pszEnd[0] = '\0';
    	pszStart = strchr(pszMessage, '+');
    	pszStart[0] = '\0';
    	pszPipe = &(pszMessage[strlen(szPIPE_DATA_AREA)]);
	    bSuccess = rTool.RT_Display_Data_Area(pszPipe, &(pszStart[1]), &(pszEnd[1]), &bLocation_Failed );

		if ((!bSuccess) || (bLocation_Failed))
			return FALSE;
		else return TRUE;
		}

	// Bad command sent
	else return FALSE;

}


