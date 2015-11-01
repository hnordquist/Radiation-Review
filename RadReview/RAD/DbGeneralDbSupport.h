
/* DbGeneralDbSupport.H  Function Prototypes */

#ifndef DbGeneralDbSupport_H
	#define DbGeneralDbSupport_H

	int InitializeDatabase(BOOL bPrompt);
	bool OpenDatabase(char *path, short sFacNum);
	int CloseDatabase();
	void GetDbName(char *szDbName);
	int SameDb(char *szDbName);
	BOOL SaveDatabaseNow(void);
	BOOL SwitchDatabases (char *szNewDbPath, short sFacNum, BOOL bMsg);
	BOOL CreateFacilityDatastore (char *szFacilityRootDir);




#endif
