#ifndef DB_OPER_H
#define DB_OPER_H

/* Raima Database Manager 4.5 [Build 17] */

/*  Scheme file for Operator Data database */
/*  Created 03-31-99*/


/* database db_oper record/key structure declarations */

struct db_oper_rec {
   char sz_op_item_id[41];
   unsigned long ul_op_id_time;
   short s_op_sta_num_key;
};

struct op_item_id_key {
   short s_op_sta_num_key;
   unsigned long ul_op_id_time;
};

/* record, field and set table entry definitions */

/* File Id Constants */

/* Record Name Constants */
#define DB_OPER_REC 10000

/* Field Name Constants */
#define SZ_OP_ITEM_ID 0L
#define UL_OP_ID_TIME 1L
#define S_OP_STA_NUM_KEY 2L
#define OP_ITEM_ID_KEY 3L

/* Set Name Constants */
#define DEFAULT_SET 20000

/* Field Sizes */
#define SIZEOF_SZ_OP_ITEM_ID 41
#define SIZEOF_UL_OP_ID_TIME 4
#define SIZEOF_S_OP_STA_NUM_KEY 2

#endif    /* DB_OPER_H */
