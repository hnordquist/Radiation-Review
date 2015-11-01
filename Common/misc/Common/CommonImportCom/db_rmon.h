#ifndef DB_RMON_H
#define DB_RMON_H

/* Raima Database Manager 4.5 [Build 17] */

/*  Scheme file for Radiation Monitoring customized for VIFM data */


/* database db_rmon record/key structure declarations */

struct db_sta_rec {
   short s_sta_num_key;
   short s_sta_picklist_num;
   char sz_sta_name[40];
   char sz_sta_short_name[10];
   short s_sta_type;
   short s_data_type;
};

struct db_day_rec {
   double d_day_beg_time_key;
   double d_day_beg_time;
   double d_day_end_time;
   unsigned long ul_day_total_pts;
};

struct db_float_data_rec {
   double d_fdata_beg_time_key;
   unsigned long ul_fdata_pts_so_far;
   double d_fdata_time[78];
   unsigned long ul_fdata_status[78];
   double d_fdata_etime[78];
   float f_data_chan1[78];
   float f_data_chan2[78];
   float f_data_chan3[78];
   float f_data_chan4[78];
   float f_data_chan5[78];
   float f_data_chan6[78];
   float f_data_chan7[78];
   float f_data_chan8[78];
};

/* record, field and set table entry definitions */

/* File Id Constants */

/* Record Name Constants */
#define DB_STA_REC 10000
#define DB_DAY_REC 10001
#define DB_FLOAT_DATA_REC 10002

/* Field Name Constants */
#define S_STA_NUM_KEY 0L
#define S_STA_PICKLIST_NUM 1L
#define SZ_STA_NAME 2L
#define SZ_STA_SHORT_NAME 3L
#define S_STA_TYPE 4L
#define S_DATA_TYPE 5L
#define D_DAY_BEG_TIME_KEY 1000L
#define D_DAY_BEG_TIME 1001L
#define D_DAY_END_TIME 1002L
#define UL_DAY_TOTAL_PTS 1003L
#define D_FDATA_BEG_TIME_KEY 2000L
#define UL_FDATA_PTS_SO_FAR 2001L
#define D_FDATA_TIME 2002L
#define UL_FDATA_STATUS 2003L
#define D_FDATA_ETIME 2004L
#define F_DATA_CHAN1 2005L
#define F_DATA_CHAN2 2006L
#define F_DATA_CHAN3 2007L
#define F_DATA_CHAN4 2008L
#define F_DATA_CHAN5 2009L
#define F_DATA_CHAN6 2010L
#define F_DATA_CHAN7 2011L
#define F_DATA_CHAN8 2012L

/* Set Name Constants */
#define STATION_SET 20000
#define STATION_TO_DAY_SET 20001
#define DAY_TO_FLOAT_DATA_SET 20002

/* Field Sizes */
#define SIZEOF_S_STA_NUM_KEY 2
#define SIZEOF_S_STA_PICKLIST_NUM 2
#define SIZEOF_SZ_STA_NAME 40
#define SIZEOF_SZ_STA_SHORT_NAME 10
#define SIZEOF_S_STA_TYPE 2
#define SIZEOF_S_DATA_TYPE 2
#define SIZEOF_D_DAY_BEG_TIME_KEY 8
#define SIZEOF_D_DAY_BEG_TIME 8
#define SIZEOF_D_DAY_END_TIME 8
#define SIZEOF_UL_DAY_TOTAL_PTS 4
#define SIZEOF_D_FDATA_BEG_TIME_KEY 8
#define SIZEOF_UL_FDATA_PTS_SO_FAR 4
#define SIZEOF_D_FDATA_TIME 624
#define SIZEOF_UL_FDATA_STATUS 312
#define SIZEOF_D_FDATA_ETIME 624
#define SIZEOF_F_DATA_CHAN1 312
#define SIZEOF_F_DATA_CHAN2 312
#define SIZEOF_F_DATA_CHAN3 312
#define SIZEOF_F_DATA_CHAN4 312
#define SIZEOF_F_DATA_CHAN5 312
#define SIZEOF_F_DATA_CHAN6 312
#define SIZEOF_F_DATA_CHAN7 312
#define SIZEOF_F_DATA_CHAN8 312

#endif    /* DB_RMON_H */
