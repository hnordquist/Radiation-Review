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
//    File Name : CopyDir.cpp     
//         Name : Shirley Klosterbuer
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"
#include "CopyDir.h"
#include "ProgressBars.h"
#include "Utilities.h"


#ifndef WIN32
	#include <dos.h>
#endif
#ifdef WIN32
	#include <stdio.h>
#endif



/*===========================================================================
 *
 *  Name	 :  CopyDirectory
 *
 *  Purpose	 :  Copy all files from the source directory to the destination
 *              directory.  Display progress using the progress bar.
 *
 *  Return value :  TRUE/FALSE
 *
 *  Special notes:
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:	gll_channel_name
 
 *  date    /	author	revision
 *  -----------------	--------
 *  07-Sep-1995	SFK		Created (based on SR Collect copy routine)
 *  03-Feb-1998 SFK		Modified for 32 bit
 *  08-Oct-1998 SFK		Removed 16-bit references, Made filenames plain strings.
 *	20-Jul-2001 SFK		Incorporated ProgressBarWithQuestion class
 *  02-May-2002	SFK		Removed terminating loop if find a file of size 0 -- just don't copy it
===========================================================================*/
int CopyDirectory(char *SourcePath, char *DestinationPath)
{
    unsigned long ulDirBytesCopied, ulTotalDirBytes;
	char szReadWildCard[DRIVE_LEN+PATH_LEN+WILDCARD_LEN+1];
	char szReadFile[DRIVE_LEN+PATH_LEN+DOSNAME_LEN+1];
	char szWriteFile[DRIVE_LEN+PATH_LEN+DOSNAME_LEN+1];
    CGFmtStr msg;  
    
	struct _finddata_t c_file;
	long lFileHandle;
	
    FILE *WriteStream, *ReadStream;
    int iNumRead, iNumWrote, iNumThisRead; 
    unsigned long ulTotalFileBytes;
    int iBufSize;
    char *pBuf;
    int iCopyStatus;
	int status;

	ProgressBarWithQuestion *pProgressBar = NULL;
	unsigned long ulTotalProgressCalls = 0;

	//pjm 26-Jan-2006 Changed from "bool" to "BOOL" to get rid of compiler warning
	BOOL bContinue;
    
	iCopyStatus = TRUE;        
   /* ------------------------------------------------------------------
	*   Calculate total bytes to be copied
	* ----------------------------------------------------------------*/
	ulDirBytesCopied = 0;
    GetDirSize(SourcePath,"*.*", &ulTotalDirBytes);  
    
   	strcpy(szReadWildCard, SourcePath);  
	strcat(szReadWildCard, "*.*");
			
   /* ------------------------------------------------------------------
	*	Allocate a big buffer for copying
	* ----------------------------------------------------------------*/
	iBufSize = 0x7fff;
	do 
	{
		iBufSize -= 0x03ff;
		pBuf = (char *)malloc(iBufSize);
	} while (!pBuf);	
	
	//	Create the progress bar
	ulTotalProgressCalls = ulTotalDirBytes/iBufSize + 1;
	msg.Printf("Copied %ld out of %ld bytes", ulDirBytesCopied, ulTotalDirBytes);
	pProgressBar = new ProgressBarWithQuestion(ulTotalProgressCalls, 1, msg, "Radiation Review: Copy Database");

   /* ------------------------------------------------------------------
	*	Find and copy the files.  Update the proogress bar with each
	*	buffer copied.
	*	Note:  files with size 0 are skipped.
	* ----------------------------------------------------------------*/
	lFileHandle = _findfirst(szReadWildCard, &c_file );
	while ((c_file.name[0] == '.') || (c_file.attrib == _A_SUBDIR)) _findnext(lFileHandle, &c_file); // skip past subdirectory names
    do {
		if (c_file.size > 0) {
			// open the file for reading
			strcpy(szReadFile, SourcePath);
			strcat(szReadFile, c_file.name);

			if ((ReadStream = fopen(szReadFile, "rb")) == NULL) 
			{
				iCopyStatus = FALSE;
			}
			else
			{
				ulTotalFileBytes = c_file.size;
				if (ulTotalFileBytes < (unsigned long)iBufSize) iBufSize = (int)ulTotalFileBytes;
					
				// open the file to write to 
				strcpy(szWriteFile, DestinationPath);
				strcat(szWriteFile, c_file.name);
				// delete the file if it already exists
				status = remove (szWriteFile);
				if ((WriteStream = fopen(szWriteFile, "wb")) != NULL) {
					iNumThisRead = iBufSize;

					// copy each file, buffer by buffer
					do 
					{
						// read and write a buffer, update all the counters
						iNumRead = fread(pBuf, sizeof(char), iNumThisRead, ReadStream);
						ulTotalFileBytes -= iNumRead;
						ulDirBytesCopied += iNumRead;
						iNumWrote = fwrite(pBuf, sizeof(char), iNumRead, WriteStream);
						if ((unsigned long)iNumThisRead > ulTotalFileBytes) iNumThisRead = (int)ulTotalFileBytes;

						// update the progress bar
						msg.Printf("Copied %ld out of %ld bytes", ulDirBytesCopied, ulTotalDirBytes);
						bContinue = pProgressBar->Progress_Bar_Update(msg);
							
					} while ((ulTotalFileBytes > 0) && bContinue);
					fclose(WriteStream);
					fclose(ReadStream);
				}
			}		
		}
	} while ((_findnext(lFileHandle, &c_file ) == 0 ) && bContinue && (iCopyStatus == TRUE));

	_findclose(lFileHandle);

	if (pBuf) 
	{
		free(pBuf);
		pBuf = NULL;
	}

	pProgressBar->Close();
	delete pProgressBar;
	return(bContinue);
}

	    