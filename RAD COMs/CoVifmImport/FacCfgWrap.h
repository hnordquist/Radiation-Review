// FacCfgWrap.h: interface for the CFacCfgWrap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACCFGWRAP_H__CE0F5BC6_45C6_11D6_8190_00C04F60E89B__INCLUDED_)
#define AFX_FACCFGWRAP_H__CE0F5BC6_45C6_11D6_8190_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DbVista.h"
//#include <vista.h>

#define MAX_NAME_LEN 39			// must be same as DB_NAME_LEN in raddefs.h
#define MAX_SHRT_NAME_LEN 9	// must be same as DB_SHORT_NAME_LEN in raddefs.h	
//#define BOT_TYPE 64

#include "db_vifm.h"

class CFacCfgWrap  
{
public:
	CFacCfgWrap();
	virtual ~CFacCfgWrap();

	CString GetFacilityDirectory(short sFacID);
	int GetNumStations(short sFacID);
	bool GetStationIDs(short sFacID, short *psNum, long* pIDs);
	bool GetStationRecord(short sFacDbNum, long lSta, struct db_sta_rec *dbSta);

private:
	struct db_sta_rec * mpStaRecs;
};

#endif // !defined(AFX_FACCFGWRAP_H__CE0F5BC6_45C6_11D6_8190_00C04F60E89B__INCLUDED_)
