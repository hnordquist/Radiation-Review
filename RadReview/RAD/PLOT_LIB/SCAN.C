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
/* ======================================================================= */
/* =============================== scan.c ================================ */
/* ======================================================================= */

/*---------------------------------------------------------------------------
*                         SPECIAL NOTE TO DEVELOPERS
*
*		Information from this file is extracted to create portions of
*		WMCA_UM.DOC.  Because of this, the appearance of that document
*		is highly dependent on the formatting of information in this file.
*		Thus, it is recommended to remain consistent with existing format
*		conventions in this file.  Please observe the following:
*		o	keep the use of tabs and spaces consistent with existing usage
*			in this file
*		o	set tab width to 4
*		o	use "preserve tabs"
*		o	keep extracted lines shorter than 80 characters
*--------------------------------------------------------------------------*/

/*+/mod/TOC------------------------------------------------------------------
* scan.c - "scanf"-like utility functions
*
* Purpose:
*		The ScanXxx() functions allow "scanf"-like operations on a character
*		string.  These functions are intended for use when scanf and its
*		related functions aren't available and when the atoi, atod, etc.
*		conversion functions are either inappropriate or too cumbersome
*		to use.
*
* Quick Reference:
*    int ScanDbl    (<>ppC, >pDouble)
*    int ScanField  (<>ppC, szDelim, >pszField)
*    int ScanFlt    (<>ppC, >pFloat)
*    int ScanHex    (<>ppC, >pLong)
*    int ScanInt    (<>ppC, >pInt)
*    int ScanLng    (<>ppC, >pLong)
*    int ScanSz     (<>ppC, iBufDim, >szBuf)
*    int ScanUint   (<>ppC, >pUint)
*
* Example:
*    #include <stdio.h>
*    #include <scan.h>
*
*          char   *pC, szBuf[100];
*          int    stat;
*          FILE   *pFile;
*          int    intVal;
*
*          pC = fgets(szBuf, 100, pFile);
*          if (pC == NULL) {
*              if (feof(pFile)) {end of file action}
*              if (ferror(pFile)) {file I/O error action}
*          }
*
*          stat = ScanInt(&pC, &intVal);
*          if (stat != 1) {error action}
*
*          printf("Value=%d, delim=0x%x\n", intVal, *(pC-1));
*
*
* BUGS:
* 1.	Doesn't handle hexadecimal or octal integer values.
* 2.	See also the BUGS: section for the various functions.
*
* Date     Author		Revision
* -------- ------------	--------
* 01-05-95 R. Cole		created
*
*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
* Copyright 1995-97, The Regents Of the University of California.  This
* program was prepared by the Regents of the University of California at
* Los Alamos National Laboratory (the University) under Contract No. W-7405-
* ENG-36 with the U.S. Department of Energy (DOE).  The University has
* certain rights in the program pursuant to the contract and the program
* should not be copied or distributed outside your organization.  All rights
* in the program are reserved by the DOE and the University.  Neither the
* U.S. Government nor the University makes any warranty express or implied,
* or assumes any liability or responsibility for the use of this software.
*
* This software was produced by the Safeguards Science and Technology
* Group (NIS-5).
*--------------------------------------------------------------------------*/

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "scan.h"

