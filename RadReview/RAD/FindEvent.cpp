/*
This work was supported by the United States Member State Support Program to IAEA Safeguards;
the U.S. Department of Energy, Office of Nonproliferation and National Security, International
Safeguards Division; and the U.S. Department of Energy, Office of Safeguards and Security.

LA-CC-14-040. This software was exported from the United States in accordance with the Export
Administration Regulations. Diversion contrary to U.S. law prohibited.

Copyright 2015, Los Alamos National Security, LLC. This software application and associated
material ("The Software") was prepared by the Los Alamos National Security, LLC. (LANS), under
Contract DE-AC52-06NA25396 with the U.S. Department of Energy (DOE). All rights in the software
application and associated material are reserved by DOE on behalf of the Government and LANS
pursuant to the contract.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or other materials provided
with the distribution.
3. Neither the name of the "Los Alamos National Security, LLC." nor the names of its contributors
may be used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS
NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRAT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : FindEvent.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

//19 Apr 2006 - pjm - changed code to reduce compiler warnings.


///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		EventFind
//		PrintChanEventResults

//	Static
//		ThresholdEventFind
//		BusyCancelBtnServiceFn
//		PrintChanEventToTextDBFileForIR
//		FormatChanEvent
//		PrintChanEventToTextFile
//		PrintChanEventToTextWin
//		WriteEventToDb
//		SlidingWindowEventFind
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "DateTimeUtils.h"
#include "DbEvent.h"
#include "DbInterval.h"
#include "DbReadStruct.h" 
#include "EventNetArea.h"
#include "EventTypeLists.h"
#include "FacilityConfig.h"
#include "FindEvent.h"
#include "ProgressBars.h"
#include "RadInit.H" 
#include "TextFile.h"
#include "upperlayer/TXTWRDB.H"		// 8-Dec-04 SFK Added relative path
#include "Utilities.h"
#include "FacilityConfig.h" //KM
#include "MyDateTime.h"
#include "TypeDefinitions.h"

#define NUM_FE_ALLOC_PTS 1600

extern RADInit *pRAD_Init;
extern BOOL glImportExit;
extern short glsFacNum; //KM
extern CMyDateTime glMyDateTime;

#define EVENT_TYPE_LEN 12
struct EVENT1 {
	BOOL bStarted;
	DATE dBegTime;
	DATE dEndTime;
	DATE dMaxTime;
	double dMaxValue;   
	double dIntSum;
	double dRealsSum;
	double dTotalsSum;
	double dSumTime;
	unsigned short usType;
	unsigned short usStatus;
	unsigned short usMeasType;
	unsigned short usNumPts;
};	

struct STATION_CHANNEL_INFO 
{
    struct IDStruct ChanID;
    char szChanLongName[DB_NAME_LEN+1];
	char szChanShortName[DB_SHORT_NAME_LEN+1];
};

static struct STATION_CHANNEL_INFO gllChanInfo[MAX_NUM_DATA_CHANS];
                                                         
static unsigned long gllulOverlapPts;
static Text_Write_Database *gllpWriteDB = NULL;                       
static BOOL gllbContinue = true;

static BOOL PrintChanEventToTextDBFileForIR(struct IDStruct ChanID, struct db_event_rec *pdbEvent, unsigned long ulNumPrinted, unsigned long ulTotalRecs);
static void FormatChanEvent(struct IDStruct ChanID, struct db_event_rec *pdbChanEvent, char *szChanShortName, unsigned long ulNumPrinted, CGFmtStr *pFormattedStr);
static void PrintChanEventToTextFile(struct IDStruct ChanID, struct db_event_rec *pdbChanEvent, char *szChanShortName, unsigned long ulNumPrinted, TextFile *pFile);
static void PrintChanEventToTextWin(struct IDStruct ChanID, struct db_event_rec *pdbChanEvent, char *szChanShortName, unsigned long ulNumPrinted, CGUI_TextWin *pWin);
static int WriteEventToDb(struct IDStruct ChanID, struct EVENT1 *pEvent, struct db_event_rec *pdbChanEvent);
static void InitializeEventStruct(struct EVENT1 *pEvent);
static void SlidingWindowEventFind(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, struct DATA_ANALYZE *pAnalyzeParms, BOOL bPrintAll, unsigned long ulTotalPts, unsigned long *pulTotalEvents);
static void ThresholdEventFind(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, struct DATA_ANALYZE *pAnalyzeParms, BOOL bPrintAll, unsigned long ulTotalPts, unsigned long *pulTotalEvents);


///////////////////////////////////////////////////////////////////////////
//	Name:	EventFind
//
//	Description:
//	Figure out the names and db channel numbers for channels to be searched.
//	Figure out which algorithm they requested and do the associated search.
//	Verify there is some data for the requested station in the db.
//
//	Declaration:
//	int EventFind(DATE dStart, DATE dEnd, struct ANALYZE_CHANS *pAnalyzeChans, struct DATA_ANALYZE *pAnalyzeParms, BOOL bPrintAll, unsigned long *pulTotalEvents)
//
//	Input:	dStart	Start of period to be searched
//			dEnd	End of period to be search
//			pAnalyzeChans	Channel specific parameters from dialog box
//			pAnalyzeParms	General analysis parameters from dialog box
//			bPrintAll	whether diagnostic prints are turned on
//			
//	Output:	none
//
//	Return:	TRUE (completed successfully)/FALSE (problem)/ CANCELED(user hit cancel button)
//	
//  date    /	author	revision
//  -----------------	--------
//	13-Dec-1995	SFK		Created
//	21-Jun-2002	SFK		Modified for DATE
//	15-May-2003	SFK		Modified to pass total number of events in/out of the function.  Moved updating of rows
//						written in Steve text db files to the calling function.
//////////////////////////////////////////////////////////////////
int EventFind(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, struct DATA_ANALYZE *pAnalyzeParms, BOOL bPrintAll, unsigned long *pulTotalEvents)
{
               
    struct db_chan_rec dbChan;  
    int  status; 
    int  j; 
    DATE dActualStartTime, dDum;
    unsigned long ulTotalPts;
    int  iNumAnalyze;
	CGFmtStr Msg;
 
   /* ------------------------------------------------------------------
    *	Determine how many channels to analyze at this station.  Read
    *	the associated channel names and save for use later in displaying
    * ----------------------------------------------------------------*/
	*pulTotalEvents = 0;
    iNumAnalyze = 0;
    for (j=0; j<MAX_NUM_DATA_CHANS; j++) 
	{
    	if ((pAnalyzeChans->sChanOffset[j]) != -1) 
		{	// is used
    		iNumAnalyze++;
			pAnalyzeChans->StaID.sChanID = pAnalyzeChans->sChanOffset[j];  // this is the channel number
			if (!pglFacCfg->GetChannelRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbChan)) return(FALSE);
    		MyStrnCpy(gllChanInfo[j].szChanLongName, dbChan.sz_chan_name, DB_NAME_LEN);
    		MyStrnCpy(gllChanInfo[j].szChanShortName, dbChan.sz_chan_short_name, DB_SHORT_NAME_LEN);
			gllChanInfo[j].ChanID = pAnalyzeChans->StaID;
    	}                                     
    	else 
		{
    		strcpy(gllChanInfo[j].szChanLongName, "");
			strcpy(gllChanInfo[j].szChanShortName, "");
			gllChanInfo[j].ChanID.sChanID = -1;
			gllChanInfo[j].ChanID.sStaID = -1;;
    	}		
    }	
    
    if (iNumAnalyze == 0) return(TRUE);
    
	//	Check to see that there is data for this day and station in the database.
	//	If no data for this station in the specified time interval, then quit.
	status = GetDataStatistics(pAnalyzeChans->StaID, true, dStart, dEnd, &dActualStartTime, &dDum, &ulTotalPts);
	
	//Add call to find out how many channels at station for EOSS
	if (pAnalyzeChans->StaID.sStaType == EOSS_TYPE)
	{
		int nNumChans = pglFacCfg->GetEnabledChansForStation (glsFacNum,(long) pAnalyzeChans->StaID.sStaID,"EOSS");
		ulTotalPts += (2*nNumChans);
	}

    if (status != TRUE) return(FALSE); 
    if (ulTotalPts == 0) {
		struct db_sta_rec dbSta;
		if (pglFacCfg->GetStationRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbSta)) {
			Msg.Printf("\n\nEvent search terminated due to no data found for %s.",dbSta.sz_sta_name);
    		if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n\n%s", Msg);
		}
    	return(TRUE);
    }	

	// Figure out which search algorithm they requested and do it.
    if (pAnalyzeParms->sWhichAlg == SIMPLE_THRESHOLD) {
    	ThresholdEventFind(dActualStartTime, dEnd, pAnalyzeChans, pAnalyzeParms, bPrintAll, ulTotalPts, pulTotalEvents);
    }	
    if ((pAnalyzeParms->sWhichAlg == SLIDING_WINDOW) || 
    	(pAnalyzeParms->sWhichAlg == SLIDING_WINDOW_WITH_LOCAL_BKG)){
    	SlidingWindowEventFind(dActualStartTime, dEnd, pAnalyzeChans, pAnalyzeParms, bPrintAll, ulTotalPts, pulTotalEvents);
    	
    }	

	if (gllpWriteDB != NULL) {
		pAnalyzeParms->pTextDbFile = gllpWriteDB;
	}

	if (gllbContinue == TRUE) return (TRUE);
    else return(CANCELED);		// hit the cancel button
}    


///////////////////////////////////////////////////////////////////////////
//	Name:	ThresholdEventFind
//
//	Description:
//	We're finally here! Actually perform the threshold search for events.
//	This simple algorithm determines whether each point is above or below
//	the threshold value associated with this channel from the dialog box.
//	If the data point is equal to the threshold value, count the data point
//	as in the event.
//
//	Declaration:
//	static void ThresholdEventFind(unsigned long ulStart, unsigned long ulEnd, struct ANALYZE_CHANS *pAnalyzeChans, struct DATA_ANALYZE *pAnalyzeParms, BOOL bPrintAll, unsigned long ulTotalPts)
//
//	Input:	dStart			Start of period to be searched
//			dEnd			End of period to be search
//			pAnalyzeChans	Channel specific parameters from dialog box
//			pAnalyzeParms	General analysis parameters from dialog box
//			bPrintAll		whether diagnostic prints are turned on
//			ulTotalPts		number of data points in review period for the station we're analyzing now
//			
//	Output:	none
//
//	Return:	TRUE (completed successfully)/FALSE (problem)/ CANCELED(user hit cancel button)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-1997	SFK		Created
//	21-Jun-2002	SFK		Modified for DATE
//	04-Jan-2005	SFK		Modify to track number of points in event
//  06-Apr-2009 JCD		Modify header and summary output to go to log file as well as screen
//////////////////////////////////////////////////////////////////
static void ThresholdEventFind(
	DATE dStart, 
	DATE dEnd, 
	struct ANALYZE_PARMS *pAnalyzeChans, 
	struct DATA_ANALYZE *pAnalyzeParms, 
	BOOL bPrintAll, 
	unsigned long ulTotalPts, 
	unsigned long *pulTotalEvents)

