// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/10/97

// Rev_Tool.CPP
// Main source file for the abstract class Review Tool


#include "RTError.H"   



/////////////////////////////////////////////////////////////////////////////////////
//  Review_Tool_Error constructor:  Setup all intial parameters, and the pipe
/////////////////////////////////////////////////////////////////////////////////////

Review_Tool_Error::Review_Tool_Error(const char *pszMy_Name, RTPipes *pRTPipes) :
	pszName(NULL),
	pPipes(pRTPipes)
	{
	
	if ((pszName = new char [strlen(pszMy_Name) + 1]) != NULL)
		strcpy(pszName, pszMy_Name);
	else
	    Error(uiRT_ERROR_NO_MEMORY);
	    
}

	
Review_Tool_Error::Review_Tool_Error(void) :
	pszName(NULL),
	pPipes(NULL)
	{}

	
Review_Tool_Error::~Review_Tool_Error(void) {

	if (pszName)
		delete [] pszName;
		
}
    
    
/////////////////////////////////////////////////////////////////////////////////////
//  Setting member functions
/////////////////////////////////////////////////////////////////////////////////////

BOOL Review_Tool_Error::Set_My_Name(const char *pszMy_Name) {

	if (pszName) delete [] pszName;
	if ((pszName = new char [strlen(pszMy_Name) + 1]) != NULL) {
		strcpy(pszName, pszMy_Name);
		return TRUE;
		}
	else
	    Error(uiRT_ERROR_NO_MEMORY);
	    
	    
	return FALSE;
	
}


BOOL Review_Tool_Error::Set_My_Pipes(RTPipes *pRT_Pipes) {

	pPipes = pRT_Pipes;

	    
	return TRUE;
	
}


/////////////////////////////////////////////////////////////////////////////////////
//  Error handling functions
/////////////////////////////////////////////////////////////////////////////////////

void Review_Tool_Error::Error(uiRT_ERROR uiError, const char *pszError_Parameter_1, const char *pszError_Parameter_2) {

	CGStr Message;
	
	
	switch(uiError) { 
		case uiRT_ERROR_NO_MEMORY :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			GUI_MsgBox("Your system has run out of free memory.  Please try to increase the available memory and then begin again.", GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_NOT_INSTALLED:
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_1 == NULL)
				Message += "The review tool";
			else
				Message += pszError_Parameter_1;
			Message += " is not installed on this system.";
			GUI_MsgBox(Message, GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_INIT_LOAD :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			Message = "";
			if (pszError_Parameter_2) {
				Message += "An error occurred while trying to load the initialization file.  However, application execution was able to continue.";
				Message += "\n\nParameter: \"";
				Message += pszError_Parameter_2;
				Message += "\"";
				if (pszError_Parameter_2)
					Message += "\n\n";
				}
			if (pszError_Parameter_1) {
				Message += "An error occurred while trying to load the initialization file.  Application execution was not able to continue.";
				Message += "\n\nParameter: \"";
				Message += pszError_Parameter_1;
				Message += "\"";
				}
			if (!pszError_Parameter_1 && !pszError_Parameter_2) {
				Message += "An unknown error occurred while trying to load the initialization file.  Application execution was not able to continue.";
				}
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_CANT_START_TOOL:
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_1 == NULL)
				Message += "The review tool";
			else
				Message += pszError_Parameter_1;
			Message += " could not be found.  The software may have to be reinstalled.";
			GUI_MsgBox(Message, GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_PIPE_OPEN_FAIL:
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_2 == NULL)
				Message += "Could not create a communication path to ";
			else {
				Message = pszError_Parameter_2;
				Message += " could not create a communication path to ";
				}
			if (pszError_Parameter_1 == NULL)
				Message += "one of the tools";
			else
				Message += pszError_Parameter_1;
			Message += " and will not be able to send information to it.";
			GUI_MsgBox(Message, GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_PIPE :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			Message = "An error occurred while trying to communicate with ";
			if (pszError_Parameter_1 == NULL)
				Message += "one of the tools";
			else {
				Message += pszError_Parameter_1;
				Message += ".";
				}
			GUI_MsgBox(Message, GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_PIPE_DEAD :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			Message = "An unrecoverable error occurred while sending a message to ";
			if (pszError_Parameter_1 == NULL)
				Message += "one of the tools";
			else {
				Message += pszError_Parameter_1;
				Message += ".";
				}
			GUI_MsgBox(Message, GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_OPTIONS_SAVE_FAIL :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			GUI_MsgBox("An error occurred while trying to save the default parameters.", GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_OPTIONS_RESET_FAIL :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			GUI_MsgBox("An error occurred while trying to reset the parameters.", GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_DB_CANT_SAVE :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_1) {
				Message = "An error occurred while trying to save the database: \"";
				Message += pszError_Parameter_1;
				Message += "\".  ";
				}
			else Message = "An error occurred while trying to save the database.  ";
			Message += "The database could not be saved.";
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_WARN);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_INSPEC_CHANGE :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			GUI_MsgBox("An error occurred while trying to change the inspection information.", GUI_ICON_WARN);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_OVERWRITE_RAWDATA :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			GUI_MsgBox("You can not overwrite the raw data.", GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_COMMON_INSPEC_LOAD :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			Message = "An error occurred while loading the common inspection information.";
			if (pszError_Parameter_1) {
				Message += "\n\nParameter: \"";
				Message += pszError_Parameter_1;
				Message += "\"";
				}
			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_COMMON_INSPEC_SAVE :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			GUI_MsgBox("An error occurred while saving the common inspection information.", GUI_ICON_ERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		default: 
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			GUI_MsgBox("An unrecognizable error has occurred.", GUI_ICON_WARN);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;			
    	}

}
