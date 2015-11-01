//RadMain.cpp     
//Primary Author: Shirley Klosterbuer

// Main source file for a Review Tool Application
// Simply creates a Review_Tool object and puts it in standby mode for first command
// 01-Jan-2002	Added CoInitialize calls for GRAND Com version 2.10
// 9-Jun-2005 SFK Verify there is a data store for the facility, if not create it.

#define CRTDBG_MAP_ALLOC		// 03-Jan-2006 SFK Use these three statements to get leak info dumped
#include <stdlib.h>
#include <crtdbg.h>

#include <fstream.h>
#include <direct.h>
#include <gui_lib.h>

#include "RAD.H"
#include "BinaryImport.h"
#include "DbGeneralDbSupport.h"
#include "DlgDirection.h"
#include "DlgEventSummary.h"
#include "DlgExport.h"
#include "DlgFindEvent.h"
#include "FacilityConfig.h"
#include "GrandImport.h"
#include "Plot.h"
#include "RadInit.H"
#include "RadImport.H"
#include "RadMain.h"
#include "RadSetup.H"
#include "RadReanalyze.H"
#include "RadReview.H"
#include "upperlayer/RevwTool.H"
#include "ReadCfg.h"
#include "TextFile.h"
#include "TimeAlign.h"	// rad 3.00
#include "Utilities.h"
#include "VifmImport.h"
#include "EOSSImport.h"
#include "FacilityConfig.h"
#include "MyDateTime.h"
#include "DbDay.h"

#define szTOOL_NAME "Radiation Review"

CGUI_Dlg *pglShutDownDlg = NULL;
BOOL glImportExit = FALSE;                   
CGUI_App *pMy_Application = NULL;
RADInit *pRAD_Init = NULL;
RADReview *pRAD_Review = NULL;	
RADImport *pRAD_Import = NULL;
RTTitle *pRT_Title = NULL;
TextFile *pDebugFile = NULL;

// Customize for new COMs
CFacilityConfig *pglFacCfg = NULL;
CEOSSImport *pglEOSSImport = NULL;
CGrandImport *pglGrandImport = NULL;
CVifmImport *pglVifmImport = NULL;
CBinaryImport *pglBinaryImport = NULL;

// The one and only TimeAlign object. hn 10/24/2007
CTimeAlign *pglTimeAlign = NULL;

CMyDateTime glMyDateTime;
CDbDay glDbDay;

// these are used to flag whether the Facility Config menu option has run
bool glbFacilityPicklistFilled = false;
bool glbStationPicklistFilled = false;
bool glbChannelPicklistFilled = false;

short glsFacNum = -1;
short glsStaNum = -1;
short glsChanNum = -1;

// HN 10/24/07 Add globals for logging settings.  Right now, send them to TimeAlign and GrandImport.
bool gbEnableLogging = false;
int  gnLoggingLevel = 3;
bool gbQuietMode = false;  //always for RAD


/* ------------------------------------------------------------------
 *  Commonly used picklist choices
 * ----------------------------------------------------------------*/
char *glnaszIntervalUnitsPicklist[4] = {"days", "hours", "minutes", NULL}; 
char *glnaszDirectionList[5] = {"In    ", "Out   ", "None  ", "In/Out", NULL};
char *glnaszLocationList[3] = {"A  ", "B  ", NULL};
char *glSummaryTypePicklist[] = {"All Events","Daily Summaries", NULL};
char *glSummarySortPicklist[] = {"Time", "Channel", NULL};               
char *glSRAlgorithmsPicklist[] = {"Reals (Doubles) Assay", "Totals (Singles) Assay", NULL};        

BOOL gllbCleanedUp = FALSE;

char glszOldDbName[DRIVE_LEN+PATH_LEN+1] = {""};


