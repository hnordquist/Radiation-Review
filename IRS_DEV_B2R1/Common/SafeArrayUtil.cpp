
#include "stdafx.h"
#include "atlbase.h"
#include "SafeArrayUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Tom Marks wrote for 1-dimensional arrays
// SFK extended to 2-dimensional safe arrays and changed file name from SA_Util to SafeArrayUtil


// CString /////////////////////////////////////////////////////////////////////
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, CString *pstrOut)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);

#ifdef _DEBUG
			ASSERT(Var.vt == VT_BSTR);
			VGetImage(Var);
#endif

			CString s(Var.bstrVal == NULL ? L"" : Var.bstrVal);
			*pstrOut = s;

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory

			//Adjust bSuccess LAST in case something blows in between where the hr is 
			//set and the function is supposed to return.
			if (hr == S_OK)	
				bSuccess = true;

		} // if parray
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lIndex, const CString &strIn)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			BSTR bstr = strIn.AllocSysString();
			CComVariant Var(bstr);
			::SysFreeString(bstr);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}


bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, CString *pstrOut)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			CString s(Var.bstrVal == NULL ? L"" : Var.bstrVal);
			*pstrOut = s;

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, const CString &strIn)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			BSTR bstr = strIn.AllocSysString();
			CComVariant Var(bstr);
			::SysFreeString(bstr);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, const BSTR &bstrIn)
{
	bool bSuccess = false;
	try
	{
		if (pArray)
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			CComVariant Var(bstrIn);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lIndex, const BSTR &bstrIn)
{
	bool bSuccess = false;
	try
	{
		if (pArray)
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			CComVariant Var(bstrIn);
			
			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}



// long ///////////////////////////////////////////////////////////////////////////////////////
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, long *plValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;
			
			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*plValue = Var.lVal;

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lIndex, long lValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			CComVariant Var(lValue);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, long *plValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{		
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*plValue = Var.lVal;

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, long lValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			CComVariant Var(lValue);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

// short ///////////////////////////////////////////////////////////////////////////////////////
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, short *psValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*psValue = (short)Var.iVal;

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lIndex, short sValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			CComVariant Var(sValue);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, short *psValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*psValue = (short)Var.iVal;

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, short sValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			CComVariant Var(sValue);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

// int /////////////////////////////////////////////////////////////////////
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, int *pnValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			*pnValue = Var.iVal;
			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}
 
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, int nValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			CComVariant Var(nValue);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, int *pnValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*pnValue = Var.iVal;

			VariantClear(&Var);
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, int nValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			CComVariant Var(nValue);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

// bool  ///////////////////////////////////////////////////////////////////////////////////
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, bool *pbValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*pbValue = Var.bVal?true:false;

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lIndex, bool bValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray)
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			CComVariant Var;
			Var.vt = VT_BOOL;
			Var = bValue;

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, bool *pbValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*pbValue = Var.bVal?true:false;

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, bool bValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			CComVariant Var;
			Var.vt = VT_BOOL;
			Var.iVal = bValue;//?VARIANT_TRUE:VARIANT_FALSE;

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

// COleDateTime ////////////////////////////////////////////////////////////////////
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, COleDateTime *podtValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*podtValue = Var.date;

			VariantClear(&Var);
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}
/*
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, COleDateTime odtValue)
{
		bool bSuccess = false;
	long alIndex[1];
	CComVariant Var;

	if (pArray) {
		alIndex[0] = lIndex;
		Var = odtValue;
		Var.vt = VT_DATE;
		HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);
		if (hr == S_OK) bSuccess = true;
	}
	return(bSuccess);
}
*/
// HN - 6/26/2006
// Changed this code to use a VARIANT instead of CComVariant.  The VT_DATE setting was not working for this.
// Instead, just use the raw VARIANT and set vt to DATE type.
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, COleDateTime odtValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
            alIndex[0] = lIndex;

			VARIANT Var;
			VariantInit (&Var);

            Var.vt = VT_DATE;
            Var.date = odtValue.m_dt;

            HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

            VariantClear(&Var);
            if (hr == S_OK) bSuccess = true;
        }
	}
	catch(...){}
	return(bSuccess);
}

bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, COleDateTime *podtValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			VARIANT Var;
			VariantInit(&Var);

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*podtValue = Var.date;

			VariantClear(&Var);
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}
/*
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, COleDateTime odtValue)
{
		bool bSuccess = false;
	long alIndex[2];
	CComVariant Var;

	if (pArray) {
		alIndex[0] = lRow;
		alIndex[1] = lColumn;
		Var = odtValue;
		HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);
		if (hr == S_OK) bSuccess = true;
	}
	return(bSuccess);
}
*/
// HN - 6/26/2006
// Changed this code to use a VARIANT instead of CComVariant.  The VT_DATE setting was not working for this.
// Instead, just use the raw VARIANT and set vt to DATE type.
bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, COleDateTime odtValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
            alIndex[1] = lColumn;

			VARIANT Var;
			VariantInit (&Var);

            Var.vt = VT_DATE;
            Var.date = odtValue.m_dt;

            HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			VariantClear(&Var);
            if (hr == S_OK) bSuccess = true;
        }

	}
	catch(...){}
	return(bSuccess);
}

// COleDateTimeSpan ///////////////////////////////////////////////////////////////////
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, COleDateTimeSpan *podtsValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*podtsValue = Var.date;

			VariantClear(&Var);	// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;			
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lIndex, COleDateTimeSpan odtsValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			CComVariant Var;
			Var = odtsValue;

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, COleDateTimeSpan *podtsValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*podtsValue = Var.date;

			VariantClear(&Var);	
			if (hr == S_OK) bSuccess = true;	
		}
		// 03-Jan-2006 SFK Free the variant memory
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, COleDateTimeSpan odtsValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			CComVariant Var;
			Var = odtsValue;

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

// double  //////////////////////////////////////////////////////////////////////////
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, double *pdValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*pdValue = Var.dblVal;

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lIndex, double dValue, bool bDate)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			CComVariant Var(dValue);
			if (bDate)
				Var.vt = VT_DATE;

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, double *pdValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*pdValue = Var.dblVal;

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;

		}
	}
	catch(...){}
	return(bSuccess);
}



bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, double dValue, bool bDate)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			CComVariant Var(dValue);

			if (bDate)
				Var.vt = VT_DATE;

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

// DATE  //////////////////////////////////////////////////////////////////////////

//bool SA_GetElem(SAFEARRAY *pArray, long lIndex, DATE *pdValue){;}
//bool SA_PutElem(SAFEARRAY *pArray, long lIndex, DATE dValue){;}
//bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, DATE *pdValue){;}
//bool SA_PutElemDATE(SAFEARRAY *pArray, long lRow, long lColumn, DATE dValue)
//{
//	return SA_PutElem(pArray,lRow,lColumn,dValue, true);
//}


// float  //////////////////////////////////////////////////////////////////////////
bool SA_GetElem(SAFEARRAY *pArray, long lIndex, float *pfValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{	
			long alIndex[1];
			alIndex[0] = lIndex;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*pfValue = (float)Var.dblVal;

			VariantClear(&Var);	
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lIndex, float fValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			CComVariant Var((double)fValue);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_GetElem(SAFEARRAY *pArray, long lRow, long lColumn, float *pfValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			VARIANT Var;
			VariantInit(&Var);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &Var);
			*pfValue = (float)Var.dblVal;

			VariantClear(&Var);			// 03-Jan-2006 SFK Free the variant memory
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

bool SA_PutElem(SAFEARRAY *pArray, long lRow, long lColumn, float fValue)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[2];
			alIndex[0] = lRow;
			alIndex[1] = lColumn;

			CComVariant Var((double)fValue);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);
			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

// 19-Oct-2005 SFK	Added general function to get size of Safearray
long SA_GetSize(SAFEARRAY *pArray, long lDim)
{
	long lSize = 0;
	try
	{
		if (pArray)
		{
			long lLowBound, lUpBound;
			SafeArrayGetLBound(pArray, lDim, &lLowBound);	// get the lower bound
			SafeArrayGetUBound(pArray, lDim, &lUpBound);	// how many channels did we get information about
			lSize = lUpBound - lLowBound + 1;			// calculate the real size of the array
		}
	}
	catch(...){}
	return (lSize);
}

// 12-Oct-2005 SFK	Extended to use VARIANT SafeArray
//bool SA_GetElem(SAFEARRAY *pArray, long lIndex, SAFEARRAY **pArrayOut)
//{
//		bool bSuccess = false;
//	long alIndex[1];

//	if (pArray) {
//		alIndex[0] = lIndex;
//		HRESULT hr = SafeArrayGetElement(pArray, alIndex, &pArrayOut);
//		if (hr == S_OK) bSuccess = true;
//	}
//	return(bSuccess);
//}

//bool SA_PutElem(SAFEARRAY *pArray, long lIndex, const SAFEARRAY &pArrayIn)
//{
//		bool bSuccess = false;
//	long alIndex[1];

//	CComVariant Var;
	
//	if (pArray) {
//		Var.pvarVal = (SAFEARRAY *)pArrayIn;
//		alIndex[0] = lIndex;
//		HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);
//		if (hr == S_OK) bSuccess = true;
//	}
//	return(bSuccess);
//}

bool SA_GetElem(SAFEARRAY *pArray, long lIndex, VARIANT& VarOut)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			VariantInit(&VarOut);				// 03-Jan-2006 SFK Always init variants

			HRESULT hr = SafeArrayGetElement(pArray, alIndex, &VarOut);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}
