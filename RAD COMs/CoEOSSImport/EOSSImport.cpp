// EOSSImport.cpp

#include "stdafx.h"
#include "CoEOSSImport.h"
#include "EOSSImport.h"
#include "EOSSDataFile.h"
#include "EOSSData.h"
#include "SafeArrayUtil.h"
#include "commondefines.h"
#include "EOSSCreateAccessDatabase.h"
#include "imagehlp.h"
#include <direct.h>

/////////////////////////////////////////////////////////////////////////////
// CEOSSImport

CEOSSImport::CEOSSImport()
	: m_csNULL(""),
	  m_csColon(",")
{
//	TRACE("CEOSSImport::CTOR()\n");
	try
	{
		// defaults for rad
		m_sVerbosityLevel = 2;
		m_bDisableLogger = true;
		m_bQuietMode = false;
		
		m_bDatabaseOpen = false;
		
		char* pbuff = new char [MAX_ERROR];
		if (_getcwd (pbuff,MAX_ERROR) != NULL)
			m_csErrorINIFile.Format ("%s\\%s",pbuff, ERROR_INI);
		else
			m_csErrorINIFile = ERROR_INI;

		if (pbuff)
		{
			delete pbuff;
			pbuff = NULL;
		}
		g_pIFacMgr.CreateInstance (CLSID_FacilityMgrData);

		g_pIFacMgr->SetFacilityMgrErrorLoggingParameters (DISABLE_LOGGER | QUIET_MODE_OFF | VERBOSITY3_NONFATAL);	//0x00030002


		m_pDb = new CEOSSDbAccess (/*m_pIFacMgr*/);
		m_bStartupCalled = false;
		
		m_csDBType = "DBAccess";

		m_pILogger = NULL;
		
		m_pChannelLookupTable = NULL;
		m_lNumDefinedStations = 0;
		m_pStationLookupTable = NULL;
		m_lNumDefinedChannels = 0;

		InitLogger ();
		
		m_StartProcess = m_StartProcess.GetCurrentTime();
		SYSTEMTIME SysTime;
		::GetSystemTime(&SysTime);
		m_sStartUpTimeMillisec = (short)SysTime.wMilliseconds; 

		m_strFacilityName = CString("UNITIALIZED FACILITY NAME");
		m_sFacID = -1;

		// Set these values to use in case of facility switch and no call to SetParameters
		VARIANT longvar;
		VariantInit(&varParms);
		VariantInit(&longvar);
		SAFEARRAYBOUND bound;
		bound.cElements = 1;
		bound.lLbound = 0;
		varParms.parray = SafeArrayCreate (VT_VARIANT,1,&bound);
		varParms.vt = VT_VARIANT|VT_ARRAY;
		longvar.vt = VT_I4;
		longvar.lVal = DISABLE_LOGGER | QUIET_MODE_OFF | VERBOSITY3_NONFATAL;
		LONG lDim [1];
		lDim[0] = 0;
		SafeArrayPutElement (varParms.parray,lDim,&longvar);

	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_SET_PARAMETERS);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_SET_PARAMETERS);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_SET_PARAMETERS);
	}	
}

CEOSSImport::~CEOSSImport()
{
//	TRACE("CEOSSImport::DTOR()\n");
	try
	{
		// Shutdown message
		CString now = COleDateTime::GetCurrentTime ().Format ();
		now = " " + now;
		LogMsg (now,EXITED);

		// Let's open and close the Acces DB only once
		if (m_pDb->mbDatabaseOpen)
		{
			CString err;
			// Only compact in destructor, this could be a time-consuming operation.
			//Peggy will do this in the CAdoDb destructor
			//m_pDb->CompactDb ();
			m_pDb->CloseDatabase ();
		}
		if (m_pDb)
		{
			delete m_pDb;
			m_pDb = NULL;
		}
		//if (m_pIFacMgr)
		//	m_pIFacMgr = NULL;
		if (m_pILogger)
			m_pILogger = NULL;
		if (varParms.vt != VT_EMPTY)
		{
			//This can blow up, remove 4-1-08 HN
			//SafeArrayDestroy (varParms.parray);
			VariantClear (&varParms);
		}
		if (m_pStationLookupTable)
		{
			delete [] m_pStationLookupTable;
			m_pStationLookupTable = NULL;
		}
		if (m_pChannelLookupTable)
		{
			delete [] m_pChannelLookupTable;
			m_pChannelLookupTable = NULL;
		}
		
	}
	catch(...)
	{
	}
}

// HN March 2008
// AddData - Read the specified data file and add to the data store.
// The read data file function came almost directly from Dave Pelowitz code used to 
// crack the ESS file, but it is stored in a DB
STDMETHODIMP CEOSSImport::AddData(
				short sFacilityID,
				BSTR bstrFilename,
				VARIANT_BOOL vbOverwrite,
				BSTR *bstrResult,
				VARIANT_BOOL *pvbResult)
{
//	TRACE ("CEOSSImport::AddData()\n");

	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	ClearError ();
	bool bSuccess = false;
	CString strResult;
	if (TestPointer(pvbResult))
		*pvbResult = VARIANT_FALSE;
	
	try
	{
		//Invalid facility
		if (g_pIFacMgr)
		{
			VARIANT_BOOL vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
			if (vbDefined == VARIANT_FALSE)
			{
				bSuccess = false;
				m_csError.Format ("FACILITY ID: %u",sFacilityID);
				LogMsg (m_csError,FACILITY_UNDEFINED);
			}
			else
				bSuccess = true;
		}
		
		if (bSuccess)
		{
			CEOSSDataFile File (g_pIFacMgr, m_bQuietMode);
			File.mbOverwrite = (vbOverwrite != VARIANT_FALSE);

			BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
			CString facDir (bstrFacDir);
			//Make sure the correct facility db is open
			if (!m_pDb->IsFacilityDBOpen (sFacilityID,facDir))
			{
				//Close this db, it is not the right one.
				m_pDb->CloseDatabase ();
				m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
				VARIANT_BOOL vbResult;
				SetParameters (sFacilityID,varParms,&vbResult);
			}
			
			// This function does all the data reading and writes to db.
			bSuccess = File.ReadDataFile(m_pDb, CString (bstrFilename), &strResult);
			if (!bSuccess)
			{
				LogMsg (strResult,FILE_READ_ERR);
			}
			//Send any errors or other info back in the bstr
			*bstrResult = strResult.AllocSysString ();
		}
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_ADD_DATA);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_ADD_DATA);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_ADD_DATA);
	}	
	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

