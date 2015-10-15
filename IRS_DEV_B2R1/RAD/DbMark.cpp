///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbMark.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		DeleteMarkRec
//		FindMarkRec
//		CreateMarkRec
//		GetDataStatisticsForMarks - Replaces NumMarksInDbInterval
//		GetDataByStationMarks - Replaces ReadMarksInDbInterval
//		DeleteDbMarks
//		ScanMarkRecs
//
//		All times are assumed to be adjusted times
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "DbDay.h"
#include "DbInterval.h"
#include "DbMark.h"
#include "FacilityConfig.h"
#include "MainDbCfg.h"

extern CDbDay glDbDay;

//===========================================================================
//
//  Name	 :  DeleteMarkRec
//
//  Purpose	 :  Determine if the db_mark_rec exists in the database and delete
//				it if is does.
//
//  Return value :  TRUE - mark already exists
//				    uiMARK_NOT_IN_DB - mark does not exist
//				    CHAN_NOT_IN_DB 	- channel does not exist
//				    DAY_NOT_IN_DB 	- day does not exist
//					DB_BAD	- unexpected db error
//
//  Special notes:  if TRUE, curr_rec = mark looking for
//					if uiMARK_NOT_IN_DB, curr_rec = day where mark should attach to
//
//  Implicit Input : 
//  Implicit Input/Output:
//  Implicit Output:
//
//  date    /	author	revision
//  -----------------	--------
//  01-Mar-1997 SFK		Created
//
//=========================================================================== 
int DeleteMarkRec(struct db_mark_rec *pdbMark)
{
	int status, db_stat;
	
	status = FindMarkRec(pdbMark);
	if (status == TRUE) {
		db_stat = d_disdel(CURR_DB);
		if (db_stat == S_OKAY) return(TRUE);
	}
    RadReviewMsg(uiDB_BAD_ERR, "DeleteMarkRec", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}



//===========================================================================
//
//  Name	 :  FindMarkRec
//
//  Purpose	 :  Determine if the db_mark_rec exists in the database
//
//  Return value :  TRUE - mark already exists
//				    uiMARK_NOT_IN_DB - mark does not exist
//				    CHAN_NOT_IN_DB 	- channel does not exist
//				    DAY_NOT_IN_DB 	- day does not exist
//					DB_BAD	- unexpected db error
//
//  Special notes:  if TRUE, curr_rec = mark looking for
//					if uiMARK_NOT_IN_DB, curr_rec = day where mark should attach to
//
//  Implicit Input : 
//  Implicit Input/Output:
//  Implicit Output:
//
//  date    /	author	revision
//  -----------------	--------
//  20-Jun-1996 SFK		Created
//	07-Aug-2002	SFK		Changed to use DATE 
//	02-Oct-2003	SFK		Modified for new FacMgr
//
//========================================================================= 
int FindMarkRec(struct db_mark_rec *pdbMark)
{
    
    struct db_mark_rec dbMark;
    int db_stat, status;
    DB_ADDR dba;
    
   // ------------------------------------------------------------------
   //	Get the station associated with the channel, if both exist.
   // ---------------------------------------------------------------- 
	status = StationExistsInMainDb(pdbMark->s_mark_sta);
    if (status != TRUE) return(status);
       
   // ------------------------------------------------------------------
   //	Now find a day record for the record.  If the day does
   //	not exist, cannot write the record.
   // ---------------------------------------------------------------- 
    status = glDbDay.FindDayRecInMainDb(pdbMark->s_mark_sta, pdbMark->d_mark_time_key);
    if (status != TRUE) return (status);
    
   // ------------------------------------------------------------------
   //	Search through all marks for this day to see if the mark
   //	already exists.
   // ---------------------------------------------------------------- 
    db_stat = d_setor(DAY_TO_MARK_SET, CURR_DB);
    if (db_stat != S_OKAY) goto db_exit;
    db_stat = d_crget(&dba, CURR_DB);
    if (db_stat != S_OKAY) goto db_exit;
    db_stat = d_findfm(DAY_TO_MARK_SET, CURR_DB);
    if (db_stat == S_EOS) goto not_in_db;
    if (db_stat != S_OKAY) goto db_exit;
   	do {
		d_recread(&dbMark, CURR_DB);
		if (db_stat != S_OKAY) goto db_exit;
		if ((pdbMark->s_mark_chan == dbMark.s_mark_chan) &&
			(pdbMark->d_mark_time_key == dbMark.d_mark_time_key) &&
			(pdbMark->s_mark_sta == dbMark.s_mark_sta) &&
			(pdbMark->s_mark_sta_type == dbMark.s_mark_sta_type)) return(TRUE);
		db_stat = d_findnm(DAY_TO_MARK_SET, CURR_DB);
	 } while (db_stat == S_OKAY);
	if (db_stat == S_EOS) goto not_in_db;
	if (db_stat != S_OKAY) goto db_exit;
    
not_in_db:
    db_stat = d_crset(&dba, CURR_DB);
    if (db_stat != S_OKAY) goto db_exit;
    return(uiMARK_NOT_IN_DB_WARN);

db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "FindMarkRec", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}       


// <f>  
//===========================================================================
//
//  Name	 :  CreateMarkRec
//
//  Purpose	 :  Create a new record and insert in the db.  If the mark
//		    	already exists in the db, do nothing.
//
//  Return value :	TRUE - record created
//				    CHAN_NOT_IN_DB 	- channel does not exist
//				    DAY_NOT_IN_DB 	- day does not exist
//					DB_BAD	- unexpected db error
//
//  Special notes:	if TRUE, curr_rec = mark just created
//
//  Implicit Input :
//  Implicit Input/Output:
//  Implicit Output:
//
//  date    /	author	revision
//  -----------------	--------
//  20-Jun-1996 SFK		Created
//
//=========================================================================== 
int CreateMarkRec(struct db_mark_rec *pdbMark)
{
    int status, db_stat;
	struct db_mark_rec dbMark;
           
   // ------------------------------------------------------------------
   //	Check if the mark is already in the db
   // ---------------------------------------------------------------- 
    status = FindMarkRec(pdbMark);
    if (status != uiMARK_NOT_IN_DB_WARN) return(status);
       
   // ------------------------------------------------------------------
   //	The record is not in the db, add it now
   // ---------------------------------------------------------------- 
	db_stat = d_fillnew(DB_MARK_REC, (char *)pdbMark, CURR_DB);
    if (db_stat != S_OKAY) goto db_exit;
    db_stat = d_connect(DAY_TO_MARK_SET, CURR_DB);
   	if (db_stat != S_OKAY) goto db_exit;  
	
	db_stat = d_recread(&dbMark, CURR_DB);	// read it back for debug
	status = FindMarkRec(pdbMark);		// read it back for debug
	return(TRUE);
	
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "CreateMarkRec", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStatisticsForMarks
//			Replaces NumMarksInDbInterval
//
//	Description:
//	Find the start time in the db.  Count the number of marks
//	in the database for the specified channel during 
//	the interval. 
//	All times are assumed to be adjusted.
//
//	Declaration:
//	int GetDataStatisticsForMarks(short sChan, DATE dStartTime, DATE dEndTime, unsigned long *pulNumMarks)
//
//	Input:	sChan		number of channel want mark count for
//			dStartTime	start of search interval
//			dEndTime	end of search interval
//			
//	Output: pulNumMarksRead	number of marks actually read
//
//	Return: TRUE - some data in DB
//			FALSE - no data or error
//			DB_BAD	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  ??			SFK		Created
//	02-Aug-2002	SFK		Changed to use DATE timebase 
//  -2+Oct-2003	SFK		Modified for new FacMgr
//////////////////////////////////////////////////////////////////
// new with DATE
int GetDataStatisticsForMarks(IDStruct ChanID, DATE dStartTime, DATE dEndTime, unsigned long *pulNumMarks)

{
    struct db_mark_rec dbMark;

    int  db_stat, status;           
    DATE dCurrentDay;		    // day you want to find in database in jul secs  
    DATE dActualEndTime;
    DATE dActualStartTime;
    unsigned long ulDum;
    BOOL bDone = FALSE;
	BOOL bLastDay = FALSE;   
    BOOL bThisDayDone;
    CGFmtStr msg;

    *pulNumMarks = 0;
    if (dEndTime < dStartTime) return(TRUE);

   // ------------------------------------------------------------------
   //	Get the station associated with the channel, if both exist.
   // ---------------------------------------------------------------- 
	status = StationExistsInMainDb(ChanID.sStaID);
    if (status != TRUE) return(status);
                 
	status = GetDataStatistics(ChanID, true, dStartTime, dEndTime, &dActualStartTime, &dActualEndTime, &ulDum);	
	if (status != TRUE) return(status);
    dCurrentDay = dActualStartTime; 
	status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dCurrentDay);	// position curr rec at first day's record
    if (status != TRUE) return(status);

    
   // ------------------------------------------------------------------
   //	Look through the marks for all days and find those that occur
   //	at or after the start time and at or before the end time.
   // ---------------------------------------------------------------- 
    do {
	    db_stat = d_setor(DAY_TO_MARK_SET, CURR_DB); 	// select set  
	    if (db_stat != S_OKAY) goto db_exit;
	    
    	bThisDayDone = FALSE;
	    db_stat = d_findfm(DAY_TO_MARK_SET, CURR_DB); 	// see if any records  
	    if (db_stat == S_EOS) {
	    	bThisDayDone = TRUE;
	    }
	    else {	
	    	if (db_stat != S_OKAY) goto db_exit;
	    }	
	                                             
		if (bThisDayDone == FALSE) {	                                             
		   	do {                                            // count the marks  
				db_stat = d_recread(&dbMark, CURR_DB);
				if (db_stat != S_OKAY) goto db_exit;
				if ((dbMark.d_mark_time_key >= dActualStartTime) &&
				   	(dbMark.d_mark_time_key <= dActualEndTime) && 
					(dbMark.s_mark_chan == ChanID.sChanID) &&
					(dbMark.s_mark_sta == ChanID.sStaID) &&
					 (dbMark.s_mark_sta_type = ChanID.sStaType)) (*pulNumMarks)++;
				db_stat = d_findnm(DAY_TO_MARK_SET, CURR_DB);
			} while (db_stat == S_OKAY);
			if (db_stat != S_EOS) goto db_exit;
		}	
		if ((int)dCurrentDay == (int)dActualEndTime) bDone = TRUE;
		if (!bDone) {	// no, find the next day
			do {
				dCurrentDay += 1.0;				// more days to search  
				if ((int)dCurrentDay == (int)dActualEndTime) bLastDay = TRUE;
				status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dCurrentDay);
	    	} while ((status != TRUE) && (bLastDay == FALSE)); 	
	    }	
    } while (!bDone);
    
	return(TRUE);
	
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "GetDataStatisticsForMarks", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}       

