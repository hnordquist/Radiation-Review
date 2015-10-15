
//ComDataStructures.H

#ifndef COMDATASTRUCTURES_H
#define COMDATASTRUCTURES_H

#include "TypeDefinitions.h"

//These data point structs were defined in both COMDATASTRUCTURES.H
//and in RADDEFS.H.  Removed them from RADDEFS.H on 2/26/2008 pjm

//<<QA>> edit the files to get rid of all this confusion - eventaully
//#define WHOLE_FLOAT_DATA_PT		WHOLE_FLOAT_DATA_PT_WITH_INT_TIMES
//#define WHOLE_FLOAT_DATA_PT1	WHOLE_FLOAT_DATA_PT_WITH_DBL_TIMES
//#define WHOLE_LONG_DATA_PT		WHOLE_LONG_DATA_PT_WITH_DBL_TIMES
//#define WHOLE_DOUBLE_DATA_PT	WHOLE_DOUBLE_DATA_PT_WITH_INT_TIMES
//#define WHOLE_DOUBLE_DATA_PT1	WHOLE_DOUBLE_DATA_PT_WITH_DBL_TIMES


//WHOLE_FLOAT_DATA_PT_WITH_INT_TIMES
struct WHOLE_FLOAT_DATA_PT {
	unsigned long  ulJulianTime;	// date in julian
	unsigned long ulStatus; 		// status byte for record
	float fData[MAX_NUM_DATA_CHANS];	// data value
	unsigned short usElapsedTime;  	// time elapsed
};
//WHOLE_FLOAT_DATA_PT_WITH_DBL_TIMES		                            
struct WHOLE_FLOAT_DATA_PT1 {
	DATE dJulianTime;				// timestamp in DATE
	unsigned long ulStatus; 		// status byte for record
	float fData[MAX_NUM_DATA_CHANS];	// data value
	double dElapsedTime;		  	// time elapsed
};

//WHOLE_DOUBLE_DATA_PT_WITH_INT_TIMES
struct WHOLE_DOUBLE_DATA_PT {
	unsigned long ulJulianTime;		// date in julian
	unsigned long ulStatus; 		// status byte for record
	double dData[MAX_NUM_DATA_CHANS];	// data value
	unsigned short usElapsedTime;  	// time elapsed
};

struct EOSS_DATA_PT
{
	short sChannelIndex;
	short sChannelID;
	int nState;
};

//WHOLE_DOUBLE_DATA_PT_WITH_DBL_TIMES
struct WHOLE_DOUBLE_DATA_PT1 {
	DATE dJulianTime;				// timestamp in DATE
	unsigned long ulStatus; 		// status byte for record
	double dData[MAX_NUM_DATA_CHANS];	// data value
	double dElapsedTime;			// time elapsed
	struct EOSS_DATA_PT eossPt;
	// For eoss data, ignore all but Julian Time and EOSS_DATA_PT
};

//WHOLE_LONG_DATA_PT_WITH_DBL_TIMES
struct WHOLE_LONG_DATA_PT {
	DATE dJulianTime;				// timestamp in DATE
	unsigned long ulStatus; 		// status byte for record
	long lData[MAX_NUM_DATA_CHANS];		// data value
	double dElapsedTime;		 	// time elapsed
	struct EOSS_DATA_PT eossPt;
};

#pragma pack(1)
//////////////////////////////////
// WHOLE_FLOAT_DATA_PT
//////////////////////////////////
struct WHOLE_FLOAT_DATA_PT_WITH_DBL_TIMES 
{
	double dJulianTime;				// DATE would not always compile
	unsigned long ulStatus; 		// status byte for record
	float fData[MAX_NUM_DATA_CHANS];	// data value
	double dElapsedTime;		 	// time elapsed
};
//////////////////////////////////
// WHOLE_FLOAT_DATA_PT1
//////////////////////////////////
struct WHOLE_FLOAT_DATA_PT_WITH_INT_TIMES 
{
	unsigned long  ulJulianTime;	// date in julian
	unsigned long ulStatus; 		// status byte for record
	float fData[MAX_NUM_DATA_CHANS];	// data value
	unsigned short usElapsedTime;  	// time elapsed
};
//////////////////////////////////
// WHOLE_LONG_DATA_PT
//////////////////////////////////
struct WHOLE_LONG_DATA_PT_WITH_DBL_TIMES 
{
	double dJulianTime;				// DATE would not always compile
	unsigned long ulStatus; 		// status byte for record
	long lData[MAX_NUM_DATA_CHANS];		// data value
	double dElapsedTime;		 	// time elapsed
};
//////////////////////////////////
// WHOLE_DOUBLE_DATA_PT
//////////////////////////////////
struct WHOLE_DOUBLE_DATA_PT_WITH_INT_TIMES 
{
	unsigned long ulJulianTime;		// date in julian
	unsigned long ulStatus; 		// status byte for record
	double dData[MAX_NUM_DATA_CHANS];	// data value
	unsigned short usElapsedTime;  	// time elapsed
};
//////////////////////////////////
// WHOLE_DOUBLE_DATA_PT1
//////////////////////////////////
struct WHOLE_DOUBLE_DATA_PT_WITH_DBL_TIMES 
{
	DATE dJulianTime;
	unsigned long ulStatus;
	double dData[MAX_NUM_DATA_CHANS];
	double dElapsedTime;
};

#pragma pack(8)

// DayData structure as known by rest of Rad
//	contains all information about a day, also defined in RadDefs.h
typedef struct DayDataStruct 
{
	DATE			dFirstDataPt;	// DATE would not always compile
	DATE			dLastDataPt;	// DATE would not always compile
	unsigned long	ulNumPtsInDay; 	// number of data points in day
}DayDataStruct;



typedef struct IDStruct {
	short	sStaID;
	short	sStaType;
	short	sDataType;
	short	sChanID;
	char	szStaType[40];
} IDStruct;



#endif //COMDATASTRUCTURES_H
