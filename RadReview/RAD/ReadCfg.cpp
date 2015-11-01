///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : ReadCfg.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"                                   
#include <direct.h>
#include "DbDefault.h"
#include "DbGeneralDbSupport.h"
#include "RadInit.H"
#include "upperlayer/RTTitle.H"	// 8-Dec-04 SFK Added relative path
#include "ReadCfg.h"
#include "Utilities.h"
#include "FacilityConfig.h"
#include "MainDbCfg.h"

extern RADInit *pRAD_Init;
extern RTTitle *pRT_Title;

// Add globals for logging settings here as externs
extern bool gbEnableLogging;
extern int gnLoggingLevel;

extern short glsFacNum;
extern short glsStaNum;
extern short glsChanNum;
           
/*====================================================================
 *
 *  Function Name:		ParseLine()
 *
 *  Function Purpose:	Parses line from a file.  Blank lines are skipped.
 *					    Looks for a comment following the line indicated by /*
 *					    and ignores everything following the first /* in the line.
 *		    			Strip all terminators and control characters from the end
 		*			    of the line.  Terminate the line with an \0.
 *
 *	Return value:		TRUE/FALSE
 *
 *  Revision History
 *
 *     DATE	AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  23-May-1991 SFK		Created
 *  21-Oct-1991 WCH		Modified for generalized REVIEW
 *  08-Apr-1992 SFK		Modified to return only TRUE or FALSE
 *  15-Jun-1993 SFK		Modified to handle cases of lines with only comments or
 *						only control characters.
 *	12-Jun-1995 SFK		Copied from M3CA version, name changed.
 *====================================================================*/
int ParseLine (char *szLine, FILE *pHandle, int *piNumLines)
{
    char *pStr;	    /* pts to start of comment in line */
    char *pStatus;
    BOOL  bNonBlank; /* 0/1 = all blank line/something in the line */
    unsigned short i;

    do {
 	   /* ------------------------------------------------------------------
        *	Read a line, return immediately if none found.
        * ----------------------------------------------------------------*/
		pStatus = fgets(szLine, MAX_LINE_LEN, pHandle);
		if (pStatus == NULL) return (FALSE);
		(*piNumLines)++;
	
	   /* ------------------------------------------------------------------
		*   Look for a comment, if one is found "strip it off" by putting
		*   in '\0' in place of the first /.
		* ----------------------------------------------------------------*/
		#define COMMENT1 "/*"  // beginning of standard comment */
		#define COMMENT2 "//"  // beginning of other kind of comment */
		pStr = strstr(szLine, COMMENT1);
		if (pStr != NULL) {
			*pStr = '\0';
		}
		else {
			pStr = strstr(szLine, COMMENT2);
			if (pStr != NULL) {
				*pStr = '\0';
			}
		}	
	
	   /* ------------------------------------------------------------------
		*  If an all blank line is found after eliminating the comment,
	 	*  skip it and read another line.
		* ----------------------------------------------------------------*/
		i = 0;
		bNonBlank = FALSE;
		while ((szLine[i] != '\0') && (i < MAX_LINE_LEN))
		{
		   if (szLine[i] > ' ')
		   {
		       bNonBlank = TRUE;
		       break;
		   }
		   i++;
		}
    } while (bNonBlank == FALSE);

   /* --------------------------------------------------------------
    *	Start at the end of the line, eliminate all trailing spaces
    *	and control characters, and put an EOS after the last ascii
    *	character.
    * ------------------------------------------------------------*/
    pStr = strchr(szLine, '\0');
    if ((pStr != NULL) && (strlen(szLine) > 0)) {
		while (*pStr <= ' ') {
	    	*pStr = '\0';
	    	pStr--;
		}
    }

    return (TRUE);
}
    
