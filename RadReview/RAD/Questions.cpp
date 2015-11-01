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