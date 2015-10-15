// C CTimeAdjust.cpp: implementation of the C CTimeAdjust class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeAdjust.h"
#include "TimeAlignIni.h"
#include <math.h>
#include "DirUtilities.h"
#include "SafeArrayUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CTimeAlignIni IniFile;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeAdjust:: CTimeAdjust()
{
}

CTimeAdjust::~ CTimeAdjust()
{
}

///////////////////////////////////////////////////////////////////////////
//	Name:	CalculateSlopeAndIntercept
//
//	Description:
//	This function calculates the slope and intercepts for this station to the
//	base station (base station is the one which all stations are aligned to).
//
//	Declaration:
//	bool CTimeAdjust::CalculateSlopeAndIntercept(double dBaseTime1, double dBaseTime1Prime, double dTime2, double dTime2Prime, double* pdSlope, double* pdIntercept)
//
//	Input:	dBaseTime1		timestamp of first matching point on the base station
//			dBaseTime2		timestamp of second matching point on the base station
//			dTime1			timestamp of first matching point on the station to be aligned
//			dTime2			timestamp of second matching point on the station to be aligned
//
//	Output:	pdSlope		calculated slope
//			pdIntercept	calculated intercept
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created  Adapted from CalculateStationAdjustSlopeAndIntercept.cpp in original Rad
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTimeAdjust::CalculateSlopeAndIntercept(double dBaseTime1, double dBaseTime2, double dTime1, double dTime2, double* pdSlope, double* pdIntercept)
{

	*pdSlope = 1;
	*pdIntercept = 0;
			
	// calculate the slope and intercept between the base endpoints and this station's endpoints
	*pdSlope = (dTime2 - dTime1)/(dBaseTime2 - dBaseTime1);

	double dIntercept = dTime1 - (*pdSlope)*dBaseTime1;
	double dIntercept1 = dTime2 - (*pdSlope)*dBaseTime2;
	*pdIntercept = (dIntercept + dIntercept1)/2;
}



///////////////////////////////////////////////////////////////////////////
//	Name:	AdjustTimestamps
//
//	Description:
//	This function calculates the timestamp adjustment for the given
//	timestamps at the station.
//
//	Declaration:
//	bool CTimeAdjust::AdjustTimestamps(const short sFacID, const short sStaID, const long lNumMembers,
//									  const DATE *pdOrigTimestamps, DATE *pdNewTimestamps)
//
//	Input:	sFacID				ID of facility associated with the station
//			sStaID				ID of station that the timestamps belong to
//			lNumMembers			number of timestamps that must be adjusted
//			pdOrigTimestamps	array containing the unadjusted timestamps
//
//	Output:	pdNewTimestamps		array containing the adjusted timestamps
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created  Adapted from AdjustTime() in AdjustTimestamp.cpp in original Rad
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	29-Apr-2002	SFK		Back to DATE timeframe -- changed roundoff to go with DATE format
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTimeAdjust::AdjustTimestamps(
	const short sFacID, 
	//const short sStaID, 
	const long lStaID,		//HN
	const short sStaType, 
	const long lNumMembers, 
	const DATE *pOrigTimestamps, 
	DATE *pNewTimestamps)

{
	struct LinearTA_Parms Parms;

	if (lStaID != -1) 
	{
		IniFile.GetTimeAlignParms(
			sFacID, 
			//sStaID,
			(short)lStaID,		//HN
			sStaType, 
			&Parms);
	}
	else 
	{
		Parms.dSlope = 1;
		Parms.dIntercept = 0;
		Parms.bEnableStation = false;
		Parms.bEnableTimeAlign = false;
	}

	if (Parms.dSlope == 0) Parms.dSlope = 1;					// guard against divide by 0

	if (Parms.bEnableStation && Parms.bEnableTimeAlign) {
		for (long i=0; i<lNumMembers; i++) {
			double dTemp = pOrigTimestamps[i] * 86400.0;		// convert to complete seconds
			dTemp = dTemp + 0.5;								// round off by adding on half a second
			dTemp = floor(dTemp);								// get a whole number of seconds
			dTemp = dTemp/86400.0;								// convert back to DATE format

			dTemp = (dTemp - Parms.dIntercept)/Parms.dSlope;	// adjust the base time

			dTemp = dTemp * 86400.0;							// convert to complete seconds
			dTemp = dTemp + 0.5;								// round off by adding on half a second
			dTemp = floor(dTemp);								// get a whole number of seconds
			dTemp = dTemp/86400.0;								// convert back to DATE format

			pNewTimestamps[i] = dTemp;
		}
	}
	else {
		for (long i=0; i<lNumMembers; i++) {
			pNewTimestamps[i] = pOrigTimestamps[i];
		}
	}
	return(true);
}


