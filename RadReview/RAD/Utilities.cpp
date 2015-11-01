///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : Utilities.cpp     
//Primary Author: Shirley Klosterbuer

#include <direct.h>
#include <gen.h>
#include "RAD.H"    
#include "Utilities.h"

static int CopyIt(char *szNewPaths, char *szOrigPaths);

void OutputGUI_TxtWinTextLine(CGUI_TextWin *pWin, const char *szLine)
{
	static int iUpdate = 0;
	if (pWin != NULL) {
		iUpdate++;
		pWin->Printf("%s", szLine);
		if (iUpdate == 250) {	// process events every once in a while
			CGUI_App::ProcessEvents();
			iUpdate = 0;
		}
	}

}


 /*====================================================================
 *
 *  Function Name:		MyStrnCpy()
 *
 *  Function Purpose:	Performs standard strncpy and puts on trailing '\0'
 *
 *	Return value:		TRUE
 *						FALSE (Path too long)
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  19-Feb-1997 SFK 1.0 Created
 *	19-Oct-2005 SFK		Modified to do strcpy/strcat when there is room enough
 *
 *====================================================================*/
char *MyStrnCpy(char *pStr2, const char *pStr1, int iCount)
{
	char *Str1;
	if ((signed)strlen(pStr1) >= iCount) {		// can only copy part of the string
		Str1 = new char [iCount];
		strncpy(Str1, pStr1, (iCount-1));
		Str1[iCount-1] = '\0';
		(strcpy(pStr2, Str1));		
		delete Str1;
		return(pStr2);
	}	
	return(strcpy(pStr2, pStr1));
}	


char *MyStrnCat(char *pStr2, const char *pStr1, int iCount)
{
	char *Str1;
	if ((signed)strlen(pStr1) >= iCount) {		// can only concatenate part of the string
		Str1 = new char [iCount];
		MyStrnCpy(Str1, pStr1, (iCount));
		strncat(pStr2, Str1, iCount);
		delete Str1;
		return(pStr2);
	}	
	return(strcat(pStr2, pStr1));
}	

// 04-Jan-2005	Fix copy if original string longer than expected
char *MyStrnCpyAndPad(char *pStr2, const char *pStr1, int iCount, char cPad)
{
	int len;
	int i;

	len = strlen(pStr1);
	if (len < iCount) {
		strncpy(pStr2, pStr1, len);
		for (i=len; i<iCount; i++) {
			pStr2[i] = cPad;
		}
	}
	else {
		// 04-Jan-2005	SFK	string is longer than space allocated
		strncpy(pStr2, pStr1, iCount-1);
	}
	pStr2[iCount-1] = '\0';

	return(pStr2);
}	

 /*====================================================================
 *
 *  Function Name:		CheckPathsLen()
 *						CheckPathLen()
 *						CheckWildcardsLen()
 *						CheckNameLen()
 *
 *  Function Purpose:	If the given string is too long, return FALSE.
 *						If requested, output an	error message.
 *
 *	Return value:		TRUE
 *						FALSE (Path too long)
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  19-Feb-1997 SFK 1.0 Created
 *
 *====================================================================*/
int CheckPathsLen(char *szPaths, unsigned char bErrMsg)
{
	if (strlen(szPaths) > PATHS) {
		if (bErrMsg == TRUE) {
			RadReviewMsg(uiPATHS_TOO_LONG_ERR, szPaths);
		}
		return(FALSE);
	}
	return(TRUE);
}			


int CheckPathLen(char *szPaths, unsigned char bErrMsg)
{
	if (strlen(szPaths) > PATH_LEN) {
		if (bErrMsg == TRUE) {
			RadReviewMsg(uiPATH_TOO_LONG_ERR, szPaths);
		}
		return(FALSE);
	}
	return(TRUE);
}			

int CheckWildcardsLen(char *szWildcards, unsigned char bErrMsg)
{
	if (strlen(szWildcards) > WILDCARDS) {
		if (bErrMsg == TRUE) {
			RadReviewMsg(uiWILDCARDS_TOO_LONG_ERR, szWildcards);
		}
		return(FALSE);
	}
	return(TRUE);
}			

int CheckNameLen(char *szName, unsigned char bErrMsg)
{
	if (strlen(szName) > DOSNAME_LEN) {
		if (bErrMsg == TRUE) {
			RadReviewMsg(uiNAME_TOO_LONG_ERR, szName);
		}
		return(FALSE);
	}
	return(TRUE);
}			


 /*====================================================================
 *
 *  Function Name:		PathCopy()         Single path copy
 *						PathsCopy()        Multiple path copy
 *
 *  Function Purpose:	If the input path() is not too long,
 *						Copy the path(s) to the output path without spaces,
 *						removing a final ';' or ',' if it exists and
 *						adding an ending '\'.
 *
 *	Return value:		TRUE
 *						uiPATH_TOO_LONG_ERR (Path too long)
 *
 *	Special:			User must check path or paths length before calling
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  19-Feb-1997 SFK 1.0 Created
 *	22-Oct-1998	SFK		Modified to remove only leading and trailing spaces
 *						and to handle ',' as a terminator in addition to ';'
 *
 *====================================================================*/
