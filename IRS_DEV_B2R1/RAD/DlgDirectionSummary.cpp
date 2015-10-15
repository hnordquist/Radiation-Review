///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgDirection.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "DateTimeUtils.h"
#include "DbDay.h"
#include "DbDefault.h"
#include "DbEvent.h"
#include "DbGeneralDbSupport.h"
#include "DlgDirection.h"
#include "FacilityConfig.h"
#include "Interval.h"
#include "PrintFacilityHeader.h"
#include "ProgressBars.h"
#include "Utilities.h"
#include "MyDateTime.h"

extern short glsFacNum;
extern char *glnaszIntervalUnitsPicklist[];
extern char *glSummaryTypePicklist[];
extern char *glSummarySortPicklist[];
extern CMyDateTime glMyDateTime;
extern char glszOldDbName[];

static CGUI_TextWin *pWin = NULL;    
//static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = {""};

static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL CloseTextWindow(CGUI_TextWin *pWinArg);


                                 
/////////////////////////////////////////////////////////////////////////////////////
//	UpdateDbParms
//
//	Called from the dialog box when the NewDefaults button is pushed.
//	The present values are read from the dialog box and written to
//	the Default database.
/////////////////////////////////////////////////////////////////////////////////////
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  
    unsigned int iChoice;
        
	if (AskUpdateQuestion() != TRUE) return;
	    
   /* ------------------------------------------------------------------
    *	Get the current parameters set in the dialog box and write
    *	to default database.
    * ----------------------------------------------------------------*/ 
	pDlg->PickListGetValue(IDC_RAD_TYPE, &iChoice);
	status = AccessDefaultDatabase(WRITE, S_RAD_EVENT_SUMM_TYPE, (short *)&iChoice);
	if (status == TRUE) {
	
		pDlg->PickListGetValue(IDC_RAD_SORT, &iChoice);
		status = AccessDefaultDatabase(WRITE, S_RAD_EVENT_SUMM_SORT, (short *)&iChoice);
		if (status != TRUE) goto exit;
    }
	SaveDatabaseNow();
	return;
    
exit:
	RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
	return;
}        
         

/////////////////////////////////////////////////////////////////////////////////////
//	CloseTextWindow
//
//	Allows the user to close the window.
/////////////////////////////////////////////////////////////////////////////////////
static BOOL CloseTextWindow(CGUI_TextWin *pWinArg)
{
	BOOL bCanClose = TRUE;
	pWin = NULL;
	return(bCanClose);
}               

//	sort type picklist
#define SORT_LEN 20
static char *gllSummarySortPicklist[] = {
				    "All Events",
				    "Direction In, Location A",
				    "Direction Out, Location A",
				    "Direction In, Location B",
				    "Direction Out, Location B",
				    "All Location A Events",
				    "All Location B Events",
				    "All Direction In Events",
				    "All Direction Out Events",
				    (char *)0
				    };               
// these definitions correspond to the positions in the gllSummarySortPicklist 
#define ALL 	0
#define IN_A 	1
#define OUT_A   2
#define IN_B 	3
#define OUT_B	4
#define ALL_A   5
#define ALL_B   6
#define ALL_IN  7
#define ALL_OUT 8

// these definistions correspond to the positions in the direction and location picklists
// in dlganlyz.cpp
#define DIRECTION_IN 	0
#define DIRECTION_OUT 1

