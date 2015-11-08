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
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support time synchronization of data
//	from various stations.
//
//	Functions:
//		AdjustStationTimeMenuOption()
//		CalculateStationAdjustSlopeAndIntercept()
//		AdjustTimestampZeroBased()
//		AdjustTimestamp()
//		CleanUpAdjust()
//		WriteDefaultDbParameters()
//		ReadDefaultDbParameters()
//		MyUpdateGraphData()
///////////////////////////////////////////////////////////////////////////
#include "gui_tbl.h"
#include "gen.h"       
#include "RAD.H"                        
#include "AdjustTime.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "Interval.h"
#include "FacilityConfig.h"
#include "Plot.h"
#include "Utilities.h"
#include "TimeAlign.h"

extern short glsFacNum;

// parameters need for stations table
static int			glliNumStas = -1;				// number of stations in this facility
static char			**gllaszStaList = NULL;			// strings containing a list of the station names
static short		gllsMaxNameLen;					// number of characters in the longest station name
static BOOL			*pgllbUseStation = NULL;		// whether or not the station is selected to be time adjusted

// parameters for table Time 1
static char			**pgllDate1 = NULL;				// pointers to the gllszDate1 strings
static char			(*gllszDate1)[DT_LEN+1] = NULL;	// strings containing the time1 date
static char			**pgllTime1 = NULL;				// pointers to the gllszTime1 strings
static char			(*gllszTime1)[DT_LEN+1] = NULL;	// strings containing the time1 time

// parameters for table Time 2
static char			**pgllDate2 = NULL;				// pointers to the gllszDate2 strings
static char			(*gllszDate2)[DT_LEN+1] = NULL;	// strings containing the time2 date
static char			**pgllTime2 = NULL;				// pointers to the gllszTime2 strings
static char			(*gllszTime2)[DT_LEN+1] = NULL;	// strings containing the time2 time
// table checkbox parameter
static BOOL			gllbUseAdjustments = FALSE;		// whether or not time adjustments should be made for any station

// slope and intercept for all the stations
static double		*pglldIntercept = NULL;			// array of intercepts for all the stations
static double		*pglldSlope = NULL;				// array of slopes for all the stations

