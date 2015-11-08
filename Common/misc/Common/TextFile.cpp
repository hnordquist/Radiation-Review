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
