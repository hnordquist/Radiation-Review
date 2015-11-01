// GrandDbVista.cpp: implementation of the CGrandDbVista class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2002, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support generic directory operations
//
//	Functions:
//		ClearDatabase
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GrandDbVista.h"
#include "GrandImportSupport.h"

#define Currtask m_Currtask

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrandDbVista::CGrandDbVista(bool bQuietMode) 
	:CDbVista(bQuietMode)
{
	FinishConstruction();
}


CGrandDbVista::CGrandDbVista()
	:CDbVista(true)  //default to silence: quiet mode ON
{
	FinishConstruction();
}

void CGrandDbVista::FinishConstruction()
{
	mstrComponentName = "Data\\Import\\MG";	// sfk 08/13/2004 Changed to be in /Import directory (other passed in)
	mstrDatabaseName = "db_rmon";
	mstrMyDbFilename = "db_rmon.dbd";
	m_pFacCfg = NULL;
	//m_bQuietMode = bQuietMode;  //is done in base class
}

CGrandDbVista::~CGrandDbVista()
{
}

void CGrandDbVista::SetFacMgr(CGrandFacCfgWrap *pFacCfg)
{
	m_pFacCfg = pFacCfg;
}

void CGrandDbVista::SetQuietMode(bool bQuietMode)
{
	m_bQuietMode = bQuietMode;
}


bool CGrandDbVista::BuildDatabaseFramework(short sFacNum)
{
	db_sta_rec dbSta;
	bool bResult = false;

	// Get number of stations in facility
	short sNum = m_pFacCfg->GetNumStations(msFacNum);
	// Get the list of IDs in this station
	long *pID = new long[sNum];		// 28-Sep-2005 SFK	Changed to longs

	if (pID != NULL)
	{
		bResult = m_pFacCfg->GetStationIDs(sFacNum, &sNum, pID);	// get the ids of all the GRAND stations

		if (bResult) 
		{
			// read the station number for each ID in the list and put in the dbVista framework
			for (short i = 0; i < sNum; i++) 
			{
				m_pFacCfg->GetStationRecord(msFacNum, pID[i], &dbSta);				
				// fill in unused fields
				dbSta.s_data_type = 0;
				dbSta.s_sta_picklist_num = 0;
				strcpy(dbSta.sz_sta_short_name, "");

				// connect to dbVista db
				d_fillnew (DB_STA_REC, (char *) &dbSta, CURR_DB);
				d_connect (STATION_SET, CURR_DB);
			}
		}
		delete [] pID;
	}
	return bResult;
}

// 02-Mar-2005 SFK Add function to handle case of a legitimate station that doesn't
// have the framework in the db.
bool CGrandDbVista::AddStationToDatabaseFramework(short sFacNum, short sStaID)
{
	db_sta_rec dbSta;
	int db_stat;

	// check if the station number is already in the database
	db_stat = d_keyfind(S_STA_NUM_KEY,(char *)&sStaID, CURR_DB);
	if (db_stat == S_OKAY) return(true);	// station number already in there.

	// Put in the dbVista framework
	m_pFacCfg->GetStationRecord(msFacNum, sStaID, &dbSta);				
	// fill in unused fields
	dbSta.s_data_type = 0;
	dbSta.s_sta_picklist_num = 0;
	strcpy(dbSta.sz_sta_short_name, "");

	// connect to dbVista db
	db_stat = d_fillnew (DB_STA_REC, (char *) &dbSta, CURR_DB);
	if (db_stat == S_OKAY) d_connect (STATION_SET, CURR_DB);
	//if (db_stat == S_OKAY) return(true);
	//else return (false);
	return (db_stat == S_OKAY); 
}

//  hn 6/8/2005	Had to modify OpenTheDatabase to send an error if creation of
//				nonexistent data store fails. 
bool CGrandDbVista::OpenTheDatabase(short sFacNum,CString* pstrErrorMsg)
{
	bool bStatus = false;
	CString strErrorMsg("");

	CString strFacilityDirectory = m_pFacCfg->GetFacilityDirectory(sFacNum);

	if (strFacilityDirectory.IsEmpty())  //15-Feb-2005 PJM Do not allow empty directory
	{
		strErrorMsg.Format("Get facility directory error.  Facility number = %d. Facility directory = %s", 
			sFacNum,strFacilityDirectory);

		//Was "NOT FOR NDAR". REINSERTED 11/26/2007 PJM
		if (!m_bQuietMode)
			MessageBox(NULL, strErrorMsg, "Facility Directory Error", MB_OK|MB_ICONWARNING);

		if (pstrErrorMsg)
			pstrErrorMsg->Format("%s", strErrorMsg);
	}
	else
	{
		mstrDatabaseRootPath = strFacilityDirectory;
		strFacilityDirectory = m_Dir.AppendPath(strFacilityDirectory, mstrComponentName);
		
		//If the datastore doesn't exist, create it and try again. hn 6/8/2005
		if (!m_Dir.FileExists (strFacilityDirectory, mstrDatabaseName + ".dbd")) 
		{
			//Create the data store if it doesn't exist.
			if (!CreateDataStore (sFacNum))
			{
				//We still didn't create the datastore.
				bStatus = false;
				strErrorMsg.Format("Database could not be created at %s.", strFacilityDirectory);

				//Was "NOT FOR NDAR". REINSERTED 11/26/2007 PJM
				if (!m_bQuietMode)
					MessageBox(NULL, strErrorMsg, "Database Creation Error", MB_OK|MB_ICONWARNING);

				if (pstrErrorMsg)
					pstrErrorMsg->Format("%s", strErrorMsg);
			}
			else
			{
				bStatus = true;
			}
		}

		//The newly created db will be opened if we successfully created it.
		CString cs;
		bStatus = OpenDatabase(strFacilityDirectory, mstrDatabaseName,&cs);
		
		// 12-Jan-2005  Check that we really can access this database.  d_open does not return an error if another
		// program is using this database as I would expect.
		if (bStatus) {
			int db_stat = d_findfm(STATION_SET, CURR_DB);
			if (db_stat == S_EACCESS) {	// someone else is using the database, so close the db now
				CloseDatabase();
			}
		}
	}
	return(bStatus);
}
// Jun-2005 hn  Added this function to create a new datastore by copying the default datastore
bool CGrandDbVista::CreateDataStore(short sFacilityID)
{
	CGrandImportSupport ImportSupport(m_pFacCfg,m_bQuietMode);

	//Determine the empty data store and destination path for the facility's data store
	//location, then copy the contents of the empty db to the place it should be.
	// The result is whether or not this copy was successful. hn 6/8
	CString emptyDBPath, dataStorePath;
	emptyDBPath = ImportSupport.GetEmptyDBLocation();
	dataStorePath = ImportSupport.GetGrandDataStorePath (sFacilityID);
	
	//Copy the empty db, return the success of the copy operation.
	return (m_Dir.CopyDirectory (emptyDBPath,dataStorePath,TRUE));
}
