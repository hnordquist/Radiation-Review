// FacCfgWrap.cpp: implementation of the CFacCfgWrap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "FacCfgWrap.h"

// HN 10/24/07 changed this import for my machine's E10R4 version of FacMgr.
//#import "..\CoFacilityMgr\CoFacilityMgr.tlb" no_namespace named_guids	// 02-Dec-2004 SFK Changed name
#import "c:\INSTALLATIONS\irs_B2R1\comlibs\CoFacilityMgr.dll" no_namespace named_guids	// 02-Dec-2004 SFK Changed name

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IFacilityMgrDataPtr pIFacCfgMgr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFacCfgWrap::CFacCfgWrap()
{
}

CFacCfgWrap::~CFacCfgWrap()
{
}

CString CFacCfgWrap::GetFacilityDirectory(short sFacID)
{
	USES_CONVERSION;
	// Change done for E10R3 9/28/05 per SFK note
	// instantiate the facmgr each time a method is called.  HN 10/24/07
	//return(W2T(pIFacCfgMgr->GetFacilityDirectory(sFacID)));
	CString strTemp("");
	pIFacCfgMgr.CreateInstance (CLSID_FacilityMgrData);
	if (pIFacCfgMgr)
	{
		strTemp = W2T (pIFacCfgMgr->GetFacilityDirectory (sFacID));
		pIFacCfgMgr = NULL;
	}
	return (strTemp);
}
                            
int CFacCfgWrap::GetNumStations(short sFacID)
{
	// Change done for E10R3 9/28/05 per SFK note
	// instantiate the facmgr each time a method is called.  HN 10/24/07
	// I changed this to send in VARIANT_TRUE, and not (VARIANT_BOOL) true, which is not the same.
	//return(pIFacCfgMgr->GetNumberOfStationsForFacility(sFacID, (VARIANT_BOOL)true)); 
	int iNum = 0;  //<<QA>>initialize the variable!  But how is it handled on the callers side?
	pIFacCfgMgr.CreateInstance (CLSID_FacilityMgrData);
	if (pIFacCfgMgr)
	{
		iNum = pIFacCfgMgr->GetNumberOfStationsForFacility (sFacID, VARIANT_TRUE);
		pIFacCfgMgr = NULL;
	}
	return (iNum);
}
