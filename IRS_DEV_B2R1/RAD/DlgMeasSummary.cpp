///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgMeasSummary.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////
#include <gen.h>
#include <gui_tbl.h>
#include "RAD.H"
#include "DateTimeUtils.h"
#include "DbDefault.h"
#include "DbEvent.h"
#include "DbGeneralDbSupport.h"
#include "DbInterval.h"
#include "DlgMeasSummary.h"
#include "FacilityConfig.h"
#include "FindMeas.h"
#include "Interval.h"
#include "PrintFacilityHeader.h"
#include "Utilities.h"
#include "FacilityConfig.h" //KM
#include "MyDateTime.h"
#include "TypeDefinitions.h"
    
#define ASSAY_BIT		0x0001
#define CF_NORM_BIT		0x0002
#define NORM_BKG_BIT	0x0004
#define EMPTY_BIT		0x0008
#define TOTALS_BIT		0x0010
#define CF_CHECK_BIT	0x0020
#define LOW_BKG_BIT		0x0040
#define HIGH_BKG_BIT	0x0080

extern short glsFacNum;		// facility that is chosen at highest level
extern char *glnaszIntervalUnitsPicklist[];
extern CMyDateTime glMyDateTime;
extern char glszOldDbName[]; 

static CGUI_TextWin *pWin;
static char **gllaszStaList = NULL;	
static char **gllnaszMeasTypeList = NULL;