///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByStationMarks
//			Replaces ReadMarksInDbInterval
//
//	Description:
//	Find the start time in the db.  Read the number of marks
//	in the database for the specified channel during 
//	the interval. 
//	All times are assumed to be adjusted.
//
//	Declaration:
//	int GetDataByStationMarks(short sChan, DATE dStartTime, DATE dEndTime, unsigned long ulNumMarks, unsigned long *pulNumMarksRead, struct db_mark_rec *pMarks)
//
//	Input:	sChan		number of channel want mark count for
//			dStartTime	start of search interval
//			dEndTime	end of search interval
//			ulNumMarks	number of marks storage has been allocated for
//			
//	Output: pulNumMarksRead	number of marks actually read
//			pMarks			array where the marks read are stored
//
//	Return: TRUE - some data in DB
//			FALSE - no data or error
//			DB_BAD	- unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  ??			SFK		Created
//	02-Aug-2002	SFK		Changed to use DATE timebase 
//	02-Oct-2003	SFK		Modified for new FacMgr
//////////////////////////////////////////////////////////////////
// new with DATE
int GetDataByStationMarks(struct IDStruct ChanID, DATE dStartTime, DATE dEndTime, unsigned long ulNumMarks, unsigned long *pulNumMarksRead, struct db_mark_rec *pMarks)
{
    struct db_mark_rec  dbMark;
    unsigned long  ulNum;
    int  db_stat, status;
    DATE dCurrentDay;		    // day you want to find in database in jul secs  
	BOOL bMarksInDay;
    DATE dActualEndTime;
    DATE dActualStartTime; 
    unsigned long ulDum;
    BOOL bDone = FALSE;
	BOOL bLastDay = FALSE;   
    CGFmtStr msg;

    *pulNumMarksRead = 0;  
    if (ulNumMarks == 0) return(TRUE);
    if (dEndTime < dStartTime) return(TRUE);
           
   // ------------------------------------------------------------------
   //	Get the station associated with the channel, if both exist.
   // ---------------------------------------------------------------- 
	status = StationExistsInMainDb(ChanID.sStaID);
    if (status != TRUE) return(status);
                         
	status = GetDataStatistics(ChanID, true, dStartTime, dEndTime, &dActualStartTime, &dActualEndTime, &ulDum);	
	if (status != TRUE) return(status);
    dCurrentDay = dActualStartTime; 
	status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dCurrentDay);	// position curr rec at first day's record
    if (status != TRUE) return(status);

    
   // ------------------------------------------------------------------
   //	Look through the marks for all days and read the first NumMarks
   //	marks that occur at or after the start time.
   // ---------------------------------------------------------------- 
    ulNum = 0;
    do {
	    db_stat = d_setor(DAY_TO_MARK_SET, CURR_DB); 	// select Mark set  
	    if (db_stat != S_OKAY) goto db_exit;
	    
	    bMarksInDay = TRUE;
	    db_stat = d_findfm(DAY_TO_MARK_SET, CURR_DB); 	// see if any marks  
	    if (db_stat == S_EOS) {
	    	bMarksInDay = FALSE;
	    }
	    else {	
	    	if (db_stat != S_OKAY) goto db_exit;
	    }	
	                                          
		if (bMarksInDay == TRUE) {
		   	do {                                            // count the marks  
				db_stat = d_recread(&dbMark, CURR_DB);
				if (db_stat != S_OKAY) goto db_exit;
				if ((dbMark.d_mark_time_key >= dActualStartTime) &&
					(dbMark.d_mark_time_key <= dActualEndTime) && 
					(dbMark.s_mark_chan == ChanID.sChanID) &&
					(dbMark.s_mark_sta == ChanID.sStaID) &&
					(dbMark.s_mark_sta_type = ChanID.sStaType)) {
					pMarks[ulNum] = dbMark;
					ulNum++;
				}	 
				db_stat = d_findnm(DAY_TO_MARK_SET, CURR_DB);
			} while ((db_stat == S_OKAY) && (ulNum < ulNumMarks));
		}	
		if ((int)dCurrentDay == (int)dActualEndTime) bDone = TRUE;
		if (!bDone) {	// no, find the next day
			do {
				dCurrentDay += 1.0;				// more days to search  
				if ((int)dCurrentDay == (int)dActualEndTime) bLastDay = TRUE;
				status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dCurrentDay);
	    	} while ((status != TRUE) && (bLastDay == FALSE)); 	
	    }	
    } while ((ulNum < ulNumMarks) && (!bDone)); 	
    *pulNumMarksRead = ulNum;
    
	return(TRUE);
	
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "GetDataByStationMarks", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}       


