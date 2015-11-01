
/* DbRdArr.H  Function Prototypes */

#ifndef DbRdArr_H
	#define DbRdArr_H

	int GetDataByChannel(short sChan, DATE dAdjStartTime, DATE dAdjEndTime, unsigned long ulNumPts, unsigned long *pulNumPts, double *pdX, double *pdY,  DATE *pdAdjTimestampOfNextPoint);	// eventually delete
	int GetDataByChannel(struct IDStruct ChanID, DATE dAdjStartTime, DATE dAdjEndTime, unsigned long ulNumPts, unsigned long *pulNumPts, double *pdX, double *pdY,  DATE *pdAdjTimestampOfNextPoint);

#endif
