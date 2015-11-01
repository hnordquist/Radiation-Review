// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/18/97

// SEBBoxes.CPP
// Class definition for the various generic dialog boxes I have found to useful



#include "SEBBoxes.H"


////////////////////////////////////////////////////////////////////////////////////
// SEB_Modeless_Message dialog box
////////////////////////////////////////////////////////////////////////////////////
// Creates and handles multiple modeless message boxes
//    Simply create a SEB_Modeless_Message object and repeatively call Go to
//       create a new message box.
//    User can call Close_All to close all of the opened messages or delete the
//       SEB_Modeless_Message object which automatically closes and cleans up
//       after itself.
////////////////////////////////////////////////////////////////////////////////////    								

// SEB_Modeless_Message constructors and destructors
SEB_Modeless_Message::SEB_Modeless_Message(const char *pszBox_Title,
											RESOURCE_ID Icon) :
											// Name of application to appear in title bar
											// The application icon to appear as the system menu (Windows 95)
	pszMsg(NULL),
	pszUse(NULL),
	pszTitle(NULL),
	uiIcon(Icon)
	{
	
	UINT ctIndex = 0;
	    

	for (ctIndex = 0; ctIndex < uiMAX_SEB_BOXES; ctIndex++)
		pBoxes[ctIndex].bOpen = FALSE;	
	
    if (pszBox_Title)
		if ((pszTitle = new char [strlen(pszBox_Title) + 1]) != NULL)
			strcpy(pszTitle, pszBox_Title);	
	    
}


SEB_Modeless_Message::SEB_Modeless_Message(const char *pszBox_Title,
											const char *pszMessage,
											RESOURCE_ID Icon) :
											// Name of application to appear in title bar
											// Message to appear in the box
											// The application icon to appear as the system menu (Windows 95)
	pszMsg(NULL),
	pszUse(NULL),
	pszTitle(NULL),
	uiIcon(Icon)
	{

	UINT ctIndex = 0;
	    
	    
	for (ctIndex = 0; ctIndex < uiMAX_SEB_BOXES; ctIndex++)
		pBoxes[ctIndex].bOpen = FALSE;	

    if (pszBox_Title)
		if ((pszTitle = new char [strlen(pszBox_Title) + 1]) != NULL)
			strcpy(pszTitle, pszBox_Title);	
	    
    Set_Message(pszMessage);
    
}


SEB_Modeless_Message::~SEB_Modeless_Message(void) {

	Close_All();  
	if (pszMsg)
		delete [] pszMsg;
	if (pszTitle)
		delete [] pszTitle;

}


// SEB_Modeless_Message starting member functions
BOOL SEB_Modeless_Message::New(BOOL bUse_Small_Question_Box) {

	UINT ctIndex = 0;
	BOOL bSlot_Found = FALSE, bSuccess = FALSE;


	// Find an open slot for the box
	while ((!bSlot_Found) && (ctIndex < uiMAX_SEB_BOXES)) {
		if (!pBoxes[ctIndex].bOpen)
			bSlot_Found = TRUE;
		else
			ctIndex++;
		}
	if (!bSlot_Found) return FALSE;
	
	// Create the box
	if ((bUse_Small_Question_Box && ((pBoxes[ctIndex].pBox = new CGUI_Dlg(IDD_SEB_MODELESS_BOX, NULL, DLG_POSN_NO_SAVE)) != NULL)) ||
		(!bUse_Small_Question_Box && ((pBoxes[ctIndex].pBox = new CGUI_Dlg(IDD_SEB_MODELESS_BOX_LARGE, NULL, DLG_POSN_NO_SAVE)) != NULL))) {
		if (uiIcon)
			pBoxes[ctIndex].pBox->SetIcon(uiIcon);
		pBoxes[ctIndex].pBox->SetTitle(pszTitle);
		pBoxes[ctIndex].pBox->DefineFldStaticTxt(IDC_SEB_MODELESS_BOX_MESSAGE);
		pBoxes[ctIndex].pBox->FldStaticTxtSetValue(IDC_SEB_MODELESS_BOX_MESSAGE, pszUse);
		pBoxes[ctIndex].pBox->DefinePushBtn(IDC_SEB_MODELESS_BOX_OK, GUI_OK);
		pBoxes[ctIndex].pBox->SetDlgExitValidateFn((CGUI_CallBack *)((void *)this), 
													(GUI_CB_DlgExitFn)&SEB_Modeless_Message::Modeless_Exit);
	    pBoxes[ctIndex].bOpen = TRUE;
	    
		// Start the box
		pBoxes[ctIndex].pBox->GoModeless(TRUE);

		bSuccess = TRUE;
		}

	// Reset back to default message	
	pszUse = pszMsg;
	
	
	return bSuccess;
	
}


BOOL SEB_Modeless_Message::New(const char *pszMessage, BOOL bSet_Message, BOOL bUse_Small_Question_Box) {

	if (bSet_Message)
		if (!Set_Message(pszMessage))
			return FALSE;
	
	pszUse = pszMessage;		
	return New(bUse_Small_Question_Box);

}


// SEB_Question_Box closing member functions
BOOL SEB_Modeless_Message::Close_All(void) {

	UINT ctIndex = 0;
	BOOL bSuccess = TRUE;


	for (ctIndex = 0; ctIndex < uiMAX_SEB_BOXES; ctIndex++)
		if (pBoxes[ctIndex].bOpen) 
			bSuccess = bSuccess && pBoxes[ctIndex].pBox->Close();
			
	CGUI_App::ProcessEvents();
	return bSuccess;

}


// SEB_Modeless_Message helper member functions
BOOL SEB_Modeless_Message::Set_Message(const char *pszMessage) {

	if ((pszMsg = new char [strlen(pszMessage) + 1]) != NULL) {
		strcpy(pszMsg, pszMessage);
		pszUse = pszMsg;	
		return TRUE;
		}
	else
		return FALSE;

}


BOOL SEB_Modeless_Message::Modeless_Exit(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction) {

   	UINT ctIndex = 0;


	for (ctIndex = 0; ctIndex < uiMAX_SEB_BOXES; ctIndex++)
		if (pBoxes[ctIndex].pBox == pDlg) 
        	pBoxes[ctIndex].bOpen = FALSE;
        	
    
	return TRUE;

}


////////////////////////////////////////////////////////////////////////////////////
// SEB_Question_Box dialog box
////////////////////////////////////////////////////////////////////////////////////
// Creates and handles multiple YES/NO question boxes
//    Has all of the properties of a SEB_Modeless_Message object, however it
//    provides YES/NO buttons and notifications of the response.
////////////////////////////////////////////////////////////////////////////////////    								

// SEB_Modeless_Message constructors and destructors
SEB_Question_Box::SEB_Question_Box(const char *pszBox_Title,
									CGUI_CallBack *pCB_Obj,
									GUI_CB_DlgNotifyFn pYES_NotifyFn,
									GUI_CB_DlgNotifyFn pNO_NotifyFn,
									RESOURCE_ID Icon,
									BOOL bUse_Small_Question_Box) :
									// Name of application to appear in title bar
									// Your object - used for notification of button press
									// Your YES callback member function - used for notification of YES button press
									// Your NO callback member function - used for notification of NO button press
									// The application icon to appear as the system menu (Windows 95)
									// Whether to use a small box or large box
	SEB_Modeless_Message(pszBox_Title, Icon),
	pObj(pCB_Obj),
	pYES(pYES_NotifyFn),
	pNO(pNO_NotifyFn),
	bSmall_Question_Box(bUse_Small_Question_Box),
	uiNumber_Buttons(2)
	{}


SEB_Question_Box::SEB_Question_Box(const char *pszBox_Title, 
									const char *pszQuestion,
									CGUI_CallBack *pCB_Obj,
									GUI_CB_DlgNotifyFn pYES_NotifyFn,
									GUI_CB_DlgNotifyFn pNO_NotifyFn,
									RESOURCE_ID Icon,
									BOOL bUse_Small_Question_Box) :
									// Name of application to appear in title bar
									// Question to appear in the box
									// Your object - used for notification of button press
									// Your YES callback member function - used for notification of YES button press
									// Your NO callback member function - used for notification of NO button press
									// The application icon to appear as the system menu (Windows 95)
									// Whether to use a small box or large box
	SEB_Modeless_Message(pszBox_Title, pszQuestion, Icon),
	pObj(pCB_Obj),
	pYES(pYES_NotifyFn),
	pNO(pNO_NotifyFn),
	bSmall_Question_Box(bUse_Small_Question_Box),
	uiNumber_Buttons(2)
	{}


SEB_Question_Box::~SEB_Question_Box(void) {}


// Sets the callbacks for the next New
BOOL SEB_Question_Box::Set_Callbacks(CGUI_CallBack *pCB_Obj, GUI_CB_DlgNotifyFn pYES_NotifyFn, GUI_CB_DlgNotifyFn pNO_NotifyFn, RESOURCE_ID Icon) {

	pObj = pCB_Obj;
	pYES = pYES_NotifyFn;
	pNO = pNO_NotifyFn;
    uiIcon = Icon;
                       
                       
	return TRUE;       
	
}

// SEB_Question_Box starting member functions
GUI_ACTION SEB_Question_Box::New(BOOL bModeless) {

	UINT ctIndex = 0;
	BOOL bSlot_Found = FALSE;
	GUI_ACTION bSuccess = GUI_NONE;


	// Find an open slot for the box
	while ((!bSlot_Found) && (ctIndex < uiMAX_SEB_BOXES)) {
		if (!pBoxes[ctIndex].bOpen)
			bSlot_Found = TRUE;
		else
			ctIndex++;
		}
	if (!bSlot_Found) return GUI_NONE;
	
	// Create the box
	if ((bSmall_Question_Box && ((pBoxes[ctIndex].pBox = new CGUI_Dlg(IDD_SEB_QUESTION_BOX, (void *)this, DLG_POSN_NO_SAVE)) != NULL)) ||
		(!bSmall_Question_Box && ((pBoxes[ctIndex].pBox = new CGUI_Dlg(IDD_SEB_QUESTION_BOX_LARGE, (void *)this, DLG_POSN_NO_SAVE)) != NULL))) {
		if (uiIcon)
			pBoxes[ctIndex].pBox->SetIcon(uiIcon);
		pBoxes[ctIndex].pBox->SetTitle(pszTitle);
		pBoxes[ctIndex].pBox->DefineFldStaticTxt(IDC_SEB_QUESTION_BOX_MESSAGE);
		pBoxes[ctIndex].pBox->FldStaticTxtSetValue(IDC_SEB_QUESTION_BOX_MESSAGE, pszUse);
		pBoxes[ctIndex].pBox->DefinePushBtn(IDC_SEB_QUESTION_BOX_YES, GUI_YES);
		pBoxes[ctIndex].pBox->DefinePushBtn(IDC_SEB_QUESTION_BOX_NO, GUI_CANCEL); // Keep cancel so that X box in corner works - NOTE: Will GPF if close box when canceled
		pBoxes[ctIndex].pBox->SetFocus(IDC_SEB_QUESTION_BOX_NO);
		if ((pObj != NULL) && (pYES != NULL))
			pBoxes[ctIndex].pBox->SetCtrlNotifyFn(IDC_SEB_QUESTION_BOX_YES, pObj, pYES);
		if ((pObj != NULL) && (pNO != NULL))
			pBoxes[ctIndex].pBox->SetCtrlNotifyFn(IDC_SEB_QUESTION_BOX_NO, pObj, pNO);
		pBoxes[ctIndex].pBox->SetDlgExitValidateFn((CGUI_CallBack *)((void *)this), 
													(GUI_CB_DlgExitFn)&SEB_Modeless_Message::Modeless_Exit);
	    pBoxes[ctIndex].bOpen = TRUE;
	    
		// Start the box
		if (bModeless) {
			pBoxes[ctIndex].pBox->GoModeless(TRUE);
			bSuccess = GUI_OK;
			}
		else {
			bSuccess = (pBoxes[ctIndex].pBox->Go());
			delete pBoxes[ctIndex].pBox;
			pBoxes[ctIndex].bOpen = FALSE;
			}
		
		}

	// Reset back to default message	
	pszUse = pszMsg;

		
	return bSuccess;
	
}


