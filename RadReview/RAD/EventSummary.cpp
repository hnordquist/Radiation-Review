///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : EventSummary.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		EventDisplayByChan
//		EventDisplayByTime
//		DailySummaryByChan
//		DailySummaryByTime
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "DbDay.h"
#include "DbEvent.h"
#include "EventNetArea.h"
#include "EventSummary.h"
#include "FacilityConfig.h"
#include "FindEvent.h"
#include "ProgressBars.h"
#include "TextFile.h"
#include "Utilities.h"
#include "MyDateTime.h"
#include "TypeDefinitions.h"
#include <math.h>

extern short glsFacNum;		// facility that is chosen at highest leve
extern CMyDateTime glMyDateTime;
 

/*===========================================================================
 *
 *  Name	 :  EventDisplayByChan
 *
 *  Purpose	 :  Display the individual event information about each channel sorted
 *				by channel.	Summarize the number of events for each individual channels
 *				and for each station.
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
 *	08-Nov-2001 SFK		Add calculation of sums and averages of width, maximum values and integral sums
 *						for all events.  Print the results.
 *	08-Aug-2002	SFK		Changed to DATE
 *	03-Oct-2003	SFK		Adapted to new FacMgr
 *	21-Dec-2004	SFK		Added option to include background events in results displayed
 *  02-Apr-2009 JCD		Modified event total calc to correct number per channel (had been running total, in conflict with label on the value)
===========================================================================*/
void EventDisplayByChan(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, CGUI_TextWin *pWin, BOOL bIncludeBkg)
{
               
    struct db_sta_rec dbSta;
    struct db_chan_rec dbChan;
	struct db_event_rec huge *pEvents = NULL;
	
	CGFmtStr msg;
    int  status; 
    int  j,k; 
    char ChanNames[MAX_NUM_DATA_CHANS][DB_NAME_LEN+1];
    char ChanShortNames[MAX_NUM_DATA_CHANS][DB_SHORT_NAME_LEN+1];
	struct IDStruct ChanIDs[MAX_NUM_DATA_CHANS];
    int iNumAnalyze;
 	unsigned long ulEventsRead, ulNumEvents;
 	unsigned long ulTotalMatchingEvents = 0;

	TextFile DumFile;

	if (!pglFacCfg->GetStationRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbSta)) 
		return;

   /* ------------------------------------------------------------------
	*	Determine how many channels to analyze at this station.  Read
	*	the associated channel names and save for use later in displaying
	* ----------------------------------------------------------------*/
	iNumAnalyze = 0;
	for (j = 0; j < MAX_NUM_DATA_CHANS; j++) 
	{
    	if ((pAnalyzeChans->sChanOffset[j]) != -1) 
		{
			pAnalyzeChans->StaID.sChanID = pAnalyzeChans->sChanOffset[j];
    		iNumAnalyze++;

			if (!pglFacCfg->GetChannelRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbChan))
				return;

    		MyStrnCpy(ChanNames[j], dbChan.sz_chan_name, DB_NAME_LEN);
    		MyStrnCpy(ChanShortNames[j], dbChan.sz_chan_short_name, DB_SHORT_NAME_LEN); //Ver 2.05 fixed to be DB_SHORT_NAME_LEN
			ChanIDs[j] = pAnalyzeChans->StaID;
			ChanIDs[j].sChanID = pAnalyzeChans->sChanOffset[j];
    	}
    	else 
		{
    		ChanNames[j][0] = '\0';
    		ChanShortNames[j][0] = '\0';
			ChanIDs[j].sStaID = -1;
    	}		
	}	
	if (iNumAnalyze == 0) return;              /* no channels at this station selected */


   /* ------------------------------------------------------------------
	*	Read all the events in the database for selected channels.
	*  Then print them based on channel.
	* ----------------------------------------------------------------*/
	for (j=0; j<MAX_NUM_DATA_CHANS; j++) {	
 		unsigned long ulTotalMatchingEventsPerChan = 0;	//JCD addition 4/2/2009
		//JCD use of ulTotalMatchingEvents changed to ulTotalMatchingEventsPerChan throughout loop 4/2/2009
		if (pAnalyzeChans->sChanOffset[j] != -1) {
			status = GetDataStatisticsForEventsByChannel(ChanIDs[j], dStart, dEnd, &ulNumEvents, ALL_EVENTS);
			if (status != TRUE) return;

			// will accumulate sum for the event width, the maximum peak value and the event integral sum
			double dWidthSum = 0;
			double dMaxSum = 0;
			double dIntegralSum = 0;
			double dNetAreaSum = 0;
			double dNumPts = 0;				// sfk 03/31/03
			double dNetArea;
			unsigned long ulNumPts;			// sfk 03/31/03
			
			if (ulNumEvents > 0) {

				//  Create the progress bar
				CGFmtStr Msg1, Msg2;
				TwoLineProgressBar *pTwoLineProgressBar;
				BOOL bContinue = true;
				unsigned long ulDum = 0;
				Msg1.Printf("Processing station %s.\n", dbSta.sz_sta_name);
				Msg2.Printf("Searched %ld out of %ld events", ulDum, ulNumEvents);
				pTwoLineProgressBar = new TwoLineProgressBar(ulNumEvents, ulNumEvents/20, Msg1, Msg2, "Radiation Review: Event Summary Progress", true, IDI_NEW_RAD_REVIEW);
				if (pTwoLineProgressBar) {

					/* allocate storage for event results to print */
					pEvents = (struct db_event_rec *)malloc((ulNumEvents+1) * sizeof(struct db_event_rec));
					if (pEvents == NULL) {
						RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
						return;
					}	
					
					/* read the events from the db */
					// 21-Dec-2004 SFK Changed to read ALL_EVENTS, not just CHANNEL_EVENTS
					if (GetDataByChannelEvents(ChanIDs[j], dStart, dEnd, ulNumEvents, &ulEventsRead, pEvents, ALL_EVENTS) != TRUE) {
						if (pEvents) free(pEvents);
						return;
					}
													 
					/* print results for all the events and count them */			                                 
					double dDum;
					for (k=0; k<(int)ulEventsRead; k++) {
						if ((pEvents[k].us_event_type == CHANNEL_EVENT) || // 21-Dec-2004  Modified to do channel and bkg, if requested
							((bIncludeBkg == TRUE) && (pEvents[k].us_event_type == BACKGROUND_EVENT) && (pEvents[k].us_event_type1 == NORMAL_BKG_MEAS))) {
							PrintChanEventResults(ChanIDs[j], &(pEvents[k]), ChanShortNames[j], &ulTotalMatchingEventsPerChan, &ulDum, pWin, &DumFile, TRUE, FALSE, FALSE);

							// Calculate the net area of the event
							EventCalculateNetArea(ChanIDs[j], &(pEvents[k]), &dNetArea, &ulNumPts, &dDum, &dDum, &dDum);

							// Accumulate the sums for this event.
							dNetAreaSum += dNetArea;
							double dTemp = (pEvents[k].d_event_end_time - pEvents[k].d_event_beg_time_key)*86400.0;
							if (dTemp == 0.0) dTemp = 1.0;		// 21-Dec-2004 Fixed = to ==
							dWidthSum += dTemp;
							dNumPts += (double)ulNumPts;								// sfk 03/31/03
							dMaxSum += pEvents[k].d_event_max_value;
							dIntegralSum += pEvents[k].d_event_integral_sum;

							//	Periodically update the progress bar
							Msg2.Printf("Searched %ld out of %ld events", k, ulEventsRead);
							bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
							if (!bContinue) break;
						}
					}    
					CGUI_App::ProcessEvents();                /* update the output page */
					if (pEvents) free(pEvents);
					pTwoLineProgressBar->Close();
					delete pTwoLineProgressBar;

				}
			}
		
			if (ulTotalMatchingEventsPerChan > 0) {  // 21-Dec-2004 SFK Changed to only do this if some events met conditions for printing
				// Print total sums that were calculated
				// print total width and number of points
				pWin->Printf("\n                      Total Sums  %5.0lf (%5.0lf)", dWidthSum, dNumPts);	

				// Print sum of maximum values in format "most appropriate" for number size
				if (dMaxSum < 10.0) pWin->Printf("%23.4lf ", dMaxSum);							// small number
				if ((dMaxSum < 1000.0) && (dMaxSum >= 10.0)) pWin->Printf("%22.1lf ", dMaxSum);	// medium
				if (dMaxSum >= 1000.0) pWin->Printf("%23.0lf ", dMaxSum);						// large

				// Print sum of integral sums in format "most appropriate" for number size
				if (dIntegralSum < 10.0) pWin->Printf("%21.4lf ", dIntegralSum);								// small number
				if ((dIntegralSum < 1000.0) && (dIntegralSum >= 10.0)) pWin->Printf("%21.1lf ", dIntegralSum);	// medium
				if (dIntegralSum >= 1000.0) pWin->Printf("%21.0lf ", dIntegralSum);								// large

				// Print net area in format "most appropriate" for number size
				if (dNetAreaSum < 10.0) pWin->Printf("%12.4lf ", dNetAreaSum);									// small number
				if ((dNetAreaSum < 1000.0) && (dIntegralSum >= 10.0)) pWin->Printf("%12.1lf ", dNetAreaSum);	// medium
				if (dNetAreaSum >= 1000.0) pWin->Printf("%12.0lf ", dNetAreaSum);								// large

				// Print averages sums that were calculated
				double dNumEvents = (double)ulTotalMatchingEventsPerChan;		// 06-Jan-2005 Fixed to use only matching events for calculation
				pWin->Printf("\n                      Averages   %6.1lf (%5.0lf)", dWidthSum/dNumEvents, dNumPts/dNumEvents);		// sfk 03/31/03

				// Print avg of maximum values in format "most appropriate" for number size
				dMaxSum = dMaxSum/dNumEvents;					// sfk 03/31/03
				if (dMaxSum < 10.0) pWin->Printf("%23.4lf ", dMaxSum);							// small number
				if ((dMaxSum < 1000.0) && (dMaxSum >= 10.0)) pWin->Printf("%22.1lf ", dMaxSum);	// medium
				if (dMaxSum >= 1000.0) pWin->Printf("%23.0lf ", dMaxSum);						// large
				
				// Print avg of integral sums in format "most appropriate" for number size
				dIntegralSum = dIntegralSum/dNumEvents;				// sfk 03/31/03
				if (dIntegralSum < 10.0) pWin->Printf("%21.4lf ", dIntegralSum);								// small number
				if ((dIntegralSum < 1000.0) && (dIntegralSum >= 10.0)) pWin->Printf("%21.1lf ", dIntegralSum);	// medium
				if (dIntegralSum >= 1000.0) pWin->Printf("%21.0lf ", dIntegralSum);								// large
				
				// Print avg net area in format "most appropriate" for number size
				dNetAreaSum = dNetAreaSum/dNumEvents;				// sfk 03/31/03
				if (dNetAreaSum < 10.0) pWin->Printf("%12.4lf ", dNetAreaSum);									// small number
				if ((dNetAreaSum < 1000.0) && (dIntegralSum >= 10.0)) pWin->Printf("%12.1lf ", dNetAreaSum);	// medium
				if (dNetAreaSum >= 1000.0) pWin->Printf("%12.0lf ", dNetAreaSum);								// large
			}

			/* Summarize channel events */
			pWin->Printf("\nChannel %s Total Events = %ld", ChanNames[j], ulTotalMatchingEventsPerChan); 

			pWin->Printf("\n\n"); 

			ulTotalMatchingEvents += ulTotalMatchingEventsPerChan;	//JCD addition 4/2/2009
		}	
	}	
			 
	/* print total events found for this station */                       
	pWin->Printf("\n\nStation %s Total Events = %ld\n\n", dbSta.sz_sta_name, ulTotalMatchingEvents); 
	pWin->Printf("______________________________________________________________________\n");
	return;

}    
        
         
         
