///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : Plt_Db.cpp     
//Primary Author: Tom Marks

///////////////////////////////////////////////////////////////////////////
// $History: Plt_db.cpp $
///////////////////////////////////////////////////////////////////////////
//19 Apr 2006 - pjm: Incorporated SK's DR00254 fix
///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include <gui_lib.h>
#include <malloc.h>
#include <gen.h>    
#include "plt_dlg.h"
#include "plt_db.h"
#include "RAD.H"
#include "DbEvent.h"
#include "DbGeneralDbSupport.h"
#include "DbInterval.h"
#include "DbLimit.h"
#include "DbMark.h"
#include "DbReadArray.h"
#include "EventTypeLists.h"
#include "ProgressBars.h" 
#include "RadError.h"	//SFK    
#include "Utilities.h"
#include "FacilityConfig.h" //KM
#include "TimeAlign.h"
#include "MyDateTime.h"

extern short glsFacNum;
extern CMyDateTime glMyDateTime;

const COLORREF GUI_ORANGE = RGB(255, 155, 50); // SCR00218 

#define CHANNEL_EVENT_COLOR			GUI_MAGENTA
#define MEASUREMENT_EVENT_COLOR		GUI_ORANGE		// SCR00218 21-Dec-2005 SFK Changed measurement color to orange, not dark magenta
#define RECONCILED_EVENT_COLOR		GUI_GREEN
#define NOT_RECONCILED_EVENT_COLOR	GUI_RED
#define BACKGROUND_EVENT_COLOR		GUI_GRAY_LIGHT
#define LOW_BKG_MEAS_COLOR			GUI_RED
#define DEFAULT_EVENT_COLOR			GUI_YELLOW

// Note: The time part of the data retrieved from the data base is converted to
// float. To avoid (minimize) roundoff errors all times have the data base start
// time subtracted before being passed onto the caller outside the CPlotDB class.
// Likewise all dates passed into this class from outside have "mulDB_StartDate"
// added before being handed to Shirley's DB routines.


CPlotDB::CPlotDB(BOOL *pbValidDB)
{
	OutputDebugString("CPlotDB::CTOR\n");

	mbValidDB = FALSE;
	miFacilityNum = (int)glsFacNum;
	maDetNames = NULL;
	maDetShortNames = NULL;
	muiNumDetectors = 0;
	mdDB_StartDate;	// Earliest date in DB in julian seconds.
	mdDB_EndDate; 	// Latest date in DB in julian seconds.

	if (GetDB_Parameters())  // find beginning and end of database
	{
		if (GetChannelNames()) // create list of channel long and short names
			mbValidDB = TRUE;
	}
	
	*pbValidDB = mbValidDB;
}


CPlotDB::~CPlotDB()
{
	delete [] maDetNames;
	delete [] maDetShortNames;
}

//////////////////////////////////////////////////////////////////////////////////////
//	Initialize mulDB_StartDate, mulDB_End Date, maDetNames and maDetShortNames


// Find the beginning and ending times in the database
// and put them in mdDB_StartDate and mdDB_EndDate
BOOL CPlotDB::GetDB_Parameters()
{
	const UINT uiCHAN_INDEX = 0;
	mbValidDB = FALSE;
	struct IDStruct StaID;
	StaID.sChanID = -1;
	StaID.sStaID = -1;
	StaID.sStaType = -1;

	// Find the first and last days in the database for the station
	mbValidDB = GetDbTimeRange(StaID, &mdDB_StartDate, &mdDB_EndDate);
	if (mdDB_EndDate <= 0.0)
	{
		RadReviewMsg(uiDB_EMPTY_STATUS);
		mbValidDB = FALSE;;
	}
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (mbValidDB) 
	{
		if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) 
			mbValidDB = FALSE;
	}

	return(mbValidDB);
}

// Create array lists of the channel long and short names and
// put them in maDetNames and maDetShortNames
BOOL CPlotDB::GetChannelNames()
{
	char **napszChanList;
	BOOL bSuccess = FALSE;
	int iIncludeNoDet = FALSE;
	short sNumChans;
	
	// Get the picklist of channel choices and transfer to CGStr type in maDetNames
	int iDum;
	bSuccess = pglFacCfg->CreateChannelPicklist(miFacilityNum, true, &napszChanList, &iDum);
	sNumChans = pglFacCfg->GetNumChannelsForFacility(glsFacNum);
	if (bSuccess) {
		muiNumDetectors = sNumChans;
		maDetNames = new CGStr [muiNumDetectors];
		if (maDetNames == NULL) {
			muiNumDetectors = 0;
			bSuccess = FALSE;
		}
		else { // fill the CGStr stuff
			for (UINT ui = 0; ui < muiNumDetectors; ui++) {
				maDetNames[ui] = napszChanList[ui];
			}
			bSuccess = TRUE;
		}
	}

	// release the picklist stuff	
	pglFacCfg->DestroyPicklist(napszChanList);
	napszChanList = NULL;

	// Repeat the above steps to get the short names list.
	bSuccess = pglFacCfg->CreateChannelPicklist(miFacilityNum, FALSE, &napszChanList, &iDum);
//KM - Why not replace previous line with following line and delete the CreateChanShortNamesPicklist() function????
//KM	bSuccess = CreateChanNamesPicklist(miFacilityNum, &napszChanList, iIncludeNoDet, &sNumChans, iSHORT_NAME, &sDumMaxLen);
	if (bSuccess) {
		muiNumDetectors = sNumChans;
		maDetShortNames = new CGStr [muiNumDetectors];
		if (maDetShortNames == NULL) {
			muiNumDetectors = 0;
			bSuccess = FALSE;
		}
		else {
			for (UINT ui = 0; ui < muiNumDetectors; ui++) {
				maDetShortNames[ui] = napszChanList[ui];
			}
			bSuccess = TRUE;
		}
	}
		
	pglFacCfg->DestroyPicklist(napszChanList);
	napszChanList = NULL;

	return(bSuccess);
}


