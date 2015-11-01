/*  Scheme file for Operator Data database */
/*  Created 03-31-99*/

database db_oper {

	data file       "db_oper.dbf"   contains db_oper_rec, system;

	key file        "db_oper.key"   contains op_item_id_key;

	record db_oper_rec {
	    char sz_op_item_id[41];
	    unsigned long ul_op_id_time;
	    short s_op_sta_num_key;
	    compound key op_item_id_key {
		s_op_sta_num_key;
		ul_op_id_time;
	    }
	}



	set default_set {
		order ascending;
		owner system;
		member db_oper_rec by ul_op_id_time;
	}


}