/* <f> */
/*===========================================================================
 *
 *  Name	 :  ReadIniAndCfgFiles
 *
 *  Purpose	 :  Reads configuration file containing the program name and db path.
 *				This routine is called on startup and anytime the user switches from
 *				one database to another.
 *
 *  Return value :
 *
 *  Special notes:	The database open when the function is called is closed and the 
 *					database described in the default.cfg is opened.
 *

 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output: 

 *  date    /	author	revision
 *  -----------------	--------
 *  16-Jul-1990 SFK	Created
 *  21-Oct-1991 WCH	Modified for generalized REVIEW
 *  24-Jan-1992 SFK	Added reading deadtime coefficients
 *  12-Jun-1995 SFK	Modified from generalized REVIEW read_configuration_file for GRAND Review, ver 2.0
 *	30-Jan-1996 SFK Split reading configuration files into reading defaults and channel names.
 *	24-Jun-1998	SFK	Modified to just deal with opening the database, removed all .ini references
 *
===========================================================================*/
int OpenAndAccessDatabase(char *szRootPath, short sFacNum)
{

    bool bstatus;
    CGFmtStr msg;                             

	if (!GetDefaultFacilityNumber(&glsFacNum)) 
		return(FALSE);

	bstatus = OpenDatabase(szRootPath, sFacNum);	// this is root path of the facility

    if (!bstatus) 
	{
		RadReviewMsg(uiRAD_INVALID_DATABASE_PATH_ERR, szRootPath);	
		pRAD_Init->bParams_Loaded = FALSE;
		return(FALSE);
   	}
	
	return(TRUE);
}    


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDefaultFacilityNumber
//
//	Description:
//	Reads which facility number to use in facility manager from the file
//	\irs\rad\irs.ini and verify a facility with that id is known to facility manager
//	
//	Declaration:
//	BOOL GetDefaultFacilityNumber(short *psFacNum)
//
//	Input:	None
//
//	Output:	*psFacNum	ID of facility
//			*pszFacName	Name of facility
//
//	Return:	True - valid facility number, False - invalid number, abort
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Jul-2004	SFK		Created
//	14-Sep-2004	SFK		Modified to read IRS.INI out of directory where RAD.EXE is running
//  16-Sep-2004	SFK		3.0.3.12 Modified to write IRS.INI to directory where RAD.EXE is running
//	01-Nov-2004	SFK		3.0.3.14 Back to reading IRS.INI from directory above RAD.EXE
//	04-Dec-2005 SFK		Verify there is an irs.ini file where we expect it
/////////////////////////////////////////////////////////////////
BOOL GetDefaultFacilityNumber(short *psFacNum)
{
	BOOL bStatus = TRUE;
	char strValue[MAX_LINE_LEN+1];
	char szFilePathAndName[MAX_LINE_LEN+1];
	char szCurWkgDir[MAX_LINE_LEN+1];

	// assumption: irs.ini is in directory above the current working directory
	_getcwd(szCurWkgDir, PATH_LEN);
	char *c = strrchr(szCurWkgDir,'\\');//find the last "\"
    *c = '\0';      //Remove the executable name from the string.

	PathCopy(szFilePathAndName, szCurWkgDir);
	strncat(szFilePathAndName, "irs.ini", MAX_LINE_LEN);

	// 04-Dec-2005 SFK Verify there is an irs.ini file where we expect it
	if (FileExists(szCurWkgDir, "irs.ini") == uiFILE_DOES_NOT_EXIST_STATUS) {
		RadReviewMsg(uiNO_IRS_INI, szCurWkgDir, "irs.ini");
		return FALSE;
	}

	// read the default facility number from the file irs.ini
	//int iLen = GetPrivateProfileString("DEFAULT_FACILITY_INFO", "FacilityID", "", strValue, MAX_LINE_LEN, "\\irs\\irs.ini");
	int iLen = GetPrivateProfileString("DEFAULT_FACILITY_INFO", "FacilityID", "", strValue, MAX_LINE_LEN, szFilePathAndName);
	*psFacNum = atoi(strValue);

	//Get logging values from the rad.ini
	if (1 == GetPrivateProfileInt("LOGGING_SETTINGS", "Disable Logging", 1, szFilePathAndName))
		gbEnableLogging = false;
	else
		gbEnableLogging = true;

	gnLoggingLevel = GetPrivateProfileInt("LOGGING_SETTINGS", "Logging Level",3,szFilePathAndName);

	if ((gnLoggingLevel <= 0) || (gnLoggingLevel > 6))
		gnLoggingLevel = 3;

	// see if there is an associated facility in the facility database
	if (!pglFacCfg->IsFacilityDefined(*psFacNum)) {
		RadReviewMsg(uiINVALID_FACILITY, "", *psFacNum);
		bStatus = FALSE;
	}

	return bStatus;
}

SetDefaultFacilityNumber(short sFacNum)
{
	char strValue[MAX_LINE_LEN+1];
	char szFilePathAndName[MAX_LINE_LEN+1];
	char szCurWkgDir[MAX_LINE_LEN+1];

	// assumption: irs.ini is in directory above the current working directory
	_getcwd(szCurWkgDir, PATH_LEN);
	char *c = strrchr(szCurWkgDir,'\\');//find the last "\"
    *c = '\0';      //Remove the executable name from the string.

	PathCopy(szFilePathAndName, szCurWkgDir);
	strncat(szFilePathAndName, "irs.ini", MAX_LINE_LEN);

	// 04-Dec-2005 SFK Verify there is an irs.ini file where we expect it
	if (FileExists(szCurWkgDir, "irs.ini") == uiFILE_DOES_NOT_EXIST_STATUS) {
		RadReviewMsg(uiNO_IRS_INI, szCurWkgDir, "irs.ini");
		return FALSE;
	}

	sprintf(strValue,"%d", sFacNum);
	return(WritePrivateProfileString("DEFAULT_FACILITY_INFO", "FacilityID", strValue, szFilePathAndName));
}
    


