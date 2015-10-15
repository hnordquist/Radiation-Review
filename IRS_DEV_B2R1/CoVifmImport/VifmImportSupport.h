// GrandVifmSupport.h: interface for the CGrandVifmSupport class.
//
// 06-Jun-2005 hn	This class was added to support finding DLL folders and data store
//					paths for the CoGrandImport interface function CreateNewDataStore.
//					This code was blatantly stolen from Kelly Michel's GPSImport project
//					and was modified to search the registry for the CoGrandImport COM.
//					The registry file GrandImport.rgs was also modified to place the 
//					COM keys associating the *.BID file type with the CoGrandImport COM
//					for use with Import Manager.
// 06-Jul-2005 SFK	Copied from GrandImportSupport.h and modified for VIFM
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIFMIMPORTSUPPORT_H)
#define AFX_VIFMIMPORTSUPPORT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVifmImportSupport  
{
public:
	CString GetEmptyDBLocation();
	CString GetVifmDataStorePath (short sFacID);
	CString GetVifmImportDLLLocation();
	CVifmImportSupport();
	virtual ~CVifmImportSupport();
private:
	CString m_DLLLocationPath;
};

#endif // !defined(AFX_VIFMIMPORTSUPPORT_H)