{
               
	struct WHOLE_DOUBLE_DATA_PT1 *pdPts;		// all data for analysis is put in doubles in the wrappers
    struct EVENT1 Event[MAX_NUM_DATA_CHANS];
    struct db_sta_rec dbSta;
    struct db_event_rec dbChanEvent;

    int  status; 
    int  i, j;
    DATE dNextStartTime;  
    unsigned long ulPtsRead;		/* pts to read from db */
    unsigned long ulPtNum;
    DATE dFirstAnalyzedTime = 0, dLastAnalyzedTime = 0;
    unsigned long ulNumPtsLeft;
    unsigned long ulNumEvents = 0;
    unsigned long ulNumAllocPts; 				/* pts were able to allocate in memory */
    unsigned long ulPtsToRead;					/* pts to read from db */
    char szDate[DT_LEN+1], szTime[DT_LEN+1];
    char szDate2[DT_LEN+1], szTime2[DT_LEN+1];
    BOOL bFirstPass = TRUE;

	// clear out the structure we use to write the event db records
	for (j=0; j<MAX_NUM_DATA_CHANS; j++) {
    	InitializeEventStruct(&Event[j]);
    }	
    
    ulNumPtsLeft = ulTotalPts;             
    dNextStartTime = dStart;
    ulPtNum = 0;

   /* ------------------------------------------------------------------
    *	Figure out how many points you need to allocate storage for --
    *	works best if only read and analyze about a 1000 pts at a time;
    *	there's always enough memory for that allocation.
	*	24-Mar-03 For now treat FLOAT and DOUBLE the same since internal
	*	to RAD doubles are treated as floats
	* ----------------------------------------------------------------*/
	if (ulTotalPts > NUM_FE_ALLOC_PTS) {
	    ulNumAllocPts = NUM_FE_ALLOC_PTS;
	}                 
	else {
		ulNumAllocPts = ulTotalPts;
	}	
	ulPtsToRead = ulNumAllocPts;		// number of points we will read from the db each time

   /* ------------------------------------------------------------------
    *	Allocate all the arrays that are needed in the processing.
	*	Reduce size if can't allocate what you need.
	* ----------------------------------------------------------------*/
	//Make this a true array instead HN 3-30-08
	//pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
	pdPts = new WHOLE_DOUBLE_DATA_PT1 [ulNumAllocPts+1];
	while ((pdPts == NULL) && (ulNumAllocPts > 50)){		// try a smaller number if can't allocate the memory
		ulNumAllocPts = ulNumAllocPts - 50;
		//pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
		pdPts = new WHOLE_DOUBLE_DATA_PT1 [ulNumAllocPts+1];
	}   
	if (ulNumAllocPts <= 0) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
    	if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n\nEvent search terminated due to memory allocation error.");
    	return; 
    }	
	
   /* ------------------------------------------------------------------
	*   Print the station specific header
	* ----------------------------------------------------------------*/
	j = 0;
	if (!pglFacCfg->GetStationRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbSta)) return;
	// print station header to string, then to both log file and text window  JCD 4/6/2009
	CGFmtStr Str, StrTmp;
	Str.Printf("\n\nStation = %s\n\n    Channels used            Threshold", dbSta.sz_sta_name);
	for (j = 0; j<MAX_NUM_DATA_CHANS; j++) 
	{  						// print channel names, thresholds
		if (pAnalyzeChans->sChanOffset[j] != -1) 
		{
			if (pAnalyzeParms->pWin)
			{
				StrTmp.Printf("\n%25s  %10.4f", gllChanInfo[j].szChanLongName, pAnalyzeChans->fThres[j]);
				Str += StrTmp;
			}
		}
	}
	if (pAnalyzeParms->pWin) 
		pAnalyzeParms->pWin->Printf(Str);		//print station name
	if (pAnalyzeParms->bLogFile) 
		pAnalyzeParms->pFile->WriteLineToTextFile(Str);
/*	Str.Printf("\n\nStation = %s", dbSta.sz_sta_name);
	if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf(Str);		//print station name
	if (pAnalyzeParms->bLogFile) pAnalyzeParms->pFile->WriteLineToTextFile(Str);
	if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n\n    Channels used            Threshold");
	for (j = 0; j<MAX_NUM_DATA_CHANS; j++) {  						// print channel names, thresholds
		if (pAnalyzeChans->sChanOffset[j] != -1) {
			if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n%25s  %10.4f", gllChanInfo[j].szChanLongName, pAnalyzeChans->fThres[j]);
		}
	}	*/	    
	if (bPrintAll == TRUE) {	// diagnostic print turned on?
		if (pAnalyzeParms->pWin) {
			pAnalyzeParms->pWin->Printf("\nDiagnostic Print Points Results");
			pAnalyzeParms->pWin->Printf("\n\n                  Chan     Pt  Arr");
			pAnalyzeParms->pWin->Printf  ("\nDate     Time      Num    Num  Idx       Data");
						                   /*90.05.07 10:10:01 1234 123456 1800 1234567890*/
		}
	}

   	//  Create the progress bar
	CGFmtStr Msg1, Msg2;
	TwoLineProgressBar *pTwoLineProgressBar;
	gllbContinue = true;
	BOOL bContinue = true;
	Msg1.Printf("Processing station %s.\n", dbSta.sz_sta_name);
	Msg2.Printf("Searched %ld out of %ld bytes", ulPtNum, ulTotalPts);
	pTwoLineProgressBar = new TwoLineProgressBar(ulTotalPts, ulTotalPts/20, Msg1, Msg2, "Radiation Review: Find Channel Events Progress", true, IDI_NEW_RAD_REVIEW);
	if (pTwoLineProgressBar) {
		do {
			i = 0;			    
		   /* ------------------------------------------------------------------
			*	Figure out how many points to read and process
			* ----------------------------------------------------------------*/
			if (ulNumPtsLeft > ulNumAllocPts) {
				ulPtsToRead = ulNumAllocPts;
			}
			else {
				ulPtsToRead = ulNumPtsLeft;
			}
			ulNumPtsLeft = ulNumPtsLeft - ulPtsToRead;
					
		   /* ------------------------------------------------------------------
			*	Read the points from the db
			* ----------------------------------------------------------------*/   
			status = GetDoubleDataByStation(pAnalyzeChans->StaID, dNextStartTime, dEnd, ulPtsToRead, &ulPtsRead, pdPts, &dNextStartTime, TRUE);

			if (status == uiDAY_NOT_IN_DB_STATUS) status = TRUE;	// handle case of no data in this subinterval 

			if (status != TRUE) {							// fatal error in the data read
				bContinue = false;
				break;
			}
			
			if (bFirstPass == TRUE) {	// first point in analysis interval
				dFirstAnalyzedTime = pdPts[0].dJulianTime;
			}                      
			
		   /* ------------------------------------------------------------------
			*	Look for the peaks in NON-EOSS Data
			* ----------------------------------------------------------------*/
			if (dbSta.s_sta_type != EOSS_TYPE)
			{
				do {
					ulPtNum++;
					for (j=0; j<(MAX_NUM_DATA_CHANS); j++) {	// for now only analyze the first 5 data channels at most
			    		if ((pAnalyzeChans->sChanOffset[j]) != -1) { // -1 = don't analyze
							// check if the raw data value should be printed
						 	if (bPrintAll == TRUE) { // write this point to the text window
								glMyDateTime.DATETimestampToDateTimeStrs(pdPts[i].dJulianTime, szDate, szTime);
								if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n%s %s %4d %6ld %4d %10.3f",
							 	   						 szDate, szTime, j, ulPtNum, i, pdPts[i].dData[j]); 
							}					
		
							// look for the first point above the threshold value
							if (pdPts[i].dData[j] >= pAnalyzeChans->fThres[j]) {		// this data point > threshold
			    				if (Event[j].bStarted == FALSE) {						// this is the start of the event
			    					if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)) pAnalyzeParms->pWin->Printf(" Peak Start");	// diagnostic label of start
			    					// fill in the start information in the event record
			    					Event[j].bStarted  = TRUE;
			    					Event[j].dBegTime  = pdPts[i].dJulianTime;    
									Event[j].dMaxTime  = pdPts[i].dJulianTime;			    					
			    					Event[j].dMaxValue = pdPts[i].dData[j];
									if (dbSta.s_sta_type != BINARY_TYPE){
			    						Event[j].dIntSum   = pdPts[i].dData[j] * pdPts[i].dElapsedTime;
									}
									else {
										Event[j].dIntSum   = pdPts[i].dData[j];	//binary events have no concept of elapsed time
									}
			    					Event[j].usType    = CHANNEL_EVENT;
									Event[j].usNumPts  = 1;		// 04-JAN-2005	Count number of points in event	
			    				}
			    				else {	// are in the middle of the event, this is not the first point in the event
									if (dbSta.s_sta_type != BINARY_TYPE){
			    						Event[j].dIntSum += pdPts[i].dData[j] * pdPts[i].dElapsedTime;	// accumulate sum
									}
									else {
										Event[j].dIntSum += pdPts[i].dData[j];							//binary events have no concept of elapsed time
									}
									Event[j].usNumPts++;
			    					if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)) pAnalyzeParms->pWin->Printf(" Above Thres");  // above thres, but not 1st pt
									if (pdPts[i].dData[j] > Event[j].dMaxValue) {  // found a new max??
										Event[j].dMaxTime = pdPts[i].dJulianTime;
										Event[j].dMaxValue = pdPts[i].dData[j];
									}					
			    				}
			    			}
			    			else {	// this point is below threshold 
			    				if (Event[j].bStarted == TRUE) {	
									if (dbSta.s_sta_type == BINARY_TYPE){
										Event[j].dEndTime = pdPts[i].dJulianTime;
									}
									else {
										// last point was in the event, so the previous point is the end of the event
										if (i > 0)  Event[j].dEndTime = pdPts[i-1].dJulianTime;
										else Event[j].dEndTime = pdPts[i].dJulianTime; // Ver 2.05 corrected for case of i==0
									}
									status = WriteEventToDb(gllChanInfo[j].ChanID, &Event[j], &dbChanEvent);		// event to DB
									PrintChanEventResults(gllChanInfo[j].ChanID, &dbChanEvent, gllChanInfo[j].szChanShortName, &ulNumEvents,	// write results to text window
														pulTotalEvents, pAnalyzeParms->pWin, pAnalyzeParms->pFile, 
														pAnalyzeParms->bTextWin, pAnalyzeParms->bLogFile, pAnalyzeParms->bDB_File);
			    					if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)) pAnalyzeParms->pWin->Printf(" Peak End was prev point");
			    					InitializeEventStruct(&Event[j]);
			    				}	    			                
			    			}	
						}	

					}	
		
					//	Periodically update the progress bar
					Msg2.Printf("Searched %ld out of %ld bytes", ulPtNum, ulTotalPts);
					gllbContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
					if (glImportExit == TRUE) gllbContinue = false;
					i++;
					if (ulPtNum > 508800-1601)
						i=i;

				} while ((i<(int)ulPtsRead) && bContinue && gllbContinue);  //fixed compiler complaint                      
			}
			else
			{
				//Look for EOSS peaks
				int nNumChans = pglFacCfg->GetEnabledChansForStation (glsFacNum,(long) dbSta.s_sta_num_key,"EOSS");
				int ChanIndex = -1;
				short CurrChannelID = 0;
				int initialValue = 0;
				dFirstAnalyzedTime = 99999.0;
				dLastAnalyzedTime = 0;
				while ( i < (int) ulPtsRead)
				{
					ulPtNum++;
					
					//Either we are at the same channel ID or not
					if (pdPts[i].eossPt.sChannelID == CurrChannelID)
					{
						if (pAnalyzeChans->sChanOffset[ChanIndex] != -1)//Analyze for this channel
						{
							if (pdPts[i].dJulianTime != 0.0)
							{
								if (pdPts[i].dJulianTime < dFirstAnalyzedTime)
									dFirstAnalyzedTime = pdPts[i].dJulianTime;
								if (pdPts[i].dJulianTime > dLastAnalyzedTime)
									dLastAnalyzedTime = pdPts[i].dJulianTime;
								if (pdPts[i].eossPt.nState == 0)
								{
									if (Event[ChanIndex].bStarted == TRUE)
									{
										Event[ChanIndex].dEndTime = pdPts[i].dJulianTime;	
										status = WriteEventToDb(gllChanInfo[ChanIndex].ChanID, &Event[ChanIndex], &dbChanEvent);		// event to DB
										PrintChanEventResults(gllChanInfo[ChanIndex].ChanID, &dbChanEvent, gllChanInfo[ChanIndex].szChanShortName, &ulNumEvents,	// write results to text window
														pulTotalEvents, pAnalyzeParms->pWin, pAnalyzeParms->pFile, 
														pAnalyzeParms->bTextWin, pAnalyzeParms->bLogFile, pAnalyzeParms->bDB_File);
			    						InitializeEventStruct(&Event[ChanIndex]);
									}//Event ending
								}
								else
								{
									//Should never have 2 values of 1 in a row, start new event
									//Start new event
									Event[ChanIndex].bStarted = TRUE;
									Event[ChanIndex].dBegTime  = pdPts[i].dJulianTime;    
									Event[ChanIndex].dMaxTime  = pdPts[i].dJulianTime;			    					
			    					Event[ChanIndex].dMaxValue = pdPts[i].eossPt.nState;
									Event[ChanIndex].dIntSum   = pdPts[i].eossPt.nState;	//binary events have no concept of elapsed time
			    					Event[ChanIndex].usType    = CHANNEL_EVENT;
									Event[ChanIndex].usNumPts  = 1;		// 04-JAN-2005	Count number of points in event	
								}
							}
							else
							{
								//We are at the end of the interval no points after
								if (Event[ChanIndex].bStarted == TRUE)
								{
									//end event and store
									Event[ChanIndex].dEndTime = dEnd;
									status = WriteEventToDb(gllChanInfo[ChanIndex].ChanID, &Event[ChanIndex], &dbChanEvent);		// event to DB
									PrintChanEventResults(gllChanInfo[ChanIndex].ChanID, &dbChanEvent, gllChanInfo[ChanIndex].szChanShortName, &ulNumEvents,	// write results to text window
														pulTotalEvents, pAnalyzeParms->pWin, pAnalyzeParms->pFile, 
														pAnalyzeParms->bTextWin, pAnalyzeParms->bLogFile, pAnalyzeParms->bDB_File);
			    					InitializeEventStruct(&Event[ChanIndex]);
								} 
							}
						}
					}
					else//Go to next channel
					{
						ChanIndex ++;
						pglFacCfg->GetChannelIDFromIndex (glsFacNum, (long) dbSta.s_sta_num_key,ChanIndex,&CurrChannelID);

						if (pAnalyzeChans->sChanOffset[ChanIndex] != -1) // This is the first point for the channel.
						{
							if (pdPts[i].dJulianTime == 0.0)
							{
								// There was no point preceding the interval
								initialValue = 0;
							}
							else
							{
								initialValue = pdPts[i].eossPt.nState;
								if (pdPts[i].dJulianTime < dFirstAnalyzedTime)
									dFirstAnalyzedTime = pdPts[i].dJulianTime;
								if (pdPts[i].dJulianTime > dLastAnalyzedTime)
									dLastAnalyzedTime = pdPts[i].dJulianTime;
								
							}
							
							//either we start w/event or not.
							if (initialValue >= pAnalyzeChans->fThres[ChanIndex])
							{
								if (Event[ChanIndex].bStarted == FALSE)
								{
									//Start new event
									Event[ChanIndex].bStarted = TRUE;
									Event[ChanIndex].dBegTime  = pdPts[i].dJulianTime;    
									Event[ChanIndex].dMaxTime  = pdPts[i].dJulianTime;			    					
			    					Event[ChanIndex].dMaxValue = pdPts[i].eossPt.nState;
									Event[ChanIndex].dIntSum   = pdPts[i].eossPt.nState;	//binary events have no concept of elapsed time
			    					Event[ChanIndex].usType    = CHANNEL_EVENT;
									Event[ChanIndex].usNumPts  = 1;		// 04-JAN-2005	Count number of points in event	
								}
							}
						}
					}

					if (bPrintAll == TRUE) 
					{   // write this point to the text window
						//Add case for fake pt
						if (pdPts[i].dJulianTime != 0.0)
						{
							glMyDateTime.DATETimestampToDateTimeStrs(pdPts[i].dJulianTime, szDate, szTime);
							if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n%s %s %4d %6ld %4d %8d %4d",
						 	   					 szDate, szTime, j, ulPtNum, i, CurrChannelID,pdPts[i].eossPt.nState); 
						}
						else
							if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf ("\nPoint before or after interval not found for %8d.",CurrChannelID);
					}					

					i++;
					// look for the first point above the threshold value
							/*if (pdPts[i].dData[j] >= pAnalyzeChans->fThres[j]) {		// this data point > threshold
			    				if (Event[j].bStarted == FALSE) {						// this is the start of the event
			    					if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)) pAnalyzeParms->pWin->Printf(" Peak Start");	// diagnostic label of start
			    					// fill in the start information in the event record
			    					Event[j].bStarted  = TRUE;
			    					Event[j].dBegTime  = pdPts[i].dJulianTime;    
									Event[j].dMaxTime  = pdPts[i].dJulianTime;			    					
			    					Event[j].dMaxValue = pdPts[i].dData[j];
									if (dbSta.s_sta_type != BINARY_TYPE){
			    						Event[j].dIntSum   = pdPts[i].dData[j] * pdPts[i].dElapsedTime;
									}
									else {
										Event[j].dIntSum   = pdPts[i].dData[j];	//binary events have no concept of elapsed time
									}
			    					Event[j].usType    = CHANNEL_EVENT;
									Event[j].usNumPts  = 1;		// 04-JAN-2005	Count number of points in event	
			    				}
			    				else {	// are in the middle of the event, this is not the first point in the event
									if (dbSta.s_sta_type != BINARY_TYPE){
			    						Event[j].dIntSum += pdPts[i].dData[j] * pdPts[i].dElapsedTime;	// accumulate sum
									}
									else {
										Event[j].dIntSum += pdPts[i].dData[j];							//binary events have no concept of elapsed time
									}
									Event[j].usNumPts++;
			    					if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)) pAnalyzeParms->pWin->Printf(" Above Thres");  // above thres, but not 1st pt
									if (pdPts[i].dData[j] > Event[j].dMaxValue) {  // found a new max??
										Event[j].dMaxTime = pdPts[i].dJulianTime;
										Event[j].dMaxValue = pdPts[i].dData[j];
									}					
			    				}
			    			}
			    			else {	// this point is below threshold 
			    				if (Event[j].bStarted == TRUE) {	
									if (dbSta.s_sta_type == BINARY_TYPE){
										Event[j].dEndTime = pdPts[i].dJulianTime;
									}
									else {
										// last point was in the event, so the previous point is the end of the event
										if (i > 0)  Event[j].dEndTime = pdPts[i-1].dJulianTime;
										else Event[j].dEndTime = pdPts[i].dJulianTime; // Ver 2.05 corrected for case of i==0
									}
									status = WriteEventToDb(gllChanInfo[j].ChanID, &Event[j], &dbChanEvent);		// event to DB
									PrintChanEventResults(gllChanInfo[j].ChanID, &dbChanEvent, gllChanInfo[j].szChanShortName, &ulNumEvents,	// write results to text window
														pulTotalEvents, pAnalyzeParms->pWin, pAnalyzeParms->pFile, 
														pAnalyzeParms->bTextWin, pAnalyzeParms->bLogFile, pAnalyzeParms->bDB_File);
			    					if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)) pAnalyzeParms->pWin->Printf(" Peak End was prev point");
			    					InitializeEventStruct(&Event[j]);
			    				}	    			                
			    			}*/	
						//}	

				}	
		
					//	Periodically update the progress bar
					Msg2.Printf("Searched %ld out of %ld bytes", ulPtNum, ulTotalPts);
					gllbContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
					if (glImportExit == TRUE) gllbContinue = false;
					i++;
					if (ulPtNum > 508800-1601)
						i=i;

				//} while ((i<(int)ulPtsRead) && bContinue && gllbContinue);  //fixed compiler complaint                      
			}
			bFirstPass = FALSE;			/* have completed at least one pass */

		} while ((ulNumPtsLeft > 0)  && bContinue && gllbContinue);
		
		pTwoLineProgressBar->Close();
		delete pTwoLineProgressBar;
		
		if (dbSta.s_sta_type != EOSS_TYPE)
			dLastAnalyzedTime = pdPts[i-1].dJulianTime;  
		   
		// print summary info for the station to string, then to both log file and text window   JCD 4/6/2009
		if (!gllbContinue) 
			Str.Printf("\nSearch canceled by user.");
		else
		{
			glMyDateTime.DATETimestampToDateTimeStrs(dFirstAnalyzedTime, szDate, szTime);
			glMyDateTime.DATETimestampToDateTimeStrs(dLastAnalyzedTime, szDate2, szTime2);
			Str.Printf("\nSearch completed successfully.\nData analyzed from %s %s to %s %s    (%ld data points)\nTotal Channel Events Found for Station %s = %ld\n\n______________________________________________________________________\n\n" \
				, szDate, szTime, szDate2, szTime2,ulTotalPts - ulNumPtsLeft,dbSta.sz_sta_name, ulNumEvents);
		}
