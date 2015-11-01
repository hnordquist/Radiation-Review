#ifndef DB_MAIN_H
#define DB_MAIN_H

/* Raima Database Manager 4.5 [Build 17] */

/*  Scheme file for Radiation Monitoring database */


/* database db_main record/key structure declarations */

struct db_fac_rec {
   short s_fac_num_key;
   short s_fac_picklist_num;
   char sz_fac_name[40];
   char sz_fac_short_name[10];
};

struct db_sta_rec {
   short s_sta_num_key;
   short s_sta_picklist_num;
   char sz_sta_name[40];
   char sz_sta_short_name[10];
   short s_sta_type;
   short s_data_type;
};

struct db_chan_rec {
   short s_chan_num_key;
   short s_chan_picklist_num;
   char sz_chan_name[40];
   char sz_chan_short_name[10];
   short s_chan_offset;
   char sz_chan_units[40];
   char sz_chan_short_units[10];
   float f_event_threshold;
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
   double d_fdata_time[50];
   unsigned long ul_fdata_status[50];
   double d_fdata_etime[50];
   float f_data_chan1[50];
   float f_data_chan2[50];
   float f_data_chan3[50];
   float f_data_chan4[50];
   float f_data_chan5[50];
   float f_data_chan6[50];
   float f_data_chan7[50];
};

struct db_double_data_rec {
   double d_ddata_beg_time_key;
   unsigned long ul_ddata_pts_so_far;
   double d_ddata_time[50];
   unsigned long ul_ddata_status[50];
   double d_ddata_etime[50];
   double d_data_chan1[50];
   double d_data_chan2[50];
   double d_data_chan3[50];
   double d_data_chan4[50];
   double d_data_chan5[50];
   double d_data_chan6[50];
   double d_data_chan7[50];
};

struct db_event_rec {
   double d_event_beg_time_key;
   double d_event_end_time;
   double d_event_max_time;
   double d_event_grp_time;
   double d_event_integral_sum;
   double d_event_max_value;
   double d_event_avg_reals_rate;
   double d_event_avg_totals_rate;
   unsigned short us_event_chan;
   unsigned short us_event_sta;
   unsigned short us_event_sta_type;
   unsigned short us_event_type;
   unsigned short us_event_status;
   unsigned short us_event_type1;
   unsigned short us_event_status1;
   unsigned short us_event_type2;
   unsigned short us_event_status2;
   short s_num_runs;
   char sz_event_id[14];
   short sdum[2];
};

struct db_dir_event_rec {
   double d_dir_event_beg_time_key;
   double d_dir_event_end_time;
   double d_dir_event_ratio_numerator;
   double d_dir_event_ratio_denominator;
   double d_dir_event_pair_int_sum[4];
   double d_dir_event_pair_max_val_sum[4];
   unsigned short us_dir_event_location;
   unsigned short us_dir_event_direction;
   unsigned short us_dir_event_status;
   unsigned short us_dir_event_num_pairs;
   unsigned short us_dir_event_from_chan[4];
   unsigned short us_dir_event_to_chan[4];
   unsigned short us_dir_event_from_sta[4];
   unsigned short us_dir_event_to_sta[4];
   unsigned short us_dir_event_sta[4];
   unsigned short us_dir_event_to_sta_type[4];
   unsigned short us_dir_event_from_sta_type[4];
   short s_dir_event_num_numerator;
   short s_dir_event_num_denominator;
};

struct db_mark_rec {
   double d_mark_time_key;
   short s_mark_chan;
   short s_mark_sta;
   short s_mark_sta_type;
};

/* record, field and set table entry definitions */

/* File Id Constants */

/* Record Name Constants */
#define DB_FAC_REC 10000
#define DB_STA_REC 10001
#define DB_CHAN_REC 10002
#define DB_DAY_REC 10003
#define DB_FLOAT_DATA_REC 10004
#define DB_DOUBLE_DATA_REC 10005
#define DB_EVENT_REC 10006
#define DB_DIR_EVENT_REC 10007
#define DB_MARK_REC 10008