//////////////////////////////////////////////////////////////////////////////////////
//	Get detector name strings

// Given the detector index, return the CGStr for the detector long name
BOOL CPlotDB::GetDetName(UINT uiDetectorIndex, CGStr *pDetName)
{
	BOOL bSuccess = FALSE;

	*pDetName = "";	
	if (uiDetectorIndex < muiNumDetectors) {
		if (maDetNames != NULL) {
			bSuccess = TRUE;
			*pDetName = maDetNames[uiDetectorIndex];
		}
	}
	return(bSuccess);
}


// Given the detector index, return the CGStr for the detector short name
BOOL CPlotDB::GetDetShortName(UINT uiDetectorIndex, CGStr *pDetName)
{
	BOOL bSuccess = FALSE;

	*pDetName = "";	
	if (uiDetectorIndex < muiNumDetectors) {
		if (maDetShortNames != NULL) {
			bSuccess = TRUE;
			*pDetName = maDetShortNames[uiDetectorIndex];
		}
	}
	return(bSuccess);
}


//////////////////////////////////////////////////////////////////////////////////////
//	Database time/date stuff

// Convert a julian time to a formatted date time string
// mod with DATE
const char *CPlotDB::AbsDateToDateAndTimeStr(DATE dDateValue)
{
	static CGFmtStr DateStr;
	char szDateStr[30];
	char szTimeStr[20];

	glMyDateTime.DATETimestampToDateTimeStrs(dDateValue, szDateStr, szTimeStr);
	DateStr = szDateStr;
	DateStr += " ";
	DateStr += szTimeStr;
	return(DateStr.Get_sz());
}

// Given a CPlotDB relative timestamp, convert it to a formatted date/time string.
// mod with DATE
const char *CPlotDB::RelDateToDateAndTimeStr(DATE dRelDateValue)
{
	static CGFmtStr DateStr;
	DATE dDateValue;
	
	// Because all plot date data is relative to mulDB_StartDate.
	dDateValue = dRelDateValue + mdDB_StartDate;
	DateStr = AbsDateToDateAndTimeStr(dDateValue);
	return(DateStr.Get_sz());
}

// Given a julian timestamp, return only the time string
// mod with DATE
const char *CPlotDB::AbsDateToTimeStr(DATE dDateValue)
{
	static CGFmtStr TimeStr;
	char szDateStr[30];
	char szTimeStr[20];

	glMyDateTime.DATETimestampToDateTimeStrs(dDateValue, szDateStr, szTimeStr);
	TimeStr = szTimeStr;
	return(TimeStr.Get_sz());
}

// Given a CPlotDB relative timestamp, return only the time string
// mod with DATE
const char *CPlotDB::RelDateToTimeStr(DATE dRelDateValue)
{
	static CGFmtStr TimeStr;
	DATE dDateValue;
	
	// Because all plot date data is relative to mlDspStartOffset.
	dDateValue = dRelDateValue + mdDB_StartDate;
	TimeStr = AbsDateToTimeStr(dDateValue);
	return(TimeStr.Get_sz());
}

// Return the values for the database start and end times for any station
// mod with DATE
BOOL CPlotDB::GetDateRange(DATE *pdDB_StartDate, DATE *pdDB_EndDate)
{
	BOOL bSuccess = TRUE;
	GetDB_Parameters();		// reread the db parameters to ge the most recent start and end times. sfk
	*pdDB_StartDate = mdDB_StartDate;
	*pdDB_EndDate = mdDB_EndDate;
	return(bSuccess);
}
                                                               
// Given a CPlotDB relative timestamp, determine which absolute day this is.
// sfk try to eliminate this call
// mod with DATE
double CPlotDB::RelDateToDate(DATE dRelDate)
{
	DATE dRealDate;

	// Because all plot date data is relative to mulDB_StartDate.
	dRealDate = dRelDate + mdDB_StartDate;
	return(dRealDate);
}

//////////////////////////////////////////////////////////////////////////////////////
//  Read the data values, the events and the marks for the specified channel
// for the specified time interval
// SFK 2.06f Changed ppdhaData, ppdhaTimestamps, dDateArrayOffset from arrays of 
// floats to arrays of doubles
// SFK 2.10 - changed DbReadArray to return the actual time of the next point rather than
//		calculate it in this routine.
//			- changed the subtraction for Grand data to assume the array returned from DbReadArray
//		is not zero-based.

