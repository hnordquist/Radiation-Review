// BinaryImport.cpp : Implementation of CBinaryImport
#include "stdafx.h"
#include "CoBinaryImport.h"
#include "BinaryImport.h"
#include "AccessBinaryDb.h"
#include "BinaryDataFile.h"
#include "BinaryData.h"
#include "BinaryImportSupport.h"
#include "SafeArrayUtil.h"

/////////////////////////////////////////////////////////////////////////////
// CBinaryImport

//	AddData - Read the specified data file and add to the data store.
// 27-Oct-2004	SFK	Added db open, if needed.  If successful, facility-specific database is open.
STDMETHODIMP CBinaryImport::AddData(short sFacilityID, BSTR bstrFilename, VARIANT_BOOL vbOverwrite, BSTR *bstrResult, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	//compiler warning
	//*pbResult = false;
	*pbResult = VARIANT_FALSE;

	CString strResult;
	CBinaryDataFile File(m_bQuietMode);
	File.mbOverwrite = (vbOverwrite != VARIANT_FALSE);

	if (!m_Db.mbDatabaseOpen) m_Db.OpenTheDatabase(sFacilityID);
	if (m_Db.mbDatabaseOpen) {
		m_Db.msFacNum = sFacilityID;
		*pbResult = File.ReadDataFile(&m_Db, W2T(bstrFilename), &strResult);	// sk 04/15/2003
	}
	else {
		strResult = "Could not open database for Binary Import";
	}
	
	*bstrResult = T2BSTR(LPCSTR(strResult));
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	
	return S_OK;
}



//	BackupDataStore - Common COM function to backup any COM specific files to a designated location
// 26-Oct-2004	SFK	Added sFacilityID implementation and db open, if necessary.  If successful,
//					facility-specific database is open.
STDMETHODIMP CBinaryImport::BackupDataStore(short sFacilityID, BSTR bstrDestinationPath, VARIANT_BOOL bQuestion, VARIANT_BOOL bDefault, VARIANT_BOOL *pbResult)
//STDMETHODIMP CBinaryImport::BackupDataStore(BSTR bstrDestinationPath, VARIANT_BOOL bQuestion, VARIANT_BOOL bDefault, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	*pbResult = VARIANT_FALSE;

	CString strDestPath = W2T(bstrDestinationPath);

	if (!m_Db.mbDatabaseOpen) 
		m_Db.OpenTheDatabase(sFacilityID);

	if (m_Db.mbDatabaseOpen) 
	{
		bool b = m_Db.BackupDatabase(strDestPath, (bool)bQuestion, (bool)bDefault);
		*pbResult = b ? VARIANT_TRUE :VARIANT_FALSE;
	}
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



//	CloseDataStore - Common COM function to do any special actions to close the data store
STDMETHODIMP CBinaryImport::CloseDataStore()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

 	m_Db.CloseDatabase();

	return S_OK;
}



STDMETHODIMP CBinaryImport::CreateNewDataStore(short sFacilityID, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pbResult = VARIANT_FALSE;//false;

	CBinaryImportSupport ImportSupport;

	// 11-Jul-2005 SFK Copied from Heather's GRAND Import stuff and modified for binary
	//This function was also added to the GrandDbVista class and is called whenever
	//a call to open a data store fails because the data store does not exist.
	//Determine the empty data store and destination path for the facility's data store
	//location, then copy the contents of the empty db to the place it should be.
	// The result is whether or not this copy was successful. hn 6/8/2005
	CString emptyDBPath, dataStorePath;
	emptyDBPath = ImportSupport.GetEmptyDBLocation();
	dataStorePath = ImportSupport.GetBinaryDataStorePath (sFacilityID);
	
	CDirUtilities Dir(m_bQuietMode);  //QUIET MODE IS ALWAYS OFF FOR the non-NDAR Binary Component pjm 11/27/2007 for B2R1

	//Copy the empty db, return the success of the copy operation.
	bool bResult = (Dir.CopyDirectory (emptyDBPath,dataStorePath,TRUE));
	//*pbResult = bResult;
	*pbResult = bResult ? VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}