//	BackupDataStore - Common COM function to backup any COM specific files to a designated location
//  March 2008, added code to backup the EOSS db to the specified location.
STDMETHODIMP CEOSSImport::BackupDataStore(
							short sFacilityID,
							BSTR bstrDestinationPath,
							VARIANT_BOOL bQuestion,
							VARIANT_BOOL bDefault,
							VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
//	TRACE ("CEOSSImport::BackupDataStore()\n");

	ClearError ();
	if (TestPointer ((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;
	bool bSuccess = false;

	try
	{
		//Invalid facility
		if (g_pIFacMgr)
		{
			VARIANT_BOOL vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
			if (vbDefined == VARIANT_FALSE)
			{
				bSuccess = false;
				m_csError.Format ("FACILITY ID: %u",sFacilityID);
				LogMsg (m_csError,FACILITY_UNDEFINED);

			}
			else
				bSuccess = true;
		}
	
		if (bSuccess)
		{
			// Make sure that \\ are there and there is only one.
			CString strDestPath ((BSTR)bstrDestinationPath);
			strDestPath.TrimRight ("\\");
			strDestPath+= "\\";

			CString strDBName;
			if (g_pIFacMgr)
			{
				// retrieve the location of the db requested.
				BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory(sFacilityID);
				CString strFacilityDirectory (bstrFacDir);
				strFacilityDirectory.TrimRight ("\\");
				strFacilityDirectory+= "\\Data\\Import\\EOSS\\";
				strDBName = strFacilityDirectory + EOSS_DB_NAME;
			}		
			
			CDirUtilities dir (m_bQuietMode);
			
			strDestPath = dir.AddDriveLetter (strDestPath);

			if(dir.FileExists(strDBName))
			{
				// Close it if it is open so that we don't get any sharing violations
				BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
				CString facDir (bstrFacDir);
				bool bOpen = m_pDb->IsFacilityDBOpen (sFacilityID,facDir);

				if (bOpen)
					m_pDb->CloseDatabase ();

				// See if there is already an EOSS db there
				// Make subdirs for Data\\import\\eoss
				strDestPath+= "\\Data\\Import\\EOSS\\";
				bool bExists = dir.FileExists (strDestPath+EOSS_DB_NAME);
				UINT answer = IDNO;
				
				if (!m_bQuietMode)
				{
					if (!bExists)
						bSuccess = true;
					else if (bExists && bQuestion != VARIANT_FALSE)
					{
						// we are supposed to ask, the result depends on their answer
						CString message;
						message.Format ("A database already exists at %s. Do you want to overwrite it?",strDestPath);
						answer = MessageBox (NULL,message,"DB already exists",MB_YESNO);
					}
					else if (bExists && bQuestion == VARIANT_FALSE && bDefault == VARIANT_FALSE)
					{
						bSuccess = false;//Not ok to overwrite, exists
						LogMsg (" database existed and overwrite option was not selected.",FILE_COPY_ERR);
					}
					else if (bExists && bQuestion == VARIANT_FALSE && bDefault != VARIANT_FALSE)
					{
						bSuccess = true;//OK to overwrite without asking
					}
					//Only use the ID value if they were asked the question.
					//HN 4-1-08
					if (bExists && bQuestion && answer == IDYES)
					{
						bSuccess = true;
					}
					else if (bExists && bQuestion && answer == IDNO)
					{
						bSuccess = false;
						//They chose not to overwrite
						LogMsg (" database existed and overwrite option was not selected.",FILE_COPY_ERR);
					}
					
				}
				else
				{
					// Quiet mode, just use the bDefault to decide.
					bSuccess = bDefault != VARIANT_FALSE?true:false;
				}

				if (bSuccess)
				{
					if (MakeSureDirectoryPathExists (strDestPath)==TRUE)
						bSuccess = CopyFile (strDBName,strDestPath+EOSS_DB_NAME,FALSE) !=0;
					if (!bSuccess)
					{
						LogMsg (strDBName,FILE_COPY_ERR);
					}
				}
			}
			else
			{
				//No db to back up
				bSuccess = false;
				LogMsg (strDBName + " did not exist.",FILE_COPY_ERR);
			}
		}
	}

	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_BACKUP_DATASTORE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_BACKUP_DATASTORE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_BACKUP_DATASTORE);
	}	
	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}



// HN March 2008
// Added code to close db after compacting.
//	CloseDataStore - Common COM function to do any special actions to close the data store
STDMETHODIMP CEOSSImport::CloseDataStore()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

//	TRACE ("CEOSSImport::CloseDataStore()\n");
	ClearError ();
	
	try
	{
		if (m_pDb->IsOpen())
		{
		//	m_pDb->CompactDb ();
			m_pDb->CloseDatabase ();
		}
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_CLOSE_DATASTORE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_CLOSE_DATASTORE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_CLOSE_DATASTORE);
	}	

	return S_OK;
}



// HN March 2008
// New method uses the ADOX library to create an access DB from scratch.
// See base class CCreateAccessDatabase and derived class CEOSSCreateAccessDatabase for
// implementation details. 
STDMETHODIMP CEOSSImport::CreateNewDataStore(
						short sFacilityID, 
						VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
//	TRACE ("CEOSSImport::CreateNewDataStore()\n");
	ClearError ();
	
	bool bSuccess = true;

	try
	{
		//Invalid station and/or facility
		if (g_pIFacMgr)
		{
			VARIANT_BOOL vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
			if (vbDefined == VARIANT_FALSE)
			{
				bSuccess = false;
				m_csError.Format ("FACILITY ID: %u",sFacilityID);
				LogMsg (m_csError,FACILITY_UNDEFINED);
			}
			else
				bSuccess = true;
		}
		if (bSuccess)
		{
			BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
			CString facDir (bstrFacDir);
			bool bAlreadyOpen = m_pDb->IsFacilityDBOpen (sFacilityID,facDir);
		
			// First, see if we already have a db open.
			if (bAlreadyOpen)
			{
				//This db is already open, why are we creating
				bSuccess = false;
				LogMsg ("datastore already exists and is open, use InitializeDataStore instead",11);
			}

			if (bSuccess)
			{
				//It wasn't open, try to create
				BSTR bstrPath;
				GetDataStorePath (sFacilityID,&bstrPath);
				CString csDBPath (bstrPath);
				CDirUtilities dir;
				bool bExists;
				bExists = dir.FileExists ((CString)csDBPath,(CString)EOSS_DB_NAME);
				SysFreeString (bstrPath);
				//Only do this if it doesn't exist
				if (!bExists)
				{
					CEOSSCreateAccessDatabase dbUtil (csDBPath,EOSS_DB_NAME,m_csErrorINIFile);
					bSuccess = dbUtil.CreateTables ();
				}
				else
				{
					LogMsg ("datastore already exists, use InitializeDataStore instead",11);
					bSuccess = false;
					BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
					CString facDir (bstrFacDir);
					bAlreadyOpen = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
				}
			}

			if (!bAlreadyOpen)
			{
				BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
				CString facDir (bstrFacDir);
				bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
			}
		}
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_CREATE_DATASTORE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_CREATE_DATASTORE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_CREATE_DATASTORE);
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}

