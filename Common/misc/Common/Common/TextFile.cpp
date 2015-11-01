///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001-2005, The Regents of the University of California.  This
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
//	18-Oct-2005 SFK Adapted from GUILib version created for Rad Review
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TextFile.h"
#include <afxdlgs.h>

// Creates the class and opens the specified file
TextFile::TextFile()
{
	mbOpen = false;
	mbCancel = false;
	mstrFilename.Empty();
}

// If the file is not closed, close it.
TextFile::~TextFile()
{
	CloseTextFile();
}

			
//	Open the specified text file.  If requested by bAskFile,
//	allow the user to change the filename of the file being opened
bool TextFile::OpenTextFile(CString Filename, bool bAskFile, bool bWrite)
{
	CString NewFileName;

	char strFilter[] = { "TXT Files (*.txt)|*.txt|All Files (*.*)|*.*||" };

	if (!mbOpen) {
		CFileDialog FileDlg(true, "*.txt", Filename, 0, strFilter);

		//const char *aszTypes[] = {"Text Files (*.txt)", "*.txt", "All Files (*.*)", "*.*",   NULL, NULL};

		//if (mpFileHandle != NULL) fclose (mpFileHandle);		// nothing should be open, verify it
		mbOpen = FALSE;
		mbCancel = FALSE;
		mstrFilename = "";
			
		//	Give user the option of changing the filename to open.
		//	Use standard GUI to get the filename
		if (bAskFile) {
			if( FileDlg.DoModal() == IDOK )	{
				if (File.Open(FileDlg.GetFileName(), CFile::modeCreate | CFile::modeWrite | CFile::typeText)) {
					mbOpen = true;
					mstrFilename = FileDlg.GetPathName();		// get the file along with the path
				}
			}
			else mbCancel = true;
		}
		else {
			if (bWrite) {
				if (File.Open(Filename, CFile::modeCreate | CFile::modeWrite)) mbOpen = true;
			}
			else {
				if (File.Open(Filename, CFile::modeRead)) mbOpen = true;		//	17-Jan-2006 SFK Added opening only for reading
			}
		}
	}
	return(true);
}


// Write text string to the file
bool TextFile::WriteLineToTextFile(CString strString)
{
	bool bStat = FALSE;

	if (!mbOpen) bStat = OpenTextFile(mstrFilename);
	if (mbOpen) {
		File.WriteString(strString);
	}
	return(true);
}


// Flush the file
void TextFile::FlushTextFile()
{
	if (mbOpen) {
		File.Flush();
	}
}


// Close the file
void TextFile::CloseTextFile()
{
	if (mbOpen) {
		File.Close();
		mbOpen = false;
		mstrFilename = "";
	}
}


// Return the open status
bool TextFile::GetOpenStatus()
{
	return(mbOpen);
}


// Return the open status
bool TextFile::GetCancelStatus()
{
	return(mbCancel);
}

//Return the filename
CString TextFile::GetFilename()
{
	return(mstrFilename);
}