bool CTimeAdjust::AdjustTimestamps(
	const short sFacID, 
	//const short sStaID,
	const long lStaID, //HN
	const short sStaType, 
	VARIANT* pvData)
{
	struct LinearTA_Parms Parms;

	if (lStaID != -1) 
	{
		IniFile.GetTimeAlignParms(
			sFacID, 
			//sStaID,
			(short)lStaID, //HN
			sStaType, 
			&Parms);
	}
	else 
	{
		Parms.dSlope = 1;
		Parms.dIntercept = 0;
		Parms.bEnableStation = false;
		Parms.bEnableTimeAlign = false;
	}

	if (Parms.dSlope == 0) Parms.dSlope = 1;					// guard against divide by 0

	if (Parms.bEnableStation && Parms.bEnableTimeAlign) {
		if (pvData->vt == (VT_VARIANT | VT_ARRAY)) {
			long lDimension[2];
			long lNumMembers;
			double dTemp;
			SafeArrayGetUBound(pvData->parray, 1, &lNumMembers);
			for (long i=0; i<lNumMembers; i++) {
				SA_GetElem(pvData->parray, i, 0, &dTemp);
				dTemp = dTemp * 86400.0;							// convert to complete seconds
				//dTemp = pOrigTimestamps[i] * 86400.0;				// convert to complete seconds
				dTemp = dTemp + 0.5;								// round off by adding on half a second
				dTemp = floor(dTemp);								// get a whole number of seconds
				dTemp = dTemp/86400.0;								// convert back to DATE format

				dTemp = (dTemp - Parms.dIntercept)/Parms.dSlope;	// adjust the base time

				dTemp = dTemp * 86400.0;							// convert to complete seconds
				dTemp = dTemp + 0.5;								// round off by adding on half a second
				dTemp = floor(dTemp);								// get a whole number of seconds
				dTemp = dTemp/86400.0;								// convert back to DATE format
				
				lDimension[0] = i;
				SA_PutElem(pvData->parray, i, 0, (double)dTemp);
				//pNewTimestamps[i] = dTemp;
			}
		}
	}
//	else {
//		for (long i=0; i<lNumMembers; i++) {
//			pNewTimestamps[i] = pOrigTimestamps[i];
//		}
//	}
	return(true);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	UnadjustTimestamps
//
//	Description:
//	This function calculates the timestamp adjustment for the given
//	timestamps at the station.
//
//	Declaration:
//	bool CTimeAdjust::UnadjustTimestamps(const short sFacID, const short sStaID, const long lNumMembers,
//										const DATE *pdAdjTimestamps, DATE *pdOrigTimestamps)
//
//	Input:	sFacID				ID of facility associated with the station
//			sStaID				ID of station that the timestamps belong to
//			lNumMembers			number of timestamps that must be adjusted
//			pdAdjTimestamps		array containing the adjusted timestamps
//
//	Output:	pdOrigTimestamps	array containing the unadjusted timestamps
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created  Adapted from UnAdjustTime() in AdjustTimestamp.cpp in original Rad
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	29-Apr-2002	SFK		Back to DATE timeframe -- changed roundoff to go with DATE format
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTimeAdjust::UnadjustTimestamps(
	const short sFacID, 
	//const short sStaID,
	const long lStaID, //HN
	const short sStaType, 
	const long lNumMembers, 
	const DATE *pAdjTimestamps, 
	DATE *pOrigTimestamps)
{
	struct LinearTA_Parms Parms;

	if (lStaID != -1) 
	{
		IniFile.GetTimeAlignParms(
			sFacID, 
			//sStaID,
			(short)lStaID, //HN
			sStaType, 
			&Parms);
	}
	else {
		Parms.dSlope = 1;
		Parms.dIntercept = 0;
		Parms.bEnableStation = false;
		Parms.bEnableTimeAlign = false;
	}

	if (Parms.bEnableStation && Parms.bEnableTimeAlign) {
		for (long i=0; i<lNumMembers; i++) {
			double dTemp = pAdjTimestamps[i] * 86400.0;	// convert to complete seconds
			dTemp = dTemp + 0.5;						// round off by adding on half a second
			dTemp = floor(dTemp);						// get a whole number of seconds
			dTemp = dTemp/86400.0;						// convert back to DATE format

			dTemp = (dTemp * Parms.dSlope) + Parms.dIntercept;

			dTemp = dTemp * 86400.0;					// convert to complete seconds
			dTemp = dTemp + 0.5;						// round off by adding on half a second
			dTemp = floor(dTemp);						// get a whole number of seconds
			dTemp = dTemp/86400.0;						// convert back to DATE format
			
			pOrigTimestamps[i] = dTemp;
		}
	}
	else {
		for (long i=0; i<lNumMembers; i++) {
			pOrigTimestamps[i] = pAdjTimestamps[i];
		}
	}

	return(true);

}