//HN March 2008
// Added method to delete date from dtStartDate to dtEndDate in EOSS db
STDMETHODIMP CEOSSImport::DeleteData(
	short sFacilityID,
	long lStationID, 
	DATE dtStartDate,
	DATE dtEndDate, 
	VARIANT_BOOL* pvbResult)
{
//	TRACE("CEOSSImport::DeleteData()\n");

	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bool bSuccess = false;
	ClearError ();
	if (TestPointer ((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;
	try
	{
		VARIANT_BOOL vbDefined = VARIANT_FALSE;

		//Invalid station and/or facility
		if (g_pIFacMgr)
		{
			VARIANT_BOOL vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
			if (vbDefined == VARIANT_FALSE)
			{
				bSuccess = false;
				m_csError.Format ("FACILITY ID: %u",sFacilityID,lStationID);
				LogMsg (m_csError,FACILITY_UNDEFINED);
			}
			else
				bSuccess = true;

			if (bSuccess)
			{
				vbDefined = g_pIFacMgr->IsStationDefined (sFacilityID,lStationID,"EOSS");
				if (vbDefined == VARIANT_FALSE)
				{
					bSuccess = false;
					m_csError.Format ("FACILITY ID: %u STATION ID: %u",sFacilityID,lStationID);
					LogMsg (m_csError,STATION_UNDEFINED);
				}
				else
					bSuccess = true;
			}

		}
		if (dtStartDate >= dtEndDate)
			bSuccess = false;

		if (bSuccess)
		{
			BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
			CString facDir (bstrFacDir);
		
			//Make sure the correct facility db is open
			if (!m_pDb->IsFacilityDBOpen (sFacilityID,facDir))
			{
				//Close this db, it is not the right one.
				m_pDb->CloseDatabase ();
				m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
				VARIANT_BOOL vbResult;
				SetParameters (sFacilityID,varParms,&vbResult);
			}

			CEOSSData Inst(m_pDb, (const int)lStationID,m_bQuietMode);

			bSuccess = Inst.EOSSDeleteData (dtStartDate, dtEndDate);

			if (!bSuccess)
				LogMsg ("could not delete database records",DELETE_DB_ERR);
		}
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_CREATE_DATASTORE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_CREATE_DATASTORE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_CREATE_DATASTORE);
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}

//HN Marchh 2008
//DeleteDataStore method added to delete the *.MDB file for the EOSS datastore
STDMETHODIMP CEOSSImport::DeleteDataStore(
						short sFacilityID,
						VARIANT_BOOL* pvbResult)
{
//	TRACE("CEOSSImport::DeleteDataStore()\n");
	ClearError ();
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	bool bSuccess = false;
	if (TestPointer ((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;
	try
	{
		//Invalid station and/or facility
		if (g_pIFacMgr)
		{
			VARIANT_BOOL vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
			if (vbDefined == VARIANT_FALSE)
			{
				bSuccess = false;
			}
			else
				bSuccess = true;
		}
		
		CString dbPathAndName;
		
		if (bSuccess)
		{
			BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
			CString facDir (bstrFacDir);
			//Make sure the correct facility db is open
			if (m_pDb->IsFacilityDBOpen (sFacilityID,facDir))
			{
				//Close this db, We are deleting i
				m_pDb->CloseDatabase ();
			}

			//It wasn't open, try to create
			BSTR bstrPath;
			GetDataStorePath (sFacilityID,&bstrPath);
			CString csDBPath (bstrPath);
			CDirUtilities dir;
			bool bExists;
			bExists = dir.FileExists ((CString)csDBPath,(CString)EOSS_DB_NAME);
			SysFreeString (bstrPath);
			
			// Can't delete if it doesn't exist
			if (!bExists)
			{
				bSuccess = false;
				LogMsg ("database did not exist",DELETE_DB_ERR);
			}
			else
			{
				CString failed;
				bSuccess = dir.DeleteDirectory ((LPCTSTR)csDBPath,&failed);
				if (!bSuccess)
				{
					LogMsg (csDBPath,DIR_DEL_ERR);
				}
			}
		}
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_CREATE_DATASTORE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_CREATE_DATASTORE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_CREATE_DATASTORE);
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}

//	26-Oct-2004	Major changes to support returning data in a VARIANT struct rather than two arrays
STDMETHODIMP CEOSSImport::GetDataByChannel(
	short sFacilityID, 
	long lStationID, 
	short sChannelID, 
	DATE StartTime, 
	DATE EndTime, 
	VARIANT *pvData, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	ClearError ();
	
//	TRACE ("CoEOSSImport::GetDataByChannel.\n");

	bool bSuccess = false;
	if (TestPointer((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;
	try
	{
		VARIANT_BOOL vbDefined = VARIANT_FALSE;
	
		CEOSSData* pBInst = NULL;
		BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
		CString facDir (bstrFacDir);

		if (!m_pDb->IsFacilityDBOpen (sFacilityID,facDir))
		{
			//A different facility was open. Close and reopen
			m_pDb->CloseDatabase ();
			bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
			VARIANT_BOOL vbResult = VARIANT_FALSE;
			SetParameters (sFacilityID,varParms,&vbResult);
		}

		if (g_pIFacMgr)
		{
			vbDefined = g_pIFacMgr->IsStationDefined (sFacilityID,lStationID,"EOSS");
		}
		if (vbDefined != VARIANT_FALSE)
		{
			pBInst = new CEOSSData(m_pDb, (int)lStationID,m_bQuietMode);	// sk 04/15/2003
			
			// Lookup seal id in our tables
			// First find the Reader ID
			bool bFound = false;
			int stationIndex = -1;
			int index = 0;

			//while (!bFound & index < m_lNumDefinedStations) 
			while (!bFound && (index < m_lNumDefinedStations))  //pjm
			{
				bFound = m_pStationLookupTable[index] == lStationID;
				if (bFound)
					stationIndex = m_pStationLookupTable[index];
				index++;
			}
		
			if (stationIndex != -1)
			{
				//Get the seal ID
				short sSealID = -1;
				sSealID = m_pChannelLookupTable [(index-1)*MAX_SEALS+(sChannelID-1)];
				if (sSealID != -1)
					bSuccess = pBInst->GetEOSSSealData (lStationID, sSealID,pvData,StartTime, EndTime);
				else
				{
					bSuccess = false;
					m_csError.Format ("FACILITY ID: %u STATION ID: %u CHANNEL ID: %u",sFacilityID,lStationID,sChannelID);
					LogMsg (m_csError,CHAN_NOT_DEFINED);
				}
			}
			else
			{
				bSuccess = false;
				m_csError.Format ("FACILITY ID: %u STATION ID: %u CHANNEL ID: %u",sFacilityID,lStationID,sChannelID);
				LogMsg (m_csError,CHAN_NOT_DEFINED);
			}
		}
		else
		{
			m_csError.Format ("FACILITY ID: %u STATION ID: %u",sFacilityID,lStationID);
			LogMsg (m_csError,STATION_UNDEFINED);
			bSuccess = false;
		}
		if (pBInst)
		{
			delete pBInst;
			pBInst = NULL;
		}

	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_GET_DATA_CHANNEL);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_GET_DATA_CHANNEL);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_GET_DATA_CHANNEL);
	}

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}

//	Gets the data for the given station for the given interval.  SAFEARRAY for results is allocated here.
//	26-Oct-2004	SFK	Added putting NextTimestamp as last element of SAFEARRAY
STDMETHODIMP CEOSSImport::GetDataByStation(
	short sFacilityID, 
	long lStationID, 
	DATE StartTime, 
	DATE EndTime, 
	VARIANT *pvData, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

//	TRACE ("CoEOSSImport::GetDataByStation.\n");

	bool bSuccess = false;
	if (TestPointer ((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;
	ClearError ();
	
	try
	{
		VARIANT_BOOL vbDefined = VARIANT_FALSE;
	
		CEOSSData* pBInst = NULL;
		int nNumSealsDefinedForStation = 0;
		short sSealIDs [MAX_SEALS];
		
		BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
		CString facDir (bstrFacDir);

			
		if (!m_pDb->IsFacilityDBOpen (sFacilityID,facDir))
		{
			//A different facility was open. Close and reopen
			m_pDb->CloseDatabase ();
			bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
			VARIANT_BOOL vbResult = VARIANT_FALSE;
			SetParameters (sFacilityID,varParms,&vbResult);
		}

		if (g_pIFacMgr)
		{
			vbDefined = g_pIFacMgr->IsStationDefined (sFacilityID,lStationID,"EOSS");
		}
		if (vbDefined != VARIANT_FALSE)
		{
			pBInst = new CEOSSData(m_pDb, (int)lStationID,m_bQuietMode);	// sk 04/15/2003
							
			//We now have to get data by channel, and only here do we know
			// how to put in the SAFEARRAY, since we have the lookup table.
			bool bFound = false;
			int stationIndex = -1;
			int index = 0;
			while (!bFound & index < m_lNumDefinedStations) 
			{
				bFound = m_pStationLookupTable[index] == lStationID;
				if (bFound)
					//stationIndex = m_pStationLookupTable[index];
					stationIndex = index;
				index++;
			}
		
			if (stationIndex != -1)
			{
				//Get the seal IDs
				short sSealID = -1;
				int ChannelIndex = (index -1) *MAX_SEALS;
				while (m_pChannelLookupTable[ChannelIndex] != -1 && nNumSealsDefinedForStation < MAX_SEALS)
				{
					sSealIDs[nNumSealsDefinedForStation] = m_pChannelLookupTable [ChannelIndex];
					nNumSealsDefinedForStation++;
					ChannelIndex++;
				}
				bSuccess = true;
			}
			else
			{
				bSuccess = false;
			}

			if (bSuccess)
			{

				//We will fill the VARIANT with the safearrays
				pvData->vt = VT_VARIANT|VT_ARRAY;
				SAFEARRAYBOUND saBound[2];
				saBound[0].cElements = nNumSealsDefinedForStation;
				saBound[0].lLbound = 0;
				saBound[1].cElements = 3;
				saBound[1].lLbound = 0;
				pvData->parray = SafeArrayCreate (VT_VARIANT,2,saBound);

				pBInst = new CEOSSData (m_pDb,lStationID,m_bQuietMode);
				
				for (int i = 0; i < nNumSealsDefinedForStation; i ++)
				{
					VARIANT vChannelData;
					VariantInit (&vChannelData);
					long l[2];
					l[0]= (long) i;
					l[1]=1;
					DATE actualStart = 0.0;
					DATE actualEnd = 0.0;
					long lDays = 0;
					
					pBInst->GetEOSSDataStatistics (sSealIDs[i],StartTime,EndTime,&actualStart,&actualEnd,&lDays);
					bSuccess = pBInst->GetEOSSSealData (lStationID,sSealIDs[i],&vChannelData,StartTime,EndTime);

					if (bSuccess)
					{
						SA_PutElem (pvData->parray,i,0,COleDateTime(actualStart));
						SafeArrayPutElement (pvData->parray,l,&vChannelData);
						SA_PutElem (pvData->parray,i,2,COleDateTime(actualEnd));
					}
					else
					{
						SA_PutElem (pvData->parray,i,0,COleDateTime(0.0));
						vChannelData.vt = VT_EMPTY;
						vChannelData.parray = NULL;
						SafeArrayPutElement (pvData->parray,l,&vChannelData);
						SA_PutElem (pvData->parray,i,2,COleDateTime(0.0));
					}
				}
			}

		}
		else
		{
			m_csError.Format ("STATION ID: %u",lStationID);
			LogMsg (m_csError,STATION_UNDEFINED);
			bSuccess = false;
		}
		if (TestPointer ((void*)pvbResult))
			*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

		if (pBInst)
		{
			delete pBInst;
			pBInst = NULL;
		}

	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_GET_DATA_STATION);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_GET_DATA_STATION);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_GET_DATA_STATION);
	}
	return S_OK;
}



// 26-Oct-2004	SFK	Added db open, if needed
STDMETHODIMP CEOSSImport::GetDataStatistics(
	short sFacilityID, 
	long  lStationID,  
	short sChannelID, 
	DATE StartTime, 
	DATE EndTime, 
	DATE *pActualStartTime, 
	DATE *pActualEndTime, 
	long *plNumMembers, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	bool bSuccess = false;
	
//	TRACE ("CEOSSImport::GetDataStatistics.\n");
	if (TestPointer ((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;
	ClearError ();
	
	try
	{
		if (TestPointer((void*)pActualStartTime));
			*pActualStartTime = 0;
		if (TestPointer((void*)pActualEndTime));
			*pActualEndTime = 0;
		if (TestPointer((void*)plNumMembers));
			*plNumMembers = 0;

		VARIANT_BOOL vbDefined = VARIANT_FALSE;
		//Check that facility exists
		if (g_pIFacMgr)
		{
			bSuccess = g_pIFacMgr->IsFacilityDefined (sFacilityID)==VARIANT_FALSE?false:true;
		}
		if (bSuccess)
		{
			BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
			CString facDir (bstrFacDir);
			if (!m_pDb->IsFacilityDBOpen (sFacilityID,facDir))
			{
				//A different facility was open. Close and reopen
				m_pDb->CloseDatabase ();
				bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
				VARIANT_BOOL vbResult = VARIANT_FALSE;
				SetParameters (sFacilityID,varParms,&vbResult);
			}
			if (lStationID != -1)
			// They are asking for a specific station
			{
				//Check to see that station exists			
				if (g_pIFacMgr)
				{
					bSuccess = g_pIFacMgr->IsStationDefined (sFacilityID,lStationID,"EOSS")==VARIANT_FALSE?false:true;
				}
				if (bSuccess)
				//station is defined
				{
					short sSealID;
					if (sChannelID != -1)
					{
						// Lookup seal id in our tables if they aren't asking for station statistics
						// First find the Reader ID
						bool bFound = false;
						int stationIndex = -1;
						int index = 0;
						while (!bFound & index < m_lNumDefinedStations) 
						{
							bFound = m_pStationLookupTable[index] == lStationID;
							if (bFound)
								stationIndex = m_pStationLookupTable[index];
							index++;
						}
			
						if (stationIndex != -1)
						{
							//Get the seal ID
							sSealID = -2;
							sSealID = m_pChannelLookupTable [(index-1)*MAX_SEALS+(sChannelID-1)];
							if (sSealID == -1)
							{
								//Channel not found/not enabled.
								bSuccess = false;
								m_csError.Format ("FACILITY ID: %u STATION ID: %u CHANNEL ID: %u",sFacilityID,lStationID,sChannelID);
								LogMsg (m_csError, CHAN_NOT_DEFINED);
							}
						}
						else
						{
							//Could not be found.
							m_csError.Format ("FACILITY ID: %u STATION ID: %u",sFacilityID,lStationID);
							LogMsg (m_csError,STATION_UNDEFINED);	
							bSuccess = false;
						}
					}
					else
					{
						//Looking for all seals for station, station is valid.
						sSealID = -1;
					}
					if (bSuccess)
					{
						CEOSSData BInst (m_pDb,lStationID,m_bQuietMode);
						bSuccess = BInst.GetEOSSDataStatistics (sSealID,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers);
					}
			
				}			
				else
				{
					m_csError.Format ("FACILITY ID: %u STATION ID: %u",sFacilityID,lStationID);
					LogMsg (m_csError,STATION_UNDEFINED);
					bSuccess = false;
				}
			}
			else
			{
				// They are asking for all stations
				CEOSSData BInst (m_pDb,-1,m_bQuietMode);
				bSuccess = BInst.GetEOSSDataStatistics (-1,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers);

			}
		}
		else
		{
			// Facility not defined
			m_csError.Format ("FACILITY ID: %u",sFacilityID);
			LogMsg (m_csError,FACILITY_UNDEFINED);
			bSuccess = false;
		}

		if (TestPointer ((void*)pvbResult))
			*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_GET_DATA_STATISTICS);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_GET_DATA_STATISTICS);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_GET_DATA_STATISTICS);
	}
	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}



//	GetDataStore - Common COM function to get the current data store path
// 26-Oct-2004	SFK	Changed name from GetDataStore -- added sFacilityID argument.  Also changed
//					to return path with COM directories appended
STDMETHODIMP CEOSSImport::GetDataStorePath(short sFacilityID, BSTR* bstrStorePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
//	TRACE ("CEOSSImport::GetDataStorePath.\n");

	ClearError ();
	try
	{
		bool bSuccess = false;
		if (g_pIFacMgr)
		{
			bSuccess = g_pIFacMgr->IsFacilityDefined (sFacilityID)==VARIANT_FALSE?false:true;
		}
		
		*bstrStorePath = A2BSTR("");

		if (g_pIFacMgr != NULL && bSuccess)
		{
			BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory(sFacilityID);
			CString strFacilityDirectory (bstrFacDir);
			strFacilityDirectory.TrimRight ("\\");
			strFacilityDirectory+= "\\Data\\Import\\EOSS\\";

			*bstrStorePath = strFacilityDirectory.AllocSysString ();
		}
		else
		{
			// Facility not defined
			m_csError.Format ("FACILITY ID: %u",sFacilityID);
			LogMsg (m_csError,FACILITY_UNDEFINED);
			bSuccess = false;
		}
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_PATH);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_PATH);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_PATH);
	}	

	return S_OK;
}



//	Common COM function to get the size of the database
// 26-Oct-2004	SFK	Modified to support opening database
STDMETHODIMP CEOSSImport::GetDataStoreSize(short sFacilityID,
										   double *pdSizeInBytes, 
										   VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	//CDirUtilities Dir;
	BOOL fOk;
	ClearError ();
	
	if (TestPointer ((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;

	*pdSizeInBytes = 0;
//	TRACE ("CEOSSImport::GetDataStoreSize.\n");
	try
	{
		
		bool bSuccess = false;
		if (g_pIFacMgr)
		{
			bSuccess = g_pIFacMgr->IsFacilityDefined (sFacilityID)==VARIANT_FALSE?false:true;
		}
		
		if (bSuccess)
		{
			BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory(sFacilityID);
			CString strFacilityDirectory (bstrFacDir);
			strFacilityDirectory.TrimRight ("\\");
			strFacilityDirectory+= "\\Data\\Import\\EOSS\\";
			CString strDBName = strFacilityDirectory + EOSS_DB_NAME;
			
			WIN32_FILE_ATTRIBUTE_DATA fileInfo;

		    fOk = GetFileAttributesEx((LPCTSTR)strDBName, GetFileExInfoStandard, (void*)&fileInfo);
			
			if (fOk)
				fOk = (0 == fileInfo.nFileSizeHigh);
			
			if (fOk)
				*pdSizeInBytes = (double)fileInfo.nFileSizeLow;
			else
			{
				m_csError.Format ("database not found at %s",strFacilityDirectory);
				LogMsg (m_csError,NO_DB);
			}

		}
		else
		{
			// Facility not defined
			m_csError.Format ("FACILITY ID: %u",sFacilityID);
			LogMsg (m_csError,FACILITY_UNDEFINED);
			bSuccess = false;
		}

		if (TestPointer ((void*)pvbResult))
			*pvbResult = fOk?VARIANT_TRUE:VARIANT_FALSE;

	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_SIZE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_SIZE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_SIZE);
	}	

	return S_OK;
}

STDMETHODIMP CEOSSImport::GetDataStoreStatus(short sFacilityID, long* plStatus)
{
	*plStatus = 0;  //unknown
//	TRACE ("CEOSSImport::GetDataStoreStatus.\n");
	bool bSuccess = false;
	ClearError ();
	
	try
	{
		VARIANT_BOOL vbDefined = VARIANT_FALSE;
		BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
		CString facDir (bstrFacDir);

		if (!m_pDb->IsFacilityDBOpen (sFacilityID))
		{
			//A different facility was open. Close and reopen
			m_pDb->CloseDatabase ();
			bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
			VARIANT_BOOL vbResult = VARIANT_FALSE;
			SetParameters (sFacilityID,varParms,&vbResult);
		}
		//DB could not be opened
		if (!bSuccess)
		{
			*plStatus = 5;
		}
		if (g_pIFacMgr)
		{
			//Leave status unknown, as the facility is not known
			vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
		}
		if (vbDefined != VARIANT_FALSE && bSuccess)
		{
			VARIANT_BOOL vbResult = VARIANT_FALSE;
			double dSize = 0;
			//Check the size
			GetDataStoreSize (sFacilityID,&dSize,&vbResult);
			if (vbResult != VARIANT_FALSE)
			{
				if (dSize > MAX_RECOMMENDED_DB_SIZE)
					*plStatus = 4;
				else if (dSize > WITHIN_FIVE_PERCENT)
					*plStatus = 3;
				else
					*plStatus = 1;
			}
			else
			{
				*plStatus = 2;
			}
		}
		else
		{
			bSuccess = false;
			m_csError.Format ("FACILITY ID: %u",sFacilityID);
			LogMsg (m_csError,FACILITY_UNDEFINED);

		}
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_STATUS);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_STATUS);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_STATUS);
	}	
	return S_OK;
}

//	Get the time of the beginning and ending of the data store for the given station.
// 26-Oct-2004	SFK	Modified to support opening database
STDMETHODIMP CEOSSImport::GetDataStoreTimeRange(
	short sFacilityID, 
	long lStationID, 
	VARIANT_BOOL bErrDisplay, 
	DATE *pFirstTimestamp, 
	DATE *pLastTimestamp, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
//	TRACE ("CEOSSImport::GetDataStoreTimeRange.\n");
	VARIANT_BOOL vbDefined = VARIANT_FALSE;
	bool bSuccess = false;
	ClearError ();
	if (TestPointer ((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;
	try
	{
		BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
		CString facDir (bstrFacDir);

		if (!m_pDb->IsFacilityDBOpen (sFacilityID))
		{
			//A different facility was open. Close and reopen
			m_pDb->CloseDatabase ();
			bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
			VARIANT_BOOL vbResult = VARIANT_FALSE;
			SetParameters (sFacilityID,varParms,&vbResult);
		}

		if (g_pIFacMgr)
		{
			vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
		}
		if (vbDefined != VARIANT_FALSE)
		{

			BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
			CString facDir (bstrFacDir);
			if (!m_pDb->IsFacilityDBOpen (sFacilityID))
			{
				//A different facility was open. Close and reopen
				m_pDb->CloseDatabase ();
				bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
				VARIANT_BOOL vbResult = VARIANT_FALSE;
				SetParameters (sFacilityID,varParms,&vbResult);
			}
			//check station if !=-1
			if (lStationID != -1)
			{
				vbDefined = g_pIFacMgr->IsStationDefined (sFacilityID,lStationID,"EOSS");
			}

			if (vbDefined != VARIANT_FALSE)
			{
				CEOSSData BInst(m_pDb, (const int)lStationID,m_bQuietMode);	// sk 04/15/2003

				*pFirstTimestamp = 0;
				*pLastTimestamp = 0;
				*pvbResult = VARIANT_FALSE;

				bSuccess = BInst.GetEOSSDataStoreTimeRange (pFirstTimestamp,pLastTimestamp);

				if (*pFirstTimestamp == 0 && *pLastTimestamp == 0)
				{
					CString dbMessage;
					dbMessage.Format ("The database for facility %u is empty",sFacilityID);
					if (bErrDisplay !=VARIANT_FALSE)
						MessageBox (NULL,dbMessage,"DB Empty",MB_OK);	
					bSuccess = true;
				}
				
			}
			else
			{
				bSuccess = false;
				m_csError.Format ("FACILITY ID: %u STATION ID: %u",sFacilityID,lStationID);
				LogMsg (m_csError,STATION_UNDEFINED);
			}

		}
		else
		{
			bSuccess = false;
			m_csError.Format ("FACILITY ID: %u",sFacilityID);
			LogMsg (m_csError,FACILITY_UNDEFINED);
		}
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_TIMERANGE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_TIMERANGE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_TIMERANGE);
	}
	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}


//	Gets day statistics for the specified time range.
// 26-Oct-2004	SFK	Modified to support opening database
STDMETHODIMP CEOSSImport::GetDayStatistics(
	short sFacilityID, 
	long lStationID, 
	DATE StartTime, 
	DATE EndTime, 
	DATE *pActualStartTime, 
	DATE *pActualEndTime, 
	long *plNumMembers, 
	VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
//	TRACE ("CEossImport:: GetDayStatistics\n");
	ClearError ();
	
	bool bSuccess = false;
	VARIANT_BOOL vbDefined = VARIANT_FALSE;
	if (TestPointer((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;
	try
	{
		*pActualStartTime = 0.0;
		*pActualEndTime = 0.0;
		*plNumMembers = 0;
		if (g_pIFacMgr)
		{
			vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
		}
		if (vbDefined != VARIANT_FALSE)
		{
			if (!m_pDb->IsFacilityDBOpen (sFacilityID))
			{
				//A different facility was open. Close and reopen
				m_pDb->CloseDatabase ();
				BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
				CString facDir (bstrFacDir);
				bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
				VARIANT_BOOL vbResult = VARIANT_FALSE;
				SetParameters (sFacilityID,varParms,&vbResult);
			}
			//Check for valid station
			if (lStationID == -1)
			{
				//All stations requested
				CEOSSData BInst(m_pDb, (int)lStationID,m_bQuietMode);	// sk 04/15/2003
				bSuccess = BInst.GetEOSSDayStatistics (lStationID,StartTime, EndTime, pActualStartTime, pActualEndTime, plNumMembers);

			}
			else
			{
				if (g_pIFacMgr)
				{
					vbDefined = g_pIFacMgr->IsStationDefined (sFacilityID, lStationID,"EOSS");
				}
				if (vbDefined != VARIANT_FALSE)
				{
					CEOSSData BInst(m_pDb, (int)lStationID,m_bQuietMode);	// sk 04/15/2003
					bSuccess = BInst.GetEOSSDayStatistics (lStationID,StartTime, EndTime, pActualStartTime, pActualEndTime, plNumMembers);
				}
				else
				{
					m_csError.Format ("FACILITY ID: %u STATION ID: %u",sFacilityID, lStationID);
					LogMsg (m_csError,STATION_UNDEFINED);
					bSuccess = false;
				}
			}

		}
		else
		{
			m_csError.Format ("FACILITY ID: %u",sFacilityID);
			LogMsg (m_csError,FACILITY_UNDEFINED);
			bSuccess = false;
		}
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_TIMERANGE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_TIMERANGE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_TIMERANGE);
	}
	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;
	// 22-Dec-2005	Close db after each action to allow 2 programs to use the COM*/
	return S_OK;
}




//	Gets day data for the specified time range.  SAFEARRAY of results is allocated here.
// 26-Oct-2004	Changed function name from GetDataByDay and eliminate plNumMembers argument since can get from SAFEARRAY info
STDMETHODIMP CEOSSImport::GetDaySummaryData(
											short sFacilityID,
											long lStationID,
											DATE StartTime,
											DATE EndTime,
											VARIANT* pvDay,
											VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
//	TRACE ("CEOSSImport::GetDaySummaryData.\n");
	ClearError ();
	bool bSuccess = false;
	VARIANT_BOOL vbDefined = VARIANT_FALSE;
	if (TestPointer ((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;
	try
	{
		if (g_pIFacMgr)
		{
			vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
		}
		if (vbDefined != VARIANT_FALSE)
		{
			if (!m_pDb->IsFacilityDBOpen (sFacilityID))
			{
				BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
				CString facDir (bstrFacDir);
				//A different facility was open. Close and reopen
				m_pDb->CloseDatabase ();
				bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
				VARIANT_BOOL vbResult = VARIANT_FALSE;
				SetParameters (sFacilityID,varParms,&vbResult);
			}

			if (g_pIFacMgr)
			{
				vbDefined = g_pIFacMgr->IsStationDefined (sFacilityID,lStationID,"EOSS");
			}
			if (vbDefined != VARIANT_FALSE)
			{
				CEOSSData BInst(m_pDb, (int)lStationID,m_bQuietMode);	// sk 04/15/2003
				bool bSuccess = BInst.GetEOSSDaySummaryData (lStationID,StartTime,EndTime,pvDay);
			}
			else
			{
				bSuccess = false;
				m_csError.Format ("STATION ID: %u",lStationID);
				LogMsg (m_csError,STATION_UNDEFINED);
			}
		}
		else
		{
			m_csError.Format ("FACILITY ID: %u STATION ID: %u",sFacilityID, lStationID);
			LogMsg (m_csError,STATION_UNDEFINED);
			bSuccess = false;
		}


	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_TIMERANGE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_TIMERANGE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_GET_DATASTORE_TIMERANGE);
	}
	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;
	

	return S_OK;
}




// Get list of all files this COM knows about in the specified directory(ies)
// 26-Oct-2004	SFK	Added interface but did not implement
// 11-Jul-2005 SFK	Copied from Heather's GRAND Import stuff and modified for binary
STDMETHODIMP CEOSSImport::GetFilesList(
									   short sFacilityID, 
									   BSTR bstrRootDir, 
									   VARIANT_BOOL bIncludeSubDirs, 
									   VARIANT_BOOL bMostRecent, 
									   VARIANT *psaFilesList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	ClearError ();
	bool bSuccess = false;
	VARIANT_BOOL vbDefined = VARIANT_FALSE;
//	TRACE ("CEOSSImport::GetFilesList.\n");
	try
	{
		CString Msg;

		SAFEARRAY *pSA;
		SAFEARRAYBOUND bounds[2];
		long lDimension[2];
		HRESULT hr;
		POSITION Pos;
		tFileRecord FileRecord;
		CList<tFileRecord, tFileRecord> FileList;
		VARIANT_BOOL bResult = VARIANT_TRUE;

		if (!m_pDb->IsFacilityDBOpen (sFacilityID))
		{
			//A different facility was open. Close and reopen
			m_pDb->CloseDatabase ();
			BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
			CString facDir (bstrFacDir);
			bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
			VARIANT_BOOL vbResult = VARIANT_FALSE;
			SetParameters (sFacilityID,varParms,&vbResult);
		}

		if (g_pIFacMgr)
		{
			vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
		}
		if (vbDefined != VARIANT_FALSE)
		{
			CEOSSDataFile EOSSFile (g_pIFacMgr,m_bQuietMode);
			//The EOSSDataFile class encapsulates the search function for us.
			USES_CONVERSION;
			CString rootDir = W2T(bstrRootDir);
			EOSSFile.GetCompleteFileList(sFacilityID, &FileList, rootDir, bIncludeSubDirs);
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
							GetDataStoreTimeRange(sFacilityID, FileRecord.StationID.lVal, false, &StartTime, 
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
						GetDataStoreTimeRange(sFacilityID, FileRecord.StationID.lVal, false, &StartTime, 
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
						if(hr != S_OK&&!m_bQuietMode)
						{
							Msg.Format("Failed to load DATE array element [%d][0]", lDimension[0]);
							MessageBox(NULL, Msg, "Error PutElement", MB_OK);
							bSuccess = false;
						}
						
						//******************************
						//Load the STATION ID
						//******************************
						lDimension[1] = 1;	//COLUMN
						hr = SafeArrayPutElement(pSA, lDimension, &FileRecord.StationID); 
						if(hr != S_OK&&!m_bQuietMode)
						{
							Msg.Format("Failed to load STATION ID array element [%d][1]", lDimension[0]);
							MessageBox(NULL, Msg, "Error PutElement", MB_OK);
							bSuccess = false;
						}
						
						//*****************************
						//Load the FILE NAME
						//*****************************
						lDimension[1] = 2;	//COLUMN
						hr = SafeArrayPutElement(pSA, lDimension, &FileRecord.File); 
						if(hr != S_OK&&!m_bQuietMode)
						{
							Msg.Format("Failed to load FILE NAME array element [%d][2]", lDimension[0]);
							MessageBox(NULL, Msg, "Error PutElement", MB_OK);
							bSuccess =false;
						}
						lDimension[0]++;  //Row
					}
		//			SysFreeString(FileRecord.File.bstrVal);
				}//end WHILE Pos != NULL
				
				
				//Release the locks on the data in the array.
				SafeArrayUnaccessData(pSA);
				
				//Set the return value
				psaFilesList->parray = pSA;
				bSuccess = true;
			}
			else //No files found.
				psaFilesList->vt = VT_EMPTY;
		//} //end IF if bResult = true (Facility exists)
		}
		else
		{
			bSuccess = false;
			m_csError.Format ("FACILITY ID: %u",sFacilityID);
			LogMsg (m_csError,FACILITY_UNDEFINED);
		}
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_GET_FILES_LIST);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_GET_FILES_LIST);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_GET_FILES_LIST);
	}
	return S_OK;
}




// 26-Oct-2004	SFK	Added interface but did not implement
STDMETHODIMP CEOSSImport::GetParameters(short sFacilityID, VARIANT* pvParms, VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

//	TRACE ("CEOSSImport::GetParameters.\n");
	ClearError ();
	bool bResult = false;
	VARIANT longvar;
	::VariantInit(&longvar);

	try
	{
		// Return current parameters.
		if (sFacilityID != m_sFacID)
		{
			// They want parameters for a facility that has not been set.
			bResult = false;
			m_csout.Format ("%u",sFacilityID);
			LogMsg (m_csout,PARMS_NOT_SET);
		}
		else if (m_sFacID == -1)
		{
			bResult = false;
			// SetParameters has not been called.
			m_csout.Format ("for facility %u", sFacilityID);
			LogMsg (m_csout,SET_PARMS_NOT_CALLED);
		}
		else
		{
			// Return the logging parms, etc in GetParameters
			SAFEARRAYBOUND bound;
			bound.cElements = 1;
			bound.lLbound = 0;
			pvParms->parray = SafeArrayCreate (VT_VARIANT,1,&bound);
			pvParms->vt = VT_VARIANT|VT_ARRAY;
			longvar.vt = VT_I4;
			long lFlags = 0;
			if (m_bDisableLogger)
				lFlags = DISABLE_LOGGER;
			if (m_bQuietMode)
				lFlags |= MASK_QUIET_MODE;
			
			lFlags |= m_sVerbosityLevel;
			longvar.lVal = lFlags;
			LONG lDim [1];
			lDim[0] = 0;
			SafeArrayPutElement (pvParms->parray,lDim,&longvar);
		}
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_GET_PARAMETERS);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_GET_PARAMETERS);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_GET_PARAMETERS);
	}	

	if (TestPointer(pvbResult))
		*pvbResult = bResult?VARIANT_TRUE:VARIANT_FALSE;
	::VariantClear (&longvar);
	return S_OK;
}



//	InitializeDataStore - Common COM function to do any special action to initialize a data store
//	26-Oct-2004	SFK	Added database open, if necessary.  Also changed calling argument to have FacilityID rather the db path
STDMETHODIMP CEOSSImport::InitializeDataStore(short sFacilityID,
											  VARIANT_BOOL bQuestion,
											  VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	ClearError ();
	bool bSuccess = false;
	VARIANT_BOOL vbDefined = VARIANT_FALSE;
//	TRACE ("CEOSSImport::InitializeDataStore.\n");
	try
	{
		if (g_pIFacMgr)
		{
			vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
		}
		if (vbDefined != VARIANT_FALSE)
		{
			// If the database existed, it was opened, otherwise it was created.
			// Delete any data in it if it existed before.
			if (!m_pDb->IsFacilityDBOpen (sFacilityID))
			{
				//A different facility was open. Close and reopen
				m_pDb->CloseDatabase ();
			}

			BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
			CString facDir (bstrFacDir);
			bSuccess = DeleteDataStore (sFacilityID,&vbDefined);
			bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
			VARIANT_BOOL vbResult = VARIANT_FALSE;
			SetParameters (sFacilityID,varParms,&vbResult);
			bSuccess = true;
		}
		else
		{
			m_csError.Format ("FACILITY ID: %u",sFacilityID);
			LogMsg (m_csError,FACILITY_UNDEFINED);
			bSuccess = false;
		}
	}

	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_INIT_DATASTORE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_INIT_DATASTORE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_INIT_DATASTORE);
	}	

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}