// <f>  
//===========================================================================
//
//  Name	 :  DeleteDbMarks
//
//  Purpose	 :  Find all mark records associated with specified facility and
//			    delete them from the database.
//
//  Return value :  TRUE - all marks for facility deleted
//					FALSE	- facility not found in db
//					DB_BAD	- unexpected db error
//
//  Global variables: Variables named with a gl_ prefix are defined in
//			    	  main.c.	Variables with a gll_ prefix are defined
//  		    	  at the top of this module.
//
//  Implicit Input :int facility;   input - unique facility number
//
//  Implicit Input/Output:
//  Implicit Output:
//
//  date    /	author	revision
//  -----------------	--------
//  20-Jun-1996 SFK		Created
//
//=========================================================================== 
int DeleteDbMarks(short sFacNum)

{
    DB_ADDR dbStaAdr, dbDayAdr;	// db address of day trying to delete  
    int status;
    int db_stat;

       
   // ------------------------------------------------------------------
   //	Check if the facility is in the database
   // ---------------------------------------------------------------- 
	status = FacilityExistsInMainDb(sFacNum);
    if (status != TRUE) return(status);

   // ------------------------------------------------------------------
   //	Next find all the stations in this facility.  Then for each
   //   station look for all the days for it and finally delete all
   //   the marks associated with every day.
   // ---------------------------------------------------------------- 
    db_stat = d_setor(FACILITY_TO_STATION_SET, CURR_DB); 
    if (db_stat != S_OKAY) goto db_exit;
    
    for (db_stat = d_findfm(FACILITY_TO_STATION_SET, CURR_DB); db_stat == S_OKAY;
		db_stat = d_findnm(FACILITY_TO_STATION_SET, CURR_DB)) {
	
		db_stat = d_crget(&dbStaAdr, CURR_DB);	// save station adr for later  
    	if (db_stat != S_OKAY) goto db_exit;            
		db_stat = d_setor(STATION_TO_DAY_SET, CURR_DB); 
    	if (db_stat != S_OKAY) goto db_exit;

	    for (db_stat = d_findfm(STATION_TO_DAY_SET, CURR_DB); db_stat == S_OKAY;
		db_stat = d_findnm(STATION_TO_DAY_SET, CURR_DB)) {
    	
			db_stat = d_crget(&dbDayAdr, CURR_DB);	// save day adr for later  
	    	if (db_stat != S_OKAY) goto db_exit;  
			db_stat = d_setor(DAY_TO_MARK_SET, CURR_DB); 
	    	if (db_stat != S_OKAY) goto db_exit;

		    for (db_stat = d_findfm(DAY_TO_MARK_SET, CURR_DB); db_stat == S_OKAY;
			db_stat = d_findnm(DAY_TO_MARK_SET, CURR_DB)) { 
			
	   			db_stat = d_disdel(CURR_DB);
				if (db_stat != S_OKAY) goto db_exit;
			}
				
			db_stat = d_crset(&dbDayAdr, CURR_DB);	// restore day adr  
	    	if (db_stat != S_OKAY) goto db_exit;  
	    }
	    
		db_stat = d_crset(&dbStaAdr, CURR_DB);	// restore sta adr  
    	if (db_stat != S_OKAY) goto db_exit;  
    }	
	    	
    return(TRUE);

db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "DeleteDbMarks", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}


