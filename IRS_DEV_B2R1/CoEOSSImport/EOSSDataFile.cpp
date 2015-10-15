// EOSSDataFile.cpp: implementation of the CEOSSDataFile class.
//
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support the virtual data file functions
//	specific to the *.bin, *.evt/*.bny files formed for EOSS data.
//
//	Functions:
//		ReadHeader
//		ReadDataFile
//		ConvertEOSSTimeToDATE
//		EossToSystemTime
//		IsRecordValid
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include "resource.h"

#include "EOSSDataFile.h"

#include "AccessBaseData.h"

#define dINCREMENT 1.0/8640000.0		// 1/100th of a second
#define OPENED 1
#define CLOSED 0

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSSDataFile::CEOSSDataFile(IFacilityMgrDataPtr pFacMgr, const bool bQuietMode)
	:CAccessDataFile(false,false)
{
	m_pIFacMgr = pFacMgr;
	m_bQuietMode = bQuietMode;
	m_pBInst=NULL;
}

CEOSSDataFile::~CEOSSDataFile()
{
	if (m_pBInst)
	{
		delete m_pBInst;
		m_pBInst = NULL;
	}
}

bool CEOSSDataFile::ReadHeader(const CString &strNameWithPath, CString *pstrErrorMsg)
{

	bool bResult = true;
	
	if (fread(&m_sLANLHeader,sizeof(m_sLANLHeader),1,mpFile) != 1) bResult = false;

	//do any header validation you need to do here
	if (bResult)
	{
		char szTemp[64];
		memcpy(szTemp,m_sLANLHeader.SizeOfHeader,sizeof(m_sLANLHeader.SizeOfHeader));
		szTemp[sizeof(m_sLANLHeader.SizeOfHeader)] = NULL;

		memcpy(szTemp,m_sLANLHeader.Version,sizeof(m_sLANLHeader.Version));
		szTemp[sizeof(m_sLANLHeader.Version)] = NULL;
		mdVersionNumber = atof (szTemp);

		memcpy(szTemp,m_sLANLHeader.StationId,sizeof(m_sLANLHeader.StationId));
		szTemp[sizeof(m_sLANLHeader.StationId)] = NULL;
		msStaNum = atoi (szTemp);
		mlStaNum = (long) msStaNum;

		memcpy(szTemp,m_sLANLHeader.FullYear,sizeof(m_sLANLHeader.FullYear));
		szTemp[sizeof(m_sLANLHeader.FullYear)] = NULL;
		miYr4 = atoi (szTemp);
		miYr = miYr4 % 100;

		memcpy(szTemp,m_sLANLHeader.Month,sizeof(m_sLANLHeader.Month));
		szTemp[sizeof(m_sLANLHeader.Month)] = NULL;
		miMon = atoi (szTemp);

		memcpy(szTemp,m_sLANLHeader.Day,sizeof(m_sLANLHeader.Day));
		szTemp[sizeof(m_sLANLHeader.Day)] = NULL;
		miDay = atoi (szTemp);
	}
 
	// The EOSS file is more complicated than others, don't try to set the initial date here
	// from the first read.
	
    if (pstrErrorMsg) pstrErrorMsg->Empty();
	miErrorNum = 0;	// no error
   	return bResult;

}