static void MyUpdateGraphData(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void CalculateStationAdjustSlopeAndIntercept(CGUI_Dlg *pDlg);
static void WriteDefaultDbParameters(CGUI_Dlg *pDlg);
static BOOL ReadDefaultDbParameters();

///////////////////////////////////////////////////////////////////////////
//	This function performs the actions for the menu option "Adjust Station
//	Timestamp".  The dialog box is displayed and values are entered.  At exit
//	from the dialog box or when the "Update Graph" key is clicked, the parameter
//	values are read, stored in the database and the graph is updated, if
//	displayed.
//
//	void AdjustStationTimeMenuOption()
//
//	Input:
//		none
//
//	Output:
//		none
//	
//	External Dependencies:
//	Multiple functions to display the dialog box, to read/write to the parameter
//	database and to calculate the slope/intercept.
//	Parameters with gll prefix are defined at the top of this file.
///////////////////////////////////////////////////////////////////////////
void AdjustStationTimeMenuOption()
{                                 
	static char	szFirstDbDate[DT_LEN+1], szLastDbDate[DT_LEN+1];              
	static unsigned long ulDbIntervalInDays = -1;
	
	static unsigned int uiUseStationCol;	
	static unsigned int uiStaCol; 
	static unsigned int uiDate1Col;
	static unsigned int uiTime1Col;
	static unsigned int uiDate2Col;
	static unsigned int uiTime2Col;

	CGFmtStr szInterval;
	GUI_ACTION DlgReturn;                                     
	CGUI_Table *pTbl;    

	int status = TRUE;     
	DATE dDum;
	char szFacName[DB_NAME_LEN+1];
	
	// verify the facility is defined
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) return;

	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return;


	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);


	///////////////////////////////////////////////////////////////////////////
    //	Determine the interval of data in the database, also read the default
	//	parameter values from the default database.
	///////////////////////////////////////////////////////////////////////////
	SetCurrentDrive();
	status = GetDatabaseIntervalParameters(szFirstDbDate, szLastDbDate, &ulDbIntervalInDays, &dDum);
	if (!status) return;
	if (dDum <= 0.0) {
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return;
	}

	ReadDefaultDbParameters();
	
	//	Create the dialog box to display the timestamp adjustment parameters
    CGUI_Dlg Dlg(IDD_TOOLS_ADJUST, NULL, DLG_POSN_NO_SAVE);
    if (Dlg.IsValid()) {
		//	Define the buttons and actions in the dialog box
		Dlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		Dlg.DefinePushBtn(IDOK, GUI_OK);
		Dlg.DefinePushBtn(ID_UPDATE, GUI_NONE, MyUpdateGraphData);
				                                               
		//	Define the facility
		Dlg.DefineFldStaticTxt(IDC_ADJUST_FACILITY);
		Dlg.FldStaticTxtSetValue(IDC_ADJUST_FACILITY, szFacName);

		//	Display current starting ending database dates and interval
		Dlg.DefineFldStaticTxt(IDC_ADJUST_CURR_DB_START);
		Dlg.FldStaticTxtSetValue(IDC_ADJUST_CURR_DB_START, szFirstDbDate);
		Dlg.DefineFldStaticTxt(IDC_ADJUST_CURR_DB_END);
		Dlg.FldStaticTxtSetValue(IDC_ADJUST_CURR_DB_END, szLastDbDate);

		szInterval.Printf("%ld days", ulDbIntervalInDays);  	
		Dlg.DefineFldStaticTxt(IDC_ADJUST_CURR_DB_INTERVAL);
		Dlg.FldStaticTxtSetValue(IDC_ADJUST_CURR_DB_INTERVAL, szInterval);
		
		//	Create a table consisting of columns for selection, station names,
		//	and two times that represent the same event for each station selected. 
	    Dlg.DefineTable(IDC_ADJUST_TABLE, &pTbl);	
		if (pTbl != NULL) {
			uiUseStationCol = pTbl->AddColCheckBox("Use", glliNumStas, pgllbUseStation);
			uiStaCol = pTbl->AddColTxt("Station", glliNumStas, gllaszStaList, gllsMaxNameLen+1);
			uiDate1Col = pTbl->AddColTxt("First Date", glliNumStas, pgllDate1, DT_LEN+1);
			uiTime1Col = pTbl->AddColTxt("First Time", glliNumStas, pgllTime1, DT_LEN+1);
			uiDate2Col = pTbl->AddColTxt("Second Date", glliNumStas, pgllDate2, DT_LEN+1);
			uiTime2Col = pTbl->AddColTxt("Second Time", glliNumStas, pgllTime2, DT_LEN+1);
			pTbl->SetColWriteEnable(uiStaCol, FALSE);				// cannot edit the station names
		}	

		Dlg.DefineCheckBox(IDC_ADJUST_CHECKBOX, &gllbUseAdjustments);
		Dlg.SetFocus(IDOK);
			
	    DlgReturn = Dlg.Go();
	    if (DlgReturn == GUI_OK) {  
	    	Dlg.RetrieveAllControlValues();							// get all values from dialog box
	    	if (pTbl != NULL) pTbl->RetrieveAllCellValues();		// retrieve the table values

//			CalculateStationAdjustSlopeAndIntercept(&Dlg);			// determine new slope and intercept

			WriteDefaultDbParameters(&Dlg);							// write values to the default db
			UpdateGraphData(FALSE);									// if the graph is displayed, update its data now.
	    }	      
	}
    
	CleanUpAdjust();												// deallocate any memory used
	return;
}   		                                 


