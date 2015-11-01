///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : RadReanalyze.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////
// Member functions for Reanalyze portion of Radiation Review

#include "RADReanalyze.h"   
#include "RAD.H"
#include "upperlayer/RTTitle.H"	// 8-Dec-04 SFK Added relative path

extern RTTitle *pRT_Title;



/////////////////////////////////////////////////////////////////////////////////////
//  RADReanalyzeconstructor, destructor
/////////////////////////////////////////////////////////////////////////////////////

RADReanalyze::RADReanalyze(void) 
	{
	}
	

RADReanalyze::~RADReanalyze(void) {
	
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
// Required through RTReanalyze to provide a Begin procedure which is specific to radiation review
////////////////////////////////////////////////////////////////////////////////////////////////////////
RTReanalyze::REANALYZE_OPERATION  RADReanalyze::RT_Reanalyze_Begin() {

	CGFmtStr Msg;
	pRT_Title->RT_Title_Splash_Close();		// shut down splash screen if it is up
	Msg.Printf("Reanalyze is not necessary for %s.", pCommon_Params->Get_My_Name());
	GUI_MsgBox(Msg, GUI_ICON_INFO);
	return (RTReanalyze::REANALYZE_EXIT);
}



// required by RT Review
BOOL RADReanalyze::RT_Reanalyze_Exit() { 
	return TRUE;
}


/////////////////////////////////////////////////////////////////////
// Access to the common data items across all review tool operations
/////////////////////////////////////////////////////////////////////
BOOL RADReanalyze::Set_Common_Params(RTInit *const pRT_Common_Params) {
    
    RADReanalyze::pCommon_Params = (RADInit *)pRT_Common_Params;
    return RTReanalyze::Set_Common_Params(pCommon_Params);
    
}



		

