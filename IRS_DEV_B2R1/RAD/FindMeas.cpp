///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : FindMeas.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		PrintMeasEventToTextWin
//		FindMeasurements

//	Static Functions
//		PrintStationParameters
//		SetSRStatusByte
//		WriteMeasToDb
//		FindMeasurementType
//		WriteGRANDMIIToDb
//		FindGRANDMeasurementType
//		is_event_type
//		is_moving
//		determine_meas_type
//		determine_run_type
//		AnalyzeAndSetSRStatusByte
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include <math.h>
#include "DbEvent.h"
#include "DbInterval.h"
#include "DbReadStruct.h"
#include "DlgFindMeas.h"
#include "EventTypeLists.h"
#include "FindMeas.h"
#include "ProgressBars.h"
#include "RADInit.h"
#include "Utilities.h"
#include "FacilityConfig.h" //KM
#include "MyDateTime.h"
#include "TypeDefinitions.h"

extern short glsFacNum; //KM
extern RADInit *pRAD_Init; //	11-Jul-2005 SFK Added
extern CMyDateTime glMyDateTime;

#define NUM_MEAS_ALLOC_PTS 1800

#define AT_BIT			0x00010000L
#define LOW_BKG_BIT		0x00020000L
#define HIGH_BKG_BIT	0x00040000L
#define EMPTY_BIT		0x00080000L
#define ASSAY_BIT		0x00100000L
#define CF_CHECK_BIT	0x00200000L
#define CF_NORM_BIT		0x00400000L
#define SHORT_BIT		0x00800000L
#define LONG_BIT		0x01000000L
#define UNUSUAL_BIT		0x02000000L
#define FIRST_RUN_BIT	0x04000000L
#define TOTALS_BIT      0x08000000L
#define MEAS_TYPES_BITS 0x087f0000L

#define MII_BIT 0x0001
                                      
extern BOOL glImportExit;
extern char *glnaszDirectionList[];
extern char *glSRAlgorithmsPicklist[];

static struct db_sta_dflt_rec glldb_StaDfltRec;
void PrintStationParameters(CGUI_TextWin *pWin);                    
static int AnalyzeAndSetSRStatusByte(struct WHOLE_DOUBLE_DATA_PT1 *pdPt);
static BOOL SetSRStatusByte(struct IDStruct StaID, DATE dStart, DATE dEnd, unsigned long ulTotalPts, BOOL bDiagPrint, CGUI_TextWin *pWin);
static int WriteMeasToDb(struct IDStruct ChanID, struct EVENT *pEvent, struct db_event_rec *pdbEvent);
static BOOL FindMeasurementType(struct IDStruct StaID, DATE dStart, unsigned long ulTotalPts, BOOL bDiagPrint, CGUI_TextWin *pWin, DATE dEnd);
static int WriteGRANDMIIToDb(struct IDStruct ChanID, struct EVENT *pEvent, struct db_event_rec *pdbEvent);
static BOOL FindGRANDMeasurementType(struct IDStruct StaID, DATE dStart, unsigned long ulTotalPts, BOOL bDiagPrint, CGUI_TextWin *pWin, DATE dEnd);
static BOOL is_event_type(int meas_type);
static BOOL is_moving(double dTotalsRate, double dAccidentalsRate, double dAccidentals);
static int determine_meas_type(int run_type, int prior_meas_type);
static int determine_run_type(double dAccidentals, double dTotalsRate, double dRealsRate, double dAccidentalsRate);
static int AnalyzeAndSetSRStatusByte(struct WHOLE_DOUBLE_DATA_PT1 *pdPt);



//  20-Aug-1999 SFK		Version 2.05 Added printing of maximum value and time for use in direction search
//										Fixed columns so they line up correctly
//	15-Dec-2004	SFK		Added getting event strings from a common list via GetEventTypeStr;
//	11-Jul-2005 SFK		Modified format to be more similar to channel event and added R/T
void PrintMeasEventToTextWin(struct db_event_rec *pdbChanEvent, CGUI_TextWin *pWin, unsigned long *pulNumEvents)
{                                                      

	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	char szID[ID_LEN+1];

	// don't print a one point unknown measurement
	if (((pdbChanEvent->d_event_end_time - pdbChanEvent->d_event_beg_time_key) == 0) &&
		 (pdbChanEvent->us_event_type1 == UNKNOWN_MEAS)) return;
	
	(*pulNumEvents)++;

	// start date and time
	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_beg_time_key, szDate, szTime);
	if (pWin != NULL) pWin->Printf("\n%s %s ", szDate, szTime);		// start time

	// only end time
	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_end_time, szDate, szTime);
	if (pWin != NULL) pWin->Printf("%s ", szTime);  				// end time only
	
	// sample id
	MyStrnCpyAndPad(szID, pdbChanEvent->sz_event_id, 13, ' ');					
	if (pWin != NULL)pWin->Printf("%s", szID);
	if (pWin != NULL) pWin->Printf("%5.0lf ", (pdbChanEvent->d_event_end_time - pdbChanEvent->d_event_beg_time_key)*86400.0);	// width

	// event type
	char szEventTypeStr[MAX_LINE_LEN+1];
	GetEventTypeStr(pdbChanEvent, szEventTypeStr);
	if (pWin != NULL) pWin->Printf("%s", szEventTypeStr);	// 15-Dec-2004 SFK Added getting Event Str from GetEventTypeStr

	
	// print the maximum rate and time
	if (pdbChanEvent->d_event_max_value > 10) {	// large positive value
		if (pWin != NULL) pWin->Printf("%9.1f ", pdbChanEvent->d_event_max_value);	// maximum value   
	}
	else {	// small positive value	
		if (pdbChanEvent->d_event_max_value > 0) {	
			if (pWin != NULL) pWin->Printf("%9.5f ", pdbChanEvent->d_event_max_value);	// maximum value   	
		}
		else { // negative value
			if (pWin != NULL) pWin->Printf("%9.1f ", pdbChanEvent->d_event_max_value);	// maximum value   	
		}
	}	
	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_max_time, szDate, szTime);
	if (pWin != NULL) pWin->Printf("%s ", szTime);  		// max time only

	// average totals rate
	if (pWin != NULL) pWin->Printf("%12.2lf ", pdbChanEvent->d_event_avg_totals_rate);	// Average totals rate

	// average reals rate
	if (pWin != NULL) pWin->Printf("%12.2lf ", pdbChanEvent->d_event_avg_reals_rate);	// Average reals rate

	// Try to determine the direction for this event and print it.
	BOOL bMatchingDirEvent;
	struct db_dir_event_rec dbDirEvent;
	unsigned short usDir;
	int status;
	status = FindDirEventAssociatedWithChanEvent(pdbChanEvent, &bMatchingDirEvent, &dbDirEvent);
	if (bMatchingDirEvent == TRUE) {
		usDir = dbDirEvent.us_dir_event_direction;
	}
	else {
		usDir = NO_DIRECTION;
	}
	if (pWin != NULL) pWin->Printf("%s", glnaszDirectionList[usDir]);

	// print R/T
	if (pdbChanEvent->d_event_avg_totals_rate > 0) {
		if (pWin != NULL) pWin->Printf("%10.3e ", pdbChanEvent->d_event_avg_reals_rate/pdbChanEvent->d_event_avg_totals_rate);	// Average reals rate/ average totals rate
	}
	else {
		if (pWin != NULL) pWin->Printf("%10.3e ", pdbChanEvent->d_event_avg_reals_rate/1.0);	// don't divide by 0
	}

	CGUI_App::ProcessEvents();						// update the output page
}	


