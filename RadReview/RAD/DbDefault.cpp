///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbDefault.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

//19 Apr 2006 - pjm - changed code to reduce compiler warnings.

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include <gen.h>
#include "RAD.h"
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"

extern short glsChanNum;       

static void InitDbDfltRec(struct db_dflt_rec *pdbDflt);                                            
static void InitDbDfltChanRec(struct db_chan_event_dflt_rec *pStruct);
static void InitDbDfltPairRec(struct db_dir_rec *pStruct);
static void InitDbDfltStaRec(struct db_sta_dflt_rec *pStruct);
DB_TASK	DfltTask;                       
static char gllDBName[DRIVE_LEN+PATH_LEN+1]="";
static char gllOldDBName[DRIVE_LEN+PATH_LEN+1]="";
static char gllDBOpen[DRIVE_LEN+PATH_LEN+1]="";

int AskUpdateQuestion()
{
   	GUI_ACTION response;
	CGFmtStr msg;
	char szDbPath[DRIVE_LEN+PATH_LEN+1];

	GetDbName(szDbPath);	             
	msg.Printf("WARNING!!\nThis will change the default parameters in the Default database at");
	msg.PrintfAppend("\n%s.", szDbPath);
	msg.PrintfAppend("\nAre you sure you want to change the defaults?");
	response = GUI_MsgBox(msg, GUI_ICON_QUESTION, GUI_YESNO);

	if (response == GUI_NO) 
		return(0);
	return(1);
}	

    
int AskUpdateINI_Question()
{
   	GUI_ACTION response;
	CGFmtStr msg;
	char szDbPath[DRIVE_LEN+PATH_LEN+1];

	GetDbName(szDbPath);	             
	msg.Printf("WARNING!!\nThis will change the default parameters in the rad.ini file at");
	msg.PrintfAppend("\n%s.", szDbPath);
	msg.PrintfAppend("\nAre you sure you want to change the defaults?");
	response = GUI_MsgBox(msg, GUI_ICON_QUESTION, GUI_YESNO);
	if (response == GUI_NO) return(FALSE);
	return(TRUE);
}	
	

void UpdateSuccessfulMsg()
{
  	CGFmtStr msg;
	char szDbPath[DRIVE_LEN+PATH_LEN+1];

	GetDbName(szDbPath);	             
	msg.Printf("Default database at\n%s\nupdated successfully.", szDbPath);
	GUI_MsgBox(msg, GUI_ICON_INFO);
}	

	

// initialize all values in the db_dflt_rec struct
static void InitDbDfltRec(struct db_dflt_rec *pdbDflt)
{
	size_t numbytes = sizeof db_dflt_rec;

	memset(pdbDflt, 0, numbytes);
}
/*
	int i;

   pdbDflt->s_dflt_facility = 0;
   pdbDflt->s_dflt_station = 0;
   pdbDflt->s_dflt_station_type = 0;
   pdbDflt->s_dflt_channel = 0;
   pdbDflt->s_integrity_station = 0;
   pdbDflt->s_integrity_station_type = 0;
   pdbDflt->s_integrity_tolerance = 1;
   pdbDflt->s_days_station = 0;
   pdbDflt->s_days_station_type = 0;
   pdbDflt->s_chan_event_srch_width = 1;
   pdbDflt->s_chan_event_srch_alg = 0;
   pdbDflt->s_chan_event_summ_sort = 0;
   pdbDflt->s_chan_event_summ_type = 0;
   pdbDflt->s_chan_event_summ_min = 0;
   pdbDflt->s_chan_event_summ_max = 0;
   pdbDflt->s_rad_event_srch_pairs = 1;
   pdbDflt->s_rad_event_summ_sort = 0;
   pdbDflt->s_rad_event_summ_type = 0;
   pdbDflt->s_mark_summ_sort = 0;
   pdbDflt->s_mark_summ_type = 0;
   pdbDflt->b_chan_event_srch_clear = TRUE;
   pdbDflt->b_chan_event_srch_smooth = FALSE;
   pdbDflt->b_chan_event_srch_log_file = FALSE;
   pdbDflt->b_chan_event_summ_log_file = FALSE;
   pdbDflt->b_rad_event_srch_clear = TRUE;
   pdbDflt->b_rad_event_srch_log_file = FALSE;
   pdbDflt->b_rad_event_summ_log_file = FALSE;
   pdbDflt->s_export_station = 0;
   pdbDflt->s_export_station_type = 0;
   pdbDflt->s_export_measurement_type = 0;
   pdbDflt->s_meas_summ_station = 0;
   pdbDflt->s_meas_summ_station_type = 0;
   pdbDflt->s_meas_summ_type = 0;
   pdbDflt->b_meas_srch_clear = TRUE;
   pdbDflt->s_meas_parm_station = 0;
   pdbDflt->s_meas_parm_station_type = 0;
   pdbDflt->s_raw_data_station = 0;
   pdbDflt->s_raw_data_station_type = 0;

   for (i=0; i<32; i++) 
   {
		pdbDflt->b_integrity_flags[i] = FALSE;
		pdbDflt->b_raw_data_flags[i] = FALSE;
   }

   pdbDflt->b_use_adjustments = FALSE;
}
*/
	

