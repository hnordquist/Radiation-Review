// TimeAlign.cpp : Implementation of CTimeAlign
#include "stdafx.h"
#include "CoLinearTimeAlign.h"
#include "TimeAlign.h"
#include "TimeAlignIni.h"
#include "DirUtilities.h"
#include "SafeArrayUtil.h"
#include "TimeAdjust.h"
#include "TypeDefinitions.h"

// HN 10/25/2007 move this to the .h file, we want these to be member variables.
// Remove constant instantiation/destruction of COMs, just create/destroy them once.
// HN 10/24/2007 This version needs to be built w/E10R4 COMS for now.  Change file locations.
//#import "..\CoGrandImport\CoGrandImport.tlb" no_namespace named_guids
// HN 10/24/2007 Change for BL2R1.  Build with NDAR COMS
//#import "c:\irs.e10r4\comlibs\CoGrandImport.dll" no_namespace named_guids
//#import "c:\irs.ndar\comlibs\CoGrandImport.dll" no_namespace named_guids
//#import "..\CoVifmImport\CoVifmImport.tlb" no_namespace named_guids
//#import "c:\irs.e10r4\comlibs\CoVifmImport.dll" no_namespace named_guids
//#import "..\CoBinaryImport\CoBinaryImport.tlb" no_namespace named_guids
//#import "c:\irs.e10r4\comlibs\CoBinaryImport.dll" no_namespace named_guids

// Change POST E10R4 was done here to account for SR and MCA types, which did not
// have an associated COM at the time of E10R4 release.  This will need to be added back for BL3
// HN 10/24/07
// #import "c:\irs.b3\comlibs\CoSrImport.dll" no_namespace named_guids
// #import "c:\irs.b3\comlibs\CoMcaImport.dll" no_namespace named_guids
/////////////////////////////////////////////////////////////////////////////
// CTimeAlign

CTimeAlignIni IniFile;
// HN 10/25/2007 move these and make them member variables.
//IGrandImportPtr pIGrandImport;
//IVifmImportPtr pIVifmImport;
//IBinaryImportPtr pIBinaryImport;

CTimeAlign::CTimeAlign()
{
	try
	{
		m_pIGrandImport.CreateInstance (CLSID_GrandImport);
		m_pIVifmImport.CreateInstance (CLSID_VifmImport);

		m_bDisableLogging = true;
		m_nLoggingVerbosity = 3;
		m_bQuietMode = false;
		m_lFlagsForLibraries = 0;
	}
	catch (...)
	{
		ASSERT(FALSE);
	}
}

CTimeAlign::~CTimeAlign ()
{
	m_pIGrandImport = NULL;
	m_pIVifmImport = NULL;
}

//	Adjust the given array of timestamps -- this is probably not used
// 05-Nov-2004	SFK	Added check to set data store path
// As far as I can tell, this change was done for BL2R0.  There is no note as to when the method signature
// was changed, but it appears that the "station type" appeared early in the game.  HN 10/24/2007
//STDMETHODIMP CTimeAlign::AdjustTimestamps(short sFacilityID, short sStationID, long lNumItems, DATE *pOriginalTimestamps, DATE *pAdjustedTimestamps, VARIANT_BOOL *pbResult)
// 10/24/07 HN Change for BL2R1, all station IDs are longs now.
// 10/25/07 HN Change all VARIANT_BOOL returns to actual VARIANT_BOOLs, not just bools
//STDMETHODIMP CTimeAlign::AdjustTimestamps(short sFacilityID, short sStationID, short sStationType, long lNumItems, DATE *pOriginalTimestamps, DATE *pAdjustedTimestamps, VARIANT_BOOL *pbResult)
STDMETHODIMP CTimeAlign::AdjustTimestamps(
	short sFacilityID, 
	long lStationID, 
	short sStationType, 
	long lNumItems, 
	DATE *pOriginalTimestamps, 
	DATE *pAdjustedTimestamps, 
	VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!IniFile.m_bOpen) IniFile.SetDataStorePath(sFacilityID);

//	CTimeAdjust Adjust;
//	bool bSuccess = Adjust.AdjustTimestamps(sFacilityID, sStationID, lNumItems, pOriginalTimestamps, pAdjustedTimestamps);
	// This had been commented out in BL1 version.  I believe it was reentered in BL2R0, now with Station Type
	// HN 10/24/2007
	CTimeAdjust Adjust;
	//bool bSuccess = Adjust.AdjustTimestamps(sFacilityID, sStationID, sStationType, lNumItems, pOriginalTimestamps, pAdjustedTimestamps);
	bool bSuccess = Adjust.AdjustTimestamps(
		sFacilityID, 
		lStationID, 
		sStationType, 
		lNumItems, 
		pOriginalTimestamps, 
		pAdjustedTimestamps);

	// The pbResult had been false in the earlier version (HUH?), now is bSuccess
	// This returns a bool instead of a VARIANT_BOOL, but we can change that in BL3, because Rad is
	// still looking for the bool-like answer.  HN 10/24/07
	*pbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}


//	Common COM function to backup any COM specific files to a designated location
// 26-Oct-2004	SFK	Added sFacility ID implementation and creating facility path
// 05-Nov-2004	SFK	Added check to set data store path
STDMETHODIMP CTimeAlign::BackupDataStore(short sFacilityID, BSTR bstrDestinationPath, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!IniFile.m_bOpen) 
		IniFile.SetDataStorePath(sFacilityID);

	CString strDestPath(bstrDestinationPath);//Remove COM conversion macros

	bool bSuccess = IniFile.CopyDataStore(
		IniFile.m_strDataStorePath, strDestPath, false);

	*pbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;//Use bool and VARIANT_BOOL precisely

	return S_OK;
}