BOOL CPlotDB::ReadChannelData(
	UINT uiPL_ChanIndex,	// I 0-based picklist index.
	DATE dDspStartOffset,	// I Date of first data relative to mdDB_StartDate
	DATE dViewPeriodDays,	// I Time period of data desired.
	BOOL *pbAbortRead,		// IO Address of variable set true to stop read.
	double huge **ppdhaData,	// O ptr to array of data values.
	double huge **ppdhaTimestamps, // O ptr to array of dates.
	ULONG *pulNumDataPts,	// O ptr to # of data points in current interval.
	ULONG *pulNumEvents,	// O ptr to # of events in DB for current interval & chan.
	EVENTS1 **paEvents,		// O ptr to array of events.
	ULONG *pulNumMarks,		// O ptr to # of marks in DB for current interval & chan.
	MARKS1 **paulMarks,		// O ptr to array of marks.
	TwoLineProgressBar *pProgress,
	ULONG *pulNumPrevPts,	// IO ptr to number of points read before this read.
	double dFracBefore,		// I fraction completed before this read.
	double dFracAfter)		// I fraction completed after this read.
{
	struct IDStruct ChanID;

	if (!mbValidDB)
		return false;//goto quit;

	if (!pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID))
		return false; //goto quit;


	if(ChanID.sStaType == EOSS_TYPE)
	{
		BOOL breturn = 
			ReadChannelDataForEOSS(
			uiPL_ChanIndex,	// I 0-based picklist index.
			dDspStartOffset,	// I Date of first data relative to mdDB_StartDate
			dViewPeriodDays,	// I Time period of data desired.
			pbAbortRead,		// IO Address of variable set true to stop read.
			ppdhaData,	// O ptr to array of data values.
			ppdhaTimestamps, // O ptr to array of dates.
			pulNumDataPts,	// O ptr to # of data points in current interval.
			pulNumEvents,	// O ptr to # of events in DB for current interval & chan.
			paEvents,		// O ptr to array of events.
			pulNumMarks,		// O ptr to # of marks in DB for current interval & chan.
			paulMarks,		// O ptr to array of marks.
			pProgress,
			pulNumPrevPts,	// IO ptr to number of points read before this read.
			dFracBefore,		// I fraction completed before this read.
			dFracAfter);		// I fraction completed after this read.

		return breturn;
	}


	// Until we are able to deal with huge arrays limit size of allocation
	// to a single segment (64k = 0x10000) minus a little allocation
	// book-keeping overhead.
	const ULONG ulDB_AT_ONCE_PTS = (0x10000 - 100L) / sizeof(double); //changed size to double 2.06f
	
	DATE dTimeOfFirstDataThisRead;	// timestamp of the actual first data point >= ulFlFrist
	ULONG ulNumPts;
	DATE dFirstDateThisViewPeriod;
	DATE dLastDateThisViewPeriod;

	//const ULONG ulRETRIEVE_PERIOD_SECS = 3600L * 4L;  // 4 hours
	//const double dRETRIEVE_PERIOD_SECS = (3600.0 * 4.0)/86400.0;  // 4 hours in DATE
	//DR00254 - was too slow with the definitions above.  The following helped a great deal
	// to speed things up.
	const double dRETRIEVE_PERIOD = 110.0;  // try to read data from 110 days to speed things up

	struct db_sta_rec dbStaRec;
	//struct IDStruct ChanID;
	
	// Set safe values -- may be changed below if success.
	bool bSuccess = false;
	int iStatus = 0;
	bool bFinished = false;

	*pulNumDataPts = 0;
	*ppdhaData = NULL;
	*ppdhaTimestamps = NULL;
	*pulNumEvents = 0;
	*paEvents = NULL;
	*pulNumMarks = 0;
	*paulMarks = NULL;

	ULONG ulPtsReadThisTime = 0L;
	ULONG ulNumPtsToReadThisTime = 0L;
	ULONG ulTotalPtsRead = 0L;
	DATE dFirstDateThisRead = 0.0;// = dFirstDateThisViewPeriod;
	DATE dLastDateThisRead = 0.0, dTimeOfNextPoint = 0.0;

	double *daTimestampsBuffer = NULL;
	double *daDataBuffer = NULL;

	//for the progress bar (move these down eventually)
	CGFmtStr Text, Text1;
	char szChanName[DB_NAME_LEN+1];
	double dDeltaFrac;// = dFracAfter - dFracBefore;
	double dFracThisRead;// = (double)ulTotalPtsRead / (double)ulNumPts;
	double dCurrentFrac;// = dFracBefore + dDeltaFrac * dFracThisRead;
#ifdef _DEBUG
	extern TextFile *pDebugFile;
#endif
/*
	if (!mbValidDB)
		goto quit;

	if (!pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID))
		goto quit;
*/
	if (!pglFacCfg->GetStationRecord(glsFacNum, &ChanID, &dbStaRec))
		goto quit;

	dFirstDateThisViewPeriod = dDspStartOffset + mdDB_StartDate;
	// ASSUMING the end points are inclusive and there are no fractional
	// seconds we need to subtract 1 second when computing dLastDateThisViewPeriod.
	// 3.00 dLastDateThisViewPeriod is the viewperiod + start
	dLastDateThisViewPeriod = dFirstDateThisViewPeriod + dViewPeriodDays;

	// don't retrieve data past the end of the db
	if (dLastDateThisViewPeriod > mdDB_EndDate) 
		dLastDateThisViewPeriod = mdDB_EndDate;

	// add this check to eliminate problem when move slider to far right, fix better later
	if (dLastDateThisViewPeriod == dFirstDateThisViewPeriod) 
	{
		iStatus = 1;
		ulNumPts = 0L;
		goto quit;
	}

	// Find out how many points are in the interval for the channel
	DATE dDum;
	iStatus = GetDataStatistics(
		ChanID, FALSE, 
		dFirstDateThisViewPeriod, 
		dLastDateThisViewPeriod, 
		&dTimeOfFirstDataThisRead, 
		&dDum, &ulNumPts);

	if (iStatus != 1)
		goto quit;

	//EOSS can return 0 pts, but still give the point before and after
	// HN 3-27-08
	//if (ChanID.sStaType != EOSS_TYPE)
	//if (ChanID.sStaType == EOSS_TYPE)  //Heather
	//{	
	//	ulNumPts += 2;
	//}
	if (ulNumPts == 0L)
	{

		bSuccess = true;	// There is no data but the channel exists.
		goto quit;
	}
		
	// Allocate storage for all the data to be read in the interval
	*ppdhaData = (double huge *)CGUI_HugeMem::Alloc(ulNumPts, sizeof(double));
	*ppdhaTimestamps = (double huge *)CGUI_HugeMem::Alloc(ulNumPts, sizeof(double));
	daTimestampsBuffer = new double[ulDB_AT_ONCE_PTS];
	daDataBuffer = new double[ulDB_AT_ONCE_PTS];

	if ((*ppdhaData == NULL) || 
		(*ppdhaTimestamps == NULL) || 
		(daTimestampsBuffer == NULL) || 
		(daDataBuffer == NULL)) 
	{
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
		goto quit;
	}

	// Read points from selected view period into our data buffers.
	dFirstDateThisRead = dFirstDateThisViewPeriod;
