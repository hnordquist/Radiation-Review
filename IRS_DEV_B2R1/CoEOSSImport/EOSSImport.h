// EOSSImport.h : Declaration of the CEOSSImport

#ifndef __EOSSIMPORT_H_
#define __EOSSIMPORT_H_

#include "resource.h" // main symbols
#include "imports.h"
#include "AccessEOSSDb.h"
#include "DirUtilities.h"


/////////////////////////////////////////////////////////////////////////////
// CEOSSImport
class ATL_NO_VTABLE CEOSSImport : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEOSSImport, &CLSID_EOSSImport>,
	public IDispatchImpl<IEOSSImport, &IID_IEOSSImport, &LIBID_COEOSSIMPORTLib>
{
public:
	CEOSSImport();
	virtual ~CEOSSImport();
	IFacilityMgrDataPtr g_pIFacMgr;
	IIRSLoggerPtr m_pILogger;
private:
	short m_sFacID;
	
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

	short m_sStartUpTimeMillisec;
	CString m_csErrorString;
	long m_lErrID;
	long m_lErrSev;
	CString m_csErrorINIFile;

	CEOSSDbAccess*		m_pDb;

	bool m_bStartupCalled;
	bool m_bDatabaseOpen;
	bool m_bQuietMode;
	bool m_bDisableLogger;
	short m_sVerbosityLevel;

	void InitLogger ();
	void SetError (CString err, long lErrID);
	void ClearError(void);
	CString FormatMessage(CString arg, long lMsgID);
	void LogMsg (const CString& msgArgs, long lMessageID);

	//SealIDs are indexed in FacMgr, build this to make sure that we can look up the data
	// in the db.
	short* m_pChannelLookupTable;
	long m_lNumDefinedStations;
	long* m_pStationLookupTable;
	long m_lNumDefinedChannels;
	
	//Default parameters to use in case facility is switched and SetParameters is not called
	VARIANT varParms;

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


DECLARE_REGISTRY_RESOURCEID(IDR_EOSSIMPORT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CEOSSImport)
	COM_INTERFACE_ENTRY(IEOSSImport)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IEOSSImport
public:
	//1
	STDMETHOD(AddData)(/*[in]*/ short sFacilityID, /*[in]*/ BSTR bstrFilename, /*[in]*/ VARIANT_BOOL bOverwrite, /*[out]*/ BSTR* bstrResult, /*[out,retval]*/ VARIANT_BOOL* pvbResult);
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
	STDMETHOD(GetDataByChannel)
		(/*[in]*/ short sFacilityID, 
		/*[in]*/ long lStationID, 
		/*[in]*/ short sChannelID, 
		/*[in]*/ DATE StartTime, 
		/*[in]*/ DATE EndTime, 
		/*[out]*/ VARIANT *pvData, 
		/*[out,retval]*/ VARIANT_BOOL* pbResult);

	//STDMETHOD(GetDataByChannel)(/*[in]*/ short sFacilityID, /*[in]*/ long lStationID, /*[in]*/ short sChannelID, /*[in]*/ DATE StartTime, /*[in]*/ DATE EndTime, /*[out]*/ VARIANT *EOSSVarStruct, /*[out,retval]*/ VARIANT_BOOL* pbResult);
	//8
	STDMETHOD(GetDataByStation)
		(/*[in]*/ short sFacilityID, 
		/*[in]*/ long lStationID,  
		/*[in]*/ DATE StartTime, 
		/*[in]*/ DATE EndTime, 
		/*[out]*/ VARIANT *GrandStruct, 
		/*[out,retval]*/ VARIANT_BOOL* pbResult);
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
	STDMETHOD(SetParameters)(/*[in]*/ short sFacilityID, /*[in]*/ VARIANT ParmsVarStruct, /*[out,retval]*/ VARIANT_BOOL* pvbResult);
};

#endif //__EOSSIMPORT_H_
