// GrandDataFile.h: interface for the CGrandDataFile class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "VistaDataFile.h"  //CommonImportCom
#include "DirUtilities.h"
#include "GrandFacCfgWrap.h"

class CGrandDataFile : public CVistaDataFile  
{
public:
	CGrandDataFile(CGrandFacCfgWrap *pFacCfg, bool bQuietMode);
	virtual ~CGrandDataFile();

	//SCR00227
	long m_lFileOffset;
	long m_lUsableFileLength;
	bool m_bIsSignedFile;
	long m_lUsableBytesReadIn;
	long m_lFileLength;

	CGrandFacCfgWrap *m_pFacCfg;

	//These are in the base class
	//CDirUtilities m_Dir;
	//bool m_bQuietMode;
	//CMyDateTime m_MyDateTime;

	bool ReadDataFile(CDbVista* pDb, const CString &strNameWithPath, CString *pstrErrorMsg=NULL);
	bool ReadHeader(const CString& strNameWithPath, CString *pstrErrorMsg);
	void GetCompleteFileList(short FacilityID, CList<tFileRecord, tFileRecord> *pFileList, 
 						     const CString& Directory,  bool IncludeSubdirs);

	bool BI0;

};

