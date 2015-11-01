// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/11/97

// RTInit.CPP
// Class definition for a Review Tool initialization file


#include "RTInit.H"


RTInit::RTInit(const char *pszInitialization_Name, RESOURCE_ID Icon, BOOL bWrite_Access) :
	Initialization_File(pszInitialization_Name, bWrite_Access),
	pParams_Default(NULL),
	pParams_Current(NULL),
	bParams_Loaded(FALSE),
	bInspec_Info_Loaded(FALSE),
	uiIcon(Icon),
	bSave_Ability(bWrite_Access)
	{

	// The following two lines MUST be completed correctly to continue
	if ((Initialization_File_Created()) &&
		((pParams_Default = new RT_PARAMS) != NULL) && ((pParams_Current = new RT_PARAMS) != NULL)) {
		pParams_Current->pszMy_Name = NULL;
		pParams_Current->pszMy_Start_Mode = NULL;
		pParams_Current->Setup.pszInspec_Info_Common_Filename = NULL;
		pParams_Current->Setup.pszFacility_Name = NULL;
		pParams_Current->Setup.Inspection_Info.pszFacility_Location = NULL;
		pParams_Current->Setup.Inspection_Info.pszInspection_Number = NULL;
		pParams_Current->Setup.Inspection_Info.pReview_Date = NULL;
		pParams_Current->Setup.Inspection_Info.pReview_Time = NULL;
		pParams_Current->Setup.Inspection_Info.ppszInspector_ID = NULL;
		pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices = NULL;
		pParams_Current->Setup.Inspection_Info.pStart_Date = NULL;
		pParams_Current->Setup.Inspection_Info.pStart_Time = NULL;
		pParams_Current->Setup.Inspection_Info.pEnd_Date = NULL;
		pParams_Current->Setup.Inspection_Info.pEnd_Time = NULL;
		pParams_Current->Setup.Inspection_Info.pszComments = NULL;
		pParams_Current->ReanalyzeImport.pszCommon_Database_Path = NULL;
		pParams_Current->ReanalyzeImport.pszMIC_Inspec_Info_Filename = NULL;
		pParams_Current->ReanalyzeImport.pszSummary_Data_Filename = NULL;
		pParams_Current->Review.ppszTool_Name = NULL;
		pParams_Current->Review.ppszTool_Exec_Name = NULL;
		pParams_Current->Review.pbTool_Installed = NULL;
		pParams_Current->Review.pbData_Match_Tools = NULL;
		pParams_Current->Review.pbData_Match_Display_Connect_Message = NULL;
		pParams_Current->Review.pbData_Match_Never_Send_Location = NULL;
		pParams_Current->Review.pbData_Match_Always_Send_Location = NULL;
		pParams_Current->Pipes.puiPipe_Timeout = NULL;
		pParams_Current->Pipes.pszPipe_My_Name = NULL;
		pParams_Current->Pipes.ppszPipe_Other_Name = NULL;		
		pParams_Default->pszMy_Name = NULL;
		pParams_Default->pszMy_Start_Mode = NULL;
		pParams_Default->Setup.pszInspec_Info_Common_Filename = NULL;
		pParams_Default->Setup.pszFacility_Name = NULL;
		pParams_Default->Setup.Inspection_Info.pszFacility_Location = NULL;
		pParams_Default->Setup.Inspection_Info.pszInspection_Number = NULL;
		pParams_Default->Setup.Inspection_Info.pReview_Date = NULL;
		pParams_Default->Setup.Inspection_Info.pReview_Time = NULL;
		pParams_Default->Setup.Inspection_Info.ppszInspector_ID = NULL;
		pParams_Default->Setup.Inspection_Info.puiInspector_ID_Choices = NULL;
		pParams_Default->Setup.Inspection_Info.pStart_Date = NULL;
		pParams_Default->Setup.Inspection_Info.pStart_Time = NULL;
		pParams_Default->Setup.Inspection_Info.pEnd_Date = NULL;
		pParams_Default->Setup.Inspection_Info.pEnd_Time = NULL;
		pParams_Default->Setup.Inspection_Info.pszComments = NULL;
		pParams_Default->ReanalyzeImport.pszCommon_Database_Path = NULL;
		pParams_Default->ReanalyzeImport.pszMIC_Inspec_Info_Filename = NULL;    
		pParams_Default->ReanalyzeImport.pszSummary_Data_Filename = NULL;
		pParams_Default->Review.ppszTool_Name = NULL;
		pParams_Default->Review.ppszTool_Exec_Name = NULL;
		pParams_Default->Review.pbTool_Installed = NULL;
		pParams_Default->Review.pbData_Match_Tools = NULL;
		pParams_Default->Review.pbData_Match_Display_Connect_Message = NULL;
		pParams_Default->Review.pbData_Match_Never_Send_Location = NULL;
		pParams_Default->Review.pbData_Match_Always_Send_Location = NULL;
		pParams_Default->Pipes.puiPipe_Timeout = NULL;
		pParams_Default->Pipes.pszPipe_My_Name = NULL;
		pParams_Default->Pipes.ppszPipe_Other_Name = NULL;
		}

}


RTInit::~RTInit(void) {
    
	if (pParams_Default) {
		RTInit_Delete_Params(pParams_Default);
		delete pParams_Default;
		}
	if (pParams_Current) {
		RTInit_Delete_Params(pParams_Current);
		delete pParams_Current;
		}

}


///////////////////////////////////////
// Load the parameters
///////////////////////////////////////

BOOL RTInit::Load_Parameters(void) {

	bParams_Loaded = FALSE;
	Label_Error = "";
	Label_Warning = "";


	// Check that the parameter structures have been created successfully
	if ((pParams_Current) && (pParams_Default) &&
		(RTInit_Load_Params(pParams_Default)) &&
		(RTInit_Load_Params(pParams_Current))) {
		bParams_Loaded = TRUE;
		RTError.Set_My_Name(Get_My_Name());
		// If failed, must abosultely quit
		if ((Load_Inspection_Info(pParams_Current)) &&
			(Load_Inspection_Info(pParams_Default))) {
			bInspec_Info_Loaded = TRUE;
			// If using a common inspection ini file then update myself now - do not care if it fails
			if (strcmp(pParams_Current->Setup.pszInspec_Info_Common_Filename, Initialization_File::pszInit_Name) != 0)
				Save_Inspection_Info(TRUE);
			}
		// If failed, be sure to clear
		// NOTE: If did fail the inspec parameters just get set as a warning and reported later
		// This allows the app to continue, even if loading common inspec info failed
		else {
			Clear_Common_Inspection_Info(pParams_Current);
			Clear_Common_Inspection_Info(pParams_Default);
			bInspec_Info_Loaded = FALSE;
			}
		}


	return bParams_Loaded;

}


BOOL RTInit::Load_Start_Mode(char **ppszStart_Mode) {

	if (!Label_Read(szRT_START_MODE, ppszStart_Mode)) {
		Label_Error = szRT_START_MODE;
		return FALSE;
		}


	return TRUE;

}


BOOL RTInit::Load_Splash_Screen_Timeout_Seconds(UINT *puiSplash_Screen_Timeout_Seconds) {

	if (!Label_Read(szRT_SPLASH_SCREEN_TIMEOUT_SECONDS, puiSplash_Screen_Timeout_Seconds)) {
		Label_Error = szRT_SPLASH_SCREEN_TIMEOUT_SECONDS;
		return FALSE;
		}


	return TRUE;

}


///////////////////////////////////////
// Switch to a new initialization file
///////////////////////////////////////

BOOL RTInit::Set_New_Initialization_File(const char *pszInitialization_Name, BOOL bWrite_Access) {

	Initialization_File_Close();
	Initialization_File_Open(pszInitialization_Name, bWrite_Access);
	return Load_Parameters();

}

///////////////////////////////////////
// Close initialization file		Added SFK 08-27-2004
///////////////////////////////////////

void RTInit::Initialization_File_Close_Now() {

	Initialization_File_Close();
}


/////////////////////////////////
// Data loading member functions
/////////////////////////////////

BOOL RTInit::RTInit_Load_Params(RT_PARAMS *pParams) {

	CGStr Temp_String;
        
		
	// Delete params if already there
	RTInit_Delete_Params(pParams);
	
    
	// Tool Parameters
	if (!Label_Read(szRT_NAME, &pParams->pszMy_Name)) {
		Label_Error = szRT_NAME;
		return FALSE;
		}
	// Reperform here because did not put in structure earlier
	else if (!Label_Read(szRT_START_MODE, &pParams->pszMy_Start_Mode)) {
		Label_Error = szRT_START_MODE;
		return FALSE;
		}
	// Reperform here because did not put in structure earlier
	else if (!Label_Read(szRT_SPLASH_SCREEN_TIMEOUT_SECONDS, &pParams->uiSplash_Screen_Timeout_Seconds)) {
		Label_Error = szRT_SPLASH_SCREEN_TIMEOUT_SECONDS;
		return FALSE;
		}

	// Setup Parameters
	// Load the parameters which tell if and where the common inspection info file is
	else if (!Label_Read(szRT_INSPEC_INFO_COMMON_FILENAME, &pParams->Setup.pszInspec_Info_Common_Filename)) {
		Label_Error = szRT_INSPEC_INFO_COMMON_FILENAME;
		return FALSE;
		}

	// Reanalyze/Import Parameters
	else if (!Label_Read(szRT_MIC_INSPEC_INFO_FILE_USE, &pParams->ReanalyzeImport.bMIC_Inspec_Info_File_Use)) {
		Label_Error = szRT_MIC_INSPEC_INFO_FILE_USE;
		return FALSE;
		}
	else if (!Label_Read(szRT_MIC_INSPEC_INFO_FILENAME, &pParams->ReanalyzeImport.pszMIC_Inspec_Info_Filename)) {
		Label_Error = szRT_MIC_INSPEC_INFO_FILENAME;
		return FALSE;
		}
	else if (!Label_Read(szRT_COMMON_DATABASE_PATH, &pParams->ReanalyzeImport.pszCommon_Database_Path)) {
		Label_Error = szRT_COMMON_DATABASE_PATH;
		return FALSE;
		}
	else if (!Label_Read(szRT_SUMMARY_DATA_CREATE_FILE, &pParams->ReanalyzeImport.bSummary_Data_Create_File)) {
		Label_Error = szRT_SUMMARY_DATA_CREATE_FILE;
		return FALSE;
		}
	else if (!Label_Read(szRT_SUMMARY_DATA_FILENAME, &pParams->ReanalyzeImport.pszSummary_Data_Filename)) {
		Label_Error = szRT_SUMMARY_DATA_FILENAME;
		return FALSE;
		}

	// Review Parameters		
	else if (!Label_Read(szRT_OTHER_TOOLS_NUM, &pParams->Review.uiTools_Num)) {
		Label_Error = szRT_OTHER_TOOLS_NUM;
		return FALSE;
		}
	else if (!Label_Read(szRT_USE_OTHER_TOOL_AS_COMMANDER, &pParams->Review.bIR_Used)) {
		Label_Error = szRT_USE_OTHER_TOOL_AS_COMMANDER;
		return FALSE;
		}
	else if (!Label_Read(szRT_OTHER_TOOL_COMMANDER_NUM, &pParams->Review.uiIR_Number)) {
		Label_Error = szRT_OTHER_TOOL_COMMANDER_NUM;
		return FALSE;
		}
	else if (!Label_Read(szRT_OTHER_TOOLS_NAMES, &pParams->Review.ppszTool_Name, pParams->Review.uiTools_Num, TRUE)) {
		Label_Error = szRT_OTHER_TOOLS_NAMES;
		return FALSE;
		}
	else if (!Label_Read(szRT_OTHER_TOOLS_EXECS, &pParams->Review.ppszTool_Exec_Name, pParams->Review.uiTools_Num, TRUE)) {
		Label_Error = szRT_OTHER_TOOLS_EXECS;
		return FALSE;
		}
	else if (!Label_Read(szRT_OTHER_TOOLS_INSTALLED, &pParams->Review.pbTool_Installed, pParams->Review.uiTools_Num)) {
		Label_Error = szRT_OTHER_TOOLS_INSTALLED;
		return FALSE;
		}
	else if (!Label_Read(szRT_DATA_MATCH_ERROR_SECONDS, &pParams->Review.fData_Match_Error_Seconds)) {
		Label_Error = szRT_DATA_MATCH_ERROR_SECONDS;
		return FALSE;
		}
	else if (!Label_Read(szRT_DATA_MATCH_MINIMIZE, &pParams->Review.bData_Match_Minimize)) {
		Label_Error = szRT_DATA_MATCH_MINIMIZE;
		return FALSE;
		}
	else if (!Label_Read(szRT_DATA_MATCH_RESTORE, &pParams->Review.bData_Match_Restore)) {
		Label_Error = szRT_DATA_MATCH_RESTORE;
		return FALSE;
		}
	else if (!Label_Read(szRT_DATA_MATCH_OTHER_TOOLS, &pParams->Review.pbData_Match_Tools, pParams->Review.uiTools_Num)) {
		Label_Error = szRT_DATA_MATCH_OTHER_TOOLS;
		return FALSE;
		}
	else if (!Label_Read(szRT_DATA_MATCH_DISPLAY_CONNECT_MESSAGE, &pParams->Review.pbData_Match_Display_Connect_Message, pParams->Review.uiTools_Num)) {
		Label_Error = szRT_DATA_MATCH_DISPLAY_CONNECT_MESSAGE;
		return FALSE;
		}
	else if (!Label_Read(szRT_DATA_MATCH_NEVER_SEND_LOCATION, &pParams->Review.pbData_Match_Never_Send_Location, pParams->Review.uiTools_Num)) {
		Label_Error = szRT_DATA_MATCH_NEVER_SEND_LOCATION;
		return FALSE;
		}
	else if (!Label_Read(szRT_DATA_MATCH_ALWAYS_SEND_LOCATION, &pParams->Review.pbData_Match_Always_Send_Location, pParams->Review.uiTools_Num)) {
		Label_Error = szRT_DATA_MATCH_ALWAYS_SEND_LOCATION;
		return FALSE;
		}

	// Pipe Parameters
	else if (!Label_Read(szRT_PIPE_MY_NAME, &pParams->Pipes.pszPipe_My_Name)) {
		Label_Error = szRT_PIPE_MY_NAME;
		return FALSE;
		}
	else if (!Label_Read(szRT_PIPE_OTHER_NAMES, &pParams->Pipes.ppszPipe_Other_Name, pParams->Review.uiTools_Num)) {
		Label_Error = szRT_PIPE_OTHER_NAMES;
		return FALSE;
		}
	else if (!Label_Read(szRT_PIPE_OTHER_TIMEOUTS, &pParams->Pipes.puiPipe_Timeout, pParams->Review.uiTools_Num)) {
		Label_Error = szRT_PIPE_OTHER_TIMEOUTS;
		return FALSE;
		}

	// Adjust index values
    pParams->Review.uiIR_Number--;
	// Adjust the paths - if path does not have a '\' at the end then add it
	if (strlen(pParams->ReanalyzeImport.pszCommon_Database_Path) == 0) {
		Label_Error = szRT_COMMON_DATABASE_PATH;
		return FALSE;
		}
	else if ('\\' != (pParams->ReanalyzeImport.pszCommon_Database_Path[strlen(pParams->ReanalyzeImport.pszCommon_Database_Path) - 1])) {
		Temp_String = pParams->ReanalyzeImport.pszCommon_Database_Path;
		Temp_String += '\\';
		delete [] pParams->ReanalyzeImport.pszCommon_Database_Path;
		pParams->ReanalyzeImport.pszCommon_Database_Path = NULL;
		if (!Temp_String.Create_sz(&(pParams->ReanalyzeImport.pszCommon_Database_Path))) {
			Label_Error = szRT_COMMON_DATABASE_PATH;
			return FALSE;
			}
		}
	
			
	return TRUE;

}


