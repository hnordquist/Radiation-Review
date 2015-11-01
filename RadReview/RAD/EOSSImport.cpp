// EOSSImport.cpp: implementation of the CEOSSImport class.
//
//////////////////////////////////////////////////////////////////////

#include "RadBuild.h"
#include "afxconv.h"
#include "ComDataStructures.h"

#include "imports.h"
#include "EOSSImport.h"		// moved afer sk 04-16-2003
#include "MyDateTime.h"
#include "SafeArrayUtil.h"
#include "TypeDefinitions.h"
#include "ErrorProcessing.h"
#include "FacilityConfig.h"

#include <string>
using namespace std;

IEOSSImportPtr pIEOSSImport;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEOSSImport::CEOSSImport(const char* szDefaultPath, short sFacilityID)
{
	//OutputDebugString("CEOSSImport::CTOR\n");

	m_sFacilityID = sFacilityID;
	mstrDataStorePath[0] = '\0';
	mstrDefaultPath[0] = '\0';
	strncpy(mstrDefaultPath, szDefaultPath, 256);
	pIEOSSImport.CreateInstance(CLSID_EOSSImport);

	if (pIEOSSImport == NULL)
	{
		MessageBox(
			NULL,
			"Could not Create Instance of EOSS Import component.\npIEOSSImport = NULL."
			,NULL,MB_OK);
	}
	else
	{		
		#define DISABLE_LOGGER		0x00020000
		#define QUIET_MODE_OFF		0

		VARIANT var;
		VariantInit(&var);
		var.vt = VT_I4;
		var.lVal = DISABLE_LOGGER | QUIET_MODE_OFF;

		pIEOSSImport->SetParameters(m_sFacilityID, var);
		VariantClear(&var);
	}
}

CEOSSImport::~CEOSSImport()
{
	//OutputDebugString("CEOSSImport::DTOR\n");
	pIEOSSImport = NULL;				
}

int CEOSSImport::AddData(
	const short sFacID, 
	const char* strFilename, 
	BOOL bOverwrite, 
	char** strResult)
{
	if (pIEOSSImport == NULL)
		return 0;

	BSTR temp1 = NULL;

	//USES_CONVERSION;

	VARIANT_BOOL vb = pIEOSSImport->AddData(sFacID, strFilename, bOverwrite, &temp1);

	CString cs(temp1);
	::SysFreeString(temp1);
	int len = cs.GetLength();
	*strResult = new char[len+1];	// allocate storage
	strcpy(*strResult, (LPCTSTR)cs);	

	//int len = strlen(W2T(temp1));	// determine length of string 
	//*strResult = new char[len+1];	// allocate storage
	//strcpy(*strResult, W2T(temp1));	// copy to storage -- caller must deallocate

	if (vb)
		return 1;
	else
		return 0;
}

int CEOSSImport::GetDataStatistics(
	const short sFacID, 
	const short sStaID, 
	const short sChanID, 
	const DATE dStart, 
	const DATE dEnd, 
	DATE *pdActualStart, 
	DATE *pdActualEnd, 
	unsigned long *pulNumPts)
{
	//OutputDebugString("CEOSSImport::GetDataStatistics()\n");
	long lNumPts;

	short sChanIndex = -1;
	if (sChanID != -1)
		sChanIndex = ConvertChanIDToIndex (sFacID,(long)sStaID,sChanID);
	if (pIEOSSImport->GetDataStatistics(sFacID, sStaID, sChanIndex, dStart, dEnd, 
			pdActualStart, pdActualEnd, &lNumPts))
	{
		*pulNumPts = lNumPts;
		return 1;
	}
	else
	{
		*pdActualStart = 0;
		*pdActualEnd = 0;
		*pulNumPts = 0;
		return 0;
	}
}

#define STARTDATE (1)
#define ENDDATE   (2)

int CEOSSImport::GetDbStartDay(
	const short sFacID, 
	const short sStaID,
	DATE *pdStartDay,
	const BOOL bErrDisplay)
{
	return GetDbStartEndDay(sFacID, sStaID,pdStartDay,bErrDisplay,STARTDATE);
}

int CEOSSImport::GetDbEndDay(
	const short sFacID, 
	const short sStaID,
	DATE *pdEndDay,
	const BOOL bErrDisplay)
{
	return GetDbStartEndDay(sFacID, sStaID,pdEndDay,bErrDisplay,ENDDATE);
}

