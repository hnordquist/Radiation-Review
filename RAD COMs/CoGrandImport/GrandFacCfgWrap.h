// GrandFacCfgWrap.h: interface for the GrandFacCfgWrap class.

#pragma once

#include "BaseFacCfgWrap.h"  //base class
#include "db_rmon.h"

class CGrandFacCfgWrap : public CBaseFacCfgWrap 
{
public:
	CGrandFacCfgWrap();
	bool GetStationRecord(short sFacDbNum, long lSta, void *pGenericDBStaRec);
};


	//These functions use the base class implementation


	//CString GetFacilityDirectory(short sFacID);
	//int GetNumStations(short sFacID);
	//bool GetStationIDs(short sFacID, short *psNum, long* pIDs);
	//bool FacilityExists(short sFacilityID);


