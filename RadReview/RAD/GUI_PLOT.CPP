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
//    File Name : Gui_plot.cpp     
//Primary Author: Tom Marks

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////
#include <math.h>	// for exp() and log()
#include <gui_lib.h>
#include <pl.h>
#include "gui_plot.h"


extern "C" double ceil(double);		// From math library.

COLORREF CGUI_PlotPane::mGraphBkgColor = GUI_WHITE;
COLORREF CGUI_PlotPane::mGraphFG_Color = GUI_BLACK;
COLORREF CGUI_PlotPane::mCursorColor = GUI_RED;	
COLORREF CGUI_PlotPane::mCursorReadoutColor = GUI_BLUE;
COLORREF CGUI_PlotPane::mTraceColor = GUI_BLUE;
COLORREF CGUI_PlotPane::mLabelColor = GUI_BLUE;

char *CGUI_PlotPane::mszTypeFace = "Times New Roman";
float CGUI_PlotPane::mfTextPtSize = 12.0F;
int CGUI_PlotPane::meDrawType = PL_LINE;

//float CGUI_PlotPane::mfLinePtSize = 0.0F;	// pjm EOSS Debug.
float CGUI_PlotPane::mfLinePtSize = 0.0F;	// Minimum; i.e. one pixel.
float CGUI_PlotPane::mfRegionLinePtSize = 1.25F;
	
float CGUI_PlotPane::mfRowSpacingPts = 2.0F;

// Log Y axis
float CGUI_PlotPane::mfAreaY_AnnotDegLog = 90.F;	// 90 => vertical; 0 => horizontal
int CGUI_PlotPane::miNumColHorzY_AnnotLog = 0;	// 0 for vertical OR number of y-annot chars
char *CGUI_PlotPane::mszY_AnnotDefaultFmtLog = PL_FMT_TENTO;
// Linear Y axis
float CGUI_PlotPane::mfAreaY_AnnotDegLinear = 90.F;	// 90 => vertical; 0 => horizontal
int CGUI_PlotPane::miNumColHorzY_AnnotLinear = 0;	// 0 for vertical OR number of y-annot chars
char *CGUI_PlotPane::mszY_AnnotDefaultFmtLinear = PL_FMT_AUTO;

const UINT uiMAX_ZOOM_FACTOR = 10000;
const double dDEFAULT_FIRST_VALUE = 0.0;
const double dDEFAULT_VALUE_INCR = 1.0;

#undef RAISED_PANEL

const float fIMPLICIT_LINE_PT_SIZE = -1.0;

//class CGUI_PlotPane;
	
// =====================================================================
// ======================= CPlotData Declaration =======================
// =====================================================================
	
// Class for details concerning the data for an individual plot "area".
// All functions return data values as doubles while storing and handling
// data in "native" format.
//
// Some design choices were made here. All dependence on data type is completely
// hidden in this class (except for overloaded versions of the functions
// CGUI_PlotPane::AddArea and CGUI_PlotPane::UpdateAreaData). Rather than have derived
// classes which take care of data type issues I chose (for better or worse) to
// have cascaded if / if else for the different types. Those cascaded if / if else
// are limited to just a few routines as listed below.
//
// If a new data type is desired (and available in the plot library) the following
// routimes need to be changed:
//		CPlotData::Plot()
//		CPlotData::GetValueGivenIndex()
//		CPlotData::Free()
//		CPlotData::CreatePositiveOnlyY_Data()
// and the following routines need to have an overloaded version added:
//		BOOL PositiveOnlyDataAvail();
//		CPlotData::GetDataPtr()
//		CPlotData::Set()

class CPlotData
{
	public:
		CPlotData();
		~CPlotData();

		void FindNearestPt(double dX, double *pdActual, ULONG *pulIndex = NULL);
		
		ULONG GetDataDim() {return(mulDataDim);};
		BOOL GetIndexGivenValue(double dValue, ULONG *pulIndex);
		BOOL GetValueGivenIndex(ULONG ulIndex, double *pdValue, BOOL bPositiveOnlyData = TRUE);
		void GetValueRange(double *pMin, double *pMax, BOOL bPositiveOnlyData = FALSE);
		BOOL GetY_GivenX(double dX, CPlotData &mPlotDataX, double *pdY, BOOL bPositiveOnlyData);
		BOOL GetImplicitDataParms(double *pdLeftMostValue = NULL, double *pdX_Incr = NULL) const;
		BOOL GetDataPtr(double huge **padData, ULONG *pulDataDim, BOOL bPositiveOnlyData = FALSE);
		BOOL GetDataPtr(float huge **pafData, ULONG *pulDataDim, BOOL bPositiveOnlyData = FALSE);
		BOOL GetDataPtr(ULONG huge **paulData, ULONG *pulDataDim) const;

		BOOL Set(ULONG ulDataDim, double huge *adData, BOOL bCopyData);
		BOOL Set(ULONG ulDataDim, float huge *afData, BOOL bCopyData);
		BOOL Set(ULONG ulDataDim, ULONG huge *aulData, BOOL bCopyData);
		BOOL Set(ULONG ulDataDim, double FirstValue = 0.0, double ValueIncr = 1.0);
		
		int Plot(PL_CTX *pCtx, CPlotData &mPlotDataX, COLORREF TraceColor, ULONG ulIndexFirst,
			ULONG ulIndexLast, float fExplicitLinePtSize = fIMPLICIT_LINE_PT_SIZE);
		
	private:
		void Free();
		void CreatePositiveOnlyY_Data(double *pdDummyforUniqueSignature);
		void CreatePositiveOnlyY_Data(float *pfDummyforUniqueSignature);
		BOOL PositiveOnlyDataAvail(double huge **padData);
		BOOL PositiveOnlyDataAvail(float huge **pafData);
		
		BOOL mbNonPositiveOrigData;
		BOOL mbPositiveOnlyDataCreated;
		double mdMinPositiveValue;
		BOOL mbLocalDataCopy;
		ULONG mulDataDim;
		double huge *madData;
		float huge *mafData;
		ULONG huge *maulData;
		// As needed, create positive array of numbers for log display.
		double huge *madPositiveData;
		float huge *mafPositiveData;
		double mdFirstValue;
		double mdValueIncr;
		double mdMin;
		double mdMax;
};


CPlotData::CPlotData()
{
	OutputDebugString("CPlotData::CPlotData()\n");

	mulDataDim = 0;
	mbLocalDataCopy = FALSE;
	madData = NULL;
	mafData = NULL;
	maulData = NULL;
	madPositiveData = NULL;
	mafPositiveData = NULL;
	mbNonPositiveOrigData = FALSE;
	mbPositiveOnlyDataCreated = FALSE;
	mdMinPositiveValue = 0.0;
	mdFirstValue = 0.0;
	mdValueIncr = 0.0;
}

CPlotData::~CPlotData()
{
	Free();
}


// Determine what type of data exists and call the appropriate plot lib routines.
// Y data comes in as "this" and X data is passed in as a parameter, CPlotData &mPlotDataX.
int CPlotData::Plot(
	PL_CTX *pCtx, 
	CPlotData &mPlotDataX, 
	COLORREF TraceColor, 
	ULONG ulIndexFirst, 
	ULONG ulIndexLast, 
	float fExplicitLinePtSize)
{
	OutputDebugString("CPlotData::Plot()\n");

	int iStatus = PL_BAD_INPUT;
	double dLeftMostValue;
	double dX_Incr;
	float fLinePtSize = CGUI_PlotPane::GetLinePtSize();

	// If we are sending output to the printer and the printer does not ahve color
	// capability and we are (over-)printing a hilite region use a thicker line.
	
	BOOL bPrinter = pCtx->bPrinter;	// this plot context is open to printer
	BOOL bPrinterColor = pCtx->bPrinterColor;	// 1 if printer has color capability
	BOOL bHiliteRegion = ulIndexFirst != 0;	// this is generally true but a bit kludgy
	BOOL bLogPlot = (pCtx->pYAxis->eScaleType == 1);
	if (bHiliteRegion && bPrinter && !bPrinterColor) fLinePtSize = CGUI_PlotPane::GetRegionLinePtSize();
            
	if (fExplicitLinePtSize != fIMPLICIT_LINE_PT_SIZE) {
		 fLinePtSize = fExplicitLinePtSize;
	}

	if (ulIndexLast >= ulIndexFirst) {
		// Allow ulIndexLast to be greater than (mulDataDim - 1) but adjust appropriately to
		// avoid addressing out-of-bounds data in the plot library.
		if (ulIndexLast > (mulDataDim - 1)) ulIndexLast = mulDataDim - 1;
		ULONG ulDataDim = ulIndexLast - ulIndexFirst + 1;
		
		if (mPlotDataX.GetImplicitDataParms(&dLeftMostValue, &dX_Incr)) {
			// Here if there is no X data (i.e. the Y data index is implicit X data)
			dLeftMostValue += dX_Incr * ulIndexFirst;
			
			if (madData != NULL) {
				iStatus = PlDrawYVector(pCtx, ulDataDim, dLeftMostValue,
					dX_Incr, &madData[ulIndexFirst], CGUI_PlotPane::GetDrawType(), fLinePtSize, TraceColor);
			}
			else if (maulData != NULL) {
				iStatus = PlDrawYVector(pCtx, ulDataDim, dLeftMostValue,
					dX_Incr, &maulData[ulIndexFirst], CGUI_PlotPane::GetDrawType(), fLinePtSize, TraceColor);
			}
		}
		else {
			double huge *adX_Data;
			double huge *adY_Data;
			float huge *afX_Data;
			float huge *afY_Data;
			ULONG huge *aulX_Data;
			ULONG ulX_DataDim, ulY_DataDim;
			
			// Get the pointer to and dimension of the X data.
			// Is the x data double?
			if (mPlotDataX.GetDataPtr(&adX_Data, &ulX_DataDim)) {
				// We are here if the X data is double.
				// Get the pointer to and dimension of the Y data.
				if (GetDataPtr(&adY_Data, &ulY_DataDim, bLogPlot)) {
					// Only plot the data if the X and Y vectors have the same dimension.
					if (ulX_DataDim == ulY_DataDim) {
						if (adY_Data != NULL) {
							iStatus = PlDrawXYVectors(pCtx, ulDataDim, &adX_Data[ulIndexFirst],
								&adY_Data[ulIndexFirst], CGUI_PlotPane::GetDrawType(), fLinePtSize, TraceColor);
						}
						else if (maulData != NULL) {
							iStatus = PL_BAD_INPUT;
							// As of 18-Oct-96 there is no capability to have different types
							// of data on the two axes so set iStatus to PL_BAD_INPUT for now.
							// It the following combination is allowed in the future...
						}
					}
				}
			}
			// Or is the x data float?
			else if (mPlotDataX.GetDataPtr(&afX_Data, &ulX_DataDim)) {
				// We are here if the X data is float.
				// Get the pointer to and dimension of the Y data.
				if (GetDataPtr(&afY_Data, &ulY_DataDim, bLogPlot)) {
					// Only plot the data if the X and Y vectors have the same dimension.
					if (ulX_DataDim == ulY_DataDim) {
						if (afY_Data != NULL) {
							iStatus = PlDrawXYVectors(pCtx, ulDataDim, &afX_Data[ulIndexFirst],
								&afY_Data[ulIndexFirst], CGUI_PlotPane::GetDrawType(), fLinePtSize, TraceColor);
						}
						else if (maulData != NULL) {
							iStatus = PL_BAD_INPUT;
							// As of 18-Oct-96 there is no capability to have different types
							// of data on the two axes so set iStatus to PL_BAD_INPUT for now.
							// It the following combination is allowed in the future...
						}
					}
				}
			}
			// Or is the x data ULONG?
			else if (mPlotDataX.GetDataPtr(&aulX_Data, &ulX_DataDim)) {
				// We are here if the X data is ULONG.
				// Only plot the data if the X and Y vectors have the same dimension.
				if (ulX_DataDim == mulDataDim) {
					if (madData != NULL) {
						iStatus = PL_BAD_INPUT;
						// As of 18-Oct-96 there is no capability to have different types
						// of data on the two axes so set iStatus to PL_BAD_INPUT for now.
						// It the following combination is allowed in the future...
						// iStatus = PlDrawXYVectors(pCtx, mulDataDim, aulX_Data, madData,
						// 	eDrawType, fLinePtSize, TraceColor);
					}
					else if (maulData != NULL) {
						// ULONG X data and ULONG Y data.
						iStatus = PlDrawXYVectors(pCtx, ulDataDim, &aulX_Data[ulIndexFirst],
							&maulData[ulIndexFirst], CGUI_PlotPane::GetDrawType(), fLinePtSize, TraceColor);
					}
				}
			}
		}
	}
	return(iStatus);
}


void CPlotData::CreatePositiveOnlyY_Data(double *pdDummyforUniqueSignature)
{
	// If there are any values which can not be plotted on a log scale make another
	// array of the Y data and substitute the smallest positive value of the actual
	// data for each non-positive value.

	if ((mbNonPositiveOrigData) && (!mbPositiveOnlyDataCreated) && (madData != NULL) && (mulDataDim > 0)) {
		madPositiveData = (double huge*)CGUI_HugeMem::Alloc(mulDataDim, sizeof(double));
			
		if (madPositiveData != NULL) {
			mbPositiveOnlyDataCreated = TRUE;
			for (ULONG ul = 0; ul < mulDataDim; ul++) {
				if (madData[ul] <= 0.0) {
					madPositiveData[ul] = mdMinPositiveValue;
				}
				else {
					madPositiveData[ul] = madData[ul];
				}
			}
		}
	}
}

	
void CPlotData::CreatePositiveOnlyY_Data(float *pfDummyforUniqueSignature)
{
	// If there are any values which can not be plotted on a log scale make another
	// array of the Y data and substitute the smallest positive value of the actual
	// data for each non-positive value.

	if ((mbNonPositiveOrigData) && (!mbPositiveOnlyDataCreated) && (mafData != NULL) && (mulDataDim > 0)) {
		mafPositiveData = (float huge*)CGUI_HugeMem::Alloc(mulDataDim, sizeof(float));
			
		if (mafPositiveData != NULL) {
			mbPositiveOnlyDataCreated = TRUE;
			for (ULONG ul = 0; ul < mulDataDim; ul++) {
				if (mafData[ul] <= 0.0) {
					mafPositiveData[ul] = (float)mdMinPositiveValue;
				}
				else {
					mafPositiveData[ul] = mafData[ul];
				}
			}
		}
	}
}

	
BOOL CPlotData::GetY_GivenX(double dX, CPlotData &mPlotDataX, double *pdY, BOOL bPositiveOnlyData)
{
	BOOL bOnDataPt = FALSE;
	ULONG ulIndex;
	double dNearestActualPt;
	
	BOOL bInRange = mPlotDataX.GetIndexGivenValue(dX, &ulIndex);
	if (bInRange) {
		mPlotDataX.FindNearestPt(dX, &dNearestActualPt, &ulIndex);
		if (dNearestActualPt == dX) {
			bOnDataPt = TRUE;
		}
		else {
			// Interpolate to get Y.
			if (dNearestActualPt > dX) {
				// ulIndex is of point to the right of dX.
				double dRightX = dNearestActualPt;
				double dLeftX;
				mPlotDataX.GetValueGivenIndex(ulIndex - 1, &dLeftX);
				double dDeltaX = dRightX - dLeftX;
				double dLeftY, dRightY;
				GetValueGivenIndex(ulIndex, &dRightY, bPositiveOnlyData);
				GetValueGivenIndex(ulIndex - 1, &dLeftY, bPositiveOnlyData);
				double dDeltaY = dRightY - dLeftY;
				double dXFrac = (dX - dLeftX) / dDeltaX;
				*pdY = dLeftY + dXFrac * dDeltaY;
			}
			else {
				// ulIndex is of point to the left of dX.
				double dRightX;
				double dLeftX = dNearestActualPt;
				mPlotDataX.GetValueGivenIndex(ulIndex + 1, &dRightX);
				double dDeltaX = dRightX - dLeftX;
				double dLeftY, dRightY;
				GetValueGivenIndex(ulIndex + 1, &dRightY, bPositiveOnlyData);
				GetValueGivenIndex(ulIndex, &dLeftY, bPositiveOnlyData);
				double dDeltaY = dRightY - dLeftY;
				double dXFrac = (dX - dLeftX) / dDeltaX;
				*pdY = dLeftY + dXFrac * dDeltaY;
			}
		}
	}
	else {
		// Set Y to appropriate end point of data.
		GetValueGivenIndex(ulIndex, pdY, bPositiveOnlyData);
	}
	return(bOnDataPt);
}


BOOL CPlotData::PositiveOnlyDataAvail(double huge **padData)
{
	BOOL bPositiveOnlyDataAvail = FALSE;

	*padData = NULL;
	
	if (madData != NULL) {
		if (mbNonPositiveOrigData) {
			if (!mbPositiveOnlyDataCreated) {
				CreatePositiveOnlyY_Data((double *)NULL);
			}
			
			if (madPositiveData != NULL) {
				*padData = madPositiveData;
				bPositiveOnlyDataAvail = TRUE;
			}
		
		}
		else {
			// Original data was all positive so just pass back pointer to it.
			*padData = madData;
			bPositiveOnlyDataAvail = TRUE;
		}
	}		
	return(bPositiveOnlyDataAvail);
}


BOOL CPlotData::PositiveOnlyDataAvail(float huge **pafData)
{
	BOOL bPositiveOnlyDataAvail = FALSE;

	*pafData = NULL;
	
	if (mafData != NULL) {
		if (mbNonPositiveOrigData) {
			if (!mbPositiveOnlyDataCreated) {
				CreatePositiveOnlyY_Data((float *)NULL);
			}
			
			if (mafPositiveData != NULL) {
				*pafData = mafPositiveData;
				bPositiveOnlyDataAvail = TRUE;
			}
		
		}
		else {
			// Original data was all positive so just pass back pointer to it.
			*pafData = mafData;
			bPositiveOnlyDataAvail = TRUE;
		}
	}		
	return(bPositiveOnlyDataAvail);
}


BOOL CPlotData::GetDataPtr(double huge **padData, ULONG *pulDataDim, BOOL bPositiveOnlyData)
{
	BOOL bSuccess = FALSE;
	
	*padData = NULL;
	*pulDataDim = mulDataDim;

	if (bPositiveOnlyData) {
		bSuccess = PositiveOnlyDataAvail(padData);
	}
	else {
		if ((madData != NULL) && (mulDataDim > 0)) {
			*padData = madData;
			bSuccess = TRUE;
		}
	}
	
	return(bSuccess);
}


BOOL CPlotData::GetDataPtr(float huge **pafData, ULONG *pulDataDim, BOOL bPositiveOnlyData)
{
	BOOL bSuccess = FALSE;
	
	*pafData = NULL;
	*pulDataDim = mulDataDim;

	if (bPositiveOnlyData) {
		bSuccess = PositiveOnlyDataAvail(pafData);
	}
	else {
		if ((mafData != NULL) && (mulDataDim > 0)) {
			*pafData = mafData;
			bSuccess = TRUE;
		}
	}
	
	return(bSuccess);
}


BOOL CPlotData::GetDataPtr(ULONG huge **paulData, ULONG *pulDataDim) const
{
	BOOL bSuccess = FALSE;
	
	*paulData = maulData;
	*pulDataDim = mulDataDim;

	if ((maulData != NULL) && (mulDataDim > 0)) {
		bSuccess = TRUE;
	}
	
	return(bSuccess);
}


BOOL CPlotData::GetImplicitDataParms(double *pdLeftMostValue, double *pdIncr) const
{
	BOOL bIsImplicit;
	
	if (mdValueIncr != 0.0) bIsImplicit = TRUE;
	else bIsImplicit = FALSE;
	
	if (pdLeftMostValue != NULL) *pdLeftMostValue = mdFirstValue;
	
	if (pdIncr != NULL) *pdIncr = mdValueIncr;
	
	return(bIsImplicit);
}


void CPlotData::FindNearestPt(double dValue, double *pdActual, ULONG *pulIndex)
{
	ULONG ulIndex;
	
	GetIndexGivenValue(dValue, &ulIndex);
	GetValueGivenIndex(ulIndex, pdActual);
	if (pulIndex != NULL) *pulIndex = ulIndex;
}


BOOL CPlotData::GetIndexGivenValue(double dGivenValue, ULONG *pulIndex)
{
	BOOL bInRange = TRUE;
	ULONG ulTrialIndex = 0;
	
	if (mdValueIncr != 0.0) {
		// We have "implicit" data.
		if (dGivenValue < mdFirstValue) {
			bInRange = FALSE;
			ulTrialIndex = 0;
		}
		else {
			ulTrialIndex = (ULONG)((dGivenValue - mdFirstValue + 0.5) / mdValueIncr);
			// bInRange test done just before return.
		}
	}
	else {
		double dMinDataValue, dMaxDataValue, dDataValue;
		GetValueGivenIndex(0, &dMinDataValue);
		if (mulDataDim > 0) {
			GetValueGivenIndex(mulDataDim - 1, &dMaxDataValue);

			if (dGivenValue < dMinDataValue) {
				bInRange = FALSE;
				ulTrialIndex = 0L;
			}
			else if (dGivenValue > dMaxDataValue) {
				bInRange = FALSE;
				ulTrialIndex = mulDataDim - 1;
			}
			else {
				double dX_Range = dMaxDataValue - dMinDataValue;
				if (dX_Range != 0.0) {
					ulTrialIndex = (LONG)(((dGivenValue - dMinDataValue) / dX_Range) * (double)(mulDataDim - 1));
					// Protect against an out-of-range value of ulTrialIndex for 
					// reference to mafX_Data[ulTrialIndex] below.
					if (ulTrialIndex >= mulDataDim) {
						ulTrialIndex = mulDataDim - 1;
					}
					// Search left while dGivenValue is less than X_Data[TrialIndex].
					BOOL bFinished = FALSE;
					do {
						GetValueGivenIndex(ulTrialIndex, &dDataValue);
						if ((ulTrialIndex >= 0) && (dGivenValue < dDataValue)) {
								ulTrialIndex--;
							}
							else {
								bFinished = TRUE;
							}
					} while (!bFinished);

					// Then search right while dGivenValue is greater than X_Data[TrialIndex].
					bFinished = FALSE;
					do {
						GetValueGivenIndex(ulTrialIndex, &dDataValue);
						if ((ulTrialIndex < (mulDataDim - 1)) && (dGivenValue > dDataValue)) {
							ulTrialIndex++;
						}
						else {
							bFinished = TRUE;
						}
					} while (!bFinished);

					// the above procedure always leaves us at the point just to the right of or on
					// dGivenValue. If we are not on the left-most point (index = 0); see if xValueX
					// is closer to the point to it's left and if so move there.
					if (ulTrialIndex > 0) {
						double dLeftValue, dRightValue;
						GetValueGivenIndex(ulTrialIndex - 1, &dLeftValue);
						GetValueGivenIndex(ulTrialIndex, &dRightValue);
						double dDeltaLeft = dGivenValue - dLeftValue;
						double dDeltaRight = dRightValue - dGivenValue;
						if (dDeltaLeft < dDeltaRight) {
							ulTrialIndex--;
						}
					}
				}
			}
		}
	}
	
	if ((ulTrialIndex >= mulDataDim) && (mulDataDim > 0)){
		bInRange = FALSE;
		ulTrialIndex = mulDataDim - 1;
	}
	*pulIndex = ulTrialIndex;
	
	return(bInRange);
}

BOOL CPlotData::GetValueGivenIndex(ULONG ulIndex, double *pdValue, BOOL bPositiveOnlyData)
{
	BOOL bInRange = FALSE;
	double dValue = 0.0;
	
	//	Allow ulIndexHigh to be > (mPlotDataX.GetDataDim() - 1). This facilitates setting a
	//	Hilite region once that will cover new pts as they come in later.

	if (ulIndex >= mulDataDim) {
		ulIndex = mulDataDim - 1;
	}
	else {
		bInRange = TRUE;
	}

	if (mdValueIncr != 0.0) {
		dValue = mdFirstValue + ulIndex * mdValueIncr;
	}
	else {
		if (mulDataDim > 0) {
			double huge *adY_Data;
			float huge *afY_Data;
			ULONG ulY_DataDim;
			
			// double data?
			if (GetDataPtr(&adY_Data, &ulY_DataDim, bPositiveOnlyData)) {
				if (adY_Data != NULL) dValue = adY_Data[ulIndex];
			}
			// or float data?
			else if (GetDataPtr(&afY_Data, &ulY_DataDim, bPositiveOnlyData)) {
				if (afY_Data != NULL) dValue = afY_Data[ulIndex];
			}
			// or ULONG data?
			else if (maulData != NULL) dValue = maulData[ulIndex];
		}
	}

	*pdValue = dValue;
	return(bInRange);
}

void CPlotData::GetValueRange(double *pMin, double *pMax, BOOL bPositiveOnlyData)
{
	*pMax = mdMax;

	if (bPositiveOnlyData) {
		*pMin = mdMinPositiveValue;
		// Make sure max value returned is at least as great as min value returned.
		if (mdMax < mdMinPositiveValue) *pMax = mdMinPositiveValue;
	}
	else *pMin = mdMin;
}

void CPlotData::Free()
{
	if (mbLocalDataCopy) {
		CGUI_HugeMem::Free(madData);
		CGUI_HugeMem::Free(mafData);
		CGUI_HugeMem::Free(maulData);
		CGUI_HugeMem::Free(madPositiveData);
		CGUI_HugeMem::Free(mafPositiveData);
	}
	madData = NULL;
	mafData = NULL;
	maulData = NULL;
	madPositiveData = NULL;
	mafPositiveData = NULL;
	mbNonPositiveOrigData = FALSE;
	mbPositiveOnlyDataCreated = FALSE;
	mulDataDim = 0;
	mdMin = 0.0;
	mdMax = 0.0;
	mbLocalDataCopy = FALSE;
	mdFirstValue = 0.0;
	mdValueIncr = 0.0;	// != 0.0 => implicit data.
}

BOOL CPlotData::Set(ULONG ulDataDim, double huge *adData, BOOL bCopyData)
{
	OutputDebugString("CPlotData::Set(ULONG, double, BOOL)\n");

	BOOL bSuccess = FALSE;

	Free();
	mulDataDim = ulDataDim;

	if (ulDataDim > 0) 
	{
		// Is there explicit data (an array of values) or just implicit (array index).
		if (adData == NULL) {
			bSuccess = Set(ulDataDim, dDEFAULT_FIRST_VALUE, dDEFAULT_VALUE_INCR);
		}
		else 
		{
			if (bCopyData) 
			{
				madData = (double huge*)CGUI_HugeMem::Alloc(ulDataDim, sizeof(double));
				if (madData != NULL) 
				{
					for (ULONG ul = 0; ul < ulDataDim; ul++)
						madData[ul] = adData[ul];

					bSuccess = TRUE;
					mbLocalDataCopy = TRUE;
				}
			}
			else 
			{
				bSuccess = TRUE;
				mbLocalDataCopy = FALSE;
				madData = adData;
			}

			if (bSuccess) 
			{
				mdMin = adData[0];
				mdMax = adData[0];
		
				for (ULONG ul = 0; ul < ulDataDim; ul++) {
					if (adData[ul] < mdMin) mdMin = adData[ul];
					if (adData[ul] > mdMax) mdMax = adData[ul];
	    		}

				// Determine aspects of data relevant for possible eventual need for
				// data to plot on log axis. Save parameters to later create positive-only
				// data array if needed.
				mbNonPositiveOrigData = FALSE;
				mdMinPositiveValue = 0.0;
			
				if (mulDataDim > 0) {
					const double dORIG_MIN_POS_VAL = 0.000001;
					mdMinPositiveValue = dORIG_MIN_POS_VAL;
					for (ULONG ul = 0; ul < mulDataDim; ul++) {
						if (madData[ul] <= 0.0) {
							mbNonPositiveOrigData = TRUE;
						}
						else if ((madData[ul] < mdMinPositiveValue) || (mdMinPositiveValue == dORIG_MIN_POS_VAL)) {
							// look for special case of binary data (infer is binary if equal to 1)
							// that goes between 1 and 0, use dORIG_MIN_POS_VAL there
							if (madData[ul] != 1.0) mdMinPositiveValue = madData[ul];
						}
					}
				}
			}
		}
	}			
	
	return(bSuccess);
}


