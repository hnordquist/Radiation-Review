///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgMarkSummary.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include <gen.h>
#include "RAD.H"
#include "DateTimeUtils.h"
#include "DbDay.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DbMark.h"
#include "DlgMarkSummary.h"
#include "FacilityConfig.h"
#include "Interval.h"
#include "PrintFacilityHeader.h"
#include "Utilities.h"
#include "MyDateTime.h"


extern short glsFacNum;
extern char *glnaszIntervalUnitsPicklist[];
extern CMyDateTime glMyDateTime;
extern char glszOldDbName[];   

static CGUI_TextWin *pWin = NULL;    


static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL CloseTextWindow(CGUI_TextWin *pWinArg);
static int PrintMarkSummary(struct db_mark_rec *dbMark, CGUI_TextWin *pWin);


                    
/* ------------------------------------------------------------------
 *	Called from the dialog box when the NewDefaults button is pushed.
 *	The present values are read from the dialog box and written to
 *	the Default database.
 * ----------------------------------------------------------------*/
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  
    unsigned int iChoice;
    
	status = AskUpdateQuestion();
	if (status != TRUE) return;
	    
   /* ------------------------------------------------------------------
    *	Get the current parameters set in the dialog box and write
    *	to default database.
    * ----------------------------------------------------------------*/ 
	pDlg->PickListGetValue(IDC_MARK_TYPE, &iChoice);
	status = AccessDefaultDatabase(WRITE, S_MARK_SUMM_TYPE, (short *)&iChoice);
	if (status != TRUE) goto exit;
	SaveDatabaseNow();
    return;

exit:
	RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
	return;
}        
         
 

/* ------------------------------------------------------------------
 *	Allows the user to close the window.
 * ----------------------------------------------------------------*/
static BOOL CloseTextWindow(CGUI_TextWin *pWinArg)
{

	BOOL bCanClose = TRUE;
	pWin = NULL;
	return(bCanClose);
}               


static unsigned long gllulPtsPrinted;
static unsigned long gllulTotalPtsPrinted;

// 12-Aug-2002	SFK	Changed to DATE
// 03-Oct-2003	SFK	Changed for new FacMgr
static int PrintMarkSummary(struct db_mark_rec *dbMark, CGUI_TextWin *pWin)
{
    char szDate[DT_LEN+1], szTime[DT_LEN+1];
    //struct db_chan_rec dbChan;
	char szChanName[DB_NAME_LEN+1];
    
    gllulPtsPrinted++;
    
	struct IDStruct ChanID;
	ChanID.sChanID = dbMark->s_mark_chan;
	ChanID.sStaID = dbMark->s_mark_sta;
	ChanID.sStaType = dbMark->s_mark_sta_type;
	pglFacCfg->GetChannelLongName(glsFacNum, &ChanID, szChanName, DB_NAME_LEN);
        
	glMyDateTime.DATETimestampToDateTimeStrs(dbMark->d_mark_time_key, szDate, szTime);
	pWin->Printf("\n%s %s %s ", szDate, szTime, szChanName);
	
	if (gllulPtsPrinted == (gllulTotalPtsPrinted + 20)) {
		gllulTotalPtsPrinted = gllulPtsPrinted; 
		CGUI_App::ProcessEvents();                /* update the output page */
	}
    return(TRUE);
}
       

/* ------------------------------------------------------------------
 *	Define picklist to be used in the dialog box.
 * ----------------------------------------------------------------*/
static char *gllMarkTypePicklist[] = {
					"All Marks",
				    "Daily Mark Summaries",
//				    "Auto Found Events",
//				    "Manual Marked Events",
				    (char *) 0
				    }; 
// these definitions correspond to the positions in the gllSummaryTypePicklist				    
#define ALL_MARKS 0
#define DAILY_MARK_SUMMARIES 1
//#define AUTO_FOUND_EVENTS 2
//#define MANUAL_MARKED_EVENTS 3				                  


/* ------------------------------------------------------------------
 *	Create and display the event summary dialog box.
 * ----------------------------------------------------------------*/
