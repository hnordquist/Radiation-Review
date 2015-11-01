
/* WriteXML.H  Function Prototypes */

#ifndef WriteXML_H
	#define WriteXML_H
	
	#define FIRST_EVENT				0
	#define NEXT_EVENT				1
	#define WRITE_EVENT_LIST_TO_XML	2

	void WriteEventSummaryToXmlFile(struct db_event_rec *pdbEvent, short sAction, char* pszFileNameAndPath);

#endif
