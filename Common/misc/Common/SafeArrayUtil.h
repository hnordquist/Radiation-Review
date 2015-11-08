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
//SafeArrayUtil.h

#pragma once

#include <afxdisp.h>  //COleDateTime/*


// CString
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, CString *pstrOut);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, const CString &strIn);
bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, CString *pstrOut);
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, const CString &strIn);

// BSTR
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, const BSTR &bstrIn);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, const BSTR &bstrIn);

// long
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, long *plValue);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, long lValue);
bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, long *plValue);
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, long lValue);

// short
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, short *psValue);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, short sValue);
bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, short *psValue);
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, short sValue);

// int
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, int *pnValue);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, int nValue);
bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, int *pnValue);
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, int nValue);

// bool
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, bool *pbValue);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, bool bValue);
bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, bool *pbValue);
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, bool bValue);

// COleDateTime
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, COleDateTime *podtValue);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, COleDateTime odtValue);
bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, COleDateTime *podtValue);
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, COleDateTime odtValue);

// COleDateTimeSpan
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, COleDateTimeSpan *podtsValue);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, COleDateTimeSpan odtsValue);
bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, COleDateTimeSpan *podtsValue);
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, COleDateTimeSpan odtsValue);

// double
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, double *pdValue);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, double dValue, bool bDATE = false);
bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, double *pdValue);
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, double dValue, bool bDATE = false);

//DATE - This doesn't work - DATE is seen as double by the compiler
//bool SA_GetElem(SAFEARRAY *pArray, long lIndex, DATE *pdValue);
//bool SA_PutElem(SAFEARRAY *pArray, long lIndex, DATE dValue);
//bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, DATE *pdValue);
//bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, DATE dValue);

//DATE - But this works.  Need to fill in the other functions.
//bool SA_GetElem(SAFEARRAY *pArray, long lIndex, DATE *pdValue);
//bool SA_PutElem(SAFEARRAY *pArray, long lIndex, DATE dValue);
//bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, DATE *pdValue);
//bool SA_PutElemDATE(SAFEARRAY *pArray, long lRow, long lColumn, DATE dValue);


// float
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, float *pfValue);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, float fValue);
bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, float *pfValue);
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, float fValue);

// SAFEARRAY
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, VARIANT *pArrayOut);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, const VARIANT *pArrayIn);
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, VARIANT& ArrayOut);
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, const VARIANT& ArrayOut);

// Size of SAFEARRAY, regardless of bounds
long SA_GetSize(SAFEARRAY *pArray, long lDim);

// Debug function to output the content of a SAFEARRAY in text.
// Courtesy Joe Longo 2 May 2007
void VGetImage(VARIANT& v);