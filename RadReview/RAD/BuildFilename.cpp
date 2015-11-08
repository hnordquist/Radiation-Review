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
//    File Name : BuildFilename.cpp     
//Primary Author: Shirley Klosterbuer/JoAnn Painter

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include <gen.h>
#include "RAD.H"
#include "BuildFilename.h"
#include "MyDateTime.h"

extern CMyDateTime glMyDateTime;


/* zeroes are needed in tables so that, for example, when a 2 is used as an
   index, I get back a '2' and not a '3' which would be the case if no zero */

static char gll_mon[13] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
             'A', 'B', 'C'};

static char gll_day[32] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
             'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
             'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
             'U', 'V'};

static char gll_hour[24] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
              'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
              'U', 'V', 'W', 'X'};

static char gll_det[36] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
             'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
             'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
             'U', 'V', 'W', 'X', 'Y', 'Z'};

static char gll_yr[62] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
             'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
             'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
             'U', 'V', 'W', 'X', 'Y', 'Z',
             'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
             'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
             'u', 'v', 'w', 'x', 'y', 'z'};



 /*====================================================================
 *
 *  Module Name:    BLDFNAME.CPP
 *
 *  Function Name:  BuildFilename()
 *
 *  Function Purpose:   Using the input parameters for date, time, and
 *                      detector number, build a DOS filename base
 *                      (that is, excluding extension) according to:
 *                       char position   definition
 *                       1       detector number
 *                       2       year (0-9 = 1990-1999,
 *				       A-Z = 2000-2025,
 *				       a-z = 2026-2051)
 *                       3       month (1-C)
 *                       4       day (1-V)
 *                       5       hour (A-X)
 *                       6 & 7   minute (always 00)
 *                       8       not used
 *
 *  Return value:   none
 *
 *  Revision History
 *
 *     DATE AUTHOR  VER REVISION
 *  ----------- ------- --- --------
 *  20-Jan-1992 jap 3.0 created
 *  25-Aug-1993 SFK     Revised to use dt_info fields
 *  03-Apr-1998 SFK	Revised to use gen seconds times
 *  23-Sep-1998 WCH	Revised year to use 0-9, A-Z and a-z
 *	12-Aug-2002	SFK	Changed to DATE
 *  03-Jan-2006 SFK	Changed to put 2 digit detector number rather than something from 0-Z
 *					This change was for DR00219
 *
 *====================================================================*/
//void BuildFilename(unsigned long ulTime, int iDetNum, char *pszExtension, char *pszFileName)
void BuildFilename(DATE dTime, int iDetNum, char *pszExtension, char *pszFileName)

{
	char szDate[DT_LEN+1], szTime[DT_LEN+1];
	int iMin, iHr, iDay, iMon, iYr;
	glMyDateTime.DATETimestampToDateTimeStrs(dTime, szDate, szTime);

	szDate[2] = '\0';
	szDate[5] = '\0';
	szTime[2] = '\0';
	szTime[5] = '\0';

	iMin = atoi(&szTime[3]);
	iHr = atoi(&szTime[0]);
	iDay = atoi(&szDate[6]);
	iMon = atoi(&szDate[3]);
	iYr = atoi(&szDate[0]);

	if (iYr > 51)
	    iYr = iYr % 10;		// Note: handles any ten years after 1951
	else
	    iYr += 10;

	
	while (iDetNum > 100) iDetNum = iDetNum - 100;	// DR00219 03-Jan-2006 SFK Handle case of detector number > 100

    //sprintf(pszFileName, "%c%c%c%c%c%02d",
	sprintf(pszFileName, "%02d%c%c%c%c%02d",		// DR00219 03-Jan-2006 SFK	Modified to print detector number
		      //gll_det[iDetNum],
					  iDetNum,						// DR00219 03-Jan-2006 SFK  Modified to print detector number, not 0-Z
                      gll_yr[iYr],
                      gll_mon[iMon],
                      gll_day[iDay],
                      gll_hour[iHr],
                      iMin);
    strcat(pszFileName, pszExtension);

    return;
}
