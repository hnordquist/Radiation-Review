///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgDaysSummary.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//	DbDaysMenuOption()
///////////////////////////////////////////////////////////////////////////

#include <gen.h>
#include "RAD.H"
#include "DateTimeUtils.h"
#include "DbDay.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DbInterval.h"
#include "FacilityConfig.h"
#include "Interval.h"
#include "PrintFacilityHeader.h"
#include "ProgressBars.h"
#include "Utilities.h"
#include "TimeAlign.h"
#include "MyDateTime.h"
#include "DlgDaysSummary.h"		// must be after FacilityConfig.h
#include "TypeDefinitions.h"
#include "EOSSImport.h"
#include "BinaryImport.h"

extern short glsFacNum;
extern char *glnaszIntervalUnitsPicklist[];
extern CMyDateTime glMyDateTime;
extern CDbDay glDbDay;
extern char glszOldDbName[];
extern CEOSSImport *glEOSSImport;
extern CBinaryImport *glBinaryImport;

static CGUI_TextWin *pWin = NULL;    
static RESOURCE_ID gllBusyBtnHit;

// static functions only in this file
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL CloseDbDaysWindow(CGUI_TextWin *pWinArg);
static void BusyCancelBtnServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void DisplayDbDays(DATE dStart, DATE dEnd, struct IDStruct *pStaID);

