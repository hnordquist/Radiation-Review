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
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : PrintFacilityHeader.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//  PrintFacilityHeader()
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "RadInit.H"
#include "RadReview.H"
#include "MyDateTime.H"
#include "FacilityConfig.h"

extern RADInit *pRAD_Init;
extern RADReview *pRAD_Review;
extern CMyDateTime glMyDateTime;

BOOL PrintFacilityHeader(CGFmtStr *pStr, short sFacNum, DATE dStart, DATE dEnd)
{
	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	char *pszInspection_Header = NULL;
	int status = FALSE;

	// check if want to use common inspection information in the header
	// Use it if there is a commander tool around or
	// I have started in Title mode
	if ((pRAD_Init->Get_IR_Used() == TRUE) || // IR is being used
		(stricmp(pRAD_Init->Get_My_Start_Mode(), "TITLE") == 0)) {	//my start mode is TITLE
		// print a facility header with common inspection information
		status = pRAD_Review->RAD_Print_Header(&pszInspection_Header);
		if (status == TRUE) {
	    	pStr->Printf("%s", pszInspection_Header);
			delete [] pszInspection_Header;
		}

	}
	CGFmtStr TmpStr;
	// either did not want to use common inspection information header or there was a
	// problem during the formation of the common inspection information header
	if (status == FALSE) {
		char szFacilityName[DB_NAME_LEN+1];
		pglFacCfg->GetFacilityLongName(sFacNum, szFacilityName, DB_NAME_LEN);
		TmpStr.Printf("Facility = %s", szFacilityName);
		*pStr += TmpStr;
	}	
	if ((dStart == 0.0) || (dEnd == 0.0)) return(TRUE);	// did not involve search interval
	//CMyDateTime Date;
	glMyDateTime.DATETimestampToDateTimeStrs(dStart, szDate, szTime);
	TmpStr.Printf("\nRequested Search Interval= %s %s to ", szDate, szTime);
	*pStr += TmpStr;
	glMyDateTime.DATETimestampToDateTimeStrs(dEnd, szDate, szTime);
	TmpStr.Printf("%s %s\n", szDate, szTime ); 
	*pStr += TmpStr;

	return (TRUE);
}

// new with DATE
BOOL PrintFacilityHeader(CGUI_TextWin *pWin, short sFacNum, DATE dStart, DATE dEnd)
{
	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	char *pszInspection_Header = NULL;
	int status = FALSE;

	// check if want to use common inspection information in the header
	// Use it if there is a commander tool around or
	// I have started in Title mode
	if ((pRAD_Init->Get_IR_Used() == TRUE) || // IR is being used
		(stricmp(pRAD_Init->Get_My_Start_Mode(), "TITLE") == 0)) {	//my start mode is TITLE
		// print a facility header with common inspection information
		status = pRAD_Review->RAD_Print_Header(&pszInspection_Header);
		if (status == TRUE) {
	    	pWin->Printf("%s", pszInspection_Header);
			delete [] pszInspection_Header;
		}

	}
	// either did not want to use common inspection information header or there was a
	// problem during the formation of the common inspection information header
	if (status == FALSE) {
		char szFacilityName[DB_NAME_LEN+1];
		pglFacCfg->GetFacilityLongName(sFacNum, szFacilityName, DB_NAME_LEN);
		pWin->Printf("Facility = %s", szFacilityName);
	}	
	if ((dStart == 0.0) || (dEnd == 0.0)) return(TRUE);	// did not involve search interval
	//CMyDateTime Date;
	glMyDateTime.DATETimestampToDateTimeStrs(dStart, szDate, szTime);
	pWin->Printf("\nRequested Search Interval= %s %s to ", szDate, szTime);
	glMyDateTime.DATETimestampToDateTimeStrs(dEnd, szDate, szTime);
	pWin->Printf("%s %s\n", szDate, szTime ); 

	return (TRUE);
}
