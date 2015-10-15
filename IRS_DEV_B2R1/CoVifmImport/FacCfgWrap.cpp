// FacCfgWrap.cpp: implementation of the CFacCfgWrap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "FacCfgWrap.h"
#include "SafeArrayUtil.h"
#include "TypeDefinitions.h"
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

// 28-Sep-2005 SFK	Added check on if pIFacCfgMgr is not NULL and
//					VariantInit of saStationRecord (VERY IMPORTANT TO DO THIS!!!!!!)
bool CFacCfgWrap::GetStationRecord(short sFacID, long lSta, struct db_sta_rec *pdbSta)
{
	VARIANT saStationRecord;
	VariantInit(&saStationRecord);
	bool bStatus = false;

	// hardwire in station type string
	pIFacCfgMgr.CreateInstance(CLSID_FacilityMgrData);
	if (pIFacCfgMgr) pIFacCfgMgr->GetStationRecord(sFacID, lSta, L"BOT", &saStationRecord);
	pIFacCfgMgr = NULL;
	if(saStationRecord.vt != VT_EMPTY)	{
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
		SA_GetElem(saStationRecord.parray, 4, &TempStr); //DR 534 pjm 12/11/2007
		// hardwire in type string
		if (_stricmp(TempStr.GetBuffer(MAX_NAME_LEN), "BOT") == 0) {
			pdbSta->s_sta_type = VIFM_TYPE;
			pdbSta->s_data_type = 1;		// 1 is FLOAT_TYPE
		}
		else {
			pdbSta->s_sta_type = 0;
			pdbSta->s_data_type = 0;
		}
	}
	else return(FALSE);

  return(TRUE);
}
                            

// 28-Sep-2005 SFK	Added check on if pIFacCfgMgr is not NULL and
//					VariantInit of List (VERY IMPORTANT TO DO THIS!!!!!!)
bool CFacCfgWrap::GetStationIDs(short sFacID, short *psNum, long* pIDs)
{
	USES_CONVERSION;

	VARIANT List;
	VariantInit(&List);

	pIFacCfgMgr.CreateInstance(CLSID_FacilityMgrData);
	if (pIFacCfgMgr) pIFacCfgMgr->GetStationIDsWithInstrumentType(sFacID, L"BOT", true, &List);
	pIFacCfgMgr = NULL;

	if(List.vt == VT_EMPTY) 
	{
		VariantClear(&List);
		return FALSE;
	}
	else 
	{
		long NumStations;
		
		SafeArrayGetUBound(List.parray, 1, &NumStations);

		if (NumStations > *psNum)
		{
			VariantClear(&List);
			return FALSE;		// unexpected number of stations
		}


		CString TempStr;
		long lTemp;
		int j=0;

		for(LONG i = 0; i <= NumStations; i++) 
		{

			SA_GetElem(List.parray, i, 0, &lTemp);			// station number id
			pIDs[j] = lTemp;
			j++;

		}
		//SafeArrayDestroy(List.parray);
		*psNum = j;		// return the number of vifm stations at the facility

	}

	VariantClear(&List);
	return(TRUE);


}

// 28-Sep-2005 SFK	Added check on if pIFacCfgMgr is not NULL
int CFacCfgWrap::GetNumStations(short sFacID)
{
	int bNum = 0;
	pIFacCfgMgr.CreateInstance(CLSID_FacilityMgrData);
	if (pIFacCfgMgr) bNum = pIFacCfgMgr->GetNumberOfStationsForFacility(sFacID, (VARIANT_BOOL)true);
	pIFacCfgMgr = NULL;
	return(bNum); 
}

// 28-Sep-2005 SFK	Added check on if pIFacCfgMgr is not NULL
CString CFacCfgWrap::GetFacilityDirectory(short sFacID)
{
	USES_CONVERSION;
	CString strTemp;
	pIFacCfgMgr.CreateInstance(CLSID_FacilityMgrData);
	if (pIFacCfgMgr) strTemp = W2T(pIFacCfgMgr->GetFacilityDirectory(sFacID));
	pIFacCfgMgr = NULL;
	return(strTemp);
}
                            
