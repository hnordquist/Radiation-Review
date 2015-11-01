///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbReadArray.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		GetDataByChannel
//
//		All times are assumed to be adjusted (values adjusted before put into array)
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "AdjustTime.h"
#include "DbDay.h"
#include "DbLimit.h"
#include "DbTime.h"
#include "FacilityConfig.h"
#include "TimeAlign.h"
#include "TypeDefinitions.h"
#include "EOSSImport.h"
#include "BinaryImport.h"

extern CEOSSImport *pglEOSSImport;
extern CBinaryImport *pglBinaryImport;

extern short glsFacNum;
extern CDbDay glDbDay;

//===========================================================================
//
//  Name	 :  ReadDbDataArray
//
//  Purpose	 :  Read only x,y data associated with a time interval in the database
//				where x is the julian time and y is one of the data channels.
//				To simplify the graphing, the x data is offset so that the time
//				associated with ulStartTime is 0.
//
//  Return value :  error from find_day()
//		    error from find_time_in_db
//		    db_vista error
//		    TRUE
//
//  Special notes:
//
//  Global variables:  Variables named with a gl_ prefix are defined in
//		       main.c.	Variables with a gll_ prefix are defined
//		       at the top of this module.
//
//  Implicit Input :
//    int   station;		input - which channel number 
//    unsigned long  ulStartTime;	input - julian time of point want to find in db 
//    unsigned int   ulNumPts;	 	input - number of points to read 
//    struct GRAND_DATA_PT ptr_raw_data[];
//  Implicit Input/Output:
//  Implicit Output:

//  date    /	author	revision
//  -----------------	--------
//  02-Jun-1995  SFK	 Created from old read_other_data
//	21-Mar-2001 SFK		Version 2.07 Added hooks into data residing in Access database
//	01-Oct-2001	 SFK	Verion 2.071 Added timestamp adjust to align stations.
//  16-Oct-2001  SFK	 Version 2.07a - Added returning the actual end time (unadjusted)
//	30-Sep-2003 SFK		Version 3.0.3 - mods for new FacMgr
//
//=========================================================================== 
// new with DATE
int GetDataByChannel(
	struct IDStruct ChanID, 
	DATE dAdjStartTime, 
	DATE dAdjEndTime, 
	unsigned long ulNumPts, 
	unsigned long *pulNumPts, 
	double *pdX, double *pdY,  
	DATE *pdAdjTimestampOfNextPoint)
{

    struct db_float_data_rec dbfData;   
    struct db_double_data_rec dbdData;   
    struct db_chan_rec dbChan;  		
    struct db_sta_rec dbSta;
    DB_ADDR dba;
    
    int i;
    int db_stat, status;
    DATE dAdjCurrentDay;
	DATE dAdjEndDbTime, dFirstDay;
	DATE dUnAdjStart, dUnAdjEnd;
    unsigned long j;
	DATE dAdjTimestampOfNextPoint;
	bool bFinished = false;
	BOOL bSame = FALSE;
    
    *pulNumPts = 0;
    if ((ulNumPts == 0) && (ChanID.sStaType != EOSS_TYPE))
		return(TRUE);
    
	//  If the data type is one of the COM types, then get data via the COMs

	if (ChanID.sStaType == EOSS_TYPE)
	{
		*pulNumPts = ulNumPts;
		status = pglEOSSImport->GetDataByChannel(glsFacNum, ChanID.sStaID, ChanID.sChanID, dAdjStartTime, dAdjEndTime, pdX, pdY, pulNumPts, pdAdjTimestampOfNextPoint, ChanID.sStaType);
		return(status);
	}
	else if (ChanID.sStaType == BINARY_TYPE)
	{
		*pulNumPts = ulNumPts;
		status = pglBinaryImport->GetDataByChannel(glsFacNum, ChanID.sStaID, ChanID.sChanID, dAdjStartTime, dAdjEndTime, pdX, pdY, pulNumPts, pdAdjTimestampOfNextPoint, ChanID.sStaType);
		return(status);
	}
	else if (pglTimeAlign->COMDataType(ChanID.sStaType)) 
	{
		*pulNumPts = ulNumPts;
		status = pglTimeAlign->GetDataByChannel(glsFacNum, ChanID.sStaID, ChanID.sChanID, dAdjStartTime, dAdjEndTime, pdX, pdY, pulNumPts, pdAdjTimestampOfNextPoint, ChanID.sStaType);
		return(status);
	}


   // ------------------------------------------------------------------
   //	Get the channel record so can determine which data channel to read
   // ---------------------------------------------------------------- 
	status = pglFacCfg->GetChannelRecord(glsFacNum, &ChanID, &dbChan);
	if (status != TRUE) return(status);
	status = pglFacCfg->GetStationRecord(glsFacNum, &ChanID, &dbSta);
	
   // ------------------------------------------------------------------
   //	Find the last day in the db for this station, will use later
   // ---------------------------------------------------------------- 
	//BOOL bStatus = GetDbTimeRange(sSta, &dAdjStartDbTime, &dAdjEndDbTime);
	BOOL bStatus = GetDbTimeRange(ChanID, &dFirstDay, &dAdjEndDbTime);	// 01-Nov-2004 SFK
    if (bStatus != TRUE) return(FALSE);
	if (dFirstDay < dAdjStartTime ) dFirstDay = dAdjStartTime;			// 01-Nov-2004 SFK
    
	dUnAdjStart = UnadjustTimestamp(ChanID, dFirstDay);					// 01-Nov-2004 SFK
	dUnAdjEnd = UnadjustTimestamp(ChanID, dAdjEndDbTime);

   switch (dbSta.s_data_type) {
    	case FLOAT_TYPE:
	   // ------------------------------------------------------------------
		//	Find the multiple record containing the starting time in the database.
		// ---------------------------------------------------------------- 
		status = FindDbDataRecContainingTime(ChanID, dUnAdjStart, SEARCH_START, &dbfData);
		if (status != TRUE) return(status);

	   // ------------------------------------------------------------------
	   //	Get the correct current owner and member set relationships of
	   //   the multiple record, so can scan through the records in the day later.
	   // ---------------------------------------------------------------- 
	   	if (d_crget(&dba, CURR_DB) != S_OKAY) goto db_exit;
	    if (d_findco(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;      
	    if (d_crset(&dba, CURR_DB) != S_OKAY) goto db_exit;
	    
	   // ------------------------------------------------------------------
	   //	Look for the starting time within the multiple record
	   // ---------------------------------------------------------------- 
	    i = 0;
	    while ((dbfData.d_fdata_time[i] < dUnAdjStart) && (i < (RAW_DATA_RECS_PER_DB_RECORD-1))) 
			i++;
	    
	
	   // ------------------------------------------------------------------
	   //	Found starting time - transfer data into users struct array
	   // ---------------------------------------------------------------- 
	    dAdjCurrentDay = dAdjStartTime;
	    j = 0;
		do {
			do {
			   // ------------------------------------------------------------------
	    		//	If some points are out of order before the start time
	    		//	asked for, set their time to start time.
	    		// ---------------------------------------------------------------- 
				if (dbfData.d_fdata_time[i] < dUnAdjStart) dbfData.d_fdata_time[i] = dUnAdjStart;
				pdX[j] = AdjustTimestampZeroBased(ChanID, dbfData.d_fdata_time[i], dAdjStartTime, dAdjEndTime);

			   // ------------------------------------------------------------------
	    		//	Temporarily, if have points out of order, label them with the
				//	same time as the last time read.
	    		// ---------------------------------------------------------------- 
				if (j > 0) if (pdX[j] <= pdX[j-1]) pdX[j] = pdX[j-1] + 1.0/864000.0; 

				switch (dbChan.s_chan_offset) {
	        		case 1:
	        			pdY[j] = (double)dbfData.f_data_chan1[i];
	        			break;
		        			
	        		case 2:
	        			pdY[j] = (double)dbfData.f_data_chan2[i];
	        			break;
		        			
	        		case 3:
	        			pdY[j] = (double)dbfData.f_data_chan3[i];
	        			break;
		        			
	        		case 4:
	        			pdY[j] = (double)dbfData.f_data_chan4[i];
	        			break;
		        			
	        		case 5:
	        			pdY[j] = (double)dbfData.f_data_chan5[i];
	        			break;
		        			
	        		case 6:
	        			pdY[j] = (double)dbfData.f_data_chan6[i];
	        			break;
		        			
	        		case 7:
	        			pdY[j] = (double)dbfData.f_data_chan7[i];
	        			break;                                  
	        	}		

		    	i++;
			   	j++;

			} while ((i<RAW_DATA_RECS_PER_DB_RECORD) && (dbfData.d_fdata_time[i] != 0.0) && (j != ulNumPts));
	
		   // ------------------------------------------------------------------
			//   When have read all the points requested, must figure out the timestamp
			//	of the next point to return.  
			// ---------------------------------------------------------------- 
			if (j == ulNumPts) {
				// see if can use the next point in this multiple record
				if ((i < RAW_DATA_RECS_PER_DB_RECORD) && (dbfData.d_fdata_time[i] != 0)) {
					dAdjTimestampOfNextPoint = dbfData.d_fdata_time[i];		// this is the next data point to be readalways update the last time with each point
					bFinished = true;
				}
				else {	// must get next record in db and read from it
	    			db_stat  = d_findnm(DAY_TO_FLOAT_DATA_SET, CURR_DB);

	       			if (db_stat == S_EOS) {		// see if there is another day in the db
						if ((int)dAdjCurrentDay == (int)dAdjEndDbTime) {	// no more days in db
							*pulNumPts = j;
							dAdjTimestampOfNextPoint = dbfData.d_fdata_time[i-1];	// best guess available
							*pdAdjTimestampOfNextPoint = AdjustTimestamp(ChanID, dAdjTimestampOfNextPoint);
							return(true);
						}	
						bSame = FALSE;
						do {
		    				dAdjCurrentDay += 1.0;
		    				status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dAdjCurrentDay);
							if ((int)dAdjCurrentDay == (int)dAdjEndDbTime) bSame = TRUE;
						} while ((status == uiDAY_NOT_IN_DB_STATUS) && !bSame);
					
						if (status != TRUE) {	// there is no more (good) data in the db, use the last time of last record read
							*pulNumPts = j;
							dAdjTimestampOfNextPoint = dbfData.d_fdata_time[i-1];	// best guess available
							*pdAdjTimestampOfNextPoint = AdjustTimestamp(ChanID, dAdjTimestampOfNextPoint);
							return(true);
						}	
                            
						// there is another day in the db, get ready to read the first record
						if (d_setor(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
						if (d_findfm(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
	    			}
					if (d_recread((char *)&dbfData, CURR_DB) != S_OKAY) goto db_exit;
	     			i = 0;
					dAdjTimestampOfNextPoint = dbfData.d_fdata_time[i];		// this is the next data point to be read
					dAdjTimestampOfNextPoint = AdjustTimestamp(ChanID, dAdjTimestampOfNextPoint);
					bFinished = true;
				}
			}		

			// ------------------------------------------------------------------
			//   Move on to the next record in this day if it exists, if no
			//	more records in this day, then move on to the next day in the db.
			// ---------------------------------------------------------------- 
			if ((j != ulNumPts)  || !bFinished) {
		    	db_stat  = d_findnm(DAY_TO_FLOAT_DATA_SET, CURR_DB);
		       	if (db_stat == S_EOS) {
					bSame = FALSE;
					do {
			    		dAdjCurrentDay += 1.0;
			    		status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dAdjCurrentDay);
						if ((int)dAdjCurrentDay == (int)dAdjEndDbTime) bSame = TRUE;
					} while ((status == uiDAY_NOT_IN_DB_STATUS) && !bSame);
				
					if (status != TRUE) {
						*pulNumPts = j;
						return(TRUE);
					}	
	
					if (d_setor(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;				                                          
					if (d_findfm(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
		    	}
		    	else {
					if (db_stat != S_OKAY) goto db_exit;
				}		    		
		        if (d_recread((char *)&dbfData, CURR_DB) != S_OKAY) goto db_exit; // debug
		     	i = 0;
			}
	    } while (j != ulNumPts);
	    break;
       
    	case DOUBLE_TYPE:
	   // ------------------------------------------------------------------
		//	Find the multiple record containing the starting time in the database.
		// ---------------------------------------------------------------- 
		status = FindDbDataRecContainingTime(ChanID, dUnAdjStart, SEARCH_START, &dbdData);
		if (status != TRUE) return(status);

	   // ------------------------------------------------------------------
	   //	Get the correct current owner and member set relationships of
	   //   the multiple record, so can scan through the records in the day later.
	   // ---------------------------------------------------------------- 
	   	if (d_crget(&dba, CURR_DB) != S_OKAY) goto db_exit;
	    if (d_findco(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;      
	    if (d_crset(&dba, CURR_DB) != S_OKAY) goto db_exit;
	    
	   // ------------------------------------------------------------------
	   //	look for the starting time within the multiple record
	   // ---------------------------------------------------------------- 
	    i = 0;
	    while ((dbdData.d_ddata_time[i] < dUnAdjStart) && (i < (RAW_DATA_RECS_PER_DB_RECORD-1))) 
			i++;

	   // ------------------------------------------------------------------
	   //	Found starting time - transfer data into users struct array
	   // ---------------------------------------------------------------- 
	    dAdjCurrentDay = dAdjStartTime;
	    j = 0;
		do {
			do {
			   // ------------------------------------------------------------------
	    		//	If some points are out of order before the start time
	    		//	asked for, set their time to start time.
	    		// ---------------------------------------------------------------- 
				if (dbdData.d_ddata_time[i] < dUnAdjStart) dbdData.d_ddata_time[i] = dUnAdjStart;
				pdX[j] = AdjustTimestampZeroBased(ChanID, dbdData.d_ddata_time[i], dAdjStartTime, dAdjEndTime);
				 					
			   // ------------------------------------------------------------------
	    		//	Temporarily, if have points out of order, label them with the
				//	same time as the last time read.
	    		// ---------------------------------------------------------------- 
				if (j > 0) if (pdX[j] <= pdX[j-1]) pdX[j] = pdX[j-1] + (float)1.0/864000.0; 

			   // ------------------------------------------------------------------
	    		//	With SR data calculate the count rates only for plotting
	    		// ---------------------------------------------------------------- 
				if (dbdData.d_ddata_etime[i] == 0) dbdData.d_ddata_etime[i] = 1.0; // watch for divide by 0
	        	switch (dbChan.s_chan_offset) {
	        		case 1:
	        			pdY[j] = (double)dbdData.d_data_chan1[i]/dbdData.d_ddata_etime[i];
	        			break;
		        			
	        		case 2:
	        			pdY[j] = (double)dbdData.d_data_chan2[i]/dbdData.d_ddata_etime[i];
	        			break;
		        			
	        		case 3:
	        			pdY[j] = (double)dbdData.d_data_chan3[i]/dbdData.d_ddata_etime[i];
	        			break;
		        			
	        		case 4:
	        			pdY[j] = (double)dbdData.d_data_chan4[i]/dbdData.d_ddata_etime[i];
	        			break;
		        			
	        		case 5:
	        			pdY[j] = (double)dbdData.d_data_chan5[i]/dbdData.d_ddata_etime[i];
	        			break;
		        			
	        		case 6:
	        			//pdY[j] = (double)dbdData.d_data_chan6[i]/dbdData.d_ddata_etime[i];
						pdY[j] = (double)dbdData.d_data_chan6[i];		// 23-May-2005 SFK don't divide reals/totals ratio by time
	        			break;
		        			
	        		case 7:
	        			pdY[j] = (double)dbdData.d_data_chan7[i]/dbdData.d_ddata_etime[i];
	        			break;                                  
	        	}
		    	i++;
		    	j++;
			} while ((i<RAW_DATA_RECS_PER_DB_RECORD) && (dbdData.d_ddata_time[i] != 0) && (j != ulNumPts));
	
	       
		   // ------------------------------------------------------------------
			//   When have read all the points requested, must figure out the timestamp
			//	of the next point to return.  
			// ---------------------------------------------------------------- 
			if (j == ulNumPts) {
				// see if can use the next point in this multiple record
				if ((i < RAW_DATA_RECS_PER_DB_RECORD) && (dbdData.d_ddata_time[i] != 0)) {
					dAdjTimestampOfNextPoint = dbdData.d_ddata_time[i];		// this is the next data point to be readalways update the last time with each point
					bFinished = true;
				}
				else {	// must get next record in db and read from it
	    			db_stat  = d_findnm(DAY_TO_DOUBLE_DATA_SET, CURR_DB);

	       			if (db_stat == S_EOS) {		// see if there is another day in the db
						if ((int)dAdjCurrentDay == (int)dAdjEndDbTime) {	// no more days in db
							*pulNumPts = j;
							dAdjTimestampOfNextPoint = dbdData.d_ddata_time[i-1];	// best guess available
							*pdAdjTimestampOfNextPoint = AdjustTimestamp(ChanID, dAdjTimestampOfNextPoint);
							return(true);
						}	
						bSame = FALSE;
						do {
		    				dAdjCurrentDay += 1.0;
		    				status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dAdjCurrentDay);
							if ((int)dAdjCurrentDay == (int)dAdjEndDbTime) bSame = TRUE;
						} while ((status == uiDAY_NOT_IN_DB_STATUS) && !bSame);
					
						if (status != TRUE) {	// there is no more (good) data in the db, use the last time of last record read
							*pulNumPts = j;
							dAdjTimestampOfNextPoint = dbdData.d_ddata_time[i-1];	// best guess available
							*pdAdjTimestampOfNextPoint = AdjustTimestamp(ChanID, dAdjTimestampOfNextPoint);
							return(true);
						}	
                            
						// there is another day in the db, get ready to read the first record
						if (d_setor(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
						if (d_findfm(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
	    			}
					if (d_recread((char *)&dbdData, CURR_DB) != S_OKAY) goto db_exit;
	     			i = 0;
					dAdjTimestampOfNextPoint = dbdData.d_ddata_time[i];		// this is the next data point to be read
					dAdjTimestampOfNextPoint = AdjustTimestamp(ChanID, dAdjTimestampOfNextPoint);
					bFinished = true;
				}
			}		

		   // ------------------------------------------------------------------
			//   Move on to the next record in this day if it exists, if no
			//	more records in this day, then move on to the next day in the db.
			// ---------------------------------------------------------------- 
			if ((j != ulNumPts)  || !bFinished) {
		    	db_stat  = d_findnm(DAY_TO_DOUBLE_DATA_SET, CURR_DB);
		       	if (db_stat == S_EOS) {
					bSame = FALSE;
					do {
			    		dAdjCurrentDay += 1.0;
			    		status = glDbDay.FindDayRecInMainDb(ChanID.sStaID, dAdjCurrentDay);
						if ((int)dAdjCurrentDay == (int)dAdjEndDbTime) bSame = TRUE;
					} while ((status == uiDAY_NOT_IN_DB_STATUS) && !bSame);
				
					if (status != TRUE) {
						*pulNumPts = j;
						return(TRUE);
					}	
	
					if (d_setor(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;				                                          
					if (d_findfm(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
		    	}
		    	else {
					if (db_stat != S_OKAY) goto db_exit;
				}		    		
		        if (d_recread((char *)&dbdData, CURR_DB) != S_OKAY) goto db_exit;	//debug
		     	i = 0;
			}
	    } while (j != ulNumPts);
	    break;

	}   
	*pdAdjTimestampOfNextPoint = dAdjTimestampOfNextPoint;
	*pulNumPts = j;       
    return(TRUE);       
    
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "ReadDbDataArray", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}