#ifdef _DEBUG
	CHAR bigbuff[1024], t1[64], t2[64], t3[64];
	{
	strcpy(t1, AbsDateToDateAndTimeStr(dFirstDateThisRead));
	sprintf(bigbuff, "\nGetting %lu points\nSta %d, Type %s, Type %d, Data %d, Channel %d\nfirst mark: %s %5.9f\n",
		ulNumPts,
		ChanID.sStaID,ChanID.szStaType,ChanID.sStaType, ChanID.sDataType,ChanID.sChanID,
		t1, dFirstDateThisRead);	pDebugFile->WriteLineToTextFile(bigbuff);
	}
#endif
	// start of loop to read all data in the view period into allocated arrays
	do 
	{
		// determine the last timestamp to read to on this read
		// 01-Apr-2005  Change RETRIEVE_PERIOD_SECS to DATE
		// 21-Feb-2006	DR00254 : Changed to be a 110 day period
		dLastDateThisRead = dFirstDateThisRead + dRETRIEVE_PERIOD;
		if (dLastDateThisRead >= dLastDateThisViewPeriod) 
		{
			bFinished = true;
			dLastDateThisRead = dLastDateThisViewPeriod;
#ifdef _DEBUG
			strcpy(t1, AbsDateToDateAndTimeStr(dLastDateThisRead));
			sprintf(bigbuff, "finished mark: %s %5.9f\n\n",t1, dLastDateThisRead);	pDebugFile->WriteLineToTextFile(bigbuff);
#endif

		}

		// adjust the last timestamp to be a shorter interval if there are more
		// than ulDB_AT_ONCE_PTS in it.
		do 
		{		
			 iStatus = GetDataStatistics(
				ChanID, FALSE, dFirstDateThisRead, dLastDateThisRead, 
				&dTimeOfFirstDataThisRead, &dDum, &ulNumPtsToReadThisTime);  // BMEND-125 due to a call here through to TimeAlign, then GrandImport where the data ends prematurely

#ifdef _DEBUG
				strcpy(t1, AbsDateToDateAndTimeStr(dFirstDateThisRead));
				strcpy(t2, AbsDateToDateAndTimeStr(dLastDateThisRead));
				strcpy(t3, AbsDateToDateAndTimeStr(dTimeOfFirstDataThisRead));
				sprintf(bigbuff, "GDS: %d =\n fdr: %s %5.9f\n ldr: %s %5.9f\n tfdr:%s %5.9f\n",iStatus, 
				t1, dFirstDateThisRead, t2, dLastDateThisRead, t3,dTimeOfFirstDataThisRead);	pDebugFile->WriteLineToTextFile(bigbuff);
#endif
			if (iStatus == uiDAY_NOT_IN_DB_STATUS) 
			{
				iStatus = 1;
				// SFK, handle case of no data this day for this subinterval but make sure
				// we don't drop out of the loop and miss data after the empty day.
			}
			else 
			{
				// If the number of pts in the interval is too great for the buffer cut
				// the time period in half until number of pts is few enough.
				if ((iStatus == 1) && (ulNumPtsToReadThisTime > ulDB_AT_ONCE_PTS)) 
				{
					DATE dCurrentPeriodDuration = dLastDateThisRead - dFirstDateThisRead;
					dLastDateThisRead = dFirstDateThisRead + dCurrentPeriodDuration/2.0;
#ifdef _DEBUG
					strcpy(t1, AbsDateToDateAndTimeStr(dLastDateThisRead));
					sprintf(bigbuff, "Last read adjustment %lu > %lu Pts \n ldr: %s %5.9f\n\n",ulNumPtsToReadThisTime, ulDB_AT_ONCE_PTS, t1, dLastDateThisRead);	pDebugFile->WriteLineToTextFile(bigbuff);
#endif
				}	
				// Only check *pbAbortRead if pProgressDlg != NULL because 
			}
		} while ((iStatus == 1) && (ulNumPtsToReadThisTime > ulDB_AT_ONCE_PTS));     

		// SFK 5-20-2002 If we reduced the interval in the last loop the condition
		//	we set earlier may no longer be true
		if (dLastDateThisRead < dLastDateThisViewPeriod)
		{
#ifdef _DEBUG
			strcpy(t1, AbsDateToDateAndTimeStr(dLastDateThisRead));
			strcpy(t2, AbsDateToDateAndTimeStr(dLastDateThisViewPeriod));
			sprintf(bigbuff, "* Not finished  because ldr < ldtvp:\n*  %s < %s, %5.9f < %5.9f\n\n",
				t1, t2,
				dLastDateThisRead, dLastDateThisViewPeriod);	pDebugFile->WriteLineToTextFile(bigbuff);
#endif
			bFinished = false;	
		}
											
		// now have an interval that we can hold in our temporary arrays
		if ((iStatus == 1) && (ulNumPtsToReadThisTime > 0L)) 
		{
			iStatus = GetDataByChannel(
				ChanID, dTimeOfFirstDataThisRead, 
				dLastDateThisRead, ulNumPtsToReadThisTime, 
				&ulPtsReadThisTime, daTimestampsBuffer, 
				daDataBuffer, &dTimeOfNextPoint);
#ifdef _DEBUG
			strcpy(t1, AbsDateToDateAndTimeStr(dTimeOfFirstDataThisRead));
			strcpy(t2, AbsDateToDateAndTimeStr(dLastDateThisRead));
			strcpy(t3, AbsDateToDateAndTimeStr(dTimeOfNextPoint));
			sprintf(bigbuff, "GDBC %lu Pts:\n tfdr:%s %5.9f\n ldr: %s %5.9f\n tnp: %s %5.9f\n", ulPtsReadThisTime,
				t1, dTimeOfFirstDataThisRead,
				t2, dLastDateThisRead,
				t3, dTimeOfNextPoint);	pDebugFile->WriteLineToTextFile(bigbuff);
#endif
			// Save previous total pts read as destination index then increment it
			// to reflect number of pts just read.
			ULONG ulDestIndex = ulTotalPtsRead;
			ulTotalPtsRead += ulPtsReadThisTime;  
													
			// Destination arrays are dimensioned ulNumPts -- make sure to stay in bounds.
			if (ulTotalPtsRead > ulNumPts) 
			{
#ifdef _DEBUG
				sprintf(bigbuff, "Total Pts Read > Pts\n [%lu] %lu > %lu (read %lu)\n", ulDestIndex, ulTotalPtsRead, ulNumPts, ulPtsReadThisTime);
				pDebugFile->WriteLineToTextFile(bigbuff);
#endif
				ulPtsReadThisTime -= (ulTotalPtsRead - ulNumPts);
				ulTotalPtsRead = ulNumPts;
				bFinished = true;
#ifdef _DEBUG
				sprintf(bigbuff, "!!!mark finished\n %lu adjusted read, %lu\n", ulPtsReadThisTime, ulTotalPtsRead);
				pDebugFile->WriteLineToTextFile(bigbuff);
#endif
			}
			else if (ulTotalPtsRead == ulNumPts) 
			{
				// If we have read as many points as are in the interval we are finished.
				// We put an explicit test here because of a bug in FindIntervalInDb which
				// returns ulNumPtsToRead = ulNumPts if we feed in a time interval which is
				// after the end of data for the day.
				// Setting bFinished here is also more efficient in that we drop out of the
				// loop without unneccessay calls to FindeIntervalInDb and ReadDbDataArray.
				// AND perhaps it is appropriate for these two routines to return a non-SUCCESS
				// status if we go beyond the end of data and that then cause ReadChannelData
				// to return with a non-successful status.
				bFinished = true;
			}
			else if (ulPtsReadThisTime < ulNumPtsToReadThisTime) 
			{		// 06-Nov-2004 Added this condition
				GUI_MsgBox("Incomplete read of data for specified interval.");
			}


			double dDateArrayOffset;
			// the plot routines expect the data to be expressed as time since the start ofthe db

			if (!pglTimeAlign->COMDataType(dbStaRec.s_sta_type)) 
			{
				// Old ReadDbDataArray returns a 0.0-based array of dates.
				// For later convenience have it be the dates relative
				// to the earliest date in the DB, mdDB_StartDate.
				// It turns out that ReadDbDataArray returns an array of dates with the first entry
				// always equal to zero. This corresponds to ulTimeOfFirstData not to ulFirstDateThisRead
				// which was used in the call to FindIntervalInDb.
				dDateArrayOffset = dTimeOfFirstDataThisRead - mdDB_StartDate;

				for (ULONG ulSrcIndex = 0; ulSrcIndex < ulPtsReadThisTime; ulSrcIndex++) 
				{
					(*ppdhaTimestamps)[ulDestIndex] = daTimestampsBuffer[ulSrcIndex] + dDateArrayOffset;
					(*ppdhaData)[ulDestIndex] = daDataBuffer[ulSrcIndex]; 
					ulDestIndex++;
				}
			}
			else 
			{	// data coming from COMs is not zero-based so must
				// adjust times so that they are seconds beyond 
				// copy the temporary data into the huge arrays
#ifdef _DEBUG
				sprintf(bigbuff, "temp array copy up to %lu\n", ulPtsReadThisTime);
				pDebugFile->WriteLineToTextFile(bigbuff);
#endif
				for (ULONG ulSrcIndex = 0; ulSrcIndex < ulPtsReadThisTime; ulSrcIndex++) 
				{
					(*ppdhaTimestamps)[ulDestIndex] = daTimestampsBuffer[ulSrcIndex] - mdDB_StartDate;
					(*ppdhaData)[ulDestIndex] = daDataBuffer[ulSrcIndex]; 
					ulDestIndex++;
				}
#ifdef _DEBUG
				strcpy(t1, AbsDateToDateAndTimeStr(daTimestampsBuffer[ulSrcIndex-1]));
				sprintf(bigbuff, "--->last one %s %5.9f\n", t1, daTimestampsBuffer[ulSrcIndex-1]);
				pDebugFile->WriteLineToTextFile(bigbuff);
#endif
			}

			// Set parameter for the next read
			dFirstDateThisRead = dTimeOfNextPoint;
#ifdef _DEBUG
			sprintf(bigbuff, "end of loop, setting next loop time %5.9f\n\n", dTimeOfNextPoint);
			pDebugFile->WriteLineToTextFile(bigbuff);
#endif

		}
		else 
		{	// there were no data points in the interval so adjust the interval
			// Increment variables for next read.
			dFirstDateThisRead = dLastDateThisRead;
#ifdef _DEBUG
			sprintf(bigbuff, "end of loop with no data, setting next loop time %5.9f\n\n", dLastDateThisRead);
			pDebugFile->WriteLineToTextFile(bigbuff);
#endif
		}
				
		// Update progess bar if present.  ???sfk how does this work
		if (pProgress != NULL) 
		{
			dDeltaFrac = dFracAfter - dFracBefore;
			dFracThisRead = (double)ulTotalPtsRead / (double)ulNumPts;
			dCurrentFrac = dFracBefore + dDeltaFrac * dFracThisRead;

			pglFacCfg->GetChannelLongName(glsFacNum, &ChanID, szChanName, DB_NAME_LEN);
			Text1.Printf("Reading Channel %s", szChanName);
			Text.Printf("%lu data points.", *pulNumPrevPts + ulTotalPtsRead);

			if (!(pProgress->TwoLineProgressBarSetValue(dCurrentFrac, Text1, Text))) 
				iStatus = 0;
		}
	} while ((iStatus == 1) && !bFinished);
									
	if (iStatus == 1) 
	{
		bSuccess = true;
		*pulNumDataPts = ulTotalPtsRead;
		*pulNumPrevPts += ulTotalPtsRead;
							  
		ReadChannelEvents((short)uiPL_ChanIndex, dDspStartOffset, dViewPeriodDays, pulNumEvents, paEvents);
		ReadChannelMarks((short)uiPL_ChanIndex, dDspStartOffset, dViewPeriodDays, pulNumMarks, paulMarks);
	}
