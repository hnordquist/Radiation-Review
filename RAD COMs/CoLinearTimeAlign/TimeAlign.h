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
// TimeAlign.h : Declaration of the CTimeAlign

#ifndef TIMEALIGN_H
#define TIMEALIGN_H

//Needed to pick up those components that that LinearTimeAlign
//requires.
#ifndef _LINEAR
#define _LINEAR 1
#endif

#include "imports.h"
#import "CoGrandImport.dll"			no_namespace named_guids
#import "CoVifmImport.dll"			no_namespace named_guids

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTimeAlign
class ATL_NO_VTABLE CTimeAlign : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTimeAlign, &CLSID_TimeAlign>,
	public IDispatchImpl<ITimeAlign, &IID_ITimeAlign, &LIBID_COLINEARTIMEALIGNLib>
{
public:
	/// HN 10/25/2007 TT #532 Make the COMS member variables.
	IGrandImportPtr m_pIGrandImport;
	IVifmImportPtr m_pIVifmImport;

	// HN 10/25/2007 Now, initialize the COMs at startup
	CTimeAlign();
	virtual ~CTimeAlign();

	bool m_bDisableLogging;
	int m_nLoggingVerbosity;
	bool m_bQuietMode;
	long m_lFlagsForLibraries;

DECLARE_REGISTRY_RESOURCEID(IDR_TIMEALIGN)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTimeAlign)
	COM_INTERFACE_ENTRY(ITimeAlign)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ITimeAlign
public:
	// These changes did not show up in WinDiff.  WTF?  Anyway, we clearly need the station type added HN 10/24/07
	//STDMETHOD(AdjustTimestamps)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID, /*[in]*/ long lNumItems, /*[in]*/ DATE* pOriginalTimestamps, /*[out]*/DATE*pAdjustedTimestamps, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(AdjustTimestamps)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ short sStationType,/*[in]*/ long lNumItems, /*[in]*/ DATE* pOriginalTimestamps, /*[out]*/DATE*pAdjustedTimestamps, /*[out,retval]*/ VARIANT_BOOL* pbResult);

	//STDMETHOD(BackupDataStore)(/*[in]*/ BSTR bstrDestinationPath, /*[out, retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(BackupDataStore)(/*[in]*/ short sFacilityID, /*[in]*/ BSTR bstrDestinationPath, /*[out, retval]*/ VARIANT_BOOL* pbResult);

	STDMETHOD(CloseDataStore)();

	//STDMETHOD(GetDataByChannel)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID, /*[in]*/ short sStationType, /*[in]*/ short sChannelID, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime,  /*[out]*/ double * pdTimestamp, /*[out]*/ double* pdData, /*[in,out]*/ long *plNumMembers, /*[out]*/ DATE* pNextTime, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetDataByChannel)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ short sStationType, /*[in]*/ short sChannelID, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime,  /*[in,out]*/ VARIANT* pvData, /*[out,retval]*/ VARIANT_BOOL* pbResult);

	//STDMETHOD(GetDataByStation)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID, /*[in]*/ short sStationType, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ double * pdTimestamp, /*[in,out]*/ VARIANT* Struct, /*[in,out]*/ long *plNumMembers, /*[out]*/ DATE* pNextTime, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetDataByStation)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ short sStationType, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ VARIANT* pvData, /*[out,retval]*/ VARIANT_BOOL* pbResult);

	STDMETHOD(GetDataStatistics)(
		/*[in]*/ short sFacilityID, 
		/*[in]*/ long lStationID, 
		/*[in]*/ short sChanID, 
		/*[in]*/ short sStationType,
		/*[in]*/ DATE StartTime, 
		/*[in]*/ DATE EndTime, 
		/*[out]*/ DATE* pActualStartTime, 
		/*[out]*/ DATE* pActualEndTime, 
		/*[out]*/ long* plNumMembers, 
		/*[out,retval]*/ VARIANT_BOOL* pbResult);

	STDMETHOD(GetDataStorePath)(/*[in]*/ short sFacilityID, /*[out]*/ BSTR* bstrStorePath);

	//STDMETHOD(GetDataByDay)    (/*[in]*/ short sFacilityID, /*[in]*/ short sStationID, /*[in]*/ short sStationType, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime,  /*[in, out]*/ VARIANT* DayVariantArray, /*[in,out]*/ long *plNumMembers,  /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetDaySummaryData) (/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ short sStationType, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime,  /*[in, out]*/ VARIANT* DayVariantArray,  /*[out,retval]*/ VARIANT_BOOL* pbResult);
	
	//STDMETHOD(GetParameters)(/*[in]*/ short sFacID, /*[in]*/ short sNum, /*[in,out]*/ VARIANT* ParmsVarStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetParameters)(/*[in]*/ short sFacID, /*[in,out]*/ VARIANT* ParmsVarStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);

	STDMETHOD(GetDayStatistics)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ short sStationType, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ DATE* pActualStartTime, /*[out]*/ DATE* pActualEndTime, /*[out]*/ long* plNumMembers, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	
	STDMETHOD(GetDataStoreTimeRange)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ short sStationType, /*[in]*/ VARIANT_BOOL bErrDisplay, /*[out]*/ DATE* FirstTimestamp, /*[out]*/DATE* LastTimestamp, /*[out,retval]*/ VARIANT_BOOL* pbResult);

//	STDMETHOD(InitializeDataStore)(/*[in]*/ BSTR bstrDestinationPath, /*[out, retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(InitializeDataStore)(/*[in]*/ short sFacilityID, /*[in]*/ VARIANT_BOOL bQuestion, /*[out, retval]*/ VARIANT_BOOL* pbSuccess);


	//STDMETHOD(OpenDataStore)(/*[out, retval]*/ VARIANT_BOOL *pbResult);
	STDMETHOD(OpenDataStore)(/*[in]*/ short sFacilityID, /*[out, retval]*/ VARIANT_BOOL *pbResult);

//	STDMETHOD(RestoreDataStore)(/*[in]*/ BSTR bstrSourcePath, /*[out, retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(RestoreDataStore)(/*[in]*/ short sFacilityID, /*[in]*/ BSTR bstrSourcePath, /*[out, retval]*/ VARIANT_BOOL* pbResult);

//	STDMETHOD(SetParameters)(/*[in]*/ short sFacilityID, /*[in]*/ short sNum, /*[in]*/ VARIANT* ParmsVarStruct, /*[out,retval/*/ VARIANT_BOOL* pbResult);
	STDMETHOD(SetParameters)(/*[in]*/ short sFacilityID, /*[in]*/ VARIANT* ParmsVarStruct, /*[out,retval/*/ VARIANT_BOOL* pbResult);

	// These changes did not show up in WinDiff.  WTF?  Anyway, we clearly need the station type added HN 10/24/07
	//STDMETHOD(UnadjustTimestamps)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID, /*[in]*/ long lNumItems, /*[in]*/ DATE* pAdjustedTimestamps, /*[out]*/ DATE* pOriginalTimestamps, /*[out, retval]*/ VARIANT_BOOL* pbResult);	
	STDMETHOD(UnadjustTimestamps)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ short sStationType, /*[in]*/ long lNumItems, /*[in]*/ DATE* pAdjustedTimestamps, /*[out]*/ DATE* pOriginalTimestamps, /*[out, retval]*/ VARIANT_BOOL* pbResult);	
};

#endif //TIMEALIGN_H