//	Common COM function to do any special actions to close the data store
STDMETHODIMP CTimeAlign::CloseDataStore()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Performs any special closing actions (typically for databases)
	// no action required for the TimeAlign module

	return S_OK;
}


//	Gets the data for the given channel for the given interval by calling appropriate other COM.
//	Adjusts any timestamps.  Storage is done by caller.
//	26-Oct-2004	SFK	Changed to support new import interface spec.  Major changes
// 05-Nov-2004	SFK	Added check to set data store path
// 10/24/07 HN Change all short station IDs to longs for BL2R1
//STDMETHODIMP CTimeAlign::GetDataByChannel(short sFacilityID, short sStationID, short sStationType,  short sChannelID, DATE StartTime, DATE EndTime, VARIANT* pvData, VARIANT_BOOL *pbResult)
STDMETHODIMP CTimeAlign::GetDataByChannel(
	short sFacilityID, long lStationID, short sStationType,  
	short sChannelID, DATE StartTime, DATE EndTime, VARIANT* pvData, 
	VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CTimeAdjust Adjust;
	DATE dUnadjustedStart, dUnadjustedEnd;
	DATE *pdUnadjustedTimestamps = NULL;
	bool bStat = true;

	if (!IniFile.m_bOpen) IniFile.SetDataStorePath(sFacilityID);

	// The calling times in StartTime and EndTime are "adjusted" times;
	//	unadjust them before calling data com
	//if (bStatus) bStatus = Adjust.UnadjustTimestamps(sFacilityID, sStationID, sStationType,(long)1, &StartTime, &dUnadjustedStart); 
	//if (bStatus) bStatus = Adjust.UnadjustTimestamps(sFacilityID, sStationID, sStationType,(long)1, &EndTime, &dUnadjustedEnd); 
	if (bStat) 
		bStat = Adjust.UnadjustTimestamps(sFacilityID, lStationID, sStationType,(long)1, &StartTime, &dUnadjustedStart); 

	if (bStat) 
		bStat = Adjust.UnadjustTimestamps(sFacilityID, lStationID, sStationType,(long)1, &EndTime, &dUnadjustedEnd); 

	if (bStat)
	{
		VARIANT_BOOL vb = VARIANT_FALSE;

		switch(sStationType)
		{
			//B2R1 - the Grand Import is an NDAR component, so uses a long station ID.
			case GRAND_TYPE:
				if (m_pIGrandImport)
					vb = m_pIGrandImport->GetDataByChannel(sFacilityID, lStationID, sChannelID, dUnadjustedStart, dUnadjustedEnd, pvData);
				break;

			//B2R1 - the VIFM and Binary imports are NOT NDAR components, so use a short station ID.
			case VIFM_TYPE:
				if (m_pIVifmImport)
					vb = m_pIVifmImport->GetDataByChannel(sFacilityID, (short) lStationID, sChannelID, dUnadjustedStart, dUnadjustedEnd, pvData);
				break;

			//case MCA_TYPE:
			//case SR_TYPE:
		}

		bStat = (vb != VARIANT_FALSE);

		//add other COMs here

		// now unadjust all the timestamps in the returned results
		if (bStat)
		{
			long lNumPts = 0;

			if (pvData->vt == (VT_VARIANT | VT_ARRAY)) 
				SafeArrayGetUBound(pvData->parray, 1, &lNumPts);

			if (lNumPts > 0) 
				bStat = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType, pvData);

		}
	}//if bStat

	*pbResult = bStat?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}


