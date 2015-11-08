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
