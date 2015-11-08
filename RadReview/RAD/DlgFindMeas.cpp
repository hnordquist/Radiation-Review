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
//
//    File Name : DlgFindMeas.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <gui_tbl.h>            
#include "RAD.H"         
#include "DateTimeUtils.h"
#include "DbDefault.h"
#include "DbEvent.h"
#include "DbGeneralDbSupport.h"
#include "DbLimit.h"
#include "DlgFindMeas.h"
#include "FindMeas.h"
#include "Interval.h"
#include "Plot.h"
#include "PrintFacilityHeader.h"
#include "RadInit.H"
#include "Utilities.h"
#include "FacilityConfig.h" //KM
#include "MyDateTime.h"

extern char *glnaszIntervalUnitsPicklist[];
extern RADInit *pRAD_Init;  
extern short glsFacNum;
extern CMyDateTime glMyDateTime;
extern char glszOldDbName[]; 

char *glSRAlgorithmsPicklist[];

static struct IDStruct gllStaID;
static short gllsFacNum;
static struct db_sta_dflt_rec  glldbStaDflt;
static int gllb_meas_make_new_status;		
static int gllb_meas_low_bkg_check;		
static int gllb_meas_high_bkg_check;
static gllbLocalParmsValid = FALSE;
static short gllsMaxNameLen;     
static CGUI_TextWin *pWin = NULL;    

// station table parameters    
static int glliNumRows;    
static unsigned int glluiCheckBoxCol;
static unsigned int gllusStaCol;                 
static BOOL *pgllbSelect = NULL;  
static char **aszStaList = NULL;

static BOOL gllbDiagPrint = FALSE;
static BOOL gllbClearEvents = FALSE;

static struct GEN_DATE_STRUCT gllGenDate;
static struct GEN_TIME_STRUCT gllGenTime;
static DATE glldUserIntervalStart = 0; 
static DATE glldEnd = 0;
static unsigned long gllulUserInterval = 0;
static unsigned int glliUserUnits = 0;                 