BOOL CPlotData::Set(ULONG ulDataDim, float huge *afData, BOOL bCopyData)
{
	BOOL bSuccess = FALSE;

	Free();
	mulDataDim = ulDataDim;

	if (ulDataDim > 0) {
		// Is there explicit data (an array of values) or just implicit (array index).
		if (afData == NULL) {
			const double dDEFAULT_FIRST_VALUE = 0.0;
			const double dDEFAULT_VALUE_INCR = 1.0;
			bSuccess = Set(ulDataDim, dDEFAULT_FIRST_VALUE, dDEFAULT_VALUE_INCR);
		}
		else {
			if (bCopyData) {
				mafData = (float huge*)CGUI_HugeMem::Alloc(ulDataDim, sizeof(float));
				if (mafData != NULL) {
					for (ULONG ul = 0; ul < ulDataDim; ul++) {
						mafData[ul] = afData[ul];
					}
					bSuccess = TRUE;
					mbLocalDataCopy = TRUE;
				}
			}
			else {
				bSuccess = TRUE;
				mbLocalDataCopy = FALSE;
				mafData = afData;
			}

			if (bSuccess) {
				mdMin = afData[0];
				mdMax = afData[0];
		
				for (ULONG ul = 0; ul < ulDataDim; ul++) {
					if (afData[ul] < mdMin) mdMin = afData[ul];
					if (afData[ul] > mdMax) mdMax = afData[ul];
	    		}

				// Determine aspects of data relevant for possible eventual need for
				// data to plot on log axis. Save parameters to later create positive-only
				// data array if needed.
				mbNonPositiveOrigData = FALSE;
				mdMinPositiveValue = 0.0;
			
				if (mulDataDim > 0) {
					const double dORIG_MIN_POS_VAL = 0.000001;
					mdMinPositiveValue = dORIG_MIN_POS_VAL;
					for (ULONG ul = 0; ul < mulDataDim; ul++) {
						if (mafData[ul] <= 0.0) {
							mbNonPositiveOrigData = TRUE;
						}
						else if ((mafData[ul] < mdMinPositiveValue) || (mdMinPositiveValue == dORIG_MIN_POS_VAL)) {
							mdMinPositiveValue = mafData[ul];
						}
					}
				}
			}
		}
	}			
	
	return(bSuccess);
}


BOOL CPlotData::Set(ULONG ulDataDim, ULONG huge *aulData, BOOL bCopyData)
{
	BOOL bSuccess = FALSE;

	Free();
	mulDataDim = ulDataDim;

	if (ulDataDim > 0) {
		// Is there explicit data (an array of values) or just implicit (array index).
		if (aulData == NULL) {
			const double dDEFAULT_FIRST_VALUE = 0.0;
			const double dDEFAULT_VALUE_INCR = 1.0;
			bSuccess = Set(ulDataDim, dDEFAULT_FIRST_VALUE, dDEFAULT_VALUE_INCR);
		}
		else {
			if (bCopyData) {
				maulData = (ULONG huge*)CGUI_HugeMem::Alloc(ulDataDim, sizeof(ULONG));
				if (maulData != NULL) {
					for (ULONG ul = 0; ul < ulDataDim; ul++) {
						maulData[ul] = aulData[ul];
					}
					bSuccess = TRUE;
					mbLocalDataCopy = TRUE;
				}
			}
			else {
				bSuccess = TRUE;
				mbLocalDataCopy = FALSE;
				maulData = aulData;
			}

			if (bSuccess) {
				mdMin = aulData[0];
				mdMax = aulData[0];
		
				for (ULONG ul = 0; ul < ulDataDim; ul++) {
					if (aulData[ul] < mdMin) mdMin = aulData[ul];
					if (aulData[ul] > mdMax) mdMax = aulData[ul];
	    		}
			}
		}
	}			
	
	return(bSuccess);
}

// For setting "implicit" data.
BOOL CPlotData::Set(ULONG ulDataDim, double dFirstValue, double dValueIncr)
{
	BOOL bSuccess = TRUE;

	Free();
	mulDataDim = ulDataDim;
	mdFirstValue = dFirstValue;
	mdValueIncr = dValueIncr;
	mdMin = mdFirstValue;

	if (ulDataDim > 0) {
		mdMax = dFirstValue + (double)(ulDataDim - 1) * dValueIncr;
	}
	else {
		mdMax = mdFirstValue;
	}
	
	return(bSuccess);
}


// =====================================================================
// ======================= CPlotArea Declaration =======================
// =====================================================================
	

// Class for details concerning an individual plot "area".
// "area" as used here is "area" defined / used in PL library.	
class CPlotArea
{
	struct HiliteRegion {
		ULONG ulIndexFirst;
		ULONG ulIndexLast;
		COLORREF Color;
		CGStr Label;
	};
	
	struct Marker {
		ULONG ulMarkerIndex;
		CGStr Label;
		COLORREF Color;
	};
	
	public:
		CPlotArea(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex);
		CPlotArea(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex, double huge *adX_Data, double huge *adY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel);
		CPlotArea(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex, float huge *afX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel);
		CPlotArea(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex, ULONG huge *aulX_Data, ULONG huge *aulY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel);
		CPlotArea(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex, ULONG huge *aulX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel);
		~CPlotArea();

		void GetAreaLabel(CGStr *pLabel);
		void GetStyleY(BOOL *pbLogY_Axis, BOOL *pbZoomAutoScale);
		void SetStyleY(BOOL bLogY_Axis, BOOL bZoomAutoScale);
		void RefreshDisplay(PL_CTX *pCtx = NULL, BOOL bEraseFirst = TRUE);
		BOOL UpdateData(double huge *adX_Data, double huge *adY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szLabel = NULL);
		BOOL UpdateData(float huge *afX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szLabel = NULL);
		BOOL UpdateData(ULONG huge *aulX_Data, ULONG huge *aulY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szLabel = NULL);
		BOOL UpdateData(ULONG huge *aulX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szLabel = NULL);
		void MoveCursorToValueX(double dX);
		void MoveCursor(double dStepsX);
		void FindY_GivenX(double dValueX, double *pdValueY);
		BOOL GetCursorXY(double *pdX, double *pdY, ULONG *pulIndex = NULL, BOOL *pbInterpolatedY = NULL, BOOL bPositiveOnlyData = FALSE);
		BOOL GetCurrentCursXByIndex(double *pdCurrentCursX, ULONG *pulCurrentCursIndexX = NULL);
		BOOL GetCurrentCursXByValue(double *pdCurrentCursX, ULONG *pulCurrentCursIndexX = NULL);
		BOOL SetCurrentCursX(ULONG ulNewCursIndexX, double *pdNewCursX);
		BOOL SetIndexGivenX(double dCursX, ULONG *pulIndex);
		BOOL GetDataRange(double *pdMinX, double *pdMaxX, double *pdMinY, double *pdMaxY, BOOL *pbAutoScale = NULL, ULONG *pulDataDim = NULL, BOOL bPositiveOnlyData = FALSE);
		void CenterCursor();
		BOOL GetY_RangeGivenX_Range(double dX_Min, double dX_Max, double *pdY_Min, double *pdY_Max, BOOL bPositiveOnlyData = FALSE);
		BOOL HasValidData() {return(TRUE);};
		void SetLimitsY(BOOL bAutoScale, double dY_Min = 0.0, double dY_Max = 0.0);
		UINT SetAreaIndex(UINT uiNewAreaIndex);
		
		BOOL AddHiliteRegion(double dLowValueX, double dHighValueX, COLORREF Color, const char *szLabel = NULL);
		void ClearHiliteRegions();
		BOOL DeleteHiliteRegion(ULONG ulFirstPtIndex, ULONG ulLastPtIndex);
		BOOL DeleteHiliteRegion(double dLowValueX, double dHighValueX);
		BOOL IsValueInHiliteRegion(double dTargetValueX, double *pdLowValueX, double *pdHighValueX, COLORREF *pHiliteColor = NULL, CGStr *pLabel = NULL);
		BOOL IsValueInHiliteRegion(double dTargetValueX, ULONG *pulFirstPtIndex, ULONG *pulLastPtIndex, COLORREF *pHiliteColor = NULL, CGStr *pLabel = NULL);
		BOOL NextHiliteRegion(double dCurrentX, double *pdNextRegionLowValueX, double *pdNextRegionHighValueX);
		BOOL PrevHiliteRegion(double dCurrentX, double *pdPrevRegionLowValueX, double *pdPrevRegionHighValueX);
		void ShowHiliteRegions(BOOL bVisible);
		
		BOOL EmphasizeRegion(double dMinX, double dMaxX, BOOL bNearestActualPt, COLORREF Color, float fLineThicknessPts, double *pdActualMinX, double *pdActualMaxX);

		BOOL AddMarker(double dMarkerValueX, const char *szLabel, COLORREF MarkerTextColor);
		void ClearMarkers();
		BOOL DeleteMarker(double dMarkerValueX);
		BOOL IsValueOnMarker(double dTargetValueX, COLORREF *pMarkerTextColor);
		void ShowMarkers(BOOL bVisible);
		
		void SetCurrentCursX(double dCurrentCursX);
		void FindNearestPtX(double dX, double *pdActual, ULONG *pulIndex = NULL);
		BOOL IsValidUpdateData(void *aDataX, void *aDataY, ULONG ulNumPts, const char *szAreaLabel);

	private:
		ULONG mulNumHiliteRegions;
		struct HiliteRegion *maHiliteRegions;
		BOOL mbHiliteRegionsVisible;

		ULONG mulNumMarkers;
		struct Marker *maMarkers;
		BOOL mbMarkersVisible;

		BOOL mbAutoScaleY;
		double mdManualMinY;
		double mdManualMaxY;
		BOOL mbLogY;
		BOOL mbX_ZoomAutoScaleY;

		void Initialize(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex);
		void UpdateDataPreProcess(const char *szAreaLabel);
		void UpdateDataPostProcess();
		BOOL RefreshBackground(PL_CTX *pCtx);
		void DisplayHiliteRegions(PL_CTX *pCtx);
		void DisplayMarkers(PL_CTX *pCtx);
		void ShowPlotLibErrMsg(int iStatus);
		BOOL IsVisibleDataInRegion(PL_CTX *pCtx, ULONG ulIndexFirst, ULONG ulIndexLast);
		BOOL FindIndexGivenX(double dValueX, ULONG *pulIndexX);
		void SetNoDataDefaults();
		UINT muiAreaIndex;
		PL_CTX *mpPlCtx;
		CGStr mDataName;
		CGUI_PlotPane *mpParentPane;

		CPlotData mPlotDataX;
		CPlotData mPlotDataY;

		double mdMinX;
		double mdMaxX;
		double mdMinY;
		double mdMaxY;
};
	

// =====================================================================
// ===================== CPlotArea Implementation ======================
// =====================================================================

CPlotArea::CPlotArea(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex)
{
	OutputDebugString("CPlotArea::CTOR\n");

	ULONG ulDataDim = 0L;
	BOOL bCopyData = FALSE;
	char *szAreaLabel = "No Data";
	
	Initialize(pParentPane, pPlCtx, uiAreaIndex);
	UpdateData((ULONG *)NULL, (ULONG *)NULL, ulDataDim, bCopyData, szAreaLabel);
}			


