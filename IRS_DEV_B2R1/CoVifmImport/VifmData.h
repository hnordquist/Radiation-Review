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
