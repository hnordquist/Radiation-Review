// GUI_Glue.cpp
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support generic routines to provide
//	interfaces between specific GUI constructs
//
//	Functions:
//		CStringToAszStrs - converts CStrings to gen_allocate_str_array structures
///////////////////////////////////////////////////////////////////////////


#include <gen.h>
#include "RAD.H" 
#include "GUI_Glue.h"





///////////////////////////////////////////////////////////////////////////
//	Name:	SetupPicklist
//
//	Description:
//	Converts a group of CStrings into the structure needed for GUI picklists
//
//	Declaration:
//	
//
//	Input:	list of strings
//			
//	Output:	none
//
//	Return:	
//	
//  date    /	author	revision
//  -----------------	--------
//	09-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
int AllocStrArray(int iNumStrs, int iMaxLen, char **pAdrs[])
{
    size_t reserved_memory = 0;

	if (iNumStrs == 0) 
		return(TRUE);

	// allocate the storage
	short sNum = gen_allocate_str_array((iMaxLen+1), (short)iNumStrs,	reserved_memory, pAdrs);
    if (sNum != iNumStrs) 		// check that everything got allocated okay
	{
    	if (*pAdrs) 
			gen_deallocate_str_array (*pAdrs);

		*pAdrs = NULL;
    	RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
    	return(FALSE);
    }
	return(TRUE);
}

void DeAllocStrArray(char **pAdrs)
{
	if (pAdrs) 
		gen_deallocate_str_array(pAdrs);

	//this pointer is passed by value.
	//caller must zero it's own pointer.
	//pAdrs = NULL;
}
