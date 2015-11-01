
/* DbLimit.H  Function Prototypes */

#ifndef DbLimit_H
	#define DbLimit_H

	#include <wtypes.h>
	BOOL GetDbTimeRange(const short sSta, DATE *pdStartTime, DATE *pdEndTime);
	BOOL GetDbTimeRange(const struct IDStruct StaID, DATE *pdStartTime, DATE *pdEndTime);


#endif
