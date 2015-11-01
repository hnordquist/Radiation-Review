// GenDateTime.h: interface for the CGenDateTime class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

//#define DT_LEN	8

#include "gen.h"		// eventually replace this with just the stuff out of gen.h that is needed

#define MS_ROUND_OFF (1. / (SECS_PER_DAY * 2.))
#define REF_TIME	0L		/* seconds at Jan 1, 1952 */
#define SECS_PER_YEAR	31536000L	/* seconds in 365 day year */
#define SECS_PER_DAY	86400L		/* seconds in one day */


class CGenDateTime  
{
public:
	CGenDateTime();
	virtual ~CGenDateTime();

	int  GenDateTimeStrToSeconds(unsigned long *ptr_total_seconds, char *date, char *time, int  iaea_format);
	void GenSecondsToDateTimeStr(char *date_str, char *time_str, unsigned long *ptr_long_time, int iaea_date_format);
	
	void GenUnpackDate(char *datestr, int *p_day, int* p_month, int *p_year, int iaea_date);
	void GenUnpackTime(char *timestr, int *p_hour, int *p_minute, int *p_second);

	int CGenDateTime::GenValidateTime(struct GEN_TIME_STRUCT *ptr_time);


	void GenJulianDaysToStruct(double double_julian, struct GEN_DATE_STRUCT *p_date, GEN_TIME_STRUCT *p_time);
	int GenDateStrToStruct(char *date_str, int  date_format, struct GEN_DATE_STRUCT *ptr_date);
	int GenDateStructToStr(struct GEN_DATE_STRUCT date, int format, char **ptr_date_str);
	int GenTimeStrToStruct(char *time_str, int  time_format, struct GEN_TIME_STRUCT *ptr_time);
	int GenTimeStructToStr(struct GEN_TIME_STRUCT time, int format, char **ptr_time_str);


};