static int CopyIt(char *szNewPaths, char *szOrigPaths)
{
	int len;
	int i;
	                                                            
	//	Strip out any leading spaces	
	i = 0;

	while (szOrigPaths[i] == ' ') i++;	// skip past leading spaces
	strcpy(szNewPaths, &szOrigPaths[i]);	// copy what is left
	len = strlen(szNewPaths);
	while (szNewPaths[len-1] == ' ') len--;	// skip past trailing spaces
	while ((szNewPaths[len-1] == ';') || (szNewPaths[len-1] == ',')) len--;		// strip off trailing ';' or ','
	szNewPaths[len] = '\0';		// if adjusted the length reapply terminator
			
   	//   Add a terminating \ if not already there
	if (szNewPaths[len-1] != '\\') {
		szNewPaths[len] = '\\';
		szNewPaths[len+1] = '\0';
	}
	return(TRUE);
}                        

int PathCopy(char *szNewPaths, char *szOrigPaths)
{
	if (CheckPathLen(szOrigPaths, TRUE) == FALSE) return(uiPATH_TOO_LONG_ERR);
	return(CopyIt(szNewPaths, szOrigPaths));
}	

int PathsCopy(char *szNewPaths, char *szOrigPaths)
{
	if (CheckPathsLen(szOrigPaths, TRUE) == FALSE) return(uiPATH_TOO_LONG_ERR);
	return(CopyIt(szNewPaths, szOrigPaths));
}	

int PathCat(char *szOrigPath, char *szPathToAdd, BOOL bErrMsg)
{
	if ((strlen(szOrigPath) + strlen(szPathToAdd) + 1) > PATHS) {
		if (bErrMsg == TRUE) {
			RadReviewMsg(uiPATHS_TOO_LONG_ERR, szOrigPath);
		}
		return(FALSE);
	}

	if ((szOrigPath[strlen(szOrigPath) -1] != ',') && (szOrigPath[strlen(szOrigPath) -1] != ';')) {
		strcat(szOrigPath, ";");
	}
	strcat(szOrigPath, szPathToAdd);
	return(TRUE);
}


int WildcardsCopy(char *szNewWildcards, char *szOrigWildcards)
{
	if (CheckWildcardsLen(szOrigWildcards, TRUE) == FALSE) return(uiWILDCARDS_TOO_LONG_ERR);
	CopyIt(szNewWildcards, szOrigWildcards);
	szNewWildcards[strlen(szNewWildcards) - 1] = '\0';	// overwrite the trailing / applied by CopyIt
	return(TRUE);
}	

/* <f> */
/*====================================================================
 *
 *  Function Name:		StripSpaces()
 *
 *  Function Purpose:	From a string strip any leading and trailing spaces.
 *
 *	Return value:		TRUE
 *						FALSE (Path too long to be copied)
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  02-Apr-1999 SFK 1.0 Created
 *
 *====================================================================*/
void StripSpaces(char *szOrigStr)
{
	CGStr szTemp;

	szTemp = szOrigStr;
	szTemp.StripLeadAndTrail();
	strcpy(szOrigStr,szTemp.Get_sz());
}

/* <f> */
/*====================================================================
 *
 *  Function Name:		StripPath()
 *
 *  Function Purpose:	From a string containing many path names, find
 *						and copy a single path name.   Strip any leading
 *						or trailing spaces but allow spaces in the name.
 *
 *	Return value:		TRUE
 *						FALSE (Path too long to be copied)
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  16-Jul-1998 SFK 1.0 Created
 *	21-Oct-1998	SFK		Changed to only strip lead only trailing and 
 *						leading spaces.  Changed to support either
 *						',' or ';' for a terminator.
 *
 *====================================================================*/