/*===========================================================================
 *
 *  Name	 :  	DbDaysDialog
 *
 *  Purpose	 :		Display the dialog box that allows the user to specify
 *					which days for which to display data.
 *					The first time in the function the default parameters are
 *					read from the database (the interval is always defaulted
 *					to the entire database).  On subsequent execution of the 
 *					option the last parameters set are used.
 *
 *
 *  Return value :  None
 *
 *  Special notes:  
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  16-Mar-1996 SFK		Created
 *  22-Sep-2003 SFK		Version 3.0.3 Integrated new Facility Mgr
 *
===========================================================================*/
void DbDaysMenuOption()
{                                 
    // Peggy says we can take these out so we can look at the values.
	// hn 10/22/2007
	static DATE dUserIntervalStart = 0.0;
	static unsigned long ulIntervalInDays = 0;
	static unsigned int iUserUnits = 0;                 
	//static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = {""};;
	static short sStaNum;
	static short sFacNum;
	static BOOL bFirst = TRUE;
	char szFirstDate[DT_LEN+1];              
	char szLastDate[DT_LEN+1];              
	static DATE dLastSearchIntervalInDays = 0;
	static struct IDStruct StaID;
	static short sStaChoice;
	CGFmtStr szInterval;
	
	char strFacName[DB_NAME_LEN+1];
    struct GEN_DATE_STRUCT GenDate;
    struct GEN_TIME_STRUCT GenTime;
                                   
    char **aszStaList = NULL; 
	struct IDStruct TempID;
    
	int status;     
	int i;
	BOOL bDataExists;
	unsigned long ulDbIntervalInDays, ulTotalPts;
	DATE dDum, dEnd;
	DATE dSearchIntervalInDays;

	static char szFirstTime[DT_LEN+1];              
	static char szLastTime[DT_LEN+1];              
	int iNumStas;
	
	GUI_ACTION DlgReturn;                                     
	CGFmtStr msg;  

	SetCurrentDrive();

	// Verify that we are working with a valid facility
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) {
		RadReviewMsg(uiFACILITY_NOT_DEFINED_ERR, "", (int)sFacNum);
		return;
	}
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return;



	//	Find the first and last days in the database for the station
	status = GetDatabaseIntervalParameters(szFirstDate, szLastDate, &ulDbIntervalInDays, &dSearchIntervalInDays);
	if (status != TRUE) return;
	if (dSearchIntervalInDays <= 0.0) {
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return;
	}
	
    if ((bFirst == TRUE) || 
		(!SameDb(glszOldDbName)) || 
		(dSearchIntervalInDays != dLastSearchIntervalInDays) || 
		(sFacNum != glsFacNum)) 
	{

		// Get the default station from the Default database.
       	status = AccessDefaultDatabase(READ, S_DAYS_STATION, &StaID.sStaID);
    	if (status != TRUE) {
			RadReviewMsg(status, "when reading from Default database.  Cannot proceed.");			
			return;
		}
       	status = AccessDefaultDatabase(READ, S_DAYS_STATION_TYPE, &StaID.sStaType);
    	if (status != TRUE) {
			RadReviewMsg(status, "when reading from Default database.  Cannot proceed.");			
			return;
		}

		if (StaID.sStaID == ANY_STATION) {
			if (pglFacCfg->GetNumStations(glsFacNum) < 1) {
  				RadReviewMsg(uiNO_STATIONS_FOR_FACILITY_ERR);
				return;
			}
		}
		else {
			// make sure you get a valid station from the db
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

		pglFacCfg->GetStationID(glsFacNum, &StaID);		// fill in StaID Record
		// Get the correct period to review
		GetReviewPeriodParameters(&dUserIntervalStart, &dDum, &ulIntervalInDays);
		iUserUnits = 0;

		// remember the current conditions for future entries to this option
		sFacNum = glsFacNum;
		GetDbName(glszOldDbName);								
		dLastSearchIntervalInDays = dSearchIntervalInDays;	
		bFirst = FALSE;
	}		
	
	iNumStas = pglFacCfg->GetNumStations(glsFacNum);

   /* ------------------------------------------------------------------
    *	Create dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg DbDaysDlg(IDD_SUMMARIES_DB_DAYS, NULL, DLG_POSN_NO_SAVE);
    if (DbDaysDlg.IsValid()) {
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		DbDaysDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		DbDaysDlg.DefinePushBtn(IDOK, GUI_OK);  
		DbDaysDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateDbParms);  		
				                                               
		// Fill in Facility portion of dialog
		pglFacCfg->GetFacilityLongName(sFacNum, strFacName, DB_NAME_LEN);
		DbDaysDlg.DefineFldStaticTxt(IDC_DAYS_FACILITY);
		DbDaysDlg.FldStaticTxtSetValue(IDC_DAYS_FACILITY, strFacName);

		// Fill in Station portion of dialog
		int iMaxLen;		// longest name in list -- not used
		if (!pglFacCfg->CreateStationPicklist(sFacNum, TRUE, &aszStaList, &iMaxLen)) return;	// list with All Stations as member
		pglFacCfg->GetPicklistChoiceForStationID(StaID, &sStaChoice);
		DbDaysDlg.DefinePickList(IDC_DAYS_STATION, aszStaList, (unsigned int *)&sStaChoice);
			
	   /* ------------------------------------------------------------------
	    *	Display current starting ending database dates and interval
	    * ----------------------------------------------------------------*/
		DbDaysDlg.DefineFldStaticTxt(IDC_DAYS_CURR_DB_START);
		DbDaysDlg.FldStaticTxtSetValue(IDC_DAYS_CURR_DB_START, szFirstDate);
					
		DbDaysDlg.DefineFldStaticTxt(IDC_DAYS_CURR_DB_END);
		DbDaysDlg.FldStaticTxtSetValue(IDC_DAYS_CURR_DB_END, szLastDate);

		szInterval.Printf("%ld days", ulDbIntervalInDays);  				
		DbDaysDlg.DefineFldStaticTxt(IDC_DAYS_CURR_DB_INTERVAL);
		DbDaysDlg.FldStaticTxtSetValue(IDC_DAYS_CURR_DB_INTERVAL, szInterval);
		
	   /* ------------------------------------------------------------------
	    *	Fill in default values for search start date and time plus interval
	    * ----------------------------------------------------------------*/
		//CMyDateTime Date;
		glMyDateTime.DATETimestampToGenStructs(dUserIntervalStart, &GenDate, &GenTime);
		DbDaysDlg.DefineFldDate(IDC_DAYS_SRCH_STRTDATE, &GenDate);
		DbDaysDlg.DefineFldTime(IDC_DAYS_SRCH_STRTTIME, &GenTime);
			                                                          
		DbDaysDlg.DefineFldNum(IDC_DAYS_SRCH_INTERVAL, &ulIntervalInDays, 1, 10000,"%ld");
		DbDaysDlg.DefinePickList(IDC_DAYS_INTRVL_UNITS_PL, glnaszIntervalUnitsPicklist, &iUserUnits);
				                             
		DbDaysDlg.SetFocus(IDC_DAYS_SRCH_INTERVAL);
			
	    DlgReturn = DbDaysDlg.Go();
			    
	    if (DlgReturn == GUI_OK) {  
			    	
	    	DbDaysDlg.RetrieveAllControlValues();
	    	
		   /* ------------------------------------------------------------------
			*   Calculate the search interval
			* ----------------------------------------------------------------*/
			status = ConvertStructsToSearchIntervalParameters(&GenDate, &GenTime, iUserUnits, ulIntervalInDays, &dUserIntervalStart, &dEnd);
			if (status != TRUE) {	// no data for any station in the interval
				RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, dUserIntervalStart, dEnd, "any station");
				pglFacCfg->DestroyPicklist(aszStaList);
				return;
			}


		   /* ------------------------------------------------------------------
			*   Handle the special case of all stations
			* ----------------------------------------------------------------*/
			if (!pglFacCfg->GetStationIDForPicklistChoice(sStaChoice, &StaID)) 
				return;

			if (StaID.sStaID == -1) 
			{		// -1 indicates all stations; verify at least one has data
				bDataExists = FALSE;
				for (i=0; i<iNumStas; i++) 
				{
					if (!pglFacCfg->GetStationIDForPicklistChoice(i, &TempID)) 
						return;
					status = GetDataStatistics(TempID, true, dUserIntervalStart, dEnd, &dDum, &dDum, &ulTotalPts);
					if ((status == TRUE) && (ulTotalPts > 0)) 
					{
						bDataExists = TRUE;
						break;
					}
				}
				if (bDataExists == FALSE) 
				{	
					RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, dUserIntervalStart, dEnd, "any station");
					pglFacCfg->DestroyPicklist(aszStaList);
					return;	
				}
			}
			else 
			{					// see if the choice selected has data
				status = GetDataStatistics(StaID, true, dUserIntervalStart, dEnd, &dDum, &dDum, &ulTotalPts);

				if ((status != TRUE) || (ulTotalPts == 0)) 
				{
					RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, dUserIntervalStart, dEnd, (const char*)aszStaList[sStaChoice]);
					pglFacCfg->DestroyPicklist(aszStaList);
					return;	
				}
			}

			// Know there is some data to display.
			// Open up the window where the days information will be displayed.
			if (pWin == NULL) 
			{                
				pWin = new CGUI_TextWin("Days in Database");
				if (pWin != NULL) 
					pWin->ShowStatusBar(TRUE);
			}   
			else 
			{
				pWin->Clear();
			}
			pWin->SetFocus();
			
			// print the facility header
			if (!PrintFacilityHeader(pWin, sFacNum, dUserIntervalStart, dEnd)) 
				return;

		   /* ------------------------------------------------------------------
			*	If all stations requested, print a summary for each station.
			*	Otherwise print the summary for the station requested.
			* ----------------------------------------------------------------*/
			if (StaID.sStaID == -1) 
			{	// print summary for all stations at facility
				//  Create the progress bar
				//	15-Nov-2004 SFK 3.0.3.16 Added progress bar
				TwoLineProgressBar *pTwoLineProgressBar;
				CGFmtStr Msg, Msg1, Msg2;
				BOOL bContinue = true;
				Msg1.Printf("Generating day summaries, please wait.");
				Msg2.Printf("");
				pTwoLineProgressBar = new TwoLineProgressBar(iNumStas, 1, Msg1, Msg2, "Radiation Review: Progress");
				if (pTwoLineProgressBar) 
				{
					for (i=0; i<iNumStas; i++) 
					{ 
						pglFacCfg->GetStationIDForPicklistChoice(i, &TempID);
						if (TempID.sStaID == -1) continue;	// skip the all stations choice
						DisplayDbDays(dUserIntervalStart, dEnd, &TempID);
							pWin->Printf("\n______________________________________");
						Msg2.Printf("Processed %d stations.\n",i);
						bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
					}
				}
				pTwoLineProgressBar->Close();
				delete pTwoLineProgressBar;
					
			}	
			else 
			{	// print summary for single selected station
				DisplayDbDays(dUserIntervalStart, dEnd, &StaID);
			}
			if (pWin != NULL) 
				pWin->EnableUserClose(CloseDbDaysWindow);	
		}	      
	}    
	pglFacCfg->DestroyPicklist(aszStaList);
}   		                                 
   		                                 
	                                     


