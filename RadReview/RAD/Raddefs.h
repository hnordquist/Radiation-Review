/* RADDEFS.H   definitions */

#ifndef RADDEFS_H
#define RADDEFS_H

#include "TypeDefinitions.h"
//#include "ComDataStructures.h"
#include "TextFile.h"
#include "upperlayer/TxtWrDb.h"
#include "FacilityConfig.h"

/*********************************************************************** 
*	Global Definitions									    
************************************************************************/
#define DT_LEN 12		// hh:mm:ss.mmm or yyyy.mm.ss	
#define STEVES_DT_LEN 25
#define RAW_DATA_RECS_PER_DB_RECORD 50
#define MAX_LINE_LEN 255	

#define YEAR_2000	1514764800 // Jan 1, 2000 in julian secs

#define MAX_DB_FACS		5				// 5 = maximum facilities allowed for in the db
#define MAX_DB_STAS		MAX_DB_FACS*10	// 50 = maximum stations allowed for in the db
#define MAX_DB_CHANS	MAX_DB_STAS*7	// 350 = maximum channels allowed for in the db

#define DRIVE_LEN	 	2
#define PATH_LEN	 	256
#define WILDCARD_LEN	4

#define DOSNAME_LEN	49
#define DB_NAME_LEN		39
#define DB_SHORT_NAME_LEN 9	// including NULL

#define PATHS 			10000
#define WILDCARDS		100

// types of directories that fit under the general architecture
#define DATA_DIR 1
#define CONFIG_DIR 2
#define LOG_DIR 3
#define ROOT_DIR 4

#define ANY_STATION -1
#define ANY_TYPE -1
	
//#define NUM_DATA_CHANS 18	// 10-Jan-2006 SFK Modify to support extra GRAND chans for gammas    
							// SCR00217
	
#define FIRST 0
#define NEXT 1
	
#define READ 0
#define WRITE 1
#define DELETE_REC 2

#define READ_FIRST 2
#define READ_NEXT 3
#define WRITE_FIRST 4
#define WRITE_NEXT 5
	
#define SEARCH_START 0
#define SEARCH_END 1

#define LOCATION 1
#define DIRECTION 2
		
#define MAX_PAIRS 4 // hardwired to Rokkasho

// define databases
#define DFLT_DB	1
#define OP_DB	2
		           
// event search algorithm types
#define SIMPLE_THRESHOLD 0
#define SLIDING_WINDOW 1
#define SLIDING_WINDOW_WITH_LOCAL_BKG 2
//#define RELATIVE_THRESHOLD 3

// types of names
#define iSHORT_NAME 0
#define iLONG_NAME 1

// types of ratio parts
#define iNUMERATOR 0
#define iDENOMINATOR 1
#define iNONE 2

#define CANCELED 10

#define ID_LEN 12

// these definitions correspond to the positions in the gllSummaryTypePicklist				    
#define ALL_EVENT_SUMMARIES 0
#define DAILY_SUMMARIES 1

// these definitions correspond to the positions in the gllSummarySortPicklist 
#define TIME_SORT 0
#define CHANNEL_SORT 1


// types of match types
#define MATCH_NUMBER	0
#define MATCH_STRING	1

// types of match items
#define FACILITY_MATCH	0
#define STATION_MATCH	1
#define CHANNEL_MATCH	2
#define NO_SEND_LOCATION_MATCH	3

// define some types for names from the facility configurator for in memory storage
#define MAX_LONG_NAME_LEN 40
#define MAX_SHORT_NAME_LEN 10 
typedef char MaxLongNameType[MAX_LONG_NAME_LEN];
typedef char MaxShortNameType[MAX_SHORT_NAME_LEN];

/*********************************************************************** 
*	Data Offset Limits
************************************************************************/
#define MIN_CHAN_OFFSET	1
#define MAX_CHAN_OFFSET	7
		                                     
/*********************************************************************** 
*	GRAND Offsets
************************************************************************/
#define NEUTRON_A_OFFSET	0
#define NEUTRON_B_OFFSET	1
#define NEUTRON_C_OFFSET	2
#define GAMMA_1_OFFSET		3
#define GAMMA_2_OFFSET		4
	                                     
/*********************************************************************** 
*	SR Offsets
************************************************************************/
#define TOTALS_OFFSET		0
#define REALS_OFFSET		1
#define ACCIDENTALS_OFFSET	2
#define TOTALS2_OFFSET		3
#define TOTALS3_OFFSET		4
	                                     
/*********************************************************************** 
*	SR Analysis Types
************************************************************************/
#define REALS_ANALYSIS		0
#define TOTALS_ANALYSIS		1

/*********************************************************************** 
*	Direction Search Types
************************************************************************/
#define START_TO_START		0
#define START_TO_END		1
#define END_TO_START		2
#define END_TO_END			3	 
#define MAX_TO_MAX			4