int StripPath(char *szMultiplePaths, char *szSinglePath, int *piLen, int *piIndex, int iMaxLenAllowed)
{
	int k;

	k = 0;

	if ((szMultiplePaths[*piIndex] == ',') || (szMultiplePaths[*piIndex] == ';') || 
		(szMultiplePaths[*piIndex] == '\0')) { // only have a terminator or empty path
		szSinglePath[k] = szMultiplePaths[*piIndex]; // copy terminator into single path
		if (szSinglePath[k] != '\0') szSinglePath[k+1] = '\0';
		*piLen = 0;
		(*piIndex)++;
		return(TRUE);
	}

	while ((szMultiplePaths[*piIndex] != ',') && (szMultiplePaths[*piIndex] != ';') && 
		   (szMultiplePaths[*piIndex] != '\0') && (k <= iMaxLenAllowed)) {
		while ((szMultiplePaths[*piIndex] == ' ') && (k == 0)) (*piIndex)++; // skip past leading spaces
		szSinglePath[k] = szMultiplePaths[*piIndex]; // copy into new single path
		k++;
		(*piIndex)++;
	}       
	(*piIndex)++;
	
	if (k > iMaxLenAllowed) return(FALSE);

	while ((k-1>0) && (szSinglePath[k-1] == ' ')) k--;	//strip trailing spaces

	// add a \ if not already there 
	if (szSinglePath[k-1] != '\\') {
		szSinglePath[k] = '\\';
		k++;
	}	   
	szSinglePath[k] = '\0';
	*piLen = k;		// return length of the path copied
	return(TRUE);
}

/* <f> */
/*====================================================================
 *
 *  Function Name:		WildcardValid()
 *
 *  Function Purpose:	Check if the specified string is a known extension.
 *
 *	Return value:		TRUE
 *						uiWILDCARD_DOES_NOT_EXIST (Path too long to be copied)
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  05-Aug-1998 SFK 1.0 Created
 *	21-Oct-1998	SFK		Changed to only strip lead only trailing and 
 *						leading spaces.  Changed to support either
 *						',' or ';' for a terminator.
 *  19-Jan-2000 SFK		Added *.bin type
 //	27-Oct-2003	SFK		Modified to use file extension of *.bny supported by MIC 1.805+
 // 16-May-2005	SFK		Added .vbf extension
 *
 *====================================================================*/
BOOL WildcardValid(char *szWildcard)
{
	// Customize for new various file extensions
	int i = 0;

	while ((szWildcard[i] != '.') && (szWildcard[i] != '\0'))
		i++;

	if (_stricmp(&szWildcard[i], ".bid") == 0) return(TRUE);
	if (_stricmp(&szWildcard[i], ".bsr") == 0) return(TRUE);
	if (_stricmp(&szWildcard[i], ".jsr") == 0) return(TRUE);
	if (_stricmp(&szWildcard[i], ".isr") == 0) return(TRUE);
	if (_stricmp(&szWildcard[i], ".hmr") == 0) return(TRUE);
	if (_stricmp(&szWildcard[i], ".mca") == 0) return(TRUE);
	if (_stricmp(&szWildcard[i], ".id") == 0) return(TRUE);
	if (_stricmp(&szWildcard[i], ".bot") == 0) return(TRUE);
	if (_stricmp(&szWildcard[i], ".bin") == 0) return(TRUE);
	if (_stricmp(&szWildcard[i], ".evt") == 0) return(TRUE);
	if (_stricmp(&szWildcard[i], ".bny") == 0) return(TRUE);
	if (_stricmp(&szWildcard[i], ".vbf") == 0) return(TRUE);
	//27November hn Add *.ess as a valid wildcard extension
	if (_stricmp(&szWildcard[i],".ess") == 0) return (TRUE);
	if (_stricmp(&szWildcard[i],".bi0") == 0) return (TRUE);
	if (_stricmp(&szWildcard[i], ".*") == 0) return(TRUE);
	//This uiWILDCARD_DOES_NOT_EXIST value is 1161.  
	//It certainly is not a BOOL.
	//The function signature says it is supposed to return a
	//BOOL.  ?????????
	return(uiWILDCARD_DOES_NOT_EXIST);
}

int StripWildcard(char *szMultipleWildcards, char *szSingleWildcard, int *piLen, int *piIndex, int iMaxLenAllowed)
{
	int k;

	k = 0;
	if ((szMultipleWildcards[*piIndex] == ',') || (szMultipleWildcards[*piIndex] == ';') || 
		(szMultipleWildcards[*piIndex] == '\0')) { // only have a terminator or empty path
		szSingleWildcard[k] = szMultipleWildcards[*piIndex]; // copy terminator into single wildcard
		if (szSingleWildcard[k] != '\0') szSingleWildcard[k+1] = '\0';
		*piLen = 0;
		(*piIndex)++;
		return(TRUE);
	}
	while ((szMultipleWildcards[*piIndex] != ',') && (szMultipleWildcards[*piIndex] != ';') &&
		   (szMultipleWildcards[*piIndex] != '\0') && (k <= iMaxLenAllowed)) {
		while ((szMultipleWildcards[*piIndex] == ' ') && (k == 0)) (*piIndex)++; // skip past leading spaces
		szSingleWildcard[k] = szMultipleWildcards[*piIndex]; // copy into new single wildcard
		k++;
		(*piIndex)++;
	}       
	(*piIndex)++;

	while ((k-1>0) && (szSingleWildcard[k-1] == ' ')) k--;	//strip trailing spaces
	
	if (k > iMaxLenAllowed) return(FALSE);

	szSingleWildcard[k] = '\0';
	*piLen = k;		// return length of the wildcard copied
	return(TRUE);
}