/*===========================================================================
 *
 *  Name	 :  	UpdateDbParms
 *
 *  Purpose	 :		Called from the dialog box when the NewDefaults button is pushed.
 *					The present values are read from the dialog box and written to
 *					the Default database.
 *
 *
 *  Return value :  None
 *
 *  Special notes:  
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  16-Mar-1996 SFK		Created
 *  22-Sep-2003 SFK		Version 3.0.3 Integrated new Facility Mgr
 *
===========================================================================*/
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  
	unsigned int sStaChoice;

	status = AskUpdateQuestion();
	if (status != TRUE) return;
	    
   /* ------------------------------------------------------------------
    *	Get the current station chosen and find its number.
    * ----------------------------------------------------------------*/
	struct IDStruct StaID;
	pDlg->PickListGetValue(IDC_DAYS_STATION, &sStaChoice);
	pglFacCfg->GetStationIDForPicklistChoice(sStaChoice, &StaID);
	
   /* ------------------------------------------------------------------
    *	Write the current values to the database
    * ----------------------------------------------------------------*/
	status = AccessDefaultDatabase(WRITE, S_DAYS_STATION, &StaID.sStaID);
	if (status != TRUE) {
		RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
	}
	status = AccessDefaultDatabase(WRITE, S_DAYS_STATION_TYPE, &StaID.sStaType);
	if (status != TRUE) {
		RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
	}
	SaveDatabaseNow();
}        
         

