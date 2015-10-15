// EOSSImport.h: interface for the CEOSSImport class.

#ifndef EOSSIMPORT_H
#define EOSSIMPORT_H

class CEOSSImport  
{
public:
	int ConvertChanIDToIndex (short sFacID,long lStationID,short sChanID);
	CEOSSImport(const char* szDefaultPath, short sFacilityID);
	virtual ~CEOSSImport();

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

	int GetDataStoreTimeRange(
		const short sFacID, 
		const short sStaID,  //LONG??
		const BOOL bErrDisplay,
		DATE *pdStartDay,
		DATE *pdEndDay);

	int GetDayStatistics(
		const short sFacID, 
		const struct IDStruct *pStaID, 
		const DATE dStart, 
		const DATE dEnd, 
		DATE *pdRealStart, 
		DATE *pdRealEnd, 
		unsigned long *pulNumPts);

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
	
	int  GetDataByStation(
		const short sFacID,
		const short sStaID,
		const DATE dStart,
		const DATE dEnd,
		struct WHOLE_LONG_DATA_PT *pData,
		unsigned long *pulNumPts,
		DATE *pdNextTime);

	int  GetDataByStation(
		const short sFacID, 
		const short sStaID, 
		const DATE dStart, 
		const DATE dEnd, 
		struct WHOLE_DOUBLE_DATA_PT1 *pData, 
		unsigned long *pulNumPts, 
		DATE *pdNextTime);


	int InitializeDatabase(const short sFacilityID, const bool bQuestion);
	void CloseDatabase();
	int  RestoreDatabase(const short sFacilityID, const char *strPath, const char *strDestPath, const bool bQuestion, const bool bPrompt);
	int  BackupDatabase(const short sFacilityID, const char *strPath, const bool bQuestion, const bool bPrompt);

private:

	int GetDbStartEndDay(  //private
		const short sFacID, 
		const short sStaID,
		DATE *pdDay,
		const BOOL bErrDisplay,
		int which);

	char mstrDataStorePath[256];
	char mstrDefaultPath[256];
	short m_sFacilityID;

};
extern CEOSSImport* pglEOSSImport;

#endif //EOSSIMPORT_H