GUI_ACTION SEB_Question_Box::New(const char *pszQuestion, BOOL bModeless, BOOL bSet_Message) {

	if (bSet_Message)
		if (!Set_Message(pszQuestion))
			return GUI_NONE;
			
	pszUse = pszQuestion;
	return New(bModeless);

}


UINT SEB_Question_Box::Get_Number_Buttons(void) {

	return uiNumber_Buttons;

}


// Disables all of the displayed buttons of all boxes
void SEB_Question_Box::Disable_All_Buttons(void) {

	UINT ctIndex;


	for (ctIndex = 0; ctIndex < uiMAX_SEB_BOXES; ctIndex++) {
		if (pBoxes[ctIndex].bOpen) {
			pBoxes[ctIndex].pBox->SetEnable(IDC_SEB_QUESTION_BOX_YES, FALSE);
			pBoxes[ctIndex].pBox->SetEnable(IDC_SEB_QUESTION_BOX_NO, FALSE);
			}
		}

	CGUI_App::ProcessEvents();

}


// SEB_Question_Box closing member functions
BOOL SEB_Question_Box::Close_All(void) {

	return SEB_Modeless_Message::Close_All();
	
}



////////////////////////////////////////////////////////////////////////////////////
// SEB_Three_Question_Box dialog box
////////////////////////////////////////////////////////////////////////////////////
// Creates and handles multiple three question boxes
//    Allows user to select amongst three choices which are set through the
//       constructor
//    Choices for model and modeless
//    A SEB_Three_Question_Box must be created on the heap if used as modeless 
//       otherwise destructor called on function exit
////////////////////////////////////////////////////////////////////////////////////    								

// SEB_Modeless_Message constructors and destructors
SEB_Three_Question_Box::SEB_Three_Question_Box(const char *pszBox_Title,
									const char *pszButton_1_Text,
									const char *pszButton_2_Text,
									const char *pszButton_3_Text,
									CGUI_CallBack *pCB_Obj,
									GUI_CB_DlgNotifyFn pButton1_NotifyFn,
									GUI_CB_DlgNotifyFn pButton2_NotifyFn,
									GUI_CB_DlgNotifyFn pButton3_NotifyFn,
									RESOURCE_ID Icon,
									BOOL bUse_Small_Question_Box) :
									// Name of application to appear in title bar
									// Your Button #1 text
									// Your Button #2 text
									// Your Button #3 text
									// Your object - used for notification of button press
									// Your Button #1 callback member function - used for notification of Button #1 press
									// Your Button #2 callback member function - used for notification of Button #2 press
									// Your Button #3 callback member function - used for notification of Button #3 press
									// The application icon to appear as the system menu (Windows 95)
									// Whether to use a small box or large box
	SEB_Question_Box(pszBox_Title),
	pszBut1_Text(NULL),
	pszBut2_Text(NULL),
	pszBut3_Text(NULL),
	pObj(pCB_Obj),
	pBut1(pButton1_NotifyFn),
	pBut2(pButton2_NotifyFn),
	pBut3(pButton3_NotifyFn),
	bSmall_Question_Box(bUse_Small_Question_Box)
	{
	
	SEB_Question_Box::uiNumber_Buttons = 3;

	SEB_Question_Box::uiIcon = Icon;
	
    if (pszButton_1_Text)
		if ((pszBut1_Text = new char [strlen(pszButton_1_Text) + 1]) != NULL)
			strcpy(pszBut1_Text, pszButton_1_Text);	
    if (pszButton_2_Text)
		if ((pszBut2_Text = new char [strlen(pszButton_2_Text) + 1]) != NULL)
			strcpy(pszBut2_Text, pszButton_2_Text);	
    if (pszButton_3_Text)
		if ((pszBut3_Text = new char [strlen(pszButton_3_Text) + 1]) != NULL)
			strcpy(pszBut3_Text, pszButton_3_Text);	

}


SEB_Three_Question_Box::SEB_Three_Question_Box(const char *pszBox_Title, 
									const char *pszQuestion,
									const char *pszButton_1_Text,
									const char *pszButton_2_Text,
									const char *pszButton_3_Text,
									CGUI_CallBack *pCB_Obj,
									GUI_CB_DlgNotifyFn pButton1_NotifyFn,
									GUI_CB_DlgNotifyFn pButton2_NotifyFn,
									GUI_CB_DlgNotifyFn pButton3_NotifyFn,
									RESOURCE_ID Icon,
									BOOL bUse_Small_Question_Box) :
									// Name of application to appear in title bar
									// Question to appear in the box
									// Your Button #1 text
									// Your Button #2 text
									// Your Button #3 text
									// Your object - used for notification of button press
									// Your Button #1 callback member function - used for notification of Button #1 press
									// Your Button #2 callback member function - used for notification of Button #2 press
									// Your Button #3 callback member function - used for notification of Button #3 press
									// The application icon to appear as the system menu (Windows 95)
									// Whether to use a small box or large box
	SEB_Question_Box(pszBox_Title, pszQuestion),
	pszBut1_Text(NULL),
	pszBut2_Text(NULL),
	pszBut3_Text(NULL),
	pObj(pCB_Obj),
	pBut1(pButton1_NotifyFn),
	pBut2(pButton2_NotifyFn),
	pBut3(pButton3_NotifyFn),
	bSmall_Question_Box(bUse_Small_Question_Box)
	{
	
	SEB_Question_Box::uiNumber_Buttons = 3;

	SEB_Question_Box::uiIcon = Icon;
	
    if (pszButton_1_Text)
		if ((pszBut1_Text = new char [strlen(pszButton_1_Text) + 1]) != NULL)
			strcpy(pszBut1_Text, pszButton_1_Text);	
    if (pszButton_2_Text)
		if ((pszBut2_Text = new char [strlen(pszButton_2_Text) + 1]) != NULL)
			strcpy(pszBut2_Text, pszButton_2_Text);	
    if (pszButton_3_Text)
		if ((pszBut3_Text = new char [strlen(pszButton_3_Text) + 1]) != NULL)
			strcpy(pszBut3_Text, pszButton_3_Text);	

}


SEB_Three_Question_Box::~SEB_Three_Question_Box(void) {

	if (pszBut1_Text)
		delete [] pszBut1_Text;
	if (pszBut2_Text)
		delete [] pszBut2_Text;
	if (pszBut3_Text)
		delete [] pszBut3_Text;

}


// Sets the callbacks for the next New
BOOL SEB_Three_Question_Box::Set_Callbacks(CGUI_CallBack *pCB_Obj, GUI_CB_DlgNotifyFn pButton1_NotifyFn, GUI_CB_DlgNotifyFn pButton2_NotifyFn, GUI_CB_DlgNotifyFn pButton3_NotifyFn, RESOURCE_ID Icon) {

	pObj = pCB_Obj;
	pBut1 = pButton1_NotifyFn;
	pBut2 = pButton2_NotifyFn;
	pBut3 = pButton3_NotifyFn;
    uiIcon = Icon;
                       
                       
	return TRUE;       
	
}

// SEB_Question_Box starting member functions
GUI_ACTION SEB_Three_Question_Box::New(BOOL bModeless) {

	UINT ctIndex = 0;
	BOOL bSlot_Found = FALSE;
	GUI_ACTION bSuccess = GUI_NONE;


	// Find an open slot for the box
	while ((!bSlot_Found) && (ctIndex < uiMAX_SEB_BOXES)) {
		if (!pBoxes[ctIndex].bOpen)
			bSlot_Found = TRUE;
		else
			ctIndex++;
		}
	if (!bSlot_Found) return GUI_NONE;
	
	// Create the box
	if ((bSmall_Question_Box && ((pBoxes[ctIndex].pBox = new CGUI_Dlg(IDD_SEB_THREE_QUESTION_BOX, (void *)this, DLG_POSN_NO_SAVE)) != NULL)) ||
		(!bSmall_Question_Box && ((pBoxes[ctIndex].pBox = new CGUI_Dlg(IDD_SEB_THREE_QUESTION_BOX_LARGE, (void *)this, DLG_POSN_NO_SAVE)) != NULL))) {
		if (uiIcon)
			pBoxes[ctIndex].pBox->SetIcon(uiIcon);
		pBoxes[ctIndex].pBox->SetTitle(pszTitle);
		pBoxes[ctIndex].pBox->DefineFldStaticTxt(IDC_SEB_THREE_BOX_MESSAGE);
		pBoxes[ctIndex].pBox->FldStaticTxtSetValue(IDC_SEB_THREE_BOX_MESSAGE, pszUse);
		pBoxes[ctIndex].pBox->DefinePushBtn(IDC_SEB_THREE_BOX_BUTTON_1, GUI_YES);
		pBoxes[ctIndex].pBox->SetText(IDC_SEB_THREE_BOX_BUTTON_1, pszBut1_Text);
		pBoxes[ctIndex].pBox->DefinePushBtn(IDC_SEB_THREE_BOX_BUTTON_2, GUI_NO);
		pBoxes[ctIndex].pBox->SetText(IDC_SEB_THREE_BOX_BUTTON_2, pszBut2_Text);
		pBoxes[ctIndex].pBox->DefinePushBtn(IDC_SEB_THREE_BOX_BUTTON_3, GUI_CANCEL);
		pBoxes[ctIndex].pBox->SetText(IDC_SEB_THREE_BOX_BUTTON_3, pszBut3_Text);
		pBoxes[ctIndex].pBox->SetFocus(IDC_SEB_THREE_BOX_BUTTON_3);
		if ((pObj != NULL) && (pBut1 != NULL))
			pBoxes[ctIndex].pBox->SetCtrlNotifyFn(IDC_SEB_THREE_BOX_BUTTON_1, pObj, pBut1);
		if ((pObj != NULL) && (pBut2 != NULL))
			pBoxes[ctIndex].pBox->SetCtrlNotifyFn(IDC_SEB_THREE_BOX_BUTTON_2, pObj, pBut2);
		if ((pObj != NULL) && (pBut3 != NULL))
			pBoxes[ctIndex].pBox->SetCtrlNotifyFn(IDC_SEB_THREE_BOX_BUTTON_3, pObj, pBut3);
	    pBoxes[ctIndex].bOpen = TRUE;
	    
		// Start the box
		if (bModeless) {
			pBoxes[ctIndex].pBox->SetDlgExitValidateFn((CGUI_CallBack *)((void *)this), 
														(GUI_CB_DlgExitFn)&SEB_Question_Box::Modeless_Exit);
			pBoxes[ctIndex].pBox->GoModeless(TRUE);
			bSuccess = GUI_OK;
			}
		else {
			bSuccess = (pBoxes[ctIndex].pBox->Go());
			delete pBoxes[ctIndex].pBox;
			pBoxes[ctIndex].bOpen = FALSE;
			}
		
		}

	// Reset back to default message	
	pszUse = pszMsg;

		
	return bSuccess;
	
}


