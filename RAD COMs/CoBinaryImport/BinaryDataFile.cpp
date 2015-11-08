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
// BinaryDataFile.cpp: implementation of the CBinaryDataFile class.
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
//	This file contains the routines to support the virtual data file functions
//	specific to the *.bin, *.evt/*.bny files formed for Binary data.
//
//	Functions:
//		ReadBinHeader
//		ReadEvtHeader
//		ReadHeader
//		ReadDataFile
//		ReadBinDataFile
//		ReadEvtDataFile
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include "resource.h"
#include "BinaryDataFile.h"
#include "BinaryData.h"
#include "FacCfgWrap.h"
#include "AccessBaseData.h"
#include "MyDateTime.h"

#define BINARY_TYPE 128		// eventually do not hardwire

#define dINCREMENT 1.0/8640000.0		// 1/100th of a second

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma pack(1)
// structures to described GPS_Rec and BinaryEventRec which are found in a *.evt/*.bny file

struct OpCode {
	short sRecSize;
	char RecTypeA;
	char RecTypeB;
};

struct BinaryEventFileRec {
	unsigned int uiTime;
	BYTE bStatus;
	unsigned short usNode;
	BYTE bState;
	BYTE bMask;
	BYTE bReserved;
	BYTE Dum;
};

// structure to describe records contained in a *.bin file
struct TRGAcquireRecord {
		unsigned int	uiJulianSeconds;	//4 bytes
		BYTE			bStatusByte;		//2 bytes	(whether real or interpolated point)
		unsigned short	usTriggerState;		//2 bytes
		unsigned short	usTriggerMask;		//2 bytes
		BYTE			bReserved;			//1 byte
};

/*struct GPSEventRec{
	unsigned int uiTime;
	unsigned short usNodeNumber;
	double dLatitude;
	double dLongitude;
	double dAltitude;
//	double dClockBias;//bob
	BYTE    bHour;
	BYTE    bMinute;
	BYTE    bSecond;
	BYTE    bDay;
	BYTE    bMonth;
	unsigned short usYear;
	BYTE    bUnused;
	float  fFixTimeA;
	float  fEastVelocity;
	float  fNorthVelocity;
	float  fUpVelocity;
	float  fClockBiasRate;
	float  fFixTimeB;
	BYTE bChecksum;
	BYTE Dum;
	BYTE Dum1;
};
*/
//	To do yet: Add formats of other possible Event file records here

/*struct GPS_Rec{
        char RecTypeA;
        char RecTypeB;
        unsigned int uiJulianTime;
        unsigned short usNodeNumber;
        double dLatitude;
        double dLongitude;
        double dAltitude;
//      double dClockBias;//bob
        BYTE    bHour;
        BYTE    bMinute;
        BYTE    bSecond;
        BYTE    bDay;
        BYTE    bMonth;
        unsigned short usYear;
        BYTE    bUnused;
        float  fFixTimeA;
        float  fEastVelocity;
        float  fNorthVelocity;
        float  fUpVelocity;
        float  fClockBiasRate;
        float  fFixTimeB;
        BYTE bChecksum;
};

*/

#pragma pack(8)



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
// The CBinaryDataFile class contains specific functionality to read
// the data files containing binary data (*.bin and *.evt/*.bny) and store
// that data in the database.  It inherits from CAccessDataFile.
//////////////////////////////////////////////////////////////////////
CBinaryDataFile::CBinaryDataFile(bool bQuietMode)
	:CAccessDataFile(false,false)
{
	m_bQuietMode = bQuietMode;
}