static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL CloseWindow(CGUI_TextWin *pWinArg);
static BOOL EvaluateEvent(struct IDStruct ChanID, unsigned short usMeasChoice, struct db_event_rec *pdbChanEvent);


                    
/* ------------------------------------------------------------------
 *	Called from the dialog box when the NewDefaults button is pushed.
 *	The present values are read from the dialog box and written to
 *	the Default database.
 * ----------------------------------------------------------------*/
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  
    unsigned int iChoice;
	short sNum;
	BOOL bAssay = FALSE;
	BOOL bCfNorm = FALSE;
	BOOL bNormBkg = FALSE;
	BOOL bEmptyAssay = FALSE;
	BOOL bTotalsAssay = FALSE;
	BOOL bCfCheck = FALSE;
	BOOL bLowBkg = FALSE;
	BOOL bHighBkg = FALSE;
	struct IDStruct StaID;

    
	status = AskUpdateQuestion();
	if (status != TRUE) return;


   /* ------------------------------------------------------------------
    *	Get the current parameters set in the dialog box and write
    *	to default database.
    * ----------------------------------------------------------------*/ 
	pDlg->CheckBoxGetValue(IDC_MEAS_SUMM_ASSAY, &bAssay);	
	pDlg->CheckBoxGetValue(IDC_MEAS_SUMM_CF_NORM, &bCfNorm);	
	pDlg->CheckBoxGetValue(IDC_MEAS_SUMM_STATIC_BKG, &bNormBkg);	
	pDlg->CheckBoxGetValue(IDC_MEAS_SUMM_TOTALS, &bTotalsAssay);	
	pDlg->CheckBoxGetValue(IDC_MEAS_SUMM_CF_CHECK, &bCfCheck);	
	pDlg->CheckBoxGetValue(IDC_MEAS_SUMM_EMPTY, &bEmptyAssay);	
	pDlg->CheckBoxGetValue(IDC_MEAS_SUMM_LOW_BKG, &bLowBkg);	
	pDlg->CheckBoxGetValue(IDC_MEAS_SUMM_HIGH_BKG, &bHighBkg);	
	sNum = 0;
	if (bAssay)			sNum = sNum | ASSAY_BIT;
	if (bCfNorm)		sNum = sNum | CF_NORM_BIT; 
	if (bNormBkg)		sNum = sNum | NORM_BKG_BIT;
	if (bEmptyAssay)	sNum = sNum | EMPTY_BIT;
	if (bTotalsAssay)	sNum = sNum | TOTALS_BIT;
	if (bCfCheck)		sNum = sNum | CF_CHECK_BIT;
	if (bLowBkg)		sNum = sNum | LOW_BKG_BIT;
	if (bHighBkg)		sNum = sNum | HIGH_BKG_BIT;

	status = AccessDefaultDatabase(WRITE, S_MEAS_SUMM_TYPE, &sNum);
	if (status != TRUE) goto exit;
	
	pDlg->PickListGetValue(IDC_MEASUREMENT_STATION, &iChoice);
	if (!pglFacCfg->GetStationIDForPicklistChoice(iChoice, &StaID)) return;

	if ((!AccessDefaultDatabase(WRITE, S_MEAS_SUMM_STATION, &StaID.sStaID)) ||
		(!AccessDefaultDatabase(WRITE, S_MEAS_SUMM_STATION_TYPE, &StaID.sStaType))) {
		goto exit; 
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
static BOOL CloseWindow(CGUI_TextWin *pWinArg)
{
	BOOL bCanClose = TRUE;
	pWin = NULL;
	return(bCanClose);
}               
       


/* ------------------------------------------------------------------
 *	Based on the station and measurement type choice, determine whether
 *	the event matches the conditions.
 * ----------------------------------------------------------------*/
// 29-Aug-2002	SFK		Patch where assume channel number is always offset plus 1
// 16-Dec-2004	SFK		Let Normal Background be a valid event
static BOOL EvaluateEvent(struct IDStruct ChanID, unsigned short usMeasChoice, struct db_event_rec *pdbChanEvent)
{
	BOOL bMeasValid = FALSE;
	BOOL bStaOk = FALSE;
	short sChan;

	// Looking on all the stations?
	if (ChanID.sStaID == -1) {
		bStaOk = TRUE;
	}
	else {
		if (pdbChanEvent->us_event_type1 == TOTALS_MEAS){
			sChan = TOTALS_OFFSET+1;
		}
		else {
			sChan = REALS_OFFSET+1;
		}
		if ((pdbChanEvent->us_event_chan == sChan) &&
			(pdbChanEvent->us_event_sta == ChanID.sStaID) &&
			(pdbChanEvent->us_event_sta_type == ChanID.sStaType)) bStaOk = TRUE;
		if ((pdbChanEvent->us_event_type == BACKGROUND_EVENT) &&		// 16-Dec-2004 Added background as type
			(pdbChanEvent->us_event_sta == ChanID.sStaID) &&
			(pdbChanEvent->us_event_sta_type == ChanID.sStaType)) bStaOk = TRUE;
	}

	if (bStaOk) {
		if ((usMeasChoice & ASSAY_BIT) == ASSAY_BIT)
			if ((pdbChanEvent->us_event_type == MEASUREMENT_EVENT) && (pdbChanEvent->us_event_type1 == ASSAY_MEAS)) bMeasValid = TRUE;

		if ((usMeasChoice & CF_NORM_BIT) == CF_NORM_BIT)
			if ((pdbChanEvent->us_event_type == MEASUREMENT_EVENT) && (pdbChanEvent->us_event_type1 == CF_NORM_MEAS)) bMeasValid = TRUE;

		if ((usMeasChoice & CF_CHECK_BIT) == CF_CHECK_BIT)
			if ((pdbChanEvent->us_event_type == MEASUREMENT_EVENT) && (pdbChanEvent->us_event_type1 == CF_CHECK_MEAS)) bMeasValid = TRUE;

		if ((usMeasChoice & EMPTY_BIT) == EMPTY_BIT)
			if ((pdbChanEvent->us_event_type == MEASUREMENT_EVENT) && (pdbChanEvent->us_event_type1 == EMPTY_MEAS)) bMeasValid = TRUE;

		if ((usMeasChoice & NORM_BKG_BIT) == NORM_BKG_BIT)
			if ((pdbChanEvent->us_event_type == BACKGROUND_EVENT) && (pdbChanEvent->us_event_type1 == NORMAL_BKG_MEAS)) bMeasValid = TRUE;

		if ((usMeasChoice & LOW_BKG_BIT) == LOW_BKG_BIT)
			if ((pdbChanEvent->us_event_type == BACKGROUND_EVENT) && (pdbChanEvent->us_event_type1 == LOW_BKG_MEAS)) bMeasValid = TRUE;

		if ((usMeasChoice & HIGH_BKG_BIT) == HIGH_BKG_BIT)
			if ((pdbChanEvent->us_event_type == BACKGROUND_EVENT) && (pdbChanEvent->us_event_type1 == HIGH_BKG_MEAS)) bMeasValid = TRUE;

		if ((usMeasChoice & TOTALS_BIT) == TOTALS_BIT)
			if ((pdbChanEvent->us_event_type == MEASUREMENT_EVENT) && (pdbChanEvent->us_event_type1 == TOTALS_MEAS)) bMeasValid = TRUE;
	}
	return(bMeasValid);
}



/* ------------------------------------------------------------------
 *	Create and display the event summary dialog box.
 * ----------------------------------------------------------------*/
//  20-Aug-1999 SFK		Version 2.05 Added printing of maximum value and time for use in direction search
//										Fixed columns so they line up correctly
void MeasurementSummariesMenuOption()
{                                 
                                                    
	// these parameters remain unchanged for each execution of the menu option                                                    
    static DATE dUserIntervalStart = 0.0;
	static unsigned long ulIntervalInDays = 0;
	static unsigned int iUserUnits = 0;                 
	static DATE dLastSearchIntervalInDays = 0;
	static BOOL bFirst = TRUE;
	static struct IDStruct StaID;
	static short sFacNum;
    static unsigned int uiStaChoice;
	static unsigned short usMeasChoice;        
//static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = {""};

    struct GEN_DATE_STRUCT GenDate;
    struct GEN_TIME_STRUCT GenTime;
	struct db_event_rec dbChanEvent;

	CGFmtStr szInterval;
    
    GUI_ACTION DlgReturn;                                     
    
	int status;     
	int j;    
	short sNumStas;
	short sMaxNameLen;
	unsigned long ulDbIntervalInDays;
	DATE dSearchIntervalInDays;
	DATE dEnd;
	DATE dDum;
	unsigned long ulTotalPts;
	unsigned long ulNumMatches;
	char szFirstDate[DT_LEN+1], szLastDate[DT_LEN+1];  	
	BOOL bAssay = FALSE;
	BOOL bCfNorm = FALSE;
	BOOL bNormBkg = FALSE;
	BOOL bEmptyAssay = FALSE;
	BOOL bTotalsAssay = FALSE;
	BOOL bCfCheck = FALSE;
	BOOL bLowBkg = FALSE;
	BOOL bHighBkg = FALSE;
	struct IDStruct TempID;	

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
	status = GetDatabaseIntervalParameters(szFirstDate, szLastDate, &ulDbIntervalInDays, &dSearchIntervalInDays);
	if (status != TRUE) return;
	if (dSearchIntervalInDays <= 0.0) {
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return;
	}
	
	    
	if ((bFirst == TRUE) || (!SameDb(glszOldDbName)) || (dSearchIntervalInDays != dLastSearchIntervalInDays) || (sFacNum != glsFacNum)) {
	   /* ------------------------------------------------------------------
	    *	Get the default parameters from the Default database.
	    * ----------------------------------------------------------------*/
		if ((!AccessDefaultDatabase(READ, S_MEAS_SUMM_TYPE, (short *)&usMeasChoice)) ||
			(!AccessDefaultDatabase(READ, S_MEAS_SUMM_STATION, &StaID.sStaID)) ||
			(!AccessDefaultDatabase(READ, S_MEAS_SUMM_STATION_TYPE, &StaID.sStaType))) {
			RadReviewMsg(status, "when reading from Default database.  Cannot proceed.");			
			return;
		} 

		// make sure have a valid station
		if (StaID.sStaID == ANY_STATION) {
			if (pglFacCfg->GetNumStations(glsFacNum) < 1) {
  				RadReviewMsg(uiNO_STATIONS_FOR_FACILITY_ERR);
				return;
			}
		}
		else {
			if (!pglFacCfg->IsStationEnabled(glsFacNum, &StaID)) {
				if (pglFacCfg->GetNumStations(glsFacNum) > 0) {
					pglFacCfg->GetFirstStationID(glsFacNum, &StaID);
				}
				else {
	  				RadReviewMsg(uiNO_STATIONS_FOR_FACILITY_ERR);
					return;
				}
			}
		}
				
	   /* ------------------------------------------------------------------
	    *	Calculate the user interval
	    * ----------------------------------------------------------------*/
		GetReviewPeriodParameters(&dUserIntervalStart, &dEnd, &ulIntervalInDays);
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
    CGUI_Dlg Dlg(IDD_SUMMARIES_MEASUREMENTS, NULL, DLG_POSN_NO_SAVE);
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
		Dlg.DefineFldStaticTxt(IDC_MEASUREMENT_SUMM_FACILITY);
		Dlg.FldStaticTxtSetValue(IDC_MEASUREMENT_SUMM_FACILITY, szFacName);
			
	   /* ------------------------------------------------------------------
	    *	Display current starting ending database dates and interval in dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefineFldStaticTxt(IDC_MEASUREMENT_SUMM_CURR_DB_START);
		Dlg.FldStaticTxtSetValue(IDC_MEASUREMENT_SUMM_CURR_DB_START, szFirstDate);
					
		Dlg.DefineFldStaticTxt(IDC_MEASUREMENT_SUMM_CURR_DB_END);
		Dlg.FldStaticTxtSetValue(IDC_MEASUREMENT_SUMM_CURR_DB_END, szLastDate);

		szInterval.Printf("%ld days", ulDbIntervalInDays);  
		Dlg.DefineFldStaticTxt(IDC_MEASUREMENT_SUMM_CURR_DB_INTERVAL);
		Dlg.FldStaticTxtSetValue(IDC_MEASUREMENT_SUMM_CURR_DB_INTERVAL, szInterval);
		
	   /* ------------------------------------------------------------------
	    *	Set up the search interval parameters in dialog box
	    * ----------------------------------------------------------------*/
		//Date.DATETimestampToGenStructs(dUserIntervalStart, &GenDate, &GenTime);
		glMyDateTime.DATETimestampToGenStructs(dUserIntervalStart, &GenDate, &GenTime);
		Dlg.DefineFldDate(IDC_MEASUREMENT_SUMM_SRCH_STRTDATE, &GenDate);
		Dlg.DefineFldTime(IDC_MEASUREMENT_SUMM_SRCH_STRTTIME, &GenTime);
		
		Dlg.DefineFldNum(IDC_MEASUREMENT_SUMM_SRCH_INTERVAL, &ulIntervalInDays, 1, 10000,"%ld");
		Dlg.DefinePickList(IDC_MEASUREMENT_SUMM_INTRVL_UNITS_PL, glnaszIntervalUnitsPicklist, &iUserUnits);

	   /* ------------------------------------------------------------------
	    *	Set up the station and measurement type options in dialog box
	    * ----------------------------------------------------------------*/
		// Explode usMeasChoice into individual components
		if ((usMeasChoice & ASSAY_BIT) == ASSAY_BIT) bAssay = TRUE;
		if ((usMeasChoice & CF_NORM_BIT) == CF_NORM_BIT) bCfNorm = TRUE;
		if ((usMeasChoice & NORM_BKG_BIT) == NORM_BKG_BIT) bNormBkg = TRUE;
		if ((usMeasChoice & EMPTY_BIT) == EMPTY_BIT) bEmptyAssay = TRUE;
		if ((usMeasChoice & TOTALS_BIT) == TOTALS_BIT) bTotalsAssay = TRUE;
		if ((usMeasChoice & CF_CHECK_BIT) == CF_CHECK_BIT) bCfCheck = TRUE;
		if ((usMeasChoice & LOW_BKG_BIT) == LOW_BKG_BIT) bLowBkg = TRUE;
		if ((usMeasChoice & HIGH_BKG_BIT) == HIGH_BKG_BIT) bHighBkg = TRUE;
		Dlg.DefineCheckBox(IDC_MEAS_SUMM_ASSAY, &bAssay);	
		Dlg.DefineCheckBox(IDC_MEAS_SUMM_CF_NORM, &bCfNorm);	
		Dlg.DefineCheckBox(IDC_MEAS_SUMM_STATIC_BKG, &bNormBkg);	
		Dlg.DefineCheckBox(IDC_MEAS_SUMM_TOTALS, &bTotalsAssay);	
		Dlg.DefineCheckBox(IDC_MEAS_SUMM_CF_CHECK, &bCfCheck);	
		Dlg.DefineCheckBox(IDC_MEAS_SUMM_EMPTY, &bEmptyAssay);	
		Dlg.DefineCheckBox(IDC_MEAS_SUMM_LOW_BKG, &bLowBkg);	
		Dlg.DefineCheckBox(IDC_MEAS_SUMM_HIGH_BKG, &bHighBkg);	

		int iTemp;
		if (!pglFacCfg->CreateStationPicklist(glsFacNum, true, &gllaszStaList, &iTemp)) return;
		sMaxNameLen = iTemp;
		short sTemp = (short)uiStaChoice;
		if (!pglFacCfg->GetPicklistChoiceForStationID(StaID, &sTemp)) return;
		uiStaChoice = sTemp;
		Dlg.DefinePickList(IDC_MEASUREMENT_STATION, gllaszStaList, &uiStaChoice);
			    
	   /* ------------------------------------------------------------------
	    *	Set up the focus and display the dialog box
	    * ----------------------------------------------------------------*/
		Dlg.SetFocus(IDC_MEASUREMENT_SUMM_SRCH_INTERVAL);   
		DlgReturn = Dlg.Go();
			    
	   /* ------------------------------------------------------------------
	    *	Returned from dialog box, now process it.
	    * ----------------------------------------------------------------*/
	    if (DlgReturn == GUI_OK) {  
			    	
	    	Dlg.RetrieveAllControlValues();	/* get dialog values */
	    	
			// Implode the individual components back into usMeasChoice
			usMeasChoice = 0;
			if (bAssay)			usMeasChoice = usMeasChoice | ASSAY_BIT;
			if (bCfNorm)		usMeasChoice = usMeasChoice | CF_NORM_BIT; 
			if (bNormBkg)		usMeasChoice = usMeasChoice | NORM_BKG_BIT;
			if (bEmptyAssay)	usMeasChoice = usMeasChoice | EMPTY_BIT;
			if (bTotalsAssay)	usMeasChoice = usMeasChoice | TOTALS_BIT;
			if (bCfCheck)		usMeasChoice = usMeasChoice | CF_CHECK_BIT;
			if (bLowBkg)		usMeasChoice = usMeasChoice | LOW_BKG_BIT;
			if (bHighBkg)		usMeasChoice = usMeasChoice | HIGH_BKG_BIT;

			if (usMeasChoice == 0) {
				RadReviewMsg(uiNO_MEAS_TYPES_SELECTED_WARN);
				return;
			}

		   /* ------------------------------------------------------------------
			*   Calculate the search interval
			* ----------------------------------------------------------------*/
			status = ConvertStructsToSearchIntervalParameters(&GenDate, &GenTime, iUserUnits, ulIntervalInDays, &dUserIntervalStart, &dEnd);
			if (status != TRUE) {	// no data for any station in the interval
				RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, dUserIntervalStart, dEnd, "any station");
				return;
			}

		   /* ------------------------------------------------------------------
			*   Handle the special case of all stations
			* ----------------------------------------------------------------*/
			if (!pglFacCfg->GetStationIDForPicklistChoice(uiStaChoice, &StaID)) return;
		   /* ------------------------------------------------------------------
			*   Verify that some data exists
			* ----------------------------------------------------------------*/
			if (StaID.sStaID == -1) {		// -1 indicates all stations; verify at least one has data
				BOOL bDataExists = FALSE;
				int iNumStas = pglFacCfg->GetNumStations(glsFacNum);
				for (int i=0; i<iNumStas; i++) {
					if (!pglFacCfg->GetStationIDForPicklistChoice(i, &TempID)) return;
					status = GetDataStatistics(TempID, true, dUserIntervalStart, dEnd, &dDum, &dDum, &ulTotalPts);
					if ((status == TRUE) && (ulTotalPts > 0)) {
						bDataExists = TRUE;
						break;
					}
				}
				if (bDataExists == FALSE) {	
					RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, dUserIntervalStart, dEnd, "any station");
					pglFacCfg->DestroyPicklist(gllaszStaList);
					gllaszStaList = NULL;
					return;	
				}
			}
			else {					// see if the choice selected has data
				status = GetDataStatistics(StaID, true, dUserIntervalStart, dEnd, &dDum, &dDum, &ulTotalPts);
				if ((status != TRUE) || (ulTotalPts == 0)) {
					RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, dUserIntervalStart, dEnd, (const char*)gllaszStaList[uiStaChoice]);
					pglFacCfg->DestroyPicklist(gllaszStaList);
					gllaszStaList = NULL;
					return;	
				}
			}


		   /* ------------------------------------------------------------------
		    *	Open up the text window where the events will be displayed
		    * ----------------------------------------------------------------*/
			if (pWin == NULL) {                
				pWin = new CGUI_TextWin("Summary of Measurements Found");
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
			*	channels involved in search
			* ----------------------------------------------------------------*/
			if (!PrintFacilityHeader(pWin, sFacNum, dUserIntervalStart, dEnd)) return;

			if (StaID.sStaID != -1) {
				if (pWin != NULL) pWin->Printf("\n\nStation = %s", gllaszStaList[uiStaChoice]);		//print station name
			}
			else {
				sNumStas = pglFacCfg->GetNumStations(glsFacNum);
				pWin->Printf("\nStations Searched:");
				for (j=0; j<(sNumStas); j++) { 
					pWin->Printf("\n     %s", gllaszStaList[j]);
				}	
			}
			if ((StaID.sStaID != -1) && (StaID.sStaType == MCA_TYPE)) {
				if (pWin != NULL) pWin->Printf("\nMeasurement summaries are not applicable to MCA data.");
				if (pWin != NULL) pWin->EnableUserClose(CloseWindow);	// 20-Sep-2005 SFK Allow user to close window
				return;
			}
			if ((StaID.sStaID != -1) && (StaID.sStaType == GRAND_TYPE)) {
				if (pWin != NULL) pWin->Printf("\nMeasurement summaries are not applicable to GRAND data.");
				if (pWin != NULL) pWin->EnableUserClose(CloseWindow);	// 20-Sep-2005 SFK Allow user to close window
				return;
			}
			if ((StaID.sStaID != -1) && (StaID.sStaType == BINARY_TYPE)) {
				if (pWin != NULL) pWin->Printf("\nMeasurement summaries are not applicable to binary data.");
				if (pWin != NULL) pWin->EnableUserClose(CloseWindow);	// 20-Sep-2005 SFK Allow user to close window
				return;

			}

		   /* ------------------------------------------------------------------
			*   Scan through the measurements and count those that match.
			* ----------------------------------------------------------------*/
			ulNumMatches = 0;
			status = ScanChannelEvents(dUserIntervalStart, FIRST, &dbChanEvent);
			if (dbChanEvent.d_event_beg_time_key <= dEnd) {
				do {
					//if (EvaluateEvent(sStaNum, usMeasChoice, &dbChanEvent)) ulNumMatches++;
					if (EvaluateEvent(StaID, usMeasChoice, &dbChanEvent)) ulNumMatches++;
					status = ScanChannelEvents(dUserIntervalStart, NEXT, &dbChanEvent);
				} while ((status ==TRUE) && (dbChanEvent.d_event_beg_time_key <= dEnd));
			}

			if (ulNumMatches == 0) {
				if (pWin != NULL) pWin->Printf("\nNo measurements found in interval.");
				pglFacCfg->DestroyPicklist(gllaszStaList);
				gllaszStaList = NULL;
				if (pWin != NULL) pWin->EnableUserClose(CloseWindow);				
				return;
			}

			if (pWin != NULL) pWin->Printf("\n\n         Start    End      ID          Width Measurement   Maximum Maximum    Avg Totals    Avg Reals Dir        R/     Station");
			if (pWin != NULL) pWin->Printf(  "\nDate     Time     Time                (secs) Type             Rate    Time       Rate         Rate               T");
						                       /*90.05.07 10:10:01 10:20:25 12345 Assay*/
			pWin->Printf("\n");
		    
		   /* ------------------------------------------------------------------
			*   Scan through the measurements and print those that match.
			* ----------------------------------------------------------------*/
			ulNumMatches = 0;
			status = ScanChannelEvents(dUserIntervalStart, FIRST, &dbChanEvent);
			char szStaName[DB_NAME_LEN+1];
			do {
				if (dbChanEvent.d_event_beg_time_key <= dEnd)
				{
					if (EvaluateEvent(StaID, usMeasChoice, &dbChanEvent)) {
						PrintMeasEventToTextWin(&dbChanEvent, pWin, &ulNumMatches);
						TempID.sChanID = dbChanEvent.us_event_chan;
						TempID.sStaID = dbChanEvent.us_event_sta;
						TempID.sStaType = dbChanEvent.us_event_sta_type;
						pglFacCfg->GetStationLongName(glsFacNum, &TempID, szStaName, DB_NAME_LEN);
						if (pWin != NULL) pWin->Printf(" %s", szStaName );
					}
					status = ScanChannelEvents(dUserIntervalStart, NEXT, &dbChanEvent);
				}
			} while ((status ==TRUE) && (dbChanEvent.d_event_beg_time_key <= dEnd));

			if (pWin != NULL) pWin->Printf("\n\nTotal Measurements Found for %s = %ld", gllaszStaList[uiStaChoice], ulNumMatches); 

			if (pWin != NULL) pWin->EnableUserClose(CloseWindow);				

			pglFacCfg->DestroyPicklist(gllaszStaList);
			gllaszStaList = NULL;
	    }	      
	}
}   		                                 
   		                                 
