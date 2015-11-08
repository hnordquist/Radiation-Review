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
// ADO_DB.h : header file
#pragma once

#pragma warning ( push)
#pragma warning ( disable : 4146)

#import <C:\Program Files (x86)\Common Files\System\ado\msado15.dll> \
rename( "EOF", "adoEOF" ) \
rename_namespace( "ADODB" )

using namespace ADODB;

#import <C:\Program Files (x86)\Common Files\System\ado\msjro.dll> \
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

