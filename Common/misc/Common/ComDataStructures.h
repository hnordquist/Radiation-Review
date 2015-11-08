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