bool CEOSSDataFile::ReadDataFile(CDbAccess* pDb, const CString &strNameWithPath, CString *pstrMsg)
{
	COleDateTime currentDay, nextDay;
	CString csErr;
	COleDateTime first, last; // First and last date stamps in file
	//Date for last date of the records in the file
	COleDateTime lastRecord;
	//Init the dates
	first.m_dt = 0;
	last.m_dt = 0;
	bool bSuccess = false, bPartialResults = false;
	CString fileNameOnly;
	fileNameOnly = strNameWithPath.Right (strNameWithPath.GetLength () - strNameWithPath.ReverseFind ('\\'));
	fileNameOnly.TrimLeft ("\\");
			
	if (OpenDataFile (strNameWithPath,pstrMsg))
	{

		if (ReadHeader())
		{
			CString strStationName="UNKNOWN_STATION_NAME";

			USES_CONVERSION;

			CString cstrStationName = W2T (m_pIFacMgr->GetStationLongName (pDb->msFacNum, (long) msStaNum, "EOSS"));
			
			if (cstrStationName.IsEmpty ())
			{
				if (pstrMsg) pstrMsg->Format("\nError: Skipping file %s with unknown station %u",m_Dir.StripPathFromFilename(strNameWithPath), msStaNum);
				if (mpFile) fclose(mpFile);
				return(false);
			}

			m_pBInst = new CEOSSData(pDb, msStaNum, m_bQuietMode);							
			bool bSealIDsRead = false;
			//Format stuff for summary
			COleDateTime fileDate (miYr4,miMon,miDay,0,0,0);
			if (pstrMsg)
			{
				CString csTemp;
				csTemp.Format ("\n%s     %u          %s\nSeal IDs in file: ", fileDate.Format (),msStaNum,strNameWithPath);
				*pstrMsg+=csTemp;
			}
            CString csPageErrorNote;
			bool bPageProblem = false;
//#ifdef _ACCUMULATE_THE_IDS
            // append any new ids encountered each time through the loop
            CMap< unsigned int , unsigned int , unsigned int , unsigned int  > eossmap;
//#endif
			while ((!bPageProblem) &&
                            ReadEOSSTable()) //this checks for eof
			{
/*				if (!bSealIDsRead)
					//TODO: This will be correct, as long as a seal is not added during this day, then
					// later EOSS Table readings will be different.  What to do?  Maybe this is good enough. HN
				{
					int i = 0;
					while (m_EOSSTable[i] != 0 && (i < MAXNUMEOSS))
					{
						//Append seal ids to string
						if (*pstrMsg)
						{
							// Append this seal id.
							CString csTemp;
							csTemp.Format ("%u ",m_EOSSTable[i]);
							*pstrMsg+= csTemp;
						}
						i++;
					}
					bSealIDsRead = true;
				}*/
//#ifdef _ACCUMULATE_THE_IDS
                {
                    int ix = 0;
                    unsigned int id;
                    while (m_EOSSTable[ix] != 0 && (ix < MAXNUMEOSS))
					{
                        if (!eossmap.Lookup(m_EOSSTable[ix], id))
                        {
                            eossmap.SetAt(m_EOSSTable[ix], m_EOSSTable[ix]);
                            //TCHAR buff[32];
                            //sprintf(buff,"added %u\n",m_EOSSTable[ix]);
                            //OutputDebugString(buff);
                        }
						ix++;
					}
                }
//#endif
                
				unsigned int uiTableIndex = 0;
				while ((!bPageProblem) &&
                    (m_EOSSTable[uiTableIndex] != 0) && (uiTableIndex < MAXNUMEOSS))
				{
					if (ReadStatusRec())
					{
			            CString csPageErrorTempNote;
						int iCount = 0;
						if (!ReadTimeRec()) continue;
						//Save time of status record for last point in db.
						else
						{
							lastRecord.m_dt = m_TimeRec.dt > lastRecord.m_dt?m_TimeRec.dt:lastRecord.m_dt;
						}
						if (ReadLogInfoRec())
						{
							//read seal log pages
							iCount = m_LogInfoRec.SealEvents;
							int iMaxCount = (m_LogInfoRec.SealLogLastPage - m_LogInfoRec.SealLogFirstPage + 1)*SEALEVENTS_PER_PAGE;
							if (iCount > iMaxCount)
							{
								iCount = iMaxCount - SEALEVENTS_PER_PAGE + iCount % SEALEVENTS_PER_PAGE;
								if (iCount % SEALEVENTS_PER_PAGE == 0)
									iCount += SEALEVENTS_PER_PAGE;
							}
							
							while (iCount > 0)
							{
								if (ReadSealLogPage())
								{
									m_sCurrSealID = m_SealLogPage.Header.Seal;
									m_bSealState = OPENED;
									for (int iRecordIndex = 0; (iRecordIndex < SEALEVENTS_PER_PAGE) && (iCount > 0); iRecordIndex++, iCount--)
									{
										ProcessSealEvent(iRecordIndex,&m_SealLogPage.Entries[iRecordIndex]);
										m_pBInst->AddSealLogPoint (mlStaNum,m_sCurrSealID,m_dtRecordTime,m_bSealState);
										m_bSealState = m_bSealState==OPENED?CLOSED:OPENED;	
										bPartialResults = true;
									}
								}
								else
								{
									csPageErrorTempNote.Format("\nError: Premature end of data seen while processing %d seal record%s", iCount, iCount == 1 ? "." : "s.");
									break; // jfl: added else to stop infinite loop risk in this unreviewed code -- BMEND-124
								}
							}
							if (m_SealLogPage.Header.Page != m_LogInfoRec.SealLogCurrentPage)
							{
								bPageProblem = true;
                                csPageErrorNote.Format("%.256s\nError: Parse did not end on current page while processing seal log records for %u.\n",csPageErrorTempNote.GetBuffer(), m_SealLogPage.Header.Seal);
                                continue;
							}

							//read system log pages (nearly identical to above)
							iCount = m_LogInfoRec.SystemEvents;
							iMaxCount = (m_LogInfoRec.SystemLogLastPage - m_LogInfoRec.SystemLogFirstPage + 1)*SYSTEMEVENTS_PER_PAGE;
							if (iCount > iMaxCount)
							{
								iCount = iMaxCount - SYSTEMEVENTS_PER_PAGE + iCount % SYSTEMEVENTS_PER_PAGE;
								if (iCount % SYSTEMEVENTS_PER_PAGE == 0)
									iCount += SYSTEMEVENTS_PER_PAGE;
							}
							while (iCount > 0)
							{
								if (ReadSystemLogPage())
								{
									for (int iRecordIndex = 0; (iRecordIndex < SYSTEMEVENTS_PER_PAGE) && (iCount > 0); iRecordIndex++, iCount--)
									{
										ProcessSystemEvent(&m_SystemLogPage.Entries[iRecordIndex]);
										bPartialResults = true;
									}
								}
								else
								{
									csPageErrorTempNote.Format("\nError: Premature end of data seen while processing %d system record%s", iCount, iCount == 1 ? "." : "s.");
									break; // jfl: added else to stop infinite loop risk in this unreviewed code -- BMEND-124
								}
							}
							if (m_SystemLogPage.Header.Page != m_LogInfoRec.SystemLogCurrentPage)
							{
								bPageProblem = true;
                                csPageErrorNote.Format("%.256s\nError: Parse did not end on current page while processing system log records for %u.\n",csPageErrorTempNote.GetBuffer(), m_SystemLogPage.Header.Seal);
                                continue;
							}
	
							//read soh log pages (nearly identical to above)
							iCount = m_LogInfoRec.SoHRecords+1;//note the +1 here -- apparent error in firmware?
							iMaxCount = (m_LogInfoRec.SoHLogLastPage - m_LogInfoRec.SoHLogFirstPage + 1)*SOHRECORDS_PER_PAGE;
							if (iCount > iMaxCount)
							{
								iCount = iMaxCount - SOHRECORDS_PER_PAGE + iCount % SOHRECORDS_PER_PAGE;
								if (iCount % SOHRECORDS_PER_PAGE == 0)
									iCount += SOHRECORDS_PER_PAGE;
							}
							while (iCount > 0)
							{
								if (ReadSOHLogPage())
								{
									for (int iRecordIndex = 0; (iRecordIndex < SOHRECORDS_PER_PAGE) && (iCount > 0); iRecordIndex++, iCount--)
									{
										ProcessSOHRecord(&m_SOHLogPage.Entries[iRecordIndex]);
										bPartialResults = true;
									}
								}
								else
								{
									csPageErrorTempNote.Format("\nError: Premature end of data seen while processing %d SOH record%s", iCount, iCount == 1 ? "." : "s.");
									break; // jfl: added else to stop infinite loop risk in this unreviewed code -- BMEND-124
								}
							}
							if (m_SOHLogPage.Header.Page != m_LogInfoRec.SoHLogCurrentPage)
							{
								bPageProblem = true;
								csPageErrorNote.Format("%.256s\nError: Parse did not end on current page while processing SOH records for %u.\n",csPageErrorTempNote.GetBuffer(), m_SOHLogPage.Header.Seal);
                                continue;
							}
						}
					}		
					uiTableIndex++;
				}
				//Now, store the last time read in the file. hn 4-29-08
				CString query = "SELECT Date_Time_LastRecord FROM EOSSDataRecordsReadTable";
				//Should only be one of these.
				unsigned int uiNumDates;
				long lNumDates;
				ADODB::_RecordsetPtr pRecordSet;
				bool bOpen = true;
				bool bRan = pDb->CreateRecordSet (query,(ADODB::_RecordsetPtr *)&pRecordSet,&bOpen,&uiNumDates);

				bool bExists = uiNumDates>0;
				bool bAdded = false;

				if (!bExists)
				{
					// This was not in the DB, add it.
					CString addStatement;
					addStatement.Format ("INSERT INTO EOSSDataRecordsReadTable (Date_Time_LastRecord,Date_Timestamp_LastRecord) VALUES (%f,%16.15f);",lastRecord.m_dt,lastRecord.m_dt);
					long lNum =0;
					bAdded = pDb->ExecuteSqlCommand (addStatement,lNumDates);
				}
				else
				{
					COleDateTime currentTime;
					bAdded = pDb->GetRecordFieldDateTime (pRecordSet,"Date_Time_LastRecord",&currentTime);
					//Update it if it is newer
					if (currentTime.m_dt < lastRecord.m_dt)
					{
						CString updateStatement;
						updateStatement.Format ("UPDATE EOSSDataRecordsReadTable SET Date_Time_LastRecord = %f,Date_Timestamp_LastRecord = %16.15f WHERE Date_Time_LastRecord = %f;",lastRecord.m_dt,lastRecord.m_dt,currentTime.m_dt);
						bAdded = pDb->ExecuteSqlCommand (updateStatement,lNumDates);
					}
				}
				if (pRecordSet)
					pDb->DestroyRecordSet ((ADODB::_RecordsetPtr )pRecordSet,true);
			}
//#ifdef _ACCUMULATE_THE_IDS
			if (pstrMsg)
			{
			   CString csTemp;
			   *pstrMsg+= "\nCumulative seals found: ";
				POSITION pos = eossmap.GetStartPosition();
				while( pos != NULL )
				{
					unsigned int id, ix;
					eossmap.GetNextAssoc( pos, ix, id);
					// Append this seal id.
					csTemp.Format ("%u ",id);
					*pstrMsg+= csTemp;
				}
			}
//#endif
			if (bPageProblem)
			{
				if (pstrMsg)
				{
					CString csTemp;
                    if (bPartialResults) 
					{
						csTemp.Format("\nWarning: Partial results were retrieved, but a problem was encountered while processing EOSS pages. File may be corrupt.%s",csPageErrorNote);
					}
					else
					{
						csTemp.Format("\nError: No results retrieved, problem encountered while processing EOSS pages. File may be corrupt.%s",csPageErrorNote);
					}
					*pstrMsg+= csTemp;
				}
			}

			//Add the data to the DB
			bSuccess = m_pBInst->AddEOSSData ();
		}
		else
		{
			if (pstrMsg) pstrMsg->Format("\nError: Could not read file header for file %s\n",fileNameOnly);
			bSuccess = false;
		}
		fclose (mpFile);
		delete m_pBInst;
		m_pBInst = NULL;
	}
	else
	{
		if (pstrMsg) pstrMsg->Format("\nError: File %s could not be opened.", fileNameOnly);
		bSuccess = false;
	}
    return(bSuccess && bPartialResults);
}