/*		if (pAnalyzeParms->pWin) {
			if (!gllbContinue) pAnalyzeParms->pWin->Printf("\nSearch canceled by user.");
			else pAnalyzeParms->pWin->Printf("\nSearch completed successfully.");
			glMyDateTime.DATETimestampToDateTimeStrs(dFirstAnalyzedTime, szDate, szTime);
			pAnalyzeParms->pWin->Printf("\nData analyzed from %s %s to ", szDate, szTime);
			glMyDateTime.DATETimestampToDateTimeStrs(dLastAnalyzedTime, szDate, szTime);
			pAnalyzeParms->pWin->Printf("%s %s", szDate, szTime);
			pAnalyzeParms->pWin->Printf("    (%ld data points)\n", (ulTotalPts - ulNumPtsLeft));
			pAnalyzeParms->pWin->Printf("Total Channel Events Found for Station %s = %ld\n\n", dbSta.sz_sta_name, ulNumEvents); 
			pAnalyzeParms->pWin->Printf("______________________________________________________________________\n\n");
		}*/
		if (pAnalyzeParms->pWin) 
			pAnalyzeParms->pWin->Printf(Str);
		if (pAnalyzeParms->bLogFile) 
			pAnalyzeParms->pFile->WriteLineToTextFile(Str);
	}

	if (pdPts != NULL) /*free(pdPts)*/delete [] pdPts;
	pdPts = NULL;

    return;
}

// 2.07 Add prints to log file and db file back in    
// 2.06 Remove print to Log File and DB File
void PrintChanEventResults(struct IDStruct ChanID, db_event_rec *pdbEventRec, char *szChanName, unsigned long *pulNumEvents,
					   unsigned long *pulTotalEvents, CGUI_TextWin *pWin, TextFile *pFile,
					   BOOL bTextWin, BOOL bLogFile, BOOL bDBFile)
{
	
	if (bTextWin) PrintChanEventToTextWin(ChanID, pdbEventRec, szChanName, *pulNumEvents, pWin);        

    if (bLogFile) PrintChanEventToTextFile(ChanID, pdbEventRec, szChanName, *pulNumEvents, pFile);        

	if (bDBFile) PrintChanEventToTextDBFileForIR(ChanID, pdbEventRec, *pulNumEvents, *pulTotalEvents);
	
	CGUI_App::ProcessEvents();                /* update the output page */             

	/* accumulate total events printed */   
	(*pulNumEvents)++;
	(*pulTotalEvents)++;
}
	
 

///////////////////////////////////////////////////////////////////////////
//	Name:	PrintChanEventToTextDBFileForIR
//
//	Description:
//	Prints the channel event to the text file to be used by IR.
//
//	Declaration:
//	static int WriteEventToDb(short sChanNum, struct EVENT1 *pEvent, struct db_event_rec *pdbChanEvent)
//
//	Input:	sChanNum	db channel number associated with event
//			pEvent		results of analysis
//			
//	Output:	pdbChanEven	db channel record that was written
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	???			SFK		Created
//	01/10/2000	SFK		2.06 - Removed last two fields from record so have only 10 fields
//	24-Jun-2002	SFK		Changed with DATE
//////////////////////////////////////////////////////////////////

static char *ppszLabels[] = {"RAD-Sequence Num",
							 "RAD-Start Timestamp",
							 "RAD-End Timestamp",
							 "RAD-Max Timestamp",
							 "RAD-Integral Sum",
							 "RAD-Max Value",
							 "RAD-Start Date/Time",
							 "RAD-End Date/Time",							 
							 "RAD-Max Date/Time",							 							 
							 "RAD-Type" };
							 // 10 columns 
							 // max column name length = RAD-Start Timestamp = 19 chars
							 // max column data length = timestamp 0000019970502090810000 = 22           
