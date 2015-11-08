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
// EOSSImport.h: interface for the CEOSSImport class.

#ifndef EOSSIMPORT_H
#define EOSSIMPORT_H

class CEOSSImport  
{
public:
	int ConvertChanIDToIndex (short sFacID,long lStationID,short sChanID);
	CEOSSImport(const char* szDefaultPath, short sFacilityID);
	virtual ~CEOSSImport();

	int AddData(
		const short sFacID, 
		const char* strFilename, 
		BOOL bOverwrite, 
		char** strResult);

	int GetDataStatistics(
		const short sFacID, 
		const short sStaID, 
		const short sChanID, 
		const DATE dStart, 
		const DATE dEnd, 
		DATE *pdActualStart, 
		DATE *pdActualEnd, 
		unsigned long *pulNumPts);

	int GetDbStartDay(
		const short sFacID, 
		const short sStaID,
		DATE *dStartDay,
		const BOOL bErrDisplay);

	int GetDbEndDay(
		const short sFacID, 
		const short sStaID,
		DATE *dEndDay,
		const BOOL bErrDisplay);

	int GetDataStoreTimeRange(
		const short sFacID, 
		const short sStaID,  //LONG??
		const BOOL bErrDisplay,
		DATE *pdStartDay,
		DATE *pdEndDay);

	int GetDayStatistics(
		const short sFacID, 
		const struct IDStruct *pStaID, 
		const DATE dStart, 
		const DATE dEnd, 
		DATE *pdRealStart, 
		DATE *pdRealEnd, 
		unsigned long *pulNumPts);

	int GetDataByDay(
		const short sFacID, 
		const struct IDStruct *pStaID, 
		const DATE dStart, 
		const DATE dEnd, 
		struct DayDataStruct *pDayStruct, 
		unsigned long *pulNumDays);

	int GetDataByChannel(
		const short sFacID, 
		const short sStaID, 
		const short sChanID, 
		const DATE dStart, 
		const DATE dEnd, 
		double *pdX, 
		double *pdY, 
		unsigned long *pulNumPts, 
		DATE *pdNextTime, 
		short sStaType);
	
	int  GetDataByStation(
		const short sFacID,
		const short sStaID,
		const DATE dStart,
		const DATE dEnd,
		struct WHOLE_LONG_DATA_PT *pData,
		unsigned long *pulNumPts,
		DATE *pdNextTime);

	int  GetDataByStation(
		const short sFacID, 
		const short sStaID, 
		const DATE dStart, 
		const DATE dEnd, 
		struct WHOLE_DOUBLE_DATA_PT1 *pData, 
		unsigned long *pulNumPts, 
		DATE *pdNextTime);


	int InitializeDatabase(const short sFacilityID, const bool bQuestion);
	void CloseDatabase();
	int  RestoreDatabase(const short sFacilityID, const char *strPath, const char *strDestPath, const bool bQuestion, const bool bPrompt);
	int  BackupDatabase(const short sFacilityID, const char *strPath, const bool bQuestion, const bool bPrompt);

private:

	int GetDbStartEndDay(  //private
		const short sFacID, 
		const short sStaID,
		DATE *pdDay,
		const BOOL bErrDisplay,
		int which);

	char mstrDataStorePath[256];
	char mstrDefaultPath[256];
	short m_sFacilityID;

};
extern CEOSSImport* pglEOSSImport;

#endif //EOSSIMPORT_H
