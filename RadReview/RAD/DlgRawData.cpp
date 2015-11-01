///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgRawData.cpp     
//Primary Author: Shirley Klosterbuer

#include "gen.h"
#include "RAD.H"
#include "DateTimeUtils.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "DbInterval.h"
#include "Display.h"
#include "DlgRawData.h"
#include "Interval.h"
#include "Utilities.h"
#include "FacilityConfig.h"
#include "MyDateTime.h"
#include "TypeDefinitions.h"
#include <math.h>

extern short glsFacNum;
extern char *glnaszIntervalUnitsPicklist[];
extern CMyDateTime glMyDateTime;
extern char glszOldDbName[DRIVE_LEN+PATH_LEN+1];

static BOOL gllbBit[32];
static CGUI_Dlg *gllpDefDlg = NULL;    

static void DisplayCheckBoxValues(CGUI_Dlg *pDlg, BOOL iState);
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL DefsExitNotifyFn(CGUI_Dlg *pDlg, GUI_ACTION Action);
static void DisplayBitDefinitions(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
        
///////////////////////////////////////////////////////////////////////////
//	Name:	DisplayCheckBoxValues, SelectAll, DeselectAll
//
//	Description:
//	Static functions dealing with the checkboxes in the dialog box.
//
//	Declaration:
//
//	Input:
//			
//	Output:
//
//	Return:
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//////////////////////////////////////////////////////////////////
// sets all boxes in the dialog box to the specified state
static void DisplayCheckBoxValues(CGUI_Dlg *pDlg, BOOL iState)
{
	pDlg->CheckBoxSetValue(IDC_BIT_31, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_30, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_29, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_28, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_27, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_26, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_25, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_24, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_23, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_22, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_21, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_20, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_19, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_18, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_17, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_16, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_15, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_14, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_13, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_12, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_11, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_10, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_9, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_8, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_7, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_6, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_5, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_4, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_3, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_2, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_1, iState);
	pDlg->CheckBoxSetValue(IDC_BIT_0, iState);
}

// Mark all bits as selected
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	int iValue = TRUE;
	DisplayCheckBoxValues(pDlg, iValue);
}        

// Mark all bits as not selected
static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	int iValue = FALSE;
	DisplayCheckBoxValues(pDlg, iValue);

}        
    
///////////////////////////////////////////////////////////////////////////
//	Name:	DefsExitNotifyFn, DisplayBitDefinitions
//
//	Description:
//	Static functions dealing with the auxiliary dialog box with the status
//	bit definitions for each of the instrument types
//
//	Declaration:
//
//	Input:
//			
//	Output:
//
//	Return:
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//////////////////////////////////////////////////////////////////
static BOOL DefsExitNotifyFn(CGUI_Dlg *pDlg, GUI_ACTION Action)
{   
	BOOL bCanClose = TRUE;
	gllpDefDlg = NULL;	// signal to ourselves that the modeless dialog box is being deleted
	return(bCanClose);
}

