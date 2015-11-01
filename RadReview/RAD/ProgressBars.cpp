///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//19 Apr 2006 - pjm
//Changed code to reduce compiler warnings.
///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
//    File Name : ProgressBars.cpp     
//         Name : Shirley Klosterbuer
//         Date : 07/17/2001
//  Description : This file contains that provide the glue to link the existing
//					GUI classes associated with Progress Bar to new part of Rad
//				  It also contains a class derived from SEB_Progress_Bar
//				  It also contains a class TwoLineProgressBar.

#include "Rad.h"
#include "upperlayer/SEBBoxes.h"
#include "ProgressGlue.h"
#include "ProgressBars.h"
	
class ProgressBarWithQuestion *pBar = NULL;
class TwoLineProgressBar *p2LineBar = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Provide these simple function links between the "new" Rad and the GUI based stuff
void ProgressBarCreate(unsigned long ulTotal_Progress_Calls, unsigned long ulCalls_Per_Update, const char *pszMessage, const char *pszBox_Title)
{
	pBar = new ProgressBarWithQuestion(ulTotal_Progress_Calls, ulCalls_Per_Update, pszMessage, pszBox_Title);

}

bool ProgressBarUpdate(const char *pszMessage)
{

	if (pBar) {
		return(TRUE == (pBar->Progress_Bar_Update(pszMessage))); //Get rid of compiler warning 20-Mar-2006 PJM
	}
	else {
		return(false);
	}
}

bool ProgressBarClose(void)
{
	bool bStatus = false;

	if (pBar) 
	{
		bStatus = (TRUE == pBar->Close()); //Get rid of compiler warning 20-Mar-2006 PJM
		pBar = NULL;
	}
	return(bStatus);
}

//To use in new part of
//strTemp.Format("Processing station %s (0 of %ld points)", m_strTableName,  ulEntireNumPts);
//ProgressBarCreate(ulEntireNumPts, ulEntireNumPts/20, strTemp, "Radiation Review: Raw Data Summary Search");

//strTemp.Format("Processing station %s (%ld of %ld points)", m_strTableName, ulNumPtsProcessed, ulEntireNumPts);
//bContinue = ProgressBarUpdate(strTemp);
//if (!bContinue) {
//	OutputTextLine(pWin, "\nSearch Aborted by User");
//	break;	// search was aborted
//}

//ProgressBarClose();


void TwoLineProgressBarCreate(unsigned long ulTotalProgressCalls, unsigned long ulCallsPerUpdate, const char *pszLine1, const char *pszLine2, const char *pszBoxTitle, bool bCancelQuestion)
{
	p2LineBar = new TwoLineProgressBar(ulTotalProgressCalls, ulCallsPerUpdate, pszLine1, pszLine2, pszBoxTitle, bCancelQuestion);

}

bool TwoLineProgressBarUpdate(const char *pszLine1, const char *pszLine2)
{
	if (p2LineBar) {
		return(TRUE == (p2LineBar->TwoLineProgressBarUpdate(pszLine1, pszLine2))); //Get rid of compiler warning 20-Mar-2006 PJM
	}
	else {
		return(false);
	}
}

bool TwoLineProgressBarClose(void)
{
	bool bStatus = false;

	if (p2LineBar) {
		bStatus = (TRUE == (p2LineBar->Close())); //Get rid of compiler warning 20-Mar-2006 PJM
		p2LineBar = NULL;
	}
	return(bStatus);
}

// To use in new part of Rad:
//strSta.Format("Processing station %s ", m_strTableName);
//strTemp.Format("Searched %ld of %ld points)", ulPtNum,  ulEntireNumPts);
//TwoLineProgressBarCreate(ulEntireNumPts, ulEntireNumPts/20, strSta, strTemp, "Radiation Review: Raw Data Summary Search");

//strTemp.Format("Searched %ld of %ld points)", ulPtNum,  ulEntireNumPts);
//bContinue = TwoLineProgressBarUpdate(strSta, strTemp);
//if (!bContinue) {
//	OutputTextLine(pWin, "\nSearch Aborted by User");
//	break;	// search was aborted
//}

//TwoLineProgressBarClose();



////////////////////////////////////////////////////////////////////////////////////
// ProgressBarWithQuestion Class
////////////////////////////////////////////////////////////////////////////////////
// Use SEB_Progress_Bar and overrides the Cancel function to provide asking a question
// "Are you sure" when canceled.
////////////////////////////////////////////////////////////////////////////////////    								

