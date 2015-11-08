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
// 8/28/96

// Pipe.H
// Class definition for pipe/messaging objects
 

#include "Pipe.H"
 
Pipe_CallBack::Pipe_CallBack(){}
Pipe_CallBack::~Pipe_CallBack(){}
 
 
 
/////////////////////////////////////////////////////////////////////////////////////
//  Two way pipe constructors
/////////////////////////////////////////////////////////////////////////////////////
Pipe::Pipe(const char *pszMy_Name, const char *pszMy_Topic, const char *pszTheir_Name, const char *pszTheir_Topic,
				Pipe_CallBack *pCB_Obj,
				Pipe_CB_StringFn mpString_Function,
				Pipe_CB_CommandFn mpCommand_Function) :
	bCommand_Function(FALSE),
	bPipe_Created(FALSE),
	bPipe_Open(FALSE),
	Direction(BOTH),
	dPipe_Time_Out(0),
	pszPipe_My_Name(NULL),
	pszPipe_My_Topic(NULL),
	pszPipe_Their_Name(NULL),
	pszPipe_Their_Topic(NULL),
	pPipe_In(NULL),
	pPipe_Out(NULL)
	{

	pszPipe_My_Name = new char [strlen(pszMy_Name) + 1];
	pszPipe_My_Topic = new char [strlen(pszMy_Topic) + 1];
	pszPipe_Their_Name = new char [strlen(pszTheir_Name) + 1];
	pszPipe_Their_Topic = new char [strlen(pszTheir_Topic) + 1];
	if ((pszPipe_My_Name != NULL) && (pszPipe_My_Topic != NULL) && (pszPipe_Their_Name != NULL) && (pszPipe_Their_Topic != NULL)) {
		strcpy(pszPipe_My_Name, pszMy_Name);
		strcpy(pszPipe_My_Topic, pszMy_Topic);
		strcpy(pszPipe_Their_Name, pszTheir_Name);
		strcpy(pszPipe_Their_Topic, pszTheir_Topic); 

		mpCB_Obj = pCB_Obj;
		mpCBO_StringFn = mpString_Function;
		mpCBO_CommandFn = mpCommand_Function;
		if (mpCBO_CommandFn != NULL)
			bCommand_Function = TRUE;		
			                      
		pPipe_In = new CGUI_DDE_Server(pszPipe_My_Name, &bPipe_Created);
		if (bPipe_Created && (pPipe_In != NULL)) {
			pPipe_In->CreateTopic(pszPipe_My_Topic, ((CGUI_CallBack *)((void *)this)), 
				(GUI_CB_RequestServerFn)&Pipe::Receive_String, 
				(GUI_CB_ExecuteServerFn)&Pipe::Receive_Command);
				}			
		}
		
}



/////////////////////////////////////////////////////////////////////////////////////
//  One way pipe enter constructors
/////////////////////////////////////////////////////////////////////////////////////

Pipe::Pipe(const char *pszMy_Name, const char *pszMy_Topic, 
				Pipe_CallBack *pCB_Obj,
				Pipe_CB_StringFn mpString_Function,
				Pipe_CB_CommandFn mpCommand_Function) :
	bCommand_Function(FALSE),
	bPipe_Created(FALSE),
	bPipe_Open(FALSE),
	Direction(ENTER),
	dPipe_Time_Out(0),
	pszPipe_My_Name(NULL),
	pszPipe_My_Topic(NULL),
	pszPipe_Their_Name(NULL),
	pszPipe_Their_Topic(NULL),
	pPipe_In(NULL),
	pPipe_Out(NULL)
	{

	pszPipe_My_Name = new char [strlen(pszMy_Name) + 1];
	pszPipe_My_Topic = new char [strlen(pszMy_Topic) + 1];
	if ((pszPipe_My_Name == NULL) || (pszPipe_My_Topic == NULL)) {
		strcpy(pszPipe_My_Name, pszMy_Name);
		strcpy(pszPipe_My_Topic, pszMy_Topic);

		mpCB_Obj = pCB_Obj;
		mpCBO_StringFn = mpString_Function;
		mpCBO_CommandFn = mpCommand_Function;
		if (mpCBO_CommandFn != NULL)
			bCommand_Function = TRUE;		
			                      
		pPipe_In = new CGUI_DDE_Server(pszPipe_My_Name, &bPipe_Created);
		if (bPipe_Created && (pPipe_In != NULL)) {
			pPipe_In->CreateTopic(pszPipe_My_Topic, ((CGUI_CallBack *)((void *)this)), 
				(GUI_CB_RequestServerFn)&Pipe::Receive_String, 
				(GUI_CB_ExecuteServerFn)&Pipe::Receive_Command);
				}			
		}
		
}


				 
/////////////////////////////////////////////////////////////////////////////////////
//  One way pipe exit constructors
/////////////////////////////////////////////////////////////////////////////////////
 
