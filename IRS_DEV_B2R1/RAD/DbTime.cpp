///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbTime.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		FindDbDataRecContainingTime
//
//		All times are assumed to be unadjusted times
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "DbDay.h"
#include "FacilityConfig.h"
#include "DbInterval.h"
#include "MainDbCfg.h"
#include "TypeDefinitions.h"
#include <math.h>

extern short glsFacNum; //KM
extern CDbDay glDbDay;

/*===========================================================================
 *
 *  Name	 :  FindDbDataRecContainingTime
 *
 *  Purpose	 :  Find the db_data multiple rec containing the specified ulTime.
 *				If the record time does not exactly match the time requested,
 *				then find the prior record in the day for the station.  If there
 *				is no prior record in the day for the station, then the closest
 *				match is the next record in the day for the station.
 *				All times are assumed to be unadjusted times.
 *
 *  Return value :  None
 *
 *  Special notes:	if TRUE, curr_rec = multiple record containing the ulTime
 *
 *
 *  Implicit Input :
				    int sSta;			input - sSta to look for (-1 = any sSta)
    				unsigned long *ptr_day; output - julian ulTime representing first ulTime in day
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  30-May-95  SFK	 	Created from old GRAND Review find_db_end_day
 *	28-Jul-98  SFK		Major rewrite to incorporate concept of looking for SEARCH_START
 *						or SEARCH_END.  Separated into two functions to handle float
 *						and double data.
 *	30-Jul-98  SFK		Remove concept of SEARCH_START, SEARCH_END.
 *	07-Aug-2002	SFK		Changed to use DATE
 *
===========================================================================*/
int FindDbDataRecContainingTime(struct IDStruct StaID, DATE dTime, short sType, struct db_float_data_rec *pdbfData)
{

	struct db_day_rec dbDay;
	struct db_float_data_rec 	dbfData; 
	struct db_sta_rec dbDebug;
    
	DB_ADDR dba;
	
    int db_stat, status;
    DATE dbBegTime1, dbBegTime2;
    BOOL bSameDay = FALSE;
    short iDbSta;	
    DATE dSearchTime;
    
	status = StationExistsInMainDb(StaID.sStaID);
    if (status != TRUE) return(status);
                    		
   /* ------------------------------------------------------------------
    *	Check that the day and sSta exist in the db. If dTime falls
    *   after the last time in this day, then move to the next day.  Read
    *   out the	time associated with the first day record for the day.
    * ----------------------------------------------------------------*/
    dSearchTime = dTime;
    status = glDbDay.FindDayRecInMainDb(StaID.sStaID, dTime);
    if (status != TRUE) goto db_exit;     // no data for this day in the db  
    
	
    db_stat = d_recread(&dbDay, CURR_DB);
	// check if the time in question is after the last record in this day
    if (db_stat != S_OKAY) return (status); 
	if (dTime > dbDay.d_day_end_time) { // check that the next day is in the db
		dSearchTime = floor(dSearchTime);
		dSearchTime += 1.0;
        status = glDbDay.FindDayRecInMainDb(StaID.sStaID, dSearchTime);
        if (status != 1) 
			return (status);       
    }	
    
    if ( d_crread(D_DAY_BEG_TIME_KEY, (char *)&dbBegTime1, CURR_DB)!= S_OKAY) goto db_exit;
    
   /* ------------------------------------------------------------------
    *	There is some data for the day in the db.
	*	Find the key that is just beyond the ulTime we are looking for.
    * ----------------------------------------------------------------*/
    if (StaID.sDataType == FLOAT_TYPE) {
		// find key closest to search time
	    db_stat = d_keyfind(D_FDATA_BEG_TIME_KEY, &dSearchTime, CURR_DB);
	    if ((db_stat != S_OKAY) && (db_stat != S_NOTFOUND)) goto db_exit;      
		// go past this key
	    db_stat = d_keynext(D_FDATA_BEG_TIME_KEY, CURR_DB);
	    if ((db_stat != S_OKAY) && (db_stat != S_NOTFOUND)) goto db_exit;      
		//read the record as a diagnostic
	    db_stat  = d_recread((char*)&dbfData, CURR_DB);  // debug
	}    

	
   /* ------------------------------------------------------------------
    *	Look through the keys before this one until find one from the
    *	requested StaID.sStaID.
    * ----------------------------------------------------------------*/
    bSameDay = FALSE;
    do {                                          
    	/*	Find the data rec just before the current one and remember its address*/
		if (StaID.sDataType == FLOAT_TYPE)  {
		    if (d_keyprev(D_FDATA_BEG_TIME_KEY, CURR_DB) == S_NOTFOUND) break; // no records before
			db_stat  = d_recread((char*)&dbfData, CURR_DB); // debug
    		if (d_crget(&dba, CURR_DB) != S_OKAY) goto db_exit;  // save address of data record
    		if (d_findco(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto db_exit; // see who owns this record
		}	
			
	    	    	
	   /* ------------------------------------------------------------------
	    *	Find the day this data rec belongs to.  If it is not the same
	    *	as the day we're looking for, then have gone back too far.
	    * ----------------------------------------------------------------*/
    	db_stat = d_recread(&dbDay, CURR_DB);	// debug
	   	if (d_crread(D_DAY_BEG_TIME_KEY, &dbBegTime2, CURR_DB) != S_OKAY) goto db_exit;
		bSameDay = SameDay(dbBegTime1, dbBegTime2);	// 24-May-2005 use function for SameDay
		if (!bSameDay) break;
	    	
    	/*	Find the sSta this day rec belongs to */
    	if (d_findco(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) goto db_exit;      
    	db_stat = d_recread(&dbDebug,CURR_DB);	// debug
    	if (d_crread(S_STA_NUM_KEY, &iDbSta, CURR_DB) != S_OKAY) goto db_exit;           
	    	    	    	    	
    } while (iDbSta != StaID.sStaID);


	// is same day but haven't found the station yet so look ahead to see if there
	// is a record for this day for the desired station
    if ((bSameDay) && (iDbSta != StaID.sStaID)) {
		// can this ever be executed??
        do {
        	if (StaID.sDataType == FLOAT_TYPE){ 
		    	/*	Find the data rec just after the current one */
	    		if (d_keynext(D_FDATA_BEG_TIME_KEY, CURR_DB) != S_OKAY) break;      
				db_stat  = d_recread((char*)&dbfData, CURR_DB); // debug
	    		if (d_crget(&dba, CURR_DB) != S_OKAY) goto db_exit;  // save address of data record
	    		if (d_findco(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
	    		db_stat = d_recread(&dbDay, CURR_DB);	// debug
	    	}	
		    	
	
       	    /* 	If this is not the day we're looking for then have gone
    			ahead too far */      
    		if (d_crread(D_DAY_BEG_TIME_KEY, &dbBegTime2, CURR_DB) != S_OKAY) goto db_exit;
    		bSameDay = SameDay(dbBegTime1, dbBegTime2);	// 24-May-2005 use function for SameDay
    		if (!bSameDay) {	// back up one record
	        	if (StaID.sDataType == FLOAT_TYPE) 
			    	if (d_keyprev(D_FDATA_BEG_TIME_KEY, CURR_DB) != S_OKAY) break;      
    			break;
    		}	
	    	
    		/*	Find the sSta this day rec belongs to */
    		if (db_stat = d_findco(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) goto db_exit;      
		
    		db_stat = d_recread(&dbDebug,CURR_DB);	// debug
    		if (d_crread(S_STA_NUM_KEY, &iDbSta, CURR_DB) != S_OKAY) goto db_exit;           
	    	    	    	
    	} while ((bSameDay) && (iDbSta != StaID.sStaID));            
    }	
	    
	    
    //
    if (!bSameDay) {	// went back too far
        do {
        	if (StaID.sDataType == FLOAT_TYPE){ 
		    	/*	Find the data rec just after the current one */
	    		if (d_keynext(D_FDATA_BEG_TIME_KEY, CURR_DB) != S_OKAY) break;      
	    		if (d_crget(&dba, CURR_DB) != S_OKAY) goto db_exit; // save address of data record
	    		/*	Find the day this data rec belongs to */
	    		if (d_findco(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;      
	    		db_stat = d_recread(&dbDay, CURR_DB);	// debug
	    	}	
		    	
	
       	    /* 	If this is not the day we're looking for then have gone
    			ahead too far */      
    		if (d_crread(D_DAY_BEG_TIME_KEY, &dbBegTime2, CURR_DB) != S_OKAY) goto db_exit;
			bSameDay = SameDay(dbBegTime1, dbBegTime2);	// 24-May-2005 use function for SameDay
    		if (!bSameDay) break;
	    	
    		/*	Find the sSta this day rec belongs to */
    		if (db_stat = d_findco(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) goto db_exit;      
    		db_stat = d_recread(&dbDebug,CURR_DB);	// debug
    		if (d_crread(S_STA_NUM_KEY, &iDbSta, CURR_DB) != S_OKAY) goto db_exit;           
	    	    	    	
    	} while (iDbSta != StaID.sStaID);            
    }	
	    
    if (!bSameDay) {
	    return(uiTIME_NOT_IN_DB_STATUS);
	}                                
	
	
	/*  Found record, restore the address of the last data record found in the key search */
	if (d_crset(&dba, CURR_DB) != S_OKAY) goto db_exit;
	if (StaID.sDataType == FLOAT_TYPE) 	// return the record with the value
		db_stat  = d_recread((char*)pdbfData, CURR_DB);

    return(TRUE);    
    	
db_exit:
	RadReviewMsg(uiDB_BAD_ERR, "FindDbDataRecContainingTime");
    return(uiDB_BAD_ERR);
}

// same thing only for doubles data
int FindDbDataRecContainingTime(IDStruct StaID,DATE dTime, short sType, struct db_double_data_rec *pdbdData)
{

	struct db_day_rec dbDay;
	struct db_double_data_rec 	dbdData; 
	struct db_sta_rec dbDebug;
    
	DB_ADDR dba;
	
    int db_stat, status;
    DATE dbBegTime1, dbBegTime2;
    BOOL bSameDay;
    short iDbSta;	
    DATE dSearchTime;
    
	status = StationExistsInMainDb(StaID.sStaID);
    if (status != TRUE) return(status);
                    		
   /* ------------------------------------------------------------------
    *	Check that the day and sSta exist in the db. If dTime falls
    *   after the last time in this day, then move to the next day.  Read
    *   out the	time associated with the first day record for the day.
    * ----------------------------------------------------------------*/
    dSearchTime = dTime;
    status = glDbDay.FindDayRecInMainDb(StaID.sStaID, dTime);
    if (status != TRUE) return (status);     // no data for this day in the db  
    
	// check if the time in question is after the last record in this day
    db_stat = d_recread(&dbDay, CURR_DB);
    if (db_stat != S_OKAY) goto db_exit; 
	if (dTime > dbDay.d_day_end_time) { // check that the next day is in the db
		dSearchTime = floor(dTime);
		dSearchTime += 1.0;
        status = glDbDay.FindDayRecInMainDb(StaID.sStaID, dSearchTime);
        if (status != TRUE) return (status);       
    }	
    
    if ( d_crread(D_DAY_BEG_TIME_KEY, (char *)&dbBegTime1, CURR_DB)!= S_OKAY) goto db_exit;
    
   /* ------------------------------------------------------------------
    *	There is some data for the day in the db.
	*	Find the key that is just beyond the dTime we are looking for.
    * ----------------------------------------------------------------*/
	// find key closest to search time
	db_stat = d_keyfind(D_DDATA_BEG_TIME_KEY, &dSearchTime, CURR_DB);
	if ((db_stat != S_OKAY) && (db_stat != S_NOTFOUND)) goto db_exit;      
	db_stat = d_keynext(D_DDATA_BEG_TIME_KEY, CURR_DB);
	// go past this key
	if ((db_stat != S_OKAY) && (db_stat != S_NOTFOUND)) goto db_exit;      
	//read the record as a diagnostic
	db_stat  = d_recread((char*)&dbdData, CURR_DB);	//debug
	
   /* ------------------------------------------------------------------
    *	Look through the keys before this one until find one from the requested sSta.
    * ----------------------------------------------------------------*/
    bSameDay = FALSE;
    do {                                          
    	/*	Find the data rec just before the current one and remember its address*/
		if (d_keyprev(D_DDATA_BEG_TIME_KEY, CURR_DB) == S_NOTFOUND) break; // no records before
		db_stat  = d_recread((char*)&dbdData, CURR_DB); // debug                           
		if (d_crget(&dba, CURR_DB) != S_OKAY) goto db_exit;  // save address of data record
		if (d_findco(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit; // see who owns this record
	    	    	
	   /* ------------------------------------------------------------------
	    *	Find the day this data rec belongs to.  If it is not the same
	    *	as the day we're looking for, then have gone back too far.
	    * ----------------------------------------------------------------*/
    	db_stat = d_recread(&dbDay, CURR_DB);	// debug
	   	if (d_crread(D_DAY_BEG_TIME_KEY, &dbBegTime2, CURR_DB) != S_OKAY) goto db_exit;
		bSameDay = SameDay(dbBegTime1, dbBegTime2);	// 24-May-2005 use function for SameDay
    	if (!bSameDay) break;
	    	
    	/*	Find the sSta this day rec belongs to */
    	if (d_findco(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) goto db_exit;      
    	db_stat = d_recread(&dbDebug,CURR_DB);	// debug
    	if (d_crread(S_STA_NUM_KEY, &iDbSta, CURR_DB) != S_OKAY) goto db_exit;           
	    	    	    	    	
    } while (iDbSta != StaID.sStaID);


	// is same day but haven't found the station yet so look ahead to see if there
	// is a record for this day for the desired station
    if ((bSameDay) && (iDbSta != StaID.sStaID)) {
		// can this ever be executed??
        do {
		    /*	Find the data rec just after the current one */
	    	if (d_keynext(D_DDATA_BEG_TIME_KEY, CURR_DB) != S_OKAY) break;      
	    	db_stat  = d_recread((char*)&dbdData, CURR_DB); // debug                           
	    	if (d_crget(&dba, CURR_DB) != S_OKAY) goto db_exit;  // save address of data record
	    	if (d_findco(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;      
	    	db_stat = d_recread(&dbDay, CURR_DB);	// debug
	
       	    /* 	If this is not the day we're looking for then have gone
    			ahead too far */      
    		if (d_crread(D_DAY_BEG_TIME_KEY, &dbBegTime2, CURR_DB) != S_OKAY) goto db_exit;
			bSameDay = SameDay(dbBegTime1, dbBegTime2);	// 24-May-2005 use function for SameDay
    		if (!bSameDay) {	// back up one record
		    	if (d_keyprev(D_DDATA_BEG_TIME_KEY, CURR_DB) != S_OKAY) break;      
    			break;
    		}	
	    	
    		/*	Find the sSta this day rec belongs to */
    		if (db_stat = d_findco(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) goto db_exit;      
    		db_stat = d_recread(&dbDebug,CURR_DB);	// debug
    		if (d_crread(S_STA_NUM_KEY, &iDbSta, CURR_DB) != S_OKAY) goto db_exit;           
	    	    	    	
    	} while ((bSameDay) && (iDbSta != StaID.sStaID));            
    }	
	    
	    
    //
    if (!bSameDay) {	// went back too far
        do {
		    /*	Find the data rec just after the current one */
	    	if (d_keynext(D_DDATA_BEG_TIME_KEY, CURR_DB) != S_OKAY) break;      
	    	if (d_crget(&dba, CURR_DB) != S_OKAY) goto db_exit;  // save address of data record
	    	/*	Find the day this data rec belongs to */
	    	if (d_findco(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;      
	    	db_stat = d_recread(&dbDay, CURR_DB);	// debug
	
       	    /* 	If this is not the day we're looking for then have gone
    			ahead too far */      
    		if (d_crread(D_DAY_BEG_TIME_KEY, &dbBegTime2, CURR_DB) != S_OKAY) goto db_exit;
			bSameDay = SameDay(dbBegTime1, dbBegTime2);	// 24-May-2005 use function for SameDay
    		if (!bSameDay) break;
	    	
    		/*	Find the sSta this day rec belongs to */
    		if (db_stat = d_findco(STATION_TO_DAY_SET, CURR_DB) != S_OKAY) goto db_exit;      
    		db_stat = d_recread(&dbDebug,CURR_DB);	// debug
    		if (d_crread(S_STA_NUM_KEY, &iDbSta, CURR_DB) != S_OKAY) goto db_exit;           
	    	    	    	
    	} while (iDbSta != StaID.sStaID);            
    }	
	    
    if (!bSameDay) {
	    return(uiTIME_NOT_IN_DB_STATUS);
	}                                
	
	
	/*  Found record, restore the address of the last data record found in the key search */
	if (d_crset(&dba, CURR_DB) != S_OKAY) goto db_exit;
	if (StaID.sDataType == DOUBLE_TYPE)	// read out record to return 
		db_stat = d_recread((char*)pdbdData, CURR_DB);

    return(TRUE);    
    	
db_exit:
	RadReviewMsg(uiDB_BAD_ERR, "FindDbDataRecContainingTime");
    return(uiDB_BAD_ERR);
}

                                                                                                                                    