ProgressBarWithQuestion::ProgressBarWithQuestion(unsigned long ulTotal_Progress_Calls, 
								unsigned long ulCalls_Per_Update,	  
								const char *pszMessage,		  
								const char *pszBox_Title,	  
								RESOURCE_ID Icon,			  
								BOOL bAllow_Cancel)			  
: SEB_Progress_Bar(ulTotal_Progress_Calls, ulCalls_Per_Update, pszMessage, pszBox_Title, Icon, bAllow_Cancel)
{


}

ProgressBarWithQuestion::~ProgressBarWithQuestion(void) 
{

}



// override protected function to ask the question
void ProgressBarWithQuestion::SEB_Progress_Bar_Cancel(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) 
{
		
	if (GUI_MsgBox("Are you sure you want to cancel the search process?", GUI_ICON_QUESTION, GUI_YESNO) == GUI_YES) {

	//if (MessageBox(NULL, "Are you sure you want to cancel?", pszTitle, MB_YESNO | MB_TASKMODAL) == IDYES) {
		bCanceled = TRUE;
	}
}

BOOL ProgressBarWithQuestion::Setup_Box(void) {

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
			pBox->DefinePushBtn(IDC_SEB_PROGRESS_BAR_CANCEL, GUI_NONE); 
			pBox->SetCtrlNotifyFn(IDC_SEB_PROGRESS_BAR_CANCEL, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgNotifyFn)&SEB_Progress_Bar::SEB_Progress_Bar_Cancel);
			}
		pBox->DefineProgressBar(IDC_SEB_PROGRESS_BAR_BAR, TRUE);
    	pBox->ProgressBarSetColors(IDC_SEB_PROGRESS_BAR_BAR, RGB(0, 128, 0), RGB(128, 128, 128));
		// keep this progress bar always on top
		::SetWindowPos(pBox->GetWndHandle(), HWND_TOP, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
		return TRUE;
		}

	
	return FALSE;
	
}

//To Use:
//ProgressBarWithQuestion *pProgressBar;
//pProgressBar = new ProgressBarWithQuestion(ulTotalProgressCalls, ulUpdatesPerCall, szLine1, szBoxTitle);
//bCancel = pProgressBar->Progress_Bar_Update(szLine1);
//pProgressBar->Close();
//delete pProgressBar

		
		//strTemp.Format("Processing station %s (0 of %ld points)", m_strTableName,  ulEntireNumPts);
		//ProgressBarCreate(ulEntireNumPts, ulEntireNumPts/20, strTemp, "Radiation Review: Raw Data Summary Search");

		//strTemp.Format("Processing station %s (%ld of %ld points)", m_strTableName, ulNumPtsProcessed, ulEntireNumPts);
		//bContinue = ProgressBarUpdate(strTemp);
		//if (!bContinue) {
		//	OutputTextLine(pWin, "\nSearch Aborted by User");
		//	break;	// search was aborted
		//}

		//ProgressBarClose();


	

////////////////////////////////////////////////////////////////////////////////////
// TwoLineProgressBar Class
////////////////////////////////////////////////////////////////////////////////////
// Creates and handles a progress bar dialog box
//    Simply create a TwoLineProgressBar object and repeatedly call it the
//       total number of times set
//    User can call Close to force close the opened box or delete the
//       TwoLineProgressBar object which automatically closes and cleans up
//       after itself.
////////////////////////////////////////////////////////////////////////////////////    								

TwoLineProgressBar::TwoLineProgressBar(unsigned long ulTotalProgressCalls,
									unsigned long ulUpdatesPerCall,
									const char *pszLine1,
									const char *pszLine2,
									const char *pszBoxTitle,
									BOOL bCancelQuestion,
									RESOURCE_ID Icon)
									// The total number of times will call Progress_Bar_Update
									// Number of calls before bar updates itself - screen updates take much time
									// Status message which appears under "Please Wait" message
									// Name of application to appear in title bar
									// Whether to ask an "Are you sure question" when closing the box
									// The application icon to appear as the system menu (Windows 95)
	
