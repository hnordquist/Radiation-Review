// AccessDataFile.cpp: implementation of the CAccessDataFile class.
//
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support generic data functions for 
//	to open and close data files.
//
//	Functions:
//		OpenDataFile
//		CloseDataFile
//		
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccessDataFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
// The CAccessDataFile class contains common functionality that happens with
// all data files that are imported into the various Access type
// databases.
// Specific data file operations for the various data files should be
// included in a class that inherits from this class (such as 
// CBinaryDataFile where specific actions associated only with 
// *.evt binary data are handled).
//////////////////////////////////////////////////////////////////////
CAccessDataFile::CAccessDataFile(bool bOverwrite, bool bSkipOutOfOrder)
{

	mbOpen = false;
	mbOverwrite = bOverwrite;
	mbSkipOutOfOrder = bSkipOutOfOrder;
	mdVersionNumber = 0.0;
	miDay = 0;
	miErrorNum = 0;
	miMon = 0;
	miYr = 0;
	miYr4 = 0;
	mpFile = NULL;
	msStaNum = -1;
	mstrFilenameWithPath.Empty();
	mdTimestampOfFirstRecordInFile = 0;
}

CAccessDataFile::~CAccessDataFile()
{

}

///////////////////////////////////////////////////////////////////////////
//	Name:	OpenDataFile
//
//	Description:
//	If a data file is not already open, then open it for reading binary data.
//	
//	Declaration:
//	bool CAccessDataFile::OpenDataFile(const CString &strNameWithPath, CString *pstrErrorMsg)
//
//	Input:	
//			mbOpen		whether data file is already open
//			
//	Output:	
//			mstrFilenameWithPath	set to strNameWithPath if data file could be opened
//			mbOpen		indicates whether file open (true) or not (false)
//			mpFile		pointer to file handle if successfully opened
//			
//
//	Return:	true (file opened) / false (some kind of error, see pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	10-Dec-2001	SFK		Created from part of ReadBIDHeader in DbImport.cpp
//////////////////////////////////////////////////////////////////
bool CAccessDataFile::OpenDataFile(const CString &strNameWithPath, CString *pstrErrorMsg)
{
	// check if any file is already open	
	if (!mbOpen) {

	   /* ------------------------------------------------------------------
		*	Open the file
		* ----------------------------------------------------------------*/
		mpFile = fopen(strNameWithPath,"rb");
		if (mpFile == NULL) {
			if (pstrErrorMsg) pstrErrorMsg->Format("Error: Unexpected error during open of %s.", strNameWithPath);
			miErrorNum = iFILE_OPEN_ERR;
			return(false);
		}
		else {
			mbOpen = true;
			mstrFilenameWithPath = strNameWithPath;
			return(true);
		}
	}	

	// a file is already open, verify it is the one we are looking for
	if (strNameWithPath == mstrFilenameWithPath) {
		rewind(mpFile);				// position at beginning of file
		return(true);
	}
	return(false);

}


///////////////////////////////////////////////////////////////////////////
//	Name:	CloseDataFile
//
//	Description:
//	If a data file is open, close it.
//	
//	Declaration:
//	void CAccessDataFile::CloseDataFile();
//
//	Input:	
//			mbOpen		whether data file is already open
//			
//	Output:	
//			mstrFilenameWithPath	null string
//			mbOpen		set to false
//			mpFile		set to NULL
//			
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	10-Dec-2001	SFK		Created from part of ReadBIDHeader in DbImport.cpp
//////////////////////////////////////////////////////////////////
void CAccessDataFile::CloseDataFile()
{
	if (mbOpen) fclose(mpFile);
	mbOpen = false;
	mstrFilenameWithPath.Empty();
	mpFile = NULL;

}



// jun-2005 Added by hn
// 11-Jul-2005 SFK	Added from GRAND stuff
long CAccessDataFile::GetStationID()
{
	return msStaNum;
}

// jun-2005 Added by hn
// 11-Jul-2005 SFK Added and modified to only use year, month day for file date
// 3-aug-2005 hn Use the four-digit year to properly initialize the COleDateTime object.
COleDateTime CAccessDataFile::GetFileDate()
{
//	COleDateTime fileDate (mdTimestampOfFirstRecordInFile);
//	int mo = fileDate.GetMonth ();
//	int day = fileDate.GetDay ();
//	int yr = fileDate.GetYear ();
	COleDateTime fileDate;
	fileDate.SetDate (miYr4,miMon,miDay);
	return fileDate;
}