/* <f> */
/*====================================================================
 *
 *  Function Name:		CountPathsAndFiles()
 *
 *  Function Purpose:	Search through all the paths specifed and
 *						count the number of files matching the wildcard.
 *
 *	Return value:		TRUE
 *						FALSE (No files found)
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  19-Feb-1997 SFK 1.0 Created
 *	21-Oct-1998 SFK		Modified to use , or ; for separator, also only
 *						remove trailing and leading spaces.
 *
 *====================================================================*/
int CountPathsAndFiles(char *szPaths, char *szWildcard, int *piNumPaths, int *piNumFiles, int *piNumWildcards)
{
	int i, k;
	int iPathIndex, iWildcardIndex;
	char szPathToMatch[PATH_LEN+1];				/* path part of name */
	char szNameToMatch[PATH_LEN+1];				/* path part of name */
	char szWildcardToMatch[DOSNAME_LEN+1];
	int iNumMatchingFiles;
	int status = TRUE;
	BOOL bErrorPosted = FALSE;
	int iLen;
	
	*piNumPaths = 0;
	*piNumFiles = 0;
	*piNumWildcards = 0;

    //   Count how many paths are specified
	i = 0;
	if (szPaths[i] == '\0') { // blank entry
		RadReviewMsg(uiBLANK_STRING_ERR, "Path/Folder Specifier"); 
		return (uiBLANK_STRING_ERR);
	}
	while (szPaths[i] != '\0') {                            
		//while (szPaths[i] == ' ') i++;   // skip spaces
		while ((szPaths[i] != ';') && (szPaths[i] != ',') && (szPaths[i] != '\0')) i++; // look for path terminator
		if ((szPaths[i] == ';') || (szPaths[i] == ',')) i++;	// have ; or , so have a path
		(*piNumPaths)++;
	} ;

    //   Count how many wildcards (file types) are specified
	i = 0;
	if (szWildcard[i] == '\0') { // blank entry
		RadReviewMsg(uiBLANK_STRING_ERR, "File Type Specifier"); 
		return (uiBLANK_STRING_ERR);
	}
	while (szWildcard[i] != '\0') {                            
		//while (szWildcard[i] == ' ') i++;   // skip spaces
		while ((szWildcard[i] != ';') && (szWildcard[i] != ',') && (szWildcard[i] != '\0')) i++; // look for terminator
		if ((szWildcard[i] == ';') || (szWildcard[i] == ',')) i++;	// have ; or , so have a wildcard
		(*piNumWildcards)++;
	} ;

	//   Count how many files are in all the paths
	iWildcardIndex = 0;
	for (k=0; k<*piNumWildcards; k++) {
		status = StripWildcard(szWildcard, szWildcardToMatch, &iLen, &iWildcardIndex, DOSNAME_LEN);
		if (status != TRUE) { /* check that no path is too long */
			RadReviewMsg(uiNAME_TOO_LONG_ERR, szWildcard); 
			continue;  // skip this path
		}	
		if (iLen == 0) {
			(*piNumWildcards)--;	// don't count non-existent wildcard
			continue;	// handle case of two terminators in a row
		}
		
		// verify that the wildcard is a valid one
		status = WildcardValid(szWildcardToMatch);
		if (status != TRUE) {	    
    		RadReviewMsg(status, szWildcardToMatch);
    		(*piNumWildcards)--;	// don't count non-existent path
			bErrorPosted = TRUE;
    		continue;
		}

		iPathIndex = 0;

		for (i=0; i<*piNumPaths; i++) {
			/* get a path from the list of all paths */
			status = StripPath(szPaths, szPathToMatch, &iLen, &iPathIndex, PATH_LEN);
			if (status != TRUE) { /* check that no path is too long */
				RadReviewMsg(uiPATH_TOO_LONG_ERR, szPaths); 
				continue;  // skip this path
			}
			if (iLen == 0) {
	    		//RadReviewMsg(uiPATH_DOES_NOT_EXIST, szPathToMatch);
	    		(*piNumPaths)--;	// don't count non-existent path
				continue;	// handle case of two terminators in a row
			}
			
			// verify that the specified path exists 
			status = PathExists(szPathToMatch);
			if (status != TRUE) {	    
	    		RadReviewMsg(status, szPathToMatch);
	    		(*piNumPaths)--;	// don't count non-existent path
				bErrorPosted = TRUE;
	    		continue;
			}
			
			/* create whole wildcard name */
			strcpy(szNameToMatch, szPathToMatch);
			strcat(szNameToMatch, szWildcardToMatch);
			/* See if there are any matching files and add to total */
			gen_count_filenames(szNameToMatch, &iNumMatchingFiles);		
			*piNumFiles  = *piNumFiles + iNumMatchingFiles;
		}   
	}
	
	// found no path
    if (*piNumPaths == 0) {
		if (bErrorPosted == FALSE)
			RadReviewMsg(uiPATH_DOES_NOT_EXIST_ERR, szPaths);
	    return(uiPATH_DOES_NOT_EXIST_ERR);
	}	
	
		// found no files, put up an error msg if the directory was there
    if (*piNumFiles == 0) {
       	RadReviewMsg(uiNO_FILES_EXIST_WARN, szWildcard, szPaths);
	    return(uiNO_FILES_EXIST_WARN);
	}	
	return(TRUE);			
}	
		