//	Gets the data for the given station for the given interval by calling appropriate other COM.
//	Adjusts any timestamps.  SAFEARRAY is allocated by COM.  Storage is done by caller.
//	26-Oct-2004	SFK	Changed to support new import interface spec.  Major changes
// 05-Nov-2004	SFK	Added check to set data store path
// 10/24/07 HN Changed for BL2R1 to accept a long station ID.
//STDMETHODIMP CTimeAlign::GetDataByStation(short sFacilityID, short sStationID, short sStationType,  DATE StartTime, DATE EndTime, VARIANT* pvData, VARIANT_BOOL *pbResult)
STDMETHODIMP CTimeAlign::GetDataByStation(
	short sFacilityID, 
	long lStationID, 
	short sStationType,  
	DATE StartTime, 
	DATE EndTime, 
	VARIANT* pvData, 
	VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CTimeAdjust Adjust;
	DATE dUnadjustedStart, dUnadjustedEnd;
	bool bStat = true;

	if (!IniFile.m_bOpen) 
		IniFile.SetDataStorePath(sFacilityID);

	// The calling times in Start and End are "adjusted" times; unadjust them before calling data com
//	if (bStatus) bStatus = Adjust.UnadjustTimestamps(sFacilityID, sStationID, sStationType,(long)1, &StartTime, &dUnadjustedStart); 
//	if (bStatus) bStatus = Adjust.UnadjustTimestamps(sFacilityID, sStationID, sStationType,(long)1, &EndTime, &dUnadjustedEnd); 
	if (bStat) 
		bStat = Adjust.UnadjustTimestamps(sFacilityID, lStationID, sStationType,1L, &StartTime, &dUnadjustedStart); 
	if (bStat) 
		bStat = Adjust.UnadjustTimestamps(sFacilityID, lStationID, sStationType,1L, &EndTime, &dUnadjustedEnd); 
	/*
	// call the appropriate COM to get the data results in "unadjusted times"
	if (bStatus && (sStationType == GRAND_TYPE)) 
	{
		//pIGrandImport.CreateInstance(CLSID_GrandImport);
		// 10/24/07 HN Change to call new Grand Import w/long station ID for BL2R1
		//bStatus = pIGrandImport->GetDataByStation (sFacilityID, sStationID, dUnadjustedStart, dUnadjustedEnd, pvData);
		vb = VARIANT_FALSE;
		if (m_pIGrandImport)
			vb = m_pIGrandImport->GetDataByStation (sFacilityID, lStationID, dUnadjustedStart, dUnadjustedEnd, pvData);
		//pIGrandImport = NULL;
		bStatus = (vb != VARIANT_FALSE);
	}
	// 10/24/07 HN Leave these as shorts, we haven't modified Vifm and Binary yet.
	if (bStatus && (sStationType == VIFM_TYPE)) 
	{
		//pIVifmImport.CreateInstance(CLSID_VifmImport);
		//bStatus = pIVifmImport->GetDataByStation (sFacilityID, sStationID, dUnadjustedStart, dUnadjustedEnd, pvData);
		vb = VARIANT_FALSE;
		if (m_pIVifmImport)
			vb = m_pIVifmImport->GetDataByStation (sFacilityID, (short) lStationID, dUnadjustedStart, dUnadjustedEnd, pvData);
		//pIVifmImport = NULL;
		bStatus = (vb != VARIANT_FALSE);
	}
	if (bStatus && (sStationType == BINARY_TYPE)) 
	{
		//pIBinaryImport.CreateInstance(CLSID_BinaryImport);
		//bStatus = pIBinaryImport->GetDataByStation (sFacilityID, sStationID, dUnadjustedStart, dUnadjustedEnd, pvData);
		vb = VARIANT_FALSE;
		if (m_pIBinaryImport)
			vb = m_pIBinaryImport->GetDataByStation (sFacilityID, (short) lStationID, dUnadjustedStart, dUnadjustedEnd, pvData);
		//pIBinaryImport = NULL;
		bStatus = (vb != VARIANT_FALSE);
	}
	// Another POST E10R4 change.  Add back in BL3 HN 10/24/07
	if ((bStatus) && (sStationType == SR_TYPE)) {
		pISrImport.CreateInstance(CLSID_SrImport);
		bStatus = pISrImport->GetDataByStation (sFacilityID, sStationID, dUnadjustedStart, dUnadjustedEnd, pvData);
		pISrImport = NULL;
	}
	if ((bStatus) && (sStationType == MCA_TYPE)) {
		pIMcaImport.CreateInstance(CLSID_McaImport);
		bStatus = pIMcaImport->GetDataByStation (sFacilityID, sStationID, dUnadjustedStart, dUnadjustedEnd, pvData);
		pIMcaImport = NULL;
	}*/

	VARIANT_BOOL vb = VARIANT_FALSE;

	if (bStat)
	{
		switch(sStationType)
		{
		case GRAND_TYPE:
			if (m_pIGrandImport)
				vb = m_pIGrandImport->GetDataByStation (sFacilityID, lStationID, dUnadjustedStart, dUnadjustedEnd, pvData);
			break;
		case VIFM_TYPE:
			if (m_pIVifmImport)
				vb = m_pIVifmImport->GetDataByStation (sFacilityID, (short) lStationID, dUnadjustedStart, dUnadjustedEnd, pvData);
			break;

		//case MCA_TYPE:
		//case SR_TYPE:
		}
	}
	bStat = (vb != VARIANT_FALSE);

	//add other COMs here

	if (bStat)
	{
		// now unadjust all the timestamps in the returned results
		long lNumPts = 0;

		if (pvData->vt == (VT_VARIANT | VT_ARRAY)) 
			SafeArrayGetUBound(pvData->parray, 1, &lNumPts);

		if (bStat && (lNumPts > 0)) 
			bStat = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType, pvData);
	}

	*pbResult = bStat?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}


