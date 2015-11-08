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
