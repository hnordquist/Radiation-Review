#ifndef DB_DFLT_H
#define DB_DFLT_H

/* Raima Database Manager 4.5 [Build 17] */

/*  Scheme file for Radiation Monitoring default parameters database */
/*  Changed 01-12-99 to add graph_dflt_rec*/
/*  Extensive mods Sep 2003 to support new facility manager */
/*  5-5-2004 Save graph trace channel and station rather than track picklist position */
/*  Changed December 2005 by SK to add d_meas_add_min for SCR00191 */

/* database db_dflt record/key structure declarations */

struct db_dflt_rec {
   short s_dflt_facility;
   short s_dflt_station;
   short s_dflt_station_type;
   short s_dflt_channel;
   short s_integrity_station;
   short s_integrity_station_type;
   short s_integrity_tolerance;
   short s_days_station;
   short s_days_station_type;
   short s_chan_event_srch_width;
   short s_chan_event_srch_alg;
   short s_chan_event_summ_sort;
   short s_chan_event_summ_type;
   short s_chan_event_summ_min;
   short s_chan_event_summ_max;
   short s_rad_event_srch_pairs;
   short s_rad_event_summ_sort;
   short s_rad_event_summ_type;
   short s_mark_summ_sort;
   short s_mark_summ_type;
   short b_chan_event_srch_clear;
   short b_chan_event_srch_smooth;
   short b_chan_event_srch_log_file;
   short b_chan_event_summ_log_file;
   short b_rad_event_srch_clear;
   short b_rad_event_srch_log_file;
   short b_rad_event_summ_log_file;
   short s_export_station;
   short s_export_station_type;
   short s_export_measurement_type;
   short s_meas_summ_station;
   short s_meas_summ_station_type;
   short s_meas_summ_type;
   short b_meas_srch_clear;
   short s_meas_parm_station;
   short s_meas_parm_station_type;
   short s_raw_data_station;
   short s_raw_data_station_type;
   short b_integrity_flags[32];
   short b_raw_data_flags[32];
   short b_use_adjustments;
};

struct db_chan_event_dflt_rec {
   short s_chan_event_chan_num;
   short s_chan_event_sta_num;
   short s_chan_event_sta_type;
   short b_chan_event_srch_select;
   short b_chan_event_summ_select;
   double d_chan_event_srch_threshold;
   double d_chan_event_srch_background;
};

struct db_sta_dflt_rec {
   short s_sta_dflt_num_key;
   short s_sta_dflt_type;
   short b_meas_srch_select;
   short b_meas_make_new_status;
   short b_meas_low_bkg_check;
   short b_meas_high_bkg_check;
   short b_meas_term_on_moving;
   short b_meas_include_at_fail;
   short s_meas_analysis_type;
   short b_export_select;
   double d_meas_low_bkg_thres;
   double d_meas_high_bkg_thres;
   double d_meas_empty_thres;
   double d_meas_reals_thres;
   double d_meas_totals_thres;
   double d_meas_cf_thres;
   double d_meas_cf_norm_thres;
   double d_meas_at_thres;
   double d_meas_at_bias;
   double d_meas_at_sigma;
   double d_meas_min_len;
   double d_meas_max_len;
   double d_meas_gate_width;
   double d_adjustment;
   double d_slope;
   double d_intercept;
   double d_time;
   double d_timeprime;
   double d_meas_add_min;
   short b_adjust_time_select;
};

struct db_dir_rec {
   short b_dir_select[350];
   short s_dir_srch_from_chan_num[350];
   short s_dir_srch_to_chan_num[350];
   short s_dir_srch_from_sta_num[350];
   short s_dir_srch_to_sta_num[350];
   short s_dir_srch_from_sta_type[350];
   short s_dir_srch_to_sta_type[350];
   short b_dir_srch_type[350];
   short s_dir_srch_direction[350];
   short s_dir_srch_location[350];
   unsigned short us_dir_srch_min_secs[350];
   unsigned short us_dir_srch_max_secs[350];
   short s_dir_srch_ratio[350];
};

struct db_graph_dflt_rec {
   char sz_config_name_key[100];
   double d_display_offset;
   short b_zoom_x100;
   short b_zoom_auto_scale;
   short b_log_y_axis;
   short b_chans_overlapped;
   unsigned short us_zoom_factor;
   unsigned short us_view_period_index;
   short b_trace_active[10];
   unsigned short us_trace_station[10];
   unsigned short us_trace_channel[10];
   unsigned short us_trace_station_type[10];
};

struct db_chan_event_dflt_key {
   short s_chan_event_chan_num;
   short s_chan_event_sta_num;
   short s_chan_event_sta_type;
};

/* record, field and set table entry definitions */

/* File Id Constants */