//	Gets the amount of data in the given interval for the given station via calls to the appropriate COM
// 05-Nov-2004	SFK	Added check to set data store path
//STDMETHODIMP CTimeAlign::GetDataStatistics(short sFacilityID, short sStationID, short sChanID, short sStationType, DATE StartTime, DATE EndTime, DATE *pActualStartTime, DATE *pActualEndTime, long *plNumMembers, VARIANT_BOOL *pbResult)
// 10/24/07 HN Change all short station IDs to longs.
STDMETHODIMP CTimeAlign::GetDataStatistics(
	short sFacilityID, 
	long lStationID, 
	short sChanID, 
	short sStationType, 
	DATE StartTime, 
	DATE EndTime, 
	DATE *pActualStartTime, 
	DATE *pActualEndTime, 
	long *plNumMembers, 
	VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CTimeAdjust Adjust;
	DATE dUnadjustedStart, dUnadjustedEnd, dUnadjustedRealStart, dUnadjustedRealEnd;
	long lNumPts = 0;
	bool bStat = true;

	*pActualStartTime = 0;
	*pActualEndTime = 0;
	*plNumMembers = 0;

	if (!IniFile.m_bOpen) 
		IniFile.SetDataStorePath(sFacilityID);

	// The calling times in StartTime and EndTime are "adjusted" times; 
	// unadjust them before calling appropriate data com
	//bStatus = Adjust.UnadjustTimestamps(sFacilityID, sStationID, sStationType, (long)1, &StartTime, &dUnadjustedStart); 
	//if (bStatus) bStatus = Adjust.UnadjustTimestamps(sFacilityID, sStationID, sStationType, (long)1, &EndTime, &dUnadjustedEnd); 
	if (bStat)
		bStat = Adjust.UnadjustTimestamps(sFacilityID, lStationID, sStationType, (long)1, &StartTime, &dUnadjustedStart); 
	if (bStat) 
		bStat = Adjust.UnadjustTimestamps(sFacilityID, lStationID, sStationType, (long)1, &EndTime, &dUnadjustedEnd); 

	VARIANT_BOOL vb = VARIANT_FALSE;

	if (bStat)
	{
		BSTR pErr = NULL;
		switch(sStationType)
		{
			case GRAND_TYPE:
				if (m_pIGrandImport)
				{
					vb = m_pIGrandImport->GetDataStatistics(sFacilityID, lStationID, sChanID, dUnadjustedStart, dUnadjustedEnd, &dUnadjustedRealStart, &dUnadjustedRealEnd, &lNumPts);
					//if (!vb)
					//{
					//	m_pIGrandImport->GetLastError(sFacilityID, &pErr);
					//}
				}
				break;

			case VIFM_TYPE:
				// 10/24/07 HN Leave VIFM and BINARY as shorts for the moment.
				if (m_pIVifmImport)
				{
					vb = m_pIVifmImport->GetDataStatistics(sFacilityID, (short)lStationID, sChanID, dUnadjustedStart, dUnadjustedEnd, &dUnadjustedRealStart, &dUnadjustedRealEnd, &lNumPts);
				}
				break;

			//case MCA_TYPE:
			//case SR_TYPE:
		}

		bStat = (vb != VARIANT_FALSE);

		if (bStat && (lNumPts > 0)) 
		{
			bStat = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType,(long)1, &dUnadjustedRealStart, pActualStartTime); 
			if (bStat) 
				bStat = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType,(long)1, &dUnadjustedRealEnd, pActualEndTime); 
		}
	}

	*plNumMembers = lNumPts;
	*pbResult = bStat?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}


/*
	// call the appropriate COM to get the data results in "unadjusted times"
	if (bStatus && (sStationType == GRAND_TYPE)) 
	{
		//pIGrandImport.CreateInstance(CLSID_GrandImport);
		//bStatus = pIGrandImport->GetDataStatistics(sFacilityID, sStationID, sChanID, dUnadjustedStart, dUnadjustedEnd, &dUnadjustedRealStart, &dUnadjustedRealEnd, &lNumPts);
		vb = VARIANT_FALSE;
		if (m_pIGrandImport)
			vb = m_pIGrandImport->GetDataStatistics(sFacilityID, lStationID, sChanID, dUnadjustedStart, dUnadjustedEnd, &dUnadjustedRealStart, &dUnadjustedRealEnd, &lNumPts);
		//pIGrandImport = NULL;
		bStatus = (vb != VARIANT_FALSE);
	}
	// 10/24/07 HN Leave these as shorts for the moment.
	else if (bStatus && (sStationType == VIFM_TYPE)) 
	{
		//pIVifmImport.CreateInstance(CLSID_VifmImport);
		//bStatus = pIVifmImport->GetDataStatistics(sFacilityID, sStationID, sChanID, dUnadjustedStart, dUnadjustedEnd, &dUnadjustedRealStart, &dUnadjustedRealEnd, &lNumPts);
		vb = VARIANT_FALSE;
		if (m_pIVifmImport)
			vb = m_pIVifmImport->GetDataStatistics(sFacilityID, (short) lStationID, sChanID, dUnadjustedStart, dUnadjustedEnd, &dUnadjustedRealStart, &dUnadjustedRealEnd, &lNumPts);
		//pIVifmImport = NULL;
		bStatus = (vb != VARIANT_FALSE);
	}
	else if (bStatus && (sStationType == BINARY_TYPE)) 
	{
		//pIBinaryImport.CreateInstance(CLSID_BinaryImport);
		//bStatus = pIBinaryImport->GetDataStatistics(sFacilityID, sStationID, sChanID, dUnadjustedStart, dUnadjustedEnd, &dUnadjustedRealStart, &dUnadjustedRealEnd, &lNumPts);
		vb = VARIANT_FALSE;
		if (m_pIBinaryImport)
			vb = m_pIBinaryImport->GetDataStatistics(sFacilityID, (short) lStationID, sChanID, dUnadjustedStart, dUnadjustedEnd, &dUnadjustedRealStart, &dUnadjustedRealEnd, &lNumPts);
		//pIBinaryImport = NULL;
		bStatus = (vb != VARIANT_FALSE);
	}
	// Again, this does not belong in version 1.0.1.1 HN 10/24/07 Add in BL3
		if ((bStatus) && (sStationType == SR_TYPE)) {
		pISrImport.CreateInstance(CLSID_SrImport);
		bStatus = pISrImport->GetDataStatistics(sFacilityID, sStationID, sChanID, dUnadjustedStart, dUnadjustedEnd, &dUnadjustedRealStart, &dUnadjustedRealEnd, &lNumPts);
		pISrImport = NULL;
	}
	if ((bStatus) && (sStationType == MCA_TYPE)) {
		pIMcaImport.CreateInstance(CLSID_McaImport);
		bStatus = pIMcaImport->GetDataStatistics(sFacilityID, sStationID, sChanID, dUnadjustedStart, dUnadjustedEnd, &dUnadjustedRealStart, &dUnadjustedRealEnd, &lNumPts);
		pIMcaImport = NULL;
	}

	// add logic for other COMs here

	// The data com returned "unadjusted" times, adjust them in the returned information
	if (bStatus && (lNumPts > 0)) 
	{
		//bStatus = Adjust.AdjustTimestamps(sFacilityID, sStationID, sStationType,(long)1, &dUnadjustedRealStart, pActualStartTime); 
		//if (bStatus) bStatus = Adjust.AdjustTimestamps(sFacilityID, sStationID, sStationType,(long)1, &dUnadjustedRealEnd, pActualEndTime); 
		bStatus = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType,(long)1, &dUnadjustedRealStart, pActualStartTime)?VARIANT_TRUE:VARIANT_FALSE; 
		if (bStatus) 
			bStatus = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType,(long)1, &dUnadjustedRealEnd, pActualEndTime)?VARIANT_TRUE:VARIANT_FALSE; 
	}

	*plNumMembers = lNumPts;c
	*pbResult = bStatus?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}
*/

