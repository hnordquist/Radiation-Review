/*
This work was supported by the United States Member State Support Program to IAEA Safeguards;
the U.S. Department of Energy, Office of Nonproliferation and National Security, International
Safeguards Division; and the U.S. Department of Energy, Office of Safeguards and Security.

LA-CC-14-040. This software was exported from the United States in accordance with the Export
Administration Regulations. Diversion contrary to U.S. law prohibited.

Copyright 2015, Los Alamos National Security, LLC. This software application and associated
material ("The Software") was prepared by the Los Alamos National Security, LLC. (LANS), under
Contract DE-AC52-06NA25396 with the U.S. Department of Energy (DOE). All rights in the software
application and associated material are reserved by DOE on behalf of the Government and LANS
pursuant to the contract.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or other materials provided
with the distribution.
3. Neither the name of the "Los Alamos National Security, LLC." nor the names of its contributors
may be used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS
NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRAT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/

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
