// DbIni.cpp: implementation of the CDbIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "DbIni.h"
#include "DirUtilities.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDbIni::CDbIni()
{
	m_strComponentName.Empty();
	m_strDefaultPath.Empty();
	m_strMyIniFilename.Empty();
	m_strDataStorePath.Empty();
	m_bOpen = false;

}

CDbIni::~CDbIni()
{

}


///////////////////////////////////////////////////////////////////////////
//	Name:	CopyDataStore
//
//	Description:
//	Copy all the files in the specified source directory to the specified
//	destination directory.  Based on bQuestion ask a question about overwriting
//	any existing files, else just copy the files whether or not something
//	already exists.
//	
//	Declaration:
//	bool CDbIni::CopyDataStore(CString &strSourceRootPath, CString &strDestinationRootPath, bool bQuestion)
//
//	Input:	strSourceRootPath		base path of where to get the datastore to copy from
//			strDestinationRootPath	base path of where to copy the datastore to
//			bQuestion				whether or not to ask a question if the datastore exists at the destination
//			m_strComponentName		name of the subpath where this instrument's datastore is kept
//			m_strMyIniFilename		name of file that indicates a datastore is present
//
//	Output:	none
//
//	Return:	true (db exists and initialized) / false (error in operation)
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//////////////////////////////////////////////////////////////////
bool CDbIni::CopyDataStore(CString &strSourceRootPath, CString &strDestinationRootPath, bool bQuestion)
{
	CDirUtilities Dir(false);  //QUIET MODE IS ALWAYS OFF FOR TIMEALIGN pjm 11/27/2007 for B2R1
	bool bOverwrite;
	bool bSuccess = false;
	CString strError;
	
	if ((m_strComponentName.GetLength() == 0) || (m_strMyIniFilename.GetLength() == 0)) {
		strError.Format("BackupDataStore: No component name or ini filename program error, cannot continue.");
		MessageBox(NULL, strError, "Fatal Error", MB_OK|MB_ICONWARNING);
		return(false);
	}

	CString strFullDestinationPath = Dir.AppendPath(strDestinationRootPath, m_strComponentName);	// create the Instrument Com specific path
	CString strFullSourcePath = Dir.AppendPath(strSourceRootPath, m_strComponentName);	// create the Instrument Com specific path

	bool bExists = Dir.FileExists(strFullDestinationPath, m_strMyIniFilename);	// does a datastore exist there already?

	if (bExists && bQuestion) {
		bOverwrite = Dir.OverwriteDbQuestion(strFullDestinationPath);		// datastore exists, see if they want to overwrite
	}
	else bOverwrite = true;

	if ((!bExists) || (bExists && bOverwrite)) {	// copy current datastore to the specified destination
		bExists = Dir.FileExists(strFullSourcePath, m_strMyIniFilename);	// is a file even there to copy?
		if (bExists) bSuccess = Dir.CopyDirectory(strFullSourcePath, strFullDestinationPath, true);
		else bSuccess = true;
	}
	return(bSuccess);
}
