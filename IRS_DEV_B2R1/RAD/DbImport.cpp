///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003-2005, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DbImport.cpp     
//Primary Author: Shirley Klosterbuer

#include "gen.h"
#include "RAD.H"
#include "BinaryImport.h"
#include "DateTimeUtils.h"
#include "DbDay.h"
#include "DbImport.h"
#include "DbOperator.h"
#include "DbTime.h"
#include "GrandImport.h"
#include "TextFile.h"
#include "TimeAlign.h"
#include "Utilities.h"
#include "FacilityConfig.h" //KM
#include "VifmImport.h"
#include "MyDateTime.h"
#include "TypeDefinitions.h"
#include "EOSSImport.h"

#include <comutil.h>

extern "C"
{
	#include "Check_Signature.h"
}

void byteswap_ushort(unsigned short *us)
{
	*us = ((*us >> 8) | (*us << 8));
}

void byteswap_ulong(unsigned long *ui)
{
	*ui = (*ui >> 24) | ((*ui << 8) & 0x00FF0000) | ((*ui >> 8) & 0x0000FF00) | (*ui << 24);
}

unsigned short  checksum_ushort(unsigned short *us)
{
	unsigned short cs = 0;
	BYTE* b = (BYTE*)us;
	cs = b[0] + b[1];
	return cs;
}

unsigned short  checksum_ulong(unsigned long *ul)
{
	unsigned short cs = 0;
	BYTE* b = (BYTE*)ul;
	for (int i=0;i<4;i++) cs += b[i];
	return cs;
}

extern short glsFacNum;
extern CMyDateTime glMyDateTime;
extern CDbDay glDbDay;

#pragma pack(1)
struct OpCode 
{
	short sRecSize;
	char RecTypeA;
	char RecTypeB;
};

typedef struct tagLANLHEADER
{
	char	SizeOfHeader[4];
	char	unused1[5];
	char	Version[5];
	char	StationId[3];
	char	Year[3];
	char	Month[3];
	char	Day[3];
	char	FullYear[4];
	char	unused2[43];
} LANLHEADER;
#pragma pack(8)

int MapCheckSigResultWRV(int csr, bool lenient);

