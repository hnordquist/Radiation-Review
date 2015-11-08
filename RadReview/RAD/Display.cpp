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
//	Copyright, 2003-2005, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : Display.cpp   
  
//Primary Author: Shirley Klosterbuer
//19 Apr 2006 - pjm - changed code to reduce compiler warnings.
///////////////////////////////////////////////////////////////////////////
//	Routines in this file are used by DlgIntegrity and DlgRawData.
//	This file contains the following functions:
//			DisplayRawData (by flags)
//	static	DisplayDoItByFlags
//			DisplayRawData (by bits)
//	static	DisplayDoItByBits
//			FormatRawDataPoint (long, float and double version)
//			PrintPointToTextWin (long, float and double version)
//			PrintPointToTextFile (long, float and double version)
//	static	PrintStatusBitsDefinitionsToWin
//	static	CloseTextWindow
///////////////////////////////////////////////////////////////////////////
#include "RAD.H"
#include <math.h>
#include "DbInterval.h"
#include "DbReadStruct.h"
#include "Display.h"
#include "PrintFacilityHeader.h"
#include "ProgressBars.h"
#include "TextFile.h"
#include "MyDateTime.h"
#include "TypeDefinitions.h"
#include <malloc.h>

extern short glsFacNum;
extern CMyDateTime glMyDateTime;   
                                                                                            
/* ------------------------------------------------------------------
 *  Special status conditions determined from bits for GRAND
 * ----------------------------------------------------------------*/
#define ALL_STAT       0    /* Display all points */
#define NO_AC_PWR_STAT 1    /* Points with no AC power present */
#define BATT_LOW_STAT  2    /* Points taken when battery was low */
#define LOW_RATE_STAT  3    /* Points with low count rate in chan. 2 */
#define RMS_STAT       4    /* Points with rms error in chan. 2 */
#define UNUSUAL_STAT   5    /* Point with any unusual status */

#define BATT_LOW_BIT  0x40  /* Battery low bit from GRAND status byte */
#define NO_AC_PWR_BIT 0x80  /* No AC power from GRAND status byte */
#define LOW_RATE_BIT  0x20  /* Low count rate in chan. 2 in GRAND status byte */
#define RMS_BIT       0x10  /* RMS error in chan 2 in GRAND status byte */
#define COLD_START_BIT 0x04
#define AUTHENTICATION_BIT 0x02
#define MII_BIT 0x01

#define SR_NO_AC_PWR_BIT 0x8000    /* ISR and JSR points with no AC power present */
#define SR_BATT_LOW_BIT  0x4000    /* ISR and JSR points with low battery present */

#define MCA_NO_AC_PWR_BIT 0x0800
#define MCA_BATT_LOW_BIT  0x0400

#define DUPLICATE_POINT_BIT 1		// from binary com
#define OUT_OF_ORDER_BIT 2			// from binary com


#define DISPLAY_ALLOC_PTS 2000

static RESOURCE_ID gllCancelBtnHit;
static CGUI_TextWin *gllpWin = NULL;     

static void PrintStatusBitsDefinitionsToWin(const struct db_sta_rec *pdbSta, CGUI_TextWin *pWin);
static void DisplayDoItByBits(const IDStruct StaID, const DATE dStart, const DATE dEnd, const BOOL bAllData, const BOOL pbBytes[], const BOOL bLogFile, const BOOL bChattyBits);
static void DisplayDoItByFlags(const struct IDStruct StaID, const DATE dStart, const DATE dEnd,  const struct DATA_DISPLAY_FLAGS *pFlags, const int iErrAllow);
static BOOL CloseTextWindow(CGUI_TextWin *pWinArg);