{
	mpBox = NULL;
	muiIcon = Icon;
	mpszTitle = NULL;
	mpszLine1 = NULL;
	mpszLine2 = NULL;
	mulTotalCalls = ulTotalProgressCalls;
	mulPerCall = ulUpdatesPerCall;
	mulCallNumber = 0;
	mbCanceled = FALSE;
	mbOpen = FALSE;
	mbCancelQuestion = bCancelQuestion;

    if (pszLine1)
		if ((mpszLine1 = new char [strlen(pszLine1) + 1]) != NULL)
			strcpy(mpszLine1, pszLine1);

    if (pszLine2)
		if ((mpszLine2 = new char [strlen(pszLine2) + 1]) != NULL)
			strcpy(mpszLine2, pszLine2);

	if (pszBoxTitle)
		if ((mpszTitle = new char [strlen(pszBoxTitle) + 1]) != NULL)
			strcpy(mpszTitle, pszBoxTitle);
	
	// Make sure that ulPer_Call is not 0 - GPF when divide
	if (mulPerCall == 0) mulPerCall = 1;

	// Help prevent progress bar from being updated too much
	if ((mulTotalCalls / mulPerCall) > 100)
		mulPerCall = mulTotalCalls / 100;

}


TwoLineProgressBar::~TwoLineProgressBar(void) 
{

	Close();
	if (mpszLine1)
		delete [] mpszLine1;
	if (mpszLine2)
		delete [] mpszLine2;
	if (mpszTitle)
		delete [] mpszTitle;

}


// Repeatively call up to number of times setFirst time called it displays box 
// Returns FALSE if cancel was pressed
// NOTE: Only need to pass message once for change
BOOL TwoLineProgressBar::TwoLineProgressBarUpdate(const char *pszLine1, const char *pszLine2) 
{

	// MUST HAVE FIRST, Must give time to determine cancel
	CGUI_App::ProcessEvents();

	// If first time then create the box and display it
	if (mulCallNumber == 0) {
	    if (SetupBox()) {
	    	mpBox->GoModeless(TRUE);
			mbCanceled = FALSE;
	    	mbOpen = TRUE;
	    }
	    else	// error in SetupBox
	    	mbCanceled = TRUE;
	}
	
	// Update the progress box
    if (!mbCanceled) {
    	mulCallNumber++;
    	if (mulCallNumber <= mulTotalCalls) {
    		if (((mulCallNumber % mulPerCall) == 0) || (mulCallNumber == mulTotalCalls)) {
				mpBox->ProgressBarSetValue(IDC_2LINE_PROGRESS_BAR_BAR, ((double)mulCallNumber/(double)mulTotalCalls));
				if (pszLine1)	mpBox->FldStaticTxtSetValue(IDC_2LINE_PROGRESS_BAR_LINE1, pszLine1);
				if (pszLine2)	mpBox->FldStaticTxtSetValue(IDC_2LINE_PROGRESS_BAR_LINE2, pszLine2);
				CGUI_App::ProcessEvents(); // Keep so that bar gets displayed including when reach 100%
			}
    	}
	}
	return(!mbCanceled);
}


BOOL TwoLineProgressBar::TwoLineProgressBarSetValue(double dValue, const char *pszLine1, const char *pszLine2) 
{

	// MUST HAVE FIRST, Must give time to determine cancel
	CGUI_App::ProcessEvents();

	// If first time then create the box and display it
	if (mulCallNumber == 0) {
	    if (SetupBox()) {
	    	mpBox->GoModeless(TRUE);
			mbCanceled = FALSE;
	    	mbOpen = TRUE;
	    }
	    else	// error in SetupBox
	    	mbCanceled = TRUE;
	}
	
	// Update the progress box
    if (!mbCanceled) {
    	mulCallNumber++;
		mpBox->ProgressBarSetValue(IDC_2LINE_PROGRESS_BAR_BAR, dValue);
		if (pszLine1)	mpBox->FldStaticTxtSetValue(IDC_2LINE_PROGRESS_BAR_LINE1, pszLine1);
		if (pszLine2)	mpBox->FldStaticTxtSetValue(IDC_2LINE_PROGRESS_BAR_LINE2, pszLine2);
		CGUI_App::ProcessEvents(); // Keep so that bar gets displayed including when reach 100%
	}
	return(!mbCanceled);
}

