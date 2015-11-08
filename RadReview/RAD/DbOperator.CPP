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
///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbOperator.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////


/*
	This file contains the functions that access the operator data.
	For Radiation Review, operator data is used to determine the item
	ID associated with a station and time.

	Specifically the following functionality is present:
	
	FindOperatorRec - given the item id, station number and time,
		determine whether the record already exists in the database.

	CreateOperatorRec - given the item id, station number and time,
		after verifing that the record does not already exist in the database,
		create a record and add it to the database.

	ReadOperatorRec - given the station number, time interval, and tolerance, 
		find the id of the record that is closest to the conditions. 
		The algorithm for the search will be:
			While there are item ids to search
				Read an id
				If the station matches the specified station number then
					if the item time is within the specified interval then
						return this id
					else 
						back up in time until you find another id for this station
						if the time for this id is within the tolerance specified
						then return this id as the match
						else return no match found.
				end if
				read the next id
			end while

	DeleteOperatorRec - given the item id, station number and time, find the
		record in the database that matches it and delete it.

	GetNumberOfIdsInDb - given the station number (with -1 meaning all stations),
		determine the number of IDs in the database for the station.

	GetListOfIdsInDb - given the station number (with -1 meaning all station),
		create a list of the IDs that can be used in a picklist.
*/

#include "RAD.H"