// access a Boolean field in the db_dflt_rec struct    
int AccessDefaultDatabase(short sAction, long lField, int *pbBool)
{
	int db_stat;                                 
	CGFmtStr msg;       
	struct db_dflt_rec dbDflt;
	long lMembers;	

/*  This was a coding error.  The boolean for the if is always false.
    pjm 2-27-2008	

	// if not a valid field in db_dflt_rec, quit   	
   	if ((lField == B_CHAN_EVENT_SRCH_CLEAR) && 
		(lField == B_CHAN_EVENT_SRCH_SMOOTH) &&
   		(lField == B_CHAN_EVENT_SRCH_LOG_FILE) && 
		(lField == B_CHAN_EVENT_SUMM_LOG_FILE) &&
   	    (lField == B_RAD_EVENT_SRCH_CLEAR) && 
		(lField == B_RAD_EVENT_SRCH_LOG_FILE) &&
   	    (lField == B_RAD_EVENT_SUMM_LOG_FILE) && 
		(lField == B_MEAS_SRCH_CLEAR) &&
		(lField == B_USE_ADJUSTMENTS)) 
		return(uiINVALID_DFLT_DB_FIELD_ERR);
*/

	// if not a valid field in db_dflt_rec, quit   	
   	if ((lField != B_CHAN_EVENT_SRCH_CLEAR) && 
		(lField != B_CHAN_EVENT_SRCH_SMOOTH) &&
   		(lField != B_CHAN_EVENT_SRCH_LOG_FILE) && 
		(lField != B_CHAN_EVENT_SUMM_LOG_FILE) &&
   	    (lField != B_RAD_EVENT_SRCH_CLEAR) && 
		(lField != B_RAD_EVENT_SRCH_LOG_FILE) &&
   	    (lField != B_RAD_EVENT_SUMM_LOG_FILE) && 
		(lField != B_MEAS_SRCH_CLEAR) &&
		(lField != B_USE_ADJUSTMENTS)) 
		return(uiINVALID_DFLT_DB_FIELD_ERR);
	   	    
   	switch (sAction) {
		case READ:
   			db_stat = d_members(DEFAULT_SET, &lMembers, 1);		/* check if there is anything to read */
   			if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
   			if (lMembers > 0) {
				db_stat = d_findfm(DEFAULT_SET, 1);
				if (db_stat == S_OKAY) { /* record in db */
					db_stat = d_recread(&dbDflt, 1);
					if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
				} 
			}	
			if ((lMembers == 0) || (db_stat != S_OKAY)) {
				InitDbDfltRec(&dbDflt);	/* get the defaults into a dummy rec  */		
			}	
			switch (lField) { 
	   			case B_CHAN_EVENT_SRCH_CLEAR:
	   				*pbBool = dbDflt.b_chan_event_srch_clear;
				break;
	   			case B_CHAN_EVENT_SRCH_SMOOTH:
	   				*pbBool = dbDflt.b_chan_event_srch_smooth;
	   			break;
	   			case B_CHAN_EVENT_SRCH_LOG_FILE:
	   				*pbBool = dbDflt.b_chan_event_srch_log_file;
	   			break; 
	   			
	   			case B_CHAN_EVENT_SUMM_LOG_FILE:
	   				*pbBool = dbDflt.b_chan_event_summ_log_file;
	   			break; 
	   			
	   			case B_RAD_EVENT_SRCH_CLEAR:
	   				*pbBool = dbDflt.b_rad_event_srch_clear;
	   			break;
	   			case B_RAD_EVENT_SRCH_LOG_FILE:
	   				*pbBool = dbDflt.b_rad_event_srch_log_file;
	   			break; 
	   			case B_RAD_EVENT_SUMM_LOG_FILE:
	   				*pbBool = dbDflt.b_rad_event_summ_log_file;
	   			break; 
				
				case B_MEAS_SRCH_CLEAR:
	   				*pbBool = dbDflt.b_meas_srch_clear;
	   			break;

				case B_USE_ADJUSTMENTS:
					*pbBool = dbDflt.b_use_adjustments;
			break;
	   		}	
		break;   	
	
		case WRITE:
		   	db_stat = d_members(DEFAULT_SET, &lMembers, 1);		/* check if there is anything to read */
   			if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
   			if (lMembers > 0) {
				db_stat = d_findfm(DEFAULT_SET, 1);
				if (db_stat == S_OKAY) {
					db_stat = d_recread(&dbDflt, 1);
					if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
					switch (lField) {
			   			case B_CHAN_EVENT_SRCH_CLEAR:
			   				dbDflt.b_chan_event_srch_clear = *pbBool;
			   			break;
			   			case B_CHAN_EVENT_SRCH_SMOOTH:
			   				dbDflt.b_chan_event_srch_smooth = *pbBool;
			   			break;                
			   			case B_CHAN_EVENT_SRCH_LOG_FILE:
			   				dbDflt.b_chan_event_srch_log_file = *pbBool;
			   			break; 
			   			
			   			case B_CHAN_EVENT_SUMM_LOG_FILE:
			   				dbDflt.b_chan_event_summ_log_file = *pbBool;
			   			break; 
			   			
			   			case B_RAD_EVENT_SRCH_CLEAR:
			   				dbDflt.b_rad_event_srch_clear = *pbBool;
			   			break;
			   			case B_RAD_EVENT_SRCH_LOG_FILE:
			   				dbDflt.b_rad_event_srch_log_file = *pbBool;
			   			break; 
			   			case B_RAD_EVENT_SUMM_LOG_FILE:
			   				dbDflt.b_rad_event_summ_log_file = *pbBool;
			   			break; 

						case B_MEAS_SRCH_CLEAR:
			   				dbDflt.b_meas_srch_clear = *pbBool;
			   			break;
					   		
						case B_USE_ADJUSTMENTS:
							dbDflt.b_use_adjustments = *pbBool;
						break;
		   			}    
		   		}	
	   			db_stat = d_recwrite(&dbDflt, 1);
	   			if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
		   		db_stat = d_recread(&dbDflt, 1);
			}
	   		else {
	   			InitDbDfltRec(&dbDflt);	/* first time writing so default all */				
				switch (lField) {
		   			case B_CHAN_EVENT_SRCH_CLEAR:
		   				dbDflt.b_chan_event_srch_clear = *pbBool;
		   			break;
		   			case B_CHAN_EVENT_SRCH_SMOOTH:
		   				dbDflt.b_chan_event_srch_smooth = *pbBool;
		   			break;
		   			case B_CHAN_EVENT_SRCH_LOG_FILE:
		   				dbDflt.b_chan_event_srch_log_file = *pbBool;
		   			break; 

					case B_CHAN_EVENT_SUMM_LOG_FILE:
		   				dbDflt.b_chan_event_summ_log_file = *pbBool;
		   			break;
		   			
		   			case B_RAD_EVENT_SRCH_CLEAR:
		   				dbDflt.b_rad_event_srch_clear = *pbBool;
		   			break;
		   			case B_RAD_EVENT_SRCH_LOG_FILE:
		   				dbDflt.b_rad_event_srch_log_file = *pbBool;
		   			break; 
		   			case B_RAD_EVENT_SUMM_LOG_FILE:
		   				dbDflt.b_rad_event_summ_log_file = *pbBool;
		   			break; 

					case B_MEAS_SRCH_CLEAR:
		   				dbDflt.b_meas_srch_clear = *pbBool;
		   			break;

					case B_USE_ADJUSTMENTS:
						dbDflt.b_use_adjustments = *pbBool;
					break;
				}  	
				db_stat = d_fillnew(DB_DFLT_REC, (char *)&dbDflt, 1);
				db_stat = d_connect(DEFAULT_SET, 1);
			}
		break;    
   	}
   	return(TRUE);
}   

