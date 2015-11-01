// VifmDataFile.cpp: implementation of the CVifmDataFile class.
//
////////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support the virtual data file functions
//	specific to the *.vbf files formed for Vifm data.
//
//	Functions:
//		ReadHeader
//		ReadDataFile
//		
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "VifmDataFile.h"
#include "VistaBaseData.h"
#include "GenDateTime.h"
#include "FacCfgWrap.h"
#include "TypeDefinitions.h"
#include <math.h>
#include <io.h>

extern "C"
{
	#include "Check_Signature.h"  //SCR 227
}

//(changed for Mike Browne 12 Dec 2007 pjm and han - Version 1.0.2.1c
//of the VIFM Import component.
//ESP 022 was created by Heather Nordquist and Peggy Moore on 12 December 2007.
//One line was changed in the VIFM import code from E10R4.  
//The maximum number of records allowed in a *.vbf file was changed from 300,000 to 500,000.
#define MAX_RECORD 500000//300000 max records a file can contain

const int BYTES_IN_HEADER = 3072; //0x0c00

static unsigned int countBuffer[MAX_RECORD][8];
static double timeStamp[MAX_RECORD];
static double elapsedTime[MAX_RECORD];


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVifmDataFile::CVifmDataFile(bool bQuietMode)
	:CVistaDataFile(false,false,bQuietMode)
{
	mbDayAlreadyExists = false;
	mbOverwrote = false;
	mbFirstPoint = true;
	mbAddedToBeginning = false;
}

CVifmDataFile::~CVifmDataFile()
{
}

