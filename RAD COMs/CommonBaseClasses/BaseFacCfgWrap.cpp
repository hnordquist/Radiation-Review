// BaseFacCfgWrap.cpp: implementation of the CBaseFacCfgWrap class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseFacCfgWrap.h"
#include "SafeArrayUtil.h"
#include "ErrorProcessing.h"  //for flag defines

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif 
//////////////////////////////////////////////////////////////////
// PUT NO operator I/O in the functions in this file.
// NDAR uses this file and must be assured that
// no blocking operator I/O occurs from within the components
// that use these functions.  NDAR is unattended.
//
// If you must, figure out a way to get the system's
// quiet mode setting and then be sure to bracket them with:
//
// if (!m_bQuietMode)
// {
//     ...do the operator's message box, printing, etc..
// }
//
// pjm 11/27/2005 for B2R1
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseFacCfgWrap::CBaseFacCfgWrap()
{
	TRACE("CBaseFacCfgWrap::CTOR_1\n");

	//Derived class needs to set the instrument name here.

	//Use this example:
	//m_csInstrumentName = CString("grand");
	//m_bstrInstrumentName = m_csInstrumentName.AllocSysString();

	HRESULT hr = m_pIFacCfgMgr.CreateInstance(CLSID_FacilityMgrData);
	SetFlags(VERBOSITY2_FATAL | QUIET_MODE | DISABLE_LOGGER);
}

CBaseFacCfgWrap::~CBaseFacCfgWrap()
{
	TRACE("CBaseFacCfgWrap::DTOR\n");

	if (m_bstrInstrumentName)
		::SysFreeString(m_bstrInstrumentName);

	if (m_pIFacCfgMgr)
		m_pIFacCfgMgr = NULL;
}
void CBaseFacCfgWrap::SetFlags(long lFlags)
{
	if (m_pIFacCfgMgr)
		m_pIFacCfgMgr->SetFacilityMgrErrorLoggingParameters(lFlags);
}

/*  Virtual function GetStationRecord is not implemented in the base class.
bool CBaseFacCfgWrap::GetStationRecord(short sFacID, long lSta, struct db_sta_rec *pdbSta)
*/
                            
// 28-Sep-2005 SFK	Added check on if m_pIFacCfgMgr is not NULL and
//					VariantInit of List (VERY IMPORTANT TO DO THIS!!!!!!)
bool CBaseFacCfgWrap::GetStationIDs(short sFacID, short *psNum, long* pIDs)
{
	bool bSuccess = false;

	VARIANT List;
	::VariantInit(&List);

	try
	{
		if (m_pIFacCfgMgr != NULL)
		{
			m_pIFacCfgMgr->GetStationIDsWithInstrumentType(sFacID, m_bstrInstrumentName, true, &List);

			if(List.vt != VT_EMPTY) 
			{
				long NumStations;
				
				SafeArrayGetUBound(List.parray, 1, &NumStations);

				if ((short)NumStations <= *psNum) 
				{
					CString TempStr;
					long lTemp;
					int j=0;
					//for(long i = 0; i <= NumStations; i++) //STOMP
					for(long i = 0; i < NumStations; i++)
					{
						SA_GetElem(List.parray, i, 0, &lTemp);			// station number id
						pIDs[j] = lTemp;
						j++;
					}
					bSuccess = true;
					*psNum = j;		// return the number of GRAND stations at the facility
				}
			}
		}
	}
	catch(...)
	{
		bSuccess = false;
	}
	::VariantClear(&List);
	return bSuccess;
}

int CBaseFacCfgWrap::GetNumStations(short sFacID)
{
	int bNum = 0;

	if (m_pIFacCfgMgr) 
		bNum = m_pIFacCfgMgr->GetNumberOfStationsForFacility(sFacID, VARIANT_TRUE);

	return(bNum); 
}

CString CBaseFacCfgWrap::GetFacilityDirectory(short sFacID)
{
	//USES_CONVERSION;
	CString strTemp("CBaseFacCfgWrap::GetFacilityDirectory() error");
	
	if (m_pIFacCfgMgr)
	{
		BSTR bstr = m_pIFacCfgMgr->GetFacilityDirectory(sFacID);
		CString cs(bstr);
		strTemp = cs;
		::SysFreeString(bstr);
	}

	//strTemp = W2T(m_pIFacCfgMgr->GetFacilityDirectory(sFacID));
	return(strTemp);
}

CString CBaseFacCfgWrap::GetFacilityLongName(short sFacID)
{
	//USES_CONVERSION;
	CString strTemp("CBaseFacCfgWrap::GetFacilityLongName() error");

	if (m_pIFacCfgMgr)
	{
		BSTR bstr = m_pIFacCfgMgr->GetFacilityLongName(sFacID);
		CString cs(bstr);
		strTemp = cs;
		::SysFreeString(bstr);
	}

	//if (m_pIFacCfgMgr)
	//	strTemp = W2T(m_pIFacCfgMgr->GetFacilityLongName(sFacID));

	return strTemp;
}
                           
bool CBaseFacCfgWrap::FacilityExists(short sFacID)
{
	VARIANT_BOOL vbExists = VARIANT_FALSE;

	if (m_pIFacCfgMgr) 
		vbExists = m_pIFacCfgMgr->IsFacilityDefined(sFacID);

	return (vbExists != VARIANT_FALSE);
}
