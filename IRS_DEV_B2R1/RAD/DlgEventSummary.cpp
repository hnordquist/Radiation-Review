////////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgEventSummary.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		CleanUpEventSummary
//		EventsSummariesMenuOption
//
//	Static
//		UpdateDbParms
//		CloseRawDataWindow
//		SelectAll
//		DeselectAll
//
///////////////////////////////////////////////////////////////////////////

#include "gui_tbl.h"
#include "gen.h"
#include "RAD.H"
#include "DateTimeUtils.h"
#include "DbDefault.h"
#include "DbEvent.h"
#include "DbGeneralDbSupport.h"
#include "DbInterval.h"
#include "DlgEventSummary.h"
#include "EventSummary.h"
#include "FacilityConfig.h"
#include "Interval.h"
#include "PrintFacilityHeader.h"
#include "Utilities.h"
#include "MyDateTime.h"
#include "TypeDefinitions.h"

extern short glsFacNum;
extern char *glnaszIntervalUnitsPicklist[];
extern char *glSummaryTypePicklist[];
extern char *glSummarySortPicklist[];
extern CMyDateTime glMyDateTime;
extern char glszOldDbName[];

static int glliNumRows;    
static unsigned int glluiCheckBoxCol;
static unsigned int glluiChanCol;                 
static BOOL *pgllSelect1 = NULL;
static char **aszChanList = NULL;
static CGUI_TextWin *pWin = NULL;
static ANALYZE_PARMS *pAnalyzeChans = NULL;    

static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL CloseRawDataWindow(CGUI_TextWin *pWinArg);
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);


                    
/* ------------------------------------------------------------------
 *	Called from the dialog box when the NewDefaults button is pushed.
 *	The present values are read from the dialog box and written to
 *	the Default database.
 * ----------------------------------------------------------------*/
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	if (!AskUpdateQuestion())
		return;

    int status;  
	CGUI_Table *pTable;
    int i;
    struct db_chan_event_dflt_rec dbChanDflt;
    short sNum;
    unsigned int iChoice;
    BOOL bBool;
	    
/* ------------------------------------------------------------------
    *	Get the current parameters set in the dialog box and write
    *	to default database.
    * ----------------------------------------------------------------*/ 
    pDlg->FldNumGetValue(IDC_CHAN_EVENT_SUMM_MIN_SEPARATION, &sNum);
	status = AccessDefaultDatabase(WRITE, S_CHAN_EVENT_SUMM_MIN, &sNum);
	if (status != TRUE) goto exit;
	
    pDlg->FldNumGetValue(IDC_CHAN_EVENT_SUMM_MAX_SEPARATION, &sNum);
	status = AccessDefaultDatabase(WRITE, S_CHAN_EVENT_SUMM_MAX, &sNum);
	if (status != TRUE) goto exit;
	
	pDlg->PickListGetValue(IDC_CHAN_EVENT_SUMM_TYPE, &iChoice);
	status = AccessDefaultDatabase(WRITE, S_CHAN_EVENT_SUMM_TYPE, (short *)&iChoice);
	if (status != TRUE) goto exit;
	
	pDlg->PickListGetValue(IDC_CHAN_EVENT_SUMM_SORT, &iChoice);
	status = AccessDefaultDatabase(WRITE, S_CHAN_EVENT_SUMM_SORT, (short *)&iChoice);
	if (status != TRUE) goto exit; 
	
	bBool = FALSE;
	status = AccessDefaultDatabase(WRITE, B_CHAN_EVENT_SUMM_LOG_FILE, (int*)&bBool);	
	if (status != TRUE) 
		goto exit;	

   /* ------------------------------------------------------------------
    *	Read the current table entry values and update the default
    *	parms in the db.
    * ----------------------------------------------------------------*/
	pDlg->GetTable(IDC_CHAN_EVENT_SUMM_PICKTABLE, &pTable);

	if (!pTable)
	{
		goto exit;
	}
	else 
	{
		IDStruct ChanID;

		pTable->RetrieveAllCellValues();

	    for (i=0; i<glliNumRows; i++) 
		{
			if (!pglFacCfg->GetChannelIDForPicklistChoice(i, &ChanID)) 
				goto exit;

			dbChanDflt.s_chan_event_chan_num = ChanID.sChanID;
			dbChanDflt.s_chan_event_sta_num = ChanID.sStaID;
			dbChanDflt.s_chan_event_sta_type = ChanID.sStaType;

			status = AccessDefaultDatabase(READ, 0, &dbChanDflt); 
			if (status != TRUE) 
				goto exit;
			dbChanDflt.b_chan_event_summ_select = pgllSelect1[i];

			status = AccessDefaultDatabase(WRITE, 0, &dbChanDflt); 
			if (status != TRUE) 
				goto exit;
	    }	  
    }
	SaveDatabaseNow();
    return;