#define A 0
#define B 1   

                                             
/////////////////////////////////////////////////////////////////////////////////////
//	DirectionSummariesDialog
//
//	Create and display the event summary dialog box.
// 07-20-2001 SFK	2.07 Incorporated ProgressBar classes.  Fixed error where type choice
//						drop down list was gotten from Event Summaries.
/////////////////////////////////////////////////////////////////////////////////////
void DirectionSummariesMenuOption()
{                                 
                                                        
	// these parameters remain unchanged for each execution of the menu option                                                    
    static DATE dUserIntervalStart = 0;
	static unsigned long ulUserInterval = 0;
	static unsigned int iUserUnits = 0;                 
	static DATE dLastSearchIntervalInDays = 0;
	static unsigned int uiEventType;
	static unsigned int uiSortType;    
	static BOOL bFirst = TRUE;
	static short sStaNum; 
	static short sFacNum;
	char szFirstDbDate[DT_LEN+1];              
	char szLastDbDate[DT_LEN+1];              
	static const char *FileTypes[] = {"*.txt", NULL};
    //struct db_fac_rec dbFac;    
    struct GEN_DATE_STRUCT GenDate;
    struct GEN_TIME_STRUCT GenTime;
    struct db_dir_event_rec dbDirEvent;
    
    GUI_ACTION DlgReturn;                                     
    CGFmtStr Msg1, Msg2;
	CGFmtStr szDbInterval;
    
	int status;     
    unsigned long ulDbIntervalInDays;
	DATE dEnd;
	DATE dSearchIntervalInDays;
 	unsigned long ulTotalMatchingDirEvents;
 	unsigned long ulDailySummRecs;
 	DATE dCurrentDay;
 	unsigned long ulTotalDirEventsInDb;
 	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	//CMyDateTime Date;

	TwoLineProgressBar *pTwoLineProgressBar = NULL;
	BOOL bContinue = true;

    
   /* ------------------------------------------------------------------
    *	Always find the first and last days in the database and set
	*	up the strings for display in the dialog box.
    * ----------------------------------------------------------------*/
	SetCurrentDrive();
	status = GetDatabaseIntervalParameters(szFirstDbDate, szLastDbDate, &ulDbIntervalInDays, &dSearchIntervalInDays);
	if (status != TRUE) return;
	if (dSearchIntervalInDays <= 0.0) {
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return;
	}
	
	if ((bFirst == TRUE) || (!SameDb(glszOldDbName)) || (dSearchIntervalInDays != dLastSearchIntervalInDays) || (sFacNum != glsFacNum)) {
	   /* ------------------------------------------------------------------
	    *	Get the default parameters from the Default database.
	    * ----------------------------------------------------------------*/
       	status = AccessDefaultDatabase(READ, S_DFLT_STATION, &sStaNum);
    	if (status == TRUE) {
			status = AccessDefaultDatabase(READ, S_RAD_EVENT_SUMM_SORT, (short *)&uiSortType);
		}	                    
		if (status == TRUE) {
			status = AccessDefaultDatabase(READ, S_RAD_EVENT_SUMM_TYPE, (short *)&uiEventType);
		}
        else {
   			RadReviewMsg(status, "when reading from Default database.  Cannot proceed.");			
			return;
		} 

	   /* ------------------------------------------------------------------
	    *	Calculate the user interval
	    * ----------------------------------------------------------------*/
		GetReviewPeriodParameters(&dUserIntervalStart, &dEnd, &ulUserInterval);
		iUserUnits = 0;

		// remember the current conditions for future entries to this option
		sFacNum = glsFacNum;
	    GetDbName(glszOldDbName);								
		dLastSearchIntervalInDays = dSearchIntervalInDays;	
		bFirst = FALSE;

	}	
	    
//	if (GetFacilityRecord(sFacNum, &dbFac) != TRUE) return;      //KM
	// verify the facility is defined
	char szFacName[DB_NAME_LEN+1];
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) return;
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return;

	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);


   /* ------------------------------------------------------------------
    *	Define the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg Dlg(IDD_SUMMARIES_DIRECTION, NULL, DLG_POSN_NO_SAVE);
	if (Dlg.IsValid()) {
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		Dlg.DefinePushBtn(IDOK, GUI_OK);                   
		Dlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateDbParms);  		       
				                                               
	   /* ------------------------------------------------------------------
	    *	Set facility label in dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefineFldStaticTxt(IDC_RAD_FACILITY);
		Dlg.FldStaticTxtSetValue(IDC_RAD_FACILITY, szFacName);
			
	   /* ------------------------------------------------------------------
	    *	Display current starting ending database dates and interval in dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefineFldStaticTxt(IDC_RAD_CURR_DB_START);
		Dlg.FldStaticTxtSetValue(IDC_RAD_CURR_DB_START, szFirstDbDate);
					
		Dlg.DefineFldStaticTxt(IDC_RAD_CURR_DB_END);
		Dlg.FldStaticTxtSetValue(IDC_RAD_CURR_DB_END, szLastDbDate);

		szDbInterval.Printf("%ld days", ulDbIntervalInDays);  		
		Dlg.DefineFldStaticTxt(IDC_RAD_CURR_DB_INTERVAL);
		Dlg.FldStaticTxtSetValue(IDC_RAD_CURR_DB_INTERVAL, szDbInterval);
		
	   /* ------------------------------------------------------------------
	    *	Set up the search interval parameters in dialog box
	    * ----------------------------------------------------------------*/
		//ConvertSecondsToStructs(&ulUserIntervalStart, &GenDate, &GenTime);
		glMyDateTime.DATETimestampToGenStructs(dUserIntervalStart, &GenDate, &GenTime);
		Dlg.DefineFldDate(IDC_RAD_SRCH_STRTDATE, &GenDate);
		Dlg.DefineFldTime(IDC_RAD_SRCH_STRTTIME, &GenTime);
		
		Dlg.DefineFldNum(IDC_RAD_SRCH_INTERVAL, &ulUserInterval, 1, 10000,"%ld");
		Dlg.DefinePickList(IDC_RAD_INTRVL_UNITS_PL, glnaszIntervalUnitsPicklist, &iUserUnits);

	   /* ------------------------------------------------------------------
	    *	Set up the output display options in dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefinePickList(IDC_RAD_TYPE, glSummaryTypePicklist, &uiEventType);
		Dlg.DefinePickList(IDC_RAD_SORT, gllSummarySortPicklist, &uiSortType);
			                          
	   /* ------------------------------------------------------------------
	    *	Set up the focus and display the dialog box
	    * ----------------------------------------------------------------*/
		Dlg.SetFocus(IDC_RAD_SRCH_INTERVAL);   
		DlgReturn = Dlg.Go();
			    
	   /* ------------------------------------------------------------------
	    *	Returned from dialog box, now process it.
	    * ----------------------------------------------------------------*/
	    if (DlgReturn == GUI_OK) {  
			    	
	    	Dlg.RetrieveAllControlValues();	/* get dialog values */
	    	
			status = ConvertStructsToSearchIntervalParameters(&GenDate, &GenTime, iUserUnits, ulUserInterval, &dUserIntervalStart, &dEnd);
			if (status != TRUE) {	// no data for any station in the interval
				RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, dUserIntervalStart, dEnd, "any station");
				return;
			}
			
		   /* ------------------------------------------------------------------
		    *	Open up the text window where the events will be displayed
		    * ----------------------------------------------------------------*/
			if (pWin == NULL) {                
				pWin = new CGUI_TextWin("Summary of Direction Events Found");
				if (pWin != NULL) pWin->ShowStatusBar(TRUE);
			}   
			else {
				pWin->Clear();
			}
			pWin->SetFocus();			
			
		   /* ------------------------------------------------------------------
			*   Display the general part of the header:
			*	facility name,
			*	search interval
			* ----------------------------------------------------------------*/
			if (!PrintFacilityHeader(pWin, sFacNum, dUserIntervalStart, dEnd)) return;
		                          
   		   /* ------------------------------------------------------------------
			*   Calculate how many events are in the interval in the db
			*   If there are no events, then stop now
			* ----------------------------------------------------------------*/
		    status = ScanDirEventRecs(dUserIntervalStart, FIRST, &dbDirEvent);
		    if (status == uiSUMMARY_NOT_FOUND_STATUS) {
	    		pWin->Printf("\n\nNo direction events found in database\n");
				pWin->Printf("______________________________________________________________________\n");
	    		if (pWin != NULL) pWin->EnableUserClose(CloseTextWindow);
	    		return;
			} else {
				if (status != TRUE) { // uiUNKNOWN_EVENT_TYPE_ERR or uiSUMMARY_NOT_FOUND_STATUS
					RadReviewMsg(status); // uiUNKNOWN_EVENT_TYPE_ERR or uiSUMMARY_NOT_FOUND_STATUS
					return;
				}
			}

			// Determine total number of events in db
			ulTotalMatchingDirEvents = 0;
			ulTotalDirEventsInDb = 0;
			do {
				status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);  
				ulTotalDirEventsInDb++;
			} while (status == TRUE);	
			if (status == uiDB_BAD_ERR) {
				RadReviewMsg(status);// uiUNKNOWN_EVENT_TYPE_ERR or uiSUMMARY_NOT_FOUND_STATUS
			    if (pWin != NULL) pWin->EnableUserClose(CloseTextWindow);
				return;			   /* unexpected db error */
			}	
	
   		   	
		   /* ------------------------------------------------------------------
		    *	Define a progress dialog box
		    * ----------------------------------------------------------------*/
			//Msg1.Printf("Processing facility %s.", dbFac.sz_fac_name);
			Msg1.Printf("Processing facility %s.", szFacName);
			Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
			pTwoLineProgressBar = new TwoLineProgressBar(ulTotalDirEventsInDb, ulTotalDirEventsInDb/20, Msg1, Msg2, "Radiation Review: Direction Search Progress");
			if (pTwoLineProgressBar) {
								
			   /* ------------------------------------------------------------------
				*   Based on the choices specified, provide the appropriate summaries
				* ----------------------------------------------------------------*/
				status = ScanDirEventRecs(dUserIntervalStart, FIRST, &dbDirEvent);
				ulTotalMatchingDirEvents = 0;

				switch (uiEventType) {
					case ALL_EVENT_SUMMARIES:
						switch (uiSortType) {
							case ALL:
								pWin->Printf("Search Method: all direction events\n");
								pWin->Printf("______________________________________________________________________\n");
				 				while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {
									if (status == TRUE) {
										PrintDirEventToTextWin(&dbDirEvent, pWin, ulTotalMatchingDirEvents); 
										ulTotalMatchingDirEvents++;
									}
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}	
							break;	
							
							case IN_A:
								pWin->Printf("Search Method: all direction events moving in at location A\n");
								pWin->Printf("______________________________________________________________________\n");
				 				while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {
									if ((status == TRUE) && 
									  (dbDirEvent.us_dir_event_direction == DIRECTION_IN) && (dbDirEvent.us_dir_event_location == A)) {
										PrintDirEventToTextWin(&dbDirEvent, pWin, ulTotalMatchingDirEvents);
										ulTotalMatchingDirEvents++;
									}
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}	
							break;		 
							
							case IN_B:
								pWin->Printf("Search Method: all direction events moving in at location B\n");
								pWin->Printf("______________________________________________________________________\n");
				 				while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {
									if ((status == TRUE) && 
										(dbDirEvent.us_dir_event_direction == DIRECTION_IN) && (dbDirEvent.us_dir_event_location == B)) {
										PrintDirEventToTextWin(&dbDirEvent, pWin, ulTotalMatchingDirEvents);
										ulTotalMatchingDirEvents++;
									}
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}	
							break;		 
							
							case OUT_A:
								pWin->Printf("Search Method: all direction events moving out at location A\n");
								pWin->Printf("______________________________________________________________________\n");
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {
									if ((status == TRUE) && 
									  (dbDirEvent.us_dir_event_direction == DIRECTION_OUT) && (dbDirEvent.us_dir_event_location == A)) {
										PrintDirEventToTextWin(&dbDirEvent, pWin, ulTotalMatchingDirEvents);
										ulTotalMatchingDirEvents++;
									}
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}	
							break;		 
							
							case OUT_B:
								pWin->Printf("Search Method: all direction events moving out at location B\n");
								pWin->Printf("______________________________________________________________________\n");
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {
									if ((status == TRUE) && (dbDirEvent.us_dir_event_direction == DIRECTION_OUT) && (dbDirEvent.us_dir_event_location == B)) {
										PrintDirEventToTextWin(&dbDirEvent, pWin, ulTotalMatchingDirEvents);
										ulTotalMatchingDirEvents++;
									}	
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}	
							break;		 
							
							case ALL_A:
								pWin->Printf("Search Method: all direction events at location A\n");
								pWin->Printf("______________________________________________________________________\n");
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {
									if ((status == TRUE)  && (dbDirEvent.us_dir_event_location == A)) {
										PrintDirEventToTextWin(&dbDirEvent, pWin, ulTotalMatchingDirEvents);
										ulTotalMatchingDirEvents++;
									}
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}	
							break;		 
							
							case ALL_B:
								pWin->Printf("Search Method: all direction events at location B\n");
								pWin->Printf("______________________________________________________________________\n");
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {
									if ((status == TRUE) && (dbDirEvent.us_dir_event_location == B)) {
										PrintDirEventToTextWin(&dbDirEvent, pWin, ulTotalMatchingDirEvents);
										ulTotalMatchingDirEvents++;
									}
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}	
							break;		 
							
							case ALL_IN:
								pWin->Printf("Search Method: all direction events moving in\n");
								pWin->Printf("______________________________________________________________________\n");
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {
									if ((status == TRUE) && (dbDirEvent.us_dir_event_direction == DIRECTION_IN)) {
										PrintDirEventToTextWin(&dbDirEvent, pWin, ulTotalMatchingDirEvents);
										ulTotalMatchingDirEvents++;
									}	
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}	
							break;	
							
							case ALL_OUT:
								pWin->Printf("Search Method: all direction events moving out B\n");
								pWin->Printf("______________________________________________________________________\n");
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {
									if ((status == TRUE) && (dbDirEvent.us_dir_event_direction == DIRECTION_OUT)) {
										PrintDirEventToTextWin(&dbDirEvent, pWin, ulTotalMatchingDirEvents);
										ulTotalMatchingDirEvents++;
									}
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}	
							break;	 
						}		 
						break;
																  
					case DAILY_SUMMARIES:
						ulTotalMatchingDirEvents = 0;	
						ulDailySummRecs = 0;
						switch (uiSortType) {
							case ALL:
								pWin->Printf("Search Method: daily summaries of all direction events\n");
								pWin->Printf("______________________________________________________________________\n");
								pWin->Printf("\nDate          Summaries");		    
									    			 //96.10.10 = 1234
								dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {					
									if (status == TRUE) {
										//if (!SameDay(dCurrentDay, dbDirEvent.d_dir_event_beg_time_key)) {
										if (!((int)dCurrentDay == (int)dbDirEvent.d_dir_event_beg_time_key)) {
											glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
											pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
											dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
											ulDailySummRecs = 1;
										}
										else {
											ulDailySummRecs++;
										}	    
										ulTotalMatchingDirEvents++;
									}	
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}
								if (ulDailySummRecs > 0) {
									glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
									pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
								}	
							break;	
							
							case IN_A:
								pWin->Printf("Search Method: daily summaries of all direction events moving in at location A\n");
								pWin->Printf("______________________________________________________________________\n");
								pWin->Printf("\nDate          Summaries");		    
									    			 //96.10.10 = 1234
								dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {					
									if ((status == TRUE) && (dbDirEvent.us_dir_event_direction == DIRECTION_IN) && (dbDirEvent.us_dir_event_location == A)) {
										if (!((int)dCurrentDay == (int)dbDirEvent.d_dir_event_beg_time_key)) {
											glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
											pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
											dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
											ulDailySummRecs = 1;
										}
										else {
											ulDailySummRecs++;
										}	    
										ulTotalMatchingDirEvents++;
									}	
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}
								if (ulDailySummRecs > 0) {
									glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
									pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
								}	
							break;	
							
							case IN_B:
								pWin->Printf("Search Method: daily summaries of all direction events moving in at location B\n");
								pWin->Printf("______________________________________________________________________\n");
								pWin->Printf("\nDate          Summaries");		    
									    			 //96.10.10 = 1234
								dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {					
									if ((status == TRUE) && (dbDirEvent.us_dir_event_direction == DIRECTION_IN) &&
										(dbDirEvent.us_dir_event_location == B)) {
										if (!((int)dCurrentDay == (int)dbDirEvent.d_dir_event_beg_time_key)) {
											glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
											pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
											dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
											ulDailySummRecs = 1;
										}
										else {
											ulDailySummRecs++;
										}	    
										ulTotalMatchingDirEvents++;
									}	
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}
								if (ulDailySummRecs > 0) {
									glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
									pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
								}	
							break;	
							
							case OUT_A:
								pWin->Printf("Search Method: daily summaries of all direction events moving out at location A\n");
								pWin->Printf("______________________________________________________________________\n");
								pWin->Printf("\nDate          Summaries");		    
									    			 //96.10.10 = 1234
								dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {					
									if ((status == TRUE) && (dbDirEvent.us_dir_event_direction == DIRECTION_OUT) && 
										(dbDirEvent.us_dir_event_location == A)) {
										if (!((int)dCurrentDay == (int)dbDirEvent.d_dir_event_beg_time_key)) {
											glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
											pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
											dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
											ulDailySummRecs = 1;
										}
										else {
											ulDailySummRecs++;
										}	    
										ulTotalMatchingDirEvents++;
									}	
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}
								if (ulDailySummRecs > 0) {
									glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
									pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
								}	
							break;	
							
							case OUT_B:
								pWin->Printf("Search Method: daily summaries of all direction events moving out at location B\n");
								pWin->Printf("______________________________________________________________________\n");
								pWin->Printf("\nDate          Summaries");		    
									    			 //96.10.10 = 1234
								dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {					
									if ((status == TRUE) && (dbDirEvent.us_dir_event_direction == DIRECTION_OUT) && (dbDirEvent.us_dir_event_location == B)) {
										if (!((int)dCurrentDay == (int)dbDirEvent.d_dir_event_beg_time_key)) {
											glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
											pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
											dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
											ulDailySummRecs = 1;
										}
										else {
											ulDailySummRecs++;
										}	    
										ulTotalMatchingDirEvents++;
									}	
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent); 							}
								if (ulDailySummRecs > 0) {
									glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
									pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
								}	
							break;	
							
							case ALL_A:
								pWin->Printf("Search Method: daily summaries of all direction events at location A\n");
								pWin->Printf("______________________________________________________________________\n");
								pWin->Printf("\nDate          Summaries");		    
									    			 //96.10.10 = 1234
								dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {					
									if ((status == TRUE)  && (dbDirEvent.us_dir_event_location == A)) {
										if (!((int)dCurrentDay == (int)dbDirEvent.d_dir_event_beg_time_key)) {
											glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
											pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
											dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
											ulDailySummRecs = 1;
										}
										else {
											ulDailySummRecs++;
										}	    
										ulTotalMatchingDirEvents++;
									}	
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}
								if (ulDailySummRecs > 0) {
									glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
									pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
								}	
							break;	
							
							case ALL_B:
								pWin->Printf("Search Method: daily summaries of all direction events at location B\n");
								pWin->Printf("______________________________________________________________________\n");
								pWin->Printf("\nDate          Summaries");		    
									    			 //96.10.10 = 1234
								dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {					
									if ((status == TRUE) && (dbDirEvent.us_dir_event_location == B)) {
										if (!((int)dCurrentDay == (int)dbDirEvent.d_dir_event_beg_time_key)) {
											glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
											pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
											dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
											ulDailySummRecs = 1;
										}
										else {
											ulDailySummRecs++;
										}	    
										ulTotalMatchingDirEvents++;
									}	
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}
								if (ulDailySummRecs > 0) {
									glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
									pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
								}	
							break;	
							
							case ALL_IN:
								pWin->Printf("Search Method: daily summaries of all direction events moving in\n");
								pWin->Printf("______________________________________________________________________\n");
								pWin->Printf("\nDate          Summaries");		    
									    			 //96.10.10 = 1234
								dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {					
									if ((status == TRUE) && (dbDirEvent.us_dir_event_direction == DIRECTION_IN)) {
										if (!((int)dCurrentDay == (int)dbDirEvent.d_dir_event_beg_time_key)) {
											glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
											pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
											dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
											ulDailySummRecs = 1;
										}
										else {
											ulDailySummRecs++;
										}	    
										ulTotalMatchingDirEvents++;
									}	
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}
								if (ulDailySummRecs > 0) {
									glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
									pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
								}	
							break;	
							
							case ALL_OUT:
								pWin->Printf("Search Method: daily summaries of all direction events moving out B\n");
								pWin->Printf("______________________________________________________________________\n");
								pWin->Printf("\nDate          Summaries");		    
									    			 //96.10.10 = 1234
								dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
								while ((status == TRUE) && (dbDirEvent.d_dir_event_beg_time_key < dEnd) && bContinue) {					
									if ((status == TRUE) && (dbDirEvent.us_dir_event_direction == DIRECTION_OUT)) {
										if (!((int)dCurrentDay == (int)dbDirEvent.d_dir_event_beg_time_key)) {
											glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
											pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
											dCurrentDay = dbDirEvent.d_dir_event_beg_time_key;
											ulDailySummRecs = 1;
										}
										else {
											ulDailySummRecs++;
										}	    
										ulTotalMatchingDirEvents++;
									}	
									Msg2.Printf("Searched %ld of %ld events", ulTotalMatchingDirEvents, ulTotalDirEventsInDb);
									bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
									status = ScanDirEventRecs(dUserIntervalStart, NEXT, &dbDirEvent);
								}
								if (ulDailySummRecs > 0) {
									glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
									pWin->Printf("\n%s = %4ld  ", szDate,  ulDailySummRecs);
								}	
							break;	
						}		 
					break;
				}// end of the big switch		
			}

			pTwoLineProgressBar->Close();
			delete pTwoLineProgressBar;

			pWin->Printf("\nTotal Direction Summaries Found = %lu\n", ulTotalMatchingDirEvents);		    			
		    pWin->Printf("______________________________________________________________________\n");
		    if (pWin != NULL) pWin->EnableUserClose(CloseTextWindow);
		}	
	}	
}   		                                 
   		                                 
