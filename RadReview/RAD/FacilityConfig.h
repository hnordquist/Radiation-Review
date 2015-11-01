// FacilityConfig.h: interface for the CFacilityConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACILITYCONFIG_H__CDD5D7F9_239E_452D_83AC_B721EC40A40B__INCLUDED_)
#define AFX_FACILITYCONFIG_H__CDD5D7F9_239E_452D_83AC_B721EC40A40B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "database\db_rmon.h"
#include "database\db_main.h"

#define TYPE_NOT_DEFINED    0 
#define STATION_NOT_DEFINED  -1 
#define CHANNEL_NOT_DEFINED -1 
#define FACILITY_NOT_DEFINED -1 

#define MAX_NAME_LEN 39			// must be same as DB_NAME_LEN in raddefs.h
#define MAX_SHRT_NAME_LEN 9	// must be same as DB_SHORT_NAME_LEN in raddefs.h

#include "comdatastructures.h"

class CFacilityConfig  
{
public:
	int GetEnabledChansForStation (short sFacilityID,long lStationID,char* csType);
	bool GetChannelIDFromIndex (short sFacID, long lStationID, short sChanIndex, short* sChanID);
	bool GetChannelIndexFromID (short sFacID,long lStationID,short sChanID,short* sIndex);
	// facility support
	int CreateFacilityPicklist(char **pAdrs[]);
	void GetFacilityDirectory(short sFacID, char *strFacDirectory, int iLen);
	short GetFacilityID(const char *strFacilityLongName);
	BOOL GetFacilityIDForPicklistChoice(short sPicklistChoice, short *psFacID);
	void GetFacilityLongName(short sFacID, char *szFacName, int iLen);
	BOOL GetFirstFacilityID(short *psFacNum);
	BOOL GetFacilityIDs(int *piFacIDs);
	BOOL GetFacilityRecord(short sFacID, struct db_fac_rec *pdbFac);
	int GetNumFacilities();
	BOOL GetPicklistChoiceForFacilityID(short sFacID, short *psPicklistPosition);
	BOOL IsFacilityDefined(short sFacID);
	BOOL DoesFacilityHaveAnyData(short sFacID);

	// station support
	int CreateStationPicklist(short sFacID, BOOL bAllStations, char **pAdrs[], int *piMaxNameLen);
	BOOL GetFirstStationID(short sFacID, IDStruct *pID);
	int GetNumStations(short sFacID);
	int GetNumRadStations(short sFacID, int *piNumRadChannels);
	BOOL GetPicklistChoiceForStationID(struct IDStruct StaID, short *psPicklistPosition);
	char * GetStationDataType(short sFacID, struct IDStruct *pID);
	BOOL GetStationID(short sFac, char *szStaLongName, struct IDStruct *pStaID);
	BOOL GetStationID(short sFac, struct IDStruct *pStaID);
	BOOL GetStationIDForPicklistChoice(short sPicklistChoice, struct IDStruct *pStaID);
	BOOL GetStationIDs(short sFacID, struct IDStruct *pIDs);
	void GetStationLongName(short sFacID, struct IDStruct *pID, char *szStaName, int iLen);
	BOOL GetStationRecord(short sFacID, struct IDStruct *pID, struct db_sta_rec *pdbSta);
	BOOL GetStationTypeName(short sFac, short sType, int iLen, char *pstrType);
	short GetStationTypeNum(short sFac, char *szFileExtension);
	BOOL IsStationEnabled(short sFacID, struct IDStruct *pStaID);
	BOOL IsTimeAlignStationActiveForThisStation(long id);

	// channel support
	int CreateChannelPicklist(short sFacID, BOOL bLongNames, char **pAdrs[], int *piMaxNameLen);
	BOOL GetChannelIDForPicklistChoice(short sPicklistChoice, IDStruct *pChanID);
	BOOL GetChannelIDs(short sFacID, IDStruct *pIDs);
	void GetChannelLongName(short sFacID, struct IDStruct *pID, char *szChanName, int iLen);
	BOOL GetChannelRecord(short sFacID, struct IDStruct *pChanID, struct db_chan_rec *pdbChan);
	BOOL GetFirstChannelID(short sFacNum, struct IDStruct *pChanID);
	int GetNumChannelsForFacility(short sFacID);
	int GetNumChannelsForStation(short sFacID, struct IDStruct *pStaID);
	BOOL GetPicklistChoiceForChannelID(struct IDStruct ChanID, short *psPicklistPosition);
	BOOL IsChannelEnabled(short sFacID, struct IDStruct *pChanID);
	BOOL GetPicklistChoicesForStation(struct IDStruct StaID, int* piNum, int *psPicklistChoices);


	// general support
	CFacilityConfig();
	virtual ~CFacilityConfig();
	void ShowUnitsManagerDlg();
	void ShowSensorManagerDlg();
	void ShowFacilityConfigDlg();
	void DestroyPicklist(char** pAdrs);

	// member variables
	BOOL m_EnabledFacilitiesOnly;
	BOOL m_OrderFacilitiesByPriority;
	BOOL m_EnabledStationsOnly;
	BOOL m_OrderStationsByPriority;
private:
	BOOL StationTypeStrToNum(const char *strType, short *pTypeNum);
	void InitPicklist(int iType, int iNumItems);
	BOOL StationTypeNumToDataType(short sType, short *pDataTypeNum);
	BOOL StationTypeStrToDataType(const char *strType, short *pDataTypeNum);
	BOOL StationTypeNumToStr(struct IDStruct *pStaID);

};

	extern CFacilityConfig *pglFacCfg;

#endif // !defined(AFX_FACILITYCONFIG_H__CDD5D7F9_239E_452D_83AC_B721EC40A40B__INCLUDED_)
