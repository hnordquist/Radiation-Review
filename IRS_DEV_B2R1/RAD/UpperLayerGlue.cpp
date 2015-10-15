///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : UpperLayerGlue.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions which provide the glue between
//	the upper layer functions without having to include their header files with 
//	all the associated GUI stuff.
//
///////////////////////////////////////////////////////////////////////////
#include <gen.h>
#include "RadInit.H"
#include "RadImport.H"
#include "UpperLayerGlue.h"


extern RADInit *pRAD_Init;
extern RADImport *pRAD_Import;  

////////////////////////////////////////////////////
const struct GEN_DATE_STRUCT* GetStartDateFromRADInit() 
{
  	return(pRAD_Init->Get_Start_Date());
}


////////////////////////////////////////////////////
const struct GEN_DATE_STRUCT* GetEndDateFromRADInit() 
{
  	return(pRAD_Init->Get_End_Date());
}


////////////////////////////////////////////////////
const struct GEN_TIME_STRUCT* GetStartTimeFromRADInit() 
{
  	return(pRAD_Init->Get_Start_Time());
}


////////////////////////////////////////////////////
const struct GEN_TIME_STRUCT* GetEndTimeFromRADInit() 
{
  	return(pRAD_Init->Get_End_Time());
}


////////////////////////////////////////////////////
BOOL GetInImportModeFromRADImport()
{
	return(pRAD_Import->bInImportMode);
}

