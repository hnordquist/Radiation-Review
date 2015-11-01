
/* Interval.H  Function Prototypes */

#ifndef Interval_H
	#define Interval_H
	
	
	BOOL ConvertStructsToSearchIntervalParameters(struct GEN_DATE_STRUCT *pAdjGenDate, struct GEN_TIME_STRUCT *pAjdGenTime, const int iIntervalUnits, unsigned long ulInterval, DATE *pdAdjStart, DATE *pdAdjEnd);
	BOOL GetDatabaseIntervalParameters(char *pszAdjFirstDate, char *pszAdjLastDate, unsigned long *pulDbIntervalInWholeDays, DATE *pdDbExactInterval);
	void GetReviewPeriodParameters(DATE *pdReviewPeriodStart, DATE *pdReviewPeriodEnd, unsigned long *pulReviewPeriodIntervalInDays);
	
	DATE ConvertMyTimestampToDATETimestamp(const double dTimestamp);
	unsigned long ConvertDATETimestampToMyTimestamp(const DATE dTimestamp);


#endif