static void WriteAdvancedDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, struct IDStruct StaID);
static void UpdateAdvancedDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL ReadAdvancedDefaultDbParameters(struct db_sta_dflt_rec *pdb_StaDfltRec);
static void StationChoiceNotifyFunction(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void AdvancedMeasurementsDialogBox(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void WriteDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL ReadDefaultDbParameters();
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL OnlyDialogBox(BOOL bSetupOnly);
static BOOL CloseRawDataWindow(CGUI_TextWin *pWinArg);
static BOOL DoIt();



/*===========================================================================
 *
 *  Name	 :  	WriteAdvancedDefaultDbParameters, UpdateAdvancedDbParms
 *					ReadAdvancedDefaultDbParameters
 *
 *  Purpose	 :		WriteAdvancedDefaultDbParameters/UpdateAdvancedDbParms:
 *					Update/Write called from the Advanced Option dialog box
 *					in the Find Measurements Menu Option when the SaveAsDefaults
 *					button is pushed. The present values are read from the dialog
 *					box and written to the Default database.
 *
 *					ReadAdvancedDefaultDbParameters
 *					Read reads the current db values to populate the Advanced Option dialog box
 *
 *
 *  Return value :  None
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  10-Feb-1998 SFK		Created
 *	21-Dec-2005	SFK		Added support for additional minutes parameter
 *
===========================================================================*/
// write the parameters from the advanced options part of Find Measurement to the db
static void WriteAdvancedDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, struct IDStruct StaID)
{
    int status;  
    struct db_sta_dflt_rec  dbStaDflt;
	BOOL bNum;
	unsigned int uiNum;
	struct IDStruct DialogStaID;

	// update the station selected at the top of the advanced dialog box
    pDlg->PickListGetValue(IDC_MEAS_ADV_STATION, &uiNum);
	if (!pglFacCfg->GetStationIDForPicklistChoice((short)uiNum, &DialogStaID)) return;
	status = AccessDefaultDatabase(WRITE, S_MEAS_PARM_STATION, &DialogStaID.sStaID);
	if (status != TRUE) goto exit;
	status = AccessDefaultDatabase(WRITE, S_MEAS_PARM_STATION_TYPE, &DialogStaID.sStaType);
	if (status != TRUE) goto exit;

	//	the db_sta_dflt_rec contains information from many menu options
	//	read values from the db that will not change in this write
	dbStaDflt.s_sta_dflt_num_key = StaID.sStaID;
	dbStaDflt.s_sta_dflt_type = StaID.sStaType;
	status = AccessDefaultDatabase(READ, 0, &dbStaDflt); 
	if (status != TRUE) goto exit;

	// fill in the values from the advanced options dialog box
	pDlg->PickListGetValue(IDC_MEAS_ADV_ANALYSIS, &uiNum);
	dbStaDflt.s_meas_analysis_type = (short)uiNum;
    pDlg->FldNumGetValue(IDC_MEAS_ADV_LOW_BKG, &dbStaDflt.d_meas_low_bkg_thres);
	pDlg->FldNumGetValue(IDC_MEAS_ADV_HIGH_BKG, &dbStaDflt.d_meas_high_bkg_thres);
    pDlg->FldNumGetValue(IDC_MEAS_ADV_EMPTY, &dbStaDflt.d_meas_empty_thres);
	pDlg->FldNumGetValue(IDC_MEAS_ADV_REALS_ASSAY, &dbStaDflt.d_meas_reals_thres);
    pDlg->FldNumGetValue(IDC_MEAS_ADV_TOTALS_ASSAY, &dbStaDflt.d_meas_totals_thres);
	pDlg->FldNumGetValue(IDC_MEAS_ADV_CF, &dbStaDflt.d_meas_cf_thres);
    pDlg->FldNumGetValue(IDC_MEAS_ADV_CF_NORM, &dbStaDflt.d_meas_cf_norm_thres);
    pDlg->FldNumGetValue(IDC_MEAS_ADV_MIN_LEN, &dbStaDflt.d_meas_min_len);
	pDlg->FldNumGetValue(IDC_MEAS_ADV_MAX_LEN, &dbStaDflt.d_meas_max_len);
	pDlg->FldNumGetValue(IDC_MEAS_ADV_ADD_TIME, &dbStaDflt.d_meas_add_min);	// SCR00191 21-Dec-2005 SFK Added
	
    pDlg->CheckBoxGetValue(IDC_MEAS_ADV_REANALYZE, &bNum);
	dbStaDflt.b_meas_make_new_status = bNum;
    pDlg->CheckBoxGetValue(IDC_MEAS_ADV_LOW_BKG_CHECK, &bNum);
	dbStaDflt.b_meas_low_bkg_check = bNum;
    pDlg->CheckBoxGetValue(IDC_MEAS_ADV_HIGH_BKG_CHECK, &bNum);
	dbStaDflt.b_meas_high_bkg_check = bNum;

	// write the new updated station record to the db
	status = AccessDefaultDatabase(WRITE, 0, &dbStaDflt);
	if (status != TRUE) goto exit;

    SaveDatabaseNow();
	return;    

exit:
	SaveDatabaseNow();
	RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
	return;
}    


// asks question before writing Advanced Options parameters
static void UpdateAdvancedDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  
	short sNum;
	struct IDStruct StaID;
	unsigned int uiNum;
    
	status = AskUpdateQuestion();
	if (status != TRUE) return;

   /* ------------------------------------------------------------------
    *	Get the station number of the current station picklist choice.
    * ----------------------------------------------------------------*/
    pDlg->PickListGetValue(IDC_MEAS_ADV_STATION, &uiNum);
	sNum = (short)uiNum;
	if (!pglFacCfg->GetStationIDForPicklistChoice(sNum, &StaID)) return;

	WriteAdvancedDefaultDbParameters(pDlg, CtrlID, StaID);	    
    return;
}        

                                
// reads all the paramters needed for the Advanced Dialog Box for Find Measurements
// some come from the db, others are in the rad.ini file
static BOOL ReadAdvancedDefaultDbParameters(struct db_sta_dflt_rec *pdb_StaDfltRec)
{                                

	int status = TRUE;     
	
   /* ------------------------------------------------------------------
    *	Get the default parameters from the Default database.
	*	Note: the calling routine must put the station number in the
	*	s_sta_meas_num_key field.
    * ----------------------------------------------------------------*/
  	status = AccessDefaultDatabase(READ, 0, pdb_StaDfltRec);

	// some of the parameters in the record are stored in the Rad.ini file
	// overwrite whatever you got from the database with the more correct
	// values in the rad.ini file.
	float fTemp;
	fTemp = pRAD_Init->Get_Meas_AT_Threshold();
	pdb_StaDfltRec->d_meas_at_thres = fTemp;

	fTemp = pRAD_Init->Get_Meas_AT_Bias();
	pdb_StaDfltRec->d_meas_at_bias = fTemp;
	
	fTemp = pRAD_Init->Get_Meas_AT_Sigma();
	pdb_StaDfltRec->d_meas_at_sigma = fTemp;

	fTemp = pRAD_Init->Get_Meas_GateWidth();
	pdb_StaDfltRec->d_meas_gate_width = fTemp;

	pdb_StaDfltRec->b_meas_term_on_moving = pRAD_Init->Get_MeasTerminateOnMoving();
	pdb_StaDfltRec->b_meas_include_at_fail= pRAD_Init->Get_MeasInclude_AT_Fail();

	if (!status) {
		RadReviewMsg(status, "when reading Measurement Parameters from Default database.  Cannot proceed.");			
		return(FALSE);
	}
	return(TRUE);
} 

// if the parameters for the specified station are now the current
// parameters in glldbStaDflt, return them.  Else read the specified
// parameters from the db.
BOOL GetSRMeasurementParameters(struct db_sta_dflt_rec *pdbStaDflt)
{
	if (gllbLocalParmsValid == TRUE) {
		if (pdbStaDflt->s_sta_dflt_num_key == glldbStaDflt.s_sta_dflt_num_key) {
			*pdbStaDflt = glldbStaDflt;
			return(TRUE);
		}
	}
	return(ReadAdvancedDefaultDbParameters(pdbStaDflt));
}

