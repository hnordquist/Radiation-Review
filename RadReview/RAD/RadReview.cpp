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
//    File Name : RadReview.cpp     
//Primary Author: Shirley Klosterbuer/Steve Buck

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////
// Review class definition for the Radiation Review review operations


// 27-Mar-2001	SFK	Version 2.07 Added option to enable/disable Tools|Data Integrity menu option       
       
#include "RAD.H"
#include "AdjustTime.h"
#include "DateTimeUtils.h"
#include "DbGeneralDbSupport.h"
#include "DlgBackupDb.h"
#include "DlgBackupCfg.h"
//#include "DlgClearDb.h"
#include "DlgCopyLogs.h"
#include "DlgDaysSummary.h"
#include "DlgDirection.h"
#include "DlgDirectionSummary.h"
#include "DlgEventSummary.h"
#include "DlgExport.h"
#include "DlgFindEvent.h"
#include "DlgFindMeas.h"
#include "DlgImportAll.h"
#include "DlgImportSome.h"
#include "DlgImportLatest.h"
#include "DlgIntegrity.h"
#include "DlgMarkSummary.h"
#include "DlgMeasSummary.h"
#include "DlgRawData.h"
#include "DlgRestoreDb.h"
#include "DlgRestoreCfg.h"
#include "DlgReviewPeriod.h"
#include "DlgSelectFacility.h"
#include "FacilityConfig.h"
#include "Plot.h"
#include "RadInit.H"
#include "RadMain.h"
#include "RadReview.H"
#include "Update.h"
#include "Utilities.h"
#include "FacilityConfig.h" //KM
#include "MyDateTime.h"


#define MAX_BUTTONS 9		// 20-Sep-2005 SFK Changed from 5 to 9 max tools

extern RADInit *pRAD_Init;
extern RADReview *pRAD_Review;
extern short glsFacNum;
extern CGUI_Dlg *pglShutDownDlg;
extern CMyDateTime glMyDateTime;

extern bool glbFacilityPicklistFilled;
extern bool glbStationPicklistFilled;
extern bool glbChannelPicklistFilled;

                                 
static BOOL RadReviewFileExitFunc();
static char gllszTimestamp[uiTIME_STAMP_LENGTH+1];                             
static int glliButtonIDs[MAX_BUTTONS] = {IDC_BUTTON0, IDC_BUTTON1, IDC_BUTTON2, IDC_BUTTON3, IDC_BUTTON4,
										 IDC_BUTTON5, IDC_BUTTON6, IDC_BUTTON7, IDC_BUTTON8}; // 20-Sep-2005 Added buttons
static IDStruct gllChanID;



/////////////////////////////////////////////////////////////////////////////////////
//  RADReview constructor, destructor
/////////////////////////////////////////////////////////////////////////////////////

RADReview::RADReview(void) :
	bMayClose(FALSE),
	bRealTimeUpdate(FALSE),
	mpToolNums(0),
	miTools(0),
	mpRAD_Review_Choices(NULL),
	mpRAD_About_Rad_Review(NULL),
	mpRAD_Product_Support(NULL)
{

}

 
RADReview::~RADReview(void) {
	//return;	//sk 04/15/2003
	if (mpRAD_Review_Choices) delete mpRAD_Review_Choices;
	if (mpRAD_About_Rad_Review) delete mpRAD_About_Rad_Review;
	if (mpRAD_Product_Support) delete mpRAD_Product_Support;
	// Try to delete to be sure memory freed
	//RAD_Review_RAD_Delete();	//sk 04/15/2003
	//if (mpMainMenu) delete mpMainMenu;  // can't delete or get exception during app destruction so get a memory leak
}