//This function was added to CGrandDataFile to recursively retrieve a list of *.BID files in
//a directory.  It reads the file header to populate the CList of tFileRecords that is converted to
//a safe array used by ImportManager.  This was flagrantly stolen from Kelly Michel and modified to
//search for and read in *.BID files. hn 6/8/2005
// 5-Jul-2005 SFK Removed dead code for readability
// 11-Jul-2005 SFK	Copied from GRAND Import and modified for binary
void CEOSSDataFile::GetCompleteFileList(short FacilityID, CList<tFileRecord, tFileRecord> *pFileList, 
									   const CString& Directory,  bool IncludeSubdirs)
{
	USES_CONVERSION;

	CFileFind Finder;
	BOOL bWorking;
	CString  FileName;
	tFileRecord FileRecord;

	CString DirWithFileMask;
	DirWithFileMask.Format("%s\\*.*", Directory);
	bWorking = Finder.FindFile(DirWithFileMask);

	
	//Don't do any of this if there are no files in the dir or if the dir can't be found.
	if (bWorking)
	{
		//If this is an "archive" directory, then skip it completely, and everything that may be underneath it.
		int StartSubDirName = Directory.ReverseFind('\\');
		if(StartSubDirName != -1)
		{
			CString SubDirName = Directory.Mid(StartSubDirName + 1);
			if(SubDirName.CompareNoCase("Archive")) //If SubDirName is not Archive...
			{
				do
				{
					bWorking = Finder.FindNextFile();
					
					if(!Finder.IsDots())
					{
						if(Finder.IsDirectory() && IncludeSubdirs)
						{
							//Recurse.
							GetCompleteFileList(FacilityID, pFileList, Finder.GetFilePath(), IncludeSubdirs);
						}
						else //if(Finder.IsNormal())
						{
 							FileName = Finder.GetFileName(); 
							CString Ext = FileName.Mid(FileName.GetLength() - 3, 3);
							if(!Ext.CompareNoCase("ESS"))
							{
								FileRecord.File.vt = VT_BSTR;
								FileRecord.File.bstrVal = (Finder.GetFilePath()).AllocSysString();
								CString err;

								//****************************************************************
								//Open the file and get info on the data in the file.  Load that
								//file data into the FileRecord structure.
								//****************************************************************
								if (OpenDataFile (W2T(FileRecord.File.bstrVal),&err))
								{

									if (!ReadHeader())
									{
										//fclose (mpFile);
									}
								//If we are not able to read the *.BID header, we fail
								//if (!ReadHeader (W2T(FileRecord.File.bstrVal),&err))
								//{
								//}
									else
									//Otherwise, save the file date and station ID read.
									{
										SYSTEMTIME sysTime;
										COleDateTime fileDate = GetFileDate ();
										FileRecord.Date.vt = VT_DATE;
										fileDate.GetAsSystemTime (sysTime);
										SystemTimeToVariantTime (&sysTime,&FileRecord.Date.date);
										FileRecord.StationID.vt = VT_I4;
										FileRecord.StationID.lVal = (long) GetStationID ();
										pFileList->AddTail (FileRecord);
										//CloseDataFile ();
										//fclose (mpFile);
									}
									if (mpFile) CloseDataFile();
								}
							}
						}
					}		
				}
				while(bWorking != 0);
			}
		}
	}

}

