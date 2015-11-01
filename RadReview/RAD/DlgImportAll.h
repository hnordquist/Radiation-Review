
/* DlgImportAll.H  Function Prototypes */

#ifndef DlgImportAll_H
	#define DlgImportAll_H
	
	BOOL DetermineAllDirectories(char *pDlgPath, CGFmtStr *pAllPaths, unsigned int *puiNumPaths);
	void ImportAllMenuOption();
	BOOL ImportAllDialogBox();
	BOOL ImportAllDoIt(BOOL bClearIfRequested);

	
#endif