/////////////////////////////////////////////////////////////////////
// Access to the common data items across all review tool operations
/////////////////////////////////////////////////////////////////////
BOOL RADReview::Set_Common_Params(RTInit *const pRT_Common_Params) {
    
    RADReview::pRadParms = (RADInit *)pRT_Common_Params;
    return RTReview::Set_Common_Params(pRadParms);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
// Required through RTReview to provide the Review Begin procedure which is specific to Operator Review
////////////////////////////////////////////////////////////////////////////////////////////////////////

RTReview::REVIEW_OPERATION RADReview::RT_Review_Begin(void) 
{

	REVIEW_OPERATION ret = RTReview::REVIEW_EXIT;

	Set_Messages_On_Hold(TRUE);		// disable communication until ready
	RADReviewMakeMainMenu();		// display my main menu

    CGUI_App::RegisterMayTerminateFn(RadReviewFileExitFunc); // will be executed with do File/Exit from Menu

	Set_Application(pMy_Application);
	
	Set_Messages_On_Hold(FALSE);	// enable communication now

	try
	{
		pMy_Application->Go();
	}
	catch(...)
	{
		ret = RTReview::REVIEW_TITLE;
	}
	
	try
	{
		// Free the memory	
		RAD_Review_RAD_Delete();
	}
	catch(...)
	{

	}

	return ret;
}


void RADReview::RADReviewSetMainMenuOptions(void)
{
	// Disable some options based on the information in the rad.ini file
	if (pRAD_Init->Get_Menu_Opt_Import_Selected() != TRUE) {
		mpMainMenu->SetEnabled(ID_IMPORT_SELECTED, FALSE);
	}
	else {
		mpMainMenu->SetEnabled(ID_IMPORT_SELECTED, TRUE);
	}

	if (pRAD_Init->Get_Menu_Opt_Import_Today() != TRUE) {
		mpMainMenu->SetEnabled(ID_IMPORT_TODAY, FALSE);
	}
	else {
		mpMainMenu->SetEnabled(ID_IMPORT_TODAY, TRUE);
	}

	if (pRAD_Init->Get_Menu_Opt_Determine_Measurements() != TRUE) {
		mpMainMenu->SetEnabled(ID_TOOLS_DETERMINE_MEASUREMENTS, FALSE);
	}
	else {
		mpMainMenu->SetEnabled(ID_TOOLS_DETERMINE_MEASUREMENTS, TRUE);
	}

	if (pRAD_Init->Get_Menu_Opt_Direction_Analysis() != TRUE) {
		mpMainMenu->SetEnabled(ID_TOOLS_DIRECTION_ANALYSIS, FALSE);
	}
	else {
		mpMainMenu->SetEnabled(ID_TOOLS_DIRECTION_ANALYSIS, TRUE);
	}

	if (pRAD_Init->Get_Menu_Opt_Export_to_INCC() != TRUE) {
		mpMainMenu->SetEnabled(ID_TOOLS_EXPORT_TO_INCC, FALSE);
	}	
	else {
		mpMainMenu->SetEnabled(ID_TOOLS_EXPORT_TO_INCC, TRUE);
	}
	if (pRAD_Init->Get_Menu_Opt_Measurements_Summary() != TRUE) {
		mpMainMenu->SetEnabled(ID_SUMMARIES_MEASUREMENTS, FALSE);
	}
	else {
		mpMainMenu->SetEnabled(ID_SUMMARIES_MEASUREMENTS, TRUE);
	}

	if (pRAD_Init->Get_Menu_Opt_Direction_Summary() != TRUE) {
		mpMainMenu->SetEnabled(ID_SUMMARIES_DIRECTION_EVENTS, FALSE);
	}
	else {
		mpMainMenu->SetEnabled(ID_SUMMARIES_DIRECTION_EVENTS, TRUE);
	}
	
	if (pRAD_Init->Get_Menu_Opt_Data_Integrity() != TRUE) {
		mpMainMenu->SetEnabled(ID_TOOLS_DATAINTEGRITYCHECKS, FALSE);
	}
	else {
		mpMainMenu->SetEnabled(ID_TOOLS_DATAINTEGRITYCHECKS, TRUE);
	}
	// set the realtime update parameter to be used by graph
	bRealTimeUpdate = pRAD_Init->Get_RealTimeUpdate();

}

// Import All
void RADReview::Protected_ImportAllMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	GetCurrentDrive();
	ImportAllMenuOption();
	SetCurrentDrive();
	Set_Messages_On_Hold(FALSE);
}
// Import Selected
void RADReview::Protected_ImportSomeMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	GetCurrentDrive();
	ImportSomeMenuOption();
	SetCurrentDrive();
	Set_Messages_On_Hold(FALSE);
}
//Import Most Recent Files
void RADReview::Protected_ImportTodayMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	GetCurrentDrive();
	ImportTodayMenuOption();
	SetCurrentDrive();
	Set_Messages_On_Hold(FALSE);
}

// Select Facility
void RADReview::Protected_SelectFacilityMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	SelectFacilityMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Data Integrity
void RADReview::Protected_ReviewPeriodMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	ReviewPeriodMenuOption(FALSE);
	Set_Messages_On_Hold(FALSE);
}
// Data Integrity
void RADReview::Protected_DataIntegrityMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	DataIntegrityMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Find Events
void RADReview::Protected_FindEventsMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	FindEventsMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Direction Analysis
void RADReview::Protected_DirectionAnalysisMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	DirectionAnalysisMenuOption();
	Set_Messages_On_Hold(FALSE);
}

// Station Timestamp
void RADReview::Protected_AdjustStationTimeMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	AdjustStationTimeMenuOption();
	Set_Messages_On_Hold(FALSE);
}

// Determine Measurements
void RADReview::Protected_DetermineMeasurementsMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	DetermineMeasurementsMenuOption();
	Set_Messages_On_Hold(FALSE);
}

// Export Files to NCC
void RADReview::Protected_ExportToINCCMenuOption()
{
	BOOL bReturn;
	Set_Messages_On_Hold(TRUE);
	bReturn = ExportToINCCMenuOption();
	if ((bReturn) && (pRAD_Init->Get_ReanalyzeEnabled() == TRUE)) {
		RTReview::RT_Reanalyze_All_Data_Initiate();	// this will put up error if fails, RAD doesn't care.
	}
	Set_Messages_On_Hold(FALSE);
}