// action taken when they switch to a different station on the advanced dialog box
static void StationChoiceNotifyFunction(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{	
	struct IDStruct StaID, TempID;

   /* ------------------------------------------------------------------
    *	Determine the current station chosen
    * ----------------------------------------------------------------*/
	unsigned int iTemp;
	pDlg->PickListGetValue(IDC_MEAS_ADV_STATION, &iTemp);
	if (!pglFacCfg->GetStationIDForPicklistChoice((short)iTemp, &StaID)) return;

	if (StaID.sStaID != glldbStaDflt.s_sta_dflt_num_key) { // different station selected, update all the fields
		// write out the current parameters in case something has changed.
		TempID.sStaID = glldbStaDflt.s_sta_dflt_num_key;
		TempID.sStaType = glldbStaDflt.s_sta_dflt_type;
		WriteAdvancedDefaultDbParameters(pDlg, CtrlID, TempID);	// write out the current parameters if something has changed.

		// now read in the information for the station that was just selected
		// and fill those values into the Advanced Dialog box
		glldbStaDflt.s_sta_dflt_num_key = StaID.sStaID;
		glldbStaDflt.s_sta_dflt_type = StaID.sStaType;
		ReadAdvancedDefaultDbParameters(&glldbStaDflt); // get the new values from the DB
		gllb_meas_make_new_status = glldbStaDflt.b_meas_make_new_status;
		gllb_meas_low_bkg_check = glldbStaDflt.b_meas_low_bkg_check;
		gllb_meas_high_bkg_check = glldbStaDflt.b_meas_high_bkg_check;

		gllbLocalParmsValid = TRUE;

		pDlg->PickListSetValue(IDC_MEAS_ADV_ANALYSIS, glldbStaDflt.s_meas_analysis_type);

	   /* ------------------------------------------------------------------
	    *	Set the edit boxes and their values
	    * ----------------------------------------------------------------*/
		pDlg->FldNumSetValue(IDC_MEAS_ADV_LOW_BKG, glldbStaDflt.d_meas_low_bkg_thres);
		pDlg->FldNumSetValue(IDC_MEAS_ADV_HIGH_BKG, glldbStaDflt.d_meas_high_bkg_thres);
		pDlg->FldNumSetValue(IDC_MEAS_ADV_EMPTY, glldbStaDflt.d_meas_empty_thres);
		pDlg->FldNumSetValue(IDC_MEAS_ADV_REALS_ASSAY, glldbStaDflt.d_meas_reals_thres);
		pDlg->FldNumSetValue(IDC_MEAS_ADV_TOTALS_ASSAY, glldbStaDflt.d_meas_totals_thres);
		pDlg->FldNumSetValue(IDC_MEAS_ADV_CF, glldbStaDflt.d_meas_cf_thres);
		pDlg->FldNumSetValue(IDC_MEAS_ADV_CF_NORM, glldbStaDflt.d_meas_cf_norm_thres);
		pDlg->FldNumSetValue(IDC_MEAS_ADV_MIN_LEN, glldbStaDflt.d_meas_min_len);
		pDlg->FldNumSetValue(IDC_MEAS_ADV_MAX_LEN, glldbStaDflt.d_meas_max_len);
		pDlg->FldNumSetValue(IDC_MEAS_ADV_ADD_TIME, glldbStaDflt.d_meas_add_min);		// SCR00191 21-Dec-2005 SFK Added
		
		pDlg->CheckBoxSetValue(IDC_MEAS_ADV_REANALYZE, gllb_meas_make_new_status);		
		pDlg->CheckBoxSetValue(IDC_MEAS_ADV_LOW_BKG_CHECK, gllb_meas_low_bkg_check);		
		pDlg->CheckBoxSetValue(IDC_MEAS_ADV_HIGH_BKG_CHECK, gllb_meas_high_bkg_check);		
	}
}
	


/*===========================================================================
 *
 *  Name	 :  	AdvancedMeasurementsDialogBox
 *
 *  Purpose	 :		Update/Write called from the dialog box when the SaveAsDefaults button is pushed.
 *					The present values are read from the dialog box and written to
 *					the Default database.
 *					Read reads the current db values for the specified station.
 *
 *
 *  Return value :  None
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  10-Feb-1998 SFK		Created
 *	21-Dec-2005 SFK		Added support for additional minutes parameter
 *
===========================================================================*/
static void AdvancedMeasurementsDialogBox(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{                                 
	static BOOL bFirst = TRUE;
//	static char szOldDbName[DRIVE_LEN+PATH_LEN+1] ={""};
	
    GUI_ACTION DlgReturn;
	unsigned int uiStaChoice;
	char **aszStaList = NULL; 

	// verify the facility is defined
	char szFacName[DB_NAME_LEN+1];
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) return;
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return;

	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);


	if ((bFirst == TRUE) || (!SameDb(glszOldDbName)) || (gllsFacNum != glsFacNum)) {
		// Figure out which station is the default one to display on the Advanced Options page
		if ((!AccessDefaultDatabase(READ, S_MEAS_PARM_STATION, &gllStaID.sStaID)) ||
			(!AccessDefaultDatabase(READ, S_MEAS_PARM_STATION_TYPE, &gllStaID.sStaType))) {
			RadReviewMsg(uiDFLT_DB_READ_ERR, "Tools | Find Measurements | Advanced Dialog Box.  Cannot proceed with menu option.");			
			return;
		}

		// make sure you get a valid station from the db
		if (!pglFacCfg->IsStationEnabled(glsFacNum, &gllStaID)) {
			if (pglFacCfg->GetNumStations(glsFacNum) > 0) {
				pglFacCfg->GetFirstStationID(glsFacNum, &gllStaID);
			}
			else {
  				RadReviewMsg(uiNO_STATIONS_FOR_FACILITY_ERR);
				return;
			}
		}

		pglFacCfg->GetStationID(glsFacNum, &gllStaID);		// fill in the station id struct

		//	Now get the rest of the parameters to go with that station to display on the advanced options page
		glldbStaDflt.s_sta_dflt_num_key = gllStaID.sStaID;
		glldbStaDflt.s_sta_dflt_type = gllStaID.sStaType;
		ReadAdvancedDefaultDbParameters(&glldbStaDflt);
		gllb_meas_make_new_status = glldbStaDflt.b_meas_make_new_status;
		gllb_meas_low_bkg_check = glldbStaDflt.b_meas_low_bkg_check;
		gllb_meas_high_bkg_check = glldbStaDflt.b_meas_high_bkg_check;

		gllbLocalParmsValid = TRUE;

		// remember the current conditions for future entries to this option
		gllsFacNum = glsFacNum;
	    GetDbName(glszOldDbName);								
		bFirst = FALSE;
	}
	
   /* ------------------------------------------------------------------
    *	Define the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg Dlg(IDD_TOOLS_MEASUREMENTS_ADVANCED, NULL, DLG_POSN_NO_SAVE);
    if (Dlg.IsValid()) {
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		Dlg.DefinePushBtn(IDOK, GUI_OK); 
		Dlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateAdvancedDbParms);  		  
		
	   /* ------------------------------------------------------------------
	    *	Set facility label in dialog box
	    * ----------------------------------------------------------------*/
	    Dlg.DefineFldStaticTxt(IDC_MEAS_ADV_FACILITY);
		Dlg.FldStaticTxtSetValue(IDC_MEAS_ADV_FACILITY, szFacName);

	   /* ------------------------------------------------------------------
	    *	Set the picklists 
	    * ----------------------------------------------------------------*/
		int iDum;
		short sPicklistChoice;
		if (!pglFacCfg->CreateStationPicklist(glsFacNum, false, &aszStaList, &iDum)) return;
		if (!pglFacCfg->GetPicklistChoiceForStationID(gllStaID, &sPicklistChoice))return;
		uiStaChoice = sPicklistChoice;
		Dlg.DefinePickList(IDC_MEAS_ADV_STATION, aszStaList, &uiStaChoice, NULL, StationChoiceNotifyFunction);
		Dlg.DefinePickList(IDC_MEAS_ADV_ANALYSIS, glSRAlgorithmsPicklist, (unsigned int *)&glldbStaDflt.s_meas_analysis_type);
		Dlg.PickListSetValue(IDC_MEAS_ADV_ANALYSIS, glldbStaDflt.s_meas_analysis_type);

	   /* ------------------------------------------------------------------
	    *	Set the edit boxes and their values
	    * ----------------------------------------------------------------*/
		// 21-Apr-2005 SFK Change upper limit for reals and totals from 100,000 to 1,000,000
		Dlg.DefineFldNum(IDC_MEAS_ADV_LOW_BKG, &glldbStaDflt.d_meas_low_bkg_thres, 0.0, 10000.0,"%f");
		Dlg.DefineFldNum(IDC_MEAS_ADV_HIGH_BKG, &glldbStaDflt.d_meas_high_bkg_thres, 0.0, 10000.0,"%f");
		Dlg.DefineFldNum(IDC_MEAS_ADV_EMPTY, &glldbStaDflt.d_meas_empty_thres, 0.0, 10000.0,"%f");
		Dlg.DefineFldNum(IDC_MEAS_ADV_REALS_ASSAY, &glldbStaDflt.d_meas_reals_thres, 0.0, 100000000.0,"%f");
		Dlg.DefineFldNum(IDC_MEAS_ADV_TOTALS_ASSAY, &glldbStaDflt.d_meas_totals_thres, 0.0, 100000000.0,"%f");
		Dlg.DefineFldNum(IDC_MEAS_ADV_CF, &glldbStaDflt.d_meas_cf_thres, 0.0, 10000.0,"%f");
		Dlg.DefineFldNum(IDC_MEAS_ADV_CF_NORM, &glldbStaDflt.d_meas_cf_norm_thres, 0.0, 10000.0,"%f");
		Dlg.DefineFldNum(IDC_MEAS_ADV_MIN_LEN, &glldbStaDflt.d_meas_min_len, 0.0, 100.0,"%3.0f");
		Dlg.DefineFldNum(IDC_MEAS_ADV_MAX_LEN, &glldbStaDflt.d_meas_max_len, 0.0, 100.0,"%3.0f");
		Dlg.DefineFldNum(IDC_MEAS_ADV_ADD_TIME, &glldbStaDflt.d_meas_add_min, 0.0, 999.0,"%3.0f");		// SCR00191 21-Dec-2005 SFK Added
		
		Dlg.DefineCheckBox(IDC_MEAS_ADV_REANALYZE, &gllb_meas_make_new_status);		
		Dlg.DefineCheckBox(IDC_MEAS_ADV_LOW_BKG_CHECK, &gllb_meas_low_bkg_check);		
		Dlg.DefineCheckBox(IDC_MEAS_ADV_HIGH_BKG_CHECK, &gllb_meas_high_bkg_check);		
		
	   /* ------------------------------------------------------------------
	    *	Set up the focus and display the dialog box
	    * ----------------------------------------------------------------*/
		Dlg.SetFocus(IDC_MEAS_ADV_STATION);   
		DlgReturn = Dlg.Go();      
	    if (DlgReturn == GUI_OK) {  
	    	Dlg.RetrieveAllControlValues();
			glldbStaDflt.b_meas_make_new_status = gllb_meas_make_new_status;
			glldbStaDflt.b_meas_low_bkg_check = gllb_meas_low_bkg_check;
			glldbStaDflt.b_meas_high_bkg_check = gllb_meas_high_bkg_check;
			// add code to support a current station set here ????????????????
			if (!pglFacCfg->GetStationIDForPicklistChoice(uiStaChoice, &gllStaID)) return;
			glldbStaDflt.s_sta_dflt_num_key = gllStaID.sStaID;
		}
		pglFacCfg->DestroyPicklist(aszStaList);
		aszStaList = NULL;
	}
	return;
}             


                
/*===========================================================================
 *
 *  Name	 :  	ReadDefaultDbParameters
 *					WriteDefaultDbParameters
 *
 *  Purpose	 :		Read reads the current db values for the specified station
 *					that are needed for the measurement analysis.
 *					Write writes the parameters that are currently set when the
 *					SaveAsDefaults button is clicked.
 *
 *
 *  Return value :  None
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  10-Feb-1998 SFK		Created
 *
===========================================================================*/
// writes parameters associated with the first Dialog Box in Find Measurements
static void WriteDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	BOOL bBool;
	int status;   
	struct db_sta_dflt_rec dbStaDflt;
	struct IDStruct StaID;

   /* ------------------------------------------------------------------
    *	Get the current parameters set in the dialog box and write
    *	to default database.
    * ----------------------------------------------------------------*/ 
    pDlg->CheckBoxGetValue(IDC_MEAS_SRCH_CLEAR, &bBool);
	status = AccessDefaultDatabase(WRITE, B_MEAS_SRCH_CLEAR, &bBool);
	if (status != TRUE) goto exit;
    
   /* ------------------------------------------------------------------
    *	Read the current table entry values and update the default
    *	parms in the db.
    * ----------------------------------------------------------------*/
	CGUI_Table *pTable;
    int i;
	pDlg->GetTable(IDC_MEAS_PICKTABLE, &pTable);
	if (pTable != NULL) {	
		pTable->RetrieveAllCellValues();
	    for (i=0; i<glliNumRows; i++) {
			if (!pglFacCfg->GetStationIDForPicklistChoice(i, &StaID)) return;
			dbStaDflt.s_sta_dflt_num_key = StaID.sStaID;	// going to change one field so read the other fields
			dbStaDflt.s_sta_dflt_type = StaID.sStaType;
			status = AccessDefaultDatabase(READ, 0, &dbStaDflt); 
			if (status != TRUE) goto exit;
			dbStaDflt.b_meas_srch_select = pgllbSelect[i]; // update the field we know about here
			status = AccessDefaultDatabase(WRITE, 0, &dbStaDflt); 
			if (status != TRUE) goto exit;
	    }	
    }
	SaveDatabaseNow();   
	return;
	                                                               
