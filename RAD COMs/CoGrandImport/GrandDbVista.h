// GrandDbVista.h: interface for the CGrandDbVista class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "VistaDb.h"
#include "db_rmon.h"
#include "vista.h"
#include "GrandFacCfgWrap.h"

class CGrandDbVista : public CDbVista  
{
public:
	CGrandDbVista(bool bQuietMode);
	CGrandDbVista();
	virtual ~CGrandDbVista();
	void FinishConstruction();
	
	virtual bool CreateDataStore (short sFacilityID);
	virtual bool BuildDatabaseFramework(short sFacNum);
	//Use base class ClearDatabase
	//virtual bool ClearDatabase(const CString& strDbName, bool bPrompt= true,  CString *pstrErrorMsg= NULL);
	virtual bool OpenTheDatabase(short sFacNum,CString *pstrErrorMsg= NULL);
	virtual bool AddStationToDatabaseFramework(short sFacNum, short sStaID);
	void SetFacMgr(CGrandFacCfgWrap *pFacCfg);
	void SetQuietMode(bool bQuietMode);

	//CDirUtilities m_Dir;  ALREADY IN BASE CLASS
	CGrandFacCfgWrap *m_pFacCfg;
	//bool m_bQuietMode; USE mbQuietMode in BASE CLASS
};

