// AccessDataFile.h: interface for the CAccessDataFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCESSDATAFILE_H__2D7602C5_ED7D_11D5_816F_00C04F60E89B__INCLUDED_)
#define AFX_ACCESSDATAFILE_H__2D7602C5_ED7D_11D5_816F_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>		// 20-Jan-2006 PJM added for CList

//	Status Returns
#define IDATAFILE		100
#define iSKIP_FILE		IDATAFILE
#define iFILE_READ_ERR	IDATAFILE+1
#define	iFILE_OPEN_ERR	IDATAFILE+2

#include "AccessBaseData.h"

// 11-Jul-2005 SFK	Added
typedef struct tFileRecord
{
	CComVariant Date;  //This is the date of the data inside the file - not the date on the file.
	CComVariant StationID;
	CComVariant File;
} tFileRecord;

class CAccessDataFile {
public:
	

	CAccessDataFile(bool bOverwrite, bool bSkipOutOfOrder);
	virtual ~CAccessDataFile();

	virtual bool ReadDataFile(CDbAccess* pDb, const CString& strNameWithPath, CString* pstrErrorMsg = NULL) = 0;
	virtual bool ReadHeader(const CString& strNameWithPath, CString* pstrErrorMsg = NULL) = 0;
	// 11-Jul-2005 SFK	Added the next 3 functions
	virtual void GetCompleteFileList (
		short FacilityID,
		CList<tFileRecord,tFileRecord> *pFileList,
		const CString& Directory, 
		bool IncludeSubdirs) = 0;

	COleDateTime GetFileDate();
	long GetStationID();

	
	bool mbSkipOutOfOrder;
	bool mbOverwrite;
	int	 miErrorNum;

protected:
	
	bool OpenDataFile(const CString& strNameWithPath, CString *pstrErrorMsg = NULL);
	void CloseDataFile();

	int		miYr;
	int		miMon;
	int		miDay;
	//3-aug-2005 hn Added four digit year for use in all inherited classes of CVistaDataFile
	int		miYr4;
	DATE	mdTimestampOfFirstRecordInFile;
	short   msStaNum;
	double  mdVersionNumber;
	CString mstrFilenameWithPath;
	bool    mbOpen;
	FILE*   mpFile;
};

#endif // !defined(AFX_ACCESSDATAFILE_H__2D7602C5_ED7D_11D5_816F_00C04F60E89B__INCLUDED_)