#ifdef _DEBUG
	sprintf(bigbuff, "\n\nTotal Points %lu, Total wit Prev %lu\n\n", ulTotalPtsRead, (*pulNumPrevPts));
	pDebugFile->WriteLineToTextFile(bigbuff);
#endif

quit:
	delete [] daTimestampsBuffer;
	delete [] daDataBuffer;
	
	if (!bSuccess) 
	{
		CGUI_HugeMem::Free(*ppdhaData);
		*ppdhaData = NULL;
		CGUI_HugeMem::Free(*ppdhaTimestamps);
		*ppdhaTimestamps = NULL;
		*paEvents = NULL;
		*pulNumDataPts = 0L;
		*pulNumEvents = 0L;
	}

	return(bSuccess?1:0);
}



////////////////////////////////////////////////////////////////////////////////
// Marks

// mod with DATE
BOOL CPlotDB::DeleteMarkRecord(UINT uiPL_ChanIndex, DATE dMarkDate)
{
	struct IDStruct ChanID;
	pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID);
	struct db_mark_rec MarkRec;
	MarkRec.s_mark_chan = ChanID.sChanID;
	MarkRec.s_mark_sta = ChanID.sStaID;
	MarkRec.s_mark_sta_type = ChanID.sStaType;
	MarkRec.d_mark_time_key = dMarkDate + mdDB_StartDate;
	BOOL bSuccess = DeleteMarkRec(&MarkRec) == TRUE;
	return(bSuccess);
}