CPlotArea::CPlotArea(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex, float huge *afX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{
	OutputDebugString("CPlotArea::CTOR\n");

	Initialize(pParentPane, pPlCtx, uiAreaIndex);
	UpdateData(afX_Data, afY_Data, ulDataDim, bCopyData, szAreaLabel);
}			


CPlotArea::CPlotArea(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex, ULONG huge *aulX_Data, ULONG huge *aulY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{
	OutputDebugString("CPlotArea::CTOR\n");

	Initialize(pParentPane, pPlCtx, uiAreaIndex);
	UpdateData(aulX_Data, aulY_Data, ulDataDim, bCopyData, szAreaLabel);
}			


CPlotArea::CPlotArea(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex, ULONG huge *aulX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{
	OutputDebugString("CPlotArea::CTOR\n");
	Initialize(pParentPane, pPlCtx, uiAreaIndex);
	UpdateData(aulX_Data, afY_Data, ulDataDim, bCopyData, szAreaLabel);
}			

CPlotArea::CPlotArea(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex, double huge *adX_Data, double huge *adY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{
	OutputDebugString("CPlotArea::CTOR\n");
	Initialize(pParentPane, pPlCtx, uiAreaIndex);
	UpdateData(adX_Data, adY_Data, ulDataDim, bCopyData, szAreaLabel);
}			

CPlotArea::~CPlotArea()
{
	ClearHiliteRegions();
	ClearMarkers();
}


void CPlotArea::Initialize(CGUI_PlotPane *pParentPane, PL_CTX *pPlCtx, UINT uiAreaIndex)
{
	muiAreaIndex = uiAreaIndex;
	mpPlCtx = pPlCtx;
	mpParentPane = pParentPane;
	mbAutoScaleY = TRUE;
	mbLogY = TRUE;
	mbX_ZoomAutoScaleY = TRUE;
 	mulNumHiliteRegions = 0;
	maHiliteRegions = NULL;
	mbHiliteRegionsVisible = TRUE;
	mbMarkersVisible = TRUE;
	maMarkers = NULL;
	SetNoDataDefaults();
}

	
BOOL CPlotArea::EmphasizeRegion(double dMinX, double dMaxX, BOOL bNearestActualPt, COLORREF Color,
	float fLineThicknessPts, double *pdActualMinX, double *pdActualMaxX)
{
	BOOL bSuccess = FALSE;
	ULONG ulIndexFirst, ulIndexLast;
	
	// For now only do nearest actual pts.
	if (bNearestActualPt) {
		// The API consumer defines the higlight region in x-values. Convert these
		// to index values.
		FindNearestPtX(dMinX, pdActualMinX, &ulIndexFirst);
		FindNearestPtX(dMaxX, pdActualMaxX, &ulIndexLast);

		if (ulIndexFirst <= ulIndexLast) {
			if (mpPlCtx != NULL) {
				BOOL bAnyDataInVisibleRegion = IsVisibleDataInRegion(mpPlCtx, ulIndexFirst, ulIndexLast);
				if (bAnyDataInVisibleRegion) {
					// mPlotDataY.Plot protects against ulIndexLast being out of bounds.
					bSuccess = mPlotDataY.Plot(mpPlCtx, mPlotDataX, Color, ulIndexFirst, ulIndexLast, fLineThicknessPts);

				}
			}
		}
	}
	return(bSuccess);
}


BOOL CPlotArea::AddHiliteRegion(double dLowValueX, double dHighValueX, COLORREF Color, const char *szLabel)
{
	BOOL bSuccess = FALSE;
	double dActualDummy;
	ULONG ulIndexLow, ulIndexHigh;
	
	// The API consumer defines the hilight region in x-values. Convert these
	// to index values for storage and later use.
	FindNearestPtX(dLowValueX, &dActualDummy, &ulIndexLow);
	FindNearestPtX(dHighValueX, &dActualDummy, &ulIndexHigh);

//	if ((ulIndexLow <= ulIndexHigh) && (ulIndexHigh < mPlotDataX.GetDataDim())) {
//	Allow ulIndexHigh to be > (mPlotDataX.GetDataDim() - 1). This facilitates setting a
//	Hilite region once that will cover new pts as they come in later.

	if (ulIndexLow <= ulIndexHigh) {
		struct HiliteRegion *aHiliteRegions = new struct HiliteRegion[mulNumHiliteRegions + 1];
		if (aHiliteRegions != NULL) {
			bSuccess = TRUE;
			
			for (ULONG ul = 0; ul < mulNumHiliteRegions; ul++) {
				aHiliteRegions[ul] = maHiliteRegions[ul];
			}
			aHiliteRegions[mulNumHiliteRegions].ulIndexFirst = ulIndexLow;
			aHiliteRegions[mulNumHiliteRegions].ulIndexLast = ulIndexHigh;
			aHiliteRegions[mulNumHiliteRegions].Color = Color;

			if (szLabel == NULL) aHiliteRegions[mulNumHiliteRegions].Label = "";
			else aHiliteRegions[mulNumHiliteRegions].Label = szLabel;
			
			delete [] maHiliteRegions;
			maHiliteRegions = aHiliteRegions;
			mulNumHiliteRegions++;
		}
	}
	return(bSuccess);
}


BOOL CPlotArea::DeleteHiliteRegion(ULONG ulIndexLow, ULONG ulIndexHigh)
{
	BOOL bSuccess = FALSE;
	ULONG ulMatchingIndex;
	
	if (maHiliteRegions != NULL) {
		ULONG ul = 0;

		while ((ul < mulNumHiliteRegions) && (!bSuccess)) {
			if (ulIndexLow == maHiliteRegions[ul].ulIndexFirst) {
				if (ulIndexHigh == maHiliteRegions[ul].ulIndexLast) {
					ulMatchingIndex = ul;
					bSuccess = TRUE;
				}
			}
			ul++;
		}
	
		if (bSuccess) {
			// Don't bother to reallocate a smaller maHiliteRegions. Just waste
			// one of the slots. Note that as soon as a region is added the array
			// is reallocated with the exact needed size. However, if we are
			// deleting the last region go ahead and delete storage.
			mulNumHiliteRegions--;
			
			if (mulNumHiliteRegions == 0) {
				delete [] maHiliteRegions;
				maHiliteRegions = NULL;
			}
			else {
				for (ul = ulMatchingIndex; ul < mulNumHiliteRegions; ul++) {
					maHiliteRegions[ul] = maHiliteRegions[ul + 1];
				}
			}
		}
	}
	return(bSuccess);
}


BOOL CPlotArea::DeleteHiliteRegion(double dLowValueX, double dHighValueX)
{
	BOOL bSuccess = FALSE;
	
	if (maHiliteRegions != NULL) {
		double dActualDummy;
		ULONG ulIndexLow, ulIndexHigh;
		
		FindNearestPtX(dLowValueX, &dActualDummy, &ulIndexLow);
		FindNearestPtX(dHighValueX, &dActualDummy, &ulIndexHigh);

		bSuccess = DeleteHiliteRegion(ulIndexLow, ulIndexHigh);
	}
	return(bSuccess);
}


void CPlotArea::DisplayHiliteRegions(PL_CTX *pCtx)
{
	ULONG ulIndexFirst;
	ULONG ulIndexLast;
	COLORREF Color;
	int iStatus;

	if (mbHiliteRegionsVisible) {
		if (!(mpParentPane->UpdateIsSuppressed())) {
			if (maHiliteRegions != NULL) {
				for (ULONG ul = 0; ul < mulNumHiliteRegions; ul++) {
					ulIndexFirst = maHiliteRegions[ul].ulIndexFirst;
					ulIndexLast = maHiliteRegions[ul].ulIndexLast;
					Color = maHiliteRegions[ul].Color;
	
					BOOL bAnyDataInVisibleRegion = IsVisibleDataInRegion(pCtx, ulIndexFirst, ulIndexLast);
					if (bAnyDataInVisibleRegion) {
						// mPlotDataY.Plot protects against ulIndexLast being out of bounds.
						iStatus = mPlotDataY.Plot(pCtx, mPlotDataX, Color, ulIndexFirst, ulIndexLast);
					}
				}
			}
		}
	}
}


void CPlotArea::ClearHiliteRegions()
{
	delete [] maHiliteRegions;
	maHiliteRegions = NULL;
	mulNumHiliteRegions = 0;
}


BOOL CPlotArea::IsValueInHiliteRegion(double dTargetValueX, double *pdLowValueX,
	double *pdHighValueX, COLORREF *pHiliteColor, CGStr *pLabel)
{
	BOOL bInHiliteRegion = FALSE;
	const BOOL bPOSITIVE_ONLY_DATA_FALSE = FALSE;
	ULONG ulTargetPtIndex;
	
	if (maHiliteRegions != NULL) {
		ULONG ul = 0;
		
		mPlotDataX.GetIndexGivenValue(dTargetValueX, &ulTargetPtIndex);
		
		while ((ul < mulNumHiliteRegions) && (!bInHiliteRegion)) {
			if (ulTargetPtIndex >= maHiliteRegions[ul].ulIndexFirst) {
				if (ulTargetPtIndex <= maHiliteRegions[ul].ulIndexLast) {
					bInHiliteRegion = TRUE;
					if (pdLowValueX != NULL) mPlotDataX.GetValueGivenIndex(maHiliteRegions[ul].ulIndexFirst, pdLowValueX, bPOSITIVE_ONLY_DATA_FALSE);
					if (pdHighValueX != NULL) mPlotDataX.GetValueGivenIndex(maHiliteRegions[ul].ulIndexLast, pdHighValueX, bPOSITIVE_ONLY_DATA_FALSE);
					if (pHiliteColor != NULL) *pHiliteColor = maHiliteRegions[ul].Color;
					if (pLabel != NULL) *pLabel = maHiliteRegions[ul].Label;
				}
			}
			ul++;
		}
	}
	return(bInHiliteRegion);
}


BOOL CPlotArea::IsValueInHiliteRegion(double dTargetValueX, ULONG *pulFirstPtIndex,
	ULONG *pulLastPtIndex, COLORREF *pHiliteColor, CGStr *pLabel)
{
	BOOL bInHiliteRegion = FALSE;
	const BOOL bPOSITIVE_ONLY_DATA_FALSE = FALSE;
	ULONG ulTargetPtIndex;
	
	if (maHiliteRegions != NULL) {
		ULONG ul = 0;
		
		mPlotDataX.GetIndexGivenValue(dTargetValueX, &ulTargetPtIndex);
		
		while ((ul < mulNumHiliteRegions) && (!bInHiliteRegion)) {
			if (ulTargetPtIndex >= maHiliteRegions[ul].ulIndexFirst) {
				if (ulTargetPtIndex <= maHiliteRegions[ul].ulIndexLast) {
					bInHiliteRegion = TRUE;
					if (pulFirstPtIndex != NULL) *pulFirstPtIndex = maHiliteRegions[ul].ulIndexFirst;
					if (pulLastPtIndex != NULL) *pulLastPtIndex = maHiliteRegions[ul].ulIndexLast;
					// Allow ulIndexLast to be > (mPlotDataX.GetDataDim() - 1). This facilitates setting a
					// Hilite region once that will cover new pts as they come in later. However, in
					// this case pass back the greatest possible index given the dimension of the data.
					if (*pulLastPtIndex > (mPlotDataX.GetDataDim() - 1)) {
						*pulLastPtIndex = mPlotDataX.GetDataDim() - 1;
					}
					if (pHiliteColor != NULL) *pHiliteColor = maHiliteRegions[ul].Color;
					if (pLabel != NULL) *pLabel = maHiliteRegions[ul].Label;
				}
			}
			ul++;
		}
	}
	return(bInHiliteRegion);
}


BOOL CPlotArea::NextHiliteRegion(double dCurrentX, double *pdNextRegionLowValueX, double *pdNextRegionHighValueX)
{
	BOOL bNextHiliteRegionExists = FALSE;
	const BOOL bPOSITIVE_ONLY_DATA_FALSE = FALSE;
	ULONG ulCurrentPtIndex;
	ULONG ulCurrentDistance, ulMinDistance;
		
	if (maHiliteRegions != NULL) {
		ULONG ul = 0;
		
		mPlotDataX.GetIndexGivenValue(dCurrentX, &ulCurrentPtIndex);
		
		// This algoritm does not assume hilite regions are in order.
		while (ul < mulNumHiliteRegions) {
			if (maHiliteRegions[ul].ulIndexFirst > ulCurrentPtIndex) {
				ulCurrentDistance = maHiliteRegions[ul].ulIndexFirst - ulCurrentPtIndex;
				if ((!bNextHiliteRegionExists) || (ulCurrentDistance < ulMinDistance)) {
					ulMinDistance = ulCurrentDistance;
					bNextHiliteRegionExists = TRUE;
					if (pdNextRegionLowValueX != NULL)
						mPlotDataX.GetValueGivenIndex(maHiliteRegions[ul].ulIndexFirst, pdNextRegionLowValueX, bPOSITIVE_ONLY_DATA_FALSE);
					if (pdNextRegionHighValueX != NULL)
						mPlotDataX.GetValueGivenIndex(maHiliteRegions[ul].ulIndexLast, pdNextRegionHighValueX, bPOSITIVE_ONLY_DATA_FALSE);
				}
			}
			ul++;
		}
	}
	return(bNextHiliteRegionExists);
}


BOOL CPlotArea::PrevHiliteRegion(double dCurrentX, double *pdPrevRegionLowValueX, double *pdPrevRegionHighValueX)
{
	BOOL bPrevHiliteRegionExists = FALSE;
	const BOOL bPOSITIVE_ONLY_DATA_FALSE = FALSE;
	ULONG ulCurrentPtIndex;
	ULONG ulCurrentDistance, ulMinDistance;
		
	if (maHiliteRegions != NULL) {
		ULONG ul = 0;
		
		mPlotDataX.GetIndexGivenValue(dCurrentX, &ulCurrentPtIndex);
		
		// This algorithm does not assume hilite regions are in order.
		while (ul < mulNumHiliteRegions) {
			if (maHiliteRegions[ul].ulIndexLast < ulCurrentPtIndex) {
				ulCurrentDistance = ulCurrentPtIndex - maHiliteRegions[ul].ulIndexLast;
				if ((!bPrevHiliteRegionExists) || (ulCurrentDistance < ulMinDistance)) {
					ulMinDistance = ulCurrentDistance;
					bPrevHiliteRegionExists = TRUE;
					if (pdPrevRegionLowValueX != NULL)
						mPlotDataX.GetValueGivenIndex(maHiliteRegions[ul].ulIndexFirst, pdPrevRegionLowValueX, bPOSITIVE_ONLY_DATA_FALSE);
					if (pdPrevRegionHighValueX != NULL)
						mPlotDataX.GetValueGivenIndex(maHiliteRegions[ul].ulIndexLast, pdPrevRegionHighValueX, bPOSITIVE_ONLY_DATA_FALSE);
				}
			}
			ul++;
		}
	}
	return(bPrevHiliteRegionExists);
}




void CPlotArea::ShowHiliteRegions(BOOL bVisible)
{
	if (mbHiliteRegionsVisible != bVisible) {
		mbHiliteRegionsVisible = bVisible;
		RefreshDisplay();
	}
}


BOOL CPlotArea::AddMarker(double dMarkerValueX, const char *szLabel, COLORREF TextColor)
{
	BOOL bSuccess = FALSE;
	double dActualDummy;
	ULONG ulMarkerIndex;
	
	FindNearestPtX(dMarkerValueX, &dActualDummy, &ulMarkerIndex);

	if (ulMarkerIndex < mPlotDataX.GetDataDim()) {
		struct Marker *aMarkers = new struct Marker[mulNumMarkers + 1];
		if (aMarkers != NULL) {
			bSuccess = TRUE;
			
			// If there are any previous markers copy them over to new array.
			if (maMarkers != NULL) {
				for (ULONG ul = 0; ul < mulNumMarkers; ul++) {
					aMarkers[ul] = maMarkers[ul];
				}
				delete [] maMarkers;
				maMarkers = NULL;
			}

			// Add new marker to last slot in array.
			aMarkers[mulNumMarkers].ulMarkerIndex = ulMarkerIndex;
			aMarkers[mulNumMarkers].Label = szLabel;
			aMarkers[mulNumMarkers].Color = TextColor;
			mulNumMarkers++;
			
			// Point member variable to newly allocated and filled array.
			maMarkers = aMarkers;
		}
	}
	return(bSuccess);
}


BOOL CPlotArea::DeleteMarker(double dMarkerValueX)
{
	BOOL bSuccess = FALSE;
	ULONG ulMatchingIndex;
	
	if (maMarkers != NULL) {
		ULONG ul = 0;
		double dActualDummy;
		ULONG ulMarkerIndex;
		
		FindNearestPtX(dMarkerValueX, &dActualDummy, &ulMarkerIndex);

		while ((ul < mulNumMarkers) && (!bSuccess)) {
			if (ulMarkerIndex == maMarkers[ul].ulMarkerIndex) {
				ulMatchingIndex = ul;
				bSuccess = TRUE;
			}
			ul++;
		}
	
		if (bSuccess) {
			// Don't bother to reallocate a smaller maMarkers. Jsut waste
			// one of the slots. Note that as soon as a marker is added the array
			// is reallocated with the exact needed size. However, if we are
			// deleting the last marker go ahead and delete storage.
			mulNumMarkers--;
			
			if (mulNumMarkers == 0) {
				delete [] maMarkers;
				maMarkers = NULL;
			}
			else {
				for (ul = ulMatchingIndex; ul < mulNumMarkers; ul++) {
					maMarkers[ul] = maMarkers[ul + 1];
				}
			}
		}
	}
	return(bSuccess);
}


void CPlotArea::DisplayMarkers(PL_CTX *pCtx)
{
	const BOOL bBOLD = FALSE;
	const BOOL bTRANSPARENT_BKG = TRUE;
	double dMarkerValueX;

	if (mbMarkersVisible) {
		if (!(mpParentPane->UpdateIsSuppressed())) {
			if (maMarkers != NULL) {
				BOOL bPositiveOnlyData = TRUE;
				// Center text vertically in trace area.
				double dMidptY;
				int iStatus;
				const BOOL bVISIBLE = TRUE;
				double dXLeft, dXRight, dYBot, dYTop;
								
				iStatus = PlAreaRangeGet(pCtx, bVISIBLE, &dXLeft, &dXRight, &dYBot, &dYTop);
				if (iStatus == PL_OK) {
					BOOL bLogDisplayY = pCtx->pYAxis->eScaleType == 1;
			
					if (bLogDisplayY) {
						// Protect against taking log of number less than or equal to 0.0
						if (dYBot <= 0.0) dYBot = 0.1;
						if (dYTop <= 0.0) dYTop = 0.1;
				
						dMidptY = exp((log(dYBot) + log(dYTop))/2);
					}
					else {
						// Linear display.
						dMidptY = (dYBot + dYTop) / 2.0;
					}
				}
				
				for (ULONG ul = 0; ul < mulNumMarkers; ul++) {
					mPlotDataX.GetValueGivenIndex(maMarkers[ul].ulMarkerIndex, &dMarkerValueX, bPositiveOnlyData);
					PlDrawText(pCtx, dMarkerValueX, dMidptY, maMarkers[ul].Label, PRT_C, 0,
						CGUI_PlotPane::GetTextPtSize(), CGUI_PlotPane::GetTypeFace(), bBOLD, bTRANSPARENT_BKG, 90.0, maMarkers[ul].Color);
				}
			}
		}
	}
}


void CPlotArea::ClearMarkers()
{
	delete [] maMarkers;
	maMarkers = NULL;
	mulNumMarkers = 0;
}


BOOL CPlotArea::IsValueOnMarker(double dTargetValueX, COLORREF *pMarkerTextColor)
{
	BOOL bOnMarker = FALSE;
	ULONG ulMarkerIndex;
	
	if (maMarkers != NULL) {
		ULONG ul = 0;
		mPlotDataX.GetIndexGivenValue(dTargetValueX, &ulMarkerIndex);
		
		while ((ul < mulNumMarkers) && (!bOnMarker)) {
			if (ulMarkerIndex == maMarkers[ul].ulMarkerIndex) {
				bOnMarker = TRUE;
				if (pMarkerTextColor != NULL) *pMarkerTextColor = maMarkers[ul].Color;
			}
			ul++;
		}
	}
	return(bOnMarker);
}


void CPlotArea::ShowMarkers(BOOL bVisible)
{
	if (mbMarkersVisible != bVisible) {
		mbMarkersVisible = bVisible;
		RefreshDisplay();
	}
}



UINT CPlotArea::SetAreaIndex(UINT uiNewAreaIndex)
{
	UINT uiPrevAreaIndex = muiAreaIndex;
	muiAreaIndex = uiNewAreaIndex;
	return(uiPrevAreaIndex);
}


void CPlotArea::GetStyleY(BOOL *pbLogY_Axis, BOOL *pbZoomAutoScale)
{
	*pbLogY_Axis = mbLogY;
	*pbZoomAutoScale = mbX_ZoomAutoScaleY;
}


void CPlotArea::SetStyleY(BOOL bLogY_Axis, BOOL bZoomAutoScale)
{
	mbLogY = bLogY_Axis;
	mbX_ZoomAutoScaleY = bZoomAutoScale;
}


void CPlotArea::SetNoDataDefaults()
{
	mbAutoScaleY = TRUE;
	mdManualMinY = 0.0;
	mdManualMaxY = 0.0;
	mdMinX = 0.0;
	mdMaxX = 0.0;
	mdMinY = 0.0;
	mdMaxY = 0.0;
}


BOOL CPlotArea::IsValidUpdateData(void *aDataX, void *aDataY, ULONG ulNumPts, const char *szLabel)
{
	BOOL bValidData = FALSE;
	const BOOL bCOPY_DATA = FALSE;
	
	// Implicit y data is not allowed. In that case or if number of pts is zero declare
	// data invalid and set X & Y data to NULL.
	 
	if ((aDataY == NULL) || (ulNumPts == 0)) {
		UpdateData((float *)NULL, (float *)NULL, 0, bCOPY_DATA, szLabel);
        SetNoDataDefaults();
	}
	else {
		bValidData = TRUE;
	}
	
	return(bValidData);
}


void CPlotArea::UpdateDataPreProcess(const char *szAreaLabel)
{
	if (szAreaLabel == NULL) mDataName = "";
	else mDataName = szAreaLabel;
}


void CPlotArea::UpdateDataPostProcess()
{
	mPlotDataX.GetValueRange(&mdMinX, &mdMaxX);
	mPlotDataY.GetValueRange(&mdMinY, &mdMaxY);
}


BOOL CPlotArea::UpdateData(double huge *adX_Data, double huge *adY_Data,
	ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{
	OutputDebugString("CPlotArea::UpdateData()\n");

	BOOL bSuccess = FALSE;
	BOOL bX_DataOK, bY_DataOK;

	UpdateDataPreProcess(szAreaLabel);
	
//IsValidUpdateData(afX_Data, afY_Data, ulDataDim, szAreaLabel);

	bX_DataOK = mPlotDataX.Set(ulDataDim, adX_Data, bCopyData);
	bY_DataOK = mPlotDataY.Set(ulDataDim, adY_Data, bCopyData);

	if (bX_DataOK && bY_DataOK) bSuccess = TRUE;

	UpdateDataPostProcess();
	
	return(bSuccess);
}


BOOL CPlotArea::UpdateData(float huge *afX_Data, float huge *afY_Data,
	ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{
	BOOL bSuccess = FALSE;
	BOOL bX_DataOK, bY_DataOK;

	UpdateDataPreProcess(szAreaLabel);
	
//IsValidUpdateData(afX_Data, afY_Data, ulDataDim, szAreaLabel);

	bX_DataOK = mPlotDataX.Set(ulDataDim, afX_Data, bCopyData);
	bY_DataOK = mPlotDataY.Set(ulDataDim, afY_Data, bCopyData);

	if (bX_DataOK && bY_DataOK) bSuccess = TRUE;

	UpdateDataPostProcess();
	
	return(bSuccess);
}


BOOL CPlotArea::UpdateData(ULONG huge *aulX_Data, ULONG huge *aulY_Data,
	ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{
	BOOL bSuccess = FALSE;
	BOOL bX_DataOK, bY_DataOK;

	UpdateDataPreProcess(szAreaLabel);
	
	bX_DataOK = mPlotDataX.Set(ulDataDim, aulX_Data, bCopyData);
	bY_DataOK = mPlotDataY.Set(ulDataDim, aulY_Data, bCopyData);

	if (bX_DataOK && bY_DataOK) bSuccess = TRUE;

	UpdateDataPostProcess();

	return(bSuccess);
}


BOOL CPlotArea::UpdateData(ULONG huge *aulX_Data, float huge *afY_Data,
	ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{
	BOOL bSuccess = FALSE;
	BOOL bX_DataOK, bY_DataOK;

	UpdateDataPreProcess(szAreaLabel);
	
	bX_DataOK = mPlotDataX.Set(ulDataDim, aulX_Data, bCopyData);
	bY_DataOK = mPlotDataY.Set(ulDataDim, afY_Data, bCopyData);

	if (bX_DataOK && bY_DataOK) bSuccess = TRUE;

	UpdateDataPostProcess();
	
	return(bSuccess);
}


void CPlotArea::SetLimitsY(BOOL bAutoScale, double dMinY, double dMaxY)
{
	mbAutoScaleY = bAutoScale;
	if (!mbAutoScaleY) {
		mdManualMinY = dMinY;
		mdManualMaxY = dMaxY;
	}
}


BOOL CPlotArea::GetDataRange(double *pdMinX, double *pdMaxX, double *pdMinY, double *pdMaxY,
	BOOL *pbAutoScaleY, ULONG *pulDataDim, BOOL bPositiveOnlyData)
{
	*pdMinX = mdMinX;
	*pdMaxX = mdMaxX;

	if (pbAutoScaleY != NULL) {
		*pbAutoScaleY = mbAutoScaleY;
	}
	
	if (mbAutoScaleY) {
		mPlotDataY.GetValueRange(pdMinY, pdMaxY, bPositiveOnlyData);
	}
	else {
		*pdMinY = mdManualMinY;
		*pdMaxY = mdManualMaxY;
	}
		
	if (pulDataDim != NULL) *pulDataDim = mPlotDataX.GetDataDim();

	return(HasValidData());
}


void CPlotArea::GetAreaLabel(CGStr *pLabel)
{
	*pLabel = mDataName;
}


// Gets the floating value of the current cursor X location for overall display (pane)
// vs. individual plot and calculates a legitimate index into the X data array.
// The current cursor X position may not be within the plotted data range. The return
// value indicates if the cursor is within the plotted data range.
// The index returned is the index of the nearest point; i.e. dataX[Index] may not equal
// CurrentCursorX returned.
BOOL CPlotArea::GetCurrentCursXByIndex(double *pdCurrentCursX, ULONG *pulCurrentCursIndex)
{
	double dCurrentCursX = 0.0;
	double dOriginalCursX = 0;
	ULONG  ulOriginalIndexX;
	ULONG ulCurrentCursIndexX = 0;
	//ULONG ulIndexX;
	BOOL bWithinRange = FALSE;
	
	if (mpParentPane != NULL) {
		//xxxxxxxxxxxxxxxxxxx here?
		mpParentPane->GetCursorUsingIndex(&ulOriginalIndexX);
		bWithinRange = mPlotDataX.GetValueGivenIndex(ulOriginalIndexX, &dOriginalCursX);
//		mpParentPane->GetCursorPos(&dCurrentCursX);
		//bWithinRange = mPlotDataX.GetIndexGivenValue(dOriginalCursX, &ulIndexX);
		//mpParentPane->GetCursorUsingIndex(&ulCurrentCursIndexX);		
		//bWithinRange = mPlotDataX.GetValueGivenIndex(dCurrentCursIndexX, &pdCurrentCursX);
	}

	if (pulCurrentCursIndex != NULL) *pulCurrentCursIndex = ulOriginalIndexX;
	if (pdCurrentCursX != NULL) *pdCurrentCursX = dOriginalCursX; 
//	*pdCurrentCursX = dCurrentCursX;
	return(bWithinRange);
}


BOOL CPlotArea::GetCurrentCursXByValue(double *pdCurrentCursX, ULONG *pulCurrentCursIndex)
{
	double dCurrentCursX = 0;
	ULONG ulIndexX;
	BOOL bWithinRange = FALSE;

	if (mpParentPane != NULL) {
		mpParentPane->GetCursorPos(&dCurrentCursX);
		bWithinRange = mPlotDataX.GetIndexGivenValue(dCurrentCursX, &ulIndexX);
		
		if (pulCurrentCursIndex != NULL) {
			*pulCurrentCursIndex = ulIndexX;
		}
	}

	*pdCurrentCursX = dCurrentCursX;
	return(bWithinRange);
}


BOOL CPlotArea::SetCurrentCursX(ULONG ulNewCursIndexX, double *pdNewCursX)
{
	BOOL bSuccess = FALSE;

	ULONG ulMaxIndex = mPlotDataX.GetDataDim() - 1;
	if (ulNewCursIndexX <= ulMaxIndex) {
		bSuccess =  mPlotDataX.GetValueGivenIndex(ulNewCursIndexX, pdNewCursX);
	}
	return(bSuccess);
}

BOOL CPlotArea::SetIndexGivenX(double dCursX, ULONG *pulIndex)
{
	return(FindIndexGivenX(dCursX, pulIndex));
}

BOOL CPlotArea::GetY_RangeGivenX_Range(double dX_Min, double dX_Max, double *pdY_Min, double *pdY_Max, BOOL bPositiveOnlyData)
{
	BOOL bSuccess = FALSE;
	ULONG ulDataDim = mPlotDataY.GetDataDim();
	
	if (ulDataDim > 0) {
		double dMinY, dMaxY, dCurrentY;
		ULONG ulMinIndex, ulMaxIndex;
						
		// Now find min and max of y values in current x range.
		mPlotDataX.GetIndexGivenValue(dX_Min, &ulMinIndex);
		mPlotDataX.GetIndexGivenValue(dX_Max, &ulMaxIndex);

		if (ulMinIndex > ulDataDim)
			ulMinIndex = ulDataDim;
		if (ulMaxIndex > ulDataDim)
			ulMaxIndex = ulDataDim;
			
		if (ulMinIndex <= ulMaxIndex) { //sfk made <= rather than <
			bSuccess = TRUE;

			if (mPlotDataY.GetValueGivenIndex(ulMinIndex, &dMinY, bPositiveOnlyData)) {
				dMaxY = dMinY;
				for (ULONG ul = ulMinIndex; ul <= ulMaxIndex; ul++) {
					if (mPlotDataY.GetValueGivenIndex(ul, &dCurrentY, bPositiveOnlyData)) {
						if (dMinY > dCurrentY) dMinY = dCurrentY;
						if (dMaxY < dCurrentY) dMaxY = dCurrentY;
					}
				}
				// this takes into account all of the data points in the given X-range.
				// However it is possible that the data points just outside the X-range
				// on the two ends can contribute to minimum or maximum Y values to be
				// displayed. If this is the case there are 2 choices: 1) Have the value
				// of the off-scale (in X) data point be the determining value of 2) have
				// the value of the line at the visible boundary be the determining value.
				// Choose case tow so get interpolated values at the visible X endpoints.
				// At the low X end:
				mPlotDataY.GetY_GivenX(dX_Min, mPlotDataX, &dCurrentY, bPositiveOnlyData);
				if (dMinY > dCurrentY) dMinY = dCurrentY;
				if (dMaxY < dCurrentY) dMaxY = dCurrentY;
				// At the high X end:
				mPlotDataY.GetY_GivenX(dX_Max, mPlotDataX, &dCurrentY, bPositiveOnlyData);
				if (dMinY > dCurrentY) dMinY = dCurrentY;
				if (dMaxY < dCurrentY) dMaxY = dCurrentY;
			}
			*pdY_Min = dMinY;
			*pdY_Max = dMaxY;
		}
	}      
	
	if (!bSuccess) {
		*pdY_Min = 0.0;
		*pdY_Max = 0.0;
	}
	
	return(bSuccess);
}


void CPlotArea::CenterCursor()
{
	double dMinFullX, dMaxFullX, dMinVisibleX, dMaxVisibleX;
	double dNearestDataPt;
	
	if (mpParentPane != NULL) {
		mpParentPane->GetLimitsX(&dMinFullX, &dMaxFullX, &dMinVisibleX, &dMaxVisibleX);

		double dVisibleMiddle = (dMinVisibleX + dMaxVisibleX) / 2.0;
		mPlotDataX.FindNearestPt(dVisibleMiddle, &dNearestDataPt);
		SetCurrentCursX(dNearestDataPt);
	}
}


void CPlotArea::SetCurrentCursX(double dCurrentCursX)
{
	if (mpParentPane != NULL) {
		mpParentPane->SetCursorPos(dCurrentCursX);
	}
	return;
}


void CPlotArea::FindNearestPtX(double dX, double *pdActual, ULONG *pulIndex)
{
	mPlotDataX.FindNearestPt(dX, pdActual, pulIndex);
}


BOOL CPlotArea::FindIndexGivenX(double dValueX, ULONG *pulIndexX)
{
	BOOL bInRange = mPlotDataX.GetIndexGivenValue(dValueX, pulIndexX);
	
	return(bInRange);
}


void CPlotArea::ShowPlotLibErrMsg(int iStatus)
{
	if ((iStatus != PL_OK) && (mpParentPane != NULL)) {
		mpParentPane->ShowPlotLibErrMsg(iStatus);
	}
}


void CPlotArea::FindY_GivenX(double dValueX, double *pdValueY)
{
	ULONG ulIndexX;
	
	mPlotDataX.GetIndexGivenValue(dValueX, &ulIndexX);
	mPlotDataY.GetValueGivenIndex(ulIndexX, pdValueY);
}


void CPlotArea::MoveCursorToValueX(double dValueX)
{   
	ULONG ulIndexX;
	double dActualX;

	mPlotDataX.FindNearestPt(dValueX, &dActualX, &ulIndexX);
	SetCurrentCursX(dActualX);
}


void CPlotArea::MoveCursor(double dStepsX)
{
	ULONG ulIndexX, ulIndexOld;
	double dCurrX;
	double dNewCursLoc;
	double dValueAtIndex;
	double dDebugCurrX;
	ULONG ulDebugIndexX;
		
	GetCurrentCursXByIndex(&dCurrX, &ulIndexX);
	ulIndexOld = ulIndexX;
	
	mPlotDataX.GetValueGivenIndex(ulIndexX, &dValueAtIndex);
	GetCurrentCursXByIndex(&dDebugCurrX, &ulDebugIndexX);
		
	if (dStepsX < 0) {
		// Moving cursor left. So if cursor is not exactly on a data point,
		// ulIndex will be the nearest point which may be to the left of the
		// cursor value. If this is the case the first step left will be to
		// that point thus we need to decrease dSteps by one and since dSteps
		// is negative we do this by adding one to it.
		if (dValueAtIndex < dCurrX) dStepsX += 1.0;
		
		if (-dStepsX > ulIndexX) {
			ulIndexX = 0L;
		}
		else {
			ulIndexX += (long)dStepsX;
		}                                          
	}
	else {
		if (dValueAtIndex > dCurrX) dStepsX -= 1.0;
		ulIndexX += (long)dStepsX;
		if (ulIndexX >= mPlotDataX.GetDataDim()) {
			ulIndexX = mPlotDataX.GetDataDim() - 1;
		}
	}

	//zzzzzzzzzzzzzzzzzzzzzz

	mPlotDataX.GetValueGivenIndex(ulIndexX, &dNewCursLoc);
	GetCurrentCursXByIndex(&dDebugCurrX, &ulDebugIndexX);
	
	mpParentPane->SetCursorUsingIndex(ulIndexX);	
	GetCurrentCursXByIndex(&dDebugCurrX, &ulDebugIndexX);
	//if (dValueAtIndex == dNewCursLoc) {
	//	double dDum;
	//	mpParentPane->SetCursorUsingIndex(ulIndexX);
		//SetCurrentCursX(ulIndexX, &dDum);
	//}
	//else {
	//	SetCurrentCursX(dNewCursLoc); 
	//}
	
}


// Returns TRUE if cursor is within the data range. If the cursor (x position) is not on a data point
// but is within range of data interpolate (linearly) to get Y and set the interposated flag.
BOOL CPlotArea::GetCursorXY(double *pdX, double *pdY, ULONG *pulIndex, BOOL *pbInterpolatedY, BOOL bPositiveOnlyData)
{
	double dCursorX, dCursorY;
	ULONG ulIndex;
	BOOL bLocalDummyInterpolatedY;
	ULONG ulLocalDummyIndex;
	double dCursorDataPtX;
	BOOL bInRange = FALSE;

	// Make sure pointers to defaultable args are OK.
	if (pbInterpolatedY == NULL) pbInterpolatedY = &bLocalDummyInterpolatedY;
	if (pulIndex == NULL) pulIndex = &ulLocalDummyIndex; 

	// Set innocuous initial values in case of failure below.
	*pbInterpolatedY = FALSE;
	dCursorX = 0.0;
	dCursorY = 0.0;
	*pulIndex = 0;
	
	bInRange = GetCurrentCursXByValue(&dCursorX, &ulIndex);
	//SetCurrentCursX(dCursorX);
	if (bInRange) {
		// Note that ulIndex is the index of the point nearest to the current
		// cursor X value. This may or may NOT be exact. So now determine if
		// the current cursor X value is an actual data point.
		*pulIndex = ulIndex;
		mPlotDataX.GetValueGivenIndex(ulIndex, &dCursorDataPtX);
		if (dCursorDataPtX == dCursorX) {
			bInRange = TRUE;
			mPlotDataY.GetValueGivenIndex(ulIndex, &dCursorY, bPositiveOnlyData);
		}
		else {
			*pbInterpolatedY = TRUE;
			mPlotDataY.GetY_GivenX(dCursorX, mPlotDataX, &dCursorY, bPositiveOnlyData);
		}
	}
	else {
		const BOOL bVISIBLE = TRUE;
		double dXLeft, dXRight, dYBot, dYTop;
		// Default y to center of visible plot range.
		PlAreaRangeGet(mpPlCtx, bVISIBLE, &dXLeft, &dXRight, &dYBot, &dYTop);
		dCursorY = (dYBot + dYTop) / 2.0;
		// dCursorX = (dXLeft + dXRight) / 2.0;
	}
	
	*pdX = dCursorX;
	*pdY = dCursorY;
	return(bInRange);
}
#ifdef _DEBUG
void mdprintf( char * format, ...)
{
	static char buf[1024];
	va_list args;
	va_start( args, format );
	vsprintf( buf, format, args );
	va_end( args);
	OutputDebugStringA( buf);
	printf("%s", buf);
}
#endif

BOOL CPlotArea::IsVisibleDataInRegion(PL_CTX *pCtx, ULONG ulIndexFirst, ULONG ulIndexLast)
{
	BOOL bAnyDataInVisibleRegion = FALSE;
#ifdef _DEBUG
mdprintf("IVDIR  %u %u\n",ulIndexFirst, ulIndexLast);
#endif
	if (mPlotDataX.GetDataDim() > 0L) {
		BOOL bVisible = TRUE;	// Get only visible (vs. full) range.
		double dXLeft, dXRight, dYBot, dYTop;
		int iStatus;
		ULONG ulMinVisibleIndex, ulMaxVisibleIndex;
	
		// Determine the current visible region.
		iStatus = PlAreaRangeGet(pCtx, bVisible, &dXLeft, &dXRight, &dYBot, &dYTop);
		FindIndexGivenX(dXLeft, &ulMinVisibleIndex);
		FindIndexGivenX(dXRight, &ulMaxVisibleIndex);
#ifdef _DEBUG
mdprintf("A %g (%u) %g (%u) %g %g\n", dXLeft, ulMinVisibleIndex, dXRight, ulMaxVisibleIndex, dYBot, dYTop);
#endif
		// Modify limit indices by region passed in.
		if (ulMinVisibleIndex < ulIndexFirst) ulMinVisibleIndex = ulIndexFirst;
		if (ulMaxVisibleIndex > ulIndexLast) ulMaxVisibleIndex = ulIndexLast;
#ifdef _DEBUG
mdprintf("B %g (%u) %g (%u) %g %g\n", dXLeft, ulMinVisibleIndex, dXRight, ulMaxVisibleIndex, dYBot, dYTop);
#endif


		if (mPlotDataX.GetImplicitDataParms()) {
			if (ulMinVisibleIndex != ulMaxVisibleIndex) {
				bAnyDataInVisibleRegion = TRUE;
			}
			else {
				bAnyDataInVisibleRegion = FALSE;
			}
		}
		else {
			// Each of the indices for the data at the end pts. obtained above
			// may be off by as much as a channel so broaden the search by one
			// channel on each end.
			if (ulMinVisibleIndex > 0) ulMinVisibleIndex--;
			if (ulMaxVisibleIndex < (mPlotDataX.GetDataDim() - 1)) ulMaxVisibleIndex++;
#ifdef _DEBUG
mdprintf("C %g (%u) %g (%u) %g %g\n", dXLeft, ulMinVisibleIndex, dXRight, ulMaxVisibleIndex, dYBot, dYTop);
#endif
			ULONG ul = ulMinVisibleIndex;
			double dDataX;
										
			do {
				mPlotDataX.GetValueGivenIndex(ul, &dDataX);
				if ((dDataX >= dXLeft) && (dDataX <= dXRight)) {
					bAnyDataInVisibleRegion = TRUE;
				}
#ifdef _DEBUG
mdprintf("D %g (%u) X %g  %g\n", dXLeft, ul, dDataX, dXRight);
#endif

				ul++;
			} while ((ul <= ulMaxVisibleIndex) && (!bAnyDataInVisibleRegion)); 
#ifdef _DEBUG
mdprintf("Data visible? %d\n", bAnyDataInVisibleRegion);
#endif

		}
	}
#ifdef _DEBUG
mdprintf("IVDIR Visible? %d\n", bAnyDataInVisibleRegion);
#endif

	return(bAnyDataInVisibleRegion);
}


BOOL CPlotArea::RefreshBackground(PL_CTX *pCtx)
{
	BOOL bSuccess = FALSE;
	int iStatus;
	
	iStatus = PlAreaClear(pCtx);
	if (iStatus == PL_OK) {
		// Now plot the axes, annotations, titles, and data.
		const int iREL_LINE_NUM = 0;         
		float fANCHOR_POINT = 0.5F;
		const BOOL bPOSN_REL_TO_AXIS = FALSE;
				
		iStatus = PlAreaTitleText(pCtx, PL_SIDE_LEFT, iREL_LINE_NUM, fANCHOR_POINT,
			(char *)mDataName.Get_sz(), PRT_CEN, bPOSN_REL_TO_AXIS, CGUI_PlotPane::GetLabelColor());
					
		if (iStatus == PL_OK) {
			iStatus = PlAreaBackground(pCtx);
			if (iStatus == PL_OK) {
				bSuccess = TRUE;
			}
		}
	}

	ShowPlotLibErrMsg(iStatus);
	return(bSuccess);
}


void CPlotArea::RefreshDisplay(PL_CTX *pCtx, BOOL bEraseFirst)
{
	OutputDebugString("CPlotArea::RefreshDisplay()\n");

	int	iStatus;
	BOOL bSuccess;

	if (pCtx == NULL) {
		pCtx = mpPlCtx;
	}
	
	if (pCtx != NULL) 
	{
		GUI_SetCursorShape(GUI_CURSOR_HOURGLASS);	// Windows cursor not plot cursor.
		iStatus = PlAreaArraySelect(pCtx, muiAreaIndex, 0);
		if (iStatus == PL_OK) 
		{
			if (bEraseFirst) 
				bSuccess = RefreshBackground(pCtx);
			else bSuccess = TRUE;

			if (bSuccess) 
			{
				char *pDataAreaMsg = NULL;
			
				if (mpParentPane->UpdateIsSuppressed()) 
				{
					pDataAreaMsg = "Update Is Suppressed";
				}
				else 
				{
					ULONG ulNumDataPts = mPlotDataX.GetDataDim();
					BOOL bAnyDataInVisibleRegion = IsVisibleDataInRegion(pCtx, 0, ulNumDataPts - 1);
	
					if (bAnyDataInVisibleRegion) 
					{
						// If the left mouse button is down assume we are in a resize or
						// move and do not waste time "drawing" trace. However, to make sure
						// when the drag operation is over the window is completely painted
						// by scheduling a repaint.
						// HOW TO TELL IF LEFT MOUSE BUTTON IS DOWN???? GetAsyncKeyState();
						BOOL bLeftMouseBtnDown = FALSE;

						if (bLeftMouseBtnDown) 
						{
							GUI_Beep();
							PlWinRepaint(pCtx);
						}
						else 
						{
							iStatus = mPlotDataY.Plot(pCtx, mPlotDataX, CGUI_PlotPane::GetTraceColor(), 0, ulNumDataPts - 1);
							if (iStatus == PL_OK) 
							{
								DisplayHiliteRegions(pCtx);
								DisplayMarkers(pCtx);
							}
						}
					}
					else {
						pDataAreaMsg = "No Data In Visible Region";
					}
	            }
	                        
				if (pDataAreaMsg != NULL) {
					double dDispX;
					double dDispY;
					int eRef;
					double dXLeft, dXRight, dYBot, dYTop;
					BOOL bVisible = TRUE;
					// Set default resonable y-axis min & max for centering text.
					BOOL bSetX_Axis = FALSE;
					BOOL bSetY_Axis = TRUE;
								
					PlAreaRangeGet(pCtx, bVisible, &dXLeft, &dXRight, &dYBot, &dYTop);
					dDispX = 0.5 * (dXLeft + dXRight);
					// Don't set new y limits to center text vertically. If we end up in a situation
					// where we are here because of scrolling while zoomed then we scroll back to a region
					// which has data the y display range if bad. Tom 28-Jul-98
					if (pCtx->pYAxis->eScaleType == 0) {
						// Linear y axis; center msg. vertically.
						dDispY = (dYBot + dYTop) / 2.0;
					}
					else {
						// Log y axis; center msg. vertically.
						double dLogTop;
						if (dYTop == 0.0) dLogTop = 0.0;
						else dLogTop = log(dYTop);

						double dLogBot;
						if (dYBot == 0.0) dLogBot = 0.0;
						else dLogBot = log(dYBot);
						dDispY = exp((dLogTop + dLogBot) / 2.0);
					}

					// Changing axis limits requires redrawing plot "background".
					RefreshBackground(pCtx);
					eRef = PRT_C;
					iStatus = PlDrawText(pCtx, dDispX, dDispY, pDataAreaMsg, eRef, 0.0F,
						CGUI_PlotPane::GetTextPtSize(), CGUI_PlotPane::GetTypeFace(), TRUE, TRUE, 0.0F, CGUI_PlotPane::GetLabelColor());

					//	iStatus = PlObjPaintAll(pCtx);	// Show all objects. Done in UpdateDisplay.
				}
			}
		}

		ShowPlotLibErrMsg(iStatus);

		GUI_SetCursorShape(GUI_CURSOR_ARROW);	// Windows cursor not plot cursor.
	}
}



// =====================================================================
// =========================== CGUI_PlotPane ===============================
// =====================================================================

/*+/csubr/TOC===============================================================
* CGUI_PlotPane constructor - create a "pane" for presenting a graph.
*
* Purpose:
*	Takes a group box within a CGUI_Dlg dialog and uses that to determine
*	the "real estate" for diplaying a graph generated with the WMCA / PL
*	library. This class takes care of most of the interactions between the
*	GUI library and the PLOT library and also handles many of the lower-
*	level details of the PLOT library.
*
*	Limitations:
*		Assumes only one column of graph areas.
*		Assumes all graph areas have the same range in X and that range
*			is displayed below the bottom area only.
*		No facility for zooming the Y axis.
*		Line plot (vs. scatter) => X data array is sorted.
*
* Return Value:
*	None.
*
* Notes:
*	The plot context is available to the consumer of this class and therefore
*	the consummer has a lot of freedom to interect directly with the plot
*	library. This class interface is intended to cover the most common uses of
*	the plot library but by no means is it intended to completly duplicate
*	the entire plot library interface.
*
*	The CGUI_PlotPane class is derived from CGUI_CallBack. This makes it
*	convenient for this class (and classes derived from this class) to
*	register member functions as call back functions to GUI lib fuctions.
*
* See Also:
*	AddArea, GetStyleY, GetLimitsY, GetCurrentCursorPos, GetPlotContext,
*	GetDialog, GetAnnotationX, GetAnnotationY, MouseClickNotify,
*	MouseDblClickedNotify, MouseDraggedNotify, Print, RegisterBottomCtrls,
*	RegisterBottomRightCtrls, RegisterRightCtrls, RemoveArea, ResizeNotifyFn,
*	ServiceRightMouseBtn, SetCurrentCursorPos, SetFocusToGraph, SetLogY,
*	SetLimitsX, SetLimitsY, SetStyleY, SetUserAnnotation, SetZoomAutoScaleY,
*	SetZoomFactorX, UpdateAreaData, UpdateDisplay.
*
*	Plot library documentation.
*
*-Date      Author     Revision
* 21-Jan-97  ToM       Created.
*-===========================================================================*/

CGUI_PlotPane::CGUI_PlotPane(
	CGUI_Dlg *pDlg,		// I - pointer to parent CGUI_Dlg
	RESOURCE_ID PaneID)	// I - RESOURCE ID of group box defining graph area.
	: CGUI_CallBack()
{
	#ifdef RAISED_PANEL
		const UINT uiFrameWidth = 5;
		const int iBevelWidth = 10;
		const UINT uiTotalFrameWidth = uiFrameWidth + iBevelWidth;
	#endif

	mbValidPlotPane = FALSE;
 	mbPrinting = FALSE;
	mpPlotLibMsgWin = NULL;
	muiZoomFactor = 1;
	BOOL bEraseBkg = FALSE;
	zRect DspRect;
	HWND hwPlot = NULL;
	int iStatus;
	muiCurrentArea = 0;
	muiNumAreas = 0;
	mdCurrentCursX = 0.0;
	mulCursorIndex = 0;
	mbRefreshBlocked = FALSE;
	mbSuppressUpdate = FALSE;
	mPaneID = PaneID;
	mpDlg = pDlg;
	//pjm 20 Dec 2007 Resolution of issue 82 eliminates need for mbGraphHasFocus
	//mbGraphHasFocus = FALSE; 
	mbUserAnnotX = FALSE;
	mbUserAnnotY = FALSE;
	mnaRightCtrlID = NULL;
	mnaBottomCtrlID = NULL;
	mnaBottomRightCtrlID = NULL;
	mbAutoScaleX = TRUE;
	mbCursorVisible = TRUE;
	miMajorGridLines = 4;

	// Register a callback fn. for user-resizing dialog box containing plot.
	pDlg->SetResizeNotifyFn(this, (GUI_CB_DlgResizeFn)&CGUI_PlotPane::ResizeNotifyFn);

	// Define a group box of give resource ID to establish real estate for plot.
#ifdef RAISED_PANEL
	// RAISED_PANEL does NOT create a raised panel and by calling DefinePane vs.
	// DefineGroupBox the mouse events in the graph are not seen by the plot
	// library -- so for now do NOT define RAISED_PANEL	21-Jan-97.
	if (pDlg->DefinePane(PaneID, NULL, RGB(0, 0, 0 ), uiFrameWidth, iBevelWidth)) {
#else
	if (pDlg->DefineGroupBox(PaneID)) {
#endif
		// Make it invisible to minimize flashing during initial setup.
		pDlg->SetVisible(PaneID, FALSE);

		// Create a plot window in the area "reserved" by the group box.
#ifdef RAISED_PANEL
		if (pDlg->PaneGetClientRect(PaneID, &DspRect)) {
			int iLeft = DspRect.left() + uiTotalFrameWidth;
			int iTop = DspRect.top() + uiTotalFrameWidth;
			int iWidth = DspRect.width() - 2 * uiTotalFrameWidth;
			int iHeight = DspRect.height() - 2 * uiTotalFrameWidth; 
#else
		if (pDlg->GetRect(PaneID, &DspRect)) {
			int iLeft = DspRect.left();
			int iTop = DspRect.top();
			int iWidth = DspRect.width();
			int iHeight = DspRect.height(); 
#endif

			hwPlot = PlWinCreate(pDlg->GetWndHandle(), mGraphBkgColor, mGraphFG_Color,
				iLeft, iTop, iWidth, iHeight);
			if (hwPlot != NULL) {                      
				// When the Windows input focus shifts to the group box we want GUI lib
				// to pass the focus on to the plot window so the plot lib gets input.
				pDlg->SetChildFocusWndHandle(PaneID, hwPlot);
				iStatus = PlPlotInit_win(&mPlCtx, hwPlot, 1, 1);
				if (iStatus == PL_OK) {
					iStatus = PlPlotStyle(&mPlCtx, 0, 1, 0, 0, CGUI_PlotPane::GetTextPtSize(), CGUI_PlotPane::GetTypeFace());
					if (iStatus == PL_OK) {
						// Register a callback fn for tracking the cursor.
						iStatus = PlWinRegCursorCallback(&mPlCtx,
							&CGUI_PlotPane::PlotLibCursorCallBackFn, this);
						if (iStatus == PL_OK) {
							iStatus = PlWinRegEventCallback(&mPlCtx,
								&CGUI_PlotPane::PlotLibEventPreFilterCallBackFn, this, TRUE);
							if (iStatus == PL_OK) {
								iStatus = PlWinRegPlotCallback(&mPlCtx,
									&CGUI_PlotPane::PlotLibPlotCallBackFn, this);
/*
								if (iStatus == PL_OK) {
									iStatus = PlWinRegObjectCallback(&mPlCtx,
										&CGUI_PlotPane::PlotLibObjCallBackFn, this);
								}
*/
							}
						}
					}
				}
			}
		}
	}
}			


CGUI_PlotPane::~CGUI_PlotPane()
{
	UINT ui;
	int iStatus;
	
	// De-register all callback fns.
	iStatus = PlWinRegCursorCallback(&mPlCtx, NULL, NULL);
	ShowPlotLibErrMsg(iStatus);
	iStatus = PlWinRegEventCallback(&mPlCtx, NULL, NULL, TRUE);
	ShowPlotLibErrMsg(iStatus);
	iStatus = PlWinRegPlotCallback(&mPlCtx, NULL, NULL);
	ShowPlotLibErrMsg(iStatus);

	const UINT uiMAX_LOOP_COUNT = 10;	// Some resonably small but non-zero number.
	UINT uiLoopCount = 0;

	do {
		iStatus = PlPlotWrapup(&mPlCtx);
		ShowPlotLibErrMsg(iStatus);
		if (iStatus == PL_DIALOG_BUSY) {
			CGUI_App::Wait(1.0);
			uiLoopCount++;
		}
	} while ((iStatus == PL_DIALOG_BUSY) && (uiLoopCount < uiMAX_LOOP_COUNT));

	// Terminate the  window message loop.
	// Next line of code added 07-aug-98 ToM
	// The plot context is allocated outside of the plot library. If that
	// memory is part of a class that is deleted when the graph is wrapping
	// up there is a problem. The problem is that PlWinProc() gets two
	// messages (WM_DESTROY=2 & WM_NCDESTROY=130) later which will cause a reference
	// to now-invalid memory and can result in a GPF. So after wrapping up we
	// will destroy the message loop to prevent this from happening.
	
	PlWinDestroy(&mPlCtx, mPlCtx.hwPlot);

	if (mpPlotLibMsgWin != NULL) {
		mpPlotLibMsgWin->SetFocus();	// To force it to the front of plot dialog.
		GUI_MsgBox("Ready to close Plot\nLIB error log window.");
		delete mpPlotLibMsgWin;
	}
	
	for (ui = 0; ui < muiNumAreas; ui++) {
		delete mapCPlotAreaAxes[ui];
		mapCPlotAreaAxes[ui] = NULL;
	}

	delete [] mnaRightCtrlID;
	delete [] mnaBottomCtrlID;
	delete [] mnaBottomRightCtrlID;
	
	// Remove check for orphaned huge memory buffers because if a CGUI_TextWin is created
	// while a plot dialog is open we will get an erroneous error msg.
	// If / when CheckForOrphans is made more sophisticated to avoid this erroneous msg.
	// we can re-enable the check.
	// During development I have had this turned on and I think there is a good chance that
	// there are not huge-memory-buffer leaks in the CGUI_PlotPane code. Tom 9-Apr-97.
	
	//CGUI_HugeMem::CheckForOrphans();
}

	
/*+/csubr/TOC===============================================================
* AddArea - Create a plotting "area" at the bottom of the plot pane.
*
* Purpose:
*	Create a new area / axis for presenting a graph within the overall
*	plot area -- this new area is added at the bottom of the areas.
*
* Return Value:
*	BOOL - TRUE => success.
*
* Notes:
*	There are overloaded versions so this function. float Y vs. float X;
*	float Y vs. unsigned long X; unsigned long Y vs. unsigned long X.
*
*	If the bCopyData parameter is FALSE, the calling program must make sure
*	the data arrays stay around until the CGUI_PlotPane has been destroyed.
*	If the bCopyData paramter is TRUE a copy of the data arrays is created
*	and the calling program can free or reuse its copy of the data arrays
*	any time after the return from this function.

	UINT *puiAreaIndex			// O - Handle (0-based index) of newly added area.
	float huge *afX_Data		// I - address of float array of X data, NULL => use 0 thru ulDataDim-1
	float huge *afY_Data		// I - address of float array of Y data.
	ULONG ulDataDim				// I - Number of data points in arrays.
	BOOL bCopyData				// I - TRUE => library should make a local copy of data.
	const char *szAreaLabel		// I - Text label for this area.
*
* See Also:
*	AddArea(over-loads), InsertArea, RemoveArea, UpdateAreaData,
*	CGUI_PlotPane constructor.
*
*-Date      Author     Revision
* 21-Jan-97  ToM       Created.
* 19-Jun-2002	SFK		Added double/double overload
*-===========================================================================*/
// add a float/float plot area to the bottom of graph
BOOL CGUI_PlotPane::AddArea(UINT *puiAreaIndex, float huge *afX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{                    
	BOOL bSuccess = FALSE;
	*puiAreaIndex = 0;
	
	if (muiNumAreas < uiMAX_NUM_PLOT_AREAS) {
		if (afY_Data != NULL) {		// Implicit y data is not allowed.
			CPlotArea *pPlotArea = new CPlotArea(this, &mPlCtx, muiNumAreas, afX_Data, afY_Data, ulDataDim, bCopyData, szAreaLabel);
			bSuccess = RegisterNewPlotArea(pPlotArea, puiAreaIndex, muiNumAreas);
		}
	}
	return(bSuccess);
}

// add a ulong/ulong plot area to the bottom of graph
BOOL CGUI_PlotPane::AddArea(UINT *puiAreaIndex, ULONG huge *aulX_Data, ULONG huge *aulY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{                    
	BOOL bSuccess = FALSE;
	*puiAreaIndex = 0;
	
	if (muiNumAreas < uiMAX_NUM_PLOT_AREAS) {
		if (aulY_Data != NULL) {		// Implicit y data is not allowed.
			CPlotArea *pPlotArea = new CPlotArea(this, &mPlCtx, muiNumAreas, aulX_Data, aulY_Data, ulDataDim, bCopyData, szAreaLabel);
			bSuccess = RegisterNewPlotArea(pPlotArea, puiAreaIndex, muiNumAreas);
		}
	}
	return(bSuccess);
}

// add a ulong/float plot area to the bottom of graph
BOOL CGUI_PlotPane::AddArea(UINT *puiAreaIndex, ULONG huge *aulX_Data, float huge *afY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{                    
	BOOL bSuccess = FALSE;
	*puiAreaIndex = 0;
	
	if (muiNumAreas < uiMAX_NUM_PLOT_AREAS) {
//		if (afY_Data != NULL) {		// Implicit y data is not allowed.
		// However we may want to go ahead and display the plot area and label.
			CPlotArea *pPlotArea = new CPlotArea(this, &mPlCtx, muiNumAreas, aulX_Data, afY_Data, ulDataDim, bCopyData, szAreaLabel);
			bSuccess = RegisterNewPlotArea(pPlotArea, puiAreaIndex, muiNumAreas);
//		}
	}
	return(bSuccess);
}

// add a double/double plot area to the bottom of graph
BOOL CGUI_PlotPane::AddArea(UINT *puiAreaIndex, double huge *adX_Data, double huge *adY_Data, ULONG ulDataDim, BOOL bCopyData, const char *szAreaLabel)
{                    
	BOOL bSuccess = FALSE;
	*puiAreaIndex = 0;
	
	if (muiNumAreas < uiMAX_NUM_PLOT_AREAS) {
		if (adY_Data != NULL) {		// Implicit y data is not allowed.
			CPlotArea *pPlotArea = new CPlotArea(this, &mPlCtx, muiNumAreas, adX_Data, adY_Data, ulDataDim, bCopyData, szAreaLabel);
			bSuccess = RegisterNewPlotArea(pPlotArea, puiAreaIndex, muiNumAreas);
		}
	}
	return(bSuccess);
}


/*+/csubr/TOC===============================================================
* InsertArea - Create & insert a plotting "area" in the plot pane.
*
* Purpose:
*	Create a new area / axis for presenting a graph within the overall
*	plot area. Place the new area at the specified position in the order
*	of the pre-existing plot areas.
*
* Return Value:
*	BOOL - TRUE => success.
*
* Notes:
*	Generally the index returned in *puiAreaIndex will be the same as that
*	passed in uiAreaIndexBefore if the call is successful. However, if the
*	uiAreaIndexBefore passed in is greater than the number of pre-existing
*	areas the value returned in *puiAreaIndex will be the number of areas
*	after the addition of the newest one -- i.e. there will not be a blank
*	area slot.
*
*	This will only create a blank graph area in the specified location. This
*	call will almost always be followed by a call to UpdateAreaData to set the
*	data and title of the new area. If one wants to add the new area to the
*	bottom of the pre-existing areas one would probably call AddArea rather
*	than InsertArea followed by UpdateAreaData.
*
*	This will affect the handles / position indices of any areas below the
*	the one just inserted.

	UINT uiAreaIndexBefore	// I - Place new area before this area.
	UINT *puiAreaIndex		// O - 0-based index of new area.
*
* See Also:
*	AddArea, RemoveArea, UpdateAreaData, CGUI_PlotPane constructor.
*
*-Date      Author     Revision
* 22-Jan-97  ToM       Created.
*-===========================================================================*/
// appears this is the way new areas are created in Rad
BOOL CGUI_PlotPane::InsertArea(UINT uiAreaIndexBefore, UINT *puiAreaIndex)	
{
	BOOL bSuccess = TRUE;
	UINT uiNewPlotAreaIndex;
	
	if (uiAreaIndexBefore < muiNumAreas) {
		uiNewPlotAreaIndex = uiAreaIndexBefore;
	}
	else {
		if (muiNumAreas < uiMAX_NUM_PLOT_AREAS) {
			uiNewPlotAreaIndex = muiNumAreas;
		}
		else {
			bSuccess = FALSE;
		}
	}

	if (bSuccess) {
		CPlotArea *pPlotArea = new CPlotArea(this, &mPlCtx, uiNewPlotAreaIndex);
		bSuccess = RegisterNewPlotArea(pPlotArea, puiAreaIndex, uiNewPlotAreaIndex);
	}
	
	return(bSuccess);
}


/*+/csubr/TOC===============================================================
* RemoveArea - Remove a plotting "area" from the plot pane.
*
* Purpose:
*	Removes the specified plot area from the plot pane.
*
* Return Value:
*	BOOL - TRUE => success.
*
* Notes:
*	This will affect the handles / position indices of any areas below the
*	the one removed.
*
* See Also:
*	AddArea, InsertArea, UpdateAreaData, CGUI_PlotPane constructor.

  	UINT uiAreaIndex	// I - position index returned by AddArea or InsertArea.
*
*-Date      Author     Revision
* 22-Jan-97  ToM       Created.
*-===========================================================================*/
BOOL CGUI_PlotPane::RemoveArea(UINT uiAreaIndex)
{
	BOOL bSuccess = FALSE;
	
	if (uiAreaIndex < muiNumAreas) {
		bSuccess = TRUE;
		delete mapCPlotAreaAxes[uiAreaIndex];
		for (UINT ui = uiAreaIndex; ui < muiNumAreas - 1; ui++) {
			mapCPlotAreaAxes[ui] = mapCPlotAreaAxes[ui + 1];
			mapCPlotAreaAxes[ui]->SetAreaIndex(ui);
		}
		mapCPlotAreaAxes[muiNumAreas - 1] = NULL;
		SetNumAreas(muiNumAreas - 1);
		SetRangeX();
	}
	return(bSuccess);
}


/*+/csubr/TOC===============================================================
* UpdateAreaData - Update the data and title for a plot area.
*
* Purpose:
*	Notify the plot of new data to be displayed.
*
* Return Value:
*	BOOL - TRUE => success.
*
* Notes:
*	If one is updating the data in several areas it is best to have bUpdateDsp
*	set to FALSE except for the last update call to minimize screen flashing.
*
*	There are overloaded versions so this function: double Y vs. double X;
*	float Y vs. float X; float Y vs. unsigned long X; unsigned long Y vs. unsigned long X.
*
* See Also:
*	AddArea, DeleteArea, InsertArea, UpdateAreaData (over-loads),
*	CGUI_PlotPane constructor.

	UINT uiAreaIndex,		// I - handle / index of area to update.
	double huge *adX_Data,	// I - array of X-axis values. NULL => use 0 thru ulNumPts-1
	double huge *adY_Data,	// I - array of Y-axis values.
	ULONG ulNumPts,			// I - number of points is data arrays.
	BOOL bCopyData,			// I - make local copy of data arrays.
	const char *szLabel,	// I - new area title; optional, default = NULL => no change.
	BOOL bUpdateDsp			// I - TRUE => update screen display
*
*-Date      Author     Revision
* 22-Jan-97  ToM       Created.
*-===========================================================================*/
// double/double overload
BOOL CGUI_PlotPane::UpdateAreaData(
	UINT uiAreaIndex, 
	double huge *adX_Data,
	double huge *adY_Data, ULONG ulNumPts,	
	BOOL bCopyData, const char *szLabel, 
	BOOL bUpdateDsp)
{
	OutputDebugString("CGUI_PlotPane::UpdateAreaData(double,double)\n");

	BOOL bSuccess = FALSE;
	CPlotArea *pAreaPlot;

	// based on the index, get the pointer to the plot area context associated with the index
	if (GetAreaPlot(uiAreaIndex, &pAreaPlot)) {
		if (pAreaPlot->IsValidUpdateData(adX_Data, adY_Data, ulNumPts, szLabel)) {
			bSuccess = pAreaPlot->UpdateData(adX_Data, adY_Data, ulNumPts, bCopyData, szLabel);
		}

		SetRangeX();
		SetRangeY(uiAreaIndex);
		if (bUpdateDsp) InvalidateGraphDisplay();	// Schedule a repaint.
	}
	return(bSuccess);
}

// float/float overload
BOOL CGUI_PlotPane::UpdateAreaData(UINT uiAreaIndex, float *afX_Data, float huge *afY_Data, ULONG ulNumPts, BOOL bCopyData, const char *szLabel, BOOL bUpdateDsp)
{
	BOOL bSuccess = FALSE;
	CPlotArea *pAreaPlot;

	if (GetAreaPlot(uiAreaIndex, &pAreaPlot)) {
		if (pAreaPlot->IsValidUpdateData(afX_Data, afY_Data, ulNumPts, szLabel)) {
			bSuccess = pAreaPlot->UpdateData(afX_Data, afY_Data, ulNumPts, bCopyData, szLabel);
		}

		SetRangeX();
		SetRangeY(uiAreaIndex);
		if (bUpdateDsp) InvalidateGraphDisplay();	// Schedule a repaint.
	}
	return(bSuccess);
}

// ulong/ulong overload
BOOL CGUI_PlotPane::UpdateAreaData(UINT uiAreaIndex, ULONG huge *aulX_Data, ULONG huge *aulY_Data, ULONG ulNumPts, BOOL bCopyData, const char *szLabel, BOOL bUpdateDsp)
{
	BOOL bSuccess = FALSE;
	CPlotArea *pAreaPlot;

	if (GetAreaPlot(uiAreaIndex, &pAreaPlot)) {
		if (pAreaPlot->IsValidUpdateData(aulX_Data, aulY_Data, ulNumPts, szLabel)) {
			bSuccess = pAreaPlot->UpdateData(aulX_Data, aulY_Data, ulNumPts, bCopyData, szLabel);
		}

		SetRangeX();
		SetRangeY(uiAreaIndex);
		InvalidateGraphDisplay();	// Schedule a repaint.
	}
	return(bSuccess);
}

// ulong/float overload
BOOL CGUI_PlotPane::UpdateAreaData(UINT uiAreaIndex, ULONG huge *aulX_Data, float huge *afY_Data, ULONG ulNumPts, BOOL bCopyData, const char *szLabel, BOOL bUpdateDsp)
{
	BOOL bSuccess = FALSE;
	CPlotArea *pAreaPlot;

	if (GetAreaPlot(uiAreaIndex, &pAreaPlot)) {
		if (pAreaPlot->IsValidUpdateData(aulX_Data, afY_Data, ulNumPts, szLabel)) {
			bSuccess = pAreaPlot->UpdateData(aulX_Data, afY_Data, ulNumPts, bCopyData, szLabel);
		}

		SetRangeX();
		SetRangeY(uiAreaIndex);
		InvalidateGraphDisplay();	// Schedule a repaint.
	}
	return(bSuccess);
}


/*+/csubr/TOC/FF===============================================================
* GetStyleY - Retrieve style parameters for Y axis.
*
* Purpose:	Retrieve style parameters for Y axis (whether using log scale,
*			whether using autoscale and whether want major grid lines)
*
* Return Value:
*	TRUE => success. FALSE => invalid axis index.
*
* Notes:
*	The Y axis for any area individually (or for all areas if uiAreaIndex is set
*	to GUI_PLOT_ALL_AREAS) has two style parameters covered by the CGUI_PlotPane
*	class. 1) The Y axis can be in log or linear display mode. 2) By default the
*	Y axis is in auto-range mode -- the minimum and maximum Y axis values are
*	automatically adjusted to be aesthetically pleasing values which maximize
*	the display of the vertical details (Y values) of the data. Sometimes it is
*	desirable to keep that Y axis display range constant as the user zooms or
*	scrolls along the X axis. Sometimes it may be desirable to have the Y axis
*	range automatically adjust to the range of Y values just in the currently
*	visible portion of the data -- not over the entire set of data (unzoomed).
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX

	UINT uiAreaIndex		// I - handle / index of area.
	BOOL *pbLogY_Axis		// O - TRUE if Y axis is in log display mode.
	BOOL *pbZoomAutoScale	// O - TRUE if Y axis auto ranges during zoom.
	int *piMajorGridLines	// O - See pl lib documentation, PlAreaYStyle().

*
*-Date      Author     Revision
* 22-Jan-97  ToM       Created.
*-===========================================================================*/
BOOL CGUI_PlotPane::GetStyleY(UINT uiAreaIndex, BOOL *pbLogY_Axis, BOOL *pbZoomAutoScale, int *piMajorGridLines)
{
	BOOL bSuccess = FALSE;
	CPlotArea *pPlotArea;

	if (piMajorGridLines != NULL) *piMajorGridLines = miMajorGridLines;

	if (GetAreaPlot(uiAreaIndex, &pPlotArea)) {
		pPlotArea->GetStyleY(pbLogY_Axis, pbZoomAutoScale);
		bSuccess = TRUE;
	}
	return(bSuccess);
}


/*+/csubr/TOC===============================================================
* SetStyleY - Set style parameters for Y axis.
*
* Purpose:  Set style parameters for Y axis (whether log scale, whether autoscale
*			whether use major gridlines)
*
* Return Value:
*	TRUE => success. FALSE => invalid axis index.
*
* Notes:
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX

	UINT uiAreaIndex,		// I - handle / index of area.
	BOOL bLogY_Axis,		// I - TRUE => log Y axis, FALSE, linear Y axis.
	BOOL bZoomAutoScale,	// I - TRUE => Auto range Y axis during zoom.
	int iMajorGridLines		// I - See pl lib documentation; default = 4.
*
*-Date      Author     Revision
* 22-Jan-97  ToM       Created.
*-===========================================================================*/
BOOL CGUI_PlotPane::SetStyleY(UINT uiAreaIndex, BOOL bLogY_Axis, BOOL bZoomAutoScale, int iMajorGridLines)
{
	BOOL bSuccess = FALSE;
	int iStatus;
	int iVerticalLogOrLinear;
	int iNumColHorzAnnotY;
	CPlotArea *pPlotArea;
	BOOL bNeedToRepaint = FALSE;

	miMajorGridLines = iMajorGridLines;

	if (GetAreaPlot(uiAreaIndex, &pPlotArea)) {
		BOOL bPrevLogY_Axis, bPrevZoomAutoScale;
			
		pPlotArea->GetStyleY(&bPrevLogY_Axis, &bPrevZoomAutoScale);
		pPlotArea->SetStyleY(bLogY_Axis, bZoomAutoScale);
		if (bLogY_Axis != bPrevLogY_Axis) {
			bNeedToRepaint = TRUE;
		}

		iStatus = PlAreaArraySelect(&mPlCtx, uiAreaIndex, 0);
		if (iStatus == PL_OK) {
			int iLocalMajorGridLines = miMajorGridLines;
			int eStyle;
			float fAreaAnnotDeg;
			char *szFmt;
			int iTitleLinesLeft = 1;
			int iTitleLinesRight = 0;
						
			if (bLogY_Axis) {
				iVerticalLogOrLinear = PL_LOG;
				iLocalMajorGridLines = 2;	// Grid lines on all decades; over-ride caller's setting.
				eStyle = PL_GRID_DOT_AXIS;	// Dotted grid lines.
				fAreaAnnotDeg = mfAreaY_AnnotDegLog;
				iNumColHorzAnnotY = miNumColHorzY_AnnotLog;
				szFmt = mszY_AnnotDefaultFmtLog;
			}
			else {
				iVerticalLogOrLinear = PL_LIN;
				// iMajorGridLines = 1;	// Label only min and max.
				//iMajorGridLines = 4;	// Break y value into 4 quadrants.
				//eStyle = PL_TICK_AXIS;	// No grid lines.
				eStyle = PL_GRID_DOT_AXIS;	// Dotted grid lines.
				fAreaAnnotDeg = mfAreaY_AnnotDegLinear;
				iNumColHorzAnnotY = miNumColHorzY_AnnotLinear;
				// Set decimal places according to data range.
				szFmt = mszY_AnnotDefaultFmtLinear;
			}
						
			iStatus = PlAreaYStyle(&mPlCtx, eStyle, iVerticalLogOrLinear,
				iLocalMajorGridLines, 1,
				fAreaAnnotDeg, iNumColHorzAnnotY,
				CGUI_PlotPane::GetTextPtSize(), CGUI_PlotPane::GetTypeFace(), szFmt,
				iTitleLinesLeft, iTitleLinesRight,
				CGUI_PlotPane::GetTextPtSize(), CGUI_PlotPane::GetTypeFace());

				// Even if we have failed above try to set the Y range, but as
				// a diagnostic aid make sure we repor the PL lib error msg first.
				ShowPlotLibErrMsg(iStatus);
				iStatus = PL_OK; 	// Clear error status. SetRangeY will report its own msgs.
				SetRangeY(uiAreaIndex);
		}
	
		ShowPlotLibErrMsg(iStatus);
	}
	
	if (bNeedToRepaint) {
		InvalidateGraphDisplay();	// Schedule a repaint.
	}
	return(bSuccess);
}


/*+/csubr/TOC===============================================================
* SetLogY - Set log / linear display of Y axis.
*
* Purpose: Set the log/linear part of the Y axis style parameters.
*
* Return Value:
*	TRUE => success. FALSE => invalid axis index.
*
* Notes:
*	The Y axis for any area individually (or for all areas if uiAreaIndex is set
*	to GUI_PLOT_ALL_AREAS) can be in log or linear display mode.
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX

	UINT uiAreaIndex,	// I - handle / index of area.
	BOOL bLogY_Axis		// I - TRUE => log display.

*
*-Date      Author     Revision
* 22-Jan-97  ToM       Created.
*-===========================================================================*/
BOOL CGUI_PlotPane::SetLogY(UINT uiAreaIndex, BOOL bLogY_Axis)
{
	BOOL bSuccess = FALSE;
	CPlotArea *pPlotArea;
	UINT uiFirstArea, uiLastArea;
	BOOL bPrevLogY_Axis, bPrevZoomAutoScale;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pPlotArea)) {
			bSuccess = TRUE;			
			pPlotArea->GetStyleY(&bPrevLogY_Axis, &bPrevZoomAutoScale);
			SetStyleY(ui, bLogY_Axis, bPrevZoomAutoScale, miMajorGridLines);
		}
	}
	return(bSuccess);
}


/*+/csubr/TOC===============================================================
* SetZoomAutoScaleY - Set auto-range of Y data during zoom.
*
* Purpose:  Set autorange part of Y style parameters
*
* Return Value:
*	TRUE => success. FALSE => invalid axis index.
*
* Notes:
*	The Y axis for any area individually (or for all areas if uiAreaIndex is set
*	to GUI_PLOT_ALL_AREAS) can auto range during an X zoom operation.
*	By default the Y axis is in auto-range mode -- the minimum and maximum Y
*	axis values are automatically adjusted to be aesthetically pleasing values
*	which maximize the display of the vertical details (Y values) of the data.
*	Sometimes it is desirable to keep that Y axis display range constant as the
*	user zooms or scrolls along the X axis. Sometimes it may be desirable to
*	have the Y axis range automatically adjust to the range of Y values just
*	in the currently visible portion of the data -- not over the entire set
*	of data (unzoomed).
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX

	UINT uiAreaIndex		// I - handle / index of area
	BOOL bZoomAutoScale		// I - TRUE => auto range Y during zoom.

*
*-Date      Author     Revision
* 22-Jan-97  ToM       Created.
*-===========================================================================*/
BOOL CGUI_PlotPane::SetZoomAutoScaleY(
	UINT uiAreaIndex,	// I - handle / index of area
	BOOL bZoomAutoScale)	// I - TRUE => auto range Y during zoom.
{
	BOOL bSuccess = FALSE;
	CPlotArea *pPlotArea;
	UINT uiFirstArea, uiLastArea;
	BOOL bPrevLogY_Axis, bPrevZoomAutoScale;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pPlotArea)) {
			bSuccess = TRUE;			
			pPlotArea->GetStyleY(&bPrevLogY_Axis, &bPrevZoomAutoScale);
			SetStyleY(ui, bPrevLogY_Axis, bZoomAutoScale, miMajorGridLines);
		}
	}
	return(bSuccess);
}


/*+/csubr/TOC/FF===============================================================
* GetLimitsY - Retrieve Y values range parameters.
*
* Purpose:	Get the min/max values of the part of the Y data currently visible
*
* Return Value:
*	TRUE => success. FALSE => invalid axis index.
*
* Notes:
*	If the user is zoomed in on the x axis, the range of data displayed
*	is different from the full range of data.
*
*	If bAutoScale is TRUE, *pdMinY and *pdMaxY are the minimim and maximum
*	values of the Y data array. If bAutoScale is FALSE, *pdMinY and *pdMaxY
*	are the values specified by the user (via calling SetLimitsY) for the
*	minimum and maximum values displayed on the Y axis.
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX

	UINT uiAreaIndex,	// I - handle / index of area.
	BOOL *pbAutoScale,	// O - TRUE => Y display range <= actual data
	double *pdY_Min,	// O - Minimum value of Y-axis display.
	double *pdY_Max		// O - Maximum value of Y-axis display.

*-Date      Author     Revision
* 22-Jan-97  ToM       Created.
* 01-Jul-98  ToM       Changed to return display range vs. actual data range.
*-===========================================================================*/
BOOL CGUI_PlotPane::GetLimitsY(UINT uiAreaIndex, BOOL *pbAutoScale, double *pdY_Min, double *pdY_Max)
{
	double dDummyMinX, dDummyMaxX;
	BOOL bSuccess = FALSE;
	CPlotArea *pPlotArea;
	int iStatus;
	BOOL bVisibleOnly;
	double dMinX, dMaxX, dMinY, dMaxY;

	if (GetAreaPlot(uiAreaIndex, &pPlotArea)) {
		bSuccess = pPlotArea->GetDataRange(&dDummyMinX, &dDummyMaxX, pdY_Min, pdY_Max, pbAutoScale);

		// PlAreaRangeGet works on currently selected area so set it after remembering current area.
		UINT uiCurrentArea = GetCurrentArea();
		if (uiAreaIndex != uiCurrentArea) SetCurrentArea(uiAreaIndex);
		bVisibleOnly = TRUE;	// i.e. visible range.
		iStatus = PlAreaRangeGet(&mPlCtx, bVisibleOnly, &dMinX, &dMaxX, &dMinY, &dMaxY);
		if (iStatus == PL_OK) {
			if (pdY_Min != NULL) *pdY_Min = dMinY;
			if (pdY_Max != NULL) *pdY_Max = dMaxY;
		}
		else {
			if (pdY_Min != NULL) *pdY_Min = 0.0;
			if (pdY_Max != NULL) *pdY_Max = 1.0;
		}
		if (uiAreaIndex != uiCurrentArea) SetCurrentArea(uiCurrentArea);
	}

	return(bSuccess);
}


/*+/csubr/TOC===============================================================
* SetLimitsY - Set Y values range parameters.
*
* Purpose:
*
* Return Value:
*	TRUE => success. FALSE => invalid axis index.
*
* Notes:
*	The input parameters will be applied to all areas if uiAreaIndex is set
*	to GUI_PLOT_ALL_AREAS.
*
*	If the user is zoomed in on the x axis, the range of data displayed
*	is different from the full range of data.
*
*	If bAutoScale is TRUE, dMinY and dMaxY are ignored because min and max
*	for the display are determined by the range of the actual data.
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX

	UINT uiAreaIndex,	// I - handle / index of area.
	BOOL bAutoScale,	// I - FALSE => use next two parameters
                        //     TRUE => data => display range.
	double dMinY,		// I - Minimum value shown on Y axis;
	double dMaxY)		// I - Maximum value shown on Y axis.

*
*-Date      Author     Revision
* 22-Jan-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::SetLimitsY(UINT uiAreaIndex, BOOL bAutoScale, double dMinY,	double dMaxY)
{
	BOOL bSuccess = FALSE;
	CPlotArea *pPlotArea;
	UINT uiFirstArea, uiLastArea;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pPlotArea)) {
			pPlotArea->SetLimitsY(bAutoScale, dMinY, dMaxY);
			SetRangeY(ui);
			bSuccess = TRUE;
		}
	}
	
	return(bSuccess);
}


/*+/csubr/TOC/FF===============================================================
* GetLimitsX - Retrieve visible and full range of X values.
*
* Purpose:
*
* Return Value:
*	None.
*
* Notes:
*	If the user is zoomed in on the x axis, the range of data displayed
*	is different from the full range of data.
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX
*
*-Date      Author     Revision
* 22-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::GetLimitsX(
	double *pdMinFullX,		// O - Minimum actual X data value.
	double *pdMaxFullX,		// O - Maximum actual X data value.
	double *pdMinVisibleX,	// O - Current minimum visible X value.
	double *pdMaxVisibleX)	// O - Current maximum visible X value.
{
	int iStatus;
	BOOL bVisibleOnly;
	double dMinX, dMaxX, dMinY, dMaxY;
	
	// Don't care which area is selected because they all have the same x range.
	bVisibleOnly = FALSE;	// i.e. full range.
	iStatus = PlAreaRangeGet(&mPlCtx, bVisibleOnly, &dMinX, &dMaxX, &dMinY, &dMaxY);
	if (iStatus == PL_OK) {
		if (pdMinFullX != NULL) *pdMinFullX = dMinX;
		if (pdMaxFullX != NULL) *pdMaxFullX = dMaxX;
	}
	else {
		if (pdMinFullX != NULL) *pdMinFullX = 0.0;
		if (pdMaxFullX != NULL) *pdMaxFullX = 1.0;
	}
	
	bVisibleOnly = TRUE;	// i.e. visible range.
	iStatus = PlAreaRangeGet(&mPlCtx, bVisibleOnly, &dMinX, &dMaxX, &dMinY, &dMaxY);
	if (iStatus == PL_OK) {
		if (pdMinVisibleX != NULL) *pdMinVisibleX = dMinX;
		if (pdMaxVisibleX != NULL) *pdMaxVisibleX = dMaxX;
	}
	else {
		if (pdMinVisibleX != NULL) *pdMinVisibleX = 0.0;
		if (pdMaxVisibleX != NULL) *pdMaxVisibleX = 1.0;
	}
}


/*+/csubr/TOC===============================================================
* SetLimitsX - Set display limits for X axis.
*
* Purpose:
*	Tell GUI_PlotPane whether the minimum and maximum values displayed on
*	the X axis are to be determined from the X data array or set explicitly
*	by the calling program. If bAutoScale is TRUE the plot will select
*	"rationalized" minimum and maximum values for the X axis display based
*	on the range of data in the X data array -- In this case the dMin and
*	dMax parameters are ignored. If bAutoScale is FALSE the plot limits on
*	the X axis will be exactly dMin and dMax -- there will no attempt to
*	"rationalize" or in any way adjust those values.
*
* Return Value:
*	None.
*
* Notes:
*	If the user is zoomed in on the x axis, the range of data displayed
*	is different from the full range of data.
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::SetLimitsX(
	BOOL bAutoScale,	// I - Auto scale X range based on data.
	double dMin,		// I - Minimum value for X axis display.
	double dMax)		// I - Maximum value for X axis display.
{
	mbAutoScaleX = bAutoScale;
	if (!mbAutoScaleX) {
		mdManualMinX = dMin;
		mdManualMaxX = dMax;
	}
}


/*+/csubr/TOC/FF===============================================================
* GetZoomFactorX - Retrieve zoom factor for the X axis.
*
* Purpose:
*	After the user closes the dialog containing the graph it may be desirable
*	to have the X axis zoomed in when the user restarts the graph dialog next
*	time. The application program can accomplish this using the GetZoomFactorX
*	at shutdown and the SetZoomFactorX at startup.
*
* Return Value:
*	None.
*
* Notes:
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

UINT CGUI_PlotPane::GetZoomFactorX()
{
	return(muiZoomFactor);
}
 
 
/*+/csubr/TOC===============================================================
* SetZoomFactorX - Set zom factor for the X axis.
*
* Purpose:
*	Programatically set the zoom factor for the X axis. The user may set an
*	X zoom factor via the interface built into CGUI_PlotPane -- i.e. the
*	user can type the letter 'X' (upper or lower case) followed by a single
*	digit specifying the zoom factor of 1 through 9.
*
* Return Value:
*	None.
*
* Notes:
*	If the user is zoomed in on the x axis, the range of data displayed
*	is different from the full range of data but this does not affect
*	the range of data available to the plot. I.e. the plot can be un-zoomed
*	without CGUI_PlotPane having to go out and retrieve the full range of data.
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, GetZoomFactorX, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::SetZoomFactorX(
	UINT uiZoomFactor)	// I - Zoom-in factor for X axis.
{
	const float fVERT_ZOOM_FACTOR = 1.0F;
	
	if (uiZoomFactor > uiMAX_ZOOM_FACTOR) {
		uiZoomFactor = uiMAX_ZOOM_FACTOR;
	}
	else if (uiZoomFactor < 1) {
		uiZoomFactor = 1;
	}
	
	UINT uiPrevZoomFactor = muiZoomFactor;
	muiZoomFactor = uiZoomFactor;
	
	// It's likely to happen that the user who derives a class from CGUI_PlotPane
	// and over-rides ZoomChangeNotify will end up calling this base SetZoomFactor
	// so let's try to prevent an endless loop.
	if (uiPrevZoomFactor != muiZoomFactor) {
		PlWinZoom(&mPlCtx, (float)uiZoomFactor, fVERT_ZOOM_FACTOR);
		ZoomChangeNotify(muiZoomFactor);
	}
}
 
 
/*+/csubr/TOC/FF===============================================================
* CenterCursor - Move cursor to the center of the visible X data.
*
* Purpose:
*	Find the data point nearest to the center of the specified area and set
*	the plot cursor on tht point.
*
* Return Value:
*	None.
*
* Notes:
*	uiAreaIndex may be GUI_PLOT_CURRENT_AREA.
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, GetZoomFactorX, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::CenterCursor(
	UINT uiAreaIndex)	// I - index of area to receive cursor.
{
	if (uiAreaIndex == GUI_PLOT_CURRENT_AREA) uiAreaIndex = muiCurrentArea;

	CPlotArea *pAreaPlot;
	if (GetAreaPlot(muiCurrentArea, &pAreaPlot)) {
		pAreaPlot->CenterCursor();
	}
}


/*+/csubr/TOC===============================================================
* GetCursorPos - Retrieve cursor X, Y and area index.
*
* Purpose:
*
* Return Value:
*	None.
*
* Notes:
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, GetZoomFactorX, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::GetCursorPos(
	double *pdCursorX,	// O - X-value of cursor location.
	double *pdCursorY,	// O - Y-value of cursor location.
	UINT *puiAreaIndex)	// O - index of area containing cursor.
{

	*pdCursorX = mdCurrentCursX;   

	if (pdCursorY != NULL) {
		CPlotArea *pAreaPlot;

		if (GetAreaPlot(muiCurrentArea, &pAreaPlot)) {
			pAreaPlot->FindY_GivenX(mdCurrentCursX, pdCursorY);
		}
	}
	
	if (puiAreaIndex != NULL) {
		*puiAreaIndex = muiCurrentArea;
	}
}

// which index in the X-array is the cursor currently at
void CGUI_PlotPane::GetCursorUsingIndex(
	ULONG *pdCursorIndexX,	// O - index of cursor location.
	//double *pdCursorY,	// O - Y-value of cursor location.
	UINT *puiAreaIndex)	// O - index of area containing cursor.
{

	*pdCursorIndexX = mulCursorIndex;   
//	unsigned long ulDum = mulCursorIndex;
	///xxxxxxxxxxxxxxxxxxxxx here?

//	if (pdCursorY != NULL) {
//		CPlotArea *pAreaPlot;

//		if (GetAreaPlot(muiCurrentArea, &pAreaPlot)) {
//			pAreaPlot->FindY_GivenX(mdCurrentCursX, pdCursorY);
//		}
//	}
	
	if (puiAreaIndex != NULL) {
		*puiAreaIndex = muiCurrentArea;
	}
}

/*+/csubr/TOC===============================================================
* SetCursorPos - Set cursor to specified area and X-value.
*
* Purpose:
*
* Return Value:
*	None.
*
* Notes:
*	If uiAreaIndex is GUI_PLOT_CURRENT_AREA the caller does not specify which
*	area -- the cursor stays in the area which it is currently in.
*
*	If the position specified is outside of the currently visible region
*	(because the graph is zoomed in X) the graph display is scrolled so that
*	the cursor is visible.
*
* See Also:
*	GetLimitsY, GetCurrentCursorPos, GetPlotContext, GetDialog, GetAnnotationX,
*	GetAnnotationY, GetZoomFactorX, SetLogY, SetLimitsX, SetLimitsY, SetStyleY,
*	SetUserAnnotation, SetZoomAutoScaleY, SetZoomFactorX
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::SetCursorPos(
	double dCursX,		// I - X value for new cursor location.
	UINT uiAreaIndex,	// I - DEFAULT = GUI_PLOT_CURRENT_AREA.
	BOOL bOnNearestDataPt,	// I - Force to nearest point,
	                        //     DEFAULT = TRUE.
	BOOL bUpdateDisplay)	// I - Force display to show new
							//		cursor loc. DEFAULT = TRUE.
{
	const float fJUMP_INCREMENT = 0.2F;
	float fX_AnchFrac;
	BOOL bVisible;
	double dXLeft, dXRight, dYBot, dYTop;
    int iStatus;
    
	if (bUpdateDisplay) ShowCursor(FALSE);

	if (uiAreaIndex == GUI_PLOT_CURRENT_AREA) uiAreaIndex = muiCurrentArea;
	else muiCurrentArea = uiAreaIndex;

	iStatus = PlAreaArraySelect(&mPlCtx, uiAreaIndex, 0);
	if (iStatus == PL_OK) {
		// Get full Area range to make sure requested X value is within Area limits.
		bVisible = FALSE;
		iStatus = PlAreaRangeGet(&mPlCtx, bVisible, &dXLeft, &dXRight, &dYBot, &dYTop);
		if (iStatus == PL_OK) {
			if (dCursX > dXRight) {
				dCursX = dXRight;
			}
			else if (dCursX < dXLeft) {
				dCursX = dXLeft;
			}
			
			CPlotArea *pAreaPlot;
			double dNearestActualPt;
			
			if (GetAreaPlot(uiAreaIndex, &pAreaPlot)) {
				pAreaPlot->FindNearestPtX(dCursX, &dNearestActualPt, &mulCursorIndex);
			}

			if (bOnNearestDataPt) dCursX = dNearestActualPt;

			// Get visible Area range.
			bVisible = TRUE;
			iStatus = PlAreaRangeGet(&mPlCtx, bVisible, &dXLeft, &dXRight, &dYBot, &dYTop);
			if (iStatus == PL_OK) {
				// If requested X value is not within visible range shift view port.
				if ((dCursX < dXLeft) || (dCursX > dXRight)) {
					if (dCursX < dXLeft) {
						// Cursor has gone out of view to the left.
						fX_AnchFrac = fJUMP_INCREMENT;
					}
					else {
						// Cursor has gone out of view to the right.
						fX_AnchFrac = 1 - fJUMP_INCREMENT;
					}
					iStatus = PlWinScroll_pos(&mPlCtx, dCursX, 0.0F, fX_AnchFrac, 0.0F);
				}
			}
		}
	}

	if (iStatus == PL_OK) {
		mdCurrentCursX = dCursX;
	}
	else {
		ShowPlotLibErrMsg(iStatus);
	}

	if (bUpdateDisplay) {
		ShowCursor(TRUE);
		UpdateCursorReadout(&mPlCtx);
		//InvalidateGraphDisplay();	// Schedule a repaintto take care of the ghost cursor. //SFK
								// Makes things jumpy so took back out
	}
}


void CGUI_PlotPane::SetCursorUsingIndex(
	ULONG ulNewCursIndex,		// I - index for new cursor location.
	UINT uiAreaIndex,			// I - DEFAULT = GUI_PLOT_CURRENT_AREA.
	BOOL bUpdateDisplay)		// I - Force display to show new cursor loc. DEFAULT = TRUE.
{
	const float fJUMP_INCREMENT = 0.2F;
	float fX_AnchFrac;
	double dXLeft, dXRight, dYBot, dYTop;
    int iStatus;
    
	if (bUpdateDisplay) ShowCursor(FALSE);

	if (uiAreaIndex == GUI_PLOT_CURRENT_AREA) uiAreaIndex = muiCurrentArea;
	else muiCurrentArea = uiAreaIndex;

	iStatus = PlAreaArraySelect(&mPlCtx, uiAreaIndex, 0);
	if (iStatus == PL_OK) {
		CPlotArea *pAreaPlot;
		if (GetAreaPlot(uiAreaIndex, &pAreaPlot)) {
			mulCursorIndex = ulNewCursIndex;

			if (pAreaPlot->SetCurrentCursX(mulCursorIndex, &mdCurrentCursX)) {
				// Get visible Area range.
				BOOL bVisible = TRUE;
				iStatus = PlAreaRangeGet(&mPlCtx, bVisible, &dXLeft, &dXRight, &dYBot, &dYTop);
				if (iStatus == PL_OK) {
					// If requested X value is not within visible range shift view port.
					if ((mdCurrentCursX < dXLeft) || (mdCurrentCursX > dXRight)) {
						if (mdCurrentCursX < dXLeft) {
							// Cursor has gone out of view to the left.
							fX_AnchFrac = fJUMP_INCREMENT;
						}
						else {
							// Cursor has gone out of view to the right.
							fX_AnchFrac = 1 - fJUMP_INCREMENT;
						}
						iStatus = PlWinScroll_pos(&mPlCtx, mdCurrentCursX, 0.0F, fX_AnchFrac, 0.0F);
					}
				}
			}
		}
	}

	if (iStatus != PL_OK) {
		ShowPlotLibErrMsg(iStatus);
	}

	if (bUpdateDisplay) {
		ShowCursor(TRUE);
		UpdateCursorReadout(&mPlCtx);
		//InvalidateGraphDisplay();	// Schedule a repaintto take care of the ghost cursor. //SFK
									// Makes things jumpy so took back out
	}
}


/*+/csubr/TOC===============================================================
* SetCursorVisible - Set cursor to specified visibility state.
*
* Purpose:
*	Make cursor (and cursor readout) visible or not.
*
* Return Value:
*	BOOL - previous visiblesate of cursor.
*
* Notes:
*
* See Also:
*	CenterCursor, GetCursorPos, SetCusorPos.
*
*-Date      Author     Revision
* 06-May-98  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::SetCursorVisible(
	BOOL bVisible)	// I - new visibility of cursor.
{
	BOOL bPrevCursorVisible = mbCursorVisible;

	UpdateCursorReadout(&mPlCtx);
	if (bVisible) {
		mbCursorVisible = bVisible;
		ShowCursor(mbCursorVisible, &mPlCtx);
	}
	else {
		ShowCursor(mbCursorVisible, &mPlCtx);
		mbCursorVisible = bVisible;
	}

	return(bPrevCursorVisible);
}


/*+/csubr/TOC/FF===============================================================
* EmphasizeRegion - Temporarily highlight trace via coloring.
*
* Purpose:
*	Similar to AddHiliteRegion except taht hilited / emphasized region is not stored.
*
* Return Value:
*	BOOL - TRUE if valid uiAreaIndex and ulFirstIndex <= ulLastIndex and
*	ulLastIndex < dimension of X data array. (This last restriction dropped 8-May-98
*	to allow creating a hilite region which will include points not yet existing.)
*
* Notes:
*	If uiAreaIndex is GUI_PLOT_ALL_AREAS a highlighted region will be
*	established for all of the graph areas.
*
*	If a graph is printed to a monochrome printer and the highlighted regions
*	are currently visible the highlighted regions will be printed "bolded".
*
* See Also:
*	AddMarker, ClearHiliteRegions, DeleteHiliteRegion, ShowHiliteRegions
*
*-Date      Author     Revision
* 30-Jun-98  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::EmphasizeRegion(
	UINT uiAreaIndex,		// I - Handle / index of area.
	double dMinX,			// I - X value of first emphasized point.
	double dMaxX,			// I - X value of last emphasized point.
	BOOL bNearestActualPt,	// I - TRUE => use pts nearest dMinx and dMaxX
	COLORREF Color,			// I - Color to highlight with.
	float fLineThicknessPts,// I - Line thickness in pts for drawing
	double *pdActualMinX,	// O - Actual min X (i.e. nearest pt to dMinX)
	double *pdActualMaxX)	// O - Actual max X (i.e. nearest pt to dMaxX)
{
	CPlotArea *pAreaPlot;
	UINT uiFirstArea, uiLastArea;
	BOOL bAnySuccess = FALSE;
	double dLocalActualMinX, dLocalActualMaxX;

	if (pdActualMinX == NULL) pdActualMinX = &dLocalActualMinX;
	if (pdActualMaxX == NULL) pdActualMaxX = &dLocalActualMaxX;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	UINT uiPrevCurrentArea = GetCurrentArea();

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pAreaPlot)) {
			SetCurrentArea(ui);	// Otherwise we will draw into the currently selected area.
			if (pAreaPlot->EmphasizeRegion(dMinX, dMaxX, bNearestActualPt, Color, fLineThicknessPts, pdActualMinX, pdActualMaxX)) {
				bAnySuccess = TRUE;
			}
		}
	}
	SetCurrentArea(uiPrevCurrentArea);
	return(bAnySuccess);
}

	
/*+/csubr/TOC===============================================================
* AddHiliteRegion - Highlight point(s) via coloring.
*
* Purpose:
*
* Return Value:
*	BOOL - TRUE if valid uiAreaIndex and ulFirstIndex <= ulLastIndex and
*	ulLastIndex < dimension of X data array. (This last restriction dropped 8-May-98
*	to allow creating a hilite region which will include points not yet existing.)
*
* Notes:
*	If uiAreaIndex is GUI_PLOT_ALL_AREAS a highlighted region will be
*	established for all of the graph areas.
*
*	If a graph is printed to a monochrome printer and the highlighted regions
*	are currently visible the highlighted regions will be printed "bolded".
*
*	Predefined Default colors are:
*		mGraphBkgColor = GUI_WHITE;
*		mGraphFG_Color = GUI_BLACK;
*		mCursorReadoutColor = GUI_BLUE;
*		mCursorColor = GUI_RED;	
*		mTraceColor = GUI_BLUE;
*		mLabelColor = GUI_BLUE;
*
* See Also:
*	AddMarker, ClearHiliteRegions, DeleteHiliteRegion, ShowHiliteRegions
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
* 19-Jun-2002	SFK		Changed comments since the values, not indexes, are passed in
*						Changed for DATE values
*-===========================================================================*/

BOOL CGUI_PlotPane::AddHiliteRegion(
	UINT uiAreaIndex,		// I - Handle / index of area.
	DATE dFirstPtXValue,	// I - X array value of first highlighted point.
	DATE dLastPtXValue,		// I - X array value of last highlighted pt.
	COLORREF Color,			// I - Color to highlight with.
	const char *szLabel)	// I - Text label; default = NULL.
{
	CPlotArea *pAreaPlot;
	UINT uiFirstArea, uiLastArea;
	BOOL bAnySuccess = FALSE;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pAreaPlot)) {
//			if (pAreaPlot->AddHiliteRegion(ulFirstPtIndex, ulLastPtIndex, Color, szLabel)) {
			if (pAreaPlot->AddHiliteRegion(dFirstPtXValue, dLastPtXValue, Color, szLabel)) {
				bAnySuccess = TRUE;
			}
		}
	}
	return(bAnySuccess);
}


/*+/csubr/TOC===============================================================
* ClearHiliteRegions - Clear memory & display of highlighted regions.
*
* Purpose:
*
* Return Value:
*	None.
*
* Notes:
*	If uiAreaIndex is GUI_PLOT_ALL_AREAS all highlighted regions in all
*	areas will be cleared (in memory and on display).
*
* See Also:
*	AddMarker, AddHiliteRegions, DeleteHiliteRegion, ShowHiliteRegions
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::ClearHiliteRegions(
	UINT uiAreaIndex)	// I - Handle / index of area.
{
	CPlotArea *pAreaPlot;
	UINT uiFirstArea, uiLastArea;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pAreaPlot)) {
			pAreaPlot->ClearHiliteRegions();
		}
	}
}

/*+/csubr/TOC===============================================================
* DeleteHiliteRegion - Clear memory & display of highlighted region.
*
* Purpose:
*
* Return Value:
*	None.
*
* Notes:
*	If uiAreaIndex is GUI_PLOT_ALL_AREAS the specified highlighted region in all
*	areas will be cleared (in memory and on display).
*
* See Also:
*	AddMarker, AddHiliteRegions, ShowHiliteRegions
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::DeleteHiliteRegion(
	UINT uiAreaIndex,	// I - Handle / index of area.
	ULONG ulFirstPtIndex,	// I - X array index of first
	                    //     highlighted point.
	ULONG ulLastPtIndex)	// I - Index of last highlighted pt.
{
	CPlotArea *pAreaPlot;
	UINT uiFirstArea, uiLastArea;
	BOOL bAnySuccess = FALSE;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pAreaPlot)) {
			if (pAreaPlot->DeleteHiliteRegion(ulFirstPtIndex, ulLastPtIndex)) {
				bAnySuccess = TRUE;
			}
		}
	}
	return(bAnySuccess);
}

/*+/csubr/TOC===============================================================
* DeleteHiliteRegion - Clear memory & display of highlighted region.
*
* Purpose:
*
* Return Value:
*	None.
*
* Notes:
*	If uiAreaIndex is GUI_PLOT_ALL_AREAS the specified highlighted region in all
*	areas will be cleared (in memory and on display).
*
* See Also:
*	AddMarker, AddHiliteRegions, ShowHiliteRegions
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::DeleteHiliteRegion(
	UINT uiAreaIndex,	// I - Handle / index of area.
	double dTargetValueX)	// I - X value within a
		                    //     highlighted region.
{   
	ULONG ulFirstPtIndex, ulLastPtIndex;
	CPlotArea *pAreaPlot;
	UINT uiFirstArea, uiLastArea;
	BOOL bAnySuccess = FALSE;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pAreaPlot)) {
			if (pAreaPlot->IsValueInHiliteRegion(dTargetValueX, &ulFirstPtIndex, &ulLastPtIndex)) {
				if (pAreaPlot->DeleteHiliteRegion(ulFirstPtIndex, ulLastPtIndex)) {
					bAnySuccess = TRUE;
				}
			}
		}
	}

	return(bAnySuccess);
}


/*+/csubr/TOC/FF===============================================================
* NextHiliteRegion - Find next hilite region.
*
* Purpose:
*	Find next hilite region & optionally move cursor.
*
* Return Value:
*	BOOL TRUE => There is a next hilite region in the current data.
*
* Notes:
*	This is a virtual function. CGUI_PlotPane only knows about the range of
*	data it has been given. The code which reads data from a database may
*	know there is more data available. If it is desirable that other data
*	be considered the user should 1) over-ride this fn 2) in that fn call
*	this base class fn and 3) if it returns FALSE use its knowledge of the 
*	larger data set to change the data which is available to CGUI_PlotPane
*	and adjust the cursor position appropriately via calling SetCursorPos.
*
*	If the view is zoomed in on the x-axis and there is a next hilite region
*	out of view, the view is scrolled as needed.
*
* See Also:
*	AddMarker, AddHiliteRegion, ClearHiliteRegions, DeleteHiliteRegion,
*	PrevHiliteRegion, ShowHiliteRegions
*
*-Date      Author     Revision
* 16-Jul-97  ToM       Created.
* 22-May-98  ToM       Added bMoveToNextRegion argument.
* 19-May-98  ToM       Added region boundary arguments.
*-===========================================================================*/

BOOL CGUI_PlotPane::NextHiliteRegion(
	BOOL bMoveToNextRegion,	// I - Actually move cursor; default=TRUE
	double *pdNextRegLowX,	// O - Low x value of next region; default = NULL
	double *pdNextRegHighX)	// O - High x value of next region; default = NULL
{
	BOOL bSuccess = FALSE;
	CPlotArea *pAreaPlot;
	double dNextRegionLowValueX, dNextRegionHighValueX;
	
	if (pdNextRegLowX == NULL) pdNextRegLowX = &dNextRegionLowValueX;
	if (pdNextRegHighX == NULL) pdNextRegHighX = &dNextRegionHighValueX;

	if (GetAreaPlot(muiCurrentArea, &pAreaPlot)) {
		if (pAreaPlot->NextHiliteRegion(mdCurrentCursX, pdNextRegLowX, pdNextRegHighX)) {
			bSuccess = TRUE;
			if (bMoveToNextRegion) {
				SetCursorPos((dNextRegionLowValueX + dNextRegionHighValueX) / 2.0);
			}
		}
	}
	return(bSuccess);
}


/*+/csubr/TOC/FF===============================================================
* PrevHiliteRegion - Find previous hilite region.
*
* Purpose:
*	Find previous hilite region & optionally move cursor.
*
* Return Value:
*	BOOL TRUE => There is a previous hilite region in the current data.
*
* Notes:
*	This is a virtual function. CGUI_PlotPane only knows about the range of
*	data it has been given. The code which reads data from a database may
*	know there is more data available. If it is desirable that other data
*	be considered the user should 1) over-ride this fn 2) in that fn call
*	this base class fn and 3) if it returns FALSE use its knowledge of the 
*	larger data set to change the data which is available to CGUI_PlotPane
*	and adjust the cursor position appropriately vai calling SetCursorPos.
*
*	If the view is zoomed in on the x-axis and there is a next hilite region
*	out of view, the view is scrolled as needed.
*
* See Also:
*	AddMarker, AddHiliteRegion, ClearHiliteRegions, DeleteHiliteRegion,
*	NextHiliteRegion, ShowHiliteRegions
*
*-Date      Author     Revision
* 16-Jul-97  ToM       Created.
* 22-May-98  ToM       Added bMoveToNextRegion argument.
* 19-May-98  ToM       Added region boundary arguments.
*-===========================================================================*/

BOOL CGUI_PlotPane::PrevHiliteRegion(
	BOOL bMoveToNextRegion,	// I - Actually move cursor; default=TRUE
	double *pdPrevRegLowX,	// O - Low x value of previous region; default = NULL;
	double *pdPrevRegHighX)	// O - High x value of previous region; default = NULL;

{
	BOOL bSuccess = FALSE;
	CPlotArea *pAreaPlot;
	double dPrevRegionLowValueX, dPrevRegionHighValueX;
	
	if (pdPrevRegLowX == NULL) pdPrevRegLowX = &dPrevRegionLowValueX;
	if (pdPrevRegHighX == NULL) pdPrevRegHighX = &dPrevRegionHighValueX;

	if (GetAreaPlot(muiCurrentArea, &pAreaPlot)) {
		if (pAreaPlot->PrevHiliteRegion(mdCurrentCursX, pdPrevRegLowX, pdPrevRegHighX)) {
			bSuccess = TRUE;
			if (bMoveToNextRegion) {
				SetCursorPos((dPrevRegionLowValueX + dPrevRegionHighValueX) / 2.0);
			}
		}
	}
	return(bSuccess);
}


/*+/csubr/TOC===============================================================
* IsCursorInHiliteRegion - Is current cursor within hilite region.
*
* Purpose:
*
* Return Value:
*	BOOL TRUE => cursor is within hilite region.
*
* Notes:
*	puiAreaIndex, pulFirstPtIndex and pulLastPtIndex only have defined values
*	if this function returns TRUE.
*
* See Also:
*	AddMarker, AddHiliteRegion, ClearHiliteRegions, DeleteHiliteRegion,
*	ShowHiliteRegions
*
*-Date      Author     Revision
* 26-Mar-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::IsCursorInHiliteRegion(
	UINT *puiAreaIndex,	// O - Index of area containing cursor.
	double *pdLowValueX,	// O - X value of first hilite pt.
							// 	in cursor-containing region
	double *pdHighValueX,	// O - X value of last hilite pt.
							// 		in cursor-containing region.
	COLORREF *pHiliteColor,	// O - Color used for this hilite region.
	CGStr *pLabel)	// O - Text label of this region; pLabel may be NULL.
	
{
	BOOL bCursorInRegion = FALSE;
	CPlotArea *pAreaPlot;
	CGStr Label;
	
	if (GetAreaPlot(muiCurrentArea, &pAreaPlot)) {
		if (pAreaPlot->IsValueInHiliteRegion(mdCurrentCursX, pdLowValueX, pdHighValueX, pHiliteColor, &Label)) {
			bCursorInRegion = TRUE;
			*puiAreaIndex = muiCurrentArea;
			if (pLabel != NULL) *pLabel = Label;
		}
	}
	return(bCursorInRegion);
}


/*+/csubr/TOC===============================================================
* ShowHiliteRegion - Show / Hide highlighted regions.
*
* Purpose:
*
* Return Value:
*	None.
*
* Notes:
*	If uiAreaIndex is GUI_PLOT_ALL_AREAS highlighted regions will be
*	hidden or shown for all of the graph areas.
*
*	If a graph is printed to a monochrome printer and the highlighted regions
*	are currently visible the highlighted regions will be printed "bolded".
*
* See Also:
*	AddMarker, AddHiliteRegion, ClearHiliteRegions, DeleteHiliteRegion,
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::ShowHiliteRegions(
	UINT uiAreaIndex,	// I - Handle / index of area.
	BOOL bVisible)		// I - FALSE => hide, TRUE => show.
{
	CPlotArea *pAreaPlot;
	UINT uiFirstArea, uiLastArea;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pAreaPlot)) {
			pAreaPlot->ShowHiliteRegions(bVisible);
		}
	}
}


/*+/csubr/TOC/FF===============================================================
* AddMarker - Label point.
*
* Purpose:
*
* Return Value:
*	BOOL - TRUE if valid uiAreaIndex and ulFirstIndex <= ulLastIndex and
*	ulLastIndex < dimension of X data array.
*
* Notes:
*	If uiAreaIndex is GUI_PLOT_ALL_AREAS a highlighted region will be
*	established for all of the graph areas.
*
*	If a graph is printed to a monochrome printer and the highlighted regions
*	are currently visible the highlighted regions will be printed "bolded".
*
*	Predefined Default colors are:
*		mGraphBkgColor = GUI_WHITE;
*		mGraphFG_Color = GUI_BLACK;
*		mCursorReadoutColor = GUI_BLUE;
*		mCursorColor = GUI_RED;	
*		mTraceColor = GUI_BLUE;
*		mLabelColor = GUI_BLUE;
*
* See Also:
*	AddHiliteRegion, ClearMarkers, DeleteMarker, IsCursorOnMarker, ShowMarkers
*
*-Date      Author     Revision
* 26-Mar-97  ToM       Created.
*-===========================================================================*/
/*
BOOL CGUI_PlotPane::AddMarker(
	UINT uiAreaIndex,	// I - Handle / index of area.
	ULONG ulMarkerIndex,	// I - data array index of marked pt.
	const char *szLabel)		// I - Text label for marker.
{
	CPlotArea *pAreaPlot;
	UINT uiFirstArea, uiLastArea;
	BOOL bAnySuccess = FALSE;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pAreaPlot)) {
			if (pAreaPlot->AddMarker(ulMarkerIndex, szLabel)) {
				bAnySuccess = TRUE;
			}
		}
	}
	return(bAnySuccess);
}
*/

BOOL CGUI_PlotPane::AddMarker(
	UINT uiAreaIndex,	// I - Handle / index of area.
	double dMarkerValueX,	// I - X value marked pt.
	const char *szLabel,	// I - Text label for marker.
	COLORREF TextColor)		// I - Color for marker text.
{
	CPlotArea *pAreaPlot;
	UINT uiFirstArea, uiLastArea;
	BOOL bAnySuccess = FALSE;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pAreaPlot)) {
			if (pAreaPlot->AddMarker(dMarkerValueX, szLabel, TextColor)) {
				bAnySuccess = TRUE;
			}
		}
	}
	return(bAnySuccess);

}

/*+/csubr/TOC===============================================================
* ClearMarkers - Clear memory & display of all marked points.
*
* Purpose:
*
* Return Value:
*	None.
*
* Notes:
*	If uiAreaIndex is GUI_PLOT_ALL_AREAS all markers in all
*	areas will be cleared (in memory and on display).
*
* See Also:
*	AddHiliteRegion, AddMarker, DeleteMarker, IsCursorOnMarker, ShowMarkers
*
*-Date      Author     Revision
* 26-Mar-97  ToM       Created.
*-===========================================================================*/


void CGUI_PlotPane::ClearMarkers(
	UINT uiAreaIndex)	// I - Handle / index of area.
{
	CPlotArea *pAreaPlot;
	UINT uiFirstArea, uiLastArea;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pAreaPlot)) {
			pAreaPlot->ClearMarkers();
		}
	}
}

