// EOSSDataFile.h: interface for the CEOSSDataFile class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "AccessDataFile.h"
#include "eossdefs.h"
#include "imports.h"
#include "EOSSData.h"

#define DUPLICATE_POINT_BIT 1
#define OUT_OF_ORDER_BIT 2

class CEOSSDataFile : public CAccessDataFile  
{
public:
	CEOSSDataFile(IFacilityMgrDataPtr pFacMgr, const bool bQuietMode);
	virtual ~CEOSSDataFile();

	//Functions to read eoss recs
	bool ReadHeader();
	bool ReadEOSSTable();
	bool ReadStatusRec();
	bool ReadTimeRec();
	bool ReadLogInfoRec();
	bool ReadSealLogPage();
	bool ReadSystemLogPage();
	bool ReadSOHLogPage();
	void ProcessSealEvent(int iEventNumber, SEALEVENT* pSealEvent);
	void ProcessSystemEvent(SYSTEMEVENT* pSystemEvent);
	void ProcessSOHRecord(SOHRECORD* pSOHRecord);
	void EossToSystemTime(SYSTEMTIME * pSystemTime, EOSSTIME * pEossTime);
	CString GetEossMessage(unsigned short usCode);
	bool StatRecOK (const STATUS& status);
	
	//EOSS data file fns
	bool ReadHeader(const CString &strNameWithPath, CString *pstrErrorMsg);
	bool ReadDataFile(CDbAccess* pDb, const CString &strNameWithPath, CString *pstrMsg);
	void GetCompleteFileList(short FacilityID, CList<tFileRecord, tFileRecord> *pFileList, const CString& Directory,  bool IncludeSubdirs);	// 11-Jul-2005 SFK added
	
	//void InitSealsState (int nNumSeals, COleDateTime& firstRecTime);
	IFacilityMgrDataPtr m_pIFacMgr;
	long GetStationID();
	
	//FOR DEBUG
	//FILE* m_outfile;

private:
	CEOSSData* m_pBInst;	

	CDirUtilities m_Dir;
	bool m_bQuietMode;
	short m_sOffset;
	
	//info from log records to get #events in file
	int m_nNumSealEventsProcessed;
	int m_nMaxSealEvents;
	int m_nSealEvents;
	int m_nMaxSystemEvents;
	int m_nSystemEvents;
	int m_nMaxSOHEvents;
	int m_nSOHEvents;

	//Event info
	long mlStaNum;
	COleDateTime m_dtRecordTime;
	short m_sCurrSealID;
	bool m_bSealState;
	
	// temporary vars to hold read data
	LANLHEADER m_sLANLHeader;
	unsigned int m_EOSSTable[MAXNUMEOSS];
	STATUS m_StatusRec;
	TIMEREC m_TimeRec;
	LOGINFO m_LogInfoRec;
	SEALLOGPAGE m_SealLogPage;
	SYSTEMLOGPAGE m_SystemLogPage;
	SOHLOGPAGE	m_SOHLogPage;

};