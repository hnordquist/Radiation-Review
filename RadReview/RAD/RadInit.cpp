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
//    File Name : RadInit.cpp     
//Primary Author: Shirley Klosterbuer/ Steve Buck

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////


// 27-Mar-2001	SFK	Version 2.07 Added option to enable/disable Tools|Data Integrity menu option


#include "RAD.H"
#include "DbGeneralDbSupport.h"
#include "DlgDirection.h"
#include "DlgEventSummary.h"
#include "DlgExport.h"
#include "DlgFindEvent.h"
#include "DlgFindMeas.h"
#include "RadError.H" 
#include "RadInit.H" 



static fstream *pMyself = NULL;    
#include <direct.h>  	



RADInit::RADInit(const char *pszInitialization_Name, RESOURCE_ID Icon, BOOL bWrite_Access) :
	RTInit(pszInitialization_Name, Icon, bWrite_Access),
	pParams_Default(NULL),
	pParams_Current(NULL),
	bParams_Loaded(FALSE),
	bSave_Ability(bWrite_Access)
{
	
	if (((pParams_Default = new RAD_PARAMS) != NULL) &&	// create the Default object
		((pParams_Current = new RAD_PARAMS) != NULL)) {	// create the Current object
//		pParams_Default->pszNamesCfg = NULL;				
//		pParams_Default->pszRADExePath = NULL;
		pParams_Default->pszNCC_DataPath = NULL;
		pParams_Default->pszImportAllPath = NULL;
		pParams_Default->pszImportAllWildcard = NULL;		
		pParams_Default->pszImportSelectPath = NULL;
		pParams_Default->pszImportSelectWildcard = NULL;		
		pParams_Default->pszImportTodayPath = NULL;				
		pParams_Default->pszImportTodayWildcard = NULL;
		pParams_Default->pszDbBackupPath = NULL;
		pParams_Default->pszDbRestoreFromPath = NULL;
		pParams_Default->pszCfgBackupPath = NULL;
		pParams_Default->pszCfgRestoreFromPath = NULL;
		pParams_Default->pszLogCopyToPath = NULL;

//		pParams_Current->pszNamesCfg = NULL;				
//		pParams_Current->pszRADExePath = NULL;
		pParams_Current->pszNCC_DataPath = NULL;
		pParams_Current->pszImportAllPath = NULL;
		pParams_Current->pszImportAllWildcard = NULL;		
		pParams_Current->pszImportSelectPath = NULL;
		pParams_Current->pszImportSelectWildcard = NULL;		
		pParams_Current->pszImportTodayPath = NULL;				
		pParams_Current->pszImportTodayWildcard = NULL;
		pParams_Current->pszDbBackupPath = NULL;
		pParams_Current->pszDbRestoreFromPath = NULL;
		pParams_Current->pszCfgBackupPath = NULL;
		pParams_Current->pszCfgRestoreFromPath = NULL;
		pParams_Current->pszLogCopyToPath = NULL;

    
	}
}


RADInit::~RADInit(void) {
    
	CloseDatabase();
	CleanUpFindEvents();
	CleanUpEventSummary(); 
	CleanUpDirectionAnalysis();
	CleanUpDetermineMeasurements();
	CleanUpExportToINCC();


    if (pParams_Default) {
		RADInit_Delete_Params(pParams_Default);
		delete pParams_Default;
	}
	if (pParams_Current) {
		RADInit_Delete_Params(pParams_Current);
		delete pParams_Current;
	}
	
	delete pMyself;
}





/////////////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////////// 
// Member Function to switch to a new initialization file
/////////////////////////////////////////////////////////////////////////////////////////// 

BOOL RADInit::Set_New_Initialization_File(const char *pszInitialization_Name, BOOL bWrite_Access) {


	bParams_Loaded = FALSE;                                                                                                   
	if (RTInit::Set_New_Initialization_File(pszInitialization_Name, bWrite_Access))
		RADInit::Load_Parameters();

	if (!bParams_Loaded) RTError.Error(uiRT_ERROR_INIT_LOAD, Label_Error);
	return bParams_Loaded;
}

// SFK  Added 08-27-2004
void RADInit::Initialization_File_Close() {

	RTInit::Initialization_File_Close_Now();

}
 

BOOL RADInit::Load_Parameters(void) {

	bParams_Loaded = FALSE;


	// Check that the parameter structures have been created successfully
	if ((pParams_Current) && (pParams_Default) &&
		// Load the RTInit parameters first
		(RTInit::Load_Parameters()) &&
		(RTInit::Parameters_Loaded()) &&
		(RADInit_Load_Params(pParams_Default)) &&
		(RADInit_Load_Params(pParams_Current))) {
		bParams_Loaded = TRUE;
		RTError.Set_My_Name(Get_My_Name());
		}


	return bParams_Loaded;

}


/////////////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////////// 
// Member Function to Load Data from the .ini files to the RAD_PARAMS structure
/////////////////////////////////////////////////////////////////////////////////////////// 

BOOL RADInit::RADInit_Load_Params(RAD_PARAMS *pParams) {

	// Delete params if already there
	RADInit_Delete_Params(pParams);
	
	// Set everything to an initial value
	pParams->bImport_Selected_Menu_Opt = FALSE;	
	pParams->bImport_Today_Menu_Opt = FALSE;	
	pParams->bDetermine_Measurements_Menu_Opt = FALSE;	
	pParams->bDirection_Analysis_Menu_Opt = FALSE;	
	pParams->bExport_to_INCC_Menu_Opt = FALSE;	
	pParams->bMeasurements_Summary_Menu_Opt = FALSE;	
	pParams->bDirection_Summary_Menu_Opt = FALSE;
	pParams->bData_Integrity_Menu_Opt = FALSE;
	
	pParams->bChanEventResults = FALSE;	
	pParams->bDirEventResults = FALSE;	
	pParams->uiMeasurementResults = 0;	//	1/3/2005 SFK changed to default all to not write

	//pParams->pszNamesCfg = NULL;				
	//pParams->pszRADExePath = NULL;

	pParams->bAutomateQuestion = FALSE;		
	pParams->bAutomateImportAll = FALSE;
	pParams->bAutomateImportToday = FALSE;
	pParams->bAutomateIntegrity = FALSE;
	pParams->bAutomateFindEvents = FALSE;
	pParams->bAutomateDirection = FALSE;		
	pParams->bAutomateGraph = FALSE;
	pParams->bAutomateMeasurement = FALSE;
	pParams->bAutomateExport = FALSE;

	pParams->pszNCC_DataPath = NULL;

	pParams->pszImportAllPath = NULL;
	pParams->pszImportAllWildcard = NULL;		
	pParams->bImportAllOverwrite = FALSE;		
	pParams->bImportAllInit = FALSE;				
	pParams->bImportAllSubfolders = TRUE;
	
	pParams->pszImportSelectPath = NULL;
	pParams->pszImportSelectWildcard = NULL;		
	pParams->bImportSelectOverwrite = FALSE;		
	pParams->bImportSelectInit = FALSE;	
	pParams->bImportSelectSubfolders = TRUE;
	
	pParams->pszImportTodayPath = NULL;				
	pParams->pszImportTodayWildcard = NULL;
	pParams->bImportTodayOverwrite = FALSE;
	pParams->bImportTodayInit = FALSE;
	pParams->bImportTodaySubfolders = TRUE;
	pParams->bImportTodaySinceLast = TRUE;
	
	pParams->bRealTimeUpdate = FALSE;
	
	pParams->pszDbBackupPath = NULL;
	pParams->pszDbRestoreFromPath = NULL;
	pParams->pszCfgBackupPath = NULL;
	pParams->pszCfgRestoreFromPath = NULL;
	pParams->pszLogCopyToPath = NULL;

	pParams->bReanalyzeEnabled = FALSE;
	pParams->bMessageIfNoLocationMatch = FALSE;

	pParams->uiLocationMatchType = 0;
	pParams->uiLocationMatchItem = 0;
	//pParams->bSendLocation = FALSE;

	pParams->fMeasAT_Threshold = 100.0;
	pParams->fMeasAT_Bias = (float)0.1;
	pParams->fMeasAT_Sigma = 3.0;
	pParams->fMeasGateWidth = 64.0;
	pParams->bMeasTerminateOnMoving = FALSE;
	pParams->bMeasInclude_AT_Fail = TRUE;

	pParams->bImportDiskPrompt = FALSE;
	pParams->bImportMultipleDisks = FALSE;
		
	if (!Label_Read(szRAD_MENU_IMPORT_SELECTED, &pParams->bImport_Selected_Menu_Opt, TRUE)) {
		Label_Error = szRAD_MENU_IMPORT_SELECTED;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MENU_IMPORT_TODAY, &pParams->bImport_Today_Menu_Opt, TRUE)) {
		Label_Error = szRAD_MENU_IMPORT_TODAY;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MENU_DETERMINE_MEASUREMENTS, &(pParams->bDetermine_Measurements_Menu_Opt), TRUE)){
		Label_Error = szRAD_MENU_DETERMINE_MEASUREMENTS;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MENU_DIRECTION_ANALYSIS, &(pParams->bDirection_Analysis_Menu_Opt), TRUE)) {
		Label_Error = szRAD_MENU_DIRECTION_ANALYSIS;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MENU_EXPORT_TO_INCC, &(pParams->bExport_to_INCC_Menu_Opt), TRUE)) {
		Label_Error = szRAD_MENU_EXPORT_TO_INCC;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MENU_MEASUREMENTS_SUMMARY, &(pParams->bMeasurements_Summary_Menu_Opt), TRUE)) {
		Label_Error = szRAD_MENU_MEASUREMENTS_SUMMARY;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MENU_DIRECTION_SUMMARY, &(pParams->bDirection_Summary_Menu_Opt), TRUE)) {
		Label_Error = szRAD_MENU_DIRECTION_SUMMARY;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MENU_INTEGRITY_CHECK, &(pParams->bData_Integrity_Menu_Opt), TRUE)) {
		Label_Error = szRAD_MENU_INTEGRITY_CHECK;
		return(FALSE);
	}
				
	if (!Label_Read(szRAD_WRITE_CHAN_EVENT_RESULTS_FILE, &(pParams->bChanEventResults), TRUE)) {
		Label_Error = szRAD_WRITE_CHAN_EVENT_RESULTS_FILE;
		return(FALSE);
	}	
	if (!Label_Read(szRAD_WRITE_DIR_EVENT_RESULTS_FILE, &(pParams->bDirEventResults), TRUE)) {
		Label_Error = szRAD_WRITE_DIR_EVENT_RESULTS_FILE;
		return(FALSE);
	}
