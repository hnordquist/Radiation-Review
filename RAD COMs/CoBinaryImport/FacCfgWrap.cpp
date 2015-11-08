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

                    