/* <f> */
/*====================================================================
 *
 *  Function Name:		PathExists()
 *
 *  Function Purpose:	Check if the specified path exists.
 *
 *	Return value:		TRUE
 *						uiPATH_TOO_LONG_ERR
 *						uiDRIVE_DOES_NOT_EXIST_ERR
 *						uiPATH_DOES_NOT_EXIST_ERR
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  19-Feb-1997 SFK 1.0 Created
 *	08-Oct-1998 SFK		Removed 16-bit code
 *	21-Oct-1998 SFK		Changed to look first for whether the drive exists
 *						if it is specified.  Otherwise just take the path
 *						as is.  This fixed the problem of files being
 *						located on a network drive (\\kaz2\data).
 *
 *====================================================================*/
#include <dos.h>   		   
#include <stdlib.h>                              
#include <errno.h>
#include <io.h>
int PathExists(char *szPath)
{         

	char szTempPath[DRIVE_LEN+PATH_LEN+1];
	int dum;
	int status;
	char szCurWkgDir[DRIVE_LEN+PATH_LEN+1];	// debug

	
    status = PathCopy(szTempPath, szPath); 
    if (status != TRUE) return(status);
    
   /* ------------------------------------------------------------------
	*   Look for cases where a drive is specified in the path name.
	*	If the drive is specified, then check that it exists.
	*	Do this check so the message returned is about the drive, not the path.
	*	If the drive exists, then go on to check about the path.
	* ------------------------------------------------------------------*/
	_getcwd(szCurWkgDir, PATH_LEN);	//debug
	if (szTempPath[1] == ':') {
		if (DriveExists(szTempPath) != TRUE) return(uiDRIVE_DOES_NOT_EXIST_ERR);          
	}	
	_getcwd(szCurWkgDir, PATH_LEN);	//debug
 
   /* ------------------------------------------------------------------
	*   Look for the special case of '\' because access call won't work.
	* ------------------------------------------------------------------*/
	if ((szTempPath[0] == '\\') && (strlen(szTempPath) == 1)) return(TRUE);

   /* ------------------------------------------------------------------
	*   For all other cases, accept the string as is but remove the
	*	trailing \ for the access call.
	*	Determine whether the path exists by the access() call.
	* ------------------------------------------------------------------*/
	dum = strlen(szTempPath);
	if (szTempPath[dum-1] == '\\') {
		szTempPath[dum-1] = '\0';
	}
	
	_getcwd(szCurWkgDir, PATH_LEN);	//debug
	status = access(szTempPath,0);	
	_getcwd(szCurWkgDir, PATH_LEN);	//debug
	if (status == 0) {
		return(TRUE);
	}
	else return(uiPATH_DOES_NOT_EXIST_ERR);
}				


    	
/* <f> */
/*====================================================================
 *
 *  Function Name:		FileExists()
 *
 *  Function Purpose:	Check if the specified path exists.
 *
 *	Return value:		TRUE
 *						uiPATH_TOO_LONG_ERR
 *						uiNAME_TOO_LONG_ERR
 *						uiFILE_DOES_NOT_EXIST
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  19-Feb-1997 SFK 1.0 Created
 *
 *====================================================================*/
int FileExists(char *szPath, char *szFile)
{         

	char szTempName[DRIVE_LEN+PATH_LEN+DOSNAME_LEN+1];
	int status;
	
	if (CheckNameLen(szFile, TRUE) == FALSE) return (uiNAME_TOO_LONG_ERR);	                     
    status = PathCopy(szTempName, szPath); 
    if (status != TRUE) return(status);
	
	if (PathExists(szTempName) == TRUE) {
		strncat(szTempName, szFile, DOSNAME_LEN);	
		szTempName[DRIVE_LEN+PATH_LEN+DOSNAME_LEN] = '\0';
		if (access(szTempName,0) == 0) return(TRUE);
	}	
	return(uiFILE_DOES_NOT_EXIST_STATUS);
}				