int CEOSSImport::GetDbStartEndDay(  //private
	const short sFacID, 
	const short sStaID,
	DATE *pdOutputDay,
	const BOOL bErrDisplay,
	int which)
{
	//OutputDebugString("CEOSSImport::GetDbStartEndDay()\n");
	DATE dStartDate, dEndDate;

	//<<QA>>Heather, LONG or SHORT for sStaID here???
	int istatus = GetDataStoreTimeRange(
			sFacID, sStaID, bErrDisplay, &dStartDate, &dEndDate);
			//sFacID, (LONG)sStaID, bErrDisplay, &dStartDay, &dEndDate);

	if (istatus == 0)
	{
		*pdOutputDay = 0.0;
		return FALSE;
	}

	if (which == STARTDATE)
		*pdOutputDay = dStartDate;
	else
		*pdOutputDay = dEndDate;

	return TRUE;
}

int CEOSSImport::GetDataStoreTimeRange(
	const short sFacID, 
	const short sStaID,  //LONG??
	const BOOL bErrDisplay,
	DATE *pdStartDay,
	DATE *pdEndDay)
{
	//OutputDebugString("CEOSSImport::GetDataStoreTimeRange()\n");

	VARIANT_BOOL vb = bErrDisplay?VARIANT_TRUE:VARIANT_FALSE;
 
	if (pIEOSSImport->GetDataStoreTimeRange(sFacID, sStaID, vb, pdStartDay, pdEndDay))
		return 1;
	else
		return 0;
}


int CEOSSImport::GetDayStatistics(
	const short sFacID, 
	const struct IDStruct *pStaID, 
	const DATE dStart, 
	const DATE dEnd, 
	DATE *pdActualStart, 
	DATE *pdActualEnd, 
	unsigned long *pulNumPts)
{
	*pdActualStart = 0;
	*pdActualEnd = 0;
	*pulNumPts = 0;

	if (pIEOSSImport == NULL)
		return 0;

	long lNumPts;

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID
 
	VARIANT_BOOL vb = 
		pIEOSSImport->GetDayStatistics(sFacID, (LONG)pStaID->sStaID, dStart, dEnd, pdActualStart, pdActualEnd, &lNumPts);

	if (vb)
	{
		*pulNumPts = lNumPts;
		return 1;
	}

	return 0;
}



int CEOSSImport::GetDataByDay(
	const short sFacID, 
	const struct IDStruct *pStaID, 
	const DATE dStart, 
	const DATE dEnd, 
	struct DayDataStruct *pDayStruct, 
	unsigned long *pulNumDays)
{
	if (pIEOSSImport == NULL)
		return 0;

	long lIndex;
	VARIANT_BOOL vb = VARIANT_FALSE;

	if (*pulNumDays == 0) 
		return(1);
	
	VARIANT vDayData;
	VariantInit(&vDayData);

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID
	vb = pIEOSSImport->GetDaySummaryData(sFacID, pStaID->sStaID, dStart, dEnd, &vDayData);

	//HRESULT GetDaySummaryData([in] short sFacilityID, [in] short sStationID, [in] DATE StartTime, [in] DATE EndTime,  [out] VARIANT* pvDay,  [out,retval] VARIANT_BOOL* pbResult);

	// now pull the data back out and put into the pre-allocated structure expected by Rad Review
	if (vb  && (vDayData.vt == (VT_VARIANT | VT_ARRAY))) 
	{	// is the type of SafeArray we are expecting 
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vDayData.parray, 1, &lUBound);	
		lUBound++;

		if (lUBound > (long)*pulNumDays)		// got back more points than expected only return first *pulNumPts
			lUBound = (long)(*pulNumDays - 1);  //fixed compiler warning

		double dTemp;
		for (lIndex=0; lIndex<lUBound; lIndex++) 
		{
			SA_GetElem(vDayData.parray, lIndex, 0, &(pDayStruct[lIndex].dFirstDataPt));
			SA_GetElem(vDayData.parray, lIndex, 1, &(pDayStruct[lIndex].dLastDataPt));
			SA_GetElem(vDayData.parray, lIndex, 2, &dTemp);
			pDayStruct[lIndex].ulNumPtsInDay = (unsigned long)dTemp;
		}
		*pulNumDays = lUBound;
	}
	VariantClear(&vDayData);

	if (vb)
		return 1;
	else
		return 0;
}

