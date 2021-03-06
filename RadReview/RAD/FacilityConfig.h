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