static void PrintStationParameters(CGUI_TextWin *pWin)
{                                                      
	if (pWin != NULL) {
		pWin->Printf("\nAnalysis Type = %s", glSRAlgorithmsPicklist[glldb_StaDfltRec.s_meas_analysis_type]);
		pWin->Printf("\nLow background threshold (totals/time) = %f ", glldb_StaDfltRec.d_meas_low_bkg_thres); 
		pWin->Printf("\nHigh background threshold (totals/time) = %f ", glldb_StaDfltRec.d_meas_high_bkg_thres); 
		pWin->Printf("\nEmpty container threshold (reals/time) = %f ", glldb_StaDfltRec.d_meas_empty_thres); 
		pWin->Printf("\nReals [doubles] assay threshold (reals/time) = %f ", glldb_StaDfltRec.d_meas_reals_thres); 
		pWin->Printf("\nTotals [singles] assay threshold (totals/time) = %f", glldb_StaDfltRec.d_meas_totals_thres); 
		pWin->Printf("\nCf/totals ratio threshold (reals/totals) = %f ", glldb_StaDfltRec.d_meas_cf_thres); 
		pWin->Printf("\nCf normalization threshold (reals/time) = %f ", glldb_StaDfltRec.d_meas_cf_norm_thres); 
		pWin->Printf("\nMinimum measurement length (minutes) = %3.0f ", glldb_StaDfltRec.d_meas_min_len); 
		pWin->Printf("\nMaximum measurement length (minutes) = %3.0f ", glldb_StaDfltRec.d_meas_max_len); 
		pWin->Printf("\nTime to add before calculated start time(minutes) = %3.0f ", glldb_StaDfltRec.d_meas_add_min); //SCR00191 21-Dec-2005 SFK Added
		pWin->Printf("\nPerform low background check =  ");
		if (glldb_StaDfltRec.b_meas_low_bkg_check == TRUE) pWin->Printf("Yes");
		else pWin->Printf("No");
		pWin->Printf("\nPerform high background check =  ");
		if (glldb_StaDfltRec.d_meas_high_bkg_thres == TRUE) pWin->Printf("Yes");
		else pWin->Printf("No");
	}
	CGUI_App::ProcessEvents();						// update the output page
}	