#if defined SCAN_TEST // {
#include <stdio.h>
#include <string.h>
int main()
{
	const char *pC;
	int		iVal;
	unsigned int uiVal;
	long	lVal;
	float	fVal;
	char	szBuf[120];

	pC = "";
	if (ScanInt(&pC, &iVal) != 0) goto EOS_expected;
	pC = "\n";
	if (ScanInt(&pC, &iVal) != 0) goto EOS_expected;
	pC = "a";
	if (ScanInt(&pC, &iVal) != 0) goto bad_expected;
	if (*(pC-1) != 'a') goto bad_delim;

	pC = "1 +2   -3 -44 +55";
	if (ScanInt(&pC, &iVal) == 0) goto bad_convert;
	if (iVal != 1) goto bad_value;
	if (ScanInt(&pC, &iVal) == 0) goto bad_convert;
	if (iVal != 2) goto bad_value;
	if (ScanInt(&pC, &iVal) == 0) goto bad_convert;
	if (iVal != -3) goto bad_value;
	if (ScanInt(&pC, &iVal) == 0) goto bad_convert;
	if (iVal != -44) goto bad_value;
	if (ScanInt(&pC, &iVal) == 0) goto bad_convert;
	if (iVal != 55) goto bad_value;
	if (*pC != '\0') goto EOS_expected;
	if (ScanInt(&pC, &iVal) != 0) goto EOS_expected;

	pC = "1 -2   32767 33000";
	if (ScanUint(&pC, &uiVal) == 0) goto bad_convert;
	if (uiVal != 1) goto bad_value;
	if (ScanUint(&pC, &uiVal) != 0) goto bad_expected;
	if (ScanUint(&pC, &uiVal) == 0) goto bad_convert;
	if (uiVal != 32767) goto bad_value;
	if (ScanUint(&pC, &uiVal) == 0) goto bad_convert;
	if (uiVal != 33000) goto bad_value;

	pC = "32767 -32767 33000 -33000";
	if (ScanInt(&pC, &iVal) == 0) goto bad_convert;
	if (iVal != 32767) goto bad_value;
	if (ScanInt(&pC, &iVal) == 0) goto bad_convert;
	if (iVal != -32767) goto bad_value;
	if (ScanInt(&pC, &iVal) != 0) goto bad_expected;
	if (ScanInt(&pC, &iVal) != 0) goto bad_expected;

	pC = "1, 2;   3	4\n";
	if (ScanInt(&pC, &iVal) == 0) goto bad_convert;
	if (*(pC-1) != ',') goto bad_delim;
	if (ScanInt(&pC, &iVal) == 0) goto bad_convert;
	if (*(pC-1) != ';') goto bad_delim;
	if (ScanInt(&pC, &iVal) == 0) goto bad_convert;
	if (*(pC-1) != '	') goto bad_delim;
	if (ScanInt(&pC, &iVal) == 0) goto bad_convert;
	if (*(pC-1) != '\n') goto bad_delim;

	pC = "2147483647 -2147483647\n";
	if (ScanLng(&pC, &lVal) == 0) goto bad_convert;
	if (lVal != 2147483647) goto bad_value;
	if (ScanLng(&pC, &lVal) == 0) goto bad_convert;
	if (lVal != -2147483647) goto bad_value;

#define CHK_FLT_ERR(fArg) fabs(fVal - fArg) > .000001
	pC = "1. +2. -3. 1.234";
	if (ScanFlt(&pC, &fVal) == 0) goto bad_convert;
	if (CHK_FLT_ERR(1.)) goto bad_value;
	if (ScanFlt(&pC, &fVal) == 0) goto bad_convert;
	if (CHK_FLT_ERR(2.)) goto bad_value;
	if (ScanFlt(&pC, &fVal) == 0) goto bad_convert;
	if (CHK_FLT_ERR(-3.)) goto bad_value;
	if (ScanFlt(&pC, &fVal) == 0) goto bad_convert;
	if (CHK_FLT_ERR(1.234)) goto bad_value;
	if (*pC != '\0') goto EOS_expected;
	if (ScanFlt(&pC, &fVal) != 0) goto EOS_expected;

	pC = "1.e3 +2.d-12 -3.E+1 1.234D0";
	if (ScanFlt(&pC, &fVal) == 0) goto bad_convert;
	if (CHK_FLT_ERR(1.e3)) goto bad_value;
	if (ScanFlt(&pC, &fVal) == 0) goto bad_convert;
	if (CHK_FLT_ERR(2.e-12)) goto bad_value;
	if (ScanFlt(&pC, &fVal) == 0) goto bad_convert;
	if (CHK_FLT_ERR(-3.E+1)) goto bad_value;
	if (ScanFlt(&pC, &fVal) == 0) goto bad_convert;
	if (CHK_FLT_ERR(1.234E0)) goto bad_value;
	if (*pC != '\0') goto EOS_expected;
	if (ScanFlt(&pC, &fVal) != 0) goto EOS_expected;

#define CHK_SZ_ERR(szArg) strcmp(szArg, szBuf) != 0
	pC = "a abc   abcdefghij";
	szBuf[9] = 'Z';
	if (ScanSz(&pC, 9, szBuf) == 0) goto bad_convert;
	if (CHK_SZ_ERR("a")) goto bad_value;
	if (szBuf[9] != 'Z') goto szBuf_clobbered;
	if (ScanSz(&pC, 9, szBuf) == 0) goto bad_convert;
	if (CHK_SZ_ERR("abc")) goto bad_value;
	if (szBuf[9] != 'Z') goto szBuf_clobbered;
	if (ScanSz(&pC, 9, szBuf) != 0) goto bad_expected;
	if (CHK_SZ_ERR("abcdefgh")) goto bad_value;
	if (szBuf[9] != 'Z') goto szBuf_clobbered;

	pC = "\"a\", 'abc'\n";
	if (ScanSz(&pC, 9, szBuf) == 0) goto bad_convert;
	if (CHK_SZ_ERR("a")) goto bad_value;
	if (*(pC-1) != ',') goto bad_delim;
	if (ScanSz(&pC, 9, szBuf) == 0) goto bad_convert;
	if (CHK_SZ_ERR("abc")) goto bad_value;
	if (*(pC-1) != '\n') goto bad_delim;
	if (*pC != '\0') goto EOS_expected;
	if (ScanSz(&pC, 9, szBuf) != 0) goto EOS_expected;

	printf("Test succeeded\n");
	return 0;
EOS_expected:
	printf("Error: expected EOS\n");
	return 1;
bad_convert:
	printf("Error: conversion didn't succeed\n");
	return 1;
bad_value:
	printf("Error: value mismatch\n");
	return 1;
bad_expected:
	printf("Error: bad value didn't produce an error\n");
	return 1;
bad_delim:
	printf("Error: expected delimiter not found\n");
	return 1;
szBuf_clobbered:
	printf("Error: szBuf clobbered\n");
	return 1;
}
#endif 

