// VifmDbVista.cpp: implementation of the CVifmDbVista class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2002, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support generic directory operations
//
//	Functions:
//		ClearDatabase
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VifmDbVista.h"
#include "FacCfgWrap.h"
#include "VifmImportSupport.h"

#define Currtask m_Currtask

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVifmDbVista::CVifmDbVista() 
	: CDbVista(false)  //Quiet mode always off for vifm
{
	mstrComponentName = "Data\\Import\\Vifm";		// sfk 08/13/2004 Changed to be in Data/Import directory
	mstrDatabaseName = "db_vifm";
	mstrMyDbFilename = "db_vifm.dbd";
}

CVifmDbVista::CVifmDbVista(bool notused_quietmode) 
	: CDbVista(false)   //Quiet mode always off for vifm
{
	mstrComponentName = "Data\\Import\\Vifm";		// sfk 08/13/2004 Changed to be in Data/Import directory
	mstrDatabaseName = "db_vifm";
	mstrMyDbFilename = "db_vifm.dbd";
}

CVifmDbVista::~CVifmDbVista()
{

}


///////////////////////////////////////////////////////////////////////////
//	Name:	ClearDatabase
//
//	Description:
//	After the generic part of the database initialize is done, reconstruct
//	the framework needed for the database by getting the appropriate information
//	from the Facility Configurator.
//	
//	Declaration:
//	bool CDbVista::ClearDatabase(const CString &strDbName, bool bPrompt, CString *pstrErrorMsg)
//
//	Input:	strDbName	name used when opened this database
//			bPrompt		whether to display prompt asking user to clear the db
//
//	Output:	pstrErrorMsg	string containing error message, if any
//
//	Return:	true (db cleared) / false (db not open or error from db internal -- see pstrErrorMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	03-Dec-2001	SFK		Created from InitializeDatabase in DbGeneralDbSupport.cpp
//	02-Apr-2002	SFK		Added getting station numbers from FacilityCom
//////////////////////////////////////////////////////////////////
// Use the ClearDatabase in the base class CVistaDb
//bool CVifmDbVista::ClearDatabase(const CString &strDbName, bool bPrompt, CString *pstrErrorMsg)
//{
//	return CVistaDb::ClearDatabase(strDbName, bPrompt, pstrErrorMsg);
//}


// 02-Mar-2005 SFK Add function to handle case of a legitimate station that doesn't
// have the framework in the db.
bool CVifmDbVista::AddStationToDatabaseFramework(short sFacNum, short sStaID)
{
	db_sta_rec dbSta;
	int db_stat;

	CFacCfgWrap FacCfg;		// 29-Sep-2005 SFK	Removed static

	// check if the station number is already in the database
	db_stat = d_keyfind(S_STA_NUM_KEY,(char *)&sStaID, CURR_DB);

	if (db_stat == S_OKAY) 
		return(true);	// station number already in there.

	// Put in the dbVista framework
	FacCfg.GetStationRecord(msFacNum, sStaID, &dbSta);				
	// fill in unused fields
	dbSta.s_data_type = 0;
	dbSta.s_sta_picklist_num = 0;
	strcpy(dbSta.sz_sta_short_name, "");

	// connect to dbVista db
	db_stat = d_fillnew (DB_STA_REC, (char *) &dbSta, CURR_DB);
	if (db_stat == S_OKAY) d_connect (STATION_SET, CURR_DB);
	if (db_stat == S_OKAY) return(true);
	else return (false);
}

bool CVifmDbVista::BuildDatabaseFramework(short sFacNum)
{
	db_sta_rec dbSta;
	bool bResult;

	CFacCfgWrap FacCfg;		// 29-Sep-2005 SFK	Removed static

	// Get number of stations in facility
	short sNum = FacCfg.GetNumStations(msFacNum);
	// Get the list of IDs in this station
	//long *pID = NULL;
	long *pID = new long[sNum];
	bResult = FacCfg.GetStationIDs(sFacNum, &sNum, pID);
	if (bResult) {
		// read the station number for each ID in the list and put in the dbVista framework
		for (short i=0; i<sNum; i++) {
			FacCfg.GetStationRecord(msFacNum, pID[i], &dbSta);				
			// fill in unused fields
			dbSta.s_data_type = 0;
			dbSta.s_sta_picklist_num = 0;
			strcpy(dbSta.sz_sta_short_name, "");

			// connect to dbVista db
			d_fillnew (DB_STA_REC, (char *) &dbSta, CURR_DB);
			d_connect (STATION_SET, CURR_DB);
		}
		delete [] pID;
	}
	return(bResult);
}

