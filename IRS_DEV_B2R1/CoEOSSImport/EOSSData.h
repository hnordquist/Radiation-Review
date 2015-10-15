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