/*===========================================================================
 *
 *  Name	 :  FindOperatorRec
 *
 *  Purpose	 :  Given the item id, station number and time,
 *				determine whether the record already exists in the database.
 *
 *  Return value :  TRUE - 		matching record exists
 *				    NO_MATCHING_REC - record not in DB
 *					other: 			unexpected DB error
 *
 *  Special notes:  
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  01-Apr-1999 SFK		Created
 *
===========================================================================*/
int FindOperatorRec(struct db_oper_rec *pdbOp)
{
    int db_stat;
	struct db_oper_rec dbOp;
	struct op_item_id_key dbOpCmpdKey;

	dbOpCmpdKey.s_op_sta_num_key = pdbOp->s_op_sta_num_key;
	dbOpCmpdKey.ul_op_id_time = pdbOp->ul_op_id_time;

    db_stat = d_keyfind(OP_ITEM_ID_KEY, (char *)&dbOpCmpdKey, OP_DB);
    if (db_stat == S_OKAY) { // found something that matches the time and station
		do {	// now check if the id matches
			d_recread ((char *)&dbOp, OP_DB);
			if (strnicmp(dbOp.sz_op_item_id, pdbOp->sz_op_item_id, ID_LEN) == 0) { // disregard case
				return(1); 
			}
			else {
				db_stat = d_keynext(OP_ITEM_ID_KEY, OP_DB);
			}
		} while (db_stat == S_OKAY);
	}
    if (db_stat == S_NOTFOUND) return(uiNO_MATCHING_OP_REC_STATUS);
    RadReviewMsg(uiDB_BAD_ERR, "FindFacRec", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}
      
      
/* <f> */
/*===========================================================================
 *
 *  Name	 :  CreateOperatorRec
 *
 *  Purpose	 :  Given the item id, station number and time,
 *				after verifing that the record does not already exist in the database,
 *				create a record and add it to the database.
 *
 *  Return value :	TRUE: new record inserted into db or already exists
 *					other: 	 unexpected DB error					
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  01-Apr-1999 SFK		Created
 *
===========================================================================*/
int CreateOperatorFacRec(struct db_oper_rec *pdbOp)
{
    int db_stat, status;
       
   /* ------------------------------------------------------------------
    *	If the record is already in the db do nothing.
    * ----------------------------------------------------------------*/
	status = FindOperatorRec(pdbOp);
	if (status != uiNO_MATCHING_OP_REC_STATUS) return(status);

   /* ------------------------------------------------------------------
    *	Assume the new record is filled and connect into the database
    * ----------------------------------------------------------------*/
    db_stat = d_fillnew(DB_OPER_REC,(char *)pdbOp, OP_DB);
    if (db_stat != S_OKAY) goto db_exit;
    db_stat = d_connect(DEFAULT_SET, OP_DB);
    if (db_stat != S_OKAY) goto db_exit;
    return(1);

db_exit:
    RadReviewMsg(uiDB_BAD_ERR, (const char *)"CreateOp", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}



/* <f> */
/*===========================================================================
 *
 *  Name	 :  ReadOperatorRec
 *
 *  Purpose	 :	Given the station number, time interval, and tolerance, 
 *				find the id of the record that is closest to the conditions. 
 *				The algorithm for the search will be:
 *				While there are item ids to search
 *					Read an id
 *					If the station matches the specified station number then
 *						if the item time is within the specified interval then
 *							return this id
 *						else 
 *							back up in time until you find another id for this station
 *							if the time for this id is within the tolerance specified
 *							then return this id as the match
 *							else return no match found.
 *					end if
 *					read the next id
 *				end while
 *
 *  Return value :  TRUE - Matching record located and read
 *					ui_NO_MATCHING_REC - Matching record could not be found
 *					other: unexpected DB error
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  01-Apr-1999 SFK		Created
 *
===========================================================================*/
int ReadOperatorRec(struct db_oper_rec *pdbOp, unsigned long ulEndTime, unsigned long ulTolerance)
{
    int status, db_stat;
	struct db_oper_rec dbOp;
        
   /* ------------------------------------------------------------------
    *	Check if an exact match exists
    * ----------------------------------------------------------------*/
    status = FindOperatorRec(pdbOp);
    if (status == 1) { // exact match found
		db_stat = d_recread(pdbOp, OP_DB);
		if (db_stat == S_OKAY) return(TRUE);
		else goto db_exit;
	}
	if (status != uiNO_MATCHING_OP_REC_STATUS) goto db_exit;
       
   /* ------------------------------------------------------------------
	*	There is not an exact match, check if there is a "close" one
    * ----------------------------------------------------------------*/
	db_stat = d_keynext(OP_ITEM_ID_KEY, OP_DB);	//move ahead to "lock" onto a rec
	db_stat = d_keyprev(OP_ITEM_ID_KEY, OP_DB);
	if (db_stat != S_OKAY) db_stat = d_keynext(OP_ITEM_ID_KEY, OP_DB); // if prev didn't lock, then move try move ahead
	if (db_stat != S_OKAY) return(uiNO_MATCHING_OP_REC_STATUS);
	db_stat = d_recread(&dbOp, OP_DB);	// read out the record that was found
	do {
		if (dbOp.s_op_sta_num_key == pdbOp->s_op_sta_num_key) { // this is the right station
			// first one checks if the id time is within the interval specified
			// second one checks if the id time is within the tolerance of the start time specified
			if (((dbOp.ul_op_id_time >= pdbOp->ul_op_id_time) && (dbOp.ul_op_id_time <= ulEndTime))  ||
				((dbOp.ul_op_id_time >= (pdbOp->ul_op_id_time - ulTolerance)) && (dbOp.ul_op_id_time <= pdbOp->ul_op_id_time)) ){
				d_recread(pdbOp, OP_DB);// this is the record
				return(TRUE);
			}
		}
		db_stat = d_keynext(OP_ITEM_ID_KEY, OP_DB);
		if (db_stat == S_NOTFOUND) return(uiNO_MATCHING_OP_REC_STATUS);
		db_stat = d_recread(&dbOp, OP_DB);	// read out the record that was found
	} while ((db_stat == S_OKAY) && (dbOp.ul_op_id_time <= ulEndTime));
	if (db_stat == S_OKAY) return (uiNO_MATCHING_OP_REC_STATUS);
    
db_exit:
    RadReviewMsg(uiDB_BAD_ERR, "ReadOperatorRec", db_stat);	// had unexpected db error if get here
    return(uiDB_BAD_ERR);
}                                         