exit:
	RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
	return;
}

// Asks question before writing parameters for the first dialog box in Find Measurements
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  

	status = AskUpdateQuestion();
	if (status != TRUE) return;
	WriteDefaultDbParameters(pDlg, CtrlID);
	
}         

// read the parameters needed for the first dialog box in Find Measurements
static BOOL ReadDefaultDbParameters()
{                                

	int status = TRUE;     
	short sNumStas;
	int i;
	struct db_sta_dflt_rec dbStaDflt;
	struct IDStruct StaID;

	status = AccessDefaultDatabase(READ, B_MEAS_SRCH_CLEAR, &gllbClearEvents);

	gllsFacNum = glsFacNum;	// 25-May-2005 Fix that gllsFacNum gets set for when running in /IMPORT mode

	// Create the station list and allocate storage for the table selection boxes and names here.
	sNumStas = pglFacCfg->GetNumStations(glsFacNum);
	if (sNumStas) {  //KM - If we have stations present (i.e. not zero stations)  
		glliNumRows = sNumStas;
		int iTemp;
		if (!pglFacCfg->CreateStationPicklist(glsFacNum, false, &aszStaList, &iTemp)) return(FALSE);
		gllsMaxNameLen = (short)iTemp;
		if (pgllbSelect) free(pgllbSelect);    
	    pgllbSelect = (BOOL *)malloc(glliNumRows*sizeof(BOOL));
		if (pgllbSelect == NULL) {
			pglFacCfg->DestroyPicklist(aszStaList);
			aszStaList = NULL;
	 		return(FALSE);
		}
	} 	

   /* ------------------------------------------------------------------
    *	Fill in the default values for the table lists
    * ----------------------------------------------------------------*/
	for (i=0; i<glliNumRows; i++) {
		if (!pglFacCfg->GetStationIDForPicklistChoice(i, &StaID)) return(FALSE);
		dbStaDflt.s_sta_dflt_num_key = StaID.sStaID;
		dbStaDflt.s_sta_dflt_type = StaID.sStaID;
		status = AccessDefaultDatabase(READ, 0, &dbStaDflt); 
		if (status != TRUE) {
			CleanUpDetermineMeasurements();
		 	return(FALSE);
		} 	
		pgllbSelect[i] = dbStaDflt.b_meas_srch_select;
		if (pgllbSelect[i] > 1) {
			// GUI_MsgBox("Invalid value for select read, set to TRUE", GUI_ICON_INFO);
			pgllbSelect[i] = TRUE;
		}
	}             
	return(TRUE);
} 


