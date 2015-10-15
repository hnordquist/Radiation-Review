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