#define STEVES_LOC_STR_LEN 22
#define STEVES_FORMATTED_STR_LEN 21
static BOOL PrintChanEventToTextDBFileForIR(struct IDStruct ChanID, struct db_event_rec *pdbEvent, unsigned long ulNumPrinted, unsigned long ulTotalRecs)
{   
	CGFmtStr Msg;
	char szLocStr[STEVES_LOC_STR_LEN+1], szStartStr[STEVES_FORMATTED_STR_LEN+1], szEndStr[STEVES_FORMATTED_STR_LEN+1], szMaxStr[STEVES_FORMATTED_STR_LEN+1];
	BOOL bStatus;
	char szEventType[EVENT_TYPE_LEN+1];
	char cSep;
	char szPathAndName[PATH_LEN+DOSNAME_LEN+1];

	// create and open the file if this is the first write	
 	if (gllpWriteDB == NULL) {
		// 3.0.3.9 Added getting path from fac mgr
		//pglFacCfg->GetFacilityDirectory(glsFacNum, szPathAndName, PATH_LEN); 
		//if ((szPathAndName[strlen(szPathAndName)-1] != '\\')) strcat(szPathAndName, "\\");
		//strcat(szPathAndName, "Data\\");
		if (!GetDirectory(glsFacNum, DATA_DIR, szPathAndName, PATH_LEN+DOSNAME_LEN+1)) return(FALSE);
		MyStrnCat(szPathAndName, pRAD_Init->Get_Summary_Data_Filename(), DOSNAME_LEN);
		// put in a place holder for number of rows since we don't know yet
		//if ((gllpWriteDB  = new Text_Write_Database(pRAD_Init->Get_Summary_Data_Filename(), (UINT)10, 99999, (const char **) ppszLabels, (UINT)STEVES_FORMATTED_STR_LEN, (UINT)STEVES_LOC_STR_LEN)) == NULL) return(FALSE);
		if ((gllpWriteDB  = new Text_Write_Database(szPathAndName, (UINT)10, 99999, (const char **) ppszLabels, (UINT)STEVES_FORMATTED_STR_LEN, (UINT)STEVES_LOC_STR_LEN)) == NULL) return(FALSE);
    	if (bStatus = gllpWriteDB->Database_Created() != TRUE) {
    		RadReviewMsg(uiFILE_WRITE_ERR, pRAD_Init->Get_Summary_Data_Filename());
    		return(FALSE);	
    	}	
    }
                  
	// separator is always tab for these files                  
    cSep = '\t';  
    
    // write sequence number
    Msg.Printf("%04ld%c", ulTotalRecs, cSep);

	/* write start date/time as a timestamp */
	glMyDateTime.DATETimestampToStevesTimestampStrings(pdbEvent->d_event_beg_time_key, szLocStr, szStartStr);
	Msg.PrintfAppend("%s%c", szLocStr, cSep);
														
	/* write end date/time as a timestamp */
	glMyDateTime.DATETimestampToStevesTimestampStrings(pdbEvent->d_event_end_time, szLocStr, szEndStr);
	Msg.PrintfAppend("%s%c", szLocStr, cSep);
	
	/* write max date/time as a timestamp */
	glMyDateTime.DATETimestampToStevesTimestampStrings(pdbEvent->d_event_max_time, szLocStr, szMaxStr);
	Msg.PrintfAppend("%s%c", szLocStr, cSep);

	/* write integral sum */
	if (pdbEvent->d_event_integral_sum < 10.0)  {
		Msg.PrintfAppend("%0.6f%c", pdbEvent->d_event_integral_sum, cSep);	// Integral Sum
	} 
	else {
		Msg.PrintfAppend("%8.1f%c", pdbEvent->d_event_integral_sum, cSep);	// Integral Sum
	}	
	
	/* write max value */
	if (pdbEvent->d_event_max_value > 0) {	
		Msg.PrintfAppend("%7.0f%c", pdbEvent->d_event_max_value, cSep);	// maximum value   
	}
	else {	
		Msg.PrintfAppend("%0.6f%c", pdbEvent->d_event_max_value, cSep);	// maximum value   	
	}	
	                     
	/* write start date/time in format of yyyymmdd - xx:xx:xx */
	Msg.PrintfAppend("%s%c", szStartStr, cSep);

	/* write end date/time in format of yyyymmdd - xx:xx:xx */
	Msg.PrintfAppend("%s%c", szEndStr, cSep);
	
	/* write max date/time in format of yyyymmdd - xx:xx:xx */
	Msg.PrintfAppend("%s%c", szMaxStr, cSep);

	
	/* Print event type */
	GetEventTypeStr(pdbEvent, szEventType);
	Msg.PrintfAppend("%s", szEventType);

	// actually write to the file
	bStatus = gllpWriteDB->Row_Add(Msg);
	if (bStatus != TRUE) {
		RadReviewMsg(uiFILE_WRITE_ERR, pRAD_Init->Get_Summary_Data_Filename());
		return(FALSE);
	}
	return(TRUE); 
}

void CloseTextDBFileForIR(const unsigned long ulTotalEvents)
{
	if (gllpWriteDB != NULL) {
		// now we finally know how many events we put in the file, so rewind the file, find "Rows", back up 6 and 
		// replace the 99999 we put in as a place holder with the actual number of rows.  Aaargh!
		char str[6];
		sprintf(str,"%-5ld", ulTotalEvents);
		gllpWriteDB->Rows_Set_Number(str);
		gllpWriteDB->Database_Close();
		delete gllpWriteDB;
		gllpWriteDB = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////
//	Name:	FormatChanEvent
//
//	Description:
//	Formats the channel event to a string for general use.
//
//	Declaration:
//	static void FormatChanEvent(struct db_event_rec *pdbChanEvent, char *szChanShortName, unsigned long ulNumPrinted, CGFmtStr *pFormattedStr)
//
//	Input:	pdbChanEvent	event to format
//			szChanShortName	name of channel event goes with
//			ulNumPrinted	which number the event is
//			
//	Output:	pFormattedStr	string containing a line of information about the event (Start timestamp,
//							End timestamp, width/num pts, maximum rate, max timestamp, integral sum,
//							net area, channel short name)
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//  			SFK		Created
//	08-Nov-2001	SFK		Changed width calculation to be end - start + 1 so includes endpoints
//	09-Nov-2001	SFK		Added calculation and print of net area
//	24-Jun-2002 SFK		Changed with DATE
//	07-Jul-2005	SFK		Added calculation of Rokkasho-specific ratios and changed formatting of output
//	11-Jul-2005	SFK		Modified printing format to be more consistent with Measurements and do either ratios
//						or intergral/average based on parameter in RAD.INI
//////////////////////////////////////////////////////////////////
static void FormatChanEvent(struct IDStruct ChanID, struct db_event_rec *pdbChanEvent, char *szChanShortName, unsigned long ulNumPrinted, CGFmtStr *pFormattedStr)
{
	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	char szEventType[EVENT_TYPE_LEN+1];
	DATE dWidth;
	CGFmtStr Str;

	if (pFormattedStr == NULL) return;	// nothing to print to

	if (ulNumPrinted == 0) 
	{	/* this is the first event found so print the header */
		// SCR 414 pjm 14 Dec 2006 Added Event ID column
		if ((pdbChanEvent->us_event_sta_type == GRAND_TYPE) && (pRAD_Init->Get_EnableRatios() == TRUE)) {
			Str.Printf("\n\n      Start    Start    End       Width    Num    Event        Maximum  Maximum    G1/       NA/        NC/   Chan   Event");
			*pFormattedStr += Str;
			Str.Printf(  "\n      Date     Time     Time     (secs)    Pts     Type           Rate     Time  (NA+NB)     NB       (NA+NB) Name   ID");
			*pFormattedStr += Str;
		}
		//Add Special EOSS header
		else if (pdbChanEvent->us_event_sta_type == EOSS_TYPE)
		{
			Str.Printf("\n\n      Start    Start    End      End          Width     Num  Event          Maximum Maximum        Integral     Net    Chan   Event");
			*pFormattedStr += Str;
			Str.Printf(  "\n      Date     Time     Date     Time        (secs)     Pts  Type           Rate    Time           Sum          Area   Name   ID");
			*pFormattedStr += Str;
		}
		else 
		{
			//HN 4-8-08 Change to print the avg rate for an event.
			Str.Printf("\n\n      Start    Start    End       Width    Num    Event        Maximum  Maximum     Integral          Net          Avg    Chan   Event");
			*pFormattedStr += Str;                                                        
			Str.Printf(  "\n      Date     Time     Time     (secs)    Pts     Type           Rate     Time          Sum         Area         Rate    Name      ID");
			*pFormattedStr += Str;
		}
	}
	
	//Add avg rate for printout HN 4-8-08 #233
	double dNetArea, dRatio1, dRatio2, dRatio3, dAvg;
	unsigned long ulNumPts;
	// Calculate the net area of the event
	EventCalculateNetArea(ChanID, pdbChanEvent, &dNetArea, &ulNumPts, &dRatio1, &dRatio2, &dRatio3);

	// print an event number
	Str.Printf("\n%5ld ", ulNumPrinted+1);
	*pFormattedStr += Str;

	// print the starting date/time
	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_beg_time_key, szDate, szTime);
	Str.Printf("%s %s ", szDate, szTime);		// start time
	*pFormattedStr += Str;

	//print the ending date/time
	// 07-Jul-2005 SFK Eliminate printing end date
	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_end_time, szDate, szTime);
	if (pdbChanEvent->us_event_sta_type != EOSS_TYPE)
		Str.Printf("%s ", szTime);  		// end time
	else
		Str.Printf("%s %s ", szDate, szTime);

	*pFormattedStr += Str;

	//	calculate and print the width (in seconds) and number of data points in event
	dWidth = (pdbChanEvent->d_event_end_time - pdbChanEvent->d_event_beg_time_key) *86400.0; 
	if (dWidth == 0.0) dWidth = 1.0;
	if (pdbChanEvent->us_event_sta_type != EOSS_TYPE)
		Str.Printf("%6.0f %6ld  ", dWidth, ulNumPts );	// width and actual number of points
	else
		Str.Printf("%10.0f %6ld  ", dWidth, ulNumPts );	// width and actual number of points
	*pFormattedStr += Str;
								
	// Print event type 
	GetEventTypeStr(pdbChanEvent, szEventType);
	Str.Printf("%s ", szEventType);
	*pFormattedStr += Str;

	// Print the maximum value, based on value of number use one of two formats
	if (pdbChanEvent->d_event_max_value > 10) {	// large positive value
		if (pdbChanEvent->d_event_max_value >= 100000000) {
			Str.Printf("%9.3e ", pdbChanEvent->d_event_max_value);	// 11-Jul-2005 SFK Added e format
		}
		else {
			Str.Printf("%9.1f ", pdbChanEvent->d_event_max_value);	// maximum value   
		}
	}
	else {	// small positive value	
		if (pdbChanEvent->d_event_max_value > 0) {	
			Str.Printf("%9.5f ", pdbChanEvent->d_event_max_value);	// maximum value   	
		}
		else { // negative value
			Str.Printf("%9.1f ", pdbChanEvent->d_event_max_value);	// maximum value   	
		}
	}
	*pFormattedStr += Str;
	
	// print the max time
	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_max_time, szDate, szTime);
	Str.Printf("%s ", szTime);		// time of maximum value in peak
	*pFormattedStr += Str;

	// 11-Jul-2005 SFK Added printing either integral/area or ratios based on parameter in RAD.INI
	if ((pdbChanEvent->us_event_sta_type == GRAND_TYPE) && (pRAD_Init->Get_EnableRatios() == TRUE)) {
		// 7-Jul-2005 SFK  print the ratios
		Str.Printf("%9.2e %9.2e %9.2e ", dRatio1, dRatio2, dRatio3);
		*pFormattedStr += Str;
	}
	else {
		// Print the integral sum, based on value of number use one of three formats
		if (pdbChanEvent->d_event_integral_sum < 999.0)  { // small number
			if (pdbChanEvent->d_event_integral_sum > 10.0)  {
				Str.Printf("%12.1f ", pdbChanEvent->d_event_integral_sum);	// Integral Sum
			}
			else {	// smallest
				Str.Printf("%12.4f ", pdbChanEvent->d_event_integral_sum);	// Integral Sum
			}
		} 
		else {	// large number
			if (pdbChanEvent->d_event_integral_sum > 1000000000000)  { // very large number
				Str.Printf("%12.5e ", pdbChanEvent->d_event_integral_sum);		// 11-Jul-2005 SFK Added e format
			}
			else {
				Str.Printf("%12.0f ", pdbChanEvent->d_event_integral_sum);		// Integral Sum
			}
		}	
		*pFormattedStr += Str;

		// Add the net area to the print out information
		if (dNetArea < 999.0)  { // small number
			if ((dNetArea > 10.0) || (dNetArea < 0))  {
				Str.Printf("%12.1f ", dNetArea);	// Net Area
			}
			else {	// smallest
				Str.Printf("%12.4f ", dNetArea);	// Net Area
			}
		} 
		else {	// large number
			if (dNetArea > 1000000000000)  { // very large number
				Str.Printf("%12.5e ", dNetArea);		// Net Area
			}
			else {
				Str.Printf("%12.0f ", dNetArea);		// Net Area
			}
		}	
		*pFormattedStr += Str;
		//New section to print avg rate.  HN #233 4-8-08
		//Do not print for EOSS
		if (pdbChanEvent->us_event_sta_type != EOSS_TYPE)
		{
			dAvg = (pdbChanEvent->d_event_integral_sum) / dWidth;
			if (dAvg < 999.0)  { // small number
				if (dAvg > 10.0)  {
					Str.Printf("%12.1f ", dAvg);	// Avg rate for event
				}
				else {	// smallest
					Str.Printf("%12.4f ", dAvg);	// Avg rate for event
				}
			} 
			else {	// large number
				if (dAvg > 1000000000000)  { // very large number
					Str.Printf("%12.5e ", dAvg);		// 11-Jul-2005 SFK Added e format
				}
				else {
					Str.Printf("%12.0f ", dAvg);		// Avg rate for event
				}
			}	
			*pFormattedStr += Str;
		}
	}
	
	// print the channel name
	Str.Printf("%s ", szChanShortName);			// channel name
	*pFormattedStr += Str;

	// SCR 414 pjm 14 Dec 2006
	//print the event ID 
	Str.Printf("%s ", pdbChanEvent->sz_event_id);//[14]
	*pFormattedStr += Str;
}