// access a short field in the db_dflt_rec struct    
int AccessDefaultDatabase(short sAction, long lField, short *pShort)
{

	int db_stat;                                 
	CGFmtStr msg;       
	struct db_dflt_rec dbDflt;
	long lMembers;	

	/* if not a valid field, then quit */   	
   	if ((lField != S_DFLT_FACILITY) && 
		(lField != S_DFLT_STATION) && (lField != S_DFLT_STATION_TYPE) && 
		(lField != S_DFLT_CHANNEL) &&
		(lField != S_DAYS_STATION) && (lField != S_DAYS_STATION_TYPE) &&
		(lField != S_INTEGRITY_STATION) && 	(lField != S_INTEGRITY_STATION_TYPE) && (lField != S_INTEGRITY_TOLERANCE) &&
		(lField != S_CHAN_EVENT_SRCH_WIDTH) && (lField != S_CHAN_EVENT_SRCH_ALG) && (lField != S_CHAN_EVENT_SUMM_SORT) && 
		(lField != S_CHAN_EVENT_SUMM_TYPE) && (lField != S_CHAN_EVENT_SUMM_MIN) && (lField != S_CHAN_EVENT_SUMM_MAX) &&
   	    (lField != S_RAD_EVENT_SRCH_PAIRS) && (lField != S_RAD_EVENT_SUMM_SORT) && (lField != S_RAD_EVENT_SUMM_TYPE) &&
		(lField != S_MARK_SUMM_SORT) && (lField != S_MARK_SUMM_TYPE) && 
		(lField != S_EXPORT_STATION)  && (lField != S_EXPORT_STATION_TYPE) && (lField != S_EXPORT_MEASUREMENT_TYPE) &&
		(lField != S_MEAS_SUMM_STATION)  && (lField != S_MEAS_SUMM_STATION_TYPE)  && (lField != S_MEAS_SUMM_TYPE) &&
   	    (lField != S_MEAS_PARM_STATION) && (lField != S_MEAS_PARM_STATION_TYPE) &&
		(lField != S_RAW_DATA_STATION) && (lField != S_RAW_DATA_STATION_TYPE)
		)return(uiINVALID_DFLT_DB_FIELD_ERR);
   	    
   	switch (sAction) {
        case READ:
   			db_stat = d_members(DEFAULT_SET, &lMembers, 1);		/* check if there is anything to read */
   			if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
   			if (lMembers > 0) {
				db_stat = d_findfm(DEFAULT_SET, 1);
				if (db_stat == S_OKAY) { /* record in db */
					db_stat = d_recread(&dbDflt, 1);
					if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
				} 
			}	
			if ((lMembers == 0) || (db_stat != S_OKAY)) {
				InitDbDfltRec(&dbDflt);	/* get the defaults into a dummy rec  */		
			}	
			switch (lField) { 
	   			case S_DFLT_FACILITY: 
	   				*pShort = dbDflt.s_dflt_facility;
	   			break;
		   			
	   			case S_DFLT_STATION:
	   				*pShort = dbDflt.s_dflt_station;
	   			break;
	   			case S_DFLT_STATION_TYPE:
	   				*pShort = dbDflt.s_dflt_station_type;
	   			break;

				case S_DFLT_CHANNEL:
	   				*pShort = dbDflt.s_dflt_channel;
	   			break;
		   			
	   			case S_DAYS_STATION:
	   				*pShort = dbDflt.s_days_station;
	   			break;
	   			case S_DAYS_STATION_TYPE:
	   				*pShort = dbDflt.s_days_station_type;
	   			break;

				case S_INTEGRITY_STATION:
	   				*pShort = dbDflt.s_integrity_station;
	   			break;
				case S_INTEGRITY_STATION_TYPE:
	   				*pShort = dbDflt.s_integrity_station_type;
	   			break;
				case S_INTEGRITY_TOLERANCE:
	   				*pShort = dbDflt.s_integrity_tolerance;
	   			break;

	   			case S_CHAN_EVENT_SRCH_WIDTH:
	   				*pShort = dbDflt.s_chan_event_srch_width;
	   			break;
	   			case S_CHAN_EVENT_SRCH_ALG:
	   				*pShort = dbDflt.s_chan_event_srch_alg;
	   			break;

				case S_CHAN_EVENT_SUMM_SORT:
	   				*pShort = dbDflt.s_chan_event_summ_sort;
	   			break;
	   			case S_CHAN_EVENT_SUMM_TYPE:
	   				*pShort = dbDflt.s_chan_event_summ_type;
	   			break; 
	   			case S_CHAN_EVENT_SUMM_MIN:
	   				*pShort = dbDflt.s_chan_event_summ_min;
	   			break; 
	   			case S_CHAN_EVENT_SUMM_MAX:
	   				*pShort = dbDflt.s_chan_event_summ_max;
	   			break; 
	   			
	   			case S_RAD_EVENT_SRCH_PAIRS:
	   				*pShort = dbDflt.s_rad_event_srch_pairs;
	   				break;
	   			case S_RAD_EVENT_SUMM_SORT:
	   				*pShort = dbDflt.s_rad_event_summ_sort;
	   			break;
	   			case S_RAD_EVENT_SUMM_TYPE:
	   				*pShort = dbDflt.s_rad_event_summ_type;
	   			break; 

	   			case S_MARK_SUMM_SORT:
	   				*pShort = dbDflt.s_mark_summ_sort;
	   			break;
	   			case S_MARK_SUMM_TYPE:
	   				*pShort = dbDflt.s_mark_summ_type;
	   			break; 

				case S_EXPORT_STATION:
	   				*pShort = dbDflt.s_export_station;
	   			break;
				case S_EXPORT_STATION_TYPE:
	   				*pShort = dbDflt.s_export_station_type;
	   			break;
	   			case S_EXPORT_MEASUREMENT_TYPE:
	   				*pShort = dbDflt.s_export_measurement_type;
	   			break; 

				case S_MEAS_SUMM_STATION:
	   				*pShort = dbDflt.s_meas_summ_station;
	   			break;
				case S_MEAS_SUMM_STATION_TYPE:
	   				*pShort = dbDflt.s_meas_summ_station_type;
	   			break;
	   			case S_MEAS_SUMM_TYPE:
	   				*pShort = dbDflt.s_meas_summ_type;
	   			break; 

				case S_MEAS_PARM_STATION:
	   				*pShort = dbDflt.s_meas_parm_station;
	   			break;
				case S_MEAS_PARM_STATION_TYPE:
	   				*pShort = dbDflt.s_meas_parm_station_type;
	   			break;

				case S_RAW_DATA_STATION:
	   				*pShort = dbDflt.s_raw_data_station;
	   			break;
				case S_RAW_DATA_STATION_TYPE:
	   				*pShort = dbDflt.s_raw_data_station_type;
	   			break;
			}		
		break;   	

        case WRITE:
   			db_stat = d_members(DEFAULT_SET, &lMembers, 1);		/* check if there is anything to read */
   			if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
   			if (lMembers > 0) {
				db_stat = d_findfm(DEFAULT_SET, 1);
				if (db_stat == S_OKAY) {
					db_stat = d_recread(&dbDflt, 1);
					if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
					switch (lField) {
			   			case S_DFLT_FACILITY: 
			   				dbDflt.s_dflt_facility = *pShort;
			   			break;
			   			
			   			case S_DFLT_STATION:
			   				dbDflt.s_dflt_station = *pShort;
			   			break;
			   			case S_DFLT_STATION_TYPE:
			   				dbDflt.s_dflt_station_type = *pShort;
			   			break;
			   			
			   			case S_DFLT_CHANNEL:
			   				dbDflt.s_dflt_channel = *pShort;
			   			break;
			   			
			   			case S_DAYS_STATION:
			   				dbDflt.s_days_station = *pShort;
			   			break;
			   			case S_DAYS_STATION_TYPE:
			   				dbDflt.s_days_station_type = *pShort;
			   			break;

			   			case S_INTEGRITY_STATION:
			   				dbDflt.s_integrity_station = *pShort;
			   			break;
			   			case S_INTEGRITY_STATION_TYPE:
			   				dbDflt.s_integrity_station_type = *pShort;
			   			break;
						case S_INTEGRITY_TOLERANCE:
	   						dbDflt.s_integrity_tolerance = *pShort;
	   					break;

			   			case S_CHAN_EVENT_SRCH_WIDTH:
			   				dbDflt.s_chan_event_srch_width = *pShort;
			   			break;
			   			case S_CHAN_EVENT_SRCH_ALG:
			   				dbDflt.s_chan_event_srch_alg = *pShort;
			   			break;

						case S_CHAN_EVENT_SUMM_SORT:
			   				dbDflt.s_chan_event_summ_sort = *pShort;
			   			break;
			   			case S_CHAN_EVENT_SUMM_TYPE:
			   				dbDflt.s_chan_event_summ_type = *pShort;
			   			break; 
			   			case S_CHAN_EVENT_SUMM_MIN:
			   				dbDflt.s_chan_event_summ_min = *pShort;
			   			break; 
			   			case S_CHAN_EVENT_SUMM_MAX:
			   				dbDflt.s_chan_event_summ_max = *pShort;
			   			break; 
			   			
			   			case S_RAD_EVENT_SRCH_PAIRS:
			   				dbDflt.s_rad_event_srch_pairs = *pShort;
			   			break;	

						case S_RAD_EVENT_SUMM_SORT:
			   				dbDflt.s_rad_event_summ_sort = *pShort ;
			   			break; 
			   			case S_RAD_EVENT_SUMM_TYPE:
			   				dbDflt.s_rad_event_summ_type = *pShort;
			   			break; 

			   			case S_MARK_SUMM_SORT:
			   				dbDflt.s_mark_summ_sort = *pShort ;
			   			break; 
			   			case S_MARK_SUMM_TYPE:
			   				dbDflt.s_mark_summ_type = *pShort;
			   			break; 

						case S_EXPORT_STATION:
	   						dbDflt.s_export_station = *pShort;
	   					break;
						case S_EXPORT_STATION_TYPE:
	   						dbDflt.s_export_station_type = *pShort;
	   					break;
	   					case S_EXPORT_MEASUREMENT_TYPE:
	   						dbDflt.s_export_measurement_type = *pShort;
	   					break; 

						case S_MEAS_SUMM_STATION:
	   						dbDflt.s_meas_summ_station = *pShort;
	   					break;
						case S_MEAS_SUMM_STATION_TYPE:
	   						dbDflt.s_meas_summ_station_type = *pShort;
	   					break;
	   					case S_MEAS_SUMM_TYPE:
	   						dbDflt.s_meas_summ_type = *pShort;
	   					break; 

	   					case S_MEAS_PARM_STATION:
	   						dbDflt.s_meas_parm_station = *pShort;
	   					break;
	   					case S_MEAS_PARM_STATION_TYPE:
	   						dbDflt.s_meas_parm_station_type = *pShort;
	   					break;

						case S_RAW_DATA_STATION:
	   						dbDflt.s_raw_data_station = *pShort;
	   					break;
						case S_RAW_DATA_STATION_TYPE:
	   						dbDflt.s_raw_data_station_type = *pShort;
	   					break;

		   			}
		   		}	
		   		db_stat = d_recwrite(&dbDflt, 1);
		   		if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
		   		db_stat = d_recread(&dbDflt, 1);
	   		}
	   		else {	
	   			InitDbDfltRec(&dbDflt);	/* first time writing so default all */		
				switch (lField) {      
		   			case S_DFLT_FACILITY: 
		   				dbDflt.s_dflt_facility = *pShort;
		   			break;
			   			
		   			case S_DFLT_STATION:
		   				dbDflt.s_dflt_station = *pShort;
		   			break;
		   			case S_DFLT_STATION_TYPE:
		   				dbDflt.s_dflt_station_type = *pShort;
		   			break;

		   			case S_DFLT_CHANNEL:
		   				dbDflt.s_dflt_channel = *pShort;
		   			break;

					case S_DAYS_STATION:
			   			dbDflt.s_days_station = *pShort;
			   		break;
			   		case S_DAYS_STATION_TYPE:
			   			dbDflt.s_days_station_type = *pShort;
			   		break;

			   		
			   		case S_INTEGRITY_STATION:
			   			dbDflt.s_integrity_station = *pShort;
			   		break;
			   		case S_INTEGRITY_STATION_TYPE:
			   			dbDflt.s_integrity_station_type = *pShort;
			   		break;
					case S_INTEGRITY_TOLERANCE:
	   					dbDflt.s_integrity_tolerance = *pShort;
	   				break;

					case S_CHAN_EVENT_SRCH_WIDTH:
		   				dbDflt.s_chan_event_srch_width = *pShort;
		   			break;
		   			case S_CHAN_EVENT_SRCH_ALG:
		   				dbDflt.s_chan_event_srch_alg = *pShort;
		   			break;

		   			case S_RAD_EVENT_SRCH_PAIRS:
		   				dbDflt.s_rad_event_srch_pairs = *pShort;
		   			break;	

					case S_CHAN_EVENT_SUMM_SORT:
		   				dbDflt.s_chan_event_summ_sort = *pShort;
		   			break;
		   			case S_CHAN_EVENT_SUMM_TYPE:
		   				dbDflt.s_chan_event_summ_type = *pShort;
		   			break; 
		   			case S_CHAN_EVENT_SUMM_MIN:
		   				dbDflt.s_chan_event_summ_min = *pShort;
		   			break; 
		   			case S_CHAN_EVENT_SUMM_MAX:
		   				dbDflt.s_chan_event_summ_max = *pShort;
		   			break; 

					case S_RAD_EVENT_SUMM_SORT:
		   				dbDflt.s_rad_event_summ_sort = *pShort;
		   			break;
		   			case S_RAD_EVENT_SUMM_TYPE:
		   				dbDflt.s_rad_event_summ_type = *pShort;
		   			break; 

	   				case S_MEAS_PARM_STATION:
	   					dbDflt.s_meas_parm_station = *pShort;
	   				break;
	   				case S_MEAS_PARM_STATION_TYPE:
	   					dbDflt.s_meas_parm_station_type = *pShort;
	   				break;

					case S_MARK_SUMM_SORT:
		   				dbDflt.s_mark_summ_sort = *pShort ;
		   			break; 
		   			case S_MARK_SUMM_TYPE:
		   				dbDflt.s_mark_summ_type = *pShort;
		   			break; 

					case S_EXPORT_STATION:
	   					dbDflt.s_export_station = *pShort;
	   				break;
					case S_EXPORT_STATION_TYPE:
	   					dbDflt.s_export_station_type = *pShort;
	   				break;
	   				case S_EXPORT_MEASUREMENT_TYPE:
	   					dbDflt.s_export_measurement_type = *pShort;
	   				break; 

					case S_MEAS_SUMM_STATION:
	   					dbDflt.s_meas_summ_station = *pShort;
	   				break;
					case S_MEAS_SUMM_STATION_TYPE:
	   					dbDflt.s_meas_summ_station_type = *pShort;
	   				break;
	   				case S_MEAS_SUMM_TYPE:
	   					dbDflt.s_meas_summ_type = *pShort;
	   				break; 

					case S_RAW_DATA_STATION:
	   					dbDflt.s_raw_data_station = *pShort;
	   				break;
					case S_RAW_DATA_STATION_TYPE:
	   					dbDflt.s_raw_data_station_type = *pShort;
	   				break;
				}  	
				db_stat = d_fillnew(DB_DFLT_REC, (char *)&dbDflt, 1);
				db_stat = d_connect(DEFAULT_SET, 1);
		    }
		break;    
   	}	
   	return(TRUE);
}   