long CEOSSDataFile::GetStationID()
{
	return mlStaNum;
}

bool CEOSSDataFile::StatRecOK(const STATUS& status)
{
	bool bReturn = false;

	bReturn = (memcmp(&status,"FAILED READ",11)!=0);
	bReturn = (memcmp(&status,"FAILED LOG ON",14)!=0);
	bReturn = (memcmp(&status,"FAILED CHALLENGE",17)!=0);

	return bReturn;
}

/*bool CEOSSDataFile::ReadEOSSTable ()
{
	
	bool bResult = false;
	if (fread(m_EOSSTable,sizeof(m_EOSSTable),1,mpFile) == 1)
	{
		//perform any further EOSS table analysis here
		bResult = true;
		//int iIndex = 0;
		//while ((SealArray[iIndex] != 0) && (iIndex < MAX_SEALS))
		//{
		//	iIndex++;
		//}
	}

	return bResult;
}

bool CEOSSDataFile::ReadStatusRecord ()
{
	bool bResult = false;
	
	//STATUS status;
	if (fread(&m_StatusRec,sizeof(m_StatusRec),1,mpFile) == 1)
		bResult = !(memcmp(&m_StatusRec,"FAIL",4) == 0);
	
	//perform any further status rec analysis here
	if (bResult)
	{
		//There is really nothing in the status record of interest at this time.
	}
	
	return bResult;
}

bool CEOSSDataFile::ReadTimeRecord()
{
	bool bResult = false;
	
	m_sOffset = 0;
	m_dtRecordTime.m_dt = 0;
	m_dtRecordTime.m_status = COleDateTime::null;

	//TIMEREC time;
	bResult = (fread(&m_TimeRec,sizeof(m_TimeRec),1,mpFile) == 1)?true:false;
	//perform any further time rec analysis here
	if (bResult)
	{
		COleDateTime cTime;
		cTime.m_dt = m_TimeRec.dt;
		cTime.m_status = m_TimeRec.status;
		m_dtRecordTime.SetDateTime (cTime.GetYear(), cTime.GetMonth(),cTime.GetDay(),cTime.GetHour(),cTime.GetMinute(),cTime.GetSecond());
		
		// Now adjust
		m_sOffset = m_TimeRec.sOffset;
		COleDateTimeSpan span;
		span.SetDateTimeSpan (0,abs(m_sOffset),0,0);
		m_dtRecordTime=m_sOffset>0?m_dtRecordTime+span:m_dtRecordTime-span;
	}
	return bResult;

}

bool CEOSSDataFile::ReadLogInfoRecord()
{
	bool bResult = false;
	//LOGINFO loginfo;
	if (fread(&m_LogInfoRec,sizeof(m_LogInfoRec),1,mpFile) == 1)
		bResult = !(memcmp(&m_LogInfoRec,"FAIL",4) == 0);

//	if (fread(&loginfo,sizeof(loginfo),1,mpFile) == 1)
//		bResult = !(memcmp(&loginfo,"FAIL",4) == 0);
	//perform any further LOGINFO page analysis here
	if (bResult)
	{
		m_nSealEvents = m_LogInfoRec.SealEvents;
		m_nMaxSealEvents = (m_LogInfoRec.SealLogLastPage - m_LogInfoRec.SealLogFirstPage + 1)*SEALEVENTS_PER_PAGE;
		m_nSystemEvents = m_LogInfoRec.SystemEvents;
		m_nMaxSystemEvents = (m_LogInfoRec.SystemLogLastPage - m_LogInfoRec.SystemLogFirstPage + 1)*SYSTEMEVENTS_PER_PAGE;
		m_nSOHEvents = m_LogInfoRec.SoHRecords +1;
		m_nMaxSOHEvents = (m_LogInfoRec.SystemLogLastPage - m_LogInfoRec.SystemLogFirstPage + 1)*SYSTEMEVENTS_PER_PAGE;

	}
	return bResult;

}

bool CEOSSDataFile::ReadSealLogPage()
{
	bool bResult = false;
	if (fread(&m_SealLogPage,sizeof(m_SealLogPage),1,mpFile) == 1)
		bResult = !(memcmp(&m_SealLogPage,"FAIL",4) == 0);

	//perform any further SEAL log page analysis here
	if (m_SealLogPage.Header.Log != 70)
		bResult = false;

	if (bResult)
	{
		// Not sure why we care about the header
		//SYSTEMTIME sTime;
		//EossToSystemTime(&sTime, &m_logpage.Header.Time);
		m_sCurrSealID = m_SealLogPage.Header.Seal;
	}
	
	return bResult;
}

void CEOSSDataFile::EossToSystemTime(SYSTEMTIME *pTime, EOSSTIME *eossTime)
{
	SYSTEMTIME SystemTime;
	
	SystemTime.wYear = 2000;
	SystemTime.wMonth = 1;
	SystemTime.wDay = 1;
	SystemTime.wHour = 0;
	SystemTime.wMinute = 0;
	SystemTime.wSecond = 0;
	SystemTime.wMilliseconds = 0;

	ULONGLONG FileTime;
	
	SystemTimeToFileTime(&SystemTime, (LPFILETIME)&FileTime);

	ULONGLONG EossTime = eossTime->HalfDays * 43200 + (43200 - eossTime->Seconds);

	FileTime += (EossTime * 10000000);
	
	FileTimeToSystemTime((LPFILETIME)&FileTime, pTime);
}

bool CEOSSDataFile::ReadSOHLogPage()
{
	bool bResult = false;
	//SOHLOGPAGE soh;
	if (fread(&m_SOHLogPage,sizeof(m_SOHLogPage),1,mpFile) == 1)
		bResult = !(memcmp(&m_SOHLogPage,"FAIL",4) == 0);

	//perform any further SOH log page analysis here
	if (m_SOHLogPage.Header.Log != 72) 
		bResult = false;


	if (bResult)
	{
	}

	return bResult;
}

bool CEOSSDataFile::ReadSystemLogPage()
{
	bool bResult = false;
	//SYSTEMLOGPAGE system;
	if (fread(&m_SystemLogPage,sizeof(m_SystemLogPage),1,mpFile) == 1)
		bResult = !(memcmp(&m_SystemLogPage,"FAIL",4) == 0);

	//perform any further SOH log page analysis here
	if (m_SystemLogPage.Header.Log != 72) 
		bResult = false;


	if (bResult)
	{
	}

	return bResult;
}

void CEOSSDataFile::ProcessSealEvent(int iEventNumber, SEALEVENT* pSealEvent)
{
//	fprintf(m_outfile,"\t\tPROCESS SEAL EVENT ");

//	SYSTEMTIME sTime;
//	EossToSystemTime(&sTime, pSealEvent);
//	fprintf(m_outfile,"\t - %s\t%04u\\%02u\\%02u %02u:%02u:%02u\n",(iEventNumber&1)?"Closed":"Opened",
//		sTime.wYear,sTime.wMonth,sTime.wDay,
//		sTime.wHour,sTime.wMinute,sTime.wSecond);
}

void CEOSSDataFile::ProcessSystemEvent(SYSTEMEVENT* pSystemEvent)
{
//	fprintf(m_outfile,"\t\tPROCESS SYSTEM EVENT\t");

//	SYSTEMTIME sTime;
//	EossToSystemTime(&sTime, &pSystemEvent->Time);
//	fprintf(m_outfile," -  %04u\\%02u\\%02u %02u:%02u:%02u  %s\n",
//		sTime.wYear,sTime.wMonth,sTime.wDay,
//		sTime.wHour,sTime.wMinute,sTime.wSecond,
//		GetEossMessage(pSystemEvent->Code));

}

void CEOSSDataFile::ProcessSOHRecord(SOHRECORD* pSOHRecord)
{
//	fprintf(m_outfile,"\t\tPROCESS SOH RECORD\t");

//	SYSTEMTIME sTime;
//	EOSSTIME eTime;
//	eTime.HalfDays = pSOHRecord->Date;
//	eTime.Seconds = 0;
//	EossToSystemTime(&sTime, &eTime);
//	fprintf(m_outfile," - %04u\\%02u\\%02u   %3d°C %3d°C   %3.1fv %3.1fv   %4u min. %3u MVEvents\n",
//		sTime.wYear,sTime.wMonth,sTime.wDay,
//		pSOHRecord->MaxTemperature,
//		pSOHRecord->MinTemperature,
//		(float)pSOHRecord->EmergencyBattery/10.0,
//		(float)pSOHRecord->MainBattery/10.0,
//		pSOHRecord->OnBatteries*12,
//		pSOHRecord->MVEvents);
}*/
bool CEOSSDataFile::ReadHeader()
{
	bool bResult = false;
	
	if (fread(&m_sLANLHeader,sizeof(m_sLANLHeader),1,mpFile) != 1)
	{
		bResult = false;
	}
	else
		bResult = true;


	//do any header validation you need to do here
	if (bResult)
	{
		char szTemp[64];
		memcpy(szTemp,m_sLANLHeader.SizeOfHeader,sizeof(m_sLANLHeader.SizeOfHeader));
		szTemp[sizeof(m_sLANLHeader.SizeOfHeader)] = NULL;

		memcpy(szTemp,m_sLANLHeader.Version,sizeof(m_sLANLHeader.Version));
		szTemp[sizeof(m_sLANLHeader.Version)] = NULL;
		mdVersionNumber = atof (szTemp);

		memcpy(szTemp,m_sLANLHeader.StationId,sizeof(m_sLANLHeader.StationId));
		szTemp[sizeof(m_sLANLHeader.StationId)] = NULL;
		msStaNum = atoi (szTemp);
		mlStaNum = (long) msStaNum;

		bResult = (mlStaNum >0 && mlStaNum < 32768);

		memcpy(szTemp,m_sLANLHeader.FullYear,sizeof(m_sLANLHeader.FullYear));
		szTemp[sizeof(m_sLANLHeader.FullYear)] = NULL;
		miYr4 = atoi (szTemp);
		miYr = miYr4 % 100;

		bResult = bResult && miYr4 > 1954 && miYr4 < 3000;

		memcpy(szTemp,m_sLANLHeader.Month,sizeof(m_sLANLHeader.Month));
		szTemp[sizeof(m_sLANLHeader.Month)] = NULL;
		miMon = atoi (szTemp);

		bResult = bResult && miMon > 0 && miMon < 13;

		memcpy(szTemp,m_sLANLHeader.Day,sizeof(m_sLANLHeader.Day));
		szTemp[sizeof(m_sLANLHeader.Day)] = NULL;
		miDay = atoi (szTemp);

		bResult = bResult && miDay > 0 && miDay < 32; // Don't need to do anything terribly fancy here
	}
 
	// The EOSS file is more complicated than others, don't try to set the initial date here
	// from the first read.
	
//    if (pstrErrorMsg) pstrErrorMsg->Empty();
	miErrorNum = 0;	// no error
   	return bResult;
}