static void PrintChanEventToTextFile(struct IDStruct ChanID, struct db_event_rec *pdbChanEvent, char *szChanShortName, unsigned long ulNumPrinted, TextFile *pFile)
{
	CGFmtStr Str;
	
	if (pFile == NULL) return;

	FormatChanEvent(ChanID, pdbChanEvent, szChanShortName, ulNumPrinted, &Str);
	pFile->WriteLineToTextFile(Str);
}	
 
//  20-Aug-1999 SFK	  Version 2.05	Fixed columns so they line up correctly
static void PrintChanEventToTextWin(struct IDStruct ChanID, struct db_event_rec *pdbChanEvent, char *szChanShortName, unsigned long ulNumPrinted, CGUI_TextWin *pWin)
{                                                      
	CGFmtStr Str;
	
	if (pWin == NULL) return;

	FormatChanEvent(ChanID, pdbChanEvent, szChanShortName, ulNumPrinted, &Str);
	pWin->Printf("%s", Str.Get_sz());

	CGUI_App::ProcessEvents();						// update the output page
}	


	
///////////////////////////////////////////////////////////////////////////
//	Name:	WriteEventToDb
//
//	Description:
//	Fill in a channel event record for the db and write it
//
//	Declaration:
//	static int WriteEventToDb(short sChanNum, struct EVENT1 *pEvent, struct db_event_rec *pdbChanEvent)
//
//	Input:	sChanNum	db channel number associated with event
//			pEvent		results of analysis
//			
//	Output:	pdbChanEven	db channel record that was written
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	???			SFK		Created
//	04-Jan-2005	SFK		Modify to track number of points in event
//////////////////////////////////////////////////////////////////
static int WriteEventToDb(struct IDStruct ChanID, struct EVENT1 *pEvent, struct db_event_rec *pdbChanEvent)
{
	// fill in db record and record in database
	pdbChanEvent->us_event_chan = ChanID.sChanID;
	pdbChanEvent->us_event_sta = ChanID.sStaID;
	pdbChanEvent->us_event_sta_type = ChanID.sStaType;
	pdbChanEvent->d_event_beg_time_key = pEvent->dBegTime;
	pdbChanEvent->d_event_end_time = pEvent->dEndTime;
	pdbChanEvent->d_event_max_time = pEvent->dMaxTime;
	pdbChanEvent->d_event_grp_time = 0;
	pdbChanEvent->d_event_integral_sum = pEvent->dIntSum;	// change name in db
	pdbChanEvent->d_event_max_value = pEvent->dMaxValue;
	pdbChanEvent->d_event_avg_reals_rate = 0;
	pdbChanEvent->d_event_avg_totals_rate = 0;
	pdbChanEvent->us_event_type = pEvent->usType;
	pdbChanEvent->us_event_status = pEvent->usStatus;
	pdbChanEvent->us_event_type1 = 0;
	pdbChanEvent->us_event_status1 = 0;
	pdbChanEvent->us_event_type2 = 0;
	pdbChanEvent->us_event_status2 = 0;
	strcpy(pdbChanEvent->sz_event_id, "");
	unsigned long ulTotalPts;
	double dDum;
	GetDataStatistics(ChanID, true, pEvent->dBegTime, pEvent->dEndTime, &dDum, &dDum, &ulTotalPts);
	if (ulTotalPts > 0x7fff) ulTotalPts = 0x7fff;	//  can't handle very large runs
	pdbChanEvent->s_num_runs = (short)ulTotalPts;	//	04-Jan-2005 Keep track of number of runs in event

	return(CreateChanEventRec(ChanID, pdbChanEvent));
}
    

    
///////////////////////////////////////////////////////////////////////////
//	Name:	InitializeEventStruct
//
//	Description:
//	Set all the parameters in the record that we write to the db to initial values
//
//	Declaration:
//	static void InitializeEventStruct(struct EVENT1 *pEvent)
//
//	Input:	pEvent	address of event to be initialized
//			
//	Output:	pEvent	all event parameters initialized
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	???			SFK		Created
//////////////////////////////////////////////////////////////////
static void InitializeEventStruct(struct EVENT1 *pEvent)
{
	pEvent->bStarted = FALSE;
	pEvent->dBegTime = 0.0;
	pEvent->dEndTime = 0.0;
	pEvent->dMaxTime = 0.0;
	pEvent->dMaxValue = 0.0;
	pEvent->dIntSum = 0.0;
	pEvent->usType = -1;   
	pEvent->usStatus = -1;
    	
	pEvent->dRealsSum = 0.0;
	pEvent->dTotalsSum = 0.0;
}				



/*===========================================================================
 *
 *  Name	 :  SlidingWindowEventFind
 *
 *  Purpose	 :  Search the specified channels at the station for events.
 *
 *  Return value :  None
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  13-Dec-1995	SFK		Created 
 *	12-Aug-2002	SFK		Changed to DATE
 *  18-Aug-2004	SFK		Changed to double math, rather than float
 *	04-Jan-2005	SFK		Modify to track number of points in event
 *  06-Apr-2009 JCD		Modify header and summary output to go to log file as well as screen
===========================================================================*/
static void SlidingWindowEventFind(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, struct DATA_ANALYZE *pAnalyzeParms, BOOL bPrintAll, unsigned long ulTotalPts, unsigned long *pulTotalEvents)
{

	struct WHOLE_DOUBLE_DATA_PT1 *pdPts;		// all data for analysis is put in doubles in the wrappers
    //struct WHOLE_FLOAT_DATA_PT1 *pfPts;             
    struct EVENT1 Event[MAX_NUM_DATA_CHANS];
//    struct FLOAT_DATA_PT *pAvgPts = NULL;
//    struct FLOAT_DATA_PT *pVPts = NULL;
//    struct FLOAT_DATA_PT *pRmsPts = NULL;
//    struct FLOAT_DATA_PT *pSmthPts = NULL;
    struct WHOLE_DOUBLE_DATA_PT1 *pdAvgPts = NULL;
    struct WHOLE_DOUBLE_DATA_PT1 *pdVPts = NULL;
    struct WHOLE_DOUBLE_DATA_PT1 *pdRmsPts = NULL;
    struct WHOLE_DOUBLE_DATA_PT1 *pdSmthPts = NULL;
    struct db_sta_rec dbSta;
    struct db_event_rec dbChanEvent;
    
    int  status; 
    unsigned long i,j,k,m;
	int  i1, i2, i3, i4;              
                                
    unsigned long ulNumEvents = 0;                                
	DATE dNextStartTime;
    unsigned long ulPtsRead;		/* pts to read from db */
    unsigned long ulPtNum;
    DATE dFirstAnalyzedTime, dLastAnalyzedTime;
	long lNumPtsPrinted = 0;		/* number of pts output */
    unsigned long ulNumPtsLeft;
    unsigned long ulNumAllocPts; 				/* pts were able to allocate in memory */
    unsigned long ulPtsToRead;					/* pts to read from db */
    unsigned long ulPeakPts, ulHalfPeakPts, ulSearchPts, ulRmsPts;
    unsigned long ulPtsThisRead, ulPtsProcess;
    char szDate[DT_LEN+1], szTime[DT_LEN+1];
    char szDate2[DT_LEN+1], szTime2[DT_LEN+1];
    double dSensitivity, dTemp;
    double dAvgSum[MAX_NUM_DATA_CHANS];      
    double dVSum1[MAX_NUM_DATA_CHANS];
    double dVSum2[MAX_NUM_DATA_CHANS];
    double dVSum3[MAX_NUM_DATA_CHANS];
	CGFmtStr Msg;
    
    BOOL bFirstPass;

    
   /* ------------------------------------------------------------------
    *	Check parameter range.  Calculate search parameters needed.
    *	Check that there are enough points to analyze in the interval.
    * ----------------------------------------------------------------*/
    ulPeakPts = (unsigned long)pAnalyzeParms->sNumPeakPts;
    ulHalfPeakPts = (ulPeakPts-1)/2;
    ulSearchPts = ulPeakPts * 2;
    ulRmsPts = (unsigned long)pAnalyzeParms->sNumRmsPts;
    dSensitivity = pAnalyzeParms->dNumSigma;
    gllulOverlapPts = ulSearchPts + (unsigned long)(1.5 * (unsigned long)ulSearchPts);
    
	if ((ulTotalPts + 2*gllulOverlapPts) > NUM_FE_ALLOC_PTS) {
	    ulNumAllocPts = NUM_FE_ALLOC_PTS;
	}                 
	else {
		ulNumAllocPts = (int)ulTotalPts + 2*gllulOverlapPts;
	}	
    
    if (ulTotalPts <= (unsigned long)2*gllulOverlapPts) {
		RadReviewMsg(uiNOT_ENOUGH_DATA_TO_ANALYZE_WARN);
    	//GUI_MsgBox("Not enough data in interval to analyze.\n Please select larger time interval.   Event search will abort now.", GUI_ICON_INFO, GUI_OK);
    	if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n\nEvent search terminated due to not enough data to analyze.");
    	return;
    }	
    
	if (ulNumAllocPts < (11*ulPeakPts)) {
		RadReviewMsg(uiPEAK_WIDTH_TOO_LARGE_WARN);
    	if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n\nEvent search terminated due to large peak width.");
    	return;
    }	

    ulNumPtsLeft = ulTotalPts;             
    dNextStartTime = dStart;
    ulPtNum = 0;

   /* ------------------------------------------------------------------
    *	Initialize the event information for all channels
    * ----------------------------------------------------------------*/
	for (j=0; j<(MAX_NUM_DATA_CHANS-2); j++) {
    	InitializeEventStruct(&Event[j]);
    }	

   /* ------------------------------------------------------------------
    *	Allocate all the arrays that are needed in the processing
	* ----------------------------------------------------------------*/
//	pfPts = (struct WHOLE_FLOAT_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_FLOAT_DATA_PT1));
//	pVPts = (struct FLOAT_DATA_PT *)malloc((ulNumAllocPts+1) * sizeof (struct FLOAT_DATA_PT));       
//	pSmthPts = (struct FLOAT_DATA_PT *)malloc((ulNumAllocPts+1) * sizeof (struct FLOAT_DATA_PT));
//	pdAvgPts = (struct FLOAT_DATA_PT *)malloc((ulNumAllocPts+1) * sizeof (struct FLOAT_DATA_PT));
//	pRmsPts = (struct FLOAT_DATA_PT *)malloc((ulNumAllocPts+1) * sizeof (struct FLOAT_DATA_PT));
	pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
	pdVPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof (struct WHOLE_DOUBLE_DATA_PT1));       
	pdSmthPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof (struct WHOLE_DOUBLE_DATA_PT1));
	pdAvgPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof (struct WHOLE_DOUBLE_DATA_PT1));
	pdRmsPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof (struct WHOLE_DOUBLE_DATA_PT1));
	while (((pdPts == NULL) || (pdVPts == NULL) || (pdSmthPts == NULL) || (pdAvgPts == NULL) || (pdRmsPts == NULL)) && (ulNumAllocPts > 50)){
	    ulNumAllocPts = ulNumAllocPts - 50;
	    if (pdPts != NULL) /*free(pdPts)*/delete [] pdPts;
		pdPts = NULL;
		if (pdVPts != NULL) free(pdVPts);
		pdVPts = NULL;
		if (pdSmthPts != NULL) free(pdSmthPts);
		pdSmthPts = NULL;
		if (pdAvgPts != NULL) free(pdAvgPts);
		pdAvgPts = NULL;
		if (pdRmsPts != NULL) free(pdRmsPts);
		pdRmsPts = NULL;
		pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
		pdVPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));       
		pdSmthPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
		pdAvgPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
		pdRmsPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ulNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
	}   
	
	if (ulNumAllocPts == 0) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
    	if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n\nEvent search terminated due to memory allocation error.");
	    if (pdPts != NULL) /*free(pdPts)*/delete [] pdPts;
		pdPts = NULL;
		if (pdVPts != NULL) free(pdVPts);
		pdVPts = NULL;
		if (pdSmthPts != NULL) free(pdSmthPts);
		pdSmthPts = NULL;
		if (pdAvgPts != NULL) free(pdAvgPts);
		pdAvgPts = NULL;
		if (pdRmsPts != NULL) free(pdRmsPts);
		pdRmsPts = NULL;
    	return; 
    }	

	// figure out how many points to read	
	// HN 6-20-08 This was wrong, we actually want overlap on back and front.  Test for pts+2*overlap
	//if ((ulTotalPts + gllulOverlapPts) > (unsigned long)ulNumAllocPts) {	/* will have to read segments */
	if ((ulTotalPts + (2*gllulOverlapPts)) > (unsigned long)ulNumAllocPts) {	/* will have to read segments */
		ulPtsToRead = ulNumAllocPts - 2*gllulOverlapPts;
// sk		ulPtsProcess = ulNumAllocPts - gllulOverlapPts;
		ulPtsProcess = ulPtsToRead;
	}
	else {
		ulPtsToRead = (int)ulTotalPts;
		ulPtsProcess = (int)ulTotalPts + gllulOverlapPts;
	}		
									                             
   /* ------------------------------------------------------------------
	*   Print the station specific header
	* ----------------------------------------------------------------*/
	j = 0;
	if (!pglFacCfg->GetStationRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbSta)) return;			// read station record 