// OpenDataStore - Common COM function to do any special action to open a data store
//	26-Oct-2004	SFK	Changed calling argument from path to FacilityID
STDMETHODIMP CEOSSImport::OpenDataStore(short sFacilityID, VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
//	TRACE ("CEOSSImport::OpenDataStore.\n");
	ClearError ();
	if (TestPointer((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;
	// Open the database.  If it doesn't exist, create it.
	try
	{
		if (m_pDb->mbDatabaseOpen) m_pDb->CloseDatabase ();
		CString csTemp ((BSTR)g_pIFacMgr->GetFacilityDirectory (sFacilityID));
		CString csDataStorePath = csTemp + "\\data\\Import\\EOSS";
		VARIANT_BOOL vbResult = VARIANT_FALSE;
		if (!m_pDb->DatabaseExists (csDataStorePath, EOSS_DB_NAME))
		{
			CreateNewDataStore (sFacilityID,&vbResult);
		}
		BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
		CString facDir (bstrFacDir);
		bool bResult= m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
		*pvbResult = bResult?VARIANT_TRUE:VARIANT_FALSE;
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_OPEN_DATASTORE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_OPEN_DATASTORE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_OPEN_DATASTORE);
	}	
		
	return S_OK;
}



//	RestoreDataStore - Common COM function to restore any COM specific files from a designated location
// 26-Oct-2004	SFK	Changed to have sFacilityID instead of bstrDestPath in arguments, opened db if necessary
STDMETHODIMP CEOSSImport::RestoreDataStore(short sFacilityID,
										   BSTR bstrSourcePath,
										   VARIANT_BOOL bQuestion,
										   VARIANT_BOOL bDefault,
										   VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

//	TRACE ("CEOSSImport::RestoreDataStore()\n");
	CString strDestPath;
	CString sourceDB (bstrSourcePath);
	ClearError ();
	
	bool bSuccess = false;
	if (TestPointer((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;
	try
	{
		//Invalid facility
		if (g_pIFacMgr)
		{
			VARIANT_BOOL vbDefined = g_pIFacMgr->IsFacilityDefined (sFacilityID);
			if (vbDefined == VARIANT_FALSE)
			{
				bSuccess = false;
			}
			else
				bSuccess = true;
		}
	
		if (bSuccess)
		{
			// Make sure that \\ are there and there is only one.
			
			CString strDBName;
			if (g_pIFacMgr)
			{
				// retrieve the destination location of the db requested.
				BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory(sFacilityID);
				CString strFacilityDirectory (bstrFacDir);
				strFacilityDirectory.TrimRight ("\\");
				strFacilityDirectory+= "\\Data\\Import\\EOSS\\";
				strDestPath = strFacilityDirectory;
				strDBName = strFacilityDirectory + EOSS_DB_NAME;
			}		
			
			CDirUtilities dir (m_bQuietMode);
			strDestPath = dir.AddDriveLetter (strDestPath);
			int answer = IDYES;
			if(dir.FileExists(strDBName))
			{
				// Close it if it is open so that we don't get any sharing violations
				bool bOpen = m_pDb->IsFacilityDBOpen (sFacilityID);

				if (bOpen)
					m_pDb->CloseDatabase ();

				//Now we must ask about overwriting if not quiet mode.
				if (!m_bQuietMode)
				{
					if (bQuestion != VARIANT_FALSE)
					{
						// we are supposed to ask, the result depends on their answer
						CString message;
						message.Format ("A database already exists at %s. Do you want to overwrite it?",strDBName);
						answer = MessageBox (NULL,message,"DB already exists",MB_YESNO);
					}
					else if (bQuestion == VARIANT_FALSE && bDefault == VARIANT_FALSE)
					{
						bSuccess = false;//Not ok to overwrite, exists
						LogMsg("database already existed at location.  No copy performed",FILE_COPY_ERR);
					}
					else if (bQuestion == VARIANT_FALSE && bDefault == VARIANT_TRUE)
						bSuccess = true;//OK to overwrite without asking

					if (answer == IDYES && bSuccess)
					{
						bSuccess = true;
					}
					else if (answer == IDNO)
					{
						bSuccess = false;
						LogMsg("user chose not to overwrite existing db.  No copy performed",FILE_COPY_ERR);
					}
				}
				else
				{
					// Quiet mode, just use the bDefault to decide.
					bSuccess = bDefault != VARIANT_FALSE?true:false;
				}

				if (bSuccess)
				{
					if (MakeSureDirectoryPathExists (strDestPath)==TRUE)
					{
						
						if (dir.FileExists (sourceDB+"\\Data\\Import\\EOSS\\"+EOSS_DB_NAME))
							bSuccess = CopyFile (sourceDB+"\\Data\\Import\\EOSS\\"+EOSS_DB_NAME,strDestPath+"\\"+EOSS_DB_NAME,FALSE) !=0;
						else
							bSuccess = false;
					}
					else
						bSuccess = false;
				}
			}
			else
			{
				if (MakeSureDirectoryPathExists (strDestPath)==TRUE)
				{
					bSuccess = CopyFile (sourceDB+"\\Data\\Import\\EOSS\\"+EOSS_DB_NAME,strDestPath+"\\"+EOSS_DB_NAME,FALSE) !=0;
				}
				//DWORD dw = ::GetLastError  ();
				LPVOID lpMsgBuf;

				::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					 NULL,
					 ::GetLastError (),
					 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					 (LPTSTR) &lpMsgBuf,
					 0,
					 NULL );
			}
		}
		
		if (TestPointer((void*)pvbResult))
			*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_RESTORE_DATASTORE);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_RESTORE_DATASTORE);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_RESTORE_DATASTORE);
	}	
	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}



STDMETHODIMP CEOSSImport::SetParameters(short sFacilityID, 
										VARIANT vData,
										VARIANT_BOOL *pvbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
//	TRACE ("CEOSSImport::SetParameters.\n");
	VARIANT var;
	ClearError ();
	::VariantInit(&var);
	bool bSuccess = false;
	if (TestPointer((void*)pvbResult))
		*pvbResult = VARIANT_FALSE;

	try
	{

		HRESULT hr1,hr2,hr3;

		CString temp ((BSTR)g_pIFacMgr->GetFacilityLongName(sFacilityID));
		m_strFacilityName = temp;
		m_strFacilityName.MakeUpper();

		if (vData.vt == (VT_VARIANT | VT_ARRAY)) 	// is it the type of SafeArray we are expecting?
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

						g_pIFacMgr->SetFacilityMgrErrorLoggingParameters(lfourbytes);	//16 July 2007 pjm

						m_sVerbosityLevel = (short)(lfourbytes & 0x0000ffff);  //1 - 6

						short high_16 = (short)(lfourbytes >> 16);

						m_bQuietMode = (0 != (high_16 & 0x0001)); //quiet mode bit is bit 0
						m_bDisableLogger = (0 != (high_16 & 0x0002)); //disable logger setting is bit 1
						
						// Add get datastore path when parameters set (and facility ID is known)
						CString csTemp ((BSTR)g_pIFacMgr->GetFacilityDirectory (sFacilityID));
						m_csDataStorePath = csTemp + "\\data\\Import\\EOSS";

						m_pDb->SetQuietMode(m_bQuietMode);
						
						// We only need to open the Access DB once.
						if (!m_pDb->mbDatabaseOpen)
						{
							CString err;
							//Ask if the database exists
							bool bExists = m_pDb->DatabaseExists (m_csDataStorePath,EOSS_DB_NAME);
							VARIANT_BOOL vbResult = VARIANT_FALSE;

							if (bExists)
							{
								BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
								CString facDir (bstrFacDir);
								m_bDatabaseOpen = m_pDb->OpenTheDatabase (sFacilityID,&err,facDir);
							}
							else
							{
								BSTR bstrFacDir = g_pIFacMgr->GetFacilityDirectory (sFacilityID);
								CString facDir (bstrFacDir);
								if (m_pDb->mbDatabaseOpen) m_pDb->CloseDatabase ();
								CreateNewDataStore (sFacilityID,&vbResult);
								bSuccess = m_pDb->OpenTheDatabase (sFacilityID,NULL,facDir);
							}
						}


						// Set Parameters for the logger, call startup
						if (!m_bDisableLogger)
							m_pILogger->Startup (MY_GUID,m_sStartUpTimeMillisec,m_sVerbosityLevel);

						// This could get called multiple times, only do startup once.
						if (!m_bStartupCalled)
						{
							m_csout.Format ("at %s",m_StartProcess.Format ("%m/%d/%Y %H:%M:%S"));
							LogMsg (m_csout,STARTUP);
							m_bStartupCalled = true;
						}
						// Save Facility ID
						m_sFacID = sFacilityID;
						bSuccess = true;

					}
				}
			}
		}

		// Now, build channel lookup table for the facility.
		if (g_pIFacMgr)
		{
			VARIANT vStations;
			VariantInit(&vStations);
			g_pIFacMgr->GetStationIDsWithInstrumentType (sFacilityID,"EOSS",VARIANT_TRUE,&vStations);
			if (vStations.parray != NULL /*&& vStations.vt == VT_VARIANT|VT_ARRAY*/)
			{
				long lLower, lUpper = 0;
				SafeArrayGetLBound (vStations.parray,1,&lLower);
				SafeArrayGetUBound (vStations.parray,1,&lUpper);
				long lNumItems = lUpper - lLower + 1;
				m_lNumDefinedStations = lNumItems;
				if (lNumItems >0)
				{
					//delete old lookup table if it exists;
					if (m_pStationLookupTable)
					{
						delete [] m_pStationLookupTable;
						m_pStationLookupTable = NULL;
					}
					if (m_pChannelLookupTable)
					{
						delete [] m_pChannelLookupTable;
						m_pChannelLookupTable = NULL;
					}
					m_pStationLookupTable = new long [lNumItems];
					m_pChannelLookupTable = new short [lNumItems*MAX_SEALS];
					m_lNumDefinedChannels = lNumItems * MAX_SEALS;
					//For each station, enter the channel indexes
					for (int i = 0; i < lNumItems; i++)
					{
						long lStationID = 0;
						//save station ids in the first indexed array
						SA_GetElem (vStations.parray,i,0,&lStationID);
						m_pStationLookupTable [i] = lStationID;
						
						VARIANT vChannels;
						VariantInit (&vChannels);
						//Get channel short names to derive the Seal IDs
						g_pIFacMgr->GetChannelIDs (sFacilityID, lStationID,"EOSS",VARIANT_TRUE,&vChannels);
						
						if (vChannels.parray /*&& vChannels.vt == VT_VARIANT|VT_ARRAY*/)
						{
							SafeArrayGetLBound (vChannels.parray,1,&lLower);
							SafeArrayGetUBound (vChannels.parray,1,&lUpper);
							long lDefinedChannels = lUpper-lLower + 1;

							//For each channel
							for (int j = 0; j < lDefinedChannels; j ++)
							{
								short sChan = -1;
								SA_GetElem (vChannels.parray,j,0,&sChan);
								m_pChannelLookupTable [i*MAX_SEALS + j] = sChan;
							}
							for (int k = lDefinedChannels; k < MAX_SEALS; k++)
							{
								m_pChannelLookupTable [i*MAX_SEALS + k] = -1;
							}
						}
					}
				}
			}
		}
		::VariantClear (&var);
		if (TestPointer((void*)pvbResult))
			*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;
	
	}
	catch (CMemoryException *e)
	{
		// Don't try to allocate an error message when having memory issues.
		e->Delete ();
	}
	catch (CFileException* cfe)
	{
		cfe->GetErrorMessage (m_csError.GetBuffer (MAX_ERROR),MAX_ERROR);
		m_csError.ReleaseBuffer ();
		m_csError = "File Exception" + m_csError;
		// Redundant with new error code
		LogMsg (m_csError,EXCEPTION_SET_PARAMETERS);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		LogMsg (m_csError,EXCEPTION_SET_PARAMETERS);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		LogMsg (m_csError,EXCEPTION_SET_PARAMETERS);
	}	

	if (TestPointer(pvbResult))
		*pvbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	::VariantClear(&var);
	return S_OK;
}


