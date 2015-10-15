// FacCfgWrap.h: interface for the CFacCfgWrap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACCFGWRAP_H__CE0F5BC6_45C6_11D6_8190_00C04F60E89B__INCLUDED_)
#define AFX_FACCFGWRAP_H__CE0F5BC6_45C6_11D6_8190_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_NAME_LEN 39			// must be same as DB_NAME_LEN in raddefs.h
#define MAX_SHRT_NAME_LEN 9	// must be same as DB_SHORT_NAME_LEN in raddefs.h	
#define BINARY_TYPE 128
// copied db_sta_rec from db_rmon.h
struct db_sta_rec {
   short s_sta_num_key;
   short s_sta_picklist_num;
   char sz_sta_name[40];
   char sz_sta_short_name[10];
   short s_sta_type;
   short s_data_type;
};

class CFacCfgWrap  
{
public:
	CFacCfgWrap();
	virtual ~CFacCfgWrap();

	CString GetFacilityDirectory(short sFacID);
	int GetNumStations(short sFacID);
	bool GetStationRecord(short sFacDbNum, long lSta, struct db_sta_rec *dbSta);
};

#endif // !defined(AFX_FACCFGWRAP_H__CE0F5BC6_45C6_11D6_8190_00C04F60E89B__INCLUDED_)