/*+/csubr/TOC===============================================================
* DeleteMarker - Clear memory & display of marked point.
*
* Purpose:
*
* Return Value:
*	None.
*
* Notes:
*	If uiAreaIndex is GUI_PLOT_ALL_AREAS all markers in all
*	areas will be deleted (in memory and on display).
*
* See Also:
*	AddHiliteRegion, AddMarker, ClearMarkers, IsCursorOnMarker, ShowMarkers
*
*-Date      Author     Revision
* 26-Mar-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::DeleteMarker(
	UINT uiAreaIndex,	// I - Handle / index of area.
	double dMarkerValueX)	// I - X value marked pt.
{
	CPlotArea *pAreaPlot;
	UINT uiFirstArea, uiLastArea;
	BOOL bAnySuccess = FALSE;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pAreaPlot)) {
			if (pAreaPlot->DeleteMarker(dMarkerValueX)) {
				bAnySuccess = TRUE;
			}
		}
	}
	return(bAnySuccess);
}

/*+/csubr/TOC===============================================================
* IsCursorOnMarker - Is current cursor on marker location.
*
* Purpose:
*
* Return Value:
*	BOOL TRUE => cursor is on marker.
*
* Notes:
*
* See Also:
*	AddHiliteRegion, AddMarker, ClearMarkers, DeleteMarker, ShowMarkers
*
*-Date      Author     Revision
* 26-Mar-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::IsCursorOnMarker(
	UINT *puiAreaIndex,	// O - Index of area containing cursor.
	double *pdMarkerValueX,	// O - X value of marked pt.
	COLORREF *pMarkerTextColor)	// O - Color used for marker text.
{
	BOOL bCursorOnMarker = FALSE;
	CPlotArea *pAreaPlot;
	
	if (GetAreaPlot(muiCurrentArea, &pAreaPlot)) {
		if (pAreaPlot->IsValueOnMarker(mdCurrentCursX, pMarkerTextColor)) {
			bCursorOnMarker = TRUE;
		}
		if (pdMarkerValueX != NULL) *pdMarkerValueX = mdCurrentCursX;
		if (puiAreaIndex != NULL) *puiAreaIndex = muiCurrentArea;
	}
	else {
		double dMinFullX, dMaxFullX, dMinVisibleX, dMaxVisibleX;

		GetLimitsX(&dMinFullX, &dMaxFullX, &dMinVisibleX, &dMaxVisibleX);
		if (pdMarkerValueX != NULL) *pdMarkerValueX = (dMinVisibleX + dMaxVisibleX) / 2.0;
		if (puiAreaIndex != NULL) *puiAreaIndex = 0;
	}
	
	return(bCursorOnMarker);
}

/*+/csubr/TOC===============================================================
* ShowMarkers - Show / Hide marked points.
*
* Purpose:
*
* Return Value:
*	None.
*
* Notes:
*	If uiAreaIndex is GUI_PLOT_ALL_AREAS marked points will be
*	hidden or shown for all of the graph areas.
*
* See Also:
*	AddHiliteRegion, AddMarker, ClearMarkers, DeleteMarker, IsCursorOnMarker
*
*-Date      Author     Revision
* 26-Mar-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::ShowMarkers(
	UINT uiAreaIndex,	// I - Handle / index of area.
	BOOL bVisible)		// I - FALSE => hide, TRUE => show.
{
	CPlotArea *pAreaPlot;
	UINT uiFirstArea, uiLastArea;

	GetFirstAndLastAreaIndex(uiAreaIndex, &uiFirstArea, &uiLastArea);

	for (UINT ui = uiFirstArea; ui <= uiLastArea; ui++) {	
		if (GetAreaPlot(ui, &pAreaPlot)) {
			pAreaPlot->ShowMarkers(bVisible);
		}
	}
}


/*+/csubr/TOC/FF===============================================================
* SetUserAnnotation - Turn off/on user's custom annotation.
*
* Purpose:
*	For example, the data along the X axis may be seconds past some Time Zero.
*	In this case the number is probably not very useful to the end-user so
*	the application probably wants to intercept requests to label the X-axis
*	and substitute a date and/or time string to be displayed.
*
* Return Value:
*	BOOL - TRUE => successful.
*
* Notes:
*
* See Also:
*	virtual functions: GetAnnotationX, GetAnnotationY
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::SetUserAnnotation(
	BOOL bUserAnnotX,
	BOOL bUserAnnotY)
{
	BOOL bSuccess;
	int iStatus;

	if ((mbUserAnnotX != bUserAnnotX) || (mbUserAnnotY = bUserAnnotY)) {
		InvalidateGraphDisplay();	// Schedule a repaint.
	}
	
	mbUserAnnotX = bUserAnnotX;
	mbUserAnnotY = bUserAnnotY;

	UINT ui = 0;
	do {
		iStatus = PlAreaArraySelect(&mPlCtx, ui, 0);	// Column 0 (last argument).

		if (mbUserAnnotY) {
			iStatus = PlAreaRegYAnnotCallback(&mPlCtx,
				&CGUI_PlotPane::PlotLibY_AxisAnnotCallBackFn, this);
		}
		else {
			iStatus = PlAreaRegYAnnotCallback(&mPlCtx, NULL, this);
		}
	
		if (iStatus == PL_OK) {
			if (mbUserAnnotX) {
				iStatus = PlAreaRegXAnnotCallback(&mPlCtx,
					&CGUI_PlotPane::PlotLibX_AxisAnnotCallBackFn, this);
			}
			else {
				iStatus = PlAreaRegXAnnotCallback(&mPlCtx, NULL, this);
			}
		}                   
		ui++;
	} while ((ui < muiNumAreas) && (iStatus == PL_OK));

	if (iStatus == PL_OK) {
		bSuccess = TRUE;
	}
	else {
		ShowPlotLibErrMsg(iStatus);
	}
	
	return(bSuccess);
}


// User's data may be such that it does not translate directly into just
// formatting a numerical value. For example, the X-axis value may be a date
// in seconds since some Zero Date and the string value of a numeric date
// will be "Mon DD, 19YY". If so the code to do that goes in here and/or in
// GetY_Annotation. By default these two routines will return the formatted
// numerical value.

/*+/csubr/TOC===============================================================
* GetAnnotationX - Virtual user fn to translate X annotation.
*
* Purpose:
*	For example, the data along the X axis may be seconds past some Time Zero.
*	In this case the number is probably not very useful to the end-user so
*	the application probably wants to intercept requests to label the X-axis
*	and substitute a date and/or time string to be displayed.
*
* Return Value:
*	None.
*
* Notes:
*
* See Also:
*	SetUserAnnotation, virtual function GetAnnotationY
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::GetAnnotationX(
	double dValue,	// I - Value to be "formatted".
	CGStr *pAnnotationStr,	// O - String to be displayed.
	BOOL bForCursorValue)	// I - TRUE => for cursor readout
	                        //     FALSE => for axis annotation.
{
	FmtDoubleValue(dValue, pAnnotationStr);
}


/*+/csubr/TOC===============================================================
* GetAnnotationX - Virtual user fn to translate X annotation.
*
* Purpose:
*	For example, because of space limitations, it may be desirable to format
*	the annotation differently depending on whether the string is to be
*	displayed along the Y-axis or as part of the cursor location readout.
*
* Return Value:
*	None.
*
* Notes:
*
* See Also:
*	SetUserAnnotation, virtual function GetAnnotationX
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::GetAnnotationY(double dValue, CGStr *pAnnotationStr, BOOL bForCursorValue)
{
	FmtDoubleValue(dValue, pAnnotationStr);
}


/*+/csubr/TOC/FF===============================================================
* RegisterRightCtrls - Glue dialog controls to right edge of dialog box.
*
* Purpose:
*	If the application programmer declares the dialog box containing the
*	CGUI_PlotPane to be resizeable s/he needs to declare each individual
*	contol is to be repositioned when the dialog box size changes. The plot
*	window pane will automatically stay anchored in the upper left corner of
*	dialog box. The other controls in the dialog box can be "attacehd" to the
*	right or bottom edges or to the right bottom corner of the dailog box.
*	"Attached" means the control will be kept the same relative distance from
*	that edge.
*
* Return Value:
*	BOOL - TRUE => successful.
*
* Notes:
*	This is a convenience function. The same effect can be obtained by calling
*	CGUI_Dlg::SetCtrlResizeBehavior for each control.
*
*	Note that there are equivalent functions: RegisterBottomCtrls and
*	RegisterBottomRightCtrls .
*
* See Also:
*	Over-loaded version of RegisterRightCtrls
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::RegisterRightCtrls(
	RESOURCE_ID *naCtrlID,	// I - Null-terminated array of ctrl ID's.
	BOOL bClearPrevList)	// I - FALSE => add new ctrls to previous
							//     list of right controls.
{
	const UINT uiMAX_CTRLS = 50;	// for sanity check wrt Null-termination of naCtrlID.
	UINT uiNumCtrls = 0;
	BOOL bSuccess = FALSE;
	
	while ((naCtrlID[uiNumCtrls] != NULL) && (uiNumCtrls < uiMAX_CTRLS)) {
		uiNumCtrls++;
	}

	if (uiNumCtrls < uiMAX_CTRLS) {	
		bSuccess = RegisterRightCtrls(naCtrlID, uiNumCtrls, bClearPrevList);
	}
	return(bSuccess);
}


/*+/csubr/TOC===============================================================
* RegisterRightCtrls - Glue dialog controls to right edge of dialog box.
*
* Purpose:
*	If the application programmer declares the dialog box containing the
*	CGUI_PlotPane to be resizeable s/he needs to declare each individual
*	contol is to be repositioned when the dialog box size changes. The plot
*	window pane will automatically stay anchored in the upper left corner of
*	dialog box. The other controls in the dialog box can be "attacehd" to the
*	right or bottom edges or to the right bottom corner of the dailog box.
*	"Attached" means the control will be kept the same relative distance from
*	that edge.
*
* Return Value:
*	BOOL - TRUE => successful.
*
* Notes:
*	This is a convenience function. The same effect can be obtained by calling
*	CGUI_Dlg::SetCtrlResizeBehavior for each control.
*
*	Note that there are equivalent functions: RegisterBottomCtrls and
*	RegisterBottomRightCtrls .
*
* See Also:
*	Over-loaded version of RegisterRightCtrls
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::RegisterRightCtrls(
	RESOURCE_ID *aCtrlID,	// I - array of ctrl ID's.
	UINT uiNumCtrls,		// I - number of ctrls in aCtrlID.
	BOOL bClearPrevList)	// I - DEFAULT: FALSE => add new ctrls
							//     to previous list of right controls.
{
	BOOL bSuccess = FALSE;
	UINT uiNewCtrlsBaseIndex = 0;
	
	if (bClearPrevList) {
		delete [] mnaRightCtrlID;
		mnaRightCtrlID = new RESOURCE_ID[uiNumCtrls + 1];	// +1 for terminating NULL.
	}
	else {
		if (mnaRightCtrlID != NULL) {
			while (mnaRightCtrlID[uiNewCtrlsBaseIndex] != NULL) {
				uiNewCtrlsBaseIndex++;
			}
		}
		
		// Allocate a new larger array big enough for prev & new ctrls and a NULL at the end.
		RESOURCE_ID *naCtrlID;
		naCtrlID = new RESOURCE_ID[uiNumCtrls + uiNewCtrlsBaseIndex + 1];	// +1 for terminating NULL.
		if (naCtrlID != NULL) { 
			for (UINT ui = 0; ui < uiNewCtrlsBaseIndex; ui++) {
				naCtrlID[ui] = mnaRightCtrlID[ui];
			}
		}
		delete [] mnaRightCtrlID;
		mnaRightCtrlID = naCtrlID;
	}

	if (mnaRightCtrlID != NULL) {
		bSuccess = TRUE;
		for (UINT ui = 0; ui < uiNumCtrls; ui++) {
			mnaRightCtrlID[ui + uiNewCtrlsBaseIndex] = aCtrlID[ui];
		}
		mnaRightCtrlID[uiNumCtrls + uiNewCtrlsBaseIndex] = NULL;
	}
	return(bSuccess);
}


BOOL CGUI_PlotPane::RegisterBottomCtrls(RESOURCE_ID *naCtrlID, BOOL bClearPrevList)
{
	const UINT uiMAX_CTRLS = 50;	// for sanity check wrt Null-termination of naCtrlID.
	UINT uiNumCtrls = 0;
	BOOL bSuccess = FALSE;

	while ((naCtrlID[uiNumCtrls] != NULL) && (uiNumCtrls < uiMAX_CTRLS)) {
		uiNumCtrls++;
	}

	if (uiNumCtrls < uiMAX_CTRLS) {
		bSuccess = RegisterBottomCtrls(naCtrlID, uiNumCtrls, bClearPrevList);
	}
	return(bSuccess);
}


BOOL CGUI_PlotPane::RegisterBottomCtrls(RESOURCE_ID *aCtrlID, UINT uiNumCtrls, BOOL bClearPrevList)
{
	BOOL bSuccess = FALSE;
	UINT uiNewCtrlsBaseIndex = 0;
	
	if (bClearPrevList) {
		delete [] mnaBottomCtrlID;
		mnaBottomCtrlID = new RESOURCE_ID[uiNumCtrls + 1];	// +1 for terminating NULL.
	}
	else {
		if (mnaBottomCtrlID != NULL) {
			while (mnaBottomCtrlID[uiNewCtrlsBaseIndex] != NULL) {
				uiNewCtrlsBaseIndex++;
			}
		}
		
		// Allocate a new larger array big enough for prev & new ctrls and a NULL at the end.
		RESOURCE_ID *naCtrlID;
		naCtrlID = new RESOURCE_ID[uiNumCtrls + uiNewCtrlsBaseIndex + 1];	// +1 for terminating NULL.
		if (naCtrlID != NULL) { 
			for (UINT ui = 0; ui < uiNewCtrlsBaseIndex; ui++) {
				naCtrlID[ui] = mnaBottomCtrlID[ui];
			}
		}
		delete [] mnaBottomCtrlID;
		mnaBottomCtrlID = naCtrlID;
	}

	if (mnaBottomCtrlID != NULL) {
		bSuccess = TRUE;
		for (UINT ui = 0; ui < uiNumCtrls; ui++) {
			mnaBottomCtrlID[ui + uiNewCtrlsBaseIndex] = aCtrlID[ui];
		}
		mnaBottomCtrlID[uiNumCtrls + uiNewCtrlsBaseIndex] = NULL;
	}
	return(bSuccess);
}

BOOL CGUI_PlotPane::RegisterBottomRightCtrls(RESOURCE_ID *naCtrlID, BOOL bClearPrevList)
{
	const UINT uiMAX_CTRLS = 50;	// for sanity check wrt Null-termination of naCtrlID.
	UINT uiNumCtrls = 0;
	BOOL bSuccess = FALSE;

	while ((naCtrlID[uiNumCtrls] != NULL) && (uiNumCtrls < uiMAX_CTRLS)) {
		uiNumCtrls++;
	}

	if (uiNumCtrls < uiMAX_CTRLS) {	
		bSuccess = RegisterBottomRightCtrls(naCtrlID, uiNumCtrls, bClearPrevList);
	}

	return(bSuccess);
}


BOOL CGUI_PlotPane::RegisterBottomRightCtrls(RESOURCE_ID *aCtrlID, UINT uiNumCtrls, BOOL bClearPrevList)
{
	BOOL bSuccess = FALSE;
	UINT uiNewCtrlsBaseIndex = 0;
	
	if (bClearPrevList) {
		delete mnaBottomRightCtrlID;
		mnaBottomRightCtrlID = new RESOURCE_ID[uiNumCtrls + 1];	// +1 for terminating NULL.
	}
	else {
		if (mnaBottomRightCtrlID != NULL) {
			while (mnaBottomRightCtrlID[uiNewCtrlsBaseIndex] != NULL) {
				uiNewCtrlsBaseIndex++;
			}
		}
		
		// Allocate a new larger array big enough for prev & new ctrls and a NULL at the end.
		RESOURCE_ID *naCtrlID;
		naCtrlID = new RESOURCE_ID[uiNumCtrls + uiNewCtrlsBaseIndex + 1];	// +1 for terminating NULL.
		if (naCtrlID != NULL) { 
			for (UINT ui = 0; ui < uiNewCtrlsBaseIndex; ui++) {
				naCtrlID[ui] = mnaBottomRightCtrlID[ui];
			}
		}
		delete mnaBottomRightCtrlID;
		mnaBottomRightCtrlID = naCtrlID;
	}

	if (mnaBottomRightCtrlID != NULL) {
		bSuccess = TRUE;
		for (UINT ui = 0; ui < uiNumCtrls; ui++) {
			mnaBottomRightCtrlID[ui + uiNewCtrlsBaseIndex] = aCtrlID[ui];
		}
		mnaBottomRightCtrlID[uiNumCtrls + uiNewCtrlsBaseIndex] = NULL;
	}
	return(bSuccess);
}


/*+/csubr/TOC/FF===============================================================
* Print - Render CGUI_PlotPane onto the printer.
*
* Purpose:
*
* Return Value:
*	None.
*
* Notes:
*
* See Also:
*	ShowHiliteRegions.
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::Print(
	const char *szHeader,	// I - Header text for printout.
	const char *szFooter)	// I - Footer text for printout.
{                            
	// Define margins for plot; i.e., without header and footer info.
	const float fLEFT_PLOT_MARGIN = 1.0F;		// In inches.
	const float fRIGHT_PLOT_MARGIN = 1.0F;
	const float fBOTTOM_PLOT_MARGIN = 1.0F;	// sfk changed bottom margine from 1.4 to 1.0
	const float fBORDER_PLOT_PTS = 3.0F;
	// Define margins for entire page, including header and footer.
	const float fLEFT_PAGE_MARGIN = 1.0F;
	const float fRIGHT_PAGE_MARGIN = 1.0F;
	const float fTOP_PAGE_MARGIN = 1.0F;
	const float fBOTTOM_PAGE_MARGIN = 0.6F; // sfk changed bottom margin to from 1.0 to 0.6
	const float fBORDER_PAGE_PTS = 0.0F;

	PL_CTX PlPrtCtx;
	float fTopPlotMargin = 1.0F;
	BOOL bSuccess = FALSE;
	BOOL bAbort = FALSE;
	BOOL bHeaderFooterBold = FALSE;
	BOOL bHeaderFooterTransparent = TRUE;
	float fHeaderFooterRotation = 0.0F;
	COLORREF HeaderFooterColor = BLACK;
	float fHeaderFooterFontSize = 13.0F;	// sfk reduced size from 15 to 13
	float fHeaderFooterOffset = 0.0F;
	    
	if (szHeader == NULL) {
		fTopPlotMargin = 1.0F;	// sfk removed float declaration since defined above
	}
	else {
		fTopPlotMargin = 1.4F;	// sfk removed float declaration since defined above
	}
	mbPrinting = TRUE;
	mpPlPrtCtx = &PlPrtCtx;		// sfk set a pointer that the callback routines can find 2.07

	GUI_SetCursorShape(GUI_CURSOR_HOURGLASS);

	int iStatus = PlPrtOpenDefault(&PlPrtCtx, "GUI Lib Graph Plot", "");
	if (iStatus == PL_OK) {
		iStatus = PlPlotInit_prt(&PlPrtCtx, &mPlCtx, fBORDER_PLOT_PTS,
			fLEFT_PLOT_MARGIN, fRIGHT_PLOT_MARGIN, fTopPlotMargin, fBOTTOM_PLOT_MARGIN);
		if (iStatus == PL_OK) {
			iStatus = PlWinScale(&PlPrtCtx);
			if (iStatus == PL_OK) {
				RefreshDisplay(&PlPrtCtx);
				// Wrapup after drawing the plot, and reinitialize with page margins
				// and with no screen plot context; i.e. NULL vs. &mPlCtx.
				iStatus = PlPlotWrapup(&PlPrtCtx);
				if (iStatus == PL_OK) {
					iStatus = PlPlotInit_prt(&PlPrtCtx, NULL, fBORDER_PAGE_PTS,
						fLEFT_PAGE_MARGIN, fRIGHT_PAGE_MARGIN, fTOP_PAGE_MARGIN, fBOTTOM_PAGE_MARGIN);
					if (iStatus == PL_OK) {
						float fX, fY;

						if (szHeader != NULL) {
							fX = 5.0F;
							fY = 10.0F;
							PlDrawText(&PlPrtCtx, fX, fY, szHeader, PRT_TC, fHeaderFooterOffset,
								fHeaderFooterFontSize, CGUI_PlotPane::GetTypeFace(), bHeaderFooterBold,
								bHeaderFooterTransparent, fHeaderFooterRotation, HeaderFooterColor);
						}

						fX = 5.0F;
						fY = 0.0F;
						if (szFooter != NULL) {	// add print of user defined footer and make smaller than header
							PlDrawText(&PlPrtCtx, fX, fY, szFooter, PRT_BC, fHeaderFooterOffset,
								fHeaderFooterFontSize-2, CGUI_PlotPane::GetTypeFace(), bHeaderFooterBold,
								bHeaderFooterTransparent, fHeaderFooterRotation, HeaderFooterColor);
						} else {
	
							CGFmtStr Footer, CurrentTimeStr, CurrentDateStr;
							// Get current date and time strings.
							CFmtdDate Date;	// Default set is current date.
							Date.Get(&CurrentDateStr);
							CFmtdTime Time;	// Default set is current time.
							Time.Get(&CurrentTimeStr);                                                           

							Footer.Printf("Printed at %s on %s",
								CurrentTimeStr.Get_sz(), CurrentDateStr.Get_sz());

							
							PlDrawText(&PlPrtCtx, fX, fY, Footer.Get_sz(), PRT_BC, fHeaderFooterOffset,
								fHeaderFooterFontSize, CGUI_PlotPane::GetTypeFace(), bHeaderFooterBold,
								bHeaderFooterTransparent, fHeaderFooterRotation, HeaderFooterColor);
						}

						iStatus = PlPrtClose(&PlPrtCtx, bAbort);
						if (iStatus == PL_OK) {
							bSuccess = TRUE;
						}
					}
				}
			}
		}
	}

	GUI_SetCursorShape(GUI_CURSOR_ARROW);
	mbPrinting = FALSE;
	CGUI_App::ProcessEvents();	// Allow system to update display.
	if (!bSuccess) {
		CGFmtStr Msg;	// added error number in message sfk
		Msg.Printf("Error printing graph. (%d)", iStatus);
		GUI_MsgBox(Msg);
	}
	SetFocus();
}


/*+/csubr/TOC===============================================================
* SuppressUpdate - Suppress drawing of graph data.
*
* Purpose:
*	Either the data may be invalid or there is a lot of data updating (for
*	more than one area) going on and we want to minimize screen flashing
*	and redraw time.
*
* Return Value:
*	BOOL - previous value of update suppression.
*
* Notes:
*	UpdateIsSuppressed() will retrieve the current value of update suppression.
*
*	InvalidateGraphDisplay is automatically called when updae supression is
*	changed.
*
* See Also:
*	InvalidateGraphDisplay.
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::SuppressUpdate(
	BOOL bSuppressUpdate)	// I - TRUE => suppress drawing plot data.
{	
	BOOL bUpdateWasSuppressed = mbSuppressUpdate;
	mbSuppressUpdate = bSuppressUpdate;
	UpdateDisplay(&mPlCtx);

	return(bUpdateWasSuppressed);
}


/*+/csubr/TOC===============================================================
* SetFocus - Move input focus to graph.
*
* Purpose:
*	When the user pushes a button in the dialog box containing the graph
*	display the input focus will move to that button. Often it is desireable
*	as a convenience to the user to automatically move the input focus back
*	to the graph after servicing the button hit.
*
* Return Value:
*	None.
*
* Notes:
*
* See Also:
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::SetFocus()
{
	mpDlg->SetFocus(mPaneID);
}
	

/*+/csubr/TOC===============================================================
* InvalidateGraphDisplay - Schedule repaint of graph.
*
* Purpose:
*	Schedules a repaint of the entire graph.
*
* Return Value:
*	None.
*
* Notes:
*	This only schedules a repaint it does not force it to occur immediately.
*
* See Also:
*	SuppressUpdate.
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::InvalidateGraphDisplay()
{
	PlWinRepaint(&mPlCtx);
}


/*+/csubr/TOC/FF===============================================================
* ShowPlotLibErrMsg - Display plot library error message.
*
* Purpose:
*	If any plot library call results in a status return other than PL_OK,
*	a CGUI_TextWin is created (on the first such message) and the text of
*	the error return is logged to that window. When the CGUI_PlotPane is
*	destroyed, if there is an plot library error log window the user is
*	notified via a GUI_Msgbox that the window is about to be destroyed and
*	an look at the contents before proceeding.
*
* Return Value:
*	None.
*
* Notes:
*	This is a virtual function and can be over-ridden if the application
*	programmer wants to handle plot library error messages in a differnet
*	manner. If it is over-ridden the over-ride needs to be aware that it
*	will often be called with a iStatus = PL_OK and will probably want
*	to skip processing these messages.
*
*	If an unattended application uses CGUI_PlotPane there can be a problem
*	with the default handling of plot library error messages. That is if
*	there are any messages during the plotting and then the CGUI_PLotPane
*	is destroyed the program will stall waiting for an operator to acknowledge
*	the message informing him about the presence of the error message window.
*	In this case the application programmer can either intercept the plot
*	library messages so that no default plot library error message log window
*	is created or s/he can call DeletePlotLibErrMsgWin immediately before
*	destroying the CGUI_PlotPane.
*
* See Also:
*	DeletePlotLibErrMsgWin, Plot library documentation.
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::ShowPlotLibErrMsg(
	int iStatus)	// I - Plot library status return.
{
	if (iStatus != PL_OK) {
		if (mpPlotLibMsgWin == NULL) {
//			mpPlotLibMsgWin = new CGUI_TextWin("Plot Library Error Log");
		}
		
		if (mpPlotLibMsgWin != NULL) {
			const char *szMsg = PlErrorMessage(&mPlCtx, iStatus);
			mpPlotLibMsgWin->Printf("%s\n", szMsg);
		}
	}
}


void CGUI_PlotPane::DeletePlotLibErrMsgWin()
{
	if (mpPlotLibMsgWin != NULL) {
		delete mpPlotLibMsgWin;
		mpPlotLibMsgWin = NULL;
	}
}


/*+/csubr/TOC/FF===============================================================
* GetPlotContext - Get current plot library context (PL_CTX).
*
* Purpose:
*	Return a pointer to the plot library PL_CTX structure so the application
*	programmer can fine tune the behavior of CGUI_PlotPane. This same
*	information can be obtained via the "protected" member variable mPlCtx.
*	See Note below.
*
* Return Value:
*	Pointer to plot library PL_CTX structure.
*
* Notes:
*	This leaves the plot library and CGUI_PlotPane class open to abuse. This
*	should only be used by someone experienced with using the plot library.
*
* See Also:
*	GetParentDlg, Plot library documentation.
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

PL_CTX *CGUI_PlotPane::GetPlotContext()
{
	if (mbPrinting) return(mpPlPrtCtx);	// sfk get the printing context if necessary 2.07
	return(&mPlCtx);
};


/*+/csubr/TOC===============================================================
* GetParentDlg - Get pointer to CGUI_Dlg containing graph.
*
* Purpose:
*	Return a pointer to the CGUI_Dlg containg the plot pane. This is for
*	convenience when the application is in a CGUI_PlotPane service fn and
*	needs to do something with respect to the parent dailog.
*
* Return Value:
*	Pointer to the CGUI_Dlg containing the CGUI_PlotPane.
*
* Notes:
*
* See Also:
*	GetPlotContext, CGUI_Dlg documentation.
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

CGUI_Dlg *CGUI_PlotPane::GetParentDlg()
{
	return(mpDlg);
};


/*+/csubr/TOC/FF===============================================================
* GetNumAreas - Get current number of plot areas.
*
* Purpose:
*
* Return Value:
*	Current number of plot areas.
*
* Notes:
*
* See Also:
*	GetCurrentArea, SetCurrentArea.
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

UINT CGUI_PlotPane::GetNumAreas()
{
	return(muiNumAreas);
};


/*+/csubr/TOC===============================================================
* GetCurrentArea - Get current plot area.
*
* Purpose:
*
* Return Value:
*	Index / handle of current plot area. Current plot area is the one containing
*	the cursor.
*
* Notes:
*
* See Also:
*	GetNumAreas, SetCurrentArea, GetCursorPos, SetCursorPos.
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

UINT CGUI_PlotPane::GetCurrentArea()
{
	int iRow = mPlCtx.iAxisRow;
	//int iCol = mPlCtx.iAxisCol;	// Only one column.
	// It turns out that what teh plot lib thinks is the current area can get out of sync
	// with muiCurrentArea so get current area directly from plot library to return to
	// caller and take this opportunity to update muiCurrentArea.
	muiCurrentArea = (UINT)iRow;
	return(muiCurrentArea);
};


/*+/csubr/TOC===============================================================
* SetCurrentArea - Set current plot area.
*
* Purpose:
*	Move the plot cursor into the specified plot area.
*
* Return Value:
*	BOOL - TRUE => valid area index input.
*
* Notes:
*
* See Also:
*	GetNumAreas, GetCurrentArea, GetCursorPos, SetCursorPos.
*
*-Date      Author     Revision
* 23-Jan-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::SetCurrentArea(
	UINT uiAreaIndex)	// I - Handle (0-based index) of area.
{
	BOOL bSuccess;
	CPlotArea *pPlotArea;

	int iStatus = PlAreaArraySelect(&mPlCtx, uiAreaIndex, 0);
	if (iStatus == PL_OK) {
		bSuccess = TRUE;
		muiCurrentArea = uiAreaIndex;
		if (GetAreaPlot(uiAreaIndex, &pPlotArea)) {
			pPlotArea->SetIndexGivenX(mdCurrentCursX, &mulCursorIndex);
			//pPlotArea->FindIndexGivenX(mdCurrentCursX, &mulCursorIndex);
		}
	}
	else bSuccess = FALSE;
	return(bSuccess);
}


/*+/csubr/TOC===============================================================
* CursorReadoutText - Virtual, Create/modify cursor readout text.
*
* Purpose:
*	This function is called just before the current cursor location readout
*	text at the bottom of the plot is displayed. By intecepting this call the
*	text that would normally be displayed can be modified or new text created.
*
* Return Value:
*	BOOL bTextAlreadyDisplayed = TRUE, cursor readout text has already been displayed.
		*						= FALSE, base class library should display text.
*
* Notes:
*
* See Also:
*	Plot library documentation.
*
*-Date      Author     Revision
* 24-Mar-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::DisplayCursorReadoutText(
	PL_CTX *pPlCtx,	// I - plot context.
	const CGStr &AreaName,	// I - Name of current plot area.
	UINT uiCursorArea,	// I - Index of current plot area.
	double dCursorX,	// I - value of current cursor X.
	double dCursorY,	// I - value of current cursor Y.
	BOOL bInterpolatedY,	// I - TRUE if cursor not on data pt.
	BOOL bCursorInRange,	// I - TRUE if cursor is within X range.
	CGFmtStr *pCursorReadoutText,	// IO - Default text (can be changed).
	COLORREF *pmCursorReadoutColor)	// IO - Default text color (can be changed).
{
	BOOL bTextAlreadyDisplayed = FALSE;
	
	return(bTextAlreadyDisplayed);
}


/*+/csubr/TOC/FF===============================================================
* ResizeNotifyFn - Virtual fn called when dialog box is resized.
*
* Purpose:
*	The default ResizeNotifyFn assumes that the plot pane is in the upper-left
*	portion of the dialog box and will grow or shrink one pixel in height and
*	width for each pixel change in the size of the dialog box. The plot pane
*	stay fixed in postion relative to the top and left edges of the dialog box.
*	All of the controls in the dialog box (except the "group box" control that
*	defines the real estate for the plot pane) will move according to how each
*	one was registered with the Register...Ctrls functions in CGUI_PlotPane.
*
* Return Value:
*	None.
*
* Notes:
*
* See Also:
*
*-Date      Author     Revision
* 24-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::ResizeNotifyFn(
	CGUI_Dlg *pDlg,		// I - dialog box being resized.
	int iDeltaDlgWidth,	// I - Change in dlg. width (pixels).
	int iDeltaDlgHeight)	// I - Change in dlg. height (pixels).
{
	UINT uiOldWidth, uiOldHeight;
	UINT uiNewHeight, uiNewWidth;
	UINT uiOldLeft, uiOldTop;
	UINT uiNewLeft, uiNewTop;
	UINT uiIndex;
	
	if (pDlg->GetCtrlSize(mPaneID, &uiOldWidth, &uiOldHeight)) {
		// Reposition ctrls glued to the right edge of the dialog box.
		if (mnaRightCtrlID != NULL) {
			uiIndex = 0;
			while (mnaRightCtrlID[uiIndex] != NULL) {
				if (pDlg->GetCtrlPosition(mnaRightCtrlID[uiIndex], &uiOldLeft, &uiOldTop)) {
					uiNewTop = uiOldTop;
					uiNewLeft = uiOldLeft + iDeltaDlgWidth;
					pDlg->SetCtrlPosition(mnaRightCtrlID[uiIndex], uiNewLeft, uiNewTop);
				}
				uiIndex++;
			}
		}

		// Reposition ctrls glued to the bottom edge of the dialog box.
		if (mnaBottomCtrlID != NULL) {
			uiIndex = 0;
			while (mnaBottomCtrlID[uiIndex] != NULL) {
				if (pDlg->GetCtrlPosition(mnaBottomCtrlID[uiIndex], &uiOldLeft, &uiOldTop)) {
					uiNewTop = uiOldTop + iDeltaDlgHeight;
					uiNewLeft = uiOldLeft;
					pDlg->SetCtrlPosition(mnaBottomCtrlID[uiIndex], uiNewLeft, uiNewTop);
				}
				uiIndex++;
			}
		}

		// Reposition ctrls glued to the right and bottom edges of the dialog box.
		if (mnaBottomRightCtrlID != NULL) {
			uiIndex = 0;
			while (mnaBottomRightCtrlID[uiIndex] != NULL) {
				if (pDlg->GetCtrlPosition(mnaBottomRightCtrlID[uiIndex], &uiOldLeft, &uiOldTop)) {
					uiNewTop = uiOldTop + iDeltaDlgHeight;
					uiNewLeft = uiOldLeft + iDeltaDlgWidth;
					pDlg->SetCtrlPosition(mnaBottomRightCtrlID[uiIndex], uiNewLeft, uiNewTop);
				}
				uiIndex++;
			}
		}

		// Resize plot area.
		uiNewHeight = uiOldHeight + iDeltaDlgHeight;
		uiNewWidth = uiOldWidth + iDeltaDlgWidth;
		pDlg->SetCtrlSize(mPaneID, uiNewWidth, uiNewHeight);

		// Let plot library know that pane has been resized.
		zRect DspRect;
		if (pDlg->GetRect(mPaneID, &DspRect)) {
			int iLeft = DspRect.left();
			int iTop = DspRect.top();
			int iWidth = DspRect.width();
			int iHeight = DspRect.height();
			int iStatus = PlWinMove(&mPlCtx, iLeft, iTop, iWidth, iHeight);
			InvalidateGraphDisplay();
		}

		// Invalidate entire dialog window so that background
		// around the controls and the controls get repainted.
		HWND hWnd = pDlg->GetWndHandle();
		if (hWnd != NULL) {
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
}


/*+/csubr/TOC===============================================================
* ServiceRightMouseBtn - Virtual fn called when right mouse btn clicked.
*
* Purpose:
*	The default ServiceRightMouseBtn puts up a GUI_MsgBox stating that the
*	mouse button was clicked and states which area and the coordinates within
*	that area of the right mouse button click.
*
* Return Value:
*	BOOL bFullyServiced = TRUE => do not allow plot lib to further process event.
*
* Notes:
*	Generally the user's derived class would want to over-ride this function
*	to at least shut off the default message and perhasp do something with the
*	event.
*
* See Also:
*
*-Date      Author     Revision
* 24-Jan-97  ToM       Created.
*-===========================================================================*/

