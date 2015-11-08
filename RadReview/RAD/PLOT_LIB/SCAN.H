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
#if ! defined HDR_scan // {
#define HDR_scan

/* ======================================================================= */
/* ================================ scan.h ================================ */
/* ======================================================================= */

/*+/mod/TOC------------------------------------------------------------------
* scan.h - definitions and prototypes for scan.c
*
* Purpose:
*		This header file supplies definitions and prototypes for use with
*		the SpwXxx() functions in scan.c.
*
* Usage:
*		#include <scan.h>
*
* Required for:
*		This header file is required for use with SpwXxx functions.
*
* Date     Author		Revision
* -------- ------------	--------
* 01-03-95 R. Cole		created
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
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined MAKE_DLL && defined WIN32
#define EXPORT16
#define EXPORT32 __declspec(dllexport)
#elif defined MAKE_DLL
#define EXPORT16 __export
#define EXPORT32
#else
#define EXPORT16
#define EXPORT32
#endif

EXPORT32 int EXPORT16 PASCAL
ScanDbl(
	const char **ppC,// IO pointer to pointer to current spot in buffer
	double	*pDbl);	// O place to store double value

EXPORT32 int EXPORT16 PASCAL
ScanField(
	const char **ppC,	// IO pointer to pointer to current spot in source string
	const char *szDelim,	// I pointer to delimiters
	const char **pszField);	// O place to store begin of field pointer

EXPORT32 int EXPORT16 PASCAL
ScanFlt(
	const char **ppC,// IO pointer to pointer to current spot in buffer
	float	*pFlt);	// O place to store float value

EXPORT32 int EXPORT16 PASCAL
ScanHex(
	const char **ppC,// IO pointer to pointer to current spot in buffer
	long	*pLng);	// O place to store long value

EXPORT32 int EXPORT16 PASCAL
ScanInt(
	const char **ppC,// IO pointer to pointer to current spot in buffer
	int		*pInt);	// O place to store integer value

EXPORT32 int EXPORT16 PASCAL
ScanLng(
	const char **ppC,// IO pointer to pointer to current spot in buffer
	long	*pLng);	// O place to store long value

EXPORT32 int EXPORT16 PASCAL
ScanSz(
	const char **ppC,// IO pointer to pointer to current spot in buffer
	int		iBufDim,// I dimension of buffer
	char	*szBuf);// O place to store string value

EXPORT32 int EXPORT16 PASCAL
ScanUint(
	const char **ppC,// IO pointer to pointer to current spot in buffer
	unsigned int *pUint);// O place to store unsigned integer value

#undef EXPORT16
#undef EXPORT32

#ifdef __cplusplus
}
#endif

#endif // ! defined HDR_scan }
