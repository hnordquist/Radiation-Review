///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgIntegrity.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		DataIntegrityMenuOption
//		DataIntegrityDialogBox
//		DataIntegrityDoIt
//		WriteDefaultDbParameters
//		UpdateDbParms
//		ReadDefaultDbParameters
//		SetCheckboxes
//		SetFlags
//		ValidateStation
//		OnlyDialogBox
//		DoIt
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "DateTimeUtils.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DbInterval.h"
#include "Display.h"
#include "Interval.h"
#include "RadInit.H"
#include "Utilities.h"
#include "FacilityConfig.h" //KM
#include "MyDateTime.h"
#include "TypeDefinitions.h"

#define	AC_POWER_FLAG 		0
#define BATTERY_LOW_FLAG    1
#define AUTHENTICATION_FLAG 2       
#define ORDER_FLAG          3
#define GAP_FLAG            4 
#define COLDSTART_FLAG      5                                                     

extern RADInit *pRAD_Init;  
extern short glsFacNum;
extern char *glnaszIntervalUnitsPicklist[];
extern CMyDateTime glMyDateTime;
extern char glszOldDbName[]; 

static IDStruct gllStaID;
static short gllsStaChoice = 0;
static char **aszStaList = NULL; 
static struct DATA_DISPLAY_FLAGS gllFlags;
static DATE glldReviewPeriodStart = 0.0; 
static DATE glldReviewPeriodEnd = 0.0; 

static unsigned long gllulInterval = 0;
static unsigned int glliUserUnits = 0;
static struct GEN_DATE_STRUCT gllGenDate;
static struct GEN_TIME_STRUCT gllGenTime;
static short gllsTolerance = 1;