exit:
	RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
	return;

}        
         

/* ------------------------------------------------------------------
 *	Allows the user to close the window.
 * ----------------------------------------------------------------*/
static BOOL CloseRawDataWindow(CGUI_TextWin *pWinArg)
{
	BOOL bCanClose = TRUE;
	pWin = NULL;
	return(bCanClose);
}               
       
                    
/* ------------------------------------------------------------------
 *	Called from the dialog box when the Select button is pushed.
 *	Checks all the boxes in the Select column
 * ----------------------------------------------------------------*/
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable;
    int i;
    
	pDlg->GetTable(IDC_CHAN_EVENT_SUMM_PICKTABLE, &pTable);
	if (pTable != NULL) 
	{
	    for (i=0; i<glliNumRows; i++)
	    	pgllSelect1[i] = TRUE;	
	    
        pTable->UpdateCol(glluiCheckBoxCol);
    }
}              
/* ------------------------------------------------------------------
 *	Called from the dialog box when the Deselect button is pushed.
 *	Unchecks all the boxes in the Select column
 * ----------------------------------------------------------------*/
static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable = NULL;
		
	pDlg->GetTable(IDC_CHAN_EVENT_SUMM_PICKTABLE, &pTable);

	if (pTable != NULL)
	{
	    for (int i = 0; i < glliNumRows; i++)
	    	pgllSelect1[i] = FALSE;
	
        pTable->UpdateCol(glluiCheckBoxCol);
    }
}        

                                                
/* ------------------------------------------------------------------
 *	Deallocate storage used in the Event Summary dialog box.
 * ----------------------------------------------------------------*/
void CleanUpEventSummary()
{				          
	delete [] pgllSelect1;
 	pgllSelect1 = NULL;

	delete [] pAnalyzeChans;
	pAnalyzeChans = NULL;

	if (aszChanList) 
		gen_deallocate_str_array(aszChanList);
	aszChanList = NULL;
}	

                

/* ------------------------------------------------------------------
 *	Create and display the event summary dialog box.
 * ----------------------------------------------------------------*/
