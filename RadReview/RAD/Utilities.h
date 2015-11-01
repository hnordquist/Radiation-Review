
/* Utilities.H  Function Prototypes */

#ifndef Utilities_H
	#define Utilities_H
	
	void OutputGUI_TxtWinTextLine(CGUI_TextWin *pWin, const char *szLine);
	char *MyStrnCpy(char *pStr2, const char *pStr1, int iCount);
	char *MyStrnCat(char *pStr2, const char *pStr1, int iCount);
	char *MyStrnCpyAndPad(char *pStr2, const char *pStr1, int iCount, char cPad);
//	int CheckPathsLen(char *szPaths, unsigned char bErrMsg);
//	int CheckPathLen(char *szPaths, unsigned char bErrMsg);
//	int CheckWildcardsLen(char *szWildcards, unsigned char bErrMsg);
//	int CheckNameLen(char *szName, unsigned char bErrMsg);
	int PathCopy(char *szNewPaths, char *szOrigPaths);
	int PathsCopy(char *szNewPaths, char *szOrigPaths);
	int PathCat(char *szOrigPath, char *szPathToAdd, BOOL bErrMsg);
	int WildcardsCopy(char *szNewWildcards, char *szOrigWildcards);
	void StripSpaces(char *szOrigStr);
	int StripPath(char *szMultiplePaths, char *szSinglePath, int *piLen, int *piIndex, int iMaxLenAllowed);
//	BOOL WildcardValid(char *szWildcard);
	int StripWildcard(char *szMultipleWildcards, char *szSingleWildcard, int *piLen, int *piIndex, int iMaxLenAllowed);
	int CountPathsAndFiles(char *szPaths, char *szWildcard, int *piNumPaths, int *piNumFiles, int *piNumWildcards);
	int PathExists(char *szPath);
	int FileExists(char *szPath, char *szFile);
	int MakePath(char *szPath);
//	int GetFileSize(char *dir,char *fname,long	*ptr_size);
	int GetDirSize(char	*dir, char *fname, unsigned long *ptr_size);
//	unsigned int GetDriveNumber(char *dir);
	void GetCurrentDrive();
	void SetCurrentDrive();
	int GetDriveSize(char *dir, unsigned long *ptr_size, unsigned long *ptr_avail);
	int DriveExists(char *dir);
	void GetBasePath(char *pBasePath);
	BOOL GetDirectory(const short sFacID, const int iType, char *pszPath, const int iLen);

#endif
