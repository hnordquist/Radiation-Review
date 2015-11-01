///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : AdoDb.cpp     
//Primary Author: Tom Marks

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "AdoDb.h"
#include <oledb.h>
#include <comip.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CAdoDb

CAdoDb::CAdoDb(const CString &strDbName, const CString &strDbPath,
			   const CString &strDbPassword, const CString &strDbProvider):
		m_bSuppressErrorMsg(false)
{
	if (strDbName.IsEmpty()) m_strDbName = "No database open";
	else m_strDbName = strDbName;

	m_strDbPath = strDbPath;

	if (strDbProvider.IsEmpty()) m_strDbProvider = "Provider=Microsoft.Jet.OLEDB.4.0";
	else m_strDbProvider = strDbProvider;

	m_strDbPassword = strDbPassword;

	m_bIsConnectionOpen = false;
	m_uiNumRecordSetsCreated = 0;
	m_uiNumRecordSetsDestroyed = 0;
}


CAdoDb::~CAdoDb()
{
	if (m_bIsConnectionOpen) {
		Close();
	}
}


bool CAdoDb::LockTable(const CString &strTblName, bool bLock)
{
	bool bSuccess = true;
	
	return(bSuccess);
}


void CAdoDb::Close()
{
	if (m_uiNumRecordSetsCreated != m_uiNumRecordSetsDestroyed) {
		CString strMsg;
		char szBuffer[50];

		strMsg = "Unequal number of record sets created and destroyed.\n\n";
		_itoa(m_uiNumRecordSetsCreated, szBuffer, 10);
		strMsg += szBuffer;
		strMsg += " created.\n";
		_itoa(m_uiNumRecordSetsDestroyed, szBuffer, 10);
		strMsg += szBuffer;
		strMsg += " destroyed.";
		//<<QA>> Remove MessageBox for now for NDAR.  Must put back for Baseline 3
		//<<QA>> Need to incorporate bQuietMode flag for Baseline 3
		//MessageBox(NULL, strMsg, "Database Error", MB_OK | MB_ICONERROR);
	}

	if (m_bIsConnectionOpen) {
		m_pConnection->Close();
		m_bIsConnectionOpen = false;
		m_pConnection = NULL;		// sfk 04/21/2003
		// Do NOT call m_pConnection->Release(); because m_pConnection is a "smart pointer".
	}

	// Don't clear m_strDbName; keep it around for possible re-open.
}


// Returns properly concatenated path + name and path and name if args are not NULL.
CString CAdoDb::GetDbName(CString *pstrDbName, CString *pstrDbPath)
{
	if (pstrDbName) *pstrDbName = m_strDbName;
	if (pstrDbPath) *pstrDbPath = m_strDbPath;
	return(CreateFullyQualifiedDbName(m_strDbName, m_strDbPath));
}


// Return proper concatenated path + name.
CString CAdoDb::CreateFullyQualifiedDbName(const CString &strDbName, const CString &strDbPath)
{
	CString strLocalDbPath = strDbPath;
	CString strFullyQualifiedDbName;

	if (!strLocalDbPath.IsEmpty()) {
		char c = strLocalDbPath[strLocalDbPath.GetLength() - 1];
		if ((c != ':') && (c != '\\')) {
			strLocalDbPath += '\\';
		}
	}

	strFullyQualifiedDbName = strLocalDbPath + strDbName;
	return(strFullyQualifiedDbName);
}


bool CAdoDb::ExecuteSqlCommand(const CString &strCmd)
{
	USES_CONVERSION;

	bool bSuccess = false;
	bool bDbWasOpen = IsOpen();
	bool bContinue;
	CString strMsg;

	m_strLastQuery.Empty();

	if (bDbWasOpen) bContinue = true;
	else bContinue = Open();

	if (bContinue) {
		if (IsValidDbConnection()) {
			_variant_t vRecsAffected(0L);
			ADODB::_RecordsetPtr pRecordSet;
//			_bstr_t bstrCmd(strCmd);

			try {
				pRecordSet = m_pConnection->Execute(T2BSTR(strCmd), &vRecsAffected, ADODB::adOptionUnspecified);
				// If the above operation did not throw an exception assume success.
				bSuccess = true;
			}
			catch( _com_error &e ) {
				strMsg.Format("ExecuteSqlCommand: %s", strCmd);
				ReportError(strMsg, e);
			}
			catch(...) {
				strMsg.Format("ExecuteSqlCommand: %s", strCmd);
				ReportError(strMsg);
			}
		}
		if (!bDbWasOpen) Close();
	}
	return(bSuccess);
}

