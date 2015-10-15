///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbDay.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		FindDayRecInMainDb
//		CreateDayRecInMainDb
//		ReadDayRecInMainDb
//		DeleteDbDayInMainDb
//
//	All times are assumed to be unadjusted
//
///////////////////////////////////////////////////////////////////////////

#include <gen.h>
#include "RAD.H"
#include "DbDay.h"
#include "MainDbCfg.h"
#include "FacilityConfig.h"
#include "TypeDefinitions.h"

extern short glsFacNum;

///////////////////////////////////////////////////////////////////////////
//	Name:	FindDayRecInMainDb
//
//	Description:
//	Determine if a dbday_rec containing the given time exists in the Rad dbVista
//	main database for the given station.
//	Uses unadjusted times.
//
//	Declaration:
//	int FindDayRec(short sSta, DATE dTime)
//
//	Input:	sSta	number of station want to find day for
//			dTime	day (DATE format) searching for
//			
//	Output: none
//
//	Return: TRUE - day exists
//		    uiDAY_NOT_IN_DB_STATUS - day does not exist
//		    STA_NOT_IN_DB - sSta does not exist
//			DB_BAD - unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  23-May-1995 SFK	Created
//	21-Jul-1995 SFK	When find day looking for make sure the day is owned by the
//					expected sSta.
//	02-Aug-2002	SFK	Changed to use DATE timebase (doubles in event rec)
//	23-Sep-2003 SFK	Changed name from FindDayRec to FindDayRecInMainDb
//////////////////////////////////////////////////////////////////
int CDbDay::FindDayRecInMainDb(short sSta, DATE dTime)
{
    struct db_day_rec dbDay;         
	DATE ddbDay;
    int db_stat, status;             
    short db_sSta;
        
   /* ------------------------------------------------------------------
    *	If the sSta is not in the database, can't do anything
    * ----------------------------------------------------------------*/
	status = StationExistsInMainDb(sSta);
   if (status != 1)
		return(status);
   
   /* ------------------------------------------------------------------
    *	Found the sSta, make it the owner of the set to search
    * ----------------------------------------------------------------*/
    if (d_setor(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) 
		goto db_exit;
                                       
   /* ------------------------------------------------------------------
    *	Search through the day records attached to this sSta.  Read
    *   out the time associated with the day and convert it to the
    *   jul secs corresponding to day start.  If this matches the
    *	day looking for then have found it.
    * ----------------------------------------------------------------*/
	db_stat = d_findfm(STATION_TO_DAY_SET, CURR_DB);
    if (db_stat == S_EOS) 
		return(uiDAY_NOT_IN_DB_STATUS);
    if (db_stat != S_OKAY) 
		goto db_exit;
	if (d_crread(D_DAY_BEG_TIME_KEY, &ddbDay, CURR_DB) != S_OKAY) 
		goto db_exit;                      
    //db_stat = d_recread(&dbDay, CURR_DB);	// debug

	if ((int)ddbDay == (int)dTime) 
		return 1;

    do 
	{
		db_stat = d_findnm(STATION_TO_DAY_SET, CURR_DB);    
		if (db_stat == S_EOS) 
			return(uiDAY_NOT_IN_DB_STATUS); 
   		if (db_stat != S_OKAY) 
				goto db_exit;	    
		if (d_crread(D_DAY_BEG_TIME_KEY, &ddbDay, CURR_DB) != S_OKAY) 
			goto db_exit;
		//db_stat = d_recread(&dbDay, CURR_DB);	// debug

		if ((int)ddbDay == (int)dTime) 
		{
    		if (d_csoread(STATION_TO_DAY_SET, S_STA_NUM_KEY, &db_sSta, CURR_DB) != S_OKAY) 
				goto db_exit;

			db_stat = d_recread(&dbDay, CURR_DB);	// debug
    		if (db_sSta == sSta )
				return 1;
		}	
    } while (db_stat == S_OKAY);	

db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "FindDayRecInMainDb", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}       



///////////////////////////////////////////////////////////////////////////
//	Name:	CreateDayRecInMainDb
//
//	Description:
//	Create a new day record for the given station and insert in the Rad dbVista main db.
//  If the day already exists in the db, do nothing.
//	Uses unadjusted times.
//
//	Declaration:
//	int CreateDayRecInMainDb(short sSta, DATE dDay)
//
//	Input:	sSta	number of station want to find day for
//			dDay	day (DATE format) searching for
//			
//	Output: none
//
//	Return: TRUE - day exists
//		    uiDAY_NOT_IN_DB	- day does not exist
//		    STA_NOT_IN_DB - sSta does not exist
//			DB_BAD - unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  23-May-1995 SFK	Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
//////////////////////////////////////////////////////////////////
int CDbDay::CreateDayRecInMainDb(short sSta, DATE dDay)
{
    struct db_day_rec dbDay;
    int status, db_stat;
    
   /* ------------------------------------------------------------------
    *	If the day is already in the db do nothing.
    * ----------------------------------------------------------------*/
	status = FindDayRecInMainDb(sSta, dDay);
    if (status != uiDAY_NOT_IN_DB_STATUS) 
		return(status);
       
   /* ------------------------------------------------------------------
    *	Find the sSta that will own this day and make it the owner of
    *	the set.
    * ----------------------------------------------------------------*/
	db_stat = d_keyfind(S_STA_NUM_KEY, (char *)&sSta, CURR_DB);
    if (db_stat != S_OKAY) goto db_exit;
	db_stat = d_setor(STATION_TO_DAY_SET, CURR_DB);
    if (db_stat != S_OKAY) goto db_exit;
       
   /* ------------------------------------------------------------------
    *	Fill the new record you want to write and connect into the database
    * ----------------------------------------------------------------*/
    dbDay.d_day_beg_time_key = dDay;
	dbDay.d_day_beg_time = dDay;
    dbDay.d_day_end_time = 0.0;
    dbDay.ul_day_total_pts = 0;
	db_stat = d_fillnew(DB_DAY_REC,(char *)&dbDay, CURR_DB);
    if (db_stat != S_OKAY) goto db_exit;
	db_stat = d_connect(STATION_TO_DAY_SET, CURR_DB);
    if (db_stat != S_OKAY) goto db_exit;
    return(TRUE);

db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "CreateDayRecInMainDb", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	ReadDayRecInMainDb
//
//	Description:
//	For a given station and day, read the associated database record from the 
//	Rad dbVista main db.
//	Uses unadjusted times.
//
//	Declaration:
//	int ReadDayRecInMainDb(short sSta, DATE dDay, struct db_day_rec *dbDay)       
//
//	Input:	sSta	number of station want to find day for
//			dDay	day (DATE format) searching for
//			
//	Output: dbDay	record read from database
//
//	Return: TRUE - day exists
//		    uiDAY_NOT_IN_DB	- day does not exist
//			DB_BAD - unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  26-Jul-1995 SFK		Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
//////////////////////////////////////////////////////////////////
int CDbDay::ReadDayRecInMainDb(short sSta, DATE dDay, struct db_day_rec *dbDay)       
{
    int db_stat;
        
   /* ------------------------------------------------------------------
    *	Find the day record associated with the number. If the day
    *	does not exist, cannot do anything.
    * ----------------------------------------------------------------*/
	db_stat = FindDayRecInMainDb(sSta, dDay);
    if (!db_stat) 
		return(db_stat);
       
   /* ------------------------------------------------------------------
    *	Read the record.
    * ----------------------------------------------------------------*/
	db_stat = d_recread(dbDay, CURR_DB);
    if (db_stat == S_OKAY) return(TRUE);
    
    RadReviewMsg(uiDB_BAD_ERR, "ReadDayRecInMainDb", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}                                         



///////////////////////////////////////////////////////////////////////////
//	Name:	DeleteDbDayInMainDb
//
//	Description:
//	Delete all records associated with specified day and station from the Rad
//	main dbVista database.
//	Uses unadjusted times.
//
//	Declaration:
//	int DeleteDbDayInMainDb(short sSta, DATE dDay)
//
//	Input:	sSta	number of station want to find day for
//			dDay	day (DATE format) want to delete
//			
//	Output: none
//
//	Return: TRUE - day and associated records deleted
//			FAIL	- day not found in db
//			DB_BAD	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  05-Feb-1990 SFK	Created
//	02-Aug-2002	SFK		Changed to use DATE timebase (doubles in event rec)
//////////////////////////////////////////////////////////////////
int CDbDay::DeleteDbDayInMainDb(struct IDStruct StaID, DATE dDay)
{
    DB_ADDR dba;	/* db address of day trying to delete */
    int status;
    int db_stat;
    struct db_sta_rec dbSta;
//	struct IDStruct StaID;

       
   /* ------------------------------------------------------------------
    *	Read the station record to decide how to delete.
    *	Check if the day is in the database.
    * ----------------------------------------------------------------*/
	//StaID.sStaID = sSta;
	status = pglFacCfg->GetStationRecord(glsFacNum, &StaID, &dbSta);
    if (!status) return(status);

	status = FindDayRecInMainDb(StaID.sStaID, dDay);
    if (status != 1) 
		return(status);
    
   /* ------------------------------------------------------------------
    *	Save db address of day, make it the owner of all data recs
    *	and then find, delete them
    * ----------------------------------------------------------------*/
    if (d_crget(&dba, CURR_DB) != S_OKAY) goto db_exit;
    
    if (dbSta.s_data_type == FLOAT_TYPE) {
	    if (d_setor(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
	    db_stat = d_findfm(DAY_TO_FLOAT_DATA_SET, CURR_DB);
	    if (db_stat == S_OKAY) {
	        do {
				if (d_disdel(CURR_DB) != S_OKAY) goto db_exit;
	    	} while (d_findnm(DAY_TO_FLOAT_DATA_SET, CURR_DB) == S_OKAY);
	    }
	    else {
	    	if (db_stat != S_EOS) goto db_exit;
	    }		
	}                                
	
    if (dbSta.s_data_type == DOUBLE_TYPE) {
	    if (d_setor(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
	    db_stat = d_findfm(DAY_TO_DOUBLE_DATA_SET, CURR_DB);
	    if (db_stat == S_OKAY) {
	        do {
				if (d_disdel(CURR_DB) != S_OKAY) goto db_exit;
	    	} while (d_findnm(DAY_TO_DOUBLE_DATA_SET, CURR_DB) == S_OKAY);
	    }
	    else {
	    	if (db_stat != S_EOS) goto db_exit;
	    }		
	}    

   /* ------------------------------------------------------------------
    *	Restore db address of day, make it the owner of all event recs
    *	and then find, delete them
    * ----------------------------------------------------------------*/
    if (d_crset(&dba, CURR_DB) != S_OKAY) goto db_exit;
    if (d_setor(DAY_TO_EVENT_SET, CURR_DB) != S_OKAY) goto db_exit;
    db_stat = d_findfm(DAY_TO_EVENT_SET, CURR_DB);
	if (db_stat == S_OKAY) {
        do {
			if (d_disdel(CURR_DB) != S_OKAY) goto db_exit;
    	} while (d_findnm(DAY_TO_EVENT_SET, CURR_DB) == S_OKAY);
    }
    else {
    	if (db_stat != S_EOS) goto db_exit;
    }		

   /* ------------------------------------------------------------------
    *	Restore db address of day, make it the owner of all rad event recs
    *	and then find, delete them
    * ----------------------------------------------------------------*/
    if (d_crset(&dba, CURR_DB) != S_OKAY) goto db_exit;
    if (d_setor(DAY_TO_DIR_EVENT_SET, CURR_DB) != S_OKAY) goto db_exit;
    db_stat = d_findfm(DAY_TO_DIR_EVENT_SET, CURR_DB);
	if (db_stat == S_OKAY) {
        do {
			if (d_disdel(CURR_DB) != S_OKAY) goto db_exit;
    	} while (d_findnm(DAY_TO_DIR_EVENT_SET, CURR_DB) == S_OKAY);
    }
    else {
    	if (db_stat != S_EOS) goto db_exit;
    }		
   /* ------------------------------------------------------------------
    *	Restore db address of day, make it the owner of all mark recs
    *	and then find, delete them
    * ----------------------------------------------------------------*/
    if (d_crset(&dba, CURR_DB) != S_OKAY) goto db_exit;
    if (d_setor(DAY_TO_MARK_SET, CURR_DB) != S_OKAY) goto db_exit;
    db_stat = d_findfm(DAY_TO_MARK_SET, CURR_DB);
	if (db_stat == S_OKAY) {
        do {
			if (db_stat = d_disdel(CURR_DB) != S_OKAY) goto db_exit;
    	} while (d_findnm(DAY_TO_MARK_SET, CURR_DB) == S_OKAY);
    }
    else {
    	if (db_stat != S_EOS) goto db_exit;
    }		
    
   /* ---------------------------------------------------------------
    *	Have deleted all data associated with this day, restore
    *	address of day record and now delete the day record.
    * --------------------------------------------------------------*/
    if (d_crset(&dba, CURR_DB) != S_OKAY) goto db_exit;
    if (d_disdel(, CURR_DB) != S_OKAY) goto db_exit;

    return(TRUE);

db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "DeleteDbDayInMainDb");	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}





                                                    