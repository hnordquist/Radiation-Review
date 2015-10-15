// ADO_DB.h : header file
#pragma once

#pragma warning ( push)
#pragma warning ( disable : 4146)

#import <msado15.dll> \
rename( "EOF", "adoEOF" ) \
rename_namespace( "ADODB" )

#import <msjro.dll> \
rename( "EOF", "jroEOF" ) \
rename_namespace( "JRO" )



#pragma warning ( pop)
//KDM: Note that files required by this class header are included in the stdafx.h file.
/////////////////////////////////////////////////////////////////////////////
// CAdoDb document

class CAdoDb
{
public:
	CAdoDb(const CString &strDbName = "", const CString &strDbPath = "",
		const CString &strDbPassword = "", const CString &strDbProvider = "");
	virtual ~CAdoDb();

	bool Open(const CString &strDbName = "", const CString &strDbPath = "", const CString &strDbPassword = "",
		CString *pstrErrorMsg = NULL, bool *pbPreviousOpenState = NULL);
	void Close();

	CString GetDbName(CString *pstrDbName = NULL, CString *pstrDbPath = NULL);

	// Caller to GetStrFieldValuesList needs to call delete [] astrValues
	UINT GetStrFieldValuesList(const CString &strTableName, const CString &strFieldName,
						CString **pastrValues, bool bDistinctValuesOnly = true);

	bool DoesTableExist(const CString &strTableName);
	int GetTableNames(CString **pastrTableNames);

	bool LockTable(const CString &strTblName, bool bLock);
	bool IsValidDbConnection();
	bool IsOpen() {return(m_bIsConnectionOpen);};
	bool ExecuteSqlCommand(const CString &strCmd);
	bool ExecuteSqlCommand(const CString &strCmd, long& lNumRecsAffected);
	void MakeNonEmpty(const CString &strValue, CString *pstrNonEmptyValue, const CString &strDefaultValue = "");
	void MakeNonEmpty(CString *pstrNonEmptyValue);
	virtual void LogDatabaseError(const char *szActivityName, _com_error &e, CString *pstrFullErrMsg = NULL);
	virtual void LogDatabaseError(const char *szActivityName, CString *pstrFullErrMsg = NULL);

	bool CompactDb(CString *pstrErrMsg = NULL, bool bOverwriteAsNeeded = true);
	bool IsReplicable();
	bool SynchronizeDb(const CString &strSecondDbPath);

	bool CreateWritableRecordSet(const CString &strQuery, ADODB::_RecordsetPtr *ppRecordSet, bool *pbWasOpen);
	bool AddRecordSet(ADODB::_RecordsetPtr pRecordSet, const COleSafeArray &avFieldNamesList,
							  const COleSafeArray &avFieldValuesList);
	bool CreateRecordSet(const CString &strQuery, ADODB::_RecordsetPtr *ppRecordSet, bool *pbWasOpen,
		UINT *puiNumItems = NULL, bool bEmptyIsSuccess = FALSE);
	void DestroyRecordSet( ADODB::_RecordsetPtr pRecordSet, bool bWasOpen);

	bool CreateUpdateableRecordSet(const CString &strTableName, const CString &strFilter,
						ADODB::_RecordsetPtr *ppRecordSet, bool *pbWasOpen, UINT *puiNumItems);

	bool GetRecordFieldString(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, CString *pstrValue, bool bSuppressErrorMsg = FALSE);

	bool GetRecordFieldDateTime(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, COleDateTime *pDateTime);

	bool GetRecordFieldBool(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, bool *pbValue);
	
	bool GetRecordFieldFloat(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, float *pfValue);
	bool GetRecordFieldDouble(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, double *pfValue);
	
	// Get 4-byte "long integer" from DB and return as a 32-bit int value.
	bool GetRecordFieldInt(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, int *pnValue);
	// Get 2-byte "integer" from DB and return as a 32-bit int value.
	bool GetRecordFieldShort(const ADODB::_RecordsetPtr pRecordSet,
				const CString &strValueLabel, int *pnValue);
	bool GetRecordFieldShort(const ADODB::_RecordsetPtr pRecordSet,		//KDM 5/13/03: Added prototype statement
				const CString &strValueLabel, short *pnValue);
//JCD for suppression of error messages
	void SuppressErrorMsg(bool bMsg) {m_bSuppressErrorMsg = bMsg;}
	bool IsSuppressingErrorMsg(){return m_bSuppressErrorMsg;}

	const CString *GetLastErrorMsg();

protected:
	CString m_strDbProvider;
	CString m_strDbPath;
	CString m_strDbName;
	CString m_strDbPassword;
	bool m_bSuppressErrorMsg;
	CString CreateConnectionString(const CString &strDbName = "", const CString &strDbPath = "",
						const CString &strDbProvider = "", const CString &strDbPassword = "");

	bool m_bIsConnectionOpen;
	ADODB::_ConnectionPtr m_pConnection;
	CString m_strLastDbErrorMsg;
	virtual void DisplayError(const CString &strErrMsg);
	CString GetErrorText(_com_error &e);
	void ReportError(const char *szActivityName, _com_error &e, const CString &strExtraInfo = "");
	void ReportError(const char *szActivityName, const CString &strExtraInfo = "");
	//const CString *GetLastErrorMsg();
	void ClearLastErrorMsg();
	CString m_strLastQuery;

	CString CreateFullyQualifiedDbName(const CString &strDbName, const CString &strDbPath);

	inline bool GetFieldValue(const ADODB::_RecordsetPtr pRecordSet, const CString &strValueLabel,
		_variant_t *pFieldValue, bool bSuppressErrorMsg = false);
	bool SetFieldValue(const ADODB::_RecordsetPtr pRecordSet, const CString &strValueLabel,
		const _variant_t &FieldValue, bool bSuppressErrorMsg = false);
	
	UINT m_uiNumRecordSetsCreated;
	UINT m_uiNumRecordSetsDestroyed;

	// Caller must "delete [] pacData".
	bool GetBinaryFromVariant(_variant_t &vData, char **pacData, DWORD *pdwDataDim);
};