/*===========================================================================
 *
 *  Name	 :  	SelectAll, DeselectAll
 *
 *  Purpose	 :		Either check or uncheck all stations in the dialog box
 *
 *
 *  Return value :  None
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  10-Feb-1998 SFK		Created
 *
===========================================================================*/
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable;
    int i;
    
	pDlg->GetTable(IDC_MEAS_PICKTABLE, &pTable);
	if (pTable != NULL) {
	    for (i=0; i<glliNumRows; i++) {
	    	pgllbSelect[i] = TRUE;
	    }	
        pTable->UpdateCol(glluiCheckBoxCol);
    }
}        


static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable;
	int i;
		
	pDlg->GetTable(IDC_MEAS_PICKTABLE, &pTable);
	if (pTable != NULL) {
	    for (i=0; i<glliNumRows; i++) {
	    	pgllbSelect[i] = FALSE;
	    }	
        pTable->UpdateCol(glluiCheckBoxCol);
    }
}        



/*===========================================================================
 *
 *  Name	 :  	CleanUpDetermineMeasurements
 *
 *  Purpose	 :		Deallocate storage used.
 ???????????????????Do I need this???????????????????????????///
 *
 *
 *  Return value :  None
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  10-Feb-1998 SFK		Created
 *
===========================================================================*/
void CleanUpDetermineMeasurements()
{   		                                       
	if (pgllbSelect) free(pgllbSelect);    
	pgllbSelect = NULL;
	pglFacCfg->DestroyPicklist(aszStaList);
	aszStaList = NULL;
}