/* Record Name Constants */
#define DB_DFLT_REC 10000
#define DB_CHAN_EVENT_DFLT_REC 10001
#define DB_STA_DFLT_REC 10002
#define DB_DIR_REC 10003
#define DB_GRAPH_DFLT_REC 10004

/* Field Name Constants */
#define S_DFLT_FACILITY 0L
#define S_DFLT_STATION 1L
#define S_DFLT_STATION_TYPE 2L
#define S_DFLT_CHANNEL 3L
#define S_INTEGRITY_STATION 4L
#define S_INTEGRITY_STATION_TYPE 5L
#define S_INTEGRITY_TOLERANCE 6L
#define S_DAYS_STATION 7L
#define S_DAYS_STATION_TYPE 8L
#define S_CHAN_EVENT_SRCH_WIDTH 9L
#define S_CHAN_EVENT_SRCH_ALG 10L
#define S_CHAN_EVENT_SUMM_SORT 11L
#define S_CHAN_EVENT_SUMM_TYPE 12L
#define S_CHAN_EVENT_SUMM_MIN 13L
#define S_CHAN_EVENT_SUMM_MAX 14L
#define S_RAD_EVENT_SRCH_PAIRS 15L
#define S_RAD_EVENT_SUMM_SORT 16L
#define S_RAD_EVENT_SUMM_TYPE 17L
#define S_MARK_SUMM_SORT 18L
#define S_MARK_SUMM_TYPE 19L
#define B_CHAN_EVENT_SRCH_CLEAR 20L
#define B_CHAN_EVENT_SRCH_SMOOTH 21L
#define B_CHAN_EVENT_SRCH_LOG_FILE 22L
#define B_CHAN_EVENT_SUMM_LOG_FILE 23L
#define B_RAD_EVENT_SRCH_CLEAR 24L
#define B_RAD_EVENT_SRCH_LOG_FILE 25L
#define B_RAD_EVENT_SUMM_LOG_FILE 26L
#define S_EXPORT_STATION 27L
#define S_EXPORT_STATION_TYPE 28L
#define S_EXPORT_MEASUREMENT_TYPE 29L
#define S_MEAS_SUMM_STATION 30L
#define S_MEAS_SUMM_STATION_TYPE 31L
#define S_MEAS_SUMM_TYPE 32L
#define B_MEAS_SRCH_CLEAR 33L
#define S_MEAS_PARM_STATION 34L
#define S_MEAS_PARM_STATION_TYPE 35L
#define S_RAW_DATA_STATION 36L
#define S_RAW_DATA_STATION_TYPE 37L
#define B_INTEGRITY_FLAGS 38L
#define B_RAW_DATA_FLAGS 39L
#define B_USE_ADJUSTMENTS 40L
#define S_CHAN_EVENT_CHAN_NUM 1000L
#define S_CHAN_EVENT_STA_NUM 1001L
#define S_CHAN_EVENT_STA_TYPE 1002L
#define B_CHAN_EVENT_SRCH_SELECT 1003L
#define B_CHAN_EVENT_SUMM_SELECT 1004L
#define D_CHAN_EVENT_SRCH_THRESHOLD 1005L
#define D_CHAN_EVENT_SRCH_BACKGROUND 1006L
#define DB_CHAN_EVENT_DFLT_KEY 1007L
#define S_STA_DFLT_NUM_KEY 2000L
#define S_STA_DFLT_TYPE 2001L
#define B_MEAS_SRCH_SELECT 2002L
#define B_MEAS_MAKE_NEW_STATUS 2003L
#define B_MEAS_LOW_BKG_CHECK 2004L
#define B_MEAS_HIGH_BKG_CHECK 2005L
#define B_MEAS_TERM_ON_MOVING 2006L
#define B_MEAS_INCLUDE_AT_FAIL 2007L
#define S_MEAS_ANALYSIS_TYPE 2008L
#define B_EXPORT_SELECT 2009L
#define D_MEAS_LOW_BKG_THRES 2010L
#define D_MEAS_HIGH_BKG_THRES 2011L
#define D_MEAS_EMPTY_THRES 2012L
#define D_MEAS_REALS_THRES 2013L
#define D_MEAS_TOTALS_THRES 2014L
#define D_MEAS_CF_THRES 2015L
#define D_MEAS_CF_NORM_THRES 2016L
#define D_MEAS_AT_THRES 2017L
#define D_MEAS_AT_BIAS 2018L
#define D_MEAS_AT_SIGMA 2019L
#define D_MEAS_MIN_LEN 2020L
#define D_MEAS_MAX_LEN 2021L
#define D_MEAS_GATE_WIDTH 2022L
#define D_ADJUSTMENT 2023L
#define D_SLOPE 2024L
#define D_INTERCEPT 2025L
#define D_TIME 2026L
#define D_TIMEPRIME 2027L
#define D_MEAS_ADD_MIN 2028L
#define B_ADJUST_TIME_SELECT 2029L
#define B_DIR_SELECT 3000L
#define S_DIR_SRCH_FROM_CHAN_NUM 3001L
#define S_DIR_SRCH_TO_CHAN_NUM 3002L
#define S_DIR_SRCH_FROM_STA_NUM 3003L
#define S_DIR_SRCH_TO_STA_NUM 3004L
#define S_DIR_SRCH_FROM_STA_TYPE 3005L
#define S_DIR_SRCH_TO_STA_TYPE 3006L
#define B_DIR_SRCH_TYPE 3007L
#define S_DIR_SRCH_DIRECTION 3008L
#define S_DIR_SRCH_LOCATION 3009L
#define US_DIR_SRCH_MIN_SECS 3010L
#define US_DIR_SRCH_MAX_SECS 3011L
#define S_DIR_SRCH_RATIO 3012L
#define SZ_CONFIG_NAME_KEY 4000L
#define D_DISPLAY_OFFSET 4001L
#define B_ZOOM_X100 4002L
#define B_ZOOM_AUTO_SCALE 4003L
#define B_LOG_Y_AXIS 4004L
#define B_CHANS_OVERLAPPED 4005L
#define US_ZOOM_FACTOR 4006L
#define US_VIEW_PERIOD_INDEX 4007L
#define B_TRACE_ACTIVE 4008L
#define US_TRACE_STATION 4009L
#define US_TRACE_CHANNEL 4010L
#define US_TRACE_STATION_TYPE 4011L