/*********************************************************************** 
*	Direction Search Types
************************************************************************/
#define IN_DIRECTION	0
#define OUT_DIRECTION	1
#define NO_DIRECTION	2
#define BOTH_DIRECTIONS	3	                           


/*********************************************************************** 
*	Event types
*	These are the types of events that can be entered into the dbVista
*	database in the db_event_rec.  These values reside in the
*	us_event_type member.  The types of CHANNEL_EVENT, MEASUREMENT_EVENT
*	and BACKGROUND_EVENT can be displayed on the graph.  The other types
*	are reserved for program expansion to be used with other Review programs.
************************************************************************/
#define UNKNOWN_EVENT			0
#define NOT_RECONCILED_EVENT	1
#define RECONCILED_EVENT		2
#define UNEXPLAINED_EVENT		3 
#define MISSING_DATA_EVENT		4                                        
#define CHANNEL_EVENT			5
#define MEASUREMENT_EVENT		6
#define BACKGROUND_EVENT		7
#define ALL_EVENTS				8
#define GRAPH_EVENTS			9	// this is not stored in the record, used only for requesting
									// measurements, channel or normal background events
#define STATION_EVENTS			10	// not stored in record, just a way to request a count by station
/*********************************************************************** 
*	Measurement types
*	This is the second level of discrimation on the types of events in the
*	dbVista database for the db_event_rec.  If the db_event_rec 
*	us_event_type member is MEASUREMENT_EVENT, then us_event_type1 can be
*	any of the following types.  For SR data, the types of UNKNOWN_MEAS,
*   LOW_BKG_MEAS, HIGH_BKG_MEAS, EMPTY_MEAS, ASSAY_MEAS, CF_CHECK_MEAS,
*   CF_CHECK_NORM_MEAS, NORMAL_BKG_MEAS and TOTALS_MEAS apply.  For GRAND
*	data, MII_MEAS applies.
************************************************************************/
#define UNKNOWN_MEAS		0		// a type of MEASUREMENT_EVENT
#define LOW_BKG_MEAS		1		// a type of BACKGROUND_EVENT
#define HIGH_BKG_MEAS		2		// a type of BACKGROUND_EVENT
#define EMPTY_MEAS			3		// a type of MEASUREMENT_EVENT
#define ASSAY_MEAS			4		// a type of MEASUREMENT_EVENT
#define CF_CHECK_MEAS		5		// a type of MEASUREMENT_EVENT
#define CF_NORM_MEAS		6		// a type of MEASUREMENT_EVENT
#define NORMAL_BKG_MEAS		7		// a type of BACKGROUND_EVENT
#define TOTALS_MEAS			8		// a type of MEASUREMENT_EVENT
#define MII_MEAS			9
#define CHAN_MEAS			10		// added 13-Dec-2004

// types of measurement picklists that can be requested
#define EXPORT_LIST		0
#define GRAPH_LIST		1
#define SUMMARY_LIST	2
#define SR_LIST -1

//note:  the choices are linked to the gllszlongrealslist in the evntlist.cpp file.
// If you change any item in the picklists in the evntlist.cpp file, be sure to 
// modify the accompanying definitions in the raddefs.h file.  
// This is messy and should be optimized at a later time.
#define SUMM_ASSAY_PL_CHC			0
#define SUMM_CF_NORM_PL_CHC			1
#define SUMM_NORM_BKG_PL_CHC		2
#define SUMM_TOTALS_ASSAY_PL_CHC	3
#define SUMM_CF_CHECK_PL_CHC		4
#define SUMM_EMPTY_ASSAY_PL_CHC		5
#define SUMM_LOW_BKG_PL_CHC			6
#define SUMM_HIGH_BKG_PL_CHC		7
#define SUMM_ALL_CHC				8

#define REALS_ASSAY_PL_CHC			0
#define REALS_CF_NORM_PL_CHC		1
#define REALS_NORM_BKG_PL_CHC		2
#define REALS_CF_CHECK_PL_CHC		3
#define REALS_EMPTY_ASSAY_PL_CHC	4
#define REALS_LOW_BKG_PL_CHC		5
#define REALS_HIGH_BKG_PL_CHC		6
#define REALS_CHAN_PL_CHC			7


#define TOTALS_ASSAY_PL_CHC			0
#define TOTALS_BKG_PL_CHC			1	// 16-Dec-2004 SFK Added bkg choice, renumbered choices
#define TOTALS_CHAN_PL_CHC			2

#define GRAND_CHAN_PL_CHC			0
#define GRAND_BKGD_PL_CHC			1

