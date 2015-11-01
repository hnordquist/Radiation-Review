// FacCfgWrap.cpp: implementation of the CFacCfgWrap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "FacCfgWrap.h"
#include "SafeArrayUtil.h"
#include "imports.h"

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

// 29-Sep-2005 SFK	Added create pIFacCfgMgr, check if pIFacCfgMgr is NULL
CString CFacCfgWrap::GetFacilityDirectory(short sFacID)
{
	USES_CONVERSION;
	CString strTemp;
	pIFacCfgMgr.CreateInstance(CLSID_FacilityMgrData);
	if (pIFacCfgMgr) strTemp = W2T(pIFacCfgMgr->GetFacilityDirectory(sFacID));
	pIFacCfgMgr = NULL;
	return(strTemp);
}
                            
// 29-Sep-2005 SFK	Added create pIFacCfgMgr, check if pIFacCfgMgr is NULL and
//					VariantInit of saStationRecord (VERY IMPORTANT TO DO THIS!!!!!!)
bool CFacCfgWrap::GetStationRecord(short sFacID, long lSta, struct db_sta_rec *pdbSta)
{
	VARIANT saStationRecord;
	VariantInit(&saStationRecord);

	// hardwire in station type string
	pIFacCfgMgr.CreateInstance(CLSID_FacilityMgrData);

	if (pIFacCfgMgr)
	{
		pIFacCfgMgr->GetStationRecord(sFacID, lSta, "binary", &saStationRecord);
		pIFacCfgMgr = NULL;
	}
	else
	{
		ASSERT(FALSE);
		VariantClear(&saStationRecord);
		return false;
	}

	if(saStationRecord.vt != VT_EMPTY)	
	{
		//Unload the information from the safearray.
		CString TempStr;

		SA_GetElem(saStationRecord.parray, 0, &(pdbSta->s_sta_num_key));

		SA_GetElem(saStationRecord.parray, 1, &(pdbSta->s_sta_picklist_num));

		SA_GetElem(saStationRecord.parray, 3, &TempStr);
		strncpy(pdbSta->sz_sta_name, TempStr.GetBuffer(MAX_NAME_LEN), MAX_NAME_LEN);
		pdbSta->sz_sta_name[MAX_NAME_LEN] = '\0';
		
		// 10-Aug-2005 SFK  Correct to use short name
		SA_GetElem(saStationRecord.parray, 3, &TempStr);
		strncpy(pdbSta->sz_sta_short_name, TempStr.GetBuffer(MAX_SHRT_NAME_LEN), MAX_SHRT_NAME_LEN);
		pdbSta->sz_sta_short_name[MAX_SHRT_NAME_LEN] = '\0';

		//SA_GetElem(saStationRecord.parray, 2, &TempStr);
		SA_GetElem(saStationRecord.parray, 4, &TempStr); //DR 533 pjm 12/11/2007
		// hardwire in type string
		if (_stricmp(TempStr.GetBuffer(MAX_NAME_LEN), "binary") == 0) 
		{
			pdbSta->s_sta_type = BINARY_TYPE;
			pdbSta->s_data_type = 1;		// 1 is FLOAT_TYPE
		}
		else 
		{
			pdbSta->s_sta_type = 0;
			pdbSta->s_data_type = 0;
		}
	}
	else
	{
		VariantClear(&saStationRecord);
		return(false);
	}
	VariantClear(&saStationRecord);
	return(true);
}

// 29-Sep-2005 SFK	Added create pIFacCfgMgr, check if pIFacCfgMgr is NULL
int CFacCfgWrap::GetNumStations(short sFacID)
{
	int iNum;
	pIFacCfgMgr.CreateInstance(CLSID_FacilityMgrData);
	if (pIFacCfgMgr) iNum = pIFacCfgMgr->GetNumberOfStationsForFacility(sFacID, (VARIANT_BOOL)true);
	pIFacCfgMgr = NULL;
	return(iNum); 
}

                    