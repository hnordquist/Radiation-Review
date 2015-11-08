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
// EOSSData.h: interface for the CEOSSData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EOSSDATA_H__02CFEFCF_E906_11D5_816F_00C04F60E89B__INCLUDED_)
#define AFX_EOSSDATA_H__02CFEFCF_E906_11D5_816F_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AccessBaseData.h"

class CEOSSDataPt:public CObject
{
public:
	
	CEOSSDataPt (long lStationID, short sChan, COleDateTime dtEventTime, bool bState):
	  m_bState (bState), m_dtEventTime (dtEventTime), m_sChannel (sChan), m_lStationID (lStationID){};
	 CEOSSDataPt ():
	  m_bState (0), m_dtEventTime (COleDateTime::GetCurrentTime()), m_sChannel (0), m_lStationID (0){};
	void Set (long lStationID, short sChan, COleDateTime dtEventTime, bool bState)
	{
		m_bState =bState;
		m_dtEventTime = dtEventTime;
		m_sChannel =sChan;
		m_lStationID =lStationID;
	};
	bool m_bState;
	COleDateTime m_dtEventTime;
	short m_sChannel;
	long m_lStationID;
};

/////////////////////////////////////////////////////////////////////////////
// CEOSSData - EOSS specific versions of what all data objects must do/be.
class CEOSSData
{
public:

	CEOSSData(
		CDbAccess* pDb, 
		const int iStaNum,  
		const bool bQuietMode);
		
	~CEOSSData();

	bool AddEOSSData(CString *pstrErrorMsg=NULL);

	bool GetEOSSDataStoreTimeRange (DATE* pdStart, DATE* pdEnd);
	bool GetEOSSDataStatistics (short sChannelID, DATE StartTime, DATE EndTime, DATE* pActualStartTime, DATE* pActualEndTime, long* plNumDays);
	bool GetEOSSDaySummaryData (long lStation, DATE dtStart, DATE dtEnd, VARIANT* pvDayData);
	bool EOSSDeleteData (const DATE& dtStart, const DATE& dtEnd);
	bool GetEOSSDayStatistics (long lStationID, DATE dStart, DATE dEnd, DATE* actualStart, DATE* actualEnd,long* lNumPoints);
	bool GetEOSSSealData (long lStationID, short sSealID, VARIANT* saData,DATE& Start, DATE& End);	
	bool AddSealLogPoint (long lStationID, short sChannel, COleDateTime eventTime, bool bState);

private:

	CDbAccess* m_pDb;					// which Access database is open now

	unsigned long m_ulNumPtsRequested;	// number of data points requested for an action
	unsigned long m_ulNumPtsActual;		// actual number of data points produced by action
	bool  m_bInterpolated;				// should we interpolate this data type
	int m_iStaNum;						// station number we are interested in now
	int m_iInstType;					// some station numbers can have multiple types of instruments (ILONs)
	int m_iChanNum;
	int m_iNumPoints;
	CEOSSDataPt* m_plogpoints;
	
};
#endif // !defined(AFX_EOSSDATA_H__02CFEFCF_E906_11D5_816F_00C04F60E89B__INCLUDED_)