/*
// Create and Display a second dialog box with the bit definitions for the current station
static void DisplayBitDefinitions(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	unsigned int uiChoice;
	struct IDStruct StaID;

	if (gllpDefDlg) 
		return;

	// Figure out which station is selected in the main dialog box
	pDlg->PickListGetValue(IDC_RD_STATION, &uiChoice);
	if (!pglFacCfg->GetStationIDForPicklistChoice(uiChoice, &StaID)) 
		return;

	// Based on the station type, create and display a new definitions dialog box with the bit definitions
	if (StaID.sStaType == GRAND_TYPE) {
		gllpDefDlg = new CGUI_Dlg (IDD_DEFS_GRAND_BITS, NULL, DLG_POSN_NO_SAVE); //12-Jul-2005 SFK Changed from rel to no save
		if (gllpDefDlg != NULL) {
			gllpDefDlg->DefinePushBtn(IDOK, GUI_OK);
			::SetWindowPos(gllpDefDlg->GetWndHandle(), HWND_TOP, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
			gllpDefDlg->GoModeless(TRUE);
		}
	}
	else if ((StaID.sStaType == JSR_TYPE) || (StaID.sStaType == JSR1_TYPE) || (StaID.sStaType == ISR_TYPE)) {			//km
		gllpDefDlg = new CGUI_Dlg (IDD_DEFS_SR_BITS, NULL, DLG_POSN_NO_SAVE); //12-Jul-2005 SFK Changed from rel to no save
		if (gllpDefDlg != NULL) {
			gllpDefDlg->DefinePushBtn(IDOK, GUI_OK);
			::SetWindowPos(gllpDefDlg->GetWndHandle(), HWND_TOP, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
			gllpDefDlg->GoModeless(TRUE);
		}
	}
	else if (StaID.sStaType == MCA_TYPE) {
		gllpDefDlg = new CGUI_Dlg (IDD_DEFS_MCA_BITS, NULL, DLG_POSN_NO_SAVE); //12-Jul-2005 SFK Changed from rel to no save
		if (gllpDefDlg != NULL) {
			gllpDefDlg->DefinePushBtn(IDOK, GUI_OK);
			::SetWindowPos(gllpDefDlg->GetWndHandle(), HWND_TOP, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
			gllpDefDlg->GoModeless(TRUE);
		}
	}
	else if (StaID.sStaType == BINARY_TYPE) {
		gllpDefDlg = new CGUI_Dlg (IDD_DEFS_BINARY_BITS, NULL, DLG_POSN_NO_SAVE); //12-Jul-2005 SFK Changed from rel to no save
		if (gllpDefDlg != NULL) {
			gllpDefDlg->DefinePushBtn(IDOK, GUI_OK);
			::SetWindowPos(gllpDefDlg->GetWndHandle(), HWND_TOP, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
			gllpDefDlg->GoModeless(TRUE);
		}
	}
	else if (StaID.sStaType == BOT_TYPE){
		MessageBox(NULL, "No status bits defined for VIFM data", "Radiation Review: VIFM Status Byte Definition", MB_OK);
	}

	// set up the notification function for when the user closes the dialog box just created
	if (gllpDefDlg) gllpDefDlg->SetDlgExitValidateFn(DefsExitNotifyFn);			
}
*/
// Create and Display a second dialog box with the bit definitions for the current station
static void DisplayBitDefinitions(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	if (gllpDefDlg)
		return;

	unsigned int uiChoice;
	struct IDStruct StaID;

	// Figure out which station is selected in the main dialog box
	pDlg->PickListGetValue(IDC_RD_STATION, &uiChoice);

	if (!pglFacCfg->GetStationIDForPicklistChoice(uiChoice, &StaID)) 
		return;

	unsigned int uiIDD = 0;

	// Based on the station type, create and display a new definitions dialog box with the bit definitions

	switch(StaID.sStaType)
	{
		case GRAND_TYPE:	uiIDD = IDD_DEFS_GRAND_BITS; break;
		case JSR_TYPE:
		case JSR1_TYPE:
		case ISR_TYPE:		uiIDD = IDD_DEFS_SR_BITS; break;
		case MCA_TYPE:		uiIDD = IDD_DEFS_MCA_BITS; break;
		case BINARY_TYPE:   uiIDD = IDD_DEFS_BINARY_BITS;break;
		//Add EOSS here
		case EOSS_TYPE:
			//Change this, there are no status bits for an EOSS instrument 4-08-08 HN
			//uiIDD = IDD_DEFS_BINARY_BITS; break;
			MessageBox(NULL, "No status bits defined for EOSS data", "Radiation Review: EOSS Status Byte Definition", MB_OK);
			break;
		case BOT_TYPE: 
			MessageBox(NULL, "No status bits defined for VIFM data", "Radiation Review: VIFM Status Byte Definition", MB_OK);
			break;
	};

	if (uiIDD != 0)
		gllpDefDlg = new CGUI_Dlg (uiIDD, NULL, DLG_POSN_NO_SAVE);

	if (gllpDefDlg != NULL) 
	{
		gllpDefDlg->DefinePushBtn(IDOK, GUI_OK);
		::SetWindowPos(gllpDefDlg->GetWndHandle(), HWND_TOP, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
		gllpDefDlg->GoModeless(TRUE);
		gllpDefDlg->SetDlgExitValidateFn(DefsExitNotifyFn);
	}
}          
                

///////////////////////////////////////////////////////////////////////////
//	Name:	RawDataMenuOption
//
//	Description:
//	Function that is called when the Diagnostics/Display Raw Data
//	option is selected from the main menu of Rad Review.
//
//	Declaration:
//	void RawDataMenuOption()
//
//	Input: none
//			
//	Output: none
//
//	Return: none
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//////////////////////////////////////////////////////////////////
void RawDataMenuOption()
{                                 
    static unsigned long ulInterval = 0;	
	static DATE dReviewPeriodStart = 0;			// replaces ulUserIntervalStart
	static unsigned int iIntervalUnits = 0;                 
	//static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = {""};;
	static struct IDStruct StaID;
	static short sFacNum;
 	static BOOL bFirst = TRUE;
	static double dDbExactIntervalLastTime = 0.0;	// replaces ulLastSearchIntervalInSecs
	static BOOL bLogFile = FALSE;
	static BOOL bChattyBits = TRUE;	// default is verbose display	
	static bAllData;
	struct GEN_DATE_STRUCT GenDate;
    struct GEN_TIME_STRUCT GenTime;
                                   
	CGFmtStr szInterval;
	char szFirstDate[DT_LEN+1];              
	char szLastDate[DT_LEN+1];              

    char **aszStaList = NULL; 
	int status;     
	unsigned int iStaChoice;
	unsigned long ulDbIntervalInDays, ulTotalPts;
	double dDbExactInterval;
	DATE dReviewPeriodEnd;	// replaces ulEnd
	
	GUI_ACTION DlgReturn;                                     
	CGFmtStr msg;   
	int i;
	BOOL bSelected;
	BOOL bStatus;
                             
	// verify the facility is defined
	char szFacName[DB_NAME_LEN+1];
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) 
		return;

	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) 
		return;

	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);

   /* ------------------------------------------------------------------
    *	Always find the first and last days in the database and set
	*	up the strings for display in the dialog box.
    * ----------------------------------------------------------------*/
	SetCurrentDrive();

	bStatus = GetDatabaseIntervalParameters(szFirstDate, szLastDate, &ulDbIntervalInDays, &dDbExactInterval);

	if (!bStatus) 
		return;

	if (dDbExactInterval <= 0.0) 
	{
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return;
	}
	

	// if anything has changed, then start over again
    if (bFirst || 
		(!SameDb(glszOldDbName)) || 
		(dDbExactInterval != dDbExactIntervalLastTime) ||
		(sFacNum != glsFacNum)) {
	   /* ------------------------------------------------------------------
	    *	Get the default station from the Default database.
	    * ----------------------------------------------------------------*/
       	if ((AccessDefaultDatabase(READ, S_DFLT_STATION, &StaID.sStaID) != TRUE) || 
		    (AccessDefaultDatabase(READ, S_DFLT_STATION_TYPE, &StaID.sStaType) != TRUE)) 
		{
  			RadReviewMsg(uiFILE_READ_ERR, "when reading from Default database.  Cannot proceed.");			
			return;
		} 

		// make sure the default is a valid station; otherwise use first in facility
		if (!pglFacCfg->IsStationEnabled(glsFacNum, &StaID)) 
		{
			if (pglFacCfg->GetNumStations(glsFacNum) > 0) 
			{
				pglFacCfg->GetFirstStationID(glsFacNum, &StaID);
			}
			else 
			{
  				RadReviewMsg(uiNO_STATIONS_FOR_FACILITY_ERR);
				return;
			}
		}

		// Get the period the user wants to review
		GetReviewPeriodParameters(&dReviewPeriodStart, &dReviewPeriodEnd, &ulInterval);
		dReviewPeriodStart = floor(dReviewPeriodStart);		// 22-Aug-2005 SFK Start at 00:00
		iIntervalUnits = 0;
	
		// First time in the dialog box, all bit definitions are unselected.
		for (i=0; i<32; i++)
			gllbBit[i] = FALSE;

		bAllData = FALSE;

		// remember the current conditions for future entries to this option
		sFacNum = glsFacNum;
	    GetDbName(glszOldDbName);								
		dDbExactIntervalLastTime = dDbExactInterval;

		bFirst = FALSE;
	}


	// Create and display the dialog box with the choices for how to display the raw data
    CGUI_Dlg RawDataDlg(IDD_DIAG_RAWDATA, NULL, DLG_POSN_NO_SAVE);	//12-Jul-2005 SFK Changed from rel to no save
    if (RawDataDlg.IsValid()) 
	{	    
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		RawDataDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		RawDataDlg.DefinePushBtn(IDOK, GUI_OK);
		RawDataDlg.DefinePushBtn(IDC_SELECT_ALL, GUI_NONE, SelectAll);
		RawDataDlg.DefinePushBtn(IDC_DESELECT_ALL, GUI_NONE, DeselectAll);
		RawDataDlg.DefinePushBtn(IDC_DSP_BIT_DEFS, GUI_NONE, DisplayBitDefinitions);
		
	   /* ------------------------------------------------------------------
	    *	Label the facility
	    * ----------------------------------------------------------------*/
		RawDataDlg.DefineFldStaticTxt(IDC_RD_FACILITY);
		RawDataDlg.FldStaticTxtSetValue(IDC_RD_FACILITY, szFacName);
			
	   /* ------------------------------------------------------------------
	    *	Set up station choices
	    * ----------------------------------------------------------------*/
		int iDum;

		if (!pglFacCfg->CreateStationPicklist(glsFacNum, false, &aszStaList, &iDum)) 
			return;

		short sTemp;
		if (!pglFacCfg->GetPicklistChoiceForStationID(StaID, &sTemp))
			return;

		iStaChoice = sTemp;
		RawDataDlg.DefinePickList(IDC_RD_STATION, aszStaList, &iStaChoice);
			
	   /* ------------------------------------------------------------------
	    *	Display current starting ending database dates and interval
	    * ----------------------------------------------------------------*/
		RawDataDlg.DefineFldStaticTxt(IDC_RD_CURR_DB_START);
		RawDataDlg.FldStaticTxtSetValue(IDC_RD_CURR_DB_START, szFirstDate);
					
		RawDataDlg.DefineFldStaticTxt(IDC_RD_CURR_DB_END);
		RawDataDlg.FldStaticTxtSetValue(IDC_RD_CURR_DB_END, szLastDate);

		szInterval.Printf("%ld days", ulDbIntervalInDays); 
		RawDataDlg.DefineFldStaticTxt(IDC_RD_CURR_DB_INTERVAL);
		RawDataDlg.FldStaticTxtSetValue(IDC_RD_CURR_DB_INTERVAL, szInterval);
		
	   /* ------------------------------------------------------------------
	    *	Fill in default values for Review Period (search interval)
		*	start date and time plus interval
	    * ----------------------------------------------------------------*/
		//Date.DATETimestampToGenStructs(dReviewPeriodStart, &GenDate, &GenTime);
		glMyDateTime.DATETimestampToGenStructs(dReviewPeriodStart, &GenDate, &GenTime);
		RawDataDlg.DefineFldDate(IDC_RD_SRCH_STRTDATE, &GenDate);
		RawDataDlg.DefineFldTime(IDC_RD_SRCH_STRTTIME, &GenTime);
			                                                          
		RawDataDlg.DefineFldNum(IDC_RD_SRCH_INTERVAL, &ulInterval, 1, 10000,"%ld");
		RawDataDlg.DefinePickList(IDC_RD_INTRVL_UNITS_PL, glnaszIntervalUnitsPicklist, &iIntervalUnits);
			
	   /* ------------------------------------------------------------------
	    *	Define all checkboxes
	    * ----------------------------------------------------------------*/
		RawDataDlg.DefineCheckBox(IDC_ALL_PTS, &bAllData);	
		RawDataDlg.DefineCheckBox(IDC_LOG_FILE, &bLogFile);
		RawDataDlg.DefineCheckBox(IDC_CHATTY_BITS, &bChattyBits);

		RawDataDlg.DefineCheckBox(IDC_BIT_31, &gllbBit[31]);
		RawDataDlg.DefineCheckBox(IDC_BIT_30, &gllbBit[30]);
		RawDataDlg.DefineCheckBox(IDC_BIT_29, &gllbBit[29]);
		RawDataDlg.DefineCheckBox(IDC_BIT_28, &gllbBit[28]);
		RawDataDlg.DefineCheckBox(IDC_BIT_27, &gllbBit[27]);
		RawDataDlg.DefineCheckBox(IDC_BIT_26, &gllbBit[26]);
		RawDataDlg.DefineCheckBox(IDC_BIT_25, &gllbBit[25]);
		RawDataDlg.DefineCheckBox(IDC_BIT_24, &gllbBit[24]);
		RawDataDlg.DefineCheckBox(IDC_BIT_23, &gllbBit[23]);
		RawDataDlg.DefineCheckBox(IDC_BIT_22, &gllbBit[22]);
		RawDataDlg.DefineCheckBox(IDC_BIT_21, &gllbBit[21]);
		RawDataDlg.DefineCheckBox(IDC_BIT_20, &gllbBit[20]);
		RawDataDlg.DefineCheckBox(IDC_BIT_19, &gllbBit[19]);
		RawDataDlg.DefineCheckBox(IDC_BIT_18, &gllbBit[18]);
		RawDataDlg.DefineCheckBox(IDC_BIT_17, &gllbBit[17]);
		RawDataDlg.DefineCheckBox(IDC_BIT_16, &gllbBit[16]);
		RawDataDlg.DefineCheckBox(IDC_BIT_15, &gllbBit[15]);
		RawDataDlg.DefineCheckBox(IDC_BIT_14, &gllbBit[14]);
		RawDataDlg.DefineCheckBox(IDC_BIT_13, &gllbBit[13]);
		RawDataDlg.DefineCheckBox(IDC_BIT_12, &gllbBit[12]);
		RawDataDlg.DefineCheckBox(IDC_BIT_11, &gllbBit[11]);
		RawDataDlg.DefineCheckBox(IDC_BIT_10, &gllbBit[10]);
		RawDataDlg.DefineCheckBox(IDC_BIT_9, &gllbBit[9]);
		RawDataDlg.DefineCheckBox(IDC_BIT_8, &gllbBit[8]);
		RawDataDlg.DefineCheckBox(IDC_BIT_7, &gllbBit[7]);
		RawDataDlg.DefineCheckBox(IDC_BIT_6, &gllbBit[6]);
		RawDataDlg.DefineCheckBox(IDC_BIT_5, &gllbBit[5]);
		RawDataDlg.DefineCheckBox(IDC_BIT_4, &gllbBit[4]);
		RawDataDlg.DefineCheckBox(IDC_BIT_3, &gllbBit[3]);
		RawDataDlg.DefineCheckBox(IDC_BIT_2, &gllbBit[2]);
		RawDataDlg.DefineCheckBox(IDC_BIT_1, &gllbBit[1]);
		RawDataDlg.DefineCheckBox(IDC_BIT_0, &gllbBit[0]);
		                             
		RawDataDlg.SetFocus(IDC_RD_SRCH_INTERVAL);
			
	    DlgReturn = RawDataDlg.Go();
			
		// Have exited the dialog box, now do the necessary actions, if any.
	    if (DlgReturn == GUI_OK) 
		{  
			// get all the values out of the dialog box			    	
	    	RawDataDlg.RetrieveAllControlValues();
	    	GetDbName(glszOldDbName);

		   /* ------------------------------------------------------------------
		    *	Check that at least one condition is checked for displaying some bits
		    * ----------------------------------------------------------------*/  
			bSelected = FALSE;

			if (!bAllData) 
			{	// did they ask to display all the raw data points
				for (i=0; i<32; i++) 
				{	// look at the individual bits
					if (gllbBit[i]) 
						bSelected = TRUE;
				}
				if (!bSelected) 
				{	
		    		RadReviewMsg(uiNO_MATCHING_RAW_DATA_WARN);
		    		goto exit;
				}
		    }	

			// They've asked to display some data, now is there any data in the interval specified
			//	for any station?
			bStatus = ConvertStructsToSearchIntervalParameters(&GenDate, &GenTime, iIntervalUnits, ulInterval, &dReviewPeriodStart, &dReviewPeriodEnd);

			if (!bStatus) 
			{	// no data for any station in the interval
				RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, dReviewPeriodStart, dReviewPeriodEnd, "any station");
				goto exit;
			}

			//	Check to see that there is data for this day and channel in the database.  
			//	If no data for this channel in the specified time interval, then quit.
			if (!pglFacCfg->GetStationIDForPicklistChoice(iStaChoice, &StaID)) 
				return;

			//DATE dDum,dDum1;
			DATE dSpecificStartTime,dSpecificEndTime;

			status = GetDataStatistics(
				StaID, 
				TRUE,  //for all channels at station
				dReviewPeriodStart, //<<QA>>
				dReviewPeriodEnd, 
				&dSpecificStartTime, 
				&dSpecificEndTime, &ulTotalPts);

		    if (!status || (ulTotalPts == 0))
		    	RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, dReviewPeriodStart, dReviewPeriodEnd, (const char *)aszStaList[iStaChoice]);
		    else
				DisplayRawData(StaID, dSpecificStartTime, dSpecificEndTime, bAllData, gllbBit, bLogFile, bChattyBits);
				//DisplayRawData(StaID, dReviewPeriodStart, dReviewPeriodEnd, bAllData, gllbBit, bLogFile);

	    }	      
exit:	    
		if (gllpDefDlg)	
		{ // close the definitions box if the user hasn't already done it
			gllpDefDlg->Close();
			gllpDefDlg = NULL;
		}
		pglFacCfg->DestroyPicklist(aszStaList);
		aszStaList = NULL;
	}    
}   		                                 