/*===========================================================================
 *
 *  Name	 : ReadBIDHeader
 *
 *  Purpose	 :  Read header from GRAND raw data file (.BID).  See GRAND
 *			    Collect Users Manual for detailed definition of header
 *			    format.
 *
 *  Return value :  TRUE
 *					uiFILE_READ_ERR_STATUS - error either opening or reading header
 *
 *  Special notes:  File is opened and left open.  At exit from successful
 *		    		read, file pointer is positioned at beginning of first
 *				    data record in file.
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:	File is left opened.

 *  date    /	author	revision
 *  -----------------	--------
 *  03/21/89	SFK	Created
 *  16-Jan-1992 SFK	Note - can't change size of header because CDM software
 *			says 70 bytes following header, LANL Collect says 69.
 *  21-Jan-1992 SFK	Came up with way to use size of header.
    22-May-1995 SFK	Comments changed for windows version.
 *	29-Sep-1998 SFK		Implemented file version number support
 *	16-Nov-1998	SFK		Added read of first time in file to return to calling routine.
 *  06-Jul-1999 SFK Version 2.04 - add checking of hdrsize, day and month to determine if reasonable header
 *	24-Sep-2003	SFK	Version 3.0.3 - modified to support new FacMgr
===========================================================================*/
static int ReadBIDHeader(char *szFilename, struct IDStruct *pStaID,int *piYr, int *piMon, int *piDay, FILE **pHandle, double *pdVer, unsigned long *pulFirstRecordTime)
{
   /* ------------------------------------------------------------------
    *	Determine if the file is signed.  
	*   If it is, position past SnF header.
    * ----------------------------------------------------------------*/
	fpos_t pos;

	//SCR00227
	//Use CheckSignatureEx() because the return is testable.
	long lFileOffset = 0;
	long lUsableFileLength = 0;
	bool bIsSignedFile = false;
	long lUsableBytesReadIn = 0;

	unsigned char *publicKey, *signatureTimestamp; //not used

	int iRetval = CheckSignatureEx (szFilename, &lFileOffset, &lUsableFileLength,  
				&publicKey, &signatureTimestamp );

	// iRetval interpretation (From Cesare's InLineVerifier.c file)
	//0 = Successfully verified
	//1 = File is CORRUPTED - signature is invalid
	//3 = Verified OK, but CA authority is unknown
	//-14 = Input file has no S/MIME format
	// All others are failures.
	// Using the lenient interpretation of various errors here
	switch(iRetval)
	{
		case 0:
			{
				//Success!!
				bIsSignedFile = true;
				pos = (fpos_t)lFileOffset;
				iRetval = MapCheckSigResultWRV(iRetval, true); // mapped to 1
				break;
			}
		case 3:
			{
				//Signature CA unknown
				bIsSignedFile = true;
				pos = (fpos_t)lFileOffset;
				iRetval = MapCheckSigResultWRV(iRetval, true); // mapped to 1
				break;
				
			}
		case -14:
			{
				//There is no signature on this file.
				//Treat as normal.
				bIsSignedFile = false;
				pos = 0;
				iRetval = MapCheckSigResultWRV(iRetval, true); // mapped to 1
				break;
			}
		case -1:
			{
				//Library out of memory error, try to continue
				bIsSignedFile = true;
				pos = (fpos_t)lFileOffset;
				iRetval = MapCheckSigResultWRV(iRetval, true); // mapped to 1
				break;
			}
		case 1:
			{
				//Invalid signature, try anyway.
				bIsSignedFile = true;
				pos = (fpos_t)lFileOffset;
				iRetval = MapCheckSigResultWRV(iRetval, true); // mapped to 1
				break;
			}
		case -10:
			{
				//Input file does not exist
				bIsSignedFile = false;
				return uiNO_INPUT_FILE;
			}
		case -11:
			{
				//Input file empty
				bIsSignedFile = true;
				return uiEMPTY_INPUT_FILE;
			}
		case -12:
			{
				//IO Error
				bIsSignedFile = false;
				return uiIO_ERROR;
			}
		default:
			{
				//This file is no good...and we don't know why??
				return uiFILE_FAILED_SNF_CHECK_SIGNATURE;
			}
	}

	int iHdrSize;
    char str[54];
	struct GRAND_DATA_PT GrandPt;


   /* ------------------------------------------------------------------
    *	Open the file and apply the SnF offset.
    * ----------------------------------------------------------------*/
	*pHandle = fopen(szFilename,"rb");
	if (*pHandle == NULL) return(uiFILE_READ_ERR_STATUS);

	//skip over the SnF header if there is one
	int itemp = fsetpos(*pHandle, &pos); //SCR00227


   /* ------------------------------------------------------------------
    *	Read the first 4 bytes to get the number of bytes in header.
    *	Based on the location of the number, determine whether the data
    *	file is from CDMPC or LANL GRAND Collect.  The CDMPC number
    *	must be decremented by 1.
    * ----------------------------------------------------------------*/
    if (fread(str, 4, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[4] = '\0';    
    iHdrSize = atoi(str);
    if (str[3] == ' ') iHdrSize = iHdrSize - 1; // this file formed by CDMPC
	if (iHdrSize <= 22) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	The next 5 bytes no longer contain useful information,	just
    *	skip by them.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
       
   /* ------------------------------------------------------------------
    *	Read past the version number in the next 5 bytes.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
	str[5] = '\0';
	*pdVer = atof(str);

   /* ------------------------------------------------------------------
    *  Read station and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
	str[3] = '\0';
	pStaID->sStaID = atoi(str);

   /* ------------------------------------------------------------------
    *	Read year and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piYr = atoi(str);

   /* ------------------------------------------------------------------
    *	Read month and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piMon = atoi(str);
	if ((*piMon < 1) || (*piMon >12)) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	Read day and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piDay = atoi(str);
	if ((*piDay < 1) || (*piDay >31)) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	Read past the expansion space in the header so the file pointer
    *	is positioned at the beginning of the first data point at exit.
    * ----------------------------------------------------------------*/
    if (fread(str, (iHdrSize - 22), 1, *pHandle)!= 1) return(uiFILE_READ_ERR_STATUS);
    
   /* ------------------------------------------------------------------
    *	Save the position of the file pointer.
	*	Read the first record in the file to get the time of it.
    *	Restore file pointer to just at the first record.
    * ----------------------------------------------------------------*/
	if(fgetpos(*pHandle, &pos ) != 0) return(uiFILE_READ_ERR_STATUS);
	if (fread(&GrandPt, sizeof(struct GRAND_DATA_PT), 1, *pHandle) == 0) return(uiFILE_READ_ERR_STATUS);
	*pulFirstRecordTime = GrandPt.ulJulianTime;
	//char* temp = new char[100];
	//sprintf (temp,"about to set file position to %i",pos);
	//MessageBox (NULL, temp, "heather's debug message", MB_OK|MB_ICONWARNING);
	//if(fsetpos(*pHandle, &pos ) != 0) return(uiFILE_READ_ERR_STATUS);
	//delete[] temp;
	//Heather's hack.  clos/open to reset.  Seems to work.
    fclose(*pHandle);
	*pHandle = fopen(szFilename,"rb");
	if (fsetpos(*pHandle, &pos) !=0) return (uiFILE_READ_ERR_STATUS);
    return(iRetval);
}

// lazily cloned from ReadBIDHEader above 
static int ReadBI0Header(char *szFilename, struct IDStruct *pStaID,int *piYr, int *piMon, int *piDay, FILE **pHandle, double *pdVer, unsigned long *pulFirstRecordTime)
{
   /* ------------------------------------------------------------------
    *	Determine if the file is signed.  
	*   If it is, position past SnF header.
    * ----------------------------------------------------------------*/
	fpos_t pos;

	//SCR00227
	//Use CheckSignatureEx() because the return is testable.
	long lFileOffset = 0;
	long lUsableFileLength = 0;
	bool bIsSignedFile = false;
	long lUsableBytesReadIn = 0;

	unsigned char *publicKey, *signatureTimestamp; //not used

	int iRetval = CheckSignatureEx (szFilename, &lFileOffset, &lUsableFileLength,  
				&publicKey, &signatureTimestamp );

	// iRetval interpretation (From Cesare's InLineVerifier.c file)
	//0 = Successfully verified
	//1 = File is CORRUPTED - signature is invalid
	//3 = Verified OK, but CA authority is unknown
	//-14 = Input file has no S/MIME format
	// All others are failures.
	// Using the lenient interpretation of various errors here
	switch(iRetval)
	{
		case 0:
			{
				//Success!!
				bIsSignedFile = true;
				pos = (fpos_t)lFileOffset;
				iRetval = MapCheckSigResultWRV(iRetval, true); // mapped to 1
				break;
			}
		case 3:
			{
				//Signature CA unknown
				bIsSignedFile = true;
				pos = (fpos_t)lFileOffset;
				iRetval = MapCheckSigResultWRV(iRetval, true); // mapped to 1
				break;
				
			}
		case -14:
			{
				//There is no signature on this file.
				//Treat as normal.
				bIsSignedFile = false;
				pos = 0;
				iRetval = MapCheckSigResultWRV(iRetval, true); // mapped to 1
				break;
			}
		case -1:
			{
				//Library out of memory error, try to continue
				bIsSignedFile = true;
				pos = (fpos_t)lFileOffset;
				iRetval = MapCheckSigResultWRV(iRetval, true); // mapped to 1
				break;
			}
		case 1:
			{
				//Invalid signature, try anyway.
				bIsSignedFile = true;
				pos = (fpos_t)lFileOffset;
				iRetval = MapCheckSigResultWRV(iRetval, true); // mapped to 1
				break;
			}
		case -10:
			{
				//Input file does not exist
				bIsSignedFile = false;
				return uiNO_INPUT_FILE;
			}
		case -11:
			{
				//Input file empty
				bIsSignedFile = true;
				return uiEMPTY_INPUT_FILE;
			}
		case -12:
			{
				//IO Error
				bIsSignedFile = false;
				return uiIO_ERROR;
			}
		default:
			{
				//This file is no good...and we don't know why??
				return uiFILE_FAILED_SNF_CHECK_SIGNATURE;
			}
	}


	int iHdrSize;
    char str[54];
	struct DMGGRAND_DATA_PT_HDR GrandPt;


   /* ------------------------------------------------------------------
    *	Open the file and apply the SnF offset.
    * ----------------------------------------------------------------*/
	*pHandle = fopen(szFilename,"rb");
	if (*pHandle == NULL) return(uiFILE_READ_ERR_STATUS);

	//skip over the SnF header if there is one
	int itemp = fsetpos(*pHandle, &pos); //SCR00227


   /* ------------------------------------------------------------------
    *	Read the first 4 bytes to get the number of bytes in header.
    *	Based on the location of the number, determine whether the data
    *	file is from CDMPC or LANL GRAND Collect.  The CDMPC number
    *	must be decremented by 1.
    * ----------------------------------------------------------------*/
    if (fread(str, 4, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[4] = '\0';    
    iHdrSize = atoi(str);
    if (str[3] == ' ') iHdrSize = iHdrSize - 1; // this file formed by CDMPC
	if (iHdrSize <= 22) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	The next 5 bytes no longer contain useful information,	just
    *	skip by them.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
       
   /* ------------------------------------------------------------------
    *	Read past the version number in the next 5 bytes.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
	str[5] = '\0';
	*pdVer = atof(str);

   /* ------------------------------------------------------------------
    *  Read station and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
	str[3] = '\0';
	pStaID->sStaID = atoi(str);

   /* ------------------------------------------------------------------
    *	Read year and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piYr = atoi(str);

   /* ------------------------------------------------------------------
    *	Read month and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piMon = atoi(str);
	if ((*piMon < 1) || (*piMon >12)) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	Read day and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piDay = atoi(str);
	if ((*piDay < 1) || (*piDay >31)) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	Read past the expansion space in the header so the file pointer
    *	is positioned at the beginning of the first data point at exit.
    * ----------------------------------------------------------------*/
    if (fread(str, (iHdrSize - 22), 1, *pHandle)!= 1) return(uiFILE_READ_ERR_STATUS);
    

   /* ------------------------------------------------------------------
    *	Save the position of the file pointer.
	*	Read the first record in the file to get the time of it.
    *	Restore file pointer to just at the first record.
    * ----------------------------------------------------------------*/
	if(fgetpos(*pHandle, &pos ) != 0) return(uiFILE_READ_ERR_STATUS);

	// read 4 bytes at a time looking for the first data record (0xffffffff)
	unsigned int tag;
	while (fread(&tag, sizeof(unsigned int),1,*pHandle))
	{
		if (tag ^ 0xFFFFFFFF) // ^ anything but the data mask shown here
			OutputDebugString("skipping non-data record\r\n");
		else
			break;
	}

	if (fread(&GrandPt, sizeof(struct DMGGRAND_DATA_PT_HDR), 1, *pHandle) == 0) return(uiFILE_READ_ERR_STATUS);
	byteswap_ulong(&(GrandPt.ulJulianTime));
	*pulFirstRecordTime = GrandPt.ulJulianTime;
	byteswap_ushort(&(GrandPt.usJSCS));
	if (checksum_ulong(&(GrandPt.ulJulianTime)) != GrandPt.usJSCS)
		OutputDebugString("skip this record\r\n");
	if(fsetpos(*pHandle, &pos ) != 0) return(uiFILE_READ_ERR_STATUS);
                    
    return(TRUE);
}

/*===========================================================================
 *
 *  Name	 : ReadMCAHeader
 *
 *  Purpose	 :  Read header from 1KMCA raw data file (.MCA).  See MIC
 *			    Users Manual for detailed definition of header format.
 *
 *  Return value :  TRUE
 *					uiFILE_READ_ERR_STATUS - error either opening or reading header
 *
 *  Special notes:  File is opened and left open.  At exit from successful
 *		    		read, file pointer is positioned at beginning of first
 *				    data record in file.
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:	File is left opened.

 *  date    /	author	revision
 *  -----------------	--------
 *  03/21/89	SFK	Created
 *  16-Jan-1992 SFK	Note - can't change size of header because CDM software
 *			says 70 bytes following header, LANL Collect says 69.
 *  21-Jan-1992 SFK	Came up with way to use size of header.
    22-May-1995 SFK	Comments changed for windows version.
	27-May-1998 SFK Created by adapting from .BID header
 *	29-Sep-1998 SFK		Implemented file version number support
 *	16-Nov-1998	SFK		Added read of first time in file to return to calling routine.
 *  06-Jul-1999  SFK Version 2.04 - add checking of hdrsize, day and month to determine if reasonable header
 *	24-Sep-2003	SFK	Version 3.0.3 - modified to support new FacMgr
===========================================================================*/
static int ReadMCAHeader(char *szFilename, struct IDStruct *pStaID,int *piYr, int *piMon, int *piDay, FILE **pHandle, double *pdVer, unsigned long *pulFirstRecordTime)
{

    int iHdrSize;
    char str[54];
	struct MCA_DATA_PT McaPt;
	fpos_t pos;

   /* ------------------------------------------------------------------
    *	Open the file
    * ----------------------------------------------------------------*/
	*pHandle = fopen(szFilename,"rb");
	if (*pHandle == NULL) return(uiFILE_READ_ERR_STATUS);
	
   /* ------------------------------------------------------------------
    *	Read the first 4 bytes to get the number of bytes in header.
    *	Based on the location of the number, determine whether the data
    *	file is from CDMPC or LANL GRAND Collect.  The CDMPC number
    *	must be decremented by 1.
    * ----------------------------------------------------------------*/
    if (fread(str, 4, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[4] = '\0';    
    iHdrSize = atoi(str);
    if (str[3] == ' ') iHdrSize = iHdrSize - 1; // this file formed by CDMPC
	if (iHdrSize <= 22) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	The next 5 bytes no longer contain useful information,	just
    *	skip by them.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
       
   /* ------------------------------------------------------------------
    *	Read past the version number in the next 5 bytes.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
	str[5] = '\0';
	*pdVer = atof(str);

   /* ------------------------------------------------------------------
    *  Read station and put it in return variable.
	*  The station can be either digits or characters.  If characters
	*  then call the station number the decimal equivalent of the character.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
	pStaID->sStaID = atoi(str);

   /* ------------------------------------------------------------------
    *	Read year and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piYr = atoi(str);

   /* ------------------------------------------------------------------
    *	Read month and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piMon = atoi(str);
	if ((*piMon < 1) || (*piMon >12)) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	Read day and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piDay = atoi(str);
	if ((*piDay < 1) || (*piDay >31)) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	Read past the expansion space in the header so the file pointer
    *	is positioned at the beginning of the first data point at exit.
    * ----------------------------------------------------------------*/
    if (fread(str, (iHdrSize - 22), 1, *pHandle)!= 1) return(uiFILE_READ_ERR_STATUS);
    
   /* ------------------------------------------------------------------
    *	Save the position of the file pointer.
	*	Read the first record in the file to get the time of it.
    *	Restore file pointer to just at the first record.
    * ----------------------------------------------------------------*/
	if(fgetpos(*pHandle, &pos ) != 0) return(uiFILE_READ_ERR_STATUS);
	if (fread(&McaPt, sizeof(struct MCA_DATA_PT), 1, *pHandle) == 0) return(uiFILE_READ_ERR_STATUS);
	*pulFirstRecordTime = McaPt.ulJulianTime;
	if(fsetpos(*pHandle, &pos ) != 0) return(uiFILE_READ_ERR_STATUS);
                    
    return(TRUE);
}

/* <f> */
/*===========================================================================
 *
 *  Name	 : ReadISRHeader
 *
 *  Purpose	 :  Read header from ISR raw data file (.ISR).  See MIC
 *			    Users Manual for detailed definition of header format.
 *
 *  Return value :  TRUE
 *					uiFILE_READ_ERR_STATUS - error either opening or reading header
 *
 *  Special notes:  File is opened and left open.  At exit from successful
 *		    		read, file pointer is positioned at beginning of first
 *				    data record in file.
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:	File is left opened.

 *  date    /	author	revision
 *  -----------------	--------
 *  03/21/89	SFK	Created
 *  16-Jan-1992 SFK	Note - can't change size of header because CDM software
 *			says 70 bytes following header, LANL Collect says 69.
 *  21-Jan-1992 SFK	Came up with way to use size of header.
    22-May-1995 SFK	Comments changed for windows version.
	27-May-1997 SFK Adapted from ReadBIDHeader
 *	29-Sep-1998 SFK		Implemented file version number support
 *	16-Nov-1998	SFK		Added read of first time in file to return to calling routine.
 *  06-Jul-1999  SFK Version 2.04 - add checking of hdrsize, day and month to determine if reasonable header
 *	24-Sep-2003	SFK	Version 3.0.3 - modified to support new FacMgr
===========================================================================*/
static int ReadISRHeader(char *szFilename, struct IDStruct *pStaID,int *piYr, int *piMon, int *piDay, FILE **pHandle, double *pdVer, unsigned long *pulFirstRecordTime)
{

    int iHdrSize;
    char str[54];
	union ISR_DATA_PT ISR_Pt;
	fpos_t pos;

   /* ------------------------------------------------------------------
    *	Open the file
    * ----------------------------------------------------------------*/
	*pHandle = fopen(szFilename,"rb");
	if (*pHandle == NULL) return(uiFILE_READ_ERR_STATUS);
	
   /* ------------------------------------------------------------------
    *	Read the first 4 bytes to get the number of bytes in header.
    * ----------------------------------------------------------------*/
    if (fread(str, 4, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[4] = '\0';    
    iHdrSize = atoi(str);
	if (iHdrSize <= 22) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	The next 5 bytes no longer contain useful information,	just
    *	skip by them.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
       
   /* ------------------------------------------------------------------
    *	Read past the version number in the next 5 bytes.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
	str[5] = '\0';
	*pdVer = atof(str);

   /* ------------------------------------------------------------------
    *  Read station and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
	pStaID->sStaID = atoi(str);

   /* ------------------------------------------------------------------
    *	Read year and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piYr = atoi(str);

   /* ------------------------------------------------------------------
    *	Read month and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piMon = atoi(str);
	if ((*piMon < 1) || (*piMon >12)) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	Read day and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piDay = atoi(str);
	if ((*piDay < 1) || (*piDay >31)) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	Read past the expansion space in the header so the file pointer
    *	is positioned at the beginning of the first data point at exit.
    * ----------------------------------------------------------------*/
    if (fread(str, (iHdrSize - 22), 1, *pHandle)!= 1) return(uiFILE_READ_ERR_STATUS);
    
   /* ------------------------------------------------------------------
    *	Save the position of the file pointer.
	*	Read the first record in the file to get the time of it.
    *	Restore file pointer to just at the first record.
    * ----------------------------------------------------------------*/
	if(fgetpos(*pHandle, &pos ) != 0) return(uiFILE_READ_ERR_STATUS);
	if (fread(&ISR_Pt, sizeof(union ISR_DATA_PT), 1, *pHandle) == 0) return(uiFILE_READ_ERR_STATUS);
	*pulFirstRecordTime = ISR_Pt.IntPt.ulJulianTime;
	if(fsetpos(*pHandle, &pos ) != 0) return(uiFILE_READ_ERR_STATUS);
                    
    return(TRUE);
}

/* <f> */
/*===========================================================================
 *
 *  Name	 : ReadBSRHeader
 *
 *  Purpose	 :  Read header from JSR raw data file (.BSR).  See SR
 *			    Collect Users Manual for detailed definition of header
 *			    format.
 *
 *  Return value :  TRUE
 *					uiFILE_READ_ERR_STATUS - error either opening or reading header
 *
 *  Special notes:  File is opened and left open.  At exit from successful
 *		    		read, file pointer is positioned at beginning of first
 *				    data record in file.
 *
 *  Implicit Input :
  *  Implicit Input/Output:
 *  Implicit Output:	File is left opened.

 *  date    /	author	revision
 *  -----------------	--------
 *  09-Jan-1997	SFK		Created from ReadBIDHeader
 *	29-Sep-1998 SFK		Implemented file version number support
 *	16-Nov-1998	SFK		Added read of first time in file to return to calling routine.
 *  06-Jul-1999  SFK Version 2.04 - add checking of hdrsize, day and month to determine if reasonable header
 *	24-Sep-2003	SFK	Version 3.0.3 - modified to support new FacMgr
===========================================================================*/
static int ReadBSRHeader(char *szFilename, struct IDStruct *pStaID,int *piYr, int *piMon, int *piDay, FILE **pHandle, double *pdVer, unsigned long *pulFirstRecordTime)
{

    int iHdrSize;
    char str[54];
	struct JSR_DATA_PT SR_Pt;
	fpos_t pos;

   /* ------------------------------------------------------------------
    *	Open the file
    * ----------------------------------------------------------------*/
	*pHandle = fopen(szFilename,"rb");
	if (*pHandle == NULL) return(uiFILE_READ_ERR_STATUS);
	
   /* ------------------------------------------------------------------
    *	Read the first 5 bytes to get the number of bytes in header.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[5] = '\0';    
    iHdrSize = atoi(str);
	if (iHdrSize <= 22) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *  Read past facility number
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *  Read station and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
	pStaID->sStaID = atoi(str);

   /* ------------------------------------------------------------------
    *	Read past the version number
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
	str[5] = '\0';
	*pdVer = atof(str);

   /* ------------------------------------------------------------------
    *	Read year and put into return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piYr = atoi(str);

   /* ------------------------------------------------------------------
    *	Read month and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piMon = atoi(&str[1]);
	if ((*piMon < 1) || (*piMon >12)) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	Read day and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[3] = '\0';
    *piDay = atoi(&str[1]);
	if ((*piDay < 1) || (*piDay >31)) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	Read past the time
    * ----------------------------------------------------------------*/
    if (fread(str, 9, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    
   /* ------------------------------------------------------------------
    *	Read past the SR type
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    
   /* ------------------------------------------------------------------
    *	Read past the inspection period
    * ----------------------------------------------------------------*/
    if (fread(str, 12, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    
   /* ------------------------------------------------------------------
    *	Read past the expansion space in the header so the file pointer
    *	is positioned at the beginning of the first data point at exit.
    *	Use 47 because header size incorrectly is 98 rather than 95.
    * ----------------------------------------------------------------*/
    if (fread(str, (iHdrSize - 47), 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);

   /* ------------------------------------------------------------------
    *	Save the position of the file pointer.
	*	Read the first record in the file to get the time of it.
    *	Restore file pointer to just at the first record.
    * ----------------------------------------------------------------*/
	if(fgetpos(*pHandle, &pos ) != 0) return(uiFILE_READ_ERR_STATUS);
	if (fread(&SR_Pt, sizeof(struct JSR_DATA_PT), 1, *pHandle) == 0) return(uiFILE_READ_ERR_STATUS);
	*pulFirstRecordTime = SR_Pt.ulJulianTime;
	if(fsetpos(*pHandle, &pos ) != 0) return(uiFILE_READ_ERR_STATUS);
                    
    return(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Name	 : ReadBinHeader
//
//	Description:
//  Read header from binary raw data file (.BIN).  Bin files were created for use
//	with an early version of Media Tracker.  Defintion of header file
//	proposed in email from Pelowitz defines this format for the header structure:
//		struct TRGAcquireHeader	{	// 73 characters
//			char SizeOfHeader[4];
//			char unused1[5];
//			char Version[5];
//			char StationId[3];//node number
//			char Year[3];
//			char Month[3];
//			char Day[3];
//			char Year4[4];
//			char unused2[43];
//		};
//
//	Declaration:
//	int CBinaryDataFile::ReadBinHeader(char *szFilename, short *psSta,int *piYr, int *piMon, int *piDay, FILE **pHandle, double *pdVer, unsigned long *pulFirstRecordTime)
//
//	Input:	
//			strNameWithPath		filename with full path that is to be opened
//			
//	Output:	pstrErrorMsg	error, if any
//
//			mdVersionNumber		version number read from header
//			msStaNum			station number read from header
//			miYr				year read from header
//			miMon				month read from header
//			miDay				day read from header
//			mdTimestampOfFirstRecordInFile	time of first record in the file in DATE
//
//			
//			
//
//	Return:	true (header read) / false (some kind of error, see pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	17-Oct-2002	SFK		Created
//	24-Sep-2003	SFK	Version 3.0.3 - modified to support new FacMgr
////////////////////////////////////////////////////////////////////////////////////////////////
static int ReadBinHeader(char *szFilename, struct IDStruct *pStaID, int *piYr, int *piMon, int *piDay, FILE **pHandle, double *pdVer, unsigned long *pulFirstRecordTime)
{
    int iHdrSize;
    char str[54];
	fpos_t pos;
	struct TRGAcquireRecord BinaryRawPt;

	//	Open the file
	*pHandle = fopen(szFilename,"rb");
	if (*pHandle == NULL) return(uiFILE_READ_ERR_STATUS);

	//	Read the first 4 bytes to get the number of bytes in header.
    if (fread(str, 4, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
    str[4] = '\0';    
    iHdrSize = atoi(str);
	if (iHdrSize <= 22) return(uiFILE_READ_ERR_STATUS);

    //	The next 5 bytes no longer contain useful information,	just
    //	skip by them.
    if (fread(str, 5, 1, *pHandle) != 1) return(uiFILE_READ_ERR_STATUS);
       
    //	Read the version number in the next 5 bytes.
    if (fread(str, 5, 1, *pHandle) != 1) return(false);
	str[5] = '\0';
	*pdVer = atof(str);

    //  Read station.
    if (fread(str, 3, 1, *pHandle) != 1) return(false);
    str[3] = '\0';
	pStaID->sStaID = atoi(str);

    //	Read year.
    if (fread(str, 3, 1, *pHandle) != 1) return(false);
    str[3] = '\0';
    *piYr = atoi(str);

    //	Read month.
    if (fread(str, 3, 1, *pHandle) != 1) return(false);
    str[3] = '\0';
    *piMon = atoi(str);
	if ((*piMon < 1) || (*piMon >12)) return(false);

    //	Read day and put it in return variable.
    if (fread(str, 3, 1, *pHandle) != 1) return(false);
    str[3] = '\0';
    *piDay = atoi(str);
	if ((*piDay < 1) || (*piDay >31)) return(false);

    //	Read 4 digit year.
    if (fread(str, 4, 1, *pHandle) != 1) return(false);
    str[4] = '\0';

    //	Read past the expansion space in the header so the file pointer
    //	is positioned at the beginning of the first data point at exit.
    if (fread(str, (iHdrSize - 26), 1, *pHandle)!= 1) return(false);
    
    //	Save the position of the file pointer.
	//	Read the first record in the file to get the time of it.
    //	Restore file pointer to be positioned at the first record.
	if(fgetpos(*pHandle, &pos ) != 0) return(false);
	if (fread(&BinaryRawPt, sizeof(struct TRGAcquireRecord), 1, *pHandle) == 0) return(false);
	*pulFirstRecordTime = BinaryRawPt.uiJulianSeconds;
	if(fsetpos(*pHandle, &pos ) != 0) return(false);
    return(true);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	ReadEvtHeader
//
//	Description:
//	Read header from an Event File (.evt or .bny).  Definition of header file in email from
//	Pelowitz 7/26/2002 has this format:
//		struct EventAcquireHeader  // 73 total characters
//		{
//			char    SizeOfHeader[4];
//			char    unused1[5];
//			char    Version[5];
//			char    StationId[3];
//			char    Year[3];
//			char    Month[3];
//			char    Day[3];
//	        char    FullYear[4];
//			char    unused2[43];
//		};

//
//	Declaration:
//	bool CBinaryDataFile::ReadEvtHeader(const CString &strNameWithPath, CString *pstrErrorMsg)
//
//	Input:	strNameWithPath		filename with full path that is to be opened
//			
//	Output:	pstrErrorMsg	error, if any
//
//			mdVersionNumber		version number read from header
//			msStaNum			station number read from header
//			miYr				year read from header
//			miMon				month read from header
//			miDay				day read from header
//			mulTimestampOfFirstRecordInFile		julian time of first record in the file
//
//			
//			
//
//	Return:	true (header read) / false (some kind of error, see pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	17-Oct-2002	SFK		Created
// 	24-Sep-2003	SFK	Version 3.0.3 - modified to support new FacMgr
//	27-Oct-2003	SFK		Modified to use file extension of *.bny supported by MIC 1.805+
//////////////////////////////////////////////////////////////////
static int ReadEvtHeader(char *szFilename, struct IDStruct *pStaID,int *piYr, int *piMon, int *piDay, FILE **pHandle, double *pdVer, unsigned long *pulFirstRecordTime)
{
    int iHdrSize;
    char str[54];
	struct BinaryEventFileRec Rec;
	struct OpCode OpRec;
	fpos_t pos;

   /* ------------------------------------------------------------------
    *	Open the file
    * ----------------------------------------------------------------*/
	*pHandle = fopen(szFilename,"rb");
	if (*pHandle == NULL) return(uiFILE_READ_ERR_STATUS);
	
   /* ------------------------------------------------------------------
    *	Read the first 4 bytes to get the number of bytes in header.
    *	Based on the location of the number, determine whether the data
    *	file is from CDMPC or LANL GRAND Collect.  The CDMPC number
    *	must be decremented by 1.
    * ----------------------------------------------------------------*/
    if (fread(str, 4, 1, *pHandle) != 1) return(false);
    str[4] = '\0';    
    iHdrSize = atoi(str);
    if (str[3] == ' ') iHdrSize = iHdrSize - 1; // this file formed by CDMPC
	if (iHdrSize <= 22) return(false);

   /* ------------------------------------------------------------------
    *	The next 5 bytes no longer contain useful information,	just
    *	skip by them.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(false);
       
   /* ------------------------------------------------------------------
    *	Read past the version number in the next 5 bytes.
    * ----------------------------------------------------------------*/
    if (fread(str, 5, 1, *pHandle) != 1) return(false);
	str[5] = '\0';
	*pdVer = atof(str);

   /* ------------------------------------------------------------------
    *  Read station of logging node and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(false);
    str[3] = '\0';
	pStaID->sStaID = atoi(str);
//	*psSta += 1000;

   /* ------------------------------------------------------------------
    *	Read year and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(false);
    str[3] = '\0';
    *piYr = atoi(str);

   /* ------------------------------------------------------------------
    *	Read month and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(false);
    str[3] = '\0';
    *piMon = atoi(str);
	if ((*piMon < 1) || (*piMon >12)) return(false);

   /* ------------------------------------------------------------------
    *	Read day and put it in return variable.
    * ----------------------------------------------------------------*/
    if (fread(str, 3, 1, *pHandle) != 1) return(false);
    str[3] = '\0';
    *piDay = atoi(str);
	if ((*piDay < 1) || (*piDay >31)) return(false);

   /* ------------------------------------------------------------------
    *	Read past the expansion space in the header so the file pointer
    *	is positioned at the beginning of the first data point at exit.
    * ----------------------------------------------------------------*/
    if (fread(str, (iHdrSize - 22), 1, *pHandle)!= 1) return(false);
    
   /* ------------------------------------------------------------------
    *	Save the position of the file pointer.
	*	Read the first record in the file to get the time of it.  Will
	*	read the first record as if it is a binary record since we are
	*	only interested in the julian time in the record and that is in
	*	the same position in all types of records in an .evt or .bny file.
    *	Restore file pointer to just at the first record.
    * ----------------------------------------------------------------*/
	if(fgetpos(*pHandle, &pos ) != 0) return(false);
	if (fread(&OpRec, sizeof(struct OpCode), 1, *pHandle) == 0) return(false);
	if (fread(&Rec, sizeof(struct BinaryEventFileRec), 1, *pHandle) == 0) return(false);
	*pulFirstRecordTime = Rec.uiTime;
	pStaID->sStaID = Rec.usNode; // DR00270 14-March-2006 SFK This is the real station number, not that in the header
	if(fsetpos(*pHandle, &pos ) != 0) return(false);

    return(true);
}

static int ReadESSHeader(char *szFilename, struct IDStruct *pStaID,int *piYr, int *piMon, int *piDay, FILE **pHandle, double *pdVer, unsigned long *pulFirstRecordTime)
{

   /* ------------------------------------------------------------------
    *	Open the file
    * ----------------------------------------------------------------*/
	*pHandle = fopen(szFilename,"rb");

	if (*pHandle == NULL) return(uiFILE_READ_ERR_STATUS);
	
	bool bResult = false;
	LANLHEADER sLANLHeader;

	if (fread(&sLANLHeader,sizeof(sLANLHeader),1,*pHandle) != 1)
	{
		bResult = false;
	}
	else
		bResult = true;


	//do any header validation you need to do here
	if (bResult)
	{
		char szTemp[64];
		memcpy(szTemp,sLANLHeader.SizeOfHeader,sizeof(sLANLHeader.SizeOfHeader));
		szTemp[sizeof(sLANLHeader.SizeOfHeader)] = NULL;

		memcpy(szTemp,sLANLHeader.Version,sizeof(sLANLHeader.Version));
		szTemp[sizeof(sLANLHeader.Version)] = NULL;
		//mdVersionNumber = atof (szTemp);

		memcpy(szTemp,sLANLHeader.StationId,sizeof(sLANLHeader.StationId));
		szTemp[sizeof(sLANLHeader.StationId)] = NULL;
		pStaID->sStaID = atoi (szTemp);
		
		bResult = (pStaID->sStaID >0 && pStaID->sStaID < 32768);

		memcpy(szTemp,sLANLHeader.FullYear,sizeof(sLANLHeader.FullYear));
		szTemp[sizeof(sLANLHeader.FullYear)] = NULL;
		int iYr4 = atoi (szTemp);
		*piYr = iYr4 % 100;

		bResult = bResult && iYr4 > 1954 && iYr4 < 3000;

		memcpy(szTemp,sLANLHeader.Month,sizeof(sLANLHeader.Month));
		szTemp[sizeof(sLANLHeader.Month)] = NULL;
		*piMon = atoi (szTemp);

		bResult = bResult && *piMon > 0 &&*piMon < 13;

		memcpy(szTemp,sLANLHeader.Day,sizeof(sLANLHeader.Day));
		szTemp[sizeof(sLANLHeader.Day)] = NULL;
		*piDay = atoi (szTemp);

		bResult = bResult && *piDay > 0 && *piDay < 32; // Don't need to do anything terribly fancy here
	}
 
	fclose (*pHandle);
	// The EOSS file is more complicated than others, don't try to set the initial date here
	// from the first read.
	
//    if (pstrErrorMsg) pstrErrorMsg->Empty();
	//miErrorNum = 0;	// no error
   	return bResult;
}


/* <f> */
/*===========================================================================
 *
 *  Name	 : ReadHeader
 *
 *  Purpose	 :  Based on the filename, read the appropriate header.
 *
 *  Return value :  TRUE
 *					uiFILE_READ_ERR_STATUS - error either opening or reading header
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  09-Jan-1997	SFK		Created from ReadBIDHeader
 *	27-May-1998 SFK		Added .ISR, .MCA support
 *	29-Sep-1998 SFK		Implemented file version number support
 *  19-Jan-2001 SFK		Added *.bin test
 *  05-Aug-2002	SFK		Changed first record time returned to DATE
 *	24-Sep-2003	SFK		Version 3.0.3 - modified to support new FacMgr
 //	27-Oct-2003	SFK		Modified to use file extension of *.bny supported by MIC 1.805+
 //	16-May-2005	SFK		Remove *.bot support, Added support of  *.vbf files
===========================================================================*/
int ReadHeader(
	char *szFilename, 
	struct IDStruct *pStaID, 
	int *piYr, int *piMon, 
	int *piDay, 
	FILE **pFileHandle, 
	double *pdVer, 
	DATE *pdFirstRecordTime)
{
	int status = 0;
	char szTemp[PATH_LEN+DOSNAME_LEN+1];
	unsigned long ulFirstRecordTime;

	//JCD - make sure that only the file extension is searched, to avoid path dependent results
	char* pPeriod = strrchr(szFilename,'.');
	if(pPeriod==NULL)
	{
		//no extension on file name - shouldn't occur
		strcpy(szTemp, szFilename);
	}
	else
	{
		//found extension
		strcpy(szTemp, pPeriod);
	}
//	strcpy(szTemp, szFilename);
	_strlwr(szTemp);

	*piYr = 0;
	*piMon = 0;
	*piDay = 0;
	*pdVer = 0.0f;

    if (strstr(szTemp, ".bid")) 
	{
		status = ReadBIDHeader(szFilename, pStaID, piYr, piMon, piDay, pFileHandle, pdVer, &ulFirstRecordTime); // does SnF mapped return value
		pStaID->sStaType = GRAND_TYPE;
		pStaID->sDataType = FLOAT_TYPE;
	}
    else if (strstr(szTemp, ".bsr")) 
	{
    	status = ReadBSRHeader(szFilename, pStaID, piYr, piMon, piDay, pFileHandle, pdVer, &ulFirstRecordTime);
		pStaID->sStaType = JSR_TYPE;
		pStaID->sDataType = DOUBLE_TYPE;
	}
	else if (strstr(szTemp, ".jsr")) 
	{
    	status = ReadISRHeader(szFilename, pStaID, piYr, piMon, piDay, pFileHandle, pdVer, &ulFirstRecordTime);
		pStaID->sStaType = JSR1_TYPE;
		pStaID->sDataType = DOUBLE_TYPE;
	}
	else if (strstr(szTemp, ".isr") || strstr(szTemp, ".hmr")) 
	{
    	status = ReadISRHeader(szFilename, pStaID, piYr, piMon, piDay, pFileHandle, pdVer, &ulFirstRecordTime);
		pStaID->sStaType = ISR_TYPE;
		pStaID->sDataType = DOUBLE_TYPE;
	}
	else if (strstr(szTemp, ".mca")) 
	{
    	status = ReadMCAHeader(szFilename, pStaID, piYr, piMon, piDay, pFileHandle, pdVer, &ulFirstRecordTime);
		pStaID->sStaType = MCA_TYPE;
		pStaID->sDataType = FLOAT_TYPE;
	}
	else if (strstr(szTemp, ".id")) 
	{
		ulFirstRecordTime = 0;
		pStaID->sStaType = OP_TYPE;
		status = 1;
	}

//*********DR325**********
//JCD - 6/28/2006 duplicate block of code removed - led to file handle leak 
//ReadMCAHeader opens a data file, this second call blew away the original file handle

//	if (strstr(szTemp, ".mca")) {
//    	status = ReadMCAHeader(szTemp, pStaID, piYr, piMon, piDay, pFileHandle, pdVer, &ulFirstRecordTime);
//		pStaID->sStaType = MCA_TYPE;
//		pStaID->sDataType = FLOAT_TYPE;
//	}

	else if (strstr(szTemp, ".bin")) 
	{
    	status = ReadBinHeader(szFilename, pStaID, piYr, piMon, piDay, pFileHandle, pdVer, &ulFirstRecordTime);
		pStaID->sStaType = BINARY_TYPE;
		pStaID->sDataType = INTEGER_TYPE;
	}
	else if ((strstr(szTemp, ".evt")) || (strstr(szTemp, ".bny")))
	{
    	status = ReadEvtHeader(szFilename, pStaID, piYr, piMon, piDay, pFileHandle, pdVer, &ulFirstRecordTime);
		pStaID->sStaType = BINARY_TYPE;
		pStaID->sDataType = INTEGER_TYPE;
	}
	// Handle special cases of VIFM data -- don't know anything about which day is in the file
	else if (strstr(szTemp, "sl1.")) 
	{
		ulFirstRecordTime = 0;
		pStaID->sStaID = 1;
		pStaID->sStaType = BOT_TYPE;
		pStaID->sDataType = FLOAT_TYPE;
		status = 1;
	}
	else if (strstr(szTemp, "sl3.")) 
	{
		ulFirstRecordTime = 0;
		pStaID->sStaID = 3;
		pStaID->sStaType = BOT_TYPE;
		pStaID->sDataType = FLOAT_TYPE;
		status = 1;
	}
	else if (strstr(szTemp, "sl5."))  
	{
		ulFirstRecordTime = 0;
		pStaID->sStaID = 5;
		pStaID->sStaType = BOT_TYPE;
		pStaID->sDataType = FLOAT_TYPE;
		status = 1;
	}
    else if (strstr(szTemp, ".vbf")) 
	{
		ulFirstRecordTime = 0;
		pStaID->sStaType = BOT_TYPE;
		pStaID->sDataType = FLOAT_TYPE;
		status = 1;
		char * ADAM_group  = strstr (strupr(szFilename), "_GP") + 3;
		pStaID->sStaID = (int) ADAM_group [0] & 0x07;
	}
	//Add EOSS type to Rad Select Files allowable type.
	else if (strstr(szTemp, ".ess"))
	{
    	status = ReadESSHeader(szFilename, pStaID, piYr, piMon, piDay, pFileHandle, pdVer, &ulFirstRecordTime);
		pStaID->sStaType = EOSS_TYPE;
		pStaID->sDataType = INTEGER_TYPE;
	}
	else if (strstr(szTemp, ".bi0")) 
	{
		status = ReadBI0Header(szFilename, pStaID, piYr, piMon, piDay, pFileHandle, pdVer, &ulFirstRecordTime);
		pStaID->sStaType = GRAND_TYPE;
		pStaID->sDataType = FLOAT_TYPE;
	}

	if (pStaID->sStaType != EOSS_TYPE)
		*pdFirstRecordTime = glMyDateTime.MyTimestampToDATETimestamp(double(ulFirstRecordTime));
	else
		*pdFirstRecordTime = 0.0;//We don't know this from an EOSS type
	return(status);
}
static int ReadOperatorFile(char *pszFilename)
{
	FILE *pHandle;
	 char *pStr;
	 struct db_oper_rec dbOp;
	 char szLine[MAX_LINE_LEN+1], szTemp[DB_NAME_LEN+1];
	 int iOffset;
	 int status;
	 struct IDStruct StaID;

	pHandle = fopen(pszFilename,"ra");
	if (pHandle == NULL) 
		return(uiFILE_READ_ERR_STATUS);

	while (fgets(szLine, MAX_LINE_LEN, pHandle) != NULL) 
	{
		// parse out the id string
		pStr = strchr(szLine, ',');
		if (pStr != NULL) {
			szLine[pStr - szLine] = '\0';
			MyStrnCpy(dbOp.sz_op_item_id, szLine, ID_LEN);
		}
		// parse out the station name
		iOffset = pStr - szLine +1;
		pStr = strchr(&szLine[iOffset], ',');
		if (pStr != NULL) 
		{
			szLine[pStr - szLine] = '\0';
			MyStrnCpy(szTemp, &szLine[iOffset], DB_NAME_LEN);
			StripSpaces(szTemp);
			pglFacCfg->GetStationID(glsFacNum, szTemp, &StaID);
			dbOp.s_op_sta_num_key = StaID.sStaID;
		}
		// parse out the id timestamp
		iOffset = pStr - szLine + 1;
		pStr = strchr(&szLine[iOffset], ',');
		if (pStr != NULL) 
		{
			szLine[pStr - szLine] = '\0';
			MyStrnCpy(szTemp, &szLine[iOffset], DB_NAME_LEN);
			StripSpaces(szTemp);
			status = ConvertTimestampStrToJulianSecs(szTemp, &dbOp.ul_op_id_time);
		}

		status = CreateOperatorFacRec(&dbOp);
	}
	fclose(pHandle);
	return(TRUE);
}


/*===========================================================================
 *
 *  Name	 :  ImportData
 *
 *  Purpose	 :  Imports data from specified GRAND or JSR file into the database if
 *			    the data isn't already in the database.
 *
 *  Return value :  TRUE - data imported
 *			    error from read_hdr
 *		    	ESCAPE_HIT - aborted the form
 *			    BAD_DATA - file contains bad data
 *			    DATA_EXISTS - data already in database and don't want to overwrite
 *		    	FILE_IO_ERROR - error in reading raw data file
 *
 *  Special notes:
 *
 *  Global variables:  Variables named with a gl_ prefix are defined in
 *				       main.c.	Variables with a gll_ prefix are defined
 *				       at the top of this module.
 *
 *  Implicit Input :	gl_data_file_handle - file we're reading now
 *                      char *filename;	input : data file containing data to be imported 
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  26-May-1989 SFK	Created
 *  04-Aug-1992 SFK	Changed to default to overwrite data if already
 *					in database, also lengthened response time to 15s.
 *	27-May-1998 SFK	Added support of ISR, 1K MCA
 	29-Sep-1998 SFK	Implemented header version number to handle MCA file redefinition and
					to distinguish between test and real ISR data
	17-Nov-1998 SFK	Added concatenate of files from two days if first is less than the second.
	01-Apr-1999 SFK	Added operator file stuff
	01-Jul-1999  SFK Version 2.04 - printed out more details on day with no data points
									patch to handle gen library not correctly displaying first 63 days of 1952
 	02-Jul-1999  SFK Version 2.04 - printed out more details when get error during file read
	20-Aug-1999 SFK Version 2.05 - clarified which day file bogus SR data belongs to
	30-Aug-1999 SFK  Version 2.05 - added support for JSR/MIC type data
	01-Sep-1999 SFK  Version 2.05 - added minimal support for Bot data
	08-Feb-2001	SFK	Version 2.07 - added support for binary data
	18-Feb-2002 SFK	Version 2.10 - Added support for GrandCom.  Temporarily put in day record to use in events.
	05-Aug-2002	SFK	Converted to DATE, removed GRAND and BOT processing.  All time values stored in
					database day and data records are expressed in DATE
//	27-Oct-2003	SFK		Modified to use file extension of *.bny supported by MIC 1.805+
//	16-May-2005	SFK		Added support of  *.vbf files and also go to VIFM com with .vbf files
//  23-May-2005 SFK		Added reals/totals in last channel of SR record
===========================================================================*/
int ImportData(
	char *szFilename, 
	BOOL bOverwrite, 
	CGUI_TextWin *pWin, 
	char *szNameOnly, 
	BOOL bSkipOutOfOrder)
{
   struct db_float_data_rec	dbFloatData;	// database record structure
    struct db_double_data_rec	dbDoubleData;	// database record structure
	struct db_day_rec			dbDay;	// database record structure 
	struct MCA_DATA_PT McaPt;
    struct JSR_DATA_PT SR_Pt;
	union ISR_DATA_PT ISR_Pt;

    int i;
    int status;
    int iYr, iMon,iDay;
	DATE dDayStartTimestamp;
    DATE dDayEndTimestamp;   /* julian time of last valid time in this day */
	DATE dFirstTimestampInDay, dLastTimestampInDay, dFirstTimestampInFile;
    unsigned long ulPtsInDaySoFar; /* how many data pts in day so far */
    unsigned long ulPtsInDay;	    /* how many data pts in entire day */
	DATE dPreviousTimestamp;
    int iInvalidData;	    		/* number of records not belonging in the day */
    int iOutOfOrder;
    int iGamAdjust;
    char szDateStr[DT_LEN+1], szDateStr1[DT_LEN+1], szTimeStr[DT_LEN+1], szFirst[DT_LEN+1], szLast[DT_LEN+1], DumStr[DT_LEN+1];;
	BOOL bOverWrite = FALSE;
	BOOL bFirstPoint = TRUE;
	BOOL bDayAlreadyExists = FALSE;
	CGFmtStr msg;
	FILE *pFileHandle = NULL;       
	DATE dFirstRecordTime;
	BOOL bFillingExistingRec = FALSE;
	DATE dExpectedTimestamp, dNewPointAdjustedTimestamp;
	DATE  dCurrentPtDATETimestamp = -1.0;
	double dPreviousElapsedTime;
	int iGrandStatus = FALSE;
	int iVifmStatus = TRUE;  //Why is this true?? pjm
	int iBinStatus = FALSE;
	int iEOSSStatus = FALSE;
	char szTempFilename[PATH_LEN+DOSNAME_LEN+1];
	struct IDStruct StaID;

	// Customize for new COMs

   /* ------------------------------------------------------------------
    *	Look for the special case of Grand data and go to its Com Object
    * ----------------------------------------------------------------*/
	//JCD - make sure that only the file extension is searched, to avoid path dependent results
	char* pPeriod = strrchr(szFilename,'.');
	if(pPeriod==NULL)
	{
		//no extension on file name - shouldn't occur
		strcpy(szTempFilename, szFilename);
	}
	else
	{
		//found extension
		strcpy(szTempFilename, pPeriod);
	}
//	strcpy(szTempFilename, szFilename);
	_strlwr(szTempFilename);
	char *pStr = NULL;

	if (strstr(szTempFilename, ".bid") || // BI0 added per BMEND-1XX
		strstr(szTempFilename, ".bi0")) 
	{
		iGrandStatus = pglGrandImport->AddData(glsFacNum, szFilename, bOverwrite, &pStr);
		if (pWin != NULL) pWin->Printf("%s", pStr);
		delete [] pStr;
		if (!iGrandStatus) 
			return(uiFILE_READ_ERR_STATUS);
		// temporarily continue the import to get a day record used for events.
		//	return(status);	
	}

   /* ------------------------------------------------------------------
    *	Look for the special case of Vifm data and go to its Com Object
    * ----------------------------------------------------------------*/
	else if ((strstr(szTempFilename, ".bot")) || 
		(strstr(szTempFilename, ".vbf")) || 
		(strstr(szTempFilename, "sl"))) 		// temporary patch
	{
		pStr = NULL;
		iVifmStatus = pglVifmImport->AddData(glsFacNum, szFilename, bOverwrite, &pStr);
		if (pWin != NULL) pWin->Printf("%s", pStr);
		delete [] pStr;
		return(iVifmStatus);	// can't add in day record -- happens when write events
	}

   /* ------------------------------------------------------------------
    *	Look for the special case of Binary data and go to its Com Object
    * ----------------------------------------------------------------*/
	else if (strstr(szTempFilename, ".bin") || 
		strstr(szTempFilename, ".evt")|| 
		strstr(szTempFilename, ".bny")) 
	{
		pStr = NULL;
		iBinStatus = pglBinaryImport->AddData(glsFacNum, szFilename, bOverwrite, &pStr);
		if (pWin != NULL) pWin->Printf("%s", pStr);
		delete [] pStr;
		if (!iBinStatus) 
			return(uiFILE_READ_ERR_STATUS);
		// temporarily continue the import to get a day record used for events.
		//return(iBinStatus);	
	}

   /* ------------------------------------------------------------------
    *	Look for the special case of EOSS data and go to its Com Object
    * ----------------------------------------------------------------*/
	else if (strstr(szTempFilename, ".ess"))
	{
		pStr = NULL;
		iEOSSStatus = pglEOSSImport->AddData(glsFacNum, szFilename, bOverwrite, &pStr);
		if (pWin != NULL) pWin->Printf("%s", pStr);
		delete [] pStr;
		return(iEOSSStatus);	// can't add in day record -- happens when write events

		// <<QA>> or should it be this? pjm
		//if (!iEOSSStatus) 
		//	return(uiFILE_READ_ERR_STATUS);
	}

   /* ------------------------------------------------------------------
    *	Read read header information
    * ----------------------------------------------------------------*/
	double dVer = 0.0f;
	status = ReadHeader(szFilename, &StaID, &iYr, &iMon, &iDay, &pFileHandle, &dVer, &dFirstRecordTime);

	if (StaID.sStaType == OP_TYPE) 
	{
		status = ReadOperatorFile(szFilename);

		if ((status == 1) && (pWin != NULL)) 
			pWin->Printf("\n%s", szNameOnly);

		return(status);
	}
   
   /* ------------------------------------------------------------------
    *	From station number determine the station name and type.
    * ----------------------------------------------------------------*/
	char szTypeName[DB_NAME_LEN+1];
	if (!pglFacCfg->GetStationTypeName(glsFacNum, StaID.sStaType, MAX_NAME_LEN, szTypeName)) {	// unknown type
		if (pWin != NULL) pWin->Printf("\nFile: %s  File skipped. Station number %d is unknown to database.  Fix names.cfg.", szFilename, StaID.sStaID);
		goto abort;
	}
	strcpy(StaID.szStaType, szTypeName);
	if ((StaID.sStaType != GRAND_TYPE) && (StaID.sStaType != JSR_TYPE) && 
		(StaID.sStaType != ISR_TYPE) &&(StaID.sStaType != JSR1_TYPE) && 
		(StaID.sStaType != MCA_TYPE) && (StaID.sStaType != BOT_TYPE) &&
		(StaID.sStaType != BINARY_TYPE)) {
    	if (pWin != NULL) pWin->Printf("\nSkipping file %s (date=%02d.%02d.%02d): unknown station type %d",szFilename, iYr, iMon, iDay, StaID.sStaType);
		if (pFileHandle != NULL) fclose(pFileHandle);
		return(uiSKIP_FILE_STATUS);
    }    

   /* ------------------------------------------------------------------
    *	This routine tries to eliminate bad data from the GRAND from
    *	getting into the database and corrupting it.  If the GRAND
    *	problem is ever found and fixed this can be removed.
    *	Check the size of the file, if it is 109 bytes long, it is
    *	probably a bad data file and should not be imported.  If the
    *	user responds within a certain time, allow him to decide whether
    *	or not to import the data.  The default is to end the import
    *	process for this file.
    
    *	FIX for timeouts***************************************
    * ----------------------------------------------------------------*/
    if (StaID.sStaType == GRAND_TYPE) {
	    if (_filelength(fileno(pFileHandle)) == 109) {
			if (pWin != NULL) pWin->Printf("\nSkipping file %s (date=%02d.%02d.%02d): bad data size of 109",szFilename, iYr, iMon, iDay);
			if (pFileHandle != NULL) fclose(pFileHandle);
			return(uiSKIP_FILE_STATUS);
	    }    
	}    

   /* ------------------------------------------------------------------
    *	Determine the DATE value representing the start time of the day this file represents.
    * ----------------------------------------------------------------*/
    sprintf(szDateStr,"%02d.%02d.%02d", iYr, iMon, iDay);
	dDayStartTimestamp = glMyDateTime.DateTimeStrsToDATETimestamp(szDateStr, "00:00:00");
	dDayEndTimestamp = glMyDateTime.DateTimeStrsToDATETimestamp(szDateStr, "23:59:59");
       
   /* ------------------------------------------------------------------
    *	Check if record for this day already exists in database
    * -----------------------------------------------------------------*/
	status = glDbDay.FindDayRecInMainDb(StaID.sStaID, dDayStartTimestamp);
    if (status == uiDB_BAD_ERR) 
		goto abort;  
	if (status == uiSTA_NOT_IN_DB_ERR) 
	{
		if ((StaID.sStaType != GRAND_TYPE) && (StaID.sStaType != BOT_TYPE) && (StaID.sStaType != BINARY_TYPE))  //(not in a COM)
		{
			if (pWin != NULL) pWin->Printf("\nError: Skipping file %s with unknown station %hd", szFilename, StaID.sStaID);
			if (pFileHandle != NULL) fclose(pFileHandle);
			return(uiSKIP_FILE_STATUS);
		}
	}
	dbDay.d_day_end_time = 0.0;

	// For instruments with data in COMs, if day exists in db already then
	// are done since this is used only to provide a day to hook events to
	if (status == TRUE) 
	{
		if ((StaID.sStaType == GRAND_TYPE) || (StaID.sStaType == BOT_TYPE) || (StaID.sStaType == BINARY_TYPE)) {
			return(TRUE);
		}
	}
	                                                          
	/* ------------------------------------------------------------------
	*	If data already in database, either automatically overwrite the 
	*	data or ask the user if they want to overwrite.
	* ----------------------------------------------------------------*/
	if (status == TRUE) {  // day exists in db
		HCURSOR OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));
		// check to see if can just add to the existing day; this is allowed
		status = glDbDay.ReadDayRecInMainDb(StaID.sStaID, dDayStartTimestamp, &dbDay);	// read out existing day record
		if (status != TRUE) goto abort;
		if (dFirstRecordTime > dbDay.d_day_end_time) { // day exists but can add to end of day
			bDayAlreadyExists = TRUE;
		}
		else {
			if (bOverwrite != TRUE) {
				msg.Printf("File %s contains data from %s which already exists in database.  Do you want to overwrite the day?",
					szFilename, szDateStr);
				if (GUI_MsgBox(msg, GUI_ICON_QUESTION, GUI_YESNO) == GUI_NO) {
					if (pWin != NULL) pWin->Printf("\nSkipping file %s (date=%02d.%02d.%02d): day's data already in database",szFilename, iYr, iMon, iDay);
					if (pFileHandle != NULL) fclose(pFileHandle);
					::SetCursor(OldCur);
					return(uiSKIP_FILE_STATUS);
				}
			}
			status = glDbDay.DeleteDbDayInMainDb(StaID, dDayStartTimestamp);
			if (status != TRUE) goto abort;	
			bOverWrite = TRUE;
		}
	}	

	/* ------------------------------------------------------------------
	*	If day is not already in db, create a new day record with all db linkages needed.
	*	If day is in db, then add to the existing record.
	* ----------------------------------------------------------------*/
	if (bDayAlreadyExists == FALSE) 
	{
		HCURSOR OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));
		bool bDataExists = true;
		// if the data went into a COM database, make sure there was some valid data in the day
		// 15-Nov-2004 SFK  3.0.3.16 Fixed to not put in framework if no valid data in day.
		if ((StaID.sStaType == GRAND_TYPE) ||
			(StaID.sStaType == BOT_TYPE) || 
			(StaID.sStaType == BINARY_TYPE)) 
		{
			double dDum, dDum1;
			unsigned long ulNumPts;

			pglTimeAlign->GetDataStatistics(glsFacNum, ANY_STATION, -1, dDayStartTimestamp, dDayEndTimestamp, &dDum, &dDum1, &ulNumPts, StaID.sStaType);
			if (ulNumPts == 0) 
				bDataExists = false;
		}
		if (bDataExists) 
		{
			if (glDbDay.CreateDayRecInMainDb(StaID.sStaID, dDayStartTimestamp)!= TRUE) 
			{
				::SetCursor(OldCur);
				goto abort;
			}

			switch (StaID.sDataType)
			{
			case FLOAT_TYPE:
				if(d_setor(DAY_TO_FLOAT_DATA_SET,CURR_DB) != S_OKAY) goto db_exit;
				break;
			case DOUBLE_TYPE:
				if(d_setor(DAY_TO_DOUBLE_DATA_SET,CURR_DB) != S_OKAY) goto db_exit;
				break;
			case INTEGER_TYPE:
				if (d_setor(DAY_TO_FLOAT_DATA_SET,CURR_DB) != S_OKAY) goto db_exit;
				break;
			default:
				::SetCursor(OldCur);
				goto db_exit;
			};

			//if ((StaID.sDataType == FLOAT_TYPE) &&  (d_setor(DAY_TO_FLOAT_DATA_SET,CURR_DB) != S_OKAY)) 
			//		goto db_exit;
			//if (StaID.sDataType == DOUBLE_TYPE) 
			//	if (d_setor(DAY_TO_DOUBLE_DATA_SET,CURR_DB) != S_OKAY) goto db_exit;
			//if (StaID.sDataType == INTEGER_TYPE)	// this isn't really the case but satisfy things for the db framework
			//	if (d_setor(DAY_TO_FLOAT_DATA_SET,CURR_DB) != S_OKAY) goto db_exit;

			i = 0;
			dPreviousTimestamp = 0;
			dLastTimestampInDay = 0;
			ulPtsInDaySoFar = 0;
			bFirstPoint = TRUE;
			bFillingExistingRec = FALSE;
		}
	}
	else { // data exists, adding to day
		HCURSOR OldCur = ::SetCursor(LoadCursor(NULL, IDC_WAIT));
		if ((StaID.sStaType == GRAND_TYPE) || (StaID.sStaType == MCA_TYPE)) {
			if (FindDbDataRecContainingTime(StaID, dbDay.d_day_end_time, SEARCH_START, &dbFloatData) != TRUE) { ::SetCursor(OldCur); goto db_exit;}
			i = 0; // find the end time in the record.
			while ((dbFloatData.d_fdata_time[i] != dbDay.d_day_end_time) && (i < (RAW_DATA_RECS_PER_DB_RECORD-1))) {
				i++;
			}
			if (i == (RAW_DATA_RECS_PER_DB_RECORD-1)) {
				i = 0;
				bFillingExistingRec = FALSE;
				ulPtsInDaySoFar = dbDay.ul_day_total_pts;
			}
			else {
				i++;
				bFillingExistingRec = TRUE;
				ulPtsInDaySoFar = dbDay.ul_day_total_pts - i;
			}
		}
		if ((StaID.sStaType == JSR_TYPE) || (StaID.sStaType == JSR1_TYPE) || (StaID.sStaType == ISR_TYPE)) {
			if (FindDbDataRecContainingTime(StaID, dbDay.d_day_end_time, SEARCH_START, &dbDoubleData) != TRUE) { ::SetCursor(OldCur); goto db_exit;}
			i = 0; // find the end time in the record.
			while ((dbDoubleData.d_ddata_time[i] != dbDay.d_day_end_time) && (i < (RAW_DATA_RECS_PER_DB_RECORD-1))) {
				i++;
			}
			if (i == (RAW_DATA_RECS_PER_DB_RECORD-1)) {
				i = 0;
				bFillingExistingRec = FALSE;
				ulPtsInDaySoFar = dbDay.ul_day_total_pts;
			}
			else {
				i++;	// this is the last point that has data
				bFillingExistingRec = TRUE;
				ulPtsInDaySoFar = dbDay.ul_day_total_pts - i;
			}
		}
		dPreviousTimestamp = dbDay.d_day_end_time;
		dLastTimestampInDay = dbDay.d_day_end_time;
		dFirstTimestampInFile = dFirstRecordTime;
		dFirstTimestampInDay = dbDay.d_day_beg_time;
		bFirstPoint = FALSE;
		::SetCursor(OldCur);
	}

	iInvalidData = 0;
	iOutOfOrder = 0;
	iGamAdjust = 0;

   /* ------------------------------------------------------------------
    *	Read records from the raw data file one at a time until all read.
    *	A group of individual records are combined into a single multiple
    *	record in the database.  As each record becomes full, write it
    *	to the database.
    * ----------------------------------------------------------------*/
//JCD - probably need to add the cond. fclose in comments below
//For these situations fclose is not otherwise happening
    switch (StaID.sStaType) 
	{
    	case GRAND_TYPE:
			if (pFileHandle != NULL) fclose(pFileHandle);
			return(iGrandStatus);		// temporary to get day record for events.
		break;    

    	case BOT_TYPE:
			if (pFileHandle != NULL) fclose(pFileHandle);
			return(iVifmStatus);		// temporary to get day record for events.
		break;    

    	case BINARY_TYPE:
			if (pFileHandle != NULL) fclose(pFileHandle);
			return(iBinStatus);		// temporary to get day record for events.
		break;    

		case MCA_TYPE:
		    while (fread(&McaPt, sizeof(struct MCA_DATA_PT), 1, pFileHandle) != 0) 
			{
				dCurrentPtDATETimestamp = glMyDateTime.MyTimestampToDATETimestamp((double)McaPt.ulJulianTime);
		       
		       /* ------------------------------------------------------------------
				*   If the point's julian time is not in this day, note it and skip this point.
				* ----------------------------------------------------------------*/
				if ((dCurrentPtDATETimestamp < dDayStartTimestamp) || (dCurrentPtDATETimestamp > dDayEndTimestamp)) 
				{
			    	iInvalidData++;
			    	continue;
				}
				
		       /* ------------------------------------------------------------------
				*   If the point is out of order count it.  
				*	If the points are in order then set a new prev point
				* ----------------------------------------------------------------*/
				if (bFirstPoint == TRUE) 
					dPreviousTimestamp = dCurrentPtDATETimestamp;

				if ((dCurrentPtDATETimestamp < dPreviousTimestamp) && (bFirstPoint == FALSE)) 
				{
				   	iOutOfOrder++;
					if (bSkipOutOfOrder == TRUE) 
						continue;
				}
			    else
				{
					dPreviousTimestamp = dCurrentPtDATETimestamp;
				}
			    
		       /* ------------------------------------------------------------------
				*   The largest point in the day is the last point and the smallest
				*	point in the day is the first point.
				* ----------------------------------------------------------------*/
			    if (dCurrentPtDATETimestamp > dLastTimestampInDay)
			    	dLastTimestampInDay = dCurrentPtDATETimestamp;

			    if (bFirstPoint == TRUE) 
				{
					dFirstTimestampInDay = dCurrentPtDATETimestamp;
					dFirstTimestampInFile = dFirstTimestampInDay;
					bFirstPoint = FALSE;
				}	
				else if (dCurrentPtDATETimestamp < dFirstTimestampInDay)
				{
				    dFirstTimestampInDay = dCurrentPtDATETimestamp;
					dFirstTimestampInFile = dFirstTimestampInDay;
				}
		
		       /* ------------------------------------------------------------------
				*   If this is the first record of a multiple record (index = 0),
				*   fill in the julian time of the 1st record and the number of
				*	pts in the day so far.
				* ----------------------------------------------------------------*/
				if (i == 0) 
				{	   
			    	dbFloatData.d_fdata_beg_time_key = dCurrentPtDATETimestamp;
			    	dbFloatData.ul_fdata_pts_so_far = ulPtsInDaySoFar;
				}
		
		       /* ------------------------------------------------------------------
				*   Fill the database record.
				*	Reverse the polarity of the authentication status bit
				* ----------------------------------------------------------------*/
				dbFloatData.d_fdata_time[i]  = dCurrentPtDATETimestamp;
				dbFloatData.ul_fdata_status[i] = (unsigned long)(McaPt.ucMcaAlarm << 16); // put alarm in upper bits
				dbFloatData.ul_fdata_status[i] += (unsigned long)(McaPt.usMcaStatus);	// add in the status byte
				dbFloatData.d_fdata_etime[i] = (double)McaPt.iElapsedTime;
				if (dVer == 1.0) 
				{
					dbFloatData.f_data_chan1[i]	 = McaPt.GrossCts;
					dbFloatData.f_data_chan2[i]	 = McaPt.U235;
					dbFloatData.f_data_chan3[i]	 = McaPt.Cs137;
					dbFloatData.f_data_chan4[i]	 = McaPt.U238;
				}
				else  // version 1.001 requires rearrangement because of redefinition of channels
				{
					dbFloatData.f_data_chan1[i]	 = McaPt.U238;		// gross counts now in MCA chan 4
					dbFloatData.f_data_chan2[i]	 = McaPt.GrossCts;	// U235  now in MCA chan 1
					dbFloatData.f_data_chan3[i]	 = McaPt.U235;		// Cs137 now in MCA chan 2
					dbFloatData.f_data_chan4[i]	 = McaPt.Cs137;		// U238  now in MCA chan 3
				}
				dbFloatData.f_data_chan5[i]	 = McaPt.Scaler;
				dbFloatData.f_data_chan6[i]	 = McaPt.U235Ratio;
				dbFloatData.f_data_chan7[i]	 = McaPt.Cs137Ratio;
				
				i++;	// count the point just processed
		
				if (i == RAW_DATA_RECS_PER_DB_RECORD) 
				{
			   	   /* ------------------------------------------------------------------
			    	*	When have enough data to fill multiple record, create the new
			    	*	record, fill it and attach to database.
			    	* ----------------------------------------------------------------*/
					if (bFillingExistingRec == FALSE) {
						if (d_fillnew(DB_FLOAT_DATA_REC,(char *)&dbFloatData, CURR_DB) != S_OKAY) return(uiDB_BAD_ERR);
						if (d_connect(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) return(uiDB_BAD_ERR);
					}
					else {	// special case if adding to an existing record in the dat
						if (d_recwrite((char *)&dbFloatData, CURR_DB) !=S_OKAY) return(uiDB_BAD_ERR);
						bFillingExistingRec = FALSE;
					}
			    	ulPtsInDaySoFar += i;	// accumulate points read so far
			    	i = 0;
			    	CGUI_App::ProcessEvents();
				}
		    }

		   /* ------------------------------------------------------------------
		    *	Got an error reading the data file.  Are expecting an EOF
		    *	error.	If it's anything else, then abort and delete partial
		    *	data already in the db.  If it's EOF, close	the raw data file
		    *	and continue.
		    * ----------------------------------------------------------------*/
		    if (feof(pFileHandle) == 0) 
			{
		    	if (pWin != NULL) 
					pWin->Printf("\nImport Error Reading File %s.  File Error = %s", szFilename, strerror( errno ) );
		    	if (pFileHandle != NULL) 
					fclose(pFileHandle);

				glDbDay.DeleteDbDayInMainDb(StaID, dDayStartTimestamp);
				return(uiFILE_READ_ERR_STATUS);
		    }
		    if (pFileHandle != NULL) 
				fclose(pFileHandle);
		
		   /* ------------------------------------------------------------------
		    *	Are at the end of the raw data file.  Fill the julian times
		    *	of the unfilled records with 0 and write the partially filled
		    *	record to the data base.
		    * ----------------------------------------------------------------*/
		    ulPtsInDay = ulPtsInDaySoFar + i;
		
		    if (i > 0) 
			{
				while (i < RAW_DATA_RECS_PER_DB_RECORD) 
				{
					dbFloatData.d_fdata_time[i] = 0;
			    	i++;
				}
				if (bFillingExistingRec == FALSE) 
				{
					if (d_fillnew(DB_FLOAT_DATA_REC,(char *)&dbFloatData, CURR_DB) != S_OKAY) 
						return(uiDB_BAD_ERR);
					if (d_connect(DAY_TO_FLOAT_DATA_SET, CURR_DB) != S_OKAY) 
						return(uiDB_BAD_ERR);
				}
				else 
				{	// special case if adding to an existing record in the dat
					if (d_recwrite((char *)&dbFloatData, CURR_DB) !=S_OKAY) 
						return(uiDB_BAD_ERR);
					bFillingExistingRec = FALSE;
				}
		    }
			break;    

		case JSR_TYPE:
		    while (fread(&SR_Pt, sizeof(struct JSR_DATA_PT), 1, pFileHandle) != 0) 
			{
				dCurrentPtDATETimestamp = glMyDateTime.MyTimestampToDATETimestamp((double)SR_Pt.ulJulianTime);
		       
		       /* ------------------------------------------------------------------
				*   If the point's julian time is not in this day, note it and skip this point.
				* ----------------------------------------------------------------*/
				if ((dCurrentPtDATETimestamp < dDayStartTimestamp) || 
					(dCurrentPtDATETimestamp > dDayEndTimestamp)) 
				{
			    	iInvalidData++;
			    	continue;
				}
				
		       /* ------------------------------------------------------------------
				*   If the point is out of order count it.  
				*	If the points are in order then set a new prev point
				* ----------------------------------------------------------------*/
				if (bFirstPoint == TRUE) 
				{
					dPreviousElapsedTime = (double)SR_Pt.usElapsedTime;
					// add a special test for cases where SR Collect is running in DOS window under NT
					// handle case of last pt(s) from prev day ending  up at the beginning of this file.
					//	Skip a first point within 1 hr + 15 minutes of the end of the day.
					// The 1hr is a temp check for data within an hour due to computers
					// going onto daylight time on Beddingfield's system.
					DATE dAmount = (900.0 + 3600.0)/86400.0;
					if ((dCurrentPtDATETimestamp + dAmount) > dDayEndTimestamp) 
					{
				    	iOutOfOrder++;
						glMyDateTime.DATETimestampToDateTimeStrs(dDayStartTimestamp,szDateStr1, szTimeStr);
						pWin->Printf("\n        In file for day %s first point ", szDateStr1);
						glMyDateTime.DATETimestampToDateTimeStrs(dCurrentPtDATETimestamp,szDateStr1, szTimeStr);
						pWin->Printf("%s %s skipped - bad timestamp.", szDateStr1, szTimeStr);
						continue;
					}
					dPreviousTimestamp = dCurrentPtDATETimestamp;
				}
				if ((dCurrentPtDATETimestamp < dPreviousTimestamp) && (bFirstPoint == FALSE)) {
			    	// add a special test for cases where SR Collect is running in DOS window under NT
					// this test handles the case when the first pt(s) for the next day end up in the
					// previous day's file.
					glMyDateTime.DATETimestampToDateTimeStrs(dDayStartTimestamp, szDateStr1, szTimeStr);
					dExpectedTimestamp = dPreviousTimestamp + dPreviousElapsedTime/86400.0;			// based on the previous point, this is the timestamp expected
					pWin->Printf("\n        In file for day %s ", szDateStr1);
					glMyDateTime.DATETimestampToDateTimeStrs(dCurrentPtDATETimestamp, szDateStr1, szTimeStr);
					pWin->Printf("out of order point found %s %s - expected", szDateStr1, szTimeStr);
					glMyDateTime.DATETimestampToDateTimeStrs(dExpectedTimestamp, szDateStr1, szTimeStr);
					pWin->Printf(" %s %s", szDateStr1, szTimeStr);
					dNewPointAdjustedTimestamp = dCurrentPtDATETimestamp + 1.0;	// figure out julian secs for a wrong date
					if ((dNewPointAdjustedTimestamp > (dExpectedTimestamp - 10.0/86400.0) ) && 
						(dNewPointAdjustedTimestamp < (dExpectedTimestamp + 10.0/86400.0)))  // 10s of NT overhead
					{
						dCurrentPtDATETimestamp = dNewPointAdjustedTimestamp;
						pWin->Printf("  Pt skipped");
				    	dPreviousTimestamp = dCurrentPtDATETimestamp;  
						dPreviousElapsedTime = (double)SR_Pt.usElapsedTime;
						continue;
					}
					if (bSkipOutOfOrder == TRUE) continue;
			    } 
			    else 
				{
			    	dPreviousTimestamp = dCurrentPtDATETimestamp;  
					dPreviousElapsedTime = (double)SR_Pt.usElapsedTime;
			    }	
			    
		       /* ------------------------------------------------------------------
				*   The largest point in the day is the last point and the smallest
				*	point in the day is the first point.
				* ----------------------------------------------------------------*/
				if (dCurrentPtDATETimestamp > dLastTimestampInDay) 
				{	
					dLastTimestampInDay = dCurrentPtDATETimestamp;
			    }	
			    if (bFirstPoint == TRUE) 
				{
					dFirstTimestampInDay = dCurrentPtDATETimestamp;
					bFirstPoint = FALSE;
					dFirstTimestampInFile = dFirstTimestampInDay;
				}	
				else 
				{
					if (dCurrentPtDATETimestamp < dFirstTimestampInDay) 
					{
				    	dFirstTimestampInDay = dCurrentPtDATETimestamp;
						dFirstTimestampInFile = dFirstTimestampInDay;
				    }	
				}
		
		       /* ------------------------------------------------------------------
				*   If this is the first record of a multiple record (index = 0),
				*   fill in the julian time of the 1st record and the number of
				*	pts in the day so far.
				* ----------------------------------------------------------------*/
				if (i == 0) 
				{	   
					dbDoubleData.d_ddata_beg_time_key = dCurrentPtDATETimestamp;
			    	dbDoubleData.ul_ddata_pts_so_far = ulPtsInDaySoFar;
				}
		
		       /* ------------------------------------------------------------------
				*   Fill the database record.
				* ----------------------------------------------------------------*/
				dbDoubleData.d_ddata_etime[i] 	= (double)SR_Pt.usElapsedTime;
				dbDoubleData.d_ddata_time[i]  	= dCurrentPtDATETimestamp;
				dbDoubleData.ul_ddata_status[i]	= (unsigned long)SR_Pt.usJSR_Status << 16;	// move to upper byte
				dbDoubleData.d_data_chan1[i]	= SR_Pt.dTotals1;
				dbDoubleData.d_data_chan2[i]	= SR_Pt.dRealsPlusAccidentals - SR_Pt.dAccidentals;
				dbDoubleData.d_data_chan3[i]	= SR_Pt.dAccidentals;
				dbDoubleData.d_data_chan4[i]	= SR_Pt.dTotals2;
				dbDoubleData.d_data_chan5[i]	= 0.0;
				dbDoubleData.d_data_chan6[i]	= dbDoubleData.d_data_chan2[i]/dbDoubleData.d_data_chan1[i];	// Add reals/totals
				dbDoubleData.d_data_chan7[i]	= 0.0;
		
				i++;	// count the point just processed
		
				if (i == RAW_DATA_RECS_PER_DB_RECORD) 
				{
			   	   /* ------------------------------------------------------------------
			    	*	When have enough data to fill multiple record, create the new
			    	*	record, fill it and attach to database.
			    	* ----------------------------------------------------------------*/
					if (bFillingExistingRec == FALSE) 
					{
			    		if (d_fillnew(DB_DOUBLE_DATA_REC,(char *)&dbDoubleData, CURR_DB) != S_OKAY) 
							goto db_exit;
			    		if (d_connect(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) 
							goto db_exit;
					}
					else 
					{	// special case if adding to an existing record in the dat
						if (d_recwrite((char *)&dbDoubleData, CURR_DB) !=S_OKAY) 
							return(uiDB_BAD_ERR);
						bFillingExistingRec = FALSE;
					}
			    	ulPtsInDaySoFar += i;	// accumulate points read so far
			    	i = 0;
				}
		    }

		   /* ------------------------------------------------------------------
		    *	Got an error reading the data file.  Are expecting an EOF
		    *	error.	If it's anything else, then abort and delete partial
		    *	data already in the db.  If it's EOF, close	the raw data file
		    *	and continue.
		    * ----------------------------------------------------------------*/
		    if (feof(pFileHandle) == 0) 
			{
		    	if (pWin != NULL) 
					pWin->Printf("\nImport Error Reading File %s.  File Error = %s", szFilename, strerror( errno ) );
		    	if (pFileHandle != NULL) 
					fclose(pFileHandle);

				glDbDay.DeleteDbDayInMainDb(StaID, dDayStartTimestamp);
				return(uiFILE_READ_ERR_STATUS);
		    }
		    if (pFileHandle != NULL) 
				fclose(pFileHandle);
		
		   /* ------------------------------------------------------------------
		    *	Are at the end of the raw data file.  Fill the julian times
		    *	of the unfilled records with 0 and write the partially filled
		    *	record to the data base.
		    * ----------------------------------------------------------------*/
		    ulPtsInDay = ulPtsInDaySoFar + i;
		
		    if (i > 0) 
			{
				while (i < RAW_DATA_RECS_PER_DB_RECORD) 
				{
					dbDoubleData.d_ddata_time[i] = 0.0;                    
			    	i++;
				}
				if (bFillingExistingRec == FALSE) 
				{
					if (d_fillnew(DB_DOUBLE_DATA_REC,(char *)&dbDoubleData, CURR_DB) != S_OKAY) 
						return(uiDB_BAD_ERR);
					if (d_connect(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) 
						return(uiDB_BAD_ERR);
				}
				else 
				{	// special case if adding to an existing record in the dat
					if (d_recwrite((char *)&dbDoubleData, CURR_DB) !=S_OKAY) 
						return(uiDB_BAD_ERR);
					bFillingExistingRec = FALSE;
				}
		    }
		break;    

    	case ISR_TYPE:
		case JSR1_TYPE:
		    while (fread(&ISR_Pt, sizeof(union ISR_DATA_PT), 1, pFileHandle) != 0) 
			{
				dCurrentPtDATETimestamp = glMyDateTime.MyTimestampToDATETimestamp((double)ISR_Pt.IntPt.ulJulianTime);
		       
		       /* ------------------------------------------------------------------
				*   If the point's julian time is not in this day, note it and skip this point.
				* ----------------------------------------------------------------*/
				if ((dCurrentPtDATETimestamp < dDayStartTimestamp) || 
					(dCurrentPtDATETimestamp > dDayEndTimestamp)) 
				{
			    	iInvalidData++;
			    	continue;
				}
				
		       /* ------------------------------------------------------------------
				*   If the point is out of order count it.  
				*	If the points are in order then set a new prev point
				* ----------------------------------------------------------------*/
				if (bFirstPoint == TRUE) dPreviousTimestamp = dCurrentPtDATETimestamp;
				if ((dCurrentPtDATETimestamp < dPreviousTimestamp) && (bFirstPoint == FALSE)) 
				{
			    	iOutOfOrder++;
					if (bSkipOutOfOrder == TRUE) 
						continue;
			    } 
			    else 
				{
					dPreviousTimestamp = dCurrentPtDATETimestamp;   
			    }	
			    
		       /* ------------------------------------------------------------------
				*   The largest point in the day is the last point and the smallest
				*	point in the day is the first point.
				* ----------------------------------------------------------------*/
			    if (dCurrentPtDATETimestamp > dLastTimestampInDay)
			    	dLastTimestampInDay = dCurrentPtDATETimestamp;
	
			    if (bFirstPoint == TRUE) 
				{
					dFirstTimestampInDay = dCurrentPtDATETimestamp;
					dFirstTimestampInFile = dFirstTimestampInDay;
					bFirstPoint = FALSE;
				}	
				else if (dCurrentPtDATETimestamp < dFirstTimestampInDay)
				{
				    dFirstTimestampInDay = dCurrentPtDATETimestamp;
					dFirstTimestampInFile = dFirstTimestampInDay;
				}			
		       /* ------------------------------------------------------------------
				*   If this is the first record of a multiple record (index = 0),
				*   fill in the julian time of the 1st record and the number of
				*	pts in the day so far.
				* ----------------------------------------------------------------*/
				if (i == 0) 
				{
					dbDoubleData.d_ddata_beg_time_key = dCurrentPtDATETimestamp;
			    	dbDoubleData.ul_ddata_pts_so_far = ulPtsInDaySoFar;
				}
		
		       /* ------------------------------------------------------------------
				*   Fill the database record.
				*	Note:  for the short term, convert the 10ths of second to whole seconds
				*	At version 1.5 of MIC, the ISR data is doubles, not int64 but code
				*	keeps old names since same size.  Also elapsed time is now is tenths
				*	of seconds so no division necessary.
				* ----------------------------------------------------------------*/
				if ((dVer == 2.2) || (dVer < 1.5))
				{
					if (dVer == 2.2) // special case old test data
						dbDoubleData.d_ddata_etime[i] 	= (double)ISR_Pt.IntPt.ElapsedTime; //////TEST data only
					else
						dbDoubleData.d_ddata_etime[i] 	= ((double)ISR_Pt.IntPt.ElapsedTime)/10.0; /////Use for real data

					dbDoubleData.d_ddata_time[i]  	= dCurrentPtDATETimestamp;
					dbDoubleData.ul_ddata_status[i]	= (unsigned long)ISR_Pt.IntPt.sISR_Status;
					dbDoubleData.d_data_chan1[i]	= (double)ISR_Pt.IntPt.Totals1;
					dbDoubleData.d_data_chan2[i]	= (double)ISR_Pt.IntPt.RealsPlusAccidentals - ISR_Pt.IntPt.Accidentals;
					dbDoubleData.d_data_chan3[i]	= (double)ISR_Pt.IntPt.Accidentals;
					dbDoubleData.d_data_chan4[i]	= (double)ISR_Pt.IntPt.Totals2;
					dbDoubleData.d_data_chan5[i]	= (double)ISR_Pt.IntPt.Totals3;
					dbDoubleData.d_data_chan6[i]	= dbDoubleData.d_data_chan2[i]/dbDoubleData.d_data_chan1[i];	// Add reals/totals
					dbDoubleData.d_data_chan7[i]	= 0.0;
				}
				else 
				{	// at 1.5 changed data type from int64 to double
					// at 1.5, changed etime to be in tenths of seconds from MIC
					// this truncation used because db does not currently support fractions of seconds
					dbDoubleData.d_ddata_etime[i] 	= ISR_Pt.DblPt.ElapsedTime;
					dbDoubleData.d_ddata_time[i]  	= glMyDateTime.MyTimestampToDATETimestamp((double)ISR_Pt.DblPt.ulJulianTime);
					dbDoubleData.ul_ddata_status[i]	= (unsigned long)ISR_Pt.DblPt.sISR_Status;
					dbDoubleData.d_data_chan1[i]	= (double)ISR_Pt.DblPt.Totals1;
					dbDoubleData.d_data_chan2[i]	= (double)ISR_Pt.DblPt.RealsPlusAccidentals - ISR_Pt.DblPt.Accidentals;
					dbDoubleData.d_data_chan3[i]	= (double)ISR_Pt.DblPt.Accidentals;
					dbDoubleData.d_data_chan4[i]	= (double)ISR_Pt.DblPt.Totals2;
					dbDoubleData.d_data_chan5[i]	= (double)ISR_Pt.DblPt.Totals3;
					dbDoubleData.d_data_chan6[i]	= dbDoubleData.d_data_chan2[i]/dbDoubleData.d_data_chan1[i];	// Add reals/totals
					dbDoubleData.d_data_chan7[i]	= 0.0;
			}

				i++;	// count the point just processed
		
				if (i == RAW_DATA_RECS_PER_DB_RECORD) 
				{
			   	   /* ------------------------------------------------------------------
			    	*	When have enough data to fill multiple record, create the new
			    	*	record, fill it and attach to database.
			    	* ----------------------------------------------------------------*/
					if (bFillingExistingRec == FALSE) 
					{
			    		if (d_fillnew(DB_DOUBLE_DATA_REC,(char *)&dbDoubleData, CURR_DB) != S_OKAY) 
							goto db_exit;
			    		if (d_connect(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) 
							goto db_exit;
					}
					else 
					{	// special case if adding to an existing record in the dat
						if (d_recwrite((char *)&dbDoubleData, CURR_DB) !=S_OKAY) 
							return(uiDB_BAD_ERR);
						bFillingExistingRec = FALSE;
					}
			    	ulPtsInDaySoFar += i;	// accumulate points read so far
			    	i = 0;
				}
		    }

		   /* ------------------------------------------------------------------
		    *	Got an error reading the data file.  Are expecting an EOF
		    *	error.	If it's anything else, then abort and delete partial
		    *	data already in the db.  If it's EOF, close	the raw data file
		    *	and continue.
		    * ----------------------------------------------------------------*/
		    if (feof(pFileHandle) == 0) 
			{
		    	if (pWin != NULL) 
					pWin->Printf("\nImport Error Reading File %s.  File Error = %s", szFilename, strerror( errno ) );
		    	if (pFileHandle != NULL) 
					fclose(pFileHandle);
				glDbDay.DeleteDbDayInMainDb(StaID, dDayStartTimestamp);
				return(uiFILE_READ_ERR_STATUS);
//				MessageBox (NULL, "eof error rad", "heather's debug message", MB_OK|MB_ICONWARNING);
		    }
		    if (pFileHandle != NULL) 
				fclose(pFileHandle);
		
		   /* ------------------------------------------------------------------
		    *	Are at the end of the raw data file.  Fill the julia times
		    *	of the unfilled records with 0 and write the partially filled
		    *	record to the data base.
		    * ----------------------------------------------------------------*/
		    ulPtsInDay = ulPtsInDaySoFar + i;
//			MessageBox (NULL, "end of file, enter dates in rad", "heather's debug message", MB_OK|MB_ICONWARNING);
		    if (i > 0) 
			{
				while (i < RAW_DATA_RECS_PER_DB_RECORD) 
				{
					dbDoubleData.d_ddata_time[i] = 0.0;                    
			    	i++;
				}
				if (bFillingExistingRec == FALSE) 
				{
					if (d_fillnew(DB_DOUBLE_DATA_REC,(char *)&dbDoubleData, CURR_DB) != S_OKAY) 
						return(uiDB_BAD_ERR);
					if (d_connect(DAY_TO_DOUBLE_DATA_SET, CURR_DB) != S_OKAY) 
						return(uiDB_BAD_ERR);
				}
				else 
				{	// special case if adding to an existing record in the dat
					if (d_recwrite((char *)&dbDoubleData, CURR_DB) !=S_OKAY) 
						return(uiDB_BAD_ERR);
					bFillingExistingRec = FALSE;
				}
		    }
			break;    
		
		default:
			break;
	}
		
   /* ------------------------------------------------------------------
    *	If there was no data for this day, delete the record you added
    *	at the start of reading the raw data file.  Print out some hints for the
	*	user as to why no data points are in the day.
    * ----------------------------------------------------------------*/
    if (ulPtsInDay == 0) 
	{
		glDbDay.DeleteDbDayInMainDb(StaID, dDayStartTimestamp);

		if ((pWin != NULL) && (dCurrentPtDATETimestamp == 0.0))
				pWin->Printf("No data points in this file.");
   
		return(TRUE);
    }

   /* ------------------------------------------------------------------
    *	Some data from this day in database - now update the day record
    *	describing this day.
    *	First, find the day record.  Then read it, update the pts field
    *	and then write it back to the database.
    * ----------------------------------------------------------------*/
	status = glDbDay.FindDayRecInMainDb(StaID.sStaID, dDayStartTimestamp);
    if (status != 1)
		return(status);

    if (d_recread(&dbDay, CURR_DB) != S_OKAY) 
		return(uiDB_BAD_ERR);      

    dbDay.ul_day_total_pts = ulPtsInDay;
    dbDay.d_day_end_time = dLastTimestampInDay;
	dbDay.d_day_beg_time = dFirstTimestampInDay;
    if (d_recwrite((char *)&dbDay, CURR_DB) !=S_OKAY) 
		return(uiDB_BAD_ERR);
//	MessageBox (NULL, "got through db write in rad", "heather's debug message", MB_OK|MB_ICONWARNING);

   /* ------------------------------------------------------------------
    *	Log which file was just imported successfully.  Include date, station
    *	name, file name and first/last time in the file.
    *	Also log if the day's data was overwritten and if there were
    *	any points out of order or data with invalid times.
    * ----------------------------------------------------------------*/
    if (pWin != NULL) 
	{ 
		glMyDateTime.DATETimestampToDateTimeStrs(dFirstTimestampInFile, DumStr, szFirst);
		glMyDateTime.DATETimestampToDateTimeStrs(dLastTimestampInDay, DumStr, szLast);
		char szStationName[DB_NAME_LEN+1];
		pglFacCfg->GetStationLongName(glsFacNum, &StaID, szStationName, DB_NAME_LEN);
    	pWin->Printf("\n%s  %25s  %s    %s    %s    %5ld", szDateStr, szStationName, szNameOnly, szFirst, szLast, ulPtsInDay);
		if (bOverWrite) pWin->Printf("  Overwrote existing day's data.");
		if (bDayAlreadyExists == TRUE) pWin->Printf("  Added to existing day's data.");
   		if (iOutOfOrder > 0) pWin->Printf("  %5d pts out of order.", iOutOfOrder);
	    if (iInvalidData > 0) pWin->Printf("  %5d rec(s) with invalid times.",iInvalidData);
	    if (iGamAdjust > 0) pWin->Printf("  %5d rec(s) during offsets.",iGamAdjust);
    }    
    return(TRUE);
                             
db_exit:                             
    status = uiDB_BAD_ERR;
abort:
    if (pFileHandle != NULL) 
		fclose(pFileHandle);

    return(status);
}


// Return a value mapped from a SnF code to the inconsistent RAD usage contexts.
// WRV 1 is good, all non-1 values are meh or bad.
// 'lenient' means proceed (ret 1) in spite of invalid sig, unknown CA or potential memory issues.
int MapCheckSigResultWRV(int csr, bool lenient)
{
	int res = 1; // means true or proceed depending upon the context
	switch (csr)
	{
	case 0:  // alles ist gut with CA, sig and data
	case -14: // unsigned file
		break;
	case 1:
		if (!lenient)
			res = uiINVALID_SIGNATURE;
		break;
	case 3:
		if (!lenient)
			res = uiUNKNOWN_CA;
		break;
	case -1:
		if (!lenient)
			res = uiMEMORY_ALLOCATION_ERR;
		break;
	case -10:
		res = uiNO_INPUT_FILE;
		break;
	case -11:
		res = uiEMPTY_INPUT_FILE;
		break;
	case -12:
		res = uiIO_ERROR;
		break;
	default:
		res = uiFILE_FAILED_SNF_CHECK_SIGNATURE;
		break;
	}
	return res;
}