STDMETHODIMP CEOSSImport::GetLastError(short /*sFacilityID*/, BSTR *pbstrErrortext, long* plError)
{
//	TRACE("CEOSSImport::GetLastError()\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	try
	{
		//Nice place for a test point...
		//Beep(200,200);
		//VARIANT_BOOL vb;
		//DeleteDataStore(sFacilityID,&vb);

		*plError = m_lErrID;
		*pbstrErrortext = m_csErrorString.AllocSysString ();
	}
	catch(...)
	{
	}
	return S_OK;
}

void CEOSSImport::InitLogger()
{
	HRESULT hr;
	CString err;
	
	// HN - 1/9/2007 Check success of COM call using macro instead.
	try
	{
		hr = m_pILogger.CreateInstance (CLSID_IRSLogger);

		if (FAILED(hr))
		{
			LPVOID lpMsgBuf;

			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					 NULL,
					 ::GetLastError (),
					 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					 (LPTSTR) &lpMsgBuf,
					 0,
					 NULL );

			err.Format ("Exception creating the logger 0x%x %s\r\n",::GetLastError (), (LPCTSTR)lpMsgBuf);
			LocalFree (lpMsgBuf); // We can't log here anyway
			LogMsg (err,COM_EXCEPTION);
		}

	}
	catch (_com_error &e)
	{
		err.Format ("COM Exception during CreateInstance (CLSID_IRSLogger) call HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		// This is ok, as long as we just set the error and don't try to talk to the logger that just failed.
		LogMsg (err,EXCEPTION_CAUGHT);
		m_pILogger = NULL;
	}
}