void EventsSummariesMenuOption()
{                                 
	// these parameters remain unchanged for each execution of the menu option                                                    
    static DATE dUserIntervalStart = 0.0;
	static unsigned long ulUserInterval = 0;
	static unsigned int iUserUnits = 0;                 
	static unsigned int uiEventType;
	static unsigned int uiSortType;    
	static unsigned long ulMinDelta; 
	static unsigned long ulMaxDelta; 
	static DATE dLastSearchIntervalInDays = 0.0; 
	//static BOOL bFirst = TRUE;
	static short sStaNum;
	static short sFacNum;
//	static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = {""};
	static short sMaxNameLen;
	static BOOL bIncludeBkg = FALSE;

	const char *FileTypes[] = {"*.txt", NULL};

	char szFirstDate[DT_LEN+1];              
	char szLastDate[DT_LEN+1];              
	CGFmtStr szInterval;
	
    db_chan_rec dbChan;

	pAnalyzeChans = NULL;
	pgllSelect1 = NULL;
    db_chan_event_dflt_rec dbChanDflt;   
    GEN_DATE_STRUCT GenDate;
    GEN_TIME_STRUCT GenTime;

    GUI_ACTION DlgReturn;                                     
    CGFmtStr msg;
    CGUI_Table *pTbl; 
    
	int status,i,j;    
	short sNumSta,sNumChans;

	unsigned long ulDbIntervalInDays;
	DATE dSearchIntervalInDays, dEnd, dDum;
	unsigned long ulTotalPts, ulPts;
	unsigned long ulNumTotalEvents, ulNumEventsPrinted;
	unsigned long ulEventPts;
	
	FILE *config_handle = NULL;
    BOOL  bGroup;
	IDStruct ChanID;

	// verify the facility is defined
	char szFacName[DB_NAME_LEN+1];

	if (!pglFacCfg->IsFacilityDefined(glsFacNum) || !pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) 
		return;

	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);
  
   /* ------------------------------------------------------------------
    *	Determine the database interval
    * ----------------------------------------------------------------*/
	SetCurrentDrive();
	status = GetDatabaseIntervalParameters(szFirstDate, szLastDate, &ulDbIntervalInDays, &dSearchIntervalInDays);
	if (status != 1) 
		return;

	if (dSearchIntervalInDays <= 0.0) 
	{
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return;
	}
	/*  THIS BLEW UP ON THE SECOND TIME AROUND.  CHANGED IT SO IT WOULD READ
	    THE DATA IT NEEDED EVERY TIME.
	if ((bFirst == TRUE) || 
		!SameDb(glszOldDbName) || 
		(dSearchIntervalInDays != dLastSearchIntervalInDays) ||
		(sFacNum != glsFacNum))
	*/
	{
		CleanUpEventSummary();

	   /* ------------------------------------------------------------------
	    *	Get the default parameters from the Default database.
	    * ----------------------------------------------------------------*/
		if (((AccessDefaultDatabase(READ, S_CHAN_EVENT_CHAN_NUM, &ChanID.sChanID)) ||
			 (AccessDefaultDatabase(READ, S_CHAN_EVENT_STA_NUM, &ChanID.sStaID)) ||
       		 (AccessDefaultDatabase(READ, S_CHAN_EVENT_STA_TYPE, &ChanID.sStaType)) ||
			 (AccessDefaultDatabase(READ, S_CHAN_EVENT_SUMM_SORT, (short *)&uiSortType)) ||
			 (AccessDefaultDatabase(READ, S_CHAN_EVENT_SUMM_TYPE, (short *)&uiEventType)) ||
			 (AccessDefaultDatabase(READ, S_CHAN_EVENT_SUMM_MIN, (short *)&ulMinDelta)) ||
			 (AccessDefaultDatabase(READ, S_CHAN_EVENT_SUMM_MAX, (short *)&ulMaxDelta))) == FALSE) 
		{
   			RadReviewMsg(status, "when reading from Default database.  Cannot proceed.");			
			return;
		} 
		
	   /* ------------------------------------------------------------------
	    *	Create channel picklist of all channels in this facility
	    * ----------------------------------------------------------------*/
		sFacNum = glsFacNum;

		int iLen;
		if (!pglFacCfg->CreateChannelPicklist(glsFacNum, true, &aszChanList, &iLen)) 
			return;

		sMaxNameLen = (short)iLen;
		glliNumRows = pglFacCfg->GetNumChannelsForFacility(glsFacNum);
				
	   /* ------------------------------------------------------------------
	    *	Allocate storage for the channel list in the table
	    * ----------------------------------------------------------------*/
		pgllSelect1 = new BOOL[glliNumRows];
	    //pgllSelect1 = (BOOL *)malloc(glliNumRows*sizeof(BOOL));
	    if (pgllSelect1 == NULL) 
		{
			pglFacCfg->DestroyPicklist(aszChanList);
			aszChanList = NULL;
		 	return;  //no leak here
		} 	
	   /* ------------------------------------------------------------------
	    *	Fill in the default values for the table of whether or not selected
	    * ----------------------------------------------------------------*/
		for (i=0; i<glliNumRows; i++) 
		{
			if (!pglFacCfg->GetChannelIDForPicklistChoice(i, &ChanID))
			{
				CleanUpEventSummary();	//fixed leaks pjm 2-27-08
				return;
			}

			dbChanDflt.s_chan_event_chan_num = ChanID.sChanID;
			dbChanDflt.s_chan_event_sta_num = ChanID.sStaID;
			dbChanDflt.s_chan_event_sta_type = ChanID.sStaType;
			status = AccessDefaultDatabase(READ, 0, &dbChanDflt); 

			if (status != 1) 
			{
				pglFacCfg->DestroyPicklist(aszChanList);
				aszChanList = NULL;
				CleanUpEventSummary();	//fixed leaks pjm 2-27-08
				return;
			} 	
			pgllSelect1[i] = dbChanDflt.b_chan_event_summ_select;
		}		
	    
	   /* ------------------------------------------------------------------
	    *	Calculate the user interval
	    * ----------------------------------------------------------------*/
		GetReviewPeriodParameters(&dUserIntervalStart, &dDum, &ulUserInterval);
		iUserUnits = 0;

		// remember the current conditions for future entries to this option
	    GetDbName(glszOldDbName);								
		dLastSearchIntervalInDays = dSearchIntervalInDays;	
		//bFirst = FALSE; //THIS BLEW UP ON THE SECOND TIME AROUND.  CHANGED IT SO IT WOULD READ
						  //THE DATA IT NEEDED EVERY TIME  PJM April 8, 2008
	}	
	sFacNum = glsFacNum;
	GetDbName(glszOldDbName);		/* remember for next execution of this option */    
   /* ------------------------------------------------------------------
    *	Define the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg Dlg(IDD_SUMMARIES_EVENTS, NULL, DLG_POSN_NO_SAVE);

	if (Dlg.IsValid()) 
	{
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		Dlg.DefinePushBtn(IDOK, GUI_OK);                   
		Dlg.DefinePushBtn(IDC_CHAN_EVENT_SUMM_ALL_BUTTON, GUI_NONE, SelectAll);          
		Dlg.DefinePushBtn(IDC_CHAN_EVENT_SUMM_NONE_BUTTON, GUI_NONE, DeselectAll);
		Dlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateDbParms);  		       
				                                               
	   /* ------------------------------------------------------------------
	    *	Set facility label in dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefineFldStaticTxt(IDC_CHAN_EVENT_SUMM_FACILITY);
		Dlg.FldStaticTxtSetValue(IDC_CHAN_EVENT_SUMM_FACILITY, szFacName);
			
	   /* ------------------------------------------------------------------
	    *	Display current starting ending database dates and interval in dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefineFldStaticTxt(IDC_CHAN_EVENT_SUMM_CURR_DB_START);
		Dlg.FldStaticTxtSetValue(IDC_CHAN_EVENT_SUMM_CURR_DB_START, szFirstDate);
					
		Dlg.DefineFldStaticTxt(IDC_CHAN_EVENT_SUMM_CURR_DB_END);
		Dlg.FldStaticTxtSetValue(IDC_CHAN_EVENT_SUMM_CURR_DB_END, szLastDate);
			
		szInterval.Printf("%ld days", ulDbIntervalInDays);  
		Dlg.DefineFldStaticTxt(IDC_CHAN_EVENT_SUMM_CURR_DB_INTERVAL);
		Dlg.FldStaticTxtSetValue(IDC_CHAN_EVENT_SUMM_CURR_DB_INTERVAL, szInterval);
		
	   /* ------------------------------------------------------------------
	    *	Set up the search interval parameters in dialog box
	    * ----------------------------------------------------------------*/
		//Date.DATETimestampToGenStructs(dUserIntervalStart, &GenDate, &GenTime);
		glMyDateTime.DATETimestampToGenStructs(dUserIntervalStart, &GenDate, &GenTime);
		Dlg.DefineFldDate(IDC_CHAN_EVENT_SUMM_SRCH_STRTDATE, &GenDate);
		Dlg.DefineFldTime(IDC_CHAN_EVENT_SUMM_SRCH_STRTTIME, &GenTime);
		
		Dlg.DefineFldNum(IDC_CHAN_EVENT_SUMM_SRCH_INTERVAL, &ulUserInterval, 1, 10000,"%ld");
		Dlg.DefinePickList(IDC_CHAN_EVENT_SUMM_INTRVL_UNITS_PL, glnaszIntervalUnitsPicklist, &iUserUnits);

	   /* ------------------------------------------------------------------
	    *	Set up the output display options in dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefinePickList(IDC_CHAN_EVENT_SUMM_TYPE, glSummaryTypePicklist, &uiEventType);
		Dlg.DefinePickList(IDC_CHAN_EVENT_SUMM_SORT, glSummarySortPicklist, &uiSortType);
		Dlg.DefineFldNum(IDC_CHAN_EVENT_SUMM_MIN_SEPARATION, &ulMinDelta, 0, 2000000000,"%ld");
		Dlg.DefineFldNum(IDC_CHAN_EVENT_SUMM_MAX_SEPARATION, &ulMaxDelta, 0, 2000000000,"%ld");
			    
	   /* ------------------------------------------------------------------
	    *	Set up the output display options in dialog box
	    * ----------------------------------------------------------------*/
		//Dlg.DefineCheckBox(IDC_CHAN_EVENT_SUMM_LOG_FILE, &bLogFile);
		//	21-Dec-2004	SFK Include a checkbox for user to request background events also displayed
		Dlg.DefineCheckBox(IDC_INCLUDE_BKG, &bIncludeBkg);
				
	   /* ------------------------------------------------------------------
	    *	Set up the table
	    * ----------------------------------------------------------------*/
		int iLen;

		if (!pglFacCfg->CreateChannelPicklist(glsFacNum, true, &aszChanList, &iLen))
		{
				CleanUpEventSummary();	//fixed leaks pjm 2-27-08
				return;
		}

		Dlg.DefineTable(IDC_CHAN_EVENT_SUMM_PICKTABLE, &pTbl);	
		if (pTbl) 
		{
			glluiCheckBoxCol = pTbl->AddColCheckBox("Select", glliNumRows, pgllSelect1);		
			glluiChanCol = pTbl->AddColTxt("Channel", glliNumRows, aszChanList, sMaxNameLen+1);
			pTbl->SetColWriteEnable(glluiChanCol, FALSE);
		}	
			
	   /* ------------------------------------------------------------------
	    *	Set up the focus and display the dialog box
	    * ----------------------------------------------------------------*/
		Dlg.SetFocus(IDC_CHAN_EVENT_SUMM_SRCH_INTERVAL);   
		DlgReturn = Dlg.Go();
			    
	   /* ------------------------------------------------------------------
	    *	Returned from dialog box, now process it.
	    * ----------------------------------------------------------------*/
	    if (DlgReturn == GUI_OK) 
		{      	
	    	Dlg.RetrieveAllControlValues();	/* get dialog values */
	    	pTbl->RetrieveAllCellValues(); 		        /* get table values */
	    	
		   /* ------------------------------------------------------------------
		    *	Check that they selected at least 1 channel
		    * ----------------------------------------------------------------*/
		    sNumChans = 0;
		    i = 0;
	    
			while ((sNumChans == 0) && (i < glliNumRows)) 
			{
				if (pgllSelect1[i] == TRUE) 
					sNumChans++;

				i++;
			}		
		    if (sNumChans == 0) 
			{
				if (pWin != NULL) pWin->Printf("\nNo channels selected.");
				if (pWin != NULL) pWin->EnableUserClose(CloseRawDataWindow);
		    	RadReviewMsg(uiNO_CHANS_SELECTED_WARN);
				goto abort; //fixed leaks pjm 2-27-08
		    }	

			// check to see if there is any data in the interval
			status = ConvertStructsToSearchIntervalParameters(&GenDate, &GenTime, iUserUnits, ulUserInterval, &dUserIntervalStart, &dEnd);
			if (status != 1) 	// no data for any station in the interval
			{
				RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, dUserIntervalStart, dEnd, "any station");
				goto abort;	//fixed leaks pjm 2-27-08
			}

		   /* ------------------------------------------------------------------
		    *	Open up the text window where the events will be displayed
		    * ----------------------------------------------------------------*/
			if (pWin == NULL) 
			{                
				pWin = new CGUI_TextWin("Summary of Channel Events Found");
				if (pWin != NULL) 
					pWin->ShowStatusBar(TRUE);
				else
					exit(0);
			}   
			else 
			{
				pWin->Clear();
			} 
			pWin->SetFocus();
			
		   /* ------------------------------------------------------------------
		    *	Allocate storage for all stations at this facility
		    * ----------------------------------------------------------------*/
			sNumSta = pglFacCfg->GetNumStations(glsFacNum);
		    if (sNumSta == 0) 
				goto abort; //return;   //KM 

		 	//pAnalyzeChans = (struct ANALYZE_PARMS  *)malloc(sNumSta*sizeof(struct ANALYZE_PARMS));
			pAnalyzeChans = new ANALYZE_PARMS[sNumSta];
			if (!pAnalyzeChans) 
			{
				RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
				goto abort; //return;
			}

		    //   Display the general part of the header:
			if (!PrintFacilityHeader(pWin, sFacNum, dUserIntervalStart, dEnd)) 
				goto abort;
					
			// Calculate search interval	                                                  
			ConvertStructsToSearchIntervalParameters(&GenDate, &GenTime, iUserUnits, ulUserInterval, &dUserIntervalStart, &dEnd);
		    		                                                            
		   /* ------------------------------------------------------------------
		    * initialize the analyze structures so no channel at any station selected
		    * ----------------------------------------------------------------*/
		    for (j=0; j<sNumSta; j++) 
			{
		    	pAnalyzeChans[j].StaID.sStaID = -1;
				pAnalyzeChans[j].StaID.sChanID = -1;
				pAnalyzeChans[j].StaID.sStaType = -1;

		    	for (i=0; i < MAX_NUM_DATA_CHANS; i++)
		    		pAnalyzeChans[j].sChanOffset[i] = -1;	
		    }	
		                                          
		   /* ------------------------------------------------------------------
			* Sort through the channels selected and associate them with their station.
			* Fill in the analysis structure with known information and check that 
			* some data exists in the time interval for the station
		    * If no data in interval, then quit now.
		    * ----------------------------------------------------------------*/
			ulNumTotalEvents = 0;
			ulTotalPts = 0;
			ulEventPts = 0;

			for (i = 0; i < glliNumRows; i++) 
			{
				if (pgllSelect1[i] == TRUE) 
				{
					if (!pglFacCfg->GetChannelIDForPicklistChoice(i, &ChanID)) 
						goto abort; //return;

					for (short j = 0; j < sNumSta; j++) 
					{
						if ((pAnalyzeChans[j].StaID.sStaID == ChanID.sStaID) && 
							(pAnalyzeChans[j].StaID.sStaType == ChanID.sStaType)) 
							break; // already have this station

						if (pAnalyzeChans[j].StaID.sStaID == -1) 
						{
							pAnalyzeChans[j].StaID.sStaID = ChanID.sStaID; 	// new station
							pAnalyzeChans[j].StaID.sChanID = ChanID.sChanID;
							pAnalyzeChans[j].StaID.sStaType = ChanID.sStaType;
							break;
						}	
					}

					// check if data exists in interval for channel's station
					// fill in the db channel number, background and threshold for each channel in the station
					pglFacCfg->GetStationID(glsFacNum, &(pAnalyzeChans[j].StaID));		// fill in complete id info
					status = GetDataStatistics(pAnalyzeChans[j].StaID, true, dUserIntervalStart, dEnd, &dDum, &dDum, &ulPts);
					ulTotalPts += ulPts;

					if (!pglFacCfg->GetChannelRecord(glsFacNum, &ChanID, &dbChan)) 
						goto abort; //return;

					pAnalyzeChans[j].sChanOffset[dbChan.s_chan_offset-1] = ChanID.sChanID;	// fill in channel number
					pAnalyzeChans[j].StaID.sChanID = ChanID.sChanID;		// 20-Dec-2004 SFK  Must set the channel ID before call Get....

					if ((status == 1) && (ulPts > 0)) 
					{
						status = GetDataStatisticsForEventsByChannel(pAnalyzeChans[j].StaID, dUserIntervalStart, dEnd, &ulEventPts, ALL_EVENTS);
						ulNumTotalEvents += ulEventPts;
		       		}
				}	
			}		
			// display channels searched
		    pWin->Printf("\nChannels Searched:");

			for (j=0; j<sNumSta; j++) 
			{
		    	for (i=0; i < MAX_NUM_DATA_CHANS; i++) 
				{
		    		if (pAnalyzeChans[j].sChanOffset[i] != -1) 
					{
						pAnalyzeChans[j].StaID.sChanID = pAnalyzeChans[j].sChanOffset[i];

						if (!pglFacCfg->GetChannelRecord(glsFacNum, &(pAnalyzeChans[j].StaID), &dbChan)) 
							goto abort;

						pWin->Printf("\n%25s", dbChan.sz_chan_name);
					}	
		    	}	
			}
			pWin->Printf("\n");

			if (ulTotalPts == 0) 
			{
				pWin->Printf("\nNo data found in interval.");
				goto abort;
			}	
			
			if (ulNumTotalEvents == 0)
			{
				pWin->Printf("\nNo channel events found in interval.");
				goto abort;
			}	
		
		   /* ------------------------------------------------------------------
			*   Based on the choices specified, provide the appropriate summaries
			* ----------------------------------------------------------------*/
			switch (uiEventType) 
			{
				case ALL_EVENT_SUMMARIES:
				   /* ------------------------------------------------------------------
				    *	If user selected to group the events, do it now.
	    			* ----------------------------------------------------------------*/
	    			bGroup = FALSE;
	    			if ((ulMinDelta >= 0) && (ulMaxDelta >0) && (ulMinDelta < ulMaxDelta)) 
					{
						// figure out which search to use based on station type
						for (j=0; j<sNumSta; j++) 
						{
							if (pAnalyzeChans[j].StaID.sStaID != -1)
							{
								if (GroupChanEvents(dUserIntervalStart, dEnd, ulMinDelta, ulMaxDelta, START_TO_START) != TRUE) 
									goto abort;	// group any dbVista data
							}
						}
	                	bGroup = TRUE;
	                }	

					if (uiSortType == TIME_SORT) 
					{
						pWin->Printf("Search Method: all events sorted by time\n");
						pWin->Printf("______________________________________________________________________\n");
						ulNumEventsPrinted = 0;
						for (j=0; j<sNumSta; j++) 
						{
							if (pAnalyzeChans[j].StaID.sStaID != -1) 
							{
								//	21-Dec-2004 SFK	Added whether to include background in results
								EventDisplayByTime(dUserIntervalStart, dEnd, &pAnalyzeChans[j], pWin, bGroup, bIncludeBkg);  
							}
						}
					}
					else if (uiSortType == CHANNEL_SORT) 
					{	
						pWin->Printf("Search Method: all events sorted by channel\n");
						pWin->Printf("______________________________________________________________________\n");
						ulNumEventsPrinted = 0;
						for (j=0; j<sNumSta; j++) 
						{
							if (pAnalyzeChans[j].StaID.sStaID != -1) 
								//	21-Dec-2004 SFK	Added whether to include background in results
								EventDisplayByChan(dUserIntervalStart, dEnd, &pAnalyzeChans[j], pWin, bIncludeBkg);  
						}
					}
					break;
					
				case DAILY_SUMMARIES:
					if (uiSortType == TIME_SORT) 
					{
						pWin->Printf("Search Method: daily summaries sorted by time\n");
						pWin->Printf("______________________________________________________________________\n");
						for (j=0; j<sNumSta; j++) 
						{
							if (pAnalyzeChans[j].StaID.sStaID != -1) 
								//	21-Dec-2004 SFK	Added whether to include background in results
								DailySummaryByTime(dUserIntervalStart, dEnd, &pAnalyzeChans[j], pWin, bIncludeBkg);  
						}
					}
					else if (uiSortType == CHANNEL_SORT) 
					{	
						pWin->Printf("Search Method: daily summaries sorted by channel\n");
						pWin->Printf("______________________________________________________________________\n");
						for (j=0; j<sNumSta; j++) 
						{
							if (pAnalyzeChans[j].StaID.sStaID != -1) 
								//	21-Dec-2004 SFK	Added whether to include background in results
								DailySummaryByChan(dUserIntervalStart, dEnd, &pAnalyzeChans[j], pWin, bIncludeBkg);  
						}
					}
					break;
			}			

	    }//if DlgReturn == GUI_OK	      
	}  //if dlg valid
abort:		
	if (pWin)		
		pWin->EnableUserClose(CloseRawDataWindow);				
			
		CleanUpEventSummary();	//fixed leaks pjm 2-27-08
		return;
}   		                                 