//	Common COM function to get the current data store path
// 05-Nov-2004	SFK	Added check to set data store path
STDMETHODIMP CTimeAlign::GetDataStorePath(short sFacilityID, BSTR *bstrStorePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Returns the path where the data store (ini file) is located.
	//USES_CONVERSION;

	if (!IniFile.m_bOpen) 
		IniFile.SetDataStorePath(sFacilityID);

	*bstrStorePath = IniFile.m_strDataStorePath.AllocSysString();

	return S_OK;
}


//	Get the time of the beginning and ending of the data store for the given station by calling appropriate COM
//	Adjusts any timestamps.
// 05-Nov-2004	SFK	Added check to set data store path
//STDMETHODIMP CTimeAlign::GetDataStoreTimeRange(short sFacilityID, short sStationID, short sStationType,  VARIANT_BOOL bErrDisplay, DATE *FirstTimestamp, DATE *LastTimestamp, VARIANT_BOOL *pbResult)
// 10/24/07 HN Change all short station IDs to longs for BL2R1
STDMETHODIMP CTimeAlign::GetDataStoreTimeRange(
	short sFacilityID, 
	long lStationID, 
	short sStationType,  
	VARIANT_BOOL vbErrDisplay, 
	DATE *FirstTimestamp, 
	DATE *LastTimestamp, 
	VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CTimeAdjust Adjust;
	DATE dStartDate, dEndDate;
	DATE dAdjustedStartDate = 10000000.0;
	DATE dAdjustedEndDate = 0.0;
	VARIANT_BOOL vbResult = VARIANT_FALSE;
	bool bStatus = false;
	VARIANT_BOOL vb;

	if (!IniFile.m_bOpen) 
		IniFile.SetDataStorePath(sFacilityID);
	
	if (lStationID == -1) 	// if station is -1, then get the earliest time of any station in any COM with a station in facility	
	{
		DATE dStart, dEnd, dAdjustedStart, dAdjustedEnd;

		vb = VARIANT_FALSE;
		if (m_pIGrandImport)
			vb = m_pIGrandImport->GetDataStoreTimeRange(sFacilityID, lStationID, vbErrDisplay, &dStart, &dEnd);
		bStatus = (vb != VARIANT_FALSE);

		if (dStart != 0)
		{
			if (bStatus) bStatus = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType,1L, &dStart, &dAdjustedStart);
			if (bStatus) bStatus = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType,1L, &dEnd, &dAdjustedEnd);
			if (bStatus) 
			{
				if (dAdjustedStart < dAdjustedStartDate) 
					dAdjustedStartDate = dAdjustedStart;
				if (dAdjustedEnd > dAdjustedEndDate) 
					dAdjustedEndDate = dAdjustedEnd;
				vbResult = VARIANT_TRUE;
			}
		}
		// 10/24/07 HN Leave this with short, has not been changed yet.
		vb = VARIANT_FALSE;
		if (m_pIVifmImport)
			vb = m_pIVifmImport->GetDataStoreTimeRange(sFacilityID, (short)lStationID, vbErrDisplay, &dStart, &dEnd);
		bStatus = (vb != VARIANT_FALSE);

		if (dStart != 0.0)
		{
			if (bStatus) bStatus = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType,1L, &dStart, &dAdjustedStart);
			if (bStatus) bStatus = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType,1L, &dEnd, &dAdjustedEnd);
			if (bStatus) 
			{
				if (dAdjustedStart < dAdjustedStartDate) 
					dAdjustedStartDate = dAdjustedStart;
				if (dAdjustedEnd > dAdjustedEndDate) 
					dAdjustedEndDate = dAdjustedEnd;
				vbResult = VARIANT_TRUE;
			}
		}

			// Add back for BL3, this was not in E10R4 HN 10/24/07