bool CEOSSDataFile::ReadEOSSTable()
{
//	fprintf(m_outfile,"READ EOSS TABLE\n");
	bool bResult = false;
	if (fread(m_EOSSTable,sizeof(m_EOSSTable),1,mpFile) == 1)
	{
		//perform any further EOSS table analysis here
		bResult = true;
		int iIndex = 0;
//		fprintf(m_outfile," - ");
		while ((m_EOSSTable[iIndex] != 0) && (iIndex < 32)) // jfl can examine for value range of acceptable id here
		{
//			fprintf(m_outfile,"%d ",m_EOSSTable[iIndex]);
			iIndex++;
		}
//		fprintf(m_outfile,"\n");
	}
	else
//		fprintf(m_outfile,"%s\n",(feof(mpFile))?"END OF FILE":"READ ERROR");
		bResult = false;
	return bResult;
}

bool CEOSSDataFile::ReadStatusRec()
{
//	fprintf(m_outfile,"READ STATUS RECORD\n");
	bool bResult = false;
	if (fread(&m_StatusRec,sizeof(m_StatusRec),1,mpFile) == 1)
		bResult = !(memcmp(&m_StatusRec,"FAIL",4) == 0);
	//perform any further status rec analysis here
	if (bResult)
	{
//		fprintf(m_outfile," - Seal line was %s during latest read\n",(m_StatusRec.Flags & FLAG_FOOPEN)?"OPEN":"CLOSED");
//		fprintf(m_outfile," - Fiber optic %s\n",(m_StatusRec.Flags & FLAG_FOFAIL)?"FAILURE":"OK");
//		fprintf(m_outfile," - Drill detection %s\n",(m_StatusRec.Flags & FLAG_HOPEN)?"OPEN":"NOT OPEN");
//		fprintf(m_outfile," - Drill detection %s\n",(m_StatusRec.Flags & FLAG_SHORT)?"SHORTED":"NOT SHORTED");
//		fprintf(m_outfile," - Normal battery consumed(uAH) %d\n",m_StatusRec.BatteryConsumed);
//		fprintf(m_outfile," - Emergency battery consumed(uAH) %d\n",m_StatusRec.EBatteryConsumed);
//		fprintf(m_outfile," - Flash memory warnings %d\n",m_StatusRec.FlashWarnings);
//		fprintf(m_outfile," - Flash memory errors %d\n",m_StatusRec.FlashErrors);
//		fprintf(m_outfile," - Majority vote memory warnings %d\n",m_StatusRec.MajoWarnings);
//		fprintf(m_outfile," - Majority vote memory errors %d\n",m_StatusRec.MajoErrors);
	}
	else
//		fprintf(m_outfile," - FAILED\n");
		bResult = false;

	return bResult;
}

