// BinaryDataFile.h: interface for the CBinaryDataFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINARYDATAFILE_H__E8101EA9_ED80_11D5_816F_00C04F60E89B__INCLUDED_)
#define AFX_BINARYDATAFILE_H__E8101EA9_ED80_11D5_816F_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AccessDataFile.h"

#define DUPLICATE_POINT_BIT 1
#define OUT_OF_ORDER_BIT 2

class CBinaryDataFile : public CAccessDataFile  
{
public:
	CBinaryDataFile(bool bQuietMode);
	virtual ~CBinaryDataFile();

	bool m_bQuietMode;

	bool ReadHeader(const CString &strNameWithPath, CString *pstrErrorMsg);
	bool ReadDataFile(CDbAccess* pDb, const CString &strNameWithPath, CString *pstrMsg);
	void GetCompleteFileList(short FacilityID, CList<tFileRecord, tFileRecord> *pFileList, const CString& Directory,  bool IncludeSubdirs);	// 11-Jul-2005 SFK added

private:
	bool ReadBinHeader(const CString &strNameWithPath, CString *pstrErrorMsg);
	bool ReadEvtHeader(const CString &strNameWithPath, CString *pstrErrorMsg);
	bool ReadBinDataFile(CDbAccess* pDb, const CString &strNameWithPath, CString *pstrMsg);
	bool ReadEvtDataFile(CDbAccess* pDb, const CString &strNameWithPath, CString *pstrMsg);

	


};

#endif // !defined(AFX_BINARYDATAFILE_H__E8101EA9_ED80_11D5_816F_00C04F60E89B__INCLUDED_)
