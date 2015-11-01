// VistaDataFile.cpp: implementation of the CVistaDataFile class.

///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support generic data functions for 
//	instruments with data in dbVista databases.
//
//	Functions:
//		OpenDataFile
//		CloseDataFile
//		GetStationID
//		GetFileDate
//		
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// There is no GUI activity in class CVistaDataFile so far.  
// This class is a part of the
// NDAR build.  IF GUI activity is added to this class, then Quiet Mode
// logic needs to be added.
// PJM 11/28/2007 B2R1
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VistaDataFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVistaDataFile::CVistaDataFile(
	bool bOverwrite, 
	bool bSkipOutOfOrder,
	bool bQuietMode)
{

	mbOpen = false;
	mbOverwrite = bOverwrite;
	mbSkipOutOfOrder = bSkipOutOfOrder;
	mdVersionNumber = 0.0;
	miDay = 0;
	miErrorNum = 0;
	miMon = 0;
	miYr = 0;
	mpFile = NULL;
	msStaNum = -1;
	mstrFilenameWithPath.Empty();
	mdTimestampOfFirstRecordInFile = 0;

	m_bQuietMode = bQuietMode;      //used by derived classes
	m_Dir.SetQuietMode(bQuietMode); //used by derived classes
}

CVistaDataFile::~CVistaDataFile()
{
}

///////////////////////////////////////////////////////////////////////////
//	Name:	OpenDataFile
//
//	Description:
//	If a data file is not already opne, then open it.
//	
//	Declaration:
//	bool CVistaDataFile::OpenDataFile(const CString &strNameWithPath, CString *pstrErrorMsg)
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
bool CVistaDataFile::OpenDataFile(
	const CString& strNameWithPath, 
	CString *pstrErrorMsg)
{
	// check if any file is already open	
	if (!mbOpen) {

	   /* ------------------------------------------------------------------
		*	Open the file
		* ----------------------------------------------------------------*/
		mpFile = fopen(strNameWithPath,"rb");
		if (mpFile == NULL) 
		{
			if (pstrErrorMsg) pstrErrorMsg->Format("Error: Error during open of %s.", strNameWithPath);
			miErrorNum = iFILE_OPEN_ERR;
			return(false);
		}
		else 
		{
			if (pstrErrorMsg) pstrErrorMsg->Empty();
			mbOpen = true;
			mstrFilenameWithPath = strNameWithPath;
			return(true);
		}
	}	

	// a file is already open, verify it is the one we are looking for
	if (strNameWithPath == mstrFilenameWithPath) 
	{
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
//	void CVistaDataFile::CloseDataFile();
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
void CVistaDataFile::CloseDataFile()
{
	if (mbOpen) 
		fclose(mpFile);
	mbOpen = false;
	mstrFilenameWithPath.Empty();
	mpFile = NULL;
}


// jun-2005 Added by hn
long CVistaDataFile::GetStationID()
{
	return msStaNum;
}

// jun-2005 Added by hn
// 11-Jul-2005 SFK Modified to only use year, month day for file date
// 3-aug-2005 hn Initialize the COleDateTime object with miYr4 or it will not work.
COleDateTime CVistaDataFile::GetFileDate()
{
	//COleDateTime fileDate (mdTimestampOfFirstRecordInFile);
	//int mo = fileDate.GetMonth ();
	//int day = fileDate.GetDay ();
	//int yr = fileDate.GetYear ();
	COleDateTime fileDate;
	fileDate.SetDate (miYr4,miMon,miDay);
	return fileDate;
}