#define EXPORT_ASSAY_PL_CHC			0
#define EXPORT_CF_PL_CHC			1
#define EXPORT_NORM_BKG_PL_CHC		2
#define EXPORT_EMPTY_ASSAY_PL_CHC	3
#define EXPORT_TOTALS_ASSAY_PL_CHC	4
#define EXPORT_ALL_CHC				5

//note:  the choices are linked to the gllszlonglist in the evntlist.cpp file.
// If you change any item in the picklists in the evntlist.cpp file, be sure to 
// modify the accompanying definitions in the raddefs.h file.  
// This is messy and should be optimized at a later time.
#define GRAPH_CHAN_EVENT_PL_CHC		0
#define GRAPH_NORM_BKG_PL_CHC		1
#define GRAPH_LOW_BKG_PL_CHC		2
#define GRAPH_HIGH_BKG_PL_CHC		3
#define GRAPH_UNEXPLAINED_PL_CHC	1


#pragma pack(1)		
/* ------------------------------------------------------------------
 *  Definition of data read from raw data files
 * ----------------------------------------------------------------*/
struct GRAND_DATA_PT {
	unsigned long  ulJulianTime;	// date in julian
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

struct DMGGRAND_DATA_PT_HDR {
	unsigned long  ulJulianTime;	// date in julian seconds
	unsigned short usJSCS;			// additive checksum for time
	BYTE Len, OpCode;		     	// length in bytes of next data block, record type opcode (MG Acquire records are 0x15, and all others are to be skipped)
};

struct DMGGRAND_DATA_PT_BDY {
	unsigned short usGrandStatus;   // status byte for record
	float fA;		     			// neutron a counts
	float fB;				     	// neutron b counts
	float fC;				     	// neutron c counts
	float fG1;				     	// gamma 1
	float fG1u;				     	// gamma 1 unc
	float fG2;				     	// gamma 2
	float fG2u;				     	// gamma 2 unc
	unsigned long usElapsedTime;	// time elapsed
	unsigned short usDCS;			// additive checksum over data
};

struct BOT_DATA_PT {
	unsigned long  ulJulianTime;	// date in julian
	unsigned short usGrandStatus;   // status byte for record
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

struct MCA_DATA_PT {
	unsigned long  ulJulianTime;	// date in julian
	unsigned short usMcaStatus;		// status byte for record
	unsigned char  ucMcaAlarm;		// alarm byte for record
	float GrossCts;				
	float U235;		     			
	float Cs137;				    
	float U238;				     	
	float Scaler;				    
	float U235Ratio;				
	float Cs137Ratio;				
	int   iElapsedTime;				// time elapsed
};

/* in common/comdatastructures.h

WHOLE_FLOAT_DATA_PT_WITH_INT_TIMES
struct WHOLE_FLOAT_DATA_PT {
	unsigned long  ulJulianTime;	// date in julian
	unsigned long ulStatus; 		// status byte for record
	float fData[MAX_NUM_DATA_CHANS];	// data value
	unsigned short usElapsedTime;  	// time elapsed
};
WHOLE_FLOAT_DATA_PT_WITH_DBL_TIMES		                            
struct WHOLE_FLOAT_DATA_PT1 {
	DATE dJulianTime;				// timestamp in DATE
	unsigned long ulStatus; 		// status byte for record
	float fData[MAX_MAX_NUM_DATA_CHANS];	// data value
	double dElapsedTime;		  	// time elapsed
};
*/

struct FLOAT_DATA_PT {
	float fData[MAX_NUM_DATA_CHANS];	// data value

struct DOUBLE_DATA_PT {
	double dData[MAX_NUM_DATA_CHANS];	// data value
	
};
	
};
				
struct JSR_DATA_PT {
	unsigned long  ulJulianTime;	// date in julian
	unsigned short usElapsedTime;	// time elapsed
	double dTotals1;     			// totals1 counts
	double dRealsPlusAccidentals;  	// reals + accidentals
	double dAccidentals;			// accidentals
	double dTotals2;				// totals2 counts
	unsigned short usJSR_Status;    // status byte for record
};

struct ISR_INT_DATA_PT {
	unsigned long ulJulianTime;		// date in julian
	signed short sISR_Status;		// status byte for record	    
	__int64 Totals1;     			// totals1 counts
	__int64 Totals2;				// totals2 counts
	__int64 Totals3;				// totals3 counts
	__int64 RealsPlusAccidentals;	// reals + accidentals
	__int64 Accidentals;			// accidentals
	__int64 ElapsedTime;			// time elapsed
};
struct ISR_DBL_DATA_PT {
	unsigned long ulJulianTime;		// date in julian
	signed short sISR_Status;		// status byte for record	    
	double Totals1;     			// totals1 counts
	double Totals2;					// totals2 counts
	double Totals3;					// totals3 counts
	double RealsPlusAccidentals;	// reals + accidentals
	double Accidentals;				// accidentals
	double ElapsedTime;				// time elapsed
};

union ISR_DATA_PT {
	struct ISR_INT_DATA_PT IntPt;
	struct ISR_DBL_DATA_PT DblPt;
};

// structure to describe records contained in a *.bin file
struct TRGAcquireRecord {
		unsigned int	uiJulianSeconds;	//4 bytes
		BYTE			bStatusByte;		//2 bytes	(whether real or interpolated point)
		unsigned short	usTriggerState;		//2 bytes
		unsigned short	usTriggerMask;		//2 bytes
		BYTE			bReserved;			//1 byte
};

struct BinaryEventFileRec {
	unsigned int uiTime;
	BYTE bStatus;
	unsigned short usNode;
	BYTE bState;
	BYTE bMask;
	BYTE bReserved;
	BYTE Dum;
};

/*
WHOLE_DOUBLE_DATA_PT_WITH_INT_TIMES
struct WHOLE_DOUBLE_DATA_PT {
	unsigned long ulJulianTime;		// date in julian
	unsigned long ulStatus; 		// status byte for record
	double dData[MAX_NUM_DATA_CHANS];	// data value
	unsigned short usElapsedTime;  	// time elapsed
};

WHOLE_DOUBLE_DATA_PT_WITH_DBL_TIMES
struct WHOLE_DOUBLE_DATA_PT1 {
	DATE dJulianTime;				// timestamp in DATE
	unsigned long ulStatus; 		// status byte for record
	double dData[MAX_NUM_DATA_CHANS];	// data value
	double dElapsedTime;			// time elapsed
};

WHOLE_LONG_DATA_PT_WITH_DBL_TIMES
struct WHOLE_LONG_DATA_PT {
	DATE dJulianTime;				// timestamp in DATE
	unsigned long ulStatus; 		// status byte for record
	long lData[MAX_NUM_DATA_CHANS];		// data value
	double dElapsedTime;		 	// time elapsed
};
*/
	
struct BIN_DATA_PT {
	unsigned long	ulJulianTime; 
	unsigned char	ucStatusByte;
	unsigned short	usTriggerState;
	unsigned short	usTriggerMask;
	unsigned char	ucReserved;
};
	
struct ANALYZE_CHANS {
	short  sStaNum;
	short sChanOffset[MAX_NUM_DATA_CHANS]; 
	float fThres[MAX_NUM_DATA_CHANS]; 
	float fBkg[MAX_NUM_DATA_CHANS];
};		
		                          
struct ANALYZE_PARMS {
	struct IDStruct StaID;
	short sChanOffset[MAX_NUM_DATA_CHANS]; 
	float fThres[MAX_NUM_DATA_CHANS]; 
	float fBkg[MAX_NUM_DATA_CHANS];
};		

struct DATA_ANALYZE {
	short	sNumPeakPts;
	short	sNumRmsPts;
	double	dNumSigma;
	BOOL	bSmooth;
	short	sWhichAlg;
	BOOL	bThresholdFunction; 
	BOOL	bLogFile;
	FILE	*pFileHandle;
	CGUI_TextWin *pWin;
	char	cSep;
	BOOL	bDB_File;
	BOOL	bTextWin;
	TextFile *pFile;
	Text_Write_Database *pTextDbFile;
};	                                
	
struct EVENT {
	BOOL bStarted;
	DATE dBegTime;
	DATE dEndTime;
	DATE dMaxTime;
	double dMaxValue;   
	double dIntSum;
	double dRealsSum;
	double dTotalsSum;
	double dSumTime;
	unsigned short usType;
	unsigned short usStatus;
	unsigned short usMeasType;
	unsigned short usNumPts;
};	
		

/* ------------------------------------------------------------------
 *  Definition of structure describing data types
 * ----------------------------------------------------------------*/
struct DATA_DISPLAY_FLAGS {
	BOOL bReadAll;
	BOOL bACPower;
	BOOL bBatteryLow;
	BOOL bColdStart;
	BOOL bAuthentication;
	BOOL bInMII;
	BOOL bOutOfOrder;
	BOOL bGaps;
};


/* ------------------------------------------------------------------
 *  Structures of file to write to INCC
 * ----------------------------------------------------------------*/
struct INCC_HEADER {
	char FileTypeStr[4];
	char MeasTypeStr[1];
	char StationStr[11];
	char ItemIdStr[12];
	char DateStr[8];
	char TimeStr[8];
	unsigned short sNumRuns;
};

struct INCC_MEASUREMENT {
	char DateStr[8];
	char TimeStr[8];
	unsigned short sElapsedTime;
	double dTotals;
	double dRealsPlusAccidentals;
	double dAccidentals;
	double dScaler1;
	double dScaler2;
	unsigned short sNumMultiplicityRuns;
};


#pragma pack(8)
#endif					                                                         

