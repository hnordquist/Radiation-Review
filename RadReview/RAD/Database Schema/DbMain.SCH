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
/*  Scheme file for Radiation Monitoring database */

database db_main {

	data file       "db_main_fac.dbf"    contains db_fac_rec,db_sta_rec, db_chan_rec, system;

	data file       "db_main_day.dbf"    contains db_day_rec;

	data file[2560] "db_main_data.dbf"   contains db_float_data_rec;

	data file[4096] "db_main_datad.dbf"  contains db_double_data_rec;

	data file       "db_main_summ.dbf"   contains db_event_rec, db_dir_event_rec, db_mark_rec;

  
	key file        "db_main_fac.key"    contains s_fac_num_key, s_sta_num_key, s_chan_num_key;

	key file[4096]  "db_main_day.key"    contains d_day_beg_time_key, d_event_beg_time_key, d_dir_event_beg_time_key, d_mark_time_key;

	key file[4096]  "db_main_data.key"   contains d_fdata_beg_time_key;
	key file[4096]  "db_main_datad.key"  contains d_ddata_beg_time_key;



	record db_fac_rec {
	unique key short  s_fac_num_key;
		       short  s_fac_picklist_num;
		        char  sz_fac_name[40];
		        char  sz_fac_short_name[10];
	}

	record db_sta_rec {
	unique key short  s_sta_num_key;
		       short  s_sta_picklist_num;
		        char  sz_sta_name[40];
		        char  sz_sta_short_name[10];
		       short  s_sta_type;
		       short  s_data_type;
	}

	record db_chan_rec {
	unique key short  s_chan_num_key;
		       short  s_chan_picklist_num;
		        char  sz_chan_name[40];
		        char  sz_chan_short_name[10];            
		       short  s_chan_offset;
		        char  sz_chan_units[40];
		        char  sz_chan_short_units[10];
		       float  f_event_threshold;
	}   

	record db_day_rec {
	key        double  d_day_beg_time_key;
		       double  d_day_beg_time;
		       double  d_day_end_time;
		unsigned long  ul_day_total_pts;
	}

	record db_float_data_rec {
	key        double  d_fdata_beg_time_key;
		unsigned long  ul_fdata_pts_so_far;
		       double  d_fdata_time[50];
		unsigned long  ul_fdata_status[50];
		       double  d_fdata_etime[50];
		        float  f_data_chan1[50];
		        float  f_data_chan2[50];
		        float  f_data_chan3[50];
		        float  f_data_chan4[50];
		        float  f_data_chan5[50];
		        float  f_data_chan6[50];
		        float  f_data_chan7[50];
	}

	record db_double_data_rec {
	key        double  d_ddata_beg_time_key;
		unsigned long  ul_ddata_pts_so_far;
		       double  d_ddata_time[50];
		unsigned long  ul_ddata_status[50];
			   double  d_ddata_etime[50];
		       double  d_data_chan1[50];
		       double  d_data_chan2[50];
		       double  d_data_chan3[50];
		       double  d_data_chan4[50];
		       double  d_data_chan5[50];
		       double  d_data_chan6[50];
		       double  d_data_chan7[50];
	}

	record  db_event_rec {
	key			double  d_event_beg_time_key;
				double  d_event_end_time;
				double  d_event_max_time;
				double  d_event_grp_time;
				double  d_event_integral_sum;
				double  d_event_max_value;
				double  d_event_avg_reals_rate;
				double  d_event_avg_totals_rate;
		unsigned short  us_event_chan;
		unsigned short	us_event_sta;
		unsigned short	us_event_sta_type;
		unsigned short  us_event_type;
		unsigned short  us_event_status;
		unsigned short  us_event_type1;
		unsigned short  us_event_status1;
		unsigned short  us_event_type2;
		unsigned short  us_event_status2;
				 short  s_num_runs;
				 char   sz_event_id[14];
				 short  sdum[2];

	}


	record  db_dir_event_rec {
	key         double  d_dir_event_beg_time_key;
		        double  d_dir_event_end_time;
				double  d_dir_event_ratio_numerator;
				double  d_dir_event_ratio_denominator;
				double  d_dir_event_pair_int_sum[4];
				double  d_dir_event_pair_max_val_sum[4];
		unsigned short  us_dir_event_location;
		unsigned short  us_dir_event_direction;
		unsigned short  us_dir_event_status;
		unsigned short  us_dir_event_num_pairs;
		unsigned short  us_dir_event_from_chan[4];
		unsigned short  us_dir_event_to_chan[4];
		unsigned short  us_dir_event_from_sta[4];
		unsigned short  us_dir_event_to_sta[4];
		unsigned short  us_dir_event_sta[4];
		unsigned short  us_dir_event_to_sta_type[4];
		unsigned short  us_dir_event_from_sta_type[4];
				 short  s_dir_event_num_numerator;
				 short  s_dir_event_num_denominator;
	}

	record db_mark_rec {
	key double  d_mark_time_key;
		 short  s_mark_chan;
		 short	s_mark_sta;
		 short	s_mark_sta_type;
	}



	set facility_set {
		order ascending;
		owner system;
		member db_fac_rec by s_fac_num_key;
	}

	set facility_to_station_set {
		order ascending;
		owner db_fac_rec;
		member db_sta_rec by s_sta_num_key;
	}

	set station_to_channel_set {
		order ascending;
		owner db_sta_rec;
		member db_chan_rec by s_chan_num_key;
	}

	set station_to_day_set {
		order ascending;
		owner db_sta_rec;
		member db_day_rec by d_day_beg_time_key;
	}

	set day_to_float_data_set {
		order ascending;
		owner db_day_rec;
		member db_float_data_rec by d_fdata_beg_time_key;
	}


	set day_to_double_data_set {
		order ascending;
		owner db_day_rec;
		member db_double_data_rec by d_ddata_beg_time_key;
	}

	set day_to_event_set {
		order ascending;
		owner db_day_rec;
		member db_event_rec by d_event_beg_time_key;
	}

	set day_to_dir_event_set {
		order ascending;
		owner db_day_rec;
		member db_dir_event_rec by d_dir_event_beg_time_key;
	}

	set day_to_mark_set {
		order ascending;
		owner db_day_rec;
		member db_mark_rec by d_mark_time_key;
	}

}
