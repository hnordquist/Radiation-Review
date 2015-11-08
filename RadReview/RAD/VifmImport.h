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
#if !defined(VifmImport_H)
#define VifmImport_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// VifmImport.h : header file
//
//int OpenVifmImportDatabase(const char* strDataStorePath);
int OpenVifmImportDatabase(const short sFacilityID);

/////////////////////////////////////////////////////////////////////////////
// CVifmImport - provides link between traditional Rad and VifmImport com object
class CVifmImport 
{
public:

	CVifmImport::CVifmImport(const char* szDefaultPath);
	virtual ~CVifmImport();

	int  AddData(const short sFacID, const char* strFilename, BOOL bOverwrite, char** strResult);
	int  BackupDatabase(const short sFacID, const char *strPath, const bool bQuestion, const bool bPrompt);
	void CloseDatabase();
	int  GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime);
	int  GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays);
	int  GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_FLOAT_DATA_PT1 *pfData, unsigned long *pulNumPts, DATE *pdNextTime);
	int  GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_DOUBLE_DATA_PT1 *pdData, unsigned long *pulNumPts, DATE *pdNextTime);
	int  GetDataStatistics(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts);
	void GetDataStoreSize(const short sFacID,double *pdSizeInBytes);
	int  GetDayStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts);
	int  GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay);
	int  GetDbStartDay(const short sFacID, const short sStaID, DATE *pdStartDay, const bool bErrDisplay);
	int  InitializeDatabase(const short sFacID, const bool bQuestion);
	bool  OpenDatabase(const short sFacID);
	int  RestoreDatabase(const short sFacID,const char *strPath, const char *strDestPath, const bool bQuestion, const bool bPrompt);
//	int  SetDatabase(const char *strPath);

protected:

	char mstrDataStorePath[256];
	char mstrDefaultPath[256];
	short m_sFacNum;
	

//private:
};
extern CVifmImport *pglVifmImport;



#endif // !defined(VifmImport_H)