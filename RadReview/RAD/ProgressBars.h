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
