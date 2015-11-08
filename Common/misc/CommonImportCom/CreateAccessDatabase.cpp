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
// CreateAccessDatabase.cpp: implementation of the CCreateAccessDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "CreateAccessDatabase.h"
#include "commondefines.h"
#include "imagehlp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCreateAccessDatabase::CCreateAccessDatabase(const CString& csDatabasePath, const CString& csDatabaseName, const CString& errINIFile)
{
	// Define ADOX object pointers to Catalog (Database),
	//  Table and Column objects.
	m_pCatalog= NULL;
    m_pTable= NULL;
	m_pIndexPtr = NULL;

	m_csErrorINIFile = errINIFile;

	HRESULT hr = S_OK;

    try
	{
		//Create object instances:
		m_pCatalog.CreateInstance(__uuidof (ADOX::Catalog));

		BOOL bResult = MakeSureDirectoryPathExists (csDatabasePath+"\\");
		BSTR bstrConxString;
		CString conxString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" +
          csDatabasePath + "\\" + csDatabaseName;
		bstrConxString = conxString.AllocSysString ();
		//Create Database
		hr = (ADOX::_CatalogPtr)m_pCatalog->Create(bstrConxString);
		::SysFreeString (bstrConxString);
	}
	catch (_com_error &e)
	{
		// Redundant with new error codein CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError.Format ("COM Exception HRESULT: 0x%x Message: %s",e.Error (), e.ErrorMessage ());
		SetError (m_csError,EXCEPTION_CAUGHT);
	}
	catch (...)
	{
		// Redundant with new error code in CBackgroundAnalysis::ExecuteAnalysis ()
		m_csError = "Unknown exception caught";
		SetError (m_csError,EXCEPTION_CAUGHT);
	}	

}

void CCreateAccessDatabase::SetError (CString err, long lErrID)
{
	m_csError = FormatMessage (err,lErrID);
	m_lErrID = lErrID | m_lErrSev;
}

void CCreateAccessDatabase::ClearError(void)
{
	m_csError = "";
	m_lErrID = 0;
}

CString CCreateAccessDatabase::FormatMessage(CString arg, long lMsgID)
{
	CString sectionName, keyName, message;
	sectionName.Format ("%u",lMsgID);
	keyName = "Msg";
	char* pszMessage = new char [MAX_ERROR];
	DWORD dw = GetPrivateProfileString ((LPCTSTR)sectionName,(LPCTSTR)keyName,"Could not format message",pszMessage,MAX_ERROR,m_csErrorINIFile);
	message = pszMessage;
	if (message == "Could not format message")
		message.Format ("Could not format message, argument: %s",arg);
	else
		message.Format (pszMessage,arg);
	keyName = "Sev";
	long lLoggerSeverity = GetPrivateProfileInt ((LPCTSTR)sectionName,(LPCTSTR)keyName,WARNING_ERROR,m_csErrorINIFile);

	if (lLoggerSeverity == FATAL || lLoggerSeverity == AUTHENTICATION)
		m_lErrSev = FATAL_ERROR;
	else if (lLoggerSeverity == ERROR)
		m_lErrSev = NON_FATAL_ERROR;
	else if (lLoggerSeverity == WARNING)
		m_lErrSev = WARNING_ERROR;
	else
		m_lErrSev = NON_ERROR;

	delete []pszMessage;
	pszMessage = NULL;
	return message;
}

CCreateAccessDatabase::~CCreateAccessDatabase()
{
	if (m_pCatalog)
		m_pCatalog= NULL;
    if (m_pTable)
		m_pTable= NULL;
	if (m_pIndexPtr)
		m_pIndexPtr= NULL;

}