//	if (!Label_Read(szRAD_WRITE_MEASUREMENT_RESULTS_FILE, &(pParams->uiMeasurementResults))) {		// 1/3/2005 SFK Changed to UINT
//		Label_Error = szRAD_WRITE_MEASUREMENT_RESULTS_FILE;
//		return(FALSE);
//	}
	
	// 15-Feb-2005 SFK Changed to convert an old RAD.INI value of TRUE/FALSE to something that has meaning with the
	// new definition of this parameter.  No longer does an annoying message pop up saying you have a bad parameter.
	// 03-Jan-2006 SFK Changed to properly handle string returned (I think)
	char *pszTemp;
	if (!Label_Read(szRAD_WRITE_MEASUREMENT_RESULTS_FILE, &(pszTemp))) {		// 1/3/2005 SFK Changed to UINT
		Label_Error = szRAD_WRITE_MEASUREMENT_RESULTS_FILE;
		return(FALSE);
	}
	else {
		if (strlen(pszTemp) > 1) { // must be an old RAD.INI with TRUE/FALSE in the value
			if (strstr(pszTemp, "FALSE")) pParams->uiMeasurementResults = 0;
			else pParams->uiMeasurementResults = 7;
		}
		else
			sscanf(pszTemp, "%u\0", &pParams->uiMeasurementResults);
	}
	delete [] pszTemp;		// this was allocated in Label_Read so need to destroy now.

//	if (!Label_Read(szRAD_NAMES_CFG, &(pParams->pszNamesCfg))) {
//		Label_Error = szRAD_NAMES_CFG;
//		return(FALSE);
//	}
//	if (!Label_Read(szRAD_EXE_PATH, &(pParams->pszRADExePath))) {
//		Label_Error = szRAD_EXE_PATH;
//		return(FALSE);
//	}			
	
	if (!Label_Read(szRAD_IMPORT_AUTOMATE_QUESTION, &(pParams->bAutomateQuestion), TRUE)) {
		Label_Error = szRAD_IMPORT_AUTOMATE_QUESTION;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_ALL, &(pParams->bAutomateImportAll), TRUE)) {	         
		Label_Error = szRAD_IMPORT_ALL;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_TODAY, &(pParams->bAutomateImportToday), TRUE)) {
		Label_Error = szRAD_IMPORT_TODAY;
		return(FALSE);
	}
	if (!Label_Read(szRAD_INTEGRITY, &(pParams->bAutomateIntegrity), TRUE)) {
			Label_Error = szRAD_INTEGRITY;
		return(FALSE);
	}
	if (!Label_Read(szRAD_FIND_EVENTS, &(pParams->bAutomateFindEvents), TRUE)) {
		Label_Error = szRAD_FIND_EVENTS;
		return(FALSE);
	}
	if (!Label_Read(szRAD_DIRECTION, &(pParams->bAutomateDirection), TRUE)) {
		Label_Error = szRAD_DIRECTION;
		return(FALSE);
	}
	if (!Label_Read(szRAD_GRAPH, &(pParams->bAutomateGraph), TRUE)) {
		Label_Error = szRAD_GRAPH;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MEASUREMENT, &(pParams->bAutomateMeasurement), TRUE)) {
		Label_Error = szRAD_MEASUREMENT;
		return(FALSE);
	}
	if (!Label_Read(szRAD_EXPORT, &(pParams->bAutomateExport), TRUE)) {	//	1/3/2005 SFK Changed to int type
			Label_Error = szRAD_EXPORT;
		return(FALSE);
	}
	
    if (!Label_Read(szRAD_NCC_DATA_PATH, &(pParams->pszNCC_DataPath))) {
		Label_Error = szRAD_NCC_DATA_PATH;
		return(FALSE);
	}
	
	if (!Label_Read(szRAD_IMPORT_ALL_PATH, &(pParams->pszImportAllPath))) {
		Label_Error = szRAD_IMPORT_ALL_PATH;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_ALL_WILDCARD, &(pParams->pszImportAllWildcard))) {
		Label_Error = szRAD_IMPORT_ALL_WILDCARD;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_ALL_OVERWRITE, &(pParams->bImportAllOverwrite), TRUE)) {
		Label_Error = szRAD_IMPORT_ALL_OVERWRITE;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_ALL_INIT, &(pParams->bImportAllInit), TRUE)) {
		Label_Error = szRAD_IMPORT_ALL_INIT;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_ALL_SUBFOLDERS, &(pParams->bImportAllSubfolders), TRUE)) {
		Label_Error = szRAD_IMPORT_ALL_SUBFOLDERS;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_SELECT_PATH, &(pParams->pszImportSelectPath))) {
		Label_Error = szRAD_IMPORT_SELECT_PATH;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_SELECT_WILDCARD, &(pParams->pszImportSelectWildcard))) {
		Label_Error = szRAD_IMPORT_SELECT_WILDCARD;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_SELECT_OVERWRITE, &(pParams->bImportSelectOverwrite), TRUE)) {
		Label_Error = szRAD_IMPORT_SELECT_OVERWRITE;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_SELECT_INIT, &(pParams->bImportSelectInit), TRUE)) {
		Label_Error = szRAD_IMPORT_SELECT_INIT;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_SELECT_SUBFOLDERS, &(pParams->bImportSelectSubfolders), TRUE)) {
		Label_Error = szRAD_IMPORT_SELECT_SUBFOLDERS;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_TODAY_PATH, &(pParams->pszImportTodayPath))) {
		Label_Error = szRAD_IMPORT_TODAY_PATH;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_TODAY_WILDCARD, &(pParams->pszImportTodayWildcard))) {
		Label_Error = szRAD_IMPORT_TODAY_WILDCARD;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_TODAY_OVERWRITE, &(pParams->bImportTodayOverwrite), TRUE)) {
		Label_Error = szRAD_IMPORT_TODAY_OVERWRITE;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_TODAY_INIT, &(pParams->bImportTodayInit), TRUE)) {
		Label_Error = szRAD_IMPORT_TODAY_INIT;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_TODAY_SUBFOLDERS, &(pParams->bImportTodaySubfolders), TRUE)) {
		Label_Error = szRAD_IMPORT_TODAY_SUBFOLDERS;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_TODAY_SINCE_LAST, &(pParams->bImportTodaySinceLast), TRUE)) {
		Label_Error = szRAD_IMPORT_TODAY_SINCE_LAST;
		return(FALSE);
	}

	if (!Label_Read(szRAD_DB_BACKUP_PATH, &(pParams->pszDbBackupPath))) {
		Label_Error = szRAD_DB_BACKUP_PATH;
		return(FALSE);
	}
	if (!Label_Read(szRAD_DB_RESTORE_FROM_PATH, &(pParams->pszDbRestoreFromPath))) {
		Label_Error = szRAD_DB_RESTORE_FROM_PATH;
		return(FALSE);
	}
	if (!Label_Read(szRAD_CFG_BACKUP_PATH, &(pParams->pszCfgBackupPath))) {
		Label_Error = szRAD_CFG_BACKUP_PATH;
		return(FALSE);
	}
	if (!Label_Read(szRAD_CFG_RESTORE_FROM_PATH, &(pParams->pszCfgRestoreFromPath))) {
		Label_Error = szRAD_CFG_RESTORE_FROM_PATH;
		return(FALSE);
	}
	if (!Label_Read(szRAD_LOG_COPY_TO_PATH, &(pParams->pszLogCopyToPath))) {
		Label_Error = szRAD_LOG_COPY_TO_PATH;
		return(FALSE);
	}

	if (!Label_Read(szRAD_REAL_TIME_UPDATE, &(pParams->bRealTimeUpdate), TRUE)) {
		Label_Error = szRAD_REAL_TIME_UPDATE;
		return(FALSE);
	}			

	if (!Label_Read(szRAD_LOCATION_NO_MATCH_MSG, &(pParams->bMessageIfNoLocationMatch), TRUE)) {
		Label_Error = szRAD_LOCATION_NO_MATCH_MSG;
		return(FALSE);
	}			
	
	if (!Label_Read(szRAD_REANALYZE_ENABLED, &(pParams->bReanalyzeEnabled), TRUE)) {
		Label_Error = szRAD_REANALYZE_ENABLED;
		return(FALSE);
	}			
	
	if (!Label_Read(szRAD_LOCATION_MATCH_TYPE, &pParams->uiLocationMatchType)) {
		Label_Error = szRAD_LOCATION_MATCH_TYPE;
		return(FALSE);
	}			
	
	if (!Label_Read(szRAD_LOCATION_MATCH_ITEM, &(pParams->uiLocationMatchItem))) {
		Label_Error = szRAD_LOCATION_MATCH_ITEM;
		return(FALSE);
	}			