///////////////////////////////////////////////////////////////////////////////////////// 
static BOOL CloseDbDaysWindow(CGUI_TextWin *pWinArg)
{

	BOOL bCanClose = TRUE;
	pWin = NULL;
	return(bCanClose);
}               
       

////////////////////////////////////////////////////////////////////////////////////////////        
                
static void BusyCancelBtnServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	GUI_ACTION response;
	
	response = GUI_MsgBox("Are you sure you want to cancel?", GUI_ICON_QUESTION, GUI_YESNO);
	if (response == GUI_YES) {
		gllBusyBtnHit = IDCANCEL;
	}		
}

        


/*===========================================================================
 *
 *  Name	 :  DisplayDbDays
 *
 *  Purpose	 :  For the given station, retrieve which days from the interval 
 *				are in the database.
 *				For each day, print the date and the number of points.
 *				If a day is missing print "missing days".
 *
 *  Return value :  None
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  15-Aug-1995	SFK	Created (based on display.c of original Grand Review)
 *	27-Mar-2001	SFK		Version 2.07 Added support of Access Binary database
 *  19-Feb-2002	SFK		Version 2.10 Added support of getting day info from GrandImportCo
 *							Also changed way other db stuff gets into to make more general
 *  18-Sep-2003 SFK		Version 3.0.3 Integrated new Facility Mgr
===========================================================================*/
static void DisplayDbDays(DATE dStart, DATE dEnd, struct IDStruct *pStaID)
{
    struct db_day_rec dbDay;
    
    char szDate[DT_LEN+1], szTime[DT_LEN+1];
    int  iStatus;
	int  i;
    DATE dDay;
    unsigned long ulDaysFound;
    unsigned long ulTotalPts;
	DATE dDum;
	BOOL bSameDay;

	DayDataStruct* pDay = NULL;
	    
    CGFmtStr msg;
	    
   /* ------------------------------------------------------------------
	*   Display the option specific header
	* ----------------------------------------------------------------*/
	char szStaName[DB_NAME_LEN+1];
	pglFacCfg->GetStationLongName(glsFacNum, pStaID, szStaName, MAX_NAME_LEN);
	pWin->Printf("\n\n\nStation = %s\n", szStaName);
	pWin->Printf("\n           Total    Start       End");			                                                              
    pWin->Printf("\n  Date     Points   Time        Time");
	                /*90.05.07   86400    10:10:01    10:10:59*/
                   
   /* ------------------------------------------------------------------
    *	Decide whether this instrument type is supported in a COM or the
	*	original dbVista database in RAD.
	*	Look through db for all days in the range, print information
    *	about those found.
    * ----------------------------------------------------------------*/
    dDay = dStart;

	// get the information in the days struct array and then use a common format routine.
	if (pStaID->sStaType == EOSS_TYPE)
	{
		iStatus = pglEOSSImport->GetDayStatistics(glsFacNum, pStaID, dStart, dEnd, &dDum, &dDum, &ulDaysFound);
		if (iStatus) 
		{
			pDay = new DayDataStruct[ulDaysFound];
			iStatus = pglEOSSImport->GetDataByDay(glsFacNum, pStaID, dStart, dEnd, pDay, &ulDaysFound);	
		}
	}
	else if (pStaID->sStaType == BINARY_TYPE)
	{
		iStatus = pglBinaryImport->GetDayStatistics(glsFacNum, pStaID, dStart, dEnd, &dDum, &dDum, &ulDaysFound);
		if (iStatus) 
		{
			pDay = new DayDataStruct[ulDaysFound];
			iStatus = pglBinaryImport->GetDataByDay(glsFacNum, pStaID, dStart, dEnd, pDay, &ulDaysFound);	
		}
	}
	else if (pglTimeAlign->COMDataType(pStaID->sStaType)) 		// is the data type supported via a COM?
	{
		iStatus = pglTimeAlign->GetDayStatistics(glsFacNum, pStaID, dStart, dEnd, &dDum, &dDum, &ulDaysFound);
		if (iStatus) 
		{
			pDay = new DayDataStruct[ulDaysFound];
			iStatus = pglTimeAlign->GetDataByDay(glsFacNum, pStaID, dStart, dEnd, pDay, &ulDaysFound);	
		}
	}
	else 
	{	// data is in old dbVista database
		// determine number of days in interval
		ulDaysFound = 0;
		bSameDay = FALSE;
		do 
		{
			iStatus = glDbDay.ReadDayRecInMainDb(pStaID->sStaID, dDay, &dbDay);
    		if ((iStatus != TRUE) && (iStatus != uiDAY_NOT_IN_DB_STATUS))	// unexpected error
			{
    			if (pWin != NULL) pWin->EnableUserClose(CloseDbDaysWindow);	
    			return;
    		}	
    		if (iStatus == TRUE)   // count the day
			{
				ulDaysFound++;		      
			}	
    		dDay += 1.0;
			//if ((int)dDay == (int)(dEnd)) bSameDay = TRUE;
			bSameDay = SameDay(dDay, dEnd);	// 24-May-2005 use function for SameDay
	    } while (!bSameDay);
		// now read out the information into the common structure
		dDay = dStart;
		pDay = new DayDataStruct[ulDaysFound];
		i=0;
		bSameDay = FALSE;
		do 
		{
			iStatus = glDbDay.ReadDayRecInMainDb(pStaID->sStaID, dDay, &dbDay);
    		if ((iStatus != TRUE) && (iStatus != uiDAY_NOT_IN_DB_STATUS))
			{	// unexpected error
    			if (pWin) pWin->EnableUserClose(CloseDbDaysWindow);
				delete [] pDay;
    			return;
    		}	
    		if (iStatus == TRUE) 
			{  // put the info in the day struct
				pDay[i].dFirstDataPt = dbDay.d_day_beg_time;
				pDay[i].dLastDataPt = dbDay.d_day_end_time;
				pDay[i].ulNumPtsInDay = dbDay.ul_day_total_pts;

				if (i < (int)ulDaysFound) 
					i++;
			}
			
    		dDay += 1.0;
			//if ((int)dDay == (int)(dEnd)) bSameDay = TRUE;
			bSameDay = SameDay(dDay, dEnd);	// 24-May-2005 use function for SameDay

	    } while (!bSameDay);

	}

	// print the day information just retrieved by either method
	ulTotalPts = 0;

	for (i=0; i<(int)ulDaysFound; i++) 
	{
		glMyDateTime.DATETimestampToDateTimeStrs((pDay[i].dFirstDataPt), szDate, szTime);
    	pWin->Printf("\n%s   %5ld    %s",	szDate, pDay[i].ulNumPtsInDay, szTime);

		glMyDateTime.DATETimestampToDateTimeStrs(pDay[i].dLastDataPt, szDate, szTime);
    	pWin->Printf("    %s", szTime);
		
		ulTotalPts += pDay[i].ulNumPtsInDay;
		if (i != int(ulDaysFound-1))
		{
			//bSameDay = FALSE;
			//if ((int)(pDay[i].dFirstDataPt + 1.0) == (int)pDay[i+1].dFirstDataPt) bSameDay = TRUE;
			bSameDay = SameDay((pDay[i].dFirstDataPt + 1.0), pDay[i+1].dFirstDataPt);	// 24-May-2005 use function for SameDay

			if (!bSameDay) 
				pWin->Printf("\nMissing Day(s)");
			
		}
		else {
			//if ((int)(pDay[i].dFirstDataPt + 1.0) == (int)dEnd) bSameDay = TRUE;
			bSameDay = SameDay((pDay[i].dFirstDataPt + 1.0), dEnd);	// 24-May-2005 use function for SameDay
			if (!bSameDay) {
				pWin->Printf("\nMissing Day(s)");
			}
		}
	}

	delete [] pDay;
		    
	pWin->Printf("\nTotal Days = %ld     Total Data Pts = %ld", ulDaysFound, ulTotalPts);
    return;
}