/* <f> */
/*===========================================================================
 *
 *  Name	 :  ReadCfgFile
 *
 *  Purpose	 :  Use Facility Manager to get facility specific information
 *				about the facilities, stations and channels this program will
 *				handle.  Either write a new record to the database containing
 *				this information or overwrite the existing record.
 *
 *  Return value : TRUE
 *
 *  Special notes:	This function will disappear when analysis information stored in COM
 *

 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output: 

 *  date    /	author	revision
 *  -----------------	--------
 *  16-Jul-1990 SFK	Created
 *  21-Oct-1991 WCH	Modified for generalized REVIEW
 *  24-Jan-1992 SFK	Added reading deadtime coefficients
 *  12-Jun-1995 SFK	Modified from generalized REVIEW read_configuration_file for GRAND Review, ver 2.0
 *	30-Jan-1996 SFK Split reading configuration files into reading defaults and channel names.
 *  05-Mar-1996 SFK Added short names to db.
 *	27-May-1998 SFK	Added support of ISR, 1K MCA
 *	07-Mar-2001 SFK	Added support of Binary type
 *
===========================================================================*/
int ReadCfgFile()
{
    struct db_fac_rec dbFacRec;
    struct db_sta_rec dbStaRec;

	int j;
    int db_status;
    BOOL bFirstFacility = TRUE;
    BOOL bFirstStation = TRUE;
	//int *piFacIDs = NULL;
	struct IDStruct *pStaIDs = NULL;
	short sFacNum;

	/* ------------------------------------------------------------------
    *	Build framework in db_main for only the default facility
    * ----------------------------------------------------------------*/
//	int status = AccessDefaultDatabase(READ, S_DFLT_FACILITY, &sFacNum);
// 	if (status != TRUE) {
//		RadReviewMsg(status, "when reading from Default database.  Cannot proceed.");			
//		return(FALSE);
//	}
	if (!GetDefaultFacilityNumber(&sFacNum)) return(FALSE);

	// verify the facility is defined
	if (!pglFacCfg->IsFacilityDefined(sFacNum)) return(FALSE);
	   /* ------------------------------------------------------------------
		*	Read the info about a facility.  If this facility number
		*	is already described in the database, overwrite the existing
		*	information.  If not in the database, then create a new
		*	record describing this facility.
		* ----------------------------------------------------------------*/
		//	get facility db #
		pglFacCfg->GetFacilityRecord(sFacNum, &dbFacRec);
		if (bFirstFacility == TRUE) {
//			AccessDefaultDatabase(WRITE, S_DFLT_FACILITY, &dbFacRec.s_fac_num_key);
			glsFacNum = dbFacRec.s_fac_num_key;
			bFirstFacility = FALSE;
		}	
		
		// 	create a new facility record for this number if it doesn't exist yet
		//	and make it the owner of the facility-station set 
		db_status = d_keyfind (S_FAC_NUM_KEY, (char *) &dbFacRec.s_fac_num_key, CURR_DB);
		if (db_status == S_NOTFOUND) {
			d_fillnew (DB_FAC_REC, (char *) &dbFacRec, CURR_DB);
			d_connect (FACILITY_SET, CURR_DB);
		}
		else {
			d_recwrite ((char *) &dbFacRec, CURR_DB);
		}
		d_setor(FACILITY_TO_STATION_SET, CURR_DB);
		
		//	get # stations for this facility 
		int iNumStations = pglFacCfg->GetNumStations(sFacNum);
		pStaIDs = new struct IDStruct[iNumStations];
		if (!pglFacCfg->GetStationIDs(sFacNum, pStaIDs)) goto exit;
		for (j=0; j < iNumStations; j++) {
			
		   /* ------------------------------------------------------------------
			*	Read the info about this station.  If this station number
			*	is already described in the database, overwrite the existing
			*	information.  If not in the database, then create a new
			*	record describing this station.
			* ----------------------------------------------------------------*/
			// get station record
			if (!pglFacCfg->GetStationRecord(sFacNum, &(pStaIDs[j]), &dbStaRec)) goto exit;
			
			if (bFirstStation == TRUE) {
				AccessDefaultDatabase(WRITE, S_DFLT_STATION, &dbStaRec.s_sta_num_key);
				AccessDefaultDatabase(WRITE, S_DFLT_STATION_TYPE, &dbStaRec.s_sta_type);
				glsStaNum = dbStaRec.s_sta_num_key;
				bFirstStation = FALSE;
			}	
			
			// create a new station record owned by current facility if it doesn't exist yet
			if (StationExistsInMainDb(dbStaRec.s_sta_num_key) != TRUE) {
				d_fillnew (DB_STA_REC, (char *) &dbStaRec, CURR_DB);
				d_connect (FACILITY_TO_STATION_SET, CURR_DB);
			}
			else {
				d_recwrite ((char *) &dbStaRec, CURR_DB);
			}   
			d_setor(STATION_TO_CHANNEL_SET, CURR_DB);

		} //end FOR each station in the facility
		delete [] pStaIDs; //free the memory for the string.
		
//    }//end FOR each facility ID
	
exit:
	SaveDatabaseNow();       // save the new parameters to the dbVista db
    return(TRUE);
}