bool SA_PutElem(SAFEARRAY *pArray, long lIndex, const VARIANT& VarIn)
{
	bool bSuccess = false;
	try
	{
		if (pArray) 
		{
			long alIndex[1];
			alIndex[0] = lIndex;

			CComVariant Var(VarIn);

			HRESULT hr = SafeArrayPutElement(pArray, alIndex, &Var);

			if (hr == S_OK) bSuccess = true;
		}
	}
	catch(...){}
	return(bSuccess);
}

//
// CODE FOR DEBUG TO PRINT OUT THE CONTENT OF A SAFEARRAY
// Courtesy Joe Longo 2 May 2007
//

void OutputDebugText(CString& cs)
{
	OutputDebugString(LPCTSTR(cs));
}

void VGetImage(VARIANT& v)
{
//#ifdef _DEBUG
#if(0)
	CString var(""); 

		switch (v.vt)
		{
			case VT_EMPTY:
				OutputDebugText(CString("VT_EMPTY\n"));
				break;
			case VT_NULL:
				OutputDebugText(CString("VT_NULL\n"));
				//var.Format("ø");
				break;
			case     VT_UI1:
				var.Format("VT_UI1\t%hu\n", v.bVal);
				OutputDebugText(var);
				break;
			case     VT_I2:
				{
				//CString cs;
				//cs.Format("ix[1] = %d\n", ix[1]);
				//var.Format("%d VT_I2\n", v.iVal);
				var.Format("VT_I2\t%d\n",v.iVal);
				OutputDebugText(var);
				break;
				}
			case     VT_I4:
				var.Format("VT_I4\t%ld\n", v.lVal);
				OutputDebugText(var);
				break;
			case     VT_R4:
				var.Format("VT_R4\t%10.6f\n", v.fltVal);
				OutputDebugText(var);
				break;
			case     VT_R8:
				var.Format("VT_R8\t%10.6lf\n", v.dblVal);
				OutputDebugText(var);
				break;
			case     VT_DATE:
				{
					COleDateTime d(v.date);
                    var.Format("VT_DATE\t%.32s %10.6lf\n", d.Format(), d.m_dt);
					OutputDebugText(var);
				}
				break;
			case     VT_BSTR:
				var = "VT_BSTR\t";
				var = var + v.bstrVal;
				var += "\n";
				OutputDebugText(var);
				break;
			case     VT_DISPATCH:
				var.Format( "VT_DISPATCH\t%x", v.pdispVal);
				OutputDebugText(var);
				break;
			case     VT_BOOL:
				var.Format( v.boolVal ? _T("true") : _T("false") );
				var = "VT_BOOL\t" + var + "\n";
				OutputDebugText(var);
				break;
			case     VT_UNKNOWN:
				var.Format( "IUnknown %x", v.pdispVal);
				break;
			case     VT_I1:
				var.Format( "%c VT_I1", v.cVal);
				break;
			case     VT_UI2:
				var.Format("%hu VT_UI2", v.uiVal);
				break;
			case     VT_UI4:
				var.Format("%u VT_UI4",  v.ulVal);
				break;
			case     VT_INT:
				var.Format("%d VT_INT",  v.intVal);
				break;
			case     VT_UINT:
				var.Format("%u VT_UINT",  v.uintVal);
				break;
				/*
			case     VT_ARRAY | VT_VARIANT:
				{
					LONG LB, UB, ix;
					HRESULT hr  = ::SafeArrayGetUBound(v.parray, 1, &UB);
					hr = ::SafeArrayGetLBound(v.parray, 1, &LB);
					CString arrvar("");
					HRESULT hri;
					for (ix = LB; ix <= UB; ix++)
					{
						VARIANT inv;
						::VariantInit(&inv);
						CString element;
						hri = ::SafeArrayGetElement(v.parray, &ix, &inv);
						if (hri == S_OK)
						{
							VGetImage(element, inv);
						}
						else
						{
							element = "*";
						}
						//arrvar.AppendFormat("[%2.2d] %s, ", ix, element);
						CString temp;
						temp.Format("[%2.2d] %s, ", ix, element);
						arrvar += temp;
					}
					var = arrvar;
				}
			break;
			*/
			case     VT_ARRAY | VT_VARIANT:
				{
					CString cs;

                    UINT dims = ::SafeArrayGetDim(v.parray);
                    if (dims == 1)
                    {
					    LONG LB, UB, ix;
					    HRESULT hr  = ::SafeArrayGetUBound(v.parray, 1, &UB);
					    hr = ::SafeArrayGetLBound(v.parray, 1, &LB);
					    CString arrvar(L"<br>");
					    arrvar.Format("%2.ld values<br>", (UB - LB) + 1);
					    HRESULT hri;
					    for (ix = LB; ix <= UB; ix++)
					    {
						    //DOUBLE ind = 0;
							VARIANT ind;
							::VariantInit(&ind);
						    CString element;
						    hri = ::SafeArrayGetElement(v.parray, &ix, &ind);
						    if (hri == S_OK)
						    {
							    //element.Format("%10.6lf",ind);
								VGetImage(ind);
						    }
						    else
						    {
							    element = "*";
						    }
						    if (ix < UB)
							{
								cs.Format("[%2.1ld] %s,", ix, element);
								arrvar += cs;
							    //arrvar.AppendFormat("[%2.1ld] %s,", ix, element);
							}
						    else
							{
								cs.Format("[%2.1ld] %s", ix, element);
								arrvar += cs;
							    //arrvar.AppendFormat("[%2.1ld] %s", ix, element);
							}
						    if (ix > 0 && (ix % 4) == 0)
							    arrvar += L"<br>";
					    }		
					    var = arrvar;
                    }
                    else  //assume 2 dimensions
                    {
					    LONG UB[2], ix[2];  // assuming starting with 0
					    HRESULT hr  = ::SafeArrayGetUBound(v.parray, 1, &UB[0]);
					    hr  = ::SafeArrayGetUBound(v.parray, 2, &UB[1]);

					    //CString arrvar;
					    //arrvar.Format("\n%2.ld x %2.ld values\n\n", (UB[0]) + 1, (UB[1] ) + 1);
					    HRESULT hri;

					    for (ix[0] = 0; ix[0] <= UB[0]; ix[0]++)
					    {
						    //DOUBLE ind = 0;
							CString cs;
							cs.Format("\nSAFEARRAY #%d of %d (index = %d)\n", ix[0]+1, UB[0]+ 1,ix[0]);
							//cs.Format("%d\t",ix[0];
							OutputDebugText(cs);

							VARIANT ind;
							VariantInit(&ind);
						    CString cselement;
					        for (ix[1] = 0; ix[1] <= UB[1]; ix[1]++)
					        {
								CString cs;
								cs.Format("%d\t",ix[1]);
								OutputDebugText(cs);

						        hri = ::SafeArrayGetElement(v.parray, (LONG*)&ix, &ind);
						        if (hri == S_OK)
						        {
							        //element.AppendFormat("%10.6lf ",ind);
									VGetImage(ind);
						        }
						        else
						        {
							        cselement = "* ";
						        }
                            }
						    if (ix[0] < UB[0])
							{
							    //arrvar.AppendFormat("[%2.1ld] %s,", ix[0], element);
								//cs.Format("[%2.1ld] %s,", ix[0], cselement);
								//arrvar += cs;
							}
						    else
							{
							    //arrvar.AppendFormat("[%2.1ld] %s", ix[0], element);
								//cs.Format("[%2.1ld] %s", ix[0], cselement);
								//arrvar += cs;
							}
						    //arrvar += L"<br>";
					    }		
					    //var = arrvar;
                    }
				}
			break;
			default:
				var = "???";
				break;
		}
	//result = var;
	CString cs("DONE\n");
	OutputDebugText(cs);
	cs.Empty();
	OutputDebugText(cs);
#endif //_DEBUG
}