// Show the Facility Config Interface 
void RADReview::Protected_FacilityConfigMenuOption()  //km
{	
	//Change the cursor to the busy cursor while the facility manager dialog loads.
	HCURSOR OldCur = SetCursor(LoadCursor(NULL, IDC_WAIT));

	Set_Messages_On_Hold(TRUE); //km
	pglFacCfg->ShowFacilityConfigDlg(); //km
	Set_Messages_On_Hold(FALSE); //km

	//Return cursor to normal non-busy cursor.
	SetCursor(OldCur);

	glbFacilityPicklistFilled = false;
	glbStationPicklistFilled = false;
	glbChannelPicklistFilled = false;
	// 16-Nov-2004 SFK 3.0.3.16 open and close everything one more time to make sure all consistent
	char szDbName[DRIVE_LEN+PATH_LEN];
	pglFacCfg->GetFacilityDirectory(glsFacNum, szDbName, DRIVE_LEN+PATH_LEN-1);
	SwitchDatabases(szDbName, glsFacNum, false);
	RadReviewMsg(uiFAC_MGR_CHANGE_INFO);

}
void RADReview::Protected_SensorManagerMenuOption()  //km
{
	//Change the cursor to the busy cursor while the facility manager dialog loads.
	HCURSOR OldCur = SetCursor(LoadCursor(NULL, IDC_WAIT));

	Set_Messages_On_Hold(TRUE); //km
	pglFacCfg->ShowSensorManagerDlg(); //km
	Set_Messages_On_Hold(FALSE); //km

	//Return cursor to normal non-busy cursor.
	SetCursor(OldCur);
}
void RADReview::Protected_UnitsManagerMenuOption()  //km
{
	//Change the cursor to the busy cursor while the facility manager dialog loads.
	HCURSOR OldCur = SetCursor(LoadCursor(NULL, IDC_WAIT));

	Set_Messages_On_Hold(TRUE); //km
	pglFacCfg->ShowUnitsManagerDlg(); //km
	Set_Messages_On_Hold(FALSE); //km

	//Return cursor to normal non-busy cursor.
	SetCursor(OldCur);
}

// Marks Summ
void RADReview::Protected_RadMarksSummariesMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	RadMarksSummariesMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Events Summ
void RADReview::Protected_EventsSummariesMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	EventsSummariesMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Measurements Summ
void RADReview::Protected_MeasurementSummariesMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	MeasurementSummariesMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Direction Summ
void RADReview::Protected_DirectionSummariesMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	DirectionSummariesMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Db Days
void RADReview::Protected_DbDaysMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	DbDaysMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Select All
/*KM  - removed per direction from Shirley Klosterbuer
void RADReview::Protected_SelectAllMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	SelectAllMenuOption();
	Set_Messages_On_Hold(FALSE);
}
*/
// Raw Data
void RADReview::Protected_RawDataMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	RawDataMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Select DB
//void RADReview::Protected_SelectDbMenuOption()
//{
//	Set_Messages_On_Hold(TRUE);
//	SelectDbMenuOption();
//	Set_Messages_On_Hold(FALSE);
//}
// Backup DB
void RADReview::Protected_BackupDbMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	BackupDbMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Restore DB
void RADReview::Protected_RestoreDbMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	RestoreDbMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Backup Cfg
void RADReview::Protected_BackupCfgMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	BackupCfgMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Restore Cfg
void RADReview::Protected_RestoreCfgMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	RestoreCfgMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Copy Logs
void RADReview::Protected_CopyLogsMenuOption()
{
	Set_Messages_On_Hold(TRUE);
	CopyLogsMenuOption();
	Set_Messages_On_Hold(FALSE);
}
// Clear DB
//void RADReview::Protected_ClearDbMenuOption()
//{
//	Set_Messages_On_Hold(TRUE);
//	GetCurrentDrive();
//	ClearDbMenuOption();
//	SetCurrentDrive();
//	Set_Messages_On_Hold(FALSE);
//}


