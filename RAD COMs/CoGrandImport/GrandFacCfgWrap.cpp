/*
This work was supported by the United States Member State Support Program to IAEA Safeguards;
the U.S. Department of Energy, Office of Nonproliferation and National Security, International
Safeguards Division; and the U.S. Department of Energy, Office of Safeguards and Security.

LA-CC-14-040. This software was exported from the United States in accordance with the Export
Administration Regulations. Diversion contrary to U.S. law prohibited.

Copyright 2015, Los Alamos National Security, LLC. This software application and associated
material ("The Software") was prepared by the Los Alamos National Security, LLC. (LANS), under
Contract DE-AC52-06NA25396 with the U.S. Department of Energy (DOE). All rights in the software
application and associated material are reserved by DOE on behalf of the Government and LANS
pursuant to the contract.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or other materials provided
with the distribution.
3. Neither the name of the "Los Alamos National Security, LLC." nor the names of its contributors
may be used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS
NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRAT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
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
                            