BOOL CGUI_PlotPane::ServiceRightMouseBtn(
	UINT uiAreaIndex,	// I - Handle (0-based index) of area.
	double dXValue,		// I - X value of click point.
	double dYValue)		// I - Y value of clcik point.
{
	CGFmtStr Msg;
	BOOL bFullyServiced = FALSE;
	
	Msg.Printf("Right mouse click on Area %u. (x,y)=(%.1f,%.1f)",
		uiAreaIndex, dXValue, dYValue);
	GUI_MsgBox(Msg);
	return(bFullyServiced);
}


/*+/csubr/TOC===============================================================
* UpdateDisplay - Virtual fn called as result of PlWinRegPlotCallback.
*
* Purpose:
*	CGUI_PlotPane registers a function to be called by the plot library
*	whenever the plot needs to be repainted. That fn must be a static member
*	of CGUI_PlotPane; however that static fn simply calls the (non-static)
*	member fn UpdateDisplay and passes along the PL_CTX.
*
* Return Value:
*	None.
*
* Notes:
*	If one wanted to do something extra to the plot, e.g. put a legend on etc.
*	s/he would possibly intercept this call (over-ride this function) then call
*	the base fn CGUI_PlotPane::UpdateDisplay(pPlCtx) and then do whatever
*	additional "drawing" was desired.
*
* See Also:
*	Plot library documentation.
*
*-Date      Author     Revision
* 24-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::UpdateDisplay(
	PL_CTX *pPlCtx,	// I - Plot library context structure.
	BOOL bEraseFirst)	// I - Call PlAreaClear before repainting.
{
	RefreshDisplay(pPlCtx, bEraseFirst);
	UpdateCursorReadout(pPlCtx);

	PlAreaArraySelect(pPlCtx, muiCurrentArea, 0);

	// According to the PLOT lib docs the program must show all objects
	// before displaying the cursor.
	PlObjPaintAll(pPlCtx);	
	
	// Plot library sends PL_CURS_ERASE before requesting that plot be drawn
	// so we need to show cursor after we have done the painting.
	ShowCursor(TRUE);
}


/*+/csubr/TOC===============================================================
* PlotLibEventPreFilter - Virtual, event-pre-filter fn.
*
* Purpose:
*	Whenever
*
* Return Value:
*	None.
*
* Notes:
*	If one wanted to do something extra to the plot, e.g. put a legend on etc.
*	s/he would possibly intercept this call (over-ride this function) then call
*	the base fn CGUI_PlotPane::UpdateDisplay(pPlCtx) and then do whatever
*	additional "drawing" was desired.
*
* See Also:
*	Plot library documentation.
*
*-Date      Author     Revision
* 24-Jan-97  ToM       Created.
*-===========================================================================*/

