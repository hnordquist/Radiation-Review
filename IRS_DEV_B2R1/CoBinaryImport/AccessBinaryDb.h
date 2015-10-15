// AccessBinaryDb.h: interface for the CBinaryDbAccess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINARYDBACCESS_H__BD000CB4_05DF_11D6_8176_00C04F60E89B__INCLUDED_)
#define AFX_BINARYDBACCESS_H__BD000CB4_05DF_11D6_8176_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AccessDb.h"

class CBinaryDbAccess : public CDbAccess  
{
public:

	CBinaryDbAccess();
	virtual ~CBinaryDbAccess();

	virtual bool ClearDatabase(const CString& strDbName, const bool bPrompt= true,  CString *pstrErrorMsg= NULL);
	virtual bool CreateDataStore (short sFacilityID);		// 11-Jul-2005 SFK added
	virtual bool OpenTheDatabase(short sFacNum, CString* pstrErrorMsg = NULL);	// 11-Jul-2005 SFK Added error msg arg

};

#endif // !defined(AFX_BINARYDBACCESS_H__BD000CB4_05DF_11D6_8176_00C04F60E89B__INCLUDED_)
