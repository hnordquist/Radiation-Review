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
// VifmDataFile.h: interface for the CVifmDataFile class.
//
#ifndef VIFMDATAFILE_H
#define VIFMDATAFILE_H

#include "VistaDataFile.h"
#include "VifmData.h"

class CVifmDataFile : public CVistaDataFile  
{
public:
	CVifmDataFile(bool bQuietMode);
	virtual ~CVifmDataFile();

	bool ReadDataFile(CDbVista* pDb, const CString &strNameWithPath, CString *pstrErrorMsg=NULL);
	bool ReadHeader(const CString& strNameWithPath, CString *pstrErrorMsg);

	bool ReadVBFHeader(const CString& strNameWithPath, CString *pstrErrorMsg);
	bool ReadVBFHeader(fpos_t pos, const CString& strNameWithPath, CString *pstrErrorMsg);

	bool FinishDayInDatabase(int i, CVifmData *pVInst, CString *pstrMsg);
	bool StartDayInDatabase(CVifmData *pVInst, int *pIndex, CString *pstrMsg);

	void GetCompleteFileList(
		short sFacilityID, 
		CList<tFileRecord, tFileRecord> *pFileList, 
		const CString& Directory,  
		bool bIncludeSubdirs);	// 06-Jul-2005 SFK added

private:

	//These are in the base class.
	//bool m_bQuietMode;
	//CDirUtilties m_Dir;
	//CMyDateTime m_MyDateTime;

	int IAEAReadDataFile (char * inputFileName, int * station_ID, int * numberOfRecords, double * samplingInterval);
	void SpikeCut (double dSamplingInterval, int iNumberOfRecords);
	void TimeToString (double timeStamp, char * timeString);
	//int FileIndexCreate (char * inputFileRoot);

    struct db_float_data_rec	mdbFloatData;	/* database record structure */
    unsigned long				mulPtsInDaySoFar; /* how many data pts in day so far */
    unsigned long				mulPtsInDay;	    /* how many data pts in entire day */
	DATE						mdFirstTimeInDay;
	DATE						mdLastTimeInDay;
	DATE						mdDayStart;
	DATE						mdFirstTimeInFile;
	DATE						mdTimestampOfLastRecordInFile;
    DATE						mdPrevTime;
	bool						mbDayAlreadyExists;
	bool						mbOverwrote;
	bool						mbFirstPoint;
	bool						mbAddedToBeginning;		// added to the beginning of day in db
	unsigned long				mulPtsToAddToEnd;		// number of points read out of db when adding to beginning of day
};

#endif // VIFMDATAFILE_H
