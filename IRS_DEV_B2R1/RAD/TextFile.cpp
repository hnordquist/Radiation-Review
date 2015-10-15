///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : TextFile.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include "Rad.h"
#include "TextFile.h"

// Creates the class and opens the specified file
TextFile::TextFile()
{
	mbOpen = FALSE;
	mpFileHandle = NULL;
	mstrFilename = "";
}

// If the file is not closed, close it.
TextFile::~TextFile()
{
	CloseTextFile();
}

			
//	Open the specified text file.  If requested by bAskFile,
//	allow the user to change the filename of the file being opened
BOOL TextFile::OpenTextFile(const char *szFilename, BOOL bAskFile)
{
	CGStr NewFileName;
	BOOL bAppend; 
	BOOL bStat;

	const char *aszTypes[] = {"Text Files (*.txt)", "*.txt",
					          "All Files (*.*)", "*.*",
							   NULL, NULL};

	if (mpFileHandle != NULL) fclose (mpFileHandle);		// nothing should be open, verify it
	mbOpen = FALSE;
	mbCancel = FALSE;
	mstrFilename = "";
		
	//	Give user the option of changing the filename to open.
	//	Use standard GUI to get the filename
	if (bAskFile) {
		bStat = GUI_FileSaveAs(NULL, szFilename, &NewFileName, aszTypes, " Select Text File", TRUE, &bAppend);
		if (bStat == TRUE) {
			if (bAppend == TRUE) {
       			mpFileHandle = fopen(NewFileName, "at");
			}
			else {
				mpFileHandle = fopen(NewFileName, "wt");
			} 
				
			if (mpFileHandle == NULL) {
				RadReviewMsg(uiFILE_OPEN_ERR, (char *)NewFileName);
			}
			else {
				mbOpen = TRUE;
				mstrFilename = NewFileName;
			}
		}
		else {
			mbCancel = TRUE;
		}
	}
	else {
		mpFileHandle = fopen(szFilename, "wt");
		if (mpFileHandle == NULL) {
			RadReviewMsg(uiFILE_OPEN_ERR, (char *)szFilename);
		}
		else {
			mbOpen = TRUE;
			mstrFilename = szFilename;
		}
	}
	return(mbOpen);
}
	

// Set the file pointer to the beginning
BOOL TextFile::ClearTextFile()
{
	BOOL bStat = FALSE;;

	if (!mbOpen) bStat = OpenTextFile(mstrFilename);
	if (bStat) {
		if (mpFileHandle != NULL) {
			rewind(mpFileHandle);
		}
	}
	return(mbOpen);
}

// Write text string to the file
BOOL TextFile::WriteLineToTextFile(const char *szLine)
{
	BOOL bStat = FALSE;

	if (!mbOpen) bStat = OpenTextFile(mstrFilename);
	if (mbOpen) bStat = fwrite(szLine, strlen(szLine), 1, mpFileHandle);
	return(bStat);
}

// Flush the file
void TextFile::FlushTextFile()
{
	if (mpFileHandle) {
		fflush(mpFileHandle);
	}
}


// Close the file
void TextFile::CloseTextFile()
{
	if (mpFileHandle) {
		fclose(mpFileHandle);
		mpFileHandle = NULL;
		mbOpen = FALSE;
		mstrFilename = "";
	}
}


// Return the open status
BOOL TextFile::GetOpenStatus()
{
	return(mbOpen);
}

// Return the open status
BOOL TextFile::GetCancelStatus()
{
	return(mbCancel);
}

//Return the filename
const char *TextFile::GetFilename()
{
	return(mstrFilename);
}