int CEOSSImport::InitializeDatabase(const short sFacilityID, const bool bQuestion)
{
	if (pIEOSSImport == NULL)
		return 0;

	VARIANT_BOOL vb = bQuestion?VARIANT_TRUE:VARIANT_FALSE;

	if (pIEOSSImport->InitializeDataStore(sFacilityID, vb))
		return 1;
	else
		return 0;
}

void CEOSSImport::CloseDatabase()
{
	if (pIEOSSImport) 
		pIEOSSImport->CloseDataStore();
}

int CEOSSImport::GetDataByChannel(
		const short sFacID, 
		const short sStaID, 
		const short sChanID, 
		const DATE dStart, 
		const DATE dEnd, 
		double *pdX, 
		double *pdY, 
		unsigned long *pulNumPts, 
		DATE *pdNextTime, 
		short sStaType)
{
	//OutputDebugString("CEOSSImport::GetDataByChannel\n");

	//if (*pulNumPts == 0) // EOSS CAN have zero "real" points.
	//	return(1);

	short sChanIndex = ConvertChanIDToIndex (sFacID,(long)sStaID,sChanID);
	*pdNextTime = 0.0;
	
	VARIANT vChannelData;
	VariantInit (&vChannelData);
	
	// Call GetDataByStation
	VARIANT_BOOL vbStatus = pIEOSSImport->GetDataByChannel (sFacID, (LONG)sStaID, sChanIndex,dStart, dEnd, &vChannelData);
	
	if (vbStatus != VARIANT_FALSE)
	{
		//Check for valid SAFEARRAY, see how many channels of data we have
		if (vChannelData.parray && vChannelData.vt == (VT_VARIANT|VT_ARRAY))
		{
			long lChannelDataLBound, lChannelDataUBound = 0;
			SafeArrayGetLBound (vChannelData.parray,1,&lChannelDataLBound);
			SafeArrayGetUBound (vChannelData.parray,1,&lChannelDataUBound);
			for (long lChanEvent = lChannelDataLBound; lChanEvent <= lChannelDataUBound; lChanEvent++)
			{
				// Get the data point out and store in pData [Index]
				DATE dtEventTime = 0;
				int nSealState = 0;
				SA_GetElem (vChannelData.parray,lChanEvent,0,&dtEventTime);
				SA_GetElem (vChannelData.parray,lChanEvent,1,(int*)&nSealState);
				pdX[lChanEvent] = dtEventTime;
				pdY[lChanEvent] = (double)nSealState;
			}
			//pjm
			//if (vChannelData.parray)
			//{
			//	SafeArrayDestroy (vChannelData.parray);
			//	VariantClear (&vChannelData);
			//}
		}
		else
		{
			vbStatus = VARIANT_FALSE;  //pjm
		}
	}
	VariantClear(&vChannelData);
	return (vbStatus!=VARIANT_FALSE?1:0);
}
	