// mod with DATE
BOOL CPlotDB::CreateMarkRecord(UINT uiPL_ChanIndex, DATE dMarkDate, int& raison)
{
	struct IDStruct ChanID;
	pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID);
	struct db_mark_rec MarkRec;
	MarkRec.s_mark_chan = ChanID.sChanID;
	MarkRec.s_mark_sta = ChanID.sStaID;
	MarkRec.s_mark_sta_type = ChanID.sStaType;
	MarkRec.d_mark_time_key = dMarkDate + mdDB_StartDate;
	BOOL bSuccess = (raison = CreateMarkRec(&MarkRec)) == TRUE;
	return(bSuccess);

}

// mod with DATE	
BOOL CPlotDB::ReadChannelMarks(
	UINT uiPL_ChanIndex,		// I zero-based detector index.
	DATE dDspStartOffset,		// I Date of first data relative to mulDB_StartDate
	DATE dViewPeriodDays,		// I Time period of data desired.
	ULONG *pulNumMarks,
	MARKS1 **pMarks)
{
	BOOL bSuccess = FALSE;
	int iStatus;
	ULONG ulNumMarks;
	DATE dFirstDateThisViewPeriod = dDspStartOffset + mdDB_StartDate;
	DATE dLastDateThisViewPeriod = dFirstDateThisViewPeriod + dViewPeriodDays;
		
	*pulNumMarks = 0;
	*pMarks = NULL;
	
	struct IDStruct ChanID;
	if (pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID)) {
		// Find out how many marks are in the interval for the channel.
		iStatus = GetDataStatisticsForMarks(ChanID, dFirstDateThisViewPeriod, dLastDateThisViewPeriod, &ulNumMarks);
		if (iStatus == TRUE) {
			bSuccess = TRUE;
			if (ulNumMarks > 0) {
				struct db_mark_rec *pMarksFromDb;
				pMarksFromDb = (struct db_mark_rec *)malloc(ulNumMarks * sizeof(struct db_mark_rec));
				if (pMarksFromDb == NULL) {
					bSuccess = FALSE;
				}
				else {
					ULONG ulMarksRead;
					iStatus =  GetDataByStationMarks(ChanID, dFirstDateThisViewPeriod, dLastDateThisViewPeriod, ulNumMarks, &ulMarksRead, pMarksFromDb);
					// Having read full mark structures into local memory,
					// save only the information needed; i.e. mark time.
					if (iStatus == TRUE) {
						*pMarks = new MARKS1[ulMarksRead];
						if (*pMarks == NULL) {
							*pulNumMarks = 0;
						}
						else {
							*pulNumMarks = ulMarksRead;
							for (ULONG ul = 0; ul < ulMarksRead; ul++) {
								(*pMarks)[ul].dRelDateSec = pMarksFromDb[ul].d_mark_time_key - mdDB_StartDate;
							}
						}
					}
					if (pMarksFromDb) free(pMarksFromDb);
				}
			}
		}
	}
	return(bSuccess);
}