bool CEOSSDataFile::ReadTimeRec()
{
//	fprintf(m_outfile,"READ TIME RECORD\n");
	bool bResult = false;
	bResult = (fread(&m_TimeRec,sizeof(m_TimeRec),1,mpFile) == 1)?true:false;
	//perform any further time rec analysis here
	COleDateTime cTime;
	cTime.m_dt = m_TimeRec.dt;
	cTime.m_status = m_TimeRec.status;
//	fprintf(m_outfile," - %s %d\n",cTime.Format("%Y.%m.%d %H:%M:%S"),m_TimeRec.sOffset);
//	fprintf(m_outfile," - Version: %d.%d\n",m_TimeRec.cVersion[0],m_TimeRec.cVersion[1]);
	return bResult;
}

bool CEOSSDataFile::ReadLogInfoRec()
{
//	fprintf(m_outfile,"READ LOGINFO RECORD\n");
	bool bResult = false;
	if (fread(&m_LogInfoRec,sizeof(m_LogInfoRec),1,mpFile) == 1)
		bResult = !(memcmp(&m_LogInfoRec,"FAIL",4) == 0);
	//perform any further LOGINFO page analysis here
	if (bResult)
	{
//		fprintf(m_outfile," - Seal: Events:%d First:%d Last:%d Start:%d Curr:%d\n",
//			m_LogInfoRec.SealEvents,m_LogInfoRec.SealLogFirstPage,
//			m_LogInfoRec.SealLogLastPage,m_LogInfoRec.SealLogStartPage,
//			m_LogInfoRec.SealLogCurrentPage);

//		fprintf(m_outfile," - System: Events:%d First:%d Last:%d Start:%d Curr:%d\n",
//			m_LogInfoRec.SystemEvents,m_LogInfoRec.SystemLogFirstPage,
//			m_LogInfoRec.SystemLogLastPage,m_LogInfoRec.SystemLogStartPage,
//			m_LogInfoRec.SystemLogCurrentPage);

//		fprintf(m_outfile," - SoH: Records:%d First:%d Last:%d Start:%d Curr:%d\n",
//			m_LogInfoRec.SoHRecords,m_LogInfoRec.SoHLogFirstPage,
//			m_LogInfoRec.SoHLogLastPage,m_LogInfoRec.SoHLogStartPage,
//			m_LogInfoRec.SoHLogCurrentPage);
	}
	else
//		fprintf(m_outfile,"FAILED\n");
		bResult = false;
	return bResult;
}