///////////////////////////////////////////////////////////////////////////
//	This function calculates the timestamp adjustment for the given
//	time at the station, if requested.  It assumes the timestamps are 
//	zero-based (i.e. the time of the point associated with dStart is 0).
//
//	double AdjustTimestampZeroBased(short sSta, double dOriginalTimestamp, double dStart, double dEnd)
//
//	Input:
//		sSta 				database number for station
//		dOriginalTimestamp	unadjusted timestamp
//		dStart				first time in interval being processed
//		dEnd				last time in interval being processed
//
//	Output:
//		returns adjusted timestamp value
//	
//	External Dependencies:
//	Values for the slope and intercept are stored in the parameter database.
//	Parameters with gll prefix are defined at the top of this file.
// 11-Dec-2003	SFK	Implemented IDStruct
///////////////////////////////////////////////////////////////////////////
double AdjustTimestampZeroBased(IDStruct StaID, double dOriginalTimestamp, double dStart, double dEnd)
{
//	struct db_sta_dflt_rec dbStaDflt;
//	int status;
	double dSlope = 1.0;
	double dIntercept = 0.0;
	double dAdjustedTimestamp = dOriginalTimestamp - dStart;
	short sPick;
	
	if (gllbUseAdjustments == TRUE) {
		if (!pglFacCfg->GetPicklistChoiceForStationID(StaID, &sPick)) return(dAdjustedTimestamp);	// get picklist entry associated with station
		dSlope = pglldSlope[sPick];
		dIntercept = pglldIntercept[sPick];
		dAdjustedTimestamp = (dOriginalTimestamp - dIntercept)/dSlope;	// adjust the base time
    	dAdjustedTimestamp = dAdjustedTimestamp - dStart;				// subtract the start time offset for graphing
		if (dAdjustedTimestamp < 0.0) dAdjustedTimestamp = 0.0;			// handle case before start time
		if (dAdjustedTimestamp > (dEnd - dStart)) 
			dAdjustedTimestamp = dEnd - dStart;							// handle case after end time
	}
	return(dAdjustedTimestamp);
}


///////////////////////////////////////////////////////////////////////////
//	This function calculates the timestamp adjustment for the given
//	time at the station, if requested.
//
//	double AdjustTimestamp(short sSta, double dOriginalTimestamp, double dStart, double dEnd)
//
//	Input:
//		sSta 			database number for station
//		dOriginalTimestamp	unadjusted timestamp
//		dStart				first time in interval being processed
//		dEnd				last time in interval being processed
//
//	Output:
//		returns adjusted timestamp value
//	
//	External Dependencies:
//	Values for the slope and intercept are stored in the parameter database.
//	Parameters with gll prefix are defined at the top of this file.
// 11-Dec-2003	SFK	Implemented IDStruct
///////////////////////////////////////////////////////////////////////////
double AdjustTimestamp(IDStruct StaID, double dOriginalTimestamp)
{
	double dSlope = 1.0;
	double dIntercept = 0.0;
	double dAdjustedTimestamp = dOriginalTimestamp;
	short sPick;
	
	if (gllbUseAdjustments == TRUE) {
		if (!pglFacCfg->GetPicklistChoiceForStationID(StaID, &sPick)) return(dAdjustedTimestamp);	// get picklist entry associated with station
		dSlope = pglldSlope[sPick];
		dIntercept = pglldIntercept[sPick];
		dAdjustedTimestamp = (dOriginalTimestamp - dIntercept)/dSlope;	// adjust the base time
	}
	return(dAdjustedTimestamp);
}


///////////////////////////////////////////////////////////////////////////
//	This function undoes the timestamp adjustment for the given
//	time at the station, if requested.
//
//	double UnadjustTimestamp(short sSta, double dOriginalTimestamp, double dStart, double dEnd)
//
//	Input:
//		sSta 				database number for station
//		dOriginalTimestamp	adjusted timestamp
//		dStart				first time in interval being processed
//		dEnd				last time in interval being processed
//
//	Output:
//		returns unadjusted timestamp value
//	
//	External Dependencies:
//	Values for the slope and intercept are stored in the parameter database.
//	Parameters with gll prefix are defined at the top of this file.
// 11-Dec-2003	SFK	Implemented IDStruct
///////////////////////////////////////////////////////////////////////////
double UnadjustTimestamp(IDStruct StaID, double dAdjustedTimestamp)
{
	double dSlope = 1.0;
	double dIntercept = 0.0;
	double dOriginalTimestamp = dAdjustedTimestamp;
	short sPick;
	
	if (gllbUseAdjustments == TRUE) {
		if (!pglFacCfg->GetPicklistChoiceForStationID(StaID, &sPick)) return(dOriginalTimestamp);	// get picklist entry associated with station
		dSlope = pglldSlope[sPick];
		dIntercept = pglldIntercept[sPick];
		dOriginalTimestamp = (dAdjustedTimestamp * dSlope) + dIntercept;
	}
	return(dOriginalTimestamp);
}