static void WriteDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL ReadDefaultDbParameters();
static void SetCheckboxes(CGUI_Dlg *pDlg, short sStaNum);
static void SetFlags(short sStaNum, struct DATA_DISPLAY_FLAGS *pFlags);
static void ValidateStation(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static int OnlyDialogBox(BOOL bAutomated);
static int DoIt();



///////////////////////////////////////////////////////////////////////////////////////////////
//	Perform both the dialog box setup and the actual search
////////////////////////////////////////////////////////////////////////////////////////////////
void DataIntegrityMenuOption()	                                     
{
	if (OnlyDialogBox(FALSE))
		DoIt();
}		


///////////////////////////////////////////////////////////////////////////////////////////////
//	Perform only the dialog box setup 
//	Called when started with /SETUP
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DataIntegrityDialogBox()
{	
	return(OnlyDialogBox(TRUE));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//	Perform just the direction search using parameters in the default db and the ini file
//	Called when started with /IMPORT
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DataIntegrityDoIt()	   
{
	ReadDefaultDbParameters();
	GetReviewPeriodParameters(&glldReviewPeriodStart, &glldReviewPeriodEnd, &gllulInterval);
	glliUserUnits = 0;
	return(DoIt());
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
 *
===========================================================================*/
static void WriteDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	BOOL bBool;
	int status;   
	short sNum;
	CGStr Str;
	unsigned int uiPicklistChoice;
	struct IDStruct StaID;

   /* ------------------------------------------------------------------
    *	Get the current parameters set in the dialog box and write
    *	to default database.
    * ----------------------------------------------------------------*/ 
    pDlg->PickListGetValue(IDC_INTEGRITY_STATION, &uiPicklistChoice);
	if (!pglFacCfg->GetStationIDForPicklistChoice((short)uiPicklistChoice, &StaID)) return;

    status = AccessDefaultDatabase(WRITE, S_INTEGRITY_STATION, &StaID.sStaID);
	if (status != TRUE) goto exit;
    status = AccessDefaultDatabase(WRITE, S_INTEGRITY_STATION_TYPE, &StaID.sStaType);
	if (status != TRUE) goto exit;
    
	pDlg->FldNumGetValue(IDC_INTEGRITY_TOLERANCE, &sNum);
    status = AccessDefaultDatabase(WRITE, S_INTEGRITY_TOLERANCE, &sNum);
	if (status != TRUE) goto exit;

    pDlg->CheckBoxGetValue(IDC_INTEGRITY_AC_POWER, &bBool);
	status = AccessDefaultDatabase(WRITE, B_INTEGRITY_FLAGS, AC_POWER_FLAG, &bBool);
	if (status != TRUE) goto exit;

    pDlg->CheckBoxGetValue(IDC_INTEGRITY_BATTERY_LOW, &bBool);
	status = AccessDefaultDatabase(WRITE, B_INTEGRITY_FLAGS, BATTERY_LOW_FLAG, &bBool);
	if (status != TRUE) goto exit;
	
    pDlg->CheckBoxGetValue(IDC_INTEGRITY_COLDSTART, &bBool);
	status = AccessDefaultDatabase(WRITE, B_INTEGRITY_FLAGS, COLDSTART_FLAG, &bBool);
	if (status != TRUE) goto exit;
	
    pDlg->CheckBoxGetValue(IDC_INTEGRITY_AUTHENTICATION, &bBool);
	status = AccessDefaultDatabase(WRITE, B_INTEGRITY_FLAGS, AUTHENTICATION_FLAG, &bBool);
	if (status != TRUE) goto exit;
	
    pDlg->CheckBoxGetValue(IDC_INTEGRITY_ORDER, &bBool);
	status = AccessDefaultDatabase(WRITE, B_INTEGRITY_FLAGS, ORDER_FLAG,  &bBool);
	if (status != TRUE) goto exit;
	
    pDlg->CheckBoxGetValue(IDC_INTEGRITY_GAP, &bBool);
	status = AccessDefaultDatabase(WRITE, B_INTEGRITY_FLAGS, GAP_FLAG, &bBool);
	if (status != TRUE) goto exit;
	SaveDatabaseNow();   
	return;
	                                                               
exit:
	RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
	return;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//	Asks the user whether to update the default parameters with the ones
//	currently in the dialog box.
////////////////////////////////////////////////////////////////////////////////////////////////
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  

	status = AskUpdateQuestion();
	if (status != TRUE) return;
	                                                                   
	WriteDefaultDbParameters(pDlg, CtrlID);
	
}         


///////////////////////////////////////////////////////////////////////////////////////////////
//	Read default parameters from the default database and set them in the local global variables.
////////////////////////////////////////////////////////////////////////////////////////////////
static BOOL ReadDefaultDbParameters()
{
//	int status;
	
   /* ------------------------------------------------------------------
    *	Get the default station from the Default database.
    * ----------------------------------------------------------------*/
	if ((!AccessDefaultDatabase(READ, S_INTEGRITY_STATION, &gllStaID.sStaID)) ||
		(!AccessDefaultDatabase(READ, S_INTEGRITY_STATION_TYPE, &gllStaID.sStaType)) ||
		(!AccessDefaultDatabase(READ, S_INTEGRITY_TOLERANCE, &gllsTolerance)) ||
		(!AccessDefaultDatabase(READ, B_INTEGRITY_FLAGS, AC_POWER_FLAG, &gllFlags.bACPower)) ||
		(!AccessDefaultDatabase(READ, B_INTEGRITY_FLAGS, BATTERY_LOW_FLAG, &gllFlags.bBatteryLow)) ||
		(!AccessDefaultDatabase(READ, B_INTEGRITY_FLAGS, COLDSTART_FLAG, &gllFlags.bColdStart)) ||
		(!AccessDefaultDatabase(READ, B_INTEGRITY_FLAGS, AUTHENTICATION_FLAG, &gllFlags.bAuthentication)) ||
		(!AccessDefaultDatabase(READ, B_INTEGRITY_FLAGS, ORDER_FLAG,  &gllFlags.bOutOfOrder)) ||
		(!AccessDefaultDatabase(READ, B_INTEGRITY_FLAGS, GAP_FLAG, &gllFlags.bGaps))) {
		RadReviewMsg(uiDFLT_DB_READ_ERR, "Tools | Integrity Dialog Box.  Cannot proceed with menu option.");			
		return(FALSE);
	}

	// make sure you get a valid station from the db
	
		// make sure have a valid station
		if (gllStaID.sStaID == ANY_STATION) {
			if (pglFacCfg->GetNumStations(glsFacNum) < 1) {
  				RadReviewMsg(uiNO_STATIONS_FOR_FACILITY_ERR);
				return(TRUE);
			}
		}
		else {
		if (!pglFacCfg->IsStationEnabled(glsFacNum, &gllStaID)) {
			if (pglFacCfg->GetNumStations(glsFacNum) > 0) {
				pglFacCfg->GetFirstStationID(glsFacNum, &gllStaID);
			}
			else {
  				RadReviewMsg(uiNO_STATIONS_FOR_FACILITY_ERR);
				return(FALSE);
			}
		}
	}

	// fill in the station id struct
	pglFacCfg->GetStationID(glsFacNum, &gllStaID);
	return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////////////////////
//	Based on the station type, enable the appropriate checkboxed in the dialog box
////////////////////////////////////////////////////////////////////////////////////////////////
//static void SetCheckboxes(CGUI_Dlg *pDlg, short sStaNum)
static void SetCheckboxes(CGUI_Dlg *pDlg, struct IDStruct *pStaID)
{
	int i;
	pDlg->SetEnable(IDC_INTEGRITY_ORDER, TRUE);
	
	// have selected one station, check to see what kind it is
	if (pStaID->sStaID >= 0) {
		if (pStaID->sStaType == GRAND_TYPE) {
				pDlg->SetEnable(IDC_INTEGRITY_COLDSTART, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_AUTHENTICATION, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_GAP, TRUE);
			}
			else if (pStaID->sStaType == ISR_TYPE) {											//km
				pDlg->SetEnable(IDC_INTEGRITY_COLDSTART, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AUTHENTICATION, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_GAP, TRUE);
			}
			else if (pStaID->sStaType == MCA_TYPE) {										//km
				pDlg->SetEnable(IDC_INTEGRITY_COLDSTART, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AUTHENTICATION, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_GAP, TRUE);
			}
			else if ((pStaID->sStaType == JSR_TYPE) || (pStaID->sStaType == JSR1_TYPE)) {				//km
				pDlg->SetEnable(IDC_INTEGRITY_COLDSTART, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AUTHENTICATION, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_GAP, TRUE);
			}
			else if (pStaID->sStaType == BINARY_TYPE) {											//km
				pDlg->SetEnable(IDC_INTEGRITY_COLDSTART, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AUTHENTICATION, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_GAP, FALSE);
			}
			else if (pStaID->sStaType == BOT_TYPE) {											//km
				pDlg->SetEnable(IDC_INTEGRITY_COLDSTART, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AUTHENTICATION, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_GAP, TRUE);
			}
			//Set checkboxes for EOSS types, no checks allowed
			else if (pStaID->sStaType == EOSS_TYPE) {											//km
				pDlg->SetEnable(IDC_INTEGRITY_COLDSTART, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AUTHENTICATION, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_GAP, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_ORDER,FALSE);
			}
			
	}
	else {		// have selected all stations, check one by one
		pDlg->SetEnable(IDC_INTEGRITY_COLDSTART, FALSE);
		pDlg->SetEnable(IDC_INTEGRITY_AUTHENTICATION, FALSE);
		pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, FALSE);
		pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, FALSE);
		IDStruct StaID;
		int iNumStas = pglFacCfg->GetNumStations(glsFacNum);
		for (i=0; i<iNumStas; i++) {
			if (!pglFacCfg->GetStationIDForPicklistChoice(i, &StaID)) return;
			if (StaID.sStaType == GRAND_TYPE) {
				pDlg->SetEnable(IDC_INTEGRITY_COLDSTART, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_AUTHENTICATION, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_GAP, TRUE);
				break; //everything turned on, can quit looking.
			}
			else if (StaID.sStaType == ISR_TYPE) {	
				pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_GAP, TRUE);
			}
			else if (StaID.sStaType == MCA_TYPE) {	
				pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, TRUE);
				pDlg->SetEnable(IDC_INTEGRITY_GAP, TRUE);
			}
			//Set checkboxes for EOSS types, no checks allowed
			else if (pStaID->sStaType == EOSS_TYPE) {											//km
				pDlg->SetEnable(IDC_INTEGRITY_COLDSTART, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AUTHENTICATION, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_AC_POWER, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_BATTERY_LOW, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_GAP, FALSE);
				pDlg->SetEnable(IDC_INTEGRITY_ORDER,FALSE);
			}

		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
//	Based on the station type change the flags to only be bits that are valid for that station type.
//	This case is needed for when there may be multiple types of data at the facility
////////////////////////////////////////////////////////////////////////////////////////////////
//static void SetFlags(short sStaNum, struct DATA_DISPLAY_FLAGS *pFlags)
static void SetFlags(IDStruct *pStaID, struct DATA_DISPLAY_FLAGS *pFlags)
{
	// have selected one station, check to see if it is a GRAND
	if (pStaID->sStaID >= 0) {
		if (pStaID->sStaType == GRAND_TYPE) {													
		}
		else if (pStaID->sStaType == ISR_TYPE) {											
			pFlags->bColdStart = 0;
			pFlags->bAuthentication = 0;
			pFlags->bBatteryLow = 0;
		}
		else if (pStaID->sStaType == MCA_TYPE) {										//km
			pFlags->bColdStart = 0;
			pFlags->bAuthentication = 0;
		}
		else if ((pStaID->sStaType == JSR_TYPE) || (pStaID->sStaType == JSR1_TYPE)) {				//km
			pFlags->bColdStart = 0;
			pFlags->bAuthentication = 0;
			pFlags->bBatteryLow = 0;
			pFlags->bACPower = 0;
		}
		else if (pStaID->sStaType == BINARY_TYPE) {											//km
			// none of these apply to BINARY_TYPE data so make sure they aren't set for analysis
			pFlags->bColdStart = 0;
			pFlags->bAuthentication = 0;
			pFlags->bBatteryLow = 0;
			pFlags->bACPower = 0;
			pFlags->bGaps = 0;
			pFlags->bInMII = 0;
		}
		else if (pStaID->sStaType == EOSS_TYPE) {											//km
			// none of these apply to BINARY_TYPE data so make sure they aren't set for analysis
			pFlags->bColdStart = 0;
			pFlags->bAuthentication = 0;
			pFlags->bBatteryLow = 0;
			pFlags->bACPower = 0;
			pFlags->bGaps = 0;
			pFlags->bInMII = 0;
			pFlags->bOutOfOrder = 0;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
//	Executed when you change the station selected in the dialog box.
////////////////////////////////////////////////////////////////////////////////////////////////
static void ValidateStation(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	IDStruct StaID;
	unsigned int uiIndex;

	// Get the current index for the station selected.
	pDlg->PickListGetValue(IDC_INTEGRITY_STATION, &uiIndex);
	pglFacCfg->GetStationIDForPicklistChoice((short)uiIndex, &StaID);
	SetCheckboxes(pDlg, &StaID);
}


///////////////////////////////////////////////////////////////////////////////////////////////
//	This displays only the dialog box part of this menu option
////////////////////////////////////////////////////////////////////////////////////////////////
static int OnlyDialogBox(BOOL bAutomated)
{                                 
//	static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = {""};;
 	static BOOL bFirst = TRUE;
	static int sFacNum;
	static double dDbExactIntervalLastTime = 0;

    CGFmtStr szInterval;
	CGStr DumStr;
	char szFirstDate[DT_LEN+1], szLastDate[DT_LEN+1];
	unsigned long ulDbIntervalInDays, ulTotalPts;
	GUI_ACTION DlgReturn;                                     
	CGFmtStr msg;   
	int iStatus;
	double dDbExactInterval;
	DATE dDum;
	char szFacName[DB_NAME_LEN+1];
	unsigned int uiPicklistChoice;
	
	SetCurrentDrive();

	// verify the facility is defined
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) return(FALSE);
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return(FALSE);

	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);

	// always get the current range of data in the db for display
	iStatus = GetDatabaseIntervalParameters(szFirstDate, szLastDate, &ulDbIntervalInDays, &dDbExactInterval);
	if (iStatus != TRUE) return(FALSE);
	if (dDbExactInterval <= 0.0) {
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return(FALSE);
	}

	// only the first time or when something changes, read the default parameters
	// and get new search intervals (possibly from .ini)
    if ((bFirst == TRUE) || 
		(!SameDb(glszOldDbName)) || 
		(dDbExactInterval != dDbExactIntervalLastTime) ||
		(sFacNum != glsFacNum)) {

		ReadDefaultDbParameters();

		// Get the period the user wants to review
		GetReviewPeriodParameters(&glldReviewPeriodStart, &glldReviewPeriodEnd, &gllulInterval);
		glliUserUnits = 0;

		// remember the current conditions for future entries to this option
		sFacNum = glsFacNum;
	    GetDbName(glszOldDbName);								
		dDbExactIntervalLastTime = dDbExactInterval;
		bFirst = FALSE;
	}
	 
    CGUI_Dlg IntegrityDlg(IDD_TOOLS_INTEGRITY, NULL, DLG_POSN_NO_SAVE);
	if (IntegrityDlg.IsValid()) {	
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		IntegrityDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		IntegrityDlg.DefinePushBtn(IDOK, GUI_OK);
		IntegrityDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateDbParms);  		  
		//This push button was silly, change based on Picklist choices HN 3-26-08
//		IntegrityDlg.DefinePushBtn(IDUPDATECONDITIONS, GUI_NONE, ValidateStation);  		  
	
		// Fill in Facility portion of dialog
		IntegrityDlg.DefineFldStaticTxt(IDC_INTEGRITY_FACILITY);
		IntegrityDlg.FldStaticTxtSetValue(IDC_INTEGRITY_FACILITY, szFacName);

		// Fill in Station portion of dialog
		int iMaxLen;		// longest name in list -- not used
		if (!pglFacCfg->CreateStationPicklist(glsFacNum, TRUE, &aszStaList, &iMaxLen)) return (FALSE);	// list with All Stations as member
		short sTemp;
		pglFacCfg->GetPicklistChoiceForStationID(gllStaID, &sTemp);
		uiPicklistChoice = sTemp;
		IntegrityDlg.DefinePickList(IDC_INTEGRITY_STATION, aszStaList, &uiPicklistChoice);
		//HN 3-26-08  Set choices based on what is chosen.
		IntegrityDlg.SetCtrlNotifyFn(IDC_INTEGRITY_STATION,ValidateStation);

	   /* ------------------------------------------------------------------
	    * 	Display current starting ending database dates and interval
	    * ----------------------------------------------------------------*/
		IntegrityDlg.DefineFldStaticTxt(IDC_INTEGRITY_CURR_DB_START);
		IntegrityDlg.FldStaticTxtSetValue(IDC_INTEGRITY_CURR_DB_START, szFirstDate);
					
		IntegrityDlg.DefineFldStaticTxt(IDC_INTEGRITY_CURR_DB_END);
		IntegrityDlg.FldStaticTxtSetValue(IDC_INTEGRITY_CURR_DB_END, szLastDate);
		
		szInterval.Printf("%ld days", ulDbIntervalInDays);  			
		IntegrityDlg.DefineFldStaticTxt(IDC_INTEGRITY_CURR_DB_INTERVAL);
		IntegrityDlg.FldStaticTxtSetValue(IDC_INTEGRITY_CURR_DB_INTERVAL, szInterval);
		
	   /* ------------------------------------------------------------------
	    *	Fill in default values for search start date and time plus interval
	    * ----------------------------------------------------------------*/
		//Date.DATETimestampToGenStructs(glldReviewPeriodStart, &gllGenDate, &gllGenTime);
		glMyDateTime.DATETimestampToGenStructs(glldReviewPeriodStart, &gllGenDate, &gllGenTime);
		IntegrityDlg.DefineFldDate(IDC_INTEGRITY_SRCH_STRTDATE, &gllGenDate);
		IntegrityDlg.DefineFldTime(IDC_INTEGRITY_SRCH_STRTTIME, &gllGenTime);
			                                                          
		IntegrityDlg.DefineFldNum(IDC_INTEGRITY_SRCH_INTERVAL, &gllulInterval, 1, 10000,"%ld");
		IntegrityDlg.DefinePickList(IDC_INTEGRITY_INTRVL_UNITS_PL, glnaszIntervalUnitsPicklist, &glliUserUnits);

		IntegrityDlg.DefineFldNum(IDC_INTEGRITY_TOLERANCE, &gllsTolerance, 0, 100,"%d");
			
	   /* ------------------------------------------------------------------
	    *	Define all checkboxes
	    * ----------------------------------------------------------------*/
		IntegrityDlg.DefineCheckBox(IDC_INTEGRITY_AC_POWER, &gllFlags.bACPower);		
		IntegrityDlg.DefineCheckBox(IDC_INTEGRITY_BATTERY_LOW, &gllFlags.bBatteryLow);		
		IntegrityDlg.DefineCheckBox(IDC_INTEGRITY_COLDSTART, &gllFlags.bColdStart);		
		IntegrityDlg.DefineCheckBox(IDC_INTEGRITY_AUTHENTICATION, &gllFlags.bAuthentication);
		IntegrityDlg.DefineCheckBox(IDC_INTEGRITY_ORDER, &gllFlags.bOutOfOrder);		
		IntegrityDlg.DefineCheckBox(IDC_INTEGRITY_GAP, &gllFlags.bGaps);		
		SetCheckboxes(&IntegrityDlg, &gllStaID);

		IntegrityDlg.SetFocus(IDC_INTEGRITY_SRCH_INTERVAL);
		if (bAutomated) IntegrityDlg.SetFocus(IDOK);
			
tryagain:	    DlgReturn = IntegrityDlg.Go();
			    
	    if (DlgReturn == GUI_OK) {  
			    	
	    	IntegrityDlg.RetrieveAllControlValues();                                          
	    //	gllsStaChoice = uiTemp;		// do I need this
	    	
		    // Get the station number first, we exit if they pick EOSS HN 3-26-08
			// get the station number associated with the station choice
			if (!pglFacCfg->GetStationIDForPicklistChoice(uiPicklistChoice, &gllStaID)) return(FALSE);

			if (gllStaID.sStaType == EOSS_TYPE)
			{
				GUI_MsgBox ("EOSS instruments cannot be analyzed for data integrity.\nChoose another instrument for analysis.",GUI_ICON_INFO, GUI_OK);
				goto abort;
			}
			
			/* ------------------------------------------------------------------
		    *	Check that at least one condition is checked
		    * ----------------------------------------------------------------*/    
		    if ((gllFlags.bACPower == FALSE ) && (gllFlags.bBatteryLow == FALSE) &&
		    	(gllFlags.bColdStart == FALSE ) && (gllFlags.bAuthentication == FALSE) &&
		    	(gllFlags.bOutOfOrder == FALSE ) && (gllFlags.bGaps == FALSE)) 
			{ 
				GUI_MsgBox("No conditions selected for integrity check.\nTry again.", GUI_ICON_INFO, GUI_OK);
				goto tryagain;
		    }	
			// get the station number associated with the station choice
			if (gllStaID.sStaType == BINARY_TYPE) 
			{
				if (gllFlags.bOutOfOrder == FALSE)
				{
					GUI_MsgBox("Binary data can only be checked for Out of Order condition.\nIntegrity check will abort now.", GUI_ICON_INFO, GUI_OK);
					goto abort;
				}
			}
			// if running in automated mode, then record the parameters
		    if (bAutomated) {
		    	WriteDefaultDbParameters(&IntegrityDlg, IDD_TOOLS_INTEGRITY);
			}	
		   /* ------------------------------------------------------------------
			*	Check to see that there is data for station(s) in the interval selected.
			* ----------------------------------------------------------------*/   
			iStatus = ConvertStructsToSearchIntervalParameters(&gllGenDate, &gllGenTime, glliUserUnits, gllulInterval, &glldReviewPeriodStart, &glldReviewPeriodEnd);
			if (gllStaID.sStaID == -1) {
				if (iStatus != TRUE) {	// no data for any station in the interval
					RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, glldReviewPeriodStart, glldReviewPeriodEnd, "any station");
					goto abort;
				}
			}
			else {
				iStatus = GetDataStatistics(gllStaID, true, glldReviewPeriodStart, glldReviewPeriodEnd, &dDum, &dDum, &ulTotalPts);
				if ((iStatus != TRUE) || (ulTotalPts == 0)) {
					RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, glldReviewPeriodStart, glldReviewPeriodEnd, (const char*)aszStaList[gllsStaChoice]);
					goto abort;
				}
			}
			
			pglFacCfg->DestroyPicklist(aszStaList);
			aszStaList = NULL;
			return(TRUE);
		}
	}
	return(FALSE);

abort:
	pglFacCfg->DestroyPicklist(aszStaList);
	aszStaList = NULL;
    return(FALSE);
}   		                                 
   		                                 