// displays the first dialog box in the Find Measurements menu option    
static BOOL OnlyDialogBox(BOOL bSetupOnly)
{                                 
	static BOOL bFirst = TRUE;
//	static char szOldDbName[DRIVE_LEN+PATH_LEN+1] ={""};
	static DATE dLastSearchIntervalInDays = 0;
	
    GUI_ACTION DlgReturn;                                     
    CGUI_Table *pTbl;    
	CGFmtStr szInterval; 
	
	int status = TRUE;     
	unsigned long ulDbIntervalInDays;
	DATE dSearchIntervalInDays;
	char szFirstDate[DT_LEN+1];
	char szLastDate[DT_LEN+1];              
	short sNumStas;
	int i;

	// verify the facility is defined
	char szFacName[DB_NAME_LEN+1];
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) return(FALSE);
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return(FALSE);

	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);

	/* ------------------------------------------------------------------
    *	Always find the first and last days in the database and set
	*	up the strings for display in the dialog box.
    * ----------------------------------------------------------------*/
	SetCurrentDrive();
	status = GetDatabaseIntervalParameters(szFirstDate, szLastDate, &ulDbIntervalInDays, &dSearchIntervalInDays);
	if (status != TRUE) return(FALSE);
	if (dSearchIntervalInDays <= 0.0) {
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return(FALSE);
	}
		    
	if ((bFirst == TRUE) || (!SameDb(glszOldDbName)) || (dSearchIntervalInDays != dLastSearchIntervalInDays) || (gllsFacNum != glsFacNum)) {
		ReadDefaultDbParameters();		// get stuff to populate this first dialog box
		GetReviewPeriodParameters(&glldUserIntervalStart, &glldEnd, &gllulUserInterval);
		glliUserUnits = 0;

		// remember the current conditions for future entries to this option
		gllsFacNum = glsFacNum;
	    GetDbName(glszOldDbName);								
		dLastSearchIntervalInDays = dSearchIntervalInDays;	
		bFirst = FALSE;
	}	

   /* ------------------------------------------------------------------
    *	Define the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg Dlg(IDD_TOOLS_MEASUREMENTS, NULL, DLG_POSN_NO_SAVE);
    if (Dlg.IsValid()) {
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		Dlg.DefinePushBtn(IDOK, GUI_OK);                   
		Dlg.DefinePushBtn(IDC_MEAS_ALL_BUTTON, GUI_NONE, SelectAll);          
		Dlg.DefinePushBtn(IDC_MEAS_NONE_BUTTON, GUI_NONE, DeselectAll); 
		Dlg.DefinePushBtn(IDC_MEAS_ADVANCED_BUTTON, GUI_NONE, AdvancedMeasurementsDialogBox);
		Dlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateDbParms);  		  
		
	   /* ------------------------------------------------------------------
	    *	Set facility label in dialog box
	    * ----------------------------------------------------------------*/
	    Dlg.DefineFldStaticTxt(IDC_MEAS_FACILITY);
		Dlg.FldStaticTxtSetValue(IDC_MEAS_FACILITY, szFacName);
			
	   /* ------------------------------------------------------------------
	    *	Display current starting ending database dates and interval in dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefineFldStaticTxt(IDC_MEAS_CURR_DB_START);
		Dlg.FldStaticTxtSetValue(IDC_MEAS_CURR_DB_START, szFirstDate);
					
		Dlg.DefineFldStaticTxt(IDC_MEAS_CURR_DB_END);
		Dlg.FldStaticTxtSetValue(IDC_MEAS_CURR_DB_END, szLastDate);
	
		szInterval.Printf("%ld days", ulDbIntervalInDays);  			
		Dlg.DefineFldStaticTxt(IDC_MEAS_CURR_DB_INTERVAL);
		Dlg.FldStaticTxtSetValue(IDC_MEAS_CURR_DB_INTERVAL, szInterval);
		
	   /* ------------------------------------------------------------------
	    *	Set up the search interval parameters in dialog box
	    * ----------------------------------------------------------------*/
		//Date.DATETimestampToGenStructs(glldUserIntervalStart, &gllGenDate, &gllGenTime);
		glMyDateTime.DATETimestampToGenStructs(glldUserIntervalStart, &gllGenDate, &gllGenTime);
		Dlg.DefineFldDate(IDC_MEAS_SRCH_STRTDATE, &gllGenDate);
		Dlg.DefineFldTime(IDC_MEAS_SRCH_STRTTIME, &gllGenTime);
	
		Dlg.DefineFldNum(IDC_MEAS_SRCH_INTERVAL, &gllulUserInterval, 1, 10000);
		Dlg.DefinePickList(IDC_MEAS_INTRVL_UNITS_PL, glnaszIntervalUnitsPicklist, &glliUserUnits);
	    
	   /* ------------------------------------------------------------------
	    *	Set up the station choices in the table
	    * ----------------------------------------------------------------*/

		Dlg.DefineTable(IDC_MEAS_PICKTABLE, &pTbl);	
		if (pTbl != NULL) {
			glluiCheckBoxCol = pTbl->AddColCheckBox("Select", glliNumRows, pgllbSelect);		
			gllusStaCol = pTbl->AddColTxt("Station", glliNumRows, aszStaList, gllsMaxNameLen+1);
			pTbl->SetColWriteEnable(gllusStaCol, FALSE);
		}	

	   /* ------------------------------------------------------------------
	    *	Set up the diagnostic print
	    * ----------------------------------------------------------------*/
		Dlg.DefineCheckBox(IDC_MEAS_SRCH_CLEAR, &gllbClearEvents);
		Dlg.DefineCheckBox(IDC_MEAS_DIAG_PRINT, &gllbDiagPrint);
			
	   /* ------------------------------------------------------------------
	    *	Set up the focus and display the dialog box
	    * ----------------------------------------------------------------*/
		Dlg.SetFocus(IDC_MEAS_SRCH_INTERVAL);
		if (bSetupOnly) Dlg.SetFocus(IDOK);
		DlgReturn = Dlg.Go();      
				    
	    if (DlgReturn == GUI_OK) {  
	    	Dlg.RetrieveAllControlValues();
	    	if (pTbl != NULL) pTbl->RetrieveAllCellValues();          /* get table values */

		   /* ------------------------------------------------------------------
		    *	Check that they selected at least 1 station to search for measurements
		    * ----------------------------------------------------------------*/
		    sNumStas = 0;
		    i = 0;
			while ((sNumStas == 0) && (i<glliNumRows)) {
				if (pgllbSelect[i] == TRUE) sNumStas++;
				i++;
			}		
		    if (sNumStas == 0) {
		    	RadReviewMsg(uiNO_STAS_SELECTED_WARN);
		    	return(FALSE);
		    }	

			// in setup mode, record parameters and quit
			if (bSetupOnly) {
				WriteDefaultDbParameters(&Dlg, IDD_TOOLS_MEASUREMENTS);
			}	
		   	// check to see if there is any data in the interval for any station
			status = ConvertStructsToSearchIntervalParameters(&gllGenDate, &gllGenTime, glliUserUnits, gllulUserInterval, &glldUserIntervalStart, &glldEnd);
			if (status != TRUE) {	// no data for any station in the interval
				RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, glldUserIntervalStart, glldEnd, "any station");
				return(FALSE);
			}
			return(TRUE);
	    }	
	}
   	return(FALSE);
}             