// 12-Aug-2002	SFK	Changed to DATE
// 03-Oct-2003	SFK	Changed for new FacMgr
void RadMarksSummariesMenuOption()
{                                 
    static struct DATA_ANALYZE AnalyzeParms= {60, 10, 3.0, FALSE};
                                                    
	// these parameters remain unchanged for each execution of the menu option                                                    
	static DATE dUserIntervalStart = 0;
	static unsigned long ulUserInterval = 0;
	static unsigned int iUserUnits = 0;                 
	static unsigned int uiMarkType;
	static BOOL bFirst = TRUE;
	static DATE dLastSearchIntervalInDays = 0.0;
	static short sFacNum;
    
    //struct db_fac_rec dbFac;    
    struct GEN_DATE_STRUCT GenDate;
    struct GEN_TIME_STRUCT GenTime;
    struct db_mark_rec dbMark;
    
    GUI_ACTION DlgReturn;                                     
    CGFmtStr szInterval;

	int status;     
	unsigned long ulDbIntervalInWholeDays;
	DATE dSearchIntervalInDays;
	DATE dEnd;
	DATE dCurrentDay;
 	unsigned long ulTotalMarks;
 	unsigned long ulDailyMarks;
 	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	char szFirstDate[DT_LEN+1];              
	char szLastDate[DT_LEN+1];              
	char **aszStaList = NULL; 

	// verify the facility is defined
	char szFacName[DB_NAME_LEN+1];
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) return;
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return;

	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);


   /* ------------------------------------------------------------------
    *	Always find the first and last days in the database and set
	*	up the strings for display in the dialog box.
    * ----------------------------------------------------------------*/
	SetCurrentDrive();
	status = GetDatabaseIntervalParameters(szFirstDate, szLastDate, &ulDbIntervalInWholeDays, &dSearchIntervalInDays);
	if (status != TRUE) return;
	if (dSearchIntervalInDays <= 0.0) {
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return;
	}
	

	if ((bFirst == TRUE) || (!SameDb(glszOldDbName)) || (dSearchIntervalInDays != dLastSearchIntervalInDays) || (sFacNum != glsFacNum)) {
	   /* ------------------------------------------------------------------
	    *	Get the default parameters from the Default database.
	    * ----------------------------------------------------------------*/
		status = AccessDefaultDatabase(READ, S_MARK_SUMM_TYPE, (short *)&uiMarkType);
		if (status != TRUE) {
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
	    
   /* ------------------------------------------------------------------
    *	Define the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg Dlg(IDD_SUMMARIES_MARKS, NULL, DLG_POSN_NO_SAVE);
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
		Dlg.FldStaticTxtSetValue(IDC_RAD_CURR_DB_START, szFirstDate);
					
		Dlg.DefineFldStaticTxt(IDC_RAD_CURR_DB_END);
		Dlg.FldStaticTxtSetValue(IDC_RAD_CURR_DB_END, szLastDate);
		
		szInterval.Printf("%ld days", ulDbIntervalInWholeDays);  	
		Dlg.DefineFldStaticTxt(IDC_RAD_CURR_DB_INTERVAL);
		Dlg.FldStaticTxtSetValue(IDC_RAD_CURR_DB_INTERVAL, szInterval);
		
	   /* ------------------------------------------------------------------
	    *	Set up the search interval parameters in dialog box
	    * ----------------------------------------------------------------*/
		glMyDateTime.DATETimestampToGenStructs(dUserIntervalStart, &GenDate, &GenTime);
		Dlg.DefineFldDate(IDC_RAD_SRCH_STRTDATE, &GenDate);
		Dlg.DefineFldTime(IDC_RAD_SRCH_STRTTIME, &GenTime);
		
		Dlg.DefineFldNum(IDC_RAD_SRCH_INTERVAL, &ulUserInterval, 1, 10000,"%ld");
		Dlg.DefinePickList(IDC_RAD_INTRVL_UNITS_PL, glnaszIntervalUnitsPicklist, &iUserUnits);

	   /* ------------------------------------------------------------------
	    *	Set up the output display options in dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefinePickList(IDC_MARK_TYPE, gllMarkTypePicklist, &uiMarkType);

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
	    	
		   /* ------------------------------------------------------------------
		    *	Setup the search interval and determine whether there is any
			*	data in the interval
		    * ----------------------------------------------------------------*/
			status = ConvertStructsToSearchIntervalParameters(&GenDate, &GenTime, iUserUnits, ulUserInterval, &dUserIntervalStart, &dEnd);
			if (status != TRUE) {	// no data for any station in the interval
				RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, dUserIntervalStart, dEnd, "any station");
				return;
			}

	
		   /* ------------------------------------------------------------------
			*	Some data exists,
			*	Open up the text window where the events will be displayed
			* ----------------------------------------------------------------*/
			if (pWin == NULL) {                
				pWin = new CGUI_TextWin("Summary of Marks Found");
				if (pWin != NULL) pWin->ShowStatusBar(TRUE);
			}   
			else {
				pWin->Clear();
			}
			pWin->SetFocus();
			
		   /* ------------------------------------------------------------------
			*   Display the general part of the header:
			*	facility name, search interval
			* ----------------------------------------------------------------*/
			if (!PrintFacilityHeader(pWin, sFacNum, dUserIntervalStart, dEnd)) return;
			
			status = ScanMarkRecs(dUserIntervalStart, FIRST, &dbMark);
			if (status != TRUE) {
				pWin->Printf("\n\nNo marks in database\n");
				pWin->Printf("______________________________________________________________________\n");
				if (pWin != NULL) pWin->EnableUserClose(CloseTextWindow);
				return;
			}	
					
		   /* ------------------------------------------------------------------
			*   Based on the choices specified, provide the appropriate summaries
			* ----------------------------------------------------------------*/
			switch (uiMarkType) {
				case ALL_MARKS:
					ulTotalMarks = 0;
					gllulPtsPrinted = 0;
					gllulTotalPtsPrinted = 0;
					pWin->Printf("Search Method: all marks\n");
					pWin->Printf("______________________________________________________________________\n");
	 		    	pWin->Printf("\nTime              Channel");		    
								  //96.10.10 13:44:44  PCAS-1
                	while ((status == TRUE) && (dbMark.d_mark_time_key < dEnd)) {
						if (status == TRUE) {
							PrintMarkSummary(&dbMark, pWin);
							ulTotalMarks++;
						}	
						status = ScanMarkRecs(dUserIntervalStart, NEXT, &dbMark);
					}	
					break;
															  
				case DAILY_MARK_SUMMARIES:
					ulTotalMarks = 0;	
					ulDailyMarks = 0;
					pWin->Printf("Search Method: daily summaries of all marks\n");
					pWin->Printf("______________________________________________________________________\n");
					pWin->Printf("\nDate       Marks");		    
								  //96.10.10 = 1234
					dCurrentDay = dbMark.d_mark_time_key;
					while ((status == TRUE) && (dbMark.d_mark_time_key < dEnd)) {					
						if (status == TRUE) {
							if ((int)dCurrentDay != (int)dbMark.d_mark_time_key) {
								glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
								pWin->Printf("\n%s = %4ld  ", szDate,  ulDailyMarks);
								dCurrentDay = dbMark.d_mark_time_key;
								ulDailyMarks = 1;
							}
							else {
								ulDailyMarks++;
							}	    
							ulTotalMarks++;
						}	
						status = ScanMarkRecs(dUserIntervalStart, NEXT, &dbMark);
					}
					if (ulDailyMarks > 0) {
						glMyDateTime.DATETimestampToDateTimeStrs(dCurrentDay, szDate, szTime);
						pWin->Printf("\n%s = %4ld  ", szDate,  ulDailyMarks);
					}	
				break;	
			}		
			pWin->Printf("\nTotal Marks Found = %lu\n", ulTotalMarks);		    			
			pWin->Printf("______________________________________________________________________\n");
			if (pWin != NULL) pWin->EnableUserClose(CloseTextWindow);
		}	
	}	
}   		                                 
   		                                 
