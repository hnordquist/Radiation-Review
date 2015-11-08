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
/* gen.h */

#ifndef  GEN_H
#define  GEN_H

/*-----------------------------------------------------------------------------
 *  Define symbolically the various date-time formats.
 *---------------------------------------------------------------------------*/

/* ------------------------------------------------
 *  9/5/89 - jap
 *  the following date-time formats have been replaced
 *  with the addition of the new date-time functions
 *  written by SK.  -1 through 5 are the same -- 6
 *  through 9 have been added.	See below.
 *
 *  #define DTF_NONE		   -1
 *  #define DTF_C		    0
 *  #define DTF_DEC		    1
 *  #define DTF_WFD		    2
 *  #define DTF_US		    3
 *  #define DTF_IAEA		    4
 *  #define DTF_MCA35		    5
 *
 * ------------------------------------------------ */
       
/* ------------------------------------------------------------------
 *  Structures needed for handling time and date
 * ----------------------------------------------------------------*/

struct GEN_TIME_STRUCT {
		    int hr;
		    int min;
		    int sec;
		    int msec;
		   };

struct GEN_DATE_STRUCT {
		    int yr;
		    int mon;
		    int day;
		   };

/*-----------------------------------------------------------------------------
 *  Define symbolically the various date-time formats.
 *  Eventually gen will be cleaned up and these will combine with the DTF
 *  already in gen.
 *---------------------------------------------------------------------------*/

#define GEN_DTF_NONE		  -1
#define GEN_DTF_C		   0
#define GEN_DTF_DEC		   1
#define GEN_DTF_WFD		   2
#define GEN_DTF_US		   3
#define GEN_DTF_IAEA		   4
#define GEN_DTF_MCA35		   5
#define GEN_DTF_EURO		   6
#define GEN_DTF_HM		   7
#define GEN_DTF_HMS		   8
#define GEN_DTF_HMSM		   9

/* number of days since gen library time zero (Jan. 1, 1952) to
   MS-DOS time zero (Jan 1, 1970) */

#define GEN_DOS_TIME_ZERO_DAYS 6575.0

/* seconds per day */

#define SECONDS_PER_DAY 86400.0

/* type of sort done by gen_get_filenames() */

#define GEN_SORT_ALPHA	  0
#define GEN_SORT_ASC_DATE 1
#define GEN_SORT_DES_DATE 2

/* allocate max # strings possible */
#define GEN_MAX_STR_ALLOC	-1

/* max length of a GEN error msg */
#define GEN_MAX_MSG_LEN 80


/*  Library status codes */

#define GEN_SUCCESS  0
#define GEN_INVALID_DATE  1
#define GEN_INVALID_DATE_FORMAT  2
#define GEN_INVALID_TIME  3
#define GEN_INVALID_TIME_FORMAT  4
#define GEN_HEAP_ERROR  5
#define GEN_INVALID_YEAR  6
#define GEN_FILE_OPEN_FAILURE  7
#define GEN_NO_FILE_NAME_MATCH  8
#define GEN_INVALID_DRIVE  9
#define GEN_FILE_WRITE_FAILURE  10

#endif

/* Library function prototypes */

char * gen_message(int status);
int gen_date_time_str_to_seconds(unsigned long *ptr_total_seconds,char *date,char *time,int iaea_format);
void gen_seconds_to_date_time_str(char *date_str,char *time_str,unsigned long *ptr_long_time,int iaea_date_format);
void gen_julian_days_to_struct(double double_julian,struct GEN_DATE_STRUCT *p_date,struct GEN_TIME_STRUCT *p_time);
double gen_struct_to_julian_days(struct GEN_DATE_STRUCT date,struct GEN_TIME_STRUCT time);
double gen_current_julian_days(void);
void gen_unpack_date(char *datestr,int *p_day,int *p_month,int *p_year,int iaea_date);
void gen_unpack_time(char *timestr,int *p_hour,int *p_minute,int *p_second);
void gen_path_get(char *filename, char *directory, char *path);
int gen_disk_usage(char drive_letter, unsigned long * space_available, double * percent_space_available);
void gen_set_drive(char drive_letter);
int gen_date_time(int format, char **ptr_string); 
int gen_date_str_to_struct(char *date_str, int date_format, struct GEN_DATE_STRUCT *ptr_date);
int gen_date_struct_to_str(struct GEN_DATE_STRUCT date, int format, char **ptr_date_str);
int gen_get_filenames(char *filename_to_match, unsigned int max_filenames_allowed, int sort_order, char ptr_filenames[][13], unsigned int  *ptr_num_filenames);
int gen_get_file_date_time_struct(char *file_name, struct GEN_DATE_STRUCT *ptr_date, struct GEN_TIME_STRUCT *ptr_time);
int gen_validate_date(struct GEN_DATE_STRUCT *ptr_date);
int gen_extend_str(int max_char, char string[]);
int gen_file_copy(char *filename, char *origin_dir, char *dest_dir);
int gen_file_delete(char *filename, char *sub_dir);
int gen_file_rename(char *filename, char *origin_dir, char *dest_dir);
void gen_count_filenames(char *filename_to_match, int *count_ptr);
int gen_get_msg(int function_code, char **ptr_message);
int gen_shorten_str(int max_char, char string[]);
int gen_allocate_str_array(int max_char, int max_strings, unsigned int reserved_memory, char ***ptr_str_array);
void gen_deallocate_str_array(char **str_array);
int gen_validate_time(struct GEN_TIME_STRUCT *ptr_time);
int gen_time_str_to_struct(char *time_str, int time_format, struct GEN_TIME_STRUCT *ptr_time);
int gen_time_struct_to_str(struct GEN_TIME_STRUCT time, int format, char **ptr_time_str);
int gen_wait(double seconds);