static BOOL CloseRawDataWindow(CGUI_TextWin *pWinArg)
{
	BOOL bCanClose = TRUE;
	pWin = NULL;
	return(bCanClose);
}               

// 13-Aug-2002 changed to DATE       
// Does the action associated with the first dialog box in the Find Measurements menu option
static BOOL DoIt()
{ 
	int i;
	int status;
	DATE dFirstDbDay, dLastDbDay;
	struct IDStruct StaID;

	SetCurrentDrive();
	// check to see if there is any data in the interval
	StaID.sStaID = ANY_STATION;
	StaID.sStaType = ANY_TYPE;
	GetDbTimeRange(StaID, &dFirstDbDay, &dLastDbDay);
	if ((glldUserIntervalStart <= dLastDbDay) && (glldEnd >= dFirstDbDay)) {
	}
	else {
		RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, glldUserIntervalStart, glldEnd, "any station");
		return(FALSE);
	}

   /* ------------------------------------------------------------------
	*	Clear measurements in database if requested
	* ----------------------------------------------------------------*/
	if (gllbClearEvents == TRUE) {
		if ((DeleteDataByChannelEvents(gllsFacNum, MEASUREMENT_EVENT)) != TRUE) return(FALSE);
		//	10-Dec-2004  Removed clearing background events automatically -- only do this using graph
		//if ((DeleteDataByChannelEvents(gllsFacNum, BACKGROUND_EVENT)) != TRUE) return(FALSE);
	}	

   /* ------------------------------------------------------------------
    *	Open up the text window where the events will be displayed
    * ----------------------------------------------------------------*/
	if (pWin == NULL) {                
		pWin = new CGUI_TextWin("Measurements Found");
		if (pWin != NULL) {
			pWin->ShowStatusBar(TRUE);
		}	
	}   
	else {
		pWin->Clear();
	}
	if (pWin != NULL) pWin->SetFocus();

	// print the facility specific header

	BOOL bFoundSome = FALSE;
	for (i=0; i<glliNumRows; i++) {
		if (pgllbSelect[i] == TRUE) {
			if (!pglFacCfg->GetStationIDForPicklistChoice(i, &StaID)) return(FALSE);
			status = FindMeasurements(StaID, glldUserIntervalStart, glldEnd, gllbDiagPrint, pWin);
		}
		if (status == TRUE) bFoundSome = TRUE;
		if (status == CANCELED) break;
	}
	if (bFoundSome == FALSE) if (pWin != NULL) pWin->Printf("\nNo measurements found in the requested interval.");

	SaveDatabaseNow();	// flush all the buffers
	UpdateGraphData(FALSE); // if the graph is displayed, update its data now.

   /* ------------------------------------------------------------------
	*	Results are now written, user can close anytime
	* ----------------------------------------------------------------*/
	if (pWin != NULL) pWin->EnableUserClose(CloseRawDataWindow);	

	return(TRUE);

}
	    
                                 
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Perform both the dialog box and the event find using parameters in the default db
///////////////////////////////////////////////////////////////////////////////////////////////////
void DetermineMeasurementsMenuOption()  // called from main menu
{      
	if (OnlyDialogBox(FALSE))
		DoIt();
}	


    
    
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Perform just the measurement find using parameters in the default db and the ini file
//	Called when started with /IMPORT
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL DetermineMeasurementsDoIt()  //called from auto import
{
	BOOL bStat;

	ReadDefaultDbParameters();
	GetReviewPeriodParameters(&glldUserIntervalStart, &glldEnd, &gllulUserInterval);
	glliUserUnits = 0;

	bStat = DoIt();
	return(bStat);
}	


///////////////////////////////////////////////////////////////////////////////////////////////
//	Perform only the dialog box setup 
//	Called when started with /SETUP
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DetermineMeasurementsDialogBox()
{	
	ReadDefaultDbParameters();
	GetReviewPeriodParameters(&glldUserIntervalStart, &glldEnd, &gllulUserInterval);
	glliUserUnits = 0;
	return(OnlyDialogBox(TRUE));
}

