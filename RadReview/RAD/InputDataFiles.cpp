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
///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : InputDataFiles.cpp     
//Primary Author: Shirley Klosterbuer

//19 Apr 2006 - pjm
//Incorporated SK's DR00271 fix
//7 Feb 2007 - pjm
//Incorporated HN's DR00423 fix

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//	InputAllFiles()
//	InputTodaysFiles()
//
///////////////////////////////////////////////////////////////////////////

#include <dos.h>
#include <time.h>  
#include "GetFile.h"     
#include "RAD.H"    
#include "DbDay.h"
#include "DbGeneralDbSupport.h"
#include "DbImport.h"
#include "DbLimit.h"
#include "FacilityConfig.h"
#include "ProgressBars.h"
#include "RadInit.H"
#include "Utilities.h"
#include "MyDateTime.h"


extern RADInit *pRAD_Init;  
extern BOOL glImportExit;
extern short glsFacNum;
extern CMyDateTime glMyDateTime;
                       
CGUI_TextWin *glImportWinPtr = NULL;  // global so can also be used with import selected
                
static RESOURCE_ID gllCancelBtnHit;

static BOOL CloseImportLogWindow(CGUI_TextWin *pWin);
                                            
                                          

/* 	Allows user to close the LogWindow by clicking on the upper left minus */
static BOOL CloseImportLogWindow(CGUI_TextWin *pWin)
{

	BOOL bCanClose = TRUE;
	glImportWinPtr = NULL;
	return(bCanClose);
}                                                  


       
/*===========================================================================
 *
 *  Name	 : 	InputAllFiles
 *
 *  Purpose	 :	Input all the files indicated by the drive, path and filename
 *				into the database (filename may be a szWildCard)
 *
 *  Return value :  None
 *
 *  Special notes:
 *
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  05-Jun-1995 SFK		Created from old GRAND Review input.c          
 *	01-Sep-1995 SFK	    Modified to import from multiple paths
 *	17-Aug-1998 SFK		Modified to import from multiple wildcards
 *	08-Oct-1998 SFK		Removed 16-bit code
 //	27-Oct-2003	SFK		Modified to use file extension of *.bny supported by MIC 1.805+
 *
===========================================================================*/
int InputAllFiles(char *szOrigPaths, char *szOrigWildCards, BOOL bOverwrite, BOOL bSkipOutOfOrder)
{
    int  i,j,m;
	int	 iPathIndex, iWildcardIndex;
    char szNameToMatch[PATH_LEN+DOSNAME_LEN+1]; 	/* string of name to search for */
	char szWildCardToMatch[DOSNAME_LEN+1];
    char szPathToMatch[PATH_LEN+1];				/* path part of name */
    char szDbPath[PATH_LEN+1];
    int  iTotalMatchingFiles = 0;   /* file names in directory */
    long lStartSecs;	    /* clock reading when start importing files */
    long lCurSecs;	    	/* clock reading after read in current file */
    int  iCurrentFile;
    int  status = TRUE;
    CGFmtStr Msg, Msg1, Msg2;
	long lTotalSize;
	int iNumPaths; 
	char szTempPaths[PATHS+1];
	char szWildCards[WILDCARDS+1];
	int iNumFiles, iFilesRequested, iNumWildcards;  
	unsigned int uiDum;
	char (*pFiles)[DOSNAME_LEN+1] = NULL;
	struct _finddata_t file;
	long lFileHandle;
	int iLen;
	long lCurSecs1;
	char szDataRoot[PATHS+1];
                          
    static BOOL first_time = TRUE;

   /* ------------------------------------------------------------------
	*   Check the length of the arguments, abort if too long
	* ----------------------------------------------------------------*/
	status = WildcardsCopy(szWildCards, szOrigWildCards);
	if (status != TRUE) return(status);
	status = PathsCopy(szTempPaths, szOrigPaths);
	if (status != TRUE) return(status);

   /* ------------------------------------------------------------------
	*   If there is a wildcard of *.id specified, then add the
	*	common shared file directory to the list for the search
	* ----------------------------------------------------------------*/
	if (strstr(_strlwr(szWildCards), "*.id") != 0) { // id files are a special case
		if (!GetDirectory(glsFacNum, DATA_DIR, szDataRoot, PATHS+1)) return(FALSE);
		status = PathCat(szTempPaths, szDataRoot, TRUE);
		//status = PathCat(szTempPaths, (char *)pRAD_Init->Get_Common_Database_Path(), TRUE);
		if (status != TRUE) return(status);
	}

   /* ------------------------------------------------------------------
	*   Find out how many paths given in the paths field, returns error
	*	if any single filename too long.
	* ----------------------------------------------------------------*/
	status = CountPathsAndFiles(szTempPaths, szWildCards, &iNumPaths, &iTotalMatchingFiles, &iNumWildcards);
	if (status != TRUE) return(FALSE);
		 
   /* ------------------------------------------------------------------
    *	Ready to import data from specified files. 
    *	Start timing how long this takes.
    * ----------------------------------------------------------------*/
    time(&lStartSecs);  
    time(&lCurSecs);                   
                
	if (glImportWinPtr == NULL) {                
		glImportWinPtr = new CGUI_TextWin("Data Import Log for All Files");
		if (glImportWinPtr != NULL) glImportWinPtr->ShowStatusBar(TRUE);
	}   
	else {
		glImportWinPtr->Clear();
		glImportWinPtr->SetTitle("Data Import Log for All Files");
	}
	glImportWinPtr->SetFocus();		

	// output database location once
	GetDbName(szDbPath);
	glImportWinPtr->Printf("Database Directory = %s", szDbPath);

   	//  Create the progress bar
	TwoLineProgressBar *pTwoLineProgressBar;
	BOOL bContinue = true;
	Msg1.Printf("");
	Msg2.Printf("");
	// use iTotalMatchingFiles*2 since call update at two parts of the process
	pTwoLineProgressBar = new TwoLineProgressBar(iTotalMatchingFiles*2, 1, Msg1, Msg2, "Radiation Review:  File Import Progress");
	if (pTwoLineProgressBar) {

	   /* ------------------------------------------------------------------
		*	Import those files that have been selected.
		* ----------------------------------------------------------------*/
		iCurrentFile = 0; 
		lTotalSize = 0;                                                  
		iWildcardIndex = 0;
		while ((status == TRUE) && (iCurrentFile != iTotalMatchingFiles) && bContinue) {
			j = 0;
			do {
				status = StripWildcard(szWildCards, szWildCardToMatch, &iLen, &iWildcardIndex, DOSNAME_LEN);
				if (status != TRUE) continue;
				if (iLen == 0) continue;	// handle case of two terminators in a row

				iPathIndex = 0;
				i = 0;
				do {
				
					/* Get a path by coping characters up to the next ';' */
					status = StripPath(szTempPaths, szPathToMatch, &iLen, &iPathIndex, PATH_LEN);
					if (status != TRUE) continue;
					if (iLen == 0) continue;	// handle case of two terminators in a row

												 
					/* Add the wildcard name to the path */
					MyStrnCpy(szNameToMatch, szPathToMatch, PATH_LEN);
					MyStrnCat(szNameToMatch, szWildCardToMatch, PATH_LEN+DOSNAME_LEN);
						
					/* check that you can access files on the drive */
					lFileHandle = _findfirst("*.*", &file); /* get all file info */
					_findclose(lFileHandle);
					if (lFileHandle == -1L) 
					{
						if (glImportWinPtr != NULL) glImportWinPtr->Printf("\nImport Error Reading File %s", szNameToMatch);
						if (glImportWinPtr != NULL) glImportWinPtr->Printf("\nImport Aborted Due to System Error.");
						bContinue = false;
						break;
					}

					/* count matching filenames in this path */
					gen_count_filenames(szNameToMatch, &iNumFiles);
					if (iNumFiles == 0) 
					{
						glImportWinPtr->Printf("\n\nImport Path = %s.  No %s files found.", szPathToMatch, szWildCardToMatch);
						i++;
						continue;
					}	

					// Print the header for these files to import										                
					glImportWinPtr->Printf("\n\nImport Path = %s", szPathToMatch);
					if (_stricmp(szWildCardToMatch, "*.id") == 0) { // id files don't have a date
						glImportWinPtr->Printf("\nFilename");			    	
					}

					else if ((_stricmp(szWildCardToMatch, "*.bot") == 0) ||
							 (_stricmp(szWildCardToMatch, "*.bid") == 0) || (_stricmp(szWildCardToMatch, "*.bi0") == 0) ||
							 (_stricmp(szWildCardToMatch, "*.bny") == 0) ||
							 (_stricmp(szWildCardToMatch, "*.evt") == 0)) 
					{// ||
							 // November 27 hn Add *.ess here
							 //(_stricmp (szWildCardToMatch, "*.ess") == 0))		{ // all Coms display times in ms
							
						glImportWinPtr->Printf("\nDate                        Station  Filename        First Rec       Last Rec     Num Recs");			    
					}
					else if ((_stricmp(szWildCardToMatch, "*sl*.*") == 0) ||
							 (_stricmp(szWildCardToMatch, "*.vbf") == 0)) 
					{ // VIFM Com displays date/times
							glImportWinPtr->Printf("\nDate                        Station  Filename    First Rec                Last Rec                 Num Recs");			    
					}

					else if ((_stricmp(szWildCardToMatch, "*.ess") == 0))
					{ // EOSS Com displays date/times
							glImportWinPtr->Printf("\nDate                        Station  Filename");			    
					}

					else 
					{
						glImportWinPtr->Printf("\nDate                        Station  Filename       First Rec   Last Rec   Num Recs");			    
					}


					// Determine number of filenames that can be handled by
					// available memory, reducing number if necessary.
					if (pFiles != NULL) free(pFiles);    
					pFiles = NULL;
					unsigned long ulAllocAmount;
					ulAllocAmount = (unsigned long)iNumFiles * (unsigned long)(DOSNAME_LEN+1);
					if (ulAllocAmount > 65535) {
						iFilesRequested = iNumFiles;
						iNumFiles = (unsigned int)65535/(unsigned int)(DOSNAME_LEN+1) - 1;
						Msg.Printf("Cannot allocate memory for %d files;\n will process only the first %d files.", iFilesRequested, iNumFiles);
						GUI_MsgBox(Msg, GUI_ICON_INFO);
						glImportWinPtr->Printf("\nOut of memory,\n will process only the first %d of files requested.", iNumFiles, iFilesRequested);
					}	
					// Then allocate storage for filenames in this path
					pFiles = (char (*)[DOSNAME_LEN+1])malloc(iNumFiles*(DOSNAME_LEN+1)) ;
					while ((pFiles == NULL) && (iNumFiles > 50)) {
						iNumFiles -= 50;
						pFiles = (char (*)[DOSNAME_LEN+1])malloc(iNumFiles*(DOSNAME_LEN+1)) ;
					}	
					if (iNumFiles <= 0) {
						RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
						bContinue = false;
					}	
					// Read in the filenames by 0=alphabetical order since GRAND Collect alters dates
					gen_get_filenames(szNameToMatch, iNumFiles, 0, pFiles, &uiDum);
					
					m = 0;
					do {
					
					   /* ------------------------------------------------------------------
						*	Form the filename and inform user which file being updated
						* ----------------------------------------------------------------*/
						MyStrnCpy(szNameToMatch,szPathToMatch, PATH_LEN);
						MyStrnCat(szNameToMatch, pFiles[m], PATH_LEN+DOSNAME_LEN);

						Msg1.Printf("Processing file %s.\n", szNameToMatch);
						bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
						lFileHandle = _findfirst(szNameToMatch, &file); /* get all file info */
						_findclose(lFileHandle);
						if (lFileHandle == -1L) {
							if (glImportWinPtr != NULL) glImportWinPtr->Printf("\nImport Error Reading File %s", szNameToMatch);
							if (glImportWinPtr != NULL) glImportWinPtr->Printf("\nImport Aborted Due to System Error.");
							bContinue = false;
							break;
						}
					   /* ------------------------------------------------------------------
						*	Actually import the data file
						* ----------------------------------------------------------------*/
//						MessageBox (NULL, "calling import data from rad", "heather's debug message", MB_OK|MB_ICONWARNING);
						status = ImportData(szNameToMatch, bOverwrite, glImportWinPtr, pFiles[m], bSkipOutOfOrder);
						iCurrentFile++;							
					   /* ------------------------------------------------------------------
						*	Data from this file successfully imported. Update the
						*	total import time so far on the status line and continue importing.
						* ----------------------------------------------------------------*/
						lTotalSize += file.size;
						lCurSecs1 = lCurSecs;
						time(&lCurSecs);
						Msg2.Printf("Total import time for %d files (%ld Kb) is %ld secs\n",iCurrentFile, (lTotalSize/1000), (lCurSecs-lStartSecs));
						bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);

						m++;
					} while ((m<iNumFiles) && bContinue); 
					i++;
				} while ((i<iNumPaths) && bContinue);	
				j++;
			} while ((j<iNumWildcards) && bContinue);
		}
		pTwoLineProgressBar->Close();
		delete pTwoLineProgressBar;

		glImportWinPtr->Printf("\n\nTotal import time for %d files (%ld Kb) is %ld secs\n",iCurrentFile, (lTotalSize/1024), (lCurSecs-lStartSecs));                      
		if (pFiles != NULL) free(pFiles);
		pFiles = NULL;
		if (glImportWinPtr != NULL) glImportWinPtr->EnableUserClose(CloseImportLogWindow);
		
	}	   	
	if (!bContinue) return CANCELED;
	return(TRUE);
}