/* Field Name Constants */
#define S_FAC_NUM_KEY 0L
#define S_FAC_PICKLIST_NUM 1L
#define SZ_FAC_NAME 2L
#define SZ_FAC_SHORT_NAME 3L
#define S_STA_NUM_KEY 1000L
#define S_STA_PICKLIST_NUM 1001L
#define SZ_STA_NAME 1002L
#define SZ_STA_SHORT_NAME 1003L
#define S_STA_TYPE 1004L
#define S_DATA_TYPE 1005L
#define S_CHAN_NUM_KEY 2000L
#define S_CHAN_PICKLIST_NUM 2001L
#define SZ_CHAN_NAME 2002L
#define SZ_CHAN_SHORT_NAME 2003L
#define S_CHAN_OFFSET 2004L
#define SZ_CHAN_UNITS 2005L
#define SZ_CHAN_SHORT_UNITS 2006L
#define F_EVENT_THRESHOLD 2007L
#define D_DAY_BEG_TIME_KEY 3000L
#define D_DAY_BEG_TIME 3001L
#define D_DAY_END_TIME 3002L
#define UL_DAY_TOTAL_PTS 3003L
#define D_FDATA_BEG_TIME_KEY 4000L
#define UL_FDATA_PTS_SO_FAR 4001L
#define D_FDATA_TIME 4002L
#define UL_FDATA_STATUS 4003L
#define D_FDATA_ETIME 4004L
#define F_DATA_CHAN1 4005L
#define F_DATA_CHAN2 4006L
#define F_DATA_CHAN3 4007L
#define F_DATA_CHAN4 4008L
#define F_DATA_CHAN5 4009L
#define F_DATA_CHAN6 4010L
#define F_DATA_CHAN7 4011L
#define D_DDATA_BEG_TIME_KEY 5000L
#define UL_DDATA_PTS_SO_FAR 5001L
#define D_DDATA_TIME 5002L
#define UL_DDATA_STATUS 5003L
#define D_DDATA_ETIME 5004L
#define D_DATA_CHAN1 5005L
#define D_DATA_CHAN2 5006L
#define D_DATA_CHAN3 5007L
#define D_DATA_CHAN4 5008L
#define D_DATA_CHAN5 5009L
#define D_DATA_CHAN6 5010L
#define D_DATA_CHAN7 5011L
#define D_EVENT_BEG_TIME_KEY 6000L
#define D_EVENT_END_TIME 6001L
#define D_EVENT_MAX_TIME 6002L
#define D_EVENT_GRP_TIME 6003L
#define D_EVENT_INTEGRAL_SUM 6004L
#define D_EVENT_MAX_VALUE 6005L
#define D_EVENT_AVG_REALS_RATE 6006L
#define D_EVENT_AVG_TOTALS_RATE 6007L
#define US_EVENT_CHAN 6008L
#define US_EVENT_STA 6009L
#define US_EVENT_STA_TYPE 6010L
#define US_EVENT_TYPE 6011L
#define US_EVENT_STATUS 6012L
#define US_EVENT_TYPE1 6013L
#define US_EVENT_STATUS1 6014L
#define US_EVENT_TYPE2 6015L
#define US_EVENT_STATUS2 6016L
#define S_NUM_RUNS 6017L
#define SZ_EVENT_ID 6018L
#define SDUM 6019L
#define D_DIR_EVENT_BEG_TIME_KEY 7000L
#define D_DIR_EVENT_END_TIME 7001L
#define D_DIR_EVENT_RATIO_NUMERATOR 7002L
#define D_DIR_EVENT_RATIO_DENOMINATOR 7003L
#define D_DIR_EVENT_PAIR_INT_SUM 7004L
#define D_DIR_EVENT_PAIR_MAX_VAL_SUM 7005L
#define US_DIR_EVENT_LOCATION 7006L
#define US_DIR_EVENT_DIRECTION 7007L
#define US_DIR_EVENT_STATUS 7008L
#define US_DIR_EVENT_NUM_PAIRS 7009L
#define US_DIR_EVENT_FROM_CHAN 7010L
#define US_DIR_EVENT_TO_CHAN 7011L
#define US_DIR_EVENT_FROM_STA 7012L
#define US_DIR_EVENT_TO_STA 7013L
#define US_DIR_EVENT_STA 7014L
#define US_DIR_EVENT_TO_STA_TYPE 7015L
#define US_DIR_EVENT_FROM_STA_TYPE 7016L
#define S_DIR_EVENT_NUM_NUMERATOR 7017L
#define S_DIR_EVENT_NUM_DENOMINATOR 7018L
#define D_MARK_TIME_KEY 8000L
#define S_MARK_CHAN 8001L
#define S_MARK_STA 8002L
#define S_MARK_STA_TYPE 8003L

/* Set Name Constants */
#define FACILITY_SET 20000
#define FACILITY_TO_STATION_SET 20001
#define STATION_TO_CHANNEL_SET 20002
#define STATION_TO_DAY_SET 20003
#define DAY_TO_FLOAT_DATA_SET 20004
#define DAY_TO_DOUBLE_DATA_SET 20005
#define DAY_TO_EVENT_SET 20006
#define DAY_TO_DIR_EVENT_SET 20007
#define DAY_TO_MARK_SET 20008