LRESULT CGUI_PlotPane::PlotLibEventPreFilter(
	HWND hWnd,
	UINT iMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	static BOOL bWaitingForZoomFactor = FALSE;
	LRESULT RetValue = 0;	// Always allow plot library to handle messages.
	BOOL bUpdateReadout = FALSE;

	switch (iMsg) {
		case WM_RBUTTONUP:
			{
				int iX = LOWORD(lParam);
				int iY = HIWORD(lParam);
				int iCol = PlWinCvtXPixToAreaCol(&mPlCtx, iX);
				int iRow = PlWinCvtYPixToAreaRow(&mPlCtx, iY);
				PlAreaArraySelect(&mPlCtx, iRow, iCol);
				double dXValue = PlWinCvtXPixToXVal(&mPlCtx, iX);
				double dYValue = PlWinCvtYPixToYVal(&mPlCtx, iY);
				UINT uiWhichArea = iRow;
					
				RetValue  = ServiceRightMouseBtn(uiWhichArea, dXValue, dYValue);
			}
			break;
		case WM_SETFOCUS:
			bUpdateReadout = TRUE;
			//pjm 20 Dec 2007 Resolution of issue 82 eliminates need for mbGraphHasFocus
			//mbGraphHasFocus = TRUE; 
			break;
		case WM_KILLFOCUS:
			bUpdateReadout = TRUE;
			//pjm 20 Dec 2007 Resolution of issue 82 eliminates need for mbGraphHasFocus
			//mbGraphHasFocus = FALSE;
			break;
		case WM_CHAR:
			switch (wParam) {
				case 'n':
				case 'N':
					// One might, for example, move to the NEXT view.
					break;
				case 'p':
				case 'P':
					// One might, for example, move to the PREVIOUS view.
					break;
				case 'x':
				case 'X':
					bWaitingForZoomFactor = TRUE;
					break;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					if (bWaitingForZoomFactor) {
						bWaitingForZoomFactor = FALSE;
						UINT uiZoomFactor = wParam - '0';
						SetZoomFactorX(uiZoomFactor);
					}
					else {
						GUI_Beep();
					}
					break;
				default:
					GUI_Beep();
					break;
			}
			break;
		default:
			break;
	}	
	if (bUpdateReadout) {
		UpdateCursorReadout(&mPlCtx);
	}
	return(RetValue);
}