///////////////////////////////////////////////////////////////////////////
//	Name:	DisplayRawData  (By flag conditions)
//
//	Description:
//	Create the GUI text window for results display, print the facility header,
//	Based on the conditions requested (Batt low, etc), display any data points
//	that match the conditions for the station(s) requested.
//
//	Declaration:
//	int DisplayRawData(const short sStaNum, const DATE dStart, const DATE dEnd, const struct DATA_DISPLAY_FLAGS *pFlags, const int iErrAllow)
//
//	Input:	dStart	beginning timestamp of data to examine
//			dEnd	ending timestamp of data to examine
//			sStaNum	station to examine (-1 is all stations)
//			pFlags	array containing which flags to search for  (defined above)
//			iErrAllow	extra gap allowed when checking for missing data
//			
//	Output: none
//
//	Return: TRUE (successful), FALSE (couldn't execute), CANCELED (user hit cancel)
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//	10-Jun-2002	SFK		Changed to use DATE timebase
//	24-Sep-2003	SFK		Version 3.0.3 - adapted to new FacMgr
//////////////////////////////////////////////////////////////////
int DisplayRawData(
	const IDStruct StaID, 
	const DATE dStart, 
	const DATE dEnd, 
	const DATA_DISPLAY_FLAGS *pFlags, 
	const int iErrAllow)
{                  
   /* ------------------------------------------------------------------
    *	Open up the window where the raw data results will be displayed
    * ----------------------------------------------------------------*/
	if (!gllpWin) 
	{                
		gllpWin = new CGUI_TextWin("Data Matching Status Flag Conditions");

		if (gllpWin) 
			gllpWin->ShowStatusBar(TRUE);
		else
			exit(0);
	}   
	else 
	{
		gllpWin->Clear();
	}
	
    gllpWin->SetFocus();  
	gllpWin->EnableUserClose(CloseTextWindow);
    
	// print the common facility header
    if (!PrintFacilityHeader(gllpWin, glsFacNum, dStart, dEnd)) 
		return(0);
  
	// see if should do all stations or only a specified station
	if (StaID.sStaID == -1) 	// check all the stations at this facility
	{
		short sNumStas = pglFacCfg->GetNumStations(glsFacNum);

		if (sNumStas == 0) 
			return(0);

		if (sNumStas > 1) 
		{                                             
			IDStruct *pStaIDs = (IDStruct *) new IDStruct[sNumStas];

			int status = pglFacCfg->GetStationIDs(glsFacNum, pStaIDs);
			if (status != TRUE) 
				return(0); 

			for (short i = 0; i < sNumStas; i++) 
			{ 
				DisplayDoItByFlags(pStaIDs[i], dStart, dEnd, pFlags, iErrAllow);

				if (gllCancelBtnHit == IDCANCEL) 
					break;
			}	

			delete [] pStaIDs;
			pStaIDs = NULL;
		}
	}	
	else 
	{	
		DisplayDoItByFlags(StaID, dStart, dEnd, pFlags, iErrAllow);
	}
	if (gllCancelBtnHit == IDCANCEL) 
		return(CANCELED);  //CANCELED = 10

	return 1;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	DisplayDoItByFlags
//
//	Description:
//	Retrieve the specified data from the database.  Based on the conditions
//	requested (Batt low, etc), display any data points that match the conditions.
//
//	Declaration:
//	static void DisplayDoItByFlags(const short sStaNum, const DATE dStart, const DATE dEnd,  const struct DATA_DISPLAY_FLAGS *pFlags, const int iErrAllow)	
//
//	Input:	ulStart	beginning timestamp of data to examine
//			ulEnd	ending timestamp of data to examine
//			sStaNum	station to examine (-1 is all stations)
//			pFlags	array containing which flags to search for  (defined above)
//			iErrAllow	extra gap allowed when checking for missing data
//			
//	Output: none
//
//	Return: none
//	
//  date    /	author	revision
//  -----------------	--------
//  15-Aug-1995	SFK	Created (based on display.c of original Grand Review)
//	27-May-1998 SFK	Added support of ISR, 1K MCA
//	28-Sep-1998 SFK	Added support of 1K MCA status bits for no ac power and battery low
//  01-Oct-1998 SFK Split into two routines to handle looking by flags or by examining all possible bits
//	15-Feb-2002 SFK Removed GUIHuge stuff.  Changed to not get one more point than needed since return
//					the time of the next point to read.  Removed iPtsToProcess concept.  Version 2.10
//	10-Jun-2002	SFK		Changed to use DATE timebase
//	26-Mar-2008 HN	Added exception for EOSS data
//////////////////////////////////////////////////////////////////
static void DisplayDoItByFlags(
	struct IDStruct StaID, 
	const DATE dStart, 
	const DATE dEnd,  
	const struct DATA_DISPLAY_FLAGS *pFlags, 
	const int iErrAllow)	
{
	if (!gllpWin)
		return;

	WHOLE_FLOAT_DATA_PT1	*pfPts = NULL;             
	WHOLE_DOUBLE_DATA_PT1	*pdPts = NULL;            
	WHOLE_LONG_DATA_PT		*plPts = NULL;

    db_sta_rec dbSta;
    
    char szDate[DT_LEN+1], szTime[DT_LEN+1], szDate1[DT_LEN+1], szTime1[DT_LEN+1];
    int  status; 
    double dGap;
	DATE dTempStartTime;
    unsigned long ulNumPts;
    unsigned long ulNumPtsToSearch;
    unsigned long ulPtsRead;		/* pts to read from db */
    unsigned long ulNumPtsPrinted = 0;	/* number of pts output */
	DATE dJulianTime, dNextJulianTime;
	double dElapsedTime;
	unsigned long ulStatus;
    unsigned long ulNumAllocPts; 				/* pts were able to allocate in memory */
    unsigned long ulPtsToRead;					/* pts to read from db */
	unsigned long ulPtsReadFromDb = 0;				// actual number pts read from db 
    BOOL bPrintIt, bPrintNext;
    CGFmtStr Msg1, Msg;
    char ReasonStr[4];	/* reasons cannot exceed 3 chars */
    unsigned long  k;
	unsigned long ulPtNum;

	DATE dErrAllow = (double)iErrAllow/86400.0;		// convert to DATE units
	DATE dAllowance, dDum;

	BOOL bContinue = true;
	TwoLineProgressBar *pTwoLineProgressBar = NULL;


    
	//   Get station information that we'll need
	status = pglFacCfg->GetStationRecord(glsFacNum, &StaID, &dbSta);
	if (status != TRUE) 
	{ 
		gllpWin->EnableUserClose(CloseTextWindow); 
		return;
	}	

	// Don't do any of this if it is an EOSS instrument.  Hn 3-26-08
   /* ------------------------------------------------------------------
	*	Get the number of points in the day.  Have already checked
	*	before that there is data in the interval in dlginteg.cpp.
	*	But for choice of All Stations there may be no data for this station
	* ----------------------------------------------------------------*/
	if (StaID.sStaType == EOSS_TYPE)
	{
		gllpWin->Printf("\n\n---------------------------------------------------------------------\n");
		gllpWin->Printf("Station = %s\n", dbSta.sz_sta_name);
		gllpWin->Printf("\nEOSS data cannot be analyzed for data integrity conditions.");

	}
	else
	{
		status = GetDataStatistics(StaID, true, dStart, dEnd, &dTempStartTime, &dDum, &ulNumPts);
		if (status != TRUE) 	// had a db error
		{
			if (ulNumPts == 0) {
				gllpWin->Printf("\n\n---------------------------------------------------------------------\n");
				gllpWin->Printf("Station = %s\n", dbSta.sz_sta_name);
				gllpWin->Printf("\nNo data found for %s in specified time interval.", dbSta.sz_sta_name);
			}
			gllpWin->EnableUserClose(CloseTextWindow);
			return;
		}	
		ulNumPtsToSearch = ulNumPts;
		ulNumPtsPrinted = 0;
	
	   /* ------------------------------------------------------------------
		*	Figure out how many points you can allocate storage for --
		*	works best if only read and display about a 1000 pts at a time;
		*	check there's always enough memory for that allocation and do it now
		* ----------------------------------------------------------------*/
		if (ulNumPts > DISPLAY_ALLOC_PTS)
			ulNumAllocPts = DISPLAY_ALLOC_PTS;                 
		else
			ulNumAllocPts = ulNumPts;
		
		bool bOK = false;	

		switch(dbSta.s_data_type)
		{
			case FLOAT_TYPE:
				if (NULL != (pfPts = new struct WHOLE_FLOAT_DATA_PT1[ulNumAllocPts+1])) {bOK = true;} break;
			case DOUBLE_TYPE:
				if (NULL != (pdPts = new struct WHOLE_DOUBLE_DATA_PT1[ulNumAllocPts+1])) {bOK = true;} break;
			case INTEGER_TYPE:
				if (NULL != (plPts = new struct WHOLE_LONG_DATA_PT[ulNumAllocPts+1])) {bOK = true;} break;
		};

		if (!bOK)
		{
			RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
			gllpWin->EnableUserClose(CloseTextWindow);    
			return;
		}
	}
   
	if (StaID.sStaType != EOSS_TYPE)
	{
		/* ------------------------------------------------------------------
		*   Display station name
		* ----------------------------------------------------------------*/
		gllpWin->Printf("\n\n---------------------------------------------------------------------\n");
		gllpWin->Printf("Station = %s\n", dbSta.sz_sta_name);
			
	   /* ------------------------------------------------------------------
		*   Display key to deciphering data
		* ----------------------------------------------------------------*/
		gllpWin->Printf("\nData matching the following conditions displayed:\n");
		if (pFlags->bReadAll == TRUE) gllpWin->Printf       ("    All - All points\n");
		if (pFlags->bACPower == TRUE) gllpWin->Printf       ("    AC  - No AC power\n");
		if (pFlags->bBatteryLow == TRUE) gllpWin->Printf    ("    Low - Low Battery\n");
		if (pFlags->bColdStart == TRUE) gllpWin->Printf     ("    CS  - GRAND cold start\n");
		if (pFlags->bAuthentication == TRUE) gllpWin->Printf("    Ath - Authentication error\n");
		if (pFlags->bInMII == TRUE) gllpWin->Printf         ("    MII - In measurement interval of interest\n");                                                              
		if (pFlags->bOutOfOrder == TRUE) gllpWin->Printf    ("    Out - Out of chronological order\n");
		if (pFlags->bGaps == TRUE) gllpWin->Printf          ("    Gap - Possible missing data\n");

	   /* ------------------------------------------------------------------
		*   Print the status byte info and column headers for the various instrument types
		* ----------------------------------------------------------------*/
		PrintStatusBitsDefinitionsToWin(&dbSta, gllpWin);
		
		if (ulNumPts == 0) 
		{
			gllpWin->Printf  ("\n\nNo data found in requested interval."); 
			gllpWin->EnableUserClose(CloseTextWindow);    
			goto exit;
		}	

		ulPtNum = 0;
		Msg1.Printf("Processing station %s.\n", dbSta.sz_sta_name);
		Msg.Printf("Searched %ld out of %ld data points", ulPtNum, ulNumPtsToSearch);
		pTwoLineProgressBar = new TwoLineProgressBar(ulNumPtsToSearch, ulNumPtsToSearch/20, Msg1, Msg, "Radiation Review: DataSearch Progress");
		if (pTwoLineProgressBar) 
		{
    
		   /* ------------------------------------------------------------------
			*   Scan through the database displaying all data you find for
			*   the specified channel.
			* ----------------------------------------------------------------*/
			do 
			{
				k = 0;
			   /* ------------------------------------------------------------------
				*	Figure out how many points to read
				* ----------------------------------------------------------------*/
				if (ulNumPts > ulNumAllocPts)
					ulPtsToRead = ulNumAllocPts;
				else
					ulPtsToRead = ulNumPts;

				ulNumPts = ulNumPts - ulPtsToRead;

			   /* ------------------------------------------------------------------
				*	Read the points from the db  and get the parameters
				*	needed to compare for this point's data
				* ----------------------------------------------------------------*/
				DATE dTemp = dTempStartTime;		// need for potential error msg

				status = 0;

				switch (dbSta.s_data_type)
				{
					case FLOAT_TYPE:
						status = GetFloatDataByStation(StaID, dTempStartTime, dEnd, ulPtsToRead, &ulPtsRead, pfPts, &dTempStartTime);
						break;
					case DOUBLE_TYPE:
						status = GetDoubleDataByStation(StaID, dTempStartTime, dEnd, ulPtsToRead, &ulPtsRead, pdPts, &dTempStartTime, TRUE);
						break;
					case INTEGER_TYPE:
						status = GetIntegerDataByStation(StaID, dTempStartTime, dEnd, ulPtsToRead, &ulPtsRead, plPts, &dTempStartTime);
						break;
				};

				if (status != 1) 
				{
					glMyDateTime.DATETimestampToDateTimeStrs(dTemp, szDate, szTime);
					gllpWin->Printf("\n\nError during read of station data starting at %s %s ", szDate, szTime);
					goto exit;
				}
																						/*
																									if (dbSta.s_data_type == FLOAT_TYPE) 
																									{
																										status = GetFloatDataByStation(StaID, dTempStartTime, dEnd, ulPtsToRead, &ulPtsRead, pfPts, &dTempStartTime);
																										if (status != 1) 
																										{
																											glMyDateTime.DATETimestampToDateTimeStrs(dTemp, szDate, szTime);
																											gllpWin->Printf("\n\nUnexpected error during read of station data starting at %s %s ", szDate, szTime);
																											goto exit;
																										}
																									}    

																									if (dbSta.s_data_type == DOUBLE_TYPE) 
																									{
																										status = GetDoubleDataByStation(StaID, dTempStartTime, dEnd, ulPtsToRead, &ulPtsRead, pdPts, &dTempStartTime, TRUE);
																										if (status != 1) 
																										{
																											glMyDateTime.DATETimestampToDateTimeStrs(dTemp, szDate, szTime);
																											gllpWin->Printf("\n\nUnexpected error during read of station data starting at %s %s ", szDate, szTime);
																											goto exit;
																										}
																									}    

																									if (dbSta.s_data_type == INTEGER_TYPE) 
																									{
																										status = GetIntegerDataByStation(StaID, dTempStartTime, dEnd, ulPtsToRead, &ulPtsRead, plPts, &dTempStartTime);
																										if (status != 1) 
																										{
																											glMyDateTime.DATETimestampToDateTimeStrs(dTemp, szDate, szTime);
																											gllpWin->Printf("\n\nUnexpected error during read of station data starting at %s %s ", szDate, szTime);
																											goto exit;
																										}
																									}    
																						*/
				ulPtsReadFromDb += ulPtsRead;
		
			   /* ------------------------------------------------------------------
				*	Process this group of points.  Use the information
				*	about each point to decide whether to print it.
				* ----------------------------------------------------------------*/
				bPrintIt = FALSE;
				bPrintNext = FALSE;
				do 
				{
				   /* ------------------------------------------------------------------
					*	Determine conditions about this point
					* ----------------------------------------------------------------*/
					if (dbSta.s_data_type == FLOAT_TYPE) {
						ulStatus = pfPts[k].ulStatus;
						dJulianTime = pfPts[k].dJulianTime;
						if ((k+1) < (int)ulPtsRead) 
							dNextJulianTime = pfPts[k+1].dJulianTime;
						dElapsedTime = pfPts[k].dElapsedTime;
					}    
					else if (dbSta.s_data_type == DOUBLE_TYPE) {
						ulStatus = pdPts[k].ulStatus;
						dJulianTime = pdPts[k].dJulianTime;
						if ((k+1) < (int)ulPtsRead) 
							dNextJulianTime = pdPts[k+1].dJulianTime;
						dElapsedTime = pdPts[k].dElapsedTime;
					}    
					else if (dbSta.s_data_type == INTEGER_TYPE) {
						ulStatus = plPts[k].ulStatus;
						dJulianTime = plPts[k].dJulianTime;
						if ((k+1) < (int)ulPtsRead) 
							dNextJulianTime = plPts[k+1].dJulianTime;
						dElapsedTime = plPts[k].dElapsedTime;
					}    
					
					
					bPrintIt = FALSE;
					if (bPrintNext == TRUE) 
					{
			    		bPrintIt = TRUE;     
			    		strcpy(ReasonStr,"   ");
					}
					else 
					{	
			    		strcpy(ReasonStr,"");
					}
					
					bPrintNext = FALSE;
					
					
					if (pFlags->bReadAll == TRUE) 
					{
						bPrintIt = TRUE;
						strcpy(ReasonStr, "All");
					}	
					
					if (pFlags->bACPower == TRUE) {         
						if (dbSta.s_sta_type == GRAND_TYPE) {
							if ((ulStatus & NO_AC_PWR_BIT) != 0) {
								bPrintIt = TRUE;
								strcpy(ReasonStr, "AC ");
							}	
						}
						if ((dbSta.s_sta_type == JSR_TYPE) || (dbSta.s_sta_type == JSR1_TYPE) || (dbSta.s_sta_type == ISR_TYPE)) {
							if ((ulStatus & SR_NO_AC_PWR_BIT) != 0) {
								bPrintIt = TRUE;
								strcpy(ReasonStr, "AC ");
							}	
						}
						if (dbSta.s_sta_type == MCA_TYPE) {
							if ((ulStatus & MCA_NO_AC_PWR_BIT) != 0) {
								bPrintIt = TRUE;
								strcpy(ReasonStr, "AC ");
							}	
						}
					}
						
					if (pFlags->bBatteryLow == TRUE) {
						if (dbSta.s_sta_type == GRAND_TYPE) {
							if ((ulStatus & BATT_LOW_BIT) != 0) {
								bPrintIt = TRUE;
								strcpy(ReasonStr, "Low");
							}	
						}
						if ((dbSta.s_sta_type == JSR_TYPE) || (dbSta.s_sta_type == JSR1_TYPE) || (dbSta.s_sta_type == ISR_TYPE)) {
							if ((ulStatus & SR_BATT_LOW_BIT) != 0) {
								bPrintIt = TRUE;
								strcpy(ReasonStr, "AC ");
							}	
						}
						if (dbSta.s_sta_type == MCA_TYPE) {
							if ((ulStatus & MCA_BATT_LOW_BIT) != 0) {
								bPrintIt = TRUE;
								strcpy(ReasonStr, "Low");
							}	
						}
					}
					
					if (pFlags->bColdStart == TRUE) {
						if ((dbSta.s_sta_type == GRAND_TYPE) && ((ulStatus & COLD_START_BIT) != 0)) {
							bPrintIt = TRUE;
							strcpy(ReasonStr, "CS ");
						}	
					}                                            
					
					if (pFlags->bAuthentication == TRUE) {
						if ((dbSta.s_sta_type == GRAND_TYPE) && ((ulStatus & AUTHENTICATION_BIT) != 0)) {
							bPrintIt = TRUE;
							strcpy(ReasonStr, "Ath");
						}	
					}                                             
					
					if (pFlags->bInMII == TRUE) {
						if ((dbSta.s_sta_type == GRAND_TYPE) && ((ulStatus & MII_BIT) != 0)) {
							bPrintIt = TRUE;
							strcpy(ReasonStr, "MII");
						}	
					}                                  
					
					if (pFlags->bOutOfOrder == TRUE) { 
						if (dbSta.s_sta_type == BINARY_TYPE) {	// binary data are checked (and marked) for order during file read
							if ((plPts[k].ulStatus & OUT_OF_ORDER_BIT) == OUT_OF_ORDER_BIT) {	// was out of order during file read
								strcpy (ReasonStr, "Out");
								bPrintIt = TRUE;
							}
						}
						else {
							if ((dNextJulianTime < dJulianTime) && ((k+1) != ulPtsToRead)) {
								bPrintIt = TRUE;
								strcpy (ReasonStr, "Out");
							}
						}
					}	
							
					if (pFlags->bGaps == TRUE) 
					{
						if (dbSta.s_sta_type != BINARY_TYPE) 	// binary data has no concept of gaps
						{
							dAllowance = 1.1 * dElapsedTime/86400.0;

							if ((k+1) < (int)ulPtsRead)
							{
								if ((dNextJulianTime > (dJulianTime + (dAllowance + dErrAllow)) && ((k+1) != ulPtsToRead))) 
								{				
									bPrintIt = TRUE;
									bPrintNext = TRUE;
									double intervalUL = (dAllowance*86400.0 + dErrAllow);
									dGap = (dNextJulianTime - dJulianTime) * 86400.0;	// convert back to secs
									if (intervalUL <= 1.0)  // 1 second gap or less needs more displayed precision to make display intelligible
										gllpWin->Printf("\nGap:  Allowed time between next two points = %.3lf-%.3lfs, Actual time=%.3lfs", 
																					dElapsedTime, intervalUL, dGap);
									else
										gllpWin->Printf("\nGap:  Allowed time between next two points = %.1lf-%.1lfs, Actual time=%.1lfs", 
																					dElapsedTime, intervalUL, dGap);

									strcpy(ReasonStr, "   ");     
								}
							}
						}
					}	

				   /* ------------------------------------------------------------------
					*	If matches, print this point and the reason why
					* ----------------------------------------------------------------*/
					if (bPrintIt == TRUE) 
					{

						if (dbSta.s_data_type == FLOAT_TYPE)   
							PrintPointToTextWin(&dbSta, dJulianTime, ReasonStr, &pfPts[k], gllpWin,ulNumPtsPrinted); 
						else if (dbSta.s_data_type == DOUBLE_TYPE)  
							PrintPointToTextWin(&dbSta, dJulianTime, ReasonStr, &pdPts[k], gllpWin,ulNumPtsPrinted);
						else if (dbSta.s_data_type == INTEGER_TYPE) 
							PrintPointToTextWin(&dbSta, dJulianTime, ReasonStr, &plPts[k], gllpWin,ulNumPtsPrinted);

						ulNumPtsPrinted++;

					}	      
					
					k++;	// count this point

				   /* ------------------------------------------------------------------
					*	Update the progress bar
					* ----------------------------------------------------------------*/
					ulPtNum++;
					Msg.Printf("Searched %ld out of %ld data points", ulPtNum, ulNumPtsToSearch);
					bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg);

				} while ((k < (int)ulPtsRead) && (gllCancelBtnHit != IDCANCEL));

			} while ((ulNumPts > 0) && bContinue);
		}
exit: 
		if (pTwoLineProgressBar) 
		{
			pTwoLineProgressBar->Close();
			delete pTwoLineProgressBar;
			pTwoLineProgressBar = NULL;
		}
	}

   	if (StaID.sStaType != EOSS_TYPE)
	{
		gllpWin->Printf("\n\nMatching = %ld", ulNumPtsPrinted);

		glMyDateTime.DATETimestampToDateTimeStrs(dStart, szDate, szTime);
		gllpWin->Printf(  "\nInterval = %s %s to", szDate, szTime);

		glMyDateTime.DATETimestampToDateTimeStrs(dJulianTime, szDate1, szTime1);
		gllpWin->Printf(  " %s %s", szDate1, szTime1);    

		gllpWin->Printf(  "  (%ld points)", ulPtsReadFromDb);		//30-Jun-2005 Modified to print number actually read
		if (gllpWin != NULL) gllpWin->EnableUserClose(CloseTextWindow);    
	}
	if (pfPts)
		delete [] pfPts;
	if (pdPts)
		delete [] pdPts;
	if (plPts)
		delete [] plPts;
    return;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	DisplayRawData  (By bits)
//
//	Description:
//	Create the GUI text window for results display, print the facility header,
//	Based on the status bits requested,  display any data points
//	that match the conditions for the station(s) requested.
//
//	Declaration:
//	int DisplayRawData(const short sStaNum, const DATE dStart, const DATE dEnd, const BOOL bAllData, const BOOL *pbBytes, const BOOL bLogFile)
//
//	Input:	dStart		beginning timestamp of data to examine
//			dEnd		ending timestamp of data to examine
//			sStaNum		station to examine (-1 is all stations)
//			bAllData	print all data points
//			pbBytes		array containing which bits in status word must be set for match
//			bLogFile	whether to print results to a log file in addition to text window
//			
//	Output: none
//
//	Return: TRUE (successful), FALSE (couldn't execute), CANCELED (user hit cancel)
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//	10-Jun-2002	SFK		Changed to use DATE timebase
//////////////////////////////////////////////////////////////////
int DisplayRawData(
	const struct IDStruct StaID, 
	const DATE dStart, 
	const DATE dEnd, 
	const BOOL bAllData, 
	const BOOL *pbBytes, 
	const BOOL bLogFile,
	const BOOL bChattyBits)
{
                       
   /* ------------------------------------------------------------------
    *	Open up the window where the raw data results will be displayed
    * ----------------------------------------------------------------*/
	if (!gllpWin)
	{                
		gllpWin = new CGUI_TextWin("Data Matching Status Bit Conditions");

		if (gllpWin) 
			gllpWin->ShowStatusBar(TRUE);
		else
			exit(0);
	}   
	else 
	{
		gllpWin->Clear();
	}	
    gllpWin->SetFocus();    
	gllpWin->EnableUserClose(CloseTextWindow);    
    
	// print the common facility header
    if (!PrintFacilityHeader(gllpWin, glsFacNum, dStart, dEnd)) 
		return(0);

	DisplayDoItByBits(StaID, dStart, dEnd,  bAllData, pbBytes, bLogFile, bChattyBits);

	if (gllCancelBtnHit == IDCANCEL) 
		return(CANCELED);  //10

	return(1);
}	


///////////////////////////////////////////////////////////////////////////
//	Name:	DisplayDoItByBits
//
//	Description:
//				Retrieve the specified data from the database.  Based on the
//				status data bits requested (dx-d0), display any data points
//				that have a 1 in that bit.
//
//	Declaration:
//	static void DisplayDoItByBits(const short sStaNum, const DATE dStart, const DATE dEnd, const BOOL bAllData, const BOOL pbBytes[], const BOOL bLogFile)	
//
//	Input:	dStart		beginning timestamp of data to examine
//			dEnd		ending timestamp of data to examine
//			sStaNum		station to examine (-1 is all stations)
//			bAllData	print all data points
//			pbBytes		array containing which bits in status word must be set for match
//			bLogFile	whether to print results to a log file in addition to text window
//			
//	Output: none
//
//	Return: none
//	
//  date    /	author	revision
//  -----------------	--------
//  15-Aug-1995	SFK	Created (based on display.c of original Grand Review)
//	27-May-1998 SFK	Added support of ISR, 1K MCA
//	28-Sep-1998 SFK	Added support of 1K MCA status bits for no ac power and battery low
//  01-Oct-1998 SFK Split into two routines to handle looking by flags or by examining all possible bits
//	15-Feb-2002 SFK Removed GUIHuge stuff.  Changed to not get one more point than needed since return
//					the time of the next point to read.  Removed iPtsToProcess concept.  Version 2.10
//	10-Jun-2002	SFK		Changed to use DATE timebase
//////////////////////////////////////////////////////////////////
static void DisplayDoItByBits(
	IDStruct StaID, 
	const DATE dStart, 
	const DATE dEnd, 
	const BOOL bAllData, 
	const BOOL pbBytes[], 
	const BOOL bLogFile,
	const BOOL bChattyBits)	
{

	if (gllpWin == NULL)
		return;

	WHOLE_FLOAT_DATA_PT1  *pfPts = NULL;             
	WHOLE_DOUBLE_DATA_PT1 *pdPts = NULL;            
	WHOLE_LONG_DATA_PT	 *plPts = NULL;            
    db_sta_rec dbSta;
    
    char szDate[DT_LEN+1], szTime[DT_LEN+1], szDate1[DT_LEN+1], szTime1[DT_LEN+1];
    int  status; 
    DATE dTempStartTime;
    unsigned long ulNumPts;
    unsigned long ulNumPtsToSearch;
    unsigned long ulPtsRead;		/* pts to read from db */
    unsigned long ulNumPtsPrinted = 0;	/* number of pts output */
	DATE dJulianTime, dNextJulianTime;
	unsigned long ulStatus, ulStatusBitMask;
    unsigned long ulNumAllocPts; 				/* pts were able to allocate in memory */
    unsigned long ulPtsToRead;					/* pts to read from db */
	unsigned long ulPtsReadFromDb = 0;				// actual number pts read from db 
    BOOL bPrintIt = TRUE;
    CGFmtStr Msg1, Msg;
    int  i;
	unsigned long k;
	unsigned long ulPtNum;
	int iNumPrinted;
	TextFile LogFile;
	BOOL bLocalLogFile = bLogFile;
	// HN 4-9-08 We need this date to keep track of the last date of EOSS data for printing
	DATE dLastJulianTime = 0;

	BOOL bContinue = true;
	TwoLineProgressBar *pTwoLineProgressBar = NULL;
                   
	if (bLocalLogFile) 
	{
		BOOL bSuccess = LogFile.OpenTextFile("RawDataLogFile.txt", TRUE);
		if (LogFile.GetCancelStatus()) 
		{
			bLocalLogFile = FALSE;
		}
		else 
		{
			if (!LogFile.GetOpenStatus()) 
			{
				RadReviewMsg(uiFILE_WRITE_ERR, LogFile.GetFilename());
    			bLocalLogFile = FALSE;	
    		}	
		}
	}

   /* ------------------------------------------------------------------
	*   Determine station type
	* ----------------------------------------------------------------*/
	status = pglFacCfg->GetStationRecord(glsFacNum, &StaID, &dbSta);
	if (status != TRUE)
	{
		gllpWin->EnableUserClose(CloseTextWindow);    
		goto exit;
	}	

   /* ------------------------------------------------------------------
	*	Get the number of points in the day.  Have already checked
	*	before that there is data in the interval in dlginteg.cpp.
	* ----------------------------------------------------------------*/
	DATE dDum;
	status = GetDataStatistics(StaID, true, dStart, dEnd, &dDum, &dDum, &ulNumPts);
	if (status != TRUE) 	// had a db error
	{
		gllpWin->EnableUserClose(CloseTextWindow);    
		goto exit;
	}	
	
	//For EOSS, allocate more points for before/after HN 3-30-08
	if (dbSta.s_sta_type == EOSS_TYPE)
	{
	   int nNumChans = pglFacCfg->GetEnabledChansForStation (glsFacNum,(long)dbSta.s_sta_num_key,"EOSS");
	   ulNumPts += (2*nNumChans);
	}	
   
	ulNumPtsToSearch = ulNumPts;
	ulNumPtsPrinted = 0;
	
   /* ------------------------------------------------------------------
	*	Figure out how many points you need to allocate storage for --
	*	works best if only read and display about a 1000 pts at a time;
	*	there's always enough memory for that allocation.
	* ----------------------------------------------------------------*/
	if (ulNumPts > DISPLAY_ALLOC_PTS)
		ulNumAllocPts = DISPLAY_ALLOC_PTS;                 
	else
		ulNumAllocPts = ulNumPts;	
	
	if (dbSta.s_data_type == FLOAT_TYPE) 
	{
		pfPts = new struct WHOLE_FLOAT_DATA_PT1[ulNumAllocPts];
		if (pfPts == NULL) 
		{
			RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
			gllpWin->EnableUserClose(CloseTextWindow);    
			goto exit;
		}	
	}	

	else if (dbSta.s_data_type == DOUBLE_TYPE) {
		pdPts = new struct WHOLE_DOUBLE_DATA_PT1[ulNumAllocPts];
		if (pdPts == NULL) 
		{
			RadReviewMsg(uiMEMORY_ALLOCATION_ERR); 
			gllpWin->EnableUserClose(CloseTextWindow);    
			goto exit;
		}	
	}	

	else if (dbSta.s_data_type == INTEGER_TYPE) {
		plPts = new struct WHOLE_LONG_DATA_PT[ulNumAllocPts];
		if (plPts == NULL) 
		{
			RadReviewMsg(uiMEMORY_ALLOCATION_ERR);  
			gllpWin->EnableUserClose(CloseTextWindow);    
			goto exit;
		}	
	}	

	dTempStartTime = dStart;

   /* ------------------------------------------------------------------
	*   Display station name
	* ----------------------------------------------------------------*/
	gllpWin->Printf("\n\n---------------------------------------------------------------------\n");
	gllpWin->Printf("Station = %s\n", dbSta.sz_sta_name);
	
	//HN 4-8-08 #376, add status bit definition key here.
	/*if (dbSta.s_sta_type == GRAND_TYPE)
	{
		gllpWin->Printf ("Status bit definitions for Grand instruments\n");
		gllpWin->Printf ("d0 -- In MII              d1 -- No authentication         d2 -- Cold Start            d3 -- Taking offsets\n");
		gllpWin->Printf ("d4 -- OverThreshold       d5 -- Changing Signal           d6 -- Battery Low           d7 -- AC power loss\n");
		gllpWin->Printf ("d8 -- N/A                 d9 -- N/A                       d10-- N/A                   d11-- N/A\n");
		gllpWin->Printf ("d12-- N/A                 d13-- N/A                       d14-- N/A                   d15-- N/A\n");
		gllpWin->Printf ("d16-- Offset Taken        d17-- Invalid Data              d18-- Saturated Gammas      d19-- AC power loss\n");
		gllpWin->Printf ("d20-- N/A                 d21-- N/A                       d22-- N/A                   d23-- N/A\n");
		gllpWin->Printf ("d24-- N/A                 d25-- N/A                       d26-- N/A                   d27-- N/A\n");
		gllpWin->Printf ("d28-- N/A                 d29-- N/A                       d30-- N/A                   d31-- N/A\n");
	}
	else if (dbSta.s_sta_type == EOSS_TYPE)
	{
		gllpWin->Printf ("No status bit definitions exist for EOSS instruments\n");
	}
	else if (dbSta.s_sta_type == SR_TYPE || dbSta.s_sta_type == JSR_TYPE)
	{
		gllpWin->Printf ("Status bit definitions for shift registers\n");
		gllpWin->Printf ("d0 -- N/A                 d1 -- N/A                       d2 -- N/A                   d3 -- N/A\n");
		gllpWin->Printf ("d4 -- N/A                 d5 -- N/A                       d6 -- Battery Low (ISR)     d7 -- AC power loss (ISR)\n");
		gllpWin->Printf ("d8 -- N/A                 d9 -- N/A                       d10-- N/A                   d11-- N/A\n");
		gllpWin->Printf ("d12-- N/A                 d13-- N/A                       d14-- N/A                   d15-- N/A\n");
		gllpWin->Printf ("d16-- A/T Test failed     d17-- Low background            d18-- High background       d19-- Empty\n");
		gllpWin->Printf ("d20-- Assay               d21-- Cf Check                  d22-- Cf Normalization      d23-- Length short\n");
		gllpWin->Printf ("d24-- Length Long         d25-- Unusual                   d26-- First run             d27-- Totals Assay\n");
		gllpWin->Printf ("d28-- N/A                 d29-- N/A                       d30-- N/A                   d31-- N/A\n");
	}
	else if (dbSta.s_sta_type == BINARY_TYPE)
	{
		gllpWin->Printf ("Binary status bits represent individual binary data inputs.  See facility operator for more information\n");
	}
	else if (dbSta.s_sta_type == MCA_TYPE)
	{
		gllpWin->Printf ("Status bit definitions for shift registers\n");
		gllpWin->Printf ("d0 -- BBM error           d1 -- BBM error                 d2 -- BBM error             d3 -- Startup error\n");
		gllpWin->Printf ("d4 -- Startup error       d5 -- Startup error             d6 -- N/A                   d7 -- N/A\n");
		gllpWin->Printf ("d8 -- No filtering        d9 -- BBM error                 d10-- Battery low           d11-- AC power loss\n");
		gllpWin->Printf ("d12-- Spectrum changing   d13-- Restart required          d14-- N/A                   d15-- N/A\n");
		gllpWin->Printf ("d16-- U235 alarm          d17-- Cs137 alarm               d18-- U238 alarm            d19-- Gross count rate alarm\n");
		gllpWin->Printf ("d20-- Scalar alarm        d21-- N/A                       d22-- N/A                   d23-- N/A\n");
		gllpWin->Printf ("d24-- N/A                 d25-- N/A                       d26-- N/A                   d27-- N/A\n");
		gllpWin->Printf ("d28-- N/A                 d29-- N/A                       d30-- N/A                   d31-- N/A\n");
	}
	else if (dbSta.s_sta_type == BOT_TYPE || dbSta.s_sta_type == VIFM_TYPE)
	{
		gllpWin->Printf ("No status bit definitions exist for VIFM instruments\n");
	}*/
   /* ------------------------------------------------------------------
	*   Display key to deciphering data
	* ----------------------------------------------------------------*/
	if (bAllData) 
	{
		gllpWin->Printf("\nDisplay all data points.");
	}
	else 
	{
		iNumPrinted = 0;
		gllpWin->Printf("\nDisplay data with status bytes having the following bits set:\n");

		for (i=0; i<32; i++) 
		{
			if (pbBytes[i] == TRUE) 
			{
				gllpWin->Printf("  d%02d", i);
				iNumPrinted++;
			}
			if (iNumPrinted == 16) 
				gllpWin->Printf("\n");
		}
	}
	gllpWin->Printf("\n");
	PrintStatusBitsDefinitionsToWin(&dbSta, gllpWin);
	
	if (ulNumPts == 0) 
	{
	    gllpWin->Printf("\n\nNo data found in requested interval."); 
	    gllpWin->EnableUserClose(CloseTextWindow);    
	    goto exit;
	}	
	    	
	ulPtNum = 0;
	Msg1.Printf("Processing station %s.\n", dbSta.sz_sta_name);
	Msg.Printf("Searched %ld out of %ld data points", ulPtNum, ulNumPtsToSearch);
	pTwoLineProgressBar = new TwoLineProgressBar(ulNumPtsToSearch, ulNumPtsToSearch/20, Msg1, Msg, "Radiation Review: DataSearch Progress");

	if (pTwoLineProgressBar) 
	{	             
	   /* ------------------------------------------------------------------
		*   Scan through the database displaying all data you find for
		*   the specified channel.
		* ----------------------------------------------------------------*/
		do 
		{
			k = 0;
		   /* ------------------------------------------------------------------
			*	Figure out how many points to read
			* ----------------------------------------------------------------*/
			if (ulNumPts > ulNumAllocPts)
				ulPtsToRead = ulNumAllocPts;
			else
				ulPtsToRead = (int)ulNumPts;

			ulNumPts = ulNumPts - ulPtsToRead;

		   /* ------------------------------------------------------------------
			*	Read the points from the db  and get the parameters
			*	needed to compare for this point's data
			* ----------------------------------------------------------------*/
			if (dbSta.s_data_type == FLOAT_TYPE) {
				status = GetFloatDataByStation(StaID, dTempStartTime, dEnd, ulPtsToRead, &ulPtsRead, pfPts, &dTempStartTime);
				if (status != TRUE) goto exit;
			}    
			else if (dbSta.s_data_type == DOUBLE_TYPE) {
				status = GetDoubleDataByStation(StaID, dTempStartTime, dEnd, ulPtsToRead, &ulPtsRead, pdPts, &dTempStartTime, TRUE);
				if (status != TRUE) goto exit;
			}    
			else if (dbSta.s_data_type == INTEGER_TYPE) 
			{
				status = GetIntegerDataByStation(
					StaID, dTempStartTime, dEnd, 
					ulPtsToRead, &ulPtsRead, 
					plPts, &dTempStartTime);
					if (ulPtsRead == 0) 
					{
					    gllpWin->Printf("\n\nUnexpected lack of integer data, unable to continue."); 
					    gllpWin->EnableUserClose(CloseTextWindow); 
						goto exit; // catch error in EOSS COM
					}
					else if (status != TRUE) goto exit;
			}    

			ulPtsReadFromDb += ulPtsRead;
	
		   /* ------------------------------------------------------------------
			*	Process this group of points.  Use the information
			*	about each point to decide whether to print it.
			* ----------------------------------------------------------------*/
			do 
			{
				bPrintIt = FALSE;
				// determine conditions about this point
				if (dbSta.s_data_type == FLOAT_TYPE) {  //1
					ulStatus = pfPts[k].ulStatus;
					dJulianTime = pfPts[k].dJulianTime;
					if (k+1 < ulPtsRead) dNextJulianTime = pfPts[k+1].dJulianTime;
				}    
				else if (dbSta.s_data_type == DOUBLE_TYPE) {  //2
					ulStatus = pdPts[k].ulStatus;
					dJulianTime = pdPts[k].dJulianTime;
					if (k+1 < ulPtsRead) dNextJulianTime = pdPts[k+1].dJulianTime;
				}
				//Branch for EOSS type
				else if (dbSta.s_data_type == INTEGER_TYPE && dbSta.s_sta_type != EOSS_TYPE) 
				{
					ulStatus = plPts[k].ulStatus;
					dJulianTime = plPts[k].dJulianTime;
					if (k+1 < ulPtsRead) dNextJulianTime = plPts[k+1].dJulianTime;
				}
				else if (dbSta.s_data_type == INTEGER_TYPE && dbSta.s_sta_type == EOSS_TYPE)
				{
					dJulianTime = plPts[k].dJulianTime;
					if (k+1 < ulPtsRead) dNextJulianTime = plPts[k+1].dJulianTime;
				}

				if (bAllData) 
				{
					bPrintIt = TRUE;
				}
				else if (dbSta.s_sta_type != EOSS_TYPE)
					// No status bit definitions for EOSS_TYPEs
				{
					ulStatusBitMask = 1;

					for (i=0; i<32; i++)  // look through all the bits to see if this point should be printed
					{
						if (pbBytes[i] == TRUE) 
						{
							// fix for Bob Parker by pjm on April 7, 2008
							if ((ulStatusBitMask & ulStatus) == ulStatusBitMask)
							{
								bPrintIt = TRUE;
								break;
							}
						}	
						ulStatusBitMask = ulStatusBitMask << 1;	//shift mask one to the left
					}
				}

			   if (dbSta.s_sta_type== EOSS_TYPE)
			   {
					bPrintIt = TRUE;
			   }
			   /* ------------------------------------------------------------------
				*	If matches, print this point
				* ----------------------------------------------------------------*/
				if (bPrintIt == TRUE)
				{

					if      (dbSta.s_data_type == FLOAT_TYPE)   
						PrintPointToTextWin(&dbSta, dJulianTime, "   ", &pfPts[k], gllpWin, ulNumPtsPrinted, bChattyBits);
					else if (dbSta.s_data_type == DOUBLE_TYPE)  
						PrintPointToTextWin(&dbSta, dJulianTime, "   ", &pdPts[k], gllpWin, ulNumPtsPrinted, bChattyBits);
					else if (dbSta.s_data_type == INTEGER_TYPE) 
						PrintPointToTextWin(&dbSta, dJulianTime, "   ", &plPts[k], gllpWin, ulNumPtsPrinted, bChattyBits);
					if (bLocalLogFile) 
					{
					if      (dbSta.s_data_type == FLOAT_TYPE)   
						PrintPointToTextFile(&dbSta, dJulianTime, "   ", &pfPts[k], &LogFile, ulNumPtsPrinted);
					else if (dbSta.s_data_type == DOUBLE_TYPE)  
						PrintPointToTextFile(&dbSta, dJulianTime, "   ", &pdPts[k], &LogFile, ulNumPtsPrinted);
					else if (dbSta.s_data_type == INTEGER_TYPE) 
						PrintPointToTextFile(&dbSta, dJulianTime, "   ", &plPts[k], &LogFile, ulNumPtsPrinted);
						}

					// Count the EOSS pt if it is real
					if (dJulianTime != 0)
						ulNumPtsPrinted++;
					//keep track of the last point in the interval for later printing.
					if (dJulianTime > dLastJulianTime)
						dLastJulianTime = dJulianTime;
				}	      
				
				k++;	// count this point

			   /* ------------------------------------------------------------------
				*	Update the progress bar
				* ----------------------------------------------------------------*/
				ulPtNum++;
				Msg.Printf("Searched %ld out of %ld data points", ulPtNum, ulNumPtsToSearch);
				bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg);

			} while ((k < ulPtsRead) && bContinue);
			
		} while ((ulNumPts > 0) && bContinue);


		pTwoLineProgressBar->Close();
		delete pTwoLineProgressBar;
		pTwoLineProgressBar = NULL;

	}

	gllpWin->Printf("\n\nMatching = %ld", ulNumPtsPrinted);

	glMyDateTime.DATETimestampToDateTimeStrs(dStart, szDate, szTime);
    gllpWin->Printf(  "\nInterval = %s %s to", szDate, szTime);

	// We have stored the date for EOSS instruments, print it at the bottom.
	if (dbSta.s_sta_type == EOSS_TYPE)
	{
		glMyDateTime.DATETimestampToDateTimeStrs(dLastJulianTime, szDate1, szTime1);
		gllpWin->Printf(  " %s %s", szDate1, szTime1);    
	}
	else
	{
		glMyDateTime.DATETimestampToDateTimeStrs(dJulianTime, szDate1, szTime1);
		gllpWin->Printf(  " %s %s", szDate1, szTime1);    
	}
	
    if (dbSta.s_sta_type != EOSS_TYPE)
		gllpWin->Printf(  "  (%ld points)", ulPtsReadFromDb);
	else
		gllpWin->Printf(  "  (%ld points)", ulNumPtsPrinted
		);
	gllpWin->EnableUserClose(CloseTextWindow);    

exit:
	delete [] pfPts;
	delete [] pdPts;    
	delete [] plPts;
    return;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	FormatRawDataPoint
//
//	Description:
//	Based on the instrument type, format the contents of a single raw data point
//	into a string for output.
//
//	There are three functions to support the different data types of float, double and long
//
//	Declaration:
//	void FormatRawDataPoint(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_LONG_DATA_PT *plPts, CGFmtStr *pFormattedString)
//	void FormatRawDataPoint(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_FLOAT_DATA_PT1 *pfPts, CGFmtStr *pFormattedString)
//	void FormatRawDataPoint(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_DOUBLE_DATA_PT1 *pdPts, CGFmtStr *pFormattedString)
//
//	Input:	pdbSta			station record information associated with data
//			dJulianTime	timestamp of data to be printed
//			pReasonStr		reason why the data point is being printed (optional)
//			pPts			raw data point (is either float, double or long in the three functions)
//			
//	Output: pFormattedString formatted data point string
//
//	Return: none
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//	10-Jun-2002	SFK		Changed to use DATE timebase
//  03-Mar-2003	SFK		Divided into different routines for the various generic data types
//////////////////////////////////////////////////////////////////
// supports long data
void FormatRawDataPoint(
	const struct db_sta_rec *pdbSta, 
	const DATE dJulianTime, 
	const char *pReasonStr, 
	const WHOLE_LONG_DATA_PT *pPts, 
	CGFmtStr *pFormattedString,
	const unsigned long)  //used only for BOT
{
	char szDate[DT_LEN+1], szTime[DT_LEN+1];

	if (!pFormattedString) 
		return;

	glMyDateTime.DATETimestampToDateTimeStrs(dJulianTime, szDate, szTime);

	switch (pdbSta->s_sta_type) 
	{
		case BINARY_TYPE:
			pFormattedString->Printf("\n%s %s %s %08x   %5.0lf      %4d     %4d        %d    %08x",
				pReasonStr, szDate, szTime, pPts->ulStatus, pPts->dElapsedTime, 
				pPts->lData[0], pPts->lData[1], pPts->lData[2], pPts->lData[3]); 
			break;
		case EOSS_TYPE:
			if (pPts->dJulianTime != 0)
			{
				pFormattedString->Printf("\n%s %s %s %4d",
					pReasonStr, szDate, szTime,  
					pPts->eossPt.nState ); 
			}
			else
			{
				pFormattedString->Printf ("\nPoint before/after interval not found for EOSS station %u", pPts->eossPt.sChannelID);
			}
			break;
		default:
			break;
	}
}

// supports double data
void FormatRawDataPoint(
	const struct db_sta_rec *pdbSta, 
	const DATE dJulianTime, 
	const char *pReasonStr, 
	const WHOLE_DOUBLE_DATA_PT1 *pPts, 
	CGFmtStr *pFormattedString,
	const unsigned long)  //used only for BOT
{
	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	double dElapsedTime;

	if (!pFormattedString) 
		return;

	// convert the date number to strings
	glMyDateTime.DATETimestampToDateTimeStrs(dJulianTime, szDate, szTime);

	switch (pdbSta->s_sta_type) 
	{

	case JSR_TYPE:
	case JSR1_TYPE:
	case ISR_TYPE:
		dElapsedTime = pPts->dElapsedTime;
		if (dElapsedTime == 0.0) 
			dElapsedTime = 1.0;
		if ((pPts->dData[0]>9999999999.0) || (pPts->dData[1]>9999999999.0) ||
			(pPts->dData[2]>9999999999.0) || (pPts->dData[3]>9999999999.0)) 
		{ 
			pFormattedString->Printf("\n%s %s %s %08x   %5.0lf   %10.3e %10.3e %10.3e %10.3e",
				pReasonStr, szDate, szTime, pPts->ulStatus, pPts->dElapsedTime, 
				pPts->dData[0], pPts->dData[1], 
				pPts->dData[2], pPts->dData[3]);
		}
		else
		{		
			pFormattedString->Printf("\n%s %s %s %08x   %5.0lf   %10.1f %10.1f %10.1f %10.1f",
			pReasonStr, szDate, szTime, pPts->ulStatus, pPts->dElapsedTime, 
			pPts->dData[0], pPts->dData[1], 
			pPts->dData[2], pPts->dData[3]);
		}	
		if (pdbSta->s_sta_type == ISR_TYPE)
		{
			if (pPts->dData[4]>9999999999.0)  
				pFormattedString->PrintfAppend(" %10.3e", pPts->dData[4]);
			else 		
				pFormattedString->PrintfAppend(" %10.1f", pPts->dData[4]);
		}
	break;

	default:
		break;
	}
}

// supports float data
// 31-Mar-2005 SFK Include ms in GRAND data printouts and subsecond elapsed times
void FormatRawDataPoint(
	const struct db_sta_rec *pdbSta, 
	const DATE dJulianTime, 
	const char *pReasonStr, 
	const WHOLE_FLOAT_DATA_PT1 *pPts, 
	CGFmtStr *pFormattedString,
	const unsigned long ulLineNum)
{
	if (!pFormattedString ) 
		return;

	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	int i = 0;
	double dSamplingInterval, dElapsedTime;

	glMyDateTime.DATETimestampToDateTimeStrs(dJulianTime, szDate, szTime);

	switch (pdbSta->s_sta_type) 
	{
	case GRAND_TYPE:
		glMyDateTime.DATETimestampToDateTimeStrs(dJulianTime, szDate, szTime, GEN_DTF_IAEA, GEN_DTF_HMSM);	// 31-Mar-2005 Include ms in printout
		pFormattedString->Printf("\n%s %s %s %08x   %7.1lf   %8.1f %8.1f %8.1f  %8.3e  %8.3e %7.2e %7.2e",
			pReasonStr, szDate, szTime, pPts->ulStatus, pPts->dElapsedTime, 
			pPts->fData[0], pPts->fData[1], pPts->fData[2], pPts->fData[3], pPts->fData[4],
			pPts->fData[5], pPts->fData[6]);
		break;

	case BOT_TYPE:
		{
			static DATE dPriorTime = 0.0f;
			DATE dElapsedTimeInSeconds;
			glMyDateTime.DATETimestampToDateTimeStrs(dJulianTime, szDate, szTime, GEN_DTF_IAEA, GEN_DTF_HMSM);
			dSamplingInterval = pPts->dElapsedTime;

			if (ulLineNum == 0)
			{
				dElapsedTime = 0.0f;
				dElapsedTimeInSeconds = 0.0f;
			}
			else
			{
				dElapsedTime = dJulianTime - dPriorTime;
				dElapsedTimeInSeconds = dElapsedTime * 24 * 60 * 60;
			}
			dPriorTime = dJulianTime;

			pFormattedString->Printf("\n%s %s %s %9.3lf %9.3lf", 
				pReasonStr, szDate, szTime, dSamplingInterval, dElapsedTimeInSeconds);

			for (i = 0; i <= 7; i++) 
				pFormattedString->PrintfAppend("%8d ", (unsigned int)pPts->fData[i]);

			break;
		}

	case MCA_TYPE:
		/*All 90.05.07 10:10:01 00000002 65535 1000000. 1000000. 1000000. 1000000. 1000000.   100.000  100.000*/
		pFormattedString->Printf("\n%s %s %s %08x   %5.0lf   %8.0f %8.0f %8.0f %8.0f  %8.0f  %7.3f %7.3f",
			pReasonStr, szDate, szTime, pPts->ulStatus, pPts->dElapsedTime, 
			pPts->fData[0], pPts->fData[1], pPts->fData[2],
			pPts->fData[3], pPts->fData[4], pPts->fData[5], pPts->fData[6]);
		break;

	default:
		break;
	}
}


///////////////////////////////////////////////////////////////////////////
//	Name:	PrintStatusDefs
//
//	Description:
//	Format and print a raw data point to a text window and update output page
//	There are three versions of the function to support float, double or long types of data
//
//	Declaration:
//	void PrintStatusDefs(const struct db_sta_rec *pdbSta, unsigned long ulStatus, CGUI_TextWin *pWin)
//
//	Input:	pdbSta			station record information associated with data
//			ulStatus		status word from raw data point
//			pWin			window to print results to
//			
//	Output: none
//
//	Return: none
//	
//  date      / 	author	revision
//  -----------------	--------
//	02/11/2009		JCD		Created
//////////////////////////////////////////////////////////////////
void PrintStatusDefs(const struct db_sta_rec *pdbSta, unsigned long ulStatus, CGUI_TextWin *pWin)
{
	static char szaSrStatusStrs[32][256];                                                                                                     
	static char szaMcaStatusStrs[32][256];                                                                                                     
	static char szaMgStatusStrs[32][256];   
	static bool bInit = false;
	if(!bInit)
	{
		strcpy(szaSrStatusStrs[0],"");
		strcpy(szaSrStatusStrs[1], "");
		strcpy(szaSrStatusStrs[2], "");
		strcpy(szaSrStatusStrs[3], "");
		strcpy(szaSrStatusStrs[4], "");
		strcpy(szaSrStatusStrs[5], "");
		strcpy(szaSrStatusStrs[6], "Battery Low (ISR only)");
		strcpy(szaSrStatusStrs[7], "No AC Power (ISR only)");
		strcpy(szaSrStatusStrs[8], "");
		strcpy(szaSrStatusStrs[9], "");
		strcpy(szaSrStatusStrs[10], "");
		strcpy(szaSrStatusStrs[11], "");
		strcpy(szaSrStatusStrs[12], "");
		strcpy(szaSrStatusStrs[13], "");
		strcpy(szaSrStatusStrs[14], "");
		strcpy(szaSrStatusStrs[15], "");
		strcpy(szaSrStatusStrs[16], "A/T test failed");
		strcpy(szaSrStatusStrs[17], "Low background");
		strcpy(szaSrStatusStrs[18], "High background");
		strcpy(szaSrStatusStrs[19], "Empty");
		strcpy(szaSrStatusStrs[20], "Verification");
		strcpy(szaSrStatusStrs[21], "Cf check source");
		strcpy(szaSrStatusStrs[22], "Cf normalization");
		strcpy(szaSrStatusStrs[23], "Length short");
		strcpy(szaSrStatusStrs[24], "Length long");
		strcpy(szaSrStatusStrs[25], "Unusual");
		strcpy(szaSrStatusStrs[26], "First run");
		strcpy(szaSrStatusStrs[27], "");
		strcpy(szaSrStatusStrs[28], "");
		strcpy(szaSrStatusStrs[29], "");
		strcpy(szaSrStatusStrs[30], "");
		strcpy(szaSrStatusStrs[31], "");

		strcpy(szaMcaStatusStrs[0], "BBM Err");
		strcpy(szaMcaStatusStrs[1], "BBM Err");
		strcpy(szaMcaStatusStrs[2], "BBM Err");
		strcpy(szaMcaStatusStrs[3], "Startup Err");
		strcpy(szaMcaStatusStrs[4], "Startup Err");
		strcpy(szaMcaStatusStrs[5], "Startup Err");
		strcpy(szaMcaStatusStrs[6], "");
		strcpy(szaMcaStatusStrs[7], "");
		strcpy(szaMcaStatusStrs[8], "No Filtering");
		strcpy(szaMcaStatusStrs[9], "BBM Error");
		strcpy(szaMcaStatusStrs[10], "Battery Low");
		strcpy(szaMcaStatusStrs[11], "No AC Power");
		strcpy(szaMcaStatusStrs[12], "Spec changing");
		strcpy(szaMcaStatusStrs[13], "Restart now");
		strcpy(szaMcaStatusStrs[14], "");
		strcpy(szaMcaStatusStrs[15], "");
		strcpy(szaMcaStatusStrs[16], "U235 count rate alarm");
		strcpy(szaMcaStatusStrs[17], "Cs 137 count rate alarm");
		strcpy(szaMcaStatusStrs[18], "U238 count rate alarm");
		strcpy(szaMcaStatusStrs[19], "Gross count rate alarm");
		strcpy(szaMcaStatusStrs[20], "Scalar alarm");
		strcpy(szaMcaStatusStrs[21], "");
		strcpy(szaMcaStatusStrs[22], "");
		strcpy(szaMcaStatusStrs[23], "");
		strcpy(szaMcaStatusStrs[24], "");
		strcpy(szaMcaStatusStrs[25], "");
		strcpy(szaMcaStatusStrs[26], "");
		strcpy(szaMcaStatusStrs[27], "");
		strcpy(szaMcaStatusStrs[28], "");
		strcpy(szaMcaStatusStrs[29], "");
		strcpy(szaMcaStatusStrs[30], "");
		strcpy(szaMcaStatusStrs[31], "");

		strcpy(szaMgStatusStrs[0], "In measurement interval of interest");
		strcpy(szaMgStatusStrs[1], "Failed Authentication");
		strcpy(szaMgStatusStrs[2], "Cold Start");
		strcpy(szaMgStatusStrs[3], "Taking Offsets");
		strcpy(szaMgStatusStrs[4], "Over Threshold");
		strcpy(szaMgStatusStrs[5], "Changing Signal");
		strcpy(szaMgStatusStrs[6], "Battery Low");
		strcpy(szaMgStatusStrs[7], "No AC Power");
		strcpy(szaMgStatusStrs[8], "");
		strcpy(szaMgStatusStrs[9], "");
		strcpy(szaMgStatusStrs[10], "");
		strcpy(szaMgStatusStrs[11], "");
		strcpy(szaMgStatusStrs[12], "");
		strcpy(szaMgStatusStrs[13], "");
		strcpy(szaMgStatusStrs[14], "");
		strcpy(szaMgStatusStrs[15], "");
		strcpy(szaMgStatusStrs[16], "Offset taken");
		strcpy(szaMgStatusStrs[17], "Invalid data");
		strcpy(szaMgStatusStrs[18], "Saturated gammas");
		strcpy(szaMgStatusStrs[19], "");
		strcpy(szaMgStatusStrs[20], "");
		strcpy(szaMgStatusStrs[21], "");
		strcpy(szaMgStatusStrs[22], "");
		strcpy(szaMgStatusStrs[23], "");
		strcpy(szaMgStatusStrs[24], "");
		strcpy(szaMgStatusStrs[25], "");
		strcpy(szaMgStatusStrs[26], "");
		strcpy(szaMgStatusStrs[27], "");
		strcpy(szaMgStatusStrs[28], "");
		strcpy(szaMgStatusStrs[29], "");
		strcpy(szaMgStatusStrs[30], "");
		strcpy(szaMgStatusStrs[31], "");
		bInit = true;
	}
	//loop over all possible bits, when one is found select the right string from
	//an array of status defs and print it
	unsigned long ulTargetBit = 1;
	for(int i=0; i<32; i++)
	{
		if(ulStatus & ulTargetBit)
		{
			CGFmtStr StrStatDesc;
			//load StrStatDesc from array per instrument type
			switch (pdbSta->s_sta_type) 
			{
			case JSR_TYPE:
			case JSR1_TYPE:
			case ISR_TYPE:
				StrStatDesc.Printf("\n         d%-2d: %s",i,szaSrStatusStrs[i]);
				break;
			case GRAND_TYPE:
				StrStatDesc.Printf("\n         d%-2d: %s",i,szaMgStatusStrs[i]);
				break;
			case MCA_TYPE:
				StrStatDesc.Printf("\n         d%-2d: %s",i,szaMcaStatusStrs[i]);
				break;
			}
			pWin->Printf("%s", StrStatDesc.Get_sz());
		}
		ulTargetBit<<=1;
	}
}

///////////////////////////////////////////////////////////////////////////
//	Name:	PrintPointToTextWin
//
//	Description:
//	Format and print a raw data point to a text window and update output page
//	There are three versions of the function to support float, double or long types of data
//
//	Declaration:
//	void PrintPointToTextWin(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_FLOAT_DATA_PT1 *pPts, CGUI_TextWin *pWin)
//	void PrintPointToTextWin(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_DOUBLE_DATA_PT1 *pPts, CGUI_TextWin *pWin)
//	void PrintPointToTextWin(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_LONG_DATA_PT *plPts, CGUI_TextWin *pWin)
//
//	Input:	pdbSta			station record information associated with data
//			dJulianTime	timestamp of data to be printed
//			pReasonStr		reason why the data point is being printed (optional)
//			pPts			raw data point (is either float, double or long in the three functions)
//			pWin			window to print results to
//			
//	Output: none
//
//	Return: none
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//	10-Jun-2002	SFK		Changed to use DATE timebase
//	04-Mar-2003	SFK		Made into three function to support different data types
//////////////////////////////////////////////////////////////////
// supports float data
void PrintPointToTextWin(
	const struct db_sta_rec *pdbSta, 
	const DATE dJulianTime, 
	const char *pReasonStr, 
	const WHOLE_FLOAT_DATA_PT1 *pPts, 
	CGUI_TextWin *pWin,
	const unsigned long ulLineNum,  //used only for VIFM
	const BOOL bChattyBits)
{
	CGFmtStr Str;

	FormatRawDataPoint(pdbSta, dJulianTime, pReasonStr, pPts, &Str, ulLineNum);
	//Add line before each date
	//pWin->Printf("\n________________________________________________________________________________");
	pWin->Printf("%s", Str.Get_sz());

	//display status bit descriptions after each record, if any
	if (bChattyBits) PrintStatusDefs(pdbSta, pPts->ulStatus,pWin);
	CGUI_App::ProcessEvents();	// update output page
}

// supports double data
void PrintPointToTextWin(
	const struct db_sta_rec *pdbSta, 
	const DATE dJulianTime, 
	const char *pReasonStr, 
	const WHOLE_DOUBLE_DATA_PT1 *pPts, 
	CGUI_TextWin *pWin,
	const unsigned long ulLineNumber,  //used only for BOT
	const BOOL bChattyBits)
{
	CGFmtStr Str;

	FormatRawDataPoint(pdbSta, dJulianTime, pReasonStr, pPts, &Str, ulLineNumber);
	//Add line before each date
	//pWin->Printf("\n________________________________________________________________________________");
	pWin->Printf("%s", Str.Get_sz());

	//display status bit descriptions after each record, if any
	if (bChattyBits) PrintStatusDefs(pdbSta, pPts->ulStatus,pWin);
	CGUI_App::ProcessEvents();	// update output page
}

// supports long data
void PrintPointToTextWin(
	const struct db_sta_rec *pdbSta, 
	const DATE dJulianTime, 
	const char *pReasonStr, 
	const WHOLE_LONG_DATA_PT *pPts, 
	CGUI_TextWin *pWin,
	const unsigned long ulLineNum, //used only for BOT
	const BOOL bChattyBits)
{
	CGFmtStr Str;
	FormatRawDataPoint(pdbSta, dJulianTime, pReasonStr, pPts, &Str, ulLineNum);
	//Add line before each date
	//pWin->Printf("\n________________________________________________________________________________");
	pWin->Printf("%s", Str.Get_sz());

	//display status bit descriptions after each record, if any
	if (bChattyBits) PrintStatusDefs(pdbSta, pPts->ulStatus,pWin);
	CGUI_App::ProcessEvents();	// update output page
}

///////////////////////////////////////////////////////////////////////////
//	Name:	PrintPointToTextFile
//
//	Description:
//	Format and print a raw data point to a data file with line numbers.
//
//	Declaration:
//	void PrintPointToTextFile(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_FLOAT_DATA_PT1 *pPts, TextFile *pFile, const unsigned long ulLineNum)
//	void PrintPointToTextFile(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_DOUBLE_DATA_PT1 *pPts, TextFile *pFile, const unsigned long ulLineNum)
//	void PrintPointToTextFile(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_LONG_DATA_PT *pPts, TextFile *pFile, const unsigned long ulLineNum)
//
//	Input:	pdbSta			station record information associated with data
//			dJulianTime		timestamp of data to be printed
//			pReasonStr		reason why the data point is being printed (optional)
//			pfPts			raw data point if data is float
//			pdPts			raw data point if data is double
//			TextFile		file handle to write to
//			ulLineNum		line number to print at beginning of line
//			
//	Output: none
//
//	Return: none
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//	10-Jun-2002	SFK		Changed to use DATE timebase
//	04-Mar-2003	SFK		Made into three function to support different data types
//////////////////////////////////////////////////////////////////
// supports float data
void PrintPointToTextFile(
	const struct db_sta_rec *pdbSta, 
	const DATE dJulianTime, 
	const char *pReasonStr, 
	const WHOLE_FLOAT_DATA_PT1 *pPts, 
	TextFile *pFile, 
	const unsigned long ulLineNum)
{
	CGFmtStr WholeLine, Str, Str1;
		
	WholeLine.Printf("\n%7ld %12.6lf", ulLineNum, dJulianTime);	// write a line number for readability + the julian time
	FormatRawDataPoint(pdbSta, dJulianTime, pReasonStr, pPts, &Str, ulLineNum); // get the formatted data point
	Str.Delete(0);  // remove the leading line feed

	WholeLine += Str;
	pFile->WriteLineToTextFile(WholeLine);
}

// supports double data
void PrintPointToTextFile(
	const struct db_sta_rec *pdbSta, 
	const DATE dJulianTime, 
	const char *pReasonStr, 
	const WHOLE_DOUBLE_DATA_PT1 *pPts, 
	TextFile *pFile, 
	const unsigned long ulLineNum)
{
	CGFmtStr WholeLine, Str, Str1;
		
	WholeLine.Printf("\n%7ld %12.6lf", ulLineNum, dJulianTime);	// write a line number for readability + the julian time
	FormatRawDataPoint(pdbSta, dJulianTime, pReasonStr, pPts, &Str, ulLineNum); // get the formatted data point
	Str.Delete(0);  // remove the leading line feed

	WholeLine += Str;
	pFile->WriteLineToTextFile(WholeLine);
}

// supports long data
void PrintPointToTextFile(
	const struct db_sta_rec *pdbSta, 
	const DATE dJulianTime, 
	const char *pReasonStr, 
	const WHOLE_LONG_DATA_PT *pPts, 
	TextFile *pFile, 
	const unsigned long ulLineNum)
{
	CGFmtStr WholeLine, Str, Str1;
		
	WholeLine.Printf("\n%7ld %12.6lf", ulLineNum, dJulianTime);	// write a line number for readability + the julian time
	FormatRawDataPoint(pdbSta, dJulianTime, pReasonStr, pPts, &Str, ulLineNum); // get the formatted data point
	Str.Delete(0);  // remove the leading line feed

	WholeLine += Str;
	pFile->WriteLineToTextFile(WholeLine);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	PrintStatusBitsDefinitionsToWin
//
//	Description:
//	Based on the station type, print the status byte defintions and the column headers
//	for the raw data to the text window
//
//	Declaration:
//	static void PrintStatusBitsDefinitionsToWin(const struct db_sta_rec *pdbSta, CGUI_TextWin *pWin)
//
//	Input:	pdbSta	station record information associated with data
//			pWin	window to print to
//			
//	Output: none
//
//	Return: none
//	
//  date    /	author	revision
//  -----------------	--------
//	????		SFK		Created
//	03-Mar-2003	SFK		Added binary type
//////////////////////////////////////////////////////////////////
void PrintStatusBitsDefinitionsToWin(const struct db_sta_rec *pdbSta, CGUI_TextWin *pWin)
{
	switch (pdbSta->s_sta_type) 
	{
		case GRAND_TYPE:
			//30-Jun-2005 SFK Added Taking Offsets and Data Adjusted text
			//25-Jan-2006 SFK Added text for d16, d17, d18  SCR00217
			pWin->Printf("\nGRAND Status Bits Definitions:\n");
			pWin->Printf("   d31:  d23:                 d15:   d7:No AC Power\n");
			pWin->Printf("   d30:  d22:                 d14:   d6:Battery Low\n");
			pWin->Printf("   d29:  d21:                 d13:   d5:Changing Signal\n");
			pWin->Printf("   d28:  d20:                 d12:   d4:Over Threshold\n");
			pWin->Printf("   d27:  d19:                 d11:   d3:Taking Offsets\n");
			pWin->Printf("   d26:  d18:Saturated gammas d10:   d2:Cold Start\n");
			pWin->Printf("   d25:  d17:Invalid data     d9 :   d1:Failed Authentication\n");
			pWin->Printf("   d24:  d16:Offset taken     d8 :   d0:In measurement interval of interest\n");

			pWin->Printf("\n\n      Date    Time          Stat     ETime      NeutA    NeutB    NeutC     Gam1        Gam2     Gam1Unc   Gam2Unc");
			pWin->Printf  ("\n                            Word     (sec)      cts/s    cts/s    cts/s ");
				    		/*All 90.05.07 10:10:01 00000002 65535 1000000. 1000000. 1000000. 1.000e+000  1.000e+000 1.00e+000 1.00e+000*/
			break;

		case MCA_TYPE:
			pWin->Printf("\nMCA Status Bits Definitions:\n");
			pWin->Printf("   d31:                d23:             d15:         d7:  \n");
			pWin->Printf("   d30:                d22:             d14:Alarm7   d6:  \n");
			pWin->Printf("   d29:Restart now     d21:Startup Err  d13:Alarm6   d5:  \n");
			pWin->Printf("   d28:Spec changing   d20:Startup Err  d12:Alarm5   d4:  \n");
			pWin->Printf("   d27:No AC Power     d19:Startup Err  d11:Alarm4   d3:  \n");
			pWin->Printf("   d26:Battery Low     d18:BBM Err      d10:Alarm3   d2:  \n");
			pWin->Printf("   d25:BBM Error       d17:BBM Err      d9 :Alarm2   d1:  \n");
			pWin->Printf("   d24:No Filtering    d16:BBM Err      d8 :Alarm1   d0:  \n");
//			Status bit definitions were incorrectly associated with the bits JCD 2/13/2009
/*			pWin->Printf("   d31:  d23:							d15:				d7:  \n");
			pWin->Printf("   d30:  d22:							d14:				d6:  \n");
			pWin->Printf("   d29:  d21:							d13:Restart now		d5:Startup error  \n");
			pWin->Printf("   d28:  d20:Scaler alarm				d12:Spec changing	d4:Startup error  \n");
			pWin->Printf("   d27:  d19:Gross count rate alarm	d11:No AC Power		d3:Startup error  \n");
			pWin->Printf("   d26:  d18:U238 count rate alarm	d10:Battery Low		d2:BBM error  \n");
			pWin->Printf("   d25:  d17:Cs 137 count rate alarm	d9 :BBM Error		d1:BBM error  \n");
			pWin->Printf("   d24:  d16:U235 count rate alarm	d8 :No Filtering	d0:BBM error  \n");*/

			pWin->Printf("\n\n      Date    Time      Stat     ETime      Chan1    Chan2    Chan3    Chan4     Chan5    Chan6   Chan7");
			pWin->Printf  ("\n                        Word     (sec)");
				    		/*All 90.05.07 10:10:01 00000002 65535 1000000. 1000000. 1000000. 1000000. 1000000.   100.000  100.000*/
			break;

		case JSR_TYPE:
		case JSR1_TYPE:
		case ISR_TYPE:
			pWin->Printf("\nSR Status Bits Definitions:\n");
			pWin->Printf("   d31:            d23:Length short       d15:   d7:No AC Power (ISR only)  \n");
			pWin->Printf("   d30:            d22:Cf normalization   d14:   d6:Battery Low (ISR only)  \n");
			pWin->Printf("   d29:            d21:Cf check source    d13:   d5:  \n");
			pWin->Printf("   d28:            d20:Verification       d12:   d4:  \n");
			pWin->Printf("   d27:            d19:Empty              d11:   d3:  \n");
			pWin->Printf("   d26:First run   d18:High background    d10:   d2:  \n");
			pWin->Printf("   d25:Unusual     d17:Low background     d9 :   d1:  \n");
			pWin->Printf("   d24:Length long d16:A/T test failed    d8 :   d0:  \n");

			pWin->Printf("\n\n       Date     Time    Stat     ETime      Totals     Reals   Accidental    Totals2    Totals3");
			pWin->Printf  ("\n                        Word     (sec)      (cts/s)    (cts/s) (cts/s)       (cts/s)    (cts/s)");
			pWin->Printf  ("\n                        (hex)");
				    			/*90.05.07 10:10:01  0002  65535    1.000e+000 1.000e+000 1.000e+000 1.000e+000*/
			break;

		case BOT_TYPE:
			pWin->Printf("\n\n    Date/Time                 Sampling Elapsed Data1    Data2    Data3    Data4    Data5    Data6    Data7    Data8");
			pWin->Printf  ("\n                              Interval Time    cts/s    cts/s    cts/s    cts/s    cts/s    cts/s    cts/s    cts/s");
			pWin->Printf  ("\n                              (sec)    (sec)");          		
			break;

		case BINARY_TYPE:
			pWin->Printf("No status bits defined");
			pWin->Printf("\n\n      Date    Time      Stat     ETime      Log    Station     State    Bit ");
			pWin->Printf  ("\n                        Word     (sec)      Node                       (hex)");
			break;
		
		default:
			break;
	}
	CGUI_App::ProcessEvents();	// update output page
}


//////////////////////////////////////////////////////////////////
//	Handler for keeping track of when user closes results window.
//////////////////////////////////////////////////////////////////
static BOOL CloseTextWindow(CGUI_TextWin *pWinArg)
{
	BOOL bCanClose = TRUE;
	gllpWin = NULL;
	return(bCanClose);
}               
       


