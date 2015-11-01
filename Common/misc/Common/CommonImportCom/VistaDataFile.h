// VistaDataFile.h: interface for the CVistaDataFile class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DirUtilities.h"
#include "MyDateTime.h"

//	Status Returns
#define IDATAFILE		100
#define iSKIP_FILE		IDATAFILE
#define iFILE_READ_ERR	IDATAFILE+1
#define	iFILE_OPEN_ERR	IDATAFILE+2

#include <afxtempl.h> // SCR00227 20-Jan-2006 added by pjm for CList definition
#include "VistaBaseData.h"

// Jun-2005 Added by hn
typedef struct tFileRecord
{
	CComVariant Date;  //This is the date of the data inside the file - not the date on the file.
	CComVariant StationID;
	CComVariant File;
} tFileRecord;

class CVistaDataFile  
{
public:
	CVistaDataFile(bool bOverwrite, bool bSkipOutOfOrder, bool bQuietMode);
	virtual ~CVistaDataFile();

	virtual bool ReadDataFile(CDbVista* pDb, const CString& strNameWithPath, CString* pstrErrorMsg = NULL) = 0;
	virtual bool ReadHeader(const CString& strNameWithPath, CString* pstrErrorMsg = NULL) = 0;
	virtual void GetCompleteFileList (short FacilityID,CList<tFileRecord,tFileRecord> *pFileList, const CString& Directory, 
		bool IncludeSubdirs) = 0;

	COleDateTime GetFileDate();
	long GetStationID();

	bool mbSkipOutOfOrder;
	bool mbOverwrite;
	int	 miErrorNum;

	bool m_bQuietMode;

	CDirUtilities m_Dir;
	CMyDateTime m_MyDateTime;
	
protected:
	
	bool OpenDataFile(const CString& strNameWithPath, CString *pstrErrorMsg = NULL);
	void CloseDataFile();

	DATE	mdTimestampOfFirstRecordInFile;
	int     miDay;
	int     miMon;
	int     miYr;
	//3-aug-2005 hn Added four digit year for use in all inherited classes of CVistaDataFile
	int		miYr4;
	short   msStaNum;
	double  mdVersionNumber;
	CString mstrFilenameWithPath;
	bool    mbOpen;
	FILE*   mpFile;
};

