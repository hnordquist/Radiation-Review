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
// ErrorProcessing.h

#pragma once

class CAdodbException;
#include "imports.h"

#define DISABLE_LOGGER		0x00020000
#define QUIET_MODE			0x00010000
#define QUIET_MODE_OFF		0
#define VERBOSITY1_AUTH		0x00000001
#define VERBOSITY2_FATAL	0x00000002
#define VERBOSITY3_NONFATAL	0x00000003
#define VERBOSITY4_WARNING	0x00000004
#define VERBOSITY5_INFO		0x00000005
#define VERBOSITY6_DIAG		0x00000006
#define VERBOSITY6_ALL		0x00000006

#define FATAL		(0x80000000)
#define NON_FATAL	(0x40000000)
#define WARNING		(0x20000000)

class CErrorProcessing
{
public:

	CErrorProcessing(
		CLSID ComponentGUID, 
		short sUniqueID,
		char* pmessageformatfile,
		short verbosity,
		char *componentname,
		COleDateTime& dtStartUp);

	virtual ~CErrorProcessing();

	void InitializeForMethod(
		const char *methodname,
		short sFacilityID = 0,
		long lStationID = 0,
		short sChannelID = 0);

	void InitializeForMethod(
		CString &methodname,
		short sFacilityID = 0,
		long lStationID = 0,
		short sChannelID = 0);

	void LogMessage(long lMessageID, CString& csinput);

	inline void SetVerbosity(short s){m_sVerbosity = s;}
	void SetDisableLogger(bool disablelogger);
	void SetGetLastErrorSeverity();
#ifdef _DEBUG
	void DoAll();  //diagnostic function
#endif
	void Generic(long lMessageID, CString& cs);
	long GetSeverityFromINIFile(long lMessageID);

	void ErrorInvalidPointer();
	void ErrorEmbeddedInterfaceCallFailed(CString& internalmethodname);
	void ErrorUnimplementedMethod();
	void ErrorOpenDatabase(const CString& additionalinfo);
	void ErrorReadFile(CString& filename, CString& additionalinfo);
	void ErrorOpenFile(const CString& filename, const CString& additionalinfo);
	void ErrorFindFile(CString& filename, CString& additionalinfo);
	void ErrorFindDatabase(CString& databasename, CString& additionalinfo);
	void ErrorDatabase(CString& dbtype, CString& additionalinfo);
	void ErrorCopyDatabase(CString& additionalinfo);
	void ErrorCreateDatabase(CString& additionalinfo);
	void COMErrorCatch(_com_error& e);
	void ADOErrorCatch(CAdodbException *cae);
	void ErrorCatch();
	void ErrorCatch(int inum);
	void ErrorDebug(CString& additionalinfo);
	void ErrorSAFEARRAY(CString& additionalinfo);
	void ErrorParametersNotSet(CString& additionalinfo);
	void ErrorStartTimeAfterEndTime(DATE starttime, DATE endtime);
	void SuccessReadFile(CString& filename);
	void StopTime();

	inline void SetUniqueID(short s){m_sUniqueID = s;}
	inline void SetFacilityName(CString& strFac)
	{
		if (!strFac.IsEmpty())
			m_csFacilityName = strFac;
		else
			m_csFacilityName = "UNKNOWN FACILITY NAME";
	}

	//Retrieved by the GetLastError method 
	CString	m_csLastErrorText;
	long	m_lLastErrorValue;

private:

	// Specific to the component and constant throughout lifetime of component
	BSTR	m_bstrComponentGUID;
	CString	m_csComponentName;
	COleDateTime m_dtStartUp;
	short	m_sUniqueID;
	bool	m_bStartTimeAlreadySent;
	CString m_csFacilityName;

	CString m_cs;//scratch
	CString m_csdatefmtstr;
	char m_charbuf[256];//scratch
	bool m_bPrependComponentAndMethodStrings;
	long m_lOriginalMessageID;
	long m_lSeverityFromIniFile;

	//Specific to the component, but may be changed via accessor functions
	BSTR	m_bstrMessageFormatFile;
	CString m_csMessageFormatFile;
	short	m_sVerbosity;
	bool	m_bDisableLogger;

	void SendStartTimeToLogger();

	//Specific to the calling interface method
	short	m_sFacilityID;
	long	m_lStationID;
	short	m_sChannelID;
	CString m_csMethodName;

	IIRSLoggerPtr m_pILogger;  //Heather smart pointer

	CString m_csNULL;
	CString m_csColon;// = ":";
	CString m_csSuccessReadingFile;// = "Success reading file: ";
	CString m_csInvalidPointer;// = "Invalid pointer";
	CString m_csInternalInterfaceCallFailed;// = "Internal interface call failed: ";
	CString m_csUnimplementedMethod;// = "Unimplemented method";
	CString m_csIntervalStartTime;// = "Interval start time (";
	CString m_csExceedsEndTime;// = ") exceeds end time (" + csend + ")";
	CString m_csCloseParen;// = ")";
	CString m_csParametersNotSet;// = "Parameters not set: ";
	CString m_csSAFEARRAYError;// = "SAFEARRAY error: ";
	CString m_csCouldNotOpenDatabase;// = "Cound not open database: ";
	CString m_csCouldNotReadFile;// = "Could not read file: ";
	CString m_csCouldNotFindFile;// = "Could not find file: ";
	CString m_csCouldNotFindDatabase;// = "Could not find database: ";
	CString m_csCouldNotOpenFile;// = "Could not open file: ";
	CString m_csCaughtCOMErrorException;// = "Caught COM Error exception: ";
	CString m_csCaughtException;// = "Caught exception";
	CString m_csCaughtADODBException; // = "Caught ADODB Exception";
	CString m_csDatabaseError;// = "Database error: ";
	CString m_csCouldNotCopyDatabase;// = "Could not copy database: ";
	CString m_csCouldNotCreateDatabase;// = "Could not create database: ";
	CString m_csDebug;// = "Debug: ";
};


class CAdodbException : public CException
{
	DECLARE_DYNAMIC(CAdodbException);

	CAdodbException();
	CAdodbException(CString &cs);
	CAdodbException(char *pch);

	char infoADO[256];

	void SetErrorString(LPCTSTR errorstr);
	void SetErrorString(CString &cs);

	BOOL GetErrorMessage(LPTSTR pszErr,UINT nMaxErr,UINT* pnHelpID = NULL);
};