//////////////////////////////////////////////////////////////////////////////////
//  Channel Events

BOOL CPlotDB::DeleteEventRecord(UINT uiPL_ChanIndex, DATE dRelDateFirst, DATE dRelDateLast)
{
	struct IDStruct ChanID;
	struct db_event_rec dbChanEvent;

	if (!pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID)) return(FALSE);
	dbChanEvent.d_event_beg_time_key = dRelDateFirst + mdDB_StartDate;
	dbChanEvent.d_event_end_time = dRelDateLast + mdDB_StartDate;

	dbChanEvent.us_event_chan = ChanID.sChanID;
	dbChanEvent.us_event_sta = ChanID.sStaID;
	dbChanEvent.us_event_sta_type = ChanID.sStaType;
	BOOL bSuccess = FALSE;
	if (DeleteDataByChannelEvents(&dbChanEvent) == TRUE) {
		bSuccess = TRUE;
		SaveDatabaseNow();	// flush all the buffers  sfk
	}
	return(bSuccess);
}

BOOL CPlotDB::ReadEventRecord(UINT uiPL_ChanIndex, DATE dRelDateFirst, DATE dRelDateLast, struct db_event_rec *pdbChanEvent)
{
	struct IDStruct ChanID;
	if (!pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID)) return(FALSE);
    struct db_event_rec dbChanEvent;
	dbChanEvent.d_event_beg_time_key = dRelDateFirst + mdDB_StartDate;
	dbChanEvent.d_event_end_time = dRelDateLast + mdDB_StartDate;
	dbChanEvent.us_event_chan = ChanID.sChanID;
	dbChanEvent.us_event_sta = ChanID.sStaID;
	dbChanEvent.us_event_sta_type = ChanID.sStaType;
	BOOL bSuccess = (ReadEventRec(&dbChanEvent, pdbChanEvent) == TRUE);
	return(bSuccess);
}

BOOL CPlotDB::CreateEventRecord(UINT uiPL_ChanIndex, DATE dRelDateFirst, DATE dRelDateLast,	const char *szEventId, UINT uiTypePickIndex)
{
    struct db_event_rec dbChanEvent, dbDum;
	unsigned short usType, usType1;

	struct IDStruct ChanID;
	if (!pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID)) return(FALSE);
	GetEventTypesForPlNum(ChanID, uiTypePickIndex, &usType, &usType1);

	dbChanEvent.d_event_beg_time_key = dRelDateFirst + mdDB_StartDate;
	dbChanEvent.d_event_end_time = dRelDateLast + mdDB_StartDate;
	dbChanEvent.us_event_chan = ChanID.sChanID;
	dbChanEvent.us_event_sta = ChanID.sStaID;
	dbChanEvent.us_event_sta_type = ChanID.sStaType;
	dbChanEvent.us_event_type = usType;
	dbChanEvent.us_event_type1 = usType1;
	strcpy(dbChanEvent.sz_event_id, szEventId);

	// Zero all other fields.
	dbChanEvent.d_event_max_time = 0;
	dbChanEvent.d_event_grp_time = 0;
	dbChanEvent.d_event_integral_sum = 0.0;
	dbChanEvent.d_event_max_value = 0.0;
	dbChanEvent.d_event_avg_reals_rate = 0.0;
	dbChanEvent.d_event_avg_totals_rate = 0.0;
	dbChanEvent.us_event_status = 0;
	dbChanEvent.us_event_status1 = 0;
	dbChanEvent.us_event_type2 = 0;
	dbChanEvent.us_event_status2 = 0;
	dbChanEvent.s_num_runs = 0;

	//BOOL bSuccess = CreateChanEventRec(ChanID, &dbChanEvent);
	//if (bSuccess) {
	// 16-Dec-2004 - changed to only Replace Event so that you correctly update all fields in the event
	BOOL bSuccess = ReplaceChanEvent(ChanID, &dbDum, ChanID, &dbChanEvent); // SFK Added to get real time update
		SaveDatabaseNow();	// flush all the buffers SFK
	//}
	return(bSuccess);
}

