// EOSSCreateAccessDatabase.cpp: implementation of the CEOSSCreateAccessDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EOSSCreateAccessDatabase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEOSSCreateAccessDatabase::CEOSSCreateAccessDatabase(const CString& csDatabasePath, const CString& csDatabaseName, const CString& csErrINIFile)
:CCreateAccessDatabase (csDatabasePath, csDatabaseName, csErrINIFile)
{

}

bool CEOSSCreateAccessDatabase::CreateTables ()
{
	bool bResult = false;
	bResult = CreateEOSSDataTable ();
	//Added this 4-29-08 to correctly store the last data record time that was read
	bResult = bResult && CreateEOSSDataFileTable ();
//	bResult = bResult && CreateDaysTable ();
	m_pCatalog = NULL;
	return bResult;
}

CEOSSCreateAccessDatabase::~CEOSSCreateAccessDatabase()
{
	if (m_pCatalog)
		m_pCatalog= NULL;
    if (m_pTable)
		m_pTable= NULL;

}

bool CEOSSCreateAccessDatabase::CreateEOSSDataTable ()
{
	bool bSuccess = false;
	//Create First table
	m_pTable.CreateInstance(__uuidof (ADOX::Table));

	if (m_pTable !=NULL && m_pCatalog !=NULL)
	{
		CString datatable = "EOSSBinaryData";
		m_pTable->PutName(datatable.AllocSysString ());
		m_pTable->Columns->Append("Date_Time",ADOX::adDate,16);
		m_pTable->Columns->Append("Date_Timestamp",ADOX::adDouble,32);
		m_pTable->Columns->Append("Reader_ID",ADOX::adInteger,16);
		m_pTable->Columns->Append("Seal_ID",ADOX::adInteger,16);
		m_pTable->Columns->Append("Binary_State",ADOX::adInteger,8);
		m_pCatalog->Tables->Append(_variant_t((IDispatch *)m_pTable));
		bSuccess = true;	
		// Now create a primary key and don't allow dups
		
		m_pIndexPtr.CreateInstance (__uuidof (ADOX::Index));

		if (m_pIndexPtr)
		{
			m_pIndexPtr->Name = "EOSSDataPK";
			m_pIndexPtr->PutPrimaryKey (VARIANT_TRUE);
			m_pIndexPtr->PutUnique (VARIANT_TRUE);
			m_pIndexPtr->Columns->Append ("Date_Timestamp",ADOX::adDouble,32);
			m_pIndexPtr->Columns->Append ("Seal_ID",ADOX::adInteger,16);
				
			m_pTable->Indexes->Append (_variant_t((IDispatch*) m_pIndexPtr));
			m_pCatalog->Tables->Refresh ();
			bSuccess = true;
		}
		else
			bSuccess = false;

		m_pTable  = NULL;		//Reset pointer so as to use again
	}
	else
	{
		bSuccess = false;
	}
	return bSuccess;
}

bool CEOSSCreateAccessDatabase::CreateDaysTable ()
{
	bool bSuccess = false;
	//Create First table
	m_pTable.CreateInstance(__uuidof (ADOX::Table));

	if (m_pTable !=NULL && m_pCatalog !=NULL)
	{
		CString datatable = "DaysDataTable";
		m_pTable->PutName(datatable.AllocSysString ());
		m_pTable->Columns->Append("Reader_ID",ADOX::adInteger,16);
		m_pTable->Columns->Append("DayStartTimestamp",ADOX::adDate,16);
		m_pTable->Columns->Append("DataStartTimestamp",ADOX::adDate,16);
		m_pTable->Columns->Append("DataEndTimestamp",ADOX::adDate,1);
		m_pTable->Columns->Append("NumberDataPoints",ADOX::adInteger,16);
		m_pCatalog->Tables->Append(_variant_t((IDispatch *)m_pTable));
		bSuccess = true;	
		// Now create a primary key and don't allow dups
		m_pCatalog->Tables->Refresh ();
		
		m_pTable  = NULL;		//Reset pointer so as to use again
	}
	else
	{
		bSuccess = false;
	}
	return bSuccess;
}

bool CEOSSCreateAccessDatabase::CreateEOSSDataFileTable()
{
	//Will create table with the single last date timestamp read into the EOSS data
	bool bSuccess = false;
	//Create First table
	m_pTable.CreateInstance(__uuidof (ADOX::Table));

	if (m_pTable !=NULL && m_pCatalog !=NULL)
	{
		CString datarecordstable = "EOSSDataRecordsReadTable";
		m_pTable->PutName(datarecordstable.AllocSysString ());
		m_pTable->Columns->Append("Date_Time_LastRecord",ADOX::adDate,16);
		m_pTable->Columns->Append("Date_Timestamp_LastRecord",ADOX::adDouble,32);
		m_pCatalog->Tables->Append(_variant_t((IDispatch *)m_pTable));
		bSuccess = true;	
		// Now create a primary key and don't allow dups
		
		m_pCatalog->Tables->Refresh ();

		m_pTable  = NULL;		//Reset pointer so as to use again
	}
	else
	{
		bSuccess = false;
	}
	return bSuccess;
}
