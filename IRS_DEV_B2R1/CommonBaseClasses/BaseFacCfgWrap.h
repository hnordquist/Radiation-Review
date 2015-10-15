// BaseFacCfgWrap.h: interface for the CBaseFacCfgWrap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afx.h>

#include "imports.h"

#define MAX_NAME_LEN 39
#define MAX_SHRT_NAME_LEN 9	
#include "TypeDefinitions.h"

class CBaseFacCfgWrap  
{
protected:
	IFacilityMgrDataPtr m_pIFacCfgMgr;
public:
	CBaseFacCfgWrap();
	virtual ~CBaseFacCfgWrap();

	virtual CString GetFacilityDirectory(short sFacID);
	virtual CString GetFacilityLongName(short sFacID);
	virtual int GetNumStations(short sFacID);
	virtual bool GetStationIDs(short sFacID, short *psNum, long* pIDs);
	virtual bool GetStationRecord(short sFacID, long lSta, void *dbSta) = 0;
	virtual bool FacilityExists(short sFacID);
	virtual void SetFlags(long lFlags);

	BSTR m_bstrInstrumentName;
	CString m_csInstrumentName;
};