///////////////////////////////////////////////////////////////////////////
//	Name:	ReadHeader
//
//	Description:
//	Read header from VIFM raw data file (.BOT).  See User Manual for VIFM-to-LANL
//	GRAND Review Program Data Conversion Tool, CSSP 84-13 Release 1.0 8 June 2000,
//	for details on header format.
//
//	Declaration:
//	bool CVifmDataFile::ReadHeader(const CString &strNameWithPath, CString *pstrErrorMsg)
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
//
//	Special Note:  This header is identical to the *.BID file header.		
//			
//
//	Return:	true (header read) / false (some kind of error, see pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	10-Dec-2001	SFK		Created from ReadBIDHeader in DbImport.cpp
//	10-Jun-2002	SFK		Converted to read files with ms data
//////////////////////////////////////////////////////////////////
bool CVifmDataFile::ReadHeader(const CString &strNameWithPath, CString *pstrErrorMsg)
{
    int iHdrSize;
    char str[54];
	struct VIFM_DATA_PT1 VifmPt;
	fpos_t pos;

   /* ------------------------------------------------------------------
    *	Open the file
    * ----------------------------------------------------------------*/
	if (!OpenDataFile(strNameWithPath, pstrErrorMsg)) 
		return(false);

	// generate an error message in case we get an error in any of the reads,
	// will clear at end of function if all okay
	if (pstrErrorMsg) 
	{
		miErrorNum = iFILE_READ_ERR;
		pstrErrorMsg->Format("Error: Error during read of %s", strNameWithPath);
	}
	
   /* ------------------------------------------------------------------
    *	Read the first 4 bytes to get the number of bytes in header.
    *	Based on the location of the number, determine whether the data
    *	file is from CDMPC or LANL GRAND Collect.  The CDMPC number
    *	must be decremented by 1.
    * ----------------------------------------------------------------*/
    if (fread(str, 4, 1, mpFile) != 1) 
		return(false);

    str[4] = '\0';    
    iHdrSize = atoi(str);

    if (str[3] == ' ') 
		iHdrSize = iHdrSize - 1; // this file formed by CDMPC

	if (iHdrSize <= 22) 
		return(false);

   /* ------------------------------------------------------------------
    *	The next 5 bytes no longer contain useful information,	just
    *	skip by them.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, mpFile) != 1) 
		return(false);
       
   /* ------------------------------------------------------------------
    *	Read past the version number in the next 5 bytes.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, mpFile) != 1) 
		return(false);

	str[5] = '\0';
	mdVersionNumber = atof(str);

   /* ------------------------------------------------------------------
    *  Read station and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, mpFile) != 1) 
		return(false);

    str[3] = '\0';
    msStaNum = atoi(str);

   /* ------------------------------------------------------------------
    *	Read year and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, mpFile) != 1) 
		return(false);

    str[3] = '\0';
    miYr = atoi(str);

   /* ------------------------------------------------------------------
    *	Read month and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, mpFile) != 1) 
		return(false);

    str[3] = '\0';
    miMon = atoi(str);

	if ((miMon < 1) || (miMon >12)) 
		return(false);

   /* ------------------------------------------------------------------
    *	Read day and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, mpFile) != 1) 
		return(false);

    str[3] = '\0';
    miDay = atoi(str);

	if ((miDay < 1) || (miDay >31)) 
		return(false);

   /* ------------------------------------------------------------------
    *	Read past the expansion space in the header so the file pointer
    *	is positioned at the beginning of the first data point at exit.
    * ----------------------------------------------------------------*/
    if (fread(str, (iHdrSize - 22), 1, mpFile)!= 1) 
		return(false);
    
   /* ------------------------------------------------------------------
    *	Save the position of the file pointer.
	*	Read the first record in the file to get the time of it.
    *	Restore file pointer to just at the first record.
    * ----------------------------------------------------------------*/
	if(fgetpos(mpFile, &pos ) != 0) 
		return(false);

	if (fread(&VifmPt, sizeof(struct VIFM_DATA_PT1), 1, mpFile) == 0) 
		return(false);

	//mdTimestampOfFirstRecordInFile = DateCvt.MyTimestampToDATETimestamp((double)VifmPt.ulJulianTime);
	mdTimestampOfFirstRecordInFile = m_MyDateTime.MyTimestampToDATETimestamp(VifmPt.dJulianTime);

	if(fsetpos(mpFile, &pos ) != 0) 
		return(false);
               
	if (pstrErrorMsg) 
		pstrErrorMsg->Empty();

	miErrorNum = 0;	// no error
    return(true);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	ReadVBFHeader
//
//	Description:
//	Read date from new VBF file header.  See VIFM Collect Application Version 5.0.8
//	Functional Specification, Verions 0.8 (Working Draft), Date June 21, 2004
//
//	Declaration:
//	bool CVifmDataFile::ReadVBFHeader(const CString &strNameWithPath, CString *pstrErrorMsg)
//
//	Input:	
//			strNameWithPath		filename with full path that is to be opened
//			
//	Output:	pstrErrorMsg	error, if any
//
//			miYr				year read from header
//			miMon				month read from header
//			miDay				day read from header
//
//	Return:	true (header read) / false (some kind of error, see pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	21-Sep-2004	SFK		Created
//////////////////////////////////////////////////////////////////
bool CVifmDataFile::ReadVBFHeader(const CString& strNameWithPath, CString *pstrErrorMsg)
{
	fpos_t pos = 0;
	
   /* ------------------------------------------------------------------
    *	Open the file
    * ----------------------------------------------------------------*/
	if (!OpenDataFile(strNameWithPath, pstrErrorMsg)) 
		return(false);

	// generate an error message in case we get an error in any of the reads,
	// will clear at end of function if all okay
	if (pstrErrorMsg) 
	{
		miErrorNum = iFILE_READ_ERR;
		pstrErrorMsg->Format("Error: Error during read of %s", strNameWithPath);
	}

	return ReadVBFHeader(pos, strNameWithPath, pstrErrorMsg);
}
bool CVifmDataFile::ReadVBFHeader(fpos_t pos, const CString& strNameWithPath, CString *pstrErrorMsg)
{
    char str[54];

   /* ------------------------------------------------------------------
    *	Skip past the first 0x010f bytes to get to the date in the header.
	*	Throw away this information.
    * ----------------------------------------------------------------*/
	pos += 0x0110;
	if(fsetpos(mpFile, &pos )) 
		return(false);
     
   /* ------------------------------------------------------------------
    *	Read year and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, mpFile) != 1) 
		return(false);

	str[4] = '\0';
    miYr = atoi(str);
	miYr4 = miYr;

   /* ------------------------------------------------------------------
    *	Read month and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, mpFile) != 1) 
		return(false);

	str[2] = '\0';
    miMon = atoi(str);

	if ((miMon < 1) || (miMon >12)) 
		return(false);

   /* ------------------------------------------------------------------
    *	Read day and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, mpFile) != 1) 
		return(false);

    str[3] = '\0';
    miDay = atoi(str);

	if ((miDay < 1) || (miDay >31)) 
		return(false);

   /* ------------------------------------------------------------------
    *	Pull the station number out of the filename	29-Sep-2005 SFK
    * ----------------------------------------------------------------*/
	char * ADAM_group;

	CString TempStr = strNameWithPath;
	LPTSTR Filename = TempStr.GetBuffer(255);

	if (strstr(strlwr(Filename), ".vbf") != NULL) 
	{
		ADAM_group = strstr (strupr(Filename), "_GP") + 3;
		msStaNum = (int) ADAM_group [0] & 0x07;
	}
	else 
	{
		ADAM_group = strrchr (Filename, '.') - 3;
		msStaNum = (int) ADAM_group [2] & 0x07;
	}

	if (pstrErrorMsg) 
		pstrErrorMsg->Empty();

	miErrorNum = 0;	// no error
    return(true);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	ReadDataFile
//
//	Description:
//	Read the data from one data file and put it into the database.  Calculate
//	information about the data read such as (first record, last record, number
//	of records read, number of records out of order, number of records taken
//	during offsets, number of records with timestamps not in the day, etc) and
//	report whether the day was overwritten or added to in the database.  Return
//	all information generated to calling routine.
//
//	Declaration:
//	bool CVifmDataFile::ReadDataFile(CDbVista* pDb, const CString &strNameWithPath, CString *pstrMsg)
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
///
//	Return:	true (header read) / false (some kind of error, see pstrErroMsg and miErrorNum)
//	
//  date    /	author	revision
//  -----------------	--------
//	10-Dec-2001	SFK		Created from ImportData in DbImport.cpp
//	10-Jun-2002	SFK		Converted to read files with ms data
//////////////////////////////////////////////////////////////////
bool CVifmDataFile::ReadDataFile(CDbVista* pDb, const CString &strNameWithPath, CString *pstrMsg)
{
    struct VIFM_DATA_PT1 VifmPt;

    int i;
    int iInvalidData;	    		/* number of records not belonging in the day */
    int iOutOfOrder;
    int iGamAdjust;
    char szDateStr[MAX_DT_LEN+1], szFirst[MAX_DT_LEN+1], szLast[MAX_DT_LEN+1];
	char szFirstDate[MAX_DT_LEN+1], szLastDate[MAX_DT_LEN+1];
	char szTempDate[MAX_DT_LEN+1], szTempTime[MAX_DT_LEN+1];
	CString TempStr;	
	DATE  dCurrentPtJulianTime = -1;
	CFacCfgWrap FacCfg;
	bool bStatus;

	mstrFilenameWithPath = strNameWithPath;

	if (strNameWithPath.Find(".bot") > 0) 
	{	// 16-May-2005 SFK this is no longer supported - can be removed
		
	   /* ------------------------------------------------------------------
		*	Read read header information
		* ----------------------------------------------------------------*/
		if (!ReadHeader(strNameWithPath, pstrMsg)) 
		{
			if (mpFile) 
				CloseDataFile();

			return(false);
		}

	   /* ------------------------------------------------------------------
		*	During read of header, got the station number.  Verify from the
		*	Facility Configuration Com that this is a valid station number and
		*	is a VIFM type.
		* ----------------------------------------------------------------*/
	
		if (pDb->mbDatabaseCleared) {	// no framework yet
			pDb->BuildDatabaseFramework(pDb->msFacNum);
			pDb->mbDatabaseCleared = false;
		}

		struct db_sta_rec dbSta;
		bool bExists = FacCfg.GetStationRecord(pDb->msFacNum, msStaNum, &dbSta);
		if (!bExists) 
		{
			if (pstrMsg) 
				pstrMsg->Format("\nError: Skipping file %s with unknown station",m_Dir.StripPathFromFilename(strNameWithPath));
			return(false);
		}
		if (dbSta.s_sta_type != VIFM_TYPE) {
			if (pstrMsg) pstrMsg->Format("\nError: Skipping file %s with unexpected station type %d",m_Dir.StripPathFromFilename(strNameWithPath), dbSta.s_sta_type);
			return(false);
		}
		CString strStationName = dbSta.sz_sta_name;
		CVifmData VInst(pDb, msStaNum, false, m_bQuietMode );

		// 02-Mar-2005 SFK Make sure this station is in the database framework		
		pDb->AddStationToDatabaseFramework(pDb->msFacNum, msStaNum);

		// By the time get here, know we have VIFM data and a valid station number
		// Determine the limits of julian times that belong in this day
		sprintf(szDateStr,"%02d.%02d.%02d", miYr, miMon, miDay); 
		mdDayStart = m_MyDateTime.DateTimeStrsToDATETimestamp(szDateStr, "00:00:00");
		DATE dFirstTimeInNextDay = mdDayStart + 1;

		StartDayInDatabase(&VInst, &i, pstrMsg);
		iInvalidData = 0;
		iOutOfOrder = 0;
		iGamAdjust = 0; 

	   /* ------------------------------------------------------------------
		*	Read records from the raw data file one at a time until all read.
		*	A group of individual records are combined into a single multiple
		*	record in the database.  As each record becomes full, write it
		*	to the database.
		* ----------------------------------------------------------------*/
		while (fread(&VifmPt, sizeof(struct VIFM_DATA_PT1), 1, mpFile) != 0) {
			dCurrentPtJulianTime = m_MyDateTime.MyTimestampToDATETimestamp(VifmPt.dJulianTime);
		   
		   /* ------------------------------------------------------------------
			*   If the point's julian time is not in this day, note it and skip this point.
			* ----------------------------------------------------------------*/
			if ((dCurrentPtJulianTime < mdDayStart) || (dCurrentPtJulianTime >= dFirstTimeInNextDay)) {
				iInvalidData++;
				m_MyDateTime.DATETimestampToDateTimeStrs(dCurrentPtJulianTime, szTempDate, szTempTime, GEN_DTF_IAEA, GEN_DTF_HMSM);
				TempStr.Format("\nInvalid record time in file %s: %14.3lf %s %s",m_Dir.StripPathFromFilename(strNameWithPath), VifmPt.dJulianTime, szTempDate, szTempTime);
				*pstrMsg += TempStr;
				continue;
			}
			
		   /* ------------------------------------------------------------------
			*   If the point is out of order count it.  
			*	If the points are in order then set a new prev point
			* ----------------------------------------------------------------*/
			if (mbFirstPoint == true) mdPrevTime = dCurrentPtJulianTime;
			if ((dCurrentPtJulianTime < mdPrevTime) && (mbFirstPoint == false)) {
				iOutOfOrder++;
				if (mbSkipOutOfOrder == true) continue;
			}
			else 
				mdPrevTime = dCurrentPtJulianTime;   

			
		   /* ------------------------------------------------------------------
			*   The largest point in the day is the last point and the smallest
			*	point in the day is the first point.
			* ----------------------------------------------------------------*/
			if (dCurrentPtJulianTime > mdLastTimeInDay)
				mdLastTimeInDay = dCurrentPtJulianTime;
			if (mbFirstPoint == true) {
				mdFirstTimeInDay = dCurrentPtJulianTime;
				mdFirstTimeInFile = mdFirstTimeInDay;
				mbFirstPoint = false;
			}	
			else {
				if (dCurrentPtJulianTime < mdFirstTimeInDay) {
					mdFirstTimeInDay = dCurrentPtJulianTime;
					mdFirstTimeInFile = mdFirstTimeInDay;
				}
			}

		   /* ------------------------------------------------------------------
			*   If this is the first record of a multiple record (index = 0),
			*   fill in the julian time of the 1st record and the number of
			*	pts in the day so far.
			* ----------------------------------------------------------------*/
			if (i == 0) {	   
				mdbFloatData.d_fdata_beg_time_key = dCurrentPtJulianTime;
				mdbFloatData.ul_fdata_pts_so_far = mulPtsInDaySoFar;
			}

		   /* ------------------------------------------------------------------
			*   Fill the database record.
			*	Reverse the polarity of the authentication status bit
			* ----------------------------------------------------------------*/
			mdbFloatData.d_fdata_time[i]  = dCurrentPtJulianTime;
			mdbFloatData.ul_fdata_status[i] = (unsigned long)(VifmPt.usVifmStatus ^ 0x0002);
			mdbFloatData.d_fdata_etime[i] = VifmPt.dElapsedTime;
			mdbFloatData.f_data_chan1[i]	 = VifmPt.fD1;
			mdbFloatData.f_data_chan2[i]	 = VifmPt.fD2;
			mdbFloatData.f_data_chan3[i]	 = VifmPt.fD3;
			mdbFloatData.f_data_chan4[i]	 = VifmPt.fD4;
			mdbFloatData.f_data_chan5[i]	 = VifmPt.fD5;
			mdbFloatData.f_data_chan6[i]	 = VifmPt.fD6;
			mdbFloatData.f_data_chan7[i]	 = VifmPt.fD7;
			mdbFloatData.f_data_chan8[i]	 = VifmPt.fD8;

			i++;	// count the point just processed

			if (i == NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD) {
			   /* ------------------------------------------------------------------
				*	When have enough data to fill multiple record, create the new
				*	record, fill it and attach to database.
				* ----------------------------------------------------------------*/
				VInst.mdbFloatDataRec = mdbFloatData;
				if (!VInst.AddData(pstrMsg)) {
					miErrorNum = VInst.miErrorNum;
					if (mpFile) CloseDataFile();
					return(false);
				}
				mulPtsInDaySoFar += i;	// accumulate points read so far
				i = 0;
			}
		}

	   /* ------------------------------------------------------------------
		*	Got an error reading the data file.  Are expecting an EOF
		*	error.	If it's anything else, then abort and delete partial
		*	data already in the db.  If it's EOF, close	the raw data file
		*	and continue.
		* ----------------------------------------------------------------*/
		int dum = feof(mpFile);
		if (feof(mpFile) == 0) {
			if (pstrMsg) pstrMsg->Format("\nImport Error Reading File %s.  File Error = %s", mstrFilenameWithPath, strerror(errno));
			if (mpFile) CloseDataFile();
			VInst.DeleteDay(mdDayStart);
			miErrorNum = iFILE_READ_ERR;
			return(false);
		}
		if (mpFile) CloseDataFile();

		FinishDayInDatabase(i, &VInst, pstrMsg);

	   /* ------------------------------------------------------------------
		*	Log which file was just imported successfully.  Include date, station
		*	name, file name and first/last time in the file.
		*	Also log if the day's data was overwritten and if there were
		*	any points out of order or data with invalid times.
		* ----------------------------------------------------------------*/
		if (pstrMsg) 
		{ 
			m_MyDateTime.DATETimestampToDateTimeStrs(mdFirstTimeInFile, szTempDate, szFirst, GEN_DTF_IAEA, GEN_DTF_HMSM);
			m_MyDateTime.DATETimestampToDateTimeStrs(mdLastTimeInDay, szTempDate, szLast, GEN_DTF_IAEA, GEN_DTF_HMSM);		
			TempStr.Format("\n%s  %25s  %s    %s    %s    %5ld", szDateStr, strStationName, m_Dir.StripPathFromFilename(strNameWithPath), szFirst, szLast, mulPtsInDay);
			*pstrMsg += TempStr;
			if (mbOverwrote) 
			{
				TempStr.Format("  Overwrote existing day's data.");
				*pstrMsg += TempStr;
			}
			if (mbDayAlreadyExists) 
			{
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
			if (iGamAdjust > 0) {
				TempStr.Format("  %5d rec(s) during offsets.",iGamAdjust);
				*pstrMsg += TempStr;
			}
		}    
		if (mpFile) CloseDataFile();
		return(true);
	}
	else 
	{	// this is a file in the original vifm format -- no translation to *.bot files

	   /* ------------------------------------------------------------------
		*	Read file using IAEA function
		* ----------------------------------------------------------------*/
		int iStaNum;
		int iNumRecs;
		double dElapsedTime;

		CString strFileName = strNameWithPath;
		int iDum = strFileName.GetLength();

		if (!IAEAReadDataFile(strFileName.GetBuffer(strFileName.GetLength()), &iStaNum, &iNumRecs, &dElapsedTime))
			return(false);

		msStaNum = iStaNum;
		mdTimestampOfFirstRecordInFile = timeStamp[0];
		mdTimestampOfLastRecordInFile = timeStamp[iNumRecs-1];

	   /* ------------------------------------------------------------------
		*	During read of header, got the station number.  Verify from the
		*	Facility Configuration Com that this is a valid station number and
		*	is a VIFM type.
		* ----------------------------------------------------------------*/
		if (pDb->mbDatabaseCleared) {	// no framework yet
			pDb->BuildDatabaseFramework(pDb->msFacNum);
			pDb->mbDatabaseCleared = FALSE;
		}

		struct db_sta_rec dbSta;
		bool bExists = FacCfg.GetStationRecord(pDb->msFacNum, msStaNum, &dbSta);

		if (!bExists) 
		{
			if (pstrMsg) 
				pstrMsg->Format("\nError: Skipping file %s with unknown station",
					m_Dir.StripPathFromFilename(strNameWithPath));

			return(false);
		}
		if (dbSta.s_sta_type != VIFM_TYPE) 
		{
			if (pstrMsg) 
				pstrMsg->Format("\nError: Skipping file %s with unexpected station type %d",
					m_Dir.StripPathFromFilename(strNameWithPath), dbSta.s_sta_type);

			return(false);
		}
		CString strStationName = dbSta.sz_sta_name;
		CVifmData VInst(pDb, msStaNum, false, m_bQuietMode);

		// By the time get here, know we have VIFM data and a valid station number
		mdDayStart = timeStamp[0];
		DATE dFirstTimeInNextDay = mdDayStart + 1;

		bStatus = StartDayInDatabase(&VInst, &i, pstrMsg);
		if (!bStatus && (miErrorNum == iSKIP_FILE)) 
		{
			if (mpFile) 
				CloseDataFile();

			return(false);
		}

		iInvalidData = 0;
		iOutOfOrder = 0;
		iGamAdjust = 0; 

	   /* ------------------------------------------------------------------
		*	Read records from the memory array one at a time until all read.
		*	A group of individual records are combined into a single multiple
		*	record in the database.  As each record becomes full, write it
		*	to the database.
		* ----------------------------------------------------------------*/
		for (int k=0; k<iNumRecs; k++)
		{
			VifmPt.dJulianTime = timeStamp[k];		// IAEA routine converts to DATE
			VifmPt.usVifmStatus = 0;
			VifmPt.fD1 = (float)countBuffer[k][0];
			VifmPt.fD2 = (float)countBuffer[k][1];
			VifmPt.fD3 = (float)countBuffer[k][2];
			VifmPt.fD4 = (float)countBuffer[k][3];
			VifmPt.fD5 = (float)countBuffer[k][4];
			VifmPt.fD6 = (float)countBuffer[k][5];
			VifmPt.fD7 = (float)countBuffer[k][6];
			VifmPt.fD8 = (float)countBuffer[k][7];
			VifmPt.dElapsedTime = dElapsedTime;

			dCurrentPtJulianTime = VifmPt.dJulianTime;
		   
		   /* ------------------------------------------------------------------
			*   If the point's julian time is not in this day, note it and skip this point.
			* ----------------------------------------------------------------*/
			//	Don't do check to see if point is within this day's limits
			//	with vifm because could have multiple days in one file
			
		   /* ------------------------------------------------------------------
			*   If the point is out of order count it.  
			*	If the points are in order then set a new prev point
			* ----------------------------------------------------------------*/
			if (mbFirstPoint) 
				mdPrevTime = dCurrentPtJulianTime;

			if ((dCurrentPtJulianTime < mdPrevTime) && !mbFirstPoint) 
			{
				iOutOfOrder++;
				if (mbSkipOutOfOrder) 
					continue;
			}
			else 
				mdPrevTime = dCurrentPtJulianTime;   

		   /* ------------------------------------------------------------------
			*   Check if this point is in a new day, if yes, then close out the current
			*	day in the database and open up the day this point belongs to
			* ----------------------------------------------------------------*/
			if ((int)VifmPt.dJulianTime != (int)mdDayStart) 
			{
				FinishDayInDatabase(i, &VInst, pstrMsg);
				mdDayStart = (int)VifmPt.dJulianTime;			// get starting time of next day
				bStatus = StartDayInDatabase(&VInst, &i, pstrMsg);

				if (!bStatus && (miErrorNum == iSKIP_FILE)) 
				{
					if (mpFile) 
						CloseDataFile();

					return(false);
				}
			}

		   /* ------------------------------------------------------------------
			*   The largest point in the day is the last point and the smallest
			*	point in the day is the first point.
			* ----------------------------------------------------------------*/
			if (dCurrentPtJulianTime > mdLastTimeInDay)
				mdLastTimeInDay = dCurrentPtJulianTime;

			if (mbFirstPoint) 
			{
				mdFirstTimeInDay = dCurrentPtJulianTime;
				mdFirstTimeInFile = mdFirstTimeInDay;
				mbFirstPoint = false;
			}	
			else 
			{
				if (dCurrentPtJulianTime < mdFirstTimeInDay) 
				{
					mdFirstTimeInDay = dCurrentPtJulianTime;
					mdFirstTimeInFile = mdFirstTimeInDay;
				}
			}

		   /* ------------------------------------------------------------------
			*   If this is the first record of a multiple record (index = 0),
			*   fill in the julian time of the 1st record and the number of
			*	pts in the day so far.
			* ----------------------------------------------------------------*/
			if (i == 0) 
			{	   
				mdbFloatData.d_fdata_beg_time_key = dCurrentPtJulianTime;
				mdbFloatData.ul_fdata_pts_so_far = mulPtsInDaySoFar;
			}

		   /* ------------------------------------------------------------------
			*   Fill the database record.
			*	Reverse the polarity of the authentication status bit
			* ----------------------------------------------------------------*/
			mdbFloatData.d_fdata_time[i]  = dCurrentPtJulianTime;
			mdbFloatData.ul_fdata_status[i] = (unsigned long)(VifmPt.usVifmStatus ^ 0x0002);
			mdbFloatData.d_fdata_etime[i] = VifmPt.dElapsedTime;
			mdbFloatData.f_data_chan1[i]	 = VifmPt.fD1;
			mdbFloatData.f_data_chan2[i]	 = VifmPt.fD2;
			mdbFloatData.f_data_chan3[i]	 = VifmPt.fD3;
			mdbFloatData.f_data_chan4[i]	 = VifmPt.fD4;
			mdbFloatData.f_data_chan5[i]	 = VifmPt.fD5;
			mdbFloatData.f_data_chan6[i]	 = VifmPt.fD6;
			mdbFloatData.f_data_chan7[i]	 = VifmPt.fD7;
			mdbFloatData.f_data_chan8[i]	 = VifmPt.fD8;

			i++;	// count the point just processed

			if (i == NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD) 
			{
			   /* ------------------------------------------------------------------
				*	When have enough data to fill multiple record, create the new
				*	record, fill it and attach to database.
				* ----------------------------------------------------------------*/
				VInst.mdbFloatDataRec = mdbFloatData;

				if (!VInst.AddData(pstrMsg)) 
				{
					miErrorNum = VInst.miErrorNum;
					return(false);
				}
				mulPtsInDaySoFar += i;	// accumulate points read so far
				i = 0;
			}
		}
		// Are finished reading all data in the array
		// Check to see if there was any data in the end of this day that we read out
		// temporarily, if yes repeat the whole process above except this time get the
		// data from the array stored at mpVifmDataPtArray
		if (mbAddedToBeginning) 
		{
		   /* ------------------------------------------------------------------
			*	Read records from the temporary memory storage where they were
			*	put when this day started.
			* ----------------------------------------------------------------*/
			for (int k=0; k<(int)mulPtsToAddToEnd; k++) 
			{
				VifmPt.dJulianTime = VInst.mpVifmDataPtArray[k].mdJulianTime;
				VifmPt.usVifmStatus = (unsigned short)VInst.mpVifmDataPtArray[k].mulStatus;
				VifmPt.fD1 = VInst.mpVifmDataPtArray[k].mfData[0];
				VifmPt.fD2 = VInst.mpVifmDataPtArray[k].mfData[1];
				VifmPt.fD3 = VInst.mpVifmDataPtArray[k].mfData[2];
				VifmPt.fD4 = VInst.mpVifmDataPtArray[k].mfData[3];
				VifmPt.fD5 = VInst.mpVifmDataPtArray[k].mfData[4];
				VifmPt.fD6 = VInst.mpVifmDataPtArray[k].mfData[5];
				VifmPt.fD7 = VInst.mpVifmDataPtArray[k].mfData[6];
				VifmPt.fD8 = VInst.mpVifmDataPtArray[k].mfData[7];
				VifmPt.dElapsedTime = VInst.mpVifmDataPtArray[k].mdElapsedTime;

				dCurrentPtJulianTime = VifmPt.dJulianTime;
			   
			   /* ------------------------------------------------------------------
				*   If the point's julian time is not in this day, note it and skip this point.
				* ----------------------------------------------------------------*/
				//	Don't do check to see if point is within this day's limits
				//	with vifm because could have multiple days in one file
				
			   /* ------------------------------------------------------------------
				*   If the point is out of order count it.  
				*	If the points are in order then set a new prev point
				* ----------------------------------------------------------------*/
				if (mbFirstPoint) 
					mdPrevTime = dCurrentPtJulianTime;

				if ((dCurrentPtJulianTime < mdPrevTime) && !mbFirstPoint) 
				{
					iOutOfOrder++;
					if (mbSkipOutOfOrder) 
						continue;
				}
				else 
					mdPrevTime = dCurrentPtJulianTime;   

			   /* ------------------------------------------------------------------
				*   Check if this point is in a new day, if yes, then close out the current
				*	day in the database and open up the day this point belongs to
				* ----------------------------------------------------------------*/
				if ((int)VifmPt.dJulianTime != (int)mdDayStart) 
				{
					FinishDayInDatabase(i, &VInst, pstrMsg);
					mdDayStart = (int)VifmPt.dJulianTime;			// get starting time of next day
					bStatus = StartDayInDatabase(&VInst, &i, pstrMsg);

					if (!bStatus && (miErrorNum == iSKIP_FILE)) 
					{
						if (mpFile) 
							CloseDataFile();

						return(false);
					}
				}

			   /* ------------------------------------------------------------------
				*   The largest point in the day is the last point and the smallest
				*	point in the day is the first point.
				* ----------------------------------------------------------------*/
				if (dCurrentPtJulianTime > mdLastTimeInDay)
					mdLastTimeInDay = dCurrentPtJulianTime;

				if (mbFirstPoint) 
				{
					mdFirstTimeInDay = dCurrentPtJulianTime;
					mdFirstTimeInFile = mdFirstTimeInDay;
					mbFirstPoint = false;
				}	
				else 
				{
					if (dCurrentPtJulianTime < mdFirstTimeInDay) 
					{
						mdFirstTimeInDay = dCurrentPtJulianTime;
						mdFirstTimeInFile = mdFirstTimeInDay;
					}
				}

			   /* ------------------------------------------------------------------
				*   If this is the first record of a multiple record (index = 0),
				*   fill in the julian time of the 1st record and the number of
				*	pts in the day so far.
				* ----------------------------------------------------------------*/
				if (i == 0) 
				{	   
					mdbFloatData.d_fdata_beg_time_key = dCurrentPtJulianTime;
					mdbFloatData.ul_fdata_pts_so_far = mulPtsInDaySoFar;
				}

			   /* ------------------------------------------------------------------
				*   Fill the database record.
				*	Reverse the polarity of the authentication status bit
				* ----------------------------------------------------------------*/
				mdbFloatData.d_fdata_time[i]  = dCurrentPtJulianTime;
				mdbFloatData.ul_fdata_status[i] = (unsigned long)(VifmPt.usVifmStatus ^ 0x0002);
				mdbFloatData.d_fdata_etime[i] = VifmPt.dElapsedTime;
				mdbFloatData.f_data_chan1[i]	 = VifmPt.fD1;
				mdbFloatData.f_data_chan2[i]	 = VifmPt.fD2;
				mdbFloatData.f_data_chan3[i]	 = VifmPt.fD3;
				mdbFloatData.f_data_chan4[i]	 = VifmPt.fD4;
				mdbFloatData.f_data_chan5[i]	 = VifmPt.fD5;
				mdbFloatData.f_data_chan6[i]	 = VifmPt.fD6;
				mdbFloatData.f_data_chan7[i]	 = VifmPt.fD7;
				mdbFloatData.f_data_chan8[i]	 = VifmPt.fD8;

				i++;	// count the point just processed

				if (i == NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD) 
				{
				   /* ------------------------------------------------------------------
					*	When have enough data to fill multiple record, create the new
					*	record, fill it and attach to database.
					* ----------------------------------------------------------------*/
					VInst.mdbFloatDataRec = mdbFloatData;

					if (!VInst.AddData(pstrMsg)) 
					{
						miErrorNum = VInst.miErrorNum;
						return(false);
					}
					mulPtsInDaySoFar += i;	// accumulate points read so far
					i = 0;
				}
			}
		}
		// wrap up this day in the database
		FinishDayInDatabase(i, &VInst, pstrMsg);

	   /* ------------------------------------------------------------------
		*	Log which file was just imported successfully.  Include date, station
		*	name, file name and first/last time in the file.
		*	Also log if the day's data was overwritten and if there were
		*	any points out of order or data with invalid times.
		* ----------------------------------------------------------------*/
		if (pstrMsg) 
		{ 
			m_MyDateTime.DATETimestampToDateTimeStrs(timeStamp[0], szFirstDate, szFirst, GEN_DTF_IAEA, GEN_DTF_HMSM);
			m_MyDateTime.DATETimestampToDateTimeStrs(timeStamp[iNumRecs-1], szLastDate, szLast, GEN_DTF_IAEA, GEN_DTF_HMSM);		
			TempStr.Format("\n%s  %25s  %s    %s %s    %s %s    %5ld", szFirstDate, strStationName, m_Dir.StripPathFromFilename(strNameWithPath), szFirstDate, szFirst, szLastDate, szLast, iNumRecs);
			*pstrMsg += TempStr;

			if (mbOverwrote) 
			{
				TempStr.Format("  Overwrote existing day's data.");
				*pstrMsg += TempStr;
			}
			if (mbDayAlreadyExists) 
			{
				TempStr.Format("  Added to existing day's data.");
				*pstrMsg += TempStr;
			}
			if (mbAddedToBeginning) 
			{
				TempStr.Format("  Added to beginning of existing day's data.");
				*pstrMsg += TempStr;
			}
			if (iOutOfOrder > 0) 
			{
				TempStr.Format("  %5d pts out of order.", iOutOfOrder);
				*pstrMsg += TempStr;
			}
			if (iInvalidData > 0) 
			{
				TempStr.Format("  %5d rec(s) with invalid times.",iInvalidData);
				*pstrMsg += TempStr;
			}
			if (iGamAdjust > 0) 
			{
				TempStr.Format("  %5d rec(s) during offsets.",iGamAdjust);
				*pstrMsg += TempStr;
			}
		}    
		if (mpFile) 
			CloseDataFile();

		return(true);
	}

}


///////////////////////////////////////////////////////////////////////////
//	Name:	FinishDayInDatabase
//
//	Description:
//	All data that belong to the current day are now in the database.
//	Fill out the existing data record with 0s and write it to database.
//	Read and update the day record to describe this day.  (If no data
//	for day, delete the record)
//
//	Declaration:
//	bool CVifmDataFile::FinishDayInDatabase(int i, CVifmData *pVInst, CString *pstrMsg)
//
//	Input:	
//			i					number of records filled in the multiple reord so far
//			pVInst				pointer to instrument
//			mulPtsInDaySoFar	number of data points in the day so far
//			mdbFloatData		array containing all the data in the current data record
//			mdDayStart			timestamp of 00:00 for this day
//			mdFirstTimeInDay	timestamp of first data record for this day
//			mdLastTimeInDay		timestamp of last data record for this day
//			
//	Output:	pstrErrorMsg	error, if any
//			miErrorNum			problem during read
//				iFILE_READ_ERROR
//				iSKIP_FILE
//				iDB_BAD
//			mulPtsInDay			total number of data points in the entire day
//
//
//	Return:	true (data written successfully) / false (some kind of error, see pstrErroMsg and miErrorNum)
//	
//  date    /	author	revision
//  -----------------	--------
//	29-May-2003	SFK		Extracted from ReadDataFile to be a separate function.
//////////////////////////////////////////////////////////////////
bool CVifmDataFile::FinishDayInDatabase(int i, CVifmData *pVInst, CString *pstrMsg)
{

	struct db_day_rec	dbDay;			/* database record structure */ 
	
	//------------------------------------------------------------------
	//	All data for this day is now in the database.  Fill the remainder
	//	of the data records with data values of 0 and write the data record
	//	into the database.
	//------------------------------------------------------------------
	mulPtsInDay = mulPtsInDaySoFar + i;
	if (i > 0) {
		while (i < NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD) {
			mdbFloatData.d_fdata_time[i] = 0;
			i++;
		}
		pVInst->mdbFloatDataRec = mdbFloatData;
		if (!pVInst->AddData(pstrMsg)) {
			miErrorNum = pVInst->miErrorNum;
			return(false);
		}

	}

	//------------------------------------------------------------------
	//	If there was no data for this day, then delete any day record
	//	that was added at the start of reading the file.
	//------------------------------------------------------------------
    if (mulPtsInDay == 0) {
		pVInst->DeleteDay(mdDayStart);
		return(true);
    }

	// ------------------------------------------------------------------
	//	Some data from this day in database - now update the day record
    //	describing this day.
    //	First, read the existing day record.  Then fill in the new information
	//	that we now know and write the updated day record in the database
    // ----------------------------------------------------------------
	int iStatus = pVInst->GetDayData(mdDayStart, &dbDay);
	if (iStatus != iDAY_IN_DB) {
		miErrorNum = iStatus;
		return (false);
	}
    dbDay.ul_day_total_pts = mulPtsInDay;
    dbDay.d_day_end_time = mdLastTimeInDay;
	dbDay.d_day_beg_time = mdFirstTimeInDay;
	iStatus = pVInst->AddDayData(&dbDay);

	return(true);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	StartDayInDatabase
//
//	Description:
//	All data that belong to the current day are now in the database.
//	Fill out the existing data record with 0s and write it to database.
//	Read and update the day record to describe this day.  (If no data
//	for day, delete the record)
//
//	Declaration:
//	bool CVifmDataFile::StartDayInDatabase(CVifmData *pVInst, int *piIndex, CString *pstrMsg)
//
//	Input:	
//			pVInst				pointer to instrument

//			mulPtsInDaySoFar	number of data points in the day so far
//			mdbFloatData		array containing all the data in the current data record
//			mdDayStart			timestamp of 00:00 for this day
//			mdFirstTimeInDay	timestamp of first data record for this day
//			mdLastTimeInDay		timestamp of last data record for this day
//			
//	Output:	pstrErrorMsg	error, if any
//			miErrorNum			problem during read
//				iFILE_READ_ERROR
//				iSKIP_FILE
//				iDB_BAD
//			mulPtsInDay			total number of data points in the entire day
//
//
//	Return:	true (data written successfully) / false (some kind of error, see pstrErroMsg and miErrorNum)
//	
//  date    /	author	revision
//  -----------------	--------
//	29-May-2003	SFK		Extracted from ReadDataFile to be a separate function.
//////////////////////////////////////////////////////////////////
bool CVifmDataFile::StartDayInDatabase(CVifmData *pVInst, int *piIndex, CString *pstrMsg)
{
	struct db_day_rec	dbDay;			/* database record structure */ 
	CString TempStr;	
	bool bDayAlreadyExists = false;
	char szDateStr[MAX_DT_LEN+1], szDum[MAX_DT_LEN+1];
	//CMyDateTime DateCvt;

	// ------------------------------------------------------------------
    //	Check if day record for this day already exists in database
    // -----------------------------------------------------------------*/
	DB_D_INTERVAL DayInDb, DumInterval;
	mulPtsInDaySoFar = 0;
//sk	mbOverwrite = false;
	int iStatus = pVInst->DayExists(mdDayStart, &mulPtsInDaySoFar, &DayInDb);
	if (iStatus == iDB_BAD) {
		miErrorNum = iStatus;
		return(false);
	}
                                                          
	// ------------------------------------------------------------------
    //	If data from this day is already in the database, several things
	//	can happen:  1) if the new data falls after the existing data, then
	//	the data are just appended to the end of the data already in the db
	//	2) if all the data in the file falls before the existing data, then
	//	read out all the old data into another memory array and rewrite the entire day
	//	3) if the new data overlaps with some of the data in the database, 
	//	if requested with mbOverwrite = false, display a question asking the
	//	user whether or not he wants to overwrite the existing data for the day.
	//	If user answers yes, then delete any existing data in the day.
    // ----------------------------------------------------------------
	mbDayAlreadyExists = false;
    if (iStatus == iDAY_IN_DB) 
	{  // day exists in db
		// check to see if can just add to the end of the existing day; this is allowed
		if (mdTimestampOfFirstRecordInFile > DayInDb.dEnd) // will add to end of day
		{
			mbDayAlreadyExists = true;
		}
		// check to see if can add to the beginning of the existing day; this is allowed
		else if (mdTimestampOfLastRecordInFile < DayInDb.dStart) 
		{	// will add to beginning of day
			iStatus = pVInst->GetDataStatistics(DayInDb, &DumInterval);	// pts how many pts in day in mulPtsActual
			if (iStatus && (pVInst->mulNumPtsActual > 0)) 
			{
				pVInst->mulNumPtsRequested = pVInst->mulNumPtsActual;
				pVInst->GetStationData(DayInDb, &DumInterval);	// puts pts in mpVifmDataPtArray
				mulPtsToAddToEnd = pVInst->mulNumPtsActual;		// how many pts are in the mpVifmDataPtArray
				// since have read the end of the day out now can delete the day
   				iStatus = pVInst->DeleteDay(mdDayStart);

				if (iStatus != iDAY_IN_DB) 
				{
					miErrorNum = iStatus;
					return(false);	
				}
				mbAddedToBeginning = true;
			}
		}
		// data overlaps some of the data in the day, can't just add to beginning or end
		else 
		{
			if (!mbOverwrite) 
			{		// display the question about overwriting data based on mbOverwrite
				m_MyDateTime.DATETimestampToDateTimeStrs(mdDayStart, szDateStr, szDum, GEN_DTF_IAEA, GEN_DTF_HMS);
        		TempStr.Format("File %s contains data from %s which already exists in database.  Do you want to overwrite data from this file?",
						mstrFilenameWithPath, szDateStr);

				if (IDNO == MessageBox(NULL, TempStr, "VifmCom: Data Already In Database", MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)) 
				{
					// user answered no, put a message in the return string to indicate will skip file
					if (pstrMsg) pstrMsg->Format("\nSkipping file %s (date=%02d.%02d.%02d): files's data already in database", 
									 		mstrFilenameWithPath, miYr, miMon, miDay);
					miErrorNum = iSKIP_FILE;
					return(false);
				}
			}
			// want to overwrite any existing data in the day, so delete the day.
   			iStatus = pVInst->DeleteDay(mdDayStart);

			if (iStatus != iDAY_IN_DB) 
			{
				miErrorNum = iStatus;
				return(false);	
			}
   			mbOverwrote = true;
		}
	}	
		
	// ------------------------------------------------------------------
    //	If day is not already in db (mbDayAlreadyExists = false), then
	//	create a day record for mdDayStart in the database and initialize
	//	all parameters used by ReadData.
	//	if day is already in the db (mbDayAlreadyExists = true) then
	//	read existing day record from the database and set all parameters
	//	used by ReadData from the day record.  Also read out the partial
	//	data record that has been filled and find where the data records are
	//	in it.  
    // ----------------------------------------------------------------
	if (!mbDayAlreadyExists) 
	{
		if (!pVInst->CreateDay(mdDayStart)) 
		{
			miErrorNum = pVInst->miErrorNum;
			return(false);
		}
		*piIndex = 0;
		mdPrevTime = 0.0;
		mdLastTimeInDay = 0.0;        
		mulPtsInDaySoFar = 0;
		mbFirstPoint = true;
		pVInst->mbFillingExistingRec = false;
	}
	else 
	{ // data exists, adding to day already in db
		// read the record containing information about the day so far from the db
		iStatus = pVInst->GetDayData(mdDayStart, &dbDay);

		if (iStatus != iDAY_IN_DB) 
		{
				miErrorNum = iStatus;
				return(false);	
		}
		mulPtsInDaySoFar = dbDay.ul_day_total_pts;

		// get the partially filled data record from the db and decide where to start
		// filling in the new data
		if (!pVInst->GetMultipleDbRecord(DayInDb.dEnd, piIndex, &mdbFloatData, BEFORE)) 
		{
			miErrorNum = pVInst->miErrorNum;
			return(false);
		}
		// decide whether there is still room to add data to this record	
		if (*piIndex == (NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD-1)) 
		{
			*piIndex = 0;
			pVInst->mbFillingExistingRec = false;
		}
		else 
		{
			(*piIndex)++;
			pVInst->mbFillingExistingRec = true;
			mulPtsInDaySoFar = mulPtsInDaySoFar - *piIndex;
		}

		mdPrevTime = dbDay.d_day_end_time;
		mdLastTimeInDay = DayInDb.dEnd;		// dbDay.ul_day_end_time;
		mdFirstTimeInFile = mdTimestampOfFirstRecordInFile;
		mdFirstTimeInDay = DayInDb.dStart;	// dbDay.ul_day_beg_time;
		mbFirstPoint = false;
	}
	return(true);
}
/***********************************************************************************************\
*																								*
*	PROGRAM: 	VIFM CDM ANALYSIS TOOL  														*
*																								*
*	MODULE: 	VIFM FILE READ.C  -  source code for VIFM file procedures and housekeeping   	*
*																								*
*	Author: 	C. Liguori - IAEA - SGTIE   													*
*																								*
*	This program has been developed using National Instruments CVI ver 6.0  					*
*	reference files:    																		*
*				VIFC Analysis.prj		Project file											*
*				VIFC Analysis.c  		main module - user interface call back functions		*
*				VIFC Data Analyze.c 	Data Analysis source code								*
*				VIFC Setup.c    		Parameter Setup Procedures source code  				*
*				VIFC Analysis.uir   	User interface resource file							*
*				VIFC Analysis.h 		User interface resource header  						*
*																								*
*																								*
*	Last Edit: 	2003-03-20		  																*
*																								*
*	History:	2002-10-07 - release of version 1.0 											*
*				2002-11-28:	hide second progress indicator during file index creation			*
*				2003-03-20:	hide second progress indicator during file reading for graph 		*
*							display																*
*																								*
\***********************************************************************************************/

//#define MAX_PATHNAME_LEN 256
#define BURST_RECORD_LENGTH 35000	  // number of records read in each file read operation
#define VIFM_RECORD_LENGTH 40		//pjm 1/19/06: SCR00226
#define VIFM_TO_LANL_DATE_OFFSET (4*365+2)*24*3600   /* base for VIFM system is 1904-01-01 00:00:00 											and not 1899-12-30 00:00 as for COleDateTime class format */ 
#define BASE_TIME (4*365)*24*3600	  /* base for VIFM system is 1904-01-01 00:00:00 
											and not 1900 as for time_t class */ 
// prototypes
static void IntByteSwap    (unsigned char * , int);
static void DoubleByteSwap (unsigned char * , int);
static void CheckTimeInterval  (void);
void SetInfo (void);

static char readBuffer [BURST_RECORD_LENGTH * 40];  //35000*40 = 1,400,000
static FILE * fileIn; 

static char inputFileName [_MAX_PATH];
//static char inputFileExtension[4];
static double inputTimeStamp;
static long lTemp;
static long maxInputFileSize;
static long maxInputFileRecords;
static int nFile;
static int currentFileIdx;

static int recordIndex;

static double firstTime, lastTime;

typedef struct 
{
    char    name [_MAX_PATH];
    double  iniTime;
    long    recordNumber;
    double  samplingInterval;
} inputFileElement;

static inputFileElement inputFileIndex [1000];
static inputFileElement rTemp;


/*----------------------------------------------------------------

--------------------     IntByteSwap      ------------------------

This static procedure converts from Motorola notation to Intel notation
an array of elementNumber 32-bit integers. The conversion is 
accomplished 'in place'.
Input:
	unsigned void * pointer : pointer to the integer array
	int elementNumber : number of element in the array
						
Output:
	none

-----------------------------------------------------------------*/

static void IntByteSwap (unsigned char * pointer, int elementNumber)
{
	int j, jX4;
	unsigned char tempStorage1;

    for (j = 0; j < elementNumber; j++)
    {
		jX4 = j*4;

        tempStorage1 = pointer [jX4];
        pointer [jX4] = pointer [jX4 + 3];
        pointer [jX4 + 3] = tempStorage1;
        
        tempStorage1 = pointer [jX4 + 1];
        pointer [jX4 + 1] = pointer [jX4 + 2];
        pointer [jX4 + 2] = tempStorage1;
    
    }
}


/*----------------------------------------------------------------

------------------     DoubleByteSwap      -----------------------

This static procedure converts from Motorola notation to Intel notation
an array of elementNumber 64-bit floating point. The conversion is 
accomplished 'in place'.
Input:
	unsigned void * pointer : pointer to the double array
	int elementNumber : number of element in the array
						
Output:
	none

-----------------------------------------------------------------*/

static void DoubleByteSwap (unsigned char * pointer, int elementNumber)
{
	int j,jX8;
	unsigned char tempStorage1;

    for (j = 0; j < elementNumber; j++)
    {
		jX8 = j*8;
        tempStorage1 = pointer [jX8];
        pointer [jX8] = pointer [jX8 + 7];
        pointer [jX8 + 7] = tempStorage1;
        
        tempStorage1 = pointer [jX8 + 1];
        pointer [jX8 + 1] = pointer [jX8 + 6];
        pointer [jX8 + 6] = tempStorage1;
        
        tempStorage1 = pointer [jX8 + 2];
        pointer [jX8 + 2] = pointer [jX8 + 5];
        pointer [jX8 + 5] = tempStorage1;
        
        tempStorage1 = pointer [jX8 + 3];
        pointer [jX8 + 3] = pointer [jX8 + 4];
        pointer [jX8 + 4] = tempStorage1;
    }
}


/*----------------------------------------------------------------

--------------------     ReadDataFile      -----------------------

This procedure reads in a specific VIFM raw data file.
Input:
	inputFileName : pointer to the string specifying the full path to the input file

Output:
	returns FALSE if file doesn't exist or cannot be opened
	returns TRUE  if successful
	
	station_ID 		: the group ID of the ADAM that produced the file (sl1 --> 1, sl3 --> 3, etc.)
	recordsNumber	: number of records actually read in
	samplingInterval : actual time interval between subsequent point, in case of 'normal' data
	
	timeStamp	[MAX_RECORDS] : array of double - time stamp of the recordsNumber data point read in (days since midnight 1899-12-30)
	elapsedTime	[MAX_RECORDS] : array of double - validity of the record (sec up to 256 times the nominal sampling interval)
	countBuffer	[8] [MAX_RECORDS]: array of integer - actual count rate for each channel and each data records	: 
	
-----------------------------------------------------------------*/

#define SEC_IN_A_DAY 86400

int CVifmDataFile::IAEAReadDataFile (
	char * inputFileName, 
	int * station_ID, 
	int * numberOfRecords, 
	double * samplingInterval)
{
	int i, k, numrec;
	char * ADAM_group;
	double dTemp1, dTempSampling;
	HANDLE hFile;
	DWORD dwFileSizeInBytes;  //unsigned long

	hFile = CreateFile (inputFileName,  // file to open
                   		GENERIC_READ,   // open for reading
                   		FILE_SHARE_READ | FILE_SHARE_WRITE,  // share mode
                   		NULL,    		// default security attributes
                   		OPEN_EXISTING,  // disposition
                   		0,       		// file attributes
                   		NULL);   		// don't copy any file's attributes

	if (hFile == INVALID_HANDLE_VALUE)    
		return FALSE;     // we can't open the file...
	
	dwFileSizeInBytes = GetFileSize(hFile, 0);
	CloseHandle(hFile);

	if ((dwFileSizeInBytes == 0xFFFFFFFF) || 
		(dwFileSizeInBytes > (MAX_RECORD * VIFM_RECORD_LENGTH))) //12 million
	{
		return FALSE;
	}

    recordIndex = 0;

	//fileIn = fopen (inputFileName, "rb");
    //if (fileIn == NULL) 
    //	return FALSE;

	mpFile = fopen (inputFileName, "rb");
    if (mpFile == NULL) 
    	return FALSE;

	fpos_t pos = 0;  //__int64
	long lFileOffset = 0;
	long lUsableFileLength = 0;	//pjm 1/19/06: SCR00226
	char str[5];
	int iNumberOfRecords = 0;

	// 16-May-2005 SFK Add read past 3072 (0x0c00) bytes of header if *.vbf extension (new format)
	if (strstr(strlwr(inputFileName), ".vbf") != NULL) 
	{
		// 23-May-2005 SFK Look for VIFM in the first four characters, if it isn't there then assume
		// the file is signed with SnF.
		//int iNum = fread (str, 4, 1, fileIn);
		int iNum = fread (str, 4, 1, mpFile);
		str[4] = '\0';    	// Add EOS

		bool bIsSigned = false;   // BMEND-124

		//pjm 1/19/06: begin most significant changes for SCR00226
		if (strcmp(str, "VIFM") != 0) // this is a signed file  // BMEND-124 jfl 6/29/10: ??? but see the Grand checksig section 
		{	
			unsigned char *publicKey, *signatureTimestamp; //not used

			//Use CheckSignatureEx() because the return is testable.
			int iStatus = CheckSignatureEx (inputFileName, &lFileOffset, &lUsableFileLength,  
				&publicKey, &signatureTimestamp );

			// iStatus interpretation (From Cesare's InLineVerifier.c file)
			//0 = Successfully verified
			//1 = File is CORRUPTED - signature is invalid
			//3 = Verified OK, but CA authority is unknown
			//-14 = Input file has no S/MIME format       // BMEND-124
			// All others are failures.
			if ((iStatus == 0) || (iStatus == 3))
			{
				pos = lFileOffset;
				bIsSigned = true;   // BMEND-124
				// calculate number of usable records in the signed file
				iNumberOfRecords = (lUsableFileLength - 0x0c00)/VIFM_RECORD_LENGTH;   // BMEND-124
			}
			else if (iStatus == -14)   // BMEND-124
			{
				// drop out of this attempt and move forward, it's probably an OK unsigned file
			}
			else
			{
				//bail - verification failed
				//fclose (fileIn);
				fclose (mpFile);
				return FALSE;
			}
		}

		if (!bIsSigned) //not signed   // BMEND-124
		{
			// calculate number of usable records in the unsigned file
			iNumberOfRecords = (dwFileSizeInBytes - 0x0c00)/VIFM_RECORD_LENGTH;
		}
		//pjm 1/19/06: end most significant changes for SCR00226

		ReadVBFHeader(pos,inputFileName,NULL);
		pos += BYTES_IN_HEADER; //0x0c00;
		fsetpos(mpFile, &pos);
	}
	else  //not *.vbf
	{
		pos = 0;
		iNumberOfRecords = (dwFileSizeInBytes - 0x000)/VIFM_RECORD_LENGTH;
	}

	int index;

    for (i = 0; !feof(mpFile); i += k)
    {
        numrec = fread (
			readBuffer,				//1,400,000 !!
			VIFM_RECORD_LENGTH,		//40
			BURST_RECORD_LENGTH,	//35000
			mpFile);

        for (k = 0; k < numrec; k++)
        {
            if (recordIndex >= MAX_RECORD)  //500,000 
				break;

			if (recordIndex >= iNumberOfRecords) 	//pjm 1/19/06: SCR00226
				break;
        
			index = k * VIFM_RECORD_LENGTH; //k * 40

            memcpy (&timeStamp[recordIndex], &readBuffer [index], 8);
            memcpy (&countBuffer[recordIndex][0], &readBuffer [index + 8], 32);
            
            DoubleByteSwap ((unsigned char *) &timeStamp[recordIndex], 1);
            IntByteSwap ((unsigned char *) &countBuffer[recordIndex][0], 8);
			// converts now from VIFM based time (1904-1-1) to OLE format (base 1899-12-30 00:00)
            timeStamp [recordIndex] += VIFM_TO_LANL_DATE_OFFSET;  
			// converts from sec to day based time&date            
			timeStamp [recordIndex] /= SEC_IN_A_DAY;
			
            recordIndex++;

        }
    }

    fclose(mpFile);
    
	// 16-May-2005 SFK Add read past 3072 (0x0c00) bytes of header if *.vbf extension (new format)
	// 19-Jan-2006 PJM Added protection code so that this will not crash program if the name of the
	//                 file does not contain the "_GP" character string.
	if (strstr(strlwr(inputFileName), ".vbf") != NULL) 
	{
		char *cfirst = strstr (strupr(inputFileName), "_GP");

		if (cfirst != NULL)
		{
			ADAM_group = cfirst + 3;//strstr (strupr(inputFileName), "_GP") + 3;
			(*station_ID) = (int)ADAM_group[0] & 0x07;
		}
		else
		{
			*station_ID = -1; //malformed filename
		}
	}
	else // extension is other than ".vbf"
	{
		ADAM_group = strrchr (inputFileName, '.') - 3;
		(*station_ID) = (int) ADAM_group [2] & 0x07;
	}

	//pjm 1/19/06: SCR00226 (Replaced the line here because it made it so
	//the last 2 records of the file were never processed.  Found the coding
	//error while walking through the new code for SCR00226.
	// the original code was: 
	//	(*recordsNumber) = recordIndex - 1;
	// should have been:
	//	(*recordsNumber) = recordIndex + 1;
	// Note that iNumberOfRecords is the same as recordIndex + 1.
    (*numberOfRecords) = iNumberOfRecords;	//pjm 1/19/06: SCR00226
    
    dTempSampling = 10;
   
    if (iNumberOfRecords > 120)
	    for (k = iNumberOfRecords - 2; k >= 100; k-=100)
	    {
	        dTemp1 = (timeStamp [k] - timeStamp [k - 100]) * (SEC_IN_A_DAY / 100);
	        if ((dTemp1 < dTempSampling) && (dTemp1 > .1)) 
	            dTempSampling = dTemp1;
	    }
	else
		dTempSampling = (timeStamp [iNumberOfRecords - 1] - timeStamp [0]) / iNumberOfRecords;
    
    (*samplingInterval) =  dTempSampling;
    
    for (k = 0; k < iNumberOfRecords - 1; k++)
    {
        elapsedTime [k] = timeStamp [k + 1] - timeStamp [k] * SEC_IN_A_DAY;
        if (elapsedTime [k] > ((*samplingInterval) * 256))
        	elapsedTime [k] = ((*samplingInterval) * 256);
    }
    elapsedTime [k] = elapsedTime [k - 1];

    SpikeCut (*samplingInterval, iNumberOfRecords);  // clear single point spikes in the raw data file

    return TRUE;
}


/*----------------------------------------------------------------

--------------------     TimeToString      -----------------------

This procedure coverts the VIFM date and time stamp value into a text 
string in the format YYYY-MM-DD hh:mm:ss.n

-----------------------------------------------------------------*/


void CVifmDataFile::TimeToString (double timeStamp, char * timeString)
{
	//<<QA>> this could cause a stomp if timeString space is not allocated prior.
	struct tm tm1;
	time_t time1;

    timeStamp *= SEC_IN_A_DAY;
    time1 = (time_t) (timeStamp - VIFM_TO_LANL_DATE_OFFSET + BASE_TIME);
    tm1 = (*gmtime (&time1));
    timeStamp = timeStamp - (time_t) timeStamp;
	strftime (timeString, 40, "%Y-%b-%d %H:%M:%S", &tm1);
	sprintf (&timeString [strlen (timeString)], ".%d", (int) (timeStamp * 10.));
}


/*----------------------------------------------------------------

--------------------     SpikeCut      -----------------------

This procedure removes from the raw data files the single point spikes
usually due to the well known ADAM deficiency in handling the counter 
reading when close to a 10k boundary for the counter content

-----------------------------------------------------------------*/

#define STATISTIC_CUT 4.5

/*
First I check if data are on a plateau (first two points).
If so and if third point shows a difference greater than 8000 counts 
and if the fourth point is still on a line with the first two points
then replace the third point with the averaged content of the two adiacent points.
In case of count rates > 2e5 I don't care removing any spike
*/

//pjm 1/19/06: Clarified as a result of adding SnF functionality for SCR00226
// There are no functional changes, only variable name changes.
void CVifmDataFile::SpikeCut(double dSamplingInterval, int iNumberOfRecords)
{
	int i, j;
	int sigma, remainder, delta;
    
    for (j = 0; j < 8; j++)
    {
	    for (i = 0; i < iNumberOfRecords - 4; i++)
	    {
	        delta =  abs ((int) countBuffer [i + 2] [j] - (int) countBuffer [i] [j]);
        
	        if (delta > (8000/dSamplingInterval))
	        {
	            sigma = (unsigned int) (sqrt (countBuffer [i + 1] [j] * dSamplingInterval) 
	            	* STATISTIC_CUT / dSamplingInterval);
	            remainder = abs (long(((countBuffer [i + 3] [j] + (int) (5000. / dSamplingInterval)) % 
	            	(int) (10000. / dSamplingInterval)) - (int) (5000. / dSamplingInterval)));
	            if ((abs ((int) countBuffer [i + 1] [j] - (int) countBuffer [i] [j]) < sigma) && 
	                (countBuffer [i + 1] [j] < 200000) &&
	                ((remainder < sigma) || (delta > 20000)) &&
	                (abs ((int) countBuffer [i + 3] [j] - (int) countBuffer [i + 1] [j]) < sigma))
	            {
	                    countBuffer [i + 2] [j] = (countBuffer [i + 1] [j] 
	                    			+ countBuffer [i + 3] [j]) / 2;
                    
	            }
	        }
	    }
    }
    
    return;
}
///////////////////////////////////////////////////////////////////////////
//	Name:	GetCompleteFileList
//
//	Description:
//	This function was added to CGrandDataFile to recursively retrieve a list of *.BID files in
//	a directory.  It reads the file header to populate the CList of tFileRecords that is converted to
//	a safe array used by ImportManager.  
//
//	Declaration:
//	void CVifmDataFile::GetCompleteFileList(short FacilityID, CList<tFileRecord, tFileRecord> *pFileList, 
//									   CString Directory,  bool IncludeSubdirs)
//
//	Input:	
//			FacilityID					number of records filled in the multiple reord so far
//			Directory				Directory of where to conduct the search (cannot end in \)
//			IncludeSubdirs	number of data points in the day so far
//			mdbFloatData		array containing all the data in the current data record
//			mdDayStart			timestamp of 00:00 for this day
//			mdFirstTimeInDay	timestamp of first data record for this day
//			mdLastTimeInDay		timestamp of last data record for this day
//			
//	Output:	CList<tFileRecord, tFileRecord> *pFileList	error, if any
//			miErrorNum			problem during read
//				iFILE_READ_ERROR
//				iSKIP_FILE
//				iDB_BAD
//			mulPtsInDay			total number of data points in the entire day
//
//
//	Return:	true (data written successfully) / false (some kind of error, see pstrErroMsg and miErrorNum)
//	
//  date    /	author	revision
//  -----------------	--------
//	xx-Jun-2005	hn		This was flagrantly stolen from Kelly Michel and modified to
//						search for and read in *.BID files.
//	06-Jul-2005	SFK		Copied from GrandDataFile.cpp and modified to support *.VBF files.  Names modified
//						to indicate types.
//////////////////////////////////////////////////////////////////
void CVifmDataFile::GetCompleteFileList(
	short sFacilityID, 
	CList<tFileRecord, tFileRecord> *pFileList, 				   
	const CString& Directory,  
	bool bIncludeSubdirs)
{
	USES_CONVERSION;

	CFileFind Finder;
	BOOL bWorking;
	CString  FileName;
	tFileRecord FileRecord;

	// Append *.* to the directory passed in
	CString DirWithFileMask;
	if (Directory.Right(1) == "\\") DirWithFileMask.Format("%s*.*", Directory);	// 06-Jul-2005 SFK Check for trailing slash
	else DirWithFileMask.Format("%s\\*.*", Directory);
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
					if(Finder.IsDirectory() && bIncludeSubdirs)
					{
						//Recurse.
						GetCompleteFileList(sFacilityID, pFileList, Finder.GetFilePath(), bIncludeSubdirs);
					}
					else //if(Finder.IsNormal())
					{
 						FileName = Finder.GetFileName(); 
						CString Ext = FileName.Mid(FileName.GetLength() - 3, 3);
						if(!Ext.CompareNoCase("VBF"))
						{
							FileRecord.File.bstrVal = (Finder.GetFilePath()).AllocSysString(); /// add path and filename

							//****************************************************************
							//Open the file and get info on the data in the file.  Load that
							//file data into the FileRecord structure.
							//****************************************************************
							CString err;
							//If we are not able to read the *.VBF header, we fail
							if (!ReadVBFHeader (W2T(FileRecord.File.bstrVal),&err))
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