// access a Boolean array field in the db_dflt_rec struct    
int AccessDefaultDatabase(short sAction, long lField, int iIndex, int *pbBool)
{

	int db_stat;                                 
	CGFmtStr msg;
	struct db_dflt_rec dbDflt;             
	long lMembers;
	
			   	    
  	/* if not a valid field, then quit */   	
   	if ((lField != B_INTEGRITY_FLAGS) && (lField != B_RAW_DATA_FLAGS)) return(uiINVALID_DFLT_DB_FIELD_ERR);
   	    
   	switch (sAction) {
        case READ:
   			db_stat = d_members(DEFAULT_SET, &lMembers, 1);		/* check if there is anything to read */
   			if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
   			if (lMembers > 0) {
				db_stat = d_findfm(DEFAULT_SET, 1);
				if (db_stat == S_OKAY) { /* record in db */
					db_stat = d_recread(&dbDflt, 1);
					if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
				} 
			}	
			if ((lMembers == 0) || (db_stat != S_OKAY)) {
				InitDbDfltRec(&dbDflt);	/* get the defaults into a dummy rec  */		
			}	
			switch (lField) { 
	   			case B_INTEGRITY_FLAGS: 
	   				*pbBool = dbDflt.b_integrity_flags[iIndex];
	   			break;
		   			
	   			case B_RAW_DATA_FLAGS:
	   				*pbBool = dbDflt.b_raw_data_flags[iIndex];
	   			break;
		   	}		
		break;   	

        case WRITE:
   			db_stat = d_members(DEFAULT_SET, &lMembers, 1);		/* check if there is anything to read */
   			if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
   			if (lMembers > 0) {
				db_stat = d_findfm(DEFAULT_SET, 1);
				if (db_stat == S_OKAY) {
					db_stat = d_recread(&dbDflt, 1);
					if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
					switch (lField) {
			   			case B_INTEGRITY_FLAGS: 
			   				dbDflt.b_integrity_flags[iIndex] = *pbBool;
			   			break;
			   			
			   			case B_RAW_DATA_FLAGS:
			   				dbDflt.b_raw_data_flags[iIndex] = *pbBool;
			   			break;
		   			}
		   		}	
	   			db_stat = d_recwrite(&dbDflt, 1);
	   			if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
				db_stat = d_recread(&dbDflt, 1);
	   		}
	   		else {	
	   			InitDbDfltRec(&dbDflt);	/* first time writing so default all */		
				switch (lField) {      
					case B_INTEGRITY_FLAGS: 
		   				dbDflt.b_integrity_flags[iIndex] = *pbBool;
		   			break;
		   			
		   			case B_RAW_DATA_FLAGS:
		   				dbDflt.b_raw_data_flags[iIndex] = *pbBool;
		   			break;
		   			
	            }  	
				db_stat = d_fillnew(DB_DFLT_REC, (char *)&dbDflt, 1);
				db_stat = d_connect(DEFAULT_SET, 1);
		    }
		break;             
		
	}	
   	return(TRUE);
}   


