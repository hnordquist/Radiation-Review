// VifmDbVista.h: interface for the CVifmDbVista class.
//
//////////////////////////////////////////////////////////////////////
#ifndef VIFMDBVISTA_H
#define VIFMDBVISTA_H

#include "VistaDb.h"
#include "db_vifm.h"
#include "vista.h"

class CVifmDbVista : public CDbVista  
{
public:
	CVifmDbVista();
	CVifmDbVista(bool bQuietMode);
	virtual ~CVifmDbVista();

	virtual bool BuildDatabaseFramework(short sFacNum);
	//virtual bool ClearDatabase(const CString& strDbName, bool bPrompt= true,  CString *pstrErrorMsg= NULL);
	//virtual bool OpenTheDatabase(short sFacNum);
	virtual bool OpenTheDatabase(short sFacNum,CString* pstrErrorMsg = NULL);	// 06-Jul-2005 SFK added with new arg
	virtual bool AddStationToDatabaseFramework(short sFacNum, short sStaID);
	virtual bool CreateDataStore(short sFacilityID);		// 06-Jul-2005 SFK Added

};

#endif // VIFMDBVISTA_H
