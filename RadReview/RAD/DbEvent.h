
/* DbEvent.H  Function Prototypes */

#ifndef DbEvent_H
	#define DbEvent_H


	void RegisterTableUpdateFunction(void (*pUpdateFnc)(void));
	int ReadEventRec(struct db_event_rec *pdbChanEvent, struct db_event_rec *pdbChanEvent1);
//	int CreateChanEventRec(struct db_event_rec *pdbChanEvent);	// eventually delete
	int CreateChanEventRec(struct IDStruct ChanID, struct db_event_rec *pdbChanEvent);
//	int ReplaceChanEvent(struct db_event_rec *pdbOldEvent, struct db_event_rec *pdbNewEvent);	// eventually delete
	int ReplaceChanEvent(struct IDStruct OldChanID, struct db_event_rec *pdbOldEvent, struct IDStruct NewChanID, struct db_event_rec *pdbNewEvent);
	
//	int GetDataStatisticsForEventsByChannel(short sChan, DATE dStartTime, DATE dEndTime, unsigned long *pulNumEvents, unsigned short usEventType);	// eventually delete
	int GetDataStatisticsForEventsByChannel(IDStruct ChanID, DATE dStartTime, DATE dEndTime, unsigned long *pulNumEvents, unsigned short usEventType);
//	int GetDataStatisticsForEventsByStation(short sSta, DATE dStartTime, DATE dEndTime, unsigned long *pulNumEvents);
		
//	int GetDataByChannelEvents(short sChan, DATE dStartTime, DATE dEndTime, unsigned long ulNumEventsToRead, unsigned long *pulNumEventsRead, struct db_event_rec huge *pChanEvents, unsigned short usEventType);	// eventually delete
	int GetDataByChannelEvents(IDStruct ChanID, DATE dStartTime, DATE dEndTime, unsigned long ulNumEventsToRead, unsigned long *pulNumEventsRead, struct db_event_rec huge *pChanEvents, unsigned short usEventType);
	
	int DeleteDataByChannelEvents(short sFacNum, unsigned short usEventType);
	int DeleteDataByChannelEvents(struct db_event_rec *pdbChanEvent);
	int ScanChannelEvents(DATE dStartTime, short sScanType, struct db_event_rec *pdbChanEvent);
	int ScanChannelEvents(DATE dStartTime,short sScanType, struct db_event_rec *pdbChanEvent, IDStruct ChanID);
	int GroupChanEvents(DATE dStartTime, DATE dEndTime, unsigned long ulMinDeltaInSeconds, unsigned long ulMaxDeltaInSeconds, short sType);


	int FindDirEventRec(struct db_dir_event_rec *pdbDirEvent);
	int CreateDirEventRec(struct db_dir_event_rec *pdbDirEvent);
	int UpdateDirEvent(struct db_dir_event_rec *pdbDirEvent);
	int ReadDirEvent(struct db_dir_event_rec *pdbDirEvent);
	int GetDataStatisticsForDirEventsByChannel(short sChan, DATE dStartTime, DATE dEndTime, unsigned long *pulNumEvents);
	int GetDataByChannelDirEvents(short sChan, DATE dStartTime, DATE dEndTime, unsigned long ulNumEventsToRead, unsigned long *pulNumEventsRead, struct db_dir_event_rec *pDirEvents);
	int DeleteDbDirEvents(short sFacNum);
	int DeleteDbDirEvent(struct db_dir_event_rec *pdbDirEvent);
	int ScanDirEventRecs(DATE dStartTime,short sType, struct db_dir_event_rec *pdbDirEvent);
	int FindDirEventAssociatedWithChanEvent(struct db_event_rec *pdbEvent, BOOL *pbMatchingEvent, struct db_dir_event_rec *pdbDirEvent);

	void FormatEventRecordForDisplay(struct db_event_rec *pdbChanEvent, CGFmtStr *pFormattedString);










	

#endif