// initialize all values in the db_event_chan_dflt_rec that are read from the db
// Note: channel number, station number and station type must be supplied by caller
static void InitDbDfltChanRec(struct db_chan_event_dflt_rec *pStruct)
{
	pStruct->b_chan_event_srch_select = FALSE;
	pStruct->b_chan_event_summ_select = FALSE;
	pStruct->d_chan_event_srch_threshold = 0;
	pStruct->d_chan_event_srch_background = 0;
}	

// access the db_event_chan_dflt_rec in thedb
// Note: channel number, station number and station type must be supplied by caller
 int AccessDefaultDatabase(short sAction, long lField, struct db_chan_event_dflt_rec *pStruct)
{
 	int db_stat;
 	long lMembers;  

	struct db_chan_event_dflt_key dbKeyToFind;
	dbKeyToFind.s_chan_event_chan_num = pStruct->s_chan_event_chan_num;
	dbKeyToFind.s_chan_event_sta_num = pStruct->s_chan_event_sta_num;
	dbKeyToFind.s_chan_event_sta_type = pStruct->s_chan_event_sta_type;
     
   	switch (sAction) {
        case READ:
        	db_stat = d_members(CHAN_DEFAULT_SET, &lMembers, 1);		/* check if there is anything to read */
   			if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
   			if (lMembers > 0) {
				db_stat = d_findfm(CHAN_DEFAULT_SET, 1);
	        	if (db_stat == S_OKAY) {
					db_stat = d_keyfind(DB_CHAN_EVENT_DFLT_KEY, (char *)&dbKeyToFind, 1);
					if (db_stat == S_OKAY) { /* record in db */
						db_stat = d_recread(pStruct, 1);
						if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
					}
				}	
			}
			if ((db_stat != S_OKAY) || (lMembers == 0)) {	
				InitDbDfltChanRec(pStruct);	/* get the defaults into a dummy rec  */		
			}	
		break;   	

        case WRITE:
			struct db_chan_event_dflt_rec Temp;
			db_stat = d_keyfind(DB_CHAN_EVENT_DFLT_KEY, (char *)&dbKeyToFind, 1);
			if (db_stat == S_OKAY) { /* record in db */
				db_stat = d_recread(&Temp, 1);
				if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
				db_stat = d_recwrite(pStruct, 1);
				if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
				db_stat = d_recread(pStruct, 1);
			} 
			else {
				db_stat = d_fillnew(DB_CHAN_EVENT_DFLT_REC, (char *)pStruct, 1);
				db_stat = d_connect(CHAN_DEFAULT_SET, 1);
			}	
		break;    
   	}	
   	return(TRUE);
}   


