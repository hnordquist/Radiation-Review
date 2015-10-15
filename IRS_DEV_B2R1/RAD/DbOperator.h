
/* DbOperator.H  Function Prototypes */

#ifndef DbOperator_H
	#define DbOperator_H

	int CreateOperatorFacRec(struct db_oper_rec *pdbOp);
	int ReadOperatorRec(struct db_oper_rec *pdbOp, unsigned long ulEndTime, unsigned long ulTolerance);



#endif
