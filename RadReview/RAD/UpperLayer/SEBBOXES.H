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
// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/18/97

// SEBBoxes.H
// Various generic dialog boxes I have found to useful
// NOTE: Be sure to include the appropriate resources!!!



#ifndef SEBBOXES_H

	#define SEBBOXES_H
                       
                       
	#include <gui_lib.h>
	#include <gen.h>
    #include "resource.h"
    
    #include <stdlib.h>
    #include <time.h>
    
    #include "SEBMacro.H"


	#define SEBBOXES_VERSION	__TIMESTAMP__
                       
    
    #define uiMAX_SEB_BOXES		6  	// The maximum number of boxes to create in the
    								//    classes which can create more then one.
    								// I don't recommend increasing number because
    								//    there is an overall displayed limit of 12
    								//    in zApp.
	// Define the following below to avoid compilation errors for the boxes which you will
	// not use.
	// USE_SEB_MODELESS_MESSAGE
	// USE_SEB_QESTION_BOX
    // USE_SEB_THREE_QUESTION_BOX
    // USE_SEB_ABOUT_BOX
    // USE_SEB_PRODUCT_SUPPORT_UC_BOX
    // USE_SEB_IAEA_INSPECTION_BOX
    // USE_SEB_PASSWORD_REQUEST
    // USE_SEB_PASSWORD_CHANGE
    

	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles multiple modeless message boxes
	//    Simply create a SEB_Modeless_Message object and repeatively call New to
	//       create a new message box.
	//    A SEB_Modeless_Message must be created on the heap otherwise destructor
	//       called on function exit
	//    User can call Close_All to close all of the opened messages or delete the
	//       SEB_Modeless_Message object which automatically closes and cleans up
	//       after itself.
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_Modeless_Message : public CGUI_CallBack {
		public:
			SEB_Modeless_Message(const char *pszBox_Title,
									RESOURCE_ID Icon = NULL); 
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
			SEB_Modeless_Message(const char *pszBox_Title,
									const char *pszMessage,
									RESOURCE_ID Icon = NULL); 
									// Name of application to appear in title bar
									// Message to appear in the box
									// The application icon to appear as the system menu (Windows 95)
			virtual ~SEB_Modeless_Message(void); 
			// Can call repeatedily and will create a new one each time - up to uiMAX_MODELESS_MESSAGES possible
			BOOL New(BOOL bUse_Small_Question_Box = TRUE);
			BOOL New(const char *pszMessage, BOOL bSet_Message = FALSE, BOOL bUse_Small_Question_Box = TRUE);     		
			// Will close and destroy all open modeless messages
			BOOL Close_All(void);     		
        protected:
			char *pszMsg, *pszTitle;
			const char *pszUse;
			RESOURCE_ID uiIcon;
			struct SEB_BOXES {
				CGUI_Dlg *pBox;
				BOOL bOpen;
				} pBoxes[uiMAX_SEB_BOXES];
			BOOL Set_Message(const char *pszMessage);
			BOOL Modeless_Exit(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);
		};



	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles multiple YES/NO question boxes
	//    Has all of the properties of a SEB_Modeless_Message object, however it
	//       provides YES/NO buttons and notifications of the response.
	//    A SEB_Question_Box must be created on the heap otherwise destructor
	//       called on function exit
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_Question_Box : public SEB_Modeless_Message {
		public:
			SEB_Question_Box(const char *pszBox_Title,
								CGUI_CallBack *pCB_Obj = NULL,
								GUI_CB_DlgNotifyFn pYES_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pNO_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL,
								BOOL bUse_Small_Question_Box = TRUE); 
								// Name of application to appear in title bar
								// Your object - used for notification of button press
								// Your YES callback member function - used for notification of YES button press
								// Your NO callback member function - used for notification of NO button press
								// The application icon to appear as the system menu (Windows 95)
								// Whether to use a small box or large box
			SEB_Question_Box(const char *pszBox_Title,
								const char *pszQuestion,
								CGUI_CallBack *pCB_Obj = NULL,
								GUI_CB_DlgNotifyFn pYES_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pNO_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL,
								BOOL bUse_Small_Question_Box = TRUE); 
								// Name of application to appear in title bar
								// Question to appear in the box
								// Your object - used for notification of button press
								// Your YES callback member function - used for notification of YES button press
								// Your NO callback member function - used for notification of NO button press
								// The application icon to appear as the system menu (Windows 95)
								// Whether to use a small box or large box
			virtual ~SEB_Question_Box(void); 
			// Sets the callbacks for the next go
			BOOL Set_Callbacks(CGUI_CallBack *pCB_Obj,
								GUI_CB_DlgNotifyFn pYES_NotifyFn,
								GUI_CB_DlgNotifyFn pNO_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL);
			// Can call repeatedily and will create a new one each time - up to uiMAX_MODELESS_MESSAGES possible
			// Note: Modeless returns GUI_OK if worked, both return GUI_NONE if error, otherwise button press GUI_YES or GUI_NO
			GUI_ACTION New(BOOL bModeless = TRUE);
			GUI_ACTION New(const char *pszQuestion, BOOL bModeless = TRUE, BOOL bSet_Message = FALSE);
			// Return the number of buttons
			virtual UINT Get_Number_Buttons(void);
			// Disables all of the displayed buttons of all boxes
			void Disable_All_Buttons(void);
			// Will close and destroy all open modeless questions
			BOOL Close_All(void);     		
		protected:
			UINT uiNumber_Buttons;
        private:
			CGUI_CallBack *pObj;
			GUI_CB_DlgNotifyFn pYES, pNO;
			BOOL bSmall_Question_Box;
		};



	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles multiple three question boxes
	//    Allows user to select amongst three choices which are set through the
	//       constructor
	//    Choices for model and modeless
	//    A SEB_Three_Question_Box must be created on the heap if used as modeless 
	//       otherwise destructor called on function exit
	//    Buttons not defined result in GUI_CANCEL 
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_Three_Question_Box : public SEB_Question_Box {
		public:
			SEB_Three_Question_Box(const char *pszBox_Title,
								const char *pszButton_1_Text,
								const char *pszButton_2_Text,
								const char *pszButton_3_Text,
								CGUI_CallBack *pCB_Obj = NULL,
								GUI_CB_DlgNotifyFn pButton1_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pButton2_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pButton3_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL,
								BOOL bUse_Small_Question_Box = TRUE); 
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
			SEB_Three_Question_Box(const char *pszBox_Title,
								const char *pszQuestion,
								const char *pszButton_1_Text,
								const char *pszButton_2_Text,
								const char *pszButton_3_Text,
								CGUI_CallBack *pCB_Obj = NULL,
								GUI_CB_DlgNotifyFn pButton1_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pButton2_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pButton3_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL,
								BOOL bUse_Small_Question_Box = TRUE); 
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
			virtual ~SEB_Three_Question_Box(void); 
			// Sets the callbacks for the next go
			BOOL Set_Callbacks(CGUI_CallBack *pCB_Obj,
								GUI_CB_DlgNotifyFn pButton1_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pButton2_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pButton3_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL);
			// Can call repeatedily and will create a new one each time - up to uiMAX_MODELESS_MESSAGES possible
			// Note: Modeless returns GUI_OK if worked, both return GUI_NONE if error, otherwise button press GUI_YES for #1, GUI_NO for #2, or GUI_CANCEL for #3
			GUI_ACTION New(BOOL bModeless = TRUE);
			GUI_ACTION New(const char *pszQuestion, BOOL bModeless = TRUE, BOOL bSet_Message = FALSE);
			// Disables all of the displayed buttons of all boxes
			void Disable_All_Buttons(void);
			// Will close and destroy all open modeless questions
			BOOL Close_All(void);     		
        private:
			CGUI_CallBack *pObj;
			GUI_CB_DlgNotifyFn pBut1, pBut2, pBut3;
			char *pszBut1_Text, *pszBut2_Text, *pszBut3_Text;
			BOOL bSmall_Question_Box;
		};



	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles my standard About Box
	//    Simply instantiate a SEB_About_Box object and call display.
	//    Note: This dialog box is model and requires a response before closing.
	//    When user deletes object it will automatically cleans up after itself.
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_About_Box : public CGUI_CallBack {
		public:
			SEB_About_Box(const char *pszApplication_Name = NULL,
							const char *pszApplication_Version = NULL,
							const char *pszReview_Tool_Version = NULL,
							const char *pszFacility_Name = NULL,
							const char *pszLocation_Name = NULL,
							RESOURCE_ID App_Icon = NULL);
							// Name of application
							// Version of application - string allows "alpha", etc.
							// Version of upper layer - string allows "alpha", etc.
							// Name of facility
							// Name of location in facility
							// The application icon to appear as the system menu (Windows 95)
							//    Note: This is not the icon which appears in the upper left corner,
							//          you must set that yourself in AppStudio
			virtual ~SEB_About_Box(void); 
			// Creates dynamically and displays the SEB_About_Box
			BOOL Display(void);
			// Will force the SEB_About_Box to close and get deleted
			BOOL Close(void);     		
        protected:
			char *pszName, *pszApp_Version, *pszRT_Version, *pszFacility, *pszLocation;

			RESOURCE_ID uiIcon;
			CGUI_Dlg *pAbout_Box;
		};



	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles my standard Product Support Box
	//    Simply instantiate a SEB_Product_Support_UC_Box object and call display.
	//    Note: This dialog box MUST be modeless! A SEB_Product_Support_Box must
	//             be created on the heap otherwise destructor called on function exit
	//    User can call Close to close the opened SEB_Product_Support_Box or delete
	//       the SEB_Modeless_Message object which automatically closes and cleans up
	//       after itself.
	//    Please note the two differences in shutting the SEB_Product_Support_Box
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_Product_Support_Box : public CGUI_CallBack {
		public:
			SEB_Product_Support_Box(const char *pszApp_Name,
							const char *pszContact_Name,
							const char *pszContact_Email,
							RESOURCE_ID App_Icon = NULL);
							// Name of application
							// The name of the person to contact for questions, bug reports, etc.
							// The email address of the person to contact for questions, bug reports, etc.
							// The application icon to appear as the system menu (Windows 95)
			virtual ~SEB_Product_Support_Box(void); 
			// Creates dynamically and displays the SEB_Product_Support_Box
			BOOL Display(void);
        protected:
			char *pszApp, *pszName, *pszEmail;
			RESOURCE_ID uiIcon;
		};



	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles the standard IAEA inspection information and box
	//    Simply instantiate a SEB_IAEA_Inspection_Box object with the required
	//       structure COMPLETELY filled.
	//    The users button press is returned from display.
	//    The users inspection information is passed back through the same structure
	//       passed in.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_IAEA_Inspection_Box : public CGUI_CallBack {
		public:
			struct IAEA_INSPECTION_INFO {
				// Y2K - All years are four digits
				char *pszFacility_Location;			// The location of the facility - *Won't change
				GEN_DATE_STRUCT *pReview_Date;		// Date of review being conducted - *Could change!
				GEN_TIME_STRUCT *pReview_Time;		// Time of review being conducted - *Could change!
				UINT uiInspector_ID_Num;			// The number of inspector IDs - *Could change!
				char **ppszInspector_ID;			// The list of inspector names and organizations - *Could change! - *Last item must be NULL!
				UINT uiInspector_ID_Choices_Num;	// The number of inspector choices - *Could change!
				UINT *puiInspector_ID_Choices;		// The list of choices of participating inspector ID's - *Could change!
				char *pszInspection_Number;			// The inspection number - *Could change! Must be a string because could have leading 00 if greater then 1999
				GEN_DATE_STRUCT *pStart_Date;		// The starting date of inspection data - *Could change! 
				GEN_TIME_STRUCT *pStart_Time;		// The starting time of inspection data - *Could change! 
				GEN_DATE_STRUCT *pEnd_Date;			// The ending date of inspection data - *Could change!
				GEN_TIME_STRUCT *pEnd_Time;			// The ending time of inspection data - *Could change!
				char *pszComments;					// The inspection comments - *Could change!
				};
			SEB_IAEA_Inspection_Box(const char *pszApp_Name,
							char **ppszFacility_Name,
							struct IAEA_INSPECTION_INFO *pIAEA_Inspection_Info,
							RESOURCE_ID App_Icon = NULL);
							// Name of the application
							// Name of facility - *Could change!
							// Structure containing box inital and result information
							// The application icon to appear as the system menu (Windows 95)
			virtual ~SEB_IAEA_Inspection_Box(void);
			// Displays the SEB_IAEA_Inspection_Box - return GUI_OK, GUI_CANCEL, or GUI_NONE (Error) 
			GUI_ACTION Display(BOOL bSet_To_New = FALSE);
			// Closes the SEB_IAEA_Inspection_Box
			BOOL Close(void);
			// Deletes an IAEA_INSPECTION_INFO structure completely
			BOOL Delete_IAEA_INSPECTION_INFO(struct IAEA_INSPECTION_INFO **ppIAEA_Inspection_Info);
        protected:
			char *pszApp, **ppszFacility;
			CGStr Temp_Add, Temp_Comments, Temp_Inspection_Number;
			struct IAEA_INSPECTION_INFO *pInfo;
			RESOURCE_ID uiIcon;
			CGUI_Dlg *pInspect_Box;
			BOOL Setup_Box(void);
			void Box_Startup(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void Set_To_New(void);
			void SEB_IAEA_Inspec_Get_Values(void);
			BOOL SEB_IAEA_Inspec_Number(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Date_Less(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Range_Date(CGUI_Dlg *pDlg);
			BOOL SEB_IAEA_Inspec_Range_Date_Less(CGUI_Dlg *pDlg);
			BOOL SEB_IAEA_Inspec_Range_Time(CGUI_Dlg *pDlg);
			BOOL SEB_IAEA_Inspec_Review_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Review_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Start_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_End_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Start_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_End_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void SEB_IAEA_Inspec_Add(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void SEB_IAEA_Inspec_Delete(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		};



		
	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles a password request dialog box
	//    Simply create a SEB_Password_Request object and call Display
	//    User can call Close to force close the opened box or delete the
	//       SEB_Password_Request object which automatically closes and cleans up
	//       after itself.
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_Password_Request : public CGUI_CallBack {
		public:
			SEB_Password_Request(const char *pszValid_Password,
									const char *pszBox_Title,
									RESOURCE_ID Icon = NULL);
									// The password to be checked 
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
			virtual ~SEB_Password_Request(void); 
			// Displays the SEB_Password_Request - returns GUI_OK password correct, GUI_CANCEL, or GUI_NONE (Error) 
			GUI_ACTION Display(void);
			// Will close and destroy open box
			BOOL Close(void);     		
        protected:
			char *pszTitle;
			RESOURCE_ID uiIcon;
			char *pszPassword;
			CGUI_Dlg *pBox;
			BOOL bOpen, bPassword_Correct;
			CGStr Entered_Password;
			BOOL Setup_Box(void);
			void SEB_Password_Request_Verify(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		};



	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles a password changing dialog box
	//    Simply create a SEB_Password_Change object and call Display
	//    User can call Close to force close the opened box or delete the
	//       SEB_Password_Change object which automatically closes and cleans up
	//       after itself.
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_Password_Change : public CGUI_CallBack {
		public:
			SEB_Password_Change(const char *pszBox_Title,
									RESOURCE_ID Icon = NULL); 
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
			virtual ~SEB_Password_Change(void); 
			// Displays the SEB_Password_Change - return GUI_OK, GUI_CANCEL, or GUI_NONE (Error) 
			GUI_ACTION Display(void);
			// Get the new password - returns NULL if a new password was not set
			const char *Get_Password(void);
			// Will close and destroy open box
			BOOL Close(void);     		
        protected:
			char *pszTitle;
			RESOURCE_ID uiIcon;
			CGUI_Dlg *pBox;
			BOOL bOpen, bPassword_Same;
			CGStr Entered_Password1, Entered_Password2;
			BOOL Setup_Box(void);
			void SEB_Password_Change_Verify(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		};



	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles a progress bar dialog box
	//    Simply create a SEB_Progress_Bar object and repeatedly call it the
	//       total number of times set
	//    User can call Close to force close the opened box or delete the
	//       SEB_Progress_Bar object which automatically closes and cleans up
	//       after itself.
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_Progress_Bar : public CGUI_CallBack {
		public:
			SEB_Progress_Bar(unsigned long ulTotal_Progress_Calls,
									unsigned long ulCalls_Per_Update, 
									const char *pszMessage = NULL,
									const char *pszBox_Title = NULL,
									RESOURCE_ID Icon = NULL,
									BOOL bAllow_Cancel = TRUE);
									// The total number of times will call Progress_Bar_Update
									// Number of calls before bar updates itself - screen updates take much time
									// Status message which appears under "Please Wait" message
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
									// Allow the progress bar to be canceled
			virtual ~SEB_Progress_Bar(void); 
			// Repeatively call up to number of times setFirst time called it displays box 
			// Returns FALSE if cancel was pressed
			// NOTE: Only need to pass message once for change
            BOOL Progress_Bar_Update(const char *pszMessage = NULL);
            // NOTE: If no parameters passed then last values kept
            BOOL Progress_Bar_Reset(unsigned long ulTotal_Progress_Calls = NULL, 
									unsigned long ulCalls_Per_Update = NULL, 
									const char *pszMessage = NULL,
									const char *pszBox_Title = NULL,
									RESOURCE_ID Icon = NULL);
			// Will close and destroy open box
			BOOL Close(void);     		
        protected:
			CGUI_Dlg *pBox;
			RESOURCE_ID uiIcon;
			char *pszTitle, *pszMsg;
			unsigned long ulTotal_Calls, ulCall_Number, ulPer_Call;
			BOOL bCanceled, bOpen;
			virtual BOOL Setup_Box(void);
			BOOL bCan_Cancel;
			virtual void SEB_Progress_Bar_Cancel(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		};



#endif


		
