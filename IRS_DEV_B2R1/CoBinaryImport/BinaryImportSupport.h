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
// 11-Jul-2005 SFK	Adapted GRAND stuff to binary
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINARYIMPORTSUPPORT_H__INCLUDED_)
#define AFX_BINARYIMPORTSUPPORT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBinaryImportSupport  
{
public:
	CString GetEmptyDBLocation();
	CString GetBinaryDataStorePath (short sFacID);
	CString GetBinaryImportDLLLocation();
	CBinaryImportSupport();
	virtual ~CBinaryImportSupport();
private:
	CString m_DLLLocationPath;
};

#endif // !defined(AFX_BINARYIMPORTSUPPORT_H__INCLUDED_)
