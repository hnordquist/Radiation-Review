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
