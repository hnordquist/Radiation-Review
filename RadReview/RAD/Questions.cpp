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
///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : Questions.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"    
#include "Utilities.h"


int AskAndMakePath(char *TempPath)
{ 

	CGFmtStr msg;
	GUI_ACTION response;
	int status;
	
	msg.Printf("The path \n%s \ndoes not exist.  \nDo you want to create it?", TempPath);
	response = GUI_MsgBox(msg, GUI_ICON_QUESTION, GUI_YESNO);
	if (response == GUI_NO) return(FALSE);
	status = MakePath(TempPath);	
	if (status != TRUE) {
		RadReviewMsg(status, (const char *)TempPath);
		return(FALSE);
	}
	return(TRUE);
}	

int AskOverwriteQuestion(char *TempPath)
{
	CGFmtStr msg;
	GUI_ACTION response;

	msg.Printf("Data or configuration files already exist at\n%s.\n\nDo you want to overwrite?", TempPath);
	response = GUI_MsgBox(msg, GUI_ICON_QUESTION, GUI_YESNO);
	if (response == GUI_NO) return(FALSE); 
	return(TRUE);
}

int AskNotEnoughRoomQuestion(unsigned long ulDbSize, unsigned long ulDiskSpace, char *szTempPath)
{
	CGFmtStr msg;
	GUI_ACTION response;
	
	msg.Printf("Approximately %ld bytes of storage are required.\nThere is only %ld bytes available at %s.\nDo you want to continue?", ulDbSize, ulDiskSpace, szTempPath);	
	response = GUI_MsgBox(msg, GUI_ICON_QUESTION, GUI_YESNO);
	if (response == GUI_NO) return(FALSE); 
	return(TRUE);
}