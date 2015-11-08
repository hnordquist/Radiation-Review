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
///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//    File Name : EventNetArea.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Functions:
//		EventCalculateNetArea()
///////////////////////////////////////////////////////////////////////////

#include "Rad.h"
#include "AdjustTime.h"
#include "DbReadStruct.h"
#include "DbInterval.h"
#include "FacilityConfig.h"
#include "TypeDefinitions.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:	EventCalculateNetArea
//
//  Purpose :	Given the integral sum of the event, calculate the net area by determining the area
//				under a straight line drawn from the beginning to the ending point of the event.
//				Use the formula (x'-x)*(y'+y)/2 where x,y is beginning of event and x',y' is end of event.
//				Also if GRAND data, calculate ratios needed for Rokkasho.
//
//	Declaration:
//	void EventCalculateNetArea(struct IDStruct ChanID, struct db_event_rec *pEvent, double *pNetArea, unsigned long *pulNumPts, double *pdRatio1, double *pdRatio2, double *pdRatio3)
//
//	Input:	struct IDStruct ChanID	describes station number, type and channel number
//			db_event_rec pEvent		describes event of interest
//
//	Output:	pdNetArea				rough calculation of net area (straight line from backgound pts)
//			pulNumPts				number of data records in the event
//			pdRatio1				if GRAND data, G1/(NA + NB)
//			pdRatio2				if GRAND data, NA/NB
//			pdRatio3				if GRAND data, NC/(NA + NB)
//
//	Return:	None
//
//  Special notes:	Assumes integral sum has been calculated for the event prior to the call
//
//  date    /	author	revision
//  -----------------	--------
// 	09-Nov-2001 SFK		Created
//	11-Dec-2001	SFK		Do a time unadjust before you get the raw data associated with the beginning
//						and ending times of the event.
//	02-Apr-2002	SFK		Remove time adjust stuff because now done in Grand Import Com.  Calculate the area taking
//						into account the different width 
//	12-Aug-2002 SFK		Changed to DATE
//	18-Aug-2004	SFK		When calculating the background value, take the count time associated with the event
//						into account.
//	22-Jun-2005	SFK		Don't bother to calculate a net area if the event is for a Binary station
//	07-Jul-2005	SFK		Calculate the ratios needed for Rokkasho in addition to net area if MiniGRAND
//						data and return them.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EventCalculateNetArea(struct IDStruct ChanID, struct db_event_rec *pEvent, double *pdNetArea, unsigned long *pulNumPts, double *pdRatio1, double *pdRatio2, double *pdRatio3)
{
	int iStatus;
	unsigned long ulNumRead;
	double dTemp, dTemp1, dTemp2;
	DATE dStart, dEnd;
	struct WHOLE_DOUBLE_DATA_PT1 *pdPts;		// all data for analysis is put in doubles in the wrappers

	*pdNetArea = pEvent->d_event_integral_sum;
	*pdRatio1 = 0.0;
	*pdRatio2 = 0.0;
	*pdRatio3 = 0.0;
	if (ChanID.sStaType == BINARY_TYPE || ChanID.sStaType == EOSS_TYPE)
	{
		*pulNumPts = 1;
		return;	// 22-Jun-2005 SFK has no meaning for binary data
	}
	//	In case the event is based on timestamped data, get back to the original now
	//	since these routines to read the raw data apply an adjustment on what is returned.
	//	This is a short term measure which should eventually be cleaned up by doing time adjust properly.

	// Get data associated with all the points in the event
	iStatus = GetDataStatistics(ChanID, false, pEvent->d_event_beg_time_key, pEvent->d_event_end_time, &dStart, &dEnd, pulNumPts);
	if (!iStatus || *pulNumPts <= 1) return;

	if ((*pulNumPts) > 2)
		iStatus = iStatus;
	
	pdPts = new struct WHOLE_DOUBLE_DATA_PT1[*pulNumPts];
	//if (pdX && pdY) {
	if (pdPts) {
		iStatus = GetDoubleDataByStation(ChanID, dStart, dEnd, *pulNumPts, &ulNumRead, pdPts, &dTemp, TRUE);
		if (!iStatus) return;

		// Calculate the area underneath the straight line
		dTemp1 = (pdPts[ulNumRead-1].dData[ChanID.sChanID -1]) * pdPts[ulNumRead-1].dElapsedTime;
		dTemp2 = (pdPts[0].dData[ChanID.sChanID-1]) * pdPts[0].dElapsedTime;
		dTemp = (*pulNumPts -1) * (dTemp1 + dTemp2)/2;
		*pdNetArea = *pdNetArea - dTemp;

		// 07-Jul-2005 SFK Added calculation of Rokkasho-specific ratios
		if (ChanID.sStaType == GRAND_TYPE) {
			double dG1Sum = 0.0;
			double dN1Sum = 0.0;
			double dN2Sum = 0.0;
			double dN3Sum = 0.0;
			for (unsigned long i=0; i<ulNumRead; i++) {
				dG1Sum += pdPts[i].dData[3] * pdPts[i].dElapsedTime;
				dN1Sum += pdPts[i].dData[0] * pdPts[i].dElapsedTime;
				dN2Sum += pdPts[i].dData[1] * pdPts[i].dElapsedTime;
				dN3Sum += pdPts[i].dData[2] * pdPts[i].dElapsedTime;
			}
			if ((dN1Sum + dN2Sum) != 0) {
				*pdRatio1 = dG1Sum/(dN1Sum + dN2Sum);
				*pdRatio3 = dN3Sum/(dN1Sum + dN2Sum);
			}
			if (dN2Sum != 0) *pdRatio2 = dN1Sum/dN2Sum;
		}
		delete [] pdPts;
	}
}