/* <f> */
/*====================================================================
 *
 *  Function Name:		MakePath()
 *
 *  Function Purpose:	Create the specified path
 *
 *	Return value:		TRUE
 *						uiPATH_TOO_LONG_ERR
  *						uiDRIVE_DOES_NOT_EXIST_ERR
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  19-Feb-1997 SFK 1.0 Created
 *
 *====================================================================*/
#include <errno.h> 
int MakePath(char *szPath)
{    	    
 
 	int  i, j, k;
 	char szWorkingPath[DRIVE_LEN+PATH_LEN+3];
 	char szPathToMake[DRIVE_LEN+PATH_LEN+1];
 	char szCurrentPath[DRIVE_LEN+PATH_LEN+1];
 	int status;
	
    status = PathCopy(szWorkingPath, szPath); 
    if (status != TRUE) return(status);
		                                               
   /* ------------------------------------------------------------------
	*   Look for cases with just a drive
	* ------------------------------------------------------------------*/
	if (((szWorkingPath[1] == ':') && (strlen(szWorkingPath) == 2)) ||
		((szWorkingPath[1] == ':') && (szWorkingPath[2] == '\\') && (strlen(szWorkingPath) == 3))) {
		if (DriveExists(szWorkingPath) == TRUE) return(TRUE);
		return(uiDRIVE_DOES_NOT_EXIST_ERR);          
	}	
 	 	
   /* ------------------------------------------------------------------
	*   Figure out the drive.  If not specified assume the current drive.
	* ----------------------------------------------------------------*/
	if (szWorkingPath[1] == ':') {
		szPathToMake[0] = szWorkingPath[0];
		i = 2;
	}
	else {
		if (strlen(szWorkingPath) > (DRIVE_LEN+PATH_LEN-2)) return (uiPATH_TOO_LONG_ERR);	                     
		szPathToMake[0] = _getdrive() + 'A' - 1;
		i = 0;
	}
	szPathToMake[1] = ':';
	szPathToMake[2] = '\0';         
	
   /* ------------------------------------------------------------------
	*   Figure out the top path.  If not specified assume the current drive.
	* ----------------------------------------------------------------*/
	if ((szWorkingPath[i] == '\\') || (szWorkingPath[i] == '/')) {
		i++;	
	}
	else {
		getcwd(szCurrentPath, PATH_LEN);
		if ((strlen(szCurrentPath) + strlen(szPathToMake)) > PATH_LEN) return(uiPATH_TOO_LONG_ERR);
		strncpy(szPathToMake, szCurrentPath, PATH_LEN);
		szPathToMake[DRIVE_LEN+PATH_LEN] = '\0';
	}
	
	strcat(szPathToMake, "\\");
	
	if ((strlen(&szWorkingPath[i]) + strlen(szPathToMake)) > (DRIVE_LEN+PATH_LEN)) return(uiPATH_TOO_LONG_ERR);
	
	j = 0;
	k = i;
	do {
		if ((szWorkingPath[k] == '\\') || (szWorkingPath[k] == '/')) {
			if ((strlen(szPathToMake) + strlen(&szWorkingPath[i])) >= DRIVE_LEN+PATH_LEN) return(uiPATH_TOO_LONG_ERR); 
			strncat(szPathToMake, &szWorkingPath[i], j);
			szPathToMake[strlen(szPathToMake)+j-1] = '\0';
			if (PathExists(szPathToMake) != TRUE) {
				status = mkdir(szPathToMake);
				if (status != 0) {
					if (errno != EACCES) {
						return(uiMAKE_DIR_ERR);
					}	
				}	
			}
			i = k;
			j = 0;
		}
		j++;
		k++;
	} while (szWorkingPath[k] != '\0');		
	return(TRUE);
}	


/* <f> */
/*====================================================================
 *
 *     Function Name:	GetFileSize()
 *
 *  Function Purpose:	Return the size of specified file in the given
 *			directory.
 *
 *	Return value:	TRUE/FALSE
 *
 *  Revision History
 *
 *     DATE	  AUTHOR   VER REVISION
 *  ----------- ---------- --- --------
 *  06-May-1994 WJH	   2.0 Created
 *	07-Dec-1995 SFK		   Removed _ from function name	
 *	08-Oct-1998 SFK		   Removed 16-bit code
 *
 *====================================================================*/