int  CEOSSImport::GetDataByStation(
		const short sFacID,
		const short sStaID,
		const DATE dStart,
		const DATE dEnd,
		struct WHOLE_LONG_DATA_PT *pData,
		unsigned long *pulNumPts,
		DATE *pdNextTime)
{
	if (*pulNumPts == 0) 
		return(1);

	*pdNextTime = 0.0;
	int Index = 0;
	
	VARIANT vStationData;
	VariantInit (&vStationData);

	// Call GetDataByStation
	VARIANT_BOOL vbStatus = pIEOSSImport->GetDataByStation (sFacID, (LONG)sStaID, dStart, dEnd, &vStationData);
	
	if (vbStatus !=VARIANT_FALSE)
	{
		//Check for valid SAFEARRAY, see how many channels of data we have
		if (vStationData.parray && vStationData.vt == (VT_VARIANT|VT_ARRAY))
		{
			long uBound,lBound = 0;
			SafeArrayGetLBound (vStationData.parray,1,&lBound);
			SafeArrayGetUBound (vStationData.parray,1,&uBound);
			
			unsigned long ArraySize = 0;
			
			// GetDataStatistics for each channel so that we can allocate the data array correctly
			for (long l = lBound; l <= uBound; l++)
			{
				DATE actualStart, actualEnd = 0;
				long ulNum;
				vbStatus = pIEOSSImport->GetDataStatistics (sFacID,(LONG)sStaID,l+1,dStart,dEnd,&actualStart,&actualEnd,&ulNum);		
				ArraySize += ulNum;
				ArraySize+=2;
			}

//			*pulNumPts = ArraySize;

			if (vbStatus != VARIANT_FALSE)
			{
				//Allocate Data array
//				pData = (struct WHOLE_LONG_DATA_PT *)malloc((ArraySize) * sizeof(struct WHOLE_LONG_DATA_PT));


				for (l = lBound; l <= uBound; l++)
				{
					VARIANT vChannelSafeArray;
					VariantInit (&vChannelSafeArray);

					long lDim[2];
					lDim[0] = l;
					lDim[1] = 1;

					//Grab each chunk of ChannelData
					HRESULT hr = SafeArrayGetElement (vStationData.parray,lDim,&vChannelSafeArray);

					if (!FAILED (hr))
					{
						if ((vChannelSafeArray.vt != VT_EMPTY) && (vChannelSafeArray.parray))
						{
							long lChannelDataLBound, lChannelDataUBound = 0;
							SafeArrayGetLBound (vChannelSafeArray.parray,1,&lChannelDataLBound);
							SafeArrayGetUBound (vChannelSafeArray.parray,1,&lChannelDataUBound);
							short sChanID = 0;
							pglFacCfg->GetChannelIDFromIndex (sFacID,sStaID,l,&sChanID);
							for (long lChanEvent = lChannelDataLBound; lChanEvent <= lChannelDataUBound; lChanEvent++)
							{
								// Get the data point out and store in pData [Index]
								DATE dtEventTime = 0;
								int nSealState = 0;
								SA_GetElem (vChannelSafeArray.parray,lChanEvent,0,&dtEventTime);
								SA_GetElem (vChannelSafeArray.parray,lChanEvent,1,(int*)&nSealState);
								if (Index < *pulNumPts)
								{
									pData[Index].dJulianTime = dtEventTime;
									pData[Index].eossPt.nState = nSealState;
									pData[Index].eossPt.sChannelIndex = l+1;
									pData[Index].eossPt.sChannelID = sChanID;
								}
								else
								{
									//TODO: BAD, need to do something.
								}
								Index ++;
							}
						}
						if (true)//vChannelSafeArray.parray)
						{
							//SafeArrayDestroy (vChannelSafeArray.parray);
							VariantClear (&vChannelSafeArray);
						}
					}
				}
			}	
		}
		if (true)//vStationData.parray)
		{
			//SafeArrayDestroy (vStationData.parray);
			VariantClear (&vStationData);
		}

	}
	*pulNumPts = Index;
	VariantClear (&vStationData);
	return (vbStatus!=VARIANT_FALSE?1:0);
}