//	if (!Label_Read(szRAD_SEND_LOCATION, &(pParams->bSendLocation), TRUE)) {
//		Label_Error = szRAD_SEND_LOCATION;
//		return(FALSE);
//	}			

	
	if (!Label_Read(szRAD_MEAS_AT_THRES, &(pParams->fMeasAT_Threshold))) {
		Label_Error = szRAD_MEAS_AT_THRES;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MEAS_AT_BIAS, &(pParams->fMeasAT_Bias))) {
		Label_Error = szRAD_MEAS_AT_BIAS;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MEAS_AT_SIGMA, &(pParams->fMeasAT_Sigma))) {
		Label_Error = szRAD_MEAS_AT_SIGMA;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MEAS_GATE_WIDTH, &(pParams->fMeasGateWidth))) {
		Label_Error = szRAD_MEAS_GATE_WIDTH;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MEAS_TERM_ON_MOVING, &(pParams->bMeasTerminateOnMoving), TRUE)) {
		Label_Error = szRAD_MEAS_TERM_ON_MOVING;
		return(FALSE);
	}
	if (!Label_Read(szRAD_MEAS_INCLUDE_AT_FAIL, &(pParams->bMeasInclude_AT_Fail), TRUE)) {
		Label_Error = szRAD_MEAS_INCLUDE_AT_FAIL;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_DISK_PROMPT, &(pParams->bImportDiskPrompt), TRUE)) {
		Label_Error = szRAD_IMPORT_DISK_PROMPT;
		return(FALSE);
	}
	if (!Label_Read(szRAD_IMPORT_MULTIPLE_DISKS, &(pParams->bImportMultipleDisks), TRUE)) {
		Label_Error = szRAD_IMPORT_MULTIPLE_DISKS;
		return(FALSE);
	}

	// 11-Jul-2005 SFK Added two new parameters and default a value so
	//	INI file will be written correctly on exit -- don't force user to edit INI.
	if (!Label_Read(szRAD_R_T_UPPER_LIMIT, &(pParams->fRTUpperLimit))) {
		pParams->fRTUpperLimit = 0.5;
		//Label_Error = szRAD_IMPORT_MULTIPLE_DISKS;
		//return(FALSE);
	}

	if (!Label_Read(szRAD_ENABLE_RATIOS, &(pParams->bEnableRatios), TRUE)) {
		pParams->bEnableRatios = FALSE;
		//Label_Error = szRAD_IMPORT_MULTIPLE_DISKS;
		//return(FALSE);
	}

	return TRUE;
}





/////////////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////////// 
// Member Function to return the status of whether the member parameter structure has been loaded
/////////////////////////////////////////////////////////////////////////////////////////// 

BOOL RADInit::Parameters_Loaded(void) {
	return bParams_Loaded;
}


                                     
                                     
                                     
/////////////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////////// 
// Member Functions to Get, Set or Reset the parameters in the RAD_PARAMS structure
/////////////////////////////////////////////////////////////////////////////////////////// 

// Whether to activate Import Selected Files/////////////////////////////////////////
BOOL RADInit::Get_Menu_Opt_Import_Selected(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImport_Selected_Menu_Opt);
	else return FALSE;
}

