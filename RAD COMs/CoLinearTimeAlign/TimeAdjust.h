// TimeAdjust.h: interface for the TimeAdjust class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEADJUST_H__6143FD96_2BD5_11D6_817E_00C04F60E89B__INCLUDED_)
#define AFX_TIMEADJUST_H__6143FD96_2BD5_11D6_817E_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTimeAdjust  
{
public:
/*
	void CalculateSlopeAndIntercept(double dBaseTime1, double dBaseTime2, double dTime1, double dTime2, double* pdSlope, double* pdIntercept);
	bool UnadjustTimestamps(const short sFacID, const short sStaID, const short sStaType, const long lNumMembers, const DATE *pAdjTimestamps, DATE *pOrigTimestamps);
	bool AdjustTimestamps(const short sFacID, const short sStaID, const short sStaType, const long lNumMembers, const DATE *pOrigTimestamps, DATE *pNewTimestamps);
	bool AdjustTimestamps(const short sFacID, const short sStaID, const short sStaType, VARIANT* pvData);
*/
	// Change all short station IDs to long for BL2R1 HN 10/24/07
	void CalculateSlopeAndIntercept(double dBaseTime1, double dBaseTime2, double dTime1, double dTime2, double* pdSlope, double* pdIntercept);
	bool UnadjustTimestamps(const short sFacID, const long lStaID, const short sStaType, const long lNumMembers, const DATE *pAdjTimestamps, DATE *pOrigTimestamps);
	bool AdjustTimestamps(const short sFacID, const long lStaID, const short sStaType, const long lNumMembers, const DATE *pOrigTimestamps, DATE *pNewTimestamps);
	bool AdjustTimestamps(const short sFacID, const long lStaID, const short sStaType, VARIANT* pvData);
	CTimeAdjust();
	virtual ~CTimeAdjust();
};

#endif // !defined(AFX_TIMEADJUST_H__6143FD96_2BD5_11D6_817E_00C04F60E89B__INCLUDED_)
