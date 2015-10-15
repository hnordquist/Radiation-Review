#if !defined(VifmImport_H)
#define VifmImport_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// VifmImport.h : header file
//
//int OpenVifmImportDatabase(const char* strDataStorePath);
int OpenVifmImportDatabase(const short sFacilityID);

/////////////////////////////////////////////////////////////////////////////
// CVifmImport - provides link between traditional Rad and VifmImport com object
class CVifmImport 
{
public:

	CVifmImport::CVifmImport(const char* szDefaultPath);
	virtual ~CVifmImport();

	int  AddData(const short sFacID, const char* strFilename, BOOL bOverwrite, char** strResult);
	int  BackupDatabase(const short sFacID, const char *strPath, const bool bQuestion, const bool bPrompt);
	void CloseDatabase();
	int  GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime);
	int  GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays);
	int  GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_FLOAT_DATA_PT1 *pfData, unsigned long *pulNumPts, DATE *pdNextTime);
	int  GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_DOUBLE_DATA_PT1 *pdData, unsigned long *pulNumPts, DATE *pdNextTime);
	int  GetDataStatistics(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts);
	void GetDataStoreSize(const short sFacID,double *pdSizeInBytes);
	int  GetDayStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts);
	int  GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay);
	int  GetDbStartDay(const short sFacID, const short sStaID, DATE *pdStartDay, const bool bErrDisplay);
	int  InitializeDatabase(const short sFacID, const bool bQuestion);
	bool  OpenDatabase(const short sFacID);
	int  RestoreDatabase(const short sFacID,const char *strPath, const char *strDestPath, const bool bQuestion, const bool bPrompt);
//	int  SetDatabase(const char *strPath);

protected:

	char mstrDataStorePath[256];
	char mstrDefaultPath[256];
	short m_sFacNum;
	

//private:
};
extern CVifmImport *pglVifmImport;



#endif // !defined(VifmImport_H)