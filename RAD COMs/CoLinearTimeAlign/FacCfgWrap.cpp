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

// HN 10/24/07 changed this import for my machine's E10R4 version of FacMgr.
//#import "..\CoFacilityMgr\CoFacilityMgr.tlb" no_namespace named_guids	// 02-Dec-2004 SFK Changed name
//#import "c:\INSTALLATIONS\irs_B2R1\comlibs\CoFacilityMgr.dll" no_namespace named_guids	// 02-Dec-2004 SFK Changed name

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
