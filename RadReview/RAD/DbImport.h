
/* DbImport.H  Function Prototypes */

#ifndef DbImport_H
	#define DbImport_H

	int ReadHeader(char *szFilename, short *psStaNum, int *piYr, int *piMon, int *piDay, FILE **pFileHandle, short *psStaType, double *pdVer, DATE *pdFirstRecordTime);
	int ImportData(char *szFilename, BOOL bOverwrite, CGUI_TextWin *win_ptr, char *szNameOnly, BOOL bSkipOutOfOrder);
	int ReadHeader(char *szFilename, struct IDStruct *pStaID, int *piYr, int *piMon, int *piDay, FILE **pFileHandle, double *pdVer, DATE *pdFirstRecordTime);

#endif
