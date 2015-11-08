
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
#ifndef PLT_DB_H
	#define PLT_DB_H


	// =====================================================================
	// =========================== CPlotDB ===========================
	// =====================================================================
	
	class CPlotDB
	{
		public:
			CPlotDB(BOOL *pbValidDB);
			~CPlotDB();

			// Rountines to get time and date strings for annotations etc.
			const char *RelDateToDateAndTimeStr(DATE dRelDateValue);
			const char *RelDateToTimeStr(DATE dRelDateValue);
			const char *AbsDateToDateAndTimeStr(DATE dDateValue);
			const char *AbsDateToTimeStr(DATE dDateValue);
			double RelDateToDate(DATE dRelDate);


			BOOL GetDB_Parameters();
			BOOL GetChannelNames();
			BOOL GetDateRange(DATE *pdDB_StartDate, DATE *pdDB_EndDate);
			BOOL GetDetName(UINT uiDetectorIndex, CGStr *pDetName);
			BOOL GetDetShortName(UINT uiDetectorIndex, CGStr *pDetName);
			UINT GetNumDet() {return(muiNumDetectors);};
			BOOL IsValid() {return(mbValidDB);};

			// modified prototype in 2.06f for using doubles to graph
			BOOL ReadChannelData(UINT uiPL_ChanIndex, DATE dDspStartOffset, DATE dViewPeriodDays, BOOL *pbAbortRead, double huge **ppdhaData, double huge **ppdhaTimestamps, ULONG *pulNumDataPts, ULONG *pulNumEvents,	EVENTS1 **paEvents, ULONG *pulNumMarks, MARKS1 **paulMarks, TwoLineProgressBar *pProgress, ULONG *pulNumPrevPts, double dFracBefore, double dFracAfter);
			BOOL ReadChannelDataForEOSS(UINT uiPL_ChanIndex, DATE dDspStartOffset, DATE dViewPeriodDays, BOOL *pbAbortRead, double huge **ppdhaData, double huge **ppdhaTimestamps, ULONG *pulNumDataPts, ULONG *pulNumEvents,	EVENTS1 **paEvents, ULONG *pulNumMarks, MARKS1 **paulMarks, TwoLineProgressBar *pProgress, ULONG *pulNumPrevPts, double dFracBefore, double dFracAfter);

			BOOL DeleteMarkRecord(UINT uiPL_ChanIndex, DATE dMarkDate);
			BOOL CreateMarkRecord(UINT uiPL_ChanIndex, DATE dMarkDate, int& raison);
			BOOL ReadChannelMarks(UINT uiPL_ChanIndex, DATE dDspStartOffset, DATE dViewPeriodDays, ULONG *pulNumMarks, MARKS1 **pMarks);

			BOOL DeleteEventRecord(UINT uiPL_ChanIndex, DATE dRelDateFirst, DATE dRelDateLast);
			BOOL CreateEventRecord(UINT uiPL_ChanIndex, DATE dRelDateFirst, DATE dRelDateLast,	const char *szEventId, UINT uiTypePickIndex);
			BOOL ReadEventRecord(UINT uiPL_ChanIndex, DATE dRelDateFirst, DATE dRelDateLast, struct db_event_rec *pdbChanEvent);
			BOOL EditEventRecord(UINT uiPL_ChanIndex, DATE dRelDateFirst, DATE dRelDateLast, DATE dNewLowValueRelSecs, DATE dNewHighValueRelSecs, const char *szEventId, UINT uiTypePickIndex);
			BOOL ReadChannelEvents(UINT uiPL_ChanIndex,	DATE dDspStartOffset, DATE dViewPeriodDays,	ULONG *pulNumEvents, EVENTS1 **paEvents);
			
		private:

			CGStr *maDetNames;
			CGStr *maDetShortNames;
			UINT muiNumDetectors;
			//int miStation;
			BOOL mbValidDB;
			int miFacilityNum;
			DATE mdDB_StartDate;	// Earliest date in DB in seconds.
			DATE mdDB_EndDate;		// Latest date in DB in seconds.
			void CancelGraphReadServiceFn(CGUI_Dlg *pDlg, RESOURCE_ID ID);
	};
	
#endif