GUI_ACTION SEB_Three_Question_Box::New(const char *pszQuestion, BOOL bModeless, BOOL bSet_Message) {

	return SEB_Question_Box::New(pszQuestion, bModeless, bSet_Message);
	
}


// Disables all of the displayed buttons of all boxes
void SEB_Three_Question_Box::Disable_All_Buttons(void) {

	UINT ctIndex;


	for (ctIndex = 0; ctIndex < uiMAX_SEB_BOXES; ctIndex++) {
		if (pBoxes[ctIndex].bOpen) {
			pBoxes[ctIndex].pBox->SetEnable(IDC_SEB_THREE_BOX_BUTTON_1, FALSE);
			pBoxes[ctIndex].pBox->SetEnable(IDC_SEB_THREE_BOX_BUTTON_2, FALSE);
			pBoxes[ctIndex].pBox->SetEnable(IDC_SEB_THREE_BOX_BUTTON_3, FALSE);
			}
		}

	CGUI_App::ProcessEvents();

}


// SEB_Question_Box closing member functions
BOOL SEB_Three_Question_Box::Close_All(void) {

	return SEB_Question_Box::Close_All();
	
}



////////////////////////////////////////////////////////////////////////////////////
// SEB_About_Box dialog box
////////////////////////////////////////////////////////////////////////////////////
// Creates and handles my standard About Box
//    Simply instantiate a SEB_About_Box object and call display.
//    Note: This dialog box is model and requires a response before closing.
//    When user deletes object it will automatically cleans up after itself.
////////////////////////////////////////////////////////////////////////////////////    								
SEB_About_Box::SEB_About_Box(const char *pszApplication_Name,
							const char *pszApplication_Version,
							const char *pszReview_Tool_Version,
							const char *pszFacility_Name,
							const char *pszLocation_Name,
							RESOURCE_ID App_Icon) :
							// Name of application
							// Version of application - string allows "alpha", etc.
							// Version of review tool - string allows "alpha", etc.
							// Name of facility
							// Name of location in facility
							// The application icon to appear as the system menu (Windows 95)
							//    Note: This is not the icon which appears in the upper left corner,
							//          you must set that yourself in AppStudio
	pszName(NULL),
	pszApp_Version(NULL),
	pszRT_Version(NULL),
	pszFacility(NULL),
	pszLocation(NULL),
	uiIcon(App_Icon),
	pAbout_Box(NULL)
	{
	
	if (pszApplication_Name) {
		if ((pszName = new char [strlen(pszApplication_Name) + 1]) != NULL)
			strcpy(pszName, pszApplication_Name);
		}
	if (pszApplication_Version) {
		if ((pszApp_Version = new char [strlen(pszApplication_Version) + 1]) != NULL)
			strcpy(pszApp_Version, pszApplication_Version);	
		}
	if (pszReview_Tool_Version) {
		if ((pszRT_Version = new char [strlen(pszReview_Tool_Version) + 1]) != NULL)
			strcpy(pszRT_Version, pszReview_Tool_Version);	
		}
	if (pszFacility_Name) {
		if ((pszFacility = new char [strlen(pszFacility_Name) + 1]) != NULL)
			strcpy(pszFacility, pszFacility_Name);	
		}
	if (pszLocation_Name) {
		if ((pszLocation = new char [strlen(pszLocation_Name) + 1]) != NULL)
			strcpy(pszLocation, pszLocation_Name);	
		}
	
}


SEB_About_Box::~SEB_About_Box(void) {

	Close();
	if (pszName) {
		delete [] pszName;
		pszName = NULL;
		}
	if (pszApp_Version) {
		delete [] pszApp_Version;
		pszApp_Version = NULL;
		}
	if (pszRT_Version) {
		delete [] pszRT_Version;
		pszRT_Version = NULL;
		}
	if (pszFacility) {
		delete [] pszFacility;
		pszFacility = NULL;
		}
	if (pszLocation) {
		delete [] pszLocation;
		pszLocation = NULL;
		}
	if (pAbout_Box) {
		delete pAbout_Box;
		pAbout_Box = NULL;
		}

}


// Displays the SEB_About_Box
BOOL SEB_About_Box::Display(void) {

	char *pszTitle = NULL, *pszApp_Version_Msg = NULL, *pszRT_Version_Msg = NULL;
	char szMemory[50];
	BOOL bSuccess = FALSE;

	
	// Create the dialog box
	if ((pAbout_Box = new CGUI_Dlg(IDD_SEB_ABOUT_BOX, NULL, DLG_POSN_SAVE_ABS)) != NULL) {

		// Set the title
		if ((pszName) &&
			((pszTitle = new char [strlen(pszName) + strlen(" - About ") + strlen(pszName) + 1]) != NULL)) {
			strcpy(pszTitle, pszName);
			strcat(pszTitle, " - About ");
			strcat(pszTitle, pszName);
			pAbout_Box->SetTitle(pszTitle);
			}
		
		// Set the app name message
		if (pszName) {
			pAbout_Box->DefineFldStaticTxt(IDC_SEB_ABOUT_APP_NAME);
			pAbout_Box->FldStaticTxtSetValue(IDC_SEB_ABOUT_APP_NAME, pszName);
			}
	
		// Set the version message
		if ((pszName) && (pszApp_Version) &&
			((pszApp_Version_Msg = new char [strlen(pszName) + strlen(" - Version ") + strlen(pszApp_Version) + 1]) != NULL)) {
			strcpy(pszApp_Version_Msg, "Version ");
			strcat(pszApp_Version_Msg, pszApp_Version);
			pAbout_Box->DefineFldStaticTxt(IDC_SEB_ABOUT_APP_VERSION);
			pAbout_Box->FldStaticTxtSetValue(IDC_SEB_ABOUT_APP_VERSION, pszApp_Version_Msg);
			}
	
		// Set the version message
		if ((pszRT_Version) &&
			((pszRT_Version_Msg = new char [strlen("operating in review tool mode ") + strlen(pszRT_Version) + 1]) != NULL)) {
			strcpy(pszRT_Version_Msg, "operating in review tool mode ");
			strcat(pszRT_Version_Msg, pszRT_Version);
			pAbout_Box->DefineFldStaticTxt(IDC_SEB_ABOUT_RT_VERSION);
			pAbout_Box->FldStaticTxtSetValue(IDC_SEB_ABOUT_RT_VERSION, pszRT_Version_Msg);
			}
	
		// Set the facility message
		pAbout_Box->DefineFldStaticTxt(IDC_SEB_ABOUT_FACILITY);
		pAbout_Box->FldStaticTxtSetValue(IDC_SEB_ABOUT_FACILITY, pszFacility);
	
		// Set the location message
		pAbout_Box->DefineFldStaticTxt(IDC_SEB_ABOUT_LOCATION);
		pAbout_Box->FldStaticTxtSetValue(IDC_SEB_ABOUT_LOCATION, pszLocation);
	
		// Setup the buttons and bitmaps
		if (uiIcon)	pAbout_Box->SetIcon(uiIcon);
		pAbout_Box->DefinePushBtn(IDC_SEB_ABOUT_OK, GUI_CANCEL); // Keep cancel so that X box in corner works - NOTE: Will GPF if close box when canceled
		pAbout_Box->SetFocus(IDC_SEB_ABOUT_OK);
        
		// Display available memory and available resources
		pAbout_Box->DefineFldStaticTxt(IDC_SEB_ABOUT_MEMORY);
		pAbout_Box->DefineFldStaticTxt(IDC_SEB_ABOUT_RESOURCES);

#ifdef _WIN32
		
		//typedef struct _MEMORYSTATUS { // mst 
		//	DWORD dwLength;        // sizeof(MEMORYSTATUS) 
		//	DWORD dwMemoryLoad;    // percent of memory in use 
		//	DWORD dwTotalPhys;     // bytes of physical memory 
		//	DWORD dwAvailPhys;     // free physical memory bytes 
		//	DWORD dwTotalPageFile; // bytes of paging file 
		//	DWORD dwAvailPageFile; // free bytes of paging file 
		//	DWORD dwTotalVirtual;  // user bytes of address space 
		//	DWORD dwAvailVirtual;  // free user bytes 
		//} MEMORYSTATUS, *LPMEMORYSTATUS; 
 
		MEMORYSTATUS Memory;
		Memory.dwLength = sizeof(MEMORYSTATUS);

		GlobalMemoryStatus(&Memory);

		sprintf(szMemory, "Memory Available: %lu KB Free", Memory.dwAvailPhys / 1000);
		pAbout_Box->FldStaticTxtSetValue(IDC_SEB_ABOUT_MEMORY, szMemory);	

		//sprintf(szMemory, "Resources Available: %lu%% Free", (((float)Memory.dwAvailVirtual / (float)Memory.dwTotalVirtual) * 100.0));
		//pAbout_Box->FldStaticTxtSetValue(IDC_SEB_ABOUT_RESOURCES, szMemory);	

#else
		DWORD dMemory_Available;

		dMemory_Available = GetFreeSpace(NULL);
		sprintf(szMemory, "Memory Available: %lu KB Free", dMemory_Available);
		pAbout_Box->FldStaticTxtSetValue(IDC_SEB_ABOUT_MEMORY, szMemory);	

		UINT uiResources_Available;
		uiResources_Available = GetFreeSystemResources(GFSR_SYSTEMRESOURCES);
		sprintf(szMemory, "Resources Available: %u%% Free", uiResources_Available);
		pAbout_Box->FldStaticTxtSetValue(IDC_SEB_ABOUT_RESOURCES, szMemory);	
#endif
		
		// Display the about box
		pAbout_Box->Go();
		bSuccess = TRUE;
		}
		

	if (pszTitle) {
		delete [] pszTitle;
		pszTitle = NULL;
		}
	if (pszApp_Version_Msg) {
		delete [] pszApp_Version_Msg;
		pszApp_Version_Msg = NULL;
		}
	if (pszRT_Version_Msg) {
		delete [] pszRT_Version_Msg;
		pszRT_Version_Msg = NULL;
		}
	if (pAbout_Box) {
		delete pAbout_Box;
		pAbout_Box = NULL;
		}
		

	return bSuccess;

}


