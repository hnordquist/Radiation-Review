
/* DbTime.H  Function Prototypes */

#ifndef DbTime_H
	#define DbTime_H

	int FindDbDataRecContainingTime(struct IDStruct StaID, DATE dTime, short sType, struct db_float_data_rec *pdbfData);
	int FindDbDataRecContainingTime(struct IDStruct StaID, DATE dTime, short sType, struct db_double_data_rec *pdbdData);



#endif