BOOL TwoLineProgressBar::SetupBox(void) 
{
	// Create the box
	if ((mpBox = new CGUI_Dlg(IDD_TWO_LINE_PROGRESS_BAR, NULL, DLG_POSN_NO_SAVE)) != NULL) {
		if (muiIcon)
			mpBox->SetIcon(muiIcon);
		if (mpszTitle)
			mpBox->SetTitle(mpszTitle);
		if (mpszLine1)
			mpBox->DefineFldStaticTxt(IDC_2LINE_PROGRESS_BAR_LINE1, mpszLine1);
		if (mpszLine2)
			mpBox->DefineFldStaticTxt(IDC_2LINE_PROGRESS_BAR_LINE2, mpszLine2);
		mpBox->DefinePushBtn(IDC_2LINE_PROGRESS_BAR_CANCEL, GUI_NONE);
		mpBox->SetCtrlNotifyFn(IDC_2LINE_PROGRESS_BAR_CANCEL, (CGUI_CallBack *)((void *)this), (GUI_CB_DlgNotifyFn)&TwoLineProgressBar::TwoLineProgressBarCancel);
		mpBox->DefineProgressBar(IDC_2LINE_PROGRESS_BAR_BAR, TRUE);
    	mpBox->ProgressBarSetColors(IDC_2LINE_PROGRESS_BAR_BAR, RGB(0, 128, 0), RGB(128, 128, 128));
		// keep this progress bar always on top
		::SetWindowPos(mpBox->GetWndHandle(), HWND_TOP, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);

		return TRUE;
	}
	return FALSE;
}


BOOL TwoLineProgressBar::TwoLineProgressBarReset(unsigned long ulTotalProgressCalls, 
									unsigned long ulUpdatesPerCall,
									const char *pszLine1,
									const char *pszLine2,
									const char *pszBoxTitle,
									RESOURCE_ID Icon) 
									// The total number of times will call Progress_Bar_Update
									// Number of calls before bar updates itself - screen updates take much time
									// First line of status message which appears under "Please Wait" message
									// Second line of status message which appears under "Please Wait" message
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
{	
	BOOL bSuccess = TRUE;

	Close();

	// Don't set new unless passed
	if (ulTotalProgressCalls)								
		mulTotalCalls = ulTotalProgressCalls;
	
	if (ulUpdatesPerCall)								
		mulPerCall = ulUpdatesPerCall;
    
	if (pszLine1) {
		if ((mpszLine1 = new char [strlen(pszLine1) + 1]) != NULL)
			strcpy(mpszLine1, pszLine1);
		else
			bSuccess = FALSE;
	}	
    
	if (pszLine2) {
		if ((mpszLine2 = new char [strlen(pszLine2) + 1]) != NULL)
			strcpy(mpszLine2, pszLine2);
		else
			bSuccess = FALSE;
	}	
    
	if (pszBoxTitle) {
		if ((mpszTitle = new char [strlen(pszBoxTitle) + 1]) != NULL)
			strcpy(mpszTitle, pszBoxTitle);
		else
			bSuccess = FALSE;	
	}
	muiIcon = Icon;

	// Make sure that ulPer_Call is not 0 - GPF when divide
	if (mulPerCall == 0) mulPerCall = 1;
	// Help prevent progress bar from being updated too much
	if ((mulTotalCalls / mulPerCall) > 100)
		mulPerCall = mulTotalCalls / 100;

	// Status variables
	mulCallNumber = 0;
	mbCanceled = FALSE;
	mbOpen = FALSE;	
	
	return bSuccess;
}
									

BOOL TwoLineProgressBar::Close(void)
{
    bool bSuccess = FALSE;
    if (mbOpen) {
		if (mpBox != NULL)
		{
			bSuccess = mpBox->Close();
			if (bSuccess) {
				delete mpBox;
				mpBox = NULL;
			}
		}
		else
		{
			bSuccess = TRUE;
		}
		return bSuccess;
	}
	return (bSuccess);
}


void TwoLineProgressBar::TwoLineProgressBarCancel(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) 
{
	if (mbCancelQuestion) {
		if (MessageBox(NULL, "Are you sure you want to cancel?", mpszTitle, MB_YESNO | MB_TASKMODAL) == IDYES) {
			mbCanceled = TRUE;
		}
	}
	else {
		mbCanceled = TRUE;
	}
}



//To Use:
//TwoLineProgressBar *pTwoLineProgressBar;
//pTwoLine = new TwoLineProgressBar(ulTotalProgressCalls, ulUpdatesPerCall, szLine1, szLine2, szBoxTitle);
//bCancel = pTwoLine->TwoLineProgressBarUpdate(szLine1, szLine2);
//pTwoLine->Close();
// delete pTwoLine
