// FacilityConfig.cpp: implementation of the CFacilityConfig class.
//
//////////////////////////////////////////////////////////////////////
//19 Apr 2006 - pjm - changed code to reduce compiler warnings.

#include "Radbuild.h"
#include "FacilityConfig.h"
#include "RadError.h"
#include "GUI_Glue.h"
#include "SafeArrayUtil.h"
#include "imports.h"
#include "database\db_main.h"
#include "TypeDefinitions.h"

extern bool glbFacilityPicklistFilled;
extern bool glbStationPicklistFilled;
extern bool glbChannelPicklistFilled;

#define FACILITY_PICKLIST 0
#define CHANNEL_PICKLIST 1
#define STATION_PICKLIST 2

#define MAX_PICKLIST_LEN 1000
struct PicklistInfo {
	short sID1;				// facility or station id
	short sID2;				// station type
	short sID3;				// channel id
	short sID4;				// station data type
	short sPicklistPosition;
	CString Type;			// whether grand, isr, etc.
	CString PicklistLongName;
	CString PicklistShortName;
};

static struct PicklistInfo gllFacilityPicklist[MAX_PICKLIST_LEN];
static struct PicklistInfo gllStationPicklist[MAX_PICKLIST_LEN];
static struct PicklistInfo gllChannelPicklist[MAX_PICKLIST_LEN];
static short gllsFacilityPicklistItems = 0;
static short gllsStationPicklistItems = 0;
static short gllsChannelPicklistItems = 0;
static short glID_List[MAX_PICKLIST_LEN];

struct MoreStationTypeStuff {
	char  szStaTypeName[MAX_NAME_LEN+1];
	char  szFileExt[5];
	short sStaType;
	short sDataType;
};

#define NUM_TYPES_DEFINED 12
// 20-Dec-2005 SFK Changed type strings for *.bsr to "bsr" (not "jsr") and for *.jsr to "jsr" (not "jsr1")
static struct MoreStationTypeStuff gllStaStuff[NUM_TYPES_DEFINED] = {
 "grand", ".bid", GRAND_TYPE, FLOAT_TYPE, 
 "bsr",   ".bsr", JSR_TYPE,   DOUBLE_TYPE,
 "isr",   ".isr", ISR_TYPE,   DOUBLE_TYPE,
 "mca",   ".mca", MCA_TYPE,   FLOAT_TYPE,
 "op",    ".id",  OP_TYPE,    FLOAT_TYPE,
 "jsr",  ".jsr", JSR1_TYPE,  DOUBLE_TYPE,
 "bot",   ".*",   BOT_TYPE,   FLOAT_TYPE,
 "bot",   ".vbf", BOT_TYPE,   FLOAT_TYPE,
 "binary",".bin", BINARY_TYPE,INTEGER_TYPE,
 "eoss",  ".ess", EOSS_TYPE, INTEGER_TYPE,
 "hmr",   ".hmr", ISR_TYPE,   DOUBLE_TYPE,
 "dmgrand", ".bi0", GRAND_TYPE, FLOAT_TYPE, 
};
																     
IFacilityMgrDataPtr pIFacMgrData;									// sk 09/15/2003
IFacilityConfigDlgsPtr pIDlgs;										// sk 09/15/2003

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFacilityConfig::CFacilityConfig()
{
    pIFacMgrData.CreateInstance(CLSID_FacilityMgrData);				// sk 09/15/2003
	if (pIFacMgrData == NULL)
	{
		MessageBox(
			NULL, 
			"Cannot create Facility Manager.\nProbable cause: Component registration errors.",
			"FATAL ERROR",MB_ICONSTOP|MB_OK);
		exit(0);
	}

	pIDlgs.CreateInstance(CLSID_FacilityConfigDlgs);				// sk 09/15/2003
	if (pIDlgs == NULL)
	{
		MessageBox(
			NULL,
			"Cannot create Facility Configs dialogs.\nProbable cause: Component registration errors.",
			"FATAL ERROR",MB_ICONSTOP|MB_OK);
		pIFacMgrData = NULL;
		exit(0);
	}

	m_EnabledFacilitiesOnly = TRUE;
	m_OrderFacilitiesByPriority = FALSE;
	m_EnabledStationsOnly = TRUE;
	m_OrderStationsByPriority = FALSE;
}

CFacilityConfig::~CFacilityConfig()
{
	pIDlgs = NULL;
	pIFacMgrData = NULL;
}



///////////////////////////////////////////////////////////////////////////
//	Name:	ShowFacilityConfigDlg, ShowSensorManagerDlg, ShowUnitsManagerDlg
//
//	Description:
//	Used to call the Facility Manager dialog boxes from the Rad main menu
//	
//	Declaration:
//	void CFacilityConfig::ShowFacilityConfigDlg()
//	void CFacilityConfig::ShowSensorManagerDlg()
//	void CFacilityConfig::ShowUnitsManagerDlg()
//
//	Input:	None
//
//	Output:	None
//
//	Return:	None
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
void CFacilityConfig::ShowFacilityConfigDlg()
{
	pIDlgs->ShowFacilityManagerDlg();
	
}
void CFacilityConfig::ShowSensorManagerDlg()
{
	pIDlgs->ShowInstrumentManagerDlg();
}

void CFacilityConfig::ShowUnitsManagerDlg()
{
	pIDlgs->ShowUnitsManagerDlg();
}