//	if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n\nStation = %s", dbSta.sz_sta_name);		//print station name
	// print station header to string, then to both log file and text window  JCD 4/6/2009
	CGFmtStr Str, StrTmp;
	Str.Printf("\n\nStation = %s\n\n", dbSta.sz_sta_name);
	if (pAnalyzeParms->pWin) 
		pAnalyzeParms->pWin->Printf(Str);		//print station name
	if (pAnalyzeParms->bLogFile) 
		pAnalyzeParms->pFile->WriteLineToTextFile(Str);
	//Don't try to execute this algorithm for EOSS data, just return
	if (dbSta.s_sta_type == EOSS_TYPE)
	{
		if (pAnalyzeParms->pWin != NULL) 
		{
			pAnalyzeParms->pWin->Printf("\nSliding window channel event algorithms not applicable to EOSS data.\n");
			pAnalyzeParms->pWin->Printf("______________________________________________________________________\n\n");
		}
		return;
	}
	Str.Printf("    Channels used            Threshold       Background");
	for (j = 0; j<MAX_NUM_DATA_CHANS; j++) 
	{  						// print channel names, thresholds
		if (pAnalyzeChans->sChanOffset[j] != -1) 
		{
			if (pAnalyzeParms->pWin)
			{
				char szBuf[20];
				if(pAnalyzeParms->sWhichAlg == SLIDING_WINDOW)
					sprintf(szBuf,"    %10.4f",pAnalyzeChans->fBkg[j]);
				else
					sprintf(szBuf,"NA");
				StrTmp.Printf("\n%25s  %10.4f  %s", gllChanInfo[j].szChanLongName, pAnalyzeChans->fThres[j], szBuf);
				Str += StrTmp;
			}
		}
	}
	if (pAnalyzeParms->pWin) 
		pAnalyzeParms->pWin->Printf(Str);		//print station name
	if (pAnalyzeParms->bLogFile) 
		pAnalyzeParms->pFile->WriteLineToTextFile(Str);

//	if (pAnalyzeParms->sWhichAlg == SLIDING_WINDOW) {
//		if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n\n    Channels used            Threshold  Background");
//		for (j = 0; j<MAX_NUM_DATA_CHANS; j++) {  						// print channel names, thresholds
//			if (pAnalyzeChans->sChanOffset[j] != -1) {

