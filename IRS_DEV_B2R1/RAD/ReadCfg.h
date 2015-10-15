
/* ReadCfg.H  Function Prototypes */

#ifndef ReadCfg_H
	#define ReadCfg_H

	int ParseLine (char *szLine, FILE *pHandle, int *piNumLines);
	int OpenAndAccessDatabase(char *szDatabaseName, short sFacNum);
	int ReadCfgFile();
	BOOL GetDefaultFacilityNumber(short *psFacNum);
	BOOL SetDefaultFacilityNumber(short sFacNum);



#endif
