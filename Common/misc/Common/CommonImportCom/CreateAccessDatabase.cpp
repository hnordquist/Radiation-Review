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
