// GrandFacCfgWrap.cpp

#include "stdafx.h"
#include "GrandFacCfgWrap.h"
#include "SafeArrayUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrandFacCfgWrap::CGrandFacCfgWrap()
{
	m_csInstrumentName = CString("grand");
	m_bstrInstrumentName = m_csInstrumentName.AllocSysString();
}

bool CGrandFacCfgWrap::GetStationRecord(short sFacID, long lSta, void *dbSta)
{
	db_sta_rec *pdbSta = (db_sta_rec *)dbSta;
	bool bStatus = false;

	VARIANT saStationRecord;
	::VariantInit(&saStationRecord);

	if (m_pIFacCfgMgr != NULL)
	{ 
		m_pIFacCfgMgr->GetStationRecord(sFacID, lSta, m_bstrInstrumentName, &saStationRecord);

		if(saStationRecord.vt != VT_EMPTY)	
		{
			//Unload the information from the safearray.
			CString TempStr;

			SA_GetElem(saStationRecord.parray, 0, &(pdbSta->s_sta_num_key));
			SA_GetElem(saStationRecord.parray, 1, &(pdbSta->s_sta_picklist_num));
			SA_GetElem(saStationRecord.parray, 2, &TempStr);  //BSTR LongName

			strncpy(pdbSta->sz_sta_name,(LPCTSTR)TempStr,sizeof(pdbSta->sz_sta_name));
			strncpy(pdbSta->sz_sta_short_name,(LPCTSTR)TempStr,sizeof(pdbSta->sz_sta_short_name));
			SA_GetElem(saStationRecord.parray, 4, &TempStr); //BSTR InstrumentType

			if (_stricmp((LPCTSTR)TempStr, (LPCTSTR)m_csInstrumentName) == 0)
			{
				pdbSta->s_sta_type = GRAND_TYPE;
				pdbSta->s_data_type = FLOAT_TYPE;		// 1 is FLOAT_TYPE
			}
			else 
			{
				pdbSta->s_sta_type = 0;
				pdbSta->s_data_type = 0;
			}
			bStatus = true;
		}
	} //end if (m_pIFacCfgMgr != NULL)

	::VariantClear(&saStationRecord);
	return bStatus;
}
                            