bool CEOSSDataFile::ReadSealLogPage()
{
//	fprintf(m_outfile,"\tREAD SEAL LOG PAGE\n");
	bool bResult = false;
	if (fread(&m_SealLogPage,sizeof(m_SealLogPage),1,mpFile) == 1)
		bResult = !(memcmp(&m_SealLogPage,"FAIL",4) == 0);

	//perform any further SEAL log page analysis here
	if (m_SealLogPage.Header.Log != 70)
		bResult = false;

	if (bResult)
	{
//		fprintf(m_outfile,"\t - Seal\t\t%u\n",m_SealLogPage.Header.Seal);
		
//		fprintf(m_outfile,"\t - Log\t\t%u\n",m_SealLogPage.Header.Log);
		
//		fprintf(m_outfile,"\t - Page\t\t%u\n",m_SealLogPage.Header.Page);
		
		SYSTEMTIME sTime;
		EossToSystemTime(&sTime, &m_SealLogPage.Header.Time);
//		fprintf(m_outfile,"\t - Time\t\t%u\\%02u\\%02u %02u:%02u:%02u\n",
//			sTime.wYear,sTime.wMonth,sTime.wDay,sTime.wHour,sTime.wMinute,sTime.wSecond);

//		fprintf(m_outfile,"\t - Authenticator\t0x%02X%02X%02X%02X%02X%02X%02X%02X\n\n",
//			m_SealLogPage.Header.Authenticator[0],
//			m_SealLogPage.Header.Authenticator[1],
//			m_SealLogPage.Header.Authenticator[2],
//			m_SealLogPage.Header.Authenticator[3],
//			m_SealLogPage.Header.Authenticator[4],
//			m_SealLogPage.Header.Authenticator[5],
//			m_SealLogPage.Header.Authenticator[6],
//			m_SealLogPage.Header.Authenticator[7]);
	}
	else
//		fprintf(m_outfile,"\t - FAILED\n\n");
		bResult = false;
	return bResult;
}
bool CEOSSDataFile::ReadSystemLogPage()
{
//	fprintf(m_outfile,"\tREAD SYSTEM LOG PAGE\n");
	bool bResult = false;
	if (fread(&m_SystemLogPage,sizeof(m_SystemLogPage),1,mpFile) == 1)
		bResult = !(memcmp(&m_SystemLogPage,"FAIL",4) == 0);

	//perform any further SYSTEM log page analysis here
	if (m_SystemLogPage.Header.Log != 83) 
		bResult = false;

	if (bResult)
	{
//		fprintf(m_outfile,"\t - Seal\t\t%u\n",m_SystemLogPage.Header.Seal);
			
//		fprintf(m_outfile,"\t - Log\t\t%u\n",m_SystemLogPage.Header.Log);
			
//		fprintf(m_outfile,"\t - Page\t\t%u\n",m_SystemLogPage.Header.Page);
			
		SYSTEMTIME sTime;
		EossToSystemTime(&sTime, &m_SystemLogPage.Header.Time);
//		fprintf(m_outfile,"\t - Time\t\t%u\\%02u\\%02u %02u:%02u:%02u\n",
//			sTime.wYear,sTime.wMonth,sTime.wDay,sTime.wHour,sTime.wMinute,sTime.wSecond);

//		fprintf(m_outfile,"\t - Authenticator\t0x%02X%02X%02X%02X%02X%02X%02X%02X\n\n",
//			m_SystemLogPage.Header.Authenticator[0],
//			m_SystemLogPage.Header.Authenticator[1],
//			m_SystemLogPage.Header.Authenticator[2],
//			m_SystemLogPage.Header.Authenticator[3],
//			m_SystemLogPage.Header.Authenticator[4],
//			m_SystemLogPage.Header.Authenticator[5],
//			m_SystemLogPage.Header.Authenticator[6],
//			m_SystemLogPage.Header.Authenticator[7]);
	}
	else
//		fprintf(m_outfile,"\t - FAILED\n\n");
		bResult = false;
	return bResult;
}
bool CEOSSDataFile::ReadSOHLogPage()
{
//	fprintf(m_outfile,"\tREAD SOH LOG PAGE\n");
	bool bResult = false;
	size_t rdx = fread(&m_SOHLogPage,sizeof(m_SOHLogPage),1,mpFile);
	if (rdx == 1)
		bResult = !(memcmp(&m_SOHLogPage,"FAIL",4) == 0);
	//else if (rdx == 0)
	//{
	//	int one, two;
	//	one = feof(mpFile);
	//	two = ferror(mpFile);
	//}

	//perform any further SOH log page analysis here
	if (m_SOHLogPage.Header.Log != 72) 
		bResult = false;

	if (bResult)
	{
		SYSTEMTIME sTime;
		EossToSystemTime(&sTime, &m_SOHLogPage.Header.Time);
	}
	return bResult;
}