/* Field Sizes */
#define SIZEOF_S_FAC_NUM_KEY 2
#define SIZEOF_S_FAC_PICKLIST_NUM 2
#define SIZEOF_SZ_FAC_NAME 40
#define SIZEOF_SZ_FAC_SHORT_NAME 10
#define SIZEOF_S_STA_NUM_KEY 2
#define SIZEOF_S_STA_PICKLIST_NUM 2
#define SIZEOF_SZ_STA_NAME 40
#define SIZEOF_SZ_STA_SHORT_NAME 10
#define SIZEOF_S_STA_TYPE 2
#define SIZEOF_S_DATA_TYPE 2
#define SIZEOF_S_CHAN_NUM_KEY 2
#define SIZEOF_S_CHAN_PICKLIST_NUM 2
#define SIZEOF_SZ_CHAN_NAME 40
#define SIZEOF_SZ_CHAN_SHORT_NAME 10
#define SIZEOF_S_CHAN_OFFSET 2
#define SIZEOF_SZ_CHAN_UNITS 40
#define SIZEOF_SZ_CHAN_SHORT_UNITS 10
#define SIZEOF_F_EVENT_THRESHOLD 4
#define SIZEOF_D_DAY_BEG_TIME_KEY 8
#define SIZEOF_D_DAY_BEG_TIME 8
#define SIZEOF_D_DAY_END_TIME 8
#define SIZEOF_UL_DAY_TOTAL_PTS 4
#define SIZEOF_D_FDATA_BEG_TIME_KEY 8
#define SIZEOF_UL_FDATA_PTS_SO_FAR 4
#define SIZEOF_D_FDATA_TIME 400
#define SIZEOF_UL_FDATA_STATUS 200
#define SIZEOF_D_FDATA_ETIME 400
#define SIZEOF_F_DATA_CHAN1 200
#define SIZEOF_F_DATA_CHAN2 200
#define SIZEOF_F_DATA_CHAN3 200
#define SIZEOF_F_DATA_CHAN4 200
#define SIZEOF_F_DATA_CHAN5 200
#define SIZEOF_F_DATA_CHAN6 200
#define SIZEOF_F_DATA_CHAN7 200
#define SIZEOF_D_DDATA_BEG_TIME_KEY 8
#define SIZEOF_UL_DDATA_PTS_SO_FAR 4
#define SIZEOF_D_DDATA_TIME 400
#define SIZEOF_UL_DDATA_STATUS 200
#define SIZEOF_D_DDATA_ETIME 400
#define SIZEOF_D_DATA_CHAN1 400
#define SIZEOF_D_DATA_CHAN2 400
#define SIZEOF_D_DATA_CHAN3 400
#define SIZEOF_D_DATA_CHAN4 400
#define SIZEOF_D_DATA_CHAN5 400
#define SIZEOF_D_DATA_CHAN6 400
#define SIZEOF_D_DATA_CHAN7 400
#define SIZEOF_D_EVENT_BEG_TIME_KEY 8
#define SIZEOF_D_EVENT_END_TIME 8
#define SIZEOF_D_EVENT_MAX_TIME 8
#define SIZEOF_D_EVENT_GRP_TIME 8
#define SIZEOF_D_EVENT_INTEGRAL_SUM 8
#define SIZEOF_D_EVENT_MAX_VALUE 8
#define SIZEOF_D_EVENT_AVG_REALS_RATE 8
#define SIZEOF_D_EVENT_AVG_TOTALS_RATE 8
#define SIZEOF_US_EVENT_CHAN 2
#define SIZEOF_US_EVENT_STA 2
#define SIZEOF_US_EVENT_STA_TYPE 2
#define SIZEOF_US_EVENT_TYPE 2
#define SIZEOF_US_EVENT_STATUS 2
#define SIZEOF_US_EVENT_TYPE1 2
#define SIZEOF_US_EVENT_STATUS1 2
#define SIZEOF_US_EVENT_TYPE2 2
#define SIZEOF_US_EVENT_STATUS2 2
#define SIZEOF_S_NUM_RUNS 2
#define SIZEOF_SZ_EVENT_ID 14
#define SIZEOF_SDUM 4
#define SIZEOF_D_DIR_EVENT_BEG_TIME_KEY 8
#define SIZEOF_D_DIR_EVENT_END_TIME 8
#define SIZEOF_D_DIR_EVENT_RATIO_NUMERATOR 8
#define SIZEOF_D_DIR_EVENT_RATIO_DENOMINATOR 8
#define SIZEOF_D_DIR_EVENT_PAIR_INT_SUM 32
#define SIZEOF_D_DIR_EVENT_PAIR_MAX_VAL_SUM 32
#define SIZEOF_US_DIR_EVENT_LOCATION 2
#define SIZEOF_US_DIR_EVENT_DIRECTION 2
#define SIZEOF_US_DIR_EVENT_STATUS 2
#define SIZEOF_US_DIR_EVENT_NUM_PAIRS 2
#define SIZEOF_US_DIR_EVENT_FROM_CHAN 8
#define SIZEOF_US_DIR_EVENT_TO_CHAN 8
#define SIZEOF_US_DIR_EVENT_FROM_STA 8
#define SIZEOF_US_DIR_EVENT_TO_STA 8
#define SIZEOF_US_DIR_EVENT_STA 8
#define SIZEOF_US_DIR_EVENT_TO_STA_TYPE 8
#define SIZEOF_US_DIR_EVENT_FROM_STA_TYPE 8
#define SIZEOF_S_DIR_EVENT_NUM_NUMERATOR 2
#define SIZEOF_S_DIR_EVENT_NUM_DENOMINATOR 2
#define SIZEOF_D_MARK_TIME_KEY 8
#define SIZEOF_S_MARK_CHAN 2
#define SIZEOF_S_MARK_STA 2
#define SIZEOF_S_MARK_STA_TYPE 2

#endif    /* DB_MAIN_H */
