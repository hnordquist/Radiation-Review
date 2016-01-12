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
// GrandImport.cpp : Implementation of CGrandImport
#include "stdafx.h"
#include "CoGrandImport.h"
#include "GrandImport.h"
#include "GrandDataFile.h"
#include "GrandDbVista.h"
#include "GrandData.h"
#include "SafeArrayUtil.h"
#include "GrandImportSupport.h"
#include "internal.h"  //TOPIC_GLOBALS definition
#include "ErrorProcessing.h"  //Flag definitions
#include <comutil.h>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define Currtask m_Db.m_Currtask

CGrandImport::CGrandImport()
	: m_csNULL(""),
	  m_csColon(","),
	  m_csGetDataStatistics("GetDataStatistics()")
{
	TRACE("CGrandImport::CTOR()\n");
	try
	{
		m_sVerbosityLevel = 2;
		m_bDisableLogger = true;

		m_bQuietMode = true;  //default to silence for NDAR's sake
		m_Dir.SetQuietMode(m_bQuietMode);
		m_Db.SetQuietMode(m_bQuietMode);

		//16 July 2007 pjm
		m_FacCfg.SetFlags(VERBOSITY2_FATAL | QUIET_MODE | DISABLE_LOGGER);	//0x00030002

		m_Db.SetFacMgr(&m_FacCfg);

		m_csDBType = "DBVista";

		m_perr = NULL;

		m_StartProcess = m_StartProcess.GetCurrentTime();
		SYSTEMTIME SysTime;
		::GetSystemTime(&SysTime);
		m_sStartUpTimeMillisec = (short)SysTime.wMilliseconds; 

		m_strFacilityName = CString("UNITIALIZED FACILITY NAME");

		InitializeErrorProcessing();
		ASSERT(TestPointer(m_perr));
	}
	catch(...)
	{
	}
}

CGrandImport::~CGrandImport()
{
	TRACE("CGrandImport::DTOR()\n");
	try
	{
		//JCD - shutdown message
		if (m_perr)
		{
			m_perr->InitializeForMethod("DTOR");
			m_perr->StopTime();
			delete m_perr;
			m_perr = NULL;
		}
	}
	catch(...)
	{
	}
}

/////////////////////////////////////////////////////////////////////////////

// 19-Jan-2006 SFK  For all the Import COMs look carefully at the concurrency issues.  right now the second call
// is ignored but no error returned to the user.  SKBL3

//Jan 2006 pjm added usage of VARIANT_TRUE and VARIANT_FALSE
//SCR00237: Add GetLastError() pjm 3-Feb-2006
//SCR00240 Change station ID from short to long

//	Read the specified data file and add to the data store.
// 26-Oct-2004	SFK	Added db open, if needed.  If successful, facility-specific database is open.

// METHOD 1 AddData

