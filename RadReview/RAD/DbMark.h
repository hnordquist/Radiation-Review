
/* DbMark.H  Function Prototypes */

#ifndef DbMark_H
	#define DbMark_H

	int DeleteMarkRec(struct db_mark_rec *pdbMark);
	int FindMarkRec(struct db_mark_rec *pdbMark);
	int CreateMarkRec(struct db_mark_rec *pdbMark);

//	int GetDataStatisticsForMarks(short sChan, DATE dStartTime, DATE dEndTime, unsigned long *pulNumMarks);	//eventually delete
	int GetDataStatisticsForMarks(IDStruct ChanID, DATE dStartTime, DATE dEndTime, unsigned long *pulNumMarks);
	
//	int GetDataByStationMarks(short sChan, DATE dStartTime, DATE dEndTime, unsigned long ulNumMarks, unsigned long *pulNumMarksRead, struct db_mark_rec *pMarks);	// eventually delete
	int GetDataByStationMarks(struct IDStruct ChanID, DATE dStartTime, DATE dEndTime, unsigned long ulNumMarks, unsigned long *pulNumMarksRead, struct db_mark_rec *pMarks);

	int ScanMarkRecs(DATE dStartTime, short sType, struct db_mark_rec *pdbMark);


#endif
