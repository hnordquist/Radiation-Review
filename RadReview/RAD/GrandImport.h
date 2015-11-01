#if !defined(GrandImport_H)
#define GrandImport_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// GrandImport.h : header file
//

//19 Apr 2006 - pjm
//Deleted some comment lines.
//Added TestPoints function for debug.

//void LocalCoInitialize();		//DR 221
//void LocalCoUninitialize();	//DR 221
int OpenGrandImportDatabase(const short sFacilityID);

/////////////////////////////////////////////////////////////////////////////
// CGrandImport - provides link between traditional Rad and GrandImport com object
class CGrandImport 
{
public:

	CGrandImport::CGrandImport(const char* szDefaultPath, const short sFacNum);
	virtual ~CGrandImport();

	int  AddData(const short sFacID, const char* strFilename, BOOL bOverwrite, char** strResult);
	int  BackupDatabase(const short sFacID, const char *strPath, const bool bQuestion, const bool bPrompt);
	void CloseDatabase();
	int  GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime);
	int  GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays);	int  GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_FLOAT_DATA_PT1 *pfData, unsigned long *pulNumPts, DATE *pdNextTime);
	int  GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_DOUBLE_DATA_PT1 *pdData, unsigned long *pulNumPts, DATE *pdNextTime);
	int  GetDataStatistics(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts);
	void GetDataStoreSize(const short sFacID, double *pdSizeInBytes);
	int  GetDayStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts);
	int  GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay);
	int  GetDbStartDay(const short sFacID, const short sStaID, DATE *pdStartDay, const bool bErrDisplay);
	int  InitializeDatabase(const short sFacID, const bool bQuestion);;
	bool OpenDatabase(const short sFacID);
	int  RestoreDatabase(const short sFacID, const char *strPath, const char *strDestPath, const bool bQuestion, const bool bPrompt);

	void TestPoints();	//pjm 1-31-2006 for testing

protected:

	char mstrDataStorePath[256];
	char mstrDefaultPath[256];
	short m_sFacNum;
	

//private:
};
extern CGrandImport *pglGrandImport;



#endif // !defined(GrandImport_H)