void CEOSSDataFile::ProcessSealEvent(int iEventNumber, SEALEVENT* pSealEvent)
{
//	fprintf(m_outfile,"\t\tPROCESS SEAL EVENT ");

	SYSTEMTIME sTime;
	EossToSystemTime(&sTime, pSealEvent);
	COleDateTime temp (sTime);
	m_dtRecordTime = temp;
//	fprintf(m_outfile,"\t - %s\t%04u\\%02u\\%02u %02u:%02u:%02u\n",(iEventNumber&1)?"Closed":"Opened",
//		sTime.wYear,sTime.wMonth,sTime.wDay,
//		sTime.wHour,sTime.wMinute,sTime.wSecond);
}

void CEOSSDataFile::ProcessSystemEvent(SYSTEMEVENT* pSystemEvent)
{
//	fprintf(m_outfile,"\t\tPROCESS SYSTEM EVENT\t");

	SYSTEMTIME sTime;
	EossToSystemTime(&sTime, &pSystemEvent->Time);
//	fprintf(m_outfile," -  %04u\\%02u\\%02u %02u:%02u:%02u  %s\n",
//		sTime.wYear,sTime.wMonth,sTime.wDay,
//		sTime.wHour,sTime.wMinute,sTime.wSecond,
//		GetEossMessage(pSystemEvent->Code));

}

void CEOSSDataFile::ProcessSOHRecord(SOHRECORD* pSOHRecord)
{
//	fprintf(m_outfile,"\t\tPROCESS SOH RECORD\t");

	SYSTEMTIME sTime;
	EOSSTIME eTime;
	eTime.HalfDays = pSOHRecord->Date;
	eTime.Seconds = 0;
	EossToSystemTime(&sTime, &eTime);
//	fprintf(m_outfile," - %04u\\%02u\\%02u   %3d°C %3d°C   %3.1fv %3.1fv   %4u min. %3u MVEvents\n",
//		sTime.wYear,sTime.wMonth,sTime.wDay,
//		pSOHRecord->MaxTemperature,
//		pSOHRecord->MinTemperature,
//		(float)pSOHRecord->EmergencyBattery/10.0,
//		(float)pSOHRecord->MainBattery/10.0,
//		pSOHRecord->OnBatteries*12,
//		pSOHRecord->MVEvents);
}

void CEOSSDataFile::EossToSystemTime(SYSTEMTIME * pSystemTime, EOSSTIME * pEossTime)
{
	SYSTEMTIME SystemTime;
	
	SystemTime.wYear = 2000;
	SystemTime.wMonth = 1;
	SystemTime.wDay = 1;
	SystemTime.wHour = 0;
	SystemTime.wMinute = 0;
	SystemTime.wSecond = 0;
	SystemTime.wMilliseconds = 0;

	ULONGLONG FileTime;
	
	SystemTimeToFileTime(&SystemTime, (LPFILETIME)&FileTime);

	ULONGLONG EossTime = pEossTime->HalfDays * 43200 + (43200 - pEossTime->Seconds);

	FileTime += (EossTime * 10000000);
	
	FileTimeToSystemTime((LPFILETIME)&FileTime, pSystemTime);
}

CString CEOSSDataFile::GetEossMessage(unsigned short usCode)
{
	CString cString;
	char szTemp[64];
	if (usCode > 0 && usCode < 19683)
	{
//		char szID[4];
//		UnMakeID(usCode,szTemp);
		cString = szTemp;
		cString += " logged on as operator 1";
	}
	else if (usCode > 20480 && usCode < 40163)
	{
//		char szID[4];
//		UnMakeID(usCode - 20480,szTemp);
		cString = szTemp;
		cString += " logged on as operator 2";
	}
	else if (usCode == 40960)
		cString = "Operator(s) logged off.";
	else if (usCode == 40976) cString = "Date and time set.";
	else if (usCode == 40977) cString = "Date and time set.";
	else if (usCode == 40992) cString = "Battery compartment opened";
	else if (usCode == 40993) cString = "Battery compartment closed";
	else if (usCode == 41008) cString = "Seal tampered with. Key-set erased";
	else if (usCode == 41024) cString = "Access denied. Authorization failed";
	else if (usCode == 41040) cString = "New key-set generated";
	else if (usCode == 41056) cString = "Reset";
	else if (usCode == 41072) cString = "Key-set erased on operator request";
	else if (usCode == 41073) cString = "Temperator out of range. Key-set erased";
	else if (usCode == 41088) cString = "Entering deep sleep mode";
//	if (Unattended(usCode)) cString += " WHILE NOT LOGGED IN";
	return cString;
}