// initialize all values in the db_dir_rec that are read from the db
static void InitDbDfltPairRec(struct db_dir_rec *pStruct)		/* get the defaults into a dummy rec  */		
{
	int i;
	
	for (i=0; i<MAX_DB_CHANS; i++) {
		pStruct->b_dir_select[i] = FALSE;
		pStruct->s_dir_srch_from_chan_num[i] = 0;
		pStruct->s_dir_srch_from_sta_num[i] = 0;
		pStruct->s_dir_srch_from_sta_type[i] = 1;
		pStruct->s_dir_srch_to_chan_num[i] = 0;
		pStruct->s_dir_srch_to_sta_num[i] = 0;
		pStruct->s_dir_srch_to_sta_type[i] = 1;
		pStruct->b_dir_srch_type[i] = 0;
		pStruct->s_dir_srch_direction[i] = 0;
		pStruct->s_dir_srch_location[i] = 0;
		pStruct->us_dir_srch_min_secs[i] = 0;
		pStruct->us_dir_srch_max_secs[i] = 200;
		pStruct->s_dir_srch_ratio[i] = 2;
	}	
}	
                    
// access the db_dir_rec in the db                    
int AccessDefaultDatabase(short sAction, long lField, struct db_dir_rec *pStruct)
{
	int db_stat;                                 
	long lMembers;

   	switch (sAction) {
        case READ:
   			db_stat = d_members(PAIR_DEFAULT_SET, &lMembers, 1);		/* check if there is anything to read */
			if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
   			if (lMembers > 0) {
				db_stat = d_findfm(PAIR_DEFAULT_SET, 1);
				if (db_stat == S_OKAY) {
					db_stat = d_recread(pStruct, 1);
				}
			}
			if ((lMembers == 0) || (db_stat != S_OKAY)) {
				InitDbDfltPairRec(pStruct);
			}	
			break;   	

		                    
        case WRITE:
        	// look for existing record and delete it
        	db_stat = d_findfm(PAIR_DEFAULT_SET, 1);
        	if (db_stat == S_OKAY) {
				db_stat = d_disdel(1);
			}
			// fill and connect the new record
			db_stat = d_fillnew(DB_DIR_REC,(char *)pStruct, 1);
		    if (db_stat == S_OKAY) {
    			db_stat = d_connect(PAIR_DEFAULT_SET, 1);
    		}	
    		if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
    		break;
   	}	
   	return(TRUE);
}   