BOOL RTInit::Load_Inspection_Info(RT_PARAMS *pParams) {

	BOOL bSuccess = FALSE;
	CGStr Old_Filename;
	BOOL bWrite_Access;


	// Check to be sure that valid values exist to continue
	if ((!pParams->Setup.pszInspec_Info_Common_Filename) ||
		(!Initialization_File::pszInit_Name)) {
		return FALSE;
		}


	// Common inspection info file is NOT the same as this one:
	//    Open and load it now and then switch back to proper later.
	if (strcmp(pParams->Setup.pszInspec_Info_Common_Filename, Initialization_File::pszInit_Name) != 0) {
		// Save the state of the current init file
		Old_Filename = Initialization_File::pszInit_Name;
		bWrite_Access = Initialization_File::bWritable;

		// Load the inspection info from the common file
		if ((Initialization_File_Open(pParams->Setup.pszInspec_Info_Common_Filename, FALSE)) &&
			(Load_Common_Inspection_Info(pParams))) {
			bSuccess = TRUE; // Don't return FALSE, just continue
			}
		// If failed to open or load then create a new one and copy over parameters in this apps ini file
		else { 
			// Reopen this apps ini file
			if ((Initialization_File_Open(Old_Filename.Get_sz(), bWrite_Access)) &&
				// Load inspection info from this apps ini file
				(Load_Common_Inspection_Info(pParams))) {
				// Open and save the new common ini -  Do not worry if do not succeed then
				if (Initialization_Save_As(pParams->Setup.pszInspec_Info_Common_Filename, TRUE))
					bSuccess = TRUE; // Don't return FALSE, just continue
				}
			}


		// Always must try to reopen, even if failed
		Initialization_File_Open(Old_Filename.Get_sz(), bWrite_Access);
		}
	// Common inspection info file IS the same as this one:
	else {
		// Load the common inspection info from this apps ini
		return (Load_Common_Inspection_Info(pParams));
		}


	// If common inspection info file is the same as this ini, then just load the parameters
	// Also, if failed trying to load another ini then try finding info in this one
	if (!bSuccess)
		return (Load_Common_Inspection_Info(pParams));
	else return TRUE;

}


BOOL RTInit::Reload_Common_Inspec_Info(void) {

	if ((Load_Inspection_Info(pParams_Default)) &&
		(Load_Inspection_Info(pParams_Current))) {
		bInspec_Info_Loaded = TRUE;
		// If using a common inspection ini file then update myself now - do not care if it fails
		if (strcmp(pParams_Current->Setup.pszInspec_Info_Common_Filename, Initialization_File::pszInit_Name) != 0)
			Save_Inspection_Info(TRUE);
		}

	// If failed, be sure to clear them and give warning message
	// This allows the app to continue, even if loading common inspec info failed
	else {
		Clear_Common_Inspection_Info(pParams_Default);
		Clear_Common_Inspection_Info(pParams_Current);
		RT_Init_Error(uiRT_INIT_ERROR_COMMON_INSPEC_LOAD);
		bInspec_Info_Loaded = FALSE;
		}


	return bInspec_Info_Loaded;

}


BOOL RTInit::Load_Common_Inspection_Info(RT_PARAMS *pParams) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;
	

	// Delete params if already there
	Delete_Common_Inspection_Info(pParams);
	

	// Load the parameters
	// Try to continue even if a parameter fails
	// Note: Put bSuccess last so it does not get evaluated first
	if (!Label_Read(szRT_FACILITY_NAME, &pParams->Setup.pszFacility_Name)) {
		bSuccess = FALSE;
		Label_Warning = szRT_FACILITY_NAME;
		}
	if (!Label_Read(szRT_FACILITY_LOCATION, &pParams->Setup.Inspection_Info.pszFacility_Location)) {
		bSuccess = FALSE;
		Label_Warning = szRT_FACILITY_LOCATION;
		}
	if (!Label_Read(szRT_INSPECTION_DATE, &pParams->Setup.Inspection_Info.pReview_Date)) {
		bSuccess = FALSE;
		Label_Warning = szRT_INSPECTION_DATE;
		}
	if (!Label_Read(szRT_INSPECTION_TIME, &pParams->Setup.Inspection_Info.pReview_Time)) {
		bSuccess = FALSE;
		Label_Warning = szRT_INSPECTION_TIME;
		}
	if (!Label_Read(szRT_INSPECTION_DATA_DATE_START, &pParams->Setup.Inspection_Info.pStart_Date)) {
		bSuccess = FALSE;
		Label_Warning = szRT_INSPECTION_DATA_DATE_START;
		}
	if (!Label_Read(szRT_INSPECTION_DATA_TIME_START, &pParams->Setup.Inspection_Info.pStart_Time)) {
		bSuccess = FALSE;
		Label_Warning = szRT_INSPECTION_DATA_TIME_START;
		}
	if (!Label_Read(szRT_INSPECTION_DATA_DATE_END, &pParams->Setup.Inspection_Info.pEnd_Date)) {
		bSuccess = FALSE;
		Label_Warning = szRT_INSPECTION_DATA_DATE_END;
		}
	if (!Label_Read(szRT_INSPECTION_DATA_TIME_END, &pParams->Setup.Inspection_Info.pEnd_Time)) {
		bSuccess = FALSE;
		Label_Warning = szRT_INSPECTION_DATA_TIME_END;
		}

	if (Label_Read(szRT_INSPECTORS_NUMBER, &pParams->Setup.Inspection_Info.uiInspector_ID_Num)) {
		if (Label_Read(szRT_INSPECTORS_IDS_CHOICES_NUM, &pParams->Setup.Inspection_Info.uiInspector_ID_Choices_Num)) {
			if (Label_Read(szRT_INSPECTORS_IDS_CHOICES, &pParams->Setup.Inspection_Info.puiInspector_ID_Choices, pParams->Setup.Inspection_Info.uiInspector_ID_Choices_Num)) {
				if (Label_Read(szRT_INSPECTORS_IDS, &pParams->Setup.Inspection_Info.ppszInspector_ID, pParams->Setup.Inspection_Info.uiInspector_ID_Num)) {
					// Adjust the index numbers - Must do it only here
					for (ctIndex = 0; ctIndex < pParams->Setup.Inspection_Info.uiInspector_ID_Choices_Num; ctIndex++)
						(pParams->Setup.Inspection_Info.puiInspector_ID_Choices[ctIndex])--;
					}
				// Dont set Choices to zero since they were correctly loaded
				else {
					pParams->Setup.Inspection_Info.uiInspector_ID_Num = 0;
					Label_Warning = szRT_INSPECTORS_IDS;
					bSuccess = FALSE;
					}
				}
			else {
				pParams->Setup.Inspection_Info.uiInspector_ID_Num = 0;
				pParams->Setup.Inspection_Info.uiInspector_ID_Choices_Num = 0;
				Label_Warning = szRT_INSPECTORS_IDS_CHOICES;
				bSuccess = FALSE;
				}
			}
		else {
			pParams->Setup.Inspection_Info.uiInspector_ID_Num = 0;
			Label_Warning = szRT_INSPECTORS_IDS_CHOICES_NUM;
			bSuccess = FALSE;
			}
		}
	else {
		pParams->Setup.Inspection_Info.uiInspector_ID_Num = 0;
		Label_Warning = szRT_INSPECTORS_NUMBER;
		bSuccess = FALSE;
		}

	if (!Label_Read(szRT_INSPECTION_NUMBER, &pParams->Setup.Inspection_Info.pszInspection_Number)) {
		bSuccess = FALSE;
		Label_Warning = szRT_INSPECTION_NUMBER;
		}
	if (!Label_Read(szRT_INSPECTION_COMMENTS, &pParams->Setup.Inspection_Info.pszComments)) {
		bSuccess = FALSE;
		Label_Warning = szRT_INSPECTION_COMMENTS;
		}

		
	return bSuccess;

}


///////////////////////////////////////
// Data loaded status member functions
///////////////////////////////////////

BOOL RTInit::Parameters_Loaded(void) {

	return bParams_Loaded;

}


BOOL RTInit::Inspec_Info_Loaded(void) {

	return bInspec_Info_Loaded;

}


// Returns the errored parameter if Parameters_Loaded returns FALSE, otherwise returns NULL
const char *RTInit::Get_Errored_Init_Parameter(void) {

	if ((Label_Error.Get_sz() != NULL) &&
		((Label_Error.Get_sz())[0] != NULL))
		return (const char *)Label_Error.Get_sz();
	else return NULL;

}


// Returns the warning parameter if Parameters_Loaded returns FALSE, otherwise returns NULL
const char *RTInit::Get_Warning_Init_Parameter(void) {

	if ((Label_Warning.Get_sz() != NULL) &&
		((Label_Warning.Get_sz())[0] != NULL))
		return (const char *)Label_Warning.Get_sz();
	else return NULL;

}


/////////////////////////////////////////////
// Data getting and setting member functions
/////////////////////////////////////////////

const char *RTInit::Get_My_Name(void) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->pszMy_Name);
	else return FALSE;
	
}

