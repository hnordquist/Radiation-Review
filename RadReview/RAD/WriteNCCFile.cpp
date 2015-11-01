///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : WriteNCCFile.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//	WriteMeasToNCCFile - generates a *.ncc file for a single measurement
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "BuildFilename.h"
#include "DbReadStruct.h"
#include "DbInterval.h"
#include "FacilityConfig.h"
#include "RadInit.H"
#include "Utilities.h"
#include "MyDateTime.h"

extern RADInit *pRAD_Init;  
extern short glsFacNum; //KM
extern CMyDateTime glMyDateTime;

///////////////////////////////////////////////////////////////////////////
//	Name:	WriteMeasToNCCFile
//
//	Description:
//	If the event type is supported by INCC, write a *.ncc file containing the runs
//	in the event.
//	
//	Declaration:
//	int WriteMeasToNCCFile(struct db_event_rec *pdbMeas, CGStr *pszName)
//
//	Input:	pdbMeas		Event to write to a *.ncc file
//
//	Output:	pszName		Name of file generated
//
//	Return:	TRUE (successful write)/FALSE (problem during write)
//	
//  date    /	author	revision
//  -----------------	--------
//				SFK		Created
//	12-Aug-2002	SFK		Changed to DATE
// 13-Aug-2004	SFK		Changed to write file underneath facility directory from Fac Mgr
//////////////////////////////////////////////////////////////////
// 
int WriteMeasToNCCFile(struct db_event_rec *pdbMeas, char *pszName)
{	    	

	FILE *pHandle = NULL;
	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	struct INCC_HEADER hdr;
	struct INCC_MEASUREMENT rec;
	struct db_sta_rec dbSta;
	int iNumWritten;
	DATE dDum;
	unsigned long ulNumPtsRead;
	struct WHOLE_DOUBLE_DATA_PT1 * pdPts;
	int status;
	char szFileName[DOSNAME_LEN+1];
	BOOL bWrite = FALSE;
	struct IDStruct StaID;
	char szFacRoot[MAX_LINE_LEN+1];
	CGStr TempStr;

	// Check if have been requested to write results in this facility
//	if (!pRAD_Init->Get_WriteMeasurementResults()) {
//		RadReviewMsg(uiNO_NCC_FILE_REQUESTED_ERR); 
//		return(FALSE);
//	}
	// See if we really want to write these
	if (((pdbMeas->us_event_type == MEASUREMENT_EVENT) && (pdbMeas->us_event_type1 == ASSAY_MEAS))  ||
		((pdbMeas->us_event_type == MEASUREMENT_EVENT) && (pdbMeas->us_event_type1 == CF_NORM_MEAS)) ||
		((pdbMeas->us_event_type == MEASUREMENT_EVENT) && (pdbMeas->us_event_type1 == EMPTY_MEAS)) ||
		((pdbMeas->us_event_type == MEASUREMENT_EVENT) && (pdbMeas->us_event_type1 == TOTALS_MEAS)) ||
		(pdbMeas->us_event_type == CHANNEL_EVENT) ||	// 04-Jan-2005  Add channel events to what is written (MG data)
		((pdbMeas->us_event_type == BACKGROUND_EVENT) && (pdbMeas->us_event_type1 == NORMAL_BKG_MEAS))) {
		bWrite = TRUE;

	}
	else {
		return(FALSE); // don't write this type to NCC
	}

	// Get the station information
	StaID.sStaID = pdbMeas->us_event_sta;
	StaID.sStaType =pdbMeas->us_event_sta_type;
	pglFacCfg->GetStationID(glsFacNum, &StaID);		// fill in all fields
	pglFacCfg->GetStationRecord(glsFacNum, &StaID, &dbSta);
	
	// Build a filename based on the date and the special INCC directory
	BuildFilename(pdbMeas->d_event_beg_time_key, dbSta.s_sta_num_key, ".ncc", szFileName);

	// 08-13-2004	Changed to get facility root path from Facility Manager
	pglFacCfg->GetFacilityDirectory(glsFacNum, szFacRoot, MAX_LINE_LEN);	// get facility root
	
	MyStrnCpy(pszName, szFacRoot, MAX_LINE_LEN);
	TempStr = pRAD_Init->Get_NCC_DataPath();
	MyStrnCat(pszName,  TempStr.Get_sz(), (MAX_LINE_LEN -strlen(pszName)));
	if (((pRAD_Init->Get_NCC_DataPath())[strlen(pRAD_Init->Get_NCC_DataPath()) - 1]) != '\\')
		MyStrnCat(pszName, "\\", 1);
	MyStrnCat(pszName, szFileName, (MAX_LINE_LEN -strlen(pszName)));
	
	pHandle = fopen(pszName, "wb");
	if (pHandle == NULL) {
		RadReviewMsg(uiFILE_OPEN_ERR, pszName);
		return(FALSE);
	}
	// generate the header
	strcpy(hdr.FileTypeStr, "IREV");
	
	if ((pdbMeas->us_event_type == MEASUREMENT_EVENT) && (pdbMeas->us_event_type1 == ASSAY_MEAS)) strcpy(hdr.MeasTypeStr, "V");
	if ((pdbMeas->us_event_type == MEASUREMENT_EVENT) && (pdbMeas->us_event_type1 == EMPTY_MEAS)) strcpy(hdr.MeasTypeStr, "V");
	if ((pdbMeas->us_event_type == MEASUREMENT_EVENT) && (pdbMeas->us_event_type1 == CF_NORM_MEAS)) strcpy(hdr.MeasTypeStr, "N");
	if ((pdbMeas->us_event_type == MEASUREMENT_EVENT) && (pdbMeas->us_event_type1 == TOTALS_MEAS)) strcpy(hdr.MeasTypeStr, "V");
	if ((pdbMeas->us_event_type == BACKGROUND_EVENT) && (pdbMeas->us_event_type1 == NORMAL_BKG_MEAS)) strcpy(hdr.MeasTypeStr, "B");
	if (pdbMeas->us_event_type == CHANNEL_EVENT) strcpy(hdr.MeasTypeStr, "V");		// 04-Jan-2005  Channel events are verifications to INCC
	
	MyStrnCpyAndPad(hdr.StationStr, dbSta.sz_sta_name, 12, ' ' );

	MyStrnCpyAndPad(hdr.ItemIdStr, pdbMeas->sz_event_id, ID_LEN+1, ' ' );

	glMyDateTime.DATETimestampToDateTimeStrs(pdbMeas->d_event_beg_time_key, szDate, szTime);
	strcpy(hdr.DateStr, szDate);
	strcpy(hdr.TimeStr, szTime);

	hdr.sNumRuns = pdbMeas->s_num_runs;

	//write the header
	iNumWritten = fwrite(&hdr, sizeof(hdr), 1, pHandle);
	if (iNumWritten != 1) {
		RadReviewMsg(uiFILE_WRITE_ERR, pszName);
		if (pHandle != NULL) fclose(pHandle);
		return(FALSE);
	}

	// Now generate and write the data records associated with this measurement
	unsigned long ulNumPts;
	status = GetDataStatistics(StaID, true, pdbMeas->d_event_beg_time_key, pdbMeas->d_event_end_time, &dDum, &dDum, &ulNumPts);
	if (status != TRUE) return(FALSE);		
	if (ulNumPts == 0) {  // no data for the station
		RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, pszName);
		if (pHandle != NULL) fclose(pHandle);
		return(FALSE);
	}
	if (ulNumPts != (unsigned long)pdbMeas->s_num_runs)
	{
		ulNumPts = ulNumPts;// only to set a breakpoint
	}

	pdPts = new struct WHOLE_DOUBLE_DATA_PT1[pdbMeas->s_num_runs+1];
	if (pdPts == NULL) {
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR); 
		fclose(pHandle);
		return(FALSE);
	}	

	status = GetDoubleDataByStation(StaID, pdbMeas->d_event_beg_time_key, pdbMeas->d_event_end_time , pdbMeas->s_num_runs, &ulNumPtsRead, pdPts, &dDum, FALSE);
	if (status != TRUE) {
		RadReviewMsg(uiDB_BAD_ERR, "WriteMeasToNCCFile"); 
		if (pHandle != NULL) fclose(pHandle);
		delete [] pdPts;
		return(FALSE);
	}

	// format the data record for all points in the event interval
	unsigned long i;
	struct db_chan_rec dbChan;
	for (i=0; i<ulNumPtsRead; i++) {
		//Date.DATETimestampToDateTimeStrs(pdPts[i].dJulianTime, szDate, szTime);
		glMyDateTime.DATETimestampToDateTimeStrs(pdPts[i].dJulianTime, szDate, szTime);  //use the global
		strcpy(rec.DateStr, szDate);
		strcpy(rec.TimeStr, szTime);
		rec.sElapsedTime = (short)(pdPts[i].dElapsedTime);
		if (pdbMeas->us_event_type == CHANNEL_EVENT) {	// 04-Jan-2005 SFK for MG data write the value of the event channel
			StaID.sChanID = pdbMeas->us_event_chan;
			pglFacCfg->GetChannelRecord(glsFacNum, &StaID, &dbChan);
			rec.dTotals = pdPts[i].dData[dbChan.s_chan_offset-1] * rec.sElapsedTime;
			rec.dRealsPlusAccidentals = 0.0;
			rec.dAccidentals = 0.0;
			rec.dScaler1 = 0.0;
			rec.dScaler2 = 0.0;
		}
		else {
			rec.dTotals = pdPts[i].dData[0];
			rec.dRealsPlusAccidentals = pdPts[i].dData[1] + pdPts[i].dData[2];
			rec.dAccidentals = pdPts[i].dData[2];
			rec.dScaler1 = pdPts[i].dData[3];
			rec.dScaler2 = pdPts[i].dData[4];
		}
		rec.sNumMultiplicityRuns = 0;

		if (pdPts[i].dData[1] < 0.0){
			i = i;
		}

		iNumWritten = fwrite(&rec, sizeof(rec), 1, pHandle);
		if (iNumWritten != 1) {
			RadReviewMsg(uiFILE_WRITE_ERR, pszName);
			if (pHandle != NULL) fclose(pHandle);
			return(FALSE);
		}
	}

	if (pHandle != NULL) fclose(pHandle);
	delete [] pdPts;

	return(TRUE);

}