
/* EventTypeLists.H  Function Prototypes */

#ifndef EventTypeLists_H
	#define EventTypeLists_H
	
	int GetEventTypePicklist(struct IDStruct ChanID, short sListType, char **pList[], short *psNumChcs);
	int GetEventTypesForPlNum(struct IDStruct ChanID, unsigned int uiChc, unsigned short *pusType1, unsigned short *pusType2);
	int GetPlNumForEventTypes(struct IDStruct ChanID, unsigned int *puiChc, unsigned short usType1, unsigned short usType2);

	void GetEventTypeStr(struct db_event_rec *pdbEvent, char *pszEventType);
	void GetEventType1Str(struct db_event_rec *pdbEvent, char *pszEventType);
	void GetEventType2Str(struct db_event_rec *pdbEvent, char *pszEventType);
	void GetDirEventStatusStr(struct db_dir_event_rec *pdbDirEvent, char *pszEventStatus);



#endif
