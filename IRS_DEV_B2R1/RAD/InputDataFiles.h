
/* InputDataFiles.H  Function Prototypes */

#ifndef InputDataFiles_H
	#define InputDataFiles_H
	
	int InputAllFiles(char *szOrigPaths, char *szOrigWildCards, BOOL bOverwrite, BOOL bSkipOutOfOrder);
	int InputTodaysFiles(char *szOrigPaths, char *szOrigWildCards, BOOL bOverwrite, BOOL bAllLastFiles);



#endif
