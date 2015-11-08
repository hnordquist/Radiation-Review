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
// AccessDataFile.h: interface for the CAccessDataFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCESSDATAFILE_H__2D7602C5_ED7D_11D5_816F_00C04F60E89B__INCLUDED_)
#define AFX_ACCESSDATAFILE_H__2D7602C5_ED7D_11D5_816F_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>		// 20-Jan-2006 PJM added for CList

//	Status Returns
#define IDATAFILE		100
#define iSKIP_FILE		IDATAFILE
#define iFILE_READ_ERR	IDATAFILE+1
#define	iFILE_OPEN_ERR	IDATAFILE+2

#include "AccessBaseData.h"

// 11-Jul-2005 SFK	Added
typedef struct tFileRecord
{
	CComVariant Date;  //This is the date of the data inside the file - not the date on the file.
	CComVariant StationID;
	CComVariant File;
} tFileRecord;

class CAccessDataFile {
public:
	

	CAccessDataFile(bool bOverwrite, bool bSkipOutOfOrder);
	virtual ~CAccessDataFile();

	virtual bool ReadDataFile(CDbAccess* pDb, const CString& strNameWithPath, CString* pstrErrorMsg = NULL) = 0;
	virtual bool ReadHeader(const CString& strNameWithPath, CString* pstrErrorMsg = NULL) = 0;
	// 11-Jul-2005 SFK	Added the next 3 functions
	virtual void GetCompleteFileList (
		short FacilityID,
		CList<tFileRecord,tFileRecord> *pFileList,
		const CString& Directory, 
		bool IncludeSubdirs) = 0;

	COleDateTime GetFileDate();
	long GetStationID();

	
	bool mbSkipOutOfOrder;
	bool mbOverwrite;
	int	 miErrorNum;

protected:
	
	bool OpenDataFile(const CString& strNameWithPath, CString *pstrErrorMsg = NULL);
	void CloseDataFile();

	int		miYr;
	int		miMon;
	int		miDay;
	//3-aug-2005 hn Added four digit year for use in all inherited classes of CVistaDataFile
	int		miYr4;
	DATE	mdTimestampOfFirstRecordInFile;
	short   msStaNum;
	double  mdVersionNumber;
	CString mstrFilenameWithPath;
	bool    mbOpen;
	FILE*   mpFile;
};

#endif // !defined(AFX_ACCESSDATAFILE_H__2D7602C5_ED7D_11D5_816F_00C04F60E89B__INCLUDED_)
