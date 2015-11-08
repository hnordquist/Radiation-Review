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
// CreateAccessDatabase.h: interface for the CCreateAccessDatabase class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#if !defined(AFX_CREATEACCESSDATABASE_H__2A91DBD8_CF1B_41B5_9BA5_91A52847A95F__INCLUDED_)
#define AFX_CREATEACCESSDATABASE_H__2A91DBD8_CF1B_41B5_9BA5_91A52847A95F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCreateAccessDatabase  
{
public:
	CCreateAccessDatabase(const CString& csDatabasePath, const CString& csDatabaseName, const CString& errINIFile);
	// This is a base class, make the CreateTables pure virtual.
	virtual bool CreateTables () = 0;
	virtual ~CCreateAccessDatabase();
	
	ADOX::_CatalogPtr m_pCatalog;
	ADOX::_TablePtr m_pTable;
	ADOX::_IndexPtr m_pIndexPtr;

	//Error stuff
	long m_lErrID;
	long m_lErrSev;
	CString m_csError;
	CString m_csErrorINIFile;
	CString FormatMessage(CString arg, long lMsgID);
	void SetError (CString err, long lErrID);
	void ClearError(void);

};

#endif // !defined(AFX_CREATEACCESSDATABASE_H__2A91DBD8_CF1B_41B5_9BA5_91A52847A95F__INCLUDED_)