// Routines to support the measurement parameters for each station (used with SR data)
// initialize all values in the db_sta_dflt_rec that are read from the db
// Note: station number and station type must be supplied by caller
static void InitDbDfltStaRec(struct db_sta_dflt_rec *pStruct)
{
	pStruct->b_meas_srch_select =  FALSE;
	pStruct->b_meas_make_new_status =  TRUE;
	pStruct->b_meas_low_bkg_check =  FALSE;
	pStruct->b_meas_high_bkg_check =  FALSE;
	pStruct->b_meas_term_on_moving =  FALSE;
	pStruct->b_meas_include_at_fail =  TRUE;
	pStruct->s_meas_analysis_type = 0;
	pStruct->b_export_select =  FALSE;
	pStruct->d_meas_low_bkg_thres = 1.0;
	pStruct->d_meas_high_bkg_thres = 100.0;
	pStruct->d_meas_empty_thres = 20.0;
	pStruct->d_meas_reals_thres = 2.0;
	pStruct->d_meas_totals_thres = 50.0;
	pStruct->d_meas_cf_thres = 0.100;
	pStruct->d_meas_cf_norm_thres = 1800.0;
	pStruct->d_meas_at_thres = 100.0;
	pStruct->d_meas_at_bias = 0.100;
	pStruct->d_meas_at_sigma = 3.0;
	pStruct->d_meas_min_len = 5;
	pStruct->d_meas_max_len = 20;
	pStruct->d_meas_gate_width = 64;
	pStruct->d_adjustment = 0;
	pStruct->d_slope = 1.0;
	pStruct->d_intercept = 0.0;
	pStruct->d_time = 0.0;
	pStruct->d_timeprime = 0.0;
	pStruct->b_adjust_time_select = FALSE;
	
}	

// Routine to access db_sta_dflt_rec in db
// Note:station number and station type must be supplied by caller 
int AccessDefaultDatabase(short sAction, long lField, struct db_sta_dflt_rec *pStruct)
{
 	int db_stat;
 	long lMembers;                               
   	    
   	switch (sAction) {
        case READ:
        	db_stat = d_members(STA_DEFAULT_SET, &lMembers, 1);		/* check if there is anything to read */
   			if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
   			if (lMembers > 0) {
				db_stat = d_findfm(STA_DEFAULT_SET, 1);
	        	if (db_stat == S_OKAY) {
					db_stat = d_keyfind(S_STA_DFLT_NUM_KEY, (char *)&(pStruct->s_sta_dflt_num_key), 1);
					if (db_stat == S_OKAY) { /* record in db */
						db_stat = d_recread(pStruct, 1);
						if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
						// handle cases where BOOLs don't get set to 1 or 0, I don't know why.
						if (pStruct->b_meas_srch_select != FALSE) pStruct->b_meas_srch_select = TRUE;
						if (pStruct->b_meas_make_new_status != FALSE) pStruct->b_meas_make_new_status = TRUE;
						if (pStruct->b_meas_low_bkg_check != FALSE) pStruct->b_meas_low_bkg_check = TRUE;
						if (pStruct->b_meas_high_bkg_check != FALSE) pStruct->b_meas_high_bkg_check = TRUE;
						if (pStruct->b_meas_term_on_moving != FALSE) pStruct->b_meas_term_on_moving = TRUE;
						if (pStruct->b_meas_include_at_fail != FALSE) pStruct->b_meas_include_at_fail = TRUE;
						if (pStruct->b_export_select != FALSE) pStruct->b_export_select = TRUE;
						if (pStruct->b_adjust_time_select != FALSE) pStruct->b_adjust_time_select = TRUE;
					}
				}	
			}
			if ((db_stat != S_OKAY) || (lMembers == 0)) {	
				InitDbDfltStaRec(pStruct);	/* get the defaults into a dummy rec  */		
			}	
		break;   	

        case WRITE:
			db_stat = d_keyfind(S_STA_DFLT_NUM_KEY, (char *)&(pStruct->s_sta_dflt_num_key), 1);
			if (db_stat == S_OKAY) { /* record in db */
				// handle cases where BOOLs don't get set to 1 or 0, I don't know why.
				if (pStruct->b_meas_srch_select != FALSE) pStruct->b_meas_srch_select = TRUE;
				if (pStruct->b_meas_make_new_status != FALSE) pStruct->b_meas_make_new_status = TRUE;
				if (pStruct->b_meas_low_bkg_check != FALSE) pStruct->b_meas_low_bkg_check = TRUE;
				if (pStruct->b_meas_high_bkg_check != FALSE) pStruct->b_meas_high_bkg_check = TRUE;
				if (pStruct->b_meas_term_on_moving != FALSE) pStruct->b_meas_term_on_moving = TRUE;
				if (pStruct->b_meas_include_at_fail != FALSE) pStruct->b_meas_include_at_fail = TRUE;
				if (pStruct->b_export_select != FALSE) pStruct->b_export_select = TRUE;
				if (pStruct->b_adjust_time_select != FALSE) pStruct->b_adjust_time_select = TRUE;
				db_stat = d_recwrite(pStruct, 1);
				if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
			} else {
				db_stat = d_fillnew(DB_STA_DFLT_REC, (char *)pStruct, 1);
				db_stat = d_connect(STA_DEFAULT_SET, 1);
			}	
		break;    
   	}	
   	return(TRUE);
}   



