// FacCfgWrap.h: interface for the CFacCfgWrap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACCFGWRAP_H__CE0F5BC6_45C6_11D6_8190_00C04F60E89B__INCLUDED_)
#define AFX_FACCFGWRAP_H__CE0F5BC6_45C6_11D6_8190_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFacCfgWrap  
{
public:
	CFacCfgWrap();
	virtual ~CFacCfgWrap();
	CString GetFacilityDirectory(short sFacID);
	int GetNumStations(short sFacID);

};

#endif // !defined(AFX_FACCFGWRAP_H__CE0F5BC6_45C6_11D6_8190_00C04F60E89B__INCLUDED_)
