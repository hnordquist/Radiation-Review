// VifmDataFile.h: interface for the CVifmDataFile class.
//
#ifndef VIFMDATAFILE_H
#define VIFMDATAFILE_H

#include "VistaDataFile.h"
#include "VifmData.h"

class CVifmDataFile : public CVistaDataFile  
{
public:
	CVifmDataFile(bool bQuietMode);
	virtual ~CVifmDataFile();

	bool ReadDataFile(CDbVista* pDb, const CString &strNameWithPath, CString *pstrErrorMsg=NULL);
	bool ReadHeader(const CString& strNameWithPath, CString *pstrErrorMsg);

	bool ReadVBFHeader(const CString& strNameWithPath, CString *pstrErrorMsg);
	bool ReadVBFHeader(fpos_t pos, const CString& strNameWithPath, CString *pstrErrorMsg);

	bool FinishDayInDatabase(int i, CVifmData *pVInst, CString *pstrMsg);
	bool StartDayInDatabase(CVifmData *pVInst, int *pIndex, CString *pstrMsg);

	void GetCompleteFileList(
		short sFacilityID, 
		CList<tFileRecord, tFileRecord> *pFileList, 
		const CString& Directory,  
		bool bIncludeSubdirs);	// 06-Jul-2005 SFK added

private:

	//These are in the base class.
	//bool m_bQuietMode;
	//CDirUtilties m_Dir;
	//CMyDateTime m_MyDateTime;

	int IAEAReadDataFile (char * inputFileName, int * station_ID, int * numberOfRecords, double * samplingInterval);
	void SpikeCut (double dSamplingInterval, int iNumberOfRecords);
	void TimeToString (double timeStamp, char * timeString);
	//int FileIndexCreate (char * inputFileRoot);

    struct db_float_data_rec	mdbFloatData;	/* database record structure */
    unsigned long				mulPtsInDaySoFar; /* how many data pts in day so far */
    unsigned long				mulPtsInDay;	    /* how many data pts in entire day */
	DATE						mdFirstTimeInDay;
	DATE						mdLastTimeInDay;
	DATE						mdDayStart;
	DATE						mdFirstTimeInFile;
	DATE						mdTimestampOfLastRecordInFile;
    DATE						mdPrevTime;
	bool						mbDayAlreadyExists;
	bool						mbOverwrote;
	bool						mbFirstPoint;
	bool						mbAddedToBeginning;		// added to the beginning of day in db
	unsigned long				mulPtsToAddToEnd;		// number of points read out of db when adding to beginning of day
};

#endif // VIFMDATAFILE_H
