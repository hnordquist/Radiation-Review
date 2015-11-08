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
