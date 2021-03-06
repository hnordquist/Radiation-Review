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
/*  Scheme file for Radiation Monitoring customized for VIFM data */

database db_vifm {


	data file       "db_vday.dbf"    contains system, db_sta_rec, db_day_rec;
	data file[12228] "db_vdata.dbf"   contains db_float_data_rec;

	key file[4096]  "db_vday.key"    contains d_day_beg_time_key, s_sta_num_key;
	key file[4096]  "db_vdata.key"   contains d_fdata_beg_time_key;


	record db_sta_rec {
	unique key short s_sta_num_key;
		short s_sta_picklist_num;
		char sz_sta_name[40];
		char sz_sta_short_name[10];
		short s_sta_type;
		short s_data_type;
	}

	record db_day_rec {
		key    double d_day_beg_time_key;
		double d_day_beg_time;
		double d_day_end_time;
		unsigned long ul_day_total_pts;
	}

	record db_float_data_rec {
	key double  d_fdata_beg_time_key;
		unsigned long  ul_fdata_pts_so_far;
		double d_fdata_time[234];
		unsigned long  ul_fdata_status[234];
		double d_fdata_etime[234];
		float          f_data_chan1[234];
		float          f_data_chan2[234];
		float          f_data_chan3[234];
		float          f_data_chan4[234];
		float          f_data_chan5[234];
		float          f_data_chan6[234];
		float          f_data_chan7[234];
		float          f_data_chan8[234];
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
