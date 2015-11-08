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
// VifmData.h: interface for the CVifmData class.
#ifndef VIFMDATA_H
#define VIFMDATA_H

#include "VistaBaseData.h"
#include "TypeDefinitions.h"
#define NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD 234
//#define NUM_DATA_CHANS	8		// 23-Aug-2005 SFK Updated to be consistent with number in db_vifm.h



#pragma pack(1)
	struct VIFM_DATA_PT {
	    unsigned long  ulJulianTime;	// date in julian
	    unsigned short usVifmStatus;   // status byte for record
	    float fD1;		     			
	    float fD2;				     	
	    float fD3;				     	
	    float fD4;				     	
	    float fD5;				     	
	    float fD6;				     	
	    float fD7;				     	
		float fD8;				     	
	    unsigned short usElapsedTime;	// time elapsed
	};
	struct VIFM_DATA_PT1 {
	    double  dJulianTime;	// date in julian
	    unsigned short usVifmStatus;   // status byte for record
	    float fD1;		     			
	    float fD2;				     	
	    float fD3;				     	
	    float fD4;				     	
	    float fD5;				     	
	    float fD6;				     	
	    float fD7;				     	
		float fD8;				     	
	    double dElapsedTime;	// time elapsed
	};
#pragma pack(8)

/////////////////////////////////////////////////////////////////////////////
// CVifmDbDataPt - each single data point retrieved from the dbVista database
//				  put into one of these
class CVifmDbDataPt : public CObject 
{
public:
	DATE			mdJulianTime;		// date in DATE
	unsigned long	mulStatus; 			// status byte for record
	float			mfData[VIFM_NUM_DATA_CHANS];	// data value
	double			mdElapsedTime;  	// time elapsed in secs or fractional secs
};

class CVifmData : public CBaseVistaData  
{
public:

	CVifmData(
		CDbVista* pDb, 
		const int iStaNum, 
		const bool bInterpolated, 
		const bool bQuietMode);

	~CVifmData();

	bool AddData(CString* pstrErrorMsg);
	bool CreateDay(DATE dDay);
	bool GetDataStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrorMsg = NULL);
	bool GetChannelData(const int &iChanOffset, const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, double *pdTimestamp, double *pdData,  CString *pstrErrorMsg=NULL);
	bool GetMultipleDbRecord(DATE dTime, int* piIndex, struct db_float_data_rec *pdbfData, SNAP eSnap);
	bool GetStationData(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, CString *pstrErrorMsg = NULL);
	bool StationDataExists();

	CVifmDbDataPt* mpVifmDataPtArray;
	struct db_float_data_rec mdbFloatDataRec;
	bool mbFillingExistingRec;
	const bool m_bShowMsg; //Always true for B2R1. NDAR uses it for real in other components.	
};

#endif // VIFMDATA_H
