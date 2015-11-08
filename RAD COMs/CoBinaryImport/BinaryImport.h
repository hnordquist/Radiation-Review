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
// BinaryImport.h : Declaration of the CBinaryImport

#ifndef __BINARYIMPORT_H_
#define __BINARYIMPORT_H_

#include "resource.h"       // main symbols
#include "AccessBinaryDb.h"

/////////////////////////////////////////////////////////////////////////////
// CBinaryImport
class ATL_NO_VTABLE CBinaryImport : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBinaryImport, &CLSID_BinaryImport>,
	public IDispatchImpl<IBinaryImport, &IID_IBinaryImport, &LIBID_COBINARYIMPORTLib>
{
public:
	CBinaryImport()
	{
		m_bQuietMode = false;//true;
	}

	bool m_bQuietMode;

	CBinaryDbAccess m_Db;

DECLARE_REGISTRY_RESOURCEID(IDR_BINARYIMPORT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBinaryImport)
	COM_INTERFACE_ENTRY(IBinaryImport)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IBinaryImport
public:
	STDMETHOD(AddData)(/*[in]*/ short sFacilityID, /*[in]*/ BSTR bstrFilename, /*[in]*/ VARIANT_BOOL bOverwrite, /*[out]*/ BSTR* bstrResult, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(BackupDataStore)(/*[in]*/ short sFacilityID, /*[in]*/BSTR bstrDestinationPath, /*[in]*/ VARIANT_BOOL bQuestion, /*[in]*/ VARIANT_BOOL bDefault, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(CloseDataStore)();
	STDMETHOD(CreateNewDataStore)(/*[in]*/ short sFacilityID, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetDataByChannel)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID, /*[in]*/ short sChannelID, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ VARIANT *GrandVarStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetDataByStation)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID,  /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ VARIANT *GrandStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetDataStatistics)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID,  /*[in]*/ short sChannelID,/*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ DATE* pActualStartTime, /*[out]*/ DATE* pActualEndTime, /*[out]*/ long* plNumMembers, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetDaySummaryData)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime,  /*[out]*/ VARIANT* DayVariantArray, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetDataStorePath)(/*[in]*/ short sFacilityID, /*[out, retval]*/ BSTR* bstrStorePath);
	STDMETHOD(GetDataStoreSize)(/*[in]*/ short sFacilityID, /*[out]*/ double* pdSizeInBytes, /*[out, retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetDataStoreTimeRange)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID, /*[in]*/ VARIANT_BOOL bErrDisplay, /*[out]*/ DATE* FirstTimestamp, /*[out]*/DATE* LastTimestamp, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetDayStatistics)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ DATE* pActualStartTime, /*[out]*/ DATE* pActualEndTime, /*[out]*/ long* plNumMembers, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(GetFilesList)(/*[in]*/ short sFacilityID, /*[in]*/ BSTR bstrRootDir, /*[in]*/ VARIANT_BOOL bIncludeSubDirs, /*[in]*/ VARIANT_BOOL bMostRecent, /*[out]*/ VARIANT* psaFilesList);
	STDMETHOD(GetParameters)(/*[in]*/ short sFacilityID, /*[out]*/ VARIANT* pvParms, /*[out,retval]*/ VARIANT_BOOL* pbResult);	
	STDMETHOD(InitializeDataStore)(/*[in]*/ short sFacilityID, /*[in]*/ VARIANT_BOOL bQuestion, /*[out, retval]*/ VARIANT_BOOL* pbSuccess);
	STDMETHOD(OpenDataStore)(/*[in]*/ short sFacilityID, /*[out, retval]*/ VARIANT_BOOL* pbSuccess);
	STDMETHOD(RestoreDataStore)(/*[in]*/ short sFacilityID, /*[in]*/ BSTR bstrSourcePath, /*[in]*/ VARIANT_BOOL bQuestion, /*[in]*/ VARIANT_BOOL bDefault, /*[out, retval]*/ VARIANT_BOOL* pbResult);
	STDMETHOD(SetParameters)(/*[in]*/ short sFacilityID, /*[in]*/ VARIANT ParmsVarStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);

};

#endif //__BINARYIMPORT_H_