// Routines to support the graph parameters
void InitDbDfltGraphRec(struct db_graph_dflt_rec *pStruct)
{
	int i;

	strcpy(pStruct->sz_config_name_key, "default");
	pStruct->d_display_offset = 0.0;
	pStruct->b_zoom_x100 =  FALSE;
	pStruct->b_zoom_auto_scale =  TRUE;
	pStruct->b_log_y_axis =  FALSE;
	pStruct->b_chans_overlapped =  FALSE;
	pStruct->us_zoom_factor = 1;
	pStruct->us_view_period_index = 10;
	for (i=0; i<10; i++) {
		pStruct->b_trace_active[i] = FALSE;
		pStruct->us_trace_station[i] = 0;
		pStruct->us_trace_channel[i] = 0;
	}
	pStruct->b_trace_active[0] = TRUE;
}	

int AccessDefaultDatabase(short sAction, long lField, struct db_graph_dflt_rec *pStruct)
{
 	int db_stat;
 	long lMembers;      
	DB_ADDR dbAdr;
   	    
   	switch (sAction) 
	{
        case READ:
        	db_stat = d_members(GRAPH_DEFAULT_SET, &lMembers, 1);		/* check if there is anything to read */
   			if (db_stat != S_OKAY) 
				return(uiDB_BAD_ERR);

   			if (lMembers > 0) 
			{
				db_stat = d_findfm(GRAPH_DEFAULT_SET, 1);
	        	if (db_stat == S_OKAY) 
				{
					// look for the configuration matching the one specified
					db_stat = d_keyfind(SZ_CONFIG_NAME_KEY, (char *)&(pStruct->sz_config_name_key), 1);
					if (db_stat == S_OKAY)  // record in db
					{
						db_stat = d_recread(pStruct, 1);
						if (db_stat != S_OKAY) 
							return(uiDB_BAD_ERR);

						// handle cases where BOOLs don't get set to 1 or 0, I don't know why.
						if (pStruct->b_zoom_x100 != FALSE) pStruct->b_zoom_x100 = TRUE;
						if (pStruct->b_zoom_auto_scale != FALSE) pStruct->b_zoom_auto_scale = TRUE;
						if (pStruct->b_log_y_axis != FALSE) pStruct->b_log_y_axis = TRUE;
						if (pStruct->b_chans_overlapped != FALSE) pStruct->b_chans_overlapped = TRUE;
					}
					else 
					{
						// requested config set doesn't exist, return the default values
						InitDbDfltGraphRec(pStruct);
					}
				}	
			}
			if ((db_stat != S_OKAY) || (lMembers == 0)) {	
				InitDbDfltGraphRec(pStruct);	/* get the defaults into a dummy rec  */		
			}	
			// 08-Nov-2004	SFK	Limit the upper index value to those possible
			if (pStruct->us_view_period_index > 18) pStruct->us_view_period_index = 10;
			break;   	

        case WRITE:
			db_stat = d_keyfind(SZ_CONFIG_NAME_KEY, (char *)&(pStruct->sz_config_name_key), 1);
			if (db_stat == S_OKAY)  // record in db
			{
				// handle cases where BOOLs don't get set to 1 or 0, I don't know why.
				if (pStruct->b_zoom_x100 != FALSE) pStruct->b_zoom_x100 = TRUE;
				if (pStruct->b_zoom_auto_scale != FALSE) pStruct->b_zoom_auto_scale = TRUE;
				if (pStruct->b_log_y_axis != FALSE) pStruct->b_log_y_axis = TRUE;
				if (pStruct->b_chans_overlapped != FALSE) pStruct->b_chans_overlapped = TRUE;

				db_stat = d_recwrite(pStruct, 1);
				if (db_stat != S_OKAY) 
					return(uiDB_BAD_ERR);
			} 
			else 
			{
				db_stat = d_fillnew(DB_GRAPH_DFLT_REC, (char *)pStruct, 1);
				db_stat = d_connect(GRAPH_DEFAULT_SET, 1);
			}	
			break;    

        case DELETE_REC:
			d_crget(&dbAdr, 1);
			db_stat = d_keyfind(SZ_CONFIG_NAME_KEY, (char *)&(pStruct->sz_config_name_key), 1);
			d_crget(&dbAdr, 1);

			if (db_stat == S_OKAY)  // record in db
			{
				d_disdel(1);	// delete the current record in the default db
				d_crget(&dbAdr, 1);
			}	
			break;
			
		default:
			return (0);
   	}	
   	return(1);
}   


int CreateGraphDefaultNamesPicklist(char **pAdrs[])
{
    
    size_t reserved_memory = 0;  //pjm 20-Jan-06 fixed uninitialized variable complaint
    int db_stat;
    int i;  
    long lMembers;  
    struct db_graph_dflt_rec dbGraphRec; 
	int sNum;

   /* ------------------------------------------------------------------
    *	Find out how many graph setups  in db
    * ----------------------------------------------------------------*/
   	db_stat = d_members(GRAPH_DEFAULT_SET, &lMembers, 1);		/* check if there is anything to read */
	if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
	
	if (lMembers == 0) { // check for the case of a completely empty default database, create default rec
		int status = AccessDefaultDatabase(READ, GRAPH_DEFAULT_SET, &dbGraphRec);
		if (status) AccessDefaultDatabase(WRITE, GRAPH_DEFAULT_SET, &dbGraphRec);
	   	db_stat = d_members(GRAPH_DEFAULT_SET, &lMembers, 1);		/* check again */
		if (db_stat != S_OKAY) return(uiDB_BAD_ERR);
	}

   /* ------------------------------------------------------------------
    *	Is at least one graph setup, create the picklist
    * ----------------------------------------------------------------*/
	i = 0;
	if (lMembers > 0) {
	   /* ------------------------------------------------------------------
		*	Is at least one graph setup, create the picklist
		* ----------------------------------------------------------------*/
		sNum = gen_allocate_str_array(100, lMembers, reserved_memory, pAdrs);
		for (db_stat = d_findfm(GRAPH_DEFAULT_SET, 1); db_stat == S_OKAY;
			db_stat = d_findnm(GRAPH_DEFAULT_SET, 1)) {
    
			d_recread ((char *) &dbGraphRec, 1);
			strcpy ((*pAdrs)[i], dbGraphRec.sz_config_name_key);
			i++;
		}	
		(*pAdrs)[i] = (char *)0;
	}
	
    return(TRUE);
}