// 06-Jul-2005 SFK Comment out and replace
/*bool CVifmDbVista::OpenTheDatabase(short sFacNum)
{
	static CFacCfgWrap FacCfg;
	CDirUtilities Dir(false);  //QUIET MODE IS ALWAYS OFF FOR VIFM pjm 11/27/2007 for B2R1

	CString strFacilityDirectory = FacCfg.GetFacilityDirectory(sFacNum);
	Db.mstrDatabaseRootPath = strFacilityDirectory;
	strFacilityDirectory = Dir.AppendPath(strFacilityDirectory, Db.mstrComponentName);
	BOOL bStatus = Db.OpenDatabase(strFacilityDirectory, Db.mstrDatabaseName);
	// 12-Jan-2005  Check that we really can access this database.  d_open does not return an error if another
	// program is using this database as I would expect.
	if (bStatus) {
		int db_stat = d_findfm(STATION_SET, CURR_DB);
		if (db_stat == S_EACCESS) {	// someone else is using the database, so close the db now
			Db.CloseDatabase();
		}
	}
	return(bStatus);
}*/

//  08-Jun-2005	hn	Modify OpenTheDatabase to create a data store if none exists and to
//					return an error if creation of nonexistent data store fails. 
//	06-Jun-2005 SFK	Copied from GrandDbVista.cpp
bool CVifmDbVista::OpenTheDatabase(short sFacNum,CString* pstrErrorMsg)
{
	CString strErrorMsg;
	CFacCfgWrap FacCfg;		// 29-Sep-2005 SFK	Removed static
	CString strFacilityDirectory = FacCfg.GetFacilityDirectory(sFacNum);
	mstrDatabaseRootPath = strFacilityDirectory;
	strFacilityDirectory = m_Dir.AppendPath(strFacilityDirectory, mstrComponentName);
	
	bool bStatus = false;

	//If the datastore doesn't exist, create it and try again. hn 6/8/2005
	if (!m_Dir.FileExists (strFacilityDirectory, mstrDatabaseName + ".dbd")) 
	{
		//Create the data store if it doesn't exist.
		if (!CreateDataStore (sFacNum))
		{
			//We still didn't create the datastore.
			strErrorMsg.Format("Database could not be created at %s.", strFacilityDirectory);
			MessageBox(NULL, strErrorMsg, "Database Creation Error", MB_OK|MB_ICONWARNING);
			pstrErrorMsg->Format("%s", strErrorMsg);
		}
		else
		{
			bStatus = true;
		}
	}

	//The newly created db will be opened if we successfully created it.
	CString cs;
	bStatus = OpenDatabase(strFacilityDirectory, mstrDatabaseName, &cs);
	
	// 12-Jan-2005  Check that we really can access this database.  d_open does not return an error if another
	// program is using this database as I would expect.
	if (bStatus) 
	{
		int db_stat = d_findfm(STATION_SET, CURR_DB);

		if (db_stat == S_EACCESS)	// someone else is using the database, so close the db now
			CloseDatabase();
	}
	return bStatus;
}

// xx-Jun-2005 hn	Added this function to create a new datastore by copying the default datastore
// 06-Jul-2005 SFK	Copied from GrandDbVista.cpp and modified for VIFM
bool CVifmDbVista::CreateDataStore(short sFacilityID)
{
	CVifmImportSupport ImportSupport;

	//Determine the empty data store and destination path for the facility's data store
	//location, then copy the contents of the empty db to the place it should be.
	// The result is whether or not this copy was successful. hn 6/8
	CString emptyDBPath, dataStorePath;
	emptyDBPath = ImportSupport.GetEmptyDBLocation();
	dataStorePath = ImportSupport.GetVifmDataStorePath (sFacilityID);
	
	//Copy the empty db, return the success of the copy operation.
	return (m_Dir.CopyDirectory (emptyDBPath,dataStorePath,true));
}
