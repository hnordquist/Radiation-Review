///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DateTimeUtils.cpp     
//Primary Author: BillHarker/Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
//	3.0.3.12	Made convert_from_yyyymmdd and convert_to_hhmmss static functions
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////
#include <gen.h>
#include "RAD.H"  
#include "DateTimeUtils.h"           
#include "upperlayer\RTComs.h"	// 8-Dec-04 SFK Added relative path


static int convert_from_yyyymmdd (char * input_string, char * output_string);
static void convert_to_hhmmss (char * input_string, char * output_string);


/*============================================================================
 *
 * function name: convert_to_yyyymmdd()			file name: dateconv.cpp
 *		  convert_from_yyyymmdd()
 *		  convert_to_hhmmss()
 *
 * purpose: convert a date string from the standard IAEA format of YY.MM.DD
 *	    to YYYYMMDD and vice versa.
 *
 * return value: SUCCESS/FAIL only for convert_from_yyyymmdd()
 *
 * special notes: 
 *
 * revision history:
 *
 *  date	author		revision
 *  ----	------		--------
 *  02/12/97 	Bill Harker	created
 *
 *============================================================================*/
static void convert_to_yyyymmdd (char * input_string, char * output_string)

{

    unsigned short i, j;

    if (strncmp (input_string, "52", 2) < 0)
	strcpy (output_string, "20");
    else
	strcpy (output_string, "19");
    i = 0;
    j = 2;
    while (input_string[i] != '\0')
    {
	if (input_string[i] != '.')
	{
	    output_string[j] = input_string[i];
	    j++;
	}
	i++;
    }
    output_string[j] = '\0';

    return;

}


static int convert_from_yyyymmdd (char * input_string, char * output_string)

{

    char year_string[5];
    char month_string[3];
    char day_string[3];
    int year, month, day;
    unsigned short i, j;
    char err_msg[MAX_LINE_LEN+1];

    /* check for a valid year */
    strncpy (year_string, input_string, 4);
    year_string[4] = '\0';
    year = atoi (year_string);
    if ((year < 1952) || (year > 2051))
    {
	sprintf (err_msg, "Year %d is illegal.", year);
	GUI_MsgBox (err_msg);
	return (FALSE);
    }
    /* check for a valid month */
    strncpy (month_string, &input_string[4], 2);
    month_string[2] = '\0';
    month = atoi (month_string);
    if ((month < 1) || (month > 12))
    {
	sprintf (err_msg, "Month %d is illegal.", month);
	GUI_MsgBox (err_msg);
	return (FALSE);
    }
    /* check for a valid day */
    strncpy (day_string, &input_string[6], 2);
    day_string[2] = '\0';
    day = atoi (day_string);
    if ((day < 1) || (day > 31))
    {
	sprintf (err_msg, "Day %d is illegal.", day);
	GUI_MsgBox (err_msg);
	return (FALSE);
    }

    i = 2;
    j = 0;

    while (input_string[i] != '\0')
    {
	if ((j == 2) || (j == 5))
	{
	    output_string[j] = '.';
	}
	else
	{
	    output_string[j] = input_string[i];
	    i++;
	}
	j++;
    }
    output_string[j] = '\0';

    return (TRUE);

}


static void convert_to_hhmmss (char * input_string, char * output_string)

{

    unsigned short i, j;

    i = 0;
    j = 0;
    while (input_string[i] != '\0')
    {
	if (input_string[i] != ':')
	{
	    output_string[j] = input_string[i];
	    j++;
	}
	i++;
    }
    output_string[j] = '\0';

    return;

}



/*====================================================================
 *
 *  Function Name:		ConvertSecondsToStructs
  *
 *  Function Purpose:	Convert a Julian time to a date and time structsConverts between IR Timestamp and standard
 *
 *	Return value:		none
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  30-Mar-1999 SFK 1.0 Created
 *	26-Jan-2000 SFK		Added ConvertStructsToSeconds
 *
 *====================================================================*/
void ConvertSecondsToStructs(unsigned long *pulSecs, GEN_DATE_STRUCT *pDate, GEN_TIME_STRUCT *pTime)

{
	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	gen_seconds_to_date_time_str(szDate, szTime, pulSecs, GEN_DTF_IAEA);
	gen_date_str_to_struct(szDate, GEN_DTF_IAEA, pDate);
	gen_time_str_to_struct(szTime, GEN_DTF_HMS, pTime);
}
                                    
