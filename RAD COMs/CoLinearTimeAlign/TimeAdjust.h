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
