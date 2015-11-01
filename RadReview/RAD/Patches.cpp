///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : Patches.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////
#include "RAD.H"                        

// this file includes patches to handle compiling under C++ 6.0

HWND CGUI_Dlg::GetWndHandle()
{
	return(hWnd);
}



void CGUI_DlgCtrl::SetChildFocusWndHandle(HWND hChildFocusWnd)
{
	mhChildFocusWnd = hChildFocusWnd;
	return;
}


void CGUI_Dlg::SetChildFocusWndHandle(RESOURCE_ID CtrlID, HWND hChildFocusWnd)
{
	CGUI_DlgCtrl *pCtrl;
	           
	if (GetDlgCtrl(CtrlID, &pCtrl)) {
		pCtrl->SetChildFocusWndHandle(hChildFocusWnd);
	}
	return;
}