void RADReview::RADReviewMakeMainMenu(void)
{

	mpMainMenu = new CGUI_Menu(pMy_Application, IDR_MAIN_MENU);

	// Create an About Box based on SEB boxes format
	mpRAD_About_Rad_Review = new SEB_About_Box(pRAD_Init->Get_My_Name(),
											   szTOOL_VERSION,
											   szREVIEW_TOOL_VERSION,
											   pRAD_Init->Get_Facility_Name(),
											   pRAD_Init->Get_Facility_Location(),
											   IDI_NEW_RAD_REVIEW);
	if (mpRAD_About_Rad_Review == NULL) {
	   	RTReview::RT_Review_Error(uiRT_REVIEW_ERROR_NO_MEMORY);
		return;
	}

	// Create a Product Support Box based on SEB boxes format
	mpRAD_Product_Support = new SEB_Product_Support_Box(pCommon_Params->Get_My_Name(), szCONTACT_NAME, szCONTACT_EMAIL, IDI_NEW_RAD_REVIEW);
	if (mpRAD_Product_Support == NULL) {
	   	RTReview::RT_Review_Error(uiRT_REVIEW_ERROR_NO_MEMORY);
		return;
	}

	// Create all the menu item entries.
	mpMainMenu->SetUpMenuItem(ID_FILE_PRINTERSETUP, GUI_PRINTER_SETUP);
	mpMainMenu->SetUpMenuItem(ID_FILE_BACKUP_DB,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_BackupDbMenuOption, "Backup current database to another directory.");
	mpMainMenu->SetUpMenuItem(ID_FILE_RESTORE_DB,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_RestoreDbMenuOption, "Restore database from previous backup.");
	mpMainMenu->SetUpMenuItem(ID_FILE_BACKUP_CFG,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_BackupCfgMenuOption, "Backup current configuration to another directory.");
	mpMainMenu->SetUpMenuItem(ID_FILE_RESTORE_CFG,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_RestoreCfgMenuOption, "Restore configuration from previous backup.");
	mpMainMenu->SetUpMenuItem(ID_FILE_COPY_LOGS,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_CopyLogsMenuOption, "Copy logs.");
	mpMainMenu->SetUpMenuItem(ID_FILE_EXIT, GUI_TERMINATE_APP); // want to do a special thing when we exit
	
	mpMainMenu->SetUpMenuItem(ID_IMPORT_ALL,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_ImportAllMenuOption, "Import all files in the specified directories into database.");
	mpMainMenu->SetUpMenuItem(ID_IMPORT_SELECTED,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_ImportSomeMenuOption, "Import selected files into database.");
	mpMainMenu->SetUpMenuItem(ID_IMPORT_TODAY,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_ImportTodayMenuOption, "Import most recent files in the specified directories into database.");

	mpMainMenu->SetUpMenuItem(ID_TOOLS_REVIEWPERIOD,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_ReviewPeriodMenuOption, "Set time interval to review.");
	mpMainMenu->SetUpMenuItem(ID_TOOLS_SELECTFACILITY,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_SelectFacilityMenuOption, "Select one facility in the database to analyze.");
	mpMainMenu->SetUpMenuItem(ID_TOOLS_DATAINTEGRITYCHECKS,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_DataIntegrityMenuOption, "Check database for integrity.");
	mpMainMenu->SetUpMenuItem(ID_TOOLS_FINDEVENTS,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_FindEventsMenuOption, "Automatically search database for events.");
	mpMainMenu->SetUpMenuItem(ID_TOOLS_DETERMINE_MEASUREMENTS,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_DetermineMeasurementsMenuOption, "Find measurements in SR data.");
	mpMainMenu->SetUpMenuItem(ID_TOOLS_GRAPH, GraphData, "Graphically view counts vs. time.");	
	mpMainMenu->SetUpMenuItem(ID_TOOLS_ADJUSTSTATIONTIMESTAMP,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_AdjustStationTimeMenuOption, "Adjust timestamp of station.");
	mpMainMenu->SetUpMenuItem(ID_TOOLS_DIRECTION_ANALYSIS,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_DirectionAnalysisMenuOption, "Analyze events for direction.");
	mpMainMenu->SetUpMenuItem(ID_TOOLS_EXPORT_TO_INCC,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_ExportToINCCMenuOption, "Specify files for NCC analysis.");
	mpMainMenu->SetUpMenuItem(ID_TOOLS_FACILITYCONFIGURATION,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_FacilityConfigMenuOption, "Configure the facility for Radiation Review"); //km
//	mpMainMenu->SetUpMenuItem(ID_TOOLS_SENSORMANAGER,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_SensorManagerMenuOption, "Define sensors and associated channels"); //km
//	mpMainMenu->SetUpMenuItem(ID_TOOLS_UNITSMANAGER,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_UnitsManagerMenuOption, "Define units and unit abbreviations"); //km

	mpMainMenu->SetUpMenuItem(ID_SUMMARIES_MARKS,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_RadMarksSummariesMenuOption, "Display marks contained in database.");
	mpMainMenu->SetUpMenuItem(ID_SUMMARIES_EVENTS,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_EventsSummariesMenuOption, "Display channel events contained in database.");
	mpMainMenu->SetUpMenuItem(ID_SUMMARIES_MEASUREMENTS,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_MeasurementSummariesMenuOption, "Display measurements contained in database.");
	mpMainMenu->SetUpMenuItem(ID_SUMMARIES_DIRECTION_EVENTS,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_DirectionSummariesMenuOption, "Display direction events contained in database.");
	mpMainMenu->SetUpMenuItem(ID_SUMMARIES_DAYS,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_DbDaysMenuOption, "Display days contained in database.");
	mpMainMenu->SetEnabled(ID_SUMMARIES_DAYS, FALSE);  //PJM April 9, 2008
	
	mpMainMenu->SetUpMenuItem(ID_DIAGNOSTICS_RAWDATA,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_RawDataMenuOption, "Display raw data contained in database.");
	//mpMainMenu->SetUpMenuItem(ID_UPDATE, MenuAutomaticUpdate, "Set default options for automatic update procdure.");
	//mpMainMenu->SetEnabled(ID_UPDATE, FALSE);
	                                                                                                                    
	//mpMainMenu->SetUpMenuItem(ID_DATABASE_SELECT,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_SelectDbMenuOption, "Close the current database and open a different one.");
	//mpMainMenu->SetUpMenuItem(ID_DATABASE_CLEAR,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::Protected_ClearDbMenuOption, "Delete all data in the current database.");
	//mpMainMenu->SetUpMenuItem(ID_DATABASE_DELETEDAYS, NULL, "Delete specified days in the current database."); 
	//mpMainMenu->SetEnabled(ID_DATABASE_DELETEDAYS, FALSE);  // not implemented yet
	                                                
 	mpMainMenu->SetUpMenuItem(ID_WINDOW_TILE, GUI_MDI_TILE);
	mpMainMenu->SetUpMenuItem(ID_WINDOW_CASCADE1, GUI_MDI_CASCADE);
	mpMainMenu->SetUpMenuItem(ID_WINDOW_ARRANGEICONS, GUI_MDI_ARRANGE_ICONS);
	
	mpMainMenu->SetUpMenuItem(ID_HELP_PRODUCTSUPPORT,(CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::RAD_Menu_Help_ProductSupport);
	mpMainMenu->SetUpMenuItem(ID_HELP_ABOUT, (CGUI_CallBack *)((void *)this), (GUI_CB_Fn)&RADReview::RAD_Menu_Help_About);
	
	//	Determine which tools, if any, are installed.
	const char *pConstStr;
	unsigned int uiNumTools, i, j;
	
	uiNumTools = pRAD_Init->Get_Tools_Num();
	mpToolNums = (int *)malloc(uiNumTools*sizeof(int)); // array that will keep track of the tool number
	j=0;
	for (i=0; i<uiNumTools; i++) {
		if ((pRAD_Init->Get_Tool_Installed(i) == TRUE) &&	// tool is installed
			(pRAD_Init->Get_Data_Match_Tool(i) == TRUE)) {	// can match this tool's data
			pConstStr = pRAD_Init->Get_Tool_Exec_Name(i);
			mpToolNums[j] = i;	// this is the tool number
			miTools++;		// count this tool 
		}
		j++;
	}	

	RADReviewSetMainMenuOptions();		// enable/disable menu options as needed
}

  

void RADReview::RAD_Review_RAD_Delete(void) {

	if (mpToolNums) {	// array that keeps track of tool numbers
		free (mpToolNums);
		mpToolNums = NULL;
	}

	if (mpRAD_About_Rad_Review) {	// SEB about box
		delete mpRAD_About_Rad_Review;
		mpRAD_About_Rad_Review = NULL;
	}

	if (mpRAD_Product_Support) {	// SEB product support box
		delete mpRAD_Product_Support;
		mpRAD_Product_Support = NULL;
	}
}
		



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Required through RTReview to provide the member functions which display associated data specific to Radiation Review
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
// Rad's own action to take when RT_Display_My_Data is called.
// 12-Aug-2002	Changed to DATE 
// 29-Aug-2002	SFK	Patch where assume channel number is always offset plus 1
BOOL RADReview::RT_Display_My_Data(const char *pszTool_Name, const char *pszTimestamp, const char *pszLocation, BOOL *pbLocation_Failed)
{
	DATE dDisplayStartTime;
	DATE dViewPeriodInDATE;
	DATE dBegTime;
//	int status;
//	int i;
	BOOL bOkay;
	struct db_sta_rec dbSta;
	struct IDStruct ChanID;
	CGFmtStr msg;

	Set_Messages_On_Hold(TRUE);		// disable any other messages
	SetCurrentDrive();
    
	*pbLocation_Failed = FALSE;
	// check for a NULL string in the location field
	// 23-Oct-98 Use NULL as indicator that there is nothing to match, this is not an error
	//if (pszLocation == NULL) {
	//	*pbLocation_Failed = TRUE;
	//}

	// display graph associated with timestamp

	//DATE dTimeStamp = Date.StevesTimestampStringToDATETimestamp(pszTimestamp);
	DATE dTimeStamp = glMyDateTime.StevesTimestampStringToDATETimestamp(pszTimestamp);

	if (!GraphDisplayedNow())
	   	GraphData();

	// want to position the cursor around the timestamp
	// if the timestamp time is currectly on the screen, just position the cursor.
	// if the timestamp is off the screen, then set the graph viewperiod to be centered around the new timestamp
	if (dTimeStamp != 0.0) {
		GraphGetViewPeriodParameters(&dDisplayStartTime, &dViewPeriodInDATE);
		if ((dTimeStamp < dDisplayStartTime) || (dTimeStamp > (dDisplayStartTime + dViewPeriodInDATE))) {
			dBegTime = dTimeStamp - dViewPeriodInDATE/2;   	
			GraphSetDisplayDate(dBegTime, dViewPeriodInDATE);
		}

		if (pszLocation != NULL) {	// NULL means no location sent
			// Got a location string from the other review tool.
			// Check first to see whether Rad is to match to facility, station or channel
			*pbLocation_Failed = TRUE;	

			// match to facility
			if (pRAD_Init->Get_LocationMatchItem() == FACILITY_MATCH) {
				if (pRAD_Init->Get_LocationMatchType() == MATCH_STRING) {	// verify want to match by name
					short sFacID = pglFacCfg->GetFacilityID((char *)pszLocation);
					if (sFacID > 0) {	// facility is known to fac mgr
						// facility is in the database, is it the current default facility?
						if (sFacID != glsFacNum) {
							RadReviewModelessMsg(uiREQUESTED_FACILTY_NOT_CURRENT_FACILTY, (char *)pszLocation);
						}
						else {
							*pbLocation_Failed = FALSE;			// found matching location
						}
					}
					else {
						RadReviewModelessMsg(uiFACILITY_NOT_DEFINED_ERR, (char *)pszLocation);
					}
				}
				else {														// match to number not supported now
					RadReviewModelessMsg(uiNO_LOCATION_NUM_MATCH_SUPPORTED_ERR, (char *)pszLocation);
				}
			}

			//if ((pRAD_Init->Get_LocationMatchItem() != NO_SEND_LOCATION_MATCH)  && (pszLocation != NULL)){
			// If the location match is by facility, verify facility and then it always works since at least 1 trace is always visible.
			
			// If the location match is by station, then check through the channels for the station and see if any match
			// match to station
			if (pRAD_Init->Get_LocationMatchItem() == STATION_MATCH) {	
				bOkay = FALSE;
				if (pRAD_Init->Get_LocationMatchType() == MATCH_STRING) {	// verify want to match by name
					if (!pglFacCfg->GetStationID(glsFacNum, (char *)pszLocation, &ChanID)) {
						RadReviewModelessMsg(uiSTA_NOT_IN_DB_ERR, (char *)pszLocation);
						return(FALSE);	// is the station in the db? //KM
					}
	
					// station is in database, see if there is a channel from this station currently being display,
					// if yes, place the cursor on the first trace found from the station
					int iPicklistChoice[MAX_NUM_DATA_CHANS+1];
					int iNum;
					if (pglFacCfg->GetPicklistChoicesForStation(ChanID, &iNum, iPicklistChoice)) {
						for (int i=0; i<iNum; i++) {
							bOkay = GraphSetCursorTime(dTimeStamp, iPicklistChoice[i]);
							if (bOkay) {
								*pbLocation_Failed = FALSE;			// found matching location
								break;
							}
						}
					}

					// If requested, display an error message if there is no channel from the matching station displayed
					if ((!bOkay) && (pRAD_Init->Get_MessageIfNoLocationMatch() == TRUE)) {
						if (!pglFacCfg->GetStationRecord(glsFacNum, &ChanID, &dbSta)) return(FALSE);
						RadReviewModelessMsg(uiNO_ASSOCIATED_GRAPH_TRACE_ERR, dbSta.sz_sta_name);
					}
				}
				else {
					RadReviewModelessMsg(uiNO_LOCATION_NUM_MATCH_SUPPORTED_ERR, (char *)pszLocation);	// match to number not supported now
				}
			}

			// Channel matches cannot be supported since multiple stations can have the same channel name.
			if (pRAD_Init->Get_LocationMatchItem() == CHANNEL_MATCH) {	
				RadReviewModelessMsg(uiNO_CHANNEL_STRING_MATCH_SUPPORTED_ERR, (char *)pszLocation);	// match to channel name supported now
			}
			/*		status = FALSE;

				bOkay = FALSE;
				if (pRAD_Init->Get_LocationMatchType() == MATCH_STRING) {	// verify want to match by name
					if (!pglFacCfg->GetChannelID(glsFacNum, (char *)pszLocation, &ChanID)) {
						RadReviewMsg(uiCHAN_NOT_IN_DB_ERR, (char *)pszLocation,);
						return(FALSE);	
					}
	
					// channel is in database, see if there is a channel from this station currently being display,
					// if yes, place the cursor on the first trace found from the station
					short sPicklistChoice;
					for (i=MIN_CHAN_OFFSET; i<=MAX_CHAN_OFFSET; i++) {
							if (!pglFacCfg->GetPicklistChoiceForChannelID(ChanID, &sPicklistChoice)) return(FALSE);
							bOkay = GraphSetCursorTime(dTimeStamp, sPicklistChoice);
							if (bOkay) break;	// found a channel from station on the graph, are finished.
						}
					}
					// If requested, display an error message if there is no channel from the matching station displayedAdd check if want to display the error msg
					if ((!bOkay) && (pRAD_Init->Get_MessageIfNoLocationMatch() == TRUE)) {
						if (!pglFacCfg->GetStationRecord(glsFacNum, &ChanID, &dbSta)) return(FALSE);
						RadReviewMsg(uiNO_ASSOCIATED_GRAPH_TRACE_ERR, dbSta.sz_sta_name);
					}
					if (bOkay) *pbLocation_Failed = FALSE;			// found matching location
				}
				else {
					RadReviewMsg(uiNO_LOCATION_NUM_MATCH_SUPPORTED_ERR);	// match to number not supported now
					status = FALSE;
				}
			}
			// If the location match is by channel, then use results of GraphSetCursorTime to determine whether or not worked.
			//bOkay = FALSE;
			//status = ReadChanRec(glsFacNum, pszLocation, &dbChan);	// is the station in the db?
			//if (status == TRUE) {
			//	bOkay = GraphSetCursorTime(ulTimeStamp, GetPlNumForChan(dbChan.s_chan_num_key));
			//	// Add check if want to display the error msg
			//	if (!bOkay) RadReviewMsg(uiNO_ASSOCIATED_GRAPH_TRACE, dbChan.sz_chan_name);
			//}*/
		}
		else {	// no location was specified so use last channel active on graph
			bOkay = GraphSetCursorTime(dTimeStamp, -1);
		}
	}

	Set_Messages_On_Hold(FALSE);	// enable other messages to come in.

    if (*pbLocation_Failed == TRUE) return FALSE;
	if (dTimeStamp == 0.0) return FALSE;
	return TRUE;

}


                
/////////////////////////////////////////////////////////////////////////////////////
//  Procedures used to display other's data
/////////////////////////////////////////////////////////////////////////////////////

// This function is called from the graph when the user chooses to close the graph with the Pipe
// choices Modeless dialog box up.
void RADReview::RAD_Review_CloseMatchDataChoices(void)
{                        
	if (mpRAD_Review_Choices != NULL) mpRAD_Review_Choices->Close();	// Causes GraphExitNotifyFn to be called.
}


// This function is called when the user selects to close the modeless dialog box showing
// the choices of which tools can have their data matched.  The dialog box is exited when
// the user selects Cancel or when any button is selected or when Close is called on the dialog
// box from RAD_Review_CloseMatchDataChoices
BOOL RADReview::ChoicesExitNotifyFn(CGUI_Dlg *pDlg, GUI_ACTION Action)
{   
	BOOL bCanClose = TRUE;
	mpRAD_Review_Choices = NULL;	// signal to ourselves that the modeless dialog box is being deleted
	return(bCanClose);
}




//  This function is called from plot.cpp.  It is used to request another tool to match to 
//	the current time of the graph cursor.
//	miTools:  how many tools are known to the upper layer
// 04-29-2004	SFK	Added check for whether tool is installed before try to match data
void RADReview::RAD_MatchDataForChannel(const char *pTimeDateStr, struct IDStruct ChanID)
{
	CGFmtStr Msg;                        
	int i, j;
	struct db_sta_rec dbSta;
	
	if (miTools == 0) return;		// no tools to interact with

	gllChanID.sChanID = ChanID.sChanID;
	gllChanID.sStaID = ChanID.sStaID;
	gllChanID.sStaType = ChanID.sStaType;

   
	//	If only one tool is available to match data to,	do not put up the
	//	button choice dialog box, just go immediately to that tool
	// ****** debug this when you get here as don't know format of pTimeDateStr without running
	if (miTools == 1) { // don't need to choose which tool
		int idbChan = 0;
 		ConvertGenStrToTimestampStr(pTimeDateStr, idbChan, gllszTimestamp);	     
		for (i=0; i<MAX_BUTTONS; i++) {
			if ((pRAD_Init->Get_Data_Match_Tool(i) == TRUE) && (pRAD_Init->Get_Tool_Installed(i) == TRUE)) {	// can match this tool's data and it is installed
				if (!pglFacCfg->GetStationRecord(glsFacNum, &ChanID, &dbSta))return;
				RTReview::RT_Display_Other_Data(pRAD_Init->Get_Tool_Name(mpToolNums[i]), gllszTimestamp, dbSta.sz_sta_name); 
				break;
			}
		}
		return;
	}	 		
		

	//	Have more than one tool to match to.  Put up the dialog box of choices.
 	if (miTools > 1) {
		if (mpRAD_Review_Choices != NULL) return;  // a dialog box is already up, don't put up another one

		int idbChan = 0;
 		ConvertGenStrToTimestampStr(pTimeDateStr, idbChan, gllszTimestamp);	// this is the time to match
 	
		//	Define a dialog box where they choose which tool to match to
	    mpRAD_Review_Choices = new CGUI_Dlg (IDD_PIPE_CHOICE, NULL);
	    if ((mpRAD_Review_Choices!= NULL) && (mpRAD_Review_Choices->IsValid())) { 


			//	Define the buttons and actions in the dialog box.
			//	Right now have 5 buttons max because of UL
		    //	Check to see which tool nums have the ability to match their data.
			//	If they can match their data, then put up a button to identify them.
		    
			mpRAD_Review_Choices->DefinePushBtn(IDCANCEL, GUI_CANCEL);

			j = 0;
			for (i=0; i<MAX_BUTTONS; i++) {
				mpRAD_Review_Choices->DefinePushBtn(glliButtonIDs[j], GUI_CANCEL);		//define it, GUI_CANCEL deletes dlg box
				mpRAD_Review_Choices->SetVisible(glliButtonIDs[i], FALSE);			// hide it
				if ((pRAD_Init->Get_Data_Match_Tool(i) == TRUE) && (pRAD_Init->Get_Tool_Installed(i) == TRUE)) {	// can match this tool's data
					mpRAD_Review_Choices->SetText(glliButtonIDs[j], pRAD_Init->Get_Tool_Name(i));	// put on the label
					mpRAD_Review_Choices->SetCtrlNotifyFn(glliButtonIDs[j], (CGUI_CallBack *)((void *)this), (GUI_CB_DlgNotifyFn)&RADReview::SelectTool); // set up callback
					mpRAD_Review_Choices->SetVisible(glliButtonIDs[j], TRUE);						// show it 
				}
				j++;
			}

			mpRAD_Review_Choices->SetDlgExitValidateFn((CGUI_CallBack *)((void *)this),	(GUI_CB_DlgExitFn)&RADReview::ChoicesExitNotifyFn);			
			mpRAD_Review_Choices->SetFocus(IDC_BUTTON0);
			
			// keep this dialog box always on top
			::SetWindowPos(mpRAD_Review_Choices->GetWndHandle(), HWND_TOP, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);

			mpRAD_Review_Choices->GoModeless(TRUE);      // TRUE indicates box deletes itself on exit
			//mpRAD_Review_Choices->Go();						// stay up until user makes the choice
		}
	}	
}	    	

// This function is called when the user selects one of the tool buttons in the modeless
// dialog box.  After you figure out which button was pushed, then ask that tool to
// display his data.
void RADReview::SelectTool(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)        
{
	int i;
	struct db_sta_rec dbSta;

	for (i=0; i<MAX_BUTTONS; i++) {	// figure out which button was pushed
		if (CtrlID == (unsigned int)glliButtonIDs[i])	break;
	}

	if (!pglFacCfg->GetStationRecord(glsFacNum, &gllChanID, &dbSta)) return;
	RTReview::RT_Display_Other_Data(pRAD_Init->Get_Tool_Name(mpToolNums[i]), gllszTimestamp, dbSta.sz_sta_name);
}	                                                                             
 
 
void RADReview::RAD_Menu_Help_ProductSupport(void) {

//	if (bBusy) return;

	Set_Messages_On_Hold(TRUE);
	mpRAD_Product_Support->Display();
	Set_Messages_On_Hold(FALSE);

}

void RADReview::RAD_Menu_Help_About(void) {

//	if (bBusy) return;

	Set_Messages_On_Hold(TRUE);
	mpRAD_About_Rad_Review->Display();
	Set_Messages_On_Hold(FALSE);

}
 

//void RADReview::RAD_Menu_Help_ProductSupport(void) {
//	CGFmtStr Msg;
	
//	SetCurrentDrive();
//	Msg.Printf("%s%s", szCONTACT_NAME, szCONTACT_EMAIL);
//	CGUI_Dlg Dlg(IDD_RAD_PRODUCT_SUPPORT, NULL, DLG_POSN_SAVE_REL);
//	if (Dlg.IsValid()) {
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
//		Dlg.DefinePushBtn(IDOK, GUI_OK);                   
//		Dlg.DefineFldStaticTxt(IDC_PRODUCT_SUPPORT);
//		Dlg.FldStaticTxtSetValue(IDC_PRODUCT_SUPPORT, Msg);
// 		Dlg.SetFocus(IDOK);   
//		Dlg.Go();
//	}	
//}


//void RADReview::RAD_Menu_Help_About(void) {
//	const static char *pszDate = __DATE__;
//	const static char *pszTime = __TIME__;
//	CGFmtStr Msg;
	
//	SetCurrentDrive();
//	Msg.Printf("%s  %s\n", pRAD_Init->Get_My_Name(), szVERSION_NUMBER);
//	Msg.PrintfAppend("Built at %s on %s", pszTime, pszDate);								   
	
//    CGUI_Dlg Dlg(IDD_RAD_ABOUT, NULL, DLG_POSN_SAVE_REL);
//	if (Dlg.IsValid()) {
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
//		Dlg.DefinePushBtn(IDOK, GUI_OK);                   
//		Dlg.DefineFldStaticTxt(IDC_VERSION_NUM);
//		Dlg.FldStaticTxtSetValue(IDC_VERSION_NUM, Msg);
// 		Dlg.SetFocus(IDOK);   
//		Dlg.Go();
//	}	
//}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Required through RTReview to provide the member functions which save the Radiation Review data
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RADReview::RT_Review_My_Save(void) {

	SaveDatabaseNow();
	return TRUE;
}
 

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Required through RTReview to provide the Review Exit procedure which is specific to Radiation
// Called when someone else is shutting down RAD Review
///////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL RADReview::RT_Review_My_Exit(void) {
	//if (pApp) pApp->Show(SW_MINIMIZE);
    CGUI_App::RegisterMayTerminateFn(NULL);	// shut down by someone else, do not do normal File/Exit action
	pRAD_Review->RAD_Common_Exit();			// someone else is shutting me down, don't ask the question
	CGUI_App::Terminate(); 
	if (pglShutDownDlg != NULL) {
		pglShutDownDlg->DefinePushBtn(IDOK, GUI_NONE);
		pglShutDownDlg->SetVisible(IDOK, FALSE);
		pglShutDownDlg->GoModeless(FALSE);
	}
//	CGUI_App::Show(SW_MINIMIZE);
	return TRUE;
	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Called from the RAD Review Menu Exit
///////////////////////////////////////////////////////////////////////////////////////////////////////
static BOOL RadReviewFileExitFunc()
{
	pRAD_Review->bMayClose = FALSE;
	pRAD_Review->RAD_Menu_File_Exit();
	return pRAD_Review->bMayClose;
}

// This function is here just so can call from RadReviewFileExitFunc()
// Asks shutdown question and then performs common shutdown functions
void RADReview::RAD_Menu_File_Exit(void) {

	if (RTReview::RT_Review_Exit_Question(IDI_NEW_RAD_REVIEW, FALSE)) {
		RAD_Common_Exit();
		bMayClose = TRUE;
	}	
}


//	common Exit procedures

void RADReview::RAD_Common_Exit(void) {

	CloseDataGraph();	// Necessary because graph is running modeless
	CleanupForExit();	// defined in radmain.c
}



// hide the actual parameter from the graph
BOOL RadReviewUsingRealTimeUpdate()
{
	return(pRAD_Review->bRealTimeUpdate);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Make my function to get to the RT Facility header function
///////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RADReview::RAD_Print_Header(char **ppszHeader, BOOL bAdd_Tool_Name_Title, BOOL bReload_Common_Inspec_Info, const char *pszDatabase_Name, const char *pszAppend_String)
{
	return(RTReview::Print_Header(ppszHeader));
}