// Will force the SEB_About_Box to close
BOOL SEB_About_Box::Close(void) {

	BOOL bSuccess = FALSE;


	if (pAbout_Box)
		bSuccess = pAbout_Box->Close();
		
	return bSuccess;
	
}



////////////////////////////////////////////////////////////////////////////////////
// SEB_Product_Support_Box dialog box
////////////////////////////////////////////////////////////////////////////////////
// Creates and handles my standard Product Support Box
//    Simply instantiate a SEB_Product_Support_Box object and call display.
//    Note: This dialog box MUST be modeless!
//    User can call Close to close the opened SEB_Product_Support_Box or delete
//       the SEB_Modeless_Message object which automatically closes and cleans up
//       after itself.
////////////////////////////////////////////////////////////////////////////////////    								

SEB_Product_Support_Box::SEB_Product_Support_Box(const char *pszApp_Name,
							const char *pszContact_Name,
							const char *pszContact_Email,
							RESOURCE_ID App_Icon) :
							// Name of application
							// The name of the person to contact for questions, bug reports, etc.
							// The email address of the person to contact for questions, bug reports, etc.
							// The application icon to appear in the system menu
	pszApp(NULL),
	pszName(NULL),
	pszEmail(NULL),
	uiIcon(App_Icon)
	{
	
	if ((pszApp = new char [strlen(pszApp_Name) + 1]) != NULL)
		strcpy(pszApp, pszApp_Name);	
	if ((pszName = new char [strlen(pszContact_Name) + 1]) != NULL)
		strcpy(pszName, pszContact_Name);	
	if ((pszEmail = new char [strlen(pszContact_Email) + 1]) != NULL)
		strcpy(pszEmail, pszContact_Email);	
		
}


SEB_Product_Support_Box::~SEB_Product_Support_Box(void) {
    
	if (pszApp) delete [] pszApp;
	if (pszName) delete [] pszName;
	if (pszEmail) delete [] pszEmail;
	
}


// Displays the SEB_About_Box
BOOL SEB_Product_Support_Box::Display(void) {

	char *pszTitle = NULL, *pszMessage_Email = NULL;
	CGUI_Dlg PS_Box(IDD_SEB_PRODUCT_SUPPORT_BOX, NULL, DLG_POSN_SAVE_ABS);
			

	// Create the dialog box
	if ((pszApp) && (pszName) && (pszEmail)) {
		if (((pszTitle = new char [strlen(pszApp) + strlen(" - Product Support") + 1]) != NULL) &&
			((pszMessage_Email = new char [strlen(pszEmail) + strlen(pszName) + 6]) != NULL)) {

			// Set the title
			strcpy(pszTitle, pszApp);
			strcat(pszTitle, " - Product Support");
			PS_Box.SetTitle(pszTitle);

			// Set email message
			strcpy(pszMessage_Email, pszEmail);
			strcat(pszMessage_Email, "   (");
			strcat(pszMessage_Email, pszName);
			strcat(pszMessage_Email, ")");
			PS_Box.DefineFldStaticTxt(IDC_SEB_PS_CONTACT);
			PS_Box.FldStaticTxtSetValue(IDC_SEB_PS_CONTACT, pszMessage_Email);

			// Set the buttons and other stuff
			if (uiIcon)	PS_Box.SetIcon(uiIcon);
			PS_Box.DefinePushBtn(IDC_SEB_PS_OK, GUI_CANCEL);  // Keep cancel so that X box in corner works - NOTE: Will GPF if close box when canceled
			PS_Box.SetFocus(IDC_SEB_PS_OK);


			// Display the box
			PS_Box.Go();
		    }


		if (pszTitle) delete [] pszTitle;
		if (pszMessage_Email) delete [] pszMessage_Email;

		return TRUE;
		}
		

	return FALSE;
	
}


////////////////////////////////////////////////////////////////////////////////////
// SEB_IAEA_Inspection_Box dialog box
////////////////////////////////////////////////////////////////////////////////////
// Creates and handles the standard IAEA inspection information and box
//    Simply instantiate a SEB_IAEA_Inspection_Box object with the required
//       structure COMPLETELY filled.
//    The users button press is returned from display.
//    The users inspection information is passed back through the same structure
//       passed in.
////////////////////////////////////////////////////////////////////////////////////    								
SEB_IAEA_Inspection_Box::SEB_IAEA_Inspection_Box(const char *pszApp_Name,
							char **ppszFacility_Name,
							struct IAEA_INSPECTION_INFO *pIAEA_Inspection_Info,
							RESOURCE_ID App_Icon) :
							// Name of the application
							// Name of facility - *Could change!
							// Structure containing box inital and result information
							// The application icon to appear as the system menu (Windows 95)
	pszApp(NULL),
	ppszFacility(ppszFacility_Name),
	pInfo(pIAEA_Inspection_Info),
	uiIcon(App_Icon),
	pInspect_Box(NULL)
	{
	
	if ((pszApp = new char [strlen(pszApp_Name) + 1]) != NULL)
		strcpy(pszApp, pszApp_Name);	

}

	
SEB_IAEA_Inspection_Box::~SEB_IAEA_Inspection_Box(void) {

	Close();
	if (pInspect_Box) {
		delete pInspect_Box;
		pInspect_Box = NULL;
		}
	if (pszApp) delete [] pszApp;
	
}


// Displays the SEB_IAEA_Inspection_Box - return GUI_OK, GUI_CANCEL, or GUI_NONE (Error) 
GUI_ACTION SEB_IAEA_Inspection_Box::Display(BOOL bSet_To_New) {

	GUI_ACTION eResult;


	// Setup the box with structure values
	if (!Setup_Box()) return GUI_NONE;

	if (bSet_To_New) Set_To_New();

	// Display the box		
	eResult = pInspect_Box->Go();

	if (eResult == GUI_OK)
		SEB_IAEA_Inspec_Get_Values();
	
	if (pInspect_Box) {
		delete pInspect_Box;
		pInspect_Box = NULL;
		}


	return eResult;
 
}


BOOL SEB_IAEA_Inspection_Box::Setup_Box(void) {
 
	char *pszMessage;
	UINT ctIndex;
	CGStr Temp_String;

    
	if ((pszApp) && (ppszFacility) && (pInfo)) {
		// Create the dialog box
		if (!pInspect_Box) {
			if ((pInspect_Box = new CGUI_Dlg(IDD_SEB_IAEA_INSPECTION_BOX, NULL, DLG_POSN_NO_SAVE)) == NULL)
				return FALSE;

			if (uiIcon)
				pInspect_Box->SetIcon(uiIcon);
			
			// Set the title
			if ((pszMessage = new char [strlen(pszApp) + strlen(" - Inspection Information") + 1]) == NULL)
				 return FALSE;
			strcpy(pszMessage, pszApp);
			strcat(pszMessage, " - Inspection Information");
			pInspect_Box->SetTitle(pszMessage);
			delete [] pszMessage;

			// Set the facility name
			Temp_String = ".   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   ";
            Temp_String += (*ppszFacility);
			pInspect_Box->DefineFldStaticTxt(IDC_SEB_IAEA_INSPEC_FACILITY_NAME, Temp_String.Get_sz());
			
            // Set the review date
			pInspect_Box->DefineFldDate(IDC_SEB_IAEA_INSPEC_REVIEW_DATE, pInfo->pReview_Date);
			pInspect_Box->SetCtrlValidateFn(IDC_SEB_IAEA_INSPEC_REVIEW_DATE, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgValidateFn)&SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Review_Date);
			pInspect_Box->DefineFldTime(IDC_SEB_IAEA_INSPEC_REVIEW_TIME, pInfo->pReview_Time, 2);
			pInspect_Box->SetCtrlValidateFn(IDC_SEB_IAEA_INSPEC_REVIEW_TIME, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgValidateFn)&SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Review_Time);

			// Set the Inspector's list box
			pInspect_Box->DefinePL_MultiSel(IDC_SEB_IAEA_INSPEC_ID_LIST);
			for (ctIndex = 0; ctIndex < pInfo->uiInspector_ID_Num; ctIndex++)
				pInspect_Box->PL_MultiSelAdd(IDC_SEB_IAEA_INSPEC_ID_LIST, pInfo->ppszInspector_ID[ctIndex]);
			// Set them all off
			for (ctIndex = 0; ctIndex < pInfo->uiInspector_ID_Num; ctIndex++)
				pInspect_Box->PL_MultiSelSetValue(IDC_SEB_IAEA_INSPEC_ID_LIST, ctIndex, FALSE);
			// Set the proper ones on
			if (pInfo->uiInspector_ID_Num > 0)
				for (ctIndex = 0; ctIndex < pInfo->uiInspector_ID_Choices_Num; ctIndex++)
					pInspect_Box->PL_MultiSelSetValue(IDC_SEB_IAEA_INSPEC_ID_LIST, pInfo->puiInspector_ID_Choices[ctIndex], TRUE);

			// Set the location box
			pInspect_Box->DefineFldStaticTxt(IDC_SEB_IAEA_INSPEC_LOCATION, pInfo->pszFacility_Location);

			// Set the Inspection number
			// Must use string since could have leading 00 if year greater then 1999
			Temp_Inspection_Number = pInfo->pszInspection_Number;
            pInspect_Box->DefineFldTxt(IDC_SEB_IAEA_INSPEC_REVIEW_NUMBER, &Temp_Inspection_Number, 10);
			pInspect_Box->SetCtrlValidateFn(IDC_SEB_IAEA_INSPEC_REVIEW_NUMBER, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgValidateFn)&SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Number);

            // Set the start and end data date
			pInspect_Box->DefineFldDate(IDC_SEB_IAEA_INSPEC_REVIEW_START_DATE, pInfo->pStart_Date);
			pInspect_Box->SetCtrlValidateFn(IDC_SEB_IAEA_INSPEC_REVIEW_START_DATE, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgValidateFn)&SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Start_Date);
			pInspect_Box->DefineFldTime(IDC_SEB_IAEA_INSPEC_REVIEW_START_TIME, pInfo->pStart_Time, 2);
			pInspect_Box->SetCtrlValidateFn(IDC_SEB_IAEA_INSPEC_REVIEW_START_TIME, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgValidateFn)&SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Start_Time);
			pInspect_Box->DefineFldDate(IDC_SEB_IAEA_INSPEC_REVIEW_END_DATE, pInfo->pEnd_Date);
			pInspect_Box->SetCtrlValidateFn(IDC_SEB_IAEA_INSPEC_REVIEW_END_DATE, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgValidateFn)&SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_End_Date);
			pInspect_Box->DefineFldTime(IDC_SEB_IAEA_INSPEC_REVIEW_END_TIME, pInfo->pEnd_Time, 2);
			pInspect_Box->SetCtrlValidateFn(IDC_SEB_IAEA_INSPEC_REVIEW_END_TIME, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgValidateFn)&SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_End_Time);
	
            // Set the Inspection comments enter box
            Temp_Comments = pInfo->pszComments;
			pInspect_Box->DefineFldTxt(IDC_SEB_IAEA_INSPEC_COMMENTS, &Temp_Comments);
			
            
			// Define the buttons
			pInspect_Box->DefinePushBtn(IDC_SEB_IAEA_INSPEC_OK, GUI_OK);
			pInspect_Box->DefinePushBtn(IDC_SEB_IAEA_INSPEC_CANCEL, GUI_CANCEL);
			pInspect_Box->DefinePushBtn(IDC_SEB_IAEA_INSPEC_ID_ADD, GUI_NONE);
			pInspect_Box->SetCtrlNotifyFn(IDC_SEB_IAEA_INSPEC_ID_ADD, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgNotifyFn)&SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Add);
			pInspect_Box->DefinePushBtn(IDC_SEB_IAEA_INSPEC_DELETE, GUI_NONE);
			pInspect_Box->SetCtrlNotifyFn(IDC_SEB_IAEA_INSPEC_DELETE, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgNotifyFn)&SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Delete);


			// Default the focus to the top most edit field
			pInspect_Box->SetDlgInitNotifyFn((CGUI_CallBack *)((void *)this), (GUI_CB_DlgNotifyFn)&SEB_IAEA_Inspection_Box::Box_Startup);
		    }
		    
		return TRUE;
		}
		         
		         
	return FALSE;
		    
}

