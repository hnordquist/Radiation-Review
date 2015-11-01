// TimeAlign.cpp: implementation of the CTimeAlign class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003-2006, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
// For TimeAlign, this means sending the TimeAlign interface a LONG station ID.
// No change internally in RAD will be done to change these from a short representation
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to wrap all calls to the TimeAlignCom object.
//
//	Functions:
//		OpenTimeAlignDatastore		
//		CTimeAlign::CTimeAlign
//		CTimeAlign::~CTimeAlign
//		CTimeAlign::OpenDatabase
//		CTimeAlign::InitializeDatabase
//		CTimeAlign::CloseDatabase
//		CTimeAlign::GetDbStartDay		
//		CTimeAlign::GetDbEndDay
//		CTimeAlign::GetDataStatistics
//		CTimeAlign::GetDataByChannel
//		CTimeAlign::GetDataByStation
//		CTimeAlign::GetDayStatistics
//		CTimeAlign::GetDataByDay
//		CTimeAlign::GetParameters
//		CTimeAlign::SetParameters
///////////////////////////////////////////////////////////////////////////
#include "RadBuild.h"
#include "ComDataStructures.h"
#include "TimeAlign.h"
#include "imports.h"
#include "MyDateTime.h"
#include "SafeArrayUtil.h"
#include "FacilityConfig.h"
#include "TypeDefinitions.h"
#include "RADError.h"

struct COM_Supported {
	int iTypeNum;
	BOOL bCOMMod;
};

const int NUM_TYPES_DEFINED = 9;
//#define NUM_TYPES_DEFINED 9
static struct COM_Supported gllTypes[NUM_TYPES_DEFINED] = 
{
	GRAND_TYPE, 1,  //1
	JSR_TYPE,   0,	//2
	ISR_TYPE,   0,	//4
	MCA_TYPE,	0,	//8
	OP_TYPE,	0,	//16
	JSR1_TYPE,	0,	//32
	BOT_TYPE,	1,	//64
	BINARY_TYPE,1,	//128
	EOSS_TYPE,	1	//512
};
// These will be read from irs.ini and then can be passed on to the COMs
extern bool gbEnableLogging;
extern int gnLoggingLevel;
extern bool gbQuietMode;

extern CMyDateTime glMyDateTime;

ITimeAlignPtr pITimeAlign;

struct TimeAlignParmsStruct *pParmsForGraphLabeling = NULL; // JFL cached info for use by graph labeling loop

