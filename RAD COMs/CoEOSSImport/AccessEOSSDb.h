// AccessEOSSDb.h: interface for the CBinaryDbAccess class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "AccessDb.h"

class CEOSSDbAccess : public CDbAccess  
{
public:
	bool IsFacilityDBOpen (short sFacilityID,const CString& csFacDir = "");

	CEOSSDbAccess();
	virtual ~CEOSSDbAccess();

	virtual bool ClearDatabase(const CString& strDbName, const bool bPrompt= true,  CString *pstrErrorMsg= NULL);
	virtual bool CreateDataStore (short sFacilityID);		
	virtual bool OpenTheDatabase(short sFacNum, CString* pstrErrorMsg = NULL,const CString& csFacDir = "");	

	void SetQuietMode(bool bQuietMode);
	//IFacilityMgrDataPtr m_pIFacMgr;
};