// changed to DATE
static BOOL SetSRStatusByte(struct IDStruct StaID, DATE dStart, DATE dEnd, unsigned long ulTotalPts, BOOL bDiagPrint, CGUI_TextWin *pWin)
{

	struct WHOLE_DOUBLE_DATA_PT1 *pdPts;

	int iNumAllocPts, iPtsToRead;
	unsigned long ulPtNum, ulNumPtsLeft, ulPtsRead;
	DATE dThisStartTime, dNextStartTime, dNextEndTime;
	int status;
	int i;

	//	Read analysis parameters from the database
	glldb_StaDfltRec.s_sta_dflt_num_key = StaID.sStaID;
	status = GetSRMeasurementParameters(&glldb_StaDfltRec);
	if (!status) return(false);


    ulNumPtsLeft = ulTotalPts;             
    dNextStartTime = dStart;
    ulPtNum = 0;

   /* ------------------------------------------------------------------
    *	Figure out how many points you need to allocate storage for --
    *	works best if only read and analyze about a 1000 pts at a time;
    *	there's always enough memory for that allocation.
    *	
	* ----------------------------------------------------------------*/
	if (ulTotalPts > NUM_MEAS_ALLOC_PTS) {
	    iNumAllocPts = NUM_MEAS_ALLOC_PTS;
	}                 
	else {
		iNumAllocPts = (int)ulTotalPts;
	}	
	iPtsToRead = iNumAllocPts;
	
   /* ------------------------------------------------------------------
    *	Allocate the array needed in the processing
	* ----------------------------------------------------------------*/
	pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((iNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
	while ((pdPts == NULL) && (iNumAllocPts > 50)){
	    iNumAllocPts = iNumAllocPts - 50;
		pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((iNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
	}   
	if (iNumAllocPts <= 0) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
    	if (pWin != NULL) pWin->Printf("\n\nNot enough memory; measurement search terminated.");
    	return(false); 
    }	
	
   /* ------------------------------------------------------------------
	*   Create the dialog box that is used to show analysis progress for this station
	* ----------------------------------------------------------------*/
	CGFmtStr Msg1, Msg2;
	TwoLineProgressBar *pTwoLineProgressBar;
	BOOL bContinue;

	char szStaName[DB_NAME_LEN+1];
	pglFacCfg->GetStationLongName(glsFacNum, &StaID, szStaName, DB_NAME_LEN);
	Msg1.Printf("Processing station %s.\n", szStaName);  //KM
	Msg2.Printf("Searched %ld out of %ld bytes", ulPtNum, ulTotalPts);
	pTwoLineProgressBar = new TwoLineProgressBar(ulTotalPts, ulTotalPts/20, Msg1, Msg2, "Radiation Review: Determine Measurement Progress");
	if (pTwoLineProgressBar) {

		do {
			ulPtNum++;
						    
		   /* ------------------------------------------------------------------
			*	Figure out how many points to read and process
			*	If necessary, read one point past the end of this segment to determine
			*	where to start the next read.
			* ----------------------------------------------------------------*/
			if (ulNumPtsLeft > (unsigned long)iNumAllocPts) {
				iPtsToRead = iNumAllocPts;
			}
			else {
				iPtsToRead = (int)ulNumPtsLeft;
			}
			ulNumPtsLeft = ulNumPtsLeft - iPtsToRead;
									
		   /* ------------------------------------------------------------------
			*	Read the points from the db
			* ----------------------------------------------------------------*/   
			status = GetDoubleDataByStation(StaID, dNextStartTime, dEnd, (unsigned long)iPtsToRead, &ulPtsRead, pdPts, &dNextEndTime, FALSE);  
			if (status == uiDAY_NOT_IN_DB_STATUS) status = TRUE;  // handle case of no data in this subinterval 
			if (status != TRUE) break;
			dThisStartTime = dNextStartTime;
			dNextStartTime = dNextEndTime;

		   /* ------------------------------------------------------------------
			*	Analyze the points read and update their status byte.
			* ----------------------------------------------------------------*/   
			i = 0;
			do {
				ulPtNum++;

				AnalyzeAndSetSRStatusByte(&pdPts[i]);

			   /* ------------------------------------------------------------------
				*	Periodically update the progress bar
				* ----------------------------------------------------------------*/
				Msg2.Printf("Searched %ld out of %ld bytes", ulPtNum, ulTotalPts);
				bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
				i++;
			} while ((i<(int)ulPtsRead) && bContinue);

		   /* ------------------------------------------------------------------
			*	Write the record with the updated status bytes back into the database
			* ----------------------------------------------------------------*/   
			status = WriteDbDataStatus(StaID, dThisStartTime, (unsigned long)iPtsToRead, pdPts);
			if (glImportExit == TRUE) bContinue = false;
					
		} while ((ulNumPtsLeft > 0)  && bContinue);
		
		pTwoLineProgressBar->Close();
		delete pTwoLineProgressBar;
	}
	
	if (pdPts != NULL) free(pdPts);
	pdPts = NULL;
    return(bContinue);

}
	
	
// Aug 2002 patch until get FacConfig upgrade.  Assume Totals are always in channel 1 and Reals in
//	channel 2
static int WriteMeasToDb(struct IDStruct ChanID, struct EVENT *pEvent, struct db_event_rec *pdbEvent)
{
	
	// fill in db record and record in database
	if (pEvent->usMeasType == TOTALS_MEAS) {
		pdbEvent->us_event_chan = TOTALS_OFFSET+1;
	}
	else {
		pdbEvent->us_event_chan = REALS_OFFSET+1;
	}
	pdbEvent->us_event_sta = ChanID.sStaID;
	pdbEvent->us_event_sta_type = ChanID.sStaType;
	pdbEvent->d_event_beg_time_key = pEvent->dBegTime;
	pdbEvent->d_event_end_time = pEvent->dEndTime;
	pdbEvent->d_event_max_time = pEvent->dMaxTime;
	pdbEvent->d_event_grp_time = 0;
	pdbEvent->d_event_integral_sum = pEvent->dIntSum;	
	pdbEvent->d_event_max_value = pEvent->dMaxValue;
	if (pEvent->dSumTime != 0) {
		pdbEvent->d_event_avg_reals_rate = pEvent->dRealsSum/pEvent->dSumTime;
		pdbEvent->d_event_avg_totals_rate = pEvent->dTotalsSum/pEvent->dSumTime;
	}
	else {
		pdbEvent->d_event_avg_reals_rate = 0;
		pdbEvent->d_event_avg_totals_rate = 0;
	}
	pdbEvent->us_event_type = pEvent->usType;
	pdbEvent->us_event_status = 0;
	pdbEvent->us_event_type1 = pEvent->usMeasType;
	pdbEvent->us_event_status1 = pEvent->usStatus;
	pdbEvent->us_event_type2 = 0;
	pdbEvent->us_event_status2 = 0;
	if (((pdbEvent->us_event_type == MEASUREMENT_EVENT) && (pdbEvent->us_event_type1 == ASSAY_MEAS)) ||
	    ((pdbEvent->us_event_type == MEASUREMENT_EVENT) && (pdbEvent->us_event_type1 == EMPTY_MEAS)) ||
		((pdbEvent->us_event_type == MEASUREMENT_EVENT) && (pdbEvent->us_event_type1 == TOTALS_MEAS))) {
		strcpy(pdbEvent->sz_event_id, "Enter ID");
	}
	else {
		strcpy(pdbEvent->sz_event_id, "None");
	}
	pdbEvent->s_num_runs = pEvent->usNumPts;

	// don't write a one point unknown measurement
	if (((pEvent->dEndTime - pEvent->dBegTime) == 0) && (pEvent->usMeasType == UNKNOWN_MEAS)) return(TRUE);
	
	int iStatus = CreateChanEventRec(ChanID, pdbEvent);
	struct db_event_rec dbTempEvent;
	if (iStatus) ReadEventRec(pdbEvent, &dbTempEvent);
	return(iStatus);
}
    


/*===========================================================================
 *
 *  Name	 :  FindMeasurementType
 *
 *  Purpose	 :  Search the status bytes looking for the beginning and ending
 *				of events.  Fill and write event records for each measurement
 *				found.
 *
 *  Return value :  None
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  12-Feb-1998	SFK		Created 
 *  20-Aug-1999 SFK		Version 2.05 Fixed finding the maximum rate value in a measurement
									   Also fix finding proper rate depending on Analysis type
 *	21-Dec-2005	SFK		Before write measurement to database, subtract the user-specified number
 *						of minutes from the measurement beginning time.
===========================================================================*/
static BOOL FindMeasurementType(struct IDStruct StaID, DATE dStart, unsigned long ulTotalPts, BOOL bDiagPrint, CGUI_TextWin *pWin, DATE dEnd)
{
               
    struct WHOLE_DOUBLE_DATA_PT1 *pdPts;             
    struct EVENT Event;
    struct db_sta_rec dbSta;
    struct db_event_rec dbChanEvent;
	struct db_sta_dflt_rec dbStaDflt;

    int  status; 
    int  i;
    DATE dNextStartTime, dNextEndTime;  
    unsigned long ulPtsRead;		/* pts to read from db */
    unsigned long ulPtNum;
    DATE dFirstAnalyzedTime, dLastAnalyzedTime;
    unsigned long ulNumPtsLeft;
    unsigned long ulNumEvents = 0;
    int iNumAllocPts; 				/* pts were able to allocate in memory */
    int iPtsToRead;					/* pts to read from db */
    char szDate[DT_LEN+1], szTime[DT_LEN+1];
    BOOL bFirstPass = TRUE;
	unsigned long ulMeasType;

	Event.bStarted = FALSE;
	Event.dBegTime = 0;
	Event.dEndTime = 0;
	Event.dMaxValue = 0.0;
	Event.dIntSum = 0.0;
	Event.usType = -1;   
	Event.usStatus = -1;
    	
	Event.dRealsSum = 0.0;
	Event.dTotalsSum = 0.0;

    ulNumPtsLeft = ulTotalPts;             
    dNextStartTime = dStart;
    ulPtNum = 0;

   /* ------------------------------------------------------------------
    *	Figure out how many points you need to allocate storage for --
    *	works best if only read and analyze about a 1000 pts at a time;
    *	there's always enough memory for that allocation.
    *	
	* ----------------------------------------------------------------*/
	if (ulTotalPts > NUM_MEAS_ALLOC_PTS) {
	    iNumAllocPts = NUM_MEAS_ALLOC_PTS;
	}                 
	else {
		iNumAllocPts = (int)ulTotalPts;
	}	
	iPtsToRead = iNumAllocPts;
	
   /* ------------------------------------------------------------------
    *	Allocate the array needed in the processing
	* ----------------------------------------------------------------*/
	pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((iNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
	while ((pdPts == NULL) && (iNumAllocPts > 50)){
	    iNumAllocPts = iNumAllocPts - 50;
		pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((iNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
	}   
	if (iNumAllocPts <= 0) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
    	if (pWin != NULL) pWin->Printf("\n\nNot enough memory; measurement search terminated.");
    	return(false); 
    }	
	
   /* ------------------------------------------------------------------
	*   Print the station specific header
	* ----------------------------------------------------------------*/
	if (!pglFacCfg->GetStationRecord(glsFacNum, &StaID, &dbSta)) return(false);
	if (pWin != NULL) pWin->Printf("\n\nStation = %s", dbSta.sz_sta_name);		//print station name
	if (dbSta.s_sta_type == JSR_TYPE) {
		if (pWin != NULL) pWin->Printf("   (JSR with SR Collect)");
		dbStaDflt.s_sta_dflt_num_key = dbSta.s_sta_num_key;
		status = GetSRMeasurementParameters(&dbStaDflt); // get parameters to analyze this station
		if (!status) return(false);
		if (dbStaDflt.b_meas_make_new_status == TRUE) {
			if (pWin != NULL) pWin->Printf(" Reanalyzed");
			PrintStationParameters(pWin);
		}
	}
	if (dbSta.s_sta_type == JSR1_TYPE) {
		if (pWin != NULL) pWin->Printf("   (JSR with MIC)");
			PrintStationParameters(pWin);
	}
	if (dbSta.s_sta_type == ISR_TYPE) {
		if (pWin != NULL) pWin->Printf("   (ISR)");
			PrintStationParameters(pWin);
	}
	if (bDiagPrint == FALSE) {
			if (pWin != NULL) pWin->Printf("\n\n         Start    End      ID          Width Measurement   Maximum Maximum    Avg Totals    Avg Reals Dir        R/");
			if (pWin != NULL) pWin->Printf(  "\nDate     Time     Time                (secs) Type             Rate    Time       Rate         Rate               T");
		                           /*90.05.07 10:10:01 10:20:25 12345 Assay*/
	}
	else {	// doing the diagnostic print
		if (pWin != NULL) pWin->Printf("\n\n                  Pt     Status          Reals         Totals");
		if (pWin != NULL) pWin->Printf  ("\nDate     Time     Num    Byte            Data          Data");
					                   /*90.05.07 10:10:01 1234 123456 1800 1234567890*/
	}
    
   /* ------------------------------------------------------------------
	*   Create the dialog box that is used to show analysis progress for this station
	* ----------------------------------------------------------------*/
	CGFmtStr Msg1, Msg2;
	TwoLineProgressBar *pTwoLineProgressBar;
	BOOL bContinue = true;

	Msg1.Printf("Processing station %s.\n", dbSta.sz_sta_name);
	Msg2.Printf("Searched %ld out of %ld bytes", ulPtNum, ulTotalPts);
	pTwoLineProgressBar = new TwoLineProgressBar(ulTotalPts, ulTotalPts/20, Msg1, Msg2, "Radiation Review: Determine Measurement Progress");
	if (pTwoLineProgressBar) {
		do {
			ulPtNum++;
						    
		   /* ------------------------------------------------------------------
			*	Figure out how many points to read and process
			*	If necessary, read one point past the end of this segment to determine
			*	where to start the next read.
			* ----------------------------------------------------------------*/
			if (ulNumPtsLeft > (unsigned long)iNumAllocPts) {
				iPtsToRead = iNumAllocPts;
			}
			else {
				iPtsToRead = (int)ulNumPtsLeft;
			}
			ulNumPtsLeft = ulNumPtsLeft - iPtsToRead;
									
		   /* ------------------------------------------------------------------
			*	Read the points from the db
			* ----------------------------------------------------------------*/   
			status = GetDoubleDataByStation(StaID, dNextStartTime, dEnd, (unsigned long)iPtsToRead, &ulPtsRead, pdPts, &dNextEndTime, FALSE);
			if (status == uiDAY_NOT_IN_DB_STATUS) status = TRUE;  // handle case of no data in this subinterval 
			if (status != TRUE) {
				bContinue = false;
				break;
			}
			dNextStartTime = dNextEndTime;
					
			if (bFirstPass == TRUE) {	// first point in analysis interval
				dFirstAnalyzedTime = pdPts[0].dJulianTime;
			}                      
			
		   /* ------------------------------------------------------------------
			*	Examine the status records of this group of points
			* ----------------------------------------------------------------*/
			i = 0;
			do {
			    ulPtNum++;
				if (bDiagPrint == TRUE) { // write raw data to the text window for diagnostics
					glMyDateTime.DATETimestampToDateTimeStrs(pdPts[i].dJulianTime, szDate, szTime);
					if (pWin != NULL) 
						pWin->Printf("\n%s %s %6ld %04x   %14.2f %14.2f", szDate, szTime, ulPtNum, pdPts[i].ulStatus , pdPts[i].dData[REALS_OFFSET], pdPts[i].dData[TOTALS_OFFSET]); 
				}
				if ((pdPts[i].ulStatus & FIRST_RUN_BIT) == FIRST_RUN_BIT) {
    				// this is the start of the meas
					// Check if currently in a meas, if yes, terminate and write that meas.
					if ((Event.bStarted == TRUE) && (Event.usMeasType != UNKNOWN_MEAS)) {
						Event.bStarted = FALSE;
						Event.dEndTime = pdPts[i].dJulianTime; // current point is the end
						if (glldb_StaDfltRec.s_meas_analysis_type == TOTALS_ANALYSIS) {	// SCR00191 21-Dec-2005 SFK Subtract user specified minutes from beginning
							Event.dBegTime = Event.dBegTime - (glldb_StaDfltRec.d_meas_add_min/1440.);	
						}
						status = WriteMeasToDb(StaID, &Event, &dbChanEvent);
						if ((bDiagPrint == TRUE) && (pWin != NULL)) {
							pWin->Printf(" Terminate last meas");
						}
		    			PrintMeasEventToTextWin(&dbChanEvent, pWin, &ulNumEvents);
					}
					// in diagnostics indicate are starting a new measurement.
    				if ((bDiagPrint == TRUE) && (pWin != NULL)) pWin->Printf(" Peak Start");

    				// fill in the start information in the meas record
    				Event.bStarted = TRUE;
    				Event.dBegTime = pdPts[i].dJulianTime;    
					Event.dEndTime = 0;    
					Event.dMaxTime = pdPts[i].dJulianTime;			    					
					Event.dMaxValue = pdPts[i].dData[REALS_OFFSET]/pdPts[i].dElapsedTime;
					if (glldb_StaDfltRec.s_meas_analysis_type == TOTALS_ANALYSIS) {
    					Event.dMaxValue = pdPts[i].dData[TOTALS_OFFSET]/pdPts[i].dElapsedTime;
					}
    				Event.dIntSum = 0.0;
					Event.dRealsSum = 0.0;
					Event.dTotalsSum = 0.0;
    				Event.usType = UNKNOWN_EVENT;
					Event.usMeasType = UNKNOWN_MEAS;
					Event.usNumPts = 0;	// point will be counted in the next section
					Event.dSumTime = 0.0;
    			}
			
				// check if any status bits are set, if yes, figure out the type of measurement 
				ulMeasType = pdPts[i].ulStatus & MEAS_TYPES_BITS;
				if ((ulMeasType != 0)  && (Event.bStarted == TRUE)) {
					Event.usNumPts++;
					if ((ulMeasType & AT_BIT) == AT_BIT) {
						if ((bDiagPrint == TRUE) && (pWin != NULL)) pWin->Printf(" Moving");
					}
					else if ((ulMeasType & LOW_BKG_BIT) == LOW_BKG_BIT) {
						Event.usMeasType = LOW_BKG_MEAS;
						Event.usType = BACKGROUND_EVENT;
						if ((bDiagPrint == TRUE) && (pWin != NULL)) pWin->Printf(" Low Bkg");
					}
					else if ((ulMeasType & HIGH_BKG_BIT) == HIGH_BKG_BIT) {
						Event.usMeasType = HIGH_BKG_MEAS;
						Event.usType = BACKGROUND_EVENT;
						if ((bDiagPrint == TRUE) && (pWin != NULL)) pWin->Printf(" High Bkg");
					}
					else if ((ulMeasType & EMPTY_BIT)== EMPTY_BIT) {
						Event.usMeasType = EMPTY_MEAS;
						Event.usType = MEASUREMENT_EVENT;
						if ((bDiagPrint == TRUE) && (pWin != NULL)) pWin->Printf(" Empty");
					}
					else if ((ulMeasType & ASSAY_BIT) == ASSAY_BIT) {
						Event.usMeasType = ASSAY_MEAS;
						Event.usType = MEASUREMENT_EVENT;
						if ((bDiagPrint == TRUE) && (pWin != NULL)) pWin->Printf(" Assay");
					}
					else if ((ulMeasType & TOTALS_BIT) == TOTALS_BIT) {
						Event.usMeasType = TOTALS_MEAS;
						Event.usType = MEASUREMENT_EVENT;
						if ((bDiagPrint == TRUE) && (pWin != NULL)) pWin->Printf(" Totals");
					}
					else if ((ulMeasType & CF_CHECK_BIT) == CF_CHECK_BIT) {
						Event.usMeasType = CF_CHECK_MEAS;
						Event.usType = MEASUREMENT_EVENT;
						if ((bDiagPrint == TRUE) && (pWin != NULL)) pWin->Printf(" Cf Check");
					}						else if ((ulMeasType & CF_NORM_BIT) == CF_NORM_BIT) {
						Event.usMeasType = CF_NORM_MEAS;
						Event.usType = MEASUREMENT_EVENT;
						if ((bDiagPrint == TRUE) && (pWin != NULL)) pWin->Printf(" Cf Norm");
					}

					if ((ulMeasType & AT_BIT) != AT_BIT) {  // only accumulate sums if not moving
						Event.dIntSum += pdPts[i].dData[REALS_OFFSET];	// accumulate sum
						Event.dRealsSum += pdPts[i].dData[REALS_OFFSET];	// accumulate sum
						Event.dTotalsSum += pdPts[i].dData[TOTALS_OFFSET];	// accumulate sum
						Event.dSumTime += pdPts[i].dElapsedTime;
					}
					if (glldb_StaDfltRec.s_meas_analysis_type == TOTALS_ANALYSIS) {
				    	if ((pdPts[i].dData[TOTALS_OFFSET]/pdPts[i].dElapsedTime) > Event.dMaxValue) {  // found a new max in totals??
							Event.dMaxTime = pdPts[i].dJulianTime;
							Event.dMaxValue = pdPts[i].dData[TOTALS_OFFSET]/pdPts[i].dElapsedTime;
						}
					}
					else {
					    if ((pdPts[i].dData[REALS_OFFSET]/pdPts[i].dElapsedTime) > Event.dMaxValue) {  // found a new max in reals??
							Event.dMaxTime = pdPts[i].dJulianTime;
							Event.dMaxValue = pdPts[i].dData[REALS_OFFSET]/pdPts[i].dElapsedTime;
						}
					}
				}
			

				// if no status bits are set and we are in a measurement, then terminate the meas.
				if ((pdPts[i].ulStatus == 0) && (Event.bStarted == TRUE)) {
					Event.bStarted = FALSE;
					Event.dEndTime = pdPts[i-1].dJulianTime;
			    	//status = WriteMeasToDb(sSta, &Event, &dbChanEvent);
					if (glldb_StaDfltRec.s_meas_analysis_type == TOTALS_ANALYSIS) {	// SCR00191 21-Dec-2005 SFK Subtract user specified minutes from beginning
						Event.dBegTime = Event.dBegTime - (glldb_StaDfltRec.d_meas_add_min/1440.);	
					}
					status = WriteMeasToDb(StaID, &Event, &dbChanEvent);

					if ((bDiagPrint == TRUE) && (pWin != NULL)) {
						pWin->Printf(" Peak End + 1");
					}
		    		PrintMeasEventToTextWin(&dbChanEvent, pWin, &ulNumEvents);

				}

			   /* ------------------------------------------------------------------
				*	Periodically update the progress bar
				* ----------------------------------------------------------------*/
				Msg2.Printf("Searched %ld out of %ld bytes", ulPtNum, ulTotalPts);
				bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
				if (glImportExit == TRUE) bContinue = false;

				i++;
			} while ((i<(int)ulPtsRead) && bContinue);	
		
			bFirstPass = FALSE;			/* have completed at least one pass */

									
		} while ((ulNumPtsLeft > 0)  && bContinue);
			
		pTwoLineProgressBar->Close();
		delete pTwoLineProgressBar;
		
		dLastAnalyzedTime = pdPts[i-1].dJulianTime;  
		   
		// print summary info
		if (pWin != NULL) {
			if (!bContinue) pWin->Printf("\nSearch canceled by user.");
			else pWin->Printf("\nSearch successfully completed.");
			glMyDateTime.DATETimestampToDateTimeStrs(dFirstAnalyzedTime, szDate, szTime);
			pWin->Printf("\nData analyzed from  %s %s to ", szDate, szTime);
			glMyDateTime.DATETimestampToDateTimeStrs(dLastAnalyzedTime, szDate, szTime);
			pWin->Printf("%s %s", szDate, szTime);
			pWin->Printf("    (%ld data points)\n", (ulTotalPts - ulNumPtsLeft));
			pWin->Printf("Total Measurements Found for Station %s = %ld\n\n", dbSta.sz_sta_name, ulNumEvents); 
			pWin->Printf("______________________________________________________________________\n\n");
		}
	}
	
	if (pdPts != NULL) free(pdPts);
	pdPts = NULL;
    return(bContinue);
}

// August 2002 patch until upgrade FacConfig  Assume A=1, B=2, C=3, G1=4, G2=5
//static int WriteGRANDMIIToDb(short sSta, struct EVENT *pEvent, struct db_event_rec *pdbEvent)
static int WriteGRANDMIIToDb(struct IDStruct ChanID, struct EVENT *pEvent, struct db_event_rec *pdbEvent)
{
	
	// fill in db record and record in database
	pdbEvent->d_event_beg_time_key = pEvent->dBegTime;
	pdbEvent->d_event_end_time = pEvent->dEndTime;
	pdbEvent->d_event_max_time = pEvent->dMaxTime;
	pdbEvent->d_event_grp_time = 0;
	pdbEvent->d_event_integral_sum = pEvent->dIntSum;	
	pdbEvent->d_event_max_value = pEvent->dMaxValue;
	pdbEvent->d_event_avg_reals_rate = 0.0;
	pdbEvent->d_event_avg_totals_rate = 0.0;
	pdbEvent->us_event_type = pEvent->usType;
	pdbEvent->us_event_status = 0;
	pdbEvent->us_event_type1 = pEvent->usMeasType;
	pdbEvent->us_event_status1 = pEvent->usStatus;
	pdbEvent->us_event_type2 = 0;
	pdbEvent->us_event_status2 = 0;
	strcpy(pdbEvent->sz_event_id, "ID");
	pdbEvent->s_num_runs = pEvent->usNumPts;

	int iNumChans = pglFacCfg->GetNumChannelsForStation(glsFacNum, &ChanID);
	struct IDStruct *pChanIDs;
	pChanIDs = new struct IDStruct[iNumChans];
	int i;
	// fill in the station number and type part of the structure
	for (i=0; i<iNumChans; i++) {
		pChanIDs[i].sStaID = ChanID.sStaID;
		pChanIDs[i].sStaType = ChanID.sStaType;
	}
	// now get the channel IDs for all channels at this station
	if (!pglFacCfg->GetChannelIDs(glsFacNum,pChanIDs)) return (FALSE);

	// now create an MII event for all the channels at this station
	for (i=0; i<iNumChans; i++) {
		pdbEvent->us_event_chan = pChanIDs[i].sChanID;
		CreateChanEventRec(pChanIDs[i], pdbEvent);
	}

	return(TRUE);
}
    


/*===========================================================================
 *
 *  Name	 :  FindGRANDMeasurementType
 *
 *  Purpose	 :  Search the status bytes looking for the beginning and ending
 *				of MII events.  Fill and write event records for each measurement
 *				found.
 *
 *  Return value :  None
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  03-Apr-1998	SFK		Created 
===========================================================================*/
static BOOL FindGRANDMeasurementType(struct IDStruct StaID, DATE dStart, unsigned long ulTotalPts, BOOL bDiagPrint, CGUI_TextWin *pWin, DATE dEnd)
{
               
    struct WHOLE_DOUBLE_DATA_PT1 *pdPts;             
    struct EVENT Event;
    struct db_event_rec dbChanEvent;

    int  status; 
    int  i;
    DATE dNextStartTime, dNextEndTime;  
    unsigned long ulPtsRead;		/* pts to read from db */
    unsigned long ulPtNum;
    DATE dFirstAnalyzedTime, dLastAnalyzedTime;
    unsigned long ulNumPtsLeft;
    unsigned long ulNumEvents = 0;
    int iNumAllocPts; 				/* pts were able to allocate in memory */
    int iPtsToRead;					/* pts to read from db */
    char szDate[DT_LEN+1], szTime[DT_LEN+1];
    BOOL bFirstPass = TRUE;

	Event.bStarted = FALSE;
	Event.dBegTime = 0;
	Event.dEndTime = 0;
	Event.dMaxValue = 0.0;
	Event.dIntSum = 0.0;
	Event.usType = -1;   
	Event.usStatus = -1;
    	
	Event.dRealsSum = 0.0;
	Event.dTotalsSum = 0.0;

    ulNumPtsLeft = ulTotalPts;             
    dNextStartTime = dStart;
    ulPtNum = 0;

   /* ------------------------------------------------------------------
    *	Figure out how many points you need to allocate storage for --
    *	works best if only read and analyze about a 1000 pts at a time;
    *	there's always enough memory for that allocation.
    *	
	* ----------------------------------------------------------------*/
	if (ulTotalPts > NUM_MEAS_ALLOC_PTS) {
	    iNumAllocPts = NUM_MEAS_ALLOC_PTS;
	}                 
	else {
		iNumAllocPts = (int)ulTotalPts;
	}	
	iPtsToRead = iNumAllocPts;
	
   /* ------------------------------------------------------------------
    *	Allocate the array needed in the processing
	* ----------------------------------------------------------------*/
	pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((iNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
	while ((pdPts == NULL) && (iNumAllocPts > 50)){
	    iNumAllocPts = iNumAllocPts - 50;
		pdPts = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((iNumAllocPts+1) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
	}   
	if (iNumAllocPts <= 0) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
    	if (pWin != NULL) pWin->Printf("\n\nNot enough memory; measurement search terminated.");
    	return(false); 
    }	
	
   /* ------------------------------------------------------------------
	*   Print the station specific header
	* ----------------------------------------------------------------*/
	char szStaName[DB_NAME_LEN+1];
	pglFacCfg->GetStationLongName(glsFacNum, &StaID, szStaName, DB_NAME_LEN);
	if (pWin != NULL) pWin->Printf("\n\nStation = %s  (GRAND)", szStaName);		//print station name 
	if (bDiagPrint == FALSE) {
		if (pWin != NULL) pWin->Printf("\n\n         Start    End      ID          Width Measurement   Totals Avg Reals");
		if (pWin != NULL) pWin->Printf(  "\nDate     Time     Time                (secs) Type          Rate       Rate ");
		                           /*90.05.07 10:10:01 10:20:25 12345 Assay*/
	}
	else {	// doing the diagnostic print
		if (pWin != NULL) pWin->Printf("\n\n                  Pt     Status          Chan");
		if (pWin != NULL) pWin->Printf  ("\nDate     Time     Num    Byte            A   ");
					                   /*90.05.07 10:10:01 1234 123456 1800 1234567890*/
	}
    
   /* ------------------------------------------------------------------
	*   Create the dialog box that is used to show analysis progress for this station
	* ----------------------------------------------------------------*/
	CGFmtStr Msg1, Msg2;
	TwoLineProgressBar *pTwoLineProgressBar;
	BOOL bContinue = true;

	Msg1.Printf("Processing station %s.\n", szStaName);
	Msg2.Printf("Searched %ld out of %ld bytes", ulPtNum, ulTotalPts);
	pTwoLineProgressBar = new TwoLineProgressBar(ulTotalPts, ulTotalPts/20, Msg1, Msg2, "Radiation Review: Determine Measurement Progress");
	if (pTwoLineProgressBar) {
		do {
			ulPtNum++;
						    
		   /* ------------------------------------------------------------------
			*	Figure out how many points to read and process
			*	If necessary, read one point past the end of this segment to determine
			*	where to start the next read.
			* ----------------------------------------------------------------*/
			if (ulNumPtsLeft > (unsigned long)iNumAllocPts) {
				iPtsToRead = iNumAllocPts;
			}
			else {
				iPtsToRead = (int)ulNumPtsLeft;
			}
			ulNumPtsLeft = ulNumPtsLeft - iPtsToRead;
									
		   /* ------------------------------------------------------------------
			*	Read the points from the db
			* ----------------------------------------------------------------*/   
			status = GetDoubleDataByStation(StaID, dNextStartTime, dEnd, (unsigned long)iPtsToRead, &ulPtsRead, pdPts, &dNextEndTime, TRUE);
			if (status == uiDAY_NOT_IN_DB_STATUS) status = TRUE;  // handle case of no data in this subinterval 
			if (status != TRUE) {
				bContinue = false;
				break;
			}
			dNextStartTime = dNextEndTime;
					
			if (bFirstPass == TRUE) {	// first point in analysis interval
				dFirstAnalyzedTime = pdPts[0].dJulianTime;
			}                      
			
		   /* ------------------------------------------------------------------
			*	Examine the status records of this group of points
			* ----------------------------------------------------------------*/
			i = 0;
			do {
			    ulPtNum++;
				if (bDiagPrint == TRUE) { // write raw data to the text window for diagnostics
					glMyDateTime.DATETimestampToDateTimeStrs(pdPts[i].dJulianTime, szDate, szTime);
					if (pWin != NULL) 
						pWin->Printf("\n%s %s %6ld %04x", szDate, szTime, ulPtNum, pdPts[i].ulStatus); 
				}
				if ((pdPts[i].ulStatus & MII_BIT) == MII_BIT) {
    				// this is the start of the meas
					if (Event.bStarted == FALSE) {
						Event.bStarted = TRUE;
						if ((bDiagPrint == TRUE) && (pWin != NULL)) {
							pWin->Printf(" New MII Start");
						}
    					// fill in the start information in the meas record
    					Event.bStarted = TRUE;
    					Event.dBegTime = pdPts[i].dJulianTime;    
						Event.dEndTime = 0;    
						Event.dMaxTime = pdPts[i].dJulianTime;			    					
    					Event.dMaxValue = 0.0;
    					Event.dIntSum = 0.0;
						Event.dRealsSum = 0.0;
						Event.dTotalsSum = 0.0;
    					Event.usType = MEASUREMENT_EVENT;
						Event.usMeasType = MII_MEAS;
						Event.usNumPts = 1;
						Event.dSumTime = 0.0;
					}
					else {
  						Event.usNumPts++;
						if ((bDiagPrint == TRUE) && (pWin != NULL)) pWin->Printf(" MII");
					}
				}

				// if no status bits are set and we are in a measurement, then terminate the meas.
				if (((pdPts[i].ulStatus  & MII_BIT) == 0) && (Event.bStarted == TRUE)) {
					Event.bStarted = FALSE;
					Event.dEndTime = pdPts[i-1].dJulianTime;
					status = WriteGRANDMIIToDb(StaID, &Event, &dbChanEvent);

					if ((bDiagPrint == TRUE) && (pWin != NULL)) {
						pWin->Printf(" MII End + 1");
					}
		    		PrintMeasEventToTextWin(&dbChanEvent, pWin, &ulNumEvents);
				}	    			                

			   /* ------------------------------------------------------------------
				*	Periodically update the progress bar
				* ----------------------------------------------------------------*/
				Msg2.Printf("Searched %ld out of %ld bytes", ulPtNum, ulTotalPts);
				bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
				if (glImportExit == TRUE) bContinue = false;

				i++;
			} while ((i<(int)ulPtsRead) && bContinue);	

			bFirstPass = FALSE;			/* have completed at least one pass */

					
		} while ((ulNumPtsLeft > 0)  && (bContinue));
			
		pTwoLineProgressBar->Close();
		delete pTwoLineProgressBar;

		dLastAnalyzedTime = pdPts[i-1].dJulianTime;  
		   
		// print summary info
		if (pWin != NULL) {
			if (!bContinue) pWin->Printf("\nSearch canceled by user.");
			else pWin->Printf("\nSearch successfully completed.");
			glMyDateTime.DATETimestampToDateTimeStrs(dFirstAnalyzedTime, szDate, szTime);
			pWin->Printf("\nData analyzed from %s %s to ", szDate, szTime);
			glMyDateTime.DATETimestampToDateTimeStrs(dLastAnalyzedTime, szDate, szTime);
			pWin->Printf("%s %s", szDate, szTime);
			pWin->Printf("    (%ld data points)\n", (ulTotalPts - ulNumPtsLeft));
			pWin->Printf("Total MIIs Events Found for Station %s = %ld\n\n", szStaName, ulNumEvents); 
			pWin->Printf("______________________________________________________________________\n\n");
		}
	}
	
	if (pdPts != NULL) free(pdPts);
	pdPts = NULL;
    return(bContinue);
}


/*===========================================================================
 *
 *  Name	 :  FindMeasurements
 *
 *  Purpose	 :  Search the specified stations and look for SR measurements
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
 *	27-May-1998 SFK	Added support of ISR, 1K MCA
===========================================================================*/
int FindMeasurements(struct IDStruct StaID, DATE dStart, DATE dEnd, BOOL bDiagPrint, CGUI_TextWin *pWin)
{
	struct db_sta_dflt_rec dbStaDflt;
    
    int  status; 
    DATE dStartTime, dEndTime;		/* julian start time of interval to review */
    unsigned long ulTotalPts;
	BOOL bContinue = true;
    
   /* ------------------------------------------------------------------
    *	Check to see that there is data for this day and station
    *	in the database.  If no data for this station in the specified
    *	time interval, then quit.
    * ----------------------------------------------------------------*/
	status = GetDataStatistics(StaID, true, dStart, dEnd, &dStartTime, &dEndTime, &ulTotalPts);
    if (status != TRUE) return(FALSE); 
    if (ulTotalPts == 0) {
		RadReviewMsg(uiNO_MATCHING_DATA_WARN);
    	if (pWin != NULL) pWin->Printf("\n\nNo data found, measurement search terminated.");
    	return(TRUE);
    }	

   /* ------------------------------------------------------------------
    *	Determine whether the data is from a JSR or ISR.  If from an
	*   ISR then must analyze the data stream now and set the status byte.
    * ----------------------------------------------------------------*/
	if ((StaID.sStaType == ISR_TYPE) || (StaID.sStaType == JSR1_TYPE)) 
		bContinue = SetSRStatusByte(StaID, dStartTime, dEndTime, ulTotalPts, FALSE, pWin);
	if (!bContinue) return(CANCELED);

	if (StaID.sStaType == JSR_TYPE) {
		dbStaDflt.s_sta_dflt_num_key = StaID.sStaID;
		status = GetSRMeasurementParameters(&dbStaDflt); // get parameters to analyze this station
		if (!status) return(FALSE);
		if (dbStaDflt.b_meas_make_new_status == TRUE) 
			bContinue = SetSRStatusByte(StaID, dStartTime, dEndTime, ulTotalPts, FALSE, pWin);
		if (!bContinue) return(CANCELED);
	}


   /* ------------------------------------------------------------------
    *	The status bytes now contain information.  Look through them
	*	to find measurement start and end times.
    * ----------------------------------------------------------------*/
//	if ((dbSta.s_sta_type == ISR_TYPE) || (dbSta.s_sta_type == JSR1_TYPE) || (dbSta.s_sta_type == JSR_TYPE)) 
	if ((StaID.sStaType == ISR_TYPE) || (StaID.sStaType == JSR1_TYPE) || (StaID.sStaType == JSR_TYPE)) 
		//bContinue = FindMeasurementType(sSta, dStartTime, ulTotalPts, bDiagPrint, pWin, dEnd);
		bContinue = FindMeasurementType(StaID, dStartTime, ulTotalPts, bDiagPrint, pWin, dEnd);
	if (StaID.sStaType == GRAND_TYPE) 
		//bContinue = FindGRANDMeasurementType(sSta, dStartTime, ulTotalPts, bDiagPrint, pWin, dEnd);
		bContinue = FindGRANDMeasurementType(StaID, dStartTime, ulTotalPts, bDiagPrint, pWin, dEnd);
//	if (dbSta.s_sta_type == MCA_TYPE) GUI_MsgBox("Measurements are not applicable to MCA data.  Option will abort now.", GUI_ICON_INFO, GUI_OK);
	
	char szStaName[DB_NAME_LEN+1];
	if (StaID.sStaType == MCA_TYPE) {
		pglFacCfg->GetStationLongName(glsFacNum, &StaID, szStaName, DB_NAME_LEN);
		if (pWin != NULL) {
			pWin->Printf("\n\nStation = %s  (MiniADC)", szStaName);		//print station name 
			pWin->Printf("\nMeasurements are not applicable to MCA data.\n");
			pWin->Printf("______________________________________________________________________\n\n");
		}
	}
	if (StaID.sStaType == BINARY_TYPE) {
		pglFacCfg->GetStationLongName(glsFacNum, &StaID, szStaName, DB_NAME_LEN);
		if (pWin != NULL) {
			pWin->Printf("\n\nStation = %s  (Binary)", szStaName);		//print station name 
			pWin->Printf("\nMeasurements are not applicable to binary data.\n");
			pWin->Printf("______________________________________________________________________\n\n");
		}
	}
	if ((StaID.sStaType == BOT_TYPE) || (StaID.sStaType == BOT_TYPE)) {
		pglFacCfg->GetStationLongName(glsFacNum, &StaID, szStaName, DB_NAME_LEN);
		if (pWin != NULL) {
			pWin->Printf("\n\nStation = %s  (VIFM)", szStaName);		//print station name 
			pWin->Printf("\nMeasurements are not applicable to VIFM data.\n");
			pWin->Printf("______________________________________________________________________\n\n");
		}
	}
	//Add message for EOSS types
	if (StaID.sStaType == EOSS_TYPE) {
		pglFacCfg->GetStationLongName(glsFacNum, &StaID, szStaName, DB_NAME_LEN);
		if (pWin != NULL) {
			pWin->Printf("\n\nStation = %s  (EOSS)", szStaName);		//print station name 
			pWin->Printf("\nMeasurements are not applicable to EOSS data.\n");
			pWin->Printf("______________________________________________________________________\n\n");
		}
	}

    if (bContinue) return(TRUE);
    else return(CANCELED);		// hit the cancel button
}    
    



    
/* Run types */
#define RUN_NBKG	 0	// Normal Background
#define RUN_LBKG	 1	// Low Background
#define RUN_HBKG	 2	// High Background
#define RUN_MOVING	 3	// Moving
#define RUN_EMPTY	 4	// Empty
#define RUN_ASSAY	 5	// Assay
#define RUN_CFCHECK	 6	// Cf Check Source
#define RUN_CFNORM	 7	// Cf Normalization
#define RUN_TOTALS	 8	// Totals Assay

#define MEAS_NONE		-1
//# from Walt define MEAS_NBKG		2 from Walt
//# from Walt define LOW_BKG_MEAS		3
//# from Walt define HIGH_BKG_MEAS		4
//# from Walt define EMPTY_MEAS		5
//# from Walt define ASSAY_MEAS		6
//# from Walt define CF_CHECK_MEAS	7
//# from Walt define CF_NORM_MEAS		8
//	#define UNKNOWN_MEAS	0		 // a type of MEASUREMENT_EVENT
//	#define LOW_BKG_MEAS	  1		   // a type of BACKGROUND_EVENT
//	#define HIGH_BKG_MEAS	   2	   // a type of BACKGROUND_EVENT
//	#define EMPTY_MEAS			 3		 // a type of MEASUREMENT_EVENT
//	#define ASSAY_MEAS			   4	   // a type of MEASUREMENT_EVENT
//	#define CF_CHECK_MEAS	   5	  // a type of MEASUREMENT_EVENT
//	#define CF_NORM_MEAS		6	  // a type of MEASUREMENT_EVENT
//	#define NORMAL_BKG_MEAS 7    // a type of BACKGROUND_EVENT
//	#define TOTALS_MEAS			  8		// a type of MEASUREMENT_EVENT
//	#define MII_MEAS			        9




/* <f> */
/*====================================================================
 *
 *     Function Name:	is_event_type()
 *
 *  Function Purpose:	Determine if measurement type is an EVENT.
 *
 *	Return value:	TRUE/FALSE.
 *
 *  Revision History
 *
 *     DATE	AUTHOR	VER REVISION
 *  ----------- ------- --- --------
 *  10-Jan-1995 WJH	2.0 Created.
 *	17-Jun-1998 SFK		Added totals assay.
 *
 *====================================================================*/

static BOOL is_event_type(int meas_type)
{
	switch (meas_type) {
	case ASSAY_MEAS:
	case EMPTY_MEAS:
	case CF_CHECK_MEAS:
	case CF_NORM_MEAS:
	case UNKNOWN_MEAS:
	case TOTALS_MEAS:
		return (TRUE);
	default:
		return (FALSE);
	}
}



/* <f> */
/*====================================================================
 *
 *  Name:		is_moving()
 *
 *  Purpose:		Indicates if material is moving.
 *			Determined by failing the A/T test.
 *
 *  Return value:	TRUE/FALSE
 *
 *  Revision History:
 *    date   / author	Revision
 *  -----------------	--------
 *  16-Jun-1988 JAP	Created.
 *  25-Feb-1992 JAP	Modified for PFPF software upgrade:  changed
 *			all references to T2G to AT (accidentals test).
 *  18-Apr-1994 WJH	Transformed form determine_AT_test().
 *  10-Jan-1995 WJH	Corrected sigmas test.
 *	29-May-1998 SFK Adapted for Radiation Review data structures and names.
 *
 *====================================================================*/

static BOOL is_moving(double dTotalsRate, double dAccidentalsRate, double dAccidentals)
{
	double dBias, dSigmas, dR;

	/* Check if passes A/T test totals rate threshold criterion */
	if (dTotalsRate < glldb_StaDfltRec.d_meas_at_thres)
		return (FALSE);	// A/T Test is not applicable

//	ASSERT(dTotalsRate > 0.0)

	/* Check if accidentals rate is zero */
	if (dAccidentalsRate == 0.0)
		return (FALSE);	// A/T Test is not applicable

	/* Calculate ratio of measured accidentals rate to theoretical rate */
	double dWidth = (glldb_StaDfltRec.d_meas_gate_width/1000000.0);
	dWidth = glldb_StaDfltRec.d_meas_gate_width * 1e-6;
	double dDum = dTotalsRate * dTotalsRate;
	dDum = dDum * dWidth;
	dR =  dAccidentalsRate / (dTotalsRate * dTotalsRate * dWidth );

	/* Check if passes A/T test bias limit criterion */
	dBias = fabs(1.0 - dR);
	if (dBias < glldb_StaDfltRec.d_meas_at_bias/100.0)
		return (FALSE);	// Passed A/T test

//	ASSERT(dAccidentalsRate > 0.0)

	/* Check if passes A/T test sigma limit criterion */
	dSigmas = dBias * sqrt(dAccidentals) / dR;
	if (dSigmas < glldb_StaDfltRec.d_meas_at_sigma)
		return (FALSE);	// Passed A/T test

	/* Failed A/T test */
	return (TRUE);
}


/* <f> */
/*====================================================================
 *
 *     Function Name:	determine_meas_type()
 *
 *  Function Purpose:	Determine measurement type.
 *
 *	Return value:	measurement type
 *
 *  Revision History
 *
 *     DATE	AUTHOR	VER REVISION
 *  ----------- ------- --- --------
 *  10-Jan-1995 WJH	2.0 Created.
 *	17-Jun-1998 SFK		Added totals assay.
 *	26-Jun-1998 SFK		Added check whether to have moving terminate the measurement.  If yes, then moving 
 *						causes the measurement type to be unknown and thus will terminate it.
 *
 *====================================================================*/

static int determine_meas_type(int run_type, int prior_meas_type)
{
	int meas_type;

	if (run_type == RUN_MOVING) {				// this run failed the A/T test
		if (glldb_StaDfltRec.b_meas_include_at_fail == FALSE) { // don't include A/T failed in runs
			meas_type = MEAS_NONE;
		}
		if (glldb_StaDfltRec.b_meas_term_on_moving == FALSE) {
			if (is_event_type(prior_meas_type))	// the prior run was a measurement so assume this continues it
				meas_type = prior_meas_type;
			else								// prior run was some kind of background
				meas_type = UNKNOWN_MEAS;		// is above reals threshold but don't know what kind yet
		}
		else meas_type = MEAS_NONE;
	} else {
		switch (run_type) {
		case RUN_NBKG:
			meas_type = NORMAL_BKG_MEAS;
			break;
		case RUN_LBKG:
			if (glldb_StaDfltRec.b_meas_low_bkg_check == TRUE){
				meas_type = LOW_BKG_MEAS;
			}
			else meas_type = NORMAL_BKG_MEAS;
			break;
		case RUN_HBKG:
			if (glldb_StaDfltRec.b_meas_high_bkg_check == TRUE) {
				meas_type = HIGH_BKG_MEAS;
			}
			else meas_type = NORMAL_BKG_MEAS;
			break;
		case RUN_EMPTY:
			meas_type = EMPTY_MEAS;
			break;
		case RUN_ASSAY:
			meas_type = ASSAY_MEAS;
			break;
		case RUN_CFCHECK:
			meas_type = CF_CHECK_MEAS;
			break;
		case RUN_CFNORM:
			meas_type = CF_NORM_MEAS;
			break;
		case RUN_TOTALS:
			meas_type = TOTALS_MEAS;
			break;
		default:	/* Should never happen! */
			meas_type = UNKNOWN_MEAS;
			break;
		}
	}

	return (meas_type);
}


/* <f> */
/*====================================================================
 *
 *     Function Name:	determine_run_type()
 *
 *  Function Purpose:	Determine the run type.
 *
 *	Return value:	run type
 *
 *  Revision History
 *
 *     DATE	AUTHOR	VER REVISION
 *  ----------- ------- --- --------
 *  27-Feb-1992 JAP	1.0 Created.
 *  03-May-1994 WJH	2.0 Revised.
 *  29-May-1998 SFK     Revised for Rad Review
 *	17-Jun-1998 SFK		Added totals assay, removed redundant totals cts checks.
 *  21-Apr-2005 SFK		Add a check for an upper limit of the R/T ratio for Cf to handle AMSR spike data
 *====================================================================*/
static int determine_run_type(double dAccidentals, double dTotalsRate, double dRealsRate, double dAccidentalsRate)
{

	if (dRealsRate < glldb_StaDfltRec.d_meas_reals_thres) {

		if (dTotalsRate <= glldb_StaDfltRec.d_meas_low_bkg_thres)
			return (RUN_LBKG);

		if (dTotalsRate < glldb_StaDfltRec.d_meas_high_bkg_thres)
			return (RUN_NBKG);

		if (glldb_StaDfltRec.s_meas_analysis_type == TOTALS_ANALYSIS) {
			if (dTotalsRate < glldb_StaDfltRec.d_meas_totals_thres)
				return (RUN_HBKG);
			else
				return (RUN_TOTALS);
		}
		else 
			return (RUN_HBKG);
	}

	if (is_moving(dTotalsRate, dAccidentalsRate, dAccidentals))
		return (RUN_MOVING);

	if (dRealsRate <= glldb_StaDfltRec.d_meas_empty_thres)
		return (RUN_EMPTY);

	/* Special case:  check if totals are too small */
	if (dTotalsRate == 0.0)
		return (RUN_LBKG);

	double iDum = dRealsRate/dTotalsRate;

	// 21-Apr-2005 SFK Add a check for an upper limit of the R/T ratio for Cf
	// 11-Jul-2005 SFK Added a parameter in RAD.INI for the upper limit rather than hardwire to 0.5
	if ((dRealsRate/dTotalsRate >= glldb_StaDfltRec.d_meas_cf_thres)  && (dRealsRate/dTotalsRate < pRAD_Init->Get_RTUpperLimit())) {
		if (dRealsRate >= glldb_StaDfltRec.d_meas_cf_norm_thres)
			return (RUN_CFNORM);
		else
			return (RUN_CFCHECK);
	} else
			return (RUN_ASSAY);


}


#define SECS_PER_MIN	60


/* <f> */
/*====================================================================
 *
 *     Function Name:	AnalyzeAndSetSRStatusByte()
 *
 *  Function Purpose:	With the raw data for current run, perform the
 *						A/T test, determine the next measurement state,
 *						execute the state transition table, accumulate
 *						measurement log information and monitor back-
 *						ground averaging.
 *
 *	Return value:	Run type.
 *
 *  Revision History
 *
 *     DATE	AUTHOR	VER REVISION
 *  ----------- ------- --- --------
 *  25-Feb-1992 JAP	1.0 Created.
 *  03-May-1994 WJH	2.0 Revised.
 *  18-Dec-1994 WJH	    Permit re-initialization of prev state.
 *  11-Jan-1994 WJH	    Revised to adhere to new user manual.
 *	29-May-1998 SFK		Modified to use in Radiation Review with database records
 *	26-Jun-1998 SFK		Only set high, low background bits if requested by parameter file
 *  08-Jul-2003 SFK		Made dMeasStartTime a static and initialized.
 *
 *====================================================================*/

static int AnalyzeAndSetSRStatusByte(struct WHOLE_DOUBLE_DATA_PT1 *pdPt)
{
	int run_type, meas_type;
	BOOL bIsFirstRun, bIsEvent;
	DATE dDuration, dMeasEndTime;
	double dTotalsRate, dAccidentalsRate, dRealsRate;
	static double dMeasStartTime = 0;

	static short sPriorMeasType = MEAS_NONE;

	pdPt->ulStatus = 0L;  // clear anything in the status byte

	/* Calculate data rates for this run */
	if (pdPt->dElapsedTime > 0) {
		dTotalsRate = pdPt->dData[TOTALS_OFFSET]/(double)pdPt->dElapsedTime;
		dRealsRate  = pdPt->dData[REALS_OFFSET]/(double)pdPt->dElapsedTime;
		dAccidentalsRate = pdPt->dData[ACCIDENTALS_OFFSET]/(double)pdPt->dElapsedTime;
	} else {
		dTotalsRate  = pdPt->dData[TOTALS_OFFSET];
		dRealsRate   = pdPt->dData[REALS_OFFSET];
		dAccidentalsRate = pdPt->dData[ACCIDENTALS_OFFSET];
	}

	/* Determine run type */
	run_type = determine_run_type(pdPt->dData[ACCIDENTALS_OFFSET], dTotalsRate, dRealsRate, dAccidentalsRate);

	/* Determine measurement type */
	meas_type = determine_meas_type(run_type, sPriorMeasType);

	/* Determine if measurement type is an event type */
	bIsEvent = is_event_type(meas_type);

	/* Determine if this is first run in a measurement */
	if (((bIsEvent && sPriorMeasType != UNKNOWN_MEAS) ||	// have an event and weren't in one before
		!bIsEvent)											// are in a background event
		&& sPriorMeasType != meas_type) {					// this is really the same measurement
		bIsFirstRun = TRUE;
		dMeasStartTime = pdPt->dJulianTime;
	}
	else {
		bIsFirstRun = FALSE;
	}

	/* Current data record is the last one we know in the measurement */
	dMeasEndTime = pdPt->dJulianTime;

	/* Determine duration of current measurement */
	dDuration = dMeasEndTime - dMeasStartTime;

	/* Signal monitor camera (if appropriate) */
//	signal_camera(bIsFirstRun, is_event, duration);

	/* Based on the measurement type, the run type, whether this it the first run,
	   and the measurement duration, set the status byte. */
	switch (meas_type) {
	case EMPTY_MEAS:
		pdPt->ulStatus |= EMPTY_BIT;
		break;
	case ASSAY_MEAS:
		pdPt->ulStatus |= ASSAY_BIT;
		break;
	case TOTALS_MEAS:
		pdPt->ulStatus |= TOTALS_BIT;
		break;
	case CF_CHECK_MEAS:
		pdPt->ulStatus |= CF_CHECK_BIT;
		break;
	case CF_NORM_MEAS:
		pdPt->ulStatus |= CF_NORM_BIT;
		break;
	case LOW_BKG_MEAS:
		pdPt->ulStatus |= LOW_BKG_BIT;
		break;
	case HIGH_BKG_MEAS:
		pdPt->ulStatus |= HIGH_BKG_BIT;
		break;
	case UNKNOWN_MEAS:
		pdPt->ulStatus |= UNUSUAL_BIT;
		break;
	}

	if ((bIsFirstRun) && (meas_type != NORMAL_BKG_MEAS)) 
		pdPt->ulStatus |= FIRST_RUN_BIT;

	if (run_type == RUN_MOVING)
		pdPt->ulStatus |= AT_BIT;

	if (bIsEvent) {
		dDuration = dMeasEndTime - dMeasStartTime; 
		if (dDuration < glldb_StaDfltRec.d_meas_min_len/86400.0)
			pdPt->ulStatus |= SHORT_BIT;
		if (dDuration > glldb_StaDfltRec.d_meas_max_len/86400.0)
			pdPt->ulStatus |= LONG_BIT;
	}

	/* Reassign prior measurement type */
	sPriorMeasType = meas_type;


	/* If background filtering enabled AND measurement type IS background */
//	if (gl_config_parms.use_filtering && !is_event) {

		/* Filter background data and write Raw Data file record */
//		filter_bkg_data(is_first_run, ptr_dt, &data_rec);

//	} else {
		/* Write Raw Data file record */
//		write_new_data_rec(&data_rec);
//	}

	/* If measurement type is NOT normal background */
//	if (meas_type != NORMAL_BKG_MEAS) {

		/* Write Measurement Log record */
//		if (is_first_run) {

			/* Write new data record */
//			write_new_mlog_rec(&mlog_rec);

//		} else {

			/* Overwrite current data record */
//			write_current_mlog_rec(&mlog_rec);
//		}
//	}

//	return (run_type);
	return(TRUE);
}

