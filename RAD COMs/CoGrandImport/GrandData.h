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
// GrandData.h: interface for the CGrandData class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "VistaBaseData.h"
#include "TypeDefinitions.h"

#define NUM_RAW_DATA_PTS_IN_MULTIPLE_DB_RECORD 78
//#define GRAND_NUM_DATA_CHANS	18			// 11-Aug-2005 increased from 15 to 18 for additional status bits

#define SATURATED_BIT 0x00040000	// 11-Aug-2005 SFK Added definition for bit to set in status word
#define INVALID_G_BIT 0x00020000	// Aug-2005 SFK Added definition for bit to set in status word
#define OFFSET_BIT 0x00010000		// 30-Jun-2005 SFK Added definition for bit to set in status word


#pragma pack(1)
//  each data point in a *.bid file is one of these
	struct GRAND_DATA_PT {
	    unsigned long  ulJulianTime;	// date in julian seconds
	    unsigned short usGrandStatus;   // status byte for record
	    float fA;		     			// neutron a counts
	    float fB;				     	// neutron b counts
	    float fC;				     	// neutron c counts
	    float fG1;				     	// gamma 1
	    float fG1u;				     	// gamma 1 unc
	    float fG2;				     	// gamma 2
	    float fG2u;				     	// gamma 2 unc
	    unsigned short usElapsedTime;	// time elapsed
	};

	// BI0 DMG files have a duality inherent in thier inner being
struct DMGGRAND_DATA_PT_HDR {
	unsigned long  ulJulianTime;	// date in julian
	unsigned short usJSCS;			// additive checksum for time
	BYTE Len, OpCode;		     	// length in bytes of next data block, record type opcode (MG Acquire records are 0x15, and all others are to be skipped)
};

typedef struct ulf {
	union {
		unsigned __int32	ul;		//4 bytes
		float	f;		
	};
} ulfpair;

struct DMGGRAND_DATA_PT_BDY {
	unsigned short usGrandStatus;   // status byte for record
	ulfpair A, B, C;
	ulfpair G1, G1u;
	ulfpair G2, G2u;
	unsigned long usElapsedTime;	// time elapsed
	unsigned short usDCS;			// additive checksum over data
};
#pragma pack(8)

/////////////////////////////////////////////////////////////////////////////
// CGrandDbDataPt - each single data point retrieved from the dbVista database
//				  put into one of these
class CGrandDbDataPt : public CObject 
{
public:
	DATE			mdJulianTime;		// date in DATE
	unsigned long	mulStatus; 			// status byte for record
	float			mfData[GRAND_NUM_DATA_CHANS];	// data value
	double			mdElapsedTime;  	// time elapsed in secs or fractional secs
};
		                                
class CGrandData : public CBaseVistaData  
{
public:
	CGrandData(CDbVista* pDb, const int iStaNum, const bool bInterpolated,bool bQuietMode);
	~CGrandData();

	bool AddData(CString* pstrErrorMsg);
	bool CreateDay(DATE dDay);
	bool GetDataStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrorMsg = NULL);
	bool GetChannelData(const int &iChanOffset, const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, double *pdTimestamp, double *pdData,  CString *pstrErrorMsg=NULL);
	bool GetMultipleDbRecord(DATE dTime, int* piIndex, struct db_float_data_rec *pdbfData, SNAP eSnap);
	bool GetStationData(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, CString *pstrErrorMsg = NULL);
	bool StationDataExists();

	CGrandDbDataPt* mpGrandDataPtArray;
	struct db_float_data_rec mdbFloatDataRec;
	bool mbFillingExistingRec;
	//bool m_bQuietMode;	

protected:
	void NewDbVistaError(CString *pstrMsg, bool bShowMsg, int status);

};