///////////////////////////////////////////////////////////////////////////
//	Name:	OpenTimeAlignDatastore
//
//	Description:
//	Set up the active data store for the object
//	
//	Declaration:
//	int OpenTimeAlignDatastore(const char* strDataStorePath)
//
//	Input:	strDataStorePath	path where database to be used is located
//
//	Output:	none
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	28-Oct-2004	SFK		Changed to call with sFacilityID rather than directory path
//////////////////////////////////////////////////////////////////
int OpenTimeAlignDatastore(const short sFacilityID)
{
	if (!pglTimeAlign->OpenDatabase(sFacilityID))
		return(0);

	return (1);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeAlign::CTimeAlign(const char* szDefaultPath, short sFacID)
{
	strncpy(mstrDefaultPath, szDefaultPath, 256);
	pITimeAlign.CreateInstance(CLSID_TimeAlign);

	VARIANT TimeAlignVarStruct;
	VariantInit(&TimeAlignVarStruct);

	//Create VARIANT vector array here since we are filling all the values on this side
	TimeAlignVarStruct.vt = VT_VARIANT | VT_ARRAY; 
	SAFEARRAYBOUND Dim[2];
	//Dim[0].cElements = sNum;		// number of rows in the array
	Dim[0].cElements = 1;		// number of rows in the array
	Dim[0].lLbound = 0;		// index starts at 0
	Dim[1].cElements = 8;	// number of columns (number of field in a single record)
	Dim[1].lLbound = 0;
	TimeAlignVarStruct.parray = SafeArrayCreate(VT_VARIANT,2,Dim);
		
	ASSERT(!gbQuietMode); //Quiet mode always disabled for RAD
	
	// 10/24/07 HN Add the logging parms here.  Add a flag to show these are logging params.
	SA_PutElem (TimeAlignVarStruct.parray, 0, 0L, -1);// Flag value says next 3 are logging params
	SA_PutElem (TimeAlignVarStruct.parray, 0, 1L, gbEnableLogging);// Logging on?
	SA_PutElem (TimeAlignVarStruct.parray, 0, 3L, gnLoggingLevel);// logging level (from INI)
	SA_PutElem (TimeAlignVarStruct.parray, 0, 4L, gbQuietMode);//Quiet mode (disable always for RAD)
	SA_PutElem (TimeAlignVarStruct.parray, 0, 5L, 0);
	SA_PutElem (TimeAlignVarStruct.parray, 0, 6L, 0);
	SA_PutElem (TimeAlignVarStruct.parray, 0, 7L, 0);
	SA_PutElem (TimeAlignVarStruct.parray, 0, 8L, 0);
	
	if (pITimeAlign) 
		pITimeAlign->SetParameters(sFacID, &TimeAlignVarStruct);

	VariantClear(&TimeAlignVarStruct);
	if (pParmsForGraphLabeling)
		delete pParmsForGraphLabeling;
	pParmsForGraphLabeling = NULL;
}


CTimeAlign::~CTimeAlign()
{
	pITimeAlign = NULL;
	if (pParmsForGraphLabeling)
	{
		delete pParmsForGraphLabeling;
		pParmsForGraphLabeling = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////
//	Name:	OpenDatabase
//
//	Description:
//	Set parameters necessary in TimeAlignCom and opens the datastore to be used by TimeAlignCom.
//	
//	Declaration:
//	int CTimeAlign::OpenDatabase(const char* strDataStorePath)
//
//	Input:	strDataStorePath	path where database to be used by TimeAlignCom is located
//			strDefaultPath		path where any default parameters to be used by TimeAlignCom are located
//
//	Output:	none
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-2002	SFK		Created from GrandImport.cpp
//	26-Oct-2004	SFK		Removed calls to SetDataStore and SetDefaultPath to conform to Import interface.
//						Also send in FacilityID rather than path
//////////////////////////////////////////////////////////////////
bool CTimeAlign::OpenDatabase(const short sFacilityID)
{
	VARIANT_BOOL vb = VARIANT_FALSE;
	
	// do the special actions required to get the data store ready to use
	if (pITimeAlign) 
		vb = pITimeAlign->OpenDataStore(sFacilityID);

	if (vb)
		return true;
	else
		return false;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	InitializeDatabase
//
//	Description:
//	Wraps TimeAlignCom InitializeDatabase method.
//	
//	Declaration:
//	int CTimeAlign::InitializeDatabase(const char* strDatabasePath)
//
//	Input:	strDatabasePath		path where db to be initialized is located.
//
//	Output:	none
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	26-Oct-2004	SFK		Changed to call with FacilityID
//////////////////////////////////////////////////////////////////
int CTimeAlign::InitializeDatabase(const short sFacilityID)
{
	int iStatus;
	
	if (pITimeAlign) iStatus = pITimeAlign->InitializeDataStore(sFacilityID, false);

	return(iStatus);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	CloseDatabase
//
//	Description:
//	Wraps TimeAlignCom CloseDataStore method.
//	
//	Declaration:
//	void CTimeAlign::CloseDatabase()
//
//	Input:	none
//
//	Output:	none
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-2002	SFK		Created from GrandImport.cpp
//////////////////////////////////////////////////////////////////
void CTimeAlign::CloseDatabase()
{
	if (pITimeAlign) pITimeAlign->CloseDataStore();
	if (pParmsForGraphLabeling)
		delete pParmsForGraphLabeling;
	pParmsForGraphLabeling = NULL;
}

// Hardwire this for now
BOOL CTimeAlign::COMDataType(short sType)
{
	if (sType == -1)		// -1 signals all types
		return (TRUE);

	for (int i=0; i<NUM_TYPES_DEFINED; i++) 
	{
		if ((sType == gllTypes[i].iTypeNum) && (gllTypes[i].bCOMMod == 1))
			return(TRUE);
	}
	return(FALSE);
}

///////////////////////////////////////////////////////////////////////////
//	Name:	GetDbStartDay
//
//	Description:
//	Wraps TimeAlignCom GetDataStoreTimeRange method and extracts the start day in the db.
//		Temporary patch calls different COMs from here -- this moves to TimeAlign Com
//	
//	Declaration:
//	int CTimeAlign::GetDbStartDay(const short sFacID, const short sStaID, DATE* pdStartDay, const bool bErrDisplay, short sStaType)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			bErrDisplay	whether to display message if no data found in db
//			sStaType	Station type
//
//	Output:	pdStartDay	aligned DATE of first day in the db
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-2002	SFK		Created from GrandImport.cpp
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	20-Apr-2002 SFK		Received DATE data from COMs
//	13-Aug-2002 SFK		Return DATE to rest of Rad Review
//////////////////////////////////////////////////////////////////
int CTimeAlign::GetDbStartDay(
	const short sFacID, 
	const short sStaID, 
	DATE* pdStartDay, 
	const bool bErrDisplay, 
	short sStaType)
{
	*pdStartDay = 0.0;

	if (pITimeAlign == NULL)
		return 0;

	DATE dEndDate;

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID
	VARIANT_BOOL vb = 
		pITimeAlign->GetDataStoreTimeRange(
		sFacID, (LONG) sStaID, sStaType, bErrDisplay, pdStartDay, &dEndDate);

	if (vb)
		return 1;
	else
		return 0;
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetDbEndDay
//
//	Description:
//	Wraps TimeAlignCom GetDataStoreTimeRange method and extracts the end day in the db.
//		Temporary patch calls different COMs from here -- this moves to TimeAlign Com
//	
//	Declaration:
//	int CTimeAlign::GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay, short sStaType)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			bErrDisplay	whether to display message if no data found in db
//			sStaType	Station Type
//
//	Output:	pdEndDay	aligned julian time of last day in the db
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-2002	SFK		Created from GrandImport.cpp
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	20-Apr-2002 SFK		Received DATE data from COMs
//	13-Aug-2002 SFK		Returned DATE to rest of Rad Review
//////////////////////////////////////////////////////////////////
int CTimeAlign::GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay, short sStaType)
{
	*pdEndDay = 0.0;

	if (pITimeAlign == NULL)
		return 0;

	DATE dStartDate;
	VARIANT_BOOL vb = VARIANT_FALSE;
 
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID
	vb = pITimeAlign->GetDataStoreTimeRange(sFacID, (LONG)sStaID, sStaType, bErrDisplay, &dStartDate, pdEndDay);

	if(vb)
		return 1;
	else
		return 0;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStatistics
//
//	Description:
//	Wraps TimeAlignCom GetDataStatistics method.
//	
//	Declaration:
//	int CTimeAlign::GetDataStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdStart, DATE *pdEnd, unsigned long *pulNumPts, short sStaType)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			ulStart		julian time for start of search
//			ulEnd		julian time for end of search
//			sStaType	Station type
//
//	Output:	pulStart	aligned julian time of first data point in the interval
//			pulEnd		aligned julian time of last data point in the interval
//			pulNumPts	number of data points between and including pulStart to pulEnd
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-2002	SFK		Created from GrandImport.cpp
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	20-Apr-2002 SFK		Received DATE data from COMs
//	05-Jun-2992 SFK		Received DATE in call from Rad
//////////////////////////////////////////////////////////////////
BOOL CTimeAlign::GetDataStatistics(
	const short sFacID, 
	const short sStaID, 
	const short sChanID, 
	const DATE dStart, 
	const DATE dEnd, 
	DATE *pdStart, 
	DATE *pdEnd, 
	unsigned long *pulNumPts, 
	short sStaType)
{
	DATE dRealStart, dRealEnd;
	long lNumPts;
	VARIANT_BOOL vbStatus = VARIANT_FALSE;

	*pdStart = 0.0;
	*pdEnd = 0.0;
	*pulNumPts = 0;

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID
	if (pITimeAlign)
	{
		vbStatus = pITimeAlign->GetDataStatistics(
			sFacID, (LONG)sStaID, sChanID, sStaType, 
			dStart, dEnd, 
			&dRealStart, 
			&dRealEnd, 
			&lNumPts);
	}
	
	if (vbStatus) 
	{
		*pdStart = dRealStart;
		*pdEnd = dRealEnd;
		*pulNumPts = lNumPts;
	}
	else
	{
		// JFL for BMEND-1
		// the real issue here is the GDS failure state does not propagate out of the ITimeAlign interface. 
		if ((dRealStart == 0.0) && (dRealStart == 0.0) && (lNumPts == 0)) // ITimeAlign COM set these to zero and returned a false for one of many possible reasons.
		{
			return uiDAY_NOT_IN_DB_STATUS; // return status code expected by caller
		}
	}
	return (vbStatus != VARIANT_FALSE ? TRUE: FALSE);

}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByChannel
//
//	Description:
//	Wraps TimeAlignCom GetDataByChannel method.
//	
//	Declaration:
//	int CTimeAlign::GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime, short sStaType)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			iChanID		Channel number at station (must be 0-6)
//			ulStart		julian time for start of search
//			ulEnd		julian time for end of search
//			sStaType	type of station
//
//	Output:	pdX			array containing julian times of all data in interval including endpoints
//			pdY			array containing data values of all data in interval including endpoints
//			pulNumPts	number of entries in the pdX or pdY array
//			pulNextTime	julian time of the next possible data point following the endpoint
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-2002	SFK		Created from GrandImport.cpp
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	20-Apr-2002 SFK		Received DATE data from COMs
//////////////////////////////////////////////////////////////////
int CTimeAlign::GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime, short sStaType)
{
	VARIANT vData;
	VariantInit(&vData);

	long lIndex;

	*pdNextTime = 0.0;
	if (*pulNumPts == 0) 
		return(1);

	vData.vt = VT_I8;
	vData.lVal = *pulNumPts;
	VARIANT_BOOL vb = VARIANT_FALSE;
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID
	if (pITimeAlign) 
		vb = pITimeAlign->GetDataByChannel(sFacID, (LONG)sStaID, sStaType, sChanID, dStart, dEnd, &vData);

	// Extract data from the SAFEARRAY and return to Rad as in previous calls
	if ((vb && (vData.vt == (VT_VARIANT | VT_ARRAY)))) 
	{
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vData.parray, 1, &lUBound);
		//pjm 25-Jan-2006 Added explicit cast to eliminate compiler warning.
		if (lUBound > (long)*pulNumPts) {		// got back more points than expected only return first *pulNumPts
			lUBound = (long)*pulNumPts - 1;
		}
		for (lIndex=0; lIndex<lUBound; lIndex++) {  // JFL < creates a last point drop here
			SA_GetElem(vData.parray, lIndex, 0, &(pdX[lIndex]));		// timestamp in column 1
			SA_GetElem(vData.parray, lIndex, 1, &(pdY[lIndex]));		// data in column 2
		}
		*pulNumPts = lIndex;									// number of points returned JFL (?) cannot fix this here because calling code eventually corrects errors introduced here and in GetDataByChannel
		SA_GetElem(vData.parray, lIndex, 0, pdNextTime);		// timestamp of next data point
	}

	VariantClear(&vData);
	BOOL bReturn = FALSE;
	if (vb != VARIANT_FALSE)
		bReturn = TRUE;

	return (bReturn);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByStation
//
//	Description:
//	Wraps TimeAlignCom GetDataByStation method
//	
//	Declaration:
//	int CTimeAlign::GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, double *pTimestamps, VARIANT *pStruct, unsigned long *pulNumPts, DATE *pdNextTime, short sStaType)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			ulStart		julian time of interval start to retrieve Grand data records
//			ulEnd		julian time of interval end to retrieve Grand data records
//			pulNumPts	number of data points expected in the range (know from GetDataStatistics)
//			pTimestamps	caller must allocate storage for pulNumPts timestamps records before this call
//			sStaType	type of station
//
//	Output:	pulNumPts	actual number of data points found in the interval
//			pTimestamps	adjusted timestamps filled in the array
//			pStruct		untouched VARIANT data array -- caller will extract information
//			pulNextTime	julian time of the next possible data record following this interval
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-2002	SFK		Created from GrandImport.cpp
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	20-Apr-2002 SFK		Received DATE data from COMs
//	05-Jun-2002 SFK		Changed to receive DATE from Rad Review
//	28-Oct-2004	SFK		Changed to receive all data in the SAFEARRAY.  Changed call in CTimeAlign to be compatible with
//						what is in the COMS?
//////////////////////////////////////////////////////////////////
int CTimeAlign::GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, VARIANT *pvData, short sStaType, unsigned long *pulNumPts)
{
	DATE dNextTime = 0.0;
	VARIANT_BOOL vb = VARIANT_FALSE;

	pvData->lVal = *pulNumPts;
	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID
	if (pITimeAlign) vb = pITimeAlign->GetDataByStation(sFacID, (LONG)sStaID, sStaType, dStart, dEnd, pvData);
	//	The instrument specific call extracts the data.

	if (vb)
		return 1;
	else
		return 0;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDayStatistics
//
//	Description:
//	Wraps TimeAlignCom GetDayStatistics TimeAlignCom method.
//	
//	Declaration:
//	int CTimeAlign::GetDayStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdRealStart, DATE *pdRealEnd, unsigned long *pulNumPts, short sStaType)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			ulStart		adjusted julian time of interval start to retrieve Grand data records
//			ulEnd		adjusted julian time of interval end to retrieve Grand data records
//			sStaType	type of station
//
//	Output:	pulNumPts	actual number of data points found in the interval
//			pulStart	adjusted julian time of first day in interval
//			pulEnd		adjusted julian time of last day in interval
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-2002	SFK		Created from GrandImport.cpp
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	18-Apr-2002 SFK		Received DATE data from COMs
//	08-Aug-2002 SFK		Use DATE with rest of Rad
//  22-Sep-2003 SFK		Modify for new FacMgr - type is in IDStruct
//////////////////////////////////////////////////////////////////
int CTimeAlign::GetDayStatistics(
	const short sFacID, 
	const struct IDStruct *pStaID, 
	const DATE dStart, 
	const DATE dEnd, 
	DATE *pdRealStart, 
	DATE *pdRealEnd, 
	unsigned long *pulNumPts)
{
	long lNumPts;
	VARIANT_BOOL bStatus = false;

	*pdRealStart = 0;
	*pdRealEnd = 0;
	*pulNumPts = 0;

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID

	if (pITimeAlign) 
		bStatus = pITimeAlign->GetDayStatistics(sFacID, (LONG)pStaID->sStaID, pStaID->sStaType, dStart, dEnd, pdRealStart, pdRealEnd, &lNumPts);
	if (bStatus) 
	{
		*pulNumPts = lNumPts;
	}
	return(bStatus);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByDay
//
//	Description:
//	Wraps TimeAlignCom GetDataByDay method.
//	
//	Declaration:
//	int CTimeAlign::GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays, short sStaType)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			ulStart		adjusted julian time of interval start to retrieve Grand data records
//			ulEnd		adjusted julian time of interval end to retrieve Grand data records
//			sStaType	type of station
//			pulNumDays	number of data points expected in the range (know from GetDayStatistics)number of data points expected in the range (know from GetDayStatistics)
//
//	Output:	pulNumDays	actual number of data points found in the interval
//			pDayStruct	pulNumPts of records filled in the array with adjusted times (caller must allocate storage)
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-2002	SFK		Created from GrandImport.cpp
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	20-Apr-2002 SFK		Received DATE data from COMs
//	08-Aug-2002 SFK		Use DATE with rest of Rad
//  22-Sep-2003 SFK		Modify for new FacMgr - type is in IDStruct
//	28-Oct-2004	SFK		Renamed COM call to GetDaySummaryData from GetDataByDay.
//						Also get pulNumDays value from number of rows in the SAFEARRAY
//////////////////////////////////////////////////////////////////
int  CTimeAlign::GetDataByDay(const short sFacID, const struct IDStruct *pStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays)
{
	VARIANT vDayData;
	VariantInit(&vDayData);

	long lIndex;
	VARIANT_BOOL bStatus = false;
	
	if (*pulNumDays == 0) 
		return(1);

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID
	if (pITimeAlign) bStatus = pITimeAlign->GetDaySummaryData(sFacID, (LONG)pStaID->sStaID, pStaID->sStaType, dStart, dEnd, &vDayData);

	// now pull the data back out and put into the pre-allocated structure expected by Rad Review
	if ((bStatus)  && (vDayData.vt == (VT_VARIANT | VT_ARRAY))) {	// is the type of SafeArray we are expecting 
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vDayData.parray, 1, &lUBound);	
		lUBound++;

		if (lUBound > (long)*pulNumDays)		// got back more points than expected only return first *pulNumPts
			lUBound = (long)(*pulNumDays - 1);  //fixed compiler warning

		double dTemp;
		for (lIndex=0; lIndex< lUBound; lIndex++) { // JFL < creates last point drop here
			SA_GetElem(vDayData.parray, lIndex, 0, &(pDayStruct[lIndex].dFirstDataPt));
			SA_GetElem(vDayData.parray, lIndex, 1, &(pDayStruct[lIndex].dLastDataPt));
			SA_GetElem(vDayData.parray, lIndex, 2, &dTemp);
			pDayStruct[lIndex].ulNumPtsInDay = (unsigned long)dTemp;
		}
		*pulNumDays = lUBound; // JFL but accidentally corrected here (?) cannot fix this here because calling code eventually corrects errors introduced here and in GetDataByChannel
	}
	VariantClear(&vDayData);
	return(bStatus);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetParameters
//
//	Description:
//	Wraps TimeAlignCom GetParameters method.
//	
//	Declaration:
//	bool CTimeAlign::GetParameters(const int sFacID,  const short sNum, struct TimeAlignParmsStruct *pTimeAlignParms)
//
//	Input:	sFacID		Facility number
//			sNum		Number of stations requested data for
//			pTimeAlignParms		structure containing the numbers of station want parameters for from the COM
//
//	Output:	pTimeAlignParms		structure containing the values read from the COM
//
//	Return:	1(okay)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-2002	SFK		Created from GrandImport.cpp
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	20-Apr-2002 SFK		Received DATE data from COMs
//	08-Dec-2003	SFK		Added Station Type to information
//	28-Oct-2004	SFK		Removed sNum from COM call
//////////////////////////////////////////////////////////////////
bool CTimeAlign::GetParameters(const int sFacID,  const long sNum, struct TimeAlignParmsStruct *pTimeAlignParms)
{

	if (pITimeAlign == NULL)
		return false;

	VARIANT_BOOL vbStatus = VARIANT_FALSE;
	DATE dTempDate;

	VARIANT TimeAlignVarStruct;
	VariantInit(&TimeAlignVarStruct);

	//Create VARIANT vector array here since we are filling in the station number on this side
	TimeAlignVarStruct.vt = VT_VARIANT | VT_ARRAY; 
	SAFEARRAYBOUND Dim[2];
	Dim[0].cElements = sNum;		// number of rows in the array
	Dim[0].lLbound = 0;		// index starts at 0
	Dim[1].cElements = 8;	// number of columns (number of field in a single record)
	Dim[1].lLbound = 0;
	TimeAlignVarStruct.parray = SafeArrayCreate(VT_VARIANT,2,Dim);

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID
	// put the station numbers wanted in the arry
	for (long i = 0; i<sNum; i++) 
	{
		SA_PutElem(TimeAlignVarStruct.parray, i, 0, (LONG)pTimeAlignParms[i].sStaNum);		// station number
		SA_PutElem(TimeAlignVarStruct.parray, i, 1, pTimeAlignParms[i].sStaType);		// station type

	}

	// get the parameters for these station numbers from Time Align 
	vbStatus = pITimeAlign->GetParameters(sFacID, &TimeAlignVarStruct);
	
	if ((VARIANT_FALSE != vbStatus) && (TimeAlignVarStruct.vt == (VT_VARIANT | VT_ARRAY))) {	// is the type of SafeArray we are expecting 
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(TimeAlignVarStruct.parray, 1, &lUBound);	
		if (lUBound > sNum) {		// got back more points than expected only return first *pulNumPts
			lUBound = sNum - 1;
		}

		for (long i = 0; i<=lUBound; i++)
		{
			// if got information successfully, put it back into the local structure for use by Rad
			// TimeAlignVarStruct contains data in this order:
			//	station number, event1 time, event2 time, slope, intercept, whether time adjust used

			SA_GetElem(TimeAlignVarStruct.parray, i, 2, &(dTempDate));							// event time 1
			//dTempDate = DateTime.DATETimestampToMyTimestamp(dTempDate);
			dTempDate = glMyDateTime.DATETimestampToMyTimestamp(dTempDate);
			pTimeAlignParms[i].ulEvent1Time = (unsigned long)dTempDate;
				
			SA_GetElem(TimeAlignVarStruct.parray, i, 3, &(dTempDate));							// event time 2
			//dTempDate = DateTime.DATETimestampToMyTimestamp(dTempDate);
			dTempDate = glMyDateTime.DATETimestampToMyTimestamp(dTempDate);
			pTimeAlignParms[i].ulEvent2Time = (unsigned long)dTempDate;
				
			SA_GetElem(TimeAlignVarStruct.parray, i, 4, &(pTimeAlignParms[i].dSlope));			// slope
				
			SA_GetElem(TimeAlignVarStruct.parray, i, 5, &(pTimeAlignParms[i].dIntercept));		// intercept
				
			SA_GetElem(TimeAlignVarStruct.parray, i, 6, &(pTimeAlignParms[i].bUseTimeAlign));	// time align used?
				
			SA_GetElem(TimeAlignVarStruct.parray, i, 7, &(pTimeAlignParms[i].bEnableStation));	// station enabled for time align?
			
		}
	}
	else 
	{
		// problem in getting stuff from TimeAlign Com, fill in harmless defaults
		pTimeAlignParms[i].ulEvent1Time = 0;
		pTimeAlignParms[i].ulEvent2Time = 0;
		pTimeAlignParms[i].dSlope = 1;
		pTimeAlignParms[i].dIntercept = 0;
		pTimeAlignParms[i].bUseTimeAlign = false;
		pTimeAlignParms[i].bEnableStation = false;
	}

	//SafeArrayDestroy(TimeAlignVarStruct.parray);
	VariantClear(&TimeAlignVarStruct);

	return(vbStatus != VARIANT_FALSE);
}


// is the station containing this channel using a timealign adjustment?
bool CTimeAlign::IsTimeAlignEnabled(const int iFacID,  const int uiPL_ChanIndex)
{
	static int iNumStas = -1;
// do a getparameters and see, return tru if TA is enabled for the station
//	return false;

	if (pParmsForGraphLabeling == NULL)
	{
		char **aszStaList = NULL;
		iNumStas = pglFacCfg->GetNumStations(iFacID);
		pParmsForGraphLabeling = new struct TimeAlignParmsStruct[iNumStas];
		int status;
		struct IDStruct StaID;
		memset(&StaID,0,sizeof(struct IDStruct));

		if (!pglFacCfg->IsFacilityDefined(iFacID)) 
			return(false);	// verify facility is okay
		int iLen;
		if (!pglFacCfg->CreateStationPicklist(iFacID, false, &aszStaList, &iLen)) 
			return(false);
		bool bOk = true;

		//	Allocate storage for all use station checkboxes that will be in the table
	//    pgllbUseStation = (BOOL *)malloc(glliNumStas*sizeof(BOOL));
	//    if (pgllbUseStation == NULL) bOk = false; 


		for (int i=0; i<iNumStas; i++) 
		{		// fill in station numbers before do request
			if (!pglFacCfg->GetStationIDForPicklistChoice(i, &StaID))
			{
				continue;
			}

			pParmsForGraphLabeling[i].sStaNum = StaID.sStaID;
			pParmsForGraphLabeling[i].sStaType = StaID.sStaType;
		}
		status = pglTimeAlign->GetParameters(iFacID, iNumStas, pParmsForGraphLabeling);
		pglFacCfg->DestroyPicklist(aszStaList);
	}
	bool result = false;

	int j =0;
	struct IDStruct ChanID;

	if (!pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID))
		return result;

	// determine which entry this is in our parameter array
	while (j<iNumStas)
	{
		if (pParmsForGraphLabeling[j].sStaNum == ChanID.sStaID)
		{
			result = (pParmsForGraphLabeling[j].bEnableStation &&  pParmsForGraphLabeling[j].bUseTimeAlign);
			break;
		}
		j++;
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////
//	Name:	SetParameters
//
//	Description:
//	Wraps TimeAlignCom SetParameters method.
//	
//	Declaration:
//	bool CTimeAlign::SetParameters(const int sFacID, short sNum, struct TimeAlignParmsStruct *pTimeAlignParms)
//
//	Input:	sFacID		Facility number
//			sNum		Number of stations sending data for
//			pTimeAlignParms		structure containing the parameters for the stations
//
//	Output:	none
//
//	Return:	1(okay)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	07-Mar-2002	SFK		Created from GrandImport.cpp
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	20-Apr-2002 SFK		Received DATE data from COMs
//	09-Dec-2003	SFK		Added Station Type information sent to COM
//	28-Oct-2004	SFK		Removed sNum from COM call
//////////////////////////////////////////////////////////////////
bool CTimeAlign::SetParameters(const int sFacID, short sNum, struct TimeAlignParmsStruct *pTimeAlignParms)
{
	VARIANT_BOOL vbStatus = VARIANT_FALSE;
	DATE dTempDate;

	VARIANT TimeAlignVarStruct;
	VariantInit(&TimeAlignVarStruct);

	//Create VARIANT vector array here since we are filling all the values on this side
	TimeAlignVarStruct.vt = VT_VARIANT | VT_ARRAY; 
	SAFEARRAYBOUND Dim[2];
	//Dim[0].cElements = sNum;		// number of rows in the array
	Dim[0].cElements = sNum+1;		// number of rows in the array
	Dim[0].lLbound = 0;		// index starts at 0
	Dim[1].cElements = 8;	// number of columns (number of field in a single record)
	Dim[1].lLbound = 0;
	TimeAlignVarStruct.parray = SafeArrayCreate(VT_VARIANT,2,Dim);
		
	// put our information into the variant array to send over to TimeAlign
	// TimeAlignVarStruct contains data in this order:
	//	station number, event1 time, event2 time, slope, intercept, whether time adjust used, whether station enabled
	bool bIsDate = true;
	for (long i = 0; i<sNum; i++) // JFL <= usage caused a garbage entry at the end of the file each write call
	{
		// Fill the station number into the VARIANT_ARRAY
		// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
		// cast to a long on the way into the TimeAlign interface for station ID

		SA_PutElem(TimeAlignVarStruct.parray, i, 0, (LONG)pTimeAlignParms[i].sStaNum);		// station number
		SA_PutElem(TimeAlignVarStruct.parray, i, 1, pTimeAlignParms[i].sStaType);		// station type
		//dTempDate = DateTime.MyTimestampToDATETimestamp(pTimeAlignParms[i].ulEvent1Time);
		dTempDate = glMyDateTime.MyTimestampToDATETimestamp(pTimeAlignParms[i].ulEvent1Time);
		SA_PutElem(TimeAlignVarStruct.parray, i, 2, dTempDate,bIsDate);							// time1 for event
		//dTempDate = DateTime.MyTimestampToDATETimestamp(pTimeAlignParms[i].ulEvent2Time);
		dTempDate = glMyDateTime.MyTimestampToDATETimestamp(pTimeAlignParms[i].ulEvent2Time);
		SA_PutElem(TimeAlignVarStruct.parray, i, 3, dTempDate,bIsDate);							// time2 for event
		SA_PutElem(TimeAlignVarStruct.parray, i, 4, pTimeAlignParms[i].dSlope);			// slope
		SA_PutElem(TimeAlignVarStruct.parray, i, 5, pTimeAlignParms[i].dIntercept);		// intercept
		SA_PutElem(TimeAlignVarStruct.parray, i, 6, pTimeAlignParms[i].bUseTimeAlign);	// use time align?
		SA_PutElem(TimeAlignVarStruct.parray, i, 7, pTimeAlignParms[i].bEnableStation);	// use for this station?

	}

	ASSERT(!gbQuietMode);

	// 10/24/07 HN Add the logging parms here.  Add a flag to show these are logging params.
	// JFL BMEND-144 this was writing into the array location 1 past the actual end
	SA_PutElem (TimeAlignVarStruct.parray, i, 0L, -1);// Flag value
	SA_PutElem (TimeAlignVarStruct.parray, i, 1L, gbEnableLogging);// Logging on?
	SA_PutElem (TimeAlignVarStruct.parray, i, 3L, gnLoggingLevel);// logging level (from INI)
	SA_PutElem (TimeAlignVarStruct.parray, i, 4L, gbQuietMode);// disable always for RAD
	SA_PutElem (TimeAlignVarStruct.parray, i, 5, 0);
	SA_PutElem (TimeAlignVarStruct.parray, i, 6, 0);
	SA_PutElem (TimeAlignVarStruct.parray, i, 7, 0);
	SA_PutElem (TimeAlignVarStruct.parray, i, 8, 0);
	
	if (pITimeAlign) 
		vbStatus = pITimeAlign->SetParameters(sFacID, &TimeAlignVarStruct);

	VariantClear(&TimeAlignVarStruct);

	return(VARIANT_FALSE != vbStatus);
}
