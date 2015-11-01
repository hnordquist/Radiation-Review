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