//	26-Oct-2004	Major changes to support returning data in a VARIANT struct rather than two arrays
STDMETHODIMP CBinaryImport::GetDataByChannel(short sFacilityID, short sStationID, short sChannelID, DATE StartTime, DATE EndTime, VARIANT *pvData, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bResult = false;

	int iStationType = BINARY_INST;
	bool bInterpolated = true;
	if ((sStationID != -1) && (sChannelID >= 0)) bInterpolated = true;	// are looking for a specific channel
	CBinaryData BInst(&m_Db, (int)sStationID, (int)sChannelID, iStationType, bInterpolated,m_bQuietMode);	// sk 04/15/2003

	DB_D_INTERVAL Requested, Actual;
	double *pdTimestamps = NULL;
	double *pdData = NULL;

	m_Db.msFacNum = sFacilityID;

	if (!m_Db.mbDatabaseOpen) m_Db.OpenTheDatabase(sFacilityID);

	if (m_Db.mbDatabaseOpen) 
	{
		Requested.dStart = StartTime;
		Requested.dEnd = EndTime;
		BInst.m_ulNumPtsRequested = pvData->lVal;

		if (BInst.m_ulNumPtsRequested > 0) 
		{
			pdTimestamps = new double[BInst.m_ulNumPtsRequested];
			pdData = new double[BInst.m_ulNumPtsRequested];

			if (pdTimestamps && pdData) 
			{
				bResult = BInst.GetChannelData(BInst.m_iChanNum, Requested, &Actual, pdTimestamps, pdData);
				if (bResult) 
				{
					
					//Create VARIANT vector array - a doubly dimensioned array of timestamps and data.
					pvData->vt = VT_VARIANT | VT_ARRAY; 
					SAFEARRAYBOUND Dim[2];
					Dim[0].cElements = BInst.m_ulNumPtsActual + 1;		// number of rows in the array plus TimestampOfNextData
					Dim[0].lLbound = 0;		// index starts at 0
					Dim[1].cElements = 2;	// number of columns (timestamp and data)
					Dim[1].lLbound = 0;
					pvData->parray = SafeArrayCreate(VT_VARIANT,2,Dim);
					long lDimension[2];
					long ul;

					bool bIsDate = true;
				
					for (ul=0; ul<(long)BInst.m_ulNumPtsActual; ul++) 
					{
						lDimension[0] = ul;
						SA_PutElem(pvData->parray, ul, 0L, pdTimestamps[ul],bIsDate);
						SA_PutElem(pvData->parray, ul, 1L, pdData[ul]);
					}
					// fill in the Next Data point time as the last element in the array
					SA_PutElem(pvData->parray, ul, 0L, BInst.m_dTimestampOfNextData, bIsDate);
				}
			}
		}
	}
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM

	delete [] pdTimestamps;
	delete [] pdData;
	*pbResult = bResult?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}