void SEB_IAEA_Inspection_Box::Box_Startup(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	// Must set select text off for inspection number here, this gets around a GUI.LIB bug
	pInspect_Box->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_NUMBER, FALSE);

}

// Closes the SEB_IAEA_Inspection_Box
BOOL SEB_IAEA_Inspection_Box::Close(void) {

	if (pInspect_Box)
		return (pInspect_Box->Close());
            
            
	return FALSE;
	
}     		


// Deletes an IAEA_INSPECTION_INFO structure completely
BOOL SEB_IAEA_Inspection_Box::Delete_IAEA_INSPECTION_INFO(struct IAEA_INSPECTION_INFO **ppIAEA_Inspection_Info) {

	UINT ctIndex;


	if ((*ppIAEA_Inspection_Info)->pszInspection_Number) {
		delete [] (*ppIAEA_Inspection_Info)->pszInspection_Number;
		(*ppIAEA_Inspection_Info)->pszInspection_Number = NULL;
		}
	SEB_DELETELISTSTRING((*ppIAEA_Inspection_Info)->ppszInspector_ID, (*ppIAEA_Inspection_Info)->uiInspector_ID_Num, ctIndex);
	if ((*ppIAEA_Inspection_Info)->puiInspector_ID_Choices) {
		delete [] (*ppIAEA_Inspection_Info)->puiInspector_ID_Choices;
		(*ppIAEA_Inspection_Info)->puiInspector_ID_Choices = NULL;
		}
	if (*ppIAEA_Inspection_Info) {
		delete (*ppIAEA_Inspection_Info);
		(*ppIAEA_Inspection_Info) = NULL;
		}


	return TRUE;

}


void SEB_IAEA_Inspection_Box::Set_To_New(void) {

	time_t Time;
	struct tm *Time_Struct;
	GEN_DATE_STRUCT Temp_Date;
	GEN_TIME_STRUCT Temp_Time;
	DWORD dInspection_Number;
	char szInspection_Number[11];
	
    
    // Get the current date and time
	time(&Time);
	Time_Struct = localtime(&Time);
    Temp_Date.yr = Time_Struct->tm_year; // Y2K compliant, will take years above 99
    Temp_Date.mon = Time_Struct->tm_mon + 1;
    Temp_Date.day = Time_Struct->tm_mday;
	Temp_Time.hr = Time_Struct->tm_hour;
	Temp_Time.min = Time_Struct->tm_min;
	Temp_Time.sec = Time_Struct->tm_sec;
	Temp_Time.msec = 0;

    // Set the review date
	pInspect_Box->FldDateSetValue(IDC_SEB_IAEA_INSPEC_REVIEW_DATE, Temp_Date);
	pInspect_Box->FldTimeSetValue(IDC_SEB_IAEA_INSPEC_REVIEW_TIME, Temp_Time);
    
    // Set the inspection number
    dInspection_Number = ((((DWORD)(Temp_Date.yr % 100)) * 10));
	sprintf(szInspection_Number, "%03lu", dInspection_Number);
    pInspect_Box->FldTxtSetValue(IDC_SEB_IAEA_INSPEC_REVIEW_NUMBER, szInspection_Number);

	// Set the inspection period
	pInspect_Box->FldDateSetValue(IDC_SEB_IAEA_INSPEC_REVIEW_START_DATE, (*pInfo->pEnd_Date));
	pInspect_Box->FldTimeSetValue(IDC_SEB_IAEA_INSPEC_REVIEW_START_TIME, (*pInfo->pEnd_Time));
	pInspect_Box->FldDateSetValue(IDC_SEB_IAEA_INSPEC_REVIEW_END_DATE, Temp_Date);
	pInspect_Box->FldTimeSetValue(IDC_SEB_IAEA_INSPEC_REVIEW_END_TIME, Temp_Time);

	// Set the comments to blank	
	pInspect_Box->FldTxtSetValue(IDC_SEB_IAEA_INSPEC_COMMENTS, "");

}


void SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Get_Values(void) {

	UINT ctIndex, ctSelected = 0;
	CGStr String;
	BOOL bSelected;
	

	// Delete the old values
	if (pInfo->pszInspection_Number) {
		delete [] (pInfo->pszInspection_Number);
		(*pInfo->pszInspection_Number) = NULL;
		}
	SEB_DELETELISTSTRING(pInfo->ppszInspector_ID, pInfo->uiInspector_ID_Num, ctIndex);
	if (pInfo->puiInspector_ID_Choices) {
		delete [] pInfo->puiInspector_ID_Choices;
		pInfo->puiInspector_ID_Choices = NULL;
		}
	if (pInfo->pszComments) {
		delete [] pInfo->pszComments;
		pInfo->pszComments = NULL;
		}

                                
	// Retrieve all of the values
	pInspect_Box->RetrieveAllControlValues();

	pInspect_Box->PL_MultiSelGetNumItems(IDC_SEB_IAEA_INSPEC_ID_LIST, &(pInfo->uiInspector_ID_Num));
	PL_MULTISELGETNUMSEL(pInfo->uiInspector_ID_Choices_Num, pInspect_Box, IDC_SEB_IAEA_INSPEC_ID_LIST, pInfo->uiInspector_ID_Num);
	if (((pInfo->pszInspection_Number = new char [strlen(Temp_Inspection_Number.Get_sz()) + 1]) != NULL) &&
		((pInfo->ppszInspector_ID = new (char (* ([pInfo->uiInspector_ID_Num])))) != NULL) &&
		((pInfo->puiInspector_ID_Choices = new UINT [pInfo->uiInspector_ID_Choices_Num]) != NULL) &&
		((pInfo->pszComments = new char [strlen(Temp_Comments.Get_sz()) + 1]) != NULL)) {
		strcpy(pInfo->pszInspection_Number, Temp_Inspection_Number.Get_sz());
		strcpy(pInfo->pszComments, Temp_Comments.Get_sz());
		for (ctIndex = 0; ctIndex < pInfo->uiInspector_ID_Num; ctIndex++) {
			pInspect_Box->PL_MultiSelGetValue(IDC_SEB_IAEA_INSPEC_ID_LIST, ctIndex, &bSelected, &String);
			if ((pInfo->ppszInspector_ID[ctIndex] = new char [strlen(String.Get_sz()) + 1]) != NULL)
				strcpy(pInfo->ppszInspector_ID[ctIndex], String.Get_sz());
			if (bSelected) {
				pInfo->puiInspector_ID_Choices[ctSelected] = ctIndex;
				ctSelected++;
				}
			}
		}

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Number(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	CGStr Inspec_Number;
	char *pszInspec_Number = NULL;
	UINT uiInspec_Number;
	BOOL bSuccess = FALSE;

UINT uitest = 87;
	// Verify that the inspection number is of the format YY0##
	pInspect_Box->FldTxtGetValue(CtrlID, &Inspec_Number);
	if ((1 == sscanf(Inspec_Number.Get_sz(), "%u", &uiInspec_Number)) &&
		(uiInspec_Number <= 99999) &&
		(strlen(Inspec_Number.Get_sz()) == 5) &&
		((pszInspec_Number = new char [strlen(Inspec_Number.Get_sz()) + 1]) != NULL) &&
		// Keep less then or equal in case character size changes
		(5 <= sprintf(pszInspec_Number, "%05u", uiInspec_Number)) &&
		(0 == ((uiInspec_Number % 1000) - (uiInspec_Number % 100))) &&
		(strcmp(pszInspec_Number, Inspec_Number.Get_sz()) == 0)) {
		bSuccess = TRUE;
		}
	else {
		GUI_MsgBox("The inspection number must be of the format YY0## where:\n   YY = Last two digits of year.\n   ## = Two digit inspection identifier.");
		}
	

	if (pszInspec_Number) {
		delete [] pszInspec_Number;
		pszInspec_Number = NULL;
		}


	return bSuccess;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	time_t Time;
	struct tm *Time_Struct;
	GEN_DATE_STRUCT Entered_Date;
	

	// Get the entered date
	// Y2K - Year returned will be four digits
	pInspect_Box->FldDateGetValue(CtrlID, &Entered_Date);

    
    // Get the current date
	time(&Time);
	Time_Struct = localtime(&Time);
	// THIS IS THE ONLY CORRECT WAY TO DO THIS!!!  UNDERSTAND THE LOGIC!!!
		// Check year greater
	if ((Entered_Date.yr > Time_Struct->tm_year + 1900) ||
		// Check month greater
		((Entered_Date.yr == Time_Struct->tm_year + 1900) &&
		(Entered_Date.mon > Time_Struct->tm_mon + 1)) ||
		// Check day greater
		((Entered_Date.yr == Time_Struct->tm_year + 1900) &&
		(Entered_Date.mon == Time_Struct->tm_mon + 1) &&
		(Entered_Date.day > Time_Struct->tm_mday))) {
		GUI_MsgBox("A date and time beyond the current one can not be entered.");
		return FALSE;
		}
	
	return TRUE;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Date_Less(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	// This member function checks to see if the date is one less then the current one
	// This is used if a time is greater then the current and must check that date is at least one less

	time_t Time;
	struct tm *Time_Struct;
	GEN_DATE_STRUCT Entered_Date;
	

	// Get the entered date
	// Y2K - Year returned will be four digits
	pInspect_Box->FldDateGetValue(CtrlID, &Entered_Date);

    
    // Get the current date
	time(&Time);
	Time_Struct = localtime(&Time);
	// THIS IS THE ONLY CORRECT WAY TO DO THIS!!!  UNDERSTAND THE LOGIC!!!
		// Check year greater
	if ((Entered_Date.yr > Time_Struct->tm_year + 1900) ||
		// Check month greater
		((Entered_Date.yr == Time_Struct->tm_year + 1900) &&
		(Entered_Date.mon > Time_Struct->tm_mon + 1)) ||
		// Check day greater
		((Entered_Date.yr == Time_Struct->tm_year + 1900) &&
		(Entered_Date.mon == Time_Struct->tm_mon + 1) &&
		(Entered_Date.day >= Time_Struct->tm_mday))) {
		GUI_MsgBox("A date and time beyond the current one can not be entered.");
		return FALSE;
		}
	
	return TRUE;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	time_t Time;
	struct tm *Time_Struct;
	GEN_TIME_STRUCT Entered_Time;
	

	// Get the entered date
	// Y2K - Year returned will be four digits
	pInspect_Box->FldTimeGetValue(CtrlID, &Entered_Time);

    
    // Get the current date
	time(&Time);
	Time_Struct = localtime(&Time);
	// THIS IS THE ONLY CORRECT WAY TO DO THIS!!!  UNDERSTAND THE LOGIC!!!
		// Check year greater
	if ((Entered_Time.hr > Time_Struct->tm_hour) ||
		// Check month greater
		((Entered_Time.hr == Time_Struct->tm_hour) &&
		(Entered_Time.min > Time_Struct->tm_min)) ||
		// Check day greater
		((Entered_Time.hr == Time_Struct->tm_hour) &&
		(Entered_Time.min == Time_Struct->tm_min) &&
		(Entered_Time.sec > Time_Struct->tm_sec))) {
		// No message displayed because now must check date at least one less then current
		return FALSE;
		}
	
	return TRUE;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Range_Date(CGUI_Dlg *pDlg) {

	GEN_DATE_STRUCT Start_Date, End_Date;
	

	pDlg->FldDateGetValue(IDC_SEB_IAEA_INSPEC_REVIEW_START_DATE, &Start_Date);
	pDlg->FldDateGetValue(IDC_SEB_IAEA_INSPEC_REVIEW_END_DATE, &End_Date);

	// Verify that starting date is equal to or less then ending date
	// NOTE: Must do as below, think about the logic!
	if ((Start_Date.yr > End_Date.yr) ||

		((Start_Date.yr == End_Date.yr) &&
		 (Start_Date.mon > End_Date.mon)) ||
		// If start year less then end then nothing else matters
	
		((Start_Date.yr == End_Date.yr) &&
		 (Start_Date.mon == End_Date.mon) &&
		 (Start_Date.day > End_Date.day))) {
		// If start month less then end then day does not matters

		GUI_MsgBox("The starting date and time must be less then or equal to the ending date and time.");

		return FALSE;
		}

	return TRUE;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Range_Date_Less(CGUI_Dlg *pDlg) {

	// This member function checks to see if the date is one less then the current one
	// This is used if a time is greater then the current and must check that date is at least one less

	GEN_DATE_STRUCT Start_Date, End_Date;
	

	pDlg->FldDateGetValue(IDC_SEB_IAEA_INSPEC_REVIEW_START_DATE, &Start_Date);
	pDlg->FldDateGetValue(IDC_SEB_IAEA_INSPEC_REVIEW_END_DATE, &End_Date);

	// Verify that starting date is equal to or less then ending date
	// NOTE: Must do as below, think about the logic!
	if ((Start_Date.yr > End_Date.yr) ||

		((Start_Date.yr == End_Date.yr) &&
		 (Start_Date.mon > End_Date.mon)) ||
		// If start year less then end then nothing else matters
	
		((Start_Date.yr == End_Date.yr) &&
		 (Start_Date.mon == End_Date.mon) &&
		 (Start_Date.day >= End_Date.day))) {
		// If start month less then end then day does not matters

		GUI_MsgBox("The starting date and time must be less then or equal to the ending date and time.");

		return FALSE;
		}

	return TRUE;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Range_Time(CGUI_Dlg *pDlg) {

	GEN_TIME_STRUCT Start_Time, End_Time;


	pDlg->FldTimeGetValue(IDC_SEB_IAEA_INSPEC_REVIEW_START_TIME, &Start_Time);
	pDlg->FldTimeGetValue(IDC_SEB_IAEA_INSPEC_REVIEW_END_TIME, &End_Time);

	// Verify that starting date is equal to or less then ending date
	// NOTE: Must do as below, think about the logic!
	if ((Start_Time.hr > End_Time.hr) ||

		((Start_Time.hr == End_Time.hr) &&
		 (Start_Time.min > End_Time.min)) ||
		// If start hr less then end then nothing else matters
	
		((Start_Time.hr == End_Time.hr) &&
		 (Start_Time.min == End_Time.min) &&
		 (Start_Time.sec > End_Time.sec))) {
		// If start min less then end then sec does not matters

		// No message displayed because now must check date at least one less then current
		return FALSE;
		}


	return TRUE;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Review_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	// Check that the entered date is not beyond the current one
	if (!SEB_IAEA_Inspec_Date(pDlg, CtrlID))
		return FALSE;

	// Check that the entered time is not beyond the current one
	if ((!SEB_IAEA_Inspec_Time(pDlg, IDC_SEB_IAEA_INSPEC_REVIEW_TIME)) &&
		(!SEB_IAEA_Inspec_Date_Less(pDlg, CtrlID))) {
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_TIME);
		return TRUE;
		}

	return TRUE;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Review_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	// Check that the entered date is not beyond the current one
	if (!SEB_IAEA_Inspec_Date(pDlg, IDC_SEB_IAEA_INSPEC_REVIEW_DATE)) {
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_DATE);
		return TRUE;
		}

	// Check that the entered time is not beyond the current one
	if ((!SEB_IAEA_Inspec_Time(pDlg, CtrlID)) &&
		(!SEB_IAEA_Inspec_Date_Less(pDlg, IDC_SEB_IAEA_INSPEC_REVIEW_DATE)))
		return FALSE;

	return TRUE;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Start_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	// Check that the entered date is not beyond the current one
	if (!SEB_IAEA_Inspec_Date(pDlg, CtrlID))
		return FALSE;

	// Check that the entered time is not beyond the current one
	if ((!SEB_IAEA_Inspec_Time(pDlg, IDC_SEB_IAEA_INSPEC_REVIEW_START_TIME)) &&
		(!SEB_IAEA_Inspec_Date_Less(pDlg, CtrlID))) {
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_START_TIME);
		return TRUE;
		}

	// Check to see if this start date is greater then the end date
	// Get the entered date
	// Y2K - Year returned will be four digits
	if (!SEB_IAEA_Inspec_Range_Date(pDlg)) {
		return FALSE;
		}

	// Dates are OK, now check times
	if ((!SEB_IAEA_Inspec_Range_Time(pDlg)) &&
		(!SEB_IAEA_Inspec_Range_Date_Less(pDlg))) {
		// Change the focus over to the starting time
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_START_TIME);
		return TRUE;
		}

	return TRUE;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_End_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	// Check that the entered date is not beyond the current one
	if (!SEB_IAEA_Inspec_Date(pDlg, CtrlID))
		return FALSE;

	// Check that the entered time is not beyond the current one
	if ((!SEB_IAEA_Inspec_Time(pDlg, IDC_SEB_IAEA_INSPEC_REVIEW_END_TIME)) &&
		(!SEB_IAEA_Inspec_Date_Less(pDlg, CtrlID))) {
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_END_TIME);
		return TRUE;
		}

	// Check to see if this start date is greater then the end date
	// Get the entered date
	// Y2K - Year returned will be four digits
	if (!SEB_IAEA_Inspec_Range_Date(pDlg)) {
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_START_DATE);
		return TRUE;
		}

	// Dates are OK, now check times
	if ((!SEB_IAEA_Inspec_Range_Time(pDlg)) &&
		(!SEB_IAEA_Inspec_Range_Date_Less(pDlg))) {
		// Change the focus over to the starting time
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_START_TIME);
		return TRUE;
		}

	return TRUE;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Start_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	// Check that the entered date is not beyond the current one
	if (!SEB_IAEA_Inspec_Date(pDlg, IDC_SEB_IAEA_INSPEC_REVIEW_START_DATE)) {
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_START_DATE);
		return TRUE;
		}

	// Check that the entered time is not beyond the current one
	if ((!SEB_IAEA_Inspec_Time(pDlg, CtrlID)) &&
		(!SEB_IAEA_Inspec_Date_Less(pDlg, IDC_SEB_IAEA_INSPEC_REVIEW_START_DATE)))
		return FALSE;

	// Check to see if this start date is greater then the end date
	// Get the entered date
	// Y2K - Year returned will be four digits
	if (!SEB_IAEA_Inspec_Range_Date(pDlg)) {
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_START_DATE);
		return TRUE;
		}

	// Dates are OK, now check times
	if ((!SEB_IAEA_Inspec_Range_Time(pDlg)) &&
		(!SEB_IAEA_Inspec_Range_Date_Less(pDlg)))
		// Change the focus over to the starting time
		return FALSE;

	return TRUE;

}


BOOL SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_End_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	// Check that the entered date is not beyond the current one
	if (!SEB_IAEA_Inspec_Date(pDlg, IDC_SEB_IAEA_INSPEC_REVIEW_END_DATE)) {
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_END_DATE);
		return TRUE;
		}

	// Check that the entered time is not beyond the current one
	if ((!SEB_IAEA_Inspec_Time(pDlg, CtrlID)) &&
		(!SEB_IAEA_Inspec_Date_Less(pDlg, IDC_SEB_IAEA_INSPEC_REVIEW_END_DATE)))
		return FALSE;

	// Check to see if this start date is greater then the end date
	// Get the entered date
	// Y2K - Year returned will be four digits
	if (!SEB_IAEA_Inspec_Range_Date(pDlg)) {
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_START_DATE);
		return TRUE;
		}

	// Dates are OK, now check times
	if ((!SEB_IAEA_Inspec_Range_Time(pDlg)) &&
		(!SEB_IAEA_Inspec_Range_Date_Less(pDlg))) {
		// Change the focus over to the starting time
		pDlg->SetFocus(IDC_SEB_IAEA_INSPEC_REVIEW_START_TIME);
		return TRUE;
		}

	return TRUE;

}


void SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Add(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	CGStr Title, Add_String;
	CGUI_Dlg Inspection_ID_Box(IDD_SEB_IAEA_INSPECTION_ID_BOX, NULL, DLG_POSN_NO_SAVE);


	// Get the new inspection ID through the inspection ID box
	if (uiIcon)	Inspection_ID_Box.SetIcon(uiIcon);
	Title = pszApp;
	Title += " - Add New Inspector ID";
	Inspection_ID_Box.SetTitle(Title.Get_sz());
	Inspection_ID_Box.DefineFldTxt(IDC_SEB_IAEA_INSPEC_ID, &Add_String);
	Inspection_ID_Box.DefinePushBtn(IDC_SEB_IAEA_INSPEC_ID_OK, GUI_OK);
	Inspection_ID_Box.DefinePushBtn(IDC_SEB_IAEA_INSPEC_ID_CANCEL, GUI_CANCEL);
	if (GUI_OK == Inspection_ID_Box.Go()) {
		Inspection_ID_Box.FldTxtGetValue(IDC_SEB_IAEA_INSPEC_ID, &Add_String);
		if (strcmp(Add_String.Get_sz(), "") != 0)
			pInspect_Box->PL_MultiSelAdd(IDC_SEB_IAEA_INSPEC_ID_LIST, Add_String);
		}

}


void SEB_IAEA_Inspection_Box::SEB_IAEA_Inspec_Delete(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	UINT ctSEB_Index, uiSEB_Num_Items, uiSEB_Num_Items_Selected, ctSelected_Item_Number;															
	CGStr Message, SEB_String;														
	char **ppszSEB_Del_Strings; 
	BOOL bSEB_Selected, bSuccess = TRUE;	


	// Get the number of selected items and check that any are selected
	pInspect_Box->PL_MultiSelGetNumItems(IDC_SEB_IAEA_INSPEC_ID_LIST, &uiSEB_Num_Items);								
	PL_MULTISELGETNUMSEL(uiSEB_Num_Items_Selected, pInspect_Box, IDC_SEB_IAEA_INSPEC_ID_LIST, uiSEB_Num_Items);
	if (uiSEB_Num_Items_Selected == 0) {
		GUI_MsgBox("Please select the inspector ID(s) to be deleted.");
		return;
		}


	// Build the warning string
	ctSelected_Item_Number = 0;
	Message = "This will delete all of the selected inspectors.  Are you sure you want to delete the following inspectors?\n\n";
	for (ctSEB_Index = 0; ctSEB_Index < uiSEB_Num_Items; ctSEB_Index++) {					
		pInspect_Box->PL_MultiSelGetValue(IDC_SEB_IAEA_INSPEC_ID_LIST, ctSEB_Index, &bSEB_Selected, &SEB_String);
		if (bSEB_Selected) {
			ctSelected_Item_Number++;
			Message += "\"";
			Message += SEB_String;
			if (uiSEB_Num_Items_Selected == 1)
				Message += "\"";
			else if (uiSEB_Num_Items_Selected == 2) {
				if (ctSelected_Item_Number == uiSEB_Num_Items_Selected - 1)
					Message += "\" and ";
				else Message += "\"";
				}
			else {
				if (ctSelected_Item_Number == uiSEB_Num_Items_Selected - 1)
					Message += "\", and ";
				else if (ctSelected_Item_Number != uiSEB_Num_Items_Selected)
					Message += "\", ";
				else Message += "\"";
				}
			}
		}


	// Warn the user and get approval
	if (GUI_YES == GUI_MsgBox(Message.Get_sz(), GUI_ICON_QUESTION, GUI_YESNO)) {
		if ((ppszSEB_Del_Strings = new (char (* ([uiSEB_Num_Items])))) != NULL) {
			for (ctSEB_Index = 0; ctSEB_Index < uiSEB_Num_Items; ctSEB_Index++) {					
				pInspect_Box->PL_MultiSelGetValue(IDC_SEB_IAEA_INSPEC_ID_LIST, ctSEB_Index, &bSEB_Selected, &SEB_String);
				if ((ppszSEB_Del_Strings[ctSEB_Index] = new char [strlen(SEB_String.Get_sz()) + 1]) != NULL) {
					if (!bSEB_Selected)
						strcpy(ppszSEB_Del_Strings[ctSEB_Index], SEB_String.Get_sz());
					else
						ppszSEB_Del_Strings[ctSEB_Index][0] = NULL;
					}
				}
			pInspect_Box->PL_MultiSelClear(IDC_SEB_IAEA_INSPEC_ID_LIST);													
			for (ctSEB_Index = 0; ctSEB_Index < uiSEB_Num_Items; ctSEB_Index++) {					
				if (strcmp(ppszSEB_Del_Strings[ctSEB_Index], "") != 0)						
					pInspect_Box->PL_MultiSelAdd(IDC_SEB_IAEA_INSPEC_ID_LIST, ppszSEB_Del_Strings[ctSEB_Index]);	
				}
			for (ctSEB_Index = 0; ctSEB_Index < uiSEB_Num_Items; ctSEB_Index++)
				delete [] ppszSEB_Del_Strings[ctSEB_Index];
			delete [] ppszSEB_Del_Strings;
			}
		}

}


////////////////////////////////////////////////////////////////////////////////////
// SEB_Password_Request dialog box
////////////////////////////////////////////////////////////////////////////////////
// Creates and handles a password request dialog box
//    Simply create a SEB_Password_Request object and call Display
//    User can call Close to force close the opened box or delete the
//       SEB_Password_Request object which automatically closes and cleans up
//       after itself.
////////////////////////////////////////////////////////////////////////////////////    								
SEB_Password_Request::SEB_Password_Request(const char *pszValid_Password,
									const char *pszBox_Title,
									RESOURCE_ID Icon) :
									// The password to be checked 
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
	pszPassword(NULL),
	pszTitle(NULL),
	uiIcon(Icon),
	bPassword_Correct(FALSE),
	pBox(NULL),
	bOpen(FALSE)
	{
	
    if (pszValid_Password)
		if ((pszPassword = new char [strlen(pszValid_Password) + 1]) != NULL)
			strcpy(pszPassword, pszValid_Password);	
    if (pszBox_Title)
		if ((pszTitle = new char [strlen(pszBox_Title) + 1]) != NULL)
			strcpy(pszTitle, pszBox_Title);	
	    
}


SEB_Password_Request::~SEB_Password_Request(void) {

	Close();
	if (pBox)
		delete pBox;
	if (pszPassword)
		delete [] pszPassword;
	if (pszTitle)
		delete [] pszTitle;

}


// SEB_Password_Request starting member functions
GUI_ACTION SEB_Password_Request::Display(void) {

	BOOL bContinue = TRUE;
	GUI_ACTION bSuccess;


	bPassword_Correct = FALSE;
	while (bContinue) {
	
		if (!Setup_Box()) {
			bContinue = FALSE;
			bSuccess = GUI_NONE;
			}
		
		if (bContinue) {
			bOpen = TRUE;
			bSuccess = pBox->Go();
			bOpen = FALSE;
			if (((bSuccess == GUI_OK) && (bPassword_Correct)) ||
				(bSuccess == GUI_CANCEL))
				bContinue = FALSE;
			}
			
		}
		
	delete pBox;
	pBox = NULL;
                    
    
    if (bPassword_Correct)                
		return GUI_OK;
	else
		return bSuccess;  // Canceled or errored only possible
	
}


BOOL SEB_Password_Request::Setup_Box(void) {

	// Create the box
	if (!pBox) {
		if ((pBox = new CGUI_Dlg(IDD_SEB_PASSWORD_REQUEST_BOX, NULL, DLG_POSN_NO_SAVE)) != NULL) {
			if (uiIcon)
				pBox->SetIcon(uiIcon);
			pBox->SetTitle(pszTitle);
			pBox->DefineFldStaticTxt(IDC_SEB_PASSWORD_REQUEST_MESSAGE);
			pBox->FldStaticTxtSetValue(IDC_SEB_PASSWORD_REQUEST_MESSAGE, "Please enter the password to continue.");
			pBox->DefineFldTxt(IDC_SEB_PASSWORD_REQUEST_PASSWORD, &Entered_Password);
			pBox->DefinePushBtn(IDC_SEB_PASSWORD_REQUEST_OK, GUI_OK);
			pBox->SetCtrlNotifyFn(IDC_SEB_PASSWORD_REQUEST_OK, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgNotifyFn)&SEB_Password_Request::SEB_Password_Request_Verify);
			pBox->DefinePushBtn(IDC_SEB_PASSWORD_REQUEST_CANCEL, GUI_CANCEL);
			return TRUE;
			}
		}
	else {
		return pBox->FldStaticTxtSetValue(IDC_SEB_PASSWORD_REQUEST_MESSAGE, "Invalid password.  Please enter the password to continue.");
	    }

	
	return FALSE;
	
}


// SEB_Password_Request closing member functions
BOOL SEB_Password_Request::Close(void) {

	if (bOpen)
		return pBox->Close();
		
		
    return FALSE;
    
}


void SEB_Password_Request::SEB_Password_Request_Verify(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	pBox->FldTxtGetValue(IDC_SEB_PASSWORD_REQUEST_PASSWORD, &Entered_Password);
	if (strcmp(pszPassword, Entered_Password.Get_sz()) == 0)
		bPassword_Correct = TRUE;
    
}


////////////////////////////////////////////////////////////////////////////////////
// SEB_Password_Change dialog box
////////////////////////////////////////////////////////////////////////////////////
// Creates and handles a password changing dialog box
//    Simply create a SEB_Password_Change object and call Display
//    User can call Close to force close the opened box or delete the
//       SEB_Password_Change object which automatically closes and cleans up
//       after itself.
////////////////////////////////////////////////////////////////////////////////////    								
SEB_Password_Change::SEB_Password_Change(const char *pszBox_Title,
									RESOURCE_ID Icon) :
									// The password to be checked 
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
	pszTitle(NULL),
	uiIcon(Icon),
	bPassword_Same(FALSE),
	pBox(NULL),
	bOpen(FALSE)
	{
	
    if (pszBox_Title)
		if ((pszTitle = new char [strlen(pszBox_Title) + 1]) != NULL)
			strcpy(pszTitle, pszBox_Title);	
	    
}


SEB_Password_Change::~SEB_Password_Change(void) {

	Close();
	if (pBox)
		delete pBox;
	if (pszTitle)
		delete [] pszTitle;

}