void CEOSSImport::SetError (CString err, long lErrID)
{
	m_csErrorString = FormatMessage (err,lErrID);
	m_lErrID = lErrID | m_lErrSev;
}

void CEOSSImport::ClearError(void)
{
	m_csErrorString = "";
	m_lErrID = 0;
}

CString CEOSSImport::FormatMessage(CString arg, long lMsgID)
{
	CString sectionName, keyName, message;
	sectionName.Format ("%u",lMsgID);
	keyName = "Msg";
	char* pszMessage = new char [MAX_ERROR];
	DWORD dw = GetPrivateProfileString ((LPCTSTR)sectionName,(LPCTSTR)keyName,"Could not format message",pszMessage,MAX_ERROR,m_csErrorINIFile);
	message = pszMessage;
	if (message == "Could not format message")
		message.Format ("Could not format message, argument: %s",arg);
	else
		message.Format (pszMessage,arg);
	keyName = "Sev";
	long lLoggerSeverity = GetPrivateProfileInt ((LPCTSTR)sectionName,(LPCTSTR)keyName,WARNING_ERROR,m_csErrorINIFile);

	if (lLoggerSeverity == FATAL || lLoggerSeverity == AUTHENTICATION)
		m_lErrSev = FATAL_ERROR;
	else if (lLoggerSeverity == ERROR)
		m_lErrSev = NON_FATAL_ERROR;
	else if (lLoggerSeverity == WARNING)
		m_lErrSev = WARNING_ERROR;
	else
		m_lErrSev = NON_ERROR;

	delete []pszMessage;
	pszMessage = NULL;
	return message;
}

