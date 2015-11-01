// AccessBinaryDb.cpp: implementation of the CBinaryDbAccess class.
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
#include "AccessBinaryDb.h"
#include "BinaryImportSupport.h"	// 11-Jul-2005 SFK Added
#include "FacCfgWrap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
// The CBinaryDbAccess class contains unique functionality to cover
// specific binary database actions for the common functions contained
// in the CDbAccess class.
//////////////////////////////////////////////////////////////////////

CBinaryDbAccess::CBinaryDbAccess()
{
	TRACE("CBinaryDbAccess::CTOR\n");
	mstrComponentName = "Data\\Import\\Binary";		// sfk 08/13/2004 Changed to be in /Import directory (other passed in)
	mstrDatabaseName = "BinaryImport.mdb";		// sfk 07/07/2004 Changed name to BinaryImport.mdb from Binary.mdb
	mstrMyDbFilename = "BinaryImport.mdb";
}


CBinaryDbAccess::~CBinaryDbAccess()
{
	TRACE("CBinaryDbAccess::DTOR\n");
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
//	bool CBinaryDbAccess::ClearDatabase(const CString &strDbName, bool bPrompt, CString *pstrErrorMsg)
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
bool CBinaryDbAccess::ClearDatabase(const CString &strDbName, bool bPrompt, CString *pstrErrorMsg)
{
	return(true);
}


// 11-Jul-2005 SFK	Check whether a database exists.  If not, then create one.
//					Also return an error if couldn't create
bool CBinaryDbAccess::OpenTheDatabase(short sFacNum, CString* pstrErrorMsg)
{
	CFacCfgWrap FacCfg;		// 28-Sep-2005 SFK Removed static
	CDirUtilities Dir(false);  //QUIET MODE IS ALWAYS OFF FOR the non-NDAR Binary Component pjm 11/27/2007 for B2R1

	CString strFacilityDirectory = FacCfg.GetFacilityDirectory(sFacNum);
	mstrDatabaseRootPath = strFacilityDirectory;
	strFacilityDirectory = Dir.AppendPath(strFacilityDirectory, mstrComponentName);

	BOOL bStatus = false;
	CString strErrorMsg;
	//If the datastore doesn't exist, create it and try again. hn 6/8/2005
	if (!Dir.FileExists (strFacilityDirectory, mstrDatabaseName)) {
		//Create the data store if it doesn't exist.
		if (!CreateDataStore (sFacNum))
		{
			//We still didn't create the datastore.
			bStatus = false;
			strErrorMsg.Format("Database could not be created at %s.", strFacilityDirectory);
			MessageBox(NULL, strErrorMsg, "Database Creation Error", MB_OK|MB_ICONWARNING);
			pstrErrorMsg->Format("%s", strErrorMsg);
		}
		else
		{
			bStatus = true;
		}
	}
	/*  for debug
	bool b;
	CString cs1("cs1");
	CString cs2("cs2");
	CString cs3("cs3");

	char str1[] = "str1";
	char str2[] = "str2";
	char str3[] = "str3";

	b = Dir.FileExists (cs1, cs1);
	b = Dir.FileExists (cs3);
	b = Dir.FileExists (str1, str2);
	b = Dir.FileExists (str3);

	b = Dir.FileExists (strFacilityDirectory, mstrDatabaseName);
	*/

	return(OpenDatabase(strFacilityDirectory, mstrDatabaseName));
}


// 11-Jul-2005 SFK  Added this function to create a new datastore by copying the default datastore
bool CBinaryDbAccess::CreateDataStore(short sFacilityID)
{
	CBinaryImportSupport ImportSupport;

	//Determine the empty data store and destination path for the facility's data store
	//location, then copy the contents of the empty db to the place it should be.
	// The result is whether or not this copy was successful. hn 6/8
	CString emptyDBPath, dataStorePath;
	emptyDBPath = ImportSupport.GetEmptyDBLocation();
	dataStorePath = ImportSupport.GetBinaryDataStorePath (sFacilityID);
	
	CDirUtilities Dir(false);  //QUIET MODE IS ALWAYS OFF FOR the non-NDAR Binary Component pjm 11/27/2007 for B2R1;
	//Copy the empty db, return the success of the copy operation.
	return (Dir.CopyDirectory (emptyDBPath,dataStorePath,TRUE));

}
