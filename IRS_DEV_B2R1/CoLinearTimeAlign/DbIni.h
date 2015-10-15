// DbIni.h: interface for the CDbIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBINI_H__052A6B0D_2D21_11D6_817E_00C04F60E89B__INCLUDED_)
#define AFX_DBINI_H__052A6B0D_2D21_11D6_817E_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDbIni  
{
public:
	
	CDbIni();
	virtual ~CDbIni();
	bool CopyDataStore(CString &strSourceRootPath, CString &strDestinationRootPath, bool bQuestion);

	CString m_strDefaultPath;
	CString m_strDataStoreRootPath;
	CString m_strDataStorePath;
	CString m_strMyIniFilename;
	CString m_strComponentName;
	bool m_bOpen;
	

};

#endif // !defined(AFX_DBINI_H__052A6B0D_2D21_11D6_817E_00C04F60E89B__INCLUDED_)