Pipe::Pipe(const char *pszTheir_Name, const char *pszTheir_Topic) :
	bCommand_Function(FALSE),
	bPipe_Created(FALSE),
	bPipe_Open(FALSE),
	Direction(EXIT),
	dPipe_Time_Out(0),
	pszPipe_My_Name(NULL),
	pszPipe_My_Topic(NULL),
	pszPipe_Their_Name(NULL),
	pszPipe_Their_Topic(NULL),
	pPipe_In(NULL),
	pPipe_Out(NULL)
	{
	pszPipe_Their_Name = new char [strlen(pszTheir_Name) + 1];
	pszPipe_Their_Topic = new char [strlen(pszTheir_Topic) + 1];
	if ((pszPipe_Their_Name == NULL) || (pszPipe_Their_Topic == NULL)) {
		strcpy(pszPipe_Their_Name, pszTheir_Name);
		strcpy(pszPipe_Their_Topic, pszTheir_Topic); 
		bPipe_Created = TRUE;
		}

} 


/////////////////////////////////////////////////////////////////////////////////////
//  Pipe destructors
/////////////////////////////////////////////////////////////////////////////////////

Pipe::~Pipe(void) {

	if (bPipe_Created) {
		if (Direction == BOTH) {
			if (pPipe_In) {
				delete pPipe_In;
				pPipe_In = NULL;
				}
			if ((bPipe_Open) && (pPipe_Out)) {
				delete pPipe_Out;
				pPipe_Out = NULL;
				}
			if (pszPipe_My_Name) {
				delete [] pszPipe_My_Name;
				pszPipe_My_Name = NULL;
				}
			if (pszPipe_My_Topic) {
				delete [] pszPipe_My_Topic;
				pszPipe_My_Topic = NULL;
				}
			if (pszPipe_Their_Name) {
				delete [] pszPipe_Their_Name;
				pszPipe_Their_Name = NULL;
				}
			if (pszPipe_Their_Topic) {
				delete [] pszPipe_Their_Topic;
				pszPipe_Their_Topic = NULL;
				}
			}
		else if (Direction == ENTER) {
			if (pPipe_In) {
				delete pPipe_In;
				pPipe_In = NULL;
				}
			if (pszPipe_My_Name) {
				delete [] pszPipe_My_Name;
				pszPipe_My_Name = NULL;
				}
			if (pszPipe_My_Topic) {
				delete [] pszPipe_My_Topic;
				pszPipe_My_Topic = NULL;
				}
			}
		else { // Direction == EXIT
			if (pPipe_In) {
				delete pPipe_In;
				pPipe_In = NULL;
				}
			if (pszPipe_Their_Name) {
				delete [] pszPipe_Their_Name;
				pszPipe_Their_Name = NULL;
				}
			if (pszPipe_Their_Topic) {
				delete [] pszPipe_Their_Topic;
				pszPipe_Their_Topic = NULL;
				}
			}
		}    

}
 
 
/////////////////////////////////////////////////////////////////////////////////////
//  Initialization procedures
/////////////////////////////////////////////////////////////////////////////////////
 
