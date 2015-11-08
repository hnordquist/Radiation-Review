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
// TimeAlign.h: interface for the CTimeAlign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEALIGN_H__C6691B5D_EA08_4DE8_B3C9_AA258C5F6B32__INCLUDED_)
#define AFX_TIMEALIGN_H__C6691B5D_EA08_4DE8_B3C9_AA258C5F6B32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ComDataStructures.h"

// TimeAlignParmsStruct - contains all information for time align parameters
struct TimeAlignParmsStruct {
	short sStaNum;
// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
// We will keep this stat number a short.  We will only switch to a LONG just as the
// interface methods are called.
	short sStaType;
	double dSlope;
	double dIntercept;
	unsigned long ulEvent1Time;
	unsigned long ulEvent2Time;
	BOOL bUseTimeAlign;
	BOOL bEnableStation;
};


//int OpenTimeAlignDatastore(const char* strDataStorePath);
int OpenTimeAlignDatastore(const short sFacilityID);

class CTimeAlign  
{
public:
	CTimeAlign(const char* szDefaultPath, short sFacID);
	~CTimeAlign();

	void CloseDatabase();
	int  GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime, short sStaType);

	//int  GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays, short sStaType);
	int  GetDataByDay(const short sFacID, const struct IDStruct *pStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays);

	int  GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, VARIANT *pvData, short sStaType, unsigned long *pulNumPts);
	BOOL  GetDataStatistics(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, DATE *pdStart, DATE *pdEnd, unsigned long *pulNumPts, short sStaType);
	int  GetDayStatistics(const short sFacID, const struct IDStruct *pStaID, const DATE dStart, const DATE dEnd, DATE *pdRealStart, DATE *pdRealEnd, unsigned long *pulNumPts);
	int  GetDayStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdRealStart, DATE *pdRealEnd, unsigned long *pulNumPts);
	int  GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay, short sStaType);
	int  GetDbStartDay(const short sFacID, const short sStaID, DATE* pdStartDay, const bool bErrDisplay, short sStaType);
	bool GetParameters(const int sFacID,  const long sNum, struct TimeAlignParmsStruct *pTimeAlignParms);
	int  InitializeDatabase(const short sFacID);
	bool  OpenDatabase(const short sFacID);
	bool SetParameters(const int sFacID, short sNum, struct TimeAlignParmsStruct *pTimeAlignParms);

	BOOL COMDataType(short sType);

	char mstrDefaultPath[256];

	bool IsTimeAlignEnabled(const int iFacID,  const int uiPL_ChanIndex);

};

extern CTimeAlign *pglTimeAlign;

#endif // !defined(AFX_TIMEALIGN_H__C6691B5D_EA08_4DE8_B3C9_AA258C5F6B32__INCLUDED_)
