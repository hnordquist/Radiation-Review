// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/12/97

// RTTitle.CPP
// Setup class definition for the abstract class Review Tool
       
       
#include "RTTitle.H"
	

/////////////////////////////////////////////////////////////////////////////////////
//  RTTitle constructor, destructor
/////////////////////////////////////////////////////////////////////////////////////

RTTitle::RTTitle(RESOURCE_ID Icon, const char *pszName_Version, const char *pszProgrammer_Email, BOOL bCreate_Title_As_App) :
	bCommon_Params_Set(FALSE),
	pCommon_Params(NULL),
	bCommon_Pipes_Set(FALSE),
	pCommon_Pipes(NULL),
	pSplash(NULL),
	pTitle_DlgApp(NULL),
	pTitle_Dlg(NULL),
	pCommon_Title(NULL),
	uiIcon(Icon),
	pszSplash_Title1(NULL),
	pszSplash_Title2(NULL),
	eStart_Operation(START_SETUP),
	bInspection_Info_Open(FALSE),
	bCreate_As_App(bCreate_Title_As_App),
	bSplash_Closed(TRUE)
{
		
	if (pszName_Version) {
		if ((pszSplash_Title1 = new char [strlen(pszName_Version) + 1]) != NULL)
			strcpy(pszSplash_Title1, pszName_Version);
		}
	if (pszProgrammer_Email) {
		if ((pszSplash_Title2 = new char [strlen(pszProgrammer_Email) + 1]) != NULL)
			strcpy(pszSplash_Title2, pszProgrammer_Email);
		}
		
}

 
RTTitle::~RTTitle(void) {

	RT_Title_Delete();
	RT_Splash_Delete();
		
}


/////////////////////////////////////////////////////////////////////
// Access to the common data items across all review tool operations
/////////////////////////////////////////////////////////////////////

BOOL RTTitle::Set_Common_Params(RTInit *const pRT_Common_Params) {
    
    pCommon_Params = pRT_Common_Params;
    bCommon_Params_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Name(pCommon_Params->Get_My_Name());
    
	return bCommon_Params_Set;
	
}


BOOL RTTitle::Set_Common_Pipes(RTPipes *const pRT_Commmon_Pipes) {

    pCommon_Pipes = pRT_Commmon_Pipes;
    bCommon_Pipes_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Pipes(pCommon_Pipes);

	return bCommon_Pipes_Set;

}


/////////////////////////////////////////////////////////////////////////////////////////////
// Displays the splash screen immediately
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL RTTitle::RT_Title_Splash_Display(UINT uiSplash_Screen_Timeout_Seconds) {

	if (pSplash) {
		return TRUE;
		}


	if (uiSplash_Screen_Timeout_Seconds != 0) {
		if ((pSplash = new CGUI_Dlg(IDD_RT_SPLASH, NULL, DLG_POSN_NO_SAVE)) == NULL) {
			RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
			return FALSE;
			}
		else {
			if (uiIcon) 
				pSplash->SetIcon(uiIcon);

			if (pszSplash_Title1)
				pSplash->DefineFldStaticTxt(IDC_RT_SPLASH_NAME_VERSION, pszSplash_Title1);
			if (pszSplash_Title2)
				pSplash->DefineFldStaticTxt(IDC_RT_SPLASH_PROGRAMMER_EMAIL, pszSplash_Title2);

			pSplash->SetTimeOut(uiSplash_Screen_Timeout_Seconds);
			// GUI.LIB FIX - Only global Exit validate functions work
			//               Must know when splash timesout and closes so that do not use splash dialog pointer later
			BOOL RT_Title_Exit_Validate(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);
			pSplash->SetDlgExitValidateFn(RT_Title_Exit_Validate);
			pSplash->SetUserData(this);

			pSplash->CGUI_Dlg::GoModeless(TRUE);
			bSplash_Closed = FALSE;
			}
		}


	return TRUE;

}


// GUI.LIB FIX - Only global Exit validate functions work
//               Must know when splash timesout and closes so that do not use splash dialog pointer later
BOOL RT_Title_Exit_Validate(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction) {

	// Must call a OPReview member function to access the necessary data
	// For some unknown reason, must get OP through user data or it GPFs
	RTTitle *pSplash = (RTTitle *)pDlg->GetUserData();
	if (pSplash)
		pSplash->bSplash_Closed = TRUE;
	return (TRUE);

}


