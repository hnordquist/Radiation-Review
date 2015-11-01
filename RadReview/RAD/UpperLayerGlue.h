
/* UpperLayerGlue.H  Function Prototypes */

#ifndef UpperLayerGlue_H
	#define UpperLayerGlue_H
	
	// RADInit Glue routines
	const struct GEN_DATE_STRUCT* GetStartDateFromRADInit(); 
	const struct GEN_DATE_STRUCT* GetEndDateFromRADInit();
	const struct GEN_TIME_STRUCT* GetStartTimeFromRADInit();
	const struct GEN_TIME_STRUCT* GetEndTimeFromRADInit();

	//RADImport Glue routines
	BOOL GetInImportModeFromRADImport();


#endif