/*+/csubr/TOC===============================================================
* ZoomChangeNotify - Virtual, zoom changed notify fn.
*
* Purpose:
*	It is possible for the end-user to change the X zoom on a plot via
*	direct interaction with CGUI_PlotPane using the keyborad. If this
*	happens, the application code may need to be notified of this.
*
* Return Value:
*	None.
*
* Notes:
*
* See Also:
*	PlotLibEventPreFilter
*
*-Date      Author     Revision
* 24-Jan-97  ToM       Created.
*-===========================================================================*/

void CGUI_PlotPane::ZoomChangeNotify(
	UINT uiNewZoomFactorX)	// I - New X-axis zoom factor.
{
}


LRESULT CGUI_PlotPane::PlotLibEventPreFilterCallBackFn(
	PL_CTX *pPlCtx,
	void *pUserData,
	HWND hWnd,
	UINT iMsg,
	WPARAM wParam,
	LPARAM lParam,
	int bDone)
{	
	CGUI_PlotPane *pPlotPane = (CGUI_PlotPane *)pUserData;
	LRESULT RetValue = 0;	// By default, allow plot library to handle messages.

	if (pPlotPane != NULL) {
		RetValue = pPlotPane->PlotLibEventPreFilter(hWnd, iMsg, wParam, lParam);
	}
	return(RetValue);
}


