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
///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbStation.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "FacilityConfig.h"
#include "PerformTimerForGuiLib.h"

extern short glsFacNum;
extern BOOL glbFacCfgStaChange;
struct db_sta_rec*  pStaRecs = NULL;

short gllsStaTypes = 0;



/*===========================================================================
 *
 *  Name	 :  StationExistsInMainDb
 *
 *  Purpose	 :  Determine if a db_sta_rec containing the given station number
 *			    exists in the database
 *
 *  Return value :  TRUE - station exists
 *					uiSTA_NOT_IN_DB - station does not exist
 *					other: unexpected DB error
 *
 *  Special notes:  if TRUE, curr_rec = station looking for
 *
 *  Implicit Input :
     				int num;	input - unique station number 
    				
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  23-May-1995 SFK		Created
 *	27-Nov-2001	SFK		Changed name from FindStaRec to StationDataExists
 *	23-Sep-2003 SFK		Changed name from StationDataExists to StationExistsinMainDb
 *
===========================================================================*/
int StationExistsInMainDb(short sSta)
{
	int db_stat;

	db_stat = d_keyfind(S_STA_NUM_KEY,(char *)&sSta, CURR_DB);

    if (db_stat == S_OKAY) 
		return 1;

    if (db_stat == S_NOTFOUND) 
		return (uiSTA_NOT_IN_DB_ERR);

	RadReviewMsg(uiDB_BAD_ERR, "StationExistsInMainDb", db_stat);
    return(uiDB_BAD_ERR);
}



/*===========================================================================
 *
 *  Name	 :  FacilityExistsInMainDb
 *
 *  Purpose	 :  Determine if a db_fac_rec containing the given facility number
 *			    exists in the database
 *
 *  Return value :  TRUE - 		facility exists
 *				    FAC_NOT_IN_DB - facility does not exist
 *					other: 			unexpected DB error
 *
 *  Special notes:  if TRUE, 	curr_rec = facility looking for
 *
 *  Implicit Input :short sFac;	input - unique facility number looking for
    				
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  23-May-1995 SFK	Created
 *	29-Sep-2003	SFK		Changed name from FindFacRec to FacilityExistsInMainDb
 *
===========================================================================*/
int FacilityExistsInMainDb(short sFac)
{
    int db_stat;

    db_stat = d_keyfind(S_FAC_NUM_KEY, (char *)&sFac, CURR_DB);
    if (db_stat == S_OKAY) return (TRUE);
    if (db_stat == S_NOTFOUND) return(uiFAC_NOT_IN_DB_ERR);
    RadReviewMsg(uiDB_BAD_ERR, "FindFacRec", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}
      