///////////////////////////////////////////////////////////////////////////////////////////////
//	This executes the action after you click on OK in the dialog box
////////////////////////////////////////////////////////////////////////////////////////////////
static int DoIt()
{    

	int iStatus;
	unsigned long ulTotalPts;
	DATE dDum;
	struct DATA_DISPLAY_FLAGS LocalFlags = gllFlags;
	
	SetCurrentDrive();
   /* ------------------------------------------------------------------
	*	Check to see that there is data for station(s) in the interval selected.
	* ----------------------------------------------------------------*/   
	if (gllStaID.sStaID == -1) {
		iStatus = ConvertStructsToSearchIntervalParameters(&gllGenDate, &gllGenTime, glliUserUnits, gllulInterval, &glldReviewPeriodStart, &glldReviewPeriodEnd);
		if (iStatus != TRUE) {	// no data for any station in the interval
			return(TRUE);
		}
	}
	else {
		// temporarily put the flags in a local variable so we can clear them if necessary for unapplicable bits
		// depending on the type of station.
		SetFlags(&gllStaID, &LocalFlags);
		iStatus = GetDataStatistics(gllStaID, true, glldReviewPeriodStart, glldReviewPeriodEnd, &dDum, &dDum, &ulTotalPts);
	    if ((iStatus != TRUE) || (ulTotalPts == 0)) {
			return(TRUE);
		}
	}
	return(DisplayRawData(gllStaID, glldReviewPeriodStart, glldReviewPeriodEnd, &LocalFlags, gllsTolerance));
}	