//				if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n%25s  %10.4f %10.4f", gllChanInfo[j].szChanLongName, pAnalyzeChans->fThres[j], pAnalyzeChans->fBkg[j]);
//			}
//		}		
//	}	
//	if (pAnalyzeParms->sWhichAlg == SLIDING_WINDOW_WITH_LOCAL_BKG) {
//		if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n\n    Channels used            Threshold");
//		for (j = 0; j<MAX_NUM_DATA_CHANS; j++) {  						// print channel names, thresholds
//			if (pAnalyzeChans->sChanOffset[j] != -1) {
//								if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n%25s  %10.4f", gllChanInfo[j].szChanLongName, pAnalyzeChans->fThres[j]);
//			}
//		}		
//	}	
	if (bPrintAll == FALSE) {
//		if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n\n         Start    End      Width Event        Maximum Integral Channel");
//		if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf(  "\nDate     Time     Time    (secs) Type         Rate    Sum      Name");
		                           /*90.05.07 10:10:01 10:20:25 12345 Missing Data 1234567 123456789 SW FC 1*/
	}
	else {	
		if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf("\n\n                  Chan     Pt  Sub               Smoothed        V  Event");
		if (pAnalyzeParms->pWin) pAnalyzeParms->pWin->Printf  ("\nDate     Time      Num    Num  Num       Data       Data       Data Status");
					                   /*90.05.07 10:10:01 1     123456 1800 1234567890 1234567890 1234567890*/
	}

	
	CGUI_App::ProcessEvents();                /* update the output page */
			             
   	//  Create the progress bar
	CGFmtStr Msg1, Msg2;
	TwoLineProgressBar *pTwoLineProgressBar;
	gllbContinue = true;
	BOOL bContinue = true;

	Msg1.Printf("Processing station %s.\n", dbSta.sz_sta_name);
	Msg2.Printf("Searched %ld out of %ld bytes", ulPtNum, ulTotalPts);
	pTwoLineProgressBar = new TwoLineProgressBar(ulTotalPts, ulTotalPts/20, Msg1, Msg2, "Radiation Review: Determine Measurement Progress");
	if (pTwoLineProgressBar) {

					
	   /* ------------------------------------------------------------------
		*	First time, must read in gllulOverlapPts extra points
		* ----------------------------------------------------------------*/
		if (ulNumPtsLeft > ulPtsToRead + gllulOverlapPts) {
			ulPtsThisRead = ulPtsToRead + gllulOverlapPts;
		}
		else {
			ulPtsThisRead = ulNumPtsLeft;
		}
		ulNumPtsLeft = ulNumPtsLeft - ulPtsThisRead;
			
			
	   /* ------------------------------------------------------------------
		*	Read the points from the db
		* ----------------------------------------------------------------*/   
//		if (dbSta.s_data_type == FLOAT_TYPE) {
//			status = GetDataByStation(pAnalyzeChans->StaID, dNextStartTime, dEnd, ulPtsThisRead, &ulPtsRead, &pfPts[gllulOverlapPts], &dNextStartTime);
//		}	
//		if (dbSta.s_data_type == DOUBLE_TYPE) {
//			status = GetDataByStation(pAnalyzeChans->StaID, dNextStartTime, dEnd, ulPtsThisRead, &ulPtsRead, &pfPts[gllulOverlapPts], &dNextStartTime);
//		}
		status = GetDoubleDataByStation(pAnalyzeChans->StaID, dNextStartTime, dEnd, ulPtsThisRead, &ulPtsRead, &pdPts[gllulOverlapPts], &dNextStartTime, TRUE);
		if (status == uiDAY_NOT_IN_DB_STATUS) status = TRUE;	// handle case of no data in this subinterval 
		if (status != TRUE) {							// fatal error in the data read
			bContinue = false;
			goto exit;
		}
		
	   /* ------------------------------------------------------------------
		*	Fill the first and last overlap points with values to get started.  
		* ----------------------------------------------------------------*/
		for (i=0; i<gllulOverlapPts; i++) {
			for (j=0; j<MAX_NUM_DATA_CHANS; j++) { 
				pdPts[i].dData[j] = pdPts[gllulOverlapPts].dData[j];
				pdSmthPts[i].dData[j] = pdPts[gllulOverlapPts].dData[j];
		    	pdVPts[i].dData[j] = pdPts[gllulOverlapPts].dData[j];
		    	pdAvgPts[i].dData[j] = pdPts[gllulOverlapPts].dData[j];
		    	pdRmsPts[i].dData[j] = 0.0;
		    }
		}

		// special case where only one buffer is read
		if ((ulPtsRead+gllulOverlapPts) < ulNumAllocPts) {
			for (i=ulPtsRead+gllulOverlapPts; i<ulNumAllocPts; i++) {
				for (j=0; j<MAX_NUM_DATA_CHANS; j++) { 
					pdPts[i].dData[j] = pdPts[ulPtsRead+gllulOverlapPts-1].dData[j];
					pdSmthPts[i].dData[j] = pdPts[ulPtsRead+gllulOverlapPts-1].dData[j];
	    			pdVPts[i].dData[j] = pdPts[ulPtsRead+gllulOverlapPts-1].dData[j];
	    			pdAvgPts[i].dData[j] = pdPts[ulPtsRead+gllulOverlapPts-1].dData[j];
	    			pdRmsPts[i].dData[j] = 0.0;
				}
			}
		}
				
	   /* ------------------------------------------------------------------
		*	Calculate the smoothed array
		* ----------------------------------------------------------------*/
		if (pAnalyzeParms->bSmooth == TRUE) {
		    for (i=gllulOverlapPts; i<ulNumAllocPts-1; i++) {   
		    	for (j=0; j<MAX_NUM_DATA_CHANS; j++) { 
		    		pdSmthPts[i].dData[j] = (pdPts[i-1].dData[j] + (2 * pdPts[i].dData[j]) + pdPts[i+1].dData[j])/4;
		    	}
		    }
		    for (j=0; j<MAX_NUM_DATA_CHANS; j++) { 
		    	pdSmthPts[ulNumAllocPts].dData[j] = pdSmthPts[ulNumAllocPts-1].dData[j];
		    }
		}
		else {
		    for (i=gllulOverlapPts; i<=ulNumAllocPts; i++) {   
		    	for (j=0; j<MAX_NUM_DATA_CHANS; j++) { 
		    		pdSmthPts[i].dData[j] = pdPts[i].dData[j];
		    	}
		    }
		}	
				
	   /* ------------------------------------------------------------------
		*	Zero out the running sum
		* ----------------------------------------------------------------*/
		for (j=0; j<MAX_NUM_DATA_CHANS; j++) {
		    dAvgSum[j] = 0.0;
		}	
				
	   /* ------------------------------------------------------------------
		*	Calculate the initial V array point 
		* ----------------------------------------------------------------*/
		for (j=0; j<MAX_NUM_DATA_CHANS; j++) {
		    dVSum1[j] = 0.0;
		    dVSum2[j] = 0.0;
		    dVSum3[j] = 0.0;
		}	
		i = gllulOverlapPts-1;
		for (k=(i-ulHalfPeakPts-ulPeakPts); k<=(i-ulHalfPeakPts-1); k++) {
		    for (j=0; j<MAX_NUM_DATA_CHANS; j++) {      
		    	dVSum1[j] += pdSmthPts[k].dData[j];
		    }
		}
				    	
		for (k=(i-ulHalfPeakPts); k<=(i+ulHalfPeakPts); k++) {
		    for (j=0; j<MAX_NUM_DATA_CHANS; j++) {
		    	dVSum2[j] += pdSmthPts[k].dData[j];
		    }
		}
				
		for (k=(i+ulHalfPeakPts+1); k<=(i+ulHalfPeakPts+ulPeakPts); k++) {
		    for (j=0; j<MAX_NUM_DATA_CHANS; j++) {
		    	dVSum3[j] += pdSmthPts[k].dData[j];
		    }
		}
				
		for (j=0; j<MAX_NUM_DATA_CHANS; j++) {
		    pdVPts[i].dData[j] = -dVSum1[j] + (2 * dVSum2[j]) - dVSum3[j];
		}
				
		bFirstPass = TRUE;		/* skip reading the first pass because already did it */
	   /* ------------------------------------------------------------------
		*	Process all points in the requested interval.
		* ----------------------------------------------------------------*/
		dFirstAnalyzedTime = pdPts[gllulOverlapPts].dJulianTime;
		do {
			if (glImportExit == TRUE) {
				gllbContinue = false;
				break;
			}
			if (bFirstPass == FALSE) {
						
									
			   /* ------------------------------------------------------------------
				*	Move the last 2*gllulOverlapPts points to the beginning of the arrays,
				*	will process from gllulOverlapPts to ProcessPts but need the others
				*	for the calculations
				* ----------------------------------------------------------------*/
				k = 0;
				for (i=ulPtsToRead; i<ulNumAllocPts; i++) {
		            pdPts[k] = pdPts[i];
		            pdVPts[k] = pdVPts[i];
				    pdSmthPts[k] = pdSmthPts[i];
	//		       	pdAvgPts[k] = pdAvgPts[i];
	//		       	pRmsPts[k] = pRmsPts[i];
				    k++;
				}   	 
						    
			   /* ------------------------------------------------------------------
				*	Figure out how many points to read from the database
				* ----------------------------------------------------------------*/
				if (ulNumPtsLeft > ulPtsToRead) {
					ulPtsThisRead = ulPtsToRead;
				}
				else {
					ulPtsThisRead = ulNumPtsLeft;
				}
				ulNumPtsLeft = ulNumPtsLeft - ulPtsThisRead;
				
				
			   /* ------------------------------------------------------------------
				*	Read the raw data points from the db
				* ----------------------------------------------------------------*/
				//if (dbSta.s_data_type == FLOAT_TYPE) {
				//	status = GetDataByStation(pAnalyzeChans->StaID, dNextStartTime, dEnd, ulPtsThisRead, &ulPtsRead, &(pfPts[2*gllulOverlapPts]), &dNextStartTime);
				//}	
				//if (dbSta.s_data_type == DOUBLE_TYPE) {
				//	status = GetDataByStation(pAnalyzeChans->StaID, dNextStartTime, dEnd, ulPtsThisRead, &ulPtsRead, (&pfPts[2*gllulOverlapPts]), &dNextStartTime);
				//}
				status = GetDoubleDataByStation(pAnalyzeChans->StaID, dNextStartTime, dEnd, ulPtsThisRead, &ulPtsRead, &(pdPts[2*gllulOverlapPts]), &dNextStartTime, TRUE);
				if (status == uiDAY_NOT_IN_DB_STATUS) status = TRUE;	// handle case of no data in this subinterval 
				if (status != TRUE) {							// fatal error in the data read
					bContinue = false;
					goto exit;
				}
				
				if (ulPtsRead < ulPtsProcess) ulPtsProcess = ulPtsRead + gllulOverlapPts;
						
			   /* ------------------------------------------------------------------
				*	If requested, calculate the smoothed array on the raw data
				* ----------------------------------------------------------------*/
				if (pAnalyzeParms->bSmooth == TRUE) {
				    for (i=gllulOverlapPts; i<ulNumAllocPts-1; i++) {   
				    	for (j=0; j<MAX_NUM_DATA_CHANS; j++) { 
				    		pdSmthPts[i].dData[j] = (pdPts[i-1].dData[j] + (2 * pdPts[i].dData[j]) + pdPts[i+1].dData[j])/4;
				    	}
				    }
				    for (j=0; j<MAX_NUM_DATA_CHANS; j++) { 
				    	pdSmthPts[ulNumAllocPts].dData[j] = pdSmthPts[ulNumAllocPts-1].dData[j];
				    }
				}
				else {
				    for (i=gllulOverlapPts; i<=ulNumAllocPts; i++) {   
				    	for (j=0; j<MAX_NUM_DATA_CHANS; j++) { 
				    		pdSmthPts[i].dData[j] = pdPts[i].dData[j];
				    	}
				    }
				}	
				
			}    
					    
		   /* ------------------------------------------------------------------
			*	Calculate the V array
			* ----------------------------------------------------------------*/
//sk			for (i=gllulOverlapPts; i<(ulPtsProcess + gllulOverlapPts/2); i++) {
			for (i=gllulOverlapPts; i<(ulPtsProcess + gllulOverlapPts); i++) {
			    i1 = i - 1 - ulHalfPeakPts - ulPeakPts;
			    i2 = i - 1 - ulHalfPeakPts;
			    i3 = i - 1 + ulHalfPeakPts + 1;
			    i4 = i - 1 + ulHalfPeakPts + ulPeakPts + 1;
					
			    for (j=0; j<MAX_NUM_DATA_CHANS; j++) {	
			    	dTemp = pdVPts[i-1].dData[j];
				    dTemp += pdSmthPts[i1].dData[j];
				    dTemp += -3*(pdSmthPts[i2].dData[j] - pdSmthPts[i3].dData[j]);
				    dTemp += -pdSmthPts[i4].dData[j];   
				    pdVPts[i].dData[j] = dTemp;
				}	
			}	
					
	
		   /* ------------------------------------------------------------------
			*	Populate the AvgV array
			* ----------------------------------------------------------------*/
	//		for (i=gllulOverlapPts; i<ulPtsProcess; i++) {
	//			for (j=0; j<NUM_DATA_CHANS; j++) {		    		
	//				fAvgSum[j] = 0.0;
	//			}	
	//			i1 = i - iRmsPts - ulPeakPts;
	//			i2 = i - ulPeakPts - 1;
	//			for (k=i1; k<=i2; k++) {        
	//				for (j=0; j<NUM_DATA_CHANS; j++) {
	//					fAvgSum[j] += pVPts[k].fData[j];
	//				}	
	//			}	     
	//			for (j=0; j<NUM_DATA_CHANS; j++) {		    		
	//		    	pdAvgPts[i].fData[j] = fAvgSum[j]/iRmsPts;
	//		    }	
	//	    }	
	
	
		   /* ------------------------------------------------------------------
			*	Populate the RMS2 array
			* ----------------------------------------------------------------*/
	//	    for (i=gllulOverlapPts; i<ulPtsProcess; i++) {      
	//	    	for (j=0; j<NUM_DATA_CHANS; j++) {
	//	    		fRmsSum[j] = 0.0;
	//	    	}
	//	    	i1 = i - iRmsPts - ulPeakPts; 	     
	//	    	i2 = i - ulPeakPts - 1;
	//	
	//	    	for (k=i1; k<=i2; k++) {
	//	    		for (j=0; j<NUM_DATA_CHANS; j++) {                                                                            
	//	    			fTerm = pVPts[k].fData[j] - pdAvgPts[k].fData[j];
	//	    			fRmsSum[j] +=  fTerm * fTerm;
	//	    		}
	//	    	} 
	//			    	
	//	    	for (j=0; j<NUM_DATA_CHANS; j++) { 
	//	    		dTemp = fRmsSum[j]/(iRmsPts-1);
	//	    		dTemp = sqrt(dTemp);
	//	    		pRmsPts[i].fData[j] = (float)dTemp;
	//	    	}
	//	    }		
	
		   /* ------------------------------------------------------------------
			*	Look for the peaks
			* ----------------------------------------------------------------*/
			double dPt, dVPt, dThres, dBkg;			    
			double dMax, dIntSum;
			BOOL bFirstNegativeLobe, bSecondNegativeLobe, bPreviousPeak;
			i = gllulOverlapPts;
			do {
			    ulPtNum++;
			    for (j=0; j<(MAX_NUM_DATA_CHANS-2); j++) {
					if (glImportExit == TRUE) {
						gllbContinue = false;
						break;
					}
					//if (gllCancelBtnHit == IDCANCEL) break;
			    	if ((pAnalyzeChans->sChanOffset[j]) != -1) {
						if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)){
							glMyDateTime.DATETimestampToDateTimeStrs(pdPts[i].dJulianTime, szDate, szTime);	
							pAnalyzeParms->pWin->Printf("\n%s %s %4d %6ld %4d %10.3f %10.3f %10.0f",
										 		szDate, szTime, j, ulPtNum, i, pdPts[i].dData[j],  pdSmthPts[i].dData[j], pdVPts[i].dData[j]); 
						}					
						// sliding window with background search
		                if (pAnalyzeChans->fBkg[j] > 0.0) {  // use background
		                    dVPt = pdVPts[i].dData[j];
		                    dPt =  pdPts[i].dData[j];
		                    dThres = pAnalyzeChans->fThres[j];
		                    dBkg = pAnalyzeChans->fBkg[j];

			    			if (Event[j].bStarted == FALSE) {	//Looking for a peak
			    				if (pdVPts[i].dData[j] >= pAnalyzeChans->fThres[j]) { // Found peak in the V's
									// Start of peak is the first data point above the background
									m = i;   
									if (pdPts[m].dData[j] >= pAnalyzeChans->fBkg[j]) {
			                            while ((pdPts[m].dData[j] >= pAnalyzeChans->fBkg[j]) && (m > 0)) {
											m--;
										}
										if (m != i) m++;	// go forward one because the last one is too far
									}	
									
				    				if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)) pAnalyzeParms->pWin->Printf(" Peak Start+%d", i-m);
										            
									dPt =  pdPts[m].dData[j];										                
									// found the start, set the event parameters
				    				Event[j].dBegTime = pdPts[m].dJulianTime;    
									Event[j].dMaxTime = pdPts[m].dJulianTime;			    					
				    				Event[j].dMaxValue = pdPts[m].dData[j];
									if (dbSta.s_sta_type != BINARY_TYPE){
				    					Event[j].dIntSum = pdPts[m].dData[j] * pdPts[m].dElapsedTime;
									}
									else {
										Event[j].dIntSum = pdPts[m].dData[j];
									}
				    				Event[j].usType = CHANNEL_EVENT;
				    				Event[j].dEndTime = 0;
				    				Event[j].usStatus = 0;
				    				// if backed up, get integral sum for everything up to i and look for new max value
				    				if (m < i) {
				    					for (k=m+1; k<=i; k++) {	// accumulate sum
											if (pdPts[k].dData[j] > Event[j].dMaxValue) {  // found a new max??
												Event[j].dMaxTime = pdPts[k].dJulianTime;
												Event[j].dMaxValue = pdPts[k].dData[j];
											}
											if (dbSta.s_sta_type != BINARY_TYPE){
				    							Event[j].dIntSum += pdPts[k].dData[j] * pdPts[k].dElapsedTime;
											}
											else {
												Event[j].dIntSum += pdPts[k].dData[j];
											}
											dIntSum = Event[j].dIntSum;					
										}
									}		
				    				Event[j].bStarted = TRUE;
				    				CGUI_App::ProcessEvents();	    			                
				    			}	
			    			}
			    			else { // in the event, check if still in or at end
			    				if ((pdPts[i].dData[j] >= pAnalyzeChans->fBkg[j]) || (pdVPts[i].dData[j] >= pAnalyzeChans->fThres[j])) { // still in the event
									if (dbSta.s_sta_type != BINARY_TYPE){
				    					Event[j].dIntSum += pdPts[i].dData[j] * pdPts[i].dElapsedTime;	// accumulate sum
									}
									else {
										Event[j].dIntSum += pdPts[i].dData[j];	// accumulate sum
									}
				    				
				    				dIntSum = Event[j].dIntSum;
				    				if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)) pAnalyzeParms->pWin->Printf(" Above V");  // above thres, but not 1st pt
									if (pdPts[i].dData[j] > Event[j].dMaxValue) {  // found a new max??
										Event[j].dMaxTime = pdPts[i].dJulianTime;
										Event[j].dMaxValue = pdPts[i].dData[j];
									}
								}
								else { // point is below the background and the V threshold, last point was the end
									if (i > 0) {
										Event[j].dEndTime = pdPts[i-1].dJulianTime;
										dPt =  pdPts[i-1].dData[j];
									}	
									else {
										Event[j].dEndTime = pdPts[i].dJulianTime;
										dPt =  pdPts[i].dData[j];
									}    
									status = WriteEventToDb(gllChanInfo[j].ChanID, &Event[j], &dbChanEvent);
									PrintChanEventResults(gllChanInfo[j].ChanID, &dbChanEvent, gllChanInfo[j].szChanShortName, &ulNumEvents,
													pulTotalEvents, pAnalyzeParms->pWin, pAnalyzeParms->pFile, 
										            pAnalyzeParms->bTextWin, pAnalyzeParms->bLogFile, pAnalyzeParms->bDB_File);

				    				if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)) pAnalyzeParms->pWin->Printf(" End + 1");
				    				InitializeEventStruct(&Event[j]);
				    			}	
			    			}	    			                
		                }
		                else { // do not use background, look for point above V threshold
							// sliding window with local background, peak is one that has
							// a V value greater than threshold, endpoints are V values above zero.
	                
				    		if (pdVPts[i].dData[j] >= pAnalyzeChans->fThres[j]) {
									
				    			if (Event[j].bStarted == FALSE) {
				    				// Look for the negative lobes both before and after this positive lobe
				    				bFirstNegativeLobe = FALSE;
				    				bSecondNegativeLobe = FALSE;
				    				bPreviousPeak = FALSE;
									i1 = i - ulPeakPts;   
									if (i1 < 0) i1 = 0;
									i2 = i + 2*ulPeakPts;
									if (i2 > (int)ulNumAllocPts) i2 = (int)ulNumAllocPts;  //compiler warning fix
									i3 = i - 2*ulPeakPts;
									if (i3 < 0) i3 = 0;
									for (m=i1; m <=i; m++) { 
										if (pdVPts[m].dData[j] < - (pAnalyzeChans->fThres[j]/2)) {
										//if (pdVPts[m].dData[j] < - 2000.0) {											
											bFirstNegativeLobe = TRUE;
											break;
										}	
									}	
									for (m=i; m <=(ULONG)i2; m++) //compiler warning fix
									{ 
										if (pdVPts[m].dData[j] < - (pAnalyzeChans->fThres[j]/2)) {										
											bSecondNegativeLobe = TRUE;
											break;
										}	
									}	
								
									for (m=i3; m <i; m++) { 
										if (pdVPts[m].dData[j] > pAnalyzeChans->fThres[j]) {										
											bPreviousPeak = TRUE;
											break;
										}	
									}	
								
									if ((bFirstNegativeLobe == TRUE) && (bSecondNegativeLobe == TRUE)  && (bPreviousPeak == FALSE)) {										
					    				
					    				/* this is about the start of the peak,  eventually look for lzero here? */
					    				if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)) pAnalyzeParms->pWin->Printf(" Start");
										
										/* start of event is determined as the first non-negative V point proceeding this value  */
										m = i;   
										// look for the 0 crossing in the V's
										if (pdVPts[m].dData[j] > 0.0) {
											while ((pdVPts[m].dData[j] > 0.0) && (m > 0)) {
												m--;
											}
											if (m != i) m++;	// go forward one because went back one too far
										}	
										
										// set the event parameters
					    				Event[j].dBegTime = pdPts[m].dJulianTime;    
										Event[j].dMaxTime = pdPts[m].dJulianTime;			    					
					    				Event[j].dMaxValue = pdPts[m].dData[j];
										if (dbSta.s_sta_type != BINARY_TYPE){
					    					Event[j].dIntSum = pdPts[m].dData[j] * pdPts[m].dElapsedTime;
										}
										else {
											Event[j].dIntSum = pdPts[m].dData[j];
										}

					    				
					    				Event[j].usType = CHANNEL_EVENT;
					    				// if backed up, process everything up to i
					    				dMax = Event[j].dMaxValue;
					    				if (m < i) {
					    					for (k=m+1; k<=i; k++) {	// accumulate sum
												if (pdPts[k].dData[j] > Event[j].dMaxValue) {  // found a new max??
													Event[j].dMaxTime = pdPts[k].dJulianTime;
													Event[j].dMaxValue = pdPts[k].dData[j];
												}
												if (dbSta.s_sta_type != BINARY_TYPE){
							    					Event[j].dIntSum += pdPts[k].dData[j] * pdPts[k].dElapsedTime;
												}
												else {
													Event[j].dIntSum += pdPts[k].dData[j];
												}
																	
											}
										}		
					    				Event[j].bStarted = TRUE;
					    				CGUI_App::ProcessEvents();	    			                
					    			}	
				    			}
				    			else {
									if (dbSta.s_sta_type != BINARY_TYPE){
				    					Event[j].dIntSum += pdPts[i].dData[j] * pdPts[i].dElapsedTime;	// accumulate sum
									}
									else {
										Event[j].dIntSum += pdPts[i].dData[j];	// accumulate sum
									}

				    				if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)) pAnalyzeParms->pWin->Printf(" Above V");  // above thres, but not 1st pt
									if (pdPts[i].dData[j] > Event[j].dMaxValue) {  // found a new max??
										Event[j].dMaxTime = pdPts[i].dJulianTime;
										Event[j].dMaxValue = pdPts[i].dData[j];
									}					
				    			}
				    		}
				    		else {	// point is below V threshold so now look for the zero crossing
				    			if (Event[j].bStarted == TRUE) {	// this is the end of the event
				    			
									m = i;   
									if (pdVPts[m].dData[j] > 0.0) {
										while ((pdVPts[m].dData[j] > 0.0) && (m < ulPtsProcess)) {
											m++;
										} 
										if (m != i) m--; // went ahead one too far
									}
									else m--; // last one was above zero
									Event[j].dEndTime = pdPts[m].dJulianTime;
									
				    				// if went ahead, process everything after the i up to where you are now
				    				if (m >= i) {
				    					for (k=i; k<=m; k++) {	// accumulate sum
											if (pdPts[k].dData[j] > Event[j].dMaxValue) {  // found a new max??
												Event[j].dMaxTime = pdPts[k].dJulianTime;
												Event[j].dMaxValue = pdPts[k].dData[j];
												dMax = Event[j].dMaxValue; 
												dPt = pdPts[k].dData[j]; 
											}
											if (dbSta.s_sta_type != BINARY_TYPE){
					    						Event[j].dIntSum += pdPts[k].dData[j] * pdPts[k].dElapsedTime;
											}
											else {
												Event[j].dIntSum += pdPts[k].dData[j];
											}
										}
									}		
									if (m > i) i = m;	// move past this event to look for others
									status = WriteEventToDb(gllChanInfo[j].ChanID, &Event[j], &dbChanEvent);
									PrintChanEventResults(gllChanInfo[j].ChanID, &dbChanEvent, gllChanInfo[j].szChanShortName, &ulNumEvents,
														pulTotalEvents, pAnalyzeParms->pWin, pAnalyzeParms->pFile, 
											            pAnalyzeParms->bTextWin, pAnalyzeParms->bLogFile, pAnalyzeParms->bDB_File);

									if ((bPrintAll == TRUE) && (pAnalyzeParms->pWin)){
										if (m == i) {
											pAnalyzeParms->pWin->Printf(" End");
										}
										else {
											pAnalyzeParms->pWin->Printf(" End + 1");
										}

									}
				    				InitializeEventStruct(&Event[j]);
				    			}	    			                
				    		}	
				    	}	
		                
					}	
								
				}	
				
			//	Periodically update the progress bar
				Msg2.Printf("Searched %ld out of %ld bytes", ulPtNum, ulTotalPts);
				gllbContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
				if (glImportExit == TRUE) gllbContinue = false;
				i++;
			//HN 6-19-08 This loop variable was wrong, you must go until ulPtsProcess + gllulOverlapPts to get all the points for analysis.
			//Some events were missing.  Fix!!
			//} while ((i<ulPtsProcess) && gllbContinue);
			} while (i<(ulPtsProcess + gllulOverlapPts) && gllbContinue);                      
			bFirstPass = FALSE;			/* have completed at least one pass */
			
		} while ((ulNumPtsLeft > 0)  && gllbContinue);
		
		pTwoLineProgressBar->Close();
		delete pTwoLineProgressBar;

		dLastAnalyzedTime = pdPts[ulPtsProcess-1].dJulianTime;  
		
		   
		// print summary info
