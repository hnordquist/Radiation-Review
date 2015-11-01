// GrandImportSupport.h: interface for the CGrandImportSupport class.
//
// This class was added to support finding DLL folders and data store
// paths for the CoGrandImport interface function CreateNewDataStore.
// This code was blatantly stolen from Kelly Michel's GPSImport project
// and was modified to search the registry for the CoGrandImport COM.
// The registry file GrandImport.rgs was also modified to place the 
// COM keys associating the *.BID file type with the CoGrandImport COM
// for use with Import Manager.
// hn 6/8
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afx.h>
#include "GrandFacCfgWrap.h"

class CGrandImportSupport  
{
public:
	CString GetEmptyDBLocation();
	CString GetGrandDataStorePath (short sFacID);
	CString GetGrandImportDLLLocation();
	CGrandImportSupport(CGrandFacCfgWrap *pFacCfg, bool bQuietMode);
	virtual ~CGrandImportSupport();
private:
	CGrandFacCfgWrap *m_pFacCfg;
	bool m_bQuietMode;
	CString m_DLLLocationPath;
};