BOOL RTTitle::RT_Title_Splash_Close(void) 
{
	try  //this will blow if the splash screen has already timed out
	{
		FlushEvents();
		CGUI_App::ProcessEvents();

		if (bSplash_Closed) return FALSE;

		if (pSplash == NULL) return FALSE;

		// GUI.LIB fix: Use these two WIN32 to check to see if splash got closed because the app got closed
		//              Must do this way because exit validates and control validates do not get called
		if ((IsWindow(pSplash->GetWndHandle())) && (IsWindowVisible(pSplash->GetWndHandle()))) 
		{
			pSplash->Close();
			FlushEvents();
			CGUI_App::ProcessEvents();
			return TRUE;
		}
		else 
			return FALSE;
	}
	catch(...)
	{
		return FALSE;
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////
// Creates and displays default\common review tool title screen
// NOTE: This class is used (this member functions called) only when this tool is
//       run individually or is the first tool to be run within a team of tools
// Return value is the next operation to be taken
// If you would like to display a different title screen or 
/////////////////////////////////////////////////////////////////////////////////////////////
RTTitle::START_OPERATION RTTitle::RT_Title_Begin(void) {

	eStart_Operation = START_EXIT;


	if (RT_Title_Create()) {

	 	RT_Title_Start_Screen_ON();

		// All pCommon_Title calls from now on MUST only be for CGUI_Dlg, if unsure, cast them.
		if (bCreate_As_App)
	 		pTitle_DlgApp->Go();
		else pTitle_Dlg->Go();

		// Don't let splash screen stay open beyond title screen - NOTE: Must be before deleting the app or dialog app
		RT_Title_Splash_Close();

		RT_Title_Delete();

		CGUI_App::ProcessEvents();
	 	}
    
    
	return (eStart_Operation);
	
}


/////////////////////////////////////////////////////////////////////////////////////////////
// The title dialog box creation and delation member functions
/////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTTitle::RT_Title_Create_App(const char *pszTool_Name) {

	if (bCreate_As_App) {
		// Return if already created
		if (pTitle_DlgApp) return TRUE;

		// Create the title dialog box
		if (pszTool_Name) {
			if ((pTitle_DlgApp = new CGUI_DlgApp(pszTool_Name, IDD_RT_TITLE, NULL, DLG_POSN_NO_SAVE)) == NULL) {
				RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
				return FALSE;
				}
			pTitle_DlgApp->SetTitle(pszTool_Name);
			}
		else {
			if ((pTitle_DlgApp = new CGUI_DlgApp("", IDD_RT_TITLE, NULL, DLG_POSN_NO_SAVE)) == NULL) {
				RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
				return FALSE;
				}
			pTitle_DlgApp->SetTitle("");
			}

		if (uiIcon) pTitle_DlgApp->SetIcon(uiIcon);
		
		// Keep a common title pointer for simplicity later
		pCommon_Title = pTitle_DlgApp;
		}

	else {
		// Return if already created
		if (pTitle_Dlg) return TRUE;

		// Create the title dialog box
		if (pszTool_Name) {
			if ((pTitle_Dlg = new CGUI_Dlg(IDD_RT_TITLE, NULL, DLG_POSN_NO_SAVE)) == NULL) {
				RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
				return FALSE;
				}
			pTitle_Dlg->SetTitle(pszTool_Name);
			}
		else {
			if ((pTitle_Dlg = new CGUI_Dlg(IDD_RT_TITLE, NULL, DLG_POSN_NO_SAVE)) == NULL) {
				RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
				return FALSE;
				}
			}

		if (uiIcon) pTitle_Dlg->SetIcon(uiIcon);

		// Keep a common title pointer for simplicity later
		pCommon_Title = (CGUI_DlgApp *)pTitle_Dlg;
		}

	CGUI_App::ProcessEvents();
	FlushEvents();


	return TRUE;

}



BOOL RTTitle::RT_Title_Create(void) {

	// Be sure it is already created
	if (!RT_Title_Create_App(pCommon_Params->Get_My_Name()))
		return FALSE;


	// GUI.LIB fix, will GPF if not exactly set for app or dlgapp
	if (bCreate_As_App) {
		// Don't set the icon here again, GUI.LIB GPF
		pTitle_DlgApp->SetTitle(pCommon_Params->Get_My_Name());
		// Setup the elements common to all title box screens
		pTitle_DlgApp->DefinePushBtn(IDC_RT_TITLE_CHANGE, GUI_NONE);
		pTitle_DlgApp->SetCtrlNotifyFn(IDC_RT_TITLE_CHANGE, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Change);
		pTitle_DlgApp->DefinePushBtn(IDC_RT_TITLE_CONTINUE, GUI_OK);
		pTitle_DlgApp->SetCtrlNotifyFn(IDC_RT_TITLE_CONTINUE, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Continue);
		pTitle_DlgApp->DefinePushBtn(IDC_RT_TITLE_NEW, GUI_OK);
		pTitle_DlgApp->SetCtrlNotifyFn(IDC_RT_TITLE_NEW, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_New);
		pTitle_DlgApp->DefinePushBtn(IDC_RT_TITLE_EXIT, GUI_CANCEL);
		pTitle_DlgApp->SetCtrlNotifyFn(IDC_RT_TITLE_EXIT, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Exit);
		// GUI.LIB FIX - Exit validate functions for an entire dialog do not accept a FALSE return value.  This is the only way to do it.
		pTitle_DlgApp->DefineFldTxt(IDC_RT_TITLE_EXIT_NOTIFY_FIX, "", 1);    
		// Do not disable or will not try to validate pTitle_DlgApp->SetEnable(IDC_RT_EXIT_NOTIFY_FIX, FALSE);
		pTitle_DlgApp->SetVisible(IDC_RT_TITLE_EXIT_NOTIFY_FIX, FALSE);
		pTitle_DlgApp->SetCtrlValidateFn(IDC_RT_TITLE_EXIT_NOTIFY_FIX, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgValidateFn)&RTTitle::RT_Title_Exit_Validate);
			
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_FACILITY_NAME);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_FACILITY_NAME);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_REVIEW_DATE_TIME);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_DATE);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_LOCATION);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_LOCATION);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_REVIEW_NUMBER);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_INSPECTORS);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_INSPECTORS); 
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_START_DATE_TIME);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_START_DATE);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_END_DATE_TIME);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_END_DATE);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_COMMENTS);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_COMMENTS);
		}
	else {
		// Don't set the icon here again, GUI.LIB GPF
		pTitle_Dlg->SetTitle(pCommon_Params->Get_My_Name());
		// Setup the elements common to all title box screens
		pTitle_Dlg->DefinePushBtn(IDC_RT_TITLE_CHANGE, GUI_NONE);
		pTitle_Dlg->SetCtrlNotifyFn(IDC_RT_TITLE_CHANGE, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Change);
		pTitle_Dlg->DefinePushBtn(IDC_RT_TITLE_CONTINUE, GUI_OK);
		pTitle_Dlg->SetCtrlNotifyFn(IDC_RT_TITLE_CONTINUE, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Continue);
		pTitle_Dlg->DefinePushBtn(IDC_RT_TITLE_NEW, GUI_OK);
		pTitle_Dlg->SetCtrlNotifyFn(IDC_RT_TITLE_NEW, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_New);
		pTitle_Dlg->DefinePushBtn(IDC_RT_TITLE_EXIT, GUI_CANCEL);
		pTitle_Dlg->SetCtrlNotifyFn(IDC_RT_TITLE_EXIT, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Exit);
		// GUI.LIB FIX - Exit validate functions for an entire dialog do not accept a FALSE return value.  This is the only way to do it.
		pTitle_Dlg->DefineFldTxt(IDC_RT_TITLE_EXIT_NOTIFY_FIX, "", 1);    
		// Do not disable or will not try to validate pTitle_Dlg->SetEnable(IDC_RT_EXIT_NOTIFY_FIX, FALSE);
		pTitle_Dlg->SetVisible(IDC_RT_TITLE_EXIT_NOTIFY_FIX, FALSE);
		pTitle_Dlg->SetCtrlValidateFn(IDC_RT_TITLE_EXIT_NOTIFY_FIX, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgValidateFn)&RTTitle::RT_Title_Exit_Validate);
			
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_FACILITY_NAME);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_FACILITY_NAME);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_REVIEW_DATE_TIME);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_DATE);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_LOCATION);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_LOCATION);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_REVIEW_NUMBER);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_INSPECTORS);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_INSPECTORS); 
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_START_DATE_TIME);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_START_DATE);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_END_DATE_TIME);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_END_DATE);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_COMMENTS);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_COMMENTS);
		}


    return TRUE;

}


