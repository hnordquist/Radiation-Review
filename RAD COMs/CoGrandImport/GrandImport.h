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
// GrandImport.h : Declaration of the CGrandImport

#ifndef __GRANDIMPORT_H_
#define __GRANDIMPORT_H_

#include "resource.h" // main symbols
#include "ErrorProcessing.h"
#include "GrandFacCfgWrap.h"
#include "GrandDbVista.h"
#include "DirUtilities.h"

//SCR00240  Changed short stationID to long.  pjm 1-31-2006
//SCR00237: Add four methods to COM interface to support Rokkasho/NDAR requirements
//          DeleteData, DeleteDataStore, GetDataStoreStatus, GetLastError
/////////////////////////////////////////////////////////////////////////////
// CGrandImport
class ATL_NO_VTABLE CGrandImport : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGrandImport, &CLSID_GrandImport>,
	public IDispatchImpl<IGrandImport, &IID_IGrandImport, &LIBID_COGRANDIMPORTLib>
{
public:
	CGrandImport();
	virtual ~CGrandImport();

private:
	CErrorProcessing *m_perr;
	CString m_csDBType;
	CString m_strFacilityName;
	COleDateTime m_StartProcess;

	//Scratch CStrings
	CString m_cs;
	CString m_csFilename;
	CString m_csResultFromDb; 
	CString m_csError;
	CString m_csDestPath;
	CString m_csEmptyDBPath;
	CString m_csDataStorePath;
	CString m_csout;
	CString m_csDBError;

	/*const*/ CString m_csNULL;
	const CString m_csColon;
	CString m_csGetDataStatistics;//"GetDataStatistics()";

	short m_sStartUpTimeMillisec;

	CDirUtilities		m_Dir;
	CGrandFacCfgWrap	m_FacCfg;
	CGrandDbVista		m_Db;

	bool m_bQuietMode;
	bool m_bDisableLogger;
	short m_sVerbosityLevel;

	//void GetDatabaseErrorText(int ginst_miErrorNum, CString& csDBError);
	//void InitializeErrorProcessing(bool bDisableLogger);
	void InitializeErrorProcessing();

	long GetNumberOfPointsToRequest(
		short sFac, 
		long lSta, 
		DATE start, 
		DATE end, 
		long asked_for);

	inline bool TestPointer(void *p)
	{
		if (p == NULL)
			return false;

		bool b = true;
		try
		{
			BYTE byte = *((BYTE*)p);	//read
			*((BYTE*)p) = byte;			//write
		}
		catch (...)
		{
			b = false;
		}
		return b;
	}


public:

DECLARE_REGISTRY_RESOURCEID(IDR_GRANDIMPORT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGrandImport)
	COM_INTERFACE_ENTRY(IGrandImport)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IGrandImport
public:
	//1
	STDMETHOD(AddData)(/*[in]*/ short sFacilityID, /*[in]*/ BSTR bstrFilename, /*[in]*/ VARIANT_BOOL bOverwrite, /*[out]*/ BSTR* bstrResult, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	//2
	STDMETHOD(BackupDataStore)(/*[in]*/ short sFacilityID, /*[in]*/BSTR bstrDestinationPath, /*[in]*/ VARIANT_BOOL bQuestion, /*[in]*/ VARIANT_BOOL bDefault, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	//3
	STDMETHOD(CloseDataStore)();
	//4
	STDMETHOD(CreateNewDataStore)(/*[in]*/ short sFacilityID, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	//5
	STDMETHOD(DeleteData)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/  DATE dtStartDate,/*[in]*/ DATE dtEndDate,  /*[out,retval]*/  VARIANT_BOOL* pbStatus);
	//6
	STDMETHOD(DeleteDataStore)(/*[in]*/ short sFacilityID, /*[out, retval]*/ VARIANT_BOOL* pvbStatus);
	//7
	STDMETHOD(GetDataByChannel)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ short sChannelID, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ VARIANT *GrandVarStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	//8
	STDMETHOD(GetDataByStation)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID,  /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ VARIANT *GrandStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	//9
	STDMETHOD(GetDataStatistics)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID,  /*[in]*/ short sChannelID,/*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ DATE* pActualStartTime, /*[out]*/ DATE* pActualEndTime, /*[out]*/ long* plNumMembers, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	//10
	STDMETHOD(GetDataStorePath)(/*[in]*/ short sFacilityID, /*[out, retval]*/ BSTR* bstrStorePath);
	//11
	STDMETHOD(GetDataStoreSize)(/*[in]*/ short sFacilityID, /*[out]*/ double* pdSizeInBytes, /*[out, retval]*/ VARIANT_BOOL* pbResult);
	//12
	STDMETHOD(GetDataStoreStatus)(/*[in]*/short sFacilityID, /*[out, retval]*/ long* plStatus);
	//13
	STDMETHOD(GetDataStoreTimeRange)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ VARIANT_BOOL bErrDisplay, /*[out]*/ DATE* FirstTimestamp, /*[out]*/DATE* LastTimestamp, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	//14
	STDMETHOD(GetDayStatistics)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ DATE* pActualStartTime, /*[out]*/ DATE* pActualEndTime, /*[out]*/ long* plNumMembers, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	//15
	STDMETHOD(GetDaySummaryData)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime,  /*[out]*/ VARIANT* DayVariantArray, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	//16
	STDMETHOD(GetFilesList)(/*[in]*/ short sFacilityID, /*[in]*/ BSTR bstrRootDir, /*[in]*/ VARIANT_BOOL bIncludeSubDirs, /*[in]*/ VARIANT_BOOL bMostRecent, /*[out]*/ VARIANT* psaFilesList);
	//17
	STDMETHOD(GetLastError)(/*[in]*/short sFacilityID, /*[in, out]*/ BSTR *pbstrErrortext, /*[out, retval]*/ long* plError);
	//18
	STDMETHOD(GetParameters)(/*[in]*/ short sFacilityID, /*[out]*/ VARIANT* pvParms, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	//19
	STDMETHOD(InitializeDataStore)(/*[in]*/ short sFacilityID, /*[in]*/ VARIANT_BOOL bQuestion, /*[out, retval]*/ VARIANT_BOOL* pbSuccess);
	//20
	STDMETHOD(OpenDataStore)(/*[in]*/ short sFacilityID, /*[out, retval]*/ VARIANT_BOOL* pbSuccess);
	//21
	STDMETHOD(RestoreDataStore)(/*[in]*/ short sFacilityID, /*[in]*/ BSTR bstrSourcePath, /*[in]*/ VARIANT_BOOL bQuestion, /*[in]*/ VARIANT_BOOL bDefault, /*[out, retval]*/ VARIANT_BOOL* pbResult);
	//22
	STDMETHOD(SetParameters)(/*[in]*/ short sFacilityID, /*[in]*/ VARIANT ParmsVarStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);
};

#endif //__GRANDIMPORT_H_
