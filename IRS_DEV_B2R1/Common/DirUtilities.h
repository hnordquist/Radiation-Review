// DirUtilities.h: interface for the CDirUtilities class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afx.h>

class CDirUtilities  
{
public:
	
	CDirUtilities(bool bQuietMode);
	CDirUtilities();
	virtual ~CDirUtilities();

	void SetQuietMode(bool b){m_bQuietMode = b;}

	CString AddDriveLetter(const CString& strPath);
	CString AppendPath(const CString& strPath1, const CString& strPath2);

	void AppendBackslash(CString &cs);
	CString AppendBackslash(const CString& strStr);

	bool CopyDirectory(const CString &strSource, const CString &strDestination, const bool bOverwrite);

	bool FileExists(const CString& strFullPath);
	bool FileExists(const CString& strRootPath, const CString& strFilename);
	bool FileExists(const char* pstrFullPath);
	bool FileExists(const char* pstrRootPath, const char* pstrFilename);

	void GetDirectorySize(const CString& strDir, double* pdNumBytes, int* piNumFiles = NULL);
	CString StripPathFromFilename(const CString& strFilenameWithPath);
	bool OverwriteDbQuestion(const CString& strDbPath);
	bool IsDots(const TCHAR* str);
	bool DeleteDirectory(const TCHAR* sPath, CString *pcsFailedFilename);
	CString BuildPath(CString strPathtoBuild, CString strDriveLetter, bool bFailOnBadDriveSpec);

	bool m_bQuietMode;
};

