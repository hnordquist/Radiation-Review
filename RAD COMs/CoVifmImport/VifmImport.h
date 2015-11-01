// VifmImport.h : Declaration of the CVifmImport

#ifndef __VIFMIMPORT_H_
#define __VIFMIMPORT_H_

#include "resource.h"       // main symbols
#include "MyDateTime.h"

/////////////////////////////////////////////////////////////////////////////
// CVifmImport
class ATL_NO_VTABLE CVifmImport : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVifmImport, &CLSID_VifmImport>,
	public IDispatchImpl<IVifmImport, &IID_IVifmImport, &LIBID_COVIFMIMPORTLib>
{
	CMyDateTime m_MyTime;

public:
	CVifmImport()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VIFMIMPORT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVifmImport)
	COM_INTERFACE_ENTRY(IVifmImport)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVifmImport
public:
	public:
	STDMETHOD(AddData)(/*[in]*/ short sFacilityID, /*[in]*/ BSTR bstrFilename, /*[in]*/ VARIANT_BOOL bOverwrite, /*[out]*/ BSTR* bstrResult, /*[out,retval]*/ VARIANT_BOOL* pbResult);

	STDMETHOD(BackupDataStore)(/*[in]*/ short sFacilityID, /*[in]*/BSTR bstrDestinationPath, /*[in]*/ VARIANT_BOOL bQuestion, /*[in]*/ VARIANT_BOOL bDefault, /*[out,retval]*/ VARIANT_BOOL* pbResult);

	STDMETHOD(CloseDataStore)();

	STDMETHOD(CreateNewDataStore)(/*[in]*/ short sFacilityID, /*[out,retval]*/ VARIANT_BOOL* pbResult);

	STDMETHOD(GetDataByChannel)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID, /*[in]*/ short sChannelID, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[in, out]*/ VARIANT *GrandVarStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);

	STDMETHOD(GetDataByStation)(/*[in]*/ short sFacilityID, /*[in]*/ short sStationID,  /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[in, out]*/ VARIANT *GrandStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);

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

	STDMETHOD(SetParameters)(/*[in]*/ short sFacilityID, /*[in]*/ VARIANT* ParmsVarStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);

};

#endif //__VIFMIMPORT_H_
