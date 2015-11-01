// TimeAlignIni.h: interface for the CTimeAlignIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEALIGNINI_H__052A6B11_2D21_11D6_817E_00C04F60E89B__INCLUDED_)
#define AFX_TIMEALIGNINI_H__052A6B11_2D21_11D6_817E_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DbIni.h"
#include "imports.h"


//#include "FacCfgWrap.h"

struct	LinearTA_Parms 
{
	short	sStaNum;
	short	sStaType;
	DATE	Event1Time;
	DATE	Event2Time;
	double	dSlope;
	double	dIntercept;
	bool	bEnableTimeAlign;
	bool	bEnableStation;
};

class CTimeAlignIni : public CDbIni  
{
public:
	CTimeAlignIni();
	virtual ~CTimeAlignIni();

	bool GetTimeAlignParms(const short sFacID, const short sStaID, const short sStaType, struct LinearTA_Parms* pParms);
	void SetDataStorePath(const short sFacID);
	bool WriteTimeAlignParms(const short sFacID, const short sStaID, const short sStaType, struct LinearTA_Parms* pParms);
	
	struct LinearTA_Parms* mpStaInfo;
	IFacilityMgrDataPtr m_pIFacCfgMgr;

//	CFacCfgWrap m_FacMgr;

};

#endif // !defined(AFX_TIMEALIGNINI_H__052A6B11_2D21_11D6_817E_00C04F60E89B__INCLUDED_)