BOOL CPlotDB::EditEventRecord(UINT uiPL_ChanIndex, DATE dRelDateFirst, DATE dRelDateLast, DATE dNewLowValueRelSecs, DATE dNewHighValueRelSecs, const char *szEventId, UINT uiTypePickIndex)
{
	BOOL bSuccess = FALSE;
	struct db_event_rec dbOldEvent;
	struct db_event_rec dbNewEvent;
	unsigned short usType, usType1;

	struct IDStruct ChanID;
	if (!pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID)) return(FALSE);
	GetEventTypesForPlNum(ChanID, uiTypePickIndex, &usType, &usType1);

	//Find the original event using these fields:
	//	us_event_chan, d_event_beg_time_key, d_event_end_time.
	//The new event must have valid information in these fields:
	//	us_event_chan, d_event_beg_time_key, d_event_end_time,
	//	us_event_type, us_event_type1.

	dbOldEvent.us_event_chan = ChanID.sChanID;
	dbOldEvent.us_event_sta = ChanID.sStaID;
	dbOldEvent.us_event_sta_type = ChanID.sStaType;
	dbOldEvent.d_event_beg_time_key = dRelDateFirst + mdDB_StartDate;
	dbOldEvent.d_event_end_time = dRelDateLast + mdDB_StartDate;

	dbNewEvent.us_event_chan = ChanID.sChanID;
	dbNewEvent.us_event_sta = ChanID.sStaID;
	dbNewEvent.us_event_sta_type = ChanID.sStaType;
	dbNewEvent.d_event_beg_time_key = dNewLowValueRelSecs + mdDB_StartDate;
	dbNewEvent.d_event_end_time = dNewHighValueRelSecs + mdDB_StartDate;
	dbNewEvent.us_event_type = usType;
	dbNewEvent.us_event_type1 = usType1;
	strcpy(dbNewEvent.sz_event_id, "");
	MyStrnCpy(dbNewEvent.sz_event_id, szEventId,ID_LEN+1);

	if (ReplaceChanEvent(ChanID, &dbOldEvent, ChanID, &dbNewEvent) == TRUE) {
		SaveDatabaseNow();	// flush all the buffers sfk
		bSuccess = TRUE;
	}
	return(bSuccess);
}


BOOL CPlotDB::ReadChannelEvents(
	UINT uiPL_ChanIndex,		// I zero-based picklist index.
	DATE dDspStartOffset,		// I Date of first data relative to mulDB_StartDate
	DATE dViewPeriodDays,		// I Time period of data desired.
	ULONG *pulNumEvents,
	EVENTS1 **paEvents)
{
	BOOL bSuccess = FALSE;
	int iStatus;
	ULONG ulNumEvents;
	DATE dFirstDateThisViewPeriod;
	DATE dLastDateThisViewPeriod;
			
	*pulNumEvents = 0;
	*paEvents = NULL;
	dFirstDateThisViewPeriod = dDspStartOffset + mdDB_StartDate;
	dLastDateThisViewPeriod = dFirstDateThisViewPeriod + dViewPeriodDays;
	
	struct IDStruct ChanID;
	if (pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID)) {
		// Find out how many events are in the interval for the channel.
		iStatus = GetDataStatisticsForEventsByChannel(ChanID, dFirstDateThisViewPeriod, dLastDateThisViewPeriod, &ulNumEvents, GRAPH_EVENTS);
		if (iStatus == TRUE) {
			bSuccess = TRUE;
			if (ulNumEvents > 0) {
				struct db_event_rec huge *pEvents;
				pEvents = (struct db_event_rec huge *)CGUI_HugeMem::Alloc(ulNumEvents, sizeof (struct db_event_rec));
				//struct db_event_rec *pEvents;
				//pEvents = (struct db_event_rec *)malloc(ulNumEvents * sizeof (struct db_event_rec));

	
				if (pEvents == NULL) {
					bSuccess = FALSE;
				}
				else {
					ULONG ulEventsRead;
					iStatus =  GetDataByChannelEvents(ChanID, dFirstDateThisViewPeriod, dLastDateThisViewPeriod, ulNumEvents, &ulEventsRead, pEvents, GRAPH_EVENTS);
					// Having read full event structures into local memory,
					// save only the information needed; i.e. begin and end
					// time of event.
					if (iStatus == TRUE) {
						*paEvents = new EVENTS1[ulEventsRead];
						if (*paEvents == NULL) {
							*pulNumEvents = 0;
						}
						else {
							*pulNumEvents = ulEventsRead;
							for (ULONG ul = 0; ul < ulEventsRead; ul++) {
								(*paEvents)[ul].dRelDateFirst = pEvents[ul].d_event_beg_time_key - mdDB_StartDate;
								(*paEvents)[ul].dRelDateLast = pEvents[ul].d_event_end_time - mdDB_StartDate;
								(*paEvents)[ul].Label = pEvents[ul].sz_event_id;

								switch (pEvents[ul].us_event_type) {
									case CHANNEL_EVENT:
										(*paEvents)[ul].Color = CHANNEL_EVENT_COLOR;
										break;
									case MEASUREMENT_EVENT:
										(*paEvents)[ul].Color = MEASUREMENT_EVENT_COLOR;
										break;
									case RECONCILED_EVENT:
										(*paEvents)[ul].Color = RECONCILED_EVENT_COLOR;
										break;
									case NOT_RECONCILED_EVENT:
									case UNEXPLAINED_EVENT:
										(*paEvents)[ul].Color = NOT_RECONCILED_EVENT_COLOR;
										break;
									case BACKGROUND_EVENT:		// 16-Dec-2004 SFK Changed to plot high/low bkg in different color than normal
										if (pEvents[ul].us_event_type1 == NORMAL_BKG_MEAS) {
											(*paEvents)[ul].Color = BACKGROUND_EVENT_COLOR;
										}
										else {
											(*paEvents)[ul].Color = LOW_BKG_MEAS_COLOR;
										}
										break;
									default:
										(*paEvents)[ul].Color = DEFAULT_EVENT_COLOR;
										break;
								}
							}
						}
					}
					CGUI_HugeMem::Free(pEvents);
					//free(pEvents);
				}
			}
		}
	}
	return(bSuccess);
}