void RTTitle::RT_Title_Delete(void) {

	if (bCreate_As_App) {
		if (pTitle_DlgApp) {
			delete pTitle_DlgApp;
			pTitle_DlgApp = NULL;
			}
		}
	else {
		if (pTitle_Dlg) {
			delete pTitle_Dlg;
			pTitle_Dlg = NULL;
			}
		}

	FlushEvents();  // Always perform after deleting a dialog app

}


void RTTitle::RT_Splash_Delete(void) {

	// Will delete itself
	RT_Title_Splash_Close();
	if (pszSplash_Title1) {
		delete [] pszSplash_Title1;
		pszSplash_Title1 = NULL;
		}
	if (pszSplash_Title2) {
		delete [] pszSplash_Title2;
		pszSplash_Title2 = NULL;
		}
	FlushEvents();  // Always perform after deleting a dialog app

}


/////////////////////////////////////////////////////////////////////////////////////////////
// The title dialog box screen displaying member functions
/////////////////////////////////////////////////////////////////////////////////////////////

void RTTitle::RT_Title_Start_Screen_ON(void) {

	CGStr Date_String, Time_String, String;
	UINT ctIndex;
	CFmtdDate Date;
	CFmtdTime Time;
	

    // Set the inspector information boxes
    pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_FACILITY_NAME, pCommon_Params->Get_Facility_Name());
    Date.Set(*pCommon_Params->Get_Inspection_Date());
	Date.Get(&Date_String);
    Time.Set(*pCommon_Params->Get_Inspection_Time());
	Time.Get(&Time_String);
	String = Date_String;
	String += "  -  ";
	String += Time_String;
    pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_REVIEW_DATE_TIME, String.Get_sz());
	pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_LOCATION, pCommon_Params->Get_Facility_Location());
    pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_REVIEW_NUMBER, pCommon_Params->Get_Inspection_Number());
	if (pCommon_Params->Get_Inspector_ID_Num() > 0) {
		String = "";
		for (ctIndex = 0 ; ctIndex < pCommon_Params->Get_Inspector_ID_Choices_Num(); ctIndex++) {
    		String += pCommon_Params->Get_Inspector_ID(pCommon_Params->Get_Inspector_ID_Choice(ctIndex));
    		if (ctIndex < (pCommon_Params->Get_Inspector_ID_Choices_Num() - 1))
    			String += ", ";
    		}
		pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_INSPECTORS, String.Get_sz());
		}
    Date.Set(*pCommon_Params->Get_Start_Date());
	Date.Get(&Date_String);
    Time.Set(*pCommon_Params->Get_Start_Time());
	Time.Get(&Time_String);
	String = Date_String;
	String += "  -  ";
	String += Time_String;
    pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_START_DATE_TIME, String.Get_sz());
    Date.Set(*pCommon_Params->Get_End_Date());
	Date.Get(&Date_String);
    Time.Set(*pCommon_Params->Get_End_Time());
	Time.Get(&Time_String);
	String = Date_String;
	String += "  -  ";
	String += Time_String;
    pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_END_DATE_TIME, String.Get_sz());
    pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_COMMENTS, pCommon_Params->Get_Inspection_Comments());


	// Turn all of the appropriate controls on    
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_FACILITY_NAME, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_FACILITY_NAME, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_FACILITY_NAME, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_FACILITY_NAME, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_DATE, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_DATE, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_REVIEW_DATE_TIME, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_REVIEW_DATE_TIME, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_LOCATION, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_LOCATION, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_LOCATION, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_LOCATION, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_REVIEW_NUMBER, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_REVIEW_NUMBER, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_INSPECTORS, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_INSPECTORS, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_INSPECTORS, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_INSPECTORS, TRUE); 
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_START_DATE, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_START_DATE, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_START_DATE_TIME, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_START_DATE_TIME, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_END_DATE, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_END_DATE, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_END_DATE_TIME, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_END_DATE_TIME, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_COMMENTS, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_COMMENTS, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_COMMENTS, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_COMMENTS, TRUE);   
    
	// Keep last, prevents hanging
	pCommon_Title->SetVisible(IDC_RT_TITLE_CHANGE, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CHANGE, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_CONTINUE, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CONTINUE, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_NEW, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_NEW, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_EXIT, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_EXIT, TRUE);

}


