
/* DbInterval.H  Function Prototypes */

#ifndef DbInterval_H
	#define DbInterval_H

	int GetDataStatistics(short sSta, DATE dStartTime, DATE dEndTime, DATE *pdActualStartTime, DATE *pdActualEndTime, unsigned long *pulNumPts);
	int GetDataStatistics(short sSta, short sChan, DATE dStartTime, DATE dEndTime, DATE *pdActualStartTime, DATE *pdActualEndTime, unsigned long *pulNumPts);
	int GetDataStatistics(IDStruct StaID, BOOL bStation, DATE dAdjStartTime, DATE dAdjEndTime, DATE *pdActualAdjStartTime, DATE *pdActualAdjEndTime, unsigned long *pulNumPts);
	
	// temporary
	void CvtTimeToDayStart(unsigned long ulDayInSecs,unsigned long *pulDayStart);
	int SameDay(unsigned long ulTime1,unsigned long ulTime2);
	int SameDay(DATE dTime1, DATE dTime2);


#endif