///////////////////////////////////////////////////////////////////////////
//	This function deallocates any memory used in the dialog box.  Most were
//	allocated in ReadDefaultDbParameters().
//
//	void CleanUpAdjust()
//
//	Input:
//		None
//
//	Output:
//		None
//	
//	External Dependencies:
//	Parameters with gll prefix are defined at the top of this file.
///////////////////////////////////////////////////////////////////////////
void CleanUpAdjust()
{
	if (gllaszStaList) gen_deallocate_str_array(gllaszStaList);
	gllaszStaList = NULL;
	
	if (pgllbUseStation) free(pgllbUseStation);
	pgllbUseStation = NULL;
	
	if (pgllDate1) free(pgllDate1);
	pgllDate1 = NULL;
	if (gllszDate1) free(gllszDate1);
	gllszDate1 = NULL;
	if (pgllTime1) free(pgllTime1);
	pgllTime1 = NULL;
	if (gllszTime1) free(gllszTime1);
	gllszTime1 = NULL;
	
	if (pgllDate2) free(pgllDate2);
	pgllDate2 = NULL;
	if (gllszDate2) free(gllszDate2);
	gllszDate2 = NULL;
	if (pgllTime2) free(pgllTime2);
	pgllTime2 = NULL;
	if (gllszTime2) free(gllszTime2);
	gllszTime2 = NULL;
	
	if (pglldSlope) free(pglldSlope);
	pglldSlope = NULL;
	
	if (pglldIntercept) free(pglldIntercept);
	pglldIntercept = NULL;	
}


///////////////////////////////////////////////////////////////////////////
//	This function is called when the Update Graph button is clicked on
//	the dialog box.  It causes the parameters to be read from the dialog
//	box, slope and intercept to be calculated and written to the database,
//	and the graph to be redrawn, if displayed.
//
//	static void MyUpdateGraphData(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
//
//	Input:
//		pDlg	dialog box where click occurred
//		CtrlID	dialog box ID that was clicked.
//
//	Output:
//		None
//	
//	External Dependencies:
///////////////////////////////////////////////////////////////////////////
static void MyUpdateGraphData(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	pDlg->RetrieveAllControlValues();					// get all values from dialog box
	CalculateStationAdjustSlopeAndIntercept(pDlg);		// determine new slope and intercept
	WriteDefaultDbParameters(pDlg);						// write values to the default db
	UpdateGraphData(TRUE);								// graph with the new values
}



///////////////////////////////////////////////////////////////////////////
//	This function calculates the slope and intercepts for all selected
//	stations.  The timestamp of the first station selected is used as the 
//	time to which all other station timestamps will be adjusted.
//
//	static void CalculateStationAdjustSlopeAndIntercept(CGUI_Dlg *pDlg)
//
//	Input:
//		pDlg	dialog box containing the table with the information
//
//	Output:
//		None
//	
//	External Dependencies:
//	Parameters with gll prefix are defined at the top of this file.
///////////////////////////////////////////////////////////////////////////
static void CalculateStationAdjustSlopeAndIntercept(CGUI_Dlg *pDlg)
{

	WriteDefaultDbParameters(pDlg);							// write values to the default db
	ReadDefaultDbParameters();								// read the new values that were calculated

}