STDMETHODIMP CGrandImport::AddData(
	short sFacilityID, 
	BSTR bstrFilename, 
	VARIANT_BOOL vbOverwrite, 
	BSTR* bstrResult, 
	VARIANT_BOOL *pvbResult)
{ 
	//TRACE("CGrandImport::AddData()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bOverwrite = (vbOverwrite != VARIANT_FALSE);
	bool bSuccess = false;

	try
	{
		m_perr->InitializeForMethod("AddData()",sFacilityID);

		m_csFilename = bstrFilename;
		m_csResultFromDb.Empty();
		m_csError.Empty();

		if (!m_csFilename.IsEmpty())
		{
			CGrandDataFile File(&m_FacCfg,m_bQuietMode);
			File.mbOverwrite = bOverwrite;

			if (!m_Db.mbDatabaseOpen)
			{
				if (!m_Db.OpenTheDatabase(sFacilityID,&m_csResultFromDb))
					m_perr->ErrorOpenDatabase(m_csResultFromDb);
			}
			if (m_Db.mbDatabaseOpen) 
			{
				m_Db.msFacNum = sFacilityID;
				bSuccess = File.ReadDataFile(&m_Db, m_csFilename, &m_csResultFromDb);

				if (!bSuccess) 
				{	// 13-Jan-2004 Added print of unexpected errors error number
					m_csError.Format(" Unexpected error %d:  File skipped.", File.miErrorNum);
					m_csError += m_csResultFromDb;
					m_perr->ErrorReadFile(m_csFilename, m_csError);
					//MessageBox (NULL, "readdatafile returned error", "heather's debug message", MB_OK|MB_ICONWARNING);
				}
				else //success
				{
					m_perr->SuccessReadFile(m_csResultFromDb);
					//MessageBox (NULL, "readdatafile success", "heather's debug message", MB_OK|MB_ICONWARNING);
				}
			}
		}
		else  //filename is empty
		{
			m_csError = "Filename was empty.";
			m_perr->ErrorOpenFile(m_csFilename, m_csError);
		}
		if (TestPointer(bstrResult))
			*bstrResult = m_csResultFromDb.AllocSysString();
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
//		MessageBox (NULL, "com error1", "heather's debug message", MB_OK|MB_ICONWARNING);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
//		MessageBox (NULL, "com error2", "heather's debug message", MB_OK|MB_ICONWARNING);
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;
	else
		//MessageBox (NULL, "fail to get test pointer", "heather's debug message", MB_OK|MB_ICONWARNING);
	/*if (bSuccess)
		MessageBox (NULL, "success add data", "heather's debug message", MB_OK|MB_ICONWARNING);
	else
		MessageBox (NULL, "fail add data", "heather's debug message", MB_OK|MB_ICONWARNING);*/
	m_Db.CloseDatabase();
	return S_OK;
}


// METHOD 2 BackupDataStore

STDMETHODIMP CGrandImport::BackupDataStore(
	short sFacilityID, 
	BSTR bstrDestinationPath, 
	VARIANT_BOOL vbQuestion, 
	VARIANT_BOOL vbDefault, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bSuccess = false;
	bool bQuestion = (vbQuestion != VARIANT_FALSE);
	bool bDefault = (vbDefault != VARIANT_FALSE);

	try
	{
		m_perr->InitializeForMethod("BackupDataStore()",sFacilityID);

		m_csResultFromDb.Empty();
		m_csError.Empty();
		m_csDestPath = bstrDestinationPath;
 
		if (m_csDestPath.IsEmpty())
		{
			m_csError = "Destination path was empty.";
			m_perr->ErrorOpenFile(m_csDestPath, m_csError);
		}

		else
		{
			if (!m_Db.mbDatabaseOpen)
			{
				if (!m_Db.OpenTheDatabase(sFacilityID,&m_csResultFromDb))
					m_perr->ErrorOpenDatabase(m_csResultFromDb);
			}

			if (m_Db.mbDatabaseOpen) 
			{
				bSuccess = m_Db.BackupDatabase(m_csDestPath, bQuestion, bDefault);

				if (!bSuccess)
				{
					m_cs = "Destination Path:" + m_csDestPath;
					m_perr->ErrorDatabase(m_csDBType,m_cs);
				}
			}
		}
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch (...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	m_Db.CloseDatabase();
	return S_OK;
}

// METHOD 3 CloseDataStore

STDMETHODIMP CGrandImport::CloseDataStore()
{
	//TRACE("CGrandImport::CloseDataStore()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	try
	{
		m_perr->InitializeForMethod("CloseDataStore()");
		m_Db.CloseDatabase();
	}
	catch (_com_error &e)
	{
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch (...)
	{
		if (m_perr) m_perr->ErrorCatch();
	}
	return S_OK;
}

// METHOD 4 CreateNewDataStore

STDMETHODIMP CGrandImport::CreateNewDataStore(short sFacilityID, VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::CreateNewDataStore()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bool bSuccess = false;

	try
	{
		m_perr->InitializeForMethod("CreateNewDataStore()",sFacilityID);

		CGrandImportSupport ImportSupport(&m_FacCfg,m_bQuietMode);
		//This function was also added to the GrandDbVista class and is called whenever
		//a call to open a data store fails because the data store does not exist.
		//Determine the empty data store and destination path for the facility's data store
		//location, then copy the contents of the empty db to the place it should be.
		// The result is whether or not this copy was successful. hn 6/8/2005
		//CString emptyDBPath, dataStorePath;
		m_csEmptyDBPath = ImportSupport.GetEmptyDBLocation();
		m_csDataStorePath = ImportSupport.GetGrandDataStorePath (sFacilityID);

		if (m_csDataStorePath.IsEmpty())
		{
			m_cs.Format("Target path for database creation is empty.  Probable cause is that facility %d does not exist.",sFacilityID);
			m_perr->ErrorCreateDatabase(m_cs);
		}
		else
		{
			//Copy the empty db, return the success of the copy operation.
			bSuccess = m_Dir.CopyDirectory(m_csEmptyDBPath,m_csDataStorePath,false);  //true means overwrite

			if (!bSuccess)
			{
				m_cs = "From:"+ m_csDataStorePath +  " To:" + m_csEmptyDBPath;
				m_perr->ErrorCreateDatabase(m_cs);
			}
		}
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch (...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}

//METHOD 5 DeleteData

//SCR00237
STDMETHODIMP CGrandImport::DeleteData(
	short sFacilityID,
	long lStationID, 
	DATE dtStartDate,
	DATE dtEndDate, 
	VARIANT_BOOL* pvbResult)
{
	//TRACE("CGrandImport::DeleteData()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bool bSuccess = false;

	try
	{
		m_perr->InitializeForMethod("DeleteData()", sFacilityID,lStationID);

		CGrandData Inst(&m_Db, (const int) lStationID, false, m_bQuietMode);	//SCR00240

		if (m_Db.mbDatabaseOpen)
			m_Db.CloseDatabase();

		if (!m_Db.OpenTheDatabase(sFacilityID,&m_csResultFromDb))
		{
			m_perr->ErrorOpenDatabase(m_csResultFromDb);
		}
		//else if (!Inst.StationDataExists())
		else if (!Inst.AnyDaysExistInDb(DB_END)) //from HN 5 Dec 2006
		{
			bSuccess = true;  //no data to delete, so we succeeded. Return VARIANT_TRUE below
		}
		else
		{//we have data in the appropriate time range

			int iEndDate = (int)dtEndDate;
			int iStartDate = (int)dtStartDate;
			int iNumberOfDaysToDelete = iEndDate - iStartDate + 1;

			if (iEndDate >= iStartDate)	//can't have end date before start date
			{
				DATE dDay = (double)iStartDate;

				for (int i = 0; i < iNumberOfDaysToDelete; i++)
				{
					Inst.DeleteDay(/*DATE*/dDay);  //it is okay if this fails
					dDay += 1.0;
				}
				bSuccess = true;
			}
			else //start date was after end date
			{
				m_perr->ErrorStartTimeAfterEndTime(dtStartDate, dtEndDate);
			}
		}
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	m_Db.CloseDatabase();		
	return S_OK;
}

// METHOD 6 DeleteDataStore

STDMETHODIMP CGrandImport::DeleteDataStore(short sFacilityID,VARIANT_BOOL* pvbResult)
{
	//TRACE("CGrandImport::DeleteDataStore()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bool bSuccess = false;

	try
	{
		m_perr->InitializeForMethod("DeleteDataStore()", sFacilityID);

		//CString csDataStorePath, csProblemFile;
		CString csProblemFile = "INVALID DIRECTORY AND/OR FILENAME";
		CGrandImportSupport ImportSupport(&m_FacCfg, m_bQuietMode);

		m_csDataStorePath	= ImportSupport.GetGrandDataStorePath(sFacilityID);

		if (!m_csDataStorePath.IsEmpty())
		{
			//we need a temporary string containing the data path minus the final slash
			//FindFirstFile in m_Dir.FileExists does not accept slash terminated input
			m_cs.Empty();

			if(m_csDataStorePath.GetAt(m_csDataStorePath.GetLength()-1)=='\\')
				m_cs = /*csTempDir = */m_csDataStorePath.Left(m_csDataStorePath.GetLength()-1);

			if(m_Dir.FileExists(m_cs))//csTempDir))
				bSuccess = m_Dir.DeleteDirectory(LPCTSTR(m_csDataStorePath), &csProblemFile);
			else
				//directory did not exist, so doesn't need to be deleted, but set return true
				bSuccess = true;
		}
		if (!bSuccess)
		{
			//CString cs;
			m_cs.Format("Cannot delete %s.", csProblemFile);
			m_perr->ErrorDatabase(m_csDBType, m_cs);
		}
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}

//METHOD 7 GetDataByChannel

STDMETHODIMP CGrandImport::GetDataByChannel(
	short sFacilityID, 
	long lStationID, 
	short sChannelID, 
	DATE StartTime, 
	DATE EndTime, 
	VARIANT *pvData, 
	VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::GetDataByChannel()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bSuccess = false;
	double *pdTimestamps = NULL, *pdData = NULL;

	try
	{
		m_perr->InitializeForMethod("GetDataByChannel()",sFacilityID,lStationID,sChannelID);

		//protect against invalid inputs.
		bool bContinue = true;

		if (TestPointer(pvData))
			::VariantInit(pvData);
			//pvData->vt = VT_EMPTY;  //In case we find no points to return
		else
			bContinue = false;

		if (StartTime > EndTime)
		{
			m_perr->ErrorStartTimeAfterEndTime(StartTime, EndTime);
			bContinue = false;
		}

		if (!bContinue)
		{
			if (TestPointer(pvbResult))
				*pvbResult = VARIANT_FALSE;
			return S_OK;
		}

		//JCD additions to make sure requested start date is not earlier than the start of data
		//When test driver requested data with an arbitrarily early date, the arbitrary date
		//caused DB Vista to return bad data (2 sets of the first days data and no data for the last day).
		//Solution is to use GetDataStatistics to correct the start time of the request to the actual start
		//date,if arbitrary start date is prior to the actual start date.

		long lnumber_of_points_to_request = 
			GetNumberOfPointsToRequest(sFacilityID, lStationID, StartTime, EndTime, pvData->lVal);
		//we called GetDataStatistics out of GetNumberOfPointsToRequest, so the error initialization needs to
		//be restored.
		m_perr->InitializeForMethod("GetDataByChannel()",sFacilityID,lStationID,sChannelID);

		if (lnumber_of_points_to_request == -1)
		{
			m_perr->ErrorEmbeddedInterfaceCallFailed(m_csGetDataStatistics);
			//m_perr->ErrorDatabase(m_csDBType, m_csNULL);
			bSuccess = false;
			goto finished;
		}

		long lDummy;
				
		//CString csout,cs,csErrorMsg,strResultFromDb;
		DATE dtActualStart, dtActualEnd;
		VARIANT_BOOL vbResult;

		//Calling an interface method from within another will alter the error processing setup,
		//so reinitialze the error processing afterwards.
		GetDataStatistics(sFacilityID,lStationID,0,StartTime,EndTime,&dtActualStart,&dtActualEnd,&lDummy,&vbResult);
		m_perr->InitializeForMethod("GetDataByChannel()",sFacilityID,lStationID,sChannelID);

		if (vbResult != VARIANT_TRUE) 
		{
			m_perr->ErrorEmbeddedInterfaceCallFailed(CString("GetDataStatistics"));
			bSuccess = false;
		}
		else if (lnumber_of_points_to_request == 0)
		{
			pvData->vt = VT_EMPTY;
			bSuccess = true;
		}	
		else //vbResult == VARIANT_TRUE and there are some points to get
		{	
			if(StartTime < dtActualStart)
				StartTime = dtActualStart;

			CGrandData Inst(&m_Db, (const int)lStationID, false, m_bQuietMode);	//SCR00240 Change station ID from short to long
			DB_D_INTERVAL Requested, Actual;

			m_Db.msFacNum = sFacilityID;

			if (!m_Db.mbDatabaseOpen)
			{
				if (!m_Db.OpenTheDatabase(sFacilityID,&m_csResultFromDb))
					m_perr->ErrorOpenDatabase(m_csResultFromDb);
			}

			if (m_Db.mbDatabaseOpen) 
			{
				Requested.dStart = StartTime;
				Requested.dEnd = EndTime;

				Inst.mulNumPtsRequested = lnumber_of_points_to_request;

				if (Inst.mulNumPtsRequested > 0) 
				{
					pdTimestamps = new double[Inst.mulNumPtsRequested];

					if (pdTimestamps) 
						pdData = new double[Inst.mulNumPtsRequested];

					if (pdData)
					{
						//CString csout("");
						m_csout.Empty();

						bSuccess = Inst.GetChannelData(sChannelID, Requested, &Actual, pdTimestamps, pdData, &m_csout);

						if (!bSuccess)	//SCR00237: Add GetLastError() pjm 3-Feb-2006
						{
							m_cs.Format(" Channel ID = %d.",sChannelID);
							m_cs += m_csColon + m_csout;
							m_perr->ErrorDatabase(m_csDBType, m_cs);
						}
					}

					if (bSuccess  && (0 != Inst.mulNumPtsActual))
					{
						//Create VARIANT vector array - a doubly dimensioned array of timestamps and data.
						pvData->vt = VT_VARIANT | VT_ARRAY; 
						SAFEARRAYBOUND Dim[2];
						Dim[0].cElements = Inst.mulNumPtsActual + 1;		// number of rows in the array plus TimestampOfNextData
						Dim[0].lLbound = 0;		// index starts at 0
						Dim[1].cElements = 2;	// number of columns (timestamp and data)
						Dim[1].lLbound = 0;
						pvData->parray = SafeArrayCreate(VT_VARIANT,2,Dim);
						long lDimension[2];
						long ul;
					
						for (ul=0; ul < (long)Inst.mulNumPtsActual; ul++) 
						{
							lDimension[0] = ul;
							SA_PutElem(pvData->parray, ul, 0L, pdTimestamps[ul], true);  //true means VT_DATE
							SA_PutElem(pvData->parray, ul, 1L, pdData[ul]);
						}
						// fill in the Next Data point time as the last element in the array
						SA_PutElem(pvData->parray, ul, 0L, Inst.mdTimestampOfNextData, true);  //true means VT_DATE
					}
				}
			}
		}
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

finished:

	delete [] pdTimestamps;
	delete [] pdData;

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	m_Db.CloseDatabase();
	return S_OK;
}

// METHOD 8 GetDataByStation

STDMETHODIMP CGrandImport::GetDataByStation(
	short sFacilityID, 
	long lStationID, 
	DATE StartTime, 
	DATE EndTime, 
	VARIANT *pvData,   //if pvData->lVal is zero, then get all of the points.
	VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::GetDataByStation()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bool bSuccess = false;

	try
	{
		m_perr->InitializeForMethod("GetDataByStation()",sFacilityID,lStationID);

		//protect against invalid inputs.
		bool bContinue = true;

		if (TestPointer(pvData))
			//pvData->vt = VT_EMPTY;  //In case we find no points to return
			::VariantInit(pvData);
		else
			bContinue = false;

		if (StartTime > EndTime)
		{
			m_perr->ErrorStartTimeAfterEndTime(StartTime, EndTime);
			bContinue = false;
		}

		if (!bContinue)
		{
			if (TestPointer(pvbResult))
				*pvbResult = VARIANT_FALSE;
			return S_OK;
		}

		//JCD additions to make sure requested start date is not earlier than the start of data
		//When test driver requested data with an arbitrarily early date, the arbitrary date
		//caused DB Vista to return bad data (2 sets of the first days data and no data for the last day).
		//Solution is to use GetDataStatistics to correct the start time of the request to the actual start
		//date,if arbitrary start date is prior to the actual start date.

		long lnumber_of_points_to_request = 
			GetNumberOfPointsToRequest(sFacilityID, lStationID, StartTime, EndTime, pvData->lVal);
		//we called GetDataStatistics out of GetNumberOfPointsToRequest, so the error initialization needs to
		//be restored.
		m_perr->InitializeForMethod("GetDataByStation()",sFacilityID,lStationID);

		if (lnumber_of_points_to_request == -1)
		{
			m_perr->ErrorEmbeddedInterfaceCallFailed(m_csGetDataStatistics);
			bSuccess = false;
			goto finished;
		}

		long lDummy;

		//CString csout,cs,csErrorMsg,strResultFromDb;
		DATE dtActualStart, dtActualEnd;
		VARIANT_BOOL vbResult;

		//Calling an interface method from within another will alter the error processing setup,
		//so reinitialze the error processing afterwards.
		GetDataStatistics(sFacilityID,lStationID,0,StartTime,EndTime,&dtActualStart,&dtActualEnd,&lDummy,&vbResult);//don't need the number of points
		m_perr->InitializeForMethod("GetDataByStation()",sFacilityID,lStationID);
	
		if (vbResult != VARIANT_TRUE) 
		{
			m_perr->ErrorEmbeddedInterfaceCallFailed(m_csGetDataStatistics);
			bSuccess = false;
		}
		else if (lnumber_of_points_to_request == 0)
		{
			pvData->vt = VT_EMPTY;
			bSuccess = true;
		}
		else //vbResult == VARIANT_TRUE and there are some points to get 
		{	
			if(StartTime < dtActualStart)
				StartTime = dtActualStart;

			CGrandData Inst(&m_Db, (const int)lStationID, false, m_bQuietMode);	//SCR00240 Change station ID from short to long
			DB_D_INTERVAL Requested, Actual;

			m_Db.msFacNum = sFacilityID;

			if (!m_Db.mbDatabaseOpen)
			{
				if (!m_Db.OpenTheDatabase(sFacilityID,&m_csResultFromDb))
					m_perr->ErrorOpenDatabase(m_csResultFromDb);
			}

			if (m_Db.mbDatabaseOpen) 
			{
				Requested.dStart = StartTime;
				Requested.dEnd = EndTime;

				Inst.mulNumPtsRequested = lnumber_of_points_to_request;

				if (Inst.mulNumPtsRequested > 0) 
				{
					bSuccess = Inst.GetStationData(Requested, &Actual, &m_csout);

					if (!bSuccess)
					{
						m_cs.Format(" Station ID = %d.",lStationID);
						m_cs += m_csColon + m_csout;
						m_perr->ErrorDatabase(m_csDBType, m_cs);
					}
					else if (0 != Inst.mulNumPtsActual)
					{	//Extract info
						//Create VARIANT vector array - a doubly dimensioned array.
						//This has an update by Shirley to 21 elements.........
						CGrandDbDataPt* pTemp = Inst.mpGrandDataPtArray; // storage allocated in GetStationData
						pvData->vt = VT_VARIANT | VT_ARRAY; 
						SAFEARRAYBOUND Dim[2];
						Dim[0].cElements = Inst.mulNumPtsActual+1;	// number of rows in the array + Next timestamp
						Dim[0].lLbound = 0;		//index starts at 0
						Dim[1].cElements = 21;	//number of columns (number of field in a single record) plus the timestamp
						Dim[1].lLbound = 0;
						pvData->parray = SafeArrayCreate(VT_VARIANT,2,Dim);
						unsigned long ul;

						for (ul=0; ul < Inst.mulNumPtsActual; ul++) 
						{
							SA_PutElem(pvData->parray, ul, 0L, (double)pTemp[ul].mdJulianTime,true);  //true forces VT_DATE
							SA_PutElem(pvData->parray, ul, 1L, (long)pTemp[ul].mulStatus);
							SA_PutElem(pvData->parray, ul, 2L, (float)pTemp[ul].mfData[0]);		// neutron A
							SA_PutElem(pvData->parray, ul, 3L, (float)pTemp[ul].mfData[1]);		// neutron B
							SA_PutElem(pvData->parray, ul, 4L, (float)pTemp[ul].mfData[2]);		// neutron C
							SA_PutElem(pvData->parray, ul, 5L, (float)pTemp[ul].mfData[3]);		// gamma 1
							SA_PutElem(pvData->parray, ul, 6L, (float)pTemp[ul].mfData[4]);		// gamma 2
							SA_PutElem(pvData->parray, ul, 7L, (float)pTemp[ul].mfData[5]);		// gamma 1 unc
							SA_PutElem(pvData->parray, ul, 8L, (float)pTemp[ul].mfData[6]);		// gamma 2 unc
							SA_PutElem(pvData->parray, ul, 9L, (float)pTemp[ul].mfData[7]);		// mulStatus bit 0 (In MII)
							SA_PutElem(pvData->parray, ul, 10L, (float)pTemp[ul].mfData[8]);	// mulStatus bit 1 (No authentication)
							SA_PutElem(pvData->parray, ul, 11L, (float)pTemp[ul].mfData[9]);	// mulStatus bit 2 (Cold start)
							SA_PutElem(pvData->parray, ul, 12L, (float)pTemp[ul].mfData[10]);	// mulStatus bit 3 (Taking offsets)
							SA_PutElem(pvData->parray, ul, 13L, (float)pTemp[ul].mfData[11]);	// mulStatus bit 4 (Over threshold)
							SA_PutElem(pvData->parray, ul, 14L, (float)pTemp[ul].mfData[12]);	// mulStatus bit 5 (Changing signal)
							SA_PutElem(pvData->parray, ul, 15L, (float)pTemp[ul].mfData[13]);	// mulStatus bit 6 (Battery low)
							SA_PutElem(pvData->parray, ul, 16L, (float)pTemp[ul].mfData[14]);	// mulStatus bit 7 (AC power loss)
							SA_PutElem(pvData->parray, ul, 17L, (float)pTemp[ul].mfData[15]);	// mulStatus bit 16 (Taking offsets)
							SA_PutElem(pvData->parray, ul, 18L, (float)pTemp[ul].mfData[16]);	// mulStatus bit 17 (Invalid gammas)
							SA_PutElem(pvData->parray, ul, 19L, (float)pTemp[ul].mfData[17]);	// mulStatus bit 18 (Saturated gammas)
							SA_PutElem(pvData->parray, ul, 20L, pTemp[ul].mdElapsedTime);
						}
						// put the time of the next data point as a double in the last row in the array
						SA_PutElem(pvData->parray, ul, 0L, Inst.mdTimestampOfNextData,true); //true forces VT_DATE
					}
				}
			}
		}
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

finished:

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	m_Db.CloseDatabase();
	return S_OK;
}

//METHOD 9 GetDataStatistics

//	Gets the amount of data in the given interval for the given station.
// 26-Oct-2004	SFK	Added db open, if needed
STDMETHODIMP CGrandImport::GetDataStatistics(
	short sFacilityID, 
	long lStationID,  
	short sUnused,//sChannelID, 
	DATE StartTime, 
	DATE EndTime, 
	DATE *pActualStartTime, 
	DATE *pActualEndTime, 
	long *plNumMembers, 
	VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::GetDataStatistics()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bool bSuccess = false;

	try
	{
		m_perr->InitializeForMethod(m_csGetDataStatistics,sFacilityID,lStationID);

		if (!TestPointer(pActualStartTime) ||
			!TestPointer(pActualEndTime) ||
			!TestPointer(plNumMembers) ||
			!TestPointer(pvbResult))
		{
			bSuccess = false;
			m_perr->ErrorInvalidPointer();
		}
		else
		{
			CGrandData Inst(&m_Db, (const int)lStationID, false, m_bQuietMode);
			DB_D_INTERVAL Requested, Actual;

			*pActualStartTime = 0.0;	//DATE
			*pActualEndTime = 0.0;		//DATE
			*plNumMembers = 0;			//long

			if (!m_Db.mbDatabaseOpen)
			{
				//CString strResultFromDb;
				if (!m_Db.OpenTheDatabase(sFacilityID,&m_csResultFromDb))
					m_perr->ErrorOpenDatabase(m_csResultFromDb);
			}

			if (m_Db.mbDatabaseOpen) 
			{
				m_Db.msFacNum = sFacilityID;
				Requested.dStart = StartTime;
				Requested.dEnd = EndTime;

				//CString csErrorMsg;
				bSuccess = Inst.GetDataStatistics(Requested, &Actual, &m_csError);

				if (bSuccess) 
				{	// extract info
					*pActualStartTime = Actual.dStart;
					*pActualEndTime = Actual.dEnd;
					*plNumMembers = Inst.mulNumPtsActual;
					//hn -- This is a test
					if (Inst.mulNumPtsActual ==0 || (*plNumMembers < 0))
						bSuccess = false;
				}
				else
				{
					m_csDBError = Inst.GetDatabaseErrorText();
					m_csError += m_csDBError;
					//Inst.GetDatabaseErrorText(csDBError);
					m_perr->ErrorDatabase(m_csDBType, m_csError);// + m_csDBError);
				}
			}
		}
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	m_Db.CloseDatabase();
	return S_OK;
}

//METHOD 10 GetDataStorePath

STDMETHODIMP CGrandImport::GetDataStorePath(short sFacilityID, BSTR* bstrStorePath)
{
	//TRACE("CGrandImport::GetDataStorePath()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	try
	{
		m_perr->InitializeForMethod("GetDataStorePath()",sFacilityID);

		//CString cs("Error");
		m_cs = "Error";

		if (!m_Db.mbDatabaseOpen)
		{
			//CString strResultFromDb;
			if (!m_Db.OpenTheDatabase(sFacilityID,&m_csResultFromDb))
				m_perr->ErrorOpenDatabase(m_csResultFromDb);
		}
		if (m_Db.mbDatabaseOpen) 
		{
			m_cs = m_Db.mstrDatabasePath;
			m_Db.CloseDatabase();
		}
		*bstrStorePath = m_cs.AllocSysString();
	}
	catch (_com_error &e)
	{
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		if (m_perr) m_perr->ErrorCatch();
	}

	return S_OK;
}

// METHOD 11 GetDataStoreSize

STDMETHODIMP CGrandImport::GetDataStoreSize(short sFacilityID, double *pdSizeInBytes, VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::GetDataStoreSize()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bool bSuccess = false;

	try
	{
		m_perr->InitializeForMethod("GetDataStoreSize()",sFacilityID);

		if (TestPointer(pdSizeInBytes))
		{
			*pdSizeInBytes = 0;

			//CString strResultFromDb;
			if (!m_Db.OpenTheDatabase(sFacilityID,&m_csResultFromDb))
				m_perr->ErrorOpenDatabase(m_csResultFromDb);

			if (m_Db.mbDatabaseOpen) 
			{
				int iDum;
				m_Dir.GetDirectorySize(m_Db.mstrDatabasePath, pdSizeInBytes, &iDum);
				bSuccess = true;
			}
		}
		else
		{
			bSuccess = false;
			m_perr->ErrorInvalidPointer();
		}
	}
	catch (_com_error &e)
	{
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	m_Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}


//	Gets day data for the specified time range.  SAFEARRAY of results is allocated here.
// 26-Oct-2004	Changed function name from GetDataByDay and eliminate plNumMembers argument since can get from SAFEARRAY info
STDMETHODIMP CGrandImport::GetDaySummaryData(
	short sFacilityID, 
	long lStationID, 
	DATE StartTime, 
	DATE EndTime, 
	VARIANT* pvDay, 
	VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::GetDaySummaryData()\n");

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bSuccess = false;

	DayDataStruct* pDay = NULL;

	try
	{
		m_perr->InitializeForMethod("GetDaySummaryData()",sFacilityID,lStationID);

		::VariantInit(pvDay);
		//pvDay->vt = VT_EMPTY;

		CGrandData Inst(&m_Db, (const int) lStationID, false, m_bQuietMode);	//SCR00240 Change station ID from short to long
		DB_D_INTERVAL Requested, Actual;
		unsigned long ul;
		CString csErrMsgFromDB,csDBError;

		if (!m_Db.mbDatabaseOpen)
		{
			CString strResultFromDb;
			if (!m_Db.OpenTheDatabase(sFacilityID,&strResultFromDb))
				m_perr->ErrorOpenDatabase(strResultFromDb);
		}

		if (m_Db.mbDatabaseOpen) 
		{
			m_Db.msFacNum = sFacilityID;
			Requested.dStart = StartTime;
			Requested.dEnd = EndTime;
			
			bSuccess = Inst.GetDayStatistics(Requested, &Actual,&csErrMsgFromDB);	// number of members in interval

			if (!bSuccess)			//SCR00237: Add GetLastError(). pjm 3-Feb-2006
			{
				Inst.GetDatabaseErrorText(csDBError);
				CString cs = csErrMsgFromDB + " " + csDBError;
				m_perr->ErrorDatabase(m_csDBType, cs);
			}

			Inst.mulNumPtsRequested = Inst.mulNumPtsActual;

			if (bSuccess && (Inst.mulNumPtsActual > 0))
			{
				pDay = new DayDataStruct[Inst.mulNumPtsActual];

				if (pDay) 
				{
					bSuccess = Inst.GetDayData(Requested, &Actual, pDay, &csErrMsgFromDB);
 
					if (bSuccess) 
					{	// extract info
						//Create VARIANT vector array - a doubly dimensioned array.
						pvDay->vt = VT_VARIANT | VT_ARRAY; 
						SAFEARRAYBOUND Dim[2];
						Dim[0].cElements = Inst.mulNumPtsActual;		// number of rows in the array
						Dim[0].lLbound = 0;		// index starts at 0
						Dim[1].cElements = 3;	// number of columns (number of field in a single record)
						Dim[1].lLbound = 0;
						pvDay->parray = SafeArrayCreate(VT_VARIANT,2,Dim);
						long lDimension[2];
					
						for (ul=0; ul<Inst.mulNumPtsActual; ul++) 
						{
							lDimension[0] = ul;

							lDimension[1] = 0;
							CComVariant v((double)pDay[ul].dFirstDataPt);
							v.vt = VT_DATE;
							SafeArrayPutElement(pvDay->parray,lDimension,&v);

							lDimension[1] = 1;
							CComVariant v1((double)pDay[ul].dLastDataPt);
							v1.vt = VT_DATE;
							SafeArrayPutElement(pvDay->parray,lDimension,&v1);
								
							lDimension[1] = 2;
							CComVariant v2((double)pDay[ul].dNumPtsInDay);
							SafeArrayPutElement(pvDay->parray,lDimension,&v2);
						}
					}
					else	//SCR00237: Add GetLastError() pjm 3-Feb-2006
					{
						Inst.GetDatabaseErrorText(csDBError);
						m_perr->ErrorDatabase(m_csDBType, csDBError);
					}
				}
			}
		}
	}
	catch (_com_error &e)
	{
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	delete [] pDay;

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	m_Db.CloseDatabase();
	return S_OK;
}


// METHOD 13 GetDataStoreTimeRange

STDMETHODIMP CGrandImport::GetDataStoreTimeRange(
	short sFacilityID, 
	long lStationID, 
	VARIANT_BOOL vbErrDisplay, 
	DATE *pFirstTimestamp, 
	DATE *pLastTimestamp, 
	VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::GetDataStoreTimeRange()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bool bSuccess = false;

	bool bErrDisplay = (vbErrDisplay != VARIANT_FALSE);

	try
	{
		m_perr->InitializeForMethod("GetDataStoreTimeRange()",sFacilityID);

		if (!TestPointer(pFirstTimestamp) && !TestPointer(pLastTimestamp))
		{
			bSuccess = false;
			m_perr->ErrorInvalidPointer();
		}
		else
		{
			*pFirstTimestamp = 0.0;
			*pLastTimestamp = 0.0;
			//CString strResultFromDb;

			m_Db.msFacNum = sFacilityID;
			
			CGrandData Inst(&m_Db, (const int)lStationID, false, m_bQuietMode);	//SCR00240 Change station ID from short to long

			if (!m_Db.OpenTheDatabase(sFacilityID,&m_csResultFromDb))
			{
				bSuccess = false;
				m_perr->ErrorOpenDatabase(m_csResultFromDb);
			}

			if (m_Db.mbDatabaseOpen) 
			{
				if (!Inst.AnyDaysExistInDb(DB_END)) //from HN 5 Dec 2006
				{
					// There was no data in db.  
					// Set return value to VARIANT_TRUE and times to 0
                    //*pFirstTimestamp = 0; (done above).
                    //*pLastTimestamp = 0;
                    bSuccess = true;
                }
				else
				{
					//CString csDBError;

					bSuccess = Inst.GetFirstTimeInDb(pFirstTimestamp, bErrDisplay);

					if (!bSuccess)
					{
						Inst.GetDatabaseErrorText(m_csDBError);
						m_perr->ErrorDatabase(m_csDBType, m_csDBError);
					}
					else //if (bSuccess)
					{
						bSuccess = Inst.GetLastTimeInDb(pLastTimestamp, bErrDisplay);

						if (!bSuccess)
						{
							Inst.GetDatabaseErrorText(m_csDBError);
							m_perr->ErrorDatabase(m_csDBType, m_csDBError);
						}
					}
				}
			}
		}
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	m_Db.CloseDatabase(); // 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}



//	Gets day statistics for the specified time range.
// 26-Oct-2004	SFK	Modified to support opening database
STDMETHODIMP CGrandImport::GetDayStatistics(
	short sFacilityID, 
	long lStationID, 
	DATE StartTime, 
	DATE EndTime, 
	DATE *pActualStartTime, 
	DATE *pActualEndTime,
	long *plNumDays, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bSuccess = false;

	try
	{
		m_perr->InitializeForMethod("GetDayStatistics()",sFacilityID,lStationID);


		//Check for good pointers....
		if (!TestPointer(pActualStartTime) ||
			!TestPointer(pActualEndTime) ||
			!TestPointer(plNumDays) ||
			!TestPointer(pvbResult))
		{
			m_perr->ErrorInvalidPointer();
		}
		else  //finally - do what the method requires
		{
			*pActualStartTime = 0;
			*pActualEndTime = 0;
			*plNumDays = 0;

			CGrandData Inst(&m_Db, (const int)lStationID, false, m_bQuietMode);	//SCR00240 Change station ID from short to long
			DB_D_INTERVAL Requested, Actual;

			m_Db.msFacNum = sFacilityID;

			CString strResultFromDb;
			if (!m_Db.OpenTheDatabase(sFacilityID,&strResultFromDb))
				m_perr->ErrorOpenDatabase(strResultFromDb);

			if (m_Db.mbDatabaseOpen) 
			{
				Requested.dStart = StartTime;
				Requested.dEnd = EndTime;
				CString cserr;
				bSuccess = Inst.GetDayStatistics(Requested, &Actual, &cserr);

				if (bSuccess) 
				{	// extract info
					*pActualStartTime = Actual.dStart;
					*pActualEndTime = Actual.dEnd;
					*plNumDays = Inst.mulNumPtsActual;
				}
				else  //SCR00237 process the error
				{
					CString csDBError;
					Inst.GetDatabaseErrorText(csDBError);
					m_perr->ErrorDatabase(m_csDBType, csDBError);
				}
			}
		} //end else
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE; 
 
	m_Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}





STDMETHODIMP CGrandImport::GetFilesList(
	short sFacilityID, 
	BSTR bstrRootDir, 
	VARIANT_BOOL vbIncludeSubDirs, 
	VARIANT_BOOL vbMostRecent, 
	VARIANT *psaFilesList)
{
	//TRACE("CGrandImport::GetFilesList()\n");

	AFX_MANAGE_STATE(AfxGetStaticModuleState( ))

	try
	{
		m_perr->InitializeForMethod("GetFilesList()");
		m_perr->ErrorUnimplementedMethod();

		if (TestPointer(psaFilesList))
			::VariantInit(psaFilesList);
	}
	catch(...)
	{
	}
	return S_OK;
}

/*

July 30, 2007 - PJM

NDAR delivery is August 1, 2007.  John Determan and Peggy Moore decided to change this to
an unimplemented method for the August delivery since it was not the "biggest alligator
in the swamp", so to speak.  The code is very likely good, but we didn't have the time
to test it appropriately.

// Get list of all files this COM knows about in the specified directory(ies)
// 26-Oct-2004	SFK	Added interface but did not implement
// xx-Jun-2005  hn  Added code to support the interface
// 05-Jul-2005	SFK	Added open/close of database
STDMETHODIMP CGrandImport::GetFilesList(
	short sFacilityID, 
	BSTR bstrRootDir, 
	VARIANT_BOOL vbIncludeSubDirs, 
	VARIANT_BOOL vbMostRecent, 
	VARIANT *psaFilesList)
{
	//TRACE("CGrandImport::GetFilesList()\n");

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bSuccess = true;	//Reverse of the others...
	bool bIncludeSubDirs = (vbIncludeSubDirs != VARIANT_FALSE);
	bool bMostRecent = (vbMostRecent != VARIANT_FALSE);



	// Added implementation 6/8/05 to recursively search folders for *.BID files.
	// After the search is complete with FileFinder, we add each *.BID file to a list
	// that is used by ImportManager.  To determine the file date, we use the CGrandDataFile
	// ReadHeader function and populate the list of FileRecords that is passed as a safe
	// array to the ImportManager interface calls.  This was also flagrantly stolen
	// from Kelly Michel. hn 6/8
	try
	{
		m_perr->InitializeForMethod("GetFilesList()",sFacilityID);

		CString Msg;
		SAFEARRAY *pSA;
		SAFEARRAYBOUND bounds[2];
		long lDimension[2];
		//HRESULT hr;
		POSITION Pos;
		tFileRecord FileRecord;
		CList<tFileRecord, tFileRecord> FileList;

		//Initialize safearray.
		::VariantInit(psaFilesList);

		CString strResultFromDb;
		if (!m_Db.OpenTheDatabase(sFacilityID,&strResultFromDb))
			m_perr->ErrorOpenDatabase(strResultFromDb);

		if (!m_Db.mbDatabaseOpen)
		{
			bSuccess = false;  //Open above failed. 
		}
		else
		{
			CGrandDataFile mgFile(&m_FacCfg,m_bQuietMode);
			//The GrandDataFile class encapsulates the search function for us.
			CString csRootDir(bstrRootDir);
			mgFile.GetCompleteFileList(sFacilityID, &FileList, csRootDir, bIncludeSubDirs);
			//If the request is for the most recent data, then determine what the latest date of the data
			//in the database is.

			//*********************************************************************************
			//If there were files found, then load the contents of the linked list into the 
			//SAFEARRAY.  If there are no files, then set the VARIANT to indicate the array is EMPTY.
			//*********************************************************************************
			if(0 != FileList.GetCount())
			{
				long LastStationID = -1;
				VARIANT_BOOL vbSuccess;
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
							GetDataStoreTimeRange(sFacilityID, FileRecord.StationID.lVal, VARIANT_FALSE, &StartTime, 
								&EndTime, &vbSuccess);
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
					if (!bSuccess)
					{
						m_perr->ErrorSAFEARRAY(Msg);
						break;
					}

					FileRecord = FileList.GetNext(Pos);

					if(bMostRecent && (FileRecord.StationID.lVal != LastStationID))
					{
						LastStationID = FileRecord.StationID.lVal;
						//embedding a method call in a method call will confuse the error processing,
						//so reinitialize each time
						GetDataStoreTimeRange(sFacilityID, FileRecord.StationID.lVal, VARIANT_FALSE, &StartTime, 
							&EndTime, &vbSuccess);
						m_perr->InitializeForMethod("GetFilesList()",sFacilityID);
					}

				//	if(!bMostRecent || (bMostRecent && FileRecord.Date.date > EndTime))	// 06-Jul-2005 SFK Changed logic to following line
					if (!bMostRecent || (bMostRecent && (int)FileRecord.Date.date >= (int)EndTime))	// 06-Jul-2005 SFK Need to include the last day in the database
					{
						//*****************************
						//Load the FILE DATA DATE
						//*****************************
						lDimension[1] = 0;	//COLUMN
						if(S_OK != SafeArrayPutElement(pSA, lDimension, &FileRecord.Date))
						{
							Msg.Format("Failed to load DATE array element [%d][0]", lDimension[0]);
							bSuccess = false;
						}
						//******************************
						//Load the STATION ID
						//******************************
						if (bSuccess)
						{
							lDimension[1] = 1;	//COLUMN
							if(S_OK != SafeArrayPutElement(pSA, lDimension, &FileRecord.StationID))
							{
								Msg.Format("Failed to load STATION ID array element [%d][1]", lDimension[0]);
								bSuccess = false;
							}
						}
						//*****************************
						//Load the FILE NAME
						//*****************************
						if (bSuccess)
						{
							lDimension[1] = 2;	//COLUMN
							if(S_OK != SafeArrayPutElement(pSA, lDimension, &FileRecord.File))
							{
								Msg.Format(" Failed to load FILE NAME array element [%d][2]", lDimension[0]);
								bSuccess = false;
							}
						}
						lDimension[0]++;  //Row
					}
				}//end WHILE Pos != NULL
				
				//Release the locks on the data in the array.
				SafeArrayUnaccessData(pSA);
				
				//Set the return value
				psaFilesList->parray = pSA;
			}
			else //No files found.
			{
				bSuccess = false;
				psaFilesList->vt = VT_EMPTY;
				CString cs = "No files found." + Msg;
				m_perr->ErrorFindFile(CString(""), cs);
			}
		}  //database open
	}  //end try

	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	m_Db.CloseDatabase();			// 6-Jul-2005 SFK	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}

*/
STDMETHODIMP CGrandImport::GetParameters(
	short notused,  /*sFacilityID*/
	VARIANT* pvParms, 
	VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::GetParameters()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bool bSuccess = false;

	try
	{
		m_perr->InitializeForMethod("GetParameters()");

		if (!TestPointer(pvParms))
		{
			m_perr->ErrorInvalidPointer();
		}
		else
		{
			::VariantClear(pvParms);

			HRESULT hr;

			SAFEARRAYBOUND boundsmain;
			long lDimensionmain;
			_variant_t var;

			long value = m_sVerbosityLevel;

			if (m_bQuietMode)
				value = value | 0x00010000;

			if (m_bDisableLogger)
				value = value | 0x00020000;

			var = value;

			boundsmain.cElements = 1;  
			boundsmain.lLbound = 0;
			pvParms->vt = VT_VARIANT | VT_ARRAY;
			pvParms->parray = SafeArrayCreate(VT_VARIANT, 1, &boundsmain);
			lDimensionmain = 0;
			hr = SafeArrayPutElement(pvParms->parray, &lDimensionmain, &var);
			bSuccess = (hr == S_OK);
		}
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE; 

	return S_OK;
}

//	Common COM function to do any special action to initialize a data store
//	26-Oct-2004	SFK	Added database open, if necessary.  Also changed calling argument to have FacilityID rather the db path
STDMETHODIMP CGrandImport::InitializeDataStore(
	short sFacilityID, 
	VARIANT_BOOL vbQuestion, 
	VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::InitializeDataStore()\n");

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bSuccess = false;

	bool bQuestion = (vbQuestion != VARIANT_FALSE);
	if (m_bQuietMode)
		bQuestion = false;



	try
	{
		m_perr->InitializeForMethod("InitializeDataStore()",sFacilityID);

		//CString strResultFromDb;
		if (!m_Db.OpenTheDatabase(sFacilityID,&m_csResultFromDb))
			m_perr->ErrorOpenDatabase(m_csResultFromDb);

		if (m_Db.mbDatabaseOpen) 
		{
			bSuccess = m_Db.InitializeDatabase(m_Db.mstrDatabaseRootPath, bQuestion);

			if (!bSuccess) //SCR00237
			{
				//CString cs;
				m_cs.Format("Root path:%s",m_Db.mstrDatabaseRootPath);
				m_perr->ErrorDatabase(m_csDBType, m_cs);
			}
		}
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	m_Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}

// Common COM function to do any special action to open a data store
//	26-Oct-2004	SFK	Changed calling argument from path to FacilityID?
STDMETHODIMP CGrandImport::OpenDataStore(short sFacilityID, VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::OpenDataStore()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bool bSuccess = false;

	try
	{	
		m_perr->InitializeForMethod("OpenDataStore()",sFacilityID);

		//CString strResultFromDb;
		if (!m_Db.OpenTheDatabase(sFacilityID,&m_csResultFromDb))
			m_perr->ErrorOpenDatabase(m_csResultFromDb);

		if (m_Db.mbDatabaseOpen)
			bSuccess = true;
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	m_Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}


//	Common COM function to restore any COM specific files from a designated location
// 26-Oct-2004	SFK	Changed to have sFacilityID instead of bstrDestPath in arguments, opened db if necessary
STDMETHODIMP CGrandImport::RestoreDataStore(
	short sFacilityID, 
	BSTR bstrSourcePath, 
	VARIANT_BOOL vbQuestion, 
	VARIANT_BOOL vbDefault, 
	VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::RestoreDataStore()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bSuccess = false;
	bool bQuestion = (vbQuestion != VARIANT_FALSE);

	if(m_bQuietMode)
		bQuestion = false;

	bool bDefault = (vbDefault != VARIANT_FALSE);

	try
	{
		m_perr->InitializeForMethod("RestoreDataStore()",sFacilityID);

		CString strSrcPath(bstrSourcePath);

		if (strSrcPath.IsEmpty())
		{
			CString strError = CString("Source path is empty.");
			m_perr->ErrorOpenFile(CString(""), strError);
		}
		else
		{
			CString strResultFromDb;
			if (!m_Db.OpenTheDatabase(sFacilityID,&strResultFromDb))
				m_perr->ErrorOpenDatabase(strResultFromDb);

			if (m_Db.mbDatabaseOpen) 
			{
				CString strDestPath = m_Db.mstrDatabaseRootPath;

				if (!m_Db.mstrDatabaseRootPath.IsEmpty())
					bSuccess = m_Db.RestoreDatabase(strSrcPath, strDestPath, bQuestion, bDefault);
			}

			if (!bSuccess) //SCR00237
			{
				CString cs;
				cs.Format("Source path = %s",strSrcPath);
				m_perr->ErrorDatabase(m_csDBType,cs);
			}
		}
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	m_Db.CloseDatabase();			// 12-Jan-2005	Close db after each action to allow 2 programs to use the COM
	return S_OK;
}


STDMETHODIMP CGrandImport::SetParameters(
	short sFacilityID,
	VARIANT vData, 
	VARIANT_BOOL *pvbResult)
{
	//TRACE("CGrandImport::SetParameters()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bool bSuccess = false;

//#ifdef _DEBUG
//	VGetImage(vData);
//#endif

	VARIANT var;
	::VariantInit(&var);

	try
	{
		m_perr->InitializeForMethod("SetParameters()");

		HRESULT hr1,hr2,hr3;

		m_strFacilityName = m_FacCfg.GetFacilityLongName(sFacilityID);
		m_strFacilityName.MakeUpper();

		//THIS IS HOW THE SPEC SAYS TO DO IT. PJM 26-NOV-2007
		if (vData.vt == VT_I4)
		{
			long lfourbytes = vData.lVal;

			m_FacCfg.SetFlags(lfourbytes);	//16 July 2007 pjm

			m_sVerbosityLevel = (short)(lfourbytes & 0x0000ffff);  //1 - 6

			short high_16 = (short)(lfourbytes >> 16);

			m_bQuietMode = (0 != (high_16 & 0x0001)); //quiet mode bit is bit 0
			m_Dir.SetQuietMode(m_bQuietMode);
			m_Db.SetQuietMode(m_bQuietMode);

			m_bDisableLogger = (0 != (high_16 & 0x0002)); //disable logger setting is bit 1

			bSuccess = true;

			m_perr->SetFacilityName(m_strFacilityName);
			m_perr->SetVerbosity(m_sVerbosityLevel);
			m_perr->SetDisableLogger(m_bDisableLogger);
		}
		//THIS IS HOW NDAR DOES IT. PJM 26-NOV-2007
		else if (vData.vt == (VT_VARIANT | VT_ARRAY)) 	// is it the type of SafeArray we are expecting?
		{
			UINT uiDim=0;
			long iUBound=-1, iLBound=-1;
			long lDimensionmain;

			uiDim = SafeArrayGetDim(vData.parray);

			if(uiDim == 1)
			{
				hr1 = SafeArrayGetUBound(vData.parray, 1, &iUBound);
				hr2 = SafeArrayGetLBound(vData.parray, 1, &iLBound);

				if ((hr1 == S_OK) && (hr2 == S_OK) && (iUBound >= 0))
				{
					lDimensionmain = iLBound;
					hr3 = SafeArrayGetElement(vData.parray, &lDimensionmain, &var);
					if (hr3 == S_OK)
					{
						long lfourbytes = var.lVal;

						m_FacCfg.SetFlags(lfourbytes);	//16 July 2007 pjm

						m_sVerbosityLevel = (short)(lfourbytes & 0x0000ffff);  //1 - 6

						short high_16 = (short)(lfourbytes >> 16);

						m_bQuietMode = (0 != (high_16 & 0x0001)); //quiet mode bit is bit 0
						m_Dir.SetQuietMode(m_bQuietMode);
						m_Db.SetQuietMode(m_bQuietMode);

						m_bDisableLogger = (0 != (high_16 & 0x0002)); //disable logger setting is bit 1

						bSuccess = true;

						m_perr->SetFacilityName(m_strFacilityName);
						m_perr->SetVerbosity(m_sVerbosityLevel);
						m_perr->SetDisableLogger(m_bDisableLogger);
					}
				}
			}
		}

		//JCD - startup message
		//This happens in a smarter CErrorProcessing class now.
		//if (m_perr) m_perr->StartTime(m_strFacilityName);
	}
	catch (_com_error &e)
	{
		bSuccess = false;
		if(m_perr) m_perr->COMErrorCatch(e);
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	::VariantClear(&var);

	return S_OK;
}


STDMETHODIMP CGrandImport::GetDataStoreStatus(short sFacilityID, long* plStatus)
{
	//TRACE("CGrandImport::GetDataStoreStatus()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bSuccess = true;  //Opposite of most other method initializations.
	long lstatus = 0;  //unknown

	try
	{
		m_perr->InitializeForMethod("GetDataStoreStatus()", sFacilityID);

		if (!TestPointer(plStatus))
		{
			m_perr->ErrorInvalidPointer();
			return S_OK;
		}

		//1 = database ok
		//2 = database is corrupted, user should take steps to repair 
		//    before the database can be continue to be accessed/used
		//3 = database is within 5% of maximum database size
		//4 = database size is over the maximum recommended database size
		//5 = database cannot be opened

		*plStatus = 1;  //Assume all is well...

		CString strFacilityDirectory = m_FacCfg.GetFacilityDirectory(sFacilityID);

		if (strFacilityDirectory.IsEmpty())
		{
			m_perr->ErrorFindFile(m_csNULL, CString("Facility directory name is blank."));
			bSuccess = false;
			*plStatus = ~0;  //set to all ones
		}
		else
		{
			strFacilityDirectory = m_Dir.AppendPath(strFacilityDirectory, m_Db.mstrComponentName);
			CString strPathAndFilename = strFacilityDirectory + "\\" + m_Db.mstrDatabaseName + ".dbd";

			// See if the database exists
			//
			if (!m_Dir.FileExists (strFacilityDirectory, m_Db.mstrDatabaseName + ".dbd"))
			{
				m_perr->ErrorFindDatabase(strPathAndFilename, CString(""));
				*plStatus = 5;	//5 = database cannot be opened (because it doesn't exist)
				return S_OK;
			}

			//save open or closed state for when we exit with database OK.
			bool b_coming_in_state_is_opened = m_Db.mbDatabaseOpen;

			//if the database is open, close it
			if (m_Db.mbDatabaseOpen)
				m_Db.CloseDatabase();

			// See if we can open the existing database
			if (!m_Db.OpenDatabase(strFacilityDirectory, m_Db.mstrDatabaseName))
			{
				*plStatus = 5;	//5 = database cannot be opened
				m_perr->ErrorOpenDatabase(CString(" "));
				return S_OK;
			}

			// Determine if database is corrupted
			// (Make a dummy call dt_internals)
			char buf[_MAX_PATH];

			int istatus = dt_internals(&Currtask,TOPIC_GLOBALS,ID_DBDPATH,0,(void *)buf,_MAX_PATH);

			if (istatus <= -900 && istatus >= -999)  //range of fatal errors (usually indicates corruption)
			{
				*plStatus = 2; //database is corrupted
				CString cs;
				cs.Format("Fatal error %d",istatus);
				m_perr->ErrorDatabase(m_csDBType,cs);
				return S_OK;
			}

			// Get the size of the database in bytes
			double dSize = 0.0;
			m_Dir.GetDirectorySize(m_Db.mstrDatabasePath, &dSize);

			if(dSize>0.95*pow(2.0,31))
				*plStatus = 3;

			else if(dSize>0.8*pow(2.0,31))
				*plStatus = 4;

			if (b_coming_in_state_is_opened && !m_Db.mbDatabaseOpen)
				m_Db.OpenDatabase(strFacilityDirectory, m_Db.mstrDatabaseName);

			else if (!b_coming_in_state_is_opened && m_Db.mbDatabaseOpen)
				m_Db.CloseDatabase(); 
		}
	}
	catch(...)
	{
		bSuccess = false;
		if (m_perr) m_perr->ErrorCatch();
		*plStatus = ~0;  //set to all ones
	}
	return S_OK;
}
//SCR00237: Add four methods to COM interface to support Rokkasho/NDAR requirements
//pjm 26-Jan-2006 (for the stub)
//pjm 3-Feb-2006 (for the code)
STDMETHODIMP CGrandImport::GetLastError(short /*sFacilityID*/, BSTR *pbstrErrortext, long* plError)
{
	//TRACE("CGrandImport::GetLastError()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	try
	{
		//Nice place for a test point...
		//Beep(200,200);
		//VARIANT_BOOL vb;
		//DeleteDataStore(sFacilityID,&vb);

		*plError = m_perr->m_lLastErrorValue;
		*pbstrErrortext = m_perr->m_csLastErrorText.AllocSysString();
	}
	catch(...)
	{
	}
	return S_OK;
}

//pjm 3-31-2006  (This eliminates asking for more points than exist.)
long CGrandImport::GetNumberOfPointsToRequest(
	short sFacilityID, long lStationID, DATE StartTime, DATE EndTime, long asked_for)
{
	//TRACE("CGrandImport::GetNumberOfPointsToRequest()\n");

	long number_of_points_in_time_interval;
	long number_of_points_to_request = -1;
	DATE dactualstart, dactualend; 
	VARIANT_BOOL vb;

	GetDataStatistics(
		sFacilityID, 
		lStationID, 
		0,				//Don't care about channel here.
		StartTime, 
		EndTime, 
		&dactualstart, 
		&dactualend,
		&number_of_points_in_time_interval,  //long
		&vb);

	if (vb != VARIANT_FALSE)
	{
		if (asked_for == 0)  //ask for all of them
		{
			number_of_points_to_request = number_of_points_in_time_interval;
		}
		else  //ask for smaller of input request or total number of points available.
		{
			if (asked_for > number_of_points_in_time_interval) //asked for too many
				number_of_points_to_request = number_of_points_in_time_interval;
			else
				number_of_points_to_request = asked_for;
		}
	}
	return number_of_points_to_request;
}

//void CGrandImport::InitializeErrorProcessing(bool bDisableLogging)
void CGrandImport::InitializeErrorProcessing()
{
	//TRACE("CGrandImport::InitializeErrorProcessing()\n");

	//Build up the fully qualified name of the INI file for error messages
	char sz[MAX_PATH];
	::GetModuleFileName(
		AfxGetInstanceHandle(),
		sz,sizeof(sz));				//fully qualifed filename of this DLL

	char *c = strrchr(sz,'\\');		//find the last "\"
	*c = '\0';						//make it null to wipe out name of this DLL
	strcat(sz,"\\");				//put back the last "\"
	strcat(sz,"CommonErrorMessages.ini");


	m_perr = new CErrorProcessing(
		CLSID_GrandImport, 
		m_sStartUpTimeMillisec,
		sz,
		m_sVerbosityLevel,
		"GrandImport",m_StartProcess);
}