void zApp::main()
{                                             
	
	RADSetup *pRAD_Setup = NULL;
	RADReanalyze *pRAD_Reanalyze = NULL;
	Review_Tool *pReview_Tool = NULL;
	
    UINT uiNum_Args;
    char **ppszCommand_Line_Args = NULL;
    char szRad_ini[PATH_LEN+DOSNAME_LEN+1];
	char szRunning_ini[PATH_LEN+DOSNAME_LEN+1];
    char szLogFile[PATH_LEN+DOSNAME_LEN+1];
	char szDataRoot[PATH_LEN+DOSNAME_LEN+1];
	char szRoot[PATH_LEN+DOSNAME_LEN+1];

	int iRunningFileHandle = NULL;
	char szCurWkgDir[PATH_LEN+1];
	char szTitleName[MAX_LINE_LEN+1];

	CoInitialize(NULL);  //DR's 220, 221, 222, 223 Releasing COM foundation incorrectly

	GetCurrentDrive();			// remember this to use later

	CGStr Arguments = cmdLine();	// Get the command line arguments
	CGFmtStr strMsg;
	CGStr strDebugFilename;
	                                
	// Based on the current working directory, figure out where default.cfg is to get exe path
	// Open the default.cfg and find the .exe path location  since want to have 
	// the running.ini file in the RAD directory so there is only one of them
	int iLineNumber = 0;
	CGFmtStr msg;


	// Create the main application first
	if (((pMy_Application = new CGUI_App((zApp *)((void *)this), szTOOL_NAME, szTOOL_NAME)) == NULL) ||
		(!pMy_Application->SetIcon(IDI_NEW_RAD_REVIEW))) {
		RadReviewMsg(uiNO_MEMORY_FOR_APP_ERR);
		//return;
		goto bail;
	}

	// get the data storage path associated with this facility
	pglFacCfg = new CFacilityConfig;

	_getcwd(szCurWkgDir, PATH_LEN);

	// get the default facility number from the \irs\rad\irs.ini file. (Eventually get from FacMgr)
	// HN 10/25/2007 BL2R1, this also retrieves the logging level, which is passed to the imports. 
	//short sFacNum;
	if (!GetDefaultFacilityNumber(&glsFacNum))//sFacNum))
		goto bail;  //DR's 221, 222, 223 Releasing COM foundation incorrectly  

	// Set the name in the title bar to reflect the facility being used.
	SetTitleBar(glsFacNum, true, szTitleName);

	// 9-Jun-2005 SFK Verify there is a data store for the facility, if not create it.
	if ((!GetDirectory(glsFacNum, ROOT_DIR, szRoot, PATH_LEN+DOSNAME_LEN+1)) ||
		(!CreateFacilityDatastore(szRoot))) {
		goto bail;  //DR's 221, 222, 223 Releasing COM foundation incorrectly  
	}

	// Set up full path and name to file we'll use to determine if Rad is running
	if (!GetDirectory(glsFacNum, CONFIG_DIR, szRunning_ini, PATH_LEN+DOSNAME_LEN+1))
		goto bail;  //DR's 221, 222, 223 Releasing COM foundation incorrectly  

	MyStrnCat(szRunning_ini, "Rad\\RadRunning.ini", PATH_LEN+DOSNAME_LEN);

	// Set up full path and name to main configuration file
	if (!GetDirectory(glsFacNum, CONFIG_DIR, szRad_ini, PATH_LEN+DOSNAME_LEN+1))
		goto bail;  //DR's 221, 222, 223 Releasing COM foundation incorrectly  

	MyStrnCat(szRad_ini, "Rad\\Rad.ini", PATH_LEN+DOSNAME_LEN);

	// General diagnostic file added in 2.07b
	// Set up full path and name to the log file and open it
	if (!GetDirectory(glsFacNum, LOG_DIR, szLogFile, PATH_LEN+DOSNAME_LEN+1))
		goto bail;  //DR's 221, 222, 223 Releasing COM foundation incorrectly  

	MyStrnCat(szLogFile, "Rad\\", PATH_LEN+DOSNAME_LEN);
	if (PathExists(szLogFile) == uiPATH_DOES_NOT_EXIST_ERR) 
		MakePath(szLogFile);

	//10-14-2004 Modified to write logs in \Rad subdirectory
	MyStrnCat(szLogFile, "RadReviewLog.txt", PATH_LEN+DOSNAME_LEN);
	pDebugFile = new TextFile;
	//CGStr strDebugFilename = szLogFile;
	strDebugFilename = szLogFile;
	pDebugFile->OpenTextFile(strDebugFilename);
	pDebugFile->ClearTextFile();

	// Log the start of the program
	//CGFmtStr strMsg;
	char szDate[DT_LEN+1] , szTime[DT_LEN+1];
	glMyDateTime.GetCurrentDateTimeStr(szDate, szTime); //<<QA>>Need to update this 
	strMsg.Printf("%s started at %s %s\n", szTitleName, szDate, szTime);
	pDebugFile->WriteLineToTextFile(strMsg.Get_sz());

	if (!GetDirectory(glsFacNum, ROOT_DIR, szRoot, PATH_LEN+DOSNAME_LEN+1))
		goto bail;

	// Customize for new COMs
	// have enough information to create the COMs now
	if (!GetDirectory(glsFacNum, DATA_DIR, szDataRoot, PATH_LEN+DOSNAME_LEN+1))
		goto bail;

	//TODO: Add these also to the imports. HN 10/25/07 (LOGGING PARAMS)
	pglGrandImport = new CGrandImport(szDataRoot, glsFacNum);
	pglVifmImport = new CVifmImport(szDataRoot);
	pglBinaryImport = new CBinaryImport(szDataRoot);
	//27November hn Create EOSS interface pointer.
	pglEOSSImport = new CEOSSImport (szDataRoot,glsFacNum);
	// HN 10/25/07 The TimeAlign COM now can be sent the logging params (they are global)
	pglTimeAlign = new CTimeAlign(szDataRoot,glsFacNum);

	/* THIS NEVER EXECUTES
	// The Facility Configurator has started -- set defaults for facility, station and channel to first
	// HN 10/25/2007 Set it to true and then check it?  DUH?  Change for BL3
	BOOL bStatus = TRUE;
	if (!bStatus) {
		RadReviewMsg(uiRAD_START_FATAL_ERR, "setting Facility Configuration");
		// Customize for new COMs
		if (pglFacCfg) delete pglFacCfg;
		if (pglGrandImport) delete pglGrandImport;
		if (pglVifmImport) delete pglVifmImport;
		if (pglBinaryImport) delete pglBinaryImport;
		if (pglTimeAlign) delete pglTimeAlign;
		//LocalCoUninitialize();
		delete pMy_Application;
		return;
	}*/
	
	// Get the command line arguments
	//CGStr Arguments = cmdLine();
	if ((Review_Tool_GetCmdLineArgs(instance(), Arguments, &uiNum_Args, &ppszCommand_Line_Args)) &&
		// Perform the Review Tool Startup check to verify tool is ready and only one running
		(Review_Tool_Startup_Check(szRunning_ini, szTOOL_NAME, uiNum_Args, (const char **)ppszCommand_Line_Args))) 
	{
		// ?? does the above open the file
		// Open the running.ini file now
		iRunningFileHandle = open(szRunning_ini, ios::in, filebuf::sh_none);

		// Create all the objects, display the splash screen immediately
		if (((pRT_Title = new RTTitle(IDI_NEW_RAD_REVIEW, szTOOL_NAME " - Version " szTOOL_VERSION, szCONTACT_NAME " - " szCONTACT_EMAIL, FALSE)) != NULL) &&
			// Do NOT need to display splash screen, Review_Tool will do so just before it loads the parameters
			//(pRT_Title->RT_Title_Splash_Display()) &&
			((pRAD_Init = new RADInit(szRad_ini, IDI_NEW_RAD_REVIEW)) != NULL) &&  //read all the rad.ini parameters
			((pRAD_Setup = new RADSetup) != NULL) &&                                                     
			((pRAD_Import = new RADImport) != NULL) &&
			((pRAD_Review = new RADReview) != NULL) &&
			((pRAD_Reanalyze = new RADReanalyze) != NULL) &&
			((pReview_Tool = new Review_Tool(IDI_NEW_RAD_REVIEW, *pRAD_Setup, *pRAD_Import, *pRAD_Reanalyze, *pRAD_Review, *pRAD_Init, pRT_Title)) != NULL)) {

			CGUI_TextWin::AllowPrinterFontScaling(3,7);

			if (OpenAndAccessDatabase(szRoot, glsFacNum)) 
			{ // open and get parameters from dbVista db
				if (pglShutDownDlg == NULL)
					pglShutDownDlg = new CGUI_Dlg(IDD_SHUTDOWN);	// create dialog box for shutdown msg

				pReview_Tool->Review_Tool_Begin(TRUE, szTOOL_NAME, uiNum_Args, ppszCommand_Line_Args);
			}	
			else 
			{
				RadReviewMsg(uiRAD_START_FATAL_ERR, "reading the rad.ini or names.cfg file");
				//GUI_MsgBox("Fatal error when reading the rad.ini or names.cfg file.", GUI_ICON_ERROR);
				goto bail;
			}

		}
		else 
		{
			RadReviewMsg(uiNO_MEMORY_FOR_APP_ERR);
			goto bail;
		}
	}
	else 
	{
		goto bail;
	}
	

bail: //DR's 221, 222, 223 Releasing COM foundation incorrectly

    if (!gllbCleanedUp) 
		CleanupForExit(); 	 	

	close(iRunningFileHandle);	// close running.ini file
	CloseDataGraph();	// make sure graph closes normally

	glMyDateTime.GetCurrentDateTimeStr(szDate, szTime);
	strMsg.Printf("%s stopped at %s %s\n", szTitleName, szDate, szTime);
	if (pDebugFile)
		pDebugFile->WriteLineToTextFile(strMsg.Get_sz());

	_fcloseall();

	delete pglFacCfg;
	delete pglGrandImport;
	delete pglVifmImport;
	delete pglBinaryImport;
	delete pglTimeAlign;
	delete pDebugFile;

	if (ppszCommand_Line_Args) // Be sure to delete the command line arguments
		SEB_DELETELISTSTRINGNULL(ppszCommand_Line_Args, uiNum_Args);

	delete pRT_Title;
	delete pRAD_Init;
	delete pRAD_Setup;
	delete pRAD_Import;
	delete pRAD_Review;
	delete pRAD_Reanalyze;
	delete pReview_Tool;
	delete pglShutDownDlg;

	delete pMy_Application; // Always delete last

	CoUninitialize(); //DR's 220, 221, 222, 223 Releasing COM foundation incorrectly

	return;  //end main
}                          

void SetTitleBar(int sFacNum, BOOL bReturnStr, char* pszName)
{
	char szFacName[MAX_LINE_LEN+1];
	char szTitleName[MAX_LINE_LEN+1];
	
	pglFacCfg->GetFacilityLongName(sFacNum, szFacName, MAX_LINE_LEN);
	strcpy(szTitleName,szTOOL_NAME);
	strcat(szTitleName, " ");
	strcat(szTitleName, szTOOL_VERSION);
	strcat(szTitleName, " for ");
	strcat(szTitleName, szFacName);
	pMy_Application->SetTitle(szTitleName);

	if (bReturnStr) 
		MyStrnCpy(pszName, szTitleName, MAX_LINE_LEN);
}




void CleanupForExit() {
	
	CloseDataGraph();
	CloseDatabase();
	CleanUpFindEvents();
	CleanUpEventSummary(); 
	CleanUpDirectionAnalysis(); 
	CleanUpExportToINCC();
	CGUI_TextWin::MDI_CloseAll();
	gllbCleanedUp = TRUE;
	
}