/* Set Name Constants */
#define DEFAULT_SET 20000
#define CHAN_DEFAULT_SET 20001
#define STA_DEFAULT_SET 20002
#define PAIR_DEFAULT_SET 20003
#define GRAPH_DEFAULT_SET 20004

/* Field Sizes */
#define SIZEOF_S_DFLT_FACILITY 2
#define SIZEOF_S_DFLT_STATION 2
#define SIZEOF_S_DFLT_STATION_TYPE 2
#define SIZEOF_S_DFLT_CHANNEL 2
#define SIZEOF_S_INTEGRITY_STATION 2
#define SIZEOF_S_INTEGRITY_STATION_TYPE 2
#define SIZEOF_S_INTEGRITY_TOLERANCE 2
#define SIZEOF_S_DAYS_STATION 2
#define SIZEOF_S_DAYS_STATION_TYPE 2
#define SIZEOF_S_CHAN_EVENT_SRCH_WIDTH 2
#define SIZEOF_S_CHAN_EVENT_SRCH_ALG 2
#define SIZEOF_S_CHAN_EVENT_SUMM_SORT 2
#define SIZEOF_S_CHAN_EVENT_SUMM_TYPE 2
#define SIZEOF_S_CHAN_EVENT_SUMM_MIN 2
#define SIZEOF_S_CHAN_EVENT_SUMM_MAX 2
#define SIZEOF_S_RAD_EVENT_SRCH_PAIRS 2
#define SIZEOF_S_RAD_EVENT_SUMM_SORT 2
#define SIZEOF_S_RAD_EVENT_SUMM_TYPE 2
#define SIZEOF_S_MARK_SUMM_SORT 2
#define SIZEOF_S_MARK_SUMM_TYPE 2
#define SIZEOF_B_CHAN_EVENT_SRCH_CLEAR 2
#define SIZEOF_B_CHAN_EVENT_SRCH_SMOOTH 2
#define SIZEOF_B_CHAN_EVENT_SRCH_LOG_FILE 2
#define SIZEOF_B_CHAN_EVENT_SUMM_LOG_FILE 2
#define SIZEOF_B_RAD_EVENT_SRCH_CLEAR 2
#define SIZEOF_B_RAD_EVENT_SRCH_LOG_FILE 2
#define SIZEOF_B_RAD_EVENT_SUMM_LOG_FILE 2
#define SIZEOF_S_EXPORT_STATION 2
#define SIZEOF_S_EXPORT_STATION_TYPE 2
#define SIZEOF_S_EXPORT_MEASUREMENT_TYPE 2
#define SIZEOF_S_MEAS_SUMM_STATION 2
#define SIZEOF_S_MEAS_SUMM_STATION_TYPE 2
#define SIZEOF_S_MEAS_SUMM_TYPE 2
#define SIZEOF_B_MEAS_SRCH_CLEAR 2
#define SIZEOF_S_MEAS_PARM_STATION 2
#define SIZEOF_S_MEAS_PARM_STATION_TYPE 2
#define SIZEOF_S_RAW_DATA_STATION 2
#define SIZEOF_S_RAW_DATA_STATION_TYPE 2
#define SIZEOF_B_INTEGRITY_FLAGS 64
#define SIZEOF_B_RAW_DATA_FLAGS 64
#define SIZEOF_B_USE_ADJUSTMENTS 2
#define SIZEOF_S_CHAN_EVENT_CHAN_NUM 2
#define SIZEOF_S_CHAN_EVENT_STA_NUM 2
#define SIZEOF_S_CHAN_EVENT_STA_TYPE 2
#define SIZEOF_B_CHAN_EVENT_SRCH_SELECT 2
#define SIZEOF_B_CHAN_EVENT_SUMM_SELECT 2
#define SIZEOF_D_CHAN_EVENT_SRCH_THRESHOLD 8
#define SIZEOF_D_CHAN_EVENT_SRCH_BACKGROUND 8
#define SIZEOF_S_STA_DFLT_NUM_KEY 2
#define SIZEOF_S_STA_DFLT_TYPE 2
#define SIZEOF_B_MEAS_SRCH_SELECT 2
#define SIZEOF_B_MEAS_MAKE_NEW_STATUS 2
#define SIZEOF_B_MEAS_LOW_BKG_CHECK 2
#define SIZEOF_B_MEAS_HIGH_BKG_CHECK 2
#define SIZEOF_B_MEAS_TERM_ON_MOVING 2
#define SIZEOF_B_MEAS_INCLUDE_AT_FAIL 2
#define SIZEOF_S_MEAS_ANALYSIS_TYPE 2
#define SIZEOF_B_EXPORT_SELECT 2
#define SIZEOF_D_MEAS_LOW_BKG_THRES 8
#define SIZEOF_D_MEAS_HIGH_BKG_THRES 8
#define SIZEOF_D_MEAS_EMPTY_THRES 8
#define SIZEOF_D_MEAS_REALS_THRES 8
#define SIZEOF_D_MEAS_TOTALS_THRES 8
#define SIZEOF_D_MEAS_CF_THRES 8
#define SIZEOF_D_MEAS_CF_NORM_THRES 8
#define SIZEOF_D_MEAS_AT_THRES 8
#define SIZEOF_D_MEAS_AT_BIAS 8
#define SIZEOF_D_MEAS_AT_SIGMA 8
#define SIZEOF_D_MEAS_MIN_LEN 8
#define SIZEOF_D_MEAS_MAX_LEN 8
#define SIZEOF_D_MEAS_GATE_WIDTH 8
#define SIZEOF_D_ADJUSTMENT 8
#define SIZEOF_D_SLOPE 8
#define SIZEOF_D_INTERCEPT 8
#define SIZEOF_D_TIME 8
#define SIZEOF_D_TIMEPRIME 8
#define SIZEOF_D_MEAS_ADD_MIN 8
#define SIZEOF_B_ADJUST_TIME_SELECT 2
#define SIZEOF_B_DIR_SELECT 700
#define SIZEOF_S_DIR_SRCH_FROM_CHAN_NUM 700
#define SIZEOF_S_DIR_SRCH_TO_CHAN_NUM 700
#define SIZEOF_S_DIR_SRCH_FROM_STA_NUM 700
#define SIZEOF_S_DIR_SRCH_TO_STA_NUM 700
#define SIZEOF_S_DIR_SRCH_FROM_STA_TYPE 700
#define SIZEOF_S_DIR_SRCH_TO_STA_TYPE 700
#define SIZEOF_B_DIR_SRCH_TYPE 700
#define SIZEOF_S_DIR_SRCH_DIRECTION 700
#define SIZEOF_S_DIR_SRCH_LOCATION 700
#define SIZEOF_US_DIR_SRCH_MIN_SECS 700
#define SIZEOF_US_DIR_SRCH_MAX_SECS 700
#define SIZEOF_S_DIR_SRCH_RATIO 700
#define SIZEOF_SZ_CONFIG_NAME_KEY 100
#define SIZEOF_D_DISPLAY_OFFSET 8
#define SIZEOF_B_ZOOM_X100 2
#define SIZEOF_B_ZOOM_AUTO_SCALE 2
#define SIZEOF_B_LOG_Y_AXIS 2
#define SIZEOF_B_CHANS_OVERLAPPED 2
#define SIZEOF_US_ZOOM_FACTOR 2
#define SIZEOF_US_VIEW_PERIOD_INDEX 2
#define SIZEOF_B_TRACE_ACTIVE 20
#define SIZEOF_US_TRACE_STATION 20
#define SIZEOF_US_TRACE_CHANNEL 20
#define SIZEOF_US_TRACE_STATION_TYPE 20

#endif    /* DB_DFLT_H */
