
/* DlgFindMeas.H  Function Prototypes */

#ifndef DlgFindMeas_H
	#define DlgFindMeas_H
	
	void CleanUpDetermineMeasurements();
	void DetermineMeasurementsMenuOption();  // called from main menu;
	BOOL DetermineMeasurementsDoIt();  //called from auto import
	BOOL DetermineMeasurementsDialogBox();
	BOOL GetSRMeasurementParameters(struct db_sta_dflt_rec *pdbStaDflt);
	
#endif