BOOL RTInit::Set_My_Name(const char *pszMy_Name) {

	if (bParams_Loaded) {
		if (pParams_Current->pszMy_Name)
			delete [] pParams_Current->pszMy_Name;
		if ((pParams_Current->pszMy_Name = new char [strlen(pszMy_Name) + 1]) != NULL) {
			strcpy(pParams_Current->pszMy_Name, pszMy_Name);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_My_Name(void) {

	return (Set_My_Name(pParams_Default->pszMy_Name));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_My_Start_Mode(void) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->pszMy_Start_Mode);
	else return FALSE;
	
}

BOOL RTInit::Set_My_Start_Mode(const char *pszMy_Start_Mode) {

	if (bParams_Loaded) {
		if (pParams_Current->pszMy_Start_Mode)
			delete [] pParams_Current->pszMy_Start_Mode;
		if ((pParams_Current->pszMy_Start_Mode = new char [strlen(pszMy_Start_Mode) + 1]) != NULL) {
			strcpy(pParams_Current->pszMy_Start_Mode, pszMy_Start_Mode);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_My_Start_Mode(void) {

	return (Set_My_Start_Mode(pParams_Default->pszMy_Start_Mode));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
UINT RTInit::Get_Splash_Screen_Timeout_Seconds(void) {

	if (bParams_Loaded)
		return (pParams_Current->uiSplash_Screen_Timeout_Seconds);
	else return FALSE;

}

BOOL RTInit::Set_Splash_Screen_Timeout_Seconds(UINT uiSplash_Screen_Timeout_Seconds) {

	if (bParams_Loaded) {
        pParams_Current->uiSplash_Screen_Timeout_Seconds = uiSplash_Screen_Timeout_Seconds;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Splash_Screen_Timeout_Seconds(void) {

	return (Set_Splash_Screen_Timeout_Seconds(pParams_Default->uiSplash_Screen_Timeout_Seconds));

}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_Inspec_Info_Common_Filename(void) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->Setup.pszInspec_Info_Common_Filename);
	else return FALSE;

}

BOOL RTInit::Set_Inspec_Info_Common_Filename(const char *pszInspec_Info_Common_Filename) {

	if (bParams_Loaded) {
		if (pParams_Current->Setup.pszInspec_Info_Common_Filename)
			delete [] pParams_Current->Setup.pszInspec_Info_Common_Filename;
		if ((pParams_Current->Setup.pszInspec_Info_Common_Filename = new char [strlen(pszInspec_Info_Common_Filename) + 1]) != NULL) {
			strcpy(pParams_Current->Setup.pszInspec_Info_Common_Filename, pszInspec_Info_Common_Filename);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Inspec_Info_Common_Filename(void) {

	return (Set_Inspec_Info_Common_Filename(pParams_Default->Setup.pszInspec_Info_Common_Filename));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
BOOL RTInit::Get_MIC_Inspec_Info_File_Use(void) {

	if (bParams_Loaded)
		return (pParams_Current->ReanalyzeImport.bMIC_Inspec_Info_File_Use);
	else return FALSE;

}

BOOL RTInit::Set_MIC_Inspec_Info_File_Use(BOOL bMIC_Inspec_Info_File_Use) {

	if (bParams_Loaded) {
		pParams_Current->ReanalyzeImport.bMIC_Inspec_Info_File_Use = bMIC_Inspec_Info_File_Use;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_MIC_Inspec_Info_File_Use(void) {

	return (Set_MIC_Inspec_Info_File_Use(pParams_Default->ReanalyzeImport.bMIC_Inspec_Info_File_Use));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_MIC_Inspec_Info_Filename(void) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->ReanalyzeImport.pszMIC_Inspec_Info_Filename);
	else return FALSE;

}

BOOL RTInit::Set_MIC_Inspec_Info_Filename(const char *pszMIC_Inspec_Info_Filename) {

	if (bParams_Loaded) {
		if (pParams_Current->ReanalyzeImport.pszMIC_Inspec_Info_Filename)
			delete [] pParams_Current->ReanalyzeImport.pszMIC_Inspec_Info_Filename;
		if ((pParams_Current->ReanalyzeImport.pszMIC_Inspec_Info_Filename = new char [strlen(pszMIC_Inspec_Info_Filename) + 1]) != NULL) {
			strcpy(pParams_Current->ReanalyzeImport.pszMIC_Inspec_Info_Filename, pszMIC_Inspec_Info_Filename);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_MIC_Inspec_Info_Filename(void) {

	return (Set_MIC_Inspec_Info_Filename(pParams_Default->ReanalyzeImport.pszMIC_Inspec_Info_Filename));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_Facility_Name(void) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->Setup.pszFacility_Name);
	else return FALSE;

}

BOOL RTInit::Set_Facility_Name(const char *pszFacility_Name) {

	if (bParams_Loaded) {
		if (pParams_Current->Setup.pszFacility_Name)
			delete [] pParams_Current->Setup.pszFacility_Name;
		if ((pParams_Current->Setup.pszFacility_Name = new char [strlen(pszFacility_Name) + 1]) != NULL) {
			strcpy(pParams_Current->Setup.pszFacility_Name, pszFacility_Name);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Facility_Name(void) {

	return (Set_Facility_Name(pParams_Default->Setup.pszFacility_Name));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_Facility_Location(void) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->Setup.Inspection_Info.pszFacility_Location);
	else return FALSE;

}

BOOL RTInit::Set_Facility_Location(const char *pszFacility_Location) {

	if (bParams_Loaded) {
		if (pParams_Current->Setup.Inspection_Info.pszFacility_Location)
			delete [] pParams_Current->Setup.Inspection_Info.pszFacility_Location;
		if ((pParams_Current->Setup.Inspection_Info.pszFacility_Location = new char [strlen(pszFacility_Location) + 1]) != NULL) {
			strcpy(pParams_Current->Setup.Inspection_Info.pszFacility_Location, pszFacility_Location);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Facility_Location(void) {

	return (Set_Facility_Location(pParams_Default->Setup.Inspection_Info.pszFacility_Location));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_Pipe_My_Name(void) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->Pipes.pszPipe_My_Name);
	else return FALSE;

}

BOOL RTInit::Set_Pipe_My_Name(const char *pszPipe_My_Name) {

	if (bParams_Loaded) {
		if (pParams_Current->Pipes.pszPipe_My_Name)
			delete [] pParams_Current->Pipes.pszPipe_My_Name;
		if ((pParams_Current->Pipes.pszPipe_My_Name = new char [strlen(pszPipe_My_Name) + 1]) != NULL) {
			strcpy(pParams_Current->Pipes.pszPipe_My_Name, pszPipe_My_Name);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Pipe_My_Name(void) {

	return (Set_Pipe_My_Name(pParams_Default->Pipes.pszPipe_My_Name));

}
            

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const GEN_DATE_STRUCT *RTInit::Get_Inspection_Date(void) {

	if (bParams_Loaded)
		return ((const GEN_DATE_STRUCT *) pParams_Current->Setup.Inspection_Info.pReview_Date);
	else return FALSE;

}

BOOL RTInit::Set_Inspection_Date(const GEN_DATE_STRUCT *pInspection_Date) {

	if (bParams_Loaded) {
		if (pParams_Current->Setup.Inspection_Info.pReview_Date)
			delete pParams_Current->Setup.Inspection_Info.pReview_Date;
		if ((pParams_Current->Setup.Inspection_Info.pReview_Date = new GEN_DATE_STRUCT) != NULL) {
			pParams_Current->Setup.Inspection_Info.pReview_Date->yr = pInspection_Date->yr;
			pParams_Current->Setup.Inspection_Info.pReview_Date->mon = pInspection_Date->mon;
			pParams_Current->Setup.Inspection_Info.pReview_Date->day = pInspection_Date->day;
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Inspection_Date(void) {

	return (Set_Inspection_Date(pParams_Default->Setup.Inspection_Info.pStart_Date));

}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const GEN_TIME_STRUCT *RTInit::Get_Inspection_Time(void) {

	if (bParams_Loaded)
		return ((const GEN_TIME_STRUCT *) pParams_Current->Setup.Inspection_Info.pReview_Time);
	else return NULL;

}

BOOL RTInit::Set_Inspection_Time(const GEN_TIME_STRUCT *pInspection_Time) {

	if (bParams_Loaded) {
		if (pParams_Current->Setup.Inspection_Info.pReview_Time)
			delete pParams_Current->Setup.Inspection_Info.pReview_Time;
		if ((pParams_Current->Setup.Inspection_Info.pReview_Time = new GEN_TIME_STRUCT) != NULL) {
			pParams_Current->Setup.Inspection_Info.pReview_Time->hr = pInspection_Time->hr;
			pParams_Current->Setup.Inspection_Info.pReview_Time->min = pInspection_Time->min;
			pParams_Current->Setup.Inspection_Info.pReview_Time->sec = pInspection_Time->sec;
			pParams_Current->Setup.Inspection_Info.pReview_Time->msec = pInspection_Time->msec;
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Inspection_Time(void) {

	return (Set_Inspection_Time(pParams_Default->Setup.Inspection_Info.pStart_Time));

}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const GEN_DATE_STRUCT *RTInit::Get_Start_Date(void) {

	if (bParams_Loaded)
		return ((const GEN_DATE_STRUCT *) pParams_Current->Setup.Inspection_Info.pStart_Date);
	else return FALSE;

}

BOOL RTInit::Set_Start_Date(const GEN_DATE_STRUCT *pStart_Date) {

	if (bParams_Loaded) {
		if (pParams_Current->Setup.Inspection_Info.pStart_Date)
			delete pParams_Current->Setup.Inspection_Info.pStart_Date;
		if ((pParams_Current->Setup.Inspection_Info.pStart_Date = new GEN_DATE_STRUCT) != NULL) {
			pParams_Current->Setup.Inspection_Info.pStart_Date->yr = pStart_Date->yr;
			pParams_Current->Setup.Inspection_Info.pStart_Date->mon = pStart_Date->mon;
			pParams_Current->Setup.Inspection_Info.pStart_Date->day = pStart_Date->day;
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Start_Date(void) {

	return (Set_Start_Date(pParams_Default->Setup.Inspection_Info.pStart_Date));

}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const GEN_TIME_STRUCT *RTInit::Get_Start_Time(void) {

	if (bParams_Loaded)
		return ((const GEN_TIME_STRUCT *) pParams_Current->Setup.Inspection_Info.pStart_Time);
	else return NULL;

}

BOOL RTInit::Set_Start_Time(const GEN_TIME_STRUCT *pStart_Time) {

	if (bParams_Loaded) {
		if (pParams_Current->Setup.Inspection_Info.pStart_Time)
			delete pParams_Current->Setup.Inspection_Info.pStart_Time;
		if ((pParams_Current->Setup.Inspection_Info.pStart_Time = new GEN_TIME_STRUCT) != NULL) {
			pParams_Current->Setup.Inspection_Info.pStart_Time->hr = pStart_Time->hr;
			pParams_Current->Setup.Inspection_Info.pStart_Time->min = pStart_Time->min;
			pParams_Current->Setup.Inspection_Info.pStart_Time->sec = pStart_Time->sec;
			pParams_Current->Setup.Inspection_Info.pStart_Time->msec = pStart_Time->msec;
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Start_Time(void) {

	return (Set_Start_Time(pParams_Default->Setup.Inspection_Info.pStart_Time));

}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const GEN_DATE_STRUCT *RTInit::Get_End_Date(void) {

	if (bParams_Loaded)
		return ((const GEN_DATE_STRUCT *) pParams_Current->Setup.Inspection_Info.pEnd_Date);
	else return FALSE;

}

BOOL RTInit::Set_End_Date(const GEN_DATE_STRUCT *pEnd_Date) {

	if (bParams_Loaded) {
		if (pParams_Current->Setup.Inspection_Info.pEnd_Date)
			delete pParams_Current->Setup.Inspection_Info.pEnd_Date;
		if ((pParams_Current->Setup.Inspection_Info.pEnd_Date = new GEN_DATE_STRUCT) != NULL) {
			pParams_Current->Setup.Inspection_Info.pEnd_Date->yr = pEnd_Date->yr;
			pParams_Current->Setup.Inspection_Info.pEnd_Date->mon = pEnd_Date->mon;
			pParams_Current->Setup.Inspection_Info.pEnd_Date->day = pEnd_Date->day;
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_End_Date(void) {

	return (Set_End_Date(pParams_Default->Setup.Inspection_Info.pEnd_Date));

}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const GEN_TIME_STRUCT *RTInit::Get_End_Time(void) {

	if (bParams_Loaded)
		return ((const GEN_TIME_STRUCT *) pParams_Current->Setup.Inspection_Info.pEnd_Time);
	else return NULL;

}

BOOL RTInit::Set_End_Time(const GEN_TIME_STRUCT *pEnd_Time) {

	if (bParams_Loaded) {
		if (pParams_Current->Setup.Inspection_Info.pEnd_Time)
			delete pParams_Current->Setup.Inspection_Info.pEnd_Time;
		if ((pParams_Current->Setup.Inspection_Info.pEnd_Time = new GEN_TIME_STRUCT) != NULL) {
			pParams_Current->Setup.Inspection_Info.pEnd_Time->hr = pEnd_Time->hr;
			pParams_Current->Setup.Inspection_Info.pEnd_Time->min = pEnd_Time->min;
			pParams_Current->Setup.Inspection_Info.pEnd_Time->sec = pEnd_Time->sec;
			pParams_Current->Setup.Inspection_Info.pEnd_Time->msec = pEnd_Time->msec;
			return TRUE;
		}
			}
		
	return FALSE;

}

BOOL RTInit::Reset_End_Time(void) {

	return (Set_End_Time(pParams_Default->Setup.Inspection_Info.pEnd_Time));

}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
UINT RTInit::Get_Inspector_ID_Num(void) {

	if (bParams_Loaded)
		return (pParams_Current->Setup.Inspection_Info.uiInspector_ID_Num);
	else return FALSE;

}

BOOL RTInit::Set_Inspector_ID_Num(UINT uiInspector_ID_Num) {

	if (bParams_Loaded) {
        pParams_Current->Setup.Inspection_Info.uiInspector_ID_Num = uiInspector_ID_Num;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Inspector_ID_Num(void) {

	return (Set_Inspector_ID_Num(pParams_Default->Setup.Inspection_Info.uiInspector_ID_Num));

}

                                                     
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_Inspector_ID(UINT uiInspector_ID_Index) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->Setup.Inspection_Info.ppszInspector_ID[uiInspector_ID_Index]);
	else return FALSE;

}

const char **RTInit::Get_Inspector_IDs(void) {

	if (bParams_Loaded)
		return ((const char **) pParams_Current->Setup.Inspection_Info.ppszInspector_ID);
	else return FALSE;

}

BOOL RTInit::Set_Inspector_ID(UINT uiInspector_ID_Index, const char *pszInspector_ID) {

	if (bParams_Loaded) {
		if (pParams_Current->Setup.Inspection_Info.ppszInspector_ID[uiInspector_ID_Index])
			delete [] pParams_Current->Setup.Inspection_Info.ppszInspector_ID[uiInspector_ID_Index];
		if ((pParams_Current->Setup.Inspection_Info.ppszInspector_ID[uiInspector_ID_Index] = new char [strlen(pszInspector_ID) + 1]) != NULL) {
			strcpy(pParams_Current->Setup.Inspection_Info.ppszInspector_ID[uiInspector_ID_Index], pszInspector_ID);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Set_Inspector_IDs(const char **ppszInspector_ID, UINT uiNum_Inspector_ID) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;
	
	if (bParams_Loaded) {
		SEB_DELETELISTSTRING(pParams_Current->Setup.Inspection_Info.ppszInspector_ID, pParams_Current->Setup.Inspection_Info.uiInspector_ID_Num, ctIndex);
		if ((pParams_Current->Setup.Inspection_Info.ppszInspector_ID = new (char (* ([uiNum_Inspector_ID])))) != NULL) {
			for (ctIndex = 0; ctIndex < uiNum_Inspector_ID; ctIndex++) {
				pParams_Current->Setup.Inspection_Info.ppszInspector_ID[ctIndex] = NULL;
				bSuccess = bSuccess && Set_Inspector_ID(ctIndex, ppszInspector_ID[ctIndex]);
				}
			return ((bSuccess) && (Set_Inspector_ID_Num(uiNum_Inspector_ID)));
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Inspector_ID(UINT uiInspector_ID_Index) {

	return (Set_Inspector_ID(uiInspector_ID_Index, pParams_Default->Setup.Inspection_Info.ppszInspector_ID[uiInspector_ID_Index]));

}

BOOL RTInit::Reset_Inspector_IDs(void) {

 	return (Set_Inspector_IDs((const char **)pParams_Default->Setup.Inspection_Info.ppszInspector_ID, pParams_Default->Setup.Inspection_Info.uiInspector_ID_Num));
 	
}

                                      
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
UINT RTInit::Get_Inspector_ID_Choices_Num(void) {

	if (bParams_Loaded)
		return (pParams_Current->Setup.Inspection_Info.uiInspector_ID_Choices_Num);
	else return FALSE;

}

BOOL RTInit::Set_Inspector_ID_Choices_Num(UINT uiInspector_ID_Choices_Num) {

	if (bParams_Loaded) {
        pParams_Current->Setup.Inspection_Info.uiInspector_ID_Choices_Num = uiInspector_ID_Choices_Num;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Inspector_ID_Choices_Num(void) {

	return (Set_Inspector_ID_Choices_Num(pParams_Default->Setup.Inspection_Info.uiInspector_ID_Choices_Num));

}

                                                     
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
UINT RTInit::Get_Inspector_ID_Choice(UINT uiInspector_ID_Choice_Num) {

	if (bParams_Loaded)
		return (pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices[uiInspector_ID_Choice_Num]);
	else return FALSE;

}

const UINT *RTInit::Get_Inspector_ID_Choices(void) {

	if (bParams_Loaded)
		return ((const UINT *) pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices);
	else return FALSE;

}

BOOL RTInit::Set_Inspector_ID_Choice(UINT uiInspector_ID_Choice_Num, UINT uiInspector_ID_Choice) {

	if (bParams_Loaded) {
        pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices[uiInspector_ID_Choice_Num] = uiInspector_ID_Choice;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Set_Inspector_ID_Choices(UINT *puiInspector_ID_Choices, UINT uiInspector_ID_Choices_Num) {
        
	UINT ctIndex;
	        
	if (bParams_Loaded) {
		if (pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices)
			delete [] pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices;
		if ((pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices = new UINT [uiInspector_ID_Choices_Num]) != NULL) {
			for (ctIndex = 0; ctIndex < uiInspector_ID_Choices_Num; ctIndex++)
				pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices[ctIndex] = puiInspector_ID_Choices[ctIndex];
			return (Set_Inspector_ID_Choices_Num(uiInspector_ID_Choices_Num));
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Inspector_ID_Choice(UINT uiInspector_ID_Choice_Num) {

	return (Set_Inspector_ID_Choice(uiInspector_ID_Choice_Num, pParams_Default->Setup.Inspection_Info.puiInspector_ID_Choices[uiInspector_ID_Choice_Num]));

}

BOOL RTInit::Reset_Inspector_ID_Choices(void) {

 	return (Set_Inspector_ID_Choices(pParams_Default->Setup.Inspection_Info.puiInspector_ID_Choices, pParams_Default->Setup.Inspection_Info.uiInspector_ID_Choices_Num));
 	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
//DWORD RTInit::Get_Inspection_Number(void) {
//
//	DWORD Temp_Inspec_Number;
//
//	if ((bParams_Loaded) &&
//		(1 == sscanf(pParams_Current->Setup.Inspection_Info.pszInspection_Number, "%ul", &Temp_Inspec_Number)))
//		return (Temp_Inspec_Number);
//	else return FALSE;
//
//}

const char *RTInit::Get_Inspection_Number(void) {

	if (bParams_Loaded)
		return (pParams_Current->Setup.Inspection_Info.pszInspection_Number);
	else return FALSE;

}

BOOL RTInit::Set_Inspection_Number(DWORD dInspection_Number) {

	if (bParams_Loaded) {
		delete [] pParams_Current->Setup.Inspection_Info.pszInspection_Number;
		if (((pParams_Current->Setup.Inspection_Info.pszInspection_Number = new char [20]) != NULL) &&
			(1 == sprintf(pParams_Current->Setup.Inspection_Info.pszInspection_Number, "%05ul", dInspection_Number)))
			return TRUE;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Set_Inspection_Number(const char *pszInspection_Number) {

	if (bParams_Loaded) {
		delete [] pParams_Current->Setup.Inspection_Info.pszInspection_Number;
		if ((pParams_Current->Setup.Inspection_Info.pszInspection_Number = new char [strlen(pszInspection_Number) + 1]) != NULL) {
			strcpy(pParams_Current->Setup.Inspection_Info.pszInspection_Number, pszInspection_Number);
			return TRUE;
			}
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Inspection_Number(void) {

	return (Set_Inspection_Number(pParams_Default->Setup.Inspection_Info.pszInspection_Number));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_Inspection_Comments(void) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->Setup.Inspection_Info.pszComments);
	else return FALSE;
	
}

BOOL RTInit::Set_Inspection_Comments(const char *pszInspection_Comments) {

	if (bParams_Loaded) {
		if (pParams_Current->Setup.Inspection_Info.pszComments)
			delete [] pParams_Current->Setup.Inspection_Info.pszComments;
		if ((pParams_Current->Setup.Inspection_Info.pszComments = new char [strlen(pszInspection_Comments) + 1]) != NULL) {
			strcpy(pParams_Current->Setup.Inspection_Info.pszComments, pszInspection_Comments);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Inspection_Comments(void) {

	return (Set_Inspection_Comments(pParams_Default->Setup.Inspection_Info.pszComments));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
UINT RTInit::Get_Tools_Num(void) {

	if (bParams_Loaded)
		return (pParams_Current->Review.uiTools_Num);
	else return FALSE;

}

BOOL RTInit::Set_Tools_Num(UINT uiTools_Num) {

	if (bParams_Loaded) {
        pParams_Current->Review.uiTools_Num = uiTools_Num;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Tools_Num(void) {

	return (Set_Tools_Num(pParams_Default->Review.uiTools_Num));

}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_Pipe_Other_Name(UINT uiPipe_Other_Name_Index) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->Pipes.ppszPipe_Other_Name[uiPipe_Other_Name_Index]);
	else return FALSE;

}

const char **RTInit::Get_Pipe_Other_Names(void) {

	if (bParams_Loaded)
		return ((const char **) pParams_Current->Pipes.ppszPipe_Other_Name);
	else return FALSE;

}

BOOL RTInit::Set_Pipe_Other_Name(UINT uiPipe_Other_Name_Index, const char *pszPipe_Other_Name) {

	if (bParams_Loaded) {
		if (pParams_Current->Pipes.ppszPipe_Other_Name[uiPipe_Other_Name_Index])
			delete [] pParams_Current->Pipes.ppszPipe_Other_Name[uiPipe_Other_Name_Index];
		if ((pParams_Current->Pipes.ppszPipe_Other_Name[uiPipe_Other_Name_Index] = new char [strlen(pszPipe_Other_Name) + 1]) != NULL) {
			strcpy(pParams_Current->Pipes.ppszPipe_Other_Name[uiPipe_Other_Name_Index], pszPipe_Other_Name);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Pipe_Other_Name(UINT uiPipe_Other_Name_Index) {

	return (Set_Pipe_Other_Name(uiPipe_Other_Name_Index, pParams_Default->Pipes.ppszPipe_Other_Name[uiPipe_Other_Name_Index]));

}

BOOL RTInit::Reset_Pipe_Other_Names(void) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;
	                
	                
 	for (ctIndex = 0; ctIndex < pParams_Default->Review.uiTools_Num; ctIndex++)
 		bSuccess = bSuccess && Reset_Pipe_Other_Name(ctIndex);
 
 	return FALSE;
 			
}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
UINT RTInit::Get_Pipe_Timeout(UINT uiPipe_Timeout_Index) {

	if (bParams_Loaded)
		return (pParams_Current->Pipes.puiPipe_Timeout[uiPipe_Timeout_Index]);
	else return FALSE;

}

BOOL RTInit::Set_Pipe_Timeout(UINT uiPipe_Timeout_Index, UINT uiPipe_Timeout) {

	if (bParams_Loaded) {
        pParams_Current->Pipes.puiPipe_Timeout[uiPipe_Timeout_Index] = uiPipe_Timeout;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Pipe_Timeout(UINT uiPipe_Timeout_Index) {

	return (Set_Pipe_Timeout(uiPipe_Timeout_Index, pParams_Default->Pipes.puiPipe_Timeout[uiPipe_Timeout_Index]));

}

BOOL RTInit::Reset_Pipe_Timeouts(void) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;
	                
	                
 	for (ctIndex = 0; ctIndex < pParams_Default->Review.uiTools_Num; ctIndex++)
 		bSuccess = bSuccess && Reset_Pipe_Timeout(ctIndex);
 
 	return FALSE;
 			
}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_Tool_Name(UINT uiTool_Name_Index) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->Review.ppszTool_Name[uiTool_Name_Index]);
	else return FALSE;

}

const char **RTInit::Get_Tool_Names(void) {

	if (bParams_Loaded)
		return ((const char **) pParams_Current->Review.ppszTool_Name);
	else return FALSE;

}

BOOL RTInit::Set_Tool_Name(UINT uiTool_Name_Index, const char *pszTool_Name) {

	if (bParams_Loaded) {
		if (pParams_Current->Review.ppszTool_Name[uiTool_Name_Index])
			delete [] pParams_Current->Review.ppszTool_Name[uiTool_Name_Index];
		if ((pParams_Current->Review.ppszTool_Name[uiTool_Name_Index] = new char [strlen(pszTool_Name) + 1]) != NULL) {
			strcpy(pParams_Current->Review.ppszTool_Name[uiTool_Name_Index], pszTool_Name);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Tool_Name(UINT uiTool_Name_Index) {

	return (Set_Tool_Name(uiTool_Name_Index, pParams_Default->Review.ppszTool_Name[uiTool_Name_Index]));

}

BOOL RTInit::Reset_Tool_Names(void) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;
	                
	                
 	for (ctIndex = 0; ctIndex < pParams_Default->Review.uiTools_Num; ctIndex++)
 		bSuccess = bSuccess && Reset_Tool_Name(ctIndex);
 
 	return FALSE;
 			
}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_Common_Database_Path(void) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->ReanalyzeImport.pszCommon_Database_Path);
	else return FALSE;

}

BOOL RTInit::Set_Common_Database_Path(const char *pszCommon_Database_Path) {

	if (bParams_Loaded) {
		if (pParams_Current->ReanalyzeImport.pszCommon_Database_Path)
			delete [] pParams_Current->ReanalyzeImport.pszCommon_Database_Path;
		if ((pParams_Current->ReanalyzeImport.pszCommon_Database_Path = new char [strlen(pszCommon_Database_Path) + 1]) != NULL) {
			strcpy(pParams_Current->ReanalyzeImport.pszCommon_Database_Path, pszCommon_Database_Path);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Common_Database_Path(void) {

	return (Set_Common_Database_Path(pParams_Default->ReanalyzeImport.pszCommon_Database_Path));

}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
BOOL RTInit::Get_Summary_Data_Create_File(void) {

	if (bParams_Loaded)
		return (pParams_Current->ReanalyzeImport.bSummary_Data_Create_File);
	else return FALSE;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_Summary_Data_Filename(void) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->ReanalyzeImport.pszSummary_Data_Filename);
	else return FALSE;

}
BOOL RTInit::Set_Summary_Data_Filename(const char *pszSummary_Data_Filename) {

	if (bParams_Loaded) {
		if (pParams_Current->ReanalyzeImport.pszSummary_Data_Filename) {
			delete [] pParams_Current->ReanalyzeImport.pszSummary_Data_Filename;
			pParams_Current->ReanalyzeImport.pszSummary_Data_Filename = NULL;
		}
		if ((pParams_Current->ReanalyzeImport.pszSummary_Data_Filename = new char [strlen(pszSummary_Data_Filename) + 1]) != NULL) {
			strcpy(pParams_Current->ReanalyzeImport.pszSummary_Data_Filename, pszSummary_Data_Filename);
			return TRUE;
			}
		}

	return FALSE;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
const char *RTInit::Get_Tool_Exec_Name(UINT uiTool_Exec_Index) {

	if (bParams_Loaded)
		return ((const char *) pParams_Current->Review.ppszTool_Exec_Name[uiTool_Exec_Index]);
	else return FALSE;

}

const char **RTInit::Get_Tool_Exec_Names(void) {

	if (bParams_Loaded)
		return ((const char **) pParams_Current->Review.ppszTool_Exec_Name);
	else return FALSE;

}

BOOL RTInit::Set_Tool_Exec_Name(UINT uiTool_Exec_Index, const char *pszTool_Exec_Name) {

	if (bParams_Loaded) {
		if (pParams_Current->Review.ppszTool_Exec_Name[uiTool_Exec_Index])
			delete [] pParams_Current->Review.ppszTool_Exec_Name[uiTool_Exec_Index];
		if ((pParams_Current->Review.ppszTool_Exec_Name[uiTool_Exec_Index] = new char [strlen(pszTool_Exec_Name) + 1]) != NULL) {
			strcpy(pParams_Current->Review.ppszTool_Exec_Name[uiTool_Exec_Index], pszTool_Exec_Name);
			return TRUE;
			}
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Tool_Exec_Name(UINT uiTool_Exec_Index) {

	return (Set_Tool_Exec_Name(uiTool_Exec_Index, pParams_Default->Review.ppszTool_Exec_Name[uiTool_Exec_Index]));

}

BOOL RTInit::Reset_Tool_Exec_Names(void) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;
	                
	                
 	for (ctIndex = 0; ctIndex < pParams_Default->Review.uiTools_Num; ctIndex++)
 		bSuccess = bSuccess && Reset_Tool_Exec_Name(ctIndex);
 
 	return FALSE;
 			
}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
BOOL RTInit::Get_Tool_Installed(UINT uiTool_Installed_Index) {

	if (bParams_Loaded)
		if (uiTool_Installed_Index < Get_Tools_Num())
			return (pParams_Current->Review.pbTool_Installed[uiTool_Installed_Index]);
		else return FALSE;
	else return FALSE;

}

BOOL RTInit::Set_Tool_Installed(UINT uiTool_Installed_Index, BOOL bTool_Installed) {

	if (bParams_Loaded) {
        pParams_Current->Review.pbTool_Installed[uiTool_Installed_Index] = bTool_Installed;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Tool_Installed(UINT uiTool_Installed_Index) {

	return (Set_Tool_Installed(uiTool_Installed_Index, pParams_Default->Review.pbTool_Installed[uiTool_Installed_Index]));

}

BOOL RTInit::Reset_Tool_Installed(void) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;
	                
	                
 	for (ctIndex = 0; ctIndex < pParams_Default->Review.uiTools_Num; ctIndex++)
 		bSuccess = bSuccess && Reset_Tool_Installed(ctIndex);
 
 	return FALSE;
 			
}

            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
BOOL RTInit::Get_IR_Used(void) {

	if (bParams_Loaded)
		return (pParams_Current->Review.bIR_Used);
	else return FALSE;

}

BOOL RTInit::Set_IR_Used(BOOL bIR_Used) {

	if (bParams_Loaded) {
        pParams_Current->Review.bIR_Used = bIR_Used;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_IR_Used(void) {

	return (Set_IR_Used(pParams_Default->Review.bIR_Used));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
UINT RTInit::Get_IR_Number(void) {

	if (bParams_Loaded)
		return (pParams_Current->Review.uiIR_Number);
	else return FALSE;

}

BOOL RTInit::Set_IR_Number(UINT uiIR_Number) {

	if (bParams_Loaded) {
        pParams_Current->Review.uiTools_Num = uiIR_Number;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_IR_Number(void) {

	return (Set_IR_Number(pParams_Default->Review.uiIR_Number));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
float RTInit::Get_Data_Match_Error_Seconds(void) {

	if (bParams_Loaded)
		return (pParams_Current->Review.fData_Match_Error_Seconds);
	else return FALSE;

}

BOOL RTInit::Set_Data_Match_Error_Seconds(float fData_Match_Error_Seconds) {

	if (bParams_Loaded) {
        pParams_Current->Review.fData_Match_Error_Seconds = fData_Match_Error_Seconds;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Data_Match_Error_Seconds(void) {

	return (Set_Data_Match_Error_Seconds(pParams_Default->Review.fData_Match_Error_Seconds));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
BOOL RTInit::Get_Data_Match_Minimize(void) {

	if (bParams_Loaded)
		return (pParams_Current->Review.bData_Match_Minimize);
	else return FALSE;

}

BOOL RTInit::Set_Data_Match_Minimize(BOOL bData_Match_Minimize) {

	if (bParams_Loaded) {
        pParams_Current->Review.bData_Match_Minimize = bData_Match_Minimize;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Data_Match_Minimize(void) {

	return (Set_Data_Match_Minimize(pParams_Default->Review.bData_Match_Minimize));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
BOOL RTInit::Get_Data_Match_Restore(void) {

	if (bParams_Loaded)
		return (pParams_Current->Review.bData_Match_Restore);
	else return FALSE;

}

BOOL RTInit::Set_Data_Match_Restore(BOOL bData_Match_Restore) {

	if (bParams_Loaded) {
        pParams_Current->Review.bData_Match_Restore = bData_Match_Restore;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Data_Match_Restore(void) {

	return (Set_Data_Match_Minimize(pParams_Default->Review.bData_Match_Restore));

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
BOOL RTInit::Get_Data_Match_Tool(UINT uiData_Match_Tool) {

	if ((bParams_Loaded) && (uiData_Match_Tool < Get_Tools_Num()))
		return (pParams_Current->Review.pbData_Match_Tools[uiData_Match_Tool]);
	else return FALSE;

}

BOOL RTInit::Set_Data_Match_Tool(UINT uiData_Match_Tool, BOOL bData_Match_Tool) {

	if ((bParams_Loaded) && (uiData_Match_Tool < Get_Tools_Num())) {
        pParams_Current->Review.pbData_Match_Tools[uiData_Match_Tool] = bData_Match_Tool;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Data_Match_Tool(void) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;

	for (ctIndex = 0; ctIndex < Get_Tools_Num(); ctIndex++)
		bSuccess = bSuccess && Set_Data_Match_Tool(ctIndex, pParams_Default->Review.pbData_Match_Tools[ctIndex]);

	return bSuccess;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
BOOL RTInit::Get_Data_Match_Display_Connect_Message(UINT uiData_Match_Tool) {

	if ((bParams_Loaded) && (uiData_Match_Tool < Get_Tools_Num()))
		return (pParams_Current->Review.pbData_Match_Display_Connect_Message[uiData_Match_Tool]);
	else return FALSE;

}

BOOL RTInit::Set_Data_Match_Display_Connect_Message(UINT uiData_Match_Tool, BOOL bDisplay_Connect_Message) {

	if ((bParams_Loaded) && (uiData_Match_Tool < Get_Tools_Num())) {
        pParams_Current->Review.pbData_Match_Display_Connect_Message[uiData_Match_Tool] = bDisplay_Connect_Message;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Data_Match_Display_Connect_Message(void) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;

	for (ctIndex = 0; ctIndex < Get_Tools_Num(); ctIndex++)
		bSuccess = bSuccess && Set_Data_Match_Display_Connect_Message(ctIndex, pParams_Default->Review.pbData_Match_Display_Connect_Message[ctIndex]);

	return bSuccess;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
BOOL RTInit::Get_Data_Match_Never_Send_Location(UINT uiData_Match_Tool) {

	if ((bParams_Loaded) && (uiData_Match_Tool < Get_Tools_Num()))
		return (pParams_Current->Review.pbData_Match_Never_Send_Location[uiData_Match_Tool]);
	else return FALSE;

}

BOOL RTInit::Set_Data_Match_Never_Send_Location(UINT uiData_Match_Tool, BOOL bNever_Send_Location) {

	if ((bParams_Loaded) && (uiData_Match_Tool < Get_Tools_Num())) {
        pParams_Current->Review.pbData_Match_Never_Send_Location[uiData_Match_Tool] = bNever_Send_Location;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Data_Match_Never_Send_Location(void) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;

	for (ctIndex = 0; ctIndex < Get_Tools_Num(); ctIndex++)
		bSuccess = bSuccess && Set_Data_Match_Never_Send_Location(ctIndex, pParams_Default->Review.pbData_Match_Never_Send_Location[ctIndex]);

	return bSuccess;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
BOOL RTInit::Get_Data_Match_Always_Send_Location(UINT uiData_Match_Tool) {

	if ((bParams_Loaded) && (uiData_Match_Tool < Get_Tools_Num()))
		return (pParams_Current->Review.pbData_Match_Always_Send_Location[uiData_Match_Tool]);
	else return FALSE;

}

BOOL RTInit::Set_Data_Match_Always_Send_Location(UINT uiData_Match_Tool, BOOL bAlways_Send_Location) {

	if ((bParams_Loaded) && (uiData_Match_Tool < Get_Tools_Num())) {
        pParams_Current->Review.pbData_Match_Always_Send_Location[uiData_Match_Tool] = bAlways_Send_Location;
		return TRUE;
		}
		
	return FALSE;

}

BOOL RTInit::Reset_Data_Match_Always_Send_Location(void) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;

	for (ctIndex = 0; ctIndex < Get_Tools_Num(); ctIndex++)
		bSuccess = bSuccess && Set_Data_Match_Always_Send_Location(ctIndex, pParams_Default->Review.pbData_Match_Always_Send_Location[ctIndex]);

	return bSuccess;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
// Parameter saving member functions 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                     
          
BOOL RTInit::Save_Parameters(void) {

	BOOL bSuccess = TRUE;		// sfk 7/14/2003 initialized bSuccess


	// Be sure that the init worker is created and ability to save
	if ((!bParams_Loaded) ||
		(!bSave_Ability))
		return FALSE;
		

	// Tool Parameters
	if (!Label_Change(szRT_NAME, pParams_Current->pszMy_Name))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_START_MODE, pParams_Current->pszMy_Start_Mode))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_SPLASH_SCREEN_TIMEOUT_SECONDS, pParams_Current->uiSplash_Screen_Timeout_Seconds))
		bSuccess = FALSE;

	// Setup Parameters
	else if (!Save_Inspection_Info())
		bSuccess = FALSE;

	// Reanalyze/Import Parameters
	else if (!Label_Change(szRT_COMMON_DATABASE_PATH, pParams_Current->ReanalyzeImport.pszCommon_Database_Path))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_MIC_INSPEC_INFO_FILE_USE, pParams_Current->ReanalyzeImport.bMIC_Inspec_Info_File_Use))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_MIC_INSPEC_INFO_FILENAME, pParams_Current->ReanalyzeImport.pszMIC_Inspec_Info_Filename))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_SUMMARY_DATA_CREATE_FILE, pParams_Current->ReanalyzeImport.bSummary_Data_Create_File))
		return FALSE;
	else if (!Label_Change(szRT_SUMMARY_DATA_FILENAME, pParams_Current->ReanalyzeImport.pszSummary_Data_Filename))
		bSuccess = FALSE;
		
	// Review Parameters		
	else if (!Label_Change(szRT_OTHER_TOOLS_NUM, pParams_Current->Review.uiTools_Num))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_USE_OTHER_TOOL_AS_COMMANDER, pParams_Current->Review.bIR_Used))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_OTHER_TOOL_COMMANDER_NUM, (pParams_Current->Review.uiIR_Number + 1)))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_OTHER_TOOLS_NAMES, (const char **)pParams_Current->Review.ppszTool_Name, pParams_Current->Review.uiTools_Num))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_OTHER_TOOLS_EXECS, (const char **)pParams_Current->Review.ppszTool_Exec_Name, pParams_Current->Review.uiTools_Num))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_OTHER_TOOLS_INSTALLED, pParams_Current->Review.pbTool_Installed, pParams_Current->Review.uiTools_Num))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_DATA_MATCH_ERROR_SECONDS, pParams_Current->Review.fData_Match_Error_Seconds))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_DATA_MATCH_MINIMIZE, pParams_Current->Review.bData_Match_Minimize))
		return FALSE;
	else if (!Label_Change(szRT_DATA_MATCH_RESTORE, pParams_Current->Review.bData_Match_Restore))
		return FALSE;
	else if (!Label_Change(szRT_DATA_MATCH_OTHER_TOOLS, pParams_Current->Review.pbData_Match_Tools, pParams_Current->Review.uiTools_Num))
		return FALSE;
	else if (!Label_Change(szRT_DATA_MATCH_DISPLAY_CONNECT_MESSAGE, pParams_Current->Review.pbData_Match_Display_Connect_Message, pParams_Current->Review.uiTools_Num))
		return FALSE;
	else if (!Label_Change(szRT_DATA_MATCH_NEVER_SEND_LOCATION, pParams_Current->Review.pbData_Match_Never_Send_Location, pParams_Current->Review.uiTools_Num))
		return FALSE;
	else if (!Label_Change(szRT_DATA_MATCH_ALWAYS_SEND_LOCATION, pParams_Current->Review.pbData_Match_Always_Send_Location, pParams_Current->Review.uiTools_Num))
		return FALSE;

	// Pipe Parameters
	else if (!Label_Change(szRT_PIPE_MY_NAME, pParams_Current->Pipes.pszPipe_My_Name))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_PIPE_OTHER_NAMES, (const char **)pParams_Current->Pipes.ppszPipe_Other_Name, pParams_Current->Review.uiTools_Num))
		bSuccess = FALSE;
	else if (!Label_Change(szRT_PIPE_OTHER_TIMEOUTS, pParams_Current->Pipes.puiPipe_Timeout, pParams_Current->Review.uiTools_Num))
		bSuccess = FALSE;

		
	bSuccess = bSuccess && RTInit_Load_Params(pParams_Default);
                    
                    
	return bSuccess;

}


BOOL RTInit::Save_Inspection_Info(void) {

	return (Save_Inspection_Info(FALSE));

}


BOOL RTInit::Save_Inspection_Info(BOOL bForce_Save_To_App_INI) {
    
    UINT ctIndex;
	BOOL bSuccess = TRUE;
	CGStr Old_Filename;
	BOOL bWrite_Access;
	BOOL bOpen_Inspec_File = FALSE;


	// Be sure that the init worker is created and ability to save
	if ((!bInspec_Info_Loaded) ||
		(!bSave_Ability)||
		(!Get_Inspec_Info_Common_Filename()) ||
		(!Get_Initialization_Name()))
		// If bParams_Loaded = FALSE then could not have started
		return FALSE;


	// If set to get inspection info from the common, then open it now (and then switch back to proper)
	// If do NOT want to use a different common, then do nothing
	if (!bForce_Save_To_App_INI) {
		if (strcmp(Get_Inspec_Info_Common_Filename(), Get_Initialization_Name()) != 0) {
			bOpen_Inspec_File = TRUE;
			Old_Filename = Initialization_File::pszInit_Name;
			bWrite_Access = Initialization_File::bWritable;

			if (!Initialization_File_Open(Get_Inspec_Info_Common_Filename(), TRUE)) {
				RT_Init_Error(uiRT_ERROR_COMMON_INSPEC_SAVE);
				// Try to reopen the old at least
				Initialization_File_Open(Old_Filename.Get_sz(), bWrite_Access);
				return FALSE;
				}
			}
		}


	// NOTE: Don't forget to change index numbers (Increase by one)
	for (ctIndex = 0; ctIndex < pParams_Current->Setup.Inspection_Info.uiInspector_ID_Choices_Num; ctIndex++)
		(pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices[ctIndex])++;
	// Not necessary for location choice since only one number

	if (!Buffer_Changes(TRUE)) // Buffer changes in memory
		bSuccess = FALSE;


	if (!Label_Change(szRT_FACILITY_NAME, pParams_Current->Setup.pszFacility_Name))
		bSuccess = FALSE;

	if (!Label_Change(szRT_FACILITY_LOCATION, pParams_Current->Setup.Inspection_Info.pszFacility_Location))
		bSuccess = FALSE;

	if (!Label_Change(szRT_INSPECTION_DATE, pParams_Current->Setup.Inspection_Info.pReview_Date))
		bSuccess = FALSE;

	if (!Label_Change(szRT_INSPECTION_TIME, pParams_Current->Setup.Inspection_Info.pReview_Time))
		bSuccess = FALSE;

	if (!Label_Change(szRT_INSPECTION_DATA_DATE_START, pParams_Current->Setup.Inspection_Info.pStart_Date))
		bSuccess = FALSE;

	if (!Label_Change(szRT_INSPECTION_DATA_TIME_START, pParams_Current->Setup.Inspection_Info.pStart_Time))
		bSuccess = FALSE;

	if (!Label_Change(szRT_INSPECTION_DATA_DATE_END, pParams_Current->Setup.Inspection_Info.pEnd_Date))
		bSuccess = FALSE;

	if (!Label_Change(szRT_INSPECTION_DATA_TIME_END, pParams_Current->Setup.Inspection_Info.pEnd_Time))
		bSuccess = FALSE;

	if (!Label_Change(szRT_INSPECTORS_NUMBER, pParams_Current->Setup.Inspection_Info.uiInspector_ID_Num))
		bSuccess = FALSE;
	
	if (pParams_Current->Setup.Inspection_Info.uiInspector_ID_Num > 0)
		if (!Label_Change(szRT_INSPECTORS_IDS, (const char **)pParams_Current->Setup.Inspection_Info.ppszInspector_ID, pParams_Current->Setup.Inspection_Info.uiInspector_ID_Num))
			bSuccess = FALSE;
	
	if (pParams_Current->Setup.Inspection_Info.uiInspector_ID_Num > 0)
		if (!Label_Change(szRT_INSPECTORS_IDS_CHOICES_NUM, pParams_Current->Setup.Inspection_Info.uiInspector_ID_Choices_Num))
			bSuccess = FALSE;
	
	if (pParams_Current->Setup.Inspection_Info.uiInspector_ID_Num > 0)
		if (pParams_Current->Setup.Inspection_Info.uiInspector_ID_Choices_Num > 0)
			if (!Label_Change(szRT_INSPECTORS_IDS_CHOICES, pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices, pParams_Current->Setup.Inspection_Info.uiInspector_ID_Choices_Num))
				bSuccess = FALSE;
	
	if (!Label_Change(szRT_INSPECTION_NUMBER, pParams_Current->Setup.Inspection_Info.pszInspection_Number))
		bSuccess = FALSE;
	
	if (!Label_Change(szRT_INSPECTION_COMMENTS, pParams_Current->Setup.Inspection_Info.pszComments))
		bSuccess = FALSE;

// OLD WAY OF SAVING
/*	if (strcmp(pParams_Current->Setup.pszFacility_Name, pParams_Default->Setup.pszFacility_Name) != 0)
		if (!Label_Change(szRT_FACILITY_NAME, pParams_Current->Setup.pszFacility_Name))
			bSuccess = FALSE;

	if ((pParams_Current->Setup.Inspection_Info.pReview_Date->yr != pParams_Default->Setup.Inspection_Info.pReview_Date->yr) ||
		(pParams_Current->Setup.Inspection_Info.pReview_Date->mon != pParams_Default->Setup.Inspection_Info.pReview_Date->mon) ||
		(pParams_Current->Setup.Inspection_Info.pReview_Date->day != pParams_Default->Setup.Inspection_Info.pReview_Date->day))
		if (!Label_Change(szRT_INSPECTION_DATE, pParams_Current->Setup.Inspection_Info.pReview_Date))
			bSuccess = FALSE;

	if ((pParams_Current->Setup.Inspection_Info.pReview_Time->hr != pParams_Default->Setup.Inspection_Info.pReview_Time->hr) ||
		(pParams_Current->Setup.Inspection_Info.pReview_Time->min != pParams_Default->Setup.Inspection_Info.pReview_Time->min) ||
		(pParams_Current->Setup.Inspection_Info.pReview_Time->sec != pParams_Default->Setup.Inspection_Info.pReview_Time->sec) ||
		(pParams_Current->Setup.Inspection_Info.pReview_Time->msec != pParams_Default->Setup.Inspection_Info.pReview_Time->msec))
		if (!Label_Change(szRT_INSPECTION_TIME, pParams_Current->Setup.Inspection_Info.pReview_Time))
			bSuccess = FALSE;

	if ((pParams_Current->Setup.Inspection_Info.pStart_Date->yr != pParams_Default->Setup.Inspection_Info.pStart_Date->yr) ||
		(pParams_Current->Setup.Inspection_Info.pStart_Date->mon != pParams_Default->Setup.Inspection_Info.pStart_Date->mon) ||
		(pParams_Current->Setup.Inspection_Info.pStart_Date->day != pParams_Default->Setup.Inspection_Info.pStart_Date->day))
		if (!Label_Change(szRT_INSPECTION_DATA_DATE_START, pParams_Current->Setup.Inspection_Info.pStart_Date))
			bSuccess = FALSE;

	if ((pParams_Current->Setup.Inspection_Info.pStart_Time->hr != pParams_Default->Setup.Inspection_Info.pStart_Time->hr) ||
		(pParams_Current->Setup.Inspection_Info.pStart_Time->min != pParams_Default->Setup.Inspection_Info.pStart_Time->min) ||
		(pParams_Current->Setup.Inspection_Info.pStart_Time->sec != pParams_Default->Setup.Inspection_Info.pStart_Time->sec) ||
		(pParams_Current->Setup.Inspection_Info.pStart_Time->msec != pParams_Default->Setup.Inspection_Info.pStart_Time->msec))
		if (!Label_Change(szRT_INSPECTION_DATA_TIME_START, pParams_Current->Setup.Inspection_Info.pStart_Time))
			bSuccess = FALSE;

	if ((pParams_Current->Setup.Inspection_Info.pEnd_Date->yr != pParams_Default->Setup.Inspection_Info.pEnd_Date->yr) ||
		(pParams_Current->Setup.Inspection_Info.pEnd_Date->mon != pParams_Default->Setup.Inspection_Info.pEnd_Date->mon) ||
		(pParams_Current->Setup.Inspection_Info.pEnd_Date->day != pParams_Default->Setup.Inspection_Info.pEnd_Date->day))
		if (!Label_Change(szRT_INSPECTION_DATA_DATE_END, pParams_Current->Setup.Inspection_Info.pEnd_Date))
			bSuccess = FALSE;

	if ((pParams_Current->Setup.Inspection_Info.pEnd_Time->hr != pParams_Default->Setup.Inspection_Info.pEnd_Time->hr) ||
		(pParams_Current->Setup.Inspection_Info.pEnd_Time->min != pParams_Default->Setup.Inspection_Info.pEnd_Time->min) ||
		(pParams_Current->Setup.Inspection_Info.pEnd_Time->sec != pParams_Default->Setup.Inspection_Info.pEnd_Time->sec) ||
		(pParams_Current->Setup.Inspection_Info.pEnd_Time->msec != pParams_Default->Setup.Inspection_Info.pEnd_Time->msec))
		if (!Label_Change(szRT_INSPECTION_DATA_TIME_END, pParams_Current->Setup.Inspection_Info.pEnd_Time))
			bSuccess = FALSE;

	if ((pParams_Current->Setup.Inspection_Info.uiLocations_Num != pParams_Default->Setup.Inspection_Info.uiLocations_Num) &&
		(!Label_Change(szRT_LOCATIONS_NUM, pParams_Current->Setup.Inspection_Info.uiLocations_Num)))
		bSuccess = FALSE;

	// Doesn't ever change, don't need to save
	//if (!Label_Change(szRT_LOCATIONS, (const char **)pParams_Current->Setup.Inspection_Info.ppszLocation_Names, pParams_Current->Setup.Inspection_Info.uiLocations_Num))
	//	bSuccess = FALSE;
	
	if ((pParams_Current->Setup.Inspection_Info.uiLocations_Choice != pParams_Default->Setup.Inspection_Info.uiLocations_Choice) &&
		(!Label_Change(szRT_LOCATION_CHOICE, (pParams_Current->Setup.Inspection_Info.uiLocations_Choice + 1))))
		bSuccess = FALSE;
	
	if ((pParams_Current->Setup.Inspection_Info.uiInspector_ID_Num != pParams_Default->Setup.Inspection_Info.uiInspector_ID_Num) &&
		(!Label_Change(szRT_INSPECTORS_NUM, pParams_Current->Setup.Inspection_Info.uiInspector_ID_Num)))
		bSuccess = FALSE;
	
	if (!Label_Change(szRT_INSPECTORS_IDS, (const char **)pParams_Current->Setup.Inspection_Info.ppszInspector_ID, pParams_Current->Setup.Inspection_Info.uiInspector_ID_Num))
		bSuccess = FALSE;
	
	if ((pParams_Current->Setup.Inspection_Info.uiInspector_ID_Choices_Num != pParams_Default->Setup.Inspection_Info.uiInspector_ID_Choices_Num) &&
		(!Label_Change(szRT_INSPECTORS_IDS_CHOICES_NUM, pParams_Current->Setup.Inspection_Info.uiInspector_ID_Choices_Num)))
		bSuccess = FALSE;
	
	if (!Label_Change(szRT_INSPECTOR_IDS_CHOICES, pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices, pParams_Current->Setup.Inspection_Info.uiInspector_ID_Choices_Num))
		bSuccess = FALSE;
	
	if ((strcmp(pParams_Current->Setup.Inspection_Info.pszInspection_Number, pParams_Default->Setup.Inspection_Info.pszInspection_Number) != 0) &&
		(!Label_Change(szRT_INSPECTION_NUMBER, pParams_Current->Setup.Inspection_Info.pszInspection_Number)))
		bSuccess = FALSE;
	
	if ((strcmp(pParams_Current->Setup.Inspection_Info.pszComments, pParams_Default->Setup.Inspection_Info.pszComments) != 0) &&
		(!Label_Change(szRT_INSPECTION_COMMENTS, pParams_Current->Setup.Inspection_Info.pszComments)))
		bSuccess = FALSE;
*/

	if (!Buffer_Changes(FALSE)) // Save changes to disk
		bSuccess = FALSE;


	else bSuccess = bSuccess && Load_Inspection_Info(pParams_Default);
    	

	// NOTE: Don't forget to change index numbers back (Decrease by one)
	for (ctIndex = 0; ctIndex < pParams_Current->Setup.Inspection_Info.uiInspector_ID_Choices_Num; ctIndex++)
		(pParams_Current->Setup.Inspection_Info.puiInspector_ID_Choices[ctIndex])--;


	// If set to get inspection info from the common, then re-open proper now
	// If do NOT want to use a different common, then do nothing
	if (bOpen_Inspec_File) {
		if (!Initialization_File_Open(Old_Filename.Get_sz(), bWrite_Access)) {
			RT_Init_Error(uiRT_ERROR_COMMON_INSPEC_SAVE);
			bSuccess = FALSE;
			}
		}


	return bSuccess;

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reset all to defaults
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTInit::Reset_Inspection_Info(void) {

	BOOL bSuccess = TRUE;
	                
	                       
    bSuccess = bSuccess && Reset_Facility_Name();
    bSuccess = bSuccess && Reset_Facility_Location();
    bSuccess = bSuccess && Reset_Inspection_Date();
    bSuccess = bSuccess && Reset_Inspection_Time();
    bSuccess = bSuccess && Reset_Start_Date();
    bSuccess = bSuccess && Reset_Start_Time();
    bSuccess = bSuccess && Reset_End_Date();
    bSuccess = bSuccess && Reset_End_Time();
    bSuccess = bSuccess && Reset_Inspector_IDs();
    bSuccess = bSuccess && Reset_Inspector_ID_Num(); // Must be after Reset_Inspector_IDs
    bSuccess = bSuccess && Reset_Inspector_ID_Choices();
    bSuccess = bSuccess && Reset_Inspector_ID_Choices_Num(); // Must be after Reset_Inspector_ID_Choices
    bSuccess = bSuccess && Reset_Inspection_Number();
    bSuccess = bSuccess && Reset_Inspection_Comments();


	return bSuccess;

}

	
BOOL RTInit::Reset_Parameters(void) {

	BOOL bSuccess = TRUE;
	                
	                       
 	bSuccess = bSuccess && Reset_My_Name();
 	bSuccess = bSuccess && Reset_My_Start_Mode();
 	bSuccess = bSuccess && Reset_Splash_Screen_Timeout_Seconds();
  	bSuccess = bSuccess && Reset_Pipe_My_Name();
 	bSuccess = bSuccess && Reset_Tools_Num();
	bSuccess = bSuccess && Reset_Pipe_Other_Names();
	bSuccess = bSuccess && Reset_Pipe_Timeouts();
 	bSuccess = bSuccess && Reset_Tool_Names();
 	bSuccess = bSuccess && Reset_Common_Database_Path();
    bSuccess = bSuccess && Reset_MIC_Inspec_Info_File_Use();
    bSuccess = bSuccess && Reset_MIC_Inspec_Info_Filename();
 	bSuccess = bSuccess && Reset_Tool_Exec_Names();
	bSuccess = bSuccess && Reset_Tool_Installed();
 	bSuccess = bSuccess && Reset_IR_Used();
 	bSuccess = bSuccess && Reset_IR_Number();
 	bSuccess = bSuccess && Reset_Data_Match_Error_Seconds();
 	bSuccess = bSuccess && Reset_Data_Match_Minimize();
 	bSuccess = bSuccess && Reset_Data_Match_Restore();
	bSuccess = bSuccess && Reset_Inspec_Info_Common_Filename();
	bSuccess = bSuccess && Reset_Inspection_Info();
	bSuccess = bSuccess && Reset_Data_Match_Tool();
	bSuccess = bSuccess && Reset_Data_Match_Display_Connect_Message();
	bSuccess = bSuccess && Reset_Data_Match_Never_Send_Location();
	bSuccess = bSuccess && Reset_Data_Match_Always_Send_Location();


	return bSuccess;

}


/////////////////////////////////////////////////
// Getting associated parameter member functions
/////////////////////////////////////////////////

BOOL RTInit::Get_Pipe_Other_Name(const char *pszTool_Name, const char **ppszPipe_Other_Name) {

	UINT ctIndex = 0;
	BOOL bFound = FALSE;
	

	if ((bParams_Loaded) && (pszTool_Name)) {
		while ((!bFound) && (ctIndex < Get_Tools_Num())) {
			if (strcmp(pszTool_Name, Get_Tool_Name(ctIndex)) == 0)
				bFound = TRUE;
			else
				ctIndex++;
			}
		}
				
	if (bFound)
		(*ppszPipe_Other_Name) = Get_Pipe_Other_Name(ctIndex);
	else
		(*ppszPipe_Other_Name) = NULL;

		
	return bFound;	

}


UINT RTInit::Get_Tool_Index_Number(const char *pszTool_Name) {

	UINT ctIndex = 0;
	BOOL bFound = FALSE;
	

	if ((bParams_Loaded) && (pszTool_Name)) {
		while ((!bFound) && (ctIndex < Get_Tools_Num())) {
			if (strcmp(pszTool_Name, Get_Tool_Name(ctIndex)) == 0)
				bFound = TRUE;
			else
				ctIndex++;
			}
		}
				   
				   
	return ctIndex;
}


UINT RTInit::Get_Pipe_Other_Index_Number(const char *pszPipe_Other_Name) {

	UINT ctIndex = 0;
	BOOL bFound = FALSE;
	

	if ((bParams_Loaded) && (pszPipe_Other_Name)) {
		while ((!bFound) && (ctIndex < Get_Tools_Num())) {
			if (strcmp(pszPipe_Other_Name, Get_Pipe_Other_Name(ctIndex)) == 0)
				bFound = TRUE;
			else
				ctIndex++;
			}
		}
				   

	if (bFound)				   
		return ctIndex;
	else
		return 0;
		
}


UINT RTInit::Get_Pipe_Timeout_Max(void) {

	UINT ctIndex = 0, uiWait_Time = 0;
	

	for (ctIndex = 0; ctIndex < Get_Tools_Num(); ctIndex++) {
		if (Get_Tool_Installed(ctIndex))
			uiWait_Time = max(uiWait_Time, Get_Pipe_Timeout(ctIndex));
		}


	return uiWait_Time;
		
}


/////////////////////////////////////////////////////////////////////////////////////
//  Useful parameter changing dialog box changes
/////////////////////////////////////////////////////////////////////////////////////

BOOL RTInit::RT_Init_Change_Inspection_Info(RESOURCE_ID Icon, BOOL bSet_To_New, BOOL bSave_Info, BOOL bReload_Inspec_Info) {

	SEB_IAEA_Inspection_Box *pInspection_Box;
	GUI_ACTION Result;


	// Allow inspection info to be changed even if load failed.  If load failed, then the inspection
	// information has just been cleared and just does not contain any info.  This allows the inspector
	// to continue (for printing, etc) even if there is a common inspection info file problem.
	if (bReload_Inspec_Info)
		Reload_Common_Inspec_Info();

	// Create the dialog box
	if ((pInspection_Box = new SEB_IAEA_Inspection_Box(Get_My_Name(), &pParams_Current->Setup.pszFacility_Name, 
							&(pParams_Current->Setup.Inspection_Info), Icon)) == NULL) {
		RT_Init_Error(uiRT_INIT_ERROR_NO_MEMORY);
    	}
    	
	Result = pInspection_Box->Display(bSet_To_New);
    delete pInspection_Box;
    CGUI_App::ProcessEvents();

	if (Result == GUI_CANCEL) {
     	return FALSE;
    	}
    else if ((Result == GUI_OK) && (bSave_Info)) {
    	if (!Save_Inspection_Info()) {
			RT_Init_Error(uiRT_INIT_ERROR_OPTIONS_SAVE_FAIL);
			// If did not save then just keep trying to continue
			}
		// If using a common inspection ini file then update myself now - do not care if it fails
		else {
			if (strcmp(pParams_Current->Setup.pszInspec_Info_Common_Filename, Initialization_File::pszInit_Name) != 0)
				Save_Inspection_Info(TRUE);  // Do not care if fails
			}
    	}


	return TRUE;

}


///////////////////////
// Delete the defaults
///////////////////////

void RTInit::RTInit_Delete_Params(RT_PARAMS *pParams) {

	if (pParams->pszMy_Name) {
		delete [] pParams->pszMy_Name;
		pParams->pszMy_Name = NULL;
		}
	if (pParams->pszMy_Start_Mode) {
		delete [] pParams->pszMy_Start_Mode;
		pParams->pszMy_Start_Mode = NULL;
		}
	if (pParams->Setup.pszInspec_Info_Common_Filename) {
		delete [] pParams->Setup.pszInspec_Info_Common_Filename;
		pParams->Setup.pszInspec_Info_Common_Filename = NULL;
		}
	if (pParams->ReanalyzeImport.pszMIC_Inspec_Info_Filename) {
		delete [] pParams->ReanalyzeImport.pszMIC_Inspec_Info_Filename;
		pParams->ReanalyzeImport.pszMIC_Inspec_Info_Filename = NULL;
		}
	Delete_Common_Inspection_Info(pParams);
	if (pParams->ReanalyzeImport.pszCommon_Database_Path) {
		delete [] pParams->ReanalyzeImport.pszCommon_Database_Path;
		pParams->ReanalyzeImport.pszCommon_Database_Path = NULL;
		}
	if (pParams->ReanalyzeImport.pszSummary_Data_Filename) {
		delete [] pParams->ReanalyzeImport.pszSummary_Data_Filename;
		pParams->ReanalyzeImport.pszSummary_Data_Filename = NULL;
		}
	Delete_2D_List(&pParams->Review.ppszTool_Name, pParams->Review.uiTools_Num);
	Delete_2D_List(&pParams->Review.ppszTool_Exec_Name, pParams->Review.uiTools_Num);
	if (pParams->Review.pbTool_Installed) {
		delete [] pParams->Review.pbTool_Installed;
		pParams->Review.pbTool_Installed = NULL;
		}
	if (pParams->Pipes.puiPipe_Timeout) {
		delete [] pParams->Pipes.puiPipe_Timeout;
		pParams->Pipes.puiPipe_Timeout = NULL;
		}
	if (pParams->Pipes.pszPipe_My_Name) {
		delete [] pParams->Pipes.pszPipe_My_Name;
		pParams->Pipes.pszPipe_My_Name = NULL;
		}
	Delete_2D_List(&pParams->Pipes.ppszPipe_Other_Name, pParams->Review.uiTools_Num);
	if (pParams->Review.pbData_Match_Tools) {
		delete [] pParams->Review.pbData_Match_Tools;
		pParams->Review.pbData_Match_Tools = NULL;
		}
	if (pParams->Review.pbData_Match_Display_Connect_Message) {
		delete [] pParams->Review.pbData_Match_Display_Connect_Message;
		pParams->Review.pbData_Match_Display_Connect_Message = NULL;
		}
	if (pParams->Review.pbData_Match_Never_Send_Location) {
		delete [] pParams->Review.pbData_Match_Never_Send_Location;
		pParams->Review.pbData_Match_Never_Send_Location = NULL;
		}
	if (pParams->Review.pbData_Match_Always_Send_Location) {
		delete [] pParams->Review.pbData_Match_Always_Send_Location;
		pParams->Review.pbData_Match_Always_Send_Location = NULL;
		}

}


void RTInit::Clear_Common_Inspection_Info(RT_PARAMS *pParams) {

	// Clear the inspection info
	// Try to keep this parameter if it is OK
	if (!pParams->Setup.pszFacility_Name) {
		pParams->Setup.pszFacility_Name = new char [1];
		pParams->Setup.pszFacility_Name[0] = NULL;
		}
	// Try to keep this parameter if it is OK
	if (!pParams->Setup.Inspection_Info.pszFacility_Location) {
		pParams->Setup.Inspection_Info.pszFacility_Location = new char [1];
		pParams->Setup.Inspection_Info.pszFacility_Location[0] = NULL;
		}
	// Try to keep this parameter if it is OK
	if (!pParams->Setup.Inspection_Info.pReview_Date) {
		pParams->Setup.Inspection_Info.pReview_Date = new GEN_DATE_STRUCT;
		// Causes to be set to current date
		pParams->Setup.Inspection_Info.pReview_Date->yr = 0;
		pParams->Setup.Inspection_Info.pReview_Date->mon = 0;
		pParams->Setup.Inspection_Info.pReview_Date->day = 0;
		}
	// Try to keep this parameter if it is OK
	if (!pParams->Setup.Inspection_Info.pReview_Time) {
		pParams->Setup.Inspection_Info.pReview_Time = new GEN_TIME_STRUCT;
		pParams->Setup.Inspection_Info.pReview_Time->hr = 0;
		pParams->Setup.Inspection_Info.pReview_Time->min = 0;
		pParams->Setup.Inspection_Info.pReview_Time->sec = 0;
		pParams->Setup.Inspection_Info.pReview_Time->msec = 0;
		}
	// Can't keep this one because don't know if Num failed
	Delete_2D_List(&pParams->Setup.Inspection_Info.ppszInspector_ID, pParams->Setup.Inspection_Info.uiInspector_ID_Num);
	pParams->Setup.Inspection_Info.uiInspector_ID_Num = 0;
	//ppszInspector_ID;				// 
	// Can't keep this one because don't know if Num failed
	if (pParams->Setup.Inspection_Info.puiInspector_ID_Choices) {
		delete [] pParams->Setup.Inspection_Info.puiInspector_ID_Choices;
		pParams->Setup.Inspection_Info.puiInspector_ID_Choices = NULL;
		}
	pParams->Setup.Inspection_Info.uiInspector_ID_Choices_Num = 0;
	//puiInspector_ID_Choices;		// Does not matter now - Does not matter if even NULL
	// Try to keep this parameter if it is OK
	if (!pParams->Setup.Inspection_Info.pszInspection_Number) {
		pParams->Setup.Inspection_Info.pszInspection_Number = new char [1];
		pParams->Setup.Inspection_Info.pszInspection_Number[0] = NULL;
		}
	// Try to keep this parameter if it is OK
	if (!pParams->Setup.Inspection_Info.pStart_Date) {
		pParams->Setup.Inspection_Info.pStart_Date = new GEN_DATE_STRUCT;
		// Causes to be set to current date
		pParams->Setup.Inspection_Info.pStart_Date->yr = 0;
		pParams->Setup.Inspection_Info.pStart_Date->mon = 0;
		pParams->Setup.Inspection_Info.pStart_Date->day = 0;
		}
	// Try to keep this parameter if it is OK
	if (!pParams->Setup.Inspection_Info.pStart_Time) {
		pParams->Setup.Inspection_Info.pStart_Time = new GEN_TIME_STRUCT;
		pParams->Setup.Inspection_Info.pStart_Time->hr = 0;
		pParams->Setup.Inspection_Info.pStart_Time->min = 0;
		pParams->Setup.Inspection_Info.pStart_Time->sec = 0;
		pParams->Setup.Inspection_Info.pStart_Time->msec = 0;
		}
	// Try to keep this parameter if it is OK
	if (!pParams->Setup.Inspection_Info.pEnd_Date) {
		pParams->Setup.Inspection_Info.pEnd_Date = new GEN_DATE_STRUCT;
		// Causes to be set to current date
		pParams->Setup.Inspection_Info.pEnd_Date->yr = 0;
		pParams->Setup.Inspection_Info.pEnd_Date->mon = 0;
		pParams->Setup.Inspection_Info.pEnd_Date->day = 0;
		}
	// Try to keep this parameter if it is OK
	if (!pParams->Setup.Inspection_Info.pEnd_Time) {
		pParams->Setup.Inspection_Info.pEnd_Time = new GEN_TIME_STRUCT;
		pParams->Setup.Inspection_Info.pEnd_Time->hr = 0;
		pParams->Setup.Inspection_Info.pEnd_Time->min = 0;
		pParams->Setup.Inspection_Info.pEnd_Time->sec = 0;
		pParams->Setup.Inspection_Info.pEnd_Time->msec = 0;
		}
	// Try to keep this parameter if it is OK
	if (!pParams->Setup.Inspection_Info.pszComments) {
		pParams->Setup.Inspection_Info.pszComments = new char [1];
		pParams->Setup.Inspection_Info.pszComments[0] = NULL;
		}

}


void RTInit::Delete_Common_Inspection_Info(RT_PARAMS *pParams) {

	if (pParams->Setup.pszFacility_Name) {
		delete [] pParams->Setup.pszFacility_Name;
		pParams->Setup.pszFacility_Name = NULL;
		}
	if (pParams->Setup.Inspection_Info.pszFacility_Location) {
		delete [] pParams->Setup.Inspection_Info.pszFacility_Location;
		pParams->Setup.Inspection_Info.pszFacility_Location = NULL;
		}
	if (pParams->Setup.Inspection_Info.pszInspection_Number) {
		delete [] pParams->Setup.Inspection_Info.pszInspection_Number;
		pParams->Setup.Inspection_Info.pszInspection_Number = NULL;
		}
	if (pParams->Setup.Inspection_Info.pReview_Date) {
		delete pParams->Setup.Inspection_Info.pReview_Date;
		pParams->Setup.Inspection_Info.pReview_Date = NULL;
		}
	if (pParams->Setup.Inspection_Info.pReview_Time) {
		delete pParams->Setup.Inspection_Info.pReview_Time;
		pParams->Setup.Inspection_Info.pReview_Time = NULL;
		}
	Delete_2D_List(&pParams->Setup.Inspection_Info.ppszInspector_ID, pParams->Setup.Inspection_Info.uiInspector_ID_Num);
	if (pParams->Setup.Inspection_Info.puiInspector_ID_Choices) {
		delete [] pParams->Setup.Inspection_Info.puiInspector_ID_Choices;
		pParams->Setup.Inspection_Info.puiInspector_ID_Choices = NULL;
		}
	if (pParams->Setup.Inspection_Info.pStart_Date) {
		delete pParams->Setup.Inspection_Info.pStart_Date;
		pParams->Setup.Inspection_Info.pStart_Date = NULL;
		}
	if (pParams->Setup.Inspection_Info.pStart_Time) {
		delete pParams->Setup.Inspection_Info.pStart_Time;
		pParams->Setup.Inspection_Info.pStart_Time = NULL;
		}
	if (pParams->Setup.Inspection_Info.pEnd_Date) {
		delete pParams->Setup.Inspection_Info.pEnd_Date;
		pParams->Setup.Inspection_Info.pEnd_Date = NULL;
		}
	if (pParams->Setup.Inspection_Info.pEnd_Time) {
		delete pParams->Setup.Inspection_Info.pEnd_Time;
		pParams->Setup.Inspection_Info.pEnd_Time = NULL;
		}
	if (pParams->Setup.Inspection_Info.pszComments) {
		delete [] pParams->Setup.Inspection_Info.pszComments;
		pParams->Setup.Inspection_Info.pszComments = NULL;
		}

}


//////////////////////////////////////////////////////////////////////////////////////////
// Use the default error messages below except when an error occurs specific to your tool
//////////////////////////////////////////////////////////////////////////////////////////

void RTInit::RT_Init_Error(uiRT_INIT_ERROR uiError, const char *pszError_Parameter_1, const char *pszError_Parameter_2) {

	switch(uiError) {
		case uiRT_INIT_ERROR_NO_MEMORY :
			RTError.Error(uiRT_ERROR_NO_MEMORY);
			break;               
		case uiRT_INIT_ERROR_OPTIONS_SAVE_FAIL :
			RTError.Error(uiRT_ERROR_OPTIONS_SAVE_FAIL);
			break;
		case uiRT_INIT_ERROR_OVERWRITE_RAWDATA :
			RTError.Error(uiRT_ERROR_OVERWRITE_RAWDATA);
			break;
		case uiRT_INIT_ERROR_COMMON_INSPEC_LOAD :
			RTError.Error(uiRT_ERROR_COMMON_INSPEC_LOAD, Label_Warning.Get_sz());
			break;
		case uiRT_INIT_ERROR_COMMON_INSPEC_SAVE :
			RTError.Error(uiRT_ERROR_COMMON_INSPEC_SAVE);
			break;
		default:
			RTError.Error(uiError, pszError_Parameter_1, pszError_Parameter_2);
			break;			
    	}

}
		

