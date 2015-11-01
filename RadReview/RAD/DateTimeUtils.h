
/* DateTimeUtils.H  Function Prototypes */

#ifndef DateTimeUtils_H
	#define DateTimeUtils_H

	//void convert_to_yyyymmdd (char * input_string, char * output_string);

	//void ConvertSecondsToStructs(unsigned long *pulSecs, GEN_DATE_STRUCT *pDate, GEN_TIME_STRUCT *pTime);
	//void ConvertStructsToSeconds(unsigned long *pulSecs, GEN_DATE_STRUCT *pDate, GEN_TIME_STRUCT *pTime);

	void ConvertGenStrToTimestampStr(const char *szOrigStr, short sdbChan, char *szNewStr);
//	void ConvertJulianSecsToTimestampStr(unsigned long ulJulianSecs, CGFmtStr *pszNewStr, char *szDateStr);
	int ConvertTimestampStrToJulianSecs(const char *pszTimestamp, unsigned long *pulTimeStamp) ;



#endif