bool CAdoDb::ExecuteSqlCommand(const CString &strCmd,long& lNumRecsAffected)
{
	USES_CONVERSION;

	bool bSuccess = false;
	bool bDbWasOpen = IsOpen();
	bool bContinue;
	CString strMsg;

	m_strLastQuery.Empty();

	if (bDbWasOpen) bContinue = true;
	else bContinue = Open();

	if (bContinue) {
		if (IsValidDbConnection()) {
			_variant_t vRecsAffected(0L);
			ADODB::_RecordsetPtr pRecordSet;
//			_bstr_t bstrCmd(strCmd);
			lNumRecsAffected = 0;
			
			try {
				pRecordSet = m_pConnection->Execute(T2BSTR(strCmd), &vRecsAffected, ADODB::adOptionUnspecified);
				// If the above operation did not throw an exception assume success.
				lNumRecsAffected = vRecsAffected.lVal;	
				bSuccess = true;
			}
			catch( _com_error &e ) {
				strMsg.Format("ExecuteSqlCommand: %s", strCmd);
				ReportError(strMsg, e);
			}
			catch(...) {
				strMsg.Format("ExecuteSqlCommand: %s", strCmd);
				ReportError(strMsg);
			}
		}
		if (!bDbWasOpen) Close();
	}
	return(bSuccess);
}

UINT CAdoDb::GetStrFieldValuesList(const CString &strTableName, const CString &strFieldName,
								CString **pastrValues, bool bDistinctValuesOnly)
{
	
	UINT uiNumItems = 0;
	ADODB::_RecordsetPtr pRecordSet;
	bool bDbWasOpen;
	CString strValue;
	CString strQuery;

	if (bDistinctValuesOnly) {
		strQuery.Format("SELECT DISTINCTROW [%s].[%s] FROM [%s] ORDER BY [%s]",
			strTableName, strFieldName, strTableName, strFieldName);
	}
	else {
		strQuery.Format("SELECT [%s].[%s] FROM [%s] ORDER BY [%s]",
			strTableName, strFieldName, strTableName, strFieldName);
	}

	if (CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bDbWasOpen)) {
		while (!pRecordSet->GetadoEOF()) {
			uiNumItems++;
			pRecordSet->MoveNext();
		}

		*pastrValues = new CString [uiNumItems];
		uiNumItems = 0;
		if (*pastrValues != NULL) {
			pRecordSet->MoveFirst();
			while (!pRecordSet->GetadoEOF()) {
				GetRecordFieldString(pRecordSet, strFieldName, &((*pastrValues)[uiNumItems]));
				uiNumItems++;
				pRecordSet->MoveNext();
			}
		}

		DestroyRecordSet(pRecordSet, bDbWasOpen);
	}
	return(uiNumItems);
}


void CAdoDb::ClearLastErrorMsg()
{
	m_strLastDbErrorMsg.Empty();
}

			
const CString *CAdoDb::GetLastErrorMsg()
{
	return(&m_strLastDbErrorMsg);
}