/*			pISrImport.CreateInstance(CLSID_SrImport);
			bStatus = pISrImport->GetDataStoreTimeRange(sFacilityID, sStationID, (VARIANT_BOOL)bErrDisplay, &dStart, &dEnd);
			if (bStatus) bStatus = Adjust.AdjustTimestamps(sFacilityID, sStationID, sStationType,(long)1, &dStart, &dAdjustedStart);
			if (bStatus) bStatus = Adjust.AdjustTimestamps(sFacilityID, sStationID, sStationType,(long)1, &dEnd, &dAdjustedEnd);
			pISrImport = NULL;
			if (bStatus) {
				if (dAdjustedStart < dAdjustedStartDate) dAdjustedStartDate = dAdjustedStart;
				if (dAdjustedEnd > dAdjustedEndDate) dAdjustedEndDate = dAdjustedEnd;
				bResult = true;
			}
			pIMcaImport.CreateInstance(CLSID_McaImport);
			bStatus = pIMcaImport->GetDataStoreTimeRange(sFacilityID, sStationID, (VARIANT_BOOL)bErrDisplay, &dStart, &dEnd);
			if (bStatus) bStatus = Adjust.AdjustTimestamps(sFacilityID, sStationID, sStationType,(long)1, &dStart, &dAdjustedStart);
			if (bStatus) bStatus = Adjust.AdjustTimestamps(sFacilityID, sStationID, sStationType,(long)1, &dEnd, &dAdjustedEnd);
			pIMcaImport = NULL;
			if (bStatus) {
				if (dAdjustedStart < dAdjustedStartDate) dAdjustedStartDate = dAdjustedStart;
				if (dAdjustedEnd > dAdjustedEndDate) dAdjustedEndDate = dAdjustedEnd;
				bResult = true;
			}
*/
			// add other com stuff here, add logic to figure out the earliest and latest time in any COM


	}
	else // is only one station, based on station type, get the information from the correct COM
	{	
		vb = VARIANT_FALSE;

		switch(sStationType)
		{
			case GRAND_TYPE:
				if (m_pIGrandImport)
					vb = m_pIGrandImport->GetDataStoreTimeRange(sFacilityID, lStationID, vbErrDisplay, &dStartDate, &dEndDate);
				break;
			case VIFM_TYPE:
				if (m_pIVifmImport)
					vb = m_pIVifmImport->GetDataStoreTimeRange(sFacilityID, (short)lStationID, vbErrDisplay, &dStartDate, &dEndDate);
				break;
			//case MCA_TYPE:
			//case SR_TYPE:
		}

		bStatus = (vb != VARIANT_FALSE);
		if (bStatus) bStatus = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType,(long)1, &dStartDate, &dAdjustedStartDate);
		if (bStatus) bStatus = Adjust.AdjustTimestamps(sFacilityID, lStationID, sStationType,(long)1, &dEndDate, &dAdjustedEndDate);
		if (bStatus) vbResult = VARIANT_TRUE;
	}

	if (vbResult == VARIANT_TRUE)
	{
		*FirstTimestamp = dAdjustedStartDate;
		*LastTimestamp = dAdjustedEndDate;
	}
	else
	{
		*FirstTimestamp = 0;
		*LastTimestamp = 0;
	}
	*pbResult = vbResult;
	return S_OK;
}


//	Gets day statistics for the specified time range by calling the appropriate COM.  Storage is allocated by COM.
//	NOTE:  No time adjustment is done on day data!!!
// 05-Nov-2004	SFK	Added check to set data store path
// 10/24/07 HN Change these methods to require long station IDs for BL2R1
// STDMETHODIMP CTimeAlign::GetDayStatistics(short sFacilityID, short sStationID, short sStationType,  DATE StartTime, DATE EndTime, DATE *pActualStartTime, DATE *pActualEndTime, long *plNumMembers, VARIANT_BOOL *pbResult)
STDMETHODIMP CTimeAlign::GetDayStatistics(
	short sFacilityID, 
	long lStationID, 
	short sStationType,  
	DATE StartTime, DATE EndTime, 
	DATE *pActualStartTime, 
	DATE *pActualEndTime, 
	long *plNumMembers, 
	VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pActualStartTime = 0;
	*pActualEndTime = 0;
	*plNumMembers = 0;

	long lNumPts = 0;
	VARIANT_BOOL vb = VARIANT_FALSE;

	if (!IniFile.m_bOpen)
		IniFile.SetDataStorePath(sFacilityID);

	switch(sStationType)
	{
		//B2R1 - the grand import is and NDAR component and uses long station ID
		case GRAND_TYPE:
			if (m_pIGrandImport)
				vb = m_pIGrandImport->GetDayStatistics(sFacilityID, lStationID, StartTime, EndTime, pActualStartTime, pActualEndTime, &lNumPts);
			break;

		//B2R1 - the vifm and binary imports are non-NDAR and use short station ID
		case VIFM_TYPE:
			if (m_pIVifmImport)
				vb = m_pIVifmImport->GetDayStatistics(sFacilityID, (short) lStationID, StartTime, EndTime, pActualStartTime, pActualEndTime, &lNumPts);
			break;

		//case MCA_TYPE:
		//case SR_TYPE:
	}

	*plNumMembers = lNumPts;
	*pbResult = vb;
	return S_OK;
}

