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
/*  Scheme file for Radiation Monitoring default parameters database */
/*  Changed 01-12-99 to add graph_dflt_rec*/
/*  Extensive mods Sep 2003 to support new facility manager */
/*  5-5-2004 Save graph trace channel and station rather than track picklist position */
/*  Changed December 2005 by SK to add d_meas_add_min for SCR00191 */


database db_dflt {

	data file       "db_dflt.dbf"   contains db_dflt_rec, db_chan_event_dflt_rec, db_sta_dflt_rec, db_dir_rec, db_graph_dflt_rec, system;

	key file        "db_dflt.key"   contains db_chan_event_dflt_key, s_sta_dflt_num_key, sz_config_name_key;


	record db_dflt_rec {
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

	}

	record db_chan_event_dflt_rec {
		short s_chan_event_chan_num;
		short s_chan_event_sta_num;
		short s_chan_event_sta_type;
		short b_chan_event_srch_select;
		short b_chan_event_summ_select;
		double d_chan_event_srch_threshold;
		double d_chan_event_srch_background;
	
		compound unique key db_chan_event_dflt_key {
		s_chan_event_chan_num;
		s_chan_event_sta_num;
		s_chan_event_sta_type;
		}
	}
	
	record db_sta_dflt_rec {
	unique key short s_sta_dflt_num_key;
		short  s_sta_dflt_type;
		short  b_meas_srch_select;
		short  b_meas_make_new_status;
		short  b_meas_low_bkg_check;
		short  b_meas_high_bkg_check;
		short  b_meas_term_on_moving;
		short  b_meas_include_at_fail;
		short  s_meas_analysis_type;
		short  b_export_select;
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
		short  b_adjust_time_select;
	}

	record db_dir_rec {
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
	}

	record db_graph_dflt_rec {
	    key char sz_config_name_key[100];
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
	}



	set default_set {
		order last;
		owner system;
		member db_dflt_rec;
	}

	set chan_default_set {
		order last;
		owner system;
		member db_chan_event_dflt_rec;
	}

	set sta_default_set {
		order last;
		owner system;
		member db_sta_dflt_rec;
	}

	set pair_default_set {
		order last;
		owner system;
		member db_dir_rec;
	}

	set graph_default_set {
		order last;
		owner system;
		member db_graph_dflt_rec;
	}

}
