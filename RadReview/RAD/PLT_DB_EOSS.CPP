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
// PLT_DB_EOSS.cpp

#include <gui_lib.h>    
#include "plt_dlg.h"
#include "plt_db.h"
#include "RAD.H"
#include "DbInterval.h"
#include "DbReadArray.h"
#include "ProgressBars.h"   
#include "MyDateTime.h"
#include "EOSSImport.h"

extern short glsFacNum;
extern CMyDateTime glMyDateTime;

BOOL CPlotDB::ReadChannelDataForEOSS(
	UINT uiPL_ChanIndex,	// I 0-based picklist index.
	DATE dDspStartOffset,	// I Date of first data relative to mdDB_StartDate
	DATE dViewPeriodDays,	// I Time period of data desired.
	BOOL *pbAbortRead,		// IO Address of variable set true to stop read.
	double huge **ppdhaData,	// O ptr to array of data values.
	double huge **ppdhaTimestamps, // O ptr to array of dates.
	ULONG *pulNumDataPts,	// O ptr to # of data points in current interval.
	ULONG *pulNumEvents,	// O ptr to # of events in DB for current interval & chan.
	EVENTS1 **paEvents,		// O ptr to array of events.
	ULONG *pulNumMarks,		// O ptr to # of marks in DB for current interval & chan.
	MARKS1 **paulMarks,		// O ptr to array of marks.
	TwoLineProgressBar *pProgress,
	ULONG *pulNumPrevPts,	// IO ptr to number of points read before this read.
	double dFracBefore,		// I fraction completed before this read.
	double dFracAfter)		// I fraction completed after this read.
{
	ULONG ulNumPts;
	DATE dFirstDateThisViewPeriod;
	DATE dLastDateThisViewPeriod;

	const double dRETRIEVE_PERIOD = 110.0;  // try to read data from 110 days to speed things up

	struct db_sta_rec dbStaRec;
	struct IDStruct ChanID;

	bool bSuccess = false;
	int iStatus = 0;

	*pulNumDataPts = 0;
	*ppdhaData = NULL;
	*ppdhaTimestamps = NULL;
	*pulNumEvents = 0;
	*paEvents = NULL;
	*pulNumMarks = 0;
	*paulMarks = NULL;

	ULONG ulTotalPtsRead = 0L;
	ULONG ulIndexFinalFlagData;

	double *daTimestampsBuffer = NULL;
	double *daDataBuffer = NULL;

	//for the progress bar (move these down eventually)
	CGFmtStr Text, Text1;
	char szChanName[DB_NAME_LEN+1];
	double dDeltaFrac;// = dFracAfter - dFracBefore;
	double dFracThisRead;// = (double)ulTotalPtsRead / (double)ulNumPts;
	double dCurrentFrac;// = dFracBefore + dDeltaFrac * dFracThisRead;

	ULONG ulIndex = 0;
	int rawdex, actual = 0,i, maxPlotPoints;
	double *dTime = NULL,*dState = NULL;

	if (!mbValidDB)
		goto quit;

	if (!pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID))
		goto quit;

	if (!pglFacCfg->GetStationRecord(glsFacNum, &ChanID, &dbStaRec))
		goto quit;

	dFirstDateThisViewPeriod = dDspStartOffset + mdDB_StartDate;
	dLastDateThisViewPeriod = dFirstDateThisViewPeriod + dViewPeriodDays;

	//
	//  dLastDateThisViewPeriod is ALWAYS the x-value at the far right of the graph for EOSS.
	//  This enables the horizontal line to extend all the way to the right, even when there
	//  is no data in the database that goes that far.  This may be backed out if necessary
	//  by taking the comments off the following two lines of code.
	//
	//  PJM May 2, 2008
	//
	// don't retrieve data past the end of the db
	//if (dLastDateThisViewPeriod > mdDB_EndDate) 
	//	dLastDateThisViewPeriod = mdDB_EndDate;

	// add this check to eliminate problem when move slider to far right, fix better later
	if (dLastDateThisViewPeriod == dFirstDateThisViewPeriod) 
	{
		iStatus = 1;
		goto quit;
	}

	// Find out how many points are in the interval for the channel
	DATE dActualStart,dActualEnd;
	iStatus = GetDataStatistics(
		ChanID, FALSE, 
		dFirstDateThisViewPeriod, 
		dLastDateThisViewPeriod,
		&dActualStart, 
		&dActualEnd, 
		&ulNumPts);

	if (iStatus != 1)  //problem with the GetDataStatistics call
		goto quit;

	pglEOSSImport->GetDataStoreTimeRange(
		glsFacNum,
		ChanID.sStaID,
		FALSE,
		&dActualStart, 
		&dActualEnd);

	if (dActualEnd < dLastDateThisViewPeriod)
		dLastDateThisViewPeriod= dActualEnd;

		
	// Allocate temporary storage 
	daTimestampsBuffer = new double[ulNumPts + 2];
	daDataBuffer = new double[ulNumPts + 2];

	if ((daTimestampsBuffer == NULL) || 
		(daDataBuffer == NULL)) 
	{
		RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
		goto quit;
	}

	double dDum;
	iStatus = GetDataByChannel(
		ChanID, dFirstDateThisViewPeriod,
		dLastDateThisViewPeriod, ulNumPts,
		&ulTotalPtsRead, daTimestampsBuffer,
		daDataBuffer, &dDum);

	ulIndexFinalFlagData = 1 + ulTotalPtsRead;

	maxPlotPoints = 2 + (2 * ulTotalPtsRead);  //two ends plus up-down, side-side

	dTime = new double[maxPlotPoints];
	dState = new double[maxPlotPoints];

	actual = 0;		//number of plot points

	/*		|data earlier than interval|data within the interval|data later than interval|
	CASE 1: |            NO            |            NO          |        NO              |
	CASE 2: |            NO            |            NO          |        YES             |
	CASE 3: |            YES           |            No          |        No              |
	CASE 4: |            YES           |            NO          |        YES             |
	CASE 5: |            NO            |            YES         |        No              |
	CASE 6: |            NO            |            YES         |        YES             |
	CASE 7: |            YES           |            YES         |        NO              |
	CASE 8: |            YES           |            YES         |        YES             |
	*/
	/////////////////////////////////////////////////////////////////////////
	//
	// CASE 1: NO:NO:NO  (NO DATA IN VISIBLE RANGE)  ----------------------------------------------OK PJM April 30, 2008
	//
	if ((daTimestampsBuffer[0] == 0.0) &&	//no data earlier than interval
		(ulTotalPtsRead == 0) &&			//no data within interval
		(daTimestampsBuffer[1] == 0.0))		//no data later than interval
	{
		//actual = 0;
		bSuccess = true;
	}
	///////////////////////////////////////////////////////////////////////
	//
	// CASE 2: NO:NO:YES  (NO DATA IN VISIBLE RANGE)  -----------------------------------------------OK PJM April 30, 2008
	//
	else if ((daTimestampsBuffer[0] == 0.0) &&	//no data earlier than interval
		(ulTotalPtsRead == 0) &&			//no data within interval
		(daTimestampsBuffer[1] != 0.0))		//yes - data later than interval
	{
		//actual = 0;
		bSuccess = true; 
	}
	///////////////////////////////////////////////////////////////////////
	//
	// CASE 3: YES:NO:NO (Horizontal line all the way across)  -------------------------------------OK PJM May 2, 2008
	//
	else if ((daTimestampsBuffer[0] != 0.0) &&	//yes - data earlier than interval
		(ulTotalPtsRead == 0) &&			//no data within interval
		(daTimestampsBuffer[1] == 0.0))		//no data later than interval
	{
		dTime[0] = dFirstDateThisViewPeriod;	//left edge of graph
		dState[0] = daDataBuffer[0];			//prior point was 1 or 0
		dTime[1] = dLastDateThisViewPeriod;		//right edge of graph
		dState[1] = daDataBuffer[0];			//horizontal line from left to right
		actual = 2; 
	}
	///////////////////////////////////////////////////////////////////////
	// 
	// CASE 4: YES|NO|YES (Horizontal line all the way across)  ---------------------------------OK PJM April 30, 2008
	//
	else if ((daTimestampsBuffer[0] != 0.0) &&	//yes - data earlier than interval
		(ulTotalPtsRead == 0) &&			//no data within interval
		(daTimestampsBuffer[1] != 0.0))		//yes -  data later than interval
	{
		dTime[0] = dFirstDateThisViewPeriod;	//left edge of graph
		dState[0] = daDataBuffer[0];			//prior point was 1 or 0
		dTime[1] = dLastDateThisViewPeriod;		//right edge of graph
		dState[1] = daDataBuffer[0];			//horizontal line from left to right
		actual = 2; 
	}
	///////////////////////////////////////////////////////////////////////
	//
	// CASE 5: NO|YES|NO
	//
	else if ((daTimestampsBuffer[0] == 0.0) &&	//no data earlier than interval
		(ulTotalPtsRead != 0) &&			//yes - data within interval
		(daTimestampsBuffer[ulIndexFinalFlagData] == 0.0)) //no data later than interval
	{
		//first graphing point is t1,s1.
		dTime[0] = daTimestampsBuffer[1];
		dState[0] = daDataBuffer[1];
		actual = 1;  //points to plot so far

		if (ulTotalPtsRead == 1)											//---------------------------OK PJM May 2, 2008
		{
			dTime[1] = dLastDateThisViewPeriod;
			dState[1] = daDataBuffer[1];
			actual = 2;  //plot only 2 points
		}
		else
		{
			for (rawdex = 2; rawdex < (int)(ulTotalPtsRead + 1); rawdex++)  //---------------------------OK PJM April 30, 2008
			{
				//t1,s0
				dTime[actual] = daTimestampsBuffer[rawdex];
				dState[actual] = daDataBuffer[rawdex-1];
				actual++;

				//t1,s1
				dTime[actual] = daTimestampsBuffer[rawdex];
				dState[actual] = daDataBuffer[rawdex];
				actual++;
			}
		}

		// No data points later than the interval, so just stop.
		//goto caseCompleted;
	}
	//////////////////////////////////////////////////////////////////////
	//
	// CASE 6: NO|YES|YES
	//
	// index right end flag data = 1 + ulTotalPtsRead
	else if ((daTimestampsBuffer[0] == 0.0) &&	//no data earlier than interval
		(ulTotalPtsRead != 0) &&			//yes - data within interval
		(daTimestampsBuffer[ulIndexFinalFlagData] != 0.0)) //yes - data later than interval
	{
		//first point to graph will be t1,s1.
		dTime[0] = daTimestampsBuffer[1];
		dState[0] = daDataBuffer[1];
		actual = 1;

		if (ulTotalPtsRead == 1)									//-----------------------OK PJM April 30, 2008
		{
			dTime[1] = dLastDateThisViewPeriod;
			dState[1] = daDataBuffer[1];
			actual = 2;  //plot only 2 points
		}
		else
		{
			for (rawdex = 2; rawdex < (int)(ulTotalPtsRead + 1); rawdex++) //-----------------OK PJM May 2, 2008
			{
				//t1,s0
				dTime[actual] = daTimestampsBuffer[rawdex-1];
				dState[actual] = daDataBuffer[rawdex];
				actual++;

				//t1,s1
				dTime[actual] = daTimestampsBuffer[rawdex];
				dState[actual] = daDataBuffer[rawdex];
				actual++;
			}

			dTime[actual] = dLastDateThisViewPeriod;	//right edge of graph OR end of data
			dState[actual] = daDataBuffer[rawdex-1];	//horizontal line from left to right
			actual++;
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//
	// CASE 7: YES|YES|NO
	//
	else if ((daTimestampsBuffer[0] != 0.0) &&	//yes - data earlier than interval
		(ulTotalPtsRead != 0) &&			//yes - data within interval
		(daTimestampsBuffer[ulIndexFinalFlagData] == 0.0))		//no data later than interval
	{
		dTime[0] = dFirstDateThisViewPeriod;	//left edge of graph
		dState[0] = daDataBuffer[0];			//prior point was 1 or 0
		actual = 1;

		if (ulTotalPtsRead == 1)											//-----------------OK PJM May 2, 2008
		{
			dTime[1] = daTimestampsBuffer[1];
			dState[1] = daDataBuffer[0];
			dTime[2] = daTimestampsBuffer[1];
			dState[2] = daDataBuffer[1];
			dTime[3] = dLastDateThisViewPeriod;
			dState[3] = daDataBuffer[1];
			actual = 4;
		}
		else
		{
			for (rawdex = 1; rawdex < (int)(ulTotalPtsRead + 1); rawdex++) //-----------------OK PJM May 2, 2008
			{
				//t1,s0
				dTime[actual] = daTimestampsBuffer[rawdex];
				dState[actual] = daDataBuffer[rawdex-1];
				actual++;

				//t1,s1
				dTime[actual] = daTimestampsBuffer[rawdex];
				dState[actual] = daDataBuffer[rawdex];
				actual++;
			}

			// No data points later than the interval, so extend final state to the right
			// edge of the graph.
			dTime[actual] = dLastDateThisViewPeriod;	//right edge of graph OR end of data
			dState[actual] = daDataBuffer[rawdex-1];	//horizontal line from left to right
			actual++;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//
	// CASE 8: YES|YES|YES
	//
	else if ((daTimestampsBuffer[0] != 0.0) &&	//yes - data earlier than interval
		(ulTotalPtsRead != 0) &&			//yes - data within interval
		(daTimestampsBuffer[ulIndexFinalFlagData] != 0.0)) //yes - data later than interval
	{
		dTime[0] = dFirstDateThisViewPeriod;	//left edge of graph
		dState[0] = daDataBuffer[0];			//prior point was 1 or 0
		actual = 1;

		if (ulTotalPtsRead == 0)									//-----------------OK PJM May 2, 2008
		{
			dTime[1] = daTimestampsBuffer[1];
			dState[1] = daDataBuffer[0];
			dTime[2] = daTimestampsBuffer[1];
			dState[2] = daDataBuffer[1];
			dTime[3] = dLastDateThisViewPeriod;
			dState[3] = daDataBuffer[1];
			actual = 4;
		}
		else
		{
			for (rawdex = 1; rawdex < (int)(ulTotalPtsRead + 1); rawdex++) //-----------------OK PJM May 2, 2008
			{
				//t1,s0
				dTime[actual] = daTimestampsBuffer[rawdex];
				dState[actual] = daDataBuffer[rawdex-1];
				actual++;

				//t1,s1
				dTime[actual] = daTimestampsBuffer[rawdex];
				dState[actual] = daDataBuffer[rawdex];
				actual++;
			}

			dTime[actual] = dLastDateThisViewPeriod;	//right edge of graph
			dState[actual] = daDataBuffer[rawdex-1];		//horizontal line from left to right
			actual++;
		}
	}

	if (actual != 0)
	{
		*ppdhaData =       (double huge *)CGUI_HugeMem::Alloc(actual, sizeof(double));
		*ppdhaTimestamps = (double huge *)CGUI_HugeMem::Alloc(actual, sizeof(double));

		for (i = 0; i < actual; i++)
		{
			(*ppdhaTimestamps)[i] = dTime[i] - mdDB_StartDate;
			(*ppdhaData)[i]	      = dState[i];	
		}

		if (pProgress != NULL) 
		{
			dDeltaFrac = dFracAfter - dFracBefore;
			dFracThisRead = (double)ulTotalPtsRead / (double)ulNumPts;
			dCurrentFrac = dFracBefore + dDeltaFrac * dFracThisRead;

			pglFacCfg->GetChannelLongName(glsFacNum, &ChanID, szChanName, DB_NAME_LEN);
			Text1.Printf("Reading Channel %s", szChanName);
			Text.Printf("%lu data points.", ulTotalPtsRead);

			if (!(pProgress->TwoLineProgressBarSetValue(dCurrentFrac, Text1, Text))) 
				iStatus = 0;
		}
										
		if (iStatus == 1) 
		{
			bSuccess = true;
			*pulNumDataPts = actual;
								  
			ReadChannelEvents((short)uiPL_ChanIndex, dDspStartOffset, dViewPeriodDays, pulNumEvents, paEvents);
			ReadChannelMarks((short)uiPL_ChanIndex, dDspStartOffset, dViewPeriodDays, pulNumMarks, paulMarks);
		}
	} // if actual != 0

quit:
	delete [] daTimestampsBuffer;
	delete [] daDataBuffer;
	delete [] dTime;
	delete [] dState;
	
	if (!bSuccess) 
	{
		CGUI_HugeMem::Free(*ppdhaData);
		*ppdhaData = NULL;
		CGUI_HugeMem::Free(*ppdhaTimestamps);
		*ppdhaTimestamps = NULL;
		*paEvents = NULL;
		*pulNumDataPts = 0L;
		*pulNumEvents = 0L;
		return 0;
	}
	else
	{
		return 1;
	}
}