///////////////////////////////////////////////////////////////////////////
//	Name:	CreateFacilityPicklist
//
//	Description:
//	Gets a list with all the Facility names from Facility Manager
//	
//	Declaration:
//	int CFacilityConfig::CreateFacilityPicklist(char **pAdrs[])
//
//	Input:	None
//
//	Output:	pAdrs		pointers needed by gen_allocate_str_array and GUI picklists
//
//	Return:	1(were some facilities)/0(error or no facilities)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//	10-Dec-2003	SFK		Modified to only read the information from FacMgr once.
//////////////////////////////////////////////////////////////////
int CFacilityConfig::CreateFacilityPicklist(char **pAdrs[])
{
	USES_CONVERSION;
	long i;
	static long lNumStrings;
	static int iNameLen;

	VARIANT List;
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// DCOM requires initialization of all VARIANTS.  Do that here.
	VariantInit (&List);

	if (!glbFacilityPicklistFilled) 
	{
		//Get a list of all facilities known to the facility configurator
		pIFacMgrData->GetFacilityNamesList(m_EnabledFacilitiesOnly, m_OrderFacilitiesByPriority, &List);

		if(List.vt == VT_EMPTY) 
		{
			MessageBox(NULL, "No Facility Names were returned or found.", "GetFacilityNamesList: Result", MB_OK | MB_ICONEXCLAMATION);
			return(FALSE);
		}

		int iLen;
		iNameLen = 0;

		//Load the facility names into an array of CStrings and figure out the longest name
		SafeArrayGetUBound(List.parray, 1, &lNumStrings);
		lNumStrings++;
		InitPicklist(FACILITY_PICKLIST, lNumStrings);			// clear out our local storage array

		for(i = 0; i <lNumStrings; i++) 
		{	// figure out longest name
			SA_GetElem(List.parray, i, &gllFacilityPicklist[i].PicklistLongName);
			iLen = gllFacilityPicklist[i].PicklistLongName.GetLength();
			if (iLen > iNameLen) 
				iNameLen = iLen;
		}
		for(i = 0; i <lNumStrings; i++) 
		{		// fill in the rest of the global facility picklist stuff
			// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
			// cast to a short coming out of FacMgr, here we just store in our local struct, which holds a short.  Comes
			// out of FacMgr as a long.

			gllFacilityPicklist[i].sID1 = (short)GetFacilityID(gllFacilityPicklist[i].PicklistLongName.GetBuffer(iNameLen));		// keep track of facility ID associated with this picklist item
			gllFacilityPicklist[i].sPicklistPosition = (short)i;					// picklist position
		}
		//SafeArrayDestroy(List.parray);
		glbFacilityPicklistFilled = true;
	}

	// Now copy CStrings over to gui picklist format
	if (AllocStrArray(lNumStrings, iNameLen, pAdrs)) 
	{
		for(i = 0; i <lNumStrings; i++) 
			strcpy ((*pAdrs)[i], gllFacilityPicklist[i].PicklistLongName.GetBuffer(iNameLen));
	}
	VariantClear(&List);
	gllsFacilityPicklistItems = (short)i;
	(*pAdrs)[i] = NULL;
	return(TRUE);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetFacilityIDForPicklistChoice
//
//	Description:
//	Given the picklist position number, find the associated Facility ID.
//	
//	Declaration:
//	BOOL CFacilityConfig::GetFacilityIDForPicklistChoice(short sPicklistChoice, short *psFacID)
//
//	Input:	sPicklistChoice		which position in the picklist
//
//	Output:	psFacID				Facility ID associated with picklist position
//
//	Return:	TRUE (returned ID)/FALSE (invalid picklist choice)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetFacilityIDForPicklistChoice(short sPicklistChoice, short *psFacID)
{
	if (sPicklistChoice > gllsFacilityPicklistItems) return FALSE;		// invalid request
	*psFacID = gllFacilityPicklist[sPicklistChoice].sID1;
	return(TRUE);	
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetPicklistChoiceForFacilityID
//
//	Description:
//	Given the Facility ID, determine which picklist position goes with it.
//	
//	Declaration:
//	BOOL CFacilityConfig::GetPicklistChoiceForFacilityID(short sFacID, short *psPicklistPosition)
//
//	Input:	sFacID		Facility ID number
//
//	Output:	psPicklistPosition	associated picklist position (0-based)
//
//	Return:	TRUE (found picklist position)/FALSE (facility not in picklist)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetPicklistChoiceForFacilityID(short sFacID, short *psPicklistPosition)
{
	for (int i=0; i<gllsFacilityPicklistItems; i++) {
		if (gllFacilityPicklist[i].sID1 == sFacID) {
			*psPicklistPosition = gllFacilityPicklist[i].sPicklistPosition;
			return(TRUE);
		}
	}
	return(FALSE);		// invalid request
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetNumFacilities
//			GetNumStations
//			GetNumChannelsForFacility
//
//	Description:
//	Gets the number of facilities that are currently known to Facility Manager
//	
//	Declaration:
//	int CFacilityConfig::GetNumFacilities()
//
//	Input:	None
//
//	Output:	None
//
//	Return:	Number of facilities
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
int CFacilityConfig::GetNumFacilities()
{
	short sNumEnabledFacilities = pIFacMgrData->GetNumberOfFacilities((VARIANT_BOOL)true);
	return(sNumEnabledFacilities);								
}

int CFacilityConfig::GetNumStations(short sFacID)
{
	//return(pIFacMgrData->GetNumberOfStationsForFacility(sFacID, (VARIANT_BOOL)true)); 
	int iDum;
	return(GetNumRadStations(sFacID, &iDum)); 
}

int CFacilityConfig::GetNumChannelsForFacility(short sFacID)
{
	//return(pIFacMgrData->GetNumberOfChannelsForFacility(sFacID, true)); 
	int iNumChans;
	GetNumRadStations(sFacID, &iNumChans);
	return(iNumChans);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetFacilityID
//
//	Description:
//	Given the facility name, return the ID used in FacilityManager.
//	If facility does not exist, ?? returned.
//	
//	Declaration:
//	short CFacilityConfig::GetFacilityID(const char *strFacilityLongName)
//
//	Input:	strFacilityLongName		long name of facility
//
//	Output:	None
//
//	Return:	facility id number from FacilityManager
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
short CFacilityConfig::GetFacilityID(const char *strFacilityLongName)
{
	return(pIFacMgrData->GetFacilityID(strFacilityLongName));
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetFacilityIDs
//
//	Description:
//	Gets all the Facility IDs known to Facility Manager
//	
//	Declaration:
//	BOOL CFacilityConfig::GetFacilityIDs(int *piFacIDs)
//
//	Input:	User must have allocated space for all the facility IDs -- know how
//			many facilities from calling GetNumFacilities before this call.
//
//	Output:	piFacIDs	Array containing facility IDs (integers)
//
//	Return:	TRUE (found some facilities)/ FALSE (no facility IDs found)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetFacilityIDs(int *piFacIDs)
{
	VARIANT List;

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// DCOM requires initialization of all VARIANTS.  Do that here.
	VariantInit (&List);

	BOOL bReturn = FALSE;

	pIFacMgrData->GetFacilityIDs(m_EnabledFacilitiesOnly,  &List);

	if(List.vt == VT_EMPTY) 
	{
		MessageBox(NULL, "No Facility IDs were returned or found.", "GetFacilityIDs: Result", MB_OK | MB_ICONEXCLAMATION);
		glID_List[0] = -1;
		//return(FALSE);
	}
	else
	{
		long Dimension[1] = {0};
		long lNumIDs;

		SafeArrayGetUBound(List.parray, 1, &lNumIDs);

		//if (lNumIDs <0) 
		//{
			//SafeArrayDestroy(List.parray);
			//return(FALSE);
		//}
		//else
		if (lNumIDs >= 0)  //<<QA>>Should 0 here put out the same message box as above?
		{
			for(LONG i = 0; i <= lNumIDs; i++)
				SA_GetElem(List.parray, i, 0, &piFacIDs[i]);

			bReturn = TRUE;
		}
		//SafeArrayDestroy(List.parray);
		//}
	}

	VariantClear(&List);
	return(bReturn);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetFacilityRecord
//
//	Description:
//	Get the record associated with the given Facility ID.
//	
//	Declaration:
//	BOOL CFacilityConfig::GetFacilityRecord(short sFacID, struct db_fac_rec *pdbFac)
//
//	Input:	sFacID		Facility ID number
//
//	Output:	pdbFac		Standard facility record known to Rad
//
//	Return:	TRUE (read a record)/FALSE (no record)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetFacilityRecord(short sFacID, struct db_fac_rec *pdbFac)
{
	VARIANT saFacilityRecord;
	VariantInit (&saFacilityRecord);

	pIFacMgrData->GetFacilityRecord(sFacID, &saFacilityRecord);

	if(saFacilityRecord.vt ==VT_EMPTY)	
	{
		VariantClear(&saFacilityRecord);
		return(FALSE);
	}
	else 
	{
		//Unload the information from the safearray.
		CString TempStr;

		SA_GetElem(saFacilityRecord.parray, 0, &pdbFac->s_fac_num_key);

		SA_GetElem(saFacilityRecord.parray, 1, &pdbFac->s_fac_picklist_num);

		SA_GetElem(saFacilityRecord.parray, 2, &TempStr);
		strncpy(pdbFac->sz_fac_name, TempStr.GetBuffer(MAX_NAME_LEN), MAX_NAME_LEN);
		pdbFac->sz_fac_name[MAX_NAME_LEN] = '\0';
		
		SA_GetElem(saFacilityRecord.parray, 3, &TempStr);
		strncpy(pdbFac->sz_fac_short_name, TempStr.GetBuffer(MAX_SHRT_NAME_LEN), MAX_SHRT_NAME_LEN);
		pdbFac->sz_fac_short_name[MAX_SHRT_NAME_LEN] = '\0';

		// skip the rest of the stuff in the facility record for now
	}
	VariantClear (&saFacilityRecord);
	return(TRUE);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetFacilityLongName
//
//	Description:
//	Given the Facility ID, get the associated long name.
//	
//	Declaration:
//	void CFacilityConfig::GetFacilityLongName(short sFacID, char *strFacName, int iLen)
//
//	Input:	sFacID		Facility ID number
//			iLen		Maximum space allocated by Rad for facility id name
//
//	Output:	strFacName	Facility long name as a char *
//
//	Return:	None
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//	20-Dec-2005 SFK		Immediately convert and copy the BSTR value returned from Facility Manager
//						pjm: This is part of the fix for DR00189: "Invalid characters in some station names"
//////////////////////////////////////////////////////////////////
void CFacilityConfig::GetFacilityLongName(short sFacID, char *strFacName, int iLen)
{
	//DR 189: Got rid of the conversion macro.  PJM 9-24-2007
	//USES_CONVERSION;
	BSTR bstr = pIFacMgrData->GetFacilityLongName(sFacID);
	CString cs(bstr);
	strncpy(strFacName,LPCTSTR(cs),iLen-1);
	//strncpy(strFacName, W2A(pIFacMgrData->GetFacilityLongName(sFacID)), iLen-1);
	strFacName[iLen] = '\0';
	::SysFreeString(bstr);;
}

void CFacilityConfig::GetFacilityDirectory(short sFacID, char *strFacDirectory, int iLen)
{
	USES_CONVERSION;
	strncpy(strFacDirectory, W2A(pIFacMgrData->GetFacilityDirectory(sFacID)), iLen-1);
	strFacDirectory[iLen] = '\0';
}

///////////////////////////////////////////////////////////////////////////
//	Name:	IsFacilityDefined
//
//	Description:
//	Check if the Facility ID is known to Facility Manager, if not put out a message.
//	
//	Declaration:
//	BOOL CFacilityConfig::IsFacilityDefined(short sFacID)
//
//	Input:	sFacID		Facility ID number
//
//	Output:	None
//
//	Return:	TRUE (facility is defined)/FALSE (not defined)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//	01-Dec-2004	SFK		Check that there is some data for a RAD station at the facility
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::IsFacilityDefined(short sFacID)
{
	if (!pIFacMgrData->IsFacilityDefined(sFacID)) 
	{
		CString Msg;
		Msg.Format("Facility %d is not defined.  Please select another facility", sFacID);
		MessageBox(NULL, Msg, "Radiation Review Information", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	return TRUE;
}

BOOL CFacilityConfig::DoesFacilityHaveAnyData(short sFacID)
{
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility with RAD data

	int iNumRadStations = 0;
	int iNumRadChannels = 0;
	struct IDStruct TempID;
	static BOOL bValidData = FALSE;
	static short sFacilityChecked;
	static bool bFirst = true;

	if ((!bFirst) && (sFacilityChecked == sFacID))  
	{
		if (!bValidData) 
		{
			RadReviewMsg(uiDB_EMPTY_STATUS);
			bFirst = true;	// 03-Jan-2006 SFK Force a check next time
		}
		return bValidData;
	}

	bFirst = false;
	sFacilityChecked = sFacID;

	unsigned long ulTotalNumPts = 0;

	VARIANT List;
	VariantInit (&List);

	pIFacMgrData->GetStationIDs(sFacID, m_EnabledStationsOnly,  &List);
	
	if(List.vt == VT_EMPTY) 
	{
		MessageBox(NULL, "No STATION IDs were returned or found.", "GetStationIDs: Result", MB_OK | MB_ICONEXCLAMATION);
		VariantClear(&List);
		return(FALSE);
	}
	else 
	{
		long NumStations;
		int iNumChannels;
		
		//Sort through the station ID records to see which types are for Rad
		SafeArrayGetUBound(List.parray, 1, &NumStations);
		CString TempStr;
		short sTempStationType;			

		// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
		// we need to pull the station ID out as a long, then convert to a short.
		long lStaID = -1;
		
		int j=0;
		for(LONG i = 0; i <= NumStations; i++) 
		{
			// Here we ask for a long from SA, then convert.
			SA_GetElem(List.parray, i, 0, &lStaID);	// get station id
			TempID.sStaID = (short) lStaID; 
			SA_GetElem(List.parray, i, 1, &TempStr);		// get station type (grand, bot, etc.)
		
			strncpy(TempID.szStaType, (LPCTSTR)TempStr, MAX_NAME_LEN);
			TempID.szStaType[MAX_NAME_LEN] = '\0';

			// only return IDs of stations Rad knows about
			if (!StationTypeStrToNum(TempID.szStaType, &sTempStationType)) {
				continue;
			}
			TempID.sStaType = sTempStationType;
			iNumRadStations++;
			iNumChannels = GetNumChannelsForStation(sFacID, &TempID);
			iNumRadChannels += iNumChannels;
		}
	}
		
	if (iNumRadStations == 0) 
	{
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return FALSE;
	}
	else 
		bValidData = true;

	VariantClear(&List);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetFirstFacilityID
//
//	Description:
//	Get the ID of the first facility known to Facility Manager
//	
//	Declaration:
//	BOOL CFacilityConfig::GetFirstFacilityID(short *psFacNum)
//
//	Input:	None
//
//	Output:	psFacNum	First Facility ID in Facility Manager
//
//	Return:	TRUE (a facility exists)/FALSE (no facilities defined)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetFirstFacilityID(short *psFacNum)
{
	VARIANT List;
	VariantInit (&List);
	VARIANT ID;
	VariantInit (&ID);

	pIFacMgrData->GetFacilityIDs(m_EnabledFacilitiesOnly,  &List);

	if(List.vt == VT_EMPTY) 
	{
		MessageBox(NULL, "No Facility IDs were returned or found.", "GetFacilityIDs: Result", MB_OK | MB_ICONEXCLAMATION);
		glID_List[0] = -1;
		VariantClear(&List);
		VariantClear(&ID);
		return(FALSE);

	}
	else
	{
		long Dimension[1] = {0};
		long lNumIDs;

		SafeArrayGetUBound(List.parray, 1, &lNumIDs);
		if (lNumIDs <0) 
		{
			VariantClear(&List);
			VariantClear(&ID);
			return(FALSE);
		}



		// Just need the first ID so read it out
		Dimension[0] = 0;
		SafeArrayGetElement(List.parray, Dimension, &ID);
		*psFacNum = ID.iVal;

	}
	VariantClear(&List);
	VariantClear(&ID);
	return(TRUE);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	CreateStationPicklist
//
//	Description:
//	Gets a list with all the Station names from Facility Manager
//	
//	Declaration:
//	int CFacilityConfig::CreateStationPicklist(short sFacID, BOOL bAllStations, char **pAdrs[], int *piMaxNameLen)	
//
//	Input:	sFacID			which facility to get stations for
//			bAllStations	true/false as to whether to include "All Stations" in the list
//
//	Output:	pAdrs			pointers to strings needed by gen_allocate_str_array and GUI picklists			
//			piMaxNameLen	longest name in the picklist
//
//	Return:	TRUE(picklist created)/0(no stations in facility)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//	25-Nov-2003	SFK		Modified to check if the station is one that Rad knows about
//	10-Dec-2003	SFK		Modified to only read the information from FacMgr once.
//////////////////////////////////////////////////////////////////
int CFacilityConfig::CreateStationPicklist(short sFacID, BOOL bAllStations, char **pAdrs[], int *piMaxNameLen)
{
	USES_CONVERSION;
	long i;
	int j;
	static int iNameLen;
	static long lNumRadStrings;

	if (!glbStationPicklistFilled) 
	{
		VARIANT List;
		// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
		// DCOM requires initialization of all VARIANTS.  Do that here.
		::VariantInit (&List);

		//Get a list of all stations known to the FacMgr for the facility
		pIFacMgrData->GetStationNamesList(sFacID, m_EnabledStationsOnly, m_OrderStationsByPriority, &List);

		if(List.vt == VT_EMPTY) {
			MessageBox(NULL, "No Station Names were returned or found.", "GetStationNamesList: Result", MB_OK | MB_ICONEXCLAMATION);
			return(FALSE);
		}

		long lDimension[1] = {0};//,0};
		long lNumStrings;
		VARIANT vName;
		//We don't send this into the interface, but initialize anyway.
		VariantInit (&vName);
		iNameLen = 0;
		int iLen;
		CString TempName, TempType;
		//short sTempID, sTempType;	// 28-Feb-2004 SFK Changed ID to a long
		long lTempID;
		short sTempType;
		BSTR bstrInstrumentType;
		j = 0;

		SafeArrayGetUBound(List.parray, 1, &lNumStrings);
		InitPicklist(STATION_PICKLIST,lNumStrings);									// clear out our local storage array
		for(i = 0; i <=lNumStrings; i++) {
			lDimension[0] = i;
			SafeArrayGetElement(List.parray, lDimension, &vName);
			TempName = W2T(vName.bstrVal);

			// verify this station is one that Rad knows about
			// We now retrieve the station ID as a long, then store as a short.
			pIFacMgrData->GetStationID(sFacID, vName.bstrVal, &lTempID, &bstrInstrumentType);		// keep track of facility ID associated with this picklist item
			//pIFacMgrData->GetStationID(sFacID, vName.bstrVal, &sTempID, &bstrInstrumentType);		// 08-Mar-2005 SFK Changed ID back to short
			TempType = W2T(bstrInstrumentType);
			if (!StationTypeStrToNum(TempType, &sTempType)) {
				continue;
			}

			// this is a Rad station, put it in the list
			gllStationPicklist[j].sID1 = (short)lTempID;	// 28-Feb-2004 SFK Changed ID to a long
			//gllStationPicklist[j].sID1 = (short)sTempID;	// 08-Mar-2005 SFK Changed ID to a short
			gllStationPicklist[j].PicklistLongName = TempName;
			iLen = gllStationPicklist[j].PicklistLongName.GetLength();
			if (iLen > iNameLen) iNameLen = iLen + 1;
			gllStationPicklist[j].Type = W2T(bstrInstrumentType);			// save instrument type string
			if ((!StationTypeStrToNum(gllStationPicklist[j].Type, &gllStationPicklist[j].sID2)) ||
				(!StationTypeStrToDataType(gllStationPicklist[j].Type, &gllStationPicklist[j].sID4))) {
				return (FALSE);
			}
			gllStationPicklist[j].sPicklistPosition = j;						// picklist position
			j++;
		}
		lNumRadStrings = j;
		glbStationPicklistFilled = true;
		SafeArrayDestroy(List.parray);
	}

	// Now copy CStrings over to gui picklist format
	//long lNumAllocStrings = lNumStrings;
	long lNumAllocStrings = lNumRadStrings;
	if (bAllStations) {
		lNumAllocStrings++;					// include space for All Stations
		//pjm 26-Jan-2006 Added explicit cast to eliminate compiler warning.
		if (iNameLen < (int)strlen("All Stations")) iNameLen = (int)strlen("All Stations") + 1;
	}
	if (AllocStrArray(lNumAllocStrings, iNameLen, pAdrs)) {
		for(i = 0; i <lNumRadStrings; i++) { 
			strcpy ((*pAdrs)[i], gllStationPicklist[i].PicklistLongName.GetBuffer(iNameLen));
		}
	}
	if (bAllStations) {		// 01-Apr-2005 Fill in all parts ofthe picklist for All choice
		strcpy ((*pAdrs)[i], "All Stations");
		gllStationPicklist[i].sID1 = -1;			// -1 is special identifier for all stations within Rad
		gllStationPicklist[i].sID2 = -1;
		gllStationPicklist[i].sID3 = -1;
		gllStationPicklist[i].sID4 = -1;
		gllStationPicklist[i].Type.Empty();
		gllStationPicklist[i].PicklistLongName.Empty();
		gllStationPicklist[i].PicklistShortName.Empty();
		gllStationPicklist[i].sPicklistPosition = (short)i;				
		i++;
	}
//	gllsStationPicklistItems = i-1;
	gllsStationPicklistItems = (short)i;
	(*pAdrs)[i] = NULL;
	*piMaxNameLen = iNameLen;
	
	return(TRUE);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetStationIDForPicklistChoice
//
//	Description:
//	Given the picklist choice, return the associated IDStruct for the station
//
//	Declaration:
//	BOOL CFacilityConfig::GetStationIDForPicklistChoice(short sPicklistChoice, IDStruct *pStaID)
//
//	Input:	sPicklistChoice		which picklist position
//
//	Output:	pStaID				IDStruct with station info
//
//	Return:	TRUE (station exists)/FALSE (invlaid picklist choice)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetStationIDForPicklistChoice(short sPicklistChoice, IDStruct *pStaID)
{
	if (sPicklistChoice > gllsStationPicklistItems) return(FALSE);			// invalid request
	pStaID->sStaID = gllStationPicklist[sPicklistChoice].sID1;
	strncpy(pStaID->szStaType, gllStationPicklist[sPicklistChoice].Type.GetBuffer(MAX_NAME_LEN), MAX_NAME_LEN);
	pStaID->szStaType[MAX_NAME_LEN] = '\0';
	pStaID->sStaType = gllStationPicklist[sPicklistChoice].sID2;
	pStaID->sDataType = gllStationPicklist[sPicklistChoice].sID4;
	return(TRUE);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetPicklistChoiceForStationID
//
//	Description:
//	Given the Station IDStruct, determine which picklist position goes with it.
//	
//	Declaration:
//	BOOL CFacilityConfig::GetPicklistChoiceForStationID(struct IDStruct StaID, short *psPicklistPosition)
//
//	Input:	StaID		IDStruct describing station
//
//	Output:	psPicklistPosition	associated picklist position (0-based)
//
//	Return:	TRUE (found picklist position)/FALSE (not in picklist)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetPicklistChoiceForStationID(struct IDStruct StaID, short *psPicklistPosition)
{
	for (int i=0; i<gllsStationPicklistItems; i++) {
		if ((gllStationPicklist[i].sID1 == StaID.sStaID) && (gllStationPicklist[i].sID2 == StaID.sStaType)) {
			*psPicklistPosition = gllStationPicklist[i].sPicklistPosition;
			return(TRUE);
		}
	}
	*psPicklistPosition = gllStationPicklist[0].sPicklistPosition;		// if nothing matches return the first one
	return(FALSE);		// invalid request
}

///////////////////////////////////////////////////////////////////////////
//	Name:	IsStationEnabled
//
//	Description:
//	Given the station information in IDStruct, return whether or not the
//	station is enabled.
//	Note: Will fill in station type string in IDStruct
//
//	Declaration:
//	BOOL CFacilityConfig::IsChannelEnabled(short sFacID, struct IDStruct ChanID)
//
//	Input:	sFacID		which facility
//			pStaID		IDStruct describing channel
//
//	Output:	none
//
//	Return:	TRUE (enabled)/FALSE (not enabled)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::IsStationEnabled(short sFacID, struct IDStruct *pStaID)
{
	USES_CONVERSION;
	//char szTypeName[MAX_NAME_LEN+1];
	//pjm 26-Jan-2006 Changed bool to BOOL to eliminate compiler warning
	BOOL bStatus1, bStatus2;

	StationTypeNumToStr(pStaID);		// make sure the station type string is valid
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// Here we just cast in the interface calls to LONG types for te station ID.

	bStatus1 = pIFacMgrData->IsStationDefined(sFacID, (LONG)pStaID->sStaID, A2W(pStaID->szStaType));
	bStatus2 = pIFacMgrData->IsStationEnabled(sFacID, (LONG) pStaID->sStaID, A2W(pStaID->szStaType));
	return(bStatus2);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	StationTypeStrToNum
//			StationTypeNumToStr
//
//	Description:
//	Functions to switch back and forth between type number and name.  Private to
//	CFacilityConfig
//
//	Declaration:
//	BOOL CFacilityConfig::StationTypeStrToNum(const char *strType, short *pTypeNum)
//
//	Input:	None
//
//	Output:	psFacNum	First Facility ID in Facility Manager
//
//	Return:	TRUE (a facility exists)/FALSE (no facilities defined)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::StationTypeStrToNum(const char *strType, short *pTypeNum)
{
	for (int i=0; i < NUM_TYPES_DEFINED; i++) 
	{
		if (_stricmp(strType, gllStaStuff[i].szStaTypeName) == 0) 
		{
			*pTypeNum = gllStaStuff[i].sStaType;
			return(TRUE); 
		}
	}
	return(FALSE);
}

// do we need this?
BOOL CFacilityConfig::StationTypeStrToDataType(const char *strType, short *pDataTypeNum)
{
	*pDataTypeNum = 0;
	for (int i=0; i<NUM_TYPES_DEFINED; i++) {
		if (_stricmp(strType, gllStaStuff[i].szStaTypeName) == 0) {
			*pDataTypeNum = gllStaStuff[i].sDataType;
			return(TRUE); 
		}
	}
	return(FALSE);
}

// do we need this??
BOOL CFacilityConfig::StationTypeNumToDataType(short sType, short *pDataTypeNum)
{
	*pDataTypeNum = 0;
	for (int i=0; i<NUM_TYPES_DEFINED; i++) {
		if (sType == gllStaStuff[i].sStaType) {
			*pDataTypeNum = gllStaStuff[i].sDataType;
			return(TRUE); 
		}
	}
	return(FALSE);
}

// Given the sStaType, fill in the string name
BOOL CFacilityConfig::StationTypeNumToStr(struct IDStruct *pStaID)
{
	for (int i=0; i<NUM_TYPES_DEFINED; i++) {
		if (gllStaStuff[i].sStaType == pStaID->sStaType) {
			strncpy(pStaID->szStaType, gllStaStuff[i].szStaTypeName, MAX_NAME_LEN);
			pStaID->szStaType[MAX_NAME_LEN] = '\0';
			return(TRUE);
		}
	}
	return(0);	// 0 is not a valid type
}
//do we need this??
short CFacilityConfig::GetStationTypeNum(short sFac, char *szFileExtension)
{
	for (int i=0; i<NUM_TYPES_DEFINED; i++) {
		if (_stricmp(szFileExtension, gllStaStuff[i].szFileExt) == 0) return(gllStaStuff[i].sStaType);
	}
	return(0);	// 0 is not a valid type
}
// do we need this??
BOOL CFacilityConfig::GetStationTypeName(short sFac, short sType, int iLen, char *pstrType)
{
	for (int i=0; i<NUM_TYPES_DEFINED; i++) {
		if (gllStaStuff[i].sStaType == sType) {
			strncpy(pstrType, gllStaStuff[i].szStaTypeName, iLen);
			pstrType[iLen] = '\0';
			return(TRUE);
		}
	}
	return(FALSE);	// 0 is not a valid type
}
// do we need this??
char * CFacilityConfig::GetStationDataType(short sFacID, struct IDStruct *pID)
{
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast short ID to long for this interface call
	return(pIFacMgrData->GetStationLongName(sFacID, (LONG)pID->sStaID, pID->szStaType ));
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetStationID
//
//	Description:
//	Given the station long name, return an IDStruct describing the station
//	
//	Declaration:
//	BOOL CFacilityConfig::GetStationID(short sFac, char *szStaLongName, struct IDStruct *pStaID)
//
//	Input:	szStaLongName		long name of station
//
//	Output:	pStaID				IDStruct associated with the station
//
//	Return:	facility id number from FacilityManager
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetStationID(short sFac, char *szStaLongName, struct IDStruct *pStaID)
{
	USES_CONVERSION;
	BSTR bstr;
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// It looks like Shirley had done some of this.  HMMMMM?

	long lTempID;	// 06-Jul-2005 SFK Changed ID to a long
//	short sTempID;

	pIFacMgrData->GetStationID(sFac, szStaLongName, &lTempID, &bstr);	// 28-Feb-2005 SFK Changed ID to a long
//	pIFacMgrData->GetStationID(sFac, szStaLongName, &sTempID, &bstr);	// 08-Mar-2005 SFK Changed ID back to short
//	pStaID->sStaID = (short)sTempID;
	pStaID->sStaID = (short)lTempID;
	strncpy(pStaID->szStaType, W2A(bstr), MAX_NAME_LEN);

	::SysFreeString(bstr);

	pStaID->szStaType[MAX_NAME_LEN] = '\0';
	if (!StationTypeStrToNum(pStaID->szStaType, &(pStaID->sStaType))) return(FALSE);
	if (!StationTypeStrToDataType(pStaID->szStaType, &(pStaID->sDataType))) return(FALSE);
	return(TRUE);
}

// fills in all station id stuff.  Station number and type must be filled in before calling this routine
BOOL CFacilityConfig::GetStationID(short sFac, struct IDStruct *pStaID)
{
	if (pStaID->sStaType == -1) {	// check for special case of any type
		strcpy(pStaID->szStaType, "");
		pStaID->sDataType = -1;
		return(TRUE);
	}

	for (int i=0; i<NUM_TYPES_DEFINED; i++) {
		if (gllStaStuff[i].sStaType == pStaID->sStaType) {
			strncpy(pStaID->szStaType, gllStaStuff[i].szStaTypeName, MAX_NAME_LEN);
			pStaID->szStaType[MAX_NAME_LEN] = '\0';
			pStaID->sDataType = gllStaStuff[i].sDataType;
			return(TRUE);
		}
	}
	return(FALSE);	// 0 is not a valid type
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetStationIDs
//
//	Description:
//	Gets all the Station IDs known to Facility Manager for the specified facility
//	
//	Declaration:
//	BOOL CFacilityConfig::GetStationIDs(short sFacID, IDStruct *pIDs)
//
//	Input:	User must have allocated space for all the station IDStructs -- know how
//			many stations from calling GetNumStations before this call.
//
//	Output:	piFacIDs	Array containing station IDStructs
//
//	Return:	TRUE (found some)/ FALSE (none found)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//	25-Nov-2003	SFK		Modified to only return IDs of station types known to Rad
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetStationIDs(short sFacID, IDStruct *pIDs)
{
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// DCOM requires initialization of all VARIANTS.  Do that here.
	VARIANT List;
	::VariantInit(&List);

	pIFacMgrData->GetStationIDs(sFacID, m_EnabledStationsOnly,  &List);
	
	if(List.vt == VT_EMPTY) 
	{
		MessageBox(NULL, "No STATION IDs were returned or found.", "GetStationIDs: Result", MB_OK | MB_ICONEXCLAMATION);
		::VariantClear(&List);
		return(FALSE);
	}
	else 
	{
		long NumStations;
		
		//Unload the station ID records contents into a string for display.			
		SafeArrayGetUBound(List.parray, 1, &NumStations);
		CString TempStr;
		// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
		// We will explicitly ask for a long here, then convert to a short
		//int iTempStationID;
		long iTempStationID;
		short sTempStationType;			
		int j=0;

		for(LONG i = 0; i <= NumStations; i++) 
		{
			SA_GetElem(List.parray, i, 0, &iTempStationID);		// get station id
			SA_GetElem(List.parray, i, 1, &TempStr);	// get station type (grand, bot, etc.)

			// only return IDs of stations Rad knows about
			//if (!StationTypeStrToNum((const char*)TempStr.GetBuffer(MAX_NAME_LEN), &sTempStationType))
			if (!StationTypeStrToNum((LPCTSTR)TempStr, &sTempStationType))
				continue;

			// Now cast it to a short
			pIDs[j].sStaID = (short)iTempStationID;
			strncpy(pIDs[j].szStaType, TempStr.GetBuffer(MAX_NAME_LEN), MAX_NAME_LEN);
			pIDs[j].szStaType[MAX_NAME_LEN] = '\0';
			pIDs[j].sStaType = sTempStationType;

			if (!StationTypeStrToDataType((const char*)&(pIDs[j].szStaType), &pIDs[j].sDataType)) 
			{
				::VariantClear(&List);
				return (FALSE);
			}
			j++;
		}
	}
	::VariantClear(&List);
	return(TRUE);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetNumRadStations
//
//	Description:
//	Gets all the Station IDs known to Facility Manager for the specified facility
//	and sorts out which ones are of the type known to Rad
//	
//	Declaration:
//	int CFacilityConfig::GetNumRadStations(short sFacID)
//
//	Input:	User must have allocated space for all the station IDStructs -- know how
//			many stations from calling GetNumStations before this call.
//
//	Output:	piFacIDs	Array containing station IDStructs
//
//	Return:	TRUE (found some)/ FALSE (none found)
//	
//  date    /	author	revision
//  -----------------	--------
//	25-Nov-2003	SFK		Created
//	01-Dec 2004	SFK		Check if any data in db for Rad stations
//////////////////////////////////////////////////////////////////
int CFacilityConfig::GetNumRadStations(short sFacID, int *piNumRadChannels)
{
	int iNumRadStations = 0;
	int iNumRadChannels = 0;
	struct IDStruct TempID;

	VARIANT List;

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// DCOM requires initialization of all VARIANTS.  Do that here.
	VariantInit (&List);

	pIFacMgrData->GetStationIDs(sFacID, m_EnabledStationsOnly,  &List);
	
	if(List.vt == VT_EMPTY) {
		MessageBox(NULL, "No STATION IDs were returned or found.", "GetStationIDs: Result", MB_OK | MB_ICONEXCLAMATION);
		return(FALSE);
	}
	else {
		long NumStations;
		int iNumChannels;
		
		//Sort through the station ID records to see which types are for Rad
		SafeArrayGetUBound(List.parray, 1, &NumStations);
		CString TempStr;
		//int iTempStationID;
		long lTempStationID=-1;
		short sTempStationType;			
		int j=0;
		for(LONG i = 0; i <= NumStations; i++) {
			// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
			// We need to yank this out of SA as a long, then convert.

			//SA_GetElem(List.parray, i, 0, &TempID.sStaID);	// get station id
			SA_GetElem(List.parray, i, 0, &lTempStationID);	// get station id
			TempID.sStaID = (short)lTempStationID;
			SA_GetElem(List.parray, i, 1, &TempStr);		// get station type (grand, bot, etc.)
		
			strncpy(TempID.szStaType, TempStr.GetBuffer(MAX_NAME_LEN), MAX_NAME_LEN);
			TempID.szStaType[MAX_NAME_LEN] = '\0';

			// only return IDs of stations Rad knows about
			if (!StationTypeStrToNum(TempID.szStaType, &sTempStationType)) {
				continue;
			}
			TempID.sStaType = sTempStationType;
			iNumRadStations++;
			iNumChannels = GetNumChannelsForStation(sFacID, &TempID);
			iNumRadChannels += iNumChannels;
		}
		SafeArrayDestroy(List.parray);
		*piNumRadChannels = iNumRadChannels;
	}
	return(iNumRadStations);
}

BOOL CFacilityConfig::GetFirstStationID(short sFacID, IDStruct *pID)
{
	VARIANT List;
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// DCOM requires initialization of all VARIANTS.  Do that here.
	VariantInit (&List);

	pIFacMgrData->GetStationIDs(sFacID, m_EnabledStationsOnly,  &List);
	
	if(List.vt == VT_EMPTY) {
		MessageBox(NULL, "No STATION IDs were returned or found.", "GetStationIDs: Result", MB_OK | MB_ICONEXCLAMATION);
		return(FALSE);
	}
	else {
		long NumStations;
		
		//Unload the first station ID records contents into a string for display.			
		SafeArrayGetUBound(List.parray, 1, &NumStations);
		CString TempStr;
		//Explicitly make this a long
		//int iTemp;
		long lTemp;
		short sTempStationType;
		for(LONG i = 0; i <= NumStations; i++) {
			SA_GetElem(List.parray, i, 0, &lTemp);		// 26-Sep-2005 SFK fixed to look at ith entry, not 0th
			SA_GetElem(List.parray, i, 1, &TempStr);	// 26-Sep-2005 SFK fixed to look at ith entry, not 0th
			if (!StationTypeStrToNum((const char*)TempStr.GetBuffer(MAX_NAME_LEN), &sTempStationType)) continue;
			//Explicitly cast to a short
			//pID->sStaID = iTemp;
			pID->sStaID = (short)lTemp;
			strncpy(pID->szStaType, TempStr.GetBuffer(MAX_NAME_LEN), MAX_NAME_LEN);
			pID->szStaType[MAX_NAME_LEN] = '\0';
			if (!StationTypeStrToNum((const char*)&(pID->szStaType), &pID->sStaType)) {
				SafeArrayDestroy(List.parray);
				return (FALSE);
			}
			if (!StationTypeStrToDataType((const char*)&(pID->szStaType), &pID->sDataType)) {
				SafeArrayDestroy(List.parray);
				return (FALSE);
			}
			break;		// have found the first station of the type known to Rad
		}
		SafeArrayDestroy(List.parray);
	}
	return(TRUE);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetStationLongName
//
//	Description:
//	Given the Facility ID and station IDStruct, get the associated long name.
//	Note: Will fill station type string into IDStruct.
//
//	Declaration:
//	void CFacilityConfig::GetStationLongName(short sFacID, struct IDStruct *pID, char *szStaName, int iLen)
//
//	Input:	sFacID		Facility ID number
//			iLen		Maximum space allocated by Rad for facility id name
//
//	Output:	strFacName	Facility long name as a char *
//
//	Return:	None
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//  20-Dec-2005	SFK		Changed to immediately convert BSTR value being returned in the call.
//						pjm: This is part of the fix for DR00189: "Invalid characters in some station names"
//////////////////////////////////////////////////////////////////
/*
void CFacilityConfig::GetStationLongName(short sFacID, struct IDStruct *pID, char *szStaName, int iLen)
{
	USES_CONVERSION;
	
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// Interface call for DCOM facmgr needs a long station ID
	StationTypeNumToStr(pID);		// make sure the station type string is valid
	//strncpy(szStaName, W2A(pIFacMgrData->GetStationLongName(sFacID, pID->sStaID, A2BSTR(pID->szStaType))), iLen-1);
	strncpy(szStaName, W2A(pIFacMgrData->GetStationLongName(sFacID, (LONG)pID->sStaID, A2BSTR(pID->szStaType))), iLen-1);
	szStaName[iLen] = '\0';
}
*/
void CFacilityConfig::GetStationLongName(short sFacID, struct IDStruct *pID, char *szStaName, int iLen)
{
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// Interface call for DCOM facmgr needs a long station ID

	//DR 189: Got rid of the conversion macro.  PJM 9-24-2007
	//USES_CONVERSION;
	StationTypeNumToStr(pID);		// make sure the station type string is valid
	CString cs(pID->szStaType);
	BSTR bstrStaType = cs.AllocSysString();
	BSTR bstrLongName = pIFacMgrData->GetStationLongName(sFacID, (long)pID->sStaID, bstrStaType);
	CString cs1(bstrLongName);
	//strncpy(szStaName, W2A(pIFacMgrData->GetStationLongName(sFacID, pID->sStaID, A2BSTR(pID->szStaType))), iLen-1);
	strncpy(szStaName, (LPCTSTR)cs1, iLen-1);
	szStaName[iLen] = '\0';
	::SysFreeString(bstrStaType);
	::SysFreeString(bstrLongName);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetStationRecord
//
//	Description:
//	Get the record associated with the given Facility ID.
//	Since Rad can't use CStrings, just get the db_sta_rec stuff and forget about the rest
//	
//	Declaration:
//	BOOL CFacilityConfig::GetStationRecord(short sFacID, struct IDStruct *pID, struct db_sta_rec *pdbSta )
//
//	Input:	sFacID		Facility ID number
//			StaOD		IDStruct describing station
//
//	Output:	pdbSta		Standard station record known to Rad
//
//	Return:	TRUE (read a record)/FALSE (no record)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetStationRecord(short sFacID, struct IDStruct *pID, struct db_sta_rec *pdbSta)
{
	VARIANT saStationRecord;

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	//Need a long in the facmgr interface call
	//Also need to initialize all VARIANTS for DCOM FacMgr
	VariantInit (&saStationRecord);
	StationTypeNumToStr(pID);		// make sure the station type string is valid
	//pIFacMgrData->GetStationRecord(sFacID, pID->sStaID, pID->szStaType, &saStationRecord);
	pIFacMgrData->GetStationRecord(sFacID, (LONG)pID->sStaID, pID->szStaType, &saStationRecord);

	if(saStationRecord.vt ==VT_EMPTY)	
	{
		VariantClear(&saStationRecord);
		return(FALSE);
	}

	//Unload the information from the safearray.
	CString TempStr;

	//SA_GetElem(saStationRecord.parray, 0, &pdbSta->s_sta_num_key);
	// HN 11/23/2007 Could this break it?  FacMgr returning a LONG here!!! 
	LONG lTemp= 0;
	SA_GetElem (saStationRecord.parray,0,&lTemp);
	pdbSta->s_sta_num_key = (short) lTemp;

	SA_GetElem(saStationRecord.parray, 1, &pdbSta->s_sta_picklist_num);

	SA_GetElem(saStationRecord.parray, 2, &TempStr);
	strncpy(pdbSta->sz_sta_name, TempStr.GetBuffer(MAX_NAME_LEN), MAX_NAME_LEN);
	pdbSta->sz_sta_name[MAX_NAME_LEN] = '\0';
	
	SA_GetElem(saStationRecord.parray, 3, &TempStr);
	strncpy(pdbSta->sz_sta_short_name, TempStr.GetBuffer(MAX_SHRT_NAME_LEN), MAX_SHRT_NAME_LEN);
	pdbSta->sz_sta_short_name[MAX_SHRT_NAME_LEN] = '\0';

	SA_GetElem(saStationRecord.parray, 4, &TempStr);

	StationTypeStrToNum(TempStr.GetBuffer(MAX_NAME_LEN), &(pdbSta->s_sta_type));

	StationTypeStrToDataType(TempStr.GetBuffer(MAX_NAME_LEN), &(pdbSta->s_data_type));

	//SafeArrayDestroy(saStationRecord.parray);
	VariantClear(&saStationRecord);

	return(TRUE);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	CreateChannelPicklist
//
//	Description:
//	Gets a list with all the Channel names for a facility from Facility Manager
//	
//	Declaration:
//	int CFacilityConfig::CreateChannelPicklist(short sFacID, int iLongNames, char **pAdrs[], int *piMaxNameLen)
//
//	Input:	sFacID			which facility want the channels for
//			bLongNames		whether want a list of long or short names
//
//	Output:	pAdrs			pointers to strings needed by gen_allocate_str_array and GUI picklists			
//			piMaxNameLen	longest name in the picklist
//
//	Return:	TRUE(picklist created)/0(no channels in facility)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//	25-Nov-2003	SFK		Modified to only return channels of station types known to Rad
//	10-Dec-2003	SFK		Modified to only read the information from FacMgr once.
//////////////////////////////////////////////////////////////////
int CFacilityConfig::CreateChannelPicklist(short sFacID, BOOL bLongNames, char **pAdrs[], int *piMaxNameLen)
{
	USES_CONVERSION;  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	long i;
	
	static long lNumRadChans;
	static int iLongNameLen;
	static int iShortNameLen;
	
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// DCOM requires initialization of all VARIANTS.  Do that here.
	VARIANT List;
	VariantInit(&List);

	if (!glbChannelPicklistFilled) 
	{
		//Get a list of all channels known to the FacilityManager for the facility and station given
		pIFacMgrData->GetChannelIDsForFacility(sFacID, VARIANT_TRUE, &List);

		if(List.vt == VT_EMPTY) 
		{
			MessageBox(NULL, "No Channel Records were returned or found.", "GetChannelIDsForFacility: Result", MB_OK | MB_ICONEXCLAMATION);
			VariantClear(&List);
			return(FALSE);
		}

		long lDimension[2] = {0,0};
		long lNumChans;
		iLongNameLen = 0;
		iShortNameLen = 0;
		int iLen;

		//Read out all the channel information and figure out the longest name
		SafeArrayGetUBound(List.parray, 1, &lNumChans);
		lNumChans++;
		InitPicklist(CHANNEL_PICKLIST, lNumChans);	// clear out our local storage array
		//Explicitly pull out a long station ID
		long lStaID;
		short sStaID, sStaType;
		CString StaTypeStr;
		int j=0;

		for(i = 0; i <lNumChans; i++) 
		{
			//SA_GetElem(List.parray, i, 0, &sStaID);				// station id
			SA_GetElem(List.parray, i, 0, &lStaID);				// station id
			sStaID = (short) lStaID;
			SA_GetElem(List.parray, i, 1, &StaTypeStr);			// station type name

			if (!StationTypeStrToNum(StaTypeStr.GetBuffer(MAX_NAME_LEN), &sStaType))
				continue;		// type unknown to Rad

			gllChannelPicklist[j].sID1 = sStaID;
			gllChannelPicklist[j].Type = StaTypeStr;
			gllChannelPicklist[j].sID2 = sStaType;
			SA_GetElem(List.parray, i, 2, &gllChannelPicklist[j].sID3);				// channel id
			SA_GetElem(List.parray, i, 3, &(gllChannelPicklist[j].PicklistLongName));	// channel long name string
			//if (!bLongNames) {
			gllChannelPicklist[j].PicklistShortName = W2T(pIFacMgrData->GetChannelShortName(sFacID, gllChannelPicklist[j].sID1, T2BSTR((LPCTSTR)gllChannelPicklist[j].Type), gllChannelPicklist[j].sID3));
			//}
			gllChannelPicklist[j].sPicklistPosition = j;							// picklist position
			if (!StationTypeNumToDataType(gllChannelPicklist[j].sID2, &gllChannelPicklist[j].sID4)) 
			{
				VariantClear(&List);
				return (FALSE);  // data type
			}
			// calculate the longest name
			iLen = gllChannelPicklist[j].PicklistLongName.GetLength();
			if (iLen > iLongNameLen) 
				iLongNameLen = iLen;
			iLen = gllChannelPicklist[j].PicklistShortName.GetLength();
			if (iLen > iShortNameLen)
				iShortNameLen = iLen;
			j++;
		}
		lNumRadChans = j;
		//SafeArrayDestroy(List.parray);
		glbChannelPicklistFilled = true;
	}

	// Now copy CStrings over to gui picklist format
	//if (AllocStrArray(lNumChans, iNameLen, pAdrs)) {
	//	for(i = 0; i <lNumChans; i++) { 
	int iNameLen;
	if (bLongNames)
		iNameLen = iLongNameLen;
	else
		iNameLen = iShortNameLen;

	if (AllocStrArray(lNumRadChans, iNameLen, pAdrs)) 
	{
		if (bLongNames) 
		{
			for(i = 0; i <lNumRadChans; i++) 
				strcpy ((*pAdrs)[i], gllChannelPicklist[i].PicklistLongName.GetBuffer(iNameLen));
		}
		else 
		{
			for(i = 0; i <lNumRadChans; i++) 
				strcpy ((*pAdrs)[i], gllChannelPicklist[i].PicklistShortName.GetBuffer(iNameLen));
		}
	}

	gllsChannelPicklistItems = (short)i;
	(*pAdrs)[i] = NULL;
	*piMaxNameLen = iNameLen;

	VariantClear(&List);
	return(TRUE);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetChannelIDForPicklistChoice
//
//	Description:
//	Given the picklist choice, return the associated IDStruct for the station
//
//	Declaration:
//	BOOL CFacilityConfig::GetStationIDForPicklistChoice(short sPicklistChoice, IDStruct *pStaID)
//
//	Input:	sPicklistChoice		which picklist position
//
//	Output:	pStaID				IDStruct with station info
//
//	Return:	TRUE (station exists)/FALSE (invlaid picklist choice)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetChannelIDForPicklistChoice(short sPicklistChoice, IDStruct *pChanID)
{
	if (sPicklistChoice > gllsChannelPicklistItems) 
		return(FALSE);			// invalid request

	pChanID->sStaID = gllChannelPicklist[sPicklistChoice].sID1;		// station number
	strncpy(pChanID->szStaType, gllChannelPicklist[sPicklistChoice].Type.GetBuffer(MAX_NAME_LEN), MAX_NAME_LEN); // station type
	pChanID->szStaType[MAX_NAME_LEN] = '\0';
	pChanID->sStaType = gllChannelPicklist[sPicklistChoice].sID2;	// station type number
	pChanID->sChanID = gllChannelPicklist[sPicklistChoice].sID3;	// channel number
	pChanID->sDataType = gllChannelPicklist[sPicklistChoice].sID4;	// channel data type
	return(TRUE);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetPicklistChoiceForChannelID
//
//	Description:
//	Given the Channel IDStruct, determine which picklist position goes with it.
//	
//	Declaration:
//	BOOL CFacilityConfig::GetPicklistChoiceForChannelID(struct IDStruct ChanID, short *psPicklistPosition)
//
//	Input:	StaID		IDStruct describing station
//
//	Output:	psPicklistPosition	associated picklist position (0-based)
//
//	Return:	TRUE (found picklist position)/FALSE (not in picklist)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetPicklistChoiceForChannelID(struct IDStruct ChanID, short *psPicklistPosition)
{
	for (int i=0; i<gllsChannelPicklistItems; i++) {
		if ((gllChannelPicklist[i].sID1 == ChanID.sStaID) && (gllChannelPicklist[i].sID2 == ChanID.sStaType) && (gllChannelPicklist[i].sID3 == ChanID.sChanID)) {
			*psPicklistPosition = gllChannelPicklist[i].sPicklistPosition;
			return(TRUE);
		}
	}
	*psPicklistPosition = gllChannelPicklist[0].sPicklistPosition;		// if nothing matches return the first one
	return(FALSE);		// invalid request
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetPicklistChoicesForStation
//
//	Description:
//	Given the Station IDStruct, determine all the channel in the channel
//	picklist for the station.  If no channels from station, return FALSE.
//	
//	Declaration:
//	BOOL CFacilityConfig::GetPicklistChoicesForStation(struct IDStruct StaID, int* piNum, int *psPicklistChoices)
//
//	Input:	StaID		IDStruct describing station
//
//	Output:	piNum		number of channels from station in the picklist
//			psPicklistChoices allocated by user before call, holds all picklist numbers for channels at station
//
//	Return:	TRUE (found channels in picklist)/FALSE (not in picklist)
//	
//  date    /	author	revision
//  -----------------	--------
//	04-May-2004	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetPicklistChoicesForStation(struct IDStruct StaID, int* piNum, int *psPicklistChoices)
{
	*piNum = 0;
	for (int i=0; i<gllsChannelPicklistItems; i++) {
		if ((gllChannelPicklist[i].sID1 == StaID.sStaID) && (gllChannelPicklist[i].sID2 == StaID.sStaType)) {
			psPicklistChoices[*piNum] = gllChannelPicklist[i].sPicklistPosition;
			(*piNum)++;
		}
		
	}
	if (*piNum > 0) return(TRUE);
	return(FALSE);		// no channels for station
}
///////////////////////////////////////////////////////////////////////////
//	Name:	GetChannelRecord
//
//	Description:
//	Get the record associated with the given Channel IDStruct for the given facility
//	Since Rad can't use CStrings, just get the db_chan_rec stuff and forget about the rest
//	Note:  the station type string will be filled into the IDStruct
//	
//	Declaration:
//	BOOL CFacilityConfig::GetChannelRecord(short sFacID, struct IDStruct *pChanID, struct db_chan_rec *pdbChan )
//
//	Input:	sFacID		Facility ID number
//			StaOD		IDStruct describing station
//
//	Output:	pdbSta		Standard station record known to Rad
//
//	Return:	TRUE (read a record)/FALSE (no record)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetChannelRecord(short sFacID, struct IDStruct *pChanID, struct db_chan_rec *pdbChan )
{
	USES_CONVERSION;

	VARIANT saChanRecord;
	VariantInit (&saChanRecord);

	StationTypeNumToStr(pChanID);		// make sure the station type string is valid
	//pIFacMgrData->GetChannelRecord(sFacID, pChanID->sStaID, A2W(pChanID->szStaType), pChanID->sChanID, &saChanRecord);
	pIFacMgrData->GetChannelRecord(sFacID, (LONG)pChanID->sStaID, A2W(pChanID->szStaType), pChanID->sChanID, &saChanRecord);
	
	if(saChanRecord.vt ==VT_EMPTY)
	{
		VariantClear (&saChanRecord);
		return(FALSE);
	}

	//Unload the information from the safearray.
	CString TempStr;

	SA_GetElem(saChanRecord.parray, 0, &pdbChan->s_chan_num_key);		// channel number

	SA_GetElem(saChanRecord.parray, 1, &pdbChan->s_chan_picklist_num);	// picklist position

	SA_GetElem(saChanRecord.parray, 2, &TempStr);						// long name
	strncpy(pdbChan->sz_chan_name, TempStr.GetBuffer(MAX_NAME_LEN), MAX_NAME_LEN);
	pdbChan->sz_chan_name[MAX_NAME_LEN] = '\0';
	
	SA_GetElem(saChanRecord.parray, 3, &TempStr);						// short name
	strncpy(pdbChan->sz_chan_short_name, TempStr.GetBuffer(MAX_SHRT_NAME_LEN), MAX_SHRT_NAME_LEN);
	pdbChan->sz_chan_short_name[MAX_SHRT_NAME_LEN] = '\0';

	SA_GetElem(saChanRecord.parray, 4, &TempStr);						// units
	strncpy(pdbChan->sz_chan_units, TempStr.GetBuffer(MAX_NAME_LEN), MAX_NAME_LEN);
	pdbChan->sz_chan_units[MAX_NAME_LEN] = '\0';

	SA_GetElem(saChanRecord.parray, 5, &TempStr);						// units
	strncpy(pdbChan->sz_chan_short_units, TempStr.GetBuffer(MAX_SHRT_NAME_LEN), MAX_SHRT_NAME_LEN);
	pdbChan->sz_chan_short_units[MAX_SHRT_NAME_LEN] = '\0';

	SA_GetElem(saChanRecord.parray, 6, &pdbChan->s_chan_offset);	// Chan Offset

	VariantClear (&saChanRecord);
	return(TRUE);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	IsChannelEnabled
//
//	Description:
//	Given the channel information in IDStruct, return whether or not the
//	channel is enabled.
//	Note: Will fill in station type string in IDStruct
//
//	Declaration:
//	BOOL CFacilityConfig::IsChannelEnabled(short sFacID, struct IDStruct ChanID)
//
//	Input:	sFacID		which facility
//			ChanID		IDStruct describing channel
//
//	Output:	none
//
//	Return:	TRUE (enabled)/FALSE (not enabled)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::IsChannelEnabled(short sFacID, struct IDStruct *pChanID)
{
	USES_CONVERSION;
	StationTypeNumToStr(pChanID);		// make sure the station type string is valid
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// use a long for the station ID
	//if (!pIFacMgrData->IsChannelDefined(sFacID, pChanID->sStaID, A2W(pChanID->szStaType), pChanID->sChanID)) return(FALSE);
	if (!pIFacMgrData->IsChannelDefined(sFacID, (LONG)pChanID->sStaID, A2W(pChanID->szStaType), pChanID->sChanID)) return(FALSE);
	//return(pIFacMgrData->IsChannelEnabled(sFacID, pChanID->sStaID, A2W(pChanID->szStaType), pChanID->sChanID));
	return(pIFacMgrData->IsChannelEnabled(sFacID, (LONG)pChanID->sStaID, A2W(pChanID->szStaType), pChanID->sChanID));
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetFirstChannelID
//
//	Description:
//	Get the ID of the first channel for the facility known to Facility Manager
//	
//	Declaration:
//	BOOL CFacilityConfig::GetFirstChannelID(short sFacNum, struct IDStruct *pChanID)
//
//	Input:	None
//
//	Output:	pChanID		IDStruct for first channel in facility
//
//	Return:	TRUE (a channel exists)/FALSE (no channels defined)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetFirstChannelID(short sFacNum, struct IDStruct *pChanID)
{
	VARIANT List;
	VariantInit (&List);

	pIFacMgrData->GetChannelIDsForFacility(sFacNum, true, &List);

	BOOL bFound;

	if(List.vt == VT_EMPTY) 
	{
		MessageBox(NULL, "No Channel IDs were found.", "GetFirstChannelID: Result", MB_OK | MB_ICONEXCLAMATION);
		pChanID->sChanID = -1;
		VariantClear(&List);
		return(FALSE);
	}
	else
	{
		long lDimension[2] = {0,0};
		long lNumChans;
		int iNameLen = 0;

		//Read out channel information for the first entry
		SafeArrayGetUBound(List.parray, 1, &lNumChans);
		lNumChans++;
		CString TempStr;
		//Use a long station id to get out of the SA
		//short sStaID,sStaType;
		short /*sStaID,*/sStaType;
		long lStaID;
		bFound = FALSE;

		for (int i=0; i<lNumChans; i++) 
		{
			//SA_GetElem(List.parray, 0, 0, &sStaID);			// station id
			SA_GetElem(List.parray, 0, 0, &lStaID);			// station id
			SA_GetElem(List.parray, 0, 1, &TempStr);					// station type name
			//if (!StationTypeStrToNum(TempStr.GetBuffer(MAX_NAME_LEN), &sStaType)) 
			if (!StationTypeStrToNum((LPCTSTR)TempStr, &sStaType))
				continue;		// type unknown to Rad

			pChanID->sStaID = (short)lStaID;
			//strncpy(pChanID->szStaType, TempStr.GetBuffer(MAX_NAME_LEN), MAX_NAME_LEN);
			strncpy(pChanID->szStaType, (LPCTSTR)TempStr, MAX_NAME_LEN);
			pChanID->szStaType[MAX_NAME_LEN] = '\0';

			if ((!StationTypeStrToNum(pChanID->szStaType, &pChanID->sStaType)) ||		// type number
				(!StationTypeStrToDataType(pChanID->szStaType, &pChanID->sDataType))) 	// data type
			{
				VariantClear(&List);
				return (FALSE);  

			}
			SA_GetElem(List.parray, 0, 2, &pChanID->sChanID);			// channel id
			bFound = TRUE;
			break;		// found a channel of a station type known to Rad
		}
		//if (!bFound) return(FALSE);
	}

	VariantClear(&List);
	return bFound;
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetChannelIDs
//
//	Description:
//	Gets all the Channel IDs known to Facility Manager for the specified station
//	
//	Declaration:
//	BOOL CFacilityConfig::GetChannelIDs(short sFacID, IDStruct *pIDs)
//
//	Input:	User must have allocated space for all the channel IDStructs -- know how
//			many channels from calling GetNumChannels before this call.
//			User must have also filled in station number and station type in the IDStructs
//
//	Output:	piFacIDs	Array containing channel IDStructs
//
//	Return:	TRUE (found some)/ FALSE (none found)
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL CFacilityConfig::GetChannelIDs(short sFacID, IDStruct *pIDs)
{
	VARIANT List;
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// DCOM requires initialization of all VARIANTS, do it here.
	VariantInit (&List);
	
	StationTypeNumToStr(&(pIDs[0]));		// make sure the station type string is valid
	//Explicitly call to FacMGR with a long station ID
	//pIFacMgrData->GetChannelIDs(sFacID, pIDs[0].sStaID, pIDs[0].szStaType, true,  &List);
	pIFacMgrData->GetChannelIDs(sFacID, (LONG)pIDs[0].sStaID, pIDs[0].szStaType, true,  &List);

	BOOL bReturn = FALSE;
	
	if(List.vt == VT_EMPTY) 
	{
		MessageBox(NULL, "No Channel IDs were returned or found.", "GetChannelIDs: Result", MB_OK | MB_ICONEXCLAMATION);
		//VariantClear(&List);
		//return(FALSE);
	}
	else 
	{
		long NumItems;
		
		//Unload the channel ID records contents into a string for display.			
		SafeArrayGetUBound(List.parray, 1, &NumItems);
		//CString TempStr;
		int iTemp;
		for(LONG i = 0; i <= NumItems; i++) 
		{
			SA_GetElem(List.parray, i, 0, &iTemp);		// get the channel id
			pIDs[i].sChanID = iTemp;
		}
		bReturn = TRUE;
		//SafeArrayDestroy(List.parray);
	}
	VariantClear(&List);
	return bReturn;
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetChannelLongName
//
//	Description:
//	Given the Facility ID and channel IDStruct, get the associated long name.
//	Note: Will fill station type string into IDStruct.
//
//	Declaration:
//	void CFacilityConfig::GetChannelLongName(short sFacID, struct IDStruct *pID, char *szChanName, int iLen)
//
//	Input:	sFacID		Facility ID number
//			pID			IDStruct describing channel
//			iLen		Maximum space allocated by Rad for name
//
//	Output:	strChanName	Facility long name as a char *
//
//	Return:	None
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//  20-Dec-2005 SFK		Immediately convert the returned BSTR value.
//						pjm: This is part of the fix for DR00189: "Invalid characters in some station names"
//////////////////////////////////////////////////////////////////
void CFacilityConfig::GetChannelLongName(short sFacID, struct IDStruct *pID, char *szChanName, int iLen)
{
	//DR 189: Got rid of the conversion macro.  PJM 9-24-2007

	StationTypeNumToStr(pID);		// make sure the station type string is valid
	CString cs0(pID->szStaType);
	BSTR bstrStaType = cs0.AllocSysString();

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// use a long in the interface call to FacMGr.
	BSTR bstrLongName = pIFacMgrData->GetChannelLongName(sFacID, (long)pID->sStaID, bstrStaType, pID->sChanID);

	CString cs1(bstrLongName);
	strncpy(szChanName, LPCTSTR(cs1), iLen-1);
	szChanName[iLen] = '\0';
	SysFreeString(bstrStaType);
	SysFreeString(bstrLongName);
}
/*
void CFacilityConfig::GetChannelLongName(short sFacID, struct IDStruct *pID, char *szChanName, int iLen)
{
	USES_CONVERSION;
	StationTypeNumToStr(pID);		// make sure the station type string is valid
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// use a long in the interface call to FacMGr.
	
	//strncpy(szChanName, W2A(pIFacMgrData->GetChannelLongName(sFacID, pID->sStaID, A2BSTR(pID->szStaType), pID->sChanID)), iLen-1);
	strncpy(szChanName, W2A(pIFacMgrData->GetChannelLongName(sFacID, (LONG)pID->sStaID, A2BSTR(pID->szStaType), pID->sChanID)), iLen-1);
	szChanName[iLen] = '\0';
}
*/
// this routine is only called based on station and my other FacilityConfig routines only return
// valid Rad stations.  So the number requested will always be in sync with only Rad type stations.
int CFacilityConfig::GetNumChannelsForStation(short sFacID, struct IDStruct *pStaID)
{
	StationTypeNumToStr(pStaID);		// make sure the station type string is valid
	//Get the number of enabled channels for the station.
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// use a long in the interface call to FacMGr.
	return  pIFacMgrData->GetNumberOfChannelsForStation(sFacID, (LONG)pStaID->sStaID, pStaID->szStaType, true);
}

void CFacilityConfig::InitPicklist(int iType, int iNumItems)
{
	int i;

	switch(iType) {
	case FACILITY_PICKLIST:
		// fill the picklist info stuff with -1
		for (i=0; i<iNumItems; i++) {
			gllFacilityPicklist[i].sID1 = -1;
			gllFacilityPicklist[i].sID2 = -1;
			gllFacilityPicklist[i].sID3 = -1;
			gllFacilityPicklist[i].sPicklistPosition = -1;
			gllFacilityPicklist[i].Type.Empty();
			gllFacilityPicklist[i].PicklistLongName.Empty();
			gllFacilityPicklist[i].PicklistShortName.Empty();
		}
		gllsFacilityPicklistItems = 0;
		break;
	case STATION_PICKLIST:
		// fill the picklist info stuff with -1
		for (i=0; i<iNumItems; i++) {
			gllStationPicklist[i].sID1 = -1;
			gllStationPicklist[i].sID2 = -1;
			gllStationPicklist[i].sID3 = -1;
			gllStationPicklist[i].sPicklistPosition = -1;
			gllStationPicklist[i].Type.Empty();
			gllStationPicklist[i].PicklistLongName.Empty();
			gllStationPicklist[i].PicklistShortName.Empty();
		}
		gllsStationPicklistItems = 0;
		break;
	case CHANNEL_PICKLIST:
		// fill the picklist info stuff with -1
		for (i=0; i<iNumItems; i++) {
			gllChannelPicklist[i].sID1 = -1;
			gllChannelPicklist[i].sID2 = -1;
			gllChannelPicklist[i].sID3 = -1;
			gllChannelPicklist[i].sPicklistPosition = -1;
			gllChannelPicklist[i].Type.Empty();
			gllChannelPicklist[i].PicklistLongName.Empty();
			gllChannelPicklist[i].PicklistShortName.Empty();
		}
		gllsChannelPicklistItems = 0;
		break;
	}
}


void CFacilityConfig::DestroyPicklist(char** pAdrs)
{
	DeAllocStrArray(pAdrs);
}



bool CFacilityConfig::GetChannelIndexFromID(short sFacID, long lStationID, short sChanID, short *sIndex)
{
	VARIANT saChanRecord;
	VariantInit (&saChanRecord);

	pIFacMgrData->GetChannelRecord (sFacID,lStationID,"EOSS",sChanID,&saChanRecord);
	if(saChanRecord.vt ==VT_EMPTY)
	{
		VariantClear (&saChanRecord);
		return false;
	}

	//Unload the information from the safearray.
	//CString TempStr;
	SA_GetElem(saChanRecord.parray, 1, sIndex);	// picklist position

	VariantClear (&saChanRecord);
	return true;

}


bool CFacilityConfig::GetChannelIDFromIndex(short sFacID, long lStationID, short sChanIndex, short *sChanID)
{
	VARIANT saChanRecord;
	VariantInit (&saChanRecord);

	pIFacMgrData->GetChannelIDs (sFacID,lStationID,"EOSS",VARIANT_TRUE,&saChanRecord);
	
	if(saChanRecord.vt != VT_EMPTY) 
	{
		SA_GetElem(saChanRecord.parray, sChanIndex, 0, sChanID);		// get the channel id
		VariantClear(&saChanRecord);
		return true;	
	}
	
	VariantClear (&saChanRecord);
	return false;

}

int CFacilityConfig::GetEnabledChansForStation(short sFacilityID,long lStationID,char* csType)
{
	int i = 0;

	VARIANT vChannels;
	VariantInit (&vChannels);

	CString csTemp (csType);
	BSTR bstr = csTemp.AllocSysString();

	pIFacMgrData->GetChannelIDs (sFacilityID,lStationID,bstr,VARIANT_TRUE,&vChannels);
	if (vChannels.parray)
	{
		long lUBound, lLBound;
		SafeArrayGetLBound (vChannels.parray,1,&lLBound);
		SafeArrayGetUBound (vChannels.parray,1,&lUBound);
		i = lUBound - lLBound + 1;
	}

	SysFreeString(bstr);
	VariantClear (&vChannels);
	return i;
}

