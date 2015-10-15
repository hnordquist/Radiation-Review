// BinaryImport.h

#ifndef BINARYIMPORT_H
#define BINARYIMPORT_H

#include "FacilityConfig.h"

int OpenBinaryImportDatabase(const short sFacilityID);

class CBinaryImport 
{
public:

	CBinaryImport::CBinaryImport(const char* szDefaultPath);
	~CBinaryImport();

	int AddData(
		const short sFacID, 
		const char* strFilename, 
		BOOL bOverwrite, 
		char** strResult);

	int GetDataStatistics(
		const short sFacID, 
		const short sStaID, 
		const short sChanID, 
		const DATE dStart, 
		const DATE dEnd, 
		DATE *pdActualStart, 
		DATE *pdActualEnd, 
		unsigned long *pulNumPts);

	int GetDbStartDay(
		const short sFacID, 
		const short sStaID,
		DATE *dStartDay,
		const BOOL bErrDisplay);

	int GetDbEndDay(
		const short sFacID, 
		const short sStaID,
		DATE *dEndDay,
		const BOOL bErrDisplay);

	int GetDataByDay(
		const short sFacID, 
		const struct IDStruct *pStaID, 
		const DATE dStart, 
		const DATE dEnd, 
		struct DayDataStruct *pDayStruct, 
		unsigned long *pulNumDays);

	int GetDataByChannel(
		const short sFacID, 
		const short sStaID, 
		const short sChanID, 
		const DATE dStart, 
		const DATE dEnd, 
		double *pdX, 
		double *pdY, 
		unsigned long *pulNumPts, 
		DATE *pdNextTime, 
		short sStaType);
/*
	int GetDataStoreTimeRange(
		const short sFacID, 
		const short sStaID,  //LONG??
		const BOOL bErrDisplay,
		DATE *pdStartDay,
		DATE *pdEndDay);
*/
	
	int  BackupDatabase(const short sFacilityID, const char *strPath, const bool bQuestion, const bool bPrompt);
	void CloseDatabase();
	//int  GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime);
	//int  GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays);
	//int  GetDataStatistics(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts);
	void GetDataStoreSize(const short sFacID, double *pdSizeInBytes);

	int GetDayStatistics(
		const short sFacID, 
		const struct IDStruct *pStaID, 
		const DATE dStart, 
		const DATE dEnd, 
		DATE *pdRealStart, 
		DATE *pdRealEnd, 
		unsigned long *pulNumPts);

	//int  GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay);
	//int  GetDbStartDay(const short sFacID, const short sStaID, DATE *pdStartDay, const bool bErrDisplay);
	int  InitializeDatabase(const short sFacID, const bool bQuestion);
	bool  OpenDatabase(const short sFacID);
	int  RestoreDatabase(const short sFacilityID, const char *strPath, const char *strDestPath, const bool bQuestion, const bool bPrompt);
	int  SetDatabase(const char *strPath);
	int  GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_LONG_DATA_PT *pData, unsigned long *pulNumPts, DATE *pdNextTime);
	int  GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_DOUBLE_DATA_PT1 *pData, unsigned long *pulNumPts, DATE *pdNextTime);
	
protected:

	char mstrDataStorePath[256];
	char mstrDefaultPath[256];

};
extern CBinaryImport *pglBinaryImport;

#endif // BINARYIMPORT_H