BOOL RADInit::Set_Menu_Opt_Import_Selected(BOOL bNewImport_Selected_Menu_Opt) {
	if (bParams_Loaded) {
        pParams_Current->bImport_Selected_Menu_Opt = bNewImport_Selected_Menu_Opt;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Menu_Opt_Import_Selected(void) {
	return (Set_Menu_Opt_Import_Selected(pParams_Default->bImport_Selected_Menu_Opt));
}




// Whether to activate Import Today's Files/////////////////////////////////////////
BOOL RADInit::Get_Menu_Opt_Import_Today(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImport_Today_Menu_Opt);
	else return FALSE;
}

BOOL RADInit::Set_Menu_Opt_Import_Today(BOOL bNewImport_Today_Menu_Opt) {
	if (bParams_Loaded) {
        pParams_Current->bImport_Today_Menu_Opt = bNewImport_Today_Menu_Opt;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Menu_Opt_Import_Today(void) {
	return (Set_Menu_Opt_Import_Today(pParams_Default->bImport_Today_Menu_Opt));
}




// Whether to Activate Determine Measurements////////////////////////////////////////
BOOL RADInit::Get_Menu_Opt_Determine_Measurements(void) {
	if (bParams_Loaded)
		return (pParams_Current->bDetermine_Measurements_Menu_Opt);
	else return FALSE;
}

BOOL RADInit::Set_Menu_Opt_Determine_Measurements(BOOL bNewDetermine_Measurements_Menu_Opt) {
	if (bParams_Loaded) {
        pParams_Current->bDetermine_Measurements_Menu_Opt = bNewDetermine_Measurements_Menu_Opt;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Menu_Opt_Determine_Measurements(void) {
	return (Set_Menu_Opt_Determine_Measurements(pParams_Default->bDetermine_Measurements_Menu_Opt));
}




// Whether to Activate Direction Analysis/////////////////////////////////////////////
BOOL RADInit::Get_Menu_Opt_Direction_Analysis(void) {
	if (bParams_Loaded)
		return (pParams_Current->bDirection_Analysis_Menu_Opt);
	else return FALSE;
}

BOOL RADInit::Set_Menu_Opt_Direction_Analysis(BOOL bNewDirection_Analysis_Menu_Opt) {
	if (bParams_Loaded) {
        pParams_Current->bDirection_Analysis_Menu_Opt = bNewDirection_Analysis_Menu_Opt;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Menu_Opt_Direction_Analysis(void) {
	return (Set_Menu_Opt_Direction_Analysis(pParams_Default->bDirection_Analysis_Menu_Opt));
}




// Whether to activate Export to INCC/////////////////////////////////////////////////
BOOL RADInit::Get_Menu_Opt_Export_to_INCC(void) {
	if (bParams_Loaded)
		return (pParams_Current->bExport_to_INCC_Menu_Opt);
	else return FALSE;
}

BOOL RADInit::Set_Menu_Opt_Export_to_INCC(BOOL bNewExport_to_INCC_Menu_Opt) {
	if (bParams_Loaded) {
        pParams_Current->bExport_to_INCC_Menu_Opt = bNewExport_to_INCC_Menu_Opt;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Menu_Opt_Export_to_INCC(void) {
	return (Set_Menu_Opt_Export_to_INCC(pParams_Default->bExport_to_INCC_Menu_Opt));
}




// Whether to activate Measurements Summary////////////////////////////////////////////
BOOL RADInit::Get_Menu_Opt_Measurements_Summary(void) {
	if (bParams_Loaded)
		return (pParams_Current->bMeasurements_Summary_Menu_Opt);
	else return FALSE;
}

BOOL RADInit::Set_Menu_Opt_Measurements_Summary(BOOL bNewMeasurements_Summary_Menu_Opt) {
	if (bParams_Loaded) {
        pParams_Current->bMeasurements_Summary_Menu_Opt = bNewMeasurements_Summary_Menu_Opt;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Menu_Opt_Measurements_Summary(void) {
	return (Set_Menu_Opt_Measurements_Summary(pParams_Default->bMeasurements_Summary_Menu_Opt));
}




// Whether to write a rad.db results file after finding channel events //////////////////////////
BOOL RADInit::Get_WriteChanEventResults(void) {
	if (bParams_Loaded)
		return (pParams_Current->bChanEventResults);
	else return FALSE;
}

BOOL RADInit::Set_WriteChanEventResults(BOOL bNewChanEventResults) {
	if (bParams_Loaded) {
        pParams_Current->bChanEventResults = bNewChanEventResults;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_WriteChanEventResults(void) {
	return (Set_WriteChanEventResults(pParams_Default->bChanEventResults));
}



// Whether to write a rad.db results file after finding direction events /////////////////////////
BOOL RADInit::Get_WriteDirEventResults(void) {
	if (bParams_Loaded)
		return (pParams_Current->bDirEventResults);
	else return FALSE;
}

BOOL RADInit::Set_WriteDirEventResults(BOOL bNewDirEventResults) {
	if (bParams_Loaded) {
        pParams_Current->bDirEventResults = bNewDirEventResults;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_WriteDirEventResults(void) {
	return (Set_WriteDirEventResults(pParams_Default->bDirEventResults));
}



// Whether to write a rad.db results file after exporting measurements to INCC ////////////////////
// 1/3/2005	SFK Change to unsigned int
UINT RADInit::Get_WriteMeasurementResults(void) {
	if (bParams_Loaded)
		return (pParams_Current->uiMeasurementResults);		// 1/3/2005	SFK Change to unsigned int
	else return FALSE;
}

BOOL RADInit::Set_WriteMeasurementResults(UINT uiNewMeasurementResults) {
	if (bParams_Loaded) {
        pParams_Current->uiMeasurementResults = uiNewMeasurementResults;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_WriteMeasurementResults(void) {
	return (Set_WriteMeasurementResults(pParams_Default->uiMeasurementResults));
}



// Whether to activate Direction Summary menu option/////////////////////////////////////////////
BOOL RADInit::Get_Menu_Opt_Direction_Summary(void) {
	if (bParams_Loaded)
		return (pParams_Current->bDirection_Summary_Menu_Opt);
	else return FALSE;
}

BOOL RADInit::Set_Menu_Opt_Direction_Summary(BOOL bNewDirection_Summary_Menu_Opt) {
	if (bParams_Loaded) {
        pParams_Current->bDirection_Summary_Menu_Opt = bNewDirection_Summary_Menu_Opt;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Menu_Opt_Direction_Summary(void) {
	return (Set_Menu_Opt_Direction_Summary(pParams_Default->bDirection_Summary_Menu_Opt));
}


// Whether to activate Data Integrity menu option/////////////////////////////////////////////
BOOL RADInit::Get_Menu_Opt_Data_Integrity(void) {
	if (bParams_Loaded)
		return (pParams_Current->bData_Integrity_Menu_Opt);
	else return FALSE;
}

BOOL RADInit::Set_Menu_Opt_Data_Integrity(BOOL bNewData_Integrity_Menu_Opt) {
	if (bParams_Loaded) {
        pParams_Current->bData_Integrity_Menu_Opt = bNewData_Integrity_Menu_Opt;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Menu_Opt_Data_Integrity(void) {
	return (Set_Menu_Opt_Data_Integrity(pParams_Default->bDirection_Summary_Menu_Opt));
}
 

//  File containing facility, station and channel names /////////////////////////////////////
//const char *RADInit::Get_Facility_Names(void) {
//	if (bParams_Loaded)
//		return ((const char *) pParams_Current->pszNamesCfg);
//	else return FALSE;
//}

//BOOL RADInit::Set_Facility_Names(const char *pszNamesCfg) {
//	if (bParams_Loaded) {
//		if (pParams_Current->pszNamesCfg)
//			delete [] pParams_Current->pszNamesCfg;
//		if ((pParams_Current->pszNamesCfg = new char [strlen(pszNamesCfg) + 1]) != NULL) {
//			strcpy(pParams_Current->pszNamesCfg, pszNamesCfg);
//			return TRUE;
//		}
//	}
//	return FALSE;
//}

//BOOL RADInit::Reset_Facility_Names(void) {
//	return (Set_Facility_Names(pParams_Default->pszNamesCfg));
//}

//  Path of RAD.EXE being used /////////////////////////////////////////////////////////////////
//const char *RADInit::Get_RADExe_Path(void) {
//	if (bParams_Loaded)
//		return ((const char *) pParams_Current->pszRADExePath);
//	else return FALSE;
//}

//BOOL RADInit::Set_RADExe_Path(const char *pszRADExePath) {
//	if (bParams_Loaded) {
//		if (pParams_Current->pszRADExePath)
//			delete [] pParams_Current->pszRADExePath;
//		if ((pParams_Current->pszRADExePath = new char [strlen(pszRADExePath) + 1]) != NULL) {
//			strcpy(pParams_Current->pszRADExePath, pszRADExePath);
//			return TRUE;
//		}
//	}
//	return FALSE;
//}

//BOOL RADInit::Reset_RADExe_Path(void) {
//	return (Set_RADExe_Path(pParams_Default->pszRADExePath));
//}


// Whether to ask question about overwriting database during /IMPORT /////////////////////////////////
BOOL RADInit::Get_AutomateQuestion(void) {
	if (bParams_Loaded)
		return (pParams_Current->bAutomateQuestion);
	else return FALSE;
}

BOOL RADInit::Set_AutomateQuestion(BOOL bNewAutomateQuestion) {
	if (bParams_Loaded) {
        pParams_Current->bAutomateQuestion = bNewAutomateQuestion;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_AutomateQuestion(void) {
	return (Set_AutomateQuestion(pParams_Default->bAutomateQuestion));
}




// Whether to automate ImportAll during /IMPORT ////////////////////////////////////////
BOOL RADInit::Get_AutomateImportAll(void) {
	if (bParams_Loaded)
		return (pParams_Current->bAutomateImportAll);
	else return FALSE;
}

BOOL RADInit::Set_AutomateImportAll(BOOL bAutomateImportAll) {
	if (bParams_Loaded) {
        pParams_Current->bAutomateImportAll = bAutomateImportAll;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_AutomateImportAll(void) {
	return (Set_AutomateImportAll(pParams_Default->bAutomateImportAll));
}




// Whether to automate Today during /IMPORT ////////////////////////////////////////////
BOOL RADInit::Get_AutomateImportToday(void) {
	if (bParams_Loaded)
		return (pParams_Current->bAutomateImportToday);
	else return FALSE;
}

BOOL RADInit::Set_AutomateImportToday(BOOL bNewAutomateImportToday) {
	if (bParams_Loaded) {
        pParams_Current->bAutomateImportToday = bNewAutomateImportToday;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_AutomateImportToday(void) {
	return (Set_AutomateImportToday(pParams_Default->bAutomateImportToday));
}




// Whether to automate integrity check during /IMPORT ////////////////////////////////
BOOL RADInit::Get_AutomateIntegrity(void) {
	if (bParams_Loaded)
		return (pParams_Current->bAutomateIntegrity);
	else return FALSE;
}

BOOL RADInit::Set_AutomateIntegrity(BOOL bNewAutomateIntegrity) {
	if (bParams_Loaded) {
        pParams_Current->bAutomateIntegrity = bNewAutomateIntegrity;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_AutomateIntegrity(void) {
	return (Set_AutomateIntegrity(pParams_Default->bAutomateIntegrity));
}




// Whether to automate find events during /IMPORT //////////////////////////////////////
BOOL RADInit::Get_AutomateFindEvents(void) {
	if (bParams_Loaded)
		return (pParams_Current->bAutomateFindEvents);
	else return FALSE;
}

BOOL RADInit::Set_AutomateFindEvents(BOOL bNewAutomateFindEvents) {
	if (bParams_Loaded) {
        pParams_Current->bAutomateFindEvents = bNewAutomateFindEvents;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_AutomateFindEvents(void) {
	return (Set_AutomateFindEvents(pParams_Default->bAutomateFindEvents));
}




// Whether to automate direction during /IMPORT ////////////////////////////////////////
BOOL RADInit::Get_AutomateDirection(void) {
	if (bParams_Loaded)
		return (pParams_Current->bAutomateDirection);
	else return FALSE;
}

BOOL RADInit::Set_AutomateDirection(BOOL bNewAutomateDirection) {
	if (bParams_Loaded) {
        pParams_Current->bAutomateDirection = bNewAutomateDirection;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_AutomateDirection(void) {
	return (Set_AutomateDirection(pParams_Default->bAutomateDirection));
}




// Whether to automate the graph  during /IMPORT //////////////////////////////////
BOOL RADInit::Get_AutomateGraph(void) {
	if (bParams_Loaded)
		return (pParams_Current->bAutomateGraph);
	else return FALSE;
}

BOOL RADInit::Set_AutomateGraph(BOOL bNewAutomateGraph) {
	if (bParams_Loaded) {
        pParams_Current->bAutomateGraph = bNewAutomateGraph;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_AutomateGraph(void) {
	return (Set_AutomateGraph(pParams_Default->bAutomateGraph));
}




// Whether to automate measurement during /IMPORT /////////////////////////////////
BOOL RADInit::Get_AutomateMeasurement(void) {
	if (bParams_Loaded)
		return (pParams_Current->bAutomateMeasurement);
	else return FALSE;
}

BOOL RADInit::Set_AutomateMeasurement(BOOL bNewAutomateMeasurement) {
	if (bParams_Loaded) {
        pParams_Current->bAutomateMeasurement = bNewAutomateMeasurement;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_AutomateMeasurement(void) {
	return (Set_AutomateMeasurement(pParams_Default->bAutomateMeasurement));
}




// Whether to automate export during /IMPORT //////////////////////////////////////////
BOOL RADInit::Get_AutomateExport(void) {
	if (bParams_Loaded)
		return (pParams_Current->bAutomateExport);
	else return FALSE;
}

BOOL RADInit::Set_AutomateExport(BOOL bNewAutomateExport) {
	if (bParams_Loaded) {
        pParams_Current->bAutomateExport = bNewAutomateExport;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_AutomateExport(void) {
	return (Set_AutomateExport(pParams_Default->bAutomateExport));
}


// Paths for .NCC files produced for use by NCC //////////////////////////////////////////////
char *RADInit::Get_NCC_DataPath(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszNCC_DataPath);
	else return FALSE;
}

BOOL RADInit::Set_NCC_DataPath(const char *pszNCC_DataPath) {
	if (bParams_Loaded) {
		if (pParams_Current->pszNCC_DataPath)
			delete [] pParams_Current->pszNCC_DataPath;
		if ((pParams_Current->pszNCC_DataPath = new char [strlen(pszNCC_DataPath) + 1]) != NULL) {
			strcpy(pParams_Current->pszNCC_DataPath, pszNCC_DataPath);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL RADInit::Reset_NCC_DataPath(void) {
	return (Set_NCC_DataPath(pParams_Default->pszNCC_DataPath));
}


// Paths for files in Import All option //////////////////////////////////////////////
char *RADInit::Get_Import_All_Path(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszImportAllPath);
	else return FALSE;
}

BOOL RADInit::Set_Import_All_Path(const char *pszImportAllPath) {
	if (bParams_Loaded) {
		if (pParams_Current->pszImportAllPath)
			delete [] pParams_Current->pszImportAllPath;
		if ((pParams_Current->pszImportAllPath = new char [strlen(pszImportAllPath) + 1]) != NULL) {
			strcpy(pParams_Current->pszImportAllPath, pszImportAllPath);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_All_Path(void) {
	return (Set_Import_All_Path(pParams_Default->pszImportAllPath));
}




// Wildcard Name for files in Import All option ////////////////////////////////////////
char *RADInit::Get_Import_All_Wildcard(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszImportAllWildcard);
	else return FALSE;
}

BOOL RADInit::Set_Import_All_Wildcard(const char *pszImportAllWildcard) {
	if (bParams_Loaded) {
		if (pParams_Current->pszImportAllWildcard)
			delete [] pParams_Current->pszImportAllWildcard;
		if ((pParams_Current->pszImportAllWildcard = new char [strlen(pszImportAllWildcard) + 1]) != NULL) {
			strcpy(pParams_Current->pszImportAllWildcard, pszImportAllWildcard);
			return TRUE;
			}
		}
	return FALSE;
}

BOOL RADInit::Reset_Import_All_Wildcard(void) {
	return (Set_Import_All_Wildcard(pParams_Default->pszImportAllWildcard));
}
    



// Whether to overwrite existing database in Import All option ////////////////////////////// 
BOOL RADInit::Get_Import_All_Overwrite(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportAllOverwrite);
	else return FALSE;
}

BOOL RADInit::Set_Import_All_Overwrite(BOOL bNewImportAllOverwrite) {
	if (bParams_Loaded) {
        pParams_Current->bImportAllOverwrite = bNewImportAllOverwrite;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_All_Overwrite(void) {
	return (Set_Import_All_Overwrite(pParams_Default->bImportAllOverwrite));
}




// Whether to initialize existing database in Import All option //////////////////////////// 
BOOL RADInit::Get_Import_All_Init(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportAllInit);
	else return FALSE;
}

BOOL RADInit::Set_Import_All_Init(BOOL bNewImportAllInit) {
	if (bParams_Loaded) {
        pParams_Current->bImportAllInit = bNewImportAllInit;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_All_Init(void) {
	return (Set_Import_All_Init(pParams_Default->bImportAllInit));
}


// Whether to include subfolders in Import All option //////////////////////////// 
BOOL RADInit::Get_Import_All_Subfolders(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportAllSubfolders);
	else return FALSE;
}

BOOL RADInit::Set_Import_All_Subfolders(BOOL bNewImportAllSubfolders) {
	if (bParams_Loaded) {
        pParams_Current->bImportAllSubfolders = bNewImportAllSubfolders;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_All_Subfolders(void) {
	return (Set_Import_All_Subfolders(pParams_Default->bImportAllSubfolders));
}



// Paths for files in Import Selected option /////////////////////////////////////////
char *RADInit::Get_Import_Select_Path(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszImportSelectPath);
	else return FALSE;
}

BOOL RADInit::Set_Import_Select_Path(const char *pszImportSelectPath) {
	if (bParams_Loaded) {
		if (pParams_Current->pszImportSelectPath)
			delete [] pParams_Current->pszImportSelectPath;
		if ((pParams_Current->pszImportSelectPath = new char [strlen(pszImportSelectPath) + 1]) != NULL) {
			strcpy(pParams_Current->pszImportSelectPath, pszImportSelectPath);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_Select_Path(void) {
	return (Set_Import_Select_Path(pParams_Default->pszImportSelectPath));
}




// Wildcard Name for files in Import Select option ////////////////////////////////////////// 
char *RADInit::Get_Import_Select_Wildcard(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszImportSelectWildcard);
	else return FALSE;
}

BOOL RADInit::Set_Import_Select_Wildcard(const char *pszImportSelectWildcard) {
	if (bParams_Loaded) {
		if (pParams_Current->pszImportSelectWildcard)
			delete [] pParams_Current->pszImportSelectWildcard;
		if ((pParams_Current->pszImportSelectWildcard = new char [strlen(pszImportSelectWildcard) + 1]) != NULL) {
			strcpy(pParams_Current->pszImportSelectWildcard, pszImportSelectWildcard);
			return TRUE;
			}
		}
	return FALSE;
}

BOOL RADInit::Reset_Import_Select_Wildcard(void) {
	return (Set_Import_Select_Wildcard(pParams_Default->pszImportSelectWildcard));
}
    



// Whether to overwrite existing database in Import Select option //////////////////////////////
BOOL RADInit::Get_Import_Select_Overwrite(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportSelectOverwrite);
	else return FALSE;
}

BOOL RADInit::Set_Import_Select_Overwrite(BOOL bNewImportSelectOverwrite) {
	if (bParams_Loaded) {
        pParams_Current->bImportSelectOverwrite = bNewImportSelectOverwrite;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_Select_Overwrite(void) {
	return (Set_Import_Select_Overwrite(pParams_Default->bImportSelectOverwrite));
}




// Whether to initialize existing database in Import Select option ////////////////////////////// 
BOOL RADInit::Get_Import_Select_Init(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportSelectInit);
	else return FALSE;
}

BOOL RADInit::Set_Import_Select_Init(BOOL bNewImportSelectInit) {
	if (bParams_Loaded) {
        pParams_Current->bImportSelectInit = bNewImportSelectInit;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_Select_Init(void) {
	return (Set_Import_Select_Init(pParams_Default->bImportSelectInit));
}


// Whether to include subfolders in Import Select option //////////////////////////// 
BOOL RADInit::Get_Import_Select_Subfolders(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportSelectSubfolders);
	else return FALSE;
}

BOOL RADInit::Set_Import_Select_Subfolders(BOOL bNewImportSelectSubfolders) {
	if (bParams_Loaded) {
        pParams_Current->bImportSelectSubfolders = bNewImportSelectSubfolders;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_Select_Subfolders(void) {
	return (Set_Import_Select_Subfolders(pParams_Default->bImportSelectSubfolders));
}



// Paths for files in Import Today option 
char *RADInit::Get_Import_Today_Path(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszImportTodayPath);
	else return FALSE;
}

BOOL RADInit::Set_Import_Today_Path(const char *pszImportTodayPath) {
	if (bParams_Loaded) {
		if (pParams_Current->pszImportTodayPath)
			delete [] pParams_Current->pszImportTodayPath;
		if ((pParams_Current->pszImportTodayPath = new char [strlen(pszImportTodayPath) + 1]) != NULL) {
			strcpy(pParams_Current->pszImportTodayPath, pszImportTodayPath);
			return TRUE;
			}
		}
	return FALSE;
}

BOOL RADInit::Reset_Import_Today_Path(void) {
	return (Set_Import_Today_Path(pParams_Default->pszImportTodayPath));
}




// Wildcard Name for files in Import Today option /////////////////////////////////////////////////
char *RADInit::Get_Import_Today_Wildcard(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszImportTodayWildcard);
	else return FALSE;
}

BOOL RADInit::Set_Import_Today_Wildcard(const char *pszImportTodayWildcard) {
	if (bParams_Loaded) {
		if (pParams_Current->pszImportTodayWildcard)
			delete [] pParams_Current->pszImportTodayWildcard;
		if ((pParams_Current->pszImportTodayWildcard = new char [strlen(pszImportTodayWildcard) + 1]) != NULL) {
			strcpy(pParams_Current->pszImportTodayWildcard, pszImportTodayWildcard);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_Today_Wildcard(void) {
	return (Set_Import_Today_Wildcard(pParams_Default->pszImportTodayWildcard));
}
    



// Whether to overwrite existing database in Import Today option ///////////////////////////////////
BOOL RADInit::Get_Import_Today_Overwrite(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportTodayOverwrite);
	else return FALSE;
}

BOOL RADInit::Set_Import_Today_Overwrite(BOOL bNewImportTodayOverwrite) {
	if (bParams_Loaded) {
        pParams_Current->bImportTodayOverwrite = bNewImportTodayOverwrite;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_Today_Overwrite(void) {
	return (Set_Import_Today_Overwrite(pParams_Default->bImportTodayOverwrite));
}




// Whether to initialize existing database in Import Today option //////////////////////////////////////
BOOL RADInit::Get_Import_Today_Init(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportTodayInit);
	else return FALSE;
}

BOOL RADInit::Set_Import_Today_Init(BOOL bNewImportTodayInit) {
	if (bParams_Loaded) {
        pParams_Current->bImportTodayInit = bNewImportTodayInit;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_Today_Init(void) {
	return (Set_Import_Today_Overwrite(pParams_Default->bImportTodayInit));
}


// Whether to include subfolders in Import Today option //////////////////////////// 
BOOL RADInit::Get_Import_Today_Subfolders(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportTodaySubfolders);
	else return FALSE;
}

BOOL RADInit::Set_Import_Today_Subfolders(BOOL bNewImportTodaySubfolders) {
	if (bParams_Loaded) {
        pParams_Current->bImportTodaySubfolders = bNewImportTodaySubfolders;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_Today_Subfolders(void) {
	return (Set_Import_Today_Subfolders(pParams_Default->bImportTodaySubfolders));
}


// Whether to import everything since last import or only this day in Import Today option //////////////////////////// 
BOOL RADInit::Get_Import_Today_Since_Last(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportTodaySinceLast);
	else return FALSE;
}

BOOL RADInit::Set_Import_Today_Since_Last(BOOL bNewImportTodaySinceLast) {
	if (bParams_Loaded) {
        pParams_Current->bImportTodaySinceLast = bNewImportTodaySinceLast;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Import_Today_Since_Last(void) {
	return (Set_Import_Today_Since_Last(pParams_Default->bImportTodaySinceLast));
}



//	Path where to backup the database ////////////////////////////////////////////////////////////////////
char *RADInit::Get_dbBackup_Path(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszDbBackupPath);
	else return FALSE;
}

BOOL RADInit::Set_dbBackup_Path(const char *pszDbBackupPath) {
	if (bParams_Loaded) {
		if (pParams_Current->pszDbBackupPath)
			delete [] pParams_Current->pszDbBackupPath;
		if ((pParams_Current->pszDbBackupPath = new char [strlen(pszDbBackupPath) + 1]) != NULL) {
			strcpy(pParams_Current->pszDbBackupPath, pszDbBackupPath);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL RADInit::Reset_dbBackup_Path(void) {
	return (Set_dbBackup_Path(pParams_Default->pszDbBackupPath));
}




//	Path from where to restore the database /////////////////////////////////////////////////////
char *RADInit::Get_dbRestore_From_Path(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszDbRestoreFromPath);
	else return FALSE;
}

BOOL RADInit::Set_dbRestore_From_Path(const char *pszDbRestoreFromPath) {
	if (bParams_Loaded) {
		if (pParams_Current->pszDbRestoreFromPath)
			delete [] pParams_Current->pszDbRestoreFromPath;
		if ((pParams_Current->pszDbRestoreFromPath = new char [strlen(pszDbRestoreFromPath) + 1]) != NULL) {
			strcpy(pParams_Current->pszDbRestoreFromPath, pszDbRestoreFromPath);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL RADInit::Reset_dbRestore_From_Path(void) {
	return (Set_dbBackup_Path(pParams_Default->pszDbRestoreFromPath));
}




//	Path where to backup the configuration ////////////////////////////////////////////////////////////////////
char *RADInit::Get_CfgBackup_Path(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszCfgBackupPath);
	else return FALSE;
}

BOOL RADInit::Set_CfgBackup_Path(const char *pszCfgBackupPath) {
	if (bParams_Loaded) {
		if (pParams_Current->pszCfgBackupPath)
			delete [] pParams_Current->pszCfgBackupPath;
		if ((pParams_Current->pszCfgBackupPath = new char [strlen(pszCfgBackupPath) + 1]) != NULL) {
			strcpy(pParams_Current->pszCfgBackupPath, pszCfgBackupPath);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL RADInit::Reset_CfgBackup_Path(void) {
	return (Set_CfgBackup_Path(pParams_Default->pszCfgBackupPath));
}




//	Path from where to restore the configuration /////////////////////////////////////////////////////
char *RADInit::Get_CfgRestore_From_Path(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszCfgRestoreFromPath);
	else return FALSE;
}

BOOL RADInit::Set_CfgRestore_From_Path(const char *pszCfgRestoreFromPath) {
	if (bParams_Loaded) {
		if (pParams_Current->pszCfgRestoreFromPath)
			delete [] pParams_Current->pszCfgRestoreFromPath;
		if ((pParams_Current->pszCfgRestoreFromPath = new char [strlen(pszCfgRestoreFromPath) + 1]) != NULL) {
			strcpy(pParams_Current->pszCfgRestoreFromPath, pszCfgRestoreFromPath);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL RADInit::Reset_CfgRestore_From_Path(void) {
	return (Set_CfgBackup_Path(pParams_Default->pszCfgRestoreFromPath));
}



//	Path to copy the logs to /////////////////////////////////////////////////
char *RADInit::Get_LogCopy_To_Path(void) {
	if (bParams_Loaded)
		return (pParams_Current->pszLogCopyToPath);
	else return FALSE;
}

BOOL RADInit::Set_LogCopy_To_Path(const char *pszLogCopyToPath) {
	if (bParams_Loaded) {
		if (pParams_Current->pszLogCopyToPath)
			delete [] pParams_Current->pszLogCopyToPath;
		if ((pParams_Current->pszLogCopyToPath = new char [strlen(pszLogCopyToPath) + 1]) != NULL) {
			strcpy(pParams_Current->pszLogCopyToPath, pszLogCopyToPath);
			return TRUE;
			}
		}
	return FALSE;
}

BOOL RADInit::Reset_LogCopy_To_Path(void) {
	return (Set_LogCopy_To_Path(pParams_Default->pszLogCopyToPath));
}


// Whether realtime updates are possible
BOOL RADInit::Get_RealTimeUpdate(void) {
	if (bParams_Loaded)
		return (pParams_Current->bRealTimeUpdate);
	else return FALSE;
}

BOOL RADInit::Set_RealTimeUpdate(BOOL bNewRealTimeUpdate) {
	if (bParams_Loaded) {
        pParams_Current->bRealTimeUpdate = bNewRealTimeUpdate;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_RealTimeUpdate(void) {
	return (Set_RealTimeUpdate(pParams_Default->bRealTimeUpdate));
}


// Whether reanalayze enabled
BOOL RADInit::Get_ReanalyzeEnabled(void) {
	if (bParams_Loaded)
		return (pParams_Current->bReanalyzeEnabled);
	else return FALSE;
}

BOOL RADInit::Set_ReanalyzeEnabled(BOOL bNewReanalyzeEnabled) {
	if (bParams_Loaded) {
        pParams_Current->bReanalyzeEnabled = bNewReanalyzeEnabled;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_ReanalyzeEnabled(void) {
	return (Set_ReanalyzeEnabled(pParams_Default->bReanalyzeEnabled));
}

// Whether display message on location mismatch
BOOL RADInit::Get_MessageIfNoLocationMatch(void) {
	if (bParams_Loaded)
		return (pParams_Current->bMessageIfNoLocationMatch);
	else return FALSE;
}

BOOL RADInit::Set_MessageIfNoLocationMatch(BOOL bNewMessageIfNoLocationMatch) {
	if (bParams_Loaded) {
        pParams_Current->bMessageIfNoLocationMatch = bNewMessageIfNoLocationMatch;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_MessageIfNoLocationMatch(void) {
	return (Set_MessageIfNoLocationMatch(pParams_Default->bMessageIfNoLocationMatch));
}

// What type of location match to be doing (string or number)
UINT RADInit::Get_LocationMatchType(void) {
	if (bParams_Loaded)
		return (pParams_Current->uiLocationMatchType);
	else return 0;
}

BOOL RADInit::Set_LocationMatchType(UINT uiNewLocationMatchType) {
	if (bParams_Loaded) {
        pParams_Current->uiLocationMatchType = uiNewLocationMatchType;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_LocationMatchType(void) {
	return (Set_LocationMatchType(pParams_Default->uiLocationMatchType));
}

// What item for type of location match to be doing (facility, station or channel)
UINT RADInit::Get_LocationMatchItem(void) {
	if (bParams_Loaded)
		return (pParams_Current->uiLocationMatchItem);
	else return 0;
}

BOOL RADInit::Set_LocationMatchItem(UINT uiNewLocationMatchItem) {
	if (bParams_Loaded) {
        pParams_Current->uiLocationMatchItem = uiNewLocationMatchItem;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_LocationMatchItem(void) {
	return (Set_LocationMatchItem(pParams_Default->uiLocationMatchItem));
}


// Whether to send location
//BOOL RADInit::Get_SendLocation(void) {
//	if (bParams_Loaded)
//		return (pParams_Current->bSendLocation);
//	else return FALSE;
//}

//BOOL RADInit::Set_SendLocation(BOOL bNewSendLocation) {
//	if (bParams_Loaded) {
//        pParams_Current->bSendLocation = bNewSendLocation;
//		return TRUE;
//	}
//	return FALSE;
//}

//BOOL RADInit::Reset_SendLocation(void) {
//	return (Set_SendLocation(pParams_Default->bSendLocation));
//}


// SR Analysis parameter for AT Threshold
float RADInit::Get_Meas_AT_Threshold(void) {
	if (bParams_Loaded)
		return (pParams_Current->fMeasAT_Threshold);
	else return FALSE;
}

BOOL RADInit::Set_Meas_AT_Threshold(float fMeasAT_Threshold) {
	if (bParams_Loaded) {
        pParams_Current->fMeasAT_Threshold = fMeasAT_Threshold;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Meas_AT_Threshold(void) {
	return (Set_Meas_AT_Threshold(pParams_Default->fMeasAT_Threshold));
}

// SR Analysis parameter for AT Bias
float RADInit::Get_Meas_AT_Bias(void) {
	if (bParams_Loaded)
		return (pParams_Current->fMeasAT_Bias);
	else return 0.0;
}

BOOL RADInit::Set_Meas_AT_Bias(float fMeasAT_Bias) {
	if (bParams_Loaded) {
        pParams_Current->fMeasAT_Bias = fMeasAT_Bias;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Meas_AT_Bias(void) {
	return (Set_Meas_AT_Bias(pParams_Default->fMeasAT_Bias));
}


// SR Analysis parameter for AT Sigma
float RADInit::Get_Meas_AT_Sigma(void) {
	if (bParams_Loaded)
		return (pParams_Current->fMeasAT_Sigma);
	else return 0.0;
}

BOOL RADInit::Set_Meas_AT_Sigma(float fMeasAT_Sigma) {
	if (bParams_Loaded) {
        pParams_Current->fMeasAT_Sigma = fMeasAT_Sigma;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Meas_AT_Sigma(void) {
	return (Set_Meas_AT_Sigma(pParams_Default->fMeasAT_Sigma));
}



// SR Analysis parameter for GateWidth
float RADInit::Get_Meas_GateWidth(void) {
	if (bParams_Loaded)
		return (pParams_Current->fMeasGateWidth);
	else return 0.0;
}

BOOL RADInit::Set_Meas_GateWidth(float fMeasGateWidth) {
	if (bParams_Loaded) {
        pParams_Current->fMeasGateWidth = fMeasGateWidth;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_Meas_GateWidth(void) {
	return (Set_Meas_GateWidth(pParams_Default->fMeasGateWidth));
}

// Whether to terminate measurement when AT test fails
BOOL RADInit::Get_MeasTerminateOnMoving(void) {
	if (bParams_Loaded)
		return (pParams_Current->bMeasTerminateOnMoving);
	else return FALSE;
}

BOOL RADInit::Set_MeasTerminateOnMoving(BOOL bMeasTerminateOnMoving) {
	if (bParams_Loaded) {
        pParams_Current->bMeasTerminateOnMoving = bMeasTerminateOnMoving;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_MeasTerminateOnMoving(void) {
	return (Set_MeasTerminateOnMoving(pParams_Default->bMeasTerminateOnMoving));
}

// Whether to include runs that fail AT test in measurement
BOOL RADInit::Get_MeasInclude_AT_Fail(void) {
	if (bParams_Loaded)
		return (pParams_Current->bMeasInclude_AT_Fail);
	else return FALSE;
}

BOOL RADInit::Set_MeasInclude_AT_Fail(BOOL bMeasInclude_AT_Fail) {
	if (bParams_Loaded) {
        pParams_Current->bMeasInclude_AT_Fail = bMeasInclude_AT_Fail;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_MeasInclude_AT_Fail(void) {
	return (Set_MeasInclude_AT_Fail(pParams_Default->bMeasInclude_AT_Fail));
}

// Whether to put up a prompt after each disk import during /IMPORT
BOOL RADInit::Get_ImportMultipleDisks(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportMultipleDisks);
	else return FALSE;
}

BOOL RADInit::Set_ImportMultipleDisks(BOOL bImportMultipleDisks) {
	if (bParams_Loaded) {
        pParams_Current->bImportMultipleDisks = bImportMultipleDisks;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_ImportMultipleDisks(void) {
	return (Set_ImportMultipleDisks(pParams_Default->bImportMultipleDisks));
}

// Whether to put up a prompt before starting the import during /IMPORT
BOOL RADInit::Get_ImportDiskPrompt(void) {
	if (bParams_Loaded)
		return (pParams_Current->bImportDiskPrompt);
	else return FALSE;
}

BOOL RADInit::Set_ImportDiskPrompt(BOOL bImportDiskPrompt) {
	if (bParams_Loaded) {
        pParams_Current->bImportDiskPrompt = bImportDiskPrompt;
		return TRUE;
	}
	return FALSE;
}

BOOL RADInit::Reset_ImportDiskPrompt(void) {
	return (Set_ImportDiskPrompt(pParams_Default->bImportDiskPrompt));
}


// 11-Jul-2005 SFK	Add function for new INI parameters
//	Default the value for the upper limit of the R/T threshold
// Whether to put up a prompt before starting the import during /IMPORT
float RADInit::Get_RTUpperLimit(void) {
	if (bParams_Loaded)
		return (pParams_Current->fRTUpperLimit);
	else return 0.5;
}

BOOL RADInit::Set_RTUpperLimit(float fRTUpperLimit) {
	if (bParams_Loaded) {
        pParams_Current->fRTUpperLimit = fRTUpperLimit;
		return TRUE;
	}
	return FALSE;
}
 
BOOL RADInit::Reset_RTUpperLimit(void) {
	return (Set_RTUpperLimit(pParams_Default->fRTUpperLimit));
}


// 11-Jul-2005 SFK Handle the parameter that specified whether to display ratios or integral/area with chan events
// Whether to put up a prompt before starting the import during /IMPORT
BOOL RADInit::Get_EnableRatios(void) {
	if (bParams_Loaded)
		return (pParams_Current->bEnableRatios);
	else return FALSE;
}

BOOL RADInit::Set_EnableRatios(BOOL bEnableRatios) {
	if (bParams_Loaded) {
        pParams_Current->bEnableRatios = bEnableRatios;
		return TRUE;
	}
	return FALSE;
}
 BOOL RADInit::Reset_EnableRatios(void) {
	return (Set_EnableRatios(pParams_Default->bEnableRatios));
}



/////////////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////////// 
// Member Functions to actually write the RAD_PARAMS struct to the .ini file (if they have changed)
/////////////////////////////////////
          
BOOL RADInit::Save_Parameters(void) {

	BOOL bSuccess;


	// Be sure that the init worker is created and ability to save
	if ((!bParams_Loaded) ||
		(!bSave_Ability))
		return FALSE;


	// NOTE: Don't forget to change index numbers (Increase by one)
	// Save the parent parameters
	if (!RTInit::Save_Parameters()) {
		RadReviewMsg(uiINSPEC_INI_ERR);
		return FALSE;
	}

		
	// Save only the RAD Specific parameters that can be changed by the program
//	if (!Label_Change(szRAD_db_PATH, pParams_Current->pszdbPath))
//		return(FALSE);

	if (!Label_Change(szRAD_NCC_DATA_PATH, pParams_Current->pszNCC_DataPath))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_ALL_PATH, pParams_Current->pszImportAllPath))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_ALL_WILDCARD, pParams_Current->pszImportAllWildcard))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_ALL_OVERWRITE, pParams_Current->bImportAllOverwrite))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_ALL_INIT, pParams_Current->bImportAllInit))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_ALL_SUBFOLDERS, pParams_Current->bImportAllSubfolders))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_SELECT_PATH, pParams_Current->pszImportSelectPath))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_SELECT_WILDCARD,  pParams_Current->pszImportSelectWildcard))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_SELECT_OVERWRITE, pParams_Current->bImportSelectOverwrite))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_SELECT_INIT, pParams_Current->bImportSelectInit))
		return(FALSE);
		
	if (!Label_Change(szRAD_IMPORT_SELECT_SUBFOLDERS, pParams_Current->bImportSelectSubfolders))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_TODAY_PATH, pParams_Current->pszImportTodayPath))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_TODAY_WILDCARD, pParams_Current->pszImportTodayWildcard))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_TODAY_OVERWRITE, pParams_Current->bImportTodayOverwrite))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_TODAY_INIT, pParams_Current->bImportTodayInit))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_TODAY_SUBFOLDERS, pParams_Current->bImportTodaySubfolders))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_TODAY_SINCE_LAST, pParams_Current->bImportTodaySinceLast))
		return(FALSE);

	if (!Label_Change(szRAD_DB_BACKUP_PATH, pParams_Current->pszDbBackupPath))
		return(FALSE);

	if (!Label_Change(szRAD_DB_RESTORE_FROM_PATH, pParams_Current->pszDbRestoreFromPath))
		return(FALSE);

	if (!Label_Change(szRAD_CFG_BACKUP_PATH, pParams_Current->pszCfgBackupPath))
		return(FALSE);

	if (!Label_Change(szRAD_CFG_RESTORE_FROM_PATH, pParams_Current->pszCfgRestoreFromPath))
		return(FALSE);

	if (!Label_Change(szRAD_LOG_COPY_TO_PATH, pParams_Current->pszLogCopyToPath))
		return(FALSE);
    		
	if (!Label_Change(szRAD_REAL_TIME_UPDATE, pParams_Current->bRealTimeUpdate))
		return(FALSE);

	if (!Label_Change(szRAD_REANALYZE_ENABLED, pParams_Current->bReanalyzeEnabled))
		return(FALSE);

	if (!Label_Change(szRAD_LOCATION_MATCH_TYPE, pParams_Current->uiLocationMatchType))
		return(FALSE);

	if (!Label_Change(szRAD_LOCATION_MATCH_ITEM, pParams_Current->uiLocationMatchItem))
		return(FALSE);
	
	if (!Label_Change(szRAD_LOCATION_NO_MATCH_MSG, pParams_Current->bMessageIfNoLocationMatch))
		return(FALSE);

//	if (!Label_Change(szRAD_SEND_LOCATION, pParams_Current->bSendLocation))
//		return(FALSE);

	if (!Label_Change(szRAD_MEAS_AT_THRES, pParams_Current->fMeasAT_Threshold))
		return(FALSE);
	if (!Label_Change(szRAD_MEAS_AT_BIAS, pParams_Current->fMeasAT_Bias))
		return(FALSE);
	if (!Label_Change(szRAD_MEAS_AT_SIGMA, pParams_Current->fMeasAT_Sigma))
		return(FALSE);
	if (!Label_Change(szRAD_MEAS_GATE_WIDTH, pParams_Current->fMeasGateWidth))
		return(FALSE);
	if (!Label_Change(szRAD_MEAS_TERM_ON_MOVING, pParams_Current->bMeasTerminateOnMoving))
		return(FALSE);
	if (!Label_Change(szRAD_MEAS_INCLUDE_AT_FAIL, pParams_Current->bMeasInclude_AT_Fail))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_DISK_PROMPT, pParams_Current->bImportDiskPrompt))
		return(FALSE);

	if (!Label_Change(szRAD_IMPORT_MULTIPLE_DISKS, pParams_Current->bImportMultipleDisks))
		return(FALSE);

	if (!Label_Change(szRAD_WRITE_CHAN_EVENT_RESULTS_FILE, pParams_Current->bChanEventResults))
		return(FALSE);
		
	if (!Label_Change(szRAD_WRITE_DIR_EVENT_RESULTS_FILE, pParams_Current->bDirEventResults))
		return(FALSE);
	
	if (!Label_Change(szRAD_WRITE_MEASUREMENT_RESULTS_FILE, pParams_Current->uiMeasurementResults))
		return(FALSE);
					
	// 11-Jul-2005 SFK Added szRAD_R_T_UPPER_LIMIT and szRAD_ENABLE_RATIOS
	if (!Label_Change(szRAD_R_T_UPPER_LIMIT, pParams_Current->fRTUpperLimit))
		return(FALSE);

	if (!Label_Change(szRAD_ENABLE_RATIOS, pParams_Current->bEnableRatios))
		return(FALSE);

	bSuccess = RADInit_Load_Params(pParams_Default);
	
	SaveDatabaseNow();
                        
	return bSuccess;
}





/////////////////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////////////////// 
// Member Function to reset all RAD_PARAMS struct values to the values originally 
// read from the .ini file
/////////////////////////////////////////////////////////////////////////////////////////// 

BOOL RADInit::Reset_Parameters(void) {

	BOOL bSuccess = TRUE;
	
	bSuccess = bSuccess && RTInit::Reset_Parameters();
                                                       
 	bSuccess = bSuccess && Reset_Menu_Opt_Import_Today();
 	bSuccess = bSuccess && Reset_Menu_Opt_Determine_Measurements();
 	bSuccess = bSuccess && Reset_Menu_Opt_Direction_Analysis();
	bSuccess = bSuccess && Reset_Menu_Opt_Export_to_INCC();
	bSuccess = bSuccess && Reset_Menu_Opt_Measurements_Summary();
	bSuccess = bSuccess && Reset_Menu_Opt_Direction_Summary();

	bSuccess = bSuccess && Reset_WriteChanEventResults();
	bSuccess = bSuccess && Reset_WriteDirEventResults();
	bSuccess = bSuccess && Reset_WriteMeasurementResults();
	
// 	bSuccess = bSuccess && Reset_Facility_Names();
// 	bSuccess = bSuccess && Reset_RADExe_Path();
 	
	bSuccess = bSuccess && Reset_NCC_DataPath();
	bSuccess = bSuccess && Reset_Import_All_Path();
 	bSuccess = bSuccess && Reset_Import_All_Wildcard();
	bSuccess = bSuccess && Reset_Import_All_Overwrite();
	bSuccess = bSuccess && Reset_Import_All_Init();
	bSuccess = bSuccess && Reset_Import_All_Subfolders();
	
    bSuccess = bSuccess && Reset_Import_Select_Path();
	bSuccess = bSuccess && Reset_Import_Select_Wildcard();
 	bSuccess = bSuccess && Reset_Import_Select_Overwrite();
 	bSuccess = bSuccess && Reset_Import_Select_Init();
	bSuccess = bSuccess && Reset_Import_Select_Subfolders();
 	
 	bSuccess = bSuccess && Reset_Import_Today_Path();
	bSuccess = bSuccess && Reset_Import_Today_Wildcard();
 	bSuccess = bSuccess && Reset_Import_Today_Overwrite();
	bSuccess = bSuccess && Reset_Import_Today_Subfolders();
	bSuccess = bSuccess && Reset_Import_Today_Since_Last();
	
	bSuccess = bSuccess && Reset_dbBackup_Path();
 	bSuccess = bSuccess && Reset_dbRestore_From_Path();
	bSuccess = bSuccess && Reset_CfgBackup_Path();
 	bSuccess = bSuccess && Reset_CfgRestore_From_Path();
	bSuccess = bSuccess && Reset_LogCopy_To_Path();

	bSuccess = bSuccess && Reset_ImportDiskPrompt();
	bSuccess = bSuccess && Reset_ImportMultipleDisks();

	// 11-Jul-2005 SFK Added Reset_RTUpperLimit and Reset_EnableRatios
	bSuccess = bSuccess && Reset_RTUpperLimit();
	bSuccess = bSuccess && Reset_EnableRatios();

	return bSuccess;
}





/////////////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////////// 
// Member Function to delete any strings that were created for the RAD_PARAMS struct
/////////////////////////////////////////////////////////////////////////////////////////// 

void RADInit::RADInit_Delete_Params(RAD_PARAMS *pParams) {

//	if (pParams->pszNamesCfg)
//		delete [] pParams->pszNamesCfg;

//	if (pParams->pszRADExePath)
//		delete [] pParams->pszRADExePath;

	if (pParams->pszNCC_DataPath)
		delete [] pParams->pszNCC_DataPath;

	if (pParams->pszImportAllPath)
		delete [] pParams->pszImportAllPath;

	if (pParams->pszImportAllWildcard)
		delete [] pParams->pszImportAllWildcard;

	if (pParams->pszImportSelectPath)
		delete [] pParams->pszImportSelectPath;

	if (pParams->pszImportSelectWildcard)
		delete [] pParams->pszImportSelectWildcard;

	if (pParams->pszImportTodayPath)
		delete [] pParams->pszImportTodayPath;

	if (pParams->pszImportTodayWildcard)
		delete [] pParams->pszImportTodayWildcard;

	if (pParams->pszDbBackupPath)
		delete [] pParams->pszDbBackupPath;

	if (pParams->pszDbRestoreFromPath)
		delete [] pParams->pszDbRestoreFromPath;

	if (pParams->pszCfgBackupPath)
		delete [] pParams->pszCfgBackupPath;

	if (pParams->pszCfgRestoreFromPath)
		delete [] pParams->pszCfgRestoreFromPath;

	if (pParams->pszLogCopyToPath)
		delete [] pParams->pszLogCopyToPath;
}