/*		if (pAnalyzeParms->pWin) {
			if (gllbContinue == false) pAnalyzeParms->pWin->Printf("\nSearch canceled by user.");
			else pAnalyzeParms->pWin->Printf("\nSearch completed successfully.");
			glMyDateTime.DATETimestampToDateTimeStrs(dFirstAnalyzedTime, szDate, szTime);
			pAnalyzeParms->pWin->Printf("\nData analyzed from %s %s to ", szDate, szTime);
			glMyDateTime.DATETimestampToDateTimeStrs(dLastAnalyzedTime, szDate, szTime);
			pAnalyzeParms->pWin->Printf("%s %s", szDate, szTime);
			pAnalyzeParms->pWin->Printf("    (%ld data points)\n", (ulTotalPts - ulNumPtsLeft));
			pAnalyzeParms->pWin->Printf("Total Channel Events Found for Station %s = %ld\n\n", dbSta.sz_sta_name, ulNumEvents); 
			pAnalyzeParms->pWin->Printf("______________________________________________________________________\n\n");
		}*/
		// print summary info for the station to string, then to both log file and text window   JCD 4/6/2009
		if (!gllbContinue) 
			Str.Printf("\nSearch canceled by user.");
		else
		{
			glMyDateTime.DATETimestampToDateTimeStrs(dFirstAnalyzedTime, szDate, szTime);
			glMyDateTime.DATETimestampToDateTimeStrs(dLastAnalyzedTime, szDate2, szTime2);
			Str.Printf("\nSearch completed successfully.\nData analyzed from %s %s to %s %s    (%ld data points)\nTotal Channel Events Found for Station %s = %ld\n\n______________________________________________________________________\n\n" \
				, szDate, szTime, szDate2, szTime2,ulTotalPts - ulNumPtsLeft,dbSta.sz_sta_name, ulNumEvents);
		}
		if (pAnalyzeParms->pWin) 
			pAnalyzeParms->pWin->Printf(Str);
		if (pAnalyzeParms->bLogFile) 
			pAnalyzeParms->pFile->WriteLineToTextFile(Str);
	}
	
exit:                   		    
	if (pdPts != NULL) /*free(pdPts);*/delete [] pdPts;
	pdPts = NULL;
	if (pdVPts != NULL) free(pdVPts);
	pdVPts = NULL;
	if (pdSmthPts != NULL) free(pdSmthPts);
	pdSmthPts = NULL;
	if (pdAvgPts != NULL) free(pdAvgPts);
	pdAvgPts = NULL;
	if (pdRmsPts != NULL) free(pdRmsPts);
	pdRmsPts = NULL;
    return;
}

