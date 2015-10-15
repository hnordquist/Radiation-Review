//SafeArrayUtil.h

#pragma once

#include <afxdisp.h>  //COleDateTime

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