// <f>  
//===========================================================================
//
//  Name	 :  ScanMarkRecs
//
//  Purpose	 :  This allows the user to scan through all mark records found
//				in the database.  They are stored in the db by time.  User can
//				select to find the record closest to a specified time or just
//				the next record in the list.
//
//  Return value :  TRUE - found an mark
//				    uiMARK_NOT_IN_DB - mark not in db
//					FALSE - Type not FIRST or NEXT
//					DB_BAD	- unexpected db error
//
//  Special notes:  if TRUE, curr_rec = mark looking for
//					if uiMARK_NOT_IN_DB_WARN, curr_rec = day where mark should attach to
//
//  Implicit Input : struct db_mark_rec *pdbMark - structure describinb mark looking for
//  Implicit Input/Output:
//  Implicit Output:
//
//  date    /	author	revision
//  -----------------	--------
//  20-Jul-1995 SFK	Created
//	07-Aug-2002	SFK		Changed to use DATE 
//
//=========================================================================== 
int ScanMarkRecs(DATE dStartTime, short sType, struct db_mark_rec *pdbMark)
{
    
    int db_stat;
        
    if (sType == FIRST) {
    	db_stat = d_keyfind(D_MARK_TIME_KEY, &dStartTime, CURR_DB);
    	if (db_stat != S_OKAY) {
    		db_stat = d_keynext(D_MARK_TIME_KEY, CURR_DB);
    	}	
    	if (db_stat == S_NOTFOUND) return(uiMARK_NOT_IN_DB_WARN);
        if (db_stat != S_OKAY) goto db_exit;
        
        db_stat = d_recread(pdbMark, CURR_DB);
        return(TRUE);
    }
    
    if (sType == NEXT) {
   		db_stat = d_keynext(D_MARK_TIME_KEY, CURR_DB);
   		if (db_stat == S_NOTFOUND) return(uiMARK_NOT_IN_DB_WARN);
        if (db_stat != S_OKAY) goto db_exit;
        
        db_stat = d_recread(pdbMark, CURR_DB);
        return(TRUE);
    }                                                  
    return(FALSE);
    
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "ScanMarkRecs", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);

}    
 
                                                                  