extern int MapCheckSigResultWRV(int csr, bool lenient = false);

       
/*===========================================================================
 *
 *  Name	 : 	InputTodaysFiles
 *
 *  Purpose	 :	Input the files for the current day indicated by the drive and path(s).
 *
 *  Return value :  None
 *
 *  Special notes:
 *
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  16-Dec-1996 SFK		Created from ImportAllFold GRAND Review input.c          
 *	01-Sep-1995 SFK	    Modified to import from multiple paths
 *	17-Aug-1998 SFK		Modified to import from multiple wildcards
 *	27-Oct-2003	SFK		Modified to use file extension of *.bny supported by MIC 1.805+
 *
===========================================================================*/
int InputTodaysFiles(char *szOrigPaths, char *szOrigWildCards, BOOL bOverwrite, BOOL bAllLastFiles)
{
    
    int i, j, k;
	int iYr, iMon, iDay;
	FILE *pFile = NULL;
	int iWildCardIndex, iPathIndex;
    char szNameToMatch[PATH_LEN+DOSNAME_LEN+1]; 	/* string of name to search for */
    char szPathToMatch[PATH_LEN+1];				/* path part of name */
	char szWildCardToMatch[DOSNAME_LEN+1];
    char szDbPath[PATH_LEN+1];
    int  status = TRUE;
    CGFmtStr Msg, Msg1, Msg2;
	int iNumPaths, iNumFiles, iFilesRequested, iNumWildcards, iTotalMatchingFiles; 
	char szTempPaths[PATHS+1]; 
	char szWildCards[WILDCARDS+1];
	unsigned int uiDum;
	char (*pFiles)[DOSNAME_LEN+1] = NULL;
	int iLen;
	DATE dFileDay, dLastTime, dDum;
	char szDateStr[DT_LEN+1];
    long lStartSecs;	    /* clock reading when start importing files */
    long lCurSecs;	    	/* clock reading after read in current file */
	int iNumImported;
	long lFileHandle;
	struct _finddata_t file;
	struct IDStruct StaID;
	char szDataRoot[PATHS+1];
                          
    static BOOL first_time = TRUE;

   /* ------------------------------------------------------------------
	*   Check the length of the arguments, abort if too long
	* ----------------------------------------------------------------*/
	status = WildcardsCopy(szWildCards, szOrigWildCards);
	if (status != TRUE) return(status);
	status = PathsCopy(szTempPaths, szOrigPaths);
	if (status != TRUE) return(status);

   /* ------------------------------------------------------------------
	*   If there is a wildcard of *.id specified, then add the
	*	common shared file directory to the list for the search
	* ----------------------------------------------------------------*/
	if (strstr(_strlwr(szWildCards), "*.id") != 0) { // id files are a special case
		if (!GetDirectory(glsFacNum, DATA_DIR, szDataRoot, PATHS+1)) return(FALSE);
		status = PathCat(szTempPaths, szDataRoot, TRUE);
		//status = PathCat(szTempPaths, (char *)pRAD_Init->Get_Common_Database_Path(), TRUE);
		if (status != TRUE) return(status);
	}

   /* ------------------------------------------------------------------
	*   See if there are any files in any of the paths
	* ----------------------------------------------------------------*/
	status = CountPathsAndFiles(szTempPaths, szWildCards, &iNumPaths, &iTotalMatchingFiles, &iNumWildcards);
	if (status != TRUE) return(FALSE);

	// open the output page	
	if (glImportWinPtr == NULL) {                
		glImportWinPtr = new CGUI_TextWin("Data Import Log for Latest File(s)");
		if (glImportWinPtr != NULL) glImportWinPtr->ShowStatusBar(TRUE);
	}   
	else {
		glImportWinPtr->Clear();
		glImportWinPtr->SetTitle("Data Import Log for Latest Files");
	}	
	glImportWinPtr->SetFocus();		

	// print database directory once
	GetDbName(szDbPath);
	glImportWinPtr->Printf("Database Directory = %s", szDbPath);

   /* ------------------------------------------------------------------
    *	Ready to import data from specified files. 
    *	Start timing how long this takes.
    * ----------------------------------------------------------------*/
    time(&lStartSecs);  
    time(&lCurSecs);                   
                
	gllCancelBtnHit = 0;
   	iYr = 0;
	iMon = 0;
	iDay = 0;

   	//  Create the progress bar
	TwoLineProgressBar *pTwoLineProgressBar;
	BOOL bContinue = true;
	Msg1.Printf("");
	Msg2.Printf("");
	// DR00271 - Use iTotalMatchingFiles*2 since call update at two parts of the process
	pTwoLineProgressBar = new TwoLineProgressBar(iTotalMatchingFiles*2, 1, Msg1, Msg2, "Radiation Review:  File Import Progress");
	if (pTwoLineProgressBar) {
		iNumImported = 0;

		// Search through the paths and wildcards, importing data from the last file in each path	
		iWildCardIndex = 0;
		j = 0;
		do {
			if (gllCancelBtnHit == IDCANCEL) break; // cancel button was hit
			status = StripWildcard(szWildCards, szWildCardToMatch, &iLen, &iWildCardIndex, DOSNAME_LEN);
			if (status != TRUE) continue;
			if (iLen == 0) continue;	// handle case of two terminators in a row
			iPathIndex = 0;
			i = 0;
			do {
				if (gllCancelBtnHit == IDCANCEL) break; // cancel button was hit
						
				// Get a path by coping characters up to the next ;
				status = StripPath(szTempPaths, szPathToMatch, &iLen, &iPathIndex, PATH_LEN);
				if (status != TRUE) continue;
				if (iLen == 0) continue;	// handle case of two terminators in a row
				
				/* create whole szWildCard name */
				MyStrnCpy(szNameToMatch, szPathToMatch, PATH_LEN);
				MyStrnCat(szNameToMatch, szWildCardToMatch, PATH_LEN+DOSNAME_LEN);
				gen_count_filenames(szNameToMatch, &iNumFiles);		
				if (iNumFiles == 0) {
					glImportWinPtr->Printf("\n\nImport Path = %s.  No %s files found.", szPathToMatch, szWildCardToMatch);
					i++;
					continue;
				}	

				// print the results header													
				glImportWinPtr->Printf("\n\nImport Path = %s", szPathToMatch);
				if (_stricmp(szWildCardToMatch, "*.id") == 0) { // id files don't have a date
					glImportWinPtr->Printf("\nFilename");			    	
				}
				else if ((_stricmp(szWildCardToMatch, "*.bot") == 0) ||
						 (_stricmp(szWildCardToMatch, "*.bid") == 0) || (_stricmp(szWildCardToMatch, "*.bi0") == 0) ||
						 (_stricmp(szWildCardToMatch, "*.bny") == 0) ||
						 (_stricmp(szWildCardToMatch, "*.evt") == 0)) { // all Coms display times in ms
						glImportWinPtr->Printf("\nDate                        Station  Filename       First Rec       Last Rec       Num Recs");			    
				}
				else if ((_stricmp(szWildCardToMatch, "*sl*.*") == 0) ||
						 (_stricmp(szWildCardToMatch, "*sl*.*") == 0)) { // VIFM Com displays date/times
						glImportWinPtr->Printf("\nDate                        Station     Filename	First Rec              Last Rec            Num Recs");
						
				}
				else if ((_stricmp(szWildCardToMatch, ".ess") == 0)){
						glImportWinPtr->Printf("\nDate                        Station     Filename    Seal IDs");
				}

				else {
					glImportWinPtr->Printf("\nDate                        Station  Filename       First Rec   Last Rec   Num Recs");			    
				}

				// Determine number of filenames that can be handled by
				// available memory, reducing number if necessary.
				if (pFiles != NULL) free(pFiles);    
				pFiles = NULL;
				unsigned long ulAllocAmount;
				ulAllocAmount = (unsigned long)iNumFiles * (unsigned long)(DOSNAME_LEN+1);
				if (ulAllocAmount > 65535) {
					iFilesRequested = iNumFiles;
					iNumFiles = (unsigned int)65535/(unsigned int)(DOSNAME_LEN+1) - 1;
					Msg.Printf("Cannot allocate memory for %d files;\n will search only the first %d files.", iFilesRequested, iNumFiles);
					GUI_MsgBox(Msg, GUI_ICON_INFO);
					glImportWinPtr->Printf("\nOut of memory,\n will process only the first %d of files requested.", iNumFiles, iFilesRequested);
				}	
				// Then allocate storage for filenames in this path
				pFiles = (char (*)[DOSNAME_LEN+1])malloc(iNumFiles*(DOSNAME_LEN+1)) ;
				while ((pFiles == NULL) && (iNumFiles > 50)) {
					iNumFiles -= 50;
					pFiles = (char (*)[DOSNAME_LEN+1])malloc(iNumFiles*(DOSNAME_LEN+1)) ;
				}	
				if (iNumFiles <= 0) {
					RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
					bContinue = false;
					break;
				}	

				// Read in the filenames by 0=alphabetical order since GRAND Collect alters dates
				gen_get_filenames(szNameToMatch, iNumFiles, 0, pFiles, &uiDum);

				if (bAllLastFiles) {
					//	If requested import files for all days since the last day in the database
					//	Always include the last day
					
					k = 0;
					do {
;
		    			MyStrnCpy(szNameToMatch, szPathToMatch, PATH_LEN);
		    			MyStrnCat(szNameToMatch, pFiles[k], PATH_LEN+DOSNAME_LEN);

						// handle the special case of *.id files, always read them
						if (strstr(szNameToMatch, ".id") != 0) {
							status = ImportData(szNameToMatch, bOverwrite, glImportWinPtr, pFiles[k], FALSE);
							continue;
						}


						// read the header to figure out which day this file is from
						status = ReadHeader(szNameToMatch, &StaID, &iYr, &iMon, &iDay, &pFile, &dDum, &dDum);
						if (pFile != NULL) fclose(pFile);
						switch (MapCheckSigResultWRV(status, true))  // using the lenient interpretation of various errors here
						{
						case 0:
							break;
						case 1:
							bContinue = true;
							break;
						case uiUNKNOWN_CA:
							{
								glImportWinPtr->Printf("\nFile %s signed by an unknown CA.", szNameToMatch);
								bContinue = true;
								break;
							}
						case uiINVALID_SIGNATURE:
							{
								glImportWinPtr->Printf("\nFile %s has an invalid signature.", szNameToMatch);
								bContinue = true;
								break;
							}
						case uiLIBRARY_ERROR:
							{
								glImportWinPtr->Printf("\nCheckSignature library error reading %s.", szNameToMatch);
								bContinue = true;
								break;
							}
						case uiNO_INPUT_FILE:
							{
								glImportWinPtr->Printf("\nFile %s does not exist.", szNameToMatch);
								bContinue = false;
								break;
							}
						case uiEMPTY_INPUT_FILE:
							{
								glImportWinPtr->Printf("\nFile %s is empty.", szNameToMatch);
								bContinue = false;
								break;
							}
						case uiIO_ERROR:
							{
								glImportWinPtr->Printf("\nIO error reading file %s.", szNameToMatch);
								bContinue = false;
								break;
							}
						case uiFILE_INVALID:
							{
								glImportWinPtr->Printf("\nFile %s is invalid.", szNameToMatch);
								bContinue = false;
								break;
							}

						default: 
							{						
								glImportWinPtr->Printf("\nImport Error Reading File Header for %s", szNameToMatch);
								// investigate the error further to see if something is wrong with the drive
								lFileHandle = _findfirst(szNameToMatch, &file); /* get all file info */
								_findclose(lFileHandle);
								if (lFileHandle == -1L) {
									Msg.Printf("A serious error was encountered with file %s.  The import will abort.");
									if (glImportWinPtr != NULL) glImportWinPtr->Printf("\nImport Aborted Due to System Error.");
									bContinue = false;
									break;
								}
								iNumImported++; 
								k++;
								continue;
							}
						}
						sprintf(szDateStr,"%02d.%02d.%02d", iYr, iMon, iDay);          
						//dFileDay = Date.DateTimeStrsToDATETimestamp(szDateStr, "00:00:00");
						dFileDay = glMyDateTime.DateTimeStrsToDATETimestamp(szDateStr, "00:00:00");
						
						// 14-Mar-2006 SFK Move line to update which file we are processing to
						//				where the user can see it (before was only update when
						//				we were actually importing data)
						// Part of fix for DR00271 
						Msg1.Printf("Processing file %s.\n", szNameToMatch);
						bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
						
						// determine the last data in the database for station indicated by the header
						status = GetDbTimeRange(StaID, &dDum, &dLastTime);

						//Fix for DR00423  HN and PJM 7-feb-07
						//if ((status == TRUE) || (status == uiDB_EMPTY_STATUS) || (status == uiNO_DATA_FOR_STATION_WARN)) {

							if (status != TRUE) 
								dLastTime = 0;

							if ((dFileDay >= dLastTime) || ((int)dLastTime == (int)dFileDay)) 
							{ // this file's day is after the last day in the database
        						status = ImportData(szNameToMatch, bOverwrite, glImportWinPtr, pFiles[k], FALSE);
								iNumImported++; 
							}


						   /* ------------------------------------------------------------------
							*	Data from this file successfully imported. Update the
							*	total import time so far on the status line and continue importing.
							* ----------------------------------------------------------------*/
							time(&lCurSecs);
							Msg2.Printf("Total import time for %d files is %ld secs\n",iNumImported, (lCurSecs-lStartSecs));
							bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
						//}
						k++;
					} while ((k<iNumFiles) && bContinue);
				}
				else {
					/* create whole filename of last file in list */
					MyStrnCpy(szNameToMatch, szPathToMatch, PATH_LEN);
					MyStrnCat(szNameToMatch, pFiles[iNumFiles-1], PATH_LEN+DOSNAME_LEN);
					// import data from last file in list
					status = ImportData(szNameToMatch, bOverwrite, glImportWinPtr, pFiles[iNumFiles-1], FALSE);
					iNumImported++;     
				   /* ------------------------------------------------------------------
					*	Data from this file successfully imported. Update the
					*	total import time so far on the status line and continue importing.
					* ----------------------------------------------------------------*/
					time(&lCurSecs);
					Msg2.Printf("Total import time for %d files is %ld secs\n",iNumImported, (lCurSecs-lStartSecs));
					bContinue = pTwoLineProgressBar->TwoLineProgressBarUpdate(Msg1, Msg2);
				}

				if (pFiles != NULL) free(pFiles);
				pFiles = NULL;
				i++;
			} while ((i<iNumPaths) && bContinue);	
			j++;
		} while ((j<iNumWildcards) && bContinue);

		pTwoLineProgressBar->Close();
		delete pTwoLineProgressBar;

		glImportWinPtr->Printf("\n\nTotal import time for %d files is %ld secs\n",iNumImported, (lCurSecs-lStartSecs));                      
		if (pFiles != NULL) free(pFiles);
		pFiles = NULL;
		if (glImportWinPtr != NULL) glImportWinPtr->EnableUserClose(CloseImportLogWindow);
	}	   	
	if (!bContinue) return CANCELED;
	return(TRUE);
}
