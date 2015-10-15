///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbReadStruct.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

//19 Apr 2006 - pjm - changed code to reduce compiler warnings.

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		GetDataByStation - Replaces ReadDbDataStruct (2 versions)
//		ReadDbDataStructFloatRate
//		WriteDbDataStatus
//
///////////////////////////////////////////////////////////////////////////
#include "RAD.H"
#include "AdjustTime.h"
#include "BinaryImport.h"
#include "EOSSImport.h"
#include "DbDay.h"
#include "DbInterval.h"
#include "DbLimit.h"
#include "DbReadStruct.h"
#include "DbTime.h"
#include "FacilityConfig.h"
#include "GrandImport.h"
#include "VifmImport.h"
#include "TypeDefinitions.h"
#include "ComDataStructures.h"

extern short glsFacNum;
extern CDbDay glDbDay;
///////////////////////////////////////////////////////////////////////////
//	Name:	GetIntegerDataByStation	(returns WHOLE_LONG_DATA_PTs)
//
//	Description:
//	Read the specified number of data values from the Binary Com database
//	starting at dAdjStartTime.  This function returns binary data in its
//	native format.
//
//	Declaration:
//	int GetIntegerDataByStation(short sSta, DATE dAdjStartTime, DATE dAdjEndTime , unsigned long ulNumPts, unsigned long *pulNumPtsRead, struct WHOLE_LONG_DATA_PT *plData, DATE *pdAdjRealEndTime)
//
//	Input:	sSta			number of station want to get float data for
//			dAdjStartTime	start of search interval
//			dAdjEndTime		end of search interval
//			ulNumPts		number of points to read from db starting at dAdjStartTime
//			
//	Output: pulNumPtsRead	number of data points actually read
//			plData			array allocated by user where binary native data is stored where:
//							plData->lData[0] = node number
//							plData->lData[1] = station number
//							plData->lData[2] = state
//							plData->lData[1] = bit  (which bit the state applies to)
//			pdAdjRealEndTime adjusted time of the last data value in the array
//
//	Return: TRUE - number of expected data points read
//			error from GetStationRecord, com call, GetDbTimeRange
//			uiDB_BAD_ERR - unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  02-Jun-1995 SFK		Created from old read_other_data
//	08-Aug-2002	SFK		Changed to fully support DATE
//  24-Sep-2003 SFK		Version 3.0.3 Modified for new FacMgr
//////////////////////////////////////////////////////////////////
int GetIntegerDataByStation(
	struct IDStruct StaID, 
	DATE dAdjStartTime, 
	DATE dAdjEndTime , 
	unsigned long ulNumPts, 
	unsigned long *pulNumPtsRead, 
	struct WHOLE_LONG_DATA_PT *plData, 
	DATE *pdAdjRealEndTime)
{
	*pulNumPtsRead = 0;
    if (ulNumPts == 0) 
		return 1;

    struct db_sta_rec dbSta;
	// ------------------------------------------------------------------
	//	Based on station type do different things here
	// ----------------------------------------------------------------
	int status = pglFacCfg->GetStationRecord(glsFacNum, &StaID, &dbSta);

    if (status != 1) 
		return(status);    

	//  If the data type is Binary, then get data via the Binary Com
	if (dbSta.s_sta_type == BINARY_TYPE) 
	{
		*pulNumPtsRead = ulNumPts;
		status = pglBinaryImport->GetDataByStation(glsFacNum, StaID.sStaID, dAdjStartTime, dAdjEndTime, plData, pulNumPtsRead, pdAdjRealEndTime);
		return(status);
	}
	else if (dbSta.s_sta_type == EOSS_TYPE)
	{
		*pulNumPtsRead = ulNumPts;
		status = pglEOSSImport->GetDataByStation(glsFacNum, StaID.sStaID, dAdjStartTime, dAdjEndTime, plData, pulNumPtsRead, pdAdjRealEndTime);
		return(status);
	}
	return(status);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetFloatDataByStation	(returns WHOLE_FLOAT_DATA_PT1s)
//			Replaces ReadDbDataStruct
//
//	Description:
//	Read the specified number of data values starting at dAdjStartTime from
//	the Coms which support float data in the normal format (Grand and VIFM)
//	or from legacy float data in the general Rad dbVista db (MCA).
//
//	For data in the general Rad dbVista db, read the specified number of data
//	values from the DAY_TO_FLOAT_DATA_SET part of the database starting at 
//	dAdjStartTime.  As the values are stored in the user designated array,
//	their timestamps are adjusted.  The timestamp of the last point is also returned.
//	All timestamps going in and out of this function are assumed to be adjusted??
//
//	Declaration:
//	int GetFloatDataByStation(short sSta, DATE dAdjStartTime, DATE dAdjEndTime , unsigned long ulNumPts, unsigned long *pulNumPtsRead, struct WHOLE_FLOAT_DATA_PT1 *pfData, DATE *pdAdjRealEndTime)
//
//	Input:	sSta			number of station want to get float data for
//			dAdjStartTime	start of search interval
//			dAdjEndTime		end of search interval
//			ulNumPts		number of points to read from db starting at dAdjStartTime
//			
//	Output: pulNumPtsRead	number of data points actually read
//			pfData			array allocated by user where floats are stored
//			pdAdjRealEndTime adjusted time of the last data value in the array
//
//	Return: TRUE - number of expected data points read
//			error from GetStationRecord, com call, GetDbTimeRange
//			uiDB_BAD_ERR - unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  02-Jun-1995 SFK		Created from old read_other_data
//	08-Aug-2002	SFK		Changed to fully support DATE
//////////////////////////////////////////////////////////////////
int GetFloatDataByStation(
	struct IDStruct StaID, 
	DATE dAdjStartTime, 
	DATE dAdjEndTime , 
	unsigned long ulNumPts, 
	unsigned long *pulNumPtsRead, 
	struct WHOLE_FLOAT_DATA_PT1 *pfData, 
	DATE *pdAdjRealEndTime)
{
	*pulNumPtsRead = 0;
    if (ulNumPts == 0) 
		return 1;

    struct db_float_data_rec dbfData;   // read raw data into here
    DB_ADDR dba;
    
    int i;
    int db_stat, status;
    DATE dAdjCurrentDay, dAdjLastDay, dFirstDay;
    unsigned long j;
	struct db_sta_rec dbSta;
	BOOL bSameDay;
    
	// ------------------------------------------------------------------
	//	Based on station type do different things here
	// ----------------------------------------------------------------
	status = pglFacCfg->GetStationRecord(glsFacNum, &StaID, &dbSta);
    if (status != 1) 
		return(status);    
	
	// Customize for new COMs
	//  If the data type is Grand, then get data via the GRANDCom
	if (dbSta.s_sta_type == GRAND_TYPE) 
	{
		*pulNumPtsRead = ulNumPts;
		return pglGrandImport->GetDataByStation(glsFacNum, StaID.sStaID, dAdjStartTime, dAdjEndTime, pfData, pulNumPtsRead, pdAdjRealEndTime); // rad 3.00
		//return status;
	}
	//  If the data type is Vifm, then get data via the Vifm Com
	else if (dbSta.s_sta_type == BOT_TYPE) 
	{
		*pulNumPtsRead = ulNumPts;
		return pglVifmImport->GetDataByStation(glsFacNum, StaID.sStaID, dAdjStartTime, dAdjEndTime, pfData, pulNumPtsRead, pdAdjRealEndTime); // rad 3.00
	}

	// ------------------------------------------------------------------
	//	Find the last day in the db for this sSta, will use later
	// ----------------------------------------------------------------
	if (!GetDbTimeRange(StaID, &dFirstDay, &dAdjLastDay))	// 01-Nov-2004 SFK 
		return 0;

	if (dFirstDay < dAdjStartTime ) 
		dFirstDay = dAdjStartTime;		// 01-Nov-2004 SFK
    
	// ------------------------------------------------------------------
	//	Find the multiple record containing the starting time in the database.
	//	All calls into database records must use unadjusted times.
	// ----------------------------------------------------------------
	DATE dUnadjustStartTime = UnadjustTimestamp(StaID, dFirstDay);	// 01-Nov-2004 SFK
	status = FindDbDataRecContainingTime(StaID, dUnadjustStartTime, SEARCH_START, &dbfData);
    if (status != 1) return(status);
    
	// ------------------------------------------------------------------
	//	Get the correct current owner and member set relationships of
	//   the multiple record, so can scan through the records in the day later.
	// ----------------------------------------------------------------
   	if (d_crget(&dba, CURR_DB) != S_OKAY) 
		goto db_exit;
    if (d_findco(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) 
		goto db_exit;      
    if (d_crset(&dba, CURR_DB) != S_OKAY) 
		goto db_exit;
      
	// ------------------------------------------------------------------
	//	Look for the starting time within the multiple record.
	//  ----------------------------------------------------------------
    i = 0;
    while ((dbfData.d_fdata_time[i] < dUnadjustStartTime) && 
		(i < (RAW_DATA_RECS_PER_DB_RECORD-1)))
	{
		i++;
	}

	// ------------------------------------------------------------------
	//	Found starting time - transfer data into users struct array
	//	Data in database are stored in DATE but must be time adjusted as
	//	they are filled into the caller's array.
	// ----------------------------------------------------------------
    dAdjCurrentDay = dAdjStartTime;
    j = 0;
	DATE dTempTime;
	
    do 
	{
		do 
		{
			dTempTime = AdjustTimestamp(StaID, dbfData.d_fdata_time[i]);
			pfData[j].dJulianTime = dTempTime;
			*pdAdjRealEndTime = dTempTime;
        	pfData[j].ulStatus 		= dbfData.ul_fdata_status[i];
        	pfData[j].fData[0] 		= dbfData.f_data_chan1[i];
			pfData[j].fData[1]		= dbfData.f_data_chan2[i];
			pfData[j].fData[2]		= dbfData.f_data_chan3[i];
			pfData[j].fData[3]		= dbfData.f_data_chan4[i];
			pfData[j].fData[4]		= dbfData.f_data_chan5[i];
			pfData[j].fData[5]		= dbfData.f_data_chan6[i];
			pfData[j].fData[6]		= dbfData.f_data_chan7[i];
			pfData[j].dElapsedTime  = dbfData.d_fdata_etime[i];
	    	j++;
	    	i++;
		} while ((i<RAW_DATA_RECS_PER_DB_RECORD) && (dbfData.d_fdata_time[i] != 0.0) && (j != ulNumPts));
       
		// ------------------------------------------------------------------
	   	//   Move on to the next record in this day if it exists, if no
		//	more records in this day, then move on to the next day in the db.
		// ----------------------------------------------------------------*/
		if (j != ulNumPts) 
		{
	    	db_stat  = d_findnm(DAY_TO_FLOAT_DATA_SET, CURR_DB);

	       	if (db_stat == S_EOS) 
			{
				bSameDay = FALSE;
				do 
				{
		    		dAdjCurrentDay += 1.0;
		    		status = glDbDay.FindDayRecInMainDb(StaID.sStaID, dAdjCurrentDay);
					bSameDay = SameDay(dAdjCurrentDay, dAdjLastDay);	// 24-May-2005 use function for SameDay
				} while ((status == uiDAY_NOT_IN_DB_STATUS) && !bSameDay);
			
				if (status != 1) 
				{
					*pulNumPtsRead = j;
					return 1;
				}	
                                          
				if (d_setor(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) 
					goto db_exit;				                                          
				if (d_findfm(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) 
					goto db_exit;
	    	}
	        if (d_recread((char *)&dbfData, CURR_DB) != S_OKAY) 
				goto db_exit;

	     	i = 0;
		}
    } while (j != ulNumPts);
       
	*pulNumPtsRead = j;       
    return 1;       
    
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "GetDataByStation (Float)");	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}




///////////////////////////////////////////////////////////////////////////
//	Name:	GetDoubleDataByStation	(returns WHOLE_DOUBLE_DATA_PT1s)
//			Replaces ReadDbDataStruct
//
//	Description:
//	Read the specified number of data values starting at dAdjStartTime from
//	the Coms which support double data in the normal format (none)
//	or from legacy double data in the general Rad dbVista db (MCA)
//	or return daouble data for analysis from any data type in the COMs (GRAND,
//	Binary or VIFM)
//
//	Read the specified number of data values from the DAY_TO_DOUBLE_DATA_SET
//	part of the database starting at dAdjStartTime.  As the values are
//	stored in the user designated array, their timestamps are adjusted.
//	The timestamp of the last point is also returned.
//	All timestamps going in and out of this function are assumed to be adjusted.
//
//	Declaration:
//	int GetDoubleDataByStation(short sSta, DATE dAdjStartTime, DATE dAdjEndTime , unsigned long ulNumPts, unsigned long *pulNumPtsRead, struct WHOLE_DOUBLE_DATA_PT1 *pdData, DATE *pdAdjRealEndTime, bool bRates)
//
//	Input:	sSta			number of station want to get float data for
//			dAdjStartTime	start of search interval
//			dAdjEndTime		end of search interval
//			ulNumPts		number of points to read from db starting at dAdjStartTime
//			bRates			in the case of SR data, raw data is not rates and it is stored like this in db
//							so can send accurate data to INCC when needed.  Most internal Rad analysis 
//							uses rates so do the calculation to rates here for that.
//			
//	Output: pulNumPtsRead	number of data points actually read
//			pdData			array allocated by user where doubles are stored
//			pdAdjRealEndTime adjusted time of the last data value in the array
//
//	Return: TRUE - number of expected data points read
//			error from GetStationRecord, com call, GetDbTimeRange
//			uiDB_BAD_ERR - unexpected db error
//	
//  date    /	author	revision
//  -----------------	--------
//  02-Jun-1995 SFK		Created from old read_other_data
//	08-Aug-2002	SFK		Changed to fully support DATE
//	24-Mar-2003	SFK		Changed to return double data to all analysis routines.
//////////////////////////////////////////////////////////////////
int GetDoubleDataByStation(
	struct IDStruct StaID, 
	DATE dAdjStartTime, 
	DATE dAdjEndTime , 
	unsigned long ulNumPts, 
	unsigned long *pulNumPtsRead, 
	struct WHOLE_DOUBLE_DATA_PT1 *pdData, 
	DATE *pdAdjRealEndTime, 
	bool bRates)
{
    db_double_data_rec dbdData;   // read raw data into here
    DB_ADDR dba;
    
    int i;
    int db_stat, status;
    DATE dAdjCurrentDay, dAdjLastDay, dFirstDay;
    unsigned long j;
	BOOL bSameDay;
	struct db_sta_rec dbSta;

    *pulNumPtsRead = 0;
    if (ulNumPts == 0) 
		return(1);
    
	// ------------------------------------------------------------------
	//	Based on station type do different things here
	// ----------------------------------------------------------------
	status = pglFacCfg->GetStationRecord(glsFacNum, &StaID, &dbSta);
    if (status != 1) 
		return(status);    
	
	// Customize for new COMs

	//  If the data type is Grand, then get data via the GRANDCom
	if (dbSta.s_sta_type == GRAND_TYPE) 
	{
		*pulNumPtsRead = ulNumPts;
		status = pglGrandImport->GetDataByStation(glsFacNum, StaID.sStaID, dAdjStartTime, dAdjEndTime, pdData, pulNumPtsRead, pdAdjRealEndTime); // rad 3.00
		return(status);
	}
	//  If the data type is Vifm, then get data via the Vifm Com
	else if (dbSta.s_sta_type == BOT_TYPE) 
	{
		*pulNumPtsRead = ulNumPts;
		status = pglVifmImport->GetDataByStation(glsFacNum, StaID.sStaID, dAdjStartTime, dAdjEndTime, pdData, pulNumPtsRead, pdAdjRealEndTime); // rad 3.00
		return(status);
	}
	//  If the data type is Binary, then get data via the Binary Com
	else if (dbSta.s_sta_type == BINARY_TYPE) 
	{
		*pulNumPtsRead = ulNumPts;
		status = pglBinaryImport->GetDataByStation(glsFacNum, StaID.sStaID, dAdjStartTime, dAdjEndTime, pdData, pulNumPtsRead, pdAdjRealEndTime); // rad 3.00
		return(status);
	}
	//  If the data type is MCA, then read the float data and convert to doubles
	else if (dbSta.s_sta_type == MCA_TYPE) 
	{
		*pulNumPtsRead = ulNumPts;
		struct WHOLE_FLOAT_DATA_PT1 *pTemp;
		pTemp = (struct WHOLE_FLOAT_DATA_PT1 *)malloc((ulNumPts+1) * sizeof(struct WHOLE_FLOAT_DATA_PT1));
		status = GetFloatDataByStation(StaID, dAdjStartTime, dAdjEndTime , ulNumPts, pulNumPtsRead, pTemp, pdAdjRealEndTime);
		for (unsigned long i=0; i<*pulNumPtsRead; i++) { //pjm: fixed compiler complaint.
			double dTempTime = AdjustTimestamp(StaID, pTemp[i].dJulianTime);
			pdData[i].dJulianTime = dTempTime;
			pdData[i].ulStatus 		= pTemp[i].ulStatus;
        	pdData[i].dData[0] 		= pTemp[i].fData[0];
			pdData[i].dData[1]		= pTemp[i].fData[1];
			pdData[i].dData[2]		= pTemp[i].fData[2];
			pdData[i].dData[3]		= pTemp[i].fData[3];
			pdData[i].dData[4]		= pTemp[i].fData[4];
			pdData[i].dData[5]		= pTemp[i].fData[5];
			pdData[i].dData[6]		= pTemp[i].fData[6];
			pdData[i].dElapsedTime  = pTemp[i].dElapsedTime;
		}
	    return(status);
	}

	//Add EOSS instrument HN 3-27-08
	else if (dbSta.s_sta_type == EOSS_TYPE)
	{
		*pulNumPtsRead = ulNumPts;
		status = pglEOSSImport->GetDataByStation (glsFacNum,StaID.sStaID,dAdjStartTime,dAdjEndTime,pdData,pulNumPtsRead,pdAdjRealEndTime); 
		return (status);
	}
	// Have SR Data
	// ------------------------------------------------------------------
    //	Find the last day in the db for this sSta, will use later
    // ----------------------------------------------------------------*/
	BOOL bStatus = GetDbTimeRange(StaID, &dFirstDay, &dAdjLastDay);		// 01-Nov-2004 SFK
    if (bStatus != TRUE) 
		return(0);
    if (dFirstDay < dAdjStartTime ) 
		dFirstDay = dAdjStartTime;		// 01-Nov-2004 SFK

    // ------------------------------------------------------------------
    //	Find the multiple record containing the starting time in the database.
    // ----------------------------------------------------------------*/
	DATE dUnadjustStartTime = UnadjustTimestamp(StaID, dFirstDay);		// 01-Nov-2004 SFK
    status = FindDbDataRecContainingTime(StaID, dUnadjustStartTime, SEARCH_START, &dbdData);
    if (status != 1) 
		return(status);
    
    // ------------------------------------------------------------------
    //	Get the correct current owner and member set relationships of
    //   the multiple record, so can scan through the records in the day later.
    // ----------------------------------------------------------------*/
   	if (d_crget(&dba, CURR_DB) != S_OKAY) goto db_exit; 	// save address of data rec
    if (d_findco(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;      
    if (d_crset(&dba, CURR_DB) != S_OKAY) goto db_exit;		// restore address of data rec
    
    
    // ------------------------------------------------------------------
    //	look for the starting time within the multiple record
    // ----------------------------------------------------------------*/
    i = 0;
    while ((dbdData.d_ddata_time[i] < dUnadjustStartTime) && (i < (RAW_DATA_RECS_PER_DB_RECORD-1))) 
		i++;


    // ------------------------------------------------------------------
    //	Found starting time - transfer data into users struct array
	//	If caller has requested rates, then calculate the rates now.
    // ----------------------------------------------------------------*/
    dAdjCurrentDay = dAdjStartTime;
    j = 0;
	DATE dTempTime;

    do 
	{
		do 
		{
			dTempTime = AdjustTimestamp(StaID, dbdData.d_ddata_time[i]);
			pdData[j].dJulianTime = dTempTime;
			*pdAdjRealEndTime = dTempTime;
        	pdData[j].ulStatus 		= dbdData.ul_ddata_status[i];
        	pdData[j].dData[0] 		= dbdData.d_data_chan1[i];
			if (bRates) pdData[j].dData[0] = pdData[j].dData[0]/dbdData.d_ddata_etime[i];

			pdData[j].dData[1]		= dbdData.d_data_chan2[i];
			if (bRates) pdData[j].dData[1] = pdData[j].dData[1]/dbdData.d_ddata_etime[i];

			pdData[j].dData[2]		= dbdData.d_data_chan3[i];
			if (bRates) pdData[j].dData[2] = pdData[j].dData[2]/dbdData.d_ddata_etime[i];

			pdData[j].dData[3]		= dbdData.d_data_chan4[i];
			if (bRates) pdData[j].dData[3] = pdData[j].dData[3]/dbdData.d_ddata_etime[i];

			pdData[j].dData[4]		= dbdData.d_data_chan5[i];
			if (bRates) pdData[j].dData[4] = pdData[j].dData[4]/dbdData.d_ddata_etime[i];

			pdData[j].dData[5]		= dbdData.d_data_chan6[i];
			//if (bRates) pdData[j].dData[5] = pdData[j].dData[5]/dbdData.d_ddata_etime[i]; if (bRates) pdData[j].dData[6] = pdData[j].dData[6]/dbdData.d_ddata_etime[i];// 27-Jul-2005 SFK don't divide reals/totals ratio by time

			pdData[j].dData[6]		= dbdData.d_data_chan7[i];
			if (bRates) pdData[j].dData[6] = pdData[j].dData[6]/dbdData.d_ddata_etime[i];

			pdData[j].dElapsedTime  = dbdData.d_ddata_etime[i];
	    	j++;
	    	i++;
		} while ((i<RAW_DATA_RECS_PER_DB_RECORD) && (dbdData.d_ddata_time[i] != 0.0) && (j != ulNumPts));

       
		// ------------------------------------------------------------------
		//   Move on to the next record in this day if it exists, if no
		//	more records in this day, then move on to the next day in the db.
		// ----------------------------------------------------------------*/
		if (j != ulNumPts) 
		{
	    	db_stat  = d_findnm(DAY_TO_DOUBLE_DATA_SET, CURR_DB);

	       	if (db_stat == S_EOS) 
			{
				bSameDay = FALSE;
				do 
				{
		    		dAdjCurrentDay += 1.0;
		    		status = glDbDay.FindDayRecInMainDb(StaID.sStaID, dAdjCurrentDay);
					//if ((int)dAdjCurrentDay == (int)dAdjLastDay) bSameDay = TRUE;
					bSameDay = SameDay(dAdjCurrentDay, dAdjLastDay);	// 24-May-2005 use function for SameDay
				} while ((status == uiDAY_NOT_IN_DB_STATUS) && !bSameDay);
			
				if (status != 1) 
				{
					*pulNumPtsRead = j;
					return(1);
				}	
                                          
				if (d_setor(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;				                                          
				if (d_findfm(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
	    	}
	        if (d_recread((char *)&dbdData, CURR_DB) != S_OKAY) goto db_exit; // debug
	     	i = 0;
		}
    } while (j != ulNumPts);
       
	*pulNumPtsRead = j;       
    return(1);       
    
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "GetDataByStation (Double)");	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}



//===========================================================================
//
//  Name	 :  WriteDbDataStatus
//
//  Purpose	 :  Write the double data status associated with a time interval
//				into the database.  (Normally called after SR type status bits
//				have been set after analyzing the data record)
//
//  Return value :  error from find_day()
//		    error from find_time_in_db
//		    db_vista error
//		    TRUE
//
//  Special notes:
//
//
//  date    /	author	revision
//  -----------------	--------
//  02-Jun-1995 SFK		Created from old read_other_data
//  29-May-1998 SFK		Created from ReadDbDataStruct
//	08-Aug-2002	SFK		Changed to DATE
//
//===========================================================================*/
int WriteDbDataStatus(struct IDStruct StaID, DATE dAdjStartTime, unsigned long ulNumPts, struct WHOLE_DOUBLE_DATA_PT1 *pdData)
{

    struct db_double_data_rec dbdData;   // read raw data into here///
    DB_ADDR dba;
    
    int i;
    int db_stat, status;
    DATE dAdjCurrentDay, dAdjLastDay, dDum;
    unsigned long j;
	BOOL bSameDay;
    
    if (ulNumPts == 0) return(TRUE);
    
	// ------------------------------------------------------------------
	//	Find the last day in the db for this sSta, will use later
	// ----------------------------------------------------------------
	BOOL bStatus = GetDbTimeRange(StaID, &dDum, &dAdjLastDay);
    if (bStatus != TRUE) return(FALSE);
    
	// ------------------------------------------------------------------
	//	Find the multiple record containing the starting time in the database.
	// ----------------------------------------------------------------
	DATE dUnadjustStartTime = UnadjustTimestamp(StaID, dAdjStartTime);
   	status = FindDbDataRecContainingTime(StaID, dUnadjustStartTime, SEARCH_START, &dbdData);
    if (status != TRUE) return(status);
    
	// ------------------------------------------------------------------
	//	Get the correct current owner and member set relationships of
	//   the multiple record, so can scan through the records in the day later.
	// ----------------------------------------------------------------
   	if (d_crget(&dba, CURR_DB) != S_OKAY) goto db_exit; 	// save address of data rec
    if (d_findco(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;      
    if (d_crset(&dba, CURR_DB) != S_OKAY) goto db_exit;		// restore address of data rec
    
    
	// ------------------------------------------------------------------
	//	look for the starting time within the multiple record
	// ----------------------------------------------------------------
    i = 0;
    while ((dbdData.d_ddata_time[i] < dUnadjustStartTime) && (i < (RAW_DATA_RECS_PER_DB_RECORD-1))) {
		i++;
    }

	// ------------------------------------------------------------------
	//	Found starting time - transfer data from users struct array
	//	to update the status part of ulNumPts records
	// ----------------------------------------------------------------*/
    dAdjCurrentDay = dAdjStartTime;
    j = 0;

    do {
		do {
        	dbdData.ul_ddata_status[i] = pdData[j].ulStatus;
	    	j++;
	    	i++;
		} while ((i<RAW_DATA_RECS_PER_DB_RECORD) && (dbdData.d_ddata_time[i] != 0) && (j != ulNumPts));
		
	    if (d_recwrite(&dbdData, CURR_DB) != S_OKAY) goto db_exit; // write the updated record
       
		// ------------------------------------------------------------------
		//   Move on to the next record in this day if it exists, if no
		//	more records in this day, then move on to the next day in the db.
		// ----------------------------------------------------------------*/
		if (j != ulNumPts) {
	    	db_stat  = d_findnm(DAY_TO_DOUBLE_DATA_SET, CURR_DB);

	       	if (db_stat == S_EOS) {
				bSameDay = FALSE;
				do {
		    		dAdjCurrentDay += 1.0;
		    		status = glDbDay.FindDayRecInMainDb(StaID.sStaID, dAdjCurrentDay);
					//if ((int)dAdjCurrentDay == (int)dAdjLastDay) bSameDay = TRUE;
					bSameDay = SameDay(dAdjCurrentDay, dAdjLastDay);	// 24-May-2005 use function for SameDay
				} while ((status == uiDAY_NOT_IN_DB_STATUS) && !bSameDay);
			
				if (status != TRUE) return(TRUE);		// at end of db
                                          
				if (d_setor(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;				                                          
				if (d_findfm(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) goto db_exit;
	    	}
	        if (d_recread((char *)&dbdData, CURR_DB) != S_OKAY) goto db_exit; // debug
	     	i = 0;
		}
    } while (j != ulNumPts);
       
    return(TRUE);       
    
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "WriteDbDataStatus");	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}