/*===========================================================================
 *
 *  Name	 :  EventDisplayByTime
 *
 *  Purpose	 :  Search the events in the database by time and display information
 *				from the specified channels.  If the starting times of the events
 *				are separated by more than ulEventSeparation seconds, then print
 *				a blank line so the lines are grouped as separate events.
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
 *  23-Jan-1996	SFK		Created 
 *	08-Nov-2001 SFK		Add calculation of sums and averages of width, maximum values and integral sums
 *						for all events.  Print the results.
 *	08-Aug-2002	SFK		Changed to DATE
 *	03-Oct-2003	SFK		Adapted to new FacMgr
 *	21-Dec-2004	SFK		Added option to include background events in results displayed
===========================================================================*/
void EventDisplayByTime(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, CGUI_TextWin *pWin, BOOL bGroup, BOOL bIncludeBkg)
{
               
    struct db_sta_rec dbSta;
    struct db_chan_rec dbChan;
	struct db_event_rec dbEvent;
	
    int  status; 
    int  j; 
	
    char ChanNames[MAX_NUM_DATA_CHANS][DB_NAME_LEN+1];
    char ChanShortNames[MAX_NUM_DATA_CHANS][DB_SHORT_NAME_LEN+1];
	struct IDStruct ChanIDs[MAX_NUM_DATA_CHANS];
    int iNumAnalyze;
 	unsigned long ulTotalMatchingEvents = 0;
 	DATE dLastGroupTime;
 	unsigned long ulEventsPrinted = 0;
	unsigned long ulNumEvents, ulDum;
	unsigned long ulNumTotalEvents = 0;
	DATE dMaxTimeOfGroupFirstEvent;

	TextFile DumFile;

	if (!pglFacCfg->GetStationRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbSta)) return;

   /* ------------------------------------------------------------------
	*	Determine how many channels to analyze at this station.  Read
	*	the associated channel names and save for use later in displaying.
	*   Also calculate the number of events associated with each channel
	*	being analyzed.
	* ----------------------------------------------------------------*/
	iNumAnalyze = 0;
	for (j=0; j<MAX_NUM_DATA_CHANS; j++) 
	{
    	if ((pAnalyzeChans->sChanOffset[j]) != -1) 
		{
    		iNumAnalyze++;
			pAnalyzeChans->StaID.sChanID = pAnalyzeChans->sChanOffset[j];

			if (!pglFacCfg->GetChannelRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbChan)) 
				return;

    		MyStrnCpy(ChanNames[j], dbChan.sz_chan_name, DB_NAME_LEN);
    		MyStrnCpy(ChanShortNames[j], dbChan.sz_chan_short_name, DB_SHORT_NAME_LEN);
			ChanIDs[j] = pAnalyzeChans->StaID;
			ChanIDs[j].sChanID = pAnalyzeChans->sChanOffset[j];
			//	count how many events are present for the selected channels
			unsigned long ulTempEvents;
			status = GetDataStatisticsForEventsByChannel(ChanIDs[j], dStart, dEnd, &ulTempEvents, ALL_EVENTS);
			ulNumTotalEvents += ulTempEvents;
    	}
    	else 
		{
    		ChanNames[j][0] = '\0';
    		ChanShortNames[j][0] = '\0';
			ChanIDs[j].sStaID = -1;
			ChanIDs[j].sStaType = -1;
			ChanIDs[j].sChanID = -1;
    	}		
	}	
	if ((iNumAnalyze == 0) || (ulNumTotalEvents == 0)) return;	/* no channels at this station selected or no events in time interval */

   /* ------------------------------------------------------------------
	*	Search through all channel events in the database.  If the event is in
	*	one of the channels selected, then print it.  Print a blank line
	*	to separate the "events".
	* ----------------------------------------------------------------*/
	status = ScanChannelEvents(dStart, FIRST, &dbEvent, pAnalyzeChans->StaID);

	if (status != TRUE) return;			   /* no events in db */
	dLastGroupTime = 0.0;
	ulNumEvents = 0;

	// will accumulate sum for the event width, the maximum peak value and the event integral sum
	double dWidthSum = 0;
	double dMaxSum = 0;
	double dIntegralSum = 0;
	double dNetAreaSum = 0;
	double dNumPts = 0;				// sfk 03/31/03
	double dNetArea;
	unsigned long ulNumPts;			// sfk 03/31/03


	//  Create the progress bar
	CGFmtStr Msg1, Msg2;
	TwoLineProgressBar *pTwoLineProgressBar;
	BOOL bContinue = true;

	ulDum = 0;
	Msg1.Printf("Processing station %s.\n", dbSta.sz_sta_name);
	Msg2.Printf("Searched %ld out of %ld events", ulNumEvents, ulNumTotalEvents);
	pTwoLineProgressBar = new TwoLineProgressBar(ulNumTotalEvents, ulNumTotalEvents/20, Msg1, Msg2, "Radiation Review: Event Summary Progress", true, IDI_NEW_RAD_REVIEW);
	if (pTwoLineProgressBar) {
		do {    
			if ((dbEvent.us_event_type == CHANNEL_EVENT) || // 21-Dec-2004  Modified to do channel and bkg, if requested
					((bIncludeBkg == TRUE) && (dbEvent.us_event_type == BACKGROUND_EVENT) && (dbEvent.us_event_type1 == NORMAL_BKG_MEAS))) {
				ulNumEvents++;		// 06-Jan-2005	Count an event only if it is the correct kine
				double dDum;
				for (j=0; j<MAX_NUM_DATA_CHANS; j++) {	
					if ((dbEvent.us_event_chan == ChanIDs[j].sChanID) && 
						(dbEvent.us_event_sta == ChanIDs[j].sStaID) &&
						(dbEvent.us_event_sta_type = ChanIDs[j].sStaType)) {
	    				if ((dLastGroupTime != dbEvent.d_event_grp_time) && (bGroup == TRUE)){
	    					/* print blank line to separate event groups and count them */
	    					pWin->Printf("\n"); 
	    					ulTotalMatchingEvents++;    
	    					dLastGroupTime = dbEvent.d_event_grp_time;
							dMaxTimeOfGroupFirstEvent = dbEvent.d_event_max_time;
	    				}
	    				PrintChanEventResults(ChanIDs[j], &dbEvent, ChanShortNames[j], &ulEventsPrinted,
										 &ulDum, pWin, &DumFile, TRUE, FALSE, FALSE);

						// Calculate the net area of the event
						EventCalculateNetArea(ChanIDs[j], &dbEvent, &dNetArea, &ulNumPts, &dDum, &dDum, &dDum);

						// Accumulate the sums for this event.
						dNetAreaSum += dNetArea;
						dNumPts += (double)ulNumPts;								// sfk 03/31/03
						double dTemp;
						dTemp = (dbEvent.d_event_end_time - dbEvent.d_event_beg_time_key) * 86400.0;
						if (dTemp == 0.0) dTemp = 1.0;
						dWidthSum += dTemp;
						dMaxSum += dbEvent.d_event_max_value;
						dIntegralSum += dbEvent.d_event_integral_sum;
	    			}
				}
			}		
			status = ScanChannelEvents(dStart, NEXT, &dbEvent, pAnalyzeChans->StaID);

			//	Periodically update the progress bar
			Msg2.Printf("Searched %ld out of %ld events", ulNumEvents, ulNumTotalEvents);
			bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);

		} while ((status == TRUE) && (dbEvent.d_event_beg_time_key < dEnd) && bContinue);    
		pTwoLineProgressBar->Close();
		delete pTwoLineProgressBar;
	}
			 
	/* print total events found for this station */                       
	if (ulEventsPrinted > 0) {	//21-Dec-2004	print only if found some matching
		// Print total sums that were calculated
		// print total width and number of points
		pWin->Printf("\n                      Total Sums  %5.0lf (%5.0lf)", dWidthSum, dNumPts);	// sfk 03/31/03

		// Print sum of maximum values in format "most appropriate" for number size
		if (dMaxSum < 10.0) pWin->Printf("%23.4lf ", dMaxSum);							// small number
		if ((dMaxSum < 1000.0) && (dMaxSum >= 10.0)) pWin->Printf("%20.1lf ", dMaxSum);	// medium
		if (dMaxSum >= 1000.0) pWin->Printf("%23.0lf ", dMaxSum);						// large

		// Print sum of integral sums in format "most appropriate" for number size
		if (dIntegralSum < 10.0) pWin->Printf("%21.4lf ", dIntegralSum);								// small number
		if ((dIntegralSum < 1000.0) && (dIntegralSum >= 10.0)) pWin->Printf("%23.1lf ", dIntegralSum);	// medium
		if (dIntegralSum >= 1000.0) pWin->Printf("%21.0lf ", dIntegralSum);								// large

		// Print net area in format "most appropriate" for number size
		if (dNetAreaSum < 10.0) pWin->Printf("%12.4lf ", dNetAreaSum);									// small number
		if ((dNetAreaSum < 1000.0) && (dIntegralSum >= 10.0)) pWin->Printf("%12.1lf ", dNetAreaSum);	// medium
		if (dNetAreaSum >= 1000.0) pWin->Printf("%12.0lf ", dNetAreaSum);								// large

		// Print averages sums that were calculated
		double dNumEvents = (double)ulNumEvents;		// sfk 03/31/03
		pWin->Printf("\n                      Averages    %5.1lf (%5.0lf)", dWidthSum/dNumEvents, dNumPts/dNumEvents);		// sfk 03/31/03

		// Print avg of maximum values in format "most appropriate" for number size
		dMaxSum = dMaxSum/dNumEvents;					// sfk 03/31/03
		if (dMaxSum < 10.0) pWin->Printf("%23.4lf ", dMaxSum);							// small number
		if ((dMaxSum < 1000.0) && (dMaxSum >= 10.0)) pWin->Printf("%20.1lf ", dMaxSum);	// medium
		if (dMaxSum >= 1000.0) pWin->Printf("%23.0lf ", dMaxSum);						// large
		
		// Print avg of integral sums in format "most appropriate" for number size
		dIntegralSum = dIntegralSum/dNumEvents;				// sfk 03/31/03
		if (dIntegralSum < 10.0) pWin->Printf("%21.4lf ", dIntegralSum);								// small number
		if ((dIntegralSum < 1000.0) && (dIntegralSum >= 10.0)) pWin->Printf("%23.1lf ", dIntegralSum);	// medium
		if (dIntegralSum >= 1000.0) pWin->Printf("%21.0lf ", dIntegralSum);								// large
		
		// Print avg net area in format "most appropriate" for number size
		dNetAreaSum = dNetAreaSum/dNumEvents;				// sfk 03/31/03
		if (dNetAreaSum < 10.0) pWin->Printf("%12.4lf ", dNetAreaSum);									// small number
		if ((dNetAreaSum < 1000.0) && (dIntegralSum >= 10.0)) pWin->Printf("%12.1lf ", dNetAreaSum);	// medium
		if (dNetAreaSum >= 1000.0) pWin->Printf("%12.0lf ", dNetAreaSum);								// large
	}

	pWin->Printf("\n\nStation %s Total = %ld Groups, %ld Events\n\n", dbSta.sz_sta_name, ulTotalMatchingEvents, ulEventsPrinted);
	pWin->Printf("______________________________________________________________________\n");


	return;
}    



