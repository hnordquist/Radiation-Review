
/* ProgressBars.H  Function Prototypes */

#ifndef ProgressBars_H

	#define ProgressBars_H

#include "upperlayer\SEBBoxes.h"
#include "resource.h"


////////////////////////////////////////////////////////////////////////////////////
// Overrides one of the functions in SEB_Progress_Bar to provide a question asked
//	after the Cancel button is clicked.
////////////////////////////////////////////////////////////////////////////////////    								
class ProgressBarWithQuestion : public SEB_Progress_Bar 
{
	public:

	ProgressBarWithQuestion(unsigned long ulTotal_Progress_Calls, // The total number of times will call Progress_Bar_Update
							unsigned long ulCalls_Per_Update,	  // Number of calls before bar updates itself - screen updates take much time
							const char *pszMessage = NULL,		  // Status message which appears under "Please Wait" message
							const char *pszBox_Title = NULL,	  // Name of application to appear in title bar
							RESOURCE_ID Icon = IDI_NEW_RAD_REVIEW,			  // The application icon to appear as the system menu (Windows 95)
							BOOL bAllow_Cancel = TRUE);			  // Allow the progress bar to be canceled);

	virtual ~ProgressBarWithQuestion(void); 


    protected:
	virtual void SEB_Progress_Bar_Cancel(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
	virtual BOOL Setup_Box(void);
};

////////////////////////////////////////////////////////////////////////////////////
//	Class that supports progress bars that have two lines of information
//  Modeled after Steve's single line progress bar
////////////////////////////////////////////////////////////////////////////////////    								
class TwoLineProgressBar : public CGUI_CallBack 
{
	public:
		// TwoLineProgressBar Parameter definitions:
		// ulTotalProgressCalls: The total number of times will call Progress_Bar_Update
		// ulCallsPerUpdate: Number of calls before bar updates itself - screen updates take much time
		// pszMessage: Status message which appears under "Please Wait" message
		// pszBoxTitle: Name of application to appear in title bar
		// bCancelQuestion: Whether to ask question when canceled
		// Icon: The application icon to appear as the system menu (Windows 95)
		TwoLineProgressBar(unsigned long ulTotalProgressCalls,
									unsigned long ulUpdatesPerCall,
									const char *pszLine1 = NULL,
									const char *pszLine2 = NULL,
									const char *pszBoxTitle = NULL,
									BOOL bCancelQuestion = TRUE,
									RESOURCE_ID Icon = IDI_ICON1);

		virtual ~TwoLineProgressBar(void); 

		// Repeatively call up to number of times set at creation, bar updated only when number of 
		//		ulCallsPerUpdate reached.
		// Returns FALSE if cancel was pressed
		// NOTE: Only need to pass message once for change
		BOOL TwoLineProgressBarUpdate(const char *pszLine1 = NULL, const char *pszLine2 = NULL);
		BOOL TwoLineProgressBar::TwoLineProgressBarSetValue(double dValue, const char *pszLine1 = NULL, const char *pszLine2 = NULL) ;

        // NOTE: If no parameters passed then last values kept
		BOOL TwoLineProgressBarReset(unsigned long ulTotalProgressCalls, 
			   						 unsigned long ulUpdatesPerCall,
 									 const char *pszLine1 = NULL,
									 const char *pszLine2 = NULL,
									 const char *pszBoxTitle = NULL,
									 RESOURCE_ID Icon = IDI_ICON1);
									 // The total number of times will call Progress_Bar_Update
									 // Number of calls before bar updates itself - screen updates take much time
									 // Status message which appears under "Please Wait" message
									 // Name of application to appear in title bar
									 // The application icon to appear as the system menu (Windows 95)

		// Will close and destroy open box
		BOOL Close(void);  
		
    
	protected:
		CGUI_Dlg *mpBox;
		RESOURCE_ID muiIcon;
		char *mpszTitle, *mpszLine1, *mpszLine2;
		unsigned long mulTotalCalls, mulCallNumber, mulPerCall;
		BOOL mbCanceled, mbOpen;
		BOOL  mbCancelQuestion;

		BOOL SetupBox(void);
		virtual void TwoLineProgressBarCancel(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) ;
};


#endif