void ConvertStructsToSeconds(unsigned long *pulSecs, GEN_DATE_STRUCT *pDate, GEN_TIME_STRUCT *pTime)
{
	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	char *pStr;
	gen_date_struct_to_str(*pDate, GEN_DTF_IAEA, &pStr);
	strcpy(szDate, pStr);
	gen_time_struct_to_str(*pTime, GEN_DTF_HMS, &pStr);
	strcpy(szTime, pStr);
	gen_date_time_str_to_seconds(pulSecs, szDate, szTime, GEN_DTF_IAEA);
}
                                    

/*====================================================================
 *
 *  Function Name:		ConvertJulianToTimestamp()
 *						ConvertTimestampToJulian()
 *
 *  Function Purpose:	Converts between IR Timestamp and standard
 *						Julian times.  Also attaches a location id when
 *						converting to a Timestamp.
 *
 *	Return value:		none
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  16-Feb-1997 SFK 1.0 Created
 *
 *====================================================================*/
#define RAD_TIMESTAMP_LEN 8+1+8+10
void ConvertGenStrToTimestampStr(const char *szOrigStr, short sdbChan, char *szNewStr)
{
// Gen format          97.01.03 09:34:56	                                                         
// TimestampStr format 1234519970103093456000

    char szTemp[RAD_TIMESTAMP_LEN+1];
	strcpy(szNewStr, "00000"); // no id specified in file
	    
    // break original rad timestamp into smaller strings
	strcpy(szTemp, szOrigStr);
	szTemp[8] = '\0';
	convert_to_yyyymmdd (szTemp, &(szNewStr[5]));
	
	szTemp[17] = '\0';
	convert_to_hhmmss((&szTemp[9]), &(szNewStr[13]));
	strcat(szNewStr,"000"); 
	
}    

static void ConvertJulianSecsToTimestampStr(unsigned long ulJulianSecs, CGFmtStr *pszNewStr, char *szDateStr)
{
// Gen format          97.01.03 09:34:56	                                                         
// TimestampStr format 1234519970103093456000

	char szTime[DT_LEN+1], szDate[DT_LEN+1];
	int iYear;
    
    // handle years > 2000
	iYear = 19;
	if (ulJulianSecs >= YEAR_2000) iYear = 20;
	gen_seconds_to_date_time_str(szDate, szTime, &ulJulianSecs, GEN_DTF_IAEA);
	szDate[2] = '\0';
	szDate[5] = '\0';
	szTime[2] = '\0';
	szTime[5] = '\0';
	pszNewStr->Printf("00000%2d%s%s%s%s%s%s000", iYear, &szDate[0], &szDate[3], &szDate[6], 
					                              &szTime[0], &szTime[3], &szTime[6]);
	sprintf(szDateStr, "%2d%s.%s.%s", iYear, &szDate[0], &szDate[3], &szDate[6]);
}    

    
int ConvertTimestampStrToJulianSecs(const char *pszTimestamp, unsigned long *pulTimeStamp) 
{

	char szDate[DT_LEN+1];
	char szTime[DT_LEN+1];
	char szTempTimestamp[uiTIME_STAMP_LENGTH+1];
	int status;
	
	*pulTimeStamp = 0;	
    strncpy(szTempTimestamp, pszTimestamp, uiTIME_STAMP_LENGTH+1); 
    szTempTimestamp[13] = '\0';
	status = convert_from_yyyymmdd(&(szTempTimestamp[5]), szDate);	
	if (status == FALSE) return (FALSE);

	strncpy(szTempTimestamp, pszTimestamp, uiTIME_STAMP_LENGTH+1); 
	strncpy(szTime, &(szTempTimestamp[13]), 2);
	szTime[2] = '\0';
	strcat(szTime, ":");
	strncat(szTime, &(szTempTimestamp[15]), 2);
	szTime[5] = '\0';
	strcat(szTime, ":");
	strncat(szTime, &(szTempTimestamp[17]), 2);
	szTime[8] = '\0';
	
	gen_date_time_str_to_seconds(pulTimeStamp, szDate, szTime, GEN_DTF_IAEA);
	return(TRUE);
}	

    
                   