/*===========================================================================
 *
 *  Name	 :  DailySummaryByChan
 *
 *  Purpose	 :  Display the individual event information about each channel sorted
 *				by channel.	Summarize the number of events for each individual channels
 *				and for each station.
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
 *  08-Aug-2002 SFK		Changed to DATE
 *	03-Oct-2003	SFK		Adapted to new FacMgr
 *	21-Dec-2004	SFK		Added option to include background events in results displayed
===========================================================================*/
void DailySummaryByChan(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, CGUI_TextWin *pWin, BOOL bIncludeBkg)
{
               
    struct db_sta_rec dbSta;
    struct db_chan_rec dbChan;
	struct db_event_rec *pEvents = NULL;
	
	CGFmtStr msg;
    int  status; 
    int  j,k; 
    char ChanNames[MAX_NUM_DATA_CHANS][DB_NAME_LEN+1];
	struct IDStruct ChanIDs[MAX_NUM_DATA_CHANS];
    int iNumAnalyze;                    
    char szDate[DT_LEN+1], szTime[DT_LEN+1];
 	unsigned long ulEventsRead, ulNumEvents;
 	unsigned long ulTotalEventsInInterval = 0; 
 	unsigned long ulTotalEventsInDay = 0; 
 	DATE dDayStart, dIntervalStart;
	unsigned long ulTotalEventsForChannel;

	if (!pglFacCfg->GetStationRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbSta)) return;

   /* ------------------------------------------------------------------
	*	Determine how many channels to analyze at this station.  Read
	*	the associated channel names and save for use later in displaying
	* ----------------------------------------------------------------*/
	iNumAnalyze = 0;
	for (j=0; j<MAX_NUM_DATA_CHANS; j++) {
    	if ((pAnalyzeChans->sChanOffset[j]) != -1) {
			pAnalyzeChans->StaID.sChanID = pAnalyzeChans->sChanOffset[j];
    		iNumAnalyze++;
			if (!pglFacCfg->GetChannelRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbChan)) return;
    		MyStrnCpy(ChanNames[j], dbChan.sz_chan_name, DB_NAME_LEN);
			ChanIDs[j] = pAnalyzeChans->StaID;
			ChanIDs[j].sChanID = pAnalyzeChans->sChanOffset[j];
    	}
    	else {
    		ChanNames[j][0] = '\0';
			ChanIDs[j].sStaID = -1;
    	}		
	}	
	if (iNumAnalyze == 0) return;              /* no channels at this station selected */


   /* ------------------------------------------------------------------
	*	Read all the events in the database for selected channels.
	*  Then print them based on channel.
	* ----------------------------------------------------------------*/
	for (j=0; j<MAX_NUM_DATA_CHANS; j++) {	
		if (pAnalyzeChans->sChanOffset[j] != -1) {
			ulTotalEventsForChannel = 0;
			status = GetDataStatisticsForEventsByChannel(ChanIDs[j], dStart, dEnd, &ulNumEvents, ALL_EVENTS);
			if (status != TRUE) return;
			
			if (ulNumEvents > 0) {
				/* allocate storage for event results to print */
				pEvents = (struct db_event_rec *)malloc(((int)ulNumEvents)*sizeof (struct db_event_rec));
				if (pEvents == NULL) {
					msg.Printf("to allocate storage for %ld events.  Select a smaller time interval to analyze.", ulNumEvents);
					RadReviewMsg(uiMEMORY_ALLOCATION_ERR, (const char *)msg);
					return;
				}	
				
				/* read the events from the db */
				status = GetDataByChannelEvents(ChanIDs[j], dStart, dEnd, ulNumEvents, &ulEventsRead, pEvents, ALL_EVENTS);
				if (status != TRUE) {
					if (pEvents) free(pEvents);
					pEvents = NULL;
					return;
				}
												 
				/* separate the events into days, print the daily summaries */			                                 
				dDayStart = floor(pEvents[0].d_event_beg_time_key);
				dIntervalStart = dStart;

				/* print 0s for days before the first event */
				while (dIntervalStart < dDayStart) {
					glMyDateTime.DATETimestampToDateTimeStrs(dIntervalStart, szDate, szTime);
					pWin->Printf("%s = %5ld\n", szDate, ulTotalEventsInDay);  
					dIntervalStart += 1.0;
				}	
				
				for (k=0; k<(int)ulEventsRead; k++) {
					if ((pEvents[k].us_event_type == CHANNEL_EVENT) || // 21-Dec-2004  Modified to do channel and bkg, if requested
						((bIncludeBkg == TRUE) && (pEvents[k].us_event_type == BACKGROUND_EVENT) && (pEvents[k].us_event_type1 == NORMAL_BKG_MEAS))) {
						if (pEvents[k].d_event_beg_time_key < (dDayStart + 1.0)) {
							ulTotalEventsInDay++;
						}
						else {
							do {
								glMyDateTime.DATETimestampToDateTimeStrs(dDayStart, szDate, szTime);
								pWin->Printf("%s = %5ld\n", szDate, ulTotalEventsInDay);  
								ulTotalEventsInInterval += ulTotalEventsInDay;
								ulTotalEventsForChannel += ulTotalEventsInDay;
								ulTotalEventsInDay = 0;
								dDayStart += 1.0;
							// is this right??} while (pEvents[k].d_event_beg_time_key > (dDayStart + 86399L));
							} while (pEvents[k].d_event_beg_time_key < dDayStart);
							ulTotalEventsInDay++; 
						}
					}	
				}
				// print summary of the last day containing events and then 0s for the rest of the
				// days in the interval
				do {				                              
					glMyDateTime.DATETimestampToDateTimeStrs(dDayStart, szDate, szTime);
					pWin->Printf("%s = %5ld\n", szDate, ulTotalEventsInDay);  
					ulTotalEventsInInterval+=ulTotalEventsInDay;
					ulTotalEventsForChannel += ulTotalEventsInDay;
					dDayStart += 1.0;
					ulTotalEventsInDay = 0;
				} while (dDayStart < dEnd);	
		
				CGUI_App::ProcessEvents();                /* update the output page */
				if (pEvents) free(pEvents);
				pEvents = NULL;
			}
		
			/* Summarize channel events */
			pWin->Printf("Channel %s Total Events = %ld\n", ChanNames[j], ulTotalEventsForChannel); 
			pWin->Printf("\n"); 
		}	
	}	
			 
	/* print total events found for this station */                       
	pWin->Printf("\nStation %s Total Events = %ld\n\n", dbSta.sz_sta_name, ulTotalEventsInInterval); 
	pWin->Printf("______________________________________________________________________\n");
	if (pEvents) free(pEvents);
	pEvents = NULL;
     return;
}    
        
         
         