CBinaryDataFile::~CBinaryDataFile()
{

}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Name	 : ReadBinHeader
//
//	Description:
//  Read header from binary raw data file (.BIN).  Bin files were created for use
//	with an early version of Media Tracker.  Defintion of header file
//	proposed in email from Pelowitz defines this format for the header structure:
//		struct TRGAcquireHeader	{	// 73 characters
//			char SizeOfHeader[4];
//			char unused1[5];
//			char Version[5];
//			char StationId[3];//node number
//			char Year[3];
//			char Month[3];
//			char Day[3];
//			char Year4[4];
//			char unused2[43];
//		};
//
//	Declaration:
//	int CBinaryDataFile::ReadBinHeader(char *szFilename, short *psSta,int *piYr, int *piMon, int *piDay, FILE **pHandle, double *pdVer, unsigned long *pulFirstRecordTime)
//
//	Input:	
//			strNameWithPath		filename with full path that is to be opened
//			
//	Output:	pstrErrorMsg	error, if any
//
//			mdVersionNumber		version number read from header
//			msStaNum			station number read from header
//			miYr				year read from header
//			miMon				month read from header
//			miDay				day read from header
//			mdTimestampOfFirstRecordInFile	time of first record in the file in DATE
//
//	Return:	true (header read) / false (some kind of error, see pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	17-Oct-2002	SFK		Created from ReadBIDHeader in GrandDataFile.cpp in GRAND COM
////////////////////////////////////////////////////////////////////////////////////////////////
bool CBinaryDataFile::ReadBinHeader(const CString &strNameWithPath, CString *pstrErrorMsg)
{

    int iHdrSize;
    char str[54];
	fpos_t pos;
	struct TRGAcquireRecord BinaryRawPt;
	CMyDateTime MyDate;

	//	Open the file
	if (!OpenDataFile(strNameWithPath, pstrErrorMsg)) return(false);

	// generate an error message in case we get an error in any of the reads,
	// will clear at end of function if all okay
	if (pstrErrorMsg) {
		miErrorNum = iFILE_READ_ERR;
		pstrErrorMsg->Format("\nError: Unexpected error reading header for file %s", strNameWithPath);
	}
	
	//	Read the first 4 bytes to get the number of bytes in header.
    if (fread(str, 4, 1, mpFile) != 1) return(false);
    str[4] = '\0';    
    iHdrSize = atoi(str);
	if (iHdrSize <= 22) return(false);

    //	The next 5 bytes no longer contain useful information,	just
    //	skip by them.
    if (fread(str, 5, 1, mpFile) != 1) return(false);
       
    //	Read past the version number in the next 5 bytes.
    if (fread(str, 5, 1, mpFile) != 1) return(false);
	str[5] = '\0';
	mdVersionNumber = atof(str);

    //  Read station.
    if (fread(str, 3, 1, mpFile) != 1) return(false);
    str[3] = '\0';
    msStaNum = atoi(str);

    //	Read year.
    if (fread(str, 3, 1, mpFile) != 1) return(false);
    str[3] = '\0';
    miYr = atoi(str);

    //	Read month.
    if (fread(str, 3, 1, mpFile) != 1) return(false);
    str[3] = '\0';
    miMon = atoi(str);
	if ((miMon < 1) || (miMon >12)) return(false);

    //	Read day and put it in return variable.
    if (fread(str, 3, 1, mpFile) != 1) return(false);
    str[3] = '\0';
    miDay = atoi(str);
	if ((miDay < 1) || (miDay >31)) return(false);

    //	Read 4 digit year.
    if (fread(str, 4, 1, mpFile) != 1) return(false);
    str[4] = '\0';
    miYr4 = atoi(str);

    //	Read past the expansion space in the header so the file pointer
    //	is positioned at the beginning of the first data point at exit.
    if (fread(str, (iHdrSize - 26), 1, mpFile)!= 1) return(false);
    
    //	Save the position of the file pointer.
	//	Read the first record in the file to get the time of it.
    //	Restore file pointer to be positioned at the first record.
	if(fgetpos(mpFile, &pos ) != 0) return(false);
	if (fread(&BinaryRawPt, sizeof(struct TRGAcquireRecord), 1, mpFile) == 0) return(false);
	mdTimestampOfFirstRecordInFile = MyDate.MyTimestampToDATETimestamp((double)BinaryRawPt.uiJulianSeconds);
	if(fsetpos(mpFile, &pos ) != 0) return(false);
                    
    if (pstrErrorMsg) pstrErrorMsg->Empty();
	miErrorNum = 0;	// no error
    return(true);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	ReadEvtHeader
//
//	Description:
//	Read header from an Event File (.evt/.bny).  Definition of header file in email from
//	Pelowitz 7/26/2002 has this format:
//		struct EventAcquireHeader  // 73 total characters
//		{
//			char    SizeOfHeader[4];
//			char    unused1[5];
//			char    Version[5];
//			char    StationId[3];
//			char    Year[3];
//			char    Month[3];
//			char    Day[3];
//	        char    FullYear[4];
//			char    unused2[43];
//		};

//
//	Declaration:
//	bool CBinaryDataFile::ReadEvtHeader(const CString &strNameWithPath, CString *pstrErrorMsg)
//
//	Input:	strNameWithPath		filename with full path that is to be opened
//			
//	Output:	pstrErrorMsg	error, if any
//
//			mdVersionNumber		version number read from header
//			msStaNum			station number read from header
//			miYr				year read from header
//			miMon				month read from header
//			miDay				day read from header
//			mulTimestampOfFirstRecordInFile		julian time of first record in the file
//
//	Return:	true (header read) / false (some kind of error, see pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	17-Oct-2002	SFK		Created from ReadBIDHeader in GrandDataFile.cpp in GRAND COM
//	21-Jun-2005 SFK		Pick up station number from first record rather than getting it
//						out of the header due to a problem in MIC 1.9.0.7
//////////////////////////////////////////////////////////////////
bool CBinaryDataFile::ReadEvtHeader(const CString &strNameWithPath, CString *pstrErrorMsg)
{
    int iHdrSize;
    char str[54];
	struct BinaryEventFileRec Rec;
	struct OpCode OpRec;
	fpos_t pos;
	CMyDateTime MyDate;

   /* ------------------------------------------------------------------
    *	Open the file
    * ----------------------------------------------------------------*/
	if (!OpenDataFile(strNameWithPath, pstrErrorMsg)) return(false);

	// generate an error message in case we get an error in any of the reads,
	// will clear at end of function if all okay
	if (pstrErrorMsg) {
		miErrorNum = iFILE_READ_ERR;
		pstrErrorMsg->Format("\nError: Unexpected error reading header for file %s", strNameWithPath);
	}
	
   /* ------------------------------------------------------------------
    *	Read the first 4 bytes to get the number of bytes in header.
    *	Based on the location of the number, determine whether the data
    *	file is from CDMPC or LANL GRAND Collect.  The CDMPC number
    *	must be decremented by 1.
    * ----------------------------------------------------------------*/
    if (fread(str, 4, 1, mpFile) != 1) return(false);
    str[4] = '\0';    
    iHdrSize = atoi(str);
    if (str[3] == ' ') iHdrSize = iHdrSize - 1; // this file formed by CDMPC
	if (iHdrSize <= 22) return(false);

   /* ------------------------------------------------------------------
    *	The next 5 bytes no longer contain useful information,	just
    *	skip by them.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, mpFile) != 1) return(false);
       
   /* ------------------------------------------------------------------
    *	Read past the version number in the next 5 bytes.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, mpFile) != 1) return(false);
	str[5] = '\0';
	mdVersionNumber = atof(str);

   /* ------------------------------------------------------------------
    *  Read station of logging node and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, mpFile) != 1) return(false);
    str[3] = '\0';
    msStaNum = atoi(str);
//	msStaNum += 1000;

   /* ------------------------------------------------------------------
    *	Read year and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, mpFile) != 1) return(false);
    str[3] = '\0';
    miYr = atoi(str);
	//3-aug-2005 hn Added a four digit year.
	if (miYr < 86)
	{
		miYr4 = miYr + 2000;
	}
	else
	{
		miYr4 = miYr + 1900;
	}


   /* ------------------------------------------------------------------
    *	Read month and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, mpFile) != 1) return(false);
    str[3] = '\0';
    miMon = atoi(str);
	if ((miMon < 1) || (miMon >12)) return(false);

   /* ------------------------------------------------------------------
    *	Read day and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, mpFile) != 1) return(false);
    str[3] = '\0';
    miDay = atoi(str);
	if ((miDay < 1) || (miDay >31)) return(false);

	/* ------------------------------------------------------------------
    *	Read past the expansion space in the header so the file pointer
    *	is positioned at the beginning of the first data point at exit.
    * ----------------------------------------------------------------*/
    if (fread(str, (iHdrSize - 22), 1, mpFile)!= 1) return(false);
    
   /* ------------------------------------------------------------------
    *	Save the position of the file pointer.
	*	Read the first record in the file to get the time of it.  Will
	*	read the first record as if it is a binary record since we are
	*	only interested in the julian time in the record and that is in
	*	the same position in all types of records in an .evt/.bny file.
    *	Restore file pointer to just at the first record.
    * ----------------------------------------------------------------*/
	if(fgetpos(mpFile, &pos ) != 0) return(false);
	// 01-Sep-2005 SFK Fixed for reading old files that might have other types
	//					of records interspersed with binary records.
	bool bFoundBinaryRecord = false;
	do {
		if (fread(&OpRec, sizeof(struct OpCode), 1, mpFile) == 0) return(false);
		if ((OpRec.RecTypeA == '3') && (OpRec.RecTypeB == '2')) {
			if (fread(&Rec, sizeof(struct BinaryEventFileRec), 1, mpFile) == 0) return(false);
			msStaNum = Rec.usNode;		// 21-Jun-2005 SFK  Pick up station number from first record
			mdTimestampOfFirstRecordInFile = MyDate.MyTimestampToDATETimestamp((double)Rec.uiTime);
			if(fsetpos(mpFile, &pos ) != 0) return(false);
			bFoundBinaryRecord = true;
		}
		else {	// was not a binary record -- skip past the record
			fpos_t FilePosition;
			fgetpos(mpFile, &FilePosition);
			FilePosition += OpRec.sRecSize - 2;	// subtract the opcode bytes that you've already read
			fsetpos(mpFile, &FilePosition);
		}
	} while (!bFoundBinaryRecord);
               
	if (pstrErrorMsg) pstrErrorMsg->Empty();
	miErrorNum = 0;	// no error
    return(true);

}


bool CBinaryDataFile::ReadHeader(const CString& strNameWithPath, CString* pstrMsg) 
{
	bool bStatus = false;
	CString strFileExtension;

	strFileExtension = strNameWithPath.Right(4);
	if (strFileExtension.CompareNoCase(".bin") == 0) 
		bStatus = ReadBinHeader(strNameWithPath, pstrMsg);
	if (strFileExtension.CompareNoCase(".evt") == 0) 
		bStatus = ReadEvtHeader(strNameWithPath, pstrMsg); 
	if (strFileExtension.CompareNoCase(".bny") == 0) 
		bStatus = ReadEvtHeader(strNameWithPath, pstrMsg); 
	return(bStatus);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	ReadDataFile
//
//	Description:
//	Based on the type of data file, use the proper routine to read the data file
//	and put the data in the database.
//
//	Declaration:
//	bool CBinaryDataFile::ReadDataFile(CDbAccess* pDb, const CString &strNameWithPath, CString *pstrMsg)
//
//	Input:	pDb					Access database will write to
//			strNameWithPath		filename with full path that is to be opened
//			
//	Output:	pstrMsg				information about the import or error, if any
//
//	Return:	true (file imported okay) / false (some kind of error, see pstrMsg and miErrorNum)
//	
//  date    /	author	revision
//  -----------------	--------
//	22-Oct-2002	SFK		Created from ImportData in DbImport.cpp
//////////////////////////////////////////////////////////////////
bool CBinaryDataFile::ReadDataFile(
	CDbAccess* pDb, 
	const CString &strNameWithPath, 
	CString *pstrMsg)
{
	if (pstrMsg) pstrMsg->Empty();	// clear the return message string

	bool bStatus = false;

	//	Read read header information
    bStatus = ReadHeader(strNameWithPath, pstrMsg);
    if (!bStatus) {			// error reading header, can't continue
		if (mpFile) fclose(mpFile);
	    return(false);
	} 

	mstrFilenameWithPath = strNameWithPath;

	// header okay,now read the proper file
	CString strFileExtension;

	strFileExtension = strNameWithPath.Right(4);
	if (strFileExtension.CompareNoCase(".bin") == 0) 
		bStatus = ReadBinDataFile(pDb, strNameWithPath, pstrMsg);
	if (strFileExtension.CompareNoCase(".evt") == 0) 
		bStatus = ReadEvtDataFile(pDb, strNameWithPath, pstrMsg); 
	if (strFileExtension.CompareNoCase(".bny") == 0) 
		bStatus = ReadEvtDataFile(pDb, strNameWithPath, pstrMsg); 

	return(bStatus);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	ReadBinDataFile
//
//	Description:
//	Read the data from a single data file and put it into an Access database.
//	This routine supports the *.BIN format (used in early versions of Media
//	Tracker and included for completeness)
//	The format of each record in a .BIN file is:
//		struct TRGAcquireRecord {
//			unsigned int	uiJulianSeconds;	//4 bytes
//			BYTE			bStatusByte;		//1 bytes	(whether real or interpolated point)
//			unsigned short	usTriggerState;		//2 bytes
//			unsigned short	usTriggerMask;		//2 bytes
//			BYTE			bReserved;			//1 byte
//		};
//	In addition to reading the records, report information about the data read such
//	as (first record, last record, number of records read, number of records
//	out of order, number of records taken during offsets, number of records
//	with timestamps not in the day, etc) and report whether the day was overwritten
//	or added to in the database.
//
//	Declaration:
//	bool CBinaryDataFile::ReadBinDataFile(CDbAccess* pDb, const CString &strNameWithPath, CString *pstrMsg)
//
//	Input:	pDb					Access database will write to
//			strNameWithPath		filename with full path that is to be opened
//			
//	Output:	pstrMsg				information about the import or error, if any
//
//			mdVersionNumber		version number read from header
//			msStaNum			station number read from header
//			miYr				year read from header
//			miMon				month read from header
//			miDay				day read from header
//			mulTimestampOfFirstRecordInFile		DATE of first record in the file
//
//	Return:	true (file imported) / false (some kind of error, see pstrMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	22-Oct-2002	SFK		Created from ImportData in DbImport.cpp
//////////////////////////////////////////////////////////////////
bool CBinaryDataFile::ReadBinDataFile(
	CDbAccess* pDb, 
	const CString &strNameWithPath, 
	CString *pstrMsg)
{

	char szDateStr[MAX_DT_LEN+1], szFirst[MAX_DT_LEN+1], szLast[MAX_DT_LEN+1], szDum[MAX_DT_LEN+1];
	bool bOverwrote = false;
	CString TempStr;

	CFacCfgWrap FacCfg;	// 28-Sep-2005 SFK Removed static

	CDirUtilities Dir(m_bQuietMode);  //QUIET MODE IS ALWAYS OFF FOR the non-NDAR Binary Component pjm 11/27/2007 for B2R1

	CMyDateTime MyDate;
	
	//	During read of header, got the station number associated with this file.
	//	Verify from the	Facility Configuration Com that this is a valid station
	//	number and is a BINARY type.
	struct db_sta_rec dbSta;
//	bool bExists = FacCfg.GetStationRecord(pDb->msFacNum, msStaNum, &dbSta);
	// temp patch for testing
	bool bExists = true;		// patch
	strcpy(dbSta.sz_sta_name,"Media Tracker Station 0");	// patch
	dbSta.s_sta_type = BINARY_TYPE;					// patch

	if (!bExists) {
		if (pstrMsg) pstrMsg->Format("\nError: Skipping file %s with unknown station",Dir.StripPathFromFilename(strNameWithPath));
		if (mpFile) fclose(mpFile);
	    return(false);
	}
	if (dbSta.s_sta_type != BINARY_TYPE) {
		if (pstrMsg) pstrMsg->Format("\nError: Skipping file %s with unexpected station type %d",Dir.StripPathFromFilename(strNameWithPath), dbSta.s_sta_type);
		if (mpFile) fclose(mpFile);
	    return(false);
	}

	// By the time get here, know we have binary data and a valid station number
	CString strStationName = dbSta.sz_sta_name;
	CBinaryData BInst(pDb, msStaNum, -1, BINARY_INST, true, m_bQuietMode);
	
    //	Determine the limits of julian times that belong in the day referred to in the file header
    sprintf(szDateStr,"%02d.%02d.%02d", miYr, miMon, miDay); 
	DB_D_INTERVAL FileDay, ActualDay;
	FileDay.dStart = MyDate.DateTimeStrsToDATETimestamp(szDateStr, "00:00:00");
	FileDay.dEnd = FileDay.dStart + 86399.0/86400.0;
	DATE dFirstTimeInNextDay = FileDay.dStart + 1.0;
    
       
	//	Check if any data already exists for this day in database
	unsigned long ulPtsInDay = 0;
	bool bStatus = BInst.DayExists(FileDay.dStart, &ulPtsInDay, &ActualDay);

	//	If data already in database for the day, check if the new data can be appended to
	//	the existing data.  If not, if bOverwrite = true then delete all data for the day;
	//	if bOverwrite = false, then ask a question as to whether they want to overwrite the
	//	day's data.
	bool bDayAlreadyExists = false;
	if (ulPtsInDay > 0) {		// ulPtsInDay nonzero says some data from day in database
		if (mdTimestampOfFirstRecordInFile > ActualDay.dEnd) {	// data beyond end of db, can add to end of day
			bDayAlreadyExists = true;
		}
		else {	// our data file overlaps with data already in db
			if (!mbOverwrite) { // don't automatically overwrite data
        		TempStr.Format("File %s contains data from %s which already exists in database.  Do you want to overwrite the day?",
					mstrFilenameWithPath, szDateStr);
				if (MessageBox(NULL, TempStr, "EventCom: Day Already In Database", MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO) {
					if (pstrMsg) pstrMsg->Format("\nSkipping file %s (date=%02d.%02d.%02d): day's data already in database", 
						mstrFilenameWithPath, miYr, miMon, miDay);
					if (mpFile) CloseDataFile();
					miErrorNum = iSKIP_FILE;
					return(false);
				}
			}	
			// are going to overwrite the data so delete the entire day
			bStatus = BInst.DeleteDay(FileDay.dStart);
			if (!bStatus) { //if (glpDb->DeleteData(&BInst, DayRequested)) {
				miErrorNum = bStatus;
				if (mpFile) CloseDataFile();
				return(false);	
			}
   			bOverwrote = true;
			ulPtsInDay = 0;
		}
	}	
		
	int i = 0;
	unsigned long ulPtsInFile = 0;
	TRGAcquireRecord RawPt;
	DATE dCurrentPtTime = 0;
	DATE dLastPtTime;
	int iDuplicateTimestamps = 0;
	int iInvalidData = 0;
	int iOutOfOrder = 0;
	dLastPtTime = 0.0;
	CBinaryRawDataPt DbPts[NUM_RAW_DATA_PTS_IN_GROUP+16];	// allow for extra records written on last raw data point
	bool bFirstPoint = true;
	DATE dFirstValidTimeInFile;

	//	Read records from the raw data file one at a time.  For each data point check whether
	//	it has the same timestamp as the previous point -- if yes, add a small increment to
	//	the timestamp read from the file.  Also check if the point really belongs in the day
	//	and whether it is in chronological order.  After you have accumulated a bunch of
	//	points, do a write to the database.
	while (fread(&RawPt, sizeof(struct TRGAcquireRecord), 1, mpFile) != 0) {
		dCurrentPtTime = MyDate.MyTimestampToDATETimestamp(RawPt.uiJulianSeconds);
		ulPtsInFile++;		// this just counts whether there was a point to read

		if (dCurrentPtTime == dLastPtTime) {			// catch two raw data points with the same timestamp
			dCurrentPtTime = dLastPtTime + dINCREMENT;	// the base time of this point is past the last point's time
			iDuplicateTimestamps++;						// count the number of data points with duplicate timestamps
			char szDateStr[MAX_DT_LEN+1], szTimeStr[MAX_DT_LEN+1];
			MyDate.DATETimestampToDateTimeStrs(dCurrentPtTime, szDateStr, szTimeStr, GEN_DTF_IAEA, GEN_DTF_HMSM);		
			TempStr.Format("\nDuplicate at %s  %s", szDateStr, szTimeStr);
			*pstrMsg += TempStr;

		}

		//	If the point's time is not in this day, note it and skip this point.
		if ((dCurrentPtTime < FileDay.dStart) || (dCurrentPtTime > FileDay.dEnd)) {
			iInvalidData++;
			continue;
		}
		
		//	If the point is out of order count it.  
		//	If the points are in order then set a new prev point
		if (bFirstPoint) dLastPtTime = dCurrentPtTime;
		if (dCurrentPtTime < dLastPtTime) {
			iOutOfOrder++;
			if (mbSkipOutOfOrder == true) continue;
		}
		else dLastPtTime = dCurrentPtTime;			// set for the next point read

		//	The largest point in the day is the last point and the smallest
		//	point in the day is the first point.
		if (dCurrentPtTime > ActualDay.dEnd) ActualDay.dEnd = dCurrentPtTime;
		if (bFirstPoint) {
			ActualDay.dStart = dCurrentPtTime;
			dFirstValidTimeInFile = dCurrentPtTime;
			bFirstPoint = false;
		}	
		else {
			if (dCurrentPtTime < ActualDay.dStart) {
				ActualDay.dStart = dCurrentPtTime;
				dFirstValidTimeInFile = dCurrentPtTime;
			}
		}

		// generate the stuff to write into the database
		// write one data record for each bit in the trigger mask that is set
		unsigned short usTempMask = 1;
		for (int j=0; j< 16; j++) {
			if ((usTempMask & RawPt.usTriggerMask) == usTempMask) {
				DbPts[i].m_dJulianTime = dCurrentPtTime;
				DbPts[i].m_ucStatus = RawPt.bStatusByte;
				// determine whether the state of the bit is on or off
				if ((RawPt.usTriggerState & usTempMask) == 0) DbPts[i].m_ucState = 0;
				else DbPts[i].m_ucState = 1;
				DbPts[i].m_usBit = j+1;						// bit number 1-16 corresponds to channel 1-16
				DbPts[i].m_usLogNodeNumber = msStaNum;		// the "station" number in the file is really the log node number
				DbPts[i].m_usStationNumber = msStaNum;		// with this data the station number and logging node are the same
				DbPts[i].m_ucReserved = RawPt.bReserved;
				i++;										// count the point just read from the file
			}
			usTempMask = usTempMask << 1;
		}
	   
		// when accumulate enough data points, write the group to the database
		if (i >= NUM_RAW_DATA_PTS_IN_GROUP) {
			BInst.m_ulNumPtsRequested = i;
			BInst.m_pBinaryRawPts = DbPts;
			BInst.AddData(pstrMsg);
			ulPtsInDay += i;	// count points read so far
			i = 0;
		}
	}

	//	Got an error reading the data file.  Are expecting an EOF
	//	error.	If it's anything else, then abort and delete the partial
	//	data already in the db.  If it's EOF, close	the raw data file
	//	and continue.
	if (feof(mpFile) == 0) {		// check for any error other than end of file
		if (pstrMsg) pstrMsg->Format("\nImport Error Reading File %s.  File Error = %s", mstrFilenameWithPath, strerror(errno));
		if (mpFile) CloseDataFile();
		BInst.DeleteDay(FileDay.dStart);
		if (mpFile) CloseDataFile();
		return(false);
	}
	if (mpFile) CloseDataFile();

	//	Are at the end of the raw data file.  So write whatever points
	//	we have read in our group to the database
	if (i > 0) {
		BInst.m_ulNumPtsRequested = i;
		BInst.m_pBinaryRawPts = DbPts;
		BInst.AddData(pstrMsg);
		ulPtsInDay += i;	// accumulate points read so far
	}

    //	If there was no data for this day, make sure there is nothing about
	//	this day in the database.
    //	Print out some hints for the user as to why no data points are in the day.
    if (ulPtsInDay == 0) {
		BInst.DeleteDay(FileDay.dStart);		// delete data for entire day
		if (pstrMsg) {							// Can return hints to caller
			if (ulPtsInFile == 0) {				// the file is completely empty
				pstrMsg->Format("\nImport Warning Reading File %s.  No binary data in file.", Dir.StripPathFromFilename(strNameWithPath));
			}
			else {
				if (iInvalidData> 0) {			// all times were invalid
					MyDate.DATETimestampToDateTimeStrs(dCurrentPtTime, szFirst, szDum, GEN_DTF_IAEA, GEN_DTF_HMSM);
    				pstrMsg->Format("\nImport Error Reading File %s.  Header indicates data from %s, file data from %s\n", Dir.StripPathFromFilename(strNameWithPath), szDateStr, szFirst);
				}
   				if (iOutOfOrder > 0)			// all data was out of order
					TempStr.Format("\nImport Error Reading File %s.  %5d pts out of order.", Dir.StripPathFromFilename(strNameWithPath), iOutOfOrder);
				*pstrMsg += TempStr;
			}
		}    
		return(true);
    }

    //	Log which file was just imported successfully.  Include date, station
    //	name, file name and first/last time in the file.
    //	Also log if the day's data was overwritten and if there were
    //	any points out of order or data with invalid times.
    if (pstrMsg) { 
		MyDate.DATETimestampToDateTimeStrs(dFirstValidTimeInFile, szDum, szFirst, GEN_DTF_IAEA, GEN_DTF_HMSM);
		MyDate.DATETimestampToDateTimeStrs(ActualDay.dEnd, szDum, szLast, GEN_DTF_IAEA, GEN_DTF_HMSM);		
		TempStr.Format("\n%s  %25s  %s    %s    %s    %5ld", szDateStr, strStationName, Dir.StripPathFromFilename(strNameWithPath), szFirst, szLast, ulPtsInDay);
		*pstrMsg += TempStr;
		if (bOverwrote == TRUE) {
			TempStr.Format("  Overwrote existing day's data.");
			*pstrMsg += TempStr;
		}
		if (bDayAlreadyExists == TRUE) {
			TempStr.Format("  Added to existing day's data.");
			*pstrMsg += TempStr;
		}
		if (iOutOfOrder > 0) {
			TempStr.Format("  %5d pts out of order.", iOutOfOrder);
			*pstrMsg += TempStr;
		}
		if (iInvalidData > 0) {
			TempStr.Format("  %5d rec(s) with invalid times.",iInvalidData);
			*pstrMsg += TempStr;
		}
		if (iDuplicateTimestamps > 0) {
			TempStr.Format("  %5d rec(s) with duplicate times.",iDuplicateTimestamps);
			*pstrMsg += TempStr;
		}
    }    

	//  Calculate and add the information about this day's worth of data
	TempStr.Empty();
	BInst.AddDayData(ActualDay, ulPtsInDay, &TempStr);
	*pstrMsg += TempStr;

    return(true);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	ReadEvtDataFile
//
//	Description:
//	Read the data from a single data file and put it into an Access database.
//	This routine supports the *.EVT format. 
//
//	An .EVT file can have several types of records in the file.  The first two characters 
//	determine the type of record.  The types possible are:
//							RecTypeA	RecTypeB		Number of Characters
//		BINARYEVENT_REC		"3"			"2"				13
//		VACOSSINFO_REC		"3"			"5"				31
//		VACOSSEVENT_REC		"3"			"6"				44
//		VACOSSERROR_REC		"3"			"9"				16
//		MICROGRAND_REC		"4"			"0"				17
//		GPSDATA_REC			"3"			"A"				65
//
//	The formats of these records can be found at the beginning of this file.
//
//	In addition to reading the file, information about the data read such
//	as (first record, last record, number of records read, number of records
//	out of order, number of records taken during offsets, number of records
//	with timestamps not in the day, etc) are reported and also whether the day was overwritten
//	or added to in the database.
//
//	Declaration:
//	bool CBinaryDataFile::ReadBinDataFile(CDbAccess* pDb, const CString &strNameWithPath, CString *pstrMsg)
//
//	Input:	
//			strNameWithPath		filename with full path that is to be opened
//			
//	Output:	pstrErrorMsg	error, if any
//
//			mdVersionNumber		version number read from header
//			msStaNum			station number read from header
//			miYr				year read from header
//			miMon				month read from header
//			miDay				day read from header
//			mulTimestampOfFirstRecordInFile		julian time of first record in the file
//			miErrorNum			problem during read
//				iFILE_READ_ERROR
//				iSKIP_FILE
//				iDB_BAD
//	
//	Return:	true (header read) / false (some kind of error, see pstrErroMsg and miErrorNum)
//	
//  date    /	author	revision
//  -----------------	--------
//	10-Dec-2001	SFK		Created from ImportData in DbImport.cpp
//////////////////////////////////////////////////////////////////
bool CBinaryDataFile::ReadEvtDataFile(CDbAccess* pDb, const CString &strNameWithPath, CString *pstrMsg)
{

	char szDateStr[MAX_DT_LEN+1], szFirst[MAX_DT_LEN+1], szLast[MAX_DT_LEN+1], szDum[MAX_DT_LEN+1];
	bool bOverwrote = false;
	CString TempStr;

	CFacCfgWrap FacCfg;	// 28-Sep-2005 SFK Removed static

	CDirUtilities Dir(m_bQuietMode);  //QUIET MODE IS ALWAYS OFF FOR the non-NDAR Binary Component pjm 11/27/2007 for B2R1

	CMyDateTime MyDate;
	
	//	During read of header, got the station number associated with this file.
	//	Verify from the	Facility Configuration Com that this is a valid station
	//	number and is a BINARY type.
	struct db_sta_rec dbSta;
	bool bExists = FacCfg.GetStationRecord(pDb->msFacNum, msStaNum, &dbSta);

	if (!bExists) {
		if (pstrMsg) pstrMsg->Format("\nError: Skipping file %s with unknown station %d",Dir.StripPathFromFilename(strNameWithPath), msStaNum);
		if (mpFile) fclose(mpFile);
	    return(false);
	}
	if (dbSta.s_sta_type != BINARY_TYPE) {
		if (pstrMsg) pstrMsg->Format("\nError: Skipping file %s with unexpected station type %d",Dir.StripPathFromFilename(strNameWithPath), dbSta.s_sta_type);
		if (mpFile) fclose(mpFile);
	    return(false);
	}

	// By the time get here, know we have binary data and a valid station number
	CString strStationName = dbSta.sz_sta_name;
	CBinaryData BInst(pDb, msStaNum, -1, BINARY_INST, true, m_bQuietMode);
	
    //	Determine the limits of julian times that belong in the day referred to in the file header
    sprintf(szDateStr,"%02d.%02d.%02d", miYr, miMon, miDay); 
	DB_D_INTERVAL FileDay, ActualDay;
	FileDay.dStart = MyDate.DateTimeStrsToDATETimestamp(szDateStr, "00:00:00");
	FileDay.dEnd = FileDay.dStart + 86399.0/86400.0;
	DATE dFirstTimeInNextDay = FileDay.dStart + 1.0;
    
       
	//	Check if any data already exists for this day for this node in database
	unsigned long ulPtsInDay = 0;
	bool bStatus = BInst.DayExists(FileDay.dStart, &ulPtsInDay, &ActualDay);

	//	If data already in database for the day, check if the new data can be appended to
	//	the existing data.  If not, if bOverwrite = true then delete all data for the day;
	//	if bOverwrite = false, then ask a question as to whether they want to overwrite the
	//	day's data.
	bool bDayAlreadyExists = false;
	if (ulPtsInDay > 0) {		// ulPtsInDay nonzero says some data from day in database
		if (mdTimestampOfFirstRecordInFile > ActualDay.dEnd) {	// data beyond end of db, can add to end of day
			bDayAlreadyExists = true;
		}
		else {	// our data file overlaps with data already in db
			if (!mbOverwrite) { // don't automatically overwrite data
        		TempStr.Format("File %s contains data from %s which already exists in database.  Do you want to overwrite the day?",
					mstrFilenameWithPath, szDateStr);
				if (MessageBox(NULL, TempStr, "EventCom: Day Already In Database", MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO) {
					if (pstrMsg) pstrMsg->Format("\nSkipping file %s (date=%02d.%02d.%02d): day's data already in database", 
						mstrFilenameWithPath, miYr, miMon, miDay);
					if (mpFile) CloseDataFile();
					miErrorNum = iSKIP_FILE;
					return(false);
				}
			}	
			// are going to overwrite the data so delete the entire day
			bStatus = BInst.DeleteDay(FileDay.dStart);
			if (!bStatus) { //if (glpDb->DeleteData(&BInst, DayRequested)) {
				miErrorNum = bStatus;
				if (mpFile) CloseDataFile();
				return(false);	
			}
   			bOverwrote = true;
			ulPtsInDay = 0;
		}
	}	
		
	int i = 0;
	unsigned long ulPtsInFile = 0;
	BinaryEventFileRec FilePt;
	OpCode Op;
	// To do yet:  add records for other types of data in event file
	DATE dCurrentPtTime = 0;
	DATE dLastPtTime;
	int iDuplicateTimestamps = 0;
	int iInvalidData = 0;
	int iOutOfOrder = 0;
	dLastPtTime = 0.0;
	short sLastStation = -1;
	CBinaryRawDataPt PtsForDb[NUM_RAW_DATA_PTS_IN_GROUP+16];	// allow for extra records written on last raw data point
	bool bFirstPoint = true;
	DATE dFirstValidTimeInFile;

	//	Read records from the raw data file one at a time.  First read the opcode and
	//	decide what kind of record you have.  Skip past any records other than the BinaryEvent type.
	//	For each data point in the BinaryEventRecs check whether it has the same timestamp as
	//	the previous point -- if yes, add a small increment to the timestamp read from the file.
	//	Also check if the point really belongs in the day and whether it is in chronological order.
	//	After you have accumulated a bunch of points, do a write to the database.
	while (fread(&Op, sizeof(struct OpCode), 1, mpFile) != 0) {	// read the op code
		// if record is BinaryEvent type, then read the record and process it.
		if ((Op.RecTypeA == '3') && (Op.RecTypeB == '2')) {
			fread(&FilePt, sizeof(struct BinaryEventFileRec), 1, mpFile);

			dCurrentPtTime = MyDate.MyTimestampToDATETimestamp(FilePt.uiTime);
			ulPtsInFile++;		// this just counts whether there was a point to read

			// catch two raw data points from the same node with the same timestamp
			if ((dCurrentPtTime == dLastPtTime) && (sLastStation == FilePt.usNode) ){			
				dCurrentPtTime = dLastPtTime + dINCREMENT;	// the base time of this point is past the last point's time
				iDuplicateTimestamps++;						// count the number of data points with duplicate timestamps
				FilePt.bStatus = FilePt.bStatus | DUPLICATE_POINT_BIT;
			}

			//	If the point's time is not in this day, note it and skip this point.
			if ((dCurrentPtTime < FileDay.dStart) || (dCurrentPtTime > FileDay.dEnd)) {
				iInvalidData++;
				continue;
			}
			
			//	If the point is out of order count it.  
			//	If the points are in order then set a new prev point
			//  Since there can be multiple node numbers logged into a single
			//	file, check that the node numbers are the same when
			//	checking if out of order.
			if (bFirstPoint) {
				dLastPtTime = dCurrentPtTime;
				sLastStation = FilePt.usNode;
			}
			if ((dCurrentPtTime < dLastPtTime)  && (sLastStation == FilePt.usNode)){
				iOutOfOrder++;
				if (mbSkipOutOfOrder == true) continue;
				FilePt.bStatus = FilePt.bStatus | OUT_OF_ORDER_BIT;
			}
			else {
				dLastPtTime = dCurrentPtTime;			// set for the next point read
				sLastStation = FilePt.usNode;
			}

			//	The largest point in the day is the last point and the smallest
			//	point in the day is the first point.
			if (dCurrentPtTime > ActualDay.dEnd) ActualDay.dEnd = dCurrentPtTime;
			if (bFirstPoint) {
				ActualDay.dStart = dCurrentPtTime;
				dFirstValidTimeInFile = dCurrentPtTime;
				bFirstPoint = false;
			}	
			else {
				if (dCurrentPtTime < ActualDay.dStart) {
					ActualDay.dStart = dCurrentPtTime;
					dFirstValidTimeInFile = dCurrentPtTime;
				}
			}

			// generate the stuff to write into the database
			// write one data record for each bit in the mask that is set
			BYTE bTempMask = 1;
			for (int j=0; j< 8; j++) {
				if ((bTempMask & FilePt.bMask) == bTempMask) {	// check if this bit is set
					PtsForDb[i].m_dJulianTime = dCurrentPtTime;
					PtsForDb[i].m_ucStatus = FilePt.bStatus;
					//if ((FilePt.bState & bTempMask) != 0) PtsForDb[i].m_ucState = 1;	// determine state of this bit
					//else PtsForDb[i].m_ucState = 0;
					PtsForDb[i].m_ucState = FilePt.bState;
					PtsForDb[i].m_usBit = j+1;			// bit number 1-8 corresponds to channel 1-8
					PtsForDb[i].m_ucReserved = FilePt.bReserved;
					PtsForDb[i].m_usLogNodeNumber = msStaNum;				// the "station" number in the file is really the log node number
//					PtsForDb[i].m_usStationNumber = FilePt.usNode + 1000;	// this is the real station of the data -- where the binary data came from
					PtsForDb[i].m_usStationNumber = FilePt.usNode;			// this is the real station of the data -- where the binary data came from
					i++;	// count the point just read from the file
				}
				bTempMask = bTempMask << 1;		// go on to next bit
			}
		   
			// when accumulate enough data points, write the group to the database
			if (i >= NUM_RAW_DATA_PTS_IN_GROUP) {
				BInst.m_ulNumPtsRequested = i;
				BInst.m_pBinaryRawPts = PtsForDb;
				BInst.AddData(pstrMsg);
				ulPtsInDay += i;	// count points read so far
				i = 0;
			}
		}
		else {	// was not a binary record -- skip past the record
			fpos_t FilePosition;
			fgetpos(mpFile, &FilePosition);
			FilePosition += Op.sRecSize - 2;	// subtract the opcode bytes that you've already read
			fsetpos(mpFile, &FilePosition);
		}
	}


	//	Got an error reading the data file.  Are expecting an EOF
	//	error.	If it's anything else, then abort and delete the partial
	//	data already in the db.  If it's EOF, close	the raw data file
	//	and continue.
	if (feof(mpFile) == 0) {		// check for any error other than end of file
		if (pstrMsg) pstrMsg->Format("\nImport Error Reading File %s.  File Error = %s", mstrFilenameWithPath, strerror(errno));
		if (mpFile) CloseDataFile();
		BInst.DeleteDay(FileDay.dStart);
		if (mpFile) CloseDataFile();
		return(false);
	}
	if (mpFile) CloseDataFile();

	//	Are at the end of the raw data file.  So write whatever points
	//	we have read in our group to the database
	if (i > 0) {
		BInst.m_ulNumPtsRequested = i;
		BInst.m_pBinaryRawPts = PtsForDb;
		BInst.AddData(pstrMsg);
		ulPtsInDay += i;	// accumulate points read so far
	}

	//	If there was no data for this day, make sure there is nothing about
	//	this day in the database.
	//	Print out some hints for the user as to why no data points are in the day.
	if (ulPtsInDay == 0) {
		BInst.DeleteDay(FileDay.dStart);		// delete data for entire day
		if (pstrMsg) {							// Can return hints to caller
			if (ulPtsInFile == 0) {				// the file is completely empty
				pstrMsg->Format("\nImport Warning Reading File %s.  No binary data in file.", Dir.StripPathFromFilename(strNameWithPath));
			}
			else {
				if (iInvalidData> 0) {			// all times were invalid
					MyDate.DATETimestampToDateTimeStrs(dCurrentPtTime, szFirst, szDum, GEN_DTF_IAEA, GEN_DTF_HMSM);
    				pstrMsg->Format("\nImport Error Reading File %s.  Header indicates data from %s, file data from %s\n", Dir.StripPathFromFilename(strNameWithPath), szDateStr, szFirst);
				}
   				if (iOutOfOrder > 0)			// all data was out of order
					TempStr.Format("\nImport Error Reading File %s.  %5d pts out of order.", Dir.StripPathFromFilename(strNameWithPath), iOutOfOrder);
				*pstrMsg += TempStr;
			}
		}    
		return(true);
	}

	//	Log which file was just imported successfully.  Include date, station
	//	name, file name and first/last time in the file.
	//	Also log if the day's data was overwritten and if there were
	//	any points out of order or data with invalid times.
	if (pstrMsg) { 
		MyDate.DATETimestampToDateTimeStrs(dFirstValidTimeInFile, szDum, szFirst, GEN_DTF_IAEA, GEN_DTF_HMSM);
		MyDate.DATETimestampToDateTimeStrs(ActualDay.dEnd, szDum, szLast, GEN_DTF_IAEA, GEN_DTF_HMSM);		
		TempStr.Format("\n%s  %25s  %s    %s    %s    %5ld", szDateStr, strStationName, Dir.StripPathFromFilename(strNameWithPath), szFirst, szLast, ulPtsInDay);
		*pstrMsg += TempStr;
		if (bOverwrote == TRUE) {
			TempStr.Format("  Overwrote existing day's data.");
			*pstrMsg += TempStr;
		}
		if (bDayAlreadyExists == TRUE) {
			TempStr.Format("  Added to existing day's data.");
			*pstrMsg += TempStr;
		}
		if (iOutOfOrder > 0) {
			TempStr.Format("  %5d pts out of order.", iOutOfOrder);
			*pstrMsg += TempStr;
		}
		if (iInvalidData > 0) {
			TempStr.Format("  %5d rec(s) with invalid times.",iInvalidData);
			*pstrMsg += TempStr;
		}
		if (iDuplicateTimestamps > 0) {
			TempStr.Format("  %5d rec(s) with duplicate times.",iDuplicateTimestamps);
			*pstrMsg += TempStr;
		}
	}    

	//  Calculate and add the information about this day's worth of data
	TempStr.Empty();
	BInst.AddDayData(ActualDay, ulPtsInDay, &TempStr);
	*pstrMsg += TempStr;
	
    return(true);
}


//This function was added to CGrandDataFile to recursively retrieve a list of *.BID files in
//a directory.  It reads the file header to populate the CList of tFileRecords that is converted to
//a safe array used by ImportManager.  This was flagrantly stolen from Kelly Michel and modified to
//search for and read in *.BID files. hn 6/8/2005
// 5-Jul-2005 SFK Removed dead code for readability
// 11-Jul-2005 SFK	Copied from GRAND Import and modified for binary
void CBinaryDataFile::GetCompleteFileList(short FacilityID, CList<tFileRecord, tFileRecord> *pFileList, 
									   const CString& Directory,  bool IncludeSubdirs)
{
	//USES_CONVERSION;

	CFileFind Finder;
	BOOL bWorking;
	CString  FileName;
	tFileRecord FileRecord;

	CString DirWithFileMask;
	DirWithFileMask.Format("%s\\*.*", Directory);
	bWorking = Finder.FindFile(DirWithFileMask);

	
	//If this is an "archive" directory, then skip it completely, and everything that may be underneath it.
	int StartSubDirName = Directory.ReverseFind('\\');
	if(StartSubDirName != -1)
	{
		CString SubDirName = Directory.Mid(StartSubDirName + 1);
		if(SubDirName.CompareNoCase("Archive")) //If SubDirName is not Archive...
		{
			do
			{
				bWorking = Finder.FindNextFile();
				
				if(!Finder.IsDots())
				{
					if(Finder.IsDirectory() && IncludeSubdirs)
					{
						//Recurse.
						GetCompleteFileList(FacilityID, pFileList, Finder.GetFilePath(), IncludeSubdirs);
					}
					else //if(Finder.IsNormal())
					{
 						FileName = Finder.GetFileName(); 
						CString Ext = FileName.Mid(FileName.GetLength() - 3, 3);
						if(!Ext.CompareNoCase("BNY"))
						{
							FileRecord.File.bstrVal = (Finder.GetFilePath()).AllocSysString();

							//****************************************************************
							//Open the file and get info on the data in the file.  Load that
							//file data into the FileRecord structure.
							//****************************************************************
							CString err;
							//If we are not able to read the *.BNY header, we fail
							CString cs(FileRecord.File.bstrVal);
							if (!ReadHeader (cs,&err))
							//if (!ReadHeader (W2T(FileRecord.File.bstrVal),&err))
							{
								if (mpFile) CloseDataFile();
							}
							else
							//Otherwise, save the file date and station ID read.
							{
								SYSTEMTIME sysTime;
								COleDateTime fileDate = GetFileDate ();
								fileDate.GetAsSystemTime (sysTime);
								SystemTimeToVariantTime (&sysTime,&FileRecord.Date.date);
								FileRecord.StationID.lVal = (long) GetStationID ();
								pFileList->AddTail (FileRecord);
								CloseDataFile ();
							}
						}
					}
				}		
			}
			while(bWorking != 0);
		}
	}

}