BOOL CGUI_PlotPane::SetRangeX()
{
	BOOL bSuccess = FALSE;
	UINT uiAreaIndex;
	int iStatus;
	CPlotArea *pAreaPlot;
	double dMinX, dMaxX, dMinY, dMaxY;
	double dOverallMinX, dOverallMaxX, dOverallMinY, dOverallMaxY;

	if (mbAutoScaleX) {
		// Find overall minimum and maximum values for any of the plot areas
		// (both x and y axis but ignore y-axis results).
		for (uiAreaIndex = 0; uiAreaIndex < muiNumAreas; uiAreaIndex++) {
			if (GetAreaPlot(uiAreaIndex, &pAreaPlot)) {
				bSuccess = pAreaPlot->GetDataRange(&dMinX, &dMaxX, &dMinY, &dMaxY);
				if (bSuccess) {
					if (uiAreaIndex == 0) {
						dOverallMinX = dMinX;
						dOverallMaxX = dMaxX;
						dOverallMinY = dMinY;
						dOverallMaxY = dMaxY;
					}
					else {
						if (dMinX < dOverallMinX)
							dOverallMinX = dMinX;
						if (dMaxX > dOverallMaxX)
							dOverallMaxX = dMaxX;
						if (dMinY < dOverallMinY)
							dOverallMinY = dMinY;
						if (dMaxY > dOverallMaxY)
							dOverallMaxY = dMaxY;
					}
				}
				else {
					// Also set y-range if we have no valid data. (So that we can center a msg in plot area.)
					dOverallMinY = 0;
					dOverallMaxY = 1;
				}
			}
		}
	}
	else {
		dOverallMinX = mdManualMinX;
		dOverallMaxX = mdManualMaxX;
	}

	for (uiAreaIndex = 0; uiAreaIndex < muiNumAreas; uiAreaIndex++) {
		iStatus = PlAreaArraySelect(&mPlCtx, uiAreaIndex, 0);
		if (iStatus == PL_OK) {
			// The following call only sets the x range; note FALSE preceding dMinY, dMaxY.
			// SFK Must set the dMinY and dMaxY to 0.0 
			iStatus = PlAreaRangeSet(&mPlCtx, TRUE, dOverallMinX, dOverallMaxX, FALSE, 0.0, 0.0);

			// If we are auto-scaling in X have the plot library "rationalize" the minimum and maximum
			// values displayed on the X axis. If the user has specified explicit values for the minimum
			// and maximum X-axis values (i.e. auto-scaling X is FALSE) do not "rationalize" those values.
			if (iStatus == PL_OK) {
				if (mbAutoScaleX) {
					iStatus = PlAreaRangeAutoEnds(&mPlCtx, TRUE, FALSE);
					if (iStatus == PL_OK) {
						bSuccess = TRUE;
					}
				}
			}
		}
	}
	
	if (!bSuccess) {
		ShowPlotLibErrMsg(iStatus);
	}

	return(bSuccess);
}


BOOL CGUI_PlotPane::SetRangeY(UINT uiAreaIndex)
{
	OutputDebugString("CGUI_PlotPane::SetRangeY\n");

	const BOOL bX_AXIS = FALSE;
	const BOOL bY_AXIS = TRUE;
	BOOL bSuccess = FALSE;
	int iStatus;
	CPlotArea *pAreaPlot;
	double dMinX, dMaxX, dMinY, dMaxY;
	BOOL bAutoScale;
	ULONG ulDataDim;
	
	iStatus = PlAreaArraySelect(&mPlCtx, uiAreaIndex, 0);
	if (iStatus == PL_OK) 
	{
		if (GetAreaPlot(uiAreaIndex, &pAreaPlot)) 
		{
			BOOL bLogY_Axis, bZoomAutoScale;
			
			pAreaPlot->GetStyleY(&bLogY_Axis, &bZoomAutoScale);
			pAreaPlot->GetDataRange(&dMinX, &dMaxX, &dMinY, &dMaxY, &bAutoScale, &ulDataDim, bLogY_Axis);

			// If the data for this area has not been set yet (ulDataDim == 0) and we
			// call PlAreaRangeSet we get an error message so skip around...
			if (ulDataDim == 0) 
			{
				iStatus = PL_OK;	// To avoid error msg. below.
			}
			else 
			{
				// The following call only sets the y range; notice FALSE preceding 0.0, 0.0
				// The x range is set elsewhere to include all areas. (See CGUI_PlotPane::SetRangeX().)
				// PL lib does not like the min and max for an axis to be equal so protect against
				// that situation.

				//Special cases for pure event data such as EOSS and Binary

				bool binary = false;
				if (dMinY < 1.0e-5)
					dMinY = 0.0;

				if ((dMinY == 0.0) && (dMaxY == 0.0))
					binary = true;
				else if ((dMinY == 0.0) && (dMaxY == 1.0))
					binary = true;
				else if ((dMinY == 1.0) && (dMaxY == 1.0))
					binary = true;

				if (binary)
				{
					dMaxY = 2.0;
					dMinY = 0.0;
					pAreaPlot->SetStyleY(FALSE,FALSE);//&bLogY_Axis, &bZoomAutoScale);
					bAutoScale = 0;
				}
				else if (dMinY == dMaxY)//Historical pre-EOSS tweaking here
				{
					if (dMinY == 0.0) 
						dMaxY += 0.001;	// Arbitrarily make the range 0.001.
					else 
						dMaxY += 0.01 * dMaxY;	// Arbitrarily make the range 1% of the minimum.
				}
				
				iStatus = PlAreaRangeSet(&mPlCtx, bX_AXIS, 0.0, 0.0, bY_AXIS, dMinY, dMaxY);


// zzzzzzzzzzzzzzzzz KLUGE Tom 16-May-97
// This works around a bug in Roger's Plot lib. It should be fixed in pl.lib...

if (mPlCtx.pYAxis->eStyle == 44) {
	if (mPlCtx.pYAxis->dDValB_minNon0 > mPlCtx.pYAxis->dDValB) {
		if (mPlCtx.pYAxis->dDValB > 0.0) {
			mPlCtx.pYAxis->dDValB_minNon0 = mPlCtx.pYAxis->dDValB;
		}
	} 
}
				
				if (iStatus == PL_OK) {
					if (bAutoScale) {
						// Call PlAreaRangeAutoEnds to modify y range to "rational" numbers.
						iStatus = PlAreaRangeAutoEnds(&mPlCtx, bX_AXIS, bY_AXIS);
						if (iStatus == PL_OK) {
							bSuccess = TRUE;
						}
					}
					else {
						// If caller specified y limits don't "rationalize" them.
						bSuccess = TRUE;
					}
				}
			}
		}
	}

	if (iStatus != PL_OK)
		ShowPlotLibErrMsg(iStatus);

	return(bSuccess);
}


BOOL CGUI_PlotPane::RegisterNewPlotArea(CPlotArea *pPlotArea, UINT *puiAreaIndex, UINT uiNewAreaIndex)
{
	BOOL bSuccess = FALSE;
	
	if (pPlotArea == NULL) {
		// Decrement number of plot areas.
		// I don't think this is neccessary, has it ever been incremented? ToM 13-Jan-97
		// SetNumAreas(muiNumAreas - 1);
	}
	else {
		//UINT uiNewAreaIndex = muiNumAreas;
		*puiAreaIndex = uiNewAreaIndex;
		if (uiNewAreaIndex != muiNumAreas) {
			// We are inserting an area so bump others "up".
			for (UINT ui = muiNumAreas; ui > uiNewAreaIndex; ui--) {
				mapCPlotAreaAxes[ui] = mapCPlotAreaAxes[ui - 1];
				mapCPlotAreaAxes[ui]->SetAreaIndex(ui);
			}
		}
		mapCPlotAreaAxes[uiNewAreaIndex] = pPlotArea;
		SetNumAreas(muiNumAreas + 1);
		bSuccess = TRUE;
		SetRangeX();
		SetRangeY(uiNewAreaIndex);
		if (muiNumAreas == 1) CenterCursor(0);	// If this is the first one center cursor in it.
	}
	return(bSuccess);
}

void CGUI_PlotPane::FmtDoubleValue(double dValue, CGStr *pStr)
{
	CGFmtStr Format("%");
	CGFmtStr FormattedStr;
	
	double dIntegerPortion = ceil(dValue);
	
	if (dIntegerPortion == dValue) {
		Format += ".0";
	}
	else {
		if (dValue > 100.0) {
			Format += ".1";
		}
		else if (dValue > 10.0) {
			Format += ".2";
		}
		else if (dValue > 1.0) {
			Format += ".3";
		}
		else if (dValue > .1) {
			Format += ".4";
		}
		else {
			Format += ".6";
		}
	}
	
	Format += "f";
	
    FormattedStr.Printf(Format.Get_sz(), dValue);
    *pStr = FormattedStr;
}


void CGUI_PlotPane::UpdateCursorReadout(PL_CTX *pPlCtx)
{
	BOOL bVisible = FALSE;
	CPlotArea *pAreaPlot;
	double dCursorX, dCursorY;
	ULONG ulIndex;

	PlPlotTitleClear(pPlCtx, PL_SIDE_BOTTOM, 0);

	//issue 82 - Cursor readout should remain when not current focus.
	//pjm 20 Dec 2007 Resolution of issue 82 eliminates need for mbGraphHasFocus
	if (mbPrinting || mbCursorVisible)
		bVisible = TRUE;

	if (bVisible) {
		if (GetAreaPlot(muiCurrentArea, &pAreaPlot)) {
			CGStr AreaName;
			CGFmtStr CursorReadoutText;
			CGStr X_ValueStr;
			CGStr Y_ValueStr;
			BOOL bInterpolatedY;
			COLORREF mCursorReadoutColor = CGUI_PlotPane::mCursorReadoutColor;
			BOOL bCursorInRange = pAreaPlot->GetCursorXY(&dCursorX, &dCursorY, &ulIndex, &bInterpolatedY);

			pAreaPlot->GetAreaLabel(&AreaName);
			BOOL bFOR_CURSOR_VALUE = TRUE;
			GetAnnotationX(dCursorX, &X_ValueStr, bFOR_CURSOR_VALUE);
			GetAnnotationY(dCursorY, &Y_ValueStr, bFOR_CURSOR_VALUE);
				
			CursorReadoutText.Printf("Cursor: %s; X=%s", AreaName.Get_sz(), X_ValueStr.Get_sz());
			if (bCursorInRange) {
				CursorReadoutText.PrintfAppend(" Y=%s", Y_ValueStr.Get_sz());
				if (bInterpolatedY) CursorReadoutText.PrintfAppend(" INTERPOLATED.");
			}
			else {
				CursorReadoutText.PrintfAppend(" OUT OF RANGE.");
			}
		
			BOOL bTextAlreadyDisplayed = DisplayCursorReadoutText(pPlCtx, AreaName, muiCurrentArea,
				dCursorX, dCursorY, bInterpolatedY, bCursorInRange, &CursorReadoutText, &mCursorReadoutColor);

			if (!bTextAlreadyDisplayed) {
				PlPlotTitleText(pPlCtx, PL_SIDE_BOTTOM, 0, .5F,
					CursorReadoutText.Get_sz(), PRT_CEN, mCursorReadoutColor);
			}
		}
	}
}


void CGUI_PlotPane::ShowCursor(BOOL bShow, PL_CTX *pPlCtx)
{
	int iStatus;
	double dCursorY;
	double dCursorX;
	CPlotArea *pAreaPlot;
	BOOL bLogY_Axis;
	BOOL bZoomAutoScale;
	BOOL bCompositeShow;
	BOOL bVisible = FALSE;
	
	if (mbPrinting || mbCursorVisible) {
		bVisible = TRUE;
	}
	bCompositeShow = bVisible && bShow;

	if (mbCursorVisible) {
		if (GetAreaPlot(muiCurrentArea, &pAreaPlot)) {
			pAreaPlot->GetStyleY(&bLogY_Axis, &bZoomAutoScale);
			// Sometimes the caller does not have the plot context to send in
			// so retrieve the current plot context from the member variable.
			if (pPlCtx == NULL) {
				pPlCtx = &mPlCtx;
			}
			if (pPlCtx != NULL) {
				iStatus = PlAreaArraySelect(pPlCtx, muiCurrentArea, 0);
				if (iStatus == PL_OK) {
					ULONG ulIndex;
					BOOL bInterpolatedY;
					
					BOOL bCursorInRange = pAreaPlot->GetCursorXY(&dCursorX, &dCursorY,
						&ulIndex, &bInterpolatedY, bLogY_Axis);
					iStatus = PlPaintCursor(pPlCtx, !bCompositeShow, dCursorX, dCursorY,
						PL_CROSS_FULL, 18.F, mCursorColor);
				}
			}
		}
	}
	return;
}


void CGUI_PlotPane::MoveCursorToValueX(double dX)
{
	CPlotArea *pAreaPlot;
	
	if (GetAreaPlot(muiCurrentArea, &pAreaPlot)) {
		pAreaPlot->MoveCursorToValueX(dX);
	}
}


void CGUI_PlotPane::MoveCursor(double dStepsX)
{
	CPlotArea *pAreaPlot;
	
	if (GetAreaPlot(muiCurrentArea, &pAreaPlot)) {
		pAreaPlot->MoveCursor(dStepsX);
	}
}


void CGUI_PlotPane::CursorCallBackFn(PL_CTX *pPlCtx, int eEvent, double dArg1, double dArg2)
{
	BOOL bUpdateCursorReadout = TRUE;
	//CPlotArea *pPlotArea;
	// This is called by the static (callback) function which has passed a pointer to the
	// appropriate object to call the non-static member functions such as this one.
	switch (eEvent) {
		case PL_CURS_AREA: 
			{
				//muiCurrentArea = (UINT)dArg1;
				SetCurrentArea((UINT)dArg1);
				//unsigned long ulDum = mulCursorIndex;
				//double dDum = mdCurrentCursX;
				//if (GetAreaPlot(muiCurrentArea, &pPlotArea)) {
				//	pPlotArea->FindIndexGivenX(mdCurrentCursX, &mulCursorIndex);
				//}
				//this->SetCurrentArea(
				ShowCursor(TRUE);
			}
			break;
		case PL_PAINT:
			ShowCursor(TRUE);
			break;
		case PL_CURS_ERASE:
			bUpdateCursorReadout = FALSE;
			ShowCursor(FALSE);
			break;
		case PL_LMOUSE:
			// Use x-value only (dArg1); ignore y-value (dArg2).
			// The cursor is hidden by the plot package before this is called. SetCursorPos
			// will, by default, hide, move and then show the cursor. So we need to show
			// the cursor before calling SetCursorPos. An alternative would be to tell
			// SetCursorPos not to update display and then explicitly call ShowCursor(TRUE)
			// after calling SetCursorPos.
			ShowCursor(TRUE);
			SetCursorPos(dArg1);
			break;
		case PL_CURS_HORIZ:
			// The cursor is hidden by the plot package before this is called. SetCursorPos
			// will, by default, hide, move and then show the cursor. So we need to show
			// the cursor before calling SetCursorPos. An alternative would be to tell
			// SetCursorPos not to update display and then explicitly call ShowCursor(TRUE)
			// after calling SetCursorPos.
			ShowCursor(TRUE);
			MoveCursor(dArg1);
			break;
		case PL_CURS_VERT:
			{
				UINT uiNumAreas = muiNumAreas;
				UINT uiCurrentArea = muiCurrentArea;
					
				if (dArg1 < 0) {
					// Moving down to next area.
					uiCurrentArea++;
					if (uiCurrentArea > (uiNumAreas - 1)) {
						uiCurrentArea = 0; 	// wrap around.
					}
				}
				else {
					if (uiCurrentArea == 0) {
						uiCurrentArea = uiNumAreas - 1; 	// wrap around.
					}
					else {
						uiCurrentArea--;
					}
				}
				SetCurrentArea(uiCurrentArea);
				//muiCurrentArea = uiCurrentArea;
				ShowCursor(TRUE);
			}

			break;
		case PL_GOT_FOCUS:
		case PL_LOST_FOCUS:
			break;
		default:
			GUI_Beep();
			break;
	}

	if (bUpdateCursorReadout) {
		UpdateCursorReadout(pPlCtx);
	}
}


void CGUI_PlotPane::PlotLibCursorCallBackFn(PL_CTX *pPlCtx, void *pUserData,
	int eEvent, double dArg1, double dArg2)
{
	// This is a static (callback) function so get a pointer to the appropriate
	// object to call the "equivalent" non-static member function.
	CGUI_PlotPane *pPlotPane = (CGUI_PlotPane *)pUserData;

	if (pPlotPane != NULL) {
		pPlotPane->CursorCallBackFn(pPlCtx, eEvent, dArg1, dArg2);
	}
}

void CGUI_PlotPane::PlotLibPlotCallBackFn(PL_CTX *pPlCtx, void *pUserData)
{
	CGUI_PlotPane *pPlotPane = (CGUI_PlotPane *)pUserData;

	// This function must be static so it can be registered with the pl lib.
	// Once it gets called get a pointer to the appropriate CGUI_PlotPane which
	// was registered with the pl lib and is now handed to us as pUserData.
	
	if (pPlotPane != NULL) {
		// If we get a plot call back while printing ignore it.
		if (!pPlotPane->mbPrinting) {
			pPlotPane->UpdateDisplay(pPlCtx);
		}
	}
}

BOOL CGUI_PlotPane::AutoY_ScaleVisibleRange(double dCurrentAnnotX)
{
	BOOL bScaleChanged = FALSE;
	BOOL bVisible = TRUE;	// Get only visible (vs. full) range.
	double dXLeft, dXRight, dYBotOrig, dYTopOrig, dYBotNew, dYTopNew;
	double dMinY, dMaxY;
	int iStatus;
	CPlotArea *pAreaPlot;
	BOOL bLogY, bZoomAutoScale;

	// Step through the Areas.
	for (UINT ui = 0; ui < muiNumAreas; ui++) 
	{
		if (GetAreaPlot(ui, &pAreaPlot)) 
		{
			pAreaPlot->GetStyleY(&bLogY, &bZoomAutoScale);

			if (bZoomAutoScale) 
			{
				iStatus = PlAreaArraySelect(&mPlCtx, ui, 0);

				if (iStatus == PL_OK) 
				{
					// Need to get dYBotOrig & dYTopOrig now that appropriate Area has been selected.
					iStatus = PlAreaRangeGet(&mPlCtx, bVisible, &dXLeft, &dXRight, &dYBotOrig, &dYTopOrig);

					if (iStatus == PL_OK) 
					{
						// PlAreaRangeSet requires positive-only numbers for log scale so if log display
						// call pAreaPlot->GetY_RangeGivenX_Range with bPositiveOnly = TRUE.
						BOOL bPositiveOnly = bLogY;

						if (pAreaPlot->GetY_RangeGivenX_Range(dXLeft, dXRight, &dMinY, &dMaxY, bPositiveOnly)) 
						{
	    					// The following call only sets the y range; note FALSE preceding 0.0, 0.0
							iStatus = PlAreaRangeSet(&mPlCtx, FALSE, 0.0, 0.0, TRUE, dMinY, dMaxY);

							if (iStatus == PL_OK) 
							{
								// Call PlAreaRangeAutoEnds to modify y range to "rational" numbers.
								//Beep(1000,1000);
								iStatus = PlAreaRangeAutoEnds(&mPlCtx, FALSE, TRUE);

								if (iStatus == PL_OK) 
								{
									iStatus = PlAreaRangeGet(&mPlCtx, bVisible, &dXLeft, &dXRight, &dYBotNew, &dYTopNew);
									
									if (iStatus == PL_OK) 
									{
										if ((dYBotNew != dYBotOrig) || (dYTopNew != dYTopOrig)) 
											bScaleChanged = TRUE;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	// Issue 164 fix: expanding graph during event edit causes the
	// cursor to move to bottom trace.
	// Solution - restore plotting control structure to point to currently
	// active trace.
	PlAreaArraySelect(&mPlCtx, muiCurrentArea, 0);

	if (bScaleChanged)
		InvalidateGraphDisplay();	// Schedule a repaint.

	return(bScaleChanged);
}

void CGUI_PlotPane::RefreshDisplay(PL_CTX *pPlCtx, BOOL bEraseFirst)
{
	CPlotArea *pAreaPlot;
	static BOOL bActive = FALSE;	
	BOOL bVerticalScaleChanged;

	if ((!mbRefreshBlocked) || (mbPrinting)) 
	{
		if (mbValidPlotPane) 
		{
			bVerticalScaleChanged = AutoY_ScaleVisibleRange(0.0f);

            if (!bVerticalScaleChanged) 
			{
				for (UINT ui = 0; ui < muiNumAreas; ui++) 
				{
					if (GetAreaPlot(ui, &pAreaPlot)) 
						pAreaPlot->RefreshDisplay(pPlCtx, bEraseFirst); 
				}
				//restore currently selected pane
				// Issue 164 fix: expanding graph during event edit causes the
				// cursor to move to bottom trace.
				// Solution - restore plotting control structure to point to currently
				// active trace.
				PlAreaArraySelect(pPlCtx, muiCurrentArea, 0);

				if (mbPrinting) 
				{
					// We pass in the plot context for printing otherwise my routines
					// will use the screen plot context which they have stored.
					UpdateCursorReadout(pPlCtx);
					ShowCursor(TRUE, pPlCtx);	// Always show cursor on printout.
				}
			}
		}
	}
}


BOOL CGUI_PlotPane::GetAreaPlot(UINT uiAreaIndex, CPlotArea **ppAreaPlot)
{
	BOOL bSuccess = FALSE;

	*ppAreaPlot = NULL;
	
	if ((uiAreaIndex >= 0) && (uiAreaIndex < muiNumAreas)) {
		*ppAreaPlot = mapCPlotAreaAxes[uiAreaIndex];
	}
	else {
		*ppAreaPlot = NULL;
	}
		
	if (*ppAreaPlot != NULL) {
		bSuccess = TRUE;
	}
	
	return(bSuccess);
}


void CGUI_PlotPane::GetFirstAndLastAreaIndex(UINT uiAreaIndex, UINT *puiFirstArea, UINT *puiLastArea)
{
	if (uiAreaIndex == GUI_PLOT_ALL_AREAS) {
		*puiFirstArea = 0;
		if (muiNumAreas > 0) *puiLastArea = muiNumAreas - 1;
		else *puiLastArea = 0;
	}
	else {
		*puiFirstArea = uiAreaIndex;
		*puiLastArea = uiAreaIndex;
	}
}


const char *CGUI_PlotPane::PlotLibX_AxisAnnotCallBackFn(PL_CTX *pCtx, void *pUserData, double dX)
{
	static CGFmtStr Annotation;
	double dXLeftFull, dXRightFull, dXLeftVisible, dXRightVisible;
	double dYBotVisible, dYTopVisible, dYBotFull, dYTopFull;
    BOOL bVisible;
	CGUI_PlotPane *pPlotPane = (CGUI_PlotPane *)pUserData;
	
	if (pPlotPane != NULL) {
		// Get full axis range in case we are auto-scaling to entire data set.
		bVisible = TRUE;
		PlAreaRangeGet(pCtx, bVisible, &dXLeftVisible, &dXRightVisible, &dYBotVisible, &dYTopVisible);
		// Get visible axis range in case we are auto-scaling to visible part only.
		bVisible = FALSE;
		PlAreaRangeGet(pCtx, bVisible, &dXLeftFull, &dXRightFull, &dYBotFull, &dYTopFull);
			
		// Annotate only top and bottom grid lines to avoid clutter.
		if ((dX == dXRightFull) || (dX == dXRightVisible) || (dX == dXLeftFull) || (dX == dXLeftVisible)) {
			pPlotPane->GetAnnotationX(dX, &Annotation);
		}
	}
	return(Annotation.Get_sz());
}


const char *CGUI_PlotPane::PlotLibY_AxisAnnotCallBackFn(PL_CTX *pCtx, void *pUserData, double dY)
{
	static CGFmtStr Annotation;
	double dXLeftFull, dXRightFull, dXLeftVisible, dXRightVisible;
	double dYBotVisible, dYTopVisible, dYBotFull, dYTopFull;
    BOOL bVisible;
	CGUI_PlotPane *pPlotPane = (CGUI_PlotPane *)pUserData;
	Annotation = ""; //sfk
	if (pPlotPane != NULL) {
		// Get full axis range in case we are auto-scaling to entire data set.
		bVisible = TRUE;
		PlAreaRangeGet(pCtx, bVisible, &dXLeftVisible, &dXRightVisible, &dYBotVisible, &dYTopVisible);
		// Get visible axis range in case we are auto-scaling to visible part only.
		bVisible = FALSE;
		PlAreaRangeGet(pCtx, bVisible, &dXLeftFull, &dXRightFull, &dYBotFull, &dYTopFull);
			
		// Unless we are in log Y axis mode annotate only top and bottom grid lines to avoid clutter.
		if ((dY == dYBotVisible) || (dY == dYTopVisible) || (dY == dYBotFull) || (dY == dYTopFull)) {
			pPlotPane->GetAnnotationY(dY, &Annotation);
		}
	}
    return(Annotation.Get_sz());
}


void CGUI_PlotPane::SetNumAreas(UINT uiNumAreas)
{
	const float fCOL_SPACING_PTS = 0.0F;
	const int iNUM_COLS = 1;
	const int iNUM_MAJOR_DIV = 1;
	const int iNUM_MINOR_DIV = 1;
	const float fANNOT_DEG = 0.0F;
	const int iMAX_ANNOT_CHAR = 0;
	const int iNUM_TITLE_LINES_ABOVE = 0;
	const int iNUM_TITLE_LINES_BELOW = 0;
					
	BOOL bSuccess = FALSE;
	UINT uiPrevNumAreas = muiNumAreas;
	int iStatus;
	
	if (uiNumAreas != uiPrevNumAreas) {
		muiNumAreas = uiNumAreas;
		
		// We do not want any repaint requests to be honored while new setup
		// is being established. Otherwise we run the danger of repainting
		// one of the areas that is not fully (even adequately) specified.
		mbRefreshBlocked = TRUE;

		// Tell the plot library how many Areas (rows) we have.
		iStatus = PlAreaArrayInit(&mPlCtx, muiNumAreas, mfRowSpacingPts, iNUM_COLS, fCOL_SPACING_PTS);
		if (iStatus == PL_OK) {
			for (UINT ui = 0; ui < muiNumAreas; ui++) {
				iStatus = PlAreaArraySelect(&mPlCtx, ui, 0);	// Column 0 (last argument).
				if (iStatus == PL_OK) {
					float fPtSize;
							
					if (ui == (muiNumAreas - 1)) {
						fPtSize = CGUI_PlotPane::GetTextPtSize();
					}
					else {
						fPtSize = 0.F;	// In other words, invisible.
					}                                                       
						
					iStatus = PlAreaXStyle(&mPlCtx, PL_TICK_AXIS, PL_LIN,
						iNUM_MAJOR_DIV, iNUM_MINOR_DIV, fANNOT_DEG, iMAX_ANNOT_CHAR, 
						fPtSize, CGUI_PlotPane::GetTypeFace(), "%0.f",
						iNUM_TITLE_LINES_BELOW, iNUM_TITLE_LINES_ABOVE,
						CGUI_PlotPane::GetTextPtSize(), CGUI_PlotPane::GetTypeFace());
					
					if (iStatus == PL_OK) {
						bSuccess = SetUserAnnotation(mbUserAnnotX, mbUserAnnotY);
					}
				}
            
            	if (ui >= uiPrevNumAreas) {
					SetStyleY(ui);		// Set new axes to defaults.
                }
    	        else {
					// Calling PlAreaArrayInit above erases all setup info for the
					// previously exisiting plot areas so restore that info.
					BOOL bPrevLogY_Axis, bPrevZoomAutoScale;
					int iMajorGridLines;
					
					GetStyleY(ui, &bPrevLogY_Axis, &bPrevZoomAutoScale, &iMajorGridLines);
					SetLogY(ui, bPrevLogY_Axis);
					SetZoomAutoScaleY(ui, bPrevZoomAutoScale);
        	    }
			}
			SetZoomFactorX(muiZoomFactor);
		}

		if (!bSuccess)
			ShowPlotLibErrMsg(iStatus);

		mbRefreshBlocked = FALSE;
	}	
	
	if (muiNumAreas == 0)
		mbValidPlotPane = FALSE;
	else
		mbValidPlotPane = TRUE;

	// If the current axis no longer exist set current axis to zero.
	if (muiCurrentArea >= muiNumAreas) {
		muiCurrentArea = 0;
	}
	
	InvalidateGraphDisplay();	// Make sure screen is updated.
}