int
GetFileSize(
char	*dir,
char	*fname,
long	*ptr_size)
{
	char complete_fname[PATH_LEN+DOSNAME_LEN+1];
	struct _finddata_t c_file;
	long lFileHandle;

#if defined(DEBUG1)
	debug_trace("get_file_size(dir = <%s>, fname = <%s>)\n", dir, fname);
#endif

    *ptr_size = 0;
	/* Build complete file name */
	gen_path_get(fname, dir, complete_fname);

	/* Get file info from DOS */
	lFileHandle = _findfirst(complete_fname, &c_file);
	_findclose(lFileHandle);
	if (lFileHandle != -1L) {
		*ptr_size = c_file.size;
		_findclose(lFileHandle);
		return (TRUE);
	} else {
		_findclose(lFileHandle);
		return (FALSE);
	}
}


/* <f> */
/*====================================================================
 *
 *     Function Name:	GetDirSize()
 *
 *  Function Purpose:	Return the size of all specified files in the given
 *			directory.
 *
 *	Return value:	TRUE/FALSE
 *
 *  Revision History
 *
 *     DATE	  AUTHOR   VER REVISION
 *  ----------- ---------- --- --------
 *	07-Dec-1995 SFK		   Created by adapting GetFileSize
 *	08-Oct-1998 SFK		   Removed 16-bit code
 *
 *====================================================================*/

int
GetDirSize(
char	*dir,
char	*fname,
unsigned long	*ptr_size)
{
	char complete_fname[PATH_LEN+DOSNAME_LEN+1];
	struct _finddata_t c_file;
	int status;
	long lFileHandle;

#if defined(DEBUG1)
	debug_trace("get_file_size(dir = <%s>, fname = <%s>)\n", dir, fname);
#endif
                  
    *ptr_size = 0;                  
	/* Build complete file name */
	gen_path_get(fname, dir, complete_fname);


	/* Get file info from DOS */
	lFileHandle = _findfirst(complete_fname, &c_file);
	if (lFileHandle != -1L) {	// -1 is the error condition
		*ptr_size = c_file.size;
	} else {
		_findclose(lFileHandle);
		return (FALSE);
	}

	status = _findnext(lFileHandle, &c_file);
	while (status == 0) {	// 0 says all okay
		*ptr_size += c_file.size;
		status = _findnext(lFileHandle, &c_file);
	}	
	_findclose(lFileHandle);
	return(TRUE);
}
	    
	    
/* <f> */
 /*====================================================================
 *
 *     Function Name:	GetDriveNumber()
 *
 *  Function Purpose:	Assumes the first two characters of the path
 *			are the drive specification, e.g. A:.
 *			If no drive spec, then returns current drive.
 *
 *	Return value:	Drive number (1 for A, 2 for B, etc)
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  11-Aug-1992 JAP     1.0 Created.
 *  21-Mar-1994 WJH     2.0 Added ':' test and default drive access.
 *	07-Dec-1995 SFK		   Removed _ from function name	
 *	08-Oct-1998 SFK		   Removed 16-bit code
 *
 *====================================================================*/
#include <ctype.h>
unsigned int
GetDriveNumber(
char	*dir)
{
	char drive_letter;
	unsigned drive_num;

	drive_letter = toupper(dir[0]);
	if (dir[1] == ':' && isupper(drive_letter))
		return (drive_letter - 'A' + 1);
	else {
		drive_num = _getdrive();
		return (drive_num);
	}
}


/* <f> */
 /*====================================================================
 *
 *     Function Name:	GetCurrentDrive(), SetCurrentDrive()
 *
 *  Function Purpose:	Finds the number associated with the current default drive.
 *
 *	Return value:	Drive number (1 for A, 2 for B, etc)
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  11-Aug-1992 JAP     1.0 Created.
 *  21-Mar-1994 WJH     2.0 Added ':' test and default drive access.
 *	07-Dec-1995 SFK		   Removed _ from function name	
 *
 *====================================================================*/
#include <direct.h>
int glliDriveNum;
char gllszCurrDir[256];

void GetCurrentDrive()
{
	glliDriveNum = _getdrive();
	 _getcwd(gllszCurrDir, 255);
}

void SetCurrentDrive()        
{
	_chdrive(glliDriveNum);
	_chdir(gllszCurrDir);
}	


/* <f> */
 /*====================================================================
 *
 *     Function Name:	GetDriveSize()
 *
 *  Function Purpose:	Determine the size (in bytes) of the drive and
 *			the size of the available space on that drive.
 *
 *	Return value:	File I/O status
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  09-May-1994 WJH     2.0 Created
 *  15-Dec-1994 WJH     2.0 Modified to catch critical errors.
 *	07-Dec-1995 SFK		    Removed _ from function name;  Removed critical error
 *							handling to use with Windows 3.1
 *	01-Jan-1998 SFK			Changed to use gen_disk_usage function for compatibility
 *							in 16 and 32 bit applications
 *	08-Oct-1998 SFK			Added check for directory name without a drive letter
 *
 *====================================================================*/