void RTTitle::RT_Title_Start_Screen_OFF(void) {

	pCommon_Title->SetVisible(IDC_RT_TITLE_CHANGE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CHANGE, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_CONTINUE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CONTINUE, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_NEW, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_NEW, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_EXIT, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_EXIT, FALSE);
    
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_FACILITY_NAME, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_FACILITY_NAME, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_FACILITY_NAME, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_FACILITY_NAME, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_DATE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_DATE, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_REVIEW_DATE_TIME, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_REVIEW_DATE_TIME, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_LOCATION, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_LOCATION, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_LOCATION, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_LOCATION, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_REVIEW_NUMBER, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_REVIEW_NUMBER, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_INSPECTORS, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_INSPECTORS, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_INSPECTORS, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_INSPECTORS, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_START_DATE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_START_DATE, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_START_DATE_TIME, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_START_DATE_TIME, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_END_DATE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_END_DATE, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_END_DATE_TIME, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_END_DATE_TIME, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_COMMENTS, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_COMMENTS, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_COMMENTS, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_COMMENTS, FALSE);   
    
}

/////////////////////////////////////////////////////////////////////////////////////////////
// The title dialog box button member functions
/////////////////////////////////////////////////////////////////////////////////////////////

void RTTitle::RT_Title_Change(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {
	
	pCommon_Pipes->Set_Messages_On_Hold(TRUE);

	pCommon_Title->SetEnable(IDC_RT_TITLE_CHANGE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CONTINUE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_NEW, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_EXIT, FALSE);

	bInspection_Info_Open = TRUE;
	if (pCommon_Params->RT_Init_Change_Inspection_Info(uiIcon)) {
		RT_Title_Start_Screen_ON();
	    CGUI_App::ProcessEvents();
		}
	else {
		pCommon_Title->SetEnable(IDC_RT_TITLE_CHANGE, TRUE);
		pCommon_Title->SetEnable(IDC_RT_TITLE_CONTINUE, TRUE);
		pCommon_Title->SetEnable(IDC_RT_TITLE_NEW, TRUE);
		pCommon_Title->SetEnable(IDC_RT_TITLE_EXIT, TRUE); 
		}
	bInspection_Info_Open = FALSE;

	pCommon_Pipes->Set_Messages_On_Hold(FALSE);
		
}


void RTTitle::RT_Title_Continue(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	eStart_Operation = START_REVIEW;

}


void RTTitle::RT_Title_New(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	BOOL bInspection_Info_Open, bMIC_Inspec_Info_Loaded = FALSE;


	pCommon_Pipes->Set_Messages_On_Hold(TRUE);


	pCommon_Title->SetEnable(IDC_RT_TITLE_CHANGE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CONTINUE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_NEW, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_EXIT, FALSE);

	// Check to see if should default inspection parameters to a mic inspection information file
	if (pCommon_Params->Get_MIC_Inspec_Info_File_Use()) {
		bMIC_Inspec_Info_Loaded = RT_Title_Load_MIC_Inspection_Information();
		}

	bInspection_Info_Open = TRUE;
	if (pCommon_Params->RT_Init_Change_Inspection_Info(uiIcon, ((!pCommon_Params->Get_MIC_Inspec_Info_File_Use()) || (!bMIC_Inspec_Info_Loaded)), FALSE, FALSE)) {
		eStart_Operation = START_IMPORT;
		}
	else {
		// If used MIC inspection information parameters above, then change them back by resetting each one
		pCommon_Params->Reset_Inspection_Info();
		eStart_Operation = START_TITLE;
		// GUI.LIB BUG - Seems to lose the change button and thinks it is an exit button
		}
	bInspection_Info_Open = FALSE;


	pCommon_Pipes->Set_Messages_On_Hold(FALSE);

}


BOOL RTTitle::RT_Title_Load_MIC_Inspection_Information(void) {

	CGUI_Dlg *pMIC_Load_Inspec_Info = NULL;
	CGStr MIC_Inspec_Info_Title, MIC_Full_File_Name;
	BOOL bMIC_Parameters_Loaded = FALSE;
	char *pszMIC_Facility = NULL, *pszMIC_Previous = NULL, *pszMIC_Date_Time = NULL, *pszMIC_Inspector_Name = NULL, *pszMIC_Inspector_ID = NULL, *pszMIC_Inspection_Number = NULL, *pszMIC_Full_Filename = NULL;
	char *ppszAsci_Month[12] = {"January","February","March", "April","May","June", "July","August","September", "October","November","December"};
	char *pszCurrent_Asci_Month;
	GEN_DATE_STRUCT Temp_Date_Struct;
	GEN_TIME_STRUCT Temp_Time_Struct;
	time_t Time;
	struct tm *Time_Struct;
	UINT ctInspector_ID, ctInspector_Choice;
	BOOL bInspector_ID_Already_Added;
	char **ppszNew_Inspector_IDs = NULL;
	UINT puiTemp_Inspector_ID_Choices[1] = {0};
	CGStr New_Inspector_Name_ID;


	bMIC_Parameters_Loaded = FALSE;


	// Create the memory for the MIC parameters first
	if ((NULL == (pszMIC_Facility = new (char [100]))) ||
		(NULL == (pszMIC_Previous = new (char [100]))) ||
		(NULL == (pszMIC_Date_Time = new (char [100]))) ||
		(NULL == (pszMIC_Inspector_Name = new (char [100]))) ||
		(NULL == (pszMIC_Inspector_ID = new (char [100]))) ||
		(NULL == (pszMIC_Inspection_Number = new (char [100]))) ||
		(NULL == (pszMIC_Full_Filename = new (char [strlen(pCommon_Params->Get_MIC_Inspec_Info_Filename()) + 1])))) {
		if (pszMIC_Facility) delete [] pszMIC_Facility;
		if (pszMIC_Previous) delete [] pszMIC_Previous;
		if (pszMIC_Date_Time) delete [] pszMIC_Date_Time;
		if (pszMIC_Inspector_Name) delete [] pszMIC_Inspector_Name;
		if (pszMIC_Inspector_ID) delete [] pszMIC_Inspector_ID;
		if (pszMIC_Inspection_Number) delete [] pszMIC_Inspection_Number;
		if (pszMIC_Full_Filename) delete [] pszMIC_Full_Filename;
		RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
		return FALSE;
		}


	// Check that the file exists.  Since it could be *.ip, check this way in order to expand the * into
	//    the first filename in the specified directory with the extension.	
	HANDLE MIC_File_Handle;
	WIN32_FIND_DATA MIC_File_Info;
	if (INVALID_HANDLE_VALUE == (MIC_File_Handle = FindFirstFile(pCommon_Params->Get_MIC_Inspec_Info_Filename(), &MIC_File_Info))) 
		RT_Title_Error(uiRT_TITLE_ERROR_MIC_INSPECTION_INFORMATION_FILE, pCommon_Params->Get_MIC_Inspec_Info_Filename());
	else {
		// Close the file search handle
		FindClose(MIC_File_Handle);
		// Build the full MIC info filename, add the path (if it exists) and then the full file name
		MIC_Full_File_Name = "";
		strcpy(pszMIC_Full_Filename, pCommon_Params->Get_MIC_Inspec_Info_Filename());
		if (NULL != strrchr(pszMIC_Full_Filename, '\\')) {
			(strrchr(pszMIC_Full_Filename, '\\'))[1] = NULL;  // The [1] is OK because if found then next character must at least be NULL
			MIC_Full_File_Name += pszMIC_Full_Filename;
			}
		MIC_Full_File_Name += MIC_File_Info.cFileName;
		// Create the dialog box for loading MIC parameters
		if (NULL == (pMIC_Load_Inspec_Info = new CGUI_Dlg(IDD_RT_MIC_LOAD_INSPEC_INFO, NULL, DLG_POSN_NO_SAVE)))
			RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
		else {
			if (uiIcon) pMIC_Load_Inspec_Info->SetIcon(uiIcon);
			MIC_Inspec_Info_Title = pCommon_Params->Get_My_Name();
			MIC_Inspec_Info_Title += " - Loading MIC Inspection Information";
			pMIC_Load_Inspec_Info->SetTitle(MIC_Inspec_Info_Title.Get_sz());
			pMIC_Load_Inspec_Info->DefinePL_MultiSel(IDC_RT_MIC_INSPEC_INFO_LIST);
			// Do not disable so that it will do everything it needs to pMIC_Load_Inspec_Info->SetEnable(IDC_RT_MIC_INSPEC_INFO_LIST, FALSE);
			pMIC_Load_Inspec_Info->SetVisible(IDC_RT_MIC_INSPEC_INFO_LIST, FALSE);
			pMIC_Load_Inspec_Info->GoModeless(FALSE);
			// Get each one of the MIC parameters
			// If found, set as new inspection parameters
			// If not found (NULL returned because copied default empty parameter) then do nothing
			if ((NULL != GetPrivateProfileString("GENERAL", "FACILITY", "", pszMIC_Facility, 100, MIC_Full_File_Name.Get_sz())) &&
				(NULL != GetPrivateProfileString("GENERAL", "PREVIOUS", "", pszMIC_Previous, 100, MIC_Full_File_Name.Get_sz())) &&
				(NULL != GetPrivateProfileString("GENERAL", "DATE_TIME", "", pszMIC_Date_Time, 100, MIC_Full_File_Name.Get_sz())) &&
				(NULL != GetPrivateProfileString("GENERAL", "INSPECTOR_NAME", "", pszMIC_Inspector_Name, 100, MIC_Full_File_Name.Get_sz())) &&
				(NULL != GetPrivateProfileString("GENERAL", "INSPECTOR_ID", "", pszMIC_Inspector_ID, 100, MIC_Full_File_Name.Get_sz())) &&
				(NULL != GetPrivateProfileString("GENERAL", "INSPECTION_NUMBER", "", pszMIC_Inspection_Number, 100, MIC_Full_File_Name.Get_sz()))) {

				// All of the parameters were found, set them for the new inspection
				bMIC_Parameters_Loaded = TRUE;


				///////////////////////////////////////////////////////////////////////////////////
				// Set the facility name
				///////////////////////////////////////////////////////////////////////////////////
				// This version does not allow the facility name to be changed
				//pCommon_Params->Set_Facility_Name(pszMIC_Facility);


				///////////////////////////////////////////////////////////////////////////////////
				// Set the current inspection date and time
				///////////////////////////////////////////////////////////////////////////////////
				time(&Time);
				Time_Struct = localtime(&Time);
				Temp_Date_Struct.yr = Time_Struct->tm_year; // Y2K compliant, will take years above 99
				Temp_Date_Struct.mon = Time_Struct->tm_mon + 1;
				Temp_Date_Struct.day = Time_Struct->tm_mday;
				Temp_Time_Struct.hr = Time_Struct->tm_hour;
				Temp_Time_Struct.min = Time_Struct->tm_min;
				Temp_Time_Struct.sec = Time_Struct->tm_sec;
				Temp_Time_Struct.msec = 0;
				pCommon_Params->Set_Inspection_Date(&Temp_Date_Struct);
				pCommon_Params->Set_Inspection_Time(&Temp_Time_Struct);


				///////////////////////////////////////////////////////////////////////////////////
				// Set the inspection number
				///////////////////////////////////////////////////////////////////////////////////
				pCommon_Params->Set_Inspection_Number(pszMIC_Inspection_Number);


				///////////////////////////////////////////////////////////////////////////////////
				// Set the inspector names and selected
				///////////////////////////////////////////////////////////////////////////////////
				// Determine if the inspectors name has already been added
				bInspector_ID_Already_Added = FALSE;
				ctInspector_ID = 0;
				while ((!bInspector_ID_Already_Added) && (ctInspector_ID < pCommon_Params->Get_Inspector_ID_Num())) {
					if (NULL == strnicmp(pszMIC_Inspector_Name, pCommon_Params->Get_Inspector_ID(ctInspector_ID), strlen(pszMIC_Inspector_Name)))
						bInspector_ID_Already_Added = TRUE;
					ctInspector_ID++;
					}

				// If not already there, add the MIC entered inspector ID and clear all choices but this one
				if (!bInspector_ID_Already_Added) {
					// Use the temporary list box to perform the sort for the newly added inspector ID
					for (ctInspector_ID = 0; ctInspector_ID < pCommon_Params->Get_Inspector_ID_Num(); ctInspector_ID++)
						pMIC_Load_Inspec_Info->PL_MultiSelAdd(IDC_RT_MIC_INSPEC_INFO_LIST, pCommon_Params->Get_Inspector_ID(ctInspector_ID));
					New_Inspector_Name_ID = pszMIC_Inspector_Name;
					New_Inspector_Name_ID += " - ";
					New_Inspector_Name_ID += pszMIC_Inspector_ID;
					pMIC_Load_Inspec_Info->PL_MultiSelAdd(IDC_RT_MIC_INSPEC_INFO_LIST, New_Inspector_Name_ID.Get_sz());
					// Get the new sorted list - do not handle if it does not create
					if (ppszNew_Inspector_IDs = new (char (* ([pCommon_Params->Get_Inspector_ID_Num() + 1])))) { // + 1 for new ID
						for (ctInspector_ID = 0; ctInspector_ID < pCommon_Params->Get_Inspector_ID_Num() + 1; ctInspector_ID++) {
							pMIC_Load_Inspec_Info->PL_MultiSelGetValue(IDC_RT_MIC_INSPEC_INFO_LIST, ctInspector_ID, &bInspector_ID_Already_Added, &New_Inspector_Name_ID);
							if ((ppszNew_Inspector_IDs[ctInspector_ID] = new char [strlen(New_Inspector_Name_ID.Get_sz()) + 1]) != NULL)
								strcpy(ppszNew_Inspector_IDs[ctInspector_ID], New_Inspector_Name_ID.Get_sz());
							}
						// Set the new inspection IDs
						pCommon_Params->Set_Inspector_IDs((const char**)ppszNew_Inspector_IDs, pCommon_Params->Get_Inspector_ID_Num() + 1); // + 1 for new ID
						// Delete the temporary inspection IDs
						SEB_DELETELISTSTRING(ppszNew_Inspector_IDs, pCommon_Params->Get_Inspector_ID_Num(), ctInspector_ID); // Get_Inspector_ID_Num now set to correct number
						}
					}

				// Clear all choices but the one which was entered through MIC
				bInspector_ID_Already_Added = FALSE;
				ctInspector_Choice = 0;
				while ((!bInspector_ID_Already_Added) && (ctInspector_Choice < pCommon_Params->Get_Inspector_ID_Num())) {
					if (NULL == strnicmp(pszMIC_Inspector_Name, pCommon_Params->Get_Inspector_ID(ctInspector_Choice), strlen(pszMIC_Inspector_Name)))
						bInspector_ID_Already_Added = TRUE;
					else
						ctInspector_Choice++;;
					}
				puiTemp_Inspector_ID_Choices[0] = ctInspector_Choice;
				pCommon_Params->Set_Inspector_ID_Choices(puiTemp_Inspector_ID_Choices, 1);


				///////////////////////////////////////////////////////////////////////////////////
				// Set the review end date and time for the data range
				///////////////////////////////////////////////////////////////////////////////////
				strtok(pszMIC_Date_Time, ", :"); // Get past day of week
				Temp_Date_Struct.day = atoi(strtok(NULL, ", :")); // Get integer day of week
				// Get integer month
				pszCurrent_Asci_Month = strtok(NULL, ", :");
				for (Temp_Date_Struct.mon = 1; ((stricmp(pszCurrent_Asci_Month, ppszAsci_Month[Temp_Date_Struct.mon - 1]) != 0) && (Temp_Date_Struct.mon <= 12)); Temp_Date_Struct.mon++);
				Temp_Date_Struct.yr = atoi(strtok(NULL, ", :")); // Get integer year
				Temp_Time_Struct.hr = atoi(strtok(NULL, ", :")); // Get integer hour
				Temp_Time_Struct.min = atoi(strtok(NULL, ", :")); // Get integer minute
				Temp_Time_Struct.sec = atoi(strtok(NULL, ", :")); // Get integer second
				pCommon_Params->Set_End_Date(&Temp_Date_Struct);
				pCommon_Params->Set_End_Time(&Temp_Time_Struct);

				///////////////////////////////////////////////////////////////////////////////////
				// Set the review start date and time for the data range
				///////////////////////////////////////////////////////////////////////////////////
				strtok(pszMIC_Previous, ", :"); // Get past day of week
				Temp_Date_Struct.day = atoi(strtok(NULL, ", :")); // Get integer day of week
				// Get integer month
				pszCurrent_Asci_Month = strtok(NULL, ", :");
				for (Temp_Date_Struct.mon = 1; ((stricmp(pszCurrent_Asci_Month, ppszAsci_Month[Temp_Date_Struct.mon - 1]) != 0) && (Temp_Date_Struct.mon <= 12)); Temp_Date_Struct.mon++);
				Temp_Date_Struct.yr = atoi(strtok(NULL, ", :")); // Get integer year
				Temp_Time_Struct.hr = atoi(strtok(NULL, ", :")); // Get integer hour
				Temp_Time_Struct.min = atoi(strtok(NULL, ", :")); // Get integer minute
				Temp_Time_Struct.sec = atoi(strtok(NULL, ", :")); // Get integer second
				pCommon_Params->Set_Start_Date(&Temp_Date_Struct);
				pCommon_Params->Set_Start_Time(&Temp_Time_Struct);

				///////////////////////////////////////////////////////////////////////////////////
				// Set the comments to blank	
				///////////////////////////////////////////////////////////////////////////////////
				pCommon_Params->Set_Inspection_Comments("");

				// Wait a little bit so that it does not flash so fast
				CGUI_App::Wait(0.5);
				//pMIC_Load_Inspec_Info->Close();
				}

			else { // All of the MIC parameters were not found, therefore warn the user and use the normal defaults
				// Wait a little bit so that it does not flash so fast
				CGUI_App::Wait(0.5);
				pMIC_Load_Inspec_Info->Close();
				bMIC_Parameters_Loaded = FALSE;
				RT_Title_Error(uiRT_TITLE_ERROR_MIC_INSPECTION_INFORMATION_FILE, pCommon_Params->Get_MIC_Inspec_Info_Filename());
				}

			// Delete the dialog box for loading MIC parameters
			if (pMIC_Load_Inspec_Info) {
				delete pMIC_Load_Inspec_Info;
				pMIC_Load_Inspec_Info = NULL;
				}
			}
		}


	if (pszMIC_Facility) {
		delete [] pszMIC_Facility;
		pszMIC_Facility = NULL;
		}
	if (pszMIC_Previous) {
		delete [] pszMIC_Previous;
		pszMIC_Previous = NULL;
		}
	if (pszMIC_Date_Time) {
		delete [] pszMIC_Date_Time;
		pszMIC_Date_Time = NULL;
		}
	if (pszMIC_Inspector_Name) {
		delete [] pszMIC_Inspector_Name;
		pszMIC_Inspector_Name = NULL;
		}
	if (pszMIC_Inspector_ID) {
		delete [] pszMIC_Inspector_ID;
		pszMIC_Inspector_ID = NULL;
		}
	if (pszMIC_Inspection_Number) {
		delete [] pszMIC_Inspection_Number;
		pszMIC_Inspection_Number = NULL;
		}
	if (pszMIC_Full_Filename) {
		delete [] pszMIC_Full_Filename;
		pszMIC_Full_Filename = NULL;
		}


	return bMIC_Parameters_Loaded;

}


void RTTitle::RT_Title_Exit(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	eStart_Operation = START_EXIT;
	
}


BOOL RTTitle::RT_Title_Exit_Validate(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	if (eStart_Operation != START_TITLE)
		// Exit to the new mode
		return TRUE;
	else {
		// Don't exit, just reset the title screen
		RT_Title_Start_Screen_ON();
	    CGUI_App::ProcessEvents();
		return FALSE;
		}

}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Provide the Title Exit procedure if necessary
///////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTTitle::RT_Title_Exit(void) {

	// Close all of the dialog boxes which are open
	if (bInspection_Info_Open) {
		// FIX: pointer to dialog box not within scope
		}

	if (bCreate_As_App)
	 	pTitle_DlgApp->CGUI_Dlg::Close();
	else pTitle_Dlg->Close();

	eStart_Operation = START_EXIT;
	
	
	return TRUE;
	
}


//////////////////////////////////////////////////////////////////////////////////////////
// Use the default error messages below except when an error occurs specific to your tool
//////////////////////////////////////////////////////////////////////////////////////////

void RTTitle::RT_Title_Error(uiRT_TITLE_ERROR uiError, const char *pszError_Parameter_1) {

	CGStr Message;


	switch(uiError) {
		case uiRT_TITLE_ERROR_NO_MEMORY :
			RTError.Error(uiRT_ERROR_NO_MEMORY);
			break;
		case uiRT_TITLE_ERROR_INSPEC_CHANGE :
			RTError.Error(uiRT_ERROR_INSPEC_CHANGE);
			break;
		case uiRT_TITLE_ERROR_MIC_INSPECTION_INFORMATION_FILE :
			Message = "The inspection information file \"";
			Message += pszError_Parameter_1;
			Message += "\" which contains the inspection information entered within Multi-Instrument Collect could not be properly read.  ";
			Message += "The inspection information for the new inspection will be defaulted according to the last inspection.";
			GUI_MsgBox(Message.Get_sz());
			break;
		default:
			RTError.Error(uiError, pszError_Parameter_1);
			break;			
    	}

}
		