///////////////////////////////////////////////////////////////////////////
//	This function retrieves all values from the dialog box and
//	writes them to the default database.
//
//	static void WriteDefaultDbParameters(CGUI_Dlg *pDlg)
//
//	Input:
//		pDlg	dialog box containing the table with the information
//
//	Output:
//		None
//	
//	External Dependencies:
//	Parameters with gll prefix are defined at the top of this file.
///////////////////////////////////////////////////////////////////////////
static void WriteDefaultDbParameters(CGUI_Dlg *pDlg)
{
	int status = 0;   
	BOOL bTimeAlignEnabled = FALSE;
	struct TimeAlignParmsStruct *pParms = NULL;
	struct IDStruct StaID;
	
	// read checkbox that indicates whether or not to use any adjustments
	// and write to default database
    pDlg->CheckBoxGetValue(IDC_ADJUST_CHECKBOX, &bTimeAlignEnabled);
	
	// read the table values and write to default database.
	CGUI_Table *pTable = NULL;
	pDlg->GetTable(IDC_ADJUST_TABLE, &pTable);

	if (pTable != NULL) 
	{	
		pTable->RetrieveAllCellValues();
		pParms = new struct TimeAlignParmsStruct[glliNumStas];

		for (int i=0; i<glliNumStas; i++) 
		{
			if (!pglFacCfg->GetStationIDForPicklistChoice(i, &StaID))
			{
				delete [] pParms;  //leak fix pjm
				return;
			}
		
			pParms[i].sStaNum = StaID.sStaID;
			pParms[i].sStaType = StaID.sStaType;

			if (StaID.sStaType == EOSS_TYPE)
			{
				if (pParms[i].bEnableStation = pgllbUseStation[i])
					GUI_MsgBox("EOSS instruments cannot be adjusted with Time Align.\nParameters for EOSS station will not be saved.", GUI_ICON_ERROR);
				pParms[i].bUseTimeAlign = false;	// whether or not to use the station
				pParms[i].bEnableStation = false;	// whether or not to use the station
				
				//unsigned long ulTemp;
				//gen_date_time_str_to_seconds(&ulTemp, gllszDate1[i], gllszTime1[i], GEN_DTF_IAEA);
				pParms[i].ulEvent1Time = 0;
				//gen_date_time_str_to_seconds(&ulTemp, gllszDate2[i], gllszTime2[i], GEN_DTF_IAEA);
				pParms[i].ulEvent2Time = 0;
				
				pParms[i].dSlope = 0.0;
				pParms[i].dIntercept = 0.0;
				pParms[i].bUseTimeAlign = FALSE;

			}
			else
			{
			
				pParms[i].bUseTimeAlign = bTimeAlignEnabled;	// whether or not to use the station
				pParms[i].bEnableStation = pgllbUseStation[i];	// whether or not to use the station
				
				unsigned long ulTemp;
				gen_date_time_str_to_seconds(&ulTemp, gllszDate1[i], gllszTime1[i], GEN_DTF_IAEA);
				pParms[i].ulEvent1Time = ulTemp;
				gen_date_time_str_to_seconds(&ulTemp, gllszDate2[i], gllszTime2[i], GEN_DTF_IAEA);
				pParms[i].ulEvent2Time = ulTemp;
				
				pParms[i].dSlope = pglldSlope[i];
				pParms[i].dIntercept = pglldIntercept[i];
				pParms[i].bUseTimeAlign = gllbUseAdjustments;
			}
		}
		
		status = pglTimeAlign->SetParameters(glsFacNum, glliNumStas, pParms);
    }
	
	delete [] pParms;
	SaveDatabaseNow();   
	return;
}

                                                      
///////////////////////////////////////////////////////////////////////////
//	This function allocates storage for all values and read the values from
//	the default database.
//
//	static BOOL ReadDefaultDbParameters()
//
//	Input:
//		None
//
//	Output:
//		None
//	
//	External Dependencies:
//	Parameters with gll prefix are defined at the top of this file.
///////////////////////////////////////////////////////////////////////////
static BOOL ReadDefaultDbParameters()
{
	int status;
	short sStaNum, sStaType;
	static BOOL bFirstTime = TRUE;
	struct TimeAlignParmsStruct *pParms = NULL;
	struct IDStruct StaID;

	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) 
		return(FALSE);	// verify facility is okay

	pglFacCfg->DestroyPicklist(gllaszStaList);

	int iLen;
	if (!pglFacCfg->CreateStationPicklist(glsFacNum, false, &gllaszStaList, &iLen)) 
		return(FALSE);

	gllsMaxNameLen = (short)iLen;
	glliNumStas = pglFacCfg->GetNumStations(glsFacNum);
	gllsMaxNameLen++;

	bool bOk = true;

	//	Allocate storage for all use station checkboxes that will be in the table
    pgllbUseStation = (BOOL *)malloc(glliNumStas*sizeof(BOOL));
    if (pgllbUseStation == NULL) bOk = false; 	

	// Allocate storage for all pointers to time strings and strings that will be in the table
	pgllDate1 = (char **)malloc(glliNumStas*sizeof(char *));			// pointers to date1 strings
    if (pgllDate1 == NULL)  bOk = false;

	gllszDate1 = (char (*)[DT_LEN+1])malloc(glliNumStas*(DT_LEN+1));	// date1 strings
    if (gllszDate1 == NULL)  bOk = false;
	
	pgllTime1 = (char **)malloc(glliNumStas*sizeof(char *));			// pointers to time1 strings
    if (pgllTime1 == NULL)  bOk = false; 
	
	gllszTime1 = (char (*)[DT_LEN+1])malloc(glliNumStas*(DT_LEN+1));	// time1 strings
    if (gllszTime1 == NULL)  bOk = false; 	

	pgllDate2 = (char **)malloc(glliNumStas*sizeof(char *));			// pointers to date2 strings
    if (pgllDate2 == NULL)  bOk = false;
	
	gllszDate2 = (char (*)[DT_LEN+1])malloc(glliNumStas*(DT_LEN+1));	// date2 strings
    if (gllszDate2 == NULL)  bOk = false;
	
	pgllTime2 = (char **)malloc(glliNumStas*sizeof(char *));			// pointers to time2 strings
    if (pgllTime2 == NULL) bOk = false;

	gllszTime2 = (char (*)[DT_LEN+1])malloc(glliNumStas*(DT_LEN+1));	// time2 strings
    if (gllszTime2 == NULL)  bOk = false; 	

	// Allocate storage for all slope and intercept information
	pglldSlope = (double *)malloc(glliNumStas*sizeof(double));
    if (pglldSlope == NULL) bOk = false;

	pglldIntercept = (double *)malloc(glliNumStas*sizeof(double));
    if (pglldIntercept == NULL) bOk = false;
	
	if (!bOk)
	{
		CleanUpAdjust();
	 	return(FALSE);
	}
	
	// get the parameters from the TimeAlignCom
	pParms = new struct TimeAlignParmsStruct[glliNumStas];

    for (int i=0; i<glliNumStas; i++) 
	{		// fill in station numbers before do request
		if (!pglFacCfg->GetStationIDForPicklistChoice(i, &StaID))
		{
			delete [] pParms;  //fixed leak pjm
			return(FALSE);
		}

		pParms[i].sStaNum = StaID.sStaID;
		pParms[i].sStaType = StaID.sStaType;
	}
	status = pglTimeAlign->GetParameters(glsFacNum, glliNumStas, pParms);
	int j;

	//	Fill in the default values for the table lists
    for (i=0; i<glliNumStas; i++) 
	{
		if (!pglFacCfg->GetStationIDForPicklistChoice(i, &StaID)) 
		{
			delete [] pParms;  //fixed leak pjm
			return(FALSE);
		}

		sStaNum = StaID.sStaID;
		sStaType = StaID.sStaType;
		
		// determine which entry this is in our parameter array
		j=0;
		while ((j<glliNumStas) && ((pParms[j].sStaNum != sStaNum) ||(pParms[j].sStaType != sStaType)))
			j++;

		// convert julian seconds and fill in the date time strings for the table
		unsigned long ulTemp = pParms[j].ulEvent1Time;
		if (ulTemp != 0) 
		{
			gen_seconds_to_date_time_str(gllszDate1[i], gllszTime1[i], &ulTemp, GEN_DTF_IAEA);
		}
		else 
		{
			strcpy(gllszDate1[i],"00.00.00");
			strcpy(gllszTime1[i],"00:00:00");
		}

		pgllDate1[i] = gllszDate1[i];
		pgllTime1[i] = gllszTime1[i];						// set up the pointer for the table

		ulTemp = pParms[j].ulEvent2Time;
		if (ulTemp != 0) 
		{
			gen_seconds_to_date_time_str(gllszDate2[i], gllszTime2[i], &ulTemp, GEN_DTF_IAEA);
		}
		else 
		{
			strcpy(gllszDate2[i],"00.00.00");
			strcpy(gllszTime2[i],"00:00:00");
		}

		pgllDate2[i] = gllszDate2[i];						// set up the pointer for the table
		pgllTime2[i] = gllszTime2[i];						// set up the pointer for the table
		
		pgllbUseStation[i] = pParms[j].bEnableStation;
		pglldSlope[i] = pParms[j].dSlope;
		pglldIntercept[i] = pParms[j].dIntercept;
		gllbUseAdjustments = pParms[j].bUseTimeAlign;
	}             
	if (bFirstTime) 
	{
		gllbUseAdjustments = FALSE;  // always set to false so must do calculation
		// JFL ths means it is always disabled the first time the content is read, ( dialog is presented?), 
		// but later, the calculation may occur after a subsequent ReadDefaultDbParameters call
		bFirstTime = FALSE;
	}
	delete [] pParms;
	return(TRUE);
}
