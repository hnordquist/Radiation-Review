#ifndef DBREADSTRUCT_H
#define DBREADSTRUCT_H

#include "ComDataStructures.h"

	int GetIntegerDataByStation(struct IDStruct StaID, DATE dAdjStartTime, DATE dAdjEndTime , unsigned long ulNumPts, unsigned long *pulNumPtsRead, struct WHOLE_LONG_DATA_PT *plData, DATE *pdAdjRealEndTime);
	int GetDoubleDataByStation(struct IDStruct StaID, DATE dAdjStartTime, DATE dAdjEndTime , unsigned long ulNumPts, unsigned long *pulNumPtsRead, struct WHOLE_DOUBLE_DATA_PT1 *pdData, DATE *pdAdjRealEndTime, bool bRates);
	int GetFloatDataByStation(struct IDStruct StaID, DATE dAdjStartTime, DATE dAdjEndTime , unsigned long ulNumPts, unsigned long *pulNumPtsRead, struct WHOLE_FLOAT_DATA_PT1 *pfData, DATE *pdAdjRealEndTime);
	int WriteDbDataStatus(struct IDStruct StaID, DATE dAdjStartTime, unsigned long ulNumPts, struct WHOLE_DOUBLE_DATA_PT1 *pdData);

#endif //DBREADSTRUCT_H