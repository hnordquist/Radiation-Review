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
      