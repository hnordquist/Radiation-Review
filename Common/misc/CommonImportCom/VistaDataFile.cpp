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
