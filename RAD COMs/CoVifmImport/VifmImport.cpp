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
// VifmImport.cpp : Implementation of CVifmImport
#include "stdafx.h"
#include "CoVifmImport.h"
#include "VifmImport.h"
#include "VifmDataFile.h"
#include "VifmDbVista.h"
#include "VifmData.h"
#include "SafeArrayUtil.h"
#include "VifmImportSupport.h"
#include "VistaBaseData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CVifmDbVista Db;
CVifmImportSupport ImportSupport;

#define Currtask m_Db.m_Currtask  //NDAR artifact.  May or may not need for B3

//	Read the specified data file and add to the data store.
// 27-Oct-2004	SFK	Added db open, if needed.  If successful, facility-specific database is open.
STDMETHODIMP CVifmImport::AddData(
	short sFacilityID, 
	BSTR bstrFilename, 
	VARIANT_BOOL bOverwrite, 
	BSTR *bstrResult, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	*pvbResult = VARIANT_FALSE;

	CString strResult;
	CVifmDataFile File(false);//m_bQuietMode);
	File.mbOverwrite = (bOverwrite != VARIANT_FALSE);

	if (!Db.mbDatabaseOpen) 
		Db.OpenTheDatabase(sFacilityID);

	if (Db.mbDatabaseOpen) 
	{
		Db.msFacNum = sFacilityID;
		CString cs(bstrFilename);
		bool b  = File.ReadDataFile(&Db, cs, &strResult);
		*pvbResult = b?VARIANT_TRUE:VARIANT_FALSE;
	}
	else 
	{
		strResult = "Could not open database for VIFM Import";
	}
	
	*bstrResult = T2BSTR(LPCSTR(strResult));
	Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



//	Common COM function to backup any COM specific files to a designated location
// 27-Oct-2004	SFK	Added sFacilityID implementation and db open, if necessary.  If successful,
//					facility-specific database is open.
STDMETHODIMP CVifmImport::BackupDataStore(
	short sFacilityID, 
	BSTR bstrDestinationPath, 
	VARIANT_BOOL vbQuestion, 
	VARIANT_BOOL vbDefault, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bQuestion = (vbQuestion != VARIANT_FALSE);
	bool bDefault = (vbDefault != VARIANT_FALSE);

	*pvbResult = VARIANT_FALSE;

	CString strDestPath(bstrDestinationPath);

	if (!Db.mbDatabaseOpen) 
		Db.OpenTheDatabase(sFacilityID);

	if (Db.mbDatabaseOpen) 
	{
		bool b = Db.BackupDatabase(strDestPath, bQuestion, bDefault);
		*pvbResult = b?VARIANT_TRUE:VARIANT_FALSE;
	}
	Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



//	Common COM function to do any special actions to close the data store
STDMETHODIMP CVifmImport::CloseDataStore()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	Db.CloseDatabase();

	return S_OK;
}


// 06-Jul-2005 SFK	Copied from GrandImport.cpp and modified for VIFM
STDMETHODIMP CVifmImport::CreateNewDataStore(short sFacilityID, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pbResult = false;

	//This function was also added to the VifmDbVista class and is called whenever
	//a call to open a data store fails because the data store does not exist.
	//Determine the empty data store and destination path for the facility's data store
	//location, then copy the contents of the empty db to the place it should be.
	// The result is whether or not this copy was successful. hn 6/8/2005
	CString emptyDBPath, dataStorePath;
	emptyDBPath = ImportSupport.GetEmptyDBLocation();
	dataStorePath = ImportSupport.GetVifmDataStorePath (sFacilityID);
	
	CDirUtilities Dir(false);  //QUIET MODE IS ALWAYS OFF FOR VIFM pjm 11/27/2007 for B2R1
	//Copy the empty db, return the success of the copy operation.
	bool bResult = (Dir.CopyDirectory (emptyDBPath,dataStorePath,TRUE));
	*pbResult = bResult;


	return S_OK;
}



//	Gets the data for the given channel for the given interval. Assumes storage is done by caller.
//	26-Oct-2004	Major changes to support returning data in a VARIANT struct rather than two arrays
STDMETHODIMP CVifmImport::GetDataByChannel(short sFacilityID, short sStationID, short sChannelID, DATE StartTime, DATE EndTime, VARIANT *pvData, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pbResult = false;

	CVifmData GInst(&Db, sStationID, false,false);//m_bQuietMode);
	DB_D_INTERVAL Requested, Actual;
	double *pdTimestamps = NULL;
	double *pdData = NULL;


	Db.msFacNum = sFacilityID;

	if (!Db.mbDatabaseOpen) Db.OpenTheDatabase(sFacilityID);
	if (Db.mbDatabaseOpen) {
		Requested.dStart = StartTime;
		Requested.dEnd = EndTime;
		GInst.mulNumPtsRequested = pvData->lVal;
		if (GInst.mulNumPtsRequested > 0) {
			pdTimestamps = new double[GInst.mulNumPtsRequested];
			if (pdTimestamps) pdData = new double[GInst.mulNumPtsRequested];
			if (pdData) *pbResult = GInst.GetChannelData(sChannelID, Requested, &Actual, pdTimestamps, pdData);
			if (*pbResult) {
				
				//Create VARIANT vector array - a doubly dimensioned array of timestamps and data.
				pvData->vt = VT_VARIANT | VT_ARRAY; 
				SAFEARRAYBOUND Dim[2];
				Dim[0].cElements = GInst.mulNumPtsActual + 1;		// number of rows in the array plus TimestampOfNextData
				Dim[0].lLbound = 0;		// index starts at 0
				Dim[1].cElements = 2;	// number of columns (timestamp and data)
				Dim[1].lLbound = 0;
				pvData->parray = SafeArrayCreate(VT_VARIANT,2,Dim);
				long lDimension[2];
				unsigned long ul;
				bool bIsDate = true;
			
				for (ul=0; ul<GInst.mulNumPtsActual; ul++) 
				{
					lDimension[0] = ul;
					SA_PutElem(pvData->parray, ul, 0L, pdTimestamps[ul], bIsDate);
					SA_PutElem(pvData->parray, ul, 1L, pdData[ul]);
				}
				// fill in the Next Data point time as the last element in the array
				SA_PutElem(pvData->parray, ul, 0L, GInst.mdTimestampOfNextData, bIsDate);
				
				// are finished with these so can delete
				if (pdTimestamps) delete [] pdTimestamps;
				if (pdData) delete [] pdData;
			}
		}
	}
	Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



//	Gets the data for the given station for the given interval.  SAFEARRAY for results is allocated here.
//	27-Oct-2004	SFK	Added putting NextTimestamp as last element of SAFEARRAY
STDMETHODIMP CVifmImport::GetDataByStation(
	short sFacilityID, 
	short sStationID, 
	DATE StartTime, DATE 
	EndTime, 
	VARIANT* pvData, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CVifmData GInst(&Db, sStationID, false,false);//m_bQuietMode);
	DB_D_INTERVAL Requested, Actual;

	*pvbResult = VARIANT_FALSE;

	if (!Db.mbDatabaseOpen) 
		Db.OpenTheDatabase(sFacilityID);

	if (Db.mbDatabaseOpen) 
	{
		Db.msFacNum = sFacilityID;
		Requested.dStart = StartTime;
		Requested.dEnd = EndTime;
		GInst.mulNumPtsRequested = pvData->lVal;

		if (GInst.mulNumPtsRequested <= 0) 
			return S_OK;		// no points requested

		*pvbResult = GInst.GetStationData(Requested, &Actual);

		CVifmDbDataPt *pTemp = GInst.mpVifmDataPtArray; // storage allocated in GetStationData

		if (*pvbResult) 
		{	// extract info

			//Create VARIANT vector array - a doubly dimensioned array.
			pvData->vt = VT_VARIANT | VT_ARRAY; 
			SAFEARRAYBOUND Dim[2];
			Dim[0].cElements = GInst.mulNumPtsActual+1;		// number of rows in the array plus Next time
			Dim[0].lLbound = 0;		// index starts at 0
			Dim[1].cElements = 11;	// number of columns (number of field in a single record)
			Dim[1].lLbound = 0;
			pvData->parray = SafeArrayCreate(VT_VARIANT,2,Dim);
			long lDimension[2];
			unsigned long ul;
			bool bIsDate = true;
		
			for (ul=0; ul<GInst.mulNumPtsActual; ul++) 
			{
				lDimension[0] = ul;

				//pdTimestamp[ul] = pTemp[ul].mdJulianTime;
				SA_PutElem(pvData->parray, ul, 0L, (double)pTemp[ul].mdJulianTime,bIsDate);

				SA_PutElem(pvData->parray, ul, 1L, (long)pTemp[ul].mulStatus);
				SA_PutElem(pvData->parray, ul, 2L, (float)pTemp[ul].mfData[0]);
				SA_PutElem(pvData->parray, ul, 3L, (float)pTemp[ul].mfData[1]);
				SA_PutElem(pvData->parray, ul, 4L, (float)pTemp[ul].mfData[2]);
				SA_PutElem(pvData->parray, ul, 5L, (float)pTemp[ul].mfData[3]);
				SA_PutElem(pvData->parray, ul, 6L, (float)pTemp[ul].mfData[4]);
				SA_PutElem(pvData->parray, ul, 7L, (float)pTemp[ul].mfData[5]);
				SA_PutElem(pvData->parray, ul, 8L, (float)pTemp[ul].mfData[6]);
				SA_PutElem(pvData->parray, ul, 9L, (float)pTemp[ul].mfData[7]);
				SA_PutElem(pvData->parray, ul, 10L, (double)pTemp[ul].mdElapsedTime,bIsDate);

			}
			// put the time of the next data point as a double in the last row in the array
			SA_PutElem(pvData->parray, ul, 0L, (double)GInst.mdTimestampOfNextData,bIsDate);
		}
	}
	Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



//	Gets the amount of data in the given interval for the given station.
// 26-Oct-2004	SFK	Added db open, if needed
STDMETHODIMP CVifmImport::GetDataStatistics(
	short sFacilityID, 
	short sStationID, 
	short sChannelID, 
	DATE StartTime, 
	DATE EndTime, 
	DATE *pActualStartTime, 
	DATE *pActualEndTime, 
	long *plNumMembers, 
	VARIANT_BOOL *pvbResult)

{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pvbResult = VARIANT_FALSE;
	bool bSuccess = false;

	CVifmData GInst(&Db, sStationID, false,false);//m_bQuietMode);
	DB_D_INTERVAL Requested, Actual;

	*pActualStartTime = 0;
	*pActualEndTime = 0;
	*plNumMembers = 0;

	if (!Db.mbDatabaseOpen) 
		Db.OpenTheDatabase(sFacilityID);

	if (Db.mbDatabaseOpen) 
	{
		Db.msFacNum = sFacilityID;
		Requested.dStart = StartTime;
		Requested.dEnd = EndTime;

		bool b = GInst.GetDataStatistics(Requested, &Actual);
	
		*pvbResult = b?VARIANT_TRUE:VARIANT_FALSE;
	
		if (b) 
		{	// extract info
			*pActualStartTime = Actual.dStart;
			*pActualEndTime = Actual.dEnd;
			*plNumMembers = GInst.mulNumPtsActual;
		}
	}
	Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



//	Common COM function to get the current data store path
// 27-Oct-2004	SFK	Changed name from GetDataStore -- added sFacilityID argument.  Also changed
//					to return path with COM directories appended
STDMETHODIMP CVifmImport::GetDataStorePath(short sFacilityID, BSTR* bstrStorePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	*bstrStorePath = A2BSTR("");

	if (!Db.mbDatabaseOpen) 
		Db.OpenTheDatabase(sFacilityID);

	if (Db.mbDatabaseOpen)
		*bstrStorePath = T2BSTR(LPCSTR(Db.mstrDatabasePath));

	Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}

//	Common COM function to get the size of the VIFM database
// 27-Oct-2004	SFK	Modified to support opening database
STDMETHODIMP CVifmImport::GetDataStoreSize(short sFacilityID, double *pdSizeInBytes, VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CDirUtilities Dir(false);  //QUIET MODE IS ALWAYS OFF FOR VIFM pjm 11/27/2007 for B2R1

	*pvbResult = VARIANT_FALSE;
	*pdSizeInBytes = 0;

	if (!Db.mbDatabaseOpen) 
		Db.OpenTheDatabase(sFacilityID);

	if (Db.mbDatabaseOpen) 
	{
		Dir.GetDirectorySize(Db.mstrDatabasePath, pdSizeInBytes);
		*pvbResult = VARIANT_TRUE;
	}
	Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



//	Get the time of the beginning and ending of the data store for the given station.
// 27-Oct-2004	SFK	Modified to support opening database
STDMETHODIMP CVifmImport::GetDataStoreTimeRange(short sFacilityID, short sStationID, VARIANT_BOOL vbErrDisplay, DATE *pFirstTimestamp, DATE *pLastTimestamp, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bHaveAnswers = false;
	bool bErr = (vbErrDisplay != VARIANT_FALSE);

	Db.msFacNum = sFacilityID;
	
	CVifmData GInst(&Db, sStationID, false,false);//m_bQuietMode);

	//default return values to errors
	*pFirstTimestamp = 0;
	*pLastTimestamp = 0;
	*pbResult = VARIANT_FALSE;

	if (!Db.mbDatabaseOpen) 
		Db.OpenTheDatabase(sFacilityID);

	if (!Db.mbDatabaseOpen)
		return S_OK;

	bHaveAnswers = GInst.GetFirstTimeInDb(pFirstTimestamp, bErr);

	if (!bHaveAnswers)
	{
		*pFirstTimestamp = 0;
		*pLastTimestamp = 0;
	}
	else
	{ 
		bHaveAnswers = GInst.GetLastTimeInDb(pLastTimestamp, bErr);
		if (!bHaveAnswers)
		{
			*pFirstTimestamp = 0;
			*pLastTimestamp = 0;
		}
	}

	//*pbResult = bStatus?VARIANT_TRUE:VARIANT_FALSE;
	*pbResult = VARIANT_TRUE;

	return S_OK;
}



//	Gets day statistics for the specified time range.
// 26-Oct-2004	SFK	Modified to support opening database
STDMETHODIMP CVifmImport::GetDayStatistics(short sFacilityID, short sStationID, DATE StartTime, DATE EndTime, DATE *pActualStartTime, DATE *pActualEndTime, long *plNumMembers, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CVifmData GInst(&Db, sStationID, false,false);//m_bQuietMode);

	DB_D_INTERVAL Requested, Actual;

	Db.msFacNum = sFacilityID;

	*pActualStartTime = 0;
	*pActualEndTime = 0;
	*plNumMembers = 0;
	*pbResult = false;

	if (!Db.mbDatabaseOpen) Db.OpenTheDatabase(sFacilityID);

	if (Db.mbDatabaseOpen) {
		Requested.dStart = StartTime;
		Requested.dEnd = EndTime;
		*pbResult = GInst.GetDayStatistics(Requested, &Actual);
		if (*pbResult) {	// extract info
			*pActualStartTime = Actual.dStart;
			*pActualEndTime = Actual.dEnd;
			*plNumMembers = GInst.mulNumPtsActual;
		}
	}
	Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



//	Gets day data for the specified time range.  SAFEARRAY of results is allocated here.
// 27-Oct-2004	Changed function name from GetDataByDay and eliminate plNumMembers argument since can get from SAFEARRAY info
STDMETHODIMP CVifmImport::GetDaySummaryData(short sFacilityID, short sStationID, DATE StartTime, DATE EndTime, VARIANT *pvDay, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CVifmData GInst(&Db, sStationID, false,false);//m_bQuietMode);
	DB_D_INTERVAL Requested, Actual;
	DayDataStruct *pDay = NULL;
	unsigned long ul;

	*pbResult = false;

	if (!Db.mbDatabaseOpen) Db.OpenTheDatabase(sFacilityID);

	if (Db.mbDatabaseOpen) {
		Db.msFacNum = sFacilityID;
		Requested.dStart = StartTime;
		Requested.dEnd = EndTime;
		*pbResult = GInst.GetDayStatistics(Requested, &Actual);	// number of members in interval
		GInst.mulNumPtsRequested = GInst.mulNumPtsActual;
		if ((*pbResult) && (GInst.mulNumPtsActual > 0)) {

			pDay = new DayDataStruct[GInst.mulNumPtsActual];
			if (pDay) {
				*pbResult = GInst.GetDayData(Requested, &Actual, pDay);

				if (*pbResult) {	// extract info

					//Create VARIANT vector array - a doubly dimensioned array.
					pvDay->vt = VT_VARIANT | VT_ARRAY; 
					SAFEARRAYBOUND Dim[2];
					Dim[0].cElements = GInst.mulNumPtsActual;		// number of rows in the array
					Dim[0].lLbound = 0;		// index starts at 0
					Dim[1].cElements = 3;	// number of columns (number of field in a single record)
					Dim[1].lLbound = 0;
					pvDay->parray = SafeArrayCreate(VT_VARIANT,2,Dim);
					long lDimension[2];
				
					for (ul=0; ul<GInst.mulNumPtsActual; ul++) {
						lDimension[0] = ul;

						lDimension[1] = 0;
						CComVariant v((double)pDay[ul].dFirstDataPt);
						SafeArrayPutElement(pvDay->parray,lDimension,&v);

						lDimension[1] = 1;
						CComVariant v1((double)pDay[ul].dLastDataPt);
						SafeArrayPutElement(pvDay->parray,lDimension,&v1);
							
						lDimension[1] = 2;
						CComVariant v2((double)pDay[ul].dNumPtsInDay);
						SafeArrayPutElement(pvDay->parray,lDimension,&v2);
					}
				}
			}
			if (pDay) delete [] pDay;
		}
	}
	Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



// Get list of all files this COM knows about in the specified directory(ies)
// 27-Oct-2004	SFK	Added interface but did not implement
// 06-Jul-2005	SFK	Copied from GrandImport.cpp
STDMETHODIMP CVifmImport::GetFilesList(short sFacilityID, BSTR bstrRootDir, VARIANT_BOOL bIncludeSubDirs, VARIANT_BOOL bMostRecent, VARIANT *psaFilesList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Added implementation 6/8/05 to recursively search folders for *.BID files.
	// After the search is complete with FileFinder, we add each *.BID file to a list
	// that is used by ImportManager.  To determine the file date, we use the CGrandDataFile
	// ReadHeader function and populate the list of FileRecords that is passed as a safe
	// array to the ImportManager interface calls.  This was also flagrantly stolen
	// from Kelly Michel. hn 6/8
	CString Msg;

	SAFEARRAY *pSA;
	SAFEARRAYBOUND bounds[2];
	long lDimension[2];
	HRESULT hr;
	POSITION Pos;
	tFileRecord FileRecord;
	CList<tFileRecord, tFileRecord> FileList;
	VARIANT_BOOL bResult = true;

	//Initialize safearray.
	::VariantInit(psaFilesList);

	// 5-Jul-2005 SFK Added check for whether DB is open as may make calls later on
	if (!Db.mbDatabaseOpen) 
		Db.OpenTheDatabase(sFacilityID);

	if (Db.mbDatabaseOpen) 
	{
		USES_CONVERSION;
		CVifmDataFile VifmFile(false);//m_bQuietMode);
		//The VifmDataFile class encapsulates the search function for us.
		CString cs(bstrRootDir);
		VifmFile.GetCompleteFileList(sFacilityID, &FileList, cs, bIncludeSubDirs);

		//If the request is for the most recent data, then determine what the latest date of the data
		//in the database is.

		//If there were files found, then load the contents of the linked list into the 
		//SAFEARRAY.  If there are no files, then set the VARIANT to indicate the array is EMPTY.
		if(FileList.GetCount())
		{
			long LastStationID = -1;
			VARIANT_BOOL Success;
			DATE StartTime, EndTime;
			
			//Determine how many files will go into the list that is output.  If the MostRecent
			//flag is enabled, then check the file list for the time of the data.
			int iNumFilesForList = bMostRecent ? 0 : FileList.GetCount(); 
			
			if(bMostRecent)	// special case, have to count up all files including the last day and beyond
			{
				Pos = FileList.GetHeadPosition();
				while(Pos)
				{
					FileRecord = FileList.GetNext(Pos);

					if(FileRecord.StationID.lVal != LastStationID)
					{
						LastStationID = FileRecord.StationID.lVal;
						GetDataStoreTimeRange(sFacilityID, FileRecord.StationID.lVal, false, &StartTime, 
							&EndTime, &Success);
					}
				//	if(!bMostRecent || (bMostRecent && FileRecord.Date.date > EndTime))	// 06-Jul-2005 SFK Changed logic to following line
					if ((int)FileRecord.Date.date >= (int)EndTime)	// 06-Jul-2005 SFK Need to include the last day in the database
						iNumFilesForList++;

				}//end WHILE Pos != NULL
			}

			//Set the dimensions of the array and create it.
			bounds[0].cElements = iNumFilesForList;  //Number of rows or number of files.
			bounds[0].lLbound = 0;
			bounds[1].cElements = 3;  //Number of columns or number of elements in each file record.
			bounds[1].lLbound = 0;
			
			psaFilesList->vt = VT_ARRAY | VT_VARIANT;
			pSA = SafeArrayCreate(VT_VARIANT, 2, bounds);
			
			//******************************************************************************
			//Load every file that was accumulated in the list into a SAFEARRAY for output.
			//******************************************************************************
			lDimension[0] = 0; //Row
			Pos = FileList.GetHeadPosition();

			while(Pos)
			{
				FileRecord = FileList.GetNext(Pos);
				if(bMostRecent && FileRecord.StationID.lVal != LastStationID)
				{
					LastStationID = FileRecord.StationID.lVal;
					GetDataStoreTimeRange(sFacilityID, FileRecord.StationID.lVal, false, &StartTime, 
						&EndTime, &Success);
				}

			//	if(!bMostRecent || (bMostRecent && FileRecord.Date.date > EndTime))	// 06-Jul-2005 SFK Changed logic to following line
				if ((int)FileRecord.Date.date >= (int)EndTime)	// 06-Jul-2005 SFK Need to include the last day in the database

				{
					//*****************************
					//Load the FILE DATA DATE
					//*****************************
					lDimension[1] = 0;	//COLUMN
					hr = SafeArrayPutElement(pSA, lDimension, &FileRecord.Date); 
					if(hr != S_OK)
					{
						Msg.Format("Failed to load DATE array element [%d][0]", lDimension[0]);
						MessageBox(NULL, Msg, "Error PutElement", MB_OK);
					}
					
					//******************************
					//Load the STATION ID
					//******************************
					lDimension[1] = 1;	//COLUMN
					hr = SafeArrayPutElement(pSA, lDimension, &FileRecord.StationID); 
					if(hr != S_OK)
					{
						Msg.Format("Failed to load STATION ID array element [%d][1]", lDimension[0]);
						MessageBox(NULL, Msg, "Error PutElement", MB_OK);
					}
					
					//*****************************
					//Load the FILE NAME
					//*****************************
					lDimension[1] = 2;	//COLUMN
					hr = SafeArrayPutElement(pSA, lDimension, &FileRecord.File); 
					if(hr != S_OK)
					{
						Msg.Format("Failed to load FILE NAME array element [%d][2]", lDimension[0]);
						MessageBox(NULL, Msg, "Error PutElement", MB_OK);
					}
					lDimension[0]++;  //Row
				}
	//			SysFreeString(FileRecord.File.bstrVal);
			}//end WHILE Pos != NULL
			
			
			//Release the locks on the data in the array.
			SafeArrayUnaccessData(pSA);
			
			//Set the return value
			psaFilesList->parray = pSA;
		}
		else //No files found.
			psaFilesList->vt = VT_EMPTY;
	}
	Db.CloseDatabase();			// 6-Jul-2005 SFK	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}




// 27-Oct-2004	SFK	Added interface but did not implement
STDMETHODIMP CVifmImport::GetParameters(short sFacilityID, VARIANT* pvParms, VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pvbResult = VARIANT_FALSE;

	return S_OK;
}



//	Common COM function to do any special action to initialize a data store
//	26-Oct-2004	SFK	Added database open, if necessary.  Also changed calling argument to have FacilityID rather the db path
STDMETHODIMP CVifmImport::InitializeDataStore(short sFacilityID, VARIANT_BOOL bQuestion, VARIANT_BOOL *pbResult)
//STDMETHODIMP CVifmImport::InitializeDataStore(BSTR bstrRootPath, VARIANT_BOOL bQuestion, VARIANT_BOOL *pbSuccess)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	*pbResult = VARIANT_FALSE;
	
	if (!Db.mbDatabaseOpen) Db.OpenTheDatabase(sFacilityID);
	if (Db.mbDatabaseOpen) {
		*pbResult = Db.InitializeDatabase(Db.mstrDatabaseRootPath, bQuestion);
	}
	Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



// Common COM function to do any special action to open a data store
//	26-Oct-2004	SFK	Changed calling argument from path to FacilityID
STDMETHODIMP CVifmImport::OpenDataStore(short sFacilityID, VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pvbResult = VARIANT_FALSE;

	if (!Db.mbDatabaseOpen) 
		Db.OpenTheDatabase(sFacilityID);

	if (Db.mbDatabaseOpen) 
	{
		*pvbResult = VARIANT_TRUE;//true;
		Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	}
	return S_OK;
}



//	Common COM function to restore any COM specific files from a designated location
// 26-Oct-2004	SFK	Changed to have sFacilityID instead of bstrDestPath in arguments, opened db if necessary
STDMETHODIMP CVifmImport::RestoreDataStore(
	short sFacilityID, 
	BSTR bstrSourcePath, 
	VARIANT_BOOL bQuestion, 
	VARIANT_BOOL bDefault, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	*pvbResult = VARIANT_FALSE;
	
	if (!Db.mbDatabaseOpen) 
		Db.OpenTheDatabase(sFacilityID);

	if (Db.mbDatabaseOpen) 
	{
		CString strSrcPath(bstrSourcePath);
		CString strDestPath = Db.mstrDatabaseRootPath;
		bool b = Db.RestoreDatabase(strSrcPath, strDestPath, bQuestion, bDefault);
		*pvbResult = b?VARIANT_TRUE:VARIANT_FALSE;
	}
	Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}

// parameters are in this order in the array:  
//		boolean for clear db question,
//		boolean for ask overwrite question,
//		boolean for whether to overwrite in backup or restore if no question asked.
// Currently there are no station specific parameters -- all are general.
STDMETHODIMP CVifmImport::SetParameters(
	short sFacilityID, 
	VARIANT *pvParms, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	VARIANT Var;
	::VariantInit(&Var);
	bool bSuccess = false;

	if (pvParms->vt == (VT_VARIANT | VT_ARRAY)) 	// is the type of SafeArray we are expecting
	{ 
		long lUpperBound;
		long lDimension[2];
		SafeArrayGetUBound(pvParms->parray, 1, &lUpperBound);

		for (long j = 0; j < lUpperBound + 1; j++) 
		{
			lDimension[0] = j;

			// boolean indicating whether to ask the clear db question during db initialization			
			lDimension[1] = 0L;
			SafeArrayGetElement(pvParms->parray, lDimension, &Var);

			// boolean indicating whether to ask the overwrite existing db when copying
			lDimension[1] = 1L;
			SafeArrayGetElement(pvParms->parray, lDimension, &Var);

			// boolean indicating whether to overwrite or not if the overwrite question is not asked.
			lDimension[1] = 2L;
			SafeArrayGetElement(pvParms->parray, lDimension, &Var);
		}
		bSuccess = true;
	}
	
	*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;
	::VariantClear(&Var);
	return S_OK;
}