void CAdoDb::DisplayError(const CString &strErrMsg)
{
	if(!m_bSuppressErrorMsg)
	{
		//<<QA>> Remove MessageBox for now for NDAR.  Must put back for Baseline 3
		//<<QA>> Need to incorporate bQuietMode flag for Baseline 3
		//MessageBox(NULL, strErrMsg, "Database Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}

//following code is : USEFUL FOR DEBUGGING AND MAKING SURE THAT MESSAGE IS SUPPRESSED
//	else
//		MessageBeep(0);
}


CString CAdoDb::GetErrorText(_com_error &e)
{
	CString strErrorMsg, strErrorMsgPart;

	strErrorMsgPart.Format("\nCode = %08lx", e.Error());
	strErrorMsg += strErrorMsgPart;
	strErrorMsgPart.Format("\nCode meaning = %s", e.ErrorMessage());
	strErrorMsg += strErrorMsgPart;
	strErrorMsgPart.Format("\nSource = %s", (LPCSTR) e.Source());
	strErrorMsg += strErrorMsgPart;
	strErrorMsgPart.Format("\nDescription = %s", (LPCTSTR) e.Description());
	strErrorMsg += strErrorMsgPart;

	return(strErrorMsg);
}


void CAdoDb::ReportError(const char *szActivityName, _com_error &e, const CString &strExtraInfo)
{
	m_strLastDbErrorMsg = "DB COM exception:\n";
	m_strLastDbErrorMsg += szActivityName;

	if (strExtraInfo.GetLength() > 0) {
		m_strLastDbErrorMsg += "\n(" + strExtraInfo + ") ";
	}

	m_strLastDbErrorMsg += GetErrorText(e);

	if (m_strLastQuery.GetLength() > 0) {
		m_strLastDbErrorMsg += "\nCURRENT DATASET QUERY = {" + m_strLastQuery + "}";
	}

	//DisplayError(m_strLastDbErrorMsg);
}


void CAdoDb::ReportError(const char *szActivityName, const CString &strExtraInfo)
{
	m_strLastDbErrorMsg = "General exception:\n";
	m_strLastDbErrorMsg += szActivityName;

	if (strExtraInfo.GetLength() > 0) {
		m_strLastDbErrorMsg += "\n(" + strExtraInfo + ") ";
	}

	if (m_strLastQuery.GetLength() > 0) {
		m_strLastDbErrorMsg += "\nCURRENT DATASET QUERY = {" + m_strLastQuery + "}";;
	}

//	DisplayError(m_strLastDbErrorMsg);
}


/*
bool CAdoDb::DoesFieldExist(const CString &strTableName, const CString &strFieldName)
{
    ADODB::_RecordsetPtr  pRstSchema = NULL;
	bool bFieldExists = false;
	CString strLine;
	CString strMsg;
	
	try {
        pRstSchema = m_pConnection->OpenSchema(adSchemaColumns);

		while ((!bFieldExists) && (!pRstSchema->GetadoEOF())) {
            _bstr_t table_name = pRstSchema->Fields->GetItem("TABLE_NAME")->Value;
			CString strLocalTableName((const char *)table_name);
            _bstr_t table_type = pRstSchema->Fields->GetItem("TABLE_TYPE")->Value;
			CString strTableType((const char *)table_type);

			if (strTableType == "TABLE") {
				if (strTableName == strLocalTableName) {
					bFieldExists = true;
				}
			}
            pRstSchema->MoveNext();
        }
        pRstSchema->Close();
    }
    catch (_com_error &e) {
		ReportError("DoesTableExist:", e);
    }
	return(bFieldExists);
}
*/


bool CAdoDb::DoesTableExist(const CString &strTableName)
{
	ADODB::_RecordsetPtr  pRstSchema = NULL;
	bool bTableExists = false;
	CString strLine;
	CString strMsg;
	bool bContinue;
	bool bDbWasOpen = IsOpen();

	if (bDbWasOpen) bContinue = true;
	else bContinue = Open();

	if (bContinue) {
		try {
			pRstSchema = m_pConnection->OpenSchema(ADODB::adSchemaTables);

			while ((!bTableExists) && (!pRstSchema->GetadoEOF())) {
				_bstr_t table_name = pRstSchema->Fields->GetItem("TABLE_NAME")->Value;
				CString strLocalTableName((const char *)table_name);
				_bstr_t table_type = pRstSchema->Fields->GetItem("TABLE_TYPE")->Value;
				CString strTableType((const char *)table_type);

				if (strTableType == "TABLE") {
					if (strTableName == strLocalTableName) {
						bTableExists = true;
					}
				}
				pRstSchema->MoveNext();
			}
			pRstSchema->Close();
			pRstSchema = NULL;  //JD freaky fix for leak shutting down 18-aug-2006
		}
		catch (_com_error &e) {
			ReportError("DoesTableExist:", e);
		}
		if (!bDbWasOpen) Close();
	}
	return(bTableExists);
}


// CString *aTableNames;
// int nNumTables = Db.GetTableNames(&aTableNames);
// ... do whatever with array of names ...
// delete [] aTableNames;

int CAdoDb::GetTableNames(CString **pastrTableNames)
{
	bool bContinue;
	bool bDbWasOpen = IsOpen();

    ADODB::_RecordsetPtr  pRstSchema = NULL;
	int nNumTables = 0;
	*pastrTableNames = NULL;

	if (bDbWasOpen) bContinue = true;
	else bContinue = Open();

	if (bContinue) {
		try {
			pRstSchema = m_pConnection->OpenSchema(ADODB::adSchemaTables);

			while (!pRstSchema->GetadoEOF()) {
				_bstr_t table_type = pRstSchema->Fields->GetItem("TABLE_TYPE")->Value;
				CString strTableType((const char *)table_type);

				if (strTableType == "TABLE") {
					nNumTables++;
				}
				pRstSchema->MoveNext();
			}

			*pastrTableNames = new CString[nNumTables];
			if (*pastrTableNames == NULL) {
				nNumTables = 0;
			}
			else {
				int nTableIndex = 0;
				pRstSchema->MoveFirst();
				while (!pRstSchema->GetadoEOF()) {
					_bstr_t table_name = pRstSchema->Fields->GetItem("TABLE_NAME")->Value;
					CString strTableName((const char *)table_name);
					_bstr_t table_type = pRstSchema->Fields->GetItem("TABLE_TYPE")->Value;
					CString strTableType((const char *)table_type);

					if (strTableType == "TABLE") {
						(*pastrTableNames)[nTableIndex] = strTableName;
						nTableIndex++;
					}
					pRstSchema->MoveNext();
				}
			}

			pRstSchema->Close();
		}
		catch (_com_error &e) {
			ReportError("GetTableNames:", e);
		}
		if (!bDbWasOpen) Close();
	}
	return(nNumTables);
}


// ===============================================================================
// For Oracle connection see: http://www.site.uottawa.ca/~kavanagh/ADO-Oracle.html
// ===============================================================================

bool CAdoDb::Open(const CString &strDbName, const CString &strDbPath, const CString &strDbPassword,
			CString *pstrErrorMsg, bool *pbPreviousOpenState)
{
	bool bSuccess = false;
	HRESULT hr;

	CString strUserName = "";

	// If we are opening another (different) database make sure to close the currently-open one first.
	if (IsOpen()) {
		if (!strDbName.IsEmpty()) {
			if ((strDbName != m_strDbName) || (strDbPath != m_strDbPath)) {
				Close();
			}
		}
	}

	if (pbPreviousOpenState) *pbPreviousOpenState = IsOpen();

	if (IsOpen()) {
		bSuccess = true;
	}
	else {
		if (!strDbName.IsEmpty()) m_strDbName = strDbName;
		if (!strDbPath.IsEmpty()) m_strDbPath = strDbPath;
		if (!strDbPassword.IsEmpty()) m_strDbPassword = strDbPassword;

		CString strConnectionString = CreateConnectionString();

		try {
			hr = m_pConnection.CreateInstance(__uuidof(ADODB::Connection));
			if (SUCCEEDED(hr)) {
				try {
//					m_pConnection->Mode = (enum ConnectModeEnum) (adModeReadWrite | adModeShareDenyWrite);
					hr = m_pConnection->Open(
						_bstr_t(strConnectionString),
						_bstr_t(strUserName),
						_bstr_t(m_strDbPassword),
						ADODB::adModeUnknown);

					if (SUCCEEDED(hr)) {
						m_bIsConnectionOpen = true;
						bSuccess = true;
					}
					else {
						if (pstrErrorMsg) {
							*pstrErrorMsg = "Failed opening database.";
							*pstrErrorMsg += "File Name:";
							*pstrErrorMsg += GetDbName();
							*pstrErrorMsg += "\n";
						}
					}
				}
				catch( _com_error &e ) {
					// Here if e.g. database file not found.
					
					ReportError("Open:", e);

					if (pstrErrorMsg) {
						*pstrErrorMsg = "Exception thrown opening database connection.\n";
						*pstrErrorMsg += "File Name: ";
						*pstrErrorMsg += GetDbName();
						*pstrErrorMsg += "\n";
						// Append info from _com_error
						*pstrErrorMsg += GetErrorText(e);
					}
				}
				catch(...) {
					ReportError("Open: ");
				}
			}
			else {
				// Here if e.g. the msadoxx.dll is missing.
				if (pstrErrorMsg) *pstrErrorMsg = "Failed creating database connection instance.";
			}
		}
		catch( _com_error &e ) {
			ReportError("Open:", e);

			if (pstrErrorMsg) {
				*pstrErrorMsg = "Exception thrown creating database connection instance.\n";
				// Append info from _com_error
				*pstrErrorMsg += GetErrorText(e);
			}
		}
		catch(...) {
			ReportError("Open: ");
		}
	}

	return(bSuccess);
}


// Obtain information from the Errors Collection
HRESULT LogAdoErrorImport(ADODB::_ConnectionPtr pConn)
{
	ADODB::ErrorsPtr   pErrors;
	ADODB::ErrorPtr    pError;
	CString     strTmp;
	HRESULT     hr = (HRESULT) 0L;
	long        nCount;
	CString strMsg,MsgPart;
	
	// Don't have an un-handled exception in the handler that
	// handles exceptions!
	try	{
		pErrors = pConn->GetErrors();
		
		nCount = pErrors->GetCount();
		
		for( long i = 0; (!FAILED(hr)) && (i < nCount); i++ )
		{
			MsgPart.Format("\t Dumping ADO Error %d of %d", i+1, nCount);
			strMsg += MsgPart;
			
			hr = pErrors->get_Item((_variant_t)((long)i), &pError );
			
			_bstr_t bstrSource     ( pError->GetSource()      );
			_bstr_t bstrDescription( pError->GetDescription() );
			_bstr_t bstrHelpFile   ( pError->GetHelpFile()    );
			_bstr_t bstrSQLState   ( pError->GetSQLState()    );
			
			MsgPart.Format("\n Number      = %ld", pError->GetNumber()       );
			strMsg += MsgPart;
			MsgPart.Format("\n Source      = %s",  (LPCTSTR) bstrSource      );
			strMsg += MsgPart;
			MsgPart.Format("\n Description = %s",  (LPCTSTR) bstrDescription );
			strMsg += MsgPart;
			MsgPart.Format("\n HelpFile    = %s",  (LPCTSTR) bstrHelpFile    );
			strMsg += MsgPart;
			MsgPart.Format("\n HelpContext = %ld", pError->GetHelpContext()  );
			strMsg += MsgPart;
			MsgPart.Format("\n SQLState    = %s",  (LPCTSTR) bstrSQLState    );
			strMsg += MsgPart;
			MsgPart.Format("\n HelpContext = %ld", pError->GetHelpContext()  );
			strMsg += MsgPart;
			MsgPart.Format("\n NativeError = %ld", pError->GetNativeError()  );
			strMsg += MsgPart;
		}
	}

	catch( CException *e ) {
		MsgPart.Format("*** UNABLE TO LOG EXCEPTION ***" );
		strMsg += MsgPart;
		e->Delete();
	}
	catch(...) {
		MsgPart.Format("*** UNABLE TO LOG EXCEPTION ***" );
		strMsg += MsgPart;
	}
	
	if (!strMsg.IsEmpty()) 
	{
		//<<QA>> Remove MessageBox for now for NDAR.  Must put back for Baseline 3
		//<<QA>> Need to incorporate bQuietMode flag for Baseline 3
		//MessageBox(NULL, strMsg, "Database Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
	return hr;
}


void CAdoDb::LogDatabaseError(const char *szActivityName, _com_error &e, CString *pstrFullErrMsg)
{
	m_strLastDbErrorMsg = "DB COM exception:\n";
	m_strLastDbErrorMsg += szActivityName;
	CString strErrorMsgPart;

	m_strLastDbErrorMsg += GetErrorText(e);

	// TBD -- needs to actually log error to DB.

	// Return error msg text to caller if requested.
	if (pstrFullErrMsg) *pstrFullErrMsg = m_strLastDbErrorMsg;
}


void CAdoDb::LogDatabaseError(const char *szActivityName, CString *pstrFullErrMsg)
{
	m_strLastDbErrorMsg = "General exception:\n";
	m_strLastDbErrorMsg += szActivityName;

	// TBD -- needs to actually log error to DB.

	// Return error msg text to caller if requested.
	if (pstrFullErrMsg) *pstrFullErrMsg = m_strLastDbErrorMsg;
}


bool CAdoDb::GetFieldValue(const ADODB::_RecordsetPtr pRecordSet, const CString &strValueLabel,
									 _variant_t *pFieldValue, bool bSupressErrorMsg)
{
	bool bValid = false;

	// GetField value has nothing to do with a query so clear last query so that
	// logic in ReportError will not create a misleading error rmsg.
	m_strLastQuery.Empty();

	try {
		*pFieldValue = pRecordSet->GetCollect((_variant_t )strValueLabel);
		if ((pFieldValue->vt != VT_NULL) && (pFieldValue->vt != 0)) {
			bValid = TRUE;
		}
	}
	catch(_com_error &e) {
		if (!bSupressErrorMsg) {
			ReportError("GetFieldValue: ", e, strValueLabel);
		}
	}
	catch(...) {
		if (!bSupressErrorMsg) {
			ReportError("GetFieldValue: ", strValueLabel);
		}
	}
	return(bValid);
}


bool CAdoDb::SetFieldValue(const ADODB::_RecordsetPtr pRecordSet, const CString &strValueLabel,
									 const _variant_t &FieldValue, bool bSupressErrorMsg)
{
	bool bValid = false;

	// SetField value has nothing to do with a query so clear last query so that
	// logic in ReportError will not create a misleading error msg.
	m_strLastQuery.Empty();

	try {
		if ((FieldValue.vt != VT_NULL) && (FieldValue.vt != 0)) {
			pRecordSet->PutCollect((_variant_t )strValueLabel, FieldValue);
			bValid = true;
		}
	}
	catch(_com_error &e) {
		if (!bSupressErrorMsg) {
			ReportError("SetFieldValue: ", e, strValueLabel);
		}
	}
	catch(...) {
		if (!bSupressErrorMsg) {
			ReportError("SetFieldValue: ", strValueLabel);
		}
	}
	return(bValid);
}


bool CAdoDb::GetRecordFieldBool(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, bool *pbValue)
{
	bool bSuccess = false;
	_variant_t vFieldValue;

	try {
		if (GetFieldValue(pRecordSet, strValueLabel, &vFieldValue)) {
			// I don't understand it but the .bval field comes 0 or 255 (-1) for FALSE / TRUE.
			*pbValue = (vFieldValue.bVal != 0);
			bSuccess = true;
		}
	}
	catch(...) {
		ReportError("GetRecordFieldBool: ");
	}

	return(bSuccess);
}


bool CAdoDb::GetRecordFieldString(const ADODB::_RecordsetPtr pRecordSet, const CString &strValueLabel,
										CString *pstrValue, bool bSupressErrorMsg)
{
	bool bSuccess = false;
	_variant_t vFieldValue;

	try {
		if (GetFieldValue(pRecordSet, strValueLabel, &vFieldValue, bSupressErrorMsg)) {
			*pstrValue = vFieldValue.bstrVal;
			bSuccess = true;
		}
		else {
			// On 6-Oct-00 added check for VT_NULL and call this success and return empty string.
			if (vFieldValue.vt == VT_NULL) {
				*pstrValue = "";
				bSuccess = true;
			}
			else {
				*pstrValue = "Error";
			}
		}
	}
	catch(...) {
		ReportError("GetRecordFieldString: ", strValueLabel);
	}
	return(bSuccess);
}


bool CAdoDb::GetRecordFieldFloat(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, float *pfValue)
{
	bool bSuccess = false;
	_variant_t vFieldValue;

	try {
		if (GetFieldValue(pRecordSet, strValueLabel, &vFieldValue)) {
			*pfValue = vFieldValue.fltVal;
			bSuccess = true;
		}
	}
	catch(...) {
		ReportError("GetRecordFieldFloat: ");
		*pfValue = 0.0F;
	}
	return(bSuccess);
}


bool CAdoDb::GetRecordFieldDouble(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, double *pdValue)
{
	bool bSuccess = false;
	_variant_t vFieldValue;

	try {
		if (GetFieldValue(pRecordSet, strValueLabel, &vFieldValue)) {
			*pdValue = vFieldValue.dblVal;
			bSuccess = true;
		}
	}
	catch(...) {
		ReportError("GetRecordFieldDouble: ");
		*pdValue = 0.0F;
	}
	return(bSuccess);
}


// Get 4-byte "long integer" from DB and return as a 32-bit int value.
bool CAdoDb::GetRecordFieldInt(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, int *pnValue)
{
	bool bSuccess = false;
	_variant_t vFieldValue;

	try {
		if (GetFieldValue(pRecordSet, strValueLabel, &vFieldValue)) {
			*pnValue = vFieldValue.lVal;
			bSuccess = true;
		}
	}
	catch(...) {
		ReportError("GetRecordFieldInt: ");
	}
	return(bSuccess);
}


// Get 2-byte "integer" from DB and return as a 32-bit int value.
bool CAdoDb::GetRecordFieldShort(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, int *pnValue)
{
	bool bSuccess = false;
	_variant_t vFieldValue;

	try {
		if (GetFieldValue(pRecordSet, strValueLabel, &vFieldValue)) {
			// *pnValue = vFieldValue.intVal;
			*pnValue = vFieldValue.iVal;
			bSuccess = true;
		}
	}
	catch(...) {
		ReportError("GetRecordFieldShort: ");  //KDM 5/13/03: Corrected the reported error message.
	}
	return(bSuccess);
}
// Get 2-byte "integer" from DB and return as a short value.  //KDM - 5/13/03: Added this method
bool CAdoDb::GetRecordFieldShort(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, short *pnValue)
{
	bool bSuccess = false;
	_variant_t vFieldValue;

	try {
		if (GetFieldValue(pRecordSet, strValueLabel, &vFieldValue)) {
			// *pnValue = vFieldValue.intVal;
			*pnValue = vFieldValue.iVal;
			bSuccess = true;
		}
	}
	catch(...) {
		ReportError("GetRecordFieldShort: ");
	}
	return(bSuccess);
}


bool CAdoDb::GetRecordFieldDateTime(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, COleDateTime *pDateTime)
{
	bool bSuccess = false;
	_variant_t vFieldValue;

	try {
		if (GetFieldValue(pRecordSet, strValueLabel, &vFieldValue)) {
			*pDateTime = vFieldValue.date;
			bSuccess = true;
		}
	}
	catch(...) {
		ReportError("GetRecordFieldDateTime: ");
	}
	return(bSuccess);
}


void CAdoDb::MakeNonEmpty(const CString &strValue, CString *pstrNonEmptyValue, const CString &strDefaultValue)
{
	if (strValue.IsEmpty()) {
		if (strDefaultValue != "") *pstrNonEmptyValue = strDefaultValue;
		else *pstrNonEmptyValue = "?";
	}
	else *pstrNonEmptyValue = strValue;
}


void CAdoDb::MakeNonEmpty(CString *pstrNonEmptyValue)
{
	if (pstrNonEmptyValue->IsEmpty()) {
		*pstrNonEmptyValue = "?";
	}
}


bool CAdoDb::IsValidDbConnection()
{
	return(m_bIsConnectionOpen);
}


/*
//Example for using CreateWritableRecordSet and AddRecordSet:
void AddRecordsTest(CAdoDb *pDb)
{
	bool bSuccess;
	ADODB::_RecordsetPtr pRecordSet;
	bool bWasOpen;
	long lArrayIndex[1];
	CString strValue;
	CString strSqlQuery = "SELECT * FROM [Citizenship] WHERE [Country] IS NULL";

	if (pDb->CreateWritableRecordSet(strSqlQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen)) {
		lArrayIndex[0] = 0;
		COleSafeArray avarFieldList;
		avarFieldList.CreateOneDim(VT_VARIANT, 1);
		avarFieldList.PutElement(lArrayIndex, &(_variant_t("Country")));
		
		COleSafeArray avarValueList;
		avarValueList.CreateOneDim(VT_VARIANT, 1);
		avarValueList.PutElement(lArrayIndex, &(_variant_t("CC")));

		for (int n = 0; n < 200; n++) {
			strValue.Format("%d", n);
			avarValueList.PutElement(lArrayIndex, &((_variant_t)strValue));
			bSuccess = pDb->AddRecordSet(pRecordSet, avarFieldList, avarValueList);

			if (n == 100) {
				ReportNumRecs(pDb);
			}
		}
		pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen);
	}
}
*/

bool CAdoDb::CreateWritableRecordSet(const CString &strQuery, ADODB::_RecordsetPtr *ppRecordSet, bool *pbWasOpen)
{
	bool bSuccess = false;
	bool bContinue;
	_bstr_t bstrQuery(strQuery);
	_variant_t vRecsAffected(0L);
	ADODB::_RecordsetPtr pRecordSet;
	_variant_t vNull;
	HRESULT hr;
	vNull.vt = VT_ERROR;
	vNull.scode = DISP_E_PARAMNOTFOUND;
	
	*ppRecordSet = NULL;
	*pbWasOpen = IsOpen();
	m_strLastQuery.Empty();

	if (*pbWasOpen) bContinue = true;
	else {
		bContinue = Open();
	}

	if (bContinue) {
		if (IsValidDbConnection()) {
			try {
				hr = pRecordSet.CreateInstance(_uuidof(ADODB::Recordset));
				pRecordSet->PutRefActiveConnection(m_pConnection);
				hr = pRecordSet->Open(bstrQuery, vNull, ADODB::adOpenForwardOnly, ADODB::adLockOptimistic, ADODB::adCmdText);
				if (hr==S_OK) {//pRecordSet->GetadoEOF()
					bSuccess = true;
					*ppRecordSet = pRecordSet;
					m_uiNumRecordSetsCreated++;
				}
			}
			catch( _com_error &e ) {
				CString strMsg = "CreateRecordSet - Connection execute; \nQuery = \"";
				strMsg += strQuery;
				strMsg += "\" ";
				ReportError(strMsg, e);
			}
			catch(...) {
				ReportError("CreateRecordSet: ");
			}
		}
	}

	return(bSuccess);
}


bool CAdoDb::AddRecordSet(ADODB::_RecordsetPtr pRecordSet, const COleSafeArray &avFieldNamesList,
						  const COleSafeArray &avFieldValuesList)
{
	bool bSuccess = false;
	try {
		pRecordSet->AddNew(avFieldNamesList, avFieldValuesList);
		bSuccess = true;
	}
	catch( _com_error &e ) {
		ReportError("AddRecordSet:", e);
	}
	catch(...) {
		ReportError("AddRecordSet: ");
	}
	return(bSuccess);
}


bool CAdoDb::CreateRecordSet(const CString &strQuery, ADODB::_RecordsetPtr *ppRecordSet,
						   bool *pbWasOpen, UINT *puiNumItems, bool bEmptyIsSuccess)
{
	bool bSuccess = false;
	bool bContinue;
	_bstr_t bstrQuery(strQuery);
	_variant_t vRecsAffected(0L);
	ADODB::_RecordsetPtr pRecordSet;
	
	*ppRecordSet = NULL;

	*pbWasOpen = IsOpen();

	if (*pbWasOpen) bContinue = true;
	else {
		bContinue = Open();
	}

	if (bContinue) {
		if (IsValidDbConnection()) {
			try {
				m_strLastQuery.Empty();
				pRecordSet = m_pConnection->Execute(bstrQuery, &vRecsAffected, ADODB::adOptionUnspecified);
				if (pRecordSet->GetadoEOF()) {
					if (puiNumItems) *puiNumItems = 0;

					if (!bEmptyIsSuccess) {
						pRecordSet->Close();
					}
					else {
						bSuccess = true;
						*ppRecordSet = pRecordSet;
						m_uiNumRecordSetsCreated++;
					}
				}
				else {
					bSuccess = true;
					*ppRecordSet = pRecordSet;
					m_uiNumRecordSetsCreated++;
					if (puiNumItems) {
						*puiNumItems = 0;
						while (!pRecordSet->GetadoEOF()) {
							(*puiNumItems)++;
							pRecordSet->MoveNext();
						}
						pRecordSet->MoveFirst();
					}
				}
			}
			catch( _com_error &e ) {
				CString strMsg = "CreateRecordSet - Connection execute; \nQuery = \"";
				strMsg += strQuery;
				strMsg += "\" ";
				ReportError(strMsg, e);
			}
			catch(...) {
				ReportError("CreateRecordSet: ");
			}
		}
	}

	if (!bSuccess) {
		if (!(*pbWasOpen)) Close();
	}
	return(bSuccess);
}


void CAdoDb::DestroyRecordSet( ADODB::_RecordsetPtr pRecordSet, bool bWasOpen)
{
	try {
		m_strLastQuery.Empty();
		pRecordSet->Close();
		m_uiNumRecordSetsDestroyed++;

		if (!bWasOpen) Close();
	}
	catch( _com_error &e ) {
		ReportError("DestroyRecordSet:", e);
	}
	catch(...) {
		ReportError("DestroyRecordSet: ");
	}
}


// Caller must "delete [] ppcBuf".
bool CAdoDb::GetBinaryFromVariant(_variant_t &vData, char **pacData, DWORD *pdwDataDim)
{
	bool bRetVal = false;

	*pacData = NULL;
	*pdwDataDim = 0;

	//Binary data is stored in the variant as an array of unsigned char
	if ((VT_ARRAY | VT_UI1) == vData.vt) {		 // OLE SAFEARRAY
		// Retrieve size of array
		int nBufLen = vData.parray->rgsabound[0].cElements;
		void *pArrayData;

		HRESULT Status = SafeArrayAccessData(vData.parray, &pArrayData );

		if (Status == S_OK) {
			*pacData = new char[nBufLen]; 
			if (*pacData != NULL) {
				bRetVal = true;
				// Copy BLOB data into newly created buffer pointed to by passed-in pointer.
				::CopyMemory(*pacData, pArrayData, nBufLen);
				*pdwDataDim = nBufLen;
			}
			SafeArrayUnaccessData(vData.parray);
		}
	}
	return bRetVal;
}


CString CAdoDb::CreateConnectionString(const CString &strDbName, const CString &strDbPath,
						const CString &strDbProvider, const CString &strDbPassword)
{
	CString strLocalDbName;
	if (strDbName.IsEmpty()) strLocalDbName = m_strDbName;
	else strLocalDbName = strDbName;

	CString strLocalDbPath;
	if (strDbPath.IsEmpty()) strLocalDbPath = m_strDbPath;
	else strLocalDbPath = strDbPath;

	CString strLocalDbProvider;
	if (strDbProvider.IsEmpty()) strLocalDbProvider = m_strDbProvider;
	else strLocalDbProvider = strDbProvider;

	CString strLocalDbPassword;
	if (strDbPassword.IsEmpty()) strLocalDbPassword = m_strDbPassword;
	else strLocalDbPassword = strDbPassword;

	CString strDbNamePlusPath = CreateFullyQualifiedDbName(strLocalDbName, strLocalDbPath);

	CString strConnectionString = strLocalDbProvider + ";Data Source=" + strDbNamePlusPath;

//	if (!strLocalDbPassword.IsEmpty()) {
//		strConnectionString += ";Password=" + strLocalDbPassword;
//	}
	return(strConnectionString);
}


bool CAdoDb::CompactDb(CString *pstrErrMsg, bool bOverwriteAsNeeded)
{
	OutputDebugString("CAdoDb::CompactDb()\n");

	const CString strCOMPACTED_DB_NAME = "CompactedTemp.mdb";

	CString strCompactedDbName = CreateFullyQualifiedDbName(strCOMPACTED_DB_NAME, m_strDbPath);
	bool bSuccess = false;
	bool bWasOpen = IsOpen();

	if (bWasOpen) Close();

	if (pstrErrMsg) pstrErrMsg->Empty();

	if (bOverwriteAsNeeded) {
		// Make sure there is not leftover compacted file from before because Compact fn will
		// not over-write an already-existing file.
		try {
			CFile File;
			if (File.Open(strCompactedDbName, CFile::modeRead)) {
				File.Close();
				CFile::Remove(strCompactedDbName);
			}
		}
		catch (CFileException *e) {
			if (pstrErrMsg) {
				*pstrErrMsg = "File handling exception in CAdoDb::CompactDb.\nFilename: ";
				*pstrErrMsg += e->m_strFileName + "\n";

				const UINT uiDIM = 500;
				char szErrMsg[uiDIM];
				if (e->GetErrorMessage(szErrMsg, uiDIM)) {
					*pstrErrMsg += szErrMsg;
				}
			}
		}
	}

	try {
		JRO::IJetEnginePtr pJet(__uuidof(JRO::JetEngine));
		if (pJet != NULL) {
			HRESULT hr;
			CString strConnection = CreateConnectionString();
			CString strDestConnection = CreateConnectionString(strCOMPACTED_DB_NAME);
			
			// convert to bstr type to pass as args to CompactDatabase fn.
			_bstr_t bstr1(strConnection);
			_bstr_t bstr2(strDestConnection);
			hr = pJet->CompactDatabase(bstr1, bstr2);
			if (SUCCEEDED(hr)) {
				bSuccess = true;
				// Need to destroy original and copy over compacted one.
				CFile::Remove(GetDbName());
				CFile::Rename(strCompactedDbName, GetDbName());
			}
		}
	}
	catch (_com_error &e) {
		if (pstrErrMsg) {
			*pstrErrMsg += "CAdoDb::CompactDb: " + GetErrorText(e) + "Target File Name: " + strCompactedDbName;
		}
		else ReportError("CAdoDb::CompactDb: ", e, "Target File Name: " + strCompactedDbName);
	}

	if (bWasOpen) Open();
	return(bSuccess);
}


bool CAdoDb::IsReplicable()
{
	bool bIsReplicable = false;
	bool bWasOpen = IsOpen();

	if (!bWasOpen) Open();

	if (IsOpen()) {
		try {
			JRO::IReplicaPtr pMaster(__uuidof(JRO::Replica));
			if (pMaster) {
				pMaster->PutRefActiveConnection(m_pConnection);
				enum JRO::ReplicaTypeEnum eType = pMaster->GetReplicaType();
				if (eType != JRO::jrRepTypeNotReplicable) bIsReplicable = true;
			}
		}
		catch (_com_error &e) {
			ReportError("CAdoDb::IsReplicable: ", e);
		}

		if (!bWasOpen) Close();
	}

//	jrRepTypeNotReplicable = 0,
//	jrRepTypeDesignMaster = 1,
//	jrRepTypeFull = 2,
//	jrRepTypePartial = 3
	return(bIsReplicable);
}

	
bool CAdoDb::SynchronizeDb(const CString &strSecondDbPath)
{
	bool bSuccess = false;
	CFile File;

	if (File.Open(strSecondDbPath, CFile::modeRead)) {	// Does it exist?
		File.Close();

		bool bWasOpen = IsOpen();
		if (!bWasOpen) Open();

		if (IsOpen()) {
			try {
				JRO::IReplicaPtr pMaster(__uuidof(JRO::Replica));
				if (pMaster) {
					HRESULT hr;
					pMaster->PutRefActiveConnection(m_pConnection);
					_bstr_t bstrTarget(strSecondDbPath);

					hr = pMaster->Synchronize(bstrTarget, JRO::jrSyncTypeImpExp, JRO::jrSyncModeDirect);
					if (SUCCEEDED(hr)) {
						bSuccess = true;
					}
				}
			}
			catch (_com_error &e) {
				ReportError("CAdoDb::SynchronizeDb: ", e);
			}

			if (!bWasOpen) Close();
		}
	}
	return(bSuccess);
}
