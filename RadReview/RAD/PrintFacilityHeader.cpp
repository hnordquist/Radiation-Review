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
