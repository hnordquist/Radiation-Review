// CreateAccessDatabase.h: interface for the CCreateAccessDatabase class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#if !defined(AFX_CREATEACCESSDATABASE_H__2A91DBD8_CF1B_41B5_9BA5_91A52847A95F__INCLUDED_)
#define AFX_CREATEACCESSDATABASE_H__2A91DBD8_CF1B_41B5_9BA5_91A52847A95F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCreateAccessDatabase  
{
public:
	CCreateAccessDatabase(const CString& csDatabasePath, const CString& csDatabaseName, const CString& errINIFile);
	// This is a base class, make the CreateTables pure virtual.
	virtual bool CreateTables () = 0;
	virtual ~CCreateAccessDatabase();
	
	ADOX::_CatalogPtr m_pCatalog;
	ADOX::_TablePtr m_pTable;
	ADOX::_IndexPtr m_pIndexPtr;

	//Error stuff
	long m_lErrID;
	long m_lErrSev;
	CString m_csError;
	CString m_csErrorINIFile;
	CString FormatMessage(CString arg, long lMsgID);
	void SetError (CString err, long lErrID);
	void ClearError(void);

};

#endif // !defined(AFX_CREATEACCESSDATABASE_H__2A91DBD8_CF1B_41B5_9BA5_91A52847A95F__INCLUDED_)