int
GetDriveSize(
char		*dir,
unsigned long	*ptr_size,
unsigned long	*ptr_avail)
{
	int status;
	double dPercentFree;
	int iCurrentDriveNumber;
	char cDriveLetter;

	if (dir[1] != ':') { // the directory name does not include a drive
		iCurrentDriveNumber = _getdrive();
		cDriveLetter = iCurrentDriveNumber + 'A' - 1;
	}
	else {
		cDriveLetter = dir[0];
	}
	status = gen_disk_usage(cDriveLetter, ptr_avail, &dPercentFree);
	if (status != GEN_SUCCESS) return(FALSE);

	*ptr_size =(long)(*ptr_avail/(dPercentFree * 100.0));

//	drive = GetDriveNumber(dir);
//	if (_dos_getdiskfree(drive, &diskspace) != 0) {
//		return (FALSE);
//	}

//	*ptr_avail = (unsigned long)diskspace.avail_clusters *
//		(unsigned long)diskspace.sectors_per_cluster *
//		(unsigned long)diskspace.bytes_per_sector;

//	*ptr_size = (unsigned long)diskspace.total_clusters *
//		(unsigned long)diskspace.sectors_per_cluster *
//		(unsigned long)diskspace.bytes_per_sector;

	return (TRUE);
}


/* <f> */
 /*====================================================================
 *
 *     Function Name:	DriveExists()
 *
 *  Function Purpose:	Determine if the specified drive exists
 *
 *	Return value:		TRUE/FALSE
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  06-Apr-1996 SFK 1.0 Created
 *	23-Dec-2004	SFK		Save and restore the current working directory on the current drive
 *
 *====================================================================*/

int
DriveExists(char *dir)
//#include <stdio.h>
//#include <conio.h>
//#include <direct.h>
//#include <stdlib.h>
{
    int status, drive, curdrive;
	char szCurWkgDir[PATH_LEN+1];
                                              
	status = TRUE;

	// Save the current working directory
	_getcwd(szCurWkgDir, PATH_LEN);
	                                          
    /* Save current drive. */
    curdrive = _getdrive();     
    
    /* If we can switch to the drive, it exists. */
    drive = GetDriveNumber(dir);
    if (_chdrive(drive)) status = FALSE; 
    
    /* Restore original drive. */
    _chdrive(curdrive);

	// Restore the current working directory
	_chdir(szCurWkgDir);
    return(status);
}



/* <f> */
 /*====================================================================
 *
 *     Function Name:	GetBasePath()
 *
 *  Function Purpose:	Determines which base path to use.  If running
 *						by itself where have a specified database as
 *						the working path, then don't need anything
 *						special.  If the working path is another tool,
 *						then must add ..\\RAD\\ to the beginning of
 *						any file name.
 *
 *	Return value:	none
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  13-Aug-1997 SFK     1.0 Created.
 *
 *====================================================================*/
#include <direct.h>

#define BASE_PATH "..\\RAD\\"

void GetBasePath(char *pBasePath)
{
	if (access("db_data.dbf",0) != 0) {
   		strcpy(pBasePath, BASE_PATH); 	//current dir is not db directory
   	}
   	else {
		strcpy(pBasePath, "");   		// current dir is db directory			
	}	

}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetDirectory
//
//	Description:
//	Given the Facility ID and type of directory to generate, get the facility root
//	directory from Facility Manager and then create the rest of the directory according
//	to the standard architecture.
//
//	Declaration:
//	void CFacilityConfig::GetChannelLongName(short sFacID, struct IDStruct *pID, char *szChanName, int iLen)
//
//	Input:	sFacID		Facility ID number
//			pID			IDStruct describing channel
//			iLen		Maximum space allocated by Rad for name
//
//	Output:	strChanName	Facility long name as a char *
//
//	Return:	None
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Sep-2003	SFK		Created
//////////////////////////////////////////////////////////////////
BOOL GetDirectory(const short sFacID, const int iType, char *pszPath, const int iLen)
{
	int iRealLen = iLen - 8;		// worst case we'll add 8 more chars config + 2 slashes

	strcpy(pszPath, "");

	pglFacCfg->GetFacilityDirectory(sFacID, pszPath, iRealLen);
	if (strlen(pszPath) > (PATH_LEN)) {
		RadReviewMsg(uiDEFAULT_CFG_PATH_TOO_LONG_ERR, pszPath, "");
		return(FALSE);
	}

	if ((pszPath[strlen(pszPath)-1] != '\\')) strcat(pszPath, "\\");
	
	if (iType == DATA_DIR) strcat(pszPath,"Data\\");
	if (iType == CONFIG_DIR) strcat(pszPath, "Config\\");
	if (iType == LOG_DIR) strcat(pszPath, "Logs\\");
	if (iType == ROOT_DIR) return(TRUE);

	return(TRUE);
}





