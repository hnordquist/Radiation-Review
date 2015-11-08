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
// TimeAlignIni.cpp: implementation of the CTimeAlignIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TimeAlignIni.h"
#include "DirUtilities.h"

// HN 10/29/07 This is silly.  Take this out per Peggy.
// Coding oddity.  We are IniFile!
//extern CTimeAlignIni IniFile;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeAlignIni::CTimeAlignIni()
{
	m_strComponentName = "TimeAlign";
	m_strMyIniFilename = "TimeAlign.ini";
	mpStaInfo = NULL;

	m_pIFacCfgMgr = NULL;
}

CTimeAlignIni::~CTimeAlignIni()
{
	if (mpStaInfo) 
		delete [] mpStaInfo;

	if (m_pIFacCfgMgr)
		m_pIFacCfgMgr = NULL;
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetTimeAlignParms
//
//	Description:
//	Gets the time align parameters for the specified station.  These are stored
//	in the m_strMyIniFilename file in the m_strDataStorePath directory.  For speed
//	create a memory array of any parameters read from the ini file.  Read the
//	parameters only from the ini file if they do not exist in the memory array.
//	
//	Declaration:
//	bool CTimeAlignIni::GetTimeAlignParms(const short sFacID, const short sStaID, struct LinearTA_Parms* pParms)
//
//	Input:	sFacID				facility number
//			sStaID				station number
//			m_strDataStorePath	root directory where the ini file is expected to be
//			m_strComponentName	subdirectory under m_strDataStorePath for this component
//			m_strMyIniFilename	name of this component's ini file
//
//	Output:	pParms	LinearTA_Parms structure containing parameters for the specified station
//
//	Return:	true (db exists and initialized) / false (error in operation)
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//	09-Dec-2003	SFK		Added station type
//////////////////////////////////////////////////////////////////
#define MAX_LINE_LEN 256
bool CTimeAlignIni::GetTimeAlignParms(const short sFacID, const short sStaID, const short sStaType, struct LinearTA_Parms* pParms)
{	
	static short sNumStas = 0;
	CString strSectionName;
	char strValue[MAX_LINE_LEN+1];
	int iLen;
	CString strNameAndPath;
	short i;

	pParms->sStaNum = -1;
	
	// if haven't yet allocated some local memory, do it now.
	if (mpStaInfo == NULL) 
	{

		if (m_pIFacCfgMgr==NULL)
			m_pIFacCfgMgr.CreateInstance(CLSID_FacilityMgrData);

		sNumStas = m_pIFacCfgMgr->GetNumberOfStationsForFacility (sFacID,VARIANT_TRUE);
//		sNumStas = 0;

		// allocate and initialize the structure to hold data in during runtime
		mpStaInfo = new LinearTA_Parms[sNumStas];
		for (i=0; i<sNumStas; i++) {
			mpStaInfo[i].sStaNum = -1;
		}
	}

	// see if the data is we need already in the memory structure
	for (i=0; i<sNumStas; i++) {
		if ((mpStaInfo[i].sStaNum == sStaID) && (mpStaInfo[i].sStaType == sStaType)) {
			*pParms = mpStaInfo[i];
			break;
		}
		if (mpStaInfo[i].sStaNum == -1) break;		// there are no more entries filled in
	}

	// if haven't found what we need, the station number will still be -1
	// read its data from the ini file
	if (pParms->sStaNum == -1) {
	
		strNameAndPath = m_strDataStorePath + m_strMyIniFilename;

		// find next empty spot in our memory array
		i=0;
		while (mpStaInfo[i].sStaNum != -1) i++;

		// unexpected number of stations in the facility, start memory array over
		if (i == sNumStas) {	
			i=0;
			delete [] mpStaInfo;
			mpStaInfo = new LinearTA_Parms[sNumStas+1];
			for (i=0; i<sNumStas; i++) {
				mpStaInfo[i].sStaNum = -1;
			}
		}

		//Read all the information for this station from the parameter ini file
		mpStaInfo[i].sStaNum = sStaID;
		mpStaInfo[i].sStaType = sStaType;
		strSectionName.Format("FACILITY_%d_STATION_%d_TYPE_%d", sFacID, sStaID, sStaType); 
		
		// read the strings from the ini file -- if iLen > 0 something was there. else set to a default value
		// HN 10/29/07 This was only done once, need iLen each time per Peggy.
		iLen = GetPrivateProfileString(strSectionName, "Slope", "", strValue, MAX_LINE_LEN, strNameAndPath);
		if (iLen > 0) mpStaInfo[i].dSlope = atof(strValue);	
		else mpStaInfo[i].dSlope = 1;

		iLen = GetPrivateProfileString(strSectionName, "Intercept", "", strValue, MAX_LINE_LEN, strNameAndPath);
		if (iLen > 0) mpStaInfo[i].dIntercept = atof(strValue);
		else mpStaInfo[i].dIntercept = 0;

		iLen = GetPrivateProfileString(strSectionName, "Event1Time", "", strValue, MAX_LINE_LEN, strNameAndPath);
		if (iLen > 0) mpStaInfo[i].Event1Time = atof(strValue);
		else mpStaInfo[i].Event1Time = 0;

		iLen = GetPrivateProfileString(strSectionName, "Event2Time", "", strValue, MAX_LINE_LEN, strNameAndPath);
		if (iLen > 0) mpStaInfo[i].Event2Time = atof(strValue);
		else mpStaInfo[i].Event2Time = 0;

		iLen = GetPrivateProfileString(strSectionName, "Enable", "", strValue, MAX_LINE_LEN, strNameAndPath);
		// HN 10/29/07 Slight change to logic here per Peggy.
		//if (iLen > 0) mpStaInfo[i].bEnableTimeAlign = atoi(strValue);
		//else mpStaInfo[i].bEnableTimeAlign = 0;
		mpStaInfo[i].bEnableTimeAlign = false;
		if (iLen > 0 && (0 != atoi (strValue)))
			mpStaInfo[i].bEnableTimeAlign = true;

		iLen = GetPrivateProfileString(strSectionName, "UseStation", "", strValue, MAX_LINE_LEN, strNameAndPath);
		// HN 10/29/07 More tweaks in logic per Peggy
		//if (iLen > 0) mpStaInfo[i].bEnableStation = atoi(strValue);
		//else mpStaInfo[i].bEnableStation = 0;
		mpStaInfo[i].bEnableStation = false;
		if (iLen > 0 && (0!= atoi (strValue)))
			mpStaInfo[i].bEnableStation = true;

		*pParms = mpStaInfo[i];
	}

	return(true);
}

	
///////////////////////////////////////////////////////////////////////////
//	Name:	WriteTimeAlignParms
//
//	Description:
//	Write the time align parameters for the specified station to the 
//	m_strMyIniFilename file in the m_strDataStorePath directory.
//	
//	Declaration:
//	bool CTimeAlignIni::WriteTimeAlignParms(const short sFacID, const short sStaID, struct LinearTA_Parms* pParms)
//
//	Input:	sFacID				facility number
//			sStaID				station number
//			sStaType				station type number
//			m_strDataStorePath	root directory where the ini file is expected to be
//			m_strComponentName	subdirectory under m_strDataStorePath for this component
//			m_strMyIniFilename	name of this component's ini file
//
//	Output:	pParms	LinearTA_Parms structure containing parameters for the specified station
//
//	Return:	true (db exists and initialized) / false (error in operation)
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//	08-Dec-2003	SFK		added station type
//////////////////////////////////////////////////////////////////
bool CTimeAlignIni::WriteTimeAlignParms(const short sFacID, const short sStaID, const short sStaType, struct LinearTA_Parms* pParms)
{	
	static bool iFirstRead = true;
	static short sNumStas;
	CString strSectionName, strValue;
	CString strNameAndPath = m_strDataStorePath + m_strMyIniFilename;

	// write the information in the parameter to the ini file
	strSectionName.Format("FACILITY_%d_STATION_%d_TYPE_%d", sFacID, sStaID, sStaType); 
	
	// write the strings to the ini file
	strValue.Format("%15.15e", pParms->dSlope);
	// HN 10/29/07 This actually returns a BOOL, not an int.
	//int bSuccess = WritePrivateProfileString(strSectionName, "Slope", strValue, strNameAndPath);
	BOOL bSuccess = WritePrivateProfileString(strSectionName, "Slope", strValue, strNameAndPath);

	if (bSuccess) {
		strValue.Format("%15.15e", pParms->dIntercept);
		bSuccess = WritePrivateProfileString(strSectionName, "Intercept", strValue, strNameAndPath);
	}

	if (bSuccess) {
		strValue.Format("%15.15e", pParms->Event1Time);
		bSuccess = WritePrivateProfileString(strSectionName, "Event1Time", strValue, strNameAndPath);
	}

	if (bSuccess) {
		strValue.Format("%15.15e", pParms->Event2Time);
		bSuccess = WritePrivateProfileString(strSectionName, "Event2Time", strValue, strNameAndPath);
	}

	if (bSuccess) {
		strValue.Format("%d", pParms->bEnableTimeAlign);
		bSuccess = WritePrivateProfileString(strSectionName, "Enable", strValue, strNameAndPath);
	}

	if (bSuccess) {
		strValue.Format("%d", pParms->bEnableStation);
		bSuccess = WritePrivateProfileString(strSectionName, "UseStation", strValue, strNameAndPath);
	}

	// force a read of the new values
	if (mpStaInfo)
	{
		delete [] mpStaInfo;
		mpStaInfo = NULL;
	}

	return (bSuccess != FALSE);

}

/*
void CTimeAlignIni::SetDataStorePath(const short sFacilityID)
{
	// Remove static for E10R4 HN 10/24/07
	//static CFacCfgWrap FacCfg;
	//CFacCfgWrap FacCfg;//SFK 9/28/05
	CDirUtilities Dir(false);  //QUIET MODE IS ALWAYS OFF FOR VIFM pjm 11/27/2007 for B2R1
	BSTR bstrFacDir;

	// Get facility path
	if (m_pIFacCfgMgr==NULL)
		m_pIFacCfgMgr.CreateInstance(CLSID_FacilityMgrData);

	bstrFacDir = m_pIFacCfgMgr->GetFacilityDirectory (sFacilityID);
	CString strFacDir (bstrFacDir);
//	CString strFacDir = "";
	Dir.AppendBackslash(strFacDir);
	//m_strDataStorePath = strFacDir;
	m_strDataStorePath = strFacDir;
	// HN 10/29/07 We just appended a backslash, don't add another.
	strFacDir += "Config\\Rad\\";
	//IniFile.m_strDataStorePath = strFacDir;
	m_strDataStorePath = strFacDir;

//	IniFile.m_bOpen = true;
	m_bOpen = true;
}
*/

void CTimeAlignIni::SetDataStorePath(const short sFacilityID)
{
	CDirUtilities Dir(false);  //QUIET MODE IS ALWAYS OFF FOR TIMEALIGN pjm 11/27/2007 for B2R1

	if (m_pIFacCfgMgr==NULL)
		m_pIFacCfgMgr.CreateInstance(CLSID_FacilityMgrData);

	BSTR bstrFacDir = m_pIFacCfgMgr->GetFacilityDirectory (sFacilityID);
	CString cs(bstrFacDir);
	SysFreeString(bstrFacDir);

	m_strDataStorePath = cs;
	Dir.AppendBackslash(m_strDataStorePath);
	m_strDataStorePath += "Config\\Rad\\";
	m_bOpen = true;
}