int Preamble(const char **ppC, int *pC, int *piCnt)
{
	while (1) {
		*pC = **ppC;
		if (!isascii(*pC))
			return 3;		// illegal character
		if (*pC == '\0') {
			if (*piCnt == 0)
				return 1;	// premature end of string
			else
				return 2;	// done with field
		}
		*ppC += 1;
		if (*piCnt == 0 && isspace(*pC))
			continue;	// skip leading white space
		(*piCnt)++;
		return 0;	// *pC contains valid character in field
	}
}

void SkipTrailWhite(const char **ppC, int c)
{
	if (isspace(c)) {
		// if delim is white space, skip trailing white space
		while (1) {
			c = **ppC;
			if (isspace(c))
				*ppC += 1;
			else
				break;
		}
	}
}
/*+/csubr/TOC----------------------------------------------------------------
* ScanDbl - convert a text field to a double value
*
* Purpose:
*		Scans characters from a text string, converting the field into
*		a double value.
*
*		Leading white space is skipped.  Conversion stops when a non-
*		digit character is encountered.
*
*		If the delimiter that stops scanning is a white space character,
*		then trailing white space is skipped.
*
* Return Value:
*		1  if a value was successfully scanned, or
*		0  otherwise
*
* Notes:
* 1.	The caller's buffer pointer is incremented as the scan proceeds.
*		On entry, the pointer should be positioned at (or before) the
*		first character of the field.  At exit, the pointer will be
*		pointing to the character following the delimiter that stopped
*		the scan.
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-05-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
ScanDbl(
const char **ppC,		// IO pointer to pointer to current spot in source string
double	*pDbl)			// O place to store double value
{
	int		c, stat;
	char	szBuf[20], *pC;
	int		nDigits=0, bGotDot=0, bInExp=0, nExpDigits=0;

	*pDbl = 0.;
	stat = ScanSz(ppC, 20, szBuf);
	if (stat != 1)
		return 0;
	for (pC=szBuf; *pC!='\0'; pC++) {
		c = toupper(*pC);
		if (bInExp == 0) {
			if (isdigit(c))
				nDigits++;
			else if (c == '.') {
				if (bGotDot++ > 0)
					return 0;	// can't have 2 dots
			}
			else {
				if (c == 'E' || c == 'D')
					bInExp = 1;	// starting exponent
				else {
					if (nDigits > 0)
						return 0;	// sign isn't first char
					if (!(c == '+' || c == '-'))
						return 0;	// illegal char
				}
			}
		}
		else {
			if (isdigit(c))
				nExpDigits++;
			else {
				if (nExpDigits > 0)
					return 0;	// sign isn't first char
				if (!(c == '+' || c == '-'))
					return 0;	// illegal char
			}
		}
	}
	if (nDigits == 0)
		return 0;	// no digits!!
	if (bInExp && nExpDigits == 0)
		return 0;	// no digits in exponent!!
	*pDbl = atof(szBuf);
	return 1;
}
/*+/csubr/TOC----------------------------------------------------------------
* ScanField - scan the next field from a text string
*
* Purpose:
*		Scans the next field from a text string, returning a pointer to
*		the first character in the field and a count of characters in
*		the field.
*
*		Scanning is based on caller-supplied delimiter characters.  The
*		scan continues until a delimiter, '\n', '\r', or '\0' is
*		encountered.
*
*		Leading white space is skipped.  (Any white space characters that
*		are delimiters won't be skipped.)
*
*		Trailing white space characters that aren't delimiters are
*		included in the count of characters in the field.
*
*		For text that is enclosed in ' (or ") characters, the delimiter
*		check (except for '\0') is skipped.  The ' (or ") characters
*		are treated as being part of the field.
*
* Return Value:
*		number of characters in the field
*
* Notes:
* 1.	The caller's buffer pointer is incremented as the scan proceeds.
*		On entry, the pointer should be positioned at (or before) the
*		first character of the field.  At exit, the pointer will be
*		pointing to the character following the delimiter that stopped
*		the scan.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-27-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
ScanField(
const char **ppC,		// IO pointer to pointer to current spot in source string
const char *szDelim,	// I pointer to delimiters
const char **pszField)	// O place to store begin of field pointer
{
	int		nChar=0;
	char	*pField=NULL;
	int		bLeading=1;
	int		c, cQuote=0;

	*pszField = NULL;

	while (1) {
		c = **ppC;
		if (c == '\0') goto done;
		*ppC += 1;
		if (cQuote == 0) {
			// Check for delim only outside quoted text.
			if (c == '\n'|| c == '\r') goto done;
			if (strchr(szDelim, c) != NULL)
				goto done;
		}
		if (nChar == 0) {
			if (isascii(c) && isspace(c))
				continue;		// skip leading white space
			else
				*pszField = *ppC - 1;
		}
		if (c == '\'' || c == '"') {
			if (cQuote == 0)
				cQuote = c;	// start quoted text
			else if (cQuote == c)
				cQuote = 0;	// end quoted text
		}
		nChar++;
	}
done:
	return nChar;
}
/*+/csubr/TOC----------------------------------------------------------------
* ScanFlt - convert a text field to a float value
*
* Purpose:
*		Scans characters from a text string, converting the field into
*		a float value.
*
*		Leading white space is skipped.  Conversion stops when a non-
*		digit character is encountered.
*
*		If the delimiter that stops scanning is a white space character,
*		then trailing white space is skipped.
*
* Return Value:
*		1  if a value was successfully scanned, or
*		0  otherwise
*
* Notes:
* 1.	The caller's buffer pointer is incremented as the scan proceeds.
*		On entry, the pointer should be positioned at (or before) the
*		first character of the field.  At exit, the pointer will be
*		pointing to the character following the delimiter that stopped
*		the scan.
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-05-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
ScanFlt(
const char **ppC,		// IO pointer to pointer to current spot in source string
float	*pFlt)			// O place to store float value
{
	double	dVal;
	int		stat;

	stat = ScanDbl(ppC, &dVal);
	if (stat != 0) {
		if (fabs(dVal) > FLT_MAX)
			stat = 0;
		else
			*pFlt = (float)dVal;
	}

	return stat;
}
/*+/csubr/TOC----------------------------------------------------------------
* ScanHex - convert a hexadecimal text field to a long value
*
* Purpose:
*		Scans characters from a text string, converting the field into
*		a long value.
*
*		Leading white space is skipped.  Conversion stops when a non-
*		hex-digit character is encountered.
*
*		If the delimiter that stops scanning is a white space character,
*		then trailing white space is skipped.
*
* Return Value:
*		1  if a value was successfully scanned, or
*		0  otherwise
*
* Notes:
* 1.	The caller's buffer pointer is incremented as the scan proceeds.
*		On entry, the pointer should be positioned at (or before) the
*		first character of the field.  At exit, the pointer will be
*		pointing to the character following the delimiter that stopped
*		the scan.
*
*-Date     Author		Revision
* -------- ------------	--------
* 02-07-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
ScanHex(
const char **ppC,		// IO pointer to pointer to current spot in source string
long	*pLong)			// O place to store long value
{
	int		stat;
	char	szBuf[20], *pC;
	int		nDigits=0;
	long	lVal;

	*pLong = lVal = 0;
	stat = ScanSz(ppC, 20, szBuf);
	if (stat != 1)
		return 0;
	for (pC=szBuf; *pC!='\0'; pC++) {
		if (isdigit(*pC)) {
			nDigits++;
			lVal = (lVal << 4) + (*pC - '0');
		}
		else if (isxdigit(*pC)) {
			nDigits++;
			lVal = (lVal << 4) + (tolower(*pC) - 'a' + '10');
		}
		else
			return 0;	// illegal char
	}
	if (nDigits == 0)
		return 0;	// no digits!!

	*pLong = lVal;
	return 1;
}
/*+/csubr/TOC----------------------------------------------------------------
* ScanInt - convert a text field to an integer value
*
* Purpose:
*		Scans characters from a text string, converting the field into
*		an integer value.
*
*		Leading white space is skipped.  Conversion stops when a non-
*		digit character is encountered.
*
*		If the delimiter that stops scanning is a white space character,
*		then trailing white space is skipped.
*
* Return Value:
*		1  if a value was successfully scanned, or
*		0  otherwise
*
* Notes:
* 1.	The caller's buffer pointer is incremented as the scan proceeds.
*		On entry, the pointer should be positioned at (or before) the
*		first character of the field.  At exit, the pointer will be
*		pointing to the character following the delimiter that stopped
*		the scan.
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-05-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
ScanInt(
const char **ppC,		// IO pointer to pointer to current spot in source string
int		*pInt)			// O place to store integer value
{
	long	longVal;
	int		stat;

	stat = ScanLng(ppC, &longVal);
	if (stat == 1) {
		if (longVal <= INT_MAX && longVal >= INT_MIN)
			*pInt = (int)longVal;
		else
			stat = 0;
	}

	return stat;
}
/*+/csubr/TOC----------------------------------------------------------------
* ScanLng - convert a text field to a long value
*
* Purpose:
*		Scans characters from a text string, converting the field into
*		a long value.
*
*		Leading white space is skipped.  Conversion stops when a non-
*		digit character is encountered.
*
*		If the delimiter that stops scanning is a white space character,
*		then trailing white space is skipped.
*
* Return Value:
*		1  if a value was successfully scanned, or
*		0  otherwise
*
* Notes:
* 1.	The caller's buffer pointer is incremented as the scan proceeds.
*		On entry, the pointer should be positioned at (or before) the
*		first character of the field.  At exit, the pointer will be
*		pointing to the character following the delimiter that stopped
*		the scan.
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-05-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
ScanLng(
const char **ppC,		// IO pointer to pointer to current spot in source string
long	*pLong)			// O place to store long value
{
	int		stat;
	char	szBuf[20], *pC;
	int		nDigits=0;

	*pLong = 0;
	stat = ScanSz(ppC, 20, szBuf);
	if (stat != 1)
		return 0;
	for (pC=szBuf; *pC!='\0'; pC++) {
		if (isdigit(*pC))
			nDigits++;
		else {
			if (pC != szBuf)
				return 0;	// sign isn't first char
			if (!(*pC == '+' || *pC == '-'))
				return 0;	// illegal char
		}
	}
	if (nDigits == 0)
		return 0;	// no digits!!
	*pLong = atol(szBuf);
	return 1;
}
/*+/csubr/TOC----------------------------------------------------------------
* ScanSz - convert a text field to a text value
*
* Purpose:
*		Scans characters from a text string, converting the field into
*		a text value.
*
*		Leading white space is skipped.  Conversion stops when a non-
*		alphanumeric character is encountered.  If the field is enclosed
*		with " or ' characters, then conversion continues until the
*		matching character is encountered.
*
*		If the delimiter that stops scanning is a white space character,
*		then trailing white space is skipped.
*
* Return Value:
*		1  if a value was successfully scanned, or
*		0  otherwise
*
* Notes:
* 1.	The caller's buffer pointer is incremented as the scan proceeds.
*		On entry, the pointer should be positioned at (or before) the
*		first character of the field.  At exit, the pointer will be
*		pointing to the character following the delimiter that stopped
*		the scan.
* 2.	For fields not enclosed by " or ' characters, the field may
*		contain: letters; numerals; and "_+-.".  Any other characters are
*		treated as delimiters and terminate the scan.
* 3.	For fields enclosed by " or ' characters, the " or ' at the
*		begin and end of the field are not stored in the caller's
*		buffer.  These fields can contain any printable characters;
*		control characters (such as '\n') aren't allowed.
* 4.	If the field is too long to fit in the buffer, then the return
*		status indicates an error.  However, the buffer in this case
*		contains the first part of the field, with a '\0' terminator.
*		The buffer pointer will be 1 past the character that couldn't
*		be stored.
*
* BUGS:
* 1.	This function doesn't allow embedded " characters in a field
*		that is delimited by " characters, and similarly for ' delimited
*		fields.
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-05-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
ScanSz(
const char **ppC,		// IO pointer to pointer to current spot in source string
int		iBufDim,		// I dimension of szBuf
char	*szBuf)			// O place to store string value
{
	int		c, iCnt=0, stat, iStored=0;
	int		cQuote=0;
	char	*pC=szBuf;

	*pC = '\0';
	while (1) {
		if ((stat = Preamble(ppC, &c, &iCnt)) != 0) {
			if (stat == 1) return 1;	// empty field
			if (stat == 2) break;		// end of field
			if (stat == 3) return 0;	// illegal char
		}
		if (c == '\'' || c == '"') {
			if (iCnt == 1) {
				cQuote = c;
				continue;	// start a quoted field
			}
			else if (cQuote == c) {
				c = **ppC;
				if (c != '\0')
					*ppC += 1;
				break;	// matching quote; done with field
			}
		}
		else if (cQuote == 0) {
			// non-quoted fields only allow "alphanumeric"
			if (!(isalnum(c) || c == '_' || c == '+' ||
						c == '-' || c == '.')) {
				if (iCnt == 1)
					return 0;	// delim was first character
				else
					break;		// end of field
			}
		}
		else if (iscntrl(c))
			return 0;	// control char in quoted field
		if (++iStored >= iBufDim)
			return 0;	// field was too big for buffer
		*(pC++) = c;
		*pC = '\0';
	}
	SkipTrailWhite(ppC, c);
	return 1;
}
/*+/csubr/TOC----------------------------------------------------------------
* ScanUint - convert a text field to an unsigned integer value
*
* Purpose:
*		Scans characters from a text string, converting the field into
*		an unsigned integer value.
*
*		Leading white space is skipped.  Conversion stops when a non-
*		digit character is encountered.
*
*		If the delimiter that stops scanning is a white space character,
*		then trailing white space is skipped.
*
* Return Value:
*		1  if a value was successfully scanned, or
*		0  otherwise
*
* Notes:
* 1.	The caller's buffer pointer is incremented as the scan proceeds.
*		On entry, the pointer should be positioned at (or before) the
*		first character of the field.  At exit, the pointer will be
*		pointing to the character following the delimiter that stopped
*		the scan.
*
*-Date     Author		Revision
* -------- ------------	--------
* 03-16-95 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
ScanUint(
const char **ppC,		// IO pointer to pointer to current spot in source string
unsigned int *pUint)	// O place to store unsigned integer value
{
	long	longVal;
	int		stat;

	stat = ScanLng(ppC, &longVal);
	if (stat == 1) {
		if (longVal <= UINT_MAX && longVal >= 0)
			*pUint = (unsigned int)longVal;
		else
			stat = 0;
	}

	return stat;
}