/*===========================================================================
 *
 *  Name	 :  DailySummaryByTime
 *
 *  Purpose	 :  Search the events in the database by time and display information
 *				from the specified channels.  
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
 *  23-Jan-1996	SFK		Created 
 *	08-Aug-2002	SFK		Changed to DATE
 *	03-Oct-2003	SFK		Adapted to new FacMgr
 *	21-Dec-2004	SFK		Added option to include background events in results displayed
===========================================================================*/
void DailySummaryByTime(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, CGUI_TextWin *pWin, BOOL bIncludeBkg)
{
               
    struct db_sta_rec dbSta;
    struct db_chan_rec dbChan;
	struct db_event_rec dbEvent;
	
    int  status; 
    int  j; 
	
    char ChanNames[MAX_NUM_DATA_CHANS][DB_NAME_LEN+1];
	struct IDStruct ChanIDs[MAX_NUM_DATA_CHANS];
    int iNumAnalyze;
 	unsigned long ulTotalEventsInInterval = 0;
 	unsigned long ulTotalEventsInDay = 0;
 	unsigned long ulEventsPrinted = 0;
 	DATE dDayStart, dIntervalStart;
 	char szDate[DT_LEN+1], szTime[DT_LEN+1];
    
	if (!pglFacCfg->GetStationRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbSta)) return;


   /* ------------------------------------------------------------------
	*	Determine how many channels to analyze at this station.  Read
	*	the associated channel names and save for use later in displaying
	* ----------------------------------------------------------------*/
	iNumAnalyze = 0;
	for (j=0; j<MAX_NUM_DATA_CHANS; j++) {
    	if ((pAnalyzeChans->sChanOffset[j]) != -1) {
			pAnalyzeChans->StaID.sChanID = pAnalyzeChans->sChanOffset[j];
    		iNumAnalyze++;
			if (!pglFacCfg->GetChannelRecord(glsFacNum, &(pAnalyzeChans->StaID), &dbChan)) return;
    		MyStrnCpy(ChanNames[j], dbChan.sz_chan_name, DB_NAME_LEN);
			ChanIDs[j] = pAnalyzeChans->StaID;
			ChanIDs[j].sChanID = pAnalyzeChans->sChanOffset[j];
    	}
    	else {
    		ChanNames[j][0] = '\0';
			ChanIDs[j].sStaID = -1;
    	}		
	}	
	if (iNumAnalyze == 0) return;              /* no channels at this station selected */

   /* ------------------------------------------------------------------
	*	Search through all events in the database.  If the event is in
	*	one of the channels selected, then print it.  Print a blank line
	*	to separate the "events".
	* ----------------------------------------------------------------*/
	status = ScanChannelEvents(dStart, FIRST, &dbEvent);
	if (status != TRUE) return;			   /* no events in db */
	dDayStart = floor(dbEvent.d_event_beg_time_key);
	dIntervalStart = dStart;
	/* print 0s for days before the first event */
	while (dIntervalStart < dDayStart) {
		glMyDateTime.DATETimestampToDateTimeStrs(dIntervalStart, szDate, szTime);
		pWin->Printf("%s = %5ld\n", szDate, ulTotalEventsInDay);  
		dIntervalStart += 1.0;
	}	


	do {    
		for (j=0; j<MAX_NUM_DATA_CHANS; j++) {	
			if ((dbEvent.us_event_chan == ChanIDs[j].sChanID) &&
				(dbEvent.us_event_sta == ChanIDs[j].sStaID) &&
				(dbEvent.us_event_sta_type = ChanIDs[j].sStaType)) {

				if (dbEvent.d_event_beg_time_key < (dDayStart + 1.0)) {
					ulTotalEventsInDay++;
				}
				else {
					do {
						glMyDateTime.DATETimestampToDateTimeStrs(dDayStart, szDate, szTime);
						pWin->Printf("%s = %5ld\n", szDate, ulTotalEventsInDay);  
						ulTotalEventsInInterval+=ulTotalEventsInDay;
						ulTotalEventsInDay = 0;
						dDayStart += 1.0;
					//is this right??} while (dbEvent.d_event_beg_time_key > (dDayStart + 86399L));
					} while (dbEvent.d_event_beg_time_key > (dDayStart + 1.0));

					ulTotalEventsInDay++;
				}
			}	

		}		
		status = ScanChannelEvents(dStart, NEXT, &dbEvent);
	} while ((status == TRUE) && (dbEvent.d_event_beg_time_key < dEnd));
	
	// print summary of the last day containing events and then 0s for the rest of the
	// days in the interval if any
	do {				                              
		glMyDateTime.DATETimestampToDateTimeStrs(dDayStart, szDate, szTime);
		pWin->Printf("%s = %5ld\n", szDate, ulTotalEventsInDay);
		ulTotalEventsInInterval+=ulTotalEventsInDay;  
		dDayStart += 1.0;  
		ulTotalEventsInDay = 0;
	} while (dDayStart < dEnd);	

			 
	/* print total events found for this station */                       
	pWin->Printf("\nStation %s Total Events = %ld \n", dbSta.sz_sta_name, ulTotalEventsInInterval);
	pWin->Printf("Channels searched:\n");
	for (j=0; j<MAX_NUM_DATA_CHANS; j++) {
	   	if (strlen(ChanNames[j]) > 0) pWin->Printf("     %s\n", ChanNames[j]);
	}	
	pWin->Printf("\n______________________________________________________________________\n");
	
    return;
}    