// SEB_Password_Request starting member functions
GUI_ACTION SEB_Password_Change::Display(void) {

	BOOL bContinue = TRUE;
	GUI_ACTION bSuccess;


	bPassword_Same = FALSE;
	while (bContinue) {
	
		if (!Setup_Box()) {
			bContinue = FALSE;
			bSuccess = GUI_NONE;
			}
		
		if (bContinue) {
			bOpen = TRUE;
			bSuccess = pBox->Go();
			bOpen = FALSE;
			if (((bSuccess == GUI_OK) && (bPassword_Same)) ||
				(bSuccess == GUI_CANCEL))
				bContinue = FALSE;
			}
			
		}
		
	delete pBox;
	pBox = NULL;
                    
    
    if (bPassword_Same)                
		return GUI_OK;
	else
		return bSuccess;  // Canceled or errored only possible
	
}


BOOL SEB_Password_Change::Setup_Box(void) {

	// Create the box
	if (!pBox) {
		if ((pBox = new CGUI_Dlg(IDD_SEB_PASSWORD_CHANGE_BOX, NULL, DLG_POSN_NO_SAVE)) != NULL) {
			if (uiIcon)
				pBox->SetIcon(uiIcon);
			pBox->SetTitle(pszTitle);
			pBox->DefineFldStaticTxt(IDC_SEB_PASSWORD_CHANGE_MESSAGE);
			pBox->FldStaticTxtSetValue(IDC_SEB_PASSWORD_CHANGE_MESSAGE, "Please enter the new password twice.");
			pBox->DefineFldTxt(IDC_SEB_PASSWORD_CHANGE_PASSWORD1, &Entered_Password1);
			pBox->DefineFldTxt(IDC_SEB_PASSWORD_CHANGE_PASSWORD2, &Entered_Password2);
			pBox->DefinePushBtn(IDC_SEB_PASSWORD_CHANGE_OK, GUI_OK);
			pBox->SetCtrlNotifyFn(IDC_SEB_PASSWORD_CHANGE_OK, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgNotifyFn)&SEB_Password_Change::SEB_Password_Change_Verify);
			pBox->DefinePushBtn(IDC_SEB_PASSWORD_CHANGE_CANCEL, GUI_CANCEL);
			return TRUE;
			}
		}
	else {
		return pBox->FldStaticTxtSetValue(IDC_SEB_PASSWORD_CHANGE_MESSAGE, "Passwords do not match.  Please re-enter the password twice.");
	    }

	
	return FALSE;
	
}


// Get the new password - returns NULL if a new password was not set
const char *SEB_Password_Change::Get_Password(void) {

	if (bPassword_Same)
		return ((const char *)Entered_Password1.Get_sz());
	else
		return NULL;

}


// SEB_Password_Request closing member functions
BOOL SEB_Password_Change::Close(void) {

	if (bOpen)
		return pBox->Close();
		
		
    return FALSE;

}


void SEB_Password_Change::SEB_Password_Change_Verify(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	pBox->FldTxtGetValue(IDC_SEB_PASSWORD_CHANGE_PASSWORD1, &Entered_Password1);
	pBox->FldTxtGetValue(IDC_SEB_PASSWORD_CHANGE_PASSWORD2, &Entered_Password2);
	if (strcmp(Entered_Password1.Get_sz(), Entered_Password2.Get_sz()) == 0)
		bPassword_Same = TRUE;
		
}


////////////////////////////////////////////////////////////////////////////////////
// SEB_Progress_Bar dialog box
////////////////////////////////////////////////////////////////////////////////////
// Creates and handles a progress bar dialog box
//    Simply create a SEB_Progress_Bar object and repeatedly call it the
//       total number of times set
//    User can call Close to force close the opened box or delete the
//       SEB_Progress_Bar object which automatically closes and cleans up
//       after itself.
////////////////////////////////////////////////////////////////////////////////////    								

SEB_Progress_Bar::SEB_Progress_Bar(unsigned long ulTotal_Progress_Calls,
									unsigned long ulUpdates_Per_Call,
									const char *pszMessage,
									const char *pszBox_Title,
									RESOURCE_ID Icon,
									BOOL bAllow_Cancel) :
									// The total number of times will call Progress_Bar_Update
									// Number of calls before bar updates itself - screen updates take much time
									// Status message which appears under "Please Wait" message
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
									// Allow the progress bar to be canceled
									// The total number of times will call Progress_Bar_Update
									// Number of calls before bar updates itself - screen updates take much time
									// Status message which appears under "Please Wait" message
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
	pBox(NULL),
	uiIcon(Icon),
	pszTitle(NULL),
	pszMsg(NULL),
	ulTotal_Calls(ulTotal_Progress_Calls),
	ulPer_Call(ulUpdates_Per_Call),
	ulCall_Number(0),
	bCanceled(FALSE),
	bOpen(FALSE),
	bCan_Cancel(bAllow_Cancel)
	{

    if (pszMessage)
		if ((pszMsg = new char [strlen(pszMessage) + 1]) != NULL)
			strcpy(pszMsg, pszMessage);
    if (pszBox_Title)
		if ((pszTitle = new char [strlen(pszBox_Title) + 1]) != NULL)
			strcpy(pszTitle, pszBox_Title);
	// Make sure that ulPer_Call is not 0 - GPF when divide
	if (ulPer_Call == 0) ulPer_Call = 1;
	// Help prevent progress bar from being updated too much
	if ((ulTotal_Calls / ulPer_Call) > 100)
		ulPer_Call = ulTotal_Calls / 100;

	    
}


SEB_Progress_Bar::~SEB_Progress_Bar(void) {

	Close();
	if (pszMsg)
		delete [] pszMsg;
	if (pszTitle)
		delete [] pszTitle;

}


// Repeatively call up to number of times setFirst time called it displays box 
// Returns FALSE if cancel was pressed
// NOTE: Only need to pass message once for change
BOOL SEB_Progress_Bar::Progress_Bar_Update(const char *pszMessage) {

	// MUST HAVE FIRST, Must give time to determine cancel
	CGUI_App::ProcessEvents();

	// If first time then create the box and display it
	if (ulCall_Number == 0) {
	    if (Setup_Box()) {
	    	pBox->GoModeless(TRUE);
			bCanceled = FALSE;
	    	bOpen = TRUE;
	    	}
	    else
	    	bCanceled = TRUE;
		}
	
	// Update the progress box
    if (!bCanceled) {
    	ulCall_Number++;
    	if (ulCall_Number <= ulTotal_Calls) {
    		if (((ulCall_Number % ulPer_Call) == 0) || (ulCall_Number == ulTotal_Calls)) {
				pBox->ProgressBarSetValue(IDC_SEB_PROGRESS_BAR_BAR, ((double)ulCall_Number/(double)ulTotal_Calls));
				if (pszMessage)
					pBox->FldStaticTxtSetValue(IDC_SEB_PROGRESS_BAR_MESSAGE, pszMessage);
				// Keep so that bar gets displayed including when reach 100%
				CGUI_App::ProcessEvents();
				}
    		}
    		
    	if (ulCall_Number == ulTotal_Calls) {
			// Remain open until the user actually closes or cancels it
			//if (bOpen) {
			//	pBox->Close();
			//	CGUI_App::ProcessEvents();
    		//	bOpen = FALSE;
			//	}
			}
    	}
	else {
		if (bOpen) {
			pBox->Close();
			CGUI_App::ProcessEvents();
    		bOpen = FALSE;
			}
		}
           
    
	return (!bCanceled);
	
}


BOOL SEB_Progress_Bar::Setup_Box(void) {

	// Create the box
	if (((bCan_Cancel) && ((pBox = new CGUI_Dlg(IDD_SEB_PROGRESS_BAR, NULL, DLG_POSN_NO_SAVE)) != NULL)) ||
		((!bCan_Cancel) && ((pBox = new CGUI_Dlg(IDD_SEB_PROGRESS_BAR_NO_CANCEL, NULL, DLG_POSN_NO_SAVE)) != NULL))) {
		if (uiIcon)
			pBox->SetIcon(uiIcon);
		if (pszTitle)
			pBox->SetTitle(pszTitle);
		if (pszMsg)
			pBox->DefineFldStaticTxt(IDC_SEB_PROGRESS_BAR_MESSAGE, pszMsg);
		if (bCan_Cancel) {
			pBox->DefinePushBtn(IDC_SEB_PROGRESS_BAR_CANCEL, GUI_CANCEL); // Keep cancel so that X box in corner works - NOTE: Will GPF if close box when canceled
			pBox->SetCtrlNotifyFn(IDC_SEB_PROGRESS_BAR_CANCEL, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgNotifyFn)&SEB_Progress_Bar::SEB_Progress_Bar_Cancel);
			}
		pBox->DefineProgressBar(IDC_SEB_PROGRESS_BAR_BAR, TRUE);
    	pBox->ProgressBarSetColors(IDC_SEB_PROGRESS_BAR_BAR, RGB(0, 128, 0), RGB(128, 128, 128));
		return TRUE;
		}

	
	return FALSE;
	
}


BOOL SEB_Progress_Bar::Progress_Bar_Reset(unsigned long ulTotal_Progress_Calls, 
									unsigned long ulUpdates_Per_Call,
									const char *pszMessage,
									const char *pszBox_Title,
									RESOURCE_ID Icon) {
									// The total number of times will call Progress_Bar_Update
									// Number of calls before bar updates itself - screen updates take much time
									// Status message which appears under "Please Wait" message
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
	
	BOOL bSuccess = TRUE;

	
	Close();
	// Don't set new unless passed
	if (ulTotal_Progress_Calls)								
		ulTotal_Calls = ulTotal_Progress_Calls;
	if (ulUpdates_Per_Call)								
		ulPer_Call = ulUpdates_Per_Call;
    if (pszMessage) {
		if ((pszMsg = new char [strlen(pszMessage) + 1]) != NULL)
			strcpy(pszMsg, pszMessage);
		else
			bSuccess = FALSE;
		}	
    if (pszBox_Title) {
		if ((pszTitle = new char [strlen(pszBox_Title) + 1]) != NULL)
			strcpy(pszTitle, pszBox_Title);
		else
			bSuccess = FALSE;	
		}
	uiIcon = Icon;

	// Make sure that ulPer_Call is not 0 - GPF when divide
	if (ulPer_Call == 0) ulPer_Call = 1;
	// Help prevent progress bar from being updated too much
	if ((ulTotal_Calls / ulPer_Call) > 100)
		ulPer_Call = ulTotal_Calls / 100;

	// Status variables
	ulCall_Number = 0;
	bCanceled = FALSE;
	bOpen = FALSE;	

	
	return bSuccess;
									
}
									

// SEB_Password_Request closing member functions
BOOL SEB_Progress_Bar::Close(void) {
    
    if (bOpen) {
		return pBox->Close();
		CGUI_App::ProcessEvents();
		bOpen = FALSE;
		}
	
	
	return FALSE;

}


void SEB_Progress_Bar::SEB_Progress_Bar_Cancel(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	bCanceled = TRUE;
	bOpen = FALSE;
	
}


