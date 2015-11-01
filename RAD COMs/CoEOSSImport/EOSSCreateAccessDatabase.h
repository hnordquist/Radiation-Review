// EOSSCreateAccessDatabase.h: interface for the CEOSSCreateAccessDatabase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EOSSCREATEACCESSDATABASE_H__9D731111_38CE_4612_AD4D_8D953094A800__INCLUDED_)
#define AFX_EOSSCREATEACCESSDATABASE_H__9D731111_38CE_4612_AD4D_8D953094A800__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\COMMONIMPORTCOM\CreateAccessDatabase.h"

class CEOSSCreateAccessDatabase : public CCreateAccessDatabase  
{
public:
	bool CreateEOSSDataFileTable ();
	CEOSSCreateAccessDatabase(const CString& csDatabasePath, const CString& csDatabaseName, const CString& csErrINIFile);
	bool CreateTables ();
	bool CreateEOSSDataTable ();
	bool CreateDaysTable ();
	virtual ~CEOSSCreateAccessDatabase();

};

#endif // !defined(AFX_EOSSCREATEACCESSDATABASE_H__9D731111_38CE_4612_AD4D_8D953094A800__INCLUDED_)
