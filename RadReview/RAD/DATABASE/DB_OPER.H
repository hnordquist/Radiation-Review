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
