// DirUtilities.cpp: implementation of the CDirUtilities class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2002, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to support generic directory operations
//
//	Functions:
//		FileExists - checks for the existence of a file
//		AppendPath - append a string to a given path with appropriate backslashes
//		AppendBackslash - append backslash to end of string if not already there
//		CopyDirectory - copy all files in one directory to another directory
//		OverwriteDbQuestion - display a common prompt for overwriting an existing database
//		StripPathFromFilename
//		AddDriveLetter
//		GetDirectorySize
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirUtilities.h"
#include "ProgressWnd.h"
#include "imagehlp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
// This class contains some useful utilities associated with directories
// and file names.
//////////////////////////////////////////////////////////////////////

CDirUtilities::CDirUtilities(bool bQuietMode)
{
	m_bQuietMode = bQuietMode;
}

CDirUtilities::CDirUtilities()
{
	m_bQuietMode = true;  //default to silence for NDAR's sake
}

CDirUtilities::~CDirUtilities()
{

}


///////////////////////////////////////////////////////////////////////////
//	Name:	FileExists
//
//	Description:
//	Checks for the existence of the specified file at the specified directory.
//	
//	Declaration:
//	bool CDirUtilities::FileExists(CString& strRootPath, CString& strFilename)
//
//	Input:	strRootPath		directory of where to look for the file
//			strFilename		filename to look for
//
//	Output:	none
//
//	Return:	true (file exists)/ false (file not present)
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//////////////////////////////////////////////////////////////////
bool CDirUtilities::FileExists(const CString& strFullPath)
{
    return  0xFFFFFFFF != GetFileAttributes((LPCTSTR)strFullPath);
}
bool CDirUtilities::FileExists(const CString& strRootPath, const CString& strFilename)
{
    return  (0xFFFFFFFF != GetFileAttributes((LPCTSTR)(strRootPath + strFilename)));
}
bool CDirUtilities::FileExists(const char* pstrRootPath, const char* pstrFilename)
{
	int iLen = strlen(pstrRootPath) + strlen(pstrFilename) + 2;
	char *pch = new char[iLen];
	memset(pch,0,iLen);
	strcpy(pch,pstrRootPath);
	strcat(pch,pstrFilename);
	DWORD dw = GetFileAttributes(pch);
	delete [] pch;
	return (dw != 0xFFFFFFFF);	
}
bool CDirUtilities::FileExists(const char* pstrFullPath)
{
	return  0xFFFFFFFF != GetFileAttributes(pstrFullPath);
}
///////////////////////////////////////////////////////////////////////////
//	Name:	AppendPath
//
//	Description:
//	Add the second path to the first path with appropriate backslashes.
//	The final string has a trailing backslash
//	
//	Declaration:
//	CString CDirUtilities::AppendPath(CString &strPath1, CString &strPath2)
//
//	Input:	strPath1		original path
//			strPath2		path to add to the original path
//
//	Output:	none
//
//	Return:	Combined path with backslash
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//////////////////////////////////////////////////////////////////
CString CDirUtilities::AppendPath(const CString &strPath1, const CString &strPath2)
{
	CString strTemp = strPath1;
	
	AppendBackslash(strTemp);
	strTemp += strPath2;			// add sub path
	AppendBackslash(strTemp);
	return(strTemp);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	AppendBackslash
//
//	Description:
//	If the far right character in the string is not a '\', append a '\'
//	to the string.
//	
//	Declaration:
//	CString CDirUtilities::AppendBackslash(CString &strStr)
//
//	Input:	strStr			string to check for backslash presence
//
//	Output:	none
//
//	Return:	String with backslash
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//////////////////////////////////////////////////////////////////
CString CDirUtilities::AppendBackslash(const CString &strStr)
{
	CString strTemp = strStr;

	strTemp.TrimRight();			// get rid of trailing spaces
	if (strTemp.Right(1) != "\\") strTemp += "\\";	// add trailing slash if not there
	return(strTemp);
}

void CDirUtilities::AppendBackslash(CString &cs)
{
	cs.TrimRight();			// get rid of trailing spaces

	if (cs.Right(1) != "\\") 
		cs += "\\";			// add trailing slash if not there
}


///////////////////////////////////////////////////////////////////////////
//	Name:	CopyDirectory
//
//	Description:
//	Copies all the files in the first directory to the second directory.
//	If the second directory does not exist, it is created. If the file exists
//	at the second directory, it is overwritten.
//	Note only files are copied not subdirectories underneath the directory.
//	
//	Declaration:
//	bool CDirUtilities::CopyDirectory(CString &strSource, CString &strDestination, bool bOverwrite)
//
//	Input:	strSource		directory to copy from
//			strDestination	direcotry to copy to
//
//	Output:	none
//
//	Return:	true (copied successfully)/ false (problem during copy of at least one file)
//
//	Special Note - this uses the imagehlp.dll for the MakeSureDirectoryPathExists call
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//	08-Nov-2004	SFK		Added FindClose to fix memory leak
//////////////////////////////////////////////////////////////////

bool CDirUtilities::CopyDirectory(
	const CString &strSource, 
	const CString &strDestination, 
	const bool bOverwrite)
{

	// WAS "NOT FOR NDAR" - REINSERTED 11/27/2007 pjm for B2R1
	// If Quiet Mode is on, then wndProgress will never
	// be activated and will go out of scope unused.
	CWnd* pWnd = AfxGetMainWnd();	
    CProgressWnd wndProgress(pWnd, "Copy Progress");

	CString strSrcFile, strDestFile, strFilename;
	BOOL bSuccess = TRUE;
	BOOL bMoreToCopy = FALSE;
	//BOOL bFailIfExists = (BOOL)!bOverwrite;
	int iDum;

	CString strSrcPath = AppendBackslash(strSource);
	CString strDestPath = AppendBackslash(strDestination);

	if (strSrcPath == strDestPath) return(true);

	// MakeSureDirectoryPathExists needs the drive at the beginning in order to work
	strDestPath = AddDriveLetter(strDestination);
	bSuccess = MakeSureDirectoryPathExists(strDestPath);
	if (!bSuccess)
		iDum = GetLastError();

	double dNumBytesToCopy;
	int iNumFilesToCopy;
	GetDirectorySize(strSource, &dNumBytesToCopy, &iNumFilesToCopy);

	WIN32_FIND_DATA FileData;
	strSrcFile = strSrcPath + "*.*";
	HANDLE pHandle = FindFirstFile(strSrcFile, &FileData);
	if (pHandle == INVALID_HANDLE_VALUE) 
		iDum = GetLastError();

	// WAS "NOT FOR NDAR" - REINSERTED 11/27/2007 pjm for B2R1
	if (!m_bQuietMode)
		wndProgress.SetRange(0,iNumFilesToCopy);

	do 
	{
		if (pHandle != INVALID_HANDLE_VALUE) 
		{
			if ((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) 
			{	// don't copy if dir
				strSrcFile = strSrcPath + FileData.cFileName;
				strDestFile = strDestPath + FileData.cFileName;
				strFilename.Format("Copying %s", FileData.cFileName);

				// WAS "NOT FOR NDAR" - REINSERTED 11/27/2007 pjm for B2R1
				if (!m_bQuietMode)
				{
					wndProgress.SetText(strFilename);         
					wndProgress.PeekAndPump();
				}

				/////////////////////////////////////////////////////////////////////////////
				// Redo the "bFailIfExists" logic explicitly pjm 3-jul-06
				//
				BOOL bFailIfExists = FALSE;

				if (!FileExists(strDestFile) || bOverwrite)
				{
					bSuccess = CopyFile(strSrcFile, strDestFile,bFailIfExists);
					if (!bSuccess) 
					{
						iDum = GetLastError();
						bMoreToCopy = FALSE;		// need this to get out of the loop when fail
					}
				}
				else
				{
					bSuccess = true;
				}

/*
				bSuccess = CopyFile(strSrcFile, strDestFile, bFailIfExists);
				if (!bSuccess) {
					iDum = GetLastError();
					bMoreToCopy = FALSE;		// need this to get out of the loop when fail
				}

*/
				//
				/////////////////////////////////////////////////////////////////////

				// WAS "NOT FOR NDAR" - REINSERTED 11/27/2007 pjm for B2R1
				if (!m_bQuietMode)
				{
					if (wndProgress.Cancelled()) 
					{
						MessageBox(NULL,"Progress Cancelled",NULL,MB_OK);
						bSuccess = FALSE;
					}
					wndProgress.StepIt();
				}
			}
			if (bSuccess) 
				bMoreToCopy = FindNextFile(pHandle, &FileData);

			if (!bMoreToCopy) 
				iDum = GetLastError();
		}
	} while (bMoreToCopy);

	if (pHandle != INVALID_HANDLE_VALUE) 
		FindClose(pHandle);

	return (bSuccess != FALSE);

	//if (bSuccess == FALSE) 
	//	return(false);
	//
	//return(true);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	OverwriteDbQuestion
//
//	Description:
//	Presents user with a question about whether or not want to overwrite a database
//	and gets answer.
//	
//	Declaration:
//	bool CDirUtilities::OverwriteDbQuestion(CString strDbPath)
//
//	Input:	strDbPath		directory containing an existing database
//
//	Output:	none
//
//	Return:	true (okay to overwrite)/ false (do not overwrite)
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//////////////////////////////////////////////////////////////////

bool CDirUtilities::OverwriteDbQuestion(const CString& strDbPath)
{
	// WAS "NOT FOR NDAR" - REINSERTED 11/27/2007 pjm for B2R1
	if (!m_bQuietMode)
	{
		CString Msg;
		Msg.Format("A database already exists at %s.  Do you want to overwrite it?", strDbPath);
		int iResponse = MessageBox(NULL, Msg, "Overwrite Existing DataStore", MB_YESNO|MB_DEFBUTTON1);
		if (iResponse == IDYES) return(true);
		return(false);
	}
	return true;
}
//

CString CDirUtilities::StripPathFromFilename(const CString &strFilenameWithPath)
{

	CString strTemp;
	int i = strFilenameWithPath.ReverseFind('\\');
	int iFilenameNumChar = strFilenameWithPath.GetLength() - i -1;
	strTemp = strFilenameWithPath.Right(iFilenameNumChar);
	return(strTemp);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	AddDriveLetter
//
//	Description:
//	If no drive letter exists in the string, the add the current drive
//	letter and ":" to the beginning of the string.
//	
//	Declaration:
//	CString CDirUtilities::AddDriveLetter(const CString strPath)
//
//	Input:	strPath		directory path
//
//	Output:	none
//
//	Return:	directory path with drive letter added
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Jan-2002	SFK		Created
//////////////////////////////////////////////////////////////////
#include <direct.h>
CString CDirUtilities::AddDriveLetter(const CString& strPath)
{
	CString strTemp = strPath;
	int iDrive;
	char cDrive;
	char strCurPath[MAX_PATH];

	// If the drive is in the string, the : will be in index position 1.
	if (strPath.GetAt(1) != ':') {
		iDrive = _getdrive();				// get the current drive number
		cDrive = (char)iDrive + 'A' - 1;	// convert to an ASCII character

		// figure out if the partial path is off the root or the current working directory
		if (strPath.GetAt(0) == '\\') {			// off the root, just add the drive to the beginning
			strTemp = cDrive;
			strTemp += ":";
			strTemp += strPath;
		}
		else {
			if (_getdcwd(iDrive, strCurPath, MAX_PATH) != NULL) {
				CString strPathOnly = strPath;
				strPathOnly.Delete(0,3);		// delete the .. from the path to add to cwd
				
				strTemp = strCurPath;
				strTemp.Delete(strTemp.GetLength()-1, 1);			// delete the trailing / from the cwd
				while (strTemp.GetAt(strTemp.GetLength()-1) != '\\')	// delete the last part of the cwd					strCurPath.Delete(strPathOnly.GetLength()-1, 1);
					strTemp.Delete(strTemp.GetLength()-1, 1);
				strTemp += strPathOnly;
			}
		}
	}
	return(strTemp);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDirectorySize
//
//	Description:
//	Calculates the number of bytes in each file in the directory and returns to
//	the user.
//	
//	Declaration:
//	void CDirUtilities::GetDirectorySize(const CString strDir, double *pdNumBytes, int *pdNumFiles)
//
//	Input:	strDir		directory of interest
//
//	Output:	pdNumBytes	number of bytes in all files in strDir
//			pdNumFiles	number of files in strDir
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	01-May-2002	SFK		Created from GetDirSize() in Rad originated by Walt Hansen, major rewrite
//	07-May-2002 SFK		Added calculating the number of files in the directory
//	08-Nov-2004	SFK		Added FindClose to fix memory leak
//////////////////////////////////////////////////////////////////
void CDirUtilities::GetDirectorySize(const CString& strDir, double *pdNumBytes, int *piNumFiles)
{
	BOOL bMoreFiles = FALSE;
	int iDum;

	CString strSearch = strDir;

	if (pdNumBytes)
	   *pdNumBytes = 0.0;

	if (piNumFiles)
		*piNumFiles = 0;

	AppendBackslash(strSearch);
	strSearch += "*.*";

	WIN32_FIND_DATA FileData;
	HANDLE pHandle = FindFirstFile(strSearch, &FileData);
	if (pHandle == INVALID_HANDLE_VALUE) 
		iDum = GetLastError();

	do {
		if (pHandle != INVALID_HANDLE_VALUE) 
		{
			if ((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) 
			{	// don't add if dir
				if (pdNumBytes)
					*pdNumBytes += (double)((FileData.nFileSizeHigh * MAXDWORD) + FileData.nFileSizeLow);
				if (piNumFiles)
					(*piNumFiles)++;
			}
			bMoreFiles = FindNextFile(pHandle, &FileData);
			if (bMoreFiles == FALSE) 
				iDum = GetLastError();
		}
	} while (bMoreFiles == TRUE);
	FindClose(pHandle);
}

#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

bool CDirUtilities::DeleteDirectory(const TCHAR* sPath, CString *pcsFailedFilename) 
{
   HANDLE hFind;    // file handle
   WIN32_FIND_DATA FindFileData;

   CString csfailedfilename("");

   TCHAR DirPath[MAX_PATH];
   TCHAR FileName[MAX_PATH];

   _tcscpy(DirPath,sPath);
   _tcscat(DirPath,"\\*");    // searching all files
   _tcscpy(FileName,sPath);
   _tcscat(FileName,"\\");

   // find the first file
   hFind = FindFirstFile(DirPath,&FindFileData);
   if(hFind == INVALID_HANDLE_VALUE)
   {
	   *pcsFailedFilename = CString(DirPath);
	   return false;
   }
   _tcscpy(DirPath,FileName);

   bool bSearch = true;

   while(bSearch) 
   {    // until we find an entry
      if(FindNextFile(hFind,&FindFileData)) 
	  {
         if(IsDots(FindFileData.cFileName)) continue;
         _tcscat(FileName,FindFileData.cFileName);
         if((FindFileData.dwFileAttributes &
            FILE_ATTRIBUTE_DIRECTORY)) 
		 {

            // we have found a directory, recurse
            if(!DeleteDirectory(FileName,&csfailedfilename)) 
			{
				*pcsFailedFilename = csfailedfilename;
                FindClose(hFind);
                return false;    // directory couldn't be deleted
            }
            // remove the empty directory
            RemoveDirectory(FileName);
             _tcscpy(FileName,DirPath);
         }
         else 
		 {
            //if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
               // change read-only file mode
               //   _chmod(FileName, _S_IWRITE);

              if(!DeleteFile(FileName)) 
				{    // delete the file
				  	*pcsFailedFilename = CString(FileName);
                    FindClose(hFind);
                    return false;
               }
               _tcscpy(FileName,DirPath);
         }
      }
      else 
	  {
         // no more files there
         if(GetLastError() == ERROR_NO_MORE_FILES)
		 {
			 bSearch = false;
		 }
         else 
		 {
              FindClose(hFind);
              return false;
         }

      }

   } //end while
   FindClose(hFind);                  // close the file handle

   return (TRUE == RemoveDirectory(sPath));     // remove the empty directory
}

bool CDirUtilities::IsDots(const TCHAR* str) {
   if(_tcscmp(str,".") && _tcscmp(str,"..")) return false;
   return true;
}
	    

CString CDirUtilities::BuildPath(CString strPathtoBuild, CString strDriveLetter, bool bFailOnBadDriveSpec)
{
	//strip path components and make sure all exist
	//if specified drive does not exist, assume C (if allowed)
	//a drive specification within the path trumps
	CString strDriveInPath, strDrive, strPath;
	int iPos;
	const DWORD dwLen = 100; 
	DWORD dwSize;
	char bufDrives[dwLen];
	//bool bSuccess = true;
	BOOL bSuccess = TRUE;

	//check for drive in path
	iPos = strPathtoBuild.Find(":");
	if(iPos>=0)
	{
		strDriveInPath = strPathtoBuild.Left(iPos);
		strPathtoBuild = strPathtoBuild.Mid(iPos+1);
		if(strPathtoBuild.Find('\\')==0)
			strPathtoBuild = strPathtoBuild.Mid(1);
	}

	//make sure a terminal '\' exists
	if(strPathtoBuild.GetAt(strPathtoBuild.GetLength()-1)!='\\')
		strPathtoBuild += "\\";

	//get drive strings from OS
	dwSize = GetLogicalDriveStrings(dwLen,bufDrives);
	if(dwSize>0)
	{
		for(int i=0;i<(int)dwSize;i++)
			if(bufDrives[i]==0)
				bufDrives[i] = '_';
		CString strDrives(bufDrives);
		strDrives.MakeUpper();

		//make sure drive spec includes ":\" and is upper case
		for(int j=0;j<2 && strPath.IsEmpty();j++)
		{
			strDrive = (j==0)?strDriveInPath:strDriveLetter;
			strDrive.MakeUpper();
			if(!strDrive.SpanIncluding("ABCDEFGHIJKLMNOPQRSTUVWXYZ").IsEmpty())
			{
				if(strDrive.GetLength()==1)
					strDrive += ":\\";
				else 
					strDrive = strDrive.Left(1) + ":\\";
			}

			iPos = strDrives.Find(strDrive);
			if(!strDrive.IsEmpty() && iPos>=0 && (GetDriveType(LPCTSTR(strDrive))==DRIVE_FIXED || GetDriveType(LPCTSTR(strDrive))==DRIVE_REMOTE))
				strPath = strDrive;
		}
		if(bFailOnBadDriveSpec && strPath.IsEmpty())
			bSuccess = FALSE;//false;
		else
		{
			if(strPath.IsEmpty())
				strPath = "C:\\";
			//find each successive path component and make sure it exists
			iPos = strPathtoBuild.Find('\\');
			while(iPos > 0)
			{
				CString strTemp = strPathtoBuild.Left(iPos);
				strPathtoBuild = strPathtoBuild.Mid(iPos+1);
				strPath += strTemp;
				CFileFind ff;
				if(!ff.FindFile(LPCTSTR(strPath)))
					bSuccess = ::CreateDirectory(strPath,NULL);
				if(bSuccess)
					iPos = strPathtoBuild.Find('\\');
				else 
					iPos = -1;
				strPath += "\\";
			}
		}
	}
	else
	{
		//couldn't find drive specs
		bSuccess = FALSE;//false;
	}
	if(!bSuccess)
		strPath.Empty();
	return strPath;	//return path that was built
}