//	Gets the data for the given station for the given interval.  SAFEARRAY for results is allocated here.
//	26-Oct-2004	SFK	Added putting NextTimestamp as last element of SAFEARRAY
STDMETHODIMP CBinaryImport::GetDataByStation(short sFacilityID, short sStationID, DATE StartTime, DATE EndTime, VARIANT *pvData, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	int iStationType = BINARY_INST;
	CBinaryData BInst(&m_Db, (int)sStationID, -1, iStationType, false, m_bQuietMode);

	DB_D_INTERVAL Requested, Actual;
	
	*pbResult = false;

	if (!m_Db.mbDatabaseOpen) m_Db.OpenTheDatabase(sFacilityID);
	if (m_Db.mbDatabaseOpen) {
		m_Db.msFacNum = sFacilityID;
		Requested.dStart = StartTime;
		Requested.dEnd = EndTime;
		BInst.m_ulNumPtsRequested = pvData->lVal;
		if (BInst.m_ulNumPtsRequested > 0) {
			*pbResult = BInst.GetStationData(Requested, &Actual);

			CBinaryDbDataPt* pTemp = BInst.m_pBinaryDbPts; // data are here in the storage allocated in GetStationData
			if (*pbResult) 	// extract info
			{
				//Create VARIANT vector array - a doubly dimensioned array.
				pvData->vt = VT_VARIANT | VT_ARRAY; 
				SAFEARRAYBOUND Dim[2];
				Dim[0].cElements = BInst.m_ulNumPtsActual+1;		// number of rows in the array plus next time
				Dim[0].lLbound = 0;		// index starts at 0
				Dim[1].cElements = 7;	// number of columns (number of fields in a single record -  count timestamp)
				Dim[1].lLbound = 0;
				pvData->parray = SafeArrayCreate(VT_VARIANT,2,Dim);
				unsigned long ul;
				bool bIsDate = true;
				for (ul=0; ul < BInst.m_ulNumPtsActual; ul++) 
				{
					SA_PutElem(pvData->parray, ul, 0L, (double)pTemp[ul].m_dTimestamp, bIsDate);
					SA_PutElem(pvData->parray, ul, 1L, (long)pTemp[ul].m_iStatus);
					SA_PutElem(pvData->parray, ul, 2L, (long)pTemp[ul].m_iLogNodeNumber);
					SA_PutElem(pvData->parray, ul, 3L, (long)pTemp[ul].m_iStationNumber);
					SA_PutElem(pvData->parray, ul, 4L, (long)pTemp[ul].m_iState);
					SA_PutElem(pvData->parray, ul, 5L, (long)pTemp[ul].m_iBit);
					SA_PutElem(pvData->parray, ul, 6L, (long)pTemp[ul].m_iReserved);
				}
				// put the time of the next data point as a double in the last row in the array
				SA_PutElem(pvData->parray, ul, 0L, (double)BInst.m_dTimestampOfNextData,bIsDate);
			}
		}
	}
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



// 26-Oct-2004	SFK	Added db open, if needed
STDMETHODIMP CBinaryImport::GetDataStatistics(short sFacilityID, short sStationID,  short sChannelID, DATE StartTime, DATE EndTime, DATE *pActualStartTime, DATE *pActualEndTime, long *plNumMembers, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pbResult = VARIANT_FALSE;

	*pActualStartTime = 0;
	*pActualEndTime = 0;
	*plNumMembers = 0;

	int iStationType = BINARY_INST;

	bool bInterpolated = false;
	if ((sStationID != -1) && (sChannelID >= 0)) 
		bInterpolated = true;	// are looking for a specific channel

	CBinaryData BInst(&m_Db, (int)sStationID, (int)sChannelID, iStationType, bInterpolated, m_bQuietMode);

	DB_D_INTERVAL Requested, Actual;

	if (!m_Db.mbDatabaseOpen) m_Db.OpenTheDatabase(sFacilityID);
	if (m_Db.mbDatabaseOpen) {
		Requested.dStart = StartTime;
		Requested.dEnd = EndTime;
		*pbResult = BInst.GetDataStatistics(Requested, &Actual);
		if (*pbResult) {
			*pActualStartTime = Actual.dStart;
			*pActualEndTime = Actual.dEnd;
			*plNumMembers = BInst.m_ulNumPtsActual;
		}
	}
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



//	GetDataStore - Common COM function to get the current data store path
// 26-Oct-2004	SFK	Changed name from GetDataStore -- added sFacilityID argument.  Also changed
//					to return path with COM directories appended
STDMETHODIMP CBinaryImport::GetDataStorePath(short sFacilityID, BSTR* bstrStorePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	*bstrStorePath = A2BSTR("");

	if (!m_Db.mbDatabaseOpen) m_Db.OpenTheDatabase(sFacilityID);
	if (m_Db.mbDatabaseOpen) {
		*bstrStorePath = T2BSTR(LPCSTR(m_Db.mstrDatabasePath));
	}
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



//	Common COM function to get the size of the database
// 26-Oct-2004	SFK	Modified to support opening database
STDMETHODIMP CBinaryImport::GetDataStoreSize(short sFacilityID, double *pdSizeInBytes, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CDirUtilities Dir(m_bQuietMode);

	*pbResult = VARIANT_FALSE;//false;
	*pdSizeInBytes = 0;

	if (!m_Db.mbDatabaseOpen) 
		m_Db.OpenTheDatabase(sFacilityID);

	if (m_Db.mbDatabaseOpen) 
	{
		int iDum;
		Dir.GetDirectorySize(m_Db.mstrDatabasePath, pdSizeInBytes, &iDum);
		*pbResult = VARIANT_TRUE;//true;
	}
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}




//	Get the time of the beginning and ending of the data store for the given station.
// 26-Oct-2004	SFK	Modified to support opening database
STDMETHODIMP CBinaryImport::GetDataStoreTimeRange(short sFacilityID, short sStationID, VARIANT_BOOL bErrDisplay, DATE *pFirstTimestamp, DATE *pLastTimestamp, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CBinaryData BInst(&m_Db, sStationID, -1, -1, false, m_bQuietMode);

	*pFirstTimestamp = 99999;
	*pLastTimestamp = 0;
	*pbResult = false;

	if (!m_Db.mbDatabaseOpen) m_Db.OpenTheDatabase(sFacilityID);
	if (m_Db.mbDatabaseOpen) {
		*pbResult = BInst.GetDataStoreTimeRange((bool)bErrDisplay, pFirstTimestamp, pLastTimestamp);
	}
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}


//	Gets day statistics for the specified time range.
// 26-Oct-2004	SFK	Modified to support opening database
STDMETHODIMP CBinaryImport::GetDayStatistics(short sFacilityID, short sStationID, DATE StartTime, DATE EndTime, DATE *pActualStartTime, DATE *pActualEndTime, long *plNumMembers, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	int iStationType = BINARY_INST;
	CBinaryData BInst(&m_Db, (int)sStationID, -1, iStationType, false, m_bQuietMode);

	DB_D_INTERVAL Requested, Actual;

	*pActualStartTime = 0;
	*pActualEndTime = 0;
	*plNumMembers = 0;
	*pbResult = false;

	if (!m_Db.mbDatabaseOpen) m_Db.OpenTheDatabase(sFacilityID);
	if (m_Db.mbDatabaseOpen) {
		Requested.dStart = StartTime;
		Requested.dEnd = EndTime;
		*pbResult = BInst.GetDayStatistics(Requested, &Actual);
		if (*pbResult) {	// extract info
			*pActualStartTime = Actual.dStart;
			*pActualEndTime = Actual.dEnd;
			*plNumMembers = BInst.m_ulNumPtsActual;
		}
	}
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}




//	Gets day data for the specified time range.  SAFEARRAY of results is allocated here.
// 26-Oct-2004	Changed function name from GetDataByDay and eliminate plNumMembers argument since can get from SAFEARRAY info
STDMETHODIMP CBinaryImport::GetDaySummaryData(short sFacilityID, short sStationID, DATE StartTime, DATE EndTime, VARIANT* pvDay, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	int iStationType = BINARY_INST;
	CBinaryData BInst(&m_Db, (int)sStationID, -1, iStationType, false, m_bQuietMode);

	DB_D_INTERVAL Requested, Actual;
	DayDataStruct* pDay = NULL;
	unsigned long ul;

	*pbResult = false;

	if (!m_Db.mbDatabaseOpen) m_Db.OpenTheDatabase(sFacilityID);
	if (m_Db.mbDatabaseOpen) {
		Requested.dStart = StartTime;
		Requested.dEnd = EndTime;
		*pbResult = BInst.GetDayStatistics(Requested, &Actual);	// number of members in interval
		BInst.m_ulNumPtsRequested = BInst.m_ulNumPtsActual;
		if ((*pbResult) && (BInst.m_ulNumPtsActual > 0)) {
			pDay = new DayDataStruct[BInst.m_ulNumPtsActual];	//allocate local storage
			if (pDay) {
				*pbResult = BInst.GetDayData(Requested, &Actual, pDay);

				if (*pbResult) {	// extract info

					//Create VARIANT vector array - a doubly dimensioned array.
					pvDay->vt = VT_VARIANT | VT_ARRAY; 
					SAFEARRAYBOUND Dim[2];
					Dim[0].cElements = BInst.m_ulNumPtsActual;		// number of rows in the array
					Dim[0].lLbound = 0;		// index starts at 0
					Dim[1].cElements = 3;	// number of columns (number of field in a single record)
					Dim[1].lLbound = 0;
					pvDay->parray = SafeArrayCreate(VT_VARIANT,2,Dim);
					long lDimension[2];
				
					for (ul=0; ul<BInst.m_ulNumPtsActual; ul++) {
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
				if (pDay) delete [] pDay;
			}
		}
	}
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}




// Get list of all files this COM knows about in the specified directory(ies)
// 26-Oct-2004	SFK	Added interface but did not implement
// 11-Jul-2005 SFK	Copied from Heather's GRAND Import stuff and modified for binary
STDMETHODIMP CBinaryImport::GetFilesList(short sFacilityID, BSTR bstrRootDir, VARIANT_BOOL bIncludeSubDirs, VARIANT_BOOL bMostRecent, VARIANT *psaFilesList)
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
	VariantInit(psaFilesList);

	// 5-Jul-2005 SFK Added check for whether DB is open as may make calls later on
	if (!m_Db.mbDatabaseOpen) m_Db.OpenTheDatabase(sFacilityID);
	if (m_Db.mbDatabaseOpen) 
	{
		USES_CONVERSION;
		CBinaryDataFile BinaryFile(m_bQuietMode);
		CString cs(bstrRootDir);
		//The BinaryDataFile class encapsulates the search function for us.
		BinaryFile.GetCompleteFileList(sFacilityID, &FileList, cs, (bool)bIncludeSubDirs);
		//If the request is for the most recent data, then determine what the latest date of the data
		//in the database is.

		//*********************************************************************************
		//If there were files found, then load the contents of the linked list into the 
		//SAFEARRAY.  If there are no files, then set the VARIANT to indicate the array is EMPTY.
		//*********************************************************************************
		if(FileList.GetCount())
		{
			long LastStationID = -1;
			VARIANT_BOOL Success;
			DATE StartTime, EndTime;

			
			//Determine how many files will go into the list that is output.  If the MostRecent
			//flag is enabled, then check the file list for the time of the data.
			int NumFilesForList = bMostRecent ? 0 : FileList.GetCount(); 
			
			if(bMostRecent)
			{
				Pos = FileList.GetHeadPosition();
				while(Pos)
				{
					FileRecord = FileList.GetNext(Pos);

					if(FileRecord.StationID.lVal != LastStationID)
					{
						LastStationID = FileRecord.StationID.lVal;
						GetDataStoreTimeRange(sFacilityID, (short)FileRecord.StationID.lVal, false, &StartTime, 
							&EndTime, &Success);
					}
				//	if(!bMostRecent || (bMostRecent && FileRecord.Date.date > EndTime))	// 06-Jul-2005 SFK Changed logic to following line
					if ((int)FileRecord.Date.date >= (int)EndTime)	// 06-Jul-2005 SFK Need to include the last day in the database
						NumFilesForList++;

				}//end WHILE Pos != NULL
			}

			//Set the dimensions of the array and create it.
			bounds[0].cElements = NumFilesForList;  //Number of rows or number of files.
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
					GetDataStoreTimeRange(sFacilityID, (short)FileRecord.StationID.lVal, false, &StartTime, 
						&EndTime, &Success);
				}

			//	if(!bMostRecent || (bMostRecent && FileRecord.Date.date > EndTime))	// 06-Jul-2005 SFK Changed logic to following line
				if (!bMostRecent || (bMostRecent && (int)FileRecord.Date.date >= (int)EndTime))	// 06-Jul-2005 SFK Need to include the last day in the database
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
	//} //end IF if bResult = true (Facility exists)
	}
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}




// 26-Oct-2004	SFK	Added interface but did not implement
STDMETHODIMP CBinaryImport::GetParameters(short sFacilityID, VARIANT* pvParms, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	//*pbResult = false;
	*pbResult = VARIANT_FALSE;

	return S_OK;
}



//	InitializeDataStore - Common COM function to do any special action to initialize a data store
//	26-Oct-2004	SFK	Added database open, if necessary.  Also changed calling argument to have FacilityID rather the db path
STDMETHODIMP CBinaryImport::InitializeDataStore(short sFacilityID, VARIANT_BOOL vbQuestion, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	bool bResult = false;
	bool bQuestion = (vbQuestion != VARIANT_FALSE);
	
	if (!m_Db.mbDatabaseOpen) 
		m_Db.OpenTheDatabase(sFacilityID);

	if (m_Db.mbDatabaseOpen)
		bResult = m_Db.InitializeDatabase(m_Db.mstrDatabaseRootPath, bQuestion);

	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	*pbResult = bResult?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}



// OpenDataStore - Common COM function to do any special action to open a data store
//	26-Oct-2004	SFK	Changed calling argument from path to FacilityID
STDMETHODIMP CBinaryImport::OpenDataStore(short sFacilityID, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pbResult = false;
	
	if (!m_Db.mbDatabaseOpen) m_Db.OpenTheDatabase(sFacilityID);
	if (m_Db.mbDatabaseOpen) {
		*pbResult = true;
	}

	return S_OK;
}



//	RestoreDataStore - Common COM function to restore any COM specific files from a designated location
// 26-Oct-2004	SFK	Changed to have sFacilityID instead of bstrDestPath in arguments, opened db if necessary
STDMETHODIMP CBinaryImport::RestoreDataStore(
	short sFacilityID, BSTR bstrSourcePath, VARIANT_BOOL vbQuestion, VARIANT_BOOL vbDefault, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	bool bResult = false;
	bool bQuestion = (vbQuestion != VARIANT_FALSE);
	bool bDefault = (vbDefault != VARIANT_FALSE);
	
	if (!m_Db.mbDatabaseOpen) 
		m_Db.OpenTheDatabase(sFacilityID);

	if (m_Db.mbDatabaseOpen) 
	{
		CString strSrcPath = W2T(bstrSourcePath);
		CString strDestPath = m_Db.mstrDatabaseRootPath;
		bResult = m_Db.RestoreDatabase(strSrcPath, strDestPath, bQuestion, bDefault);
	}
	// 15-Mar-2006 SFK  Removed db close action because the close takes so long
	// DR 269			Another scheme will have to be figured out to allow 2 programs to use the COM
	//					unless Access automatically handles this -- talk with Kelly re Facility Manager
	//m_Db.CloseDatabase();			// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM
	*pbResult = bResult?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}



STDMETHODIMP CBinaryImport::SetParameters(
	short sFacilityID, 
	VARIANT vData, 
	VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (vData.vt == VT_I4)//long
	{
		long lfourbytes = vData.lVal;
		short high_16 = (short)(lfourbytes >> 16);
		m_bQuietMode = (0 != (high_16 & 0x0001)); //quiet mode bit is bit 0
	}
	*pbResult = VARIANT_TRUE;

	return S_OK;
}


