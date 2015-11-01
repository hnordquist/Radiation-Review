/*  Scheme file for Radiation Monitoring customized for VIFM data */

database db_rmon {


	data file       "db_day.dbf"    contains system, db_sta_rec, db_day_rec;
	data file[4096] "db_data.dbf"   contains db_float_data_rec;

	key file[4096]  "db_day.key"    contains d_day_beg_time_key, s_sta_num_key;
	key file[4096]  "db_data.key"   contains d_fdata_beg_time_key;


	record db_sta_rec {
	unique key short	s_sta_num_key;
		short			s_sta_picklist_num;
		char			sz_sta_name[40];
		char			sz_sta_short_name[10];
		short			s_sta_type;
		short			s_data_type;
	}

	record db_day_rec {
		key	double		d_day_beg_time_key;
		double			d_day_beg_time;
		double			d_day_end_time;
		unsigned long	ul_day_total_pts;
	}

	record db_float_data_rec {
	key double			d_fdata_beg_time_key;
		unsigned long	ul_fdata_pts_so_far;
		double			d_fdata_time[78];
		unsigned long	ul_fdata_status[78];
		double			d_fdata_etime[78];
		float			f_data_chan1[78];
		float			f_data_chan2[78];
		float			f_data_chan3[78];
		float			f_data_chan4[78];
		float			f_data_chan5[78];
		float			f_data_chan6[78];
		float			f_data_chan7[78];
		float			f_data_chan8[78];
	}


	set station_set {
		order ascending;
		owner system;
		member db_sta_rec by s_sta_num_key;
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

}