//	Gets day data for the specified time range by calling the appropriate COM.  Storage is allocated by COM.
//	NOTE:  No time adjustment is done on day data!!!
//	28-Oct-2004	SFK  Removed all references to plNumMembers since you can get that from members in the SAFEARRAY
// 05-Nov-2004	SFK	Added check to set data store path
//STDMETHODIMP CTimeAlign::GetDaySummaryData(short sFacilityID, short sStationID, short sStationType,  DATE StartTime, DATE EndTime, VARIANT *DayVariantArray, VARIANT_BOOL *pbResult)
STDMETHODIMP CTimeAlign::GetDaySummaryData(
	short sFacilityID, 
	long lStationID, 
	short sStationType,  
	DATE StartTime, 
	DATE EndTime, 
	VARIANT *DayVariantArray, 
	VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	VARIANT_BOOL vb = VARIANT_FALSE;

	if (!IniFile.m_bOpen) 
		IniFile.SetDataStorePath(sFacilityID);

	switch (sStationType)
	{
		//B2R1 - the grand import is and NDAR component and uses long station ID
		case GRAND_TYPE:
			if (m_pIGrandImport)
				vb = m_pIGrandImport->GetDaySummaryData(sFacilityID, lStationID, StartTime, EndTime, DayVariantArray);
			break;

		//B2R1 - the vifm and binary imports are non-NDAR and use short station ID
		case VIFM_TYPE:
			if (m_pIVifmImport)
				vb = m_pIVifmImport->GetDaySummaryData(sFacilityID, (short) lStationID, StartTime, EndTime, DayVariantArray);
			break;

		//case MCA_TYPE:
		//case SR_TYPE:
	}

	*pbResult = vb;
	return S_OK;
}


//	Return the parameters that are currently being used by this COM
//	08-Dec-2003 SFK modified to add station type
// 05-Nov-2004	SFK	Added check to set data store path
STDMETHODIMP CTimeAlign::GetParameters(
	short sFacilityID, 
	VARIANT *ParmsVarStruct, 
	VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	struct LinearTA_Parms Parms;
	bool ok = false;

	if (!IniFile.m_bOpen) 
		IniFile.SetDataStorePath(sFacilityID);

	long j;
	long lNum = 0;

	if (ParmsVarStruct->vt == (VT_VARIANT | VT_ARRAY)) 
	{
		SafeArrayGetUBound(ParmsVarStruct->parray, 1, &lNum);

		for (j=0; j<=lNum; j++) 
		{
			ok = SA_GetElem(ParmsVarStruct->parray, j, 0, &Parms.sStaNum);		// station number
			if (ok) ok = SA_GetElem(ParmsVarStruct->parray, j, 1, &Parms.sStaType);		// station type

			// read the data for this channel from the ini file
			if (ok) ok = IniFile.GetTimeAlignParms(sFacilityID, Parms.sStaNum, Parms.sStaType, &Parms);

			bool bIsDate = true;
			if (ok) ok = SA_PutElem(ParmsVarStruct->parray, j, 0L, Parms.sStaNum);			// station number
			if (ok) ok = SA_PutElem(ParmsVarStruct->parray, j, 1L, Parms.sStaType);			// station type
			if (ok) ok = SA_PutElem(ParmsVarStruct->parray, j, 2L, Parms.Event1Time, bIsDate);		// event time 1
			if (ok) ok = SA_PutElem(ParmsVarStruct->parray, j, 3L, Parms.Event2Time, bIsDate);		// event time 2
			if (ok) ok = SA_PutElem(ParmsVarStruct->parray, j, 4L, Parms.dSlope);			// slope
			if (ok) ok = SA_PutElem(ParmsVarStruct->parray, j, 5L, Parms.dIntercept);		// intercept
			if (ok) ok = SA_PutElem(ParmsVarStruct->parray, j, 6L, Parms.bEnableTimeAlign);	// use time align??
			if (ok) ok = SA_PutElem(ParmsVarStruct->parray, j, 7L, Parms.bEnableStation);	// station used for time align?
			if (!ok)
				break;
		}
	}

	*pbResult = ok?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}


//	Common COM function to do any special action to initialize a data store
// 26-Oct-2004	SFK	Does this have meaning for Time Align???
// 05-Nov-2004	SFK	Added check to set data store path
STDMETHODIMP CTimeAlign::InitializeDataStore(short sFacilityID, VARIANT_BOOL bQuestion, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pbResult = VARIANT_TRUE;
	return S_OK;
}

// Common COM function to do any special action to open a data store
// 05-Nov-2004	SFK	Added check to set data store path
STDMETHODIMP CTimeAlign::OpenDataStore(short sFacilityID, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
	// Performs any special opening actions (typically for databases)

	IniFile.SetDataStorePath(sFacilityID);		// set the data store path always on an open
	if (IniFile.mpStaInfo) 
	{
		delete [] IniFile.mpStaInfo;
		IniFile.mpStaInfo = NULL;
	}

	*pbResult = VARIANT_TRUE;
	return S_OK;
}

//	Common COM function to restore any COM specific files from a designated location
// 05-Nov-2004	SFK	Added check to set data store path
STDMETHODIMP CTimeAlign::RestoreDataStore(short sFacilityID, BSTR bstrSourcePath, VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!IniFile.m_bOpen) 
		IniFile.SetDataStorePath(sFacilityID);

	// Copies the data store (ini file) from bstrSourcePath to m_strDataStorePath
	CString strSrcPath(bstrSourcePath);
	bool bSuccess = IniFile.CopyDataStore(strSrcPath, IniFile.m_strDataStorePath, false);
	*pbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}