void CEOSSImport::LogMsg(const CString &msgArgs, long lMessageID)
{
	// No need to do all this if the logger is shot.
	if (!m_bDisableLogger && m_pILogger != NULL)
	{
		COleDateTime now;
		now = COleDateTime::GetCurrentTime ();
		
		VARIANT vMessageArgs;
		::VariantInit (&vMessageArgs);
		vMessageArgs.vt = VT_VARIANT | VT_ARRAY;
		
		SAFEARRAYBOUND rgsabound;
		rgsabound.cElements = 1;
		rgsabound.lLbound = 0;

		vMessageArgs.parray = SafeArrayCreate (VT_VARIANT,1,&rgsabound);

		//Plug leak 8-4-07
		SA_PutElem (vMessageArgs.parray,0,msgArgs);
		BSTR msgFile;
		msgFile = m_csErrorINIFile.AllocSysString ();
		m_pILogger->LogMessage (now.m_dt,m_sFacID,lMessageID,m_sVerbosityLevel,msgFile,vMessageArgs);
		::SysFreeString(msgFile);

		//Should not do this anyway, it is not the consumer.
		//if (TestPointer ((void*)vMessageArgs.parray) && vMessageArgs.parray != NULL)
		//{
			// SafeArrayDestroy (vMessageArgs.parray);
			// This can blow up, don't do it.
		//}
		::VariantClear (&vMessageArgs);
	}
	
	SetError (msgArgs,lMessageID);
}