BOOL Pipe::Pipe_Created(void) {
                     
	if ((Direction == BOTH) || (Direction == ENTER)) {
		if (pPipe_In != NULL)
			return bPipe_Created;
		else return FALSE;
		}
	else // Direction == EXIT
		return bPipe_Created;
	
}


BOOL Pipe::Open_Communications(void) {

	// If already open then just return TRUE
	if ((bPipe_Created) && (bPipe_Open))
		return TRUE;

	// Check to see that the Server is open
	if ((bPipe_Created) && (!bPipe_Open)) {
		pPipe_Out = new CGUI_DDE_Client(pszPipe_Their_Name, pszPipe_Their_Topic, &bPipe_Open);
		if (bPipe_Open && (pPipe_Out != NULL))
			return TRUE;
		else if (pPipe_Out != NULL) {
			delete pPipe_Out;
			pPipe_Out = NULL;
			}
		}

	return FALSE;

}

            
BOOL Pipe::Close_Communications(void) {

	// If already closed then just return TRUE
	if (!bPipe_Open)
		return TRUE;

	// Close down the connection only if it was opened
	if (bPipe_Open) {
		delete pPipe_Out;
		pPipe_Out = NULL;
		bPipe_Open = FALSE;
		return TRUE;
		}

	return FALSE;

}

            
BOOL Pipe::Set_Timeout(double dTime_Out) {

	dPipe_Time_Out = dTime_Out;
	
	return TRUE;

}


/////////////////////////////////////////////////////////////////////////////////////
//  Sending procedures
/////////////////////////////////////////////////////////////////////////////////////
						
BOOL Pipe::Send(const char *pszString_Send, char **pszString_Receive) {

    CGStr pRetMsg("");
    BOOL bSuccess;


	if (bPipe_Open) {    
		(*pszString_Receive) = NULL;
		bSuccess = pPipe_Out->Request(pszString_Send, &pRetMsg, dPipe_Time_Out);
		if (bSuccess) {
			if (((*pszString_Receive) = new char [strlen(pRetMsg.Get_sz()) + 1]) != NULL) {
				strcpy((*pszString_Receive), pRetMsg.Get_sz());
				return TRUE;
				}
			}
		}
	
	return FALSE;  
	
}


BOOL Pipe::Send(const char *pszCommand_Send, BOOL bVerify_Response) {

    CGStr pRetMsg("");
    BOOL bSuccess = FALSE;

    
	if (bPipe_Open) {
		if (bVerify_Response) {
			bSuccess = pPipe_Out->Request(pszCommand_Send, &pRetMsg, dPipe_Time_Out);
			if (bSuccess)
				bSuccess = bSuccess && (strcmp(pszCommand_Send, pRetMsg.Get_sz()) == NULL);
			}
		else
			bSuccess = pPipe_Out->Execute(pszCommand_Send, NULL, dPipe_Time_Out);
		}
		
	
	return bSuccess;

}


/////////////////////////////////////////////////////////////////////////////////////
//  Private receiving procedures
/////////////////////////////////////////////////////////////////////////////////////

BOOL Pipe::Receive_String(CGStr ItemName, CGStr *pReturnData) {

//	return ((mpCB_Obj->*mpCBO_StringFn)((char *) ItemName.Get_sz(), (char *) pReturnData->Get_sz()));


	BOOL bSuccess;
	char *pszResponse; 

    
    bSuccess = ((mpCB_Obj->*mpCBO_StringFn)((char *) ItemName.Get_sz(), &pszResponse));
    (*pReturnData) = pszResponse;
    //delete [] pszResponse;
    
    return bSuccess; 
    
}
 
 
BOOL Pipe::Receive_Command(CGStr Cmd) {
    
	CGStr Sub_String;
    UINT uiStart_Index = 1;

    
	Cmd.GetSubStr(&uiStart_Index, " ]", &Sub_String);	
   
	return ((mpCB_Obj->*mpCBO_CommandFn)((char *) Sub_String.Get_sz()));
	
}