//	Set the given parameters in the proper variables
//	08-Dec-2003 SFK modified to add station type
// 05-Nov-2004	SFK	Added check to set data store path
STDMETHODIMP CTimeAlign::SetParameters(
	short sFacilityID, 
	VARIANT* ParmsVarStruct, 
	VARIANT_BOOL* pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	long j;
	struct LinearTA_Parms Parms;
	bool bSuccess = false;
	CTimeAdjust NewParms;
	DATE BaseTime1, BaseTime2;
	bool bFirstUsedStation = false;
	long lNum = 0;
	short sStation = 0;

	if (!IniFile.m_bOpen) 
		IniFile.SetDataStorePath(sFacilityID);

	if (ParmsVarStruct->vt == (VT_VARIANT | VT_ARRAY)) 	// is the type of SafeArray we are expecting 
	{
		SafeArrayGetUBound(ParmsVarStruct->parray, 1, &lNum);

		for (j=0; j<=lNum; j++) 
		{
			// Temporary station number var in case we are retrieving logging parms
			//short sStation;
			//SA_GetElem(ParmsVarStruct->parray, j, 0, &(Parms.sStaNum));	// station number
			SA_GetElem(ParmsVarStruct->parray, j, 0, &sStation);			// station number
			// HN 10/25/2007 Change this to retrieve logging parms when they are there.  
			// Station # of -1 sent, then bool,int, bool for the other settings, 0s to fill in.
			if (sStation < 0)
			{
				// We have logging parms
				SA_GetElem (ParmsVarStruct->parray,j,1L,&m_bDisableLogging);
				SA_GetElem (ParmsVarStruct->parray,j,2L,&m_nLoggingVerbosity);
				SA_GetElem (ParmsVarStruct->parray,j,3L,&m_bQuietMode);

				if (m_pIGrandImport)
				{
					VARIANT vParms;
					VariantInit(&vParms);
					vParms.vt = VT_I4; //long
			
					//SAFEARRAYBOUND bound;
					//bound.cElements = 1;
					//bound.lLbound = 0;
					//vParms.vt = VT_VARIANT|VT_ARRAY;
					//vParms.parray = ::SafeArrayCreate(VT_VARIANT, 1, &bound);

					long lFlags = m_nLoggingVerbosity;

					if (m_bQuietMode)
						lFlags|=0x00010000;

					if (m_bDisableLogging)
						lFlags|=0x00020000;

					vParms.lVal = lFlags;

					VARIANT_BOOL vbSuccess = m_pIGrandImport->SetParameters (sFacilityID,&vParms);
					bSuccess = (vbSuccess != VARIANT_FALSE);

					VariantClear(&vParms);
				}
			}
			else //sStation >= 0
			{
				Parms.sStaNum = sStation;
				SA_GetElem(ParmsVarStruct->parray, j, 1L, &(Parms.sStaType));			// station type
				SA_GetElem(ParmsVarStruct->parray, j, 2L, &(Parms.Event1Time));		// event time 1
				SA_GetElem(ParmsVarStruct->parray, j, 3L, &(Parms.Event2Time));		// event time 2
				SA_GetElem(ParmsVarStruct->parray, j, 4L, &(Parms.dSlope));			// slope - not used
				SA_GetElem(ParmsVarStruct->parray, j, 5L, &(Parms.dIntercept));		// intercept - not used
				SA_GetElem(ParmsVarStruct->parray, j, 6L, &(Parms.bEnableTimeAlign));	// use time align??
				SA_GetElem(ParmsVarStruct->parray, j, 7L, &(Parms.bEnableStation));	// station enabled for time align?

				// regardless of what the slope and intercept were, recalculate new values
				// for the times given,  always use first station that is selected for use
				// as the base
				if (!bFirstUsedStation && Parms.bEnableStation) 
				{
					BaseTime1 = Parms.Event1Time;
					BaseTime2 = Parms.Event2Time;
					bFirstUsedStation = true;
				}
				if (Parms.bEnableStation)
					NewParms.CalculateSlopeAndIntercept(BaseTime1, BaseTime2, Parms.Event1Time, Parms.Event2Time, &(Parms.dSlope), &(Parms.dIntercept));

				// write the data into the ini file
				bSuccess = IniFile.WriteTimeAlignParms(sFacilityID, Parms.sStaNum, Parms.sStaType, &Parms);
			}
		}
	}
	
	*pbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

// Unadjust the given array of timestamps -- this is probably not used
// 05-Nov-2004	SFK	Added check to set data store path
// This appears to have shown up early (BL2R0) to use the station type HN 10/24/07
//STDMETHODIMP CTimeAlign::UnadjustTimestamps(short sFacilityID, short sStationID, long lNumItems, DATE *pAdjustedTimestamps, DATE *pOriginalTimestamps, VARIANT_BOOL *pbResult)
STDMETHODIMP CTimeAlign::UnadjustTimestamps(
	short sFacilityID, 
	long lStationID, 
	short sStationType, 
	long lNumItems, 
	DATE *pAdjustedTimestamps, 
	DATE *pOriginalTimestamps, 
	VARIANT_BOOL *pbResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!IniFile.m_bOpen) 
		IniFile.SetDataStorePath(sFacilityID);

	// Like AdjustTimestamps, this was added back in.
	CTimeAdjust Adjust;
	bool bSuccess = Adjust.UnadjustTimestamps(sFacilityID, lStationID, sStationType, lNumItems, pAdjustedTimestamps, pOriginalTimestamps);

	*pbResult = bSuccess?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}