int  CEOSSImport::GetDataByStation(
		const short sFacID, 
		const short sStaID, 
		const DATE dStart, 
		const DATE dEnd, 
		struct WHOLE_DOUBLE_DATA_PT1 *pData, 
		unsigned long *pulNumPts, 
		DATE *pdNextTime)
{
	if (*pulNumPts == 0) 
		return(1);

	*pdNextTime = 0.0;
	
	VARIANT vStationData;
	VariantInit (&vStationData);
	
	// Call GetDataByStation
	VARIANT_BOOL vbStatus = pIEOSSImport->GetDataByStation (sFacID, (LONG)sStaID, dStart, dEnd, &vStationData);
	
	if (vbStatus !=VARIANT_FALSE)
	{
		//Check for valid SAFEARRAY, see how many channels of data we have
		if (vStationData.parray && vStationData.vt == (VT_VARIANT|VT_ARRAY))
		{
			long uBound,lBound = 0;
			SafeArrayGetLBound (vStationData.parray,1,&lBound);
			SafeArrayGetUBound (vStationData.parray,1,&uBound);
			
			unsigned long ArraySize = 0;
			
			// GetDataStatistics for each channel so that we can allocate the data array correctly
			for (long l = lBound; l <= uBound; l++)
			{
				DATE actualStart, actualEnd = 0;
				long ulNum;
				vbStatus = pIEOSSImport->GetDataStatistics (sFacID,(LONG)sStaID,l+1,dStart,dEnd,&actualStart,&actualEnd,&ulNum);		
				ArraySize += ulNum;
				ArraySize+=2;
			}
			
			//*pulNumPts = ArraySize;
			
			if (vbStatus != VARIANT_FALSE)
			{
				//Allocate Data array
				//pData = (struct WHOLE_DOUBLE_DATA_PT1 *)malloc((ArraySize) * sizeof(struct WHOLE_DOUBLE_DATA_PT1));
				int Index = 0;

				for (l = lBound; l <= uBound; l++)
				{
					VARIANT vChannelSafeArray;
					VariantInit (&vChannelSafeArray);
					long lDim[2];
					lDim[0] = l;
					lDim[1] = 1;
					//Grab each chunk of ChannelData
					HRESULT hr = SafeArrayGetElement (vStationData.parray,lDim,&vChannelSafeArray);
					if (!FAILED (hr))
					{
						if (vChannelSafeArray.parray)
						{
							long lChannelDataLBound, lChannelDataUBound = 0;
							SafeArrayGetLBound (vChannelSafeArray.parray,1,&lChannelDataLBound);
							SafeArrayGetUBound (vChannelSafeArray.parray,1,&lChannelDataUBound);
							short sChanID = 0;
							pglFacCfg->GetChannelIDFromIndex (sFacID,sStaID,l,&sChanID);
							for (long lChanEvent = lChannelDataLBound; lChanEvent <= lChannelDataUBound; lChanEvent++)
							{
								// Get the data point out and store in pData [Index]
								DATE dtEventTime = 0;
								int nSealState = 0;
								SA_GetElem (vChannelSafeArray.parray,lChanEvent,0,&dtEventTime);
								SA_GetElem (vChannelSafeArray.parray,lChanEvent,1,(int*)&nSealState);

								if (Index < (*pulNumPts))
								{
									pData[Index].dJulianTime = dtEventTime;
									pData[Index].eossPt.nState = nSealState;
									pData[Index].eossPt.sChannelIndex = l+1;
									pData[Index].eossPt.sChannelID = sChanID;
								}
								else
								{
									// TODO: This is not good
								}
								Index ++;
							}
						}
						//if (true);//vChannelSafeArray.parray)
						//{
							//SafeArrayDestroy (vChannelSafeArray.parray);
						//	VariantClear (&vChannelSafeArray);
						//}
					}
					VariantClear (&vChannelSafeArray);
				}  //end for...


			}	
		}
		if (true)//vStationData.parray)
		{
			//SafeArrayDestroy (vStationData.parray);
			VariantClear (&vStationData);
		}

	}

	VariantClear (&vStationData);
	return (vbStatus!=VARIANT_FALSE?1:0);
}

int CEOSSImport::ConvertChanIDToIndex(short sFacID, long lStationID, short sChanID)
{
	short sChanIndex;
	pglFacCfg->GetChannelIndexFromID (sFacID,lStationID,sChanID,&sChanIndex);
	return sChanIndex;
}

//HN 4-7-08 Add backup/restore options for EOSS

// 28-Oct-2004	SFK	Added sFacilityID arg to calling parms of function and COM call
int CEOSSImport::BackupDatabase(const short sFacilityID, const char *strPath, const bool bQuestion, const bool bPrompt)
{
	VARIANT_BOOL vb = VARIANT_FALSE;
	
	if (pIEOSSImport) 
		vb = pIEOSSImport->BackupDataStore(sFacilityID, strPath, bQuestion?VARIANT_TRUE:VARIANT_FALSE, bPrompt?VARIANT_TRUE:VARIANT_FALSE);

	return(vb != VARIANT_FALSE);
}

// 28-Oct-2004	SFK	Added sFacilityID arg to calling parms of function and COM call
int CEOSSImport::RestoreDatabase(const short sFacilityID, const char *strPath, const char *strDestPath, const bool bQuestion, const bool bPrompt)
{
	VARIANT_BOOL vb = VARIANT_FALSE;
	
	if (pIEOSSImport) 
		vb = pIEOSSImport->RestoreDataStore(sFacilityID, strPath, bQuestion?VARIANT_TRUE:VARIANT_FALSE, bPrompt?VARIANT_TRUE:VARIANT_FALSE);
	return((vb != VARIANT_FALSE)?1:0);
}