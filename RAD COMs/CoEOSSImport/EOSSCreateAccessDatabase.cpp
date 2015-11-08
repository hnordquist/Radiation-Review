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
