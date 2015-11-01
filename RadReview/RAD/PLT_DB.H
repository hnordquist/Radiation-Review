
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

